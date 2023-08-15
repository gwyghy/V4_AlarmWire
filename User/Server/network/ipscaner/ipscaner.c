/*
*********************************************************************************************************
*	                                            DoubleWires
*                           Commuication Controller For The Mining Industry
*
*                                  (c) Copyright 1994-2013  HNDZ 
*                       All rights reserved.  Protected by international copyright laws.
*
*
*    File    : ipscaner.c
*    Module  : user
*    Version : V1.0
*    History :
*   -----------------
*              Version  Date           By            Note
*              v1.0     
*********************************************************************************************************
*/

#include "lwip/mem.h"
#include "lwip/memp.h"
#include "lwip/tcp.h"
#include "lwip/tcp_impl.h"
#include "lwip/udp.h"
#include "netif/etharp.h"
#include "lwip/dhcp.h"
#include "ethernetif.h"
#include "netmain.h"
#include "netconf.h"
#include "stm32f4x7_eth.h"
#include "bsp.h"
#include <stdio.h>
#include <stm32f4xx.h>

#if (SERVER_NETWOEK_IPSCANER_EN==1)

#define IPSCAN_BYTE1	0x55
#define IPSCAN_BYTE2	0xAA
#define IPSCAN_BYTE3	0x00
#define IPSCAN_BYTE4	0x00

#define IPSET_BYTE1		0x11
#define IPSET_BYTE2		0x11
#define IPSET_BYTE3		0x11
#define IPSET_BYTE4		0x11


char ethsend_ip(struct netif *netif, struct pbuf *p, struct eth_addr *src, struct eth_addr *dst)
{
	struct eth_hdr *ethhdr = (struct eth_hdr *)p->payload;

	ETHADDR32_COPY(&ethhdr->dest, dst);
	ETHADDR16_COPY(&ethhdr->src, src);
	ethhdr->type = PP_HTONS(ETHTYPE_IP);

	return netif->linkoutput(netif, p);
}

void IpHdrExchange(struct ip_hdr *src_hdr,struct ip_hdr *dst_hdr,uint16_t len,STR_NCONIFG *net)
{  
	dst_hdr->_v_hl     = src_hdr->_v_hl;
	dst_hdr->_tos      = src_hdr->_tos;
	dst_hdr->_len      = htons(len);
	dst_hdr->_id       = src_hdr->_id;
	dst_hdr->_offset   = src_hdr->_offset;
	dst_hdr->_ttl      = src_hdr->_ttl;
	dst_hdr->_proto    = src_hdr->_proto;
	dst_hdr->_chksum   = 0x0000;
	dst_hdr->src.addr  = net->ipaddr.addr;
	dst_hdr->dest.addr = src_hdr->src.addr;

}

/******************************************************************************************************
                                      ipscan_receive_callback

  @Description : This function is called when an UDP ip scan packet has been received on the port UDP_PORT.
  @Arguments   : arg  - user supplied argument(udp_pcb.recv_arg)
                 pcb  - the udp_pcb which received data
				 p    - the packet buffer that was received
				 addr - the remote ip address from which the packet was received
				 port - the remote port from which the packet was received
  @Returns     : none
 ******************************************************************************************************/
