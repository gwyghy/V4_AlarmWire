/*
*********************************************************************************************************
*	                                  
*	ģ������ : TEXT�û���
*	�ļ����� : User_Text.c
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
#include "User_Text.h"
/*
*********************************************************************************************************
*	                                     �궨��
*********************************************************************************************************
*/
void CreateSlveContorTextShow(WM_MESSAGE * pMsg)
{
	WM_HWIN hItem;	
	
	
	TEXT_CreateEx(352-60,23,64,20,pMsg->hWin,WM_CF_SHOW,TEXT_CF_HCENTER|TEXT_CF_VCENTER,GUI_ID_TEXT41," ");
	
	TEXT_CreateEx(125-60,4,230,45,pMsg->hWin,WM_CF_SHOW,TEXT_CF_HCENTER|TEXT_CF_VCENTER,GUI_ID_TEXT42," ");
	
	TEXT_CreateEx(34,60,128,40,pMsg->hWin,WM_CF_SHOW,TEXT_CF_HCENTER|TEXT_CF_VCENTER,GUI_ID_TEXT43," ");
	
	TEXT_CreateEx(198,60,128,40,pMsg->hWin,WM_CF_SHOW,TEXT_CF_HCENTER|TEXT_CF_VCENTER,GUI_ID_TEXT44," ");
	
	TEXT_CreateEx(34,100,128,40,pMsg->hWin,WM_CF_SHOW,TEXT_CF_HCENTER|TEXT_CF_VCENTER,GUI_ID_TEXT45," ");
	
	TEXT_CreateEx(198,100,128,40,pMsg->hWin,WM_CF_SHOW,TEXT_CF_HCENTER|TEXT_CF_VCENTER,GUI_ID_TEXT46," ");
	
	TEXT_CreateEx(34,140,128,40,pMsg->hWin,WM_CF_SHOW,TEXT_CF_HCENTER|TEXT_CF_VCENTER,GUI_ID_TEXT47," ");
	
	TEXT_CreateEx(198,140,128,40,pMsg->hWin,WM_CF_SHOW,TEXT_CF_HCENTER|TEXT_CF_VCENTER,GUI_ID_TEXT48," ");

	

	
	hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT41);
	TEXT_SetFont(hItem, &GUI_FontHZ16);
	TEXT_SetText(hItem, "  ");
	TEXT_SetTextColor(hItem,GUI_WHITE);
//	TEXT_SetBkColor(hItem,GUI_WHITE);
	TEXT_SetTextAlign(hItem,GUI_TA_HCENTER | GUI_TA_VCENTER);

	
	hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT42);
	TEXT_SetFont(hItem, &GUI_FontHZ32);
	TEXT_SetText(hItem,"���ܱ�����");
	TEXT_SetTextColor(hItem,GUI_WHITE);
//	TEXT_SetBkColor(hItem,GUI_GRAY);
	TEXT_SetTextAlign(hItem,GUI_TA_HCENTER | GUI_TA_VCENTER);	
	
	hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT43);
	TEXT_SetFont(hItem, &GUI_FontHZ32);
	TEXT_SetText(hItem," ");
	TEXT_SetTextColor(hItem,GUI_WHITE);
//	TEXT_SetBkColor(hItem,GUI_GRAY);
	TEXT_SetTextAlign(hItem,GUI_TA_HCENTER | GUI_TA_VCENTER);
	
	hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT44);
	TEXT_SetFont(hItem, &GUI_FontHZ32);
	TEXT_SetText(hItem," ");
	TEXT_SetTextColor(hItem,GUI_WHITE);
//	TEXT_SetBkColor(hItem,GUI_GRAY);
	TEXT_SetTextAlign(hItem,GUI_TA_HCENTER | GUI_TA_VCENTER);
	
		hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT45);
	TEXT_SetFont(hItem, &GUI_FontHZ32);
	TEXT_SetText(hItem," ");
	TEXT_SetTextColor(hItem,GUI_WHITE);
//	TEXT_SetBkColor(hItem,GUI_GRAY);
	TEXT_SetTextAlign(hItem,GUI_TA_HCENTER | GUI_TA_VCENTER);
	
	hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT46);
	TEXT_SetFont(hItem, &GUI_FontHZ32);
	TEXT_SetText(hItem," ");
	TEXT_SetTextColor(hItem,GUI_WHITE);
