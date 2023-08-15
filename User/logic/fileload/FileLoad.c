/*
*********************************************************************************************************
*	                                            V4
*                           Commuication Controller For The Mining Industry
*
*                                  (c) Copyright 1994-2013  HNDZ 
*                       All rights reserved.  Protected by international copyright laws.
*
*
*    File    : FileLoad.c
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
#include "FileLoad.h"
#include "DoFatfsFiles.h"
#include "Can_Pro.h"
#include "Util_CRC.h"
#include "Util_Timer.h"
#include "FileUpdate.h"
#include "Util_Timer.h"
#include "udp_app_proc.h"
#include "sys_msg_def.h"
#include "V4_Wireless.h"


//char *dirnames[] = {"sc","emvd","hub","ir","cxb","angle","server","person","romote","jp","wl","person","alarm","program"};
char *dirnames = "update";

uint8_t FileSaveAddress[1024*1024*2]    __attribute__((at(MEM_SDRAM_FILESAVE_POS)));
uint32_t SdramFileSavept = 0;
uint32_t FatfsFileSavept = 0;
uint8_t updRecvParaFlag = 0;

STR_CAN_RECV_PROGRAM canRecvProgram;
STR_UDP_UPDATE_FILE udpRecvFile;

uint8_t UdpRecvFileTimeoutId;
uint8_t UdpRecvFileLongTimeoutId;
uint8_t CanRecvProgramTimeoutId;

//程序当前的更新状态
uint8_t canRecvProgramStatus = NORMALSTATUS;
uint8_t udpRecvFileStatus = FILENORMALSTATUS;

uint16_t udpFileCrc;
uint16_t udpRecvFileSem;

uint32_t updateTimer = 0;

static char fatfsFilename[40];


static void CanRecvVersionProc(void);
static void CanRecvProgramProc(void);
static void UdpRecvFileReadyProc(void);
static void UdpRecvFileDataProc(void);
static void UdpRecvFileFinCrcProc(void);

void (*FileRecvProc[])() =
{
	NULL,
	CanRecvVersionProc,
	CanRecvProgramProc,
	UdpRecvFileReadyProc,
	UdpRecvFileDataProc,
	UdpRecvFileFinCrcProc,
	NULL,
	SendFilProc,
	SendYkqParaProc,
	SendAlarm_YkqParaProc,
};

uint8_t GUIDeviceTypeToProDeviceType(uint8_t device)
{
	uint8_t res = 0XFF;
	switch(device)
	{
		case GUI_TYPE_SC:
			res = ID_SC;
			break;
		case GUI_TYPE_EMVD:
			res = ID_EMVD;
			break;
		case GUI_TYPE_ANGLE:
			res = ID_ANGLE;
			break;
		case GUI_TYPE_PERSON:
			res = ID_PERSON;
			break;
		case GUI_TYPE_ALARM:
			res = ID_ALARM;
			break;
		case GUI_TYPE_ALARM_UWB_ANGLE: //ghy 增加多功能声光报警器程序类型
			res = ID_ALARM;
			break;
		case GUI_TYPE_HUB: //ghy 增加MiniHUB程序类型
			res = ID_HUB1;
			break;
		case GUI_TYPE_HIGH:
			res = ID_HIGH;
			break;
		case GUI_TYPE_IR:
			res = ID_IRRX;
			break;
		case GUI_TYPE_PERSONBL:
			res = ID_PERSON;
			break;
		case GUI_TYPE_ALARMBL:
			res = ID_ALARM;
			break;
		case GUI_TYPE_HIGHBL:
			res = ID_HIGH;
			break;
		default:
			break;
	}	
	return res;
}


uint8_t GUIDeviceTypeToProActDeviceType(uint8_t device)
{
	uint8_t res = 0XFF;
	switch(device)
	{
		case GUI_TYPE_SC:
			res = ID_SC;
			break;
		case GUI_TYPE_EMVD:
			res = ID_EMVD;
			break;
		case GUI_TYPE_ANGLE:
			res = ID_ANGLE;
			break;
		case GUI_TYPE_PERSON:
			res = ID_PERSON;
			break;
		case GUI_TYPE_ALARM:
			res = ID_ALARM;
			break;
		case GUI_TYPE_ALARM_UWB_ANGLE: //ghy 增加多功能声光报警器设备类型
			res = ID_ALARM;
			break;
		case GUI_TYPE_HUB:  //ghy 增加MiniHUB设备类型
			res = ID_HUB1;
			break;
		case GUI_TYPE_HIGH:
			res = ID_HIGH;
			break;
		case GUI_TYPE_IR:
			res = ID_IRRX;
			break;
		case GUI_TYPE_PERSONBL:
			res = 0X87;
			break;
		case GUI_TYPE_ALARMBL:
			res = 0X8C;
			break;
		case GUI_TYPE_HIGHBL:
			res = 0X8B;
			break;
		default:
			break;
	}	
	return res;
}


void CanInitRecvFileQueue(void)
{
	memset(&canRecvProgram,0x00,sizeof(STR_CAN_RECV_PROGRAM));
	SdramFileSavept = 0;
	FatfsFileSavept = 0;
}


void UdpInitRecvFileQueue(void)
{
	memset(&udpRecvFile,0x00,sizeof(STR_UDP_UPDATE_FILE));
}


uint8_t getProLoadStatus()
{
	return canRecvProgramStatus;
}
void setProLoadStatus(uint8_t status)
{
	canRecvProgramStatus = status;
}


uint8_t getFileRecvStatus()
{
	return udpRecvFileStatus;
}
void setFileRecvStatus(uint8_t status)
{
	udpRecvFileStatus = status;
}



void CanWriteProgramQueueProc(uint8_t * buff,uint16_t size)
{
	memcpy(FileSaveAddress+SdramFileSavept,buff,size);
	SdramFileSavept += size;
}

uint32_t UpdateTimerApi()
{
	return updateTimer;
}

void setUpdateTImerApi(uint32_t timer)
{
	updateTimer = timer;
}

/*******************************************************************************************
**函数名称：CanRecvProgramTimeout_callBack
**函数作用：接收can程序棒超时回调
**函数参数：无
**函数输出：无
**注意事项：无
*******************************************************************************************/
void CanRecvProgramTimeout_callBack(uint16_t id)
{
	uint8_t nameTmp[30] = {0};
	switch(getProLoadStatus())
	{
		case PROLOADING:
			Fatfs_closeFile();
			DeleteDirFile(fatfsFilename);
			setProLoadStatus(PROLOADBREAK);
			ee_WriteBytes(nameTmp, FILE_RECV_COMPLETE_START, 30);
			stopTimerEvent(CanRecvProgramTimeoutId);
			deleteTimerEvent(CanRecvProgramTimeoutId);
			break;
	}
}
 

