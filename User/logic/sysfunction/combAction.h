#ifndef __COMBACTION_H__
#define __COMBACTION_H__

#include "singleAction.h"


#define COMBACTION_SINGLE_NUM	10

/*------------------------param define----------------------*/
typedef struct
{
	ParamUnit 	Name;		//��������
	ParamUnit 	ID; 		//����ID
	ParamUnit   enable;		//ʹ��
	ParamUnit   alarmType;	//��������
	ParamUnit	alarmMode_sound;//��������ʽ
	ParamUnit	alarmMode_light;//�ⱨ����ʽ	
	ParamUnit   alarmTime;	//Ԥ��ʱ��
	ParamUnit	cycleCnt;	//ѭ������
	ParamUnit	cycleStart1;//ѭ����ʼ
	ParamUnit	cycleEnd1;  //ѭ������
	ParamUnit	cycleStart2;
	ParamUnit	cycleEnd2;	
	ParamUnit	controlMode;//���Ʒ�ʽ������Զ��/����/Զ��
	ParamUnit	localControlMode;//���ܿ��Ʒ�ʽ���ֶ�/�Զ�
	ParamUnit actionPriority; //�������ȼ�
}CombAction_BaseParamType;

typedef struct
{
	ParamUnit	zeroDelayEnable;	//0����ʱ�Ƿ�ʹ��
	ParamUnit	zeroDelayTime;		//0����ʱʱ��
	SigActionParamType		SigParam;		//����������
}CombAction_actionParamType;

typedef struct
{
	CombAction_BaseParamType	baseParam;
	CombAction_actionParamType	actionParam[COMBACTION_SINGLE_NUM];
}CombActionParamType;


/*------------------------state define----------------------*/
typedef struct
{
	ParamUnit		RunState;	//����ִ��״̬
	ParamUnit		RunStateBak;//����ִ��״̬(����)
	ActionCtrlType	ActionCtrl;	//����������Ϣ
}CombActionStateType;


/*------------------------module define----------------------*/
typedef struct
{
	CombActionParamType		*pParam;
	CombActionStateType		*pState;
	
	int 					ticks_alarm;
	int 					ticks_delay[COMBACTION_SINGLE_NUM];
	u8 					    delayFin[COMBACTION_SINGLE_NUM];
	u8						suspend_flag;
	u16						cycle_cnt;
	u16						cycle_start;
	u16						cycle_end;
	u8						runFin[COMBACTION_SINGLE_NUM];
}CombActionModuleType;


/*------------------------other define----------------------*/
void CombActionModuleInit(void);
void CombActionModuleLoad(u8 moduleID,CombActionParamType *pParam,CombActionStateType *pState);
u8 CombActionCtrlFunc(ActionCtrlType ctrl,u8 actionID);
u8 CombActionStepProcess(u16 delay,u8 moduleID);
u8 getCombActionFin(u8 combNum,u8 actionID,u8 *finFlag);
u8 CombActionAllStop(u8 actionID);
u8 CombActionPerformJudge(ActionCtrlType ctrl,u8 actionID);
u8 checkCombActionPriorityState(u8 moduleID);

#endif
