/*
 * main.h
 *
 *  Created on: Mar 21, 2019
 *      Author: isthatme
 */

#ifndef MAIN_H_
#define MAIN_H_

#include <stdbool.h>
#include "stm32f3xx.h"
#include "filter.h"
#include "constants.h"
#include "dma.h"
#include "peak_detector.h"
#include "gain_control.h"
#include "i2c.h"
#include "config.h"
#include "comms.h"
#include "clocks.h"
#include "can.h"
#include "amplifier.h"
#include "timers.h"
#include "uart.h"
#include "adc.h"

// Definitions
#define SAMPLE_LEN		TOTAL_SAMPLES

#define CLOCKGEN_I2C_ADDR	0b0110000 // Not sure if correct

// TODO change these to match hardware
#define LED_PORT	GPIOA
#define LED_PIN		GPIO_PIN_10

typedef enum {
    SEND_STOPPED,
    SEND_STARTED,
    SEND_COMPLETED,
} send_status_t;

// Global flags
extern bool sampling_complete;
extern send_status_t sending_ref_status;
extern send_status_t sending_a_status;
extern send_status_t sending_b_status;
#ifdef ADC_FOUR_CHANNELS
extern send_status_t sending_c_status;
#endif

// Input buffers
extern uint16_t channel_ref_buffer[SAMPLE_LEN];
extern uint16_t channel_a_buffer[SAMPLE_LEN];
extern uint16_t channel_b_buffer[SAMPLE_LEN];
#ifdef ADC_FOUR_CHANNELS
extern uint16_t channel_c_buffer[SAMPLE_LEN];
#endif

extern Filter band_pass_filter;


// Function to check and re-arrange sending status at the end of transmission
void check_sending_status(void);

#endif /* MAIN_H_ */
