#ifndef __DEVMNG_API_H__
#define __DEVMNG_API_H__

#include "includes.h"
#include "devmng.h"

	

u8 read_data (uint8_t SupportId,uint8_t DriverId,uint8_t DeviceId,uint8_t ChildDeviceId,uint8_t PointId,void *pdata);
u8 write_data(uint8_t SupportId,uint8_t DriverId,uint8_t DeviceId,uint8_t ChildDeviceId,uint8_t PointId,void *pdata);
u8 get_device_state(uint8_t SupportId,uint8_t DriverId,uint8_t DeviceId,uint8_t ChildDeviceId,OnlineType *state);
u8 ctrl_phone(uint8_t PhoneId,void *pdata);

#endif
