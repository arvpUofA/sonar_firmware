#ifndef UART_H_
#define UART_H_

#include "stm32f3xx.h"

extern bool receive_finished_flag;
extern uint8_t incoming_message_len;
extern char incoming_message[50];

/** @brief Initialise peripheral, along with GPIO.
 * 
 */
void uart_init();

/** @brief Write out UART data
 * 
 */
void uart_send(uint8_t* data, uint16_t len);


#endif