/*
*********************************************************************************************************
*	                                            DoubleWires
*                           Commuication Controller For The Mining Industry
*
*                                  (c) Copyright 1994-2013  HNDZ 
*                       All rights reserved.  Protected by international copyright laws.
*
*
*    File    : rs485.h
*    Module  : 串口底层驱动
*    Version : V1.0
*    History :
*   -----------------
*              Version  Date           By            Note
*              v1.0     2013-09-13     
*
*********************************************************************************************************
*/

#ifndef __BSP_RS485_H__
#define __BSP_RS485_H__

#include "bsp.h"

#define  RX_LEN                                   20

#define	 USART_RX_SIZE							  256//40
#define	 USART_TX_SIZE							  256//16
#define	 USART_SEND_CHAR_INTERVAL			  	  200//Modsim测试使用，1000//5//发送数据时，两字符之间的间隔(以OS时钟周期为单位)。此处为9600，估算最大值

/*
*********************************************************************************************************
				定义串口A相关参数
*********************************************************************************************************
*/
#define RS485A_USART             UART4
#define RS485A_IRQn          	 UART4_IRQn
#define GPIO_PORT_RS485A_TX      GPIOC
#define GPIO_PORT_RS485A_RX      GPIOC
#define GPIO_PORT_RS485A_EN      GPIOG
#define GPIO_PIN_RS485A_TX     	 GPIO_Pin_10
#define GPIO_PIN_RS485A_RX       GPIO_Pin_11
#define GPIO_PIN_RS485A_EN     	 GPIO_Pin_15

#define RS485A_IRQHandler		 UART4_IRQHandler

#define R485A_SendEnable()       GPIOG->BSRRH = GPIO_Pin_15
#define R485A_SendDisable()      GPIOG->BSRRL = GPIO_Pin_15
#define R485B_SendEnable()       GPIOD->BSRRH = GPIO_Pin_6
#define R485B_SendDisable()      GPIOD->BSRRL = GPIO_Pin_6

#define IsR485A_SendEnable()	 GPIOG->BSRRH == GPIO_Pin_15 ? 1 : 0
/*
*********************************************************************************************************
				定义串口B相关参数
*********************************************************************************************************
*/
#define RS485B_USART             UART5
#define RS485B_IRQn          	 UART5_IRQn
#define GPIO_PORT_RS485B_TX      GPIOC
#define GPIO_PORT_RS485B_RX      GPIOD
#define GPIO_PORT_RS485B_EN      GPIOD
#define GPIO_PIN_RS485B_TX     	 GPIO_Pin_12
#define GPIO_PIN_RS485B_RX       GPIO_Pin_2
#define GPIO_PIN_RS485B_EN     	 GPIO_Pin_6

#define RS485B_IRQHandler		 UART5_IRQHandler

#define R485B_SendEnable()       GPIOD->BSRRH = GPIO_Pin_6
#define R485B_SendDisable()      GPIOD->BSRRL = GPIO_Pin_6

#define IsR485B_SendEnable()	 GPIOD->BSRRH == GPIO_Pin_6 ? 1 : 0
/*
*********************************************************************************************************
				定义串口参数结构体
*********************************************************************************************************
*/
typedef struct
{
    int16_t 	RxTimeout;
    int16_t 	TxTimeout;
	uint8_t  	UartBufTx[USART_TX_SIZE];
	int16_t 	BufTxRd;
	int16_t		BufTxWr;
	uint8_t  	UartBufRx[USART_RX_SIZE]; 
	int16_t		BufRxRd;
	int16_t		BufRxWr;
}USART_DATA_PARA;


extern USART_DATA_PARA Rs485A;
extern USART_DATA_PARA Rs485B;

void Rs485_Data_Init(uint8_t mode);

int  BSP_RS485A_Init(uint16_t usBaudRate,uint16_t usParityMode);
void BSP_RS485B_Init(uint16_t usBaudRate,uint16_t usParityMode);

u16 u16UsartA_IsRec_Over(uint32_t u32CorrespTime , u8 *u8Buf);
u16 u16UsartB_IsRec_Over(uint32_t u32CorrespTime , u8 *u8Buf);

int16_t ReadA_RxData(int16_t buf_bit);
int16_t ReadB_RxData(int16_t buf_bit);

void WriteA_Send_Data(uint8_t *rssend, int16_t send_bit);
void UsartA_Send_Data(void);
void UsartB_Send_Data(uint8_t *rssend, int16_t send_bit);

void RS485BEnable(void);
void RS485BDisable(void);



#endif /* __COM_H__ */
