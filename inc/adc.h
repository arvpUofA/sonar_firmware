/*
 * adc.h
 *
 *  Created on: Mar 21, 2019
 *      Author: isthatme
 */

#ifndef ADC_H_
#define ADC_H_


/*
 * ADC config:
 *
 * This is the plan, depends on final hardware but I'm going to try to design around this
 * anyways.
 *
 * ADC1 -> Hydrophone REF
 * ADC2 -> Hydrophone Channel A
 * ADC3 -> Hydrophone Channel B
 * ADC4 -> Peak Detector
 */

/** @brief Sets up GPIO to use with ADC
 *
 */
void setup_adc_gpio(void);

/** @brief Initializes ADCs for DMA conversions
 *
 */
void setup_adc(void);

#endif /* ADC_H_ */
