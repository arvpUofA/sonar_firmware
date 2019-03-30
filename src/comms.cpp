#include <string.h>
#include "comms.h"
#include "gain_control.h"
#include "peak_detector.h"

CommInterface::CommInterface(void) {
  messageReceived = false;
  debugFlag = false;
  // testLED = false;
}

void CommInterface::parseMessage(char* in_message) {
  char command[MAX_COMMAND_LEN];
  char variable[MAX_VARIABLE_LEN];
  char in_arg[MAX_ARGUMENT_LEN];
  int char_left = 0;

  // Attempt to read in message
  int8_t assigned = sscanf(in_message, "$%s %s %s %n",
                    command, variable, in_arg, &char_left);

  // Check for an invalid message
  if ((assigned < 2) || (char_left > 0)) {
    // print error stuff here
    return;
  }

  // Loop over possible commands
  for (uint_fast8_t i = 0; i < NUMBER_COMMANDS; i++) {
    if (strcmp(command, commands[i]) == 0) {
      commandIndex = i;
      // Loop over possible variables
      for (uint_fast8_t j = 0; j < NUMBER_VARIABLES; j++) {
        if (strcmp(variable, variables[j]) == 0) {
          variableIndex = j;
          argument = in_arg;
          this->CommInterface::handleValidMessage();
          return;
        }
      }

    }
  }

  // Print invalid message stuff here:
  return;
}

void CommInterface::handleValidMessage() {
  if (commandIndex == 0) {
    // If we have a "set" command
    this->CommInterface::setValue();
  } else {
    // If we have a "get" command
    this->CommInterface::sendLocalVariable();
  }
}

// Set the reference to the filter object so we can access its variables
void CommInterface::setFilterPointer(Filter& _filter) { filter = &_filter; }


// This method applies the arguments to their variables if a set command was
// used
void CommInterface::setValue(void) {
  char return_message[100];

  // Attempt to read in arguments
  if (sscanf(argument, "%f", &output_float) != 1) {
    // print error message
    return;
  }
  if (sscanf(argument, "%d", &output_int) != 1) {
    // print error message
    return;
  }


  switch (variableIndex) {
    case 0:
      gain_control_s.desired_peak = output_float;
      break;
    case 1:
      gain_control_s.hold_gain = output_int;
      break;
    case 2:
      gain_control_s.proportional.gain = output_float;
      break;
    case 3:
      gain_control_s.integral.gain = output_float;
      break;
    case 4:
      gain_control_s.integral.error_max = output_float;
      gain_control_s.integral.error_min = -output_float;
      break;
    case 5:
      gain_control_s.floor_gain_duration = output_int;
      break;
    case 6:
      gain_control_s.nudge_gain_duration = output_int;
      break;
    case 7:
      gain_control_s.invalid_ping_duration = output_int;
      break;
    case 8:
      gain_control_s.nudge_gain_value = output_float;
      break;
    case 9:
      peak_detector_s.valid_start_time = output_int;
      break;
    case 10:
      peak_detector_s.valid_end_time = output_int;
      break;
    case 11:
      gain_control_s.optimal_gain = output_float;
      break;
    case 14:
      filter->setCenterFreq(output_int);
      break;
    case 15:
      gain_control_s.debug_flag = output_int;
      debugFlag = (bool)output_int;
      break;
    case 16:
      // not useful
      break;
    case 17:
      gain_control_s.valid_mean = output_float;
      break;
    case 18:
      gain_control_s.valid_variance = output_float;
      break;
    default:
      // Do nothing
      break;
  }

  // Very naive, just *some* output
  sprintf(return_message, "Set command - variable index: %d, argument: %s\n",
      variableIndex, argument);

  this->writeOut((uint8_t*) return_message, strlen(return_message));
}

// This method retrieves the values of the selected variable using a get
// command.  Variable is sent over Serial starting with a '$'
void CommInterface::sendLocalVariable(void) {
  switch (variableIndex) {
    case 0:
      output_float = gain_control_s.desired_peak;
      break;
    case 1:
      output_int = gain_control_s.hold_gain;
      break;
    case 2:
      output_float = gain_control_s.proportional.gain;
      break;
    case 3:
      output_float = gain_control_s.integral.gain;
      break;
    case 4:
      output_float = gain_control_s.integral.error_max;
      break;
    case 11:
      output_float = gain_control_s.optimal_gain;
      break;
    case 12:
      // Doesn't matter, will get outputted in debug info
      break;
    case 13:
      // Doesn't matter, will get outputted in debug info
      break;
    case 14:
      // Doesn't matter, will get outputted in debug info
      break;
    case 16:
      // Doesn't matter, will get outputted in debug info
      break;
    case 17:
      output_float = gain_control_s.valid_mean;
      break;
    case 18:
      output_float = gain_control_s.valid_variance;
    default:
      // Do nothing
      break;
  }

  this->CommInterface::prepareTransmission();
}

// Prepares value for transmission. ie: float or int then adds a '$' to the
// start of message
void CommInterface::prepareTransmission(void) {
  char out_message[100];

  // variable index 13 and 14 are pingStatus and centerFreq respectively
  if ((variableIndex == 13) || (variableIndex == 14) || (variableIndex == 16) ||
      (variableIndex == 1)) {
    sprintf(out_message, "$%s %d", variables[variableIndex], output_int);
  } else {
    sprintf(out_message, "$%s %f", variables[variableIndex], output_float);
  }

  this->writeOut((uint8_t*) out_message, strlen(out_message));
}