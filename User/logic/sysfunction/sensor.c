/*
*********************************************************************************************************
*	                                            DoubleWires
*                           Commuication Controller For The Mining Industry
*
*                                  (c) Copyright 1994-2013  HNDZ 
*                       All rights reserved.  Protected by international copyright laws.
*
*
*    File    : sensor.c
*    Module  : system function
*    Version : V1.0
*    History :
*   -----------------
*              ?????????
*              Version  Date           By            Note
*              v1.0     
*
*********************************************************************************************************
*/

#define __SENSOR_C__

#include "sensor.h"
#include "udp_app_proc.h"
#include "sys_msg_def.h"

#if (LOGIC_SYSFUNC_EN == 1)

SensorModuleType 	sensorModule[SENSOR_MAX];
static u16 statefail[SENSOR_MAX]={0};

/*******************************************************************************************
*����������������ģ�ͳ�ʼ��
*��ڲ�����pParamȡֵ��
*            ����������ָ��
*          pStateȡֵ��
*             ������״ָ̬��
*          manualpStateȡֵ��
*             �ֶ����ƣ����ؼܣ�������״ָ̬��
*          sensorIdȡֵ��
*             ������id��
*����ֵ��  ��
*******************************************************************************************/
void SensorModuleInit(SensorParamType *pParam,SensorStateType *pState,SensorStateType *manualpState ,u8 sensorId)
{
	if((sensorId >= SENSOR_MAX)||(pParam == NULL))
		return;
	
	memset(&sensorModule[sensorId],0,sizeof(SensorModuleType));
	sensorModule[sensorId].pParam = pParam;
	sensorModule[sensorId].pState = pState;
	sensorModule[sensorId].manualState = manualpState;
	
	sensorModule[sensorId].apiFunc[SENSORAPI_NONE]= NULL;
	sensorModule[sensorId].apiFunc[SENSORAPI_MoreThan]= SensorApi_MoreThan;//�����������жϺ���
	sensorModule[sensorId].apiFunc[SENSORAPI_LessThan]= SensorApi_LessThan;
	sensorModule[sensorId].apiFunc[SENSORAPI_MoreThan_rate]= SensorApi_MoreThan_rate;
	sensorModule[sensorId].apiFunc[SENSORAPI_LessThan_rate]= SensorApi_LessThan_rate;
	sensorModule[sensorId].apiFunc[SENSORAPI_MoreThan_angle]= SensorApi_MoreThan_angle;
	sensorModule[sensorId].apiFunc[SENSORAPI_LessThan_angle]= SensorApi_LessThan_angle;
	sensorModule[sensorId].apiFunc[SENSORAPI_Sensor1MoreSensor2] = SensorApi_Sensor1MoreSensor2;
	sensorModule[sensorId].apiFunc[SENSORAPI_Sensor1LessSensor2] = SensorApi_Sensor1LessSensor2;	
	
}


//static void JudgeSensorMoreLimit(SensorModuleType *psensor,ParamUnit value)
//{
//	if(psensor->pState->state_fail == FailType_FAIL)
//		return;
//	if(psensor->pParam->moreLimitTimes == 0)
//		return;
//	if(psensor->pParam->moreLimitValue == 0)
//		return;
//	if(abs(psensor->CalculateValue_last-value)> psensor->pParam->moreLimitValue)
//	{
//		psensor->pState->state_fail = FailType_MORELIMIT;
//		psensor->MoreLimitTimes++;
//	}
//	else
//	{
//		psensor->MoreLimitTimes = 0;
//		psensor->CalculateValue_last = value;
//		psensor->pState->state_fail = FailType_NORMAL;
//	}
//	if(psensor->MoreLimitTimes>= psensor->pParam->moreLimitTimes)
//	{
//		psensor->MoreLimitTimes = 0;
//		psensor->CalculateValue_last = value;
//		psensor->pState->state_fail = FailType_NORMAL;
//	}

//}

