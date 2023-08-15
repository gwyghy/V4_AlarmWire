/**
  ******************************************************************************
  * @file    netconf.c
  * @author  MCD Application Team
  * @version V1.1.0
  * @date    31-July-2013
  * @brief   Network connection configuration
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2013 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
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
#include "stm32f4x7_eth_bsp.h"
#include "bsp.h"
#include <stdio.h>
#include <stm32f4xx.h>

#include "tcpip.h"
#include "sys_msg_def.h"
#include "Util_Timer.h"
#include "udp_app_proc.h"

#if ((SERVER_NETWORK_UDP_EN ==1)||(SERVER_NETWOEK_IPSCANER_EN==1))


/* Private typedef -----------------------------------------------------------*/
#define MAX_DHCP_TRIES        4

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static STR_NCONIFG  gnetconfig;
struct netif gnetif;
uint32_t TCPTimer  = 0;
uint32_t ARPTimer  = 0;
uint32_t IPaddress = 0;
u8 ipUpdate=0;
u8 ipUpdate_config=0;
static u32 ipupdate_timerid=0;

#ifdef USE_DHCP
uint32_t DHCPfineTimer   = 0;
uint32_t DHCPcoarseTimer = 0;
__IO uint8_t DHCP_state;
#endif

extern __IO uint32_t  EthStatus;

/* Private functions ---------------------------------------------------------*/
void LwIP_DHCP_Process_Handle(void);


/******************************************************************************************************
                                      LwIP_Init

  @Description : Initializes the lwIP stack
  @Arguments   : none
  @Returns     : none
 ******************************************************************************************************/
void LwIP_Init(void)
{
	STR_NCONIFG *net = GetNetConfig();
	
	/* Initializes the dynamic memory heap defined by MEM_SIZE.*/
	mem_init();

	/* Initializes the memory pools defined by MEMP_NUM_x.*/
	memp_init();
  
#ifdef USE_DHCP
	ipaddr.addr  = 0;
	netmask.addr = 0;
	gw.addr      = 0;
#else
	NetSet_Restore();	
#endif  
  
	/* - netif_add(struct netif *netif, struct ip_addr *ipaddr,
	struct ip_addr *netmask, struct ip_addr *gw,
	void *state, err_t (* init)(struct netif *netif),
	err_t (* input)(struct pbuf *p, struct netif *netif))

	Adds your network interface to the netif_list. Allocate a struct
	netif and pass a pointer to this structure as the first argument.
	Give pointers to cleared ip_addr structures when using DHCP,
	or fill them with sane numbers otherwise. The state pointer may be NULL.

	The init function pointer must point to a initialization function for
	your ethernet netif interface. The following code illustrates it's use.*/
	netif_add(&gnetif, &net->ipaddr, &net->netmask, &net->gateway, NULL, &ethernetif_init, &ethernet_input);

	/*  Registers the default network interface.*/
	netif_set_default(&gnetif);

	if (EthStatus == (ETH_INIT_FLAG | ETH_LINK_FLAG))
	{
		/* Set Ethernet link flag */
		gnetif.flags |= NETIF_FLAG_LINK_UP;

		/* When the netif is fully configured this function must be called */
		netif_set_up(&gnetif);

#ifdef USE_DHCP
		DHCP_state = DHCP_START;
#endif /* USE_DHCP */
	}
	else
	{
		/* When the netif link is down this function must be called */
		netif_set_down(&gnetif);

#ifdef USE_DHCP
		DHCP_state = DHCP_LINK_DOWN;
#endif /* USE_DHCP */
	}

	/* Set the link callback function, this function is called on change of link status*/
	netif_set_link_callback(&gnetif, ETH_link_callback);
}

/******************************************************************************************************
                                      LwIP_Pkt_Handle

  @Description : Called when a frame is received
  @Arguments   : none
  @Returns     : none
 ******************************************************************************************************/
void LwIP_Pkt_Handle(void)
{
	/* Read a received packet from the Ethernet buffers and send it to the lwIP for handling */
	ethernetif_input(&gnetif);
}

/******************************************************************************************************
                                      LwIP_Periodic_Handle

  @Description : LwIP periodic tasks
  @Arguments   : localtime the current LocalTime value
  @Returns     : none
 ******************************************************************************************************/
