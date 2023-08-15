/*
*********************************************************************************************************
*	                                  
*	模块名称 : User_Listview用户库
*	文件名称 : User_Listview.c
*	版    本 : V1.0
*	说    明 : 实现按键各种功能
*              
*	修改记录 :
*		版本号   日期         作者          说明
*		V1.0    2016-07-16   Eric2013  	    首版    
*                                     
*	Copyright (C), 2015-2020, 安富莱电子 www.armfly.com
*
*********************************************************************************************************
*/
#include "User_Listview.h"
#include "MenuTask.h"
#include "includes.h"
#include "sys_msg_def.h"
extern const char *SelectItem[][15];
/*
*********************************************************************************************************
*	                                     宏定义
*********************************************************************************************************
*/
 SETLIST   MenuListtemp;
 INITLIST   MenuList[300];
/*
*********************************************************************************************************
*	                                   序号
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
*	函 数 名: 
*	功能说明: 初始化列表
*	形    参: 
*	返 回 值: 
*********************************************************************************************************
*/
void USER_ReviseListview(WM_MESSAGE * pMsg)
{
	WM_HWIN hItem;
	hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW0);
	LISTVIEW_SetHeaderHeight(hItem,50);         //设置标题的高度
	LISTVIEW_SetRowHeight(hItem,40);            //设置LISTVIEW的行高
	LISTVIEW_SetBkColor(hItem,LISTVIEW_CI_UNSEL,GUI_BLACK);    //设置背景颜色
	LISTVIEW_SetBkColor(hItem,LISTVIEW_CI_SEL,GUI_BLACK);    //设置背景颜色
	LISTVIEW_SetBkColor(hItem,LISTVIEW_CI_SELFOCUS,GUI_WHITE);    //设置背景颜色
	LISTVIEW_SetFont(hItem, &GUI_FontHZ24);
	LISTVIEW_SetTextColor(hItem,LISTVIEW_CI_UNSEL,GUI_WHITE);
	LISTVIEW_SetTextColor(hItem,LISTVIEW_CI_SEL,GUI_WHITE);
	LISTVIEW_SetTextColor(hItem,LISTVIEW_CI_SELFOCUS,GUI_BLACK);

}
/*
*********************************************************************************************************
*	函 数 名: 
*	功能说明: 添加列表头
*	形    参: 
*	返 回 值: 
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
		//向LISTVIEW添加列
		LISTVIEW_AddColumn(hItem,hader[i].wide, hader[i].u8headerMame,  pig_site[i] | GUI_TA_VCENTER); 
	}
}

/*
*********************************************************************************************************
*	函 数 名: 
*	功能说明: 获取所有需要显示的参数
*	形    参: 
*	返 回 值: 
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
	SysParamFun(DATA_MSG_READ_OBJECT_PARAM,id,0,0,&para,0,0);
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
*	函 数 名: 
*	功能说明: 显示列表
*	形    参: 
*	返 回 值: 
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
		if(MenuList[i].Parameter->Input_Type == EDIT_SEL)
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
*	函 数 名: 
*	功能说明: 选择第0行
*	形    参: 
*	返 回 值: 
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
*	函 数 名: 
*	功能说明: 移动选择的行
*	形    参: 
*	返 回 值: 
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
*	函 数 名: 
*	功能说明: 根据参数类型显示按键名称
*	形    参: 
*	返 回 值: 
*********************************************************************************************************
*/
void USER_DisplyButtonNameByType(WM_MESSAGE * pMsg)
{
	const char* oveaall = {"全面"};
	const char* local = {"本地"};
	const char* negative = {"+/-"};
//	const char* decimal = {"."};
	USER_ClearAllButtonShow(pMsg);
	USER_ReviseButtonShow(pMsg,MENU_BUTTON_LEFT_ID1,oveaall);
	if(MenuListtemp.Parameter.Modift_Type == 3)
		USER_ReviseButtonShow(pMsg,MENU_BUTTON_LEFT_ID2,local);
	if(MenuListtemp.Parameter.Input_Type == EDIT_SIGNED_INT)
		USER_ReviseButtonShow(pMsg,MENU_BUTTON_LEFT_ID3,negative);
//	if(MenuListtemp.Input_Type == EDIT_FLOAT)
//		USER_ReviseButtonShow(pMsg,MENU_BUTTON_LEFT_ID3,decimal);
}
/*
*********************************************************************************************************
*	函 数 名: 
*	功能说明: 确定设置当前行
*	形    参: 
*	返 回 值: 
*********************************************************************************************************
*/
char  pbuf[8];
void USER_EnterListviewRow(WM_MESSAGE * pMsg)
{
	uint8_t	rank[3] = {0};
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
	USER_DisplyButtonNameByType(pMsg);
	LISTVIEW_GetItemText(hItem,2,Row,pbuf,8);
}
/*
*********************************************************************************************************
*	函 数 名: 
*	功能说明: 退出设置当前行
*	形    参: 
*	返 回 值: 
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
*	函 数 名: 
*	功能说明: 参数值+1或者-1
*	形    参: 
*	返 回 值: 
*********************************************************************************************************
*/
void USER_ValueListviewData(uint8_t Direction,uint8_t sign)
{
	if(sign == 0)//参数为无符号类型
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
	else//有符号类型
	{
		if(Direction)
		{
			if(((int16_t)MenuListtemp.Parameter.Para_Value+1) > MenuListtemp.Parameter.Max_Value)
				MenuListtemp.Parameter.Para_Value = MenuListtemp.Parameter.Min_Value;
			else
				MenuListtemp.Parameter.Para_Value += 1;  
		}
		else
		{
			if(((int16_t)MenuListtemp.Parameter.Para_Value-1) < MenuListtemp.Parameter.Min_Value)
				MenuListtemp.Parameter.Para_Value = MenuListtemp.Parameter.Max_Value;
			else
				MenuListtemp.Parameter.Para_Value -= 1;  
		}
	}
	
}
/*
*********************************************************************************************************
*	函 数 名: 
*	功能说明: 选择型参数输入
*	形    参: 
*	返 回 值: 
*********************************************************************************************************
*/
void USER_EditSelSetValue(WM_MESSAGE * pMsg,uint16_t key)
{
	WM_HWIN hItem;
	uint8_t row,i;
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
		/* 其它的键值不处理 */
			break;
	}
}
/*
*********************************************************************************************************
*	函 数 名: 
*	功能说明: 整型参数输入
*	形    参: 
*	返 回 值: 
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
			if(GetBufIndValue())//连续输入
			{
				MenuListtemp.Parameter.Para_Value = MenuListtemp.Parameter.Para_Value*10 + DigitKeyToChr(key);
			}
			else//重新输入
			{
				MenuListtemp.Parameter.Para_Value = DigitKeyToChr(key);
				SetBufIndValue(1);//连续输入
			}
			if(MenuListtemp.Parameter.Para_Value > MenuListtemp.Parameter.Max_Value)
				MenuListtemp.Parameter.Para_Value = MenuListtemp.Parameter.Max_Value;
			if(MenuListtemp.Parameter.Para_Value < MenuListtemp.Parameter.Min_Value)
				MenuListtemp.Parameter.Para_Value = MenuListtemp.Parameter.Min_Value;
			sprintf(buf, "%d",MenuListtemp.Parameter.Para_Value);	
			AddUnitDispiaySETLIST(buf,&MenuListtemp);			
			p = buf;
			LISTVIEW_SetItemText(hItem, 2, row, p);
			break;	
		default:
			/* 其它的键值不处理 */
			break;			
	}
}
/*
*********************************************************************************************************
*	函 数 名: 
*	功能说明: 浮点单精度参数输入
*	形    参: 
*	返 回 值: 
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
			if(GetBufIndValue())//连续输入
			{
				MenuListtemp.Parameter.Para_Value = MenuListtemp.Parameter.Para_Value*10 + DigitKeyToChr(key);
			}
			else//重新输入
			{
				MenuListtemp.Parameter.Para_Value = DigitKeyToChr(key);
				SetBufIndValue(1);//连续输入
			}
			if(MenuListtemp.Parameter.Para_Value > MenuListtemp.Parameter.Max_Value)
				MenuListtemp.Parameter.Para_Value = MenuListtemp.Parameter.Max_Value;
			if(MenuListtemp.Parameter.Para_Value < MenuListtemp.Parameter.Min_Value)
				MenuListtemp.Parameter.Para_Value = MenuListtemp.Parameter.Min_Value;
			sprintf(buf, "%d.%d",MenuListtemp.Parameter.Para_Value/10,MenuListtemp.Parameter.Para_Value%10);		
			AddUnitDispiaySETLIST(buf,&MenuListtemp);
			p = buf;
			LISTVIEW_SetItemText(hItem, 2, row, p);
			break;	
		default:
			/* 其它的键值不处理 */
			break;			
	}
}
/*
*********************************************************************************************************
*	函 数 名: 
*	功能说明: 浮点双精度参数输入
*	形    参: 
*	返 回 值: 
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
			if(GetBufIndValue())//连续输入
			{
				MenuListtemp.Parameter.Para_Value = MenuListtemp.Parameter.Para_Value*10 + DigitKeyToChr(key);
			}
			else//重新输入
			{
				MenuListtemp.Parameter.Para_Value = DigitKeyToChr(key);
				SetBufIndValue(1);//连续输入
			}
			if(MenuListtemp.Parameter.Para_Value > MenuListtemp.Parameter.Max_Value)
				MenuListtemp.Parameter.Para_Value = MenuListtemp.Parameter.Max_Value;
			if(MenuListtemp.Parameter.Para_Value < MenuListtemp.Parameter.Min_Value)
				MenuListtemp.Parameter.Para_Value = MenuListtemp.Parameter.Min_Value;
			sprintf(buf, "%d.%02d",MenuListtemp.Parameter.Para_Value/100,MenuListtemp.Parameter.Para_Value%100);		
			AddUnitDispiaySETLIST(buf,&MenuListtemp);
			p = buf;
			LISTVIEW_SetItemText(hItem, 2, row, p);
			break;	
		default:
			/* 其它的键值不处理 */
			break;			
	}
}
/*
*********************************************************************************************************
*	函 数 名: 
*	功能说明: 带符号的整型参数输入
*	形    参: 
*	返 回 值: 
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
			if(GetBufIndValue())//连续输入
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
			else//重新输入
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
					SetBufIndValue(1);//连续输入
				}
			}
			if((int16_t)MenuListtemp.Parameter.Para_Value > MenuListtemp.Parameter.Max_Value)
				MenuListtemp.Parameter.Para_Value = MenuListtemp.Parameter.Max_Value;
			if((int16_t)MenuListtemp.Parameter.Para_Value < MenuListtemp.Parameter.Min_Value)
				MenuListtemp.Parameter.Para_Value = MenuListtemp.Parameter.Min_Value;
			sprintf(buf, "%d",(int16_t)MenuListtemp.Parameter.Para_Value);		
			AddUnitDispiaySETLIST(buf,&MenuListtemp);
			p = buf;
			LISTVIEW_SetItemText(hItem, 2, row, p);
			break;	
		default:
			/* 其它的键值不处理 */
			break;			
	}
}
/*
*********************************************************************************************************
*	函 数 名: 
*	功能说明: 开始设置参数
*	形    参: 
*	返 回 值: 
*********************************************************************************************************
*/
void USER_SetValueListviewData(WM_MESSAGE * pMsg,uint16_t key)
{
	uint8_t i,row = 0;
	char * p;
	char buf[9];
	row = GetRowValue();
	
	switch (MenuListtemp.Parameter.Input_Type) 
	{ 
		case EDIT_SEL:
		case EDIT_SEL_CMD:
			USER_EditSelSetValue(pMsg,key);
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

/*
*********************************************************************************************************
*	函 数 名: 
*	功能说明: 保存参数的处理
*	形    参: 
*	返 回 值: 
*********************************************************************************************************
*/
uint8_t USER_SaveValueListviewData(WM_MESSAGE * pMsg,uint16_t key,uint64_t id)
{
	uint8_t row,flag = 0;
	row = GetRowValue();
	//全面参数的处理
	if(MenuListtemp.Parameter.Modift_Type == 1)
	{
		//全面
		if(key == USER_KEY_LEFT1)
		{
			SetMenuListtempValue(&MenuListtemp);
			SysParamFun(DATA_MSG_WRITE_OBJECT_PARAM,id,1,MenuListtemp.Deviation,&MenuListtemp.Parameter.Para_Value,0,1);
			GetShowAgain();
			flag = 1;
		}
	}
	//本地&全面参数的处理	
	if(MenuListtemp.Parameter.Modift_Type == 3)
	{
		//全面
		if(key == USER_KEY_LEFT1)
		{
			SetMenuListtempValue(&MenuListtemp);
			SysParamFun(DATA_MSG_WRITE_OBJECT_PARAM,id,1,MenuListtemp.Deviation,&MenuListtemp.Parameter.Para_Value,0,1);
			GetShowAgain();
			flag = 1;
		}
		//本地
		if(key == USER_KEY_LEFT2)
		{
			SetMenuListtempValue(&MenuListtemp);
			SysParamFun(DATA_MSG_WRITE_OBJECT_PARAM,id,1,MenuListtemp.Deviation,&MenuListtemp.Parameter.Para_Value,0,0);
			GetShowAgain();
			flag = 1;
		}
	}
	//退出（未保存）的处理
	if(key == USER_KEY_LEFT)
	{
		LISTVIEW_SetItemText(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW0), 2, row, pbuf);
		flag = 1;
	}
	return flag;
}









