/*
*********************************************************************************************************
*	                                            V4
*                           Commuication Controller For The Mining Industry
*
*                                  (c) Copyright 1994-2013  HNDZ 
*                       All rights reserved.  Protected by international copyright laws.
*
*
*    File    : Can_Pro.c
*    Module  : user
*    Version : V1.0
*    History :
*   -----------------
*              Version  Date           By            Note
*              v1.0         
*
*********************************************************************************************************
*/

#define   __CAN_BUS_C__

#include "includes.h"
#include "NativeCan_Driver.h"
#include "Can_Bus.h"
#include "Util_TImer.h"
#include "V2_Can_Pro.h"
#include "Log_Data_Mng.h"

#define deviceStatusMsgSize 2
#define deviceAPPMsgSize 2

//bus层已发送函数定时器
uint16_t BusCanSentTimerId;
//pro层循环发送定时器
uint16_t ProCanLoopTimerId;
//设备心跳定时器
uint16_t DeviceHeartTimerId;

//设备应用信息数组
stDeviceAppMsgQueue deviceAPPMsg[deviceAPPMsgSize];
//设备状态信息数组
stDeviceStatusMsgQueue deviceStatusmsg[deviceStatusMsgSize];

//can循环发送数组
stCanLoopSendQueue  CanLoopSendQueue;

/*******************************************************************************************
*功能描述：can循环发送数组初始化
*入口参数：无
*返回值：  无
*******************************************************************************************/
static void canLoopSendDataInit(void)
{
	CanLoopSendQueue.u16CanLoopSendCnt = 0;
	memset(&CanLoopSendQueue.CanLoopSend, 0 ,sizeof(CanLoopSendQueue.CanLoopSend));	
}


/*******************************************************************************************
*功能描述：插入can循环发送数组
*入口参数：CanLoopSend取值：
*            can循环发送结构体
*返回值：  无
*******************************************************************************************/
static void InsCanLoopSend(stCanLoopSend* CanLoopSend)
{
	uint8_t i = 0;
	#if OS_CRITICAL_METHOD == 3                      
    OS_CPU_SR  cpu_sr = 0;
	#endif
	
	if(CanLoopSendQueue.u16CanLoopSendCnt < CanLoopSendQueueNum)
	{
		OS_ENTER_CRITICAL();
		for(i = 1 ; i<CanLoopSendQueueNum; i++)
		{
			if(CanLoopSendQueue.CanLoopSend[i].backTimer == 0)
			{				
				memcpy(&(CanLoopSendQueue.CanLoopSend[i]),CanLoopSend,sizeof(stCanLoopSend));
				CanLoopSendQueue.u16CanLoopSendCnt++;
				break;
			}
		}
		OS_EXIT_CRITICAL();
	}
}

/*******************************************************************************************
*功能描述：修改can循环发送数组
*入口参数：CanLoopSend取值：
*            can循环发送结构体
*          CMD取值：
*             命令字
*返回值：  无
*******************************************************************************************/
static void ModifyCanLoopSend(stCanLoopSend* CanLoopSend, uint32_t CMD)
{
	uint8_t i = 0;
	#if OS_CRITICAL_METHOD == 3                      
    OS_CPU_SR  cpu_sr = 0;
	#endif
	
	if(CanLoopSendQueue.u16CanLoopSendCnt)
	{
		OS_ENTER_CRITICAL();		
		for(i = 0; i<CanLoopSendQueueNum; i++)
		{
			if((CanLoopSendQueue.CanLoopSend[i].SendFrame.ID != CMD))   //判断数据的点和数据位
				continue;
			memcpy(&(CanLoopSendQueue.CanLoopSend[i].SendFrame), &CanLoopSend->SendFrame, sizeof(STR_Can_Frame));
			break;
		}
		OS_EXIT_CRITICAL();
	}
}

/*******************************************************************************************
*功能描述：删除can循环发送数组
*入口参数：CMD取值：
*             命令字
*返回值：  无
*******************************************************************************************/
static void DelCanLoopSend(uint32_t CMD)
{
	uint8_t i = 0;
	#if OS_CRITICAL_METHOD == 3                      
		OS_CPU_SR  cpu_sr = 0;
	#endif

	if(CanLoopSendQueue.u16CanLoopSendCnt)
	{
		OS_ENTER_CRITICAL();
		for(i = 1; i<CanLoopSendQueueNum; i++)
		{
			if((CanLoopSendQueue.CanLoopSend[i].SendFrame.ID != CMD))   //判断数据的点和数据位
				continue;

			CanLoopSendQueue.u16CanLoopSendCnt --;
			memset(&(CanLoopSendQueue.CanLoopSend[i]),0,sizeof(stCanLoopSend));
//			break;
		}
		OS_EXIT_CRITICAL();
	}
}


/*******************************************************************************************
*功能描述：pro层can发送定时器回调函数
*入口参数：id取值：
*             回调函数id号
*返回值：  无
*******************************************************************************************/
void ProCanTimerSend_Callback(uint16_t id)
{
	uint8_t i;
	uint16_t device;
	stCanTrsQueue SentCan;
	
	#if OS_CRITICAL_METHOD == 3                      
		OS_CPU_SR  cpu_sr = 0;
	#endif

	if(CanLoopSendQueue.u16CanLoopSendCnt)
	{
		for(i = 1; i<CanLoopSendQueueNum; i++)
		{
			if(CanLoopSendQueue.CanLoopSend[i].backTimer != 0)
			{
				OS_ENTER_CRITICAL();		
				CanLoopSendQueue.CanLoopSend[i].timer --;
				OS_EXIT_CRITICAL();
				if(CanLoopSendQueue.CanLoopSend[i].timer == 0)
				{
					OS_ENTER_CRITICAL();
					if(CanLoopSendQueue.CanLoopSend[i].count != 0XFF && CanLoopSendQueue.CanLoopSend[i].count >0)
						CanLoopSendQueue.CanLoopSend[i].count--;
					
					CanLoopSendQueue.CanLoopSend[i].timer = CanLoopSendQueue.CanLoopSend[i].backTimer;
//					CanLoopSendQueue.CanLoopSend[i].SendFrame = (*CanLoopSendQueue.CanLoopSend[i].sendiffun)(&CanLoopSendQueue.CanLoopSend[i].SendFrame);
				  device = CanLoopSendQueue.CanLoopSend[i].device;
					memcpy(&(SentCan.busHead),&(CanLoopSendQueue.CanLoopSend[i].head),sizeof(CANBUSHEAD));
					memcpy(&(SentCan.TrsFrame),&(CanLoopSendQueue.CanLoopSend[i].SendFrame),sizeof(STR_Can_Frame));
					OS_EXIT_CRITICAL();
					CanBusDealFunc(device, CANBUS_MSG_InsCanHighTrsQueue,&SentCan);
					
					if(CanLoopSendQueue.CanLoopSend[i].count == 0)
					{
						memset(&CanLoopSendQueue.CanLoopSend[i],0,sizeof(CanLoopSendQueue.CanLoopSend[i]));
						if(CanLoopSendQueue.u16CanLoopSendCnt)
							CanLoopSendQueue.u16CanLoopSendCnt--;
					}
				}
			}
		}
	}
}

