/*
*********************************************************************************************************
*	                                            V4
*                           Commuication Controller For The Mining Industry
*
*                                  (c) Copyright 1994-2013  HNDZ 
*                       All rights reserved.  Protected by international copyright laws.
*
*
*    File    : Usart_Bus.c
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
#include "Wireless_Bus.h"

#include "Util_CRC.h"

uint8_t u16UsartSendPtr = 0;
USARTBUS_DATA  UsartBusDev[USART_DRIVER_DEV_NUM];


const USART_Driver UsartDevPara[USART_DRIVER_DEV_NUM] = {
	{
		DriverUsart_Init,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
	},
};




/*******************************************************************************************
**函数名称：UsartBusDataInit
**函数作用：UsartBus数据初始化
**函数参数：无
**函数输出：无
**注意事项：无
*******************************************************************************************/
static void UsartBusDataInit(uint8_t usartdev)
{
	uint8_t i;
	#if OS_CRITICAL_METHOD == 3                      
    OS_CPU_SR  cpu_sr = 0;
#endif

	OS_ENTER_CRITICAL();
	
	memset(UsartBusDev[usartdev].UsartTrs, 0 ,sizeof(UsartBusDev[usartdev].UsartTrs));
	memset(UsartBusDev[usartdev].UsartRev, 0 ,sizeof(UsartBusDev[usartdev].UsartRev));
	memset(UsartBusDev[usartdev].UsartSent, 0 ,sizeof(UsartBusDev[usartdev].UsartSent));
	UsartBusDev[usartdev].u16UsartTrsCnt = 0;
	UsartBusDev[usartdev].u16UsartRevCnt = 0;
	UsartBusDev[usartdev].u16UsartSentCnt = 0;
	
	//已发送
	UsartBusDev[usartdev].pUsartSentRead = NULL;
	UsartBusDev[usartdev].pUsartSentWrite =&UsartBusDev[usartdev].UsartSent[0];
	for(i=0 ; i<USART_SENT_QUEUE_NUM - 1; i++)
	{
		UsartBusDev[usartdev].UsartSent[i].pNext = &UsartBusDev[usartdev].UsartSent[i+1];
	}	
	UsartBusDev[usartdev].UsartSent[USART_SENT_QUEUE_NUM-1].pNext = NULL;		
	
	//高优先级发送
	UsartBusDev[usartdev].pUsartTrsHead = &UsartBusDev[usartdev].UsartTrs[0];
	UsartBusDev[usartdev].pUsartTrsEnd = &UsartBusDev[usartdev].UsartTrs[0];
	for(i=0 ; i<USART_TX_QUEUE_NUM - 1; i++)
	{
		UsartBusDev[usartdev].UsartTrs[i].pNext = &UsartBusDev[usartdev].UsartTrs[i+1];
	}	
	UsartBusDev[usartdev].UsartTrs[USART_TX_QUEUE_NUM-1].pNext = &UsartBusDev[usartdev].UsartTrs[0];
		
	
	//接收
	UsartBusDev[usartdev].pUsartRevHead = &UsartBusDev[usartdev].UsartRev[0];
	UsartBusDev[usartdev].pUsartRevEnd =&UsartBusDev[usartdev].UsartRev[0];
	for(i=0 ; i<USART_RX_QUEUE_NUM - 1; i++)
	{
		UsartBusDev[usartdev].UsartRev[i].pNext = &UsartBusDev[usartdev].UsartRev[i+1];
	}	
	UsartBusDev[usartdev].UsartRev[USART_RX_QUEUE_NUM-1].pNext = &UsartBusDev[usartdev].UsartRev[0];
	
	OS_EXIT_CRITICAL();
		
}

uint8_t UsartOpenDriver(uint16_t driver)
{
	return 1;
}

uint8_t UsartCloseDriver(uint16_t driver)
{
	return 1;
}


