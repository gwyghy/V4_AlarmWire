/*
*********************************************************************************************************
*	                                            DoubleWires
*                           Commuication Controller For The Mining Industry
*
*                                  (c) Copyright 1994-2013  HNDZ 
*                       All rights reserved.  Protected by international copyright laws.
*
*
*    File    : sys_data_type.h
*    Module  : system function
*    Version : V1.0
*    History :
*   -----------------
*              ϵͳ״̬�ṹ���������Ͷ���
*              Version  Date           By            Note
*              v1.0     
*
*********************************************************************************************************
*/
#ifndef __SYS_DATA_TYPE_H__
#define __SYS_DATA_TYPE_H__

#include "includes.h"
#include "sensor.h"
#include "singleAction.h"
#include "combAction.h"
#include "groupAction.h"
#include "part.h"

#define  SERVER_REPORT_ENABLE

#define  SYSDEVICE_NUM			22
#define  CTRL_PARAM_LEN_MAX		100
#define  CTRL_BUF_LEN			10
#define  ACTIONINFO_BUF_LEN		40
#define  PERSON_MAX				11
#define  MANUALACTIONINFO_LEN   10

enum
{
	REPORTENUM_ACTION=0,
	REPORTENUM_EMERGENCY,
	REPORTENUM_BISUO,
	REPORTENUM_STOP,
	REPORTENUM_FAIL,
	REPORTENUM_SENSOR,
	REPORTENUM_DEVICE,
	REPORTENUM_PERSON,
	REPORTENUM_SYSTEM,
	REPORTENUM_MAX
};

enum
{
	SYSMODE_NORMAL =0,
	SYSMODE_SLAVE_CTRL,
	SYSMODE_CONFIG,
	SYSMODE_CONFIG_FIN,
	SYSMODE_MASTER_CTRL
};

typedef struct
{
	ParamUnit		verHigh;
	ParamUnit		verLow;
}softVerType;

typedef struct
{
	ParamUnit		driverEnable;   //�豸ʹ��
	ParamUnit		driverID; 		//�豸����can������
	ParamUnit		deviceType; 	//�豸����
	ParamUnit		deviceNum;  	//�豸���
	ParamUnit		deviceState;	//�豸״̬
	ParamUnit		deviceValue;	//�豸��ֵ
	softVerType		deviceAppVer;	//Ӧ�ð汾
	softVerType		deviceBootVer;	//boot�汾	
}deviceStateType;

typedef struct
{
	ParamUnit 		personNum;		//id
	ParamUnit 		personDis;		//����	
	ParamUnit  		personPer;		//Ȩ��
	ParamUnit  		personStar;		//״̬
}personStateType;

typedef struct
{	
	ParamUnit		emergencyState; //��ͣ״̬ 0:����  1����ͣ
	ParamUnit		bisuoState;     //����״̬
	ParamUnit		stopState;      //ֹͣ״̬
	ParamUnit		hwEmergency;    //Ӳ�����¼�ͣ
	ParamUnit		hwBisuo;        //Ӳ�����±���
	ParamUnit		hwStop;    		//Ӳ������ֹͣ
	ParamUnit   	PersonLockState; // ��Ա����
	ParamUnit		Resetreason;    //�ϴθ�λԭ��
//	ParamUnit       Startupmode;		//������ʽ	1.�յ���json  2.�ϴ������쳣���ҿ��Ź���λ   
//											  //3.�ϴ������쳣���ҷǿ��Ź���λ   4.�ָ�Ĭ�ϲ���   
//											  //5.�ڲ�json���ֶ��޸�  6.���������������	
//	ParamUnit       Jsonmode;	 	//json   2���ڲ�json����  1���ⲿjson ����
	ParamUnit		B12VState;      //B12V״̬	      0:����
	ParamUnit		slaveMode;      //�ӿ�״̬,0:���� 1���ӿ� 2����ַ����״̬
	ParamUnit       LeftCommState;  //���ڼ�ͨ��״̬  0������
	ParamUnit       RightCommState; //���ڼ�ͨ��״̬  0������
	ParamUnit		globalCRC;      //ȫ�ֲ���crc
	ParamUnit		LeftglobalCRCState; //ȫ�ֲ���crc�Ƿ�һ��
	ParamUnit		RightglobalCRCState; //ȫ�ֲ���crc�Ƿ�һ��
	ParamUnit		globalVersionState; //ȫ�ְ汾�Ƿ�һ��
	ParamUnit		LeftNextVersionState; //�ڼܰ汾�Ƿ�һ��
//	ParamUnit		RightNextVersionState; //�ڼܰ汾�Ƿ�һ��
	ParamUnit		IrDevID;          //�������ID
	ParamUnit		IrRevStatus;      //�������״̬
	ParamUnit		IrRevTimes;       //������մ���
	
	ParamUnit		DirectionOperation; //���з��� //0: ��   1����	
	ParamUnit		OperationNumber;	//�����	
	ParamUnit		ShearersLocation;	//ú��λ��
	
	ParamUnit		runMode; 			//0������ģʽ  1������ģʽ	
	
	ParamUnit       NowCutNumber;     //�ݶ����ڴ��䵱ǰ����
	ParamUnit		MineStopStatus;    //0��1��
	ParamUnit		MineStartStation; //ú������λ�õ���ʼ
	ParamUnit		MineEndStation;  //ú������λ�õĽ���
	
	
	ParamUnit		SignalStrength; 	//0�����ź� 1���ź��� 2���ź���  3���ź�ǿ
	ParamUnit   ServerState;
	ParamUnit		stopSrc;    		//ֹͣ��Դ
	
	
}globalStateType;

