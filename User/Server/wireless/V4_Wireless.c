/*
*********************************************************************************************************
*	                                            V4
*                           Commuication Controller For The Mining Industry
*
*                                  (c) Copyright 1994-2013  HNDZ 
*                       All rights reserved.  Protected by international copyright laws.
*
*
*    File    : V4_Wireless.c
*    Module  : user
*    Version : V1.0
*    History :
*   -----------------
*              Version  Date           By            Note
*              v1.0         
*
*********************************************************************************************************
*/

#include "includes.h"
#include "V4_Wireless.h"
#include "Wireless_Bus.h"
#include "Util_Timer.h"
#include "Util_CRC.h"
#include "DoFatfsFiles.h"
#include "FileLoad.h"
#include "sys_msg_def.h"
#include "SysFunc_API.h"
#include "IR_Rx.h"
#include "udp_app_proc.h"
#include "ProjectConfig.h"

#include "Wireless_Alarm.h"

uint16_t WLOnlineTimer = TIMERCLOSE;//����ģ������״̬
uint16_t BusDataProcTimer = TIMERCLOSE;//bus�����ݴ���
uint16_t YkqManualSingleTimer = TIMERCLOSE;//������ʱ��
uint16_t YkqIrTimer = TIMERCLOSE;//�������ɹ�
uint16_t YkqStopStateTimer = TIMERCLOSE;//ң����ֹͣͣ״̬
uint16_t YkqDecodeTimer = TIMERCLOSE;

uint8_t WirelessUniversalTimerId;
V4YKQ_DATA V4ykq;
uint8_t YkqStopFlag = 0;

uint8_t WirelessVersion[3];
uint8_t WirelessOnline = 0;
uint8_t lastLiushuiNum = 0;

uint8_t V4ykqParaQueue[YKQ_PARA_SIZE];
u16 V4ykqParaSize = 0;

uint8_t YkqLiushuiNumber = 0;
uint8_t ManualActionNumber[10] = {0};
uint32_t ManualActionBitNumber = 0;
uint32_t NeighborManualActionBitNumber = 0;

uint32_t manualActionID = 0;


#define ACTION_NUM_MAX		96

//�����ļ�����
__packed typedef struct
{
	u8 ActionType;			//��������
	u8 ActionID;			//����ID
	u8 ActionName[12];		//������
	u8 MannulCtrlMode;		//�ֶ�����
	u8 AutoCtrlMode;		//�Զ�����
	u8 ActionKey;			//����ֵ
	u8 ForbidActionType;		//��ֹ��������
	u8 ForbidActionID;		//��ֹ����id
}ActionMess;
//�����ļ�ͷ��
__packed typedef struct
{
	u8 Veron[4];		//�汾��Ϣ
	u8 ApplyScene;		//Ӧ�ó���
	u8 TotalAction;		//��������
	u16 Verify;		//У��
	u8  Nouse[24];
}ActionInfoHead;
//�����ļ���Ϣ
__packed typedef struct
{
	ActionInfoHead Head;
	ActionMess	Body[ACTION_NUM_MAX];
}ACTION_INFO;


typedef enum
{
	NO_ACTION,
	SIGNAL_ACTION,
	COMBINATION_ACTION,
	GROUP_ACTION,
}ACTION_TYPE;


//�۷�
#if (APPLICATION_SCENARIO_SELECTION == STANDARD_FOUR_PILLAR_FULLY_MECHANIZED)

ACTION_INFO ActionInformation = 
{
	.Head = 
	{
		.Veron ={0x12,0x34,0x56,0x78},
		.ApplyScene = 1,
		.TotalAction = 42,
		.Verify = 0x5ded,
		.Nouse = {0},
	},
	.Body = 
	{
		{	.ActionType = SIGNAL_ACTION,
			.ActionID = 2,
			.ActionName = "��ǰ��",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = DISABLE,
			.ActionKey = 1,
			.ForbidActionType = SIGNAL_ACTION,
			.ForbidActionID = 1,
		},
		{	.ActionType = SIGNAL_ACTION,
			.ActionID = 1,
			.ActionName = "��ǰ��",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = DISABLE,
			.ActionKey = 2,
			.ForbidActionType = SIGNAL_ACTION,
			.ForbidActionID = 1,
		},
		{	.ActionType = SIGNAL_ACTION,
			.ActionID = 12,
			.ActionName = "������",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = DISABLE,
			.ActionKey = 3,
			.ForbidActionType = SIGNAL_ACTION,
			.ForbidActionID = 11,
		},
		{	.ActionType = SIGNAL_ACTION,
			.ActionID = 11,
			.ActionName = "������",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = DISABLE,
			.ActionKey = 4,
			.ForbidActionType = SIGNAL_ACTION,
			.ForbidActionID = 12,
		},
		{	.ActionType = SIGNAL_ACTION,
			.ActionID = 3,
			.ActionName = "�Ƽ�",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = DISABLE,
			.ActionKey = 5,
			.ForbidActionType = SIGNAL_ACTION,
			.ForbidActionID = 4,
		},
		{	.ActionType = SIGNAL_ACTION,
			.ActionID = 4,
			.ActionName = "����",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = ENABLE,
			.ActionKey = 6,
			.ForbidActionType = SIGNAL_ACTION,
			.ForbidActionID = 3,
		},
		{	.ActionType = SIGNAL_ACTION,
			.ActionID = 10,
			.ActionName = "�커��",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = DISABLE,
			.ActionKey = 7,
			.ForbidActionType = SIGNAL_ACTION,
			.ForbidActionID = 9,
		},
		{	.ActionType = SIGNAL_ACTION,
			.ActionID = 9,
			.ActionName = "�ջ���",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = DISABLE,
			.ActionKey = 8,
			.ForbidActionType = SIGNAL_ACTION,
			.ForbidActionID = 10,
		},
		{	.ActionType = SIGNAL_ACTION,
			.ActionID = 8,
			.ActionName = "������",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = DISABLE,
			.ActionKey = 9,
			.ForbidActionType = SIGNAL_ACTION,
			.ForbidActionID = 7,
		},
		{	.ActionType = SIGNAL_ACTION,
			.ActionID = 7,
			.ActionName = "������",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = DISABLE,
			.ActionKey = 10,
			.ForbidActionType = SIGNAL_ACTION,
			.ForbidActionID = 8,
		},
		{	.ActionType = SIGNAL_ACTION,
			.ActionID = 28,
			.ActionName = "������",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = DISABLE,
			.ActionKey = 11,
			.ForbidActionType = SIGNAL_ACTION,
			.ForbidActionID = 0,
		},
		{	.ActionType = SIGNAL_ACTION,
			.ActionID = 5,
			.ActionName = "̧��",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = DISABLE,
			.ActionKey = 12,
			.ForbidActionType = SIGNAL_ACTION,
			.ForbidActionID = 0,
		},
		{	.ActionType = SIGNAL_ACTION,
			.ActionID = 18,
			.ActionName = "��β��",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = DISABLE,
			.ActionKey = 13,
			.ForbidActionType = SIGNAL_ACTION,
			.ForbidActionID = 17,
		},
		{	.ActionType = SIGNAL_ACTION,
			.ActionID = 17,
			.ActionName = "��β��",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = DISABLE,
			.ActionKey = 14,
			.ForbidActionType = SIGNAL_ACTION,
			.ForbidActionID = 18,
		},
		{	.ActionType = SIGNAL_ACTION,
			.ActionID = 14,
			.ActionName = "��ǰ��",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = DISABLE,
			.ActionKey = 15,
			.ForbidActionType = SIGNAL_ACTION,
			.ForbidActionID = 13,
		},
		{	.ActionType = SIGNAL_ACTION,
			.ActionID = 13,
			.ActionName = "��ǰ��",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = DISABLE,
			.ActionKey = 16,
			.ForbidActionType = SIGNAL_ACTION,
			.ForbidActionID = 14,
		},
		{	.ActionType = SIGNAL_ACTION,
			.ActionID = 20,
			.ActionName = "����",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = DISABLE,
			.ActionKey = 17,
			.ForbidActionType = SIGNAL_ACTION,
			.ForbidActionID = 19,
		},
		{	.ActionType = SIGNAL_ACTION,
			.ActionID = 19,
			.ActionName = "�ղ��",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = DISABLE,
			.ActionKey = 18,
			.ForbidActionType = SIGNAL_ACTION,
			.ForbidActionID = 20,
		},
		{	.ActionType = SIGNAL_ACTION,
			.ActionID = 16,
			.ActionName = "�춥��",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = DISABLE,
			.ActionKey = 19,
			.ForbidActionType = SIGNAL_ACTION,
			.ForbidActionID = 15,
		},
		{	.ActionType = SIGNAL_ACTION,
			.ActionID = 15,
			.ActionName = "�ն���",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = DISABLE,
			.ActionKey = 20,
			.ForbidActionType = SIGNAL_ACTION,
			.ForbidActionID = 16,
		},
		{	.ActionType = SIGNAL_ACTION,
			.ActionID = 24,
			.ActionName = "���ڲ�",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = DISABLE,
			.ActionKey = 21,
			.ForbidActionType = SIGNAL_ACTION,
			.ForbidActionID = 23,
		},		
		{	.ActionType = SIGNAL_ACTION,
			.ActionID = 23,
			.ActionName = "���ڲ�",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = DISABLE,
			.ActionKey = 22,
			.ForbidActionType = SIGNAL_ACTION,
			.ForbidActionID = 24,
		},
		{	.ActionType = SIGNAL_ACTION,
			.ActionID = 27,
			.ActionName = "�ɺ���",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = DISABLE,
			.ActionKey = 23,
			.ForbidActionType = SIGNAL_ACTION,
			.ForbidActionID = 28,
		},
		{	.ActionType = SIGNAL_ACTION,
			.ActionID = 29,
			.ActionName = "����",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = DISABLE,
			.ActionKey = 24,
			.ForbidActionType = SIGNAL_ACTION,
			.ForbidActionID = 0,
		},
		{	.ActionType = SIGNAL_ACTION,
			.ActionID = 30,
			.ActionName = "������",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = DISABLE,
			.ActionKey = 24,
			.ForbidActionType = SIGNAL_ACTION,
			.ForbidActionID = 0,
		},
		{	.ActionType = COMBINATION_ACTION,
			.ActionID = 1,
			.ActionName = "�Զ��Ƽ�",
			.MannulCtrlMode = DISABLE,
			.AutoCtrlMode = ENABLE,
			.ActionKey = 5,
			.ForbidActionType = COMBINATION_ACTION,
			.ForbidActionID = 0,
		},
		{	.ActionType = COMBINATION_ACTION,
			.ActionID = 3,
			.ActionName = "�Զ�������",
			.MannulCtrlMode = DISABLE,
			.AutoCtrlMode = ENABLE,
			.ActionKey = 7,
			.ForbidActionType = COMBINATION_ACTION,
			.ForbidActionID = 3,
		},
		{	.ActionType = COMBINATION_ACTION,
			.ActionID = 2,
			.ActionName = "�Զ�������",
			.MannulCtrlMode = DISABLE,
			.AutoCtrlMode = ENABLE,
			.ActionKey = 8,
			.ForbidActionType = COMBINATION_ACTION,
			.ForbidActionID = 4,
		},
		{	.ActionType = COMBINATION_ACTION,
			.ActionID = 4,
			.ActionName = "�Զ���ú",
			.MannulCtrlMode = DISABLE,
			.AutoCtrlMode = ENABLE,
			.ActionKey = 13,
			.ForbidActionType = COMBINATION_ACTION,
			.ForbidActionID = 0,
		},		
		{	.ActionType = GROUP_ACTION,
			.ActionID = 13,
			.ActionName = "���鲹ѹ",
			.MannulCtrlMode = DISABLE,
			.AutoCtrlMode = ENABLE,
			.ActionKey = 1,
			.ForbidActionType = GROUP_ACTION,
			.ForbidActionID = 0,
		},	
		{	.ActionType = GROUP_ACTION,
			.ActionID = 2,
			.ActionName = "�����Ƽ�",
			.MannulCtrlMode = DISABLE,
			.AutoCtrlMode = ENABLE,
			.ActionKey = 5,
			.ForbidActionType = GROUP_ACTION,
			.ForbidActionID = 0,
		},
		{	.ActionType = GROUP_ACTION,
			.ActionID = 1,
			.ActionName = "��������",
			.MannulCtrlMode = DISABLE,
			.AutoCtrlMode = ENABLE,
			.ActionKey = 6,
			.ForbidActionType = GROUP_ACTION,
			.ForbidActionID = 0,
		},
		{	.ActionType = GROUP_ACTION,
			.ActionID = 8,
			.ActionName = "�����커��",
			.MannulCtrlMode = DISABLE,
			.AutoCtrlMode = ENABLE,
			.ActionKey = 7,
			.ForbidActionType = GROUP_ACTION,
			.ForbidActionID = 0,
		},
		{	.ActionType = GROUP_ACTION,
			.ActionID = 7,
			.ActionName = "�����ջ���",
			.MannulCtrlMode = DISABLE,
			.AutoCtrlMode = ENABLE,
			.ActionKey = 8,
			.ForbidActionType = GROUP_ACTION,
			.ForbidActionID = 0,
		},
		{	.ActionType = GROUP_ACTION,
			.ActionID = 6,
			.ActionName = "����������",
			.MannulCtrlMode = DISABLE,
			.AutoCtrlMode = ENABLE,
			.ActionKey = 9,
			.ForbidActionType = GROUP_ACTION,
			.ForbidActionID = 0,
		},
		{	.ActionType = GROUP_ACTION,
			.ActionID = 5,
			.ActionName = "����������",
			.MannulCtrlMode = DISABLE,
			.AutoCtrlMode = ENABLE,
			.ActionKey = 10,
			.ForbidActionType = GROUP_ACTION,
			.ForbidActionID = 0,
		},	
		{	.ActionType = GROUP_ACTION,
			.ActionID = 9,
			.ActionName = "����������",
			.MannulCtrlMode = DISABLE,
			.AutoCtrlMode = ENABLE,
			.ActionKey = 11,
			.ForbidActionType = GROUP_ACTION,
			.ForbidActionID = 0,
		},
		{	.ActionType = GROUP_ACTION,
			.ActionID = 12,
			.ActionName = "�����ú",
			.MannulCtrlMode = DISABLE,
			.AutoCtrlMode = ENABLE,
			.ActionKey = 13,
			.ForbidActionType = GROUP_ACTION,
			.ForbidActionID = 0,
		},		
		{	.ActionType = GROUP_ACTION,
			.ActionID = 10,
			.ActionName = "��������",
			.MannulCtrlMode = DISABLE,
			.AutoCtrlMode = ENABLE,
			.ActionKey = 17,
			.ForbidActionType = GROUP_ACTION,
			.ForbidActionID = 0,
		},
		{	.ActionType = GROUP_ACTION,
			.ActionID = 4,
			.ActionName = "����������",
			.MannulCtrlMode = DISABLE,
			.AutoCtrlMode = ENABLE,
			.ActionKey = 21,
			.ForbidActionType = GROUP_ACTION,
			.ForbidActionID = 0,
		},
		{	.ActionType = GROUP_ACTION,
			.ActionID = 3,
			.ActionName = "����������",
			.MannulCtrlMode = DISABLE,
			.AutoCtrlMode = ENABLE,
			.ActionKey = 22,
			.ForbidActionType = GROUP_ACTION,
			.ForbidActionID = 0,
		},		
		{	.ActionType = GROUP_ACTION,
			.ActionID = 11,
			.ActionName = "��������",
			.MannulCtrlMode = DISABLE,
			.AutoCtrlMode = ENABLE,
			.ActionKey = 24,
			.ForbidActionType = GROUP_ACTION,
			.ForbidActionID = 0,
		},
	},
};


