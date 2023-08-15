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


#ifndef __StateConfig_H
#define __StateConfig_H

#include "User_GUI.h"



/*
************************************************************************
*						供外部文件调用
************************************************************************
*/

void Creator_Statewindow(WM_MESSAGE * pMsg,uint8_t count);
void State_ShowButtonForPage(WM_MESSAGE * pMsg,uint32_t id);
void State_ShowListviewForPage(WM_MESSAGE * pMsg,uint32_t id);
void InitStateText(WM_MESSAGE * pMsg);
#endif
