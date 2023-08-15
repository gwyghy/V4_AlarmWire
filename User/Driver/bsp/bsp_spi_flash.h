/********************************************************************************
* 文件名称：	flash.h
* 作	者：	马如意   
* 当前版本：   	V1.0
* 完成日期：    2014.02.17
* 功能描述: 	定义flash.h头文件
* 历史信息：   
*           	版本信息     完成时间      原作者        注释
*
*       >>>>  在工程中的位置  <<<<
*          	  3-应用层
*             2-协议层
*          √  1-硬件驱动层
*********************************************************************************
* Copyright (c) 2014,天津华宁电子有限公司 All rights reserved.
*********************************************************************************/
#ifndef __BSP_SPI_FLASH_H
#define __BSP_SPI_FLASH_H
/********************************************************************************
* .h头文件
*********************************************************************************/
#include "includes.h"
/********************************************************************************
* #define宏定义
*********************************************************************************/
#define FLASH_MODEL            W25QXX 								//flash model
#define SPI_FALSH_FOR_BOOTLOADER  //在bootloader程序中，为节省FLASH空间，使用的预编译指令,马如意
#define SPI_WAIT_TIMEDLY        800000
/*-------------------INSTRUMENT CONFIGURATION---------------------------------*/
#if	FLASH_MODEL == W25QXX

	#define W25QXX_SPI                       SPI1
	#define W25QXX_SPI_CLK                   SPI_BaudRatePrescaler_2
	#define ENABLE_SPI_RCC() 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE)
		
	#define W25QXX_SPI_SCK_PIN               GPIO_Pin_5                  
	#define W25QXX_SPI_SCK_GPIO_PORT         GPIOA                       
	#define W25QXX_SPI_SCK_GPIO_CLK          RCC_APB1Periph_GPIOA

	#define W25QXX_SPI_MISO_PIN              GPIO_Pin_6                  
	#define W25QXX_SPI_MISO_GPIO_PORT        GPIOA                       
	#define W25QXX_SPI_MISO_GPIO_CLK         RCC_APB1Periph_GPIOA

	#define W25QXX_SPI_MOSI_PIN              GPIO_Pin_5                 
	#define W25QXX_SPI_MOSI_GPIO_PORT        GPIOB                       
	#define W25QXX_SPI_MOSI_GPIO_CLK         RCC_APB1Periph_GPIO5

	#define W25QXX_CS_PIN                    GPIO_Pin_11                  
	#define W25QXX_CS_GPIO_PORT              GPIOB
	#define W25QXX_CS_GPIO_CLK               RCC_APB1Periph_GPIOB

	#define W25QXX_WP_PIN                    GPIO_Pin_3
	#define W25QXX_WP_GPIO_PORT              GPIOD
	#define W25QXX_WP_GPIO_CLK               RCC_APB1Periph_GPIOD

#endif /*end of FLASH_MODEL == W25QXX*/



#define W25QXX_CS_LOW()       GPIO_ResetBits(W25QXX_CS_GPIO_PORT, W25QXX_CS_PIN)
#define W25QXX_CS_HIGH()      GPIO_SetBits(W25QXX_CS_GPIO_PORT, W25QXX_CS_PIN) 

#define W25QXX_WP_LOW()       GPIO_ResetBits(W25QXX_WP_GPIO_PORT, W25QXX_WP_PIN)
#define W25QXX_WP_HIGH()      GPIO_SetBits(W25QXX_WP_GPIO_PORT, W25QXX_WP_PIN)  
/********************************************************************************
* 常量定义
*********************************************************************************/
#if FLASH_MODEL == W25QXX 
	#define W25QXX_CMD_WRITE          0x02  /*!< Write to Memory instruction */
	#define W25QXX_CMD_WRSR           0x01  /*!< Write Status Register instruction */
	#define W25QXX_CMD_WREN           0x06  /*!< Write enable instruction */
	#define W25QXX_CMD_READ           0x03  /*!< Read from Memory instruction */
	#define W25QXX_CMD_RDSR1          0x05  /*!< Read Status Register instruction  */
	#define W25QXX_CMD_RDSR2          0x35  /*!< Read Status Register instruction  */
	#define W25QXX_CMD_RDID           0x9F  /*!< Read identification */
	#define W25QXX_CMD_SE             0x20  /*!< Sector Erase instruction */
	#define W25QXX_CMD_BE_32K         0x52  /*!< Bulk Erase instruction */
	#define W25QXX_CMD_BE_64K         0xD8  /*!< Bulk Erase instruction */
	#define W25QXX_CMD_BE       	  0xC7	/*!< Chip Erase instruction */	
	#define W25QXX_WIP_FLAG           0x01  /*!< Write In Progress (WIP) flag */
	#define W25QXX_DUMMY_BYTE         0xA5
	
	#define	W25Q32BV_ID               0x00EF4016//W25Q32BV的ID号
	#define	W25Q64BV_ID				  0x00EF4017//W25Q64BV的ID号
#endif
/* Private define -*/
#define FLASH_SIZE_IN_BYTES    64*16*4*1024	                        //flash size
#define BLOCK_SIZE_IN_BYTES    16*4*1024							//flash block size 
#define SECTOR_SIZE_IN_BYTES   4*1024								//flash sector size

#if FLASH_SIZE_IN_BYTES == 64*16*4*1024
	#define FLASH_ADDRESS_MAX   0x003fffff
#else
	#define FLASH_ADDRESS_MAX   0xffffffff
#endif


#define W25QXX_SPI_PAGESIZE       0x100

#define W25QXX_M25P128_ID         0x202018
#define W25QXX_M25P64_ID          0x202017

/*ERROR指示*/
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
#define ERROR_DATA_NUM		  ERROR_4
/********************************************************************************
* 全局变量声明
*********************************************************************************/
/********************************************************************************
* 函数声明
*********************************************************************************/
int8_t BSP_W25QXXInit(void);
u8 W25QXX_EraseSector(u32 SectorAddr);
u8 W25QXX_EraseBulk(u32 BulkAddr);
u8 W25QXX_EraseChip(void);

u8 W25QXX_WritePage(u8* pBuffer, u32 WriteAddr, u16 NumByteToWrite);
u8 W25QXX_WriteBuffer(u8* pBuffer, u32 WriteAddr, u16 NumByteToWrite);
u8 W25QXX_ReadBuffer(u8* pBuffer, u32 ReadAddr, u16 NumByteToRead);
u32 W25QXX_ReadID(void);
void W25QXX_StartReadSequence(u32 ReadAddr);

u8 W25QXX_ReadByte(void);
u8 W25QXX_SendByte(u8 byte);
u16 W25QXX_SendHalfWord(u16 HalfWord);
void W25QXX_WriteEnable(void);
void W25QXX_WaitForWriteEnd(void);
void W25QXX_LowLevel_DeInit(void);
void W25QXX_LowLevel_Init(void);


#endif /* __STM32_SPI_FLASH_H */
