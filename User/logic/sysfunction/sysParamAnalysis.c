/*
*********************************************************************************************************
*	                                            DoubleWires
*                           Commuication Controller For The Mining Industry
*
*                                  (c) Copyright 1994-2013  HNDZ 
*                       All rights reserved.  Protected by international copyright laws.
*
*
*    File    : sysParamAnalysis.c
*    Module  : system function
*    Version : V1.0
*    History :
*   -----------------
*              实现参数解析功能
*              Version  Date           By            Note
*              v1.0     
*
*********************************************************************************************************
*/

#include "sys_default.h"
#include "sys_data_type.h"
#include "sys_param_type.h"
#include "Json_Analysis.h"
#include "cJSON.h"
#include "sysParamAnalysis.h"
#include "sys_msg_def.h"
#include "netmain.h"
#include "crc.h"
#include "ProjectConfig.h"
#include "bsp_i2c_gpio.h"
#include "ff.h"

#if (LOGIC_SYSFUNC_EN == 1)

#define SYS_JSON_MAX    		 (sizeof(SysParamType)/2)
#define SYS_JSON_Single_START    (0)
#define SYS_JSON_Single_LEN      (sizeof(SigActionParamType)/2)
#define SYS_JSON_Comb_START      (SYS_JSON_Single_START+SYS_JSON_Single_LEN*SINGLEACTION_NUM_MAX)
#define SYS_JSON_Comb_LEN      	 (sizeof(CombActionParamType)/2)
#define SYS_JSON_Group_START     (SYS_JSON_Comb_START+SYS_JSON_Comb_LEN*COMBACTION_NUM_MAX)
#define SYS_JSON_Group_LEN       (sizeof(groupActionParamType)/2)
#define SYS_JSON_Part_START    	 (SYS_JSON_Group_START+SYS_JSON_Group_LEN*GROUPACTION_NUM_MAX)
#define SYS_JSON_Part_LEN      	 (sizeof(PartParamType)/2)
#define SYS_JSON_Sensor_START    (SYS_JSON_Part_START+SYS_JSON_Part_LEN*PART_NUM_MAX)
#define SYS_JSON_Sensor_LEN      (sizeof(SensorParamType)/2)
#define SYS_JSON_Global_START    (SYS_JSON_Sensor_START+SYS_JSON_Sensor_LEN*SENSOR_MAX)
#define SYS_JSON_Global_LEN      (sizeof(globalParamType)/2)
	
static u8 *SysJsonPoint[SYS_JSON_MAX] __attribute__((at(MEM_SDRAM_SYSJSON_POS)));



/******************struct to level***********************/
static void  LoadDefaultParam_combsingle(SigActionParamType	 *pParam,uint64_t levelValue,u8 level,u8 *pjsonValue,u8 singleNum,u8 combNum,u8 sel)
{
	int j,i,m;
	ParamUnit *pdata;
	uint64_t templevel,levelValueTmp;
	u8 num,triggernum;
	u16 sysjson_pos=0;
	u32 sysjson_start=0;
	u32 sysjson_start_base;
	
	pdata = (ParamUnit *)(&(pParam->baseParam));
	sysjson_start = SYS_JSON_Comb_START+combNum*SYS_JSON_Comb_LEN+sizeof(CombAction_BaseParamType)/2+(sizeof(CombAction_actionParamType)/2)*singleNum+2;
	for(j=0;j<(sizeof(SigAction_BaseParamType)/sizeof(ParamUnit)) ;j++)
	{
		if(sel)
			memcpy(pdata, pjsonValue+DEFAULT_VALUE_POS,2);
		SysJsonPoint[sysjson_start+sysjson_pos]=pjsonValue;
		pjsonValue = pjsonValue +JSONPARAM_DEFAULT_LEN;
		pdata = pdata + 1;
		sysjson_pos++;
	}

	pdata = (ParamUnit *)(&(pParam->actionParam));
	for(j=0;j<(sizeof(SigAction_ActionParamType)/sizeof(ParamUnit)) ;j++)
	{
		if(sel)
			memcpy(pdata, pjsonValue+DEFAULT_VALUE_POS,2);
		SysJsonPoint[sysjson_start+sysjson_pos]=pjsonValue;
		pjsonValue = pjsonValue +JSONPARAM_DEFAULT_LEN;
		pdata = pdata + 1;
		sysjson_pos++;
	}	
	
	num = getJsonParaStructSize(levelValue);
	if(num <1)
		return;
	
	num =num-1;
	for(j=0;(j< num)&&(j<SINGLE_ACTION_TRIGGER_MAX) ;j++)
	{
		levelValueTmp = SYSPARAM_FUNC_LEVEL(levelValue,j+2,level);
		
		//触发条件
		templevel = SYSPARAM_FUNC_LEVEL(levelValueTmp,1,level+1);
		pjsonValue = getJsonParaAddress(templevel);
		pdata = (ParamUnit *)(&(pParam->tiggerParam[j].cond));
		sysjson_pos=0;
		sysjson_start_base = sizeof(SigAction_BaseParamType)/2+sizeof(SigAction_ActionParamType)/2;
		
		for(i=0;i<(sizeof(SigAction_TriggerCondType)/sizeof(ParamUnit)) ;i++)
		{
			if(sel)
				memcpy(pdata, pjsonValue+DEFAULT_VALUE_POS,2);
			SysJsonPoint[sysjson_start+sysjson_start_base+j*(sizeof(SigAction_TriggerType)/2)+sysjson_pos]=pjsonValue;
			pjsonValue = pjsonValue +JSONPARAM_DEFAULT_LEN;
			pdata = pdata + 1;
			sysjson_pos++;
		}			
		
		//触发动作
		triggernum = getJsonParaStructSize(levelValueTmp);
		if(triggernum <1)
			continue;
		
		triggernum = triggernum-1;
		templevel = SYSPARAM_FUNC_LEVEL(levelValueTmp,2,level+1);
		pjsonValue = getJsonParaAddress(templevel);
		pdata = (ParamUnit *)(&(pParam->tiggerParam[j].action[0]));
		for(m=0; (m<triggernum)&&(m<TRIGGER_ACTION_MAX);m++)
		{
			for(i=0;i<(sizeof(SigAction_TriggerActionType)/sizeof(ParamUnit)) ;i++)
			{
				if(sel)
					memcpy(pdata, pjsonValue+DEFAULT_VALUE_POS,2);
				
				SysJsonPoint[sysjson_start+sysjson_start_base+j*(sizeof(SigAction_TriggerType)/2)+sysjson_pos]=pjsonValue;
				pjsonValue = pjsonValue +JSONPARAM_DEFAULT_LEN;
				pdata = pdata + 1;
				sysjson_pos++;
			}	
		}
	}	
	
}