/*******************************************************************************************
*功能描述：bus层数据重发数据管理回调函数
*入口参数：id取值：
*             回调函数id号
*返回值：  无
*******************************************************************************************/
void BusCanTimerSent_Callback(uint16_t id)
{
	uint16_t u16Way;
	uint16_t tmp = 10;
	for(u16Way = 0; u16Way<CAN_DRIVER_DEV_NUM; u16Way++)
	{
		CanBusDealFunc(u16Way,CANBUS_MSG_CheckCanSentInterval,&tmp);
	}
}

//默认（电磁阀、声光、人员、红外接收）均在线
static uint8_t EMVD_VersionFlag = 1;
static uint8_t ALARM_VersionFlag = 1;
static uint8_t PERSON_VersionFlag = 1;
static uint8_t IR_VersonFlag = 1;
//ghy 添加H各个HUB的在线状态
static uint8_t HUB1_VersionFlag = 1;
static uint8_t HUB2_VersionFlag = 1;
static uint8_t HUB3_VersionFlag = 1;
/*******************************************************************************************
*功能描述：设备错误、正常写日志处理
*入口参数：id取值：
*            设备类型（电磁阀、声光、人员、红外接收）
*          status取值：
*             命令字
*返回值：  无
*******************************************************************************************/
void DeviceFaultProc(uint16_t id,uint8_t status)
{
	switch(id)
	{
		case EMVD_Version:
			if(EMVD_VersionFlag != status)
			{
				EMVD_VersionFlag = status;
				WriteFault_API(FAULT_HOST_DEVICE,ID_EMVD,FAULT_EVENT_OFFLINE,status);
			}
			break;
		case ALARM_Version:
			if(ALARM_VersionFlag != status)
			{
				ALARM_VersionFlag = status;
				WriteFault_API(FAULT_HOST_DEVICE,ID_ALARM,FAULT_EVENT_OFFLINE,status);
			}
			break;
		case PERSON_Version:
			if(PERSON_VersionFlag != status)
			{
				PERSON_VersionFlag = status;
				WriteFault_API(FAULT_HOST_DEVICE,ID_PERSON,FAULT_EVENT_OFFLINE,status);
			}
			break;
		case IR_Verson:
			if(IR_VersonFlag != status)
			{
				IR_VersonFlag = status;
				WriteFault_API(FAULT_HOST_DEVICE,ID_IRRX,FAULT_EVENT_OFFLINE,status);
			}
			break;
		case  HUB1_Version:  //ghy 添加HUB故障状态写入
			if(HUB1_VersionFlag != status)
			{
				HUB1_VersionFlag = status;
				WriteFault_API(FAULT_HOST_HUB1,ID_HUB1,FAULT_EVENT_OFFLINE,status);
			}
			break;
		case  HUB2_Version:
			if(HUB2_VersionFlag != status)
			{
				HUB2_VersionFlag = status;
				WriteFault_API(FAULT_HOST_HUB2,ID_HUB2,FAULT_EVENT_OFFLINE,status);
			}
			
			break;
		case  HUB3_Version:
			if(HUB3_VersionFlag != status)
			{
				HUB3_VersionFlag = status;
				WriteFault_API(FAULT_HOST_HUB3,ID_HUB3,FAULT_EVENT_OFFLINE,status);
			}
			break;
		
		
		default:
			break;
	}
}


