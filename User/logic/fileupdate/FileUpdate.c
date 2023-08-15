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

#include "includes.h"
#include "FileUpdate.h"
#include "DoFatfsFiles.h"
#include "Can_Pro.h"
#include "V2_Can_Pro.h"
#include "Util_CRC.h"
#include "usartapp.h"
#include "udp_app_proc.h"
#include "bsp_spi_norflash.h"
#include "sys_msg_def.h"
#include "V4_Wireless.h"

CAN_PROGRAM_UPDATE_DATA canUpdate_Queue;
STR_UDP_UPDATE_FILE udpUpdateFileType;

STR_UPDATE_QUEUE FileUpdate_Queue;

SEND_FILE_DATA send_file_data;

static char fatfsFilename[40];

uint32_t sendTimer = TIMERINIT;

uint8_t useFatfsingFlag = 0;


uint32_t sendTimerAPI()
{
	return sendTimer;
}

uint8_t getUseFatfsFlag(void)
{
	return useFatfsingFlag;
}


/*******************************************************************************************
**函数名称：CanFildReadInfoProc
**函数作用：can发送程序读取程序头
**函数参数：无
**函数输出：无
**注意事项：无
*******************************************************************************************/
void CanFildReadInfoProc(char * name)
{		
	uint8_t i;
	sendTimer = TIMERINIT;	
	memset(&canUpdate_Queue,0,sizeof(CAN_PROGRAM_UPDATE_DATA));
	memset(&FileUpdate_Queue,0,sizeof(STR_UPDATE_QUEUE));
	for(i = 0; i<sizeof(canUpdate_Queue.fileName);i++)
	{
		if(*(name) == '\0')
			return;
		canUpdate_Queue.fileName[i] = *(name+i);
	}
//	memcpy(canUpdate_Queue.fileName,name,sizeof(canUpdate_Queue.fileName));
	memset(fatfsFilename,0,sizeof(fatfsFilename));
	
	sprintf(fatfsFilename,"%s%s", "0:update/",canUpdate_Queue.fileName);
	Fatfs_openFile(fatfsFilename,0);
	
	useFatfsingFlag = 1;

	Fatfs_readFile(fatfsFilename,FileUpdate_Queue.u8ScReadProgramQueue, PROGRAM_INFO_SIZE);
	memcpy(&canUpdate_Queue.programinfo,FileUpdate_Queue.u8ScReadProgramQueue,sizeof(canUpdate_Queue.programinfo));
}




/*******************************************************************************************
**函数名称：CanReadDataProc
**函数作用：can发送程序读取程序
**函数参数：无
**函数输出：无
**注意事项：无
*******************************************************************************************/
uint8_t CanReadDataProc(uint16_t *readsize)
{
	if(canUpdate_Queue.programinfo.u32PrgSize-canUpdate_Queue.nowprosize>=SC_UPDATE_PROGRAM_QUEUE_MAX)
	{
		Fatfs_readFile(fatfsFilename,FileUpdate_Queue.u8ScReadProgramQueue, SC_UPDATE_PROGRAM_QUEUE_MAX);
		*readsize =  SC_UPDATE_PROGRAM_QUEUE_MAX;
		canUpdate_Queue.nowprosize += SC_UPDATE_PROGRAM_QUEUE_MAX;	
	}
	else if(canUpdate_Queue.programinfo.u32PrgSize > canUpdate_Queue.nowprosize)
	{
		Fatfs_readFile(fatfsFilename,FileUpdate_Queue.u8ScReadProgramQueue, canUpdate_Queue.programinfo.u32PrgSize-canUpdate_Queue.nowprosize);
		*readsize =  canUpdate_Queue.programinfo.u32PrgSize-canUpdate_Queue.nowprosize;
		canUpdate_Queue.nowprosize = canUpdate_Queue.programinfo.u32PrgSize; 
		useFatfsingFlag = 0;
	}
	else
	{
		Fatfs_closeFile();
		useFatfsingFlag = 0;
		memset(&FileUpdate_Queue,0,sizeof(STR_UPDATE_QUEUE));
		return 1;
	}
	return 0;
}


