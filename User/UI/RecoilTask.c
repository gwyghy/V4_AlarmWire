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
//extern uint16_t RecoilButton[];
//extern u8  NumberOfOperations;
extern GUI_MEMDEV_Handle 	PowerOn_DEV,
							PassWord_DEV[2],
							HomeFrame_DEV[5],
							Group_DEV[5];
					
WM_HTIMER 	RecoilContor_hTimer;
			
			
WM_HWIN 	RecoilContor_hWin;
		
		
static void RecoilContor_cbDialog(WM_MESSAGE * pMsg) 
{
	static uint8_t Recoil;
	switch (pMsg->MsgId) 
	{
		case WM_CREATE:
			SendLEDNavigationData(KEY_RECOIL,KEY_ALL);
			OvertimeOpen();
			ChangeInfoButton(BackGround_hWin,6);
			Setstheinitialstate();
//			TEXT_CreateEx(15, 125, 330, 50,pMsg->hWin,WM_CF_SHOW,TEXT_CF_HCENTER|TEXT_CF_VCENTER,GUI_ID_TEXT30," ");
			TEXT_CreateEx(15, 125+100, 330, 50,pMsg->hWin,WM_CF_SHOW,TEXT_CF_HCENTER|TEXT_CF_VCENTER,GUI_ID_TEXT31," ");
			TEXT_CreateEx(15, 125+50, 330, 50,pMsg->hWin,WM_CF_SHOW,TEXT_CF_HCENTER|TEXT_CF_VCENTER,GUI_ID_TEXT32," ");
//			TEXT_SetFont(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT30), &GUI_FontHZ32);
			TEXT_SetFont(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT31), &GUI_FontHZ32);
			TEXT_SetFont(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT32), &GUI_FontHZ32);
			
//			TEXT_SetTextColor(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT30),GUI_WHITE);
			TEXT_SetTextColor(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT31),GUI_WHITE);
			TEXT_SetTextColor(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT32),GUI_LIGHTYELLOW);
		
			TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT32), "=开启自动反冲洗=");	
			TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT31), "=请按启动键=");		
			Recoil = 1;
			WM_SetFocus(pMsg->hWin);
			break;
		case WM_TIMER:
			if(pMsg->hWin != WM_GetFocussedWindow())
				WM_SetFocus(pMsg->hWin);
			OvertimeReturn(pMsg);
			Determineneedtoreturninterface(pMsg,1);
			WM_RestartTimer(RecoilContor_hTimer,500);
			break;	
		case WM_KEY:
			SendLEDNavigationData(KEY_RECOIL,KEY_ALL);
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
						UdpLinkLayerSend_app(&Recoil,2,0,0 ,0,AppProcIDType_RecoilControl_request);	
						GUI_EndDialog(pMsg->hWin, 0);
						CreateHomeFrame();							
						break;						
					case USER_KEY_LEFT1:	
						Recoil = 1;
						TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT32), "=开启自动反冲洗=");			
					break;	
					case USER_KEY_LEFT2:
						Recoil = 2;
						TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT32), "=关闭自动反冲洗=");
					break;	
				}
				
			}
			else
			{
				ChangeInfoKey(BackGround_hWin,((WM_KEY_INFO*)(pMsg->Data.p))->Key,0);
			}	
		case WM_PAINT:		
			GUI_MEMDEV_WriteAt(PowerOn_DEV, 109, 25);
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
WM_HWIN CreateRecoilContor(void) 
{		
	RecoilContor_hWin = WM_CreateWindowAsChild(60,0,360,320,BackGround_hWin,WM_CF_SHOW|WM_CF_HASTRANS,RecoilContor_cbDialog,0);
	RecoilContor_hTimer = WM_CreateTimer(WM_GetClientWindow(RecoilContor_hWin), 0, 10, 0);
	return RecoilContor_hWin;
}
