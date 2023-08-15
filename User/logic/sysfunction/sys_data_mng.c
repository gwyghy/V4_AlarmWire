/*
*********************************************************************************************************
*	                                            DoubleWires
*                           Commuication Controller For The Mining Industry
*
*                                  (c) Copyright 1994-2013  HNDZ 
*                       All rights reserved.  Protected by international copyright laws.
*
*
*    File    : sys_data_mng.c
*    Module  : system function
*    Version : V1.0
*    History :
*   -----------------
*              系统参数读/写控制
*              Version  Date           By            Note
*              v1.0    
*
*********************************************************************************************************
*/

#define __SYS_DATA_MNG_C__

#include "sys_default.h"
#include "sys_msg_def.h"
#include "sysParamAnalysis.h"
#include "netconf.h"
#include "udp_app_proc.h"
#include "FileLoad.h"
#include "Util_Timer.h"
#include "V2_Can_Pro.h"
#include "ProjectConfig.h"

#if (LOGIC_SYSFUNC_EN == 1)

//系统参数数组
SysParamType   SysParam    __attribute__((at(MEM_SDRAM_SYSFUNC_POS)));
//系统状态数组
SysStateType	 SysState    __attribute__((at(MEM_SDRAM_SYSFUNC_POS+MEM_SDRAM_ALIGN_SIZE(sizeof(SysParamType)))));
//网络参数数组
NetParamType	 SysNetParam;
//系统控制数组
static SysCtrlInfoType  ctrlInfoTemp;
//停止、闭锁、急停状态数组
SysStopType		 SysStopState[256]={0};

static u32 		 reset_timerid=0;
//手动控制单动作数组
u8				 ManualActionClear[MANUALACTIONINFO_LEN];
u8         ManualActionSupportNum;

static u8        versionState[2]={0};

u16 kzqTurnonReson = 0;

const STR_SysInfo SYS_RESTORE = 
{
    20130809u,         ///< release date
    23060808u,         ///< uimage id
    0xE1020000,        ///< product type

};


/*******************************************************************************************
*功能描述：初始化uwb人员定位参数
*入口参数：无
*返回值：  无
*******************************************************************************************/
static void updatePersonParam(void)
{
	stPersonParam PersonParam;
	
	PersonParam.scNum = SysParam.globalParam.SupportParam.currentSupportID;
	PersonParam.range = SysParam.globalParam.DevicePeopleParam.CheckRange;
	PersonParam.u8WORKPARAM.WORKPARAM.trsPower = SysParam.globalParam.DevicePeopleParam.SendPower;
	PersonParam.u8WORKPARAM.WORKPARAM.workMode =1;
	PersonParam.u8WORKPARAM.WORKPARAM.calcMode =1;
	PersonParam.u8WORKPARAM.WORKPARAM.disVariation =0;
	PersonParam.interval = SysParam.globalParam.DevicePeopleParam.ReportInterval;
	PersonParam.scHigh = 0;
	CanProSetParam(ID_PERSON,&PersonParam,0);	
}

/*******************************************************************************************
*功能描述：获取系统参数、状态函数
*入口参数：sel取值：
*            0：状态；
*            1：参数
*          Type取值：
*           ENUM_SINGLE_ACTION：     单动作
*           ENUM_COMBINATION_ACTION：组合动作
*           ENUM_GROUP_ACTION：      成组动作
*           ENUM_PART：              部件              
*           ENUM_SENSOR：            传感器
*           ENUM_SYSTEM：            系统
*           ENUM_DEVICE：            设备
*          Id取值：
*            id号
*          pdata：
*            参数、状态地址
*          unitLen：
*            返回参数、状态长度
*返回值：Type类型状态
					0：失败；
					1：成功
*******************************************************************************************/
static u8 getSysData(u8 sel,u8 Type,u8 Id,ParamUnit **pdata,u16 *unitLen)
{
	if(Type >= ENUMTYPE_MAX)
		return 0;
	
	switch(Type)
	{
		case ENUM_SINGLE_ACTION:
		{
			if(Id >SINGLEACTION_NUM_MAX)
				return 0;
			if(sel)
			{
				*pdata = (ParamUnit *)(&SysParam.SigAction[Id-1]);
				*unitLen = sizeof(SigActionParamType)/2;
			}
			else
			{
				*pdata = (ParamUnit *)(&SysState.SigAction[Id-1]);
				*unitLen = sizeof(SigActionStateType)/2;
			}
			
		}
		break;
		
		case ENUM_COMBINATION_ACTION:
		{
			if(Id >COMBACTION_NUM_MAX)
				return 0;
			if(sel)
			{
				*pdata = (ParamUnit *)(&SysParam.CombAction[Id-1]);
				*unitLen = sizeof(CombActionParamType)/2;
			}
			else
			{
				*pdata = (ParamUnit *)(&SysState.CombAction[Id-1]);
				*unitLen = sizeof(CombActionStateType)/2;
			}
		}
		break;
		
		case ENUM_GROUP_ACTION:
		{
			if(Id >GROUPACTION_NUM_MAX)
				return 0;
			if(sel)
			{
				*pdata = (ParamUnit *)(&SysParam.GroupAction[Id-1]);
				*unitLen = sizeof(groupActionParamType)/2;
			}
			else
			{
				*pdata = (ParamUnit *)(&SysState.GroupAction[Id-1]);
				*unitLen = sizeof(groupActionStateType)/2;
			}
		}
		break;
		
		case ENUM_PART:
		{
			if(Id >PART_NUM_MAX)
				return 0;
			if(sel)
			{
				*pdata = (ParamUnit *)(&SysParam.Part[Id-1]);
				*unitLen = sizeof(PartParamType)/2;
			}
			else
			{
				*pdata = (ParamUnit *)(&SysState.Part[Id-1]);
				*unitLen = sizeof(PartStateType)/2;
			}
		}
		break;
		
		case ENUM_SENSOR:
		{
			if(Id >SENSOR_MAX)
				return 0;
			if(sel)
			{
				*pdata = (ParamUnit *)(&SysParam.Sensor[Id-1]);
				*unitLen = sizeof(SensorParamType)/2;
			}
			else
			{
				*pdata = (ParamUnit *)(&SysState.Sensor[Id-1]);
				*unitLen = sizeof(SensorStateType)/2;
			}
		}
		break;
		
		case ENUM_SYSTEM:
		{
			if(sel)
			{
				switch(Id)
				{
					case 	globalParamEnum_Support:
						*pdata = (ParamUnit *)(&SysParam.globalParam.SupportParam);
						*unitLen = sizeof(globalParam_SupportType)/2;
						break;
					
					case	globalParamEnum_Server:
						*pdata = (ParamUnit *)(&SysParam.globalParam.ServerParam);
						*unitLen = sizeof(globalParam_ServerType)/2;
						break;
					
					case	globalParamEnum_Key:
						*pdata = (ParamUnit *)(&SysParam.globalParam.KeyConfig);
						*unitLen = sizeof(globalParam_KeyType)/2;
						break;
					
					case	globalParamEnum_AssistKey:
						*pdata = (ParamUnit *)(&SysParam.globalParam.AssistKeyConfig);
						*unitLen = sizeof(globalParam_AssistKeyType)/2;
						break;
					
					case	globalParamEnum_UIConfig:
						*pdata = (ParamUnit *)(&SysParam.globalParam.UIConfig);
						*unitLen = sizeof(globalParam_UIConfigType)/2;
						break;
					
					case	globalParamEnum_GroupAssistKey:
						*pdata = (ParamUnit *)(&SysParam.globalParam.GroupAssistKeyConfig);
						*unitLen = sizeof(globalParam_GroupAssistKeyType)/2;
						break;
					
					case	globalParamEnum_DeviceAlarm:
						*pdata = (ParamUnit *)(&SysParam.globalParam.DeviceAlarmParam);
						*unitLen = sizeof(globalParam_DeviceAlarmType)/2;
						break;
					
					case	globalParamEnum_DevicePeople:
						*pdata = (ParamUnit *)(&SysParam.globalParam.DevicePeopleParam);
						*unitLen = sizeof(globalParam_DevicePeopleType)/2;
						break;
					
					case	globalParamEnum_DeviceInfirred:
						*pdata = (ParamUnit *)(&SysParam.globalParam.DeviceInfirredParam);
						*unitLen = sizeof(globalParam_DeviceInfirredType)/2;
						break;
					
					case	globalParamEnum_DeviceWireless:
						*pdata = (ParamUnit *)(&SysParam.globalParam.DeviceWirelessParam);
						*unitLen = sizeof(globalParam_DeviceWirelessType)/2;
						break;
					case    globalParamEnum_DeviceTrans:
						*pdata = (ParamUnit *)(&SysParam.globalParam.DeviceTransParam);
						*unitLen = sizeof(globalParam_DeviceTransType)/2;
						break;
					case globalParamEnum_DeviceHub:  //ghy 增加HUB
						*pdata = (ParamUnit *)(&SysParam.globalParam.DeviceHubParam);
						*unitLen = sizeof(globalParam_DeviceHubType)/2;
						break;					
					default:
						break;
				}
				
			}
			else
			{
				*pdata = (ParamUnit *)(&SysState.globalState);
				*unitLen = sizeof(globalStateType)/2;
			}
		}
		break;
		
		case ENUM_DEVICE:
		{
			if(sel==0)
//			{
//				*pdata = (ParamUnit *)(&SysParam.globalParam);
//				*unitLen = sizeof(globalParamType)/2;
//			}
//			else
			{
				*pdata = (ParamUnit *)(&SysState.Device[Id-1]);
				*unitLen = sizeof(deviceStateType)/2;
			}
		}
		break;
		
		default:
			break;
	}
	return 1;
}