/*******************************************************************************************
**函数名称：CanSendDataProc
**函数作用：can发送程序过程函数
**函数参数：无
**函数输出：无
**注意事项：无
*******************************************************************************************/
void CanSendDataProc(char* name, uint8_t device)
{
	uint16_t programSize;
	uint8_t data[8];
	uint8_t driver;
	uint8_t actDevice;
	stDeviceStatusMsg deviceMsg;
	CanFildReadInfoProc(name);	//打开文件，读取bin文件头信息
	canUpdate_Queue.packetNum = 1;
	actDevice = GUIDeviceTypeToProActDeviceType(device);
	device = GUIDeviceTypeToProDeviceType(device);
	if(GetDeviceCanMsg(device,0, &driver) == 0)
	{
		Fatfs_closeFile();
		useFatfsingFlag = 0;
		sendTimer = TIMEROVER;
		return;
	}
	memcpy(data,(FileUpdate_Queue.u8ScReadProgramQueue + FileUpdate_Queue.readprogrampt),CAN_SEND_BUF_SIZE);
	CanProDirectSend(driver,device,0,CXB_SEND_DEITION, data,CAN_SEND_BUF_SIZE,canUpdate_Queue.packetNum);	
	
	if(actDevice == ID_ALARM ||actDevice == ID_PERSON)//声光和人员特殊处理，需等待跳转BootLoader
		OSTimeDly(1000);		//等待跳转BootLoader
	if(actDevice == ID_HIGH)
		OSTimeDly(2000);		//等待跳转BootLoader
	if(actDevice == ID_IRRX)
		OSTimeDly(1000);
	if(actDevice == ID_EMVD)
		OSTimeDly(2000);
	if(actDevice == ID_ANGLE)
		OSTimeDly(1000);
	if(actDevice == ID_HUB1)//minihub ghy
		OSTimeDly(1000);
	switch(actDevice)
	{
		case ID_ALARM:
			GetCanDeviceStatusMsg(driver,actDevice,ALARM_Version,&deviceMsg);//查询状态
			break;
		case ID_PERSON:
			GetCanDeviceStatusMsg(driver,actDevice,PERSON_Version,&deviceMsg);//查询状态
			break;
		case ID_HIGH:
			GetCanDeviceStatusMsg(driver,actDevice,HIGH_Verson,&deviceMsg);//查询状态
			break;
		case ID_ANGLE:
			GetCanDeviceStatusMsg(driver,actDevice,ANGLE0_Version,&deviceMsg);//查询状态
			break;
		case ID_IRRX:
			GetCanDeviceStatusMsg(driver,actDevice,IR_Verson,&deviceMsg);//查询状态
			break;
		case ID_EMVD:
			GetCanDeviceStatusMsg(driver,actDevice,EMVD_Version,&deviceMsg);//查询状态
			break;
		case ID_HUB1:
		case ID_HUB2:
		case ID_HUB3:
			GetCanDeviceStatusMsg(driver,actDevice,0,&deviceMsg);//查询状态
			break;
	}//查询状态
	if(actDevice == ID_ALARM || actDevice == ID_HIGH)//声光和人员特殊处理，需等待跳转BootLoader
	{
		if(deviceMsg.deviceStatus == DEVICEPROGRAM)
			CanProDirectSend(driver,device,0,CXB_SEND_DEITION, data,CAN_SEND_BUF_SIZE,canUpdate_Queue.packetNum);
		else
		{
			if(actDevice == ID_HIGH)
				CanProDirectSend(driver,device,0,CXB_SEND_DEITION, data,CAN_SEND_BUF_SIZE,canUpdate_Queue.packetNum);
			deviceMsg.deviceStatus = DEVICEPROGRAM;
			SetCanDeviceStatusMsg(driver,actDevice,1,&deviceMsg);//查询状态;
		}
	}

	canUpdate_Queue.packetNum++;
	FileUpdate_Queue.readprogrampt = FileUpdate_Queue.readprogrampt+CAN_SEND_BUF_SIZE;
	if(canUpdate_Queue.programinfo.u32PrgSize<THE_DEV_PRG_BULK_SIZE)
		OSTimeDly(FLASH_BULK_ERASER_TIME);
	else
		OSTimeDly((canUpdate_Queue.programinfo.u32PrgSize/THE_DEV_PRG_BULK_SIZE+1)*FLASH_BULK_ERASER_TIME);
	
	while(FileUpdate_Queue.readprogrampt < PROGRAM_INFO_SIZE)//发送头信息其他帧
	{
		memcpy(data,(FileUpdate_Queue.u8ScReadProgramQueue + FileUpdate_Queue.readprogrampt),CAN_SEND_BUF_SIZE);
		CanProDirectSend(driver,device,0,CXB_SEND_DEITION, data,CAN_SEND_BUF_SIZE,canUpdate_Queue.packetNum);
		canUpdate_Queue.packetNum++;
		FileUpdate_Queue.readprogrampt += CAN_SEND_BUF_SIZE;
		sendTimer = canUpdate_Queue.nowprosize*100/canUpdate_Queue.programinfo.u32PrgSize;
		OSTimeDly(CAN_SEND_INTERVAL);
	}
	
	canUpdate_Queue.packetNum = 1;
	FileUpdate_Queue.readprogrampt = 0;
	while(!CanReadDataProc(&programSize))//发程序
	{
		do
		{
			if(programSize>=CAN_SEND_BUF_SIZE)
			{
				CanProDirectSend(driver,device,0,CXB_SEND_PROGRAM, FileUpdate_Queue.u8ScReadProgramQueue + FileUpdate_Queue.readprogrampt,CAN_SEND_BUF_SIZE,canUpdate_Queue.packetNum);
				FileUpdate_Queue.readprogrampt += CAN_SEND_BUF_SIZE;
				programSize = programSize - CAN_SEND_BUF_SIZE;
			}
			else
			{
				CanProDirectSend(driver,device,0,CXB_SEND_PROGRAM, FileUpdate_Queue.u8ScReadProgramQueue + FileUpdate_Queue.readprogrampt,programSize,canUpdate_Queue.packetNum);
				FileUpdate_Queue.readprogrampt += CAN_SEND_BUF_SIZE;
				programSize = 0;
			}
			FileUpdate_Queue.readprogrampt %= SC_UPDATE_PROGRAM_QUEUE_MAX;
			canUpdate_Queue.packetNum++;
			sendTimer = canUpdate_Queue.nowprosize*100/canUpdate_Queue.programinfo.u32PrgSize;
			OSTimeDly(CAN_SEND_INTERVAL);
		}while(programSize>0);
	}
	
	if(actDevice == LOAD_TYPE_HIGH_BL || actDevice == LOAD_TYPE_ALARM_BL || actDevice == LOAD_TYPE_PERSON_BL)
	{
		CanProSend(driver,device,0,ALARM_RESET,1,0);
	}

	deviceMsg.deviceStatus = DEVICENORMAL;
	SetCanDeviceStatusMsg(driver,actDevice,0,&deviceMsg);//查询状态;
}


