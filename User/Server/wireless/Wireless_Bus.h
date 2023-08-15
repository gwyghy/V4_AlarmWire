/*
*********************************************************************************************************
*	                                            V4
*                           Commuication Controller For The Mining Industry
*
*                                  (c) Copyright 1994-2013  HNDZ 
*                       All rights reserved.  Protected by international copyright laws.
*
*
*    File    : Wireless_Bus.h
*    Module  : user
*    Version : V1.0
*    History :
*   -----------------
*              Version  Date           By            Note
*              v1.0         
*
*********************************************************************************************************
*/

#ifndef __WIRELESS_BUS_H__
#define __WIRELESS_BUS_H__


#include "NativeCan_Driver.h"
#include "Wireless_Driver.h"

#define  USART_DRIVER_DEV_NUM     1

#define  NATIVEUSART1             0

#define  USART_TX_QUEUE_NUM		20
#define  USART_RX_QUEUE_NUM		20
#define  USART_SENT_QUEUE_NUM		20


typedef struct
{
	uint16_t count;
	uint16_t overtime;
	uint16_t backtime;
	uint16_t ack;

}USARTBUSHEAD;


/**V2外围设备can帧格式***/
__packed typedef struct
{
	__packed union
	{
		__packed struct
		{
			unsigned int	RxID:3;				//接收方ID
			unsigned int	TxID:3;				//发送方ID
			unsigned int	FrameType:10;		//帧类别	
			unsigned int	Sum:4;		//子帧序号或
			unsigned int	LiuShuiNumb:5;		//流水序列号，0x0-0xf循环，ACK=1应答帧的流水，采用被应答帧的流水
			unsigned int	Sub:1;		//组合帧/子帧，0：组合帧总数帧：1：子帧
			unsigned int	ACK:1;				//应答标志，1:应答，0:无需应答			
			unsigned int	Reservd:2;			//预留区域。用于传程序:01,其他:00	
			unsigned int	NoUsed:3;			//不存在区域
		} ID;//帧ID
		uint32_t u32Id;//帧ID
	} u32ID;	//帧ID
	uint8_t	u16DLC;			//帧长度
	uint8_t	u8DT[8];			//帧数据

} 
STR_WLCAN_Frame;


typedef struct
{
	STR_Usart_Frame      UsartData;
	USARTBUSHEAD         busHead;
	void                 *pNext;
} stUsartTrsQueue;

typedef struct
{
	STR_Usart_Frame     UsartData;
	void               *pNext;
} stUsartRxQueue;


typedef struct {
	void        (*Init)(uint32_t);
	void        (*Open)(uint16_t);
	void        (*Close)(uint16_t);
	int16_t    (*Read)(uint16_t , STR_Usart_Frame* );
	int16_t    (*Write)(uint16_t , STR_Usart_Frame* );
	uint16_t    (*IoCtl)(uint16_t , uint8_t , void *);
} USART_Driver;

typedef struct {
	
	USART_Driver       Cfg;
	
	
	stUsartTrsQueue    UsartTrs[USART_TX_QUEUE_NUM];	      //高优先级发送链表
	uint16_t           u16UsartTrsCnt;							      //高优先级发送链表项数
	stUsartTrsQueue*   pUsartTrsHead;						          //高优先级发送链表首项
	stUsartTrsQueue*   pUsartTrsEnd;							        //高优先级发送链表末项
	
	
	stUsartRxQueue     UsartRev[USART_RX_QUEUE_NUM];			      //接收队列
	uint16_t           u16UsartRevCnt;							          //接收队列项数
	stUsartRxQueue*    pUsartRevHead;							            //接收队列首项
	stUsartRxQueue*    pUsartRevEnd;							            //接收队列末项
	
																										    
	stUsartTrsQueue    UsartSent[USART_SENT_QUEUE_NUM];	        //发送完毕队列
	uint16_t         	 u16UsartSentCnt;							          //发送完毕队列项数
	stUsartTrsQueue*   pUsartSentRead;						            //发送完毕队列处理链表
	stUsartTrsQueue*   pUsartSentWrite;							          //发送完毕队列写入链表
				
} USARTBUS_DATA;

typedef enum
{
	USARTBUS_MSG_InsUsartSentQueue,
	USARTBUS_MSG_DelUsartSentQueue,
	USARTBUS_MSG_InsUsartTrsQueue,
	USARTBUS_MSG_DelUsartTrsQueue,
	USARTBUS_MSG_InsUsartRevQueue,
	USARTBUS_MSG_GetUsartRevQueue,
	USARTBUS_MSG_CheckUsartSentInterval,
	USARTBUS_MSG_MAX
}USARTBUS_MSG;



uint8_t UsartBusInit(uint16_t device,uint32_t baud);
void InsUsartRevQueue(uint16_t u16Way, STR_Usart_Frame* RevBufTemp);
void InsUsartTrsQueue(uint16_t u16Way, USARTBUS_DATA * pUsartBus, stUsartTrsQueue *SentUsart);
void UsartRecvDataProc(uint8_t *data,uint8_t len);
void UsartDriverTxDealFun(void);
uint8_t UsartBusDealFunc(uint16_t u16Way, USARTBUS_MSG msg, void * pData);


#endif


