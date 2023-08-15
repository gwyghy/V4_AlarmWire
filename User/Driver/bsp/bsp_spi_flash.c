/********************************************************************************
* 文件名称：	flash.c
* 作	者：	马如意   
* 当前版本：   	V1.0
* 完成日期：    2014.02.17
* 功能描述: 	完成SPI FLASH(W25QXX)的驱动，实现初始化、读写、擦除等操作。
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
/********************************************************************************
* .h头文件
*********************************************************************************/
#include "includes.h"
/*当需要向SPI FLASH中写入字库时，在工程文件中定义宏INIT_FLASH1或INIT_FLASH2*/
#ifdef INIT_FLASH1
	#include "ziku.h"
#endif

#ifdef INIT_FLASH2
	#include "ziku.h"
#endif
/********************************************************************************
* #define宏定义
*********************************************************************************/

/********************************************************************************
* 常量定义
*********************************************************************************/
#ifdef INIT_FLASH1
	extern const u8  GB2312_A[20*96][32];
	extern const u8  GB2312_B[20*96][32];
	extern const u8  GB2312_C[20*96][32];
	extern const u8  GB2312_D[20*96][32];
	extern const u8  GB2312_E[7*96][32];
	extern const u8  ASCII[100][16];
#endif
#ifdef INIT_FLASH2
	extern const u8  GB2312_D[20*96][32];	
	extern const u8  GB2312_E[7*96][32];
	extern const u8  ASCII[100][16];
#endif

/********************************************************************************
* 变量定义
*********************************************************************************/

/********************************************************************************
* 函数定义
*********************************************************************************/

/*******************************************************************************************
**函数名称：W25QXX_CfgSPI
**函数作用：配置STM32内部SPI硬件的工作模式、速度等参数，用于访问SPI接口的串行Flash。
**函数参数：无
**函数输出：无
**注意事项：无
*******************************************************************************************/
static void W25QXX_CfgSPI(void)
{
	SPI_InitTypeDef  SPI_InitStructure;

	/* 打开SPI时钟 */
	ENABLE_SPI_RCC();

	/* 配置SPI硬件参数 */
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;	/* 数据方向：2线全双工 */
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		/* STM32的SPI工作模式 ：主机模式 */
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;	/* 数据位长度 ： 8位 */
	/* SPI_CPOL和SPI_CPHA结合使用决定时钟和数据采样点的相位关系、
	   本例配置: 总线空闲是高电平,第2个边沿（上升沿采样数据)
	*/
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;			/* 时钟上升沿采样数据 */
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;		/* 时钟的第2个边沿采样数据 */
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;			/* 片选控制方式：软件控制 */

	/* 设置波特率预分频系数 */
	SPI_InitStructure.SPI_BaudRatePrescaler = W25QXX_SPI_CLK;

	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	/* 数据位传输次序：高位先传 */
	SPI_InitStructure.SPI_CRCPolynomial = 7;			/* CRC多项式寄存器，复位后为7。本例程不用 */
	SPI_Init(W25QXX_SPI, &SPI_InitStructure);

	SPI_Cmd(W25QXX_SPI, DISABLE);			/* 先禁止SPI  */

	SPI_Cmd(W25QXX_SPI, ENABLE);				/* 使能SPI  */
}
/*********************************************************
** 函数名：W25QXX_Init
** 输　入：None  
** 输　出：None
** 功能描述：Initialize the flash.
** 全局变量：None
** 调用模块：None
** 作　者:	李微辰
** 日　期： 2012.12.17
** 修　改：
** 日　期：
** 版本：V1.0A
************************************************************/
int8_t BSP_W25QXXInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
#ifdef INIT_FLASH1
	u32 i = 0x00;
	uint8_t  ttp[16]={0};	
#endif

#ifdef INIT_FLASH2
	u32 i = 0x00;
	uint8_t  ttp[16]={0};	
#endif
	/* 使能GPIO 时钟 */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOD, ENABLE);

	/* 配置 SCK, MISO 、 MOSI 为复用功能 */
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource5, GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource5, GPIO_AF_SPI1);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 ;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 ;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	/* 配置片选口线为推挽输出模式 */
	W25QXX_CS_HIGH();		/* 片选置高，不选中 */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	/* 配置写保护口线为推挽输出模式 */
	W25QXX_WP_HIGH();		/* 片选置高，不选中 */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	W25QXX_WP_HIGH();		/* 片选置高，不选中 */

	/* 配置SPI硬件参数用于访问串行Flash */
	W25QXX_CfgSPI();
	
	if(W25QXX_ReadID())//获取设备号成功
		return 0x01;
