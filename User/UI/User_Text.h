/*
*********************************************************************************************************
*	                                  
*	ģ������ : button�û���
*	�ļ����� : UserButton.h
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


#ifndef __User_Text_H
#define __User_Text_H

#include "User_GUI.h"



/*
************************************************************************
*						���ⲿ�ļ�����
************************************************************************
*/

void USER_ReviseTextShow(WM_MESSAGE * pMsg,uint16_t ID_TEXT,const char* s);
void USER_ReviseTextBkColor(WM_MESSAGE * pMsg,uint16_t ID_TEXT,uint32_t Color);
uint8_t USER_ButtonReviseTextShow(WM_MESSAGE * pMsg,uint16_t ID_BUTTON,uint16_t ID_TEXT);
void GetTextNameInArray(uint32_t id,char * array);
void USER_ReviseTextStyle(WM_MESSAGE * pMsg) ;
void CreateHomeFrameTextShow(WM_MESSAGE * pMsg);
void CreateSuddenStopTextShow(WM_MESSAGE * pMsg);
void CreateGroupTextShow(WM_MESSAGE * pMsg);
void CreateMasterContorTextShow(WM_MESSAGE * pMsg);
void CreateSlveContorTextShow(WM_MESSAGE * pMsg);
void InitMenuText(WM_MESSAGE * pMsg);
void CreateRackContorTextShow(WM_MESSAGE * pMsg);
#endif
