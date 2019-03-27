/*
 * peak_detector.c
 *
 *  Created on: Mar 27, 2019
 *      Author: isthatme
 */

#include "peak_detector.h"
#include "main.h"
#include "adc.h"


void peak_detector_init(void) {
	// Initialise GPIO
	GPIO_InitTypeDef gpio;
	gpio.Pin = PEAK_CLEAR_PIN;
	gpio.Mode = GPIO_MODE_OUTPUT_PP;
	gpio.Pull = GPIO_NOPULL;
	gpio.Speed = GPIO_SPEED_FREQ_MEDIUM;

	HAL_GPIO_Init(PEAK_CLEAR_PIN_PORT, &gpio);
	HAL_GPIO_WritePin(PEAK_CLEAR_PIN_PORT, PEAK_CLEAR_PIN, 1); // Turn BJT off

	// set up adc
	adc_peak_setup();


	// Set initial peak detector settings
	peak_detector_settings_s.valid_start_time = PEAK_VALID_START_TIME;
	peak_detector_settings_s.valid_end_time = PEAK_VALID_END_TIME;
	peak_detector_settings_s.noise_threshold = PEAK_NOISE_THRESHOLD;
}

void peak_detector_reset(void) {

}

void peak_detector_low(void) {

}

ping_status_t peak_get_ping_status(uint16_t* peak_level) {
	float good_level = 2 * peak_detector_settings_s.noise_threshold;
	static uint16_t valid_counter = 0;
	static uint16_t ping_offset_time = HAL_GetTick();

	// Read ADC
	peak_level = adc_peak_read();
	// Scale from 12-bit range to -> 0-3v3
	float peak_input = (peak_level / (2^12 - 1) ) * 3.3;

	// If The signal is below the noise floor
	if (peak_level <= good_level) { // spooky
		HAL_GPIO_WritePin(LED_PORT, LED_PIN, 0); // Turn off ping LED
		return PING_INVALID;
	}

	// if the signal is above the noise floor
	// If the first valid signal
	if (valid_counter == 0) {
		// Set elapsed time to 0
		ping_offset_time = HAL_GetTick();

		valid_counter = 1;
	}

	// if we have had previous valid signals
	if (valid_counter > 0) {
		uint16_t ping_offset_delta = HAL_GetTick() - ping_offset_time;

		// If we are before the valid ping start time
		if (ping_offset_delta < peak_detector_settings_s.valid_start_time) {
			HAL_GPIO_WritePin(LED_PORT, LED_PIN, 0); // Turn off ping LED
			// Koltin's old code returned a 3 here, but there are no checks for 3, so I'm leaving it like this
			return PING_INVALID;

		// If we are within the valid times
		} else if ((ping_offset_delta >= peak_detector_settings_s.valid_start_time)
				&& (ping_offset_delta < peak_detector_settings_s.valid_end_time)) {
			HAL_GPIO_WritePin(LED_PORT, LED_PIN, 1); // Turn on ping LED
			return PING_VALID;

		// Past valid times
		} else {
			HAL_GPIO_WritePin(LED_PORT, LED_PIN, 0); // Turn off ping LED
			valid_counter = 0;
			return PING_OVER;
		}
	}

	// Actually handled otherwise but this is just to get rid of warnings
	return PING_INVALID;
}
