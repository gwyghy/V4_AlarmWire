/*
*********************************************************************************************************
*	                                            V4
*                           Commuication Controller For The Mining Industry
*
*                                  (c) Copyright 1994-2013  HNDZ 
*                       All rights reserved.  Protected by international copyright laws.
*
*
*    File    : FileUpdate.c
*    Module  : user
*    Version : V1.0
*    History :
*   -----------------
*              Version  Date           By            Note
*              v1.0         
*
*********************************************************************************************************
*/
#ifndef __FILEUPDATE_H__
#define __FILEUPDATE_H__

#include "FileLoad.h"


#define SC_SAVE_PROGRAM_QUEUE_SINGLE  512
#define PROGRAM_INFO_SIZE  256

#define USART_SEND_BUF_SIZE  128

#define CAN_SEND_BUF_SIZE   8

#define UDP_SEND_CRC_COUNT  5

#define UDP_REPEATSEAD_COUNT 1

#define SC_UPDATE_PROGRAM_QUEUE_MAX 5*512

#define		FLASH_BULK_ERASER_TIME				(3000)//块擦除时间
#define THE_DEV_PRG_BULK_SIZE		    	0x10000//块定义。64K

#define CAN_SEND_INTERVAL   20

#define UDP_SENDPACKET_NUM_MAX   255

#define UDP_BROADCAST_ADDRESS  255

enum
{
	UPDATESEIF,
	UPDATELEFT,
	UPDATERIGHT,
	UPDATEALL
};


enum
{
	TIMERINIT = 0,
	TIMEROVER = 100
};

enum
{
	FILETYPE,
	PARATYPE
};

enum{
	PARAALL = 107,//全部所有参数
	PARAOVERALL = 106//全局参数
};

typedef struct
{
 uint8_t u8ScReadProgramQueue[SC_UPDATE_PROGRAM_QUEUE_MAX];
 uint16_t readprogrampt;
} STR_UPDATE_QUEUE;

typedef struct
{
	uint8_t fileType;
	uint8_t device;
	uint8_t updateType;
	uint8_t* paraAddress;
	uint32_t paraSize;
}
SEND_FILE_DATA;


typedef struct
{
	PROGRAM_VERSION_TYPE programinfo;
	char fileName[FILENAMESIZE];
	uint32_t nowprosize;
	uint32_t packetNum;
}
CAN_PROGRAM_UPDATE_DATA;

uint32_t sendTimerAPI(void);
uint8_t getUseFatfsFlag(void);
void CanSendDataProc(char* name, uint8_t device);
void UsartSendDataProc(char* name, uint8_t device);
void SendFile_API(uint8_t fileType,char *name ,uint8_t device,uint8_t updateType);
void UDPUpdate_CrcAck_API(uint16_t crc,uint8_t number);
void UDPUpdate_LastCrcAck_API(uint16_t crc,uint8_t number);
void UpdateFileTimer(uint8_t sendDriver, uint8_t * min, uint8_t *s);
void SendFilProc(void);

#endif


