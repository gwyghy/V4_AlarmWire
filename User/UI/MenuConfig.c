/*
*********************************************************************************************************
*	                                  
*	ģ������ : 
*	�ļ����� : MenuConfig.c
*	��    �� : V1.0
*	˵    �� : ʵ�ְ������ֹ���
*              
*	�޸ļ�¼ :
*		�汾��   ����         ����          ˵��
*		V1.0    2016-07-16   Eric2013  	    �װ�    
*                                     
*	Copyright (C), 2015-2020, ���������� www.armfly.com
*
*********************************************************************************************************
*/
#include "MenuConfig.h"
#include "User_GUI.h"
/*
*********************************************************************************************************
*	                                     �궨��
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*	                                    ȫ�ֱ���
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*	                                     �б�ͷ����
*********************************************************************************************************
*/
const LISTVIEW_HEADER Meum_LISTVIEW_HEADER[] =
{
	{39,""},
	{190,"����"},
	{131,"��ֵ"},
};
/*
*********************************************************************************************************
*	                                     ѡ�������
*********************************************************************************************************
*/

const char *SelectItem[][15] =
{/*��ֹ�޸�*/

{"��"},
{"��ֹ","����"},	
{"�绰","����","��","��","��"},
{"ģ����","����","����"},
{"ģ����","����","����"},
{"1","10","100","1000"},
{"��","can1","can2","ģ�����ӿ�"},
{"������","��ŷ�������","�����","�Ƕ�","�߶�","����","��Ա","����","����","����","HUB1","HUB2","HUB3","�Ǹ�һ��","�๦������"},
{"��","ģ��1","ģ��2","ģ��3","ģ��4"},
{"����","����"},
{"����","���ⱨ��","ĳ״̬�򷽷�"},
{"����","����","����"},
{"��","��","����"},
{"ֱ������","ֹͣ","��ʱ����","��ͣ","�ָ�","������ͣ"},
{"NO","ALWAYS","2040","1020"},
{"NO","ALWAYS","BLUE","RED","2020"},
{"��","��"},
{"���","�Ҽ�","ȫ��"},
{"0db","3db","6db","9db","12db","15db","18db","21db","24db","27db","30db","33db"},
{"��϶���","������"},
{"ִ��","������һ��"},
{"����Զ��","����","Զ��"},
{"�ֶ�","�Զ�"},
{"����","ȫ��"},
{"�м��","���ɼ�","��ͷ��"},
{"��","X��","Y��","Z��"},
{"��װ","��װ"},
{"��","��","��"},
{"USART","CAN"},


};/*��ֹ�޸�*/

const char *ServeSelectItem[][15] =
{/*��ֹ�޸�*/

{"��"},
{"����","��ú��","�Զ�"},	
{"��","������","��ͷ��ǰ","��β��ǰ","�����","����ת��","���ƻ�β","ƽ���г�","Ӧ�ó���8","Ӧ�ó���9","Ӧ�ó���10"},
{"��","���ܴ���","˳�򴥷�","�������","�ظ�����"},
{"��",">","<"},
{"��","����","����","����"},
{"��","������","��϶���"},
};/*��ֹ�޸�*/
/*
*********************************************************************************************************
*	                                     �����������Զ���
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*	                                    �������ƶ���
*********************************************************************************************************
*/
const char *ButtonName[][12] = 
{
	{"ϵͳ����","������","������","��϶���","����","���鶯��",
	 " "," "," "," "," "," "},
};


/*
*********************************************************************************************************
*	                                    �ı����ƶ���
*********************************************************************************************************
*/

const char TextName[] = 
{
	"�˵�����"
};

/*
*********************************************************************************************************
*	�� �� ��: 
*	����˵��: ��ȡָ��ͳ���
*	��    ��: 
*	�� �� ֵ: 
*********************************************************************************************************
*/
void GetPointerFromLogicalPast(uint64_t id,MENU_PARA **address ,uint8_t *length)
{

}


