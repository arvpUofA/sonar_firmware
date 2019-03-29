/*
 * dma.c
 *
 *  Created on: Mar 21, 2019
 *      Author: isthatme
 */


/*
 * This file contains all the functions pertinant to DMA transfers.
 *
 * This includes interrupt handlers and initialization functions,
 * as well as any little helper functions we need.
 */

#include "stm32f3xx.h"

#include "main.h"
#include "dma.h"
#include "timers.h"
#include "adc.h"

DMA_HandleTypeDef hdma1_ch1;  // ADC1
DMA_HandleTypeDef hdma2_ch1;  // ADC2
DMA_HandleTypeDef hdma2_ch5;  // ADC3

// Callbacks
static void adc1_cmplt_callback(DMA_HandleTypeDef *_hdma);

void setup_dma(void) {
	// Enable DMA IRQ for ADC1, see handler below
	NVIC_SetPriority(DMA1_Channel1_IRQn, 0);
	NVIC_EnableIRQ(DMA1_Channel1_IRQn);

	// DMA1, channel 1 setup for ADC1
	hdma1_ch1.Instance = DMA1_Channel1;
	hdma1_ch1.Init.Direction = DMA_PERIPH_TO_MEMORY;
	hdma1_ch1.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
	hdma1_ch1.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
	hdma1_ch1.Init.PeriphInc = DMA_PINC_DISABLE; // I don't think this needs to increment
	hdma1_ch1.Init.MemInc = DMA_MINC_ENABLE; // Data should be incremented
	hdma1_ch1.Init.Mode = DMA_NORMAL; // Shouldn't re-start transfer at the end.
	hdma1_ch1.Init.Priority = DMA_PRIORITY_VERY_HIGH; // Nothing else should be using this channel anyways

	HAL_DMA_Init(&hdma1_ch1);

	// DMA2, channel 1 setup for ADC2
	hdma2_ch1.Instance = DMA2_Channel1;
	hdma2_ch1.Init = hdma1_ch1.Init; // Settings are all the same

	HAL_DMA_Init(&hdma2_ch1);

	// DMA 2, channel 5 setup for ADC3
	hdma2_ch5.Instance = DMA2_Channel5;
	hdma2_ch5.Init = hdma1_ch1.Init; // Settings are all the same

	HAL_DMA_Init(&hdma2_ch5);

	/*
	 * We only need to register one callback (which we'll do for ADC1)
	 * because the ADC conversions are all synced up, and run off one timer,
	 * so once one is complete, they all are.
	 */
	HAL_DMA_RegisterCallback(&hdma1_ch1, HAL_DMA_XFER_CPLT_CB_ID, adc1_cmplt_callback);
}

static void adc1_cmplt_callback(DMA_HandleTypeDef *_hdma) {
	trigger_timer_stop();
	adc_stop();
}

void dma_start_xfer(void) {
	HAL_DMA_Start_IT(&hdma1_ch1, (uint32_t) &(ADC1->DR), (uint32_t) channel_ref_buffer, SAMPLE_LEN); // Set up DMA transfer
	HAL_DMA_Start_IT(&hdma2_ch1, (uint32_t) &(ADC2->DR), (uint32_t) channel_a_buffer, SAMPLE_LEN); // Set up DMA transfer
	HAL_DMA_Start_IT(&hdma2_ch5, (uint32_t) &(ADC3->DR), (uint32_t) channel_b_buffer, SAMPLE_LEN); // Set up DMA transfer

	adc_start();
}

/* ------------- Interrupt Handlers ------------- */

// ADC1 DMA Interrupt
void DMA1_CH1_IRQHandler(void) {
	// Call HAL handler because it gives nice interface to use
	HAL_DMA_IRQHandler(&hdma1_ch1);
}
