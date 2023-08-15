/************************************************************
** �ļ���:  ir_emit.c
** Copyright (c) 1998-1999 ��������Ӽ������޹�˾����������
** ������:  HNDZ
** �ա���:  2015.04
** �޸���:	
** �ա���:	
** �衡��:  ���ⷢ�������봦�����
**          ���ڵ�Һ�ؿ���v2.0��Ŀ��IR���ͱ���ΪNEC��ʽ��
** �桡��:	V1.0A
**-----------------------------------------------------------
*************************************************************/
#ifndef __IR_EMIT_C__
#define __IR_EMIT_C__
#endif

#include "includes.h"
#include "IR_Rx.h"
#include "V4_Wireless.h"


#define TOGGLE_STEP			52									//���TOGGLE����ֵ�����Ƶ���ʱ���й�

static vu32 s_vu32EmitCompleted = TRUE;						//IR�������
static vu32 s_vu32PWMCount = 0;								//PWM�������
static vu32 s_vu32ArrayPt = 0;									//��������ָ��
static vu16 s_vu16EmitTimeArray[80];							//IR����Carrier��idle��ռPWM���ڼ�������
/***********************************************************
** ��������IR_EmitInit
** �䡡�룺None
** �䡡����None
** ����������IR������Χ�豸��ʼ�� 
************************************************************/
void IR_EmitInit(void)
{
//#ifndef DYK_SS
	GPIO_InitTypeDef			GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef		TIM_Structure;					//���嶨ʱ���ṹ��
	TIM_OCInitTypeDef			TIM_OCInitStructure;
	NVIC_InitTypeDef			NVIC_InitStructure; 
	
	RCC_AHB1PeriphClockCmd(IR_OUT_GPIO_RCC, ENABLE);			//IR����˿�ʱ��ʹ��

	/* Configure IR_OUT_GPIO_PIN in output pushpull mode */
	GPIO_InitStructure.GPIO_Pin = IR_OUT_GPIO_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(IR_OUT_GPIO_PORT, &GPIO_InitStructure);

	//����IR�������Ÿ��ù���
	GPIO_PinAFConfig(IR_OUT_GPIO_PORT, IR_OUT_SOURCE, IR_OUT_AF);

	TIM_TimeBaseStructInit(&TIM_Structure);						//��ʼ����ʱ���ṹ��
	TIM_OCStructInit(&TIM_OCInitStructure);						//��ʼ����ʱ��ͨ���ṹ��
	TIM_DeInit(IR_TIM);											//����TIMĬ��ģʽ


	RCC_APB2PeriphClockCmd(IR_TIM_RCC, ENABLE);					//IR���䶨ʱ��ʱ��ʹ��

	//��ֹ��ʱ���������
	TIM_Cmd(IR_TIM, DISABLE);									//�رն�ʱ��
	TIM_CtrlPWMOutputs(IR_TIM, DISABLE);						//�ر���������˿�

#if 1	//ʹ����PWM������ʹ���ظ�����������
	//��ʼ��IR������ʹ�õĶ�ʱ��
	TIM_Structure.TIM_Period = 14;								//���ó�ʼֵ,��ʱ26.25us�жϣ�����PWM����(15/571.43KHz)

	TIM_Structure.TIM_Prescaler = 293;						//���ö�ʱ��3�ķ�Ƶֵ��ʱ��Ϊ571.43KHz(168M/294)
	TIM_Structure.TIM_ClockDivision = TIM_CKD_DIV1;				//����ʱ�ӷָ�
	TIM_Structure.TIM_CounterMode = TIM_CounterMode_Up;			//TIM���ϼ���ģʽ
    TIM_Structure.TIM_RepetitionCounter = 0;					//�ظ�������
	TIM_TimeBaseInit(IR_TIM, &TIM_Structure);					//���ṹ���ֵ����ʼ����ʱ��
	TIM_InternalClockConfig(IR_TIM);							//����TIM��ʱ��Ϊ�ڲ�ʱ��

	//��ʼ��IR������ʹ�õĶ�ʱ����PWM���ͨ��
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;			//PWM1ģʽ 
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;//���ʹ��
	TIM_OCInitStructure.TIM_Pulse = (TIM_Structure.TIM_Period+1)/3;//����PWMռ�ձȡ�
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;	//���ü��ԡ�
	TIM_OC1Init(IR_TIM, &TIM_OCInitStructure);					//��ʼ��ͨ��1���
	TIM_OC1PreloadConfig(IR_TIM, TIM_OCPreload_Enable);			//ʹ���Զ����رȽϲ���

	//��ֹͨ���ж�
	TIM_ITConfig(IR_TIM, TIM_IT_CC1 , DISABLE);					//��ֹ����Ƚ�ģʽ�ж�

	TIM_ClearFlag(IR_TIM, TIM_FLAG_Update);						//������±�ʶ
	//��ʼ����ʱ������ж�
    TIM_ITConfig(IR_TIM, TIM_IT_Update, ENABLE);				//ʹ��IR_TIM����ж�Դ

	//ʹ�ܶ�ʱ��ARR�Զ���װ
	TIM_ARRPreloadConfig(IR_TIM, ENABLE);						//ʹ���Զ���װ

	//��ʼ����ʱ���ж�
    NVIC_InitStructure.NVIC_IRQChannel = IR_TIM_IRQ;			//ʹ�ܻ���ʧ��ָ����IRQͨ�� TIMȫ���ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0f;//��ʹ���ж����ȼ�Ƕ�ס���ΪSysTick���ж����ȼ�Ϊ0x0f
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;	//1;	//���ó�Ա NVIC_IRQChannel�еĴ����ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				//ʹ�� NVIC_IRQChannel
	NVIC_Init(&NVIC_InitStructure);    
#endif


	//��ʼ��������
	IR_TIM->CNT = 0x0000;										//��ʱ���ļ���������
	//��ֹ��ʱ��
	TIM_Cmd(IR_TIM, DISABLE);
	//��ֹ��ʱ�������
	TIM_CtrlPWMOutputs(IR_TIM, DISABLE);
//#endif
}
/***********************************************************
** ��������IREmitEnabled
** �䡡�룺None
** �䡡����None
** ����������ʹ��IR���䶨ʱ��
************************************************************/
static void IREmitEnabled(void)
{
//#ifndef DYK_SS
	u32 i = 0;
	
	s_vu32PWMCount = 0;
	s_vu32ArrayPt = 1;											//�ظ�������װ�ص�ά��
	s_vu32EmitCompleted = FALSE;								//IR����δ���

	//����װ��
	TIM_ARRPreloadConfig(IR_TIM, DISABLE);						//ʹ������װ��
	TIM_SetAutoreload(IR_TIM, 14);								//���ó�ʼֵ,��ʱ26.25us�жϣ�����PWM����(15/571.43KHz)

	TIM_PrescalerConfig(IR_TIM, 293, TIM_PSCReloadMode_Update);	//���ö�ʱ��3�ķ�Ƶֵ��ʱ��Ϊ571.43KHz(168M/294)
	//װ���ظ�����ֵ
	IR_TIM->RCR = s_vu16EmitTimeArray[0];
	//�Զ���װ
	TIM_ARRPreloadConfig(IR_TIM, ENABLE);						//ʹ���Զ���װ
	
	//�رպ������
	IR_RxCmd(DISABLE);

	//��ʼ��������
	IR_TIM->CNT = 0x0000;										//��ʱ���ļ���������
	TIM_ClearFlag(IR_TIM, TIM_FLAG_Update);						//������±�ʶ
	//ʹ�ܶ�ʱ�������
	TIM_CtrlPWMOutputs(IR_TIM, ENABLE);							//��ʼ���������ź�
	//ʹ�ܶ�ʱ��
	TIM_Cmd(IR_TIM, ENABLE);									//ʹ�ܶ�ʱ��
	while(!IR_TIM->CNT && ((++i)<100));							//�ȴ�������ʼ
	//װ����һ�ε��ظ�����ֵ�����������¼�������
	IR_TIM->RCR = s_vu16EmitTimeArray[s_vu32ArrayPt];
//#endif
}
/***********************************************************
** ��������	 IR_TIM_CC_IRQHandler
** �������������ⷢ���жϳ���ʹ����Toggle����
************************************************************/
void IR_TIM_CC_IRQHandler(void)
{
#if OS_CRITICAL_METHOD == 3                      /* Allocate storage for CPU status register */
    OS_CPU_SR  cpu_sr = 0;
#endif
//#ifndef DYK_SS
	u32 capture;
//#endif
	
	OS_ENTER_CRITICAL();                         /* Tell uC/OS-II that we are starting an ISR */
	OSIntEnter();
	OS_EXIT_CRITICAL();

//#ifndef DYK_SS
	if(TIM_GetITStatus(IR_TIM,TIM_IT_CC1) == SET)				//�ж��Ƿ�����ʱ������ж�
	{
		TIM_ClearITPendingBit(IR_TIM, TIM_IT_CC1);			//��ʱ������жϴ�����������
		// ���IR_TIM���벶��1��ֵ
		capture = TIM_GetCapture1(IR_TIM);
		// ����IR_TIM����Ƚ�1�Ĵ���ֵ
		// IR_TIM������ֵ���ϼ���
		TIM_SetCompare1(IR_TIM, capture + TOGGLE_STEP);
		// �ۼ�Toggle����
		s_vu32PWMCount++;
		// ����0����1��һ�� 
		if ((s_vu16EmitTimeArray[s_vu32ArrayPt] * 2) == s_vu32PWMCount)
		{
			s_vu32PWMCount = 0;
			s_vu32ArrayPt++;
			TIM_CtrlPWMOutputs(IR_TIM, (s_vu32ArrayPt & 0x1) ? DISABLE : ENABLE);// ż����,������������
			if (s_vu32ArrayPt >= 69) 
			{
				TIM_Cmd(IR_TIM, DISABLE);
				s_vu32EmitCompleted = TRUE;					//IR�������
				//ʹ�ܺ������
				IR_RxCmd(ENABLE);
			}	
		}
	}
//#endif
	OSIntExit();
}
/***********************************************************
** ��������	 IR_TIM_IRQHandler
** �������������ⷢ���жϳ���ʹ����PWM�ķ���������ʹ���ظ�����������
************************************************************/
void IR_TIM_IRQHandler(void)
{
#if OS_CRITICAL_METHOD == 3                      /* Allocate storage for CPU status register */
    OS_CPU_SR  cpu_sr = 0;
#endif

	OS_ENTER_CRITICAL();                         /* Tell uC/OS-II that we are starting an ISR */
	OSIntEnter();
	OS_EXIT_CRITICAL();

//#ifndef DYK_SS
///* ʹ���ظ�����������
	if(TIM_GetITStatus(IR_TIM,TIM_IT_Update) == SET)				//�ж��Ƿ�����ʱ������ж�
	{
		TIM_ClearITPendingBit(IR_TIM, TIM_IT_Update);			//��ʱ������жϴ�����������

 		if ((s_vu32ArrayPt == 0) 
		|| (s_vu32ArrayPt == 68) 
		|| ((s_vu32ArrayPt & 0x1) == 1))
			TIM_CtrlPWMOutputs(IR_TIM, ENABLE);					//���������Ա��������
		else
			TIM_CtrlPWMOutputs(IR_TIM, DISABLE);				//˫�������Ա����������
		if(s_vu32ArrayPt >= 72)									//���1��������������1��idle̬����Ҫ�����
		{
			TIM_Cmd(IR_TIM, DISABLE);							//�ź��������,�ر�
			IR_TIM->CNT = 0x0000;								//��ʱ���ļ���������
			s_vu32EmitCompleted = TRUE;							//IR�������
			//ʹ�ܺ������
			IR_RxCmd(ENABLE);
		}else
		{
			u32 i=0;
			s_vu32ArrayPt++;
			while(!IR_TIM->CNT && ((++i)<100));					//�ȴ�������ʼ
			//װ����һ�ε��ظ�����ֵ�����������¼�������
			IR_TIM->RCR = s_vu16EmitTimeArray[s_vu32ArrayPt];
		}
	}
	OSIntExit();
}
/***********************************************************
** ��������IsCompletedForIREmit
** �䡡�룺None
** �䡡����TRUE=IR������ɣ�FALSE=IR������
** ������������ȡIR������ɱ�־
************************************************************/
u32 IsCompletedForIREmit(void)
{
	u32	i;
#if OS_CRITICAL_METHOD == 3                      /* Allocate storage for CPU status register */
    OS_CPU_SR  cpu_sr = 0;
#endif

	OS_ENTER_CRITICAL();                         /* Tell uC/OS-II that we are starting an ISR */
	i = s_vu32EmitCompleted;
	OS_EXIT_CRITICAL();
	return(i);
}
/***********************************************************
** ��������IR_EmitProc
** �䡡�룺Wl_EmitCan����չCAN��ʽ��������
** �䡡����TRUE=���ͣ�FALSE=����
** ����������IR�������ݴ���Ӧ�������������ڵ��øú���
************************************************************/
u32 IR_EmitProc(EXT_CAN_FRAME_s *IR_EmitCan)
{
//#ifndef DYK_SS
	u32	i,j,k;
	IR_INFO_u	data;
	u8	c;

	if ((IR_EmitCan->u32ID.ID.FT != V4YKQ_IR_CODEMATCH_ACK) 
		&& (IR_EmitCan->u32ID.ID.FT != V4YKQ_IR_CODEMATCH))
		return(FALSE);
	if (IR_EmitCan->u32ID.ID.FT == V4YKQ_IR_CODEMATCH_ACK) 
	{
		data.sIrInfo.Type = 2;									//��������
		data.sIrInfo.ScNoLSB3 = (((u32)IR_EmitCan->u8DT[4] << 8) + IR_EmitCan->u8DT[3]) & 0x7;//����ܺŵ�3λ
		data.sIrInfo.Sign1 = 1;									//��ʼ��ʶ
		data.sIrInfo.ScNoMSB6 = ((((u32)IR_EmitCan->u8DT[4] << 8) + IR_EmitCan->u8DT[3]) >> 3) & 0x3f;//����ܺŵ�6λ
		data.sIrInfo.Dir = 1;									//���ͷ���
		data.sIrInfo.Sign2 = 0;									//�м��ֽڱ�ʶ
		data.sIrInfo.Result = IR_EmitCan->u8DT[2] & 0x01;		//������
		data.sIrInfo.ACK = 0;									//Ӧ���ʶ
		data.sIrInfo.RemoteID = IR_EmitCan->u8DT[5] & 0xf;		//ң����ID
		data.sIrInfo.ScNoIncDir = IR_EmitCan->u8DT[1] & 0x01;	//֧�ܿ������ܺ�����
		data.sIrInfo.Sign3 = 0;									//�м��ֽڱ�ʶ
	}
	else
	{
///* for test
		data.sIrInfo.Type = 1;									//��������
		data.sIrInfo.ScNoLSB3 = 0;								//����ܺŵ�3λ
		data.sIrInfo.Sign1 = 1;									//��ʼ��ʶ
		data.sIrInfo.ScNoMSB6 = 0;								//����ܺŵ�6λ
		data.sIrInfo.Dir = 0;									//���ͷ���
		data.sIrInfo.Sign2 = 0;									//�м��ֽڱ�ʶ
		data.sIrInfo.Result = 0;								//������
		data.sIrInfo.ACK = 0;									//Ӧ���ʶ
		data.sIrInfo.RemoteID = 1;								//ң����ID
		data.sIrInfo.ScNoIncDir = 0;							//֧�ܿ������ܺ�����
		data.sIrInfo.Sign3 = 0;									//�м��ֽڱ�ʶ
//*/
	}

	k = 0;
///* �ظ���������
 	s_vu16EmitTimeArray[k++] = 171;								//k=0,k=1���������Ա����9ms����������
 	s_vu16EmitTimeArray[k++] = 170;								//k=0,k=1���������Ա����9ms����������

	s_vu16EmitTimeArray[k++] = 170;								//��3�����������Ա����4.5ms�ĵ͵�ƽ
	for (i = 0; i < 4; i++)
	{
		if (i == 3)												//���ⷢ������ֻ����4���ֽڣ����У����һ���ǵ�����2���ķ���
		{
			c = data.u8IrInfo[i - 1];
			c = ~c;
		}
		else
			c = data.u8IrInfo[i];
		for (j = 0; j < 8; j++)
		{
			if((c >> j) & 1)									//Bit=1ʱ��Carrier=0.56ms, Idle=1.68ms
			{
				s_vu16EmitTimeArray[k++] = 20;
				s_vu16EmitTimeArray[k++] = 63;
			}else												//Bit=0ʱ��Carrier=0.56ms, Idle=0.56ms
			{
				s_vu16EmitTimeArray[k++] = 20;
				s_vu16EmitTimeArray[k++] = 20;
			}
		}
	}
///* �ظ���������
 	s_vu16EmitTimeArray[k++] = 120;
 	s_vu16EmitTimeArray[k++] = 110;
 	s_vu16EmitTimeArray[k++] = 110;
//*/
/* ��PWM������TOGGLE��
	s_vu16EmitTimeArray[k++]=342;								//���ͽ�����
*/
	s_vu16EmitTimeArray[k++] = 85;
	s_vu16EmitTimeArray[k++] = 21;
	s_vu16EmitTimeArray[k++] = 100;								//�����
	s_vu32PWMCount = 0;
	s_vu32ArrayPt = 0;
	
///* �ظ���������
	IREmitEnabled();											//��������

	return(TRUE);
}

