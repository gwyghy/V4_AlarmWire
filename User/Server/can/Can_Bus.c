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
#include "Can_Bus.h"
#include "Can_Pro.h"
#include "NativeCan_Driver.h"
#include "SPICan_Driver.h"
#include "Util_TImer.h"



CANBUS_DATA  CanBusDev[CAN_DRIVER_DEV_NUM];
static uint8_t CanSendPrioFlag[CAN_DRIVER_DEV_NUM]={1,1};//���ȼ����ͱ�־λ
static uint8_t CanSendTimeOutFlag[CAN_DRIVER_DEV_NUM]={0,0};//��ʱ��־λ
static uint8_t CanSendAbortFlag[CAN_DRIVER_DEV_NUM]={0,0};//�����־λ
static uint16_t TimeoutId[] = {0,0};
static uint16_t ErrId[] = {0,0};

static uint8_t initFlag = 0;


const CAN_Driver CanDevPara[CAN_DRIVER_DEV_NUM] = {
	{
		DriverNativeCan_Init,
		DriverNativeCan_Open,
		DriverNativeCan_Close,
		DriverNativeCan_Read,
		DriverNativeCan_Write,
		DriverNativeCan_IoCtl,
	},
	{
		DriverNativeCan_Init,
		DriverNativeCan_Open,
		DriverNativeCan_Close,
		DriverNativeCan_Read,
		DriverNativeCan_Write,
		DriverNativeCan_IoCtl,
	},
//	{
//		DriverSPICan_Init,
//		NULL,
//		NULL,
//		DriverSPICan_Read,
//		DriverSPICan_Write,
//		DriverSPICan_IoCtl,
//	}
//	,
//	{
//		DriverSPICan_Init,
//		NULL,
//		NULL,
//		DriverSPICan_Read,
//		DriverSPICan_Write,
//		DriverSPICan_IoCtl,
//	}
//	,
};




/*******************************************************************************************
**�������ƣ�CanBusDataInit
**�������ã�canBus���ݳ�ʼ��
**������������
**�����������
**ע�������
*******************************************************************************************/
static void CanBusDataInit(uint8_t candev)
{
	uint8_t i;
	#if OS_CRITICAL_METHOD == 3                      
    OS_CPU_SR  cpu_sr = 0;
#endif

	OS_ENTER_CRITICAL();
	
	memset(CanBusDev[candev].CanLowTrs, 0 ,sizeof(CanBusDev[candev].CanLowTrs));
	memset(CanBusDev[candev].CanHighTrs, 0 ,sizeof(CanBusDev[candev].CanHighTrs));
	memset(CanBusDev[candev].CanRev, 0 ,sizeof(CanBusDev[candev].CanRev));
	memset(CanBusDev[candev].CanSent, 0 ,sizeof(CanBusDev[candev].CanSent));
	CanBusDev[candev].u16CanLowTrsCnt = 0;
	CanBusDev[candev].u16CanHighTrsCnt = 0;
	CanBusDev[candev].u16CanRevCnt = 0;
	CanBusDev[candev].u16CanSentCnt = 0;
	
	//�ѷ���
	CanBusDev[candev].pCanSentRead = NULL;
	CanBusDev[candev].pCanSentWrite =&CanBusDev[candev].CanSent[0];
	for(i=0 ; i<CAN_SENT_QUEUE_NUM - 1; i++)
	{
		CanBusDev[candev].CanSent[i].pNext = &CanBusDev[candev].CanSent[i+1];
	}	
	CanBusDev[candev].CanSent[CAN_SENT_QUEUE_NUM-1].pNext = NULL;		
	
	
	//�����ȼ�����
	CanBusDev[candev].pCanLowTrsHead = &CanBusDev[candev].CanLowTrs[0];
	CanBusDev[candev].pCanLowTrsEnd = &CanBusDev[candev].CanLowTrs[0];
	for(i=0 ; i<CAN_TX_QUEUE_NUM - 1; i++)
	{
		CanBusDev[candev].CanLowTrs[i].pNext = &CanBusDev[candev].CanLowTrs[i+1];
	}	
	CanBusDev[candev].CanLowTrs[CAN_TX_QUEUE_NUM-1].pNext = &CanBusDev[candev].CanLowTrs[0];
	
	
	//�����ȼ�����
	CanBusDev[candev].pCanHighTrsHead = &CanBusDev[candev].CanHighTrs[0];
	CanBusDev[candev].pCanHighTrsEnd = &CanBusDev[candev].CanHighTrs[0];
	for(i=0 ; i<CAN_TX_QUEUE_NUM - 1; i++)
	{
		CanBusDev[candev].CanHighTrs[i].pNext = &CanBusDev[candev].CanHighTrs[i+1];
	}	
	CanBusDev[candev].CanHighTrs[CAN_TX_QUEUE_NUM-1].pNext = &CanBusDev[candev].CanHighTrs[0];
		
	
	//����
	CanBusDev[candev].pCanRevHead = &CanBusDev[candev].CanRev[0];
	CanBusDev[candev].pCanRevEnd =&CanBusDev[candev].CanRev[0];
	for(i=0 ; i<CAN_RX_QUEUE_NUM - 1; i++)
	{
		CanBusDev[candev].CanRev[i].pNext = &CanBusDev[candev].CanRev[i+1];
	}	
	CanBusDev[candev].CanRev[CAN_RX_QUEUE_NUM-1].pNext = &CanBusDev[candev].CanRev[0];
	
	OS_EXIT_CRITICAL();
		
}

