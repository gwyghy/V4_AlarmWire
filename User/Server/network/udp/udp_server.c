/*
*********************************************************************************************************
*	                                            DoubleWires
*                           Commuication Controller For The Mining Industry
*
*                                  (c) Copyright 1994-2013  HNDZ 
*                       All rights reserved.  Protected by international copyright laws.
*
*
*    File    : udp_server.c
*    Module  : user/udp
*    Version : V1.1.0.0
*    Brief   : STM32F4xx DCP
*    History : 
*   -----------------
*              Version  Date           By            Note
*              v1.0        
*
*********************************************************************************************************
 */

 
#include "udp_server.h"
#include <string.h>
#include <stdio.h>
#include "netmain.h"
#include "lwip/pbuf.h"
#include "lwip/udp.h"

 #if (SERVER_NETWORK_UDP_EN ==1)

static UdpServerType UdpServer;
struct ip_addr  ipServerAddr;
//uint8_t udpTestBuf[20]={1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20};

void UdpServerRevCallback(void *arg, struct udp_pcb *upcb, struct pbuf *p, struct ip_addr *addr, u16_t port)
{
	if(p != NULL)
	{
		if(p->len > UDP_FRAME_LENGTH_MAX)	
		{	
			return;
		}
		
		set_phy_linkstatus(1);
		UdpServer.UdpInsertRecvBuf(p->payload, p->len, addr);
		/* Free the p buffer */
		pbuf_free(p);			
	} 
}

void UdpServerInit(UdpServerFunc RecvFunc)
{
	err_t             err;
	
	/* Create a new UDP control block  */
	UdpServer.upcb = udp_new();

	if (UdpServer.upcb)
	{
		/* Bind the upcb to the UDP_PORT port */
		/* Using IP_ADDR_ANY allow the upcb to be used by any local interface */
		err = udp_bind(UdpServer.upcb, IP_ADDR_ANY, UDP_SERVER_PORT);

		if(err == ERR_OK)
		{
			UdpServer.UdpInsertRecvBuf = RecvFunc;
			/* Set a receive callback for the upcb */
			udp_recv(UdpServer.upcb, UdpServerRevCallback, NULL);//设置接收数据时的回调函数
		}
		else
		{
			printf("can not bind pcb\r\n");
		}
		
		//IP4_ADDR(&ipServerAddr, UDP_SERVER_IP_ADDR0, UDP_SERVER_IP_ADDR1, UDP_SERVER_IP_ADDR2, UDP_SERVER_IP_ADDR3);
		//IP4_ADDR(&ipServerAddr, 255, 255, 255, 255);
	}
}

//#define UDPSEND_TEST     //todo

//ip_addr_t addr_pc;


u8 UdpServerSend(u8 *u8Buf, u16 u16BufLength,struct ip_addr *addr)
{
	struct pbuf *send_p;
#if OS_CRITICAL_METHOD == 3                      
    OS_CPU_SR  cpu_sr = 0;
#endif

	OS_ENTER_CRITICAL();
	
#ifdef UDPSEND_TEST
	u8 hostaddr=0;
//	u8 testbuf[5];
	hostaddr = (u8)(addr->addr >>24);
#endif
//	addr_pc.addr = (u32)(172|16<<8|1<<16|233<<24);//IP地址
	
	//IP4_ADDR(addr, IP_ADDR0, IP_ADDR1, 255, 255);
	
	/* allocate pbuf from pool*/
	send_p =  pbuf_alloc(PBUF_TRANSPORT, u16BufLength, PBUF_POOL);
	/* copy data to pbuf */
	pbuf_take(send_p, u8Buf, u16BufLength);
	/* send udp data */
	udp_sendto(UdpServer.upcb, send_p, addr, UDP_SERVER_PORT); 
//	if((addr->addr&0xFF000000) != 0)
//		udp_sendto(UdpServer.upcb, send_p, &addr_pc, UDP_SERVER_PORT);
	/* free pbuf */
	pbuf_free(send_p);
	
//	if((addr->addr&0xff000000) == 0)
//	{
//		send_p =  pbuf_alloc(PBUF_TRANSPORT, u16BufLength, PBUF_POOL);
//		/* copy data to pbuf */
//		pbuf_take(send_p, u8Buf, u16BufLength);
//		/* send udp data */
////		if(send_p->len != 0x0e)
////		if(u8Buf[8] == 5 && u8Buf[10] == 0)
////		{
////			u8Buf[8] = 5;
////		}
//			
//		udp_sendto(UdpServer.upcb, send_p, &addr_pc, UDP_SERVER_PORT); 
//		pbuf_free(send_p);
//	}
	OS_EXIT_CRITICAL();

#ifdef UDPSEND_TEST	
	IP4_ADDR(addr, IP_ADDR0, IP_ADDR1, IP_ADDR2, 117);
//	memset(testbuf,0xff,4);
//	testbuf[4] =hostaddr;
	u8Buf[u16BufLength-1] = hostaddr;
	send_p =  pbuf_alloc(PBUF_TRANSPORT, u16BufLength, PBUF_POOL);
	/* copy data to pbuf */
	pbuf_take(send_p, u8Buf, u16BufLength);
//	pbuf_take(send_p+u16BufLength, testbuf, 5);
	/* send udp data */
	udp_sendto(UdpServer.upcb, send_p, addr, UDP_SERVER_PORT); 
	/* free pbuf */
	pbuf_free(send_p);
#endif

//	/* allocate pbuf from pool*/
//	send_p =  pbuf_alloc(PBUF_TRANSPORT, 20, PBUF_POOL);
//	/* copy data to pbuf */
//	pbuf_take(send_p, u8Buf, 20);
//	IP4_ADDR(&ipServerAddr, 255, 255, 255, 255);
//	/* send udp data */
//	udp_sendto(UdpServer.upcb, send_p, &ipServerAddr/*addr*/, UDP_SERVER_PORT); 
//	/* free pbuf */
//	pbuf_free(send_p);	
		
	return 1;
}

#endif
