
/* Includes ------------------------------------------------------------------*/
#ifndef __BSP_SPI_ADC_H__
#define __BSP_SPI_ADC_H__

#include "includes.h"

// SPI5
#define	SPI5_PORT			  GPIOF
#define SPI5_CLK_PIN		GPIO_Pin_7
#define SPI5_MISO_PIN		GPIO_Pin_8
#define SPI5_MOSI_PIN		GPIO_Pin_9

#define SPI5_GPIO_RCC		RCC_AHB1Periph_GPIOF
#define SPI5_APB			  APB2
#define SPI5_RCC			  RCC_APB2Periph_SPI5

#define SPI5_CLK_SOURCE		GPIO_PinSource7
#define SPI5_MISO_SOURCE	GPIO_PinSource8
#define SPI5_MOSI_SOURCE	GPIO_PinSource9

#define	SPI5_GPIO_AF		GPIO_AF_SPI5

//------------按照应用功能定义————————

#define AD_SPI_RCC	SPI5_RCC

// AD接口
#define AD_SPI		      SPI5
#define AD_SPI_APB	    SPI5_APB
#define AD_SPI_PORT	  	SPI5_PORT
#define AD_SPI_SCK_PIN	SPI5_CLK_PIN
#define AD_SPI_MISO_PIN	SPI5_MISO_PIN
#define AD_SPI_MOSI_PIN	SPI5_MOSI_PIN

#define AD_GPIO_RCC    	SPI5_GPIO_RCC

#define AD_SPI_SCK_SOURCE	  SPI5_CLK_SOURCE
#define AD_SPI_MISO_SOURCE	SPI5_MISO_SOURCE
#define AD_SPI_MOSI_SOURCE	SPI5_MOSI_SOURCE

#define AD_SPI_GPIO_AF	SPI5_GPIO_AF
				
#define AD_SPI_CS_PORT	GPIOF
#define AD_SPI_CS_PIN	  GPIO_Pin_6

#define AD_CS_GPIO_RCC   	RCC_AHB1Periph_GPIOF


//-------------------------------Chip Select Enable/Disable---------------------
#define MCP3204_CS_LOW()		GPIO_ResetBits(AD_SPI_CS_PORT, AD_SPI_CS_PIN)
#define MCP3204_CS_HIGH()		GPIO_SetBits(AD_SPI_CS_PORT, AD_SPI_CS_PIN)

/* 数据片选 */ 

#define Analog1			   0x00
#define Analog2				 0x01
#define Analog3			   0x02
#define Analog4		     0x03	//第四路模拟量输入
//------------------------------------------------------------------------------
  
void MCP3204_Init(void);
u16 MCP3204_Value(u8 channel, u16 *value);

#endif 