void UsartSendAPIProc(uint8_t device,uint8_t *data,uint16_t len)
{
	if(device == GUI_TYPE_KEYBOARD)
	{
		SendProgramTransmissionData(data,len);
	}
	else if(device == GUI_TYPE_WIRELESS)
	{
		WirelessSendProData(data,len);
	}
	
}


/*******************************************************************************************
**函数名称：CanSendDataProc
**函数作用：can发送程序过程函数
**函数参数：无
**函数输出：无
**注意事项：无
*******************************************************************************************/
void UsartSendDataProc(char* name, uint8_t device)
{
	uint16_t programSize;
	CanFildReadInfoProc(name);	//发送头信息第一帧
	canUpdate_Queue.packetNum = 1;
	UsartSendAPIProc(device,FileUpdate_Queue.u8ScReadProgramQueue + FileUpdate_Queue.readprogrampt,USART_SEND_BUF_SIZE);
	canUpdate_Queue.packetNum++;
	FileUpdate_Queue.readprogrampt = FileUpdate_Queue.readprogrampt+USART_SEND_BUF_SIZE;
	if(canUpdate_Queue.programinfo.u32PrgSize<THE_DEV_PRG_BULK_SIZE)
		OSTimeDly(FLASH_BULK_ERASER_TIME);
	else
		OSTimeDly(canUpdate_Queue.programinfo.u32PrgSize/THE_DEV_PRG_BULK_SIZE*FLASH_BULK_ERASER_TIME);
	
	while(FileUpdate_Queue.readprogrampt < PROGRAM_INFO_SIZE)//发送头信息其他帧
	{
		UsartSendAPIProc(device,FileUpdate_Queue.u8ScReadProgramQueue + FileUpdate_Queue.readprogrampt,USART_SEND_BUF_SIZE);
		canUpdate_Queue.packetNum++;
		FileUpdate_Queue.readprogrampt += USART_SEND_BUF_SIZE;
		sendTimer = canUpdate_Queue.nowprosize*100/canUpdate_Queue.programinfo.u32PrgSize;
		OSTimeDly(CAN_SEND_INTERVAL);
	}
	
	canUpdate_Queue.packetNum = 1;
	FileUpdate_Queue.readprogrampt = 0;
	while(!CanReadDataProc(&programSize))//发程序
	{
		do
		{
			if(programSize>=USART_SEND_BUF_SIZE)
			{
				UsartSendAPIProc(device,FileUpdate_Queue.u8ScReadProgramQueue + FileUpdate_Queue.readprogrampt,USART_SEND_BUF_SIZE);
				FileUpdate_Queue.readprogrampt += USART_SEND_BUF_SIZE;
				programSize = programSize - USART_SEND_BUF_SIZE;
			}
			else
			{
				UsartSendAPIProc(device,FileUpdate_Queue.u8ScReadProgramQueue + FileUpdate_Queue.readprogrampt,programSize);
				FileUpdate_Queue.readprogrampt += programSize;
				programSize = 0;
			}
			FileUpdate_Queue.readprogrampt %= SC_UPDATE_PROGRAM_QUEUE_MAX;
			canUpdate_Queue.packetNum++;
			sendTimer = canUpdate_Queue.nowprosize*100/canUpdate_Queue.programinfo.u32PrgSize;
			OSTimeDly(100);
		}while(programSize>0);
	}	
}


void UDPUpdate_GetfileSize(void)
{
	memset(&FileUpdate_Queue,0,sizeof(STR_UPDATE_QUEUE));	
	memset(fatfsFilename,0,sizeof(fatfsFilename));
	if(udpUpdateFileType.udpUpdateReady.u8FileSuffixType[0] == 'p' && udpUpdateFileType.udpUpdateReady.u8FileSuffixType[1] == 'n')
		sprintf(fatfsFilename,"%s%s%s%s", "0:",udpUpdateFileType.udpUpdateReady.u8FileName,".",udpUpdateFileType.udpUpdateReady.u8FileSuffixType);
	else
		sprintf(fatfsFilename,"%s%s%s%s", "0:update/",udpUpdateFileType.udpUpdateReady.u8FileName,".",udpUpdateFileType.udpUpdateReady.u8FileSuffixType);
	udpUpdateFileType.udpUpdateReady.u32FileSize = Fatfs_openFile(fatfsFilename,0);
	udpUpdateFileType.u32tmpFileSize = udpUpdateFileType.udpUpdateReady.u32FileSize;
}

