/*
 * gain_control.h
 *
 *  Created on: Mar 27, 2019
 *      Author: isthatme
 */

#ifndef GAIN_CONTROL_H_
#define GAIN_CONTROL_H_

#include <stdbool.h>

// Everything here can actually be ints, no real reason to use floats
struct {
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
} gain_control_settings_s;



/** @brief Initialises gain control.
 *
 */
void gain_control_init(void);

/** @brief Runs gain control.
 *
 * @retval 0 No valid ping detected.
 * @retval 1 Valid ping detected.
 */
bool gain_control_run(void);


#endif /* GAIN_CONTROL_H_ */
