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
 * Ugh, I started out with just C, then moved to C++ and took some concepts.
 * It's still cleaner than the old preprocessor firmware, in my opinion,
 * but I do need to go back through at some point and redefine a
 * bunch of my interfaces.
 *
 * 3-Channel ADC is theoretically working,
 *
 * Overall TODO:
 * - Get UAVCAN integrated/working (theoretically finished?)
 * - Clear out old variable changing stuff (there's only really 4 or 5 necessary ones)
 * 
 * Feature requests from Rumman:
 * - Add ability to set static gain
 * - Clearer state (is it not detecting pings at all, ping valid, ping invalid, between pings, etc.) (publish over CAN!)
 * - Get rid of some command line variables
 * - Improve valid state detection etc.
 *
 * Long term stuff:
 * - Write USB library and integrate into comms
 */
#include <stdbool.h>

// Uncomment for four ADC channels vs 3
//#define ADC_FOUR_CHANNELS

#include "stm32f3xx.h"
#include "main.h"

CommInterface* comm_interface;

// Flags
bool sampling_complete = false;
send_status_t sending_ref_status = SEND_STOPPED;
send_status_t sending_a_status = SEND_STOPPED;
send_status_t sending_b_status = SEND_STOPPED;
#ifdef ADC_FOUR_CHANNELS
send_status_t sending_c_status = SEND_STOPPED;
#endif

// Input buffers
uint16_t channel_ref_buffer[SAMPLE_LEN];
uint16_t channel_a_buffer[SAMPLE_LEN];
uint16_t channel_b_buffer[SAMPLE_LEN];
#ifdef ADC_FOUR_CHANNELS
uint16_t channel_c_buffer[SAMPLE_LEN];
#endif

static void check_incoming_message();

int main(void)
{
	// Initialize clocks
	systemclock_config();
	periph_clocks_init();
	HAL_Init();

	// Peripheral init
	setup_adc_gpio();
	trigger_timer_init();
	setup_adc();
	setup_dma();

	I2C i2c(1, 100000, 42);

	MCP3021 peak_adc(i2c.get_i2c_handle(), 72);
	Filter band_pass_filter(i2c);
	*comm_interface = CommInterface();


	gain_control_init(&peak_adc);


	band_pass_filter.setCenterFreq(27);
	comm_interface->setFilterPointer(band_pass_filter);
	// Set method to write out over communications interface
	comm_interface->writeOut = uart_send;

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
		check_incoming_message();

		gain_control_run();

		// If our reading is complete.
		if (sampling_complete) {
			can_send_raw_data(SONAR_CHANNEL_REF, channel_ref_buffer, SAMPLE_LEN);
			can_start_transmit();
			sending_ref_status = SEND_STARTED;

			#ifdef ADC_FOUR_CHANNELS
			can_send_raw_data(SONAR_CHANNEL_C, channel_c_buffer, SAMPLE_LEN);
			#endif

			sampling_complete = false;
		}

		if (sending_ref_status == SEND_COMPLETED) {
			sending_ref_status = SEND_STOPPED;
			can_send_raw_data(SONAR_CHANNEL_A, channel_a_buffer, SAMPLE_LEN);
			can_start_transmit();
			sending_a_status = SEND_STARTED;
		}

		if (sending_a_status == SEND_COMPLETED) {
			sending_a_status = SEND_STOPPED;
			can_send_raw_data(SONAR_CHANNEL_B, channel_b_buffer, SAMPLE_LEN);
			can_start_transmit();
			sending_b_status = SEND_STARTED;
		}

		#ifdef ADC_FOUR_CHANNELS
		if (sending_b_status == SEND_COMPLETED) {
			sending_b_status = SEND_STOPPED;
			can_send_raw_data(SONAR_CHANNEL_C, channel_c_buffer, SAMPLE_LEN);
			can_start_transmit();
			sending_c_status = SEND_STARTED;
		}
		#endif
		
	}
}

// Function to check and re-arrange sending status at the end of transmission
void check_sending_status(void) {
	if (sending_ref_status == SEND_STARTED) {
		sending_ref_status = SEND_COMPLETED;
	} else if (sending_a_status == SEND_STARTED) {
		sending_a_status = SEND_COMPLETED;
	} else if (sending_b_status == SEND_STARTED) {
		sending_b_status = SEND_COMPLETED;
	#ifdef ADC_FOUR_CHANNELS
		} else if (sending_c_status == SEND_STARTED) {
			sending_c_status = SEND_COMPLETED;
		}
	#else
		}
	#endif
}

static void check_incoming_message() {
	if (receive_finished_flag) {
		// If we are finished receiving, parse message
		comm_interface->parseMessage(incoming_message);
		incoming_message_len = 0; // Reset length so reception can work
		receive_finished_flag = false; // Done receiving, lower flag
	}
}