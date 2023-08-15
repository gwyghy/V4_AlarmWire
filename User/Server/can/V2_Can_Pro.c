/*
*********************************************************************************************************
*	                                            V4
*                           Commuication Controller For The Mining Industry
*
*                                  (c) Copyright 1994-2013  HNDZ 
*                       All rights reserved.  Protected by international copyright laws.
*
*
*    File    : Can_Bus.c
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
#include "Can_Pro.h"
#include "NativeCan_Driver.h"
#include "Can_Bus.h"
#include "V2_Can_Pro.h"
#include "Util_CRC.h"
#include "DoFatfsFiles.h"
#include "FileLoad.h"
#include "Log_Data_Type.h"
#include "Log_Data_Mng.h"

#include "Wireless_Alarm.h"

#define CXB_RECV_QUEUE_MAX 256


//��ŷ����������ܽӿ������
#define EMVD_OUTNUMB_MAX			32	
/**����������ת�������������¹��ܼ�ת��Ϊ��Ӧ��ŷ�����������ڵ������֡�2018.08.16����ŷ����������Ϊ26����ڡ�**/
/**��Ӧ�ĵ�ŷ�����������汾��ΪV2.6.X��**/
/**2019��9�·ݣ���ŷ����������Ϊ30��������ڡ�����汾��ҲΪV2.6.X��**/
/**ActCmdTransTbl_V26������26����~32����**/
static const int ActCmdTransTbl_V26[EMVD_OUTNUMB_MAX] = 
{
	22,23,20,21,
	18,19,16,17,
	27,26,24,25,
	9, 8, 7, 6,
	5, 4, 3, 2,
	1, 0,15,14,
	13,12,10,11,
	28,29,30,31
};

//����������ת�������������¹��ܼ�ת��Ϊ��Ӧ��ŷ�����������ڵ������֡�
//��Ӧ�ĵ�ŷ�����������汾��ΪV1.0.x
static const int ActCmdTransTbl_V22[EMVD_OUTNUMB_MAX] = 
{	
	16,17,14,15,
	12,13,10,11,
	21,20,18,19,
	9, 8, 7, 6,
	5, 4, 3, 2,
	1, 0,-1,-1,
	-1,-1,-1,-1,
	-1,-1,-1,-1
};

//can������ˮ��
uint8_t S_LiuShuiNumb = 0x00;

//��ŷ������ⱨ��������ִ��id
uint32_t emvdAction = 0;
uint8_t alarmAction = 0;

//��Ա��λ���Ƕȡ��߶����в���
stPersonParam v2_person_para;
stAngleParam  v2_angle_para;
stAngleParam  v2_high_para;

//��������ճ������鼰дָ��
static uint8_t  u8ScRecProgramQueue[CXB_RECV_QUEUE_MAX];
static uint16_t u16CxbRecvQueuepWrite = 0;


uint8_t downloadDevice;

/*******************************************************************************************
*���������������-����������
*��ڲ�����pdataȡֵ��
*             can֡��Դ��can1��can2��
*          frameȡֵ��
*             can֡
*����ֵ��  ��
*******************************************************************************************/
uint16_t CXB_Heart_Proc(void *pdata , STR_Can_Frame* frame)
{
	stDeviceStatusMsg deviceMsg = {0};
	
	GetCanDeviceStatusMsg((uint16_t)pdata,ID_CXB,CXB_Version, &deviceMsg);	
	
	deviceMsg.device = ID_CXB;
	deviceMsg.subDevice = CXB_Version;
	deviceMsg.HeartTimer = V2_CXB_HEARTTIMER;
	deviceMsg.isOnline = DEVICEONLINE;
//	deviceMsg.isEnable = DEVICEENABLE;
	deviceMsg.backTimer = deviceMsg.HeartTimer;
	deviceMsg.deviceVersion[0] = frame->Data[5];
	deviceMsg.deviceVersion[1] = frame->Data[6];
	deviceMsg.deviceVersion[2] = frame->Data[7];

	SetCanDeviceStatusMsg((uint16_t)pdata,deviceMsg.device,deviceMsg.subDevice, &deviceMsg);	
	return 1;
}

/*******************************************************************************************
*���������������-���ճ�������
*��ڲ�����pdataȡֵ��
*             can֡��Դ��can1��can2��
*          frameȡֵ��
*             can֡
*����ֵ��  ��
*******************************************************************************************/
uint16_t CXB_Program_Proc(void *pdata , STR_Can_Frame* frame)
{
	STR_V2CXBCAN_Frame cxbframe;
	memcpy(&cxbframe,frame,sizeof(STR_Can_Frame));
	if(cxbframe.u32ID.ID.PacketNum == 1)
	{
		u16CxbRecvQueuepWrite = 0;
		memset(u8ScRecProgramQueue,0,sizeof(u8ScRecProgramQueue));
	}
	if(u16CxbRecvQueuepWrite+frame->DLC > 256)
		return 0;
	memcpy(u8ScRecProgramQueue+u16CxbRecvQueuepWrite,frame->Data,frame->DLC);
	u16CxbRecvQueuepWrite+=frame->DLC;
	return 1;
}

/*******************************************************************************************
*���������������-���ճ���crc������
*��ڲ�����pdataȡֵ��
*             can֡��Դ��can1��can2��
*          frameȡֵ��
*             can֡
*����ֵ��  ��
*******************************************************************************************/
uint16_t CXB_Program_Crc_Proc(void *pdata , STR_Can_Frame* frame)
{
	uint32_t u32StartPacketNumb = 0;
	uint16_t u16Crc = 0, u16CrcTemp = 0, u16crcpacket;
	STR_V2CXBCAN_Frame cxbframe;
	stCanTrsQueue canTrsQueue;
	CANPROHEAD prohead;
	uint8_t frametype;
	uint8_t isRight = 0;
	
	memcpy(&cxbframe,frame,sizeof(STR_Can_Frame));
	
	frametype = cxbframe.u32ID.ID.FrameType;
	u32StartPacketNumb = (u32)(frame->Data[0x00]|(frame->Data[0x01]<<8));
	u32StartPacketNumb |= (u32)((frame->Data[0x02]<<16)|(frame->Data[0x03]<<24));	
	u16crcpacket = (u32)(frame->Data[0x04]|(frame->Data[0x05]<<8));
	u16Crc = (u32)(frame->Data[0x06]|(frame->Data[0x07]<<8));

	Crc16Ccitt(u8ScRecProgramQueue, u16CxbRecvQueuepWrite, &u16CrcTemp);//�����ʱ��У��ֵ

	if(u16CrcTemp == u16Crc)
	{
		CanWriteProgramQueueProc(u8ScRecProgramQueue, u16CxbRecvQueuepWrite);
		memset(u8ScRecProgramQueue,0,CXB_RECV_QUEUE_MAX);
		//������		
		u16CxbRecvQueuepWrite = 0;
		//��Ӧ��
		isRight = 1;
		cxbframe.u8DT[6] = (u16CrcTemp & 0xFF);
		cxbframe.u8DT[7] = (u16CrcTemp & 0xFF00)>>8;
	}
	else
	{
		//�建��
		memset(u8ScRecProgramQueue,0,CXB_RECV_QUEUE_MAX);
		u16CxbRecvQueuepWrite = 0;
		//��crc
		isRight = 0;
		cxbframe.u8DT[6] = (u16CrcTemp & 0xFF);
		cxbframe.u8DT[7] = (u16CrcTemp & 0xFF00)>>8;
	}
	
	//��Ӧ��
	cxbframe.u32ID.ID.RxID = 4;
	cxbframe.u32ID.ID.ACK = 0;
	memcpy(&canTrsQueue.TrsFrame, &cxbframe, sizeof(STR_Can_Frame));
	getV2ProDeviceMsg(V2ID_CXB,frametype,&prohead);
	canTrsQueue.busHead.ack = prohead.ack;
	canTrsQueue.busHead.backtime = prohead.overtime;
	canTrsQueue.busHead.count = prohead.count;
	canTrsQueue.busHead.prio = 1;
	canTrsQueue.busHead.overtime = prohead.overtime;
	CanBusDealFunc((uint16_t)pdata, CANBUS_MSG_InsCanHighTrsQueue, &canTrsQueue);
	if(isRight)
	{
		if(cxbframe.u32ID.ID.FrameType == CXB_SEND_DEITION_CRC)
			OSMboxPost(revProgramSem,(void*)CANLOADHEADSEM);
		else if(cxbframe.u32ID.ID.FrameType == CXB_SEND_PROGRAM_CRC)
			OSMboxPost(revProgramSem,(void*)CANLOADPROGRAMSEM);
	}
	return 1;
}
/*******************************************************************************************
*���������������-���ճ���Ӧ����
*��ڲ�����pdataȡֵ��
*             can֡��Դ��can1��can2��
*          frameȡֵ��
*             can֡
*����ֵ��  ��
*******************************************************************************************/
uint16_t CXB_Send_ACK_Proc(void *pdata , STR_Can_Frame* frame)
{
	if(frame->Data[0] == downloadDevice)
	{
		setProLoadStatus(REQPROLOAD);
		return 1;
	}
	else if(frame->Data[0] == 0XFF)
	{
		setProLoadStatus(NOPROTYPE);
	}
	return 0;
}

uint16_t CXB_Send_NOACK_Proc(void *pdata , STR_Can_Frame* frame)
{
//	setProLoadStatus(CXBUNCONNECT);
	return 1;
}


/*******************************************************************************************
*������������ŷ�-��������
*��ڲ�����pdataȡֵ��
*             can֡��Դ��can1��can2��
*          frameȡֵ��
*             can֡
*����ֵ��  ��
*******************************************************************************************/
uint16_t Emvd_Heart_Proc(void *pdata , STR_Can_Frame* frame)
{
	stDeviceStatusMsg deviceMsg = {0};
	GetCanDeviceStatusMsg((uint16_t)pdata,ID_EMVD,EMVD_Version, &deviceMsg);	
	
	deviceMsg.device = ID_EMVD;
	deviceMsg.subDevice = EMVD_Version;
	deviceMsg.HeartTimer = V2_EMVD_HEARTTIMER;
	deviceMsg.isOnline = DEVICEONLINE;
	deviceMsg.isEnable = DEVICEENABLE;
	deviceMsg.backTimer = deviceMsg.HeartTimer;
	deviceMsg.deviceVersion[0] = frame->Data[5];
	deviceMsg.deviceVersion[1] = frame->Data[6];
	deviceMsg.deviceVersion[2] = frame->Data[7];	

	SetCanDeviceStatusMsg((uint16_t)pdata, deviceMsg.device, deviceMsg.subDevice, &deviceMsg);	
	return 1;
}

/*******************************************************************************************
*������������ŷ�-�����
*��ڲ�����pdataȡֵ��
*             can֡��Դ��can1��can2��
*          frameȡֵ��
*             can֡
*����ֵ��  ��
*******************************************************************************************/
uint16_t Emvd_Check_Proc(void *pdata, STR_Can_Frame* frame)
{
	uint8_t res = 1;
	uint32_t emvdtmp1, emvdtmp;
	//���ŷ������
	emvdtmp =  ((uint32_t)frame->Data[0]);
	emvdtmp |= ((uint32_t)frame->Data[1])<<8;	
	emvdtmp |= ((uint32_t)frame->Data[2])<<16;
	emvdtmp |= ((uint32_t)frame->Data[3])<<24;
	
	GetCanDeviceAPPMsg((uint16_t)pdata,ID_EMVD, 0 ,EMVDIO, &emvdtmp1);
	
    //if(emvdtmp1 != emvdtmp)
   // return 0;
	SetCanDeviceAPPMsg((uint16_t)pdata, ID_EMVD, 0 ,EMVDFLAG, 1);
	SetCanDeviceAPPMsg((uint16_t)pdata, ID_EMVD, 0 ,EMVDSTATUS, frame->Data[4]);	
	return res;
}
extern V4YKQ_DATA V4ykq;
/*******************************************************************************************
*�����������������-����
*��ڲ�����pdataȡֵ��
*             can֡��Դ��can1��can2��
*          frameȡֵ��
*             can֡
*����ֵ��  ��
*******************************************************************************************/
uint16_t Ir_Heart_Proc(void *pdata , STR_Can_Frame* frame)
{
	stDeviceStatusMsg deviceMsg = {0};
	
	if(frame->Data[0] != 'W')
	{
		//��frameָ��ת��ΪSTR_WLCAN_Frame*���ͣ�����һ��
		STR_WLCAN_Frame wl_frame;
		wl_frame.u32ID.u32Id = frame->ID;
		wl_frame.u16DLC = frame->DLC;
		memcpy(wl_frame.u8DT, frame->Data, sizeof(frame->Data));  
		InsAlarmWirelessIrRecvData_proc(&wl_frame,sizeof(wl_frame));	
	}		
	if(frame->Data[3] == '1')
	{		
	   //��frameָ��ת��ΪSTR_WLCAN_Frame*���ͣ�����һ��
		STR_WLCAN_Frame wl_frame;
		wl_frame.u32ID.u32Id = frame->ID;
		wl_frame.u16DLC = frame->DLC;
		memcpy(wl_frame.u8DT, frame->Data, sizeof(frame->Data));  
		InsAlarmWirelessIrRecvData_proc(&wl_frame,sizeof(wl_frame));	
	}
	if(frame->Data[3] == '0')
	{
		GetCanDeviceStatusMsg((uint16_t)pdata,ID_IRRX,IR_Verson, &deviceMsg);		
		deviceMsg.device = ID_IRRX;
		deviceMsg.subDevice = IR_Verson;
		deviceMsg.HeartTimer = V2_IR_HEARTTIMER;
		deviceMsg.isOnline = DEVICEONLINE;
	//	deviceMsg.isEnable = DEVICEENABLE;
		deviceMsg.backTimer = deviceMsg.HeartTimer;
		deviceMsg.deviceVersion[0] = frame->Data[5];
		deviceMsg.deviceVersion[1] = frame->Data[6];
		deviceMsg.deviceVersion[2] = frame->Data[7];	
		SetCanDeviceStatusMsg((uint16_t)pdata, deviceMsg.device, deviceMsg.subDevice, &deviceMsg);	
		return 1;
	}

}

