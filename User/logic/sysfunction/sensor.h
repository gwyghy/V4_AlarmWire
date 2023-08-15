#ifndef __SENSOR_H__
#define __SENSOR_H__

#include "includes.h"
#include "devmng_api.h"
#include "alarm.h"

#define SENSOR_MAX    20 
#define FREQ_DEAL_OFFSET    10 //10%


#define SENSOR_FAIL_VALUE   0x9999

enum
{
	ENUM_SINGLE_ACTION =0,
	ENUM_COMBINATION_ACTION,
	ENUM_GROUP_ACTION,
	ENUM_ALARM,
	ENUM_SENSOR,
	ENUM_PART,
	ENUM_DEVICE,
	ENUM_SYSTEM,
	ENUM_GROUP_ACTION_KZQ,
	ENUMTYPE_MAX
};

/*------------------------param define----------------------*/

typedef enum
{
	PointType_FREQ =0,
	PointType_SWITCH_OPEN,
	PointType_SWITCH_CLOSE,
	PointTypeMAX
}PointType;

enum
{
	SensorExtParamType_NONE =0,
	SensorExtParamType_Reverse ,
	SensorExtParamType_MAX
};

typedef struct
{
	ParamUnit 	sensorName;		//传感器名称
	ParamUnit 	sensorID; 		//传感器ID
	ParamUnit   enable;			//传感器使能
	PointIDType pointID; 		//点ID
	ParamUnit   alarmEnable;	//报警使能
	ParamUnit   alarmType;		//报警类型	
	ParamUnit	alarmMode_sound;//声报警方式
	ParamUnit	alarmMode_light;//光报警方式
	ParamUnit   alarmTime;		//报警时间
	ParamUnit   type;			//传感器类型
	ParamUnit   scopeMax;		//量程上限
	ParamUnit   scopeMin;		//量程下限
	ParamUnit   freqMax;		//模拟量上限
	ParamUnit   freqMin;		//模拟量下限
	ParamUnit	precision;		//原始值精度
	ParamUnit	precision_logic;//计算值精度
	ParamUnit   factor;			//校正系数
	ParamUnit   offset;			//零点偏置
	ParamUnit   extParam;		//扩展参数
}SensorParamType;


/*------------------------state define----------------------*/

typedef enum 
{
	FailType_NORMAL=0,
	FailType_FAIL ,
	FailType_MORELIMIT
}FailType;	

typedef struct
{
	ParamUnit	originalValue;	//原始值
	ParamUnit   calculateValue;	//计算值
	ParamUnit   rate;			//变化率
	ParamUnit	state_online;	//在线状态
	ParamUnit	state_fail;		//故障状态
}SensorStateType;


/*------------------------sensor module define----------------------*/
typedef int (*SensorApiType )(u8 sensorID,u8 sensorID2,ParamUnit param1,ParamUnit param2,void *pdata);

enum
{
	SENSORAPI_NONE =0,
	SENSORAPI_MoreThan,    //x>y
	SENSORAPI_LessThan,    //x<y
	SENSORAPI_MoreThan_rate,    //rate x>y
	SENSORAPI_LessThan_rate,    //rate x<y
	SENSORAPI_MoreThan_angle,   //angle x>y
	SENSORAPI_LessThan_angle,   //angle x<y	
	SENSORAPI_Sensor1MoreSensor2,
	SENSORAPI_Sensor1LessSensor2,
	SENSORAPI_MAX
};

typedef struct
{
	SensorParamType		*pParam;
	SensorStateType		*pState;
	SensorStateType   *manualState;
	SensorApiType		apiFunc[SENSORAPI_MAX];
	
	ParamUnit			CalculateValue_old;
	ParamUnit     CalculateValue_last;
	ParamUnit     MoreLimitTimes;
	u16                 tick;
}SensorModuleType;


/*------------------------other define----------------------*/
extern SensorModuleType 	sensorModule[SENSOR_MAX];

void SensorModuleMngTask(void);
void SensorModuleInit(SensorParamType *pParam,SensorStateType *pState,SensorStateType *manualpState ,u8 sensorId);
u8 getCalculateValue(u8 sensorID,ParamUnit *value);
int SensorApi_MoreThan(u8 sensorID,u8 sensorID2,ParamUnit param1,ParamUnit param2,void *pdata);
int SensorApi_LessThan(u8 sensorID,u8 sensorID2,ParamUnit param1,ParamUnit param2,void *pdata);
int SensorApi_MoreThan_rate(u8 sensorID,u8 sensorID2,ParamUnit param1,ParamUnit param2,void *pdata);
int SensorApi_LessThan_rate(u8 sensorID,u8 sensorID2,ParamUnit param1,ParamUnit param2,void *pdata);
int SensorApi_MoreThan_angle(u8 sensorID,u8 sensorID2,ParamUnit param1,ParamUnit param2,void *pdata);
int SensorApi_LessThan_angle(u8 sensorID,u8 sensorID2,ParamUnit param1,ParamUnit param2,void *pdata);
int SensorApi_Sensor1MoreSensor2(u8 sensorID,u8 sensorID2,ParamUnit param1,ParamUnit param2,void *pdata);
int SensorApi_Sensor1LessSensor2(u8 sensorID,u8 sensorID2,ParamUnit param1,ParamUnit param2,void *pdata);
SensorApiType getSensorApi(u8 apiID);
u8 getSensorState(u8 sensorID,SensorStateType *value);
void setManualSensorState(u8 sensorID,SensorStateType * manualState);
u8 getManualSensorcalcValue(u8 singleActionId,char ** calcValue);
u8 getSensorStateDataToReport(u8* sensorData);

#endif
