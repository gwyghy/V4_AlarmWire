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
*              系统状态结构的数据类型定义
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
	ParamUnit		driverEnable;   //设备使能
	ParamUnit		driverID; 		//设备所属can驱动号
	ParamUnit		deviceType; 	//设备类型
	ParamUnit		deviceNum;  	//设备序号
	ParamUnit		deviceState;	//设备状态
	ParamUnit		deviceValue;	//设备数值
	softVerType		deviceAppVer;	//应用版本
	softVerType		deviceBootVer;	//boot版本	
}deviceStateType;

typedef struct
{
	ParamUnit 		personNum;		//id
	ParamUnit 		personDis;		//距离	
	ParamUnit  		personPer;		//权限
	ParamUnit  		personStar;		//状态
}personStateType;

typedef struct
{	
	ParamUnit		emergencyState; //急停状态 0:正常  1：急停
	ParamUnit		bisuoState;     //闭锁状态
	ParamUnit		stopState;      //停止状态
	ParamUnit		hwEmergency;    //硬件拍下急停
	ParamUnit		hwBisuo;        //硬件拍下闭锁
	ParamUnit		hwStop;    		//硬件拍下停止
	ParamUnit   	PersonLockState; // 人员闭锁
	ParamUnit		Resetreason;    //上次复位原因
//	ParamUnit       Startupmode;		//启动方式	1.收到新json  2.上次启动异常，且看门狗复位   
//											  //3.上次启动异常，且非看门狗复位   4.恢复默认参数   
//											  //5.内部json被手动修改  6.其他情况（正常）	
//	ParamUnit       Jsonmode;	 	//json   2：内部json启动  1：外部json 启动
	ParamUnit		B12VState;      //B12V状态	      0:正常
	ParamUnit		slaveMode;      //从控状态,0:正常 1：从控 2：地址配置状态
	ParamUnit       LeftCommState;  //左邻架通信状态  0：错误
	ParamUnit       RightCommState; //右邻架通信状态  0：错误
	ParamUnit		globalCRC;      //全局参数crc
	ParamUnit		LeftglobalCRCState; //全局参数crc是否一致
	ParamUnit		RightglobalCRCState; //全局参数crc是否一致
	ParamUnit		globalVersionState; //全局版本是否一致
	ParamUnit		LeftNextVersionState; //邻架版本是否一致
//	ParamUnit		RightNextVersionState; //邻架版本是否一致
	ParamUnit		IrDevID;          //红外接收ID
	ParamUnit		IrRevStatus;      //红外接收状态
	ParamUnit		IrRevTimes;       //红外接收次数
	
	ParamUnit		DirectionOperation; //运行方向 //0: 左   1：右	
	ParamUnit		OperationNumber;	//工序号	
	ParamUnit		ShearersLocation;	//煤机位置
	
	ParamUnit		runMode; 			//0：正常模式  1：跟机模式	
	
	ParamUnit       NowCutNumber;     //暂定用于传输当前刀数
	ParamUnit		MineStopStatus;    //0否1是
	ParamUnit		MineStartStation; //煤机所在位置的起始
	ParamUnit		MineEndStation;  //煤机所在位置的结束
	
	
	ParamUnit		SignalStrength; 	//0：无信号 1：信号弱 2：信号中  3：信号强
	ParamUnit   ServerState;
	ParamUnit		stopSrc;    		//停止来源
	
	
}globalStateType;

typedef struct
{
	SigActionStateType		SigAction[SINGLEACTION_NUM_MAX];
	CombActionStateType		CombAction[COMBACTION_NUM_MAX];
	groupActionStateType	GroupAction[GROUPACTION_NUM_MAX];
	PartStateType			Part[PART_NUM_MAX];
	SensorStateType			Sensor[SENSOR_MAX];
	SensorStateType     	ManualSensor[SENSOR_MAX];
	deviceStateType			Device[SYSDEVICE_NUM];    //只显示使能的设备
	personStateType			Person[PERSON_MAX];
	globalStateType			globalState;
}SysStateType;

/**
* @系统参数.
*      relUniqueId - 发布ID，默认是发布日期 RELEASEID <sys_default.h>
*      uImageId    - 烧写文件标识字，uImageId发生改变，需要初始化系统数据，在正常的程序升级中，
*                    如果没有对数据结构进行改变或不需要恢复默认的数据结构，则不需要修改uImageId
*                    默认是IMAGEID <sys_default.h>
*      DeviceType  - 设备类型
*/
typedef struct
{
	uint32_t      relUniqueId;               ///< release date
	uint32_t      uImageId;                  ///< uimage id
	uint32_t      DeviceType;                ///< product type
}STR_SysInfo;

typedef struct
{
	u8				  actionType;			//0:单动作 1：组合动作  2：成组动作
	u8				  actionID;	
	u8				  ParamLength;
	u8				  ParamBuf[CTRL_PARAM_LEN_MAX];
	ActionCtrlType    ctrl;
	void              *pNext;
}SysCtrlInfoType;

typedef struct
{
	SysCtrlInfoType    buf[CTRL_BUF_LEN];	//发送消息buf
	u8                 Cnt;					//发送链表项数
	SysCtrlInfoType*   pHead;				//发送链表首项
	SysCtrlInfoType*   pEnd;				//发送链表末项
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
		u8		STOP   		:1;				//停止状态
		u8		BISUO   	:1;				//闭锁状态
		u8		EMERGENCY   :1;				//急停状态
		u8		HWEMERGENCY :1;			    //硬件急停状态
		u8		VERSION     :1;         //控制器版本状态
		u8    OVERALLPARA :1;         //全局参数不一致
		u8    NONE     		:2;             
	} SYSStop;
	u8 FuncStop;
}SysStopType;

typedef struct
{
	u8				  actionType;			//0:单动作 1：组合动作  2：成组动作
	u8				  actionID;	
	u8				  runstate;
	void              *pNext;
}ActionInfoType;

typedef struct
{
	ActionInfoType     buf[ACTIONINFO_BUF_LEN];	//发送消息buf
	u8                 Cnt;					//发送链表项数
	ActionInfoType*    pHead;				//发送链表首项
	ActionInfoType*    pEnd;				//发送链表末项
	ActionInfoType*    pFree;				//空闲链表
}ActionInfoBufType;


#endif /* __SYS_DATA_TYPE_H__ */
