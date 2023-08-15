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
extern SigActionParamType  ActionParamType;
extern CombActionParamType  BaseParamType;
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
					
WM_HTIMER 	RackContor_hTimer;
			
			
WM_HWIN 	RackContor_hWin;
		
			
static void RackContor_cbDialog(WM_MESSAGE * pMsg) 
{
	static uint8_t sige[6] = {0};
	static uint8_t sige1[6] = {0};
	uint16_t i;
	uint16_t length;
	char buf[10];
	static  SingleStartUp  TempActoin,CombinedActoin;
	globalStateType  temp;
	switch (pMsg->MsgId) 
	{
		case WM_CREATE:
			Setstheinitialstate();
			memset(sige,0,6);
			memset(sige1,0,6);

//			SendLEDNavigationData(KEY_MASTER,KEY_ALL);
			OvertimeOpen();
			CreateRackContorTextShow(pMsg);
			memset(buf,0,sizeof(buf));
			sprintf(buf,"辅助%d/%d",GetSinglePageValue()+1,GetMAXpage(ShowAuxiliaryAction[0].MAX)+1);
			TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT43),buf);
			memset(&TempActoin,0,sizeof(TempActoin));
			memset(&CombinedActoin,0,sizeof(CombinedActoin));
			WM_SetFocus(pMsg->hWin);
		break;
		case MSG_CHANGE_RACK:	
			for(i=0;i<ShowAuxiliaryAction[0].MAX+1;i++)
			{
				if((pMsg ->Data.v == ShowAuxiliaryAction[i].id) && (ShowAuxiliaryAction[i].type ==1))
				{
					CombinedActoin.Action = ShowAuxiliaryAction[i];
				}
			}		
			TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT40), CombinedActoin.Action.name);
		break;
		case WM_KEY:
			SendLEDNavigationData(KEY_MASTER,KEY_ALL);
			if((((WM_KEY_INFO*)(pMsg->Data.p))->PressedCnt) > 0) //按键按下
			{
				ChangeInfoKey(BackGround_hWin,((WM_KEY_INFO*)(pMsg->Data.p))->Key,1);
				switch (((WM_KEY_INFO*)(pMsg->Data.p))->Key) 
				{ 
					case USER_KEY_ESC:
						GUI_EndDialog(pMsg->hWin, 0);
						CreateHomeFrame();				
						break;
					case USER_KEY_ASSIST:	
						if(memcmp(sige,sige1,6) == 0)
						{
							ChangeInfoButton(BackGround_hWin,0);
							memset(buf,0,sizeof(buf));
							sprintf(buf,"辅助%d/%d",GetSinglePageValue()+1,GetMAXpage(ShowAuxiliaryAction[0].MAX)+1);
							TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT43),buf);	
						}
						break;
					case USER_KEY_ENTER:					
						ActionControlOfThisRack(CombinedActoin,0);					
						memset(&CombinedActoin,0,sizeof(CombinedActoin));
						break;
					case USER_KEY_LEFT1:
						if((1+(GetSinglePageValue()*6)) > ShowAuxiliaryAction[0].MAX)
							break;
						sige[0] = 1;
						TempActoin.Action = ShowAuxiliaryAction[((GetSinglePageValue()*6))];
						if(TempActoin.Action.type == Combin)
						{
							GUIGetDateFunAPI(API_Comblnation_Action,TempActoin.Action.id,&BaseParamType,0);
							if((BaseParamType.baseParam.controlMode != 0) && (BaseParamType.baseParam.controlMode != 1))
							{
								return;
							}
							CombinedActoin.Action = TempActoin.Action;
							TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT40), CombinedActoin.Action.name);
							TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT41), "=请按启动键=");
						}
						else if(TempActoin.Action.type == Sing)
						{
							
							GUIGetDateFunAPI(API_Singel_Action,TempActoin.Action.id,&ActionParamType,0);
							if((ActionParamType.actionParam.controlMode != 0) && (ActionParamType.actionParam.controlMode != 1))//
							{
								return;
							}
							memset(&CombinedActoin,0,sizeof(CombinedActoin));
							TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT40), " ");
							TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT41), " ");
							ActionControlOfThisRack(TempActoin,0);	
						}							
						break;
					case USER_KEY_LEFT2:
						if((3+(GetSinglePageValue()*6)) > ShowAuxiliaryAction[0].MAX)
							break;
						sige[1] = 1;
						TempActoin.Action = ShowAuxiliaryAction[2+((GetSinglePageValue()*6))];
						if(TempActoin.Action.type == Combin)
						{
							GUIGetDateFunAPI(API_Comblnation_Action,TempActoin.Action.id,&BaseParamType,0);
							if((BaseParamType.baseParam.controlMode != 0) && (BaseParamType.baseParam.controlMode != 1))
							{
								return;
							}
							CombinedActoin.Action = TempActoin.Action;
							TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT40), CombinedActoin.Action.name);
							TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT41), "=请按启动键=");
						}
						else if(TempActoin.Action.type == Sing)
						{
							
							GUIGetDateFunAPI(API_Singel_Action,TempActoin.Action.id,&ActionParamType,0);
							if((ActionParamType.actionParam.controlMode != 0) && (ActionParamType.actionParam.controlMode != 1))//
							{
								return;
							}
							memset(&CombinedActoin,0,sizeof(CombinedActoin));
							TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT40), " ");
							TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT41), " ");
							ActionControlOfThisRack(TempActoin,0);	
						}	
						break;
					case USER_KEY_LEFT3:
						if((5+(GetSinglePageValue()*6)) > ShowAuxiliaryAction[0].MAX)
							break;
						sige[2] = 1;
						TempActoin.Action = ShowAuxiliaryAction[4+((GetSinglePageValue()*6))];
						if(TempActoin.Action.type == Combin)
						{
							GUIGetDateFunAPI(API_Comblnation_Action,TempActoin.Action.id,&BaseParamType,0);
							if((BaseParamType.baseParam.controlMode != 0) && (BaseParamType.baseParam.controlMode != 1))
							{
								return;
							}
							CombinedActoin.Action = TempActoin.Action;
							TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT40), CombinedActoin.Action.name);
							TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT41), "=请按启动键=");
						}
						else if(TempActoin.Action.type == Sing)
						{
							
							GUIGetDateFunAPI(API_Singel_Action,TempActoin.Action.id,&ActionParamType,0);
							if((ActionParamType.actionParam.controlMode != 0) && (ActionParamType.actionParam.controlMode != 1))//
							{
								return;
							}
							memset(&CombinedActoin,0,sizeof(CombinedActoin));
							TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT40), " ");
							TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT41), " ");
							ActionControlOfThisRack(TempActoin,0);	
						}	
						break;
					case USER_KEY_RIGHT1:
						if((2+(GetSinglePageValue()*6)) > ShowAuxiliaryAction[0].MAX)
							break;
						sige[3] = 1;
						TempActoin.Action = ShowAuxiliaryAction[1+((GetSinglePageValue()*6))];
						if(TempActoin.Action.type == Combin)
						{
							GUIGetDateFunAPI(API_Comblnation_Action,TempActoin.Action.id,&BaseParamType,0);
							if((BaseParamType.baseParam.controlMode != 0) && (BaseParamType.baseParam.controlMode != 1))
							{
								return;
							}
							CombinedActoin.Action = TempActoin.Action;
							TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT40), CombinedActoin.Action.name);
							TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT41), "=请按启动键=");
						}
						else if(TempActoin.Action.type == Sing)
						{
							
							GUIGetDateFunAPI(API_Singel_Action,TempActoin.Action.id,&ActionParamType,0);
							if((ActionParamType.actionParam.controlMode != 0) && (ActionParamType.actionParam.controlMode != 1))//
							{
								return;
							}
							memset(&CombinedActoin,0,sizeof(CombinedActoin));
							TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT40), " ");
							TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT41), " ");
							ActionControlOfThisRack(TempActoin,0);	
						}	
						break;
					case USER_KEY_RIGHT2:
						if((4+(GetSinglePageValue()*6)) > ShowAuxiliaryAction[0].MAX)
							break;
						sige[4] = 1;
						TempActoin.Action = ShowAuxiliaryAction[3+((GetSinglePageValue()*6))];
						if(TempActoin.Action.type == Combin)
						{
							GUIGetDateFunAPI(API_Comblnation_Action,TempActoin.Action.id,&BaseParamType,0);
							if((BaseParamType.baseParam.controlMode != 0) && (BaseParamType.baseParam.controlMode != 1))
							{
								return;
							}
							CombinedActoin.Action = TempActoin.Action;
							TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT40), CombinedActoin.Action.name);
							TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT41), "=请按启动键=");
						}
						else if(TempActoin.Action.type == Sing)
						{
							
							GUIGetDateFunAPI(API_Singel_Action,TempActoin.Action.id,&ActionParamType,0);
							if((ActionParamType.actionParam.controlMode != 0) && (ActionParamType.actionParam.controlMode != 1))//
							{
								return;
							}
							memset(&CombinedActoin,0,sizeof(CombinedActoin));
							TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT40), " ");
							TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT41), " ");
							ActionControlOfThisRack(TempActoin,0);	
						}	
						break;
					case USER_KEY_RIGHT3:
						if((6+(GetSinglePageValue()*6)) > ShowAuxiliaryAction[0].MAX)
							break;
						sige[5] = 1;
						TempActoin.Action = ShowAuxiliaryAction[5+((GetSinglePageValue()*6))];
						if(TempActoin.Action.type == Combin)
						{
							GUIGetDateFunAPI(API_Comblnation_Action,TempActoin.Action.id,&BaseParamType,0);
							if((BaseParamType.baseParam.controlMode != 0) && (BaseParamType.baseParam.controlMode != 1))
							{
								return;
							}
							CombinedActoin.Action = TempActoin.Action;
							TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT40), CombinedActoin.Action.name);
							TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT41), "=请按启动键=");
						}
						else if(TempActoin.Action.type == Sing)
						{
							
							GUIGetDateFunAPI(API_Singel_Action,TempActoin.Action.id,&ActionParamType,0);
							if((ActionParamType.actionParam.controlMode != 0) && (ActionParamType.actionParam.controlMode != 1))//
							{
								return;
							}
							memset(&CombinedActoin,0,sizeof(CombinedActoin));
							TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT40), " ");
							TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT41), " ");
							ActionControlOfThisRack(TempActoin,0);	
						}	
						break;
					case USER_KEY_2:
						TempActoin.Action = ShowKeyboardAction[0][0];
						GUIGetDateFunAPI(API_Singel_Action,TempActoin.Action.id,&ActionParamType,0);
						if((ActionParamType.actionParam.controlMode != 0) && (ActionParamType.actionParam.controlMode != 1))//
						{
							;
						}
						else
						{
							memset(&CombinedActoin,0,sizeof(CombinedActoin));
							TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT40), " ");
							TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT41), " ");
							ActionControlOfThisRack(TempActoin,0);	
						}
						TempActoin.Action = ShowKeyboardAction[0][1];
						GUIGetDateFunAPI(API_Singel_Action,TempActoin.Action.id,&ActionParamType,0);
						if((ActionParamType.actionParam.controlMode != 0) && (ActionParamType.actionParam.controlMode != 1))//
						{
							;
						}
						else
						{
							memset(&CombinedActoin,0,sizeof(CombinedActoin));
							TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT40), " ");
							TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT41), " ");
							ActionControlOfThisRack(TempActoin,0);	
						}
						break;	
					case USER_KEY_3:
						TempActoin.Action = ShowKeyboardAction[1][0];
						GUIGetDateFunAPI(API_Singel_Action,TempActoin.Action.id,&ActionParamType,0);
						if((ActionParamType.actionParam.controlMode != 0) && (ActionParamType.actionParam.controlMode != 1))//
						{
							;
						}
						else
						{
							memset(&CombinedActoin,0,sizeof(CombinedActoin));
							TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT40), " ");
							TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT41), " ");
							ActionControlOfThisRack(TempActoin,0);	
						}
						TempActoin.Action = ShowKeyboardAction[1][1];
						GUIGetDateFunAPI(API_Singel_Action,TempActoin.Action.id,&ActionParamType,0);
						if((ActionParamType.actionParam.controlMode != 0) && (ActionParamType.actionParam.controlMode != 1))//
						{
							;
						}
						else
						{
							memset(&CombinedActoin,0,sizeof(CombinedActoin));
							TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT40), " ");
							TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT41), " ");
							ActionControlOfThisRack(TempActoin,0);	
						}
						break;	
					case USER_KEY_5:
						TempActoin.Action = ShowKeyboardAction[2][0];
						GUIGetDateFunAPI(API_Singel_Action,TempActoin.Action.id,&ActionParamType,0);
						if((ActionParamType.actionParam.controlMode != 0) && (ActionParamType.actionParam.controlMode != 1))//
						{
							;
						}
						else
						{
							memset(&CombinedActoin,0,sizeof(CombinedActoin));
							TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT40), " ");
							TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT41), " ");
							ActionControlOfThisRack(TempActoin,0);	
						}
						TempActoin.Action = ShowKeyboardAction[2][1];
						GUIGetDateFunAPI(API_Singel_Action,TempActoin.Action.id,&ActionParamType,0);
						if((ActionParamType.actionParam.controlMode != 0) && (ActionParamType.actionParam.controlMode != 1))//
						{
							;
						}
						else
						{
							memset(&CombinedActoin,0,sizeof(CombinedActoin));
							TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT40), " ");
							TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT41), " ");
							ActionControlOfThisRack(TempActoin,0);	
						}
						break;	
					case USER_KEY_6:
						TempActoin.Action = ShowKeyboardAction[3][0];					
						GUIGetDateFunAPI(API_Singel_Action,TempActoin.Action.id,&ActionParamType,0);
						if((ActionParamType.actionParam.controlMode != 0) && (ActionParamType.actionParam.controlMode != 1))//
						{
							;
						}
						else
						{
							memset(&CombinedActoin,0,sizeof(CombinedActoin));
							TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT40), " ");
							TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT41), " ");
							ActionControlOfThisRack(TempActoin,0);	
						}
						TempActoin.Action = ShowKeyboardAction[3][1];
						GUIGetDateFunAPI(API_Singel_Action,TempActoin.Action.id,&ActionParamType,0);
						if((ActionParamType.actionParam.controlMode != 0) && (ActionParamType.actionParam.controlMode != 1))//
						{
							;
						}
						else
						{
							memset(&CombinedActoin,0,sizeof(CombinedActoin));
							TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT40), " ");
							TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT41), " ");
							ActionControlOfThisRack(TempActoin,0);	
						}
						break;	
					case USER_KEY_8:
						TempActoin.Action = ShowKeyboardAction[4][0];
						GUIGetDateFunAPI(API_Singel_Action,TempActoin.Action.id,&ActionParamType,0);
						if((ActionParamType.actionParam.controlMode != 0) && (ActionParamType.actionParam.controlMode != 1))//
						{
							;
						}
						else
						{
							memset(&CombinedActoin,0,sizeof(CombinedActoin));
							TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT40), " ");
							TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT41), " ");
							ActionControlOfThisRack(TempActoin,0);	
						}
						TempActoin.Action = ShowKeyboardAction[4][1];
						GUIGetDateFunAPI(API_Singel_Action,TempActoin.Action.id,&ActionParamType,0);
						if((ActionParamType.actionParam.controlMode != 0) && (ActionParamType.actionParam.controlMode != 1))//
						{
							;
						}
						else
						{
							memset(&CombinedActoin,0,sizeof(CombinedActoin));
							TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT40), " ");
							TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT41), " ");
							ActionControlOfThisRack(TempActoin,0);	
						}
						break;	
					case USER_KEY_9:
						TempActoin.Action = ShowKeyboardAction[5][0];
						GUIGetDateFunAPI(API_Singel_Action,TempActoin.Action.id,&ActionParamType,0);
						if((ActionParamType.actionParam.controlMode != 0) && (ActionParamType.actionParam.controlMode != 1))//
						{
							;
						}
						else
						{
							memset(&CombinedActoin,0,sizeof(CombinedActoin));
							TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT40), " ");
							TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT41), " ");
							ActionControlOfThisRack(TempActoin,0);	
						}
						TempActoin.Action = ShowKeyboardAction[5][1];
						GUIGetDateFunAPI(API_Singel_Action,TempActoin.Action.id,&ActionParamType,0);
						if((ActionParamType.actionParam.controlMode != 0) && (ActionParamType.actionParam.controlMode != 1))//
						{
							;
						}
						else
						{
							memset(&CombinedActoin,0,sizeof(CombinedActoin));
							TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT40), " ");
							TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT41), " ");
							ActionControlOfThisRack(TempActoin,0);	
						}
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
						if(TempActoin.Action.type == Combin)
						{
							;
						}
						else if(TempActoin.Action.type == Sing)
						{
							
							GUIGetDateFunAPI(API_Singel_Action,TempActoin.Action.id,&ActionParamType,0);
							if((ActionParamType.actionParam.controlMode != 0) && (ActionParamType.actionParam.controlMode != 1))//
							{
								return;
							}
							memset(&CombinedActoin,0,sizeof(CombinedActoin));
							TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT40), " ");
							TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT41), " ");
							ActionControlOfThisRack(TempActoin,1);	
						}							
						break;
					case USER_KEY_LEFT2:
						if((3+(GetSinglePageValue()*6)) > ShowAuxiliaryAction[0].MAX)
							break;
						sige[1] = 0;
						TempActoin.Action = ShowAuxiliaryAction[2+((GetSinglePageValue()*6))];
						if(TempActoin.Action.type == Combin)
						{
							;
						}
						else if(TempActoin.Action.type == Sing)
						{
							
							GUIGetDateFunAPI(API_Singel_Action,TempActoin.Action.id,&ActionParamType,0);
							if((ActionParamType.actionParam.controlMode != 0) && (ActionParamType.actionParam.controlMode != 1))//
							{
								return;
							}
							memset(&CombinedActoin,0,sizeof(CombinedActoin));
							TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT40), " ");
							TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT41), " ");
							ActionControlOfThisRack(TempActoin,1);	
						}	
						break;
					case USER_KEY_LEFT3:
						if((5+(GetSinglePageValue()*6)) > ShowAuxiliaryAction[0].MAX)
							break;
						sige[2] = 0;
						TempActoin.Action = ShowAuxiliaryAction[4+((GetSinglePageValue()*6))];
						if(TempActoin.Action.type == Combin)
						{
							;
						}
						else if(TempActoin.Action.type == Sing)
						{
							
							GUIGetDateFunAPI(API_Singel_Action,TempActoin.Action.id,&ActionParamType,0);
							if((ActionParamType.actionParam.controlMode != 0) && (ActionParamType.actionParam.controlMode != 1))//
							{
								return;
							}
							memset(&CombinedActoin,0,sizeof(CombinedActoin));
							TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT40), " ");
							TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT41), " ");
							ActionControlOfThisRack(TempActoin,1);	
						}	
						break;
					case USER_KEY_RIGHT1:
						if((2+(GetSinglePageValue()*6)) > ShowAuxiliaryAction[0].MAX)
							break;
						sige[3] = 0;
						TempActoin.Action = ShowAuxiliaryAction[1+((GetSinglePageValue()*6))];
						if(TempActoin.Action.type == Combin)
						{
							;
						}
						else if(TempActoin.Action.type == Sing)
						{
							
							GUIGetDateFunAPI(API_Singel_Action,TempActoin.Action.id,&ActionParamType,0);
							if((ActionParamType.actionParam.controlMode != 0) && (ActionParamType.actionParam.controlMode != 1))//
							{
								return;
							}
							memset(&CombinedActoin,0,sizeof(CombinedActoin));
							TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT40), " ");
							TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT41), " ");
							ActionControlOfThisRack(TempActoin,1);	
						}	
						break;
					case USER_KEY_RIGHT2:
						if((4+(GetSinglePageValue()*6)) > ShowAuxiliaryAction[0].MAX)
							break;
						sige[4] = 0;
						TempActoin.Action = ShowAuxiliaryAction[3+((GetSinglePageValue()*6))];
						if(TempActoin.Action.type == Combin)
						{
							;
						}
						else if(TempActoin.Action.type == Sing)
						{
							
							GUIGetDateFunAPI(API_Singel_Action,TempActoin.Action.id,&ActionParamType,0);
							if((ActionParamType.actionParam.controlMode != 0) && (ActionParamType.actionParam.controlMode != 1))//
							{
								return;
							}
							memset(&CombinedActoin,0,sizeof(CombinedActoin));
							TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT40), " ");
							TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT41), " ");
							ActionControlOfThisRack(TempActoin,1);	
						}	
						break;
					case USER_KEY_RIGHT3:
						if((6+(GetSinglePageValue()*6)) > ShowAuxiliaryAction[0].MAX)
							break;
						sige[5] = 0;
						TempActoin.Action = ShowAuxiliaryAction[5+((GetSinglePageValue()*6))];
						if(TempActoin.Action.type == Combin)
						{
							;
						}
						else if(TempActoin.Action.type == Sing)
						{
							
							GUIGetDateFunAPI(API_Singel_Action,TempActoin.Action.id,&ActionParamType,0);
							if((ActionParamType.actionParam.controlMode != 0) && (ActionParamType.actionParam.controlMode != 1))//
							{
								return;
							}
							memset(&CombinedActoin,0,sizeof(CombinedActoin));
							TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT40), " ");
							TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT41), " ");
							ActionControlOfThisRack(TempActoin,1);	
						}	
						break;
					case USER_KEY_2:
						TempActoin.Action = ShowKeyboardAction[0][0];
						GUIGetDateFunAPI(API_Singel_Action,TempActoin.Action.id,&ActionParamType,0);
						if((ActionParamType.actionParam.controlMode != 0) && (ActionParamType.actionParam.controlMode != 1))//
						{
							;
						}
						else
						{
							memset(&CombinedActoin,0,sizeof(CombinedActoin));
							TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT40), " ");
							TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT41), " ");
							ActionControlOfThisRack(TempActoin,1);	
						}
						TempActoin.Action = ShowKeyboardAction[0][1];
						GUIGetDateFunAPI(API_Singel_Action,TempActoin.Action.id,&ActionParamType,0);
						if((ActionParamType.actionParam.controlMode != 0) && (ActionParamType.actionParam.controlMode != 1))//
						{
							;
						}
						else
						{
							memset(&CombinedActoin,0,sizeof(CombinedActoin));
							TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT40), " ");
							TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT41), " ");
							ActionControlOfThisRack(TempActoin,1);	
						}
						break;	
					case USER_KEY_3:
						TempActoin.Action = ShowKeyboardAction[1][0];
						GUIGetDateFunAPI(API_Singel_Action,TempActoin.Action.id,&ActionParamType,0);
						if((ActionParamType.actionParam.controlMode != 0) && (ActionParamType.actionParam.controlMode != 1))//
						{
							;
						}
						else
						{
							memset(&CombinedActoin,0,sizeof(CombinedActoin));
							TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT40), " ");
							TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT41), " ");
							ActionControlOfThisRack(TempActoin,1);	
						}
						TempActoin.Action = ShowKeyboardAction[1][1];
						GUIGetDateFunAPI(API_Singel_Action,TempActoin.Action.id,&ActionParamType,0);
						if((ActionParamType.actionParam.controlMode != 0) && (ActionParamType.actionParam.controlMode != 1))//
						{
							;
						}
						else
						{
							memset(&CombinedActoin,0,sizeof(CombinedActoin));
							TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT40), " ");
							TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT41), " ");
							ActionControlOfThisRack(TempActoin,1);	
						}
						break;	
					case USER_KEY_5:
						TempActoin.Action = ShowKeyboardAction[2][0];
						GUIGetDateFunAPI(API_Singel_Action,TempActoin.Action.id,&ActionParamType,0);
						if((ActionParamType.actionParam.controlMode != 0) && (ActionParamType.actionParam.controlMode != 1))//
						{
							;
						}
						else
						{
							memset(&CombinedActoin,0,sizeof(CombinedActoin));
							TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT40), " ");
							TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT41), " ");
							ActionControlOfThisRack(TempActoin,1);	
						}
						TempActoin.Action = ShowKeyboardAction[2][1];
						GUIGetDateFunAPI(API_Singel_Action,TempActoin.Action.id,&ActionParamType,0);
						if((ActionParamType.actionParam.controlMode != 0) && (ActionParamType.actionParam.controlMode != 1))//
						{
							;
						}
						else
						{
							memset(&CombinedActoin,0,sizeof(CombinedActoin));
							TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT40), " ");
							TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT41), " ");
							ActionControlOfThisRack(TempActoin,1);	
						}
						break;	
					case USER_KEY_6:
						TempActoin.Action = ShowKeyboardAction[3][0];					
						GUIGetDateFunAPI(API_Singel_Action,TempActoin.Action.id,&ActionParamType,0);
						if((ActionParamType.actionParam.controlMode != 0) && (ActionParamType.actionParam.controlMode != 1))//
						{
							;
						}
						else
						{
							memset(&CombinedActoin,0,sizeof(CombinedActoin));
							TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT40), " ");
							TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT41), " ");
							ActionControlOfThisRack(TempActoin,1);	
						}
						TempActoin.Action = ShowKeyboardAction[3][1];
						GUIGetDateFunAPI(API_Singel_Action,TempActoin.Action.id,&ActionParamType,0);
						if((ActionParamType.actionParam.controlMode != 0) && (ActionParamType.actionParam.controlMode != 1))//
						{
							;
						}
						else
						{
							memset(&CombinedActoin,0,sizeof(CombinedActoin));
							TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT40), " ");
							TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT41), " ");
							ActionControlOfThisRack(TempActoin,1);	
						}
						break;	
					case USER_KEY_8:
						TempActoin.Action = ShowKeyboardAction[4][0];
						GUIGetDateFunAPI(API_Singel_Action,TempActoin.Action.id,&ActionParamType,0);
						if((ActionParamType.actionParam.controlMode != 0) && (ActionParamType.actionParam.controlMode != 1))//
						{
							;
						}
						else
						{
							memset(&CombinedActoin,0,sizeof(CombinedActoin));
							TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT40), " ");
							TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT41), " ");
							ActionControlOfThisRack(TempActoin,1);	
						}
						TempActoin.Action = ShowKeyboardAction[4][1];
						GUIGetDateFunAPI(API_Singel_Action,TempActoin.Action.id,&ActionParamType,0);
						if((ActionParamType.actionParam.controlMode != 0) && (ActionParamType.actionParam.controlMode != 1))//
						{
							;
						}
						else
						{
							memset(&CombinedActoin,0,sizeof(CombinedActoin));
							TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT40), " ");
							TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT41), " ");
							ActionControlOfThisRack(TempActoin,1);	
						}
						break;	
					case USER_KEY_9:
						TempActoin.Action = ShowKeyboardAction[5][0];
						GUIGetDateFunAPI(API_Singel_Action,TempActoin.Action.id,&ActionParamType,0);
						if((ActionParamType.actionParam.controlMode != 0) && (ActionParamType.actionParam.controlMode != 1))//
						{
							;
						}
						else
						{
							memset(&CombinedActoin,0,sizeof(CombinedActoin));
							TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT40), " ");
							TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT41), " ");
							ActionControlOfThisRack(TempActoin,1);	
						}
						TempActoin.Action = ShowKeyboardAction[5][1];
						GUIGetDateFunAPI(API_Singel_Action,TempActoin.Action.id,&ActionParamType,0);
						if((ActionParamType.actionParam.controlMode != 0) && (ActionParamType.actionParam.controlMode != 1))//
						{
							;
						}
						else
						{
							memset(&CombinedActoin,0,sizeof(CombinedActoin));
							TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT40), " ");
							TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT41), " ");
							ActionControlOfThisRack(TempActoin,1);	
						}
						break;					
					default:
						break;			
				}
			}
			break;		
		case WM_TIMER:	
			if(pMsg->hWin != WM_GetFocussedWindow())
				WM_SetFocus(pMsg->hWin);
			OvertimeReturn(pMsg);
			Determineneedtoreturninterface(pMsg,0);
			GUIGetDateFunAPI(API_Support_Statur,0,&temp,&length);
			if(temp.slaveMode == 1)
			{
				GUI_EndDialog(pMsg->hWin, 0);
				CreateSlveContor();
			}
			if(GetAuxiliarySige())
			{
				ChangeInfoButton(BackGround_hWin,3);
				ResetAuxiliarySigereturn();
			}			
			WM_RestartTimer(RackContor_hTimer,100);
			break;
		case WM_PAINT:
			GUI_MEMDEV_WriteAt(HomeFrame_DEV[1], 65, 50);
			GUI_MEMDEV_WriteAt(Group_DEV[2], 60, 270);
//			GUI_MEMDEV_WriteAt(PassWord_DEV[1], 60+49, 285);
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
WM_HWIN CreateRackContor(void) 
{		
	RackContor_hWin = WM_CreateWindowAsChild(60,0,360,320,BackGround_hWin,WM_CF_SHOW|WM_CF_HASTRANS,RackContor_cbDialog,0);
	
	RackContor_hTimer = WM_CreateTimer(WM_GetClientWindow(RackContor_hWin), 0, 10, 0);
	CurrentWindow = rackwin;
	return RackContor_hWin;
}
