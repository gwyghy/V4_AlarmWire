/*
*********************************************************************************************************
*	                                  
*	ģ������ : User_Listview�û���
*	�ļ����� : User_Listview.c
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
#include "User_Listview.h"
#include "MenuTask.h"
#include "includes.h"
#include "sys_msg_def.h"
#include "FileUpdate.h"
#include "udp_app_proc.h"
extern const char *SelectItem[][15];
extern const char *ServeSelectItem[][15];
/*
*********************************************************************************************************
*	                                     �궨��
*********************************************************************************************************
*/
 SETLIST   MenuListtemp;
 INITLIST   MenuList[300];
/*
*********************************************************************************************************
*	                                   ���
*********************************************************************************************************
*/

#define ActionOptions_MAX     	48
#define SensorOptions_MAX 			30

#define PAGE_0_0_0_MAX  7
#define PAGE_1_0_0_MAX  5
#define PAGE_5_0_0_MAX  4



char *ActionOptions[ActionOptions_MAX];
char *SensorOptions[SensorOptions_MAX];

	

/*
*********************************************************************************************************
*	�� �� ��: 
*	����˵��: ��ʼ���б�
*	��    ��: 
*	�� �� ֵ: 
*********************************************************************************************************
*/
void USER_ReviseListview(WM_MESSAGE * pMsg)
{
	WM_HWIN hItem;
	hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW0);
	LISTVIEW_SetHeaderHeight(hItem,50);         //���ñ���ĸ߶�
	LISTVIEW_SetRowHeight(hItem,40);            //����LISTVIEW���и�
	LISTVIEW_SetBkColor(hItem,LISTVIEW_CI_UNSEL,GUI_BLACK);    //���ñ�����ɫ
	LISTVIEW_SetBkColor(hItem,LISTVIEW_CI_SEL,GUI_BLACK);    //���ñ�����ɫ
	LISTVIEW_SetBkColor(hItem,LISTVIEW_CI_SELFOCUS,GUI_WHITE);    //���ñ�����ɫ
	LISTVIEW_SetFont(hItem, &GUI_FontHZ24);
	LISTVIEW_SetTextColor(hItem,LISTVIEW_CI_UNSEL,GUI_WHITE);
	LISTVIEW_SetTextColor(hItem,LISTVIEW_CI_SEL,GUI_WHITE);
	LISTVIEW_SetTextColor(hItem,LISTVIEW_CI_SELFOCUS,GUI_BLACK);

}
/*
*********************************************************************************************************
*	�� �� ��: 
*	����˵��: ����б�ͷ
*	��    ��: 
*	�� �� ֵ: 
*********************************************************************************************************
*/
void USER_ReviseListviewHeader(WM_MESSAGE * pMsg,const LISTVIEW_HEADER *hader,uint8_t count)
{
	uint8_t i;
	static int pig_site[6] = {GUI_TA_CENTER, GUI_TA_CENTER, GUI_TA_CENTER,GUI_TA_CENTER};
	WM_HWIN hItem,hHeader;
	hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW0);
	hHeader = LISTVIEW_GetHeader(hItem);
	HEADER_SetTextColor(hHeader,GUI_BLACK);
	HEADER_SetFont(hHeader, &GUI_FontHZ24);
	count = count/sizeof(LISTVIEW_HEADER);	
	for (i = 0; i < count; i++)
	{
		//��LISTVIEW�����
		LISTVIEW_AddColumn(hItem,hader[i].wide, hader[i].u8headerMame,  pig_site[i] | GUI_TA_VCENTER); 
	}
}

/*
*********************************************************************************************************
*	�� �� ��: 
*	����˵��: ��ȡ������Ҫ��ʾ�Ĳ���
*	��    ��: 
*	�� �� ֵ: 
*********************************************************************************************************
*/
uint8_t GetListviewData(uint64_t id)
{
	uint8_t i ,count,j= 0;
	MENU_PARA temp_value;
	uint8_t * temp;
	uint16_t * para = NULL;
	i = sizeof(temp_value);
	temp = getJsonParaAddress(id);
	count = getJsonParaChildSize(id);
	SysParamFun(DATA_MSG_READ_OBJECT_PARAM,id,0,0,&para,0,0,0);
	for(i= 0;i<count;i++)
	{	
		memcpy(&temp_value,temp,26);	
		if(temp_value.Show_Noshow == DISPLAY_SHOW)
		{
			MenuList[j].Parameter = (MENU_PARA *)temp;	
			MenuList[j].Para_Value = *para;
			MenuList[j].Deviation = i;
			j ++;
		}
		temp = temp+26;
		para = para+1;
	}
	return j;
}
/*
*********************************************************************************************************
*	�� �� ��: 
*	����˵��: ��ʾ�б�
*	��    ��: 
*	�� �� ֵ: 
*********************************************************************************************************
*/
void ServerUSER_ShwoListviewData(WM_MESSAGE * pMsg,uint8_t count)
{
	uint8_t i;
	WM_HWIN hItem;
	static char * p[3];
	char buf[15];
	char numberbuf[3];
	hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW0);
	LISTVIEW_DeleteAllRows(hItem);
