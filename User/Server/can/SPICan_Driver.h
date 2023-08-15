/*
*********************************************************************************************************
*	                                            V4
*                           Commuication Controller For The Mining Industry
*
*                                  (c) Copyright 1994-2013  HNDZ 
*                       All rights reserved.  Protected by international copyright laws.
*
*
*    File    : SPICan_Driver.h
*    Module  : user
*    Version : V1.0
*    History :
*   -----------------
*              Version  Date           By            Note
*              v1.0         
*
*********************************************************************************************************
*/

#ifndef __SPICAN_DRIVER_H__
#define __SPICAN_DRIVER_H__

/*****************************************************************************************/
/**Ԥ����ָ�����Ŀ��ʹ�õ��ⲿ����****/
#define SPICAN_2_FOSC_4M					0x00
#define SPICAN_2_FOSC_8M					0x01
#define SPICAN_2_FOSC_16M					0x02
#define SPICAN_2_USE_FOSC					SPICAN_2_FOSC_8M


/****************SPI CAN �������***************/
/**MCP_2515 SPI���֧��10M***/
#define SPICAN_1_BAUNDRATE_PRESCALER	SPI_BaudRatePrescaler_8//ʱ�ӷ�Ƶϵ��.APB1:42M/8 = 5.25 MHz
/**MCP_2515 CAN���֧��1M***/
#define SPICAN_1_CAN_BANDRATE_SET		SPICAN_BAUDRATE_200K//CAN �������趨


//�����ⶨ�����²���
/*************************������HUB�ӿ�**************************************************************/
/*** SPI CAN1�ӿ� **************/
/**SPI CAN1��ʹ�õ�Ӳ������***/
#define SPICAN_1						SPI2//hub��ʹ�õ�SPI�ڶ���
#define GPIO_SPICAN_1_AF_DEFINE			GPIO_AF_SPI2//���ù��ܶ���
#define RCC_SPICAN_1_APBxCmd()			RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE)//SPIʹ��ʱ�ӣ��������ڵ�APB1\APB2�����޸�
#define RCC_SPICAN_1_AF_APBxCmd()		RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
#define SPICAN_1_INT_IRQn				SPI2_IRQn
#define SPICAN_1_INT_IRQHandler			SPI2_IRQHandler

/**����˿ڡ����š���������(GPIO_Pin_sources)��ʱ�Ӷ���*****/
#define GPIO_SPICAN_1_SCLK  			GPIOB
#define PIN_SPICAN_1_SCLK				GPIO_Pin_10
#define GPIO_SPICAN_1_PINSOURCE_SCLK	GPIO_PinSource10
#define RCC_SPICAN_1_SCLK				RCC_AHB1Periph_GPIOB

#define GPIO_SPICAN_1_MOSI  			GPIOC
#define PIN_SPICAN_1_MOSI				GPIO_Pin_3
#define GPIO_SPICAN_1_PINSOURCE_MOSI	GPIO_PinSource3
#define RCC_SPICAN_1_MOSI				RCC_AHB1Periph_GPIOC

#define GPIO_SPICAN_1_MISO  			GPIOC
#define PIN_SPICAN_1_MISO				GPIO_Pin_2
#define GPIO_SPICAN_1_PINSOURCE_MISO	GPIO_PinSource2
#define RCC_SPICAN_1_MIS0				RCC_AHB1Periph_GPIOC

#define GPIO_SPICAN_1_CS 				GPIOF
#define PIN_SPICAN_1_CS					GPIO_Pin_10
#define RCC_SPICAN_1_CS					RCC_AHB1Periph_GPIOF

#define GPIO_SPICAN_1_RST 				GPIOE
#define PIN_SPICAN_1_RST				GPIO_Pin_3
#define RCC_SPICAN_1_RST				RCC_AHB1Periph_GPIOE

#define GPIO_SPICAN_1_TX0RST 			GPIOA
#define PIN_SPICAN_1_TX0RST				GPIO_Pin_0
#define RCC_SPICAN_1_TX0RST				RCC_AHB1Periph_GPIOA

#define GPIO_SPICAN_1_TX1RST 			GPIOF
#define PIN_SPICAN_1_TX1RST				GPIO_Pin_8
#define RCC_SPICAN_1_TX1RST				RCC_AHB1Periph_GPIOF

