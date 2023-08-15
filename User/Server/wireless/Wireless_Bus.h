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


/**V2��Χ�豸can֡��ʽ***/
__packed typedef struct
{
	__packed union
	{
		__packed struct
		{
			unsigned int	RxID:3;				//���շ�ID
			unsigned int	TxID:3;				//���ͷ�ID
			unsigned int	FrameType:10;		//֡���	
			unsigned int	Sum:4;		//��֡��Ż�
			unsigned int	LiuShuiNumb:5;		//��ˮ���кţ�0x0-0xfѭ����ACK=1Ӧ��֡����ˮ�����ñ�Ӧ��֡����ˮ
			unsigned int	Sub:1;		//���֡/��֡��0�����֡����֡��1����֡
			unsigned int	ACK:1;				//Ӧ���־��1:Ӧ��0:����Ӧ��			
			unsigned int	Reservd:2;			//Ԥ���������ڴ�����:01,����:00	
			unsigned int	NoUsed:3;			//����������
		} ID;//֡ID
		uint32_t u32Id;//֡ID
	} u32ID;	//֡ID
	uint8_t	u16DLC;			//֡����
	uint8_t	u8DT[8];			//֡����

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
	
	
	stUsartTrsQueue    UsartTrs[USART_TX_QUEUE_NUM];	      //�����ȼ���������
	uint16_t           u16UsartTrsCnt;							      //�����ȼ�������������
	stUsartTrsQueue*   pUsartTrsHead;						          //�����ȼ�������������
	stUsartTrsQueue*   pUsartTrsEnd;							        //�����ȼ���������ĩ��
	
	
	stUsartRxQueue     UsartRev[USART_RX_QUEUE_NUM];			      //���ն���
	uint16_t           u16UsartRevCnt;							          //���ն�������
	stUsartRxQueue*    pUsartRevHead;							            //���ն�������
	stUsartRxQueue*    pUsartRevEnd;							            //���ն���ĩ��
	
																										    
	stUsartTrsQueue    UsartSent[USART_SENT_QUEUE_NUM];	        //������϶���
	uint16_t         	 u16UsartSentCnt;							          //������϶�������
	stUsartTrsQueue*   pUsartSentRead;						            //������϶��д�������
	stUsartTrsQueue*   pUsartSentWrite;							          //������϶���д������
				
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