//	LISTVIEW_SetFont(hItem, &GUI_FontHZ32x33);
	for(i=0;i<count;i++)
	{
		memset(buf,0x00,sizeof(buf));
		memset(numberbuf,0x00,sizeof(numberbuf));
		sprintf(numberbuf, "%d", i+1);
		p[0] = numberbuf;
		p[1] = MenuList[i].Parameter->Para_Name;	
		if((MenuList[i].Parameter->Input_Type == EDIT_SEL))
		{
			if(MenuList[i].Parameter->pSelItem < 100)
			{
				p[2] = (char *)SelectItem[MenuList[i].Parameter->pSelItem][MenuList[i].Para_Value];
			}
			else
			{
				p[2] = (char *)ServeSelectItem[MenuList[i].Parameter->pSelItem - 100][MenuList[i].Para_Value];
			}
		}
		else if(MenuList[i].Parameter->Input_Type == EDIT_INT)
		{	
			sprintf(buf, "%d", MenuList[i].Para_Value);
			AddUnitDispiayINITLIST(buf,&MenuList[i]);
			p[2] = buf;
		}
		else if(MenuList[i].Parameter->Input_Type == EDIT_FLOAT)
		{	
			sprintf(buf, "%d.%d",MenuList[i].Para_Value/10, MenuList[i].Para_Value%10);	
			AddUnitDispiayINITLIST(buf,&MenuList[i]);			
			p[2] = buf;
		}
		else if(MenuList[i].Parameter->Input_Type == EDIT_DOUBLE)
		{	
			sprintf(buf, "%d.%02d",MenuList[i].Para_Value/100, MenuList[i].Para_Value%100);	
			AddUnitDispiayINITLIST(buf,&MenuList[i]);			
			p[2] = buf;
		}
		else if(MenuList[i].Parameter->Input_Type == EDIT_SIGNED_INT)
		{	
			sprintf(buf, "%d",(int16_t)MenuList[i].Para_Value);	
			AddUnitDispiayINITLIST(buf,&MenuList[i]);
			p[2] = buf;
		}
		else if(MenuList[i].Parameter->Input_Type == EDIT_CMD)
		{	
//			sprintf(buf, "%d",(int16_t)MenuList[i].Parameter->Para_Value);	
			p[2] = buf;
		}
		else
			;
		LISTVIEW_AddRow(hItem, p);
		memset(p,0x00,sizeof(p));
	}
	WM_SetFocus(hItem);	
	LISTVIEW_IncSel(hItem);
	USER_ChooseListviewStartRow(pMsg);	
}
/*
*********************************************************************************************************
*	�� �� ��: 
*	����˵��: ��ʾ�б�
*	��    ��: 
*	�� �� ֵ: 
*********************************************************************************************************
*/
void USER_ShwoListviewData(WM_MESSAGE * pMsg,uint8_t count)
{
	uint8_t i;
	WM_HWIN hItem;
	static char * p[3];
	char buf[15];
	char numberbuf[3];
	hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW0);
	LISTVIEW_DeleteAllRows(hItem);
//	LISTVIEW_SetFont(hItem, &GUI_FontHZ32x33);
	for(i=0;i<count;i++)
	{
		memset(buf,0x00,sizeof(buf));
		memset(numberbuf,0x00,sizeof(numberbuf));
		sprintf(numberbuf, "%d", i+1);
		p[0] = numberbuf;
		p[1] = MenuList[i].Parameter->Para_Name;	
		if((MenuList[i].Parameter->Input_Type == EDIT_SEL)
			||((MenuList[i].Parameter->Input_Type > 99)))
		{
			if(MenuList[i].Parameter->pSelItem > 0xEF)
			{
				if(MenuList[i].Para_Value == 0)
				{
					p[2] = (char *)SelectItem[0][0];
				}
				else
					AuxiliaryActionShow(&p[2],MenuList[i].Parameter->pSelItem,MenuList[i].Para_Value);
			}
			else
			{
				p[2] = (char *)SelectItem[MenuList[i].Parameter->pSelItem][MenuList[i].Para_Value];
			}
		}
		else if(MenuList[i].Parameter->Input_Type == EDIT_INT)
		{	
			sprintf(buf, "%d", MenuList[i].Para_Value);
			AddUnitDispiayINITLIST(buf,&MenuList[i]);
			p[2] = buf;
		}
		else if(MenuList[i].Parameter->Input_Type == EDIT_FLOAT)
		{	
			sprintf(buf, "%d.%d",MenuList[i].Para_Value/10, MenuList[i].Para_Value%10);	
			AddUnitDispiayINITLIST(buf,&MenuList[i]);			
			p[2] = buf;
		}
		else if(MenuList[i].Parameter->Input_Type == EDIT_DOUBLE)
		{	
			sprintf(buf, "%d.%02d",MenuList[i].Para_Value/100, MenuList[i].Para_Value%100);	
			AddUnitDispiayINITLIST(buf,&MenuList[i]);			
			p[2] = buf;
		}
		else if(MenuList[i].Parameter->Input_Type == EDIT_SIGNED_INT)
		{	
			sprintf(buf, "%d",(int16_t)MenuList[i].Para_Value);	
			AddUnitDispiayINITLIST(buf,&MenuList[i]);
			p[2] = buf;
		}
		else if(MenuList[i].Parameter->Input_Type == EDIT_CMD)
		{	
//			sprintf(buf, "%d",(int16_t)MenuList[i].Para_Value);	
			p[2] = buf;
		}
		else
			;
		LISTVIEW_AddRow(hItem, p);
		memset(p,0x00,sizeof(p));
	}
	WM_SetFocus(hItem);	
	LISTVIEW_IncSel(hItem);
	USER_ChooseListviewStartRow(pMsg);	
}
/*
*********************************************************************************************************
*	�� �� ��: 
*	����˵��: ѡ���0��
*	��    ��: 
*	�� �� ֵ: 
*********************************************************************************************************
*/
void USER_ChooseListviewStartRow(WM_MESSAGE * pMsg)
{
	WM_HWIN hItem;
	hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW0);
	LISTVIEW_SetSel(hItem,0);
	SetRowValue(LISTVIEW_GetSel(hItem));
}

