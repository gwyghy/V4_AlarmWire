/*
*********************************************************************************************************
*	                                            DoubleWires
*                           Commuication Controller For The Mining Industry
*
*                                  (c) Copyright 1994-2013  HNDZ 
*                       All rights reserved.  Protected by international copyright laws.
*
*
*    File    : alarm.c
*    Module  : system function
*    Version : V1.0
*    History :
*   -----------------
*              实现报警模型功能
*              Version  Date           By            Note
*              v1.0     
*
*********************************************************************************************************
*/

#define __ALARM_C__

#include "alarm.h"

#if (LOGIC_SYSFUNC_EN == 1)

AlarmModuleType AlarmModule;
static AlarmCtrlBufType    AlarmCtrlMng;
static u8 AlarmAllStopFlag=0;
static u8 alarmCmdFunc(AlarmUnitType *pUnit,int alarmTime,BaseCtrlType cmd,u8 mode,u8 ctrlEventType,u8 ctrlEventID,u8 alarmMode);

static void AlarmCtrlBufInit(void)
{
	memset(AlarmCtrlMng.buf, 0 ,sizeof(AlarmCtrlMng.buf));
	AlarmCtrlMng.Cnt=0;
}

void AlarmModuleInit(void)
{
	u8 i;
	
	memset(&AlarmModule,0,sizeof(AlarmModuleType));
	for(i=0;i<ALARM_UNIT_PHONE;i++)
		AlarmModule.alarmDev[i].alarmFunc = alarmFunc;
	AlarmModule.alarmDev[ALARM_UNIT_PHONE].alarmFunc = alarmFunc_phone;
	
	AlarmModule.alarmDev[ALARM_UNIT_SOUND].enable = EnableType_ENABLE;
	AlarmModule.alarmDev[ALARM_UNIT_LIGHT].enable = EnableType_ENABLE;
	AlarmModuleConfig(ALARM_UNIT_LIGHT,DRIVER_CAN1,ID_ALARM,ALARM_LED);
	AlarmModuleConfig(ALARM_UNIT_SOUND,DRIVER_CAN1,ID_ALARM,ALARM_BEEP);
	AlarmCtrlBufInit();
}

void AlarmModuleConfig(uint8_t type,uint8_t DriverId,uint8_t DeviceId,uint8_t PointId)
{
	if(type >=ALARM_UNIT_PHONE)
		return;
	
	AlarmModule.alarmDev[type].point.driver = DriverId;
	AlarmModule.alarmDev[type].point.device = DeviceId;
	AlarmModule.alarmDev[type].point.point = PointId;
}

void AlarmModuleConfig_phone(uint8_t PhoneId)
{
	AlarmModule.alarmDev[ALARM_UNIT_PHONE].phoneAddr = PhoneId;
}

u8 AlarmModuleEnable(EnableType enable)
{
	u8 i;
	if(enable == EnableType_DISABLE)
	{
		for(i=0;i<ALARM_UNIT_PHONE;i++)
		{
			if(AlarmModule.alarmDev[i].state != BaseRunType_RUN)
				continue;
//			data[0] = BaseCtrlType_STOP;
//			ret =AlarmModule.alarmDev[i].alarmFunc(AlarmModule.alarmDev[i].point,AlarmModule.alarmDev[i].phoneAddr,&data[0],0,1);
//			if(ret)
//			{
//				AlarmModule.alarmDev[i].state = BaseRunType_STOP;
//				AlarmModule.alarmDev[i].alarmTime = 0;
//				AlarmModule.alarmDev[i].ticks =0;
//			}
				write_data(SUPPORT_DEFAULT_ADDR,AlarmModule.alarmDev[i].point.driver,AlarmModule.alarmDev[i].point.device,0,AlarmModule.alarmDev[i].point.point,0);
		}
	}
	for(i=0;i<ALARM_UNIT_PHONE;i++)
		AlarmModule.alarmDev[i].enable = enable;

	return 1;
}

