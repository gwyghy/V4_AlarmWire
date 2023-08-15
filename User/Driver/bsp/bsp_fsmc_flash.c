/*
*********************************************************************************************************
*
*   ģ������ : FSMC����Flash����ģ��
*   �ļ����� : bsp_fsmc_flash.c
*   ��    �� : V1.5
*   ˵    �� : �ṩFSMC����NAND Flash (HY27UF081G2A�� 8bit 128K�ֽ� ��ҳ)�Ķ�д���
*
*   �޸ļ�¼ :
*       �汾��  ����        ����     ˵��
*       V1.4    2013-08-20  ����   ��ʽ����
*       V1.5    2014-01-20  ��� ���Ӳ��ECCУ��
*
*   Copyright (C), 2013-2014, ��������
*
*   ���� HY27UF081G2A
*                 Bit7 Bit6 Bit5 Bit4 Bit3 Bit2 Bit1 Bit0
*       ��1�ֽڣ� A7   A6   A5   A4   A3   A2   A1   A0     (AddrInPage ��bit7 - bit0)
*       ��2�ֽڣ� 0    0    0    0    A11  A10  A9   A8     (AddrInPage ��bit11 - bit8, ��4bit������0)
*       ��3�ֽڣ� A19  A18  A17  A16  A15  A14  A13  A12    ҳ�ŵĵͰ�λ
*       ��4�ֽڣ� A27  A26  A25  A24  A23  A22  A21  A20    ҳ�ŵĸ߰�λ
*
*
*       �е�ַ: A0 ~ A13  14λ����ҳ�ڵ�ַ�����ɱ�ʾ16k      ֻ�õ�8832
*       �е�ַ:A14 ~ A33  20λ����ҳ���ַ�����ɱ�ʾ8192��   ֻ�õ�4152
*
*   ���ݶ�дʱʹ��STMӲ��ECCУ�飬��֤���ݵİ�ȫ�ԡ��ڶ�ȡʱʹ��ECC��1λ�������������ݽ��м��������
*   ������ʹ��ÿ512�ֽڼ���һ��ECC���ұ����ڱ���������Ϊÿҳ���ݴ�СΪ2048�ֽڣ�ÿ������ʹ��512�ֽڣ�
*   ����һ����Ҫ����4��������512�ֽڼ����ECCʹ��24bit������ÿҳ��Ҫ12�ֽڱ�������ECC���ݡ�
*   ռ�õ�ַλ��Ϊ�ӱ�������16�ֽڿ�ʼ������Ϊ��һ������ECC���ڶ�������ECC������
*   ps����Ϊ����������Ҫ����һЩ����������Ϣ����nand_flash���棬��ע�⻮��ʱҪ��֤���ص���
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
*   �� �� ��: FSMC_NAND_Init
*   ����˵��: ����FSMC��GPIO����NAND Flash�ӿڡ�������������ڶ�дnand flashǰ������һ�Ρ�
*   ��    �Σ���
*   �� �� ֵ: ��
*********************************************************************************************************
*/
void FSMC_NAND_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    FSMC_NANDInitTypeDef  FSMC_NANDInitStructure;
    FSMC_NAND_PCCARDTimingInitTypeDef  p;

    /*--NAND Flash GPIOs ����  ------
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

        PG6/FSMC_INT2   (�������ò�ѯ��ʽ��æ���˿�����Ϊ��ͨGPIO���빦��ʹ��)
    */

    /* ʹ�� GPIO ʱ�� */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD | RCC_AHB1Periph_GPIOE | RCC_AHB1Periph_GPIOG, ENABLE);
    /* ʹ�� FSMC ʱ�� */
    RCC_AHB3PeriphClockCmd(RCC_AHB3Periph_FSMC, ENABLE);
    /* ����GPIOD */
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
    /* ����GPIOE */
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

    /* INT2 ��������Ϊ�ڲ��������룬����æ�ź� */
    GPIO_InitStructure.GPIO_Pin   = PIN_FSMC_INT2;                          //����GPIOG, PG6��Ϊæ��Ϣ������Ϊ����
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
    GPIO_Init(GPIOG, &GPIO_InitStructure);

    /* ���� FSMC ʱ�� */
    p.FSMC_SetupTime = 0x1;
    p.FSMC_WaitSetupTime = 0x3;
    p.FSMC_HoldSetupTime = 0x2;
    p.FSMC_HiZSetupTime  = 0x1;
    
    FSMC_NANDInitStructure.FSMC_Bank = FSMC_Bank2_NAND;                     // ����FSMC NAND BANK �� 
    FSMC_NANDInitStructure.FSMC_Waitfeature = FSMC_Waitfeature_Disable;     // ����ȴ�ʱ��ʹ�� 
    FSMC_NANDInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_8b;  // ���ݿ�� 8bit 

    FSMC_NANDInitStructure.FSMC_ECC = FSMC_ECC_Disable;                     // ECC������;�������ʹ��,��ʼ������ر� 
    FSMC_NANDInitStructure.FSMC_ECCPageSize = FSMC_ECCPageSize_512Bytes;    // ECC ҳ���С��ʹ��ÿ512�ֽڶ�ȡһ�� 

    FSMC_NANDInitStructure.FSMC_TCLRSetupTime = 0x01;                       // CLE�ͺ�RE��֮����ӳ٣�HCLK������ 
    FSMC_NANDInitStructure.FSMC_TARSetupTime  = 0x01;                       // ALE�ͺ�RE��֮����ӳ٣�HCLK������ 
    FSMC_NANDInitStructure.FSMC_CommonSpaceTimingStruct    = &p;            // FSMC Common Space Timing 
    FSMC_NANDInitStructure.FSMC_AttributeSpaceTimingStruct = &p;            // FSMC Attribute Space Timing 

    FSMC_NANDInit(&FSMC_NANDInitStructure);

    /* FSMC NAND Bank ʹ�� */
    FSMC_NANDCmd(FSMC_Bank2_NAND, ENABLE);
}

