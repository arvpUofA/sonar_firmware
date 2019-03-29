/*
 * gain_control.h
 *
 *  Created on: Mar 27, 2019
 *      Author: isthatme
 */

#ifndef GAIN_CONTROL_H_
#define GAIN_CONTROL_H_

#include <stdbool.h>
#include "stm32f3xx.h"

// Everything here can actually be ints, no real reason to use floats
typedef struct {
	struct {
		float gain;
	} proportional;

	struct {
		float gain;
		float error_max;
		float error_min;
	} integral;

	float optimal_gain;
	float peak_level;
	float desired_peak;

	uint16_t nudge_gain_duration; // in ms (???)
	uint16_t floor_gain_duration;
	float nudge_gain_value;
} gain_control_settings_t;

extern gain_control_settings_t gain_control_settings_s;


/** @brief Initialises gain control.
 *
 */
void gain_control_init(void);

/** @brief Runs gain control.
 *
 * @retval 0 No valid ping detected.
 * @retval 1 Valid ping detected.
 */
void gain_control_run(void);


#endif /* GAIN_CONTROL_H_ */
