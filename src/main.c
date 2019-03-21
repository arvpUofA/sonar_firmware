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

// Stuff required for FFT
#define ARM_MATH_CM4
#include "arm_math.h"

#define FFT_LEN 1024

// Peripheral handles
DMA_HandleTypeDef hdma2;
ADC_HandleTypeDef hadc3;
TIM_HandleTypeDef htim3;
TIM_HandleTypeDef htim4;

// FFT data structures
arm_rfft_instance_q15 fft;
uint16_t fft_x_buffer[FFT_LEN];
uint16_t fft_x_output[FFT_LEN];
			
// Setup ADC1, channel 1.
// This lives on PA0, which is also A0 on the arduino pinout

static void clocks(void) {
	__HAL_RCC_DMA2_CLK_ENABLE();
	__HAL_RCC_ADC34_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_TIM3_CLK_ENABLE();
	__HAL_RCC_TIM4_CLK_ENABLE();
}

static void setup_adc_gpio(void) {
	// Set up gpio for analog input

	GPIO_InitTypeDef gpio;

	// I think this is all you need
	gpio.Pin = GPIO_PIN_0;
	gpio.Mode = GPIO_MODE_ANALOG;
	gpio.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOA, &gpio);
}

// Set up timer to operate as TRGO trigger for ADCs
static void setup_timer(void) {

	// Need to set up timer to get the correct update frequency
	htim3.Instance = TIM3;
	// 64MHz clock, we want 500kHz sample rate (probably overkill)
	//
	htim3.Init.Prescaler = (64000000 / 500000);
	htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim3.Init.Period = 1; // Not sure if this is valud
	htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;

	HAL_TIM_Base_Init(&htim3);

	// Configure to output on TRGO on update event
	// (TRGO should update at 500kHz)
	TIM_MasterConfigTypeDef master;
	master.MasterOutputTrigger = TIM_TRGO_UPDATE;
	master.MasterOutputTrigger2 = TIM_TRGO2_RESET;
	master.MasterSlaveMode = TIM_MASTERSLAVEMODE_ENABLE;
	HAL_TIMEx_MasterConfigSynchronization(&htim3, &master);

	// Setting up TIM4 as slave
	// Leaving this in for debugging
	htim4.Instance = TIM4;
	htim4.Init.Prescaler = 0;
	htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim4.Init.Period = 10000;
	htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	HAL_TIM_Base_Init(&htim4);

	TIM_SlaveConfigTypeDef slave;
	slave.SlaveMode = TIM_SLAVEMODE_EXTERNAL1;
	slave.InputTrigger = TIM_TS_ITR2; // ???
	slave.TriggerPolarity = TIM_TRIGGERPOLARITY_RISING;
	slave.TriggerPrescaler = TIM_TRIGGERPRESCALER_DIV1;
	slave.TriggerFilter = 0;
	HAL_TIM_SlaveConfigSynchronization(&htim4, &slave);

	__HAL_TIM_ENABLE(&htim4);


}

void dma_cmplt_callback(DMA_HandleTypeDef *_hdma) {
	// stop TIM3
	HAL_TIM_Base_Stop(&htim3);


	// deal with FFT
	arm_rfft_q15(&fft, (q15_t *) fft_x_buffer, (q15_t *) fft_x_output);
}

void dma_all_callback(DMA_HandleTypeDef *_hdma) {
	uint8_t thing = 0;
	thing++;
}

// IRQ handler for DMA channel 1
void DMA2_CH5_IRQHandler(void) {


	// Call HAL handler because it gives nice function
	HAL_DMA_IRQHandler(&hdma2);
	// I think this function clears the interrupts
}

void ADC3_IRQHandler(void) {
	__HAL_ADC_CLEAR_FLAG(&hadc3, ADC_IT_EOC);
	// do nothing
	uint8_t thing = 0;
	thing++;

}

static void setup_dma(void) {
	uint32_t rc;
	// Enable DMA IRQ
	NVIC_SetPriority(DMA2_Channel5_IRQn, 0);
	NVIC_EnableIRQ(DMA2_Channel5_IRQn);

	/*
	 * From the STM32F303 Reference manual:
	 *
	 * These are wrong because I did a global search and replace
	 * ADC1 is available on DMA2, channel 1
	 * ADC2 is on DMA2, channels 2 and 3, DMA2, channels 1 and 2
	 * ADC3 is on DMA2, channel 5
	 * ADC4 is on DMA2, channels 2 and 4
	 */
	hdma2.Instance = DMA2_Channel5;
	hdma2.Init.Direction = DMA_PERIPH_TO_MEMORY;
	hdma2.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
	hdma2.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
	hdma2.Init.PeriphInc = DMA_PINC_DISABLE; // I don't think this needs to increment
	hdma2.Init.MemInc = DMA_MINC_ENABLE; // Data should be incremented
	hdma2.Init.Mode = DMA_NORMAL; // Shouldn't re-start transfer at the end.
	hdma2.Init.Priority = DMA_PRIORITY_VERY_HIGH; // Nothing else should be using this channel anyways

	// Configure DMA
	rc = HAL_DMA_Init(&hdma2);

	// Register callback
	HAL_DMA_RegisterCallback(&hdma2, HAL_DMA_XFER_CPLT_CB_ID, dma_cmplt_callback);
	HAL_DMA_RegisterCallback(&hdma2, HAL_DMA_XFER_ALL_CB_ID, dma_all_callback);
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
	RCC_OscInitTypeDef RCC_OscInitStruct;
	RCC_ClkInitTypeDef RCC_ClkInitStruct;
	RCC_PeriphCLKInitTypeDef PeriphClkInit;

	// Configure HSE to use external clock source, and enable PLL with x8 multiplication
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState = RCC_HSE_ON; // On nucleo boards, the HSE is sued through the MCO feature
	RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL8;
	HAL_RCC_OscConfig(&RCC_OscInitStruct);

	// Enable internal clocks and switch over sources
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
								| RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2; // APB1 has max 36MHz clock speed
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
	HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2);

	// Enable ADC12 peripheral clock
	PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC12;
	PeriphClkInit.Adc12ClockSelection = RCC_ADC12PLLCLK_DIV1;
	HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit);
}

int main(void)
{

	systemclock_config();

	clocks();
	setup_adc_gpio();
	setup_timer();
	setup_adc();
	setup_dma();

	// FFT setup
	// Uses too much flash for the K8!!!
	arm_rfft_init_q15(&fft, FFT_LEN, 0, 0);


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

		HAL_ADC_Start(&hadc3);

		// wait for data to be ready
		//while (!(ADC3->ISR & ADC_ISR_ADRDY_Pos));

		HAL_Delay(2000); // poll once every 2s
	}
}


void ADC_DMAConvCmplt(ADC_HandleTypeDef* hadc) {
	// do nothing
}

void ADC_DMAConvHalfCplt(ADC_HandleTypeDef* hadc) {
	// do nothing
}

void ADC_DMALevelOutOfWindow(ADC_HandleTypeDef* hadc) {
	// do nothing
}

void ADC_DMAError(ADC_HandleTypeDef* hadc) {
	// do nothing
}