//	TEXT_SetBkColor(hItem,GUI_GRAY);
	TEXT_SetTextAlign(hItem,GUI_TA_HCENTER | GUI_TA_VCENTER);
	
	hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT47);
	TEXT_SetFont(hItem, &GUI_FontHZ32);
	TEXT_SetText(hItem," ");
	TEXT_SetTextColor(hItem,GUI_WHITE);
//	TEXT_SetBkColor(hItem,GUI_GRAY);
	TEXT_SetTextAlign(hItem,GUI_TA_HCENTER | GUI_TA_VCENTER);
	
	hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT48);
	TEXT_SetFont(hItem, &GUI_FontHZ32);
	TEXT_SetText(hItem," ");
	TEXT_SetTextColor(hItem,GUI_WHITE);
//	TEXT_SetBkColor(hItem,GUI_GRAY);
	TEXT_SetTextAlign(hItem,GUI_TA_HCENTER | GUI_TA_VCENTER);
}
void CreateRackContorTextShow(WM_MESSAGE * pMsg)
{
	WM_HWIN hItem;	
	TEXT_CreateEx(106-60,90,268,35,pMsg->hWin,WM_CF_SHOW,TEXT_CF_HCENTER|TEXT_CF_VCENTER,GUI_ID_TEXT40," ");
	
	TEXT_CreateEx(106-60,90+40+50,268,35,pMsg->hWin,WM_CF_SHOW,TEXT_CF_HCENTER|TEXT_CF_VCENTER,GUI_ID_TEXT41," ");
	
	TEXT_CreateEx(125-60,4,230,45,pMsg->hWin,WM_CF_SHOW,TEXT_CF_HCENTER|TEXT_CF_VCENTER,GUI_ID_TEXT42," ");
	
	TEXT_CreateEx(352-60,23,64,20,pMsg->hWin,WM_CF_SHOW,TEXT_CF_HCENTER|TEXT_CF_VCENTER,GUI_ID_TEXT43," ");
	

	hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT40);
	TEXT_SetFont(hItem, &GUI_FontHZ32);
	TEXT_SetText(hItem, "  ");
	TEXT_SetTextColor(hItem,GUI_WHITE);
//	TEXT_SetBkColor(hItem,GUI_WHITE);
	TEXT_SetTextAlign(hItem,GUI_TA_HCENTER | GUI_TA_VCENTER);

	hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT41);
	TEXT_SetFont(hItem, &GUI_FontHZ32);
	TEXT_SetText(hItem, "=�밴������=");
	TEXT_SetTextColor(hItem,GUI_LIGHTYELLOW);
//	TEXT_SetBkColor(hItem,GUI_WHITE);
	TEXT_SetTextAlign(hItem,GUI_TA_HCENTER | GUI_TA_VCENTER);

	
	hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT42);
	TEXT_SetFont(hItem, &GUI_FontHZ32);
	TEXT_SetText(hItem,"���ܿ���");
	TEXT_SetTextColor(hItem,GUI_WHITE);
//	TEXT_SetBkColor(hItem,GUI_GRAY);
	TEXT_SetTextAlign(hItem,GUI_TA_HCENTER | GUI_TA_VCENTER);	
	
	hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT43);
	TEXT_SetFont(hItem, &GUI_FontHZ16);
	TEXT_SetText(hItem, "����1/3");
	TEXT_SetTextColor(hItem,GUI_WHITE);
