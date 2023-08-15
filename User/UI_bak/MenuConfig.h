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


#ifndef __Menu_Config_H
#define __Menu_Config_H

//#include "User_GUI.h"
#include <stdio.h>
#include <stdint.h>
#include "WM.h"
#define PAGE_0_0_0     0x000000
typedef struct INITLIST INITLIST;	  
typedef struct SETLIST SETLIST;
/**参数结构体**/

typedef struct
{
	char         Para_Name[13];	//参数名称
	uint8_t	 pSelItem;	//选择项表
	uint16_t     Max_Value;	//最大值,
	uint16_t     Min_Value;	//最小值
	uint8_t 	 Modift_Type;	//修改类型			0-全面参数；1-全面&本地		参数
	uint8_t 	 Pass_Word;   //密码权限    
	uint8_t 	 Input_Type;	//输入类型
	uint8_t 	 Show_Noshow;	//显示或不显示   	0-不显示；1-显示	
	uint16_t 	 Para_Value;	//数值
	uint16_t   	 Unit_Name; 	//单位
}MENU_PARA;

struct INITLIST
{
	MENU_PARA  *Parameter;
	uint16_t   Para_Value;
	uint16_t  Deviation;	//偏移
};
struct SETLIST
{
	MENU_PARA  Parameter;
	uint16_t  Deviation;	//偏移
	uint8_t  Scope;  //修改范围
};

/*
************************************************************************
*						供外部文件调用
************************************************************************
*/

//extern const char *ButtonName[];
//extern char TextName[];
void Creator_Menuwindow(WM_MESSAGE * pMsg);
void Menu_SetCurrentPage(uint16_t key);
void Menu_ShowButtonForPage(WM_MESSAGE * pMsg,uint64_t id);
void Menu_ShowListviewForPage(WM_MESSAGE * pMsg,uint64_t id);
void Menu_ShowTextForPage(WM_MESSAGE * pMsg,uint64_t id);
void GetPointerFromLogicalPast(uint64_t id,MENU_PARA **address ,uint8_t *length);
uint8_t GetMAXpage(uint8_t id);
#endif
