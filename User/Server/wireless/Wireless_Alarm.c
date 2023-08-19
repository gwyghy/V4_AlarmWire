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
#include "Wireless_Alarm.h"
#include "V2_Can_Pro.h"
#include "Can_Bus.h"
#include "IR_Rx.h"
#include "udp_app_proc.h"
#include "alarm.h"
#include "sys_msg_def.h"

extern uint8_t S_LiuShuiNumb;

/********************************************************************************
* ���������� ���ͱ��ܼܺ�
* ��ڲ�����
* ����ֵ����
********************************************************************************/
void Alarm_SendSCAddressData(void)
{

	stCanTrsQueue canTrsQueue;
	memset(&canTrsQueue,0,sizeof(stCanTrsQueue));
	STR_V2CAN_Frame v2frame;
	v2frame.u32ID.ID.ACK = 0;
	v2frame.u32ID.ID.FrameType = FT_WL_SET_WLALARMPARA;			
	v2frame.u32ID.ID.RxID = V2ID_WL;
	v2frame.u32ID.ID.TxID = V2ID_SC;
	v2frame.u32ID.ID.Reservd = 0;
	v2frame.u32ID.ID.Sub = 0;
	v2frame.u32ID.ID.Sum = 0;
	v2frame.u32ID.ID.LiuShuiNumb = S_LiuShuiNumb;
	S_LiuShuiNumb++;
	S_LiuShuiNumb %= 0x0F;

	v2frame.u16DLC = 3;
	
	v2frame.u8DT[0] =  getSupportID();
	v2frame.u8DT[1] =  0x33;
	v2frame.u8DT[2] =  0x33;
	memcpy(&canTrsQueue.TrsFrame,&v2frame,sizeof(STR_Can_Frame));
    CanBusDealFunc(NATIVECAN1,CANBUS_MSG_InsCanHighTrsQueue,&canTrsQueue);
	
}
/********************************************************************************
* ���������� ���ý��շ���ַ
* ��ڲ�����
* ����ֵ����
********************************************************************************/
void  Alarm_SetSCAddressData(u8 RecvAdress)
{
	stCanTrsQueue canTrsQueue;
	memset(&canTrsQueue,0,sizeof(stCanTrsQueue));
	STR_V2CAN_Frame v2frame;
	v2frame.u32ID.ID.ACK = 0;
	v2frame.u32ID.ID.FrameType = FT_SC_TO_WL_SET_RECEIVER_ADD;			
	v2frame.u32ID.ID.RxID = V2ID_WL;
	v2frame.u32ID.ID.TxID = V2ID_SC;
	v2frame.u32ID.ID.Reservd = 0;
	v2frame.u32ID.ID.Sub = 0;
	v2frame.u32ID.ID.Sum = 0;
	v2frame.u32ID.ID.LiuShuiNumb = S_LiuShuiNumb;
	S_LiuShuiNumb++;
	S_LiuShuiNumb %= 0x0F;

	v2frame.u16DLC = 1;
	
	v2frame.u8DT[0] =  RecvAdress;
	memcpy(&canTrsQueue.TrsFrame,&v2frame,sizeof(STR_Can_Frame));
    CanBusDealFunc(NATIVECAN1,CANBUS_MSG_InsCanHighTrsQueue,&canTrsQueue);
}

/*******************************************************************************************
**�������ƣ�InsAlarmWirelessIrRecvData_proc
**�������ã��������CAN����
**������������
**�����������
**ע�������
*******************************************************************************************/
void InsAlarmWirelessIrRecvData_proc(STR_WLCAN_Frame * data,uint8_t len)
{
	STR_Usart_Frame frame;
	if(data->u32ID.ID.FrameType == FT_WLBEAT && data->u8DT[0] == 'W')
	{  
		if(data->u8DT[4] == 0)
			frame.FrameType = WIRELESS_HEART;
		else if(data->u8DT[4] == 1)
			frame.FrameType = WIRELESS_STARTHINT;
	}				
	else
	{
		frame.FrameType = WIRELESS_DATA;
	}
	memcpy(frame.SendBuf,data,len);	
	InsUsartRevQueue(0,&frame);
}
/*******************************************************************************************
**�������ƣ�InsSendWirelessAlarmDataProc
**�������ã��������뷢�Ͷ���
**������������    
**�����������
**ע�������
*******************************************************************************************/
void InsSendWirelessAlarmDataProc(STR_WLCAN_Frame* frame,uint8_t addr)
{
	 u8 u8i;
	 stCanTrsQueue canTrsQueue;
	 memset(&canTrsQueue,0,sizeof(stCanTrsQueue));
	 canTrsQueue.TrsFrame.ID = frame->u32ID.u32Id;
	 canTrsQueue.TrsFrame.DLC = frame->u16DLC;       
     memcpy(canTrsQueue.TrsFrame.Data,frame->u8DT,sizeof(frame->u8DT));
     CanBusDealFunc(NATIVECAN1,CANBUS_MSG_InsCanHighTrsQueue,&canTrsQueue); 
	 
 }                   