/*******************************************************************************************
*�����������������-���մ���
*��ڲ�����pdataȡֵ��
*             can֡��Դ��can1��can2��
*          frameȡֵ��
*             can֡
*����ֵ��  ��
*******************************************************************************************/
uint16_t Ir_Rx_Proc(void *pdata , STR_Can_Frame* frame)
{	
	uint8_t res = 1;
	uint32_t data;
	stDeviceStatusMsg deviceMsg = {0};
	
	//��ֵ
	GetCanDeviceAPPMsg((uint16_t)pdata, ID_IRRX,0,0,&data);
	data++;
	data%=0xffff;
	SetCanDeviceAPPMsg((uint16_t)pdata, ID_IRRX,0,0,data);

	//����������
	GetCanDeviceStatusMsg((uint16_t)pdata,ID_IRRX, IR_Verson, &deviceMsg);
	deviceMsg.HeartTimer = V2_IR_HEARTTIMER;
	SetCanDeviceStatusMsg((uint16_t)pdata,ID_IRRX, IR_Verson, &deviceMsg);	

	return res;	
}



/*******************************************************************************************
*�����������Ƕȡ��߶�-����
*��ڲ�����pdataȡֵ��
*             can֡��Դ��can1��can2��
*          frameȡֵ��
*             can֡
*����ֵ��  ��
*******************************************************************************************/
uint16_t Angle_Heart_Proc(void *pdata , STR_Can_Frame* frame)
{
	uint8_t res = 1;
	uint8_t device;
	stDeviceStatusMsg deviceMsg = {0};
		
	device = frame->Data[3];
	
	if(device < V2_HIGH_HEART)//�Ƕ�����
	{
		if(device == 0x3a) //�๦�����ⱨ�����ϵ����(����ʹ��-0x3a)
		{
			GetCanDeviceStatusMsg((uint16_t)pdata, ID_ANGLE_ALARM,ALARMANGLE_Version,&deviceMsg);	
			deviceMsg.device = ID_ANGLE_ALARM;
			deviceMsg.subDevice = ALARMANGLE_Version;
			deviceMsg.HeartTimer = V2_ANGLE_HEARTTIMER;
			deviceMsg.isOnline = DEVICEONLINE;
			deviceMsg.backTimer = deviceMsg.HeartTimer;
		}
		else //��ͨ���
		{
			device = device - V2_ANGLE_HEART;
			GetCanDeviceStatusMsg((uint16_t)pdata, ID_ANGLE, ANGLE0_Version + device, &deviceMsg);	
			deviceMsg.device = ID_ANGLE;
			deviceMsg.subDevice = ANGLE0_Version+device;
			deviceMsg.HeartTimer = V2_ANGLE_HEARTTIMER;
			deviceMsg.isOnline = DEVICEONLINE;
			deviceMsg.backTimer = deviceMsg.HeartTimer;
		}
		//�豸��ʼ��
		if(frame->Data[4] == DEVICEINIT)
		{
			CanProSendParam((uint16_t)pdata, ID_ANGLE,0);
		}
		deviceMsg.deviceVersion[0] = frame->Data[5];
		deviceMsg.deviceVersion[1] = frame->Data[6];
		deviceMsg.deviceVersion[2] = frame->Data[7];	
	}
	else if(device ==0x4a)//�Ǹ�һ���еĵ����Ƕ�
	{
		GetCanDeviceStatusMsg((uint16_t)pdata, ID_ANGLE_HIGH, 1, &deviceMsg);	
		deviceMsg.device = ID_ANGLE_HIGH;
		deviceMsg.subDevice = 1;
		deviceMsg.HeartTimer = V2_ANGLE_HEARTTIMER;
		deviceMsg.isOnline = DEVICEONLINE;
		deviceMsg.backTimer = deviceMsg.HeartTimer;	
		deviceMsg.deviceVersion[0] = frame->Data[5];
		deviceMsg.deviceVersion[1] = frame->Data[6];
		deviceMsg.deviceVersion[2] = frame->Data[7];	
	}
	else if(device ==0x4b)//�Ǹ�һ��Ķ����Ƕ�
	{
		GetCanDeviceStatusMsg((uint16_t)pdata, ID_ANGLE_HIGH, 2, &deviceMsg);	
		deviceMsg.device = ID_ANGLE_HIGH;
		deviceMsg.subDevice = 2;
		deviceMsg.HeartTimer = V2_ANGLE_HEARTTIMER;
		deviceMsg.isOnline = DEVICEONLINE;
		deviceMsg.backTimer = deviceMsg.HeartTimer;
		deviceMsg.deviceVersion[0] = frame->Data[5];
		deviceMsg.deviceVersion[1] = frame->Data[6];
		deviceMsg.deviceVersion[2] = frame->Data[7];	
	}
	else if(device  == V2_HIGH_HEART)//�߶�����
	{
		GetCanDeviceStatusMsg((uint16_t)pdata, ID_HIGH, HIGH_Verson, &deviceMsg);	
		deviceMsg.device = ID_HIGH;
		deviceMsg.subDevice = HIGH_Verson;
		deviceMsg.isOnline = DEVICEONLINE;
		deviceMsg.HeartTimer = V2_HIGH_HEARTTIMER;
		deviceMsg.backTimer = deviceMsg.HeartTimer;
		//�豸��ʼ��
		if(frame->Data[4] == DEVICEINIT)
		{
			CanProSendParam((uint16_t)pdata, ID_HIGH,0);
		}
		else if(frame->Data[4] == DEVICEPROGRAM)
		{
			deviceMsg.deviceStatus = DEVICEPROGRAM;
		}
		else
			deviceMsg.deviceStatus = DEVICENORMAL;
		
		deviceMsg.deviceVersion[0] = frame->Data[5];
		deviceMsg.deviceVersion[1] = frame->Data[6];
		deviceMsg.deviceVersion[2] = frame->Data[7];	
	}
	else if(device  == V2_HIGHBL_HEART)
	{
		GetCanDeviceStatusMsg((uint16_t)pdata, ID_HIGH, HIGH_Verson, &deviceMsg);
		deviceMsg.device = ID_HIGH;
		deviceMsg.subDevice = HIGH_Verson;
//		deviceMsg.isOnline = DEVICEONLINE;
		deviceMsg.HeartTimer = V2_HIGH_HEARTTIMER;
		deviceMsg.backTimer = deviceMsg.HeartTimer;
		deviceMsg.deviceBLVersion[0] = frame->Data[5];
		deviceMsg.deviceBLVersion[1] = frame->Data[6];
		deviceMsg.deviceBLVersion[2] = frame->Data[7];
	}
//	deviceMsg.isEnable = DEVICEENABLE;
	SetCanDeviceStatusMsg((uint16_t)pdata, deviceMsg.device, deviceMsg.subDevice, &deviceMsg);	
	return res;
}


/*******************************************************************************************
*�����������Ƕȡ�180�л�
*��ڲ�����AngleValueȡֵ��
*             �Ƕ�ֵ
*����ֵ��  �Ƕ�ֵ
*******************************************************************************************/
s16 GetAdjustAngleValue(u16 AngleValue)
{
	if ((AngleValue > 9000) && (AngleValue <= 18000))
	{
		AngleValue = 18000 - AngleValue;
	}
	else if ((AngleValue >= 0xB9B0) && (AngleValue < 0xDCD8))
	{
		AngleValue = ~(18000 + AngleValue) - 1;
	}
	
	return (s16)AngleValue;
}

/*******************************************************************************************
*�����������Ƕȡ��߶�-�ϱ�����
*��ڲ�����pdataȡֵ��
*             can֡��Դ��can1��can2��
*          frameȡֵ��
*             can֡
*����ֵ��  ��
*******************************************************************************************/
uint16_t Angle_Report_Proc(void *pdata , STR_Can_Frame* frame)
{
	uint8_t res = 1;
	uint16_t data;
	stDeviceStatusMsg deviceMsg = {0};
	
	if(frame->Data[1] == 0 || frame->Data[1] == 2 || frame->Data[1] ==3 || frame->Data[1] ==4 ) //0:��ͨ��� 2:��̬��� 3:������� 4:�Ǹ�һ��
	{	
		if(frame->Data[1] ==3)  //�������
		{
			//��Ƕ�xyֵ
			data = ((uint16_t)frame->Data[2])|((uint16_t)frame->Data[3]<<8);
			data = GetAdjustAngleValue(data);
			SetCanDeviceAPPMsg((uint16_t)pdata, ID_ANGLE_ALARM, frame->Data[0], ANGLE_X, data);
			data = ((uint16_t)frame->Data[4])|((uint16_t)frame->Data[5]<<8);
			SetCanDeviceAPPMsg((uint16_t)pdata, ID_ANGLE_ALARM, frame->Data[0], ANGLE_Y, data);
			if(frame->DLC == 0x08)
			{		
				data = ((uint16_t)frame->Data[6])|((uint16_t)frame->Data[7]<<8);
				SetCanDeviceAPPMsg((uint16_t)pdata, ID_ANGLE_ALARM, frame->Data[0], ANGLE_Z, data);
			}		
			//����Ƕ��������
			GetCanDeviceStatusMsg((uint16_t)pdata,ID_ANGLE_ALARM, ALARMANGLE_Version, &deviceMsg);
			deviceMsg.HeartTimer = V2_ANGLE_HEARTTIMER;
			SetCanDeviceStatusMsg((uint16_t)pdata,ID_ANGLE_ALARM, deviceMsg.subDevice, &deviceMsg);
		}
		else if(frame->Data[1] ==4) //�Ǹ�һ��
		{			
			//��Ƕ�xyֵ
			data = ((uint16_t)frame->Data[2])|((uint16_t)frame->Data[3]<<8);
			data = GetAdjustAngleValue(data);
			SetCanDeviceAPPMsg((uint16_t)pdata, ID_ANGLE_HIGH, frame->Data[0],ANGLE_X, data);
			data = ((uint16_t)frame->Data[4])|((uint16_t)frame->Data[5]<<8);
			SetCanDeviceAPPMsg((uint16_t)pdata, ID_ANGLE_HIGH, frame->Data[0],ANGLE_Y, data);
			if(frame->DLC == 0x08)
			{		
				data = ((uint16_t)frame->Data[6])|((uint16_t)frame->Data[7]<<8);
				SetCanDeviceAPPMsg((uint16_t)pdata, ID_ANGLE_HIGH, frame->Data[0],ANGLE_Z, data);
			}
			GetCanDeviceStatusMsg((uint16_t)pdata,ID_ANGLE_HIGH,frame->Data[0]+1, &deviceMsg);
			deviceMsg.HeartTimer = V2_ANGLE_HEARTTIMER;
			SetCanDeviceStatusMsg((uint16_t)pdata,ID_ANGLE_HIGH, deviceMsg.subDevice, &deviceMsg);
			
		}
		else  //��ͨ���
		{
						//��Ƕ�xyֵ
			data = ((uint16_t)frame->Data[2])|((uint16_t)frame->Data[3]<<8);
			data = GetAdjustAngleValue(data);
			SetCanDeviceAPPMsg((uint16_t)pdata, ID_ANGLE, frame->Data[0], ANGLE_X, data);
			data = ((uint16_t)frame->Data[4])|((uint16_t)frame->Data[5]<<8);
			SetCanDeviceAPPMsg((uint16_t)pdata, ID_ANGLE, frame->Data[0], ANGLE_Y, data);
			if(frame->DLC == 0x08)
			{		
				data = ((uint16_t)frame->Data[6])|((uint16_t)frame->Data[7]<<8);
				SetCanDeviceAPPMsg((uint16_t)pdata, ID_ANGLE, frame->Data[0], ANGLE_Z, data);
			}		
			//����Ƕ��������
			GetCanDeviceStatusMsg((uint16_t)pdata,ID_ANGLE, frame->Data[0]+1, &deviceMsg);
			deviceMsg.HeartTimer = V2_ANGLE_HEARTTIMER;
			SetCanDeviceStatusMsg((uint16_t)pdata,ID_ANGLE, deviceMsg.subDevice, &deviceMsg);
		}
	}
	else
	{
		//��߶�
		data = frame->Data[3];
		data = (data<<8)|(uint16_t)frame->Data[2];
		SetCanDeviceAPPMsg((uint16_t)pdata, ID_HIGH, frame->Data[0], 0, data);
		
		//����߶��������
		GetCanDeviceStatusMsg((uint16_t)pdata, ID_HIGH, HIGH_Verson,&deviceMsg);
		deviceMsg.HeartTimer = V2_HIGH_HEARTTIMER;
		SetCanDeviceStatusMsg((uint16_t)pdata, ID_HIGH, HIGH_Verson, &deviceMsg);	
	}
	return res;
}