uint8_t CanOpenDriver(uint16_t driver)
{
	CanBusDev[driver].Cfg.Open(driver);
}

uint8_t CanCloseDriver(uint16_t driver)
{
	CanBusDev[driver].Cfg.Close(driver);
}


/*******************************************************************************************
**�������ƣ�CanBusInit
**�������ã�canBus��ʼ��
**������������
**�����������
**ע�������
*******************************************************************************************/
uint8_t CanBusInit(uint16_t device,uint8_t baud)
{
	
	CAN_Driver   *cfg;
	
	if(device >= CAN_DRIVER_DEV_NUM)
	{
		return 0;
	}
	initFlag++;
	CanBusDataInit(device);	
	
	cfg = &(CanBusDev[device].Cfg);
	memcpy(cfg,&CanDevPara[device], sizeof(CAN_Driver));
	
	cfg->Init(device,baud);//���ò�����
	
	
	if(initFlag == 1)
	{
		TimeoutId[0] = addTimerEvent(10,0XFF,CanSendTIMOUT_Callback);
		ErrId[0] = addTimerEvent(1,0XFF,CanSendERR_callback);
	}	
	return 1;
}

/*******************************************************************************************
**�������ƣ�CanBusIoCtl
**�������ã���װCan_Driver��IoCtl
**������������
**�����������
**ע�������
*******************************************************************************************/
uint8_t CanBusIoCtl(uint8_t busId, uint8_t func, void *argp)
{
	if(busId >= CAN_DRIVER_DEV_NUM)
	{
		return 0;
	}
	CanBusDev[busId].Cfg.IoCtl(busId ,func, argp);		
	return 1;
}


/*******************************************************************************************
**�������ƣ�InsCanSentQueue
**�������ã����ݴ��뷢����ɶ���
**������������
**�����������
**ע�������
*******************************************************************************************/
void InsCanSentQueue(CANBUS_DATA * pCanBus, stCanTrsQueue *SentCan)//�����ѷ��Ͷ���read
{
	stCanTrsQueue    *pCanSentTemp;
#if OS_CRITICAL_METHOD == 3                      
    OS_CPU_SR  cpu_sr = 0;
#endif

	OS_ENTER_CRITICAL();
	
	if(SentCan->busHead.ack == 1 || SentCan->busHead.count>1)
	{
		if(SentCan->busHead.count > 0)
			SentCan->busHead.count--;
		
		if(pCanBus->u16CanSentCnt <CAN_SENT_QUEUE_NUM)
		{
			memcpy(&(pCanBus->pCanSentWrite->TrsFrame),&(SentCan->TrsFrame),sizeof(STR_Can_Frame));
			memcpy(&(pCanBus->pCanSentWrite->busHead),&(SentCan->busHead),sizeof(STR_Can_Frame));
			pCanSentTemp = pCanBus->pCanSentWrite;
			pCanBus->pCanSentWrite = pCanBus->pCanSentWrite->pNext;
			
			pCanSentTemp->pNext =  pCanBus->pCanSentRead;
			pCanBus->pCanSentRead = pCanSentTemp;
			pCanBus->u16CanSentCnt ++;
		}
	}
	OS_EXIT_CRITICAL();
}



