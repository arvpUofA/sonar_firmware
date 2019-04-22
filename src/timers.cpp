/*
 * timers.c
 *
 *  Created on: Mar 21, 2019
 *      Author: isthatme
 */


#include "stm32f3xx.h"
#include "main.h"
#include "timers.h"

// Timer for ADC clock synchronization
TIM_HandleTypeDef htim3;
// Timers for 32-bit usec counter
TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim4;

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

void usec_timer_init(void) {
	/* Configuring Timer 2 as the base timer, once it fills up,
	 * it sends the signal to Timer 4, which uses that as a clock.
	 */
	htim2.Instance = TIM2;
	// Clock needs to run at 1MHz to get 1usec resolution
	htim2.Init.Prescaler = (SystemCoreClock / 1000000) - 1;
	htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim2.Init.Period = UINT16_MAX;
	htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	HAL_TIM_Base_Init(&htim2);

	TIM_MasterConfigTypeDef master;
	master.MasterOutputTrigger = TIM_TRGO_UPDATE;
	master.MasterOutputTrigger2 = TIM_TRGO2_RESET;
	master.MasterSlaveMode = TIM_MASTERSLAVEMODE_ENABLE;
	HAL_TIMEx_MasterConfigSynchronization(&htim2, &master);

	htim4.Instance = TIM4;
	htim4.Init.Prescaler = 0;
	htim4.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim4.Init.Period = UINT16_MAX;
	htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	HAL_TIM_Base_Init(&htim4);

	// Not sure how to do this config with the HAL
	// TIM4 ITR1 is the TRGO output from TIM2
	TIM4->SMCR &= (!TIM_SMCR_TS_Msk) | (0b001 << TIM_SMCR_TS_Pos); // Am I right here?
	TIM4->SMCR |= (0b111 << TIM_SMCR_SMS_Pos); // Select external clock 1 as source

	HAL_TIM_Base_Start(&htim2);
	HAL_TIM_Base_Start(&htim4);
}

uint64_t usec_timer_read(void) {
	// Nice static counter to keep a larger value without overflowing.
	static uint64_t usec_counter = 0;

	// Update counter
	usec_counter += TIM2->CNT;
	usec_counter += TIM4->CNT << 16;

	// Clear timers
	TIM2->CNT = 0;
	TIM4->CNT = 0;

	return usec_counter;
}

// Literally just here for a 1ms delay in canard_stm32.c
int usleep(useconds_t usec) {
	uint64_t delay_until = usec_timer_read() + usec;

	// Yeah yeah it's a hardloop delay. Who cares.
	while (delay_until > usec_timer_read());

	return 0;
}
