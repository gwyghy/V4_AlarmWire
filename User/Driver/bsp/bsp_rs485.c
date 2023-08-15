/*
*********************************************************************************************************
*	                                            DoubleWires
*                           Commuication Controller For The Mining Industry
*
*                                  (c) Copyright 1994-2013  HNDZ 
*                       All rights reserved.  Protected by international copyright laws.
*
*
*    File    : rs485.c
*    Module  : 串口底层驱动
*    Version : V1.0
*    History :
*   -----------------
*              Version  Date           By            Note
*              v1.0     2013-09-13     
*
*********************************************************************************************************
*/

#include "bsp_rs485.h"
#include "OsEvent.h"
#include "string.h"

USART_DATA_PARA Rs485A;
USART_DATA_PARA	Rs485B;



/*
*********************************************************************************************************
*
* Description : void Rs485_Data_Init(void) 
*

* Argument(s) : none.
*
* Return(s)   : 
*********************************************************************************************************
*/
void Rs485_Data_Init(uint8_t mode) 
{
	if(mode ==0)
	{
		memset(&Rs485A, 0x00, sizeof(USART_DATA_PARA));
		PLATFORM_OS_SemCreate(&RS485A_RecSem,0,"RS485A_RecSem");
		PLATFORM_OS_SemCreate(&Rs485A_SenSem,0,"Rs485A_SenSem");
	}
	else
	{
		memset(&Rs485B, 0x00, sizeof(USART_DATA_PARA));
		PLATFORM_OS_SemCreate(&Rs485B_SenSem,0,"Rs485B_SenSem");
		PLATFORM_OS_SemCreate(&RS485B_RecSem,0,"RS485B_RecSem");			
	}
}
/*
*********************************************************************************************************
*
* Description : BSP_RS485A_CONFIG 
*
* Argument(s) : none.
*
* Return(s)   : 
*********************************************************************************************************
*/

static void BSP_RS485A_CONFIG(void)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource10, GPIO_AF_UART4);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource11, GPIO_AF_UART4);                                      
}


/*
*********************************************************************************************************
*                                               BSP_RS485_Init
*
* Description : initialize 485A&485B 
*
* Argument(s) : none.
*
* Return(s)   : 1(for OK)
*********************************************************************************************************
*/
int BSP_RS485A_Init(uint16_t usBaudRate,uint16_t usParityMode)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef  NVIC_InitStructure;
    /* 第1步： 配置GPIO  */
	
	BSP_RS485A_CONFIG();
	
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Pin   = GPIO_PIN_RS485A_TX;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIO_PORT_RS485A_TX, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Pin   = GPIO_PIN_RS485A_RX;
	GPIO_Init(GPIO_PORT_RS485A_RX, &GPIO_InitStructure);
		
	GPIO_InitStructure.GPIO_Pin   = GPIO_PIN_RS485A_EN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIO_PORT_RS485A_EN, &GPIO_InitStructure);

	
	R485A_SendDisable();					      // 置为低电平，接收状态	
	

	/* 第2步： 配置串口硬件参数                                */

	USART_InitStructure.USART_BaudRate            = usBaudRate;	
	USART_InitStructure.USART_StopBits            = USART_StopBits_1;
	
	if(usParityMode == 0)
	{
		USART_InitStructure.USART_Parity          = USART_Parity_No ;
		USART_InitStructure.USART_WordLength      = USART_WordLength_8b;
	}
	else if(usParityMode ==1)
	{
		USART_InitStructure.USART_Parity          = USART_Parity_Odd ;
		USART_InitStructure.USART_WordLength      = USART_WordLength_9b;
	}
	else if(usParityMode ==2)
	{
		USART_InitStructure.USART_Parity          = USART_Parity_Even ;
		USART_InitStructure.USART_WordLength      = USART_WordLength_9b;
	}
	
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(RS485A_USART, &USART_InitStructure);
	
	USART_ClearFlag(RS485A_USART, USART_FLAG_RXNE);
	USART_ITConfig(RS485A_USART, USART_IT_RXNE, ENABLE);
	
