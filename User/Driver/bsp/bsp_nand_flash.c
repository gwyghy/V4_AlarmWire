/*
*********************************************************************************************************
*
*   模块名称 : NAND Flash驱动模块
*   文件名称 : bsp_nand_flash.c
*   版    本 : V1.4
*   说    明 : 提供NAND Flash (HY27UF081G2A， 8bit 128K字节 大页)的底层接口函数。
*
*   修改记录 :
*       版本号  日期        作者     说明
*       V1.4    2013-08-20  辛鑫   正式发布
*       V1.5    2013-01-20  马金超 去除ECC操作，ECC造作由底层自动进行
*       V1.6    2013-02-08  马金超 添加了掉电保护
*
*   Copyright (C), 2013-2014, 华宁电子
*
*********************************************************************************************************
*/
#if 0
#include "stm32f4xx.h"
#include "bsp_shell.h"
#include "bsp_fsmc_flash.h"
#include "bsp_nand_flash.h"

#include <stdio.h>
#include <string.h>
/*
    该驱动适用用于海力士：HY27UF081G2A，其他ic可能需要有部分修改。
    【NAND Flash 结构定义】
    备用区有16x4字节，每page 2048字节，每512字节一个扇区，每个扇区对应16字节的备用区：

    每个PAGE的逻辑结构，前面512Bx4是主数据区，后面16Bx4是备用区
    ┌────────────┐┌────────────┐┌────────────┐┌────────────┐┌────────────┐┌────────────┐┌────────────┐┌────────────┐
    │ Main area  ││ Main area  ││ Main area  ││Main area   ││ Spare area ││ Spare area ││ Spare area ││Spare area  │
    │            ││            ││            ││            ││            ││            ││            ││            │
    │   512B     ││    512B    ││    512B    ││    512B    ││    16B     ││     16B    ││     16B    ││    16B     │
    └────────────┘└────────────┘└────────────┘└────────────┘└────────────┘└────────────┘└────────────┘└────────────┘

    K9F1G08U0A 和 HY27UF081G2A 是兼容的。芯片出厂时，厂商保证芯片的第1个块是好块。如果是坏块，则在该块的第1个PAGE的第1个字节
    或者第2个PAGE（当第1个PAGE坏了无法标记为0xFF时）的第1个字节写入非0xFF值。坏块标记值是随机的，软件直接判断是否等于0xFF即可。

    为了便于在NAND Flash 上移植Fat文件系统，我们对64B的备用区采用以下分配方案:
    - BI : 坏块标志(Bad Block Identifier)。每个BLOCK的第1个PAGE或者第2个PAGE的第1个字节指示该块是否坏块。0xFF表示好块，不是0xFF表示坏块。
    - USED : 该块使用标志。0xFF表示空闲块；0x00表示有效数据PAGE。
    - LBN0 LBN1 : 逻辑块号(Logic Block No) 。从0开始编码。只在每个BLOCK的第1个PAGE有效，其它PAGE该字段固定为0xFF FF
    - ECC : ECC校验由驱动层提供，该层次无需考虑，具体占用的位置，请参见驱动层。
    - ERASED ：交换块中的数据需要擦除前，将该标志置位，标志该数据无效。0x00表示数据已无效。
    - RSVD : 保留字节，Reserved
    【实现功能】
    (1) 实现了坏块管理。
    (2) 实现了磨损平衡，磨损平衡还存在缺陷，效果不好。　
    (3) 添加了掉电保护机制，保证了在随机掉电的情况下，NAND内的文件系统不会崩溃。
    (4) 添加了ECC校验保证数据的准确性，该校验在驱动层实现，该层次无需考虑。

    ps：虽然添加了掉电保护，但是在一种情况下是无解的，那就是在掉电修复期间，源块和交换块正好此时成为坏块，那么此时修复会失败，
        文件系统可能会出现未知情况。
    建议：在app层建立文件夹出错时，尝试换一个新的名字重新建立，这样更加保证文件系统的安全。
*/

/* 全局变量，记录计算的ECC值，按扇区计算 */
uint8_t s_ucTempBuf[BUFFSIZE]; /* 大缓冲区，2112字节. 用于读出比较 */

/* 逻辑块号映射表，用于记录此块对应的有效块 如果值为0xFFFF，说明此块为数据交换块*/
uint16_t s_usLUT[NAND_BLOCK_COUNT];
static uint16_t s_usValidDataBlockCount; /* 有效的数据块个数 */
static uint16_t s_currentRemapBlock = MAX_TABLE_LOG_ADDR; /* 当前映射的块数   */

static uint8_t NAND_BuildLUT(void);
static uint8_t NAND_ReBuildLUT(void);

static void     NAND_SaveLUTTable(void);
static uint16_t NAND_FindFreeBlock (void);
static uint8_t  NAND_MarkUsedBlock(uint32_t _ulBlockNo);

static uint16_t NAND_AddrToLogBlockNo(uint32_t _ulMemAddr);
static uint8_t  NAND_WriteToNewBlock(uint8_t *_pWriteBuf, uint32_t _SectorNo, uint16_t _SectorSize, uint32_t _SectorCount);

static uint8_t  NAND_IsFreeSector(uint32_t _PhyPageNo, uint16_t _usAddrInPage, uint16_t _SectorSize);
static uint8_t  NAND_IsFreeBlock(uint32_t _ulBlockNo);

static uint8_t  NAND_WriteOneSector(uint8_t *_pBuf, uint32_t _SectorNo, uint16_t _SectorSize);

static uint8_t NAND_IsBlockDataValid(uint32_t _ulBlockNo);
static uint8_t NAND_MarkErasedSwapBlock(uint32_t _ulBlockNo);
static uint8_t NAND_WriteToSrcBlock(uint32_t _SrcBlockNo, uint32_t _TarBlockNo);
/*
*********************************************************************************************************
*   函 数 名: bsp_NandFlashinit
*   功能说明: 初始化FSMC芯片，并建立坏块管理映射表，进行掉电检测及修复，BSP_Init()函数调用
*   形    参：无
*   返 回 值: NAND_INIT_OK正数正常
*             NAND_INIT_ERR负数异常
*********************************************************************************************************
*/
int8_t bsp_NandFlashinit(void)
{
    if ((NAND_Init() == NAND_OK) && (NAND_CheckFileSystem() == NAND_OK))
    {
        return NAND_INIT_OK;
    }
    else
    {
        return NAND_INIT_ERR;
    }
}

/*
*********************************************************************************************************
*   函 数 名: NAND_ReadID
*   功能说明: 读NAND Flash的ID。ID存储到形参指定的结构体变量中。
*   形    参：无
*   返 回 值: 32bit的NAND Flash ID
*********************************************************************************************************
*/
uint32_t NAND_ReadID(void)
{
    __IO uint32_t data = 0;
    __IO uint8_t i;

    /* 发送命令 */
    NAND_CMD_AREA  = NAND_CMD_READID;
    NAND_ADDR_AREA = NAND_CMD_READID_TRUE1;

    /* 顺序读取NAND Flash的ID */
    data = *(__IO uint32_t *)(Bank_NAND_ADDR | DATA_AREA);
    
    /* 大小端转换 */
    data =  ((data << 24) & 0xFF000000) |
            ((data << 8 ) & 0x00FF0000) |
            ((data >> 8 ) & 0x0000FF00) |
            ((data >> 24) & 0x000000FF) ;

    return data;
}

/*
*********************************************************************************************************
*   函 数 名: NAND_Init
*   功能说明: 初始化NAND Flash接口
*   形    参：无
*   返 回 值: 执行结果：
*               - NAND_FAIL 表示失败
*               - NAND_OK   表示成功
*********************************************************************************************************
*/
uint8_t NAND_Init(void)
{
    uint8_t status;

    FSMC_NAND_Init(); // 配置FSMC和GPIO用于NAND Flash接口
    FSMC_NAND_Reset(); // 通过复位命令复位NAND Flash到读状态

    status = NAND_BuildLUT();// 建立块管理表 LUT = Look up table 
    return status;
}

uint16_t NAND_GetcurrentMap(void)
{
    return s_currentRemapBlock;
}

