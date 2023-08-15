/*
*********************************************************************************************************
*	                                            DoubleWires
*                           Commuication Controller For The Mining Industry
*
*                                  (c) Copyright 1994-2013  HNDZ 
*                       All rights reserved.  Protected by international copyright laws.
*
*
*    File    : udp_app_callback.c
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

 
#include "udp_app_callback.h"
#include "sys_msg_def.h"
#include "memory.h"
#include "crc.h"
#include "Util_Timer.h"
#include "ServerMenuConfig.h"
#include "ProjectConfig.h"
#if (SERVER_NETWORK_UDP_EN ==1)

extern AppProcInfoType AppProcData;
extern void setVlanBcm(u8 vlanMode);

//--------------------------read state--------------------------
u8 udpAppCB_readState_request(UDPFrameType *pframe)
{
	u8 type;
	u8 id;
	u16 length =0;
	u16 offset =0;
	u8 *pObjectData=NULL;
	u16 retLen=0;
	u8 addr;
	u16 funcid;
	
	if((pframe==NULL)||(pframe->pdata==NULL))
		return 0;
	if((pframe->length < (APPPROC_FRAME_IDLENGTH +2)) || (pframe->length >(APPPROC_FRAME_IDLENGTH + 6)))
		return 0;
		
	memcpy(&funcid,pframe->pdata,2);
	type = pframe->pdata[APPPROC_FRAME_IDLENGTH];
	id   = pframe->pdata[APPPROC_FRAME_IDLENGTH+1];
	if((pframe->length > (APPPROC_FRAME_IDLENGTH +2))&& (pframe->length <=(APPPROC_FRAME_IDLENGTH + 6)))
	{
		memcpy(&length , &(pframe->pdata[APPPROC_FRAME_IDLENGTH+2]),2);
		memcpy(&offset , &(pframe->pdata[APPPROC_FRAME_IDLENGTH+4]),2);
	}
	
	SysDataFun(DATA_MSG_READ_OBJECT,type,id,length,offset,&pObjectData,&retLen);
	if((pObjectData ==NULL)||(retLen==0))
		return 0;
	
	addr = IP_ADDR_SUPPORT(pframe->addr);
	setAppDevOnLine(pframe->header.u16FuncID.FUNC.Type,addr);
	if(funcid==AppProcIDType_readState_request)		
		UdpLinkLayerSend_app(pObjectData,retLen,1,pframe->header.SN ,addr,AppProcIDType_readState_confirm);
	else if(funcid==AppProcIDType_getParam_request)		
		UdpLinkLayerSend_app(pObjectData,retLen,1,pframe->header.SN ,addr,AppProcIDType_getParam_confirm);	
	
	return 1;
}

u8 udpAppCB_request_outtime(UDPFrameType *pframe)
{
	u8 addr;
	
	if((pframe==NULL)||(pframe->pdata==NULL))
		return 0;
	
	addr = IP_ADDR_SUPPORT(pframe->addr);
	setAppDevOffLine(pframe->header.u16FuncID.FUNC.Type,addr);
	return 1;
}

u8 udpAppCB_readState_confirm(UDPFrameType *pframe)
{
	u8 type;
	u8 id;
	u16 length =0;
	u16 offset =0;
	u8 *pObjectData=NULL;
	u8 addr;
	u16 funcid;
	
	if((pframe==NULL)||(pframe->pdata==NULL))
		return 0;
	if(pframe->length <(APPPROC_FRAME_IDLENGTH + 6))
		return 0;
		
	memcpy(&funcid,pframe->pdata,2);
	type = pframe->pdata[APPPROC_FRAME_IDLENGTH];
	id   = pframe->pdata[APPPROC_FRAME_IDLENGTH+1];
	memcpy(&length , &(pframe->pdata[APPPROC_FRAME_IDLENGTH+2]),2);
	memcpy(&offset , &(pframe->pdata[APPPROC_FRAME_IDLENGTH+4]),2);
	pObjectData = &(pframe->pdata[APPPROC_FRAME_IDLENGTH+6]);
	addr = IP_ADDR_SUPPORT(pframe->addr);
	
	if(funcid==AppProcIDType_readState_confirm)
		DeviceMngDataFun(DEVDATA_MSG_OBJECT,addr,type,id,length,offset,pObjectData);
	else if(funcid==AppProcIDType_getParam_confirm)
		DeviceMngDataFun(DEVDATA_MSG_PARAM,addr,type,id,length,offset,pObjectData);
	setAppDevOnLine(pframe->header.u16FuncID.FUNC.Type,addr);
	
	return 1;
}

u8 udpAppCB_readState_multiple_request(UDPFrameType *pframe)
{
	u8 type;
	u8 startid,endid;
	u16 length =0;
	u16 offset =0;
	u8 *pObjectData=NULL;
	u16 retLen=0;
	u8 addr;
	u8 i;
	u8 *ptemp =NULL;
	u16 tempLen=0;
	u16 funcid;
	
	if((pframe==NULL)||(pframe->pdata==NULL))
		return 0;
	if((pframe->length < (APPPROC_FRAME_IDLENGTH +3)) || (pframe->length >(APPPROC_FRAME_IDLENGTH + 7)))
		return 0;
		
	memcpy(&funcid,pframe->pdata,2);
	type = pframe->pdata[APPPROC_FRAME_IDLENGTH];
	startid   = pframe->pdata[APPPROC_FRAME_IDLENGTH+1];
	endid   = pframe->pdata[APPPROC_FRAME_IDLENGTH+2];
	if((pframe->length > (APPPROC_FRAME_IDLENGTH +3))&& (pframe->length <=(APPPROC_FRAME_IDLENGTH + 7)))
	{
		memcpy(&length , &(pframe->pdata[APPPROC_FRAME_IDLENGTH+3]),2);
		memcpy(&offset , &(pframe->pdata[APPPROC_FRAME_IDLENGTH+5]),2);
	}
	
	if(startid>endid)
		return 0;
	
	addr = IP_ADDR_SUPPORT(pframe->addr);
	setAppDevOnLine(pframe->header.u16FuncID.FUNC.Type,addr);
			
	ptemp = memory_malloc(APPPROC_USER_FRAME_LEN);
	if(ptemp ==NULL)
		return 0;

	
	tempLen = 5;
	
	for(i=startid; i<=endid; i++)
	{
		SysDataFun(DATA_MSG_READ_OBJECT,type,i,length,offset,&pObjectData,&retLen);
		if((pObjectData !=NULL)&&(retLen!=0))
		{
			if((tempLen + retLen)<=APPPROC_USER_FRAME_LEN)
			{
				memcpy(ptemp+tempLen,pObjectData,retLen);
				tempLen += retLen;
			}
			else if(tempLen>0)
			{
				if(funcid==AppProcIDType_readState_multiple_request)	
					UdpLinkLayerSend_app(ptemp,tempLen,1,pframe->header.SN ,addr,AppProcIDType_readState_multiple_confirm);
				else if(funcid==AppProcIDType_getParam_multiple_request)	
					UdpLinkLayerSend_app(ptemp,tempLen,1,pframe->header.SN ,addr,AppProcIDType_getParam_multiple_confirm);
				tempLen =0;
				memcpy(ptemp+tempLen,pObjectData,retLen);
				tempLen += retLen;
			}
		}
	}
	
	ptemp[0] = type;
	ptemp[1] = startid;
	ptemp[2] = endid;
	ptemp[3] = retLen;
	ptemp[4] = 0;
	
	if(tempLen>0)
	{
		if(funcid==AppProcIDType_readState_multiple_request)	
			UdpLinkLayerSend_app(ptemp,tempLen,1,pframe->header.SN ,addr,AppProcIDType_readState_multiple_confirm);
		else if(funcid==AppProcIDType_getParam_multiple_request)	
			UdpLinkLayerSend_app(ptemp,tempLen,1,pframe->header.SN ,addr,AppProcIDType_getParam_multiple_confirm);
		tempLen =0;
	}
	memory_free((void *)ptemp);
	
	return 1;
}


u8 udpAppCB_ykqDecode_request(UDPFrameType *pframe)
{
	u8 tmp[3];
	if((pframe==NULL)||(pframe->pdata==NULL))
		return 0;
	if(pframe->length <(APPPROC_FRAME_IDLENGTH + 5))
		return 0;
	memcpy(&tmp,&pframe->pdata[2],3);
	Wireless_SCnet_Decode_proc(tmp);
	return 1;
}


u8 udpAppCB_arpNetMaintain_request(UDPFrameType *pframe)
{
	u8 addr;
	u8 temp[10] = {0};
	if((pframe==NULL)||(pframe->pdata==NULL))
		return 0;
	
	addr = IP_ADDR_SUPPORT(pframe->addr);
	UdpLinkLayerSend_app(temp,4,0,pframe->header.SN ,addr,AppProcIDType_arpNetMaintain_confirm);
	return 1;
}


u8 udpAppCB_readState_multiple_confirm(UDPFrameType *pframe)
{
	u8 type;
	u8 startid,endid;
	u16 length =0;
	u16 offset =0;
	u8 *pObjectData=NULL;
	u8 addr;
	u8 i;
	u16 funcid;
	
	if((pframe==NULL)||(pframe->pdata==NULL))
		return 0;
	if(pframe->length <(APPPROC_FRAME_IDLENGTH + 7))
		return 0;
		
	memcpy(&funcid,pframe->pdata,2);
	type = pframe->pdata[APPPROC_FRAME_IDLENGTH];
	startid   = pframe->pdata[APPPROC_FRAME_IDLENGTH+1];
	endid   = pframe->pdata[APPPROC_FRAME_IDLENGTH+2];
	
	memcpy(&length , &(pframe->pdata[APPPROC_FRAME_IDLENGTH+3]),2);
	memcpy(&offset , &(pframe->pdata[APPPROC_FRAME_IDLENGTH+5]),2);
	pObjectData = &(pframe->pdata[APPPROC_FRAME_IDLENGTH+7]);
	addr = IP_ADDR_SUPPORT(pframe->addr);
	setAppDevOnLine(pframe->header.u16FuncID.FUNC.Type,addr);
	
	if(startid>endid)
		return 0;
	
	if((pframe->length -APPPROC_FRAME_IDLENGTH -7) < (endid-startid+1)*length)
		return 0;
	
	for(i=startid; i<=endid; i++)
	{
		if(funcid==AppProcIDType_readState_multiple_confirm)
			DeviceMngDataFun(DEVDATA_MSG_OBJECT,addr,type,i,length,offset,pObjectData+length*(i-startid));
		else if(funcid==AppProcIDType_getParam_multiple_confirm)
			DeviceMngDataFun(DEVDATA_MSG_PARAM,addr,type,i,length,offset,pObjectData+length*(i-startid));
	}
		
	return 1;
}

u8 udpAppCB_stateReport_indication(UDPFrameType *pframe)
{
	u8 StateType;
	u8 DeviceType,id;
	u16 length =0;
	u8 *pObjectData=NULL;
	u8 addr;
	
	if((pframe==NULL)||(pframe->pdata==NULL))
		return 0;
	if(pframe->length <(APPPROC_FRAME_IDLENGTH +5))
		return 0;
		
	StateType = pframe->pdata[APPPROC_FRAME_IDLENGTH];
	DeviceType= pframe->pdata[APPPROC_FRAME_IDLENGTH+1];
	id = pframe->pdata[APPPROC_FRAME_IDLENGTH+2];
	memcpy(&length , &(pframe->pdata[APPPROC_FRAME_IDLENGTH+3]),2);
	if(length>0)
		pObjectData = &(pframe->pdata[APPPROC_FRAME_IDLENGTH+5]);
	addr = IP_ADDR_SUPPORT(pframe->addr);
	
	DeviceMngDataFun((DevDataMsg_t)((u8)DEVDATA_MSG_STATE_ACTION+StateType),addr,DeviceType,id,length,0,pObjectData);
	setAppDevOnLine(pframe->header.u16FuncID.FUNC.Type,addr);
	
	return 1;
}

//--------------------------control device--------------------------
u8 udpAppCB_controlDevice_request(UDPFrameType *pframe)
{
	u8 type;
	u8 id;
	u8 ControlParamLength;
	u8 *pObjectData=NULL;
	u8 addr;
	int ret;
	u8 temp[4];
	
	if((pframe==NULL)||(pframe->pdata==NULL))
		return 0;
	if(pframe->length <(APPPROC_FRAME_IDLENGTH + 5))
		return 0;
		
	type = *(pframe->pdata +APPPROC_FRAME_IDLENGTH);
	id   = *(pframe->pdata +APPPROC_FRAME_IDLENGTH+1);
	ControlParamLength = *(pframe->pdata +APPPROC_FRAME_IDLENGTH+4);	
	pObjectData = (pframe->pdata +APPPROC_FRAME_IDLENGTH+2);
	addr = IP_ADDR_SUPPORT(pframe->addr);
	
	ret = SysDataFun(DATA_MSG_CTRL,type,id,pframe->length-APPPROC_FRAME_IDLENGTH-2,0,pObjectData,0);
	setAppDevOnLine(pframe->header.u16FuncID.FUNC.Type,addr);
	
	temp[0] = type;
	temp[1] = id;
	temp[2] = (ret >0)? 1:0;
	if(ret > 0)
		UdpLinkLayerSend_app(temp,3,1,pframe->header.SN ,addr,AppProcIDType_control_confirm);
	else
	{
		temp[3] = (u8)ret;
		UdpLinkLayerSend_app(temp,4,1,pframe->header.SN ,addr,AppProcIDType_control_confirm);
	}
	
	return 1;
}

u8 udpAppCB_request_ack(UDPFrameType *pframe)
{
	u8 addr;
	
	if((pframe==NULL)||(pframe->pdata==NULL))
		return 0;
	
	addr = IP_ADDR_SUPPORT(pframe->addr);
	setAppDevOnLine(pframe->header.u16FuncID.FUNC.Type,addr);
	return 1;
}

u8 udpAppCB_manualControlDevice_request(UDPFrameType *pframe)
{
	u8 Length;
	u8 addr;
	
	if((pframe==NULL)||(pframe->pdata==NULL))
		return 0;
	if(pframe->length <(APPPROC_FRAME_IDLENGTH + 1))
		return 0;
		
	Length = pframe->length-APPPROC_FRAME_IDLENGTH;
	memset(ManualActionClear,0,MANUALACTIONINFO_LEN);
	if(Length<MANUALACTIONINFO_LEN)
		memcpy(ManualActionClear,pframe->pdata +APPPROC_FRAME_IDLENGTH,Length);

	addr = IP_ADDR_SUPPORT(pframe->addr);
	ManualActionSupportNum = addr;
	setAppDevOnLine(pframe->header.u16FuncID.FUNC.Type,addr);
	
	return 1;
}

u8 udpAppCB_writeStateDevice_request(UDPFrameType *pframe)
{
	u8 Length;
	u8 addr;
	uint16_t len=0;
	if((pframe==NULL)||(pframe->pdata==NULL))
		return 0;
	if(pframe->length <(APPPROC_FRAME_IDLENGTH + 1))
		return 0;
	NetworkHeartbeatTimesZero();
	Length = pframe->length-APPPROC_FRAME_IDLENGTH;
	memset(ManualActionClear,0,MANUALACTIONINFO_LEN);
	if(Length<MANUALACTIONINFO_LEN)
		memcpy(ManualActionClear,pframe->pdata +APPPROC_FRAME_IDLENGTH,Length);

	addr = IP_ADDR_SUPPORT(pframe->addr);
	
	setAppDevOnLine(pframe->header.u16FuncID.FUNC.Type,addr);
	
	
	if(pframe->pdata[APPPROC_FRAME_IDLENGTH+2] > 10)
		return 0;	
	len = (u8 *)(&SysState.globalState.DirectionOperation)-(u8 *)(&SysState.globalState.emergencyState);
	SysDataFun(DATA_MSG_WRITE_OBJECT,ENUM_SYSTEM,0,pframe->pdata[APPPROC_FRAME_IDLENGTH+2],len/2+pframe->pdata[APPPROC_FRAME_IDLENGTH+3],&pframe->pdata[APPPROC_FRAME_IDLENGTH+4],&len);
		
	return 1;
}
extern uint16_t FollowButton[];
u8  NumberOfOperations;
u8 udpAppCB_followControlDevice_confirm(UDPFrameType *pframe)
{	
	u8 addr;
	if((pframe==NULL)||(pframe->pdata==NULL))
		return 0;
	if(pframe->length < APPPROC_FRAME_IDLENGTH)
		return 0;

	addr = IP_ADDR_SUPPORT(pframe->addr);
	setAppDevOnLine(pframe->header.u16FuncID.FUNC.Type,addr);
	
	NumberOfOperations = pframe->pdata[APPPROC_FRAME_IDLENGTH];
	if(NumberOfOperations > 6)
		NumberOfOperations = 6;
	if(NumberOfOperations == 0)
		NumberOfOperations = 0xFF;
	memcpy(FollowButton,&pframe->pdata[APPPROC_FRAME_IDLENGTH+1],NumberOfOperations*2);
	return 1;
}
u16  RecoilState;
u16  RecoilNumber;
u8 udpAppCB_recoilControlDevice_confirm(UDPFrameType *pframe)
{	
	u8 addr;
	if((pframe==NULL)||(pframe->pdata==NULL))
		return 0;
	if(pframe->length < APPPROC_FRAME_IDLENGTH)
		return 0;

	addr = IP_ADDR_SUPPORT(pframe->addr);
	setAppDevOnLine(pframe->header.u16FuncID.FUNC.Type,addr);
	memcpy(&RecoilState,&pframe->pdata[APPPROC_FRAME_IDLENGTH],2);
	memcpy(&RecoilNumber,&pframe->pdata[APPPROC_FRAME_IDLENGTH+2],2);
//	RecoilState = pframe->pdata[APPPROC_FRAME_IDLENGTH];
//	RecoilNumber = pframe->pdata[APPPROC_FRAME_IDLENGTH+1];
	return 1;
}
uint16_t transportplane[4];
u8 udpAppCB_TransportPlane_request(UDPFrameType *pframe)
{	
	u8 addr;
	u8 devtype, len; 
	if((pframe==NULL)||(pframe->pdata==NULL))
		return 0;
	if(pframe->length < APPPROC_FRAME_IDLENGTH)
		return 0;

	addr = IP_ADDR_SUPPORT(pframe->addr);
	setAppDevOnLine(pframe->header.u16FuncID.FUNC.Type,addr);
	
	memcpy(&devtype,&pframe->pdata[APPPROC_FRAME_IDLENGTH],2);
	if(devtype == 5)//后溜
	{		
		memcpy(&len,&pframe->pdata[APPPROC_FRAME_IDLENGTH+2],2);		
		memcpy(&transportplane,&pframe->pdata[APPPROC_FRAME_IDLENGTH+4],len);
	}
	if(devtype == 3)//前溜
	{		
		memcpy(&len,&pframe->pdata[APPPROC_FRAME_IDLENGTH+2],2);	
		//从这开始写
	}
	return 1;
}
//-------------------------- param --------------------------
u8 udpAppCB_setParam_request(UDPFrameType *pframe)
{
	u8 type;
	u8 id;
	u16 length =0;
	u16 offset =0;
	u8 addr;
	u16 funcid;
	int ret;
	u8 temp[4];
	
	if((pframe==NULL)||(pframe->pdata==NULL))
		return 0;
	if(pframe->length <(APPPROC_FRAME_IDLENGTH + 6))
		return 0;
		
	memcpy(&funcid,pframe->pdata,2);
	type = pframe->pdata[APPPROC_FRAME_IDLENGTH];
	id   = pframe->pdata[APPPROC_FRAME_IDLENGTH+1];
	memcpy(&length , &(pframe->pdata[APPPROC_FRAME_IDLENGTH+2]),2);
	memcpy(&offset , &(pframe->pdata[APPPROC_FRAME_IDLENGTH+4]),2);
	
	ret = SysDataFun(DATA_MSG_WRITE_OBJECT_PARAM,type,id,length/2,offset,&(pframe->pdata[APPPROC_FRAME_IDLENGTH+6]),0);
	
	addr = IP_ADDR_SUPPORT(pframe->addr);
	setAppDevOnLine(pframe->header.u16FuncID.FUNC.Type,addr);
	
	temp[0] = type;
	temp[1] = id;
	temp[2] = (ret > 0)? 1:0;
	if(ret > 0)
	{
		UdpLinkLayerSend_app(temp,3,1,pframe->header.SN ,addr,AppProcIDType_setParam_confirm);			
	}
	else
	{
		temp[3] = (u8)ret;
		UdpLinkLayerSend_app(temp,4,1,pframe->header.SN ,addr,AppProcIDType_setParam_confirm);
	}
	
	return 1;
}

u8 udpAppCB_setParam_multiple_request(UDPFrameType *pframe)
{
	u8 type;
	u8 startid,endid;
	u16 length =0;
	u16 offset =0;
	u8 addr;
	u16 funcid;
	int ret;
	u8 i;
	u8 temp[5];
	u8 flag=0;
	u8 *pObjectData=NULL;
	
	if((pframe==NULL)||(pframe->pdata==NULL))
		return 0;
	if(pframe->length <(APPPROC_FRAME_IDLENGTH + 7))
		return 0;
		
	memcpy(&funcid,pframe->pdata,2);
	type = pframe->pdata[APPPROC_FRAME_IDLENGTH];
	startid = pframe->pdata[APPPROC_FRAME_IDLENGTH+1];
	endid   = pframe->pdata[APPPROC_FRAME_IDLENGTH+2];
	memcpy(&length , &(pframe->pdata[APPPROC_FRAME_IDLENGTH+3]),2);
	memcpy(&offset , &(pframe->pdata[APPPROC_FRAME_IDLENGTH+5]),2);	
	
	addr = IP_ADDR_SUPPORT(pframe->addr);
	setAppDevOnLine(pframe->header.u16FuncID.FUNC.Type,addr);
	
	if(startid>endid)
		return 0;
	
	if((pframe->length -APPPROC_FRAME_IDLENGTH -7) < (endid-startid+1)*length)
		return 0;
	
	pObjectData = &(pframe->pdata[APPPROC_FRAME_IDLENGTH+7]);
	temp[0] = type;
	temp[1] = startid;
	temp[2] = endid;
	temp[3] = 1;
	for(i=startid; i<=endid; i++)
	{
		ret = SysDataFun(DATA_MSG_WRITE_OBJECT_PARAM,type,i,length/2,offset,pObjectData+length*(i-startid),0);
		if(ret<=0)
		{
			temp[3] = 0;
			temp[4] = (u8)ret;
			flag=1;
			break;
		}
	}
	
	if(flag)
		UdpLinkLayerSend_app(temp,5,1,pframe->header.SN ,addr,AppProcIDType_setParam_multiple_confirm);
	else
		UdpLinkLayerSend_app(temp,4,1,pframe->header.SN ,addr,AppProcIDType_setParam_multiple_confirm);
	
	return 1;
}

u8 udpAppCB_checkParam_request(UDPFrameType *pframe)
{
	u8 type;
	u8 startid,endid;
	u8 addr;
	int ret;
	u8 i;
	u8 temp[5];
	u8 *pObjectData=NULL;
	u16 retLen;
	uint16_t crcval=0xffff;
	
	if((pframe==NULL)||(pframe->pdata==NULL))
		return 0;
	if(pframe->length < APPPROC_FRAME_IDLENGTH)
		return 0;

	addr = IP_ADDR_SUPPORT(pframe->addr);
	setAppDevOnLine(pframe->header.u16FuncID.FUNC.Type,addr);
	
	if(pframe->length >=5)
	{
		type = pframe->pdata[APPPROC_FRAME_IDLENGTH];
		startid = pframe->pdata[APPPROC_FRAME_IDLENGTH+1];
		endid   = pframe->pdata[APPPROC_FRAME_IDLENGTH+2];
		
		if(startid>endid)
			return 0;
		
		for(i=startid; i<=endid; i++)
		{
			ret = SysDataFun(DATA_MSG_READ_OBJECT_PARAM,type,i,0,0,&pObjectData,&retLen);
			if(ret<=0)
				return 0;
			
			crcval = CRCCal_break(pObjectData,  retLen,crcval);			
		}
		
		memcpy(temp,&crcval,2);
		temp[2] = type;
		temp[3] = startid;
		temp[4] = endid;
		UdpLinkLayerSend_app(temp,5,1,pframe->header.SN ,addr,AppProcIDType_checkParam_confirm);
	}
	else
	{
		ret = SysDataFun(DATA_MSG_GetParamCRC,0,0,0,0,&pObjectData,&retLen);
		if((ret<=0)||(pObjectData==NULL)||(retLen!=2))
			return 0;
		
		memcpy(temp,&pObjectData,2);
		UdpLinkLayerSend_app(temp,2,1,pframe->header.SN ,addr,AppProcIDType_checkParam_confirm);		
	}
	
	return 1;
}

u8 udpAppCB_checkParam_confirm(UDPFrameType *pframe)
{
	u8 type;
	u8 startid,endid;
	u8 addr;
	int ret;
	u8 i;
	u8 *pObjectData=NULL;
	u16 retLen;
	uint16_t crcval=0xffff;
	uint16_t confirm_crc=0;
	u8 result[2];
	
	if((pframe==NULL)||(pframe->pdata==NULL))
		return 0;
	if(pframe->length < APPPROC_FRAME_IDLENGTH+2)
		return 0;

	addr = IP_ADDR_SUPPORT(pframe->addr);
	setAppDevOnLine(pframe->header.u16FuncID.FUNC.Type,addr);
	
	memcpy(&confirm_crc,&pframe->pdata[APPPROC_FRAME_IDLENGTH],2);
	if(pframe->length >=7)
	{		
		type = pframe->pdata[APPPROC_FRAME_IDLENGTH+2];
		startid = pframe->pdata[APPPROC_FRAME_IDLENGTH+3];
		endid   = pframe->pdata[APPPROC_FRAME_IDLENGTH+4];
		
		if(startid>endid)
			return 0;
		
		for(i=startid; i<=endid; i++)
		{
			ret = SysDataFun(DATA_MSG_READ_OBJECT_PARAM,type,i,0,0,&pObjectData,&retLen);
			if(ret<=0)
				return 0;
			
			crcval = CRCCal_break(pObjectData,  retLen,crcval);			
		}
		
		if(crcval !=confirm_crc)
		{
			result[0]=0;
			result[1]=addr;
			SysDataFun(DATA_MSG_SetParamCRCResult,type,0,2,0,&result,0);
		}
		else
		{
			result[0]=1;
			result[1]=addr;
			SysDataFun(DATA_MSG_SetParamCRCResult,type,0,2,0,&result,0);
		}
	}
	else
	{
		ret = SysDataFun(DATA_MSG_GetParamCRC,0,0,0,0,&crcval,&retLen);
		if((ret<=0)||(retLen!=2))
			return 0;
		
		if(crcval !=confirm_crc)
		{
			result[0]=0;
			result[1]=addr;
			SysDataFun(DATA_MSG_SetParamCRCResult,0,2,0,0,&result,0);
		}
		else
		{
			result[0]=1;
			result[1]=addr;
			SysDataFun(DATA_MSG_SetParamCRCResult,0,2,0,0,&result,0);
		}	
	}
	
	return 1;	
}

u8 udpAppCB_setParam_UIMode_request(UDPFrameType *pframe)
{
	u16 length =0;
	u16 offset =0;
	u8 addr;
	u16 funcid,global;
	uint64_t levels;
	
	if((pframe==NULL)||(pframe->pdata==NULL))
		return 0;
	if(pframe->length <(APPPROC_FRAME_IDLENGTH + 14))
		return 0;
		
	memcpy(&funcid,pframe->pdata,2);
	memcpy(&levels,&(pframe->pdata[APPPROC_FRAME_IDLENGTH]),8);
	memcpy(&global , &(pframe->pdata[APPPROC_FRAME_IDLENGTH+8]),2);
	memcpy(&length , &(pframe->pdata[APPPROC_FRAME_IDLENGTH+10]),2);
	memcpy(&offset , &(pframe->pdata[APPPROC_FRAME_IDLENGTH+12]),2);
	SysParamFun(DATA_MSG_WRITE_OBJECT_PARAM,levels,length,offset,&(pframe->pdata[APPPROC_FRAME_IDLENGTH+14]),0,0,0);
	
	addr = IP_ADDR_SUPPORT(pframe->addr);
	setAppDevOnLine(pframe->header.u16FuncID.FUNC.Type,addr);
	
	return 1;
}

u8 udpAppCB_serverOperate_request(UDPFrameType *pframe)
{
	u16 length =0;
	u8 addr,serverid;
	u16 funcid;
	
	if((pframe==NULL)||(pframe->pdata==NULL))
		return 0;
	if(pframe->length <(APPPROC_FRAME_IDLENGTH + 2))
		return 0;
		
	memcpy(&funcid,pframe->pdata,2);
	memcpy(&serverid,&(pframe->pdata[APPPROC_FRAME_IDLENGTH]),2);
	
	if(serverid ==AppProcServerType_reset_controller)
		SysDataFun(DATA_MSG_RESET_CONTROLLER,0,0,0,0,0,0);
	else if(serverid ==AppProcServerType_reset_defaultParam)
		SysDataFun(DATA_MSG_RESET_DEFAULTPARAM,0,0,0,0,0,0);
	
	addr = IP_ADDR_SUPPORT(pframe->addr);
	setAppDevOnLine(pframe->header.u16FuncID.FUNC.Type,addr);
	
	return 1;
}
uint64_t Real_ID;
uint8_t  ServerMAXbutton;
extern uint8_t  feedback[3];
extern Displayrefresh   Serviceparameters;
u8 udpAppCB_serverDisplays_confirm(UDPFrameType *pframe)
{
	uint64_t id = 0;	
	u8 addr,serverid,i;
	if((pframe==NULL)||(pframe->pdata==NULL))
		return 0;
	if(pframe->length < APPPROC_FRAME_IDLENGTH)
		return 0;

	addr = IP_ADDR_SUPPORT(pframe->addr);
	setAppDevOnLine(pframe->header.u16FuncID.FUNC.Type,addr);
	
	id |= ((uint64_t)pframe->pdata[APPPROC_FRAME_IDLENGTH+16]<<56		) ;
	id |= ((uint64_t)pframe->pdata[APPPROC_FRAME_IDLENGTH+15]<<48 		);
	id |= ((uint64_t)pframe->pdata[APPPROC_FRAME_IDLENGTH+14]<<40 		);
	id |= ((uint64_t)pframe->pdata[APPPROC_FRAME_IDLENGTH+13]<<32 		);
	id |= ((uint64_t)pframe->pdata[APPPROC_FRAME_IDLENGTH+12]<<24 		);
	id |= ((uint64_t)pframe->pdata[APPPROC_FRAME_IDLENGTH+11]<<16 		);
	id |= ((uint64_t)pframe->pdata[APPPROC_FRAME_IDLENGTH+10]<<8 		);
	id |= ((uint64_t)pframe->pdata[APPPROC_FRAME_IDLENGTH+9]  );
	if(id)
		Real_ID = id;
	serverid = pframe->pdata[APPPROC_FRAME_IDLENGTH+8];
	switch(serverid)
	{
		case 0:
			
		break;
		case 1:
			if(feedback[0] == 1)
				feedback[0] = 2;
			Serviceparameters.TextDisplay.count = pframe->pdata[APPPROC_FRAME_IDLENGTH+17];
			for(i=0;i<Serviceparameters.TextDisplay.count;i++)
			{
				memcpy(Serviceparameters.TextDisplay.buf[i],&pframe->pdata[APPPROC_FRAME_IDLENGTH+18+26*i],26);				
			}
		break;
		case 2:
			if(feedback[1] == 1)
				feedback[1] = 2;
			Serviceparameters.ButtonDisplay.count = pframe->pdata[APPPROC_FRAME_IDLENGTH+17];
			for(i=0;i<Serviceparameters.ButtonDisplay.count;i++)
			{
				memcpy(Serviceparameters.ButtonDisplay.buf[i],&pframe->pdata[APPPROC_FRAME_IDLENGTH+18+26*i],26);				
			}
			ServerMAXbutton = Serviceparameters.ButtonDisplay.count;
		break;
		case 3:
			if(feedback[2] == 1)
				feedback[2] = 2;
			Serviceparameters.ListviewDisplaye.count = pframe->pdata[APPPROC_FRAME_IDLENGTH+17];
			for(i=0;i<Serviceparameters.ListviewDisplaye.count;i++)
			{
				memcpy(Serviceparameters.ListviewDisplaye.buf[i],&pframe->pdata[APPPROC_FRAME_IDLENGTH+18+26*i],26);				
			}			
		break;
		default:
		break;	
	}
	
	return 1;
}
extern uint8_t  SERVER_PARAMETERS_SAVED_SUCCESSFULLY;
u8 udpAppCB_serverRevise_confirm(UDPFrameType *pframe)
{
	uint64_t id = 0;	
	u8 addr,serverid,i;
	if((pframe==NULL)||(pframe->pdata==NULL))
		return 0;
	if(pframe->length < APPPROC_FRAME_IDLENGTH)
		return 0;

	addr = IP_ADDR_SUPPORT(pframe->addr);
	setAppDevOnLine(pframe->header.u16FuncID.FUNC.Type,addr);
	
	id |= ((uint64_t)pframe->pdata[APPPROC_FRAME_IDLENGTH+7]<<56		) ;
	id |= ((uint64_t)pframe->pdata[APPPROC_FRAME_IDLENGTH+6]<<48 		);
	id |= ((uint64_t)pframe->pdata[APPPROC_FRAME_IDLENGTH+5]<<40 		);
	id |= ((uint64_t)pframe->pdata[APPPROC_FRAME_IDLENGTH+4]<<32 		);
	id |= ((uint64_t)pframe->pdata[APPPROC_FRAME_IDLENGTH+3]<<24 		);
	id |= ((uint64_t)pframe->pdata[APPPROC_FRAME_IDLENGTH+2]<<16 		);
	id |= ((uint64_t)pframe->pdata[APPPROC_FRAME_IDLENGTH+1]<<8 		);
	id |= ((uint64_t)pframe->pdata[APPPROC_FRAME_IDLENGTH+0]  );
//	if(id)
//		Real_ID = id;
	serverid = pframe->pdata[APPPROC_FRAME_IDLENGTH+8];
	if(serverid == 1)//修改成功
	{
		SERVER_PARAMETERS_SAVED_SUCCESSFULLY = 1;
	}
	else
	{
		SERVER_PARAMETERS_SAVED_SUCCESSFULLY = 0;
	}
	return 1;
}


u16 InsServer;
u8 udpAppCB_serverINS_request(UDPFrameType *pframe)
{
	uint8_t  addr,moduletype,moduleID,number,u8i;
	uint16_t start,end,clear,offset,val,unitLen;
	uint16_t lascValue = 0, lascValue1 = 0, calcMode = 0,sensorID = 0,totalNumber = 0;
	uint32_t flashStart;
	uint64_t levels;
	uint8_t u8tmp[14] = {0};
	ParamUnit	*pUnit=NULL;
	
	if((pframe==NULL)||(pframe->pdata==NULL))
		return 0;
	if(pframe->length <(APPPROC_FRAME_IDLENGTH + 16))
		return 0;
	
	number = getSelfAddr();

	memcpy(&totalNumber,  &(pframe->pdata[APPPROC_FRAME_IDLENGTH+16]),2);	
	memcpy(&start,  &(pframe->pdata[APPPROC_FRAME_IDLENGTH+totalNumber*10+18]),2);
	memcpy(&end  ,  &(pframe->pdata[APPPROC_FRAME_IDLENGTH+totalNumber*10+20]),2);
	
	if((number >= start) && (number <= end))
	{			
		memcpy(&clear, &(pframe->pdata[APPPROC_FRAME_IDLENGTH]),2);	
		if(clear == 3)//clear lasc data
		{
			flashStart = (u8 *)pUnit-(u8 *)(&SysParam);
			ee_ReadBytes((u8 *)&val, SYS_PARAM_START+flashStart+offset*2, 2);
			memcpy((u8 *)pUnit+offset*2,(u8 *)&val,2);
			InsServer = 0;
		}
		else
		{
		
			memcpy(&calcMode,&(pframe->pdata[APPPROC_FRAME_IDLENGTH+2]),2);
			memcpy(&sensorID,&(pframe->pdata[APPPROC_FRAME_IDLENGTH+4]),2);
			
			memcpy(&levels,  &(pframe->pdata[APPPROC_FRAME_IDLENGTH+6]),8);
			memcpy(&offset,  &(pframe->pdata[APPPROC_FRAME_IDLENGTH+14]),2);
		
			for(u8i = 0;u8i<totalNumber;u8i++)
			{
				memcpy(&levels,  &(pframe->pdata[APPPROC_FRAME_IDLENGTH+u8i*10+18]),8);
				memcpy(&offset,  &(pframe->pdata[APPPROC_FRAME_IDLENGTH+u8i*10+26]),2);
				LevelToStruct(levels,&pUnit,&unitLen,&moduletype,&moduleID);
				
				if(((offset+1)> unitLen)||(pUnit ==NULL))
					return 0;
				else
				{
					memcpy(&lascValue,(u8 *)&(pframe->pdata[APPPROC_FRAME_IDLENGTH+totalNumber*10+22+2*(number-start)]),2);
					
					if(calcMode == 1)//
					{
						if(sensorID == 0)
							return 0;
						if(SysState.Sensor[sensorID-1].state_fail == FailType_NORMAL && SysState.Sensor[sensorID-1].state_online == OnlineType_ONLINE)
						{
							if(lascValue>SysState.Sensor[sensorID-1].calculateValue)
							{
								lascValue = 0;
								lascValue1 = 0;
							}
							else
							{
								lascValue1 = SysState.Sensor[sensorID-1].calculateValue - lascValue;
							}
						}
						else
						{
							if(lascValue>850)
								lascValue1 = 0;
							else
								lascValue1 = 850 - lascValue;
						}
					}
					else
					{
						if(lascValue>850)
							lascValue1 = 0;
						else
							lascValue1 = 850 - lascValue;
						//read push target
					}
					memcpy((u8 *)pUnit+offset*2,&lascValue1,2);
					if(lascValue1 == 0)
						InsServer = 1;
					else		
						memcpy((u8 *)&InsServer, &lascValue1,2);
					
				}
			}
		}
		memcpy(&u8tmp[0],&clear,2);
		memcpy(&u8tmp[2],&calcMode,2);
		memcpy(&u8tmp[4],&sensorID,2);
		memcpy(&u8tmp[6],&SysState.Sensor[sensorID].calculateValue,2);
		memcpy(&u8tmp[8],&lascValue,2);
		memcpy(&u8tmp[10],&lascValue1,2);
		addr = IP_ADDR_SUPPORT(pframe->addr);
		UdpLinkLayerSend_app(u8tmp,14,0,pframe->header.SN ,addr,AppProcIDType_serverINS_confirm);
		
	}
	setAppDevOnLine(pframe->header.u16FuncID.FUNC.Type,addr);
	
	return 1;
}

static u8 sysParamFlag[20] = {0};
u8 udpAppCB_sysParamSynchronous_request(UDPFrameType *pframe)
{
	uint64_t levels = 0;
	u8 addr,paramTotalNum,i,moduletype,moduleID;	
	u16 offset,unitLen,serverValue,selfValue;
	ParamUnit	*pUnit=NULL;
	if((pframe==NULL)||(pframe->pdata==NULL))
		return 0;
	if(pframe->length < APPPROC_FRAME_IDLENGTH)
		return 0;

	addr = IP_ADDR_SUPPORT(pframe->addr);
	setAppDevOnLine(pframe->header.u16FuncID.FUNC.Type,addr);
	
	memcpy(&paramTotalNum,&(pframe->pdata[APPPROC_FRAME_IDLENGTH]),2);
	
	for(i = 0;i<paramTotalNum;i++)
	{
		memcpy(&levels,  &(pframe->pdata[APPPROC_FRAME_IDLENGTH+i*12+2]),8);
		memcpy(&offset,  &(pframe->pdata[APPPROC_FRAME_IDLENGTH+i*12+10]),2);
		memcpy(&serverValue,  &(pframe->pdata[APPPROC_FRAME_IDLENGTH+i*12+12]),2);
		
		//获取对应参数的
		LevelToStruct(levels,&pUnit,&unitLen,&moduletype,&moduleID);
		if(((offset+1)> unitLen)||(pUnit ==NULL))
			return 0;
		
		
		selfValue = *(pUnit+(offset-1));
		
		if(selfValue != serverValue)
		{
			if(sysParamFlag[i] == 0)
			{
				//写入故障
				WriteFault_API(FAULT_HOST_OTHER,0,FAULT_EVENT_SYSPARAM_MINSUPPORTNUM_NOTSAME+i,FAULT_STATUS_FAULT);
				sysParamFlag[i] = 1;
			}
			else
				continue;
		}
		else
		{
			if(sysParamFlag[i] == 1)
			{
				//清除故障
				WriteFault_API(FAULT_HOST_OTHER,0,FAULT_EVENT_SYSPARAM_MINSUPPORTNUM_NOTSAME+i,FAULT_STATUS_NORMAL);
				sysParamFlag[i] = 0;
			}
			else
				continue;
		}	
	}
}

//-------------------------- network --------------------------
u8 udpAppCB_enterCfgMode_request(UDPFrameType *pframe)
{
	if(getAppProcMode()==APPPROC_MODE_NORMAL)
	{		
		AppProcData.configTimerId = addTimerEvent(1000, 1, TimerCallback_enterCfgMode);
		if(AppProcData.configTimerId != 0xff)
		{
			setAppProcMode(APPPROC_MODE_CONFIG);
			SetSysFuncMode(SYSMODE_CONFIG);
			startTimerEvent(AppProcData.configTimerId);
		}
//		else
//			AppProcData.configTimerId=0;
	}
	return 1;
}

void TimerCallback_BcmModify(u16 data)
{
	u8 vlan;
	
#ifdef VLAN_AUTO_ENABLE	
	vlan = getBcmInfo_vlanType();
	setVlanBcm(vlan);
#endif
	if(AppProcData.configTimerId !=0xff)
	{
		deleteTimerEvent(AppProcData.configTimerId);
		AppProcData.configTimerId =0xff;
	}
	//setAppProcMode(APPPROC_MODE_NORMAL);
}
u8 request_cnt=0;
u8 udpAppCB_setAddr_request(UDPFrameType *pframe)
{	
	u8 addr;
	u8 direct;
	u8 vlan,rightVlan;
	u8 temp[28];
	u8 sendvlan;
	u8 recvSelfID[DEVICE_SELFID_MAX];
	
	if((pframe==NULL)||(pframe->pdata==NULL))
		return 0;
	if(pframe->length < APPPROC_FRAME_IDLENGTH+27)
		return 0;

//	if(getAppProcMode()==APPPROC_MODE_IPRESQUEST)
//		return 0;	
//	if(getAppProcMode()!=APPPROC_MODE_CONFIG)
//		return 0;
	
	addr = pframe->pdata[APPPROC_FRAME_IDLENGTH];
	if(addr > SysParam.globalParam.SupportParam.MaxSupportID)
		return 1;
	if(addr < SysParam.globalParam.SupportParam.MinSupportID)
		return 1;
	direct = pframe->pdata[APPPROC_FRAME_IDLENGTH+1];
	rightVlan = vlan   = pframe->pdata[APPPROC_FRAME_IDLENGTH+2];
	memcpy(&temp[3],&(pframe->pdata[APPPROC_FRAME_IDLENGTH+3]),12);
	memcpy(recvSelfID,&(pframe->pdata[APPPROC_FRAME_IDLENGTH+15]),DEVICE_SELFID_MAX);
	
	
	setBcmInfo_ipaddr(addr,1,&temp[3]);
	
	if(AppProcData.direct)
	{
		if(direct)
		{
			if(addr==APPPROC_DEV_CONTROLLER_NUM)
				return 1;
//			if(addr >= SysParam.globalParam.SupportParam.MaxSupportID)
//				return 1;
			addr ++;
			setSelfID(DEVICE_SELFID_LEFT,recvSelfID,1);
			
	//		temp[0] = pframe->pdata[APPPROC_FRAME_IDLENGTH];
	//		memcpy(&temp[1],getSelfID(DEVICE_SELFID),DEVICE_SELFID_MAX);
	//		UdpLinkLayerSend_app_vlan(temp,1+DEVICE_SELFID_MAX,1,pframe->header.SN ,UDP_LINK_BROADCAST_ADDR,AppProcIDType_setAddr_confirm,VLANID_V2);		
		}
		else
		{
//			if(addr <=SysParam.globalParam.SupportParam.MinSupportID)
//				return 1;
			addr --;
			setSelfID(DEVICE_SELFID_RIGHT,recvSelfID,1);
			
	//		temp[0] = pframe->pdata[APPPROC_FRAME_IDLENGTH];
	//		memcpy(&temp[1],getSelfID(DEVICE_SELFID),DEVICE_SELFID_MAX);
	//		UdpLinkLayerSend_app_vlan(temp,1+DEVICE_SELFID_MAX,1,pframe->header.SN ,UDP_LINK_BROADCAST_ADDR,AppProcIDType_setAddr_confirm,VLANID_V3);		
		}
		//memcpy(&temp[3],&(pframe->pdata[APPPROC_FRAME_IDLENGTH+3]),12);
		
		if(vlan >= VLANType_MAX)
			return 0;
		
		//setBcmInfo_vlanType(vlan);
		vlan = (vlan==VLANType_V2V3)? VLANType_V3V2 :VLANType_V2V3;
		
		temp[0] = addr;
		temp[1] = direct;
		temp[2] = vlan;
		temp[6] = addr;
	#ifdef VLAN_AUTO_ENABLE
		setVlanBcm(VLANType_V3V2);
		if(direct)
			sendvlan = VLANID_V2;
		else
			sendvlan = VLANID_V3;
	#else
		vlan = getBcmInfo_vlanType();
		if(direct)
			sendvlan = (vlan==VLANType_V2V3)?VLANID_V3:VLANID_V2;
		else
			sendvlan = (vlan==VLANType_V2V3)?VLANID_V2:VLANID_V3;
	#endif
	}
	else
	{
		if(direct==0)
		{
			if(addr==APPPROC_DEV_CONTROLLER_NUM)
				return 1;
			addr ++;
			setSelfID(DEVICE_SELFID_RIGHT,recvSelfID,1);		
			
	//		temp[0] = pframe->pdata[APPPROC_FRAME_IDLENGTH];
	//		memcpy(&temp[1],getSelfID(DEVICE_SELFID),DEVICE_SELFID_MAX);
	//		UdpLinkLayerSend_app_vlan(temp,1+DEVICE_SELFID_MAX,1,pframe->header.SN ,UDP_LINK_BROADCAST_ADDR,AppProcIDType_setAddr_confirm,VLANID_V2);		
		}
		else
		{
			if(addr <=1)
				return 1;
			addr --;
			setSelfID(DEVICE_SELFID_LEFT,recvSelfID,1);
			
	//		temp[0] = pframe->pdata[APPPROC_FRAME_IDLENGTH];
	//		memcpy(&temp[1],getSelfID(DEVICE_SELFID),DEVICE_SELFID_MAX);
	//		UdpLinkLayerSend_app_vlan(temp,1+DEVICE_SELFID_MAX,1,pframe->header.SN ,UDP_LINK_BROADCAST_ADDR,AppProcIDType_setAddr_confirm,VLANID_V3);		
		}
		//memcpy(&temp[3],&(pframe->pdata[APPPROC_FRAME_IDLENGTH+3]),12);
		
		if(vlan >= VLANType_MAX)
			return 0;
		
		//setBcmInfo_vlanType(vlan);
		vlan = (vlan==VLANType_V2V3)? VLANType_V3V2 :VLANType_V2V3;
		
		temp[0] = addr;
		temp[1] = direct;
		temp[2] = vlan;
		temp[6] = addr;
	#ifdef VLAN_AUTO_ENABLE
		setVlanBcm(VLANType_V3V2);
		if(direct)
			sendvlan = VLANID_V2;
		else
			sendvlan = VLANID_V3;
	#else
		vlan = getBcmInfo_vlanType();
		if(direct)
			sendvlan = (vlan==VLANType_V2V3)?VLANID_V3:VLANID_V2;
		else
			sendvlan = (vlan==VLANType_V2V3)?VLANID_V2:VLANID_V3;
	#endif		
	}
	memcpy(&temp[15],getSelfID(DEVICE_SELFID),DEVICE_SELFID_MAX);
	UdpLinkLayerSend_app_vlan(temp,15+DEVICE_SELFID_MAX,0,0 ,UDP_LINK_BROADCAST_ADDR,AppProcIDType_setAddr_request,sendvlan);	//@@todo tag/ by direct
	
	temp[0] = pframe->pdata[APPPROC_FRAME_IDLENGTH];
	memcpy(&temp[1],getSelfID(DEVICE_SELFID),DEVICE_SELFID_MAX);
	UdpLinkLayerSend_app(temp,1+DEVICE_SELFID_MAX,1,pframe->header.SN ,UDP_LINK_BROADCAST_ADDR,AppProcIDType_setAddr_confirm); //@@todo tag/ by direct

	AppProcData.configTimerId = addTimerEvent(1000, 1, TimerCallback_BcmModify);
	if(AppProcData.configTimerId != 0xff)
	{
		startTimerEvent(AppProcData.configTimerId);
	}
//	else
//		AppProcData.configTimerId=0;
	request_cnt++;

#ifdef VLAN_AUTO_ENABLE	
	setBcmInfo_vlanType(rightVlan);
#endif
	//setAppProcMode(APPPROC_MODE_IPRESQUEST);
	
	return 1;
}

u8 udpAppCB_setAddr_confirm(UDPFrameType *pframe)
{	
	u8 addr,selfaddr;
	u8 recvSelfID[DEVICE_SELFID_MAX];
	
	if((pframe==NULL)||(pframe->pdata==NULL))
		return 0;
	if(pframe->length < APPPROC_FRAME_IDLENGTH+13)
		return 0;
	
//	if(getAppProcMode()!=APPPROC_MODE_IPRESQUEST)
//		return 0;
	
	addr = pframe->pdata[APPPROC_FRAME_IDLENGTH];
	memcpy(recvSelfID,&(pframe->pdata[APPPROC_FRAME_IDLENGTH+1]),DEVICE_SELFID_MAX);
	selfaddr = getBcmInfo_ipaddr();
	
	if(AppProcData.direct)
	{
		if((selfaddr+1) == addr)
		{
			setSelfID(DEVICE_SELFID_RIGHT,recvSelfID,1);
		}
		else if((addr+1) == selfaddr)
		{
			setSelfID(DEVICE_SELFID_LEFT,recvSelfID,1);
		}
	}
	else
	{
		if((selfaddr+1) == addr)
		{
			setSelfID(DEVICE_SELFID_LEFT,recvSelfID,1);
		}
		else if((addr+1) == selfaddr)
		{
			setSelfID(DEVICE_SELFID_RIGHT,recvSelfID,1);
		}		
	}
	
	setAppProcMode(APPPROC_MODE_NORMAL);
	return 1;
}

u8 udpAppCB_netHeart_request(UDPFrameType *pframe)
{
	u8 addr;
	u8 Response[41];
	globalStateType* pstate=NULL;
	uint16_t len=0;
	if((pframe==NULL)||(pframe->pdata==NULL))
		return 0;
	if(pframe->length < APPPROC_FRAME_IDLENGTH)
		return 0;
	NetworkHeartbeatTimesZero();
	addr = IP_ADDR_SUPPORT(pframe->addr);
	setAppDevOnLine(pframe->header.u16FuncID.FUNC.Type,addr);
	SysDataFun(DATA_MSG_READ_OBJECT,ENUM_SYSTEM,0,0,0,&pstate,&len);
	if(pframe->pdata[APPPROC_FRAME_IDLENGTH] == 0)
	{
		memcpy(Response,&pstate->globalCRC,2);//
		UdpLinkLayerSend_app(Response,2,0,pframe->header.SN ,addr,AppProcIDType_netHeart_confirm);
	}
	else if(pframe->pdata[APPPROC_FRAME_IDLENGTH] == 1)
	{	
		memcpy(Response,&pstate->globalCRC,2);//
		Response[2] = UDP_DEVICE_CONTROLLER;
		memcpy(&Response[3],getSelfID(DEVICE_SELFID),12);
		Response[15] = UDP_DEVICE_CONTROLLER;
		memcpy(&Response[16],getSelfID(DEVICE_SELFID_LEFT),12);
		Response[28] = UDP_DEVICE_CONTROLLER;
		memcpy(&Response[29],getSelfID(DEVICE_SELFID_RIGHT),12);		
		UdpLinkLayerSend_app(Response,41,0,pframe->header.SN ,addr,AppProcIDType_netHeart_confirm);
	}
	return 1;
}

u8 udpAppCB_timeSync_request(UDPFrameType *pframe)
{
	u8 addr;
	u8 year;
	u8 month;
	u8 day;
	u8 hour;
	u8 min;
	u8 sec;
	u8 ret;
	
	if((pframe==NULL)||(pframe->pdata==NULL))
		return 0;
	if(pframe->length < APPPROC_FRAME_IDLENGTH+6)
		return 0;

	addr = IP_ADDR_SUPPORT(pframe->addr);
	setAppDevOnLine(pframe->header.u16FuncID.FUNC.Type,addr);
	
	year = pframe->pdata[APPPROC_FRAME_IDLENGTH+2-2];
	month= pframe->pdata[APPPROC_FRAME_IDLENGTH+3-2];
	day  = pframe->pdata[APPPROC_FRAME_IDLENGTH+4-2];
	hour = pframe->pdata[APPPROC_FRAME_IDLENGTH+5-2];
	min  = pframe->pdata[APPPROC_FRAME_IDLENGTH+6-2];
	sec  = pframe->pdata[APPPROC_FRAME_IDLENGTH+7-2];
	
	if(/*(year <1970)||*/(month>12)||(day>31)||(hour>23)||(min>59)||(sec>59))
		return 0;
	
	SysDataFun(DATA_MSG_SetSysTime,0,0,6,0,&pframe->pdata[APPPROC_FRAME_IDLENGTH+2-2],0);
	
	ret =1;
	UdpLinkLayerSend_app(&ret,1,1,pframe->header.SN ,addr,AppProcIDType_timeSync_confirm);
	return 1;
}

u8 udpAppCB_commCheck_request(UDPFrameType *pframe)
{
	u8 addr;
	u8 id[DEVICE_SELFID_MAX],selfParam[APPPROC_SELF_PARAM_LEN],saveParam[APPPROC_SELF_PARAM_LEN];
	u8 dataSource=0;
	u8 *saveid;
	u8 ret=0;
	u8 temp[2],ver[V4_CONTROLLER_VERSION_LEN];
	u8 vlan,direct;
	
	if((pframe==NULL)||(pframe->pdata==NULL))
		return 0;
	if(pframe->length < APPPROC_FRAME_IDLENGTH+DEVICE_SELFID_MAX+APPPROC_SELF_PARAM_LEN+V4_CONTROLLER_VERSION_LEN)
		return 0;

	addr = IP_ADDR_SUPPORT(pframe->addr);
	setAppDevOnLine(pframe->header.u16FuncID.FUNC.Type,addr);
	
	memcpy(&id[0],&pframe->pdata[APPPROC_FRAME_IDLENGTH],DEVICE_SELFID_MAX);
	memcpy(&selfParam[0],&pframe->pdata[APPPROC_FRAME_IDLENGTH+DEVICE_SELFID_MAX],APPPROC_SELF_PARAM_LEN);
	memcpy(&ver[0],&pframe->pdata[APPPROC_FRAME_IDLENGTH+DEVICE_SELFID_MAX+APPPROC_SELF_PARAM_LEN],V4_CONTROLLER_VERSION_LEN);
	dataSource = pframe->header.u16FuncID.FUNC.VLAN;
	vlan = getBcmInfo_vlanType();
	//dataSource = bcmApi_getLRDataSource();

	if(vlan == VLANType_V2V3)
	{
		if(dataSource==VLANID_V2)
		{
			saveid = getSelfID(DEVICE_SELFID_LEFT);
			direct = DEVICE_SELFID_LEFT;
		}
		else
		{
			saveid = getSelfID(DEVICE_SELFID_RIGHT);
			direct = DEVICE_SELFID_RIGHT;
		}
	}
	else
	{
		if(dataSource==VLANID_V3)
		{
			saveid = getSelfID(DEVICE_SELFID_LEFT);
			direct = DEVICE_SELFID_LEFT;
		}
		else
		{
			saveid = getSelfID(DEVICE_SELFID_RIGHT);
			direct = DEVICE_SELFID_RIGHT;
		}
	}
	
	ret = memcmp(id,saveid,DEVICE_SELFID_MAX);
	if(ret==0)
	{
		temp[0] = 1;
		temp[1] = direct;
		SysDataFun(DATA_MSG_SetCommCheckState,0,0,2,0,temp,0);
	}
	else
	{
		temp[0] = 0;
		temp[1] = direct;
		SysDataFun(DATA_MSG_SetCommCheckState,0,0,2,0,temp,0);		
	}
	clearUdpAppProcTimer(direct);
	
	getUdpAppSelfParamInfo(saveParam,APPPROC_SELF_PARAM_LEN);
	ret = memcmp(selfParam,saveParam,APPPROC_SELF_PARAM_LEN);
	if(ret==0)
	{
		temp[0] = 1;
		temp[1] = direct;
		SysDataFun(DATA_MSG_SetCommCheckGlobalCRC,0,0,2,0,temp,0);
	}
	else
	{
		temp[0] = 0;
		temp[1] = direct;
		SysDataFun(DATA_MSG_SetCommCheckGlobalCRC,0,0,2,0,temp,0);		
	}
	
    if((ver[0] != V4_CONTROLLER_VERSION_BYTE1)||(ver[1] != V4_CONTROLLER_VERSION_BYTE2)
	  ||(ver[2] != V4_CONTROLLER_VERSION_BYTE3)||(ver[3] != V4_CONTROLLER_VERSION_BYTE4))
	{
		temp[0] = 0;
		temp[1] = direct;
		SysDataFun(DATA_MSG_SetCommCheckGlobalVERSION,0,0,2,0,temp,0);
	}
	else
	{
		temp[0] = 1;
		temp[1] = direct;
		SysDataFun(DATA_MSG_SetCommCheckGlobalVERSION,0,0,2,0,temp,0);		
	}
	
//	saveid = getSelfID(DEVICE_SELFID);
//	UdpLinkLayerSend_app(saveid,DEVICE_SELFID_MAX,1,pframe->header.SN ,addr,AppProcIDType_commCheck_confirm);
	return 1;
}