/*******************************************************************************************
*�������������������ڲɼ�����
*��ڲ�����pParamȡֵ��
*            ����������ָ��
*          pStateȡֵ��
*             ������״ָ̬��
*          delayȡֵ��
*             ��ʱʱ��
*          sensorIdȡֵ��
*             ������id��
*����ֵ��  ��
*******************************************************************************************/
static u8 SensorFreqSample(SensorParamType *pParam,SensorStateType *pState,u16 delay,u8 sensorId)
{
	PointIDType point;
	u16 value;
	u8 ret;
	float freqMinTemp,freqMaxTemp;
	float caltemp,factorTemp,calvalue;
	int16_t valuetemp,orignal_angle;
	float orignalTemp;
	u8 alarmMode[2];
	
	point = pParam->pointID;
	ret = read_data (SUPPORT_DEFAULT_ADDR,point.driver,point.device,point.subDevice,point.point,&value);
	if(ret ==0)
		return 0;
	//adģ�����������ɼ�
	if(point.driver == DRIVER_AD)
	{		
		orignalTemp = ((float)value/1000)*((float)pParam->precision);  //AD??
		pState->originalValue = orignalTemp;
		
		freqMinTemp = (float)pParam->freqMin*(100 - FREQ_DEAL_OFFSET)/100;    //????????95% 
		freqMaxTemp = (float)pParam->freqMax*(100 + FREQ_DEAL_OFFSET)/100;    //????????105% 
		if((orignalTemp > freqMaxTemp)||(orignalTemp < freqMinTemp))	//??
		{
			if(pState->state_fail != FailType_FAIL)
			{
				WriteFault_API(FAULT_HOST_SENSOR,sensorId+1,FAULT_EVENT_FAULT,FAULT_STATUS_FAULT);
				
				if(pParam->alarmEnable == (ParamUnit)EnableType_ENABLE)
				{
					if(pParam->alarmType < (ParamUnit)AlarmType_Phone)
					{
						alarmMode[0] = pParam->alarmMode_sound;
						alarmMode[1] = pParam->alarmMode_light;
						CtrlAlarm((AlarmType)(pParam->alarmType),pParam->alarmTime,BaseCtrlType_STARTUP,alarmMode,ENUM_SENSOR,sensorId+1,ALARM_MODE_1);
					}
				}
			}
			
			pState->state_fail=FailType_FAIL;
//			pState->calculateValue=SENSOR_FAIL_VALUE;
			pState->rate=0;
			
			return 0;
		}
		
		if((pState->originalValue>= freqMinTemp)&&(pState->originalValue< (float)pParam->freqMin))//???????95%~100% ??????
			pState->originalValue = pParam->freqMin;
		else if((pState->originalValue<= freqMaxTemp)&&(pState->originalValue> (float)pParam->freqMax))//???????100%~105% ??????
			pState->originalValue = pParam->freqMax;
		
		caltemp = orignalTemp;//*(float)pParam->factor/100+pParam->offset;   //y = kx+b     jhy  220708
		factorTemp = (float)(pParam->freqMax-pParam->freqMin)/(float)(pParam->scopeMax - pParam->scopeMin);
		
		if(pParam->extParam ==SensorExtParamType_Reverse)//????---????
		{
			calvalue = pParam->scopeMax -(float)(caltemp-pParam->freqMin)/factorTemp;   
			if(calvalue<0)
				calvalue =0;
		}
		else
			calvalue = (float)(caltemp-pParam->freqMin)/factorTemp + pParam->scopeMin;//???
		
		pState->calculateValue = (ParamUnit)(calvalue);//*pParam->precision_logic);   // jhy  220708
//		JudgeSensorMoreLimit(&sensorModule[sensorId],pState->calculateValue*(float)pParam->factor/100+pParam->offset);
		pState->calculateValue = pState->calculateValue*(float)pParam->factor/100+pParam->offset;  // jhy  220708

//		if(pState->state_fail != FailType_MORELIMIT)
		{
			if(pState->state_fail!=FailType_NORMAL)
			{
				WriteFault_API(FAULT_HOST_SENSOR,sensorId+1,FAULT_EVENT_FAULT,FAULT_STATUS_NORMAL);
			}			
			pState->state_fail=FailType_NORMAL;
		}
	}
	//can�������ɼ�
	else  if((point.driver == DRIVER_CAN1)||(point.driver == DRIVER_CAN2))
	{		
		//�߶�
		if(point.device ==ID_HIGH)
		{
			orignalTemp = ((float)value/10)*((float)pParam->precision);   //CAN??
			valuetemp = pState->originalValue =orignalTemp;	
			
			freqMinTemp = (float)pParam->freqMin*(100 - FREQ_DEAL_OFFSET)/100;
			freqMaxTemp = (float)pParam->freqMax*(100 + FREQ_DEAL_OFFSET)/100;
			if((orignalTemp > freqMaxTemp)||(orignalTemp < freqMinTemp))	
			{
				if(pState->state_fail!=FailType_FAIL)
				{
					WriteFault_API(FAULT_HOST_SENSOR,sensorId+1,FAULT_EVENT_FAULT,FAULT_STATUS_FAULT);
					if(pParam->alarmEnable == (ParamUnit)EnableType_ENABLE)
					{
						if(pParam->alarmType < (ParamUnit)AlarmType_Phone)
						{
							alarmMode[0] = pParam->alarmMode_sound;
							alarmMode[1] = pParam->alarmMode_light;
							CtrlAlarm((AlarmType)(pParam->alarmType),pParam->alarmTime,BaseCtrlType_STARTUP,alarmMode,ENUM_SENSOR,sensorId+1,ALARM_MODE_1);
						}
					}					
				}				
				pState->state_fail=FailType_FAIL;
//				pState->calculateValue=SENSOR_FAIL_VALUE;
				pState->rate=0;
				return 0;
			}
			else
			{
				if(pState->state_fail!=FailType_NORMAL)
				{
					WriteFault_API(FAULT_HOST_SENSOR,sensorId+1,FAULT_EVENT_FAULT,FAULT_STATUS_NORMAL);
				}
				pState->state_fail=FailType_NORMAL;
			}
			
			if((pState->originalValue>= freqMinTemp)&&(pState->originalValue< (float)pParam->freqMin))
				pState->originalValue = pParam->freqMin;
			else if((pState->originalValue<= freqMaxTemp)&&(pState->originalValue> (float)pParam->freqMax))
				pState->originalValue = pParam->freqMax;
					
			caltemp = orignalTemp;//*(float)pParam->factor/100+pParam->offset;    // jhy  220708
			factorTemp = (float)(pParam->freqMax-pParam->freqMin)/(float)(pParam->scopeMax - pParam->scopeMin);
			if(pParam->extParam ==SensorExtParamType_Reverse)
			{
				calvalue = pParam->scopeMax -(float)(caltemp-pParam->freqMin)/factorTemp;
				if(calvalue<0)
					calvalue =0;
			}
			else
				calvalue = (float)(caltemp-pParam->freqMin)/factorTemp + pParam->scopeMin;

			pState->calculateValue = (ParamUnit)(calvalue);//*pParam->precision_logic);			// jhy  220708
			pState->calculateValue = pState->calculateValue*(float)pParam->factor/100+pParam->offset;   // jhy  220708
		}
		//�Ƕ�
		else
		{
			valuetemp = (int16_t)value;			
			if((valuetemp> ((int16_t)pParam->freqMax*100))||(valuetemp< (int16_t)pParam->freqMin*100))
			{
				if(pState->state_fail!=FailType_FAIL)
				{
					WriteFault_API(FAULT_HOST_SENSOR,sensorId+1,FAULT_EVENT_FAULT,FAULT_STATUS_FAULT);
					if(pParam->alarmEnable == (ParamUnit)EnableType_ENABLE)
					{
						if(pParam->alarmType < (ParamUnit)AlarmType_Phone)
						{
							alarmMode[0] = pParam->alarmMode_sound;
							alarmMode[1] = pParam->alarmMode_light;
							CtrlAlarm((AlarmType)(pParam->alarmType),pParam->alarmTime,BaseCtrlType_STARTUP,alarmMode,ENUM_SENSOR,sensorId+1,ALARM_MODE_1);
						}
					}					
				}
				
				pState->state_fail=FailType_FAIL;
				pState->calculateValue=0;
				pState->rate=0;				
				return 0;
			}
			else
			{
				if(pState->state_fail!=FailType_NORMAL)
				{
					WriteFault_API(FAULT_HOST_SENSOR,sensorId+1,FAULT_EVENT_FAULT,FAULT_STATUS_NORMAL);
				}
				pState->state_fail=FailType_NORMAL;	
			}

			pState->originalValue =value;			
			pState->calculateValue =(u16)(valuetemp/10); 		
		}
	}
	else
	{
		pState->originalValue =0;
		pState->calculateValue=value;
		pState->state_fail=FailType_NORMAL;
	}
	return 1;
}

