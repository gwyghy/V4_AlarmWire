/*
*********************************************************************************************************
*	                                            DoubleWires
*                           Commuication Controller For The Mining Industry
*
*                                  (c) Copyright 1994-2013  HNDZ 
*                       All rights reserved.  Protected by international copyright laws.
*
*
*    File    : stm32f4x7_eth_bsp.c
*    Module  : user
*    Version : V1.1.0.0
*    Brief   : STM32F4x7 Ethernet hardware configuration.
*    History : 
*   -----------------
*              Version  Date           By            Note
*              v1.0     2013-12-11     
*              v1.1     2014-06-27     FanHaiCHun
*
*********************************************************************************************************
*/
#include  <includes.h>

#if ((SERVER_NETWORK_UDP_EN ==1)||(SERVER_NETWOEK_IPSCANER_EN==1))

#include "bsp.h"
#include "lwip/opt.h"
#include "stm32f4x7_eth.h"
#include "stm32f4x7_eth_bsp.h"
#include "netmain.h"
#include "netif.h"
#include "netconf.h"
#include "lwip/dhcp.h"

#define  PHY_DEBUG_MODE					0

ETH_InitTypeDef ETH_InitStructure;
__IO uint32_t   EthStatus = 0;
__IO uint32_t   PhyLinkStatus = 0;  //网络连接状态 0表示初始连接 1：表示网络连接已经有通讯
//__IO uint32_t   PhyResetData = 0;  // 0表示未接受到数据 1：表示已经接受到数据

extern struct netif gnetif;
#ifdef USE_DHCP
extern __IO uint8_t DHCP_state;
#endif /* LWIP_DHCP */

void        EXTI15_10_IRQFunction(void);
uint32_t    Eth_ITConfig(void);

static void ETH_GPIO_Config(void);
static void ETH_MACDMA_Config(void);
static void reETH_MACDMA_Config(void);

void set_phy_linkstatus(uint32_t linkstatus)
{
	PhyLinkStatus = linkstatus;
}
uint32_t get_phylinkstatus(void)
{
	return PhyLinkStatus;
}
// void set_phy_resetdata(uint32_t res)
// {
// 	PhyResetData = res;
// }
// uint32_t get_phy_resetdata(void)
// {
// 	return PhyResetData;
// }
void EthStatus_Config(uint32_t Flag, FunctionalState NewState)
{
	if (NewState != DISABLE)
	{	
		EthStatus |= Flag;
	}
	else
	{	
		EthStatus &=(~(uint32_t)Flag);
	}
}
ITStatus EthStatus_Get(uint32_t Flag)
{  
	if ((EthStatus & Flag) != (uint32_t)RESET)
	{
		return(SET);
	}
	
	return(RESET);	
}

#if (PHY_DEBUG_MODE==1)
/******************************************************************************************************
                                          ETH_PHY_RegPrint
  @Description : Print the register value, only for debug..
  @Arguments   : none
  @Returns     : none
 ******************************************************************************************************/
void ETH_PHY_RegPrint(uint16_t PHYAddress)
{
	uint16_t tempReg;
	uint32_t addr;
	
	RUN_TRACE("DP83848C Register List: \r\n");
	for (addr=0;addr<8;addr++)
	{
		tempReg = ETH_ReadPHYRegister(PHYAddress, addr);
		RUN_TRACE(" Register%2d=0x%4x\r\n",addr,tempReg);
	}
	addr = 0x10;
	tempReg = ETH_ReadPHYRegister(PHYAddress, addr);
	RUN_TRACE(" Register%2d=0x%4x\r\n",addr,tempReg);
	
	addr = 0x17;
	tempReg = ETH_ReadPHYRegister(PHYAddress, addr);
	RUN_TRACE(" Register%2d=0x%4x\r\n",addr,tempReg);
}
#endif
/******************************************************************************************************
                                          ETH_BSP_Config
  @Description : Ethernet Config
  @Arguments   : none
  @Returns     : none
 ******************************************************************************************************/
void ETH_BSP_Config(void)
{	
	/* Configure the GPIO ports for ethernet pins */
	ETH_GPIO_Config();
	
	/* Configure the Ethernet MAC/DMA */
	ETH_MACDMA_Config();
	
	/* Read PHY status register: Get Ethernet link status */
	if (ETH_ReadPHYRegister(DP83848_PHY_ADDRESS, PHY_SR) & 1)
	{
		EthStatus |= ETH_LINK_FLAG;
	}
	
	/* Configure the PHY to generate an interrupt on change of link status */
	Eth_Link_PHYITConfig(DP83848_PHY_ADDRESS);
	
#if (PHY_DEBUG_MODE==1)
	ETH_PHY_RegPrint(DP83848_PHY_ADDRESS);     
#endif	
	
	/* Configure the EXTI for Ethernet link status */
	Eth_Link_EXTIConfig();
	
	BSP_IntEn(EXTI15_10_IRQn);
	BSP_IntVectSet(EXTI15_10_IRQn, EXTI15_10_IRQFunction);
	
	Eth_ITConfig();
}

