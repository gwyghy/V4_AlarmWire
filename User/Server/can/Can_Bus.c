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
static uint8_t CanSendPrioFlag[CAN_DRIVER_DEV_NUM]={1,1};//优先级发送标志位
static uint8_t CanSendTimeOutFlag[CAN_DRIVER_DEV_NUM]={0,0};//超时标志位
static uint8_t CanSendAbortFlag[CAN_DRIVER_DEV_NUM]={0,0};//错误标志位
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
**函数名称：CanBusDataInit
**函数作用：canBus数据初始化
**函数参数：无
**函数输出：无
**注意事项：无
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
	
	//已发送
	CanBusDev[candev].pCanSentRead = NULL;
	CanBusDev[candev].pCanSentWrite =&CanBusDev[candev].CanSent[0];
	for(i=0 ; i<CAN_SENT_QUEUE_NUM - 1; i++)
	{
		CanBusDev[candev].CanSent[i].pNext = &CanBusDev[candev].CanSent[i+1];
	}	
	CanBusDev[candev].CanSent[CAN_SENT_QUEUE_NUM-1].pNext = NULL;		
	
	
	//低优先级发送
	CanBusDev[candev].pCanLowTrsHead = &CanBusDev[candev].CanLowTrs[0];
	CanBusDev[candev].pCanLowTrsEnd = &CanBusDev[candev].CanLowTrs[0];
	for(i=0 ; i<CAN_TX_QUEUE_NUM - 1; i++)
	{
		CanBusDev[candev].CanLowTrs[i].pNext = &CanBusDev[candev].CanLowTrs[i+1];
	}	
	CanBusDev[candev].CanLowTrs[CAN_TX_QUEUE_NUM-1].pNext = &CanBusDev[candev].CanLowTrs[0];
	
	
	//高优先级发送
	CanBusDev[candev].pCanHighTrsHead = &CanBusDev[candev].CanHighTrs[0];
	CanBusDev[candev].pCanHighTrsEnd = &CanBusDev[candev].CanHighTrs[0];
	for(i=0 ; i<CAN_TX_QUEUE_NUM - 1; i++)
	{
		CanBusDev[candev].CanHighTrs[i].pNext = &CanBusDev[candev].CanHighTrs[i+1];
	}	
	CanBusDev[candev].CanHighTrs[CAN_TX_QUEUE_NUM-1].pNext = &CanBusDev[candev].CanHighTrs[0];
		
	
	//接收
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
**函数名称：CanBusInit
**函数作用：canBus初始化
**函数参数：无
**函数输出：无
**注意事项：无
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
	
	cfg->Init(device,baud);//设置波特率
	
	
	if(initFlag == 1)
	{
		TimeoutId[0] = addTimerEvent(10,0XFF,CanSendTIMOUT_Callback);
		ErrId[0] = addTimerEvent(1,0XFF,CanSendERR_callback);
	}	
	return 1;
}

/*******************************************************************************************
**函数名称：CanBusIoCtl
**函数作用：封装Can_Driver的IoCtl
**函数参数：无
**函数输出：无
**注意事项：无
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
**函数名称：InsCanSentQueue
**函数作用：数据存入发送完成队列
**函数参数：无
**函数输出：无
**注意事项：无
*******************************************************************************************/
void InsCanSentQueue(CANBUS_DATA * pCanBus, stCanTrsQueue *SentCan)//加入已发送队列read
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
**函数名称：stCanTrsQueue
**函数作用：检查已发送队列中的CMD消息
**函数参数：无
**函数输出：无
**注意事项：无
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
**函数名称：DelCanSentQueue
**函数作用：删除已发送队列数据
**函数参数：无
**函数输出：无
**注意事项：无
*******************************************************************************************/
void DelCanSentQueue(uint32_t CMD, uint16_t u16Way)//删除已发送队列
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
**函数名称：InsCanHighTrsQueue
**函数作用：插入高优先级发送队列数据
**函数参数：无
**函数输出：无
**注意事项：无
*******************************************************************************************/
void InsCanHighTrsQueue(uint16_t u16Way, CANBUS_DATA * pCanBus, stCanTrsQueue *SentCan)//加入高优先级发送队列
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
**函数名称：DelCanHighTrsQueue
**函数作用：删除高优先级发送队列数据
**函数参数：无
**函数输出：无
**注意事项：无
*******************************************************************************************/
void DelCanHighTrsQueue(CANBUS_DATA * pCanBus)//删除高优先级送队列
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
**函数名称：InsCanHighTrsQueue
**函数作用：插入高优先级发送队列数据
**函数参数：u16Way：发送设备类型，* pCanBus：发送设备工厂类，*SentCan：数据
**函数输出：无
**注意事项：无
*******************************************************************************************/
void InsCanLowTrsQueue(uint16_t u16Way, CANBUS_DATA * pCanBus, stCanTrsQueue *SentCan)//加入低优先级发送队列
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
**函数名称：DelCanHighTrsQueue
**函数作用：删除高优先级发送队列数据
**函数参数：无
**函数输出：无
**注意事项：无
*******************************************************************************************/
void DelCanLowTrsQueue(CANBUS_DATA * pCanBus)//删除低优先级队列
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
**函数名称：InsCanRevQueue
**函数作用：加入接收队列
**函数参数：无
**函数输出：无
**注意事项：无
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
//释放信号量	
	u16Way |= 0xF0;
	OSMboxPost(CanRTxTaskSendSem,(void*)u16Way);
}

