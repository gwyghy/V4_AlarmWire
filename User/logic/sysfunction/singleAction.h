#ifndef __SINGLEACTION_H__
#define __SINGLEACTION_H__

#include "includes.h"
#include "sensor.h"


#define SINGLEACTION_NUM_MAX	40  
#define COMBACTION_NUM_MAX		6 

#define SUPPORT_ADDR_MIN 	1
#define SUPPORT_ADDR_MAX 	250

#define SINGLEACTION_ALARM_ALWAY  0
#define SINGLEACTION_ALARM_MODE_SOUND_DEFAULT  1
#define SINGLEACTION_ALARM_MODE_LIGHT_DEFAULT  1


/*------------------------param define----------------------*/
#define TRIGGER_ACTION_MAX    		4
#define SINGLE_ACTION_TRIGGER_MAX   5
#define TRIGGER_TIMER_MAX			40	
#define SINGLE_MODULE_MAX           SINGLEACTION_NUM_MAX //(COMBACTION_NUM_MAX*COMBACTION_SINGLE_NUM+SINGLEACTION_NUM_MAX)

typedef enum
{
	TriggerType_SELF =0,
	TriggerType_OTHER
}TriggerType;

typedef enum
{
	TriggerEventType_ACTION =0,
	TriggerEventType_ALARM,
	TriggerEventType_STATE,
	TriggerEventType_METHOD,
	TriggerEventTypeMAX
}TriggerEventType;

typedef enum
{
	TriggerActionPoint_START =0,
	TriggerActionPoint_RUN,
	TriggerActionPoint_STOP
}TriggerActionPointType;

typedef enum
{
	TriggerDirectType_LEFT =0,
	TriggerDirectType_RIGHT,
	TriggerDirectType_BOTH
}TriggerDirectType;

typedef enum
{
	ControlModeType_LocalRemote=0,
	ControlModeType_Local,
	ControlModeType_Remote
}ControlModeType;

typedef enum
{
	ActionPriority_High = 0,
	ActionPriority_Middle = 1,
	ActionPriority_Low = 2
}ActionPriority;

typedef struct
{
	ParamUnit 	Name;		//��������	
	ParamUnit 	ID; 		//����ID
	ParamUnit   enable;		//ʹ��
	PointIDType pointID; 	//��ID
}SigAction_BaseParamType;

typedef struct
{
	ParamUnit   alarmType;		//��������
	ParamUnit	alarmMode_sound;//��������ʽ
	ParamUnit	alarmMode_light;//�ⱨ����ʽ
	ParamUnit   alarmTime;		//Ԥ��ʱ��
	ParamUnit 	delayTime;		//��ʱʱ��
	ParamUnit 	execTime; 		//ִ��ʱ��
	ParamUnit	controlMode;    //���Ʒ�ʽ������Զ��/����/Զ��
	ParamUnit	localControlMode;//���ܿ��Ʒ�ʽ���ֶ�/�Զ�
	ParamUnit	mutexActionID1;  //��ֹ����1
	ParamUnit	mutexActionID2;  //��ֹ����2
}SigAction_ActionParamType;

typedef struct
{
	ParamUnit	TriggerEnable;		    //����ʹ��
	ParamUnit 	TriggerActionPoint; //��������ʱ��
	ParamUnit 	methodID;		        //����ID
	ParamUnit 	sensorID; 		      //������ID
	ParamUnit   sensorID2;          //������ID2
	ParamUnit   sensorParam1;	      //����1����
	ParamUnit   sensorParam2;	      //����2����
}SigAction_TriggerCondType;

typedef struct
{
	ParamUnit 	TriggerActionID;	  //��������ID
	ParamUnit 	TriggerActionName; 	//������������
	ParamUnit   TriggerActionEnable;//��������ʹ��
	ParamUnit   TriggerType;		    //��������
	ParamUnit   TriggerActionCmd;   //����ִ�з�ʽ
	ParamUnit   TriggerEventType;	  //�����¼�����
	ParamUnit 	TriggerDelayTime;   //������ʱʱ��
	ParamUnit 	TriggerDirection;   //��������
	ParamUnit 	TriggerGap;         //�������
	ParamUnit 	TriggerSupportNum;  //��������
	ParamUnit 	ExtTriggerFuncID;   //���ⴥ���ص�ID
}SigAction_TriggerActionType;

typedef struct
{
	SigAction_TriggerCondType	cond;
	SigAction_TriggerActionType action[TRIGGER_ACTION_MAX];
}SigAction_TriggerType;

typedef struct
{
	SigAction_BaseParamType		baseParam;
	SigAction_ActionParamType	actionParam;
	SigAction_TriggerType		tiggerParam[SINGLE_ACTION_TRIGGER_MAX];
}SigActionParamType;


