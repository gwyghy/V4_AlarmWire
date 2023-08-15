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
#include "udp_app_proc.h"
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
extern uint16_t FollowButton[];
extern u8  NumberOfOperations;
extern GUI_MEMDEV_Handle 	PowerOn_DEV,
							PassWord_DEV[2],
							HomeFrame_DEV[5],
							Group_DEV[5];
					
WM_HTIMER 	FollowContor_hTimer;
			
			
WM_HWIN 	FollowContor_hWin;
		
		
static void FollowContor_cbDialog(WM_MESSAGE * pMsg) 
{
	char buf[15];
	static uint8_t follow[5];
	switch (pMsg->MsgId) 
	{
		case WM_CREATE:
			SendLEDNavigationData(KEY_PASS,KEY_ALL);
			OvertimeOpen();
			ChangeInfoButton(BackGround_hWin,5);
			Setstheinitialstate();
			TEXT_CreateEx(15, 125, 330, 50,pMsg->hWin,WM_CF_SHOW,TEXT_CF_HCENTER|TEXT_CF_VCENTER,GUI_ID_TEXT30," ");
			TEXT_CreateEx(15, 125+50, 330, 50,pMsg->hWin,WM_CF_SHOW,TEXT_CF_HCENTER|TEXT_CF_VCENTER,GUI_ID_TEXT31," ");
			TEXT_CreateEx(15, 125+100, 330, 50,pMsg->hWin,WM_CF_SHOW,TEXT_CF_HCENTER|TEXT_CF_VCENTER,GUI_ID_TEXT32," ");
			TEXT_SetFont(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT30), &GUI_FontHZ32);
			TEXT_SetFont(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT31), &GUI_FontHZ32);
			TEXT_SetFont(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT32), &GUI_FontHZ32);
			
			TEXT_SetTextColor(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT30),GUI_WHITE);
			TEXT_SetTextColor(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT31),GUI_WHITE);
			TEXT_SetTextColor(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT32),GUI_LIGHTYELLOW);
		
			TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT32), "=请选择工序=");			
			memset(follow,0,5);
			WM_SetFocus(pMsg->hWin);
			break;
		case MSG_CHANGE_FOLLOW:
			if(pMsg ->Data.v  == 0) 
			{
				TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT30), "跟机方向:←");
				follow[4] = 1;
			}
			else if(pMsg ->Data.v  == 1)
			{
				TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT30), "跟机方向:→");
				follow[4] = 2;
			}
			else
				;
			break;	
		case WM_TIMER:
			if(pMsg->hWin != WM_GetFocussedWindow())
				WM_SetFocus(pMsg->hWin);
			OvertimeReturn(pMsg);
			Determineneedtoreturninterface(pMsg,1);
			WM_RestartTimer(FollowContor_hTimer,500);
			break;	
		case WM_KEY:
			SendLEDNavigationData(KEY_PASS,KEY_ALL);
			if((((WM_KEY_INFO*)(pMsg->Data.p))->PressedCnt) > 0) //按键按下
			{
				ChangeInfoKey(BackGround_hWin,((WM_KEY_INFO*)(pMsg->Data.p))->Key,1);
				switch (((WM_KEY_INFO*)(pMsg->Data.p))->Key) 
				{ 
					case USER_KEY_ESC:
						GUI_EndDialog(pMsg->hWin, 0);
						CreateHomeFrame();				
						break;
					case USER_KEY_ENTER:
						if(((follow[1] != 2) && (follow[1] != 3)) && (follow[2] == 0) && (follow[3] == 0))
							break;
						follow[0] = 0;
						UdpLinkLayerSend_app(follow,5,0,0 ,0,AppProcIDType_followControl_request);	
						GUI_EndDialog(pMsg->hWin, 0);
						CreateHomeFrame();							
						break;
					case USER_KEY_WORK:	
						memset(buf,0,15);
						memset(&follow[2],0,2);
						follow[1] = 2;
						sprintf(buf,"%s","关闭自动跟机");	
						TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT31), buf);
						TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT32), "=按启动键关闭跟机=");			
					break;	

					case USER_KEY_ASSIST:	
						memset(buf,0,15);
						memset(&follow[2],0,2);
						follow[1] = 3;
						sprintf(buf,"%s","暂停自动跟机");	
						TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT31), buf);
						TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT32), "=按启动键关闭跟机=");			
					break;						
						
					case USER_KEY_LEFT1:	
						if((NumberOfOperations <= 0) || (NumberOfOperations == 0xFF))
							break;
						memset(buf,0,15);
						memcpy(&follow[2],&FollowButton[0],2);
						follow[1] = 1;
						sprintf(buf,"%s%d","当前工序",FollowButton[0]);	
						TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT31), buf);
						TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT32), "=按启动键启动跟机=");			
					break;	
					case USER_KEY_LEFT2:
						if((NumberOfOperations <= 2)|| (NumberOfOperations == 0xFF))
							break;
						memset(buf,0,15);
						memcpy(&follow[2],&FollowButton[2],2);
						sprintf(buf,"%s%d","当前工序",FollowButton[2]);	
						follow[1] = 1;
						TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT31), buf);
						TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT32), "=按启动键启动跟机=");
					break;	
					case USER_KEY_LEFT3:
						if((NumberOfOperations <= 4)|| (NumberOfOperations == 0xFF))
							break;
						memset(buf,0,15);
						memcpy(&follow[2],&FollowButton[4],2);
						sprintf(buf,"%s%d","当前工序",FollowButton[4]);	
						follow[1] = 1;
						TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT31), buf);
						TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT32), "=按启动键启动跟机=");
					break;	
					case USER_KEY_RIGHT1:
						if((NumberOfOperations <= 1)|| (NumberOfOperations == 0xFF))
							break;						
						memset(buf,0,15);
						memcpy(&follow[2],&FollowButton[1],2);
						sprintf(buf,"%s%d","当前工序",FollowButton[1]);
						follow[1] = 1;					
						TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT31), buf);
						TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT32), "=按启动键启动跟机=");
					break;	
					case USER_KEY_RIGHT2:
						if((NumberOfOperations <= 3)|| (NumberOfOperations == 0xFF))
							break;						
						memset(buf,0,15);
						memcpy(&follow[2],&FollowButton[3],2);
						sprintf(buf,"%s%d","当前工序",FollowButton[3]);	
						follow[1] = 1;
						TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT31), buf);
						TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT32), "=按启动键启动跟机=");
					break;	
					case USER_KEY_RIGHT3:	
						if((NumberOfOperations <= 5)|| (NumberOfOperations == 0xFF))
							break;
						memset(buf,0,15);
						memcpy(&follow[2],&FollowButton[5],2);
						sprintf(buf,"%s%d","当前工序",FollowButton[5]);	
						follow[1] = 1;
						TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT31), buf);
						TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT32), "=按启动键启动跟机=");
					break;	
				}
				
			}
			else
			{
				ChangeInfoKey(BackGround_hWin,((WM_KEY_INFO*)(pMsg->Data.p))->Key,0);
			}	
		case WM_PAINT:		
			GUI_MEMDEV_WriteAt(PowerOn_DEV, 109, 25);
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
WM_HWIN CreateFollowContor(void) 
{		
	FollowContor_hWin = WM_CreateWindowAsChild(60,0,360,320,BackGround_hWin,WM_CF_SHOW|WM_CF_HASTRANS,FollowContor_cbDialog,0);
	CurrentWindow = followwin;
	FollowContor_hTimer = WM_CreateTimer(WM_GetClientWindow(FollowContor_hWin), 0, 10, 0);
	return FollowContor_hWin;
}
