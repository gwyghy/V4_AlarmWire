/*
*********************************************************************************************************
*	                                            V4
*                           Commuication Controller For The Mining Industry
*
*                                  (c) Copyright 1994-2013  HNDZ 
*                       All rights reserved.  Protected by international copyright laws.
*
*
*    File    : Can_Driver.c
*    Module  : user
*    Version : V1.0
*    History :
*   -----------------
*              Version  Date           By            Note
*              v1.0         
*
*********************************************************************************************************
*/

#define   __CAN_DRIVER_C__
#include "includes.h"
#include "NativeCan_Driver.h"
#include "Can_Bus.h"




/*******************************************************************************************
**函数名称：DriverCan_SetSpeed
**函数作用：原生can设置发送波特率
**函数参数：无
**函数输出：无
**注意事项：无
*******************************************************************************************/
static void DriverNativeCan_SetSpeed(CAN_TypeDef * can, u16 u16CANLSpeed)
{
	CAN_InitTypeDef CAN_InitStructure;
	RCC_ClocksTypeDef  rcc_clocks; 

	/* Configure CAN1 and CAN2 */  
	/* CAN1 and CAN2 register init */
	CAN_DeInit(can);

	/* Struct init*/
	CAN_StructInit(&CAN_InitStructure);

	/* CAN1 and CAN2  cell init */
	CAN_InitStructure.CAN_TTCM = DISABLE;			//禁止时间触发
	CAN_InitStructure.CAN_ABOM = ENABLE;			//离线自动恢复
	CAN_InitStructure.CAN_AWUM = ENABLE;			//自动唤醒
	//CAN_InitStructure.CAN_NART = DISABLE;			//禁止重传
	CAN_InitStructure.CAN_NART = ENABLE;			//允许自动重传
	CAN_InitStructure.CAN_RFLM = DISABLE;			//接收溢出不锁定
	CAN_InitStructure.CAN_TXFP = ENABLE;			//优先级由发送请求的顺序来决定
	CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;	//CAN_Mode_LoopBack
	CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;
	CAN_InitStructure.CAN_BS1 = CAN_BS1_3tq;
	CAN_InitStructure.CAN_BS2 = CAN_BS2_2tq;
 
    RCC_GetClocksFreq(&rcc_clocks);					//调用标准库函数，获取系统时钟。

	// PCLK1/((1+BS1+BS2)*Prescaler)
	CAN_InitStructure.CAN_Prescaler = rcc_clocks.PCLK1_Frequency / 1000 / u16CANLSpeed / (3 + CAN_InitStructure.CAN_BS1 + CAN_InitStructure.CAN_BS2);
	/*Initializes the CAN1 */
	CAN_Init(can, &CAN_InitStructure);				//复位CAN1 CAN2
}
/*******************************************************************************************
**函数名称：Can2PowerEn_Init
**函数作用：can2电源引脚供电
**函数参数：无
**函数输出：无
**注意事项：无
*******************************************************************************************/
void Can2PowerEn_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(CAN2_EN_RCC, ENABLE);
	GPIO_InitStructure.GPIO_Pin = CAN2_EN_Pin;
	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_Init(CAN2_EN_Port, &GPIO_InitStructure);
	
	B12V_ON();

}


/*******************************************************************************************
**函数名称：Can2PowerEn_Init
**函数作用：can2电源引脚供电
**函数参数：无
**函数输出：无
**注意事项：无
*******************************************************************************************/
void Can2_1Enable_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(EN_CAN2_2_RCC, ENABLE);
	GPIO_InitStructure.GPIO_Pin = EN_CAN2_2_Pin;
	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_Init(EN_CAN2_2_Port, &GPIO_InitStructure);
}

