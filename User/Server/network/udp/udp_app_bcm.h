#ifndef __UDP_APP_BCM_H
#define __UDP_APP_BCM_H

//#include "bcm8953xlib.h"
#include "stm32f4xx.h"

enum
{
	VLANType_V2V3=0,
	VLANType_V3V2,
	VLANType_MAX
};

enum
{
	VLANID_V2=2,
	VLANID_V3=3
};

typedef struct
{
	u8    ipaddr;
	u8    vlanType;
}bcmInfoType;


void setBcmInfo_ipaddr(u8 addr,u8 sel,u8 *pdata);
u8 getBcmInfo_ipaddr(void);
void setBcmInfo_vlanType(u8 vlan);
u8 getBcmInfo_vlanType(void);

void bcmApi_setLRCommBreak(void);
void bcmApi_setLRCommNormal(void);
u8 bcmApi_getLRDataSource(void);

#endif