/*
*********************************************************************************************************
*   �� �� ��: FSMC_NAND_PageCopyBack
*   ����˵��: ��һҳ���ݸ��Ƶ�����һ��ҳ��Դҳ��Ŀ��ҳ����ͬΪż��ҳ��ͬΪ����ҳ
*   ��    �Σ�- _ulSrcPageNo: Դҳ��
*             - _ulTarPageNo: Ŀ��ҳ��
*   �� �� ֵ: ִ�н����
*               - NAND_FAIL ��ʾʧ��
*               - NAND_OK   ��ʾ�ɹ�
*
*   ˵    ���������ֲ��Ƽ�����ҳ����֮ǰ����У��Դҳ��λУ�飬������ܻ����λ���󣬱�����δʵ��
*
*********************************************************************************************************
*/
uint8_t FSMC_NAND_PageCopyBack(uint32_t _ulSrcPageNo, uint32_t _ulTarPageNo)
{
    __IO uint32_t i;
    __IO uint16_t timeOut = 0;
    
    /* ���Ͳ������� */
    NAND_CMD_AREA = NAND_CMD_COPYBACK_A;

    NAND_ADDR_AREA = 0;
    NAND_ADDR_AREA = 0;
    
    NAND_ADDR_AREA = _ulSrcPageNo;
    NAND_ADDR_AREA = (_ulSrcPageNo & 0xFF00) >> 8;

    NAND_CMD_AREA = NAND_CMD_COPYBACK_B;

    /* ����ȴ���������������쳣, �˴�Ӧ���жϳ�ʱ */
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

    /* ���Ͳ������� */
    NAND_ADDR_AREA = 0;
    NAND_ADDR_AREA = 0;
    
    NAND_ADDR_AREA = _ulTarPageNo;
    NAND_ADDR_AREA = (_ulTarPageNo & 0xFF00) >> 8;

    NAND_CMD_AREA = NAND_CMD_COPYBACK_D;

    /* ������״̬ */
    if (FSMC_NAND_GetStatus() == NAND_READY)
    {
        return NAND_OK;
    }
    return NAND_FAIL;
}

