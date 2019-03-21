/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
*/

/*
 * CURRENT STATE:
 *
 * Timer is working to send an output to TRGO. Is the timing right? Idk.
 * TRGO works after I call HAL_ADC_Start()
 * DMA seems to be set up, not sure if anything needs to be done.
 */

#include "stm32f3xx.h"

#include "main.h"
#include "clocks.h"
#include "dma.h"

// Setup ADC1, channel 1.
// This lives on PA0, which is also A0 on the arduino pinout

static void setup_adc_gpio(void) {
	// Set up gpio for analog input

	GPIO_InitTypeDef gpio;

	// I think this is all you need
	gpio.Pin = GPIO_PIN_0;
	gpio.Mode = GPIO_MODE_ANALOG;
	gpio.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOA, &gpio);
}


void ADC3_IRQHandler(void) {
	__HAL_ADC_CLEAR_FLAG(&hadc3, ADC_IT_EOC);
	// do nothing
	uint8_t thing = 0;
	thing++;

}


static void setup_adc(void) {
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

	// Enable the ADC
	__HAL_ADC_ENABLE(&hadc3);
	//__HAL_ADC_ENABLE_IT(&hadc3, ADC_IT_EOC);
	// Enable DMA mode
    SET_BIT(hadc3.Instance->CFGR, ADC_CFGR_DMAEN);
}

static void systemclock_config(void) {
}

int main(void)
{
	// Initialize clocks
	systemclock_config();
	periph_clocks_init();

	// Peripheral init
	setup_adc_gpio();
	trigger_timer_init();
	setup_adc();
	setup_dma();

	/*
	 * Steps required to start conversion:
	 * - Set up DMA transfer to memory (HAL_DMA_START_IT()
	 * - Start TIM3
	 * - that should be it
	 *
	 * The DMA transfer complete callback should stop TIM3
	 */

	// TODO change this main loop structure. as of now everything runs in an interrupt,
	// which isn't ideal
	while(1) {
		uint32_t rc;
		rc = HAL_DMA_Start_IT(&hdma2, (uint32_t) &(ADC3->DR), (uint32_t) fft_x_buffer, FFT_LEN); // Set up DMA transfer
		rc = HAL_TIM_Base_Start(&htim3); // start conversions

		// TODO figure out what exactly this does that I'm not doing
		HAL_ADC_Start(&hadc3);

		HAL_Delay(2000); // poll once every 2s
	}
}
