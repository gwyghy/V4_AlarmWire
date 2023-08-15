/*
*********************************************************************************************************
*
*   模块名称 : FSMC操作Flash驱动模块
*   文件名称 : bsp_fsmc_flash.c
*   版    本 : V1.5
*   说    明 : 提供FSMC控制NAND Flash (HY27UF081G2A， 8bit 128K字节 大页)的读写命令。
*
*   修改记录 :
*       版本号  日期        作者     说明
*       V1.4    2013-08-20  辛鑫   正式发布
*       V1.5    2014-01-20  马金超 添加硬件ECC校验
*
*   Copyright (C), 2013-2014, 华宁电子
*
*   对于 HY27UF081G2A
*                 Bit7 Bit6 Bit5 Bit4 Bit3 Bit2 Bit1 Bit0
*       第1字节： A7   A6   A5   A4   A3   A2   A1   A0     (AddrInPage 的bit7 - bit0)
*       第2字节： 0    0    0    0    A11  A10  A9   A8     (AddrInPage 的bit11 - bit8, 高4bit必须是0)
*       第3字节： A19  A18  A17  A16  A15  A14  A13  A12    页号的低八位
*       第4字节： A27  A26  A25  A24  A23  A22  A21  A20    页号的高八位
*
*
*       列地址: A0 ~ A13  14位代表页内地址。最大可表示16k      只用到8832
*       行地址:A14 ~ A33  20位代表页块地址，最大可表示8192块   只用到4152
*
*   数据读写时使用STM硬件ECC校验，保证数据的安全性。在读取时使用ECC的1位纠错函数，对数据进行检测或纠正。
*   该驱动使用每512字节计算一个ECC，且保存在备份区，因为每页数据大小为2048字节，每个扇区使用512字节，
*   所以一共需要保存4个扇区，512字节计算的ECC使用24bit，所以每页需要12字节保存所有ECC数据。
*   占用地址位置为从备用区第16字节开始，依次为第一扇区的ECC，第二扇区的ECC。。。
*   ps：因为备份区还需要保存一些其他辅助信息，在nand_flash层面，请注意划分时要保证不重叠。
*
*********************************************************************************************************
*/

#if 0
#include "stm32f4xx.h"
#include "bsp_fsmc_flash.h"
#include "bsp_nand_flash.h"
#include <stdio.h>
#include "string.h"
extern uint8_t s_ucTempBuf[BUFFSIZE];