/*
*********************************************************************************************************
*   函 数 名: NAND_WriteToNewBlock
*   功能说明: 将旧块的数据复制到新块，并将新的数据写入这个新块
*   形    参： _pWriteBuf   ：写数据缓冲区
*               _SectorNo    ：起始扇区号
*               _SectorSize  ：扇区长度，必须是512字节的整数倍
*               _SectorCount ：扇区数量
*   返 回 值: 执行结果：
*               - NAND_FAIL 表示失败
*               - NAND_OK   表示成功
*********************************************************************************************************
*/
static uint8_t NAND_WriteToNewBlock(uint8_t *_pWriteBuf, uint32_t _SectorNo, uint16_t _SectorSize, uint32_t _SectorCount)
{
    uint16_t n, i;
    uint32_t usLBN; // 逻辑块号 
    uint8_t LBNbuf[2];
    uint16_t usAddrInPage; // 页内偏移地址 
    uint8_t  ucReturn = NAND_OK;

    uint32_t ulTemp;
    uint16_t usNewBlock;
    uint16_t ulSrcBlock;
    uint16_t usInitPageNo;
    uint16_t usEndPageNo;
    uint16_t remainSectorNum;

    uint8_t  *usPBuf;
    
    if (_SectorCount == 0)
        return NAND_FAIL;

    /* 根据逻辑扇区号计算逻辑块号 */
    usLBN = _SectorNo / SECTORS_NUM_PER_BLOCK;
    ulSrcBlock = NAND_LBNtoPBN(usLBN);// 查询LUT表获得物理块号 
    if (ulSrcBlock >= NAND_BLOCK_COUNT)
    {
        /* 没有格式化，usPBN = 0xFFFF */
        return NAND_FAIL;
    }

    ulTemp = (_SectorNo % SECTORS_NUM_PER_BLOCK) * _SectorSize; // 计算块内偏移地址 
    usInitPageNo = ulTemp / NAND_PAGE_SIZE; // 在块内起始页号 

    usAddrInPage = ulTemp % NAND_PAGE_SIZE; // 计算页内偏移地址 
    usEndPageNo = ((_SectorNo % SECTORS_NUM_PER_BLOCK) + _SectorCount) / SECTORS_NUM_PER_PAGE; //在块内结束页号 

    /* 找到该块对应的逻辑号 */
    memset(LBNbuf, 0xFF, 2);
    if(FSMC_NAND_ReadSpare(LBNbuf, ulSrcBlock * PAGES_NUM_PER_BLOCK, LBN0_OFFSET, 2) == NAND_FAIL)
    {
        return NAND_FAIL;
    }
    if (LBNbuf[0] == 0xFF && LBNbuf[1] == 0xFF)
    {
       return NAND_FAIL;
    }

    /* 增加循环的目的是处理目标块为坏块的情况 */
    for (n = 0; n < REMAP_BLOCK_NUM; n++)
    {
        usNewBlock = NAND_FindFreeBlock(); // 搜寻一个可用块
        if (usNewBlock >= NAND_BLOCK_COUNT)
        {
            return NAND_FAIL; // 查找空闲块失败 
        }
        
        usPBuf = _pWriteBuf; // 指向写缓冲
        remainSectorNum = (NAND_PAGE_SIZE - usAddrInPage) / _SectorSize; //计算页内剩余扇区数 
        /* 使用NAND Flash 提供的整页Copy-Back功能，拷贝之前的页 */
        for (i = 0; i < usInitPageNo; i++)
        {
            if (FSMC_NAND_PageCopyBack(ulSrcBlock * PAGES_NUM_PER_BLOCK + i,
                                       usNewBlock * PAGES_NUM_PER_BLOCK + i) == NAND_FAIL)
            {
                ucReturn = NAND_FAIL;
                break;
            }
        }

        if (ucReturn == NAND_FAIL)
        {
            ucReturn = NAND_OK;
            NAND_MarkBadBlock(usNewBlock); // 将新块标记为坏块
            continue; // 重新寻找新块拷贝一次
        }

        if (remainSectorNum < _SectorCount) // 在当前页写不开
        {
            /* 先写满当前页 */
            if (FSMC_NAND_PageCopyBackEx(ulSrcBlock * PAGES_NUM_PER_BLOCK + i, usNewBlock * PAGES_NUM_PER_BLOCK + i,
                                         usPBuf, usAddrInPage, remainSectorNum*_SectorSize) == NAND_FAIL)
            {
                NAND_MarkBadBlock(usNewBlock); // 将新块标记为坏块
                continue; // 重新寻找新块拷贝一次
            }
            usPBuf += remainSectorNum*_SectorSize;
            remainSectorNum = _SectorCount - remainSectorNum;
            i++;

            /* 开始写整页 */
            for (; i < usEndPageNo; i++)
            {
                /* 如果写入的数据在当前页，则需要使用带随机数据的Copy-Back命令 */
                if (FSMC_NAND_WriteData(usPBuf, usNewBlock * PAGES_NUM_PER_BLOCK + i, 0, NAND_PAGE_SIZE) == NAND_FAIL)
                {
                    ucReturn = NAND_FAIL;
                    break;
                }
                usPBuf += NAND_PAGE_SIZE;
                remainSectorNum -= SECTORS_NUM_PER_PAGE;
            }

            if (ucReturn == NAND_FAIL)
            {
                ucReturn = NAND_OK;
                NAND_MarkBadBlock(usNewBlock); // 将新块标记为坏块
                continue; // 重新寻找新块拷贝一次
            }

            /* 写最后一页 */
            if (i == usEndPageNo) // 写到最后一页
            {
                if (remainSectorNum) // 2013年9月4日修改此处
                {
                    /* 如果写入的数据在当前页，则需要使用带随机数据的Copy-Back命令 */
                    if (FSMC_NAND_PageCopyBackEx(ulSrcBlock * PAGES_NUM_PER_BLOCK + i, usNewBlock * PAGES_NUM_PER_BLOCK + i,
                                                 usPBuf, 0, remainSectorNum*_SectorSize) == NAND_FAIL)
                    {
                        NAND_MarkBadBlock(usNewBlock); // 将新块标记为坏块
                        continue; // 重新寻找新块拷贝一次
                    }
                    i++;
                }
            }
        }
        else
        {
            /* 如果写入的数据在当前页，则需要使用带随机数据的Copy-Back命令 */
            if (FSMC_NAND_PageCopyBackEx(ulSrcBlock * PAGES_NUM_PER_BLOCK + i, usNewBlock * PAGES_NUM_PER_BLOCK + i,
                                         usPBuf, usAddrInPage, _SectorSize*_SectorCount) == NAND_FAIL)
            {
                NAND_MarkBadBlock(usNewBlock); // 将新块标记为坏块
                continue; // 重新寻找新块拷贝一次
            }
            i++;
        }

        /* 使用page-copy功能，将后面的页全部复制*/
        for (; i < PAGES_NUM_PER_BLOCK; i++)
        {
            if (FSMC_NAND_PageCopyBack(ulSrcBlock * PAGES_NUM_PER_BLOCK + i,
                                       usNewBlock * PAGES_NUM_PER_BLOCK + i) == NAND_FAIL)
            {
                ucReturn = NAND_FAIL;
                break;
            }
        }

        if (ucReturn == NAND_FAIL)
        {
            ucReturn = NAND_OK;
            NAND_MarkBadBlock(usNewBlock); // 将新块标记为坏块
            continue; // 重新寻找新块拷贝一次
        }
        break;
    }

    if (n == REMAP_BLOCK_NUM) // 没有映射块可用了
    {
        return NAND_FAIL;
    }
    /* 目标块更新成功 */
    else
    {

        /* 将最后一页的逻辑号标注上，放在此处为了可靠的掉电保护 */
        /* 即使原来存在数据，因为数据相同也不会报错 */
        if(FSMC_NAND_WriteSpare(LBNbuf, (usNewBlock + 1) * PAGES_NUM_PER_BLOCK - 1, LBN0_OFFSET, 2) == NAND_FAIL)
        {
            return NAND_FAIL;
        }

        /* 增加循环的目的是处理目标块为坏块的情况 */
        for (n = 0; n < REMAP_BLOCK_NUM; n++)
        {
            /* 擦除源BLOCK */
            if (FSMC_NAND_EraseBlock(ulSrcBlock) != NAND_READY)
            {
                NAND_MarkBadBlock(usLBN); // 将源块标记为坏块
                s_usLUT[usLBN] = s_currentRemapBlock; // 更新逻辑表

                ulSrcBlock = s_currentRemapBlock;
                s_currentRemapBlock++;
                NAND_SaveLUTTable(); // 保存LUT表 */
                
                if (s_currentRemapBlock >= MAX_REMAP_LOG_ADDR) // 坏块映射块用光了
                {
                    return NAND_FAIL;
                }

                continue; // 重新寻找新块拷贝一次
            }
            /* 拷贝回原来块中 */
            for (i = 0; i < PAGES_NUM_PER_BLOCK; i++)
            {
                if (FSMC_NAND_PageCopyBack(usNewBlock * PAGES_NUM_PER_BLOCK + i,
                                           ulSrcBlock * PAGES_NUM_PER_BLOCK + i) == NAND_FAIL)
                {
                    ucReturn = NAND_FAIL;
                    break;
                }
            }

            if (ucReturn == NAND_FAIL)
            {
                NAND_MarkBadBlock(usLBN); // 将源块标记为坏块
                s_usLUT[usLBN] = s_currentRemapBlock; // 更新逻辑表

                ulSrcBlock = s_currentRemapBlock;
                s_currentRemapBlock++;
                if (s_currentRemapBlock >= MAX_REMAP_LOG_ADDR) // 坏块映射块用光了
                {
                    return NAND_FAIL;
                }
                NAND_SaveLUTTable(); // 保存LUT表 */

                ucReturn = NAND_OK;
                continue; // 重新寻找新块拷贝一次
            }
            else
            {
                /* 标记该块已用 */
                NAND_MarkUsedBlock(ulSrcBlock);
                break;
            }
        }

        if (n == REMAP_BLOCK_NUM) // 没有映射块可用了
        {
            ucReturn = NAND_FAIL;
        }
    }

    if (NAND_MarkErasedSwapBlock(usNewBlock))//将交换区的擦除标志置位
    {
        if (FSMC_NAND_EraseBlock(usNewBlock) != NAND_READY) // 擦除交换块
        {
            NAND_MarkBadBlock(usNewBlock); // 将新块标记为坏块
        }
    }
    else
    {
        NAND_MarkBadBlock(usNewBlock);
    }

    return ucReturn;// 写入成功 
}

