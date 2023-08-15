/*
*********************************************************************************************************
*	                                            TK100+
*                           Commuication Controller For The Mining Industry
*
*                                  (c) Copyright 1994-2013  HNDZ
*                       All rights reserved.  Protected by international copyright laws.
*
*
*    File    : bsp_rtc.c
*    Module  : user
*    Version : V1.0
*    History :
*   -----------------
*              Version  Date           By            Note
*              v1.0     2013-09-05    辛鑫
*
*********************************************************************************************************
*/
#include "bsp.h"
#include <string.h>

/* RTC时钟源选择 */
/* #define RTC_CLOCK_SOURCE_LSI */ /* LSI used as RTC source clock. The RTC Clock
                                      may varies due to LSI frequency dispersion. */
#define RTC_CLOCK_SOURCE_LSE   /* LSE used as RTC source clock */

#define  RTC_SET_FLAG      0x32F2             // RTC设置标志
#define  RTC_YEAR_BASE       2000

#define  RTC_AlarmMaskExceptSec       ((uint32_t)0x80808000)

static int  RTC_Config(void);
static void InitTimeDataVar(RTC_TimeData *rtcValue);

/*
*********************************************************************************************************
*	函 数 名: bsp_InitRtc
*	功能说明: 初始化RTC时钟. 该函数被 bsp_Init() 调用。
*	形    参：无
*	返 回 值: 正数为正常
*********************************************************************************************************
*/
int BSP_InitRtc(void)
{
    RTC_InitTypeDef RTC_InitStructure;
    RTC_TimeData    timaData;
    /* RTC configuration  */
    if (RTC_Config() < 0)
    {
        return (-1);
    }

    /* Configure the RTC data register and RTC prescaler */
    RTC_InitStructure.RTC_AsynchPrediv = 0x7F;               // 异步预分频器值，不得大于0x7F
    RTC_InitStructure.RTC_SynchPrediv  = 0xFF;               // 同步预分频器值，不得大于0x1FFF
    RTC_InitStructure.RTC_HourFormat   = RTC_HourFormat_24;  // 24小时制

    /* Check on RTC init */
    if (RTC_Init(&RTC_InitStructure) == ERROR)
    {
		return (-1);
    }

    if (RTC_ReadBackupRegister(RTC_BKP_DR0) != RTC_SET_FLAG)
    {
			RUN_TRACE_INFO("The rtc time had lose!\r\n");
			InitTimeDataVar(&timaData);		/* 初始化日期时钟 */
	    Set_Data_Time(timaData);		/* 设置系统时间   */
    }

	return (1);
}

static int RTC_Config(void)
{
    ErrorStatus status = ERROR;
    __IO uint32_t timeOut  = 0;
    /* Enable the PWR clock */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);

    /* Allow access to RTC */
    PWR_BackupAccessCmd(ENABLE);

#if defined (RTC_CLOCK_SOURCE_LSI)  /* LSI used as RTC source clock*/
    /* The RTC Clock may varies due to LSI frequency dispersion. */
    /* Enable the LSI OSC */
    RCC_LSICmd(ENABLE);

    /* Wait till LSI is ready */
    while (RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET)
    {
        timeOut++;
        if (timeOut > RTCTIMEOUT)
        {
            return (-1);
        }
    }

    /* Select the RTC Clock Source */
    RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);

#elif defined (RTC_CLOCK_SOURCE_LSE) /* LSE used as RTC source clock */
    /* Enable the LSE OSC */
    RCC_LSEConfig(RCC_LSE_ON);

    /* Wait till LSE is ready */
    while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
    {
        timeOut++;
        if (timeOut > RTCTIMEOUT)
        {
            RUN_TRACE("Wait LSE clock timeout!\r\n");
			return (-1);
        }
    }

    /* Select the RTC Clock Source */
    RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);

#else
  #error Please select the RTC Clock source inside the bsp_rtc.c file
#endif /* RTC_CLOCK_SOURCE_LSI */

	RTC_AlarmCmd(RTC_Alarm_A, DISABLE);
	RTC_AlarmCmd(RTC_Alarm_B, DISABLE);
    /* Enable the RTC Clock */
    RCC_RTCCLKCmd(ENABLE);

    /* Wait for RTC APB registers synchronisation */
    status = RTC_WaitForSynchro();

    if (status == ERROR)
    {
		return (-1);
    }
    return (1);
}

