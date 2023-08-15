/*
*********************************************************************************************************
*	                                            DoubleWires
*                           Commuication Controller For The Mining Industry
*
*                                  (c) Copyright 1994-2013  HNDZ 
*                       All rights reserved.  Protected by international copyright laws.
*
*
*    File    : udp_datalink.c
*    Module  : user/udp
*    Version : V1.1.0.0
*    Brief   : STM32F4xx DCP
*    History : 
*   -----------------
*              Version  Date           By            Note
*              v1.0        
*
*********************************************************************************************************
 */

 
#include "udp_datalink.h"
#include "udp_app_proc.h"
#include "crc.h"
#include "memory.h"


#if (SERVER_NETWORK_UDP_EN ==1)
 
static UDPLinkLayerType UDPLinkLayerMng;
static u8 UDPLinkDeviceType=1;
/*
* function: init
*/
void UdpLinkLayerInit(void)
{
	int i;
	
	for(i=0; i<UDPProcess_ID_MAX;i++)
	{
		UDPLinkLayerMng.udpProcessFunc[i]= NULL;
	}
	UDPLinkLayerMng.udpProcessFunc[UDPProcess_ID_SupportCom] = UdpAppProcFunc;
	UDPLinkLayerMng.udpProcessFunc[UDPProcess_ID_PHONE] 	 = UdpUpdateProcFunc;
	UDPLinkLayerMng.udpProcessFunc[UDPProcess_ID_Update] 	 = UdpUpdateProcFunc;
	
	UDPLinkLayerMng.OverList.compareFunc = CompareFunc_OverList;
	UDPLinkLayerMng.OverList.freeFunc = freeFunc_OverList;
	
	chain_init(&UDPLinkLayerMng.RecvList.list, CHAIN_TYPE_APPEND, UDPLinkLayerMng.RecvList.freeFunc,UDPLinkLayerMng.RecvList.compareFunc );
	chain_init(&UDPLinkLayerMng.TrsList.list,  CHAIN_TYPE_APPEND, UDPLinkLayerMng.TrsList.freeFunc, UDPLinkLayerMng.TrsList.compareFunc );
	chain_init(&UDPLinkLayerMng.OverList.list, CHAIN_TYPE_APPEND, UDPLinkLayerMng.OverList.freeFunc,UDPLinkLayerMng.OverList.compareFunc );
	chain_init(&UDPLinkLayerMng.PollList.list, CHAIN_TYPE_APPEND, UDPLinkLayerMng.PollList.freeFunc,UDPLinkLayerMng.PollList.compareFunc );
	
	UDPLinkLayerMng.linkSN = 0;
	UdpServerInit(UdpLinkRecv);
}

void setUdpLinkParam(u8 addr0,u8 addr1,u8 addr2,u8 addr3,u8 type)
{
	UDPLinkLayerMng.linkParam.ipAddr0 = addr0;
	UDPLinkLayerMng.linkParam.ipAddr1 = addr1;
	UDPLinkLayerMng.linkParam.ipAddr2 = addr2;
	UDPLinkLayerMng.linkParam.ipAddr3 = addr3;
	UDPLinkDeviceType =type;
}

/*
* function: receive
*/
static void getUdpLinkHeader(UDPHeadType *pheader,u8 *pdata)
{
	pheader->SN  = *((u16*) pdata);
	pheader->crc = *((u16*) (pdata+2));
	pheader->u16FuncID.FuncID = *((u16*) (pdata+4));
	pheader->TotalNum  = *(pdata+6);
	pheader->CurNum    = *(pdata+7);
}

static void setUdpLinkHeader(UDPHeadType *pheader,u8 *pdata)
{
	*((u16*) pdata) = pheader->SN;
	*((u16*) (pdata+2)) = pheader->crc;
	*((u16*) (pdata+4)) = pheader->u16FuncID.FuncID;
	*(pdata+6) = pheader->TotalNum;
	*(pdata+7) = pheader->CurNum;
}

static void setUdpFrameInfo(UDPFrameType *pframe,u8 *pdata)
{
	pframe->addr = *((struct ip_addr*)pdata);
	pframe->length = *((u16*)(pdata+sizeof(struct ip_addr)));
	getUdpLinkHeader(&(pframe->header),pdata+UDP_FRAME_INFO_LEN); 
	if(pframe->length)
		pframe->pdata =  pdata+ UDP_FRAME_INFO_LEN +UDP_FRAME_HEADER_LEN;
	else
		pframe->pdata = NULL;
}