/*
*********************************************************************************************************
*   函 数 名: NAND_Write
*   功能说明: 写一个扇区
*   形    参： _MemAddr   : 内存单元物理地址
*               _pReadbuff ：存放待写数据的缓冲区的指针
*               _usSize    ：数据长度，必须是FAT_SECTOR_SIZE字节的整数倍
*   返 回 值: 执行结果：
*               - NAND_FAIL 表示失败
*               - NAND_OK   表示成功
*********************************************************************************************************
*/
uint8_t NAND_Write(uint32_t _ulMemAddr, uint8_t *_pWriteBuf, uint16_t _usSize)
{
    uint16_t usLBN; // 逻辑块号
    uint16_t usPBN; // 物理块号
    uint32_t ulPhyPageNo; // 物理页号
    uint32_t usSectorsInBlock; // 块内偏移扇区数
    uint16_t usAddrInPage; // 页内偏移地址
    uint32_t ulTemp;

    /* 数据长度必须是FAT_SECTOR_SIZE字节整数倍 */
    if ((_usSize % FAT_SECTOR_SIZE) != 0)
    {
        return NAND_FAIL;
    }

    usLBN = NAND_AddrToLogBlockNo(_ulMemAddr); // 获得逻辑块号
    usPBN = NAND_LBNtoPBN(usLBN); // 查询LUT表获得物理块号

    ulTemp       = _ulMemAddr % NAND_BLOCK_SIZE; // 块内偏移地址
    usSectorsInBlock = ulTemp / FAT_SECTOR_SIZE;
    ulPhyPageNo  = usPBN * PAGES_NUM_PER_BLOCK + ulTemp / NAND_PAGE_SIZE; // 计算物理页号 
    usAddrInPage = ulTemp % NAND_PAGE_SIZE; //计算页内偏移地址 

    /*　如果是未使用块 */
    if (NAND_IsFreeBlock(usPBN))
    {
        if (FSMC_NAND_WriteData(_pWriteBuf, ulPhyPageNo, usAddrInPage, _usSize) == NAND_FAIL)
        {
            return NAND_FAIL;
        }

        /* 标记该块已用 */
        if (NAND_MarkUsedBlock(ulPhyPageNo) == NAND_FAIL)
        {
            /* 标记失败，将数据写入到另外一个块（空闲块） */
            return NAND_FAIL;
        }

        return NAND_OK; // 写入成功 
    }

    /* 将数据写入到另外一个块（空闲块） */
    usSectorsInBlock += usLBN*SECTORS_NUM_PER_BLOCK;
    return NAND_WriteToNewBlock(_pWriteBuf, usSectorsInBlock, FAT_SECTOR_SIZE, _usSize/FAT_SECTOR_SIZE);
}

/*
*********************************************************************************************************
*   函 数 名: NAND_Read
*   功能说明: 读一个扇区
*   形    参： _MemAddr   : 内存单元物理地址
*               _pReadbuff ：存放读出数据的缓冲区的指针
*               _usSize    ：数据长度，必须是4字节的整数倍
*   返 回 值: 执行结果：
*               - NAND_FAIL 表示失败
*               - NAND_OK   表示成功
*********************************************************************************************************
*/
uint8_t NAND_Read(uint32_t _ulMemAddr, uint8_t *_pReadBuf, uint16_t _usSize)
{
    uint16_t usLBN; // 逻辑块号 
    uint16_t usPBN; // 物理块号 
    uint32_t ulPhyPageNo; // 物理页号 
    uint16_t usAddrInPage; // 页内偏移地址 
    uint32_t ulTemp;

    /* 数据长度必须是4字节整数倍 */
    if ((_usSize % 4) != 0)
    {
        return NAND_FAIL;
    }

    usLBN = NAND_AddrToLogBlockNo(_ulMemAddr);  // 计算物理块号 
    usPBN = NAND_LBNtoPBN(usLBN); // 查询LUT表获得物理块号 

    if (usPBN >= NAND_BLOCK_COUNT)
    {
        /* 没有格式化，usPBN = 0xFFFF */
        return NAND_FAIL;
    }

    ulTemp       = _ulMemAddr % NAND_BLOCK_SIZE;
    ulPhyPageNo  = usPBN * PAGES_NUM_PER_BLOCK + ulTemp / NAND_PAGE_SIZE; // 计算物理页号 
    usAddrInPage = ulTemp % NAND_PAGE_SIZE; // 计算页内偏移地址 

    if (FSMC_NAND_ReadData(_pReadBuf, ulPhyPageNo, usAddrInPage, _usSize) == NAND_FAIL)
    {
        return NAND_FAIL; // 读NAND Flash失败
    }

    /* 成功 */
    return NAND_OK;
}

/*
*********************************************************************************************************
*   函 数 名: NAND_WriteOneSector
*   功能说明: 该函数用于文件系统，写一个扇区数据。扇区大小可以是512字节或2048字节
*   形    参： _pBuf : 存放数据的缓冲区的指针
*               _SectorNo   ：扇区号
*               _SectorSize ：每个扇区的大小
*   返 回 值: 执行结果：
*               - NAND_FAIL 表示失败
*               - NAND_OK   表示成功
*   注 意:    该函数为了配合写多扇区函数使用，写前没进行块擦除操作，单独使用此函数要注意
*********************************************************************************************************
*/
static uint8_t NAND_WriteOneSector(uint8_t *_pBuf, uint32_t _SectorNo, uint16_t _SectorSize)
{
    uint16_t usLBN; // 逻辑块号
    uint16_t usPBN; // 物理块号
    uint32_t uiPhyPageNo; // 物理页号
    uint16_t usAddrInPage; // 页内偏移地址
    uint32_t ulTemp;
    
    if (_SectorSize == 0)
        return NAND_FAIL;

    /* 根据逻辑扇区号计算逻辑块号 */
    usLBN = _SectorNo / SECTORS_NUM_PER_BLOCK;
    usPBN = NAND_LBNtoPBN(usLBN); // 查询LUT表获得物理块号 
    if (usPBN >= NAND_BLOCK_COUNT)
    {
        /* 没有格式化，usPBN = 0xFFFF */
        return NAND_FAIL;
    }

    ulTemp = (_SectorNo % SECTORS_NUM_PER_BLOCK) * _SectorSize; // 计算块内偏移地址
    uiPhyPageNo  = usPBN * PAGES_NUM_PER_BLOCK + ulTemp / NAND_PAGE_SIZE; //计算物理页号
    usAddrInPage = ulTemp % NAND_PAGE_SIZE; // 计算页内偏移地址

    if (!NAND_IsFreeSector(uiPhyPageNo, usAddrInPage, _SectorSize)) // 如果不是空闲扇区
    {
        return EWRITE;  // 写NAND Flash失败
    }

    if (FSMC_NAND_WriteData(_pBuf, uiPhyPageNo, usAddrInPage, _SectorSize) == NAND_FAIL)
    {
        return NAND_FAIL; // 写NAND Flash失败
    }

    return NAND_OK;
}