//	BSP_IntVectSet(USART1_IRQn, SOFT_USART1_IRQHandler);

	NVIC_InitStructure.NVIC_IRQChannel            = RS485A_IRQn;//USART1通道
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd         = ENABLE;
	NVIC_Init(&NVIC_InitStructure);	
	
	USART_Cmd(RS485A_USART, ENABLE);	
	
	USART_ClearFlag(RS485A_USART, USART_FLAG_TC);     /* 清发送完成标志，Transmission Complete flag */
	USART_ClearFlag(RS485A_USART, USART_FLAG_PE);
	
	return(1);
}

/*
*********************************************************************************************************
*                                               BSP_RS485B_Init
*
* Description : initialize 485B 
*
* Argument(s) : none.
*
* Return(s)   : 1(for OK)
*********************************************************************************************************
*/
void BSP_RS485B_Init(uint16_t usBaudRate,uint16_t usParityMode)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef  NVIC_InitStructure;
		
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOD, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, ENABLE);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource12, GPIO_AF_UART5);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource2, GPIO_AF_UART5);
	
    /* 第1步： 配置GPIO  */
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Pin   = GPIO_PIN_RS485B_TX;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIO_PORT_RS485B_TX, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Pin   = GPIO_PIN_RS485B_RX;
	GPIO_Init(GPIO_PORT_RS485B_RX, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin   = GPIO_PIN_RS485B_EN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIO_PORT_RS485B_EN, &GPIO_InitStructure);
	
	R485B_SendDisable();					      // 置为低电平，接收状态
	
	USART_InitStructure.USART_BaudRate            = usBaudRate;	
	USART_InitStructure.USART_StopBits            = USART_StopBits_1;
	
	if(usParityMode == 0)
	{
		USART_InitStructure.USART_Parity          = USART_Parity_No ;
		USART_InitStructure.USART_WordLength      = USART_WordLength_8b;
	}
	else if(usParityMode ==1)
	{
		USART_InitStructure.USART_Parity          = USART_Parity_Odd ;
		USART_InitStructure.USART_WordLength      = USART_WordLength_9b;
	}
	else if(usParityMode ==2)
	{
		USART_InitStructure.USART_Parity          = USART_Parity_Even ;
		USART_InitStructure.USART_WordLength      = USART_WordLength_9b;
	}
	
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx;	
	USART_Init(RS485B_USART, &USART_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel            = RS485B_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd         = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	

	USART_Cmd(RS485B_USART, ENABLE);

	USART_ClearFlag(RS485B_USART, USART_FLAG_TC);     /* 清发送完成标志，Transmission Complete flag */
	USART_ClearFlag(RS485B_USART, USART_FLAG_IDLE);	
	USART_ClearFlag(RS485B_USART, USART_FLAG_PE);
}

