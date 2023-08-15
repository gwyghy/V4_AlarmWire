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
#include "FileLoad.h"
#include "DoFatfsFiles.h"
#include "FileUpdate.h"
#include "sys_param_type.h"
#include "sys_msg_def.h"
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
WM_HTIMER DeveloperhTimer;
WM_HWIN DeveloperhDlg;
extern GUI_MEMDEV_Handle  	BackGround_DEV;
EDIT_Handle hEdit;
/*
*********************************************************************************************************
*	                       GUI_WIDGET_CREATE_INFO��������
*********************************************************************************************************
*/

char *Developer_Grope[] =
{
	"�м��",
	"����",
	"���س���",
	"���³���",
};	
char *SupportType_Grope[] =
{
	"������",
	"��ǰ��",
	"���ƻ�β",
	"ƽ���г�",
};	
char *IPaddressg_Grope[] =
{
	"IP��ַ",
//	"��������",
//	"Ĭ������",
};
char *ProgramType_Grope[] =
{
	"���ؿ�����",
	"���ص�ŷ�",
	"���ؽ�  ��",
	"������  Ա",
	"������  ��",
	"���ظ�  ��",
	"���غ�  ��",
	"���ؼ�  ��",
	"������  ��",
	"���ؿɱ��",
	"������ԱBL",
	"��������BL",
	"���ظ߶�BL",
	"���ض๦������",   //ghy �������ض๦�����⹦��
	"����MiniHUB"          //ghy ��������MiniHUB����
};
char *Filefolder_Grope[] = 
{
	"���¿�����",
	"���µ�ŷ�",
	"���½�  ��",
	"������  Ա",
	"������  ��",
	"���¸�  ��",
	"���º�  ��",
	"���¼�  ��",
	"������  ��",
	"���¿ɱ��",
	"������ԱBL",
	"��������BL",
	"���¸߶�BL",
	"���¶๦������", //ghy ���Ӹ��¶๦�����⹦��
	"����MiniHUB"		 //ghy ���Ӹ���MiniHUB����
};


