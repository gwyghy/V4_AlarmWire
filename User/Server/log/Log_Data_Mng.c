/*
*********************************************************************************************************
*	                                            V4
*                           Commuication Controller For The Mining Industry
*
*                                  (c) Copyright 1994-2013  HNDZ 
*                       All rights reserved.  Protected by international copyright laws.
*
*
*    File    : Log_Data_Mng.c
*    Module  : user
*    Version : V1.0
*    History :
*   -----------------
*              Version  Date           By            Note
*              v1.0         
*
*********************************************************************************************************
*/


#include "includes.h"
#include "Log_Data_Mng.h"
#include "udp_app_proc.h"
#include "sys_msg_def.h"
#include "sysParamAnalysis.h"

//故障数组
FAULT_INFO fault_info;
//日志数组
LOG_INFO log_info;


const    uint32_t Year_Secs_Accu[5]     = {0,31622400,63158400,94694400,126230400};          //一年有多少秒

const    uint32_t Month_Secs_Accu_C[13] = { 0,2678400,5097600,7776000,10368000,              //普通年一个月多少秒
                                            13046400,15638400,18316800,20995200,
                                            23587200,26265600,28857600,31536000};

const    uint32_t Month_Secs_Accu_L[13] = {0,2678400,5184000,7862400,10454400,	             //闰年一个月多少秒
                                           13132800,15724800,18403200,21081600,
                                           23673600,26352000,28944000,31622400};

const    uint32_t Month_Days_Accu_C[13] = {0,31,59,90,120,151,181,212,243,273,304,334,365};  //普通年天数
const    uint32_t Month_Days_Accu_L[13] = {0,31,60,91,121,152,182,213,244,274,305,335,366};  //闰年天数

/*********************************************************
** 函数名：Leap_Year
** 输　入：uint32_t year  系统年份	 
** 输　出：None
** 功能描述：判断是否为闰年
************************************************************/
uint8_t Leap_Year(uint32_t year)
{
	if(year%4==0)               //必须能被4整除  
	{   
		 if(year%100==0)   
		 {   
			 if(year%400==0)  
					return 1;       //如果以00结尾,还要能被400整除          
				else  
					return 0;     
		 }
	   else
	   {   
		  return 1;     
	   }  
  }  
	else
	{  
		return 0;    
  }  
}
/*********************************************************
** 函数名：Calendar_Couter
** 输　入：uint32_t TimeVar :计数值	 
** 输　出：None
** 功能描述：UNIX转换计算函数
************************************************************/
static void Calendar_Couter(uint32_t TimeVar,RTC_TimeData* Sys_TimeData)
{
	uint32_t Num4Y=0;
	uint32_t NumY=0;
	uint32_t OffSec=0;
	uint32_t Off4Y = 0;
	uint32_t i=0;
	uint32_t NumDay=0;        
	uint32_t offmonth=0;

	Num4Y = TimeVar / SecsPerFourYear;
	OffSec = TimeVar % SecsPerFourYear;

	for(i=1;i<5;i++)
	{
		if(OffSec > Year_Secs_Accu[i])
	    Off4Y++;
	} 
	/* Numer of Complete Year */
	NumY = Num4Y*4 + Off4Y;
	Sys_TimeData ->sys_year = 2000+NumY;			  //计算year ok

	OffSec = OffSec - Year_Secs_Accu[Off4Y];
	if(Leap_Year(Sys_TimeData ->sys_year)==1)		  //闰年
	{
		for(i=1;i<13;i++)
		{
		  if(OffSec > Month_Secs_Accu_L[i])
			  offmonth++;
		}    
		Sys_TimeData ->sys_data.RTC_Month =1 + offmonth ;
		OffSec = OffSec - Month_Secs_Accu_L[Sys_TimeData ->sys_data.RTC_Month-1];
	}
	else
	{
		for(i=1;i<13;i++)
		{
			 if(OffSec > Month_Secs_Accu_C[i])
				offmonth++;
		}    
		Sys_TimeData ->sys_data.RTC_Month =1 + offmonth ;
		OffSec = OffSec - Month_Secs_Accu_C[Sys_TimeData ->sys_data.RTC_Month-1];
	}									           //计算月ok

	NumDay = OffSec / SecsPerDay;
	OffSec = OffSec % SecsPerDay;
	Sys_TimeData ->sys_data.RTC_Date = NumDay+1;	   //计算day ok

	/* Compute  hours */
	Sys_TimeData ->sys_time.RTC_Hours   = OffSec / 3600;
	/* Compute minutes */
	Sys_TimeData ->sys_time.RTC_Minutes = (OffSec % 3600) / 60;
	/* Compute seconds */
	Sys_TimeData ->sys_time.RTC_Seconds = (OffSec % 3600) % 60;
}