#elif (APPLICATION_SCENARIO_SELECTION == LIVE_CHICKEN_AND_RABBIT_WORKING_FACE || APPLICATION_SCENARIO_SELECTION == LIVE_CHICKEN_AND_RABBIT_LEAD_SUPPORT)
//����м�ܼ���ͷ��
ACTION_INFO ActionInformation = 
{
	.Head = 
	{
		.Veron ={0x12,0x34,0x55,0x78},
		.ApplyScene = 1,
		.TotalAction = 21,
		.Verify = 0x3E08,
		.Nouse = {0},
	},
	.Body = 
	{
		{	.ActionType = SIGNAL_ACTION,
			.ActionID = 4,
			.ActionName = "�Ƽ�",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = DISABLE,
			.ActionKey = 4,
			.ForbidActionType = SIGNAL_ACTION,
			.ForbidActionID = 3,
		},
		{	.ActionType = SIGNAL_ACTION,
			.ActionID = 2,
			.ActionName = "����",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = DISABLE,
			.ActionKey = 1,
			.ForbidActionType = SIGNAL_ACTION,
			.ForbidActionID = 1,
		},
		{	.ActionType = SIGNAL_ACTION,
			.ActionID = 3,
			.ActionName = "����",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = ENABLE,
			.ActionKey = 3,
			.ForbidActionType = SIGNAL_ACTION,
			.ForbidActionID = 4,
		},
		{	.ActionType = SIGNAL_ACTION,
			.ActionID = 1,
			.ActionName = "����",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = DISABLE,
			.ActionKey = 2,
			.ForbidActionType = SIGNAL_ACTION,
			.ForbidActionID = 2,
		},
		{	.ActionType = SIGNAL_ACTION,
			.ActionID = 5,
			.ActionName = "��ƽ��",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = DISABLE,
			.ActionKey = 6,
			.ForbidActionType = SIGNAL_ACTION,
			.ForbidActionID = 6,
		},
		{	.ActionType = SIGNAL_ACTION,
			.ActionID = 6,
			.ActionName = "��ƽ��",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = DISABLE,
			.ActionKey = 5,
			.ForbidActionType = SIGNAL_ACTION,
			.ForbidActionID = 5,
		},
		{	.ActionType = SIGNAL_ACTION,
			.ActionID = 7,
			.ActionName = "�ղ໤",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = DISABLE,
			.ActionKey = 18,
			.ForbidActionType = 1,
			.ForbidActionID = 8,
		},
		{	.ActionType = SIGNAL_ACTION,
			.ActionID = 8,
			.ActionName = "��໤",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = DISABLE,
			.ActionKey = 17,
			.ForbidActionType = SIGNAL_ACTION,
			.ForbidActionID = 7,
		},
		{	.ActionType = SIGNAL_ACTION,
			.ActionID = 10,
			.ActionName = "̧����",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = DISABLE,
			.ActionKey = 11,
			.ForbidActionType = SIGNAL_ACTION,
			.ForbidActionID = 0,
		},
		{	.ActionType = SIGNAL_ACTION,
			.ActionID = 9,
			.ActionName = "����",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = ENABLE,
			.ActionKey = 12,
			.ForbidActionType = SIGNAL_ACTION,
			.ForbidActionID = 0,
		},
		{	.ActionType = SIGNAL_ACTION,
			.ActionID = 11,
			.ActionName = "��������",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = DISABLE,
			.ActionKey = 10,
			.ForbidActionType = SIGNAL_ACTION,
			.ForbidActionID = 0,
		},
		{	.ActionType = SIGNAL_ACTION,
			.ActionID = 12,
			.ActionName = "��������",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = DISABLE,
			.ActionKey = 9,
			.ForbidActionType = SIGNAL_ACTION,
			.ForbidActionID = 0,
		},
		{	.ActionType = COMBINATION_ACTION,
			.ActionID = 1,
			.ActionName = "�Զ��Ƽ�",
			.MannulCtrlMode = DISABLE,
			.AutoCtrlMode = ENABLE,
			.ActionKey = 4,
			.ForbidActionType = COMBINATION_ACTION,
			.ForbidActionID = 0,
		},
		{	.ActionType = COMBINATION_ACTION,
			.ActionID = 2,
			.ActionName = "�Զ���ѹ",
			.MannulCtrlMode = DISABLE,
			.AutoCtrlMode = ENABLE,
			.ActionKey = 1,
			.ForbidActionType = COMBINATION_ACTION,
			.ForbidActionID = 0,
		},
		{	.ActionType = GROUP_ACTION,
			.ActionID = 2,
			.ActionName = "�����Ƽ�",
			.MannulCtrlMode = DISABLE,
			.AutoCtrlMode = ENABLE,
			.ActionKey = 4,
			.ForbidActionType = GROUP_ACTION,
			.ForbidActionID = 0,
		},
		{	.ActionType = GROUP_ACTION,
			.ActionID = 1,
			.ActionName = "��������",
			.MannulCtrlMode = DISABLE,
			.AutoCtrlMode = ENABLE,
			.ActionKey = 3,
			.ForbidActionType = GROUP_ACTION,
			.ForbidActionID = 0,
		},
		{	.ActionType = GROUP_ACTION,
			.ActionID = 3,
			.ActionName = "��������",
			.MannulCtrlMode = DISABLE,
			.AutoCtrlMode = ENABLE,
			.ActionKey = 16,
			.ForbidActionType = GROUP_ACTION,
			.ForbidActionID = 0,
		},
		{	.ActionType = GROUP_ACTION,
			.ActionID = 4,
			.ActionName = "��������",
			.MannulCtrlMode = DISABLE,
			.AutoCtrlMode = ENABLE,
			.ActionKey = 12,
			.ForbidActionType = GROUP_ACTION,
			.ForbidActionID = 0,
		},
		{	.ActionType = GROUP_ACTION,
			.ActionID = 5,
			.ActionName = "����������",
			.MannulCtrlMode = DISABLE,
			.AutoCtrlMode = ENABLE,
			.ActionKey = 10,
			.ForbidActionType = GROUP_ACTION,
			.ForbidActionID = 0,
		},
		{	.ActionType = GROUP_ACTION,
			.ActionID = 6,
			.ActionName = "����������",
			.MannulCtrlMode = DISABLE,
			.AutoCtrlMode = ENABLE,
			.ActionKey = 9,
			.ForbidActionType = GROUP_ACTION,
			.ForbidActionID = 0,
		},
		{	.ActionType = GROUP_ACTION,
			.ActionID = 7,
			.ActionName = "���鲹ѹ",
			.MannulCtrlMode = DISABLE,
			.AutoCtrlMode = ENABLE,
			.ActionKey = 1,
			.ForbidActionType = GROUP_ACTION,
			.ForbidActionID = 0,
		},
	},
};


#elif (APPLICATION_SCENARIO_SELECTION == LIVE_CHICKEN_AND_RABBIT_SELF_MOVING_TAIL)
//������ƻ�β
ACTION_INFO ActionInformation = 
{
	.Head = 
	{
		.Veron ={0x12,0x34,0x55,0x78},
		.ApplyScene = 1,
		.TotalAction = 16,
		.Verify = 0x5B9F,
		.Nouse = {0},
	},
	.Body = 
	{
		{	.ActionType = SIGNAL_ACTION,
			.ActionID = 1,
			.ActionName = "��ǰ��",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = DISABLE,
			.ActionKey = 1,
			.ForbidActionType = SIGNAL_ACTION,
			.ForbidActionID = 2,
		},
		{	.ActionType = SIGNAL_ACTION,
			.ActionID = 2,
			.ActionName = "��ǰ��",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = DISABLE,
			.ActionKey = 2,
			.ForbidActionType = SIGNAL_ACTION,
			.ForbidActionID = 1,
		},
		{	.ActionType = SIGNAL_ACTION,
			.ActionID = 3,
			.ActionName = "�Һ���",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = DISABLE,
			.ActionKey = 3,
			.ForbidActionType = SIGNAL_ACTION,
			.ForbidActionID = 4,
		},
		{	.ActionType = SIGNAL_ACTION,
			.ActionID = 4,
			.ActionName = "�Һ�",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = DISABLE,
			.ActionKey = 4,
			.ForbidActionType = SIGNAL_ACTION,
			.ForbidActionID = 3,
		},
		{	.ActionType = SIGNAL_ACTION,
			.ActionID = 5,
			.ActionName = "ǰ��",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = DISABLE,
			.ActionKey = 5,
			.ForbidActionType = SIGNAL_ACTION,
			.ForbidActionID = 6,
		},
		{	.ActionType = SIGNAL_ACTION,
			.ActionID = 6,
			.ActionName = "����",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = DISABLE,
			.ActionKey = 6,
			.ForbidActionType = SIGNAL_ACTION,
			.ForbidActionID = 5,
		},
		{	.ActionType = SIGNAL_ACTION,
			.ActionID = 7,
			.ActionName = "��ǰ��",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = DISABLE,
			.ActionKey = 7,
			.ForbidActionType = SIGNAL_ACTION,
			.ForbidActionID = 8,
		},
		{	.ActionType = SIGNAL_ACTION,
			.ActionID = 8,
			.ActionName = "��ǰ��",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = DISABLE,
			.ActionKey = 8,
			.ForbidActionType = SIGNAL_ACTION,
			.ForbidActionID = 7,
		},
		{	.ActionType = SIGNAL_ACTION,
			.ActionID = 9,
			.ActionName = "�����",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = DISABLE,
			.ActionKey = 9,
			.ForbidActionType = SIGNAL_ACTION,
			.ForbidActionID = 10,
		},
		{	.ActionType = SIGNAL_ACTION,
			.ActionID = 10,
			.ActionName = "���",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = DISABLE,
			.ActionKey = 10,
			.ForbidActionType = SIGNAL_ACTION,
			.ForbidActionID = 9,
		},
		{	.ActionType = SIGNAL_ACTION,
			.ActionID = 11,
			.ActionName = "ǰ����",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = DISABLE,
			.ActionKey = 11,
			.ForbidActionType = SIGNAL_ACTION,
			.ForbidActionID = 12,
		},
		{	.ActionType = SIGNAL_ACTION,
			.ActionID = 12,
			.ActionName = "ǰ�ܽ�",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = DISABLE,
			.ActionKey = 12,
			.ForbidActionType = SIGNAL_ACTION,
			.ForbidActionID = 11,
		},
		{	.ActionType = SIGNAL_ACTION,
			.ActionID = 13,
			.ActionName = "ǰ���",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = DISABLE,
			.ActionKey = 13,
			.ForbidActionType = SIGNAL_ACTION,
			.ForbidActionID = 14,
		},
		{	.ActionType = SIGNAL_ACTION,
			.ActionID = 14,
			.ActionName = "ǰ�Ұ�",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = DISABLE,
			.ActionKey = 14,
			.ForbidActionType = SIGNAL_ACTION,
			.ForbidActionID = 13,
		},
		{	.ActionType = SIGNAL_ACTION,
			.ActionID = 15,
			.ActionName = "�����",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = DISABLE,
			.ActionKey = 15,
			.ForbidActionType = SIGNAL_ACTION,
			.ForbidActionID = 0,
		},
		{	.ActionType = SIGNAL_ACTION,
			.ActionID = 16,
			.ActionName = "���Ұ�",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = DISABLE,
			.ActionKey = 16,
			.ForbidActionType = SIGNAL_ACTION,
			.ForbidActionID = 0,
		},		
	},
};