const LISTVIEW_HEADER Developer_LISTVIEW_HEADER[] =
{
	{280,""},

};
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = 
{
  { WINDOW_CreateIndirect, 	 "Window",   MENU_WINDOW_ID0, 0, 0, 480, 320, 0, 0x0, 0 },
  { TEXT_CreateIndirect, 	 "Text",     MENU_TEXT_ID0, 0, 0, 480, 50, 0, 0x0, 0 },
  { TEXT_CreateIndirect, 	 "Text",     GUI_ID_TEXT1, 90, 280, 84, 40, 0, 0x0, 0 },
  { TEXT_CreateIndirect, 	 "Text",     GUI_ID_TEXT2, 176, 280, 54, 40, 0, 0x0, 0 },
  { TEXT_CreateIndirect,     "Text",     GUI_ID_TEXT3, 202, 280, 246, 40, 0, 0x0, 0 },
  { LISTVIEW_CreateIndirect, "Listview", GUI_ID_LISTVIEW0, 60, 65, 140, 190, 0, 0x0, 10 },
  { LISTVIEW_CreateIndirect, "Listview", GUI_ID_LISTVIEW1, 220, 65, 200, 190, 0, 0x0, 10 },
};
/*
*********************************************************************************************************
*	�� �� ��: 
*	����˵��: ѡ���Ͳ�������
*	��    ��: 
*	�� �� ֵ: 
*********************************************************************************************************
*/
void Developer_init(WM_MESSAGE * pMsg)
{
	WM_HWIN hItem;
	uint8_t  temp;
	char * sensor;
	char * addrow[10] = {"IP:","192",".","168",".","6",".","192"};
	uint8_t i =1;
	GUI_SetPenSize(10);
	GUI_SetColor(GUI_WHITE);
	TEXT_SetFont(WM_GetDialogItem(pMsg->hWin, MENU_TEXT_ID0), &GUI_FontHZ32);
	TEXT_SetText(WM_GetDialogItem(pMsg->hWin, MENU_TEXT_ID0),"������ģʽ");
	TEXT_SetTextColor(WM_GetDialogItem(pMsg->hWin, MENU_TEXT_ID0),GUI_WHITE);
	TEXT_SetTextAlign(WM_GetDialogItem(pMsg->hWin, MENU_TEXT_ID0),GUI_TA_HCENTER | GUI_TA_VCENTER);

	TEXT_SetFont(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT1), &GUI_Font24_1);
	TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT1),"(C)2021");
	TEXT_SetTextColor(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT1),GUI_WHITE);
	TEXT_SetTextAlign(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT1),GUI_TA_HCENTER | GUI_TA_VCENTER);

	TEXT_SetFont(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT2), &GUI_Font24_1);
	TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT2),"HNDZ");
	TEXT_SetTextColor(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT2),GUI_RED);
	TEXT_SetTextAlign(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT2),GUI_TA_HCENTER | GUI_TA_VCENTER);

	TEXT_SetFont(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT3), &GUI_Font24_1);
	TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT3),", All Right Reserved");
	TEXT_SetTextColor(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT3),GUI_WHITE);
	TEXT_SetTextAlign(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT3),GUI_TA_HCENTER | GUI_TA_VCENTER);


	hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW0);
	LISTVIEW_SetHeaderHeight(hItem,0);         //���ñ���ĸ߶�
	LISTVIEW_SetRowHeight(hItem,47);            //����LISTVIEW���и�
	LISTVIEW_SetBkColor(hItem,LISTVIEW_CI_UNSEL,GUI_BLACK);    //���ñ�����ɫ
	LISTVIEW_SetBkColor(hItem,LISTVIEW_CI_SEL,GUI_BLACK);    //���ñ�����ɫ
	LISTVIEW_SetBkColor(hItem,LISTVIEW_CI_SELFOCUS,GUI_WHITE);    //���ñ�����ɫ
	LISTVIEW_SetFont(hItem, &GUI_FontHZ24);
	LISTVIEW_SetTextColor(hItem,LISTVIEW_CI_UNSEL,GUI_WHITE);
	LISTVIEW_SetTextColor(hItem,LISTVIEW_CI_SEL,GUI_WHITE);
	LISTVIEW_SetTextColor(hItem,LISTVIEW_CI_SELFOCUS,GUI_BLACK);
	LISTVIEW_SetGridVis(hItem,1);
	LISTVIEW_AddColumn(hItem,140, " ",  GUI_TA_CENTER | GUI_TA_VCENTER); 
	
	SysDataFun(DATA_MSG_GET_DEVICETYPE_PARAM, 0,0,0,0,(void *)&temp,0);
	i = temp;
	Developer_Grope[0] = SupportType_Grope[i];
	for(i=0;i<GUI_COUNTOF(Developer_Grope);i++ )
	{
		sensor = Developer_Grope[i];
		LISTVIEW_AddRow(hItem,&sensor);
	}
	
	LISTVIEW_IncSel(hItem);
	WM_SetFocus(hItem);
	LISTVIEW_SetSel(hItem,0);
	
	
	hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW1);
	LISTVIEW_SetHeaderHeight(hItem,0);         //���ñ���ĸ߶�
	LISTVIEW_SetRowHeight(hItem,47);            //����LISTVIEW���и�
	LISTVIEW_SetBkColor(hItem,LISTVIEW_CI_UNSEL,GUI_BLACK);    //���ñ�����ɫ
	LISTVIEW_SetBkColor(hItem,LISTVIEW_CI_SEL,GUI_BLACK);    //���ñ�����ɫ
	LISTVIEW_SetBkColor(hItem,LISTVIEW_CI_SELFOCUS,GUI_WHITE);    //���ñ�����ɫ
	LISTVIEW_SetFont(hItem, &GUI_FontHZ24);
	LISTVIEW_SetTextColor(hItem,LISTVIEW_CI_UNSEL,GUI_WHITE);
	LISTVIEW_SetTextColor(hItem,LISTVIEW_CI_SEL,GUI_WHITE);
	LISTVIEW_SetTextColor(hItem,LISTVIEW_CI_SELFOCUS,GUI_BLACK);
	LISTVIEW_SetGridVis(hItem,1);
	LISTVIEW_AddColumn(hItem,200, " ",  GUI_TA_LEFT | GUI_TA_VCENTER); 
	for(i=0;i<GUI_COUNTOF(SupportType_Grope);i++ )
	{
		sensor = SupportType_Grope[i];
		LISTVIEW_AddRow(hItem,&sensor);
	}
	TEXT_CreateEx(60,60,360,75,pMsg->hWin,WM_CF_HIDE,TEXT_CF_HCENTER|TEXT_CF_VCENTER,GUI_ID_TEXT21," ");
	TEXT_SetBkColor(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT21),GUI_BLACK);
	TEXT_CreateEx(60,185,360,75,pMsg->hWin,WM_CF_HIDE,TEXT_CF_HCENTER|TEXT_CF_VCENTER,GUI_ID_TEXT22," ");
	TEXT_SetBkColor(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT22),GUI_BLACK);
	TEXT_CreateEx(60,135,360,50,pMsg->hWin,WM_CF_HIDE,TEXT_CF_HCENTER|TEXT_CF_VCENTER,GUI_ID_TEXT23," ");
	TEXT_SetBkColor(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT23),GUI_BLACK);
	TEXT_SetTextColor(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT23),GUI_WHITE);
	TEXT_SetFont(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT23), &GUI_FontHZ32);
	LISTVIEW_CreateEx(60,135,360,50,pMsg->hWin,WM_CF_HIDE,TEXT_CF_HCENTER|TEXT_CF_VCENTER,GUI_ID_LISTVIEW2);
	hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW2);
	LISTVIEW_SetHeaderHeight(hItem,0);         //���ñ���ĸ߶�
	LISTVIEW_SetRowHeight(hItem,50);            //����LISTVIEW���и�
	LISTVIEW_SetBkColor(hItem,LISTVIEW_CI_UNSEL,GUI_WHITE);    //���ñ�����ɫ
	LISTVIEW_SetBkColor(hItem,LISTVIEW_CI_SEL,GUI_WHITE);    //���ñ�����ɫ
	LISTVIEW_SetBkColor(hItem,LISTVIEW_CI_SELFOCUS,GUI_WHITE);    //���ñ�����ɫ
	LISTVIEW_SetFont(hItem, &GUI_FontHZ24);
	LISTVIEW_SetTextColor(hItem,LISTVIEW_CI_UNSEL,GUI_BLACK);
	LISTVIEW_SetTextColor(hItem,LISTVIEW_CI_SEL,GUI_BLACK);
	LISTVIEW_SetTextColor(hItem,LISTVIEW_CI_SELFOCUS,GUI_BLACK);