static void  LoadDefaultParam_comb(u8 sel)
{
	u8 i,j,num,basenum,totalnum,combActionId;
	uint64_t levels=0;
	uint64_t templevel,levels1;
	CombActionParamType *pParam;
	ParamUnit *pdata;
	u8 *pjsonValue;
	u16 sysjson_pos=0;
	u32 sysjson_start=0;	
	
	levels1 = SYSPARAM_FUNC_LEVEL(levels,ParamIndex_comb,1);
	totalnum = getJsonParaStructSize(levels1);
#if (IDANDINDEXAPART == 0)
	pParam = SysParam.CombAction;
#endif
	for(i=0; (i< totalnum)&&(i<COMBACTION_NUM_MAX);i++)
	{
		levels = SYSPARAM_FUNC_LEVEL(levels1,1+i,2);
		num = getJsonParaStructSize(levels);
		if(num <1)
			break;
		
		templevel = SYSPARAM_FUNC_LEVEL(levels,1,3);
		basenum = getJsonParaChildSize(templevel);
		pjsonValue = getJsonParaAddress(templevel);
		
#if (IDANDINDEXAPART == 1)	
		memcpy(&combActionId, pjsonValue+DEFAULT_VALUE_POS+26,2);
		if(combActionId == 0 || combActionId>COMBACTION_NUM_MAX)
			continue;
		pParam = &SysParam.CombAction[combActionId-1];
#endif
		
		
		pdata = (ParamUnit *)(&(pParam->baseParam));
		sysjson_pos=0;
		for(j=0;(j< basenum)&&(j<(sizeof(CombAction_BaseParamType)/sizeof(ParamUnit))) ;j++)
		{
			if(sel)
				memcpy(pdata, pjsonValue+DEFAULT_VALUE_POS,2);
			
			SysJsonPoint[SYS_JSON_Comb_START+i*SYS_JSON_Comb_LEN+sysjson_pos]=pjsonValue;
			pjsonValue = pjsonValue +JSONPARAM_DEFAULT_LEN;
			pdata = pdata + 1;
			sysjson_pos++;
		}
		
		num = num-1;
		for(j=0; (j< num)&&(j<COMBACTION_SINGLE_NUM) ;j++)
		{
			templevel = SYSPARAM_FUNC_LEVEL(levels,j+2,3);
			templevel = SYSPARAM_FUNC_LEVEL(templevel,1,4);
			pjsonValue = getJsonParaAddress(templevel);
			
			if(sel)
				memcpy(&(pParam->actionParam[j].zeroDelayEnable), pjsonValue+DEFAULT_VALUE_POS,2);
			SysJsonPoint[SYS_JSON_Comb_START+i*SYS_JSON_Comb_LEN+sysjson_pos]=pjsonValue;
			pjsonValue = pjsonValue +JSONPARAM_DEFAULT_LEN;
			sysjson_pos++;
			
			if(sel)
				memcpy(&(pParam->actionParam[j].zeroDelayTime), pjsonValue+DEFAULT_VALUE_POS,2);
			SysJsonPoint[SYS_JSON_Comb_START+i*SYS_JSON_Comb_LEN+sysjson_pos]=pjsonValue;
			pjsonValue = pjsonValue +JSONPARAM_DEFAULT_LEN;
			sysjson_pos++;
			
			templevel = SYSPARAM_FUNC_LEVEL(levels,j+2,3);
			LoadDefaultParam_combsingle(&(pParam->actionParam[j].SigParam),templevel,4,pjsonValue,j,i,sel);
		}
#if (IDANDINDEXAPART == 0)	
		pParam++;
#endif
	}
}

static void  LoadDefaultParam_single(u8 sel)
{
	u8 i,j,m,n,num,size,triggernum,singleActionId;
	uint64_t levels=0;
	uint64_t templevel,templevel1,levelValueTmp;
	SigActionParamType *pParam;
	ParamUnit *pdata;
	u8 *pjsonValue;
	u16 sysjson_pos=0;
	u32 sysjson_start=0;
	
	levels = SYSPARAM_FUNC_LEVEL(levels,ParamIndex_single,1);
	num = getJsonParaStructSize(levels);
	
#if (IDANDINDEXAPART == 0)
	pParam = &SysParam.SigAction[0]; 
#endif

	for(i=0; (i< num)&&(i<SINGLEACTION_NUM_MAX);i++)
	{		
		templevel1 = SYSPARAM_FUNC_LEVEL(levels,i+1,2);
		templevel = SYSPARAM_FUNC_LEVEL(templevel1,1,3);
		pjsonValue = getJsonParaAddress(templevel);

#if (IDANDINDEXAPART == 1)
		memcpy(&singleActionId, pjsonValue+DEFAULT_VALUE_POS+26,2);
		if(singleActionId == 0 || singleActionId>SINGLEACTION_NUM_MAX)
			continue;
		pParam = &SysParam.SigAction[singleActionId-1];
#endif

		pdata = (ParamUnit *)(&(pParam->baseParam));
		sysjson_pos=0;
		for(j=0;j<(sizeof(SigAction_BaseParamType)/sizeof(ParamUnit)) ;j++)
		{
			if(sel)
				memcpy(pdata, pjsonValue+DEFAULT_VALUE_POS,2);
			
			SysJsonPoint[SYS_JSON_Single_START+i*SYS_JSON_Single_LEN+sysjson_pos]=pjsonValue;
			pjsonValue = pjsonValue +JSONPARAM_DEFAULT_LEN;
			pdata = pdata + 1;
			sysjson_pos++;
		}

		pdata = (ParamUnit *)(&(pParam->actionParam));
		for(j=0;j<(sizeof(SigAction_ActionParamType)/sizeof(ParamUnit)) ;j++)
		{
			if(sel)
				memcpy(pdata, pjsonValue+DEFAULT_VALUE_POS,2);
			
			SysJsonPoint[SYS_JSON_Single_START+i*SYS_JSON_Single_LEN+sysjson_pos]=pjsonValue;
			pjsonValue = pjsonValue +JSONPARAM_DEFAULT_LEN;
			pdata = pdata + 1;
			sysjson_pos++;
		}

		size = getJsonParaStructSize(templevel1);
		if(size <1)
		{
			pParam++;
			continue;
		}
		
		size =size-1;
		for(j=0;(j< size)&&(j<SINGLE_ACTION_TRIGGER_MAX) ;j++)
		{
			levelValueTmp = SYSPARAM_FUNC_LEVEL(templevel1,j+2,3);
			
			//触发条件
			templevel = SYSPARAM_FUNC_LEVEL(levelValueTmp,1,4);
			pjsonValue = getJsonParaAddress(templevel);
			pdata = (ParamUnit *)(&(pParam->tiggerParam[j].cond));
			sysjson_pos=0;
			sysjson_start = SYS_JSON_Single_START+i*SYS_JSON_Single_LEN+sizeof(SigAction_BaseParamType)/2+sizeof(SigAction_ActionParamType)/2;
			for(n=0;n<(sizeof(SigAction_TriggerCondType)/sizeof(ParamUnit)) ;n++)
			{
				if(sel)
					memcpy(pdata, pjsonValue+DEFAULT_VALUE_POS,2);
				
				SysJsonPoint[sysjson_start+j*(sizeof(SigAction_TriggerType)/2)+sysjson_pos]=pjsonValue;
				pjsonValue = pjsonValue +JSONPARAM_DEFAULT_LEN;
				pdata = pdata + 1;
				sysjson_pos++;
			}			
			
			//触发动作
			triggernum = getJsonParaStructSize(levelValueTmp);
			if(triggernum <1)
				continue;
			
			triggernum = triggernum-1;
			templevel = SYSPARAM_FUNC_LEVEL(levelValueTmp,2,4);
			pjsonValue = getJsonParaAddress(templevel);
			pdata = (ParamUnit *)(&(pParam->tiggerParam[j].action[0]));
			for(m=0; (m<triggernum)&&(m<TRIGGER_ACTION_MAX);m++)
			{
				for(n=0;n<(sizeof(SigAction_TriggerActionType)/sizeof(ParamUnit)) ;n++)
				{
					if(sel)
						memcpy(pdata, pjsonValue+DEFAULT_VALUE_POS,2);
					
					SysJsonPoint[sysjson_start+j*(sizeof(SigAction_TriggerType)/2)+sysjson_pos]=pjsonValue;
					pjsonValue = pjsonValue +JSONPARAM_DEFAULT_LEN;
					pdata = pdata + 1;
					sysjson_pos++;
				}	
			}
		}
#if (IDANDINDEXAPART == 0)
		pParam++;
#endif
	}	
}

