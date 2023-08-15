/*
*********************************************************************************************************
*	                                  
*	ģ������ : button�û���
*	�ļ����� : UserButton.c
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
#include "User_GUI.h"
/*
*********************************************************************************************************
*	                                     �궨��
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*	�� �� ��: 
*	����˵��: ��ȡ�������ֵ
*	��    ��: 
*	�� �� ֵ: 
*********************************************************************************************************
*/
uint8_t GetMAXbutton(uint64_t id)
{
	uint8_t count = 0;
	if(id == 0)
	{
		count = 6;
	}
	else
		count = getJsonParaStructSize(id);

	return count;	
}
/*
*********************************************************************************************************
*	�� �� ��: 
*	����˵��: ��ȡҪ��ʾ�İ������Ƶ�һ��������
*	��    ��: 
*	�� �� ֵ: 
*********************************************************************************************************
*/
void GetButtonNameInArray(uint32_t id,char * array[])
{
//	uint8_t i ,count,j;
//	MENU_PARA * temp_value;
//	GetPointerFromLogicalPast(id,&temp_value,&count);
//	for(i= 0;i<count;i++)
//	{
//		if(temp_value->Attrbute.ATTRBUTE_PARA.para.Show_Noshow == 1)
//		{
//			array[j] = temp_value->Para_Name;
//			if(j++ > 6)
//				return ;
//		}
//		
//		
//		
//		
//		
//		
//		
//		if(i < count)
//		{
//			if(temp_value->Attrbute.ATTRBUTE_PARA.para.Show_Noshow == 1)
//				array[i] = temp_value->Para_Name;
//		}
//		else
//		{
//			array[i] = " ";
//		}
//		temp_value++;	
//	}
}
/*
*********************************************************************************************************
*	�� �� ��: 
*	����˵��: �޸İ������
*	��    ��: 
*	�� �� ֵ: 
*********************************************************************************************************
*/
void USER_ReviseButtonStyle(WM_MESSAGE * pMsg) 
{
//	char *  Button[] = {"��ǰ��","�Ƽ�","�커��","��ǰ��","����","�ջ���"};
	WM_HWIN hItem;
	BUTTON_SKINFLEX_PROPS Prop;
	GUI_EnableAlpha(1);
	BUTTON_GetSkinFlexProps(&Prop,BUTTON_SKINFLEX_PI_FOCUSSED);
	Prop.aColorUpper[0] = GUI_TRANSPARENT;
	Prop.aColorLower[0] = GUI_TRANSPARENT;
	Prop.aColorUpper[1] = GUI_TRANSPARENT;
	Prop.aColorLower[1] = GUI_TRANSPARENT;
	Prop.aColorFrame[0] = GUI_TRANSPARENT;
	Prop.aColorFrame[1] = GUI_TRANSPARENT;
	Prop.aColorFrame[2] = GUI_TRANSPARENT;
	Prop.Radius = 5;
	BUTTON_SetSkinFlexProps(&Prop,BUTTON_SKINFLEX_PI_FOCUSSED);
	
	BUTTON_GetSkinFlexProps(&Prop,BUTTON_SKINFLEX_PI_PRESSED);
	Prop.aColorUpper[0] = GUI_TRANSPARENT;
	Prop.aColorLower[0] = GUI_TRANSPARENT;
	Prop.aColorUpper[1] = GUI_TRANSPARENT;
	Prop.aColorLower[1] = GUI_TRANSPARENT; 
	Prop.aColorFrame[0] = GUI_TRANSPARENT;
	Prop.aColorFrame[1] = GUI_TRANSPARENT;
	Prop.aColorFrame[2] = GUI_TRANSPARENT;
	Prop.Radius = 5;
	BUTTON_SetSkinFlexProps(&Prop,BUTTON_SKINFLEX_PI_PRESSED);
	
	BUTTON_GetSkinFlexProps(&Prop,BUTTON_SKINFLEX_PI_ENABLED);
	Prop.aColorUpper[0] = GUI_TRANSPARENT;
	Prop.aColorLower[0] = GUI_TRANSPARENT;
	Prop.aColorUpper[1] = GUI_TRANSPARENT;
	Prop.aColorLower[1] = GUI_TRANSPARENT; 
	Prop.aColorFrame[0] = GUI_TRANSPARENT;
	Prop.aColorFrame[1] = GUI_TRANSPARENT;
	Prop.aColorFrame[2] = GUI_TRANSPARENT;
	Prop.Radius = 5;
	BUTTON_SetSkinFlexProps(&Prop,BUTTON_SKINFLEX_PI_ENABLED);
	
	BUTTON_GetSkinFlexProps(&Prop,BUTTON_SKINFLEX_PI_DISABLED);
	Prop.aColorUpper[0] = GUI_TRANSPARENT;
	Prop.aColorLower[0] = GUI_TRANSPARENT;
	Prop.aColorUpper[1] = GUI_TRANSPARENT;
	Prop.aColorLower[1] = GUI_TRANSPARENT; 
	Prop.aColorFrame[0] = GUI_TRANSPARENT;
	Prop.aColorFrame[1] = GUI_TRANSPARENT;
	Prop.aColorFrame[2] = GUI_TRANSPARENT;
	Prop.Radius = 5;
	BUTTON_SetSkinFlexProps(&Prop,BUTTON_SKINFLEX_PI_DISABLED);
	
	hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_BUTTON0);	
	BUTTON_SetFont(hItem, &GUI_FontHZ24);
	BUTTON_SetBitmap(hItem,BUTTON_BI_PRESSED,&bmleft_btn1_hover);
	BUTTON_SetTextColor(hItem,BUTTON_CI_PRESSED,GUI_WHITE);
	BUTTON_SetTextColor(hItem,BUTTON_CI_UNPRESSED,GUI_WHITE);
	BUTTON_SetTextAlign(hItem,GUI_TA_HCENTER | GUI_TA_VCENTER);
	
	hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_BUTTON1);	
	BUTTON_SetFont(hItem, &GUI_FontHZ24);
	BUTTON_SetBitmap(hItem,BUTTON_BI_PRESSED,&bmright_btn1_hover);
	BUTTON_SetTextColor(hItem,BUTTON_CI_PRESSED,GUI_WHITE);
	BUTTON_SetTextColor(hItem,BUTTON_CI_UNPRESSED,GUI_WHITE);
	BUTTON_SetTextAlign(hItem,GUI_TA_HCENTER | GUI_TA_VCENTER);
	
	hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_BUTTON2);	
	BUTTON_SetFont(hItem, &GUI_FontHZ24);
	BUTTON_SetBitmap(hItem,BUTTON_BI_PRESSED,&bmleft_btn2_hover);
	BUTTON_SetTextColor(hItem,BUTTON_CI_PRESSED,GUI_WHITE);
	BUTTON_SetTextColor(hItem,BUTTON_CI_UNPRESSED,GUI_WHITE);
	BUTTON_SetTextAlign(hItem,GUI_TA_HCENTER | GUI_TA_VCENTER);
	
	hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_BUTTON3);	
	BUTTON_SetFont(hItem, &GUI_FontHZ24);
	BUTTON_SetBitmap(hItem,BUTTON_BI_PRESSED,&bmleft_btn2_hover);
	BUTTON_SetTextColor(hItem,BUTTON_CI_PRESSED,GUI_WHITE);
	BUTTON_SetTextColor(hItem,BUTTON_CI_UNPRESSED,GUI_WHITE);
	BUTTON_SetTextAlign(hItem,GUI_TA_HCENTER | GUI_TA_VCENTER);
	
	hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_BUTTON4);	
	BUTTON_SetFont(hItem, &GUI_FontHZ24);
	BUTTON_SetBitmap(hItem,BUTTON_BI_PRESSED,&bmleft_btn3_hover);
	BUTTON_SetTextColor(hItem,BUTTON_CI_PRESSED,GUI_WHITE);
	BUTTON_SetTextColor(hItem,BUTTON_CI_UNPRESSED,GUI_WHITE);
	BUTTON_SetTextAlign(hItem,GUI_TA_HCENTER | GUI_TA_VCENTER);
	
	hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_BUTTON5);	
	BUTTON_SetFont(hItem, &GUI_FontHZ24);
	BUTTON_SetBitmap(hItem,BUTTON_BI_PRESSED,&bmright_btn3_hover);
	BUTTON_SetTextColor(hItem,BUTTON_CI_PRESSED,GUI_WHITE);
	BUTTON_SetTextColor(hItem,BUTTON_CI_UNPRESSED,GUI_WHITE);
	BUTTON_SetTextAlign(hItem,GUI_TA_HCENTER | GUI_TA_VCENTER);