void reETH_BSP_Config(void)
{	
	/* Configure the GPIO ports for ethernet pins */
	ETH_GPIO_Config();
	
	/* Configure the Ethernet MAC/DMA */
	reETH_MACDMA_Config();
	
	/* Read PHY status register: Get Ethernet link status */
	if (ETH_ReadPHYRegister(DP83848_PHY_ADDRESS, PHY_SR) & 1)
	{
		EthStatus |= ETH_LINK_FLAG;
	}
	
	/* Configure the PHY to generate an interrupt on change of link status */
	Eth_Link_PHYITConfig(DP83848_PHY_ADDRESS);
	
#if (PHY_DEBUG_MODE==1)
	ETH_PHY_RegPrint(DP83848_PHY_ADDRESS);     
#endif	
	
	/* Configure the EXTI for Ethernet link status */
	Eth_Link_EXTIConfig();
	
	BSP_IntEn(EXTI15_10_IRQn);
	BSP_IntVectSet(EXTI15_10_IRQn, EXTI15_10_IRQFunction);
	
	Eth_ITConfig();
}

// void reETH_BSP_Config(void)
// {		
// 	/* Configure the Ethernet MAC/DMA */
// //  	reETH_MACDMA_Config();
// //  	if(EthStatus != 0)
// // 		EthStatus |= ETH_LINK_FLAG;
// 	
// // 	/* Configure the EXTI for Ethernet link status */
// // 	Eth_Link_EXTIConfig();
// // 	
// // 	BSP_IntEn(EXTI15_10_IRQn);
// // 	BSP_IntVectSet(EXTI15_10_IRQn, EXTI15_10_IRQFunction);
// // 	
// // 	Eth_ITConfig();
// 	  ETH_BSP_Config();
// }
/******************************************************************************************************
                                          ETH_MACDMA_Config

  @Description : Configures the Ethernet Interface
  @Arguments   : none
  @Returns     : none
 ******************************************************************************************************/
static void ETH_MACDMA_Config(void)
{
	/* Enable ETHERNET clock  */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_ETH_MAC | RCC_AHB1Periph_ETH_MAC_Tx | RCC_AHB1Periph_ETH_MAC_Rx, ENABLE);

	/* Reset ETHERNET on AHB Bus */
	ETH_DeInit();

	/* Software reset */
	ETH_SoftwareReset();

	/* Wait for software reset */
	while (ETH_GetSoftwareResetStatus() == SET);


	/* ETHERNET Configuration --------------------------------------------------*/
	/* Call ETH_StructInit if you don't like to configure all ETH_InitStructure parameter */
	ETH_StructInit(&ETH_InitStructure);

	/* Fill ETH_InitStructure parametrs */
	/*------------------------   MAC   -----------------------------------*/
	ETH_InitStructure.ETH_AutoNegotiation           = ETH_AutoNegotiation_Disable;//ETH_AutoNegotiation_Enable; -bt
//	ETH_InitStructure.ETH_AutoNegotiation           = ETH_AutoNegotiation_Disable; 
	ETH_InitStructure.ETH_Speed                     = ETH_Speed_100M;  //-bt
	ETH_InitStructure.ETH_Mode                      = ETH_Mode_FullDuplex;
	ETH_InitStructure.ETH_LoopbackMode              = ETH_LoopbackMode_Disable;
	ETH_InitStructure.ETH_RetryTransmission         = ETH_RetryTransmission_Disable;
	ETH_InitStructure.ETH_AutomaticPadCRCStrip      = ETH_AutomaticPadCRCStrip_Disable;
	ETH_InitStructure.ETH_ReceiveAll                = ETH_ReceiveAll_Disable;
	ETH_InitStructure.ETH_BroadcastFramesReception  = ETH_BroadcastFramesReception_Enable;
	ETH_InitStructure.ETH_PromiscuousMode           = ETH_PromiscuousMode_Disable;
	ETH_InitStructure.ETH_MulticastFramesFilter     = ETH_MulticastFramesFilter_Perfect;
	ETH_InitStructure.ETH_UnicastFramesFilter       = ETH_UnicastFramesFilter_Perfect;
#ifdef CHECKSUM_BY_HARDWARE
	ETH_InitStructure.ETH_ChecksumOffload           = ETH_ChecksumOffload_Enable;