/*******************************************************************************************
**函数名称：UDPUpdate_Start
**函数作用：udp发送程序开始帧
**函数参数：无
**函数输出：无
**注意事项：无
*******************************************************************************************/
void UDPUpdate_Start(char * name,uint16_t updateSupportID, uint8_t udpDirver)
{
	uint8_t i,j = 0;
	uint8_t flag = 0;
	udpUpdateApiType frameInfo;
	
	memset(&udpUpdateFileType,0,sizeof(udpUpdateFileType));
	for(i = 0; i<(FILESUFFIXTYPE+FILENAMESIZE);i++)
	{
		if(*(name+i) == '\0')
			break;
		if(*(name+i) == '.')
		{
			flag = 1;
			continue;
		}
		if(!flag)
			udpUpdateFileType.udpUpdateReady.u8FileName[i] = *(name+i);
		else
		{
			udpUpdateFileType.udpUpdateReady.u8FileSuffixType[j] = *(name+i);
			j++;
		}
	}

	UDPUpdate_GetfileSize();
	/*udp发送第一包*/
	frameInfo.ack =0;
	frameInfo.curNum=1;
	frameInfo.totalNum=1;
	UdpUpdateProcCtrl(UpdateProcIDType_UpdateStart_request,(void *)(&udpUpdateFileType.udpUpdateReady),sizeof(UDP_UPDATE_READY),updateSupportID,frameInfo);
}

void UDPUpdate_CrcAck_API(uint16_t crc,uint8_t number)
{
	if(crc == udpUpdateFileType.tmpCRC)
	{
		udpUpdateFileType.recvCrcNumber[number] ++;
	}
}

void UDPUpdate_LastCrcAck_API(uint16_t res,uint8_t number)
{
	if(res == 0)//正确
	{
		udpUpdateFileType.recvCrcNumber[number] ++;
	}
}


uint8_t  UDPUpdate_CrcErrorCalc(uint8_t * res[])
{
	//获取最大架号，获取最小架号
	uint8_t tmpNum;
	uint8_t i;
	uint8_t counts = 0;

	for(i = 0; i<tmpNum ; i++)
	{
		if(udpUpdateFileType.recvCrcNumber[i] != udpUpdateFileType.crcCounts)
		{
			res[counts] = &udpUpdateFileType.recvCrcNumber[i];
			counts++;
		}
	}
	return counts;
}

void UDPRepeatsendProc(uint16_t dataSize)
{
	//获取最大架号，获取最小架号
	uint8_t tmpNum;
	uint8_t i,j;
	udpUpdateApiType frameInfo;
	FileUpdate_Queue.readprogrampt = 0;
	if(udpUpdateFileType.u16FilePackNum<dataSize/SC_SAVE_PROGRAM_QUEUE_SINGLE)
		udpUpdateFileType.u16FilePackNum = udpUpdateFileType.u16FilePackNum+UDP_SENDPACKET_NUM_MAX - dataSize/SC_SAVE_PROGRAM_QUEUE_SINGLE;
	else
		udpUpdateFileType.u16FilePackNum = udpUpdateFileType.u16FilePackNum - dataSize/SC_SAVE_PROGRAM_QUEUE_SINGLE;
	for(j = 0; j< dataSize/SC_SAVE_PROGRAM_QUEUE_SINGLE; j++)
	{
		for(i = 1; i<=tmpNum ; i++)
		{
			if(udpUpdateFileType.crcCounts - udpUpdateFileType.recvCrcNumber[i] == 1)//两次未收到不重发
			{
				/*udp重发*/
				frameInfo.ack = 0;
				frameInfo.curNum=udpUpdateFileType.u16FilePackNum%UDP_SENDPACKET_NUM_MAX;
				if(udpUpdateFileType.u32tmpFileSize % SC_SAVE_PROGRAM_QUEUE_SINGLE > 0)
					frameInfo.totalNum=frameInfo.totalNum=udpUpdateFileType.u32tmpFileSize/SC_SAVE_PROGRAM_QUEUE_SINGLE+1;
				else
					frameInfo.totalNum=frameInfo.totalNum=udpUpdateFileType.u32tmpFileSize/SC_SAVE_PROGRAM_QUEUE_SINGLE+1;
				
				UdpUpdateProcCtrl(UpdateProcIDType_SendData_request,(void *)(FileUpdate_Queue.u8ScReadProgramQueue+FileUpdate_Queue.readprogrampt),SC_SAVE_PROGRAM_QUEUE_SINGLE,i,frameInfo);
				
				if(dataSize - FileUpdate_Queue.readprogrampt<SC_SAVE_PROGRAM_QUEUE_SINGLE)
					;
				OSTimeDly(10);
			}
		}
		FileUpdate_Queue.readprogrampt += SC_SAVE_PROGRAM_QUEUE_SINGLE;
		udpUpdateFileType.u16FilePackNum++;
	}
}