/*
*********************************************************************************************************
*                                               BSP_RS485_Init
*
* Description : int16_t Usart_IsRec_Over(void) 
*
* Argument(s) : u32CorrespTime:等待超时的时间，u8Buf:数据输出的地址
			   依据modbus协议，两帧之间时间间隔至少3.5个字符；一帧之内字符间隔要小于1.5个字符。
*
* Return(s)   : RS485接收的字节数。(马如意，2014.05.14进行了完善)
*********************************************************************************************************
*/
u16 u16UsartA_IsRec_Over(uint32_t u32CorrespTime , u8 *u8Buf)
{
	uint8_t err;

	PLATFORM_OS_SemClear(&RS485A_RecSem);
	err = PLATFORM_OS_SemWait(&RS485A_RecSem, u32CorrespTime);
	if(err == DEF_TIMEOUT)//等待超时，未收到数据
	{
//		USART_ITConfig(RS485A_USART, USART_IT_RXNE, DISABLE); 
		return 0x00;		
	}
	else if(err == DEF_OK)//已经接收到数据，等待数据接收完毕
	{
		while(1)
		{
			err = PLATFORM_OS_SemWait(&RS485A_RecSem, USART_SEND_CHAR_INTERVAL);
			if(err == DEF_TIMEOUT)//接收完成
			{
				PLATFORM_OS_ENTER_CRITICAL();
				Rs485A.BufRxRd = Rs485A.BufRxWr;
				memcpy(u8Buf, Rs485A.UartBufRx,Rs485A.BufRxWr);//将数据写入
				Rs485A.BufRxWr = 0;
				PLATFORM_OS_EXIT_CRITICAL();
				return Rs485A.BufRxRd;								
			}
		}
	}
	else
		return 0x00;
		
}
/*
*********************************************************************************************************
*                                               BSP_RS485_Init
*
* Description : int16_t Usart_IsRec_Over(void) 
*
* Argument(s) : none.
*
* Return(s)   : 
*********************************************************************************************************
*/

u16 u16UsartB_IsRec_Over(uint32_t u32CorrespTime , u8 *u8Buf)
{
	uint8_t err;

	PLATFORM_OS_SemClear(&RS485B_RecSem);
	err = PLATFORM_OS_SemWait(&RS485B_RecSem, u32CorrespTime);
	if(err == DEF_TIMEOUT)//等待超时，未收到数据
	{
//		USART_ITConfig(RS485B_USART, USART_IT_RXNE, DISABLE);        
		return 0x00;		
	}
	else if(err == DEF_OK)//已经接收到数据，等待数据接收完毕
	{
		while(1)
		{
			err = PLATFORM_OS_SemWait(&RS485B_RecSem, USART_SEND_CHAR_INTERVAL);
			if(err == DEF_TIMEOUT)//接收完成
			{
				PLATFORM_OS_ENTER_CRITICAL();
				Rs485B.BufRxRd = Rs485B.BufRxWr;
				memcpy(u8Buf, Rs485B.UartBufRx,Rs485B.BufRxWr);//将数据写入
				Rs485B.BufRxWr = 0;
				PLATFORM_OS_EXIT_CRITICAL();
//				USART_ITConfig(RS485B_USART, USART_IT_RXNE, DISABLE);
				return Rs485B.BufRxRd;								
			}
		}
	}
//	USART_ITConfig(RS485B_USART, USART_IT_RXNE, DISABLE);           
	return 0x00;
		
}

/*
*********************************************************************************************************
*
* Description : int16_t Read_RxData(int16_t) 
*
* Argument(s) : none.
*
* Return(s)   : 
*********************************************************************************************************
*/
int16_t ReadA_RxData(int16_t buf_bit)
{
	if(buf_bit < Rs485A.BufRxRd)
	{	
		return ((int16_t)Rs485A.UartBufRx[buf_bit] << 8) | Rs485A.UartBufRx[buf_bit + 1];					
	}	
	else
		return -1;
		
}

int16_t ReadB_RxData(int16_t buf_bit)
{
	if(buf_bit < Rs485B.BufRxRd)
	{	
		return ((int16_t)Rs485B.UartBufRx[buf_bit] << 8) | Rs485B.UartBufRx[buf_bit + 1];					
	}	
	else
		return -1;
		
}

/*
*********************************************************************************************************
*
* Description : void Write_Send_Data(uint8_t *rssend, int16_t send_bit)
*
* Argument(s) : 发送一帧数据
*
* Return(s)   : 
*********************************************************************************************************
*/
void WriteA_Send_Data(uint8_t *rssend, int16_t send_bit)
{
	PLATFORM_OS_ENTER_CRITICAL();
	Rs485A.BufTxWr = send_bit;
	Rs485A.BufTxRd = 0;
	memcpy(Rs485A.UartBufTx, rssend, send_bit);
	PLATFORM_OS_EXIT_CRITICAL();
}