/*******************************************************************************************
**函数名称：CanRecvVersionProc
**函数作用：接收can程序棒的版本信息
**函数参数：无
**函数输出：无
**注意事项：无
*******************************************************************************************/
void CanRecvVersionProc(void)
{
	uint8_t typetmp;
	uint32_t versiontmp;
	RTC_TimeData   time;
	char month[3];
	char data[3];
	Get_Data_Time(&time);
	
	CanRecvProgramTimeoutId = addTimerEvent(20000,0xFF,CanRecvProgramTimeout_callBack);
	startTimerEvent(CanRecvProgramTimeoutId);
	
	
	typetmp = (FileSaveAddress[0]&0xFF);
	versiontmp =FileSaveAddress[11]<<24|FileSaveAddress[10]<<16|FileSaveAddress[9]<<8|FileSaveAddress[8];
	canRecvProgram.fileCrc = FileSaveAddress[29]<<8|FileSaveAddress[28];
	canRecvProgram.fileSize =FileSaveAddress[15]<<24|FileSaveAddress[14]<<16|FileSaveAddress[13]<<8|FileSaveAddress[12];
	
	updateTimer = 0;
	
	if(typetmp>LOAD_TYPE_ALARM_BL)
		return;
	if(typetmp == LOAD_TYPE_SC)//不是V4程序
	{
		if(FileSaveAddress[11] != 8)
		{
			setProLoadStatus(PROLOADCRCERROR);
			return;
		}
	}
	if(time.sys_data.RTC_Month<10)
		sprintf(month,"%s%d","0",time.sys_data.RTC_Month);
	else
		sprintf(month,"%d",time.sys_data.RTC_Month);
	
	if(time.sys_data.RTC_Date<10)
		sprintf(data,"%s%d","0",time.sys_data.RTC_Date);
	else
		sprintf(data,"%d",time.sys_data.RTC_Date);
	
	
	
	sprintf(canRecvProgram.fileName,"%s%d%d%d%d%s%d%s%s%s", getFatfsFileName(canRecvProgram.cxb_recv_device),(versiontmp>>24)&0xFF,(versiontmp>>16)&0xFF,\
		(versiontmp>>8)&0xFF,versiontmp&0xFF,"_",time.sys_data.RTC_Year,month,data,".bin");
	memset(fatfsFilename,0,sizeof(fatfsFilename));
	sprintf(fatfsFilename,"%s%s", "0:update/",canRecvProgram.fileName);
	setProLoadStatus(PROLOADING);
	
	ee_WriteBytes((u8*)fatfsFilename, FILE_RECV_COMPLETE_START, 30);
}


