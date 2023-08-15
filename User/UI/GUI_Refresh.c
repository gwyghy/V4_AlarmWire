/*
*********************************************************************************************************
*	                                            DoubleWires
*                           Commuication Controller For The Mining Industry
*
*                                  (c) Copyright 1994-2021  HNDZ 
*                       All rights reserved.  Protected by international copyright laws.
*
*
*    File    : 
*    Module  : user
*    Version : V1.0
*    History :
*   -----------------
*              Version  Date           By            Note
*              v1.0     2021-10   
*
*********************************************************************************************************
*/	
#include "ProjectConfig.h"
#include "User_GUI.h"
#include "sys_msg_def.h"
/*
*********************************************************************************************************
*	                                     宏定义
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*	                                    全局变量
*********************************************************************************************************
*/
extern WM_HTIMER 	ShearerLeft_hTimer,
			ShearerRight_hTimer,
			DtrectionLeft_hTimer,
			DtrectionRight_hTimer;
extern WM_HWIN  ShearerLeft_hWin,
		ShearerRight_hWin,
		DtrectionLeft_hWin,
		DtrectionRight_hWin;




extern WM_HWIN SuddenStop_hWin,Warningtwo_hWin;
extern NameAndId   ShowAuxiliaryAction[AuxiliaryActionMAX];
extern SensorShow   ShowSensorShow[SensorMAX];