/*********************************************************
** 函数名：UNIX_Couter
** 输　入：None	 
** 输　出：uint32_t计数值
** 功能描述：用户时间转换计算函数
************************************************************/
static uint32_t UNIX_Couter(RTC_TimeData Sys_TimeData)
{
	uint32_t  LeapY=0;
	uint32_t  ComY=0;
	uint32_t  TotDays=0;
	uint32_t  TotSeconds=0;

	if(Sys_TimeData.sys_year==2000)
	  LeapY = 0;
	else
	  LeapY = (Sys_TimeData.sys_year - 2000 -1)/4 +1;    
	ComY = (Sys_TimeData.sys_year - 2000)-(LeapY);

	if(Leap_Year(Sys_TimeData.sys_year)==0)
    //common year
		TotDays = LeapY*366 + ComY*365 + Month_Days_Accu_C[Sys_TimeData.sys_data.RTC_Month-1] + (Sys_TimeData.sys_data.RTC_Date-1); 
	else
    //leap year
	  TotDays = LeapY*366 + ComY*365 + Month_Days_Accu_L[Sys_TimeData.sys_data.RTC_Month-1] + (Sys_TimeData.sys_data.RTC_Date-1); 
  
	TotSeconds = TotDays*SecsPerDay + (Sys_TimeData.sys_time.RTC_Hours*3600 + Sys_TimeData.sys_time.RTC_Minutes*60 + Sys_TimeData.sys_time.RTC_Seconds);
	return TotSeconds;	
}


/*********************************************************
** 函数名：UNIX_Couter
** 输　入：None	 
** 输　出：uint32_t计数值
** 功能描述：用户时间转换计算函数
************************************************************/
static uint32_t UNIX_Couter_V4(RTC_TimeData Sys_TimeData)
{
	uint32_t  TotSeconds=0;

	TotSeconds = (Sys_TimeData.sys_data.RTC_Date<<24)|(Sys_TimeData.sys_time.RTC_Hours<<16)|(Sys_TimeData.sys_time.RTC_Minutes<<8)|(Sys_TimeData.sys_time.RTC_Seconds);
	return TotSeconds;	
}