static void getUdpFrameInfo(UDPFrameType *pframe,u8 *pdata)
{
	//*((struct ip_addr*)pdata) = pframe->addr;
	memcpy(pdata,(u8 *)(&(pframe->addr)),sizeof(struct ip_addr));
	*((u16*)(pdata+sizeof(struct ip_addr))) = pframe->length;
	setUdpLinkHeader(&(pframe->header),pdata+UDP_FRAME_INFO_LEN); 
	if(pframe->length)
		memcpy(pdata+ UDP_FRAME_INFO_LEN +UDP_FRAME_HEADER_LEN , pframe->pdata,pframe->length);
}

void UdpLinkRecv(u8 *pdata, u16 length, struct ip_addr *addr)
{
	uint16_t crcval=0;
	UDPHeadType header;
	UDPFrameType frame;
	u8 * ptemp= NULL;
	
	if(length < UDP_FRAME_HEADER_LEN)
		return;
	
	getUdpLinkHeader(&header,pdata);
	if((header.u16FuncID.FUNC.Type != UDPLinkDeviceType)&&(header.u16FuncID.FUNC.Type != UDP_DEVICE_ALL))
		return;
	
	CRCCal(pdata+UDP_FRAME_CRC_OFFSET,  length-UDP_FRAME_CRC_OFFSET, (uint8_t *)(&crcval));
	if(crcval != header.crc)    //todo open ok
		return;
	
	ptemp = memory_malloc(UDP_FRAME_INFO_LEN+length);
	if(ptemp ==NULL)
		return;	

	frame.addr = *addr;
	frame.header = header;
	frame.length = length -UDP_FRAME_HEADER_LEN;
	if(frame.length)
		frame.pdata = pdata+UDP_FRAME_HEADER_LEN;
	else
		frame.pdata = NULL;
	getUdpFrameInfo(&frame,ptemp);
	
	if(addNode(&UDPLinkLayerMng.RecvList.list, (void *)ptemp) ==0)
		PLATFORM_OS_SemPost(&UDPPktRecvSem);
}

int CompareFunc_OverList(void *p1, void *p2)
{
	UDPFrameType frame1,frame2;
	u8 addr;
	
	setUdpFrameInfo(&frame1,(u8*)p1 + sizeof(UdpLinkOverType));
	setUdpFrameInfo(&frame2,(u8*)p2);
	
	if(frame1.header.SN == frame2.header.SN)
	{
		addr = ((u32)(frame1.addr.addr)) >> 24;
		if((addr == UDP_LINK_BROADCAST_ADDR)||(frame1.addr.addr == frame2.addr.addr))
		{
			return 0;
		}
	}
	return -1;
}

void freeFunc_OverList(void *p1)
{
	if(p1 !=NULL)
	{
		memory_free((void *)p1);
	}
	return ;
}

void UdpLinkLayerProcess(void)
{
	UDPFrameType frame;
	UDPFrameType *pframe;
	UDPProcType proc;
	u8 ret=0;
	u8 *pdatabuf=NULL;
		
	pdatabuf = (u8 *)chain_pop_head(&UDPLinkLayerMng.RecvList.list);
	if(pdatabuf == NULL)
		return;
	
	if(getSysInitState() == 0)
	{
		memory_free((void *)pdatabuf);
		return;
	}
	
	setUdpFrameInfo(&frame,pdatabuf);
	pframe = &frame;
	pframe->pdata = (u8 *)pdatabuf + UDP_FRAME_INFO_LEN +UDP_FRAME_HEADER_LEN;
	if(pframe->header.u16FuncID.FUNC.ACK)
	{
		deleteOne(&UDPLinkLayerMng.OverList.list, (void *)pdatabuf);
	}
	
	if(pframe->header.u16FuncID.FUNC.FuncPort < UDPProcess_ID_MAX)
	{
		proc.recvCBFun=NULL;
		ret = UDPLinkLayerMng.udpProcessFunc[pframe->header.u16FuncID.FUNC.FuncPort]((void *)&proc , pframe);
		if(ret)
		{
			if(proc.recvCBFun!=NULL)
				proc.recvCBFun(pframe);
		}
	}
	
	memory_free((void *)pdatabuf);
}