static uint8_t CheckFrameCmd(STR_Can_Frame * frame,uint32_t cmd)
{
	if((frame->ID & cmd) != 0)
	{
		return 1;
	}
	return 0;
}

/*******************************************************************************************
**�������ƣ�stCanTrsQueue
**�������ã�����ѷ��Ͷ����е�CMD��Ϣ
**������������
**�����������
**ע�������
*******************************************************************************************/
 stCanTrsQueue * CheckCanSentQueue(uint32_t CMD,CANBUS_DATA * pCanBus)
{
	stCanTrsQueue *pCanSentTemp ,*pCanSentResTemp;
	
	pCanSentResTemp=NULL;
	pCanSentTemp = pCanBus->pCanSentRead;
	if(pCanBus ->u16CanSentCnt ==0)
	{
		return pCanSentResTemp;
	}
	
	do
	{
		if(CheckFrameCmd(&(pCanSentTemp->TrsFrame),CMD))
		{
			pCanSentResTemp = pCanSentTemp;
			break;
		}
		pCanSentTemp = pCanSentTemp->pNext;
	}while(pCanSentTemp != NULL);
	
	return pCanSentResTemp;
}


/*******************************************************************************************
**�������ƣ�DelCanSentQueue
**�������ã�ɾ���ѷ��Ͷ�������
**������������
**�����������
**ע�������
*******************************************************************************************/
void DelCanSentQueue(uint32_t CMD, uint16_t u16Way)//ɾ���ѷ��Ͷ���
{
	stCanTrsQueue	*pCanSentDelTemp = NULL;
	stCanTrsQueue	*pCanSentTemp = 	CanBusDev[u16Way].pCanSentRead;
#if OS_CRITICAL_METHOD == 3                      
    OS_CPU_SR  cpu_sr = 0;
#endif

	OS_ENTER_CRITICAL();


	pCanSentDelTemp = CheckCanSentQueue(CMD,&CanBusDev[u16Way]);
	if(pCanSentDelTemp != NULL)
	{
		if(pCanSentTemp == pCanSentDelTemp)
		{
			CanBusDev[u16Way].pCanSentRead = CanBusDev[u16Way].pCanSentRead ->pNext;
			CanBusDev[u16Way].u16CanSentCnt --;
			pCanSentTemp->pNext = CanBusDev[u16Way].pCanSentWrite;
			CanBusDev[u16Way].pCanSentWrite = pCanSentTemp;
		}
		else
		{		
			do
			{
				if(pCanSentTemp -> pNext == pCanSentDelTemp)
				{
					pCanSentTemp -> pNext = pCanSentDelTemp->pNext;
					pCanSentDelTemp->pNext = CanBusDev[u16Way].pCanSentWrite;
					CanBusDev[u16Way].pCanSentWrite = pCanSentDelTemp;
					CanBusDev[u16Way].u16CanSentCnt --;
					break;
				}
				pCanSentTemp = pCanSentTemp->pNext;
			}while(pCanSentTemp !=NULL);
		}
	}

	OS_EXIT_CRITICAL();
}



