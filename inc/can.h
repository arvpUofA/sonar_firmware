#ifndef CAN_H_
#define CAN_H_

#include "stm32f3xx.h"
#include "canard.h"
#include "canard_stm32.h"
#include "config.h"

/** @brief Initialise CAN hardware, as well as libcanard.
 * 
 */
void can_init(void);


#endif