/********************************************************************************
* �ļ����ƣ�	flash.c
* ��	�ߣ�	������   
* ��ǰ�汾��   	V1.0
* ������ڣ�    2014.02.17
* ��������: 	���SPI FLASH(W25QXX)��������ʵ�ֳ�ʼ������д�������Ȳ�����
* ��ʷ��Ϣ��   
*           	�汾��Ϣ     ���ʱ��      ԭ����        ע��
*
*       >>>>  �ڹ����е�λ��  <<<<
*          	  3-Ӧ�ò�
*             2-Э���
*          ��  1-Ӳ��������
*********************************************************************************
* Copyright (c) 2014,������������޹�˾ All rights reserved.
*********************************************************************************/
/********************************************************************************
* .hͷ�ļ�
*********************************************************************************/
#include "includes.h"
/*����Ҫ��SPI FLASH��д���ֿ�ʱ���ڹ����ļ��ж����INIT_FLASH1��INIT_FLASH2*/
#ifdef INIT_FLASH1
	#include "ziku.h"
#endif

#ifdef INIT_FLASH2
	#include "ziku.h"
#endif
/********************************************************************************
* #define�궨��
*********************************************************************************/

/********************************************************************************
* ��������
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
* ��������
*********************************************************************************/

/********************************************************************************
* ��������
*********************************************************************************/

/*******************************************************************************************
**�������ƣ�W25QXX_CfgSPI
**�������ã�����STM32�ڲ�SPIӲ���Ĺ���ģʽ���ٶȵȲ��������ڷ���SPI�ӿڵĴ���Flash��
**������������
**�����������
**ע�������
*******************************************************************************************/
static void W25QXX_CfgSPI(void)
{
	SPI_InitTypeDef  SPI_InitStructure;

	/* ��SPIʱ�� */
	ENABLE_SPI_RCC();

	/* ����SPIӲ������ */
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;	/* ���ݷ���2��ȫ˫�� */
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		/* STM32��SPI����ģʽ ������ģʽ */
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;	/* ����λ���� �� 8λ */
	/* SPI_CPOL��SPI_CPHA���ʹ�þ���ʱ�Ӻ����ݲ��������λ��ϵ��
	   ��������: ���߿����Ǹߵ�ƽ,��2�����أ������ز�������)
	*/
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;			/* ʱ�������ز������� */
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;		/* ʱ�ӵĵ�2�����ز������� */
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;			/* Ƭѡ���Ʒ�ʽ��������� */

	/* ���ò�����Ԥ��Ƶϵ�� */
	SPI_InitStructure.SPI_BaudRatePrescaler = W25QXX_SPI_CLK;

	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	/* ����λ������򣺸�λ�ȴ� */
	SPI_InitStructure.SPI_CRCPolynomial = 7;			/* CRC����ʽ�Ĵ�������λ��Ϊ7�������̲��� */
	SPI_Init(W25QXX_SPI, &SPI_InitStructure);

	SPI_Cmd(W25QXX_SPI, DISABLE);			/* �Ƚ�ֹSPI  */

	SPI_Cmd(W25QXX_SPI, ENABLE);				/* ʹ��SPI  */
}
/*********************************************************
** ��������W25QXX_Init
** �䡡�룺None  
** �䡡����None
** ����������Initialize the flash.
** ȫ�ֱ�����None
** ����ģ�飺None
** ������:	��΢��
** �ա��ڣ� 2012.12.17
** �ޡ��ģ�
** �ա��ڣ�
** �汾��V1.0A
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
	/* ʹ��GPIO ʱ�� */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOD, ENABLE);

	/* ���� SCK, MISO �� MOSI Ϊ���ù��� */
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
	
	/* ����Ƭѡ����Ϊ�������ģʽ */
	W25QXX_CS_HIGH();		/* Ƭѡ�øߣ���ѡ�� */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	/* ����д��������Ϊ�������ģʽ */
	W25QXX_WP_HIGH();		/* Ƭѡ�øߣ���ѡ�� */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	W25QXX_WP_HIGH();		/* Ƭѡ�øߣ���ѡ�� */

	/* ����SPIӲ���������ڷ��ʴ���Flash */
	W25QXX_CfgSPI();
	
	if(W25QXX_ReadID())//��ȡ�豸�ųɹ�
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
** ��������W25QXX_EraseSector
** �䡡�룺SectorAddr ---Easer the adress of sector.  
** �䡡����Result
** ����������Easer a complete Sector to 0xff.
** ȫ�ֱ�����None
** ����ģ�飺None
** ������:	��΢��
** �ա��ڣ� 2012.12.17
** �ޡ��ģ�
** �ա��ڣ�
** �汾��V1.0A
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
** ��������W25QXX_EraseBulk
** �䡡�룺None  
** �䡡����Result
** ����������Easer a complete Bulk(64KB) to 0xff.
** ȫ�ֱ�����None
** ����ģ�飺None
** ������:	��΢��
** �ա��ڣ� 2012.12.17
** �ޡ��ģ�
** �ա��ڣ�
** �汾��V1.0A
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
** ��������W25QXX_EraseChip
** �䡡�룺pBuffer ---The buffer containing the data to be writing to flash.
**         WriteAddr ---Address in flash for paunching the instrution of write.
**         NumByteToWrite ---The mount of bytes to be writed  
** �䡡����None
** ������������������оƬ
** ȫ�ֱ�����None
** ����ģ�飺None
** ������:	������
** �ա��ڣ� 2014.02.20
** �ޡ��ģ�
** �ա��ڣ�
** �汾��V1.0A
************************************************************/
u8 W25QXX_EraseChip(void)
{
	W25QXX_WriteEnable();								/* ����дʹ������ */

	/* ������������ */
	W25QXX_CS_LOW();									/* ʹ��Ƭѡ */
	W25QXX_SendByte(W25QXX_CMD_BE);							/* ������Ƭ�������� */
	W25QXX_CS_HIGH();									/* ����Ƭѡ */

	W25QXX_WaitForWriteEnd();							/* �ȴ�����Flash�ڲ�д������� */
	
	return SUCCESS;
}