//数组初始化
extern u8 turnonRes;
void Info_Data_Init()
{
	uint8_t JsonSuccess;
	//故障
	uint8_t i = 0;
	//故障
	fault_info.fault_info_count = 0;
	memset(fault_info.fault_info_queue, 0 ,sizeof(fault_info.fault_info_queue));
	
	fault_info.pUsed = NULL;
	fault_info.pFree =&fault_info.fault_info_queue[0];
	for(i=0 ; i<FAULT_INFO_SIZE - 1; i++)
	{
		fault_info.fault_info_queue[i].pNext = &fault_info.fault_info_queue[i+1];
	}	
	fault_info.fault_info_queue[FAULT_INFO_SIZE-1].pNext = NULL;
	
	//日志
	log_info.log_info_pt = 0;
	log_info.log_info_isfull = 0;
	memset(log_info.log_info_queue,0,sizeof(log_info.log_info_queue));
		
	if(RCC->CSR &0x80000000)
		WriteLog_API(0,LOG_TYPE_RESET_SYSTEM,LOG_STATUS_RESET_LOWPAWER,0,0);
	else if(RCC->CSR &0x40000000)
		WriteLog_API(0,LOG_TYPE_RESET_SYSTEM,LOG_STATUS_RESET_WWATCHDOG,0,0);
	else if(RCC->CSR &0x20000000)
		WriteLog_API(0,LOG_TYPE_RESET_SYSTEM,LOG_STATUS_RESET_IWATCHDOG,0,0);
	else if(RCC->CSR &0x10000000)
		WriteLog_API(0,LOG_TYPE_RESET_SYSTEM,LOG_STATUS_RESET_SOFTWARE,0,0);	
	else
		WriteLog_API(0,LOG_TYPE_RESET_SYSTEM,LOG_STATUS_RESET_PORRSTF,0,0);
	RCC->CSR = 0x1000000;
	switch(turnonRes)
	{		
		case Json_Newlyreceived:
			
			break;
		case Json_Watchdogreset:
			break;
		case Json_Watchdognotreset:
			break;
		case Json_Recoveryparameters:
			break;
		case Json_Modify:
			break;
		case Json_Other:
			break;
		default:
			break;				
	}
	bsp_DelayMS(20);
	ee_ReadBytes((uint8_t *)(&JsonSuccess), JSON_STARTUP_MODE, 1);
	bsp_DelayMS(20);
	if(JsonSuccess == 0x78)
	{
		WriteLog_API(0,LOG_TYPE_JSON_TEXT,LOG_STATUS_START_OUT,0,0);
	}
	else
	{
		WriteLog_API(0,LOG_TYPE_JSON_TEXT,LOG_STATUS_START_IN,0,0);
	}
}
//查询故障数组
static stFaultInfo *checkFaultInfoQueue(STR_FAULT_INFO *faultInfo)
{
	stFaultInfo* resfaultInfo = NULL;
	stFaultInfo* tmpfaultInfo = fault_info.pUsed;
	
	if(fault_info.fault_info_count == 0)
		return resfaultInfo;
	
	if(faultInfo->host_type == FAULT_HOST_OTHER)
	{
		do
		{
			if(tmpfaultInfo->faultInfo.event == faultInfo->event)
			{
				resfaultInfo = tmpfaultInfo;
				break;
			}
			tmpfaultInfo = tmpfaultInfo->pNext;
		}while(tmpfaultInfo != NULL);
	}
	else
	{
		
		do
		{
			if((tmpfaultInfo->faultInfo.host_name == faultInfo->host_name) && \
				(tmpfaultInfo->faultInfo.host_type == faultInfo->host_type))
			{
				resfaultInfo = tmpfaultInfo;
				break;
			}
			tmpfaultInfo = tmpfaultInfo->pNext;
		}while(tmpfaultInfo != NULL);	
	}
	return resfaultInfo;
}
static void InsFaultInfoQueue(STR_FAULT_INFO *faultInfo)
{
	stFaultInfo * tmpInfo;
	
	if(fault_info.fault_info_count <FAULT_INFO_SIZE)
	{
		memcpy(&(fault_info.pFree->faultInfo),faultInfo,sizeof(STR_FAULT_INFO));
		tmpInfo = fault_info.pFree;
		fault_info.pFree = fault_info.pFree->pNext;
		
		tmpInfo->pNext =  fault_info.pUsed;
		fault_info.pUsed = tmpInfo;
		fault_info.fault_info_count ++;
	}
}
//删除故障数组
static void DelFaultInfoQueue(STR_FAULT_INFO *faultInfo)
{
	uint8_t i;
	stFaultInfo* delfaultInfo = NULL;
	stFaultInfo* tmpfaultInfo = fault_info.pUsed;
	delfaultInfo = checkFaultInfoQueue(faultInfo);
	if(delfaultInfo != NULL)
	{	
		if(delfaultInfo == tmpfaultInfo)
		{
			fault_info.pUsed = fault_info.pUsed->pNext;
			fault_info.fault_info_count--;
			tmpfaultInfo->pNext = fault_info.pFree;
			fault_info.pFree = tmpfaultInfo;
		}
		else
		{
			do
			{
				if(tmpfaultInfo->pNext == delfaultInfo)
				{
					tmpfaultInfo->pNext = delfaultInfo->pNext;
					delfaultInfo->pNext = fault_info.pFree;
					fault_info.pFree = delfaultInfo;
					fault_info.fault_info_count--;
					break;
				}
				tmpfaultInfo = tmpfaultInfo->pNext;
			}while(tmpfaultInfo != NULL);
		}
	}
}