/*******************************************************************************************
**函数名称：CanRecvProgramProc
**函数作用：接收can程序棒的程序
**函数参数：无
**函数输出：无
**注意事项：无
*******************************************************************************************/
void CanRecvProgramProc(void)
{
	uint32_t sizeTmp = 0;
	uint16_t i;
	uint16_t writeconts = 0;
	uint8_t nameTmp[30] = {0};
	#if OS_CRITICAL_METHOD == 3u                     /* Allocate storage for CPU status register           */
			OS_CPU_SR  cpu_sr = 0u;
	#endif
	if(getProLoadStatus() == PROLOADCRCERROR)
		return;
	startTimerEvent(CanRecvProgramTimeoutId);
//	if(SdramFileSavept > FatfsFileSavept)
//	{
//		sizeTmp = SdramFileSavept - FatfsFileSavept;
//		Fatfs_writeFile(FileSaveAddress+FatfsFileSavept,sizeTmp);
//		FatfsFileSavept += sizeTmp;
//	}
	updateTimer = SdramFileSavept*100/canRecvProgram.fileSize;
	
	if(SdramFileSavept == (canRecvProgram.fileSize+RECV_SAVE_PROGRAM_QUEUE_MAX))
	{	
//		Fatfs_closeFile();
		stopTimerEvent(CanRecvProgramTimeoutId);
		deleteTimerEvent(CanRecvProgramTimeoutId);
		
//		canRecvProgram.calcCrc = 0;
//		Crc16Ccitt(FileSaveAddress+RECV_SAVE_PROGRAM_QUEUE_MAX, canRecvProgram.fileSize, &canRecvProgram.calcCrc);	
		
		
		OS_ENTER_CRITICAL();		
		
		Fatfs_createFile(fatfsFilename);
		Fatfs_openFile(fatfsFilename,1);
		
		writeconts = SdramFileSavept/(32*1024)+1;
		for(i = 0;i<writeconts;i++)
		{
			WDTReload(0);
			if(SdramFileSavept == FatfsFileSavept)
				break;
			if((SdramFileSavept - FatfsFileSavept)<(32*1024))
			{
				sizeTmp = SdramFileSavept - FatfsFileSavept;
				Fatfs_writeFile(FileSaveAddress+FatfsFileSavept,sizeTmp);
				FatfsFileSavept += sizeTmp;
			}
			else
			{	
				Fatfs_writeFile(FileSaveAddress+FatfsFileSavept,32*1024);
				FatfsFileSavept+=(32*1024);
			}
		}
		WDTReload(0);
		
		Fatfs_closeFile();
		
		OS_EXIT_CRITICAL();
		Fatfs_openFile(fatfsFilename,0);
		canRecvProgram.calcCrc = 0;
		memset(FileSaveAddress,0,canRecvProgram.fileSize+RECV_SAVE_PROGRAM_QUEUE_MAX);
		OS_ENTER_CRITICAL();
		Fatfs_readFile(fatfsFilename,FileSaveAddress,canRecvProgram.fileSize+RECV_SAVE_PROGRAM_QUEUE_MAX);
		Fatfs_closeFile();
		Crc16Ccitt(FileSaveAddress+RECV_SAVE_PROGRAM_QUEUE_MAX, canRecvProgram.fileSize, &canRecvProgram.calcCrc);
		OS_EXIT_CRITICAL();
		
		if(canRecvProgram.calcCrc == canRecvProgram.fileCrc)
		{
			setProLoadStatus(PROLOADSUCCESS);
			deleteMoreFile(canRecvProgram.cxb_recv_device);
		}
		else
		{
			setProLoadStatus(PROLOADCRCERROR);
			DeleteDirFile(fatfsFilename);
		}
		ee_WriteBytes(nameTmp, FILE_RECV_COMPLETE_START, 30);
	}
}

/*******************************************************************************************
**函数名称：UdpRecvFileReadyProc
**函数作用：接收udp的文件头信息
**函数参数：无
**函数输出：无
**注意事项：无
*******************************************************************************************/
void UdpRecvFileReadyApi(uint8_t *data, uint16_t len,uint8_t address)
{ 
	if(len<30)
		return;
	UdpInitRecvFileQueue();
	memcpy(&udpRecvFile.udpUpdateReady,data,sizeof(UDP_UPDATE_READY));
	if(udpRecvFile.udpUpdateReady.u32FileSize == 0)
		return;
	udpRecvFileSem = UPDLOADHEADSEM;
	udpRecvFile.udpRecvFileAddress = address;
	
	SdramFileSavept = 0;
	FatfsFileSavept = 0;
	OSMboxPost(revProgramSem,(void*)udpRecvFileSem);
}


/*******************************************************************************************
**函数名称：UdpRecvFileDataApi
**函数作用：接收udp的文件数据
**函数参数：无
**函数输出：无
**注意事项：无
*******************************************************************************************/
void UdpRecvFileDataApi(uint8_t nowpackNum, uint8_t *data, uint16_t len,uint8_t isFin)
{	
	if(udpRecvFile.u16FilePackNum/MAX_NUMBER)
		udpRecvFile.u16FilePackNum = nowpackNum+MAX_NUMBER*(udpRecvFile.u16FilePackNum/MAX_NUMBER);
	else
		udpRecvFile.u16FilePackNum = nowpackNum;
	
	SdramFileSavept = (udpRecvFile.u16FilePackNum-1)*SC_SAVE_PROGRAM_QUEUE_SINGLE;
		
	memcpy(FileSaveAddress+SdramFileSavept,data,len);
	udpRecvFileSem = UPDLOADPROGRAMSEM;
	OSMboxPost(revProgramSem,(void*)udpRecvFileSem);
}