//	LISTVIEW_SetGridVis(hItem,1);
	LISTVIEW_AddColumn(hItem,90, " ",  GUI_TA_RIGHT | GUI_TA_VCENTER); 
	LISTVIEW_AddColumn(hItem,40, " ",  GUI_TA_CENTER | GUI_TA_VCENTER); 
	LISTVIEW_AddColumn(hItem,14, " ",  GUI_TA_CENTER | GUI_TA_VCENTER); 
	LISTVIEW_AddColumn(hItem,40, " ",  GUI_TA_CENTER | GUI_TA_VCENTER); 
	LISTVIEW_AddColumn(hItem,14, " ",  GUI_TA_CENTER | GUI_TA_VCENTER); 
	LISTVIEW_AddColumn(hItem,40, " ",  GUI_TA_CENTER | GUI_TA_VCENTER); 
	LISTVIEW_AddColumn(hItem,14, " ",  GUI_TA_CENTER | GUI_TA_VCENTER); 
	LISTVIEW_AddColumn(hItem,40, " ",  GUI_TA_CENTER | GUI_TA_VCENTER); 
	LISTVIEW_AddRow(hItem,addrow);
	LISTVIEW_EnableCellSelect(hItem,1);
}
/*
*********************************************************************************************************
*	�� �� ��: 
*	����˵��: ѡ���Ͳ�������
*	��    ��: 
*	�� �� ֵ: 
*********************************************************************************************************
*/

void USER_GUI_Filefolder(WM_MESSAGE * pMsg)
{
	WM_HWIN hItem;
	uint8_t i;
	char * p;
	hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW1);
	for(i=0;i<GUI_COUNTOF(Filefolder_Grope);i++ )
	{
		p = Filefolder_Grope[i];
		LISTVIEW_AddRow(hItem,&p);
	}
}
/*
*********************************************************************************************************
*	�� �� ��: 
*	����˵��: ѡ���Ͳ�������
*	��    ��: 
*	�� �� ֵ: 
*********************************************************************************************************
*/
void USER_GUI_ID_LISTVIEW1(WM_MESSAGE * pMsg)
{
	WM_HWIN hItem;
	uint8_t row,i;
	char * p;
	
	if(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW0) != WM_GetFocussedWindow())
		return;
	row = LISTVIEW_GetSel(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW0));
	hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW1);
	LISTVIEW_DeleteAllRows(hItem);
	switch (row) 
	{ 
		case 0:
			for(i=0;i<GUI_COUNTOF(SupportType_Grope);i++ )
			{
				p = SupportType_Grope[i];
				LISTVIEW_AddRow(hItem,&p);
			}
			break;
		case 1:
			
			for(i=0;i<GUI_COUNTOF(IPaddressg_Grope);i++ )
			{
				p = IPaddressg_Grope[i];
				LISTVIEW_AddRow(hItem,&p);
			}
			break;
		case 2:
			for(i=0;i<GUI_COUNTOF(ProgramType_Grope);i++ )
			{
				p = ProgramType_Grope[i];
				LISTVIEW_AddRow(hItem,&p);
			}
			break;
		case 3:
			for(i=0;i<GUI_COUNTOF(Filefolder_Grope);i++ )
			{
				p = Filefolder_Grope[i];
				LISTVIEW_AddRow(hItem,&p);
			}
			break;
		default:
		/* �����ļ�ֵ������ */
			break;
	}
}

static void _usercbeditaddkey(EDIT_Handle hobj ,int key) 
{
	switch (key) 
	{ 
		case USER_KEY_RIGHT1:
			EDIT_SetpfAddKeyEx(hobj,_usercbeditaddkey);
			EDIT_AddKey(hobj,'1');
			break ;
		case USER_KEY_RIGHT2:
			EDIT_SetpfAddKeyEx(hobj,_usercbeditaddkey);
			EDIT_AddKey(hobj,GUI_KEY_BACKSPACE);
			break ;
	}
}