#elif (APPLICATION_SCENARIO_SELECTION == LIVE_CHICKEN_AND_RABBIT_CABLE_CAR)
//����豸�г�
ACTION_INFO ActionInformation = 
{
	.Head = 
	{
		.Veron ={0x12,0x34,0x55,0x78},
		.ApplyScene = 1,
		.TotalAction = 16,
		.Verify = 0x2c0d,
		.Nouse = {0},
	},
	.Body = 
	{
		{	.ActionType = SIGNAL_ACTION,
			.ActionID = 1,
			.ActionName = "���͸�1",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = DISABLE,
			.ActionKey = 1,
			.ForbidActionType = SIGNAL_ACTION,
			.ForbidActionID = 2,
		},
		{	.ActionType = SIGNAL_ACTION,
			.ActionID = 2,
			.ActionName = "���͸�1",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = DISABLE,
			.ActionKey = 2,
			.ForbidActionType = SIGNAL_ACTION,
			.ForbidActionID = 1,
		},
		{	.ActionType = SIGNAL_ACTION,
			.ActionID = 3,
			.ActionName = "���͸�2",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = DISABLE,
			.ActionKey = 3,
			.ForbidActionType = SIGNAL_ACTION,
			.ForbidActionID = 4,
		},
		{	.ActionType = SIGNAL_ACTION,
			.ActionID = 4,
			.ActionName = "���͸�2",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = DISABLE,
			.ActionKey = 4,
			.ForbidActionType = SIGNAL_ACTION,
			.ForbidActionID = 3,
		},
		{	.ActionType = SIGNAL_ACTION,
			.ActionID = 5,
			.ActionName = "���͸�3",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = DISABLE,
			.ActionKey = 5,
			.ForbidActionType = SIGNAL_ACTION,
			.ForbidActionID = 6,
		},
		{	.ActionType = SIGNAL_ACTION,
			.ActionID = 6,
			.ActionName = "���͸�3",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = DISABLE,
			.ActionKey = 6,
			.ForbidActionType = SIGNAL_ACTION,
			.ForbidActionID = 5,
		},
		{	.ActionType = SIGNAL_ACTION,
			.ActionID = 7,
			.ActionName = "���͸�4",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = DISABLE,
			.ActionKey = 7,
			.ForbidActionType = SIGNAL_ACTION,
			.ForbidActionID = 8,
		},
		{	.ActionType = SIGNAL_ACTION,
			.ActionID = 8,
			.ActionName = "���͸�4",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = DISABLE,
			.ActionKey = 8,
			.ForbidActionType = SIGNAL_ACTION,
			.ForbidActionID = 7,
		},
		{	.ActionType = SIGNAL_ACTION,
			.ActionID = 9,
			.ActionName = "���͸�5",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = DISABLE,
			.ActionKey = 9,
			.ForbidActionType = SIGNAL_ACTION,
			.ForbidActionID = 10,
		},
		{	.ActionType = SIGNAL_ACTION,
			.ActionID = 10,
			.ActionName = "���͸�5",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = DISABLE,
			.ActionKey = 10,
			.ForbidActionType = SIGNAL_ACTION,
			.ForbidActionID = 9,
		},
		{	.ActionType = SIGNAL_ACTION,
			.ActionID = 11,
			.ActionName = "���͸�6",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = DISABLE,
			.ActionKey = 11,
			.ForbidActionType = SIGNAL_ACTION,
			.ForbidActionID = 12,
		},
		{	.ActionType = SIGNAL_ACTION,
			.ActionID = 12,
			.ActionName = "���͸�6",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = DISABLE,
			.ActionKey = 12,
			.ForbidActionType = SIGNAL_ACTION,
			.ForbidActionID = 11,
		},
		{	.ActionType = SIGNAL_ACTION,
			.ActionID = 13,
			.ActionName = "���͸�7",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = DISABLE,
			.ActionKey = 13,
			.ForbidActionType = SIGNAL_ACTION,
			.ForbidActionID = 14,
		},
		{	.ActionType = SIGNAL_ACTION,
			.ActionID = 14,
			.ActionName = "���͸�7",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = DISABLE,
			.ActionKey = 14,
			.ForbidActionType = SIGNAL_ACTION,
			.ForbidActionID = 13,
		},
		{	.ActionType = SIGNAL_ACTION,
			.ActionID = 15,
			.ActionName = "���͸�8",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = DISABLE,
			.ActionKey = 15,
			.ForbidActionType = SIGNAL_ACTION,
			.ForbidActionID = 0,
		},
		{	.ActionType = SIGNAL_ACTION,
			.ActionID = 16,
			.ActionName = "���͸�8",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = DISABLE,
			.ActionKey = 16,
			.ForbidActionType = SIGNAL_ACTION,
			.ForbidActionID = 0,
		},		
	},
};





#elif (APPLICATION_SCENARIO_SELECTION == DALIUTA_WORKING_FACE)
//�������м��
ACTION_INFO ActionInformation = 
{
	.Head = 
	{
		.Veron ={0x12,0x34,0x55,0x78},
		.ApplyScene = 1,
		.TotalAction = 21,
		.Verify = 0xE22A,
		.Nouse = {0},
	},
	.Body = 
	{
		{	.ActionType = SIGNAL_ACTION,
			.ActionID = 4,
			.ActionName = "�Ƽ�",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = DISABLE,
			.ActionKey = 4,
			.ForbidActionType = SIGNAL_ACTION,
			.ForbidActionID = 3,
		},
		{	.ActionType = SIGNAL_ACTION,
			.ActionID = 2,
			.ActionName = "����",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = DISABLE,
			.ActionKey = 1,
			.ForbidActionType = SIGNAL_ACTION,
			.ForbidActionID = 1,
		},
		{	.ActionType = SIGNAL_ACTION,
			.ActionID = 3,
			.ActionName = "����",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = ENABLE,
			.ActionKey = 3,
			.ForbidActionType = SIGNAL_ACTION,
			.ForbidActionID = 4,
		},
		{	.ActionType = SIGNAL_ACTION,
			.ActionID = 1,
			.ActionName = "����",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = DISABLE,
			.ActionKey = 2,
			.ForbidActionType = SIGNAL_ACTION,
			.ForbidActionID = 2,
		},
		{	.ActionType = SIGNAL_ACTION,
			.ActionID = 5,
			.ActionName = "��ƽ��",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = DISABLE,
			.ActionKey = 6,
			.ForbidActionType = SIGNAL_ACTION,
			.ForbidActionID = 6,
		},
		{	.ActionType = SIGNAL_ACTION,
			.ActionID = 6,
			.ActionName = "��ƽ��",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = DISABLE,
			.ActionKey = 5,
			.ForbidActionType = SIGNAL_ACTION,
			.ForbidActionID = 5,
		},
		{	.ActionType = SIGNAL_ACTION,
			.ActionID = 7,
			.ActionName = "�ղ໤",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = DISABLE,
			.ActionKey = 18,
			.ForbidActionType = 1,
			.ForbidActionID = 8,
		},
		{	.ActionType = SIGNAL_ACTION,
			.ActionID = 8,
			.ActionName = "��໤",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = DISABLE,
			.ActionKey = 17,
			.ForbidActionType = SIGNAL_ACTION,
			.ForbidActionID = 7,
		},
		{	.ActionType = SIGNAL_ACTION,
			.ActionID = 10,
			.ActionName = "̧����",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = DISABLE,
			.ActionKey = 11,
			.ForbidActionType = SIGNAL_ACTION,
			.ForbidActionID = 0,
		},
		{	.ActionType = SIGNAL_ACTION,
			.ActionID = 9,
			.ActionName = "�ջ���2",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = ENABLE,
			.ActionKey = 14,
			.ForbidActionType = SIGNAL_ACTION,
			.ForbidActionID = 0,
		},
		{	.ActionType = SIGNAL_ACTION,
			.ActionID = 11,
			.ActionName = "��������",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = DISABLE,
			.ActionKey = 10,
			.ForbidActionType = SIGNAL_ACTION,
			.ForbidActionID = 0,
		},
		{	.ActionType = SIGNAL_ACTION,
			.ActionID = 12,
			.ActionName = "��������",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = DISABLE,
			.ActionKey = 9,
			.ForbidActionType = SIGNAL_ACTION,
			.ForbidActionID = 0,
		},
		{	.ActionType = SIGNAL_ACTION,
			.ActionID = 13,
			.ActionName = "�ջ���",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = DISABLE,
			.ActionKey = 8,
			.ForbidActionType = SIGNAL_ACTION,
			.ForbidActionID = 14,
		},
		{	.ActionType = SIGNAL_ACTION,
			.ActionID = 14,
			.ActionName = "�커��",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = DISABLE,
			.ActionKey = 7,
			.ForbidActionType = SIGNAL_ACTION,
			.ForbidActionID = 13,
		},	
		{	.ActionType = SIGNAL_ACTION,
			.ActionID = 15,
			.ActionName = "�ջ���3",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = DISABLE,
			.ActionKey = 16,
			.ForbidActionType = SIGNAL_ACTION,
			.ForbidActionID = 16,
		},
		{	.ActionType = SIGNAL_ACTION,
			.ActionID = 16,
			.ActionName = "�커��3",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = DISABLE,
			.ActionKey = 15,
			.ForbidActionType = SIGNAL_ACTION,
			.ForbidActionID = 15,
		},		
		{	.ActionType = SIGNAL_ACTION,
			.ActionID = 17,
			.ActionName = "�󷴳�1",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = DISABLE,
			.ActionKey = 19,
			.ForbidActionType = SIGNAL_ACTION,
			.ForbidActionID = 0,
		},
		{	.ActionType = SIGNAL_ACTION,
			.ActionID = 18,
			.ActionName = "�󷴳�2",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = DISABLE,
			.ActionKey = 20,
			.ForbidActionType = SIGNAL_ACTION,
			.ForbidActionID = 0,
		},
		{	.ActionType = SIGNAL_ACTION,
			.ActionID = 19,
			.ActionName = "�ҷ���1",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = DISABLE,
			.ActionKey = 21,
			.ForbidActionType = SIGNAL_ACTION,
			.ForbidActionID = 0,
		},
		{	.ActionType = SIGNAL_ACTION,
			.ActionID = 20,
			.ActionName = "�ҷ���2",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = DISABLE,
			.ActionKey = 22,
			.ForbidActionType = SIGNAL_ACTION,
			.ForbidActionID = 0,
		},		
		{	.ActionType = COMBINATION_ACTION,
			.ActionID = 1,
			.ActionName = "�Զ��Ƽ�",
			.MannulCtrlMode = DISABLE,
			.AutoCtrlMode = ENABLE,
			.ActionKey = 4,
			.ForbidActionType = COMBINATION_ACTION,
			.ForbidActionID = 0,
		},
		{	.ActionType = COMBINATION_ACTION,
			.ActionID = 2,
			.ActionName = "�Զ���ѹ",
			.MannulCtrlMode = DISABLE,
			.AutoCtrlMode = ENABLE,
			.ActionKey = 1,
			.ForbidActionType = COMBINATION_ACTION,
			.ForbidActionID = 0,
		},
		{	.ActionType = COMBINATION_ACTION,
			.ActionID = 3,
			.ActionName = "�Զ��ջ�����",
			.MannulCtrlMode = DISABLE,
			.AutoCtrlMode = ENABLE,
			.ActionKey = 15,
			.ForbidActionType = COMBINATION_ACTION,
			.ForbidActionID = 0,
		},
		{	.ActionType = COMBINATION_ACTION,
			.ActionID = 4,
			.ActionName = "�Զ��커����",
			.MannulCtrlMode = DISABLE,
			.AutoCtrlMode = ENABLE,
			.ActionKey = 14,
			.ForbidActionType = COMBINATION_ACTION,
			.ForbidActionID = 0,
		},
		{	.ActionType = GROUP_ACTION,
			.ActionID = 2,
			.ActionName = "�����Ƽ�",
			.MannulCtrlMode = DISABLE,
			.AutoCtrlMode = ENABLE,
			.ActionKey = 4,
			.ForbidActionType = GROUP_ACTION,
			.ForbidActionID = 0,
		},
		{	.ActionType = GROUP_ACTION,
			.ActionID = 1,
			.ActionName = "��������",
			.MannulCtrlMode = DISABLE,
			.AutoCtrlMode = ENABLE,
			.ActionKey = 3,
			.ForbidActionType = GROUP_ACTION,
			.ForbidActionID = 0,
		},
		{	.ActionType = GROUP_ACTION,
			.ActionID = 3,
			.ActionName = "��������",
			.MannulCtrlMode = DISABLE,
			.AutoCtrlMode = ENABLE,
			.ActionKey = 12,
			.ForbidActionType = GROUP_ACTION,
			.ForbidActionID = 0,
		},
		{	.ActionType = GROUP_ACTION,
			.ActionID = 4,
			.ActionName = "����������",
			.MannulCtrlMode = DISABLE,
			.AutoCtrlMode = ENABLE,
			.ActionKey = 10,
			.ForbidActionType = GROUP_ACTION,
			.ForbidActionID = 0,
		},
		{	.ActionType = GROUP_ACTION,
			.ActionID = 5,
			.ActionName = "����������",
			.MannulCtrlMode = DISABLE,
			.AutoCtrlMode = ENABLE,
			.ActionKey = 9,
			.ForbidActionType = GROUP_ACTION,
			.ForbidActionID = 0,
		},
		{	.ActionType = GROUP_ACTION,
			.ActionID = 6,
			.ActionName = "���鲹ѹ",
			.MannulCtrlMode = DISABLE,
			.AutoCtrlMode = ENABLE,
			.ActionKey = 1,
			.ForbidActionType = GROUP_ACTION,
			.ForbidActionID = 0,
		},
		{	.ActionType = GROUP_ACTION,
			.ActionID = 7,
			.ActionName = "�����ջ���",
			.MannulCtrlMode = DISABLE,
			.AutoCtrlMode = ENABLE,
			.ActionKey = 8,
			.ForbidActionType = GROUP_ACTION,
			.ForbidActionID = 0,
		},
		{	.ActionType = GROUP_ACTION,
			.ActionID = 8,
			.ActionName = "�����커��",
			.MannulCtrlMode = DISABLE,
			.AutoCtrlMode = ENABLE,
			.ActionKey = 7,
			.ForbidActionType = GROUP_ACTION,
			.ForbidActionID = 0,
		},
		{	.ActionType = GROUP_ACTION,
			.ActionID = 9,
			.ActionName = "����������",
			.MannulCtrlMode = DISABLE,
			.AutoCtrlMode = ENABLE,
			.ActionKey = 16,
			.ForbidActionType = GROUP_ACTION,
			.ForbidActionID = 0,
		},
		{	.ActionType = GROUP_ACTION,
			.ActionID = 10,
			.ActionName = "����������",
			.MannulCtrlMode = DISABLE,
			.AutoCtrlMode = ENABLE,
			.ActionKey = 15,
			.ForbidActionType = GROUP_ACTION,
			.ForbidActionID = 0,
		},		
	},
};

