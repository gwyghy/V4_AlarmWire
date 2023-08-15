/*
*********************************************************************************************************
*	                                            DoubleWires
*                           Commuication Controller For The Mining Industry
*
*                                  (c) Copyright 1994-2013  HNDZ 
*                       All rights reserved.  Protected by international copyright laws.
*
*
*    File    : sysParamAnalysis.h
*    Module  : system function
*    Version : V1.0
*    History :
*   -----------------
*              ʵ�ֲ�����������
*              Version  Date           By            Note
*              v1.0     
*
*********************************************************************************************************
*/

#ifndef __SYSPARAMANALYSIS_H__
#define __SYSPARAMANALYSIS_H__

#include "includes.h"

#define V4FILE_SYSPARAM_JSON	"0:/update/DEFAULT_SC_JSON.txt"  //"0:/v4para/para.txt"

#define SYSPARAM_FUNC_LEVEL(x,value,level)    (x|((uint64_t)(value) << (64 - 8*(level))))   //xԭֵ value��ǰindex  level����
#define SYSPARAM_READ_LEVEL(value,level)      (uint8_t)(value >> (64 - 8*(level)))
#define DEFAULT_VALUE_POS		22    //�ڲ������е�λ��
#define DEFAULT_GLOBAL_FLAG_POS	18

#define JSONPARAM_DEFAULT_LEN   26


enum
{
	ParamIndex_sysparam =1,
	ParamIndex_single,
	ParamIndex_sensor,
	ParamIndex_comb,     
	ParamIndex_part,
	ParamIndex_group,
	ParamIndex_MAX
};

enum
{
	Json_Newlyreceived =1,//�յ���json
	Json_Watchdogreset,//�ϴ������쳣���ҿ��Ź���λ
	Json_Watchdognotreset,//�ϴ������쳣���ҷǿ��Ź���λ
	Json_Recoveryparameters,  //�ָ�Ĭ�ϲ���   
	Json_Modify,//�ڲ�json���ֶ��޸�
	Json_Other,//�������
};



u8  sysParamInit(void);
u8 LevelToStruct(uint64_t levels,u16 **pdata,u16 *unitLen,u8 *moduletype,u8 *moduleID);
u16 GlobalParamCRCCal(void);
u8 IsJsonParamGlobal(u32 offset);

#endif
