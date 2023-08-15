/*
*********************************************************************************************************
*	                                            DoubleWires
*                           Commuication Controller For The Mining Industry
*
*                                  (c) Copyright 1994-2013  HNDZ 
*                       All rights reserved.  Protected by international copyright laws.
*
*
*    File    : sys_param_type.h
*    Module  : system function
*    Version : V1.0
*    History :
*   -----------------
*              系统参数结构的数据类型定义
*              Version  Date           By            Note
*              v1.0     
*
*********************************************************************************************************
*/
#ifndef __SYS_PARAM_TYPE_H__
#define __SYS_PARAM_TYPE_H__

#include "includes.h"
#include "sys_data_type.h"

#define		KEY_HAS_PARAM_NUM			2
#define		KEY_CONFIG_MAX				6
#define		ASSISTKEY_CONFIG_MAX		6
#define		ASSIST_PAGE_MAX				5
#define		UISENSOR_CONFIG_MAX			6

enum
{
	globalParamEnum_Support =1,
	globalParamEnum_Server,
	globalParamEnum_Key,
	globalParamEnum_AssistKey,
	globalParamEnum_UIConfig,
	globalParamEnum_GroupAssistKey,
	globalParamEnum_DeviceAlarm,
	globalParamEnum_DevicePeople,
	globalParamEnum_DeviceInfirred,
	globalParamEnum_DeviceWireless,
	globalParamEnum_DeviceHub, //增加HUB ghy
	globalParamEnum_DeviceTrans,
	globalParamEnum_MAX
};

typedef struct
{
	ParamUnit		currentSupportID;  //本架编号
	ParamUnit		SupportDirect;	   //架号增向
	ParamUnit		MinSupportID;	     //最小架号
	ParamUnit		MaxSupportID;	     //最大架号
	ParamUnit		StopScope;		     //停止范围
	ParamUnit		GapSupportNum;	   //隔架架数
	ParamUnit		ServerEnable;	     //服务器使能
	ParamUnit		MasterCtrlTime;    //主控时间


	ParamUnit   ManualCtrlFirst;   //手动控制优先

	ParamUnit		SupportType;       //支架类型，0：中间架 1：过渡架  2：端头架
	ParamUnit		DangganEnable;//挡杆闭锁
	ParamUnit		MinBisuoID;   //闭锁小号
	ParamUnit		MaxBisuoID;	 //闭锁大号
	ParamUnit		Paring_Method; //对码方式
	
}globalParam_SupportType;

typedef struct
{
	ParamUnit		ResetController;   //复位控制器
	ParamUnit		ResetServer;	   //复位服务器
	ParamUnit		TransInterfaceParam;//传送接口表
	ParamUnit		TransSensorParam;  //传送传感器接口
	ParamUnit		TransAssistKey;    //传送辅助动作
	ParamUnit		TransKeyConfig;    //传送键盘配置
	ParamUnit		TransGlobalParam;  //传送全局参数
	ParamUnit		TransAllParam;     //传送所有参数
	ParamUnit		RestoreDefaultParam;//恢复出厂默认参数
	ParamUnit		ParameterPassword;    //参数密码
	ParamUnit		DeveloperPassword;    //开发者密码
	ParamUnit		ServerPassword;    //服务器密码
	ParamUnit		SaveParam;         //保存参数
	ParamUnit		RestoreParam;      //恢复参数
}globalParam_ServerType;

typedef struct
{
	ParamUnit		KeyConfig[KEY_CONFIG_MAX*KEY_HAS_PARAM_NUM];                //键盘配置
}globalParam_KeyType;

typedef struct
{
	ParamUnit		AssistKeyConfig[ASSISTKEY_CONFIG_MAX*ASSIST_PAGE_MAX];       //辅助键盘配置
}globalParam_AssistKeyType;

typedef struct
{
	ParamUnit		UISensorConfig[UISENSOR_CONFIG_MAX];       //主界面显示项配置
}globalParam_UIConfigType;

typedef struct
{
	ParamUnit		GroupAssistKeyConfig[ASSISTKEY_CONFIG_MAX*ASSIST_PAGE_MAX];       //主界面显示项配置
}globalParam_GroupAssistKeyType;

typedef struct
{
	ParamUnit		DevAlarmEnable;  	//声光报警使能
}globalParam_DeviceAlarmType;

typedef struct
{
	ParamUnit		PeoplePosEnable;  		//人员定位使能
	ParamUnit		PeopleBisuoEnable;   	//人员闭锁使能
	ParamUnit		BisuoRange;  			//闭锁距离
	ParamUnit		BisuoDifference;  //解锁差值
	ParamUnit		BisuoTime;  			//闭锁时间
	ParamUnit		CheckRange;  		    //检测距离
	ParamUnit		ReportInterval;  	    //上报间隔
	ParamUnit		SendPower;  		    //发送功率
	ParamUnit   LockActionID1;     //闭锁动作ID1
	ParamUnit   LockActionID2;     //闭锁动作ID2
}globalParam_DevicePeopleType;

typedef struct
{
	ParamUnit		InfirredRevEnable;  	//红外接收使能
	ParamUnit		InfirredCodeEnable;  	//红外对码使能
}globalParam_DeviceInfirredType;

typedef struct
{
	ParamUnit		WirelessEnable;  	    //无线使能
}globalParam_DeviceWirelessType;

typedef struct
{
	ParamUnit		HubEnable;  	//HUB1使能
	ParamUnit		Hub2Enable;		//HUB2使能
	ParamUnit		Hub3Enable;		//HUB3使能
}globalParam_DeviceHubType;

typedef struct
{
	ParamUnit  TriggerUp1;
	ParamUnit  TriggerUp2;
	ParamUnit  TriggerDown1;
	ParamUnit  TriggerDown2;
}globalParam_DeviceTransType;

typedef struct
{
	globalParam_SupportType			SupportParam;
	globalParam_ServerType			ServerParam;
	globalParam_KeyType				KeyConfig;
	globalParam_AssistKeyType		AssistKeyConfig;
	globalParam_UIConfigType		UIConfig;
	globalParam_GroupAssistKeyType  GroupAssistKeyConfig;
	globalParam_DeviceAlarmType		DeviceAlarmParam;
	globalParam_DevicePeopleType	DevicePeopleParam;
	globalParam_DeviceInfirredType	DeviceInfirredParam;
	globalParam_DeviceWirelessType	DeviceWirelessParam;
	globalParam_DeviceHubType       DeviceHubParam;
	globalParam_DeviceTransType     DeviceTransParam;
	
}globalParamType;

typedef struct
{
	SigActionParamType		SigAction[SINGLEACTION_NUM_MAX];   //todo 40ok
	CombActionParamType		CombAction[COMBACTION_NUM_MAX];    //todo 16ok
	groupActionParamType	GroupAction[GROUPACTION_NUM_MAX];  //todo 42ok
	PartParamType			Part[PART_NUM_MAX];                //todo 20ok
	SensorParamType			Sensor[SENSOR_MAX];                //todo 32ok
	globalParamType			globalParam;
}SysParamType;

typedef struct
{
	u8	  NetParam_IPAddress[4];
	u8    NetParam_NetMask[4];
	u8    NetParam_GateWay[4];
	u8    NetParam_Mac[6];
	u8	  Vlan;
	u8    deviceType;
	u8    LeftSelfID[12];
	u8    RightSelfID[12];
}NetParamType;

#endif /* __SYS_PARAM_TYPE_H__ */