#elif (APPLICATION_SCENARIO_SELECTION == JINJIE_31117_WORKING_FACE)
//�����м��
ACTION_INFO ActionInformation = 
{
	.Head = 
	{
		.Veron ={0x12,0x34,0x55,0x78},
		.ApplyScene = 1,
		.TotalAction = 35,
		.Verify = 0xDAC6,
		.Nouse = {0},
	},
	.Body = 
	{
		{	.ActionType = SIGNAL_ACTION,
			.ActionID = 4,
			.ActionName = "�Ƽ�",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = DISABLE,
			.ActionKey = 4,
			.ForbidActionType = SIGNAL_ACTION,
			.ForbidActionID = 3,
		},
		{	.ActionType = SIGNAL_ACTION,
			.ActionID = 2,
			.ActionName = "����",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = DISABLE,
			.ActionKey = 1,
			.ForbidActionType = SIGNAL_ACTION,
			.ForbidActionID = 1,
		},
		{	.ActionType = SIGNAL_ACTION,
			.ActionID = 3,
			.ActionName = "����",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = ENABLE,
			.ActionKey = 3,
			.ForbidActionType = SIGNAL_ACTION,
			.ForbidActionID = 4,
		},
		{	.ActionType = SIGNAL_ACTION,
			.ActionID = 1,
			.ActionName = "����",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = DISABLE,
			.ActionKey = 2,
			.ForbidActionType = SIGNAL_ACTION,
			.ForbidActionID = 2,
		},
		{	.ActionType = SIGNAL_ACTION,
			.ActionID = 5,
			.ActionName = "��ƽ��",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = DISABLE,
			.ActionKey = 6,
			.ForbidActionType = SIGNAL_ACTION,
			.ForbidActionID = 6,
		},
		{	.ActionType = SIGNAL_ACTION,
			.ActionID = 6,
			.ActionName = "��ƽ��",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = DISABLE,
			.ActionKey = 5,
			.ForbidActionType = SIGNAL_ACTION,
			.ForbidActionID = 5,
		},
		{	.ActionType = SIGNAL_ACTION,
			.ActionID = 7,
			.ActionName = "�ղ໤",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = DISABLE,
			.ActionKey = 18,
			.ForbidActionType = 1,
			.ForbidActionID = 8,
		},
		{	.ActionType = SIGNAL_ACTION,
			.ActionID = 8,
			.ActionName = "��໤",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = DISABLE,
			.ActionKey = 17,
			.ForbidActionType = SIGNAL_ACTION,
			.ForbidActionID = 7,
		},
		{	.ActionType = SIGNAL_ACTION,
			.ActionID = 10,
			.ActionName = "̧����",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = DISABLE,
			.ActionKey = 11,
			.ForbidActionType = SIGNAL_ACTION,
			.ForbidActionID = 0,
		},
		{	.ActionType = SIGNAL_ACTION,
			.ActionID = 9,
			.ActionName = "����",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = ENABLE,
			.ActionKey = 12,
			.ForbidActionType = SIGNAL_ACTION,
			.ForbidActionID = 0,
		},
		{	.ActionType = SIGNAL_ACTION,
			.ActionID = 11,
			.ActionName = "��������",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = DISABLE,
			.ActionKey = 10,
			.ForbidActionType = SIGNAL_ACTION,
			.ForbidActionID = 0,
		},
		{	.ActionType = SIGNAL_ACTION,
			.ActionID = 12,
			.ActionName = "��������",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = DISABLE,
			.ActionKey = 9,
			.ForbidActionType = SIGNAL_ACTION,
			.ForbidActionID = 0,
		},
		{	.ActionType = SIGNAL_ACTION,
			.ActionID = 13,
			.ActionName = "�ջ���",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = DISABLE,
			.ActionKey = 8,
			.ForbidActionType = SIGNAL_ACTION,
			.ForbidActionID = 14,
		},
		{	.ActionType = SIGNAL_ACTION,
			.ActionID = 14,
			.ActionName = "�커��",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = DISABLE,
			.ActionKey = 7,
			.ForbidActionType = SIGNAL_ACTION,
			.ForbidActionID = 13,
		},		
		{	.ActionType = SIGNAL_ACTION,
			.ActionID = 15,
			.ActionName = "�յ׵�",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = DISABLE,
			.ActionKey = 19,
			.ForbidActionType = SIGNAL_ACTION,
			.ForbidActionID = 0,
		},
		{	.ActionType = SIGNAL_ACTION,
			.ActionID = 16,
			.ActionName = "��׵�",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = DISABLE,
			.ActionKey = 20,
			.ForbidActionType = SIGNAL_ACTION,
			.ForbidActionID = 0,
		},
		{	.ActionType = SIGNAL_ACTION,
			.ActionID = 17,
			.ActionName = "����1",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = DISABLE,
			.ActionKey = 21,
			.ForbidActionType = SIGNAL_ACTION,
			.ForbidActionID = 0,
		},
		{	.ActionType = SIGNAL_ACTION,
			.ActionID = 18,
			.ActionName = "����2",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = DISABLE,
			.ActionKey = 22,
			.ForbidActionType = SIGNAL_ACTION,
			.ForbidActionID = 0,
		},
		{	.ActionType = SIGNAL_ACTION,
			.ActionID = 22,
			.ActionName = "����",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = DISABLE,
			.ActionKey = 16,
			.ForbidActionType = SIGNAL_ACTION,
			.ForbidActionID = 0,
		},
		{	.ActionType = COMBINATION_ACTION,
			.ActionID = 1,
			.ActionName = "�Զ��Ƽ�",
			.MannulCtrlMode = DISABLE,
			.AutoCtrlMode = ENABLE,
			.ActionKey = 4,
			.ForbidActionType = COMBINATION_ACTION,
			.ForbidActionID = 0,
		},
		{	.ActionType = COMBINATION_ACTION,
			.ActionID = 2,
			.ActionName = "�Զ���ѹ",
			.MannulCtrlMode = DISABLE,
			.AutoCtrlMode = ENABLE,
			.ActionKey = 1,
			.ForbidActionType = COMBINATION_ACTION,
			.ForbidActionID = 0,
		},
		{	.ActionType = COMBINATION_ACTION,
			.ActionID = 3,
			.ActionName = "������",
			.MannulCtrlMode = DISABLE,
			.AutoCtrlMode = ENABLE,
			.ActionKey = 8,
			.ForbidActionType = COMBINATION_ACTION,
			.ForbidActionID = 0,
		},		
		{	.ActionType = COMBINATION_ACTION,
			.ActionID = 4,
			.ActionName = "������",
			.MannulCtrlMode = DISABLE,
			.AutoCtrlMode = ENABLE,
			.ActionKey = 7,
			.ForbidActionType = COMBINATION_ACTION,
			.ForbidActionID = 0,
		},
		{	.ActionType = COMBINATION_ACTION,
			.ActionID = 5,
			.ActionName = "�Զ�����ϴ",
			.MannulCtrlMode = DISABLE,
			.AutoCtrlMode = ENABLE,
			.ActionKey = 21,
			.ForbidActionType = COMBINATION_ACTION,
			.ForbidActionID = 0,
		},
		{	.ActionType = GROUP_ACTION,
			.ActionID = 2,
			.ActionName = "�����Ƽ�",
			.MannulCtrlMode = DISABLE,
			.AutoCtrlMode = ENABLE,
			.ActionKey = 4,
			.ForbidActionType = GROUP_ACTION,
			.ForbidActionID = 0,
		},
		{	.ActionType = GROUP_ACTION,
			.ActionID = 1,
			.ActionName = "��������",
			.MannulCtrlMode = DISABLE,
			.AutoCtrlMode = ENABLE,
			.ActionKey = 3,
			.ForbidActionType = GROUP_ACTION,
			.ForbidActionID = 0,
		},
		{	.ActionType = GROUP_ACTION,
			.ActionID = 3,
			.ActionName = "��������",
			.MannulCtrlMode = DISABLE,
			.AutoCtrlMode = ENABLE,
			.ActionKey = 16,
			.ForbidActionType = GROUP_ACTION,
			.ForbidActionID = 0,
		},
		{	.ActionType = GROUP_ACTION,
			.ActionID = 4,
			.ActionName = "��������",
			.MannulCtrlMode = DISABLE,
			.AutoCtrlMode = ENABLE,
			.ActionKey = 12,
			.ForbidActionType = GROUP_ACTION,
			.ForbidActionID = 0,
		},
		{	.ActionType = GROUP_ACTION,
			.ActionID = 5,
			.ActionName = "����������",
			.MannulCtrlMode = DISABLE,
			.AutoCtrlMode = ENABLE,
			.ActionKey = 10,
			.ForbidActionType = GROUP_ACTION,
			.ForbidActionID = 0,
		},
		{	.ActionType = GROUP_ACTION,
			.ActionID = 6,
			.ActionName = "����������",
			.MannulCtrlMode = DISABLE,
			.AutoCtrlMode = ENABLE,
			.ActionKey = 9,
			.ForbidActionType = GROUP_ACTION,
			.ForbidActionID = 0,
		},
		{	.ActionType = GROUP_ACTION,
			.ActionID = 7,
			.ActionName = "���鲹ѹ",
			.MannulCtrlMode = DISABLE,
			.AutoCtrlMode = ENABLE,
			.ActionKey = 1,
			.ForbidActionType = GROUP_ACTION,
			.ForbidActionID = 0,
		},
		{	.ActionType = GROUP_ACTION,
			.ActionID = 8,
			.ActionName = "�����ջ���",
			.MannulCtrlMode = DISABLE,
			.AutoCtrlMode = ENABLE,
			.ActionKey = 8,
			.ForbidActionType = GROUP_ACTION,
			.ForbidActionID = 0,
		},
		{	.ActionType = GROUP_ACTION,
			.ActionID = 9,
			.ActionName = "�����커��",
			.MannulCtrlMode = DISABLE,
			.AutoCtrlMode = ENABLE,
			.ActionKey = 7,
			.ForbidActionType = GROUP_ACTION,
			.ForbidActionID = 0,
		},
		{	.ActionType = GROUP_ACTION,
			.ActionID = 10,
			.ActionName = "����������",
			.MannulCtrlMode = DISABLE,
			.AutoCtrlMode = ENABLE,
			.ActionKey = 19,
			.ForbidActionType = GROUP_ACTION,
			.ForbidActionID = 0,
		},
		{	.ActionType = GROUP_ACTION,
			.ActionID = 11,
			.ActionName = "����������",
			.MannulCtrlMode = DISABLE,
			.AutoCtrlMode = ENABLE,
			.ActionKey = 20,
			.ForbidActionType = GROUP_ACTION,
			.ForbidActionID = 0,
		},
	},
};

#else

#endif






static void Wireless_SC_Decode_Proc(void);//�����������������
static void Wireless_Action_Ack_Proc(uint8_t actionType,uint8_t antionID);
static uint8_t GetWirelessHeadMsg(uint16_t frameType ,WIRELESSPROHEAD * head);

//��ȡ����ID
uint8_t getSupportID(void)
{
	uint16_t len=0;
	uint8_t* pstate=NULL;
	globalParam_SupportType prar;

	SysDataFun(DATA_MSG_READ_OBJECT_PARAM,ENUM_SYSTEM,globalParamEnum_Support,0,0,&pstate,&len);
	memcpy((uint8_t*)&prar,pstate,len);
	return prar.currentSupportID;
}

//��ȡ�ܺ�����
uint8_t getSupportDirect(void)
{
	uint16_t len=0;
	uint8_t* pstate=NULL;
	globalParam_SupportType prar;

	SysDataFun(DATA_MSG_READ_OBJECT_PARAM,ENUM_SYSTEM,globalParamEnum_Support,0,0,&pstate,&len);
	memcpy((uint8_t*)&prar,pstate,len);
	return prar.SupportDirect;
}

//��ȡ����ʱ��
uint8_t getMasterCtrlTime(void)
{
	uint16_t len=0;
	uint8_t* pstate=NULL;
	globalParam_SupportType prar;

	SysDataFun(DATA_MSG_READ_OBJECT_PARAM,ENUM_SYSTEM,globalParamEnum_Support,0,0,&pstate,&len);
	memcpy((uint8_t*)&prar,pstate,len);
	return prar.MasterCtrlTime;
}

//��ȡ���ܼ���
uint8_t getGapSupportNum(void)
{
	uint16_t len=0;
	uint8_t* pstate=NULL;
	globalParam_SupportType prar;

	SysDataFun(DATA_MSG_READ_OBJECT_PARAM,ENUM_SYSTEM,globalParamEnum_Support,0,0,&pstate,&len);
	memcpy((uint8_t*)&prar,pstate,len);
	return prar.GapSupportNum;
}



void YkqCtrlAlarmProc(u8 state)
{
	u8 aa = 1;
	
	if(state == 0)
		CtrlAlarm(AlarmType_SoundLight,0,BaseCtrlType_STARTUP,&aa,3,2,1);
	else
		CtrlAlarm(AlarmType_SoundLight,0,BaseCtrlType_STOP,&aa,3,2,1);
}




void ykqControllAlarmProc(uint8_t num)
{
	uint8_t alar[2];
	uint8_t scNum;
	scNum = getSupportID();
	if(V4ykq.controllNum != num)
	{
		alar[0] = 0;
		alar[1] = num;				
		SysDataFun(DATA_MSG_CTRL_ALARM,0,0,2,0,alar,0);
		
		if(scNum != V4ykq.controllNum)
		{
			alar[0] = 1;
			alar[1] = V4ykq.controllNum;				
			SysDataFun(DATA_MSG_CTRL_ALARM,0,0,2,0,alar,0);
		}
		V4ykq.controllNum = num;
	}
}


u8 getJtState(void)
{
	u16 i;
	u8 j = 0;
	for(i = 0;i<256;i++)
	{
		if(SysStopState[i].SYSStop.HWEMERGENCY == 1)
			j++;
	}
	return j;
}

uint8_t getBsState(void)
{
	u16 i;
	u8 j = 0;
	u8  num = getSupportID();
	
	for(i = num-1;i<=num+1;i++)
	{
		if(SysStopState[i].SYSStop.BISUO == 1)
			j++;
	}
	return j;
}


//��ȡ��ͣ����״̬
uint8_t getJtBsTzState(void)
{
	globalStateType*  temp111;
	uint16_t len=0;
	SysDataFun(DATA_MSG_READ_OBJECT,ENUM_SYSTEM,0,0,0,&temp111,&len);

	if(getJtState() != V4ykq.hwemergencyNum)
		return 1;
	if(getBsState() != V4ykq.bisuoNum)
		return 1;

	if(temp111->stopState == 1 || temp111->hwStop == 1)
	{
		if(YkqStopFlag)
			return 0;
		else
			return 1;
	}
	else
	{
		if(YkqStopFlag)
		{
//			YkqCtrlAlarmProc(0);
			YkqStopFlag = 0;
		}
	}
	return 0;
}


//��ȡ���߰汾��
uint8_t *getWirelessVersion(void)
{
	return WirelessVersion;
}
//��ȡ��������״̬
uint8_t getWirelssOnline(void)
{
	return WirelessOnline;
}

/*******************************************************************************************
**�������ƣ�LoadV4YkqPara
**�������ã�����ң���������ļ�
**������������
**�����������
**ע�������
*******************************************************************************************/
void LoadV4YkqPara(void)
{
	char ykqName[30] = "0:/update/ykqpara.bin";
	u16 OldCrc = 0;
	u16 fileCrc = 0;
	uint32_t len;
	
	
	//��ȡ�ļ�	
	V4ykqParaSize = Fatfs_openFile(ykqName,0);
	if(V4ykqParaSize == 0)
	{
		V4ykqParaSize = ActionInformation.Head.TotalAction*sizeof(ActionMess)+sizeof(ActionInfoHead);
		memcpy(V4ykqParaQueue,&ActionInformation,V4ykqParaSize);
		return;
	}
	Fatfs_readFile(ykqName,V4ykqParaQueue,V4ykqParaSize);
	Fatfs_closeFile();
	
	Crc16Ccitt(V4ykqParaQueue+sizeof(ActionInfoHead),sizeof(ActionMess)*V4ykqParaQueue[5],&OldCrc);
	fileCrc = (u16)(V4ykqParaQueue[7]<<8|V4ykqParaQueue[6]);
	if(OldCrc != fileCrc)
	{
		V4ykqParaSize = ActionInformation.Head.TotalAction*sizeof(ActionMess)+sizeof(ActionInfoHead);
		memcpy(V4ykqParaQueue,&ActionInformation,V4ykqParaSize);
		return;
	}
//	Crc16Ccitt((const u8 *)(&ActionInformation.Body),sizeof(ActionInformation.Body[0])*ActionInformation.Head.TotalAction,&OldCrc);
}