/*
*********************************************************************************************************
*	�� �� ��: 
*	����˵��: �ƶ�ѡ�����
*	��    ��: 
*	�� �� ֵ: 
*********************************************************************************************************
*/
void USER_MoveListviewRow(WM_MESSAGE * pMsg,uint8_t Direction)
{
	WM_HWIN hItem;
	uint8_t row;
	hItem = WM_GetFocussedWindow();
	row = LISTVIEW_GetSel(hItem);
	if(Direction)
	{	
		if(row >= LISTVIEW_GetNumRows(hItem) -1)
		{
			SetRowValue(0);
			LISTVIEW_SetSel(hItem, 0);
		}
		else
		{
			GUI_SendKeyMsg(GUI_KEY_DOWN, 1);
			SetRowValue((uint8_t)LISTVIEW_GetSel(hItem));
		}			
	}
	else
	{
		if(row == 0)
		{
			SetRowValue(LISTVIEW_GetNumRows(hItem) -1);
			LISTVIEW_SetSel(hItem, LISTVIEW_GetNumRows(hItem) -1);
		}
		else
		{
			GUI_SendKeyMsg(GUI_KEY_UP, 1);
			SetRowValue((uint8_t)LISTVIEW_GetSel(hItem));
		}
	}
	row = GetRowValue();
	LISTVIEW_SetItemBkColor(hItem, (uint8_t)2, (uint8_t)row, LISTVIEW_CI_SELFOCUS, GUI_WHITE);
}
/*
*********************************************************************************************************
*	�� �� ��: 
*	����˵��: ���ݲ���������ʾ��������
*	��    ��: 
*	�� �� ֵ: 
*********************************************************************************************************
*/
void USER_DisplyButtonNameByType(WM_MESSAGE * pMsg,uint8_t val)
{
	const char* oveaok = {"ȷ��"};
	const char* oveaall = {"ȫ��"};
	const char* local = {"����"};
	const char* negative = {"+/-"};
//	const char* decimal = {"."};
	USER_ClearAllButtonShow(pMsg);
	if(val == 0)
	{
		USER_ReviseButtonShow(pMsg,MENU_BUTTON_LEFT_ID1,oveaall);
		if(MenuListtemp.Parameter.Modift_Type == 1)
		{
			USER_ReviseButtonShow(pMsg,MENU_BUTTON_LEFT_ID1,oveaall);
		}
		else if(MenuListtemp.Parameter.Modift_Type == 3)
		{
			USER_ReviseButtonShow(pMsg,MENU_BUTTON_LEFT_ID1,oveaall);
			USER_ReviseButtonShow(pMsg,MENU_BUTTON_LEFT_ID2,local);
		}
		else if(MenuListtemp.Parameter.Modift_Type == 4)
		{
			USER_ReviseButtonShow(pMsg,MENU_BUTTON_LEFT_ID1,oveaok);
		}
		if(MenuListtemp.Parameter.Input_Type == EDIT_SIGNED_INT)
			USER_ReviseButtonShow(pMsg,MENU_BUTTON_LEFT_ID3,negative);
	}
	else
	{
		USER_ReviseButtonShow(pMsg,MENU_BUTTON_LEFT_ID1,oveaok);
		if(MenuListtemp.Parameter.Input_Type == EDIT_SIGNED_INT)
			USER_ReviseButtonShow(pMsg,MENU_BUTTON_LEFT_ID3,negative);
	}
	USER_ReviseButtonShow(pMsg,MENU_BUTTON_RIGHT_ID1,"1");
	USER_ReviseButtonShow(pMsg,MENU_BUTTON_RIGHT_ID2,"4");
	USER_ReviseButtonShow(pMsg,MENU_BUTTON_RIGHT_ID3,"7");
	
}
/*
*********************************************************************************************************
*	�� �� ��: 
*	����˵��: ȷ�����õ�ǰ��
*	��    ��: 
*	�� �� ֵ: 
*********************************************************************************************************
*/
char  parabuf[8];
void USER_EnterListviewRow(WM_MESSAGE * pMsg,uint8_t val)
{
	WM_HWIN hItem;
	uint8_t Row;
	hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW0);
	Row = GetRowValue();
	LISTVIEW_SetItemBkColor(hItem, (uint8_t)2, (uint8_t)Row, LISTVIEW_CI_SELFOCUS, GUI_GRAY);
	SetconfigState(1);
	SetBufIndValue(0);
	MenuListtemp.Parameter = * MenuList[Row].Parameter;
	MenuListtemp.Parameter.Para_Value = MenuList[Row].Para_Value;
	MenuListtemp.Deviation = MenuList[Row].Deviation;
	SetMenuListtempMaxValue(&MenuListtemp);
	USER_DisplyButtonNameByType(pMsg,val);
	LISTVIEW_GetItemText(hItem,2,Row,parabuf,8);
}
/*
*********************************************************************************************************
*	�� �� ��: 
*	����˵��: �˳����õ�ǰ��
*	��    ��: 
*	�� �� ֵ: 
*********************************************************************************************************
*/
void USER_ExitListviewRow(WM_MESSAGE * pMsg)
{
	WM_HWIN hItem;
	uint8_t Row;
	hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW0);
	Row = GetRowValue();
	SetconfigState(0);
	
	LISTVIEW_SetItemBkColor(hItem, (uint8_t)2, (uint8_t)Row, LISTVIEW_CI_SELFOCUS, GUI_WHITE);
	
}