u8 AlarmModuleEnable_phone(EnableType enable)
{
	AlarmModule.alarmDev[ALARM_UNIT_PHONE].enable = enable;
	return 1;
}

static u8 InsAlarmCtrlBufQueue(AlarmCtrlType *pdata,AlarmCtrlType *pmode)
{	
	int i;
	u8  flag=0;
	u8  save=0xff;
	u8  insert=0;
	
	pmode->eventMode =ALARM_MODE_NONE;
	if(AlarmCtrlMng.Cnt <ALARMCTRL_BUF_LEN)
	{
		for(i=0;i<ALARMCTRL_BUF_LEN;i++)
		{
			if(AlarmCtrlMng.buf[i].use )
			{
				if((AlarmCtrlMng.buf[i].eventType == pdata->eventType)&&
				  (AlarmCtrlMng.buf[i].eventID   == pdata->eventID)
				  /*&&(AlarmCtrlMng.buf[i].eventMode == pdata->eventMode)*/)
				{
					flag =1;
					break;
				}
			}			
		}
		
		if(flag)
			return 0;
		
		for(i=0;i<ALARMCTRL_BUF_LEN;i++)
		{			
			if(AlarmCtrlMng.buf[i].use ==0)
			{
				AlarmCtrlMng.buf[i].eventType = pdata->eventType;
				AlarmCtrlMng.buf[i].eventID   = pdata->eventID;
				AlarmCtrlMng.buf[i].eventMode = pdata->eventMode;
				AlarmCtrlMng.buf[i].alarmTime = pdata->alarmTime*LOGIC_TIME_MS;
				AlarmCtrlMng.buf[i].use =1;
				AlarmCtrlMng.Cnt ++;
				insert = i;
				break;
			}
		}
		
		for(i=0;i<ALARMCTRL_BUF_LEN;i++)
		{
			if(AlarmCtrlMng.buf[i].use )
			{
				if(AlarmCtrlMng.buf[i].eventMode == ALARM_MODE_2)
				{
					memcpy(pmode,&AlarmCtrlMng.buf[i],sizeof(AlarmCtrlType));
					return 1;
				}
				else if(AlarmCtrlMng.buf[i].eventMode == ALARM_MODE_1)
				{
					save = i;
				}				
			}			
		}	
		if(save!=0xff)
		{
			memcpy(pmode,&AlarmCtrlMng.buf[save],sizeof(AlarmCtrlType));
			return (save+1);
		}
		else
			return (insert+1);
	}
	return 0;
}

static u8 DelAlarmCtrlBufQueue(AlarmCtrlType *pdata,AlarmCtrlType *pmode)
{
	int i;
	u8  save=0xff;
	
	pmode->eventMode =ALARM_MODE_NONE;
	if(AlarmCtrlMng.Cnt)
	{	
		for(i=0;i<ALARMCTRL_BUF_LEN;i++)
		{
			if(AlarmCtrlMng.buf[i].use )
			{
				if((AlarmCtrlMng.buf[i].eventType == pdata->eventType)&&
				  (AlarmCtrlMng.buf[i].eventID   == pdata->eventID)
				  /*&&(AlarmCtrlMng.buf[i].eventMode == pdata->eventMode)*/)
				{
					AlarmCtrlMng.buf[i].eventType = 0;
					AlarmCtrlMng.buf[i].eventID   = 0;
					AlarmCtrlMng.buf[i].use =0;
					AlarmCtrlMng.buf[i].eventMode = 0;
					AlarmCtrlMng.buf[i].alarmTime = 0;
					AlarmCtrlMng.Cnt --;
					break;
				}
			}
		}
		
		for(i=0;i<ALARMCTRL_BUF_LEN;i++)
		{
			if(AlarmCtrlMng.buf[i].use )
			{
				if(AlarmCtrlMng.buf[i].eventMode == ALARM_MODE_2)
				{
					memcpy(pmode,&AlarmCtrlMng.buf[i],sizeof(AlarmCtrlType));
					return (i+1);
				}
				else if(AlarmCtrlMng.buf[i].eventMode == ALARM_MODE_1)
				{
					save = i;
				}				
			}			
		}	
		if(save!=0xff)
		{
			memcpy(pmode,&AlarmCtrlMng.buf[save],sizeof(AlarmCtrlType));	
			return (save+1);
		}
	}
	return 0;
}