void ipscan_receive_callback(void *arg, struct udp_pcb *upcb, struct pbuf *p, struct ip_addr *addr, u16_t port)
{
	struct pbuf *pkt;
	uint8_t *recdata = p->payload;
	
	// received ip scanner packet
	if ((*recdata==IPSCAN_BYTE1)&&(*(recdata+1)==IPSCAN_BYTE2)&&(*(recdata+2)==IPSCAN_BYTE3)&&(*(recdata+2)==IPSCAN_BYTE4))
	{
		uint32_t i;
		struct udp_hdr *udphdr;
		struct eth_hdr *eth_hdr;
		struct eth_addr eth_local;
		uint8_t *sendata;
		STR_NCONIFG *net = GetNetConfig();
		
		pkt = pbuf_alloc(PBUF_RAW,(SIZEOF_ETH_HDR+IP_HLEN+UDP_HLEN+88), PBUF_POOL);

		pbuf_header(pkt, -(SIZEOF_ETH_HDR+IP_HLEN));
		udphdr         = (struct udp_hdr *)pkt->payload;
		udphdr->src    = htons(upcb->local_port);
		udphdr->dest   = htons(port);
		udphdr->len    = htons(pkt->tot_len);
		udphdr->chksum = 0x0000; 
		
		pbuf_header(pkt, -UDP_HLEN);
		sendata = pkt->payload;
		
		for (i=0;i<88;i++)	*(sendata+i) = 0x00;
		
		*(sendata+4)  = (uint8_t)(net->device_type);
 		*(sendata+5)  = (uint8_t)(net->device_type>>8);
		
		
		*(sendata+6)  = net->hwaddr[0];
		*(sendata+7)  = net->hwaddr[1];
		*(sendata+8)  = net->hwaddr[2];
		*(sendata+9)  = net->hwaddr[3];
		*(sendata+10) = net->hwaddr[4];
		*(sendata+11) = net->hwaddr[5];
			
		*(sendata+20) = ip4_addr1(&net->ipaddr.addr);
		*(sendata+21) = ip4_addr2(&net->ipaddr.addr);
		*(sendata+22) = ip4_addr3(&net->ipaddr.addr);
		*(sendata+23) = ip4_addr4(&net->ipaddr.addr);
		
		*(sendata+24) = ip4_addr1(&net->gateway.addr);
		*(sendata+25) = ip4_addr2(&net->gateway.addr);
		*(sendata+26) = ip4_addr3(&net->gateway.addr);
		*(sendata+27) = ip4_addr4(&net->gateway.addr);
		
		*(sendata+28) = ip4_addr1(&net->netmask.addr);
		*(sendata+29) = ip4_addr2(&net->netmask.addr);
		*(sendata+30) = ip4_addr3(&net->netmask.addr);
		*(sendata+31) = ip4_addr4(&net->netmask.addr);
			
		for (i=0; i<16;i++)
		{
			*(sendata+48+i) = net->device_name[i];
		}
		
		*(sendata+84)  = (uint8_t)(net->web_port);	
		*(sendata+85)  = (uint8_t)(net->web_port>>8);
		
		
#if CHECKSUM_GEN_UDP
		/* 软件校验，此处需要添加代码 */
#endif
		pbuf_header(pkt, (IP_HLEN + UDP_HLEN));		
		pbuf_header(p,   (IP_HLEN + UDP_HLEN));
		IpHdrExchange((struct ip_hdr *)p->payload,(struct ip_hdr *)pkt->payload,(IP_HLEN + UDP_HLEN + 88),GetNetConfig());
		
		pbuf_header(pkt, SIZEOF_ETH_HDR);		
		pbuf_header(p,   SIZEOF_ETH_HDR);
		eth_hdr = (struct eth_hdr *)p->payload;		
		eth_local.addr[0] = net->hwaddr[0];
		eth_local.addr[1] = net->hwaddr[1];
		eth_local.addr[2] = net->hwaddr[2];
		eth_local.addr[3] = net->hwaddr[3];
		eth_local.addr[4] = net->hwaddr[4];
		eth_local.addr[5] = net->hwaddr[5];
		ethsend_ip(GetCurrentNetif(),pkt,&eth_local,&eth_hdr->src);
		
		pbuf_free(pkt);
	}
	else 
	{
		
	}
	pbuf_free(p);
   
}
/******************************************************************************************************
                                      ipscanserver_init

  @Description : Initialize the server application.
  @Arguments   : none
  @Returns     : none
 ******************************************************************************************************/
void ipscanserver_init(void)
{
	struct udp_pcb *upcb;
	err_t err;

	upcb = udp_new();

	if (upcb)
	{
		err = udp_bind(upcb, IP_ADDR_ANY, IPSCAN_PORT);

		if (err == ERR_OK)
		{
			udp_recv(upcb, ipscan_receive_callback, NULL);
		}
		else
		{
			udp_remove(upcb);
		}
	}
	else
	{
		/* can not create pcb */
	} 
}

#endif