static void CheckCanSentInterval(uint16_t u16Way, uint16_t delay)
{
	stCanTrsQueue    *pCanSentTemp ,*pCanSentProTemp;
	CANBUS_DATA * pCanBus = &CanBusDev[u16Way];
#if OS_CRITICAL_METHOD == 3                      
    OS_CPU_SR  cpu_sr = 0;
#endif

	OS_ENTER_CRITICAL();
	
	if(CanBusDev[u16Way].u16CanSentCnt == 0)
	{
		OS_EXIT_CRITICAL();
		return;
	}	
	pCanSentProTemp = CanBusDev[u16Way].pCanSentRead;
	if(pCanSentProTemp == NULL)
	{
		CanBusDev[u16Way].u16CanSentCnt = 0;
		OS_EXIT_CRITICAL();
		return;
	}
	pCanSentTemp = pCanSentProTemp ->pNext;
	if(pCanSentTemp == NULL)
	{
		if(pCanSentProTemp->busHead.overtime >= delay)
		{
			pCanSentProTemp->busHead.overtime -= delay;
		}
		else
		{
			pCanSentProTemp->busHead.overtime = 0;
		}
		
		if(pCanSentProTemp->busHead.overtime == 0)
		{
			if(pCanSentProTemp->busHead.count > 0)
			{
				pCanSentProTemp->busHead.overtime = pCanSentProTemp->busHead.backtime;
				if(pCanSentProTemp->busHead.prio)
					InsCanHighTrsQueue(u16Way,pCanBus,pCanSentProTemp);
				else
					InsCanLowTrsQueue(u16Way,pCanBus,pCanSentProTemp);
			}
			if(pCanSentProTemp->busHead.count == 0 && pCanSentProTemp->busHead.ack == 1)
				CanRevTimeoutProc(&pCanSentProTemp->TrsFrame);
		
			CanBusDev[u16Way].pCanSentRead = NULL;
			pCanSentProTemp->pNext = CanBusDev[u16Way].pCanSentWrite;
			CanBusDev[u16Way].pCanSentWrite = pCanSentProTemp;
			CanBusDev[u16Way].u16CanSentCnt = 0;
		}
		OS_EXIT_CRITICAL();
		return ;
	}
	while(pCanSentTemp != NULL)
	{
		if(pCanSentTemp->busHead.overtime >= delay)
		{
			pCanSentTemp->busHead.overtime -= delay;
		}
		else
		{
			pCanSentTemp->busHead.overtime = 0;
		}
		
		if(pCanSentTemp->busHead.overtime == 0)
		{
			if(pCanSentTemp->busHead.count > 0)
			{
				pCanSentTemp->busHead.overtime = pCanSentTemp->busHead.backtime;
				if(pCanSentTemp->busHead.prio)
					InsCanHighTrsQueue(u16Way,pCanBus,pCanSentTemp);
				else
					InsCanLowTrsQueue(u16Way,pCanBus,pCanSentTemp);
			}
			if(pCanSentTemp->busHead.count == 0)
				CanRevTimeoutProc(&pCanSentTemp->TrsFrame);
			
			pCanSentProTemp -> pNext = pCanSentTemp->pNext;
			pCanSentTemp->pNext = CanBusDev[u16Way].pCanSentWrite;
			CanBusDev[u16Way].pCanSentWrite = pCanSentTemp;
			CanBusDev[u16Way].u16CanSentCnt --;
			pCanSentTemp = pCanSentProTemp -> pNext;
		}
		else
		{
			pCanSentTemp = pCanSentTemp->pNext;
			pCanSentProTemp = pCanSentProTemp ->pNext;
		} 
	}
	OS_EXIT_CRITICAL();
}
/*******************************************************************************************
**�������ƣ�InsCanHighTrsQueue
**�������ã���������ȼ����Ͷ�������
**������������
**�����������
**ע�������
*******************************************************************************************/
void InsCanHighTrsQueue(uint16_t u16Way, CANBUS_DATA * pCanBus, stCanTrsQueue *SentCan)//��������ȼ����Ͷ���
{
#if OS_CRITICAL_METHOD == 3                      
    OS_CPU_SR  cpu_sr = 0;
#endif
	OS_ENTER_CRITICAL();
	if(pCanBus->u16CanHighTrsCnt == 0)
	{
		memcpy(&(pCanBus->pCanHighTrsHead->TrsFrame), &SentCan->TrsFrame, sizeof(STR_Can_Frame));
		memcpy(&(pCanBus->pCanHighTrsHead->busHead), &SentCan->busHead, sizeof(CANBUSHEAD));
		pCanBus->pCanHighTrsHead = pCanBus->pCanHighTrsHead->pNext;
		pCanBus->u16CanHighTrsCnt ++;
//		
//		if(testemvdtimer)
//		{
//			testemvdtimer = 0;
//			UdpLinkLayerSend_app(test111,3,1,0x1802 ,2,8);
//		}
			
		pCanBus->Cfg.Write(u16Way,&(pCanBus->pCanHighTrsEnd->TrsFrame));
		
		CanSendPrioFlag[u16Way] = 1;
		CanSendAbortFlag[u16Way] = 0;
		startTimerEvent(TimeoutId[0]);
	}
	else
	{
		if(CanBusDev[u16Way].u16CanHighTrsCnt >= CAN_TX_QUEUE_NUM)
		{
			OS_EXIT_CRITICAL();
			return;		
		}
		memcpy(&(pCanBus->pCanHighTrsHead->TrsFrame), &SentCan->TrsFrame, sizeof(STR_Can_Frame));
		memcpy(&(pCanBus->pCanHighTrsHead->busHead), &SentCan->busHead, sizeof(CANBUSHEAD));		
		pCanBus->pCanHighTrsHead = pCanBus->pCanHighTrsHead->pNext;
		pCanBus->u16CanHighTrsCnt ++;
	}
	OS_EXIT_CRITICAL();
}