/*
*********************************************************************************************************
*	�� �� ��: 
*	����˵��: ����ֵ+1����-1
*	��    ��: 
*	�� �� ֵ: 
*********************************************************************************************************
*/
void USER_ValueListviewData(uint8_t Direction,uint8_t sign)
{
	if(sign == 0)//����Ϊ�޷�������
	{
		if(Direction)
		{
			if((MenuListtemp.Parameter.Para_Value+1) > MenuListtemp.Parameter.Max_Value)
				MenuListtemp.Parameter.Para_Value = MenuListtemp.Parameter.Min_Value;
			else
				MenuListtemp.Parameter.Para_Value += 1;  
		}
		else
		{
			if((MenuListtemp.Parameter.Para_Value-1) < MenuListtemp.Parameter.Min_Value)
				MenuListtemp.Parameter.Para_Value = MenuListtemp.Parameter.Max_Value;
			else
				MenuListtemp.Parameter.Para_Value -= 1;  
		}
	}
	else//�з�������
	{
		if(Direction)
		{
			if(((int16_t)MenuListtemp.Parameter.Para_Value+1) > (int16_t)MenuListtemp.Parameter.Max_Value)
				MenuListtemp.Parameter.Para_Value = MenuListtemp.Parameter.Min_Value;
			else
				MenuListtemp.Parameter.Para_Value += 1;  
		}
		else
		{
			if(((int16_t)MenuListtemp.Parameter.Para_Value-1) < (int16_t)MenuListtemp.Parameter.Min_Value)
				MenuListtemp.Parameter.Para_Value = MenuListtemp.Parameter.Max_Value;
			else
				MenuListtemp.Parameter.Para_Value -= 1;  
		}
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
void ServeUSER_EditSelSetValue(WM_MESSAGE * pMsg,uint16_t key)
{
	WM_HWIN hItem;
	uint8_t row;
	char * p;
	row = GetRowValue();
	hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW0);
	switch (key) 
	{ 
		case USER_KEY_UP:
			USER_ValueListviewData(1,0);
			if(MenuListtemp.Parameter.pSelItem < 100)
			{
				p = (char *)SelectItem[MenuListtemp.Parameter.pSelItem][MenuListtemp.Parameter.Para_Value];
			}
			else
			{
				p = (char *)ServeSelectItem[MenuListtemp.Parameter.pSelItem - 100][MenuListtemp.Parameter.Para_Value];
			}
			LISTVIEW_SetItemText(hItem, 2, row, p);
			break;
		case USER_KEY_DOWN:
			USER_ValueListviewData(0,0);
						if(MenuListtemp.Parameter.pSelItem < 100)
			{
				p = (char *)SelectItem[MenuListtemp.Parameter.pSelItem][MenuListtemp.Parameter.Para_Value];
			}
			else
			{
				p = (char *)ServeSelectItem[MenuListtemp.Parameter.pSelItem - 100][MenuListtemp.Parameter.Para_Value];
			}
			LISTVIEW_SetItemText(hItem, 2, row, p);
			break;	
		default:
		/* �����ļ�ֵ������ */
			break;
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
void USER_EditSelSetValue(WM_MESSAGE * pMsg,uint16_t key)
{
	WM_HWIN hItem;
	uint8_t row;
	char * p;
	row = GetRowValue();
	hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW0);
	switch (key) 
	{ 
		case USER_KEY_UP:
			USER_ValueListviewData(1,0);
			if(MenuListtemp.Parameter.pSelItem > 0xEF)
			{
				if(MenuListtemp.Parameter.Para_Value == 0)
				{
					p = (char *)SelectItem[0][0];
				}
				else
					AuxiliaryActionShowEnter(&p,MenuListtemp.Parameter.pSelItem ,MenuListtemp.Parameter.Para_Value);
			}
			else
			{
				p = (char *)SelectItem[MenuListtemp.Parameter.pSelItem][MenuListtemp.Parameter.Para_Value];
			}
			LISTVIEW_SetItemText(hItem, 2, row, p);
			break;
		case USER_KEY_DOWN:
			USER_ValueListviewData(0,0);
			if(MenuListtemp.Parameter.pSelItem > 0xEF)
			{
				if(MenuListtemp.Parameter.Para_Value == 0)
				{
					p = (char *)SelectItem[0][0];
				}
				else
					AuxiliaryActionShowEnter(&p,MenuListtemp.Parameter.pSelItem ,MenuListtemp.Parameter.Para_Value);
			}
			else
			{
				p = (char *)SelectItem[MenuListtemp.Parameter.pSelItem][MenuListtemp.Parameter.Para_Value];
			}
			LISTVIEW_SetItemText(hItem, 2, row, p);
			break;	
		default:
		/* �����ļ�ֵ������ */
			break;
	}
}
/*
*********************************************************************************************************
*	�� �� ��: 
*	����˵��: ���Ͳ�������
*	��    ��: 
*	�� �� ֵ: 
*********************************************************************************************************
*/
void USER_EditIntSetValue(WM_MESSAGE * pMsg,uint16_t key)
{
	WM_HWIN hItem;
	uint8_t row;
	char * p;
	char buf[15];
	hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW0);
	row = GetRowValue();
	switch (key) 
	{ 
		case USER_KEY_UP:
			USER_ValueListviewData(1,0);
			sprintf(buf, "%d",MenuListtemp.Parameter.Para_Value);	
			AddUnitDispiaySETLIST(buf,&MenuListtemp);	
			p = buf;
			LISTVIEW_SetItemText(hItem, 2, row, p);
			break;

		case USER_KEY_DOWN:
			USER_ValueListviewData(0,0);
			sprintf(buf, "%d",MenuListtemp.Parameter.Para_Value);	
			AddUnitDispiaySETLIST(buf,&MenuListtemp);		
			p = buf;
			LISTVIEW_SetItemText(hItem, 2, row, p);
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
			if(GetBufIndValue())//��������
			{
				MenuListtemp.Parameter.Para_Value = MenuListtemp.Parameter.Para_Value*10 + DigitKeyToChr(key);
			}
			else//��������
			{
				MenuListtemp.Parameter.Para_Value = DigitKeyToChr(key);
				SetBufIndValue(1);//��������
			}
			if(MenuListtemp.Parameter.Para_Value > MenuListtemp.Parameter.Max_Value)
			{
				MenuListtemp.Parameter.Para_Value = MenuListtemp.Parameter.Max_Value;
				MenuListtemp.NumericRange = 0;
			}
			else if(MenuListtemp.Parameter.Para_Value < MenuListtemp.Parameter.Min_Value)
//				MenuListtemp.Parameter.Para_Value = MenuListtemp.Parameter.Min_Value;
				MenuListtemp.NumericRange = 1;
			else
				MenuListtemp.NumericRange = 0;
			sprintf(buf, "%d",MenuListtemp.Parameter.Para_Value);	
			AddUnitDispiaySETLIST(buf,&MenuListtemp);			
			p = buf;
			LISTVIEW_SetItemText(hItem, 2, row, p);
		break;
//		case USER_KEY_WORK:
//			if(GetBufIndValue())//��������
//			{
//				if(MenuListtemp.Parameter.Para_Value != 0)
//					MenuListtemp.Parameter.Para_Value = MenuListtemp.Parameter.Para_Value/10;
//			}
//			if(MenuListtemp.Parameter.Para_Value > MenuListtemp.Parameter.Max_Value)
//				MenuListtemp.Parameter.Para_Value = MenuListtemp.Parameter.Max_Value;
//			if(MenuListtemp.Parameter.Para_Value < MenuListtemp.Parameter.Min_Value)
//				MenuListtemp.Parameter.Para_Value = MenuListtemp.Parameter.Min_Value;
//			sprintf(buf, "%d",MenuListtemp.Parameter.Para_Value);	
//			AddUnitDispiaySETLIST(buf,&MenuListtemp);			
//			p = buf;
//			LISTVIEW_SetItemText(hItem, 2, row, p);
//		break;
		default:
			/* �����ļ�ֵ������ */
			break;			
	}
}
/*
*********************************************************************************************************
*	�� �� ��: 
*	����˵��: ���㵥���Ȳ�������
*	��    ��: 
*	�� �� ֵ: 
*********************************************************************************************************
*/
void USER_EditFloatSetValue(WM_MESSAGE * pMsg,uint16_t key)
{
	WM_HWIN hItem;
	uint8_t row;
	char * p;
	char buf[15];
	hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW0);
	row = GetRowValue();
	switch (key) 
	{ 
		case USER_KEY_UP:
			USER_ValueListviewData(1,0);
			sprintf(buf, "%d.%d",MenuListtemp.Parameter.Para_Value/10,MenuListtemp.Parameter.Para_Value%10);
			AddUnitDispiaySETLIST(buf,&MenuListtemp);
			p = buf;
			LISTVIEW_SetItemText(hItem, 2, row, p);
			break;

		case USER_KEY_DOWN:
			USER_ValueListviewData(0,0);
			sprintf(buf, "%d.%d",MenuListtemp.Parameter.Para_Value/10,MenuListtemp.Parameter.Para_Value%10);
			AddUnitDispiaySETLIST(buf,&MenuListtemp);
			p = buf;
			LISTVIEW_SetItemText(hItem, 2, row, p);
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
			if(GetBufIndValue())//��������
			{
				MenuListtemp.Parameter.Para_Value = MenuListtemp.Parameter.Para_Value*10 + DigitKeyToChr(key);
			}
			else//��������
			{
				MenuListtemp.Parameter.Para_Value = DigitKeyToChr(key);
				SetBufIndValue(1);//��������
			}
			if(MenuListtemp.Parameter.Para_Value > MenuListtemp.Parameter.Max_Value)
			{
				MenuListtemp.Parameter.Para_Value = MenuListtemp.Parameter.Max_Value;
				MenuListtemp.NumericRange = 0;
			}
			else if(MenuListtemp.Parameter.Para_Value < MenuListtemp.Parameter.Min_Value)
//				MenuListtemp.Parameter.Para_Value = MenuListtemp.Parameter.Min_Value;
				MenuListtemp.NumericRange = 1;
			else
				MenuListtemp.NumericRange = 0;
			sprintf(buf, "%d.%d",MenuListtemp.Parameter.Para_Value/10,MenuListtemp.Parameter.Para_Value%10);		
			AddUnitDispiaySETLIST(buf,&MenuListtemp);
			p = buf;
			LISTVIEW_SetItemText(hItem, 2, row, p);
			break;	
		default:
			/* �����ļ�ֵ������ */
			break;			
	}
}
/*
*********************************************************************************************************
*	�� �� ��: 
*	����˵��: ����˫���Ȳ�������
*	��    ��: 
*	�� �� ֵ: 
*********************************************************************************************************
*/
void USER_EditDoubleSetValue(WM_MESSAGE * pMsg,uint16_t key)
{
	WM_HWIN hItem;
	uint8_t row;
	char * p;
	char buf[15];
	hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW0);
	row = GetRowValue();
	switch (key) 
	{ 
		case USER_KEY_UP:
			USER_ValueListviewData(1,0);
			sprintf(buf, "%d.%02d",MenuListtemp.Parameter.Para_Value/100,MenuListtemp.Parameter.Para_Value%100);
			AddUnitDispiaySETLIST(buf,&MenuListtemp);
			p = buf;
			LISTVIEW_SetItemText(hItem, 2, row, p);
			break;

		case USER_KEY_DOWN:
			USER_ValueListviewData(0,0);
			sprintf(buf, "%d.%02d",MenuListtemp.Parameter.Para_Value/100,MenuListtemp.Parameter.Para_Value%100);
			p = buf;
			LISTVIEW_SetItemText(hItem, 2, row, p);
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
			if(GetBufIndValue())//��������
			{
				MenuListtemp.Parameter.Para_Value = MenuListtemp.Parameter.Para_Value*10 + DigitKeyToChr(key);
			}
			else//��������
			{
				MenuListtemp.Parameter.Para_Value = DigitKeyToChr(key);
				SetBufIndValue(1);//��������
			}
			if(MenuListtemp.Parameter.Para_Value > MenuListtemp.Parameter.Max_Value)
			{
				MenuListtemp.Parameter.Para_Value = MenuListtemp.Parameter.Max_Value;
				MenuListtemp.NumericRange = 0;
			}
			else if(MenuListtemp.Parameter.Para_Value < MenuListtemp.Parameter.Min_Value)
//				MenuListtemp.Parameter.Para_Value = MenuListtemp.Parameter.Min_Value;
				MenuListtemp.NumericRange = 1;
			else
				MenuListtemp.NumericRange = 0;
			sprintf(buf, "%d.%02d",MenuListtemp.Parameter.Para_Value/100,MenuListtemp.Parameter.Para_Value%100);		
			AddUnitDispiaySETLIST(buf,&MenuListtemp);
			p = buf;
			LISTVIEW_SetItemText(hItem, 2, row, p);
			break;	
		default:
			/* �����ļ�ֵ������ */
			break;			
	}
}
/*
*********************************************************************************************************
*	�� �� ��: 
*	����˵��: �����ŵ����Ͳ�������
*	��    ��: 
*	�� �� ֵ: 
*********************************************************************************************************
*/
void USER_EditSignedIntSetValue(WM_MESSAGE * pMsg,uint16_t key)
{
	WM_HWIN hItem;
	uint8_t row;
	static BOOL sing = 0;
	char * p;
	char buf[15];
	hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW0);
	row = GetRowValue();
	switch (key) 
	{ 
		case USER_KEY_UP:
			USER_ValueListviewData(1,1);
			sprintf(buf, "%d",(int16_t)MenuListtemp.Parameter.Para_Value);
			AddUnitDispiaySETLIST(buf,&MenuListtemp);
			p = buf;
			LISTVIEW_SetItemText(hItem, 2, row, p);
			break;
		case USER_KEY_DOWN:
			USER_ValueListviewData(0,1);
			sprintf(buf, "%d",(int16_t)MenuListtemp.Parameter.Para_Value);	
			AddUnitDispiaySETLIST(buf,&MenuListtemp);
			p = buf;
			LISTVIEW_SetItemText(hItem, 2, row, p);
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
		case USER_KEY_LEFT3:
			if(GetBufIndValue())//��������
			{	
				if(key == USER_KEY_LEFT3)
				{
					sing = !sing; 
					MenuListtemp.Parameter.Para_Value = ~MenuListtemp.Parameter.Para_Value+1;
				}
				else
				{
					if(sing)
						MenuListtemp.Parameter.Para_Value = MenuListtemp.Parameter.Para_Value*10 - DigitKeyToChr(key);
					else
						MenuListtemp.Parameter.Para_Value = MenuListtemp.Parameter.Para_Value*10 + DigitKeyToChr(key);
				}
			}
			else//��������
			{
				if(MenuListtemp.Parameter.Para_Value > (INT16_MAX+1))
					sing = 1; 	
				if(key == USER_KEY_LEFT3)
				{
					sing = !sing; 
					MenuListtemp.Parameter.Para_Value = ~MenuListtemp.Parameter.Para_Value+1;
				}
				else 
				{
					MenuListtemp.Parameter.Para_Value = DigitKeyToChr(key);
					if(sing)
					{
						MenuListtemp.Parameter.Para_Value = ~MenuListtemp.Parameter.Para_Value+1;
					}
					SetBufIndValue(1);//��������
				}
			}
			if((int16_t)MenuListtemp.Parameter.Para_Value > (int16_t)MenuListtemp.Parameter.Max_Value)
			{
				MenuListtemp.Parameter.Para_Value = MenuListtemp.Parameter.Max_Value;
				MenuListtemp.NumericRange = 0;
			}
			else if((int16_t)MenuListtemp.Parameter.Para_Value < (int16_t)MenuListtemp.Parameter.Min_Value)
			{
//				MenuListtemp.Parameter.Para_Value = MenuListtemp.Parameter.Min_Value;
				MenuListtemp.NumericRange = 1;
			}
			else
				MenuListtemp.NumericRange = 0;
			sprintf(buf, "%d",(int16_t)MenuListtemp.Parameter.Para_Value);		
			AddUnitDispiaySETLIST(buf,&MenuListtemp);
			p = buf;
			LISTVIEW_SetItemText(hItem, 2, row, p);
			break;	
		default:
			/* �����ļ�ֵ������ */
			break;			
	}
}
/*
*********************************************************************************************************
*	�� �� ��: 
*	����˵��: ��ʼ���ò���
*	��    ��: 
*	�� �� ֵ: 
*********************************************************************************************************
*/
void USER_SetValueListviewData(WM_MESSAGE * pMsg,uint16_t key,uint8_t val)
{	
	if(MenuListtemp.Parameter.Input_Type > 99)
	{
		USER_EditSelSetValue(pMsg,key);
	}
	switch (MenuListtemp.Parameter.Input_Type) 
	{ 
		case EDIT_SEL:
		case EDIT_SEL_CMD:
			if(val == 0)
				USER_EditSelSetValue(pMsg,key);
			else
				ServeUSER_EditSelSetValue(pMsg,key);
			break;
		case EDIT_INT:
		    USER_EditIntSetValue(pMsg,key);
			break;
		case EDIT_FLOAT:
			USER_EditFloatSetValue(pMsg,key);
			break;
		case EDIT_DOUBLE:
			USER_EditDoubleSetValue(pMsg,key);
			break;
		case EDIT_CMD:
			break;
		case EDIT_SIGNED_INT:
			USER_EditSignedIntSetValue(pMsg,key);
			break;
		default:
			break;
	}
}
extern uint64_t Real_ID;
uint16_t Send_parameter_flag = 0;
/*
*********************************************************************************************************
*	�� �� ��: 
*	����˵��: ��������Ĵ���
*	��    ��: 
*	�� �� ֵ: 
*********************************************************************************************************
*/
uint8_t USER_SaveValueListviewData(WM_MESSAGE * pMsg,uint16_t key,uint64_t id,uint8_t val)
{
	uint8_t row,flag = 0;
	uint8_t server[20];
	uint16_t temp; 
	row = GetRowValue();
	if(val ==0)
	{
		//ȫ������Ĵ���
		if(MenuListtemp.Parameter.Modift_Type == 1)
		{

			//ȫ��
			if(key == USER_KEY_LEFT1)
			{
				if(MenuListtemp.NumericRange == 1)
				{
					goto abnormal;
				}
				SetMenuListtempValue(&MenuListtemp);
				MenuList[row].Para_Value = MenuListtemp.Parameter.Para_Value;
				SysParamFun(DATA_MSG_WRITE_OBJECT_PARAM,id,1,MenuListtemp.Deviation,&MenuListtemp.Parameter.Para_Value,0,1,1);
				GetShowAgain(0);
				flag = 1;
			}
		}
		//����&ȫ������Ĵ���	
		if(MenuListtemp.Parameter.Modift_Type == 3)
		{
			//ȫ��
			if(key == USER_KEY_LEFT1)
			{
				if(MenuListtemp.NumericRange == 1)
				{
					goto abnormal;
				}
				SetMenuListtempValue(&MenuListtemp);
				MenuList[row].Para_Value = MenuListtemp.Parameter.Para_Value;
				SysParamFun(DATA_MSG_WRITE_OBJECT_PARAM,id,1,MenuListtemp.Deviation,&MenuListtemp.Parameter.Para_Value,0,1,1);
				GetShowAgain(0);
				flag = 1;
			}
			//����
			if(key == USER_KEY_LEFT2)
			{
				if(MenuListtemp.NumericRange == 1)
				{
					goto abnormal;
				}
				SetMenuListtempValue(&MenuListtemp);
				MenuList[row].Para_Value = MenuListtemp.Parameter.Para_Value;
				SysParamFun(DATA_MSG_WRITE_OBJECT_PARAM,id,1,MenuListtemp.Deviation,&MenuListtemp.Parameter.Para_Value,0,0,1);
				GetShowAgain(0);
				flag = 1;
			}
		}
		if(MenuListtemp.Parameter.Modift_Type == 4)
		{
			if(key == USER_KEY_LEFT1)
			{
				flag = 1;
				switch(MenuListtemp.Parameter.Input_Type)
				{
					case 100:
						if(MenuListtemp.Parameter.Para_Value == 0)//benjia
						{
							ETH_Stop();
							BCM_551CS_LOW();
							NVIC_SystemReset(); 
						}
						else if(MenuListtemp.Parameter.Para_Value == 1)//quanmian
						{
							SysDataFun(DATA_MSG_RESET_ALL_CONTROLLER,0,0,0,0,0,0);
						}
						break;
					case 106:					
					case 107:
						Send_parameter_flag = 1;
						if(MenuListtemp.Parameter.Para_Value == 0)//zuojia
						{
							SendFile_API(PARATYPE,0,MenuListtemp.Parameter.Input_Type,UPDATELEFT);
						}
						else if(MenuListtemp.Parameter.Para_Value == 1)//youjia
						{
							SendFile_API(PARATYPE,0,MenuListtemp.Parameter.Input_Type,UPDATERIGHT);
						}
						else if(MenuListtemp.Parameter.Para_Value == 2)//quanmian
						{
							SendFile_API(PARATYPE,0,MenuListtemp.Parameter.Input_Type,UPDATEALL);
						}
						
						break;
						
					case 108:
						if(MenuListtemp.Parameter.Para_Value == 0)//benjia
						{
							SysDataFun(DATA_MSG_RESET_DEFAULTPARAM,0,0,0,0,0,0);
						}
						else if(MenuListtemp.Parameter.Para_Value == 1)//quanmian
						{
							SysDataFun(DATA_MSG_RESET_ALL_DEFAULTPARAM,0,0,0,0,0,0);
						}
						break;		
						
					case 102:
						
						break;	
					default:				
						break;
				}
			}
		}
	}
	else
	{
		if(key == USER_KEY_LEFT1)
		{
			if(MenuListtemp.NumericRange == 1)
			{
				goto abnormal;
			}
			MenuList[row].Para_Value = MenuListtemp.Parameter.Para_Value;
			memcpy(&server[0],&Real_ID,8);
			temp = 1;
			memcpy(&server[8],(u8 *)(&temp),2);
			temp = 2;
			memcpy(&server[10],(u8 *)(&temp),2);
			temp = MenuListtemp.Deviation;
			memcpy(&server[12],(u8 *)(&temp),2);
			temp = MenuListtemp.Parameter.Para_Value;
			memcpy(&server[14],(u8 *)(&temp),2);
			temp = MenuListtemp.Parameter.Input_Type;
			memcpy(&server[16],(u8 *)(&temp),2);
			UdpLinkLayerSend_app(server,18,0,0 ,0,AppProcIDType_serverRevise_request);
			flag = 1;
		}
	}
	//�˳���δ���棩�Ĵ���
	if(key == USER_KEY_LEFT)
	{
	abnormal:
		LISTVIEW_SetItemText(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW0), 2, row, parabuf);
		flag = 1;
	}
	return flag;
}









