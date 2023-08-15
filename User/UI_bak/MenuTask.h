/*
*********************************************************************************************************
*	                                  
*	模块名称 : GUI界面主函数
*	文件名称 : MenuTask.c
*	版    本 : V1.0
*	说    明 : GUI界面主函数
*		版本号   日期         作者            说明
*		v1.0    2015-08-05  Eric2013  	      首版
*
*	Copyright (C), 2015-2020, 安富莱电子 www.armfly.com
*
*********************************************************************************************************
*/

#ifndef __MENUTASK_H
#define __MENUTASK_H

#include "User_GUI.h"

/*
*********************************************************************************************************
*	                                     宏定义
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
*						供外部文件调用
************************************************************************
*/
extern void CreateMenuTask(void);
extern void TOUCH_Calibration(void);
#endif

/*****************************(END OF FILE) *********************************/
