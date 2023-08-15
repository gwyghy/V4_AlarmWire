#ifndef __UDP_UPDATE_CALLBACK_H
#define __UDP_UPDATE_CALLBACK_H

#include "udp_app_proc.h"

#define  UDPUPDATE_FILE_SIZE_LEN	4
#define  UDPUPDATE_FILE_TYPE_LEN	FILESUFFIXTYPE
#define  UDPUPDATE_FILE_NAME_LEN	FILENAMESIZE


u8 udpUpdateCB_UpdateStart_request(UDPFrameType *pframe);
u8 udpUpdateCB_request_outtime(UDPFrameType *pframe);
u8 udpUpdateCB_SendData_request(UDPFrameType *pframe);
u8 udpUpdateCB_RecvDataAck_request(UDPFrameType *pframe);
u8 udpUpdateCB_DataCRC_request(UDPFrameType *pframe);
u8 udpUpdateCB_DataCRC_confirm(UDPFrameType *pframe);
u8 udpUpdateCB_UpdateNow_request(UDPFrameType *pframe);

#endif
