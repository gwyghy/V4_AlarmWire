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
	ParamUnit 	Name;		//动作名称
	ParamUnit 	ID; 		//动作ID
	ParamUnit   enable;		//使能
	ParamUnit   alarmType;	//报警类型
	ParamUnit	alarmMode_sound;//声报警方式
	ParamUnit	alarmMode_light;//光报警方式	
	ParamUnit   alarmTime;	//预警时间
	ParamUnit	localControlMode;//本架控制方式：手动/自动
}groupAction_BaseParamType;

typedef struct
{
	ParamUnit 	actionName;		//关联动作名称
	ParamUnit	actionType;		//关联动作类型（组合动作、单动作）
	ParamUnit 	actionID; 		//关联动作ID
	ParamUnit	safeGap;		//安全间隔
	ParamUnit	scope;			//执行范围
	ParamUnit	actionGap;		//执行间隔数
	ParamUnit	delayTime;		//组间或架间延时时间
	ParamUnit	groupCnt;		//组内执行架数
	ParamUnit	groupDirct;		//成组方向
}groupAction_actionParamType;

typedef struct
{
	ParamUnit	TriggerEnable;	//触发使能
	ParamUnit 	methodID;		//方法ID
	ParamUnit   TriggerActionID;//关联动作ID（组合动作时判定此动作执行，才会进行触发条件判断）
	ParamUnit 	sensorID; 		//传感器ID
	ParamUnit   sensorParam1;	//参数1定义
	ParamUnit   sensorParam2;	//参数2定义
	ParamUnit 	TriggerActionPoint; //触发动作时刻
	ParamUnit 	TriggerDelayTime;   //触发延时时间
	ParamUnit 	TriggerType;	//触发类型
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
	ParamUnit		RunState;	//动作执行状态
	ParamUnit		RunStateBak;//动作执行状态(备份)
	ActionCtrlType	ActionCtrl;	//动作控制信息
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
