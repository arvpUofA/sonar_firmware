/*
 * gain_control.c
 *
 *  Created on: Mar 27, 2019
 *      Author: isthatme
 */


#include <stdbool.h>
#include "gain_control.h"
#include "constants.h"
#include "peak_detector.h"
#include "main.h"
#include "adc.h"
#include "amplifier.h"


gain_control_t gain_control_s;

void gain_control_init(void) {
	peak_detector_init();
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
}


static float gain_controller(uint64_t* p_average_peak_level_sum,
			uint16_t* p_average_peak_level_counter);

// TODO maybe break up into smaller, more manageable functions
void gain_control_run(void) {
	// Values
	static uint64_t average_peak_level_sum = 0;
	static uint16_t average_peak_level_counter = 0;
	ping_status_t ping_status;
	static ping_status_t previous_ping_status = PING_INVALID;

	// Timers
	static uint16_t nudge_timer = 0;
	static uint16_t floor_gain_timer = 0;
	static uint16_t led_timer = 0;

	// Flags
	static bool gain_floored_flag = false;
	static bool hold_gain_flag = false;
	static bool led_set_flag = false;

	// If the gain isn't floored, get the ping status
	if (!gain_floored_flag) {
		ping_status = peak_get_ping_status(&gain_control_s.peak_level);
	}

	// If gain is not floored, run our controller
	if ((ping_status == PING_VALID) && !gain_floored_flag) {
		if (!hold_gain_flag) {
			 gain_control_s.optimal_gain = gain_controller(&average_peak_level_sum, &average_peak_level_counter);
			 amplifier_set_gain(gain_control_s.optimal_gain);
		}

		nudge_timer = HAL_GetTick();
	}

	// Ping is over, floor gain
	if (ping_status == PING_OVER) {
		if (!gain_floored_flag) {
			amplifier_set_gain(0); // Floor gain at 0dB

			// Reset timers
			floor_gain_timer = HAL_GetTick();
			nudge_timer = HAL_GetTick();

			// Gain is now floored, set flag
			gain_floored_flag = true;
		}
	}

	// If ping has been invalid for a certain time, and gain is not floored
	// nudge gain up slightly
	if (!gain_floored_flag) {
		if (ping_status == 0) {
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
		}
	}

	// Don't floor the gain for too long
	if (gain_floored_flag) {
		if ((HAL_GetTick() - floor_gain_timer) > gain_control_s.floor_gain_duration) {
			// lower flag
			gain_floored_flag = false;

			// Apply calculated optimal gain
			amplifier_set_gain(gain_control_s.optimal_gain);
		}
	}

	// Reset peak detector if ping is invalid
	if ((ping_status == PING_INVALID) || (ping_status == PING_OVER)) {
			peak_detector_reset();
	}

	// Start blinking LED if ping is valid
	if ((previous_ping_status == PING_INVALID) && (ping_status == PING_VALID)
			&& (!led_set_flag)) {
		HAL_GPIO_WritePin(LED_PORT, LED_PIN, GPIO_PIN_SET); // turn on LED
		led_set_flag = true;

		// Reset LED timer
		led_timer = HAL_GetTick();
	}

	// Turn off LED if more than 200ms has elapsed
	if (led_set_flag) {
		if (HAL_GetTick() - led_timer > 200) {
			led_set_flag = false;
			HAL_GPIO_WritePin(LED_PORT, LED_PIN, GPIO_PIN_RESET); // Turn off LED
		}
	}

	// TODO implement serial printing of ping info once it's over
	if ((ping_status == PING_OVER) && (previous_ping_status == PING_INVALID)) {

	}

	previous_ping_status = ping_status;
}

/** @brief Runs gain control (PID or psuedo-PID) on peak level
 *
 */
static float gain_controller(uint64_t* p_average_peak_level_sum,
		uint16_t* p_average_peak_level_counter) {
	int16_t error; // Can be 16 bits because ADC is 12 bit
	static int16_t sum_error = 0;
	float gain = 0;

	// Max peak level to accept
	error = gain_control_s.desired_peak - gain_control_s.peak_level;

	// Summing peak level so we can average
	*p_average_peak_level_sum += gain_control_s.peak_level;
	(*p_average_peak_level_counter)++;

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