static void InitTimeDataVar(RTC_TimeData *rtcValue)
{
    rtcValue->sys_year = 2021;

	rtcValue->sys_time.RTC_Hours   = 5;
    rtcValue->sys_time.RTC_Minutes = 20;
    rtcValue->sys_time.RTC_Seconds = 0;

    rtcValue->sys_data.RTC_WeekDay = 5;
    rtcValue->sys_data.RTC_Date    = 18;
    rtcValue->sys_data.RTC_Month   = 2;
}

void Set_Data_Time(RTC_TimeData rtcValue)
{
    rtcValue.sys_data.RTC_Year = (uint8_t)(rtcValue.sys_year - RTC_YEAR_BASE); // 实际需要加上2000

    RTC_SetTime(RTC_Format_BIN, &rtcValue.sys_time);
    RTC_SetDate(RTC_Format_BIN, &rtcValue.sys_data);

    /* Indicator for the RTC configuration */
    RTC_WriteBackupRegister(RTC_BKP_DR0, RTC_SET_FLAG);
}

void Set_Data_SysDate(RTC_TimeData rtcValue)
{
    rtcValue.sys_data.RTC_Year = (uint8_t)(rtcValue.sys_year - RTC_YEAR_BASE); // 实际需要加上2000

    RTC_SetDate(RTC_Format_BIN, &rtcValue.sys_data);

    /* Indicator for the RTC configuration */
    RTC_WriteBackupRegister(RTC_BKP_DR0, RTC_SET_FLAG);
}

void Set_Data_SysTime(RTC_TimeData rtcValue)
{
    RTC_SetTime(RTC_Format_BIN, &rtcValue.sys_time);

    /* Indicator for the RTC configuration */
    RTC_WriteBackupRegister(RTC_BKP_DR0, RTC_SET_FLAG);
}

void Get_Data_Time(RTC_TimeData *rtcValue)
{
    RTC_GetDate(RTC_Format_BIN, &(rtcValue->sys_data));
    RTC_GetTime(RTC_Format_BIN, &(rtcValue->sys_time));
	
	rtcValue->sys_year = rtcValue->sys_data.RTC_Year + RTC_YEAR_BASE;
}

int8_t RTC_AlarmXConfig(void)
{
    RTC_AlarmTypeDef RTC_AlarmStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
	
	/* EXTI configuration */
    EXTI_ClearITPendingBit(EXTI_Line17);
    EXTI_InitStructure.EXTI_Line = EXTI_Line17;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);
	
    /* Enable the RTC Alarm Interrupt */
	BSP_IntVectSet(RTC_Alarm_IRQn, SOFT_Alarm_IRQHandler);
	
    NVIC_InitStructure.NVIC_IRQChannel = RTC_Alarm_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    /* Set the alarmA Masks */
	RTC_AlarmStructure.RTC_AlarmDateWeekDaySel = RTC_AlarmDateWeekDaySel_Date;  // 忽略星期
    RTC_AlarmStructure.RTC_AlarmMask = RTC_AlarmMaskExceptSec;
    RTC_AlarmStructure.RTC_AlarmTime.RTC_Seconds = 0x3B;    // 报警设置59秒
	
    RTC_SetAlarm(RTC_Format_BIN, RTC_Alarm_A, &RTC_AlarmStructure);
   
    /* Set AlarmA subseconds and enable SubSec Alarm : generate 8 interripts per Second */
    //RTC_AlarmSubSecondConfig(RTC_Alarm, 0xFF, RTC_AlarmSubSecondMask_SS14_5);
  
    if (RTC_GetFlagStatus(RTC_FLAG_ALRAWF) == RESET)
	{
		return (-1);
	}
		
	RTC_ClearFlag(RTC_FLAG_ALRAF);                  // 清除RTC闹钟标志
    /* Enable AlarmA interrupt */
    RTC_ITConfig(RTC_IT_ALRA, ENABLE);
    
	RTC_AlarmCmd(RTC_Alarm_A, ENABLE);
	
	return 1;
}