/*******************************************************************************************
**函数名称：UdpRecvFileFinCrcApi
**函数作用：接收udp的最后一帧
**函数参数：无
**函数输出：无
**注意事项：无
*******************************************************************************************/
void UdpRecvFileFinCrcApi(uint16_t recvCrc)
{
	udpFileCrc = recvCrc;
	udpRecvFileSem = UDPLOADFINCRCSEM;
	OSMboxPost(revProgramSem,(void*)udpRecvFileSem);
}


/*******************************************************************************************
**函数名称：UdpRecvFileUpdateStatusApi
**函数作用：接收udp的更新状态
**函数参数：无
**函数输出：无
**注意事项：无
*******************************************************************************************/
void UdpRecvFileUpdateStatusApi(uint16_t updateStatus)
{
	setFileRecvStatus(updateStatus);
//	udpRecvFileSem = UDPLOADUPDATESEM;
//	OSMboxPost(revProgramSem,(void*)UDPLOADUPDATESEM);
}


/*******************************************************************************************
**函数名称：UdpRecvFileTimeout_callBack
**函数作用：接收udp的超时
**函数参数：无
**函数输出：无
**注意事项：无
*******************************************************************************************/
void UdpRecvFileTimeout_callBack(uint16_t id)
{
//	udpUpdateApiType frameInfo;
	uint8_t filename[30] = {0};
	/*udp发送crc*/
	
//	frameInfo.ack =0;
//	frameInfo.curNum=1;
//	frameInfo.totalNum=1;
//	UdpUpdateProcCtrl(UpdateProcIDType_RecvDataAck_request,(void *)(&udpRecvFile.u16CRC),udpRecvFile.udpRecvFileAddress,device,frameInfo);
//	
	udpRecvFile.UdpRecvFileCounts = 0;
	Fatfs_closeFile();
	DeleteDirFile(fatfsFilename);
	ee_WriteBytes(filename, FILE_RECV_COMPLETE_START, sizeof(filename));
	setFileRecvStatus(FILEREVFAIL);
	deleteTimerEvent(UdpRecvFileTimeoutId);
	deleteTimerEvent(UdpRecvFileLongTimeoutId);
	WriteLog_API(0,LOG_TYPE_UPDATE_PROGRAM,LOG_STAUTS_UPDATE_PRO_FAIL,0,0);
	
//	setFileRecvStatus(FILECRCFAIL);
}



/*******************************************************************************************
**函数名称：UdpRecvFileReadyProc
**函数作用：接收udp的头帧处理
**函数参数：无
**函数输出：无
**注意事项：无
*******************************************************************************************/
void UdpRecvFileReadyProc()
{		
	char names[30] = {0};
	if(udpRecvFile.udpUpdateReady.u32FileSize == 0)
		return;
	if(udpRecvFile.udpUpdateReady.u32FileSize > 1024*1024)
		return;
	
	UdpRecvFileLongTimeoutId = addTimerEvent(20000,0xFF,UdpRecvFileTimeout_callBack);
	startTimerEvent(UdpRecvFileLongTimeoutId);
	
	if(udpRecvFile.udpUpdateReady.u8FileSuffixType[0] == 'b'||udpRecvFile.udpUpdateReady.u8FileSuffixType[0] == 't')//bin文件
	{
		if(checkBinFileName(udpRecvFile.udpUpdateReady.u8FileName) == 0)
		{
			if((udpRecvFile.udpUpdateReady.u8FileName[0] == 'y') && (udpRecvFile.udpUpdateReady.u8FileName[1] == 'k'))
				;
			else
			{
				udpRecvFile.udpUpdateReady.u32FileSize = 0;
				return;
			}
		}

		updRecvParaFlag = FILETYPE;
		sprintf(names,"%s%s%s", udpRecvFile.udpUpdateReady.u8FileName, ".",udpRecvFile.udpUpdateReady.u8FileSuffixType);
		memset(fatfsFilename,0,sizeof(fatfsFilename));
		sprintf(fatfsFilename,"%s%s","0:update/",names);

	}
	else if(udpRecvFile.udpUpdateReady.u8FileSuffixType[0] == 'p'&& udpRecvFile.udpUpdateReady.u8FileSuffixType[1] == 'n')//png文件
	{
		sprintf(names,"%s%s%s", udpRecvFile.udpUpdateReady.u8FileName, ".",udpRecvFile.udpUpdateReady.u8FileSuffixType);
		memset(fatfsFilename,0,sizeof(fatfsFilename));
		sprintf(fatfsFilename,"%s%s","0:",names);
	}
	else if(udpRecvFile.udpUpdateReady.u8FileSuffixType[0] == 'p'&&udpRecvFile.udpUpdateReady.u8FileSuffixType[1] == 'a')
	{
		updRecvParaFlag = PARATYPE;
	}
	else
	{
		udpRecvFile.udpUpdateReady.u32FileSize = 0;
		return;		
	}

	updateTimer = 0;
	setFileRecvStatus(FILEREADY);
}

