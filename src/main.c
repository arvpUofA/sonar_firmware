/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
*/

/*
 * CURRENT STATE:
 *
 * Timer is working to send an output to TRGO. Is the timing right? Idk.
 * TRGO works after I call HAL_ADC_Start()
 * DMA seems to be set up, not sure if anything needs to be done.
 */

#include "stm32f3xx.h"

#include "main.h"
#include "clocks.h"
#include "timers.h"
#include "dma.h"
#include "adc.h"


int main(void)
{
	// Initialize clocks
	systemclock_config();
	periph_clocks_init();

	// Peripheral init
	setup_adc_gpio();
	trigger_timer_init();
	setup_adc();
	setup_dma();


	// TODO change this main loop structure. as of now everything runs in an interrupt,
	// which isn't ideal
	while(1) {
		HAL_DMA_Start_IT(&hdma2, (uint32_t) &(ADC3->DR), (uint32_t) channel_ref_buffer, SAMPLE_LEN); // Set up DMA transfer
		HAL_TIM_Base_Start(&htim3); // start conversions

		// TODO figure out what exactly this does that I'm not doing by hand
		HAL_ADC_Start(&hadc3);

		HAL_Delay(2000); // poll once every 2s
	}
}
