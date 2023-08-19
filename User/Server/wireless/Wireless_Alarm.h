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


//��չCAN����֡���ࣺWL
enum
{
	FT_WLBEAT=0,					//��������
	FT_MINE_POS=1,					//��ѯ����ú��λ������
	FT_WL_TX,						//�������߷�������
	FT_WL_RX,						//�������߽�������
	FT_REMOTE_CHECK_IR,				//ң�����������
	FT_REMOTE_CHECK_IR_RESULT,		//ң�������������
	FT_REMOTE_CHECK_WL,				//ң�������߶���
	FT_REMOTE_CHECK_WL_RESULT,		//ң�������߶�����
	FT_REMOTE_GROUP_INFO,			//������Ϣ
	FT_REMOTE_SENSOR_DATA,			//����������
	FT_REMOTE_KEY_PRESSED,			//ң�������Ƽ���һ�ΰ���
	FT_REMOTE_KEY_PRESSED_PERMIT,	//ң�������Ƽ���һ�ΰ���,�Ƿ�����ü�
	FT_REMOTE_KEY_LONG_PRESSED,		//ң�������Ƽ���������
	FT_REMOTE_KEY_RELEASE,			//ң�������Ƽ�̧��
	FT_REMOTE_MATCH_RELEASE_FROM_SC,//�������
	FT_REMOTE_MATCH_RELEASE_FROM_WL,//�������
	FT_REMOTE_SET_PARA,				//��ң��������ĳЩ����Ĳ���
	FT_REMOTE_KEY_RELEASE_PERMIT,	//ң�������Ƽ���һ�ΰ��º��ͷ�,�Ƿ�����ü���ֻ����ڸ��ܶ���������Ч��
	FT_REMOTE_ADD_PRS_EN,			//ң���������Զ���ѹ����
	FT_REMOTE_ADD_PRS_EN_PERMIT,	//ң���������Զ���ѹ����Ӧ��
	FT_REMOTE_ADD_PRS_DIS,			//ң���������Զ���ѹ�ر�
	FT_REMOTE_ADD_PRS_DIS_PERMIT,	//ң���������Զ���ѹ�ر�Ӧ��
	FT_WL_SET_WLALARMPARA = 23,			//��������433����ģ�����
	FT_SC_TO_WL_SET_RECEIVER_ADD = 24,  //����ң�������ͷ���ID
	FT_WL_MAX,						//���һ��
	FT_WL_SET_PARA=30,				//�������ߺ���ģ�����
	FT_WL_MATCH_END=31,				//֪ͨ���ߺ���ģ��������
	
// 	FT_GENERAL_CODE_REVD_REPORT,	//ȫ���á�������ձ���
// 	FT_GENERAL_CODE_REFRESH_REPORT,	//ȫ���á�������±���
// 	FT_GENERAL_INQUIRE_VERSION,		//ȫ���á���ѯ�汾��
// 	FT_GENERAL_REPORT_VERSION		//ȫ���á�����汾��
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