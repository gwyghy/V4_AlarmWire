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

//bus���ѷ��ͺ�����ʱ��
uint16_t BusCanSentTimerId;
//pro��ѭ�����Ͷ�ʱ��
uint16_t ProCanLoopTimerId;
//�豸������ʱ��
uint16_t DeviceHeartTimerId;

//�豸Ӧ����Ϣ����
stDeviceAppMsgQueue deviceAPPMsg[deviceAPPMsgSize];
//�豸״̬��Ϣ����
stDeviceStatusMsgQueue deviceStatusmsg[deviceStatusMsgSize];

//canѭ����������
stCanLoopSendQueue  CanLoopSendQueue;

/*******************************************************************************************
*����������canѭ�����������ʼ��
*��ڲ�������
*����ֵ��  ��
*******************************************************************************************/
static void canLoopSendDataInit(void)
{
	CanLoopSendQueue.u16CanLoopSendCnt = 0;
	memset(&CanLoopSendQueue.CanLoopSend, 0 ,sizeof(CanLoopSendQueue.CanLoopSend));	
}


/*******************************************************************************************
*��������������canѭ����������
*��ڲ�����CanLoopSendȡֵ��
*            canѭ�����ͽṹ��
*����ֵ��  ��
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
*�����������޸�canѭ����������
*��ڲ�����CanLoopSendȡֵ��
*            canѭ�����ͽṹ��
*          CMDȡֵ��
*             ������
*����ֵ��  ��
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
			if((CanLoopSendQueue.CanLoopSend[i].SendFrame.ID != CMD))   //�ж����ݵĵ������λ
				continue;
			memcpy(&(CanLoopSendQueue.CanLoopSend[i].SendFrame), &CanLoopSend->SendFrame, sizeof(STR_Can_Frame));
			break;
		}
		OS_EXIT_CRITICAL();
	}
}

/*******************************************************************************************
*����������ɾ��canѭ����������
*��ڲ�����CMDȡֵ��
*             ������
*����ֵ��  ��
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
			if((CanLoopSendQueue.CanLoopSend[i].SendFrame.ID != CMD))   //�ж����ݵĵ������λ
				continue;

			CanLoopSendQueue.u16CanLoopSendCnt --;
			memset(&(CanLoopSendQueue.CanLoopSend[i]),0,sizeof(stCanLoopSend));
//			break;
		}
		OS_EXIT_CRITICAL();
	}
}


/*******************************************************************************************
*����������pro��can���Ͷ�ʱ���ص�����
*��ڲ�����idȡֵ��
*             �ص�����id��
*����ֵ��  ��
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
*����������bus�������ط����ݹ���ص�����
*��ڲ�����idȡֵ��
*             �ص�����id��
*����ֵ��  ��
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

//Ĭ�ϣ���ŷ������⡢��Ա��������գ�������
static uint8_t EMVD_VersionFlag = 1;
static uint8_t ALARM_VersionFlag = 1;
static uint8_t PERSON_VersionFlag = 1;
static uint8_t IR_VersonFlag = 1;
//ghy ���H����HUB������״̬
static uint8_t HUB1_VersionFlag = 1;
static uint8_t HUB2_VersionFlag = 1;
static uint8_t HUB3_VersionFlag = 1;
/*******************************************************************************************
*�����������豸��������д��־����
*��ڲ�����idȡֵ��
*            �豸���ͣ���ŷ������⡢��Ա��������գ�
*          statusȡֵ��
*             ������
*����ֵ��  ��
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
		case  HUB1_Version:  //ghy ���HUB����״̬д��
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
*�����������豸������ʱ��ʱ���ص�����
*��ڲ�����idȡֵ��
*            ��ʱ��id��
*����ֵ��  ��
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
			//ghy ����δʹ��ʱ���������Ϣ
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
	//ghy ���HUB������Ϣ��ʾ
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
		for(j=0;j<11;j++)//ghy ��3s���ղ�����Ա�뿪����Ϣ����Ѹ���Ա��ǩ����Ϣ�����
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


//�ж�can�����Ƿ�ΪӦ��֡
uint8_t CanRecIsAck(STR_Can_Frame * frame)
{
	uint8_t res = 0;
	if((frame->ID & V4CANPROFRAME) != 0) //V4��Χ�豸
	{
		;
	}
	else  //V2��Χ�豸
	{
	  res = CanRecIsAck_V2(frame);
	}
	return res;
}

/*******************************************************************************************
*����������can��pro���ʼ��
*��ڲ�����deviceȡֵ��
*            can�豸������ͷָ��
*          bundsȡֵ��
*             can�豸�Ĳ���������ͷָ��
*          numȡֵ��
*             can�豸�ĸ���
*����ֵ��  ��
*******************************************************************************************/
void CanProInit(uint16_t* device, uint16_t* bunds,uint8_t num)
{
	uint8_t i;
	//bus���ʼ��can
	for(i = 0; i<num; i++)
	{
		CanBusInit(device[i],bunds[i]);
	}
	//�Ƕ���Ա��������ʼ������
	v2AngleandPersonInit();
	//��Χ�豸Ӧ�ú�״̬���ݳ�ʼ��
	memset(&deviceStatusmsg,0,sizeof(stDeviceStatusMsgQueue)*2);
	memset(&deviceAPPMsg,0,sizeof(deviceAPPMsg)*2);

	//can��pro��ѭ�����������ʼ��
	canLoopSendDataInit();
	
	//bus���ѷ��ͺ�����ʱ����ʼ��
	BusCanSentTimerId = addTimerEvent(1,0XFF,BusCanTimerSent_Callback);//1ms
	startTimerEvent(BusCanSentTimerId);

	//pro��ѭ�����ͺ�����ʱ����ʼ��	
	ProCanLoopTimerId = addTimerEvent(1,0XFF,ProCanTimerSend_Callback);//1ms
	startTimerEvent(ProCanLoopTimerId);	

	//�豸����������ʱ����ʼ��		
	DeviceHeartTimerId = addTimerEvent(100,0XFF,DeviceHeartTimerCallback);//100ms
	startTimerEvent(DeviceHeartTimerId);

}

