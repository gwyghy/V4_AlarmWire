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


//电磁阀驱动器功能接口最大数
#define EMVD_OUTNUMB_MAX			32	
/**动作命令字转换表，将按键按下功能键转换为对应电磁阀驱动器输出口的命令字。2018.08.16，电磁阀驱动器变更为26输出口。**/
/**对应的电磁阀驱动器程序版本号为V2.6.X。**/
/**2019年9月份，电磁阀驱动器变更为30功能输出口。程序版本号也为V2.6.X。**/
/**ActCmdTransTbl_V26适用于26功能~32功能**/
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

//动作命令字转换表，将按键按下功能键转换为对应电磁阀驱动器输出口的命令字。
//对应的电磁阀驱动器程序版本号为V1.0.x
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

//can发送流水号
uint8_t S_LiuShuiNumb = 0x00;

//电磁阀、声光报警器动作执行id
uint32_t emvdAction = 0;
uint8_t alarmAction = 0;

//人员定位、角度、高度运行参数
stPersonParam v2_person_para;
stAngleParam  v2_angle_para;
stAngleParam  v2_high_para;

//程序棒接收程序数组及写指针
static uint8_t  u8ScRecProgramQueue[CXB_RECV_QUEUE_MAX];
static uint16_t u16CxbRecvQueuepWrite = 0;


uint8_t downloadDevice;

