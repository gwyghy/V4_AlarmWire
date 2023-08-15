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

uint16_t WLOnlineTimer = TIMERCLOSE;//无线模块在线状态
uint16_t BusDataProcTimer = TIMERCLOSE;//bus层数据处理
uint16_t YkqManualSingleTimer = TIMERCLOSE;//单动作时间
uint16_t YkqIrTimer = TIMERCLOSE;//红外对码成功
uint16_t YkqStopStateTimer = TIMERCLOSE;//遥控器停停止状态
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

//配置文件主体
__packed typedef struct
{
	u8 ActionType;			//动作类型
	u8 ActionID;			//动作ID
	u8 ActionName[12];		//动作名
	u8 MannulCtrlMode;		//手动控制
	u8 AutoCtrlMode;		//自动动作
	u8 ActionKey;			//按键值
	u8 ForbidActionType;		//禁止动作类型
	u8 ForbidActionID;		//禁止动作id
}ActionMess;
//配置文件头部
__packed typedef struct
{
	u8 Veron[4];		//版本信息
	u8 ApplyScene;		//应用场景
	u8 TotalAction;		//动作个数
	u16 Verify;		//校验
	u8  Nouse[24];
}ActionInfoHead;
//配置文件信息
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


//综放
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
			.ActionName = "升前柱",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = DISABLE,
			.ActionKey = 1,
			.ForbidActionType = SIGNAL_ACTION,
			.ForbidActionID = 1,
		},
		{	.ActionType = SIGNAL_ACTION,
			.ActionID = 1,
			.ActionName = "降前柱",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = DISABLE,
			.ActionKey = 2,
			.ForbidActionType = SIGNAL_ACTION,
			.ForbidActionID = 1,
		},
		{	.ActionType = SIGNAL_ACTION,
			.ActionID = 12,
			.ActionName = "升后柱",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = DISABLE,
			.ActionKey = 3,
			.ForbidActionType = SIGNAL_ACTION,
			.ForbidActionID = 11,
		},
		{	.ActionType = SIGNAL_ACTION,
			.ActionID = 11,
			.ActionName = "降后柱",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = DISABLE,
			.ActionKey = 4,
			.ForbidActionType = SIGNAL_ACTION,
			.ForbidActionID = 12,
		},
		{	.ActionType = SIGNAL_ACTION,
			.ActionID = 3,
			.ActionName = "移架",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = DISABLE,
			.ActionKey = 5,
			.ForbidActionType = SIGNAL_ACTION,
			.ForbidActionID = 4,
		},
		{	.ActionType = SIGNAL_ACTION,
			.ActionID = 4,
			.ActionName = "推溜",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = ENABLE,
			.ActionKey = 6,
			.ForbidActionType = SIGNAL_ACTION,
			.ForbidActionID = 3,
		},
		{	.ActionType = SIGNAL_ACTION,
			.ActionID = 10,
			.ActionName = "伸护帮",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = DISABLE,
			.ActionKey = 7,
			.ForbidActionType = SIGNAL_ACTION,
			.ForbidActionID = 9,
		},
		{	.ActionType = SIGNAL_ACTION,
			.ActionID = 9,
			.ActionName = "收护帮",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = DISABLE,
			.ActionKey = 8,
			.ForbidActionType = SIGNAL_ACTION,
			.ForbidActionID = 10,
		},
		{	.ActionType = SIGNAL_ACTION,
			.ActionID = 8,
			.ActionName = "伸伸缩",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = DISABLE,
			.ActionKey = 9,
			.ForbidActionType = SIGNAL_ACTION,
			.ForbidActionID = 7,
		},
		{	.ActionType = SIGNAL_ACTION,
			.ActionID = 7,
			.ActionName = "收伸缩",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = DISABLE,
			.ActionKey = 10,
			.ForbidActionType = SIGNAL_ACTION,
			.ForbidActionID = 8,
		},
		{	.ActionType = SIGNAL_ACTION,
			.ActionID = 28,
			.ActionName = "拉后溜",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = DISABLE,
			.ActionKey = 11,
			.ForbidActionType = SIGNAL_ACTION,
			.ForbidActionID = 0,
		},
		{	.ActionType = SIGNAL_ACTION,
			.ActionID = 5,
			.ActionName = "抬底",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = DISABLE,
			.ActionKey = 12,
			.ForbidActionType = SIGNAL_ACTION,
			.ForbidActionID = 0,
		},
		{	.ActionType = SIGNAL_ACTION,
			.ActionID = 18,
			.ActionName = "伸尾梁",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = DISABLE,
			.ActionKey = 13,
			.ForbidActionType = SIGNAL_ACTION,
			.ForbidActionID = 17,
		},
		{	.ActionType = SIGNAL_ACTION,
			.ActionID = 17,
			.ActionName = "收尾梁",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = DISABLE,
			.ActionKey = 14,
			.ForbidActionType = SIGNAL_ACTION,
			.ForbidActionID = 18,
		},
		{	.ActionType = SIGNAL_ACTION,
			.ActionID = 14,
			.ActionName = "伸前梁",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = DISABLE,
			.ActionKey = 15,
			.ForbidActionType = SIGNAL_ACTION,
			.ForbidActionID = 13,
		},
		{	.ActionType = SIGNAL_ACTION,
			.ActionID = 13,
			.ActionName = "收前梁",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = DISABLE,
			.ActionKey = 16,
			.ForbidActionType = SIGNAL_ACTION,
			.ForbidActionID = 14,
		},
		{	.ActionType = SIGNAL_ACTION,
			.ActionID = 20,
			.ActionName = "伸插板",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = DISABLE,
			.ActionKey = 17,
			.ForbidActionType = SIGNAL_ACTION,
			.ForbidActionID = 19,
		},
		{	.ActionType = SIGNAL_ACTION,
			.ActionID = 19,
			.ActionName = "收插板",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = DISABLE,
			.ActionKey = 18,
			.ForbidActionType = SIGNAL_ACTION,
			.ForbidActionID = 20,
		},
		{	.ActionType = SIGNAL_ACTION,
			.ActionID = 16,
			.ActionName = "伸顶侧",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = DISABLE,
			.ActionKey = 19,
			.ForbidActionType = SIGNAL_ACTION,
			.ForbidActionID = 15,
		},
		{	.ActionType = SIGNAL_ACTION,
			.ActionID = 15,
			.ActionName = "收顶侧",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = DISABLE,
			.ActionKey = 20,
			.ForbidActionType = SIGNAL_ACTION,
			.ForbidActionID = 16,
		},
		{	.ActionType = SIGNAL_ACTION,
			.ActionID = 24,
			.ActionName = "伸掩侧",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = DISABLE,
			.ActionKey = 21,
			.ForbidActionType = SIGNAL_ACTION,
			.ForbidActionID = 23,
		},		
		{	.ActionType = SIGNAL_ACTION,
			.ActionID = 23,
			.ActionName = "收掩侧",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = DISABLE,
			.ActionKey = 22,
			.ForbidActionType = SIGNAL_ACTION,
			.ForbidActionID = 24,
		},
		{	.ActionType = SIGNAL_ACTION,
			.ActionID = 27,
			.ActionName = "松后溜",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = DISABLE,
			.ActionKey = 23,
			.ForbidActionType = SIGNAL_ACTION,
			.ForbidActionID = 28,
		},
		{	.ActionType = SIGNAL_ACTION,
			.ActionID = 29,
			.ActionName = "喷雾",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = DISABLE,
			.ActionKey = 24,
			.ForbidActionType = SIGNAL_ACTION,
			.ForbidActionID = 0,
		},
		{	.ActionType = SIGNAL_ACTION,
			.ActionID = 30,
			.ActionName = "后喷雾",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = DISABLE,
			.ActionKey = 24,
			.ForbidActionType = SIGNAL_ACTION,
			.ForbidActionID = 0,
		},
		{	.ActionType = COMBINATION_ACTION,
			.ActionID = 1,
			.ActionName = "自动移架",
			.MannulCtrlMode = DISABLE,
			.AutoCtrlMode = ENABLE,
			.ActionKey = 5,
			.ForbidActionType = COMBINATION_ACTION,
			.ForbidActionID = 0,
		},
		{	.ActionType = COMBINATION_ACTION,
			.ActionID = 3,
			.ActionName = "自动伸联动",
			.MannulCtrlMode = DISABLE,
			.AutoCtrlMode = ENABLE,
			.ActionKey = 7,
			.ForbidActionType = COMBINATION_ACTION,
			.ForbidActionID = 3,
		},
		{	.ActionType = COMBINATION_ACTION,
			.ActionID = 2,
			.ActionName = "自动收联动",
			.MannulCtrlMode = DISABLE,
			.AutoCtrlMode = ENABLE,
			.ActionKey = 8,
			.ForbidActionType = COMBINATION_ACTION,
			.ForbidActionID = 4,
		},
		{	.ActionType = COMBINATION_ACTION,
			.ActionID = 4,
			.ActionName = "自动放煤",
			.MannulCtrlMode = DISABLE,
			.AutoCtrlMode = ENABLE,
			.ActionKey = 13,
			.ForbidActionType = COMBINATION_ACTION,
			.ForbidActionID = 0,
		},		
		{	.ActionType = GROUP_ACTION,
			.ActionID = 13,
			.ActionName = "成组补压",
			.MannulCtrlMode = DISABLE,
			.AutoCtrlMode = ENABLE,
			.ActionKey = 1,
			.ForbidActionType = GROUP_ACTION,
			.ForbidActionID = 0,
		},	
		{	.ActionType = GROUP_ACTION,
			.ActionID = 2,
			.ActionName = "成组移架",
			.MannulCtrlMode = DISABLE,
			.AutoCtrlMode = ENABLE,
			.ActionKey = 5,
			.ForbidActionType = GROUP_ACTION,
			.ForbidActionID = 0,
		},
		{	.ActionType = GROUP_ACTION,
			.ActionID = 1,
			.ActionName = "成组推溜",
			.MannulCtrlMode = DISABLE,
			.AutoCtrlMode = ENABLE,
			.ActionKey = 6,
			.ForbidActionType = GROUP_ACTION,
			.ForbidActionID = 0,
		},
		{	.ActionType = GROUP_ACTION,
			.ActionID = 8,
			.ActionName = "成组伸护帮",
			.MannulCtrlMode = DISABLE,
			.AutoCtrlMode = ENABLE,
			.ActionKey = 7,
			.ForbidActionType = GROUP_ACTION,
			.ForbidActionID = 0,
		},
		{	.ActionType = GROUP_ACTION,
			.ActionID = 7,
			.ActionName = "成组收护帮",
			.MannulCtrlMode = DISABLE,
			.AutoCtrlMode = ENABLE,
			.ActionKey = 8,
			.ForbidActionType = GROUP_ACTION,
			.ForbidActionID = 0,
		},
		{	.ActionType = GROUP_ACTION,
			.ActionID = 6,
			.ActionName = "成组伸伸缩",
			.MannulCtrlMode = DISABLE,
			.AutoCtrlMode = ENABLE,
			.ActionKey = 9,
			.ForbidActionType = GROUP_ACTION,
			.ForbidActionID = 0,
		},
		{	.ActionType = GROUP_ACTION,
			.ActionID = 5,
			.ActionName = "成组收伸缩",
			.MannulCtrlMode = DISABLE,
			.AutoCtrlMode = ENABLE,
			.ActionKey = 10,
			.ForbidActionType = GROUP_ACTION,
			.ForbidActionID = 0,
		},	
		{	.ActionType = GROUP_ACTION,
			.ActionID = 9,
			.ActionName = "成组拉后溜",
			.MannulCtrlMode = DISABLE,
			.AutoCtrlMode = ENABLE,
			.ActionKey = 11,
			.ForbidActionType = GROUP_ACTION,
			.ForbidActionID = 0,
		},
		{	.ActionType = GROUP_ACTION,
			.ActionID = 12,
			.ActionName = "成组放煤",
			.MannulCtrlMode = DISABLE,
			.AutoCtrlMode = ENABLE,
			.ActionKey = 13,
			.ForbidActionType = GROUP_ACTION,
			.ForbidActionID = 0,
		},		
		{	.ActionType = GROUP_ACTION,
			.ActionID = 10,
			.ActionName = "成组拉溜",
			.MannulCtrlMode = DISABLE,
			.AutoCtrlMode = ENABLE,
			.ActionKey = 17,
			.ForbidActionType = GROUP_ACTION,
			.ForbidActionID = 0,
		},
		{	.ActionType = GROUP_ACTION,
			.ActionID = 4,
			.ActionName = "成组伸联动",
			.MannulCtrlMode = DISABLE,
			.AutoCtrlMode = ENABLE,
			.ActionKey = 21,
			.ForbidActionType = GROUP_ACTION,
			.ForbidActionID = 0,
		},
		{	.ActionType = GROUP_ACTION,
			.ActionID = 3,
			.ActionName = "成组收联动",
			.MannulCtrlMode = DISABLE,
			.AutoCtrlMode = ENABLE,
			.ActionKey = 22,
			.ForbidActionType = GROUP_ACTION,
			.ForbidActionID = 0,
		},		
		{	.ActionType = GROUP_ACTION,
			.ActionID = 11,
			.ActionName = "成组喷雾",
			.MannulCtrlMode = DISABLE,
			.AutoCtrlMode = ENABLE,
			.ActionKey = 24,
			.ForbidActionType = GROUP_ACTION,
			.ForbidActionID = 0,
		},
	},
};


