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
**函数名称：BspTimer_Init
**函数作用：定时器初始化函数
**函数参数：TIMx：定时器（2、3、4、5、6、7），Period：时间
**函数输出：无
**注意事项：无
*******************************************************************************************/
void BspTimer_Init(TIM_TypeDef* TIMx, uint16_t Period, BspTimerCallback Callback)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;				             //定时器初始化结构
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
	RCC_GetClocksFreq(&rcc_clocks);					//调用标准库函数，获取系统时钟。

 	TIM_TimeBaseStructure.TIM_Period =Period;     					         
	TIM_TimeBaseStructure.TIM_Prescaler = 859;       				         //时钟分频
	TIM_TimeBaseStructure.TIM_ClockDivision =TIM_CKD_DIV1;   		         //时钟分割
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;		         //模式,向上计数模式
	TIM_TimeBaseInit(TIMx, &TIM_TimeBaseStructure);					         //按结构体的值，初始化定时器
	
	NVIC_InitStructure.NVIC_IRQChannel = TIMx_IRQn;                          
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0;                // 设置成员 NVIC_IRQChannel中的先占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;                       // 设置成员 NVIC_IRQChannel中的从优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = (FunctionalState)ENABLE;         // 使能 NVIC_IRQChannel
	NVIC_Init(&NVIC_InitStructure);                                          // 初始化外设 NVIC 寄存器
	
	TIMx->CNT=0;

	TIM_ClearITPendingBit(TIMx, TIM_IT_Update);
	TIM_ClearFlag(TIMx, TIM_FLAG_Update);	
	TIM_SetCounter(TIMx,0);
	TIM_ITConfig(TIMx, TIM_IT_Update, (FunctionalState)ENABLE);				// 使能中断请求
	TIM_Cmd(TIMx, (FunctionalState)DISABLE);
		
	if(Callback != NULL)
	{
		BspCallback = Callback;
	}
}

/*******************************************************************************************
**函数名称：BspTimer_Control
**函数作用：控制tim的启动，停止
**函数参数：TIMx：定时器，state：启动、停止
**函数输出：无
**注意事项：无
*******************************************************************************************/
void BspTimer_Ctrl(TIM_TypeDef* TIMx, FunctionalState state)
{
	TIM_ClearITPendingBit(TIMx, TIM_IT_Update);
	TIM_ClearFlag(TIMx, TIM_FLAG_Update);	
	TIM_SetCounter(TIMx,0);
	TIM_Cmd(TIMx, state);
}

/*******************************************************************************************
**函数名称：TIM5_IRQHandler
**函数作用：TIM5定时中断
**函数参数：无
**函数输出：无
**注意事项：无
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





