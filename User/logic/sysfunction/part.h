#ifndef __PART_H__
#define __PART_H__

#include "includes.h"
#include "sensor.h"


#define PART_NUM_MAX		    16
#define PART_TRIGGER_NUM_MAX    4

/*------------------------param define----------------------*/

typedef struct
{
	ParamUnit 	Name;		//��������
	ParamUnit 	ID; 		//����ID
	ParamUnit   ActionID1;  //��������ID1
	ParamUnit   ActionID2;  //��������ID2
}PartBaseParamType;

typedef struct
{
	ParamUnit	TriggerEnable;	//����ʹ��
	ParamUnit 	methodID;		//����ID
	ParamUnit 	sensorID; 		//������ID
	ParamUnit   sensorParam1;	//����1����
	ParamUnit   sensorParam2;	//����2����
}PartTriggerCondType;

typedef struct
{
	ParamUnit 	alarmEnable;	//����ʹ��
	ParamUnit 	alarmType; 		//��������
	ParamUnit	alarmMode_sound;//��������ʽ
	ParamUnit	alarmMode_light;//�ⱨ����ʽ	
	//ParamUnit 	alarmTime; 		//����ʱ��
	ParamUnit   reportInterval; //�ϱ����ʱ��
	ParamUnit   reportScope;	//�ϱ�������Χ
	ParamUnit   reportMax;	    //���������ż����ֵ
	ParamUnit 	reportMin;	    //���������ż���Сֵ
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
	ProtectType_Normal,			// ����״̬
	ProtectType_LowProtect,		// �����ޱ���״̬
	ProtectType_HighProtect		// �����ޱ���
}ProtectType;

typedef enum
{
	PartDeviceStateEnum_STATE0=0,//״̬0���ա���
	PartDeviceStateEnum_STATE1,  //״̬1���졢��
}PartDeviceStateType;

typedef struct
{
	ParamUnit		partState;	//����״̬
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