#elif (APPLICATION_SCENARIO_SELECTION == LIVE_CHICKEN_AND_RABBIT_WORKING_FACE || APPLICATION_SCENARIO_SELECTION == LIVE_CHICKEN_AND_RABBIT_LEAD_SUPPORT)
//活鸡兔中间架及端头架
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
			.ActionName = "移架",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = DISABLE,
			.ActionKey = 4,
			.ForbidActionType = SIGNAL_ACTION,
			.ForbidActionID = 3,
		},
		{	.ActionType = SIGNAL_ACTION,
			.ActionID = 2,
			.ActionName = "升柱",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = DISABLE,
			.ActionKey = 1,
			.ForbidActionType = SIGNAL_ACTION,
			.ForbidActionID = 1,
		},
		{	.ActionType = SIGNAL_ACTION,
			.ActionID = 3,
			.ActionName = "推溜",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = ENABLE,
			.ActionKey = 3,
			.ForbidActionType = SIGNAL_ACTION,
			.ForbidActionID = 4,
		},
		{	.ActionType = SIGNAL_ACTION,
			.ActionID = 1,
			.ActionName = "降柱",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = DISABLE,
			.ActionKey = 2,
			.ForbidActionType = SIGNAL_ACTION,
			.ForbidActionID = 2,
		},
		{	.ActionType = SIGNAL_ACTION,
			.ActionID = 5,
			.ActionName = "收平衡",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = DISABLE,
			.ActionKey = 6,
			.ForbidActionType = SIGNAL_ACTION,
			.ForbidActionID = 6,
		},
		{	.ActionType = SIGNAL_ACTION,
			.ActionID = 6,
			.ActionName = "伸平衡",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = DISABLE,
			.ActionKey = 5,
			.ForbidActionType = SIGNAL_ACTION,
			.ForbidActionID = 5,
		},
		{	.ActionType = SIGNAL_ACTION,
			.ActionID = 7,
			.ActionName = "收侧护",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = DISABLE,
			.ActionKey = 18,
			.ForbidActionType = 1,
			.ForbidActionID = 8,
		},
		{	.ActionType = SIGNAL_ACTION,
			.ActionID = 8,
			.ActionName = "伸侧护",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = DISABLE,
			.ActionKey = 17,
			.ForbidActionType = SIGNAL_ACTION,
			.ForbidActionID = 7,
		},
		{	.ActionType = SIGNAL_ACTION,
			.ActionID = 10,
			.ActionName = "抬底座",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = DISABLE,
			.ActionKey = 11,
			.ForbidActionType = SIGNAL_ACTION,
			.ForbidActionID = 0,
		},
		{	.ActionType = SIGNAL_ACTION,
			.ActionID = 9,
			.ActionName = "喷雾",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = ENABLE,
			.ActionKey = 12,
			.ForbidActionType = SIGNAL_ACTION,
			.ForbidActionID = 0,
		},
		{	.ActionType = SIGNAL_ACTION,
			.ActionID = 11,
			.ActionName = "收伸缩梁",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = DISABLE,
			.ActionKey = 10,
			.ForbidActionType = SIGNAL_ACTION,
			.ForbidActionID = 0,
		},
		{	.ActionType = SIGNAL_ACTION,
			.ActionID = 12,
			.ActionName = "伸伸缩梁",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = DISABLE,
			.ActionKey = 9,
			.ForbidActionType = SIGNAL_ACTION,
			.ForbidActionID = 0,
		},
		{	.ActionType = COMBINATION_ACTION,
			.ActionID = 1,
			.ActionName = "自动移架",
			.MannulCtrlMode = DISABLE,
			.AutoCtrlMode = ENABLE,
			.ActionKey = 4,
			.ForbidActionType = COMBINATION_ACTION,
			.ForbidActionID = 0,
		},
		{	.ActionType = COMBINATION_ACTION,
			.ActionID = 2,
			.ActionName = "自动补压",
			.MannulCtrlMode = DISABLE,
			.AutoCtrlMode = ENABLE,
			.ActionKey = 1,
			.ForbidActionType = COMBINATION_ACTION,
			.ForbidActionID = 0,
		},
		{	.ActionType = GROUP_ACTION,
			.ActionID = 2,
			.ActionName = "成组移架",
			.MannulCtrlMode = DISABLE,
			.AutoCtrlMode = ENABLE,
			.ActionKey = 4,
			.ForbidActionType = GROUP_ACTION,
			.ForbidActionID = 0,
		},
		{	.ActionType = GROUP_ACTION,
			.ActionID = 1,
			.ActionName = "成组推溜",
			.MannulCtrlMode = DISABLE,
			.AutoCtrlMode = ENABLE,
			.ActionKey = 3,
			.ForbidActionType = GROUP_ACTION,
			.ForbidActionID = 0,
		},
		{	.ActionType = GROUP_ACTION,
			.ActionID = 3,
			.ActionName = "成组拉溜",
			.MannulCtrlMode = DISABLE,
			.AutoCtrlMode = ENABLE,
			.ActionKey = 16,
			.ForbidActionType = GROUP_ACTION,
			.ForbidActionID = 0,
		},
		{	.ActionType = GROUP_ACTION,
			.ActionID = 4,
			.ActionName = "成组喷雾",
			.MannulCtrlMode = DISABLE,
			.AutoCtrlMode = ENABLE,
			.ActionKey = 12,
			.ForbidActionType = GROUP_ACTION,
			.ForbidActionID = 0,
		},
		{	.ActionType = GROUP_ACTION,
			.ActionID = 5,
			.ActionName = "成组收伸缩",
			.MannulCtrlMode = DISABLE,
			.AutoCtrlMode = ENABLE,
			.ActionKey = 10,
			.ForbidActionType = GROUP_ACTION,
			.ForbidActionID = 0,
		},
		{	.ActionType = GROUP_ACTION,
			.ActionID = 6,
			.ActionName = "成组伸伸缩",
			.MannulCtrlMode = DISABLE,
			.AutoCtrlMode = ENABLE,
			.ActionKey = 9,
			.ForbidActionType = GROUP_ACTION,
			.ForbidActionID = 0,
		},
		{	.ActionType = GROUP_ACTION,
			.ActionID = 7,
			.ActionName = "成组补压",
			.MannulCtrlMode = DISABLE,
			.AutoCtrlMode = ENABLE,
			.ActionKey = 1,
			.ForbidActionType = GROUP_ACTION,
			.ForbidActionID = 0,
		},
	},
};


