/*
*********************************************************************************************************
*	                                  
*	ģ������ : GUI����������
*	�ļ����� : HomeTask.c
*	��    �� : V1.0
*	˵    �� : GUI����������
*		�汾��   ����         ����            ˵��
*		v1.0    2015-08-05  Eric2013  	      �װ�
*
*	Copyright (C), 2015-2020, ���������� www.armfly.com
*
*********************************************************************************************************
*/

#ifndef __HOMETASK_H
#define __HOMETASK_H

#include "User_GUI.h"

/*
*********************************************************************************************************
*	                                     �궨��
*********************************************************************************************************
*/


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