/*
* function: send
*/
u8 UdpLinkLayerSend(u8 *pdata,u16 length,u8 ack,u16 sn,u8 devtype,u8 funcport,u8 supportAddr,u8 vlan)
{
	uint16_t crcval=0;
	UDPHeadType header;
	UDPFrameType frame;
	u8 * ptemp= NULL;
	u8 currentNum=1;
	int i;
	u8 temp[UDP_FRAME_HEADER_LEN];
	u16 remainLength = length;
	u16 currentLength=0;
	u8 *pCurData=pdata;
	uint16_t crc=0;
	uint16_t crcval_temp=0;
	
	if(length > UDP_FRAME_DATA_MAX)
		return 0;
	
	memset(&header,0,sizeof(UDPHeadType));
	memset(&frame,0,sizeof(UDPFrameType));
	if(length >0)
		header.TotalNum = length/(UDP_FRAME_LENGTH_MAX-UDP_FRAME_HEADER_LEN) + (length%(UDP_FRAME_LENGTH_MAX-UDP_FRAME_HEADER_LEN))? 1:0;
	else
		header.TotalNum = 1;
	
	for(i=0 ; i<header.TotalNum ;i++)
	{
		header.CurNum = currentNum;
		header.u16FuncID.FUNC.ACK = ack;
		if(ack)
			header.SN = sn;
		else
			header.SN = UDPLinkLayerMng.linkSN;
		UDPLinkLayerMng.linkSN ++;
		
		header.u16FuncID.FUNC.Type = (devtype >=UDP_DEVICE_MAX)? UDP_DEVICE_CONTROLLER:devtype;
		header.u16FuncID.FUNC.FuncPort = funcport;
		
		if(currentNum == header.TotalNum)
			header.u16FuncID.FUNC.FIN =1;
		else
			header.u16FuncID.FUNC.FIN =0;
		header.u16FuncID.FUNC.VLAN = vlan;
			
		setUdpLinkHeader(&header,temp);
		
		if(length >0)
		{
			crcval_temp =0xffff;
			crcval = CRCCal_break(temp+UDP_FRAME_CRC_OFFSET,  UDP_FRAME_HEADER_LEN-UDP_FRAME_CRC_OFFSET,crcval_temp);
			
			if(remainLength/(UDP_FRAME_LENGTH_MAX-UDP_FRAME_HEADER_LEN) >0 )
			{
				currentLength = (UDP_FRAME_LENGTH_MAX-UDP_FRAME_HEADER_LEN);				
			}
			else
			{
				currentLength = remainLength % (UDP_FRAME_LENGTH_MAX-UDP_FRAME_HEADER_LEN);
			}
			remainLength = remainLength -currentLength;
			
			CRCCal_continue(pCurData,  currentLength,(uint8_t *)(&crc) ,crcval);
		}
		else
		{
			CRCCal(temp+UDP_FRAME_CRC_OFFSET,  UDP_FRAME_HEADER_LEN-UDP_FRAME_CRC_OFFSET,(uint8_t *)(&crc));
		}
		header.crc = crc;
		
		if(supportAddr == 0)
			IP_ADDR(&frame.addr , UDPLinkLayerMng.linkParam.ipAddr0,UDPLinkLayerMng.linkParam.ipAddr1, UDP_DEVICE_CONTROLLER, 0);
		else	
			IP_ADDR(&frame.addr , UDPLinkLayerMng.linkParam.ipAddr0,UDPLinkLayerMng.linkParam.ipAddr1, (supportAddr==UDP_LINK_BROADCAST_ADDR)?UDP_LINK_BROADCAST_ADDR:devtype,supportAddr);
		
		memcpy(&frame.header , &header,sizeof(UDPHeadType));
		frame.length = currentLength;
		frame.pdata = pCurData;
		pCurData = pCurData +currentLength;
		
		ptemp = memory_malloc(UDP_FRAME_INFO_LEN+UDP_FRAME_HEADER_LEN + currentLength);
		if(ptemp ==NULL)
			return 0;
		
		getUdpFrameInfo(&frame,ptemp);
		
		if(addNode(&UDPLinkLayerMng.TrsList.list, (void *)ptemp) ==0)
			PLATFORM_OS_SemPost(&UDPPktSendSem);		
		
		currentNum ++;
	}
	return 1;
}