static u8 DelAllAlarmCtrlBufQueue(void)
{
	int i;
	
	if(AlarmCtrlMng.Cnt)
	{	
		for(i=0;i<ALARMCTRL_BUF_LEN;i++)
		{
			if(AlarmCtrlMng.buf[i].use )
			{
				AlarmCtrlMng.buf[i].eventType = 0;
				AlarmCtrlMng.buf[i].eventID   = 0;
				AlarmCtrlMng.buf[i].use =0;
				AlarmCtrlMng.buf[i].eventMode = 0;
				AlarmCtrlMng.buf[i].alarmTime = 0;
			}
		}
		AlarmCtrlMng.Cnt =0;
	}
	return 1;
}

u8 alarmFunc(PointIDType point,u8 phoneAddr,void *pdata,u8 alarmMode,u8 beepflag)
{
	u8 ret;
	
	if(pdata ==NULL)
		return 0;
	
	if(*((u8 *)pdata) == BaseCtrlType_STARTUP)
		;    //tode set alarm param

	if(alarmMode == ALARM_MODE_1)
	{
		if(AlarmModule.alarmDev[ALARM_UNIT_SOUND].enable != EnableType_DISABLE)
			SetCanDeviceAPPMsg(point.driver-1, point.device, 0, point.point, RED_ALWAYS);
	}
	else if(alarmMode == ALARM_MODE_2)
	{
		if(AlarmModule.alarmDev[ALARM_UNIT_SOUND].enable != EnableType_DISABLE)
			SetCanDeviceAPPMsg(point.driver-1, point.device, 0, point.point, RED20_BLUE20);
	}	
	if(AlarmModule.alarmDev[ALARM_UNIT_SOUND].enable != EnableType_DISABLE)
		ret =write_data(SUPPORT_DEFAULT_ADDR,point.driver,point.device,0,point.point,pdata);
	
	if((*((u8 *)pdata) == BaseCtrlType_STARTUP)||(beepflag))
		write_data (SUPPORT_DEFAULT_ADDR,DRIVER_IO,0,0,DevIOOutputPointType_BEEP_ACTION,pdata);
	return ret;
}

u8 alarmFunc_phone(PointIDType point,u8 phoneAddr,void *pdata,u8 alarmMode,u8 beepflag)
{
	u8 ret;
	
	ret =ctrl_phone(phoneAddr,pdata);
	return ret;
}