//	USER_ReviseAllButtonShow(pMsg,Button);
//	SetSinglePageValue(0);
}

/*
*********************************************************************************************************
*	�� �� ��: 
*	����˵��: ��ʼ������������ʾ
*	��    ��: 
*	�� �� ֵ: 
*********************************************************************************************************
*/
void InitMenuButton(WM_MESSAGE * pMsg)
{
	
	WM_HWIN hItem;
		BUTTON_SKINFLEX_PROPS Prop;
	GUI_EnableAlpha(1);
	BUTTON_GetSkinFlexProps(&Prop,BUTTON_SKINFLEX_PI_FOCUSSED);
	Prop.aColorUpper[0] = GUI_TRANSPARENT;
	Prop.aColorLower[0] = GUI_TRANSPARENT;
	Prop.aColorUpper[1] = GUI_TRANSPARENT;
	Prop.aColorLower[1] = GUI_TRANSPARENT;
	Prop.aColorFrame[0] = GUI_TRANSPARENT;
	Prop.aColorFrame[1] = GUI_TRANSPARENT;
	Prop.aColorFrame[2] = GUI_TRANSPARENT;
	Prop.Radius = 5;
	BUTTON_SetSkinFlexProps(&Prop,BUTTON_SKINFLEX_PI_FOCUSSED);
	
	BUTTON_GetSkinFlexProps(&Prop,BUTTON_SKINFLEX_PI_PRESSED);
	Prop.aColorUpper[0] = GUI_TRANSPARENT;
	Prop.aColorLower[0] = GUI_TRANSPARENT;
	Prop.aColorUpper[1] = GUI_TRANSPARENT;
	Prop.aColorLower[1] = GUI_TRANSPARENT; 
	Prop.aColorFrame[0] = GUI_TRANSPARENT;
	Prop.aColorFrame[1] = GUI_TRANSPARENT;
	Prop.aColorFrame[2] = GUI_TRANSPARENT;
	Prop.Radius = 5;
	BUTTON_SetSkinFlexProps(&Prop,BUTTON_SKINFLEX_PI_PRESSED);
	
	BUTTON_GetSkinFlexProps(&Prop,BUTTON_SKINFLEX_PI_ENABLED);
	Prop.aColorUpper[0] = GUI_TRANSPARENT;
	Prop.aColorLower[0] = GUI_TRANSPARENT;
	Prop.aColorUpper[1] = GUI_TRANSPARENT;
	Prop.aColorLower[1] = GUI_TRANSPARENT; 
	Prop.aColorFrame[0] = GUI_TRANSPARENT;
	Prop.aColorFrame[1] = GUI_TRANSPARENT;
	Prop.aColorFrame[2] = GUI_TRANSPARENT;
	Prop.Radius = 5;
	BUTTON_SetSkinFlexProps(&Prop,BUTTON_SKINFLEX_PI_ENABLED);
	
	BUTTON_GetSkinFlexProps(&Prop,BUTTON_SKINFLEX_PI_DISABLED);
	Prop.aColorUpper[0] = GUI_TRANSPARENT;
	Prop.aColorLower[0] = GUI_TRANSPARENT;
	Prop.aColorUpper[1] = GUI_TRANSPARENT;
	Prop.aColorLower[1] = GUI_TRANSPARENT; 
	Prop.aColorFrame[0] = GUI_TRANSPARENT;
	Prop.aColorFrame[1] = GUI_TRANSPARENT;
	Prop.aColorFrame[2] = GUI_TRANSPARENT;
	Prop.Radius = 5;
	BUTTON_SetSkinFlexProps(&Prop,BUTTON_SKINFLEX_PI_DISABLED);
	hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_BUTTON0);
	BUTTON_SetFont(hItem, &GUI_FontHZ24);
	BUTTON_SetTextColor(hItem,BUTTON_CI_PRESSED,GUI_WHITE);
	BUTTON_SetTextColor(hItem,BUTTON_CI_UNPRESSED,GUI_WHITE);
	hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_BUTTON1);
	BUTTON_SetFont(hItem, &GUI_FontHZ24);
	BUTTON_SetTextColor(hItem,BUTTON_CI_PRESSED,GUI_WHITE);
	BUTTON_SetTextColor(hItem,BUTTON_CI_UNPRESSED,GUI_WHITE);
	hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_BUTTON2);
	BUTTON_SetFont(hItem, &GUI_FontHZ24);
	BUTTON_SetTextColor(hItem,BUTTON_CI_PRESSED,GUI_WHITE);
	BUTTON_SetTextColor(hItem,BUTTON_CI_UNPRESSED,GUI_WHITE);
	hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_BUTTON3);
	BUTTON_SetFont(hItem, &GUI_FontHZ24);
	BUTTON_SetTextColor(hItem,BUTTON_CI_PRESSED,GUI_WHITE);
	BUTTON_SetTextColor(hItem,BUTTON_CI_UNPRESSED,GUI_WHITE);
	hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_BUTTON4);
	BUTTON_SetFont(hItem, &GUI_FontHZ24);
	BUTTON_SetTextColor(hItem,BUTTON_CI_PRESSED,GUI_WHITE);
	BUTTON_SetTextColor(hItem,BUTTON_CI_UNPRESSED,GUI_WHITE);
	hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_BUTTON5);
	BUTTON_SetFont(hItem, &GUI_FontHZ24);
	BUTTON_SetTextColor(hItem,BUTTON_CI_PRESSED,GUI_WHITE);
	BUTTON_SetTextColor(hItem,BUTTON_CI_UNPRESSED,GUI_WHITE);	
}

