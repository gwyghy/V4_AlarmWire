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
WM_HWIN Menu_hDlg;
WM_HTIMER hTimerMenu,Warningone_hWin;
extern GUI_MEMDEV_Handle  	BackGround_DEV,SuddenStop_DEV;
extern uint16_t Send_parameter_flag;
/*
*********************************************************************************************************
*	                       GUI_WIDGET_CREATE_INFO类型数组
*********************************************************************************************************
*/
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = 
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
uint8_t  MAXbutton = 6;
extern uint8_t  assistflag;
/*
*********************************************************************************************************
*	函 数 名: _cbDialog
*	功能说明: 对话框的回调函数
*	形    参: pMsg 消息指针
*	返 回 值: 无
*********************************************************************************************************
*/
static void _cbDialog(WM_MESSAGE * pMsg) 
{
	char buf[20];
	globalStateType  temp;
	static uint8_t frist;
	static uint8_t start;
	uint8_t	rank[LEVEL_MAX] = {0};
	static uint64_t  id,idback;
	static uint64_t  leve,leveold;
	static uint16_t page,pageold;
	switch (pMsg->MsgId) 
	{
		case WM_INIT_DIALOG:		
			//初始化列表控件
			SendLEDNavigationData(0,KEY_ALL);
			MAXbutton = 6;
			Setstheinitialstate();
			OvertimeOpen();
			Creator_Menuwindow(pMsg);
			idback = 0;
			frist = 0;
			start = 0;
			break;
		case WM_PAINT:
			GUI_MEMDEV_WriteAt(BackGround_DEV, 0, 0);
			break;
		case WM_TIMER:
			if(pMsg->hWin != WM_GetFocussedWindow())
				WM_SetFocus(pMsg->hWin);
			OvertimeReturn(pMsg);
			GUIGetDateFunAPI(API_Support_Statur,0,&temp,0);
			if(Send_parameter_flag ==1)
			{
				if(frist == 0)
				{
					frist = 1;
					CreateWarningone();
					TEXT_CreateEx(60,105,360,50,pMsg->hWin,WM_CF_SHOW,TEXT_CF_HCENTER|TEXT_CF_VCENTER,GUI_ID_TEXT23," ");
					TEXT_SetTextColor(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT23),GUI_WHITE);
					TEXT_SetFont(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT23), &GUI_FontHZ32);
					TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT23),"正在发送:0%");
					start = 1;	
					OvertimeClose();				
				}
				else
				{
					if(sendTimerAPI() != 100)
					{
						memset(buf,0,sizeof(buf));
						sprintf(buf,"正在发送:%d%%",sendTimerAPI());
						TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT23),buf);
					}
					else
					{
						GUI_EndDialog(Warningone_hWin, 0);
						WM_DeleteWindow(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT23));
						start = 0;
						frist = 0;
						Send_parameter_flag = 0;
						OvertimeOpen();
					}
				}			
			}
			else
			{
				if(temp.slaveMode == 2)
				{				
					if(frist == 0)
					{
						frist = 1;
						CreateWarningone();
						TEXT_CreateEx(60,105,360,50,pMsg->hWin,WM_CF_SHOW,TEXT_CF_HCENTER|TEXT_CF_VCENTER,GUI_ID_TEXT23," ");
						TEXT_SetTextColor(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT23),GUI_WHITE);
						TEXT_SetFont(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT23), &GUI_FontHZ32);
						TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT23),"正在配址，请勿动");
						start = 1;					
					}
				}
				else if(temp.slaveMode == 3)
				{
					GUI_EndDialog(Warningone_hWin, 0);
					WM_DeleteWindow(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT23));
					SetSysFuncMode(SYSMODE_NORMAL);
					start = 0;
					frist = 0;
				}
				else if(temp.slaveMode == 1)
				{
					GUI_EndDialog(pMsg->hWin, 0);
					CreateBackGround();
				}
				else
					Determineneedtoreturninterface(pMsg,1);
			}
			
			WM_RestartTimer(hTimerMenu,100);
            break;
		case WM_KEY:
			if(start)
				break;
			if((((WM_KEY_INFO*)(pMsg->Data.p))->PressedCnt) > 0) //按键按下
			{
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
					USER_SetValueListviewData(pMsg,((WM_KEY_INFO*)(pMsg->Data.p))->Key,0);
					//参数保存处理
					if(USER_SaveValueListviewData(pMsg,((WM_KEY_INFO*)(pMsg->Data.p))->Key,id,0))
					{						
						USER_ExitListviewRow(pMsg);
						Menu_ShowButtonForPage(pMsg,id);						
					}
				}
				else //开始选择参数
				{				
					Menu_SetCurrentPage(((WM_KEY_INFO*)(pMsg->Data.p))->Key,pMsg);
					//页面不变需不需要刷新???
					id = Getrankvalue(rank);
					leve = GetLevelValue();
					page = GetPageValue();
					if((id != idback) || (leve != leveold) || (page != pageold))
					{
						Menu_ShowButtonForPage(pMsg,id);
						Menu_ShowListviewForPage(pMsg,id);
						if(assistflag !=1)
						{
							Menu_ShowTextForPage(pMsg,id);	
						
						}
						idback = id;
						leveold = leve;
						pageold = page;
						
					}
					assistflag = 0;
					switch (((WM_KEY_INFO*)(pMsg->Data.p))->Key) 
					{ 	
						case USER_KEY_UP:
							USER_MoveListviewRow(pMsg,0);
							break;		
						case USER_KEY_DOWN:
							USER_MoveListviewRow(pMsg,1);
							break;
						case USER_KEY_RIGHT:
							USER_EnterListviewRow(pMsg,0);
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
extern uint8_t  CurrentWindow;
/*
*********************************************************************************************************
*	函 数 名: MainTask
*	功能说明: GUI主函数
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/

void CreateMenuTask(void) 
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
	Menu_hDlg = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), &_cbDialog, 0, 0, 0);
	
	hTimerMenu = WM_CreateTimer(WM_GetClientWindow(Menu_hDlg), 0, 100, 0);
	CurrentWindow = menuwin;
	//	while(1)
//	{
//		GUI_Delay(1);
//	}
}
static void Warningone_cbDialog(WM_MESSAGE * pMsg) 
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
WM_HWIN CreateWarningone(void)
{
	Warningone_hWin = WM_CreateWindowAsChild(60,50,360,163,Menu_hDlg,WM_CF_SHOW|WM_CF_HASTRANS,Warningone_cbDialog,0);
//	Warning_hTimer = WM_CreateTimer(WM_GetClientWindow(Warning_hWin), 0, 100, 0);
	
	return Warningone_hWin;
}
/***************************** 安富莱电子 www.armfly.com (END OF FILE) *********************************/