static u8 alarmCmdFunc(AlarmUnitType *pUnit,int alarmTime,BaseCtrlType cmd,u8 mode,u8 ctrlEventType,u8 ctrlEventID,u8 alarmMode)
{
	u8 data[2],processMode;
	int processTime;
	u8 ret=1;
	AlarmCtrlType alarmCtrl,saveCtrl;
	
	switch(cmd)
	{
		case BaseCtrlType_STARTUP:
		{		
			alarmCtrl.eventType =ctrlEventType;
			alarmCtrl.eventID =ctrlEventID;
			alarmCtrl.eventMode =alarmMode;
			alarmCtrl.alarmTime =alarmTime*LOGIC_TIME_MS;
			if(pUnit == &AlarmModule.alarmDev[ALARM_UNIT_LIGHT])
			{
				ret =InsAlarmCtrlBufQueue(&alarmCtrl,&saveCtrl);
				if(ret)
				{
					pUnit->AlarmEventType = AlarmCtrlMng.buf[ret-1].eventType;
					pUnit->AlarmEventID = AlarmCtrlMng.buf[ret-1].eventID;
					processMode =saveCtrl.eventMode;
					processTime =saveCtrl.alarmTime;
				}
				else
					break;
			}
			else
			{
				processMode =alarmMode;
				processTime =alarmTime*LOGIC_TIME_MS;
			}
			if(ret)
			{				
				if((pUnit->state == BaseRunType_RUN)&&(pUnit->alarmMode !=processMode))
				{
					data[0] = BaseCtrlType_STOP;
					pUnit->alarmFunc(pUnit->point,pUnit->phoneAddr,data,0,0);
				}
				
				data[0] = BaseCtrlType_STARTUP;
				data[1] = mode;			
				ret =pUnit->alarmFunc(pUnit->point,pUnit->phoneAddr,data,processMode,0);
				if(ret)
				{
					pUnit->state = BaseRunType_RUN;
					pUnit->alarmTime = processTime;
					pUnit->ticks =0;
					pUnit->alarmMode =processMode;
				}
			}
		}
		break;
		
		case BaseCtrlType_STOP:
		{
			if(pUnit->state != BaseRunType_RUN)
				return 0;
			
			alarmCtrl.eventType =ctrlEventType;
			alarmCtrl.eventID =ctrlEventID;
			alarmCtrl.eventMode =alarmMode;
			alarmCtrl.alarmTime =alarmTime*LOGIC_TIME_MS;
			ret = DelAlarmCtrlBufQueue(&alarmCtrl,&saveCtrl);
			if(AlarmCtrlMng.Cnt==0)
			{
				data[0] = BaseCtrlType_STOP;
				ret =pUnit->alarmFunc(pUnit->point,pUnit->phoneAddr,data,alarmMode,1);
				pUnit->state = BaseRunType_STOP;
				pUnit->alarmTime = 0;
				pUnit->ticks =0;
				pUnit->alarmMode = 0;
			}	
			else if(ret>0)
			{
				if(pUnit->alarmMode !=saveCtrl.eventMode)
				{
					data[0] = BaseCtrlType_STOP;
					pUnit->alarmFunc(pUnit->point,pUnit->phoneAddr,data,0,0);
					
					data[0] = BaseCtrlType_STARTUP;
					data[1] = mode;	
					pUnit->AlarmEventType = AlarmCtrlMng.buf[ret-1].eventType;
					pUnit->AlarmEventID = AlarmCtrlMng.buf[ret-1].eventID;					
					ret =pUnit->alarmFunc(pUnit->point,pUnit->phoneAddr,data,saveCtrl.eventMode,0);
					if(ret)
					{
						pUnit->state = BaseRunType_RUN;
						pUnit->alarmTime = saveCtrl.alarmTime;
						pUnit->ticks =0;
						pUnit->alarmMode =saveCtrl.eventMode;
					}					
				}	
				else
				{
					pUnit->alarmTime = saveCtrl.alarmTime;
					pUnit->ticks =0;					
				}
			}
		}
		break;
		
		default:
			break;
	}
	return ret;
}

u8 CtrlAlarm(AlarmType	type,u16 alarmTime,BaseCtrlType cmd,void *pdata,u8 ctrlEventType,u8 ctrlEventID,u8 alarmMode)
{
	u8 ret=0;
	
	if((pdata ==NULL)&&(cmd==BaseCtrlType_STARTUP))
		return 0;
	
	if(type ==AlarmType_Phone)
	{
		if(pdata == NULL)
			return 0;
		
//		if(AlarmModule.alarmDev[ALARM_UNIT_PHONE].enable == EnableType_DISABLE)
//			return 0;
		
		AlarmModule.alarmDev[ALARM_UNIT_PHONE].alarmFunc(AlarmModule.alarmDev[ALARM_UNIT_PHONE].point,AlarmModule.alarmDev[ALARM_UNIT_PHONE].phoneAddr,pdata,alarmMode,0);
		return 1;
	}
	else 
	{
//		if(AlarmModule.alarmDev[ALARM_UNIT_SOUND].enable == EnableType_DISABLE)
//			return 0;
		
		if((type ==AlarmType_SoundLight )||(type ==AlarmType_Sound ))
		{
			ret = alarmCmdFunc(&AlarmModule.alarmDev[ALARM_UNIT_SOUND],alarmTime*LOGIC_TIME_MS,cmd,*((u8*)pdata),ctrlEventType,ctrlEventID,alarmMode);
		}
		if((type ==AlarmType_SoundLight )||(type ==AlarmType_Light ))
		{
			ret |= alarmCmdFunc(&AlarmModule.alarmDev[ALARM_UNIT_LIGHT],alarmTime*LOGIC_TIME_MS,cmd,*((u8*)pdata+1),ctrlEventType,ctrlEventID,alarmMode);
		}
		return ret;
	}
}