#endif

	/*------------------------   DMA   -----------------------------------------*/
	/* When we use the Checksum offload feature, we need to enable the Store and Forward mode: 
	the store and forward guarantee that a whole frame is stored in the FIFO, so the MAC can insert/verify the checksum, 
	if the checksum is OK the DMA can handle the frame otherwise the frame is dropped */
	ETH_InitStructure.ETH_DropTCPIPChecksumErrorFrame = ETH_DropTCPIPChecksumErrorFrame_Enable; 
	ETH_InitStructure.ETH_ReceiveStoreForward         = ETH_ReceiveStoreForward_Enable;
	ETH_InitStructure.ETH_TransmitStoreForward        = ETH_TransmitStoreForward_Enable;

	ETH_InitStructure.ETH_ForwardErrorFrames          = ETH_ForwardErrorFrames_Disable;
	ETH_InitStructure.ETH_ForwardUndersizedGoodFrames = ETH_ForwardUndersizedGoodFrames_Disable;
	ETH_InitStructure.ETH_SecondFrameOperate          = ETH_SecondFrameOperate_Enable;
	ETH_InitStructure.ETH_AddressAlignedBeats         = ETH_AddressAlignedBeats_Enable;
	ETH_InitStructure.ETH_FixedBurst                  = ETH_FixedBurst_Enable;
	ETH_InitStructure.ETH_RxDMABurstLength            = ETH_RxDMABurstLength_32Beat;
	ETH_InitStructure.ETH_TxDMABurstLength            = ETH_TxDMABurstLength_32Beat;
	ETH_InitStructure.ETH_DMAArbitration              = ETH_DMAArbitration_RoundRobin_RxTx_2_1;

	/* Configure Ethernet */
	EthStatus = ETH_Init(&ETH_InitStructure, DP83848_PHY_ADDRESS);
}

/******************************************************************************************************
                                          reETH_MACDMA_Config

  @Description : Configures the Ethernet Interface
  @Arguments   : none
  @Returns     : none
 ******************************************************************************************************/
static void reETH_MACDMA_Config(void)
{
	/* Enable ETHERNET clock  */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_ETH_MAC | RCC_AHB1Periph_ETH_MAC_Tx | RCC_AHB1Periph_ETH_MAC_Rx, ENABLE);

	/* Reset ETHERNET on AHB Bus */
	ETH_DeInit();

	/* Software reset */
	ETH_SoftwareReset();

	/* Wait for software reset */
	while (ETH_GetSoftwareResetStatus() == SET);


	/* ETHERNET Configuration --------------------------------------------------*/
	/* Call ETH_StructInit if you don't like to configure all ETH_InitStructure parameter */
	ETH_StructInit(&ETH_InitStructure);

	/* Fill ETH_InitStructure parametrs */
	/*------------------------   MAC   -----------------------------------*/
	ETH_InitStructure.ETH_AutoNegotiation           = ETH_AutoNegotiation_Disable;//ETH_AutoNegotiation_Enable; -bt
//	ETH_InitStructure.ETH_AutoNegotiation           = ETH_AutoNegotiation_Disable; 
	ETH_InitStructure.ETH_Speed                     = ETH_Speed_100M;  //-bt
	ETH_InitStructure.ETH_Mode                      = ETH_Mode_FullDuplex;
	ETH_InitStructure.ETH_LoopbackMode              = ETH_LoopbackMode_Disable;
	ETH_InitStructure.ETH_RetryTransmission         = ETH_RetryTransmission_Disable;
	ETH_InitStructure.ETH_AutomaticPadCRCStrip      = ETH_AutomaticPadCRCStrip_Disable;
	ETH_InitStructure.ETH_ReceiveAll                = ETH_ReceiveAll_Disable;
	ETH_InitStructure.ETH_BroadcastFramesReception  = ETH_BroadcastFramesReception_Enable;
	ETH_InitStructure.ETH_PromiscuousMode           = ETH_PromiscuousMode_Disable;
	ETH_InitStructure.ETH_MulticastFramesFilter     = ETH_MulticastFramesFilter_Perfect;
	ETH_InitStructure.ETH_UnicastFramesFilter       = ETH_UnicastFramesFilter_Perfect;
#ifdef CHECKSUM_BY_HARDWARE
	ETH_InitStructure.ETH_ChecksumOffload           = ETH_ChecksumOffload_Enable;
