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
#include "User_GUI.h"
#include "includes.h"					//ucos 使用	  
#include "sys_msg_def.h"
#include "SysFunc_API.h"
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

extern WM_HWIN BackGround_hWin;


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
					
WM_HTIMER 	SlveContor_hTimer;
			
			
WM_HWIN   SlveContor_hWin;
extern NameAndId  ShowAuxiliaryAction[];		
extern NameAndId   SingleActionBUF[];
extern NameAndId  ShowKeyboardAction[][2];
void  SlveContorRef(WM_MESSAGE * pMsg) 	
{
	uint8_t i,j,k = 0;
	NameAndId showbuf[6] = {0};
	uint8_t* pstate=NULL;
	uint16_t len=0;
	ActionInfoType temp;
	SysDataFun(DATA_MSG_GetActionInfo,0,0,0,0,&pstate,&len);
	
	if(len != 0)
	{
		memcpy(&temp,pstate,sizeof(temp));
		for(i=0;i<len;i++)
		{
			for(j=0;j<=SingleActionMAX;j++)
			{
				if(temp.actionType != 0)
				{
					k--;
					break;
				}
				if((SingleActionBUF[j].id) == temp.actionID)
				{
					memcpy(&showbuf[k],&SingleActionBUF[j],sizeof(NameAndId));				
					break;
				}
			}
			memcpy(&temp,temp.pNext,sizeof(temp));
			k ++;
		}
	}
	for(i= 0 ;i<6;i++)
	{
		TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT43+i)," ");
	}
	TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT43),showbuf[0].name);
	TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT44),showbuf[1].name);
	TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT45),showbuf[2].name);
	TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT46),showbuf[3].name);
	TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT47),showbuf[4].name);
	TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT48),showbuf[5].name);
	
	
	
}
static void SlveContor_cbDialog(WM_MESSAGE * pMsg) 
{
	uint32_t t0, t1;
	static uint8_t beepsta;
	WM_HWIN hItem;
	int     NCode;
	int     Id;
	static  SingleStartUp  TempActoin;
	uint8_t buf[4] = {0,1,0,0};
	uint8_t buf1[4] = {1,1,0,0};
	static uint8_t sige[6] = {0};
	static uint8_t sige1[6] = {0};
	switch (pMsg->MsgId) 
	{
		case WM_CREATE:
			CreateSlveContorTextShow(pMsg);	
			WM_SetFocus(pMsg->hWin);
		break;
		case WM_TIMER:	
			SlveContorRef(pMsg);	
			SupporthomeRefresh(pMsg);	
			if(pMsg->hWin != WM_GetFocussedWindow())
				WM_SetFocus(pMsg->hWin);
			WM_RestartTimer(SlveContor_hTimer,100);
			break;
		case WM_PAINT:
			t0 = GUI_GetTime();
			GUI_MEMDEV_WriteAt(HomeFrame_DEV[1], 65, 50);
			GUI_MEMDEV_WriteAt(Group_DEV[2], 60, 270);
			GUI_MEMDEV_WriteAt(PassWord_DEV[1], 60+49, 285);
		
			t1 = GUI_GetTime() - t0;
			break;
				case WM_KEY:
			if((((WM_KEY_INFO*)(pMsg->Data.p))->PressedCnt) > 0) //按键按下
			{
				
				switch (((WM_KEY_INFO*)(pMsg->Data.p))->Key) 
				{ 
//					case USER_KEY_UP:
//						GUI_EndDialog(pMsg->hWin, 0);
//						ChangeInfoText(CreateMasterContor(),0);	
//						break;
//					case USER_KEY_DOWN:
//						GUI_EndDialog(pMsg->hWin, 0);
//						ChangeInfoText(CreateMasterContor(),1);
//						break;
//					case USER_KEY_LEFT:
//						GUI_EndDialog(pMsg->hWin, 0);
//						ChangeInfoText(CreateGroupLeft(),0);
//						ChangeInfoButton(BackGround_hWin,2);
//						break;
//					case USER_KEY_RIGHT:
//						GUI_EndDialog(pMsg->hWin, 0);
//						ChangeInfoText(CreateGroupLeft(),1);
//						ChangeInfoButton(BackGround_hWin,2);					
//						break;
					case USER_KEY_ASSIST:
						if(memcmp(sige,sige1,6) == 0)
							ChangeInfoButton(BackGround_hWin,0);
						break;
//					case USER_KEY_MENU:	
//						GUI_EndDialog(pMsg->hWin, 0);
//						CreatePassWordInPut();
//						break;
					case USER_KEY_LEFT1:
						if((1+(GetSinglePageValue()*6)) > ShowAuxiliaryAction[0].MAX)
							break;
						sige[0] = 1;
						TempActoin.Action = ShowAuxiliaryAction[((GetSinglePageValue()*6))];
						ActionControlOfThisRack(TempActoin,buf);
						break;
					case USER_KEY_LEFT2:
						if((3+(GetSinglePageValue()*6)) > ShowAuxiliaryAction[0].MAX)
							break;
						sige[1] = 1;
						TempActoin.Action = ShowAuxiliaryAction[2+((GetSinglePageValue()*6))];
						ActionControlOfThisRack(TempActoin,buf);
						break;	
					case USER_KEY_LEFT3:
						if((5+(GetSinglePageValue()*6)) > ShowAuxiliaryAction[0].MAX)
							break;
						sige[2] = 1;
						TempActoin.Action = ShowAuxiliaryAction[4+((GetSinglePageValue()*6))];
						ActionControlOfThisRack(TempActoin,buf);
						break;	
					case USER_KEY_RIGHT1:
						if((2+(GetSinglePageValue()*6)) > ShowAuxiliaryAction[0].MAX)
							break;
						sige[3] = 1;
						TempActoin.Action = ShowAuxiliaryAction[1+((GetSinglePageValue()*6))];
						ActionControlOfThisRack(TempActoin,buf);
						break;	
					case USER_KEY_RIGHT2:
						if((4+(GetSinglePageValue()*6)) > ShowAuxiliaryAction[0].MAX)
							break;
						sige[4] = 1;
						TempActoin.Action = ShowAuxiliaryAction[3+((GetSinglePageValue()*6))];
						ActionControlOfThisRack(TempActoin,buf);
						break;	
					case USER_KEY_RIGHT3:
						if((6+(GetSinglePageValue()*6)) > ShowAuxiliaryAction[0].MAX)
							break;
						sige[5] = 1;
						TempActoin.Action = ShowAuxiliaryAction[5+((GetSinglePageValue()*6))];
						ActionControlOfThisRack(TempActoin,buf);
						break;							
					case USER_KEY_2:
						TempActoin.Action = ShowKeyboardAction[0][0];
						ActionControlOfThisRack(TempActoin,buf);
						TempActoin.Action = ShowKeyboardAction[0][1];
						ActionControlOfThisRack(TempActoin,buf);	
						break;	
					case USER_KEY_3:
						TempActoin.Action = ShowKeyboardAction[1][0];
						ActionControlOfThisRack(TempActoin,buf);
						TempActoin.Action = ShowKeyboardAction[1][1];
						ActionControlOfThisRack(TempActoin,buf);	
						break;	
					case USER_KEY_5:
						TempActoin.Action = ShowKeyboardAction[2][0];
						ActionControlOfThisRack(TempActoin,buf);
						TempActoin.Action = ShowKeyboardAction[2][1];
						ActionControlOfThisRack(TempActoin,buf);	
						break;	
					case USER_KEY_6:
						TempActoin.Action = ShowKeyboardAction[3][0];
						ActionControlOfThisRack(TempActoin,buf);
						TempActoin.Action = ShowKeyboardAction[3][1];
						ActionControlOfThisRack(TempActoin,buf);	
						break;	
					case USER_KEY_8:
						TempActoin.Action = ShowKeyboardAction[4][0];
						ActionControlOfThisRack(TempActoin,buf);
						TempActoin.Action = ShowKeyboardAction[4][1];
						ActionControlOfThisRack(TempActoin,buf);	
						break;	
					case USER_KEY_9:
						TempActoin.Action = ShowKeyboardAction[5][0];
						ActionControlOfThisRack(TempActoin,buf);
						TempActoin.Action = ShowKeyboardAction[5][1];
						ActionControlOfThisRack(TempActoin,buf);	
						break;						
					default:
						break;			
				}
				ChangeInfoKey(BackGround_hWin,((WM_KEY_INFO*)(pMsg->Data.p))->Key,1);
			}
			else //按键抬起
			{				
				
				switch (((WM_KEY_INFO*)(pMsg->Data.p))->Key) 
				{ 
//					case USER_KEY_WORK:	
//						GUI_EndDialog(BackGround_hWin, 0);
//						CreateStateTask();
//						break;
					case USER_KEY_LEFT1:
						if((1+(GetSinglePageValue()*6)) > ShowAuxiliaryAction[0].MAX)
							break;
						sige[0] = 0;
						TempActoin.Action = ShowAuxiliaryAction[((GetSinglePageValue()*6))];
						ActionControlOfThisRack(TempActoin,buf1);
						break;
					case USER_KEY_LEFT2:
						if((3+(GetSinglePageValue()*6)) > ShowAuxiliaryAction[0].MAX)
							break;
						sige[1] = 0;
						TempActoin.Action = ShowAuxiliaryAction[2+((GetSinglePageValue()*6))];
						ActionControlOfThisRack(TempActoin,buf1);
						break;	
					case USER_KEY_LEFT3:
						if((5+(GetSinglePageValue()*6)) > ShowAuxiliaryAction[0].MAX)
							break;
						sige[2] = 0;
						TempActoin.Action = ShowAuxiliaryAction[4+((GetSinglePageValue()*6))];
						ActionControlOfThisRack(TempActoin,buf1);
						break;	
					case USER_KEY_RIGHT1:
						if((2+(GetSinglePageValue()*6)) > ShowAuxiliaryAction[0].MAX)
							break;
						sige[3] = 0;
						TempActoin.Action = ShowAuxiliaryAction[1+((GetSinglePageValue()*6))];
						ActionControlOfThisRack(TempActoin,buf1);
						break;	
					case USER_KEY_RIGHT2:
						if((4+(GetSinglePageValue()*6)) > ShowAuxiliaryAction[0].MAX)
							break;
						sige[4] = 0;
						TempActoin.Action = ShowAuxiliaryAction[3+((GetSinglePageValue()*6))];
						ActionControlOfThisRack(TempActoin,buf1);
						break;	
					case USER_KEY_RIGHT3:
						if((6+(GetSinglePageValue()*6)) > ShowAuxiliaryAction[0].MAX)
							break;
						sige[5] = 0;
						TempActoin.Action = ShowAuxiliaryAction[5+((GetSinglePageValue()*6))];
						ActionControlOfThisRack(TempActoin,buf1);
						break;							
					case USER_KEY_2:
						TempActoin.Action = ShowKeyboardAction[0][0];
						ActionControlOfThisRack(TempActoin,buf1);
						TempActoin.Action = ShowKeyboardAction[0][1];
						ActionControlOfThisRack(TempActoin,buf1);	
						break;	
					case USER_KEY_3:
						TempActoin.Action = ShowKeyboardAction[1][0];
						ActionControlOfThisRack(TempActoin,buf1);
						TempActoin.Action = ShowKeyboardAction[1][1];
						ActionControlOfThisRack(TempActoin,buf1);	
						break;	
					case USER_KEY_5:
						TempActoin.Action = ShowKeyboardAction[2][0];
						ActionControlOfThisRack(TempActoin,buf1);
						TempActoin.Action = ShowKeyboardAction[2][1];
						ActionControlOfThisRack(TempActoin,buf1);	
						break;	
					case USER_KEY_6:
						TempActoin.Action = ShowKeyboardAction[3][0];
						ActionControlOfThisRack(TempActoin,buf1);
						TempActoin.Action = ShowKeyboardAction[3][1];
						ActionControlOfThisRack(TempActoin,buf1);	
						break;	
					case USER_KEY_8:
						TempActoin.Action = ShowKeyboardAction[4][0];
						ActionControlOfThisRack(TempActoin,buf1);
						TempActoin.Action = ShowKeyboardAction[4][1];
						ActionControlOfThisRack(TempActoin,buf1);	
						break;	
					case USER_KEY_9:
						TempActoin.Action = ShowKeyboardAction[5][0];
						ActionControlOfThisRack(TempActoin,buf1);
						TempActoin.Action = ShowKeyboardAction[5][1];
						ActionControlOfThisRack(TempActoin,buf1);	
						break;	
					default:
						break;	
				}
				ChangeInfoKey(BackGround_hWin,((WM_KEY_INFO*)(pMsg->Data.p))->Key,0);
			}
			break;
		default:
			WM_DefaultProc(pMsg);
			break;
	}
}

/*
*********************************************************************************************************
*	函 数 名: 
*	功能说明: 创建从控画面
*	形    参: 
*	返 回 值: 
*********************************************************************************************************
*/
WM_HWIN CreateSlveContor(void) 
{		
	SlveContor_hWin = WM_CreateWindowAsChild(60,0,360,320,BackGround_hWin,WM_CF_SHOW|WM_CF_HASTRANS,SlveContor_cbDialog,0);
	
	SlveContor_hTimer = WM_CreateTimer(WM_GetClientWindow(SlveContor_hWin), 0, 10, 0);
	return SlveContor_hWin;
}
