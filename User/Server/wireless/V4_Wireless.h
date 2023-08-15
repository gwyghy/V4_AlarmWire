/*
*********************************************************************************************************
*	                                            V4
*                           Commuication Controller For The Mining Industry
*
*                                  (c) Copyright 1994-2013  HNDZ 
*                       All rights reserved.  Protected by international copyright laws.
*
*
*    File    : V4_Wireless.h
*    Module  : user
*    Version : V1.0
*    History :
*   -----------------
*              Version  Date           By            Note
*              v1.0         
*
*********************************************************************************************************
*/

#ifndef __V4_WIRELESS_H__
#define __V4_WIRELESS_H__

#include "Wireless_Bus.h"


#define SCNET_DECODE_RANGE    5


#define YKQ_DEV_TYPE 				(0x00100A08)//ң����
#define YKQ_PARA_SIZE       3000
#define WIRELESS_ADDRESS    2
#define YKQ_CONNECT_TIMER   2000

#define TIMERCLOSE  0XFFFF


#define WLONLINETIMER   5000
#define BUSDATAPROCTIMER 10
#define YKQMANUALSINGLETIMER 300
#define IROVERTIMER   1000
#define YKQSTOPSTATETIMER   500
#define YKQDECODETIMER  2*60*100
enum
{
	WIRELESS_HEART,
	WIRELESS_DATA,
	WIRELESS_STARTHINT,
	WIRELESS_RESERVED,
	WIRELESS_PARASET,
	WIRELESS_UPDATE,
	WIRELESS_RESTORATION
};


enum
{
	V4YKQ_HEART = 0,
	V4YKQ_IR_CODEMATCH = 4,
	V4YKQ_IR_CODEMATCH_ACK = 5,
	V4YKQ_WL_CODEMATCH = 6,
	V4YKQ_WL_CODEMATCH_ACK = 7,
	V4YKQ_ACTIVE_DECODE = 14,
	V4YKQ_ACTIVE_DECODE_ACK = 6,
	V4YKQ_PASSIVE_DECODE = 15,
	V4YKQ_PASSIVE_DECODE_ACK = 8,
	V4YKQ_CONTROL_DATA = 10,
	V4YKQ_CONTROL_DATA_ACK = 11,
	V4YKQ_READ_STATE = 12,
	V4YKQ_READ_STATE_ACK = 12,
	V4YKQ_PARA_REQUEST = 12,
	V4YKQ_PARA_REQUEST_ACK = 12,
	V4YKQ_PARA_WRITE = 12,
	V4YKQ_PARA_WRITE_ACK = 12,
	V4YKQ_FILE_REQUEST = 18,
	V4YKQ_NEIGHBOR_REQUEST = 19,
	V4YKQ_NEIGHBOR_REQUEST_ACK = 20,
	V4WIRELESS_BUS_REQUEST = 100,
	V4WIRELESS_BUS_REQUEST_ACK = 101
};



enum
{
	STATE_IDLE,
	STATE_IR_CODEMATCHING,
	STATE_IR_SUCCESS,
	STATE_IR_FAIL,
	STATE_WL_CODEMATCHING,
	STATE_WL_SUCCESS,
	STATE_WL_FAIL,
};

enum
{
	SINGLE_CONTROL = 1,
	COMB_CONTROL = 2,
	GROUP_CONTROL = 3,
	FUNCTION_KEY = 4
};
enum
{
	MANUAL_ACTION,
	AUTO_ACTION
};



typedef struct
{
	uint8_t SCNum;//���������
	uint8_t YkqNum;//ң�������
	uint8_t YkqState;//ң����״̬
	uint8_t hwemergencyNum;
	uint8_t bisuoNum;
	uint8_t YkqKey;//��Կ
	uint8_t YkqCodingMode;//���뷽ʽ  0���������룻1��+-������
	uint8_t controllNum;
	uint16_t YkqSCCrc;//����crc
	uint16_t YkqCrc;//ң����crc
}
V4YKQ_DATA;



enum
{
	WIRELESS_NOACK,
	WIRELESS_ACK
};


#define RFID_UPDATE_LEN   55

/**����Ŀ��ʹ�õ��������ݸ�ʽ***/
__packed typedef struct
{
	__packed union
	{
		__packed struct
		{
			unsigned int	SDL:6;				//���շ�ID�����ͷ�ID������6bit��
			unsigned int	FrameType:10;		//֡���
			unsigned int	Reservd:16;			//Ԥ������
		} ID;//֡ID
		u32 u32Id;//֡ID
	} u32ID;	//֡ID
	u8	u8DLC;				//֡����
	u8	u8DT[RFID_UPDATE_LEN];			//֡����
}RFID_UPDATE_FRAME_TYPE;

/**֡���Ͷ���**/
enum
{
	RFID_FT_UPDATE_DWL_VERSION = 0x1DF,//����汾��Ϣ
	RFID_FT_UPDATE_WRITE_VERSION = 0x1DE,//д��汾��Ϣ	
	RFID_FT_UPDATE_DWL_CODE = 0x1DD,//����������
	RFID_FT_UPDATE_WRITE_CODE = 0x1DC,//д��汾��Ϣ
	RFID_FT_UPDATE_END = 0x1DB	//���¹��̽���
};


typedef struct
{
	uint16_t  frametype;        	//֡����
	uint16_t  overtime;						//��ʱʱ��
	uint16_t  count;              //���ʹ���
	uint8_t   ack;                //Ӧ��
	uint16_t  (*rececallbacfun)(STR_WLCAN_Frame* );     //���ջص�����
	uint16_t  (*overcallbacfun)(void * , STR_WLCAN_Frame* );     //��ʱ�ص�����
	uint16_t  (*sendiffun)(void *);                            //���������ص�����
	uint16_t  (*sendprofun)(void *);                           //����ǰ����ص�����
}
WIRELESSPROHEAD;

uint8_t *getWirelessVersion(void);
void InsUsartIrRecvData_proc(STR_WLCAN_Frame * data,uint8_t len);
void SendYkqParaProc(void);
void SendAlarm_YkqParaProc(void);
void WirelessSendProData(u8* pFrame,u8 len);
void SendSCAddressData(void);
void SendWirelessBusData(uint8_t state,uint8_t id);
void SendWirelessResart(void);
void UsartRec_task(void);
u8 Wireless_SCnet_Decode_proc(u8 *pdata);
void IR_CodeMatching_Ack_Proc(uint8_t coderes);
void YkqCtrlAlarmProc(u8 state);
uint8_t getSupportID(void);
uint8_t getSupportDirect(void);
uint8_t getMasterCtrlTime(void);
u8 getJtState(void);
uint8_t getBsState(void);
void Wireless_CodeMatching_Ack_Proc(uint8_t state);
void InsSendWirelessDataProc(STR_WLCAN_Frame* frame,uint8_t addr);
#endif
