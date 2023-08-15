/*
*********************************************************************************************************
*
*   ģ������ : NAND Flash ����ģ��
*   �ļ����� : bsp_fsmc_flash.h
*   ��    �� : V1.5
*   ˵    �� : ͷ�ļ�
*
*   Copyright (C), 2013-2014, ��������
*
*********************************************************************************************************
*/
#if 0
#ifndef __BSP_FSMC_FLASH_H
#define __BSP_FSMC_FLASH_H

#include "bsp_plat_data.h"
/*  ����GPIOD */
#define  FSMC_D2                   GPIOD,  GPIO_PinSource0          // D0
#define  FSMC_D3                   GPIOD,  GPIO_PinSource1          // D1
#define  FSMC_NOE                  GPIOD,  GPIO_PinSource4          // D4
#define  FSMC_NWE                  GPIOD,  GPIO_PinSource5          // D5
#define  FSMC_NCE2                 GPIOD,  GPIO_PinSource7          // D7
#define  FSMC_CLE                 GPIOD,  GPIO_PinSource11          // D11
#define  FSMC_ALE                 GPIOD,  GPIO_PinSource12          // D12
#define  FSMC_D0                  GPIOD,  GPIO_PinSource14          // D14
#define  FSMC_D1                  GPIOD,  GPIO_PinSource15          // D15

#define  PIN_FSMC_D2                            GPIO_Pin_0
#define  PIN_FSMC_D3                            GPIO_Pin_1
#define  PIN_FSMC_NOE                           GPIO_Pin_4
#define  PIN_FSMC_NWE                           GPIO_Pin_5
#define  PIN_FSMC_NCE2                          GPIO_Pin_7
#define  PIN_FSMC_CLE                          GPIO_Pin_11
#define  PIN_FSMC_ALE                          GPIO_Pin_12
#define  PIN_FSMC_D0                           GPIO_Pin_14
#define  PIN_FSMC_D1                           GPIO_Pin_15

/*  ����GPIOE */
#define  FSMC_D4                   GPIOE,  GPIO_PinSource7          // E7
#define  FSMC_D5                   GPIOE,  GPIO_PinSource8          // E8
#define  FSMC_D6                   GPIOE,  GPIO_PinSource9          // E9
#define  FSMC_D7                  GPIOE,  GPIO_PinSource10          // E10

#define  PIN_FSMC_D4                            GPIO_Pin_7
#define  PIN_FSMC_D5                            GPIO_Pin_8
#define  PIN_FSMC_D6                            GPIO_Pin_9
#define  PIN_FSMC_D7                           GPIO_Pin_10

/*  ����GPIOG */
#define  FSMC_INT2                       GPIOG, GPIO_Pin_6          // �ȴ�FSMC�������
#define  PIN_FSMC_INT2                          GPIO_Pin_6


/* ����NAND Flash�������ַ���������Ӳ�������� */
#define Bank2_NAND_ADDR             ((uint32_t)0x70000000)
#define Bank_NAND_ADDR                     Bank2_NAND_ADDR

/* �������NAND Flash�õ�3���� */
#define NAND_CMD_AREA       *(__IO uint8_t *)(Bank_NAND_ADDR | CMD_AREA)
#define NAND_ADDR_AREA      *(__IO uint8_t *)(Bank_NAND_ADDR | ADDR_AREA)
#define NAND_DATA_AREA      *(__IO uint8_t *)(Bank_NAND_ADDR | DATA_AREA)

/* Exported constants --------------------------------------------------------*/
/* NAND Area definition  for STM3210E-EVAL Board RevD */
#define CMD_AREA                          (uint32_t)(1<<16)         /* A16 = CLE  high */
#define ADDR_AREA                         (uint32_t)(1<<17)         /* A17 = ALE high */
#define DATA_AREA                    ((uint32_t)0x00000000)

/* FSMC NAND memory command */
/* ������� */
#define NAND_CMD_AREA_A                     ((uint8_t)0x00)
#define NAND_CMD_AREA_TRUE1                 ((uint8_t)0x30)
#define NAND_CMD_AREA_B                     ((uint8_t)0x05)
#define NAND_CMD_AREA_TRUE2                 ((uint8_t)0xE0)
/* д����� */
#define NAND_CMD_WRITE0                     ((uint8_t)0x80)
#define NAND_CMD_WRITE1                     ((uint8_t)0x85)
#define NAND_CMD_WRITE_TRUE1                ((uint8_t)0x10)
/* ��������� */
#define NAND_CMD_ERASE0                     ((uint8_t)0x60)
#define NAND_CMD_ERASE1                     ((uint8_t)0xD0)
/* ��ȡ�豸ID */
#define NAND_CMD_READID                     ((uint8_t)0x90)
#define NAND_CMD_READID_TRUE1               ((uint8_t)0x00)