/*******************************************************************************************
**函数名称：UDPUpdate_SendData
**函数作用：udp读取fatfs文件
**函数参数：无
**函数输出：无
**注意事项：无
*******************************************************************************************/
uint8_t UDPUpdate_ReadData(uint16_t *readsize,uint8_t *finFlag)
{
	if(udpUpdateFileType.udpUpdateReady.u32FileSize>SC_UPDATE_PROGRAM_QUEUE_MAX)
	{
		//读取数据
		memset(FileUpdate_Queue.u8ScReadProgramQueue,0,sizeof(FileUpdate_Queue.u8ScReadProgramQueue));
		Fatfs_readFile(fatfsFilename, FileUpdate_Queue.u8ScReadProgramQueue, SC_UPDATE_PROGRAM_QUEUE_MAX);
		FileUpdate_Queue.readprogrampt = 0;
		//计算总校验
		Crc16Ccitt(FileUpdate_Queue.u8ScReadProgramQueue,SC_UPDATE_PROGRAM_QUEUE_MAX,&udpUpdateFileType.u16CRC);
		//计算5帧的校验
		udpUpdateFileType.tmpCRC = 0;
		Crc16Ccitt(FileUpdate_Queue.u8ScReadProgramQueue,SC_UPDATE_PROGRAM_QUEUE_MAX,&udpUpdateFileType.tmpCRC);
		
		*readsize =  SC_UPDATE_PROGRAM_QUEUE_MAX;
		udpUpdateFileType.udpUpdateReady.u32FileSize -= SC_UPDATE_PROGRAM_QUEUE_MAX;
		*finFlag = 0;
	}
	else if(udpUpdateFileType.udpUpdateReady.u32FileSize <= SC_UPDATE_PROGRAM_QUEUE_MAX && udpUpdateFileType.udpUpdateReady.u32FileSize != 0)
	{
		memset(FileUpdate_Queue.u8ScReadProgramQueue,0,sizeof(FileUpdate_Queue.u8ScReadProgramQueue));
		Fatfs_readFile(fatfsFilename, FileUpdate_Queue.u8ScReadProgramQueue, udpUpdateFileType.udpUpdateReady.u32FileSize);
		
		useFatfsingFlag = 0;
		FileUpdate_Queue.readprogrampt = 0;
		Crc16Ccitt(FileUpdate_Queue.u8ScReadProgramQueue,udpUpdateFileType.udpUpdateReady.u32FileSize,&udpUpdateFileType.u16CRC);
		
		udpUpdateFileType.tmpCRC = 0;
		Crc16Ccitt(FileUpdate_Queue.u8ScReadProgramQueue,udpUpdateFileType.udpUpdateReady.u32FileSize,&udpUpdateFileType.tmpCRC);
		
		*readsize =  udpUpdateFileType.udpUpdateReady.u32FileSize;
		udpUpdateFileType.udpUpdateReady.u32FileSize = 0; 
		*finFlag = 1;
	}
	else
	{
		Fatfs_closeFile();
		return 1;
	}
	return 0;
}

 /*******************************************************************************************
**函数名称：UDPsendDataProc
**函数作用：udp发送程序
**函数参数：无
**函数输出：无
**注意事项：无
*******************************************************************************************/
void UDPsendDataProc(char* name, uint8_t sendDriver ,uint8_t updateSupportID)
{
	uint16_t programSize;
	uint8_t repeatCount = 0;
	uint8_t finFlag;
	udpUpdateApiType frameInfo;
	uint32_t senttmp = 0;
	
	if(updateSupportID == 0)
	{
		sendTimer = TIMEROVER;
		return;
	}

	UDPUpdate_Start(name,updateSupportID,sendDriver);	//发送开始帧
	if(udpUpdateFileType.udpUpdateReady.u32FileSize == 0)
	{
		return;
	}
	OSTimeDly(2000);
	frameInfo.curNum = 1;
	sendTimer = TIMERINIT;
	useFatfsingFlag = 1;
	while(!UDPUpdate_ReadData(&programSize,&finFlag))//发程序
	{
		do
		{
			if(programSize>=SC_SAVE_PROGRAM_QUEUE_SINGLE)
			{						
				udpUpdateFileType.u16FilePackNum ++;	
				/*udp发送*/
				frameInfo.ack =0;
				if(frameInfo.curNum == 0)
					frameInfo.curNum = 1;

				if(udpUpdateFileType.u32tmpFileSize/SC_SAVE_PROGRAM_QUEUE_SINGLE > UDP_SENDPACKET_NUM_MAX)//总包数大于255
				{
					senttmp = (udpUpdateFileType.u32tmpFileSize/SC_SAVE_PROGRAM_QUEUE_SINGLE%UDP_SENDPACKET_NUM_MAX);//已发送数据大小
					if(senttmp >= (udpUpdateFileType.udpUpdateReady.u32FileSize/SC_SAVE_PROGRAM_QUEUE_SINGLE))
					{
						if(udpUpdateFileType.u32tmpFileSize%SC_SAVE_PROGRAM_QUEUE_SINGLE)
							frameInfo.totalNum = senttmp+1;
						else
							frameInfo.totalNum = senttmp;
					}
					else
						frameInfo.totalNum = UDP_SENDPACKET_NUM_MAX;
				}
				else
				{
					if(udpUpdateFileType.u32tmpFileSize%SC_SAVE_PROGRAM_QUEUE_SINGLE)
						frameInfo.totalNum = udpUpdateFileType.u32tmpFileSize/SC_SAVE_PROGRAM_QUEUE_SINGLE+1;
					else
						frameInfo.totalNum = udpUpdateFileType.u32tmpFileSize/SC_SAVE_PROGRAM_QUEUE_SINGLE;
				}	
									
				UdpUpdateProcCtrl(UpdateProcIDType_SendData_request,(void *)(FileUpdate_Queue.u8ScReadProgramQueue+FileUpdate_Queue.readprogrampt),SC_SAVE_PROGRAM_QUEUE_SINGLE,updateSupportID,frameInfo);
				
				FileUpdate_Queue.readprogrampt = FileUpdate_Queue.readprogrampt+SC_SAVE_PROGRAM_QUEUE_SINGLE;
				programSize = programSize - SC_SAVE_PROGRAM_QUEUE_SINGLE;
				frameInfo.curNum++;
			}
			else
			{
				udpUpdateFileType.u16FilePackNum++;
				
				if(frameInfo.curNum == 0)
					frameInfo.curNum = 1;
				/*udp发送*/
				frameInfo.ack =0;
	
				UdpUpdateProcCtrl(UpdateProcIDType_SendData_request,(void *)(FileUpdate_Queue.u8ScReadProgramQueue+FileUpdate_Queue.readprogrampt),programSize,updateSupportID,frameInfo);
				
				FileUpdate_Queue.readprogrampt = FileUpdate_Queue.readprogrampt+SC_SAVE_PROGRAM_QUEUE_SINGLE;
				programSize = 0;
				frameInfo.curNum++;
			}
			if(udpUpdateFileType.u16FilePackNum*SC_SAVE_PROGRAM_QUEUE_SINGLE > udpUpdateFileType.u32tmpFileSize)
				sendTimer = TIMEROVER;
			else
				sendTimer = udpUpdateFileType.u16FilePackNum*SC_SAVE_PROGRAM_QUEUE_SINGLE*100/udpUpdateFileType.u32tmpFileSize;
			OSTimeDly(12);
		}while(programSize>0);
		
			OSTimeDly(500);//等所有支架给应答
//			for(repeatCount = 0; repeatCount<UDP_REPEATSEAD_COUNT;repeatCount++)
//				UDPRepeatsendProc(programSize1);//重发
	}
	/*udp发送最后校验*/
	frameInfo.ack =0;
	frameInfo.curNum=1;
	frameInfo.totalNum=1;
	UdpUpdateProcCtrl(UpdateProcIDType_DataCRC_request,(void *)(&udpUpdateFileType.u16CRC),2,updateSupportID,frameInfo);
	
	OSTimeDly(1000);//等所有支架给应答
//	UDPUpdate_CrcErrorCalc();

	Fatfs_openFile(fatfsFilename,0);
	Fatfs_readFile(fatfsFilename, FileUpdate_Queue.u8ScReadProgramQueue, 1);
	Fatfs_closeFile();
	
	if(updateSupportID == UDP_BROADCAST_ADDRESS)
		UdpRecvFileUpdateStatusProc(FileUpdate_Queue.u8ScReadProgramQueue[0] ,udpUpdateFileType.udpUpdateReady.u8FileSuffixType[0], fatfsFilename);
}


