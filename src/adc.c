/*
 * adc.c
 *
 *  Created on: Mar 21, 2019
 *      Author: isthatme
 */

#include "stm32f3xx.h"
#include "main.h"
#include "adc.h"

// TODO do all ADC GPIO stuff, not just one
// TODO set up ADC4 for peak detector stuff

ADC_HandleTypeDef hadc1;
ADC_HandleTypeDef hadc2;
ADC_HandleTypeDef hadc3;
ADC_HandleTypeDef hadc4;

void setup_adc_gpio(void) {
	// Set up gpio for analog input

	GPIO_InitTypeDef gpio;

	// I think this is all you need
	gpio.Pin = GPIO_PIN_0;
	gpio.Mode = GPIO_MODE_ANALOG;
	gpio.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOA, &gpio);
}


// Maybe split this up into three functions?
void setup_adc(void) {
	// ADC1 Init
	hadc1.Instance = ADC1;
	hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV1;
	hadc1.Init.Resolution = ADC_RESOLUTION_12B;
	hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT; // Doesn't really matter.
	hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;
	hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV; // Since we do single conversion per sequence, both are equal maybe
	hadc1.Init.LowPowerAutoWait = DISABLE;
	hadc1.Init.ContinuousConvMode = DISABLE;
	hadc1.Init.NbrOfConversion = 1;
	hadc1.Init.DiscontinuousConvMode = DISABLE;
	//hadc.Init.NbrOfDiscConversion = 1;
	hadc1.Init.ExternalTrigConv = ADC_EXTERNALTRIGCONV_T3_TRGO;;
	hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_RISING;
	hadc1.Init.DMAContinuousRequests = DISABLE; // DMA not running in circular mode
	hadc1.Init.Overrun = ADC_OVR_DATA_OVERWRITTEN; // Data should only be overwritten if DMA is over anyways
	HAL_ADC_Init(&hadc1);

	hadc2.Instance = ADC2;
	hadc2.Init = hadc1.Init; // same settings
	HAL_ADC_Init(&hadc2);

	hadc3.Instance = ADC3;
	hadc3.Init = hadc1.Init; // same settings
	HAL_ADC_Init(&hadc3);

	// Initialize channels
	ADC_ChannelConfTypeDef channel;
	channel.Channel = ADC_CHANNEL_1; // not sure which channels I will need
	channel.Rank = ADC_REGULAR_RANK_1;
	channel.SamplingTime = ADC_SAMPLETIME_19CYCLES_5; // Middle of the road for speed
	channel.SingleDiff = ADC_SINGLE_ENDED;
	channel.OffsetNumber = ADC_OFFSET_NONE;
	channel.Offset = 0;
	HAL_ADC_ConfigChannel(&hadc1, &channel);

	channel.Channel = ADC_CHANNEL_1; // not sure which channels I will need
	HAL_ADC_ConfigChannel(&hadc2, &channel);

	channel.Channel = ADC_CHANNEL_1; // not sure which channels I will need
	HAL_ADC_ConfigChannel(&hadc3, &channel);

	// Configure multimode
	ADC_MultiModeTypeDef multimode;
	multimode.Mode = ADC_MODE_INDEPENDENT;
	if (HAL_ADCEx_MultiModeConfigChannel(&hadc1, &multimode) != HAL_OK) {
		while(1);
	}
	if (HAL_ADCEx_MultiModeConfigChannel(&hadc2, &multimode) != HAL_OK) {
		while(1);
	}
	if (HAL_ADCEx_MultiModeConfigChannel(&hadc3, &multimode) != HAL_OK) {
		while(1);
	}

	// Enable DMA mode
    SET_BIT(hadc1.Instance->CFGR, ADC_CFGR_DMAEN);
    SET_BIT(hadc2.Instance->CFGR, ADC_CFGR_DMAEN);
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

void adc_peak_setup(void) {
	hadc4.Instance = ADC4;
}