//	TEXT_SetBkColor(hItem,GUI_WHITE);
	TEXT_SetTextAlign(hItem,GUI_TA_HCENTER | GUI_TA_VCENTER);
}
void CreateMasterContorTextShow(WM_MESSAGE * pMsg)
{
	WM_HWIN hItem;	
	TEXT_CreateEx(65-60,23,64,20,pMsg->hWin,WM_CF_SHOW,TEXT_CF_HCENTER|TEXT_CF_VCENTER,GUI_ID_TEXT40," ");
	
	TEXT_CreateEx(352-60,23,64,20,pMsg->hWin,WM_CF_SHOW,TEXT_CF_HCENTER|TEXT_CF_VCENTER,GUI_ID_TEXT41," ");
	
	TEXT_CreateEx(125-60,4,230,45,pMsg->hWin,WM_CF_SHOW,TEXT_CF_HCENTER|TEXT_CF_VCENTER,GUI_ID_TEXT42," ");
	
	TEXT_CreateEx(34,60,128,40,pMsg->hWin,WM_CF_SHOW,TEXT_CF_HCENTER|TEXT_CF_VCENTER,GUI_ID_TEXT43," ");
	
	TEXT_CreateEx(198,60,128,40,pMsg->hWin,WM_CF_SHOW,TEXT_CF_HCENTER|TEXT_CF_VCENTER,GUI_ID_TEXT44," ");
	
	TEXT_CreateEx(34,100,128,40,pMsg->hWin,WM_CF_SHOW,TEXT_CF_HCENTER|TEXT_CF_VCENTER,GUI_ID_TEXT45," ");
	
	TEXT_CreateEx(198,100,128,40,pMsg->hWin,WM_CF_SHOW,TEXT_CF_HCENTER|TEXT_CF_VCENTER,GUI_ID_TEXT46," ");
	
	TEXT_CreateEx(34,140,128,40,pMsg->hWin,WM_CF_SHOW,TEXT_CF_HCENTER|TEXT_CF_VCENTER,GUI_ID_TEXT47," ");
	
	TEXT_CreateEx(198,140,128,40,pMsg->hWin,WM_CF_SHOW,TEXT_CF_HCENTER|TEXT_CF_VCENTER,GUI_ID_TEXT48," ");
	
	TEXT_CreateEx(34,210,290,40,pMsg->hWin,WM_CF_SHOW,TEXT_CF_HCENTER|TEXT_CF_VCENTER,GUI_ID_TEXT49," ");

	hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT40);
	TEXT_SetFont(hItem, &GUI_FontHZ16);
	TEXT_SetText(hItem, "  ");
	TEXT_SetTextColor(hItem,GUI_WHITE);
//	TEXT_SetBkColor(hItem,GUI_WHITE);
	TEXT_SetTextAlign(hItem,GUI_TA_HCENTER | GUI_TA_VCENTER);

	hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT41);
	TEXT_SetFont(hItem, &GUI_FontHZ16);
	TEXT_SetText(hItem, "����1/3");
	TEXT_SetTextColor(hItem,GUI_WHITE);
//	TEXT_SetBkColor(hItem,GUI_WHITE);
	TEXT_SetTextAlign(hItem,GUI_TA_HCENTER | GUI_TA_VCENTER);

	
	hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT42);
	TEXT_SetFont(hItem, &GUI_FontHZ32);
	TEXT_SetText(hItem,"���ڼܿ���");
	TEXT_SetTextColor(hItem,GUI_WHITE);
//	TEXT_SetBkColor(hItem,GUI_GRAY);
	TEXT_SetTextAlign(hItem,GUI_TA_HCENTER | GUI_TA_VCENTER);	
	

	
	hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT43);
	TEXT_SetFont(hItem, &GUI_FontHZ32);
	TEXT_SetText(hItem," ");
	TEXT_SetTextColor(hItem,GUI_WHITE);
//	TEXT_SetBkColor(hItem,GUI_GRAY);
	TEXT_SetTextAlign(hItem,GUI_TA_HCENTER | GUI_TA_VCENTER);
	
	hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT44);
	TEXT_SetFont(hItem, &GUI_FontHZ32);
	TEXT_SetText(hItem," ");
	TEXT_SetTextColor(hItem,GUI_WHITE);
//	TEXT_SetBkColor(hItem,GUI_GRAY);
	TEXT_SetTextAlign(hItem,GUI_TA_HCENTER | GUI_TA_VCENTER);
	
		hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT45);
	TEXT_SetFont(hItem, &GUI_FontHZ32);
	TEXT_SetText(hItem," ");
	TEXT_SetTextColor(hItem,GUI_WHITE);
//	TEXT_SetBkColor(hItem,GUI_GRAY);
	TEXT_SetTextAlign(hItem,GUI_TA_HCENTER | GUI_TA_VCENTER);
	
	hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT46);
	TEXT_SetFont(hItem, &GUI_FontHZ32);
	TEXT_SetText(hItem," ");
	TEXT_SetTextColor(hItem,GUI_WHITE);
//	TEXT_SetBkColor(hItem,GUI_GRAY);
	TEXT_SetTextAlign(hItem,GUI_TA_HCENTER | GUI_TA_VCENTER);
	
	hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT47);
	TEXT_SetFont(hItem, &GUI_FontHZ32);
	TEXT_SetText(hItem," ");
	TEXT_SetTextColor(hItem,GUI_WHITE);
