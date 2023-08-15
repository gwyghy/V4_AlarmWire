/*
*********************************************************************************************************
*	                                  
*	模块名称 : button用户库
*	文件名称 : UserButton.h
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


#ifndef __User_Text_H
#define __User_Text_H

#include "User_GUI.h"



/*
************************************************************************
*						供外部文件调用
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