void LwIP_Periodic_Handle(uint32_t period)
{
#if LWIP_TCP
	/* TCP periodic process every 250 ms */
	TCPTimer += period;
	if (TCPTimer >= TCP_TMR_INTERVAL)
	{
		TCPTimer =  0;
		tcp_tmr();
	}
#endif
  
	/* ARP periodic process every 5s */
	ARPTimer += period;
	if (ARPTimer >= ARP_TMR_INTERVAL)
	{
		ARPTimer =  0;
		etharp_tmr();
	}
  
#ifdef USE_DHCP
	/* Fine DHCP periodic process every 500ms */
	DHCPfineTimer += period;
	if (DHCPfineTimer >= DHCP_FINE_TIMER_MSECS)
	{
		DHCPfineTimer =  0;
		dhcp_fine_tmr();
		if ((DHCP_state != DHCP_ADDRESS_ASSIGNED) && 
			(DHCP_state != DHCP_TIMEOUT) &&
			(DHCP_state != DHCP_LINK_DOWN))
		{
			/* toggle LED1 to indicate DHCP on-going process */
			STM_EVAL_LEDToggle(LED1);

			/* process DHCP state machine */
			LwIP_DHCP_Process_Handle();    
		}
	}

	/* DHCP Coarse periodic process every 60s */
	DHCPcoarseTimer += period
	if (DHCPcoarseTimer >= DHCP_COARSE_TIMER_MSECS)
	{
		DHCPcoarseTimer =  0;
		dhcp_coarse_tmr();
	}

#endif
}

#ifdef USE_DHCP
/******************************************************************************************************
                                      LwIP_DHCP_Process_Handle

  @Description : LwIP_DHCP_Process_Handle
  @Arguments   : none
  @Returns     : none
 ******************************************************************************************************/
void LwIP_DHCP_Process_Handle()
{
	struct ip_addr ipaddr;
	struct ip_addr netmask;
	struct ip_addr gw;
	uint8_t iptab[4] = {0};
	uint8_t iptxt[20];
  
	switch (DHCP_state)
	{
		case DHCP_START:
			DHCP_state = DHCP_WAIT_ADDRESS;
			dhcp_start(&gnetif);
			/* IP address should be set to 0 every time we want to assign a new DHCP address */
			IPaddress = 0;
			break;

		case DHCP_WAIT_ADDRESS:
			/* Read the new IP address */
			IPaddress = gnetif.ip_addr.addr;

			if (IPaddress!=0) 
			{
				DHCP_state = DHCP_ADDRESS_ASSIGNED;	

				/* Stop DHCP */
				dhcp_stop(&gnetif);
			}
			else
			{
				/* DHCP timeout */
				if (gnetif.dhcp->tries > MAX_DHCP_TRIES)
				{
					DHCP_state = DHCP_TIMEOUT;

					/* Stop DHCP */
					dhcp_stop(&gnetif);

					/* Static address used */
					IP4_ADDR(&ipaddr, IP_ADDR0 ,IP_ADDR1 , IP_ADDR2 , IP_ADDR3 );
					IP4_ADDR(&netmask, NETMASK_ADDR0, NETMASK_ADDR1, NETMASK_ADDR2, NETMASK_ADDR3);
					IP4_ADDR(&gw, GW_ADDR0, GW_ADDR1, GW_ADDR2, GW_ADDR3);
					netif_set_addr(&gnetif, &ipaddr , &netmask, &gw);
				}
			}
			break;
	
		default: 
			break;
	}
}
#endif

/******************************************************************************************************
                                      NetAddr_Update

  @Description : Net address update
  @Arguments   : netif the already initialized lwip network interface structure for this ethernetif
  @Returns     : none
  @note        : IP地址更新，仅在运行中修改IP地址后，调用该函数；
                 修改IP地址步骤：
                    step1  调用NetSet_Addr函数，将新的IP地址写入系统参数中
                    step2  调用存储函数，存储系统参数
                    step3  调用NetAddr_Update函数更新
 ******************************************************************************************************/
void NetAddr_Update(struct netif *netif)
{
	STR_NCONIFG* net    = &gnetconfig;
	
	netif_set_addr(netif, &net->ipaddr, &net->netmask, &net->gateway);
}