/*
*********************************************************************************************************
*	�� �� ��: 
*	����˵��: �޸�һ��������ʾ����
*	��    ��: 
*	�� �� ֵ: 
*********************************************************************************************************
*/
uint8_t USER_ReviseButtonShow(WM_MESSAGE * pMsg,uint16_t ID_BUTTON,const char* s)
{
	uint16_t count;
	WM_HWIN hItem;
	char a[10] = {0};
	if(s != 0)
	{	
		count = GUI__strlen(s);
		memcpy(a,s,count);
		if(count<=6)
		{
			if(count == 4)
			{
				a[4] = a[3];
				a[3] = a[2];
				a[2] = '\n';
			}
			
			else if(count == 5)
			{
				a[8] = a[7];
				a[7] = a[6];			
				a[6] = a[5];
				a[5] = a[4];
				a[4] = '\n';
			}
			
			
			else if(count == 6)
			{
				a[7] = a[5];
				a[6] = a[4];
				a[5] = '\n';
				a[4] = a[3];
				a[3] = a[2];
				a[2] = '\n';
			}
			else
			{
				;		
			}
		}
		else if(count==7)
		{
			a[8] = a[7];
			a[7] = a[6];			
			a[6] = a[5];
			a[5] = a[4];
			a[4] = '\n';
		}
		else if(count==8)
		{
			a[8] = a[7];
			a[7] = a[6];			
			a[6] = a[5];
			a[5] = a[4];
			a[4] = '\n';
		}
		else if(count==9)
		{
			a[8] = a[7];
			a[7] = a[6];			
			a[6] = a[5];
			a[5] = a[4];
			a[4] = '\n';
		}
		else 
		{
			;	
		}
	}
	hItem = WM_GetDialogItem(pMsg->hWin,ID_BUTTON);
//	BUTTON_SetFont(hItem, &GUI_FontHZ32x33);
	BUTTON_SetBkColor(hItem,BUTTON_CI_UNPRESSED,0xff901e);
	BUTTON_SetText(hItem,a);
	return 1;
}
/*
*********************************************************************************************************
*	�� �� ��: 
*	����˵��: ���ȫ��������ʾ����
*	��    ��: 
*	�� �� ֵ: 
*********************************************************************************************************
*/
void USER_ClearAllButtonShow(WM_MESSAGE * pMsg)
{
	WM_HWIN hItem;
	char a[10] = {0};
	uint16_t count;
	for(count=0;count<6;count++)
	{
		hItem = WM_GetDialogItem(pMsg->hWin,GUI_ID_BUTTON0+count);
//		BUTTON_SetFont(hItem, &GUI_FontHZ32x33);
		BUTTON_SetBkColor(hItem,BUTTON_CI_UNPRESSED,0xff901e);
		BUTTON_SetText(hItem,a);
	}
}
/*
*********************************************************************************************************
*	�� �� ��: 
*	����˵��: �޸�ȫ��������ʾ����
*	��    ��: 
*	�� �� ֵ: 
*********************************************************************************************************
*/
void USER_ReviseAllButtonShow(WM_MESSAGE * pMsg, char* s[])
{
	uint16_t count,i;
	//��ȡ��ǰҳ��
	i = (GetPageValue()*6);	
	for(count=0;count<6;count++)
	{
		USER_ReviseButtonShow(pMsg,GUI_ID_BUTTON0+count,s[i++]);
	}
}
/*
*********************************************************************************************************
*	�� �� ��: 
*	����˵��: �޸Ķ���������ʾ����
*	��    ��: 
*	�� �� ֵ: 
*********************************************************************************************************
*/
void USER_ReviseSingleActionButtonShow(WM_MESSAGE * pMsg,NameAndId Options[],uint8_t val)
{
	uint16_t count,i;
	//��ȡ��ǰҳ��
	if(val == 0)
		i = (GetSinglePageValue()*6);	
	else
		i = (GetGroupPageValue()*6);	
	for(count=0;count<6;count++)
	{
		USER_ReviseButtonShow(pMsg,GUI_ID_BUTTON0+count,Options[i++].name);
	}
	
}






