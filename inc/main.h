/*
 * main.h
 *
 *  Created on: Mar 21, 2019
 *      Author: isthatme
 */

#ifndef MAIN_H_
#define MAIN_H_

// Definitions
#define SAMPLE_LEN		1024

// Peripheral handles
DMA_HandleTypeDef hdma1;
DMA_HandleTypeDef hdma2;

ADC_HandleTypeDef hadc3;

TIM_HandleTypeDef htim3;
TIM_HandleTypeDef htim4;


// Input buffers
uint16_t channel_ref_buffer[SAMPLE_LEN];
uint16_t channel_a_buffer[SAMPLE_LEN];
uint16_t channel_b_buffer[SAMPLE_LEN];

#endif /* MAIN_H_ */
