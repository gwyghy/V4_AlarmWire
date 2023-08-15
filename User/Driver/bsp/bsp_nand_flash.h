/*
*********************************************************************************************************
*
* ģ������ : NAND Flash ����ģ��
* �ļ����� : bsp_nand_flash.h
* ��    �� : V1.5
* ˵    �� : ͷ�ļ�
*
* Copyright (C), 2013-2014, ��������
*
*********************************************************************************************************
*/
#if 0
#ifndef __BSP_NAND_FLASH_H
#define __BSP_NAND_FLASH_H

typedef struct
{
  uint8_t Maker_ID;
  uint8_t Device_ID;
  uint8_t Third_ID;
  uint8_t Fourth_ID;
}NAND_IDTypeDef;

typedef struct
{
  uint16_t Zone;
  uint16_t Block;
  uint16_t Page;
} NAND_ADDRESS_T;

/*
  ������Ч�� NAND ID
  HY27UF081G2A    = 0xAD 0xF1 0x80 0x1D
  K9F1G08U0A    = 0xEC 0xF1 0x80 0x15
  K9F1G08U0B    = 0xEC 0xF1 0x00 0x95
*/
#define NAND_MAKER_ID                                  0xAD
#define NAND_DEVICE_ID                                 0xF1
#define NAND_THIRD_ID                                  0x80
#define NAND_FOURTH_ID                                 0x1D

#define HY27UF081G2A_ID                          0xADF1801D
/*�������ڵı�־*/
#define NAND_BAD_BLOCK_FLAG                            0xF0          /* ���ڵ�1��page�������ĵ�1���ֽ�д���0xFF���ݱ�ʾ���� */
#define NAND_USED_BLOCK_FLAG                           0x00          /* ���ڵ�1��page�������ĵ�2���ֽ�д���0xFF���ݱ�ʾ��ʹ�õĿ� */
#define NAND_ERASE_BLOCK_FLAG                          0x00          /* �������һ��page�����������1���ֽ�д���0XFF���ݱ�ʾ���ý�����������Ч */
/*�������ڱ�־λ��ƫ�Ƶ�ַ*/
#define BI_OFFSET                                         0          /* ���ڵ�1��page�������ĵ�1���ֽ��ǻ����־ */
#define USED_OFFSET                                       1          /* ���ڵ�1��page�������ĵ�1���ֽ������ñ�־ */
#define LBN0_OFFSET                                       2          /* ���ڵ�1��page�������ĵ�3���ֽڱ�ʾ�߼���ŵ�8bit */
#define LBN1_OFFSET                                       3          /* ���ڵ�1��page�������ĵ�4���ֽڱ�ʾ�߼���Ÿ�8bit */
#define VALID_SPARE_SIZE                                  4          /* ʵ��ʹ�õı�������С,���ں����ڲ��������ݻ�������С */
#define ERASED_OFFSET                                    63          /* ��������ʹ�ã�������־λ */
/*nand�Ĺ�����Ϣ*/
#define FAT_SECTOR_SIZE                                 512          /* 512 bytes per sector*/
#define TABLE_PACKET_SIZE                              2048

#define SECTORS_NUM_PER_PAGE    (NAND_PAGE_SIZE / FAT_SECTOR_SIZE)            // 2K/512 = 4
#define SECTORS_NUM_PER_BLOCK   (PAGES_NUM_PER_BLOCK * SECTORS_NUM_PER_PAGE)  // 64*4 = 256

#define MAX_DATA_LOG_ADDR                               960          /* 960�������� 120M */
#define MAX_TABLE_LOG_ADDR                              963          /* 3�黵�������¼��Ϣ */
#define MAX_REMAP_LOG_ADDR                              993          /* 30�黵��ӳ��� */
#define MAX_SWAP_LOG_ADDR                              1024          /* 31�����ݽ����� */

#define BAD_TABLE_NUM                                     3          /* �����������   */
#define REMAP_BLOCK_NUM                                  30          /* �黵��ӳ������ */
#define SWAP_BLOCK_NUM                                   31          /* ���ݽ��������   */

#define MIN_BLOCK_NUM                                   900          /* ��С�ÿ����� */

#define NAND_INIT_OK                                      1
#define NAND_INIT_ERR                                  (-1)

#define PAGE_TO_WRITE      (Transfer_Length/FAT_SECTOR_SIZE)
#define BAD_BALOK_TEST_CYCLE                              5          /* �б𻵿��㷨���ظ���д����  */

int8_t  bsp_NandFlashinit(void);

uint8_t NAND_Init(void);
uint8_t NAND_Format(void);
uint8_t NAND_CheckFileSystem(void);
uint8_t NAND_Read(uint32_t _ulMemAddr, uint8_t *_pReadBuf, uint16_t _usSize);
uint8_t NAND_Write(uint32_t _ulMemAddr, uint8_t *_pWriteBuf, uint16_t _usSize);

uint32_t NAND_ReadID(void);
uint16_t NAND_GetcurrentMap(void);
uint32_t NAND_FormatCapacity(void);
uint16_t NAND_LBNtoPBN(uint32_t _uiLBN);

void    NAND_MarkBadBlock(uint32_t _ulBlockNo);
uint8_t NAND_IsBadBlock(uint32_t _ulBlockNo);
uint8_t NAND_IsBufOk(uint8_t *_pBuf, uint32_t _ulLen, uint8_t _ucValue);

uint8_t NAND_WriteMultiSectors(uint8_t *_pBuf, uint32_t _SectorNo, uint16_t _SectorSize, uint32_t _SectorCount);
uint8_t NAND_ReadMultiSectors(uint8_t *_pBuf, uint32_t _SectorNo, uint16_t _SectorSize, uint32_t _SectorCount);

#endif /* __NAND_FLASH_H */

/***************************** �������� (END OF FILE) *********************************/
#endif