static void  LoadDefaultParam_sensor(u8 sel)
{
	u8 i,j,num,basenum,sensorId;
	uint64_t levels=0;
	uint64_t templevel;
	SensorParamType *pParam;
	ParamUnit *pdata;
	u8 *pjsonValue;
	u16 precision[4]={1,10,100,1000};
	u16 sysjson_pos=0;
	
	levels = SYSPARAM_FUNC_LEVEL(levels,ParamIndex_sensor,1);
	num = getJsonParaStructSize(levels);
#if (IDANDINDEXAPART == 0)
	pParam = &SysParam.Sensor[0];
#endif
	for(i=0; (i< num)&&(i<SENSOR_MAX);i++)
	{
		templevel = SYSPARAM_FUNC_LEVEL(levels,1+i,2);
		basenum = getJsonParaChildSize(templevel);
		if(basenum <1)
			break;
		
		pjsonValue = getJsonParaAddress(templevel);
		
#if (IDANDINDEXAPART == 1)	
		memcpy(&sensorId, pjsonValue+DEFAULT_VALUE_POS+26,2);
		if(sensorId == 0 || sensorId>SENSOR_MAX)
			continue;
		pParam = &SysParam.Sensor[sensorId-1];
#endif
		
		pdata = (ParamUnit *)(&(pParam->sensorName));
		sysjson_pos=0;
		for(j=0;(j< basenum)&&(j<(sizeof(SensorParamType)/sizeof(ParamUnit))) ;j++)
		{
			if(sel)
				memcpy(pdata, pjsonValue+DEFAULT_VALUE_POS,2);
			
			SysJsonPoint[SYS_JSON_Sensor_START+i*SYS_JSON_Sensor_LEN+sysjson_pos]=pjsonValue;
			pjsonValue = pjsonValue +JSONPARAM_DEFAULT_LEN;
			pdata = pdata + 1;
			sysjson_pos++;
		}
		
		if(pParam->precision<4)
		{
			pParam->precision = precision[pParam->precision];
		}
		else
			pParam->precision = 1;
		
		if(pParam->precision_logic<4)
		{
			pParam->precision_logic = precision[pParam->precision_logic];
		}
		else
			pParam->precision_logic = 1;

#if (IDANDINDEXAPART == 0)
		pParam++;
#endif
	}
}

