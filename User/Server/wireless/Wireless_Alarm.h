 /*
*********************************************************************************************************
*	                                            V4
*                           Commuication Controller For The Mining Industry
*
*                                  (c) Copyright 1994-2013  HNDZ 
*                       All rights reserved.  Protected by international copyright laws.
*
*
*    File    : Wireless_433Can.h
*    Module  : user
*    Version : V1.0
*    History :
*   -----------------
*              Version  Date           By            Note
*              v1.0         
*
*********************************************************************************************************
*/
#ifndef __WIRELESS_ALARM_H__
#define __WIRELESS_ALARM_H__

#include "V4_Wireless.h"
#include "includes.h"
#include "Wireless_Bus.h"


//扩展CAN数据帧分类：WL
enum
{
	FT_WLBEAT=0,					//心跳数据
	FT_MINE_POS=1,					//查询红外煤机位置数据
	FT_WL_TX,						//无线总线发送数据
	FT_WL_RX,						//无线总线接收数据
	FT_REMOTE_CHECK_IR,				//遥控器红外对码
	FT_REMOTE_CHECK_IR_RESULT,		//遥控器红外对码结果
	FT_REMOTE_CHECK_WL,				//遥控器无线对码
	FT_REMOTE_CHECK_WL_RESULT,		//遥控器无线对码结果
	FT_REMOTE_GROUP_INFO,			//成组信息
	FT_REMOTE_SENSOR_DATA,			//传感器数据
	FT_REMOTE_KEY_PRESSED,			//遥控器控制键第一次按下
	FT_REMOTE_KEY_PRESSED_PERMIT,	//遥控器控制键第一次按下,是否允许该键
	FT_REMOTE_KEY_LONG_PRESSED,		//遥控器控制键连续按下
	FT_REMOTE_KEY_RELEASE,			//遥控器控制键抬起
	FT_REMOTE_MATCH_RELEASE_FROM_SC,//解除对码
	FT_REMOTE_MATCH_RELEASE_FROM_WL,//解除对码
	FT_REMOTE_SET_PARA,				//向遥控器发送某些必须的参数
	FT_REMOTE_KEY_RELEASE_PERMIT,	//遥控器控制键第一次按下后释放,是否允许该键（只针对邻隔架动作控制有效）
	FT_REMOTE_ADD_PRS_EN,			//遥控器控制自动补压开启
	FT_REMOTE_ADD_PRS_EN_PERMIT,	//遥控器控制自动补压开启应答
	FT_REMOTE_ADD_PRS_DIS,			//遥控器控制自动补压关闭
	FT_REMOTE_ADD_PRS_DIS_PERMIT,	//遥控器控制自动补压关闭应答
	FT_WL_SET_WLALARMPARA = 23,			//设置声光433无线模块参数
	FT_SC_TO_WL_SET_RECEIVER_ADD = 24,  //设置遥控器发送方的ID
	FT_WL_MAX,						//最后一项
	FT_WL_SET_PARA=30,				//设置无线红外模块参数
	FT_WL_MATCH_END=31,				//通知无线红外模块对码结束
	
// 	FT_GENERAL_CODE_REVD_REPORT,	//全适用。程序接收报告
// 	FT_GENERAL_CODE_REFRESH_REPORT,	//全适用。程序更新报告
// 	FT_GENERAL_INQUIRE_VERSION,		//全适用。查询版本号
// 	FT_GENERAL_REPORT_VERSION		//全适用。报告版本号
};


void Alarm_SendSCAddressData(void);
void  Alarm_SetSCAddressData(u8 RecvAdress);
void InsAlarmWirelessIrRecvData_proc(STR_WLCAN_Frame * data,uint8_t len);
uint16_t WirelessAlarm_CodeMatching_Proc(void *pdata , STR_Can_Frame* frame);
uint16_t  WirelessAlarm_KeyProc(void *pdata , STR_Can_Frame* frame);
void InsSendWirelessAlarmDataProc(STR_WLCAN_Frame* frame,uint8_t addr);
uint16_t  WirelessAlarm_PassiveDecodePro(void *pdata , STR_Can_Frame* frame);
uint16_t  WirelessAlarm_Neighbor_Request_Proc(void *pdata , STR_Can_Frame* frame);
uint16_t  WirelessAlarm_Para_Request_Proc(void *pdata , STR_Can_Frame* frame);
#endif