/*******************************************************************************************
**�������ƣ�DelCanHighTrsQueue
**�������ã�ɾ�������ȼ����Ͷ�������
**������������
**�����������
**ע�������
*******************************************************************************************/
void DelCanHighTrsQueue(CANBUS_DATA * pCanBus)//ɾ�������ȼ��Ͷ���
{
	uint8_t u16Way;
#if OS_CRITICAL_METHOD == 3                      
    OS_CPU_SR  cpu_sr = 0;
#endif

	OS_ENTER_CRITICAL();
	if(pCanBus->u16CanHighTrsCnt)
	{
		if(pCanBus->pCanHighTrsEnd->busHead.count > 1)
		{
			InsCanSentQueue(pCanBus, pCanBus->pCanHighTrsEnd);
		}	
		pCanBus->u16CanHighTrsCnt --;
		pCanBus->pCanHighTrsEnd = pCanBus->pCanHighTrsEnd->pNext;	
	}
	OS_EXIT_CRITICAL();
}

/*******************************************************************************************
**�������ƣ�InsCanHighTrsQueue
**�������ã���������ȼ����Ͷ�������
**����������u16Way�������豸���ͣ�* pCanBus�������豸�����࣬*SentCan������
**�����������
**ע�������
*******************************************************************************************/
void InsCanLowTrsQueue(uint16_t u16Way, CANBUS_DATA * pCanBus, stCanTrsQueue *SentCan)//��������ȼ����Ͷ���
{
#if OS_CRITICAL_METHOD == 3                      
    OS_CPU_SR  cpu_sr = 0;
#endif
	
	if (pCanBus->u16CanLowTrsCnt == 0 && pCanBus->u16CanHighTrsCnt == 0)
	{
		OS_ENTER_CRITICAL();
		memcpy(&(pCanBus->pCanLowTrsHead->TrsFrame), &SentCan->TrsFrame, sizeof(STR_Can_Frame));
		memcpy(&(pCanBus->pCanLowTrsHead->busHead), &SentCan->busHead, sizeof(CANBUSHEAD));		
		pCanBus->pCanLowTrsHead = pCanBus->pCanLowTrsHead->pNext;
		pCanBus->u16CanLowTrsCnt ++;
		OS_EXIT_CRITICAL();
		pCanBus->Cfg.Write(u16Way,&(pCanBus->pCanLowTrsEnd->TrsFrame));	
		
		CanSendPrioFlag[u16Way] = 0;
		CanSendAbortFlag[u16Way] = 0;
		startTimerEvent(TimeoutId[0]);
	}
	else
	{
		OS_ENTER_CRITICAL();
		memcpy(&(pCanBus->pCanLowTrsHead->TrsFrame), &SentCan->TrsFrame, sizeof(STR_Can_Frame));
		memcpy(&(pCanBus->pCanLowTrsHead->busHead), &SentCan->busHead, sizeof(CANBUSHEAD));		
		pCanBus->pCanLowTrsHead = pCanBus->pCanLowTrsHead->pNext;
		pCanBus->u16CanLowTrsCnt ++;
		OS_EXIT_CRITICAL();
	}
}