/*******************************************************************************************
*�����������������ɼ��仯�ʺ��������磺�Ƽ��ٶȣ�
*��ڲ�����pParamȡֵ��
*            ����������ָ��
*          pStateȡֵ��
*             ������״ָ̬��
*          delayȡֵ��
*             ��ʱʱ��
*          sensorIdȡֵ��
*             ������id��
*����ֵ��  ��
*******************************************************************************************/
static u8 SensorSwitchSample(SensorParamType *pParam,SensorStateType *pState,u16 delay,u8 sensorId)
{
	return 1;
}

static u8 SensorRateProc(SensorModuleType *psensor,u16 delay)
{
	ParamUnit value,value_old,temp;
	
	psensor->tick += delay;
	if(psensor->tick >= 1000)
	{
		psensor->tick =0;
		value = psensor->pState->calculateValue;
		value_old = psensor->CalculateValue_old;
		
		if((value==0)&&(value_old==0))
			psensor->pState->rate =0;
		else if(value_old==0)
		{
			psensor->CalculateValue_old = psensor->pState->calculateValue;
		}
		else if(value >= value_old)
		{
			psensor->pState->rate = (ParamUnit)((float)(value-value_old)/(float)value_old*100);
			psensor->CalculateValue_old = psensor->pState->calculateValue;
		}
		else if(value < value_old)
		{
			temp = (ParamUnit)((float)(value_old-value)/(float)value_old*100);
			psensor->pState->rate = ParamUnit_SETHIGHTBIT(temp,1);
			psensor->CalculateValue_old = psensor->pState->calculateValue;
		}
	}
	return 1;
}