/*******************************************************************************************
**�������ƣ�WirelessAlarm_CodeMatching_Proc
**�������ã��ӵ���������
**������������
**�����������
**ע�������
*******************************************************************************************/
uint16_t  WirelessAlarm_CodeMatching_Proc(void *pdata , STR_Can_Frame* frame)
{
    // ��frameָ��ת��ΪSTR_WLCAN_Frame*���ͣ�����һ��
    STR_WLCAN_Frame wl_frame;
	wl_frame.u32ID.u32Id = frame->ID;
	wl_frame.u16DLC = frame->DLC;
    memcpy(wl_frame.u8DT, frame->Data, sizeof(frame->Data));  // ���ݿ�������
    InsUsartIrRecvData_proc(&wl_frame, sizeof(STR_WLCAN_Frame));
}
/*******************************************************************************************
**�������ƣ�WirelessAlarm_KeyProc
**�������ã��ӵ���������
**������������
**�����������
**ע�������
*******************************************************************************************/
uint16_t  WirelessAlarm_KeyProc(void *pdata , STR_Can_Frame* frame)
{
	// ��frameָ��ת��ΪSTR_WLCAN_Frame*���ͣ�����һ��
    STR_WLCAN_Frame wl_frame;
	wl_frame.u32ID.u32Id = frame->ID;
	wl_frame.u16DLC = frame->DLC;
    memcpy(wl_frame.u8DT, frame->Data, sizeof(frame->Data));  // ���ݿ�������
    InsUsartIrRecvData_proc(&wl_frame, sizeof(STR_WLCAN_Frame));
}  

/*******************************************************************************************
**�������ƣ�WirelessAlarm_PassiveDecodePro     
**�������ã�ң������������������
**������������
**�����������
**ע�������
*******************************************************************************************/

uint16_t  WirelessAlarm_PassiveDecodePro(void *pdata , STR_Can_Frame* frame)
{
	// ��frameָ��ת��ΪSTR_WLCAN_Frame*���ͣ�����һ��
    STR_WLCAN_Frame wl_frame;
	wl_frame.u32ID.u32Id = frame->ID;
	wl_frame.u16DLC = frame->DLC;
    memcpy(wl_frame.u8DT, frame->Data, sizeof(frame->Data));  // ���ݿ�������
    InsUsartIrRecvData_proc(&wl_frame, sizeof(STR_WLCAN_Frame));
} 

/*******************************************************************************************
**�������ƣ�WirelessAlarm_Neighbor_Request_Proc    
**�������ã�ң�����ڼܿ�������
**������������
**�����������
**ע�������
*******************************************************************************************/
uint16_t  WirelessAlarm_Neighbor_Request_Proc(void *pdata , STR_Can_Frame* frame)
{
	// ��frameָ��ת��ΪSTR_WLCAN_Frame*���ͣ�����һ��
    STR_WLCAN_Frame wl_frame;
	wl_frame.u32ID.u32Id = frame->ID;
	wl_frame.u16DLC = frame->DLC;
    memcpy(wl_frame.u8DT, frame->Data, sizeof(frame->Data));  // ���ݿ�������
    InsUsartIrRecvData_proc(&wl_frame, sizeof(STR_WLCAN_Frame));
}
/*******************************************************************************************
**�������ƣ�WirelessAlarm_Para_Request_Proc    
**�������ã�ң���������ļ�����
**������������
**�����������
**ע�������
*******************************************************************************************/
uint16_t  WirelessAlarm_Para_Request_Proc(void *pdata , STR_Can_Frame* frame)
{
	 // ��frameָ��ת��ΪSTR_WLCAN_Frame*���ͣ�����һ��
    STR_WLCAN_Frame wl_frame;
	wl_frame.u32ID.u32Id = frame->ID;
	wl_frame.u16DLC = frame->DLC;
    memcpy(wl_frame.u8DT, frame->Data, sizeof(frame->Data));  // ���ݿ�������
    InsUsartIrRecvData_proc(&wl_frame, sizeof(STR_WLCAN_Frame));	
}