typedef struct
{
	SigActionStateType		SigAction[SINGLEACTION_NUM_MAX];
	CombActionStateType		CombAction[COMBACTION_NUM_MAX];
	groupActionStateType	GroupAction[GROUPACTION_NUM_MAX];
	PartStateType			Part[PART_NUM_MAX];
	SensorStateType			Sensor[SENSOR_MAX];
	SensorStateType     	ManualSensor[SENSOR_MAX];
	deviceStateType			Device[SYSDEVICE_NUM];    //ֻ��ʾʹ�ܵ��豸
	personStateType			Person[PERSON_MAX];
	globalStateType			globalState;
}SysStateType;

/**
* @ϵͳ����.
*      relUniqueId - ����ID��Ĭ���Ƿ������� RELEASEID <sys_default.h>
*      uImageId    - ��д�ļ���ʶ�֣�uImageId�����ı䣬��Ҫ��ʼ��ϵͳ���ݣ��������ĳ��������У�
*                    ���û�ж����ݽṹ���иı����Ҫ�ָ�Ĭ�ϵ����ݽṹ������Ҫ�޸�uImageId
*                    Ĭ����IMAGEID <sys_default.h>
*      DeviceType  - �豸����
*/
typedef struct
{
	uint32_t      relUniqueId;               ///< release date
	uint32_t      uImageId;                  ///< uimage id
	uint32_t      DeviceType;                ///< product type
}STR_SysInfo;

typedef struct
{
	u8				  actionType;			//0:������ 1����϶���  2�����鶯��
	u8				  actionID;	
	u8				  ParamLength;
	u8				  ParamBuf[CTRL_PARAM_LEN_MAX];
	ActionCtrlType    ctrl;
	void              *pNext;
}SysCtrlInfoType;

typedef struct
{
	SysCtrlInfoType    buf[CTRL_BUF_LEN];	//������Ϣbuf
	u8                 Cnt;					//������������
	SysCtrlInfoType*   pHead;				//������������
	SysCtrlInfoType*   pEnd;				//��������ĩ��
}SysCtrlInfoBufType;

enum
{
	SYSSTOP_UNPRESS =0,
	SYSSTOP_PRESS,
	SYSSTOP_STATE_MAX
};

typedef union
{
	struct{						
		u8		STOP   		:1;				//ֹͣ״̬
		u8		BISUO   	:1;				//����״̬
		u8		EMERGENCY   :1;				//��ͣ״̬
		u8		HWEMERGENCY :1;			    //Ӳ����ͣ״̬
		u8		VERSION     :1;         //�������汾״̬
		u8    OVERALLPARA :1;         //ȫ�ֲ�����һ��
		u8    NONE     		:2;             
	} SYSStop;
	u8 FuncStop;
}SysStopType;

typedef struct
{
	u8				  actionType;			//0:������ 1����϶���  2�����鶯��
	u8				  actionID;	
	u8				  runstate;
	void              *pNext;
}ActionInfoType;

typedef struct
{
	ActionInfoType     buf[ACTIONINFO_BUF_LEN];	//������Ϣbuf
	u8                 Cnt;					//������������
	ActionInfoType*    pHead;				//������������
	ActionInfoType*    pEnd;				//��������ĩ��
	ActionInfoType*    pFree;				//��������
}ActionInfoBufType;


#endif /* __SYS_DATA_TYPE_H__ */