/*******************************************************************************************
*���������������������ϱ���������������
*��ڲ�����sensorIDȡֵ��
*            ������id��
*����ֵ��  ��
*******************************************************************************************/
static u8 sensorReport_sensor(u8 sensorID)
{
#ifdef SERVER_REPORT_ENABLE
	u8 temp[40];
	u8 serverID;
	u16 length;
		
	if(getSeverStatus() == 0)
		return 1;
	
	memset(temp,0,sizeof(temp));
	
	temp[0] = REPORTENUM_SENSOR;
	temp[1] = ENUM_SENSOR;
	temp[2] = sensorModule[sensorID].pParam->sensorID;
	length =sizeof(SensorStateType);
	memcpy(&temp[3],&length,2);
	memcpy(&temp[5],sensorModule[sensorID].pState,length);
	
	serverID = getSysServerAddr();
	UdpAppProcCtrl(AppProcCtrl_stateReport_indication,temp,5+length,serverID);//qinxi
#endif
	return 1;
}


/*******************************************************************************************
*�������������������ڲɼ�����
*��ڲ�����sensorIdȡֵ��
*            ������id��
*          delayȡֵ��
*             �ɼ�����
*����ֵ��  ��
*******************************************************************************************/
u8 sensorReportCount = 0;
static void SensorSample(u8 sensorId,u16 delay)
{
	PointIDType point;
	OnlineType  onlinestate=OnlineType_OFFLINE;
	u8 ret;
	
	if(sensorModule[sensorId].pParam ==NULL)
		return ;
	
	if(sensorModule[sensorId].pParam->enable == (ParamUnit)EnableType_DISABLE)
	{
		memset(sensorModule[sensorId].pState,0,sizeof(SensorStateType));
		return;
	}
	
	point = sensorModule[sensorId].pParam->pointID;
	if(get_device_state(SUPPORT_DEFAULT_ADDR,point.driver,point.device,point.subDevice,&onlinestate)==0)
		return;
	
	if(onlinestate !=sensorModule[sensorId].pState->state_online)
		sensorReport_sensor(sensorId);
	
	if(onlinestate == OnlineType_OFFLINE)
	{
		memset(sensorModule[sensorId].pState,0,sizeof(SensorStateType));
		return;		
	}
	
	sensorModule[sensorId].pState->state_online = OnlineType_ONLINE;
	if(sensorModule[sensorId].pParam->type == (ParamUnit)PointType_FREQ)
	{
		ret = SensorFreqSample(sensorModule[sensorId].pParam,sensorModule[sensorId].pState,delay,sensorId);
		if(ret)
			SensorRateProc(&sensorModule[sensorId],delay);
	}
	else
		SensorSwitchSample(sensorModule[sensorId].pParam,sensorModule[sensorId].pState,delay,sensorId);
	
	if(statefail[sensorId] !=sensorModule[sensorId].pState->state_fail)
	{
		sensorReportCount++;
		if(sensorReportCount>2)
		{
			sensorReportCount = 0;
			sensorReport_sensor(sensorId);
			statefail[sensorId] =sensorModule[sensorId].pState->state_fail;
		}
	}
}