/*******************************************************************************************
*����������HUB-��������
*��ڲ�����pdataȡֵ��
*             can֡��Դ��can1��can2��
*          frameȡֵ��
*             can֡
*����ֵ��  ��
*******************************************************************************************/
uint16_t Hub_Heart_Proc(void *pdata , STR_Can_Frame* frame)
{
	uint8_t deviceType;
	uint8_t device;
	uint8_t subdevice;
	stDeviceStatusMsg deviceMsg = {0};
	
    deviceType =frame->Data[0];
	if(deviceType == 'H')
	{
		device = frame->Data[3];
		GetCanDeviceStatusMsg((uint16_t)pdata,ID_HUB1 + device ,0 , &deviceMsg);
		deviceMsg.device = ID_HUB1 + device;
		deviceMsg.subDevice = 0 ;
		deviceMsg.HeartTimer = V2_HUB_HEARTTIMER;
		deviceMsg.isOnline = DEVICEONLINE;
		deviceMsg.backTimer = deviceMsg.HeartTimer;
		deviceMsg.deviceVersion[0] = frame->Data[5];
		deviceMsg.deviceVersion[1] = frame->Data[6];
		deviceMsg.deviceVersion[2] = frame->Data[7];	
		SetCanDeviceStatusMsg((uint16_t)pdata, deviceMsg.device,deviceMsg.subDevice, &deviceMsg);			
	}
	else 
	{
		device = frame->Data[3];
		subdevice = frame->Data[2] - V2_ALARM_HEART;
		switch(device)
		{
			case HUB1:
				GetCanDeviceStatusMsg((uint16_t)pdata,ID_HUB1,subdevice + 1,&deviceMsg);
				deviceMsg.device = ID_HUB1;
				deviceMsg.subDevice = subdevice + 1;
				deviceMsg.HeartTimer = V2_ANGLE_HEARTTIMER;
				deviceMsg.isOnline = DEVICEONLINE;
				deviceMsg.backTimer = deviceMsg.HeartTimer;
				break;	
			case HUB2:
				GetCanDeviceStatusMsg((uint16_t)pdata,ID_HUB2, subdevice +1,&deviceMsg);
				deviceMsg.device = ID_HUB2;
				deviceMsg.subDevice = subdevice + 1;
				deviceMsg.HeartTimer = V2_ANGLE_HEARTTIMER;
				deviceMsg.isOnline = DEVICEONLINE;
				deviceMsg.backTimer = deviceMsg.HeartTimer;			
				break;				
			case HUB3:
				GetCanDeviceStatusMsg((uint16_t)pdata,ID_HUB3,subdevice +1,&deviceMsg);
				deviceMsg.device = ID_HUB3;
				deviceMsg.subDevice = subdevice + 1;
				deviceMsg.HeartTimer = V2_ANGLE_HEARTTIMER;
				deviceMsg.isOnline = DEVICEONLINE;
				deviceMsg.backTimer = deviceMsg.HeartTimer;
				break;
			default:
				break;
		}
		deviceMsg.deviceVersion[0] = frame->Data[5];
		deviceMsg.deviceVersion[1] = frame->Data[6];
		deviceMsg.deviceVersion[2] = frame->Data[7];
		SetCanDeviceStatusMsg((uint16_t)pdata, deviceMsg.device, deviceMsg.subDevice, &deviceMsg);			
	}

	return 1;
}

/*******************************************************************************************
*����������ģ������180�л�
*��ڲ�����AngleValueȡֵ��
*             �Ƕ�ֵ
*����ֵ��  �Ƕ�ֵ
*******************************************************************************************/
u16 GetAdjustAnalogValue(u16 AnalogValue)
{
	AnalogValue = AnalogValue * 1000/819	;// x * 2 * 2500 / 4095	//ʵ�ʵ�ѹ�����룬�ο���ѹ2.5V	
	return (u16)AnalogValue;
}


/*******************************************************************************************
*����������������һ��HUBģ�����ϱ����ݴ���
*��ڲ�����pdataȡֵ��
*             can֡��Դ��can1��can2��
*          frameȡֵ��
*             can֡
*����ֵ��  ��
*******************************************************************************************/
uint16_t FirstHub_Analog_Proc(void *pdata , STR_Can_Frame* frame)
{
	uint16_t data;
	stDeviceStatusMsg deviceMsg = {0};
	
	data = ((uint16_t)frame->Data[0])|((uint16_t)frame->Data[1]<<8);
	data = GetAdjustAnalogValue(data);
	SetCanDeviceAPPMsg((uint16_t)pdata, ID_HUB1, 0, ANALOG_1, data);
	data = ((uint16_t)frame->Data[2])|((uint16_t)frame->Data[3]<<8);
	data = GetAdjustAnalogValue(data);
	SetCanDeviceAPPMsg((uint16_t)pdata, ID_HUB1, 0, ANALOG_2, data);
	data = ((uint16_t)frame->Data[4])|((uint16_t)frame->Data[5]<<8);
	data = GetAdjustAnalogValue(data);
	SetCanDeviceAPPMsg((uint16_t)pdata, ID_HUB1, 0, ANALOG_3, data);
	data = ((uint16_t)frame->Data[6])|((uint16_t)frame->Data[7]<<8);
	data = GetAdjustAnalogValue(data);
	SetCanDeviceAPPMsg((uint16_t)pdata, ID_HUB1, 0, ANALOG_4, data);	
}
/*******************************************************************************************
*���������������ڶ���HUBģ�����ϱ����ݴ���
*��ڲ�����pdataȡֵ��
*             can֡��Դ��can1��can2��
*          frameȡֵ��
*             can֡
*����ֵ��  ��
*******************************************************************************************/
uint16_t SecondHub_Analog_Proc(void *pdata , STR_Can_Frame* frame)
{
	uint16_t data;
	stDeviceStatusMsg deviceMsg = {0};
	
	data = ((uint16_t)frame->Data[0])|((uint16_t)frame->Data[1]<<8);
	data = GetAdjustAnalogValue(data);
	SetCanDeviceAPPMsg((uint16_t)pdata, ID_HUB2, 0, ANALOG_1, data);
	data = ((uint16_t)frame->Data[2])|((uint16_t)frame->Data[3]<<8);
	data = GetAdjustAnalogValue(data);
	SetCanDeviceAPPMsg((uint16_t)pdata, ID_HUB2, 0, ANALOG_2, data);
	data = ((uint16_t)frame->Data[4])|((uint16_t)frame->Data[5]<<8);
	data = GetAdjustAnalogValue(data);
	SetCanDeviceAPPMsg((uint16_t)pdata, ID_HUB2, 0, ANALOG_3, data);
	data = ((uint16_t)frame->Data[6])|((uint16_t)frame->Data[7]<<8);
	data = GetAdjustAnalogValue(data);
	SetCanDeviceAPPMsg((uint16_t)pdata, ID_HUB2, 0, ANALOG_4, data);	
}

/*******************************************************************************************
*��������������������HUBģ�����ϱ����ݴ���
*��ڲ�����pdataȡֵ��
*             can֡��Դ��can1��can2��
*          frameȡֵ��
*             can֡
*����ֵ��  ��
*******************************************************************************************/
uint16_t ThirdHub_Analog_Proc(void *pdata , STR_Can_Frame* frame)
{
	uint16_t data;
	stDeviceStatusMsg deviceMsg = {0};
	
	data = ((uint16_t)frame->Data[0])|((uint16_t)frame->Data[1]<<8);
	data = GetAdjustAnalogValue(data);
	SetCanDeviceAPPMsg((uint16_t)pdata, ID_HUB3, 0, ANALOG_1, data);
	data = ((uint16_t)frame->Data[2])|((uint16_t)frame->Data[3]<<8);
	data = GetAdjustAnalogValue(data);
	SetCanDeviceAPPMsg((uint16_t)pdata, ID_HUB3, 0, ANALOG_2, data);
	data = ((uint16_t)frame->Data[4])|((uint16_t)frame->Data[5]<<8);
	data = GetAdjustAnalogValue(data);
	SetCanDeviceAPPMsg((uint16_t)pdata, ID_HUB3, 0, ANALOG_3, data);
	data = ((uint16_t)frame->Data[6])|((uint16_t)frame->Data[7]<<8);
	data = GetAdjustAnalogValue(data);
	SetCanDeviceAPPMsg((uint16_t)pdata, ID_HUB3, 0, ANALOG_4, data);
	
}
/*******************************************************************************************
*����������������һ��HUB�ϵĽǶ�X,Y��Ƕ��ϱ����ݴ���
*��ڲ�����pdataȡֵ��
*             can֡��Դ��can1��can2��
*          frameȡֵ��
*             can֡
*����ֵ��  ��
*******************************************************************************************/
uint16_t FirstHub_AngleXY_Proc(void *pdata , STR_Can_Frame* frame)
{
	uint16_t data;
	STR_V2CAN_Frame v2frame;
	stDeviceStatusMsg deviceMsg = {0};
	memcpy(&v2frame, frame, sizeof(STR_V2CAN_Frame));
	if(v2frame.u32ID.ID.Sub ==0)
	{
		//��Ƕ�xyֵ
		data = ((uint16_t)frame->Data[0])|((uint16_t)frame->Data[1]<<8);
		data = GetAdjustAngleValue(data);
		SetCanDeviceAPPMsg((uint16_t)pdata, ID_HUB1, 1,ANGLE_X, data);
		data = ((uint16_t)frame->Data[2])|((uint16_t)frame->Data[3]<<8);
		data = GetAdjustAngleValue(data);
		SetCanDeviceAPPMsg((uint16_t)pdata, ID_HUB1, 1,ANGLE_Y, data);
		data = ((uint16_t)frame->Data[4])|((uint16_t)frame->Data[5]<<8);
		data = GetAdjustAngleValue(data);
		SetCanDeviceAPPMsg((uint16_t)pdata, ID_HUB1, 2,ANGLE_X, data);
		data = ((uint16_t)frame->Data[6])|((uint16_t)frame->Data[7]<<8);
		data = GetAdjustAngleValue(data);
		SetCanDeviceAPPMsg((uint16_t)pdata, ID_HUB1, 2,ANGLE_Y, data);
	}
	if(v2frame.u32ID.ID.Sub ==1)
	{
		//��Ƕ�xyֵ
		data = ((uint16_t)frame->Data[0])|((uint16_t)frame->Data[1]<<8);
		data = GetAdjustAngleValue(data);
		SetCanDeviceAPPMsg((uint16_t)pdata, ID_HUB1, 3,ANGLE_X, data);
		data = ((uint16_t)frame->Data[2])|((uint16_t)frame->Data[3]<<8);
		data = GetAdjustAngleValue(data);
		SetCanDeviceAPPMsg((uint16_t)pdata, ID_HUB1, 3,ANGLE_Y, data);
		data = ((uint16_t)frame->Data[4])|((uint16_t)frame->Data[5]<<8);
		data = GetAdjustAngleValue(data);
		SetCanDeviceAPPMsg((uint16_t)pdata, ID_HUB1, 4,ANGLE_X, data);
		data = ((uint16_t)frame->Data[6])|((uint16_t)frame->Data[7]<<8);
		data = GetAdjustAngleValue(data);
		SetCanDeviceAPPMsg((uint16_t)pdata, ID_HUB1, 4,ANGLE_Y, data);
	}
}
/*******************************************************************************************
*���������������ڶ���HUB�ϵĽǶ�X,Y��Ƕ��ϱ����ݴ���
*��ڲ�����pdataȡֵ��
*             can֡��Դ��can1��can2��
*          frameȡֵ��
*             can֡
*����ֵ��  ��
*******************************************************************************************/
uint16_t SecondHub_AngleXY_Proc(void *pdata , STR_Can_Frame* frame)
{
	uint16_t data;
	STR_V2CAN_Frame v2frame;
	stDeviceStatusMsg deviceMsg = {0};
	memcpy(&v2frame, frame, sizeof(STR_V2CAN_Frame));
	if(v2frame.u32ID.ID.Sub ==0)
	{
		//��Ƕ�xyֵ
		data = ((uint16_t)frame->Data[0])|((uint16_t)frame->Data[1]<<8);
		data = GetAdjustAngleValue(data);
		SetCanDeviceAPPMsg((uint16_t)pdata, ID_HUB2, 1,ANGLE_X, data);
		data = ((uint16_t)frame->Data[2])|((uint16_t)frame->Data[3]<<8);
		data = GetAdjustAngleValue(data);
		SetCanDeviceAPPMsg((uint16_t)pdata, ID_HUB2, 1,ANGLE_Y, data);
		data = ((uint16_t)frame->Data[4])|((uint16_t)frame->Data[5]<<8);
		data = GetAdjustAngleValue(data);
		SetCanDeviceAPPMsg((uint16_t)pdata, ID_HUB2, 2,ANGLE_X, data);
		data = ((uint16_t)frame->Data[6])|((uint16_t)frame->Data[7]<<8);
		data = GetAdjustAngleValue(data);
		SetCanDeviceAPPMsg((uint16_t)pdata, ID_HUB2, 2,ANGLE_Y, data);
	}
	if(v2frame.u32ID.ID.Sub ==1)
	{
		//��Ƕ�xyֵ
		data = ((uint16_t)frame->Data[0])|((uint16_t)frame->Data[1]<<8);
		data = GetAdjustAngleValue(data);
		SetCanDeviceAPPMsg((uint16_t)pdata, ID_HUB2, 3,ANGLE_X, data);
		data = ((uint16_t)frame->Data[2])|((uint16_t)frame->Data[3]<<8);
		data = GetAdjustAngleValue(data);
		SetCanDeviceAPPMsg((uint16_t)pdata, ID_HUB2, 3,ANGLE_Y, data);
		data = ((uint16_t)frame->Data[4])|((uint16_t)frame->Data[5]<<8);
		data = GetAdjustAngleValue(data);
		SetCanDeviceAPPMsg((uint16_t)pdata, ID_HUB2, 4,ANGLE_X, data);
		data = ((uint16_t)frame->Data[6])|((uint16_t)frame->Data[7]<<8);
		data = GetAdjustAngleValue(data);
		SetCanDeviceAPPMsg((uint16_t)pdata, ID_HUB2, 4,ANGLE_Y, data);
	}
}
/*******************************************************************************************
*��������������������HUB�ϵĽǶ�X,Y��Ƕ��ϱ����ݴ���
*��ڲ�����pdataȡֵ��
*             can֡��Դ��can1��can2��
*          frameȡֵ��
*             can֡
*����ֵ��  ��
*******************************************************************************************/
uint16_t ThirdHub_AngleXY_Proc(void *pdata , STR_Can_Frame* frame)
{
	uint16_t data;
	STR_V2CAN_Frame v2frame;
	stDeviceStatusMsg deviceMsg = {0};
	memcpy(&v2frame, frame, sizeof(STR_V2CAN_Frame));
	if(v2frame.u32ID.ID.Sub ==0)
	{
		//��Ƕ�xyֵ
		data = ((uint16_t)frame->Data[0])|((uint16_t)frame->Data[1]<<8);
		data = GetAdjustAngleValue(data);
		SetCanDeviceAPPMsg((uint16_t)pdata, ID_HUB3, 1,ANGLE_X, data);
		data = ((uint16_t)frame->Data[2])|((uint16_t)frame->Data[3]<<8);
		data = GetAdjustAngleValue(data);
		SetCanDeviceAPPMsg((uint16_t)pdata, ID_HUB3, 1,ANGLE_Y, data);
		data = ((uint16_t)frame->Data[4])|((uint16_t)frame->Data[5]<<8);
		data = GetAdjustAngleValue(data);
		SetCanDeviceAPPMsg((uint16_t)pdata, ID_HUB3, 2,ANGLE_X, data);
		data = ((uint16_t)frame->Data[6])|((uint16_t)frame->Data[7]<<8);
		data = GetAdjustAngleValue(data);
		SetCanDeviceAPPMsg((uint16_t)pdata, ID_HUB3, 2,ANGLE_Y, data);
	}
	if(v2frame.u32ID.ID.Sub ==1)
	{
		//��Ƕ�xyֵ
		data = ((uint16_t)frame->Data[0])|((uint16_t)frame->Data[1]<<8);
		data = GetAdjustAngleValue(data);
		SetCanDeviceAPPMsg((uint16_t)pdata, ID_HUB3, 3,ANGLE_X, data);
		data = ((uint16_t)frame->Data[2])|((uint16_t)frame->Data[3]<<8);
		data = GetAdjustAngleValue(data);
		SetCanDeviceAPPMsg((uint16_t)pdata, ID_HUB3, 3,ANGLE_Y, data);
		data = ((uint16_t)frame->Data[4])|((uint16_t)frame->Data[5]<<8);
		data = GetAdjustAngleValue(data);
		SetCanDeviceAPPMsg((uint16_t)pdata, ID_HUB3, 4,ANGLE_X, data);
		data = ((uint16_t)frame->Data[6])|((uint16_t)frame->Data[7]<<8);
		data = GetAdjustAngleValue(data);
		SetCanDeviceAPPMsg((uint16_t)pdata, ID_HUB3, 4,ANGLE_Y, data);
	}
}
/*******************************************************************************************
*����������������һ��HUB�ϵĽǶ�Z��Ƕ��ϱ����ݴ���
*��ڲ�����pdataȡֵ��
*             can֡��Դ��can1��can2��
*          frameȡֵ��
*             can֡
*����ֵ��  ��
*******************************************************************************************/
uint16_t FirstHub_AngleZ_Proc(void *pdata , STR_Can_Frame* frame)
{
	uint16_t data;
	STR_V2CAN_Frame v2frame;
	memcpy(&v2frame, frame, sizeof(STR_V2CAN_Frame));
	//��Ƕ�xyֵ
	data = ((uint16_t)frame->Data[0])|((uint16_t)frame->Data[1]<<8);
	data = GetAdjustAngleValue(data);
	SetCanDeviceAPPMsg((uint16_t)pdata, ID_HUB1, 1,ANGLE_Z, data);
	data = ((uint16_t)frame->Data[2])|((uint16_t)frame->Data[3]<<8);
	data = GetAdjustAngleValue(data);
	SetCanDeviceAPPMsg((uint16_t)pdata, ID_HUB1, 2,ANGLE_Z, data);
	data = ((uint16_t)frame->Data[4])|((uint16_t)frame->Data[5]<<8);
	data = GetAdjustAngleValue(data);
	SetCanDeviceAPPMsg((uint16_t)pdata, ID_HUB1, 3,ANGLE_Z, data);
	data = ((uint16_t)frame->Data[6])|((uint16_t)frame->Data[7]<<8);
	data = GetAdjustAngleValue(data);
	SetCanDeviceAPPMsg((uint16_t)pdata, ID_HUB1, 4,ANGLE_Z, data);
	
}

