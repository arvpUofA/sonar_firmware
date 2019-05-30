/*
 * amplifier.c
 *
 *  Created on: Mar 27, 2019
 *      Author: isthatme
 */

#include "amplifier.h"

DAC_HandleTypeDef hdac1;

void amplifier_init(void) {
	// Configure GPIO
	GPIO_InitTypeDef gpio;
	gpio.Pin = AMP_OUT_PIN;
	gpio.Mode = GPIO_MODE_ANALOG;
	HAL_GPIO_Init(AMP_OUT_PORT, &gpio);

	// Initialise DAC1
	hdac1.Instance = DAC1;
	HAL_DAC_Init(&hdac1);

	// Configure channel 1
	DAC_ChannelConfTypeDef channel;
	channel.DAC_Trigger = DAC_TRIGGER_SOFTWARE;
	channel.DAC_OutputBuffer = DAC_OUTPUTBUFFER_ENABLE; // Enable to be safe??
	HAL_DAC_ConfigChannel(&hdac1, &channel, DAC_CHANNEL_1);

	// Enable DAC
	HAL_DAC_Start(&hdac1, DAC_CHANNEL_1);
}

void amplifier_set_gain(float gain) {
	float GPOS = 0; // VGA positive terminal
	float GNEG = 0.9; // VGA negative terminal
	float GPRA = 26; // Pre-amp gain
	uint32_t out_val;

	if (gain > 60) {
		return;
	} else if (gain < 0) {
		return;
	} else {
		// Formula found in page 21 of datasheet
		GPOS = (gain - GPRA - 4.4) / 49.9 + GNEG;

		out_val = (GPOS / VREF) * 4096;
		HAL_DAC_SetValue(&hdac1, DAC_CHANNEL_1, DAC_ALIGN_12B_R, out_val);
	}
}
