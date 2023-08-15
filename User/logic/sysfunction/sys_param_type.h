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
*              ϵͳ�����ṹ���������Ͷ���
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
	globalParamEnum_DeviceHub, //����HUB ghy
	globalParamEnum_DeviceTrans,
	globalParamEnum_MAX
};

typedef struct
{
	ParamUnit		currentSupportID;  //���ܱ��
	ParamUnit		SupportDirect;	   //�ܺ�����
	ParamUnit		MinSupportID;	     //��С�ܺ�
	ParamUnit		MaxSupportID;	     //���ܺ�
	ParamUnit		StopScope;		     //ֹͣ��Χ
	ParamUnit		GapSupportNum;	   //���ܼ���
	ParamUnit		ServerEnable;	     //������ʹ��
	ParamUnit		MasterCtrlTime;    //����ʱ��


	ParamUnit   ManualCtrlFirst;   //�ֶ���������

	ParamUnit		SupportType;       //֧�����ͣ�0���м�� 1�����ɼ�  2����ͷ��
	ParamUnit		DangganEnable;//���˱���
	ParamUnit		MinBisuoID;   //����С��
	ParamUnit		MaxBisuoID;	 //�������
	ParamUnit		Paring_Method; //���뷽ʽ
	
}globalParam_SupportType;

typedef struct
{
	ParamUnit		ResetController;   //��λ������
	ParamUnit		ResetServer;	   //��λ������
	ParamUnit		TransInterfaceParam;//���ͽӿڱ�
	ParamUnit		TransSensorParam;  //���ʹ������ӿ�
	ParamUnit		TransAssistKey;    //���͸�������
	ParamUnit		TransKeyConfig;    //���ͼ�������
	ParamUnit		TransGlobalParam;  //����ȫ�ֲ���
	ParamUnit		TransAllParam;     //�������в���
	ParamUnit		RestoreDefaultParam;//�ָ�����Ĭ�ϲ���
	ParamUnit		ParameterPassword;    //��������
	ParamUnit		DeveloperPassword;    //����������
	ParamUnit		ServerPassword;    //����������
	ParamUnit		SaveParam;         //�������
	ParamUnit		RestoreParam;      //�ָ�����
}globalParam_ServerType;

typedef struct
{
	ParamUnit		KeyConfig[KEY_CONFIG_MAX*KEY_HAS_PARAM_NUM];                //��������
}globalParam_KeyType;

typedef struct
{
	ParamUnit		AssistKeyConfig[ASSISTKEY_CONFIG_MAX*ASSIST_PAGE_MAX];       //������������
}globalParam_AssistKeyType;

typedef struct
{
	ParamUnit		UISensorConfig[UISENSOR_CONFIG_MAX];       //��������ʾ������
}globalParam_UIConfigType;

typedef struct
{
	ParamUnit		GroupAssistKeyConfig[ASSISTKEY_CONFIG_MAX*ASSIST_PAGE_MAX];       //��������ʾ������
}globalParam_GroupAssistKeyType;

typedef struct
{
	ParamUnit		DevAlarmEnable;  	//���ⱨ��ʹ��
}globalParam_DeviceAlarmType;

typedef struct
{
	ParamUnit		PeoplePosEnable;  		//��Ա��λʹ��
	ParamUnit		PeopleBisuoEnable;   	//��Ա����ʹ��
	ParamUnit		BisuoRange;  			//��������
	ParamUnit		BisuoDifference;  //������ֵ
	ParamUnit		BisuoTime;  			//����ʱ��
	ParamUnit		CheckRange;  		    //������
	ParamUnit		ReportInterval;  	    //�ϱ����
	ParamUnit		SendPower;  		    //���͹���
	ParamUnit   LockActionID1;     //��������ID1
	ParamUnit   LockActionID2;     //��������ID2
}globalParam_DevicePeopleType;

typedef struct
{
	ParamUnit		InfirredRevEnable;  	//�������ʹ��
	ParamUnit		InfirredCodeEnable;  	//�������ʹ��
}globalParam_DeviceInfirredType;

typedef struct
{
	ParamUnit		WirelessEnable;  	    //����ʹ��
}globalParam_DeviceWirelessType;

typedef struct
{
	ParamUnit		HubEnable;  	//HUB1ʹ��
	ParamUnit		Hub2Enable;		//HUB2ʹ��
	ParamUnit		Hub3Enable;		//HUB3ʹ��
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