static uint8_t FSMC_NAND_ReadStatus(void);
static uint8_t FSMC_NAND_GetStatus(void);
static uint8_t FSMC_NAND_WriteRandom(uint8_t *_pBuffer, uint32_t _ulPageNo, uint16_t _usAddrInPage, uint16_t _usByteCount);
static uint8_t FSMC_NAND_ReadRandom(uint8_t *_pBuffer, uint32_t _ulPageNo, uint16_t _usAddrInPage, uint16_t _usByteCount);
static uint8_t FSMC_NAND_DataCorrect(uint32_t generatedEcc, uint32_t readEcc, uint8_t *data);
/*
*********************************************************************************************************
*   函 数 名: FSMC_NAND_Init
*   功能说明: 配置FSMC和GPIO用于NAND Flash接口。这个函数必须在读写nand flash前被调用一次。
*   形    参：无
*   返 回 值: 无
*********************************************************************************************************
*/
void FSMC_NAND_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    FSMC_NANDInitTypeDef  FSMC_NANDInitStructure;
    FSMC_NAND_PCCARDTimingInitTypeDef  p;

    /*--NAND Flash GPIOs 配置  ------
        PD0/FSMC_D2
        PD1/FSMC_D3
        PD4/FSMC_NOE
        PD5/FSMC_NWE
        PD7/FSMC_NCE2
        PD11/FSMC_A16
        PD12/FSMC_A17
        PD14/FSMC_D0
        PD15/FSMC_D1

        PE7/FSMC_D4
        PE8/FSMC_D5
        PE9/FSMC_D6
        PE10/FSMC_D7

        PG6/FSMC_INT2   (本例程用查询方式判忙，此口线作为普通GPIO输入功能使用)
    */

    /* 使能 GPIO 时钟 */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD | RCC_AHB1Periph_GPIOE | RCC_AHB1Periph_GPIOG, ENABLE);
    /* 使能 FSMC 时钟 */
    RCC_AHB3PeriphClockCmd(RCC_AHB3Periph_FSMC, ENABLE);
    /* 配置GPIOD */
    GPIO_PinAFConfig(FSMC_D2,   GPIO_AF_FSMC);
    GPIO_PinAFConfig(FSMC_D3,   GPIO_AF_FSMC);
    GPIO_PinAFConfig(FSMC_NOE,  GPIO_AF_FSMC);
    GPIO_PinAFConfig(FSMC_NWE,  GPIO_AF_FSMC);
    GPIO_PinAFConfig(FSMC_NCE2, GPIO_AF_FSMC);
    GPIO_PinAFConfig(FSMC_CLE,  GPIO_AF_FSMC);
    GPIO_PinAFConfig(FSMC_ALE,  GPIO_AF_FSMC);
    GPIO_PinAFConfig(FSMC_D0,   GPIO_AF_FSMC);
    GPIO_PinAFConfig(FSMC_D1,   GPIO_AF_FSMC);

    GPIO_InitStructure.GPIO_Pin = PIN_FSMC_D2   |  PIN_FSMC_D3 | PIN_FSMC_NOE | PIN_FSMC_NWE |
                                  PIN_FSMC_NCE2 | PIN_FSMC_CLE | PIN_FSMC_ALE |  PIN_FSMC_D0 | PIN_FSMC_D1;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;

    GPIO_Init(GPIOD, &GPIO_InitStructure);
    /* 配置GPIOE */
    GPIO_PinAFConfig(FSMC_D4, GPIO_AF_FSMC);
    GPIO_PinAFConfig(FSMC_D5, GPIO_AF_FSMC);
    GPIO_PinAFConfig(FSMC_D6, GPIO_AF_FSMC);
    GPIO_PinAFConfig(FSMC_D7, GPIO_AF_FSMC);

    GPIO_InitStructure.GPIO_Pin   = PIN_FSMC_D4 | PIN_FSMC_D5 | PIN_FSMC_D6 | PIN_FSMC_D7;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOE, &GPIO_InitStructure);

    /* INT2 引脚配置为内部上来输入，用于忙信号 */
    GPIO_InitStructure.GPIO_Pin   = PIN_FSMC_INT2;                          //配置GPIOG, PG6作为忙信息，配置为输入
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
    GPIO_Init(GPIOG, &GPIO_InitStructure);

    /* 配置 FSMC 时序 */
    p.FSMC_SetupTime = 0x1;
    p.FSMC_WaitSetupTime = 0x3;
    p.FSMC_HoldSetupTime = 0x2;
    p.FSMC_HiZSetupTime  = 0x1;
    
    FSMC_NANDInitStructure.FSMC_Bank = FSMC_Bank2_NAND;                     // 定义FSMC NAND BANK 号 
    FSMC_NANDInitStructure.FSMC_Waitfeature = FSMC_Waitfeature_Disable;     // 插入等待时序使能 
    FSMC_NANDInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_8b;  // 数据宽度 8bit 

    FSMC_NANDInitStructure.FSMC_ECC = FSMC_ECC_Disable;                     // ECC错误检查和纠正功能使能,初始化必须关闭 
    FSMC_NANDInitStructure.FSMC_ECCPageSize = FSMC_ECCPageSize_512Bytes;    // ECC 页面大小，使用每512字节读取一次 

    FSMC_NANDInitStructure.FSMC_TCLRSetupTime = 0x01;                       // CLE低和RE低之间的延迟，HCLK周期数 
    FSMC_NANDInitStructure.FSMC_TARSetupTime  = 0x01;                       // ALE低和RE低之间的延迟，HCLK周期数 
    FSMC_NANDInitStructure.FSMC_CommonSpaceTimingStruct    = &p;            // FSMC Common Space Timing 
    FSMC_NANDInitStructure.FSMC_AttributeSpaceTimingStruct = &p;            // FSMC Attribute Space Timing 

    FSMC_NANDInit(&FSMC_NANDInitStructure);

    /* FSMC NAND Bank 使能 */
    FSMC_NANDCmd(FSMC_Bank2_NAND, ENABLE);
}

/*
*********************************************************************************************************
*   函 数 名: FSMC_NAND_PageCopyBack
*   功能说明: 将一页数据复制到另外一个页。源页和目标页必须同为偶数页或同为奇数页
*   形    参：- _ulSrcPageNo: 源页号
*             - _ulTarPageNo: 目标页号
*   返 回 值: 执行结果：
*               - NAND_FAIL 表示失败
*               - NAND_OK   表示成功
*
*   说    明：数据手册推荐：在页复制之前，先校验源页的位校验，否则可能会积累位错误，本函数未实现
*
*********************************************************************************************************
*/
uint8_t FSMC_NAND_PageCopyBack(uint32_t _ulSrcPageNo, uint32_t _ulTarPageNo)
{
    __IO uint32_t i;
    __IO uint16_t timeOut = 0;
    
    /* 发送操作命令 */
    NAND_CMD_AREA = NAND_CMD_COPYBACK_A;

    NAND_ADDR_AREA = 0;
    NAND_ADDR_AREA = 0;
    
    NAND_ADDR_AREA = _ulSrcPageNo;
    NAND_ADDR_AREA = (_ulSrcPageNo & 0xFF00) >> 8;

    NAND_CMD_AREA = NAND_CMD_COPYBACK_B;

    /* 必须等待，否则读出数据异常, 此处应该判断超时 */
    for (i = 0; i < 20; i++);
    while(GPIO_ReadInputDataBit(FSMC_INT2) == 0)
    {
        timeOut++;
        if (timeOut > WAITTIMEOUT)
        {
            return NAND_FAIL;
        }
    }

    NAND_CMD_AREA = NAND_CMD_COPYBACK_C;

    /* 发送操作命令 */
    NAND_ADDR_AREA = 0;
    NAND_ADDR_AREA = 0;
    
    NAND_ADDR_AREA = _ulTarPageNo;
    NAND_ADDR_AREA = (_ulTarPageNo & 0xFF00) >> 8;

    NAND_CMD_AREA = NAND_CMD_COPYBACK_D;

    /* 检查操作状态 */
    if (FSMC_NAND_GetStatus() == NAND_READY)
    {
        return NAND_OK;
    }
    return NAND_FAIL;
}