#ifdef INIT_FLASH1
    if(W25Q32BV_ID == W25QXX_ReadID())                                                  //Read the ID of flash to avoid the wrong instrument operation.  
    {	
		W25QXX_ReadBuffer(ttp, 0, 16);
		for(i=0;i<0x400000;i+=0x10000)
		{W25QXX_EraseBulk(i);}   
		W25QXX_ReadBuffer(ttp, 0, 16);                                                       //Erasing the sector which is located from 0x100000
		W25QXX_WriteBuffer((u8 *)GB2312_A, 0x000000, 1920*32);						 //Writing to 0x100000
		W25QXX_WriteBuffer((u8 *)GB2312_B, 0x010000, 1920*32);
		W25QXX_WriteBuffer((u8 *)GB2312_C, 0x020000, 1920*32);  
    }
#endif
#ifdef INIT_FLASH2
	if(W25Q32BV_ID == W25QXX_ReadID())                                                  //Read the ID of flash to avoid the wrong instrument operation.  
    {
		W25QXX_ReadBuffer(ttp, 0x030000,16);
		W25QXX_WriteBuffer((u8 *)GB2312_D, 0x030000, 1920*32); 
		W25QXX_WriteBuffer((u8 *)GB2312_E, 0x040000, 672*32); 
		W25QXX_WriteBuffer((u8 *)ASCII, 0x050000, 100*16);
	}
#endif
	return -1;
}

/*********************************************************
** 函数名：W25QXX_EraseSector
** 输　入：SectorAddr ---Easer the adress of sector.  
** 输　出：Result
** 功能描述：Easer a complete Sector to 0xff.
** 全局变量：None
** 调用模块：None
** 作　者:	李微辰
** 日　期： 2012.12.17
** 修　改：
** 日　期：
** 版本：V1.0A
************************************************************/
u8 W25QXX_EraseSector(u32 SectorAddr)
{
  if( SectorAddr >FLASH_ADDRESS_MAX) 
	  return ERROR_UP_OVERFLOW;
  
  /*!< Send write enable instruction */
  W25QXX_WriteEnable();

  /*!< Sector Erase */
  /*!< Select the FLASH: Chip Select low */
  W25QXX_CS_LOW();
  /*!< Send Sector Erase instruction */
  W25QXX_SendByte(W25QXX_CMD_SE);
  /*!< Send SectorAddr high nibble address byte */
  W25QXX_SendByte((SectorAddr & 0xFF0000) >> 16);
  /*!< Send SectorAddr medium nibble address byte */
  W25QXX_SendByte((SectorAddr & 0xFF00) >> 8);
  /*!< Send SectorAddr low nibble address byte */
  W25QXX_SendByte(SectorAddr & 0xFF);
  /*!< Deselect the FLASH: Chip Select high */
  W25QXX_CS_HIGH();

  /*!< Wait the end of Flash writing */
  W25QXX_WaitForWriteEnd();
  return SUCCESS;
}

/*********************************************************
** 函数名：W25QXX_EraseBulk
** 输　入：None  
** 输　出：Result
** 功能描述：Easer a complete Bulk(64KB) to 0xff.
** 全局变量：None
** 调用模块：None
** 作　者:	李微辰
** 日　期： 2012.12.17
** 修　改：
** 日　期：
** 版本：V1.0A
************************************************************/
u8 W25QXX_EraseBulk(u32 BulkAddr)
{
  if( BulkAddr >FLASH_ADDRESS_MAX) 
	  return ERROR_UP_OVERFLOW;
  W25QXX_WriteEnable();
  W25QXX_CS_LOW();
  W25QXX_SendByte(W25QXX_CMD_BE_64K);
  W25QXX_SendByte((BulkAddr & 0xFF0000) >> 16);
  W25QXX_SendByte((BulkAddr & 0xFF00) >> 8);
  W25QXX_SendByte(BulkAddr & 0xFF);
  W25QXX_CS_HIGH();
  W25QXX_WaitForWriteEnd();
  return SUCCESS;
}

