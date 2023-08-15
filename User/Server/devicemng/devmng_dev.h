#ifndef __DEVMNG_DEV_H__
#define __DEVMNG_DEV_H__

#include "includes.h"
#include "Can_Pro.h"

#define DEV_CAN_SEND_INTERVAL    150    //40ms
#define DEV_CAN_SEND_CNT         0xFF  //Ò»Ö±·¢ËÍ
#define DEV_AD_CHANNEL_MAX		 6
#define DEV_AD_SPI_CHANNEL_MAX	 4

enum
{
	DevmngDevCtlType_ENABLE =0,
	DevmngDevCtlType_ONLINE,
	DevmngDevCtlType_MAX
};

enum
{
	DevIOInputPointType_EMERGENCY =1,
	DevIOInputPointType_BISUO,
	DevIOInputPointType_STOP,
	DevIOInputPointType_PERSON_BISUO,
	DevIOInputPointType_MAX
};

enum
{	
	DevIOOutputPointType_LightBisuo =1,
	DevIOOutputPointType_LightNetTrs,
	DevIOOutputPointType_LightNetRecv,
	DevIOOutputPointType_BEEP_NEAR_SELECTED,
	DevIOOutputPointType_BEEP_LOCKED,
	DevIOOutputPointType_BEEP_UNLOCKED,
	DevIOOutputPointType_BEEP_END_PRESSED,
	DevIOOutputPointType_BEEP_ACTION,
	DevIOOutputPointType_B12V,
	DevIOOutputPointType_MAX
};

typedef enum 
{
	OnlineType_OFFLINE =0,
	OnlineType_ONLINE
}OnlineType;

int16_t   DevCtl_Can(uint8_t type,uint16_t driver, uint16_t device, uint16_t subDev, void *pdata);
int16_t   DevRead_Can(uint16_t driver, uint16_t device, uint16_t subDev, uint16_t point, uint16_t* data);
int16_t   DevWrite_Can(uint16_t driver, uint16_t device, uint16_t subDev, uint16_t point, uint16_t status);

int16_t   DevCtl_AD(uint8_t type,uint16_t driver, uint16_t device, uint16_t subDev, void *pdata);
int16_t   DevRead_AD(uint16_t driver, uint16_t device, uint16_t subDev, uint16_t point, uint16_t* data);
int16_t   DevWrite_AD(uint16_t driver, uint16_t device, uint16_t subDev, uint16_t point, uint16_t status);

int16_t   DevCtl_Net(uint8_t type,uint16_t driver, uint16_t device, uint16_t subDev, void *pdata);
int16_t   DevRead_Net(uint16_t driver, uint16_t device, uint16_t subDev, uint16_t point, uint16_t* data);
int16_t   DevWrite_Net(uint16_t driver, uint16_t device, uint16_t subDev, uint16_t point, uint16_t status);

int16_t   DevCtl_IO(uint8_t type,uint16_t driver, uint16_t device, uint16_t subDev, void *pdata);
int16_t   DevRead_IO(uint16_t driver, uint16_t device, uint16_t subDev, uint16_t point, uint16_t* data);
int16_t   DevWrite_IO(uint16_t driver, uint16_t device, uint16_t subDev, uint16_t point, uint16_t status);

#endif
