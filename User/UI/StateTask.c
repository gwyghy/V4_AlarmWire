/*
*********************************************************************************************************
*	                                  
*	ģ������ : GUI����������
*	�ļ����� : MainTask.c
*	��    �� : V1.0
*	˵    �� : ��ʵ����Ҫѧϰ�ⲿʵ�尴�������ؼ���
*              
*	�޸ļ�¼ :
*		�汾��   ����         ����          ˵��
*		V1.0    2016-07-16   Eric2013  	    �װ�    
*                                     
*	Copyright (C), 2015-2020, ���������� www.armfly.com
*
*********************************************************************************************************
*/
#include "includes.h"
#include "User_GUI.h"
#include "usartapp.h"
/*
*********************************************************************************************************
*	                                     �궨��
*********************************************************************************************************
*/
/*
*********************************************************************************************************
*	                                     �б�ͷ����
*********************************************************************************************************
*/
WM_HTIMER hTimer;
WM_HWIN hDlg;
extern GUI_MEMDEV_Handle  	BackGround_DEV;

/*
*********************************************************************************************************
*	                       GUI_WIDGET_CREATE_INFO��������
*********************************************************************************************************
*/
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = 
{
	{ WINDOW_CreateIndirect, 	 "Window",   MENU_WINDOW_ID0, 0, 0, 480, 320, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, 	 "Text",     MENU_TEXT_ID0, 70, 0, 342, 50, 0, 0x0, 0 },
	{ LISTVIEW_CreateIndirect, "Listview", MENU_LISTVIEW_ID0, 59, 50, 362, 270, 0, 0x0, 10 },
	{ BUTTON_CreateIndirect, " ", MENU_BUTTON_LEFT_ID1, 0, 0, 59, 101, 0, 0x0, 0 },
	{ BUTTON_CreateIndirect, " ", MENU_BUTTON_LEFT_ID2, 0, 106, 59, 110, 0, 0x0, 0 },
	{ BUTTON_CreateIndirect, " ", MENU_BUTTON_LEFT_ID3, 0, 220, 59, 101, 0, 0x0, 0 },
	{ BUTTON_CreateIndirect, " ", MENU_BUTTON_RIGHT_ID1, 422, 0, 59, 101, 0, 0x0, 0 },
	{ BUTTON_CreateIndirect, " ", MENU_BUTTON_RIGHT_ID2, 422, 106, 59, 110, 0, 0x0, 0 },
	{ BUTTON_CreateIndirect, " ", MENU_BUTTON_RIGHT_ID3, 422, 220, 59, 101, 0, 0x0, 0 },	
};
extern uint8_t person_refresh;
/*
*********************************************************************************************************
*	�� �� ��: _cbDialog
*	����˵��: �Ի���Ļص�����
*	��    ��: pMsg ��Ϣָ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void _StateDialog(WM_MESSAGE * pMsg) 
{
	switch (pMsg->MsgId) 
	{
		case WM_INIT_DIALOG:		
			//��ʼ���б�ؼ�
			person_refresh = 0;
			SendLEDNavigationData(0,KEY_ALL);
			Setstheinitialstate();
			OvertimeOpen();
			InitStateText(pMsg);
			USER_ReviseButtonStyle(pMsg);
			Creator_Statewindow(pMsg,1);
			WM_SetFocus(pMsg->hWin);
			
			break;
		case WM_PAINT:
			GUI_MEMDEV_WriteAt(BackGround_DEV, 0, 0);
			break;
		case WM_TIMER:
			if(pMsg->hWin != WM_GetFocussedWindow())
			WM_SetFocus(pMsg->hWin);
			if(person_refresh == 1)
			{
				show_person_refresh(pMsg);
			}
			else if(person_refresh == 2)
			{
				show_senson_refresh(pMsg);
			}
			else if(person_refresh == 3)
			{
				show_function_refresh(pMsg);
			}
			OvertimeReturn(pMsg);
			Determineneedtoreturninterface(pMsg,1);
			WM_RestartTimer(hTimer,100);
		break;
		case WM_KEY:
			if((((WM_KEY_INFO*)(pMsg->Data.p))->PressedCnt) > 0) //��������
			{
				USER_DownButtonShow(pMsg,((WM_KEY_INFO*)(pMsg->Data.p))->Key);
				switch (((WM_KEY_INFO*)(pMsg->Data.p))->Key) 
				{ 	
					case USER_KEY_ESC:
						GUI_EndDialog(pMsg->hWin, 0);
						CreateBackGround();
					break;	
					case USER_KEY_LEFT1:	
						WM_DeleteWindow(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW0));
						LISTVIEW_CreateEx(59,50,360,270,pMsg->hWin,WM_CF_SHOW,GUI_TA_LEFT|TEXT_CF_VCENTER,GUI_ID_LISTVIEW0);
						Creator_Statewindow(pMsg,1);
						TEXT_SetText(WM_GetDialogItem(pMsg->hWin, MENU_TEXT_ID0),"������");						
						break;
					case USER_KEY_RIGHT1:	
						WM_DeleteWindow(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW0));
						LISTVIEW_CreateEx(59,50,360,270,pMsg->hWin,WM_CF_SHOW,TEXT_CF_HCENTER|TEXT_CF_VCENTER,GUI_ID_LISTVIEW0);
						Creator_Statewindow(pMsg,2);
						TEXT_SetText(WM_GetDialogItem(pMsg->hWin, MENU_TEXT_ID0),"��־");						
						break;
					case USER_KEY_LEFT2:	
						WM_DeleteWindow(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW0));
						LISTVIEW_CreateEx(59,50,360,270,pMsg->hWin,WM_CF_SHOW,TEXT_CF_HCENTER|TEXT_CF_VCENTER,GUI_ID_LISTVIEW0);
						Creator_Statewindow(pMsg,3);
						TEXT_SetText(WM_GetDialogItem(pMsg->hWin, MENU_TEXT_ID0),"��Ա��λ");						
						break;
					case USER_KEY_RIGHT2:	
						WM_DeleteWindow(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW0));
						LISTVIEW_CreateEx(59,50,360,270,pMsg->hWin,WM_CF_SHOW,TEXT_CF_HCENTER|TEXT_CF_VCENTER,GUI_ID_LISTVIEW0);
						Creator_Statewindow(pMsg,4);						
						TEXT_SetText(WM_GetDialogItem(pMsg->hWin, MENU_TEXT_ID0),"�汾��Ϣ");
						break;
					case USER_KEY_LEFT3:	
						WM_DeleteWindow(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW0));
						LISTVIEW_CreateEx(59,50,360,270,pMsg->hWin,WM_CF_SHOW,TEXT_CF_HCENTER|TEXT_CF_VCENTER,GUI_ID_LISTVIEW0);
						Creator_Statewindow(pMsg,5);						
						TEXT_SetText(WM_GetDialogItem(pMsg->hWin, MENU_TEXT_ID0),"������Ϣ");
						break;
					case USER_KEY_RIGHT3:	
						WM_DeleteWindow(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW0));
						LISTVIEW_CreateEx(59,50,360,270,pMsg->hWin,WM_CF_SHOW,TEXT_CF_HCENTER|TEXT_CF_VCENTER,GUI_ID_LISTVIEW0);
						Creator_Statewindow(pMsg,6);						
						TEXT_SetText(WM_GetDialogItem(pMsg->hWin, MENU_TEXT_ID0),"������Ϣ");
						break;
					case USER_KEY_UP:					
						USER_MoveListviewRow(pMsg,0);
						break;
					case USER_KEY_DOWN:
						USER_MoveListviewRow(pMsg,1);
						break;
					default:
						break;			
				}				
			}
			else //����̧��
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
*	�� �� ��: MainTask
*	����˵��: GUI������
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
extern uint8_t  CurrentWindow;
void CreateStateTask(void) 
{
	/*
	 ���ڶ໺��ʹ����ڴ��豸������˵��
	   1. ʹ�ܶ໺���ǵ��õ����º������û�Ҫ��LCDConf_Lin_Template.c�ļ��������˶໺�壬���ô˺�������Ч��
		  WM_MULTIBUF_Enable(1);
	   2. ����ʹ��ʹ���ڴ��豸�ǵ��ú�����WM_SetCreateFlags(WM_CF_MEMDEV);
	   3. ���emWin�����ö໺��ʹ����ڴ��豸��֧�֣���ѡһ���ɣ����������ѡ��ʹ�ö໺�壬ʵ��ʹ��
		  STM32F429BIT6 + 32λSDRAM + RGB565/RGB888ƽ̨���ԣ��໺�������Ч�Ľ��ʹ����ƶ����߻���ʱ��˺��
		  �У�����Ч����������ԣ�ͨ��ʹ�ܴ���ʹ���ڴ��豸���������ġ�
	   4. ����emWin����Ĭ���ǿ��������塣
	*/
//	WM_MULTIBUF_Enable(1);
//	WM_SetCreateFlags(WM_CF_MEMDEV);

	hDlg = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), &_StateDialog, WM_HBKWIN, 0, 0);
//	GUI_ExecCreatedDialog(hDlg);

	hTimer = WM_CreateTimer(WM_GetClientWindow(hDlg), 0, 500, 0);
	CurrentWindow = statewin;
}

/***************************** ���������� www.armfly.com (END OF FILE) *********************************/
