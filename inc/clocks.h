/*
 * clocks.h
 *
 *  Created on: Mar 21, 2019
 *      Author: isthatme
 */

#ifndef CLOCKS_H_
#define CLOCKS_H_

/** @brief Initializes system clocks
 *
 * Currently takes an external 8MHz crystal.
 *
 * TODO: move to 12MHz to squeeze more performance
 *
 * Crystal -> PLL x8 -> SYSCLK -> everything else
 */
void systemclock_config(void);


/** @brief Enables necessary peripheral clocks
 *
 */
void periph_clocks_init(void);



#endif /* CLOCKS_H_ */
