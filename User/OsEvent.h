/*
*********************************************************************************************************
*	                                            DoubleWires
*                           Commuication Controller For The Mining Industry
*
*                                  (c) Copyright 1994-2013  HNDZ 
*                       All rights reserved.  Protected by international copyright laws.
*
*
*    File    : OsEvent.h
*    Module  : user
*    Version : V1.0
*    History :
*   -----------------
*              Version  Date           By            Note
*              v1.0     2013-09-14    
*
*********************************************************************************************************
*/

#ifndef _OSEVENT_H__
#define _OSEVENT_H__

#include "os_api.h"

/*
*********************************************************************************************************
*                                           OS_EVENTS
*********************************************************************************************************
*/
#ifdef __MAIN_C__
	#define MAIN_EXT	
#else
	#define MAIN_EXT                             extern
#endif

MAIN_EXT PLATFORM_OS_SEM 		                        KeyMbox;
MAIN_EXT PLATFORM_OS_SEM 		                        SYSMutex;
MAIN_EXT PLATFORM_OS_SEM 		                        SYSCtrlSem;	

MAIN_EXT PLATFORM_OS_SEM 					    		RS485A_RecSem;
MAIN_EXT PLATFORM_OS_SEM                                Rs485A_SenSem;
MAIN_EXT PLATFORM_OS_SEM 					    		RS485B_RecSem;
MAIN_EXT PLATFORM_OS_SEM                                Rs485B_SenSem;
	
MAIN_EXT PLATFORM_OS_SEM								EthPktSem;
MAIN_EXT PLATFORM_OS_SEM                                UDPPktRecvSem	;
MAIN_EXT PLATFORM_OS_SEM                                UDPPktSendSem	;
	
MAIN_EXT OS_EVENT                             *UtilTimerTaskSendSem;	
MAIN_EXT OS_EVENT                             *CanRTxTaskSendSem;
MAIN_EXT OS_EVENT                             *revProgramSem;
MAIN_EXT OS_EVENT                             *fatfsSaveFileSem;

MAIN_EXT OS_EVENT                             *UsartRTxTaskSendSem;
	
#endif
