//#include "pngdisplay.h"
//#include "bmpdisplay.h"
//#include "EmWinHZFont.h"
//#include "GUI.h"
//#include "malloc.h"
//#include "delay.h"
//#include "ff.h"
//#include "tftlcd.h"
//#include "DIALOG.h"

//#include "led.h"
//#include "pcf8574.h"
/*
*********************************************************************************************************
*	                                  
*	模块名称 : 
*	文件名称 : HomeTask.c
*	版    本 : V1.0
*	说    明 : 实现按键各种功能
*              
*	修改记录 :
*		版本号   日期         作者          说明
*		V1.0    2016-07-16   Eric2013  	    首版    
*                                     
*	Copyright (C), 2015-2020, 安富莱电子 www.armfly.com
*
*********************************************************************************************************
*/	
#include "User_GUI.h"
#include "includes.h"					//ucos 使用	  
#include "usartapp.h"
#include <math.h>
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
extern NameAndId   ShowAuxiliaryAction[];
extern WM_HWIN BackGround_hWin;
extern NameAndId  ShowKeyboardAction[][2];

extern GUI_MEMDEV_Handle  	BackGround_DEV,
					Button_DEV,
					Signal_DEV1,
					Signal_DEV2,
					Signal_DEV3,
					TranSport_DEV ,
					DtrectionLeft_DEV0,
					DtrectionLeft_DEV1,
					DtrectionLeft_DEV2,
					DtrectionLeft_DEV3,
					DtrectionRight_DEV0,
					DtrectionRight_DEV1,
					DtrectionRight_DEV2,
					DtrectionRight_DEV3,
					ShearerLeft_DEV1,
					ShearerLeft_DEV2,
					ShearerRight_DEV1,
					ShearerRight_DEV2,
					SuddenStop_DEV,
					PowerOn_DEV,
					PassWord_DEV[2],
					HomeFrame_DEV[5],
					Group_DEV[5];
					
WM_HTIMER 	MasterContor_hTimer;
			
			
WM_HWIN 	MasterContor_hWin;
		

int targer,targerold;


u8 restartAlarmFlag = 0;
void SlaveControlRestartAlarm(void)
{
	u8 temp[2] = {0};
	u16 length;
	u8 i = 0;
	u8 stateStart = 0;
	u8 stateEnd = 0;

	if(targer == 0)
	{
		return;
	}
	if(targer>(SysParam.globalParam.SupportParam.StopScope+SysParam.globalParam.SupportParam.MinSupportID))
	{
		stateStart = targer - SysParam.globalParam.SupportParam.StopScope;
	}
	else
	{
		stateStart = SysParam.globalParam.SupportParam.MinSupportID;
	}
	
	if((targer+SysParam.globalParam.SupportParam.StopScope)<SysParam.globalParam.SupportParam.MaxSupportID)
	{
		stateEnd = targer+SysParam.globalParam.SupportParam.StopScope;
	}
	else
	{
		stateEnd = SysParam.globalParam.SupportParam.MaxSupportID;
	}
		
	for(i = stateStart;i<=stateEnd;i++)
	{
		if(SysStopState[i].SYSStop.STOP == 1)
		{
			restartAlarmFlag = 1;
			return;
		}
	}
	for(i = (targer-1);i<=(targer+1);i++)
	{
		if(i<SysParam.globalParam.SupportParam.MinSupportID)
			continue;
		if(i>SysParam.globalParam.SupportParam.MaxSupportID)
			continue;
		if(SysStopState[i].SYSStop.BISUO == 1)
		{
			restartAlarmFlag = 1;
			return;
		}
	}
	if(restartAlarmFlag == 1)
	{
		restartAlarmFlag = 2;
		return;
	}
	if(restartAlarmFlag == 2)
	{
		restartAlarmFlag = 0;
		//发新的预警
		temp[0] = 0;
		temp[1] = targer;				
		SysDataFun(DATA_MSG_CTRL_ALARM,0,0,2,0,&temp[0],0);
	}
}

