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
	ParamUnit 	sensorName;		//����������
	ParamUnit 	sensorID; 		//������ID
	ParamUnit   enable;			//������ʹ��
	PointIDType pointID; 		//��ID
	ParamUnit   alarmEnable;	//����ʹ��
	ParamUnit   alarmType;		//��������	
	ParamUnit	alarmMode_sound;//��������ʽ
	ParamUnit	alarmMode_light;//�ⱨ����ʽ
	ParamUnit   alarmTime;		//����ʱ��
	ParamUnit   type;			//����������
	ParamUnit   scopeMax;		//��������
	ParamUnit   scopeMin;		//��������
	ParamUnit   freqMax;		//ģ��������
	ParamUnit   freqMin;		//ģ��������
	ParamUnit	precision;		//ԭʼֵ����
	ParamUnit	precision_logic;//����ֵ����
	ParamUnit   factor;			//У��ϵ��
	ParamUnit   offset;			//���ƫ��
	ParamUnit   extParam;		//��չ����
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
	ParamUnit	originalValue;	//ԭʼֵ
	ParamUnit   calculateValue;	//����ֵ
	ParamUnit   rate;			//�仯��
	ParamUnit	state_online;	//����״̬
	ParamUnit	state_fail;		//����״̬
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