/*******************************************************************************************
**�������ƣ�DelCanHighTrsQueue
**�������ã�ɾ�������ȼ����Ͷ�������
**������������
**�����������
**ע�������
*******************************************************************************************/
void DelCanLowTrsQueue(CANBUS_DATA * pCanBus)//ɾ�������ȼ�����
{
#if OS_CRITICAL_METHOD == 3                      
    OS_CPU_SR  cpu_sr = 0;
#endif

	OS_ENTER_CRITICAL();
	if(pCanBus->u16CanLowTrsCnt)
	{	
		if(pCanBus->pCanLowTrsEnd->busHead.count > 1)
		{
			InsCanSentQueue(pCanBus, pCanBus->pCanLowTrsEnd);
		}	
		pCanBus->u16CanLowTrsCnt --;
		pCanBus->pCanLowTrsEnd = pCanBus->pCanLowTrsEnd->pNext;	
	}
	OS_EXIT_CRITICAL();
}


/*******************************************************************************************
**�������ƣ�InsCanRevQueue
**�������ã�������ն���
**������������
**�����������
**ע�������
*******************************************************************************************/
void InsCanRevQueue(uint16_t u16Way, STR_Can_Frame* RevBufTemp)
{
	#if OS_CRITICAL_METHOD == 3                      
    OS_CPU_SR  cpu_sr = 0;
	#endif
	OS_ENTER_CRITICAL();	
	if(CanBusDev[u16Way].u16CanRevCnt < CAN_RX_QUEUE_NUM)
	{
		memcpy(&CanBusDev[u16Way].pCanRevHead->RxFrame, RevBufTemp, sizeof(STR_Can_Frame));
		CanBusDev[u16Way].pCanRevHead = CanBusDev[u16Way].pCanRevHead->pNext;
		CanBusDev[u16Way].u16CanRevCnt ++;
	}
	OS_EXIT_CRITICAL();		
//�ͷ��ź���	
	u16Way |= 0xF0;
	OSMboxPost(CanRTxTaskSendSem,(void*)u16Way);
}

/*******************************************************************************************
**�������ƣ�GetCanRevQueue
**�������ã����ն���ȡ��
**������������
**�����������
**ע�������
*******************************************************************************************/
uint8_t GetCanRevQueue(CANBUS_DATA * pCanBus, STR_Can_Frame * RevBufTemp)
{
	uint8_t res = 1;
	#if OS_CRITICAL_METHOD == 3                      
    OS_CPU_SR  cpu_sr = 0;
	#endif
	OS_ENTER_CRITICAL();
		
	if(pCanBus->u16CanRevCnt > 0)
	{
		memcpy(RevBufTemp, &(pCanBus->pCanRevEnd->RxFrame), sizeof(STR_Can_Frame));
		pCanBus->pCanRevEnd = pCanBus->pCanRevEnd->pNext;
		pCanBus->u16CanRevCnt --;
	}
	else
		res = 0;
	
	OS_EXIT_CRITICAL();
	return res;
}

