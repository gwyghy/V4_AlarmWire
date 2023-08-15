#ifndef __COMBACTION_H__
#define __COMBACTION_H__

#include "singleAction.h"


#define COMBACTION_SINGLE_NUM	10

/*------------------------param define----------------------*/
typedef struct
{
	ParamUnit 	Name;		//动作名称
	ParamUnit 	ID; 		//动作ID
	ParamUnit   enable;		//使能
	ParamUnit   alarmType;	//报警类型
	ParamUnit	alarmMode_sound;//声报警方式
	ParamUnit	alarmMode_light;//光报警方式	
	ParamUnit   alarmTime;	//预警时间
	ParamUnit	cycleCnt;	//循环次数
	ParamUnit	cycleStart1;//循环开始
	ParamUnit	cycleEnd1;  //循环结束
	ParamUnit	cycleStart2;
	ParamUnit	cycleEnd2;	
	ParamUnit	controlMode;//控制方式：本架远程/本架/远程
	ParamUnit	localControlMode;//本架控制方式：手动/自动
	ParamUnit actionPriority; //动作优先级
}CombAction_BaseParamType;

typedef struct
{
	ParamUnit	zeroDelayEnable;	//0点延时是否使能
	ParamUnit	zeroDelayTime;		//0点延时时间
	SigActionParamType		SigParam;		//单动作参数
}CombAction_actionParamType;

typedef struct
{
	CombAction_BaseParamType	baseParam;
	CombAction_actionParamType	actionParam[COMBACTION_SINGLE_NUM];
}CombActionParamType;


/*------------------------state define----------------------*/
typedef struct
{
	ParamUnit		RunState;	//动作执行状态
	ParamUnit		RunStateBak;//动作执行状态(备份)
	ActionCtrlType	ActionCtrl;	//动作控制信息
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