static void  LoadDefaultParam_global(u8 sel)
{
	u8 i,j,num,sizenum;
	uint64_t levels=0;
	uint64_t templevel;
	ParamUnit *pdata;
	u8 *pjsonValue;
	u16 sysjson_pos=0;
	
	levels = SYSPARAM_FUNC_LEVEL(levels,ParamIndex_sysparam,1);
	num = getJsonParaStructSize(levels);
	
	if(num>0)
	{
		templevel = SYSPARAM_FUNC_LEVEL(levels,1,2);
		sizenum = getJsonParaChildSize(templevel);
		pjsonValue = getJsonParaAddress(templevel);
		pdata = (ParamUnit *)(&SysParam.globalParam.SupportParam);
		for(i=0; (i< sizenum)&&(i<(sizeof(globalParam_SupportType)/sizeof(ParamUnit)));i++)
		{
			if(sel)
				memcpy(pdata, pjsonValue+DEFAULT_VALUE_POS,2);
			SysJsonPoint[SYS_JSON_Global_START+sysjson_pos]=pjsonValue;
			pjsonValue = pjsonValue +JSONPARAM_DEFAULT_LEN;
			pdata = pdata + 1;
			sysjson_pos	++;		
		}
	}
	
	if(num>1)
	{
		templevel = SYSPARAM_FUNC_LEVEL(levels,2,2);
		sizenum = getJsonParaChildSize(templevel);
		pjsonValue = getJsonParaAddress(templevel);
		pdata = (ParamUnit *)(&SysParam.globalParam.ServerParam);
		for(i=0; (i< sizenum)&&(i<(sizeof(globalParam_ServerType)/sizeof(ParamUnit)));i++)
		{
			if(sel)
				memcpy(pdata, pjsonValue+DEFAULT_VALUE_POS,2);
			SysJsonPoint[SYS_JSON_Global_START+sysjson_pos]=pjsonValue;
			pjsonValue = pjsonValue +JSONPARAM_DEFAULT_LEN;
			pdata = pdata + 1;
			sysjson_pos++;			
		}
	}	
	
	if(num>2)
	{
		templevel = SYSPARAM_FUNC_LEVEL(levels,3,2);
		sizenum = getJsonParaChildSize(templevel);
		pjsonValue = getJsonParaAddress(templevel);
		pdata = (ParamUnit *)(&SysParam.globalParam.KeyConfig);
		for(i=0; (i< sizenum)&&(i<(sizeof(globalParam_KeyType)/sizeof(ParamUnit)));i++)
		{
			if(sel)
				memcpy(pdata, pjsonValue+DEFAULT_VALUE_POS,2);
			SysJsonPoint[SYS_JSON_Global_START+sysjson_pos]=pjsonValue;
			pjsonValue = pjsonValue +JSONPARAM_DEFAULT_LEN;
			pdata = pdata + 1;	
			sysjson_pos++;			
		}
	}		
	
	if(num>3)
	{
		templevel = SYSPARAM_FUNC_LEVEL(levels,4,2);
		sizenum = getJsonParaChildSize(templevel);
		pjsonValue = getJsonParaAddress(templevel);
		pdata = (ParamUnit *)(&SysParam.globalParam.AssistKeyConfig);
		for(i=0; (i< sizenum)&&(i<(sizeof(globalParam_AssistKeyType)/sizeof(ParamUnit)));i++)
		{
			if(sel)
				memcpy(pdata, pjsonValue+DEFAULT_VALUE_POS,2);
			SysJsonPoint[SYS_JSON_Global_START+sysjson_pos]=pjsonValue;
			pjsonValue = pjsonValue +JSONPARAM_DEFAULT_LEN;
			pdata = pdata + 1;
			sysjson_pos++;			
		}
	}	
	
	if(num>4)
	{
		templevel = SYSPARAM_FUNC_LEVEL(levels,5,2);
		sizenum = getJsonParaChildSize(templevel);
		pjsonValue = getJsonParaAddress(templevel);
		pdata = (ParamUnit *)(&SysParam.globalParam.UIConfig);
		for(i=0; (i< sizenum)&&(i<(sizeof(globalParam_UIConfigType)/sizeof(ParamUnit)));i++)
		{
			if(sel)
				memcpy(pdata, pjsonValue+DEFAULT_VALUE_POS,2);
			SysJsonPoint[SYS_JSON_Global_START+sysjson_pos]=pjsonValue;
			pjsonValue = pjsonValue +JSONPARAM_DEFAULT_LEN;
			pdata = pdata + 1;	
			sysjson_pos++;			
		}
	}	

	if(num>5)//////////////////////
	{
		templevel = SYSPARAM_FUNC_LEVEL(levels,6,2);
		sizenum = getJsonParaChildSize(templevel);
		pjsonValue = getJsonParaAddress(templevel);
		pdata = (ParamUnit *)(&SysParam.globalParam.GroupAssistKeyConfig);
		for(i=0; (i< sizenum)&&(i<(sizeof(globalParam_GroupAssistKeyType)/sizeof(ParamUnit)));i++)
		{
			if(sel)
				memcpy(pdata, pjsonValue+DEFAULT_VALUE_POS,2);
			SysJsonPoint[SYS_JSON_Global_START+sysjson_pos]=pjsonValue;
			pjsonValue = pjsonValue +JSONPARAM_DEFAULT_LEN;
			pdata = pdata + 1;		
			sysjson_pos++;			
		}
	}	
	
	if(num>6)
	{
		templevel = SYSPARAM_FUNC_LEVEL(levels,7,2);
		sizenum = getJsonParaChildSize(templevel);
		pjsonValue = getJsonParaAddress(templevel);
		pdata = (ParamUnit *)(&SysParam.globalParam.DeviceAlarmParam);
		for(i=0; (i< sizenum)&&(i<(sizeof(globalParam_DeviceAlarmType)/sizeof(ParamUnit)));i++)
		{
			if(sel)
				memcpy(pdata, pjsonValue+DEFAULT_VALUE_POS,2);
			SysJsonPoint[SYS_JSON_Global_START+sysjson_pos]=pjsonValue;
			pjsonValue = pjsonValue +JSONPARAM_DEFAULT_LEN;
			pdata = pdata + 1;		
			sysjson_pos++;			
		}
	}	
	
	if(num>7)
	{
		templevel = SYSPARAM_FUNC_LEVEL(levels,8,2);
		sizenum = getJsonParaChildSize(templevel);
		pjsonValue = getJsonParaAddress(templevel);
		pdata = (ParamUnit *)(&SysParam.globalParam.DevicePeopleParam);
		for(i=0; (i< sizenum)&&(i<(sizeof(globalParam_DevicePeopleType)/sizeof(ParamUnit)));i++)
		{
			if(sel)
				memcpy(pdata, pjsonValue+DEFAULT_VALUE_POS,2);
			SysJsonPoint[SYS_JSON_Global_START+sysjson_pos]=pjsonValue;
			pjsonValue = pjsonValue +JSONPARAM_DEFAULT_LEN;
			pdata = pdata + 1;	
			sysjson_pos++;			
		}
	}	

	if(num>8)
	{
		templevel = SYSPARAM_FUNC_LEVEL(levels,9,2);
		sizenum = getJsonParaChildSize(templevel);
		pjsonValue = getJsonParaAddress(templevel);
		pdata = (ParamUnit *)(&SysParam.globalParam.DeviceInfirredParam);
		for(i=0; (i< sizenum)&&(i<(sizeof(globalParam_DeviceInfirredType)/sizeof(ParamUnit)));i++)
		{
			if(sel)
				memcpy(pdata, pjsonValue+DEFAULT_VALUE_POS,2);
			SysJsonPoint[SYS_JSON_Global_START+sysjson_pos]=pjsonValue;
			pjsonValue = pjsonValue +JSONPARAM_DEFAULT_LEN;
			pdata = pdata + 1;	
			sysjson_pos++;			
		}
	}	

	if(num>9)
	{
		templevel = SYSPARAM_FUNC_LEVEL(levels,10,2);
		sizenum = getJsonParaChildSize(templevel);
		pjsonValue = getJsonParaAddress(templevel);
		pdata = (ParamUnit *)(&SysParam.globalParam.DeviceWirelessParam);
		for(i=0; (i< sizenum)&&(i<(sizeof(globalParam_DeviceWirelessType)/sizeof(ParamUnit)));i++)
		{
			if(sel)
				memcpy(pdata, pjsonValue+DEFAULT_VALUE_POS,2);
			SysJsonPoint[SYS_JSON_Global_START+sysjson_pos]=pjsonValue;
			pjsonValue = pjsonValue +JSONPARAM_DEFAULT_LEN;
			pdata = pdata + 1;	
			sysjson_pos++;
		}
	}
}

static void  LoadDefaultParam_group(u8 sel)
{
	u8 i,j,k,num,triggerNum,groupActionId;
	uint64_t levels=0;
	uint64_t templevel;
	groupActionParamType *pParam;
	ParamUnit *pdata;
	u8 *pjsonValue;
	u16 sysjson_pos=0;
	
	levels = SYSPARAM_FUNC_LEVEL(levels,ParamIndex_group,1);
	num = getJsonParaStructSize(levels);
#if (IDANDINDEXAPART == 0)
	pParam = &SysParam.GroupAction[0];
#endif
	for(i=0; (i< num)&&(i<GROUPACTION_NUM_MAX);i++)
	{		
		templevel = SYSPARAM_FUNC_LEVEL(levels,i+1,2);
		triggerNum = getJsonParaStructSize(templevel);
		templevel = SYSPARAM_FUNC_LEVEL(templevel,1,3);
		pjsonValue = getJsonParaAddress(templevel);
		
		
#if (IDANDINDEXAPART == 1)
		memcpy(&groupActionId, pjsonValue+DEFAULT_VALUE_POS+26,2);
		if(groupActionId == 0 || groupActionId>GROUPACTION_NUM_MAX)
			continue;
		pParam = &SysParam.GroupAction[GROUPACTION_NUM_MAX-1];
#endif	

		pdata = (ParamUnit *)(&(pParam->baseParam));
		sysjson_pos=0;
		for(j=0;j<(sizeof(groupAction_BaseParamType)/sizeof(ParamUnit)) ;j++)
		{
			if(sel)
				memcpy(pdata, pjsonValue+DEFAULT_VALUE_POS,2);
			SysJsonPoint[SYS_JSON_Group_START+i*SYS_JSON_Group_LEN+sysjson_pos]=pjsonValue;
			pjsonValue = pjsonValue +JSONPARAM_DEFAULT_LEN;
			pdata = pdata + 1;
			sysjson_pos++;
		}

		pdata = (ParamUnit *)(&(pParam->actionParam));
		for(j=0;j<(sizeof(groupAction_actionParamType)/sizeof(ParamUnit)) ;j++)
		{
			if(sel)
				memcpy(pdata, pjsonValue+DEFAULT_VALUE_POS,2);
			SysJsonPoint[SYS_JSON_Group_START+i*SYS_JSON_Group_LEN+sysjson_pos]=pjsonValue;
			pjsonValue = pjsonValue +JSONPARAM_DEFAULT_LEN;
			pdata = pdata + 1;
			sysjson_pos++;
		}
		
		if(triggerNum >2)
		{
			triggerNum =triggerNum-2;
					
			pdata = (ParamUnit *)(&(pParam->tiggerParam[0]));
			for(k=0; (k< triggerNum)&&(k<GROUP_ACTION_TRIGGER_MAX);k++)
			{
				for(j=0;j<(sizeof(groupAction_TriggerCondType)/sizeof(ParamUnit)) ;j++)
				{
					if(sel)
						memcpy(pdata, pjsonValue+DEFAULT_VALUE_POS,2);
					SysJsonPoint[SYS_JSON_Group_START+i*SYS_JSON_Group_LEN+sysjson_pos]=pjsonValue;
					pjsonValue = pjsonValue +JSONPARAM_DEFAULT_LEN;
					pdata = pdata + 1;
					sysjson_pos++;
				}
			}
		}
#if (IDANDINDEXAPART == 0)	
		pParam++;
#endif
	}	
}