//	TEXT_SetBkColor(hItem,GUI_GRAY);
	TEXT_SetTextAlign(hItem,GUI_TA_HCENTER | GUI_TA_VCENTER);
	
	hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT48);
	TEXT_SetFont(hItem, &GUI_FontHZ32);
	TEXT_SetText(hItem," ");
	TEXT_SetTextColor(hItem,GUI_WHITE);
//	TEXT_SetBkColor(hItem,GUI_GRAY);
	TEXT_SetTextAlign(hItem,GUI_TA_HCENTER | GUI_TA_VCENTER);
	
	hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT49);
	TEXT_SetFont(hItem, &GUI_FontHZ32);
	TEXT_SetText(hItem," ");
	TEXT_SetTextColor(hItem,GUI_WHITE);
//	TEXT_SetBkColor(hItem,GUI_GRAY);
	TEXT_SetTextAlign(hItem,GUI_TA_HCENTER | GUI_TA_VCENTER);
}
void CreateGroupTextShow(WM_MESSAGE * pMsg)
{
	WM_HWIN hItem;	
	TEXT_CreateEx(352-60,23,64,20,pMsg->hWin,WM_CF_SHOW,TEXT_CF_HCENTER|TEXT_CF_VCENTER,GUI_ID_TEXT31," ");
	
	TEXT_CreateEx(106-60,90,268,35,pMsg->hWin,WM_CF_SHOW,TEXT_CF_HCENTER|TEXT_CF_VCENTER,GUI_ID_TEXT32," ");
	
	TEXT_CreateEx(95-60,147,75,37,pMsg->hWin,WM_CF_SHOW,TEXT_CF_HCENTER|TEXT_CF_VCENTER,GUI_ID_TEXT33," ");
	
	TEXT_CreateEx(306-60,147,75,37,pMsg->hWin,WM_CF_SHOW,TEXT_CF_HCENTER|TEXT_CF_VCENTER,GUI_ID_TEXT34," ");
	
	TEXT_CreateEx(125-60,4,230,45,pMsg->hWin,WM_CF_SHOW,TEXT_CF_HCENTER|TEXT_CF_VCENTER,GUI_ID_TEXT35," ");
	
//	TEXT_CreateEx(192-60,112,20,44,pMsg->hWin,WM_CF_SHOW,TEXT_CF_HCENTER|TEXT_CF_VCENTER,GUI_ID_TEXT36," ");
	hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT31);
	TEXT_SetFont(hItem, &GUI_FontHZ16);
	TEXT_SetText(hItem, "����1/3");
	TEXT_SetTextColor(hItem,GUI_WHITE);
//	TEXT_SetBkColor(hItem,GUI_WHITE);
	TEXT_SetTextAlign(hItem,GUI_TA_HCENTER | GUI_TA_VCENTER);

	hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT32);
	TEXT_SetFont(hItem, &GUI_FontHZ32);
	TEXT_SetText(hItem, "��������������");
	TEXT_SetTextColor(hItem,GUI_WHITE);
//	TEXT_SetBkColor(hItem,GUI_WHITE);
	TEXT_SetTextAlign(hItem,GUI_TA_HCENTER | GUI_TA_VCENTER);

	hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT33);
	TEXT_SetFont(hItem, &GUI_Font32B_ASCII);
	TEXT_SetText(hItem, "105");
	TEXT_SetTextColor(hItem,GUI_BLACK);
	TEXT_SetTextAlign(hItem,GUI_TA_HCENTER | GUI_TA_VCENTER);
	
	hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT34);
	TEXT_SetFont(hItem, &GUI_Font32B_ASCII);
	TEXT_SetText(hItem, "86");
	TEXT_SetTextColor(hItem,GUI_BLACK);
	TEXT_SetTextAlign(hItem,GUI_TA_HCENTER | GUI_TA_VCENTER);
	
	hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT35);
	TEXT_SetFont(hItem, &GUI_FontHZ32);
	TEXT_SetText(hItem, "���鶯������");
	TEXT_SetTextColor(hItem,GUI_WHITE);
