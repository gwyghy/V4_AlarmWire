/*
*********************************************************************************************************
*	                                  
*	模块名称 : GUI界面主函数
*	文件名称 : HomeTask.c
*	版    本 : V1.0
*	说    明 : GUI界面主函数
*		版本号   日期         作者            说明
*		v1.0    2015-08-05  Eric2013  	      首版
*
*	Copyright (C), 2015-2020, 安富莱电子 www.armfly.com
*
*********************************************************************************************************
*/

#ifndef __HOMETASK_H
#define __HOMETASK_H

#include "User_GUI.h"

/*
*********************************************************************************************************
*	                                     宏定义
*********************************************************************************************************
*/


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
void AppTaskGUI(void);
void TOUCH_Calibration(void);
void Create_Button_Frame(void);
WM_HWIN CreateMoveFrame_Right(void);
WM_HWIN CreatePassWordInPut(void);
WM_HWIN CreateBackGround(void) ;
WM_HWIN CreateMoveFrame_Left(void) ;
WM_HWIN CreateSuddenStop(WM_HWIN  pMsg);
WM_HWIN CreateHomeFrame(void) ;
WM_HWIN CreateSignal(void) ;
WM_HWIN CreateWarning(void);
WM_HWIN CreateWarningtwo(WM_HWIN  pMsg);

#endif

/*****************************(END OF FILE) *********************************/