static void ParamCheck(void)
{
	;	//todo
}

/*******************************************************************************************
*功能描述：设置传感器、设备的在线状态
*入口参数：Type取值：             
*           ENUM_SENSOR：            传感器
*           ENUM_SYSTEM：            系统
*          Id取值：
*            id号
*          offset：
*            参数偏移
*          Len：
*            参数长度
*返回值：Type类型状态
					0：失败；
					1：成功
*******************************************************************************************/
u8 ParamProcess(u8 Type,u8 Id,u16 offset,u16 Len)
{
	stDeviceStatusMsg deviceMsg = {0};
	PointIDType point;
	u8 gap;
	u8 driver=0;
	
	if(Type >= ENUMTYPE_MAX)
		return 0;
	
	switch(Type)
	{				
		case ENUM_SENSOR:
		{
			if(Id >SENSOR_MAX)
				return 0;

			gap = ((u8 *)(&SysParam.Sensor[Id-1].enable) -(u8 *)(&SysParam.Sensor[Id-1].sensorName))/2;
			if((offset >gap)||((offset+Len)<gap))
				return 0;

			point =SysParam.Sensor[Id-1].pointID;
			if((point.driver == DRIVER_CAN1)||(point.driver == DRIVER_CAN2))
			{
				GetCanDeviceStatusMsg(point.driver-1, point.device, point.subDevice, &deviceMsg);
				deviceMsg.isEnable = SysParam.Sensor[Id-1].enable;
				SetCanDeviceStatusMsg(point.driver-1, point.device, point.subDevice, &deviceMsg);	
			}
		}
		break;
		
		case ENUM_SYSTEM:
		{
			switch(Id)
			{
				case 	globalParamEnum_Support:
				{
					gap = 0;
					if((offset >gap)||((offset+Len)<gap))
						return 0;
					
					SysState.globalState.slaveMode =SYSMODE_CONFIG;
					SysNetParam.NetParam_IPAddress[3] = SysParam.globalParam.SupportParam.currentSupportID;
					ee_WriteBytes((uint8_t *)(&SysNetParam), NET_PARAM_START, sizeof(NetParamType));
					setUpdateFlag(1);
					setSelfAddr(SysNetParam.NetParam_IPAddress[3],1);
					updatePersonParam();
				}
					break;
				
				case	globalParamEnum_Server:
					break;
				
				case	globalParamEnum_Key:
					GetShowAgain(2);
					break;
				
				case	globalParamEnum_AssistKey:
					GetShowAgain(1);
					break;
				
				case	globalParamEnum_UIConfig:
					GetShowAgain(3);
					break;
				
				case	globalParamEnum_GroupAssistKey:
					GetShowAgain(4);
					break;
				
				case	globalParamEnum_DeviceAlarm:
				{
					AlarmModuleEnable(SysParam.globalParam.DeviceAlarmParam.DevAlarmEnable);
					//GetDeviceCanMsg(ID_ALARM, 0, &driver);
					GetCanDeviceStatusMsg(DRIVER_CAN1-1, ID_ALARM, 0, &deviceMsg);
					deviceMsg.isEnable = SysParam.globalParam.DeviceAlarmParam.DevAlarmEnable;
					SetCanDeviceStatusMsg(DRIVER_CAN1-1, ID_ALARM, 0, &deviceMsg);	
					
					GetCanDeviceStatusMsg(DRIVER_CAN2-1, ID_ALARM, 0, &deviceMsg);
					deviceMsg.isEnable = SysParam.globalParam.DeviceAlarmParam.DevAlarmEnable;
					SetCanDeviceStatusMsg(DRIVER_CAN2-1, ID_ALARM, 0, &deviceMsg);					
				}
					break;
				
				case	globalParamEnum_DevicePeople:  //todo 人员参数
				{
					//GetDeviceCanMsg(ID_PERSON, 0, &driver);
					GetCanDeviceStatusMsg(DRIVER_CAN1-1, ID_PERSON, 0, &deviceMsg);
					deviceMsg.isEnable = SysParam.globalParam.DevicePeopleParam.PeoplePosEnable;
					SetCanDeviceStatusMsg(DRIVER_CAN1-1, ID_PERSON, 0, &deviceMsg);	
					
					GetCanDeviceStatusMsg(DRIVER_CAN2-1, ID_PERSON, 0, &deviceMsg);
					deviceMsg.isEnable = SysParam.globalParam.DevicePeopleParam.PeoplePosEnable;
					SetCanDeviceStatusMsg(DRIVER_CAN2-1, ID_PERSON, 0, &deviceMsg);	
					updatePersonParam();
				}
					break;
				
				case	globalParamEnum_DeviceInfirred:  //todo 红外对码
				{
					//GetDeviceCanMsg(ID_IRRX, 0, &driver);
					GetCanDeviceStatusMsg(DRIVER_CAN1-1, ID_IRRX, 0, &deviceMsg);
					deviceMsg.isEnable = SysParam.globalParam.DeviceInfirredParam.InfirredRevEnable;
					SetCanDeviceStatusMsg(DRIVER_CAN1-1, ID_IRRX, 0, &deviceMsg);	
					
					GetCanDeviceStatusMsg(DRIVER_CAN2-1, ID_IRRX, 0, &deviceMsg);
					deviceMsg.isEnable = SysParam.globalParam.DeviceInfirredParam.InfirredRevEnable;
					SetCanDeviceStatusMsg(DRIVER_CAN2-1, ID_IRRX, 0, &deviceMsg);						
				}
					break;
				
				case	globalParamEnum_DeviceWireless:
				{
					//GetDeviceCanMsg(ID_WIRELESS, 0, &driver);
//					GetCanDeviceStatusMsg(DRIVER_CAN1-1, ID_WIRELESS, 0, &deviceMsg);
//					deviceMsg.isEnable = SysParam.globalParam.DeviceWirelessParam.WirelessEnable;
//					SetCanDeviceStatusMsg(DRIVER_CAN1-1, ID_WIRELESS, 0, &deviceMsg);	
//					
//					GetCanDeviceStatusMsg(DRIVER_CAN2-1, ID_WIRELESS, 0, &deviceMsg);
//					deviceMsg.isEnable = SysParam.globalParam.DeviceWirelessParam.WirelessEnable;
//					SetCanDeviceStatusMsg(DRIVER_CAN2-1, ID_WIRELESS, 0, &deviceMsg);						
				}
				break;
				case  globalParamEnum_DeviceTrans:
				break;
				case globalParamEnum_DeviceHub:
				{
					GetCanDeviceStatusMsg(DRIVER_CAN1-1, ID_HUB1, 0, &deviceMsg);
					deviceMsg.isEnable = SysParam.globalParam.DeviceHubParam.HubEnable;
					SetCanDeviceStatusMsg(DRIVER_CAN1-1, ID_HUB1, 0, &deviceMsg);	
					
					GetCanDeviceStatusMsg(DRIVER_CAN1-1, ID_HUB2, 0, &deviceMsg);
					deviceMsg.isEnable = SysParam.globalParam.DeviceHubParam.Hub2Enable;
					SetCanDeviceStatusMsg(DRIVER_CAN1-1, ID_HUB2, 0, &deviceMsg);	
					
					GetCanDeviceStatusMsg(DRIVER_CAN1-1, ID_HUB3, 0, &deviceMsg);
					deviceMsg.isEnable = SysParam.globalParam.DeviceHubParam.Hub3Enable;
					SetCanDeviceStatusMsg(DRIVER_CAN1-1, ID_HUB3, 0, &deviceMsg);	
					
					GetCanDeviceStatusMsg(DRIVER_CAN2-1, ID_HUB1, 0, &deviceMsg);
					deviceMsg.isEnable = SysParam.globalParam.DeviceHubParam.HubEnable;
					SetCanDeviceStatusMsg(DRIVER_CAN2-1, ID_HUB1, 0, &deviceMsg);	
					
					GetCanDeviceStatusMsg(DRIVER_CAN2-1, ID_HUB2, 0, &deviceMsg);
					deviceMsg.isEnable = SysParam.globalParam.DeviceHubParam.Hub2Enable;
					SetCanDeviceStatusMsg(DRIVER_CAN2-1, ID_HUB2, 0, &deviceMsg);	
					
					GetCanDeviceStatusMsg(DRIVER_CAN2-1, ID_HUB3, 0, &deviceMsg);
					deviceMsg.isEnable = SysParam.globalParam.DeviceHubParam.Hub3Enable;
					SetCanDeviceStatusMsg(DRIVER_CAN2-1, ID_HUB3, 0, &deviceMsg);	
				}					
				break;
				default:
					break;
			}
				
		}
		break;
		
		default:
			break;
	}
	return 1;
}