/*******************************************************************************************
*������������ȡ����������ֵ
*��ڲ�����sensorIDȡֵ��
*            ������id��
*          valueȡֵ��
*             ����������ֵָ��
*����ֵ��  ��
*******************************************************************************************/
u8 getCalculateValue(u8 sensorID,ParamUnit *value)
{
	if(sensorID >= SENSOR_MAX)
		return 0;
	
	*value = sensorModule[sensorID].pState->calculateValue;
	return 1;
}


/*******************************************************************************************
*������������ȡ������״̬����
*��ڲ�����sensorIDȡֵ��
*            ������id��
*          valueȡֵ��
*             ������״ָ̬��
*����ֵ��  ��
*******************************************************************************************/
u8 getSensorState(u8 sensorID,SensorStateType *value)
{
	if(sensorID >= SENSOR_MAX)
		return 0;
	
	memcpy(value , sensorModule[sensorID].pState,sizeof(SensorStateType));
	return 1;
}


/*******************************************************************************************
*����������������ģ������
*��ڲ�������
*����ֵ��  ��
*******************************************************************************************/
void SensorModuleMngTask(void)
{	
	u16 i;  
	u16     delay = 10;
	PLATFORM_OS_TimeDly(2000+getSelfAddr()*5);  //jhy    Reduce concurrency 20220628
	
	while(1)
	{
		for(i=0; i<SENSOR_MAX;i++)
		{
			SensorSample(i,delay);
		}
		BeepProc();
		PLATFORM_OS_TimeDly(delay);	
	}
}


/*******************************************************************************************
*�����������������жϷ����������趨ֵ��
*��ڲ�����sensorIDȡֵ��
*            ������id��
*          sensorID2ȡֵ��
*             ������id2��
*          param1ȡֵ��
*             �趨ֵ1
*          param2ȡֵ��
*             �趨ֵ2
*����ֵ��  ���㣺1     �����㣺-1
*******************************************************************************************/
int SensorApi_MoreThan(u8 sensorID,u8 sensorID2,ParamUnit param1,ParamUnit param2,void *pdata)
{
	if((sensorID > SENSOR_MAX)||(sensorID==0))
		return -1;
	
	if((sensorModule[sensorID-1].pState->state_online == OnlineType_OFFLINE)||
	  (sensorModule[sensorID-1].pState->state_fail == FailType_FAIL)||
//		(sensorModule[sensorID-1].pState->state_fail == FailType_MORELIMIT)||
	  (sensorModule[sensorID-1].pParam->enable == (ParamUnit)EnableType_DISABLE))
	{
		if(param2 >0)
			return 1;
		else
			return -1;
	}
	
	if(sensorModule[sensorID-1].pState->calculateValue > param1)
	{
		TIME_TRACE
		printk("sensor id %d : %d > %d \r\n", sensorID,sensorModule[sensorID-1].pState->calculateValue,param1);
		return 1;
	}
	else
		return 0;
}

/*******************************************************************************************
*�����������������жϷ�����С���趨ֵ��
*��ڲ�����sensorIDȡֵ��
*            ������id��
*          sensorID2ȡֵ��
*             ������id2��
*          param1ȡֵ��
*             �趨ֵ1
*          param2ȡֵ��
*             �趨ֵ2
*����ֵ��  ���㣺1     �����㣺-1
*******************************************************************************************/
int SensorApi_LessThan(u8 sensorID,u8 sensorID2,ParamUnit param1,ParamUnit param2,void *pdata)
{
	if((sensorID > SENSOR_MAX)||(sensorID==0))
		return -1;

	if((sensorModule[sensorID-1].pState->state_online == OnlineType_OFFLINE)||
	  (sensorModule[sensorID-1].pState->state_fail == FailType_FAIL)||
//		(sensorModule[sensorID-1].pState->state_fail == FailType_MORELIMIT)||
	  (sensorModule[sensorID-1].pParam->enable == (ParamUnit)EnableType_DISABLE))
	{
		if(param2 >0)
			return 1;
		else
			return -1;
	}
	
	if(sensorModule[sensorID-1].pState->calculateValue < param1)
	{
		TIME_TRACE
		printk("sensor id %d : %d < %d \r\n", sensorID,sensorModule[sensorID-1].pState->calculateValue,param1);
		return 1;
	}
	else
		return 0;
}

