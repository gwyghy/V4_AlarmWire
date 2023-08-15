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
#include "udp_app_proc.h"
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
WM_HWIN ServerMenu_hDlg;
WM_HTIMER ServerhTimerMenu,WarningoneServer_hWin;
extern GUI_MEMDEV_Handle  	BackGround_DEV,SuddenStop_DEV;
extern uint16_t Send_parameter_flag;
extern uint8_t  CurrentWindow;
/*
*********************************************************************************************************
*	                       GUI_WIDGET_CREATE_INFO��������
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
*	�� �� ��: _cbDialog
*	����˵��: �Ի���Ļص�����
*	��    ��: pMsg ��Ϣָ��
*	�� �� ֵ: ��
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
			//��ʼ���б�ؼ�
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
//					TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT23),"��������������ʧ��");
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
					TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT23),"Զ������ͨ���쳣");//��ȡ����������ʧ��
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

			if((((WM_KEY_INFO*)(pMsg->Data.p))->PressedCnt) > 0) //��������
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
				if(GetconfigState())//��ʼ���ò���
				{
					id = Getrankvalue(rank);
					USER_SetValueListviewData(pMsg,((WM_KEY_INFO*)(pMsg->Data.p))->Key,1);
					//�������洦��
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
				else //��ʼѡ�����
				{				
					ServerMenu_SetCurrentPage(((WM_KEY_INFO*)(pMsg->Data.p))->Key,pMsg);
					//ҳ�治���費��Ҫˢ��???
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

void CreateServerMenuTask(void) 
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
*	�� �� ��: 
*	����˵��: ������ͣ����
*	��    ��: 
*	�� �� ֵ: 
*********************************************************************************************************
*/
WM_HWIN CreateServerWarningone(void)
{
	WarningoneServer_hWin = WM_CreateWindowAsChild(60,50,360,163,ServerMenu_hDlg,WM_CF_SHOW|WM_CF_HASTRANS,WarningoneServer_cbDialog,0);
//	Warning_hTimer = WM_CreateTimer(WM_GetClientWindow(Warning_hWin), 0, 100, 0);
	return WarningoneServer_hWin;
}
/***************************** ���������� www.armfly.com (END OF FILE) *********************************/
