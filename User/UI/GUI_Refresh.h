/*
*********************************************************************************************************
*	                                  
*	ģ������ : GUI����������
*	�ļ����� : HomeTask.c
*	��    �� : V1.0
*	˵    �� : GUI����������
*		�汾��   ����         ����            ˵��
*		v1.0    2015-08-05  Eric2013  	      �װ�
*
*	Copyright (C), 2015-2020, ���������� www.armfly.com
*
*********************************************************************************************************
*/

#ifndef __GUI_REFRESH_H
#define __GUI_REFRESH_H

#include "User_GUI.h"
#include "User_Button.h"

/*
*********************************************************************************************************
*	                                     �궨��
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
*						���ⲿ�ļ�����
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