#elif (APPLICATION_SCENARIO_SELECTION == LIVE_CHICKEN_AND_RABBIT_SELF_MOVING_TAIL)
//活鸡兔自移机尾
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
			.ActionName = "右前升",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = DISABLE,
			.ActionKey = 1,
			.ForbidActionType = SIGNAL_ACTION,
			.ForbidActionID = 2,
		},
		{	.ActionType = SIGNAL_ACTION,
			.ActionID = 2,
			.ActionName = "右前降",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = DISABLE,
			.ActionKey = 2,
			.ForbidActionType = SIGNAL_ACTION,
			.ForbidActionID = 1,
		},
		{	.ActionType = SIGNAL_ACTION,
			.ActionID = 3,
			.ActionName = "右后升",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = DISABLE,
			.ActionKey = 3,
			.ForbidActionType = SIGNAL_ACTION,
			.ForbidActionID = 4,
		},
		{	.ActionType = SIGNAL_ACTION,
			.ActionID = 4,
			.ActionName = "右后降",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = DISABLE,
			.ActionKey = 4,
			.ForbidActionType = SIGNAL_ACTION,
			.ForbidActionID = 3,
		},
		{	.ActionType = SIGNAL_ACTION,
			.ActionID = 5,
			.ActionName = "前移",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = DISABLE,
			.ActionKey = 5,
			.ForbidActionType = SIGNAL_ACTION,
			.ForbidActionID = 6,
		},
		{	.ActionType = SIGNAL_ACTION,
			.ActionID = 6,
			.ActionName = "后退",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = DISABLE,
			.ActionKey = 6,
			.ForbidActionType = SIGNAL_ACTION,
			.ForbidActionID = 5,
		},
		{	.ActionType = SIGNAL_ACTION,
			.ActionID = 7,
			.ActionName = "左前升",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = DISABLE,
			.ActionKey = 7,
			.ForbidActionType = SIGNAL_ACTION,
			.ForbidActionID = 8,
		},
		{	.ActionType = SIGNAL_ACTION,
			.ActionID = 8,
			.ActionName = "左前降",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = DISABLE,
			.ActionKey = 8,
			.ForbidActionType = SIGNAL_ACTION,
			.ForbidActionID = 7,
		},
		{	.ActionType = SIGNAL_ACTION,
			.ActionID = 9,
			.ActionName = "左后升",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = DISABLE,
			.ActionKey = 9,
			.ForbidActionType = SIGNAL_ACTION,
			.ForbidActionID = 10,
		},
		{	.ActionType = SIGNAL_ACTION,
			.ActionID = 10,
			.ActionName = "左后降",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = DISABLE,
			.ActionKey = 10,
			.ForbidActionType = SIGNAL_ACTION,
			.ForbidActionID = 9,
		},
		{	.ActionType = SIGNAL_ACTION,
			.ActionID = 11,
			.ActionName = "前架升",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = DISABLE,
			.ActionKey = 11,
			.ForbidActionType = SIGNAL_ACTION,
			.ForbidActionID = 12,
		},
		{	.ActionType = SIGNAL_ACTION,
			.ActionID = 12,
			.ActionName = "前架降",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = DISABLE,
			.ActionKey = 12,
			.ForbidActionType = SIGNAL_ACTION,
			.ForbidActionID = 11,
		},
		{	.ActionType = SIGNAL_ACTION,
			.ActionID = 13,
			.ActionName = "前左摆",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = DISABLE,
			.ActionKey = 13,
			.ForbidActionType = SIGNAL_ACTION,
			.ForbidActionID = 14,
		},
		{	.ActionType = SIGNAL_ACTION,
			.ActionID = 14,
			.ActionName = "前右摆",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = DISABLE,
			.ActionKey = 14,
			.ForbidActionType = SIGNAL_ACTION,
			.ForbidActionID = 13,
		},
		{	.ActionType = SIGNAL_ACTION,
			.ActionID = 15,
			.ActionName = "后左摆",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = DISABLE,
			.ActionKey = 15,
			.ForbidActionType = SIGNAL_ACTION,
			.ForbidActionID = 0,
		},
		{	.ActionType = SIGNAL_ACTION,
			.ActionID = 16,
			.ActionName = "后右摆",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = DISABLE,
			.ActionKey = 16,
			.ForbidActionType = SIGNAL_ACTION,
			.ForbidActionID = 0,
		},		
	},
};




