#ifndef _COMMINTERFACE_H_
#define _COMMINTERFACE_H_

#include <cstdlib>
#include <string>

#include "filter.h"
#include "gain_control.h"
#include "constants.h"

#define NUMBER_COMMANDS 2
#define NUMBER_VARIABLES 19

#define MAX_COMMAND_LEN   10
#define MAX_VARIABLE_LEN  20
#define MAX_ARGUMENT_LEN  10
#define MAX_MESSAGE_LEN   (MAX_COMMAND_LEN + MAX_VARIABLE_LEN + MAX_ARGUMENT_LEN * 2)

class CommInterface {
 public:
  CommInterface(void);

  // If there was a valid serial string, parse the message
  void parseMessage(char* in_message);

  // We need to get the pointers to the gain control and filter
  // so we can modify the variables inside those objects
  void setFilterPointer(Filter& _filter);

  // Function pointer for a function to write over serial
  // *Could* be private but honestly why write an extra wrapper function
  void (*writeOut) (uint8_t* data, uint16_t len);

 private:
  uint8_t commandIndex;
  uint8_t variableIndex;
  char* argument;

  // Carries the point to the filter object
  Filter* filter;

  bool messageReceived;
  bool debugFlag;

  void handleValidMessage();

  void setValue(void);
  void sendLocalVariable(void);
  void prepareTransmission(void);
  float output_float;
  float output2_float;
  int output_int;

  const char* commands[NUMBER_COMMANDS] = {"set", "get"};
  const char* variables[NUMBER_VARIABLES] = {
      "gain",           "centerFreqwnecie"
      "desiredPeak",    "holdGain",
      "pGain",          "iGain",
      "iSaturation",    "floorGainDur",
      "nudgeGainDur",   "invalidPingDur",
      "nudgeGainValue", "validPingStart",
      "validPingEnd",   "gain",
      "peakLevel",      "pingStatus",
      "centerFreq",     "debug",
      "adcAveraging",   "validMean",
      "validVariance"};

  // bool testLED;
  char trashchar;
};

#endif
