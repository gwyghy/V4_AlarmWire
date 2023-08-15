/*
*********************************************************************************************************
*	                                  
*	模块名称 : GUI用户库
*	文件名称 : User_GUI.h
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


#ifndef __User_GUI_H
#define __User_GUI_H
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "includes.h"
#include "GUI.h"
#include "DIALOG.h"
#include "WM.h"
#include "BUTTON.h"
#include "CHECKBOX.h"
#include "DROPDOWN.h"
#include "EDIT.h"
#include "FRAMEWIN.h"
#include "LISTBOX.h"
#include "MULTIEDIT.h"
#include "RADIO.h"
#include "SLIDER.h"
#include "TEXT.h"
#include "PROGBAR.h"
#include "SCROLLBAR.h"
#include "LISTVIEW.h"
#include "GRAPH.h"
#include "MENU.h"
#include "MULTIPAGE.h"
#include "ICONVIEW.h"
#include "TREEVIEW.h"
#include "MenuConfig.h"
#include "MenuTask.h"
#include "HomeConfig.h"
#include "HomeTask.h"
#include "GUIKey.h"
#include "User_Button.h"
#include "User_Text.h"
#include "User_Listview.h"
#include "ff.h"
#include "GroupTask.h"
#include "MasterContorTask.h"
#include "SlveContorTask.h"
#include "GUI_Refresh.h"
#include "GUI_api.h"
#include "StateConfig.h"
#include "StateTask.h"
#include "Json_Analysis.h"
#include "DeveloperTask.h"

#include "ServerMenuConfig.h"
#include "ServerMenuTask.h"
/*
*********************************************************************************************************
*	                                     宏定义
*********************************************************************************************************
*/
#define LEVEL_MAX  8
#define MSG_CHANGE_TEXT (WM_USER + 0)
#define MSG_CHANGE_KEY (WM_USER + 1)
#define MSG_CHANGE_BUTTON (WM_USER + 2)
#define MSG_CHANGE_RACK (WM_USER + 3)
#define MSG_CHANGE_FOLLOW (WM_USER + 4)

#define GUI_ID_WINDOW0  (GUI_ID_USER + 0x00)
#define GUI_ID_WINDOW1  (GUI_ID_USER + 0x01)
#define GUI_ID_WINDOW2  (GUI_ID_USER + 0x02)
#define GUI_ID_WINDOW3  (GUI_ID_USER + 0x03)


#define GUI_ID_TEXT10	(GUI_ID_USER + 0x10)
#define GUI_ID_TEXT11	(GUI_ID_USER + 0x11)
#define GUI_ID_TEXT12	(GUI_ID_USER + 0x12)
#define GUI_ID_TEXT13	(GUI_ID_USER + 0x13)
#define GUI_ID_TEXT14	(GUI_ID_USER + 0x14)
#define GUI_ID_TEXT15	(GUI_ID_USER + 0x15)
#define GUI_ID_TEXT16	(GUI_ID_USER + 0x16)
#define GUI_ID_TEXT17	(GUI_ID_USER + 0x17)
#define GUI_ID_TEXT18	(GUI_ID_USER + 0x18)
#define GUI_ID_TEXT19	(GUI_ID_USER + 0x19)
#define GUI_ID_TEXT20	(GUI_ID_USER + 0x20)

//jiting
#define GUI_ID_TEXT21	(GUI_ID_USER + 0x21)
#define GUI_ID_TEXT22	(GUI_ID_USER + 0x22)
#define GUI_ID_TEXT23	(GUI_ID_USER + 0x23)
#define GUI_ID_TEXT24	(GUI_ID_USER + 0x24)
#define GUI_ID_TEXT25	(GUI_ID_USER + 0x25)
#define GUI_ID_TEXT26	(GUI_ID_USER + 0x26)
#define GUI_ID_TEXT27	(GUI_ID_USER + 0x27)
#define GUI_ID_TEXT28	(GUI_ID_USER + 0x28)
#define GUI_ID_TEXT29	(GUI_ID_USER + 0x29)

#define GUI_ID_TEXT30	(GUI_ID_USER + 0x30)
#define GUI_ID_TEXT31	(GUI_ID_USER + 0x31)
#define GUI_ID_TEXT32	(GUI_ID_USER + 0x32)
#define GUI_ID_TEXT33	(GUI_ID_USER + 0x33)
#define GUI_ID_TEXT34	(GUI_ID_USER + 0x34)
#define GUI_ID_TEXT35	(GUI_ID_USER + 0x35)
#define GUI_ID_TEXT36	(GUI_ID_USER + 0x36)
#define GUI_ID_TEXT37	(GUI_ID_USER + 0x37)
#define GUI_ID_TEXT38	(GUI_ID_USER + 0x38)				   
#define GUI_ID_TEXT39	(GUI_ID_USER + 0x39)


