/*
*********************************************************************************************************
*	                                            DoubleWires
*                           Commuication Controller For The Mining Industry
*
*                                  (c) Copyright 1994-2013  HNDZ 
*                       All rights reserved.  Protected by international copyright laws.
*
*
*    File    : sys_msg_def.h
*    Module  : system function
*    Version : V1.0
*    History :
*   -----------------
*              系统所用的消息命令字定义
*              Version  Date           By            Note
*              v1.0         
*
*********************************************************************************************************
*/

#ifndef _SYS_MSG_DEF_H_
#define _SYS_MSG_DEF_H_

#include "sys_data_type.h"
#include "sys_param_type.h"
//****************************************************************//
//**************************eeprom 分配***************************//
//****************************************************************//
#define  FIRST_TURNON_START     0x0000
#define  FIRST_TURNON_LEN		4
#define  NET_PARAM_START		(FIRST_TURNON_START+FIRST_TURNON_LEN)   //4
#define  JSON_PARAM_DEFAULT_START   (NET_PARAM_START+sizeof(NetParamType))//4+44 = 48
#define  JSON_PARAM_USING_START     (JSON_PARAM_DEFAULT_START+sizeof(NetParamType)+30)//48+44+30 = 122
#define  FILE_RECV_COMPLETE_START   (JSON_PARAM_USING_START+sizeof(NetParamType)+30)// 122+44+30 = 196
	
/****************下一个开始196+30 = 226**************/
//json文件接收成功标志，占一个字节，0xFF表示失败；0x00表示接收json成功；0x01表示读取json死机
#define  JSON_FILE_RECEPTION_SUCCESS     (FILE_RECV_COMPLETE_START+sizeof(NetParamType)+30) 
//上次json文件crc，占两个字节，用来保存上次收到json文件的CRC数值
#define  LAST_JSON_FILE_CRC           JSON_FILE_RECEPTION_SUCCESS+1  
//启动方式，占一个字节     0xff：内部json启动      0x00：外部json启动
#define  JSON_STARTUP_MODE           LAST_JSON_FILE_CRC+2

//系统由测试程序->用户程序的第一次上电,需要初始化网络IP，网关和子网掩码
#define  KZQ_FIRST_TURNON     JSON_STARTUP_MODE+4
//验证eeprom读出数据是否正确
#define  EEPROM_TURE        KZQ_FIRST_TURNON+4
#define  EEPROM_TURE_LEN       5
//死机次数
#define  CRASH_TIMES     EEPROM_TURE+  EEPROM_TURE_LEN

/**************0x999已经被HomeTask.c占用*************/
#define  SYS_PARAM_START		0x1000

/**
* @ DoubleWires功能命令字
*/
typedef enum 
{	
	SYS_CMD_MAX
}SysCmd_t;


typedef enum 
{		
	DATA_MSG_READ_OBJECT,
	DATA_MSG_WRITE_OBJECT,
	DATA_MSG_READ_OBJECT_PARAM,
	DATA_MSG_WRITE_OBJECT_PARAM,
	DATA_MSG_CTRL,  //length start at "id"
	DATA_MSG_CTRL_ALARM,
	DATA_MSG_GetParamCRC,
	DATA_MSG_SetParamCRCResult, //data[0] 0:crc error 1:right ;data[1] addr
	DATA_MSG_SetSysTime,
	DATA_MSG_SetCommCheckState, //data[0] 0:error 1:right;
	                            //data[1] 0:left 1:right
	DATA_MSG_SetCommCheckGlobalCRC,//data[0] 0:error 1:right;
	                               //data[1] 0:left 1:right	
	DATA_MSG_SetCommCheckGlobalVERSION,//data[0] 0:error 1:right;
	DATA_MSG_GetActionInfo,
	DATA_MSG_GetStopInfo,
	DATA_MSG_SetStopPress,
	DATA_MSG_COPY_FROM_FLASH,
	DATA_MSG_COPY_TO_FLASH,
	DATA_MSG_GET_IP_PARAM,
	DATA_MSG_SET_IP_PARAM_GLOBAL,
	DATA_MSG_SET_IP_PARAM,
	DATA_MSG_SET_VLAN_PARAM,
	DATA_MSG_GET_DEVICETYPE_PARAM,//0:工作面支架 1：超前架 2：自移机尾
	DATA_MSG_SET_DEVICETYPE_PARAM,
	DATA_MSG_SET_LeftSelfID_PARAM,
	DATA_MSG_SET_RightSelfID_PARAM,
	
	DATA_MSG_GET_GLOBAL_PARAM,
	DATA_MSG_SET_GLOBAL_PARAM,
	DATA_MSG_SET_ALL_PARAM,
	DATA_MSG_RESET_CONTROLLER,      //复位控制器
	DATA_MSG_RESET_ALL_CONTROLLER,  //复位所有控制器
	DATA_MSG_RESET_DEFAULTPARAM,    //恢复出厂默认参数
	DATA_MSG_RESET_ALL_DEFAULTPARAM,//恢复出厂默认参数,所有	
	DATA_MSG_MAX
}DataMsg_t;

