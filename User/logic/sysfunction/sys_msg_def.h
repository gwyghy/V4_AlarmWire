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
*              ϵͳ���õ���Ϣ�����ֶ���
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
//**************************eeprom ����***************************//
//****************************************************************//
#define  FIRST_TURNON_START     0x0000
#define  FIRST_TURNON_LEN		4
#define  NET_PARAM_START		(FIRST_TURNON_START+FIRST_TURNON_LEN)   //4
#define  JSON_PARAM_DEFAULT_START   (NET_PARAM_START+sizeof(NetParamType))//4+44 = 48
#define  JSON_PARAM_USING_START     (JSON_PARAM_DEFAULT_START+sizeof(NetParamType)+30)//48+44+30 = 122
#define  FILE_RECV_COMPLETE_START   (JSON_PARAM_USING_START+sizeof(NetParamType)+30)// 122+44+30 = 196
	
/****************��һ����ʼ196+30 = 226**************/
//json�ļ����ճɹ���־��ռһ���ֽڣ�0xFF��ʾʧ�ܣ�0x00��ʾ����json�ɹ���0x01��ʾ��ȡjson����
#define  JSON_FILE_RECEPTION_SUCCESS     (FILE_RECV_COMPLETE_START+sizeof(NetParamType)+30) 
//�ϴ�json�ļ�crc��ռ�����ֽڣ����������ϴ��յ�json�ļ���CRC��ֵ
#define  LAST_JSON_FILE_CRC           JSON_FILE_RECEPTION_SUCCESS+1  
//������ʽ��ռһ���ֽ�     0xff���ڲ�json����      0x00���ⲿjson����
#define  JSON_STARTUP_MODE           LAST_JSON_FILE_CRC+2

//ϵͳ�ɲ��Գ���->�û�����ĵ�һ���ϵ�,��Ҫ��ʼ������IP�����غ���������
#define  KZQ_FIRST_TURNON     JSON_STARTUP_MODE+4
//��֤eeprom���������Ƿ���ȷ
#define  EEPROM_TURE        KZQ_FIRST_TURNON+4
#define  EEPROM_TURE_LEN       5
//��������
#define  CRASH_TIMES     EEPROM_TURE+  EEPROM_TURE_LEN

/**************0x999�Ѿ���HomeTask.cռ��*************/
#define  SYS_PARAM_START		0x1000

/**
* @ DoubleWires����������
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
	DATA_MSG_GET_DEVICETYPE_PARAM,//0:������֧�� 1����ǰ�� 2�����ƻ�β
	DATA_MSG_SET_DEVICETYPE_PARAM,
	DATA_MSG_SET_LeftSelfID_PARAM,
	DATA_MSG_SET_RightSelfID_PARAM,
	
	DATA_MSG_GET_GLOBAL_PARAM,
	DATA_MSG_SET_GLOBAL_PARAM,
	DATA_MSG_SET_ALL_PARAM,
	DATA_MSG_RESET_CONTROLLER,      //��λ������
	DATA_MSG_RESET_ALL_CONTROLLER,  //��λ���п�����
	DATA_MSG_RESET_DEFAULTPARAM,    //�ָ�����Ĭ�ϲ���
	DATA_MSG_RESET_ALL_DEFAULTPARAM,//�ָ�����Ĭ�ϲ���,����	
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


int SysDataFun(DataMsg_t msg, u8 Type,u8 Id,u16 Length,u16 Offset,void *pdata,u16 *retLen);       ///< ������ȡ������
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