#define GUI_ID_TEXT40	(GUI_ID_USER + 0x40)
#define GUI_ID_TEXT41	(GUI_ID_USER + 0x41)
#define GUI_ID_TEXT42	(GUI_ID_USER + 0x42)
#define GUI_ID_TEXT43	(GUI_ID_USER + 0x43)
#define GUI_ID_TEXT44	(GUI_ID_USER + 0x44)
#define GUI_ID_TEXT45	(GUI_ID_USER + 0x45)
#define GUI_ID_TEXT46	(GUI_ID_USER + 0x46)
#define GUI_ID_TEXT47	(GUI_ID_USER + 0x47)
#define GUI_ID_TEXT48	(GUI_ID_USER + 0x48)				   
#define GUI_ID_TEXT49	(GUI_ID_USER + 0x49)



enum
{
	homewin = 1,
	followwin,
	masterwin,
	slverwin,
	rackwin,
	passwin,
	groupwin,
	statewin,
	menuwin,
	Serverhmenuwin,
	developerwin
};
	

typedef struct{
	
	uint32_t   MENUID; 	
 	uint8_t    menupaga;  
	uint8_t    Singlepaga; 
	uint8_t    SingleMAX; 
	uint8_t    Grouppaga;  
	uint8_t    GroupMAX; 
 	uint8_t    level; 	
	uint8_t    rank[LEVEL_MAX]; 
	uint8_t    row; 
	uint8_t    column; 
	uint8_t    config;
	uint8_t    BufInd;
	uint8_t    Auxiliary;
}USER_PARAM;




/*
************************************************************************
*						emwin字库
************************************************************************
*/



extern const GUI_FONT GUI_FontHZ46x46 ;
extern const GUI_FONT GUI_FontHZ50x50 ;
extern const GUI_FONT GUI_FontHZ45x45 ;
extern const GUI_FONT GUI_FontHZ40x40 ;
extern const GUI_FONT GUI_FontHZ30x30 ;
extern const GUI_FONT GUI_FontHZ24x24 ;
extern const GUI_FONT GUI_FontHZ12;
extern const GUI_FONT GUI_FontHZ16;
extern const GUI_FONT GUI_FontHZ24;
extern const GUI_FONT GUI_FontHZ32;

extern GUI_CONST_STORAGE GUI_BITMAP bmleft_btn1_hover;
extern GUI_CONST_STORAGE GUI_BITMAP bmleft_btn2_hover;
extern GUI_CONST_STORAGE GUI_BITMAP bmleft_btn3_hover;
extern GUI_CONST_STORAGE GUI_BITMAP bmright_btn1_hover;
extern GUI_CONST_STORAGE GUI_BITMAP bmright_btn3_hover;
/*
************************************************************************
*						供外部文件调用
************************************************************************
*/
void USER_WM_PAINT(void);
//设置函数
void SetPageValue(uint8_t value);
void SetRowValue(uint8_t value);
void SetconfigState(uint8_t value);
void Setrankvalue(uint8_t i,uint8_t value);
void SetLevelValue(uint8_t value);
void SetBufIndValue(uint8_t value);
void SetGroupPageValue(uint8_t value);
void SetSinglePageValue(uint8_t value);
void SetSingleMAXValue(uint8_t value);
void SetGroupMAXValue(uint8_t value);
void SetAuxiliaryPageValue(uint8_t value);
//获取函数

uint8_t GetGroupPageValue(void);
uint8_t GetSinglePageValue(void);
uint8_t GetPageValue(void);
uint8_t GetRowValue(void);
uint8_t GetconfigState(void);
uint64_t Getrankvalue(uint8_t * arr);
uint8_t GetLevelValue(void);
uint8_t GetBufIndValue(void);
uint8_t GetSingleMAXValue(void);
uint8_t GetGroupMAXValue(void);
uint8_t GetAuxiliaryPageValue(void);
void ChangeInfoText(WM_HWIN hWin,uint16_t  pStr);
void ChangeInfoKey(WM_HWIN hWin,int  pKey,int pCnt);
void ChangeInfoButton(WM_HWIN hWin,uint16_t  pStr);
void ChangeInfoRack(WM_HWIN hWin,uint16_t  pStr);
void OvertimeClose(void);
void OvertimeOpen(void);
void OvertimeZero(void);
void OvertimeReturn(WM_MESSAGE * pMsg);
void Setstheinitialstate(void);
void Determineneedtoreturninterface(WM_MESSAGE * pMsg,uint8_t val);
#endif