/*
*********************************************************************************************************
*   函 数 名: NAND_WriteMultiSectors
*   功能说明: 该函数用于文件系统，连续写多个扇区数据。扇区大小可以是512字节
*   形    参： _pBuf : 存放数据的缓冲区的指针
*               _SectorNo    ：扇区号
*               _SectorSize  ：每个扇区的大小
*               _SectorCount : 扇区个数
*   返 回 值: 执行结果：
*               - NAND_FAIL 表示失败
*               - NAND_OK   表示成功
*********************************************************************************************************
*/
uint8_t NAND_WriteMultiSectors(uint8_t *_pBuf, uint32_t _SectorNo, uint16_t _SectorSize, uint32_t _SectorCount)
{
    uint32_t i;
    uint16_t usLBN; // 逻辑块号
    uint16_t usPBN; // 物理块号
    uint16_t usSectorInBlock; // 块内扇区数
    uint16_t remainSectorNum; // 剩余扇区数量

    uint8_t  *usPBuf;
    uint8_t  ucReturn;

    /*
        HY27UF081G2A = 128M Flash.  有 1024个BLOCK, 每个BLOCK包含64个PAGE， 每个PAGE包含2048+64字节，
        擦除最小单位是BLOCK， 编程最小单位是字节。

        每个PAGE在逻辑上可以分为4个512字节扇区。
    */
    usLBN = _SectorNo / SECTORS_NUM_PER_BLOCK; // 计算当前块号
    usPBN = NAND_LBNtoPBN(usLBN); // 查询LUT表获得物理块号

    usSectorInBlock = _SectorNo % SECTORS_NUM_PER_BLOCK; // 计算块内偏移扇区数
    usPBuf = _pBuf;  // 直接操作参数指针会导致USB通讯错误

    if ((usSectorInBlock+_SectorCount) > SECTORS_NUM_PER_BLOCK)                   
    {   /* 一块写不下 */
        remainSectorNum = usSectorInBlock + _SectorCount - SECTORS_NUM_PER_BLOCK;
        /* 先写完本块 */
        for (i = 0; i < (_SectorCount-remainSectorNum); i++)
        {
            ucReturn = NAND_WriteOneSector(usPBuf, _SectorNo, _SectorSize); // 写一扇区内容
            if (NAND_FAIL == ucReturn)
            {
                return NAND_FAIL;   
            }
            else if (EWRITE == ucReturn)
            {
                break;
            }
            _SectorNo++;
            usPBuf += _SectorSize; // 缓冲区偏移
        }

        if (i == (_SectorCount-remainSectorNum)) // 正常写完
        {
            /* 标记该块已用 */
            if (NAND_MarkUsedBlock(usPBN) == NAND_FAIL)
            {
                return NAND_FAIL;    
            }
        }
        else  // 是使用块
        {
            /* 将数据写入到另外一个块（空闲块） */
            ucReturn = NAND_WriteToNewBlock(usPBuf, _SectorNo, _SectorSize, (_SectorCount-remainSectorNum-i));
            _SectorNo += _SectorCount-remainSectorNum-i;
            usPBuf += (_SectorCount-remainSectorNum-i)*_SectorSize; // add by xinxin

            if (ucReturn != NAND_OK)
            {
                return NAND_FAIL;   
            }
        }
        /* 写下一块 */
        usLBN++;
        usPBN = NAND_LBNtoPBN(usLBN); // 查询LUT表获得物理块号

        for (i = 0; i < remainSectorNum; i++)
        {
            ucReturn = NAND_WriteOneSector(usPBuf, _SectorNo, _SectorSize); // 写一扇区内容
            if (NAND_FAIL == ucReturn)
            {
                return NAND_FAIL;   
            }
            else if (EWRITE == ucReturn)
            {
                break;
            }
            _SectorNo++;
            usPBuf += _SectorSize; // 缓冲区偏移
        }

        if (i == remainSectorNum) // 正常写完
        {
            /* 标记该块已用 */
            if (NAND_MarkUsedBlock(usPBN) == NAND_FAIL)
            {
                return NAND_FAIL;   
            }
        }
        else // 是使用块
        {
            /* 将数据写入到另外一个块（空闲块） */
            ucReturn = NAND_WriteToNewBlock(usPBuf, _SectorNo, _SectorSize, remainSectorNum-i);
            if (ucReturn != NAND_OK) // add by xinxin
            {
                return NAND_FAIL;    
            }
        }
    }
    else        
    {   /* 一块写得下 */
        for (i = 0; i < _SectorCount; i++)
        {
            ucReturn = NAND_WriteOneSector(usPBuf, _SectorNo, _SectorSize); // 写一扇区内容
            if (NAND_FAIL == ucReturn)
            {
                return NAND_FAIL;    
            }
            else if (EWRITE == ucReturn)
            {
                break;
            }
            _SectorNo++;
            usPBuf += _SectorSize; // 缓冲区偏移
        }

        if (i == _SectorCount) // 所有扇区写完
        {
            /* 标记该块已用 */
            if (NAND_MarkUsedBlock(usPBN) == NAND_FAIL)
            {
                return NAND_FAIL;    
            }
        }
        else // 是使用扇区
        {
            /* 将数据写入到另外一个块（空闲块） */
            ucReturn = NAND_WriteToNewBlock(usPBuf, _SectorNo, _SectorSize, _SectorCount-i);
            if (ucReturn != NAND_OK)
            {
                return NAND_FAIL;    
            }
        }
    }

    return NAND_OK;     
}

/*
*********************************************************************************************************
*   函 数 名: NAND_ReadOneSector
*   功能说明: 该函数用于文件系统，按扇区读数据。读1个扇区，扇区大小可以是512字节
*   形    参： _pBuf : 存放读出数据的缓冲区的指针
*               _SectorNo   ：扇区号
*               _SectorSize ：每个扇区的大小
*   返 回 值: 执行结果：
*               - NAND_FAIL 表示失败
*               - NAND_OK   表示成功
*********************************************************************************************************
*/
static uint8_t NAND_ReadOneSector(uint8_t *_pBuf, uint32_t _SectorNo, uint16_t _SectorSize)
{
    uint16_t usLBN; // 逻辑块号 
    uint16_t usPBN; // 物理块号 
    uint32_t uiPhyPageNo; // 物理页号 
    uint16_t usAddrInPage; // 页内偏移地址 
    uint32_t ulTemp;

    /* 根据逻辑扇区号和扇区大小计算逻辑块号 */
    usLBN = _SectorNo / SECTORS_NUM_PER_BLOCK;
    usPBN = NAND_LBNtoPBN(usLBN); // 查询LUT表获得物理块号 

    if (usPBN >= NAND_BLOCK_COUNT)
    {
        /* 没有格式化，usPBN = 0xFFFF */
        return NAND_FAIL;
    }

    ulTemp = (_SectorNo % SECTORS_NUM_PER_BLOCK) * _SectorSize;
    uiPhyPageNo  = usPBN * PAGES_NUM_PER_BLOCK + ulTemp / NAND_PAGE_SIZE;  // 计算物理页号 
    usAddrInPage = ulTemp % NAND_PAGE_SIZE; // 计算页内偏移地址 

    if (FSMC_NAND_ReadData(_pBuf, uiPhyPageNo, usAddrInPage, _SectorSize) == NAND_FAIL)
    {
        return NAND_FAIL; // 读NAND Flash失败 
    }

    return NAND_OK;
}