/*
*********************************************************************************************************
*   函 数 名: FSMC_NAND_PageCopyBackEx
*   功能说明: 将一页数据复制到另外一个页,并更新目标页中的部分数据。源页和目标页必须同为偶数页或同为奇数页
*   形    参：- _ulSrcPageNo:  源页号
*             - _ulTarPageNo:  目标页号
*             - _usAddrInPage: 页内偏移地址，pBuf的内容将写入这个地址开始单元
*             - _pBuf:   数据缓冲区
*             - _usSize: 数据大小
*   返 回 值: 执行结果：
*               - NAND_FAIL 表示失败
*               - NAND_OK   表示成功
*
*   说    明：数据手册推荐：在页复制之前，先校验源页的位校验，否则可能会积累位错误，本函数未实现
*
*********************************************************************************************************
*/
uint8_t FSMC_NAND_PageCopyBackEx(uint32_t _ulSrcPageNo, uint32_t _ulTarPageNo, uint8_t *_pBuf,
                                 uint16_t _usAddrInPage, uint16_t _usSize)
{
    uint32_t i,j;
    uint32_t eccValue;
    uint32_t eccSpareAddr;
    uint8_t sectorNum, sectorCount, eccOffset;

    memset(s_ucTempBuf, 0xFF, NAND_PAGE_TOTAL_SIZE); //擦除操作后为0XFF，这里初始化必须为FF，否则会出现问题
    /* 读取数据源并修改源数据 */
    FSMC_NAND_ReadPage(s_ucTempBuf, _ulSrcPageNo, 0, NAND_PAGE_TOTAL_SIZE);
    for (i = _usAddrInPage; i < (_usAddrInPage+_usSize); i++)
    {
        s_ucTempBuf[i] = *_pBuf++;
    }

    /* 发送操作命令 */
    NAND_CMD_AREA = NAND_CMD_WRITE0;
     
    NAND_ADDR_AREA = 0;
    NAND_ADDR_AREA = 0;
     
    NAND_ADDR_AREA = _ulTarPageNo;
    NAND_ADDR_AREA = (_ulTarPageNo & 0xFF00) >> 8;

    /* 获取一些在写入时需要的参数 */
    sectorNum = _usAddrInPage / FAT_SECTOR_SIZE;  //sectorNum为页中对应的扇区号
    eccOffset = sectorNum * VALID_ECC_SIZE;  //eccOffset为每个ECC值在buf中的偏移量
    sectorCount = _usSize / FAT_SECTOR_SIZE;  //sectorCount为需要写入扇区的个数
    
    j = 0;
    /* 按一定逻辑将对应扇区数据写到对应位置 */
    for(i = 0; i < NAND_PAGE_TOTAL_SIZE; i++)
    {
        j++;
        /*判断开启ecc校验点的位置*/
        if((_pBuf != NULL) && (i == _usAddrInPage))
        {
            FSMC_NANDECCCmd(FSMC_Bank2_NAND, ENABLE);   
        }

        NAND_DATA_AREA = s_ucTempBuf[i];
        
        if (j == SECTOR_SIZE && sectorCount > 0)
        {
            j = 0;
            if (i >= _usAddrInPage)
            {
                sectorCount--;
                eccValue = (FSMC_NAND_ReadECC() & 0X00FFFFFF);
                FSMC_NAND_ClearECC();
                
                /* 求取一下地址 */
                eccSpareAddr =  ECC_OFFSET_ADDR + eccOffset;
                /* 将写入数据时计算的ECC放在对应的备份区中 */
                /* 因为备份区需要等数据区写完后才使用，所以可以使用此方法修改备份区的ECC */
                s_ucTempBuf[eccSpareAddr] =  (uint8_t)(eccValue & 0xFF);
                s_ucTempBuf[eccSpareAddr + 1] =  (uint8_t)((eccValue >> 8) & 0XFF);
                s_ucTempBuf[eccSpareAddr + 2] =  (uint8_t)((eccValue >> 16) & 0XFF);    
                eccOffset += VALID_ECC_SIZE;
            }
        }
    }
    FSMC_NANDECCCmd(FSMC_Bank2_NAND, DISABLE);
    /* 发送页写命令结束 */  
    NAND_CMD_AREA = NAND_CMD_WRITE_TRUE1;

    /* 等待完成 */
    for (i = 0; i < 20; i++);
    while(GPIO_ReadInputDataBit(FSMC_INT2) == 0);
    
    /* 检测操作状态 */
    if (FSMC_NAND_GetStatus() == NAND_READY)
    {
        return NAND_OK;
    }
    return NAND_FAIL;

}