/*********************************************************
** 函数名：W25QXX_EraseChip
** 输　入：pBuffer ---The buffer containing the data to be writing to flash.
**         WriteAddr ---Address in flash for paunching the instrution of write.
**         NumByteToWrite ---The mount of bytes to be writed  
** 输　出：None
** 功能描述：擦除整个芯片
** 全局变量：None
** 调用模块：None
** 作　者:	马如意
** 日　期： 2014.02.20
** 修　改：
** 日　期：
** 版本：V1.0A
************************************************************/
u8 W25QXX_EraseChip(void)
{
	W25QXX_WriteEnable();								/* 发送写使能命令 */

	/* 擦除扇区操作 */
	W25QXX_CS_LOW();									/* 使能片选 */
	W25QXX_SendByte(W25QXX_CMD_BE);							/* 发送整片擦除命令 */
	W25QXX_CS_HIGH();									/* 禁能片选 */

	W25QXX_WaitForWriteEnd();							/* 等待串行Flash内部写操作完成 */
	
	return SUCCESS;
}

/*********************************************************
** 函数名：W25QXX_WritePage
** 输　入：pBuffer ---The buffer containing the data to be writing to flash.
**         WriteAddr ---Address in flash for paunching the instrution of write.
**         NumByteToWrite ---The mount of bytes to be writed  
** 输　出：None
** 功能描述：Write an entire page of data to flash.
** 全局变量：None
** 调用模块：None
** 作　者:	李微辰
** 日　期： 2012.12.17
** 修　改：
** 日　期：
** 版本：V1.0A
************************************************************/
u8 W25QXX_WritePage(u8* pBuffer, u32 WriteAddr, u16 NumByteToWrite)
{
  if( WriteAddr >FLASH_ADDRESS_MAX) 
	  return ERROR_UP_OVERFLOW;
  /*!< Enable the write access to the FLASH */
  W25QXX_WriteEnable();

  /*!< Select the FLASH: Chip Select low */
  W25QXX_CS_LOW();
  /*!< Send "Write to Memory " instruction */
  W25QXX_SendByte(W25QXX_CMD_WRITE);
  /*!< Send WriteAddr high nibble address byte to write to */
  W25QXX_SendByte((WriteAddr & 0xFF0000) >> 16);
  /*!< Send WriteAddr medium nibble address byte to write to */
  W25QXX_SendByte((WriteAddr & 0xFF00) >> 8);
  /*!< Send WriteAddr low nibble address byte to write to */
  W25QXX_SendByte(WriteAddr & 0xFF);

  /*!< while there is data to be written on the FLASH */
  while (NumByteToWrite--)
  {
    /*!< Send the current byte */
    W25QXX_SendByte(*pBuffer);
    /*!< Point on the next byte to be written */
    pBuffer++;
  }

  /*!< Deselect the FLASH: Chip Select high */
  W25QXX_CS_HIGH();

  /*!< Wait the end of Flash writing */
  W25QXX_WaitForWriteEnd();
  return SUCCESS;
}