/*******************************************************************************************
**函数名称：DriverCan_Init
**函数作用：原生can1初始化
**函数参数：无
**函数输出：无
**注意事项：无
*******************************************************************************************/
void DriverNativeCan_Init(uint16_t device, uint16_t baud)
{
	CAN_TypeDef* Canx;
	GPIO_InitTypeDef  GPIO_InitStructure;
	CAN_FilterInitTypeDef CAN_FilterInitStructure;
	NVIC_InitTypeDef  NVIC_InitStructure;
	if(device == NATIVECAN1)
		Canx = CAN1;
	else if(device == NATIVECAN2)
		Canx = CAN2;
	else if(device == NULL)
		return;
	
		
	if(Canx == CAN2)
	{
		Can2_1Enable_Init();
		Can2PowerEn_Init();
	}
	
	if(Canx == CAN1)
	{
		/* Configure CAN1 and CAN2 IOs */
		/* CAN1 and CAN2 clocks enable */
		RCC_AHB1PeriphClockCmd(CAN1_DEV_GPIO_RCC, ENABLE);
		//设置CANL,CANR引脚复用功能
		GPIO_PinAFConfig(CAN1_DEV_Port, CAN1_DEV_RX_SOURCE, GPIO_AF_CAN1);
		GPIO_PinAFConfig(CAN1_DEV_Port, CAN1_DEV_TX_SOURCE, GPIO_AF_CAN1);	
	}
	else
	{	
		//设置CANL,CANR引脚复用功能
		GPIO_PinAFConfig(CAN2_DEV_Port, CAN2_DEV_RX_SOURCE, CAN2_DEV_GPIO_AF);
		GPIO_PinAFConfig(CAN2_DEV_Port, CAN2_DEV_TX_SOURCE, CAN2_DEV_GPIO_AF);	  
		RCC_AHB1PeriphClockCmd(CAN2_DEV_GPIO_RCC, ENABLE);
	}
		
		/* Configure CAN1_TX_PIN in output pushpull mode */  
	if(Canx == CAN1)   
		GPIO_InitStructure.GPIO_Pin = CAN1_DEV_TxPin;
	else
		GPIO_InitStructure.GPIO_Pin = CAN2_DEV_TxPin;
	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	
	if(Canx == CAN1)   
		GPIO_Init(CAN1_DEV_Port, &GPIO_InitStructure);
	else
		GPIO_Init(CAN2_DEV_Port, &GPIO_InitStructure);

	/* Configure CAN1_RX_PIN in input no pull mode */
	
	if(Canx == CAN1)  
		GPIO_InitStructure.GPIO_Pin = CAN1_DEV_RxPin;
	else
		GPIO_InitStructure.GPIO_Pin = CAN2_DEV_RxPin;		
	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	//GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	
	if(Canx == CAN1)
		GPIO_Init(CAN1_DEV_Port, &GPIO_InitStructure);	
	else
		GPIO_Init(CAN2_DEV_Port, &GPIO_InitStructure);	

	/* CAN1 Periph clocks enable */
	if(Canx == CAN1)
		RCC_APB1PeriphClockCmd(CAN1_DEV_RCC ,ENABLE);  				 //使能CAN1 CAN2功能时钟
	else
		RCC_APB1PeriphClockCmd(CAN2_DEV_RCC ,ENABLE);  				 //使能CAN1 CAN2功能时钟
  
	DriverNativeCan_SetSpeed(Canx,baud);

	/* CAN1 filter init */
						//过滤器号		  //接收所有数据，不滤波，id=0 mask=0xffff
	CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;
	CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;
	CAN_FilterInitStructure.CAN_FilterIdHigh = 0x0000;
	CAN_FilterInitStructure.CAN_FilterIdLow = 0x0000;
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0x0000;
	CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0x0000;
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = 0;
	CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;					//必须至少使能一个;
	if(Canx == CAN1)
		CAN_FilterInitStructure.CAN_FilterNumber = 1;	
	else
		CAN_FilterInitStructure.CAN_FilterNumber = 15;							//必须至少使能一个;应该判断定义STM32F10X_CL与否，以兼容其它芯片
	
	CAN_FilterInit(&CAN_FilterInitStructure);	
  
	if(Canx == CAN1)
		NVIC_InitStructure.NVIC_IRQChannel = CAN1_RX0_IRQn;
	else
		NVIC_InitStructure.NVIC_IRQChannel = CAN2_RX0_IRQn;
	
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x1;			//不使用中断优先级嵌套。因为SysTick的中断优先级为0x0f
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;						//0级用于SysTick
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	if(Canx == CAN1)
		NVIC_InitStructure.NVIC_IRQChannel = CAN1_TX_IRQn;
	else
		NVIC_InitStructure.NVIC_IRQChannel = CAN2_TX_IRQn;
	
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x1;			//不使用中断优先级嵌套。因为SysTick的中断优先级为0x0f
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;						//0级用于SysTick
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	DriverNativeCan_Close(device);
//	CAN_ITConfig(Canx, CAN_IT_TME, ENABLE);
//	CAN_ITConfig(Canx, CAN_IT_FMP0, ENABLE);
	
}


/*******************************************************************************************
**函数名称：DriverNativeCan_Open
**函数作用：原生can的关闭
**函数参数：无
**函数输出：无
**注意事项：无
*******************************************************************************************/
void DriverNativeCan_Open(uint16_t device)
{
	CAN_TypeDef* CANx;
	if(device == NATIVECAN1)
		CANx = CAN1;
	else if(device == NATIVECAN2)
		CANx = CAN2;
	else if(device == NULL)
		return;	
	
	CAN_ITConfig(CANx, CAN_IT_TME, ENABLE);
	CAN_ITConfig(CANx, CAN_IT_FMP0, ENABLE);
}