static u8 ParamProcess_address(u8 *paddress,u8 global)
{
	stDeviceStatusMsg deviceMsg = {0};
	PointIDType point;
	u8 driver=0;
	u8 i,temp[2];	
	
	for(i=0;i<SENSOR_MAX;i++)
	{
		if((u8 *)paddress == (u8 *)(&SysParam.Sensor[i].enable))
		{
			point =SysParam.Sensor[i].pointID;
			if((point.driver == DRIVER_CAN1)||(point.driver == DRIVER_CAN2))
			{
				GetCanDeviceStatusMsg(point.driver-1, point.device, point.subDevice, &deviceMsg);
				deviceMsg.isEnable = SysParam.Sensor[i].enable;
				SetCanDeviceStatusMsg(point.driver-1, point.device, point.subDevice, &deviceMsg);	
			}	
			return 1;
		}
	}
	
	if((u8 *)paddress == (u8 *)(&SysParam.globalParam.SupportParam.currentSupportID))
	{
		
		if((*paddress>SysParam.globalParam.SupportParam.MaxSupportID) || (*paddress<SysParam.globalParam.SupportParam.MinSupportID))
			return 1;
		if(global == 1)
		{
			memset(SysStopState,0,sizeof(SysStopType)*256);
			memset(&SysState.globalState,0,14);
		}

		
		SysState.globalState.slaveMode =SYSMODE_CONFIG;
		SysNetParam.NetParam_IPAddress[3] = SysParam.globalParam.SupportParam.currentSupportID;
		ee_WriteBytes((uint8_t *)(&SysNetParam), NET_PARAM_START, sizeof(NetParamType));
		setUpdateFlag(1);
		if(global)
			setSelfAddr(SysNetParam.NetParam_IPAddress[3],1);
		else
			setSelfAddr(SysNetParam.NetParam_IPAddress[3],0);
		updatePersonParam();
	}
	else if((u8 *)paddress == (u8 *)(&SysParam.globalParam.DeviceAlarmParam.DevAlarmEnable))
	{
		AlarmModuleEnable(SysParam.globalParam.DeviceAlarmParam.DevAlarmEnable);
		GetCanDeviceStatusMsg(DRIVER_CAN1-1, ID_ALARM, 0, &deviceMsg);
		deviceMsg.isEnable = SysParam.globalParam.DeviceAlarmParam.DevAlarmEnable;
		SetCanDeviceStatusMsg(DRIVER_CAN1-1, ID_ALARM, 0, &deviceMsg);	
		
		GetCanDeviceStatusMsg(DRIVER_CAN2-1, ID_ALARM, 0, &deviceMsg);
		deviceMsg.isEnable = SysParam.globalParam.DeviceAlarmParam.DevAlarmEnable;
		SetCanDeviceStatusMsg(DRIVER_CAN2-1, ID_ALARM, 0, &deviceMsg);
	}
	else if((u8 *)paddress == (u8 *)(&SysParam.globalParam.DevicePeopleParam.PeoplePosEnable)) //todo 人员参数
	{
		GetCanDeviceStatusMsg(DRIVER_CAN1-1, ID_PERSON, 0, &deviceMsg);
		deviceMsg.isEnable = SysParam.globalParam.DevicePeopleParam.PeoplePosEnable;
		SetCanDeviceStatusMsg(DRIVER_CAN1-1, ID_PERSON, 0, &deviceMsg);	
		
		GetCanDeviceStatusMsg(DRIVER_CAN2-1, ID_PERSON, 0, &deviceMsg);
		deviceMsg.isEnable = SysParam.globalParam.DevicePeopleParam.PeoplePosEnable;
		SetCanDeviceStatusMsg(DRIVER_CAN2-1, ID_PERSON, 0, &deviceMsg);
	}
	else if(((u8 *)paddress > (u8 *)(&SysParam.globalParam.DevicePeopleParam.PeoplePosEnable)) 
		   &&((u8 *)paddress < (u8 *)(&SysParam.globalParam.DeviceInfirredParam.InfirredRevEnable)))
	{
		updatePersonParam();
	}	
	else if((u8 *)paddress == (u8 *)(&SysParam.globalParam.DeviceInfirredParam.InfirredRevEnable)) //todo 红外对码
	{
		GetCanDeviceStatusMsg(DRIVER_CAN1-1, ID_IRRX, 0, &deviceMsg);
		deviceMsg.isEnable = SysParam.globalParam.DeviceInfirredParam.InfirredRevEnable;
		SetCanDeviceStatusMsg(DRIVER_CAN1-1, ID_IRRX, 0, &deviceMsg);	
		
		GetCanDeviceStatusMsg(DRIVER_CAN2-1, ID_IRRX, 0, &deviceMsg);
		deviceMsg.isEnable = SysParam.globalParam.DeviceInfirredParam.InfirredRevEnable;
		SetCanDeviceStatusMsg(DRIVER_CAN2-1, ID_IRRX, 0, &deviceMsg);
	}
	else if((u8 *)paddress == (u8 *)(&SysParam.globalParam.DeviceWirelessParam.WirelessEnable)) 
	{
		GetCanDeviceStatusMsg(DRIVER_CAN1-1, ID_WIRELESS, 0, &deviceMsg);
		deviceMsg.isEnable = SysParam.globalParam.DeviceWirelessParam.WirelessEnable;
		SetCanDeviceStatusMsg(DRIVER_CAN1-1, ID_WIRELESS, 0, &deviceMsg);	
		
		GetCanDeviceStatusMsg(DRIVER_CAN2-1, ID_WIRELESS, 0, &deviceMsg);
		deviceMsg.isEnable = SysParam.globalParam.DeviceWirelessParam.WirelessEnable;
		SetCanDeviceStatusMsg(DRIVER_CAN2-1, ID_WIRELESS, 0, &deviceMsg);	
	}
	else if((u8 *)paddress == (u8 *)(&SysParam.globalParam.DeviceHubParam.HubEnable)) // todo HUB使能 ghy
	{
		GetCanDeviceStatusMsg(DRIVER_CAN1-1, ID_HUB1, 0, &deviceMsg);
		deviceMsg.isEnable = SysParam.globalParam.DeviceHubParam.HubEnable;
		SetCanDeviceStatusMsg(DRIVER_CAN1-1, ID_HUB1, 0, &deviceMsg);	
		
		GetCanDeviceStatusMsg(DRIVER_CAN2-1, ID_HUB1, 0, &deviceMsg);
		deviceMsg.isEnable = SysParam.globalParam.DeviceHubParam.HubEnable;
		SetCanDeviceStatusMsg(DRIVER_CAN2-1, ID_HUB1, 0, &deviceMsg);	
	}
	else if((u8 *)paddress == (u8 *)(&SysParam.globalParam.DeviceHubParam.Hub2Enable)) // todo HUB2使能 ghy
	{
		GetCanDeviceStatusMsg(DRIVER_CAN1-1, ID_HUB2, 0, &deviceMsg);
		deviceMsg.isEnable = SysParam.globalParam.DeviceHubParam.Hub2Enable;
		SetCanDeviceStatusMsg(DRIVER_CAN1-1, ID_HUB2, 0, &deviceMsg);

		GetCanDeviceStatusMsg(DRIVER_CAN2-1, ID_HUB2, 0, &deviceMsg);
		deviceMsg.isEnable = SysParam.globalParam.DeviceHubParam.Hub2Enable;
		SetCanDeviceStatusMsg(DRIVER_CAN2-1, ID_HUB2, 0, &deviceMsg);	
	}
	else if((u8 *)paddress == (u8 *)(&SysParam.globalParam.DeviceHubParam.Hub3Enable)) // todo HUB3使能 ghy
	{
		
		GetCanDeviceStatusMsg(DRIVER_CAN1-1, ID_HUB3, 0, &deviceMsg);
		deviceMsg.isEnable = SysParam.globalParam.DeviceHubParam.Hub3Enable;
		SetCanDeviceStatusMsg(DRIVER_CAN1-1, ID_HUB3, 0, &deviceMsg);
		
		GetCanDeviceStatusMsg(DRIVER_CAN2-1, ID_HUB3, 0, &deviceMsg);
		deviceMsg.isEnable = SysParam.globalParam.DeviceHubParam.Hub3Enable;
		SetCanDeviceStatusMsg(DRIVER_CAN2-1, ID_HUB3, 0, &deviceMsg);
	}
	else if(((u8 *)paddress >= (u8 *)(&SysParam.globalParam.KeyConfig.KeyConfig[0]))
		   &&((u8 *)paddress < (u8 *)(&SysParam.globalParam.AssistKeyConfig.AssistKeyConfig[0])))
	{
		GetShowAgain(2);
	}
	else if(((u8 *)paddress >= (u8 *)(&SysParam.globalParam.AssistKeyConfig.AssistKeyConfig[0]))
		   &&((u8 *)paddress < (u8 *)(&SysParam.globalParam.UIConfig.UISensorConfig[0])))
	{
		GetShowAgain(1);
	}
	else if(((u8 *)paddress >= (u8 *)(&SysParam.globalParam.UIConfig.UISensorConfig[0]))
		   &&((u8 *)paddress <= (u8 *)(&SysParam.globalParam.UIConfig.UISensorConfig[UISENSOR_CONFIG_MAX-1])))
	{
		GetShowAgain(3);
	}
	else if(((u8 *)paddress >= (u8 *)(&SysParam.globalParam.GroupAssistKeyConfig.GroupAssistKeyConfig[0]))
		   &&((u8 *)paddress <= (u8 *)(&SysParam.globalParam.GroupAssistKeyConfig.GroupAssistKeyConfig[ASSISTKEY_CONFIG_MAX*ASSIST_PAGE_MAX-1])))
	{
		GetShowAgain(4);
	}	
	if(((u8 *)paddress >= (u8 *)(&SysParam.globalParam.SupportParam.SupportDirect))
	  &&((u8 *)paddress <= (u8 *)(&SysParam.globalParam.SupportParam.MaxSupportID)))
	{
		temp[0] = 1;
		temp[1] = DEVICE_SELFID_RIGHT;
		SysDataFun(DATA_MSG_SetCommCheckState,0,0,2,0,temp,0);	
		temp[1] = DEVICE_SELFID_LEFT;
		SysDataFun(DATA_MSG_SetCommCheckState,0,0,2,0,temp,0);		
	}	
	return 1;
}

