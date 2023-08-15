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
#include "Bsp_Time.h"
#include "Util_Timer.h"

Util_Timer       Utiltimer[TIMEREVENT];	//�洢�ķ���ֵ����
uint16_t         u16TimerCnt;
Post_Para_UtilTimer postPara;
/*******************************************************************************************
**�������ƣ�UtilTimerCallback
**�������ã���ʱ������ص�����
**������������
**�����������
**ע�������
*******************************************************************************************/
void UtilTimerCallback(uint16_t value)
{
	#if OS_CRITICAL_METHOD == 3u                               /* Allocate storage for CPU status register */
		OS_CPU_SR  cpu_sr = 0u;
	#endif
	uint8_t i,j = 0;
	for(i = 0;i<TIMEREVENT;i++)
	{
		if(Utiltimer[i].mode == TIMER_STOP && Utiltimer[i].count == 0)//used timer and forget delete
		{
			OS_ENTER_CRITICAL();
			Utiltimer[i].timer = 0;
			Utiltimer[i].mode = 0;
			Utiltimer[i].count = 0;
			Utiltimer[i].backTimer = 0;
		//	Utiltimer[id].Callback = NULL;
			OS_EXIT_CRITICAL();
		}
		if(Utiltimer[i].mode == TIMER_START && Utiltimer[i].timer > 0)
		{
			Utiltimer[i].timer--;
			if(Utiltimer[i].timer == 0)
			{
				if(Utiltimer[i].count == 0XFF)
				{
					Utiltimer[i].timer = Utiltimer[i].backTimer;
				}
				else if(Utiltimer[i].count != 0)
				{
					Utiltimer[i].timer = Utiltimer[i].backTimer;
					Utiltimer[i].count--;
				}
				if(Utiltimer[i].count == 0)
					Utiltimer[i].mode = TIMER_STOP;
				
				postPara.id[i] = 1;
				postPara.value[i] = value;
				j++;
			}
		}
	}
	if(j)
	{
		OSSemPost(UtilTimerTaskSendSem);	
	}
}

/*******************************************************************************************
**�������ƣ�UtilTimerInit
**�������ã�util�������ݳ�ʼ��
**����������TIMx ����ʱ��
**�����������
**ע�������
*******************************************************************************************/
void UtilTimerInit(TIM_TypeDef* TIMx)
{
	UtilTimerTaskSendSem = OSSemCreate(0);
	memset(Utiltimer,0,sizeof(Utiltimer));
	memset(&postPara,0,sizeof(Post_Para_UtilTimer));
	BspTimer_Init(TIMx,99,UtilTimerCallback);  //1ms
	BspTimer_Ctrl(TIMx,ENABLE);	
}

/*******************************************************************************************
**�������ƣ�addTimerEvent
**�������ã�util�������ݳ�ʼ��
**����������TIMx ����ʱ��
**�����������
**ע�������
*******************************************************************************************/
uint16_t addTimerEvent(uint16_t internal_value, uint8_t count, BspTimerCallback  Callback)
{
	uint8_t i;
	for(i = 0;i<TIMEREVENT;i++)
	{
		if(Utiltimer[i].mode > TIMER_NOUSE)
			continue;
		
		Utiltimer[i].mode = TIMER_STOP;
		Utiltimer[i].timer = internal_value;
		Utiltimer[i].count = count;
		Utiltimer[i].backTimer = internal_value;
		Utiltimer[i].Callback = Callback;
		
		return i;		
	}
	return 0xff;
}

/*******************************************************************************************
**�������ƣ�startTimerEvent
**�������ã���ʱ�����¼��ÿ�ʼ��־
**����������TIMx ����ʱ��
**�����������
**ע�������
*******************************************************************************************/
void startTimerEvent(uint16_t id)
{
	#if OS_CRITICAL_METHOD == 3u                               /* Allocate storage for CPU status register */
    OS_CPU_SR  cpu_sr = 0u;
	#endif
	OS_ENTER_CRITICAL();
	Utiltimer[id].timer = Utiltimer[id].backTimer;
	Utiltimer[id].mode = TIMER_START;
	OS_EXIT_CRITICAL();
}
/*******************************************************************************************
**�������ƣ�stopTimerEvent
**�������ã���ʱ�����¼���ֹͣ��־
**����������TIMx ����ʱ��
**�����������
**ע�������
*******************************************************************************************/
void stopTimerEvent(uint16_t id)
{
	#if OS_CRITICAL_METHOD == 3u                               /* Allocate storage for CPU status register */
    OS_CPU_SR  cpu_sr = 0u;
	#endif
	OS_ENTER_CRITICAL();
	Utiltimer[id].mode = TIMER_STOP;
	OS_EXIT_CRITICAL();
}

/*******************************************************************************************
**�������ƣ�deleteTimerEvent
**�������ã���ʱ����ɾ���¼�
**����������TIMx ����ʱ��
**�����������
**ע�������
*******************************************************************************************/
void deleteTimerEvent(uint16_t id)
{
	#if OS_CRITICAL_METHOD == 3u                               /* Allocate storage for CPU status register */
    OS_CPU_SR  cpu_sr = 0u;
	#endif
	OS_ENTER_CRITICAL();
	Utiltimer[id].timer = 0;
	Utiltimer[id].mode = 0;
	Utiltimer[id].count = 0;
	Utiltimer[id].backTimer = 0;
//	Utiltimer[id].Callback = NULL;
	OS_EXIT_CRITICAL();
}


void Util_Timer_Task(void)
{
	INT8U err;
	uint16_t i = 0;
	while (1) 
	{	
		OSSemPend(UtilTimerTaskSendSem,0,&err);
		if(err == OS_ERR_NONE)
		{
			for(i = 0; i < TIMEREVENT; i++)
			{
				if(postPara.id[i] == 1)
				{
					postPara.id[i] = 0;
					if(Utiltimer[i].Callback != NULL)
						(*Utiltimer[i].Callback)(postPara.value[i]);
				}
			}
		}
	}
}