/*******************************************************************************************
**函数名称：UDPUpdate_Start
**函数作用：udp发送程序开始帧
**函数参数：无
**函数输出：无
**注意事项：无
*******************************************************************************************/
void UDPUpdatePara_Start(uint16_t updateSupportID, uint8_t sendparaType)
{
	udpUpdateApiType frameInfo;
	
	memset(&udpUpdateFileType,0,sizeof(udpUpdateFileType));

	udpUpdateFileType.udpUpdateReady.u8FileSuffixType[0] = 'p';
	udpUpdateFileType.udpUpdateReady.u8FileSuffixType[1] = 'a';
	udpUpdateFileType.udpUpdateReady.u8FileSuffixType[2] = 'r';
	udpUpdateFileType.udpUpdateReady.u8FileSuffixType[3] = 'a';
	
	//获取para大小
	udpUpdateFileType.udpUpdateReady.u16FileType = sendparaType;
	udpUpdateFileType.udpUpdateReady.u32FileSize = send_file_data.paraSize;
	udpUpdateFileType.u32tmpFileSize = udpUpdateFileType.udpUpdateReady.u32FileSize;
	/*udp发送第一包*/
	frameInfo.ack =0;
	frameInfo.curNum=1;
	frameInfo.totalNum=1;
	UdpUpdateProcCtrl(UpdateProcIDType_UpdateStart_request,(void *)(&udpUpdateFileType.udpUpdateReady),sizeof(UDP_UPDATE_READY),updateSupportID,frameInfo);
}