/*******************************************************************************************
*功能描述：复位回调函数
*入口参数：data取值：             
*           无实际意义
*返回值：无
*******************************************************************************************/
void TimerCallback_reset(u16 data)
{
	deleteTimerEvent(reset_timerid);
	ETH_Stop();
	BCM_551CS_LOW();
	NVIC_SystemReset(); 
}

/*******************************************************************************************
*功能描述：恢复默认参数回调函数
*入口参数：data取值：             
*           无实际意义
*返回值：无
*******************************************************************************************/
void TimerCallback_reset_param(u16 data)
{
	u8 temp[4];
	
	
	//先解除eeprom写保护
	GPIO_ResetBits(GPIO_PORT_I2C, I2C_WP_PIN);
	bsp_DelayMS(10);
	
	deleteTimerEvent(reset_timerid);
	ee_ReadBytes(temp, FIRST_TURNON_START, FIRST_TURNON_LEN);
	temp[0]=0x66;
	temp[1]=0x66;
	temp[2]=0x66;
	temp[3]=0x66;
	ee_WriteBytes(temp, FIRST_TURNON_START, FIRST_TURNON_LEN);
	ETH_Stop();
	BCM_551CS_LOW();
	NVIC_SystemReset();
}

/*********************************************************
* 功能描述：SysDataFun
* 入口参数：None
** 输　出：None
** 功能描述：控制器参数读写管理
************************************************************/
//返回异常码，负数
int SysDataFun(DataMsg_t msg, u8 Type,u8 Id,u16 Length,u16 Offset,void *pdata,u16 *retLen)
{	
	ParamUnit	*pUnit=NULL;
	u16 unitLen=0;
	u8 ret=0;
	int i,flag;
	ActionInfoType *pActionInfo;
	u32 flashStart;
	u8 temp[12],hostaddr,max,min;
	u16 funcid;
	RTC_TimeData time;
	static u8 leftParaNotsame = 0;
	static u8 rightParaNotsame = 0;
	
	if (msg> DATA_MSG_MAX)		return -1;
	
	switch(msg)
    {
		case DATA_MSG_READ_OBJECT:
		{
			ret =getSysData(0,Type,Id,&pUnit,&unitLen);
			if(((Offset+Length)> unitLen)||(pUnit ==NULL))
				ret=0;
			else
			{
				*((u8 **)pdata) = (u8 *)pUnit;
				*retLen = unitLen*2;
			}
		}
			break;
		
		case DATA_MSG_WRITE_OBJECT:
		{
			ret =getSysData(0,Type,Id,&pUnit,&unitLen);
			if(((Offset+Length)> unitLen)||(pUnit ==NULL))
				ret=0;
			else
			{
				memcpy((u8 *)pUnit+Offset*2,(u8 *)pdata,Length*2);
			}
		}
			break;
		
		case DATA_MSG_READ_OBJECT_PARAM:
		{
			ret =getSysData(1,Type,Id,&pUnit,&unitLen);
			if(((Offset+Length)> unitLen)||(pUnit ==NULL))
				ret=0;
			else
			{
				*((u8 **)pdata) = (u8 *)pUnit;
				*retLen = unitLen*2;
			}
		}
			break;
		
		case DATA_MSG_WRITE_OBJECT_PARAM:
		{
			ret =getSysData(0,Type,Id,&pUnit,&unitLen);
			if(((Offset+Length)> unitLen)||(pUnit ==NULL))
				ret=0;
			else
			{
				memcpy((u8 *)pUnit+Offset*2,(u8 *)pdata,Length*2);
				ParamCheck();
				ee_WriteBytes((u8 *)pUnit+Offset*2, SYS_PARAM_START+((u8 *)pUnit+Offset*2-(u8*)&SysParam), Length*2);	
				ParamProcess(Type,Id,Offset,Length);
			}
		}
			break;	

		case DATA_MSG_CTRL:
		{
			if(Length <2)
				break;
			
			memset(&ctrlInfoTemp,0,sizeof(SysCtrlInfoType));
			ctrlInfoTemp.actionType = Type;
			ctrlInfoTemp.actionID   = Id;
			ctrlInfoTemp.ctrl.CtrlCmd  = *((u8 *)pdata);
			ctrlInfoTemp.ctrl.CtrlMode = *((u8 *)pdata +1);			
			if(Length == 4)
			{
				ctrlInfoTemp.ctrl.CtrlSourceAddr = getSelfAddr();
				ctrlInfoTemp.ctrl.CtrlSourceType = 0;
			}
			if(Length >=7)
			{
				ctrlInfoTemp.ctrl.CtrlSourceAddr = *((u8 *)pdata +3);
				ctrlInfoTemp.ctrl.CtrlSourceType = *((u8 *)pdata +4);
				memcpy(&ctrlInfoTemp.ctrl.UserID , (u8 *)pdata +5,2);
				if((*((u8 *)pdata +2)) <= CTRL_PARAM_LEN_MAX)
				{
					ctrlInfoTemp.ParamLength = *((u8 *)pdata +2);				
					memcpy(ctrlInfoTemp.ParamBuf , (u8 *)pdata +7,ctrlInfoTemp.ParamLength);
				}
			}
			
//			if((ctrlInfoTemp.actionType == ENUM_SINGLE_ACTION)&&(Id <=SINGLEACTION_NUM_MAX))
//			{
//				flag =0;
//				for(i=0; i<SINGLE_ACTION_TRIGGER_MAX;i++)
//				{
//					if(SysParam.SigAction[Id-1].tiggerParam[i].cond.TriggerEnable)
//					{
//						flag =1;
//						break;
//					}
//				}
//				
//				if(flag)
//					ctrlInfoTemp.ctrl.CtrlMode = 1;
//				else
//					ctrlInfoTemp.ctrl.CtrlMode = 0;
//			}
			
			InsCtrlBufQueue(&ctrlInfoTemp);
			ret =1;
			
			PLATFORM_OS_SemPost(&SYSCtrlSem);
		}
			break;
		
		case DATA_MSG_CTRL_ALARM:  //data0:起   1:停  data1:addr    
		{			
			memset(temp,0,sizeof(temp));
			temp[0] = ENUM_ALARM;
			temp[1] = 1;
			temp[2] = *((u8 *)pdata);
			temp[3] = ActionCtrl_ModeType_AUTO;
			temp[4] = 0;
			temp[5] = getSelfAddr();
			temp[6] = ActionCtrl_SourceType_CONTROLLER;
			
			UdpAppProcCtrl(AppProcCtrl_control_request,temp,9,*((u8 *)pdata+1));
		}
			break;
		
		case DATA_MSG_GetActionInfo:
		{
			pActionInfo = getActionInfoHead();
			*((ActionInfoType **)pdata) = pActionInfo;
			*retLen = getActionInfoBufSzie();
		}
			break;
		
		case DATA_MSG_GetStopInfo:
		{
			*((u8 *)pdata) =0;
			*((u8 *)pdata+1) =0;
			*retLen =2;
			
			if(SysState.globalState.hwEmergency == SYSSTOP_PRESS)
			{
				*((u8 *)pdata) = SysParam.globalParam.SupportParam.currentSupportID;
				break;
			}
							
			for(i =0;i<256;i++)
			{
				if((SysStopState[i].SYSStop.EMERGENCY == SYSSTOP_PRESS)&&
				  (i!=SysParam.globalParam.SupportParam.currentSupportID))
				{
					*((u8 *)pdata) = i;
					break;
				}			
			}
			
			if(SysState.globalState.hwStop == SYSSTOP_PRESS)
			{
				*((u8 *)pdata+1) = SysParam.globalParam.SupportParam.currentSupportID;
				break;
			}

			hostaddr = SysParam.globalParam.SupportParam.currentSupportID;
			if((hostaddr+ SysParam.globalParam.SupportParam.StopScope )<=SysParam.globalParam.SupportParam.MaxSupportID)
				max = hostaddr+ SysParam.globalParam.SupportParam.StopScope ;
			else
				max = SysParam.globalParam.SupportParam.MaxSupportID;
			
			if((SysParam.globalParam.SupportParam.MinSupportID+SysParam.globalParam.SupportParam.StopScope)<=hostaddr)
				min = hostaddr- SysParam.globalParam.SupportParam.StopScope ;
			else
				min = SysParam.globalParam.SupportParam.MinSupportID;
			
			for(i=min;i<=max;i++)
			{
				if((SysStopState[i].SYSStop.STOP == SYSSTOP_PRESS)&&
				  (i!=SysParam.globalParam.SupportParam.currentSupportID))
				{
					*((u8 *)pdata+1) = i;
					break;
				}
			}			
		}
		break;
		
		case DATA_MSG_SetStopPress:
		{
			SysState.globalState.hwStop =*((u8 *)pdata);
			SysState.globalState.stopSrc = Id;
		}
		break;
		
		case  DATA_MSG_COPY_FROM_FLASH:
		{ 
			ee_ReadBytes((uint8_t *)(&SysNetParam), NET_PARAM_START, sizeof(NetParamType));
			ee_ReadBytes((uint8_t *)(&SysParam), SYS_PARAM_START, sizeof(SysParamType));
		}
			break;

		case  DATA_MSG_COPY_TO_FLASH:     
		{ 
			ee_WriteBytes((uint8_t *)(&SysNetParam), NET_PARAM_START, sizeof(NetParamType));
			ee_WriteBytes((uint8_t *)(&SysParam), SYS_PARAM_START, sizeof(SysParamType));			
		}
			break;
		
		case DATA_MSG_GET_IP_PARAM:
		{
			*((NetParamType *)pdata) = SysNetParam;
		}
		break;
		
		case DATA_MSG_SET_IP_PARAM_GLOBAL:
		{
			
			memset(SysStopState,0,sizeof(SysStopType)*256);
			memset(&SysState.globalState,0,14);
			
			SysState.globalState.slaveMode =SYSMODE_CONFIG;
			//SysNetParam = *((NetParamType *)pdata);
			memcpy(&SysNetParam , (NetParamType *)pdata,12);
			SysParam.globalParam.SupportParam.currentSupportID=SysNetParam.NetParam_IPAddress[3];
			flashStart = (u8 *)(&SysParam.globalParam.SupportParam.currentSupportID)-(u8 *)(&SysParam);
			ee_WriteBytes((u8 *)(&SysParam.globalParam.SupportParam.currentSupportID), SYS_PARAM_START+flashStart, 2);
			ee_WriteBytes((uint8_t *)(&SysNetParam), NET_PARAM_START, sizeof(NetParamType));
			setUpdateFlag(1);
			setSelfAddr(SysNetParam.NetParam_IPAddress[3],1);
			updatePersonParam();
		}
		break;
		
		case DATA_MSG_SET_IP_PARAM:
		{
			
			
			memset(SysStopState,0,sizeof(SysStopType)*256);
			memset(&SysState.globalState,0,14);
			
			SysState.globalState.slaveMode =SYSMODE_CONFIG;

			memcpy(&SysNetParam , (NetParamType *)pdata,12);
			SysParam.globalParam.SupportParam.currentSupportID=SysNetParam.NetParam_IPAddress[3];
			flashStart = (u8 *)(&SysParam.globalParam.SupportParam.currentSupportID)-(u8 *)(&SysParam);
			ee_WriteBytes((u8 *)(&SysParam.globalParam.SupportParam.currentSupportID), SYS_PARAM_START+flashStart, 2);			
			ee_WriteBytes((uint8_t *)(&SysNetParam), NET_PARAM_START, sizeof(NetParamType));
			setUpdateFlag(1);
			setSelfAddr(SysNetParam.NetParam_IPAddress[3],0);
			updatePersonParam();
		}
		break;
		
		case DATA_MSG_SET_VLAN_PARAM:
		{
			SysNetParam.Vlan = *((u8 *)pdata);
			ee_WriteBytes((uint8_t *)(&SysNetParam), NET_PARAM_START, sizeof(NetParamType));
		}
		break;	

		case DATA_MSG_GET_DEVICETYPE_PARAM:
		{
			if(SysNetParam.deviceType == UDP_DEVICE_CONTROLLER_ChaoQian)
				*((u8 *)pdata) =1;
			else if(SysNetParam.deviceType == UDP_DEVICE_CONTROLLER_ZiYiJinWei)
				*((u8 *)pdata) =2;	
			else if(SysNetParam.deviceType == UDP_DEVICE_CONTROLLER_PingBanLieChe) //ghy Ôö¼ÓÆ½°åÁÐ³µÉè±¸
				*((u8 *)pdata) =3;	
			else 
				*((u8 *)pdata) =0;
		}
		break;
		
		case DATA_MSG_SET_DEVICETYPE_PARAM:
		{
			if(*((u8 *)pdata) ==1)
				SysNetParam.deviceType = UDP_DEVICE_CONTROLLER_ChaoQian;
			else if(*((u8 *)pdata) ==2)
				SysNetParam.deviceType = UDP_DEVICE_CONTROLLER_ZiYiJinWei;
			else if(*((u8 *)pdata) ==3) //ghy 
				SysNetParam.deviceType = UDP_DEVICE_CONTROLLER_PingBanLieChe;
			else
				SysNetParam.deviceType =UDP_DEVICE_CONTROLLER;
//			SysNetParam.deviceType = *((u8 *)pdata);
			SysNetParam.NetParam_IPAddress[2] =SysNetParam.deviceType;
			setAppProcDeviceType(SysNetParam.deviceType);
			ee_WriteBytes((uint8_t *)(&SysNetParam), NET_PARAM_START, sizeof(NetParamType));
			setUpdateFlag(1);
		}
		break;
		
		case DATA_MSG_SET_LeftSelfID_PARAM:
		{
			memcpy(SysNetParam.LeftSelfID,(u8 *)pdata,DEVICE_SELFID_MAX);
			ee_WriteBytes((uint8_t *)(&SysNetParam), NET_PARAM_START, sizeof(NetParamType));
		}
		break;

		case DATA_MSG_SET_RightSelfID_PARAM:
		{
			memcpy(SysNetParam.RightSelfID,(u8 *)pdata,DEVICE_SELFID_MAX);
			ee_WriteBytes((uint8_t *)(&SysNetParam), NET_PARAM_START, sizeof(NetParamType));
		}
		break;		

		case DATA_MSG_SetCommCheckState:
//    20230130   jhy   Not reflected			
//		{
//			if(*((u8 *)pdata+1) == DEVICE_SELFID_LEFT)
//			{
//				if(SysState.globalState.LeftCommState != *((u8 *)pdata))
//				{
//					if(*((u8 *)pdata))
//						WriteFault_API(FAULT_HOST_OTHER,0,FAULT_EVENT_LEFT_NEIGHBOR_COMMUNICATION_WRONG,FAULT_STATUS_NORMAL);
//					else
//						WriteFault_API(FAULT_HOST_OTHER,0,FAULT_EVENT_LEFT_NEIGHBOR_COMMUNICATION_WRONG,FAULT_STATUS_FAULT);
//				}
//				SysState.globalState.LeftCommState = *((u8 *)pdata);
//			}
//			else
//			{
//				if(SysState.globalState.RightCommState != *((u8 *)pdata))
//				{
//					if(*((u8 *)pdata))
//						WriteFault_API(FAULT_HOST_OTHER,0,FAULT_EVENT_RIGHT_NEIGHBOR_COMMUNICATION_WRONG,FAULT_STATUS_NORMAL);
//					else
//						WriteFault_API(FAULT_HOST_OTHER,0,FAULT_EVENT_RIGHT_NEIGHBOR_COMMUNICATION_WRONG,FAULT_STATUS_FAULT);					
//				}				
//				SysState.globalState.RightCommState = *((u8 *)pdata);
//			}
//		}
		break;
		
		case DATA_MSG_SetCommCheckGlobalCRC:
		{
			if(*((u8 *)pdata+1) == DEVICE_SELFID_LEFT)
			{
				if(SysState.globalState.LeftglobalCRCState != *((u8 *)pdata))
				{
					if(*((u8 *)pdata) == 0)
					{
						if(leftParaNotsame<3)
						{
							leftParaNotsame++;
							return ret;
						}
					}
					else
					{
						leftParaNotsame = 0;
					}
						
					SysState.globalState.LeftglobalCRCState = *((u8 *)pdata);
				
					if(*((u8 *)pdata))
						WriteFault_API(FAULT_HOST_OTHER,0,FAULT_EVENT_ALL_PARA_NOT_MATCH_LEFTNEIGHBOR,FAULT_STATUS_NORMAL);
					else
						WriteFault_API(FAULT_HOST_OTHER,0,FAULT_EVENT_ALL_PARA_NOT_MATCH_LEFTNEIGHBOR,FAULT_STATUS_FAULT);
					
					flag = SysState.globalState.LeftglobalCRCState == 0?1:0;
					
					temp[0] = DEVDATA_MSG_STATE_PARA_DIF;
					temp[1] = ENUM_SYSTEM;
					temp[2] = flag;
					temp[3] = 0;
					temp[4] = 0;				
				
					UdpAppProcCtrl(AppProcCtrl_stateReport_indication,temp,5,UDP_LINK_BROADCAST_ADDR);
				}		
			}
			else if(*((u8 *)pdata+1) == DEVICE_SELFID_RIGHT)
			{
				
				if(*((u8 *)pdata) == 0)
				{
					if(rightParaNotsame<3)
					{
						rightParaNotsame++;
						return ret;
					}
				}
				else
				{
					rightParaNotsame = 0;
				}
							
				if(SysState.globalState.RightglobalCRCState != *((u8 *)pdata))
				{
					SysState.globalState.RightglobalCRCState = *((u8 *)pdata);
					if(*((u8 *)pdata))
						WriteFault_API(FAULT_HOST_OTHER,0,FAULT_EVENT_ALL_PARA_NOT_MATCH_RIGHTNEIGHBOR,FAULT_STATUS_NORMAL);
					else
						WriteFault_API(FAULT_HOST_OTHER,0,FAULT_EVENT_ALL_PARA_NOT_MATCH_RIGHTNEIGHBOR,FAULT_STATUS_FAULT);
					
					
					flag = SysState.globalState.RightglobalCRCState == 0?1:0;
					
					
					temp[0] = DEVDATA_MSG_STATE_PARA_DIF;
					temp[1] = ENUM_SYSTEM;
					temp[2] = flag;
					temp[3] = 0;
					temp[4] = 0;				
				
					UdpAppProcCtrl(AppProcCtrl_stateReport_indication,temp,5,UDP_LINK_BROADCAST_ADDR);
				}
			}
			if(SysState.globalState.LeftglobalCRCState == 0 || SysState.globalState.RightglobalCRCState == 0)
			{
				SysStopState[SysNetParam.NetParam_IPAddress[3]].SYSStop.OVERALLPARA = 1;
			}
			else
			{
				SysStopState[SysNetParam.NetParam_IPAddress[3]].SYSStop.OVERALLPARA = 0;
			}
		}
		break;

		case DATA_MSG_SetCommCheckGlobalVERSION:
		{
			if(*((u8 *)pdata+1) == DEVICE_SELFID_LEFT)
				versionState[0] = (*((u8 *)pdata) >0)?0:1;
			else
				versionState[1] = (*((u8 *)pdata) >0)?0:1;
			
			flag = versionState[0]|versionState[1];
			if(SysState.globalState.LeftNextVersionState != (u16)flag)
			{
				if(flag==0)
				{
					WriteFault_API(FAULT_HOST_OTHER,0,FAULT_EVENT_PARGRAM_NOT_MATCH_NEIGHBOR,FAULT_STATUS_NORMAL);
				}
				else
				{
					WriteFault_API(FAULT_HOST_OTHER,0,FAULT_EVENT_PARGRAM_NOT_MATCH_NEIGHBOR,FAULT_STATUS_FAULT);
				}
				
				temp[0] = DEVDATA_MSG_STATE_FAIL;
				temp[1] = ENUM_SYSTEM;
				temp[2] = flag;
				temp[3] = 0;
				temp[4] = 0;				
				
				UdpAppProcCtrl(AppProcCtrl_stateReport_indication,temp,5,UDP_LINK_BROADCAST_ADDR);
			}
			SysStopState[SysNetParam.NetParam_IPAddress[3]].SYSStop.VERSION = flag;
			SysState.globalState.LeftNextVersionState = (u16)flag;
		}
		break;		
		
		case DATA_MSG_RESET_CONTROLLER:
		{
			PLATFORM_OS_TimeDly(getSelfAddr()*10);
			ETH_Stop();
			BCM_551CS_LOW();
			NVIC_SystemReset(); 
		}
		break;
		
		case DATA_MSG_RESET_ALL_CONTROLLER:
		{
			funcid = AppProcServerType_reset_controller;
			UdpAppProcCtrl(AppProcCtrl_serverOperate_request,&funcid,2,UDP_LINK_BROADCAST_ADDR);
			reset_timerid = addTimerEvent(1000, 1, TimerCallback_reset);  
			if(reset_timerid != 0xff)
			{
				startTimerEvent(reset_timerid);
			}		
		}
		break;	

		case DATA_MSG_RESET_DEFAULTPARAM:
		{
			//先解除eeprom写保护
			GPIO_ResetBits(GPIO_PORT_I2C, I2C_WP_PIN);
			bsp_DelayMS(10);
			
			ee_ReadBytes(temp, FIRST_TURNON_START, FIRST_TURNON_LEN);
			temp[0]=0x66;
			temp[1]=0x66;
			temp[2]=0x66;
			temp[3]=0x66;
			ee_WriteBytes(temp, FIRST_TURNON_START, FIRST_TURNON_LEN);
			ETH_Stop();
			BCM_551CS_LOW();
			NVIC_SystemReset();
		}
		break;	

		case DATA_MSG_RESET_ALL_DEFAULTPARAM:
		{
			funcid = AppProcServerType_reset_defaultParam;
			UdpAppProcCtrl(AppProcCtrl_serverOperate_request,&funcid,2,UDP_LINK_BROADCAST_ADDR);
			reset_timerid = addTimerEvent(1000, 1, TimerCallback_reset_param);  
			if(reset_timerid != 0xff)
			{
				startTimerEvent(reset_timerid);
			}		
		}
		break;	
		case DATA_MSG_SetSysTime:
		{
			time.sys_year =  *((u8 *)pdata)+2000;
//			time.sys_data.RTC_Year = *((u8 *)pdata);
			time.sys_data.RTC_Month = *((u8 *)pdata+1);
			time.sys_data.RTC_Date  = *((u8 *)pdata+2);
			time.sys_time.RTC_Hours = *((u8 *)pdata+3);
			time.sys_time.RTC_Minutes = *((u8 *)pdata+4);
			time.sys_time.RTC_Seconds = *((u8 *)pdata+5);		
			Set_Data_Time(time);
		}
		break;
		
		default:
			break;
	}
    return ret;	
}