#define GPIO_SPICAN_1_TX2RST 			GPIOF
#define PIN_SPICAN_1_TX2RST				GPIO_Pin_9
#define RCC_SPICAN_1_TX2RST				RCC_AHB1Periph_GPIOF

/**����˿ڡ����š�ʱ�Ӷ���*****/
#define GPIO_SPICAN_1_INT 				GPIOC
#define PIN_SPICAN_1_INT				GPIO_Pin_1
#define RCC_SPICAN_1_INT				RCC_AHB1Periph_GPIOC

#define GPIO_SPICAN_1_RX0BF 			GPIOC
#define PIN_SPICAN_1_RX0BF				GPIO_Pin_13
#define RCC_SPICAN_1_RX0BF				RCC_AHB1Periph_GPIOC

#define GPIO_SPICAN_1_RX1BF 			GPIOC
#define PIN_SPICAN_1_RX1BF				GPIO_Pin_0
#define RCC_SPICAN_1_RX1BF				RCC_AHB1Periph_GPIOC



/****************�ж϶���**********************/
/**����Ŀ��ʹ��INT��Ϊ���ж�***/
#define SPICAN_1_INT_EXTI_PORT_SOURCE   EXTI_PortSourceGPIOC
#define SPICAN_1_INT_EXTI_PIN_SOURCE    EXTI_PinSource1
#define SPICAN_1_INT_EXTI_LINE          EXTI_Line1
#define SPICAN_1_INT_EXTI_IRQn          EXTI1_IRQn 
#define SPICAN_1_INT_EXTI_IRQHandler	  EXTI1_IRQHandler


/******************************************��ŷ��������ӿ�*****************************************************/
/*** SPI CAN2�ӿ� **************/
/**SPI CAN2��ʹ�õ�Ӳ������***/
#define SPICAN_2						SPI1//hub��ʹ�õ�SPI�ڶ���
#define GPIO_SPICAN_2_AF_DEFINE			GPIO_AF_SPI1//���ù��ܶ���
#define RCC_SPICAN_2_APBxCmd()			RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1|RCC_APB2Periph_SYSCFG, ENABLE)//SPIʹ��ʱ�ӣ��������ڵ�APB1\APB2�����޸�
#define RCC_SPICAN_2_AF_APBxCmd()        RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE)
#define SPICAN_2_INT_IRQn				SPI1_IRQn
#define SPICAN_2_INT_IRQHandler			SPI1_IRQHandler

/**����˿ڡ����š���������(GPIO_Pin_sources)��ʱ�Ӷ���*****/
#define GPIO_SPICAN_2_SCLK  			GPIOA
#define PIN_SPICAN_2_SCLK				GPIO_Pin_5
#define GPIO_SPICAN_2_PINSOURCE_SCLK	GPIO_PinSource5
#define RCC_SPICAN_2_SCLK				RCC_AHB1Periph_GPIOA

#define GPIO_SPICAN_2_MOSI  			GPIOA
#define PIN_SPICAN_2_MOSI				GPIO_Pin_7
#define GPIO_SPICAN_2_PINSOURCE_MOSI	GPIO_PinSource7
#define RCC_SPICAN_2_MOSI				RCC_AHB1Periph_GPIOA

#define GPIO_SPICAN_2_MISO  			GPIOA
#define PIN_SPICAN_2_MISO				GPIO_Pin_6
#define GPIO_SPICAN_2_PINSOURCE_MISO	GPIO_PinSource6
#define RCC_SPICAN_2_MIS0				RCC_AHB1Periph_GPIOA

#define GPIO_SPICAN_2_CS 				GPIOA
#define PIN_SPICAN_2_CS					GPIO_Pin_1
#define RCC_SPICAN_2_CS					RCC_AHB1Periph_GPIOA

#define GPIO_SPICAN_2_RST 				GPIOA
#define PIN_SPICAN_2_RST				GPIO_Pin_3
#define RCC_SPICAN_2_RST				RCC_AHB1Periph_GPIOA

#define GPIO_SPICAN_2_TX0RST 			GPIOC
#define PIN_SPICAN_2_TX0RST				GPIO_Pin_4
#define RCC_SPICAN_2_TX0RST				RCC_AHB1Periph_GPIOC

#define GPIO_SPICAN_2_TX1RST 			GPIOA
#define PIN_SPICAN_2_TX1RST				GPIO_Pin_4
#define RCC_SPICAN_2_TX1RST				RCC_AHB1Periph_GPIOA