/*
*********************************************************************************************************
*   �� �� ��: FSMC_NAND_PageCopyBackEx
*   ����˵��: ��һҳ���ݸ��Ƶ�����һ��ҳ,������Ŀ��ҳ�еĲ������ݡ�Դҳ��Ŀ��ҳ����ͬΪż��ҳ��ͬΪ����ҳ
*   ��    �Σ�- _ulSrcPageNo:  Դҳ��
*             - _ulTarPageNo:  Ŀ��ҳ��
*             - _usAddrInPage: ҳ��ƫ�Ƶ�ַ��pBuf�����ݽ�д�������ַ��ʼ��Ԫ
*             - _pBuf:   ���ݻ�����
*             - _usSize: ���ݴ�С
*   �� �� ֵ: ִ�н����
*               - NAND_FAIL ��ʾʧ��
*               - NAND_OK   ��ʾ�ɹ�
*
*   ˵    ���������ֲ��Ƽ�����ҳ����֮ǰ����У��Դҳ��λУ�飬������ܻ����λ���󣬱�����δʵ��
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

    memset(s_ucTempBuf, 0xFF, NAND_PAGE_TOTAL_SIZE); //����������Ϊ0XFF�������ʼ������ΪFF��������������
    /* ��ȡ����Դ���޸�Դ���� */
    FSMC_NAND_ReadPage(s_ucTempBuf, _ulSrcPageNo, 0, NAND_PAGE_TOTAL_SIZE);
    for (i = _usAddrInPage; i < (_usAddrInPage+_usSize); i++)
    {
        s_ucTempBuf[i] = *_pBuf++;
    }

    /* ���Ͳ������� */
    NAND_CMD_AREA = NAND_CMD_WRITE0;
     
    NAND_ADDR_AREA = 0;
    NAND_ADDR_AREA = 0;
     
    NAND_ADDR_AREA = _ulTarPageNo;
    NAND_ADDR_AREA = (_ulTarPageNo & 0xFF00) >> 8;

    /* ��ȡһЩ��д��ʱ��Ҫ�Ĳ��� */
    sectorNum = _usAddrInPage / FAT_SECTOR_SIZE;  //sectorNumΪҳ�ж�Ӧ��������
    eccOffset = sectorNum * VALID_ECC_SIZE;  //eccOffsetΪÿ��ECCֵ��buf�е�ƫ����
    sectorCount = _usSize / FAT_SECTOR_SIZE;  //sectorCountΪ��Ҫд�������ĸ���
    
    j = 0;
    /* ��һ���߼�����Ӧ��������д����Ӧλ�� */
    for(i = 0; i < NAND_PAGE_TOTAL_SIZE; i++)
    {
        j++;
        /*�жϿ���eccУ����λ��*/
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
                
                /* ��ȡһ�µ�ַ */
                eccSpareAddr =  ECC_OFFSET_ADDR + eccOffset;
                /* ��д������ʱ�����ECC���ڶ�Ӧ�ı������� */
                /* ��Ϊ��������Ҫ��������д����ʹ�ã����Կ���ʹ�ô˷����޸ı�������ECC */
                s_ucTempBuf[eccSpareAddr] =  (uint8_t)(eccValue & 0xFF);
                s_ucTempBuf[eccSpareAddr + 1] =  (uint8_t)((eccValue >> 8) & 0XFF);
                s_ucTempBuf[eccSpareAddr + 2] =  (uint8_t)((eccValue >> 16) & 0XFF);    
                eccOffset += VALID_ECC_SIZE;
            }
        }
    }
    FSMC_NANDECCCmd(FSMC_Bank2_NAND, DISABLE);
    /* ����ҳд������� */  
    NAND_CMD_AREA = NAND_CMD_WRITE_TRUE1;

    /* �ȴ���� */
    for (i = 0; i < 20; i++);
    while(GPIO_ReadInputDataBit(FSMC_INT2) == 0);
    
    /* ������״̬ */
    if (FSMC_NAND_GetStatus() == NAND_READY)
    {
        return NAND_OK;
    }
    return NAND_FAIL;

}