/*******************************************************************************************
**�������ƣ�WirelessUniversal_Callback
**�������ã�ͨ�ö�ʱ����ʹ��
**������������
**�����������
**ע�������
*******************************************************************************************/
void WirelessUniversal_Callback(uint16_t id)
{
	uint16_t u16Way;
	uint16_t tmp = 10;
	if(WLOnlineTimer != TIMERCLOSE && WLOnlineTimer != 0)
		WLOnlineTimer -= 20;
	if(BusDataProcTimer != TIMERCLOSE && BusDataProcTimer != 0)
		BusDataProcTimer -= 10;
	if(YkqManualSingleTimer != TIMERCLOSE && YkqManualSingleTimer != 0)
		YkqManualSingleTimer -= 20;	
	if(YkqIrTimer != TIMERCLOSE && YkqIrTimer != 0)
		YkqIrTimer -= 20;
	if(YkqStopStateTimer != TIMERCLOSE && YkqStopStateTimer != 0)
		YkqStopStateTimer -= 20;
	if(YkqDecodeTimer != TIMERCLOSE && YkqDecodeTimer != 0)
		YkqDecodeTimer -= 2;

	//usart�����߳�ʱ�ص�
	if(WLOnlineTimer == 0)
	{
		memset(&WirelessVersion,0,sizeof(WirelessVersion));
	}
	if(BusDataProcTimer == 0)
	{
		BusDataProcTimer = BUSDATAPROCTIMER;
		for(u16Way = 0; u16Way<USART_DRIVER_DEV_NUM; u16Way++)
		{
			UsartBusDealFunc(u16Way,USARTBUS_MSG_CheckUsartSentInterval,&tmp);
		}
	}
	if(YkqManualSingleTimer == 0)
	{
		//ֹͣ���ж���
		memset(ManualActionClear,0,MANUALACTIONINFO_LEN);
	}
	if(YkqIrTimer == 0)
	{
		V4ykq.YkqState = STATE_IDLE;
		V4ykq.YkqNum = 0;
		V4ykq.YkqCodingMode = 0;
		V4ykq.controllNum = 0;
		StateLed(0,LED_MATCH_BUSY|LED_MATCH_IR);
	}
	if(YkqStopStateTimer == 0)
	{
		if(V4ykq.YkqState != STATE_WL_SUCCESS)
		{
			YkqStopStateTimer = TIMERCLOSE;
			return;
		}
		
		if(getJtBsTzState())
		{
			Wireless_SC_Decode_Proc();
			YkqStopStateTimer = TIMERCLOSE;
		}	
		else
		{
			YkqStopStateTimer = YKQSTOPSTATETIMER;		
		}
	}
	if(YkqDecodeTimer == 0)
	{
		Wireless_SC_Decode_Proc();
		YkqDecodeTimer = TIMERCLOSE;
	}
}

/*******************************************************************************************
**�������ƣ�Wireless_Heart_Proc
**�������ã�����ģ����������
**������������
**�����������
**ע�������
*******************************************************************************************/
uint16_t Wireless_Heart_Proc(STR_WLCAN_Frame* frame)
{

	//��ȡ���ܼܺ�
	uint8_t scNum;
	uint8_t i,j = 0;
	scNum = getSupportID();
	if(frame->u8DT[0] != scNum)
		return 0;
	if(frame->u8DT[1] != V4ykq.YkqNum)
		return 0;

	manualActionID = frame->u8DT[3]|(frame->u8DT[4]<<8)|(frame->u8DT[5]<<16)|(frame->u8DT[6]<<24);
	if(manualActionID == 0)
	{
		YkqManualSingleTimer  = TIMERCLOSE;
		if(V4ykq.controllNum == scNum)
			memset(ManualActionClear,0,MANUALACTIONINFO_LEN);
		if(NeighborManualActionBitNumber != 0)
		{
			for(i = 0; i<32;i++)
			{
				if(((NeighborManualActionBitNumber>>i)&0x01) == 0x01)
				{
					SysNetCtrlFun(V4ykq.controllNum,ENUM_SINGLE_ACTION,i,ActionCtrl_CmdType_STOP,ActionCtrl_ModeType_MANUAL,0,NULL);					
				}
			}
			NeighborManualActionBitNumber = 0;
		}
	}
	else
	{
		if(V4ykq.controllNum == scNum)
		{
			YkqManualSingleTimer  = YKQMANUALSINGLETIMER;
			memset(ManualActionNumber,0,10);		
			for(i = 0; i<32;i++)
			{
				if(((manualActionID>>i)&0x01) == 0x01)
				{
					if(j>10)
						break;
					ManualActionNumber[j] = i+1;
					j++;
				}
			}		
			memset(ManualActionClear,0,MANUALACTIONINFO_LEN);
			memcpy(ManualActionClear,ManualActionNumber,10);
		}
		else
		{
			manualActionID = (manualActionID<<1);
			if(manualActionID != NeighborManualActionBitNumber)
			{
				if(NeighborManualActionBitNumber == 0)
					return 1;
				if(((NeighborManualActionBitNumber-1)&NeighborManualActionBitNumber) == 0)
					return 1;
				if(manualActionID>NeighborManualActionBitNumber)
					return 1;
					
				NeighborManualActionBitNumber = NeighborManualActionBitNumber- manualActionID;
				for(i = 0; i<32;i++)
				{
					if(((NeighborManualActionBitNumber>>i)&0x01) == 0x01)
					{
						SysNetCtrlFun(V4ykq.controllNum,ENUM_SINGLE_ACTION,i,ActionCtrl_CmdType_STOP,ActionCtrl_ModeType_MANUAL,0,NULL);
					}
				}
			}
		}
	}

	return 1;
}
/*******************************************************************************************
**�������ƣ�IR_CodeMatch_Proc
**�������ã�������ն���
**������������
**�����������
**ע�������
*******************************************************************************************/
void IR_CodeMatching_Ack_Proc(uint8_t coderes)
{
	//��ȡ���ܼܺ�
	uint8_t scNum;
	//��ȡ����ʱ��
	uint16_t scactiveTimer;

	
	EXT_CAN_FRAME_s frame = {0};
	
	scNum = getSupportID();
	scactiveTimer = getMasterCtrlTime();
	
	frame.u32ID.ID.ACK = 0;
	frame.u32ID.ID.FT = V4YKQ_IR_CODEMATCH_ACK;
	frame.u32ID.ID.RID = 3;
	frame.u16DLC = 8;
	frame.u8DT[0] = V4ykq.YkqKey;
	frame.u8DT[1] = getSupportDirect();
	frame.u8DT[2] = coderes;
	frame.u8DT[3] = scNum;
	frame.u8DT[4] = 0;
	frame.u8DT[5] = V4ykq.YkqNum;
	frame.u8DT[6] = scactiveTimer>>8;
	frame.u8DT[7] = scactiveTimer&0xFF;
	
	IR_EmitProc(&frame);
}


/*******************************************************************************************
**�������ƣ�IR_CodeMatch_Proc
**�������ã�������ն���
**������������
**�����������
**ע�������
*******************************************************************************************/
uint16_t IR_CodeMatching_Proc(STR_WLCAN_Frame* frame)
{
	EXT_CAN_FRAME_s frametmp;
	
	if(SysParam.globalParam.DeviceInfirredParam.InfirredCodeEnable == EnableType_DISABLE)
		return 0;
	memcpy(&frametmp,frame,sizeof(frametmp));
	if(V4ykq.YkqState == STATE_IDLE)
	{
		V4ykq.YkqState = STATE_IR_CODEMATCHING;
		V4ykq.YkqNum = frame->u8DT[5];
	}
	if(V4ykq.YkqState == STATE_WL_SUCCESS)
	{
		if(V4ykq.YkqNum != frame->u8DT[5])
			return 0;
	}
	V4ykq.YkqNum = frametmp.u8DT[5];
	StateLed(LED_MATCH_BUSY,0);
	IR_CodeMatching_Ack_Proc(1);
	YkqIrTimer = IROVERTIMER;
	return 0;
}




/*******************************************************************************************
**�������ƣ�Wireless_CodeMatching_Proc
**�������ã��ӵ���������
**������������
**�����������
**ע�������
*******************************************************************************************/
uint16_t Wireless_CodeMatching_Proc(STR_WLCAN_Frame* frame)
{
	//��ȡ���ܼܺ�
	uint8_t scNum;
	uint8_t alar[2];
	scNum = getSupportID();
	uint8_t decodetmp[5] = {0};
	if(frame->u8DT[4] != scNum)
		return 0;
//	if((frame->u8DT[2]&0x80) != 0x80)//����V4ң����
//		return 0;
	if((frame->u8DT[2]&0x40) != 0x40)//����Ǻ������
	{
		if(V4ykq.YkqState != STATE_IR_CODEMATCHING)
		{
			if(V4ykq.YkqNum != frame->u8DT[5])
				return 0;
		}
	}
	else//����ǡ�������
	{
		if(V4ykq.YkqState == STATE_WL_SUCCESS)
		{
			if(V4ykq.YkqNum != frame->u8DT[5])
				return 0;
		}
		else
			V4ykq.YkqNum = frame->u8DT[5];
	}
		
	//����crc
	V4ykq.YkqCrc = (frame->u8DT[7]<<8)|frame->u8DT[6];
	
	V4ykq.YkqKey = frame->u8DT[0];
	V4ykq.YkqNum = frame->u8DT[5];
	V4ykq.YkqState = STATE_WL_SUCCESS;
	V4ykq.controllNum = scNum;
	V4ykq.hwemergencyNum = getJtState();
	V4ykq.bisuoNum = getBsState();
	V4ykq.YkqCodingMode = ((frame->u8DT[2]&0x80) == 0x40)?0:1;
	Wireless_CodeMatching_Ack_Proc(1);
	StateLed(LED_MATCH_IR,LED_MATCH_BUSY);
	

	
	YkqStopStateTimer = YKQSTOPSTATETIMER;
	
	decodetmp[0] = V4ykq.YkqNum;
	decodetmp[1] = scNum;
	decodetmp[2] = SCNET_DECODE_RANGE;//range
	
	UdpAppProcCtrl(AppProcCtrl_ykqDecode_request,decodetmp,5,255);
	
	AlarmAllStop();
	OSTimeDly(50);
	YkqCtrlAlarmProc(0);
	
	return 1;
}


/*******************************************************************************************
**�������ƣ�InsSendWirelessDataProc
**�������ã��������뷢�Ͷ���
**������������
**�����������
**ע�������
*******************************************************************************************/
void InsSendWirelessDataProc(STR_WLCAN_Frame* frame,uint8_t addr)
{
	uint8_t res;
	uint16_t crctmp = 0;
	stUsartTrsQueue queue;
	WIRELESSPROHEAD head;
	
	queue.UsartData.ACK = 0;
	queue.UsartData.Serial = YkqLiushuiNumber;
	YkqLiushuiNumber++;
	YkqLiushuiNumber %= 0x0F;
	queue.UsartData.Address = 0;
	queue.UsartData.FrameType = WIRELESS_DATA;
	queue.UsartData.u8DLC = sizeof(STR_WLCAN_Frame)+1;
	queue.UsartData.SendBuf[0] = 0xFF - addr;
	memcpy(&queue.UsartData.SendBuf[1],frame,sizeof(STR_WLCAN_Frame));
	
	Crc16Ccitt(&queue.UsartData.Address,queue.UsartData.u8DLC+5,&crctmp);
	
	queue.UsartData.CRCHigh = crctmp>>8;
	queue.UsartData.CRCLow = crctmp&0xFF;
	
	res = GetWirelessHeadMsg(frame->u32ID.ID.FrameType,&head);
	if(res == 0)
		return;
	queue.busHead.ack = head.ack;
	queue.busHead.backtime = head.overtime;
	queue.busHead.overtime = head.overtime;
	queue.busHead.count = head.count;
	
	UsartBusDealFunc(0,USARTBUS_MSG_InsUsartTrsQueue,&queue);
}

/*******************************************************************************************
**�������ƣ�Wireless_CodeMatching_Ack_Proc
**�������ã����߶�����Ӧ��
**������������
**�����������
**ע�������
*******************************************************************************************/
void Wireless_CodeMatching_Ack_Proc(uint8_t state)
{
	//��ȡ���ܼܺ�
	uint8_t scNum;
	//��ȡ����ʱ��
	uint16_t scactiveTimer;
	
	STR_WLCAN_Frame frame = {0};
	
	scNum = getSupportID();
	scactiveTimer = getMasterCtrlTime();
	frame.u32ID.ID.ACK = 0;
	frame.u32ID.ID.FrameType = V4YKQ_WL_CODEMATCH_ACK;
	frame.u32ID.ID.LiuShuiNumb = YkqLiushuiNumber;
	YkqLiushuiNumber++;
	YkqLiushuiNumber %= 0x0F;
	frame.u32ID.ID.RxID = 3;
	frame.u16DLC = 8;
	frame.u8DT[0] = V4ykq.YkqKey;
	frame.u8DT[1] = getSupportDirect();
	frame.u8DT[2] = V4ykq.YkqCodingMode == 0?0x00:0x40;
	frame.u8DT[2] |= 0x80;
	frame.u8DT[2] |= state;
	V4ykq.YkqSCCrc = (V4ykqParaQueue[7]<<8|V4ykqParaQueue[6]);
//	frame.u8DT[3] = V4ykq.YkqSCCrc == V4ykq.YkqCrc?1:0;
	frame.u8DT[3] =1;
	frame.u8DT[4] = scNum;
	frame.u8DT[5] = V4ykq.YkqNum;
	frame.u8DT[6] = scactiveTimer>>8;
	frame.u8DT[7] = scactiveTimer&0xFF;
	if(SysParam.globalParam.SupportParam.Paring_Method ==0) //���뷽ʽΪ����ʱ
		InsSendWirelessDataProc(&frame,V4ykq.YkqNum);
	else if(SysParam.globalParam.SupportParam.Paring_Method ==1)//���뷽ʽΪCANʱ
		InsSendWirelessAlarmDataProc(&frame,V4ykq.YkqNum);
	YkqIrTimer = TIMERCLOSE;
	YkqDecodeTimer = YKQDECODETIMER;
}