/*********************************************************
** 函数名：W25QXX_WriteBuffer
** 输　入：pBuffer ---The buffer containing the data to be writing to flash.
**         WriteAddr ---Address in flash for paunching the instrution of write.
**         NumByteToWrite ---The mount of bytes to be writed  
** 输　出：None
** 功能描述：Write a certain mount of data to flash.
** 全局变量：None
** 调用模块：None
** 作　者:	李微辰
** 日　期： 2012.12.17
** 修　改：
** 日　期：
** 版本：V1.0A
************************************************************/
u8 W25QXX_WriteBuffer(u8* pBuffer, u32 WriteAddr, u16 NumByteToWrite)
{
  u8 NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0, temp = 0;

  //if((WriteAddr+NumByteToWrite) > FLASH_ADDRESS_MAX) return FALSE;
  if((WriteAddr+NumByteToWrite)+NumByteToWrite > FLASH_ADDRESS_MAX) 
	  return ERROR_UP_OVERFLOW;
  Addr = WriteAddr % W25QXX_SPI_PAGESIZE;
  count = W25QXX_SPI_PAGESIZE - Addr;
  NumOfPage =  NumByteToWrite / W25QXX_SPI_PAGESIZE;
  NumOfSingle = NumByteToWrite % W25QXX_SPI_PAGESIZE;

  if (Addr == 0) /*!< WriteAddr is W25QXX_PAGESIZE aligned  */
  {
    if (NumOfPage == 0) /*!< NumByteToWrite < W25QXX_PAGESIZE */
    {
      W25QXX_WritePage(pBuffer, WriteAddr, NumByteToWrite);
    }
    else /*!< NumByteToWrite > W25QXX_PAGESIZE */
    {
      while (NumOfPage--)
      {
        W25QXX_WritePage(pBuffer, WriteAddr, W25QXX_SPI_PAGESIZE);
        WriteAddr +=  W25QXX_SPI_PAGESIZE;
        pBuffer += W25QXX_SPI_PAGESIZE;
      }

      W25QXX_WritePage(pBuffer, WriteAddr, NumOfSingle);
    }
  }
  else /*!< WriteAddr is not W25QXX_PAGESIZE aligned  */
  {
    if (NumOfPage == 0) /*!< NumByteToWrite < W25QXX_PAGESIZE */
    {
      if (NumOfSingle > count) /*!< (NumByteToWrite + WriteAddr) > W25QXX_PAGESIZE */
      {
        temp = NumOfSingle - count;

        W25QXX_WritePage(pBuffer, WriteAddr, count);
        WriteAddr +=  count;
        pBuffer += count;

        W25QXX_WritePage(pBuffer, WriteAddr, temp);
      }
      else
      {
        W25QXX_WritePage(pBuffer, WriteAddr, NumByteToWrite);
      }
    }
    else /*!< NumByteToWrite > W25QXX_PAGESIZE */
    {
      NumByteToWrite -= count;
      NumOfPage =  NumByteToWrite / W25QXX_SPI_PAGESIZE;
      NumOfSingle = NumByteToWrite % W25QXX_SPI_PAGESIZE;

      W25QXX_WritePage(pBuffer, WriteAddr, count);
      WriteAddr +=  count;
      pBuffer += count;

      while (NumOfPage--)
      {
        W25QXX_WritePage(pBuffer, WriteAddr, W25QXX_SPI_PAGESIZE);
        WriteAddr +=  W25QXX_SPI_PAGESIZE;
        pBuffer += W25QXX_SPI_PAGESIZE;
      }

      if (NumOfSingle != 0)
      {
        W25QXX_WritePage(pBuffer, WriteAddr, NumOfSingle);
      }
    }
  }
  return SUCCESS;
}

/*********************************************************
** 函数名：W25QXX_ReadBuffer
** 输　入：pBuffer ---The buffer containing the data read from flash.
**         ReadAddr ---Address in flash for paunching the instrution of read.
**         NumByteToRead ---The mount of bytes to be read  
** 输　出：None
** 功能描述：Read a certain mount of content in flash and write them to a fixed memory.
** 全局变量：None
** 调用模块：None
** 作　者:	李微辰
** 日　期： 2012.12.17
** 修　改：
** 日　期：
** 版本：V1.0A
************************************************************/
u8 W25QXX_ReadBuffer(u8* pBuffer, u32 ReadAddr, u16 NumByteToRead)
{
  if((ReadAddr+NumByteToRead) + NumByteToRead > FLASH_ADDRESS_MAX) 
	  return ERROR_UP_OVERFLOW;
  /*!< Select the FLASH: Chip Select low */
  W25QXX_CS_LOW();

  /*!< Send "Read from Memory " instruction */
  W25QXX_SendByte(W25QXX_CMD_READ);

  /*!< Send ReadAddr high nibble address byte to read from */
  W25QXX_SendByte((ReadAddr & 0xFF0000) >> 16);
  /*!< Send ReadAddr medium nibble address byte to read from */
  W25QXX_SendByte((ReadAddr& 0xFF00) >> 8);
  /*!< Send ReadAddr low nibble address byte to read from */
  W25QXX_SendByte(ReadAddr & 0xFF);

  while (NumByteToRead--) /*!< while there is data to be read */
  {
    /*!< Read a byte from the FLASH */
    *pBuffer = W25QXX_SendByte(W25QXX_DUMMY_BYTE);
    /*!< Point to the next location where the byte read will be saved */
    pBuffer++;
  }

  /*!< Deselect the FLASH: Chip Select high */
  W25QXX_CS_HIGH();
  return SUCCESS;
}