//	TEXT_SetBkColor(hItem,GUI_WHITE);
	TEXT_SetTextAlign(hItem,GUI_TA_HCENTER | GUI_TA_VCENTER);	
}
void CreateSuddenStopTextShow(WM_MESSAGE * pMsg)
{
	WM_HWIN hItem;	
	TEXT_CreateEx(115-60,105-50,80,44,pMsg->hWin,WM_CF_HIDE,TEXT_CF_HCENTER|TEXT_CF_VCENTER,GUI_ID_TEXT21," ");
	
	TEXT_CreateEx(212-60,105-50,80,44,pMsg->hWin,WM_CF_HIDE,TEXT_CF_HCENTER|TEXT_CF_VCENTER,GUI_ID_TEXT22," ");
	
	TEXT_CreateEx(295-60,105-50,80,45,pMsg->hWin,WM_CF_HIDE,TEXT_CF_HCENTER|TEXT_CF_VCENTER,GUI_ID_TEXT23," ");
	
	TEXT_CreateEx(192-60,107-50,20,44,pMsg->hWin,WM_CF_HIDE,TEXT_CF_HCENTER|TEXT_CF_VCENTER,GUI_ID_TEXT24," ");
	
	TEXT_CreateEx(115-60,105-50,125,44,pMsg->hWin,WM_CF_HIDE,TEXT_CF_HCENTER|TEXT_CF_VCENTER,GUI_ID_TEXT25," ");
	
	TEXT_CreateEx(240-60,105-50,125,44,pMsg->hWin,WM_CF_HIDE,TEXT_CF_HCENTER|TEXT_CF_VCENTER,GUI_ID_TEXT26," ");
	
	
	hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT21);
	TEXT_SetFont(hItem, &GUI_FontHZ40x40);
	TEXT_SetText(hItem, "��ͣ");
	TEXT_SetTextColor(hItem,GUI_WHITE);
	TEXT_SetTextAlign(hItem,GUI_TA_HCENTER | GUI_TA_VCENTER);

	hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT22);
	TEXT_SetFont(hItem, &GUI_FontHZ40x40);
	TEXT_SetText(hItem, "λ��");
	TEXT_SetTextColor(hItem,GUI_WHITE);
	TEXT_SetTextAlign(hItem,GUI_TA_LEFT | GUI_TA_VCENTER);

	hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT23);
	TEXT_SetFont(hItem, &GUI_FontHZ45x45);
	TEXT_SetText(hItem, " ");
	TEXT_SetTextColor(hItem,GUI_RED);
	TEXT_SetTextAlign(hItem,GUI_TA_HCENTER | GUI_TA_VCENTER);
	
	hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT24);
	TEXT_SetFont(hItem, &GUI_FontHZ46x46);
	TEXT_SetText(hItem, ",");
	TEXT_SetTextColor(hItem,GUI_WHITE);
	TEXT_SetTextAlign(hItem,GUI_TA_HCENTER | GUI_TA_VCENTER);
	
	hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT25);
	TEXT_SetFont(hItem, &GUI_FontHZ40x40);
	TEXT_SetText(hItem, " ");
	TEXT_SetTextColor(hItem,GUI_WHITE);
	TEXT_SetTextAlign(hItem,GUI_TA_RIGHT | GUI_TA_VCENTER);
	
	hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT26);
	TEXT_SetFont(hItem, &GUI_FontHZ40x40);
	TEXT_SetText(hItem, " ");
	TEXT_SetTextColor(hItem,GUI_WHITE);
	TEXT_SetTextAlign(hItem,GUI_TA_LEFT | GUI_TA_VCENTER);

}
void CreateHomeFrameTextShow(WM_MESSAGE * pMsg)
{
	//�ܺ�
	TEXT_CreateEx(150,14,60,30,pMsg->hWin,WM_CF_SHOW,TEXT_CF_HCENTER|TEXT_CF_VCENTER,GUI_ID_TEXT0," ");
	//����
	TEXT_CreateEx(230,18,60,30,pMsg->hWin,WM_CF_SHOW,TEXT_CF_HCENTER|TEXT_CF_VCENTER,GUI_ID_TEXT1," ");
	//ʱ��
	TEXT_CreateEx(291,18,60,30,pMsg->hWin,WM_CF_SHOW,TEXT_CF_HCENTER|TEXT_CF_VCENTER,GUI_ID_TEXT2," ");
	//״̬//֧��
	TEXT_CreateEx(45,14,46,30, pMsg->hWin,WM_CF_SHOW,GUI_TA_RIGHT|TEXT_CF_VCENTER,GUI_ID_TEXT12," ");
	//״̬//
	TEXT_CreateEx(45+46,14,46,30, pMsg->hWin,WM_CF_SHOW,GUI_TA_LEFT|TEXT_CF_VCENTER,GUI_ID_TEXT13," ");
	//��ʾ1
	TEXT_CreateEx(10,218,170,30,pMsg->hWin,WM_CF_SHOW,TEXT_CF_HCENTER|TEXT_CF_VCENTER,GUI_ID_TEXT4," ");
	//��ʾ2
	TEXT_CreateEx(190,218,170,30,pMsg->hWin,WM_CF_SHOW,TEXT_CF_HCENTER|TEXT_CF_VCENTER,GUI_ID_TEXT5," ");
	//��ʾ3
	TEXT_CreateEx(10,253,170,30,pMsg->hWin,WM_CF_SHOW,TEXT_CF_HCENTER|TEXT_CF_VCENTER,GUI_ID_TEXT6," ");
	//��ʾ4
	TEXT_CreateEx(190,253,170,30,pMsg->hWin,WM_CF_SHOW,TEXT_CF_HCENTER|TEXT_CF_VCENTER,GUI_ID_TEXT7," ");
	//��ʾ5
	TEXT_CreateEx(10,288,170,30,pMsg->hWin,WM_CF_SHOW,TEXT_CF_HCENTER|TEXT_CF_VCENTER,GUI_ID_TEXT8," ");
	//��ʾ6
	TEXT_CreateEx(190,288,170,30,pMsg->hWin,WM_CF_SHOW,TEXT_CF_HCENTER|TEXT_CF_VCENTER,GUI_ID_TEXT9," ");
	//ú��λ��
	 TEXT_CreateEx(150,65,60,30,pMsg->hWin,WM_CF_SHOW,TEXT_CF_HCENTER|TEXT_CF_VCENTER,GUI_ID_TEXT10," ");
	//�����
	 TEXT_CreateEx(130,65+40,100,30,pMsg->hWin,WM_CF_SHOW,TEXT_CF_HCENTER|TEXT_CF_VCENTER,GUI_ID_TEXT11," ");
	 
	 //���������
	 //�����1
	TEXT_CreateEx(10,165-35,170,30,pMsg->hWin,WM_CF_SHOW,TEXT_CF_HCENTER|TEXT_CF_VCENTER,GUI_ID_TEXT14," ");
	//�����2
	TEXT_CreateEx(190,165-35,170,30,pMsg->hWin,WM_CF_SHOW,TEXT_CF_HCENTER|TEXT_CF_VCENTER,GUI_ID_TEXT15," ");
	//�����3
	TEXT_CreateEx(10,200-35,170,30,pMsg->hWin,WM_CF_SHOW,TEXT_CF_HCENTER|TEXT_CF_VCENTER,GUI_ID_TEXT16," ");
	//�����4
	TEXT_CreateEx(190,200-35,170,30,pMsg->hWin,WM_CF_SHOW,TEXT_CF_HCENTER|TEXT_CF_VCENTER,GUI_ID_TEXT17," ");
}
void USER_ReviseTextStyle(WM_MESSAGE * pMsg) 
{
	WM_HWIN hItem;
	hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT0);	
	TEXT_SetFont(hItem, &GUI_Font32B_ASCII);
	TEXT_SetText(hItem, " ");
	TEXT_SetTextColor(hItem,GUI_WHITE);
	TEXT_SetTextAlign(hItem,GUI_TA_HCENTER | GUI_TA_VCENTER);

	hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT1);	
	TEXT_SetFont(hItem, GUI_FONT_24B_ASCII);
	TEXT_SetText(hItem, "9.23");
	TEXT_SetTextColor(hItem,GUI_WHITE);
	TEXT_SetTextAlign(hItem,GUI_TA_HCENTER | GUI_TA_VCENTER);

	hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT2);	
	TEXT_SetFont(hItem, GUI_FONT_24B_ASCII);
	TEXT_SetText(hItem, "13:48");
	TEXT_SetTextColor(hItem,GUI_WHITE);
	TEXT_SetTextAlign(hItem,GUI_TA_HCENTER | GUI_TA_VCENTER);

	hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT12);	
	TEXT_SetFont(hItem, &GUI_FontHZ24x24);
	TEXT_SetText(hItem, " ");
	TEXT_SetTextColor(hItem,GUI_WHITE);
	TEXT_SetTextAlign(hItem,GUI_TA_RIGHT | GUI_TA_VCENTER);
	
	hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT13);	
	TEXT_SetFont(hItem, &GUI_FontHZ24x24);
	TEXT_SetText(hItem, " ");
	TEXT_SetTextColor(hItem,GUI_WHITE);
	TEXT_SetTextAlign(hItem,GUI_TA_LEFT | GUI_TA_VCENTER);
	
	hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT4);	
	TEXT_SetFont(hItem, &GUI_FontHZ24);
	TEXT_SetText(hItem, " ");
	TEXT_SetTextColor(hItem,GUI_WHITE);
	TEXT_SetTextAlign(hItem,GUI_TA_LEFT | GUI_TA_VCENTER);
	
	hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT5);	
	TEXT_SetFont(hItem, &GUI_FontHZ24);
	TEXT_SetText(hItem, " ");
	TEXT_SetTextColor(hItem,GUI_WHITE);
	TEXT_SetTextAlign(hItem,GUI_TA_LEFT | GUI_TA_VCENTER);
	
	hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT6);	
	TEXT_SetFont(hItem, &GUI_FontHZ24);
	TEXT_SetText(hItem, " ");
	TEXT_SetTextColor(hItem,GUI_WHITE);
	TEXT_SetTextAlign(hItem,GUI_TA_LEFT | GUI_TA_VCENTER);
	
	hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT7);	
	TEXT_SetFont(hItem, &GUI_FontHZ24);
	TEXT_SetText(hItem, " ");
	TEXT_SetTextColor(hItem,GUI_WHITE);
	TEXT_SetTextAlign(hItem,GUI_TA_LEFT | GUI_TA_VCENTER);
	
	hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT8);	
	TEXT_SetFont(hItem, &GUI_FontHZ24);
	TEXT_SetText(hItem, " ");
	TEXT_SetTextColor(hItem,GUI_WHITE);
	TEXT_SetTextAlign(hItem,GUI_TA_LEFT | GUI_TA_VCENTER);
	
	hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT9);	
	TEXT_SetFont(hItem, &GUI_FontHZ24);
	TEXT_SetText(hItem, " ");
	TEXT_SetTextColor(hItem,GUI_WHITE);
	TEXT_SetTextAlign(hItem,GUI_TA_LEFT | GUI_TA_VCENTER);
	
	hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT10);	
	TEXT_SetFont(hItem, &GUI_Font32B_ASCII);
	TEXT_SetText(hItem, "85");
	TEXT_SetTextColor(hItem,GUI_BLACK);
	TEXT_SetTextAlign(hItem,GUI_TA_HCENTER | GUI_TA_VCENTER);
	
	hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT11);	
	TEXT_SetFont(hItem, &GUI_FontHZ24);
	TEXT_SetText(hItem, "����11");
	TEXT_SetTextColor(hItem,GUI_WHITE);