static void  LoadDefaultParam_part(u8 sel)
{
	u8 i,j,k,num,triggerNum,partId;
	uint64_t levels=0;
	uint64_t templevel;
	PartParamType *pParam;
	ParamUnit *pdata;
	u8 *pjsonValue;
	u16 sysjson_pos=0;
	
	levels = SYSPARAM_FUNC_LEVEL(levels,ParamIndex_part,1);
	num = getJsonParaStructSize(levels);
#if (IDANDINDEXAPART == 0)	
	pParam = &SysParam.Part[0];
#endif
	for(i=0; (i< num)&&(i<PART_NUM_MAX);i++)
	{		
		templevel = SYSPARAM_FUNC_LEVEL(levels,i+1,2);
		triggerNum = getJsonParaStructSize(templevel);
		templevel = SYSPARAM_FUNC_LEVEL(templevel,1,3);
		pjsonValue = getJsonParaAddress(templevel);
		
		
#if (IDANDINDEXAPART == 1)	
		memcpy(&partId, pjsonValue+DEFAULT_VALUE_POS+26,2);
		if(partId == 0 || partId>PART_NUM_MAX)
			continue;
		pParam = &SysParam.Part[partId-1];
#endif

		pdata = (ParamUnit *)(&(pParam->baseParam));
		sysjson_pos=0;
		for(j=0;j<(sizeof(PartBaseParamType)/sizeof(ParamUnit)) ;j++)
		{
			if(sel)
				memcpy(pdata, pjsonValue+DEFAULT_VALUE_POS,2);
			SysJsonPoint[SYS_JSON_Part_START+i*SYS_JSON_Part_LEN+sysjson_pos]=pjsonValue;
			pjsonValue = pjsonValue +JSONPARAM_DEFAULT_LEN;
			pdata = pdata + 1;
			sysjson_pos++;
		}
		
		if(triggerNum >1)
		{
			triggerNum =triggerNum-1;
					
			pdata = (ParamUnit *)(&(pParam->tiggerParam[0]));
			for(k=0; (k< triggerNum)&&(k<PART_TRIGGER_NUM_MAX);k++)
			{
				for(j=0;j<(sizeof(PartTriggerType)/sizeof(ParamUnit)) ;j++)
				{
					if(sel)
						memcpy(pdata, pjsonValue+DEFAULT_VALUE_POS,2);
					SysJsonPoint[SYS_JSON_Part_START+i*SYS_JSON_Part_LEN+sysjson_pos]=pjsonValue;
					pjsonValue = pjsonValue +JSONPARAM_DEFAULT_LEN;
					pdata = pdata + 1;
					sysjson_pos++;
				}
			}
		}
#if (IDANDINDEXAPART == 0)
		pParam++;
#endif
	}	
}

static u8  LoadDefaultParam(u8 sel)
{
	u8 ret=1;
	u32 i;
	
	memset(&SysParam,0,sizeof(SysParamType));
	for(i=0;i<SYS_JSON_MAX;i++)
	    SysJsonPoint[i]=NULL;
	
	LoadDefaultParam_single(sel);
	LoadDefaultParam_sensor(sel);
	LoadDefaultParam_comb(sel);
	LoadDefaultParam_group(sel);
	LoadDefaultParam_part(sel);
	LoadDefaultParam_global(sel);
	
	return ret;
}

void RestoreDefaultParametersexternalStartup(void)
{
	u8 ret;
	char file_name[30] ={0};	
	ee_ReadBytes((uint8_t *)file_name,JSON_PARAM_USING_START,30);
	ret = AnalysisJsonProc(file_name);			
	//load run parameter from outside json file
	ret = LoadDefaultParam(1);
	//load network parameter from eeprom
	memset(&SysNetParam,0,sizeof(NetParamType));
	ee_ReadBytes((uint8_t *)(&SysNetParam), NET_PARAM_START, sizeof(NetParamType));
	SysParam.globalParam.SupportParam.currentSupportID = SysNetParam.NetParam_IPAddress[3];
	//save run and network parameter in eeprom
	SysDataFun(DATA_MSG_COPY_TO_FLASH,0,0,0,0,0,0);
}

void RestoreFactorySetting(void)
{
	u8 ret;

	ret = AnalysisInsideJsonProc();
	
	ret = LoadDefaultParam(1);
	
//	memset(&SysNetParam,0,sizeof(NetParamType));
//	ee_ReadBytes((uint8_t *)(&SysNetParam), NET_PARAM_START, sizeof(NetParamType));							
//	SysParam.globalParam.SupportParam.currentSupportID = SysNetParam.NetParam_IPAddress[3];			
//	//save run and network parameter in eeprom		
//	SysDataFun(DATA_MSG_COPY_TO_FLASH,0,0,0,0,0,0);	
//	Write_FirstFlag();
	
	memset(&SysNetParam,0,sizeof(NetParamType));
	
	SysNetParam.NetParam_IPAddress[0] = IP_ADDR0;
	SysNetParam.NetParam_IPAddress[1] = IP_ADDR1;
	SysNetParam.NetParam_IPAddress[2] = IP_ADDR2;
	SysNetParam.NetParam_IPAddress[3] = IP_ADDR3;
	
	SysNetParam.NetParam_NetMask[0] = NETMASK_ADDR0;
	SysNetParam.NetParam_NetMask[1] = NETMASK_ADDR1;
	SysNetParam.NetParam_NetMask[2] = NETMASK_ADDR2;
	SysNetParam.NetParam_NetMask[3] = NETMASK_ADDR3;
	
	SysNetParam.NetParam_GateWay[0] = GW_ADDR0;
	SysNetParam.NetParam_GateWay[1] = GW_ADDR1;
	SysNetParam.NetParam_GateWay[2] = GW_ADDR2;
	SysNetParam.NetParam_GateWay[3] = GW_ADDR3;
	
	CreateSysMAC(&(SysNetParam.NetParam_Mac[0]));
	SysNetParam.Vlan = 0;
	SysNetParam.deviceType = 1; //UDP_DEVICE_CONTROLLER

	SysDataFun(DATA_MSG_COPY_TO_FLASH,0,0,0,0,0,0);	
	Write_FirstFlag();
}
u8 turnonRes;

