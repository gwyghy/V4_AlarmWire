#ifndef __UDP_APP_CALLBACK_H
#define __UDP_APP_CALLBACK_H

#include "udp_app_proc.h"

u8 udpAppCB_readState_request(UDPFrameType *pframe);
u8 udpAppCB_request_outtime(UDPFrameType *pframe);
u8 udpAppCB_readState_confirm(UDPFrameType *pframe);
u8 udpAppCB_readState_multiple_request(UDPFrameType *pframe);
u8 udpAppCB_readState_multiple_confirm(UDPFrameType *pframe);
u8 udpAppCB_stateReport_indication(UDPFrameType *pframe);

u8 udpAppCB_controlDevice_request(UDPFrameType *pframe);
u8 udpAppCB_request_ack(UDPFrameType *pframe);
u8 udpAppCB_manualControlDevice_request(UDPFrameType *pframe);

u8 udpAppCB_writeStateDevice_request(UDPFrameType *pframe);

u8 udpAppCB_setParam_request(UDPFrameType *pframe);
u8 udpAppCB_setParam_multiple_request(UDPFrameType *pframe);
u8 udpAppCB_checkParam_request(UDPFrameType *pframe);
u8 udpAppCB_checkParam_confirm(UDPFrameType *pframe);
u8 udpAppCB_setParam_UIMode_request(UDPFrameType *pframe);
u8 udpAppCB_serverOperate_request(UDPFrameType *pframe);

u8 udpAppCB_enterCfgMode_request(UDPFrameType *pframe);
u8 udpAppCB_setAddr_request(UDPFrameType *pframe);
u8 udpAppCB_setAddr_confirm(UDPFrameType *pframe);
u8 udpAppCB_netHeart_request(UDPFrameType *pframe);
u8 udpAppCB_timeSync_request(UDPFrameType *pframe);
u8 udpAppCB_commCheck_request(UDPFrameType *pframe);
u8 udpAppCB_commCheck_confirm(UDPFrameType *pframe);
u8 udpAppCB_followControlDevice_confirm(UDPFrameType *pframe);
u8 udpAppCB_serverDisplays_confirm(UDPFrameType *pframe);
u8 udpAppCB_serverRevise_confirm(UDPFrameType *pframe);

u8 udpAppCB_recoilControlDevice_confirm(UDPFrameType *pframe);
u8 udpAppCB_serverINS_request(UDPFrameType *pframe);
u8 udpAppCB_sysParamSynchronous_request(UDPFrameType *pframe);
u8 udpAppCB_ykqDecode_request(UDPFrameType *pframe);
u8 udpAppCB_arpNetMaintain_request(UDPFrameType *pframe);
u8 udpAppCB_TransportPlane_request(UDPFrameType *pframe);
#endif