/*******************************************************************************************
*���������������ڶ���HUB�ϵĽǶ�Z��Ƕ��ϱ����ݴ���
*��ڲ�����pdataȡֵ��
*             can֡��Դ��can1��can2��
*          frameȡֵ��
*             can֡
*����ֵ��  ��
*******************************************************************************************/
uint16_t SecondHub_AngleZ_Proc(void *pdata , STR_Can_Frame* frame)
{
	uint16_t data;
	STR_V2CAN_Frame v2frame;
	memcpy(&v2frame, frame, sizeof(STR_V2CAN_Frame));
	//��Ƕ�xyֵ
	data = ((uint16_t)frame->Data[0])|((uint16_t)frame->Data[1]<<8);
	data = GetAdjustAngleValue(data);
	SetCanDeviceAPPMsg((uint16_t)pdata, ID_HUB2, 1,ANGLE_Z, data);
	data = ((uint16_t)frame->Data[2])|((uint16_t)frame->Data[3]<<8);
	data = GetAdjustAngleValue(data);
	SetCanDeviceAPPMsg((uint16_t)pdata, ID_HUB2, 2,ANGLE_Z, data);
	data = ((uint16_t)frame->Data[4])|((uint16_t)frame->Data[5]<<8);
	data = GetAdjustAngleValue(data);
	SetCanDeviceAPPMsg((uint16_t)pdata, ID_HUB2, 3,ANGLE_Z, data);
	data = ((uint16_t)frame->Data[6])|((uint16_t)frame->Data[7]<<8);
	data = GetAdjustAngleValue(data);
	SetCanDeviceAPPMsg((uint16_t)pdata, ID_HUB2, 4,ANGLE_Z, data);	
}
/*******************************************************************************************
*��������������������HUB�ϵĽǶ�Z��Ƕ��ϱ����ݴ���
*��ڲ�����pdataȡֵ��
*             can֡��Դ��can1��can2��
*          frameȡֵ��
*             can֡
*����ֵ��  ��
*******************************************************************************************/
uint16_t ThirdHub_AngleZ_Proc(void *pdata , STR_Can_Frame* frame)
{
	uint16_t data;
	STR_V2CAN_Frame v2frame;
	memcpy(&v2frame, frame, sizeof(STR_V2CAN_Frame));
	//��Ƕ�xyֵ
	data = ((uint16_t)frame->Data[0])|((uint16_t)frame->Data[1]<<8);
	data = GetAdjustAngleValue(data);
	SetCanDeviceAPPMsg((uint16_t)pdata, ID_HUB3, 1,ANGLE_Z, data);
	data = ((uint16_t)frame->Data[2])|((uint16_t)frame->Data[3]<<8);
	data = GetAdjustAngleValue(data);
	SetCanDeviceAPPMsg((uint16_t)pdata, ID_HUB3, 2,ANGLE_Z, data);
	data = ((uint16_t)frame->Data[4])|((uint16_t)frame->Data[5]<<8);
	data = GetAdjustAngleValue(data);
	SetCanDeviceAPPMsg((uint16_t)pdata, ID_HUB3, 3,ANGLE_Z, data);
	data = ((uint16_t)frame->Data[6])|((uint16_t)frame->Data[7]<<8);
	data = GetAdjustAngleValue(data);
	SetCanDeviceAPPMsg((uint16_t)pdata, ID_HUB3, 4,ANGLE_Z, data);
	
}
/*******************************************************************************************
*�������������ⱨ����-����
*��ڲ�����pdataȡֵ��
*             can֡��Դ��can1��can2��
*          frameȡֵ��
*             can֡
*����ֵ��  ��
*******************************************************************************************/
uint16_t Alarm_Heart_Proc(void *pdata , STR_Can_Frame* frame)
{
	uint8_t device;
	stDeviceStatusMsg deviceMsg = {0};
	if(frame->Data[4] == DEVICEINIT)
	{
		CanProSendParam((uint16_t)pdata, ID_PERSON,0);
	}		
	device = frame->Data[3];
	if(device == V2_ALARM_HEART || device == V2_ALARMBL_HEART)
	{
		GetCanDeviceStatusMsg((uint16_t)pdata,ID_ALARM, ALARM_Version, &deviceMsg);
		
		if(frame->Data[4] == DEVICEPROGRAM)
			deviceMsg.deviceStatus = DEVICEPROGRAM;
		else
			deviceMsg.deviceStatus = DEVICENORMAL;
		
		deviceMsg.device = ID_ALARM;
		deviceMsg.subDevice = ALARM_Version;
		deviceMsg.HeartTimer = V2_ALARM_HEARTTIMER;
		if(device == V2_ALARM_HEART)
		{
			deviceMsg.deviceVersion[0] = frame->Data[5];
			deviceMsg.deviceVersion[1] = frame->Data[6];
			deviceMsg.deviceVersion[2] = frame->Data[7];
		}
		else
		{
			deviceMsg.deviceBLVersion[0] = frame->Data[5];
			deviceMsg.deviceBLVersion[1] = frame->Data[6];
			deviceMsg.deviceBLVersion[2] = frame->Data[7];	
		}
	}
	else if(device == V2_PERSON_HEART)
	{
		GetCanDeviceStatusMsg((uint16_t)pdata,ID_PERSON, PERSON_Version, &deviceMsg);	
		if(frame->Data[4] == DEVICEPROGRAM)
			deviceMsg.deviceStatus = DEVICEPROGRAM;
		else
			deviceMsg.deviceStatus = DEVICENORMAL;
		
		deviceMsg.device = ID_PERSON;
		deviceMsg.subDevice = PERSON_Version;
		deviceMsg.HeartTimer = V2_ALARM_HEARTTIMER;
		deviceMsg.deviceVersion[0] = frame->Data[5];
		deviceMsg.deviceVersion[1] = frame->Data[6];
		deviceMsg.deviceVersion[2] = frame->Data[7];
	}
	else if(device == V2_PERSONBL_HEART)
	{
		if(frame->Data[4] == DEVICEPROGRAM)
			deviceMsg.deviceStatus = DEVICEPROGRAM;
		else
			deviceMsg.deviceStatus = DEVICENORMAL;
		
		deviceMsg.device = ID_PERSON;
		deviceMsg.subDevice = PERSON_Version;
		deviceMsg.HeartTimer = V2_ALARM_HEARTTIMER;
		deviceMsg.deviceBLVersion[0] = frame->Data[5];
		deviceMsg.deviceBLVersion[1] = frame->Data[6];
		deviceMsg.deviceBLVersion[2] = frame->Data[7];	
	}
	
	deviceMsg.isOnline = DEVICEONLINE;
//	deviceMsg.isEnable = DEVICEENABLE;
	deviceMsg.backTimer = deviceMsg.HeartTimer;
	
	SetCanDeviceStatusMsg((uint16_t)pdata,deviceMsg.device, deviceMsg.subDevice, &deviceMsg);	
	return 1;
}


