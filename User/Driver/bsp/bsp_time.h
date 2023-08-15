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

#ifndef __BSP_TIME_H__
#define __BSP_TIME_H__


typedef void (*BspTimerCallback)(uint16_t);

void BspTimer_Init(TIM_TypeDef* TIMx, uint16_t Period, BspTimerCallback Callback);
void BspTimer_Ctrl(TIM_TypeDef* TIMx, FunctionalState state);

#endif


