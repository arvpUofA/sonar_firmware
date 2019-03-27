/*
 * gain_control.c
 *
 *  Created on: Mar 27, 2019
 *      Author: isthatme
 */


#include <stdbool.h>
#include "gain_control.h"
#include "adc.h"

void gain_control_init(void) {

}

bool gain_control_run(void) {
	float error;
	static float sum_error = 0;
	static bool gain_floored_flag = false;

	// If the gain isn't floored, get the ping status
	if (!gain_floored_flag) {

	}

	return false;
}