/*
*********************************************************************************************************
*   �� �� ��: FSMC_NAND_WritePage
*   ����˵��: дһ��������NandFlashָ��ҳ���ָ��λ�ã�д������ݳ��Ȳ�����һҳ�Ĵ�С
*             δ����ECC��ֵ��һ������д�뱸�����ڵ�����
*   ��    �Σ�- _pBuffer:  ָ�������д���ݵĻ�����
*             - _ulPageNo: ҳ�ţ����е�ҳͳһ���룬��ΧΪ��0 - 65535
*             - _usAddrInPage : ҳ�ڵ�ַ����ΧΪ��0-2111
*             - _usByteCount:   д����ֽڸ���
*   �� �� ֵ: ִ�н����
*               - NAND_FAIL ��ʾʧ��
*               - NAND_OK   ��ʾ�ɹ�
*********************************************************************************************************
*/
uint8_t FSMC_NAND_WritePage(uint8_t *_pBuffer, uint32_t _ulPageNo, uint16_t _usAddrInPage, uint16_t _usByteCount)
{
    uint32_t i;

    /* ���Ͳ������� */
    NAND_CMD_AREA = NAND_CMD_WRITE0;

    NAND_ADDR_AREA = _usAddrInPage & 0xFF;
    NAND_ADDR_AREA = (_usAddrInPage & 0xFF00) >> 8;

    NAND_ADDR_AREA = _ulPageNo;
    NAND_ADDR_AREA = (_ulPageNo & 0xFF00) >> 8;

    /* д���� */
    for(i = 0; i < _usByteCount; i++)
    {
        NAND_DATA_AREA = _pBuffer[i];
    }

    NAND_CMD_AREA = NAND_CMD_WRITE_TRUE1;

    /* �ȴ�æ״̬���� */
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
*   �� �� ��: FSMC_NAND_WriteRandom
*   ����˵��: дһ��������NandFlashָ��ҳ���ָ��λ�ã�д������ݳ���Ϊx�������Ĵ�С,�ҵ�ַ��������������
*             д��ECC�Ĺ������Զ��ģ��ú�����������д�����ݺ���Զ��򱸷��ڶ�Ӧλ��д��ECCֵ
*   ��    �Σ�- _pBuffer:  ָ�������д���ݵĻ�����
*             - _ulPageNo: ҳ�ţ����е�ҳͳһ���룬��ΧΪ��0 - 65535
*             - _usAddrInPage : ҳ�ڵ�ַ����ΧΪ��0-2111
*             - _usByteCount:   д����ֽڸ���
*   �� �� ֵ: ִ�н����
*               - NAND_FAIL ��ʾʧ��
*               - NAND_OK   ��ʾ�ɹ�
*********************************************************************************************************
*/
static uint8_t FSMC_NAND_WriteRandom(uint8_t *_pBuffer, uint32_t _ulPageNo, uint16_t _usAddrInPage, uint16_t _usByteCount)
{
    uint32_t i,j;
    uint32_t eccValue;
    uint32_t eccSpareAddr;
    uint8_t eccSpareBuf[TOTAL_ECC_SIZE];
    uint8_t sectorNum, sectorCount, eccOffset;

    /* ���Ͳ������� */
    NAND_CMD_AREA = NAND_CMD_WRITE0;

    NAND_ADDR_AREA = _usAddrInPage & 0xFF;
    NAND_ADDR_AREA = (_usAddrInPage & 0xFF00) >> 8;

    NAND_ADDR_AREA = _ulPageNo;
    NAND_ADDR_AREA = (_ulPageNo & 0xFF00) >> 8;

    sectorNum = _usAddrInPage / FAT_SECTOR_SIZE; //sectorNumΪҳ�ж�Ӧ��������
    eccOffset = sectorNum * VALID_ECC_SIZE; //eccOffsetΪÿ��ECCֵ��buf�е�ƫ����

    FSMC_NANDECCCmd(FSMC_Bank2_NAND, ENABLE);    

    j = 0;
    /* д���� */
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
            /*��д������ʱ�����ECC���ڶ�Ӧ�ı�����buf��*/
            eccSpareBuf[eccOffset] =  (uint8_t)(eccValue & 0xFF);
            eccSpareBuf[eccOffset + 1] =  (uint8_t)((eccValue >> 8) & 0XFF);
            eccSpareBuf[eccOffset + 2] =  (uint8_t)((eccValue >> 16) & 0XFF);    
            eccOffset += VALID_ECC_SIZE;
        }
    }
    FSMC_NANDECCCmd(FSMC_Bank2_NAND, DISABLE);  

    NAND_CMD_AREA = NAND_CMD_WRITE1;

    /* ���»�ȡƫ�Ƶ�ַ */
    eccOffset = sectorNum * VALID_ECC_SIZE;   
    eccSpareAddr =  ECC_OFFSET_ADDR + eccOffset;
    /* ����ҳ����չ������ַ�ĵ�ַ */
    NAND_ADDR_AREA = eccSpareAddr & 0xFF;
    NAND_ADDR_AREA = (eccSpareAddr & 0xFF00) >> 8;

    /* ����Ӧ�����е�ECCֵд�뵽��Ӧ����չ������ */
    sectorCount = _usByteCount / FAT_SECTOR_SIZE; //��ȡд�������ĸ���
    for(i = eccOffset; i < (eccOffset + (sectorCount * VALID_ECC_SIZE)); i++)
    {                                               //��չҳ�ڴ�eccOffset��ʼ��д��(sectorCount * VALID_ECC_SIZE)��ֵ
        NAND_DATA_AREA = eccSpareBuf[i];
    }

    /* ����ҳд������� */  
    NAND_CMD_AREA = NAND_CMD_WRITE_TRUE1;

    /* �ȴ���� */
    for (i = 0; i < 20; i++);                           
	while(GPIO_ReadInputDataBit(FSMC_INT2) == 0);
    
    /* ������״̬ */
    if (FSMC_NAND_GetStatus() == NAND_READY)
    {
        return NAND_OK;
    }
    return NAND_FAIL;
}