#define NAND_CMD_LOCK_STATUS                ((uint8_t)0x7A)
#define NAND_CMD_RESET                      ((uint8_t)0xFF)
/* ��������� */
#define NAND_CMD_COPYBACK_A                 ((uint8_t)0x00)         /* PAGE COPY-BACK �������� */
#define NAND_CMD_COPYBACK_B                 ((uint8_t)0x35)
#define NAND_CMD_COPYBACK_C                 ((uint8_t)0x85)
#define NAND_CMD_COPYBACK_D                 ((uint8_t)0x10)
/* ��NAND Flash��״̬�� */
#define NAND_CMD_STATUS                     ((uint8_t)0x70)         

#define NAND_OK                                           1
#define NAND_FAIL                                         2
#define EREAD                                             3
#define EWRITE                                            4
#define EBUILT                                            5
#define NAND_DATA_ERR                                     6         /* ��չ��������ECC�������������޷����� */
#define NAND_ECC_ERR                                      7         /* ��չ��������ECC�����������󣬿���ʹ�� */

/* FSMC NAND memory parameters */
#define NAND_PAGE_SIZE                  ((uint16_t) 0x0800)         /* 2 * 1024 bytes per page w/o Spare Area */
#define PAGES_NUM_PER_BLOCK             ((uint16_t) 0x0040)         /* 64 pages per block */
#define BLOCKS_NUM_PER_ZONE             ((uint16_t) 0x0400)         /* 1024 Block per zone */
#define NAND_BLOCK_SIZE                 ((uint32_t)0x20000)         /* 64*2k = 128k  */
#define NAND_ZONE_SIZE                ((uint32_t)0x8000000)         /* 128k*1024 = 128M */
#define NAND_SPARE_AREA_SIZE            ((uint16_t) 0x0040)         /* last 64 bytes as spare area */
#define NAND_MAX_ZONE                   ((uint16_t) 0x0001)         /* 1 zones of 1024 block */

#define MAX_PHY_BLOCKS_PER_ZONE                        1024         /* ÿ������������� */
#define MAX_LOG_BLOCKS_PER_ZONE                        1000         /* ÿ��������߼���� */

#define NAND_BLOCK_COUNT                               1024         /* ����� */
#define NAND_PAGE_TOTAL_SIZE        (NAND_PAGE_SIZE + NAND_SPARE_AREA_SIZE)
                                                                    /* ҳ���ܴ�С */
/* NAND memory status */
#define NAND_BUSY                           ((uint8_t)0x00)
#define NAND_ERROR                          ((uint8_t)0x01)
#define NAND_READY                          ((uint8_t)0x40)
#define NAND_TIMEOUT_ERROR                  ((uint8_t)0x80)

#define BUFFSIZE                      NAND_PAGE_TOTAL_SIZE
#define VALID_ECC_SIZE                                    3         /* ÿ��ECC��ռ�õ��ֽ��� */
#define TOTAL_ECC_SIZE                                   12         /* �ܹ�ʹ�õ�ECCλ����С */
#define ECC_OFFSET_ADDR                                2064         /* ��չҳECC��ʼ��ַ2048+16 */
#define ECC_START_ADDR                                   16         /* ��չҳECCƫ�Ƶ�ַ */

#define WAITTIMEOUT                                   60000         /* �ȴ�ʱ�䳬ʱ */
#define SECTOR_SIZE                                     512         /* ����������С */


void     FSMC_NAND_Init(void);
uint8_t  FSMC_NAND_Reset(void);

void     FSMC_NAND_ClearECC(void);
uint32_t FSMC_NAND_ReadECC(void);

uint8_t  FSMC_NAND_EraseBlock(uint32_t _ulBlockNo);
uint8_t  FSMC_NAND_PageCopyBack(uint32_t _ulSrcPageNo, uint32_t _ulTarPageNo);
uint8_t  FSMC_NAND_PageCopyBackEx(uint32_t _ulSrcPageNo, uint32_t _ulTarPageNo,
                                 uint8_t *_pBuf, uint16_t _usOffset, uint16_t _usSize);

uint8_t FSMC_NAND_WriteData(uint8_t *_pBuffer, uint32_t _ulPageNo, uint16_t _usAddrInPage, uint16_t _usByteCount);
uint8_t FSMC_NAND_ReadData(uint8_t *_pBuffer, uint32_t _ulPageNo, uint16_t _usAddrInPage, uint16_t _usByteCount);

uint8_t FSMC_NAND_WritePage(uint8_t *_pBuffer, uint32_t _ulPageNo, uint16_t _usAddrInPage, uint16_t _usByteCount);
uint8_t FSMC_NAND_ReadPage(uint8_t *_pBuffer, uint32_t _ulPageNo, uint16_t _usAddrInPage, uint16_t _usByteCount);

uint8_t FSMC_NAND_WriteSpare(uint8_t *_pBuffer, uint32_t _ulPageNo, uint16_t _usAddrInSpare, uint16_t _usByteCount);
uint8_t FSMC_NAND_ReadSpare(uint8_t *_pBuffer, uint32_t _ulPageNo, uint16_t _usAddrInSpare, uint16_t _usByteCount);

#endif /* __FSMC_FLASH_H */
#endif

