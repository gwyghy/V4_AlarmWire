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
	{200,"����"},
	{103,"��ֵ"},
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
{"can1","can2","ģ�����ӿ�"},
{"��ŷ�������","�Ƕ�","�߶�","����","����","��Ա"},
{"ģ��1","ģ��2","ģ��3","ģ��4"},
{"����","����"},
{"����","���ⱨ��","ĳ״̬�򷽷�"},
{"����","����","����"},
{"��","��","����"},
{"ֱ������","ֹͣ","��ʱ����","��ͣ","�ָ�"},
{"NO","ALWAYS","2040","1020"},
{"NO","ALWAYS","BLUE","RED","2020"},
{"��","��"},
{"��","��","ȫ��"},
{"1db","3db","6db","9db","12db","18db","21db","24db","27db","30db","33db"},








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
/*
*********************************************************************************************************
*	�� �� ��: 
*	����˵��: ���õ�ǰҳ��
*	��    ��: 
*	�� �� ֵ: 
*********************************************************************************************************
*/
void Menu_SetCurrentPage(uint16_t key)
{
	uint8_t	rank[LEVEL_MAX] = {0};
	uint8_t level;
	uint64_t id;
	static uint8_t  MAXbutton = 6;
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
			break;
		case USER_KEY_LEFT:	
			Setrankvalue(level,0);
			if(level > 0)
			{
				Setrankvalue(level-1,0);
				SetLevelValue(level-1);
			}
			SetPageValue(0);			
			MAXbutton = GetMAXbutton(Getrankvalue(rank));
			break;
			default:
				break;			
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
char * TextOptions;
char buf[9];
char show[30];
uint64_t temp;
uint8_t	rank[LEVEL_MAX] = {0};
temp = Getrankvalue(rank);
memset(buf,0,sizeof(buf));
memset(show,0,sizeof(show));
// sprintf((char *)buf, "%d""%d""%d""%d""%d""%d""%d""%d",rank[0],rank[1],rank[2],rank[3],rank[4],rank[5],rank[6],rank[7]);

for(i=0;i<8;i++)
{
	if(rank[i] != 0)
	{
		send |= ((uint64_t)rank[i] << ((7-i)*8));
		sprintf((char *)buf,"%x",(int)(send>>32));
		strcat(show,buf);
		strcat(show,"/");
	}
}


if(id == 0)
{
	USER_ReviseTextShow(pMsg,MENU_TEXT_ID0,"�˵�����");
}	
else
{
//	show(strlen(show));
	USER_ReviseTextShow(pMsg,MENU_TEXT_ID0,(char *)show);
}
return;
}
/*
*********************************************************************************************************
*	�� �� ��: 
*	����˵��: ��ʼ���ò���
*	��    ��: 
*	�� �� ֵ: 
*********************************************************************************************************
*/
void Menu_SetValueListviewData(WM_MESSAGE * pMsg,uint16_t key)
{
	
}


























