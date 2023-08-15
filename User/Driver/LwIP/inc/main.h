/**
  ******************************************************************************
  * @file    main.h
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
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include "stm32f4x7_eth_bsp.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/

#define USE_LCD        /* enable LCD  */
//#define USE_DHCP       /* enable DHCP, if disabled static address is used */
   
/* Uncomment SERIAL_DEBUG to enables retarget of printf to serial port
   for debug purpose */   
//#define SERIAL_DEBUG 

 
/* MAC ADDRESS: MAC_ADDR0:MAC_ADDR1:MAC_ADDR2:MAC_ADDR3:MAC_ADDR4:MAC_ADDR5 */
#define MAC_ADDR0   					0x00
#define MAC_ADDR1   					0x20
#define MAC_ADDR2   					0xC8
#define MAC_ADDR3   					0x85
#define MAC_ADDR4   					0x64
#define MAC_ADDR5   					0x09
 
/*Static IP ADDRESS: IP_ADDR0.IP_ADDR1.IP_ADDR2.IP_ADDR3 */
#define IP_ADDR0   						192
#define IP_ADDR1   						168
#define IP_ADDR2   						100
#define IP_ADDR3   						168
   
/*NETMASK*/
#define NETMASK_ADDR0   				255
#define NETMASK_ADDR1   				255
#define NETMASK_ADDR2   				255
#define NETMASK_ADDR3   				0

/*Gateway Address*/
#define GW_ADDR0   						192
#define GW_ADDR1   						168
#define GW_ADDR2   						100
#define GW_ADDR3   						1

/* MII and RMII mode selection */
#define MODE_MII						1
#define MODE_RMII						2
#define PHY_MODE						MODE_RMII

/* Uncomment the define below to clock the PHY from external 25MHz crystal (only for MII mode) */
#if 	PHY_MODE == MODE_MII
 #define PHY_CLOCK_MCO
#endif

   
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void Time_Update(void);  
void Delay(uint32_t nCount);


#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */


