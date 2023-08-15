/*
*********************************************************************************************************
*	                                            DoubleWires
*                           Commuication Controller For The Mining Industry
*
*                                  (c) Copyright 1994-2013  HNDZ 
*                       All rights reserved.  Protected by international copyright laws.
*
*
*    File    : combAction.c
*    Module  : system function
*    Version : V1.0
*    History :
*   -----------------
*              实现组合动作模型功能
*              Version  Date           By            Note
*              v1.0     
*
*********************************************************************************************************
*/

#define __COMBACTION_C__

#include "combAction.h"
#include "udp_app_proc.h"
#include "sys_msg_def.h"

#if (LOGIC_SYSFUNC_EN == 1)

CombActionModuleType 	CombActionModule[COMBACTION_NUM_MAX];

void CombActionModuleInit(void)
{
	memset(CombActionModule,0,sizeof(CombActionModule));
}

void CombActionModuleLoad(u8 moduleID,CombActionParamType *pParam,CombActionStateType *pState)
{
	if((pState == NULL)||(pParam == NULL)||(moduleID >=COMBACTION_NUM_MAX))
		return;
	
	CombActionModule[moduleID].pParam = pParam;
	CombActionModule[moduleID].pState = pState;
}

static u8 CombActionAutoCtrlFunc(ActionCtrlType ctrl,CombActionParamType *pParam,CombActionStateType *pState)
{
	u8 i,id;
	
	switch(ctrl.CtrlCmd)
	{
		case ActionCtrl_CmdType_STARTUP:
		{
//			if(pState->RunState != RunStateType_IDLE)
//				return 0;
			
			pState->RunState = RunStateType_STARTUP;
			//add log
		}
		break;

		case ActionCtrl_CmdType_WAITSTARTUP:
		{
			if(pState->RunState != RunStateType_IDLE)
				return 0;
			
			pState->RunState = RunStateType_ALARM;
			//add log			
		}
		break;

		case ActionCtrl_CmdType_STOP:
		{
			if(pState->RunState == RunStateType_IDLE)
				return 0;
			
			pState->RunState = RunStateType_STOP;
			//add log				
		}
		break;

		case ActionCtrl_CmdType_SUSPEND:
		{
			if(pState->RunState == RunStateType_IDLE)
				return 0;
			
			pState->RunStateBak = pState->RunState;
			pState->RunState = RunStateType_SUSPEND;
			//add log
		}
		break;

		case ActionCtrl_CmdType_RESUME:
		{
			if(pState->RunState != RunStateType_SUSPEND)
				return 0;
			
			pState->RunState = pState->RunStateBak;	
			
			for(i=0; i<COMBACTION_SINGLE_NUM;i++)
			{
				if(pParam->actionParam[i].SigParam.baseParam.enable == EnableType_ENABLE)
				{									
					ctrl.CtrlCmd = ActionCtrl_CmdType_RESUME;
					ctrl.CtrlMode = pState->ActionCtrl.CtrlMode; //ActionCtrl_ModeType_AUTO;
					ctrl.CtrlSourceType = pState->ActionCtrl.CtrlSourceType; //ActionCtrl_SourceType_CONTROLLER;
					ctrl.CtrlSourceAddr = pState->ActionCtrl.CtrlSourceAddr;
					ctrl.UserID =0;
			
					id = pParam->actionParam[i].SigParam.baseParam.ID;
					if(id ==0)
					{
						continue;
					}
					SigActionCtrlFunc(ctrl,id-1,pParam->baseParam.ID);
				}
			}
			//add log
		}
		break;
		
		default:
			return 0;		
	}
	
	memcpy(&pState->ActionCtrl,&ctrl,sizeof(ActionCtrlType));
	return 1;
}

u8 getCombActionFin(u8 combNum,u8 actionID,u8 *finFlag)
{	
	if((combNum >COMBACTION_NUM_MAX)||(actionID > SINGLEACTION_NUM_MAX))
		return 0;
	
	*finFlag = CombActionModule[combNum -1].runFin[actionID-1];
	return 1;
}

static u8 serverReportAction_comb(u8 moduleID)
{
#ifdef SERVER_REPORT_ENABLE
	u8 temp[40];
	u8 serverID;
	u16 length;
		
	if(getSeverStatus() == 0)
		return 1;
	
	memset(temp,0,sizeof(temp));
	
	temp[0] = REPORTENUM_ACTION;
	temp[1] = ENUM_COMBINATION_ACTION;
	temp[2] = CombActionModule[moduleID].pParam->baseParam.ID;
	length =sizeof(CombActionStateType);
	memcpy(&temp[3],&length,2);
	memcpy(&temp[5],CombActionModule[moduleID].pState,length);
	
	serverID = getSysServerAddr();
	UdpAppProcCtrl(AppProcCtrl_stateReport_indication,temp,5+length,serverID);
#endif
	return 1;
}


u8 checkSigActionTriggerFlag_comb(u8 moduleID, u8 actionID)
{
	CombActionParamType *pParam = CombActionModule[moduleID].pParam;
	u8 j,k,state;
	
	for(j = 0;j<SINGLE_ACTION_TRIGGER_MAX;j++)
	{
		for(k = 0;k<TRIGGER_ACTION_MAX;k++)
		{ 
			if(pParam->actionParam[actionID-1].SigParam.tiggerParam[j].action[k].TriggerActionID == 0)
				continue;
			if(pParam->actionParam[actionID-1].SigParam.baseParam.enable == EnableType_DISABLE)
				continue;
			if(pParam->actionParam[actionID-1].SigParam.baseParam.ID<pParam->actionParam[actionID-1].SigParam.tiggerParam[j].action[k].ExtTriggerFuncID)
				continue;
			if(pParam->actionParam[actionID-1].SigParam.tiggerParam[j].action[k].TriggerActionCmd == ActionCtrl_CmdType_STARTUP ||\
				pParam->actionParam[actionID-1].SigParam.tiggerParam[j].action[k].TriggerActionCmd == ActionCtrl_CmdType_WAITSTARTUP)
			{
				
				if(pParam->actionParam[actionID-1].SigParam.tiggerParam[j].cond.TriggerEnable == EnableType_ENABLE&&\
					pParam->actionParam[actionID-1].SigParam.tiggerParam[j].action[k].TriggerActionEnable == EnableType_ENABLE&&\
					pParam->actionParam[actionID-1].SigParam.baseParam.enable == EnableType_ENABLE)
				{
					if(pParam->actionParam[actionID-1].zeroDelayEnable == EnableType_ENABLE)
						setSigActionTriggerFlag_comb(pParam->actionParam[actionID-1].SigParam.tiggerParam[j].action[k].TriggerActionID-1,1);
					else
					{
						getSigActionTriggerFlag_comb(pParam->actionParam[actionID-1].SigParam.baseParam.ID-1,&state);
						if(state == 1)
						{
							setSigActionTriggerFlag_comb(pParam->actionParam[actionID-1].SigParam.tiggerParam[j].action[k].TriggerActionID-1,1);
						}
					}
				}
			}
		}
	}
}



u8 CombActionStepProcess(u16 delay,u8 moduleID)
{
	CombActionParamType *pParam = CombActionModule[moduleID].pParam;
	CombActionStateType *pState = CombActionModule[moduleID].pState;
	u16 runState;
	u8 id,i,j,k,finFlag,finFlagTemp,finFlagTotal,cycleStepFlag,idtemp,trggerFlag,triggerState;
	ActionCtrlType ctrl;
	Action_RunStateType typetmp;
	u8 loginfo[6];
	
	if((pState == NULL)||(pParam == NULL))
		return 0;
	if(pParam->baseParam.enable == EnableType_DISABLE && pState->RunState == RunStateType_IDLE)
		return 0;
	
	if((CombActionModule[moduleID].suspend_flag)&&(pState->RunState==RunStateType_SUSPEND))
		return 0;
	else
		CombActionModule[moduleID].suspend_flag =0;
	
	runState = pState->RunState;
	if(pState->RunStateBak != pState->RunState)
	{
		serverReportAction_comb(moduleID);
	}

	switch(pState->RunState)
	{
		case RunStateType_IDLE:
		{
			
		}
		break;
		
		case RunStateType_ALARM:
		{			
			if(pState->RunStateBak != pState->RunState)
			{
				TIME_TRACE
				printk("comb %d alarm \r\n", moduleID+1);
				CtrlAlarm((AlarmType)(pParam->baseParam.alarmType ),pParam->baseParam.alarmTime,BaseCtrlType_STARTUP,0,ENUM_COMBINATION_ACTION,moduleID+1,ALARM_MODE_1);
			}
			
			CombActionModule[moduleID].ticks_alarm +=delay;
			if(CombActionModule[moduleID].ticks_alarm > (int)(pParam->baseParam.alarmTime) *LOGIC_TIME_MS)
			{
				CombActionModule[moduleID].ticks_alarm =0;
				pState->RunState = RunStateType_STARTUP;
				CombActionModule[moduleID].cycle_cnt=0;
				CtrlAlarm((AlarmType)(pParam->baseParam.alarmType ),pParam->baseParam.alarmTime,BaseCtrlType_STOP,0,ENUM_COMBINATION_ACTION,moduleID+1,ALARM_MODE_NONE);
			}
		}
		break;
		
		case RunStateType_STARTUP:
		{	
			CombActionModule[moduleID].cycle_cnt=0;
			ctrl.CtrlCmd = ActionCtrl_CmdType_WAITSTARTUP;
			ctrl.CtrlMode = pState->ActionCtrl.CtrlMode; //ActionCtrl_ModeType_AUTO;
			ctrl.CtrlSourceType = pState->ActionCtrl.CtrlSourceType; //ActionCtrl_SourceType_CONTROLLER;
			ctrl.CtrlSourceAddr = pState->ActionCtrl.CtrlSourceAddr;
			ctrl.UserID =0;
	
			id = pParam->actionParam[0].SigParam.baseParam.ID;
			if(id ==0)
			{
				pState->RunState = RunStateType_IDLE;
				break;
			}
			TIME_TRACE
			printk("comb %d startup \r\n", moduleID+1);
			//if(pState->ActionCtrl.CtrlSourceAddr != getSelfAddr())
				AddActionInfo(ENUM_COMBINATION_ACTION,moduleID+1,RunStateType_RUN);
			
			
			for(i=0; i<COMBACTION_SINGLE_NUM;i++)
			{
				idtemp = pParam->actionParam[i].SigParam.baseParam.ID;
				if(idtemp ==0)
				{
					continue;
				}
				setSigActionTriggerFlag_comb(pParam->actionParam[i].SigParam.baseParam.ID-1,0);
				if(pParam->actionParam[i].zeroDelayEnable == EnableType_ENABLE &&\
					pParam->actionParam[i].SigParam.baseParam.enable == EnableType_ENABLE)
				{
					setSigActionTriggerFlag_comb(pParam->actionParam[i].SigParam.baseParam.ID-1,1);
				}
			}
			for(i=0; i<COMBACTION_SINGLE_NUM;i++)
			{
				idtemp = pParam->actionParam[i].SigParam.baseParam.ID;
				if(idtemp ==0)
				{
					continue;
				}
				clearSigActionFin_comb(pParam->baseParam.ID,idtemp);
				checkSigActionTriggerFlag_comb(moduleID,i);
			}
					
			SigActionModuleLoad(id-1,&(pParam->actionParam[0].SigParam),0);
			SigActionCtrlFunc(ctrl,id-1,pParam->baseParam.ID);
			pState->RunState = RunStateType_RUN;
			memset(CombActionModule[moduleID].ticks_delay,0,sizeof(CombActionModule[moduleID].ticks_delay));	
			memset(CombActionModule[moduleID].runFin,0,sizeof(CombActionModule[moduleID].runFin));	
			memset(CombActionModule[moduleID].delayFin,0,sizeof(CombActionModule[moduleID].delayFin));
			CombActionModule[moduleID].cycle_start = pParam->baseParam.cycleStart1;
			CombActionModule[moduleID].cycle_end = pParam->baseParam.cycleEnd1;	

			loginfo[0]=pState->ActionCtrl.CtrlSourceAddr;
			loginfo[1]=pState->ActionCtrl.CtrlSourceType;
			memcpy(&loginfo[2],&pState->ActionCtrl.UserID,2);
			loginfo[4]=pState->ActionCtrl.CtrlMode;
			WriteLog_API(moduleID +1,LOG_TYPE_HANDLE_COMB,LOG_STATUS_START,loginfo,5);
		}
		break;
		
		case RunStateType_RUN:
		{
			for(i=0; i<COMBACTION_SINGLE_NUM;i++)
			{
				if((pParam->actionParam[i].zeroDelayEnable)&&(CombActionModule[moduleID].delayFin[i]==0))
				{
					CombActionModule[moduleID].ticks_delay[i] +=delay;
					if(CombActionModule[moduleID].ticks_delay[i] >= (int)(pParam->actionParam[i].zeroDelayTime)*LOGIC_TIME_MS)
					{
						CombActionModule[moduleID].ticks_delay[i]=0;
						CombActionModule[moduleID].delayFin[i]=1;
									
						ctrl.CtrlCmd = ActionCtrl_CmdType_WAITSTARTUP;
						ctrl.CtrlMode = pState->ActionCtrl.CtrlMode; //ActionCtrl_ModeType_AUTO;
						ctrl.CtrlSourceType = pState->ActionCtrl.CtrlSourceType; //ActionCtrl_SourceType_CONTROLLER;
						ctrl.CtrlSourceAddr = pState->ActionCtrl.CtrlSourceAddr;
						ctrl.UserID =0;
				
						id = pParam->actionParam[i].SigParam.baseParam.ID;
						if(id ==0)
						{
							break;
						}
						SigActionModuleLoad(id-1,&(pParam->actionParam[i].SigParam),0);
						SigActionCtrlFunc(ctrl,id-1,pParam->baseParam.ID);
					}
				}
			}
			
			finFlag =0;
			finFlagTotal=0;
			cycleStepFlag=0;
			if((CombActionModule[moduleID].cycle_end >= CombActionModule[moduleID].cycle_start)
			   &&(pParam->baseParam.cycleCnt !=0)&&(CombActionModule[moduleID].cycle_start!=0))  
			{
							
//				trggerFlag = CombActionModule[moduleID].cycle_end-CombActionModule[moduleID].cycle_start+1;
//				
//				for(i=CombActionModule[moduleID].cycle_start; i<=CombActionModule[moduleID].cycle_end;i++)
//				{
//					id = pParam->actionParam[i-1].SigParam.baseParam.ID;
//					if(id ==0)
//						continue;
//					if(pParam->actionParam[i-1].SigParam.baseParam.enable == EnableType_DISABLE && pParam->actionParam[i-1].zeroDelayEnable == EnableType_ENABLE)
//					{
//						if(trggerFlag)
//							trggerFlag --;
//					}
//					else if(pParam->actionParam[i-1].SigParam.baseParam.enable == EnableType_DISABLE && pParam->actionParam[i-1].zeroDelayEnable == EnableType_DISABLE)
//					{
//						if(trggerFlag)
//							trggerFlag --;
//					}
////					else if(pParam->actionParam[i-1].SigParam.baseParam.enable == EnableType_ENABLE)
//					{
//						for(j = 0;j<SINGLE_ACTION_TRIGGER_MAX;j++)
//						{
//							for(k = 0;k<TRIGGER_ACTION_MAX;k++)
//							{ 
//								if(pParam->actionParam[i-1].SigParam.tiggerParam[j].action[k].TriggerActionID == id)
//									continue;
//								if(pParam->actionParam[i-1].SigParam.tiggerParam[j].action[k].TriggerActionID == 0)
//									continue;
//								if(pParam->actionParam[i-1].SigParam.tiggerParam[j].action[k].TriggerActionCmd == ActionCtrl_CmdType_STARTUP ||\
//									pParam->actionParam[i-1].SigParam.tiggerParam[j].action[k].TriggerActionCmd == ActionCtrl_CmdType_WAITSTARTUP)
//								{
//									if(pParam->actionParam[i-1].SigParam.tiggerParam[j].cond.TriggerEnable == EnableType_DISABLE||\
//										pParam->actionParam[i-1].SigParam.tiggerParam[j].action[k].TriggerActionEnable == EnableType_DISABLE||\
//										pParam->actionParam[i-1].SigParam.baseParam.enable == EnableType_DISABLE)
//									{	
//										if(trggerFlag)
//											trggerFlag --;
//									}
//								}
//							}
//						}
//					}
//				}
				trggerFlag = 0;
				for(i=CombActionModule[moduleID].cycle_start; i<=CombActionModule[moduleID].cycle_end;i++)
				{
					id = pParam->actionParam[i-1].SigParam.baseParam.ID;
					if(id ==0)
						continue;
					
					getSigActionTriggerFlag_comb(id-1,&triggerState);
					if(triggerState == 1)
						trggerFlag++;
					if(getSingleActionState(id-1,&typetmp))
					{
						if(typetmp == RunStateType_IDLE)
						{
							getSigActionFin_comb(pParam->baseParam.ID,id,&finFlagTemp);							
							if(finFlagTemp>1 && triggerState == 1)
							{
								finFlag++;
							}
						}
					}
//					getSigActionFin_comb(pParam->baseParam.ID,id,&finFlagTemp);
//					if((pParam->actionParam[i-1].SigParam.baseParam.enable == EnableType_DISABLE)||(finFlagTemp>1))
//						finFlag++;
				}
				
				if(finFlag == trggerFlag)  //第一轮循环结束
				{
					CombActionModule[moduleID].cycle_cnt ++;
					if(CombActionModule[moduleID].cycle_cnt >= pParam->baseParam.cycleCnt)
						cycleStepFlag=1;
					else
					{
						for(i=CombActionModule[moduleID].cycle_start; i<=CombActionModule[moduleID].cycle_end;i++)
						{
							id = pParam->actionParam[i-1].SigParam.baseParam.ID;
							if(id ==0)
							{
								continue;
							}
							clearSigActionFin_comb(pParam->baseParam.ID,id);
							CombActionModule[moduleID].delayFin[i-1]=0;
						}
						
						ctrl.CtrlCmd = ActionCtrl_CmdType_WAITSTARTUP;
						ctrl.CtrlMode = pState->ActionCtrl.CtrlMode; //ActionCtrl_ModeType_AUTO;
						ctrl.CtrlSourceType = pState->ActionCtrl.CtrlSourceType; //ActionCtrl_SourceType_CONTROLLER;
						ctrl.CtrlSourceAddr = pState->ActionCtrl.CtrlSourceAddr;
						ctrl.UserID =0;
				
						id = pParam->actionParam[CombActionModule[moduleID].cycle_start-1].SigParam.baseParam.ID;
						if(id ==0)
						{
							pState->RunState = RunStateType_IDLE;
							break;
						}
						SigActionModuleLoad(id-1,&(pParam->actionParam[CombActionModule[moduleID].cycle_start-1].SigParam),0);
						SigActionCtrlFunc(ctrl,id-1,pParam->baseParam.ID);
					}

					if(cycleStepFlag)  //第一轮循环结束
					{						
						ctrl.CtrlCmd = ActionCtrl_CmdType_WAITSTARTUP;
						ctrl.CtrlMode = pState->ActionCtrl.CtrlMode; //ActionCtrl_ModeType_AUTO;
						ctrl.CtrlSourceType = pState->ActionCtrl.CtrlSourceType; //ActionCtrl_SourceType_CONTROLLER;
						ctrl.CtrlSourceAddr = pState->ActionCtrl.CtrlSourceAddr;
						ctrl.UserID =0;
				
						idtemp =CombActionModule[moduleID].cycle_end;						
						if((pParam->baseParam.cycleEnd2 >= pParam->baseParam.cycleStart2)
							&&(pParam->baseParam.cycleCnt !=0)&&(pParam->baseParam.cycleStart2!=0)) //存在第二轮循环  
						{
							CombActionModule[moduleID].cycle_start = pParam->baseParam.cycleStart2;
							CombActionModule[moduleID].cycle_end = pParam->baseParam.cycleEnd2;
							CombActionModule[moduleID].cycle_cnt = 0;
							cycleStepFlag = 0;
						}
						else
						{
							CombActionModule[moduleID].cycle_start = 0;
							CombActionModule[moduleID].cycle_end = 0;
						}
						
						if(idtemp >= COMBACTION_SINGLE_NUM)
						{
							pState->RunState = RunStateType_STOP;
							break;
						}
						
						id = pParam->actionParam[idtemp].SigParam.baseParam.ID;
						if(id ==0)
						{
							pState->RunState = RunStateType_STOP;
							break;
						}
						SigActionModuleLoad(id-1,&(pParam->actionParam[idtemp].SigParam),0);
						SigActionCtrlFunc(ctrl,id-1,pParam->baseParam.ID);						
					}
				}					
			}
			else  //无循环或循环从第一个开始整体循环
			{
				for(i=0; i<COMBACTION_SINGLE_NUM;i++)
				{
					finFlag = finFlagTotal;
					id = pParam->actionParam[i].SigParam.baseParam.ID;
					if(id ==0)
						continue;
					if(getSingleActionState(id-1,&typetmp))
					{
						if(typetmp == RunStateType_IDLE)
						{				
							continue;
						}
						else
						{
							finFlag = finFlagTotal+1;
							break;
						}
					}
//					getSigActionFin_comb(pParam->baseParam.ID,id,&finFlagTemp);
//					if(/*(pParam->actionParam[i].SigParam.baseParam.enable == EnableType_DISABLE)||*/(finFlagTemp>1))
//						finFlag++;
//					if(pParam->actionParam[i].SigParam.baseParam.enable == EnableType_ENABLE)
//					{
//							finFlagTotal++;
//					}
				}
				
				if(finFlag == finFlagTotal)
				{
					CombActionModule[moduleID].cycle_cnt ++;
					if(CombActionModule[moduleID].cycle_cnt >= pParam->baseParam.cycleCnt)
						pState->RunState = RunStateType_STOP;
					else
					{
						pState->RunState = RunStateType_STARTUP;
					}
					
					for(i=0; i<COMBACTION_SINGLE_NUM;i++)
					{
						id = pParam->actionParam[i].SigParam.baseParam.ID;
						if(id ==0)
						{
							continue;
						}
						clearSigActionFin_comb(pParam->baseParam.ID,id);
						CombActionModule[moduleID].delayFin[i]=0;
					}					
				}
			}
			
			for(i=0; i<COMBACTION_SINGLE_NUM;i++)
			{
				id = pParam->actionParam[i].SigParam.baseParam.ID;
				if(id ==0)
					continue;
				
				getSigActionFin_comb(pParam->baseParam.ID,id-1,&CombActionModule[moduleID].runFin[i]);
			}
		}
		break;
		
		case RunStateType_SUSPEND:
		{
			TIME_TRACE
			printk("comb %d suspend \r\n", moduleID+1);
			CombActionModule[moduleID].suspend_flag =1;
			
			for(i=0; i<COMBACTION_SINGLE_NUM;i++)
			{
				if(pParam->actionParam[i].SigParam.baseParam.enable == EnableType_ENABLE)
				{									
					ctrl.CtrlCmd = ActionCtrl_CmdType_SUSPEND;
					ctrl.CtrlMode = pState->ActionCtrl.CtrlMode; //ActionCtrl_ModeType_AUTO;
					ctrl.CtrlSourceType = pState->ActionCtrl.CtrlSourceType; //ActionCtrl_SourceType_CONTROLLER;
					ctrl.CtrlSourceAddr = pState->ActionCtrl.CtrlSourceAddr;
					ctrl.UserID =0;
			
					id = pParam->actionParam[i].SigParam.baseParam.ID;
					if(id ==0)
					{
						continue;
					}
					SigActionModuleLoad(id-1,&(pParam->actionParam[i].SigParam),0);
					SigActionCtrlFunc(ctrl,id-1,pParam->baseParam.ID);
				}
			}
			
			loginfo[0]=pState->ActionCtrl.CtrlSourceAddr;
			loginfo[1]=pState->ActionCtrl.CtrlSourceType;
			memcpy(&loginfo[2],&pState->ActionCtrl.UserID,2);
			loginfo[4]=pState->ActionCtrl.CtrlMode;
			WriteLog_API(moduleID +1,LOG_TYPE_HANDLE_COMB,LOG_STATUS_SUSPEND,loginfo,5);
		}
		break;
		
		case RunStateType_STOP:
		{
			pState->RunState = RunStateType_IDLE;
			TIME_TRACE
			printk("comb %d stop \r\n", moduleID+1);
			//if(pState->ActionCtrl.CtrlSourceAddr != getSelfAddr())
				AddActionInfo(ENUM_COMBINATION_ACTION,moduleID+1,RunStateType_IDLE);
			
			for(i=0; i<COMBACTION_SINGLE_NUM;i++)
			{
				if(pParam->actionParam[i].SigParam.baseParam.enable == EnableType_ENABLE)
				{									
					ctrl.CtrlCmd = ActionCtrl_CmdType_STOP;
					ctrl.CtrlMode = pState->ActionCtrl.CtrlMode; //ActionCtrl_ModeType_AUTO;
					ctrl.CtrlSourceType = pState->ActionCtrl.CtrlSourceType; //ActionCtrl_SourceType_CONTROLLER;
					ctrl.CtrlSourceAddr = pState->ActionCtrl.CtrlSourceAddr;
					ctrl.UserID =0;
			
					id = pParam->actionParam[i].SigParam.baseParam.ID;
					if(id ==0)
					{
						continue;
					}

					clearSigActionFin_comb(pParam->baseParam.ID,id);
					SigActionModuleLoad(id-1,&(pParam->actionParam[i].SigParam),0);
					SigActionCtrlFunc_stop(ctrl,id-1,pParam->baseParam.ID);
				}
			}
			
			loginfo[0]=pState->ActionCtrl.CtrlSourceAddr;
			loginfo[1]=pState->ActionCtrl.CtrlSourceType;
			memcpy(&loginfo[2],&pState->ActionCtrl.UserID,2);
			loginfo[4]=pState->ActionCtrl.CtrlMode;
			WriteLog_API(moduleID +1,LOG_TYPE_HANDLE_COMB,LOG_STATUS_STOP,loginfo,5);
		}
		break;
		
		default:
			break;
	}
	if(CombActionModule[moduleID].suspend_flag ==0)
		pState->RunStateBak = runState;
	
	return 1;
}

u8 CombActionCtrlFunc(ActionCtrlType ctrl,u8 actionID)
{
	CombActionParamType *pParam = CombActionModule[actionID].pParam;
	CombActionStateType *pState = CombActionModule[actionID].pState;
	
	if((pState == NULL)||(pParam == NULL))
		return 0;
	if(pParam->baseParam.enable == EnableType_DISABLE)
		return 0;	
	
	if(ctrl.CtrlMode == ActionCtrl_ModeType_MANUAL)
		return 0;
	
	CombActionAutoCtrlFunc(ctrl,pParam,pState);
	return 1;
}

u8 CombActionAllStop(u8 actionID)
{
	CombActionParamType *pParam = CombActionModule[actionID].pParam;
	CombActionStateType *pState = CombActionModule[actionID].pState;
	
	if((pState == NULL)||(pParam == NULL))
		return 0;
	if(pParam->baseParam.enable == EnableType_DISABLE)
		return 0;	
	
	if(pState->RunState != RunStateType_IDLE)
		pState->RunState = RunStateType_STOP;
	return 1;
}

u8 CombActionPerformJudge(ActionCtrlType ctrl,u8 actionID)
{
	CombActionParamType *pParam = CombActionModule[actionID].pParam;
	
	if(pParam == NULL)
		return 0;
	if(pParam->baseParam.enable == EnableType_DISABLE)
		return 0;

	if((pParam->baseParam.controlMode ==ControlModeType_LocalRemote)||
	  ((ctrl.CtrlSourceType == ActionCtrl_SourceType_CONTROLLER) &&(ctrl.CtrlSourceAddr !=getSelfAddr() ))||
	  ((pParam->baseParam.controlMode ==ControlModeType_Local)&&(ctrl.CtrlSourceType == ActionCtrl_SourceType_CONTROLLER)
	    &&(ctrl.CtrlSourceAddr ==getSelfAddr() ))||
	   ((pParam->baseParam.controlMode ==ControlModeType_Remote)&&(ctrl.CtrlSourceType != ActionCtrl_SourceType_CONTROLLER)))
	{
		return 1;
	}
	return 0;	
}


