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
DMA_HandleTypeDef hdma1_ch1;  // ADC1
DMA_HandleTypeDef hdma2_ch1;  // ADC2
DMA_HandleTypeDef hdma2_ch5;  // ADC3


// Input buffers
uint16_t channel_ref_buffer[SAMPLE_LEN];
uint16_t channel_a_buffer[SAMPLE_LEN];
uint16_t channel_b_buffer[SAMPLE_LEN];

#endif /* MAIN_H_ */
