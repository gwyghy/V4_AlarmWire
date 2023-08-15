#ifndef __DEVMNG_H__
#define __DEVMNG_H__

#include "devmng_dev.h"

#define SUPPORT_DEFAULT_ADDR    0


enum
{		
	DRIVER_CAN1=1,   
	DRIVER_CAN2,
	DRIVER_AD,     //spi-ad
	DRIVER_NET,    //×óÓÒÁÚ¼ÜÍøÂç/can
	DRIVER_IO ,    //±¾¼ÜIO
	DRIVER_MAX
};

enum
{
	DEVICETYPE_CONTROLLER=1,
	DEVICETYPE_SERVER ,
	DEVICETYPE_DATATRANS
};

typedef struct {
	int16_t    (*Read)(uint16_t driver, uint16_t device, uint16_t subDev, uint16_t point, uint16_t* data);
	int16_t    (*Write)(uint16_t driver,uint16_t device, uint16_t subDev, uint16_t point, uint16_t status);
	int16_t    (*DevCtl)(uint8_t type , uint16_t driver, uint16_t device, uint16_t subDev, void *);	
} Devmng_DevType;

typedef struct
{
	Devmng_DevType	DevList[DRIVER_MAX];
} DevmngType;

DevmngType * getDeviceMng(void);

#endif
