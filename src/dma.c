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

#include "main.h"


static void setup_dma(void) {
	uint32_t rc;
	// Enable DMA IRQ
	NVIC_SetPriority(DMA2_Channel5_IRQn, 0);
	NVIC_EnableIRQ(DMA2_Channel5_IRQn);

	/*
	 * From the STM32F303 Reference manual:
	 *
	 * These are wrong because I did a global search and replace
	 * ADC1 is available on DMA2, channel 1
	 * ADC2 is on DMA2, channels 2 and 3, DMA2, channels 1 and 2
	 * ADC3 is on DMA2, channel 5
	 * ADC4 is on DMA2, channels 2 and 4
	 */
	hdma2.Instance = DMA2_Channel5;
	hdma2.Init.Direction = DMA_PERIPH_TO_MEMORY;
	hdma2.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
	hdma2.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
	hdma2.Init.PeriphInc = DMA_PINC_DISABLE; // I don't think this needs to increment
	hdma2.Init.MemInc = DMA_MINC_ENABLE; // Data should be incremented
	hdma2.Init.Mode = DMA_NORMAL; // Shouldn't re-start transfer at the end.
	hdma2.Init.Priority = DMA_PRIORITY_VERY_HIGH; // Nothing else should be using this channel anyways

	// Configure DMA
	rc = HAL_DMA_Init(&hdma2);

	// Register callback
	HAL_DMA_RegisterCallback(&hdma2, HAL_DMA_XFER_CPLT_CB_ID, dma_cmplt_callback);
	HAL_DMA_RegisterCallback(&hdma2, HAL_DMA_XFER_ALL_CB_ID, dma_all_callback);
}