/*
*********************************************************************************************************
*   函 数 名: FSMC_NAND_WritePage
*   功能说明: 写一组数据至NandFlash指定页面的指定位置，写入的数据长度不大于一页的大小
*             未计算ECC的值，一般用于写入备份区内的数据
*   形    参：- _pBuffer:  指向包含待写数据的缓冲区
*             - _ulPageNo: 页号，所有的页统一编码，范围为：0 - 65535
*             - _usAddrInPage : 页内地址，范围为：0-2111
*             - _usByteCount:   写入的字节个数
*   返 回 值: 执行结果：
*               - NAND_FAIL 表示失败
*               - NAND_OK   表示成功
*********************************************************************************************************
*/
uint8_t FSMC_NAND_WritePage(uint8_t *_pBuffer, uint32_t _ulPageNo, uint16_t _usAddrInPage, uint16_t _usByteCount)
{
    uint32_t i;

    /* 发送操作命令 */
    NAND_CMD_AREA = NAND_CMD_WRITE0;

    NAND_ADDR_AREA = _usAddrInPage & 0xFF;
    NAND_ADDR_AREA = (_usAddrInPage & 0xFF00) >> 8;

    NAND_ADDR_AREA = _ulPageNo;
    NAND_ADDR_AREA = (_ulPageNo & 0xFF00) >> 8;

    /* 写数据 */
    for(i = 0; i < _usByteCount; i++)
    {
        NAND_DATA_AREA = _pBuffer[i];
    }

    NAND_CMD_AREA = NAND_CMD_WRITE_TRUE1;

    /* 等待忙状态结束 */
    for (i = 0; i < 20; i++);
    while(GPIO_ReadInputDataBit(FSMC_INT2) == 0);

    if (FSMC_NAND_GetStatus() == NAND_READY)
    {
        return NAND_OK;
    }
    return NAND_FAIL;
}

/*
*********************************************************************************************************
*   函 数 名: FSMC_NAND_WriteRandom
*   功能说明: 写一组数据至NandFlash指定页面的指定位置，写入的数据长度为x个扇区的大小,且地址是扇区的整数倍
*             写入ECC的过程是自动的，该函数在数据区写入数据后会自动向备份内对应位置写入ECC值
*   形    参：- _pBuffer:  指向包含待写数据的缓冲区
*             - _ulPageNo: 页号，所有的页统一编码，范围为：0 - 65535
*             - _usAddrInPage : 页内地址，范围为：0-2111
*             - _usByteCount:   写入的字节个数
*   返 回 值: 执行结果：
*               - NAND_FAIL 表示失败
*               - NAND_OK   表示成功
*********************************************************************************************************
*/
static uint8_t FSMC_NAND_WriteRandom(uint8_t *_pBuffer, uint32_t _ulPageNo, uint16_t _usAddrInPage, uint16_t _usByteCount)
{
    uint32_t i,j;
    uint32_t eccValue;
    uint32_t eccSpareAddr;
    uint8_t eccSpareBuf[TOTAL_ECC_SIZE];
    uint8_t sectorNum, sectorCount, eccOffset;

    /* 发送操作命令 */
    NAND_CMD_AREA = NAND_CMD_WRITE0;

    NAND_ADDR_AREA = _usAddrInPage & 0xFF;
    NAND_ADDR_AREA = (_usAddrInPage & 0xFF00) >> 8;

    NAND_ADDR_AREA = _ulPageNo;
    NAND_ADDR_AREA = (_ulPageNo & 0xFF00) >> 8;

    sectorNum = _usAddrInPage / FAT_SECTOR_SIZE; //sectorNum为页中对应的扇区号
    eccOffset = sectorNum * VALID_ECC_SIZE; //eccOffset为每个ECC值在buf中的偏移量

    FSMC_NANDECCCmd(FSMC_Bank2_NAND, ENABLE);    

    j = 0;
    /* 写数据 */
    memset(eccSpareBuf, 0XFF, TOTAL_ECC_SIZE);
    for(i = 0; i < _usByteCount; i++)
    {
        j++;
        NAND_DATA_AREA = _pBuffer[i];
        if (j == SECTOR_SIZE)
        {
            j = 0;
            eccValue = (FSMC_NAND_ReadECC() & 0X00FFFFFF);
            FSMC_NAND_ClearECC();
            /*将写入数据时计算的ECC放在对应的备份区buf中*/
            eccSpareBuf[eccOffset] =  (uint8_t)(eccValue & 0xFF);
            eccSpareBuf[eccOffset + 1] =  (uint8_t)((eccValue >> 8) & 0XFF);
            eccSpareBuf[eccOffset + 2] =  (uint8_t)((eccValue >> 16) & 0XFF);    
            eccOffset += VALID_ECC_SIZE;
        }
    }
    FSMC_NANDECCCmd(FSMC_Bank2_NAND, DISABLE);  

    NAND_CMD_AREA = NAND_CMD_WRITE1;

    /* 重新获取偏移地址 */
    eccOffset = sectorNum * VALID_ECC_SIZE;   
    eccSpareAddr =  ECC_OFFSET_ADDR + eccOffset;
    /* 发送页内扩展分区地址的地址 */
    NAND_ADDR_AREA = eccSpareAddr & 0xFF;
    NAND_ADDR_AREA = (eccSpareAddr & 0xFF00) >> 8;

    /* 将对应扇区中的ECC值写入到对应的扩展分区中 */
    sectorCount = _usByteCount / FAT_SECTOR_SIZE; //获取写入扇区的个数
    for(i = eccOffset; i < (eccOffset + (sectorCount * VALID_ECC_SIZE)); i++)
    {                                               //扩展页内从eccOffset开始，写入(sectorCount * VALID_ECC_SIZE)个值
        NAND_DATA_AREA = eccSpareBuf[i];
    }

    /* 发送页写命令结束 */  
    NAND_CMD_AREA = NAND_CMD_WRITE_TRUE1;

    /* 等待完成 */
    for (i = 0; i < 20; i++);                           
	while(GPIO_ReadInputDataBit(FSMC_INT2) == 0);
    
    /* 检测操作状态 */
    if (FSMC_NAND_GetStatus() == NAND_READY)
    {
        return NAND_OK;
    }
    return NAND_FAIL;
}