/*******************************************************************************************
*�������������ⱨ����-�ϱ����ݣ���Ա��
*��ڲ�����pdataȡֵ��
*             can֡��Դ��can1��can2��
*          frameȡֵ��
*             can֡
*����ֵ��  ��
*******************************************************************************************/
uint16_t Alarm_Report_Proc(void *pdata , STR_Can_Frame* frame)
{
	uint32_t data,data1;
	uint8_t tmp,status,pos;
	uint8_t Tmp[12];
	stDeviceStatusMsg deviceMsg = {0};
	STR_V2CAN_Frame v2frame;
	
	memcpy(&v2frame,frame,sizeof(STR_Can_Frame));
						
	GetCanDeviceStatusMsg((uint16_t)pdata, ID_PERSON, PERSON_Version,&deviceMsg);
	deviceMsg.HeartTimer = V2_PERSON_HEARTTIMER;
	SetCanDeviceStatusMsg((uint16_t)pdata, ID_PERSON, PERSON_Version, &deviceMsg);	
	
	Tmp[0] = v2frame.u32ID.u32Id & 0xff;
	Tmp[1] = (v2frame.u32ID.u32Id >> 8) & 0xff;
	Tmp[2] = (v2frame.u32ID.u32Id >> 16) & 0xff;
	Tmp[3] = (v2frame.u32ID.u32Id >> 24) & 0xff;
	Tmp[4] = v2frame.u16DLC & 0xff;
	Tmp[5] = v2frame.u8DT[0];
	Tmp[6] = v2frame.u8DT[1];
	Tmp[7] = v2frame.u8DT[2];
	Tmp[8] = v2frame.u8DT[3];
	Tmp[9] = v2frame.u8DT[4];
	Tmp[10] = v2frame.u8DT[5];
	Tmp[11] = v2frame.u8DT[6];	
	tmp = CRC_8(Tmp, 12);
	
	if(tmp != v2frame.u8DT[7])
		return 0;
	
	status = v2frame.u8DT[1]&0x0F;
	
	data = ((uint16_t)v2frame.u8DT[2])|((uint16_t)v2frame.u8DT[3]<<8);  // ���
	
	if(data<UWBDYKMIN)
		return 0;
	
	for(tmp = 0; tmp<11; tmp++)
	{
		GetCanDeviceAPPMsg((uint16_t)pdata, ID_PERSON,tmp,PERSONNUM,&data1);
		if(data1 == 0)
			pos = tmp;
		if(data1 == data)
		{
			if(status>PERSONINTO)
			{
				SetCanDeviceAPPMsg((uint16_t)pdata, ID_PERSON, tmp, PERSONMSG, 0);
				SetCanDeviceAPPMsg((uint16_t)pdata, ID_PERSON, tmp, PERSONNUM, 0);
				SetCanDeviceAPPMsg((uint16_t)pdata, ID_PERSON, tmp, PERSONDIS, 0);
				SetCanDeviceAPPMsg((uint16_t)pdata, ID_PERSON, tmp, PERSONBATTRY,0);
				SetCanDeviceAPPMsg((uint16_t)pdata, ID_PERSON, tmp, PERSONTIMEOUT,0);				
			}
			else
			{
				GetCanDeviceStatusMsg((uint16_t)pdata,ID_PERSON,PERSON_Version,&deviceMsg);
				if(deviceMsg.isEnable == 0)
				{
					SetCanDeviceAPPMsg((uint16_t)pdata, ID_PERSON, tmp, PERSONMSG, 0);
					SetCanDeviceAPPMsg((uint16_t)pdata, ID_PERSON, tmp, PERSONNUM, 0);
					SetCanDeviceAPPMsg((uint16_t)pdata, ID_PERSON, tmp, PERSONDIS, 0);
					SetCanDeviceAPPMsg((uint16_t)pdata, ID_PERSON, tmp, PERSONBATTRY,0);
					SetCanDeviceAPPMsg((uint16_t)pdata, ID_PERSON, tmp, PERSONTIMEOUT, 0);
					return 0;
				}
				data = ((uint16_t)v2frame.u8DT[0])|((uint16_t)v2frame.u8DT[1]<<8);
				SetCanDeviceAPPMsg((uint16_t)pdata, ID_PERSON, tmp, PERSONMSG, data);
				data = ((uint16_t)v2frame.u8DT[2])|((uint16_t)v2frame.u8DT[3]<<8);
				SetCanDeviceAPPMsg((uint16_t)pdata, ID_PERSON, tmp, PERSONNUM, data);
				data = ((uint16_t)v2frame.u8DT[4])|((uint16_t)v2frame.u8DT[5]<<8);
				SetCanDeviceAPPMsg((uint16_t)pdata, ID_PERSON, tmp, PERSONDIS, data);
				data = 30;
				SetCanDeviceAPPMsg((uint16_t)pdata, ID_PERSON, tmp, PERSONTIMEOUT, data);
				data = v2frame.u8DT[6];
				SetCanDeviceAPPMsg((uint16_t)pdata, ID_PERSON, tmp, PERSONBATTRY, data);			
			}
			break;
		}
		if(tmp == 10)
		{
			if(status == PERSONINTO)
			{
				GetCanDeviceStatusMsg((uint16_t)pdata,ID_PERSON,PERSON_Version,&deviceMsg);
				if(deviceMsg.isEnable == 0)
				{
					SetCanDeviceAPPMsg((uint16_t)pdata, ID_PERSON, tmp, PERSONMSG, 0);
					SetCanDeviceAPPMsg((uint16_t)pdata, ID_PERSON, tmp, PERSONNUM, 0);
					SetCanDeviceAPPMsg((uint16_t)pdata, ID_PERSON, tmp, PERSONDIS, 0);
					SetCanDeviceAPPMsg((uint16_t)pdata, ID_PERSON, tmp, PERSONBATTRY,0);
					SetCanDeviceAPPMsg((uint16_t)pdata, ID_PERSON, tmp, PERSONTIMEOUT, 0);
					return 0;
				}
				data = ((uint16_t)v2frame.u8DT[0])|((uint16_t)v2frame.u8DT[1]<<8);
				SetCanDeviceAPPMsg((uint16_t)pdata, ID_PERSON, pos, PERSONMSG, data);
				data = ((uint16_t)v2frame.u8DT[2])|((uint16_t)v2frame.u8DT[3]<<8);
				SetCanDeviceAPPMsg((uint16_t)pdata, ID_PERSON, pos, PERSONNUM, data);
				data = ((uint16_t)v2frame.u8DT[4])|((uint16_t)v2frame.u8DT[5]<<8);
				SetCanDeviceAPPMsg((uint16_t)pdata, ID_PERSON, pos, PERSONDIS, data);
				data = 30;
				SetCanDeviceAPPMsg((uint16_t)pdata, ID_PERSON, tmp, PERSONTIMEOUT, data);
				data = v2frame.u8DT[6];
				SetCanDeviceAPPMsg((uint16_t)pdata, ID_PERSON, tmp, PERSONBATTRY, data);			
			}
		}
	}
	return 1;
}

CANPROHEAD v2_emvdinfo[] = 
{
	/*��ŷ�������*/
	//����
	{
		1,       //�豸����
		0,       //֡����
		0,       //ӳ���
		0,       //��ʱʱ��
		0,       //���ʹ���
		0,       //Ӧ��
		Emvd_Heart_Proc,    //���ջص�����
		NULL,               
		NULL,               
		NULL,               
	},
		//����
	{
		1,       //�豸����
		1,       //֡����
		1,       //ӳ���
		0,       //��ʱʱ��
		0,       //���ʹ���
		0,       //Ӧ��
		NULL,     
		NULL,     
		NULL,     
		NULL,     //����ǰ����ص�����
	},
	//���
	{
		1,       //�豸����
		2,       //֡����
		2,       //ӳ���
		30,       //��ʱʱ��
		1,       //���ʹ���
		0,       //Ӧ��
		NULL,     
		NULL,     
		NULL,     
		NULL,     //����ǰ����ص�����
	},
	//�����
	{
		1,       //�豸����
		3,       //֡����
		3,       //ӳ���
		0,       //��ʱʱ��
		0,       //���ʹ���
		0,       //Ӧ��
		Emvd_Check_Proc,     //���ջص�����
		NULL,     
		NULL,     
		NULL,    
	},	
	//����
	{
		1,       //�豸����
		4,       //֡����
		0x11,       //ӳ���
		0,       //��ʱʱ��
		0,       //���ʹ���
		0,       //Ӧ��
		NULL,     
		NULL,     
		NULL,     
		NULL,     //����ǰ����ص�����
	},
	//���
	{
		1,       //�豸����
		5,       //֡����
		0x12,       //ӳ���
		0,       //��ʱʱ��
		0,       //���ʹ���
		0,       //Ӧ��
		NULL,     
		NULL,     
		NULL,     
		NULL,     //����ǰ����ص�����
	},
	//�����
	{
		1,       //�豸����
		6,       //֡����
		0x13,    //ӳ���
		0,       //��ʱʱ��
		0,       //���ʹ���
		0,       //Ӧ��
		NULL,    //���ջص�����
		NULL,     
		NULL,     
		NULL,    
	},	
};
CANPROHEAD v2_hubinfo[] = 
{
	/*HUB�����*/
	//HUB������Ƕ������ϱ�
	{
		2,       //�豸����
		0,       //ӳ���
		0,       //֡����
		0,       //��ʱʱ��
		0,       //���ʹ���
		0,       //Ӧ��
		Hub_Heart_Proc,    //���ջص�����
		NULL,               
		NULL,               
		NULL,               
	},
	/*��һ��HUB�����*/
	//ģ���������ϱ�
	{
		2,       //�豸����
		1,       //ӳ���
		2,    //֡����
		0,       //��ʱʱ��
		0,       //���ʹ���
		0,       //Ӧ��
		FirstHub_Analog_Proc,    //���ջص�����
		NULL,               
		NULL,               
		NULL,               
	},
	/*��һ��HUB�����*/
	//�Ƕȴ�����X,Y�������ϱ�
	{
		2,       //�豸����
		2,       //ӳ���
		4,    //֡����
		0,       //��ʱʱ��
		0,       //���ʹ���
		0,       //Ӧ��
		FirstHub_AngleXY_Proc,    //���ջص�����
		NULL,               
		NULL,               
		NULL,               
	},
	/*��һ��HUB�����*/
	//�Ƕȴ�����Z�������ϱ�
	{
		2,       //�豸����
		3,       //ӳ���
		24,    //֡����
		0,       //��ʱʱ��
		0,       //���ʹ���
		0,       //Ӧ��
		FirstHub_AngleZ_Proc,    //���ջص�����
		NULL,               
		NULL,               
		NULL,               
	},
	/*�ڶ���HUB�����*/
	//ģ���������ϱ�
	{
		2,       //�豸����
		4,       //ӳ���
		32,    	//֡����
		0,       //��ʱʱ��
		0,       //���ʹ���
		0,       //Ӧ��
		SecondHub_Analog_Proc,    //���ջص�����
		NULL,               
		NULL,               
		NULL,               
	},
	/*�ڶ���HUB�����*/
	//�Ƕȴ�����X,Y�������ϱ�
	{
		2,       //�豸����
		5,       //ӳ���
		34,    //֡����
		0,       //��ʱʱ��
		0,       //���ʹ���
		0,       //Ӧ��
		SecondHub_AngleXY_Proc,    //���ջص�����
		NULL,               
		NULL,               
		NULL,               
	},
	/*�ڶ���HUB�����*/
	//�Ƕȴ�����Z�������ϱ�
	{
		2,       //�豸����
		6,       //ӳ���
		54,    //֡����
		0,       //��ʱʱ��
		0,       //���ʹ���
		0,       //Ӧ��
		SecondHub_AngleZ_Proc,    //���ջص�����
		NULL,               
		NULL,               
		NULL,               
	},
	/*������HUB�����*/
	//ģ���������ϱ�
	{
		2,       //�豸����
		7,       //ӳ���
		62,    //֡����
		0,       //��ʱʱ��
		0,       //���ʹ���
		0,       //Ӧ��
		ThirdHub_Analog_Proc,    //���ջص�����
		NULL,               
		NULL,               
		NULL,               
	},
	/*������HUB�����*/
	//�Ƕȴ�����X,Y�������ϱ�
	{
		2,       //�豸����
		8,       //ӳ���
		64,    //֡����
		0,       //��ʱʱ��
		0,       //���ʹ���
		0,       //Ӧ��
		ThirdHub_AngleXY_Proc,    //���ջص�����
		NULL,               
		NULL,               
		NULL,               
	},
	/*������HUB�����*/
	//�Ƕȴ�����Z�������ϱ�
	{
		2,       //�豸����
		9,       //ӳ���
		84,    	//֡����
		0,       //��ʱʱ��
		0,       //���ʹ���
		0,       //Ӧ��
		ThirdHub_AngleZ_Proc,    //���ջص�����
		NULL,               
		NULL,               
		NULL,               
	},
	

};