//	TEXT_SetBkColor(hItem,GUI_WHITE);
	TEXT_SetTextAlign(hItem,GUI_TA_HCENTER | GUI_TA_VCENTER);
	
	
	//
	hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT14);	
	TEXT_SetFont(hItem, &GUI_FontHZ24);
	TEXT_SetText(hItem, " ");
	TEXT_SetTextColor(hItem,GUI_WHITE);
	TEXT_SetTextAlign(hItem,GUI_TA_LEFT | GUI_TA_VCENTER);
	
	hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT15);	
	TEXT_SetFont(hItem, &GUI_FontHZ24);
	TEXT_SetText(hItem, " ");
	TEXT_SetTextColor(hItem,GUI_WHITE);
	TEXT_SetTextAlign(hItem,GUI_TA_LEFT | GUI_TA_VCENTER);
	
	hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT16);	
	TEXT_SetFont(hItem, &GUI_FontHZ24);
	TEXT_SetText(hItem, " ");
	TEXT_SetTextColor(hItem,GUI_WHITE);
	TEXT_SetTextAlign(hItem,GUI_TA_LEFT | GUI_TA_VCENTER);
	
	hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT17);	
	TEXT_SetFont(hItem, &GUI_FontHZ24);
	TEXT_SetText(hItem, " ");
	TEXT_SetTextColor(hItem,GUI_WHITE);
	TEXT_SetTextAlign(hItem,GUI_TA_LEFT | GUI_TA_VCENTER);
}


