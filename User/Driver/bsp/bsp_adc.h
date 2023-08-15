
/* Includes ------------------------------------------------------------------*/
#ifndef __BSP_ADC_H__
#define __BSP_ADC_H__

#include "includes.h"

#define SAMP_COUNT 10

// ADC3 channel 3
#define ADC3_V            ADC3
#define ADC3_V_CHANNEL    ADC_Channel_3
#define	ADC3_V_PORT			  GPIOA
#define ADC3_V_PIN	     	GPIO_Pin_3

#define ADC3_V_GPIO_RCC		RCC_AHB1Periph_GPIOF
#define ADC3_V_RCC			  RCC_APB2Periph_ADC3

#define ADC3_SOURCE	  	  GPIO_PinSource3

// ADC1 channel 9
#define ADC1_A            ADC1
#define ADC1_A_CHANNEL    ADC_Channel_9
#define	ADC1_A_PORT			  GPIOB
#define ADC1_A_PIN	     	GPIO_Pin_1

#define ADC1_A_GPIO_RCC		RCC_AHB1Periph_GPIOB
#define ADC1_A_RCC			  RCC_APB2Periph_ADC1

#define ADC1_A_SOURCE	  	GPIO_PinSource1


#define adc_vol    1
#define adc_cur    2

void ADC_Configuration(void);
uint16_t  AdcPro(uint8_t adcNum);
#endif