static u8 serverReportFail(log_int host_type, log_int host_name, log_int event, log_int status)
{
#ifdef SERVER_REPORT_ENABLE
	u8 temp[10];
	u8 serverID;
	u16 length;
		
	if(getSeverStatus() == 0)
		return 1;
	
	memset(temp,0,sizeof(temp));
	
	temp[0] = REPORTENUM_FAIL;
	temp[1] = ENUM_SYSTEM;
	temp[2] = 0;
	length = 4;
	memcpy(&temp[3],&length,2);
	temp[5] =host_type;
	temp[6] =host_name;
	temp[7] =event;
	temp[8] =status;
	
	serverID = getSysServerAddr();
	UdpAppProcCtrl(AppProcCtrl_stateReport_indication,temp,9,serverID);
#endif
	return 1;
}

//写故障数组对外api
void WriteFault_API(log_int host_type, log_int host_name, log_int event, log_int status)
{
	RTC_TimeData   time;
	STR_FAULT_INFO faultInfo;
	
	faultInfo.host_type = host_type;
	faultInfo.host_name = host_name;
	faultInfo.event = event;
	faultInfo.status = status;
	
	if(status == FAULT_STATUS_NORMAL)
	{
		DelFaultInfoQueue(&faultInfo);
	}
	else
	{
		Get_Data_Time(&time);
		faultInfo.time = UNIX_Couter_V4(time);
		InsFaultInfoQueue(&faultInfo);
	}
	
	//serverReportFail(host_type, host_name,event,status);
}
//读故障数组对外api
uint8_t ReadFault_API(uint8_t fault_id, uint8_t fault_size, STR_FAULT_INFO *info)
{
	uint8_t i = 0;
	uint8_t index = 0;
	uint8_t res = 0;
	stFaultInfo* tmpfaultInfo = fault_info.pUsed;
	STR_FAULT_INFO tmpinfo;
	i = fault_id;
	
	
	for(i = 0; i<fault_id; i++)
	{
		if(tmpfaultInfo == NULL)
			break;
		tmpfaultInfo = tmpfaultInfo->pNext;
		index++;
	}
	
	if(index<fault_id)
			return res;
	
//	for(i = 0;i<FAULT_INFO_SIZE;i++)
	{
//		if(res == fault_size)
//			break;
		
		if(tmpfaultInfo != NULL)
		{
			memcpy(&tmpinfo,&(tmpfaultInfo->faultInfo),sizeof(STR_FAULT_INFO));
			index++;
			index %= FAULT_INFO_SIZE;
			res++;
		}
		else
			return res;
	}
	memcpy(info, &tmpinfo, sizeof(tmpinfo));
	return res;
}

static u8 serverReportLog(log_int host, log_int type, log_int status,void *data ,uint8_t len)
{
#ifdef SERVER_REPORT_ENABLE
	u8 temp[40];
	u8 serverID;
	u16 length;
		
	if(getSeverStatus() == 0)
		return 1;
	
	if(len>30)
		return 0;
	

	
	memset(temp,0,sizeof(temp));
	
	temp[0] = host;
	temp[1] = type;
	temp[2] = status;
	length = len;
	memcpy(&temp[3],&length,2);
	memcpy(&temp[5],(u8 *)data,length);
	
	serverID = getSysServerAddr();
	UdpAppProcCtrl(AppProcCtrl_logReport_indication,temp,5+len,serverID);
#endif
	return 1;
}


