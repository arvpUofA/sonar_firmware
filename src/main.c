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
 * - Get ADC going for peak detector
 * - Get UAVCAN integrated/working
 * - Re-implement ping detection + gain control
 */
#include <stdbool.h>

#include "stm32f3xx.h"

#include "main.h"
#include "clocks.h"
#include "timers.h"
#include "dma.h"
#include "adc.h"

// Flags
bool ping_started = false;
bool ping_active = false;


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


	/*
	 * Currently, this will run a transfer every 2s, and do nothing with it
	 *
	 * First steps are probably to piggyback and use signals from existing teensy.
	 *
	 * Once that's verified working, we can pull the peak detector signal and
	 * implement everything in here.
	 *
	 * Program flow plan: (why am I writing python?)
	 * Everything in main should be async, meaning we should be able to do other useful
	 * stuff if we are waiting for something to complete.
	 * Maybe implement a state machine? idk
	 *
	 * Init
	 *
	 * loop:
	 * 		handle_gain_control
	 * 		if (ping_complete):
	 *				for buffer in buffers:
	 *						wait for previous buffer to finish transferring
	 *						convert first x samples to float
	 *						start_sending (sets up CAN_DONE interrupt)
	 *						finish converting to float
	 *
	 * CAN_DONE_IRQ:
	 * 		send next n values in frame
	 *
	 *
	 * handle_gain_control:
	 * 			do gain stuff, idk
	 * 			start sampling once we have stable signal
	 */
	while(1) {
		// Start pulling ADC values if we have a ping
		if (ping_started) {
			dma_start_xfer();
			trigger_timer_start();

			// Don't call this loop again
			ping_started = false;
			ping_active = true;
		}

		if(run_ping_control() && !ping_active) {
			ping_started = true;
		}


	}
}