/*
*********************************************************************************************************
*
* Description : void Usart_Send_Data(uint8_t *rssend, int16_t send_bit)
*
* Argument(s) : 发送一帧数据
*
* Return(s)   : 
*********************************************************************************************************
*/
void UsartA_Send_Data(void)
{
	uint8_t err;
	
	if(Rs485A.BufTxWr > 0)
	{
		R485A_SendEnable();
		USART_SendData(RS485A_USART, Rs485A.UartBufTx[Rs485A.BufTxRd++]);
		USART_ITConfig(RS485A_USART, USART_IT_TC, ENABLE);
		
		PLATFORM_OS_SemClear(&Rs485A_SenSem);
		err = PLATFORM_OS_SemWait(&Rs485A_SenSem, 0);	
	}
}

void UsartB_Send_Data(uint8_t *rssend, int16_t send_bit)
{
	uint8_t err;
	
	PLATFORM_OS_ENTER_CRITICAL();
	Rs485B.BufTxWr = send_bit;
	Rs485B.BufTxRd = 0;
	memcpy(Rs485B.UartBufTx, rssend, send_bit);
	PLATFORM_OS_EXIT_CRITICAL();
	
	if(Rs485B.BufTxWr > 0)
	{
		R485B_SendEnable();	
		USART_SendData(RS485B_USART, Rs485B.UartBufTx[Rs485B.BufTxRd++]);
		USART_ITConfig(RS485B_USART, USART_IT_TC, ENABLE);

		PLATFORM_OS_SemClear(&Rs485B_SenSem);
		err = PLATFORM_OS_SemWait(&Rs485B_SenSem, 0);		
	}
}

/*
*********************************************************************************************************
*
* Description : void RS485A_IRQHandler(void)
*
* Argument(s) : 485中断处理函数
*
* Return(s)   : 
*********************************************************************************************************
*/
void RS485A_IRQHandler(void)
{
	uint8_t clearmid;
	PLATFORM_OS_ENTER_INT();
	
	if (((RS485A_USART->SR & USART_FLAG_RXNE) != 0)&&((RS485A_USART->SR & USART_FLAG_ORE) == 0)&&((RS485A_USART->SR & USART_FLAG_NE) == 0))			//接收RXNE中断置位
	{
		if ((RS485A_USART->SR & USART_FLAG_PE) == 0)
		{
			if (Rs485A.BufRxWr <= USART_RX_SIZE)	
			{
				if(Rs485A.BufRxWr >= USART_RX_SIZE)
					Rs485A.BufRxWr = 0;			
				Rs485A.UartBufRx[Rs485A.BufRxWr++] = (uint8_t)(USART_ReceiveData(RS485A_USART));
				PLATFORM_OS_SemPost(&RS485A_RecSem);			
			}	
		}
		else		
		{
			clearmid = (uint8_t)(USART_ReceiveData(RS485A_USART));
			USART_ClearFlag(RS485A_USART, USART_FLAG_PE);
			USART_ClearFlag(RS485A_USART, USART_FLAG_RXNE);			
		}
	}
	else
	{
		clearmid = (uint8_t)(USART_ReceiveData(RS485A_USART));
		USART_ClearFlag(RS485A_USART, USART_FLAG_RXNE);	
		USART_ClearFlag(RS485A_USART, USART_FLAG_ORE);
		USART_ClearFlag(RS485A_USART, USART_FLAG_NE);
	}
	
	if ((RS485A_USART->SR & USART_FLAG_TC) != 0)	        //发送TXE中断置位
	{		
		if (Rs485A.BufTxRd >= Rs485A.BufTxWr)			
		{
			USART_ClearFlag(RS485A_USART, USART_FLAG_TC); 
			USART_ITConfig(RS485A_USART, USART_IT_TC, DISABLE);	
			USART_ClearFlag(RS485A_USART, USART_FLAG_RXNE);
			USART_ITConfig(RS485A_USART, USART_IT_RXNE, ENABLE);
			Rs485A.BufRxWr = 0;
			R485A_SendDisable();				
			PLATFORM_OS_SemPost(&Rs485A_SenSem);
		}
		else
		{
			USART_SendData(RS485A_USART, Rs485A.UartBufTx[Rs485A.BufTxRd++]);
		}
	}
	PLATFORM_OS_EXIT_INT();
}

