/*
 * timers.c
 *
 *  Created on: Mar 21, 2019
 *      Author: isthatme
 */


#include "stm32f3xx.h"
#include "main.h"
#include "timers.h"

// Peripheral handler
TIM_HandleTypeDef htim3;

void trigger_timer_init(void) {
	// Need to set up timer to get the correct update frequency
	htim3.Instance = TIM3;
	// 64MHz clock, we want 1MSPS
	//
	htim3.Init.Prescaler = (64000000 / 1000000);
	htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim3.Init.Period = 1; // TODO verify if we need a 1 or a 0 here
	htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;

	HAL_TIM_Base_Init(&htim3);

	// Configure to output on TRGO on update event
	// (TRGO should update at 500kHz)
	TIM_MasterConfigTypeDef master;
	master.MasterOutputTrigger = TIM_TRGO_UPDATE;
	master.MasterOutputTrigger2 = TIM_TRGO2_RESET;
	master.MasterSlaveMode = TIM_MASTERSLAVEMODE_ENABLE;
	HAL_TIMEx_MasterConfigSynchronization(&htim3, &master);
}

void trigger_timer_start(void) {
	HAL_TIM_Base_Start(&htim3); // start trigger timer
}

void trigger_timer_stop(void) {
	HAL_TIM_Base_Stop(&htim3); // stop trigger timer
}
