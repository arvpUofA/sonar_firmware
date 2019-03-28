/*
 * filter.h
 *
 *  Created on: Mar 27, 2019
 *      Author: isthatme
 */

#ifndef FILTER_H_
#define FILTER_H_

#include "stm32f3xx.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Library for filter using SI5351 clock generator
 * Using a modified version of this library;
 * https://github.com/NT7S/Si5351
 * Actually I may just modify the adafruit library
 */

/** @brief Initialise filter with default centre frequency of 27kHz
 *
 */
void filter_init(void);

/** @brief Set centre frequency of filter
 *
 * @param freq Centre frequency of filter (in kHz)
 */
void filter_set_centre_freq(uint8_t freq);


#ifdef __cplusplus
}
#endif

#endif /* FILTER_H_ */