u8 udpAppCB_commCheck_confirm(UDPFrameType *pframe)
{
	u8 addr;
	u8 id[DEVICE_SELFID_MAX];
	u8 dataSource=0;
	u8 *saveid;
	u8 ret=0;
	u8 temp[2];
	
	if((pframe==NULL)||(pframe->pdata==NULL))
		return 0;
	if(pframe->length < APPPROC_FRAME_IDLENGTH+12)
		return 0;

	addr = IP_ADDR_SUPPORT(pframe->addr);
	setAppDevOnLine(pframe->header.u16FuncID.FUNC.Type,addr);
	
	memcpy(id,&pframe->pdata[APPPROC_FRAME_IDLENGTH+2],DEVICE_SELFID_MAX);
	//dataSource = bcmApi_getLRDataSource();
	dataSource = pframe->header.u16FuncID.FUNC.VLAN;
	if(dataSource==0)
		saveid = getSelfID(DEVICE_SELFID_LEFT);
	else
		saveid = getSelfID(DEVICE_SELFID_RIGHT);
	
	ret = memcmp(id,saveid,DEVICE_SELFID_MAX);
	if(ret==0)
	{
		temp[0] = 1;
		temp[1] = dataSource;
		SysDataFun(DATA_MSG_SetCommCheckState,0,0,2,0,temp,0);
	}
	else
	{
		temp[0] = 0;
		temp[1] = dataSource;
		SysDataFun(DATA_MSG_SetCommCheckState,0,0,2,0,temp,0);		
	}
	return 1;
}

#endif
