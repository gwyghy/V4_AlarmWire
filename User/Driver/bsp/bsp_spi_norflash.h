/**************************************************************************
**日  期：2012.03.01                                                               
**创建人：李微辰
**修改人：
**使  用：
**************************************************************************/ 
#ifndef __BSP_SPI_NORFLASH_H__
#define __BSP_SPI_NORFLASH_H__

/* Includes ------------------------------------------------------------------*/
//#include <stdio.h>
#include "includes.h"

#define SUCCESS 0
#define ERROR_1 1
#define ERROR_2 2
#define ERROR_3 3
#define ERROR_4 4
#define ERROR_5 5
#define ERROR_6 6
#define ERROR_7 7
#define ERROR_8 8
#define ERROR_9 9
#define ERROR_10 10

#define ERROR_UP_OVERFLOW     ERROR_1
#define ERROR_DOWN_OVERFLOW   ERROR_2
#define ERROR_OUTOFFRAME      ERROR_3
#define ERROR_OVERTIME		  ERROR_4
#define ERROR_DATA_NUM		  ERROR_5


// SPI3
#define	SPI3_PORT			GPIOC
#define SPI3_CLK_PIN		GPIO_Pin_10
#define SPI3_MISO_PIN		GPIO_Pin_11
#define SPI3_MOSI_PIN		GPIO_Pin_12

#define SPI3_GPIO_RCC		RCC_AHB1Periph_GPIOC
#define SPI3_APB			APB1
#define SPI3_RCC			RCC_APB1Periph_SPI3

#define SPI3_CLK_SOURCE		GPIO_PinSource10
#define SPI3_MISO_SOURCE	GPIO_PinSource11
#define SPI3_MOSI_SOURCE	GPIO_PinSource12

#define	SPI3_GPIO_AF		GPIO_AF_SPI3

//------------按照应用功能定义————————


#define FLASH_AD_SPI_RCC	SPI3_RCC

// FLASH接口
#define FLASH_AD_SPI		SPI3
#define FLASH_AD_SPI_APB	SPI3_APB
#define FLASH_SPI_PORT		SPI3_PORT
#define FLASH_SPI_SCK_PIN	SPI3_CLK_PIN
#define FLASH_SPI_MISO_PIN	SPI3_MISO_PIN
#define FLASH_SPI_MOSI_PIN	SPI3_MOSI_PIN

#define FLASH_AD_GPIO_RCC	SPI3_GPIO_RCC
//#define FLASH_AD_SPI_RCC	SPI3_RCC

#define FLASH_SPI_SCK_SOURCE	SPI3_CLK_SOURCE
#define FLASH_SPI_MISO_SOURCE	SPI3_MISO_SOURCE
#define FLASH_SPI_MOSI_SOURCE	SPI3_MOSI_SOURCE

#define FLASH_SPI_GPIO_AF	SPI3_GPIO_AF

#define FLASH_SPI_CS_PORT	GPIOA
#define FLASH_SPI_CS_PIN	GPIO_Pin_15

#define FLASH_CS_GPIO_RCC	RCC_AHB1Periph_GPIOA

#define FLASH_SPI_WP_PORT	GPIOD
#define FLASH_SPI_WP_PIN	GPIO_Pin_2

#define FLASH_WP_GPIO_RCC	RCC_AHB1Periph_GPIOD

/*-------------------INSTRUMENT CONFIGURATION---------------------------------*/
#define FLASH_SIZE_IN_BYTES    32*1024*1024	                        //flash size,8M字节
#define FLASH_MODEL            W25QXX 								//flash model

#if	FLASH_MODEL == W25QXX

#define W25QXX_CMD_JEDECID        0x9F

#define W25QXX_CMD_WREN           0x06  /*!< Write enable instruction */
#define W25QXX_CMD_WRDIS          0x04  /*!< Write disable instruction */
#define W25QXX_CMD_WRITE          0x12  /*!< Write to Memory instruction */

#define W25QXX_CMD_READ           0x13  /*!< Read from Memory instruction */

#define W25QXX_CMD_RDSR1          0x05  /*!< Read Status Register instruction  */
#define W25QXX_CMD_RDSR2          0x35  /*!< Read Status Register instruction  */
#define W25QXX_CMD_RDSR3          0x15  /*!< Read Status Register instruction  */

#define W25QXX_CMD_SE             0x20  /*!< Sector Erase instruction */
#define W25QXX_CMD_BE_32K         0x52  /*!< Bulk Erase instruction */
#define W25QXX_CMD_BE_64K         0xD8  /*!< Bulk Erase instruction */

#define W25QXX_CMD_4BYTE          0xB7
#endif /*end of FLASH_MODEL == W25QXX*/

/* Private define ------------------------------------------------------------*/
#if FLASH_SIZE_IN_BYTES == 64*16*4*1024
#define FLASH_ADDRESS_MAX   0x003fffff
#else
	#define FLASH_ADDRESS_MAX   0x01ffffff
#endif

#define W25QXX_WIP_FLAG           0x01  /*!< Write In Progress (WIP) flag */

#define W25QXX_DUMMY_BYTE         0xFF
#define W25QXX_SPI_PAGESIZE       0x100

#define W25QXX_M25P128_ID         0x202018
#define W25QXX_M25P64_ID          0x202017
//-------------------------------Chip Select Enable/Disable----------------------------
#define W25QXX_CS_LOW()       GPIO_ResetBits(FLASH_SPI_CS_PORT, FLASH_SPI_CS_PIN)
#define W25QXX_CS_HIGH()      GPIO_SetBits(FLASH_SPI_CS_PORT, FLASH_SPI_CS_PIN) 

#define W25QXX_WP_LOW()       GPIO_ResetBits(FLASH_SPI_WP_PORT, FLASH_SPI_WP_PIN)
#define W25QXX_WP_HIGH()      GPIO_SetBits(FLASH_SPI_WP_PORT, FLASH_SPI_WP_PIN)
//-------------------------------------------------------------------------------------
  
#define W25X_ReadStatusReg1		0x05 
#define W25X_ReadStatusReg2		0x35 
#define W25X_ReadStatusReg3		0x15 
#define W25X_WriteStatusReg1    0x01 


u8 W25QXX_SendByte(u8 byte);
void W25QXX_Write_Enable(void);
void W25QXX_DeInit(void);
void W25QXX_Init(void);
void W25QXX_Erase_Sector(u32 Dst_Addr);
void W25QXX_Write_NoCheck(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite);
void W25QXX_Write(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite);
u8 W25QXX_Read(u8* pBuffer, u32 ReadAddr, u16 NumByteToRead);
u32 W25QXX_ReadID(void);


#endif