/*******************************************************************************************
*功能描述：设备心跳超时定时器回调函数
*入口参数：id取值：
*            定时器id号
*返回值：  无
*******************************************************************************************/
void DeviceHeartTimerCallback(uint16_t id)
{
	uint16_t i,j;
	#if OS_CRITICAL_METHOD == 3                      
		OS_CPU_SR  cpu_sr = 0;
	#endif
	uint32_t emvdio = 0;
//	OS_ENTER_CRITICAL();
	for(j = 0; j < deviceStatusMsgSize; j++)
	{
		for(i = 0; i<DEVICE_MAX_Version; i++)
		{
			if(deviceStatusmsg[j].deviceStatusMsgQueue[i].backTimer != 0)
			{
				if(deviceStatusmsg[j].deviceStatusMsgQueue[i].deviceStatus == DEVICEPROGRAM)
					continue;
				if(deviceStatusmsg[j].deviceStatusMsgQueue[i].HeartTimer > 1)
					deviceStatusmsg[j].deviceStatusMsgQueue[i].HeartTimer --;
				else 
					deviceStatusmsg[j].deviceStatusMsgQueue[i].HeartTimer = 0;
							
				if(deviceStatusmsg[j].deviceStatusMsgQueue[i].HeartTimer == 0)
				{
					deviceStatusmsg[j].deviceStatusMsgQueue[i].isOnline = 0;
					memset(deviceStatusmsg[j].deviceStatusMsgQueue[i].deviceBLVersion,0,3);
					memset(deviceStatusmsg[j].deviceStatusMsgQueue[i].deviceVersion,0,3);
				}
				else
				{			
					if(i == EMVD_Version || i == ALARM_Version|| i == PERSON_Version || i == IR_Verson \
						|| i == HUB1_Version || i ==HUB2_Version||i == HUB3_Version)
					{
						DeviceFaultProc(i,FAULT_STATUS_NORMAL);
					}
				}
			}
			//ghy 参数未使能时清除故障信息
			if(deviceStatusmsg[j].deviceStatusMsgQueue[i].isEnable == 0)
			{
				if(i == EMVD_Version || i == ALARM_Version|| i == PERSON_Version || i == IR_Verson \
					|| i == HUB1_Version || i ==HUB2_Version||i == HUB3_Version)
				{
					DeviceFaultProc(i,FAULT_STATUS_NORMAL);
				}
			}
		}
	}
	if( deviceStatusmsg[0].deviceStatusMsgQueue[EMVD_Version].isOnline == 0)
	{
		if(deviceStatusmsg[1].deviceStatusMsgQueue[EMVD_Version].isOnline == 0){
			DeviceFaultProc(EMVD_Version,FAULT_STATUS_FAULT);
			GetCanDeviceAPPMsg(1,ID_EMVD,0,EMVDIO,&emvdio);
			if(emvdio != 0)
			{
				canLoopSendDataInit();
			}
		}
	}
	if(deviceStatusmsg[0].deviceStatusMsgQueue[ALARM_Version].isEnable == 1&& deviceStatusmsg[0].deviceStatusMsgQueue[ALARM_Version].isOnline == 0)
	{
		if(deviceStatusmsg[1].deviceStatusMsgQueue[ALARM_Version].isEnable == 1&& deviceStatusmsg[1].deviceStatusMsgQueue[ALARM_Version].isOnline == 0)
			DeviceFaultProc(ALARM_Version,FAULT_STATUS_FAULT);
	}
	if(deviceStatusmsg[0].deviceStatusMsgQueue[PERSON_Version].isEnable == 1&& deviceStatusmsg[0].deviceStatusMsgQueue[PERSON_Version].isOnline == 0)
	{
		if(deviceStatusmsg[1].deviceStatusMsgQueue[PERSON_Version].isEnable == 1&& deviceStatusmsg[1].deviceStatusMsgQueue[PERSON_Version].isOnline == 0)
			DeviceFaultProc(PERSON_Version,FAULT_STATUS_FAULT);		
	}
	if(deviceStatusmsg[0].deviceStatusMsgQueue[IR_Verson].isEnable == 1&& deviceStatusmsg[0].deviceStatusMsgQueue[IR_Verson].isOnline == 0)
	{
		if(deviceStatusmsg[1].deviceStatusMsgQueue[IR_Verson].isEnable == 1&& deviceStatusmsg[1].deviceStatusMsgQueue[IR_Verson].isOnline == 0)
			DeviceFaultProc(IR_Verson,FAULT_STATUS_FAULT);		
	}
	//ghy 添加HUB故障信息提示
	if(deviceStatusmsg[0].deviceStatusMsgQueue[HUB1_Version].isEnable == 1&& deviceStatusmsg[0].deviceStatusMsgQueue[HUB1_Version].isOnline == 0)
	{
		if(deviceStatusmsg[1].deviceStatusMsgQueue[HUB1_Version].isEnable == 1&& deviceStatusmsg[1].deviceStatusMsgQueue[HUB1_Version].isOnline == 0)
			DeviceFaultProc(HUB1_Version,FAULT_STATUS_FAULT);		
	}
	if(deviceStatusmsg[0].deviceStatusMsgQueue[HUB2_Version].isEnable == 1&& deviceStatusmsg[0].deviceStatusMsgQueue[HUB2_Version].isOnline == 0)
	{
		if(deviceStatusmsg[1].deviceStatusMsgQueue[HUB2_Version].isEnable == 1&& deviceStatusmsg[1].deviceStatusMsgQueue[HUB2_Version].isOnline == 0)
			DeviceFaultProc(HUB2_Version,FAULT_STATUS_FAULT);		
	}
	if(deviceStatusmsg[0].deviceStatusMsgQueue[HUB3_Version].isEnable == 1&& deviceStatusmsg[0].deviceStatusMsgQueue[HUB3_Version].isOnline == 0)
	{
		if(deviceStatusmsg[1].deviceStatusMsgQueue[HUB3_Version].isEnable == 1&& deviceStatusmsg[1].deviceStatusMsgQueue[HUB3_Version].isOnline == 0)
			DeviceFaultProc(HUB3_Version,FAULT_STATUS_FAULT);		
	}
	
//	OS_EXIT_CRITICAL();
	for(i=0;i<2;i++)
	{
		for(j=0;j<11;j++)//ghy 若3s内收不到人员离开的信息，则把该人员标签的信息清除掉
		{
			deviceAPPMsg[i].deviceAppMsg.personappmsg[j].person = 0;
				continue;
			if(deviceAPPMsg[i].deviceAppMsg.personappmsg[j].personTimeout >0)
				deviceAPPMsg[i].deviceAppMsg.personappmsg[j].personTimeout --;
			if(deviceAPPMsg[i].deviceAppMsg.personappmsg[j].personTimeout ==0)
			{
				deviceAPPMsg[i].deviceAppMsg.personappmsg[j].personDis = 0;
				deviceAPPMsg[i].deviceAppMsg.personappmsg[j].person = 0;
				deviceAPPMsg[i].deviceAppMsg.personappmsg[j].personBattry =0;
				deviceAPPMsg[i].deviceAppMsg.personappmsg[j].personPer =0;
				deviceAPPMsg[i].deviceAppMsg.personappmsg[j].personNum = 0;
			}
		}
	}

}


//判断can接收是否为应答帧
uint8_t CanRecIsAck(STR_Can_Frame * frame)
{
	uint8_t res = 0;
	if((frame->ID & V4CANPROFRAME) != 0) //V4外围设备
	{
		;
	}
	else  //V2外围设备
	{
	  res = CanRecIsAck_V2(frame);
	}
	return res;
}

/*******************************************************************************************
*功能描述：can的pro层初始化
*入口参数：device取值：
*            can设备的数组头指针
*          bunds取值：
*             can设备的波特率数组头指针
*          num取值：
*             can设备的个数
*返回值：  无
*******************************************************************************************/
void CanProInit(uint16_t* device, uint16_t* bunds,uint8_t num)
{
	uint8_t i;
	//bus层初始化can
	for(i = 0; i<num; i++)
	{
		CanBusInit(device[i],bunds[i]);
	}
	//角度人员传感器初始化参数
	v2AngleandPersonInit();
	//外围设备应用和状态数据初始化
	memset(&deviceStatusmsg,0,sizeof(stDeviceStatusMsgQueue)*2);
	memset(&deviceAPPMsg,0,sizeof(deviceAPPMsg)*2);

	//can的pro层循环发送数组初始化
	canLoopSendDataInit();
	
	//bus层已发送函数定时器初始化
	BusCanSentTimerId = addTimerEvent(1,0XFF,BusCanTimerSent_Callback);//1ms
	startTimerEvent(BusCanSentTimerId);

	//pro层循环发送函数定时器初始化	
	ProCanLoopTimerId = addTimerEvent(1,0XFF,ProCanTimerSend_Callback);//1ms
	startTimerEvent(ProCanLoopTimerId);	

	//设备心跳函数定时器初始化		
	DeviceHeartTimerId = addTimerEvent(100,0XFF,DeviceHeartTimerCallback);//100ms
	startTimerEvent(DeviceHeartTimerId);

}

uint8_t getTypeDevice(uint16_t device)
{
	return V2TYPEDEVICE;
}


/*******************************************************************************************
*功能描述：can的pro层发送函数（程序棒）
*入口参数：driver取值：
*            驱动号
*          device取值：
*             设备号
*          subDev取值：
*             子设备号
*          point取值：
*             点号
*          data取值：
*             数据（程序棒的设备类型）
*          flag取值：
*             flag为阀检测专用
*返回值：  无
*******************************************************************************************/
void CanProSend(uint16_t driver, uint16_t device, uint16_t subDev, uint16_t point, uint8_t data, uint8_t flag)
{
	stCanTrsQueue canTrsQueue;
	//获取V4设备，V2设备查询不同的数组
	if(getTypeDevice(device) == V2TYPEDEVICE) //V2
	{
		//根据 device childDev point data查询pro的prohead
		if(!V2Pro_action_prohead(driver,device, subDev, point, data, flag, &(canTrsQueue.TrsFrame), &canTrsQueue.busHead))
		{
			return;
		}
	}
	else if(getTypeDevice(device) == V4TYPEDEVICE)  //V4
	{	
		//根据 device childDev point data查询pro的prohead	
	}
	else
	{
		return;
	}	
	
	
	//放入队列发送
	if(canTrsQueue.busHead.prio)
		CanBusDealFunc(driver, CANBUS_MSG_InsCanHighTrsQueue, &canTrsQueue);
	else
		CanBusDealFunc(driver, CANBUS_MSG_InsCanLowTrsQueue, &canTrsQueue);
}

