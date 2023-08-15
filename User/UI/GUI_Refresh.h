/*
*********************************************************************************************************
*	                                  
*	模块名称 : GUI界面主函数
*	文件名称 : HomeTask.c
*	版    本 : V1.0
*	说    明 : GUI界面主函数
*		版本号   日期         作者            说明
*		v1.0    2015-08-05  Eric2013  	      首版
*
*	Copyright (C), 2015-2020, 安富莱电子 www.armfly.com
*
*********************************************************************************************************
*/

#ifndef __GUI_REFRESH_H
#define __GUI_REFRESH_H

#include "User_GUI.h"
#include "User_Button.h"

/*
*********************************************************************************************************
*	                                     宏定义
*********************************************************************************************************
*/

enum
{
	SUPPORTSTATE_FREE = 0,
	SUPPORTSTATE_FOLLOW = 1,
	SUPPORTSTATE_FOLLOW_STOP = 2
};

/*
************************************************************************
*						  FatFs
************************************************************************
*/
#define ACTION_INFO_SIZE   6
typedef struct
{
	NameAndId     actionInfo;
	void  *            pNext;
} stActionInfo;


typedef struct {
	stActionInfo action_info_queue[ACTION_INFO_SIZE];
	stActionInfo *pFree;
	stActionInfo *pUsed;
	uint8_t action_info_count;
}ACTOIN_INFO;







/*
************************************************************************
*						供外部文件调用
************************************************************************
*/
void SupportStatusRefresh(WM_HWIN  pMsg,uint16_t arr);
void SupportNumberRefresh(WM_MESSAGE * pMsg,uint8_t initState);
void RunningDirectionRefresh(WM_MESSAGE * pMsg);
void GroupRefresh(WM_MESSAGE * pMsg,NameAndId * buf,uint16_t Direction,GroupStartUp * pdata);
void Info_Action_Init(void);
void InsActionInfoQueue(NameAndId *actionInfo);
void DelActionInfoQueue(NameAndId *actionInfo);
uint8_t ReadAction_API(NameAndId *info);
void TransportPlaneRefresh(WM_MESSAGE * pMsg);
void DisplayItemRefresh(WM_MESSAGE * pMsg ,uint8_t initState);
void SupporthomeRefresh(WM_MESSAGE * pMsg);
void SupportSlaveRefresh(WM_MESSAGE * pMsg);
void CurrentDataRefresh(WM_MESSAGE * pMsg);
void SupportStopRefresh(WM_MESSAGE * pMsg);
void StatusRefresh(WM_MESSAGE * pMsg,uint8_t initState);
void PersonLockStatusRefresh(WM_HWIN  pMsg);
stActionInfo * checkActionInfoQueue(NameAndId *actionInfo);
#endif

/*****************************(END OF FILE) *********************************/