/*******************************************************************************************
**函数名称：UdpRecvFileDataProc
**函数作用：接收udp的数据处理
**函数参数：无
**函数输出：无
**注意事项：无
*******************************************************************************************/
void UdpRecvFileDataProc()
{
	udpUpdateApiType frameInfo;
	uint32_t sizeTmp = 0;

	if(udpRecvFile.udpUpdateReady.u32FileSize == 0)
		return;
	if(udpRecvFile.udpUpdateReady.u32FileSize > 1024*1024)
		return;
	if(udpRecvFile.udpUpdateReady.u16FrameType != FILEDATA)
	{
		udpRecvFile.udpUpdateReady.u16FrameType = FILEDATA;
		UdpRecvFileTimeoutId = addTimerEvent(5000,0xFF,UdpRecvFileTimeout_callBack);
		setFileRecvStatus(FILEDATA);
	}
	startTimerEvent(UdpRecvFileTimeoutId);
	startTimerEvent(UdpRecvFileLongTimeoutId);
	udpRecvFile.UdpRecvFileCounts++;
	updateTimer = udpRecvFile.u16FilePackNum*SC_SAVE_PROGRAM_QUEUE_SINGLE*100/udpRecvFile.udpUpdateReady.u32FileSize;
	if(updateTimer>100)
		updateTimer = 100;
	
	if(udpRecvFile.u16FilePackNum*SC_SAVE_PROGRAM_QUEUE_SINGLE >= udpRecvFile.udpUpdateReady.u32FileSize)
	{		
		stopTimerEvent(UdpRecvFileTimeoutId);
		udpRecvFile.u16CRC = 0;
		SdramFileSavept = udpRecvFile.udpUpdateReady.u32FileSize;
//		if(SdramFileSavept > FatfsFileSavept && updRecvParaFlag == FILETYPE)
//		{
//			sizeTmp = SdramFileSavept - FatfsFileSavept;
//			Fatfs_writeFile(FileSaveAddress+FatfsFileSavept,sizeTmp);
//			FatfsFileSavept += sizeTmp;
//		}
	}
	else if(udpRecvFile.UdpRecvFileCounts == UDP_SEND_CRC_COUNT)
	{
		
		WDTReload(0);
		stopTimerEvent(UdpRecvFileTimeoutId);
		//计算5帧的校验
		udpRecvFile.tmpCRC = 0;
		Crc16Ccitt(FileSaveAddress+SdramFileSavept-(UDP_SEND_CRC_COUNT-1)*SC_SAVE_PROGRAM_QUEUE_SINGLE,UDP_SEND_CRC_COUNT*SC_SAVE_PROGRAM_QUEUE_SINGLE,&udpRecvFile.tmpCRC);
		
		/*udp发送crc*/	
		frameInfo.ack =0;
		frameInfo.curNum=1;
		frameInfo.totalNum=1;
		UdpUpdateProcCtrl(UpdateProcIDType_RecvDataAck_request,(void *)(&udpRecvFile.tmpCRC),2,udpRecvFile.udpRecvFileAddress,frameInfo);
		
		//存数据
//		if(SdramFileSavept > FatfsFileSavept && updRecvParaFlag == FILETYPE)
//		{
//			sizeTmp = SdramFileSavept - FatfsFileSavept;
//			Fatfs_writeFile(FileSaveAddress+FatfsFileSavept,sizeTmp);
//			FatfsFileSavept += sizeTmp;
//		}			
		udpRecvFile.UdpRecvFileCounts = 0;
		stopTimerEvent(UdpRecvFileTimeoutId);
	}
}

