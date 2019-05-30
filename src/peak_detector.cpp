/*
 * peak_detector.c
 *
 *  Created on: Mar 27, 2019
 *      Author: isthatme
 */

#include "peak_detector.h"
#include "main.h"

peak_detector_t peak_detector_s;

#define GOOD_LEVEL 2 * PEAK_NOISE

void peak_detector_init(MCP3021* adc) {
	// Initialise GPIO
	GPIO_InitTypeDef gpio;
	gpio.Pin = PEAK_CLEAR_PIN;
	gpio.Mode = GPIO_MODE_OUTPUT_PP;
	gpio.Pull = GPIO_NOPULL;
	gpio.Speed = GPIO_SPEED_FREQ_MEDIUM;

	HAL_GPIO_Init(PEAK_CLEAR_PIN_PORT, &gpio);
	HAL_GPIO_WritePin(PEAK_CLEAR_PIN_PORT, PEAK_CLEAR_PIN, GPIO_PIN_SET); // Turn BJT off

	peak_detector_s.adc = adc;

	// Set initial peak detector settings
	peak_detector_s.valid_start_time = PINGVALIDSTART;
	peak_detector_s.valid_end_time = PINGVALIDEND;
	peak_detector_s.noise_threshold = PEAK_NOISE;
}

void peak_detector_reset(void) {

}

void peak_detector_low(void) {

}

ping_status_t peak_get_ping_status(float* peak_level) {
	static uint16_t valid_counter = 0;
	static uint16_t ping_offset_time = 0;

	// Read ADC
	*peak_level = peak_detector_s.adc->single_conversion();
	// Scale from 12-bit range to -> 0-3v3
	float peak_input = (*peak_level / ((2^12) - 1) ) * 3.3;

	// If The signal is below the noise floor
	if (peak_input <= GOOD_LEVEL) { // spooky
		HAL_GPIO_WritePin(LED_PORT, LED_PIN, GPIO_PIN_RESET); // Turn off ping LED
		return PING_INVALID;
	}

	// if the signal is above the noise floor
	// If the first valid signal
	if (valid_counter == 0) {
		// Set elapsed time to 0
		ping_offset_time = usec_timer_read();

		valid_counter = 1;

		dma_start_xfer(); // Start transfer
	}

	// if we have had previous valid signals
	if (valid_counter > 0) {
		uint16_t ping_offset_delta = usec_timer_read() - ping_offset_time;

		// If we are before the valid ping start time
		if (ping_offset_delta < PINGVALIDSTART) {
			HAL_GPIO_WritePin(LED_PORT, LED_PIN, GPIO_PIN_RESET); // Turn off ping LED
			// Koltin's old code returned a 3 here, but there are no checks for 3, so I'm leaving it like this
			return PING_INVALID;

		// If we are within the valid times
		} else if ((ping_offset_delta >= PINGVALIDSTART)
				&& (ping_offset_delta < PINGVALIDEND)) {
			HAL_GPIO_WritePin(LED_PORT, LED_PIN, GPIO_PIN_SET); // Turn on ping LED
			return PING_VALID;

		// Past valid times
		} else {
			HAL_GPIO_WritePin(LED_PORT, LED_PIN, GPIO_PIN_RESET); // Turn off ping LED
			valid_counter = 0;
			return PING_OVER;
		}
	}

	// Actually handled otherwise but this is just to get rid of warnings
	return PING_INVALID;
}
