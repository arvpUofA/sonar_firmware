/*
 * gain_control.c
 *
 *  Created on: Mar 27, 2019
 *      Author: isthatme
 */


#include <stdbool.h>
#include "main.h"


gain_control_t gain_control_s;

void gain_control_init(MCP3021* adc) {
	peak_detector_init(adc);
	amplifier_init();

	// Set constants
	gain_control_s.desired_peak = DESIRED_PEAK;
	gain_control_s.proportional.gain = P_GAIN;
	gain_control_s.integral.gain = I_GAIN;
	gain_control_s.integral.error_max = MAX_I_ERROR;
	gain_control_s.integral.error_min = -MAX_I_ERROR;
	gain_control_s.floor_gain_duration = FLOOR_GAIN_DURATION;
	gain_control_s.nudge_gain_duration = NUDGE_GAIN_DURATION;
	gain_control_s.invalid_ping_duration = INVALID_PING_DURATION;
	gain_control_s.nudge_gain_value = NUDGE_VALUE;
	gain_control_s.valid_mean = DEFAULT_VALIDMEAN;
	gain_control_s.valid_variance = DEFAULT_VALIDVARIANCE;

	gain_control_s.average_peak_level_sum = 0;
	gain_control_s.average_peak_level_counter = 0;
	gain_control_s.average_peak_level_squared_sum = 0;
	gain_control_s.average_peak_level_squared_counter = 0;
}


static float gain_controller();

// TODO maybe break up into smaller, more manageable functions
void gain_control_run(void) {
	// Values
	ping_status_t ping_status = PING_INVALID;
	static ping_status_t previous_ping_status = PING_INVALID;

	// Timers
	static uint16_t nudge_timer = 0;
	static uint16_t floor_gain_timer = 0;
	static uint16_t led_timer = 0;

	// Flags
	static bool gain_floored_flag = false;
	static bool hold_gain_flag = false;
	static bool led_set_flag = false;

	if (!gain_floored_flag) {
		
		// If the gain isn't floored, get the ping status
		ping_status = peak_get_ping_status(&gain_control_s.peak_level);
		
		switch(ping_status) {
			
			// If ping has been invalid for a certain time, and gain is not floored
			// nudge gain up slightly
			case PING_INVALID:
				// Check elapsed time
				if ((HAL_GetTick() - nudge_timer) > gain_control_s.nudge_gain_duration) {
					// are we holding gain?
					if (!hold_gain_flag) {
						// Nudge value up
						gain_control_s.optimal_gain += gain_control_s.nudge_gain_value;
					}

					// Set gain
					amplifier_set_gain(gain_control_s.optimal_gain);

					// Reset the nudge timer
					nudge_timer = HAL_GetTick();
				}
				// Reset peak detector if ping is invalid
				peak_detector_reset();

			// If gain is not floored, run our controller
			case PING_VALID:
				if (!hold_gain_flag) {
					gain_control_s.optimal_gain = gain_controller();
					amplifier_set_gain(gain_control_s.optimal_gain);
				}

				nudge_timer = HAL_GetTick();
				
				// Start blinking LED if ping is valid
				if ((previous_ping_status == PING_INVALID) && (!led_set_flag)) {
					HAL_GPIO_WritePin(LED_PORT, LED_PIN, GPIO_PIN_SET); // turn on LED
					led_set_flag = true;

					// Ping is valid, so we should start the transfer
					dma_start_xfer();

					// Reset LED timer
					led_timer = HAL_GetTick();
				}
				break;
			
			// Ping is over, floor gain	
			case PING_OVER:	
				amplifier_set_gain(0); // Floor gain at 0dB

				// Reset timers
				floor_gain_timer = HAL_GetTick();
				nudge_timer = HAL_GetTick();

				// Gain is now floored, set flag
				gain_floored_flag = true;
				
				peak_detector_reset();

				// TODO implement serial printing of ping info once it's over
				// AsK
				if (previous_ping_status == PING_INVALID) {
					// use uart_send or something you define on top to write out the debug info
				}
				break;
		}
	}
	else {
		// Don't floor the gain for too long
		if ((HAL_GetTick() - floor_gain_timer) > gain_control_s.floor_gain_duration) {
			// lower flag
			gain_floored_flag = false;

			// Apply calculated optimal gain
			amplifier_set_gain(gain_control_s.optimal_gain);
		}
	}

	previous_ping_status = ping_status;

	// Turn off LED if more than 200ms has elapsed
	if (led_set_flag) {
		if (HAL_GetTick() - led_timer > 200) {
			led_set_flag = false;
			HAL_GPIO_WritePin(LED_PORT, LED_PIN, GPIO_PIN_RESET); // Turn off LED
		}
	}
}

/** @brief Runs gain control (PID or psuedo-PID) on peak level
 *
 */
static float gain_controller() {
	int16_t error; // Can be 16 bits because ADC is 12 bit
	static int16_t sum_error = 0;
	float gain = 0;

	// Max peak level to accept
	error = gain_control_s.desired_peak - gain_control_s.peak_level;

	// Summing peak level so we can average
	gain_control_s.average_peak_level_sum += gain_control_s.peak_level;
	gain_control_s.average_peak_level_counter++;

	// sum average peak squared
	gain_control_s.average_peak_level_squared_sum += 
			gain_control_s.peak_level * gain_control_s.peak_level;
	gain_control_s.average_peak_level_squared_counter++;

	// Add to the integral error
	sum_error += error;

	// Saturate error
	if (sum_error > gain_control_s.integral.error_max) {
		sum_error = gain_control_s.integral.error_max;
	} else if (sum_error < gain_control_s.integral.error_min) {
		sum_error = gain_control_s.integral.error_max;
	}

	// Apply gains
	gain += gain_control_s.proportional.gain * error;
	gain += gain_control_s.integral.gain * sum_error;

	return gain;
}


bool gain_control_check_calibration(void) {
	float average_peak_level;
	float average_peak_level_squared;
	if (gain_control_s.average_peak_level_counter != 0) {
		average_peak_level =
				gain_control_s.average_peak_level_sum / gain_control_s.average_peak_level_counter;
	} else {
		average_peak_level = gain_control_s.peak_level;
	}

	if (gain_control_s.average_peak_level_squared_counter != 0) {
		average_peak_level_squared = 
				gain_control_s.average_peak_level_squared_sum / gain_control_s.average_peak_level_squared_counter;
	} else {
		average_peak_level_squared = gain_control_s.peak_level * gain_control_s.peak_level;
	}

	float variance = average_peak_level_squared - (average_peak_level * average_peak_level);

	if ((average_peak_level > (gain_control_s.desired_peak - gain_control_s.valid_mean)) &&
		(average_peak_level < (gain_control_s.desired_peak + gain_control_s.valid_mean)) &&
		(variance < gain_control_s.valid_variance) &&
		(variance > (-1 * gain_control_s.valid_variance))) {
			if ((variance > 0.0001) || (variance < -0.0001)) {
				return true;
			} else {
				return false;
			}
	} else {
		return false;
	}
}