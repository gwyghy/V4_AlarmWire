#ifndef __PART_H__
#define __PART_H__

#include "includes.h"
#include "sensor.h"


#define PART_NUM_MAX		    16
#define PART_TRIGGER_NUM_MAX    4

/*------------------------param define----------------------*/

typedef struct
{
	ParamUnit 	Name;		//部件名称
	ParamUnit 	ID; 		//部件ID
	ParamUnit   ActionID1;  //关联动作ID1
	ParamUnit   ActionID2;  //关联动作ID2
}PartBaseParamType;

typedef struct
{
	ParamUnit	TriggerEnable;	//触发使能
	ParamUnit 	methodID;		//方法ID
	ParamUnit 	sensorID; 		//传感器ID
	ParamUnit   sensorParam1;	//参数1定义
	ParamUnit   sensorParam2;	//参数2定义
}PartTriggerCondType;

typedef struct
{
	ParamUnit 	alarmEnable;	//报警使能
	ParamUnit 	alarmType; 		//报警类型
	ParamUnit	alarmMode_sound;//声报警方式
	ParamUnit	alarmMode_light;//光报警方式	
	//ParamUnit 	alarmTime; 		//报警时间
	ParamUnit   reportInterval; //上报间隔时间
	ParamUnit   reportScope;	//上报浮动范围
	ParamUnit   reportMax;	    //主动发送门槛最大值
	ParamUnit 	reportMin;	    //主动发送门槛最小值
}PartTriggerActionType;

typedef struct
{
	PartTriggerCondType	  cond;
	PartTriggerActionType action;
}PartTriggerType;

typedef struct
{
	PartBaseParamType	baseParam;
	PartTriggerType		tiggerParam[PART_TRIGGER_NUM_MAX];
}PartParamType;


/*------------------------state define----------------------*/

typedef enum  
{	
	ProtectType_Protect=0,
	ProtectType_Normal,			// 正常状态
	ProtectType_LowProtect,		// 超下限保护状态
	ProtectType_HighProtect		// 超上限保护
}ProtectType;

typedef enum
{
	PartDeviceStateEnum_STATE0=0,//状态0：收、降
	PartDeviceStateEnum_STATE1,  //状态1：伸、升
}PartDeviceStateType;

typedef struct
{
	ParamUnit		partState;	//部件状态
}PartStateType;



/*------------------------part module define----------------------*/
typedef struct
{
	PartParamType		*pParam;
	PartStateType		*pState;
	
	int 				ticks_delay[PART_TRIGGER_NUM_MAX];
	u8					alarm_flag[PART_TRIGGER_NUM_MAX];
	ParamUnit			value_bak[PART_TRIGGER_NUM_MAX];
}partModuleType;


/*------------------------other define----------------------*/
void partModuleInit(void);
void partModuleLoad(u8 moduleID,PartParamType *pParam,PartStateType *pState);
u8 partCycleProcess(u8 moduleID,u16 delay) ;
uint8_t getPartInSingleActionOfSensorId(u8 singleActionId);
#endif
