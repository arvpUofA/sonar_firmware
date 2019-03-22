/*
 * adc.c
 *
 *  Created on: Mar 21, 2019
 *      Author: isthatme
 */

#include "stm32f3xx.h"
#include "main.h"
#include "adc.h"

// TODO get all ADCs running

ADC_HandleTypeDef hadc1;
ADC_HandleTypeDef hadc2;
ADC_HandleTypeDef hadc3;

void setup_adc_gpio(void) {
	// Set up gpio for analog input

	GPIO_InitTypeDef gpio;

	// I think this is all you need
	gpio.Pin = GPIO_PIN_0;
	gpio.Mode = GPIO_MODE_ANALOG;
	gpio.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOA, &gpio);
}


void setup_adc(void) {
	NVIC_SetPriority(ADC3_IRQn, 0);
	NVIC_EnableIRQ(ADC3_IRQn);

	hadc3.Instance = ADC3;
	hadc3.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV1;
	hadc3.Init.Resolution = ADC_RESOLUTION_12B;
	hadc3.Init.DataAlign = ADC_DATAALIGN_RIGHT; // Doesn't really matter.
	hadc3.Init.ScanConvMode = ADC_SCAN_DISABLE;
	hadc3.Init.EOCSelection = ADC_EOC_SINGLE_CONV; // Since we do single conversion per sequence, both are equal maybe
	hadc3.Init.LowPowerAutoWait = DISABLE;
	hadc3.Init.ContinuousConvMode = DISABLE;
	hadc3.Init.NbrOfConversion = 1;
	hadc3.Init.DiscontinuousConvMode = DISABLE;
	//hadc.Init.NbrOfDiscConversion = 1;
	hadc3.Init.ExternalTrigConv = ADC_EXTERNALTRIGCONV_T3_TRGO;;
	hadc3.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_RISING;
	hadc3.Init.DMAContinuousRequests = DISABLE; // DMA not running in circular mode
	hadc3.Init.Overrun = ADC_OVR_DATA_OVERWRITTEN; // Data should only be overwritten if DMA is over anyways
	HAL_ADC_Init(&hadc3);

	// Initialize channel
	ADC_ChannelConfTypeDef channel;
	channel.Channel = ADC_CHANNEL_12;
	channel.Rank = ADC_REGULAR_RANK_1;
	channel.SamplingTime = ADC_SAMPLETIME_19CYCLES_5; // Middle of the road for speed
	channel.SingleDiff = ADC_SINGLE_ENDED;
	channel.OffsetNumber = ADC_OFFSET_NONE;
	channel.Offset = 0;

	HAL_ADC_ConfigChannel(&hadc3, &channel);

	// Configure multimode
	ADC_MultiModeTypeDef multimode;
	multimode.Mode = ADC_MODE_INDEPENDENT;
	if (HAL_ADCEx_MultiModeConfigChannel(&hadc3, &multimode) != HAL_OK) {
		while(1);
	}

	// Enable DMA mode
    SET_BIT(hadc3.Instance->CFGR, ADC_CFGR_DMAEN);
}

void adc_start(void) {
	HAL_ADC_Start(&hadc1);
	HAL_ADC_Start(&hadc2);
	HAL_ADC_Start(&hadc3);
}

void adc_stop(void) {
	HAL_ADC_Stop(&hadc1);
	HAL_ADC_Stop(&hadc2);
	HAL_ADC_Stop(&hadc3);
}
