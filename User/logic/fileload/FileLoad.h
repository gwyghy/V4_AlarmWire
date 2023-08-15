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
	FILEREADY = 1,//׼������
	FILEDATA = 2,//���ڽ���
	FILECRCSUCCESS = 3,//У����ȷ
	FILECRCFAIL = 4,//У�����
	FILEREVSUCCESS = 5,//���ճɹ�
	FILEREVFAIL = 6,//���մ���
	FILESAVING = 7,//���ڱ��棨���ղ���ʹ�ã�
	FILESENDING = 8,//���ڷ���
	FILENORMALSTATUS = 0xFF,//����״̬
};


enum
{
	CXBUNCONNECT    = 0,//δ����
	CXBCONNECTED    = 1,// ����
	REQPROLOAD      = 2,//׼������
	PROLOADING      = 3,//��������
	NOPROTYPE       = 4,//û�и����ͳ���
	PROLOADBREAK    = 5,//�����ж�
	PROLOADCRCERROR = 6,//���մ���
	PROLOADSUCCESS  = 7,//���ճɹ�
	NORMALSTATUS = 0xFF,//����״̬
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
	LOAD_TYPE_ALARM_UWB_ANGLE = 0x0E,//ghy ���Ӷ๦�����ⱨ��������
	LOAD_TYPE_MINIHUB = 0x0F        //ghy ����MiniHUB�豸����
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
	GUI_TYPE_ALARM_UWB_ANGLE = 13,  //ghy ���Ӷ๦��������ʾ����
	GUI_TYPE_HUB =14 //ghy ����HUB��ʾ����
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



//ͨѶЭ���У��汾��Ϣ�Ľṹ�嶨�塣�ܹ�ռ��256���ֽڣ�64���֡����ֽ���ǰ�����ֽ��ں�
typedef struct
{
	uint32_t u32PrgDevType;//����汾���豸����
	uint32_t u32TargetBoardType;//Ŀ�������
	uint32_t u32PrgVer;//Ӧ�ó���İ汾��
	uint32_t u32PrgSize;//Ӧ�ó���Ĵ�С
	uint32_t u32PrgDate;//Ӧ�ó�����������
	uint32_t u32PrgEncryptType;//Ӧ�ó���ļ����㷨��0x00Ϊ�޼���	
	uint32_t u32PrgWriteBaseaddr;//����д��Ļ���ַ
	uint32_t u32PrgCrc16;//Ӧ�ó����CRCУ��ֵ
	uint32_t reserved[55];//Ԥ����Ϣ��52����
	uint32_t u32VerInfCrc16;//�����汾��Ϣ�����CRCУ�飬ΪCRC16��ֻռ�����е������ֽ�
}PROGRAM_VERSION_TYPE;


typedef struct{
	uint16_t u16FrameType;//��������
	uint16_t u16FileType;
	uint32_t u32FileSize;//�ļ���С
	char  u8FileSuffixType[FILESUFFIXTYPE];//�ļ���houzhui
	char  u8FileName[FILENAMESIZE];//�ļ���
}UDP_UPDATE_READY;



//udp�������ͷ��Ϣ�����ֽ���ǰ�����ֽ��ں�
typedef struct
{

	uint16_t u16FilePackNum;
	UDP_UPDATE_READY udpUpdateReady;
	uint32_t u32tmpFileSize;//�ļ���С
	uint16_t u16CRC;//�����ļ���CRC
	uint16_t tmpCRC;//��ʱ��CRC
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


