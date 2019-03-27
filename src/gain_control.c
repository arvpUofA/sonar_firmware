/*
 * gain_control.c
 *
 *  Created on: Mar 27, 2019
 *      Author: isthatme
 */


#include <stdbool.h>
#include "gain_control.h"
#include "peak_detector.h"
#include "main.h"
#include "adc.h"
#include "amplifier.h"

void gain_control_init(void) {
	peak_detector_init();
	amplifier_init();

}

// TODO maybe break up into smaller, more manageable functions
void gain_control_run(void) {
	// Values
	uint16_t peak_level;
	static uint64_t average_peak_level_sum = 0;
	static uint16_t average_peak_level_counter = 0;
	ping_status_t ping_status;
	static ping_status_t previous_ping_status = 0;
	static float optimal_gain = 0;

	// Timers
	static uint16_t nudge_timer = HAL_GetTick();
	static uint16_t floor_gain_timer = HAL_GetTick();
	static uint16_t led_timer = HAL_GetTick();

	// Flags
	static bool gain_floored_flag = false;
	static bool hold_gain_flag = false;
	static bool led_set_flag = false;

	// If the gain isn't floored, get the ping status
	if (!gain_floored_flag) {
		ping_status = peak_get_ping_status(&peak_level);
	}

	// If gain is not floored, run our controller
	if ((ping_status == PING_VALID) && !gain_floored_flag) {
		if (!hold_gain_flag) {
			 optimal_gain = gain_controller(peak_level);
			 amplifier_set_gain(optimal_gain);
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
			if ((HAL_GetTick() - nudge_timer) > gain_control_settings_s.nudge_gain_duration) {
				// are we holding gain?
				if (!hold_gain_flag) {
					// Nudge value up
					optimal_gain += gain_control_settings_s.nudge_gain_value;
				}

				// Set gain
				amplifier_set_gain(optimal_gain);

				// Reset the nudge timer
				nudge_timer = HAL_GetTick();
			}
		}
	}

	// Don't floor the gain for too long
	if (gain_floored_flag) {
		if ((HAL_GetTick() - floor_gain_timer) > gain_control_settings_s.floor_gain_duration) {
			// lower flag
			gain_floored_flag = false;

			// Apply calculated optimal gain
			amplifier_set_gain(optimal_gain);
		}
	}

	// Reset peak detector if ping is invalid
	if ((ping_status == PING_INVALID) || (ping_status == PING_OVER)) {
			peak_detector_reset();
	}

	// Start blinking LED if ping is valid
	if ((previous_ping_status == PING_INVALID) && (ping_status == PING_VALID)
			&& (!led_set_flag)) {
		HAL_GPIO_WritePin(LED_PORT, LED_PIN, 1); // turn on LED
		led_set_flag = true;

		// Reset LED timer
		led_timer = HAL_GetTick();
	}

	// Turn off LED if more than 200ms has elapsed
	if (led_set_flag) {
		if (HAL_GetTick() - led_timer > 200) {
			led_set_flag = false;
			HAL_GPIO_WritePin(LED_PORT, LED_PIN, 0); // Turn off LED
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
static float gain_controller(uint16_t peak_level,
		uint64_t* p_average_peak_level_sum, uint16_t* p_average_peak_level_counter) {
	int16_t error; // Can be 16 bits because ADC is 12 bit
	static int16_t sum_error = 0;
	float gain = 0;

	// Max peak level to accept
	error = gain_control_settings_s.desired_peak - peak_level;

	// Summing peak level so we can average
	*p_average_peak_level_sum += peak_level;
	(*p_average_peak_level_counter)++;

	// Add to the integral error
	sum_error += error;

	// Saturate error
	if (sum_error > gain_control_settings_s.integral.error_max) {
		sum_error = gain_control_settings_s.integral.error_max;
	} else if (sum_error < gain_control_settings_s.integral.error_min) {
		sum_error = gain_control_settings_s.integral.error_max;
	}

	// Apply gains
	gain += gain_control_settings_s.proportional.gain * error;
	gain += gain_control_settings_s.integral.gain * sum_error;

	return gain;
}