/*******************************************************************************************
**函数名称：UsartBusInit
**函数作用：UsartBus初始化
**函数参数：无
**函数输出：无
**注意事项：无
*******************************************************************************************/
uint8_t UsartBusInit(uint16_t device,uint32_t baud)
{
	
	USART_Driver   *cfg;
	
	if(device >= USART_DRIVER_DEV_NUM)
	{
		return 0;
	}
	UsartBusDataInit(device);
	
	cfg = &(UsartBusDev[device].Cfg);
	memcpy(cfg,&UsartDevPara[device], sizeof(USART_Driver));
	
	cfg->Init(baud);//设置波特率
	
	return 1;
}

/*******************************************************************************************
**函数名称：UsartBusIoCtl
**函数作用：封装Usart_Driver的IoCtl
**函数参数：无
**函数输出：无
**注意事项：无
*******************************************************************************************/
uint8_t UsartBusIoCtl(uint8_t busId, uint8_t func, void *argp)
{	
	return 1;
}


/*******************************************************************************************
**函数名称：InsUsartSentQueue
**函数作用：数据存入发送完成队列
**函数参数：无
**函数输出：无
**注意事项：无
*******************************************************************************************/
void InsUsartSentQueue(USARTBUS_DATA * pUsartBus, stUsartTrsQueue *SentUsart)//加入已发送队列read
{
	stUsartTrsQueue    *pUsartSentTemp;
#if OS_CRITICAL_METHOD == 3                      
    OS_CPU_SR  cpu_sr = 0;
#endif

	OS_ENTER_CRITICAL();
	
	if(SentUsart->busHead.ack == 1 || SentUsart->busHead.count>1)
	{
		if(SentUsart->busHead.count > 0)
			SentUsart->busHead.count--;
		
		if(pUsartBus->u16UsartSentCnt <USART_SENT_QUEUE_NUM)
		{
			memcpy(&(pUsartBus->pUsartSentWrite->UsartData),&(SentUsart->UsartData),sizeof(STR_Usart_Frame));
			memcpy(&(pUsartBus->pUsartSentWrite->busHead),&(SentUsart->busHead),sizeof(USARTBUSHEAD));
			pUsartSentTemp = pUsartBus->pUsartSentWrite;
			pUsartBus->pUsartSentWrite = pUsartBus->pUsartSentWrite->pNext;
			
			pUsartSentTemp->pNext =  pUsartBus->pUsartSentRead;
			pUsartBus->pUsartSentRead = pUsartSentTemp;
			pUsartBus->u16UsartSentCnt ++;
		}
	}
	OS_EXIT_CRITICAL();
}



static uint8_t CheckUsartFrameCmd(STR_WLCAN_Frame * frame,uint32_t cmd)
{
	if((frame->u32ID.ID.FrameType & cmd) != 0)
	{
		return 1;
	}
	return 0;
}

/*******************************************************************************************
**函数名称：stUsartTrsQueue
**函数作用：检查已发送队列中的CMD消息
**函数参数：无
**函数输出：无
**注意事项：无
*******************************************************************************************/
 stUsartTrsQueue * CheckUsartSentQueue(uint32_t CMD,USARTBUS_DATA * pUsartBus)
{
	stUsartTrsQueue *pUsartSentTemp ,*pUsartSentResTemp;
	
	pUsartSentResTemp=NULL;
	pUsartSentTemp = pUsartBus->pUsartSentRead;
	if(pUsartBus ->u16UsartSentCnt ==0)
	{
		return pUsartSentResTemp;
	}
	
	do
	{
		if(CheckUsartFrameCmd((STR_WLCAN_Frame *)&(pUsartSentTemp->UsartData.SendBuf),CMD))
		{
			pUsartSentResTemp = pUsartSentTemp;
			break;
		}
		pUsartSentTemp = pUsartSentTemp->pNext;
	}while(pUsartSentTemp != NULL);
	
	return pUsartSentResTemp;
}