#elif (APPLICATION_SCENARIO_SELECTION == LIVE_CHICKEN_AND_RABBIT_CABLE_CAR)
//活鸡兔设备列车
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
			.ActionName = "伸油缸1",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = DISABLE,
			.ActionKey = 1,
			.ForbidActionType = SIGNAL_ACTION,
			.ForbidActionID = 2,
		},
		{	.ActionType = SIGNAL_ACTION,
			.ActionID = 2,
			.ActionName = "收油缸1",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = DISABLE,
			.ActionKey = 2,
			.ForbidActionType = SIGNAL_ACTION,
			.ForbidActionID = 1,
		},
		{	.ActionType = SIGNAL_ACTION,
			.ActionID = 3,
			.ActionName = "伸油缸2",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = DISABLE,
			.ActionKey = 3,
			.ForbidActionType = SIGNAL_ACTION,
			.ForbidActionID = 4,
		},
		{	.ActionType = SIGNAL_ACTION,
			.ActionID = 4,
			.ActionName = "收油缸2",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = DISABLE,
			.ActionKey = 4,
			.ForbidActionType = SIGNAL_ACTION,
			.ForbidActionID = 3,
		},
		{	.ActionType = SIGNAL_ACTION,
			.ActionID = 5,
			.ActionName = "伸油缸3",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = DISABLE,
			.ActionKey = 5,
			.ForbidActionType = SIGNAL_ACTION,
			.ForbidActionID = 6,
		},
		{	.ActionType = SIGNAL_ACTION,
			.ActionID = 6,
			.ActionName = "收油缸3",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = DISABLE,
			.ActionKey = 6,
			.ForbidActionType = SIGNAL_ACTION,
			.ForbidActionID = 5,
		},
		{	.ActionType = SIGNAL_ACTION,
			.ActionID = 7,
			.ActionName = "伸油缸4",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = DISABLE,
			.ActionKey = 7,
			.ForbidActionType = SIGNAL_ACTION,
			.ForbidActionID = 8,
		},
		{	.ActionType = SIGNAL_ACTION,
			.ActionID = 8,
			.ActionName = "收油缸4",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = DISABLE,
			.ActionKey = 8,
			.ForbidActionType = SIGNAL_ACTION,
			.ForbidActionID = 7,
		},
		{	.ActionType = SIGNAL_ACTION,
			.ActionID = 9,
			.ActionName = "伸油缸5",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = DISABLE,
			.ActionKey = 9,
			.ForbidActionType = SIGNAL_ACTION,
			.ForbidActionID = 10,
		},
		{	.ActionType = SIGNAL_ACTION,
			.ActionID = 10,
			.ActionName = "收油缸5",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = DISABLE,
			.ActionKey = 10,
			.ForbidActionType = SIGNAL_ACTION,
			.ForbidActionID = 9,
		},
		{	.ActionType = SIGNAL_ACTION,
			.ActionID = 11,
			.ActionName = "伸油缸6",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = DISABLE,
			.ActionKey = 11,
			.ForbidActionType = SIGNAL_ACTION,
			.ForbidActionID = 12,
		},
		{	.ActionType = SIGNAL_ACTION,
			.ActionID = 12,
			.ActionName = "收油缸6",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = DISABLE,
			.ActionKey = 12,
			.ForbidActionType = SIGNAL_ACTION,
			.ForbidActionID = 11,
		},
		{	.ActionType = SIGNAL_ACTION,
			.ActionID = 13,
			.ActionName = "伸油缸7",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = DISABLE,
			.ActionKey = 13,
			.ForbidActionType = SIGNAL_ACTION,
			.ForbidActionID = 14,
		},
		{	.ActionType = SIGNAL_ACTION,
			.ActionID = 14,
			.ActionName = "收油缸7",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = DISABLE,
			.ActionKey = 14,
			.ForbidActionType = SIGNAL_ACTION,
			.ForbidActionID = 13,
		},
		{	.ActionType = SIGNAL_ACTION,
			.ActionID = 15,
			.ActionName = "伸油缸8",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = DISABLE,
			.ActionKey = 15,
			.ForbidActionType = SIGNAL_ACTION,
			.ForbidActionID = 0,
		},
		{	.ActionType = SIGNAL_ACTION,
			.ActionID = 16,
			.ActionName = "收油缸8",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = DISABLE,
			.ActionKey = 16,
			.ForbidActionType = SIGNAL_ACTION,
			.ForbidActionID = 0,
		},		
	},
};