/*******************************************************************************************
**函数名称：UdpRecvFileFinCrcProc
**函数作用：接收udp的数据最后一帧
**函数参数：无
**函数输出：无
**注意事项：无
*******************************************************************************************/
void UdpRecvFileFinCrcProc()
{
	udpUpdateApiType frameInfo;
	uint8_t filename[30] = {0};
	uint16_t crcRes = 0;
	uint16_t i,writecounts;
	uint16_t sizeTmp;
	
	#if OS_CRITICAL_METHOD == 3u                     /* Allocate storage for CPU status register           */
			OS_CPU_SR  cpu_sr = 0u;
	#endif
	
	if(udpRecvFile.udpUpdateReady.u32FileSize == 0)
		return;
	
	deleteTimerEvent(UdpRecvFileTimeoutId);
	deleteTimerEvent(UdpRecvFileLongTimeoutId);
	
	if(updRecvParaFlag == FILETYPE)
	{	
		udpRecvFile.u16CRC = 0;
		Crc16Ccitt(FileSaveAddress,udpRecvFile.udpUpdateReady.u32FileSize,&udpRecvFile.u16CRC);
		
		
		if(udpFileCrc == udpRecvFile.u16CRC)
			crcRes = 0;
		else
			crcRes = 1;
		
		/*udp发送crc*/	
		frameInfo.ack =0;
		frameInfo.curNum=1;
		frameInfo.totalNum=1;
		UdpUpdateProcCtrl(UpdateProcIDType_DataCRC_confirm,(void *)(&crcRes),2,udpRecvFile.udpRecvFileAddress,frameInfo);	
			
		if(udpFileCrc != udpRecvFile.u16CRC)
		{
			setFileRecvStatus(FILEREVFAIL);
			return;
		}
		
		ee_WriteBytes((u8 *)fatfsFilename, FILE_RECV_COMPLETE_START, 30);
		OS_ENTER_CRITICAL();
		
		Fatfs_createFile(fatfsFilename);
		Fatfs_openFile(fatfsFilename,1);
		
		writecounts = SdramFileSavept/(32*1024)+1;
		for(i = 0;i<writecounts;i++)
		{
			WDTReload(0);
			if(SdramFileSavept == FatfsFileSavept)
				break;
			if((SdramFileSavept - FatfsFileSavept)<(32*1024))
			{
				sizeTmp = SdramFileSavept - FatfsFileSavept;
				Fatfs_writeFile(FileSaveAddress+FatfsFileSavept,sizeTmp);
				FatfsFileSavept += sizeTmp;
			}
			else
			{	
				Fatfs_writeFile(FileSaveAddress+FatfsFileSavept,32*1024);
				FatfsFileSavept+=(32*1024);
			}
		}
		WDTReload(0);		
		Fatfs_closeFile();

		OS_EXIT_CRITICAL();
		
		memset(FileSaveAddress,0,sizeof(FileSaveAddress));
		Fatfs_openFile(fatfsFilename,0);
		Fatfs_readFile(fatfsFilename,FileSaveAddress,udpRecvFile.udpUpdateReady.u32FileSize);
		Fatfs_closeFile();
		udpRecvFile.u16CRC = 0;
		Crc16Ccitt(FileSaveAddress,udpRecvFile.udpUpdateReady.u32FileSize,&udpRecvFile.u16CRC);
		
		if(SdramFileSavept == 0)
			return;		
		
		if(udpFileCrc == udpRecvFile.u16CRC)
			crcRes = 0;
		else
			crcRes = 1;
				
		ee_WriteBytes(filename, FILE_RECV_COMPLETE_START, 30);
		
		if(udpFileCrc != udpRecvFile.u16CRC)
		{
			DeleteDirFile(fatfsFilename);
			setFileRecvStatus(FILEREVFAIL);
			WriteLog_API(0,LOG_TYPE_UPDATE_PROGRAM,LOG_STAUTS_UPDATE_PRO_FAIL,0,0);
		}
		else
		{
			WriteLog_API(0,LOG_TYPE_UPDATE_PROGRAM,LOG_STATUS_UPDATE_PRO_SUCCESS,0,0);			
			UdpRecvFileUpdateStatusProc(FileSaveAddress[0], udpRecvFile.udpUpdateReady.u8FileSuffixType[0],fatfsFilename);
			setFileRecvStatus(FILEREVSUCCESS);		
		}
	}
	else if(updRecvParaFlag == PARATYPE)
	{
		udpRecvFile.u16CRC = 0;
		updateTimer = 100;
		Crc16Ccitt(FileSaveAddress,udpRecvFile.udpUpdateReady.u32FileSize,&udpRecvFile.u16CRC);
		if(udpRecvFile.u16CRC == udpFileCrc)
		{
			setFileRecvStatus(FILESAVING);
			if(udpRecvFile.udpUpdateReady.u16FileType == PARAALL)
			{
				SysDataFun_ext(DATA_MSG_SET_ALL_PARAM, udpRecvFile.udpUpdateReady.u32FileSize,FileSaveAddress,0);
				setFileRecvStatus(FILEREVSUCCESS);
			}
			else
			{
				SysDataFun_ext(DATA_MSG_SET_GLOBAL_PARAM, udpRecvFile.udpUpdateReady.u32FileSize,FileSaveAddress,0);
				setFileRecvStatus(FILEREVSUCCESS);
			}
			WriteLog_API(0,LOG_TYPE_UPDATE_PROGRAM,LOG_STATUS_UPDATE_PARA_SUCCESS,0,0);
		}
		else
		{
			setFileRecvStatus(FILEREVFAIL);
			WriteLog_API(0,LOG_TYPE_UPDATE_PROGRAM,LOG_STATUS_UPDATE_PARA_FAIL,0,0);
		}
	}
}

uint8_t deviceTypeToUIList(uint8_t device)
{
	uint8_t res= 0xFF;
	switch(device)
	{
		case LOAD_TYPE_SC:
			res = GUI_TYPE_SC;
			break;
		case LOAD_TYPE_EMVD:
			res = GUI_TYPE_EMVD;
			break;
		case LOAD_TYPE_ANGLE:
			res = GUI_TYPE_ANGLE;
			break;
		case LOAD_TYPE_PERSON:
			res = GUI_TYPE_PERSON;
			break;
		case LOAD_TYPE_ALARM:
			res = GUI_TYPE_ALARM;
			break;
		case LOAD_TYPE_HIGH:
			res = GUI_TYPE_HIGH;
			break;
		case LOAD_TYPE_IR:
			res = GUI_TYPE_IR;
			break;
		case LOAD_TYPE_KEYBOARD:
			res = GUI_TYPE_KEYBOARD;
			break;
		case LOAD_TYPE_WL:
			res = GUI_TYPE_WIRELESS;
			break;			
		case LOAD_TYPE_PERSON_BL:
			res = GUI_TYPE_PERSONBL;
			break;		
		case LOAD_TYPE_ALARM_BL:
			res = GUI_TYPE_ALARMBL;
			break;			
		case LOAD_TYPE_HIGH_BL:
			res = GUI_TYPE_HIGHBL;
			break;	
		case LOAD_TYPE_ALARM_UWB_ANGLE: //多功能声光报警器
			res = GUI_TYPE_ALARM_UWB_ANGLE;
			break;	
		case LOAD_TYPE_MINIHUB: //minihub设备
			res = GUI_TYPE_HUB;
			break;
	}
	return res;
}