/*******************************************************************************************
**函数名称：DriverNativeCan_Close
**函数作用：原生can的关闭
**函数参数：无
**函数输出：无
**注意事项：无
*******************************************************************************************/
void DriverNativeCan_Close(uint16_t device)
{
	CAN_TypeDef* CANx;
	if(device == NATIVECAN1)
		CANx = CAN1;
	else if(device == NATIVECAN2)
		CANx = CAN2;
	else if(device == NULL)
		return;	
	
	CAN_ITConfig(CANx, CAN_IT_TME, DISABLE);
	CAN_ITConfig(CANx, CAN_IT_FMP0, DISABLE);	
}


/*******************************************************************************************
**函数名称：DriverCan_Write
**函数作用：原生can数据发送
**函数参数：无
**函数输出：无
**注意事项：无
*******************************************************************************************/
int16_t DriverNativeCan_Write(uint16_t device, STR_Can_Frame* TxCan)
{
	CAN_TypeDef* CANx;
	
	if(device == NATIVECAN1)
		CANx = CAN1;
	else if(device == NATIVECAN2)
		CANx = CAN2;
	else if(device == NULL)
		return -1;
	
	CANx->sTxMailBox[0].TIR = (((u32)TxCan->ID << 3) | ((u32)1 << 2));	//扩展格式、数据帧
    
   	CANx->sTxMailBox[0].TDTR &= (u32)0xFFFFFFF0;
   	CANx->sTxMailBox[0].TDTR |= (u32)(TxCan->DLC & 0x0f);		  
   	CANx->sTxMailBox[0].TDLR = (((u32)(TxCan->Data[3]) << 24) | 
								((u32)(TxCan->Data[2]) << 16) |
								((u32)(TxCan->Data[1]) << 8)  | 
								((u32)(TxCan->Data[0])));
   	CANx->sTxMailBox[0].TDHR = (((u32)(TxCan->Data[7]) << 24) | 
								((u32)(TxCan->Data[6]) << 16) |
								((u32)(TxCan->Data[5]) << 8)  | 
								((u32)(TxCan->Data[4])));
   	CANx->sTxMailBox[0].TIR |= ((u32)0x00000001);						//请求发送
	return 1;
}
/*******************************************************************************************
**函数名称：DriverCan_Read
**函数作用：原生can数据读取
**函数参数：无
**函数输出：无
**注意事项：无
*******************************************************************************************/
int16_t DriverNativeCan_Read(uint16_t device, STR_Can_Frame* RxCan)
{
	uint16_t i;
	CanRxMsg  CanRxMsgTemp;
	CAN_TypeDef* CANx;
	
	if(device == NATIVECAN1)
		CANx = CAN1;
	else if(device == NATIVECAN2)
		CANx = CAN2;
	else
		return -1;
	
	memset((char *)&CanRxMsgTemp, 0x00, sizeof(CanRxMsg));
	memset(RxCan, 0x00, sizeof(STR_Can_Frame));
	
	CAN_Receive(CANx, CAN_FIFO0, &CanRxMsgTemp);
	if(CanRxMsgTemp.IDE == CAN_ID_EXT)			//判断扩展帧
	{
		RxCan->ID = CanRxMsgTemp.ExtId;
		RxCan->DLC   = CanRxMsgTemp.DLC;
		if(RxCan->DLC >8)
		{
			return -1;
		}
		for(i = 0; i < CanRxMsgTemp.DLC; i++)
		{
			RxCan->Data[i] = CanRxMsgTemp.Data[i];
		}
	}
	else	
		return -1;
	return (RxCan->DLC);		
}
/*******************************************************************************************
**函数名称：DriverCan_IoCtl
**函数作用：原生canIO控制
**函数参数：无
**函数输出：无
**注意事项：无
*******************************************************************************************/
uint16_t  DriverNativeCan_IoCtl(uint16_t device, uint8_t   func, void   *p_arg)
{
	CAN_TypeDef* CANx;
	if(device == NATIVECAN1)
		CANx = CAN1;
	else if(device == NATIVECAN2)
		CANx = CAN2;
	else if(device == NULL)
		return -1;
	
 	switch (func)
	{
		case CANCTL_SET_SEND_INT:
			CAN_ITConfig(CANx, CAN_IT_TME, (*(uint16_t*)p_arg == ENABLE)? ENABLE : DISABLE);
			break;
		
		case CANCTL_CHECK_SEND_MAIL0:
			if((CANx->TSR & CAN_TSR_TME0) != CAN_TSR_TME0)
				*(uint16_t *)p_arg = 0;
			else
				*(uint16_t *)p_arg = 1;
			break;
			
		case CANCTL_ABORT_SEND_MAIL0:
			(CANx ->TSR) |= CAN_TSR_ABRQ0;
			break;	

		case CANCTL_CHECK_SEND_TXOK0:
			if((CANx->TSR & CAN_TSR_TXOK0) != CAN_TSR_TXOK0)
				*(uint16_t *)p_arg = 0;
			else
				*(uint16_t *)p_arg = 1;
			break;		
		
		default:
			break;
	}
	return(1);
}