/*
*********************************************************************************************************
*
* Description : void RS485A_IRQHandler(void)
*
* Argument(s) : 485中断处理函数
*
* Return(s)   : 
*********************************************************************************************************
*/
void RS485B_IRQHandler(void)
{
	uint8_t clearmid;
	PLATFORM_OS_ENTER_INT();
	
	if (((RS485B_USART->SR & USART_FLAG_RXNE) != 0)&&((RS485B_USART->SR & USART_FLAG_ORE) == 0)&&((RS485B_USART->SR & USART_FLAG_NE) == 0))		//接收RXNE中断置位
	{
		if ((RS485B_USART->SR & USART_FLAG_PE) == 0)
		{		
			if (Rs485B.BufRxWr <= USART_RX_SIZE)	
			{
				if(Rs485B.BufRxWr >= USART_RX_SIZE)
					Rs485B.BufRxWr = 0;			
				Rs485B.UartBufRx[Rs485B.BufRxWr++] = (uint8_t)(USART_ReceiveData(RS485B_USART));
				PLATFORM_OS_SemPost(&RS485B_RecSem);			
			}
		}
        else
		{	
			clearmid = (uint8_t)(USART_ReceiveData(RS485B_USART));
			USART_ClearFlag(RS485B_USART, USART_FLAG_PE);
			USART_ClearFlag(RS485B_USART, USART_FLAG_RXNE);
		}			
	}
	else
	{
		clearmid = (uint8_t)(USART_ReceiveData(RS485B_USART));
		USART_ClearFlag(RS485B_USART, USART_FLAG_RXNE);
		USART_ClearFlag(RS485B_USART, USART_FLAG_ORE);
		USART_ClearFlag(RS485B_USART, USART_FLAG_NE);
	}
	
	if ((RS485B_USART->SR & USART_FLAG_TC) != 0)	        //发送TXE中断置位
	{		
		if (Rs485B.BufTxRd >= Rs485B.BufTxWr)			
		{
			USART_ClearFlag(RS485B_USART, USART_FLAG_TC); 
			USART_ITConfig(RS485B_USART, USART_IT_TC, DISABLE);	
			USART_ClearFlag(RS485B_USART, USART_FLAG_RXNE);
			USART_ITConfig(RS485B_USART, USART_IT_RXNE, ENABLE);
			Rs485B.BufRxWr = 0;
			R485B_SendDisable();				
			PLATFORM_OS_SemPost(&Rs485B_SenSem);
		}
		else
		{
			USART_SendData(RS485B_USART, Rs485B.UartBufTx[Rs485B.BufTxRd++]);
		}
	}
		
	PLATFORM_OS_EXIT_INT();
} 

/*
*********************************************************************************************************
*											RS485AEnable
* Description : 开启485A 
*
* Argument(s) : 无
*
* Return(s)   : 无
*********************************************************************************************************
*/
void RS485BEnable(void)
{
	R485B_SendDisable();					      // 置为低电平，接收状态
	USART_ClearFlag(RS485B_USART, USART_FLAG_RXNE);
	USART_ClearFlag(RS485B_USART, USART_FLAG_TC); 
	USART_ITConfig(RS485B_USART, USART_IT_RXNE, ENABLE);
}

/*
*********************************************************************************************************
*											RS485ADisable
* Description :  关闭485A
*
* Argument(s) : 无
*
* Return(s)   : 无
*********************************************************************************************************
*/
void RS485BDisable(void)
{
	USART_ITConfig(RS485B_USART, USART_IT_RXNE, DISABLE);
}