int DeviceMngDataFun(DevDataMsg_t msg,u8 supportAddr, u8 Type,u8 Id,u16 Length,u16 Offset,void *pdata)
{
	u8 ret=0;
	
	if (msg> DEVDATA_MSG_MAX)		return -1;
	
	switch(msg)
	{
		case DEVDATA_MSG_STATE_EMERGENCY:
		{
			if(Id >=SYSSTOP_STATE_MAX)
				break;
			
			SysStopState[supportAddr].SYSStop.EMERGENCY =Id;
			SysStopState[supportAddr].SYSStop.HWEMERGENCY = Id;
			SysEmergencyStateUpdate();
//			if(SysParam.globalParam.DeviceWirelessParam.WirelessEnable == EnableType_ENABLE)
//		    SendWirelessBusData(Id,supportAddr);
		}
		break;
		
		case DEVDATA_MSG_STATE_BISUO:
		{
			if(Id >=SYSSTOP_STATE_MAX)
				break;
			
			SysStopState[supportAddr].SYSStop.BISUO =Id;
		}
			break;
		
		case DEVDATA_MSG_STATE_STOP:
		{
			if(Id >=SYSSTOP_STATE_MAX)
				break;
			
			SysStopState[supportAddr].SYSStop.STOP =Id;
		}
			break;
		
		case DEVDATA_MSG_STATE_PERSONBISUO:
		{
			setZeroPersonLockState(supportAddr,Id);
		}
			break;
		
		case DEVDATA_MSG_STATE_FAIL:
		{
			if(Id >=SYSSTOP_STATE_MAX)
				break;
			
			SysStopState[supportAddr].SYSStop.VERSION =Id;
		}
			break;
		
		case DEVDATA_MSG_STATE_SENSOR:
		{
			setManualSensorState(Id,(SensorStateType *)pdata);
		}
			break;
		case DEVDATA_MSG_STATE_PARA_DIF:
			if(Id >=SYSSTOP_STATE_MAX)
				break;
			SysStopState[supportAddr].SYSStop.OVERALLPARA =Id;
			break;
		default:
			break;
	}
	
	return 1;
}