CANPROHEAD v2_irrxinfo[] = 
{
	/*���ⷢ����*/
	//����
	{
		3,       //�豸����
		0,       //֡����
		0,       //ӳ���
		0,       //��ʱʱ��
		0,       //���ʹ���
		0,       //Ӧ��
		Ir_Heart_Proc,    //���ջص�����
		NULL,               
		NULL,               
		NULL,               
	},
	//�����ϱ�
	{
		3,       //�豸����
		1,       //֡����
		1,       //ӳ���
		0,       //��ʱʱ��
		0,       //���ʹ���
		0,       //Ӧ��
		Ir_Rx_Proc,
		NULL,     
		NULL,     
		NULL,     //����ǰ����ص�����
	},	
    //ң�������߶���
	{
		3,       //�豸����
		6,       //֡����
		6,       //ӳ���
		0,       //��ʱʱ��
		0,       //���ʹ���
		0,       //Ӧ��
		WirelessAlarm_CodeMatching_Proc,
		NULL,     
		NULL,     
		NULL,     //����ǰ����ص�����
	},
	//ң�������߶���Ӧ��
	{
		3,       //�豸����
		7,       //֡����
		7,       //ӳ���
		0,       //��ʱʱ��
		0,       //���ʹ���
		0,       //Ӧ��
		NULL,
		NULL,     
		NULL,     
		NULL,     //����ǰ����ص�����
	},
	//ң�������߿��ƣ��������һ����������
	{
		3,       //�豸����
		10,       //֡����
		10,       //ӳ���
		0,       //��ʱʱ��
		0,       //���ʹ���
		0,       //Ӧ��
		WirelessAlarm_KeyProc,
		NULL,     
		NULL,     
		NULL,     //����ǰ����ص�����
	},
	//ң�������߿��ƣ��������һ�����£�������Ӧ��
	{
		3,       //�豸����
		11,       //֡����
		11,       //ӳ���
		0,       //��ʱʱ��
		0,       //���ʹ���
		0,       //Ӧ��
		NULL,
		NULL,     
		NULL,     
		NULL,     //����ǰ����ص�����
	},
	//ң�������߿��ƣ�������������������
	{
		3,       //�豸����
		12,       //֡����
		12,       //ӳ���
		0,       //��ʱʱ��
		0,       //���ʹ���
		0,       //Ӧ��
		NULL,
		NULL,     
		NULL,     
		NULL,     //����ǰ����ص�����
	},
	//ң�������߿��ƣ�������������������
	{
		3,       //�豸����
		13,       //֡����
		13,       //ӳ���
		0,       //��ʱʱ��
		0,       //���ʹ���
		0,       //Ӧ��
		NULL,
		NULL,     
		NULL,     
		NULL,     //����ǰ����ص�����
	},
	//ң�������߿��ƣ�������������������
	{
		3,       //�豸����
		15,       //֡����
		15,       //ӳ���
		0,       //��ʱʱ��
		0,       //���ʹ���
		0,       //Ӧ��
		WirelessAlarm_PassiveDecodePro,
		NULL,     
		NULL,     
		NULL,     //����ǰ����ص�����
	},
	//ң�������߿��ƣ�ң���������ڼܿ�������
	{
		3,       //�豸����
		19,       //֡����
		19,       //ӳ���
		0,       //��ʱʱ��
		0,       //���ʹ���
		0,       //Ӧ��
		WirelessAlarm_Neighbor_Request_Proc,
		NULL,     
		NULL,     
		NULL,     //����ǰ����ص�����
	},
	//ң�������߿��ƣ�ң���������ڼܿ�������
	{
		3,       //�豸����
		18,       //֡����
		18,       //ӳ���
		0,       //��ʱʱ��
		0,       //���ʹ���
		0,       //Ӧ��
		WirelessAlarm_Para_Request_Proc,
		NULL,     
		NULL,     
		NULL,     //����ǰ����ص�����
	},
	
};



CANPROHEAD v2_angleinfo[] = 
{
	/*�Ƕȡ��߶ȴ�����*/
	//����
	{
		5,       //�豸����
		0,       //֡����
		0,       //ӳ���
		0,       //��ʱʱ��
		0,       //���ʹ���
		0,       //Ӧ��
		Angle_Heart_Proc,    //���ջص�����
		NULL,     
		NULL,     
		NULL,    
	},
	//�Ƕ��ϱ�
	{
		5,       //�豸����
		1,       //֡����
		0x100,       //ӳ���
		0,       //��ʱʱ��
		0,       //���ʹ���
		0,       //Ӧ��
		Angle_Report_Proc,    //���ջص�����
		NULL,     
		NULL,     
		NULL,    
	},
	//�ǶȲ�ѯ
	{
		5,       //�豸����
		2,       //֡����
		0x101,       //ӳ���
		0,       //��ʱʱ��
		0,       //���ʹ���
		0,       //Ӧ��
		NULL,    
		NULL,     
		NULL,     
		NULL,    //����ǰ����ص�����
	},	
	//���ù�������
	{
		5,       //�豸����
		3,       //֡����
		0x105,       //ӳ���
		0,       //��ʱʱ��
		0,       //���ʹ���
		0,       //Ӧ��
		NULL,    
		NULL,     
		NULL,     
		NULL,    //����ǰ����ص�����
	},
	//��������Ӧ��
	{
		5,       //�豸����
		4,       //֡����
		0x105,       //ӳ���
		0,       //��ʱʱ��
		0,       //���ʹ���
		0,       //Ӧ��
		NULL,    
		NULL,     
		NULL,     
		NULL,    //����ǰ����ص�����
	},
};

CANPROHEAD v2_alarminfo[] = 
{
	/*���⡢��Ա������*/
	//����
	{
		7,       //�豸����
		0,       //֡����
		0,       //ӳ���
		0,       //��ʱʱ��
		0,       //���ʹ���
		0,       //Ӧ��
		Alarm_Heart_Proc,     //���ջص�����
		NULL,     
		NULL,     
		NULL,    
	},
	//��Ա��λ��Ϣ�ϱ�
	{
		7,       //�豸����
		1,       //֡����
		1,       //ӳ���
		0,       //��ʱʱ��
		0,       //���ʹ���
		0,       //Ӧ��
		Alarm_Report_Proc,    //���ջص�����
		NULL,     
		NULL,     
		NULL,    
	},
	//��ѯ��Ա��λ��Ϣ
	{
		7,       //�豸����
		2,       //֡����
		2,       //ӳ���
		0,       //��ʱʱ��
		0,       //���ʹ���
		0,       //Ӧ��
		NULL,     
		NULL,     
		NULL,     
		NULL,    //����ǰ����ص�����
	},
	//���ù�������
	{
		7,       //�豸����
		3,       //֡����
		3,       //ӳ���
		0,       //��ʱʱ��
		0,       //���ʹ���
		0,       //Ӧ��
		NULL,     //���ջص�����
		NULL,     
		NULL,     
		NULL,    
	},
	//���ⱨ��Ԥ��
	{
		7,       //�豸����
		4,       //֡����
		4,       //ӳ���
		0,       //��ʱʱ��
		0,       //���ʹ���
		0,       //Ӧ��
		NULL,     
		NULL,     
		NULL,     
		NULL,    //����ǰ����ص�����
	},
	//���ⱨ������
	{
		7,       //�豸����
		5,       //֡����
		5,       //ӳ���
		0,       //��ʱʱ��
		0,       //���ʹ���
		0,       //Ӧ��
		NULL,     //���ջص�����
		NULL,     
		NULL,     
		NULL,    
	},
	//��λ
	{
		7,       //�豸����
		6,       //֡����
		6,       //ӳ���
		30,       //��ʱʱ��
		3,       //���ʹ���
		0,       //Ӧ��
		NULL,     //���ջص�����
		NULL,     
		NULL,     
		NULL,    
	},
};


CANPROHEAD v2_cxbinfo[] = 
{
	/*�����*/
	//����
	{
		4,       //�豸����
		0,       //֡����
		4,       //ӳ���
		0,       //��ʱʱ��
		0,       //���ʹ���
		0,       //Ӧ��
		CXB_Heart_Proc,     //���ջص�����
		NULL,     
		NULL,     
		NULL,    
	},
	//SC�������س���
	{
		4,       //�豸����
		1,       //֡����
		0,       //ӳ���
		1000,    //��ʱʱ��
		3,       //���ʹ���
		1,       //Ӧ��
		CXB_Send_ACK_Proc,    //���ջص�����
		CXB_Send_NOACK_Proc,     
		NULL,     
		NULL,    
	},
	//CXB���س���汾��Ϣ
	{
		4,       //�豸����
		3,       //֡����
		1,       //ӳ���
		0,       //��ʱʱ��
		0,       //���ʹ���
		0,       //Ӧ��
		CXB_Program_Proc,     //���ջص�����
		NULL,     
		NULL,     
		NULL,    
	},
	//CXB���Ͱ汾��ϢУ���
	{
		4,       //�豸����
		4,       //֡����
		6,       //ӳ���
		0,       //��ʱʱ��
		0,       //���ʹ���
		0,       //Ӧ��
		CXB_Program_Crc_Proc,     
		NULL,     
		NULL,     
		NULL,    //����ǰ����ص�����
	},
	//SC���Ͱ汾��ϢУ�����Ӧ��
	{
		4,       //�豸����
		5,       //֡����
		6,       //ӳ���
		0,       //��ʱʱ��
		0,       //���ʹ���
		0,       //Ӧ��
		NULL,     //���ջص�����
		NULL,     
		NULL,     
		NULL,    
	},
	//CXB���ͳ������
	{
		4,       //�豸����
		6,       //֡����
		2,       //ӳ���
		0,       //��ʱʱ��
		0,       //���ʹ���
		0,       //Ӧ��
		CXB_Program_Proc,     //���ջص�����
		NULL,
		NULL,
		NULL,
	},
	//CXB���ʹ���У���
	{
		4,       //�豸����
		7,       //֡����
		7,       //ӳ���
		0,       //��ʱʱ��
		0,       //���ʹ���
		0,       //Ӧ��
		CXB_Program_Crc_Proc,     //���ջص�����
		NULL,
		NULL,
		NULL,
	},
};


/*******************************************************************************************
**�������ƣ���������
**�������ã�
**����������
**�����������
**ע�������
** ������:	
** �ա��ڣ� 
*******************************************************************************************/
uint32_t Get_V2RxID_Type(STR_V2CAN_Frame * rxCan)
{
	return  rxCan->u32ID.ID.RxID;
}
uint32_t Get_V2TxID_Type(STR_V2CAN_Frame * rxCan)
{
	return  rxCan->u32ID.ID.TxID;
}
uint32_t Get_V2Frame_Type(STR_V2CAN_Frame * rxCan)
{
	return  rxCan->u32ID.ID.FrameType;
}
uint32_t Get_V2Frame_Program(STR_V2CAN_Frame * rxCan)
{
	return  rxCan->u32ID.ID.Reservd;
}


/*******************************************************************************************
*�����������Ƕȡ��߶ȡ���Ա�Ĳ�����ʼ��
*��ڲ�������
*����ֵ��  ��
*******************************************************************************************/
void v2AngleandPersonInit(void)
{
	#if OS_CRITICAL_METHOD == 3                      
    OS_CPU_SR  cpu_sr = 0;
	#endif
	OS_ENTER_CRITICAL();
	v2_person_para.scNum = 0x0005;
	v2_person_para.u8WORKPARAM.WORKPARAM.workMode = 1;
	v2_person_para.u8WORKPARAM.WORKPARAM.calcMode = 0;	
	v2_person_para.u8WORKPARAM.WORKPARAM.disVariation = 0;
	v2_person_para.u8WORKPARAM.WORKPARAM.trsPower = 11;
	v2_person_para.interval = 20;
	v2_person_para.range = 50;
	v2_person_para.scHigh = 0;
	
	v2_angle_para.forwardTimes = 0x00;
	v2_angle_para.workMode = 0x02;
	v2_angle_para.interval = ANGLE_REPORT_TIMER;
	v2_angle_para.RxID = V2ID_SC;
	v2_angle_para.angleVariation = ANGLE_REPORT_VALUE;
	v2_angle_para.RxSubID  = 0;
	v2_angle_para.reserve = 0;	
	
	v2_high_para.forwardTimes = 0x00;
	v2_high_para.workMode = 0x02;
	v2_high_para.interval = HIGH_REPORT_TIMER;
	v2_high_para.RxID = V2ID_SC;
	v2_high_para.angleVariation = HIGH_REPORT_VALUE;
	v2_high_para.RxSubID  = 1;
	v2_high_para.reserve = 0;	
	OS_EXIT_CRITICAL();
}


/*******************************************************************************************
*���������������豸��֡id��ִ�ж�Ӧ��can���ջص���ѯ
*��ڲ�����deviceȡֵ��
*             �豸����
*          frameȡֵ��
*             can֡
*          headȡֵ��
*             can֡
*����ֵ��  ��
*******************************************************************************************/
uint8_t getV2ProDeviceMsg(uint8_t device, uint16_t frameid, CANPROHEAD* head)
{
	uint8_t res = 0;
	uint8_t i;
	switch(device)
	{
		case V2ID_EMVD:
			for(i = 0;i< sizeof(v2_emvdinfo)/sizeof(CANPROHEAD); i++)
			{
				if(frameid == v2_emvdinfo[i].frameid)
				{
					res = 1;
					memcpy(head,&(v2_emvdinfo[i]),sizeof(CANPROHEAD));
					break;
				}
			}
			break;
		case V2ID_HUB:
			for(i = 0;i< sizeof(v2_hubinfo)/sizeof(CANPROHEAD); i++)
			{
				if(frameid == v2_hubinfo[i].frameid)
				{
					res = 1;
					memcpy(head,&(v2_hubinfo[i]),sizeof(CANPROHEAD));
					break;
				}
			}
			break;		
		case V2ID_ANGLE:
			for(i = 0;i< sizeof(v2_angleinfo)/sizeof(CANPROHEAD); i++)
			{
				if(frameid == v2_angleinfo[i].frameid)
				{
					res = 1;
					memcpy(head,&(v2_angleinfo[i]),sizeof(CANPROHEAD));
					break;
				}
			}
			break;
		 case V2ID_ALARM:
			for(i = 0;i< sizeof(v2_alarminfo)/sizeof(CANPROHEAD); i++)
			{
				if(frameid == v2_alarminfo[i].frameid)
				{
					res = 1;
					memcpy(head,&(v2_alarminfo[i]),sizeof(CANPROHEAD));
					break;
				}
			}
			break;
			case V2ID_WL:
			for(i = 0;i< sizeof(v2_irrxinfo)/sizeof(CANPROHEAD); i++)
			{
				if(frameid == v2_irrxinfo[i].frameid)
				{
					res = 1;
					memcpy(head,&(v2_irrxinfo[i]),sizeof(CANPROHEAD));
					break;
				}
			}
			break;
			case V2ID_CXB:
			for(i = 0;i< sizeof(v2_cxbinfo)/sizeof(CANPROHEAD); i++)
			{
				if(frameid == v2_cxbinfo[i].frameid)
				{
					res = 1;
					memcpy(head,&(v2_cxbinfo[i]),sizeof(CANPROHEAD));
					break;
				}
			}
			break;
		default:
			res = 0;
			break;
	}
	if(res == 0)
			res = 0;
	return res;	
}