/*******************************************************************************************
**函数名称：deleteMoreFile
**函数作用：删除多余的文件
**函数参数：无
**函数输出：无
**注意事项：无
*******************************************************************************************/
void deleteMoreFile(uint8_t fileType)
{
	uint8_t fileSize = 0;
	uint8_t i,j;
	uint32_t data[3] = {0};
	uint32_t index = 0;
	char* res[10];
	char name[40];
	fileSize = Fatfs_FindDir("0:/update",res,1,fileType);
	if(fileSize >= 3)	
	{
		for(i = 0;i<fileSize;i++)
		{
			sprintf(name,"%s%s","0:update/",res[i]);
			if(strcmp(fatfsFilename,name) == 0)
			{
				data[i] = 0;
				continue;
			}
			if(i == 4)
				break;
			j = 0;
			while(res[i][j] != '\0')
			{
				if(res[i][j] == '_')
				{
					data[i]  = ((res[i][j+1]-0x30)*10+(res[i][j+2]-0x30))<<16;
					data[i] |= ((res[i][j+3]-0x30)*10+(res[i][j+4]-0x30))<<8;
					data[i] |= ((res[i][j+5]-0x30)*10+(res[i][j+6]-0x30));
					break;
				}
				j++;
			}
		}
		j = 0;
		for(i = 0;i<fileSize;i++)
		{
			if(data[i] == 0)
				continue;
			if(index == 0)
			{
				j = i;
				index = data[i];
			}
			else
			{
				if(index<data[i])
				{
					j = i;
					index = data[i];
				}
			}
		}
		sprintf(name,"%s%s","0:update/",res[j]);
		DeleteDirFile(name);
	}
}
	

/*******************************************************************************************
**函数名称：UdpRecvFileUpdateStatusProc
**函数作用：接收udp的是否更新
**函数参数：无
**函数输出：无
**注意事项：无
*******************************************************************************************/
void UdpRecvFileUpdateStatusProc(uint8_t deviceType, uint8_t fileSuffixType, char * fileNames)
{
	char name[30];
	uint8_t       JsonSuccess;
	uint32_t deviceTypetmp = 0;
	uint8_t jsonflag[4];

	
	if(fileSuffixType == 'b')
	{
		if(*(fileNames+9) == 'y'&& *(fileNames+10) == 'k')
		{
			NVIC_SystemReset();
			return;
		}
		deviceTypetmp = deviceTypeToUIList(deviceType);
		if(deviceType == 0)
		{
			name[0] = 1;
			memcpy(name+1,(fileNames+9),sizeof(name));
			W25QXX_Erase_Sector(0x1F80000/4096);
			W25QXX_Write_NoCheck((uint8_t *)name,0x1F80000,40);
			deleteMoreFile(deviceTypetmp);
			ETH_Stop();
			BCM_551CS_LOW();
			NVIC_SystemReset();
		}
		else if(deviceTypetmp == GUI_TYPE_KEYBOARD ||deviceTypetmp == GUI_TYPE_WIRELESS)//键盘，无线
		{		
			setFileRecvStatus(FILESENDING);
			memcpy(name,(fileNames+9),sizeof(name));
			deleteMoreFile(deviceTypetmp);
			UsartSendDataProc(name,deviceTypetmp);
		}
		else
		{
			setFileRecvStatus(FILESENDING);
			memcpy(name,(fileNames+9),sizeof(name));
			deleteMoreFile(deviceTypetmp);
			CanSendDataProc(name,deviceTypetmp);
		}
	}
	else if(fileSuffixType == 't')//para.txt
	{
		ee_WriteBytes(fileNames, JSON_PARAM_USING_START, 30);
			
		//先校验文件CRC
		//写入json接收成功标志
		JsonSuccess = 0;
		ee_WriteBytes((uint8_t *)(&JsonSuccess), JSON_FILE_RECEPTION_SUCCESS, 1);
		
		deleteMoreFile(deviceTypetmp);
		ETH_Stop();
		BCM_551CS_LOW();
		NVIC_SystemReset();
	}
	else if(fileSuffixType == 'p')
	{
//		ETH_Stop();
//		BCM_551CS_LOW();
//		NVIC_SystemReset();
	}
}

/*******************************************************************************************
**函数名称：CanRequestProgramProc
**函数作用：UI调用程序棒下载程序接口
**函数参数：无
**函数输出：无
**注意事项：无
*******************************************************************************************/
void CanRequestProgramProc(uint16_t device)
{
	uint8_t driver;
	if(GetDeviceCanMsg(ID_CXB,0,&driver) == 0)
	{
		setProLoadStatus(CXBUNCONNECT);
		return;
	}
	CanInitRecvFileQueue();
	updateTimer = 0;
	CanProSend(driver,ID_CXB,0,0,device,0);
	canRecvProgram.cxb_recv_device = device;
}