/*******************************************************************************************
**函数名称：DelUsartSentQueue
**函数作用：删除已发送队列数据
**函数参数：无
**函数输出：无
**注意事项：无
*******************************************************************************************/
void DelUsartSentQueue(uint32_t CMD, uint16_t u16Way)//删除已发送队列
{
	stUsartTrsQueue	*pUsartSentDelTemp = NULL;
	stUsartTrsQueue	*pUsartSentTemp = 	UsartBusDev[u16Way].pUsartSentRead;
#if OS_CRITICAL_METHOD == 3                      
    OS_CPU_SR  cpu_sr = 0;
#endif

	OS_ENTER_CRITICAL();
	pUsartSentDelTemp = CheckUsartSentQueue(CMD,&UsartBusDev[u16Way]);
	if(pUsartSentDelTemp != NULL)
	{
		if(pUsartSentTemp == pUsartSentDelTemp)
		{
			UsartBusDev[u16Way].pUsartSentRead = UsartBusDev[u16Way].pUsartSentRead ->pNext;
			UsartBusDev[u16Way].u16UsartSentCnt --;
			pUsartSentTemp->pNext = UsartBusDev[u16Way].pUsartSentWrite;
			UsartBusDev[u16Way].pUsartSentWrite = pUsartSentTemp;
		}
		else
		{		
			do
			{
				if(pUsartSentTemp -> pNext == pUsartSentDelTemp)
				{
					pUsartSentTemp -> pNext = pUsartSentDelTemp->pNext;
					pUsartSentDelTemp->pNext = UsartBusDev[u16Way].pUsartSentWrite;
					UsartBusDev[u16Way].pUsartSentWrite = pUsartSentDelTemp;
					UsartBusDev[u16Way].u16UsartSentCnt --;
					break;
				}
				pUsartSentTemp = pUsartSentTemp->pNext;
			}while(pUsartSentTemp !=NULL);
		}
	}

	OS_EXIT_CRITICAL();
}


static void CheckUsartSentInterval(uint16_t u16Way, uint16_t delay)
{
	stUsartTrsQueue    *pUsartSentTemp ,*pUsartSentProTemp;
	USARTBUS_DATA * pUsartBus = &UsartBusDev[u16Way];
#if OS_CRITICAL_METHOD == 3                      
    OS_CPU_SR  cpu_sr = 0;
#endif

	if(UsartBusDev[u16Way].u16UsartSentCnt == 0)
	{
		return;
	}	
	pUsartSentProTemp = UsartBusDev[u16Way].pUsartSentRead;
	if(pUsartSentProTemp == NULL)
	{
		UsartBusDev[u16Way].u16UsartSentCnt = 0;
		return;
	}
	pUsartSentTemp = pUsartSentProTemp ->pNext;
	if(pUsartSentTemp == NULL)
	{
		if(pUsartSentProTemp->busHead.overtime >= delay)
		{
			pUsartSentProTemp->busHead.overtime -= delay;
		}
		else
		{
			pUsartSentProTemp->busHead.overtime = 0;
		}
		
		if(pUsartSentProTemp->busHead.overtime == 0)
		{
			if(pUsartSentProTemp->busHead.count > 0)
			{
				pUsartSentProTemp->busHead.overtime = pUsartSentProTemp->busHead.backtime;
					InsUsartTrsQueue(u16Way,pUsartBus,pUsartSentProTemp);
			}
//			if(pUsartSentProTemp->busHead.count == 0 && pUsartSentProTemp->busHead.ack == 1)
//			;
			OS_ENTER_CRITICAL();
			UsartBusDev[u16Way].pUsartSentRead = NULL;
			pUsartSentProTemp->pNext = UsartBusDev[u16Way].pUsartSentWrite;
			UsartBusDev[u16Way].pUsartSentWrite = pUsartSentProTemp;
			UsartBusDev[u16Way].u16UsartSentCnt = 0;
			OS_EXIT_CRITICAL();
		}
		return ;
	}
	while(pUsartSentTemp != NULL)
	{
		if(pUsartSentTemp->busHead.overtime >= delay)
		{
			pUsartSentTemp->busHead.overtime -= delay;
		}
		else
		{
			pUsartSentTemp->busHead.overtime = 0;
		}
		
		if(pUsartSentTemp->busHead.overtime == 0)
		{
			if(pUsartSentTemp->busHead.count > 0)
			{
				pUsartSentTemp->busHead.overtime = pUsartSentTemp->busHead.backtime;
					InsUsartTrsQueue(u16Way,pUsartBus,pUsartSentTemp);
			}
//			if(pUsartSentTemp->busHead.count == 0)
//				;
			OS_ENTER_CRITICAL();
			pUsartSentProTemp -> pNext = pUsartSentTemp->pNext;
			pUsartSentTemp->pNext = UsartBusDev[u16Way].pUsartSentWrite;
			UsartBusDev[u16Way].pUsartSentWrite = pUsartSentTemp;
			UsartBusDev[u16Way].u16UsartSentCnt --;
			pUsartSentTemp = pUsartSentProTemp -> pNext;
			OS_EXIT_CRITICAL();
		}
		else
		{
			pUsartSentTemp = pUsartSentTemp->pNext;
			pUsartSentProTemp = pUsartSentProTemp ->pNext;
		} 
	}
}