#endif

	/*------------------------   DMA   -----------------------------------------*/
	/* When we use the Checksum offload feature, we need to enable the Store and Forward mode: 
	the store and forward guarantee that a whole frame is stored in the FIFO, so the MAC can insert/verify the checksum, 
	if the checksum is OK the DMA can handle the frame otherwise the frame is dropped */
	ETH_InitStructure.ETH_DropTCPIPChecksumErrorFrame = ETH_DropTCPIPChecksumErrorFrame_Enable; 
	ETH_InitStructure.ETH_ReceiveStoreForward         = ETH_ReceiveStoreForward_Enable;
	ETH_InitStructure.ETH_TransmitStoreForward        = ETH_TransmitStoreForward_Enable;

	ETH_InitStructure.ETH_ForwardErrorFrames          = ETH_ForwardErrorFrames_Disable;
	ETH_InitStructure.ETH_ForwardUndersizedGoodFrames = ETH_ForwardUndersizedGoodFrames_Disable;
	ETH_InitStructure.ETH_SecondFrameOperate          = ETH_SecondFrameOperate_Enable;
	ETH_InitStructure.ETH_AddressAlignedBeats         = ETH_AddressAlignedBeats_Enable;
	ETH_InitStructure.ETH_FixedBurst                  = ETH_FixedBurst_Enable;
	ETH_InitStructure.ETH_RxDMABurstLength            = ETH_RxDMABurstLength_32Beat;
	ETH_InitStructure.ETH_TxDMABurstLength            = ETH_TxDMABurstLength_32Beat;
	ETH_InitStructure.ETH_DMAArbitration              = ETH_DMAArbitration_RoundRobin_RxTx_2_1;

	/* Configure Ethernet */
	EthStatus = reETH_Init(&ETH_InitStructure, DP83848_PHY_ADDRESS);
}

/******************************************************************************************************
                                      ETH_GPIO_Config_RMII/ETH_GPIO_Config

  @Description : Configures the different GPIO ports.
  @Arguments   : none
  @Returns     : none
 ******************************************************************************************************/
static void ETH_GPIO_Config(void)
{
#if PHY_MODE == MODE_RMII
	
	GPIO_InitTypeDef GPIO_InitStructure;

	/* 网卡接口采用RMII
		ETH_RMII_RX_CLK------------------->PA1
		ETH_MDIO-------------------------->PA2
		RMII_CRS_DV----------------------->PA7
		ETH_MDC--------------------------->PC1
		ETH_RMII_RX_D0-------------------->PC4
		ETH_RMII_RX_D1-------------------->PC5
		ETH_RMII_TX_EN------------------->PG11
		FSMC_A24/ETH_RMII_TXD0----------->PG13
		ETH_RMII_TXD1-------------------->PG14
	*/
	/* Enable GPIOs clocks */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOG | RCC_AHB1Periph_GPIOH, ENABLE);

	/* Enable SYSCFG clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

	SYSCFG_ETH_MediaInterfaceConfig(SYSCFG_ETH_MediaInterface_RMII);

	/* Configure PA1, PA2 and PA7 */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_7;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_PinAFConfig(GPIOA, GPIO_PinSource1, GPIO_AF_ETH);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_ETH);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_ETH);

	/* Configure PC1, PC4 and PC5 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_4 | GPIO_Pin_5;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource1, GPIO_AF_ETH);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource4, GPIO_AF_ETH);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource5, GPIO_AF_ETH);

	/* Configure PG11, PG14 and PG13 */
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_11 | GPIO_Pin_13 | GPIO_Pin_14;
	GPIO_Init(GPIOG, &GPIO_InitStructure);
	GPIO_PinAFConfig(GPIOG, GPIO_PinSource11, GPIO_AF_ETH);
	GPIO_PinAFConfig(GPIOG, GPIO_PinSource13, GPIO_AF_ETH);
	GPIO_PinAFConfig(GPIOG, GPIO_PinSource14, GPIO_AF_ETH);
	
#else

	GPIO_InitTypeDef GPIO_InitStructure;

	/* Enable GPIOs clocks */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB |
						 RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOI |
						 RCC_AHB1Periph_GPIOG | RCC_AHB1Periph_GPIOH |
						 RCC_AHB1Periph_GPIOF, ENABLE);

	/* Enable SYSCFG clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

	/* Configure MCO (PA8) */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;  
	GPIO_Init(GPIOA, &GPIO_InitStructure);
  
#ifdef PHY_CLOCK_MCO
	/* Output HSE clock (25MHz) on MCO pin (PA8) to clock the PHY */
	RCC_MCO1Config(RCC_MCO1Source_HSE, RCC_MCO1Div_1);
#endif /* PHY_CLOCK_MCO */
	SYSCFG_ETH_MediaInterfaceConfig(SYSCFG_ETH_MediaInterface_MII);