/*******************************************************************************************
**�������ƣ�Wireless_SC_Decode_Proc
**�������ã������������������
**������������
**�����������
**ע�������
*******************************************************************************************/
static void Wireless_SC_Decode_Proc(void)
{
	//��ȡ���ܼܺ�
	uint8_t scNum;
	uint8_t alar[2];
	STR_WLCAN_Frame frame = {0};
	
	if(V4ykq.YkqNum == 0)
		return;
	scNum = getSupportID();
	frame.u32ID.ID.ACK = 0;
	frame.u32ID.ID.FrameType = V4YKQ_ACTIVE_DECODE;
	frame.u32ID.ID.LiuShuiNumb = YkqLiushuiNumber;
	YkqLiushuiNumber++;
	YkqLiushuiNumber &= 0x0F;
	frame.u32ID.ID.RxID = 3;
	frame.u16DLC = 8;
	frame.u8DT[0] = 0;
	frame.u8DT[1] = 0;
	frame.u8DT[2] = 0;
	frame.u8DT[3] = scNum;
	frame.u8DT[4] = V4ykq.YkqNum;
	if(SysParam.globalParam.SupportParam.Paring_Method ==0)//���뷽ʽѡ��Ϊ����ʱ
		frame.u8DT[5] = 0;
	else if(SysParam.globalParam.SupportParam.Paring_Method ==1)//���뷽ʽѡ��ΪCANʱ
		frame.u8DT[5] = V4ykq.YkqNum;
	frame.u8DT[6] = 0;
	frame.u8DT[7] = 0;

	if(SysParam.globalParam.SupportParam.Paring_Method ==0)//���뷽ʽѡ��Ϊ����ʱ
		InsSendWirelessDataProc(&frame,V4ykq.YkqNum);
	else if(SysParam.globalParam.SupportParam.Paring_Method ==1)//���뷽ʽѡ��ΪCANʱ
	    InsSendWirelessAlarmDataProc(&frame,V4ykq.YkqNum);
	V4ykq.YkqNum = 0;
	V4ykq.YkqState = STATE_IDLE;
	StateLed(0,LED_MATCH_BUSY|LED_MATCH_IR);
	
	YkqCtrlAlarmProc(1);
	if(V4ykq.controllNum != scNum)
	{
		alar[0] = 1;
		alar[1] = V4ykq.controllNum;				
		SysDataFun(DATA_MSG_CTRL_ALARM,0,0,2,0,alar,0);
	}
	V4ykq.controllNum = 0;
}

u8 Wireless_SCnet_Decode_proc(u8 *pdata)
{
		//��ȡ���ܼܺ�
	uint8_t scNum;
	scNum = getSupportID();
	uint8_t alar[2];
	if((*(pdata+1))> scNum)
	{
		if((*(pdata+1)) > (scNum + SCNET_DECODE_RANGE))
			return 0;
	}
	else
	{
		if(((*(pdata+1))+ SCNET_DECODE_RANGE) < scNum )
			return 0;
	}
	if(*pdata != V4ykq.YkqNum)
		return 0;
	if(V4ykq.YkqState != STATE_WL_SUCCESS)
		return 0;
	
	YkqCtrlAlarmProc(1);
	if(V4ykq.controllNum != scNum)
	{
		alar[0] = 1;
		alar[1] = V4ykq.controllNum;				
		SysDataFun(DATA_MSG_CTRL_ALARM,0,0,2,0,alar,0);
	}
	
	V4ykq.YkqNum = 0;
	V4ykq.controllNum = 0;
	V4ykq.YkqState = STATE_IDLE;
	StateLed(0,LED_MATCH_BUSY|LED_MATCH_IR);
	return 1;
}

	

/*******************************************************************************************
**�������ƣ�Wireless_Ykq_Decode_Proc
**�������ã�ң���������������
**������������
**�����������
**ע�������
*******************************************************************************************/
uint16_t Wireless_Ykq_Decode_Proc(STR_WLCAN_Frame* frame)
{
	//��ȡ���ܼܺ�
	uint8_t scNum;
	uint8_t alar[2];
	scNum = getSupportID();
	if(frame->u8DT[3] != scNum)
		return 0;
	if(frame->u8DT[4] != V4ykq.YkqNum)
		return 0;
	if(V4ykq.YkqState != STATE_WL_SUCCESS)
		return 0;
	

	if(V4ykq.controllNum != scNum)
	{
		alar[0] = 1;
		alar[1] = V4ykq.controllNum;				
		SysDataFun(DATA_MSG_CTRL_ALARM,0,0,2,0,alar,0);
	}
	V4ykq.YkqNum = 0;
	V4ykq.controllNum = 0;
	V4ykq.YkqState = STATE_IDLE;
	StateLed(0,LED_MATCH_BUSY|LED_MATCH_IR);
	YkqCtrlAlarmProc(1);
	return 1;
}

void ManualActionHeart_Proc(uint8_t actionId,uint8_t status,uint8_t addr)
{
	uint32_t numtmp = 1;
	if(status == ActionCtrl_CmdType_STOP)
	{
		if((ManualActionBitNumber & (numtmp<<actionId))== (numtmp<<actionId))
			ManualActionBitNumber -= (numtmp<<actionId);
	}
	else if(status == ActionCtrl_CmdType_STARTUP)
	{
		if(ManualActionBitNumber == 0)
			YkqManualSingleTimer = YKQMANUALSINGLETIMER;
		ManualActionBitNumber |= (numtmp<<actionId);
	}
	if(ManualActionBitNumber == 0)
	{
		YkqManualSingleTimer  = TIMERCLOSE;
		memset(ManualActionClear,0,MANUALACTIONINFO_LEN);
		
		YkqCtrlAlarmProc(0);
	}
}



/*******************************************************************************************
**�������ƣ�Wireless_Action_Proc
**�������ã��������ƴ���
**������������
**�����������
**ע�������
*******************************************************************************************/
uint16_t Wireless_Action_Proc(STR_WLCAN_Frame* frame)
{
	//��ȡ���ܼܺ�
	uint8_t pstate = 1;
	uint8_t scNum;
	uint8_t supportDirect;
	uint8_t ManualOrAuto;
	uint8_t buf[7] = {0};
	uint32_t numtmp = 1;
	scNum = getSupportID();
//	if(frame->u8DT[1] != scNum)
//		return 0;
	if(frame->u8DT[0] != V4ykq.YkqNum)
		return 0;
	if(V4ykq.YkqState != STATE_WL_SUCCESS)
		return 0;
	lastLiushuiNum = frame->u32ID.ID.LiuShuiNumb;
	
	//frame->u8DT[2]//bit2�ֶ�/�Զ�
	//frame->u8DT[3]//�����豸ID
	//frame->u8DT[4]//����������
	//frame->u8DT[5]//��ʼ�ܺ�
	//frame->u8DT[6]//�����ܺ�
	//frame->u8DT[7]//ִ�з���
	YkqDecodeTimer = YKQDECODETIMER;
	
	switch(frame->u8DT[2]&0X07)
	{
		case SINGLE_CONTROL:

			if((frame->u8DT[2]&0x08) == 0x08)
				buf[1] = 1;//�Զ�
			else
				buf[1] = 0;//�ֶ�
			buf[0] = frame->u8DT[4];

				
			if(frame->u8DT[1] == scNum)
			{			
				if(buf[1] == 0)
				{
					ManualActionHeart_Proc(frame->u8DT[3],frame->u8DT[4],frame->u8DT[1]);
				}
				
				buf[3] = V4ykq.YkqNum;
				buf[4] = ActionCtrl_SourceType_HANDLER;
				buf[5] = 0;
				buf[6] = 0;
				SysDataFun(DATA_MSG_CTRL,ENUM_SINGLE_ACTION,frame->u8DT[3],7,0,buf,0);
			}
			else
			{
//				ykqControllAlarmProc(frame->u8DT[1]);
				if(buf[1] == 0)
				{
					if(frame->u8DT[4] == ActionCtrl_CmdType_STOP)
					{
						if((NeighborManualActionBitNumber & (numtmp<<frame->u8DT[3]))== (numtmp<<frame->u8DT[3]))
							NeighborManualActionBitNumber -= (numtmp<<frame->u8DT[3]);
					}
					else if(frame->u8DT[4] == ActionCtrl_CmdType_STARTUP)
					{
						NeighborManualActionBitNumber |= (numtmp<<frame->u8DT[3]);
					}
					SysNetCtrlFun(frame->u8DT[1],ENUM_SINGLE_ACTION,frame->u8DT[3],frame->u8DT[4],buf[1],0,NULL);
				}
				else
					SysNetCtrlFun(frame->u8DT[1],ENUM_SINGLE_ACTION,frame->u8DT[3],frame->u8DT[4],buf[1],0,NULL);
			}
			break;
		case COMB_CONTROL:
			if((frame->u8DT[2]&0x08) == 0x08)
				buf[1] = 1;
			else
				buf[1] = 0;
			buf[0] = frame->u8DT[4];
			if(frame->u8DT[1] == scNum)
			{
				buf[3] = V4ykq.YkqNum;
				buf[4] = ActionCtrl_SourceType_HANDLER;
				buf[5] = 0;
				buf[6] = 0;
				SysDataFun(DATA_MSG_CTRL,ENUM_COMBINATION_ACTION,frame->u8DT[3],7,0,buf,0);
			}
			else
				SysNetCtrlFun(frame->u8DT[1],ENUM_COMBINATION_ACTION,frame->u8DT[3],frame->u8DT[4],buf[1],0,NULL);
			break;
		case GROUP_CONTROL:
			
			supportDirect = getSupportDirect();
			if((frame->u8DT[2]&0x08) == 0x08)
				ManualOrAuto = 1;
			else
				ManualOrAuto = 0;
			if(supportDirect == 0)//��
			{
				if(frame->u8DT[7] == 0)//��->Զ
				{
					if(frame->u8DT[5]>scNum)
					{
						buf[2] = 0;
						buf[3] = 0;
					}
					else
					{
						buf[2] = 1;
						buf[3] = 1;
					}
				}
				else//��<-Զ
				{
					if(frame->u8DT[5]>scNum)
					{
						buf[2] = 1;
						buf[3] = 0;
					}
					else
					{
						buf[2] = 0;
						buf[3] = 1;
					}
				}
			}	
			else
			{
				if(frame->u8DT[7] == 0)//��->Զ
				{
					if(frame->u8DT[5]>scNum)
					{
						buf[2] = 1;
						buf[3] = 1;
					}
					else
					{
						buf[2] = 0;
						buf[3] = 0;
					}
				}
				else//��<-Զ
				{
					if(frame->u8DT[5]>scNum)
					{
						buf[2] = 0;
						buf[3] = 1;
					}
					else
					{
						buf[2] = 1;
						buf[3] = 0;
					}
				}
			}
			buf[0] = frame->u8DT[5];
			buf[1] = frame->u8DT[6];
			SysNetCtrlFun(0X00,ENUM_GROUP_ACTION,frame->u8DT[3],0x02,ManualOrAuto,4,buf);
			break;
		case FUNCTION_KEY:
			YkqStopFlag = 1;
			SysDataFun(DATA_MSG_SetStopPress,ENUM_SYSTEM,ActionCtrl_SourceType_HANDLER,0,0,&pstate,0);
			break;
	}
	Wireless_Action_Ack_Proc(frame->u8DT[2],frame->u8DT[3]);
	return 1;
}


/*******************************************************************************************
**�������ƣ�Wireless_Action_Ack_Proc
**�������ã��������ƴ���
**������������
**�����������
**ע�������
*******************************************************************************************/
static void Wireless_Action_Ack_Proc(uint8_t actionType,uint8_t antionID)
{
		//��ȡ���ܼܺ�
	uint8_t scNum;
	STR_WLCAN_Frame frame = {0};
	
	if(V4ykq.YkqNum == 0)
		return;
	scNum = getSupportID();
	frame.u32ID.ID.ACK = 0;
	frame.u32ID.ID.FrameType = V4YKQ_CONTROL_DATA_ACK;
	frame.u32ID.ID.LiuShuiNumb = YkqLiushuiNumber;
	YkqLiushuiNumber++;
	YkqLiushuiNumber %= 0x0F;
	frame.u32ID.ID.RxID = 3;
	frame.u16DLC = 8;
	frame.u8DT[0] = V4ykq.YkqNum;
	frame.u8DT[1] = scNum;
	frame.u8DT[2] = actionType;
	frame.u8DT[3] = antionID;
	frame.u8DT[4] = 1;
	frame.u8DT[5] = 0;
	if(SysParam.globalParam.SupportParam.Paring_Method ==0) //���뷽ʽΪ����ʱ		
		frame.u8DT[6] = 0;
	else if(SysParam.globalParam.SupportParam.Paring_Method ==1)//���뷽ʽΪCANʱ
		frame.u8DT[6] = V4ykq.YkqNum;
	frame.u8DT[7] = 0;
	
	if(SysParam.globalParam.SupportParam.Paring_Method ==0) //���뷽ʽΪ����ʱ
		InsSendWirelessDataProc(&frame,V4ykq.YkqNum);
	else if(SysParam.globalParam.SupportParam.Paring_Method ==1)//���뷽ʽΪCANʱ
		InsSendWirelessAlarmDataProc(&frame,V4ykq.YkqNum);
}