#define GPIO_SPICAN_2_TX2RST 			GPIOC
#define PIN_SPICAN_2_TX2RST				GPIO_Pin_5
#define RCC_SPICAN_2_TX2RST				RCC_AHB1Periph_GPIOC

/**����˿ڡ����š�ʱ�Ӷ���*****/
#define GPIO_SPICAN_2_INT 				GPIOA
#define PIN_SPICAN_2_INT				GPIO_Pin_2
#define RCC_SPICAN_2_INT				RCC_AHB1Periph_GPIOA

#define GPIO_SPICAN_2_RX0BF 			GPIOB
#define PIN_SPICAN_2_RX0BF				GPIO_Pin_1
#define RCC_SPICAN_2_RX0BF				RCC_AHB1Periph_GPIOB

#define GPIO_SPICAN_2_RX1BF 			GPIOB
#define PIN_SPICAN_2_RX1BF				GPIO_Pin_0
#define RCC_SPICAN_2_RX1BF				RCC_AHB1Periph_GPIOB

/****************SPI CAN �������***************/
/**MCP_2515 SPI���֧��10M***/
#define SPICAN_2_BAUNDRATE_PRESCALER	SPI_BaudRatePrescaler_16//ʱ�ӷ�Ƶϵ��.APB2:84M/16 = 5.25 MHz
/**MCP_2515 CAN���֧��1M***/
#define SPICAN_2_CAN_BANDRATE_SET		SPICAN_BAUDRATE_200K//CAN �������趨

/****************�ж϶���**********************/
/**����Ŀ��ʹ��INT��Ϊ���ж�***/
#define SPICAN_2_INT_EXTI_PORT_SOURCE   EXTI_PortSourceGPIOA
#define SPICAN_2_INT_EXTI_PIN_SOURCE    EXTI_PinSource2
#define SPICAN_2_INT_EXTI_LINE          EXTI_Line2
#define SPICAN_2_INT_EXTI_IRQn          EXTI2_IRQn 
#define SPICAN_2_INT_EXTI_IRQHandler	EXTI2_IRQHandler

typedef enum
{
	SPICAN_BAUDRATE_50K 	= 0,
	SPICAN_BAUDRATE_80K 	= 1,
	SPICAN_BAUDRATE_100K  	= 2,
	SPICAN_BAUDRATE_125K    = 3,
	SPICAN_BAUDRATE_200K  	= 4,
	SPICAN_BAUDRATE_250K 	= 5,
	SPICAN_BAUDRATE_400K 	= 6,
	SPICAN_BAUDRATE_500K 	= 7,
	SPICAN_BAUDRATE_MAX 	= 8
}SPICAN_BAUNDRATE_TYPE;


/**���巢�ͻ�����ʱʹ�õĶ���****/
typedef enum
{
	SPICAN_FRAME_LENTH_1 = 1,//��ȡ1��������������
	SPICAN_FRAME_LENTH_2 = 2,//��ȡ2��������������
	SPICAN_FRAME_LENTH_3 = 3,//��ȡ3��������������
	SPICAN_FRAME_LENTH_4 = 4,//��ȡ4��������������
	SPICAN_FRAME_LENTH_5 = 5,//��ȡ5��������������
	SPICAN_FRAME_LENTH_6 = 6,//��ȡ6��������������
	SPICAN_FRAME_LENTH_7 = 7,//��ȡ7��������������
	/**�ϳ���***/
	SPICAN_FRAME_LENTH_8 = 8//��ȡ8��������������
}SPICAN_FRAME_LENGTH_TYPE;



/**����֡���Ͷ���*****/
typedef enum
{
	SPICAN_CANFRAME_STAND = 0x00,//��׼֡
	SPICAN_CANFRAME_EXTEND = 0x01,//��չ֡
}SPICAN_FRAME_ID_TYPE;



uint16_t DriverSPICan_Init(uint16_t device, uint16_t sBaud);
uint16_t DriverSPICan_Read(uint16_t device, STR_Can_Frame* RxCan);
uint16_t DriverSPICan_Write(uint16_t device, STR_Can_Frame* TxCan);
uint16_t DriverSPICan_IoCtl(uint16_t device, uint8_t   func, void   *p_arg);


#endif
