/**
  ******************************************************************************
  * @file    netmain.h
  * @author  MCD Application Team
  * @version V1.1.0
  * @date    31-July-2013 
  * @brief   This file contains all the functions prototypes for the main.c 
  *          file.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __NETMAIN_H__
#define __NETMAIN_H__

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include "stm32f4x7_eth_bsp.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/

#define DEV_TYPE						0xF115
#define DEV_NAME_LEN					15
#define DEV_NAME_STR					"V4Controller"
#define WEB_PORT_DEF					80
#define IPSCAN_PORT						8992

	 
//#define USE_DHCP       /* enable DHCP, if disabled static address is used */	 

/* MAC ADDRESS: MAC_ADDR0:MAC_ADDR1:MAC_ADDR2:MAC_ADDR3:MAC_ADDR4:MAC_ADDR5 */
#define MAC_ADDR0   					0x00
#define MAC_ADDR1   					0x20
#define MAC_ADDR2   					0xC8
#define MAC_ADDR3   					0x85
#define MAC_ADDR4   					0x64
#define MAC_ADDR5   					0x39
 
/*Static IP ADDRESS: IP_ADDR0.IP_ADDR1.IP_ADDR2.IP_ADDR3 */
#define IP_ADDR0   						172//192 //192 //169
#define IP_ADDR1   						16//168 //168 //254
#define IP_ADDR2   						1  //100 //100 //6
#define IP_ADDR3   						239  //168 //168 //6
   
/*NETMASK*/
#define NETMASK_ADDR0   				255
#define NETMASK_ADDR1   				255
#define NETMASK_ADDR2   				0
#define NETMASK_ADDR3   				0

/*Gateway Address*/
#define GW_ADDR0   						172//192 //10 //192//169
#define GW_ADDR1   						16//168 //86 //168//254
#define GW_ADDR2   						1  //100  //20 //100//6
#define GW_ADDR3   						1

/*在UDP协议中，PC端默认的IP地址(此地址需与main.h中定义的地址在同一网段)**/
#define	UDP_SERVER_IP_ADDR0   						172//192		//169//192
#define UDP_SERVER_IP_ADDR1   						16//168		//254//168
#define UDP_SERVER_IP_ADDR2   						1//100		//6//100
#define UDP_SERVER_IP_ADDR3   						7//2		//9//2

/* MII and RMII mode selection */
#define MODE_MII						1
#define MODE_RMII						2
#define PHY_MODE						MODE_RMII

/* Uncomment the define below to clock the PHY from external 25MHz crystal (only for MII mode) */
#if 	PHY_MODE == MODE_MII
 #define PHY_CLOCK_MCO
#endif

typedef struct
{
	uint16_t  device_type;
	uint8_t   device_name[DEV_NAME_LEN + 1];
	
	ip_addr_t ipaddr;
	ip_addr_t netmask;
	ip_addr_t gateway;
	uint8_t   hwaddr[6];
	uint16_t  web_port;
	
}STR_NCONIFG;

   
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void          Time_Update      (void);  
void          Delay            (uint32_t nCount);

void          NetAddr_Update   (struct netif *netif);
void          NetHwaddr_Update (struct netif *netif);
void          NetSet_Addr      (ip_addr_t *ipaddr, ip_addr_t *netmask, ip_addr_t *gw);
void          NetSet_Hwaddr    (uint8_t* Addr);
void          NetSet_DevName   (uint8_t *name);
void          NetSet_Restore   (void);
STR_NCONIFG  *GetNetConfig     (void);
struct netif *GetCurrentNetif  (void);

void          ipscanserver_init(void);

void 		  GetCurrentIPAddr (u8_t *addr);

#ifdef __cplusplus
}
#endif

#endif /* __NETMAIN_H__ */