void FileLoad_task()
{
	INT8U err;
	uint8_t u8Way;
	char* res[10];
	char filename[30];
	
	u16 writeconts,i;
	u32 sizeTmp;
//	uint32_t size = 0;
//	uint8_t data[6] = {1,2,3,4,5,6};
	
//	for(writeconts = 0;writeconts<6400;writeconts++)
//	{
//		W25QXX_Erase_Sector(writeconts);
//	}
	
//	Fatfs_Mkfs();//需要关看门狗
//	Fatfs_Getfree();
//	memset(FileSaveAddress,0,sizeof(FileSaveAddress));
	Fatfs_CreateDir("update");//创建data文件夹
//	
//	size = Fatfs_openFile("0:华宁电子图标.png",0);//打开
//	Fatfs_readFile("0:华宁电子图标.png",FileSaveAddress,size);//读
//	Fatfs_closeFile();//关文件
//	Fatfs_createFile("0:data/华宁电子图标.png");//创建文件
//	Fatfs_openFile("0:data/华宁电子图标.png",1);//打开文件
//	Fatfs_writeFile(FileSaveAddress,size);//写入
//	Fatfs_closeFile();//关闭
//	Fatfs_FindDir("0:/data",res,1,0);//查询
//	Fatfs_FindDir("0:/update",res,1,0);
//	Fatfs_FindDir("0:",res,1,0);
//	OSTimeDly(6000);
//	DeleteDirFile("角  度_V3111.bin");
//	DeleteDirFile("控制器_V1234.bin");
//	DeleteDirFile("0:/update/可编程_V0001.txt");
//	DeleteDirFile("0:/update/可编程_V0002.txt");
//	DeleteDirFile("0:/update/声  光_V1020.bin");
//	DeleteDirFile("0:/update/声  光_V0666.bin");
//	DeleteDirFile("0:/update/控制器_V8113.bin");
//	DeleteDirFile("0:/update/控制器_V8100.bin");
//	DeleteDirFile("0:/update/人  员_V2190.bin");
//	DeleteDirFile("0:/update/人  员_V0229.bin");
//	DeleteDirFile("0:/update/角  度_V3111.bin");
//	DeleteDirFile("0:/update/高  度_V1111.bin");
//	DeleteDirFile("0:/update/键  盘_V1111.bin");
//	DeleteDirFile("0:/update/声光BL_V0102.bin");
//	DeleteDirFile("0:/update/声光BL_V0111.bin");
//	DeleteDirFile("0:/update/电磁阀_V0264.bin");
//	DeleteDirFile("0:/update/电磁阀_V0101.bin");
//	DeleteDirFile("0:/update/红外RX_V0100.bin");
//	DeleteDirFile("0:/update/红外RX_V0203.bin");
//	DeleteDirFile("0:/update/wx1111_210218.bin");
	DeleteDirFile("0:/update/dgns0405_230807.bin");
	DeleteDirFile("0:/update/dgns0407_230809.bin");
//	DeleteDirFile("0:/update/8105.bin");
//	DeleteDirFile("0:/update/7654.bin");
//	DeleteDirFile("0:/update/6543.bin");
//	DeleteDirFile("0:/update/area1_sc_1.txt");
//	DeleteDirFile("0:/update/ykqpara.bin");
//	filedir_delete("0:/update");
//    SdramFileSavept = 0x123456;
//		Fatfs_createFile("0:update/测试fatfs文件名测试fatfs文件名11");
//		Fatfs_openFile("0:update/测试fatfs文件名测试fatfs文件名11",1);
//		
//		writeconts = SdramFileSavept/(32*1024)+1;
//		for(i = 0;i<writeconts;i++)
//		{
//			WDTReload(0);
//			if(SdramFileSavept == FatfsFileSavept)
//				break;
//			if((SdramFileSavept - FatfsFileSavept)<(32*1024))
//			{
//				sizeTmp = SdramFileSavept - FatfsFileSavept;
//				Fatfs_writeFile(FileSaveAddress+FatfsFileSavept,sizeTmp);
//				FatfsFileSavept += sizeTmp;
//			}
//			else
//			{	
//				Fatfs_writeFile(FileSaveAddress+FatfsFileSavept,32*1024);
//				FatfsFileSavept+=(32*1024);
//			}
//		}
//		WDTReload(0);
//		
//		Fatfs_closeFile();
//	Fatfs_DeleteDirFile("0:update");
//	Fatfs_Getfree();
//	Fatfs_FindDir("0:/update",res,1,0);
	ee_ReadBytes((uint8_t *)filename,FILE_RECV_COMPLETE_START,sizeof(filename));
	DeleteDirFile(filename);
	
	while(1)
	{
		u8Way = ((uint8_t)OSMboxPend(revProgramSem,0,&err));
		(FileRecvProc[u8Way])();
	}
}











