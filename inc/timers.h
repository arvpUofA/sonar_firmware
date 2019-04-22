/*
 * timers.h
 *
 *  Created on: Mar 21, 2019
 *      Author: isthatme
 */

#ifndef TIMERS_H_
#define TIMERS_H_

// I wish C let you define a number as 1_000_000_000
#define BILLION 1000000000

/** @brief Initializes timer to use as ADC trigger.
 *
 */
void trigger_timer_init(void);

/** @brief Starts trigger timer
 *
 */
void trigger_timer_start(void);

/** @brief Stops trigger timer
 *
 */
void trigger_timer_stop(void);

/** @brief Initialises microsecond timer
 */
void usec_timer_init(void);

/** @brief Reads value from microsecond timer
 * 
 * Assumes you call it at least once every 4 ksec, or else things may overflow
 */
uint64_t usec_timer_read(void);

#endif /* TIMERS_H_ */
