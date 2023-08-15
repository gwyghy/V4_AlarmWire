/*
*********************************************************************************************************
*	                                  
*	模块名称 : GUI界面主函数
*	文件名称 : MainTask.c
*	版    本 : V1.0
*	说    明 : 本实验主要学习外部实体按键操作控件。
*              
*	修改记录 :
*		版本号   日期         作者          说明
*		V1.0    2016-07-16   Eric2013  	    首版    
*                                     
*	Copyright (C), 2015-2020, 安富莱电子 www.armfly.com
*
*********************************************************************************************************
*/
#include "includes.h"
#include "User_GUI.h"
#include "usartapp.h"
#include "sys_data_type.h"
#include "sys_msg_def.h"
#include "FileUpdate.h"
#include "udp_app_proc.h"
/*
*********************************************************************************************************
*	                                     宏定义
*********************************************************************************************************
*/
/*
*********************************************************************************************************
*	                                     列表头定义
*********************************************************************************************************
*/
WM_HWIN ServerMenu_hDlg;
WM_HTIMER ServerhTimerMenu,WarningoneServer_hWin;
extern GUI_MEMDEV_Handle  	BackGround_DEV,SuddenStop_DEV;
extern uint16_t Send_parameter_flag;
extern uint8_t  CurrentWindow;
/*
*********************************************************************************************************
*	                       GUI_WIDGET_CREATE_INFO类型数组
*********************************************************************************************************
*/
static const GUI_WIDGET_CREATE_INFO Server_aDialogCreate[] = 
{
  { WINDOW_CreateIndirect, 	 "Window",   MENU_WINDOW_ID0, 0, 0, 480, 320, 0, 0x0, 0 },
  { TEXT_CreateIndirect, 	 "Text",     MENU_TEXT_ID0, 100, 0, 280, 50, 0, 0x0, 0 },
  { TEXT_CreateIndirect, 	 "Text",     GUI_ID_TEXT1, 390, 23, 25, 18, 0, 0x0, 0 },
  { LISTVIEW_CreateIndirect, "Listview", MENU_LISTVIEW_ID0, 59, 50, 362, 270, 0, 0x0, 10 },
	{ BUTTON_CreateIndirect, " ", MENU_BUTTON_LEFT_ID1, 0, 0, 59, 101, 0, 0x0, 0 },
	{ BUTTON_CreateIndirect, " ", MENU_BUTTON_LEFT_ID2, 0, 106, 59, 110, 0, 0x0, 0 },
	{ BUTTON_CreateIndirect, " ", MENU_BUTTON_LEFT_ID3, 0, 220, 59, 101, 0, 0x0, 0 },
	{ BUTTON_CreateIndirect, " ", MENU_BUTTON_RIGHT_ID1, 422, 0, 59, 101, 0, 0x0, 0 },
	{ BUTTON_CreateIndirect, " ", MENU_BUTTON_RIGHT_ID2, 422, 106, 59, 110, 0, 0x0, 0 },
	{ BUTTON_CreateIndirect, " ", MENU_BUTTON_RIGHT_ID3, 422, 220, 59, 101, 0, 0x0, 0 },	
};
uint8_t  feedback[3];
uint8_t  SERVER_PARAMETERS_SAVED_SUCCESSFULLY;
extern uint8_t  ServerMAXbutton;
extern uint8_t  Serverpagelast[8];
extern uint8_t  levelchange;
extern Displayrefresh   Serviceparameters;
/*
*********************************************************************************************************
*	函 数 名: _cbDialog
*	功能说明: 对话框的回调函数
*	形    参: pMsg 消息指针
*	返 回 值: 无
*********************************************************************************************************
*/
static void Server_cbDialog(WM_MESSAGE * pMsg) 
{
	char buf[20];
	uint8_t tempsave = 0;
	static uint8_t start;
	uint8_t	rank[LEVEL_MAX] = {0};
	static uint64_t  id,idback;
	uint8_t server[9];
	static uint8_t PARAMETER_ACCESS_FLAG = 0;
	static uint8_t PARAMETER_SAVE_FLAG = 0;
	static uint8_t Sendtimes = 0;
	static uint8_t Savetimes = 0;
	switch (pMsg->MsgId) 
	{
		case WM_INIT_DIALOG:		
			//初始化列表控件
			SendLEDNavigationData(0,KEY_ALL);
			Setstheinitialstate();
			OvertimeOpen();		
			ServerCreator_Menuwindow(pMsg);
			memset(Serverpagelast,0,sizeof(Serverpagelast));
			idback = 0;
			start = 0;
			PARAMETER_ACCESS_FLAG = 0;
			PARAMETER_SAVE_FLAG = 0;
			Sendtimes = 0;
			Savetimes = 0;
			SERVER_PARAMETERS_SAVED_SUCCESSFULLY = 0;
			break;
		case WM_PAINT:
			GUI_MEMDEV_WriteAt(BackGround_DEV, 0, 0);
			break;
		case WM_TIMER:
			if(pMsg->hWin != WM_GetFocussedWindow())
				WM_SetFocus(pMsg->hWin);
			OvertimeReturn(pMsg);			
			Determineneedtoreturninterface(pMsg,0);
			if(PARAMETER_SAVE_FLAG)
			{
				start = 1;
				Savetimes++;
				if(Savetimes > 3)
				{
					start = 0;
					PARAMETER_SAVE_FLAG = 0;
					Savetimes = 0;
					RecoveryServerParameters(pMsg);
//					CreateServerWarningone();
//					TEXT_CreateEx(60,105,360,50,pMsg->hWin,WM_CF_SHOW,TEXT_CF_HCENTER|TEXT_CF_VCENTER,GUI_ID_TEXT23," ");
//					TEXT_SetTextColor(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT23),GUI_WHITE);
//					TEXT_SetFont(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT23), &GUI_FontHZ32);
//					TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT23),"服务器参数保存失败");
				}
				if(SERVER_PARAMETERS_SAVED_SUCCESSFULLY == 1)
				{
					start = 0;
					PARAMETER_SAVE_FLAG = 0;
					Savetimes = 0;
				}
				else
				{
					if(SendServerParameters() == 0)
					{
						Savetimes = 3;
					}
				}
			}
			if(PARAMETER_ACCESS_FLAG)
			{
				start = 1;
				Sendtimes++;
				if(Sendtimes > 4)
				{
					start = 2;
					PARAMETER_ACCESS_FLAG = 0;
					Sendtimes = 0;
					memset(feedback,0,sizeof(feedback));
					CreateServerWarningone();
					TEXT_CreateEx(60,105,360,50,pMsg->hWin,WM_CF_SHOW,TEXT_CF_HCENTER|TEXT_CF_VCENTER,GUI_ID_TEXT23," ");
					TEXT_SetTextColor(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT23),GUI_WHITE);
					TEXT_SetFont(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT23), &GUI_FontHZ32);
					TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT23),"远端网络通信异常");//获取服务器参数失败
				}
				if((feedback[0] == 2) && (feedback[1] == 2) && (feedback[2] == 2))
				{
					start = 0;
					Sendtimes = 0;
					PARAMETER_ACCESS_FLAG = 0;
					if(levelchange == 1)
					{
						levelchange = 0;
						if(Serviceparameters.ButtonDisplay.count)
						{
							Serverpagelast[GetLevelValue()] = GetPageValue();					
							SetLevelValue(GetLevelValue()+1);
						}					
					}			
					memset(buf,0,sizeof(buf));
					sprintf(buf,"%d/%d",GetPageValue()+1,GetMAXpage(ServerMAXbutton)+1);
					TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT1),buf);
					ServerMenu_ShowTextForPage(pMsg);
					ServerMenu_ShowButtonForPage(pMsg);
					ServerMenu_ShowListviewForPage(pMsg);					
				}
				else
				{
					if(feedback[0] != 2)
					{
						memcpy(server,&id,8);
						server[8] = 1;
						UdpLinkLayerSend_app(server,9,0,0 ,0,AppProcIDType_serverDisplays_request);
					}
					if(feedback[1] != 2)
					{
						memcpy(server,&id,8);
						server[8] = 2;
						UdpLinkLayerSend_app(server,9,0,0 ,0,AppProcIDType_serverDisplays_request);
					}
					if(feedback[2] != 2)
					{
						memcpy(server,&id,8);
						server[8] = 3;
						UdpLinkLayerSend_app(server,9,0,0 ,0,AppProcIDType_serverDisplays_request);
					}
				}	
			}	
			WM_RestartTimer(ServerhTimerMenu,100);
            break;
		case WM_KEY:

			if((((WM_KEY_INFO*)(pMsg->Data.p))->PressedCnt) > 0) //按键按下
			{
				if(start)
				{
					if(start == 2)
					{
						GUI_EndDialog(pMsg->hWin, 0);
						CreateBackGround();
					}				
					break;
				}				
				USER_DownButtonShow(pMsg,((WM_KEY_INFO*)(pMsg->Data.p))->Key);
				switch (((WM_KEY_INFO*)(pMsg->Data.p))->Key) 
				{ 
					case USER_KEY_ESC:
						GUI_EndDialog(pMsg->hWin, 0);
						CreateBackGround();
					break;
				}				
				if(GetconfigState())//开始设置参数
				{
					id = Getrankvalue(rank);
					USER_SetValueListviewData(pMsg,((WM_KEY_INFO*)(pMsg->Data.p))->Key,1);
					//参数保存处理
					tempsave = USER_SaveValueListviewData(pMsg,((WM_KEY_INFO*)(pMsg->Data.p))->Key,id,1);
					if(tempsave)
					{
						if(tempsave != 2)
							PARAMETER_SAVE_FLAG = 1;	
						USER_ExitListviewRow(pMsg);	
						ServerMenu_ShowButtonForPage(pMsg);						
					}
					WM_RestartTimer(ServerhTimerMenu,100);
				}
				else //开始选择参数
				{				
					ServerMenu_SetCurrentPage(((WM_KEY_INFO*)(pMsg->Data.p))->Key,pMsg);
					//页面不变需不需要刷新???
					id = Getrankvalue(rank);				
					if(id != idback)
					{	
						PARAMETER_ACCESS_FLAG = 1;
						memset(feedback,1,sizeof(feedback));						
						memcpy(server,&id,8);
						server[8] = 1;
						UdpLinkLayerSend_app(server,9,0,0 ,0,AppProcIDType_serverDisplays_request);
						memcpy(server,&id,8);
						server[8] = 2;
						UdpLinkLayerSend_app(server,9,0,0 ,0,AppProcIDType_serverDisplays_request);
						memcpy(server,&id,8);
						server[8] = 3;
						UdpLinkLayerSend_app(server,9,0,0 ,0,AppProcIDType_serverDisplays_request);
						
						idback = id;
						WM_RestartTimer(ServerhTimerMenu,100);
					}
					switch (((WM_KEY_INFO*)(pMsg->Data.p))->Key) 
					{ 	
						case USER_KEY_UP:
							USER_MoveListviewRow(pMsg,0);
							break;		
						case USER_KEY_DOWN:
							USER_MoveListviewRow(pMsg,1);
							break;
						case USER_KEY_RIGHT:
							USER_EnterListviewRow(pMsg,1);
							break;
						default:
							break;			
					}				
				}	
			}
			else //按键抬起
			{
				USER_UpButtonShow(pMsg,((WM_KEY_INFO*)(pMsg->Data.p))->Key);
			}
			break;
		default:
			WM_DefaultProc(pMsg);
			break;
	}
}
/*
*********************************************************************************************************
*	函 数 名: MainTask
*	功能说明: GUI主函数
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/

void CreateServerMenuTask(void) 
{

	/*
	 关于多缓冲和窗口内存设备的设置说明
	   1. 使能多缓冲是调用的如下函数，用户要在LCDConf_Lin_Template.c文件中配置了多缓冲，调用此函数才有效：
		  WM_MULTIBUF_Enable(1);
	   2. 窗口使能使用内存设备是调用函数：WM_SetCreateFlags(WM_CF_MEMDEV);
	   3. 如果emWin的配置多缓冲和窗口内存设备都支持，二选一即可，且务必优先选择使用多缓冲，实际使用
		  STM32F429BIT6 + 32位SDRAM + RGB565/RGB888平台测试，多缓冲可以有效的降低窗口移动或者滑动时的撕裂
		  感，并有效的提高流畅性，通过使能窗口使用内存设备是做不到的。
	   4. 所有emWin例子默认是开启三缓冲。
	*/