/*------------------------state define----------------------*/
typedef enum
{	
	RunStateType_IDLE = 0,	// 0����״̬
	RunStateType_ALARM,	    // 1��ǰΪԤ��״̬
	RunStateType_DELAY,	    // 2��ǰΪ��ʱ״̬
	RunStateType_STOP,	    // 3��ǰΪֹͣ״̬
	RunStateType_STARTUP,	  // 4��ǰΪ����״̬
	RunStateType_RUN ,	    // 5��ǰΪ����״̬
	RunStateType_RUN_MANUAL,// 6��ǰΪ����״̬(�ֶ�ʱ)
	RunStateType_SUSPEND ,	// 7��ǰΪ��ͣ״̬
	RunStateType_SUSPEND_ACTION ,// 8��ǰΪ��ͣ״̬,����������ʱ
	RunStateType_MAX	
}Action_RunStateType;

typedef enum
{
//	ActionCtrl_CmdType_NONE=0,      //������
	ActionCtrl_CmdType_STARTUP=0,   //0ֱ������
	ActionCtrl_CmdType_STOP,        //1ֹͣ
	ActionCtrl_CmdType_WAITSTARTUP, //2��ʱ����
	ActionCtrl_CmdType_SUSPEND,     //3��ͣ
	ActionCtrl_CmdType_RESUME,      //4�ָ�
	ActionCtrl_CmdType_SUSPEND_ACTION,//5��ͣ������������ʱ
	ActionCtrl_CmdTypeMAX
}ActionCtrl_CmdType;

typedef enum
{
	ActionCtrl_ModeType_MANUAL =0,  //�ֶ�����
	ActionCtrl_ModeType_AUTO        //�Զ�����
}ActionCtrl_ModeType;

typedef enum
{
	ActionCtrl_SourceType_CONTROLLER=0,	//������
	ActionCtrl_SourceType_HANDLER,		//ң����
	ActionCtrl_SourceType_SERVER,		//������
	ActionCtrl_SourceType_CONSOLE,		//����̨
	ActionCtrl_SourceType_REMOTE,		//Զ��
	ActionCtrl_SourceType_MAX
}ActionCtrl_SourceType;

typedef struct
{
	ParamUnit	CtrlCmd;	   		//��������
	ParamUnit	CtrlMode;			//���Ʒ�ʽ
	ParamUnit	CtrlSourceAddr;		//����Դ��ַ
	ParamUnit	CtrlSourceType;		//����Դ����
	ParamUnit	UserID;				//�û�ID
}ActionCtrlType;

typedef struct
{
	ParamUnit		RunState;    //����ִ��״̬
	ParamUnit		RunStateBak; //����ִ��״̬(����)
	ParamUnit   RunTime;	   //����ʱ��
	ParamUnit		CombNum;     //��������������϶�����	
	ActionCtrlType	ActionCtrl;	//����������Ϣ
}SigActionStateType;	


/*------------------------module define----------------------*/
typedef int (*sigActionApiType )(u8 actionID,u8 combID,void *pdata);

enum
{
	SIGACTIONAPI_NONE =0,
	SIGACTIONAPI_COMB_STOP,    //������϶���ֹͣ
	SIGACTIONAPI_MAX
};

typedef struct
{
	SigActionParamType		*pParam;
	SigActionStateType		*pState;
	
	int						ticks_alarm;
	int 					ticks_run;
	u8						suspend_flag;
	u8						suspend_action_flag;
	u8						combAction_num;
	u8            trigger_flag;
	u8						runFin[COMBACTION_NUM_MAX];
	sigActionApiType		apiFunc[SIGACTIONAPI_MAX];
}SigActionModuleType;


typedef struct
{
	SigAction_TriggerActionType		*pTigger;
	ActionCtrlType *pCtrl;
	int		ticks;
	u8		enbale;
	u8 		moduleID;
	u8      combID;
}TiggerTimerType;


/*------------------------other define----------------------*/
void SigActionModuleLoad(u8 moduleID,SigActionParamType *pParam,SigActionStateType *pState);
void SigActionTrggerTimerInit(void);
void SigActionModuleInit(void);
u8 SigActionCtrlFunc(ActionCtrlType ctrl,u8 actionID,u8 combAction_num);
u8 SigActionCtrlFunc_stop(ActionCtrlType ctrl,u8 actionID,u8 combAction_num);
void TiggerDelayCycle(u16 delay);
u8 getSigActionFin_comb(u8 combNum,u8 actionID,u8 *finFlag);
u8 clearSigActionFin_comb(u8 combNum,u8 actionID);
u8 SigActionStepProcess(u16 delay,u8 moduleID);
u8 SigActionAllStop(u8 moduleID);
u8 SigActionPerformJudge(ActionCtrlType ctrl,u8 actionID,u16 *pmutexAction);

int sigActionApi_CombStopFunc (u8 actionID,u8 combID,void *pdata);

u8 getSingleActionState(u8 moduleID, Action_RunStateType* type);
u8 getSigActionTriggerFlag_comb(u8 actionID,u8 *state);
u8 setSigActionTriggerFlag_comb(u8 actionID,u8 state);
void SigActionRunStateJudge(ActionCtrlType ctrl,u8 moduleID);
u8 AutoSigActionAllStop(u8 moduleID);
#endif
