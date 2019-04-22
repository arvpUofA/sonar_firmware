/*
 * clocks.c
 *
 *  Created on: Mar 21, 2019
 *      Author: isthatme
 */


#include "stm32f3xx.h"


void systemclock_config(void) {
	RCC_OscInitTypeDef RCC_OscInitStruct;
	RCC_ClkInitTypeDef RCC_ClkInitStruct;

	// Configure HSE to use external clock source, and enable PLL with x8 multiplication
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState = RCC_HSE_ON; // On nucleo boards, the HSE is sued through the MCO feature
	RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL8;
	HAL_RCC_OscConfig(&RCC_OscInitStruct);

	// Enable internal clocks and switch over sources to PLL sysclock
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
								| RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2; // APB1 has max 36MHz clock speed
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
	HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2);
}


void periph_clocks_init(void) {
	RCC_PeriphCLKInitTypeDef PeriphClkInit;


	// Enable various peripheral clocks
	__HAL_RCC_DMA2_CLK_ENABLE();
	__HAL_RCC_ADC12_CLK_ENABLE();
	__HAL_RCC_ADC34_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_TIM2_CLK_ENABLE();
	__HAL_RCC_TIM3_CLK_ENABLE();
	__HAL_RCC_TIM4_CLK_ENABLE();

	// Enable ADC12 peripheral clock
	// can't quite remember why this isn't included in the above things
	PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC12;
	PeriphClkInit.Adc12ClockSelection = RCC_ADC12PLLCLK_DIV1;
	HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit);
}