void CanProDirectSend(uint16_t driver, uint16_t device, uint16_t subDev, uint16_t point, uint8_t* data, uint8_t len, uint32_t packetNum)
{
	stCanTrsQueue canTrsQueue;
	//获取V4设备，V2设备查询不同的数组
	if(getTypeDevice(device) == V2TYPEDEVICE) //V2
	{
		V2Pro_cxb_prohead(device, point, data, len, packetNum, &(canTrsQueue.TrsFrame), &canTrsQueue.busHead);
	}
	else if(getTypeDevice(device) == V4TYPEDEVICE)  //V4
	{	
		//根据 device childDev point data查询pro的prohead
	}
	else
	{
		return;
	}	
	
	
	//放入队列发送
	if(canTrsQueue.busHead.prio)
		CanBusDealFunc(driver, CANBUS_MSG_InsCanHighTrsQueue, &canTrsQueue);
	else
		CanBusDealFunc(driver, CANBUS_MSG_InsCanLowTrsQueue, &canTrsQueue);
}


/*******************************************************************************************
*功能描述：can的pro层循环发送函数（电磁阀、声光）
*入口参数：driver取值：
*            驱动号
*          device取值：
*             设备号
*          subDev取值：
*             子设备号
*          point取值：
*             点号
*          status取值：
*             数据（电磁阀的启动\停止、声光报警类型）
*          interval取值：
*             循环发送时间间隔
*          count取值：
*             循环发送次数
*          sendiffun取值：
*             循环发送回调函数
*返回值：  无
*******************************************************************************************/
void CanProLoopSend(uint16_t driver,uint16_t device, uint16_t subDev, uint16_t point, uint16_t status, uint8_t interval, uint8_t count, LoopSendIfFun sendiffun)
{
	uint8_t res;
	stCanLoopSend CanLoopSend;
	uint32_t cmd;
	
	//获取V4设备，V2设备查询不同的数组
	if(getTypeDevice(device) == V2TYPEDEVICE) //V2
	{
		res = V2Pro_action_prohead(driver, device, subDev, point, status, 0, &(CanLoopSend.SendFrame), &CanLoopSend.head);
		//根据 device childDev point data查询pro的prohead
		if(res == DELETEDATA)
		{
			cmd = CanLoopSend.SendFrame.ID;
			DelCanLoopSend(cmd);
			return;
		}
		else if(res == MODIFYDATA)
		{
			cmd = CanLoopSend.SendFrame.ID;
			ModifyCanLoopSend(&CanLoopSend, cmd);
			return;
		}
		else if(res == DELETEEMVDDATA)
		{
			cmd = CanLoopSend.SendFrame.ID;
			DelCanLoopSend(cmd);
			
			CanLoopSend.backTimer = interval;
			CanLoopSend.timer = 1;
			CanLoopSend.count = 1;
			CanLoopSend.device = driver;
			CanLoopSend.sendiffun = sendiffun;
			
			InsCanLoopSend(&CanLoopSend);	
			return;
		}
		else if(res == NODEALDATA)
			return;
	}
	else if(getTypeDevice(device) == V4TYPEDEVICE)  //V4
	{	
		//根据 device childDev point data查询pro的prohead	
	}
	else
	{
		return;
	}
	
	CanLoopSend.backTimer = interval;
	CanLoopSend.timer = 1;
	CanLoopSend.count = count;
	CanLoopSend.device = driver;
	CanLoopSend.sendiffun = sendiffun;
	
	InsCanLoopSend(&CanLoopSend);
}


/*******************************************************************************************
*功能描述：can的pro层设置外围设备参数（人员定位参数配置提供外部接口）
*入口参数：device取值：
*             设备号
*          param取值：
*             参数指针
*          isNow取值：
*             是否立即发送
*返回值：  无
*******************************************************************************************/
void CanProSetParam(uint16_t device, void * param,uint8_t isNow)
{
	V2CanProSetParam(device,param,isNow);
}


/*******************************************************************************************
*功能描述：can的pro层设置外围设备参数（角度、高度参数配置提供外部接口）
*入口参数：driver取值：
*            驱动号
*          device取值：
*             设备号
*          subdevice取值：
*             子设备号
*返回值：  无
*******************************************************************************************/
void CanProSendParam(uint16_t driver, uint16_t device, uint16_t subdevice)
{
	CANPROHEAD canprohead;
	uint8_t deiverTmp = 0xff;
	stCanTrsQueue canTrsQueue;
	if(getTypeDevice(device) == V2TYPEDEVICE) //V2
	{
		 if(!V2CanProSendParam(device, subdevice, &canTrsQueue.TrsFrame, &canprohead))
			 return;
	}
	
	canTrsQueue.busHead.ack = 0;
	canTrsQueue.busHead.backtime = 0;
	canTrsQueue.busHead.count = 0;
	canTrsQueue.busHead.overtime = 0;
	canTrsQueue.busHead.prio = 1;
	canTrsQueue.busHead.overtime = 0;
	
	//放入队列发送
	if(canTrsQueue.busHead.prio)
		CanBusDealFunc(driver, CANBUS_MSG_InsCanHighTrsQueue, &canTrsQueue);
}