/*
*********************************************************************************************************
*	�� �� ��: 
*	����˵��: �����˵��б�
*	��    ��: 
*	�� �� ֵ: 
*********************************************************************************************************
*/
void Creator_Menuwindow(WM_MESSAGE * pMsg)
{
	USER_ReviseListview(pMsg);
	USER_ReviseListviewHeader(pMsg,Meum_LISTVIEW_HEADER,sizeof(Meum_LISTVIEW_HEADER));
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
	Menu_ShowButtonForPage(pMsg,PAGE_0_0_0);	
	InitMenuText(pMsg);		
	Menu_ShowListviewForPage(pMsg,0x100000000000000);	

	TEXT_SetFont(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT1), &GUI_FontHZ16);
	TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT1), "1/1");
	TEXT_SetTextColor(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT1),GUI_WHITE);
//	TEXT_SetBkColor(hItem,GUI_WHITE);
	TEXT_SetTextAlign(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT1),GUI_TA_HCENTER | GUI_TA_VCENTER);
}

/*
*********************************************************************************************************
*	�� �� ��: 
*	����˵��: ��ȡҳ�����ֵ
*	��    ��: 
*	�� �� ֵ: 
*********************************************************************************************************
*/
uint8_t GetMAXpage(uint8_t id)
{
	uint8_t page = 0;
	if(id > 6)
	{
		if(id%6)
			page = (id/6);
		else
			page = (id/6)-1;
	}
	return page;
}
uint8_t  assistflag;
extern uint8_t  MAXbutton;
/*
*********************************************************************************************************
*	�� �� ��: 
*	����˵��: ���õ�ǰҳ��
*	��    ��: 
*	�� �� ֵ: 
*********************************************************************************************************
*/
void Menu_SetCurrentPage(uint16_t key,WM_MESSAGE* pMsg)
{
	char buf[10];
	uint8_t	rank[LEVEL_MAX] = {0};
	uint8_t level;
	uint64_t id;
	static uint8_t  pagelast[8];
	level = GetLevelValue();
	id = Getrankvalue(rank);
	switch (key) 	 
	{
		case USER_KEY_LEFT1:		
			if((1+(GetPageValue()*6)) > MAXbutton)
				break;
			Setrankvalue(level,1+(GetPageValue()*6));
			if((getJsonParaIsChilds(Getrankvalue(rank))) && (level<LEVEL_MAX))//����к���
			{
				pagelast[GetLevelValue()] = GetPageValue();
				SetLevelValue(level+1);
				MAXbutton = GetMAXbutton(Getrankvalue(rank));
			}
			break;
		case USER_KEY_LEFT2:
			if((3+(GetPageValue()*6)) > MAXbutton)
				break;
			Setrankvalue(level,3+(GetPageValue()*6));
			if((getJsonParaIsChilds(Getrankvalue(rank))) && (level<LEVEL_MAX))//����к���
			{
				pagelast[GetLevelValue()] = GetPageValue();
				SetLevelValue(level+1);
				MAXbutton = GetMAXbutton(Getrankvalue(rank));
			}
			break;	
		case USER_KEY_LEFT3:
			if((5+(GetPageValue()*6)) > MAXbutton)
				break;					
			Setrankvalue(level,5+(GetPageValue()*6));
			if((getJsonParaIsChilds(Getrankvalue(rank))) && (level<LEVEL_MAX))//����к���
			{
				pagelast[GetLevelValue()] = GetPageValue();
				SetLevelValue(level+1);
				MAXbutton = GetMAXbutton(Getrankvalue(rank));
			}
			break;
		case USER_KEY_RIGHT1:
			if((2+(GetPageValue()*6)) > MAXbutton)
				break;					
			Setrankvalue(level,2+(GetPageValue()*6));
			if((getJsonParaIsChilds(Getrankvalue(rank))) && (level<LEVEL_MAX))//����к���
			{
				pagelast[GetLevelValue()] = GetPageValue();
				SetLevelValue(level+1);
				MAXbutton = GetMAXbutton(Getrankvalue(rank));
			}
			break;					
		case USER_KEY_RIGHT2:
			if((4+(GetPageValue()*6)) > MAXbutton)
				break;					
			Setrankvalue(level,4+(GetPageValue()*6));
			if((getJsonParaIsChilds(Getrankvalue(rank))) && (level<LEVEL_MAX))//����к���
			{
				pagelast[GetLevelValue()] = GetPageValue();
				SetLevelValue(level+1);
				MAXbutton = GetMAXbutton(Getrankvalue(rank));
			}
			break;
		case USER_KEY_RIGHT3:
			if((6+(GetPageValue()*6)) > MAXbutton)
				break;					
			Setrankvalue(level,6+(GetPageValue()*6));
			if((getJsonParaIsChilds(Getrankvalue(rank))) && (level<LEVEL_MAX))//����к���
			{
				pagelast[GetLevelValue()] = GetPageValue();
				SetLevelValue(level+1);
				MAXbutton = GetMAXbutton(Getrankvalue(rank));
			}
			break;
		case USER_KEY_ASSIST:
			if(id == 0)
				break ;
			if(GetMAXpage(MAXbutton) <= GetPageValue())
			{
				SetPageValue(0);
				Setrankvalue(level,1+(GetPageValue()*6));
				
			}
			else
			{
				SetPageValue(GetPageValue()+1);
				Setrankvalue(level,1+(GetPageValue()*6));
			}
			assistflag = 1;
			break;
		case USER_KEY_LEFT:	
			Setrankvalue(level,0);
			if(level > 0)
			{
				Setrankvalue(level-1,0);
				SetLevelValue(level-1);
			}
			SetPageValue(pagelast[GetLevelValue()]);			
			MAXbutton = GetMAXbutton(Getrankvalue(rank));
			break;
			default:
				break;			
	}
	id = Getrankvalue(rank);
	if(id == 0)
	{
		TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT1),"1/1");
	}
	else
	{
		memset(buf,0,sizeof(buf));
		sprintf(buf,"%d/%d",GetPageValue()+1,GetMAXpage(MAXbutton)+1);
		TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT1),buf);
	}
}
/*
*********************************************************************************************************
*	�� �� ��: 
*	����˵��: ���ݵ�ǰҳ����ʾ����
*	��    ��: 
*	�� �� ֵ: 
*********************************************************************************************************
*/
void Menu_ShowButtonForPage(WM_MESSAGE * pMsg,uint64_t id)
{
	static uint8_t leve,leveold;
	uint8_t  count,i;
	STR_V4PARA_NAME *temp = NULL;
	static char * ButtonOptions[40];
	if(id == 0)		
	{
		USER_ReviseAllButtonShow(pMsg,ButtonName[0]);
		leveold = 0;
	}
	else
	{		
		leve = GetLevelValue();
		if(leve != leveold)
		{
			memset(ButtonOptions,0,sizeof(ButtonOptions));
			count = getJsonParaStruct(id,&temp);
			if(count != 0)
			{	
				for(i=0;i<count;i++)
				{
					ButtonOptions[i] = (char *)temp;
					temp = temp+1;
				}
			}
			leveold = leve;
		}
		USER_ReviseAllButtonShow(pMsg,ButtonOptions);
	}
}
	