/*******************************************************************************************
*����������V4�豸����ת��V2���豸����
*��ڲ�����deviceȡֵ��
*             �豸����
*����ֵ��  ��
*******************************************************************************************/
uint8_t deviceToV2type(uint8_t device)
{
	uint8_t res;
	switch(device)
	{
		case ID_ANGLE:
			res = V2ID_ANGLE;
			break;
		case ID_HIGH:
			res = V2ID_ANGLE;
			break;
		case ID_ALARM:
			res = V2ID_ALARM;
			break;
		case ID_PERSON:
			res = V2ID_PERSON;
			break;		
		case ID_CXB:
			res = V2ID_CXB;
			break;		
		case ID_IRRX:
			res = V2ID_WL;
			break;
	}
	return res;
}


/*******************************************************************************************
*����������V4�����������ͣ�תV2������������
*��ڲ�����deviceȡֵ��
*             �豸����
*����ֵ��  ��
*******************************************************************************************/
uint8_t deviceToV2type_update(uint8_t device)
{
	uint8_t res;
	switch(device)
	{
		case GUI_TYPE_SC://"������"
			res = LOAD_TYPE_SC;
			break;
		case GUI_TYPE_EMVD://"��ŷ�"
			res = LOAD_TYPE_EMVD;
			break;
		case GUI_TYPE_ANGLE://"��  ��"
			res = LOAD_TYPE_ANGLE;
			break;
		case GUI_TYPE_PERSON://"��  Ա"
			res = LOAD_TYPE_PERSON;
			break;	
		case GUI_TYPE_ALARM://"��  ��"
			res = LOAD_TYPE_ALARM;
			break;
		case GUI_TYPE_HIGH://"��  ��"
			res = LOAD_TYPE_HIGH;
			break;
		case GUI_TYPE_IR://"����RX"
			res = LOAD_TYPE_IR;
			break;
		case GUI_TYPE_KEYBOARD://"����"
			res = LOAD_TYPE_KEYBOARD;
			break;
		case GUI_TYPE_WIRELESS://"����"
			res = LOAD_TYPE_WL;
			break;
		case GUI_TYPE_PERSONBL://"��Աbl"
			res = LOAD_TYPE_PERSON_BL;
			break;
		case GUI_TYPE_ALARMBL://"����bl"
			res = LOAD_TYPE_ALARM_BL;
			break;
		case GUI_TYPE_HIGHBL://"�߶�bl"
			res = LOAD_TYPE_HIGH_BL;
			break;
	    case GUI_TYPE_ALARM_UWB_ANGLE: //ghy "�๦�����ⱨ����"
			res = LOAD_TYPE_ALARM_UWB_ANGLE;
			break;
		case GUI_TYPE_HUB: //ghy MiniHUB
			res = LOAD_TYPE_MINIHUB;
			break;
	}
	return res;
}


/*******************************************************************************************
*����������V2Э����ճ�ʱ
*��ڲ�����frameȡֵ��
*             can֡
*����ֵ��  ��
*******************************************************************************************/
void V2CanRevTimeoutProc(STR_V2CAN_Frame* frame)
{
	uint8_t TxId;
	uint16_t FrameType;
	STR_V2CXBCAN_Frame cxbFrame;
	CANPROHEAD prohead;	
	
	if((frame->u32ID.u32Id & V2CANPROFRAME_UPDATE) != V2CANPROFRAME_UPDATE)		
	{
		TxId = Get_V2TxID_Type(frame);
		FrameType = Get_V2Frame_Type(frame);
			
		if(getV2ProDeviceMsg(TxId,FrameType,&prohead))
			prohead.overcallbacfun(NULL,(STR_Can_Frame*)frame);
	}
	else if((frame->u32ID.u32Id & V2CANPROFRAME_UPDATE) == V2CANPROFRAME_UPDATE)
	{
		memcpy(&cxbFrame,frame,sizeof(STR_V2CXBCAN_Frame));
		FrameType = cxbFrame.u32ID.ID.FrameType;
		TxId = cxbFrame.u32ID.ID.RxID;
		if(getV2ProDeviceMsg(TxId,FrameType,&prohead))
			prohead.overcallbacfun(NULL,(STR_Can_Frame*)frame);
	}
		
}

/*******************************************************************************************
*����������V2���ò������Ƕȡ��߶ȡ���Ա��
*��ڲ�����deviceȡֵ��
*             �豸����
*          paramȡֵ��
*             ����ָ��
*          isNowȡֵ��
*             �Ƿ���������  1����������
*����ֵ��  ��
*******************************************************************************************/
uint8_t V2CanProSetParam(uint16_t device, void * param,uint8_t isNow)
{
	uint8_t res = 1;
	uint16_t v2device;
	uint8_t driver = 0xff;
	v2device = deviceToV2type(device);
	switch(device)
	{
		case ID_ANGLE:	
			memcpy(&v2_angle_para, param, sizeof(stAngleParam));		
			break;		
		case ID_HIGH:	
			memcpy(&v2_high_para, param, sizeof(stAngleParam));		
			break;		
		case ID_PERSON:	
			memcpy(&v2_person_para, param, sizeof(stPersonParam));
			if(GetDeviceCanMsg(device,0,&driver) == 0)
				return 0;
			if(isNow)
				CanProSend(driver,ID_PERSON,0,ALARM_RESET,1,0);
			break;
		default :
			res = 0;
			break;
	}
	return res;

}


/*******************************************************************************************
*����������V2���Ͳ������Ƕȡ��߶ȡ���Ա��
*��ڲ�����deviceȡֵ��
*             �豸����
*          subdeviceȡֵ��
*             ���豸����
*          frameȡֵ��
*             can֡
*          canproȡֵ��
*             canpro�ṹ��
*����ֵ��  ��
*******************************************************************************************/
uint8_t V2CanProSendParam(uint16_t device, uint16_t subdevice, STR_Can_Frame* frame, CANPROHEAD* canpro)
{
	uint8_t res = 1;
	STR_V2CAN_Frame v2frame;
	uint16_t v2device;
	uint16_t len=0;
	
	v2device = deviceToV2type(device);
	switch(device)
	{
		case ID_ANGLE:	
		
			if(!getV2ProDeviceMsg(v2device,ANGLE_SET,canpro))
				return 0;
			
			v2frame.u32ID.ID.ACK = 0;
			v2frame.u32ID.ID.FrameType = canpro->frameid;
		  v2frame.u32ID.ID.RxID = v2device;
			v2frame.u32ID.ID.TxID = V2ID_SC;
			v2frame.u32ID.ID.Reservd = 0;
			v2frame.u32ID.ID.Sub = 0;
			v2frame.u32ID.ID.Sum = 0;
			v2frame.u32ID.ID.LiuShuiNumb = S_LiuShuiNumb;
			S_LiuShuiNumb++;
			S_LiuShuiNumb %= 0x0F;
			
			v2frame.u16DLC = 8;
			v2frame.u8DT[0] = v2_angle_para.forwardTimes;
			v2frame.u8DT[1] = v2_angle_para.workMode;
			v2frame.u8DT[2] = v2_angle_para.interval&0xFF;
			v2frame.u8DT[3] = (v2_angle_para.interval&0xFF00)>>8;
			v2frame.u8DT[4] = v2_angle_para.RxID;
			v2frame.u8DT[5] = v2_angle_para.angleVariation;
			v2frame.u8DT[6] = v2_angle_para.RxSubID ;
			v2frame.u8DT[7] = v2_angle_para.reserve;
			memcpy(frame,&v2frame,sizeof(STR_Can_Frame));
			
			break;
			
		case ID_HIGH:	
			if(!getV2ProDeviceMsg(v2device,ANGLE_SET,canpro))
				return 0;
			v2frame.u32ID.ID.ACK = 0;
			v2frame.u32ID.ID.FrameType = canpro->frameid;
			v2frame.u32ID.ID.RxID = v2device;
			v2frame.u32ID.ID.TxID = V2ID_SC;
			v2frame.u32ID.ID.Reservd = 0;
			v2frame.u32ID.ID.Sub = 0;
			v2frame.u32ID.ID.Sum = 0;
			v2frame.u32ID.ID.LiuShuiNumb = S_LiuShuiNumb;
			S_LiuShuiNumb++;
			S_LiuShuiNumb %= 0x0F;
			
			v2frame.u16DLC = 8;
			v2frame.u8DT[0] = v2_high_para.forwardTimes;
			v2frame.u8DT[1] = v2_high_para.workMode;
			v2frame.u8DT[2] = v2_high_para.interval&0xFF;
			v2frame.u8DT[3] = (v2_high_para.interval&0xFF00)>>8;
  		    v2frame.u8DT[4] = v2_high_para.RxID;
			v2frame.u8DT[5] = v2_high_para.angleVariation;
  	     	v2frame.u8DT[6] = v2_high_para.RxSubID;
			v2frame.u8DT[7] = v2_high_para.reserve;
			memcpy(frame,&v2frame,sizeof(STR_Can_Frame));
			
			break;
			
		case ID_PERSON:	
			if(!getV2ProDeviceMsg(v2device,PERSON_SET,canpro))
				return 0;
			v2frame.u32ID.ID.ACK = 0;
			v2frame.u32ID.ID.FrameType = canpro->frameid;			
		  v2frame.u32ID.ID.RxID = v2device;
			v2frame.u32ID.ID.TxID = V2ID_SC;
			v2frame.u32ID.ID.Reservd = 0;
			v2frame.u32ID.ID.Sub = 0;
			v2frame.u32ID.ID.Sum = 0;
			v2frame.u32ID.ID.LiuShuiNumb = S_LiuShuiNumb;
			S_LiuShuiNumb++;
			S_LiuShuiNumb %= 0x0F;
			
			
			v2frame.u16DLC = 8;
			v2frame.u8DT[0] = v2_person_para.scNum&0xFF;
			v2frame.u8DT[1] = (v2_person_para.scNum&0xFF00)>>8;
			v2frame.u8DT[2] = v2_person_para.u8WORKPARAM.u8workparam;
			v2frame.u8DT[3] = v2_person_para.interval;
  		v2frame.u8DT[4] = v2_person_para.range&0xFF;
			v2frame.u8DT[5] = (v2_person_para.range&0xFF00)>>8;
  		v2frame.u8DT[6] = v2_person_para.scHigh&0xFF;
			v2frame.u8DT[7] = (v2_person_para.scHigh&0xFF00)>>8;
			
			memcpy(frame,&v2frame,sizeof(STR_Can_Frame));
			break;
		default :
			res = 0;
			break;
	}
	return res;
}
/*******************************************************************************************
*����������V2���ճ�����̺������������
*��ڲ�����u16Wayȡֵ��
*             can֡���ķ���
*          frameȡֵ��
*             can֡
*����ֵ��  ��
*******************************************************************************************/
void V2CanRevProgramProc(uint16_t u16Way, STR_V2CXBCAN_Frame* frame)
{
	uint16_t FrameType;
	CANPROHEAD prohead;
	
	FrameType = frame->u32ID.ID.FrameType;
		
	if(getV2ProDeviceMsg(V2ID_CXB,FrameType,&prohead))
		prohead.rececallbacfun((void*)u16Way, (STR_Can_Frame*)frame);
}



/*******************************************************************************************
*����������V2�������ݹ��̺���
*��ڲ�����u16Wayȡֵ��
*             can֡���ķ���
*          frameȡֵ��
*             can֡
*����ֵ��  ��
*******************************************************************************************/
void V2CanRevProc(uint16_t u16Way, STR_V2CAN_Frame* frame)
{
	uint8_t TxId;
	uint16_t FrameType;
	CANPROHEAD prohead;	
	
	TxId = Get_V2TxID_Type(frame);
	if(TxId == 0x04)//�����
		FrameType = TxId;
	else
		FrameType = Get_V2Frame_Type(frame);
		
	if(getV2ProDeviceMsg(TxId,FrameType,&prohead))
		if(prohead.rececallbacfun != NULL)
			prohead.rececallbacfun((void*)u16Way, (STR_Can_Frame*)frame);
}

uint8_t getDeviceDeiver(uint8_t device)
{
	return NATIVECAN1;
}

/*******************************************************************************************
*����������V2���������Ƿ�ΪӦ������
*��ڲ�����frameȡֵ��
*             can֡
*����ֵ��  ��
*******************************************************************************************/
uint8_t  CanRecIsAck_V2(STR_Can_Frame* frame)
{
	uint8_t res = 0;
	STR_V2CAN_Frame v2frame;
	STR_V2CXBCAN_Frame cxbFrame;
	CANPROHEAD head;

	if(((frame->ID & V2CANPROFRAME_UPDATE) != V2CANPROFRAME_UPDATE) &&((frame->ID & V2CANPROACK) == V2CANPROACK))
	{
		memcpy(&v2frame, frame, sizeof(STR_V2CAN_Frame));
		if(getV2ProDeviceMsg(v2frame.u32ID.ID.TxID,v2frame.u32ID.ID.FrameType,&head))
		{
			if(head.ack == 1)
			{
				head.rececallbacfun(NULL,frame);
				res = 1;
			}
		}
	}		
	else if((frame->ID & V2CANPROFRAME_UPDATE) == V2CANPROFRAME_UPDATE)
	{
		if((frame->ID&0xFF) == 0x00)
		{
			memcpy(&cxbFrame, frame, sizeof(STR_V2CXBCAN_Frame));
			if(cxbFrame.u32ID.ID.FrameType == 0)
			{
				if(getV2ProDeviceMsg(V2ID_CXB, cxbFrame.u32ID.ID.FrameType, &head))
					head.rececallbacfun(NULL,frame);
				res = 1;
			}
		}
	}
	return res;
}