/*******************************************************************************************
*功能描述：can的pro层获取设备应用数据函数（对外、对内）
*入口参数：driver取值：
*            驱动号
*          device取值：
*             设备号
*          subDev取值：
*             子设备号
*          point取值：
*             点号
*          data取值：
*             数据指针
*返回值：  无
*******************************************************************************************/
void GetCanDeviceAPPMsg(uint16_t driver, uint16_t device, uint16_t subDev, uint16_t point, uint32_t* data)
{ 
	switch (device)
	{		
		case ID_EMVD:
			switch(point)
			{
				case EMVDIO:
					*data = deviceAPPMsg[driver].deviceAppMsg.emvdappmsg[0].iovalue;
					break;
				case EMVDSTATUS:
					*data = deviceAPPMsg[driver].deviceAppMsg.emvdappmsg[0].status;
					break;
				case EMVDFLAG:
					*data = deviceAPPMsg[driver].deviceAppMsg.emvdappmsg[0].flag;
					break;
			}			
			break;
		case ID_ANGLE:
			switch(point)
			{
				case ANGLE_X:
				*data = deviceAPPMsg[driver].deviceAppMsg.angleappmsg[subDev].angle_x;
					break;      
				case ANGLE_Y: 
				*data = deviceAPPMsg[driver].deviceAppMsg.angleappmsg[subDev].angle_y;
					break;      
				case ANGLE_Z: 
				*data = deviceAPPMsg[driver].deviceAppMsg.angleappmsg[subDev].angle_z;
					break;
			}
			break;
		case ID_HIGH:
			*data = deviceAPPMsg[driver].deviceAppMsg.hgihappmsg[subDev].high;
			break;
		case ID_PERSON:
			switch(point)
			{
				case PERSONMSG:
					*data = deviceAPPMsg[driver].deviceAppMsg.personappmsg[subDev].person;
					*data |= (uint16_t)deviceAPPMsg[driver].deviceAppMsg.personappmsg[subDev].personPer<<8;
					break;
				case PERSONNUM:
					*data = deviceAPPMsg[driver].deviceAppMsg.personappmsg[subDev].personNum;
					break;
				case PERSONDIS:
					*data = deviceAPPMsg[driver].deviceAppMsg.personappmsg[subDev].personDis;
					break;
				case PERSONBATTRY:
					*data = deviceAPPMsg[driver].deviceAppMsg.personappmsg[subDev].personBattry ;
					break;
				
			}
			break;
		case ID_IRRX:
			*data = deviceAPPMsg[driver].deviceAppMsg.irappmsg[subDev].irCounts;
			break;
		case ID_ALARM:
			*data = deviceAPPMsg[driver].deviceAppMsg.alarmappmsg[0].alarmMode[point];
			break;
		case ID_ANGLE_ALARM:
			switch(point)
			{
				case ANGLE_X:
					*data = deviceAPPMsg[driver].deviceAppMsg.anglealarmappMsg[0].angle_x ;
				break;
				case ANGLE_Y:
					*data =deviceAPPMsg[driver].deviceAppMsg.anglealarmappMsg[0].angle_y;
				break;
				case ANGLE_Z:
					*data = deviceAPPMsg[driver].deviceAppMsg.anglealarmappMsg[0].angle_z;
				break;
			}
			break;
		case ID_ANGLE_HIGH:
			switch(point)
			{
				case ANGLE_X:
					*data = deviceAPPMsg[driver].deviceAppMsg.highangleappMsg[subDev].angle_x ;
				break;
				case ANGLE_Y:
					*data =deviceAPPMsg[driver].deviceAppMsg.highangleappMsg[subDev].angle_y;
				break;
				case ANGLE_Z:
					*data = deviceAPPMsg[driver].deviceAppMsg.highangleappMsg[subDev].angle_z;
				break;
			}
			break;	
		case ID_HUB1:
			if(subDev == 0)
			{
				switch(point)
				{
					case  ANALOG_1:
						*data = deviceAPPMsg[driver].deviceAppMsg.minihubappmsg[0].analog1;
						break;
					case  ANALOG_2:
						*data = deviceAPPMsg[driver].deviceAppMsg.minihubappmsg[0].analog2;
						break;
					case  ANALOG_3:
						*data = deviceAPPMsg[driver].deviceAppMsg.minihubappmsg[0].analog3;
						break;
					case  ANALOG_4:
						*data = deviceAPPMsg[driver].deviceAppMsg.minihubappmsg[0].analog4;
						break;
				}
			}
			else
			{
				switch(point)
				{
					case ANGLE_X:
						*data = deviceAPPMsg[driver].deviceAppMsg.minihubappmsg[0].hubangle[subDev-1].angle_x;
						break;
					case ANGLE_Y:
						*data = deviceAPPMsg[driver].deviceAppMsg.minihubappmsg[0].hubangle[subDev -1].angle_y;
						break;
					case ANGLE_Z:
						*data = deviceAPPMsg[driver].deviceAppMsg.minihubappmsg[0].hubangle[subDev -1].angle_z;
						break;
				}
			}
		break;			
		case ID_HUB2:
			if(subDev == 0)
			{
				switch(point)
				{
					case  ANALOG_1:
					*data = deviceAPPMsg[driver].deviceAppMsg.minihubappmsg[1].analog1;
						break;
					case  ANALOG_2:
					*data = deviceAPPMsg[driver].deviceAppMsg.minihubappmsg[1].analog2;
						break;
					case  ANALOG_3:
					*data = deviceAPPMsg[driver].deviceAppMsg.minihubappmsg[1].analog3;
						break;
					case  ANALOG_4:
					*data = deviceAPPMsg[driver].deviceAppMsg.minihubappmsg[1].analog4;
						break;
				}
			}
			else
			{
				switch(point)
				{
					case ANGLE_X:
					*data = deviceAPPMsg[driver].deviceAppMsg.minihubappmsg[1].hubangle[subDev -1].angle_x;
						break;
					case ANGLE_Y:
					*data =	deviceAPPMsg[driver].deviceAppMsg.minihubappmsg[1].hubangle[subDev -1].angle_y;
						break;
					case ANGLE_Z:
					*data =	deviceAPPMsg[driver].deviceAppMsg.minihubappmsg[1].hubangle[subDev -1].angle_z;
						break;
				}
			}
		break;
		case ID_HUB3:
			if(subDev == 0)
			{
				switch(point)
				{
					case  ANALOG_1:
					*data =deviceAPPMsg[driver].deviceAppMsg.minihubappmsg[2].analog1;
						break;
					case  ANALOG_2:
					*data =	deviceAPPMsg[driver].deviceAppMsg.minihubappmsg[2].analog2;
						break;
					case  ANALOG_3:
					*data =	deviceAPPMsg[driver].deviceAppMsg.minihubappmsg[2].analog3;
						break;
					case  ANALOG_4:
					*data =	deviceAPPMsg[driver].deviceAppMsg.minihubappmsg[2].analog4;
						break;
				}
			}
			else
			{
				switch(point)
				{
					case ANGLE_X:
					*data =	deviceAPPMsg[driver].deviceAppMsg.minihubappmsg[2].hubangle[subDev -1].angle_x;
						break;
					case ANGLE_Y:
					*data =	deviceAPPMsg[driver].deviceAppMsg.minihubappmsg[2].hubangle[subDev -1].angle_y;
						break;
					case ANGLE_Z:
					*data =	deviceAPPMsg[driver].deviceAppMsg.minihubappmsg[2].hubangle[subDev -1].angle_z;
						break;
				}
			}
		break;	
	}
}


