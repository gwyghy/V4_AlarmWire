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
WM_HTIMER hTimerMenu;
extern GUI_MEMDEV_Handle  	BackGround_DEV;

/*
*********************************************************************************************************
*	                       GUI_WIDGET_CREATE_INFO类型数组
*********************************************************************************************************
*/
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = 
{
  { WINDOW_CreateIndirect, 	 "Window",   MENU_WINDOW_ID0, 0, 0, 480, 320, 0, 0x0, 0 },
  { TEXT_CreateIndirect, 	 "Text",     MENU_TEXT_ID0, 70, 0, 342, 50, 0, 0x0, 0 },
  { LISTVIEW_CreateIndirect, "Listview", MENU_LISTVIEW_ID0, 59, 50, 362, 270, 0, 0x0, 10 },
  { BUTTON_CreateIndirect,   "",   MENU_BUTTON_LEFT_ID1, 0, 0, 60, 106, 0, 0x0, 0 },
  { BUTTON_CreateIndirect,   "",   MENU_BUTTON_LEFT_ID2, 0, 107, 60, 106, 0, 0x0, 0 },
  { BUTTON_CreateIndirect,   "",   MENU_BUTTON_LEFT_ID3, 0, 214, 60, 106, 0, 0x0, 0 },
  { BUTTON_CreateIndirect,   "",   MENU_BUTTON_RIGHT_ID1, 420, 0, 60, 106, 0, 0x0, 0 },
  { BUTTON_CreateIndirect,   "",   MENU_BUTTON_RIGHT_ID2, 420, 107, 60, 106, 0, 0x0, 0 },
  { BUTTON_CreateIndirect,   "",   MENU_BUTTON_RIGHT_ID3, 420, 214, 60, 106, 0, 0x0, 0 },
};

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
	static uint8_t i;
	uint8_t	rank[LEVEL_MAX] = {0};
	uint16_t temp1;
	static uint64_t  id,idback;
	static uint64_t  leve,leveold;
	WM_HWIN hDlg,hItem;
	switch (pMsg->MsgId) 
	{
		case WM_INIT_DIALOG:		
			//初始化列表控件
			OvertimeOpen();
			Creator_Menuwindow(pMsg);
			idback = 0;
			break;
		case WM_PAINT:
			GUI_MEMDEV_WriteAt(BackGround_DEV, 0, 0);
			break;
		case WM_TIMER:
			if(pMsg->hWin != WM_GetFocussedWindow())
				WM_SetFocus(pMsg->hWin);
			OvertimeReturn(pMsg);
			WM_RestartTimer(hTimerMenu,100);
            break;
		case WM_KEY:
			if((((WM_KEY_INFO*)(pMsg->Data.p))->PressedCnt) > 0) //按键按下
			{
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
					USER_SetValueListviewData(pMsg,((WM_KEY_INFO*)(pMsg->Data.p))->Key);
					//参数保存处理
					if(USER_SaveValueListviewData(pMsg,((WM_KEY_INFO*)(pMsg->Data.p))->Key,id))
					{						
						USER_ExitListviewRow(pMsg);
						Menu_ShowButtonForPage(pMsg,id);						
					}
				}
				else //开始选择参数
				{
					USER_DownButtonShow(pMsg,((WM_KEY_INFO*)(pMsg->Data.p))->Key);
					Menu_SetCurrentPage(((WM_KEY_INFO*)(pMsg->Data.p))->Key);
					//页面不变需不需要刷新???
					id = Getrankvalue(rank);
					leve = GetLevelValue();
					if((id != idback) || (leve != leveold))
					{
						Menu_ShowButtonForPage(pMsg,id);
						Menu_ShowListviewForPage(pMsg,id);
						Menu_ShowTextForPage(pMsg,id);	
						idback = id;
						leveold = leve;
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
							USER_EnterListviewRow(pMsg);
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

void CreateMenuTask(void) 
{
	WM_HWIN hDlg;
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
	hDlg = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), &_cbDialog, 0, 0, 0);
	hTimerMenu = WM_CreateTimer(WM_GetClientWindow(hDlg), 0, 10000, 0);
//	while(1)
//	{
//		GUI_Delay(1);
//	}
}

/***************************** 安富莱电子 www.armfly.com (END OF FILE) *********************************/