/*******************************************************************************************
**�������ƣ�sendYkqParaProc
**�������ã�����ң��������
**������������
**�����������
**ע�������
*******************************************************************************************/
void SendYkqParaProc(void)
{
#ifdef USE_V4YKQ
	stUsartTrsQueue queue;
	RFID_UPDATE_FRAME_TYPE updateFrame;
	uint16_t paraPt = 0;
	uint16_t crctmp = 0;
	uint32_t ProgSendPacketNumb = 0;
	
	OSTimeDly(1000);
			
	while(paraPt < V4ykqParaSize)
	{
		ProgSendPacketNumb++;
		
		memset(&queue,0,sizeof(queue));
		memset(&updateFrame,0,sizeof(RFID_UPDATE_FRAME_TYPE));
		
		updateFrame.u8DT[1] = ProgSendPacketNumb&0xFF;
		updateFrame.u8DT[2] = (ProgSendPacketNumb>>8)&0xFF;
	
		if(paraPt==0)
		{
			queue.busHead.ack = 0;
			queue.busHead.count = 2;
			queue.busHead.backtime = 50;
			queue.busHead.overtime = 50;
			
			queue.UsartData.ACK = 0;
			queue.UsartData.Address = 0;
			queue.UsartData.FrameType = WIRELESS_DATA;
			
			queue.UsartData.Serial = YkqLiushuiNumber;
			YkqLiushuiNumber++;
			YkqLiushuiNumber &= 0x0F;

			updateFrame.u32ID.ID.SDL = YKQ_DEV_TYPE&0xFF;
			updateFrame.u32ID.ID.Reservd = 0;
			updateFrame.u8DT[0] = 0xFF - V4ykq.YkqNum;			
			updateFrame.u32ID.ID.FrameType = RFID_FT_UPDATE_DWL_VERSION;
			updateFrame.u8DLC = 32+3;			
			memcpy(&updateFrame.u8DT[3],V4ykqParaQueue,32);

			queue.UsartData.u8DLC = updateFrame.u8DLC+4+1+1;			
			queue.UsartData.SendBuf[0] = 0xFF - V4ykq.YkqNum;			
			memcpy(&queue.UsartData.SendBuf[1],&updateFrame,queue.UsartData.u8DLC);
			
			crctmp = 0;
			Crc16Ccitt(&queue.UsartData.Address,queue.UsartData.u8DLC+5,&crctmp);	
			queue.UsartData.CRCHigh = (crctmp>>8)&0xFF;
			queue.UsartData.CRCLow = crctmp&0xFF;
		
			paraPt = 32;
			
			UsartBusDealFunc(0,USARTBUS_MSG_InsUsartTrsQueue,&queue);
			OSTimeDly(500);	

			updateFrame.u32ID.ID.FrameType = RFID_FT_UPDATE_WRITE_VERSION;
			updateFrame.u8DT[3] = 1;
			updateFrame.u8DLC  = 4;
			
			queue.UsartData.SendBuf[0] = 0xFF - V4ykq.YkqNum;
			queue.UsartData.u8DLC = updateFrame.u8DLC+4+1+1;
			memcpy(&queue.UsartData.SendBuf[1],&updateFrame, queue.UsartData.u8DLC-1);
			queue.UsartData.Serial = YkqLiushuiNumber;
			YkqLiushuiNumber++;
			YkqLiushuiNumber &= 0x0F;
			
			crctmp = 0;
			Crc16Ccitt(&queue.UsartData.Address,queue.UsartData.u8DLC+5,&crctmp);
			queue.UsartData.CRCHigh =(crctmp>>8)&0xFF;
			queue.UsartData.CRCLow = crctmp&0xFF;

			UsartBusDealFunc(0,USARTBUS_MSG_InsUsartTrsQueue,&queue);
			OSTimeDly(500);			
		}
		else
		{
			queue.busHead.ack = 0;
			queue.busHead.count = 2;
			queue.busHead.backtime = 50;
			queue.busHead.overtime = 50;
			
			queue.UsartData.ACK = 0;
			queue.UsartData.Address = 0;
			queue.UsartData.FrameType = WIRELESS_DATA;
			queue.UsartData.Serial = YkqLiushuiNumber;
			YkqLiushuiNumber++;
			YkqLiushuiNumber &= 0x0F;
			
			updateFrame.u32ID.ID.SDL = YKQ_DEV_TYPE&0xFF;
			updateFrame.u32ID.ID.Reservd = 0;
			updateFrame.u32ID.ID.FrameType = RFID_FT_UPDATE_DWL_CODE;
			updateFrame.u8DT[0] = 0xFF - V4ykq.YkqNum;	
			
			if(V4ykqParaSize > 32 + paraPt)
			{
				memcpy(&updateFrame.u8DT[3],V4ykqParaQueue+paraPt, 32);
				updateFrame.u8DLC = 32+3;
				paraPt+= 32;
			}
			else
			{
				memcpy(&updateFrame.u8DT[3],V4ykqParaQueue+paraPt, V4ykqParaSize - paraPt);
				updateFrame.u8DLC = V4ykqParaSize - paraPt+3;
				paraPt = V4ykqParaSize;
			}
			queue.UsartData.SendBuf[0] = 0xFF - V4ykq.YkqNum;
			queue.UsartData.u8DLC = updateFrame.u8DLC+4+1+1;
			memcpy(&queue.UsartData.SendBuf[1],&updateFrame,queue.UsartData.u8DLC-1);
	
			crctmp = 0;
			Crc16Ccitt(&queue.UsartData.Address,queue.UsartData.u8DLC+5,&crctmp);
			queue.UsartData.CRCHigh =(crctmp>>8)&0xFF;
			queue.UsartData.CRCLow = crctmp&0xFF;
			
			UsartBusDealFunc(0,USARTBUS_MSG_InsUsartTrsQueue,&queue);
			OSTimeDly(200);
			
			if((paraPt-32)%256 == 0 || paraPt == V4ykqParaSize)
			{
				queue.UsartData.Serial = YkqLiushuiNumber;
				YkqLiushuiNumber++;
				YkqLiushuiNumber &= 0x0F;
				updateFrame.u32ID.ID.FrameType = RFID_FT_UPDATE_WRITE_CODE;
				updateFrame.u8DLC = 4;
				updateFrame.u8DT[3] = 1;
				
				queue.UsartData.SendBuf[0] = 0xFF - V4ykq.YkqNum;
				queue.UsartData.u8DLC = updateFrame.u8DLC+4+1+1;
				memcpy(&queue.UsartData.SendBuf[1],&updateFrame,queue.UsartData.u8DLC-1);
				
				crctmp = 0;
				Crc16Ccitt(&queue.UsartData.Address,queue.UsartData.u8DLC+5,&crctmp);
				queue.UsartData.CRCHigh = (crctmp>>8)&0xFF;
				queue.UsartData.CRCLow = crctmp&0xFF;
				
				UsartBusDealFunc(0,USARTBUS_MSG_InsUsartTrsQueue,&queue);
				OSTimeDly(200);	
			}
		}
	}
	queue.UsartData.Serial = YkqLiushuiNumber;
	YkqLiushuiNumber++;
	YkqLiushuiNumber &= 0x0F;
	updateFrame.u32ID.ID.FrameType = RFID_FT_UPDATE_END;
	updateFrame.u8DLC = 4;
	queue.UsartData.u8DLC = updateFrame.u8DLC+4+1+1;
	queue.UsartData.SendBuf[0] = 0xFF - V4ykq.YkqNum;
	memcpy(&queue.UsartData.SendBuf[1],&updateFrame,queue.UsartData.u8DLC-1);
	crctmp = 0;
	Crc16Ccitt(&queue.UsartData.Address,queue.UsartData.u8DLC+5,&crctmp);
	queue.UsartData.CRCHigh = (crctmp>>8)&0xFF;
	queue.UsartData.CRCLow = crctmp&0xFF;		
	UsartBusDealFunc(0,USARTBUS_MSG_InsUsartTrsQueue,&queue);
	OSTimeDly(200);	
#endif
}

/*******************************************************************************************
**�������ƣ�SendAlarm_YkqParaProc
**�������ã��������Ϸ���ң��������
**������������
**�����������
**ע�������
*******************************************************************************************/
void SendAlarm_YkqParaProc(void)
{
	return;
}
/*******************************************************************************************
**�������ƣ�Wireless_Para_Request_Proc
**�������ã�ң�����ļ�����
**������������
**�����������
**ע�������
*******************************************************************************************/
uint16_t Wireless_Para_Request_Proc(STR_WLCAN_Frame* frame)
{
		//��ȡ���ܼܺ�
	uint8_t scNum;
	scNum = getSupportID();
//	if(lastLiushuiNum == frame->u32ID.ID.LiuShuiNumb)
//		return 0;
	if(frame->u8DT[1] != scNum)
		return 0;
	if(frame->u8DT[0] != V4ykq.YkqNum)
		return 0;
	if(V4ykq.YkqState != STATE_WL_SUCCESS)
		return 0;	
	lastLiushuiNum = frame->u32ID.ID.LiuShuiNumb;
	
	if(SysParam.globalParam.SupportParam.Paring_Method == 0)//�����뷽ʽѡ��Ϊ����ʱ
		OSMboxPost(revProgramSem,(void*)SENDYKQPARASEM);
	else if(SysParam.globalParam.SupportParam.Paring_Method ==1)//�����뷽ʽѡ��ΪCANʱ
		OSMboxPost(revProgramSem,(void*)SENDALARMYKQPARASEM);	
	return 1;
}

uint16_t Wireless_Bus_Request_Ack_Proc(STR_WLCAN_Frame* frame)
{
	if(SysParam.globalParam.DeviceWirelessParam.WirelessEnable == EnableType_ENABLE)
		DeviceMngDataFun(DEVDATA_MSG_STATE_EMERGENCY,frame->u8DT[1],0,frame->u8DT[0],0,0,NULL);
	return 1;
}

void Wireless_Neighbor_ack_Proc(void)
{
		//��ȡ���ܼܺ�
	uint8_t scNum;
	STR_WLCAN_Frame frame = {0};
	
	if(V4ykq.YkqNum == 0)
		return;
	scNum = getSupportID();
	frame.u32ID.ID.ACK = 0;
	frame.u32ID.ID.FrameType = V4YKQ_NEIGHBOR_REQUEST_ACK;
	frame.u32ID.ID.LiuShuiNumb = YkqLiushuiNumber;
	YkqLiushuiNumber++;
	YkqLiushuiNumber %= 0x0F;
	frame.u32ID.ID.RxID = 3;
	frame.u16DLC = 3;
	frame.u8DT[0] = V4ykq.YkqNum;
	frame.u8DT[1] = scNum;
	frame.u8DT[2] = V4ykq.controllNum;
	frame.u8DT[3] = 0;
	frame.u8DT[4] = 0;
	frame.u8DT[5] = 0;
	frame.u8DT[6] = 0;
	frame.u8DT[7] = 0;	
	
	if(SysParam.globalParam.SupportParam.Paring_Method ==0) //�����뷽ʽѡ��Ϊ����ʱ
		InsSendWirelessDataProc(&frame,V4ykq.YkqNum);
	else if(SysParam.globalParam.SupportParam.Paring_Method ==1)//�����뷽ʽѡ��ΪCANʱ
		InsSendWirelessAlarmDataProc(&frame,V4ykq.YkqNum);
}

uint16_t Wireless_Neighbor_Request_Proc(STR_WLCAN_Frame* frame)
{
			//��ȡ���ܼܺ�
	uint8_t scNum;
	uint8_t gapNum;
	uint8_t conNum = 0;
	scNum = getSupportID();	
	gapNum = getGapSupportNum()+1;
	

	if(frame->u8DT[1] != scNum)
		return 0;
	if(frame->u8DT[0] != V4ykq.YkqNum)
		return 0;
	if(V4ykq.YkqState != STATE_WL_SUCCESS)
		return 0;
	
	if(scNum > frame->u8DT[2])
	{
		if(scNum-frame->u8DT[2] > gapNum)
		{
			conNum = scNum-gapNum;
		}
		else
		{
			conNum = frame->u8DT[2];
		}
	}
	else
	{
		if(frame->u8DT[2] - scNum > gapNum)
		{
			conNum = scNum+gapNum;
		}
		else
		{
			conNum = frame->u8DT[2];
		}
	}
	if(conNum != V4ykq.controllNum)
	{
		ykqControllAlarmProc(conNum);
	}
	Wireless_Neighbor_ack_Proc();
}



WIRELESSPROHEAD v4_wireless[] = 
{
	/*ң����*/
	//����
	{
		0,       //֡����
		0,       //��ʱʱ��
		0,       //���ʹ���
		0,       //Ӧ��
		Wireless_Heart_Proc,     //���ջص�����
		NULL,     
		NULL,     
		NULL,    
	},
	//ң�����������
	{
		4,       //֡����
		0,       //��ʱʱ��
		0,       //���ʹ���
		0,       //Ӧ��
		IR_CodeMatching_Proc,    //���ջص�����
		NULL,     
		NULL,     
		NULL,    
	},
	//ң�������������
	{
		5,       //֡����
		0,       //��ʱʱ��
		0,       //���ʹ���
		0,       //Ӧ��
		NULL,    //���ջص�����
		NULL,     
		NULL,     
		NULL,    
	},
	//ң�������߶���
	{
		6,       //֡����
		0,       //��ʱʱ��
		0,       //���ʹ���
		0,       //Ӧ��
		Wireless_CodeMatching_Proc,     
		NULL,     
		NULL,     
		NULL,    //����ǰ����ص�����
	},
	//ң�������߶�����
	{
		7,       //֡����
		0,       //��ʱʱ��
		0,       //���ʹ���
		0,       //Ӧ��
		NULL,     
		NULL,     
		NULL,     
		NULL,    //����ǰ����ص�����
	},
	//�����������������
	{
		14,       //֡����
		60,       //��ʱʱ��
		3,       //���ʹ���
		0,       //Ӧ��
		NULL,     
		NULL,     
		NULL,     
		NULL,    //����ǰ����ص�����
	},
	//ң���������������Ӧ��
	{
		6,       //֡����
		0,       //��ʱʱ��
		0,       //���ʹ���
		0,       //Ӧ��
		NULL,     
		NULL,     
		NULL,     
		NULL,    //����ǰ����ص�����
	},
	//ң���������������
	{
		15,       //֡����
		0,       //��ʱʱ��
		0,       //���ʹ���
		0,       //Ӧ��
		Wireless_Ykq_Decode_Proc,     //���ջص�����
		NULL,     
		NULL,     
		NULL,    
	},
	//ң�����������������
	{
		8,       //֡����
		60,       //��ʱʱ��
		3,       //���ʹ���
		0,       //Ӧ��
		NULL,     
		NULL,     
		NULL,     
		NULL,    //����ǰ����ص�����
	},
	//��������
	{
		10,       //֡����
		0,       //��ʱʱ��
		0,       //���ʹ���
		0,       //Ӧ��
		Wireless_Action_Proc,     //���ջص�����
		NULL,     
		NULL,     
		NULL,    
	},
	//��������Ӧ��
	{
		11,       //֡����
		30,       //��ʱʱ��
		3,       //���ʹ���
		0,       //Ӧ��
		NULL,     //���ջص�����
		NULL,     
		NULL,     
		NULL,    
	},
	//��״̬
	{
		11,       //֡����
		0,       //��ʱʱ��
		0,       //���ʹ���
		0,       //Ӧ��
		NULL,     //���ջص�����
		NULL,     
		NULL,     
		NULL,    
	},
	//��״̬Ӧ��
	{
		12,       //֡����
		0,       //��ʱʱ��
		0,       //���ʹ���
		0,       //Ӧ��
		NULL,     //���ջص�����
		NULL,     
		NULL,     
		NULL,    
	},
	//������ȡ����
	{
		13,      //֡����
		0,       //��ʱʱ��
		0,       //���ʹ���
		0,       //Ӧ��
		NULL,    //���ջص�����
		NULL,     
		NULL,     
		NULL,    
	},
	//������ȡ����Ӧ��
	{
		14,      //֡����
		0,       //��ʱʱ��
		0,       //���ʹ���
		0,       //Ӧ��
		NULL,    //���ջص�����
		NULL,     
		NULL,     
		NULL,    
	},
	//����д������
	{
		15,      //֡����
		0,       //��ʱʱ��
		0,       //���ʹ���
		0,       //Ӧ��
		NULL,    //���ջص�����
		NULL,     
		NULL,     
		NULL,    
	},
	//����д������Ӧ��
	{
		16,      //֡����
		0,       //��ʱʱ��
		0,       //���ʹ���
		0,       //Ӧ��
		NULL,    //���ջص�����
		NULL,     
		NULL,     
		NULL,    
	},
	//ң�����ļ�����
	{
		18,      //֡����
		0,       //��ʱʱ��
		0,       //���ʹ���
		0,       //Ӧ��
		Wireless_Para_Request_Proc,     //���ջص�����
		NULL,     
		NULL,     
		NULL,    
	},
	//ң�����ڼ�
	{
		19,      //֡����
		0,       //��ʱʱ��
		0,       //���ʹ���
		0,       //Ӧ��
		Wireless_Neighbor_Request_Proc,     //���ջص�����
		NULL,
		NULL,
		NULL,
	},
	//ң�����ڼ�Ӧ��
	{
		20,      //֡����
		0,       //��ʱʱ��
		0,       //���ʹ���
		0,       //Ӧ��
		NULL,    //���ջص�����
		NULL,
		NULL,
		NULL,
	},
	
	
	//��������
	{
		100,     //֡����
		0,       //��ʱʱ��
		0,       //���ʹ���
		0,       //Ӧ��
		Wireless_Bus_Request_Ack_Proc,     //���ջص�����
		NULL,     
		NULL,     
		NULL,    
	},
};