/*********************************************************
** 函数名：W25QXX_ReadID
** 输　入：pBuffer1 ---None  
** 输　出：JEDEC ID
** 功能描述：Read the JEDEC ID(manufacturer id and device id)
** 全局变量：None
** 调用模块：None
** 作　者:	李微辰
** 日　期： 2012.12.17
** 修　改：
** 日　期：
** 版本：V1.0A
************************************************************/
u32 W25QXX_ReadID(void)
{
  u32 Temp = 0, Temp0 = 0, Temp1 = 0, Temp2 = 0;

  /*!< Select the FLASH: Chip Select low */
  W25QXX_CS_LOW();

  /*!< Send "RDID " instruction */
  W25QXX_SendByte(0x9F);

  /*!< Read a byte from the FLASH */
  Temp0 = W25QXX_SendByte(W25QXX_DUMMY_BYTE);

  /*!< Read a byte from the FLASH */
  Temp1 = W25QXX_SendByte(W25QXX_DUMMY_BYTE);

  /*!< Read a byte from the FLASH */
  Temp2 = W25QXX_SendByte(W25QXX_DUMMY_BYTE);

  /*!< Deselect the FLASH: Chip Select high */
  W25QXX_CS_HIGH();

  Temp = (Temp0 << 16) | (Temp1 << 8) | Temp2;

  return Temp;
}
/*********************************************************
** 函数名：W25QXX_StartReadSequence
** 输　入：ReadAddr ---The start adress to be reading  
** 输　出：1
** 功能描述：Read a seri of data from a start adress continuously.
** 全局变量：None
** 调用模块：None
** 作　者:	李微辰
** 日　期： 2012.12.17
** 修　改：
** 日　期：
** 版本：V1.0A
************************************************************/
void W25QXX_StartReadSequence(u32 ReadAddr)
{
  /*!< Select the FLASH: Chip Select low */
  W25QXX_CS_LOW();

  /*!< Send "Read from Memory " instruction */
  W25QXX_SendByte(W25QXX_CMD_READ);

  /*!< Send the 24-bit address of the address to read from -------------------*/
  /*!< Send ReadAddr high nibble address byte */
  W25QXX_SendByte((ReadAddr & 0xFF0000) >> 16);
  /*!< Send ReadAddr medium nibble address byte */
  W25QXX_SendByte((ReadAddr& 0xFF00) >> 8);
  /*!< Send ReadAddr low nibble address byte */
  W25QXX_SendByte(ReadAddr & 0xFF);
}

/*********************************************************
** 函数名：W25QXX_ReadByte
** 输　入：None  
** 输　出：Byte which is read.
** 功能描述：Read a byte data from the spi bus.
** 全局变量：None
** 调用模块：None
** 作　者:	李微辰
** 日　期： 2012.12.17
** 修　改：
** 日　期：
** 版本：V1.0A
************************************************************/

#ifndef  SPI_FLASH_FOR_BOOTLOADER  //在bootloader程序中，为节省FLASH空间，使用的预编译指令
u8 W25QXX_ReadByte(void)
{
  return (W25QXX_SendByte(W25QXX_DUMMY_BYTE));
}
#endif

/*********************************************************
** 函数名：W25QXX_SendByte
** 输　入：byte --- 8-bit data being sent. 
** 输　出：8-bit data being received.
** 功能描述：Sending the 8-bit data.
** 全局变量：None
** 调用模块：None
** 作　者:	李微辰
** 日　期： 2012.12.17
** 修　改：
** 日　期：
** 版本：V1.0A
************************************************************/
u8 W25QXX_SendByte(u8 byte)
{
  /*!< Loop while DR register in not emplty */
	u32 spitimeover = 0;
  while (SPI_I2S_GetFlagStatus(W25QXX_SPI, SPI_I2S_FLAG_TXE) == RESET && spitimeover <= SPI_WAIT_TIMEDLY)
	{
		spitimeover++;
	}
	if(spitimeover > SPI_WAIT_TIMEDLY)
		return 0;
  /*!< Send byte through the SPI1 peripheral */
  SPI_I2S_SendData(W25QXX_SPI, byte);

  /*!< Wait to receive a byte */
	spitimeover = 0;
  while (SPI_I2S_GetFlagStatus(W25QXX_SPI, SPI_I2S_FLAG_RXNE) == RESET && spitimeover <= SPI_WAIT_TIMEDLY)
	{
		spitimeover++;
	}

	if(spitimeover > SPI_WAIT_TIMEDLY)
		return 0;
  /*!< Return the byte read from the SPI bus */
  return SPI_I2S_ReceiveData(W25QXX_SPI);
}