/*******************************************************************************************
*�����������������жϷ��������ڱ仯��ֵ��
*��ڲ�����sensorIDȡֵ��
*            ������id��
*          sensorID2ȡֵ��
*             ������id2��
*          param1ȡֵ��
*             �趨ֵ1
*          param2ȡֵ��
*             �趨ֵ2
*����ֵ��  ���㣺1     �����㣺-1
*******************************************************************************************/
int SensorApi_MoreThan_rate(u8 sensorID,u8 sensorID2,ParamUnit param1,ParamUnit param2,void *pdata)
{
	if((sensorID > SENSOR_MAX)||(sensorID==0))
		return -1;
	
	if((sensorModule[sensorID-1].pState->state_online == OnlineType_OFFLINE)||
	  (sensorModule[sensorID-1].pState->state_fail == FailType_FAIL)||
	  (sensorModule[sensorID-1].pParam->enable == (ParamUnit)EnableType_DISABLE))
	{
		if(param2 >0)
			return 1;
		else
			return -1;
	}
	
	if(sensorModule[sensorID-1].pState->rate > param1)
	{
		TIME_TRACE
		printk("sensor id %d : rate %d > %d \r\n", sensorID,sensorModule[sensorID-1].pState->rate,param1);
		return 1;
	}
	else
		return 0;
}


/*******************************************************************************************
*�����������������жϷ�����С�ڱ仯��ֵ��
*��ڲ�����sensorIDȡֵ��
*            ������id��
*          sensorID2ȡֵ��
*             ������id2��
*          param1ȡֵ��
*             �趨ֵ1
*          param2ȡֵ��
*             �趨ֵ2
*����ֵ��  ���㣺1     �����㣺-1
*******************************************************************************************/
int SensorApi_LessThan_rate(u8 sensorID,u8 sensorID2,ParamUnit param1,ParamUnit param2,void *pdata)
{
	if((sensorID > SENSOR_MAX)||(sensorID==0))
		return -1;

	if((sensorModule[sensorID-1].pState->state_online == OnlineType_OFFLINE)||
	  (sensorModule[sensorID-1].pState->state_fail == FailType_FAIL)||
	  (sensorModule[sensorID-1].pParam->enable == (ParamUnit)EnableType_DISABLE))
	{
		if(param2 >0)
			return 1;
		else
			return -1;
	}
	
	if(sensorModule[sensorID-1].pState->rate < param1)
	{
		TIME_TRACE
		printk("sensor id %d : rate %d < %d \r\n", sensorID,sensorModule[sensorID-1].pState->rate,param1);
		return 1;
	}
	else
		return 0;
}


/*******************************************************************************************
*�����������������жϷ��������ڽǶ�ֵ��
*��ڲ�����sensorIDȡֵ��
*            ������id��
*          sensorID2ȡֵ��
*             ������id2��
*          param1ȡֵ��
*             �趨ֵ1
*          param2ȡֵ��
*             �趨ֵ2
*����ֵ��  ���㣺1     �����㣺-1
*******************************************************************************************/
int SensorApi_MoreThan_angle(u8 sensorID,u8 sensorID2,ParamUnit param1,ParamUnit param2,void *pdata)
{
	if((sensorID > SENSOR_MAX)||(sensorID==0))
		return -1;
	
	if((sensorModule[sensorID-1].pState->state_online == OnlineType_OFFLINE)||
	  (sensorModule[sensorID-1].pState->state_fail == FailType_FAIL)||
	  (sensorModule[sensorID-1].pParam->enable == (ParamUnit)EnableType_DISABLE))
	{
		if(param2 >0)
			return 1;
		else
			return -1;
	}
	
	if((int16_t)(sensorModule[sensorID-1].pState->calculateValue) > (int16_t)param1)
	{
		TIME_TRACE
		printk("sensor id %d : %d > %d \r\n", sensorID,sensorModule[sensorID-1].pState->calculateValue,param1);
		return 1;
	}
	else
		return 0;
}