/*******************************************************************************************
**函数名称：InsUsartTrsQueue
**函数作用：插入高优先级发送队列数据
**函数参数：无
**函数输出：无
**注意事项：无
*******************************************************************************************/
void InsUsartTrsQueue(uint16_t u16Way, USARTBUS_DATA * pUsartBus, stUsartTrsQueue *SentUsart)//加入高优先级发送队列
{
#if OS_CRITICAL_METHOD == 3                      
    OS_CPU_SR  cpu_sr = 0;
#endif

	if(pUsartBus->u16UsartTrsCnt == 0)
	{
		OS_ENTER_CRITICAL();
		memcpy(&(pUsartBus->pUsartTrsHead->UsartData), &SentUsart->UsartData, sizeof(STR_Usart_Frame));
		memcpy(&(pUsartBus->pUsartTrsHead->busHead), &SentUsart->busHead, sizeof(USARTBUSHEAD));
		pUsartBus->pUsartTrsHead = pUsartBus->pUsartTrsHead->pNext;
		pUsartBus->u16UsartTrsCnt ++;
		OS_EXIT_CRITICAL();
		
		USART_SendData(USART1,pUsartBus->pUsartTrsEnd->UsartData.Address);
		USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
	}
	else
	{
		if(UsartBusDev[u16Way].u16UsartTrsCnt >= USART_TX_QUEUE_NUM)
		{
			return;		
		}
		OS_ENTER_CRITICAL();
		memcpy(&(pUsartBus->pUsartTrsHead->UsartData), &SentUsart->UsartData, sizeof(STR_Usart_Frame));
		memcpy(&(pUsartBus->pUsartTrsHead->busHead), &SentUsart->busHead, sizeof(USARTBUSHEAD));		
		pUsartBus->pUsartTrsHead = pUsartBus->pUsartTrsHead->pNext;
		pUsartBus->u16UsartTrsCnt ++;
		OS_EXIT_CRITICAL();
	}
}

/*******************************************************************************************
**函数名称：DelUsartTrsQueue
**函数作用：删除高优先级发送队列数据
**函数参数：无
**函数输出：无
**注意事项：无
*******************************************************************************************/
void DelUsartTrsQueue(USARTBUS_DATA * pUsartBus)//删除高优先级送队列
{
	uint8_t u16Way;
#if OS_CRITICAL_METHOD == 3                      
    OS_CPU_SR  cpu_sr = 0;
#endif

	OS_ENTER_CRITICAL();
	if(pUsartBus->u16UsartTrsCnt)
	{
		if(pUsartBus->pUsartTrsEnd->busHead.count > 1)
		{
			InsUsartSentQueue(pUsartBus, pUsartBus->pUsartTrsEnd);
		}
		pUsartBus->u16UsartTrsCnt --;
		pUsartBus->pUsartTrsEnd = pUsartBus->pUsartTrsEnd->pNext;	
	}
	OS_EXIT_CRITICAL();
}

