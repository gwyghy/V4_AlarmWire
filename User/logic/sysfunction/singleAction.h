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
	ParamUnit 	Name;		//动作名称	
	ParamUnit 	ID; 		//动作ID
	ParamUnit   enable;		//使能
	PointIDType pointID; 	//点ID
}SigAction_BaseParamType;

typedef struct
{
	ParamUnit   alarmType;		//报警类型
	ParamUnit	alarmMode_sound;//声报警方式
	ParamUnit	alarmMode_light;//光报警方式
	ParamUnit   alarmTime;		//预警时间
	ParamUnit 	delayTime;		//延时时间
	ParamUnit 	execTime; 		//执行时间
	ParamUnit	controlMode;    //控制方式：本架远程/本架/远程
	ParamUnit	localControlMode;//本架控制方式：手动/自动
	ParamUnit	mutexActionID1;  //禁止动作1
	ParamUnit	mutexActionID2;  //禁止动作2
}SigAction_ActionParamType;

typedef struct
{
	ParamUnit	TriggerEnable;		    //触发使能
	ParamUnit 	TriggerActionPoint; //触发动作时刻
	ParamUnit 	methodID;		        //方法ID
	ParamUnit 	sensorID; 		      //传感器ID
	ParamUnit   sensorID2;          //传感器ID2
	ParamUnit   sensorParam1;	      //参数1定义
	ParamUnit   sensorParam2;	      //参数2定义
}SigAction_TriggerCondType;

typedef struct
{
	ParamUnit 	TriggerActionID;	  //触发动作ID
	ParamUnit 	TriggerActionName; 	//触发动作名称
	ParamUnit   TriggerActionEnable;//触发动作使能
	ParamUnit   TriggerType;		    //触发类型
	ParamUnit   TriggerActionCmd;   //触发执行方式
	ParamUnit   TriggerEventType;	  //触发事件类型
	ParamUnit 	TriggerDelayTime;   //触发延时时间
	ParamUnit 	TriggerDirection;   //触发方向
	ParamUnit 	TriggerGap;         //触发间隔
	ParamUnit 	TriggerSupportNum;  //触发架数
	ParamUnit 	ExtTriggerFuncID;   //特殊触发回调ID
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
	RunStateType_IDLE = 0,	// 0空闲状态
	RunStateType_ALARM,	    // 1当前为预警状态
	RunStateType_DELAY,	    // 2当前为延时状态
	RunStateType_STOP,	    // 3当前为停止状态
	RunStateType_STARTUP,	  // 4当前为启动状态
	RunStateType_RUN ,	    // 5当前为运行状态
	RunStateType_RUN_MANUAL,// 6当前为运行状态(手动时)
	RunStateType_SUSPEND ,	// 7当前为暂停状态
	RunStateType_SUSPEND_ACTION ,// 8当前为暂停状态,动作继续计时
	RunStateType_MAX	
}Action_RunStateType;

typedef enum
{
//	ActionCtrl_CmdType_NONE=0,      //空命令
	ActionCtrl_CmdType_STARTUP=0,   //0直接启动
	ActionCtrl_CmdType_STOP,        //1停止
	ActionCtrl_CmdType_WAITSTARTUP, //2延时启动
	ActionCtrl_CmdType_SUSPEND,     //3暂停
	ActionCtrl_CmdType_RESUME,      //4恢复
	ActionCtrl_CmdType_SUSPEND_ACTION,//5暂停动作，继续计时
	ActionCtrl_CmdTypeMAX
}ActionCtrl_CmdType;

typedef enum
{
	ActionCtrl_ModeType_MANUAL =0,  //手动控制
	ActionCtrl_ModeType_AUTO        //自动控制
}ActionCtrl_ModeType;

typedef enum
{
	ActionCtrl_SourceType_CONTROLLER=0,	//控制器
	ActionCtrl_SourceType_HANDLER,		//遥控器
	ActionCtrl_SourceType_SERVER,		//服务器
	ActionCtrl_SourceType_CONSOLE,		//操作台
	ActionCtrl_SourceType_REMOTE,		//远控
	ActionCtrl_SourceType_MAX
}ActionCtrl_SourceType;

typedef struct
{
	ParamUnit	CtrlCmd;	   		//控制命令
	ParamUnit	CtrlMode;			//控制方式
	ParamUnit	CtrlSourceAddr;		//控制源地址
	ParamUnit	CtrlSourceType;		//控制源类型
	ParamUnit	UserID;				//用户ID
}ActionCtrlType;

typedef struct
{
	ParamUnit		RunState;    //动作执行状态
	ParamUnit		RunStateBak; //动作执行状态(备份)
	ParamUnit   RunTime;	   //运行时间
	ParamUnit		CombNum;     //单动作所属的组合动作号	
	ActionCtrlType	ActionCtrl;	//动作控制信息
}SigActionStateType;	


/*------------------------module define----------------------*/
typedef int (*sigActionApiType )(u8 actionID,u8 combID,void *pdata);

enum
{
	SIGACTIONAPI_NONE =0,
	SIGACTIONAPI_COMB_STOP,    //所在组合动作停止
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