#elif (APPLICATION_SCENARIO_SELECTION == DALIUTA_WORKING_FACE)
//大柳塔中间架
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
			.ActionName = "移架",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = DISABLE,
			.ActionKey = 4,
			.ForbidActionType = SIGNAL_ACTION,
			.ForbidActionID = 3,
		},
		{	.ActionType = SIGNAL_ACTION,
			.ActionID = 2,
			.ActionName = "升柱",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = DISABLE,
			.ActionKey = 1,
			.ForbidActionType = SIGNAL_ACTION,
			.ForbidActionID = 1,
		},
		{	.ActionType = SIGNAL_ACTION,
			.ActionID = 3,
			.ActionName = "推溜",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = ENABLE,
			.ActionKey = 3,
			.ForbidActionType = SIGNAL_ACTION,
			.ForbidActionID = 4,
		},
		{	.ActionType = SIGNAL_ACTION,
			.ActionID = 1,
			.ActionName = "降柱",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = DISABLE,
			.ActionKey = 2,
			.ForbidActionType = SIGNAL_ACTION,
			.ForbidActionID = 2,
		},
		{	.ActionType = SIGNAL_ACTION,
			.ActionID = 5,
			.ActionName = "收平衡",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = DISABLE,
			.ActionKey = 6,
			.ForbidActionType = SIGNAL_ACTION,
			.ForbidActionID = 6,
		},
		{	.ActionType = SIGNAL_ACTION,
			.ActionID = 6,
			.ActionName = "伸平衡",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = DISABLE,
			.ActionKey = 5,
			.ForbidActionType = SIGNAL_ACTION,
			.ForbidActionID = 5,
		},
		{	.ActionType = SIGNAL_ACTION,
			.ActionID = 7,
			.ActionName = "收侧护",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = DISABLE,
			.ActionKey = 18,
			.ForbidActionType = 1,
			.ForbidActionID = 8,
		},
		{	.ActionType = SIGNAL_ACTION,
			.ActionID = 8,
			.ActionName = "伸侧护",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = DISABLE,
			.ActionKey = 17,
			.ForbidActionType = SIGNAL_ACTION,
			.ForbidActionID = 7,
		},
		{	.ActionType = SIGNAL_ACTION,
			.ActionID = 10,
			.ActionName = "抬底座",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = DISABLE,
			.ActionKey = 11,
			.ForbidActionType = SIGNAL_ACTION,
			.ForbidActionID = 0,
		},
		{	.ActionType = SIGNAL_ACTION,
			.ActionID = 9,
			.ActionName = "收护帮2",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = ENABLE,
			.ActionKey = 14,
			.ForbidActionType = SIGNAL_ACTION,
			.ForbidActionID = 0,
		},
		{	.ActionType = SIGNAL_ACTION,
			.ActionID = 11,
			.ActionName = "收伸缩梁",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = DISABLE,
			.ActionKey = 10,
			.ForbidActionType = SIGNAL_ACTION,
			.ForbidActionID = 0,
		},
		{	.ActionType = SIGNAL_ACTION,
			.ActionID = 12,
			.ActionName = "伸伸缩梁",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = DISABLE,
			.ActionKey = 9,
			.ForbidActionType = SIGNAL_ACTION,
			.ForbidActionID = 0,
		},
		{	.ActionType = SIGNAL_ACTION,
			.ActionID = 13,
			.ActionName = "收护帮",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = DISABLE,
			.ActionKey = 8,
			.ForbidActionType = SIGNAL_ACTION,
			.ForbidActionID = 14,
		},
		{	.ActionType = SIGNAL_ACTION,
			.ActionID = 14,
			.ActionName = "伸护帮",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = DISABLE,
			.ActionKey = 7,
			.ForbidActionType = SIGNAL_ACTION,
			.ForbidActionID = 13,
		},	
		{	.ActionType = SIGNAL_ACTION,
			.ActionID = 15,
			.ActionName = "收护帮3",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = DISABLE,
			.ActionKey = 16,
			.ForbidActionType = SIGNAL_ACTION,
			.ForbidActionID = 16,
		},
		{	.ActionType = SIGNAL_ACTION,
			.ActionID = 16,
			.ActionName = "伸护帮3",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = DISABLE,
			.ActionKey = 15,
			.ForbidActionType = SIGNAL_ACTION,
			.ForbidActionID = 15,
		},		
		{	.ActionType = SIGNAL_ACTION,
			.ActionID = 17,
			.ActionName = "左反冲1",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = DISABLE,
			.ActionKey = 19,
			.ForbidActionType = SIGNAL_ACTION,
			.ForbidActionID = 0,
		},
		{	.ActionType = SIGNAL_ACTION,
			.ActionID = 18,
			.ActionName = "左反冲2",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = DISABLE,
			.ActionKey = 20,
			.ForbidActionType = SIGNAL_ACTION,
			.ForbidActionID = 0,
		},
		{	.ActionType = SIGNAL_ACTION,
			.ActionID = 19,
			.ActionName = "右反冲1",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = DISABLE,
			.ActionKey = 21,
			.ForbidActionType = SIGNAL_ACTION,
			.ForbidActionID = 0,
		},
		{	.ActionType = SIGNAL_ACTION,
			.ActionID = 20,
			.ActionName = "右反冲2",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = DISABLE,
			.ActionKey = 22,
			.ForbidActionType = SIGNAL_ACTION,
			.ForbidActionID = 0,
		},		
		{	.ActionType = COMBINATION_ACTION,
			.ActionID = 1,
			.ActionName = "自动移架",
			.MannulCtrlMode = DISABLE,
			.AutoCtrlMode = ENABLE,
			.ActionKey = 4,
			.ForbidActionType = COMBINATION_ACTION,
			.ForbidActionID = 0,
		},
		{	.ActionType = COMBINATION_ACTION,
			.ActionID = 2,
			.ActionName = "自动补压",
			.MannulCtrlMode = DISABLE,
			.AutoCtrlMode = ENABLE,
			.ActionKey = 1,
			.ForbidActionType = COMBINATION_ACTION,
			.ForbidActionID = 0,
		},
		{	.ActionType = COMBINATION_ACTION,
			.ActionID = 3,
			.ActionName = "自动收护联动",
			.MannulCtrlMode = DISABLE,
			.AutoCtrlMode = ENABLE,
			.ActionKey = 15,
			.ForbidActionType = COMBINATION_ACTION,
			.ForbidActionID = 0,
		},
		{	.ActionType = COMBINATION_ACTION,
			.ActionID = 4,
			.ActionName = "自动伸护联动",
			.MannulCtrlMode = DISABLE,
			.AutoCtrlMode = ENABLE,
			.ActionKey = 14,
			.ForbidActionType = COMBINATION_ACTION,
			.ForbidActionID = 0,
		},
		{	.ActionType = GROUP_ACTION,
			.ActionID = 2,
			.ActionName = "成组移架",
			.MannulCtrlMode = DISABLE,
			.AutoCtrlMode = ENABLE,
			.ActionKey = 4,
			.ForbidActionType = GROUP_ACTION,
			.ForbidActionID = 0,
		},
		{	.ActionType = GROUP_ACTION,
			.ActionID = 1,
			.ActionName = "成组推溜",
			.MannulCtrlMode = DISABLE,
			.AutoCtrlMode = ENABLE,
			.ActionKey = 3,
			.ForbidActionType = GROUP_ACTION,
			.ForbidActionID = 0,
		},
		{	.ActionType = GROUP_ACTION,
			.ActionID = 3,
			.ActionName = "成组拉溜",
			.MannulCtrlMode = DISABLE,
			.AutoCtrlMode = ENABLE,
			.ActionKey = 12,
			.ForbidActionType = GROUP_ACTION,
			.ForbidActionID = 0,
		},
		{	.ActionType = GROUP_ACTION,
			.ActionID = 4,
			.ActionName = "成组收伸缩",
			.MannulCtrlMode = DISABLE,
			.AutoCtrlMode = ENABLE,
			.ActionKey = 10,
			.ForbidActionType = GROUP_ACTION,
			.ForbidActionID = 0,
		},
		{	.ActionType = GROUP_ACTION,
			.ActionID = 5,
			.ActionName = "成组伸伸缩",
			.MannulCtrlMode = DISABLE,
			.AutoCtrlMode = ENABLE,
			.ActionKey = 9,
			.ForbidActionType = GROUP_ACTION,
			.ForbidActionID = 0,
		},
		{	.ActionType = GROUP_ACTION,
			.ActionID = 6,
			.ActionName = "成组补压",
			.MannulCtrlMode = DISABLE,
			.AutoCtrlMode = ENABLE,
			.ActionKey = 1,
			.ForbidActionType = GROUP_ACTION,
			.ForbidActionID = 0,
		},
		{	.ActionType = GROUP_ACTION,
			.ActionID = 7,
			.ActionName = "成组收护帮",
			.MannulCtrlMode = DISABLE,
			.AutoCtrlMode = ENABLE,
			.ActionKey = 8,
			.ForbidActionType = GROUP_ACTION,
			.ForbidActionID = 0,
		},
		{	.ActionType = GROUP_ACTION,
			.ActionID = 8,
			.ActionName = "成组伸护帮",
			.MannulCtrlMode = DISABLE,
			.AutoCtrlMode = ENABLE,
			.ActionKey = 7,
			.ForbidActionType = GROUP_ACTION,
			.ForbidActionID = 0,
		},
		{	.ActionType = GROUP_ACTION,
			.ActionID = 9,
			.ActionName = "成组收联动",
			.MannulCtrlMode = DISABLE,
			.AutoCtrlMode = ENABLE,
			.ActionKey = 16,
			.ForbidActionType = GROUP_ACTION,
			.ForbidActionID = 0,
		},
		{	.ActionType = GROUP_ACTION,
			.ActionID = 10,
			.ActionName = "成组伸联动",
			.MannulCtrlMode = DISABLE,
			.AutoCtrlMode = ENABLE,
			.ActionKey = 15,
			.ForbidActionType = GROUP_ACTION,
			.ForbidActionID = 0,
		},		
	},
};