uint8_t getTypeDevice(uint16_t device)
{
	return V2TYPEDEVICE;
}


/*******************************************************************************************
*����������can��pro�㷢�ͺ������������
*��ڲ�����driverȡֵ��
*            ������
*          deviceȡֵ��
*             �豸��
*          subDevȡֵ��
*             ���豸��
*          pointȡֵ��
*             ���
*          dataȡֵ��
*             ���ݣ���������豸���ͣ�
*          flagȡֵ��
*             flagΪ�����ר��
*����ֵ��  ��
*******************************************************************************************/
void CanProSend(uint16_t driver, uint16_t device, uint16_t subDev, uint16_t point, uint8_t data, uint8_t flag)
{
	stCanTrsQueue canTrsQueue;
	//��ȡV4�豸��V2�豸��ѯ��ͬ������
	if(getTypeDevice(device) == V2TYPEDEVICE) //V2
	{
		//���� device childDev point data��ѯpro��prohead
		if(!V2Pro_action_prohead(driver,device, subDev, point, data, flag, &(canTrsQueue.TrsFrame), &canTrsQueue.busHead))
		{
			return;
		}
	}
	else if(getTypeDevice(device) == V4TYPEDEVICE)  //V4
	{	
		//���� device childDev point data��ѯpro��prohead	
	}
	else
	{
		return;
	}	
	
	
	//������з���
	if(canTrsQueue.busHead.prio)
		CanBusDealFunc(driver, CANBUS_MSG_InsCanHighTrsQueue, &canTrsQueue);
	else
		CanBusDealFunc(driver, CANBUS_MSG_InsCanLowTrsQueue, &canTrsQueue);
}

void CanProDirectSend(uint16_t driver, uint16_t device, uint16_t subDev, uint16_t point, uint8_t* data, uint8_t len, uint32_t packetNum)
{
	stCanTrsQueue canTrsQueue;
	//��ȡV4�豸��V2�豸��ѯ��ͬ������
	if(getTypeDevice(device) == V2TYPEDEVICE) //V2
	{
		V2Pro_cxb_prohead(device, point, data, len, packetNum, &(canTrsQueue.TrsFrame), &canTrsQueue.busHead);
	}
	else if(getTypeDevice(device) == V4TYPEDEVICE)  //V4
	{	
		//���� device childDev point data��ѯpro��prohead
	}
	else
	{
		return;
	}	
	
	
	//������з���
	if(canTrsQueue.busHead.prio)
		CanBusDealFunc(driver, CANBUS_MSG_InsCanHighTrsQueue, &canTrsQueue);
	else
		CanBusDealFunc(driver, CANBUS_MSG_InsCanLowTrsQueue, &canTrsQueue);
}