/*
*********************************************************************************************************
*   函 数 名: FSMC_NAND_ReadPage
*   功能说明: 从NandFlash指定页面的指定位置读一组数据，读出的数据长度不大于一页的大小
*             未计算ECC的值，一般用于读取备份区内的数据
*   形    参：- _pBuffer:  指向包含待写数据的缓冲区
*             - _ulPageNo: 页号，所有的页统一编码，范围为：0 - 65535
*             - _usAddrInPage : 页内地址，范围为：0-2111
*             - _usByteCount:   字节个数
*   返 回 值: 执行结果：
*               - NAND_FAIL 表示失败
*               - NAND_OK   表示成功
*********************************************************************************************************
*/
uint8_t FSMC_NAND_ReadPage(uint8_t *_pBuffer, uint32_t _ulPageNo, uint16_t _usAddrInPage, uint16_t _usByteCount)
{
    __IO uint16_t i;

    /* 发送操作命令 */
    NAND_CMD_AREA = NAND_CMD_AREA_A;

    NAND_ADDR_AREA = _usAddrInPage & 0xFF;
    NAND_ADDR_AREA = (_usAddrInPage & 0xFF00) >> 8;

    NAND_ADDR_AREA = _ulPageNo;
    NAND_ADDR_AREA = (_ulPageNo & 0xFF00) >> 8;
    
    NAND_CMD_AREA = NAND_CMD_AREA_TRUE1;

    /* 等待完成 */
    for (i = 0; i < 20; i++);
    while(GPIO_ReadInputDataBit(FSMC_INT2) == 0);

    /* 读数据到缓冲区pBuffer */
    for(i = 0; i < _usByteCount; i++)
    {
        _pBuffer[i] = NAND_DATA_AREA;
    }

    return NAND_OK;
}

