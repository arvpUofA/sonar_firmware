#ifndef UART_H_
#define UART_H_

#include "stm32f3xx.h"

/** @brief Initialise peripheral, along with GPIO.
 * 
 */
void uart_init(void);

/** @brief Write out UART data
 * 
 */
void uart_send(uint8_t* data, uint16_t len);


#endif