void SOFT_Alarm_IRQHandler(void)  
{ 
	/* Check on the AlarmA falg and on the number of interrupts per Second (1) */
    if (RTC_GetITStatus(RTC_IT_ALRA) != RESET)
    {
        /* Clear RTC AlarmA Flags */
        RTC_ClearITPendingBit(RTC_IT_ALRA);
        RTC_USART_ShowTime();
    }
    else if (RTC_GetITStatus(RTC_IT_ALRB) != RESET)
    {
        /* Clear RTC AlarmA Flags */
        RTC_ClearITPendingBit(RTC_IT_ALRB);
    }
	EXTI_ClearITPendingBit(EXTI_Line17);
}

void RTC_USART_ShowTime(void)
{
    RTC_TimeData CurrentTimeDate;
	char SerialPortBuffer[29];

    Get_Data_Time(&CurrentTimeDate);                         // 读取当前时间日期

	RTCTimeDateToString(SerialPortBuffer, &CurrentTimeDate); // 转换时间日期为字符串
	RUN_TRACE("      \r\n");
    RUN_TRACE("%s\r\n", SerialPortBuffer);
}

static uint8_t IsLeapYear(uint16_t year) 
{
	return (year % 400==0 || ( year %4==0 && year %100 !=0 ));
}

static uint16_t GetDaysOfMonth(uint16_t year, uint8_t month)
{
	uint16_t i = 0;
	switch (month-1)
	{
		case 11:
			i += 30;
		case 10:
			i += 31;
		case 9:
			i += 30;
		case 8:
			i += 31;
		case 7:
			i += 31;
		case 6:
			i += 30;
		case 5:
			i += 31;
		case 4:
			i += 30;
		case 3:
			i += 31;
		case 2:
			i = (IsLeapYear(year))?(i + 29):(i + 28);
		case 1:
			i += 31;
	}
	return i;
}

uint32_t Time_To_Secs(RTC_TimeTypeDef time)
{
	return (time.RTC_Hours*3600 + time.RTC_Minutes*60 + time.RTC_Seconds);
}

void Secs_To_RunTime(uint32_t secs, Run_TimeTypeDef *time)
{
	time->run_hours = secs / 3600;
	secs %= 3600;
	
	time->run_mins = secs / 60;
	time->run_secs = secs % 60;
	
	while (time->run_hours >= 24)
	{
		time->run_days++;
		time->run_hours -= 24;
	}
}

/*
*********************************************************************************************************
*	函 数 名: Get_Run_Time
*	功能说明: 计算从起始时间到现在的运行时间。
*	形    参：startTimeData为起始时间，Run_Time为已运行的时间，hours32为32位小时数
*	返 回 值: 1为正常，如起始时间大于系统时间则失败，返回-1，返回0则运行时间为0
*********************************************************************************************************
*/
int8_t Get_Run_Time(RTC_TimeData startTimeData, Run_TimeTypeDef *Run_Time)
{
	RTC_TimeData CurrentTimeDate;
	int8_t res = 0;
	uint32_t i = 0;
	Get_Data_Time(&CurrentTimeDate);                   // 读取当前时间日期
	
	res = Cmp_Two_Time(startTimeData, CurrentTimeDate);
	if (res > 0)
	{
		return (-1);
	}
	else if (res == 0)     // 时间相等
	{
		return 0;
	}
	
	/* 计算天数差 */
	Run_Time->run_days = 0;
	for (i = startTimeData.sys_year; i < CurrentTimeDate.sys_year; i++)
	{
		Run_Time->run_days += ((IsLeapYear(i)) ? 366 : 365);
	}
	
	Run_Time->run_days += GetDaysOfMonth(CurrentTimeDate.sys_year, CurrentTimeDate.sys_data.RTC_Month);
	for (i = 1; i < CurrentTimeDate.sys_data.RTC_Date; i++)
	{
		Run_Time->run_days++;
	}
	
	Run_Time->run_days -= GetDaysOfMonth(startTimeData.sys_year, startTimeData.sys_data.RTC_Month);
	for (i = 1; i <= startTimeData.sys_data.RTC_Date; i++)
	{
		Run_Time->run_days--;
	}
    
	/* 计算时间差 */
	i = 86400 - Time_To_Secs(startTimeData.sys_time);  // 86400 = 24*3600
	i += Time_To_Secs(CurrentTimeDate.sys_time);
	
	Secs_To_RunTime(i, Run_Time);
	return 1;
}

