#ifndef __UDP_SERVER_H
#define __UDP_SERVER_H

#include "stm32f4xx.h"
#include "stm32f4x7_eth_bsp.h"


#define UDP_SERVER_PORT   				18000
#define UDP_FRAME_LENGTH_MAX			1024


typedef void (*UdpServerFunc)(u8 *pdata, u16 length, struct ip_addr *addr);

typedef struct {
	struct udp_pcb *upcb;
	UdpServerFunc   UdpInsertRecvBuf;
} UdpServerType;


void UdpServerInit(UdpServerFunc RecvFunc);
u8 UdpServerSend(u8 *u8Buf, u16 u16BufLength,struct ip_addr *addr);

#endif /* _UDP_SERVER_H */