u8 UdpLinkLayerSend_ext(u8 *pdata,u16 length,u8 ack,u16 sn,u8 devtype,u8 funcport,u8 supportAddr,u8 totalNum,u8 curNum)
{
	uint16_t crcval=0;
	UDPHeadType header;
	UDPFrameType frame;
	u8 * ptemp= NULL;
	u8 currentNum=1;
	u8 temp[UDP_FRAME_HEADER_LEN];
	u16 remainLength = length;
	u16 currentLength=0;
	u8 *pCurData=pdata;
	uint16_t crc=0;
	uint16_t crcval_temp=0;
	
	if(length > UDP_FRAME_DATA_MAX)
		return 0;
	
	memset(&header,0,sizeof(UDPHeadType));
	memset(&frame,0,sizeof(UDPFrameType));
	header.TotalNum = totalNum;
	header.CurNum = curNum;
	
	header.u16FuncID.FUNC.ACK = ack;
	if(ack)
		header.SN = sn;
	else
		header.SN = UDPLinkLayerMng.linkSN;
	UDPLinkLayerMng.linkSN ++;
	
	header.u16FuncID.FUNC.Type = (devtype >=UDP_DEVICE_MAX)? UDP_DEVICE_CONTROLLER:devtype;
	header.u16FuncID.FUNC.FuncPort = funcport;
	
	if(currentNum == header.TotalNum)
		header.u16FuncID.FUNC.FIN =1;
	else
		header.u16FuncID.FUNC.FIN =0;
		
	setUdpLinkHeader(&header,temp);
	
	if(length >0)
	{
		crcval_temp =0xffff;
		crcval = CRCCal_break(temp+UDP_FRAME_CRC_OFFSET,  UDP_FRAME_HEADER_LEN-UDP_FRAME_CRC_OFFSET,crcval_temp);
		
		if(remainLength/(UDP_FRAME_LENGTH_MAX-UDP_FRAME_HEADER_LEN) >0 )
		{
			currentLength = (UDP_FRAME_LENGTH_MAX-UDP_FRAME_HEADER_LEN);				
		}
		else
		{
			currentLength = remainLength % (UDP_FRAME_LENGTH_MAX-UDP_FRAME_HEADER_LEN);
		}
		remainLength = remainLength -currentLength;
		
		CRCCal_continue(pCurData,  currentLength,(uint8_t *)(&crc) ,crcval);
	}
	else
	{
		CRCCal(temp+UDP_FRAME_CRC_OFFSET,  UDP_FRAME_HEADER_LEN-UDP_FRAME_CRC_OFFSET,(uint8_t *)(&crc));
	}
	header.crc = crc;
	
	IP_ADDR(&frame.addr , UDPLinkLayerMng.linkParam.ipAddr0,UDPLinkLayerMng.linkParam.ipAddr1, (supportAddr==UDP_LINK_BROADCAST_ADDR)?UDP_LINK_BROADCAST_ADDR:devtype,supportAddr);
	memcpy(&frame.header , &header,sizeof(UDPHeadType));
	frame.length = currentLength;
	frame.pdata = pCurData;
	pCurData = pCurData +currentLength;
	
	ptemp = memory_malloc(UDP_FRAME_INFO_LEN+UDP_FRAME_HEADER_LEN + currentLength);
	if(ptemp ==NULL)
		return 0;
	
	getUdpFrameInfo(&frame,ptemp);
	
	if(addNode(&UDPLinkLayerMng.TrsList.list, (void *)ptemp) ==0)
		PLATFORM_OS_SemPost(&UDPPktSendSem);		

	return 1;
}

void UdpLinkLayerSendProcess(void)
{
	UDPFrameType *pframe=NULL;
	UDPFrameType frame;
	UDPProcType proc;
	u8 ret=0;
	u8 *ptemp;
	u8 *povertemp;
	u8 *pdatabuf;
	UdpLinkOverType overInfo;
	
	pdatabuf = (u8 *)chain_pop_head(&UDPLinkLayerMng.TrsList.list);
	if(pdatabuf == NULL)
		return;
	
	setUdpFrameInfo(&frame,pdatabuf);
	pframe = &frame;
	pframe->pdata = (u8 *)pdatabuf + UDP_FRAME_INFO_LEN +UDP_FRAME_HEADER_LEN;
	UdpServerSend((u8 *)pdatabuf+UDP_FRAME_INFO_LEN, pframe->length + UDP_FRAME_HEADER_LEN,&(pframe->addr));
	
	if(pframe->header.u16FuncID.FUNC.ACK == 0)
	{
		if(pframe->header.u16FuncID.FUNC.FuncPort < UDPProcess_ID_MAX)
		{
			ret = UDPLinkLayerMng.udpProcessFunc[pframe->header.u16FuncID.FUNC.FuncPort]((void *)&proc , pframe);
			if(ret)
			{
				if(proc.repeatNum!=0)
				{				
					povertemp = memory_malloc(UDP_FRAME_INFO_LEN+UDP_FRAME_HEADER_LEN+pframe->length+sizeof(UdpLinkOverType));
					if(povertemp !=NULL)
					{
						overInfo.overtime = proc.overtime;
						overInfo.currenttime = 0;
						overInfo.repeatNum = proc.repeatNum;
						memcpy(povertemp,&overInfo ,sizeof(UdpLinkOverType));
						
						memcpy(povertemp+sizeof(UdpLinkOverType),pframe ,UDP_FRAME_INFO_LEN+UDP_FRAME_HEADER_LEN+pframe->length);
						memcpy(povertemp+sizeof(UdpLinkOverType)+UDP_FRAME_INFO_LEN+UDP_FRAME_HEADER_LEN,pframe->pdata ,pframe->length);
						addNode(&UDPLinkLayerMng.OverList.list, (void *)povertemp);
					}
				}
			}
		}
	}
	
	memory_free((void *)pdatabuf);
}