/******************************************************************************************************
                                      NetHwaddr_Update

  @Description : Net hardware address update
  @Arguments   : netif the already initialized lwip network interface structure for this ethernetif
  @Returns     : none
  @note        : 物理地址更新，仅在运行中修改物理地址后，调用该函数
                 修改物理地址步骤：
                   step1：调用NetSet_Hwaddr函数，将新的IP地址写入系统参数中
                   step2：调用存储函数，存储系统参数
                   step3：调用NetHwaddr_Update函数更新
 ******************************************************************************************************/
void NetHwaddr_Update(struct netif *netif)
{
	STR_NCONIFG* net    = &gnetconfig;
	if(netif != NULL){
		netif->hwaddr_len = ETHARP_HWADDR_LEN;
		
		netif->hwaddr[0]  =  net->hwaddr[0];
		netif->hwaddr[1]  =  net->hwaddr[1];
		netif->hwaddr[2]  =  net->hwaddr[2];
		netif->hwaddr[3]  =  net->hwaddr[3];
		netif->hwaddr[4]  =  net->hwaddr[4];
		netif->hwaddr[5]  =  net->hwaddr[5];
	}
  
	ETH_MACAddressConfig(ETH_MAC_Address0, net->hwaddr); 	
}

/******************************************************************************************************
                                      NetSet_Addr

  @Description : Net set address
  @Arguments   : ipaddr,netmask,gw
  @Returns     : none
 ******************************************************************************************************/
void NetSet_Addr(ip_addr_t *ipaddr, ip_addr_t *netmask, ip_addr_t *gw)
{
	STR_NCONIFG* net = &gnetconfig;
	
	net->ipaddr.addr  = ipaddr->addr;
	net->netmask.addr = netmask->addr;
	net->gateway.addr = gw->addr;	
}

/******************************************************************************************************
                                      NetSet_Hwaddr

  @Description : Net set hardware address
  @Arguments   : hardware address
  @Returns     : none
 ******************************************************************************************************/
void NetSet_Hwaddr(uint8_t* Addr)
{
	STR_NCONIFG* net = &gnetconfig;

	net->hwaddr[0] = *Addr;
	net->hwaddr[1] = *(Addr+1);
	net->hwaddr[2] = *(Addr+2);
	net->hwaddr[3] = *(Addr+3);
	net->hwaddr[4] = *(Addr+4);
	net->hwaddr[5] = *(Addr+5);
	
}

/******************************************************************************************************
                                      NetSet_DevName

  @Description : Net set device name
  @Arguments   : device name
  @Returns     : none
 ******************************************************************************************************/
void NetSet_DevName(uint8_t *name)
{
	uint32_t len;
	STR_NCONIFG* net = &gnetconfig;
	
	len = strlen((char*)(name));
	if (len > DEV_NAME_LEN)
	{
		return;
	}
	strcpy((char*)(&net->device_name[0]),(char*)(name));
	
}

/******************************************************************************************************
                                      NetSet_Restore

  @Description : Net set device name
  @Arguments   : none
  @Returns     : none
  @notes       : 网络参数恢复出厂默认值
 ******************************************************************************************************/
void NetSet_Restore(void)
{
	int i;
	STR_NCONIFG* net = &gnetconfig;

#if 0
	IP4_ADDR(&(net->ipaddr),  IP_ADDR0, IP_ADDR1, IP_ADDR2, IP_ADDR3);
	IP4_ADDR(&(net->netmask), NETMASK_ADDR0, NETMASK_ADDR1 , NETMASK_ADDR2, NETMASK_ADDR3);
	IP4_ADDR(&(net->gateway), GW_ADDR0, GW_ADDR1, GW_ADDR2, GW_ADDR3);

	net->hwaddr[0]   =  MAC_ADDR0;
	net->hwaddr[1]   =  MAC_ADDR1;
	net->hwaddr[2]   =  MAC_ADDR2;
	net->hwaddr[3]   =  MAC_ADDR3;
	net->hwaddr[4]   =  MAC_ADDR4;
	net->hwaddr[5]   =  MAC_ADDR5;
#else
	IP4_ADDR(&(net->ipaddr),  SysNetParam.NetParam_IPAddress[0], SysNetParam.NetParam_IPAddress[1], SysNetParam.NetParam_IPAddress[2], SysNetParam.NetParam_IPAddress[3]);
	IP4_ADDR(&(net->netmask), SysNetParam.NetParam_NetMask[0], SysNetParam.NetParam_NetMask[1] , SysNetParam.NetParam_NetMask[2], SysNetParam.NetParam_NetMask[3]);
	IP4_ADDR(&(net->gateway), SysNetParam.NetParam_GateWay[0], SysNetParam.NetParam_GateWay[1], SysNetParam.NetParam_GateWay[2], SysNetParam.NetParam_GateWay[3]);

	for(i=0; i<6;i++)
		net->hwaddr[i]   =  SysNetParam.NetParam_Mac[i];

#endif
	
	net->web_port    = WEB_PORT_DEF;
	
	net->device_type = DEV_TYPE;
	
}