//	WM_MULTIBUF_Enable(1);
	ServerMenu_hDlg = GUI_CreateDialogBox(Server_aDialogCreate, GUI_COUNTOF(Server_aDialogCreate), &Server_cbDialog, 0, 0, 0);
	ServerhTimerMenu = WM_CreateTimer(WM_GetClientWindow(ServerMenu_hDlg), 0, 100, 0);
	CurrentWindow = Serverhmenuwin;
}
static void WarningoneServer_cbDialog(WM_MESSAGE * pMsg) 
{
	switch (pMsg->MsgId) 
	{
		case WM_PAINT:
			GUI_MEMDEV_WriteAt(SuddenStop_DEV, 60, 50);
		
			break;
		default:
			WM_DefaultProc(pMsg);
			break;
	}
}
/*
*********************************************************************************************************
*	函 数 名: 
*	功能说明: 创建急停画面
*	形    参: 
*	返 回 值: 
*********************************************************************************************************
*/
WM_HWIN CreateServerWarningone(void)
{
	WarningoneServer_hWin = WM_CreateWindowAsChild(60,50,360,163,ServerMenu_hDlg,WM_CF_SHOW|WM_CF_HASTRANS,WarningoneServer_cbDialog,0);
//	Warning_hTimer = WM_CreateTimer(WM_GetClientWindow(Warning_hWin), 0, 100, 0);
	return WarningoneServer_hWin;
}
/***************************** 安富莱电子 www.armfly.com (END OF FILE) *********************************/
