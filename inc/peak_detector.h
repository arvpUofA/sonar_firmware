/*
 * peak_detector.h
 *
 *  Created on: Mar 27, 2019
 *      Author: isthatme
 */

#ifndef PEAK_DETECTOR_H_
#define PEAK_DETECTOR_H_

#include "stm32f3xx.h"

// TODO match these to hardware
#define PEAK_CLEAR_PIN 		GPIO_PIN_11
#define PEAK_CLEAR_PIN_PORT	GPIOA

#define PEAK_VALID_START_TIME	200
#define PEAK_VALID_END_TIME		1500
#define PEAK_NOISE_THRESHOLD	0.6 // Noise threshold (in volts)

// Ping status
typedef enum {
	PING_INVALID = 0,
	PING_VALID = 1,
	PING_OVER = 2,
} ping_status_t;


// Various settings
typedef struct {
	uint16_t valid_start_time;
	uint16_t valid_end_time;
	float noise_threshold;
} peak_detector_t;

extern peak_detector_t peak_detector_s;


void peak_detector_init();


void peak_detector_reset();


// I want to change this function name, just not sure what to
void peak_detector_low();

/** @brief Reads peak detector value and finds the ping status
 */
ping_status_t peak_get_ping_status(float* peak_level);

#endif /* PEAK_DETECTOR_H_ */