/*******************************************************************************************
*功能描述：can的pro层设置设备应用数据函数（对外、对内）
*入口参数：driver取值：
*            驱动号
*          device取值：
*             设备号
*          subDev取值：
*             子设备号
*          point取值：
*             点号
*          data取值：
*             数据指针
*返回值：  无
*******************************************************************************************/
void SetCanDeviceAPPMsg(uint16_t driver, uint16_t device, uint16_t subDev, uint16_t point, uint32_t data)
{
	#if OS_CRITICAL_METHOD == 3                      
		OS_CPU_SR  cpu_sr = 0;
	#endif
//	OS_ENTER_CRITICAL();
	switch (device)
	{
		case ID_EMVD:
			switch(point)
			{
				case EMVDIO:
					deviceAPPMsg[driver].deviceAppMsg.emvdappmsg[0].iovalue  = data;
					break;
				case EMVDSTATUS:
					deviceAPPMsg[driver].deviceAppMsg.emvdappmsg[0].status  = data;
					break;
				case EMVDFLAG:
					deviceAPPMsg[driver].deviceAppMsg.emvdappmsg[0].flag  = data;
					break;
			}				
			break;
		case ID_ANGLE:
			switch(point)
			{
				case ANGLE_X:
					deviceAPPMsg[driver].deviceAppMsg.angleappmsg[subDev].angle_x = data;
					break;
				case ANGLE_Y:
					deviceAPPMsg[driver].deviceAppMsg.angleappmsg[subDev].angle_y = data;
					break;
				case ANGLE_Z:
					deviceAPPMsg[driver].deviceAppMsg.angleappmsg[subDev].angle_z = data;
					break;
			}
			break;
		case ID_HIGH:
			deviceAPPMsg[driver].deviceAppMsg.hgihappmsg[subDev].high = (data&0xFFFF);
			break;
		case ID_PERSON:
			switch(point)
			{
				case PERSONMSG:
					deviceAPPMsg[driver].deviceAppMsg.personappmsg[subDev].person = data&0xFF;
					deviceAPPMsg[driver].deviceAppMsg.personappmsg[subDev].personPer = (data&0xFF00)>>8;
					break;
				case PERSONNUM:
					deviceAPPMsg[driver].deviceAppMsg.personappmsg[subDev].personNum = data;
					break;
				case PERSONDIS:
					deviceAPPMsg[driver].deviceAppMsg.personappmsg[subDev].personDis = data;
					break;
				case PERSONTIMEOUT:
					deviceAPPMsg[driver].deviceAppMsg.personappmsg[subDev].personTimeout = data;
				case PERSONBATTRY:
					deviceAPPMsg[driver].deviceAppMsg.personappmsg[subDev].personBattry = data;
					break;
				
			}
			break;
		case ID_IRRX:
			deviceAPPMsg[driver].deviceAppMsg.irappmsg[subDev].irCounts = data;
			break;
		case ID_ALARM:
			deviceAPPMsg[driver].deviceAppMsg.alarmappmsg[0].alarmMode[point] = data;
			break;
		case ID_ANGLE_ALARM:
			switch(point)
			{
				case ANGLE_X:
					deviceAPPMsg[driver].deviceAppMsg.anglealarmappMsg[0].angle_x= data;
				break;
				case ANGLE_Y:
					deviceAPPMsg[driver].deviceAppMsg.anglealarmappMsg[0].angle_y= data;
				break;
				case ANGLE_Z:
					deviceAPPMsg[driver].deviceAppMsg.anglealarmappMsg[0].angle_z= data;
				break;
			}
		break;	
		case ID_ANGLE_HIGH:
			switch(point)
			{
				case ANGLE_X:
					deviceAPPMsg[driver].deviceAppMsg.highangleappMsg[subDev].angle_x= data;
				break;
				case ANGLE_Y:
					deviceAPPMsg[driver].deviceAppMsg.highangleappMsg[subDev].angle_y= data;
				break;
				case ANGLE_Z:
					deviceAPPMsg[driver].deviceAppMsg.highangleappMsg[subDev].angle_z= data;
				break;
			}
		break;	
		case ID_HUB1:
			if(subDev == 0)
			{
				switch(point)
				{
					case  ANALOG_1:
						deviceAPPMsg[driver].deviceAppMsg.minihubappmsg[0].analog1= data;
						break;
					case  ANALOG_2:
						deviceAPPMsg[driver].deviceAppMsg.minihubappmsg[0].analog2= data;
						break;
					case  ANALOG_3:
						deviceAPPMsg[driver].deviceAppMsg.minihubappmsg[0].analog3= data;
						break;
					case  ANALOG_4:
						deviceAPPMsg[driver].deviceAppMsg.minihubappmsg[0].analog4= data;
						break;
				}
			}
			else
			{
				switch(point)
				{
					case ANGLE_X:
						deviceAPPMsg[driver].deviceAppMsg.minihubappmsg[0].hubangle[subDev-1].angle_x= data;
						break;
					case ANGLE_Y:
						deviceAPPMsg[driver].deviceAppMsg.minihubappmsg[0].hubangle[subDev-1].angle_y= data;
						break;
					case ANGLE_Z:
						deviceAPPMsg[driver].deviceAppMsg.minihubappmsg[0].hubangle[subDev-1].angle_z= data;
						break;
				}
			}
		break;			
		case ID_HUB2:
			if(subDev == 0)
			{
				switch(point)
				{
					case  ANALOG_1:
						deviceAPPMsg[driver].deviceAppMsg.minihubappmsg[1].analog1= data;
						break;
					case  ANALOG_2:
						deviceAPPMsg[driver].deviceAppMsg.minihubappmsg[1].analog2= data;
						break;
					case  ANALOG_3:
						deviceAPPMsg[driver].deviceAppMsg.minihubappmsg[1].analog3= data;
						break;
					case  ANALOG_4:
						deviceAPPMsg[driver].deviceAppMsg.minihubappmsg[1].analog4= data;
						break;
				}
			}
			else
			{
				switch(point)
				{
					case ANGLE_X:
						deviceAPPMsg[driver].deviceAppMsg.minihubappmsg[1].hubangle[subDev-1].angle_x= data;
						break;
					case ANGLE_Y:
						deviceAPPMsg[driver].deviceAppMsg.minihubappmsg[1].hubangle[subDev-1].angle_y= data;
						break;
					case ANGLE_Z:
						deviceAPPMsg[driver].deviceAppMsg.minihubappmsg[1].hubangle[subDev-1].angle_z= data;
						break;
				}
			}
		break;
		case ID_HUB3:
			if(subDev == 0)
			{
				switch(point)
				{
					case  ANALOG_1:
						deviceAPPMsg[driver].deviceAppMsg.minihubappmsg[2].analog1= data;
						break;
					case  ANALOG_2:
						deviceAPPMsg[driver].deviceAppMsg.minihubappmsg[2].analog2= data;
						break;
					case  ANALOG_3:
						deviceAPPMsg[driver].deviceAppMsg.minihubappmsg[2].analog3= data;
						break;
					case  ANALOG_4:
						deviceAPPMsg[driver].deviceAppMsg.minihubappmsg[2].analog4= data;
						break;
				}
			}
			else
			{
				switch(point)
				{
					case ANGLE_X:
						deviceAPPMsg[driver].deviceAppMsg.minihubappmsg[2].hubangle[subDev-1].angle_x= data;
						break;
					case ANGLE_Y:
						deviceAPPMsg[driver].deviceAppMsg.minihubappmsg[2].hubangle[subDev-1].angle_y= data;
						break;
					case ANGLE_Z:
						deviceAPPMsg[driver].deviceAppMsg.minihubappmsg[2].hubangle[subDev-1].angle_z= data;
						break;
				}
			}
		break;	
		default:
			break;
	}
//	OS_EXIT_CRITICAL();
}


