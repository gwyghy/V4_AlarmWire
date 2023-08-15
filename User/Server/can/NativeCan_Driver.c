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
**�������ƣ�DriverCan_SetSpeed
**�������ã�ԭ��can���÷��Ͳ�����
**������������
**�����������
**ע�������
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
	CAN_InitStructure.CAN_TTCM = DISABLE;			//��ֹʱ�䴥��
	CAN_InitStructure.CAN_ABOM = ENABLE;			//�����Զ��ָ�
	CAN_InitStructure.CAN_AWUM = ENABLE;			//�Զ�����
	//CAN_InitStructure.CAN_NART = DISABLE;			//��ֹ�ش�
	CAN_InitStructure.CAN_NART = ENABLE;			//�����Զ��ش�
	CAN_InitStructure.CAN_RFLM = DISABLE;			//�������������
	CAN_InitStructure.CAN_TXFP = ENABLE;			//���ȼ��ɷ��������˳��������
	CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;	//CAN_Mode_LoopBack
	CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;
	CAN_InitStructure.CAN_BS1 = CAN_BS1_3tq;
	CAN_InitStructure.CAN_BS2 = CAN_BS2_2tq;
 
    RCC_GetClocksFreq(&rcc_clocks);					//���ñ�׼�⺯������ȡϵͳʱ�ӡ�

	// PCLK1/((1+BS1+BS2)*Prescaler)
	CAN_InitStructure.CAN_Prescaler = rcc_clocks.PCLK1_Frequency / 1000 / u16CANLSpeed / (3 + CAN_InitStructure.CAN_BS1 + CAN_InitStructure.CAN_BS2);
	/*Initializes the CAN1 */
	CAN_Init(can, &CAN_InitStructure);				//��λCAN1 CAN2
}
/*******************************************************************************************
**�������ƣ�Can2PowerEn_Init
**�������ã�can2��Դ���Ź���
**������������
**�����������
**ע�������
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
**�������ƣ�Can2PowerEn_Init
**�������ã�can2��Դ���Ź���
**������������
**�����������
**ע�������
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
**�������ƣ�DriverCan_Init
**�������ã�ԭ��can1��ʼ��
**������������
**�����������
**ע�������
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
		//����CANL,CANR���Ÿ��ù���
		GPIO_PinAFConfig(CAN1_DEV_Port, CAN1_DEV_RX_SOURCE, GPIO_AF_CAN1);
		GPIO_PinAFConfig(CAN1_DEV_Port, CAN1_DEV_TX_SOURCE, GPIO_AF_CAN1);	
	}
	else
	{	
		//����CANL,CANR���Ÿ��ù���
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
		RCC_APB1PeriphClockCmd(CAN1_DEV_RCC ,ENABLE);  				 //ʹ��CAN1 CAN2����ʱ��
	else
		RCC_APB1PeriphClockCmd(CAN2_DEV_RCC ,ENABLE);  				 //ʹ��CAN1 CAN2����ʱ��
  
	DriverNativeCan_SetSpeed(Canx,baud);

	/* CAN1 filter init */
						//��������		  //�����������ݣ����˲���id=0 mask=0xffff
	CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;
	CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;
	CAN_FilterInitStructure.CAN_FilterIdHigh = 0x0000;
	CAN_FilterInitStructure.CAN_FilterIdLow = 0x0000;
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0x0000;
	CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0x0000;
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = 0;
	CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;					//��������ʹ��һ��;
	if(Canx == CAN1)
		CAN_FilterInitStructure.CAN_FilterNumber = 1;	
	else
		CAN_FilterInitStructure.CAN_FilterNumber = 15;							//��������ʹ��һ��;Ӧ���ж϶���STM32F10X_CL����Լ�������оƬ
	
	CAN_FilterInit(&CAN_FilterInitStructure);	
  
	if(Canx == CAN1)
		NVIC_InitStructure.NVIC_IRQChannel = CAN1_RX0_IRQn;
	else
		NVIC_InitStructure.NVIC_IRQChannel = CAN2_RX0_IRQn;
	
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x1;			//��ʹ���ж����ȼ�Ƕ�ס���ΪSysTick���ж����ȼ�Ϊ0x0f
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;						//0������SysTick
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	if(Canx == CAN1)
		NVIC_InitStructure.NVIC_IRQChannel = CAN1_TX_IRQn;
	else
		NVIC_InitStructure.NVIC_IRQChannel = CAN2_TX_IRQn;
	
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x1;			//��ʹ���ж����ȼ�Ƕ�ס���ΪSysTick���ж����ȼ�Ϊ0x0f
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;						//0������SysTick
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	DriverNativeCan_Close(device);
//	CAN_ITConfig(Canx, CAN_IT_TME, ENABLE);
//	CAN_ITConfig(Canx, CAN_IT_FMP0, ENABLE);
	
}