/*******************************************************************************************
*�����������������жϷ�����С�ڽǶ�ֵ��
*��ڲ�����sensorIDȡֵ��
*            ������id��
*          sensorID2ȡֵ��
*             ������id2��
*          param1ȡֵ��
*             �趨ֵ1
*          param2ȡֵ��
*             �趨ֵ2
*����ֵ��  ���㣺1     �����㣺-1
*******************************************************************************************/
int SensorApi_LessThan_angle(u8 sensorID,u8 sensorID2,ParamUnit param1,ParamUnit param2,void *pdata)
{
	if((sensorID > SENSOR_MAX)||(sensorID==0))
		return -1;

	if((sensorModule[sensorID-1].pState->state_online == OnlineType_OFFLINE)||
	  (sensorModule[sensorID-1].pState->state_fail == FailType_FAIL)||
	  (sensorModule[sensorID-1].pParam->enable == (ParamUnit)EnableType_DISABLE))
	{
		if(param2 >0)
			return 1;
		else
			return -1;
	}
	
	if((int16_t)(sensorModule[sensorID-1].pState->calculateValue) < (int16_t)param1)
	{
		TIME_TRACE
		printk("sensor id %d : %d < %d \r\n", sensorID,sensorModule[sensorID-1].pState->calculateValue,param1);
		return 1;
	}
	else
		return 0;
}

/*******************************************************************************************
*�����������������жϷ�����������1���ڴ�����2��
*��ڲ�����sensorIDȡֵ��
*            ������id��
*          sensorID2ȡֵ��
*             ������id2��
*          param1ȡֵ��
*             �趨ֵ1
*          param2ȡֵ��
*             �趨ֵ2
*����ֵ��  ���㣺1     �����㣺-1
*******************************************************************************************/
int SensorApi_Sensor1MoreSensor2(u8 sensorID,u8 sensorID2,ParamUnit param1,ParamUnit param2,void *pdata)
{
	if((sensorID > SENSOR_MAX)||(sensorID==0))
		return -1;
	
	if((sensorID2 > SENSOR_MAX)||(sensorID2==0))
		return -1;

	if((sensorModule[sensorID2-1].pState->state_online == OnlineType_OFFLINE)||
	  (sensorModule[sensorID2-1].pState->state_fail == FailType_FAIL)||
	  (sensorModule[sensorID2-1].pParam->enable == (ParamUnit)EnableType_DISABLE))
	{
		if(param2 >0)
			return 1;
		else
			return -1;
	}
	
//	if((int16_t)(sensorModule[sensorID-1].pState->calculateValue)>(int16_t)(sensorModule[sensorID2-1].pState->calculateValue))
	{
		if((int16_t)(sensorModule[sensorID-1].pState->calculateValue)>((int16_t)sensorModule[sensorID2-1].pState->calculateValue+(int16_t)param1*sensorModule[sensorID-1].pParam->precision_logic))
		{
			TIME_TRACE
			printk("sensor id %d : %d < %d \r\n", sensorID,sensorModule[sensorID-1].pState->calculateValue,param1);
			return 1;
		}
		else
			return 0;
	}
}

/*******************************************************************************************
*�����������������жϷ�����������1С�ڴ�����2��
*��ڲ�����sensorIDȡֵ��
*            ������id��
*          sensorID2ȡֵ��
*             ������id2��
*          param1ȡֵ��
*             �趨ֵ1
*          param2ȡֵ��
*             �趨ֵ2
*����ֵ��  ���㣺1     �����㣺-1
*******************************************************************************************/
//sensor1<(sensor2+x)
int SensorApi_Sensor1LessSensor2(u8 sensorID,u8 sensorID2,ParamUnit param1,ParamUnit param2,void *pdata)
{
	if((sensorID > SENSOR_MAX)||(sensorID==0))
		return -1;
	
	if((sensorID2 > SENSOR_MAX)||(sensorID2==0))
		return -1;

	if((sensorModule[sensorID2-1].pState->state_online == OnlineType_OFFLINE)||
	  (sensorModule[sensorID2-1].pState->state_fail == FailType_FAIL)||
	  (sensorModule[sensorID2-1].pParam->enable == (ParamUnit)EnableType_DISABLE))
	{
		if(param2 >0)
			return 1;
		else
			return -1;
	}

//	if((int16_t)(sensorModule[sensorID-1].pState->calculateValue)>(int16_t)(sensorModule[sensorID2-1].pState->calculateValue))
	{
		if((int16_t)(sensorModule[sensorID-1].pState->calculateValue)<((int16_t)sensorModule[sensorID2-1].pState->calculateValue+(int16_t)param1*sensorModule[sensorID-1].pParam->precision_logic))
		{
			TIME_TRACE
			printk("sensor id %d : %d < %d \r\n", sensorID,sensorModule[sensorID-1].pState->calculateValue,param1);
			return 1;
		}
		else
			return 0;
	}
}