/*
*********************************************************************************************************
*   函 数 名: NAND_ReadMultiSectors
*   功能说明: 该函数用于文件系统，按扇区读数据。读1个或多个扇区，扇区大小可以是512字节
*   形    参： _pBuf : 存放读出数据的缓冲区的指针
*               _SectorNo ：扇区号
*               _SectorSize ：每个扇区的大小
*               _SectorCount : 扇区个数
*   返 回 值: 执行结果：
*               - NAND_OK 成功;
*               - NAND_FAIL 失败
*********************************************************************************************************
*/
uint8_t NAND_ReadMultiSectors(uint8_t *_pBuf, uint32_t _SectorNo, uint16_t _SectorSize, uint32_t _SectorCount)
{
    uint32_t i;
    uint8_t  *usPBuf;
    usPBuf = _pBuf; // 直接操作参数指针会导致USB通讯错误
    /*
        HY27UF081G2A = 128M Flash.  有 1024个BLOCK, 每个BLOCK包含64个PAGE， 每个PAGE包含2048+64字节，
        擦除最小单位是BLOCK， 编程最小单位是字节。

        每个PAGE在逻辑上可以分为4个512字节扇区。
    */
    for (i = 0; i < _SectorCount; i++)
    {
        if(NAND_ReadOneSector(usPBuf, _SectorNo, _SectorSize) == NAND_FAIL) // 读一个扇区数据
        {
            return NAND_FAIL; // 读NAND Flash失败
        }

        _SectorNo++;
        usPBuf += _SectorSize; // 缓冲区偏移
    }

    /* 成功 */
    return NAND_OK;
}

/*
*********************************************************************************************************
*   函 数 名: NAND_BuildLUT
*   功能说明: 在内存中创建坏块管理表
*   形    参：-
*   返 回 值: 执行结果
*              - NAND_OK： 成功；  
*              - NAND_FAIL：失败
*********************************************************************************************************
*/
static uint8_t NAND_BuildLUT(void)
{
    uint16_t i, j, k, t;
    uint32_t currentReadPage;
    uint16_t remainSize;

    for (i = 0; i < NAND_BLOCK_COUNT; i++)
    {
        s_usLUT[i] = 0xFFFF; // 填充无效值，用于重建LUT后，判断LUT是否合理
    }

    /* 读取坏块管理表记录信息 */
    for (i = 0; i < BAD_TABLE_NUM; i++)
    {
        currentReadPage = (i+MAX_DATA_LOG_ADDR) * PAGES_NUM_PER_BLOCK;
        
        if (!NAND_IsBadBlock(i+MAX_DATA_LOG_ADDR)) // 如果坏块管理表不是坏块
        {
            if (FSMC_NAND_ReadSpare((uint8_t *)&s_currentRemapBlock, currentReadPage, LBN0_OFFSET, 2) == NAND_FAIL) // 读取已使用的坏块映射
            {
                continue; // 读取下一个逻辑表
            }

            remainSize = NAND_BLOCK_COUNT << 1;
            k = 0,
            t = 0;
            while (1)
            {
                if (FSMC_NAND_ReadData(s_ucTempBuf, currentReadPage + t, 0, TABLE_PACKET_SIZE) == NAND_FAIL)
                {
                    break; // 读取下一个逻辑表
                }

                for (j = 0; j < TABLE_PACKET_SIZE; k++)
                {
                    s_usLUT[k] = s_ucTempBuf[j] + ((u16)s_ucTempBuf[j+1]<<8); // 更新LUT表
                    j += 2;
                }
                remainSize -= TABLE_PACKET_SIZE;
                t++;

                if (remainSize < TABLE_PACKET_SIZE) // 不足2048
                {
                    if (remainSize) // 仍有剩余字节
                    {
                        if (FSMC_NAND_ReadData(s_ucTempBuf, currentReadPage + t, 0, remainSize) == NAND_FAIL)
                        {
                            break; // 读取下一个逻辑表
                        }

                        for (j = 0; j < remainSize; k++)
                        {
                            s_usLUT[k] = s_ucTempBuf[j] + ((u16)s_ucTempBuf[j+1]<<8); // 更新LUT表
                            j += 2;
                        }
                        remainSize = 0;
                    }
                    break;
                }
            }

            if (remainSize == 0)
            {
                break;
            }
        }
    }

    if (i == BAD_TABLE_NUM) // 如果坏块管理表全损坏
    {
        NAND_ReBuildLUT(); // 重新建立坏块管理表
    }

    /* LUT建立完毕，检查是否合理 */
    for (i = 0; i < NAND_BLOCK_COUNT; i++)
    {
        if (s_usLUT[i] == 0xFFFF) // 第一个为0xFFFF值
        {
            s_usValidDataBlockCount = i;
            break;
        }
    }

    if (s_usValidDataBlockCount < MIN_BLOCK_NUM)
    {
        /* 错误： 最大的有效逻辑块号小于MIN_BLOCK_NUM。可能是没有格式化 */
        return NAND_FAIL;
    }

    for (i = 0; i < s_usValidDataBlockCount; i++)
    {
        if (s_usLUT[i] == 0xFFFF)
        {
            return NAND_FAIL;    // 错误：LUT表逻辑块号存在跳跃现象，可能是没有格式化
        }
    }

    /* 重建LUT正常 */
    return NAND_OK;
}

/*
*********************************************************************************************************
*   函 数 名: NAND_ReBuildLUT
*   功能说明: 根据每页额外信息，在内存中创建坏块管理表
*   形    参：-
*   返 回 值: 执行结果：
*               - NAND_OK： 成功；     
*               - NAND_FAIL：失败
*********************************************************************************************************
*/
static uint8_t NAND_ReBuildLUT(void)
{
    uint16_t i;
    uint8_t  buf[VALID_SPARE_SIZE];

    for (i = 0; i < NAND_BLOCK_COUNT; i++)
    {
        /* 读每个块的第1个PAGE，偏移地址为LBN0_OFFSET的数据 */
        FSMC_NAND_ReadSpare(buf, i * PAGES_NUM_PER_BLOCK, BI_OFFSET, VALID_SPARE_SIZE);

        s_usLUT[i] = buf[LBN0_OFFSET] + ((u16)buf[LBN1_OFFSET] << 8);// 计算读出的逻辑块号

        if (s_usLUT[i] != i)// 逻辑块与物理块不等，则为重映射
        {
            s_currentRemapBlock++;
        }
    }
    return NAND_OK;
}

/*
*********************************************************************************************************
*   函 数 名: NAND_AddrToLogBlockNo
*   功能说明: 内存逻辑地址转换为逻辑块号
*   形    参：_ulMemAddr：逻辑内存地址
*   返 回 值: 物理页号， 如果是 0xFFFFFFFF 则表示错误
*********************************************************************************************************
*/
static uint16_t NAND_AddrToLogBlockNo(uint32_t _ulMemAddr)
{
    uint16_t usLBN; // 逻辑块号 
    usLBN = _ulMemAddr / NAND_BLOCK_SIZE;//计算逻辑块号 

    return usLBN;
}

/*
*********************************************************************************************************
*   函 数 名: NAND_LBNtoPBN
*   功能说明: 逻辑块号转换为物理块号
*   形    参: _uiLBN : 逻辑块号 Logic Block No
*   返 回 值: 物理块号， 如果是 0xFFFFFFFF 则表示错误
*********************************************************************************************************
*/
uint16_t NAND_LBNtoPBN(uint32_t _uiLBN)
{
    uint16_t usPBN;     /* 物理块号 */

    /* 如果逻辑块号大于有效的数据块个数则固定返回0xFFFF, 调用该函数的代码应该检查出这种错误 */
    if (s_usLUT[_uiLBN] == 0xFFFF)
    {
        return 0xFFFF;
    }

    /* 查询LUT表，获得映射后的物理块号 */
    usPBN = s_usLUT[_uiLBN];
    return usPBN;
}