/*
*********************************************************************************************************
*	�� �� ��: 
*	����˵��: ���ݵ�ǰҳ����ʾ�б�
*	��    ��: 
*	�� �� ֵ: 
*********************************************************************************************************
*/
void Menu_ShowListviewForPage(WM_MESSAGE * pMsg,uint64_t id)
{
	if(id == 0)
		id = 0x100000000000000;
	USER_ShwoListviewData(pMsg,GetListviewData(id));
}
/*
*********************************************************************************************************
*	�� �� ��: 
*	����˵��: ���ݵ�ǰҳ����ʾ�ı�
*	��    ��: 
*	�� �� ֵ: 
*********************************************************************************************************
*/
void Menu_ShowTextForPage(WM_MESSAGE * pMsg,uint64_t id)
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
	for(i=0;i<8;i++)
	{
		if(rank[i] != 0)
		{
			send |= ((uint64_t)rank[i] << ((7-i)*8));
			getJsonMenuName(send,&tempname);
			strcat(show,tempname);
			strcat(show,"/");
		}
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
	if(id == 0)
	{
		USER_ReviseTextShow(pMsg,MENU_TEXT_ID0,"�˵�����");
	}	
	else
	{
		USER_ReviseTextShow(pMsg,MENU_TEXT_ID0,(char *)Table);
	}
	return;
}

