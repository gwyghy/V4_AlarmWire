/*
*********************************************************************************************************
*	                                            DoubleWires
*                           Commuication Controller For The Mining Industry
*
*                                  (c) Copyright 1994-2013  HNDZ 
*                       All rights reserved.  Protected by international copyright laws.
*
*
*    File    : udp_update_callback.c
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

 
#include "udp_update_callback.h"
#include "sys_msg_def.h"
#include "memory.h"
#include "crc.h"
#include "FileLoad.h"
#include "FileUpdate.h"


#if (SERVER_NETWORK_UDP_EN ==1)


u8 udpUpdateCB_UpdateStart_request(UDPFrameType *pframe)
{
	u8 addr;
	
	if((pframe==NULL)||(pframe->pdata==NULL))
		return 0;
	if(pframe->length < sizeof(UDP_UPDATE_READY))
		return 0;
	
	addr = IP_ADDR_SUPPORT(pframe->addr);
	setAppDevOnLine(pframe->header.u16FuncID.FUNC.Type,addr);
	UdpRecvFileReadyApi(pframe->pdata, pframe->length,addr);
	
	return 1;
}
//u8 tmp20=0;
//u8 tmp_test[300]={0};
u8 udpUpdateCB_SendData_request(UDPFrameType *pframe)
{
	u8 addr;
	u16 DataLength;
	u16 func=0;
	
	if((pframe==NULL)||(pframe->pdata==NULL))
		return 0;
	if(pframe->length < 4)
		return 0;
	
	memcpy((u8 *)(&DataLength),&(pframe->pdata[APPPROC_FRAME_IDLENGTH]),2);
	addr = IP_ADDR_SUPPORT(pframe->addr);
	setAppDevOnLine(pframe->header.u16FuncID.FUNC.Type,addr);
	
	//UdpLinkLayerSend_ext((u8*)(&func),2,1,0,0,UDPProcess_ID_Update,addr,1,1);
	UdpRecvFileDataApi(pframe->header.CurNum, &(pframe->pdata[APPPROC_FRAME_IDLENGTH+2]), DataLength,pframe->header.u16FuncID.FUNC.FIN);
	
//	tmp_test[tmp20] = pframe->header.CurNum;
//	tmp20++;
	return 1;
}

u8 udpUpdateCB_RecvDataAck_request(UDPFrameType *pframe)
{
	u8 addr;
	u16 CRCData;
	
	if((pframe==NULL)||(pframe->pdata==NULL))
		return 0;
	if(pframe->length < 4)
		return 0;
	
	memcpy((u8 *)(&CRCData),&(pframe->pdata[APPPROC_FRAME_IDLENGTH]),2);
	addr = IP_ADDR_SUPPORT(pframe->addr);
	setAppDevOnLine(pframe->header.u16FuncID.FUNC.Type,addr);
	
	UDPUpdate_CrcAck_API(CRCData,addr);
	
	return 1;
}

u8 udpUpdateCB_DataCRC_request(UDPFrameType *pframe)
{
	u8 addr;
	u16 CRCData;
	
	if((pframe==NULL)||(pframe->pdata==NULL))
		return 0;
	if(pframe->length < 4)
		return 0;
	
	memcpy((u8 *)(&CRCData),&(pframe->pdata[APPPROC_FRAME_IDLENGTH]),2);
	addr = IP_ADDR_SUPPORT(pframe->addr);
	setAppDevOnLine(pframe->header.u16FuncID.FUNC.Type,addr);
	
	UdpRecvFileFinCrcApi(CRCData);
	
	return 1;
}

u8 udpUpdateCB_DataCRC_confirm(UDPFrameType *pframe)
{
	u8 addr;
	u16 result;
	
	if((pframe==NULL)||(pframe->pdata==NULL))
		return 0;
	if(pframe->length < 4)
		return 0;
	
	memcpy((u8 *)(&result),&(pframe->pdata[APPPROC_FRAME_IDLENGTH]),2);
	addr = IP_ADDR_SUPPORT(pframe->addr);
	setAppDevOnLine(pframe->header.u16FuncID.FUNC.Type,addr);
	
	UDPUpdate_LastCrcAck_API(result,addr);
	
	return 1;
}

u8 udpUpdateCB_request_outtime(UDPFrameType *pframe)
{
	u8 addr;
	
	if((pframe==NULL)||(pframe->pdata==NULL))
		return 0;
	
	addr = IP_ADDR_SUPPORT(pframe->addr);
	setAppDevOffLine(pframe->header.u16FuncID.FUNC.Type,addr);
	return 1;
}

u8 udpUpdateCB_UpdateNow_request(UDPFrameType *pframe)
{
	u8 addr;
	u16 Flag;
	
	if((pframe==NULL)||(pframe->pdata==NULL))
		return 0;
	if(pframe->length < 4)
		return 0;
	
	memcpy((u8 *)(&Flag),&(pframe->pdata[APPPROC_FRAME_IDLENGTH]),2);
	addr = IP_ADDR_SUPPORT(pframe->addr);
	setAppDevOnLine(pframe->header.u16FuncID.FUNC.Type,addr);
	
	UdpRecvFileUpdateStatusApi(Flag);
	
	return 1;
}

#endif
