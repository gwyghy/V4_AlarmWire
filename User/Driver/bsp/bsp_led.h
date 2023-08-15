/*
*********************************************************************************************************
*	                                            DoubleWires
*                           Commuication Controller For The Mining Industry
*
*                                  (c) Copyright 1994-2013  HNDZ 
*                       All rights reserved.  Protected by international copyright laws.
*
*
*    File    : bsp_led.h
*    Module  : user
*    Version : V1.0
*    History :
*   -----------------
*              Version  Date           By            Note
*              v1.0     2013-08-14     
*
*********************************************************************************************************
*/

#ifndef __BSP_LED_H__
#define __BSP_LED_H__

#include "bsp.h"

#if		BOARD_VERSION == ARMFLY_V5
	/*
		STM32-V5 ������LED���߷��䣺
			LD1     : PI10/TP_NCS          (�͵�ƽ�������ߵ�ƽϨ��)
			LD2     : PF7/NRF24L01_CSN     (�͵�ƽ�������ߵ�ƽϨ��)
			LD3     : PF8/SF_CS            (�͵�ƽ�������ߵ�ƽϨ��)
			LD4     : PC2/NRF905_CSN/VS1053_XCS  (�͵�ƽ�������ߵ�ƽϨ��)
	*/

	/* �����ڶ�Ӧ��RCCʱ�� */
	#define RCC_ALL_LED 	(RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOF | RCC_AHB1Periph_GPIOI)

	#define GPIO_PORT_LED1  GPIOI
	#define GPIO_PIN_LED1	GPIO_Pin_10

	#define GPIO_PORT_LED2  GPIOF
	#define GPIO_PIN_LED2	GPIO_Pin_7

	#define GPIO_PORT_LED3  GPIOF
	#define GPIO_PIN_LED3	GPIO_Pin_8

	#define GPIO_PORT_LED4  GPIOC
	#define GPIO_PIN_LED4	GPIO_Pin_2
	
#elif	BOARD_VERSION == TK100_V1



#elif	BOARD_VERSION == TK100_V2


	
#endif


#endif /* __BSP_LED_H__ */
