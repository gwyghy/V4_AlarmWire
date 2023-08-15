/*
*********************************************************************************************************
*	                                            DoubleWires
*                           Commuication Controller For The Mining Industry
*
*                                  (c) Copyright 1994-2013  HNDZ 
*                       All rights reserved.  Protected by international copyright laws.
*
*
*    File    : part.c
*    Module  : system function
*    Version : V1.0
*    History :
*   -----------------
*              实现部件模型功能
*              Version  Date           By            Note
*              v1.0     
*
*********************************************************************************************************
*/

#define __PART_C__
#include "ProjectConfig.h"
#include "part.h"
#include "sys_data_type.h"
#include "udp_app_proc.h"
#include "sys_msg_def.h"
#include "math.h"


#if (LOGIC_SYSFUNC_EN == 1)

partModuleType 	partModule[PART_NUM_MAX];

void partModuleInit(void)
{
	memset(partModule,0,sizeof(partModule));
}

/*******************************************************************************************
*功能描述：部件模型数据加载
*入口参数：moduleID取值：
*            模型ID
*          pParam取值：
*             部件参数指针
*          pState取值：
*             部件状态指针
*返回值：  无
*******************************************************************************************/
void partModuleLoad(u8 moduleID,PartParamType *pParam,PartStateType *pState)
{
	if((pState == NULL)||(pParam == NULL)||(moduleID >=PART_NUM_MAX))
		return;
	
	partModule[moduleID].pParam = pParam;
	partModule[moduleID].pState = pState;
}

static u8 TiggerCondFound(PartTriggerCondType *ptrigger)
{
	SensorApiType apiFunc;
	int ret=0;
	
	if((ptrigger == NULL)||(ptrigger->TriggerEnable == EnableType_DISABLE))
		return 0;
	
	if(ptrigger->sensorID ==0)
		return 0;
	
	apiFunc = getSensorApi(ptrigger->methodID);
	if(apiFunc==NULL)
		return 0;
	
	ret = apiFunc(ptrigger->sensorID,0,ptrigger->sensorParam1,ptrigger->sensorParam2,0);
	return ((ret<0)?0:ret);
}

#if (APPLICATION_SCENARIO_SELECTION == LIVE_CHICKEN_AND_RABBIT_WORKING_FACE)
u16 lastMeterValue = 0;
u16 SensorFailState = 0;
#endif

/*******************************************************************************************
*功能描述：部件模型上报传感器状态
*入口参数：sensorID取值：
*            传感器模型ID
*          value取值：
*             传感器参数指针
*返回值：  无
*******************************************************************************************/
static u8 partReport_sensor(u8 sensorID,SensorStateType *value)
{
#ifdef SERVER_REPORT_ENABLE
	u8 temp[20];
	u8 serverID;
	u16 length;
	
	if(getSeverStatus() == 0)
		return 1;

	memset(temp,0,sizeof(temp));
	
	temp[0] = REPORTENUM_SENSOR;
	temp[1] = ENUM_SENSOR;
	temp[2] = sensorID;
	length =sizeof(SensorStateType);
	memcpy(&temp[3],&length,2);
	memcpy(&temp[5],value,length);
	#if (APPLICATION_SCENARIO_SELECTION == LIVE_CHICKEN_AND_RABBIT_WORKING_FACE)
	if(sensorID == 2)
	{
		if(abs(lastMeterValue-value->calculateValue)>100)
		{
			memcpy(&temp[9],&lastMeterValue,2);
			SensorFailState = FailType_MORELIMIT;
			memcpy(&temp[13],&SensorFailState,2);
		}
	}
	#endif
	
	serverID = getSysServerAddr();
	UdpAppProcCtrl(AppProcCtrl_stateReport_indication,temp,5+length,serverID);//qinxi
#if (APPLICATION_SCENARIO_SELECTION == LIVE_CHICKEN_AND_RABBIT_WORKING_FACE)	
	if(sensorID == 2)
		lastMeterValue = value->calculateValue;
#endif
#endif
	return 1;
}

