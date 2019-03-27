/*
 * comms.h
 *
 *  Created on: Mar 27, 2019
 *      Author: isthatme
 */

#ifndef COMMS_H_
#define COMMS_H_

#include "stm32f3xx.h"

/*
 * Comms interface.
 *
 * Two requirements:
 *    - call comms_init(), with function pointer to serial sending function
 *    - call comms_receive(buffer, len) when you receive messages
 */

#define MAX_COMMAND_LENGTH 	5	// Maximum string length of command
#define MAX_VARIABLE_LENGTH	20 	// Maximum string length of variable
#define NUM_COMMANDS		2   // Number of supported commands
#define NUM_VARIABLES		19	// Number of supported variables

static const char* commands[NUM_COMMANDS] = {
		"get",
		"set",
};

static const char* variables[NUM_VARIABLES] = {
		"desiredPeak", "holdGain", "pGain",
		"iGain", "iSaturation", "floorGainDur",
		"nudgeGainDur", "invalidPingDur", "nudgeGainValue",
		"validPingStart", "validPingEnd", "gain",
		"peakLevel", "pingStatus", "centerFreq",
		"debug", "adcAveraging", "validMean",
		"validVariance"
};

typedef void (*comms_send_message_t) (uint8_t buffer, uint16_t len);

struct {
	comms_send_message_t send_message;
} comms_s;

/** @brief Initialises variables, functions for comms interface.
 *
 * @param send_message Function pointer to function that handles sending messages.
 */
void comms_init(comms_send_message_t* send_message);

/** @brief To be called when a message is received.
 *
 * @param buffer Byte-wise message buffer to store message
 * @param len Length of buffer
 */
void comms_receive(uint8_t* buffer, uint16_t len);


#endif /* COMMS_H_ */