u8 checkCombActionIsMutexSingleAction(u8 moduleID1,u8 moduleID2)
{
	u8 i,j;
	u8 res = 0;
	CombActionParamType *pParam1 = CombActionModule[moduleID1].pParam;
	CombActionParamType *pParam2 = CombActionModule[moduleID2].pParam;
	
	for(i = 0;i<COMBACTION_SINGLE_NUM;i++)
	{
		for(j = 0;j<COMBACTION_SINGLE_NUM;j++)
		{
			if(pParam1->actionParam[i].SigParam.baseParam.ID == pParam2->actionParam[j].SigParam.actionParam.mutexActionID1 ||
				pParam1->actionParam[i].SigParam.baseParam.ID == pParam2->actionParam[j].SigParam.actionParam.mutexActionID2)
			{
				res = 1;
				break;
			}
		}
	}
}


u8 checkCombActionPriorityState(u8 moduleID)
{
	u8 i;
	u8 res = 1;
	CombActionParamType *pParam = CombActionModule[moduleID].pParam;
	if(pParam->baseParam.actionPriority == ActionPriority_Low)
	{
		for(i = 0;i < COMBACTION_NUM_MAX; i++)
		{
			if(CombActionModule[i].pState->RunState == RunStateType_IDLE || CombActionModule[i].pState->RunState == RunStateType_STOP)
				continue;
			else
			{
				if(checkCombActionIsMutexSingleAction(i,moduleID))
				{
					res = 0;
					break;
				}
				else
				{
					;
				}
			}
		}
	}
	else if(pParam->baseParam.actionPriority == ActionPriority_Middle)
	{
		for(i = 0;i < COMBACTION_NUM_MAX; i++)
		{
			if(CombActionModule[i].pState->RunState == RunStateType_IDLE || CombActionModule[i].pState->RunState == RunStateType_STOP)
				continue;
			else
			{
				if(CombActionModule[i].pParam->baseParam.actionPriority == ActionPriority_Low)
				{
					if(checkCombActionIsMutexSingleAction(i,moduleID))
						CombActionAllStop(i);
				}
				else if(CombActionModule[i].pParam->baseParam.actionPriority == ActionPriority_Middle)
				{
					;
				}
				break;
			}
		}
	}
	return res;
}

#endif