/*
*********************************************************************************************************
*	函 数 名: Cmp_Two_Time
*	功能说明: 比较两个时间的大小
*	形    参：两个时间
*	返 回 值: 1为大于，-1为小于，返回0则相等
*********************************************************************************************************
*/
int8_t Cmp_Two_Time(RTC_TimeData firstTimeData, RTC_TimeData secondTimeData)
{
	/* 比较年 */
	if (firstTimeData.sys_year > secondTimeData.sys_year)
	{
		return 1;
	}
	else if (firstTimeData.sys_year < secondTimeData.sys_year)
	{
		return (-1);
	}
	/* 比较月 */
	else if (firstTimeData.sys_data.RTC_Month > secondTimeData.sys_data.RTC_Month)
	{
		return 1;
	}
	else if (firstTimeData.sys_data.RTC_Month < secondTimeData.sys_data.RTC_Month)
	{
		return (-1);
	}
	/* 比较日 */
	else if (firstTimeData.sys_data.RTC_Date > secondTimeData.sys_data.RTC_Date)
	{
		return 1;
	}
	else if (firstTimeData.sys_data.RTC_Date < secondTimeData.sys_data.RTC_Date)
	{
		return (-1);
	}
	
	/* 比较小时 */
	if (firstTimeData.sys_time.RTC_Hours > secondTimeData.sys_time.RTC_Hours)
	{
		return 1;
	}
	else if (firstTimeData.sys_time.RTC_Hours < secondTimeData.sys_time.RTC_Hours)
	{
		return (-1);
	}
	/* 比较分钟 */
	else if (firstTimeData.sys_time.RTC_Minutes > secondTimeData.sys_time.RTC_Minutes)
	{
		return 1;
	}
	else if (firstTimeData.sys_time.RTC_Minutes < secondTimeData.sys_time.RTC_Minutes)
	{
		return (-1);
	}
	/* 比较秒 */
	else if (firstTimeData.sys_time.RTC_Seconds > secondTimeData.sys_time.RTC_Seconds)
	{
		return 1;
	}
	else if (firstTimeData.sys_time.RTC_Seconds < secondTimeData.sys_time.RTC_Seconds)
	{
		return (-1);
	}
	
	return 0;
}

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//函数名称：时间转字符串
//参数值：
//      @Target：目标字符串（需要9个字节）
//      @Source：源时间结构体
//返回值：无
//说明：时间字符串格式为“hh:mm:ss”其中hh分别在Target[0]
//      与Target[1]，不足位数的需要补0。
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
void RTCTimeToString(char *Target, RTC_TimeTypeDef *Source)
{
	sprintf(Target,"%02d:%02d:%02d",(int)(Source->RTC_Hours),(int)(Source->RTC_Minutes),(int)(Source->RTC_Seconds));
}

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//函数名称：日期转字符串
//参数值：
//      @Target：目标字符串（需要11个字节）
//      @Source：源日期结构体
//返回值：无
//说明：时间字符串格式为“yyyy-mm-dd”其中yyyy分别在Source[0]
//      、Source[1]、Source[2]、Source[3]，不足位数的需要补0。
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
void RTCDateToString(char *Target, RTC_DateTypeDef *Source)
{
	sprintf(Target,"%04d-%02d-%02d",((int)Source->RTC_Year + RTC_YEAR_BASE),(int)(Source->RTC_Month),(int)(Source->RTC_Date));
}

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//函数名称：时间日期转字符串
//参数值：
//      @Target：目标字符串（需要29个字节）
//      @Source：源时间日期结构体
//返回值：
//      @0：转换成功
//      @1：转换失败
//说明：时间日期字符串格式为“yyyy-mm-dd,hh:mm:ss”其中yyyy分别在
//      Source[0]、Source[1]、Source[2]、Source[3]，不足位数的
//      需要补0。
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
void RTCTimeDateToString(char *Target, RTC_TimeData *Source)
{
	strcpy(&Target[0], "Date:");
	RTCDateToString(&Target[5], &(Source->sys_data));
	strcpy(&Target[15], "Time:");
	RTCTimeToString(&Target[20], &(Source->sys_time));
}