ACTOIN_INFO  action_info;
void Info_Action_Init(void)
{
	//故障
	uint8_t i = 0;
	//故障
	action_info.action_info_count = 0;
	memset(action_info.action_info_queue, 0 ,sizeof(action_info.action_info_queue));
	
	action_info.pUsed = NULL;
	action_info.pFree =&action_info.action_info_queue[0];
	for(i=0 ; i<ACTION_INFO_SIZE - 1; i++)
	{
		action_info.action_info_queue[i].pNext = &action_info.action_info_queue[i+1];
	}	
	action_info.action_info_queue[ACTION_INFO_SIZE-1].pNext = NULL;
}
//查询故障数组
stActionInfo * checkActionInfoQueue(NameAndId *actionInfo)
{
	stActionInfo* resactionInfo = NULL;
	stActionInfo* tmpactionInfo = action_info.pUsed;
	
	if(action_info.action_info_count == 0)
		return resactionInfo;
	do
	{
		if(tmpactionInfo->actionInfo.id == actionInfo->id)
		{
			resactionInfo = tmpactionInfo;
			break;
		}
		tmpactionInfo = tmpactionInfo->pNext;
	}while(tmpactionInfo != NULL);

	return resactionInfo;
}
 void InsActionInfoQueue(NameAndId *actionInfo)
{
	stActionInfo * tmpInfo;
	if(actionInfo->id == 0)
	{
		return;
	}
	if(action_info.action_info_count <ACTION_INFO_SIZE)
	{
		memcpy(&(action_info.pFree->actionInfo),actionInfo,sizeof(NameAndId));
		tmpInfo = action_info.pFree;
		action_info.pFree = action_info.pFree->pNext;
		
		tmpInfo->pNext =  action_info.pUsed;
		action_info.pUsed = tmpInfo;
		action_info.action_info_count ++;
	}
}
//删除故障数组
 void DelActionInfoQueue(NameAndId *actionInfo)
{

	stActionInfo* delactionInfo = NULL;
	stActionInfo* tmpactionInfo = action_info.pUsed;
	delactionInfo = checkActionInfoQueue(actionInfo);
	if(actionInfo->id == 0)
	{
		return;
	}
	if(delactionInfo != NULL)
	{	
		if(delactionInfo == tmpactionInfo)
		{
			action_info.pUsed = action_info.pUsed->pNext;
			action_info.action_info_count--;
			tmpactionInfo->pNext = action_info.pFree;
			action_info.pFree = tmpactionInfo;
		}
		else
		{
			do
			{
				if(tmpactionInfo->pNext == delactionInfo)
				{
					tmpactionInfo->pNext = delactionInfo->pNext;
					delactionInfo->pNext = action_info.pFree;
					action_info.pFree = delactionInfo;
					action_info.action_info_count--;
					break;
				}
				tmpactionInfo = tmpactionInfo->pNext;
			}while(tmpactionInfo != NULL);
		}
	}
}
//读故障数组对外api
uint8_t ReadAction_API(NameAndId *info)
{
	uint8_t i = 0;
	uint8_t index = 0;
	stActionInfo* tmpactionInfo = action_info.pUsed;

	for(i = 0; i<6; i++)
	{
		if(tmpactionInfo == NULL)
			break;
		memcpy(info,&(tmpactionInfo->actionInfo),sizeof(NameAndId));
		tmpactionInfo = tmpactionInfo->pNext;
		info++;
		index++;
	}
	return index;
}
/*
*********************************************************************************************************
*	函 数 名: 
*	功能说明: 判断返回主界面
*	形    参: 
*	返 回 值: 
*********************************************************************************************************
*/
void SupporthomeRefresh(WM_MESSAGE * pMsg)
{
	static uint8_t lastdata;
	uint8_t length;
	globalStateType  temp;
	GUIGetDateFunAPI(API_Support_Slave,0,&temp,&length);
	
	if(temp.slaveMode == 0)
	{
		lastdata++;
		if(lastdata > 2)
		{
			lastdata = 0;
			GUI_EndDialog(pMsg->hWin, 0);
			CreateHomeFrame();
		}
	}
	else
	{
		lastdata = 0;
	}
}
/*
*********************************************************************************************************
*	函 数 名: 
*	功能说明: 刷新急停闭锁状态
*	形    参: 
*	返 回 值: 
*********************************************************************************************************
*/
void SupportStopRefresh(WM_MESSAGE * pMsg)
{
	uint8_t length,pdata[2];
	globalStateType  temp;
	char buf[10];
	GUIGetDateFunAPI(API_Support_Statur,0,&temp,&length);
	
	if(temp.hwEmergency == 1)
	{
		WM_HideWindow(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT25));
		WM_HideWindow(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT26));
		WM_ShowWindow(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT21));
		WM_ShowWindow(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT22));
		WM_ShowWindow(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT23));
		WM_ShowWindow(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT24));
		SysDataFun(DATA_MSG_GetStopInfo,0,0,0,0,pdata,(uint16_t*)&length);
		sprintf(buf,"%d",pdata[0]);
		TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT21),"本架");
		TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT22),"急停");
		TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT23),buf);		
	}
	else if(temp.emergencyState == 1)
	{
		WM_HideWindow(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT25));
		WM_HideWindow(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT26));
		WM_ShowWindow(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT21));
		WM_ShowWindow(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT22));
		WM_ShowWindow(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT23));
		WM_ShowWindow(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT24));
		SysDataFun(DATA_MSG_GetStopInfo,0,0,0,0,pdata,(uint16_t*)&length);
		sprintf(buf,"%d",pdata[0]);
		TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT21),"急停");
		TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT22),"位置");
		TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT23),buf);	
	}
	else if(temp.hwBisuo == 1)
	{
		WM_ShowWindow(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT25));
		WM_ShowWindow(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT26));
		WM_HideWindow(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT21));
		WM_HideWindow(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT22));
		WM_HideWindow(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT23));
		WM_HideWindow(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT24));
		TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT25),"本架");
		TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT26),"闭锁");	
	}
	else if(temp.bisuoState == 1)
	{
		WM_ShowWindow(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT25));
		WM_ShowWindow(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT26));
		WM_HideWindow(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT21));
		WM_HideWindow(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT22));
		WM_HideWindow(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT23));
		WM_HideWindow(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT24));
		TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT25),"软件");
		TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT26),"闭锁");		
	}
	else if(temp.hwStop == 1)
	{
		WM_ShowWindow(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT25));
		WM_ShowWindow(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT26));
		WM_HideWindow(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT21));
		WM_HideWindow(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT22));
		WM_HideWindow(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT23));
		WM_HideWindow(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT24));
		TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT25),"本架");
		TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT26),"停止");		
	}
	else if(temp.stopState == 1)
	{
		WM_HideWindow(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT25));
		WM_HideWindow(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT26));
		WM_ShowWindow(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT21));
		WM_ShowWindow(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT22));
		WM_ShowWindow(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT23));
		WM_ShowWindow(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT24));
		SysDataFun(DATA_MSG_GetStopInfo,0,0,0,0,pdata,(uint16_t*)&length);
		memset(buf,0,sizeof(buf));
		sprintf(buf,"%d",pdata[1]);
		if(pdata[1] == 0)
		{
			memset(buf,0,sizeof(buf));
			return;
		}
		TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT21),"停止");
		TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT22),"位置");
		TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT23),buf);
	}
	else if(temp.PersonLockState == 1)
	{
		WM_ShowWindow(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT25));
		WM_ShowWindow(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT26));
		WM_HideWindow(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT21));
		WM_HideWindow(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT22));
		WM_HideWindow(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT23));
		WM_HideWindow(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT24));
		TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT25),"人员");
		TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT26),"闭锁");		
	}
	else
		;
}

