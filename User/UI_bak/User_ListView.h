/*
*********************************************************************************************************
*	                                  
*	模块名称 : User_Listview用户库
*	文件名称 : User_Listview.h
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


#ifndef __User_Listview_H
#define __User_Listview_H

#include "User_GUI.h"

enum	
{
	DISPLAY_NULL,
	DISPLAY_NO_SHOW,
	DISPLAY_SHOW,
	DISPLAY_SPECAL_SHOW,
	DISPLAY_SPECAL_SHOW1,
	
};
enum	
{

    EDIT_SEL		=	1,		//选择项输入
	EDIT_INT		=	2,		//整型数据
	EDIT_FLOAT		=	3,		//浮点数-单精度
	
	EDIT_DOUBLE		=	4,		//浮点数-双精度
    EDIT_NOTE		=	5,		//显示项无输入
    EDIT_CMD		=	6,		//命令项

    EDIT_SEL_CMD	=	7,		//命令选择项
	
	EDIT_SIGNED_INT  = 8,		//有符号整型		
    EDIT_SIGNED_FLOAT	=	9,	//有符号浮点型

};
//列表头结构体
typedef struct
{
	uint16_t wide;
	char u8headerMame[9];				
}LISTVIEW_HEADER;
/*
************************************************************************
*						供外部文件调用
************************************************************************
*/

void USER_ReviseListview(WM_MESSAGE * pMsg);
void USER_ReviseListviewHeader(WM_MESSAGE * pMsg,const LISTVIEW_HEADER *hader,uint8_t count);
void USER_ReviseListviewRow(WM_MESSAGE * pMsg);
void USER_ChooseListviewStartRow(WM_MESSAGE * pMsg);
void USER_MoveListviewRow(WM_MESSAGE * pMsg,uint8_t Direction);
void USER_EnterListviewRow(WM_MESSAGE * pMsg);
void USER_ExitListviewRow(WM_MESSAGE * pMsg);
void USER_SetValueListviewData(WM_MESSAGE * pMsg,uint16_t key);
void USER_ShwoListviewData(WM_MESSAGE * pMsg,uint8_t count);
uint8_t USER_SaveValueListviewData(WM_MESSAGE * pMsg,uint16_t key,uint64_t id);
uint8_t GetListviewData(uint64_t id);
#endif
