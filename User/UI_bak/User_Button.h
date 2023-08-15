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


#ifndef User_Button_H
#define User_Button_H

//#include "User_GUI.h"
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "WM.h"


typedef struct
{
	char * name;
	uint16_t  id;
	uint16_t  type;
	uint16_t  MAX;
}NameAndId;

typedef struct
{
	char  name[12];
	char * nameshow;
	uint16_t  id;
	uint16_t  MAX;
	uint16_t  Nuit;
	uint16_t  Modift;
	uint16_t  acc;
}SensorShow;

/*
************************************************************************
*						供外部文件调用
************************************************************************
*/
uint8_t USER_ReviseButtonShow(WM_MESSAGE * pMsg,uint16_t ID_BUTTON,const char* s);
void USER_DownButtonShow(WM_MESSAGE * pMsg,uint16_t ID_BUTTON);
void USER_UpButtonShow(WM_MESSAGE * pMsg,uint16_t ID_BUTTON);
void USER_ReviseAllButtonShow(WM_MESSAGE * pMsg, char* s[]);
void USER_ClearAllButtonShow(WM_MESSAGE * pMsg);
void USER_ReviseAgileButtonShow(WM_MESSAGE * pMsg,char * Options[]);
void USER_ReviseOnePageButtonShow(WM_MESSAGE * pMsg,char * Options[]);
uint8_t GetMAXbutton(uint64_t id);
void GetButtonNameInArray(uint32_t id,char * array[]);
void USER_ReviseButtonStyle(WM_MESSAGE * pMsg) ;
void InitMenuButton(WM_MESSAGE * pMsg);
void USER_ReviseSingleActionButtonShow(WM_MESSAGE * pMsg,NameAndId Options[],uint8_t val);
#endif
