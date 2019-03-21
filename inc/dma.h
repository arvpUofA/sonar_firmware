/*
 * dma.h
 *
 *  Created on: Mar 21, 2019
 *      Author: isthatme
 */

#ifndef DMA_H_
#define DMA_H_

/*
 * Steps required to start conversion:
 * - Set up DMA transfer to memory (HAL_DMA_START_IT()
 * - Start TIM3
 * - that should be it
 *
 * The DMA transfer complete callback should stop TIM3
 */


/** @brief Function to set up DMA channels and peripherals for ADC 1, 2, and 3.
 *
 *  @note Eventually there should be error handling but for now
 *  	as long as I verify peripheral init this is fine.
 */
void setup_dma(void);



#endif /* DMA_H_ */