void AlarmAllStop(void)
{
	AlarmAllStopFlag =1;
}

void AlarmMng(u16 delay)
{
	u8 i,j;
	u8 data[2],ret;
	AlarmUnitType *pUnit;
	
	for(i=0;i<ALARM_UNIT_PHONE;i++)
	{
//		if(AlarmModule.alarmDev[i].enable !=EnableType_ENABLE)
//			continue;
		
		if(AlarmModule.alarmDev[i].state == BaseRunType_RUN)
		{
			if(i !=ALARM_UNIT_LIGHT)
			{
				if(AlarmModule.alarmDev[i].alarmTime !=0)
				{
					AlarmModule.alarmDev[i].ticks +=delay;
					if(AlarmModule.alarmDev[i].ticks >= AlarmModule.alarmDev[i].alarmTime)
					{
						data[0] = BaseCtrlType_STOP;
						ret =AlarmModule.alarmDev[i].alarmFunc(AlarmModule.alarmDev[i].point,AlarmModule.alarmDev[i].phoneAddr,&data[0],0,1);
						if(ret)
						{
							AlarmModule.alarmDev[i].state = BaseRunType_STOP;
							AlarmModule.alarmDev[i].alarmTime = 0;
							AlarmModule.alarmDev[i].ticks =0;
						}					
					}
				}
			}
			else
			{
				if(AlarmModule.alarmDev[i].alarmTime !=0)
				{
					AlarmModule.alarmDev[i].ticks +=delay;
					if(AlarmModule.alarmDev[i].ticks >= AlarmModule.alarmDev[i].alarmTime)
					{						
						pUnit =&AlarmModule.alarmDev[i];
						alarmCmdFunc(pUnit,pUnit->alarmTime,BaseCtrlType_STOP,0,pUnit->AlarmEventType,pUnit->AlarmEventID,pUnit->alarmMode);					
					}
				}

				for(j=0;j<ALARMCTRL_BUF_LEN;j++)
				{
					if(AlarmCtrlMng.buf[j].use )
					{
						if(AlarmModule.alarmDev[i].alarmTime !=0)
						{
							if(AlarmCtrlMng.buf[j].alarmTime >delay)
								AlarmCtrlMng.buf[j].alarmTime -= delay;
							else
							{
								AlarmCtrlMng.buf[j].eventType = 0;
								AlarmCtrlMng.buf[j].eventID   = 0;
								AlarmCtrlMng.buf[j].use =0;
								AlarmCtrlMng.buf[j].eventMode = 0;
								AlarmCtrlMng.buf[j].alarmTime = 0;
								AlarmCtrlMng.Cnt --;
							}
						}
					}
				}
			}
			
			if(AlarmAllStopFlag)
			{				
				data[0] = BaseCtrlType_STOP;
				ret =AlarmModule.alarmDev[i].alarmFunc(AlarmModule.alarmDev[i].point,AlarmModule.alarmDev[i].phoneAddr,&data[0],0,1);
				if(ret)
				{
					AlarmModule.alarmDev[i].state = BaseRunType_STOP;
					AlarmModule.alarmDev[i].alarmTime = 0;
					AlarmModule.alarmDev[i].ticks =0;
				}	
				
				if(i ==ALARM_UNIT_LIGHT)
					DelAllAlarmCtrlBufQueue();
			}
		}
	}
	AlarmAllStopFlag =0;
}

#endif