/*
*********************************************************************************************************
*	�� �� ��: _cbDialog
*	����˵��: �Ի���Ļص�����
*	��    ��: pMsg ��Ϣָ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void _DeveloperDialog(WM_MESSAGE * pMsg) 
{
	WM_HWIN hItem;
	char buf[30];
	uint8_t length;
	globalStateType  StateTypetemp;
	static NetParamType  temp;
	static uint8_t flag;
	static uint8_t sige;
	static uint8_t rowtemp;
	static uint8_t rowtemp2;
	static uint8_t start;
	static uint8_t colu;
	uint8_t i;
	char * p;
	char* res[10];
	uint8_t count;
	static uint16_t ipadd[10] ;
	static uint8_t  BufInd;
	static uint8_t  frist;
	switch (pMsg->MsgId) 
	{
		case WM_INIT_DIALOG:		
			//��ʼ���б�ؼ�
			SendLEDNavigationData(0,KEY_ALL);
			flag = 0;
			sige = 0;
			rowtemp = 0;
			rowtemp2 = 0;
			start = 0;
			frist = 0;
			OvertimeOpen();
			Developer_init(pMsg);	
			break;
		case WM_PAINT:
			GUI_SetBkColor(GUI_BLACK);
			GUI_Clear();
			GUI_DrawLine(30,50,450,50);   //��������
			GUI_DrawLine(30,54,450,54);   //��������
		
			GUI_DrawLine(30,270,450,270);   //��������
			GUI_DrawLine(30,266,450,266);   //��������
			break;
		case WM_TIMER:
			if(pMsg->hWin != WM_GetFocussedWindow())
				WM_SetFocus(pMsg->hWin);
			OvertimeReturn(pMsg);			
			if(start == 1)
			{
				switch(getProLoadStatus())
				{
					case CXBUNCONNECT:
						TEXT_SetText(WM_GetDialogItem(pMsg->hWin, MENU_TEXT_ID0),"�����δ����");
						setProLoadStatus(0xFF);
						start = 0;
						frist = 0;
						OvertimeOpen();
						break;
					case NOPROTYPE:
						TEXT_SetText(WM_GetDialogItem(pMsg->hWin, MENU_TEXT_ID0),"û�и����ͳ���");
						setProLoadStatus(0xFF);
						start = 0;
						frist = 0;
						OvertimeOpen();
						break;
					case PROLOADBREAK:
						TEXT_SetText(WM_GetDialogItem(pMsg->hWin, MENU_TEXT_ID0),"�����ж�");
						WM_HideWindow(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT21));
						WM_HideWindow(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT22));
						WM_DeleteWindow(WM_GetDialogItem(pMsg->hWin, GUI_ID_PROGBAR0));
						setProLoadStatus(0xFF);
						start = 0;
						frist = 0;
						OvertimeOpen();
						break;
					case PROLOADCRCERROR:
						TEXT_SetText(WM_GetDialogItem(pMsg->hWin, MENU_TEXT_ID0),"���մ���");
						WM_HideWindow(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT21));
						WM_HideWindow(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT22));
						WM_DeleteWindow(WM_GetDialogItem(pMsg->hWin, GUI_ID_PROGBAR0));
						setProLoadStatus(0xFF);
						start = 0;
						frist = 0;
						OvertimeOpen();
						break;
					case PROLOADSUCCESS:
						TEXT_SetText(WM_GetDialogItem(pMsg->hWin, MENU_TEXT_ID0),"���ճɹ�");
						WM_HideWindow(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT21));
						WM_HideWindow(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT22));
						WM_DeleteWindow(WM_GetDialogItem(pMsg->hWin, GUI_ID_PROGBAR0));
						setProLoadStatus(0xFF);
						start = 0;
						frist = 0;
						OvertimeOpen();
						break;
					case PROLOADING:
						if(frist == 0)
						{
							frist = 1;
							WM_ShowWindow(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT21));
							WM_ShowWindow(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT22));
							PROGBAR_CreateEx(60,135,360,50,pMsg->hWin,WM_CF_SHOW,PROGBAR_CF_HORIZONTAL,GUI_ID_PROGBAR0);
							hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_PROGBAR0);
							PROGBAR_SetFont(hItem, &GUI_FontHZ24);
							PROGBAR_SetMinMax(hItem, 0, 100); 
							OvertimeClose();
						}						
						PROGBAR_SetValue(WM_GetDialogItem(pMsg->hWin, GUI_ID_PROGBAR0),UpdateTimerApi());	
						break;
					default:
						break;	
				}				
			}
			else if(start == 2)
			{
				if(sendTimerAPI() != 100)
				{
					PROGBAR_SetValue(WM_GetDialogItem(pMsg->hWin, GUI_ID_PROGBAR0),sendTimerAPI());
				}
				else
				{
					TEXT_SetText(WM_GetDialogItem(pMsg->hWin, MENU_TEXT_ID0),"�������");
					GUI_Delay(1000);
					TEXT_SetText(WM_GetDialogItem(pMsg->hWin, MENU_TEXT_ID0),"L2:����R2:ȫ��L3:���R3:�Ҽ�"); //ghy 
					WM_HideWindow(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT21));
					WM_HideWindow(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT22));
					WM_DeleteWindow(WM_GetDialogItem(pMsg->hWin, GUI_ID_PROGBAR0));
					frist = 0;  //ghy 
					start = 0;
					OvertimeOpen();
				}
			}
			else
			{
				if(getFileRecvStatus() == FILESENDING)
				{
					if(frist == 0)
					{
						frist = 1;
						TEXT_SetText(WM_GetDialogItem(pMsg->hWin, MENU_TEXT_ID0),"���ڷ��ͣ����Ե�");
						WM_ShowWindow(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT21));
						WM_ShowWindow(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT22));
						PROGBAR_CreateEx(60,135,360,50,pMsg->hWin,WM_CF_SHOW,PROGBAR_CF_HORIZONTAL,GUI_ID_PROGBAR0);
						hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_PROGBAR0);
						PROGBAR_SetFont(hItem, &GUI_FontHZ24);
						PROGBAR_SetMinMax(hItem, 0, 100); 
						OvertimeClose();
					}
					else
					{
						if(sendTimerAPI() != 100)
						{
							PROGBAR_SetValue(WM_GetDialogItem(pMsg->hWin, GUI_ID_PROGBAR0),sendTimerAPI());
						}
						else
						{
							TEXT_SetText(WM_GetDialogItem(pMsg->hWin, MENU_TEXT_ID0),"�������");
							GUI_Delay(1000);
							TEXT_SetText(WM_GetDialogItem(pMsg->hWin, MENU_TEXT_ID0),"L2:����R2:ȫ��L3:���R3:�Ҽ�"); //ghy 
							WM_HideWindow(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT21));
							WM_HideWindow(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT22));
							WM_DeleteWindow(WM_GetDialogItem(pMsg->hWin, GUI_ID_PROGBAR0));
							setFileRecvStatus(0xFF);
							start = 0;
							OvertimeOpen();
						}
					}				
				}
				
				else
				{
					GUIGetDateFunAPI(API_Support_Statur,0,&StateTypetemp,0);
					if(StateTypetemp.slaveMode == 2)
					{
						if(frist == 0)
						{
							frist = 1;

							WM_ShowWindow(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT21));
							WM_ShowWindow(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT22));
							WM_ShowWindow(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT23));
							TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT23),"������ַ������");
							start = 3;					
						}
					}
					else if(StateTypetemp.slaveMode == 3)
					{

						WM_HideWindow(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT21));
						WM_HideWindow(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT22));
						WM_HideWindow(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT23));
						SetSysFuncMode(SYSMODE_NORMAL);
						start = 0;
						frist = 0;
					}
				}
			}
			WM_RestartTimer(DeveloperhTimer,100);
            break;
		case WM_KEY:
			if(start)
				break;
			if((((WM_KEY_INFO*)(pMsg->Data.p))->PressedCnt) > 0) //��������
			{
				switch (((WM_KEY_INFO*)(pMsg->Data.p))->Key) 
				{ 
					case USER_KEY_ESC:
						GUI_EndDialog(pMsg->hWin, 0);
						CreateBackGround();
					break;					
					case USER_KEY_UP:		
						rowtemp = LISTVIEW_GetSel(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW0));	
						USER_MoveListviewRow(pMsg,0);
						USER_GUI_ID_LISTVIEW1(pMsg);
						if((rowtemp == 2)&&(flag==1))
						{ 
							TEXT_SetText(WM_GetDialogItem(pMsg->hWin, MENU_TEXT_ID0),"L1:�޸�");
						}	
						break;
					case USER_KEY_DOWN:
						rowtemp = LISTVIEW_GetSel(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW0));
						USER_MoveListviewRow(pMsg,1);
						USER_GUI_ID_LISTVIEW1(pMsg);
						if((rowtemp == 2)&&(flag==1))
						{
							TEXT_SetText(WM_GetDialogItem(pMsg->hWin, MENU_TEXT_ID0),"L1:�޸�");
						}	
						break;
					case USER_KEY_RIGHT:
						if(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW2) == WM_GetFocussedWindow())
						{
							BufInd = 0;
							if(colu < 2)
								colu = colu+2;
							else
								colu = 1;
							for(i=0;i<8;i++)
							{
								LISTVIEW_SetItemTextColor(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW2), (uint8_t)i, (uint8_t)0, LISTVIEW_CI_UNSEL, GUI_BLACK);
								LISTVIEW_SetItemTextColor(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW2), (uint8_t)i, (uint8_t)0, LISTVIEW_CI_SEL, GUI_BLACK);
								LISTVIEW_SetItemTextColor(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW2), (uint8_t)i, (uint8_t)0, LISTVIEW_CI_SELFOCUS, GUI_BLACK);
								LISTVIEW_SetItemBkColor(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW2), (uint8_t)i, (uint8_t)0, LISTVIEW_CI_UNSEL, GUI_WHITE);
								LISTVIEW_SetItemBkColor(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW2), (uint8_t)i, (uint8_t)0, LISTVIEW_CI_SEL, GUI_WHITE);
								LISTVIEW_SetItemBkColor(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW2), (uint8_t)i, (uint8_t)0, LISTVIEW_CI_SELFOCUS, GUI_WHITE);
							}
							
							
							LISTVIEW_SetItemTextColor(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW2), (uint8_t)colu, (uint8_t)0, LISTVIEW_CI_UNSEL, GUI_WHITE);
							LISTVIEW_SetItemTextColor(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW2), (uint8_t)colu, (uint8_t)0, LISTVIEW_CI_SEL, GUI_WHITE);
							LISTVIEW_SetItemTextColor(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW2), (uint8_t)colu, (uint8_t)0, LISTVIEW_CI_SELFOCUS, GUI_WHITE);
							LISTVIEW_SetItemBkColor(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW2), (uint8_t)colu, (uint8_t)0, LISTVIEW_CI_UNSEL, GUI_LIGHTBLUE);
							LISTVIEW_SetItemBkColor(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW2), (uint8_t)colu, (uint8_t)0, LISTVIEW_CI_SEL, GUI_LIGHTBLUE);
							LISTVIEW_SetItemBkColor(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW2), (uint8_t)colu, (uint8_t)0, LISTVIEW_CI_SELFOCUS, GUI_LIGHTBLUE);
						}
						if(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW0) == WM_GetFocussedWindow())
						{
							LISTVIEW_IncSel(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW1));
							WM_SetFocus(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW1));
							LISTVIEW_SetSel(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW1),0);
							rowtemp = LISTVIEW_GetSel(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW0));
							if((rowtemp == 0) || (rowtemp == 2))
							{
								flag = 1;
								TEXT_SetText(WM_GetDialogItem(pMsg->hWin, MENU_TEXT_ID0),"L1:�޸�");
							}
							else if(rowtemp == 3)
							{
								flag = 2;
								TEXT_SetText(WM_GetDialogItem(pMsg->hWin, MENU_TEXT_ID0),"L1:��");
							}
							else if(rowtemp == 1)
							{
								flag = 3;
								TEXT_SetText(WM_GetDialogItem(pMsg->hWin, MENU_TEXT_ID0),"L1:�޸�");
							}
						}
						break;
					case USER_KEY_LEFT:
						if(sige == 0)
						{
							if(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW1) == WM_GetFocussedWindow())
							{
								LISTVIEW_IncSel(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW0));
								WM_SetFocus(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW0));
								LISTVIEW_SetSel(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW0),rowtemp);
								TEXT_SetText(WM_GetDialogItem(pMsg->hWin, MENU_TEXT_ID0),"������ģʽ");	
								flag = 0;								
							}
						}
						else if((sige == 1) || (sige == 3))
						{
														
							if(sige == 3)
								flag = 2;
							if(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW1) == WM_GetFocussedWindow())
							{
								sige = 0;
								LISTVIEW_DeleteAllRows(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW1));
								USER_GUI_Filefolder(pMsg);
								LISTVIEW_IncSel(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW1));
								WM_SetFocus(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW1));
								LISTVIEW_SetSel(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW1),rowtemp2);
								TEXT_SetText(WM_GetDialogItem(pMsg->hWin, MENU_TEXT_ID0),"L1:��");
							}

						}
						else if(sige == 2)
						{
								sige = 0;
								LISTVIEW_IncSel(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW1));
								WM_SetFocus(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW1));
								LISTVIEW_SetSel(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW1),rowtemp2);
								TEXT_SetText(WM_GetDialogItem(pMsg->hWin, MENU_TEXT_ID0),"L1:�޸�");
								WM_HideWindow(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW2));
								WM_HideWindow(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT21));
								WM_HideWindow(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT22));
						}
						break;
					case USER_KEY_LEFT1:
						if((flag == 0) || (sige == 1))//ghy �޳��������ѡ�������豸
							break;
						else if(flag == 1)
						{
							rowtemp2 = LISTVIEW_GetSel(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW1));	
							if(rowtemp == 2)
							{
								CanRequestProgramProc(rowtemp2);
								start = 1;
								TEXT_SetText(WM_GetDialogItem(pMsg->hWin, MENU_TEXT_ID0),"�������أ����Ե�");
							}
							else if(rowtemp == 0)
							{
								SysDataFun(DATA_MSG_SET_DEVICETYPE_PARAM, 0,0,0,0,(void *)&rowtemp2,0);
//								CreateDeveloperTask(); //ghy ѡ���豸֮����ʾ
								//�޸ĺ�λ������,//qinxi
								ETH_Stop();
								BCM_551CS_LOW();
								NVIC_SystemReset(); 
							}
						}
						else if(flag == 2)//
						{
							sige = 1;
							
							rowtemp2 = LISTVIEW_GetSel(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW1));
							LISTVIEW_DeleteAllRows(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW1));
							count = Fatfs_FindDir("0:/update",res,1,rowtemp2);
							if(count ==0)								
							{
								p = "�޳���";
								sige = 3;
								flag = 0;
								LISTVIEW_AddRow(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW1),&p);
							}
							for(i=0;i<count;i++ )
							{
								p = res[i];
								LISTVIEW_AddRow(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW1),&p);
							}
							TEXT_SetText(WM_GetDialogItem(pMsg->hWin, MENU_TEXT_ID0),"L2:����R2:ȫ��L3:���R3:�Ҽ�");
							LISTVIEW_IncSel(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW1));
							WM_SetFocus(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW1));
							LISTVIEW_SetSel(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW1),0);
						}
						else if(flag == 3)
						{
							BufInd = 0;
							sige = 2;
							colu = 1;
							TEXT_SetText(WM_GetDialogItem(pMsg->hWin, MENU_TEXT_ID0),"L2:����");
							SysDataFun(DATA_MSG_GET_IP_PARAM, 0,0,0,0,(void *)&temp,0);
							
							
							
							memset(buf,0,sizeof(buf));
							sprintf(buf, "%d",temp.NetParam_IPAddress[0]);	
							p = buf;
							LISTVIEW_SetItemText(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW2), 1, 0, p);
							
							memset(buf,0,sizeof(buf));
							sprintf(buf, "%d",temp.NetParam_IPAddress[1]);	
							p = buf;
							LISTVIEW_SetItemText(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW2), 3, 0, p);
							
							memset(buf,0,sizeof(buf));
							sprintf(buf, "%d",temp.NetParam_IPAddress[2]);	
							p = buf;
							LISTVIEW_SetItemText(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW2), 5, 0, p);
							
							memset(buf,0,sizeof(buf));
							sprintf(buf, "%d",temp.NetParam_IPAddress[3]);	
							p = buf;
							LISTVIEW_SetItemText(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW2), 7, 0, p);
							
							for(i=0;i<8;i++)
							{
								LISTVIEW_SetItemTextColor(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW2), (uint8_t)i, (uint8_t)0, LISTVIEW_CI_UNSEL, GUI_BLACK);
								LISTVIEW_SetItemTextColor(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW2), (uint8_t)i, (uint8_t)0, LISTVIEW_CI_SEL, GUI_BLACK);
								LISTVIEW_SetItemTextColor(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW2), (uint8_t)i, (uint8_t)0, LISTVIEW_CI_SELFOCUS, GUI_BLACK);
								LISTVIEW_SetItemBkColor(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW2), (uint8_t)i, (uint8_t)0, LISTVIEW_CI_UNSEL, GUI_WHITE);
								LISTVIEW_SetItemBkColor(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW2), (uint8_t)i, (uint8_t)0, LISTVIEW_CI_SEL, GUI_WHITE);
								LISTVIEW_SetItemBkColor(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW2), (uint8_t)i, (uint8_t)0, LISTVIEW_CI_SELFOCUS, GUI_WHITE);
							}
							
							
							LISTVIEW_SetItemTextColor(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW2), (uint8_t)colu, (uint8_t)0, LISTVIEW_CI_UNSEL, GUI_WHITE);
							LISTVIEW_SetItemTextColor(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW2), (uint8_t)colu, (uint8_t)0, LISTVIEW_CI_SEL, GUI_WHITE);
							LISTVIEW_SetItemTextColor(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW2), (uint8_t)colu, (uint8_t)0, LISTVIEW_CI_SELFOCUS, GUI_WHITE);
							LISTVIEW_SetItemBkColor(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW2), (uint8_t)colu, (uint8_t)0, LISTVIEW_CI_UNSEL, GUI_LIGHTBLUE);
							LISTVIEW_SetItemBkColor(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW2), (uint8_t)colu, (uint8_t)0, LISTVIEW_CI_SEL, GUI_LIGHTBLUE);
							LISTVIEW_SetItemBkColor(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW2), (uint8_t)colu, (uint8_t)0, LISTVIEW_CI_SELFOCUS, GUI_LIGHTBLUE);
							
							WM_ShowWindow(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW2));
							WM_ShowWindow(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT21));
							WM_ShowWindow(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT22));
							WM_SetFocus(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW2));

						}
						break;	
						case USER_KEY_RIGHT1:	
						case USER_KEY_RIGHT2:	
						case USER_KEY_RIGHT3:	
						case USER_KEY_2:	
						case USER_KEY_3:	
						case USER_KEY_5:	
						case USER_KEY_6:	
						case USER_KEY_8:	
						case USER_KEY_9:	
						case USER_KEY_ASSIST:	
						if(sige == 2)
						{
							if(BufInd == 1)//��������
							{
								ipadd[colu] = ipadd[colu]*10+DigitKeyToChr(((WM_KEY_INFO*)(pMsg->Data.p))->Key);
								if(colu == 1)
								{
									if(ipadd[colu]>223)
										ipadd[colu] = 223;
								}
								else
								{
									if(ipadd[colu]>255)
										ipadd[colu] = 255;
								}
							}
							else//��������
							{
								ipadd[colu] = DigitKeyToChr(((WM_KEY_INFO*)(pMsg->Data.p))->Key);
								BufInd = 1;//��������
							}
							memset(buf,0,sizeof(buf));
							sprintf(buf, "%d",ipadd[colu]);	
							p = buf;
							LISTVIEW_SetItemText(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW2), colu, 0, p);
						}
						else if(sige == 1)
						{
							if((((WM_KEY_INFO*)(pMsg->Data.p))->Key) == USER_KEY_RIGHT2)
							{
								start = 2;
								TEXT_SetText(WM_GetDialogItem(pMsg->hWin, MENU_TEXT_ID0),"���ڷ��ͣ����Ե�");
								WM_ShowWindow(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT21));
								WM_ShowWindow(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT22));
								PROGBAR_CreateEx(60,135,360,50,pMsg->hWin,WM_CF_SHOW,PROGBAR_CF_HORIZONTAL,GUI_ID_PROGBAR0);
								hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_PROGBAR0);
								PROGBAR_SetFont(hItem, &GUI_FontHZ24);
								PROGBAR_SetMinMax(hItem, 0, 100); 
								OvertimeClose();
								LISTVIEW_GetItemText(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW1),0,LISTVIEW_GetSel(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW1)),buf,30);
								SendFile_API(FILETYPE,buf,rowtemp2,UPDATEALL);
							}
							else if((((WM_KEY_INFO*)(pMsg->Data.p))->Key) == USER_KEY_RIGHT3)
							{
								start = 2;
								TEXT_SetText(WM_GetDialogItem(pMsg->hWin, MENU_TEXT_ID0),"���ڷ��ͣ����Ե�");
								WM_ShowWindow(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT21));
								WM_ShowWindow(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT22));
								PROGBAR_CreateEx(60,135,360,50,pMsg->hWin,WM_CF_SHOW,PROGBAR_CF_HORIZONTAL,GUI_ID_PROGBAR0);
								hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_PROGBAR0);
								PROGBAR_SetFont(hItem, &GUI_FontHZ24);
								PROGBAR_SetMinMax(hItem, 0, 100); 
								OvertimeClose();
								LISTVIEW_GetItemText(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW1),0,LISTVIEW_GetSel(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW1)),buf,30);
								SendFile_API(FILETYPE,buf,rowtemp2,UPDATERIGHT);
							}
						}
						break;	
						case USER_KEY_LEFT2:	
							if(sige == 1)
							{
								start = 2;
								TEXT_SetText(WM_GetDialogItem(pMsg->hWin, MENU_TEXT_ID0),"���ڷ��ͣ����Ե�");
								WM_ShowWindow(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT21));
								WM_ShowWindow(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT22));
								PROGBAR_CreateEx(60,135,360,50,pMsg->hWin,WM_CF_SHOW,PROGBAR_CF_HORIZONTAL,GUI_ID_PROGBAR0);
								hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_PROGBAR0);
								PROGBAR_SetFont(hItem, &GUI_FontHZ24);
								PROGBAR_SetMinMax(hItem, 0, 100); 
								OvertimeClose();
								LISTVIEW_GetItemText(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW1),0,LISTVIEW_GetSel(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW1)),buf,30);
								SendFile_API(FILETYPE,buf,rowtemp2,UPDATESEIF);
							}
							if(sige == 2)
							{
								sige = 0;
								
								
								memset(buf,0,sizeof(buf));
								LISTVIEW_GetItemTextSorted(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW2),1,0,(char*)buf,4);
								if(buf[2] != 0)
								{
									length = (buf[0]-0x30)*100 + (buf[1]-0x30)*10 + (buf[2]-0x30);
								}
								else if(buf[1] != 0)
								{
									length = (buf[0]-0x30)*10 + (buf[1]-0x30);
								}
								else
								{
									length = (buf[0]-0x30);
								}
								

								temp.NetParam_IPAddress[0] = length;
								temp.NetParam_GateWay[0] = length;
								
								memset(buf,0,sizeof(buf));
								LISTVIEW_GetItemTextSorted(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW2),3,0,(char*)buf,4);
								if(buf[2] != 0)
								{
									length = (buf[0]-0x30)*100 + (buf[1]-0x30)*10 + (buf[2]-0x30);
								}
								else if(buf[1] != 0)
								{
									length = (buf[0]-0x30)*10 + (buf[1]-0x30);
								}
								else
								{
									length = (buf[0]-0x30);
								}
								temp.NetParam_IPAddress[1] = length;
								temp.NetParam_GateWay[1] = length;
								
								SysDataFun(DATA_MSG_SET_IP_PARAM_GLOBAL, 0,0,0,0,(void *)&temp,0);
								
								LISTVIEW_IncSel(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW1));
								WM_SetFocus(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW1));
								LISTVIEW_SetSel(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW1),rowtemp2);
								TEXT_SetText(WM_GetDialogItem(pMsg->hWin, MENU_TEXT_ID0),"L1:�޸�");
								WM_HideWindow(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW2));
								WM_HideWindow(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT21));
								WM_HideWindow(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT22));
							}
							break;	
						case USER_KEY_LEFT3:	
							if(sige == 1)
							{
								start = 2;
								TEXT_SetText(WM_GetDialogItem(pMsg->hWin, MENU_TEXT_ID0),"���ڷ��ͣ����Ե�");
								WM_ShowWindow(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT21));
								WM_ShowWindow(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT22));
								PROGBAR_CreateEx(60,135,360,50,pMsg->hWin,WM_CF_SHOW,PROGBAR_CF_HORIZONTAL,GUI_ID_PROGBAR0);
								hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_PROGBAR0);
								PROGBAR_SetFont(hItem, &GUI_FontHZ24);
								PROGBAR_SetMinMax(hItem, 0, 100); 
								OvertimeClose();
								LISTVIEW_GetItemText(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW1),0,LISTVIEW_GetSel(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW1)),buf,30);
								SendFile_API(FILETYPE,buf,rowtemp2,UPDATELEFT);
							}
							break;		
					default:
						break;			
				}				
			}
			else //����̧��
			{

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
void CreateDeveloperTask(void) 
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

	DeveloperhDlg = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), &_DeveloperDialog, 0, 0, 0);
//	GUI_ExecCreatedDialog(hDlg);

	DeveloperhTimer = WM_CreateTimer(WM_GetClientWindow(DeveloperhDlg), 0, 500, 0);
	CurrentWindow = developerwin;
}

/***************************** ���������� www.armfly.com (END OF FILE) *********************************/
