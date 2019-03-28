/*
 * dma.h
 *
 *  Created on: Mar 21, 2019
 *      Author: isthatme
 */

#ifndef DMA_H_
#define DMA_H_

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Steps required to start conversion:
 * - Set up DMA transfer to memory (HAL_DMA_START_IT()
 * - Start TIM3
 * - that should be it
 *
 * The DMA transfer complete callback should stop TIM3
 *
 *
 * From the STM32F303 Reference manual:
 *
 * ADC1 is available on DMA1, channel 1
 * ADC2 is on DMA1, channels 2 and 3, and DMA2, channels 1 and 2
 * ADC3 is on DMA2, channel 5
 * ADC4 is on DMA2, channels 2 and 4
*/

DMA_HandleTypeDef hdma1_ch1;  // ADC1
DMA_HandleTypeDef hdma2_ch1;  // ADC2
DMA_HandleTypeDef hdma2_ch5;  // ADC3


/** @brief Function to set up DMA channels and peripherals for ADC 1, 2, and 3.
 *
 *  @note Eventually there should be error handling but for now
 *  	as long as I verify peripheral init this is fine.
 */
void setup_dma(void);

/** @brief Sets up DMA transfer
 *
 *  @note Does not start trigger timer
 */
void dma_start_xfer(void);


#ifdef __cplusplus
}
#endif

#endif /* DMA_H_ */