/***********************************************************
** ��������	 ActCmdTransPro()
** �䡡�룺  CommandWord								   
** �䡡����  ��
** �������������������µ����⶯��������ת��Ϊ��ŷ���������Ӧ����ڵ�������
** ������:	 ����
** �ա��ڣ�  2014.09.10
** �汾��    V1.1A
************************************************************/
uint32_t ActCmdTransPro(uint16_t driver,uint32_t CommandWord)
{
	stDeviceStatusMsg temp = {0};
	uint32_t res = 0XFF;
	GetCanDeviceStatusMsg(driver, ID_EMVD, EMVD_Version,&temp);
	if(temp.deviceVersion[0] == 2)
	{
		if (ActCmdTransTbl_V26[CommandWord - 1] != -1)
				res = 1 << ActCmdTransTbl_V26[CommandWord - 1];
	}
	else if(temp.deviceVersion[0] == 1 /*&& temp.deviceVersion[1] != 0*/)
	{
		if (ActCmdTransTbl_V22[CommandWord - 1] != -1)
				res = 1 << ActCmdTransTbl_V22[CommandWord - 1];
	}
	return res;
}

void V2Pro_cxb_prohead(uint16_t device, uint32_t point, uint8_t *data, uint8_t len,uint32_t packetNum, STR_Can_Frame* frame, CANBUSHEAD* canbus)
{
	uint8_t i;
  STR_V2CXBCAN_Frame cxbframe = {0};
	
  device = deviceToV2type(device);
	
	cxbframe.u32ID.ID.Reservd = 0x01;
	cxbframe.u32ID.ID.ACK = 0;
	cxbframe.u32ID.ID.FrameType = point;
	cxbframe.u32ID.ID.RxID = device;
	cxbframe.u32ID.ID.PacketNum = packetNum;
	cxbframe.u32ID.ID.NoUsed = 0;
	
	cxbframe.u16DLC = len;
	for(i = 0; i<len; i++)
	{
		cxbframe.u8DT[i] = data[i];	
	}
	memcpy(frame, &cxbframe, sizeof(STR_Can_Frame));
	
	canbus->ack = 0;
	canbus->backtime = 10;
	canbus->count = 2;
	canbus->overtime = 10;
	canbus->prio = 1;	
}



uint8_t V2Pro_action_prohead(uint16_t driver, uint16_t device, uint16_t subDev, uint32_t point, uint32_t status, uint8_t flag, STR_Can_Frame* frame, CANBUSHEAD* canbus)
{
	uint8_t res = INSERTADATA;
	uint16_t v2device;
	STR_V2CAN_Frame v2frame;
	
  STR_V2CXBCAN_Frame cxbframe;
	CANPROHEAD  canpro;
	uint8_t Tmp[9];
	uint32_t alarmMode = 0;
	uint8_t emvdDriver;
	
	v2device = deviceToV2type(device);
	
	switch(v2device)
	{		
		//�����
		case V2ID_CXB:
			
			u16CxbRecvQueuepWrite = 0;
			memset(u8ScRecProgramQueue,0,sizeof(u8ScRecProgramQueue));
		
			getV2ProDeviceMsg(v2device,point,&canpro);
			cxbframe.u32ID.ID.Reservd = 0x01;
			cxbframe.u32ID.ID.ACK = canpro.ack;
			cxbframe.u32ID.ID.FrameType = canpro.frameid;
			cxbframe.u32ID.ID.RxID = 4;
			cxbframe.u32ID.ID.PacketNum = 0;
			cxbframe.u32ID.ID.NoUsed = 0;
			
			cxbframe.u16DLC = 1;
			cxbframe.u8DT[0] = deviceToV2type_update(status);
			downloadDevice = cxbframe.u8DT[0];	
			memcpy(frame, &cxbframe, sizeof(STR_Can_Frame));
			break;
		//��ŷ�������			
		case V2ID_EMVD:
			
			if(point != 0xFFFF)
			{		
				if(GetDeviceCanMsg(ID_EMVD,0,&emvdDriver) == 0)
				{
					if(emvdAction != 0)
					{
						emvdAction = 0;
						res = DELETEDATA;
					}
					else
					{
						return NODEALDATA;
					}
				}
				else
				{
					point =	ActCmdTransPro(driver, point);//��ȡ��ŷ���
					if(point == 0XFF)
					{
						if(emvdAction != 0)
						{
							emvdAction = 0;
							res = DELETEDATA;
						}
						else
						{
							return NODEALDATA;
						}
					}
					else
					{
						if(status == EMVD_AMARM_ACTION)
						{
							if(emvdAction != 0)
								res = MODIFYDATA;
							else
							{
								res = INSERTADATA;
		//						testemvdtimer = 1;
		//						UdpLinkLayerSend_app(test111,3,1,0x0218 ,2,8);
							}
							
							emvdAction |= point;
						}
						else
						{
							if((emvdAction&point) == point)
								emvdAction = emvdAction - point;
							if(emvdAction != 0)
								res = MODIFYDATA;
							else
							{
								res = DELETEEMVDDATA;
							}
						}
					}
				}
			}
			else//ͣ���з�
			{
				emvdAction = 0;
				res = DELETEDATA;
			}
			
			if(flag == 1)//�����
			{
				getV2ProDeviceMsg(v2device,EMVD_CHECK_1,&canpro);		
				SetCanDeviceAPPMsg(driver, v2device, subDev, EMVDFLAG, 1);
				SetCanDeviceAPPMsg(driver, v2device, subDev, EMVDIO, emvdAction);
				SetCanDeviceAPPMsg(driver, v2device, subDev, EMVDSTATUS, 0);
			}
			else         //����
			{
				getV2ProDeviceMsg(v2device,EMVD_ACTION_1,&canpro);
			}

			v2frame.u32ID.ID.Reservd = 0;
			v2frame.u32ID.ID.ACK = canpro.ack;
			v2frame.u32ID.ID.FrameType = canpro.frameid;
			v2frame.u32ID.ID.Sub = 0;
			v2frame.u32ID.ID.Sum = 0;	
			v2frame.u32ID.ID.RxID = canpro.devicetype;
			v2frame.u32ID.ID.TxID = 0;
			v2frame.u32ID.ID.NoUsed = 0;
			v2frame.u32ID.ID.LiuShuiNumb = 0;//S_LiuShuiNumb;
			S_LiuShuiNumb++;
			S_LiuShuiNumb %= 0x0F;
			
			v2frame.u16DLC = 5;
			
			v2frame.u8DT[0] = (emvdAction &0xFF);
			v2frame.u8DT[1] = ((emvdAction &0xFF00)>>8);	
			v2frame.u8DT[2] = ((emvdAction &0xFF0000)>>16);
			v2frame.u8DT[3] = ((emvdAction &0xFF000000)>>24);
			
			Tmp[0] = v2frame.u32ID.u32Id & 0xff;
			Tmp[1] = (v2frame.u32ID.u32Id >> 8) & 0xff;
			Tmp[2] = (v2frame.u32ID.u32Id >> 16) & 0xff;
			Tmp[3] = (v2frame.u32ID.u32Id >> 24) & 0xff;
			Tmp[4] = v2frame.u16DLC & 0xff;
			Tmp[5] = v2frame.u8DT[0];
			Tmp[6] = v2frame.u8DT[1];
			Tmp[7] = v2frame.u8DT[2];
			Tmp[8] = v2frame.u8DT[3];
			
			v2frame.u8DT[4] = CRC_8(Tmp, 9);
			
			memcpy(frame, &v2frame, sizeof(STR_Can_Frame));
			
			break;
		//���ⱨ����	
		case V2ID_ALARM:
			if(point == 2)
			{
				GetCanDeviceAPPMsg(driver,device,0,ALARM_LED,&alarmMode);
				if(alarmMode == RED_ALWAYS)
					point = ALARM_WARN;
				else if(alarmMode == RED20_BLUE20)
					point = ALARM_ACTION;
				else
					point = ALARM_WARN;
				
				getV2ProDeviceMsg(v2device,point,&canpro);					
				if(status == 1)
				{
					if(alarmAction == ALARM_BEEP_LED)
					{	
						alarmAction = ALARM_BEEP_LED;
						res = NODEALDATA;
						return res;
					}
					else
					{
						alarmAction = ALARM_BEEP_LED;
						res = INSERTADATA;
					}
				}
				else
				{
					if(alarmAction == ALARM_BEEP_LED)
					{	
						alarmAction = ALARM_NO;
						res = DELETEDATA;
					}
					else
					{
						alarmAction = ALARM_NO;
						res = NODEALDATA;
						return res;
					}				
				}
				v2frame.u32ID.ID.Reservd = 0;
				v2frame.u32ID.ID.ACK = canpro.ack;
				v2frame.u32ID.ID.FrameType = canpro.frametype;
				v2frame.u32ID.ID.Sub = 0;
				v2frame.u32ID.ID.Sum = 0;	
				v2frame.u32ID.ID.RxID = canpro.devicetype;
				v2frame.u32ID.ID.TxID = 0;
				v2frame.u32ID.ID.NoUsed = 0;
				v2frame.u32ID.ID.LiuShuiNumb = 0;
				if(canpro.frametype == ALARM_WARN)
				{
					v2frame.u8DT[1] = 0x01;
					v2frame.u16DLC = 0x05;
					v2frame.u8DT[0] = 0x01;		//0x01;

					v2frame.u8DT[2] = (u8)((0x02030 >> 8) & 0xFF);
					v2frame.u8DT[3] = (u8)(0x02030 & 0xFF);

					Tmp[0] = v2frame.u32ID.u32Id & 0xff;
					Tmp[1] = (v2frame.u32ID.u32Id>>8) & 0xff;
					Tmp[2] = (v2frame.u32ID.u32Id>>16) & 0xff;
					Tmp[3] = (v2frame.u32ID.u32Id>>24) & 0xff;
					Tmp[4] = v2frame.u16DLC & 0xff;
					Tmp[5] = v2frame.u8DT[0];
					Tmp[6] = v2frame.u8DT[1];
					Tmp[7] = v2frame.u8DT[2];
					Tmp[8] = v2frame.u8DT[3];

					v2frame.u8DT[4] = CRC_8(Tmp, 9);
				}
				else if(canpro.frametype == ALARM_ACTION)
				{
					v2frame.u8DT[1] = 0x02;
					v2frame.u16DLC = 0x05;
					v2frame.u8DT[0] = 0x01;		//0x01;

					v2frame.u8DT[2] = (u8)((0x02030 >> 8) & 0xFF);
					v2frame.u8DT[3] = (u8)(0x02030 & 0xFF);

					Tmp[0] = v2frame.u32ID.u32Id & 0xff;
					Tmp[1] = (v2frame.u32ID.u32Id>>8) & 0xff;
					Tmp[2] = (v2frame.u32ID.u32Id>>16) & 0xff;
					Tmp[3] = (v2frame.u32ID.u32Id>>24) & 0xff;
					Tmp[4] = v2frame.u16DLC & 0xff;
					Tmp[5] = v2frame.u8DT[0];
					Tmp[6] = v2frame.u8DT[1];
					Tmp[7] = v2frame.u8DT[2];
					Tmp[8] = v2frame.u8DT[3];

					v2frame.u8DT[4] = CRC_8(Tmp, 9);
				}
				else
					res = DELETEDATA;
			}
			else
			{
				getV2ProDeviceMsg(v2device,point,&canpro);		
				if(canpro.frametype == ALARM_RESET)
				{
					v2frame.u32ID.ID.Reservd = 0;
					v2frame.u32ID.ID.ACK = canpro.ack;
					v2frame.u32ID.ID.FrameType = canpro.frametype;
					v2frame.u32ID.ID.Sub = 0;
					v2frame.u32ID.ID.Sum = 0;	
					v2frame.u32ID.ID.RxID = canpro.devicetype;
					v2frame.u32ID.ID.TxID = 0;
					v2frame.u32ID.ID.NoUsed = 0;
					v2frame.u32ID.ID.LiuShuiNumb = 0;
					S_LiuShuiNumb = 0;
	//				S_LiuShuiNumb %= 0x0F;
						
					if(device == ID_ALARM)
						v2frame.u8DT[0] = 0x30;
					else if(device == ID_PERSON)
						v2frame.u8DT[0] = 0x31;
					v2frame.u16DLC = 0x01;
				}
				else
				{
					res = NODEALDATA;
					return res;
				}
			}	
			memcpy(frame, &v2frame, sizeof(STR_Can_Frame));			
			break;
		default:
			res = DELETEDATA;
			break;
	}
	
	canbus->ack = canpro.ack;
	canbus->backtime = canpro.overtime;
	canbus->count = canpro.count;
	canbus->overtime = canbus->backtime;
	canbus->prio = 1;
	
	return res;
}


void CanFrameDeal_V2(uint16_t u16Way, STR_V2CAN_Frame* RevBufTemp)
{
	//V2��Χ�豸���³���	
	if((RevBufTemp->u32ID.u32Id&V2CANPROFRAME_UPDATE) != 0) //V2���³���
	{
		V2CanRevProgramProc(u16Way, (STR_V2CXBCAN_Frame*)RevBufTemp);	
	}
	//V2��������
	else
		V2CanRevProc(u16Way, RevBufTemp);	
}