uint8_t SupportStatusdata;
extern uint8_t MoveFramedata;

/*
*********************************************************************************************************
*	函 数 名: 
*	功能说明: 支架状态刷新
*	形    参: 
*	返 回 值: 
*********************************************************************************************************
*/
//extern u8 texxxxxxxxxxxxxxt;

void StatusRefresh(WM_MESSAGE * pMsg,uint8_t initState)
{
	char  buf[10];
	uint8_t length;
	static uint8_t runMode = 3,OperationNumber = 0,ShearersLocation = 0,MineStopStatus = 0;
	globalStateType  temp;

	
//	if(texxxxxxxxxxxxxxt == 10)
//	{
//		TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT12),"XX");
//		TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT13),"XX");
//		return;
//	}
	
	
	
	GUIGetDateFunAPI(API_Support_Statur,0,&temp,&length);
	if(temp.runMode != runMode || initState == 0)
	{
		if(temp.runMode == SUPPORTSTATE_FREE)
		{
			TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT12),"支架");
			TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT13),"空闲");			
		}
		else if(temp.runMode == SUPPORTSTATE_FOLLOW)
		{
			TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT12),"支架");
			TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT13),"跟机");			
		}
		else if(temp.runMode == SUPPORTSTATE_FOLLOW_STOP)
		{
			TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT12),"暂停");
			TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT13),"跟机");			
		}
		else
			;
	}
	if(temp.OperationNumber != OperationNumber || initState == 0 || temp.runMode != runMode)
	{
		if((temp.runMode == SUPPORTSTATE_FREE) || (temp.runMode == SUPPORTSTATE_FOLLOW_STOP))
		{
			TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT11)," ");
		}
		else if(temp.runMode == SUPPORTSTATE_FOLLOW)
		{
			memset(buf,0,sizeof(buf));
			sprintf(buf,"工序%d",temp.OperationNumber);//gongxu hao
			TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT11),buf);		
		}
	}
	
	if((temp.ShearersLocation != ShearersLocation) || initState == 0 || (temp.MineStopStatus != MineStopStatus))
	{
		memset(buf,0,sizeof(buf));
		if(temp.MineStopStatus)
			sprintf(buf,"%d!",temp.ShearersLocation);//meiji weizhi
		else
			sprintf(buf,"%d",temp.ShearersLocation);//meiji weizhi
		TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT10),buf);
	}
	
	runMode = temp.runMode;
	OperationNumber = temp.OperationNumber;
	ShearersLocation = temp.ShearersLocation;	
	MineStopStatus = temp.MineStopStatus;	
}
uint8_t PersonStatusdata = 0;
/*
*********************************************************************************************************
*	函 数 名: 
*	功能说明: 刷新人员闭锁信息
*	形    参: 
*	返 回 值: 
*********************************************************************************************************
*/
void PersonLockStatusRefresh(WM_HWIN  pMsg)
{
	uint8_t length,time;
	char buf[10];	
	globalStateType  temp;
	
	GUIGetDateFunAPI(API_Support_Statur,0,&temp,&length);
	if((temp.PersonLockState == 1) && PersonStatusdata ==0)
	{
		PersonStatusdata = 1;//已经创建
		CreateWarningtwo(pMsg);
		
		TEXT_CreateEx(115-60,105,125,44,pMsg,WM_CF_SHOW,GUI_TA_RIGHT|TEXT_CF_VCENTER,GUI_ID_TEXT27," ");	
		TEXT_CreateEx(240-60,105,125,44,pMsg,WM_CF_SHOW,GUI_TA_LEFT|TEXT_CF_VCENTER,GUI_ID_TEXT28," ");
		TEXT_CreateEx(70,105,44,44,pMsg,WM_CF_SHOW,TEXT_CF_HCENTER|TEXT_CF_VCENTER,GUI_ID_TEXT29," ");
//		TEXT_CreateEx(0,105,360,50,pMsg,WM_CF_SHOW,TEXT_CF_HCENTER|TEXT_CF_VCENTER,GUI_ID_TEXT27," ");
		TEXT_SetTextColor(WM_GetDialogItem(pMsg, GUI_ID_TEXT27),GUI_WHITE);
		TEXT_SetFont(WM_GetDialogItem(pMsg, GUI_ID_TEXT27), &GUI_FontHZ40x40);
		TEXT_SetTextColor(WM_GetDialogItem(pMsg, GUI_ID_TEXT28),GUI_WHITE);
		TEXT_SetFont(WM_GetDialogItem(pMsg, GUI_ID_TEXT28), &GUI_FontHZ40x40);
		TEXT_SetTextColor(WM_GetDialogItem(pMsg, GUI_ID_TEXT29),GUI_WHITE);
		TEXT_SetFont(WM_GetDialogItem(pMsg, GUI_ID_TEXT29), &GUI_FontHZ40x40);
		TEXT_SetText(WM_GetDialogItem(pMsg, GUI_ID_TEXT27),"人员");
		TEXT_SetText(WM_GetDialogItem(pMsg, GUI_ID_TEXT28),"闭锁");
	}
	else if((temp.PersonLockState == 0) && PersonStatusdata ==1)
	{
		PersonStatusdata = 0;//已经创建		
		GUI_EndDialog(Warningtwo_hWin, 0);
		WM_DeleteWindow(WM_GetDialogItem(pMsg, GUI_ID_TEXT27));	
		WM_DeleteWindow(WM_GetDialogItem(pMsg, GUI_ID_TEXT28));	
		WM_DeleteWindow(WM_GetDialogItem(pMsg, GUI_ID_TEXT29));	
	}
	else if((temp.PersonLockState == 2) && PersonStatusdata ==1)
	{
		//获取剩余时间
		time = getPersonLockResumeTimer();
		sprintf(buf,"%d",time);
		TEXT_SetText(WM_GetDialogItem(pMsg, GUI_ID_TEXT29),buf);
		TEXT_SetText(WM_GetDialogItem(pMsg, GUI_ID_TEXT27),"秒后");
		TEXT_SetText(WM_GetDialogItem(pMsg, GUI_ID_TEXT28),"启动");
	}
	else
		;
}
/*
*********************************************************************************************************
*	函 数 名: 
*	功能说明: 刷新急停闭锁状态
*	形    参: 
*	返 回 值: 
*********************************************************************************************************
*/
void SupportStatusRefresh(WM_HWIN  pMsg,uint16_t arr)
{
	uint8_t length;
	globalStateType  temp;
	GUIGetDateFunAPI(API_Support_Statur,0,&temp,&length);
	if(((temp.hwEmergency == 1) ||(temp.hwBisuo == 1) ||(temp.hwStop == 1) ||(temp.emergencyState == 1) || (temp.bisuoState == 1) ||(temp.stopState == 1)||(temp.PersonLockState == 1)) && SupportStatusdata ==0)
	{
		SupportStatusdata = 1;//已经创建
		CreateSuddenStop(pMsg);
		if(MoveFramedata ==1)
		{
			WM_DeleteTimer(ShearerRight_hTimer);
			WM_DeleteTimer(DtrectionRight_hTimer);
		}
		else if(MoveFramedata ==2)
		{
			WM_DeleteTimer(ShearerLeft_hTimer);
			WM_DeleteTimer(DtrectionLeft_hTimer);
		}
	}
	else if(((temp.hwEmergency == 0) &&(temp.hwBisuo == 0) &&(temp.hwStop == 0) &&(temp.emergencyState == 0) && (temp.bisuoState == 0) &&(temp.stopState == 0) &&(temp.PersonLockState == 0)) && SupportStatusdata ==1)
	{
		SupportStatusdata = 0;//已经创建		
		GUI_EndDialog(SuddenStop_hWin, 0);
#if (MAIN_INTERFACE_ANIMATION == 1)
		if(MoveFramedata ==1)      
		{
			ShearerRight_hTimer = WM_CreateTimer(WM_GetClientWindow(ShearerRight_hWin), 0, 100, 0);
			DtrectionRight_hTimer = WM_CreateTimer(WM_GetClientWindow(DtrectionRight_hWin), 0, 100, 0);
		}
		else if(MoveFramedata ==2)
		{
			ShearerLeft_hTimer = WM_CreateTimer(WM_GetClientWindow(ShearerLeft_hWin), 0, 100, 0);
			DtrectionLeft_hTimer = WM_CreateTimer(WM_GetClientWindow(DtrectionLeft_hWin), 0, 100, 0);
		}
#endif
	}
	if(arr == 1)
	{
		if((temp.slaveMode == 1) && (temp.hwEmergency == 0) && (temp.hwBisuo == 0) && (temp.hwStop == 0))
		{
			GUI_EndDialog(pMsg, 0);
			CreateSlveContor();
		}	
	}
}
/*
*********************************************************************************************************
*	函 数 名: 
*	功能说明: 支架编号刷新
*	形    参: 
*	返 回 值: 
*********************************************************************************************************
*/
void SupportNumberRefresh(WM_MESSAGE * pMsg,uint8_t initState)
{
	uint8_t length,pdata;
	static uint8_t lastdata = 0;
	char  buf[3];
	GUIGetDateFunAPI(API_Support_Number,0,&pdata,&length);
	if(pdata != lastdata ||initState == 0)
	{
		sprintf(buf, "%d", pdata);
		TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT0),buf);
		lastdata = pdata;		
	}
}
/*
*********************************************************************************************************
*	函 数 名: 
*	功能说明: 刷新当前日期
*	形    参: 
*	返 回 值: 
*********************************************************************************************************
*/
void CurrentDataRefresh(WM_MESSAGE * pMsg)
{
	RTC_TimeData   time;
	char buf[10];
	Get_Data_Time(&time);
	memset(buf,0,sizeof(buf));
	sprintf(buf,"%d.%d",time.sys_data.RTC_Month,time.sys_data.RTC_Date);
	TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT1), buf);
	memset(buf,0,sizeof(buf));
	sprintf(buf,"%d:%02d",time.sys_time.RTC_Hours,time.sys_time.RTC_Minutes);
	TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT2), buf);
}
/*
*********************************************************************************************************
*	函 数 名: 
*	功能说明: 刷新运行方向
*	形    参: 
*	返 回 值: 
*********************************************************************************************************
*/
extern WM_HWIN window;
void RunningDirectionRefresh(WM_MESSAGE * pMsg)
{
	globalStateType  temp;
	uint8_t length,pdata;
	uint16_t transportp[4];
	
	if(SupportStatusdata == 1)
		return;
	GUIGetDateFunAPI(API_Support_Statur,0,&temp,&length);
	pdata = temp.DirectionOperation;

	if((pdata == 0) && (MoveFramedata == 1))
	{
		window = CreateMoveFrame_Left();		
	}
	else if((pdata == 1) && (MoveFramedata == 2))
	{
		window = CreateMoveFrame_Right();
	}
	else
		;	
    //获取是否显示
	GUIGetDateFunAPI(API_Transport_Plane,0,&transportp,&length);
	if((transportp[0] == 0) && (transportp[1] == 0) && (transportp[2] == 0) && (transportp[3] == 0))
	{
		//显示窗口
		WM_ShowWindow (window);
	}
	else
	{
		//隐藏窗口
		WM_HideWindow(window);
	}
}
/*
*********************************************************************************************************
*	函 数 名: 
*	功能说明: 刷新运行方向
*	形    参: 
*	返 回 值: 
*********************************************************************************************************
*/
extern uint16_t transportplane[];
void TransportPlaneRefresh(WM_MESSAGE * pMsg)
{
	char buf[15];
	char buf1[15];
    uint16_t transportp[4];
	uint8_t length;
    //获取是否显示
	GUIGetDateFunAPI(API_Transport_Plane,0,&transportp,&length);

	if(transportp[0] == 1)
	{
		sprintf(buf,"%s","机头1");
		strcat(buf," ");
		if(transportplane[0] != 0)
		{
			sprintf(buf1,"%d",transportplane[0]);
		}
		else
		{
			sprintf(buf1,"%s","***");
		}
		strcat(buf,buf1);
		strcat(buf,"A");
		TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT14),buf);
	}
	else
	{
		TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT14)," ");
	}
	
	if(transportp[1] == 1)
	{
		sprintf(buf,"%s","机头2");
		strcat(buf," ");
		if(transportplane[1] != 0)
		{
			sprintf(buf1,"%d",transportplane[1]);
		}
		else
		{
			sprintf(buf1,"%s","***");
		}
		strcat(buf,buf1);
		strcat(buf,"A");
		TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT15),buf);
	}
	else
	{
		TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT15)," ");
	}
	
	if(transportp[2] == 1)
	{
		sprintf(buf,"%s","机尾1");
		strcat(buf," ");
		if(transportplane[2] != 0)
		{
			sprintf(buf1,"%d",transportplane[2]);
		}
		else
		{
			sprintf(buf1,"%s","***");
		}
		strcat(buf,buf1);
		strcat(buf,"A");
		TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT16),buf);
	}
	else
	{
		TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT16)," ");
	}

	if(transportp[3] == 1)
	{
		sprintf(buf,"%s","机尾2");
		strcat(buf," ");
		if(transportplane[3] != 0)
		{
			sprintf(buf1,"%d",transportplane[3]);
		}
		else
		{
			sprintf(buf1,"%s","***");
		}
		strcat(buf,buf1);
		strcat(buf,"A");
		TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT17),buf);
	}
	else
	{
		TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT17)," ");
	}

}
/*
*********************************************************************************************************
*	函 数 名: 
*	功能说明: 刷新显示项1
*	形    参: 
*	返 回 值: 
*********************************************************************************************************
*/
//extern uint16_t port4State;
void DisplayItemRefresh(WM_MESSAGE * pMsg ,uint8_t initState)
{
	uint8_t length,i;
	static  char buf[15];
	static uint16_t sensorValueLast[6] = {0};
	static uint8_t timer[6] = {0};
	char buf1[15];
	SensorStateType SensorState;

	
	for(i=0;i<6;i++)
	{

		if(ShowSensorShow[i].id == 0)
		{
			TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT4+i)," ");	
			continue;
		}
		
		timer[i]++;
		memcpy(buf,ShowSensorShow[i].nameshow,GUI__strlen(ShowSensorShow[i].nameshow));
		strcat(buf," ");
		GUIGetDateFunAPI(API_Display_Item,ShowSensorShow[i].id,&SensorState,&length);
		if((SensorState.state_online == FailType_NORMAL) || (SensorState.state_fail == OnlineType_ONLINE))
		{
			sprintf(buf1,"***");	
		}
		else
		{
			if(abs(sensorValueLast[i]-SensorState.calculateValue)>4 || initState == 0 || timer[i]>= 10)    //
			{
				timer[i] = 0;
				sensorValueLast[i] = SensorState.calculateValue;
			}
			else
			{
				memset(buf,0,sizeof(buf));
				continue;
			}
			if(ShowSensorShow[i].acc == 3)
				sprintf(buf1,"%d.%d",SensorState.calculateValue/10,SensorState.calculateValue%10);
			else if(ShowSensorShow[i].acc == 4)
				sprintf(buf1,"%d.%02d",SensorState.calculateValue/100,SensorState.calculateValue%100);
			else if(ShowSensorShow[i].acc == 9)
				sprintf(buf1,"%d.%d",(int16_t)SensorState.calculateValue/10,SensorState.calculateValue%10);
			else
				sprintf(buf1,"%d",SensorState.calculateValue);
		}
		strcat(buf,buf1);
		AddUnitDispiay(buf,ShowSensorShow[i].Nuit);
		TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT4+i),buf);	
		memset(buf,0,sizeof(buf));
	}
}
/*
*********************************************************************************************************
*	函 数 名: 
*	功能说明: 刷新成组界面
*	形    参: 
*	返 回 值: 
*********************************************************************************************************
*/
void GroupRefresh(WM_MESSAGE * pMsg,NameAndId * pGroupAction,uint16_t Direction,GroupStartUp * pdata)
{
	char  showbuf[15];
	uint8_t length;
	uint8_t number,direction,numberMAX,numberMIN;
	int SrartNumber,EndNumber;
	uint8_t SafeInte,ScopExec;
	groupActionParamType  grouptemp;
	GUIGetDateFunAPI(API_Support_Number,0,&number,&length);	//本架编号
	GUIGetDateFunAPI(API_Number_Increase,0,&direction,&length);	//架号增向
	GUIGetDateFunAPI(API_Number_MAX,0,&numberMAX,&length);	//最大架号
	GUIGetDateFunAPI(API_Number_MIN,0,&numberMIN,&length);	//最小架号
	
	GUIGetDateFunAPI(API_Group_Action,pGroupAction->id,&grouptemp,&length);
	SafeInte = grouptemp.actionParam.safeGap;
	ScopExec = grouptemp.actionParam.scope;
	//架号增向与成组方向一致，起始架=本架架号+安全间隔；结束架=本架架号+安全间隔+执行范围
	if(Direction == direction)
	{
		SrartNumber = number+SafeInte+1;
		EndNumber = number+SafeInte+ScopExec;
		if(SrartNumber == numberMAX)
		{
			EndNumber = SrartNumber;
		}
		else if(SrartNumber > numberMAX)
		{
			SrartNumber = 0;
			EndNumber = 0;
		}
		else
		{
			if(EndNumber >= numberMAX)
				EndNumber = numberMAX;
		}
	}	
	//架号增向与程序方向相反，起始架=本架架号-安全间隔；结束架=本架架号-安全间隔-执行范围
	else
	{
		SrartNumber = number-SafeInte-1;
		EndNumber = number-SafeInte-ScopExec;
		if(SrartNumber == numberMIN)
		{
			EndNumber = SrartNumber;
		}
		else if(SrartNumber < numberMIN)
		{
			SrartNumber = 0;
			EndNumber = 0;
		}
		else
		{
			if(EndNumber <= numberMIN)
				EndNumber = numberMIN;
		}
		
	}
	pdata->GroupID = pGroupAction->id;
	pdata->ActionType = Group;
	pdata->SrartNumber = SrartNumber;
	pdata->EndNumber = EndNumber;
	memset(showbuf, 0x00  , sizeof(showbuf));
	if(pGroupAction->name == 0)
	{
		sprintf(showbuf, "%s","无动作");
	}
	else
	{
		sprintf(showbuf, "成组%s", pGroupAction->name);
	}
	TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT32),showbuf);
	memset(showbuf, 0x00  , sizeof(showbuf));
	if(Direction == 1)
	{
		sprintf(showbuf, "%d", SrartNumber);
		TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT33),showbuf);
		memset(showbuf, 0x00  , sizeof(showbuf));
		sprintf(showbuf, "%d", EndNumber);
		TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT34),showbuf);
	}
	if(Direction == 0)
	{
		sprintf(showbuf, "%d", SrartNumber);
		TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT34),showbuf);
		memset(showbuf, 0x00  , sizeof(showbuf));
		sprintf(showbuf, "%d", EndNumber);
		TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT33),showbuf);
	}
}
/*
*********************************************************************************************************
*	函 数 名: 
*	功能说明: 刷新成组界面结束架
*	形    参: 
*	返 回 值: 
*********************************************************************************************************
*/
void GroupAddRefresh(WM_MESSAGE * pMsg,GroupStartUp * pdata)
{
	char  showbuf[15];
	uint16_t length;
	uint16_t number,direction;
	uint16_t EndNumber;
	GUIGetDateFunAPI(API_Support_Number,0,&number,&length);	//本架编号
	GUIGetDateFunAPI(API_Number_Increase,0,&direction,&length);	//架号增向
	
	if(pdata->Direction == direction)
	{
		pdata->EndNumber++;
	}	
	
	else
	{
		pdata->EndNumber--;
	}
	memset(showbuf, 0x00  , sizeof(showbuf));
	sprintf(showbuf, "%d", EndNumber);
	TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT34),showbuf);
}