/*******************************************************************************************
**�������ƣ�DriverNativeCan_Open
**�������ã�ԭ��can�Ĺر�
**������������
**�����������
**ע�������
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
**�������ƣ�DriverNativeCan_Close
**�������ã�ԭ��can�Ĺر�
**������������
**�����������
**ע�������
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
**�������ƣ�DriverCan_Write
**�������ã�ԭ��can���ݷ���
**������������
**�����������
**ע�������
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
	
	CANx->sTxMailBox[0].TIR = (((u32)TxCan->ID << 3) | ((u32)1 << 2));	//��չ��ʽ������֡
    
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
   	CANx->sTxMailBox[0].TIR |= ((u32)0x00000001);						//������
	return 1;
}
/*******************************************************************************************
**�������ƣ�DriverCan_Read
**�������ã�ԭ��can���ݶ�ȡ
**������������
**�����������
**ע�������
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
	if(CanRxMsgTemp.IDE == CAN_ID_EXT)			//�ж���չ֡
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
**�������ƣ�DriverCan_IoCtl
**�������ã�ԭ��canIO����
**������������
**�����������
**ע�������
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
**�������ƣ�CAN1_RX_IRQHandler
**�������ã�can1�����ж�
**������������
**�����������
**ע�������
*******************************************************************************************/
void CAN1_RX0_IRQHandler(void)
{
	int16_t  res; 
	uint32_t j, u32FMP0;
	STR_Can_Frame RevBufTemp;
	
	OSIntEnter();	
	
	CAN1->RF0R = (CAN_RF0R_FULL0 | CAN_RF0R_FOVR0);			//��������жϵ��������
	u32FMP0 = CAN1->RF0R & CAN_RF0R_FMP0;					//��������FIFO0����
	if (u32FMP0)											//���ڽ�������
	{
		for (j = 0; j < u32FMP0; j ++)
		{
	
			memset((char *)&RevBufTemp, 0x00, sizeof(STR_Can_Frame));	
			res = DriverNativeCan_Read(NATIVECAN1, &RevBufTemp);
			//�������жϽ��
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
**�������ƣ�CAN1_TX_IRQHandler
**�������ã�can1�����ж�
**������������
**�����������
**ע�������
*******************************************************************************************/
void CAN1_TX_IRQHandler(void)
{
	uint16_t  res;
	if(CAN_GetITStatus(CAN1, CAN_IT_TME) != RESET)
	{
	//��ȡ����״̬
		DriverNativeCan_IoCtl(NATIVECAN1,CANCTL_CHECK_SEND_TXOK0,&res);
		CAN_ClearITPendingBit(CAN1, CAN_IT_TME);
		CanDriverTxDealFun(NATIVECAN1,res);
	}
	OSIntExit();
}


/*******************************************************************************************
**�������ƣ�CAN1_RX_IRQHandler
**�������ã�can1�����ж�
**������������
**�����������
**ע�������
*******************************************************************************************/
void CAN2_RX0_IRQHandler(void)
{
	int16_t   res; 
	u32 j, u16FMP0;
	STR_Can_Frame RevBufTemp;
	
	OSIntEnter();	
	
	CAN2->RF0R = (CAN_RF0R_FULL0 | CAN_RF0R_FOVR0);			//��������жϵ��������
	u16FMP0 = CAN2->RF0R & CAN_RF0R_FMP0;					//��������FIFO0����
	if (u16FMP0)											//���ڽ�������
	{
		for (j = 0; j < u16FMP0; j ++)
		{
			memset(&RevBufTemp, 0x00, sizeof(STR_Can_Frame));	
			res = DriverNativeCan_Read(NATIVECAN2, &RevBufTemp);
			//�������жϽ��
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
**�������ƣ�CAN2_TX_IRQHandler
**�������ã�can2�����ж�
**������������
**�����������
**ע�������
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