void WriteLog_API(log_int host, log_int type, log_int status,void *data ,uint8_t len)
{
	RTC_TimeData   time;
	Get_Data_Time(&time);
	log_info.log_info_queue[log_info.log_info_pt].time = UNIX_Couter_V4(time);
	log_info.log_info_queue[log_info.log_info_pt].host = host;
	log_info.log_info_queue[log_info.log_info_pt].type = type;
	log_info.log_info_queue[log_info.log_info_pt].status = status;
	memset(log_info.log_info_queue[log_info.log_info_pt].data,0,sizeof(log_info.log_info_queue[log_info.log_info_pt].data));
	memcpy(log_info.log_info_queue[log_info.log_info_pt].data,data,len);
	log_info.log_info_pt++;
	if(log_info.log_info_pt == LOG_INFO_SIZE)
		log_info.log_info_isfull = 1;		
	log_info.log_info_pt %= LOG_INFO_SIZE;
	
	//serverReportLog(host, type, status,data ,len);
}


uint8_t getFaultLength_API()
{
	return fault_info.fault_info_count;
}


uint8_t getLogLength_API()
{
	return log_info.log_info_pt;
}


uint8_t ReadLog_API(uint8_t log_id, uint8_t log_size, STR_LOG_INFO* logInfo)
{
	uint8_t i;
	uint8_t res = 0;
	STR_LOG_INFO logInfotmp = {0};
	uint8_t readpt = log_info.log_info_pt;

	for(i = 0; i<log_id; i++)
	{
		if(readpt == 0)
		{
			if(!log_info.log_info_isfull)
				return res;
			readpt = LOG_INFO_SIZE;		
		}
		if(readpt == 0)
			readpt = LOG_INFO_SIZE;
		readpt--;
	}	
	for(i = 0; i<log_size; i++)
	{
		memcpy(&logInfotmp,&log_info.log_info_queue[readpt],sizeof(STR_LOG_INFO));
		if(readpt == 0)
		{
			if(!log_info.log_info_isfull)
				break;
			readpt = LOG_INFO_SIZE;		
		}
		if(readpt == 0)
			readpt = LOG_INFO_SIZE;
		readpt--;		
	}
	memcpy(logInfo, &logInfotmp, sizeof(logInfotmp));
	return res;
}


void test_task(void)
{	
	uint8_t res = 0;
	uint8_t data[6] = {1,2,3,4,5,6};
	STR_FAULT_INFO faultInfo;
	STR_LOG_INFO logInfotmp;
	Info_Data_Init();
	
	while(1)
	{
		WriteFault_API(1,20,1,0);
		WriteFault_API(0,20,1,0);
		WriteFault_API(2,1,3,0);
		WriteFault_API(2,2,4,0);
		WriteFault_API(2,3,5,0);
		WriteFault_API(2,4,6,0);
		OSTimeDly(1000);
		WriteFault_API(0,20,1,1);
		WriteFault_API(2,1,3,1);
		WriteFault_API(2,2,4,1);
		WriteFault_API(2,3,5,1);
		WriteFault_API(2,4,6,1);
		OSTimeDly(1000);
//		res = ReadFault_API(0,1,&faultInfo);
		res = ReadFault_API(1,1,&faultInfo);
		res = ReadFault_API(2,1,&faultInfo);
		res = ReadFault_API(3,1,&faultInfo);
		res = ReadFault_API(4,1,&faultInfo);
		res = ReadFault_API(5,1,&faultInfo);
		res = ReadFault_API(6,1,&faultInfo);
		res = ReadFault_API(0,1,&faultInfo);
		
		WriteLog_API(1,1,1,data,4);
		WriteLog_API(1,2,1,data,3);
		WriteLog_API(1,3,1,data,2);
		WriteLog_API(1,4,1,data,1);
		ReadLog_API(1,1,&logInfotmp);
		OSTimeDly(1000);
	}	
}