/*
*********************************************************************************************************
*   函 数 名: FSMC_NAND_ReadRandom
*   功能说明: 从NandFlash指定页面的指定位置读一组数据，读出的数据长度不大于一页的大小。
*             读取数据后会自动将备份区内的ECC和计算获取的ECC进行比对，保证数据的有效性
*             ECC过程可以矫正1个比特位的错误并且检测出2个比特位的错误
*   形    参：- _pBuffer:  指向包含待写数据的缓冲区
*             - _ulPageNo: 页号，所有的页统一编码，范围为：0 - 65535
*             - _usAddrInPage : 页内地址，范围为：0-2111
*             - _usByteCount:   字节个数
*   返 回 值: 执行结果：
*               - NAND_FAIL 表示失败
*               - NAND_OK   表示成功
*********************************************************************************************************
*/
static uint8_t FSMC_NAND_ReadRandom(uint8_t *_pBuffer, uint32_t _ulPageNo, uint16_t _usAddrInPage, uint16_t _usByteCount)
{
    __IO uint16_t i, j;
    uint32_t gEccValue[4], rEccValue[4];
    uint8_t eccSpareBuf[TOTAL_ECC_SIZE];
    uint8_t sectorNum, sectorCount, eccOffset;

    /* 发送操作命令 */
    NAND_CMD_AREA = NAND_CMD_AREA_A;

    NAND_ADDR_AREA = _usAddrInPage & 0xFF;
    NAND_ADDR_AREA = (_usAddrInPage & 0xFF00) >> 8;

    NAND_ADDR_AREA = _ulPageNo;
    NAND_ADDR_AREA = (_ulPageNo & 0xFF00) >> 8;
    
    sectorNum = _usAddrInPage / FAT_SECTOR_SIZE; //sectorNum为页中对应的扇区号
    eccOffset = sectorNum * VALID_ECC_SIZE; 

    NAND_CMD_AREA = NAND_CMD_AREA_TRUE1;

    /* 等待完成 */
    for (i = 0; i < 20; i++);
    while(GPIO_ReadInputDataBit(FSMC_INT2) == 0);
    
    FSMC_NANDECCCmd(FSMC_Bank2_NAND, ENABLE); 

    j = 0;       
    memset(gEccValue, 0, 4);
    /* 读数据到缓冲区pBuffer */
    for(i = 0; i < _usByteCount; i++)
    {   
        j++;
        _pBuffer[i] = NAND_DATA_AREA;
        if (j == SECTOR_SIZE)
        {
            j = 0;
            gEccValue[sectorNum] = (FSMC_NAND_ReadECC() & 0X00FFFFFF);
            FSMC_NAND_ClearECC();
            sectorNum++;
        }
    }
    FSMC_NANDECCCmd(FSMC_Bank2_NAND, DISABLE);  
    
    /* 发送操作命令 */
    NAND_CMD_AREA = NAND_CMD_AREA_B;

    NAND_ADDR_AREA = ECC_OFFSET_ADDR & 0xFF;
    NAND_ADDR_AREA = (ECC_OFFSET_ADDR & 0xFF00) >> 8;
  
    NAND_CMD_AREA = NAND_CMD_AREA_TRUE2;

    memset(eccSpareBuf, 0, TOTAL_ECC_SIZE);
    for(i = 0; i < TOTAL_ECC_SIZE; i++)
    {
        eccSpareBuf[i] = NAND_DATA_AREA;
    }

    memset(rEccValue, 0, 4);

    /* 获取一些在读取时需要的参数 */
    sectorNum = _usAddrInPage / FAT_SECTOR_SIZE; //sectorNum为页中对应的扇区号
    eccOffset = sectorNum * VALID_ECC_SIZE; //eccOffset为每个ECC值在buf中的偏移量
    sectorCount = _usByteCount / FAT_SECTOR_SIZE; //sectorCount为需要写入扇区的个数

    for(i = sectorNum; i < (sectorNum + sectorCount); i++)
    {                                                //扩展页内从eccOffset开始，写入(sectorCount * VALID_ECC_SIZE)个值
        rEccValue[i] = (uint32_t)(eccSpareBuf[eccOffset] & 0xFF)| 
                               (uint32_t)((eccSpareBuf[eccOffset+1] << 8) & 0XFF00)|
                               (uint32_t)((eccSpareBuf[eccOffset+2] << 16) & 0XFF0000);
        eccOffset += VALID_ECC_SIZE;
        /* 格式化后，初始值为0xffffff，此时计算的值为0，所以肯定不一样，直接算成功 */
        if ((rEccValue[i] == 0xffffff) && (gEccValue[i] == 0))
        {  
            /*此处故意不做任何处理*/  
        }
        else
        {
            if (FSMC_NAND_DataCorrect(gEccValue[i], rEccValue[i], &_pBuffer[i*512]) == NAND_DATA_ERR)
            {
                return NAND_FAIL;
            }
            
        }
    }
    return NAND_OK;
}

/*
*********************************************************************************************************
*   函 数 名: FSMC_NAND_WriteSpare
*   功能说明: 向1个PAGE的Spare区写入数据
*   形    参：- _pBuffer:  指向包含待写数据的缓冲区
*             - _ulPageNo: 页号，所有的页统一编码，范围为：0 - 65535
*             - _usAddrInSpare : 页内备用区的偏移地址，范围为：0-63
*             - _usByteCount:    写入的字节个数
*   返 回 值: 执行结果：
*               - NAND_FAIL 表示失败
*               - NAND_OK   表示成功
*********************************************************************************************************
*/
uint8_t FSMC_NAND_WriteSpare(uint8_t *_pBuffer, uint32_t _ulPageNo, uint16_t _usAddrInSpare, uint16_t _usByteCount)
{
    if (_usByteCount > NAND_SPARE_AREA_SIZE)
    {
        return NAND_FAIL;
    }
    return FSMC_NAND_WritePage(_pBuffer, _ulPageNo, NAND_PAGE_SIZE + _usAddrInSpare, _usByteCount);
}

/*
*********************************************************************************************************
*   函 数 名: FSMC_NAND_ReadSpare
*   功能说明: 读1个PAGE的Spare区的数据
*   形    参：- _pBuffer:  指向包含待写数据的缓冲区
*             - _ulPageNo: 页号，所有的页统一编码，范围为：0 - 65535
*             - _usAddrInSpare : 页内备用区的偏移地址，范围为：0-63
*             - _usByteCount:    写入的字节个数
*   返 回 值: 执行结果：
*               - NAND_FAIL 表示失败
*               - NAND_OK   表示成功
*********************************************************************************************************
*/
uint8_t FSMC_NAND_ReadSpare(uint8_t *_pBuffer, uint32_t _ulPageNo, uint16_t _usAddrInSpare, uint16_t _usByteCount)
{
    if (_usByteCount > NAND_SPARE_AREA_SIZE)
    {
        return NAND_FAIL;
    }
    return FSMC_NAND_ReadPage(_pBuffer, _ulPageNo, NAND_PAGE_SIZE + _usAddrInSpare, _usByteCount);
}

