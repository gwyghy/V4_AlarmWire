/*
*********************************************************************************************************
*	                                            DoubleWires
*                           Commuication Controller For The Mining Industry
*
*                                  (c) Copyright 1994-2013  HNDZ 
*                       All rights reserved.  Protected by international copyright laws.
*
*
*    File    : stm32f4x7_eth_bsp.h
*    Module  : user
*    Version : V1.1.0.0
*    Brief   : Header for stm32f4x7_eth_bsp.c file.
*    History : 
*   -----------------
*              Version  Date           By            Note
*              v1.0     2013-12-11     
*
*********************************************************************************************************
*/
#ifndef __STM32F4x7_ETH_BSP_H__
#define __STM32F4x7_ETH_BSP_H__

#ifdef __cplusplus
 extern "C" {
#endif

#include "netif.h"
	 
#define DP83848_PHY_ADDRESS             ((uint16_t) 0x01)   //1 --bt(551只有port0~4有pyh地址)

/* Specific defines for EXTI line, used to manage Ethernet link status */
#define ETH_LINK_EXTI_LINE              EXTI_Line14
#define ETH_LINK_EXTI_PORT_SOURCE       EXTI_PortSourceGPIOB
#define ETH_LINK_EXTI_PIN_SOURCE        EXTI_PinSource14
#define ETH_LINK_EXTI_IRQn              EXTI15_10_IRQn 
	 
/* PB13 */
#define ETH_LINK_PIN                    GPIO_Pin_13
#define ETH_LINK_GPIO_PORT              GPIOB
#define ETH_LINK_GPIO_CLK               RCC_AHB1Periph_GPIOB

/* Ethernet Flags for EthStatus variable */   
#define ETH_INIT_FLAG                   0x01	/* Ethernet Init Flag   */
#define ETH_LINK_FLAG           	    0x10	/* Ethernet Link Flag   */
#define ETH_TASK_OK_FLAG				0x0100  /* Ethernet Task Create */


void     EthStatus_Config     (uint32_t Flag, FunctionalState NewState);
ITStatus EthStatus_Get        (uint32_t Flag);
void     ETH_BSP_Config       (void);
uint32_t Eth_Link_PHYITConfig (uint16_t PHYAddress);
void     Eth_Link_EXTIConfig  (void);
void     Eth_Link_ITHandler   (void);
void     ETH_link_callback    (struct netif *netif);
void 	 set_phy_linkstatus	  (uint32_t linkstatus);
uint32_t Eth_Link_Check       (uint16_t PHYAddress);
uint32_t get_phylinkstatus		(void);
void     Eth_Link_Stop		   (void);
void set_phy_resetdata(uint32_t res);
uint32_t get_phy_resetdata(void);
void bttest_eth(void);

#ifdef __cplusplus
}
#endif

#endif /* __STM32F4x7_ETH_BSP_H__ */
