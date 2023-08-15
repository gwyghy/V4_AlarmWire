/*
*********************************************************************************************************
*	                                            V4
*                           Commuication Controller For The Mining Industry
*
*                                  (c) Copyright 1994-2013  HNDZ 
*                       All rights reserved.  Protected by international copyright laws.
*
*
*    File    : FileLoad.h
*    Module  : user
*    Version : V1.0
*    History :
*   -----------------
*              Version  Date           By            Note
*              v1.0         
*
*********************************************************************************************************
*/

#ifndef __FILELOAD_H__
#define __FILELOAD_H__


#define RECV_SAVE_PROGRAM_QUEUE_MAX 256
#define UDP_RECV_FILE_QUEUE_SIZE  10240
#define PROGRAM_TYPE   0x00100A00
#define FILENAMESIZE   20
#define FILESUFFIXTYPE   4

#define MAX_NUMBER    255

enum
{
	CANLOADHEADSEM = 1,
	CANLOADPROGRAMSEM = 2,
	UPDLOADHEADSEM = 3,
	UPDLOADPROGRAMSEM = 4,
	UDPLOADFINCRCSEM = 5,
	UDPLOADUPDATESEM = 6,
	SENDFILESEM = 7,
	SENDYKQPARASEM = 8,
	SENDALARMYKQPARASEM,
};

enum
{
	FILENORMAL = 0,
	FILEREADY = 1,//准备接收
	FILEDATA = 2,//正在接收
	FILECRCSUCCESS = 3,//校验正确
	FILECRCFAIL = 4,//校验错误
	FILEREVSUCCESS = 5,//接收成功
	FILEREVFAIL = 6,//接收错误
	FILESAVING = 7,//正在保存（接收参数使用）
	FILESENDING = 8,//正在发送
	FILENORMALSTATUS = 0xFF,//正常状态
};


enum
{
	CXBUNCONNECT    = 0,//未连接
	CXBCONNECTED    = 1,// 连接
	REQPROLOAD      = 2,//准备下载
	PROLOADING      = 3,//正在下载
	NOPROTYPE       = 4,//没有该类型程序
	PROLOADBREAK    = 5,//下载中断
	PROLOADCRCERROR = 6,//接收错误
	PROLOADSUCCESS  = 7,//接收成功
	NORMALSTATUS = 0xFF,//正常状态
};


enum{
	
	LOAD_TYPE_SC = 0x00,
	LOAD_TYPE_EMVD = 0x01,
	LOAD_TYPE_HUB = 0x02,
	LOAD_TYPE_IR = 0x03,
	LOAD_TYPE_CXB = 0x04,
	LOAD_TYPE_ANGLE = 0x05,
	LOAD_TYPE_SERVER = 0x06,
	LOAD_TYPE_PERSON = 0x07,
	LOAD_TYPE_WIRELESS = 0x08,
	LOAD_TYPE_KEYBOARD = 0x09,
	LOAD_TYPE_WL = 0x0A,
	LOAD_TYPE_HIGH = 0x0B,
	LOAD_TYPE_ALARM = 0x0C,
	LOAD_TYPE_ALARM_UWB_ANGLE = 0x0E,//ghy 增加多功能声光报警器类型
	LOAD_TYPE_MINIHUB = 0x0F        //ghy 增加MiniHUB设备类型
};