/*******************************************************************************************
**函数名称：InsUsartRevQueue
**函数作用：加入接收队列
**函数参数：无
**函数输出：无
**注意事项：无
*******************************************************************************************/
void InsUsartRevQueue(uint16_t u16Way, STR_Usart_Frame* RevBufTemp)
{
	#if OS_CRITICAL_METHOD == 3                      
    OS_CPU_SR  cpu_sr = 0;
	#endif
	OS_ENTER_CRITICAL();	
	if(UsartBusDev[u16Way].u16UsartRevCnt < USART_RX_QUEUE_NUM)
	{
		memcpy(&UsartBusDev[u16Way].pUsartRevHead->UsartData, RevBufTemp, sizeof(STR_Usart_Frame));
		UsartBusDev[u16Way].pUsartRevHead = UsartBusDev[u16Way].pUsartRevHead->pNext;
		UsartBusDev[u16Way].u16UsartRevCnt ++;
	}
	OS_EXIT_CRITICAL();
//释放信号量	
	u16Way |= 0xF0;
	OSMboxPost(UsartRTxTaskSendSem,(void*)u16Way);
}

/*******************************************************************************************
**函数名称：GetUsartRevQueue
**函数作用：接收队列取数
**函数参数：无
**函数输出：无
**注意事项：无
*******************************************************************************************/
uint8_t GetUsartRevQueue(USARTBUS_DATA * pUsartBus, STR_Usart_Frame * RevBufTemp)
{
	uint8_t res = 1;
	#if OS_CRITICAL_METHOD == 3                      
    OS_CPU_SR  cpu_sr = 0;
	#endif
	OS_ENTER_CRITICAL();
		
	if(pUsartBus->u16UsartRevCnt > 0)
	{
		memcpy(RevBufTemp, &(pUsartBus->pUsartRevEnd->UsartData), sizeof(STR_Usart_Frame));
		pUsartBus->pUsartRevEnd = pUsartBus->pUsartRevEnd->pNext;
		pUsartBus->u16UsartRevCnt --;
	}
	else
		res = 0;
	
	OS_EXIT_CRITICAL();
	return res;
}

/*******************************************************************************************
**函数名称：UsartDriverTxDealFun
**函数作用：Usart驱动发送处理
**函数参数：无
**函数输出：无
**注意事项：无
*******************************************************************************************/
void UsartDriverTxDealFun(void)
{
	//usart发送
	if ((USART1->SR & USART_FLAG_TXE) != 0)//数据移位完成将新数据存入
	{
		u16UsartSendPtr++;
		if(u16UsartSendPtr == (sizeof(STR_Usart_Frame)-1))//发送一包中最后一个数
		{
			USART_SendData(USART1,UsartBusDev[0].pUsartTrsEnd->UsartData.CRCLow);
			USART_ITConfig(USART1, USART_IT_TXE, DISABLE); //关闭USART_IT_TXE中断
			USART_ClearFlag(USART1, USART_FLAG_TC); 
			USART_ITConfig(USART1, USART_IT_TC, ENABLE);//USART_IT_TC中断
		}
		else if(u16UsartSendPtr < (sizeof(STR_Usart_Frame)-1))
		{
			if(u16UsartSendPtr == (UsartBusDev[0].pUsartTrsEnd->UsartData.u8DLC + 5))
				u16UsartSendPtr = sizeof(STR_Usart_Frame) - 2;
			USART_SendData(USART1, *((u8*)&UsartBusDev[0].pUsartTrsEnd->UsartData+u16UsartSendPtr));
			USART_ClearFlag(USART1, USART_FLAG_TC); //清除USART_FLAG_TC中断标志
		}			
	}
	if ((USART1->SR & USART_FLAG_TC) != 0)
	{		
		if (u16UsartSendPtr >= (sizeof(STR_Usart_Frame)-1))//最后一个数据发送完成
		{
			USART_ClearFlag(USART1, USART_FLAG_TC); //清除USART_FLAG_TC中断标志
			USART_ITConfig(USART1, USART_IT_TC, DISABLE);	//USART_IT_TC中断
			u16UsartSendPtr = 0;
			UsartBusDealFunc(0,USARTBUS_MSG_DelUsartTrsQueue,NULL);
			
			if(UsartBusDev[0].u16UsartTrsCnt>0)
			{
				USART_SendData(USART1,UsartBusDev[0].pUsartTrsEnd->UsartData.Address);
				USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
			}
		}
	}
}