static void MasterContor_cbDialog(WM_MESSAGE * pMsg) 
{
	static uint8_t sige[12] = {0};
	static uint8_t sige1[12] = {0};
	uint16_t i,j;
	static uint16_t k;
	uint16_t length;
	static int beepsta;
	uint8_t pstate = 1;
	uint8_t number,direction,range,numberMAX,numberMIN;
	static NameAndId showbuf[6];
	char buf[10];
	char * sensorbuf;
	char singleActionAndSensor[20];
	uint8_t sensorRes;
	uint8_t temp[2];
	static  SingleStartUp  TempActoin,CombinedActoin;
	switch (pMsg->MsgId) 
	{
		case WM_CREATE:
			SetSysFuncMode(SYSMODE_MASTER_CTRL);
			Setstheinitialstate();
			SendLEDNavigationData(KEY_MASTER,KEY_ALL);
			OvertimeOpen();
			CreateMasterContorTextShow(pMsg);
			memset(sige,0,12);
			memset(sige1,0,12);
			beepsta = 0;
			targer = 0;
			targerold = 0;
			restartAlarmFlag = 0;
			Info_Action_Init();
			memset(&TempActoin,0,sizeof(TempActoin));
			memset(&CombinedActoin,0,sizeof(CombinedActoin));
			memset(buf,0,sizeof(buf));
			sprintf(buf,"辅助%d/%d",GetSinglePageValue()+1,GetMAXpage(ShowAuxiliaryAction[0].MAX)+1);
			TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT41),buf);
			WM_SetFocus(pMsg->hWin);
		break;
		case MSG_CHANGE_TEXT:	
			GUIGetDateFunAPI(API_Support_Number,0,&number,&length);	//本架编号
			GUIGetDateFunAPI(API_Number_Increase,0,&direction,&length);	//架号增向
			GUIGetDateFunAPI(API_Control_Range,0,&range,&length);	//隔架架数
			GUIGetDateFunAPI(API_Number_MAX,0,&numberMAX,&length);	//最大架号
			GUIGetDateFunAPI(API_Number_MIN,0,&numberMIN,&length);	//最小架号
			if(direction == 0)
			{
				if(pMsg ->Data.v  == 0) //左邻架
				{
					if(abs(beepsta+1) > range+1)
						break;
					if(!++beepsta)
						beepsta++;
				}
				else//右成组API_Number_Increase
				{
					if(abs(beepsta-1) > range+1)
						break;
					if(!--beepsta)
						beepsta--;
				}
			}
			else
			{
				if(pMsg ->Data.v  == 0) //左邻架
				{
					if(abs(beepsta-1) > range+1)
					break;
					if(!--beepsta)
						beepsta--;
				}
				else//右成组API_Number_Increase
				{
					if(abs(beepsta+1) > range+1)
						break;
					if(!++beepsta)
						beepsta++;
				}			
			}
			targer = number+beepsta;
		    if(targer < numberMIN)
				targer = numberMIN;
			if(targer > numberMAX)
				targer = numberMAX;
			TempActoin.Number = number;
			TempActoin.targer = targer;			
			sprintf(buf,"目标:%d",targer);
			TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT40),buf);				
			if(targer != targerold)
			{
				//发新的预警
				temp[0] = 0;
				temp[1] = targer;				
				SysDataFun(DATA_MSG_CTRL_ALARM,0,0,2,0,&temp[0],0);		
				
				if(targerold != 0)
				{
					//发旧的停止预警
					temp[0] = 1;
					temp[1] = targerold;				
					SysDataFun(DATA_MSG_CTRL_ALARM,0,0,2,0,&temp[0],0);	
				}
				
				targerold = targer;
				
				//架号发生了变化，清除组合动作信息
				memset(&CombinedActoin,0,sizeof(CombinedActoin));
				
				
			}
			/*发送目标架号到逻辑*/
			if(direction == left)
			{
				if(beepsta < 0)
				{
					if(beepsta == -1)
					{
						TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT42),"右邻架控制");	
					}
					else
					{
						sprintf(buf,"右隔%d架控制",abs(beepsta)-1);
						TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT42),buf);	
					}
				}
				else if(beepsta > 0)
				{
					if(beepsta == 1)
					{
						TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT42),"左邻架控制");	
					}
					else
					{
						sprintf(buf,"左隔%d架控制",abs(beepsta)-1);
						TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT42),buf);
					}
				}
				else
				{
					TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT42),"急停");
				}
			}
			else if(direction == right)
			{
				if(beepsta > 0)
				{
					if(beepsta == 1)
					{
						TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT42),"右邻架控制");	
					}
					else
					{
						sprintf(buf,"右隔%d架控制",abs(beepsta)-1);
						TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT42),buf);	
					}
				}
				else if(beepsta < 0)
				{
					if(beepsta == -1)
					{
						TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT42),"左邻架控制");	
					}
					else
					{
						sprintf(buf,"左隔%d架控制",abs(beepsta)-1);
						TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT42),buf);
					}
				}
				else
				{
					TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT42),"急停");
				}
			}
		break;
		case WM_KEY:
			SendLEDNavigationData(KEY_MASTER,KEY_ALL);
			if((((WM_KEY_INFO*)(pMsg->Data.p))->PressedCnt) > 0) //按键按下
			{
				ChangeInfoKey(BackGround_hWin,((WM_KEY_INFO*)(pMsg->Data.p))->Key,1);
				switch (((WM_KEY_INFO*)(pMsg->Data.p))->Key) 
				{ 
					case USER_KEY_ESC:
						if(ReadAction_API(showbuf) != 0)
							break;
						temp[0] = 1;
						temp[1] = targer;				
						SysDataFun(DATA_MSG_CTRL_ALARM,0,0,2,0,&temp[0],0);	
//						SysDataFun(DATA_MSG_SetStopPress,ENUM_SYSTEM,0,0,0,&pstate,0);
						GUI_EndDialog(pMsg->hWin, 0);
						CreateHomeFrame();				
						break;
					case USER_KEY_UP:
						if(memcmp(sige,sige1,12) == 0)
						{
							ChangeInfoText(pMsg->hWin,0);
						}							
						break;
					case USER_KEY_DOWN:
						if(memcmp(sige,sige1,12) == 0)
						{
							ChangeInfoText(pMsg->hWin,1);
						}
						break;
					case USER_KEY_LEFT:
						if(getJsonParaChildSize(0x600000000000000) == 1)
							break;
						else
						{
							if(memcmp(sige,sige1,12) == 0)
							{
								temp[0] = 1;
								temp[1] = targer;				
								SysDataFun(DATA_MSG_CTRL_ALARM,0,0,2,0,&temp[0],0);	
								GUI_EndDialog(pMsg->hWin, 0);
								ChangeInfoText(CreateGroupLeft(),0);	
								ChangeInfoButton(BackGround_hWin,2);		
							}							
							break;
						}					
					case USER_KEY_RIGHT:
						if(getJsonParaChildSize(0x600000000000000) == 1)
							break;
						else 
						{
							if(memcmp(sige,sige1,12) == 0)
							{						
								temp[0] = 1;
								temp[1] = targer;				
								SysDataFun(DATA_MSG_CTRL_ALARM,0,0,2,0,&temp[0],0);	
								GUI_EndDialog(pMsg->hWin, 0);
								ChangeInfoText(CreateGroupLeft(),1);
								ChangeInfoButton(BackGround_hWin,2);	
							}							
							break;
						}
					case USER_KEY_ASSIST:	
						if(memcmp(sige,sige1,12) == 0)
						{
							ChangeInfoButton(BackGround_hWin,0);
							memset(buf,0,sizeof(buf));
							sprintf(buf,"辅助%d/%d",GetSinglePageValue()+1,GetMAXpage(ShowAuxiliaryAction[0].MAX)+1);
							TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT41),buf);	
						}
						break;
					case USER_KEY_ENTER:
						ActionControlOfAdjacentRack(CombinedActoin,0);
						memset(&CombinedActoin,0,sizeof(CombinedActoin));
						break;

					case USER_KEY_LEFT1:
						if((1+(GetSinglePageValue()*6)) > ShowAuxiliaryAction[0].MAX)
							break;
						sige[0] = 1;
						TempActoin.Action = ShowAuxiliaryAction[((GetSinglePageValue()*6))];
						
						if(TempActoin.Action.type == Sing)
						{
							QueryOppositeAction(TempActoin,0);
							//发送TempActoin
						}
						else if(TempActoin.Action.type == Combin)
						{
							CombinedActoin = TempActoin;
							memset(showbuf,0,sizeof(showbuf));
							//发送全部抬起
						}
						break;
					case USER_KEY_LEFT2:
						if((3+(GetSinglePageValue()*6)) > ShowAuxiliaryAction[0].MAX)
							break;
						sige[1] = 1;
						 TempActoin.Action = ShowAuxiliaryAction[2+((GetSinglePageValue()*6))];
						
						if(TempActoin.Action.type == Sing)
						{
							QueryOppositeAction(TempActoin,0);
							//发送TempActoin
						}
						else if(TempActoin.Action.type == Combin)
						{
							CombinedActoin = TempActoin;
							memset(showbuf,0,sizeof(showbuf));
							//发送全部抬起
						}
						break;
					case USER_KEY_LEFT3:
						if((5+(GetSinglePageValue()*6)) > ShowAuxiliaryAction[0].MAX)
							break;
						sige[2] = 1;
						 TempActoin.Action = ShowAuxiliaryAction[4+((GetSinglePageValue()*6))];
						
						if(TempActoin.Action.type == Sing)
						{
							QueryOppositeAction(TempActoin,0);
							//发送TempActoin
						}
						else if(TempActoin.Action.type == Combin)
						{
							CombinedActoin = TempActoin;
							memset(showbuf,0,sizeof(showbuf));
							//发送全部抬起
						}
						break;
					case USER_KEY_RIGHT1:
						if((2+(GetSinglePageValue()*6)) > ShowAuxiliaryAction[0].MAX)
							break;
						sige[3] = 1;
						 TempActoin.Action = ShowAuxiliaryAction[1+((GetSinglePageValue()*6))];
						
						if(TempActoin.Action.type == Sing)
						{
							QueryOppositeAction(TempActoin,0);
							//发送TempActoin
						}
						else if(TempActoin.Action.type == Combin)
						{
							CombinedActoin = TempActoin;
							memset(showbuf,0,sizeof(showbuf));
							//发送全部抬起
						}
						break;
					case USER_KEY_RIGHT2:
						if((4+(GetSinglePageValue()*6)) > ShowAuxiliaryAction[0].MAX)
							break;
						sige[4] = 1;
						 TempActoin.Action = ShowAuxiliaryAction[3+((GetSinglePageValue()*6))];
						
						if(TempActoin.Action.type == Sing)
						{
							QueryOppositeAction(TempActoin,0);
							//发送TempActoin
						}
						else if(TempActoin.Action.type == Combin)
						{
							CombinedActoin = TempActoin;
							memset(showbuf,0,sizeof(showbuf));
							//发送全部抬起
						}
						break;
					case USER_KEY_RIGHT3:
						if((6+(GetSinglePageValue()*6)) > ShowAuxiliaryAction[0].MAX)
							break;
						sige[5] = 1;
						 TempActoin.Action = ShowAuxiliaryAction[5+((GetSinglePageValue()*6))];
						
						if(TempActoin.Action.type == Sing)
						{
							QueryOppositeAction(TempActoin,0);
							//发送TempActoin
						}
						else if(TempActoin.Action.type == Combin)
						{
							CombinedActoin = TempActoin;
							memset(showbuf,0,sizeof(showbuf));
							//发送全部抬起
						}
						break;
					case USER_KEY_2:
						sige[6] = 1;
						TempActoin.Action = ShowKeyboardAction[0][0];
							QueryOppositeAction(TempActoin,0);
						TempActoin.Action = ShowKeyboardAction[0][1];
							QueryOppositeAction(TempActoin,0);
						break;	
					case USER_KEY_3:
						sige[7] = 1;
						TempActoin.Action = ShowKeyboardAction[1][0];
							QueryOppositeAction(TempActoin,0);
						TempActoin.Action = ShowKeyboardAction[1][1];
							QueryOppositeAction(TempActoin,0);
						break;	
					case USER_KEY_5:
						sige[8] = 1;
						TempActoin.Action = ShowKeyboardAction[2][0];
							QueryOppositeAction(TempActoin,0);
						TempActoin.Action = ShowKeyboardAction[2][1];
							QueryOppositeAction(TempActoin,0);
						break;	
					case USER_KEY_6:
						sige[9] = 1;
						TempActoin.Action = ShowKeyboardAction[3][0];					
							QueryOppositeAction(TempActoin,0);
						TempActoin.Action = ShowKeyboardAction[3][1];
							QueryOppositeAction(TempActoin,0);
						break;	
					case USER_KEY_8:
						sige[10] = 1;
						TempActoin.Action = ShowKeyboardAction[4][0];
							QueryOppositeAction(TempActoin,0);
						TempActoin.Action = ShowKeyboardAction[4][1];
							QueryOppositeAction(TempActoin,0);
						break;	
					case USER_KEY_9:
						sige[11] = 1;
						TempActoin.Action = ShowKeyboardAction[5][0];
							QueryOppositeAction(TempActoin,0);
						TempActoin.Action = ShowKeyboardAction[5][1];
							QueryOppositeAction(TempActoin,0);
						break;					
					default:
						break;			
				}
			}
			else //按键抬起
			{				
				ChangeInfoKey(BackGround_hWin,((WM_KEY_INFO*)(pMsg->Data.p))->Key,0);
				switch (((WM_KEY_INFO*)(pMsg->Data.p))->Key) 
				{ 
					case USER_KEY_LEFT1:
						if((1+(GetSinglePageValue()*6)) > ShowAuxiliaryAction[0].MAX)
							break;
						sige[0] = 0;
						 TempActoin.Action = ShowAuxiliaryAction[((GetSinglePageValue()*6))];
						
						if(TempActoin.Action.type == Sing)
						{
							ActionControlOfAdjacentRack(TempActoin,1);
							DelActionInfoQueue(&TempActoin.Action);
							//发送TempActoin
						}
						else if(TempActoin.Action.type == Combin)
						{
							CombinedActoin = TempActoin;
							memset(showbuf,0,sizeof(showbuf));
							//发送全部抬起
						}
						break;
					case USER_KEY_LEFT2:
						if((3+(GetSinglePageValue()*6)) > ShowAuxiliaryAction[0].MAX)
							break;
						sige[1] = 0;
						 TempActoin.Action = ShowAuxiliaryAction[2+((GetSinglePageValue()*6))];
						
						if(TempActoin.Action.type == Sing)
						{
							ActionControlOfAdjacentRack(TempActoin,1);
							DelActionInfoQueue(&TempActoin.Action);
							//发送TempActoin
						}
						else if(TempActoin.Action.type == Combin)
						{
							CombinedActoin = TempActoin;
							memset(showbuf,0,sizeof(showbuf));
							//发送全部抬起
						}
						break;
					case USER_KEY_LEFT3:
						if((5+(GetSinglePageValue()*6)) > ShowAuxiliaryAction[0].MAX)
							break;
						sige[2] = 0;
						 TempActoin.Action = ShowAuxiliaryAction[4+((GetSinglePageValue()*6))];
						
						if(TempActoin.Action.type == Sing)
						{
							ActionControlOfAdjacentRack(TempActoin,1);
							DelActionInfoQueue(&TempActoin.Action);
							//发送TempActoin
						}
						else if(TempActoin.Action.type == Combin)
						{
							CombinedActoin = TempActoin;
							memset(showbuf,0,sizeof(showbuf));
							//发送全部抬起
						}
						break;
					case USER_KEY_RIGHT1:
						if((2+(GetSinglePageValue()*6)) > ShowAuxiliaryAction[0].MAX)
							break;
						sige[3] = 0;
						 TempActoin.Action = ShowAuxiliaryAction[1+((GetSinglePageValue()*6))];
						
						if(TempActoin.Action.type == Sing)
						{
							ActionControlOfAdjacentRack(TempActoin,1);
							DelActionInfoQueue(&TempActoin.Action);
							//发送TempActoin
						}
						else if(TempActoin.Action.type == Combin)
						{
							CombinedActoin = TempActoin;
							memset(showbuf,0,sizeof(showbuf));
							//发送全部抬起
						}
						break;
					case USER_KEY_RIGHT2:
						if((4+(GetSinglePageValue()*6)) > ShowAuxiliaryAction[0].MAX)
							break;
						sige[4] = 0;
						 TempActoin.Action = ShowAuxiliaryAction[3+((GetSinglePageValue()*6))];
						
						if(TempActoin.Action.type == Sing)
						{
							ActionControlOfAdjacentRack(TempActoin,1);
							DelActionInfoQueue(&TempActoin.Action);
							//发送TempActoin
						}
						else if(TempActoin.Action.type == Combin)
						{
							CombinedActoin = TempActoin;
							memset(showbuf,0,sizeof(showbuf));
							//发送全部抬起
						}
						break;
					case USER_KEY_RIGHT3:
						if((6+(GetSinglePageValue()*6)) > ShowAuxiliaryAction[0].MAX)
							break;
						sige[5] = 0;
						 TempActoin.Action = ShowAuxiliaryAction[5+((GetSinglePageValue()*6))];
						
						if(TempActoin.Action.type == Sing)
						{
							ActionControlOfAdjacentRack(TempActoin,1);
							DelActionInfoQueue(&TempActoin.Action);
							//发送TempActoin
						}
						else if(TempActoin.Action.type == Combin)
						{
							CombinedActoin = TempActoin;
							memset(showbuf,0,sizeof(showbuf));
							//发送全部抬起
						}
						break;
					case USER_KEY_2:
						sige[6] = 0;
						TempActoin.Action = ShowKeyboardAction[0][0];
						ActionControlOfAdjacentRack(TempActoin,1);
						DelActionInfoQueue(&TempActoin.Action);
						TempActoin.Action = ShowKeyboardAction[0][1];
						ActionControlOfAdjacentRack(TempActoin,1);	
						DelActionInfoQueue(&TempActoin.Action);
						break;	
					case USER_KEY_3:
						sige[7] = 0;
						TempActoin.Action = ShowKeyboardAction[1][0];
						ActionControlOfAdjacentRack(TempActoin,1);
						DelActionInfoQueue(&TempActoin.Action);
						TempActoin.Action = ShowKeyboardAction[1][1];
						ActionControlOfAdjacentRack(TempActoin,1);	
						DelActionInfoQueue(&TempActoin.Action);
						break;	
					case USER_KEY_5:
						sige[8] = 0;
						TempActoin.Action = ShowKeyboardAction[2][0];
						ActionControlOfAdjacentRack(TempActoin,1);
						DelActionInfoQueue(&TempActoin.Action);
						TempActoin.Action = ShowKeyboardAction[2][1];
						ActionControlOfAdjacentRack(TempActoin,1);	
						DelActionInfoQueue(&TempActoin.Action);
						break;	
					case USER_KEY_6:
						sige[9] = 0;
						TempActoin.Action = ShowKeyboardAction[3][0];
						ActionControlOfAdjacentRack(TempActoin,1);
						DelActionInfoQueue(&TempActoin.Action);
						TempActoin.Action = ShowKeyboardAction[3][1];
						ActionControlOfAdjacentRack(TempActoin,1);	
						DelActionInfoQueue(&TempActoin.Action);
						break;	
					case USER_KEY_8:
						sige[10] = 0;
						TempActoin.Action = ShowKeyboardAction[4][0];
						DelActionInfoQueue(&TempActoin.Action);
						ActionControlOfAdjacentRack(TempActoin,1);
						TempActoin.Action = ShowKeyboardAction[4][1];						
						ActionControlOfAdjacentRack(TempActoin,1);	
						DelActionInfoQueue(&TempActoin.Action);
						break;	
					case USER_KEY_9:
						sige[11] = 0;
						TempActoin.Action = ShowKeyboardAction[5][0];
						ActionControlOfAdjacentRack(TempActoin,1);
						DelActionInfoQueue(&TempActoin.Action);
						TempActoin.Action = ShowKeyboardAction[5][1];
						ActionControlOfAdjacentRack(TempActoin,1);	
						DelActionInfoQueue(&TempActoin.Action);
						break;	
				}
				
			}		
			memset(showbuf,0,sizeof(showbuf));
			k = ReadAction_API(showbuf);
			for(i=0,j=k-1;i<k/2;i++,j--)
			{
				NameAndId tem = showbuf[i];
				showbuf[i] = showbuf[j];
				showbuf[j] = tem;
			}
			for(i= 0 ;i<7;i++)
			{
				TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT43+i)," ");
			}

			if(k == 1)
			{
				sensorRes = getManualSensorcalcValue(showbuf[0].id,&sensorbuf);
				if(sensorRes)
				{
  					memset(singleActionAndSensor,0,sizeof(singleActionAndSensor));
					strcat(singleActionAndSensor,showbuf[0].name);
					strncat(singleActionAndSensor,sensorbuf,4);
					TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT43),singleActionAndSensor);
				}
				else
				{
					TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT43),showbuf[0].name);
				}
			}
			else
			{
				TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT43),showbuf[0].name);
			}
			TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT44),showbuf[1].name);
			TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT45),showbuf[2].name);
			TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT46),showbuf[3].name);
			TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT47),showbuf[4].name);
			TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT48),showbuf[5].name);			
		  	TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT49),CombinedActoin.Action.name);
			break;		
		case WM_TIMER:	
			if(pMsg->hWin != WM_GetFocussedWindow())
				WM_SetFocus(pMsg->hWin);
			OvertimeReturn(pMsg);
			SlaveControlRestartAlarm();
			Determineneedtoreturninterface(pMsg,0);
			if(GetAuxiliarySige())
			{
				ChangeInfoButton(BackGround_hWin,3);
				ResetAuxiliarySigereturn();
			}
			if(k == 1)
			{
				sensorRes = getManualSensorcalcValue(showbuf[0].id,&sensorbuf);
				if(sensorRes)
				{
  					memset(singleActionAndSensor,0,sizeof(singleActionAndSensor));
					strcat(singleActionAndSensor,showbuf[0].name);
					strncat(singleActionAndSensor,sensorbuf,4);
					TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT43),singleActionAndSensor);
				}
			}
			WM_RestartTimer(MasterContor_hTimer,100);
			break;
		case WM_PAINT:
			GUI_MEMDEV_WriteAt(HomeFrame_DEV[1], 65, 50);
			GUI_MEMDEV_WriteAt(Group_DEV[2], 60, 270);
			break;
		default:
			WM_DefaultProc(pMsg);
			break;
	}
}


extern uint8_t  CurrentWindow;
/*
*********************************************************************************************************
*	函 数 名: 
*	功能说明: 创建邻架画面
*	形    参: 
*	返 回 值: 
*********************************************************************************************************
*/
WM_HWIN CreateMasterContor(void) 
{		
	MasterContor_hWin = WM_CreateWindowAsChild(60,0,360,320,BackGround_hWin,WM_CF_SHOW|WM_CF_HASTRANS,MasterContor_cbDialog,0);
	
	MasterContor_hTimer = WM_CreateTimer(WM_GetClientWindow(MasterContor_hWin), 0, 10, 0);
	CurrentWindow = masterwin;
	return MasterContor_hWin;
}