/*******************************************************************************************
*功能描述：can的pro层设置设备应用数据函数（对外、对内）
*入口参数：driver取值：
*            驱动号
*          device取值：
*             设备号
*          subDev取值：
*             子设备号
*          point取值：
*             点号
*          deviceMsg取值：
*             设备信息指针
*返回值：  无
*******************************************************************************************/
void SetCanDeviceStatusMsg(uint16_t driver, uint16_t device, uint16_t subdevice, stDeviceStatusMsg* deviceMsg)
{
	#if OS_CRITICAL_METHOD == 3                      
		OS_CPU_SR  cpu_sr = 0;
	#endif
//	OS_ENTER_CRITICAL();
	switch(device)
	{
		case ID_EMVD:
			memcpy(&deviceStatusmsg[driver].deviceStatusMsgQueue[EMVD_Version], deviceMsg, sizeof(stDeviceStatusMsg));
			break;
		case ID_CXB:
			memcpy(&deviceStatusmsg[driver].deviceStatusMsgQueue[CXB_Version], deviceMsg, sizeof(stDeviceStatusMsg));
			break;
		case ID_ANGLE:
			memcpy(&deviceStatusmsg[driver].deviceStatusMsgQueue[subdevice], deviceMsg, sizeof(stDeviceStatusMsg));
			break;
		case ID_HIGH:
			memcpy(&deviceStatusmsg[driver].deviceStatusMsgQueue[HIGH_Verson], deviceMsg, sizeof(stDeviceStatusMsg));
			break;
		case ID_ALARM:
			memcpy(&deviceStatusmsg[driver].deviceStatusMsgQueue[ALARM_Version], deviceMsg, sizeof(stDeviceStatusMsg));
			break;			
		case ID_PERSON:
			memcpy(&deviceStatusmsg[driver].deviceStatusMsgQueue[PERSON_Version], deviceMsg, sizeof(stDeviceStatusMsg));
			break;		
		case ID_IRRX:
			memcpy(&deviceStatusmsg[driver].deviceStatusMsgQueue[IR_Verson], deviceMsg, sizeof(stDeviceStatusMsg));
			break;
		case ID_KEYBOARD:
			memcpy(&deviceStatusmsg[driver].deviceStatusMsgQueue[ID_KEYBOARD], deviceMsg, sizeof(stDeviceStatusMsg));
			break;
		case ID_WIRELESS:
			memcpy(&deviceStatusmsg[driver].deviceStatusMsgQueue[ID_WIRELESS], deviceMsg, sizeof(stDeviceStatusMsg));
			break;
		case ID_ANGLE_ALARM :
			memcpy(&deviceStatusmsg[driver].deviceStatusMsgQueue[ALARMANGLE_Version], deviceMsg, sizeof(stDeviceStatusMsg));
			break;
		case ID_ANGLE_HIGH :
			memcpy(&deviceStatusmsg[driver].deviceStatusMsgQueue[ALARMANGLE_Version + subdevice], deviceMsg, sizeof(stDeviceStatusMsg));
			break;
		case ID_HUB1:
			memcpy(&deviceStatusmsg[driver].deviceStatusMsgQueue[HUB1_Version + subdevice], deviceMsg, sizeof(stDeviceStatusMsg));
			break;
		case ID_HUB2:
			memcpy(&deviceStatusmsg[driver].deviceStatusMsgQueue[HUB2_Version + subdevice], deviceMsg, sizeof(stDeviceStatusMsg));
			break;
		case ID_HUB3:
			memcpy(&deviceStatusmsg[driver].deviceStatusMsgQueue[HUB3_Version + subdevice], deviceMsg, sizeof(stDeviceStatusMsg));
			break;
		default:
			break;
	}	
//	OS_EXIT_CRITICAL();
}


/*******************************************************************************************
*功能描述：can的pro层获取设备状态数据函数（对外、对内）
*入口参数：driver取值：
*            驱动号
*          device取值：
*             设备号
*          subDev取值：
*             子设备号
*          deviceMsg取值：
*             设备信息指针
*返回值：  无
*******************************************************************************************/
void GetCanDeviceStatusMsg(uint16_t driver, uint16_t device, uint16_t subdevice, stDeviceStatusMsg* deviceMsg)
{
	switch(device)
	{
		case ID_EMVD:
			memcpy(deviceMsg, &deviceStatusmsg[driver].deviceStatusMsgQueue[EMVD_Version], sizeof(stDeviceStatusMsg));
			break;
		case ID_CXB:
			memcpy(deviceMsg, &deviceStatusmsg[driver].deviceStatusMsgQueue[CXB_Version], sizeof(stDeviceStatusMsg));
			break;
		case ID_ANGLE:
			memcpy(deviceMsg, &deviceStatusmsg[driver].deviceStatusMsgQueue[subdevice], sizeof(stDeviceStatusMsg));
			break;
		case ID_HIGH:
			memcpy(deviceMsg, &deviceStatusmsg[driver].deviceStatusMsgQueue[HIGH_Verson], sizeof(stDeviceStatusMsg));
			break;
		case ID_ALARM:
			memcpy(deviceMsg, &deviceStatusmsg[driver].deviceStatusMsgQueue[ALARM_Version], sizeof(stDeviceStatusMsg));
			break;			
		case ID_PERSON:
			memcpy(deviceMsg, &deviceStatusmsg[driver].deviceStatusMsgQueue[PERSON_Version], sizeof(stDeviceStatusMsg));
			break;		
		case ID_IRRX:
			memcpy(deviceMsg, &deviceStatusmsg[driver].deviceStatusMsgQueue[IR_Verson], sizeof(stDeviceStatusMsg));
			break;
		case ID_KEYBOARD:
			memcpy(deviceMsg, &deviceStatusmsg[driver].deviceStatusMsgQueue[ID_KEYBOARD], sizeof(stDeviceStatusMsg));
			break;		
		case ID_WIRELESS:
			memcpy(deviceMsg, &deviceStatusmsg[driver].deviceStatusMsgQueue[ID_WIRELESS], sizeof(stDeviceStatusMsg));
			break;
		case ID_ANGLE_ALARM:
			memcpy(deviceMsg, &deviceStatusmsg[driver].deviceStatusMsgQueue[ALARMANGLE_Version], sizeof(stDeviceStatusMsg));
			break;
		case ID_ANGLE_HIGH:
			memcpy(deviceMsg, &deviceStatusmsg[driver].deviceStatusMsgQueue[ALARMANGLE_Version+subdevice], sizeof(stDeviceStatusMsg));
			break;
		case ID_HUB1:
			memcpy(deviceMsg, &deviceStatusmsg[driver].deviceStatusMsgQueue[HUB1_Version + subdevice], sizeof(stDeviceStatusMsg));
			break;	
		case ID_HUB2:
			memcpy(deviceMsg, &deviceStatusmsg[driver].deviceStatusMsgQueue[HUB2_Version + subdevice], sizeof(stDeviceStatusMsg));
			break;
		case ID_HUB3:
			memcpy(deviceMsg, &deviceStatusmsg[driver].deviceStatusMsgQueue[HUB3_Version + subdevice], sizeof(stDeviceStatusMsg));
			break;
		default:
			break;
	}	
}