/*******************************************************************************************
**函数名称：CAN1_RX_IRQHandler
**函数作用：can1接收中断
**函数参数：无
**函数输出：无
**注意事项：无
*******************************************************************************************/
void CAN1_RX0_IRQHandler(void)
{
	int16_t  res; 
	uint32_t j, u32FMP0;
	STR_Can_Frame RevBufTemp;
	
	OSIntEnter();	
	
	CAN1->RF0R = (CAN_RF0R_FULL0 | CAN_RF0R_FOVR0);			//清除接收中断的满和溢出
	u32FMP0 = CAN1->RF0R & CAN_RF0R_FMP0;					//接收数据FIFO0计数
	if (u32FMP0)											//存在接收数据
	{
		for (j = 0; j < u32FMP0; j ++)
		{
	
			memset((char *)&RevBufTemp, 0x00, sizeof(STR_Can_Frame));	
			res = DriverNativeCan_Read(NATIVECAN1, &RevBufTemp);
			//读数据判断结果
			if(RevBufTemp.ID == 0)
			{
				res = res;
			}
			if(res >= 0 && res<9 )
			{
				InsCanRevQueue(NATIVECAN1, &RevBufTemp);	
			}
			
		}
	}
	OSIntExit();
}


/*******************************************************************************************
**函数名称：CAN1_TX_IRQHandler
**函数作用：can1发送中断
**函数参数：无
**函数输出：无
**注意事项：无
*******************************************************************************************/
void CAN1_TX_IRQHandler(void)
{
	uint16_t  res;
	if(CAN_GetITStatus(CAN1, CAN_IT_TME) != RESET)
	{
	//获取发送状态
		DriverNativeCan_IoCtl(NATIVECAN1,CANCTL_CHECK_SEND_TXOK0,&res);
		CAN_ClearITPendingBit(CAN1, CAN_IT_TME);
		CanDriverTxDealFun(NATIVECAN1,res);
	}
	OSIntExit();
}


/*******************************************************************************************
**函数名称：CAN1_RX_IRQHandler
**函数作用：can1接收中断
**函数参数：无
**函数输出：无
**注意事项：无
*******************************************************************************************/
void CAN2_RX0_IRQHandler(void)
{
	int16_t   res; 
	u32 j, u16FMP0;
	STR_Can_Frame RevBufTemp;
	
	OSIntEnter();	
	
	CAN2->RF0R = (CAN_RF0R_FULL0 | CAN_RF0R_FOVR0);			//清除接收中断的满和溢出
	u16FMP0 = CAN2->RF0R & CAN_RF0R_FMP0;					//接收数据FIFO0计数
	if (u16FMP0)											//存在接收数据
	{
		for (j = 0; j < u16FMP0; j ++)
		{
			memset(&RevBufTemp, 0x00, sizeof(STR_Can_Frame));	
			res = DriverNativeCan_Read(NATIVECAN2, &RevBufTemp);
			//读数据判断结果
			if(RevBufTemp.ID == 0)
			{
				res = res;
			}
			if(res >= 0 && res<9)
			{
					InsCanRevQueue(NATIVECAN2, &RevBufTemp);	
			}
			else
			{
				res = -1;
			}
		}
	}
	OSIntExit();
}

/*******************************************************************************************
**函数名称：CAN2_TX_IRQHandler
**函数作用：can2发送中断
**函数参数：无
**函数输出：无
**注意事项：无
*******************************************************************************************/
void CAN2_TX_IRQHandler(void)
{
	uint16_t  res;
	OSIntEnter();
	
	if (CAN_GetITStatus(CAN2, CAN_IT_TME) != RESET)
	{
		DriverNativeCan_IoCtl(NATIVECAN2,CANCTL_CHECK_SEND_TXOK0,&res);
		CAN_ClearITPendingBit(CAN2, CAN_IT_TME);
		CanDriverTxDealFun(NATIVECAN2,res);
	}
	OSIntExit();
}