/*
*********************************************************************************************************
*   �� �� ��: FSMC_NAND_ReadPage
*   ����˵��: ��NandFlashָ��ҳ���ָ��λ�ö�һ�����ݣ����������ݳ��Ȳ�����һҳ�Ĵ�С
*             δ����ECC��ֵ��һ�����ڶ�ȡ�������ڵ�����
*   ��    �Σ�- _pBuffer:  ָ�������д���ݵĻ�����
*             - _ulPageNo: ҳ�ţ����е�ҳͳһ���룬��ΧΪ��0 - 65535
*             - _usAddrInPage : ҳ�ڵ�ַ����ΧΪ��0-2111
*             - _usByteCount:   �ֽڸ���
*   �� �� ֵ: ִ�н����
*               - NAND_FAIL ��ʾʧ��
*               - NAND_OK   ��ʾ�ɹ�
*********************************************************************************************************
*/
uint8_t FSMC_NAND_ReadPage(uint8_t *_pBuffer, uint32_t _ulPageNo, uint16_t _usAddrInPage, uint16_t _usByteCount)
{
    __IO uint16_t i;

    /* ���Ͳ������� */
    NAND_CMD_AREA = NAND_CMD_AREA_A;

    NAND_ADDR_AREA = _usAddrInPage & 0xFF;
    NAND_ADDR_AREA = (_usAddrInPage & 0xFF00) >> 8;

    NAND_ADDR_AREA = _ulPageNo;
    NAND_ADDR_AREA = (_ulPageNo & 0xFF00) >> 8;
    
    NAND_CMD_AREA = NAND_CMD_AREA_TRUE1;

    /* �ȴ���� */
    for (i = 0; i < 20; i++);
    while(GPIO_ReadInputDataBit(FSMC_INT2) == 0);

    /* �����ݵ�������pBuffer */
    for(i = 0; i < _usByteCount; i++)
    {
        _pBuffer[i] = NAND_DATA_AREA;
    }

    return NAND_OK;
}

