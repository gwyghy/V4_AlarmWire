/*
*********************************************************************************************************
*	                                            DoubleWires
*                           Commuication Controller For The Mining Industry
*
*                                  (c) Copyright 1994-2013  HNDZ 
*                       All rights reserved.  Protected by international copyright laws.
*
*
*    File    : SysFunc_API.h
*    Module  : system function
*    Version : V1.0
*    History :
*   -----------------
*              实现控制器功能
*              Version  Date           By            Note
*              v1.0     
*
*********************************************************************************************************
*/

#ifndef __SYSFUNC_API_H__
#define __SYSFUNC_API_H__

#include "sys_data_type.h"
#include "sys_param_type.h"
#include "sys_msg_def.h"

#define WDT_NUM     8
#define WDT_MANAGE  0
#define WDT_GUI     1
#define WDT_SYSTEM_FUNCTION   2
#define WDT_NO_USE      0xFF
#define WDT_RST_VALUE   8
#define WDT_CLEAR_VALUE 0x00

void FuncInit(void);
int SysNetCtrlFun(u8 addr,u8 Type,u8 Id,u8 cmd,u8 mode,u16 Length,u8 *pdata);
void ActionDataInit(void);
uint8_t getSysInitState(void);
#endif