typedef enum 
{		
	DEVDATA_MSG_STATE_ACTION =0,
	DEVDATA_MSG_STATE_EMERGENCY,
	DEVDATA_MSG_STATE_BISUO,
	DEVDATA_MSG_STATE_STOP,
	DEVDATA_MSG_STATE_PERSONBISUO,
	DEVDATA_MSG_STATE_FAIL,
	DEVDATA_MSG_STATE_SENSOR,
	DEVDATA_MSG_STATE_DEVICE,
	DEVDATA_MSG_STATE_PERSON,
	DEVDATA_MSG_STATE_SYSTEM,
	DEVDATA_MSG_STATE_PARA_DIF,
	DEVDATA_MSG_OBJECT,
	DEVDATA_MSG_PARAM,
	DEVDATA_MSG_MAX
}DevDataMsg_t;


extern SysParamType     SysParam;
extern SysStateType	 	SysState;
extern NetParamType	 	SysNetParam;
extern SysStopType		SysStopState[256];
extern u8				ManualActionClear[MANUALACTIONINFO_LEN];
extern u8       ManualActionSupportNum;
extern u16      kzqTurnonReson;


int SysDataFun(DataMsg_t msg, u8 Type,u8 Id,u16 Length,u16 Offset,void *pdata,u16 *retLen);       ///< 参数读取及设置
int DeviceMngDataFun(DevDataMsg_t msg,u8 supportAddr, u8 Type,u8 Id,u16 Length,u16 Offset,void *pdata);
int getActionParam(u8 combAction_num,u8 actionID,SigActionParamType **pactionParam);
int getActionParam_single(u8 actionID,SigActionParamType **pactionParam);
u8 getSysServerAddr(void);
void InsCtrlBufQueue(SysCtrlInfoType *pdata);
int SysParamFun(DataMsg_t msg, uint64_t levels,u16 Length,u16 Offset,void *pdata,u16 *retLen,u8 globalFlag,u8 Report);
void SysEmergencyStateUpdate(void);
void AddActionInfo(u8 type,u8 id,u8 runState);
u8 CheckManualActionList(u8 *pactionList);
void AddManualActionInfo(u8 type,u8 id,u8 runState);
u8  getManualActionInfoBufSzie(void);
ActionInfoType * getActionInfoHead(void);
u8  getActionInfoBufSzie(void);
uint8_t Is_FirstTurnON(void);
void Write_FirstFlag(void);
u8 ParamProcess(u8 Type,u8 Id,u16 offset,u16 Len);
void SetSysFuncMode(u8 mode);
u8 GetSysFuncMode(void);
int SysDataFun_ext(DataMsg_t msg, u32 Length,void *pdata,u32 *retLen);
void NetworkHeartbeatTimesZero(void);
uint8_t getSeverStatus(void);
#endif
