/*
*********************************************************************************************************
*
* 模块名称 : NAND Flash 驱动模块
* 文件名称 : bsp_nand_flash.h
* 版    本 : V1.5
* 说    明 : 头文件
*
* Copyright (C), 2013-2014, 华宁电子
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
  定义有效的 NAND ID
  HY27UF081G2A    = 0xAD 0xF1 0x80 0x1D
  K9F1G08U0A    = 0xEC 0xF1 0x80 0x15
  K9F1G08U0B    = 0xEC 0xF1 0x00 0x95
*/
#define NAND_MAKER_ID                                  0xAD
#define NAND_DEVICE_ID                                 0xF1
#define NAND_THIRD_ID                                  0x80
#define NAND_FOURTH_ID                                 0x1D

#define HY27UF081G2A_ID                          0xADF1801D
/*备份区内的标志*/
#define NAND_BAD_BLOCK_FLAG                            0xF0          /* 块内第1个page备用区的第1个字节写入非0xFF数据表示坏块 */
#define NAND_USED_BLOCK_FLAG                           0x00          /* 块内第1个page备用区的第2个字节写入非0xFF数据表示已使用的块 */
#define NAND_ERASE_BLOCK_FLAG                          0x00          /* 块内最后一个page备用区的最后1个字节写入非0XFF数据表示，该交换快数据无效 */
/*备份区内标志位的偏移地址*/
#define BI_OFFSET                                         0          /* 块内第1个page备用区的第1个字节是坏块标志 */
#define USED_OFFSET                                       1          /* 块内第1个page备用区的第1个字节是已用标志 */
#define LBN0_OFFSET                                       2          /* 块内第1个page备用区的第3个字节表示逻辑块号低8bit */
#define LBN1_OFFSET                                       3          /* 块内第1个page备用区的第4个字节表示逻辑块号高8bit */
#define VALID_SPARE_SIZE                                  4          /* 实际使用的备用区大小,用于函数内部声明数据缓冲区大小 */
#define ERASED_OFFSET                                    63          /* 仅交换区使用，擦除标志位 */
/*nand的管理信息*/
#define FAT_SECTOR_SIZE                                 512          /* 512 bytes per sector*/
#define TABLE_PACKET_SIZE                              2048

#define SECTORS_NUM_PER_PAGE    (NAND_PAGE_SIZE / FAT_SECTOR_SIZE)            // 2K/512 = 4
#define SECTORS_NUM_PER_BLOCK   (PAGES_NUM_PER_BLOCK * SECTORS_NUM_PER_PAGE)  // 64*4 = 256

#define MAX_DATA_LOG_ADDR                               960          /* 960块数据区 120M */
#define MAX_TABLE_LOG_ADDR                              963          /* 3块坏块管理表记录信息 */
#define MAX_REMAP_LOG_ADDR                              993          /* 30块坏块映射块 */
#define MAX_SWAP_LOG_ADDR                              1024          /* 31块数据交换块 */

#define BAD_TABLE_NUM                                     3          /* 坏块管理表个数   */
#define REMAP_BLOCK_NUM                                  30          /* 块坏块映射块个数 */
#define SWAP_BLOCK_NUM                                   31          /* 数据交换块个数   */

#define MIN_BLOCK_NUM                                   900          /* 最小好块数量 */

#define NAND_INIT_OK                                      1
#define NAND_INIT_ERR                                  (-1)

#define PAGE_TO_WRITE      (Transfer_Length/FAT_SECTOR_SIZE)
#define BAD_BALOK_TEST_CYCLE                              5          /* 判别坏块算法的重复擦写次数  */

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

/***************************** 华宁电子 (END OF FILE) *********************************/
#endif
