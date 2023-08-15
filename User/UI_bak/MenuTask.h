/*
*********************************************************************************************************
*	                                  
*	ģ������ : GUI����������
*	�ļ����� : MenuTask.c
*	��    �� : V1.0
*	˵    �� : GUI����������
*		�汾��   ����         ����            ˵��
*		v1.0    2015-08-05  Eric2013  	      �װ�
*
*	Copyright (C), 2015-2020, ���������� www.armfly.com
*
*********************************************************************************************************
*/

#ifndef __MENUTASK_H
#define __MENUTASK_H

#include "User_GUI.h"

/*
*********************************************************************************************************
*	                                     �궨��
*********************************************************************************************************
*/
#define  MENU_BUTTON_LEFT_ID1         GUI_ID_BUTTON0 
#define  MENU_BUTTON_LEFT_ID2         GUI_ID_BUTTON2 
#define  MENU_BUTTON_LEFT_ID3         GUI_ID_BUTTON4 
#define  MENU_BUTTON_RIGHT_ID1        GUI_ID_BUTTON1 
#define  MENU_BUTTON_RIGHT_ID2        GUI_ID_BUTTON3 
#define  MENU_BUTTON_RIGHT_ID3        GUI_ID_BUTTON5 
#define  MENU_TEXT_ID0       		  GUI_ID_TEXT0 
#define  MENU_LISTVIEW_ID0            GUI_ID_LISTVIEW0
#define  MENU_WINDOW_ID0 			  GUI_ID_WINDOW0


/*
************************************************************************
*						  FatFs
************************************************************************
*/



/*
************************************************************************
*						���ⲿ�ļ�����
************************************************************************
*/
extern void CreateMenuTask(void);
extern void TOUCH_Calibration(void);
#endif

/*****************************(END OF FILE) *********************************/