/*
*********************************************************************************************************
*   函 数 名: FSMC_NAND_WriteData
*   功能说明: 向1个PAGE的主数据区写入数据，自动写入ECC
*   形    参：- _pBuffer:  指向包含待写数据的缓冲区
*             - _ulPageNo: 页号，所有的页统一编码，范围为：0 - 65535
*             - _usAddrInPage : 页内数据区的偏移地址，范围为：0-2047
*             - _usByteCount:   写入的字节个数
*   返 回 值: 执行结果：
*               - NAND_FAIL 表示失败
*               - NAND_OK   表示成功
*********************************************************************************************************
*/
uint8_t FSMC_NAND_WriteData(uint8_t *_pBuffer, uint32_t _ulPageNo, uint16_t _usAddrInPage, uint16_t _usByteCount)
{
    if (_usByteCount > NAND_PAGE_SIZE || _usAddrInPage > NAND_PAGE_SIZE || (_usByteCount % FAT_SECTOR_SIZE) != 0 )
    {
        return NAND_FAIL;
    }
    return FSMC_NAND_WriteRandom(_pBuffer, _ulPageNo, _usAddrInPage, _usByteCount);
}

/*
*********************************************************************************************************
*   函 数 名: FSMC_NAND_ReadData
*   功能说明: 读1个PAGE的主数据的数据，自动进行ECC比对
*   形    参：- _pBuffer:  指向包含待写数据的缓冲区
*             - _ulPageNo: 页号，所有的页统一编码，范围为：0 - 65535
*             - _usAddrInPage : 页内数据区的偏移地址，范围为：0-2047
*             - _usByteCount:   写入的字节个数
*   返 回 值: 执行结果：
*               - NAND_FAIL 表示失败
*               - NAND_OK   表示成功
*********************************************************************************************************
*/
uint8_t FSMC_NAND_ReadData(uint8_t *_pBuffer, uint32_t _ulPageNo, uint16_t _usAddrInPage, uint16_t _usByteCount)
{
    if (_usByteCount > NAND_PAGE_SIZE || _usAddrInPage > NAND_PAGE_SIZE || (_usByteCount % FAT_SECTOR_SIZE) != 0 )
    {
        return NAND_FAIL;
    }
    return FSMC_NAND_ReadRandom(_pBuffer, _ulPageNo, _usAddrInPage, _usByteCount);
}

/*
*********************************************************************************************************
*   函 数 名: FSMC_NAND_EraseBlock
*   功能说明: 擦除NAND Flash一个块（block）
*   形    参：- _ulBlockNo: 块号，范围为：0 - 1023
*   返 回 值: NAND操作状态，有如下几种值：
*             - NAND_TIMEOUT_ERROR  : 超时错误
*             - NAND_READY          : 操作成功
*********************************************************************************************************
*/
uint8_t FSMC_NAND_EraseBlock(uint32_t _ulBlockNo)
{
    /* 发送擦除命令 */
    NAND_CMD_AREA = NAND_CMD_ERASE0;

    /* 块号转换为页编号 */
    _ulBlockNo <<= 6;                                  
    NAND_ADDR_AREA = _ulBlockNo;
    NAND_ADDR_AREA = _ulBlockNo >> 8;

    NAND_CMD_AREA = NAND_CMD_ERASE1;
    return (FSMC_NAND_GetStatus());
}

/*
*********************************************************************************************************
*   函 数 名: FSMC_NAND_Reset
*   功能说明: 复位NAND Flash
*   形    参：无
*   返 回 值: 无
*********************************************************************************************************
*/
uint8_t FSMC_NAND_Reset(void)
{
    NAND_CMD_AREA = NAND_CMD_RESET;

    /* 检查操作状态 */
    if (FSMC_NAND_GetStatus() == NAND_READY)
    {
        return NAND_OK;
    }

    return NAND_FAIL;
}

/*
*********************************************************************************************************
*   函 数 名: FSMC_NAND_ReadStatus
*   功能说明: 使用Read statuc 命令读NAND Flash内部状态
*   形    参：- Address: 被擦除的快内任意地址
*   返 回 值: NAND操作状态，有如下几种值：
*             - NAND_BUSY:  内部正忙
*             - NAND_READY: 内部空闲，可以进行下步操作
*             - NAND_ERROR: 先前的命令执行失败
*********************************************************************************************************
*/
static uint8_t FSMC_NAND_ReadStatus(void)
{
    uint8_t ucData;
    uint8_t ucStatus = NAND_BUSY;
    
    /* 读状态操作 */
    NAND_CMD_AREA = NAND_CMD_STATUS;
    ucData = *(__IO uint8_t *)(Bank_NAND_ADDR);

    if((ucData & NAND_ERROR) == NAND_ERROR)
    {
        ucStatus = NAND_ERROR;
    }
    else if((ucData & NAND_READY) == NAND_READY)
    {
        ucStatus = NAND_READY;
    }
    else
    {
        ucStatus = NAND_BUSY;
    }

    return (ucStatus);
}

