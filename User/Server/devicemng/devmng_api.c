/*
*********************************************************************************************************
*	                                            DoubleWires
*                           Commuication Controller For The Mining Industry
*
*                                  (c) Copyright 1994-2013  HNDZ 
*                       All rights reserved.  Protected by international copyright laws.
*
*
*    File    : devmng_api.c
*    Module  : system function
*    Version : V1.0
*    History :
*   -----------------
*              实现设备管理功能
*              Version  Date           By            Note
*              v1.0     
*
*********************************************************************************************************
*/

#define __DEVMNG_API_C__

#include "devmng_api.h"

#if (LOGIC_SYSFUNC_EN == 1)

u8 read_data (uint8_t SupportId,uint8_t DriverId,uint8_t DeviceId,uint8_t ChildDeviceId,uint8_t PointId,void *pdata)
{
	DevmngType *pDevMng;
	u16 value;
	
	if(SupportId == SUPPORT_DEFAULT_ADDR)
	{
		if((DriverId >=DRIVER_MAX)||(DriverId ==0))
			return 0;
		
		pDevMng = getDeviceMng();
		if(pDevMng ==NULL)
			return 0;
		
		pDevMng->DevList[DriverId-1].Read(DriverId-1,DeviceId,ChildDeviceId,PointId,&value);
		*((u16 *)pdata) = value;
		return 1;
	}
	else
	{
		
	}
	
	return 0;
}

u8 write_data(uint8_t SupportId,uint8_t DriverId,uint8_t DeviceId,uint8_t ChildDeviceId,uint8_t PointId,void *pdata)
{
	DevmngType *pDevMng;
	u8 value;
	
	if(SupportId == SUPPORT_DEFAULT_ADDR)
	{
		if((DriverId >=DRIVER_MAX)||(DriverId ==0))
			return 0;
		
		pDevMng = getDeviceMng();
		if(pDevMng ==NULL)
			return 0;
		
		value = *((u8 *)pdata);
		pDevMng->DevList[DriverId-1].Write(DriverId-1,DeviceId,ChildDeviceId,PointId,value);
		return 1;
	}
	else
	{
		
	}
	
	return 0;
}	

u8 get_device_state(uint8_t SupportId,uint8_t DriverId,uint8_t DeviceId,uint8_t ChildDeviceId,OnlineType *state)
{
	DevmngType *pDevMng;
	
	if(DriverId == 0)
		return 0;	
	
	if(SupportId == SUPPORT_DEFAULT_ADDR)
	{
		if(DriverId >=DRIVER_MAX)
			return 0;

		pDevMng = getDeviceMng();
		if(pDevMng ==NULL)
			return 0;
		
		pDevMng->DevList[DriverId-1].DevCtl(DevmngDevCtlType_ONLINE,DriverId-1,DeviceId,ChildDeviceId,(u8 *)state);
		return 1;
	}
	else
	{
		if(DriverId!=DRIVER_NET)
			return 0;
		
		pDevMng = getDeviceMng();
		if(pDevMng ==NULL)
			return 0;
		
		pDevMng->DevList[DriverId-1].DevCtl(DevmngDevCtlType_ONLINE,SupportId,0,0,(u8 *)state);
		return 1;		
	}
	return 0;
}

u8 ctrl_phone(uint8_t PhoneId,void *pdata)
{
	return 1;
}

#endif