/*
*********************************************************************************************************
*   函 数 名: NAND_FindFreeBlock
*   功能说明: 查找一个可用的块作为交换块。
*   形    参：-
*   返 回 值: 块号，如果是0xFFFF表示失败
*********************************************************************************************************
*/
static uint16_t NAND_FindFreeBlock (void)
{
    static uint16_t swapNum = MAX_REMAP_LOG_ADDR; // 从交换区地址开始
    uint16_t i;
    uint16_t freeBlock = 0xFFFF;

    for (i = 0; i < SWAP_BLOCK_NUM; i++) // 尝试所有交换块
    {
        if (NAND_IsFreeBlock(swapNum))
        {
            freeBlock = swapNum;
            swapNum++;
            break;
        }
        swapNum++;
    }

    if (swapNum >= MAX_SWAP_LOG_ADDR)
    {
        swapNum = MAX_REMAP_LOG_ADDR;
    }

    return freeBlock;
}

/*
*********************************************************************************************************
*   函 数 名: NAND_IsBufOk
*   功能说明: 判断内存缓冲区的数据是否全部为指定值
*   形    参：- _pBuf : 输入缓冲区
*             - _ulLen : 缓冲区长度
*             - __ucValue : 缓冲区每个单元的正确数值
*   返 回 值: 1 ：全部正确;
*             0 ：不正确
*********************************************************************************************************
*/
uint8_t NAND_IsBufOk(uint8_t *_pBuf, uint32_t _ulLen, uint8_t _ucValue)
{
    uint32_t i;

    for (i = 0; i < _ulLen; i++)
    {
        if (_pBuf[i] != _ucValue)
        {
            return 0;
        }
    }

    return 1;
}

/*
*********************************************************************************************************
*   函 数 名: NAND_IsBadBlock
*   功能说明: 根据坏块标记检测NAND Flash指定的块是否坏块
*   形    参: _ulBlockNo ：块号 0 - 1023 （对于128M字节，2K Page的NAND Flash，有1024个块）
*   返 回 值: 0 ：该块可用;
*             1 ：该块是坏块
*********************************************************************************************************
*/
uint8_t NAND_IsBadBlock(uint32_t _ulBlockNo)
{
    uint8_t ucFlag;

    /* 如果NAND Flash出厂前已经标注为坏块了，则就认为是坏块 */
    FSMC_NAND_ReadSpare(&ucFlag, _ulBlockNo * PAGES_NUM_PER_BLOCK, BI_OFFSET, 1);
    if (ucFlag != 0xFF)
    {
        return 1;
    }

    FSMC_NAND_ReadSpare(&ucFlag, (_ulBlockNo + 1) * PAGES_NUM_PER_BLOCK - 1, BI_OFFSET, 1);
    if (ucFlag != 0xFF)
    {
        return 1;
    }

    return 0;   // 是好块 
}

/*
*********************************************************************************************************
*   函 数 名: NAND_IsFreeBlock
*   功能说明: 根据坏块标记和USED标志检测是否可用块
*   形    参: _ulBlockNo ：块号 0 - 1023 （对于128M字节，2K Page的NAND Flash，有1024个块）
*   返 回 值: 1 ：该块可用;
*             0 ：该块是坏块或者已占用
*********************************************************************************************************
*/
static uint8_t NAND_IsFreeBlock(uint32_t _ulBlockNo)
{
    uint8_t ucFlag;

    /* 如果NAND Flash出厂前已经标注为坏块了，则就认为是坏块 */
    if (NAND_IsBadBlock(_ulBlockNo))
    {
        return 0;
    }

    FSMC_NAND_ReadSpare(&ucFlag, _ulBlockNo * PAGES_NUM_PER_BLOCK, USED_OFFSET, 1);
    if (ucFlag == 0xFF)
    {
        return 1;//该块可用
    }

    return 0;
}

/*
*********************************************************************************************************
*   函 数 名: NAND_IsFreeSector
*   功能说明: 根据扇区内数据是否全为FF判断扇区是否使用
*   形    参: _uiPhyPageNo ：物理页号，usAddrInPage：页内偏移地址
*   返 回 值: 1 ：该块未使用;
*             0 ：该扇区已使用
*********************************************************************************************************
*/
static uint8_t NAND_IsFreeSector(uint32_t _PhyPageNo, uint16_t _usAddrInPage, uint16_t _SectorSize)
{
    FSMC_NAND_ReadData(s_ucTempBuf, _PhyPageNo, _usAddrInPage, _SectorSize);
    if (NAND_IsBufOk(s_ucTempBuf, _SectorSize, 0xFF))
    {
        return 1;
    }

    return 0;
}

/*
*********************************************************************************************************
*   函 数 名: NAND_MarkUsedBlock
*   功能说明: 标记NAND Flash指定的块为已用块
*   形    参: _ulBlockNo ：块号 0 - 1023 （对于128M字节，2K Page的NAND Flash，有1024个块）
*   返 回 值: 执行结果：
*               - NAND_OK:标记成功;
*               - NAND_FAIL：标记失败，上级软件应该进行坏块处理。
*********************************************************************************************************
*/
static uint8_t NAND_MarkUsedBlock(uint32_t _ulBlockNo)
{
    uint32_t ulPageNo;
    uint8_t  ucFlag;

    /* 计算块的第1个页号 */
    ulPageNo = _ulBlockNo * PAGES_NUM_PER_BLOCK; // 计算该块第1个页的页号

    /* 块内第1个page备用区的第2个字节写入非0xFF数据表示已用块 */
    ucFlag = NAND_USED_BLOCK_FLAG;
    if (FSMC_NAND_WriteSpare(&ucFlag, ulPageNo, USED_OFFSET, 1) == NAND_FAIL)
    {
        /* 如果标记失败，则需要标注这个块为坏块 */
        return NAND_FAIL;
    }

    return NAND_OK;
}

/*
*********************************************************************************************************
*   函 数 名: NAND_MarkErasedSwapBlock
*   功能说明: 标记NAND Flash指定的块的擦除标志
*   形    参: _ulBlockNo ：块号 0 - 1023 （对于128M字节，2K Page的NAND Flash，有1024个块）
*   返 回 值: 执行结果:
*               - NAND_OK:标记成功;
*               - NAND_FAIL：标记失败，上级软件应该进行坏块处理。
*********************************************************************************************************
*/
static uint8_t NAND_MarkErasedSwapBlock(uint32_t _ulBlockNo)
{
    uint32_t ulPageNo;
    uint8_t  ucFlag;

    /* 计算块的最后1个页号 */
    ulPageNo = (_ulBlockNo + 1) * PAGES_NUM_PER_BLOCK - 1; //计算该块最后1个页的页号 

    /* 最后一页的扩展数据区内的最后一位表示擦除标志位 */
    ucFlag = NAND_ERASE_BLOCK_FLAG;
    if (FSMC_NAND_WriteSpare(&ucFlag, ulPageNo, ERASED_OFFSET, 1) == NAND_FAIL)
    {
        /* 如果标记失败，则需要标注这个块为坏块 */
        return NAND_FAIL;
    }

    return NAND_OK;
}

/*
*********************************************************************************************************
*   函 数 名: NAND_IsFreeBlock
*   功能说明: 读取一块内最后一页的擦除标志
*   形    参: _ulBlockNo ：块号 0 - 1023 （对于128M字节，2K Page的NAND Flash，有1024个块）
*   返 回 值: 1 ：擦除标志置位，该块数据已经无用，需擦除;
*             0 ：该块未准备擦除，不确定数据是否有效
*********************************************************************************************************
*/
static uint8_t NAND_IsErasedSwapBlock(uint32_t _ulBlockNo)
{
    uint8_t ucFlag;

    /* 读取该块的最后一页的ERASED_OFFSET位，如果该位位0x00，则擦除标志已经置位 */
    FSMC_NAND_ReadSpare(&ucFlag, ((_ulBlockNo + 1) * PAGES_NUM_PER_BLOCK -1), ERASED_OFFSET, 1);
    if (ucFlag != 0xFF)
    {
        return 1;
    }

    return 0;
}

/*
*********************************************************************************************************
*   函 数 名: NAND_MarkBadBlock
*   功能说明: 标记NAND Flash指定的块为坏块
*   形    参: _ulBlockNo ：块号 0 - 1023 （对于128M字节，2K Page的NAND Flash，有1024个块）
*   返 回 值: 无
*********************************************************************************************************
*/
void NAND_MarkBadBlock(uint32_t _ulBlockNo)
{
    uint32_t ulPageNo;
    uint8_t  ucFlag;

    /* 计算块的第1个页号 */
    ulPageNo = _ulBlockNo * PAGES_NUM_PER_BLOCK; //计算该块第1个页的页号 

    /* 块内第1个page备用区的第1个字节写入非0xFF数据表示坏块 */
    ucFlag = NAND_BAD_BLOCK_FLAG;
    if (FSMC_NAND_WriteSpare(&ucFlag, ulPageNo, BI_OFFSET, 1) == NAND_FAIL)
    {
        /* 如果第1个页标记失败，则在最后一页标记 */
        FSMC_NAND_WriteSpare(&ucFlag, ulPageNo + PAGES_NUM_PER_BLOCK - 1, BI_OFFSET, 1);

    }
}