/*******************************************************************************************
**函数名称：GetCanRevQueue
**函数作用：接收队列取数
**函数参数：无
**函数输出：无
**注意事项：无
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
**函数名称：CanDriverTxDealFun
**函数作用：can驱动发送处理
**函数参数：无
**函数输出：无
**注意事项：无
*******************************************************************************************/
void CanDriverTxDealFun(uint16_t u16Way, uint16_t res)
{
	static uint8_t count = 0;
	CANBUS_DATA * pCanBus = &CanBusDev[u16Way];
	if(res == 0)                                         //发送不成功
	{
		if(count++ > 5)
		{
			count = 0;
			CanBusDev[u16Way].Cfg.IoCtl(u16Way, CANCTL_ABORT_SEND_MAIL0,&res);
			//删除
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
	
				//删除帧   jhyjhy
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
			CanSendTimeOutFlag[u16Way] = 0;		//发送完成
			startTimerEvent(TimeoutId[0]);
		}			
		else if(CanBusDev[u16Way].u16CanLowTrsCnt > 0)
		{
			CanBusDev[u16Way].Cfg.Write(u16Way,&(CanBusDev[u16Way].pCanLowTrsEnd->TrsFrame));
			CanSendTimeOutFlag[u16Way] = 0;		//发送完成
			startTimerEvent(TimeoutId[0]);
		}
		else
		{
			CanSendTimeOutFlag[u16Way] = 2;	//缓存空
			stopTimerEvent(TimeoutId[0]);
		}	
	}
}


/*******************************************************************************************
**函数名称：CanSendTIMOUT_IRQHandler
**函数作用：can数据发送超时定时器中断处理
**函数参数：无
**函数输出：无
**注意事项：无
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
				if(res == 0)                                                    //0#发送邮箱不可用
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
				if(res == 0)                                                    //0#发送邮箱不可用
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
**函数名称：CanSendERR_IRQHandler
**函数作用：can数据发送错误定时器中断处理
**函数参数：无
**函数输出：无
**注意事项：无
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
**函数名称：CanBusDealFunc
**函数作用：接口函数
**函数参数：无
**函数输出：无
**注意事项：无
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
		
		case CANBUS_MSG_DelCanSentQueue:               //供外部使用
			DelCanSentQueue(*(uint32_t *)pData, u16Way);
			break;
		
		case CANBUS_MSG_InsCanLowTrsQueue:             //供外部使用
			InsCanLowTrsQueue(u16Way, &CanBusDev[u16Way], (stCanTrsQueue *)pData);
			break;
		
		case CANBUS_MSG_DelCanLowTrsQueue:             //供外部使用
			DelCanLowTrsQueue(&CanBusDev[u16Way]);
			break;
		
		case CANBUS_MSG_InsCanHighTrsQueue:            //供外部使用
			InsCanHighTrsQueue(u16Way,&CanBusDev[u16Way], (stCanTrsQueue *)pData);
			break;
		
		case CANBUS_MSG_DelCanHighTrsQueue:            //供外部使用
			DelCanHighTrsQueue(&CanBusDev[u16Way]);
			break;		
		
		case CANBUS_MSG_GetCanRevQueue:                //供外部获取接收数据用	
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