/*
*********************************************************************************************************
*	�� �� ��: 
*	����˵��: �޸Ķ���������ʾ����
*	��    ��: 
*	�� �� ֵ: 
*********************************************************************************************************
*/
void USER_ReviseAgileButtonShow(WM_MESSAGE * pMsg,char * Options[])
{
	uint16_t count,i;
	//��ȡ��ǰҳ��
	i = (GetPageValue()*6);	
	for(count=0;count<6;count++)
	{
		USER_ReviseButtonShow(pMsg,GUI_ID_BUTTON0+count,Options[i++]);
	}
	
}
/*
*********************************************************************************************************
*	�� �� ��: 
*	����˵��: �޸Ķ���������ʾ����
*	��    ��: 
*	�� �� ֵ: 
*********************************************************************************************************
*/
void USER_ReviseOnePageButtonShow(WM_MESSAGE * pMsg,char * Options[])
{
	uint16_t count;
	//��ȡ��ǰҳ��
	for(count=0;count<6;count++)
	{
//		USER_ReviseButtonShow(pMsg,GUI_ID_BUTTON0+count,Options[i++]);
	}
	
}
/*
*********************************************************************************************************
*	�� �� ��: 
*	����˵��: ��������
*	��    ��: 
*	�� �� ֵ: 
*********************************************************************************************************
*/
void USER_DownButtonShow(WM_MESSAGE * pMsg,uint16_t key)
{
	switch (key) 
	{
		case USER_KEY_LEFT1:
			BUTTON_SetTextOffset(WM_GetDialogItem(pMsg->hWin, MENU_BUTTON_LEFT_ID1),0,5);
			BUTTON_SetPressed(WM_GetDialogItem(pMsg->hWin, MENU_BUTTON_LEFT_ID1),1);
			break;
		case USER_KEY_LEFT2:
			BUTTON_SetTextOffset(WM_GetDialogItem(pMsg->hWin, MENU_BUTTON_LEFT_ID2),0,5);
			BUTTON_SetPressed(WM_GetDialogItem(pMsg->hWin, MENU_BUTTON_LEFT_ID2),1);
			break;
		case USER_KEY_LEFT3:
			BUTTON_SetTextOffset(WM_GetDialogItem(pMsg->hWin, MENU_BUTTON_LEFT_ID3),0,5);
			BUTTON_SetPressed(WM_GetDialogItem(pMsg->hWin, MENU_BUTTON_LEFT_ID3),1);
			break;
		case USER_KEY_RIGHT1:
			BUTTON_SetTextOffset(WM_GetDialogItem(pMsg->hWin, MENU_BUTTON_RIGHT_ID1),0,5);
			BUTTON_SetPressed(WM_GetDialogItem(pMsg->hWin, MENU_BUTTON_RIGHT_ID1),1);
			break;
		case USER_KEY_RIGHT2:
			BUTTON_SetTextOffset(WM_GetDialogItem(pMsg->hWin, MENU_BUTTON_RIGHT_ID2),0,5);
			BUTTON_SetPressed(WM_GetDialogItem(pMsg->hWin, MENU_BUTTON_RIGHT_ID2),1);
			break;
		case USER_KEY_RIGHT3:
			BUTTON_SetTextOffset(WM_GetDialogItem(pMsg->hWin, MENU_BUTTON_RIGHT_ID3),0,5);
			BUTTON_SetPressed(WM_GetDialogItem(pMsg->hWin, MENU_BUTTON_RIGHT_ID3),1);
			break;
		default:
			break;
	}
}
/*
*********************************************************************************************************
*	�� �� ��: 
*	����˵��: ����̧��
*	��    ��: 
*	�� �� ֵ: 
*********************************************************************************************************
*/
void USER_UpButtonShow(WM_MESSAGE * pMsg,uint16_t key)
{
	switch (key) 
	{
		case USER_KEY_LEFT1:
			BUTTON_SetTextOffset(WM_GetDialogItem(pMsg->hWin, MENU_BUTTON_LEFT_ID1),0,0);
			BUTTON_SetPressed(WM_GetDialogItem(pMsg->hWin, MENU_BUTTON_LEFT_ID1),0);
			break;
		case USER_KEY_LEFT2:
			BUTTON_SetTextOffset(WM_GetDialogItem(pMsg->hWin, MENU_BUTTON_LEFT_ID2),0,0);
			BUTTON_SetPressed(WM_GetDialogItem(pMsg->hWin, MENU_BUTTON_LEFT_ID2),0);
			break;
		case USER_KEY_LEFT3:
			BUTTON_SetTextOffset(WM_GetDialogItem(pMsg->hWin, MENU_BUTTON_LEFT_ID3),0,0);
			BUTTON_SetPressed(WM_GetDialogItem(pMsg->hWin, MENU_BUTTON_LEFT_ID3),0);
			break;
		case USER_KEY_RIGHT1:
			BUTTON_SetTextOffset(WM_GetDialogItem(pMsg->hWin, MENU_BUTTON_RIGHT_ID1),0,0);
			BUTTON_SetPressed(WM_GetDialogItem(pMsg->hWin, MENU_BUTTON_RIGHT_ID1),0);
			break;
		case USER_KEY_RIGHT2:
			BUTTON_SetTextOffset(WM_GetDialogItem(pMsg->hWin, MENU_BUTTON_RIGHT_ID2),0,0);
			BUTTON_SetPressed(WM_GetDialogItem(pMsg->hWin, MENU_BUTTON_RIGHT_ID2),0);
			break;
		case USER_KEY_RIGHT3:
			BUTTON_SetTextOffset(WM_GetDialogItem(pMsg->hWin, MENU_BUTTON_RIGHT_ID3),0,0);
			BUTTON_SetPressed(WM_GetDialogItem(pMsg->hWin, MENU_BUTTON_RIGHT_ID3),0);
			break;
		default:
			break;
	}
}
/*
*********************************************************************************************************
*	�� �� ��: 
*	����˵��: ��ʾ��������
*	��    ��: 
*	�� �� ֵ: 
*********************************************************************************************************
*/












































