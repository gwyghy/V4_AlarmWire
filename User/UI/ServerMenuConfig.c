#include "User_GUI.h"

extern INITLIST   MenuList[300];
extern Displayrefresh   Serviceparameters;
const LISTVIEW_HEADER ServerMeum_LISTVIEW_HEADER[] =
{
	{39,""},
	{190,"����"},
	{131,"��ֵ"},
};
/*
*********************************************************************************************************
*	�� �� ��: 
*	����˵��: ��ȡ������Ҫ��ʾ�Ĳ���
*	��    ��: 
*	�� �� ֵ: 
*********************************************************************************************************
*/
uint8_t ServerGetListviewData(void)
{
	uint8_t i ,count,j= 0;
	MENU_PARA temp_value;
	uint8_t * temp;
	for(i= 0;i<Serviceparameters.ListviewDisplaye.count;i++)
	{	
		memcpy(&temp_value,&Serviceparameters.ListviewDisplaye.buf[i],26);	
		if(temp_value.Show_Noshow == DISPLAY_SHOW)
		{
			MenuList[j].Parameter = (MENU_PARA *)&Serviceparameters.ListviewDisplaye.buf[i];	
			MenuList[j].Deviation = i;
			MenuList[j].Para_Value = MenuList[j].Parameter->Para_Value;
			j ++;
		}
		temp = temp+26;
	}
	return j;
}
/*
*********************************************************************************************************
*	�� �� ��: 
*	����˵��: ���ݵ�ǰҳ����ʾ�б�
*	��    ��: 
*	�� �� ֵ: 
*********************************************************************************************************
*/
void ServerMenu_ShowListviewForPage(WM_MESSAGE * pMsg)
{
//	if(id == 0)
//		id = 0x100000000000000;
	ServerUSER_ShwoListviewData(pMsg,ServerGetListviewData());
}

/*
*********************************************************************************************************
*	�� �� ��: 
*	����˵��: ���ݵ�ǰҳ����ʾ�ı�
*	��    ��: 
*	�� �� ֵ: 
*********************************************************************************************************
*/
void ServerMenu_ShowTextForPage(WM_MESSAGE * pMsg)
{
	uint8_t i;
	uint64_t send = 0;
	char * tempname;
	char * TextOptions;
	char buf[9];
	char show[100];
	char Table[100];
	uint64_t temp;
	uint8_t	rank[LEVEL_MAX] = {0};
	temp = Getrankvalue(rank);
	memset(buf,0,sizeof(buf));
	memset(Table,0,sizeof(Table));
	memset(show,0,sizeof(show));
	
	for(i=0;i<Serviceparameters.TextDisplay.count;i++)
	{
		strcat(show,(char *)Serviceparameters.TextDisplay.buf[i]);
		strcat(show,"/");
	}
	show[strlen(show)-1] = '\0';
	if(strlen(show) > 20)
	{
		while(1)
		{
			if(strlen(show) < 20)
			{
				break;
			}
			else
			{
				TextOptions = strchr(show, '/');
				memset(Table,0,sizeof(Table));
				memcpy(Table,TextOptions+1,strlen(TextOptions)-1);
				memset(show,0,sizeof(show));
				memcpy(show,Table,strlen(Table));
			}
		}
		memset(Table,0,sizeof(Table));
		memcpy(Table,"../",sizeof("../"));
		strcat(Table,show);
	}
	else
	{
		memcpy(Table,show,strlen(show));
	}

	USER_ReviseTextShow(pMsg,MENU_TEXT_ID0,(char *)Table);

	return;
}
/*
*********************************************************************************************************
*	�� �� ��: 
*	����˵��: ���ݵ�ǰҳ����ʾ����
*	��    ��: 
*	�� �� ֵ: 
*********************************************************************************************************
*/
void ServerMenu_ShowButtonForPage(WM_MESSAGE * pMsg)
{
	static uint8_t leve,leveold;
	uint8_t  count,i;
	STR_V4PARA_NAME *temp = NULL;
	static char * ButtonOptions[40];
		
	leve = GetLevelValue();
//	if(leve != leveold)
//	{
		memset(ButtonOptions,0,sizeof(ButtonOptions));
		if(Serviceparameters.ButtonDisplay.count != 0)
		{	
			for(i=0;i<Serviceparameters.ButtonDisplay.count;i++)
			{
				ButtonOptions[i] = (char *)Serviceparameters.ButtonDisplay.buf[i];
			}
		}
//		leveold = leve;
//	}
	USER_ReviseAllButtonShow(pMsg,ButtonOptions);

}