/* Ethernet pins configuration ************************************************/
	/*
        ETH_MDIO -------------------------> PA2
        ETH_MDC --------------------------> PC1
        ETH_PPS_OUT ----------------------> PB5
        ETH_MII_CRS ----------------------> PH2
        ETH_MII_COL ----------------------> PH3
        ETH_MII_RX_ER --------------------> PI10
        ETH_MII_RXD2 ---------------------> PH6
        ETH_MII_RXD3 ---------------------> PH7
        ETH_MII_TX_CLK -------------------> PC3
        ETH_MII_TXD2 ---------------------> PC2
        ETH_MII_TXD3 ---------------------> PB8
        ETH_MII_RX_CLK/ETH_RMII_REF_CLK---> PA1
        ETH_MII_RX_DV/ETH_RMII_CRS_DV ----> PA7
        ETH_MII_RXD0/ETH_RMII_RXD0 -------> PC4
        ETH_MII_RXD1/ETH_RMII_RXD1 -------> PC5
        ETH_MII_TX_EN/ETH_RMII_TX_EN -----> PG11
        ETH_MII_TXD0/ETH_RMII_TXD0 -------> PG13
        ETH_MII_TXD1/ETH_RMII_TXD1 -------> PG14
                                                  */

	/* Configure PA1, PA2 and PA7 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_7;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource1, GPIO_AF_ETH);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_ETH);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_ETH);

	/* Configure PB5 and PB8 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_8;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource5, GPIO_AF_ETH);	
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource8, GPIO_AF_ETH);

	/* Configure PC1, PC2, PC3, PC4 and PC5 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource1, GPIO_AF_ETH);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource2, GPIO_AF_ETH);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource3, GPIO_AF_ETH);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource4, GPIO_AF_ETH);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource5, GPIO_AF_ETH);
								
	/* Configure PG11, PG14 and PG13 */
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_11 | GPIO_Pin_13 | GPIO_Pin_14;
	GPIO_Init(GPIOG, &GPIO_InitStructure);
	GPIO_PinAFConfig(GPIOG, GPIO_PinSource11, GPIO_AF_ETH);
	GPIO_PinAFConfig(GPIOG, GPIO_PinSource13, GPIO_AF_ETH);
	GPIO_PinAFConfig(GPIOG, GPIO_PinSource14, GPIO_AF_ETH);

	/* Configure PH2, PH3, PH6, PH7 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_Init(GPIOH, &GPIO_InitStructure);
	GPIO_PinAFConfig(GPIOH, GPIO_PinSource2, GPIO_AF_ETH);
	GPIO_PinAFConfig(GPIOH, GPIO_PinSource3, GPIO_AF_ETH);
	GPIO_PinAFConfig(GPIOH, GPIO_PinSource6, GPIO_AF_ETH);
	GPIO_PinAFConfig(GPIOH, GPIO_PinSource7, GPIO_AF_ETH);

	/* Configure PI10 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_Init(GPIOI, &GPIO_InitStructure);
	GPIO_PinAFConfig(GPIOI, GPIO_PinSource10, GPIO_AF_ETH);
	
#endif
}

extern u32 netRuntimer;
/******************************************************************************************************
                                      Eth_Link_ITHandler

  @Description : This function handles Ethernet link status
  @Arguments   : none
  @Returns     : none
 ******************************************************************************************************/