//u8 texxxxxxxxxxxxxxt;
u8  sysParamInit(void)
{	
	static FIL FRIST;
	char name[10] = {"first"};
	u8 res;
	
	u8 ret,JsonSuccess,i = 0;
	u32 nettemp = 0xFFFFFFFF;
	char file_name[30] ={0};

	//判断是否时首次上电
	res = f_open(&FRIST,(const TCHAR*)name,FA_READ);//尝试打开这个文件
	if(res == FR_NO_FILE)
	{
		RestoreFactorySetting();
		//创建一个文件，路径为filename,如果文件原本就存在的话会被新建的文件覆盖掉！
		f_open(&FRIST,(const TCHAR*)name,FA_READ|FA_WRITE|FA_CREATE_ALWAYS);
		return 0;
	}
	else if(res == FR_OK) 
	{
		f_close(&FRIST);//如果打开成功，就关闭掉
	}
	//判断读取eep是否正常，如果异常间隔100ms最多读取4次
	while(1)
	{
		bsp_DelayMS(50);
		ee_ReadBytes((uint8_t *)(&nettemp), NET_PARAM_START, sizeof(u32));
		bsp_DelayMS(50);
		if(nettemp != 0xFFFFFFFF)
		{
			break;
		}
		i++;
		if(i >= 4)
		{
			//写保护+置位一个标志告诉主界面显示异常“”
			GPIO_SetBits(GPIO_PORT_I2C, I2C_WP_PIN);
//			f_open(&text,(const TCHAR*)name1,FA_READ|FA_WRITE|FA_CREATE_ALWAYS);
			break;
		}
	}

		
	turnonRes = Is_FirstTurnON();
#if(JSONANDPROGRAMAPART == 1)
	bsp_DelayMS(20);
	JsonSuccess = 0x01;
	ee_WriteBytes((uint8_t *)(&JsonSuccess), JSON_FILE_RECEPTION_SUCCESS, 1);
	bsp_DelayMS(20);
	ee_ReadBytes((uint8_t *)(&JsonSuccess), JSON_STARTUP_MODE, 1);
#endif
	switch(turnonRes)
	{
#if(JSONANDPROGRAMAPART == 1)		
		case Json_Newlyreceived:
			//外部启动，恢复默认值
			RestoreDefaultParametersexternalStartup();
			break;
		case Json_Watchdogreset:
			//读内部启动，恢复默认值
//			RestoreFactorySetting();
			//read inside json file
			ret = AnalysisInsideJsonProc();				
			//load run parameter from outside json file
			ret = LoadDefaultParam(1);				
			memset(&SysNetParam,0,sizeof(NetParamType));
			ee_ReadBytes((uint8_t *)(&SysNetParam), NET_PARAM_START, sizeof(NetParamType));							
			SysParam.globalParam.SupportParam.currentSupportID = SysNetParam.NetParam_IPAddress[3];			
			//save run and network parameter in eeprom		
			SysDataFun(DATA_MSG_COPY_TO_FLASH,0,0,0,0,0,0);	
			Write_FirstFlag();
			break;
#endif
		case Json_Recoveryparameters:
#if(JSONANDPROGRAMAPART == 1)	
			//根据启动标志启动，恢复默认参数
			if(JsonSuccess == 0x78)//外部启动
			{
				RestoreDefaultParametersexternalStartup(); ///需要修改
				Write_FirstFlag();
			}
			else//内部启动
#endif
			{
				RestoreFactorySetting();
			}
			break;
		case Json_Modify:
#if(JSONANDPROGRAMAPART == 1)	
			//启动标志为内部启动时，恢复默认参数内部启动。否侧外部启动不恢复默认参数
			if(JsonSuccess == 0x78)//外部启动
			{
				ee_ReadBytes((uint8_t *)file_name,JSON_PARAM_USING_START,30);
				ret = AnalysisJsonProc(file_name);
				if(ret == 0)//如果没有外部json文件
				{
					//下次内部启动
					JsonSuccess = 0xFF;//下次内部启动
					ee_WriteBytes((uint8_t *)(&JsonSuccess), JSON_STARTUP_MODE, 1);
					//read inside json file
					ret = AnalysisInsideJsonProc();				
					//load run parameter from outside json file
					ret = LoadDefaultParam(1);				
					memset(&SysNetParam,0,sizeof(NetParamType));
					ee_ReadBytes((uint8_t *)(&SysNetParam), NET_PARAM_START, sizeof(NetParamType));							
					SysParam.globalParam.SupportParam.currentSupportID = SysNetParam.NetParam_IPAddress[3];			
					//save run and network parameter in eeprom		
					SysDataFun(DATA_MSG_COPY_TO_FLASH,0,0,0,0,0,0);	
					Write_FirstFlag();
				}
				else
				{
					ret = LoadDefaultParam(0);
					memset(&SysNetParam,0,sizeof(NetParamType));
					ee_ReadBytes((uint8_t *)(&SysNetParam), NET_PARAM_START, sizeof(NetParamType));							
					SysParam.globalParam.SupportParam.currentSupportID = SysNetParam.NetParam_IPAddress[3];						
					SysDataFun(DATA_MSG_COPY_FROM_FLASH,0,0,0,0,0,0);
					Write_FirstFlag();
				}
			}
			else//内部启动
#endif
			{
				//read inside json file
				ret = AnalysisInsideJsonProc();				
				//load run parameter from outside json file
				ret = LoadDefaultParam(1);				
				memset(&SysNetParam,0,sizeof(NetParamType));
				ee_ReadBytes((uint8_t *)(&SysNetParam), NET_PARAM_START, sizeof(NetParamType));							
				SysParam.globalParam.SupportParam.currentSupportID = SysNetParam.NetParam_IPAddress[3];			
				//save run and network parameter in eeprom		
				SysDataFun(DATA_MSG_COPY_TO_FLASH,0,0,0,0,0,0);	
				Write_FirstFlag();
			}
			break;
		case Json_Other:
		case Json_Watchdognotreset:
#if(JSONANDPROGRAMAPART == 1)				
			//根据启动标志启动，不恢复默认参数
			if(JsonSuccess == 0x78)//外部启动
			{
				ee_ReadBytes((uint8_t *)file_name,JSON_PARAM_USING_START,30);
				ret = AnalysisJsonProc(file_name);
				ret = LoadDefaultParam(0);	
				memset(&SysNetParam,0,sizeof(NetParamType));
				ee_ReadBytes((uint8_t *)(&SysNetParam), NET_PARAM_START, sizeof(NetParamType));							
				SysParam.globalParam.SupportParam.currentSupportID = SysNetParam.NetParam_IPAddress[3];			
				//save run and network parameter in eeprom				
				SysDataFun(DATA_MSG_COPY_FROM_FLASH,0,0,0,0,0,0);	
			}
			else//内部启动
#endif
			{
				ret = AnalysisInsideJsonProc();
				ret = LoadDefaultParam(0);		
				SysDataFun(DATA_MSG_COPY_FROM_FLASH,0,0,0,0,0,0);	
			}
			break;
		default:
			break;				
	}
	//程序运行正常，清楚死机标志
	JsonSuccess = 0xFF;
	ee_WriteBytes((uint8_t *)(&JsonSuccess), JSON_FILE_RECEPTION_SUCCESS, 1);
	//死机次数清零
	JsonSuccess = 0x00;
	ee_WriteBytes((uint8_t *)(&JsonSuccess), CRASH_TIMES, 1);
	return ret;
}