//u16 tmpsensor[6] = {0};
/*******************************************************************************************
*功能描述：部件周期性处理函数
*入口参数：sensorID取值：
*            传感器模型ID
*          delay取值：
*             延时时间
*返回值：  无
*******************************************************************************************/
u8 partCycleProcess(u8 moduleID,u16 delay)   //1000ms
{
	PartParamType *pParam ;
	PartStateType *pState ;
	u8 i,ret;
	ParamUnit value;
	int value_abs;
	u8 alarmMode[2];
	u8 loginfo;
	SensorStateType sensorTmp;
	static u8 overflow[SENSOR_MAX] = {0};
	if(moduleID >=PART_NUM_MAX)
		return 0;
	
	pParam = partModule[moduleID].pParam;
	pState = partModule[moduleID].pState;	
	if((pState == NULL)||(pParam == NULL))
		return 0;
	
	for(i=0; i<PART_TRIGGER_NUM_MAX;i++)
	{
		if(pParam->tiggerParam[i].cond.TriggerEnable == EnableType_DISABLE)
			continue;
		
		ret = TiggerCondFound(&(pParam->tiggerParam[i].cond));
		if(ret)
		{
			if(pParam->tiggerParam[i].action.alarmEnable == EnableType_ENABLE)
			{
				if(partModule[moduleID].alarm_flag[i] ==0)
				{
					alarmMode[0] = pParam->tiggerParam[i].action.alarmMode_sound;
					alarmMode[1] = pParam->tiggerParam[i].action.alarmMode_light;
					CtrlAlarm((AlarmType)(pParam->tiggerParam[i].action.alarmType),0,BaseCtrlType_STARTUP,alarmMode,ENUM_PART,moduleID+1,ALARM_MODE_1);
					partModule[moduleID].alarm_flag[i] =1;
					
					loginfo =moduleID+1;
					WriteLog_API(pParam->tiggerParam[i].cond.sensorID,LOG_TYPE_ALARM,LOG_STATUS_SENSOR_ALARM,&loginfo,1);
				}
			}
			else
				partModule[moduleID].alarm_flag[i] =0;			
		}
		else
		{
			if(pParam->tiggerParam[i].action.alarmEnable == EnableType_ENABLE)
			{
				if(partModule[moduleID].alarm_flag[i] !=0)
				{
					CtrlAlarm((AlarmType)(pParam->tiggerParam[i].action.alarmType),0,BaseCtrlType_STOP,0,ENUM_PART,moduleID+1,ALARM_MODE_NONE);
					partModule[moduleID].alarm_flag[i] =0;
					
					loginfo =moduleID+1;
					WriteLog_API(pParam->tiggerParam[i].cond.sensorID,LOG_TYPE_ALARM,LOG_STATUS_SENSOR_NORMAL,&loginfo,1);					
				}
			}
			else
				partModule[moduleID].alarm_flag[i] =0;
		}
		
		if((pParam->tiggerParam[i].cond.sensorID ==0)||(pParam->tiggerParam[i].cond.sensorID >SENSOR_MAX))
			continue;
		
		getCalculateValue(pParam->tiggerParam[i].cond.sensorID -1 ,&value);
		getSensorState(pParam->tiggerParam[i].cond.sensorID - 1,&sensorTmp);
		partModule[moduleID].ticks_delay[i] +=delay;
		if(partModule[moduleID].ticks_delay[i] >= (int)(pParam->tiggerParam[i].action.reportInterval)*100)   //1000  jhy 220625    more data test
		{
			
//			if(tmpsensor[pParam->tiggerParam[i].cond.sensorID-1] == 0)
//				tmpsensor[pParam->tiggerParam[i].cond.sensorID-1] = getSelfAddr();
//			
//			sensorModule[pParam->tiggerParam[i].cond.sensorID-1].pState->calculateValue = tmpsensor[pParam->tiggerParam[i].cond.sensorID-1];
//			sensorModule[pParam->tiggerParam[i].cond.sensorID-1].pState->state_online = OnlineType_ONLINE;
//			if(tmpsensor[pParam->tiggerParam[i].cond.sensorID-1]== 999)
//				tmpsensor[pParam->tiggerParam[i].cond.sensorID-1] = 1;
//			else
//				tmpsensor[pParam->tiggerParam[i].cond.sensorID-1]++;
			
			partModule[moduleID].ticks_delay[i] =0;
			partReport_sensor(pParam->tiggerParam[i].cond.sensorID,&sensorTmp);
			partModule[moduleID].value_bak[i] =value;
			continue;
		}
		
		value_abs =abs(value -partModule[moduleID].value_bak[i] );
		if(value_abs > pParam->tiggerParam[i].action.reportScope)
		{
			partModule[moduleID].ticks_delay[i] =0;
			partReport_sensor(pParam->tiggerParam[i].cond.sensorID,&sensorTmp);
			partModule[moduleID].value_bak[i] =value;
			continue;
		}
		if((value > pParam->tiggerParam[i].action.reportMax)||(value < pParam->tiggerParam[i].action.reportMin))
		{
			if(overflow[moduleID] == 0)
			{
				overflow[moduleID] = 1;
				partModule[moduleID].ticks_delay[i] =0;
				partReport_sensor(pParam->tiggerParam[i].cond.sensorID,&sensorTmp);
				partModule[moduleID].value_bak[i] =value;
				continue;
			}
		}
		else
		{
			if(value_abs > pParam->tiggerParam[i].action.reportScope)
			{
				overflow[moduleID] = 0;
			}
		}
		partModule[moduleID].value_bak[i] =value;
			
	}
	
	return 1;
}

/*******************************************************************************************
*功能描述：获取部件中单动作所属的传感器ID
*入口参数：singleActionId取值：
*            单动作模型ID
*返回值：  0：无传感器     >0 :传感器id
*******************************************************************************************/
uint8_t getPartInSingleActionOfSensorId(u8 singleActionId)
{
	u8 i;
	u8 sensorId = 0;
	for(i = 0 ;i<PART_NUM_MAX;i++)
	{
		if(i>=2)
			break;
		if(partModule[i].pParam->baseParam.ActionID1 == singleActionId || partModule[i].pParam->baseParam.ActionID2 == singleActionId)
		{
			if(partModule[i].pParam->tiggerParam[0].cond.sensorID != 0)
			{		
					sensorId = partModule[i].pParam->tiggerParam[0].cond.sensorID;
					break;
			}
		}
	}
	return sensorId;
}

#endif