extern void WDTNoReload(void);
extern void LwipProcess_Trigger(void);
void Eth_ITHandler(void)
{
	
	netRuntimer = 0;
	/* Normal interrupt summary */
	if ((ETH->DMASR & ETH_DMA_IT_NIS) != (uint32_t)RESET)
	{
		/* Receive interrupt */ 
		if ((ETH->DMASR & ETH_DMA_IT_R) != (uint32_t)RESET)
		{
			if ((EthStatus & ETH_TASK_OK_FLAG) != (uint32_t)RESET)
			{
				LwipProcess_Trigger();	
			}	
			ETH->DMASR = (uint32_t) ETH_DMA_IT_R;
		}		
		/* Transmit interrupt */
		if ((ETH->DMASR & ETH_DMA_IT_T) != (uint32_t)RESET)
		{
			ETH->DMASR = (uint32_t) ETH_DMA_IT_T;
		}		
		/* Transmit buffer unavailable */
		if ((ETH->DMASR & ETH_DMA_IT_TBU) != (uint32_t)RESET)
		{
			ETH->DMASR = (uint32_t) ETH_DMA_IT_TBU;
		}		
		/* Early receive interrupt */
		if ((ETH->DMASR & ETH_DMA_IT_ER) != (uint32_t)RESET)
		{
			ETH->DMASR = (uint32_t) ETH_DMA_IT_ER;
		}

		ETH->DMASR = (uint32_t) ETH_DMA_IT_NIS;
	}
	
	/* Abnormal interrupt summary */
	if ((ETH->DMASR & ETH_DMA_IT_AIS) != (uint32_t)RESET)
	{
		/* Transmit process stopped interrupt */
		if ((ETH->DMASR & ETH_DMA_IT_TPS) != (uint32_t)RESET)
		{
			ETH->DMASR = (uint32_t) ETH_DMA_IT_TPS;
		}
		/* Transmit jabber timeout interrupt */
		if ((ETH->DMASR & ETH_DMA_IT_TJT) != (uint32_t)RESET)
		{
			ETH->DMASR = (uint32_t) ETH_DMA_IT_TJT;
		}
		/* Overflow interrupt */
		if ((ETH->DMASR & ETH_DMA_IT_RO) != (uint32_t)RESET)
		{
			ETH->DMASR = (uint32_t) ETH_DMA_IT_RO;
		}
		/* Underflow interrupt */
		if ((ETH->DMASR & ETH_DMA_IT_TU) != (uint32_t)RESET)
		{
			ETH->DMASR = (uint32_t) ETH_DMA_IT_TU;
		}
		/* Receive buffer unavailable interrupt */
		if ((ETH->DMASR & ETH_DMA_IT_RBU) != (uint32_t)RESET)
		{
			ETH->DMASR = (uint32_t) ETH_DMA_IT_RBU;
		}
		/* Receive process stopped interrupt */
		if ((ETH->DMASR & ETH_DMA_IT_RPS) != (uint32_t)RESET)
		{
			ETH->DMASR = (uint32_t) ETH_DMA_IT_RPS;
		}
		/* Receive watchdog timeout interrupt */
		if ((ETH->DMASR & ETH_DMA_IT_RWT) != (uint32_t)RESET)
		{
			ETH->DMASR = (uint32_t) ETH_DMA_IT_RWT;
		}
		/* Early transmit interrupt */
		if ((ETH->DMASR & ETH_DMA_IT_ET) != (uint32_t)RESET)
		{
			ETH->DMASR = (uint32_t) ETH_DMA_IT_ET;
		}
		/* Fatal bus error interrupt */
		if ((ETH->DMASR & ETH_DMA_IT_FBE) != (uint32_t)RESET)
		{
			ETH->DMASR = (uint32_t) ETH_DMA_IT_FBE;
		}	
		
		ETH->DMASR = (uint32_t) ETH_DMA_IT_AIS;
	}
}


/******************************************************************************************************
                                      Eth_Link_PHYITConfig

  @Description : Configure the PHY to generate an interrupt on change of link status
  @Arguments   : PHYAddress: external PHY address
  @Returns     : none
 ******************************************************************************************************/
uint32_t Eth_ITConfig(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	
	ETH_DMAITConfig((ETH_DMA_IT_NIS|ETH_DMA_IT_R|ETH_DMA_IT_AIS|ETH_DMA_IT_FBE),ENABLE);
	
	NVIC_InitStructure.NVIC_IRQChannel                   = ETH_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	BSP_IntEn(ETH_IRQn);
	BSP_IntVectSet(ETH_IRQn, Eth_ITHandler);
	
	/* Return SUCCESS */
	return ETH_SUCCESS;   
}

/******************************************************************************************************
                                      Eth_Link_Check

  @Description : Deteck the link status
  @Arguments   : Phyaddress
  @Returns     : none
 ******************************************************************************************************/
extern void ETH_CntInit(void);
void Eth_Link_Stop(void)
{
	ETH_CntInit();
	ETH_Stop();
	EthStatus &= ~ETH_LINK_FLAG; 
	EthStatus = 0;
	netif_set_link_down(&gnetif);
}
extern void relow_level_init(uint8_t init_sign);
uint32_t Eth_Link_Check(uint16_t PHYAddress)
{
	if (!(EthStatus & ETH_LINK_FLAG))
	{
		
		if ((ETH_ReadPHYRegister(PHYAddress, PHY_SR) & 1))
		{				
			reETH_BSP_Config();
			if(EthStatus != 0)
			{
				relow_level_init(0);
				EthStatus_Config(ETH_TASK_OK_FLAG,ENABLE);
	 //			netif_set_link_up(&gnetif);
				if (!(gnetif.flags & NETIF_FLAG_LINK_UP)) 
				{
					gnetif.flags |= NETIF_FLAG_LINK_UP;
					netif_set_up(&gnetif); 
				}
			}
			return 1;
				
		
		}	
							
	}
	else
	{
		if ((ETH_ReadPHYRegister(PHYAddress, PHY_SR) & 1) == 0)
		{
//			set_phy_resetdata(0); //拔掉网线之后重现开始网络复位标记处理。
			set_phy_linkstatus(0);
			
			ETH_Stop();
			EthStatus &= ~ETH_LINK_FLAG;
// 			netif_set_down(&gnetif);			
 			netif_set_link_down(&gnetif);
			return 2;
		}
	}
	return 0;
}


