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
	
	stCanTrsQueue    CanLowTrs[CAN_TX_QUEUE_NUM];		      //发送链表
	uint16_t         u16CanLowTrsCnt;					            //发送链表项数
	stCanTrsQueue*   pCanLowTrsHead;				              //发送链表首项
	stCanTrsQueue*   pCanLowTrsEnd;					              //发送链表末项
	
	stCanTrsQueue    CanHighTrs[CAN_TX_QUEUE_NUM];	      //高优先级发送链表
	uint16_t         u16CanHighTrsCnt;							      //高优先级发送链表项数
	stCanTrsQueue*   pCanHighTrsHead;						          //高优先级发送链表首项
	stCanTrsQueue*   pCanHighTrsEnd;							        //高优先级发送链表末项
	
	stCanRxQueue     CanRev[CAN_RX_QUEUE_NUM];			      //接收队列
	uint16_t         u16CanRevCnt;							          //接收队列项数
	stCanRxQueue*    pCanRevHead;							            //接收队列首项
	stCanRxQueue*    pCanRevEnd;							            //接收队列末项
																										    
	stCanTrsQueue    CanSent[CAN_SENT_QUEUE_NUM];	        //发送完毕队列
	uint16_t         u16CanSentCnt;							          //发送完毕队列项数
	stCanTrsQueue*   pCanSentRead;						            //发送完毕队列处理链表
	stCanTrsQueue*   pCanSentWrite;							          //发送完毕队列写入链表
				
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
void InsCanHighTrsQueue(uint16_t u16Way, CANBUS_DATA * pCanBus, stCanTrsQueue *SentCan);//加入高优先级发送队列
void DelCanHighTrsQueue(CANBUS_DATA * pCanBus);//删除高优先级送队列;//删除高优先级送队列
void InsCanLowTrsQueue(uint16_t u16Way, CANBUS_DATA * pCanBus, stCanTrsQueue *SentCan);//加入低优先级发送队列
void DelCanLowTrsQueue(CANBUS_DATA * pCanBus);//删除低优先级队列
void DelCanSentQueue(uint32_t CMD, uint16_t u16Way);//删除已发送队列
void InsCanSentQueue(CANBUS_DATA * pCanBus, stCanTrsQueue *SentCan);//加入已发送队列read
void InsCanRevQueue(uint16_t u16Way, STR_Can_Frame* RevBufTemp);
void CanDriverTxDealFun(uint16_t u16Way, uint16_t res);
void CanSendTIMOUT_Callback(uint16_t u16Way);
void CanSendERR_callback(uint16_t u16Way);

#endif