enum{
	LOAD_TYPE_SC_BL = 0x80,
	LOAD_TYPE_EMVD_BL = 0x81,
	LOAD_TYPE_HUB_BL = 0x82,
	LOAD_TYPE_IR_BL = 0x83,
	LOAD_TYPE_CXB_BL = 0x84,
	LOAD_TYPE_ANGLE_BL = 0x85,
	LOAD_TYPE_SERVER_BL = 0x86,
	LOAD_TYPE_PERSON_BL = 0x87,
	LOAD_TYPE_ROMOTE_BL = 0x88,
	LOAD_TYPE_KEYBOARD_BL = 0x89,
	LOAD_TYPE_WL_BL = 0x8A,
	LOAD_TYPE_HIGH_BL = 0x8B,
	LOAD_TYPE_ALARM_BL = 0x8C

};
enum
{
	GUI_TYPE_SC = 0,
	GUI_TYPE_EMVD = 1,
	GUI_TYPE_ANGLE = 2,
	GUI_TYPE_PERSON = 3,
	GUI_TYPE_ALARM = 4,
	GUI_TYPE_HIGH = 5,
	GUI_TYPE_IR = 6,
	GUI_TYPE_KEYBOARD = 7,
	GUI_TYPE_WIRELESS = 8,
	GUI_TYPE_PROGRAM = 9,
	GUI_TYPE_PERSONBL = 10,
	GUI_TYPE_ALARMBL = 11,
	GUI_TYPE_HIGHBL = 12,
	GUI_TYPE_ALARM_UWB_ANGLE = 13,  //ghy 增加多功能声光显示类型
	GUI_TYPE_HUB =14 //ghy 增加HUB显示类型
};


typedef struct
{
	uint16_t fileCrc;
	uint16_t calcCrc;
	uint32_t fileSize;
	uint8_t cxb_recv_device;
	char fileName[FILENAMESIZE];
}
STR_CAN_RECV_PROGRAM;



//通讯协议中，版本信息的结构体定义。总共占用256个字节，64个字。低字节在前，高字节在后。
typedef struct
{
	uint32_t u32PrgDevType;//程序版本的设备类型
	uint32_t u32TargetBoardType;//目标板类型
	uint32_t u32PrgVer;//应用程序的版本号
	uint32_t u32PrgSize;//应用程序的大小
	uint32_t u32PrgDate;//应用程序的完成日期
	uint32_t u32PrgEncryptType;//应用程序的加密算法，0x00为无加密	
	uint32_t u32PrgWriteBaseaddr;//程序写入的基地址
	uint32_t u32PrgCrc16;//应用程序的CRC校验值
	uint32_t reserved[55];//预留信息，52个字
	uint32_t u32VerInfCrc16;//整个版本信息程序的CRC校验，为CRC16，只占用其中的两个字节
}PROGRAM_VERSION_TYPE;


typedef struct{
	uint16_t u16FrameType;//命令数据
	uint16_t u16FileType;
	uint32_t u32FileSize;//文件大小
	char  u8FileSuffixType[FILESUFFIXTYPE];//文件名houzhui
	char  u8FileName[FILENAMESIZE];//文件名
}UDP_UPDATE_READY;



//udp程序更新头信息，低字节在前，高字节在后。
typedef struct
{

	uint16_t u16FilePackNum;
	UDP_UPDATE_READY udpUpdateReady;
	uint32_t u32tmpFileSize;//文件大小
	uint16_t u16CRC;//整个文件的CRC
	uint16_t tmpCRC;//临时的CRC
	uint8_t crcCounts;
	uint8_t udpRecvFileAddress;
	uint8_t UdpRecvFileCounts;
	uint8_t recvCrcNumber[MAX_NUMBER];
}STR_UDP_UPDATE_FILE;


uint8_t GUIDeviceTypeToProDeviceType(uint8_t device);
uint8_t GUIDeviceTypeToProActDeviceType(uint8_t device);
uint8_t getProLoadStatus(void);
uint8_t getFileRecvStatus(void);
void setFileRecvStatus(uint8_t status);
uint32_t UpdateTimerApi(void);
void setUpdateTImerApi(uint32_t timer);
void setProLoadStatus(uint8_t status);
void CanWriteProgramQueueProc(uint8_t * buff,uint16_t size);
void UdpRecvFileReadyApi(uint8_t *data, uint16_t len,uint8_t address);
void UdpRecvFileDataApi(uint8_t nowpackNum, uint8_t *data, uint16_t len,uint8_t isFin);
void UdpRecvFileFinCrcApi(uint16_t recvCrc);
void UdpRecvFileUpdateStatusApi(uint16_t updateStatus);
void CanRequestProgramProc(uint16_t device);
void UdpRecvFileUpdateStatusProc(uint8_t deviceType, uint8_t fileSuffixType, char * fileNames);
void deleteMoreFile(uint8_t fileType);
void FileLoad_task(void);
#endif