/*******************************************************************************************
*功能描述：获取设备子设备所在的can驱动号
*入口参数：device取值：
*             设备号
*          subDev取值：
*             子设备号
*          driver取值：
*             can驱动号指针
*返回值：  1：正确 0：错误
*******************************************************************************************/
uint8_t GetDeviceCanMsg(uint16_t device, uint16_t subDev, uint8_t *driver)
{
	uint8_t res = 0;
	uint8_t i = 0;
	uint8_t tmp;
	
	switch(device)
	{
		case ID_EMVD:
			for(i = 0;i < deviceStatusMsgSize; i++)
			{
				if(deviceStatusmsg[i].deviceStatusMsgQueue[EMVD_Version].isOnline)
				{
					res++;
					tmp = i;
				}
			}
			break;
		case ID_ANGLE:
			for(i = 0;i < deviceStatusMsgSize; i++)
			{
				if(deviceStatusmsg[i].deviceStatusMsgQueue[subDev+1].isOnline
					||deviceStatusmsg[i].deviceStatusMsgQueue[HUB1_Version+1].isOnline
				    ||deviceStatusmsg[i].deviceStatusMsgQueue[HUB2_Version+1].isOnline
				    ||deviceStatusmsg[i].deviceStatusMsgQueue[HUB3_Version+1].isOnline)
				{
					res++;
					tmp = i;
				}
			}
			break;
		case ID_HIGH:
			for(i = 0;i < deviceStatusMsgSize; i++)
			{
				if(deviceStatusmsg[i].deviceStatusMsgQueue[HIGH_Verson].isOnline)
				{
					res++;
					tmp = i;
				}
			}
			break;
		case ID_ALARM:
			for(i = 0;i < deviceStatusMsgSize; i++)
			{
				if(deviceStatusmsg[i].deviceStatusMsgQueue[ALARM_Version].isOnline)
				{
					res++;
					tmp = i;
				}
			}
			break;
		case ID_PERSON:
			for(i = 0;i < deviceStatusMsgSize; i++)
			{
				if(deviceStatusmsg[i].deviceStatusMsgQueue[PERSON_Version].isOnline)
				{
					res++;
					tmp = i;
				}
			}
			break;	
		case ID_CXB:
			for(i = 0;i < deviceStatusMsgSize; i++)
			{
				if(deviceStatusmsg[i].deviceStatusMsgQueue[CXB_Version].isOnline)
				{
					res++;
					tmp = i;
				}
			}
			break;
		case ID_IRRX:
			for(i = 0;i < deviceStatusMsgSize; i++)
			{
				if(deviceStatusmsg[i].deviceStatusMsgQueue[IR_Verson].isOnline)
				{
					res++;
					tmp = i;
				}
			}
			break;
		//ghy 增加HUB在线状态判断
		case ID_HUB1:
			for(i = 0;i < deviceStatusMsgSize; i++)
			{
				if(deviceStatusmsg[i].deviceStatusMsgQueue[HUB1_Version+subDev].isOnline)
				{
					res++;
					tmp = i;
				}
			}
			break;
		case ID_HUB2:
			for(i = 0;i < deviceStatusMsgSize; i++)
			{
				if(deviceStatusmsg[i].deviceStatusMsgQueue[HUB1_Version+subDev].isOnline)
				{
					res++;
					tmp = i;
				}
			}
			break;
		 case ID_HUB3:
			for(i = 0;i < deviceStatusMsgSize; i++)
			{
				if(deviceStatusmsg[i].deviceStatusMsgQueue[HUB1_Version+subDev].isOnline)
				{
					res++;
					tmp = i;
				}
			}
			break;		
		default:
			res  = 0;
			break;
	}
	*driver = tmp;
	return res;
}


/*******************************************************************************************
*功能描述：can接收超时处理函数
*入口参数：RevBufTemp取值：
*             can帧
*返回值：  无
*******************************************************************************************/
void CanRevTimeoutProc(STR_Can_Frame* RevBufTemp)
{
	if((RevBufTemp->ID&V4CANPROFRAME) != 0) //V4外围设备
	{

	}
	else  //V2外围设备
	{
		V2CanRevTimeoutProc((STR_V2CAN_Frame*)RevBufTemp);
	}
}

/*******************************************************************************************
*功能描述：can接收超时处理函数
*入口参数：u16Way取值：
*             can帧来源（can1、can2）
*          RevBufTemp取值：
*             can帧
*返回值：  无
*******************************************************************************************/
void CanRecDeal(uint16_t u16Way, STR_Can_Frame* RevBufTemp)
{
	uint8_t temp;
	if(temp == 0)
		return;
	
	if((*CanRecIsAck)(RevBufTemp))//是否为接收应答帧
	{
		CanBusDealFunc(u16Way,CANBUS_MSG_DelCanSentQueue,&RevBufTemp->ID);
	}
	else
	{
		if((RevBufTemp->ID&V4CANPROFRAME) != 0) //V4外围设备
		{
//			CanFrameDeal_V4((STR_V4CAN_Frame*)RevBufTemp);
		}
		else  //V2外围设备
		{
			CanFrameDeal_V2(u16Way, (STR_V2CAN_Frame*)RevBufTemp);
		}
	}
}


/*******************************************************************************************
*功能描述：can接收任务
*入口参数：u16Way取值：
*             can帧来源（can1、can2）
*          RevBufTemp取值：
*             can帧
*返回值：  无
*******************************************************************************************/
void CanRec_task(void)
{
	INT8U err;
	uint8_t u8Way;
	STR_Can_Frame RevBufTemp;
	//复位外围设备can驱动
	CanOpenDriver(NATIVECAN1);
	CanOpenDriver(NATIVECAN2);
	//复位声光报警器
	CanProSend(NATIVECAN1,ID_ALARM,0,ALARM_RESET,1,0);
	OSTimeDly(10);
	//复位人员定位
	CanProSend(NATIVECAN1,ID_PERSON,0,ALARM_RESET,1,0);
	while (1) 
	{	
		u8Way = ((uint8_t)OSMboxPend(CanRTxTaskSendSem,0,&err));
		if(err == OS_ERR_NONE)
		{
			u8Way = (u8Way&0x0F);
			while(CanBusDealFunc(u8Way,CANBUS_MSG_GetCanRevQueue,&RevBufTemp))
			{
				CanRecDeal(u8Way, &RevBufTemp);
			}
		}
	}
}