#elif (APPLICATION_SCENARIO_SELECTION == JINJIE_31117_WORKING_FACE)
//锦界中间架
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
			.ActionName = "移架",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = DISABLE,
			.ActionKey = 4,
			.ForbidActionType = SIGNAL_ACTION,
			.ForbidActionID = 3,
		},
		{	.ActionType = SIGNAL_ACTION,
			.ActionID = 2,
			.ActionName = "升柱",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = DISABLE,
			.ActionKey = 1,
			.ForbidActionType = SIGNAL_ACTION,
			.ForbidActionID = 1,
		},
		{	.ActionType = SIGNAL_ACTION,
			.ActionID = 3,
			.ActionName = "推溜",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = ENABLE,
			.ActionKey = 3,
			.ForbidActionType = SIGNAL_ACTION,
			.ForbidActionID = 4,
		},
		{	.ActionType = SIGNAL_ACTION,
			.ActionID = 1,
			.ActionName = "降柱",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = DISABLE,
			.ActionKey = 2,
			.ForbidActionType = SIGNAL_ACTION,
			.ForbidActionID = 2,
		},
		{	.ActionType = SIGNAL_ACTION,
			.ActionID = 5,
			.ActionName = "收平衡",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = DISABLE,
			.ActionKey = 6,
			.ForbidActionType = SIGNAL_ACTION,
			.ForbidActionID = 6,
		},
		{	.ActionType = SIGNAL_ACTION,
			.ActionID = 6,
			.ActionName = "伸平衡",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = DISABLE,
			.ActionKey = 5,
			.ForbidActionType = SIGNAL_ACTION,
			.ForbidActionID = 5,
		},
		{	.ActionType = SIGNAL_ACTION,
			.ActionID = 7,
			.ActionName = "收侧护",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = DISABLE,
			.ActionKey = 18,
			.ForbidActionType = 1,
			.ForbidActionID = 8,
		},
		{	.ActionType = SIGNAL_ACTION,
			.ActionID = 8,
			.ActionName = "伸侧护",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = DISABLE,
			.ActionKey = 17,
			.ForbidActionType = SIGNAL_ACTION,
			.ForbidActionID = 7,
		},
		{	.ActionType = SIGNAL_ACTION,
			.ActionID = 10,
			.ActionName = "抬底座",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = DISABLE,
			.ActionKey = 11,
			.ForbidActionType = SIGNAL_ACTION,
			.ForbidActionID = 0,
		},
		{	.ActionType = SIGNAL_ACTION,
			.ActionID = 9,
			.ActionName = "喷雾",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = ENABLE,
			.ActionKey = 12,
			.ForbidActionType = SIGNAL_ACTION,
			.ForbidActionID = 0,
		},
		{	.ActionType = SIGNAL_ACTION,
			.ActionID = 11,
			.ActionName = "收伸缩梁",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = DISABLE,
			.ActionKey = 10,
			.ForbidActionType = SIGNAL_ACTION,
			.ForbidActionID = 0,
		},
		{	.ActionType = SIGNAL_ACTION,
			.ActionID = 12,
			.ActionName = "伸伸缩梁",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = DISABLE,
			.ActionKey = 9,
			.ForbidActionType = SIGNAL_ACTION,
			.ForbidActionID = 0,
		},
		{	.ActionType = SIGNAL_ACTION,
			.ActionID = 13,
			.ActionName = "收护帮",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = DISABLE,
			.ActionKey = 8,
			.ForbidActionType = SIGNAL_ACTION,
			.ForbidActionID = 14,
		},
		{	.ActionType = SIGNAL_ACTION,
			.ActionID = 14,
			.ActionName = "伸护帮",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = DISABLE,
			.ActionKey = 7,
			.ForbidActionType = SIGNAL_ACTION,
			.ForbidActionID = 13,
		},		
		{	.ActionType = SIGNAL_ACTION,
			.ActionID = 15,
			.ActionName = "收底调",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = DISABLE,
			.ActionKey = 19,
			.ForbidActionType = SIGNAL_ACTION,
			.ForbidActionID = 0,
		},
		{	.ActionType = SIGNAL_ACTION,
			.ActionID = 16,
			.ActionName = "伸底调",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = DISABLE,
			.ActionKey = 20,
			.ForbidActionType = SIGNAL_ACTION,
			.ForbidActionID = 0,
		},
		{	.ActionType = SIGNAL_ACTION,
			.ActionID = 17,
			.ActionName = "反冲1",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = DISABLE,
			.ActionKey = 21,
			.ForbidActionType = SIGNAL_ACTION,
			.ForbidActionID = 0,
		},
		{	.ActionType = SIGNAL_ACTION,
			.ActionID = 18,
			.ActionName = "反冲2",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = DISABLE,
			.ActionKey = 22,
			.ForbidActionType = SIGNAL_ACTION,
			.ForbidActionID = 0,
		},
		{	.ActionType = SIGNAL_ACTION,
			.ActionID = 22,
			.ActionName = "拉溜",
			.MannulCtrlMode = ENABLE,
			.AutoCtrlMode = DISABLE,
			.ActionKey = 16,
			.ForbidActionType = SIGNAL_ACTION,
			.ForbidActionID = 0,
		},
		{	.ActionType = COMBINATION_ACTION,
			.ActionID = 1,
			.ActionName = "自动移架",
			.MannulCtrlMode = DISABLE,
			.AutoCtrlMode = ENABLE,
			.ActionKey = 4,
			.ForbidActionType = COMBINATION_ACTION,
			.ForbidActionID = 0,
		},
		{	.ActionType = COMBINATION_ACTION,
			.ActionID = 2,
			.ActionName = "自动补压",
			.MannulCtrlMode = DISABLE,
			.AutoCtrlMode = ENABLE,
			.ActionKey = 1,
			.ForbidActionType = COMBINATION_ACTION,
			.ForbidActionID = 0,
		},
		{	.ActionType = COMBINATION_ACTION,
			.ActionID = 3,
			.ActionName = "收联动",
			.MannulCtrlMode = DISABLE,
			.AutoCtrlMode = ENABLE,
			.ActionKey = 8,
			.ForbidActionType = COMBINATION_ACTION,
			.ForbidActionID = 0,
		},		
		{	.ActionType = COMBINATION_ACTION,
			.ActionID = 4,
			.ActionName = "伸联动",
			.MannulCtrlMode = DISABLE,
			.AutoCtrlMode = ENABLE,
			.ActionKey = 7,
			.ForbidActionType = COMBINATION_ACTION,
			.ForbidActionID = 0,
		},
		{	.ActionType = COMBINATION_ACTION,
			.ActionID = 5,
			.ActionName = "自动反冲洗",
			.MannulCtrlMode = DISABLE,
			.AutoCtrlMode = ENABLE,
			.ActionKey = 21,
			.ForbidActionType = COMBINATION_ACTION,
			.ForbidActionID = 0,
		},
		{	.ActionType = GROUP_ACTION,
			.ActionID = 2,
			.ActionName = "成组移架",
			.MannulCtrlMode = DISABLE,
			.AutoCtrlMode = ENABLE,
			.ActionKey = 4,
			.ForbidActionType = GROUP_ACTION,
			.ForbidActionID = 0,
		},
		{	.ActionType = GROUP_ACTION,
			.ActionID = 1,
			.ActionName = "成组推溜",
			.MannulCtrlMode = DISABLE,
			.AutoCtrlMode = ENABLE,
			.ActionKey = 3,
			.ForbidActionType = GROUP_ACTION,
			.ForbidActionID = 0,
		},
		{	.ActionType = GROUP_ACTION,
			.ActionID = 3,
			.ActionName = "成组拉溜",
			.MannulCtrlMode = DISABLE,
			.AutoCtrlMode = ENABLE,
			.ActionKey = 16,
			.ForbidActionType = GROUP_ACTION,
			.ForbidActionID = 0,
		},
		{	.ActionType = GROUP_ACTION,
			.ActionID = 4,
			.ActionName = "成组喷雾",
			.MannulCtrlMode = DISABLE,
			.AutoCtrlMode = ENABLE,
			.ActionKey = 12,
			.ForbidActionType = GROUP_ACTION,
			.ForbidActionID = 0,
		},
		{	.ActionType = GROUP_ACTION,
			.ActionID = 5,
			.ActionName = "成组收伸缩",
			.MannulCtrlMode = DISABLE,
			.AutoCtrlMode = ENABLE,
			.ActionKey = 10,
			.ForbidActionType = GROUP_ACTION,
			.ForbidActionID = 0,
		},
		{	.ActionType = GROUP_ACTION,
			.ActionID = 6,
			.ActionName = "成组伸伸缩",
			.MannulCtrlMode = DISABLE,
			.AutoCtrlMode = ENABLE,
			.ActionKey = 9,
			.ForbidActionType = GROUP_ACTION,
			.ForbidActionID = 0,
		},
		{	.ActionType = GROUP_ACTION,
			.ActionID = 7,
			.ActionName = "成组补压",
			.MannulCtrlMode = DISABLE,
			.AutoCtrlMode = ENABLE,
			.ActionKey = 1,
			.ForbidActionType = GROUP_ACTION,
			.ForbidActionID = 0,
		},
		{	.ActionType = GROUP_ACTION,
			.ActionID = 8,
			.ActionName = "成组收护帮",
			.MannulCtrlMode = DISABLE,
			.AutoCtrlMode = ENABLE,
			.ActionKey = 8,
			.ForbidActionType = GROUP_ACTION,
			.ForbidActionID = 0,
		},
		{	.ActionType = GROUP_ACTION,
			.ActionID = 9,
			.ActionName = "成组伸护帮",
			.MannulCtrlMode = DISABLE,
			.AutoCtrlMode = ENABLE,
			.ActionKey = 7,
			.ForbidActionType = GROUP_ACTION,
			.ForbidActionID = 0,
		},
		{	.ActionType = GROUP_ACTION,
			.ActionID = 10,
			.ActionName = "成组收联动",
			.MannulCtrlMode = DISABLE,
			.AutoCtrlMode = ENABLE,
			.ActionKey = 19,
			.ForbidActionType = GROUP_ACTION,
			.ForbidActionID = 0,
		},
		{	.ActionType = GROUP_ACTION,
			.ActionID = 11,
			.ActionName = "成组伸联动",
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






static void Wireless_SC_Decode_Proc(void);//控制器主动解除对码
static void Wireless_Action_Ack_Proc(uint8_t actionType,uint8_t antionID);
static uint8_t GetWirelessHeadMsg(uint16_t frameType ,WIRELESSPROHEAD * head);

//获取本架ID
uint8_t getSupportID(void)
{
	uint16_t len=0;
	uint8_t* pstate=NULL;
	globalParam_SupportType prar;

	SysDataFun(DATA_MSG_READ_OBJECT_PARAM,ENUM_SYSTEM,globalParamEnum_Support,0,0,&pstate,&len);
	memcpy((uint8_t*)&prar,pstate,len);
	return prar.currentSupportID;
}

//获取架号增向
uint8_t getSupportDirect(void)
{
	uint16_t len=0;
	uint8_t* pstate=NULL;
	globalParam_SupportType prar;

	SysDataFun(DATA_MSG_READ_OBJECT_PARAM,ENUM_SYSTEM,globalParamEnum_Support,0,0,&pstate,&len);
	memcpy((uint8_t*)&prar,pstate,len);
	return prar.SupportDirect;
}

//获取主控时间
uint8_t getMasterCtrlTime(void)
{
	uint16_t len=0;
	uint8_t* pstate=NULL;
	globalParam_SupportType prar;

	SysDataFun(DATA_MSG_READ_OBJECT_PARAM,ENUM_SYSTEM,globalParamEnum_Support,0,0,&pstate,&len);
	memcpy((uint8_t*)&prar,pstate,len);
	return prar.MasterCtrlTime;
}

//获取隔架架数
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


//获取急停闭锁状态
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


//获取无线版本号
uint8_t *getWirelessVersion(void)
{
	return WirelessVersion;
}
//获取无线在线状态
uint8_t getWirelssOnline(void)
{
	return WirelessOnline;
}

/*******************************************************************************************
**函数名称：LoadV4YkqPara
**函数作用：加载遥控器参数文件
**函数参数：无
**函数输出：无
**注意事项：无
*******************************************************************************************/
void LoadV4YkqPara(void)
{
	char ykqName[30] = "0:/update/ykqpara.bin";
	u16 OldCrc = 0;
	u16 fileCrc = 0;
	uint32_t len;
	
	
	//读取文件	
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
**函数名称：WirelessUniversal_Callback
**函数作用：通用定时器的使用
**函数参数：无
**函数输出：无
**注意事项：无
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

	//usart不在线超时回调
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
		//停止所有动作
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
**函数名称：Wireless_Heart_Proc
**函数作用：无线模块心跳处理
**函数参数：无
**函数输出：无
**注意事项：无
*******************************************************************************************/
uint16_t Wireless_Heart_Proc(STR_WLCAN_Frame* frame)
{

	//获取本架架号
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
**函数名称：IR_CodeMatch_Proc
**函数作用：红外接收对码
**函数参数：无
**函数输出：无
**注意事项：无
*******************************************************************************************/
void IR_CodeMatching_Ack_Proc(uint8_t coderes)
{
	//获取本架架号
	uint8_t scNum;
	//获取主控时间
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
**函数名称：IR_CodeMatch_Proc
**函数作用：红外接收对码
**函数参数：无
**函数输出：无
**注意事项：无
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
**函数名称：Wireless_CodeMatching_Proc
**函数作用：接到对码数据
**函数参数：无
**函数输出：无
**注意事项：无
*******************************************************************************************/
uint16_t Wireless_CodeMatching_Proc(STR_WLCAN_Frame* frame)
{
	//获取本架架号
	uint8_t scNum;
	uint8_t alar[2];
	scNum = getSupportID();
	uint8_t decodetmp[5] = {0};
	if(frame->u8DT[4] != scNum)
		return 0;
//	if((frame->u8DT[2]&0x80) != 0x80)//不是V4遥控器
//		return 0;
	if((frame->u8DT[2]&0x40) != 0x40)//如果是红外对码
	{
		if(V4ykq.YkqState != STATE_IR_CODEMATCHING)
		{
			if(V4ykq.YkqNum != frame->u8DT[5])
				return 0;
		}
	}
	else//如果是±键对码
	{
		if(V4ykq.YkqState == STATE_WL_SUCCESS)
		{
			if(V4ykq.YkqNum != frame->u8DT[5])
				return 0;
		}
		else
			V4ykq.YkqNum = frame->u8DT[5];
	}
		
	//计算crc
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
**函数名称：InsSendWirelessDataProc
**函数作用：数据塞入发送对列
**函数参数：无
**函数输出：无
**注意事项：无
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
**函数名称：Wireless_CodeMatching_Ack_Proc
**函数作用：无线对码结果应答
**函数参数：无
**函数输出：无
**注意事项：无
*******************************************************************************************/
void Wireless_CodeMatching_Ack_Proc(uint8_t state)
{
	//获取本架架号
	uint8_t scNum;
	//获取主控时间
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
	if(SysParam.globalParam.SupportParam.Paring_Method ==0) //对码方式为串口时
		InsSendWirelessDataProc(&frame,V4ykq.YkqNum);
	else if(SysParam.globalParam.SupportParam.Paring_Method ==1)//对码方式为CAN时
		InsSendWirelessAlarmDataProc(&frame,V4ykq.YkqNum);
	YkqIrTimer = TIMERCLOSE;
	YkqDecodeTimer = YKQDECODETIMER;
}

/*******************************************************************************************
**函数名称：Wireless_SC_Decode_Proc
**函数作用：控制器主动解除对码
**函数参数：无
**函数输出：无
**注意事项：无
*******************************************************************************************/
static void Wireless_SC_Decode_Proc(void)
{
	//获取本架架号
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
	if(SysParam.globalParam.SupportParam.Paring_Method ==0)//对码方式选择为串口时
		frame.u8DT[5] = 0;
	else if(SysParam.globalParam.SupportParam.Paring_Method ==1)//对码方式选择为CAN时
		frame.u8DT[5] = V4ykq.YkqNum;
	frame.u8DT[6] = 0;
	frame.u8DT[7] = 0;

	if(SysParam.globalParam.SupportParam.Paring_Method ==0)//对码方式选择为串口时
		InsSendWirelessDataProc(&frame,V4ykq.YkqNum);
	else if(SysParam.globalParam.SupportParam.Paring_Method ==1)//对码方式选择为CAN时
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
		//获取本架架号
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
**函数名称：Wireless_Ykq_Decode_Proc
**函数作用：遥控器主动解除对码
**函数参数：无
**函数输出：无
**注意事项：无
*******************************************************************************************/
uint16_t Wireless_Ykq_Decode_Proc(STR_WLCAN_Frame* frame)
{
	//获取本架架号
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
**函数名称：Wireless_Action_Proc
**函数作用：动作控制处理
**函数参数：无
**函数输出：无
**注意事项：无
*******************************************************************************************/
uint16_t Wireless_Action_Proc(STR_WLCAN_Frame* frame)
{
	//获取本架架号
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
	
	//frame->u8DT[2]//bit2手动/自动
	//frame->u8DT[3]//被控设备ID
	//frame->u8DT[4]//控制命令字
	//frame->u8DT[5]//起始架号
	//frame->u8DT[6]//结束架号
	//frame->u8DT[7]//执行方向
	YkqDecodeTimer = YKQDECODETIMER;
	
	switch(frame->u8DT[2]&0X07)
	{
		case SINGLE_CONTROL:

			if((frame->u8DT[2]&0x08) == 0x08)
				buf[1] = 1;//自动
			else
				buf[1] = 0;//手动
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
			if(supportDirect == 0)//左
			{
				if(frame->u8DT[7] == 0)//近->远
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
				else//近<-远
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
				if(frame->u8DT[7] == 0)//近->远
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
				else//近<-远
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
**函数名称：Wireless_Action_Ack_Proc
**函数作用：动作控制处理
**函数参数：无
**函数输出：无
**注意事项：无
*******************************************************************************************/
static void Wireless_Action_Ack_Proc(uint8_t actionType,uint8_t antionID)
{
		//获取本架架号
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
	if(SysParam.globalParam.SupportParam.Paring_Method ==0) //对码方式为串口时		
		frame.u8DT[6] = 0;
	else if(SysParam.globalParam.SupportParam.Paring_Method ==1)//对码方式为CAN时
		frame.u8DT[6] = V4ykq.YkqNum;
	frame.u8DT[7] = 0;
	
	if(SysParam.globalParam.SupportParam.Paring_Method ==0) //对码方式为串口时
		InsSendWirelessDataProc(&frame,V4ykq.YkqNum);
	else if(SysParam.globalParam.SupportParam.Paring_Method ==1)//对码方式为CAN时
		InsSendWirelessAlarmDataProc(&frame,V4ykq.YkqNum);
}



/*******************************************************************************************
**函数名称：sendYkqParaProc
**函数作用：发送遥控器数据
**函数参数：无
**函数输出：无
**注意事项：无
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
**函数名称：SendAlarm_YkqParaProc
**函数作用：从声光上发送遥控器数据
**函数参数：无
**函数输出：无
**注意事项：无
*******************************************************************************************/
void SendAlarm_YkqParaProc(void)
{
	return;
}
/*******************************************************************************************
**函数名称：Wireless_Para_Request_Proc
**函数作用：遥控器文件请求
**函数参数：无
**函数输出：无
**注意事项：无
*******************************************************************************************/
uint16_t Wireless_Para_Request_Proc(STR_WLCAN_Frame* frame)
{
		//获取本架架号
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
	
	if(SysParam.globalParam.SupportParam.Paring_Method == 0)//当对码方式选择为串口时
		OSMboxPost(revProgramSem,(void*)SENDYKQPARASEM);
	else if(SysParam.globalParam.SupportParam.Paring_Method ==1)//当对码方式选择为CAN时
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
		//获取本架架号
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
	
	if(SysParam.globalParam.SupportParam.Paring_Method ==0) //当对码方式选择为串口时
		InsSendWirelessDataProc(&frame,V4ykq.YkqNum);
	else if(SysParam.globalParam.SupportParam.Paring_Method ==1)//当对码方式选择为CAN时
		InsSendWirelessAlarmDataProc(&frame,V4ykq.YkqNum);
}

uint16_t Wireless_Neighbor_Request_Proc(STR_WLCAN_Frame* frame)
{
			//获取本架架号
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
	/*遥控器*/
	//心跳
	{
		0,       //帧类型
		0,       //超时时间
		0,       //发送次数
		0,       //应答
		Wireless_Heart_Proc,     //接收回调函数
		NULL,     
		NULL,     
		NULL,    
	},
	//遥控器红外对码
	{
		4,       //帧类型
		0,       //超时时间
		0,       //发送次数
		0,       //应答
		IR_CodeMatching_Proc,    //接收回调函数
		NULL,     
		NULL,     
		NULL,    
	},
	//遥控器红外对码结果
	{
		5,       //帧类型
		0,       //超时时间
		0,       //发送次数
		0,       //应答
		NULL,    //接收回调函数
		NULL,     
		NULL,     
		NULL,    
	},
	//遥控器无线对码
	{
		6,       //帧类型
		0,       //超时时间
		0,       //发送次数
		0,       //应答
		Wireless_CodeMatching_Proc,     
		NULL,     
		NULL,     
		NULL,    //发送前处理回调函数
	},
	//遥控器无线对码结果
	{
		7,       //帧类型
		0,       //超时时间
		0,       //发送次数
		0,       //应答
		NULL,     
		NULL,     
		NULL,     
		NULL,    //发送前处理回调函数
	},
	//控制器主动解除对码
	{
		14,       //帧类型
		60,       //超时时间
		3,       //发送次数
		0,       //应答
		NULL,     
		NULL,     
		NULL,     
		NULL,    //发送前处理回调函数
	},
	//遥控器主动解除对码应答
	{
		6,       //帧类型
		0,       //超时时间
		0,       //发送次数
		0,       //应答
		NULL,     
		NULL,     
		NULL,     
		NULL,    //发送前处理回调函数
	},
	//遥控器主动解除对码
	{
		15,       //帧类型
		0,       //超时时间
		0,       //发送次数
		0,       //应答
		Wireless_Ykq_Decode_Proc,     //接收回调函数
		NULL,     
		NULL,     
		NULL,    
	},
	//遥控器主动解除对码结果
	{
		8,       //帧类型
		60,       //超时时间
		3,       //发送次数
		0,       //应答
		NULL,     
		NULL,     
		NULL,     
		NULL,    //发送前处理回调函数
	},
	//动作控制
	{
		10,       //帧类型
		0,       //超时时间
		0,       //发送次数
		0,       //应答
		Wireless_Action_Proc,     //接收回调函数
		NULL,     
		NULL,     
		NULL,    
	},
	//动作控制应答
	{
		11,       //帧类型
		30,       //超时时间
		3,       //发送次数
		0,       //应答
		NULL,     //接收回调函数
		NULL,     
		NULL,     
		NULL,    
	},
	//读状态
	{
		11,       //帧类型
		0,       //超时时间
		0,       //发送次数
		0,       //应答
		NULL,     //接收回调函数
		NULL,     
		NULL,     
		NULL,    
	},
	//读状态应答
	{
		12,       //帧类型
		0,       //超时时间
		0,       //发送次数
		0,       //应答
		NULL,     //接收回调函数
		NULL,     
		NULL,     
		NULL,    
	},
	//参数读取请求
	{
		13,      //帧类型
		0,       //超时时间
		0,       //发送次数
		0,       //应答
		NULL,    //接收回调函数
		NULL,     
		NULL,     
		NULL,    
	},
	//参数读取请求应答
	{
		14,      //帧类型
		0,       //超时时间
		0,       //发送次数
		0,       //应答
		NULL,    //接收回调函数
		NULL,     
		NULL,     
		NULL,    
	},
	//参数写入请求
	{
		15,      //帧类型
		0,       //超时时间
		0,       //发送次数
		0,       //应答
		NULL,    //接收回调函数
		NULL,     
		NULL,     
		NULL,    
	},
	//参数写入请求应答
	{
		16,      //帧类型
		0,       //超时时间
		0,       //发送次数
		0,       //应答
		NULL,    //接收回调函数
		NULL,     
		NULL,     
		NULL,    
	},
	//遥控器文件请求
	{
		18,      //帧类型
		0,       //超时时间
		0,       //发送次数
		0,       //应答
		Wireless_Para_Request_Proc,     //接收回调函数
		NULL,     
		NULL,     
		NULL,    
	},
	//遥控器邻架
	{
		19,      //帧类型
		0,       //超时时间
		0,       //发送次数
		0,       //应答
		Wireless_Neighbor_Request_Proc,     //接收回调函数
		NULL,
		NULL,
		NULL,
	},
	//遥控器邻架应答
	{
		20,      //帧类型
		0,       //超时时间
		0,       //发送次数
		0,       //应答
		NULL,    //接收回调函数
		NULL,
		NULL,
		NULL,
	},
	
	
	//无线总线
	{
		100,     //帧类型
		0,       //超时时间
		0,       //发送次数
		0,       //应答
		Wireless_Bus_Request_Ack_Proc,     //接收回调函数
		NULL,     
		NULL,     
		NULL,    
	},
};


/*******************************************************************************************
**函数名称：WirelessSendProData
**函数作用：发送程序更新帧
**函数参数：无
**函数输出：无
**注意事项：无
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
**函数名称：GetWirelessHeadMsg
**函数作用：得到无线请求pro协议
**函数参数：无
**函数输出：无
**注意事项：无
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
**函数名称：UsartRecvWorkDataProc
**函数作用：usart接收回调处理
**函数参数：无
**函数输出：无
**注意事项：无
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
**函数名称：UsartRecvHeartDataProc
**函数作用：usart心跳处理处理
**函数参数：无
**函数输出：无
**注意事项：无
*******************************************************************************************/
void UsartRecvHeartDataProc(uint8_t *candata)
{
	if(SysParam.globalParam.SupportParam.Paring_Method == 0) //对码方式选择为串口上时
	{
		WirelessVersion[0] = candata[5];
		WirelessVersion[1] = candata[6];
		WirelessVersion[2] = candata[7];
	}
	else if(SysParam.globalParam.SupportParam.Paring_Method == 1)//对码方式选择为CAN时
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
* 功能描述： 发送本架架号
* 入口参数：
* 返回值：无
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
* 功能描述： 发送重启
* 入口参数：
* 返回值：无
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
* 功能描述： 无线总线
* 入口参数：
* 返回值：无
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
				if(SysParam.globalParam.SupportParam.Paring_Method == 0) //对码方式选择为串口上时
					SendSCAddressData();
				else if(SysParam.globalParam.SupportParam.Paring_Method == 1)//对码方式选择为CAN时
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
		   if(SysParam.globalParam.SupportParam.Paring_Method == 0) //对码方式选择为串口上时
				SendSCAddressData();
		   else if(SysParam.globalParam.SupportParam.Paring_Method == 1)//对码方式选择为CAN时
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
	if(SysParam.globalParam.SupportParam.Paring_Method ==0) //无线模块
		SendSCAddressData();
	else if(SysParam.globalParam.SupportParam.Paring_Method ==1)//声光433				
		Alarm_SendSCAddressData();		
	V4ykq.SCNum = getSupportID();
}

//uint16_t OldCrc = 0;

//接收任务
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



