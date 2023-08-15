#ifndef __GROUPACTION_H__
#define __GROUPACTION_H__

#include "includes.h"
#include "sensor.h"
#include "combAction.h"
#include "singleAction.h"


#define GROUPACTION_NUM_MAX		22 
#define GROUP_ACTION_TRIGGER_MAX		2
#define GROUP_ACTION_SUPPORT_NUM_MAX    200

/*------------------------param define----------------------*/
typedef enum
{
	groupActionEnum_ActionType_SINGLE=0,
	groupActionEnum_ActionType_COMBINATION
}groupActionEnum_actionType;

typedef enum
{
	groupActionEnum_triggerType_RUN=0,
	groupActionEnum_triggerType_NEXTRUN
}groupActionEnum_triggerType;

typedef struct
{
	ParamUnit 	Name;		//��������
	ParamUnit 	ID; 		//����ID
	ParamUnit   enable;		//ʹ��
	ParamUnit   alarmType;	//��������
	ParamUnit	alarmMode_sound;//��������ʽ
	ParamUnit	alarmMode_light;//�ⱨ����ʽ	
	ParamUnit   alarmTime;	//Ԥ��ʱ��
	ParamUnit	localControlMode;//���ܿ��Ʒ�ʽ���ֶ�/�Զ�
}groupAction_BaseParamType;

typedef struct
{
	ParamUnit 	actionName;		//������������
	ParamUnit	actionType;		//�����������ͣ���϶�������������
	ParamUnit 	actionID; 		//��������ID
	ParamUnit	safeGap;		//��ȫ���
	ParamUnit	scope;			//ִ�з�Χ
	ParamUnit	actionGap;		//ִ�м����
	ParamUnit	delayTime;		//����ܼ���ʱʱ��
	ParamUnit	groupCnt;		//����ִ�м���
	ParamUnit	groupDirct;		//���鷽��
}groupAction_actionParamType;

typedef struct
{
	ParamUnit	TriggerEnable;	//����ʹ��
	ParamUnit 	methodID;		//����ID
	ParamUnit   TriggerActionID;//��������ID����϶���ʱ�ж��˶���ִ�У��Ż���д��������жϣ�
	ParamUnit 	sensorID; 		//������ID
	ParamUnit   sensorParam1;	//����1����
	ParamUnit   sensorParam2;	//����2����
	ParamUnit 	TriggerActionPoint; //��������ʱ��
	ParamUnit 	TriggerDelayTime;   //������ʱʱ��
	ParamUnit 	TriggerType;	//��������
}groupAction_TriggerCondType;

typedef struct
{
	groupAction_BaseParamType		baseParam;
	groupAction_actionParamType		actionParam;
	groupAction_TriggerCondType		tiggerParam[GROUP_ACTION_TRIGGER_MAX];
}groupActionParamType;


/*------------------------state define----------------------*/
typedef struct
{
	ParamUnit		RunState;	//����ִ��״̬
	ParamUnit		RunStateBak;//����ִ��״̬(����)
	ActionCtrlType	ActionCtrl;	//����������Ϣ
}groupActionStateType;	



/*------------------------module define----------------------*/
enum
{
	GroupModuleParamENUM_STARTID=0,
	GroupModuleParamENUM_ENDID,
	GroupModuleParamENUM_DIRECT,
	GroupModuleParamENUM_MAX
};

typedef struct
{
	groupActionParamType	*pParam;
	groupActionStateType	*pState;
	
	int 					ticks_delay;
	u8						suspend_flag;
	u8						currentOperateId;
	//u8						actionSupport[GROUP_ACTION_SUPPORT_NUM_MAX];
	u8						startID;
	u8						endID;
	u8						direct;
	u8						groupEndID;
	u8						groupFinish;
}groupActionModuleType;

/*------------------------other define----------------------*/
void groupActionModuleInit(void);
void groupActionModuleLoad(u8 moduleID,groupActionParamType *pParam,groupActionStateType *pState);
u8 groupActionCtrlFunc(ActionCtrlType ctrl,u8 actionID,u8 *pctrlParam,u8 length);
u8 groupActionStepProcess(u16 delay,u8 moduleID);
u8 groupActionAllStop(u8 actionID);
u8 stopGroupAction(u8 startId,u8 endId,u8 moduleID,ActionCtrl_CmdType cmd);
void groupActionJudge(ActionCtrlType ctrl,u8 actionID);

#endif