/*
*********************************************************************************************************
*	�� �� ��: 
*	����˵��: ��ʼ���˵��ı�
*	��    ��: 
*	�� �� ֵ: 
*********************************************************************************************************
*/	
void InitMenuText(WM_MESSAGE * pMsg)
{
	
	WM_HWIN hItem;
	hItem = WM_GetDialogItem(pMsg->hWin, MENU_TEXT_ID0);
	TEXT_SetFont(hItem, &GUI_FontHZ24);
	TEXT_SetText(hItem,"�˵�����");
	TEXT_SetTextColor(hItem,GUI_WHITE);
	TEXT_SetTextAlign(hItem,GUI_TA_HCENTER | GUI_TA_VCENTER);
}


/*
*********************************************************************************************************
*	�� �� ��: 
*	����˵��: ��ȡҪ��ʾ�İ������Ƶ�һ��������
*	��    ��: 
*	�� �� ֵ: 
*********************************************************************************************************
*/
void GetTextNameInArray(uint32_t id,char * array)
{
	uint8_t count;
	MENU_PARA * temp_value;
	GetPointerFromLogicalPast(id,&temp_value,&count);

	if(temp_value->Show_Noshow == 1)
		*array = *temp_value->Para_Name;

		temp_value++;	

}
/*
*********************************************************************************************************
*	�� �� ��: 
*	����˵��: �޸��ı���ʾ
*	��    ��: 
*	�� �� ֵ: 
*********************************************************************************************************
*/
void USER_ReviseTextShow(WM_MESSAGE * pMsg,uint16_t ID_TEXT,const char* s)
{
	WM_HWIN hItem;
	hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT);
