/*
*********************************************************************************************************
*	                                  
*	ģ������ : 
*	�ļ����� : MenuConfig.h
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


#ifndef __StateConfig_H
#define __StateConfig_H

#include "User_GUI.h"



/*
************************************************************************
*						���ⲿ�ļ�����
************************************************************************
*/

void Creator_Statewindow(WM_MESSAGE * pMsg,uint8_t count);
void State_ShowButtonForPage(WM_MESSAGE * pMsg,uint32_t id);
void State_ShowListviewForPage(WM_MESSAGE * pMsg,uint32_t id);
void InitStateText(WM_MESSAGE * pMsg);
#endif
