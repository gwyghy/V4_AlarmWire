/*
*********************************************************************************************************
*	                                  
*	模块名称 : 
*	文件名称 : MenuConfig.h
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
