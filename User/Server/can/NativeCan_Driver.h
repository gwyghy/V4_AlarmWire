/*
*********************************************************************************************************
*	                                            V4
*                           Commuication Controller For The Mining Industry
*
*                                  (c) Copyright 1994-2013  HNDZ 
*                       All rights reserved.  Protected by international copyright laws.
*
*
*    File    : Can_Driver.h
*    Module  : user
*    Version : V1.0
*    History :
*   -----------------
*              Version  Date           By            Note
*              v1.0         
*
*********************************************************************************************************
*/

#ifndef __NATIVECAN_DRIVER_H__
#define __NATIVECAN_DRIVER_H__

#define CAN1_DEV_RCC	      RCC_APB1Periph_CAN1
#define CAN1_DEV_Port 	    GPIOA
#define CAN1_DEV_TxPin 		  GPIO_Pin_12
#define CAN1_DEV_RxPin 	  	GPIO_Pin_11
#define CAN1_DEV_GPIO_RCC   RCC_AHB1Periph_GPIOA
#define CAN1_DEV_GPIO_AF	  GPIO_AF_CAN1
#define CAN1_DEV_RX_SOURCE	GPIO_PinSource11
#define CAN1_DEV_TX_SOURCE	GPIO_PinSource12

#define CAN2_DEV_RCC	      RCC_APB1Periph_CAN2
#define CAN2_DEV_Port 	    GPIOB
#define CAN2_DEV_TxPin 	  	GPIO_Pin_13
#define CAN2_DEV_RxPin 	  	GPIO_Pin_12
#define CAN2_DEV_GPIO_RCC	  RCC_AHB1Periph_GPIOB
#define CAN2_DEV_GPIO_AF	  GPIO_AF_CAN2
#define CAN2_DEV_RX_SOURCE	GPIO_PinSource12
#define CAN2_DEV_TX_SOURCE	GPIO_PinSource13

//控制B路12V引脚
#define CAN2_EN_RCC         RCC_AHB1Periph_GPIOH
#define CAN2_EN_Port 	      GPIOH
#define CAN2_EN_Pin 	  	  GPIO_Pin_11|GPIO_Pin_12

//控制电磁阀can的第二路使能
#define EN_CAN2_2_RCC         RCC_AHB1Periph_GPIOH
#define EN_CAN2_2_Port 	      GPIOH
#define EN_CAN2_2_Pin 	  	  GPIO_Pin_5

enum
{
	CANCTL_SET_SEND_INT =0,
	CANCTL_CHECK_SEND_MAIL0,
	CANCTL_ABORT_SEND_MAIL0,
	CANCTL_CHECK_SEND_TXOK0,
};

typedef struct 
{
	uint32_t	ID;
	uint8_t		Data[8];
	uint16_t  DLC;
} STR_Can_Frame;

typedef struct {
	void        (*Init)(uint16_t,uint16_t);
	void        (*Open)(uint16_t);
	void        (*Close)(uint16_t);
	int16_t    (*Read)(uint16_t , STR_Can_Frame* );
	int16_t    (*Write)(uint16_t , STR_Can_Frame* );
	uint16_t    (*IoCtl)(uint16_t , uint8_t , void *);
} CAN_Driver;


#define B12V_ON()   GPIO_ResetBits(CAN2_EN_Port,CAN2_EN_Pin)
#define B12V_OFF()   GPIO_SetBits(CAN2_EN_Port,CAN2_EN_Pin)


void DriverNativeCan_Init(uint16_t device, uint16_t baud);
void DriverNativeCan_Open(uint16_t device);
void DriverNativeCan_Close(uint16_t device);
int16_t DriverNativeCan_Write(uint16_t device, STR_Can_Frame* TxCan);
int16_t DriverNativeCan_Read(uint16_t device, STR_Can_Frame* RxCan);
uint16_t DriverNativeCan_IoCtl(uint16_t device, uint8_t   func, void   *p_arg);


#endif