/*******************************************************************************************
**函数名称：UDPsendDataProc
**函数作用：udp发送程序
**函数参数：无
**函数输出：无
**注意事项：无
*******************************************************************************************/
void UDPsendParaProc(char* name, uint8_t sendparaType ,uint8_t updateSupportID)
{
	uint8_t repeatCount = 0;
	uint8_t finFlag;
	udpUpdateApiType frameInfo;
	uint8_t* paraPt = send_file_data.paraAddress;
	uint32_t senttmp = 0;
	
	if(updateSupportID == 0)
		return;

	UDPUpdatePara_Start(updateSupportID,sendparaType);	//发送开始帧
	OSTimeDly(2000);
	frameInfo.curNum = 1;
	sendTimer = TIMERINIT;
	while(udpUpdateFileType.u32tmpFileSize)//发程序
	{
		if(udpUpdateFileType.u32tmpFileSize>=SC_SAVE_PROGRAM_QUEUE_SINGLE)
		{						
			udpUpdateFileType.u16FilePackNum ++;
			/*udp发送*/
			frameInfo.ack =0;
			if(frameInfo.curNum == 0)
				frameInfo.curNum = 1;
			if(udpUpdateFileType.udpUpdateReady.u32FileSize/SC_SAVE_PROGRAM_QUEUE_SINGLE>UDP_SENDPACKET_NUM_MAX)//总包数大于255
			{
				senttmp = udpUpdateFileType.udpUpdateReady.u32FileSize/SC_SAVE_PROGRAM_QUEUE_SINGLE%UDP_SENDPACKET_NUM_MAX;//已发送数据大小
				if(senttmp >= udpUpdateFileType.u32tmpFileSize/SC_SAVE_PROGRAM_QUEUE_SINGLE)
					frameInfo.totalNum = senttmp;
				else
					frameInfo.totalNum = UDP_SENDPACKET_NUM_MAX;
			}
			else
			{
				frameInfo.totalNum = udpUpdateFileType.udpUpdateReady.u32FileSize/SC_SAVE_PROGRAM_QUEUE_SINGLE;
			}
								
			UdpUpdateProcCtrl(UpdateProcIDType_SendData_request,paraPt,SC_SAVE_PROGRAM_QUEUE_SINGLE,updateSupportID,frameInfo);
			paraPt+=SC_SAVE_PROGRAM_QUEUE_SINGLE;
			udpUpdateFileType.u32tmpFileSize -= SC_SAVE_PROGRAM_QUEUE_SINGLE;
			frameInfo.curNum++;
		}
		else
		{
			udpUpdateFileType.u16FilePackNum++;
			
			if(frameInfo.curNum == 0)
				frameInfo.curNum = 1;
			/*udp发送*/
			frameInfo.ack = 0;		
			UdpUpdateProcCtrl(UpdateProcIDType_SendData_request,paraPt,udpUpdateFileType.u32tmpFileSize,updateSupportID,frameInfo);
			
			udpUpdateFileType.u32tmpFileSize = 0;
			frameInfo.curNum++;
		}
		if(udpUpdateFileType.u16FilePackNum*SC_SAVE_PROGRAM_QUEUE_SINGLE >udpUpdateFileType.udpUpdateReady.u32FileSize)
			sendTimer = TIMEROVER;
		else
			sendTimer = udpUpdateFileType.u16FilePackNum*SC_SAVE_PROGRAM_QUEUE_SINGLE*100/udpUpdateFileType.udpUpdateReady.u32FileSize;
		OSTimeDly(12);
		
		if(udpUpdateFileType.u16FilePackNum % UDP_SEND_CRC_COUNT == 0)
			OSTimeDly(500);//等所有支架给应答
//			for(repeatCount = 0; repeatCount<UDP_REPEATSEAD_COUNT;repeatCount++)
//				UDPRepeatsendProc(programSize1);//重发
	}
	
	OSTimeDly(500);
	Crc16Ccitt(send_file_data.paraAddress,udpUpdateFileType.udpUpdateReady.u32FileSize,&udpUpdateFileType.u16CRC);

	/*udp发送最后校验*/
	frameInfo.ack =0;
	frameInfo.curNum=1;
	frameInfo.totalNum=1;
	UdpUpdateProcCtrl(UpdateProcIDType_DataCRC_request,(void *)(&udpUpdateFileType.u16CRC),2,updateSupportID,frameInfo);
	
	OSTimeDly(1000);//等所有支架给应答	
//	UDPUpdate_CrcErrorCalc();
}

