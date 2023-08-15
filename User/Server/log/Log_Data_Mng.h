/*
*********************************************************************************************************
*	                                            V4
*                           Commuication Controller For The Mining Industry
*
*                                  (c) Copyright 1994-2013  HNDZ 
*                       All rights reserved.  Protected by international copyright laws.
*
*
*    File    : Can_Bus.c
*    Module  : user
*    Version : V1.0
*    History :
*   -----------------
*              Version  Date           By            Note
*              v1.0         
*
*********************************************************************************************************
*/

#ifndef _LOG_DATA_MNG_H_
#define _LOG_DATA_MNG_H_

#include "Log_Data_Type.h"

#define FAULT_INFO_SIZE   20
#define LOG_INFO_SIZE   200


#define  WRITE_TO_FFLASH        0
#define  READ_FROM_FFLASH       1

#define  NONE_ERR               1
#define  ERR_LOG_OVERFLOW       -1
#define  ERR_LOG_FFLASH         -2

#define  SAVE_TIME_ARRIVE       1
#define  SAVE_TIME_NOARRIVE     0
#define  SEC_PRECISION          5000
#define  SEC_PRECISION_OFFSET   3

#define  F_ERR_NONE             1
#define  F_ERR_DISK             -1
#define  F_ERR_OPEN             -2
#define  F_ERR_WRITE            -3

#define  SecsPerComYear         3153600    //(365*3600*24)
#define  SecsPerLeapYear        31622400   //(366*3600*24)
#define  SecsPerFourYear        126230400  //((365*3600*24)*3+(366*3600*24))
#define  SecsPerDay             (3600*24)



typedef struct
{
	STR_FAULT_INFO     faultInfo;
	void  *            pNext;
} stFaultInfo;


typedef struct {
	stFaultInfo fault_info_queue[FAULT_INFO_SIZE];
	stFaultInfo *pFree;
	stFaultInfo *pUsed;
	uint8_t fault_info_count;
}FAULT_INFO;

typedef struct {
 STR_LOG_INFO log_info_queue[LOG_INFO_SIZE];
 uint8_t log_info_pt;
 uint8_t log_info_isfull;
}LOG_INFO;

void Info_Data_Init(void);
void   WriteFault_API(log_int host_type, log_int host_name, log_int event, log_int status);
void WriteLog_API(log_int host, log_int type, log_int status,void *data ,uint8_t len);
uint8_t ReadFault_API(uint8_t fault_id, uint8_t fault_size, STR_FAULT_INFO *info);
uint8_t ReadLog_API(uint8_t log_id, uint8_t log_size, STR_LOG_INFO* logInfo);
uint8_t getLogLength_API(void);
uint8_t getFaultLength_API(void);

#endif



