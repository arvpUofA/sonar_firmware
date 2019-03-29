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

  // Check the incomming serialEvent here
  void checkSerial(void);

  // If there was a valid serial string, parse the message
  void parseMessage(void);

  // Returns true if the incomming serial string was valid
  bool validIncommingSerialMessage(void);

  // We need to get the pointers to the gain control and filter
  // so we can modify the variables inside those objects
  void getGainControlPointer(GainControl& _gainControl);
  void getFilterPointer(Filter& _filter);

 private:
  char message[MAX_MESSAGE_LEN];

  // Carries the pointer to the gainControl object
  GainControl* gainControl;
  // Carries the point to the filter object
  Filter* filter;

  bool messageReceived;
  bool debugFlag;

  uint8_t commandIndex;
  uint8_t variableIndex;
  uint8_t argument1Length;
  uint8_t argument2Length;

  void getCommand(void);
  void getVariable(void);
  void getArgument(void);
  void getSecondArgument(void);
  char argument1String[MAX_ARGUMENT_LEN];
  char argument2String[MAX_ARGUMENT_LEN];

  void setValue(void);
  void sendLocalVariable(void);
  void prepareTransmission(void);
  float output_float;
  float output2_float;
  int output_int;

  void cleanup(void);

  const char* commands[NUMBER_COMMANDS] = {"set", "get"};
  const char* variables[NUMBER_VARIABLES] = {
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