int getActionParam(u8 combAction_num,u8 actionID,SigActionParamType **pactionParam)
{
	int i;
	
	if((combAction_num >COMBACTION_NUM_MAX)||(actionID >SINGLEACTION_NUM_MAX)
	   ||(combAction_num ==0)||(actionID ==0))
		return 0;
	
	for(i=0;i<COMBACTION_SINGLE_NUM;i++)
	{
		if(SysParam.CombAction[combAction_num-1].actionParam[i].SigParam.baseParam.ID == actionID)
		{
			*pactionParam = &(SysParam.CombAction[combAction_num-1].actionParam[i].SigParam);
			return 1;
		}
	}
	
	return 0;
}

int getActionParam_single(u8 actionID,SigActionParamType **pactionParam)
{
	int i;
	
	if(actionID >=SINGLEACTION_NUM_MAX)
		return 0;
	
	*pactionParam = &SysParam.SigAction[actionID];
	return 1;
}

u8 getSysServerAddr(void)
{
	return 0;
}

int SysParamFun(DataMsg_t msg, uint64_t levels,u16 Length,u16 Offset,void *pdata,u16 *retLen,u8 globalFlag,u8 Report)
{
	ParamUnit	*pUnit=NULL;
	uint64_t level;
	u16 unitLen,Input_Type=0;
	u8 ret=0;
	u32	 flashStart;
	u8 i;
	u8 moduletype,moduleID;
	u8 temp[54];
	u16 flag;
	
	if (msg> DATA_MSG_MAX)		return -1;
	
	switch(msg)
    {		
		case DATA_MSG_READ_OBJECT_PARAM:
		{
			LevelToStruct(levels,&pUnit,&unitLen,&moduletype,&moduleID);
			if(((Offset+Length)> unitLen)||(pUnit ==NULL))
				ret=0;
			else
			{
				*((u16 **)pdata) = pUnit;
				*retLen = unitLen;
			}
		}
			break;
		
		case DATA_MSG_WRITE_OBJECT_PARAM:
		{
			LevelToStruct(levels,&pUnit,&unitLen,&moduletype,&moduleID);
			if(((Offset+Length)> unitLen)||(pUnit ==NULL))
				ret=0;
			else
			{
				memcpy((u8 *)pUnit+Offset*2,(u8 *)pdata,Length*2);
				ParamCheck();
				flashStart = (u8 *)pUnit-(u8 *)(&SysParam);
				if(flashStart>=0)
				{
					ee_WriteBytes((u8 *)pdata, SYS_PARAM_START+flashStart+Offset*2, Length*2);
					for(i=0;i<Length;i++)
						ParamProcess_address((u8 *)pUnit+Offset*2+i*2,globalFlag);
				}
				else
					flashStart=0;
				
//				if((u8 *)pUnit == (u8 *)(&SysParam.globalParam.SupportParam.currentSupportID))
//				{
//					SysNetParam.NetParam_IPAddress[3] = SysParam.globalParam.SupportParam.currentSupportID;
//					ee_WriteBytes((uint8_t *)(&SysNetParam), NET_PARAM_START, sizeof(NetParamType));
//					setUpdateFlag(1);
//					setSelfAddr(SysNetParam.NetParam_IPAddress[3],1);					
//				}
				
				if((globalFlag)&&((u8 *)(pUnit+Offset) != (u8 *)(&SysParam.globalParam.SupportParam.currentSupportID)))
				{										
					if(Length*2 > 40)
						break;
					ModifyTheLevel(levels,temp);
//					memcpy(temp,(u8 *)(&level),8);
					flag=1;
					memcpy(&temp[8],(u8 *)(&flag),2);
					memcpy(&temp[10],(u8 *)(&Length),2);
					memcpy(&temp[12],(u8 *)(&Offset),2);
					memcpy(&temp[14],(u8 *)pdata,Length*2);
					Input_Type = *(((u8 *)(pdata))-2);
					memcpy(&temp[16],(u8 *)(&Input_Type),2);
					if(Report)
						UdpAppProcCtrl(AppProcCtrl_setParam_UIMode_request,temp,16+Length*2,UDP_LINK_BROADCAST_ADDR);					
				}
				else
				{
					if(Length*2 > 40)
						break;
					ModifyTheLevel(levels,temp);
					if((u8 *)(pUnit+Offset) == (u8 *)(&SysParam.globalParam.SupportParam.currentSupportID))
						flag=1;
					else
						flag = 0;
					memcpy(&temp[8],(u8 *)(&flag),2);
					memcpy(&temp[10],(u8 *)(&Length),2);
					memcpy(&temp[12],(u8 *)(&Offset),2);
					memcpy(&temp[14],(u8 *)pdata,Length*2);
					Input_Type = *(((u8 *)(pdata))-2);
					memcpy(&temp[16],(u8 *)(&Input_Type),2);
					if(Report)
						UdpAppProcCtrl(AppProcCtrl_setParam_UIMode_request,temp,16+Length*2,getSysServerAddr());					
				}
				
				flag =0;
				for(i=0;i<Length;i++)
				{
					if(IsJsonParamGlobal((flashStart+Offset*2)/2+i))
					{
						flag =1;
						break;
					}
				}
				if(flag)
				{
					SysState.globalState.globalCRC = GlobalParamCRCCal();
					setUdpAppSelfParamInfo((u8 *)(&SysState.globalState.globalCRC),2);
				}
					
			}
		}
			break;	
		
		default:
			break;
	}
    return ret;		
}

