/*
*********************************************************************************************************
*	                                            V4
*                           Commuication Controller For The Mining Industry
*
*                                  (c) Copyright 1994-2013  HNDZ 
*                       All rights reserved.  Protected by international copyright laws.
*
*
*    File    : V2_Can_Pro.h
*    Module  : user
*    Version : V1.0
*    History :
*   -----------------
*              Version  Date           By            Note
*              v1.0         
*
*********************************************************************************************************
*/

#ifndef __V2_CAN_PRO_H__
#define __V2_CAN_PRO_H__

#include "Can_Pro.h"
#include "Can_Bus.h"



//�豸RID,TID
enum
{
	V2ID_SC = 0,						  //֧�ܿ�����
	V2ID_EMVD = 1,						//��ŷ�������1
	V2ID_HUB =2,					    //HUB����� ghy
	V2ID_WL = 3,						//�������
	V2ID_CXB = 4,						//�����2
	V2ID_ANGLE = 5,				 //�Ƕȴ�����5
	V2ID_HIGH = 5,            	//�߶�4
	V2ID_SS = 6,				//֧�ܷ�����5
	V2ID_ALARM = 7,		      	//���ⱨ����6
	V2ID_PERSON = 7,          //��Ա7
	V2ID_DEVICEMAX
};

enum
{
	V2_ANGLE_HEART = 0x30,
	V2_HIGH_HEART = 0x48,
	V2_HIGHBL_HEART = 0x70,
	V2_ALARM_HEART = 0x30,
	V2_ALARMBL_HEART = 0x60,
	V2_PERSON_HEART = 0x31,
	V2_PERSONBL_HEART = 0x61
};

enum 
{
	HUB1 =0x00,
	HUB2 =0x01,
	HUB3 =0x02
};


typedef struct
{
	uint8_t  forwardTimes;
	uint8_t  workMode;
	uint16_t interval;
	uint8_t  RxID;
	uint8_t  angleVariation;
	uint8_t  RxSubID;
	uint8_t  reserve;
}
stAngleParam;


typedef struct
{
	uint16_t scNum;//֧�ܱ��
	union{
		struct{			
			unsigned int  workMode       :1;  //1�����ϱ�
			unsigned int  calcMode       :1;  //1ˮƽ����
			unsigned int	disVariation   :2;	//0���仯����20cm
			unsigned int	trsPower       :4;	//���͹���

        } WORKPARAM;
		uint8_t u8workparam;
	} u8WORKPARAM;
	uint8_t	     interval;		//�ϱ����0.1s	          
	uint16_t	     range;		  //��ⷶΧcm
	uint16_t	     scHigh;	  //֧�ܸ߶�
} 
stPersonParam;


/**V2�����can֡��ʽ***/
__packed typedef struct
{
	__packed union
	{
		__packed struct
		{
			unsigned int	RxID:3;				//���շ�ID
			unsigned int	FrameType:3;				//���ͷ�ID
			unsigned int	PacketNum:20;		//֡���	
			unsigned int	ACK:1;				//Ӧ���־��1:Ӧ��0:����Ӧ��			
			unsigned int	Reservd:2;			//Ԥ���������ڴ�����:01,����:00	
			unsigned int	NoUsed:3;			//����������
		} ID;//֡ID
		uint32_t u32Id;//֡ID
	} u32ID;	//֡ID
	uint8_t	u8DT[8];			//֡����
	uint16_t	u16DLC;			//֡����
} STR_V2CXBCAN_Frame;


enum
{
	V2_HIGH_HEARTTIMER = 90,
	HIGH_REPORT_TIMER = 3000,
	HIGH_REPORT_VALUE = 2
};





enum
{
	V2_ANGLE_HEARTTIMER = 90,
	ANGLE_REPORT = 0x100,
	ANGLE_CHECK = 0x101,
	ANGLE_SET = 0x105,
	ANGLE_REPORT_TIMER = 3000,
	ANGLE_REPORT_VALUE = 10,
};


enum
{
	V2_HUB_HEARTTIMER = 90,
};

enum
{
	V2_EMVD_HEARTTIMER = 15,
	EMVD_ACTION_1 = 0x01,
	EMVD_CHECK_1 = 0x02,
	EMVD_ACTION_2 = 0x11,
	EMVD_CHECK_2 = 0x12,
};



enum
{
	V2_IR_HEARTTIMER = 15,
};


enum
{
	V2_PERSON_HEARTTIMER = 50,
	V2_ALARM_HEARTTIMER = 50,
	PERSON_REPORT = 0x01,
	PERSON_CHECK = 0x02,
	PERSON_SET  = 0x03,
	ALARM_WARN = 0X04,
	ALARM_ACTION = 0X05,
	ALARM_RESET = 0X06,
};

enum
{
	HEARTONCE = 0,
	HEARTPERIOD = 1,
};

enum
{
	DEVICEUNENABLE = 0,
	DEVICEENABLE = 1,
};

enum
{
	DEVICEOFFLINE = 0,
	DEVICEONLINE = 1,
};


enum
{
	V2_CXB_HEARTTIMER = 250,
	SC_CXB_REQUEST_PROGRAM = 0x00,
	CXB_SEND_DEITION = 0x01,
	CXB_SEND_PROGRAM = 0X02,
	CXB_HEART = 0X04,
	CXB_SEND_DEITION_CRC = 0x06,
	CXB_SEND_PROGRAM_CRC = 0X07,
};


void v2AngleandPersonInit(void);
uint8_t getV2ProDeviceMsg(uint8_t device, uint16_t frameid, CANPROHEAD* head);
void V2CanRevTimeoutProc(STR_V2CAN_Frame* frame);
uint8_t CanRecIsAck_V2(STR_Can_Frame* frame);
uint8_t V2CanProSetParam(uint16_t device, void * param,uint8_t isNow);
uint8_t V2CanProSendParam(uint16_t device, uint16_t subdevice, STR_Can_Frame* frame, CANPROHEAD* canpro);
void CanFrameDeal_V2(uint16_t u16Way, STR_V2CAN_Frame* RevBufTemp);
uint8_t V2Pro_action_prohead(uint16_t driver,uint16_t device, uint16_t childDev, uint32_t point, uint32_t status, uint8_t flag, STR_Can_Frame* frame, CANBUSHEAD* canbus);
void V2Pro_cxb_prohead(uint16_t device, uint32_t point, uint8_t *data, uint8_t len,uint32_t packetNum, STR_Can_Frame* frame, CANBUSHEAD* canbus);

#if 0
void Key_task();
#endif
#endif
