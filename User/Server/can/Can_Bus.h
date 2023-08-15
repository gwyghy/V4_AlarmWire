/*
*********************************************************************************************************
*	                                            V4
*                           Commuication Controller For The Mining Industry
*
*                                  (c) Copyright 1994-2013  HNDZ 
*                       All rights reserved.  Protected by international copyright laws.
*
*
*    File    : Can_Bus.h
*    Module  : user
*    Version : V1.0
*    History :
*   -----------------
*              Version  Date           By            Note
*              v1.0         
*
*********************************************************************************************************
*/

#ifndef __CAN_BUS_H__
#define __CAN_BUS_H__


#include "NativeCan_Driver.h"

#define  CAN_DRIVER_DEV_NUM     2

#define  NATIVECAN1             0
#define  NATIVECAN2             1
#define  SPICAN1                2
#define  SPICAN2                3

#define  CAN_TX_QUEUE_NUM		100
#define  CAN_RX_QUEUE_NUM		100
#define  CAN_SENT_QUEUE_NUM		100


typedef struct
{
	uint16_t count;
	uint16_t overtime;
	uint16_t backtime;
	uint16_t ack;
	uint16_t prio;

}CANBUSHEAD;




typedef struct
{
	STR_Can_Frame        TrsFrame;
	CANBUSHEAD           busHead;
	void                 *pNext;
} stCanTrsQueue;

typedef struct
{
	STR_Can_Frame      RxFrame;
	void               *pNext;
} stCanRxQueue;

typedef struct {
	
	CAN_Driver       Cfg;
	
	stCanTrsQueue    CanLowTrs[CAN_TX_QUEUE_NUM];		      //��������
	uint16_t         u16CanLowTrsCnt;					            //������������
	stCanTrsQueue*   pCanLowTrsHead;				              //������������
	stCanTrsQueue*   pCanLowTrsEnd;					              //��������ĩ��
	
	stCanTrsQueue    CanHighTrs[CAN_TX_QUEUE_NUM];	      //�����ȼ���������
	uint16_t         u16CanHighTrsCnt;							      //�����ȼ�������������
	stCanTrsQueue*   pCanHighTrsHead;						          //�����ȼ�������������
	stCanTrsQueue*   pCanHighTrsEnd;							        //�����ȼ���������ĩ��
	
	stCanRxQueue     CanRev[CAN_RX_QUEUE_NUM];			      //���ն���
	uint16_t         u16CanRevCnt;							          //���ն�������
	stCanRxQueue*    pCanRevHead;							            //���ն�������
	stCanRxQueue*    pCanRevEnd;							            //���ն���ĩ��
																										    
	stCanTrsQueue    CanSent[CAN_SENT_QUEUE_NUM];	        //������϶���
	uint16_t         u16CanSentCnt;							          //������϶�������
	stCanTrsQueue*   pCanSentRead;						            //������϶��д�������
	stCanTrsQueue*   pCanSentWrite;							          //������϶���д������
				
} CANBUS_DATA;

typedef enum
{
	CANBUS_MSG_InsCanSentQueue,
	CANBUS_MSG_DelCanSentQueue,
	CANBUS_MSG_InsCanLowTrsQueue,
	CANBUS_MSG_DelCanLowTrsQueue,
	CANBUS_MSG_InsCanHighTrsQueue,
	CANBUS_MSG_DelCanHighTrsQueue,
	CANBUS_MSG_InsCanRevQueue,
	CANBUS_MSG_GetCanRevQueue,
	CANBUS_MSG_CheckCanSentInterval,
	
	CANBUS_MSG_MAX
}CANBUS_MSG;


uint8_t CanBusInit(uint16_t device,uint8_t baud);
uint8_t CanBusIoCtl(uint8_t busId, uint8_t func, void *argp);
// void CanTrsProc(uint16_t u16Way);
uint8_t CanBusDealFunc(uint16_t u16Way,CANBUS_MSG msg,void * pData);
uint8_t CanOpenDriver(uint16_t driver);
uint8_t CanCloseDriver(uint16_t driver);
void InsCanHighTrsQueue(uint16_t u16Way, CANBUS_DATA * pCanBus, stCanTrsQueue *SentCan);//��������ȼ����Ͷ���
void DelCanHighTrsQueue(CANBUS_DATA * pCanBus);//ɾ�������ȼ��Ͷ���;//ɾ�������ȼ��Ͷ���
void InsCanLowTrsQueue(uint16_t u16Way, CANBUS_DATA * pCanBus, stCanTrsQueue *SentCan);//��������ȼ����Ͷ���
void DelCanLowTrsQueue(CANBUS_DATA * pCanBus);//ɾ�������ȼ�����
void DelCanSentQueue(uint32_t CMD, uint16_t u16Way);//ɾ���ѷ��Ͷ���
void InsCanSentQueue(CANBUS_DATA * pCanBus, stCanTrsQueue *SentCan);//�����ѷ��Ͷ���read
void InsCanRevQueue(uint16_t u16Way, STR_Can_Frame* RevBufTemp);
void CanDriverTxDealFun(uint16_t u16Way, uint16_t res);
void CanSendTIMOUT_Callback(uint16_t u16Way);
void CanSendERR_callback(uint16_t u16Way);

#endif