uint8_t Is_FirstTurnON(void)
{
#include "RTE_Components.h"             // Component selection
	uint32_t      FirstTurnOn;
	uint8_t       JsonSuccess;
	uint8_t       times;
	u8 ret;
	
#if(JSONANDPROGRAMAPART == 1)	
	ee_ReadBytes((uint8_t *)(&JsonSuccess), JSON_FILE_RECEPTION_SUCCESS, 1);
	bsp_DelayMS(20);
	if(JsonSuccess == 0x00)//上电前刚接收完一次新的json
	{
		JsonSuccess = 0x78;//下次外部启动
		ee_WriteBytes((uint8_t *)(&JsonSuccess), JSON_STARTUP_MODE, 1);
		bsp_DelayMS(20);
		return  Json_Newlyreceived; 
	}	
	else if(JsonSuccess == 0x01)//上电前刚因为读取json死机
	{
		//获取已经死了几次，第五次死机的时候读内部json启动并且恢复默认参数
		ee_ReadBytes((uint8_t *)(&times), CRASH_TIMES, 1);
		if((RCC->CSR &0x20000000) && (times >=3) && times != 0xFF)
		{
			JsonSuccess = 0xFF;//下次内部启动
			ee_WriteBytes((uint8_t *)(&JsonSuccess), JSON_STARTUP_MODE, 1);
				bsp_DelayMS(20);
			return  Json_Watchdogreset; 
		}
		else
		{
			times++;
			ee_WriteBytes((uint8_t *)(&times), CRASH_TIMES, 1);
//			JsonSuccess = 0x78;//下次外部启动
//			ee_WriteBytes((uint8_t *)(&JsonSuccess), JSON_STARTUP_MODE, 1);
			bsp_DelayMS(20);
			return  Json_Watchdognotreset; 
		}
		
		
	}	
	else
#endif		
	{
		bsp_DelayMS(20);
		ret = ee_ReadBytes((uint8_t *)(&FirstTurnOn), FIRST_TURNON_START, FIRST_TURNON_LEN);
		if(ret)
		{
			if(FirstTurnOn == RESTORE_FASTORY_SETTING)
			{
				return Json_Recoveryparameters;
			}
			else if(FirstTurnOn != FIRST_TURNON_FLAG)
			{
				bsp_DelayMS(20);
				JsonSuccess = 0xFF;//下次内部启动
				ee_WriteBytes((uint8_t *)(&JsonSuccess), JSON_STARTUP_MODE, 1);
				bsp_DelayMS(20);
				return  Json_Modify;
			}
			else
			{
			}
		}
	}
	return Json_Other;
}