void bttest_eth(void)
{
			reETH_BSP_Config();
				relow_level_init(0);
				EthStatus_Config(ETH_TASK_OK_FLAG,ENABLE);
	 //			netif_set_link_up(&gnetif);
//				if (!(gnetif.flags & NETIF_FLAG_LINK_UP)) 
//				{
					gnetif.flags |= NETIF_FLAG_LINK_UP;
					netif_set_up(&gnetif); 
//				}
}

/******************************************************************************************************
                                      Eth_Link_PHYITConfig

  @Description : Configure the PHY to generate an interrupt on change of link status
  @Arguments   : PHYAddress: external PHY address
  @Returns     : none
 ******************************************************************************************************/
uint32_t Eth_Link_PHYITConfig(uint16_t PHYAddress)
{
	uint16_t tmpreg = 0;

	/* Read MICR register */
	tmpreg = ETH_ReadPHYRegister(PHYAddress, PHY_MICR);

	/* Enable output interrupt events to signal via the INT pin */
	tmpreg |= (uint16_t)(PHY_MICR_INT_EN | PHY_MICR_INT_OE);
	if(!(ETH_WritePHYRegister(PHYAddress, PHY_MICR, tmpreg)))
	{
		/* Return ERROR in case of write timeout */
		return ETH_ERROR;
	}

	/* Read MISR register */
	tmpreg = ETH_ReadPHYRegister(PHYAddress, PHY_MISR);

	/* Enable Interrupt on change of link status */
	tmpreg |= (uint16_t)PHY_MISR_LINK_INT_EN;
	if(!(ETH_WritePHYRegister(PHYAddress, PHY_MISR, tmpreg)))
	{
		/* Return ERROR in case of write timeout */
		return ETH_ERROR;
	}
	/* Return SUCCESS */
	return ETH_SUCCESS;   
}

/******************************************************************************************************
                                      Eth_Link_EXTIConfig

  @Description : EXTI configuration for Ethernet link status.
  @Arguments   : none
  @Returns     : none
 ******************************************************************************************************/
