/*
*********************************************************************************************************
*	                                            DoubleWires
*                           Commuication Controller For The Mining Industry
*
*                                  (c) Copyright 1994-2013  HNDZ 
*                       All rights reserved.  Protected by international copyright laws.
*
*
*    File    : udp_app_bcm.c
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

#include "udp_app_bcm.h"
#include "sys_msg_def.h"


static bcmInfoType bcmInfo;
extern void setVlanBcm(u8 vlanMode);


void setBcmInfo_ipaddr(u8 addr,u8 sel,u8 *pdata)
{
	NetParamType *pParam;
	
	bcmInfo.ipaddr = addr;
	
	if((sel)&&(pdata != 0))
	{
		pParam = (NetParamType *)pdata;
		SysDataFun(DATA_MSG_SET_IP_PARAM,0,0,0,0,pParam,0);	
	}
}

u8 getBcmInfo_ipaddr(void)
{
	return bcmInfo.ipaddr;
}

void setBcmInfo_vlanType(u8 vlan)
{
	bcmInfo.vlanType = vlan;
	SysDataFun(DATA_MSG_SET_VLAN_PARAM, 0,0,0,0,(void *)&bcmInfo.vlanType,0);
}

u8 getBcmInfo_vlanType(void)
{
	return bcmInfo.vlanType;
}

void bcmApi_setLRCommBreak(void)
{
	setVlanBcm(VLANType_V2V3);
}

void bcmApi_setLRCommNormal(void)
{
	
}

u8 bcmApi_getLRDataSource(void)
{
	
}