/*
*********************************************************************************************************
*   函 数 名: NAND_Format
*   功能说明: NAND Flash格式化，擦除所有的数据，重建LUT
*   形    参：无
*   返 回 值: 执行结果:
*               - NAND_OK : 成功; 
*               - NAND_Fail ：失败（一般是坏块数量过多导致）
*********************************************************************************************************
*/
uint8_t NAND_Format(void)
{
    uint16_t i;
    uint16_t usGoodBlockCount = 0;
    uint8_t  status;

    uint8_t  buf[VALID_SPARE_SIZE];

    buf[BI_OFFSET]   = NAND_BAD_BLOCK_FLAG;
    buf[USED_OFFSET] = 0xFF;

    for (i = 0; i < NAND_BLOCK_COUNT; i++)
    {
        s_usLUT[i] = 0xFFFF; // 填充无效值，用于重建LUT后，判断LUT是否合理
    }
    s_currentRemapBlock = MAX_TABLE_LOG_ADDR; // 当前映射的块数 

    /* 擦除每个块 */
    for (i = MAX_DATA_LOG_ADDR; i < MAX_SWAP_LOG_ADDR; i++) // 先擦除记录区信息
    {
        if (!NAND_IsBadBlock(i)) // 如果是好块才能擦除
        {
            status = FSMC_NAND_EraseBlock(i);
            if (status != NAND_READY) // 重试一次
            {
                status = FSMC_NAND_EraseBlock(i);
            }
            if (status != NAND_READY)
            {
                NAND_MarkBadBlock(i); // 标记为坏块，但不映射
            }
        }
    }

    for (i = 0; i < MAX_DATA_LOG_ADDR; i++) // 擦除数据区信息
    {
        if (!NAND_IsBadBlock(i)) // 如果是好块才擦除
        {
            status = FSMC_NAND_EraseBlock(i);
            if (status != NAND_READY)
            {
                status = FSMC_NAND_EraseBlock(i);
            }

            if (status == NAND_READY) // 擦除成功
            {
                usGoodBlockCount++;
                FSMC_NAND_WriteSpare((uint8_t *)&i, i * PAGES_NUM_PER_BLOCK, LBN0_OFFSET, 2);
                FSMC_NAND_WriteSpare((uint8_t *)&i, (i+1) * PAGES_NUM_PER_BLOCK - 1, LBN0_OFFSET, 2);//写入结尾处的lbo
                s_usLUT[i] = i; // 更新逻辑表
                continue;
            }
        }

        /* 进行坏块映射 */
        while (NAND_IsBadBlock(s_currentRemapBlock)) // 判断映射块是否可用
        {
            s_currentRemapBlock++;

            if (s_currentRemapBlock >= MAX_REMAP_LOG_ADDR)  // 坏块映射块用光了
            {
                return NAND_FAIL;
            }
        }

        buf[LBN0_OFFSET] = (u8)s_currentRemapBlock;
        buf[LBN1_OFFSET] = (u8)(s_currentRemapBlock>>8);
        // 信息记录在第一页和最后一页
        FSMC_NAND_WriteSpare(buf, i * PAGES_NUM_PER_BLOCK, BI_OFFSET, VALID_SPARE_SIZE);
        FSMC_NAND_WriteSpare(buf, (i+1) * PAGES_NUM_PER_BLOCK - 1, BI_OFFSET, VALID_SPARE_SIZE);//写入结尾处的lbo

        s_usLUT[i] = s_currentRemapBlock;// 更新逻辑表
        s_currentRemapBlock++;
    }

    /* 如果好块的数量少于MIN_BLOCK_NUM，则NAND Flash报废 */
    if (usGoodBlockCount < MIN_BLOCK_NUM)
    {
        return NAND_FAIL;
    }

    NAND_SaveLUTTable(); //重新保存LUT表

    return NAND_OK;
}

/*
*********************************************************************************************************
*   函 数 名: NAND_SaveLUTTable
*   功能说明: 保存逻辑映射表
*   形    参：无
*   返 回 值: 无
*********************************************************************************************************
*/
static void NAND_SaveLUTTable(void)
{
    uint16_t i, j, k = 0, t = 0;
    uint32_t currentWritePage;
    uint16_t remainSize;
    /* 写前擦除 */
    for (i = 0; i < BAD_TABLE_NUM; i++)
    {
        if (FSMC_NAND_EraseBlock(i+MAX_DATA_LOG_ADDR) != NAND_READY)
        {
            NAND_MarkBadBlock(i+MAX_DATA_LOG_ADDR);
        }
        else   // 经测试发现扩展区必须擦除后立即写入，否则写不进去，待研究
        {
            currentWritePage = (i+MAX_DATA_LOG_ADDR) * PAGES_NUM_PER_BLOCK;
            FSMC_NAND_WriteSpare((uint8_t *)&s_currentRemapBlock, currentWritePage, LBN0_OFFSET, 2); // 记录已使用的坏块映射
        }
    }

    remainSize = NAND_BLOCK_COUNT << 1;
    while (1)
    {
        for (j = 0; j < TABLE_PACKET_SIZE; k++)
        {
            s_ucTempBuf[j]   = (u8)s_usLUT[k];
            s_ucTempBuf[j+1] = (u8)(s_usLUT[k] >> 8);
            j += 2;
        }

        for (i = 0; i < BAD_TABLE_NUM; i++)
        {
            currentWritePage = (i+MAX_DATA_LOG_ADDR) * PAGES_NUM_PER_BLOCK;
            FSMC_NAND_WriteData(s_ucTempBuf, currentWritePage + t, 0, TABLE_PACKET_SIZE);
        }
        remainSize -= TABLE_PACKET_SIZE;
        t++;

        if (remainSize < TABLE_PACKET_SIZE)    // 不足2048
        {
            if (remainSize)                    // 仍有剩余字节
            {
                for (j = 0; j < remainSize; k++)
                {
                    s_ucTempBuf[j]   = (u8)s_usLUT[k];
                    s_ucTempBuf[j+1] = (u8)(s_usLUT[k] >> 8);
                    j += 2;
                }

                for (i = 0; i < BAD_TABLE_NUM; i++)
                {
                    currentWritePage = (i+MAX_DATA_LOG_ADDR) * PAGES_NUM_PER_BLOCK;
                    FSMC_NAND_WriteData(s_ucTempBuf, currentWritePage + t, 0, remainSize);
                }
                remainSize = 0;
            }
            
            break;
       }
    }
}

/*
*********************************************************************************************************
*   函 数 名: NAND_FormatCapacity
*   功能说明: NAND Flash格式化后的有效容量
*   形    参：无
*   返 回 值: 容量大小
*********************************************************************************************************
*/
uint32_t NAND_FormatCapacity(void)
{
    /* 计算用于存储数据的数据块个数 */
    return (MAX_DATA_LOG_ADDR * NAND_BLOCK_SIZE);
}