/*******************************************************************************************
**�������ƣ�CanDriverTxDealFun
**�������ã�can�������ʹ���
**������������
**�����������
**ע�������
*******************************************************************************************/
void CanDriverTxDealFun(uint16_t u16Way, uint16_t res)
{
	static uint8_t count = 0;
	CANBUS_DATA * pCanBus = &CanBusDev[u16Way];
	if(res == 0)                                         //���Ͳ��ɹ�
	{
		if(count++ > 5)
		{
			count = 0;
			CanBusDev[u16Way].Cfg.IoCtl(u16Way, CANCTL_ABORT_SEND_MAIL0,&res);
			//ɾ��
			if(CanSendPrioFlag[u16Way])
			{
				DelCanHighTrsQueue(pCanBus);
			}
			else
			{
				DelCanLowTrsQueue(pCanBus);
			}
		}
		if(CanBusDev[u16Way].u16CanHighTrsCnt > 0)
		{
			CanBusDev[u16Way].Cfg.IoCtl(u16Way, CANCTL_ABORT_SEND_MAIL0, 0);
			CanSendPrioFlag[u16Way] = 1;
			CanSendTimeOutFlag[u16Way] = 0;
			CanSendAbortFlag[u16Way] =1;
			
 			startTimerEvent(TimeoutId[0]);
			startTimerEvent(ErrId[0]);
		}
		else if(CanBusDev[u16Way].u16CanLowTrsCnt > 0)
		{
			CanBusDev[u16Way].Cfg.IoCtl(u16Way, CANCTL_ABORT_SEND_MAIL0, 0);
			CanSendPrioFlag[u16Way] = 0;
			CanSendTimeOutFlag[u16Way] = 0;
			CanSendAbortFlag[u16Way] =1;
			
			startTimerEvent(TimeoutId[0]);
			startTimerEvent(ErrId[0]);
		}
	}
	else
	{
		count = 0;
		CanSendAbortFlag[u16Way] = 0;
	
				//ɾ��֡   jhyjhy
		if(CanSendPrioFlag[u16Way])
		{			
			DelCanHighTrsQueue(pCanBus);
		}
		else
		{
			DelCanLowTrsQueue(pCanBus);
		}	
				
		if(CanBusDev[u16Way].u16CanHighTrsCnt > 0)
		{
			CanBusDev[u16Way].Cfg.Write(u16Way,&(CanBusDev[u16Way].pCanHighTrsEnd->TrsFrame));
			CanSendTimeOutFlag[u16Way] = 0;		//�������
			startTimerEvent(TimeoutId[0]);
		}			
		else if(CanBusDev[u16Way].u16CanLowTrsCnt > 0)
		{
			CanBusDev[u16Way].Cfg.Write(u16Way,&(CanBusDev[u16Way].pCanLowTrsEnd->TrsFrame));
			CanSendTimeOutFlag[u16Way] = 0;		//�������
			startTimerEvent(TimeoutId[0]);
		}
		else
		{
			CanSendTimeOutFlag[u16Way] = 2;	//�����
			stopTimerEvent(TimeoutId[0]);
		}	
	}
}


/*******************************************************************************************
**�������ƣ�CanSendTIMOUT_IRQHandler
**�������ã�can���ݷ��ͳ�ʱ��ʱ���жϴ���
**������������
**�����������
**ע�������
*******************************************************************************************/
void CanSendTIMOUT_Callback(uint16_t id)
{

	uint16_t  res;
	uint16_t u16Way;
	CANBUS_DATA * pCanBus;
	for(u16Way = 0;u16Way<CAN_DRIVER_DEV_NUM;u16Way++)
	{
		pCanBus = &CanBusDev[u16Way];
		
		if(CanSendTimeOutFlag[u16Way] != 2)
		{
			if((pCanBus->u16CanHighTrsCnt > 0) && CanSendPrioFlag[u16Way] == 1)
			{
				pCanBus->Cfg.IoCtl(u16Way, CANCTL_CHECK_SEND_MAIL0,&res);
				if(res == 0)                                                    //0#�������䲻����
				{
					pCanBus->Cfg.IoCtl(u16Way, CANCTL_ABORT_SEND_MAIL0,0);
					CanSendTimeOutFlag[u16Way] = 0;
					DelCanHighTrsQueue(pCanBus);
				}
				if(pCanBus->u16CanHighTrsCnt > 0)
				{
					pCanBus->Cfg.Write(u16Way,&(pCanBus->pCanHighTrsEnd->TrsFrame));
				}
			}
			if(((CanBusDev[u16Way].u16CanLowTrsCnt > 0) && CanSendPrioFlag[u16Way] == 0))				
			{
				CanBusDev[u16Way].Cfg.IoCtl(u16Way, CANCTL_CHECK_SEND_MAIL0,&res);
				if(res == 0)                                                    //0#�������䲻����
				{
					CanBusDev[u16Way].Cfg.IoCtl(u16Way, CANCTL_ABORT_SEND_MAIL0,0);
					CanSendTimeOutFlag[u16Way] = 0;
					DelCanLowTrsQueue(pCanBus);
					if(CanBusDev[u16Way].u16CanLowTrsCnt > 0)
					{
						CanBusDev[u16Way].Cfg.Write(u16Way,&(CanBusDev[u16Way].pCanLowTrsEnd->TrsFrame));
					}
//					else
//					{
//							
//					}
				}
			}		
		}	
	}
	
	stopTimerEvent(TimeoutId[0]);
	CanSendTimeOutFlag[u16Way] = 2;	
}