/******************level to struct***********************/
u8 LevelToStruct(uint64_t levels,u16 **pdata,u16 *unitLen,u8 *moduletype,u8 *moduleID)
{
	u8 type=0;
	u8 id,lev1,lev2,lev3;
	
	*pdata =NULL;
	*unitLen =0;
	*moduletype=0;
	*moduleID=0;
	type = SYSPARAM_READ_LEVEL(levels,1);
	if(type ==0)
	{
		*pdata = (ParamUnit *)(&SysParam.globalParam.SupportParam);
		*unitLen = sizeof(globalParam_SupportType)/2;
		*moduletype = ParamIndex_sysparam;
		*moduleID = globalParamEnum_Support;
		return 1;
	}
	
			
	switch(type)
	{
		case ParamIndex_sysparam:
		{
			type = SYSPARAM_READ_LEVEL(levels,2);
			if(type ==0)
			{
				*pdata = (ParamUnit *)(&SysParam.globalParam.SupportParam);
				*unitLen = sizeof(globalParam_SupportType)/2;
				*moduletype = ENUM_SYSTEM;
				*moduleID = globalParamEnum_Support;	
				break;
			}
			
			switch(type)
			{
				case 	globalParamEnum_Support:
					*pdata = (ParamUnit *)(&SysParam.globalParam.SupportParam);
					*unitLen = sizeof(globalParam_SupportType)/2;
					*moduletype = ENUM_SYSTEM;
					*moduleID = globalParamEnum_Support;				
					break;
				
				case	globalParamEnum_Server:
					*pdata = (ParamUnit *)(&SysParam.globalParam.ServerParam);
					*unitLen = sizeof(globalParam_ServerType)/2;
					*moduletype = ENUM_SYSTEM;
					*moduleID = globalParamEnum_Server;
					break;
				
				case	globalParamEnum_Key:
					*pdata = (ParamUnit *)(&SysParam.globalParam.KeyConfig);
					*unitLen = sizeof(globalParam_KeyType)/2;
					*moduletype = ENUM_SYSTEM;
					*moduleID = globalParamEnum_Key;				
					break;
				
				case	globalParamEnum_AssistKey:
					*pdata = (ParamUnit *)(&SysParam.globalParam.AssistKeyConfig);
					*unitLen = sizeof(globalParam_AssistKeyType)/2;
					*moduletype = ENUM_SYSTEM;
					*moduleID = globalParamEnum_AssistKey;				
					break;
				
				case	globalParamEnum_UIConfig:
					*pdata = (ParamUnit *)(&SysParam.globalParam.UIConfig);
					*unitLen = sizeof(globalParam_UIConfigType)/2;
					*moduletype = ENUM_SYSTEM;
					*moduleID = globalParamEnum_UIConfig;					
					break;
				
				case	globalParamEnum_GroupAssistKey:
					*pdata = (ParamUnit *)(&SysParam.globalParam.GroupAssistKeyConfig);
					*unitLen = sizeof(globalParam_AssistKeyType)/2;
					*moduletype = ENUM_SYSTEM;
					*moduleID = globalParamEnum_GroupAssistKey;				
					break;
				
				case	globalParamEnum_DeviceAlarm:
					*pdata = (ParamUnit *)(&SysParam.globalParam.DeviceAlarmParam);
					*unitLen = sizeof(globalParam_DeviceAlarmType)/2;
					*moduletype = ENUM_SYSTEM;
					*moduleID = globalParamEnum_DeviceAlarm;				
					break;
				
				case	globalParamEnum_DevicePeople:
					*pdata = (ParamUnit *)(&SysParam.globalParam.DevicePeopleParam);
					*unitLen = sizeof(globalParam_DevicePeopleType)/2;
					*moduletype = ENUM_SYSTEM;
					*moduleID = globalParamEnum_DevicePeople;				
					break;
				
				case	globalParamEnum_DeviceInfirred:
					*pdata = (ParamUnit *)(&SysParam.globalParam.DeviceInfirredParam);
					*unitLen = sizeof(globalParam_DeviceInfirredType)/2;
					*moduletype = ENUM_SYSTEM;
					*moduleID = globalParamEnum_DeviceInfirred;				
					break;
				
				case	globalParamEnum_DeviceWireless:
					*pdata = (ParamUnit *)(&SysParam.globalParam.DeviceWirelessParam);
					*unitLen = sizeof(globalParam_DeviceWirelessType)/2;
					*moduletype = ENUM_SYSTEM;
					*moduleID = globalParamEnum_DeviceWireless;					
					break;
				 case	globalParamEnum_DeviceTrans:
					*pdata = (ParamUnit *)(&SysParam.globalParam.DeviceTransParam);
					*unitLen = sizeof(globalParam_DeviceTransType)/2;
					*moduletype = ENUM_SYSTEM;
					*moduleID = globalParamEnum_DeviceTrans;
					break;
				 case  globalParamEnum_DeviceHub:						//ghy 增加HUB
					*pdata = (ParamUnit *)(&SysParam.globalParam.DeviceHubParam);
					*unitLen = sizeof(globalParam_DeviceHubType)/2;
					*moduleID = globalParamEnum_DeviceHub;					
					break;
				
				default:
					break;
			}
		}
			break;
		
		case ParamIndex_single:
		{
			id = SYSPARAM_READ_LEVEL(levels,2);
			if(id >SINGLEACTION_NUM_MAX)
				break;
			
			if(id==0)
			{
				*pdata = (ParamUnit *)(&SysParam.SigAction[0].baseParam);
				*unitLen = (sizeof(SigAction_BaseParamType)/2)+sizeof(SigAction_ActionParamType)/2;
				*moduletype = ENUM_SINGLE_ACTION;
				*moduleID = 1;					
				break;
			}
			
			lev1 = SYSPARAM_READ_LEVEL(levels,3);
			if(lev1 <2)
			{
				*pdata = (ParamUnit *)(&SysParam.SigAction[id-1].baseParam);
				*unitLen = (sizeof(SigAction_BaseParamType)/2)+sizeof(SigAction_ActionParamType)/2;
			}
			else
			{
				lev2 = SYSPARAM_READ_LEVEL(levels,4);
				
				if(lev2 <2)
				{
					*pdata = (ParamUnit *)(&SysParam.SigAction[id-1].tiggerParam[lev1-2].cond);
					*unitLen = sizeof(SigAction_TriggerCondType)/2;	
				}
				else
				{
					*pdata = (ParamUnit *)(&SysParam.SigAction[id-1].tiggerParam[lev1-2].action[lev2-2]);
					*unitLen = sizeof(SigAction_TriggerActionType)/2;
				}
			}
			*moduletype = ENUM_SINGLE_ACTION;
			*moduleID = id;	
		}
			break;
		
		case ParamIndex_sensor:
		{
			id = SYSPARAM_READ_LEVEL(levels,2);
			if(id >SENSOR_MAX)
				break;

			if(id==0)
			{
				*pdata = (ParamUnit *)(&SysParam.Sensor[0]);
				*unitLen = sizeof(SensorParamType)/2;
				*moduletype = ENUM_SENSOR;
				*moduleID = 1;					
				break;
			}
			
			*pdata = (ParamUnit *)(&SysParam.Sensor[id-1]);
			*unitLen = sizeof(SensorParamType)/2;	
			*moduletype = ENUM_SENSOR;
			*moduleID = id;			
		}
			break;
		
		case ParamIndex_comb:
		{
			id = SYSPARAM_READ_LEVEL(levels,2);
			if(id >COMBACTION_NUM_MAX)
				break;
			
			if(id==0)
			{
				*pdata = (ParamUnit *)(&SysParam.CombAction[0].baseParam);
				*unitLen = sizeof(CombAction_BaseParamType)/2;	
				*moduletype = ENUM_COMBINATION_ACTION;
				*moduleID = 1;	
				break;
			}
			
			lev1 = SYSPARAM_READ_LEVEL(levels,3);
			if(lev1>(COMBACTION_SINGLE_NUM+1))
				break;
			
			if(lev1 ==0)
			{
				*pdata = (ParamUnit *)(&SysParam.CombAction[id-1].baseParam);
				*unitLen = sizeof(CombAction_BaseParamType)/2;	
				*moduletype = ENUM_COMBINATION_ACTION;
				*moduleID = 1;					
				break;
			}
	
			*moduletype = ENUM_COMBINATION_ACTION;
			*moduleID = id;				
			if(lev1 ==1)
			{
				*pdata = (ParamUnit *)(&SysParam.CombAction[id-1].baseParam);
				*unitLen = sizeof(CombAction_BaseParamType)/2;				
			}
			else
			{
				lev2 = SYSPARAM_READ_LEVEL(levels,4);
				if(lev2>(SINGLE_ACTION_TRIGGER_MAX+1))
					break;
				
				if(lev2 ==0)
				{
					*pdata = (ParamUnit *)(&SysParam.CombAction[id-1].actionParam[lev1-2].zeroDelayEnable);
					*unitLen = 2+(sizeof(SigAction_BaseParamType)/2)+sizeof(SigAction_ActionParamType)/2;
					break;
				}
				
				if(lev2 ==1)
				{
					*pdata = (ParamUnit *)(&SysParam.CombAction[id-1].actionParam[lev1-2].zeroDelayEnable);
					*unitLen = 2+(sizeof(SigAction_BaseParamType)/2)+sizeof(SigAction_ActionParamType)/2;
				}
				else
				{
					lev3 = SYSPARAM_READ_LEVEL(levels,5);
					if(lev3>(TRIGGER_ACTION_MAX+1))
						break;
					
					if(lev3 <2)
					{
						*pdata = (ParamUnit *)(&SysParam.CombAction[id-1].actionParam[lev1-2].SigParam.tiggerParam[lev2-2].cond);
						*unitLen = sizeof(SigAction_TriggerCondType)/2;						
					}
					else
					{
						*pdata = (ParamUnit *)(&SysParam.CombAction[id-1].actionParam[lev1-2].SigParam.tiggerParam[lev2-2].action[lev3-2]);
						*unitLen = sizeof(SigAction_TriggerActionType)/2;
					}
				}
			}
		}
			break;
		
		case ParamIndex_part:
		{
			id = SYSPARAM_READ_LEVEL(levels,2);
			if(id >PART_NUM_MAX)
				break;	
			
			if(id==0)
			{
				*pdata = (ParamUnit *)(&SysParam.Part[0].baseParam);
				*unitLen = sizeof(PartBaseParamType)/2;
				*moduletype = ENUM_PART;
				*moduleID = 1;					
				break;
			}

			lev1 = SYSPARAM_READ_LEVEL(levels,3);
			if(lev1>(PART_TRIGGER_NUM_MAX+1))
				break;

			*moduletype = ENUM_PART;
			*moduleID = id;	
			if(lev1 <2)
			{
				*pdata = (ParamUnit *)(&SysParam.Part[id-1].baseParam);
				*unitLen = sizeof(PartBaseParamType)/2;
			}	
			else
			{	
				lev2 = SYSPARAM_READ_LEVEL(levels,4);
				
				if(lev2 <2)
				{
					*pdata = (ParamUnit *)(&SysParam.Part[id-1].tiggerParam[lev1-2].cond);
					*unitLen = sizeof(PartTriggerCondType)/2;
				}
				else
				{
					*pdata = (ParamUnit *)(&SysParam.Part[id-1].tiggerParam[lev1-2].action);
					*unitLen = sizeof(PartTriggerActionType)/2;
				}				
			}
		}			
			break;
		
		case ParamIndex_group:
		{
			id = SYSPARAM_READ_LEVEL(levels,2);
			if(id >GROUPACTION_NUM_MAX)
				break;	
			
			if(id==0)
			{
				*pdata = (ParamUnit *)(&SysParam.GroupAction[0].baseParam);
				*unitLen = sizeof(groupAction_BaseParamType)/2;	
				*moduletype = ENUM_GROUP_ACTION;
				*moduleID = 1;					
				break;
			}

			*moduletype = ENUM_GROUP_ACTION;
			*moduleID = id;	
			lev1 = SYSPARAM_READ_LEVEL(levels,3);
			if(lev1>(GROUP_ACTION_TRIGGER_MAX+2))
				break;

			if(lev1 <2)
			{
				*pdata = (ParamUnit *)(&SysParam.GroupAction[id-1].baseParam);
				*unitLen = sizeof(groupAction_BaseParamType)/2;
			}	
			else if(lev1 ==2)
			{
				*pdata = (ParamUnit *)(&SysParam.GroupAction[id-1].actionParam);
				*unitLen = sizeof(groupAction_actionParamType)/2;
			}
			else
			{
				*pdata = (ParamUnit *)(&SysParam.GroupAction[id-1].tiggerParam[lev1-3]);
				*unitLen = sizeof(groupAction_TriggerCondType)/2;				
			}
		}
			break;
		
		default:
			break;
	}
	
	return 1;
}