SensorApiType getSensorApi(u8 apiID)
{
	if(apiID >=SENSORAPI_MAX)
		return NULL;
	else
		return sensorModule[0].apiFunc[apiID];
}

/*******************************************************************************************
*���������������ֶ����Ʊ��ؼܴ���������
*��ڲ�����sensorIDȡֵ��
*            ������id��
*          manualStateȡֵ��
*             �ֶ����Ʊ��ؼܴ�����״̬����ָ��
*����ֵ��  ��
*******************************************************************************************/
void setManualSensorState(u8 sensorID,SensorStateType * manualState)
{
	sensorModule[sensorID-1].manualState->calculateValue = manualState->calculateValue;
	sensorModule[sensorID-1].manualState->originalValue = manualState->originalValue;
	sensorModule[sensorID-1].manualState->rate = manualState->rate;
	sensorModule[sensorID-1].manualState->state_fail = manualState->state_fail;
	sensorModule[sensorID-1].manualState->state_online = manualState->state_online;
}

/*******************************************************************************************
*������������ȡ�ֶ����Ʊ��ؼܴ���������
*��ڲ�����singleActionIdȡֵ��
*            ������id��
*          calcValueȡֵ��
*             ��ȡ������ֵ
*����ֵ��  ��
*******************************************************************************************/
char calcValuetmp[10] = {0};
u8 getManualSensorcalcValue(u8 singleActionId,char **calcValue)
{
	u8 sensorID;
	memset(calcValuetmp,0,sizeof(calcValuetmp));
	sensorID = getPartInSingleActionOfSensorId(singleActionId);//��ȡ���������ڵĲ����������Ĵ�����
	if(sensorID == 0)
		return 0;
	if(sensorModule[sensorID-1].manualState->state_fail == FailType_FAIL)
		return 0;
	if(sensorModule[sensorID-1].manualState->state_online == OnlineType_OFFLINE)
		return 0;

	if(sensorModule[sensorID-1].pParam->precision_logic == 10)
		sprintf(calcValuetmp,"%d.%d",sensorModule[sensorID-1].manualState->calculateValue/10,sensorModule[sensorID-1].manualState->calculateValue%10);
	else
		sprintf(calcValuetmp,"%d",sensorModule[sensorID-1].manualState->calculateValue);
	
	*calcValue = calcValuetmp;
	return 1;	
}


/*******************************************************************************************
*������������ȡ������״̬�����ϱ�������״̬�ϱ���ͬʱ�ϱ���
*��ڲ�����sensorDataȡֵ��
*            ��������������ָ��
*����ֵ��  ��
*******************************************************************************************/
u8 getSensorStateDataToReport(u8* sensorData)
{
	u8 i;
	u8 sensorIndex = 0;
	for(i = 0;i<SENSOR_MAX;i++)
	{
		if(sensorIndex>100)
			break;
		if(i>SENSOR_MAX)
			continue;
		if(sensorModule[i].pParam->sensorID == 0)
			continue;
//		if(sensorModule[i].pParam->enable == EnableType_DISABLE)
//			continue;
//		if(sensorModule[sensorID].pState->state_fail == FailType_FAIL)
//			continue;
//		if(sensorModule[sensorID].pState->state_online == OnlineType_OFFLINE)
//			continue;
		sensorData[sensorIndex++] = i+1;
		sensorData[sensorIndex++] = 0;
		memcpy(&sensorData[sensorIndex],sensorModule[i].pState,sizeof(SensorStateType));
		sensorIndex += sizeof(SensorStateType);
	}	
	return sensorIndex;
}


#endif