/*******************************************************************************************
*功能描述：程序棒-心跳处理函数
*入口参数：pdata取值：
*             can帧来源（can1、can2）
*          frame取值：
*             can帧
*返回值：  无
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
*功能描述：程序棒-接收程序处理函数
*入口参数：pdata取值：
*             can帧来源（can1、can2）
*          frame取值：
*             can帧
*返回值：  无
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
*功能描述：程序棒-接收程序crc处理函数
*入口参数：pdata取值：
*             can帧来源（can1、can2）
*          frame取值：
*             can帧
*返回值：  无
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

	Crc16Ccitt(u8ScRecProgramQueue, u16CxbRecvQueuepWrite, &u16CrcTemp);//计算此时的校验值

	if(u16CrcTemp == u16Crc)
	{
		CanWriteProgramQueueProc(u8ScRecProgramQueue, u16CxbRecvQueuepWrite);
		memset(u8ScRecProgramQueue,0,CXB_RECV_QUEUE_MAX);
		//存数据		
		u16CxbRecvQueuepWrite = 0;
		//回应答
		isRight = 1;
		cxbframe.u8DT[6] = (u16CrcTemp & 0xFF);
		cxbframe.u8DT[7] = (u16CrcTemp & 0xFF00)>>8;
	}
	else
	{
		//清缓存
		memset(u8ScRecProgramQueue,0,CXB_RECV_QUEUE_MAX);
		u16CxbRecvQueuepWrite = 0;
		//发crc
		isRight = 0;
		cxbframe.u8DT[6] = (u16CrcTemp & 0xFF);
		cxbframe.u8DT[7] = (u16CrcTemp & 0xFF00)>>8;
	}
	
	//回应答
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
*功能描述：程序棒-接收程序应答函数
*入口参数：pdata取值：
*             can帧来源（can1、can2）
*          frame取值：
*             can帧
*返回值：  无
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
*功能描述：电磁阀-心跳函数
*入口参数：pdata取值：
*             can帧来源（can1、can2）
*          frame取值：
*             can帧
*返回值：  无
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
*功能描述：电磁阀-阀检测
*入口参数：pdata取值：
*             can帧来源（can1、can2）
*          frame取值：
*             can帧
*返回值：  无
*******************************************************************************************/
uint16_t Emvd_Check_Proc(void *pdata, STR_Can_Frame* frame)
{
	uint8_t res = 1;
	uint32_t emvdtmp1, emvdtmp;
	//存电磁阀检测结果
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
*功能描述：红外接收-心跳
*入口参数：pdata取值：
*             can帧来源（can1、can2）
*          frame取值：
*             can帧
*返回值：  无
*******************************************************************************************/
uint16_t Ir_Heart_Proc(void *pdata , STR_Can_Frame* frame)
{
	stDeviceStatusMsg deviceMsg = {0};
	
	if(frame->Data[0] != 'W')
	{
		//将frame指针转换为STR_WLCAN_Frame*类型，保持一致
		STR_WLCAN_Frame wl_frame;
		wl_frame.u32ID.u32Id = frame->ID;
		wl_frame.u16DLC = frame->DLC;
		memcpy(wl_frame.u8DT, frame->Data, sizeof(frame->Data));  
		InsAlarmWirelessIrRecvData_proc(&wl_frame,sizeof(wl_frame));	
	}		
	if(frame->Data[3] == '1')
	{		
	   //将frame指针转换为STR_WLCAN_Frame*类型，保持一致
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
*功能描述：红外接收-接收次数
*入口参数：pdata取值：
*             can帧来源（can1、can2）
*          frame取值：
*             can帧
*返回值：  无
*******************************************************************************************/
uint16_t Ir_Rx_Proc(void *pdata , STR_Can_Frame* frame)
{	
	uint8_t res = 1;
	uint32_t data;
	stDeviceStatusMsg deviceMsg = {0};
	
	//存值
	GetCanDeviceAPPMsg((uint16_t)pdata, ID_IRRX,0,0,&data);
	data++;
	data%=0xffff;
	SetCanDeviceAPPMsg((uint16_t)pdata, ID_IRRX,0,0,data);

	//清除心跳相关
	GetCanDeviceStatusMsg((uint16_t)pdata,ID_IRRX, IR_Verson, &deviceMsg);
	deviceMsg.HeartTimer = V2_IR_HEARTTIMER;
	SetCanDeviceStatusMsg((uint16_t)pdata,ID_IRRX, IR_Verson, &deviceMsg);	

	return res;	
}



/*******************************************************************************************
*功能描述：角度、高度-心跳
*入口参数：pdata取值：
*             can帧来源（can1、can2）
*          frame取值：
*             can帧
*返回值：  无
*******************************************************************************************/
uint16_t Angle_Heart_Proc(void *pdata , STR_Can_Frame* frame)
{
	uint8_t res = 1;
	uint8_t device;
	stDeviceStatusMsg deviceMsg = {0};
		
	device = frame->Data[3];
	
	if(device < V2_HIGH_HEART)//角度心跳
	{
		if(device == 0x3a) //多功能声光报警器上的倾角(心跳使用-0x3a)
		{
			GetCanDeviceStatusMsg((uint16_t)pdata, ID_ANGLE_ALARM,ALARMANGLE_Version,&deviceMsg);	
			deviceMsg.device = ID_ANGLE_ALARM;
			deviceMsg.subDevice = ALARMANGLE_Version;
			deviceMsg.HeartTimer = V2_ANGLE_HEARTTIMER;
			deviceMsg.isOnline = DEVICEONLINE;
			deviceMsg.backTimer = deviceMsg.HeartTimer;
		}
		else //普通倾角
		{
			device = device - V2_ANGLE_HEART;
			GetCanDeviceStatusMsg((uint16_t)pdata, ID_ANGLE, ANGLE0_Version + device, &deviceMsg);	
			deviceMsg.device = ID_ANGLE;
			deviceMsg.subDevice = ANGLE0_Version+device;
			deviceMsg.HeartTimer = V2_ANGLE_HEARTTIMER;
			deviceMsg.isOnline = DEVICEONLINE;
			deviceMsg.backTimer = deviceMsg.HeartTimer;
		}
		//设备初始化
		if(frame->Data[4] == DEVICEINIT)
		{
			CanProSendParam((uint16_t)pdata, ID_ANGLE,0);
		}
		deviceMsg.deviceVersion[0] = frame->Data[5];
		deviceMsg.deviceVersion[1] = frame->Data[6];
		deviceMsg.deviceVersion[2] = frame->Data[7];	
	}
	else if(device ==0x4a)//角高一体中的底座角度
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
	else if(device ==0x4b)//角高一体的顶梁角度
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
	else if(device  == V2_HIGH_HEART)//高度心跳
	{
		GetCanDeviceStatusMsg((uint16_t)pdata, ID_HIGH, HIGH_Verson, &deviceMsg);	
		deviceMsg.device = ID_HIGH;
		deviceMsg.subDevice = HIGH_Verson;
		deviceMsg.isOnline = DEVICEONLINE;
		deviceMsg.HeartTimer = V2_HIGH_HEARTTIMER;
		deviceMsg.backTimer = deviceMsg.HeartTimer;
		//设备初始化
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
*功能描述：角度±180切换
*入口参数：AngleValue取值：
*             角度值
*返回值：  角度值
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
*功能描述：角度、高度-上报数据
*入口参数：pdata取值：
*             can帧来源（can1、can2）
*          frame取值：
*             can帧
*返回值：  无
*******************************************************************************************/
uint16_t Angle_Report_Proc(void *pdata , STR_Can_Frame* frame)
{
	uint8_t res = 1;
	uint16_t data;
	stDeviceStatusMsg deviceMsg = {0};
	
	if(frame->Data[1] == 0 || frame->Data[1] == 2 || frame->Data[1] ==3 || frame->Data[1] ==4 ) //0:普通倾角 2:动态倾角 3:声光倾角 4:角高一体
	{	
		if(frame->Data[1] ==3)  //声光倾角
		{
			//存角度xy值
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
			//清除角度心跳相关
			GetCanDeviceStatusMsg((uint16_t)pdata,ID_ANGLE_ALARM, ALARMANGLE_Version, &deviceMsg);
			deviceMsg.HeartTimer = V2_ANGLE_HEARTTIMER;
			SetCanDeviceStatusMsg((uint16_t)pdata,ID_ANGLE_ALARM, deviceMsg.subDevice, &deviceMsg);
		}
		else if(frame->Data[1] ==4) //角高一体
		{			
			//存角度xy值
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
		else  //普通倾角
		{
						//存角度xy值
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
			//清除角度心跳相关
			GetCanDeviceStatusMsg((uint16_t)pdata,ID_ANGLE, frame->Data[0]+1, &deviceMsg);
			deviceMsg.HeartTimer = V2_ANGLE_HEARTTIMER;
			SetCanDeviceStatusMsg((uint16_t)pdata,ID_ANGLE, deviceMsg.subDevice, &deviceMsg);
		}
	}
	else
	{
		//存高度
		data = frame->Data[3];
		data = (data<<8)|(uint16_t)frame->Data[2];
		SetCanDeviceAPPMsg((uint16_t)pdata, ID_HIGH, frame->Data[0], 0, data);
		
		//清除高度心跳相关
		GetCanDeviceStatusMsg((uint16_t)pdata, ID_HIGH, HIGH_Verson,&deviceMsg);
		deviceMsg.HeartTimer = V2_HIGH_HEARTTIMER;
		SetCanDeviceStatusMsg((uint16_t)pdata, ID_HIGH, HIGH_Verson, &deviceMsg);	
	}
	return res;
}


/*******************************************************************************************
*功能描述：HUB-心跳函数
*入口参数：pdata取值：
*             can帧来源（can1、can2）
*          frame取值：
*             can帧
*返回值：  无
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
*功能描述：模拟量±180切换
*入口参数：AngleValue取值：
*             角度值
*返回值：  角度值
*******************************************************************************************/
u16 GetAdjustAnalogValue(u16 AnalogValue)
{
	AnalogValue = AnalogValue * 1000/819	;// x * 2 * 2500 / 4095	//实际电压被减半，参考电压2.5V	
	return (u16)AnalogValue;
}


/*******************************************************************************************
*功能描述：级联第一个HUB模拟量上报数据处理
*入口参数：pdata取值：
*             can帧来源（can1、can2）
*          frame取值：
*             can帧
*返回值：  无
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
*功能描述：级联第二个HUB模拟量上报数据处理
*入口参数：pdata取值：
*             can帧来源（can1、can2）
*          frame取值：
*             can帧
*返回值：  无
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
*功能描述：级联第三个HUB模拟量上报数据处理
*入口参数：pdata取值：
*             can帧来源（can1、can2）
*          frame取值：
*             can帧
*返回值：  无
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
*功能描述：级联第一个HUB上的角度X,Y轴角度上报数据处理
*入口参数：pdata取值：
*             can帧来源（can1、can2）
*          frame取值：
*             can帧
*返回值：  无
*******************************************************************************************/
uint16_t FirstHub_AngleXY_Proc(void *pdata , STR_Can_Frame* frame)
{
	uint16_t data;
	STR_V2CAN_Frame v2frame;
	stDeviceStatusMsg deviceMsg = {0};
	memcpy(&v2frame, frame, sizeof(STR_V2CAN_Frame));
	if(v2frame.u32ID.ID.Sub ==0)
	{
		//存角度xy值
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
		//存角度xy值
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
*功能描述：级联第二个HUB上的角度X,Y轴角度上报数据处理
*入口参数：pdata取值：
*             can帧来源（can1、can2）
*          frame取值：
*             can帧
*返回值：  无
*******************************************************************************************/
uint16_t SecondHub_AngleXY_Proc(void *pdata , STR_Can_Frame* frame)
{
	uint16_t data;
	STR_V2CAN_Frame v2frame;
	stDeviceStatusMsg deviceMsg = {0};
	memcpy(&v2frame, frame, sizeof(STR_V2CAN_Frame));
	if(v2frame.u32ID.ID.Sub ==0)
	{
		//存角度xy值
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
		//存角度xy值
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
*功能描述：级联第三个HUB上的角度X,Y轴角度上报数据处理
*入口参数：pdata取值：
*             can帧来源（can1、can2）
*          frame取值：
*             can帧
*返回值：  无
*******************************************************************************************/
uint16_t ThirdHub_AngleXY_Proc(void *pdata , STR_Can_Frame* frame)
{
	uint16_t data;
	STR_V2CAN_Frame v2frame;
	stDeviceStatusMsg deviceMsg = {0};
	memcpy(&v2frame, frame, sizeof(STR_V2CAN_Frame));
	if(v2frame.u32ID.ID.Sub ==0)
	{
		//存角度xy值
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
		//存角度xy值
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
*功能描述：级联第一个HUB上的角度Z轴角度上报数据处理
*入口参数：pdata取值：
*             can帧来源（can1、can2）
*          frame取值：
*             can帧
*返回值：  无
*******************************************************************************************/
uint16_t FirstHub_AngleZ_Proc(void *pdata , STR_Can_Frame* frame)
{
	uint16_t data;
	STR_V2CAN_Frame v2frame;
	memcpy(&v2frame, frame, sizeof(STR_V2CAN_Frame));
	//存角度xy值
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
*功能描述：级联第二个HUB上的角度Z轴角度上报数据处理
*入口参数：pdata取值：
*             can帧来源（can1、can2）
*          frame取值：
*             can帧
*返回值：  无
*******************************************************************************************/
uint16_t SecondHub_AngleZ_Proc(void *pdata , STR_Can_Frame* frame)
{
	uint16_t data;
	STR_V2CAN_Frame v2frame;
	memcpy(&v2frame, frame, sizeof(STR_V2CAN_Frame));
	//存角度xy值
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
*功能描述：级联第三个HUB上的角度Z轴角度上报数据处理
*入口参数：pdata取值：
*             can帧来源（can1、can2）
*          frame取值：
*             can帧
*返回值：  无
*******************************************************************************************/
uint16_t ThirdHub_AngleZ_Proc(void *pdata , STR_Can_Frame* frame)
{
	uint16_t data;
	STR_V2CAN_Frame v2frame;
	memcpy(&v2frame, frame, sizeof(STR_V2CAN_Frame));
	//存角度xy值
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
*功能描述：声光报警器-心跳
*入口参数：pdata取值：
*             can帧来源（can1、can2）
*          frame取值：
*             can帧
*返回值：  无
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
*功能描述：声光报警器-上报数据（人员）
*入口参数：pdata取值：
*             can帧来源（can1、can2）
*          frame取值：
*             can帧
*返回值：  无
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
	
	data = ((uint16_t)v2frame.u8DT[2])|((uint16_t)v2frame.u8DT[3]<<8);  // 编号
	
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
	/*电磁阀驱动器*/
	//心跳
	{
		1,       //设备类型
		0,       //帧类型
		0,       //映射号
		0,       //超时时间
		0,       //发送次数
		0,       //应答
		Emvd_Heart_Proc,    //接收回调函数
		NULL,               
		NULL,               
		NULL,               
	},
		//动作
	{
		1,       //设备类型
		1,       //帧类型
		1,       //映射号
		0,       //超时时间
		0,       //发送次数
		0,       //应答
		NULL,     
		NULL,     
		NULL,     
		NULL,     //发送前处理回调函数
	},
	//检测
	{
		1,       //设备类型
		2,       //帧类型
		2,       //映射号
		30,       //超时时间
		1,       //发送次数
		0,       //应答
		NULL,     
		NULL,     
		NULL,     
		NULL,     //发送前处理回调函数
	},
	//检测结果
	{
		1,       //设备类型
		3,       //帧类型
		3,       //映射号
		0,       //超时时间
		0,       //发送次数
		0,       //应答
		Emvd_Check_Proc,     //接收回调函数
		NULL,     
		NULL,     
		NULL,    
	},	
	//动作
	{
		1,       //设备类型
		4,       //帧类型
		0x11,       //映射号
		0,       //超时时间
		0,       //发送次数
		0,       //应答
		NULL,     
		NULL,     
		NULL,     
		NULL,     //发送前处理回调函数
	},
	//检测
	{
		1,       //设备类型
		5,       //帧类型
		0x12,       //映射号
		0,       //超时时间
		0,       //发送次数
		0,       //应答
		NULL,     
		NULL,     
		NULL,     
		NULL,     //发送前处理回调函数
	},
	//检测结果
	{
		1,       //设备类型
		6,       //帧类型
		0x13,    //映射号
		0,       //超时时间
		0,       //发送次数
		0,       //应答
		NULL,    //接收回调函数
		NULL,     
		NULL,     
		NULL,    
	},	
};
CANPROHEAD v2_hubinfo[] = 
{
	/*HUB汇接器*/
	//HUB心跳与角度心跳上报
	{
		2,       //设备类型
		0,       //映射号
		0,       //帧类型
		0,       //超时时间
		0,       //发送次数
		0,       //应答
		Hub_Heart_Proc,    //接收回调函数
		NULL,               
		NULL,               
		NULL,               
	},
	/*第一个HUB汇接器*/
	//模拟量数据上报
	{
		2,       //设备类型
		1,       //映射号
		2,    //帧类型
		0,       //超时时间
		0,       //发送次数
		0,       //应答
		FirstHub_Analog_Proc,    //接收回调函数
		NULL,               
		NULL,               
		NULL,               
	},
	/*第一个HUB汇接器*/
	//角度传感器X,Y轴数据上报
	{
		2,       //设备类型
		2,       //映射号
		4,    //帧类型
		0,       //超时时间
		0,       //发送次数
		0,       //应答
		FirstHub_AngleXY_Proc,    //接收回调函数
		NULL,               
		NULL,               
		NULL,               
	},
	/*第一个HUB汇接器*/
	//角度传感器Z轴数据上报
	{
		2,       //设备类型
		3,       //映射号
		24,    //帧类型
		0,       //超时时间
		0,       //发送次数
		0,       //应答
		FirstHub_AngleZ_Proc,    //接收回调函数
		NULL,               
		NULL,               
		NULL,               
	},
	/*第二个HUB汇接器*/
	//模拟量数据上报
	{
		2,       //设备类型
		4,       //映射号
		32,    	//帧类型
		0,       //超时时间
		0,       //发送次数
		0,       //应答
		SecondHub_Analog_Proc,    //接收回调函数
		NULL,               
		NULL,               
		NULL,               
	},
	/*第二个HUB汇接器*/
	//角度传感器X,Y轴数据上报
	{
		2,       //设备类型
		5,       //映射号
		34,    //帧类型
		0,       //超时时间
		0,       //发送次数
		0,       //应答
		SecondHub_AngleXY_Proc,    //接收回调函数
		NULL,               
		NULL,               
		NULL,               
	},
	/*第二个HUB汇接器*/
	//角度传感器Z轴数据上报
	{
		2,       //设备类型
		6,       //映射号
		54,    //帧类型
		0,       //超时时间
		0,       //发送次数
		0,       //应答
		SecondHub_AngleZ_Proc,    //接收回调函数
		NULL,               
		NULL,               
		NULL,               
	},
	/*第三个HUB汇接器*/
	//模拟量数据上报
	{
		2,       //设备类型
		7,       //映射号
		62,    //帧类型
		0,       //超时时间
		0,       //发送次数
		0,       //应答
		ThirdHub_Analog_Proc,    //接收回调函数
		NULL,               
		NULL,               
		NULL,               
	},
	/*第三个HUB汇接器*/
	//角度传感器X,Y轴数据上报
	{
		2,       //设备类型
		8,       //映射号
		64,    //帧类型
		0,       //超时时间
		0,       //发送次数
		0,       //应答
		ThirdHub_AngleXY_Proc,    //接收回调函数
		NULL,               
		NULL,               
		NULL,               
	},
	/*第三个HUB汇接器*/
	//角度传感器Z轴数据上报
	{
		2,       //设备类型
		9,       //映射号
		84,    	//帧类型
		0,       //超时时间
		0,       //发送次数
		0,       //应答
		ThirdHub_AngleZ_Proc,    //接收回调函数
		NULL,               
		NULL,               
		NULL,               
	},
	

};


CANPROHEAD v2_irrxinfo[] = 
{
	/*红外发射器*/
	//心跳
	{
		3,       //设备类型
		0,       //帧类型
		0,       //映射号
		0,       //超时时间
		0,       //发送次数
		0,       //应答
		Ir_Heart_Proc,    //接收回调函数
		NULL,               
		NULL,               
		NULL,               
	},
	//数据上报
	{
		3,       //设备类型
		1,       //帧类型
		1,       //映射号
		0,       //超时时间
		0,       //发送次数
		0,       //应答
		Ir_Rx_Proc,
		NULL,     
		NULL,     
		NULL,     //发送前处理回调函数
	},	
    //遥控器无线对码
	{
		3,       //设备类型
		6,       //帧类型
		6,       //映射号
		0,       //超时时间
		0,       //发送次数
		0,       //应答
		WirelessAlarm_CodeMatching_Proc,
		NULL,     
		NULL,     
		NULL,     //发送前处理回调函数
	},
	//遥控器无线对码应答
	{
		3,       //设备类型
		7,       //帧类型
		7,       //映射号
		0,       //超时时间
		0,       //发送次数
		0,       //应答
		NULL,
		NULL,     
		NULL,     
		NULL,     //发送前处理回调函数
	},
	//遥控器无线控制，控制码第一个按下请求
	{
		3,       //设备类型
		10,       //帧类型
		10,       //映射号
		0,       //超时时间
		0,       //发送次数
		0,       //应答
		WirelessAlarm_KeyProc,
		NULL,     
		NULL,     
		NULL,     //发送前处理回调函数
	},
	//遥控器无线控制，控制码第一个按下，接受与应答
	{
		3,       //设备类型
		11,       //帧类型
		11,       //映射号
		0,       //超时时间
		0,       //发送次数
		0,       //应答
		NULL,
		NULL,     
		NULL,     
		NULL,     //发送前处理回调函数
	},
	//遥控器无线控制，控制码连续按下请求
	{
		3,       //设备类型
		12,       //帧类型
		12,       //映射号
		0,       //超时时间
		0,       //发送次数
		0,       //应答
		NULL,
		NULL,     
		NULL,     
		NULL,     //发送前处理回调函数
	},
	//遥控器无线控制，控制码连续按下请求
	{
		3,       //设备类型
		13,       //帧类型
		13,       //映射号
		0,       //超时时间
		0,       //发送次数
		0,       //应答
		NULL,
		NULL,     
		NULL,     
		NULL,     //发送前处理回调函数
	},
	//遥控器无线控制，控制码连续按下请求
	{
		3,       //设备类型
		15,       //帧类型
		15,       //映射号
		0,       //超时时间
		0,       //发送次数
		0,       //应答
		WirelessAlarm_PassiveDecodePro,
		NULL,     
		NULL,     
		NULL,     //发送前处理回调函数
	},
	//遥控器无线控制，遥控器发送邻架控制请求
	{
		3,       //设备类型
		19,       //帧类型
		19,       //映射号
		0,       //超时时间
		0,       //发送次数
		0,       //应答
		WirelessAlarm_Neighbor_Request_Proc,
		NULL,     
		NULL,     
		NULL,     //发送前处理回调函数
	},
	//遥控器无线控制，遥控器发送邻架控制请求
	{
		3,       //设备类型
		18,       //帧类型
		18,       //映射号
		0,       //超时时间
		0,       //发送次数
		0,       //应答
		WirelessAlarm_Para_Request_Proc,
		NULL,     
		NULL,     
		NULL,     //发送前处理回调函数
	},
	
};



CANPROHEAD v2_angleinfo[] = 
{
	/*角度、高度传感器*/
	//心跳
	{
		5,       //设备类型
		0,       //帧类型
		0,       //映射号
		0,       //超时时间
		0,       //发送次数
		0,       //应答
		Angle_Heart_Proc,    //接收回调函数
		NULL,     
		NULL,     
		NULL,    
	},
	//角度上报
	{
		5,       //设备类型
		1,       //帧类型
		0x100,       //映射号
		0,       //超时时间
		0,       //发送次数
		0,       //应答
		Angle_Report_Proc,    //接收回调函数
		NULL,     
		NULL,     
		NULL,    
	},
	//角度查询
	{
		5,       //设备类型
		2,       //帧类型
		0x101,       //映射号
		0,       //超时时间
		0,       //发送次数
		0,       //应答
		NULL,    
		NULL,     
		NULL,     
		NULL,    //发送前处理回调函数
	},	
	//设置工作参数
	{
		5,       //设备类型
		3,       //帧类型
		0x105,       //映射号
		0,       //超时时间
		0,       //发送次数
		0,       //应答
		NULL,    
		NULL,     
		NULL,     
		NULL,    //发送前处理回调函数
	},
	//工作参数应答
	{
		5,       //设备类型
		4,       //帧类型
		0x105,       //映射号
		0,       //超时时间
		0,       //发送次数
		0,       //应答
		NULL,    
		NULL,     
		NULL,     
		NULL,    //发送前处理回调函数
	},
};

CANPROHEAD v2_alarminfo[] = 
{
	/*声光、人员传感器*/
	//心跳
	{
		7,       //设备类型
		0,       //帧类型
		0,       //映射号
		0,       //超时时间
		0,       //发送次数
		0,       //应答
		Alarm_Heart_Proc,     //接收回调函数
		NULL,     
		NULL,     
		NULL,    
	},
	//人员定位信息上报
	{
		7,       //设备类型
		1,       //帧类型
		1,       //映射号
		0,       //超时时间
		0,       //发送次数
		0,       //应答
		Alarm_Report_Proc,    //接收回调函数
		NULL,     
		NULL,     
		NULL,    
	},
	//查询人员定位信息
	{
		7,       //设备类型
		2,       //帧类型
		2,       //映射号
		0,       //超时时间
		0,       //发送次数
		0,       //应答
		NULL,     
		NULL,     
		NULL,     
		NULL,    //发送前处理回调函数
	},
	//设置工作参数
	{
		7,       //设备类型
		3,       //帧类型
		3,       //映射号
		0,       //超时时间
		0,       //发送次数
		0,       //应答
		NULL,     //接收回调函数
		NULL,     
		NULL,     
		NULL,    
	},
	//声光报警预警
	{
		7,       //设备类型
		4,       //帧类型
		4,       //映射号
		0,       //超时时间
		0,       //发送次数
		0,       //应答
		NULL,     
		NULL,     
		NULL,     
		NULL,    //发送前处理回调函数
	},
	//声光报警动作
	{
		7,       //设备类型
		5,       //帧类型
		5,       //映射号
		0,       //超时时间
		0,       //发送次数
		0,       //应答
		NULL,     //接收回调函数
		NULL,     
		NULL,     
		NULL,    
	},
	//复位
	{
		7,       //设备类型
		6,       //帧类型
		6,       //映射号
		30,       //超时时间
		3,       //发送次数
		0,       //应答
		NULL,     //接收回调函数
		NULL,     
		NULL,     
		NULL,    
	},
};


CANPROHEAD v2_cxbinfo[] = 
{
	/*程序棒*/
	//心跳
	{
		4,       //设备类型
		0,       //帧类型
		4,       //映射号
		0,       //超时时间
		0,       //发送次数
		0,       //应答
		CXB_Heart_Proc,     //接收回调函数
		NULL,     
		NULL,     
		NULL,    
	},
	//SC请求下载程序
	{
		4,       //设备类型
		1,       //帧类型
		0,       //映射号
		1000,    //超时时间
		3,       //发送次数
		1,       //应答
		CXB_Send_ACK_Proc,    //接收回调函数
		CXB_Send_NOACK_Proc,     
		NULL,     
		NULL,    
	},
	//CXB下载程序版本信息
	{
		4,       //设备类型
		3,       //帧类型
		1,       //映射号
		0,       //超时时间
		0,       //发送次数
		0,       //应答
		CXB_Program_Proc,     //接收回调函数
		NULL,     
		NULL,     
		NULL,    
	},
	//CXB发送版本信息校验包
	{
		4,       //设备类型
		4,       //帧类型
		6,       //映射号
		0,       //超时时间
		0,       //发送次数
		0,       //应答
		CXB_Program_Crc_Proc,     
		NULL,     
		NULL,     
		NULL,    //发送前处理回调函数
	},
	//SC发送版本信息校验包的应答
	{
		4,       //设备类型
		5,       //帧类型
		6,       //映射号
		0,       //超时时间
		0,       //发送次数
		0,       //应答
		NULL,     //接收回调函数
		NULL,     
		NULL,     
		NULL,    
	},
	//CXB发送程序代码
	{
		4,       //设备类型
		6,       //帧类型
		2,       //映射号
		0,       //超时时间
		0,       //发送次数
		0,       //应答
		CXB_Program_Proc,     //接收回调函数
		NULL,
		NULL,
		NULL,
	},
	//CXB发送代码校验包
	{
		4,       //设备类型
		7,       //帧类型
		7,       //映射号
		0,       //超时时间
		0,       //发送次数
		0,       //应答
		CXB_Program_Crc_Proc,     //接收回调函数
		NULL,
		NULL,
		NULL,
	},
};


/*******************************************************************************************
**函数名称：内联函数
**函数作用：
**函数参数：
**函数输出：无
**注意事项：无
** 作　者:	
** 日　期： 
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
*功能描述：角度、高度、人员的参数初始化
*入口参数：无
*返回值：  无
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
*功能描述：根据设备、帧id，执行对应的can接收回调查询
*入口参数：device取值：
*             设备名称
*          frame取值：
*             can帧
*          head取值：
*             can帧
*返回值：  无
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
*功能描述：V4设备类型转成V2的设备类型
*入口参数：device取值：
*             设备类型
*返回值：  无
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
*功能描述：V4程序下载类型，转V2程序下载类型
*入口参数：device取值：
*             设备类型
*返回值：  无
*******************************************************************************************/
uint8_t deviceToV2type_update(uint8_t device)
{
	uint8_t res;
	switch(device)
	{
		case GUI_TYPE_SC://"控制器"
			res = LOAD_TYPE_SC;
			break;
		case GUI_TYPE_EMVD://"电磁阀"
			res = LOAD_TYPE_EMVD;
			break;
		case GUI_TYPE_ANGLE://"角  度"
			res = LOAD_TYPE_ANGLE;
			break;
		case GUI_TYPE_PERSON://"人  员"
			res = LOAD_TYPE_PERSON;
			break;	
		case GUI_TYPE_ALARM://"声  光"
			res = LOAD_TYPE_ALARM;
			break;
		case GUI_TYPE_HIGH://"高  度"
			res = LOAD_TYPE_HIGH;
			break;
		case GUI_TYPE_IR://"红外RX"
			res = LOAD_TYPE_IR;
			break;
		case GUI_TYPE_KEYBOARD://"键盘"
			res = LOAD_TYPE_KEYBOARD;
			break;
		case GUI_TYPE_WIRELESS://"无线"
			res = LOAD_TYPE_WL;
			break;
		case GUI_TYPE_PERSONBL://"人员bl"
			res = LOAD_TYPE_PERSON_BL;
			break;
		case GUI_TYPE_ALARMBL://"声光bl"
			res = LOAD_TYPE_ALARM_BL;
			break;
		case GUI_TYPE_HIGHBL://"高度bl"
			res = LOAD_TYPE_HIGH_BL;
			break;
	    case GUI_TYPE_ALARM_UWB_ANGLE: //ghy "多功能声光报警器"
			res = LOAD_TYPE_ALARM_UWB_ANGLE;
			break;
		case GUI_TYPE_HUB: //ghy MiniHUB
			res = LOAD_TYPE_MINIHUB;
			break;
	}
	return res;
}


/*******************************************************************************************
*功能描述：V2协议接收超时
*入口参数：frame取值：
*             can帧
*返回值：  无
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
*功能描述：V2设置参数（角度、高度、人员）
*入口参数：device取值：
*             设备类型
*          param取值：
*             参数指针
*          isNow取值：
*             是否立即发送  1：立即发送
*返回值：  无
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
*功能描述：V2发送参数（角度、高度、人员）
*入口参数：device取值：
*             设备类型
*          subdevice取值：
*             子设备类型
*          frame取值：
*             can帧
*          canpro取值：
*             canpro结构体
*返回值：  无
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
*功能描述：V2接收程序过程函数（程序棒）
*入口参数：u16Way取值：
*             can帧来的方向
*          frame取值：
*             can帧
*返回值：  无
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
*功能描述：V2接收数据过程函数
*入口参数：u16Way取值：
*             can帧来的方向
*          frame取值：
*             can帧
*返回值：  无
*******************************************************************************************/
void V2CanRevProc(uint16_t u16Way, STR_V2CAN_Frame* frame)
{
	uint8_t TxId;
	uint16_t FrameType;
	CANPROHEAD prohead;	
	
	TxId = Get_V2TxID_Type(frame);
	if(TxId == 0x04)//程序棒
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
*功能描述：V2接收数据是否为应答数据
*入口参数：frame取值：
*             can帧
*返回值：  无
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
** 函数名：	 ActCmdTransPro()
** 输　入：  CommandWord								   
** 输　出：  无
** 功能描述：将按键按下的虚拟动作命令字转换为电磁阀驱动器对应输出口的命令字
** 作　者:	 沈万江
** 日　期：  2014.09.10
** 版本：    V1.1A
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
		//程序棒
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
		//电磁阀驱动器			
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
					point =	ActCmdTransPro(driver, point);//获取电磁阀点
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
			else//停所有阀
			{
				emvdAction = 0;
				res = DELETEDATA;
			}
			
			if(flag == 1)//阀检测
			{
				getV2ProDeviceMsg(v2device,EMVD_CHECK_1,&canpro);		
				SetCanDeviceAPPMsg(driver, v2device, subDev, EMVDFLAG, 1);
				SetCanDeviceAPPMsg(driver, v2device, subDev, EMVDIO, emvdAction);
				SetCanDeviceAPPMsg(driver, v2device, subDev, EMVDSTATUS, 0);
			}
			else         //动作
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
		//声光报警器	
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
	//V2外围设备更新程序	
	if((RevBufTemp->u32ID.u32Id&V2CANPROFRAME_UPDATE) != 0) //V2更新程序
	{
		V2CanRevProgramProc(u16Way, (STR_V2CXBCAN_Frame*)RevBufTemp);	
	}
	//V2其他数据
	else
		V2CanRevProc(u16Way, RevBufTemp);	
}