/*
*********************************************************************************************************
*   函 数 名: FSMC_NAND_GetStatus
*   功能说明: 获取NAND Flash操作状态
*   形    参：- Address: 被擦除的快内任意地址
*   返 回 值: NAND操作状态，有如下几种值：
*             - NAND_TIMEOUT_ERROR  : 超时错误
*             - NAND_READY          : 操作成功
*********************************************************************************************************
*/
static uint8_t FSMC_NAND_GetStatus(void)
{
    uint32_t ulTimeout = 0x10000;
    uint8_t ucStatus = NAND_READY;

    ucStatus = FSMC_NAND_ReadStatus();

    /* 等待NAND操作结束，超时后会退出 */
    while ((ucStatus != NAND_READY) &&( ulTimeout != 0x00))
    {
        ucStatus = FSMC_NAND_ReadStatus();
        ulTimeout--;
    }

    if(ulTimeout == 0x00)
    {
        ucStatus =  NAND_TIMEOUT_ERROR;
    }

    return (ucStatus);
}

/*
*********************************************************************************************************
*   函 数 名: FSMC_NAND_ClearECC
*   功能说明: 清空ECC校验值
*   形    参：-
*   返 回 值: -
*********************************************************************************************************
*/
void FSMC_NAND_ClearECC(void)
{
    FSMC_NANDECCCmd(FSMC_Bank2_NAND, DISABLE); // 下次计算前先清空ECC
    FSMC_NANDECCCmd(FSMC_Bank2_NAND, ENABLE); // ECC使能
}

/*
*********************************************************************************************************
*   函 数 名: FSMC_NAND_ReadECC
*   功能说明: 获取ECC校验值
*   形    参：-
*   返 回 值: ECC校验值
*********************************************************************************************************
*/
uint32_t FSMC_NAND_ReadECC(void)
{
    volatile uint32_t ECCValue = 0;
    __IO uint16_t timeOut  = 0;

    while(FSMC_GetFlagStatus(FSMC_Bank2_NAND, FSMC_FLAG_FEMPT) == RESET) // 判定FIFO状态，1为空FlagStatus
    {
        timeOut++;
        if (timeOut > WAITTIMEOUT)
        {
            return 0xFFFFFFFF;
        }
    }

    ECCValue = FSMC_GetECC(FSMC_Bank2_NAND);   
    return ECCValue;
}

/*
*********************************************************************************************************
*   函 数 名: FSMC_NAND_DataCorrect(仅用于512字节的ECC校验)
*   功能说明: 可检测并修改数据中的错误,矫正1个比特位的错误并且检测出2个比特位的错误
*   形    参：-generatedEcc:计算出来的ecc值
              -readEcc：通过扩展分区读取出来的ECC值
              -data:读出的数据
*   返 回 值: NAND_OK：数据没有错误，或者数据有误但已经矫正
*             NAND_ECC_ERR：数据没有错误，造成数据错误的是备份区的ECC有误
*             NAND_DATA_ERR ：数据错误且无法矫正
*********************************************************************************************************
*/
static uint8_t FSMC_NAND_DataCorrect(uint32_t generatedEcc, uint32_t readEcc, uint8_t *data)
{
#define ECC_MASK24    0x00FFFFFF // 24 valid ECC parity bits. 
#define ECC_MASK      0x00555555 // 12 ECC parity bits.       

    uint32_t count, bitNum, byteAddr;
    uint32_t mask;
    uint32_t syndrome;
    uint32_t eccP; // 12 even ECC parity bits.
    uint32_t eccPn; // 12 odd ECC parity bits.  
    syndrome = (generatedEcc ^ readEcc) & ECC_MASK24;

    if (syndrome == 0) 
        return (NAND_OK); // No errors in data. 
     

    eccPn = syndrome & ECC_MASK; // Get 12 odd parity bits.  
    eccP  = (syndrome >> 1) & ECC_MASK; // Get 12 even parity bits. 

    if ((eccPn ^ eccP) == ECC_MASK) // 1-bit correctable error ? 
    {
        bitNum = (eccP & 0x01) |
                 ((eccP >> 1) & 0x02) |
                 ((eccP >> 2) & 0x04);

        byteAddr = ((eccP >> 6) & 0x001) |
                   ((eccP >> 7) & 0x002) |
                   ((eccP >> 8) & 0x004) |
                   ((eccP >> 9) & 0x008) |
                   ((eccP >> 10) & 0x010) |
                   ((eccP >> 11) & 0x020) |
                   ((eccP >> 12) & 0x040) |
                   ((eccP >> 13) & 0x080) |
                   ((eccP >> 14) & 0x100) ;

        data[ byteAddr ] ^= 1 << bitNum;
        return NAND_OK;
    }

    /* Count number of one's in the syndrome. */
    count = 0;
    mask  = 0x00800000;
    while (mask)
    {
        if (syndrome & mask)
            count++;
        mask >>= 1;
    }

    if (count == 1) // Error in the ECC itself.
        return NAND_ECC_ERR;

    return NAND_DATA_ERR; // Unable to correct data.

#undef ECC_MASK
#undef ECC_MASK24
}
#endif
/***************************** 华宁电子 (END OF FILE) *********************************/