/*
*********************************************************************************************************
*   �� �� ��: FSMC_NAND_ReadRandom
*   ����˵��: ��NandFlashָ��ҳ���ָ��λ�ö�һ�����ݣ����������ݳ��Ȳ�����һҳ�Ĵ�С��
*             ��ȡ���ݺ���Զ����������ڵ�ECC�ͼ����ȡ��ECC���бȶԣ���֤���ݵ���Ч��
*             ECC���̿��Խ���1������λ�Ĵ����Ҽ���2������λ�Ĵ���
*   ��    �Σ�- _pBuffer:  ָ�������д���ݵĻ�����
*             - _ulPageNo: ҳ�ţ����е�ҳͳһ���룬��ΧΪ��0 - 65535
*             - _usAddrInPage : ҳ�ڵ�ַ����ΧΪ��0-2111
*             - _usByteCount:   �ֽڸ���
*   �� �� ֵ: ִ�н����
*               - NAND_FAIL ��ʾʧ��
*               - NAND_OK   ��ʾ�ɹ�
*********************************************************************************************************
*/
static uint8_t FSMC_NAND_ReadRandom(uint8_t *_pBuffer, uint32_t _ulPageNo, uint16_t _usAddrInPage, uint16_t _usByteCount)
{
    __IO uint16_t i, j;
    uint32_t gEccValue[4], rEccValue[4];
    uint8_t eccSpareBuf[TOTAL_ECC_SIZE];
    uint8_t sectorNum, sectorCount, eccOffset;

    /* ���Ͳ������� */
    NAND_CMD_AREA = NAND_CMD_AREA_A;

    NAND_ADDR_AREA = _usAddrInPage & 0xFF;
    NAND_ADDR_AREA = (_usAddrInPage & 0xFF00) >> 8;

    NAND_ADDR_AREA = _ulPageNo;
    NAND_ADDR_AREA = (_ulPageNo & 0xFF00) >> 8;
    
    sectorNum = _usAddrInPage / FAT_SECTOR_SIZE; //sectorNumΪҳ�ж�Ӧ��������
    eccOffset = sectorNum * VALID_ECC_SIZE; 

    NAND_CMD_AREA = NAND_CMD_AREA_TRUE1;

    /* �ȴ���� */
    for (i = 0; i < 20; i++);
    while(GPIO_ReadInputDataBit(FSMC_INT2) == 0);
    
    FSMC_NANDECCCmd(FSMC_Bank2_NAND, ENABLE); 

    j = 0;       
    memset(gEccValue, 0, 4);
    /* �����ݵ�������pBuffer */
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
    
    /* ���Ͳ������� */
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

    /* ��ȡһЩ�ڶ�ȡʱ��Ҫ�Ĳ��� */
    sectorNum = _usAddrInPage / FAT_SECTOR_SIZE; //sectorNumΪҳ�ж�Ӧ��������
    eccOffset = sectorNum * VALID_ECC_SIZE; //eccOffsetΪÿ��ECCֵ��buf�е�ƫ����
    sectorCount = _usByteCount / FAT_SECTOR_SIZE; //sectorCountΪ��Ҫд�������ĸ���

    for(i = sectorNum; i < (sectorNum + sectorCount); i++)
    {                                                //��չҳ�ڴ�eccOffset��ʼ��д��(sectorCount * VALID_ECC_SIZE)��ֵ
        rEccValue[i] = (uint32_t)(eccSpareBuf[eccOffset] & 0xFF)| 
                               (uint32_t)((eccSpareBuf[eccOffset+1] << 8) & 0XFF00)|
                               (uint32_t)((eccSpareBuf[eccOffset+2] << 16) & 0XFF0000);
        eccOffset += VALID_ECC_SIZE;
        /* ��ʽ���󣬳�ʼֵΪ0xffffff����ʱ�����ֵΪ0�����Կ϶���һ����ֱ����ɹ� */
        if ((rEccValue[i] == 0xffffff) && (gEccValue[i] == 0))
        {  
            /*�˴����ⲻ���κδ���*/  
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
*   �� �� ��: FSMC_NAND_WriteSpare
*   ����˵��: ��1��PAGE��Spare��д������
*   ��    �Σ�- _pBuffer:  ָ�������д���ݵĻ�����
*             - _ulPageNo: ҳ�ţ����е�ҳͳһ���룬��ΧΪ��0 - 65535
*             - _usAddrInSpare : ҳ�ڱ�������ƫ�Ƶ�ַ����ΧΪ��0-63
*             - _usByteCount:    д����ֽڸ���
*   �� �� ֵ: ִ�н����
*               - NAND_FAIL ��ʾʧ��
*               - NAND_OK   ��ʾ�ɹ�
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
*   �� �� ��: FSMC_NAND_ReadSpare
*   ����˵��: ��1��PAGE��Spare��������
*   ��    �Σ�- _pBuffer:  ָ�������д���ݵĻ�����
*             - _ulPageNo: ҳ�ţ����е�ҳͳһ���룬��ΧΪ��0 - 65535
*             - _usAddrInSpare : ҳ�ڱ�������ƫ�Ƶ�ַ����ΧΪ��0-63
*             - _usByteCount:    д����ֽڸ���
*   �� �� ֵ: ִ�н����
*               - NAND_FAIL ��ʾʧ��
*               - NAND_OK   ��ʾ�ɹ�
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
*   �� �� ��: FSMC_NAND_WriteData
*   ����˵��: ��1��PAGE����������д�����ݣ��Զ�д��ECC
*   ��    �Σ�- _pBuffer:  ָ�������д���ݵĻ�����
*             - _ulPageNo: ҳ�ţ����е�ҳͳһ���룬��ΧΪ��0 - 65535
*             - _usAddrInPage : ҳ����������ƫ�Ƶ�ַ����ΧΪ��0-2047
*             - _usByteCount:   д����ֽڸ���
*   �� �� ֵ: ִ�н����
*               - NAND_FAIL ��ʾʧ��
*               - NAND_OK   ��ʾ�ɹ�
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
*   �� �� ��: FSMC_NAND_ReadData
*   ����˵��: ��1��PAGE�������ݵ����ݣ��Զ�����ECC�ȶ�
*   ��    �Σ�- _pBuffer:  ָ�������д���ݵĻ�����
*             - _ulPageNo: ҳ�ţ����е�ҳͳһ���룬��ΧΪ��0 - 65535
*             - _usAddrInPage : ҳ����������ƫ�Ƶ�ַ����ΧΪ��0-2047
*             - _usByteCount:   д����ֽڸ���
*   �� �� ֵ: ִ�н����
*               - NAND_FAIL ��ʾʧ��
*               - NAND_OK   ��ʾ�ɹ�
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
*   �� �� ��: FSMC_NAND_EraseBlock
*   ����˵��: ����NAND Flashһ���飨block��
*   ��    �Σ�- _ulBlockNo: ��ţ���ΧΪ��0 - 1023
*   �� �� ֵ: NAND����״̬�������¼���ֵ��
*             - NAND_TIMEOUT_ERROR  : ��ʱ����
*             - NAND_READY          : �����ɹ�
*********************************************************************************************************
*/
uint8_t FSMC_NAND_EraseBlock(uint32_t _ulBlockNo)
{
    /* ���Ͳ������� */
    NAND_CMD_AREA = NAND_CMD_ERASE0;

    /* ���ת��Ϊҳ��� */
    _ulBlockNo <<= 6;                                  
    NAND_ADDR_AREA = _ulBlockNo;
    NAND_ADDR_AREA = _ulBlockNo >> 8;

    NAND_CMD_AREA = NAND_CMD_ERASE1;
    return (FSMC_NAND_GetStatus());
}

/*
*********************************************************************************************************
*   �� �� ��: FSMC_NAND_Reset
*   ����˵��: ��λNAND Flash
*   ��    �Σ���
*   �� �� ֵ: ��
*********************************************************************************************************
*/
uint8_t FSMC_NAND_Reset(void)
{
    NAND_CMD_AREA = NAND_CMD_RESET;

    /* ������״̬ */
    if (FSMC_NAND_GetStatus() == NAND_READY)
    {
        return NAND_OK;
    }

    return NAND_FAIL;
}

/*
*********************************************************************************************************
*   �� �� ��: FSMC_NAND_ReadStatus
*   ����˵��: ʹ��Read statuc �����NAND Flash�ڲ�״̬
*   ��    �Σ�- Address: �������Ŀ��������ַ
*   �� �� ֵ: NAND����״̬�������¼���ֵ��
*             - NAND_BUSY:  �ڲ���æ
*             - NAND_READY: �ڲ����У����Խ����²�����
*             - NAND_ERROR: ��ǰ������ִ��ʧ��
*********************************************************************************************************
*/
static uint8_t FSMC_NAND_ReadStatus(void)
{
    uint8_t ucData;
    uint8_t ucStatus = NAND_BUSY;
    
    /* ��״̬���� */
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
*   �� �� ��: FSMC_NAND_GetStatus
*   ����˵��: ��ȡNAND Flash����״̬
*   ��    �Σ�- Address: �������Ŀ��������ַ
*   �� �� ֵ: NAND����״̬�������¼���ֵ��
*             - NAND_TIMEOUT_ERROR  : ��ʱ����
*             - NAND_READY          : �����ɹ�
*********************************************************************************************************
*/
static uint8_t FSMC_NAND_GetStatus(void)
{
    uint32_t ulTimeout = 0x10000;
    uint8_t ucStatus = NAND_READY;

    ucStatus = FSMC_NAND_ReadStatus();

    /* �ȴ�NAND������������ʱ����˳� */
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
*   �� �� ��: FSMC_NAND_ClearECC
*   ����˵��: ���ECCУ��ֵ
*   ��    �Σ�-
*   �� �� ֵ: -
*********************************************************************************************************
*/
void FSMC_NAND_ClearECC(void)
{
    FSMC_NANDECCCmd(FSMC_Bank2_NAND, DISABLE); // �´μ���ǰ�����ECC
    FSMC_NANDECCCmd(FSMC_Bank2_NAND, ENABLE); // ECCʹ��
}

/*
*********************************************************************************************************
*   �� �� ��: FSMC_NAND_ReadECC
*   ����˵��: ��ȡECCУ��ֵ
*   ��    �Σ�-
*   �� �� ֵ: ECCУ��ֵ
*********************************************************************************************************
*/
uint32_t FSMC_NAND_ReadECC(void)
{
    volatile uint32_t ECCValue = 0;
    __IO uint16_t timeOut  = 0;

    while(FSMC_GetFlagStatus(FSMC_Bank2_NAND, FSMC_FLAG_FEMPT) == RESET) // �ж�FIFO״̬��1Ϊ��FlagStatus
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
*   �� �� ��: FSMC_NAND_DataCorrect(������512�ֽڵ�ECCУ��)
*   ����˵��: �ɼ�Ⲣ�޸������еĴ���,����1������λ�Ĵ����Ҽ���2������λ�Ĵ���
*   ��    �Σ�-generatedEcc:���������eccֵ
              -readEcc��ͨ����չ������ȡ������ECCֵ
              -data:����������
*   �� �� ֵ: NAND_OK������û�д��󣬻������������Ѿ�����
*             NAND_ECC_ERR������û�д���������ݴ�����Ǳ�������ECC����
*             NAND_DATA_ERR �����ݴ������޷�����
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
/***************************** �������� (END OF FILE) *********************************/