/******************************************************************************************************
                                      GetNetConfig

  @Description : 取得指向网络参数的指针
  @Arguments   : none
  @Returns     : none
 ******************************************************************************************************/
STR_NCONIFG *GetNetConfig(void)
{
	return(&gnetconfig);
}

struct netif *GetCurrentNetif(void)
{
	return(&gnetif);
}

/******************************************************************************************************
                                      GetCurrentIPAddr

  @Description : 取得设备IP地址
  @Arguments   : none
  @Returns     : none
 ******************************************************************************************************/
void GetCurrentIPAddr(u8_t *addr)
{
	STR_NCONIFG* net = &gnetconfig;
	u32_t        temp= net->ipaddr.addr;
	
#if BYTE_ORDER == BIG_ENDIAN
	*addr     = (u8_t)((temp&0xff000000)>>24);
	*(addr+1) = (u8_t)((temp&0x00ff0000)>>16);
	*(addr+2) = (u8_t)((temp&0x0000ff00)>>8);
	*(addr+3) = (u8_t)(temp&0x000000ff);
#else
	*(addr+3) = (u8_t)((temp&0xff000000)>>24);
	*(addr+2) = (u8_t)((temp&0x00ff0000)>>16);
	*(addr+1) = (u8_t)((temp&0x0000ff00)>>8);
	*addr     = (u8_t)(temp&0x000000ff);
#endif

}

void TimerCallback_UpdateIP(u16 data)
{
	ipUpdate_config =1;
	deleteTimerEvent(ipupdate_timerid);
}

void UpdateIPConfig(void)
{	
	STR_NCONIFG* net = &gnetconfig;
	
	if(ipUpdate)
	{	
		ipupdate_timerid = addTimerEvent(5000, 1, TimerCallback_UpdateIP);  //10ms
		if(ipupdate_timerid != 0xff)
		{
			startTimerEvent(ipupdate_timerid);
		}
		else
			ipupdate_timerid=0;
			
		ipUpdate=0;
	}
	
	if(ipUpdate_config)
	{		
		IP4_ADDR(&(net->ipaddr),  SysNetParam.NetParam_IPAddress[0], SysNetParam.NetParam_IPAddress[1], SysNetParam.NetParam_IPAddress[2], SysNetParam.NetParam_IPAddress[3]);
		IP4_ADDR(&(net->netmask), SysNetParam.NetParam_NetMask[0], SysNetParam.NetParam_NetMask[1] , SysNetParam.NetParam_NetMask[2], SysNetParam.NetParam_NetMask[3]);
		IP4_ADDR(&(net->gateway), SysNetParam.NetParam_GateWay[0], SysNetParam.NetParam_GateWay[1], SysNetParam.NetParam_GateWay[2], SysNetParam.NetParam_GateWay[3]);	
				
		netif_set_down(&gnetif);
		setUdpLinkParam(SysNetParam.NetParam_IPAddress[0], SysNetParam.NetParam_IPAddress[1], SysNetParam.NetParam_IPAddress[2], SysNetParam.NetParam_IPAddress[3],SysNetParam.deviceType);
		NetAddr_Update(&gnetif);		
		netif_set_up(&gnetif);

		ipUpdate_config =0;
		SetSysFuncMode(SYSMODE_CONFIG_FIN);
	}
}

void setUpdateFlag(u8 flag)
{
	ipUpdate = flag;
}

#endif