void SendFile_API(uint8_t fileType,char *name ,uint8_t device,uint8_t updateType)
{
	uint16_t tmpSem;
	
	sendTimer = TIMERINIT;
	memset(&send_file_data,0,sizeof(send_file_data));
	send_file_data.fileType = fileType;	
	
	if(fileType == FILETYPE)
	{
		if(*(name+6) == 0)
			return;
		send_file_data.device = device;//更新设备
		send_file_data.updateType = updateType;//本架，全面，左右邻架
		memcpy(fatfsFilename,name,sizeof(fatfsFilename));
		if(fatfsFilename[6] == 0)
			return;
	}
	else
	{
		send_file_data.device = device;//所有参数，全局参数
		send_file_data.updateType = updateType;
		//逻辑中获取参数起始地址
		SysDataFun_ext(DATA_MSG_GET_GLOBAL_PARAM,0,&send_file_data.paraAddress,&send_file_data.paraSize);
	}
	tmpSem = SENDFILESEM;
	OSMboxPost(revProgramSem,(void*)tmpSem);
}



uint8_t calcSupportIDfun(uint8_t updateType)
{
	uint8_t res = 0;
	uint8_t* pstate=NULL;
	uint16_t len=0;
	globalParam_SupportType prar;

	SysDataFun(DATA_MSG_READ_OBJECT_PARAM,ENUM_SYSTEM,globalParamEnum_Support,0,0,&pstate,&len);
	memcpy((uint8_t*)&prar,pstate,len);	

	if(updateType == UPDATELEFT)
	{
		if(prar.currentSupportID == prar.MinSupportID && prar.SupportDirect == 1)//当前架号最小架号，架号增向为右，要求更新左邻架
			return 0;
		
		if( prar.SupportDirect == 1)//右
			res = prar.currentSupportID - 1;
		else
			res = prar.currentSupportID + 1;
	}
	else if(updateType == UPDATERIGHT)
	{
		if(prar.currentSupportID == prar.MaxSupportID && prar.SupportDirect == 0)//当前架号最大架号，架号增向为左，要求更新右邻架
			return 0;
		
		if( prar.SupportDirect == 1)//右
			res = prar.currentSupportID + 1;
		else
			res = prar.currentSupportID - 1;
	}
	return res;
}


/*******************************************************************************************
**函数名称：SendProgramProc
**函数作用：控制器发送程序
**函数参数：无
**函数输出：无
**注意事项：无
*******************************************************************************************/
void SendFilProc(void)
{
	char name[30];
	uint8_t device;
	uint8_t updateType;
	uint8_t jsonflag[4];
				
	uint8_t supportID;
	updateType = send_file_data.updateType;
	supportID = calcSupportIDfun(updateType);
	device = send_file_data.device;
	
	if(send_file_data.fileType == FILETYPE)
	{
		if(updateType == UPDATESEIF)
		{
			if(device == GUI_TYPE_SC)//控制器
			{
				name[0] = 1;//置上电更新标志
				memcpy(name+1,fatfsFilename,sizeof(name));
				W25QXX_Erase_Sector(0x1F80000/4096);
				W25QXX_Write_NoCheck((uint8_t *)name,0x1F80000,sizeof(name));
				ETH_Stop();
				BCM_551CS_LOW();				
				NVIC_SystemReset();
			}
			else if(device == GUI_TYPE_PROGRAM)//json
			{	
//				sprintf(name,"%s%s", "0:update/",fatfsFilename);
//				ee_WriteBytes((uint8_t *)name, JSON_PARAM_USING_START, 30);
//				//设置重新读取json默认值存ee
//				ee_ReadBytes(jsonflag, PARAM_MODIFY_START, FIRST_TURNON_LEN);
//				jsonflag[3]++;
//				ee_WriteBytes(jsonflag, PARAM_MODIFY_START, FIRST_TURNON_LEN);
				ETH_Stop();
				BCM_551CS_LOW();
				NVIC_SystemReset();
			}	
			else if(device == GUI_TYPE_KEYBOARD ||device == GUI_TYPE_WIRELESS)//键盘，无线
			{
				OSTimeDly(500);
				UsartSendDataProc(fatfsFilename,device);
			}
			else if(device != 0)
			{
				OSTimeDly(500);
				CanSendDataProc(fatfsFilename,device);
			}
		}
		else if(updateType == UPDATELEFT)
			UDPsendDataProc(fatfsFilename,device,supportID);
		else if(updateType == UPDATERIGHT)
			UDPsendDataProc(fatfsFilename,device,supportID);
		else if(updateType == UPDATEALL)
			UDPsendDataProc(fatfsFilename,device,UDP_BROADCAST_ADDRESS);
	}
	else
	{
		if(updateType == UPDATELEFT)
			UDPsendParaProc(fatfsFilename,device,supportID);
		else if(updateType == UPDATERIGHT)
			UDPsendParaProc(fatfsFilename,device,supportID);
		else if(updateType == UPDATEALL)
			UDPsendParaProc(fatfsFilename,device,UDP_BROADCAST_ADDRESS);
	}
}