/*********************************************************
** 函数名：W25QXX_SendHalfWord
** 输　入：HalfWord --- 16-bit data being sent.  
** 输　出：16-bit data being received.
** 功能描述：Sending the 16-bit data.
** 全局变量：None
** 调用模块：None
** 作　者:	李微辰
** 日　期： 2012.12.17
** 修　改：
** 日　期：
** 版本：V1.0A
************************************************************/

#ifndef  SPI_FLASH_FOR_BOOTLOADER  //在bootloader程序中，为节省FLASH空间，使用的预编译指令
u16 W25QXX_SendHalfWord(u16 HalfWord)
{
  /*!< Loop while DR register in not emplty */
	u32 spitimeover = 0;
  while (SPI_I2S_GetFlagStatus(W25QXX_SPI, SPI_I2S_FLAG_TXE) == RESET && spitimeover <= SPI_WAIT_TIMEDLY)
	{
		spitimeover++;
	}
	if(spitimeover > SPI_WAIT_TIMEDLY)
		return 0;
  /*!< Send Half Word through the sFLASH peripheral */
  SPI_I2S_SendData(W25QXX_SPI, HalfWord);

  /*!< Wait to receive a Half Word */
	spitimeover = 0;
  while (SPI_I2S_GetFlagStatus(W25QXX_SPI, SPI_I2S_FLAG_RXNE) == RESET && spitimeover <= SPI_WAIT_TIMEDLY)
	{
		spitimeover++;
	}
	if(spitimeover > SPI_WAIT_TIMEDLY)
		return 0;
  /*!< Return the Half Word read from the SPI bus */
  return SPI_I2S_ReceiveData(W25QXX_SPI);
}
#endif

/*********************************************************
** 函数名：W25QXX_WriteEnable
** 输　入：None  
** 输　出：None
** 功能描述：Send the writing command byte to prepare for writing.(Demonstrated in the protocol of SPI standard in Manual of W25QXX)
** 全局变量：None
** 调用模块：None
** 作　者:	李微辰
** 日　期： 2012.12.17
** 修　改：
** 日　期：
** 版本：V1.0A
************************************************************/
void W25QXX_WriteEnable(void)
{
  /*!< Select the FLASH: Chip Select low */
  W25QXX_CS_LOW();

  /*!< Send "Write Enable" instruction */
  W25QXX_SendByte(W25QXX_CMD_WREN);

  /*!< Deselect the FLASH: Chip Select high */
  W25QXX_CS_HIGH();
}

/*********************************************************
** 函数名：W25QXX_WaitForWriteEnd
** 输　入：none  
** 输　出：none
** 功能描述：Waiting for the last writing operation finished.
** 全局变量：None
** 调用模块：None
** 作　者:	李微辰
** 日　期： 2012.12.17
** 修　改：
** 日　期：
** 版本：V1.0A
************************************************************/
void W25QXX_WaitForWriteEnd(void)
{
  u8 flashstatus = 0;
	u32 spitimeover = 0;
  /*!< Select the FLASH: Chip Select low */
  W25QXX_CS_LOW();

  /*!< Send "Read Status Register" instruction */
  W25QXX_SendByte(W25QXX_CMD_RDSR1);

  /*!< Loop as long as the memory is busy with a write cycle */
  do
  {
    /*!< Send a dummy byte to generate the clock needed by the FLASH
    and put the value of the status register in FLASH_Status variable */
    flashstatus = W25QXX_SendByte(W25QXX_DUMMY_BYTE);
		spitimeover++;

  }
  while ((flashstatus & W25QXX_WIP_FLAG) == SET && spitimeover <= SPI_WAIT_TIMEDLY); /* Write in progress */

  /*!< Deselect the FLASH: Chip Select high */
  W25QXX_CS_HIGH();
}