/*******************************************************************************************
**�������ƣ�CanSendERR_IRQHandler
**�������ã�can���ݷ��ʹ���ʱ���жϴ���
**������������
**�����������
**ע�������
*******************************************************************************************/
void CanSendERR_callback(uint16_t id)
{
	uint16_t u16Way;
	for(u16Way = 0;u16Way<CAN_DRIVER_DEV_NUM;u16Way++)
	{
		if(CanSendAbortFlag[u16Way] == 1)
		{
			if(CanSendPrioFlag[u16Way] == 1)
				CanBusDev[u16Way].Cfg.Write(u16Way, &(CanBusDev[u16Way].pCanHighTrsEnd->TrsFrame));
			CanSendAbortFlag[u16Way] = 0;
			startTimerEvent(TimeoutId[0]);
			stopTimerEvent(ErrId[0]);
		}
		if(CanSendAbortFlag[u16Way] == 1)
		{
			if(CanSendPrioFlag[u16Way] == 0)
				CanBusDev[u16Way].Cfg.Write(u16Way,&(CanBusDev[u16Way].pCanLowTrsEnd->TrsFrame));
			CanSendAbortFlag[u16Way] = 0;
			startTimerEvent(TimeoutId[0]);
			stopTimerEvent(ErrId[0]);
		}
	}
}

/*******************************************************************************************
**�������ƣ�CanBusDealFunc
**�������ã��ӿں���
**������������
**�����������
**ע�������
*******************************************************************************************/
uint8_t CanBusDealFunc(uint16_t u16Way, CANBUS_MSG msg, void * pData)
{
	uint8_t temp = 1;
	
	if(msg > CANBUS_MSG_MAX)
	{
		return 0;
	}
	if(u16Way > 3)
		return 0;
	switch(msg)
	{
		case CANBUS_MSG_InsCanSentQueue:
			InsCanSentQueue(&CanBusDev[u16Way], (stCanTrsQueue *)pData);
			break;
		
		case CANBUS_MSG_DelCanSentQueue:               //���ⲿʹ��
			DelCanSentQueue(*(uint32_t *)pData, u16Way);
			break;
		
		case CANBUS_MSG_InsCanLowTrsQueue:             //���ⲿʹ��
			InsCanLowTrsQueue(u16Way, &CanBusDev[u16Way], (stCanTrsQueue *)pData);
			break;
		
		case CANBUS_MSG_DelCanLowTrsQueue:             //���ⲿʹ��
			DelCanLowTrsQueue(&CanBusDev[u16Way]);
			break;
		
		case CANBUS_MSG_InsCanHighTrsQueue:            //���ⲿʹ��
			InsCanHighTrsQueue(u16Way,&CanBusDev[u16Way], (stCanTrsQueue *)pData);
			break;
		
		case CANBUS_MSG_DelCanHighTrsQueue:            //���ⲿʹ��
			DelCanHighTrsQueue(&CanBusDev[u16Way]);
			break;		
		
		case CANBUS_MSG_GetCanRevQueue:                //���ⲿ��ȡ����������	
			temp = GetCanRevQueue(&CanBusDev[u16Way],(STR_Can_Frame*)pData);
			break;
		
		case CANBUS_MSG_CheckCanSentInterval:
			CheckCanSentInterval(u16Way,*(uint16_t*) pData);
			break;
		
		default:
			break;
	}
	return temp;
}