/******************全局、本地相关判断***********************/
u8 IsJsonParamGlobal(u32 offset)
{
	u8 data;
	u8 *paddr;
	
	if(offset >=SYS_JSON_MAX)
		return 0;
	
	paddr = SysJsonPoint[offset];
	data = *(paddr+DEFAULT_GLOBAL_FLAG_POS);	
//	data = *(SysJsonPoint[offset]+DEFAULT_GLOBAL_FLAG_POS);
	if(data==1)
		return 1;
	return 0;
}

u16 GlobalParamCRCCal(void)
{
	uint32_t	i,len; 
	uint8_t		j,flag,dat[2];
	uint8_t		tmp;
	uint16_t	crcVal = 0xffff;
	
	len = sizeof(SysParamType)/2;
	for(i = 0; i < len; i++)
	{
		flag = IsJsonParamGlobal(i);
		if(flag==0)
			continue;
		if(i == (sizeof(SysParamType)-sizeof(SysParam.globalParam))/2)//如果是本架架号
		{
			continue;
		}
		
		memcpy(dat,(u8 *)((u16*)(&SysParam)+i),2);
		crcVal ^= dat[0];
		for(j = 0; j < 8; j++)
		{
			tmp = crcVal & 0x01;
			crcVal >>= 1;
			if(tmp)
			{
				crcVal ^= 0xa001;
			}
		 }
		
		crcVal ^= dat[1];
		for(j = 0; j < 8; j++)
		{
			tmp = crcVal & 0x01;
			crcVal >>= 1;
			if(tmp)
			{
				crcVal ^= 0xa001;
			}
		 }		
	 }		
	 return crcVal;
}

#endif