/***************************************************************************************
** 功能描述：发送应答数据
** 函数输入：无
** 函数输出：None
** 注意事项：
******************************************************************************************/
void SendWLResponseData(u8 Serial , u8 Data,u8 ft)
{
	u16 u16CrcTemp = 0;
	stUsartTrsQueue queue;
	
	queue.UsartData.ACK = 0;
	queue.UsartData.Address = 0;
	queue.UsartData.FrameType = ft;
	queue.UsartData.Serial = Serial;
	queue.UsartData.u8DLC = 1;
	queue.UsartData.SendBuf[0] = 1;
	Crc16Ccitt(&queue.UsartData.Address,6,&u16CrcTemp);	
	queue.UsartData.CRCHigh = (u16CrcTemp>>8);
	queue.UsartData.CRCLow = u16CrcTemp&0xFF;
	
	queue.busHead.ack = 0;
	queue.busHead.backtime = 0;
	queue.busHead.count = 0;
	queue.busHead.overtime = 0;
//	UsartBusDealFunc(0,USARTBUS_MSG_InsUsartTrsQueue,&queue);	
}


/*******************************************************************************************
**函数名称：UsartRecvDataProc
**函数作用：Usart驱动接收处理
**函数参数：无
**函数输出：无
**注意事项：无
*******************************************************************************************/

void UsartRecvDataProc(uint8_t *data,uint8_t len)
{
	u16 u16CrcTemp = 0x00;
	Crc16Ccitt(data,data[4]+5,&u16CrcTemp);	
		
 	if(data[data[4]+5] == (u8)((u16CrcTemp&0xFF00)>>8)  		
		   && data[data[4]+6] ==  (u8)(u16CrcTemp&0x00FF))
	{
		InsUsartRevQueue(0,(STR_Usart_Frame *)data);
		if(data[3] == 1)
		{
			SendWLResponseData(data[1],1,data[2]);
		}
	}
	else
	{
		u16CrcTemp = 0x00;
	}
}

/*******************************************************************************************
**函数名称：UsartBusDealFunc
**函数作用：接口函数
**函数参数：无
**函数输出：无
**注意事项：无
*******************************************************************************************/
uint8_t UsartBusDealFunc(uint16_t u16Way, USARTBUS_MSG msg, void * pData)
{
	uint8_t temp = 1;
	
	if(msg > USARTBUS_MSG_MAX)
	{
		return 0;
	}
	if(u16Way > 3)
		return 0;
	switch(msg)
	{
		case USARTBUS_MSG_InsUsartSentQueue:
			InsUsartSentQueue(&UsartBusDev[u16Way], (stUsartTrsQueue *)pData);
			break;
		
		case USARTBUS_MSG_DelUsartSentQueue:               //供外部使用
			DelUsartSentQueue(*(uint32_t *)pData, u16Way);
			break;
		
		case USARTBUS_MSG_InsUsartTrsQueue:            //供外部使用
			InsUsartTrsQueue(u16Way,&UsartBusDev[u16Way], (stUsartTrsQueue *)pData);
			break;
		
		case USARTBUS_MSG_DelUsartTrsQueue:            //供外部使用
			DelUsartTrsQueue(&UsartBusDev[u16Way]);
			break;		
		
		case USARTBUS_MSG_GetUsartRevQueue:                //供外部获取接收数据用	
			temp = GetUsartRevQueue(&UsartBusDev[u16Way],(STR_Usart_Frame*)pData);
			break;
		
		case USARTBUS_MSG_CheckUsartSentInterval:
			CheckUsartSentInterval(u16Way,*(uint16_t*) pData);
			break;
		
		default:
			break;
	}
	return temp;
}


