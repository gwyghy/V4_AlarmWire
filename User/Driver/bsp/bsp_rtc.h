/*
*********************************************************************************************************
*	                                            TK100+
*                           Commuication Controller For The Mining Industry
*
*                                  (c) Copyright 1994-2013  HNDZ
*                       All rights reserved.  Protected by international copyright laws.
*
*
*    File    : bsp_rtc.h
*    Module  : user
*    Version : V1.0
*    History :
*   -----------------
*              Version  Date           By            Note
*              v1.0     2013-09-05    辛鑫
*
*********************************************************************************************************
*/
#ifndef __BSP_RTC_H
#define __BSP_RTC_H


#define RTCTIMEOUT                  15000000  /* 等待时间超时 */

typedef struct
{
    uint16_t        sys_year;        // 底层年寄存器为8位，此变量作为转换
	RTC_TimeTypeDef sys_time;
    RTC_DateTypeDef sys_data;
} RTC_TimeData;

typedef struct
{
    uint32_t        run_days;        
	uint8_t         run_hours;
    uint8_t         run_mins;
	uint8_t         run_secs;
} Run_TimeTypeDef;                   // 此结构体用于计算运行时间

int  BSP_InitRtc(void);
void Set_Data_Time(RTC_TimeData rtcValue);
void Get_Data_Time(RTC_TimeData *rtcValue);
void SOFT_Alarm_IRQHandler(void);

int8_t RTC_AlarmXConfig(void);
void RTC_USART_ShowTime(void);
void RTCTimeDateToString(char *Target, RTC_TimeData *Source);

int8_t Get_Run_Time(RTC_TimeData startTimeData, Run_TimeTypeDef *Run_Time);
int8_t Cmp_Two_Time(RTC_TimeData firstTimeData, RTC_TimeData secondTimeData);

void Set_Data_SysDate(RTC_TimeData rtcValue);
void Set_Data_SysTime(RTC_TimeData rtcValue);

#endif