/*******************************************************************************************
*����������can��pro��ѭ�����ͺ�������ŷ������⣩
*��ڲ�����driverȡֵ��
*            ������
*          deviceȡֵ��
*             �豸��
*          subDevȡֵ��
*             ���豸��
*          pointȡֵ��
*             ���
*          statusȡֵ��
*             ���ݣ���ŷ�������\ֹͣ�����ⱨ�����ͣ�
*          intervalȡֵ��
*             ѭ������ʱ����
*          countȡֵ��
*             ѭ�����ʹ���
*          sendiffunȡֵ��
*             ѭ�����ͻص�����
*����ֵ��  ��
*******************************************************************************************/
void CanProLoopSend(uint16_t driver,uint16_t device, uint16_t subDev, uint16_t point, uint16_t status, uint8_t interval, uint8_t count, LoopSendIfFun sendiffun)
{
	uint8_t res;
	stCanLoopSend CanLoopSend;
	uint32_t cmd;
	
	//��ȡV4�豸��V2�豸��ѯ��ͬ������
	if(getTypeDevice(device) == V2TYPEDEVICE) //V2
	{
		res = V2Pro_action_prohead(driver, device, subDev, point, status, 0, &(CanLoopSend.SendFrame), &CanLoopSend.head);
		//���� device childDev point data��ѯpro��prohead
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
		//���� device childDev point data��ѯpro��prohead	
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
*����������can��pro��������Χ�豸��������Ա��λ���������ṩ�ⲿ�ӿڣ�
*��ڲ�����deviceȡֵ��
*             �豸��
*          paramȡֵ��
*             ����ָ��
*          isNowȡֵ��
*             �Ƿ���������
*����ֵ��  ��
*******************************************************************************************/
void CanProSetParam(uint16_t device, void * param,uint8_t isNow)
{
	V2CanProSetParam(device,param,isNow);
}


/*******************************************************************************************
*����������can��pro��������Χ�豸�������Ƕȡ��߶Ȳ��������ṩ�ⲿ�ӿڣ�
*��ڲ�����driverȡֵ��
*            ������
*          deviceȡֵ��
*             �豸��
*          subdeviceȡֵ��
*             ���豸��
*����ֵ��  ��
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
	
	//������з���
	if(canTrsQueue.busHead.prio)
		CanBusDealFunc(driver, CANBUS_MSG_InsCanHighTrsQueue, &canTrsQueue);
}

/*******************************************************************************************
*����������can��pro���ȡ�豸Ӧ�����ݺ��������⡢���ڣ�
*��ڲ�����driverȡֵ��
*            ������
*          deviceȡֵ��
*             �豸��
*          subDevȡֵ��
*             ���豸��
*          pointȡֵ��
*             ���
*          dataȡֵ��
*             ����ָ��
*����ֵ��  ��
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
*����������can��pro�������豸Ӧ�����ݺ��������⡢���ڣ�
*��ڲ�����driverȡֵ��
*            ������
*          deviceȡֵ��
*             �豸��
*          subDevȡֵ��
*             ���豸��
*          pointȡֵ��
*             ���
*          dataȡֵ��
*             ����ָ��
*����ֵ��  ��
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
*����������can��pro�������豸Ӧ�����ݺ��������⡢���ڣ�
*��ڲ�����driverȡֵ��
*            ������
*          deviceȡֵ��
*             �豸��
*          subDevȡֵ��
*             ���豸��
*          pointȡֵ��
*             ���
*          deviceMsgȡֵ��
*             �豸��Ϣָ��
*����ֵ��  ��
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
*����������can��pro���ȡ�豸״̬���ݺ��������⡢���ڣ�
*��ڲ�����driverȡֵ��
*            ������
*          deviceȡֵ��
*             �豸��
*          subDevȡֵ��
*             ���豸��
*          deviceMsgȡֵ��
*             �豸��Ϣָ��
*����ֵ��  ��
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
*������������ȡ�豸���豸���ڵ�can������
*��ڲ�����deviceȡֵ��
*             �豸��
*          subDevȡֵ��
*             ���豸��
*          driverȡֵ��
*             can������ָ��
*����ֵ��  1����ȷ 0������
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
		//ghy ����HUB����״̬�ж�
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
*����������can���ճ�ʱ������
*��ڲ�����RevBufTempȡֵ��
*             can֡
*����ֵ��  ��
*******************************************************************************************/
void CanRevTimeoutProc(STR_Can_Frame* RevBufTemp)
{
	if((RevBufTemp->ID&V4CANPROFRAME) != 0) //V4��Χ�豸
	{

	}
	else  //V2��Χ�豸
	{
		V2CanRevTimeoutProc((STR_V2CAN_Frame*)RevBufTemp);
	}
}

/*******************************************************************************************
*����������can���ճ�ʱ������
*��ڲ�����u16Wayȡֵ��
*             can֡��Դ��can1��can2��
*          RevBufTempȡֵ��
*             can֡
*����ֵ��  ��
*******************************************************************************************/
void CanRecDeal(uint16_t u16Way, STR_Can_Frame* RevBufTemp)
{
	uint8_t temp;
	if(temp == 0)
		return;
	
	if((*CanRecIsAck)(RevBufTemp))//�Ƿ�Ϊ����Ӧ��֡
	{
		CanBusDealFunc(u16Way,CANBUS_MSG_DelCanSentQueue,&RevBufTemp->ID);
	}
	else
	{
		if((RevBufTemp->ID&V4CANPROFRAME) != 0) //V4��Χ�豸
		{
//			CanFrameDeal_V4((STR_V4CAN_Frame*)RevBufTemp);
		}
		else  //V2��Χ�豸
		{
			CanFrameDeal_V2(u16Way, (STR_V2CAN_Frame*)RevBufTemp);
		}
	}
}


/*******************************************************************************************
*����������can��������
*��ڲ�����u16Wayȡֵ��
*             can֡��Դ��can1��can2��
*          RevBufTempȡֵ��
*             can֡
*����ֵ��  ��
*******************************************************************************************/
void CanRec_task(void)
{
	INT8U err;
	uint8_t u8Way;
	STR_Can_Frame RevBufTemp;
	//��λ��Χ�豸can����
	CanOpenDriver(NATIVECAN1);
	CanOpenDriver(NATIVECAN2);
	//��λ���ⱨ����
	CanProSend(NATIVECAN1,ID_ALARM,0,ALARM_RESET,1,0);
	OSTimeDly(10);
	//��λ��Ա��λ
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




