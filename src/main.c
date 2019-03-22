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
 * Working for channel 2
 *
 * Overall TODO:
 * - Get 3 DMA ADCs going
 * - Get ADC going for peak detector
 * - Get UAVCAN integrated/working
 * - Re-implement ping detection + gain control
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
		dma_start_xfer();
		trigger_timer_start();

		// TODO figure out what exactly this does that I'm not doing by hand
		HAL_ADC_Start(&hadc3);

		HAL_Delay(2000); // poll once every 2s
	}
}
