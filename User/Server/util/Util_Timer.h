/*
*********************************************************************************************************
*	                                            V4
*                           Commuication Controller For The Mining Industry
*
*                                  (c) Copyright 1994-2013  HNDZ 
*                       All rights reserved.  Protected by international copyright laws.
*
*
*    File    : Bsp_Time.h
*    Module  : user
*    Version : V1.0
*    History :
*   -----------------
*              Version  Date           By            Note
*              v1.0         
*
*********************************************************************************************************
*/

#ifndef __UTIL_TIME_H__
#define __UTIL_TIME_H__
#include  "bsp_time.h"


#define TIMEREVENT   25


typedef struct {
	uint16_t     timer;
	uint8_t      mode;
	uint8_t      count;
	uint16_t     backTimer;
	BspTimerCallback     Callback;
} Util_Timer;

typedef struct {
	uint8_t id[TIMEREVENT];
	uint16_t value[TIMEREVENT];
} Post_Para_UtilTimer;



enum{
	TIMER_NOUSE = 0,
	TIMER_STOP  = 1,
	TIMER_START = 2,
};


void UtilTimerInit(TIM_TypeDef* TIMx);
uint16_t addTimerEvent(uint16_t internal_value, uint8_t count, BspTimerCallback  Callback);
void startTimerEvent(uint16_t id);
void stopTimerEvent(uint16_t id);
void deleteTimerEvent(uint16_t id);
void Util_Timer_Task(void);
#endif


