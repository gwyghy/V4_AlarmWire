/*
*********************************************************************************************************
*	                                            DoubleWires
*                           Commuication Controller For The Mining Industry
*
*                                  (c) Copyright 1994-2013  HNDZ 
*                       All rights reserved.  Protected by international copyright laws.
*
*
*    File    : devmng.c
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

#define __DEVMNG_C__

#include "devmng.h"

#if (SERVER_DEVICEMNG_EN == 1)

static DevmngType Devmng;
u16    DevmngAD_value[DEV_AD_CHANNEL_MAX];

static const Devmng_DevType Devmng_DevInfo[]={
	
	{

		DevRead_Can,
		DevWrite_Can,
		DevCtl_Can
	},
	
	{
		DevRead_Can,
		DevWrite_Can,
		DevCtl_Can
	},
	{
		DevRead_AD,
		DevWrite_AD,
		DevCtl_AD
	},

	{
		DevRead_Net,
		DevWrite_Net,
		DevCtl_Net
	},	
	{
		DevRead_IO,
		DevWrite_IO,
		DevCtl_IO
	},
};

static void DevmngDataInit(void)
{
	int i;
	
	for(i=0; i<DRIVER_MAX;i++)
		Devmng.DevList[i] = Devmng_DevInfo[i];
	memset(DevmngAD_value,0,sizeof(DevmngAD_value));
}

DevmngType * getDeviceMng(void)
{
	return &Devmng;
}

void  DeviceMng_Task(void *p_arg)
{
	uint8_t err,res;	
	u16     delay=10,sensorvalue = 0;
	int i;
	
	(void)p_arg;
	
	DevmngDataInit();
	
	while (1) 
	{	
		for(i=0;i<DEV_AD_SPI_CHANNEL_MAX;i++)
		{
			res = MCP3204_Value(i,&sensorvalue);
			if(res)
				DevmngAD_value[i] = sensorvalue;
		}
//		for(i=0;i<2;i++)
//			DevmngAD_value[DEV_AD_SPI_CHANNEL_MAX+i] = AdcPro(i+1);
		
		PLATFORM_OS_TimeDly(delay);
	}
}

#endif