void Write_FirstFlag(void)
{
	uint32_t      FirstTurnOn;
	
	FirstTurnOn = FIRST_TURNON_FLAG;
	ee_WriteBytes((uint8_t *)(&FirstTurnOn), FIRST_TURNON_START, FIRST_TURNON_LEN);
}

void SetSysFuncMode(u8 mode)
{
	SysState.globalState.slaveMode =mode;
}

u8 GetSysFuncMode(void)
{
	return SysState.globalState.slaveMode;
}

int SysDataFun_ext(DataMsg_t msg, u32 Length,void *pdata,u32 *retLen)
{
	uint32_t i;
	u8 flag;
	uint32_t cnt=0;
	
	switch(msg)
	{
		case DATA_MSG_GET_GLOBAL_PARAM:
		{
			*((u8 **)pdata) = (u8 *)(&SysParam);
			*retLen = sizeof(SysParamType);			
		}
		break;
		
		case DATA_MSG_SET_GLOBAL_PARAM:
		{
			if((pdata==NULL)||(Length!=sizeof(SysParamType)))
				break;
			
			flag =0;
			setUpdateTImerApi(0);
			for(i=0;i<Length/2;i++)
			{	
				if(i == (sizeof(SysParamType)-sizeof(SysParam.globalParam))/2)//如果是本架架号
				{
					continue;
				}
				if(IsJsonParamGlobal(i))
				{
					memcpy((u8 *)(&SysParam)+2*i,(u8 *)pdata+2*i,2);
					ee_WriteBytes((u8 *)pdata+2*i, SYS_PARAM_START+2*i, 2);
					ParamProcess_address((u8 *)(&SysParam)+2*i,0);
					flag=1;
				}
				if(cnt != i/(Length/200))
				{
					cnt = i/(Length/200);
					setUpdateTImerApi(cnt+1);
					PLATFORM_OS_TimeDly(50);
					WDTReload(0);
				}
			}
			if(flag)
			{
				SysState.globalState.globalCRC = GlobalParamCRCCal();
				setUdpAppSelfParamInfo((u8 *)(&SysState.globalState.globalCRC),2);
			}			
			setUpdateTImerApi(100);
		}
		break;
		
		case DATA_MSG_SET_ALL_PARAM:
		{
			if((pdata==NULL)||(Length!=sizeof(SysParamType)))
				break;
			
			flag =0;
			setUpdateTImerApi(0);
			for(i=0;i<Length/2;i++)
			{
				if(i == (sizeof(SysParamType)-sizeof(SysParam.globalParam))/2)//如果是本架架号
					continue;
				if(IsJsonParamGlobal(i))
				{
					flag=1;
				}
				
				if(2*i != ((u8 *)(&SysParam.globalParam.SupportParam.currentSupportID) -(u8 *)(&SysParam)))
				{
					memcpy((u8 *)(&SysParam)+2*i,(u8 *)pdata+2*i,2);
					ee_WriteBytes((u8 *)pdata+2*i, SYS_PARAM_START+2*i, 2);
					ParamProcess_address((u8 *)(&SysParam)+2*i,0);
				}
				
				if(cnt != i/(Length/200))
				{
					cnt = i/(Length/200);
					setUpdateTImerApi(cnt+1);
					PLATFORM_OS_TimeDly(50);
					WDTReload(0);
				}
			}
			
			if(flag)
			{
				SysState.globalState.globalCRC = GlobalParamCRCCal();
				setUdpAppSelfParamInfo((u8 *)(&SysState.globalState.globalCRC),2);
			}			
			setUpdateTImerApi(100);
		}
		break;
		
		default:
			break;
	}
}

/*******************************************************************************************
*功能描述：获取服务器在线状态
*入口参数：无
*返回值：Type类型状态
					0：不在线；
					1：在线
*******************************************************************************************/
uint8_t getSeverStatus(void)
{
	
	if(SysParam.globalParam.SupportParam.ServerEnable == EnableType_DISABLE)
		return 0;
//	if(SysState.globalState.ServerState == OnlineType_OFFLINE)
//		return 0;
	return 1;
}

#endif
