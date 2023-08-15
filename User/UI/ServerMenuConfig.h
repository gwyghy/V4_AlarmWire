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
#include "WM.h"
#include "includes.h"
#ifndef __SERVERMENUCONFIG_H
#define __SERVERMENUCONFIG_H


typedef struct{	
	uint8_t   count; 	
 	uint8_t   buf[30][26];  
}ButtonPage;
typedef struct{	
	uint8_t   count; 	
 	uint8_t   buf[30][26];  
}TextPage;
typedef struct{	
	uint8_t   count; 	
 	uint8_t   buf[30][26];  
}ListviewPage;


typedef struct{	
	uint8_t			refresh;
	ButtonPage  	ButtonDisplay; 	
 	TextPage   		TextDisplay;  
	ListviewPage	ListviewDisplaye;
}Displayrefresh;


void ServerMenu_ShowListviewForPage(WM_MESSAGE * pMsg);
void ServerMenu_ShowTextForPage(WM_MESSAGE * pMsg);
void ServerMenu_ShowButtonForPage(WM_MESSAGE * pMsg);
void ServerMenu_SetCurrentPage(uint16_t key,WM_MESSAGE* pMsg);
void ServerCreator_Menuwindow(WM_MESSAGE * pMsg);
#endif
