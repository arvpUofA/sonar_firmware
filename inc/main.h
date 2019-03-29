/*
 * main.h
 *
 *  Created on: Mar 21, 2019
 *      Author: isthatme
 */

#ifndef MAIN_H_
#define MAIN_H_

#include <stdbool.h>
#include "stm32f3xx.h"
#include "filter.h"

// Definitions
#define SAMPLE_LEN		1024

#define CLOCKGEN_I2C_ADDR	0b0110000 // Not sure if correct

// TODO change these to match hardware
#define LED_PORT	GPIOA
#define LED_PIN		GPIO_PIN_10

// Input buffers
extern uint16_t channel_ref_buffer[SAMPLE_LEN];
extern uint16_t channel_a_buffer[SAMPLE_LEN];
extern uint16_t channel_b_buffer[SAMPLE_LEN];

extern Filter band_pass_filter;

#endif /* MAIN_H_ */
