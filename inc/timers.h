/*
 * timers.h
 *
 *  Created on: Mar 21, 2019
 *      Author: isthatme
 */

#ifndef TIMERS_H_
#define TIMERS_H_

#ifdef __cplusplus
extern "C" {
#endif

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

#ifdef __cplusplus
}
#endif

#endif /* TIMERS_H_ */