/*
* function: overtime
*/
void UdpLinkLayerCycleProcess(u16 delay)
{
	UDPFrameType *pframe=NULL;
	UDPFrameType frame;
	UDPProcType proc;
	u8 *ptemp;
	u8 *povertemp;
	UdpLinkOverType *poverInfo;
	ChainNode *pcurNode;
	ChainNode *pdeleteNode;
	ChainNode *pPreviousNode;
	u8 ret=0;
	
	if(UDPLinkLayerMng.OverList.list.head == NULL)
		return;
	
	povertemp = UDPLinkLayerMng.OverList.list.head->data;
	pPreviousNode = pcurNode = UDPLinkLayerMng.OverList.list.head;
	while((pcurNode !=NULL)&&(povertemp !=NULL))
	{
		poverInfo = (UdpLinkOverType *)povertemp;
		poverInfo->currenttime +=delay;
		if(poverInfo->currenttime >= poverInfo->overtime)
		{
			pframe = &frame;
			setUdpFrameInfo(&frame,povertemp+sizeof(UdpLinkOverType));
			ptemp = memory_malloc(UDP_FRAME_INFO_LEN+UDP_FRAME_HEADER_LEN + pframe->length);
			if(ptemp !=NULL)
			{
				memcpy(ptemp,(u8*)pframe,UDP_FRAME_INFO_LEN+UDP_FRAME_HEADER_LEN);
				memcpy(ptemp+UDP_FRAME_INFO_LEN+UDP_FRAME_HEADER_LEN,pframe->pdata, pframe->length);
//				if(addNode(&UDPLinkLayerMng.TrsList.list, (void *)ptemp) ==0)
//					PLATFORM_OS_SemPost(&UDPPktSendSem);

				UdpServerSend((u8 *)ptemp+UDP_FRAME_INFO_LEN, pframe->length + UDP_FRAME_HEADER_LEN,&(pframe->addr));
				memory_free((void *)ptemp);
			}
						
			if(poverInfo->repeatNum >0)
				poverInfo->repeatNum --;
			poverInfo->currenttime =0;
			if(poverInfo->repeatNum ==0)
			{				
				proc.overCBFun =NULL;
				ret = UDPLinkLayerMng.udpProcessFunc[pframe->header.u16FuncID.FUNC.FuncPort]((void *)&proc , pframe);
				if(ret)
				{
					if(proc.overCBFun!=NULL)
						proc.overCBFun(pframe);
				}
				
				memory_free((void *)povertemp);
				
				pdeleteNode = pcurNode;
				if (pdeleteNode == UDPLinkLayerMng.OverList.list.head)
				{
					UDPLinkLayerMng.OverList.list.head = pdeleteNode->next;
				}
				else
				{
					pPreviousNode->next = pdeleteNode->next;
				}

				if (pdeleteNode == UDPLinkLayerMng.OverList.list.tail)
				{
					if(pPreviousNode !=pdeleteNode)
						UDPLinkLayerMng.OverList.list.tail = pPreviousNode;
					else
					{
						UDPLinkLayerMng.OverList.list.tail = NULL;
						pPreviousNode->next =NULL;
					}
				}				
								
				pcurNode = pcurNode ->next;
				freeChainNode(&UDPLinkLayerMng.OverList.list,pdeleteNode);
				povertemp = pcurNode->data;
				continue;
			}
		}
		
		pPreviousNode =pcurNode;
		pcurNode = pcurNode ->next;
		povertemp = pcurNode->data;
	}
}

#endif