//	TEXT_SetFont(hItem, &GUI_FontHZ32x33);
	TEXT_SetText(hItem,s);
	TEXT_SetTextAlign(hItem,GUI_TA_HCENTER | GUI_TA_VCENTER);

}
/*
*********************************************************************************************************
*	�� �� ��: 
*	����˵��: �ı��ı�������ɫ
*	��    ��: 
*	�� �� ֵ: 
*********************************************************************************************************
*/
void USER_ReviseTextBkColor(WM_MESSAGE * pMsg,uint16_t ID_TEXT,uint32_t Color)
{
	WM_HWIN hItem;
	hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT);
	TEXT_SetBkColor(hItem,Color);	
}
/*
*********************************************************************************************************
*	�� �� ��: 
*	����˵��: ��Ҫ��ʾ����ɫ���ı�
*	��    ��: 
*	�� �� ֵ: 
*********************************************************************************************************
*/
void USER_ReviseTextColor(WM_MESSAGE * pMsg,uint32_t Color)
{

}
/*
*********************************************************************************************************
*	�� �� ��: 
*	����˵��: ���ݰ����޸��ı�
*	��    ��: 
*	�� �� ֵ: 
*********************************************************************************************************
*/
uint8_t USER_ButtonReviseTextShow(WM_MESSAGE * pMsg,uint16_t ID_BUTTON,uint16_t ID_TEXT)
{
	WM_HWIN hItem;
	char a[20]={0};
	uint16_t count;
	hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON);
	BUTTON_GetText(hItem,a,sizeof(a));
	count = GUI__strlen(a);
	if(count==5)
	{
		a[2] = a[3];
		a[3] = a[4];
		a[4] = '\0';
	}
	else if(count==8)
	{
		a[2] = a[3];
		a[3] = a[4];
		a[4] = a[6];
		a[5] = a[7];
		a[6] = '\0';
	}
	else if(count==9)
	{
		a[4] = a[5];
		a[5] = a[6];			
		a[6] = a[7];
		a[7] = a[8];
		a[8] = '\0';
	}
	else
	{
		return 0;
	}
	hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT);
//	TEXT_SetFont(hItem, &GUI_FontHZ32x33);
	TEXT_SetText(hItem,a);
	TEXT_SetTextAlign(hItem,GUI_TA_HCENTER | GUI_TA_VCENTER);
	return 1;
}


