/*
 * amplifier.h
 *
 *  Created on: Mar 27, 2019
 *      Author: isthatme
 */

#ifndef AMPLIFIER_H_
#define AMPLIFIER_H_


/*
 * Library for setting gain of AD8336 variable gain amplifier (VGA).
 * Uses DAC on STM32.
 *
 * Pre-amp is set to 26dB, reference voltage set to 0.9V
 */

#include "stm32f3xx.h"

#define AMP_OUT_PIN 	GPIO_PIN_15
#define AMP_OUT_PORT	GPIOB

/** @brief Initialize amplifier control DAC
 *
 */
void amplifier_init(void);


/** @brief Sets amplifier gain
 *
 *  @param gain Amplifier gain in dB, from 0-60
 */
void amplifier_set_gain(float gain);

#endif /* AMPLIFIER_H_ */