/*********************************************************
** ��������W25QXX_WritePage
** �䡡�룺pBuffer ---The buffer containing the data to be writing to flash.
**         WriteAddr ---Address in flash for paunching the instrution of write.
**         NumByteToWrite ---The mount of bytes to be writed  
** �䡡����None
** ����������Write an entire page of data to flash.
** ȫ�ֱ�����None
** ����ģ�飺None
** ������:	��΢��
** �ա��ڣ� 2012.12.17
** �ޡ��ģ�
** �ա��ڣ�
** �汾��V1.0A
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
** ��������W25QXX_WriteBuffer
** �䡡�룺pBuffer ---The buffer containing the data to be writing to flash.
**         WriteAddr ---Address in flash for paunching the instrution of write.
**         NumByteToWrite ---The mount of bytes to be writed  
** �䡡����None
** ����������Write a certain mount of data to flash.
** ȫ�ֱ�����None
** ����ģ�飺None
** ������:	��΢��
** �ա��ڣ� 2012.12.17
** �ޡ��ģ�
** �ա��ڣ�
** �汾��V1.0A
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
** ��������W25QXX_ReadBuffer
** �䡡�룺pBuffer ---The buffer containing the data read from flash.
**         ReadAddr ---Address in flash for paunching the instrution of read.
**         NumByteToRead ---The mount of bytes to be read  
** �䡡����None
** ����������Read a certain mount of content in flash and write them to a fixed memory.
** ȫ�ֱ�����None
** ����ģ�飺None
** ������:	��΢��
** �ա��ڣ� 2012.12.17
** �ޡ��ģ�
** �ա��ڣ�
** �汾��V1.0A
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
** ��������W25QXX_ReadID
** �䡡�룺pBuffer1 ---None  
** �䡡����JEDEC ID
** ����������Read the JEDEC ID(manufacturer id and device id)
** ȫ�ֱ�����None
** ����ģ�飺None
** ������:	��΢��
** �ա��ڣ� 2012.12.17
** �ޡ��ģ�
** �ա��ڣ�
** �汾��V1.0A
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
** ��������W25QXX_StartReadSequence
** �䡡�룺ReadAddr ---The start adress to be reading  
** �䡡����1
** ����������Read a seri of data from a start adress continuously.
** ȫ�ֱ�����None
** ����ģ�飺None
** ������:	��΢��
** �ա��ڣ� 2012.12.17
** �ޡ��ģ�
** �ա��ڣ�
** �汾��V1.0A
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
** ��������W25QXX_ReadByte
** �䡡�룺None  
** �䡡����Byte which is read.
** ����������Read a byte data from the spi bus.
** ȫ�ֱ�����None
** ����ģ�飺None
** ������:	��΢��
** �ա��ڣ� 2012.12.17
** �ޡ��ģ�
** �ա��ڣ�
** �汾��V1.0A
************************************************************/

#ifndef  SPI_FLASH_FOR_BOOTLOADER  //��bootloader�����У�Ϊ��ʡFLASH�ռ䣬ʹ�õ�Ԥ����ָ��
u8 W25QXX_ReadByte(void)
{
  return (W25QXX_SendByte(W25QXX_DUMMY_BYTE));
}
#endif

/*********************************************************
** ��������W25QXX_SendByte
** �䡡�룺byte --- 8-bit data being sent. 
** �䡡����8-bit data being received.
** ����������Sending the 8-bit data.
** ȫ�ֱ�����None
** ����ģ�飺None
** ������:	��΢��
** �ա��ڣ� 2012.12.17
** �ޡ��ģ�
** �ա��ڣ�
** �汾��V1.0A
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
** ��������W25QXX_SendHalfWord
** �䡡�룺HalfWord --- 16-bit data being sent.  
** �䡡����16-bit data being received.
** ����������Sending the 16-bit data.
** ȫ�ֱ�����None
** ����ģ�飺None
** ������:	��΢��
** �ա��ڣ� 2012.12.17
** �ޡ��ģ�
** �ա��ڣ�
** �汾��V1.0A
************************************************************/

#ifndef  SPI_FLASH_FOR_BOOTLOADER  //��bootloader�����У�Ϊ��ʡFLASH�ռ䣬ʹ�õ�Ԥ����ָ��
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
** ��������W25QXX_WriteEnable
** �䡡�룺None  
** �䡡����None
** ����������Send the writing command byte to prepare for writing.(Demonstrated in the protocol of SPI standard in Manual of W25QXX)
** ȫ�ֱ�����None
** ����ģ�飺None
** ������:	��΢��
** �ա��ڣ� 2012.12.17
** �ޡ��ģ�
** �ա��ڣ�
** �汾��V1.0A
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
** ��������W25QXX_WaitForWriteEnd
** �䡡�룺none  
** �䡡����none
** ����������Waiting for the last writing operation finished.
** ȫ�ֱ�����None
** ����ģ�飺None
** ������:	��΢��
** �ա��ڣ� 2012.12.17
** �ޡ��ģ�
** �ա��ڣ�
** �汾��V1.0A
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
