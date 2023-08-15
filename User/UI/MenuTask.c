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
#include "sys_data_type.h"
#include "sys_msg_def.h"
#include "FileUpdate.h"
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
WM_HWIN Menu_hDlg;
WM_HTIMER hTimerMenu,Warningone_hWin;
extern GUI_MEMDEV_Handle  	BackGround_DEV,SuddenStop_DEV;
extern uint16_t Send_parameter_flag;
/*
*********************************************************************************************************
*	                       GUI_WIDGET_CREATE_INFO��������
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
*	�� �� ��: _cbDialog
*	����˵��: �Ի���Ļص�����
*	��    ��: pMsg ��Ϣָ��
*	�� �� ֵ: ��
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
			//��ʼ���б�ؼ�
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
					TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT23),"���ڷ���:0%");
					start = 1;	
					OvertimeClose();				
				}
				else
				{
					if(sendTimerAPI() != 100)
					{
						memset(buf,0,sizeof(buf));
						sprintf(buf,"���ڷ���:%d%%",sendTimerAPI());
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
						TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT23),"������ַ������");
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
			if((((WM_KEY_INFO*)(pMsg->Data.p))->PressedCnt) > 0) //��������
			{
				USER_DownButtonShow(pMsg,((WM_KEY_INFO*)(pMsg->Data.p))->Key);
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
					USER_SetValueListviewData(pMsg,((WM_KEY_INFO*)(pMsg->Data.p))->Key,0);
					//�������洦��
					if(USER_SaveValueListviewData(pMsg,((WM_KEY_INFO*)(pMsg->Data.p))->Key,id,0))
					{						
						USER_ExitListviewRow(pMsg);
						Menu_ShowButtonForPage(pMsg,id);						
					}
				}
				else //��ʼѡ�����
				{				
					Menu_SetCurrentPage(((WM_KEY_INFO*)(pMsg->Data.p))->Key,pMsg);
					//ҳ�治���費��Ҫˢ��???
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
extern uint8_t  CurrentWindow;
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
*	�� �� ��: 
*	����˵��: ������ͣ����
*	��    ��: 
*	�� �� ֵ: 
*********************************************************************************************************
*/
WM_HWIN CreateWarningone(void)
{
	Warningone_hWin = WM_CreateWindowAsChild(60,50,360,163,Menu_hDlg,WM_CF_SHOW|WM_CF_HASTRANS,Warningone_cbDialog,0);
//	Warning_hTimer = WM_CreateTimer(WM_GetClientWindow(Warning_hWin), 0, 100, 0);
	
	return Warningone_hWin;
}
/***************************** ���������� www.armfly.com (END OF FILE) *********************************/
