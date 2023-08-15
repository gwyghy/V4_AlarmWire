/*
*********************************************************************************************************
*	                                            DoubleWires
*                           Commuication Controller For The Mining Industry
*
*                                  (c) Copyright 1994-2013  HNDZ 
*                       All rights reserved.  Protected by international copyright laws.
*
*
*    File    : devmng_dev.c
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

#define __DEVMNG_DEV_C__

#include "devmng_dev.h"
#include "udp_app_proc.h"
#include "bsp_jtbs.h"
#include "bsp_leds.h"
#include "bsp_beeps.h"


#if (SERVER_DEVICEMNG_EN == 1)

extern u16  DevmngAD_value[DEV_AD_CHANNEL_MAX];

int16_t   DevRead_Can(uint16_t driver, uint16_t device, uint16_t subDev, uint16_t point, uint16_t* data)
{
	int16_t ret=0;
	u8 online=0;
	uint32_t temp;
	u16 u16temp = 0x9999;//无效值
	stDeviceStatusMsg  deviceMsg;
	
	ret = DevCtl_Can(DevmngDevCtlType_ONLINE,driver, device, subDev, &online);
	if((ret==0)||(online != OnlineType_ONLINE))
		return 0;
	switch(device)
	{
		case ID_HUB1:
			GetCanDeviceStatusMsg(driver,device,0,&deviceMsg);
			if(deviceMsg.isEnable == ENABLE)
			{
				GetCanDeviceAPPMsg(driver, device, subDev, point-1, &temp);
				* data = (u16)temp;
			}
			else
			{
				* data = u16temp;
			}
			break;
		case ID_HUB2:
			GetCanDeviceStatusMsg(driver,device,0,&deviceMsg);
			if(deviceMsg.isEnable == ENABLE)
			{
				GetCanDeviceAPPMsg(driver, device, subDev, point-1, &temp);
				* data = (u16)temp;
			}
			else
			{
				* data = u16temp;
			}
			break;
		case ID_HUB3:
			GetCanDeviceStatusMsg(driver,device,0,&deviceMsg);
			if(deviceMsg.isEnable == ENABLE)
			{
				GetCanDeviceAPPMsg(driver, device, subDev, point-1, &temp);
				*data = (u16)temp;
			}
			else
			{
				* data = u16temp;
			}
			break;
		default:
			 GetCanDeviceAPPMsg(driver, device, subDev-1, point-1, &temp);
			*data = (u16)temp;
			return 1;
			break;
	}
	return 1;
}

int16_t   DevWrite_Can(uint16_t driver, uint16_t device, uint16_t subDev, uint16_t point, uint16_t status)
{
	int16_t ret=0;
	u8 online=0;
	
//	ret = DevCtl_Can(DevmngDevCtlType_ONLINE,driver, device, subDev, &online);
//	if((ret==0)||(online != OnlineType_ONLINE))
//		return 0;
	
	printk("DevWrite_Can %d %d %d %d ,status:%d \r\n", driver,device,subDev,point,status);
	CanProLoopSend(driver, device, subDev, point, status,DEV_CAN_SEND_INTERVAL,DEV_CAN_SEND_CNT,0);
	return 1;
}

int16_t   DevCtl_Can(uint8_t type,uint16_t driver, uint16_t device, uint16_t subDev, void *pdata)
{
	int16_t ret=0;
	stDeviceStatusMsg deviceMsg;	
	switch(type)
	{
		case DevmngDevCtlType_ENABLE:
			GetCanDeviceStatusMsg( driver,device,subDev,&deviceMsg);
			*((u8*)pdata) = deviceMsg.isEnable;
			ret =1;
		break;
		
		case DevmngDevCtlType_ONLINE:
			GetCanDeviceStatusMsg( driver,device,subDev,&deviceMsg);
			*((u8*)pdata) = deviceMsg.isOnline;
			ret =1;
		break;	

		default:
			break;
	}
	return ret;
}

int16_t   DevRead_AD(uint16_t driver, uint16_t device, uint16_t subDev, uint16_t point, uint16_t* data)
{
	uint32_t temp;
	u16 u16temp = 0;//无效值
	stDeviceStatusMsg  deviceMsg;
	
	switch(device)
	{
		case ID_SC:
				if((point >DEV_AD_CHANNEL_MAX)||(point ==0))
				return 0;
				*data = DevmngAD_value[point-1];
				return 1;
		break;			
		case ID_HUB1:
			GetCanDeviceStatusMsg(0,device,0,&deviceMsg);
			if(deviceMsg.isEnable == ENABLE && deviceMsg.isOnline ==1)
			{
				if((point >DEV_AD_CHANNEL_MAX)||(point == 0 || subDev != 0)) //ghy 模拟量上子设备号必须设置为0
					return 0;
				GetCanDeviceAPPMsg(0, device, subDev, point-1, &temp); 
				*data = (u16)temp;
				return 1;
			}
			else 
			{
				*data = u16temp;
			}
			
		break;
		case ID_HUB2:
			GetCanDeviceStatusMsg(0,device,0,&deviceMsg);
			if(deviceMsg.isEnable == ENABLE && deviceMsg.isOnline ==1)
			{
				if((point >DEV_AD_CHANNEL_MAX)||(point == 0 || subDev != 0)) //ghy 模拟量上子设备号必须设置为0
					return 0;
				GetCanDeviceAPPMsg(0, device, subDev, point-1, &temp); 
				*data = (u16)temp;
				return 1;
			}
			else 
			{
				*data = u16temp;
			}
		break;
		case ID_HUB3:
			GetCanDeviceStatusMsg(0,device,0,&deviceMsg);
			if(deviceMsg.isEnable == ENABLE && deviceMsg.isOnline ==1)
			{
				if((point >DEV_AD_CHANNEL_MAX)||(point == 0 || subDev != 0)) //ghy 模拟量上子设备号必须设置为0
					return 0;
				GetCanDeviceAPPMsg(0, device, subDev, point-1, &temp); 
				*data = (u16)temp;
				return 1;
			}
			else 
			{
				*data = u16temp;
			}
			break;
		default:
			break;	
	}
}

int16_t   DevWrite_AD(uint16_t driver, uint16_t device, uint16_t subDev, uint16_t point, uint16_t status)
{
	return 1;
}

int16_t   DevCtl_AD(uint8_t type,uint16_t driver, uint16_t device, uint16_t subDev, void *pdata)
{
	uint8_t ret = 0;
	switch(type)
	{
		case DevmngDevCtlType_ENABLE:
			ret = 1;
		break;
		
		case DevmngDevCtlType_ONLINE:
			ret = 1;
			*((u8*)pdata) = ret;
			ret =1;
		break;	

		default:
			break;
	}
	return ret;
}

int16_t   DevRead_Net(uint16_t driver, uint16_t device, uint16_t subDev, uint16_t point, uint16_t* data)
{
	return 1;
}

int16_t   DevWrite_Net(uint16_t driver, uint16_t device, uint16_t subDev, uint16_t point, uint16_t status)
{
//	int16_t ret=0;
//	u8 online=0;
//	
//	ret = DevCtl_Net(DevmngDevCtlType_ONLINE,driver, device, subDev, &online);
//	if((ret==0)||(online != OnlineType_ONLINE))
//		return 0;

	return 1;
}

int16_t   DevCtl_Net(uint8_t type,uint16_t driver, uint16_t device, uint16_t subDev, void *pdata)
{
	int16_t ret=0;
	u8 temp;
	
	switch(type)
	{		
		case DevmngDevCtlType_ONLINE:
			if( getAppDevOnLine(UDP_DEVICE_CONTROLLER,driver,&temp) >0)
			{
				*((u8*)pdata) = temp;
				ret =1;
			}
			break;	

		default:
			break;
	}
	return ret;
}

int16_t   DevRead_IO(uint16_t driver, uint16_t device, uint16_t subDev, uint16_t point, uint16_t* data)
{
	if((point >=DevIOInputPointType_MAX)||(point ==0))
		return 0;

	if(point == DevIOInputPointType_EMERGENCY)
		*data = CheckJT();
	else if(point == DevIOInputPointType_BISUO)
		*data = CheckBS();
	else
		*data = 0;
	
	return 1;
}

int16_t   DevWrite_IO(uint16_t driver, uint16_t device, uint16_t subDev, uint16_t point, uint16_t status)
{
	if((point >=DevIOOutputPointType_MAX)||(point ==0))
		return 0;

	if(point == DevIOOutputPointType_LightBisuo)
	{
		if(status)
			StateLed(LED_STOP,0);
		else
			StateLed(0,LED_STOP);
	}
	else if(point == DevIOOutputPointType_LightNetTrs)
	{
		if(status)
			StateLed(LED_COM_TRS,0);
		else
			StateLed(0,LED_COM_TRS);		
	}
	else if(point == DevIOOutputPointType_LightNetRecv)
	{
		if(status)
			StateLed(LED_COM_REV,0);
		else
			StateLed(0,LED_COM_REV);		
	}
	else if(point == DevIOOutputPointType_BEEP_NEAR_SELECTED)
	{
		if(status)
			BeepOn(BEEP_NEAR_SELECTED);
		else
			BeepOff();		
	}
	else if(point == DevIOOutputPointType_BEEP_LOCKED)
	{
		if(status)
			BeepOn(BEEP_LOCKED);
		else
			BeepOff();		
	}
	else if(point == DevIOOutputPointType_BEEP_UNLOCKED)
	{
		if(status)
			BeepOn(BEEP_UNLOCKED);
		else
			BeepOff();		
	}
	else if(point == DevIOOutputPointType_BEEP_END_PRESSED)
	{
		if(status)
			BeepOn(BEEP_END_PRESSED);
		else
			BeepOff();		
	}
	else if(point == DevIOOutputPointType_BEEP_ACTION)
	{
		if(status)
			BeepOn(BEEP_AUTO_ACTION);
		else
			BeepOff();		
	}	
	else if(point == DevIOOutputPointType_B12V)
	{
		if(status)
			B12V_ON();
		else
			B12V_OFF();		
	}	
	
	return 1;
}

int16_t   DevCtl_IO(uint8_t type,uint16_t driver, uint16_t device, uint16_t subDev, void *pdata)
{
	return 1;
}

#endif
