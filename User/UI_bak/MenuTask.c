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
WM_HTIMER hTimerMenu;
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
  { BUTTON_CreateIndirect,   "",   MENU_BUTTON_LEFT_ID1, 0, 0, 60, 106, 0, 0x0, 0 },
  { BUTTON_CreateIndirect,   "",   MENU_BUTTON_LEFT_ID2, 0, 107, 60, 106, 0, 0x0, 0 },
  { BUTTON_CreateIndirect,   "",   MENU_BUTTON_LEFT_ID3, 0, 214, 60, 106, 0, 0x0, 0 },
  { BUTTON_CreateIndirect,   "",   MENU_BUTTON_RIGHT_ID1, 420, 0, 60, 106, 0, 0x0, 0 },
  { BUTTON_CreateIndirect,   "",   MENU_BUTTON_RIGHT_ID2, 420, 107, 60, 106, 0, 0x0, 0 },
  { BUTTON_CreateIndirect,   "",   MENU_BUTTON_RIGHT_ID3, 420, 214, 60, 106, 0, 0x0, 0 },
};

/*
*********************************************************************************************************
*	�� �� ��: _cbDialog
*	����˵��: �Ի���Ļص�����
*	��    ��: pMsg ��Ϣָ��
*	�� �� ֵ: ��
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
			//��ʼ���б�ؼ�
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
			if((((WM_KEY_INFO*)(pMsg->Data.p))->PressedCnt) > 0) //��������
			{
				switch (((WM_KEY_INFO*)(pMsg->Data.p))->Key) 
				{ 
					case USER_KEY_ESC:
						GUI_EndDialog(pMsg->hWin, 0);
						CreateBackGround();
					break;
				}				
				if(GetconfigState())//��ʼ���ò���
				{
					id = Getrankvalue(rank);
					USER_SetValueListviewData(pMsg,((WM_KEY_INFO*)(pMsg->Data.p))->Key);
					//�������洦��
					if(USER_SaveValueListviewData(pMsg,((WM_KEY_INFO*)(pMsg->Data.p))->Key,id))
					{						
						USER_ExitListviewRow(pMsg);
						Menu_ShowButtonForPage(pMsg,id);						
					}
				}
				else //��ʼѡ�����
				{
					USER_DownButtonShow(pMsg,((WM_KEY_INFO*)(pMsg->Data.p))->Key);
					Menu_SetCurrentPage(((WM_KEY_INFO*)(pMsg->Data.p))->Key);
					//ҳ�治���費��Ҫˢ��???
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

void CreateMenuTask(void) 
{
	WM_HWIN hDlg;
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
	hDlg = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), &_cbDialog, 0, 0, 0);
	hTimerMenu = WM_CreateTimer(WM_GetClientWindow(hDlg), 0, 10000, 0);
//	while(1)
//	{
//		GUI_Delay(1);
//	}
}

/***************************** ���������� www.armfly.com (END OF FILE) *********************************/