void Eth_Link_EXTIConfig(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	/* Enable the INT (PB14) Clock */
	RCC_AHB1PeriphClockCmd(ETH_LINK_GPIO_CLK, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

// 	/* Configure INT pin as input */
// 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
// 	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
// 	GPIO_InitStructure.GPIO_Pin  = ETH_LINK_PIN;
// 	GPIO_Init(ETH_LINK_GPIO_PORT, &GPIO_InitStructure);

	/* Connect EXTI Line to INT Pin */
	SYSCFG_EXTILineConfig(ETH_LINK_EXTI_PORT_SOURCE, ETH_LINK_EXTI_PIN_SOURCE);

	/* Configure EXTI line */
	EXTI_InitStructure.EXTI_Line    = ETH_LINK_EXTI_LINE;
	EXTI_InitStructure.EXTI_Mode    = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;  
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);

	/* Enable and set the EXTI interrupt to priority 1*/
	NVIC_InitStructure.NVIC_IRQChannel                   = EXTI15_10_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

/******************************************************************************************************
                                      Eth_Link_ITHandler

  @Description : This function handles Ethernet link status
  @Arguments   : none
  @Returns     : none
 ******************************************************************************************************/
void Eth_Link_ITHandler(void)
{
	uint16_t PHYAddress = DP83848_PHY_ADDRESS;
	
	/* Check whether the link interrupt has occurred or not */
	if (((ETH_ReadPHYRegister(PHYAddress, PHY_MISR)) & PHY_LINK_STATUS) != 0)
	{
		if ((ETH_ReadPHYRegister(PHYAddress, PHY_SR) & 1))
		{
			netif_set_link_up(&gnetif);
		}
		else
		{
			netif_set_link_down(&gnetif);
		}
	}
}

/******************************************************************************************************
                                      EXTI15_10_IRQFunction

  @Description : 
  @Arguments   : none
  @Returns     : none
 ******************************************************************************************************/
void EXTI15_10_IRQFunction(void)
{
	if(EXTI_GetITStatus(ETH_LINK_EXTI_LINE) != RESET)
	{
		Eth_Link_ITHandler();
		/* Clear interrupt pending bit */
		EXTI_ClearITPendingBit(ETH_LINK_EXTI_LINE);
	}
}

/******************************************************************************************************
                                      ETH_link_callback

  @Description : Link callback function, this function is called on change of link status
  @Arguments   : none
  @Returns     : none
 ******************************************************************************************************/
void ETH_link_callback(struct netif *netif)
{
	__IO uint32_t timeout = 0;
	uint32_t tmpreg;
	uint16_t RegValue;
// 	struct ip_addr ipaddr;
// 	struct ip_addr netmask;
// 	struct ip_addr gw;
	
#ifndef USE_DHCP
	//uint8_t iptab[4] = {0};
	//uint8_t iptxt[20];
#endif /* USE_DHCP */


	if (netif_is_link_up(netif))
	{
		/* Restart the auto-negotiation */
		if(ETH_InitStructure.ETH_AutoNegotiation != ETH_AutoNegotiation_Disable)
		{
			/* Reset Timeout counter */
			timeout = 0;

			/* Enable auto-negotiation */
			ETH_WritePHYRegister(DP83848_PHY_ADDRESS, PHY_BCR, PHY_AutoNegotiation);

			/* Wait until the auto-negotiation will be completed */
			do
			{
				timeout++;
			} while (!(ETH_ReadPHYRegister(DP83848_PHY_ADDRESS, PHY_BSR) & PHY_AutoNego_Complete) && (timeout < (uint32_t)PHY_READ_TO));

			/* Reset Timeout counter */
			timeout = 0;

			/* Read the result of the auto-negotiation */
			RegValue = ETH_ReadPHYRegister(DP83848_PHY_ADDRESS, PHY_SR);

			/* Configure the MAC with the Duplex Mode fixed by the auto-negotiation process */
			if((RegValue & PHY_DUPLEX_STATUS) != (uint16_t)RESET)
			{
				/* Set Ethernet duplex mode to Full-duplex following the auto-negotiation */
				ETH_InitStructure.ETH_Mode = ETH_Mode_FullDuplex;  
			}
			else
			{
				/* Set Ethernet duplex mode to Half-duplex following the auto-negotiation */
				ETH_InitStructure.ETH_Mode = ETH_Mode_HalfDuplex;
			}
			
			/* Configure the MAC with the speed fixed by the auto-negotiation process */
			if(RegValue & PHY_SPEED_STATUS)
			{
				/* Set Ethernet speed to 10M following the auto-negotiation */
				ETH_InitStructure.ETH_Speed = ETH_Speed_10M; 
			}
			else
			{
				/* Set Ethernet speed to 100M following the auto-negotiation */ 
				ETH_InitStructure.ETH_Speed = ETH_Speed_100M;
			}

			/*------------------------ ETHERNET MACCR Re-Configuration --------------------*/
			/* Get the ETHERNET MACCR value */
			tmpreg = ETH->MACCR;

			/* Set the FES bit according to ETH_Speed value */ 
			/* Set the DM bit according to ETH_Mode value */ 
			tmpreg |= (uint32_t)(ETH_InitStructure.ETH_Speed | ETH_InitStructure.ETH_Mode);

			/* Write to ETHERNET MACCR */
			ETH->MACCR = (uint32_t)tmpreg;

			_eth_delay_(ETH_REG_WRITE_DELAY);
			tmpreg = ETH->MACCR;
			ETH->MACCR = tmpreg;
		}

		/* Restart MAC interface */
		ETH_Start();

#ifdef USE_DHCP
		ipaddr.addr  = 0;
		netmask.addr = 0;
		gw.addr      = 0;
		DHCP_state   = DHCP_START;
#else
// 		IP4_ADDR(&ipaddr, IP_ADDR0, IP_ADDR1, IP_ADDR2, IP_ADDR3);
// 		IP4_ADDR(&netmask, NETMASK_ADDR0, NETMASK_ADDR1 , NETMASK_ADDR2, NETMASK_ADDR3);
// 		IP4_ADDR(&gw, GW_ADDR0, GW_ADDR1, GW_ADDR2, GW_ADDR3);
#endif /* USE_DHCP */

//		netif_set_addr(&gnetif, &ipaddr , &netmask, &gw);

		/* When the netif is fully configured this function must be called.*/
		netif_set_up(&gnetif);    

	}
	else
	{
		ETH_Stop();
#ifdef USE_DHCP
		DHCP_state = DHCP_LINK_DOWN;
		dhcp_stop(netif);
#endif /* USE_DHCP */

		/*  When the netif link is down this function must be called.*/
		netif_set_down(&gnetif);
	}
}
#endif