/*******************************************************************************************
**�������ƣ�WirelessSendProData
**�������ã����ͳ������֡
**������������
**�����������
**ע�������
*******************************************************************************************/
void WirelessSendProData(u8* pFrame,u8 len)
{
#ifdef USE_V4YKQ
	uint16_t crctmp = 0;
	stUsartTrsQueue queue;
	memset(&queue.UsartData,0x00,sizeof(queue.UsartData));
	queue.UsartData.FrameType = WIRELESS_UPDATE;//3
	queue.UsartData.ACK = 1;
	memcpy(queue.UsartData.SendBuf,pFrame,len);
	queue.UsartData.SendBuf[len] = len;
	queue.UsartData.u8DLC = len+1;
	queue.busHead.ack = 0;
	queue.busHead.backtime = 0;
	queue.busHead.count = 0;
	queue.busHead.overtime = 0;
	queue.UsartData.Serial = YkqLiushuiNumber;
	YkqLiushuiNumber++;
	YkqLiushuiNumber %= 0xFF;
	queue.UsartData.Address = 0;
	
	Crc16Ccitt(&queue.UsartData.Address,queue.UsartData.u8DLC+5,&crctmp);
	
	queue.UsartData.CRCLow = crctmp&0xFF;
	
	queue.UsartData.CRCHigh = (crctmp>>8)&0xFF;
	
	
	UsartBusDealFunc(0,USARTBUS_MSG_InsUsartTrsQueue,&queue);
	
	OSTimeDly(100);
#endif
}


/*******************************************************************************************
**�������ƣ�GetWirelessHeadMsg
**�������ã��õ���������proЭ��
**������������
**�����������
**ע�������
*******************************************************************************************/
static uint8_t GetWirelessHeadMsg(uint16_t frameType ,WIRELESSPROHEAD * head)
{
	uint8_t i;
	uint8_t res = 0;
	for(i = 0 ;i < sizeof(v4_wireless)/sizeof(WIRELESSPROHEAD); i++)
	{
		if(v4_wireless[i].frametype == frameType)
		{
			memcpy(head,&(v4_wireless[i]),sizeof(WIRELESSPROHEAD));
			res = 1;
			break;
		}
	}
	return res;
}

/*******************************************************************************************
**�������ƣ�UsartRecvWorkDataProc
**�������ã�usart���ջص�����
**������������
**�����������
**ע�������
*******************************************************************************************/
void UsartRecvWorkDataProc(STR_WLCAN_Frame *candata)
{
	uint8_t res;
	WIRELESSPROHEAD head;
	res = GetWirelessHeadMsg(candata->u32ID.ID.FrameType,&head);
	if(res == 0)
		return;
	if(head.rececallbacfun != NULL)
		head.rececallbacfun(candata);
}

/*******************************************************************************************
**�������ƣ�UsartRecvHeartDataProc
**�������ã�usart����������
**������������
**�����������
**ע�������
*******************************************************************************************/
void UsartRecvHeartDataProc(uint8_t *candata)
{
	if(SysParam.globalParam.SupportParam.Paring_Method == 0) //���뷽ʽѡ��Ϊ������ʱ
	{
		WirelessVersion[0] = candata[5];
		WirelessVersion[1] = candata[6];
		WirelessVersion[2] = candata[7];
	}
	else if(SysParam.globalParam.SupportParam.Paring_Method == 1)//���뷽ʽѡ��ΪCANʱ
	{
		if(candata[5] == 'W')
		{
			WirelessVersion[0] = candata[10];
			WirelessVersion[1] = candata[11];
			WirelessVersion[2] = candata[12];
		}
	}

}



/********************************************************************************
* ���������� ���ͱ��ܼܺ�
* ��ڲ�����
* ����ֵ����
********************************************************************************/
void SendSCAddressData(void)
{
#ifdef USE_V4YKQ
	uint16_t crctmp = 0;
	stUsartTrsQueue queue;
	memset(&queue.UsartData,0x00,sizeof(queue.UsartData));
	queue.UsartData.FrameType = WIRELESS_PARASET;//4
	queue.UsartData.ACK = 1;
	queue.UsartData.SendBuf[0] = getSupportID();
	
	queue.UsartData.SendBuf[1] = 0x33;
	queue.UsartData.SendBuf[2] = 0x33;
	queue.UsartData.u8DLC = 3;
	queue.UsartData.Address = 0;
	queue.UsartData.Serial = YkqLiushuiNumber;
	YkqLiushuiNumber++;
	YkqLiushuiNumber %= 0xFF;
	Crc16Ccitt(&queue.UsartData.Address,queue.UsartData.u8DLC+5,&crctmp);
	
	queue.UsartData.CRCLow = crctmp&0xFF;
	queue.UsartData.CRCHigh = crctmp>>8;
	queue.busHead.ack = 0;
	queue.busHead.backtime = 0;
	queue.busHead.count = 0;
	queue.busHead.overtime = 0;
	
	UsartBusDealFunc(0,USARTBUS_MSG_InsUsartTrsQueue,&queue);
	
	if(V4ykq.YkqState == STATE_WL_SUCCESS)
	{
		Wireless_CodeMatching_Ack_Proc(1);
	}
#endif
}


/********************************************************************************
* ���������� ��������
* ��ڲ�����
* ����ֵ����
********************************************************************************/
void SendWirelessResart(void)
{
#ifdef USE_V4YKQ
	u16 data = 1;
	uint16_t crctmp = 0;
	stUsartTrsQueue queue;
	memset(&queue,0x00,sizeof(stUsartTrsQueue));
	queue.UsartData.FrameType = WIRELESS_RESTORATION;
	queue.UsartData.ACK = 1;
	queue.UsartData.u8DLC = 4;
	queue.UsartData.SendBuf[0] = (u8)( data & 0x000000FF);
	queue.UsartData.SendBuf[1] = (u8)((data & 0x0000FF00) >> 8);
	queue.UsartData.SendBuf[2] = (u8)((data & 0x00FF0000) >> 16);
	queue.UsartData.SendBuf[3] = (u8)((data & 0xFF000000) >> 24);
	
	queue.UsartData.Address = 0;
	queue.UsartData.Serial = YkqLiushuiNumber;
	YkqLiushuiNumber++;
	YkqLiushuiNumber %= 0xFF;
	Crc16Ccitt(&queue.UsartData.Address,queue.UsartData.u8DLC+5,&crctmp);
	
	queue.UsartData.CRCLow = crctmp&0xFF;
	queue.UsartData.CRCHigh = crctmp>>8;
	queue.busHead.ack = 0;
	queue.busHead.backtime = 0;
	queue.busHead.count = 0;
	queue.busHead.overtime = 0;
	
	UsartBusDealFunc(0,USARTBUS_MSG_InsUsartTrsQueue,&queue);
#endif
}

/********************************************************************************
* ���������� ��������
* ��ڲ�����
* ����ֵ����
********************************************************************************/
void SendWirelessBusData(uint8_t state,uint8_t id)
{
#ifdef USE_V4YKQ
	uint16_t crctmp = 0;
	stUsartTrsQueue queue;
	STR_WLCAN_Frame frame;
	memset(&queue,0x00,sizeof(stUsartTrsQueue));
	queue.UsartData.FrameType = WIRELESS_DATA;
	queue.UsartData.ACK = 1;

	frame.u32ID.ID.ACK = 0;
	frame.u32ID.ID.FrameType = V4WIRELESS_BUS_REQUEST;
	frame.u32ID.ID.LiuShuiNumb = YkqLiushuiNumber;
	YkqLiushuiNumber++;
	YkqLiushuiNumber %= 0xFF;
	frame.u32ID.ID.NoUsed = 0;
	frame.u32ID.ID.Reservd = 0;
	frame.u32ID.ID.RxID = 0;
	frame.u32ID.ID.Sub = 0;
	frame.u32ID.ID.Sum = 0;
	frame.u32ID.ID.TxID = 0;
	frame.u8DT[0] = state;
	frame.u8DT[1] = id;
	frame.u16DLC = 2;
	
	queue.UsartData.SendBuf[0] = 0;
	queue.UsartData.u8DLC = 2+4+1+1;
	memcpy(&queue.UsartData.SendBuf[1],&frame,queue.UsartData.u8DLC);
	
	queue.UsartData.Address = 0;
	queue.UsartData.Serial = YkqLiushuiNumber;
	YkqLiushuiNumber++;
	YkqLiushuiNumber %= 0xFF;
	Crc16Ccitt(&queue.UsartData.Address,queue.UsartData.u8DLC+5,&crctmp);
	
	queue.UsartData.CRCLow = crctmp&0xFF;
	queue.UsartData.CRCHigh = crctmp>>8;
	queue.busHead.ack = 0;
	queue.busHead.backtime = 0;
	queue.busHead.count = 0;
	queue.busHead.overtime = 0;	
	UsartBusDealFunc(0,USARTBUS_MSG_InsUsartTrsQueue,&queue);
#endif
}

static u8 ykqledState = 0;

void UsartRecDealProc(uint8_t u8Way, STR_Usart_Frame* frame)
{	
	u8 scNum = getSupportID();
	WirelessOnline = 1;
	WLOnlineTimer = WLONLINETIMER;
	if(V4ykq.YkqState == STATE_IDLE && ykqledState ==1)
	{
		StateLed(0,LED_MATCH_BUSY);
		ykqledState = 0;
	}
	switch(frame->FrameType)
	{
		case WIRELESS_HEART:
			UsartRecvHeartDataProc(frame->SendBuf);
			if(scNum != V4ykq.SCNum)
			{   
				if(SysParam.globalParam.SupportParam.Paring_Method == 0) //���뷽ʽѡ��Ϊ������ʱ
					SendSCAddressData();
				else if(SysParam.globalParam.SupportParam.Paring_Method == 1)//���뷽ʽѡ��ΪCANʱ
					Alarm_SendSCAddressData();
				V4ykq.SCNum = scNum;
			}
			break;
		case WIRELESS_DATA:
			if(ykqledState)
			{
				StateLed(LED_MATCH_BUSY,0);
				ykqledState = 0;
			}
			else
			{
				StateLed(0,LED_MATCH_BUSY);
				ykqledState = 1;
			}
			UsartRecvWorkDataProc((STR_WLCAN_Frame*)frame->SendBuf);

			break;
		case WIRELESS_STARTHINT:			
		   if(SysParam.globalParam.SupportParam.Paring_Method == 0) //���뷽ʽѡ��Ϊ������ʱ
				SendSCAddressData();
		   else if(SysParam.globalParam.SupportParam.Paring_Method == 1)//���뷽ʽѡ��ΪCANʱ
				Alarm_SendSCAddressData();
			break;
	}
}


void InsUsartIrRecvData_proc(STR_WLCAN_Frame * data,uint8_t len)
{
	STR_Usart_Frame frame;
	frame.FrameType = WIRELESS_DATA;
	memcpy(frame.SendBuf,data,len);
	InsUsartRevQueue(0,&frame);
}




void Wireless_Pro_Init(void)
{
	#ifdef USE_V4YKQ
	UsartBusInit(0,115200);	
	IR_RxUartInit();
	IR_EmitInit();
	IR_RxCmd(ENABLE);
	#endif

}

void Wireless_task_Init(void)
{
//	SendWirelessResart();
	
	WirelessUniversalTimerId = addTimerEvent(20,0XFF,WirelessUniversal_Callback);//20ms
	startTimerEvent(WirelessUniversalTimerId);
	BusDataProcTimer = BUSDATAPROCTIMER;
	WLOnlineTimer = WLONLINETIMER;
	
	LoadV4YkqPara();
	if(SysParam.globalParam.SupportParam.Paring_Method ==0) //����ģ��
		SendSCAddressData();
	else if(SysParam.globalParam.SupportParam.Paring_Method ==1)//����433				
		Alarm_SendSCAddressData();		
	V4ykq.SCNum = getSupportID();
}

//uint16_t OldCrc = 0;

//��������
void UsartRec_task(void)
{
	INT8U err;
	uint8_t u8Way;
	STR_Usart_Frame RevBufTemp;
	
	Wireless_task_Init();
		
//	Crc16Ccitt((const u8 *)(&ActionInformation.Body),sizeof(ActionInformation.Body[0])*ActionInformation.Head.TotalAction,&OldCrc);
	
	while(1)
	{
		u8Way = ((uint8_t)OSMboxPend(UsartRTxTaskSendSem,0,&err));
		if(err == OS_ERR_NONE)
		{
			u8Way = (u8Way&0x0F);
			while(UsartBusDealFunc(u8Way,USARTBUS_MSG_GetUsartRevQueue,&RevBufTemp))
			{
				UsartRecDealProc(u8Way, &RevBufTemp);
			}
		}
	}
}