/*
*********************************************************************************************************
*   函 数 名: NAND_CheckFileSystem
*   功能说明: 掉电保护机制一部分,在初始化nand后，检测所有交换区，如果内部有有效数据代表意外掉电
*               启动修复，如果没有有效数据，则将交换区擦除。
*               在修复过程中如果源块或交换块成为坏块，该修复过程失败，文件系统可能损坏。
*   形    参：无
*   返 回 值: 执行结果：
*              - NAND_OK   操作成功
*              - NAND_FAIL 操作失败
*********************************************************************************************************
*/
uint8_t NAND_CheckFileSystem(void)
{
    int i;
    uint8_t tempBuf[2];
    uint16_t logicNum;
    uint32_t tarBlock;
    uint32_t ulPageNo;

    logicNum = 0xFFFF;
    /* 检测所有交换块 */
    for (i = MAX_REMAP_LOG_ADDR; i < MAX_SWAP_LOG_ADDR; i++)
    {
        if (NAND_IsBlockDataValid(i) == NAND_OK) //在交换块内发现有效数据，需要回写
        {
            /* 读取交换块内有效数据对应的逻辑号 */
            ulPageNo = i * PAGES_NUM_PER_BLOCK; // 计算该块第1个页的页号 
            if (FSMC_NAND_ReadSpare(tempBuf, ulPageNo, LBN0_OFFSET, 2) == NAND_OK) // 读取逻辑号
            {
                logicNum = tempBuf[0] + ((uint16_t)tempBuf[1] << 8);
                if(logicNum != 0xFFFF)
                {   
                    /* 获取物理扇区号 */
                    tarBlock = (uint32_t)NAND_LBNtoPBN(logicNum);
                }
                else
                {
                    return NAND_FAIL;
                }
            }
            else
            {
                return NAND_FAIL;
            }

            /* 擦除源数据块 */
            if (FSMC_NAND_EraseBlock(tarBlock) != NAND_READY) 
            {
                NAND_MarkBadBlock(tarBlock); 
                return NAND_FAIL;
            }

            /* 数据回写：将交换的有效数据写回到源数据块中 */
            if (NAND_WriteToSrcBlock(i, tarBlock) == NAND_OK)
            {
                if (NAND_MarkErasedSwapBlock(i))//擦除之前想置位，防止此时断电
                {
                    /* 擦除交换区 */
                    if (FSMC_NAND_EraseBlock(i) != NAND_READY) 
                    {
                        NAND_MarkBadBlock(i); 
                        return NAND_FAIL;
                    }
                    /* 执行到此处已经恢复完成，但没退出，用于检查剩余交换块 */
                }
                else
                {
                    NAND_MarkBadBlock(i);
                }
            }
            else
            {
                return NAND_FAIL;
            }

        }
    }

    return NAND_OK;
}

/*
*********************************************************************************************************
*   函 数 名: NAND_IsBlockDataValid
*   功能说明: 掉电保护机制一部分，该函数用于检测交换分区内是否存在有效数据，如果数据无效或无数据，则将该
*               交换块擦除，保证交换块是干净可以的。
*         ps：只有在第一页和最后一页的逻辑号相同且不等于0XFFFF，且这两页数据能经过ecc验证，
*               其他情况均认为数据无效，擦除该块。
*             
*   形    参：_ulBlockNo：要检查的块号
*   返 回 值: 执行结果  
*               - NAND_OK   交换块内数据有效
*               - NAND_FAIL 交换块内数据无效
*********************************************************************************************************
*/
static uint8_t NAND_IsBlockDataValid(uint32_t _ulBlockNo)
{
    uint16_t logicNum1,logicNum2;
    uint8_t tempBuf[2];
    uint32_t ulPageNo;

    logicNum1 = 0xFFFF;
    if (!NAND_IsBadBlock(_ulBlockNo)) // 如果坏块管理表不是坏块
    {
        /* 如果擦除标志被置位，则数据无效，可直接擦除 */
        if (NAND_IsErasedSwapBlock(_ulBlockNo))
        {
            if (FSMC_NAND_EraseBlock(_ulBlockNo) != NAND_READY) 
            {
                NAND_MarkBadBlock(_ulBlockNo); 
            }
            return NAND_FAIL;
        }

    /* 检测交换块数据和逻辑号是否有效,所有无效状态均擦除块后返回 */
        
        /* 检测第1页内数据和逻辑号是否有效 */
        ulPageNo = _ulBlockNo * PAGES_NUM_PER_BLOCK; // 计算该块第1个页的页号 
        if (FSMC_NAND_ReadSpare(tempBuf, ulPageNo, LBN0_OFFSET, 2) == NAND_OK) // 读取逻辑号
        {   
            logicNum1 = tempBuf[0] + ((uint16_t)tempBuf[1] << 8);
            if (logicNum1 != 0xFFFF)  
            {   /* 该逻辑号有效 */

                /* 检测第1页的数据有效 */
                if (FSMC_NAND_ReadData(s_ucTempBuf, ulPageNo, 0, NAND_PAGE_SIZE) == NAND_OK)
                {
                    /* 此处没有任何操作，继续检测该块内最后1页 */
                }
                else
                {
                    /* 擦除该交换块 */
                    if (FSMC_NAND_EraseBlock(_ulBlockNo) != NAND_READY) 
                    {
                        NAND_MarkBadBlock(_ulBlockNo); 
                    }
                    return NAND_FAIL;
                }
            }
            else
            {
                /* 擦除该交换块 */
                if (FSMC_NAND_EraseBlock(_ulBlockNo) != NAND_READY) 
                {
                    NAND_MarkBadBlock(_ulBlockNo); 
                }
                return NAND_FAIL; //读出的逻辑号位FFFF，证明数据未写入到此就掉电了
            }
        }
        else
        {
            return NAND_FAIL; //读该页数据失败返回
        }
        
        /* 执行到此处，证明第一页的数据和逻辑号有效 */

        /* 检测最后1页内数据和逻辑号是否有效 */
        logicNum2 = 0xFFFF;
        ulPageNo = (_ulBlockNo + 1) * PAGES_NUM_PER_BLOCK - 1; // 计算该块最后1个页的页号 
        if (FSMC_NAND_ReadSpare(tempBuf, ulPageNo, LBN0_OFFSET, 2) == NAND_OK) // 读取逻辑号
        {   
            logicNum2 = tempBuf[0] + ((uint16_t)tempBuf[1] << 8);
            if (logicNum2 != 0xFFFF) 
            {   /* 该逻辑号有效 */
                
                /* 检测最后1页的数据有效 */
                if (FSMC_NAND_ReadData(s_ucTempBuf, ulPageNo, 0, NAND_PAGE_SIZE) == NAND_OK)
                {
                    if (logicNum1 == logicNum2)
                    {
                        return NAND_OK;
                    }
                    else
                    {
                        if (FSMC_NAND_EraseBlock(_ulBlockNo) != NAND_READY) 
                        {
                            NAND_MarkBadBlock(_ulBlockNo); 
                        }
                        return NAND_FAIL; //两个逻辑号不同，返回
                    }
                }
                else
                {
                    /* 擦除该交换块 */
                    if (FSMC_NAND_EraseBlock(_ulBlockNo) != NAND_READY) 
                    {
                        NAND_MarkBadBlock(_ulBlockNo); 
                    }
                    return NAND_FAIL; //数据无效，返回
                }
            }
            else
            {
                /* 擦除该交换块 */
                if (FSMC_NAND_EraseBlock(_ulBlockNo) != NAND_READY) 
                {
                    NAND_MarkBadBlock(_ulBlockNo); 
                }
                return NAND_FAIL; //读出的逻辑号位FFFF，证明数据未写入到此就掉电了
            }
        }
        else
        {
            return NAND_FAIL; //读该页数据失败返回
        }
    }

    return NAND_FAIL; //坏块直接返回
}

/*
*********************************************************************************************************
*   函 数 名: NAND_WriteToSrcBlock
*   功能说明: 将TarBlockNo内的数据写回到SrcBlockNo块
*   形    参：  
*               _SrcBlockNo  ：源数据块地址
*               _TarBlockNo   ：交换数据块地址
*   返 回 值: 执行结果：
*               - NAND_OK   表示成功
*               - NAND_FAIL 表示失败
*********************************************************************************************************
*/
static uint8_t NAND_WriteToSrcBlock(uint32_t _SrcBlockNo, uint32_t _TarBlockNo)
{
    int i;
    uint32_t _ulSrcPageNo, _ulTarPageNo;
    _ulSrcPageNo = _SrcBlockNo * PAGES_NUM_PER_BLOCK; // 计算源数据块第1个页的页号 
    _ulTarPageNo = _TarBlockNo * PAGES_NUM_PER_BLOCK; // 计算交换数据块第1个页的页号 

    for (i = 0; i < PAGES_NUM_PER_BLOCK; i++)
    {
        if(FSMC_NAND_PageCopyBack(_ulSrcPageNo, _ulTarPageNo) == NAND_OK)
        {
            _ulSrcPageNo++;
            _ulTarPageNo++;
        }
        else
        {
            return NAND_FAIL;
        }
    }

    return NAND_OK;
}

/***************************** 华宁电子 (END OF FILE) *********************************/

#endif