/*
*********************************************************************************************************
*	�� �� ��: 
*	����˵��: �����˵��б�
*	��    ��: 
*	�� �� ֵ: 
*********************************************************************************************************
*/
void ServerCreator_Menuwindow(WM_MESSAGE * pMsg)
{
	USER_ReviseListview(pMsg);
	USER_ReviseListviewHeader(pMsg,ServerMeum_LISTVIEW_HEADER,sizeof(ServerMeum_LISTVIEW_HEADER));
	Setrankvalue(0,0);
	Setrankvalue(1,0);
	Setrankvalue(2,0);
	Setrankvalue(3,0);
	Setrankvalue(4,0);
	Setrankvalue(5,0);
	Setrankvalue(6,0);
	Setrankvalue(7,0);
	SetPageValue(0);
	SetLevelValue(0);
	SetRowValue(0);
	SetconfigState(0);
	USER_ReviseButtonStyle(pMsg);
	InitMenuText(pMsg);		
	ServerMenu_ShowButtonForPage(pMsg);	
	ServerMenu_ShowTextForPage(pMsg);		
	ServerMenu_ShowListviewForPage(pMsg);	

	TEXT_SetFont(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT1), &GUI_FontHZ16);
	TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT1), "1/1");
	TEXT_SetTextColor(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT1),GUI_WHITE);
//	TEXT_SetBkColor(hItem,GUI_WHITE);
	TEXT_SetTextAlign(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT1),GUI_TA_HCENTER | GUI_TA_VCENTER);
}
/*
*********************************************************************************************************
*	�� �� ��: 
*	����˵��: ��ȡ�������ֵ
*	��    ��: 
*	�� �� ֵ: 
*********************************************************************************************************
*/
uint8_t ServerGetMAXbutton(uint64_t id)
{
	uint8_t count = 0;
	if(id == 0)
	{
		count = 6;
	}
	else
		count = Serviceparameters.ButtonDisplay.count;

	return count;	
}
extern uint8_t  ServerMAXbutton;
uint8_t  Serverpagelast[8] = {0};
uint8_t  levelchange;
/*
*********************************************************************************************************
*	�� �� ��: 
*	����˵��: ���õ�ǰҳ��
*	��    ��: 
*	�� �� ֵ: 
*********************************************************************************************************
*/
void ServerMenu_SetCurrentPage(uint16_t key,WM_MESSAGE* pMsg)
{
	char buf[10];
	uint8_t	rank[LEVEL_MAX] = {0};
	uint8_t level;
	uint64_t id;
	level = GetLevelValue();
	id = Getrankvalue(rank);
	switch (key) 	 
	{
		case USER_KEY_LEFT1:		
			if((1+(GetPageValue()*6)) > ServerMAXbutton)
				break;
			Setrankvalue(level,1+(GetPageValue()*6));
			if(level<LEVEL_MAX)//����к���
			{
				levelchange = 1;
			}
			break;
		case USER_KEY_LEFT2:
			if((3+(GetPageValue()*6)) > ServerMAXbutton)
				break;
			Setrankvalue(level,3+(GetPageValue()*6));
			if(level<LEVEL_MAX)//����к���
			{
				levelchange = 1;
			}
			break;	
		case USER_KEY_LEFT3:
			if((5+(GetPageValue()*6)) > ServerMAXbutton)
				break;					
			Setrankvalue(level,5+(GetPageValue()*6));
			if(level<LEVEL_MAX)//����к���
			{
				levelchange = 1;
			}
			break;
		case USER_KEY_RIGHT1:
			if((2+(GetPageValue()*6)) > ServerMAXbutton)
				break;					
			Setrankvalue(level,2+(GetPageValue()*6));
			if(level<LEVEL_MAX)//����к���
			{
				levelchange = 1;
			}
			break;					
		case USER_KEY_RIGHT2:
			if((4+(GetPageValue()*6)) > ServerMAXbutton)
				break;					
			Setrankvalue(level,4+(GetPageValue()*6));
			if(level<LEVEL_MAX)//����к���
			{
				levelchange = 1;
			}
			break;
		case USER_KEY_RIGHT3:
			if((6+(GetPageValue()*6)) > ServerMAXbutton)
				break;					
			Setrankvalue(level,6+(GetPageValue()*6));
			if(level<LEVEL_MAX)//����к���
			{
				levelchange = 1;
			}
			break;
		case USER_KEY_ASSIST:
			if(GetMAXpage(ServerMAXbutton) <= GetPageValue())
			{
				SetPageValue(0);
				ServerMenu_ShowButtonForPage(pMsg);
			}
			else
			{
				SetPageValue(GetPageValue()+1);
				ServerMenu_ShowButtonForPage(pMsg);
			}

			break;
		case USER_KEY_LEFT:	
			Setrankvalue(level,0);
			if(level > 0)
			{
				if(ServerMAXbutton != 0)
				{
					Setrankvalue(level-1,0);
					SetLevelValue(level-1);
				}
			}
			else
			{
				memset(Serverpagelast,0,sizeof(Serverpagelast));
			}
			SetPageValue(Serverpagelast[GetLevelValue()]);
			break;
			default:
				break;			
	}
	id = Getrankvalue(rank);
	memset(buf,0,sizeof(buf));
	sprintf(buf,"%d/%d",GetPageValue()+1,GetMAXpage(ServerMAXbutton)+1);
	TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT1),buf);
}










