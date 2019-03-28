/*
 * comms.c
 *
 *  Created on: Mar 27, 2019
 *      Author: isthatme
 */

#include <string.h>
#include <stdbool.h>
#include "comms.h"

static uint8_t* invalid_message_string = (uint8_t*) "Invalid Message!\n";

void comms_init(comms_send_message_t send_message) {
	comms_s.send_message = send_message;
}

static int8_t find_command(uint8_t* buffer, uint16_t* len);
static int8_t find_variable(uint8_t* buffer, uint16_t* len);

void comms_receive(uint8_t* buffer, uint16_t len) {
	int8_t command;
	int8_t variable;

	if (buffer[0] != '$') {
		// Command invalid, notify that it is invalid
		comms_s.send_message(invalid_message_string, sizeof(invalid_message_string));
	}

	// Find command, if invalid notify
	if ((command = find_command(buffer, &len)) < 0) {
		comms_s.send_message(invalid_message_string, sizeof(invalid_message_string));
		return;
	}

	// Find variable, if invalid notify
	if ((variable = find_variable(buffer, &len)) < 0) {
		comms_s.send_message(invalid_message_string, sizeof(invalid_message_string));
		return;
	}

	// TODO write get/set stuff

}

/**
 * Finds command, and trims it off the string
 * Returns -1 if no command found, the number of the command otherwise
 */
static int8_t find_command(uint8_t* buffer, uint16_t* len) {
	char command[MAX_COMMAND_LENGTH];

	// Does sscanf format the string with \0 at the end?
	sscanf((char*) buffer, "$%s", command); // Find command string


	for (uint8_t i = 0; i < NUM_COMMANDS; i++) {
		if (strcmp(commands[i], command) == 0) {
			// Valid command found
			uint8_t command_length = strlen(commands[i]);
			*len -= command_length + 1; // Adjust length
			buffer += command_length + 1; // Trim string
			return i;
		}
	}

	return -1;
}

static int8_t find_variable(uint8_t* buffer, uint16_t* len) {
	char variable[MAX_VARIABLE_LENGTH];

	sscanf((char*) buffer, "%s", variable); // Pull out variable name

	for (uint8_t i = 0; i < NUM_VARIABLES; i++) {
		if (strcmp(variables[i], variable) ==0) {
			// Valid variable found
			uint8_t var_length = strlen(variables[i]);
			*len -= var_length + 1;		// Adjust length
			buffer += var_length + 1;	// Trim string
			return i;
		}
	}

	return -1;
}
