/*
*********************************************************************************************************
*	                                                 V4
*                           Commuication Controller For The Mining Industry
*
*                                  (c) Copyright 1994-2013  HNDZ 
*                       All rights reserved.  Protected by international copyright laws.
*
*
*    File    : BspTimer_Init.c
*    Module  : user
*    Version : V1.0
*    History :
*   -----------------
*              Version  Date           By            Note
*              v1.0     2013-08-15     
*
*********************************************************************************************************
*/
#include "includes.h"
#include "bsp_time.h"


BspTimerCallback BspCallback = NULL;
/*******************************************************************************************
**�������ƣ�BspTimer_Init
**�������ã���ʱ����ʼ������
**����������TIMx����ʱ����2��3��4��5��6��7����Period��ʱ��
**�����������
**ע�������
*******************************************************************************************/
void BspTimer_Init(TIM_TypeDef* TIMx, uint16_t Period, BspTimerCallback Callback)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;				             //��ʱ����ʼ���ṹ
	NVIC_InitTypeDef  NVIC_InitStructure;  
	RCC_ClocksTypeDef  rcc_clocks; 
	uint8_t TIMx_IRQn;

	if (TIMx == TIM2)
	{
		RCC_APB1PeriphClockCmd (RCC_APB1Periph_TIM2, ENABLE);
		TIMx_IRQn = TIM2_IRQn;
	}
	else if(TIMx == TIM3)
	{
		RCC_APB1PeriphClockCmd (RCC_APB1Periph_TIM3, ENABLE);
		TIMx_IRQn = TIM4_IRQn;
	}	
	else if(TIMx == TIM4)
	{
		RCC_APB1PeriphClockCmd (RCC_APB1Periph_TIM4, ENABLE);
		TIMx_IRQn = TIM4_IRQn;
	}	
	else if(TIMx == TIM5)
	{
		RCC_APB1PeriphClockCmd (RCC_APB1Periph_TIM5, ENABLE);
		TIMx_IRQn = TIM5_IRQn;
	}	
	else if(TIMx == TIM6)
	{
		RCC_APB1PeriphClockCmd (RCC_APB1Periph_TIM6, ENABLE);
		TIMx_IRQn = TIM6_DAC_IRQn;
	}
	else if(TIMx == TIM7)
	{
		RCC_APB1PeriphClockCmd (RCC_APB1Periph_TIM7, ENABLE);
		TIMx_IRQn = TIM7_IRQn;
	}
	else
		return;
	
	TIM_DeInit(TIMx);
	RCC_GetClocksFreq(&rcc_clocks);					//���ñ�׼�⺯������ȡϵͳʱ�ӡ�

 	TIM_TimeBaseStructure.TIM_Period =Period;     					         
	TIM_TimeBaseStructure.TIM_Prescaler = 859;       				         //ʱ�ӷ�Ƶ
	TIM_TimeBaseStructure.TIM_ClockDivision =TIM_CKD_DIV1;   		         //ʱ�ӷָ�
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;		         //ģʽ,���ϼ���ģʽ
	TIM_TimeBaseInit(TIMx, &TIM_TimeBaseStructure);					         //���ṹ���ֵ����ʼ����ʱ��
	
	NVIC_InitStructure.NVIC_IRQChannel = TIMx_IRQn;                          
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0;                // ���ó�Ա NVIC_IRQChannel�е���ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;                       // ���ó�Ա NVIC_IRQChannel�еĴ����ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd = (FunctionalState)ENABLE;         // ʹ�� NVIC_IRQChannel
	NVIC_Init(&NVIC_InitStructure);                                          // ��ʼ������ NVIC �Ĵ���
	
	TIMx->CNT=0;

	TIM_ClearITPendingBit(TIMx, TIM_IT_Update);
	TIM_ClearFlag(TIMx, TIM_FLAG_Update);	
	TIM_SetCounter(TIMx,0);
	TIM_ITConfig(TIMx, TIM_IT_Update, (FunctionalState)ENABLE);				// ʹ���ж�����
	TIM_Cmd(TIMx, (FunctionalState)DISABLE);
		
	if(Callback != NULL)
	{
		BspCallback = Callback;
	}
}

/*******************************************************************************************
**�������ƣ�BspTimer_Control
**�������ã�����tim��������ֹͣ
**����������TIMx����ʱ����state��������ֹͣ
**�����������
**ע�������
*******************************************************************************************/
void BspTimer_Ctrl(TIM_TypeDef* TIMx, FunctionalState state)
{
	TIM_ClearITPendingBit(TIMx, TIM_IT_Update);
	TIM_ClearFlag(TIMx, TIM_FLAG_Update);	
	TIM_SetCounter(TIMx,0);
	TIM_Cmd(TIMx, state);
}

/*******************************************************************************************
**�������ƣ�TIM5_IRQHandler
**�������ã�TIM5��ʱ�ж�
**������������
**�����������
**ע�������
*******************************************************************************************/
void TIM5_IRQHandler(void)
{
	uint16_t id = 0;
	OSIntEnter();
	if(TIM_GetITStatus(TIM5,TIM_IT_Update )!= RESET)
	{
		TIM_ClearITPendingBit(TIM5, TIM_IT_Update);
		TIM_ClearFlag(TIM5, TIM_FLAG_Update);	
		
		BspCallback(id);
		
//		TIM_SetCounter(TIM5,0);
	}
	OSIntExit();
}





