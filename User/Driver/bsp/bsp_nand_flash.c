/*
*********************************************************************************************************
*
*   ģ������ : NAND Flash����ģ��
*   �ļ����� : bsp_nand_flash.c
*   ��    �� : V1.4
*   ˵    �� : �ṩNAND Flash (HY27UF081G2A�� 8bit 128K�ֽ� ��ҳ)�ĵײ�ӿں�����
*
*   �޸ļ�¼ :
*       �汾��  ����        ����     ˵��
*       V1.4    2013-08-20  ����   ��ʽ����
*       V1.5    2013-01-20  ��� ȥ��ECC������ECC�����ɵײ��Զ�����
*       V1.6    2013-02-08  ��� ����˵��籣��
*
*   Copyright (C), 2013-2014, ��������
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
    �������������ں���ʿ��HY27UF081G2A������ic������Ҫ�в����޸ġ�
    ��NAND Flash �ṹ���塿
    ��������16x4�ֽڣ�ÿpage 2048�ֽڣ�ÿ512�ֽ�һ��������ÿ��������Ӧ16�ֽڵı�������

    ÿ��PAGE���߼��ṹ��ǰ��512Bx4����������������16Bx4�Ǳ�����
    ��������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������
    �� Main area  ���� Main area  ���� Main area  ����Main area   ���� Spare area ���� Spare area ���� Spare area ����Spare area  ��
    ��            ����            ����            ����            ����            ����            ����            ����            ��
    ��   512B     ����    512B    ����    512B    ����    512B    ����    16B     ����     16B    ����     16B    ����    16B     ��
    ��������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������

    K9F1G08U0A �� HY27UF081G2A �Ǽ��ݵġ�оƬ����ʱ�����̱�֤оƬ�ĵ�1�����Ǻÿ顣����ǻ��飬���ڸÿ�ĵ�1��PAGE�ĵ�1���ֽ�
    ���ߵ�2��PAGE������1��PAGE�����޷����Ϊ0xFFʱ���ĵ�1���ֽ�д���0xFFֵ��������ֵ������ģ����ֱ���ж��Ƿ����0xFF���ɡ�

    Ϊ�˱�����NAND Flash ����ֲFat�ļ�ϵͳ�����Ƕ�64B�ı������������·��䷽��:
    - BI : �����־(Bad Block Identifier)��ÿ��BLOCK�ĵ�1��PAGE���ߵ�2��PAGE�ĵ�1���ֽ�ָʾ�ÿ��Ƿ񻵿顣0xFF��ʾ�ÿ飬����0xFF��ʾ���顣
    - USED : �ÿ�ʹ�ñ�־��0xFF��ʾ���п飻0x00��ʾ��Ч����PAGE��
    - LBN0 LBN1 : �߼����(Logic Block No) ����0��ʼ���롣ֻ��ÿ��BLOCK�ĵ�1��PAGE��Ч������PAGE���ֶι̶�Ϊ0xFF FF
    - ECC : ECCУ�����������ṩ���ò�����迼�ǣ�����ռ�õ�λ�ã���μ������㡣
    - ERASED ���������е�������Ҫ����ǰ�����ñ�־��λ����־��������Ч��0x00��ʾ��������Ч��
    - RSVD : �����ֽڣ�Reserved
    ��ʵ�ֹ��ܡ�
    (1) ʵ���˻������
    (2) ʵ����ĥ��ƽ�⣬ĥ��ƽ�⻹����ȱ�ݣ�Ч�����á���
    (3) ����˵��籣�����ƣ���֤����������������£�NAND�ڵ��ļ�ϵͳ���������
    (4) �����ECCУ�鱣֤���ݵ�׼ȷ�ԣ���У����������ʵ�֣��ò�����迼�ǡ�

    ps����Ȼ����˵��籣����������һ����������޽�ģ��Ǿ����ڵ����޸��ڼ䣬Դ��ͽ��������ô�ʱ��Ϊ���飬��ô��ʱ�޸���ʧ�ܣ�
        �ļ�ϵͳ���ܻ����δ֪�����
    ���飺��app�㽨���ļ��г���ʱ�����Ի�һ���µ��������½������������ӱ�֤�ļ�ϵͳ�İ�ȫ��
*/

/* ȫ�ֱ�������¼�����ECCֵ������������ */
uint8_t s_ucTempBuf[BUFFSIZE]; /* �󻺳�����2112�ֽ�. ���ڶ����Ƚ� */

/* �߼����ӳ������ڼ�¼�˿��Ӧ����Ч�� ���ֵΪ0xFFFF��˵���˿�Ϊ���ݽ�����*/
uint16_t s_usLUT[NAND_BLOCK_COUNT];
static uint16_t s_usValidDataBlockCount; /* ��Ч�����ݿ���� */
static uint16_t s_currentRemapBlock = MAX_TABLE_LOG_ADDR; /* ��ǰӳ��Ŀ���   */

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
*   �� �� ��: bsp_NandFlashinit
*   ����˵��: ��ʼ��FSMCоƬ���������������ӳ������е����⼰�޸���BSP_Init()��������
*   ��    �Σ���
*   �� �� ֵ: NAND_INIT_OK��������
*             NAND_INIT_ERR�����쳣
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
*   �� �� ��: NAND_ReadID
*   ����˵��: ��NAND Flash��ID��ID�洢���β�ָ���Ľṹ������С�
*   ��    �Σ���
*   �� �� ֵ: 32bit��NAND Flash ID
*********************************************************************************************************
*/
uint32_t NAND_ReadID(void)
{
    __IO uint32_t data = 0;
    __IO uint8_t i;

    /* �������� */
    NAND_CMD_AREA  = NAND_CMD_READID;
    NAND_ADDR_AREA = NAND_CMD_READID_TRUE1;

    /* ˳���ȡNAND Flash��ID */
    data = *(__IO uint32_t *)(Bank_NAND_ADDR | DATA_AREA);
    
    /* ��С��ת�� */
    data =  ((data << 24) & 0xFF000000) |
            ((data << 8 ) & 0x00FF0000) |
            ((data >> 8 ) & 0x0000FF00) |
            ((data >> 24) & 0x000000FF) ;

    return data;
}

/*
*********************************************************************************************************
*   �� �� ��: NAND_Init
*   ����˵��: ��ʼ��NAND Flash�ӿ�
*   ��    �Σ���
*   �� �� ֵ: ִ�н����
*               - NAND_FAIL ��ʾʧ��
*               - NAND_OK   ��ʾ�ɹ�
*********************************************************************************************************
*/
uint8_t NAND_Init(void)
{
    uint8_t status;

    FSMC_NAND_Init(); // ����FSMC��GPIO����NAND Flash�ӿ�
    FSMC_NAND_Reset(); // ͨ����λ���λNAND Flash����״̬

    status = NAND_BuildLUT();// ���������� LUT = Look up table 
    return status;
}

uint16_t NAND_GetcurrentMap(void)
{
    return s_currentRemapBlock;
}

/*
*********************************************************************************************************
*   �� �� ��: NAND_WriteToNewBlock
*   ����˵��: ���ɿ�����ݸ��Ƶ��¿飬�����µ�����д������¿�
*   ��    �Σ� _pWriteBuf   ��д���ݻ�����
*               _SectorNo    ����ʼ������
*               _SectorSize  ���������ȣ�������512�ֽڵ�������
*               _SectorCount ����������
*   �� �� ֵ: ִ�н����
*               - NAND_FAIL ��ʾʧ��
*               - NAND_OK   ��ʾ�ɹ�
*********************************************************************************************************
*/
static uint8_t NAND_WriteToNewBlock(uint8_t *_pWriteBuf, uint32_t _SectorNo, uint16_t _SectorSize, uint32_t _SectorCount)
{
    uint16_t n, i;
    uint32_t usLBN; // �߼���� 
    uint8_t LBNbuf[2];
    uint16_t usAddrInPage; // ҳ��ƫ�Ƶ�ַ 
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

    /* �����߼������ż����߼���� */
    usLBN = _SectorNo / SECTORS_NUM_PER_BLOCK;
    ulSrcBlock = NAND_LBNtoPBN(usLBN);// ��ѯLUT���������� 
    if (ulSrcBlock >= NAND_BLOCK_COUNT)
    {
        /* û�и�ʽ����usPBN = 0xFFFF */
        return NAND_FAIL;
    }

    ulTemp = (_SectorNo % SECTORS_NUM_PER_BLOCK) * _SectorSize; // �������ƫ�Ƶ�ַ 
    usInitPageNo = ulTemp / NAND_PAGE_SIZE; // �ڿ�����ʼҳ�� 

    usAddrInPage = ulTemp % NAND_PAGE_SIZE; // ����ҳ��ƫ�Ƶ�ַ 
    usEndPageNo = ((_SectorNo % SECTORS_NUM_PER_BLOCK) + _SectorCount) / SECTORS_NUM_PER_PAGE; //�ڿ��ڽ���ҳ�� 

    /* �ҵ��ÿ��Ӧ���߼��� */
    memset(LBNbuf, 0xFF, 2);
    if(FSMC_NAND_ReadSpare(LBNbuf, ulSrcBlock * PAGES_NUM_PER_BLOCK, LBN0_OFFSET, 2) == NAND_FAIL)
    {
        return NAND_FAIL;
    }
    if (LBNbuf[0] == 0xFF && LBNbuf[1] == 0xFF)
    {
       return NAND_FAIL;
    }

    /* ����ѭ����Ŀ���Ǵ���Ŀ���Ϊ�������� */
    for (n = 0; n < REMAP_BLOCK_NUM; n++)
    {
        usNewBlock = NAND_FindFreeBlock(); // ��Ѱһ�����ÿ�
        if (usNewBlock >= NAND_BLOCK_COUNT)
        {
            return NAND_FAIL; // ���ҿ��п�ʧ�� 
        }
        
        usPBuf = _pWriteBuf; // ָ��д����
        remainSectorNum = (NAND_PAGE_SIZE - usAddrInPage) / _SectorSize; //����ҳ��ʣ�������� 
        /* ʹ��NAND Flash �ṩ����ҳCopy-Back���ܣ�����֮ǰ��ҳ */
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
            NAND_MarkBadBlock(usNewBlock); // ���¿���Ϊ����
            continue; // ����Ѱ���¿鿽��һ��
        }

        if (remainSectorNum < _SectorCount) // �ڵ�ǰҳд����
        {
            /* ��д����ǰҳ */
            if (FSMC_NAND_PageCopyBackEx(ulSrcBlock * PAGES_NUM_PER_BLOCK + i, usNewBlock * PAGES_NUM_PER_BLOCK + i,
                                         usPBuf, usAddrInPage, remainSectorNum*_SectorSize) == NAND_FAIL)
            {
                NAND_MarkBadBlock(usNewBlock); // ���¿���Ϊ����
                continue; // ����Ѱ���¿鿽��һ��
            }
            usPBuf += remainSectorNum*_SectorSize;
            remainSectorNum = _SectorCount - remainSectorNum;
            i++;

            /* ��ʼд��ҳ */
            for (; i < usEndPageNo; i++)
            {
                /* ���д��������ڵ�ǰҳ������Ҫʹ�ô�������ݵ�Copy-Back���� */
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
                NAND_MarkBadBlock(usNewBlock); // ���¿���Ϊ����
                continue; // ����Ѱ���¿鿽��һ��
            }

            /* д���һҳ */
            if (i == usEndPageNo) // д�����һҳ
            {
                if (remainSectorNum) // 2013��9��4���޸Ĵ˴�
                {
                    /* ���д��������ڵ�ǰҳ������Ҫʹ�ô�������ݵ�Copy-Back���� */
                    if (FSMC_NAND_PageCopyBackEx(ulSrcBlock * PAGES_NUM_PER_BLOCK + i, usNewBlock * PAGES_NUM_PER_BLOCK + i,
                                                 usPBuf, 0, remainSectorNum*_SectorSize) == NAND_FAIL)
                    {
                        NAND_MarkBadBlock(usNewBlock); // ���¿���Ϊ����
                        continue; // ����Ѱ���¿鿽��һ��
                    }
                    i++;
                }
            }
        }
        else
        {
            /* ���д��������ڵ�ǰҳ������Ҫʹ�ô�������ݵ�Copy-Back���� */
            if (FSMC_NAND_PageCopyBackEx(ulSrcBlock * PAGES_NUM_PER_BLOCK + i, usNewBlock * PAGES_NUM_PER_BLOCK + i,
                                         usPBuf, usAddrInPage, _SectorSize*_SectorCount) == NAND_FAIL)
            {
                NAND_MarkBadBlock(usNewBlock); // ���¿���Ϊ����
                continue; // ����Ѱ���¿鿽��һ��
            }
            i++;
        }

        /* ʹ��page-copy���ܣ��������ҳȫ������*/
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
            NAND_MarkBadBlock(usNewBlock); // ���¿���Ϊ����
            continue; // ����Ѱ���¿鿽��һ��
        }
        break;
    }

    if (n == REMAP_BLOCK_NUM) // û��ӳ��������
    {
        return NAND_FAIL;
    }
    /* Ŀ�����³ɹ� */
    else
    {

        /* �����һҳ���߼��ű�ע�ϣ����ڴ˴�Ϊ�˿ɿ��ĵ��籣�� */
        /* ��ʹԭ���������ݣ���Ϊ������ͬҲ���ᱨ�� */
        if(FSMC_NAND_WriteSpare(LBNbuf, (usNewBlock + 1) * PAGES_NUM_PER_BLOCK - 1, LBN0_OFFSET, 2) == NAND_FAIL)
        {
            return NAND_FAIL;
        }

        /* ����ѭ����Ŀ���Ǵ���Ŀ���Ϊ�������� */
        for (n = 0; n < REMAP_BLOCK_NUM; n++)
        {
            /* ����ԴBLOCK */
            if (FSMC_NAND_EraseBlock(ulSrcBlock) != NAND_READY)
            {
                NAND_MarkBadBlock(usLBN); // ��Դ����Ϊ����
                s_usLUT[usLBN] = s_currentRemapBlock; // �����߼���

                ulSrcBlock = s_currentRemapBlock;
                s_currentRemapBlock++;
                NAND_SaveLUTTable(); // ����LUT�� */
                
                if (s_currentRemapBlock >= MAX_REMAP_LOG_ADDR) // ����ӳ����ù���
                {
                    return NAND_FAIL;
                }

                continue; // ����Ѱ���¿鿽��һ��
            }
            /* ������ԭ������ */
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
                NAND_MarkBadBlock(usLBN); // ��Դ����Ϊ����
                s_usLUT[usLBN] = s_currentRemapBlock; // �����߼���

                ulSrcBlock = s_currentRemapBlock;
                s_currentRemapBlock++;
                if (s_currentRemapBlock >= MAX_REMAP_LOG_ADDR) // ����ӳ����ù���
                {
                    return NAND_FAIL;
                }
                NAND_SaveLUTTable(); // ����LUT�� */

                ucReturn = NAND_OK;
                continue; // ����Ѱ���¿鿽��һ��
            }
            else
            {
                /* ��Ǹÿ����� */
                NAND_MarkUsedBlock(ulSrcBlock);
                break;
            }
        }

        if (n == REMAP_BLOCK_NUM) // û��ӳ��������
        {
            ucReturn = NAND_FAIL;
        }
    }

    if (NAND_MarkErasedSwapBlock(usNewBlock))//���������Ĳ�����־��λ
    {
        if (FSMC_NAND_EraseBlock(usNewBlock) != NAND_READY) // ����������
        {
            NAND_MarkBadBlock(usNewBlock); // ���¿���Ϊ����
        }
    }
    else
    {
        NAND_MarkBadBlock(usNewBlock);
    }

    return ucReturn;// д��ɹ� 
}

/*
*********************************************************************************************************
*   �� �� ��: NAND_Write
*   ����˵��: дһ������
*   ��    �Σ� _MemAddr   : �ڴ浥Ԫ�����ַ
*               _pReadbuff ����Ŵ�д���ݵĻ�������ָ��
*               _usSize    �����ݳ��ȣ�������FAT_SECTOR_SIZE�ֽڵ�������
*   �� �� ֵ: ִ�н����
*               - NAND_FAIL ��ʾʧ��
*               - NAND_OK   ��ʾ�ɹ�
*********************************************************************************************************
*/
uint8_t NAND_Write(uint32_t _ulMemAddr, uint8_t *_pWriteBuf, uint16_t _usSize)
{
    uint16_t usLBN; // �߼����
    uint16_t usPBN; // ������
    uint32_t ulPhyPageNo; // ����ҳ��
    uint32_t usSectorsInBlock; // ����ƫ��������
    uint16_t usAddrInPage; // ҳ��ƫ�Ƶ�ַ
    uint32_t ulTemp;

    /* ���ݳ��ȱ�����FAT_SECTOR_SIZE�ֽ������� */
    if ((_usSize % FAT_SECTOR_SIZE) != 0)
    {
        return NAND_FAIL;
    }

    usLBN = NAND_AddrToLogBlockNo(_ulMemAddr); // ����߼����
    usPBN = NAND_LBNtoPBN(usLBN); // ��ѯLUT����������

    ulTemp       = _ulMemAddr % NAND_BLOCK_SIZE; // ����ƫ�Ƶ�ַ
    usSectorsInBlock = ulTemp / FAT_SECTOR_SIZE;
    ulPhyPageNo  = usPBN * PAGES_NUM_PER_BLOCK + ulTemp / NAND_PAGE_SIZE; // ��������ҳ�� 
    usAddrInPage = ulTemp % NAND_PAGE_SIZE; //����ҳ��ƫ�Ƶ�ַ 

    /*�������δʹ�ÿ� */
    if (NAND_IsFreeBlock(usPBN))
    {
        if (FSMC_NAND_WriteData(_pWriteBuf, ulPhyPageNo, usAddrInPage, _usSize) == NAND_FAIL)
        {
            return NAND_FAIL;
        }

        /* ��Ǹÿ����� */
        if (NAND_MarkUsedBlock(ulPhyPageNo) == NAND_FAIL)
        {
            /* ���ʧ�ܣ�������д�뵽����һ���飨���п飩 */
            return NAND_FAIL;
        }

        return NAND_OK; // д��ɹ� 
    }

    /* ������д�뵽����һ���飨���п飩 */
    usSectorsInBlock += usLBN*SECTORS_NUM_PER_BLOCK;
    return NAND_WriteToNewBlock(_pWriteBuf, usSectorsInBlock, FAT_SECTOR_SIZE, _usSize/FAT_SECTOR_SIZE);
}

/*
*********************************************************************************************************
*   �� �� ��: NAND_Read
*   ����˵��: ��һ������
*   ��    �Σ� _MemAddr   : �ڴ浥Ԫ�����ַ
*               _pReadbuff ����Ŷ������ݵĻ�������ָ��
*               _usSize    �����ݳ��ȣ�������4�ֽڵ�������
*   �� �� ֵ: ִ�н����
*               - NAND_FAIL ��ʾʧ��
*               - NAND_OK   ��ʾ�ɹ�
*********************************************************************************************************
*/
uint8_t NAND_Read(uint32_t _ulMemAddr, uint8_t *_pReadBuf, uint16_t _usSize)
{
    uint16_t usLBN; // �߼���� 
    uint16_t usPBN; // ������ 
    uint32_t ulPhyPageNo; // ����ҳ�� 
    uint16_t usAddrInPage; // ҳ��ƫ�Ƶ�ַ 
    uint32_t ulTemp;

    /* ���ݳ��ȱ�����4�ֽ������� */
    if ((_usSize % 4) != 0)
    {
        return NAND_FAIL;
    }

    usLBN = NAND_AddrToLogBlockNo(_ulMemAddr);  // ���������� 
    usPBN = NAND_LBNtoPBN(usLBN); // ��ѯLUT���������� 

    if (usPBN >= NAND_BLOCK_COUNT)
    {
        /* û�и�ʽ����usPBN = 0xFFFF */
        return NAND_FAIL;
    }

    ulTemp       = _ulMemAddr % NAND_BLOCK_SIZE;
    ulPhyPageNo  = usPBN * PAGES_NUM_PER_BLOCK + ulTemp / NAND_PAGE_SIZE; // ��������ҳ�� 
    usAddrInPage = ulTemp % NAND_PAGE_SIZE; // ����ҳ��ƫ�Ƶ�ַ 

    if (FSMC_NAND_ReadData(_pReadBuf, ulPhyPageNo, usAddrInPage, _usSize) == NAND_FAIL)
    {
        return NAND_FAIL; // ��NAND Flashʧ��
    }

    /* �ɹ� */
    return NAND_OK;
}

/*
*********************************************************************************************************
*   �� �� ��: NAND_WriteOneSector
*   ����˵��: �ú��������ļ�ϵͳ��дһ���������ݡ�������С������512�ֽڻ�2048�ֽ�
*   ��    �Σ� _pBuf : ������ݵĻ�������ָ��
*               _SectorNo   ��������
*               _SectorSize ��ÿ�������Ĵ�С
*   �� �� ֵ: ִ�н����
*               - NAND_FAIL ��ʾʧ��
*               - NAND_OK   ��ʾ�ɹ�
*   ע ��:    �ú���Ϊ�����д����������ʹ�ã�дǰû���п��������������ʹ�ô˺���Ҫע��
*********************************************************************************************************
*/
static uint8_t NAND_WriteOneSector(uint8_t *_pBuf, uint32_t _SectorNo, uint16_t _SectorSize)
{
    uint16_t usLBN; // �߼����
    uint16_t usPBN; // ������
    uint32_t uiPhyPageNo; // ����ҳ��
    uint16_t usAddrInPage; // ҳ��ƫ�Ƶ�ַ
    uint32_t ulTemp;
    
    if (_SectorSize == 0)
        return NAND_FAIL;

    /* �����߼������ż����߼���� */
    usLBN = _SectorNo / SECTORS_NUM_PER_BLOCK;
    usPBN = NAND_LBNtoPBN(usLBN); // ��ѯLUT���������� 
    if (usPBN >= NAND_BLOCK_COUNT)
    {
        /* û�и�ʽ����usPBN = 0xFFFF */
        return NAND_FAIL;
    }

    ulTemp = (_SectorNo % SECTORS_NUM_PER_BLOCK) * _SectorSize; // �������ƫ�Ƶ�ַ
    uiPhyPageNo  = usPBN * PAGES_NUM_PER_BLOCK + ulTemp / NAND_PAGE_SIZE; //��������ҳ��
    usAddrInPage = ulTemp % NAND_PAGE_SIZE; // ����ҳ��ƫ�Ƶ�ַ

    if (!NAND_IsFreeSector(uiPhyPageNo, usAddrInPage, _SectorSize)) // ������ǿ�������
    {
        return EWRITE;  // дNAND Flashʧ��
    }

    if (FSMC_NAND_WriteData(_pBuf, uiPhyPageNo, usAddrInPage, _SectorSize) == NAND_FAIL)
    {
        return NAND_FAIL; // дNAND Flashʧ��
    }

    return NAND_OK;
}

/*
*********************************************************************************************************
*   �� �� ��: NAND_WriteMultiSectors
*   ����˵��: �ú��������ļ�ϵͳ������д����������ݡ�������С������512�ֽ�
*   ��    �Σ� _pBuf : ������ݵĻ�������ָ��
*               _SectorNo    ��������
*               _SectorSize  ��ÿ�������Ĵ�С
*               _SectorCount : ��������
*   �� �� ֵ: ִ�н����
*               - NAND_FAIL ��ʾʧ��
*               - NAND_OK   ��ʾ�ɹ�
*********************************************************************************************************
*/
uint8_t NAND_WriteMultiSectors(uint8_t *_pBuf, uint32_t _SectorNo, uint16_t _SectorSize, uint32_t _SectorCount)
{
    uint32_t i;
    uint16_t usLBN; // �߼����
    uint16_t usPBN; // ������
    uint16_t usSectorInBlock; // ����������
    uint16_t remainSectorNum; // ʣ����������

    uint8_t  *usPBuf;
    uint8_t  ucReturn;

    /*
        HY27UF081G2A = 128M Flash.  �� 1024��BLOCK, ÿ��BLOCK����64��PAGE�� ÿ��PAGE����2048+64�ֽڣ�
        ������С��λ��BLOCK�� �����С��λ���ֽڡ�

        ÿ��PAGE���߼��Ͽ��Է�Ϊ4��512�ֽ�������
    */
    usLBN = _SectorNo / SECTORS_NUM_PER_BLOCK; // ���㵱ǰ���
    usPBN = NAND_LBNtoPBN(usLBN); // ��ѯLUT����������

    usSectorInBlock = _SectorNo % SECTORS_NUM_PER_BLOCK; // �������ƫ��������
    usPBuf = _pBuf;  // ֱ�Ӳ�������ָ��ᵼ��USBͨѶ����

    if ((usSectorInBlock+_SectorCount) > SECTORS_NUM_PER_BLOCK)                   
    {   /* һ��д���� */
        remainSectorNum = usSectorInBlock + _SectorCount - SECTORS_NUM_PER_BLOCK;
        /* ��д�걾�� */
        for (i = 0; i < (_SectorCount-remainSectorNum); i++)
        {
            ucReturn = NAND_WriteOneSector(usPBuf, _SectorNo, _SectorSize); // дһ��������
            if (NAND_FAIL == ucReturn)
            {
                return NAND_FAIL;   
            }
            else if (EWRITE == ucReturn)
            {
                break;
            }
            _SectorNo++;
            usPBuf += _SectorSize; // ������ƫ��
        }

        if (i == (_SectorCount-remainSectorNum)) // ����д��
        {
            /* ��Ǹÿ����� */
            if (NAND_MarkUsedBlock(usPBN) == NAND_FAIL)
            {
                return NAND_FAIL;    
            }
        }
        else  // ��ʹ�ÿ�
        {
            /* ������д�뵽����һ���飨���п飩 */
            ucReturn = NAND_WriteToNewBlock(usPBuf, _SectorNo, _SectorSize, (_SectorCount-remainSectorNum-i));
            _SectorNo += _SectorCount-remainSectorNum-i;
            usPBuf += (_SectorCount-remainSectorNum-i)*_SectorSize; // add by xinxin

            if (ucReturn != NAND_OK)
            {
                return NAND_FAIL;   
            }
        }
        /* д��һ�� */
        usLBN++;
        usPBN = NAND_LBNtoPBN(usLBN); // ��ѯLUT����������

        for (i = 0; i < remainSectorNum; i++)
        {
            ucReturn = NAND_WriteOneSector(usPBuf, _SectorNo, _SectorSize); // дһ��������
            if (NAND_FAIL == ucReturn)
            {
                return NAND_FAIL;   
            }
            else if (EWRITE == ucReturn)
            {
                break;
            }
            _SectorNo++;
            usPBuf += _SectorSize; // ������ƫ��
        }

        if (i == remainSectorNum) // ����д��
        {
            /* ��Ǹÿ����� */
            if (NAND_MarkUsedBlock(usPBN) == NAND_FAIL)
            {
                return NAND_FAIL;   
            }
        }
        else // ��ʹ�ÿ�
        {
            /* ������д�뵽����һ���飨���п飩 */
            ucReturn = NAND_WriteToNewBlock(usPBuf, _SectorNo, _SectorSize, remainSectorNum-i);
            if (ucReturn != NAND_OK) // add by xinxin
            {
                return NAND_FAIL;    
            }
        }
    }
    else        
    {   /* һ��д���� */
        for (i = 0; i < _SectorCount; i++)
        {
            ucReturn = NAND_WriteOneSector(usPBuf, _SectorNo, _SectorSize); // дһ��������
            if (NAND_FAIL == ucReturn)
            {
                return NAND_FAIL;    
            }
            else if (EWRITE == ucReturn)
            {
                break;
            }
            _SectorNo++;
            usPBuf += _SectorSize; // ������ƫ��
        }

        if (i == _SectorCount) // ��������д��
        {
            /* ��Ǹÿ����� */
            if (NAND_MarkUsedBlock(usPBN) == NAND_FAIL)
            {
                return NAND_FAIL;    
            }
        }
        else // ��ʹ������
        {
            /* ������д�뵽����һ���飨���п飩 */
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
*   �� �� ��: NAND_ReadOneSector
*   ����˵��: �ú��������ļ�ϵͳ�������������ݡ���1��������������С������512�ֽ�
*   ��    �Σ� _pBuf : ��Ŷ������ݵĻ�������ָ��
*               _SectorNo   ��������
*               _SectorSize ��ÿ�������Ĵ�С
*   �� �� ֵ: ִ�н����
*               - NAND_FAIL ��ʾʧ��
*               - NAND_OK   ��ʾ�ɹ�
*********************************************************************************************************
*/
static uint8_t NAND_ReadOneSector(uint8_t *_pBuf, uint32_t _SectorNo, uint16_t _SectorSize)
{
    uint16_t usLBN; // �߼���� 
    uint16_t usPBN; // ������ 
    uint32_t uiPhyPageNo; // ����ҳ�� 
    uint16_t usAddrInPage; // ҳ��ƫ�Ƶ�ַ 
    uint32_t ulTemp;

    /* �����߼������ź�������С�����߼���� */
    usLBN = _SectorNo / SECTORS_NUM_PER_BLOCK;
    usPBN = NAND_LBNtoPBN(usLBN); // ��ѯLUT���������� 

    if (usPBN >= NAND_BLOCK_COUNT)
    {
        /* û�и�ʽ����usPBN = 0xFFFF */
        return NAND_FAIL;
    }

    ulTemp = (_SectorNo % SECTORS_NUM_PER_BLOCK) * _SectorSize;
    uiPhyPageNo  = usPBN * PAGES_NUM_PER_BLOCK + ulTemp / NAND_PAGE_SIZE;  // ��������ҳ�� 
    usAddrInPage = ulTemp % NAND_PAGE_SIZE; // ����ҳ��ƫ�Ƶ�ַ 

    if (FSMC_NAND_ReadData(_pBuf, uiPhyPageNo, usAddrInPage, _SectorSize) == NAND_FAIL)
    {
        return NAND_FAIL; // ��NAND Flashʧ�� 
    }

    return NAND_OK;
}

/*
*********************************************************************************************************
*   �� �� ��: NAND_ReadMultiSectors
*   ����˵��: �ú��������ļ�ϵͳ�������������ݡ���1������������������С������512�ֽ�
*   ��    �Σ� _pBuf : ��Ŷ������ݵĻ�������ָ��
*               _SectorNo ��������
*               _SectorSize ��ÿ�������Ĵ�С
*               _SectorCount : ��������
*   �� �� ֵ: ִ�н����
*               - NAND_OK �ɹ�;
*               - NAND_FAIL ʧ��
*********************************************************************************************************
*/
uint8_t NAND_ReadMultiSectors(uint8_t *_pBuf, uint32_t _SectorNo, uint16_t _SectorSize, uint32_t _SectorCount)
{
    uint32_t i;
    uint8_t  *usPBuf;
    usPBuf = _pBuf; // ֱ�Ӳ�������ָ��ᵼ��USBͨѶ����
    /*
        HY27UF081G2A = 128M Flash.  �� 1024��BLOCK, ÿ��BLOCK����64��PAGE�� ÿ��PAGE����2048+64�ֽڣ�
        ������С��λ��BLOCK�� �����С��λ���ֽڡ�

        ÿ��PAGE���߼��Ͽ��Է�Ϊ4��512�ֽ�������
    */
    for (i = 0; i < _SectorCount; i++)
    {
        if(NAND_ReadOneSector(usPBuf, _SectorNo, _SectorSize) == NAND_FAIL) // ��һ����������
        {
            return NAND_FAIL; // ��NAND Flashʧ��
        }

        _SectorNo++;
        usPBuf += _SectorSize; // ������ƫ��
    }

    /* �ɹ� */
    return NAND_OK;
}

/*
*********************************************************************************************************
*   �� �� ��: NAND_BuildLUT
*   ����˵��: ���ڴ��д�����������
*   ��    �Σ�-
*   �� �� ֵ: ִ�н��
*              - NAND_OK�� �ɹ���  
*              - NAND_FAIL��ʧ��
*********************************************************************************************************
*/
static uint8_t NAND_BuildLUT(void)
{
    uint16_t i, j, k, t;
    uint32_t currentReadPage;
    uint16_t remainSize;

    for (i = 0; i < NAND_BLOCK_COUNT; i++)
    {
        s_usLUT[i] = 0xFFFF; // �����Чֵ�������ؽ�LUT���ж�LUT�Ƿ����
    }

    /* ��ȡ���������¼��Ϣ */
    for (i = 0; i < BAD_TABLE_NUM; i++)
    {
        currentReadPage = (i+MAX_DATA_LOG_ADDR) * PAGES_NUM_PER_BLOCK;
        
        if (!NAND_IsBadBlock(i+MAX_DATA_LOG_ADDR)) // ������������ǻ���
        {
            if (FSMC_NAND_ReadSpare((uint8_t *)&s_currentRemapBlock, currentReadPage, LBN0_OFFSET, 2) == NAND_FAIL) // ��ȡ��ʹ�õĻ���ӳ��
            {
                continue; // ��ȡ��һ���߼���
            }

            remainSize = NAND_BLOCK_COUNT << 1;
            k = 0,
            t = 0;
            while (1)
            {
                if (FSMC_NAND_ReadData(s_ucTempBuf, currentReadPage + t, 0, TABLE_PACKET_SIZE) == NAND_FAIL)
                {
                    break; // ��ȡ��һ���߼���
                }

                for (j = 0; j < TABLE_PACKET_SIZE; k++)
                {
                    s_usLUT[k] = s_ucTempBuf[j] + ((u16)s_ucTempBuf[j+1]<<8); // ����LUT��
                    j += 2;
                }
                remainSize -= TABLE_PACKET_SIZE;
                t++;

                if (remainSize < TABLE_PACKET_SIZE) // ����2048
                {
                    if (remainSize) // ����ʣ���ֽ�
                    {
                        if (FSMC_NAND_ReadData(s_ucTempBuf, currentReadPage + t, 0, remainSize) == NAND_FAIL)
                        {
                            break; // ��ȡ��һ���߼���
                        }

                        for (j = 0; j < remainSize; k++)
                        {
                            s_usLUT[k] = s_ucTempBuf[j] + ((u16)s_ucTempBuf[j+1]<<8); // ����LUT��
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

    if (i == BAD_TABLE_NUM) // �����������ȫ��
    {
        NAND_ReBuildLUT(); // ���½�����������
    }

    /* LUT������ϣ�����Ƿ���� */
    for (i = 0; i < NAND_BLOCK_COUNT; i++)
    {
        if (s_usLUT[i] == 0xFFFF) // ��һ��Ϊ0xFFFFֵ
        {
            s_usValidDataBlockCount = i;
            break;
        }
    }

    if (s_usValidDataBlockCount < MIN_BLOCK_NUM)
    {
        /* ���� ������Ч�߼����С��MIN_BLOCK_NUM��������û�и�ʽ�� */
        return NAND_FAIL;
    }

    for (i = 0; i < s_usValidDataBlockCount; i++)
    {
        if (s_usLUT[i] == 0xFFFF)
        {
            return NAND_FAIL;    // ����LUT���߼���Ŵ�����Ծ���󣬿�����û�и�ʽ��
        }
    }

    /* �ؽ�LUT���� */
    return NAND_OK;
}

/*
*********************************************************************************************************
*   �� �� ��: NAND_ReBuildLUT
*   ����˵��: ����ÿҳ������Ϣ�����ڴ��д�����������
*   ��    �Σ�-
*   �� �� ֵ: ִ�н����
*               - NAND_OK�� �ɹ���     
*               - NAND_FAIL��ʧ��
*********************************************************************************************************
*/
static uint8_t NAND_ReBuildLUT(void)
{
    uint16_t i;
    uint8_t  buf[VALID_SPARE_SIZE];

    for (i = 0; i < NAND_BLOCK_COUNT; i++)
    {
        /* ��ÿ����ĵ�1��PAGE��ƫ�Ƶ�ַΪLBN0_OFFSET������ */
        FSMC_NAND_ReadSpare(buf, i * PAGES_NUM_PER_BLOCK, BI_OFFSET, VALID_SPARE_SIZE);

        s_usLUT[i] = buf[LBN0_OFFSET] + ((u16)buf[LBN1_OFFSET] << 8);// ����������߼����

        if (s_usLUT[i] != i)// �߼���������鲻�ȣ���Ϊ��ӳ��
        {
            s_currentRemapBlock++;
        }
    }
    return NAND_OK;
}

/*
*********************************************************************************************************
*   �� �� ��: NAND_AddrToLogBlockNo
*   ����˵��: �ڴ��߼���ַת��Ϊ�߼����
*   ��    �Σ�_ulMemAddr���߼��ڴ��ַ
*   �� �� ֵ: ����ҳ�ţ� ����� 0xFFFFFFFF ���ʾ����
*********************************************************************************************************
*/
static uint16_t NAND_AddrToLogBlockNo(uint32_t _ulMemAddr)
{
    uint16_t usLBN; // �߼���� 
    usLBN = _ulMemAddr / NAND_BLOCK_SIZE;//�����߼���� 

    return usLBN;
}

/*
*********************************************************************************************************
*   �� �� ��: NAND_LBNtoPBN
*   ����˵��: �߼����ת��Ϊ������
*   ��    ��: _uiLBN : �߼���� Logic Block No
*   �� �� ֵ: �����ţ� ����� 0xFFFFFFFF ���ʾ����
*********************************************************************************************************
*/
uint16_t NAND_LBNtoPBN(uint32_t _uiLBN)
{
    uint16_t usPBN;     /* ������ */

    /* ����߼���Ŵ�����Ч�����ݿ������̶�����0xFFFF, ���øú����Ĵ���Ӧ�ü������ִ��� */
    if (s_usLUT[_uiLBN] == 0xFFFF)
    {
        return 0xFFFF;
    }

    /* ��ѯLUT�����ӳ���������� */
    usPBN = s_usLUT[_uiLBN];
    return usPBN;
}

/*
*********************************************************************************************************
*   �� �� ��: NAND_FindFreeBlock
*   ����˵��: ����һ�����õĿ���Ϊ�����顣
*   ��    �Σ�-
*   �� �� ֵ: ��ţ������0xFFFF��ʾʧ��
*********************************************************************************************************
*/
static uint16_t NAND_FindFreeBlock (void)
{
    static uint16_t swapNum = MAX_REMAP_LOG_ADDR; // �ӽ�������ַ��ʼ
    uint16_t i;
    uint16_t freeBlock = 0xFFFF;

    for (i = 0; i < SWAP_BLOCK_NUM; i++) // �������н�����
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
*   �� �� ��: NAND_IsBufOk
*   ����˵��: �ж��ڴ滺�����������Ƿ�ȫ��Ϊָ��ֵ
*   ��    �Σ�- _pBuf : ���뻺����
*             - _ulLen : ����������
*             - __ucValue : ������ÿ����Ԫ����ȷ��ֵ
*   �� �� ֵ: 1 ��ȫ����ȷ;
*             0 ������ȷ
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
*   �� �� ��: NAND_IsBadBlock
*   ����˵��: ���ݻ����Ǽ��NAND Flashָ���Ŀ��Ƿ񻵿�
*   ��    ��: _ulBlockNo ����� 0 - 1023 ������128M�ֽڣ�2K Page��NAND Flash����1024���飩
*   �� �� ֵ: 0 ���ÿ����;
*             1 ���ÿ��ǻ���
*********************************************************************************************************
*/
uint8_t NAND_IsBadBlock(uint32_t _ulBlockNo)
{
    uint8_t ucFlag;

    /* ���NAND Flash����ǰ�Ѿ���עΪ�����ˣ������Ϊ�ǻ��� */
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

    return 0;   // �Ǻÿ� 
}

/*
*********************************************************************************************************
*   �� �� ��: NAND_IsFreeBlock
*   ����˵��: ���ݻ����Ǻ�USED��־����Ƿ���ÿ�
*   ��    ��: _ulBlockNo ����� 0 - 1023 ������128M�ֽڣ�2K Page��NAND Flash����1024���飩
*   �� �� ֵ: 1 ���ÿ����;
*             0 ���ÿ��ǻ��������ռ��
*********************************************************************************************************
*/
static uint8_t NAND_IsFreeBlock(uint32_t _ulBlockNo)
{
    uint8_t ucFlag;

    /* ���NAND Flash����ǰ�Ѿ���עΪ�����ˣ������Ϊ�ǻ��� */
    if (NAND_IsBadBlock(_ulBlockNo))
    {
        return 0;
    }

    FSMC_NAND_ReadSpare(&ucFlag, _ulBlockNo * PAGES_NUM_PER_BLOCK, USED_OFFSET, 1);
    if (ucFlag == 0xFF)
    {
        return 1;//�ÿ����
    }

    return 0;
}

/*
*********************************************************************************************************
*   �� �� ��: NAND_IsFreeSector
*   ����˵��: ���������������Ƿ�ȫΪFF�ж������Ƿ�ʹ��
*   ��    ��: _uiPhyPageNo ������ҳ�ţ�usAddrInPage��ҳ��ƫ�Ƶ�ַ
*   �� �� ֵ: 1 ���ÿ�δʹ��;
*             0 ����������ʹ��
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
*   �� �� ��: NAND_MarkUsedBlock
*   ����˵��: ���NAND Flashָ���Ŀ�Ϊ���ÿ�
*   ��    ��: _ulBlockNo ����� 0 - 1023 ������128M�ֽڣ�2K Page��NAND Flash����1024���飩
*   �� �� ֵ: ִ�н����
*               - NAND_OK:��ǳɹ�;
*               - NAND_FAIL�����ʧ�ܣ��ϼ����Ӧ�ý��л��鴦��
*********************************************************************************************************
*/
static uint8_t NAND_MarkUsedBlock(uint32_t _ulBlockNo)
{
    uint32_t ulPageNo;
    uint8_t  ucFlag;

    /* �����ĵ�1��ҳ�� */
    ulPageNo = _ulBlockNo * PAGES_NUM_PER_BLOCK; // ����ÿ��1��ҳ��ҳ��

    /* ���ڵ�1��page�������ĵ�2���ֽ�д���0xFF���ݱ�ʾ���ÿ� */
    ucFlag = NAND_USED_BLOCK_FLAG;
    if (FSMC_NAND_WriteSpare(&ucFlag, ulPageNo, USED_OFFSET, 1) == NAND_FAIL)
    {
        /* ������ʧ�ܣ�����Ҫ��ע�����Ϊ���� */
        return NAND_FAIL;
    }

    return NAND_OK;
}

/*
*********************************************************************************************************
*   �� �� ��: NAND_MarkErasedSwapBlock
*   ����˵��: ���NAND Flashָ���Ŀ�Ĳ�����־
*   ��    ��: _ulBlockNo ����� 0 - 1023 ������128M�ֽڣ�2K Page��NAND Flash����1024���飩
*   �� �� ֵ: ִ�н��:
*               - NAND_OK:��ǳɹ�;
*               - NAND_FAIL�����ʧ�ܣ��ϼ����Ӧ�ý��л��鴦��
*********************************************************************************************************
*/
static uint8_t NAND_MarkErasedSwapBlock(uint32_t _ulBlockNo)
{
    uint32_t ulPageNo;
    uint8_t  ucFlag;

    /* ���������1��ҳ�� */
    ulPageNo = (_ulBlockNo + 1) * PAGES_NUM_PER_BLOCK - 1; //����ÿ����1��ҳ��ҳ�� 

    /* ���һҳ����չ�������ڵ����һλ��ʾ������־λ */
    ucFlag = NAND_ERASE_BLOCK_FLAG;
    if (FSMC_NAND_WriteSpare(&ucFlag, ulPageNo, ERASED_OFFSET, 1) == NAND_FAIL)
    {
        /* ������ʧ�ܣ�����Ҫ��ע�����Ϊ���� */
        return NAND_FAIL;
    }

    return NAND_OK;
}

/*
*********************************************************************************************************
*   �� �� ��: NAND_IsFreeBlock
*   ����˵��: ��ȡһ�������һҳ�Ĳ�����־
*   ��    ��: _ulBlockNo ����� 0 - 1023 ������128M�ֽڣ�2K Page��NAND Flash����1024���飩
*   �� �� ֵ: 1 ��������־��λ���ÿ������Ѿ����ã������;
*             0 ���ÿ�δ׼����������ȷ�������Ƿ���Ч
*********************************************************************************************************
*/
static uint8_t NAND_IsErasedSwapBlock(uint32_t _ulBlockNo)
{
    uint8_t ucFlag;

    /* ��ȡ�ÿ�����һҳ��ERASED_OFFSETλ�������λλ0x00���������־�Ѿ���λ */
    FSMC_NAND_ReadSpare(&ucFlag, ((_ulBlockNo + 1) * PAGES_NUM_PER_BLOCK -1), ERASED_OFFSET, 1);
    if (ucFlag != 0xFF)
    {
        return 1;
    }

    return 0;
}

/*
*********************************************************************************************************
*   �� �� ��: NAND_MarkBadBlock
*   ����˵��: ���NAND Flashָ���Ŀ�Ϊ����
*   ��    ��: _ulBlockNo ����� 0 - 1023 ������128M�ֽڣ�2K Page��NAND Flash����1024���飩
*   �� �� ֵ: ��
*********************************************************************************************************
*/
void NAND_MarkBadBlock(uint32_t _ulBlockNo)
{
    uint32_t ulPageNo;
    uint8_t  ucFlag;

    /* �����ĵ�1��ҳ�� */
    ulPageNo = _ulBlockNo * PAGES_NUM_PER_BLOCK; //����ÿ��1��ҳ��ҳ�� 

    /* ���ڵ�1��page�������ĵ�1���ֽ�д���0xFF���ݱ�ʾ���� */
    ucFlag = NAND_BAD_BLOCK_FLAG;
    if (FSMC_NAND_WriteSpare(&ucFlag, ulPageNo, BI_OFFSET, 1) == NAND_FAIL)
    {
        /* �����1��ҳ���ʧ�ܣ��������һҳ��� */
        FSMC_NAND_WriteSpare(&ucFlag, ulPageNo + PAGES_NUM_PER_BLOCK - 1, BI_OFFSET, 1);

    }
}

/*
*********************************************************************************************************
*   �� �� ��: NAND_Format
*   ����˵��: NAND Flash��ʽ�����������е����ݣ��ؽ�LUT
*   ��    �Σ���
*   �� �� ֵ: ִ�н��:
*               - NAND_OK : �ɹ�; 
*               - NAND_Fail ��ʧ�ܣ�һ���ǻ����������ർ�£�
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
        s_usLUT[i] = 0xFFFF; // �����Чֵ�������ؽ�LUT���ж�LUT�Ƿ����
    }
    s_currentRemapBlock = MAX_TABLE_LOG_ADDR; // ��ǰӳ��Ŀ��� 

    /* ����ÿ���� */
    for (i = MAX_DATA_LOG_ADDR; i < MAX_SWAP_LOG_ADDR; i++) // �Ȳ�����¼����Ϣ
    {
        if (!NAND_IsBadBlock(i)) // ����Ǻÿ���ܲ���
        {
            status = FSMC_NAND_EraseBlock(i);
            if (status != NAND_READY) // ����һ��
            {
                status = FSMC_NAND_EraseBlock(i);
            }
            if (status != NAND_READY)
            {
                NAND_MarkBadBlock(i); // ���Ϊ���飬����ӳ��
            }
        }
    }

    for (i = 0; i < MAX_DATA_LOG_ADDR; i++) // ������������Ϣ
    {
        if (!NAND_IsBadBlock(i)) // ����Ǻÿ�Ų���
        {
            status = FSMC_NAND_EraseBlock(i);
            if (status != NAND_READY)
            {
                status = FSMC_NAND_EraseBlock(i);
            }

            if (status == NAND_READY) // �����ɹ�
            {
                usGoodBlockCount++;
                FSMC_NAND_WriteSpare((uint8_t *)&i, i * PAGES_NUM_PER_BLOCK, LBN0_OFFSET, 2);
                FSMC_NAND_WriteSpare((uint8_t *)&i, (i+1) * PAGES_NUM_PER_BLOCK - 1, LBN0_OFFSET, 2);//д���β����lbo
                s_usLUT[i] = i; // �����߼���
                continue;
            }
        }

        /* ���л���ӳ�� */
        while (NAND_IsBadBlock(s_currentRemapBlock)) // �ж�ӳ����Ƿ����
        {
            s_currentRemapBlock++;

            if (s_currentRemapBlock >= MAX_REMAP_LOG_ADDR)  // ����ӳ����ù���
            {
                return NAND_FAIL;
            }
        }

        buf[LBN0_OFFSET] = (u8)s_currentRemapBlock;
        buf[LBN1_OFFSET] = (u8)(s_currentRemapBlock>>8);
        // ��Ϣ��¼�ڵ�һҳ�����һҳ
        FSMC_NAND_WriteSpare(buf, i * PAGES_NUM_PER_BLOCK, BI_OFFSET, VALID_SPARE_SIZE);
        FSMC_NAND_WriteSpare(buf, (i+1) * PAGES_NUM_PER_BLOCK - 1, BI_OFFSET, VALID_SPARE_SIZE);//д���β����lbo

        s_usLUT[i] = s_currentRemapBlock;// �����߼���
        s_currentRemapBlock++;
    }

    /* ����ÿ����������MIN_BLOCK_NUM����NAND Flash���� */
    if (usGoodBlockCount < MIN_BLOCK_NUM)
    {
        return NAND_FAIL;
    }

    NAND_SaveLUTTable(); //���±���LUT��

    return NAND_OK;
}

/*
*********************************************************************************************************
*   �� �� ��: NAND_SaveLUTTable
*   ����˵��: �����߼�ӳ���
*   ��    �Σ���
*   �� �� ֵ: ��
*********************************************************************************************************
*/
static void NAND_SaveLUTTable(void)
{
    uint16_t i, j, k = 0, t = 0;
    uint32_t currentWritePage;
    uint16_t remainSize;
    /* дǰ���� */
    for (i = 0; i < BAD_TABLE_NUM; i++)
    {
        if (FSMC_NAND_EraseBlock(i+MAX_DATA_LOG_ADDR) != NAND_READY)
        {
            NAND_MarkBadBlock(i+MAX_DATA_LOG_ADDR);
        }
        else   // �����Է�����չ���������������д�룬����д����ȥ�����о�
        {
            currentWritePage = (i+MAX_DATA_LOG_ADDR) * PAGES_NUM_PER_BLOCK;
            FSMC_NAND_WriteSpare((uint8_t *)&s_currentRemapBlock, currentWritePage, LBN0_OFFSET, 2); // ��¼��ʹ�õĻ���ӳ��
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

        if (remainSize < TABLE_PACKET_SIZE)    // ����2048
        {
            if (remainSize)                    // ����ʣ���ֽ�
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
*   �� �� ��: NAND_FormatCapacity
*   ����˵��: NAND Flash��ʽ�������Ч����
*   ��    �Σ���
*   �� �� ֵ: ������С
*********************************************************************************************************
*/
uint32_t NAND_FormatCapacity(void)
{
    /* �������ڴ洢���ݵ����ݿ���� */
    return (MAX_DATA_LOG_ADDR * NAND_BLOCK_SIZE);
}

/*
*********************************************************************************************************
*   �� �� ��: NAND_CheckFileSystem
*   ����˵��: ���籣������һ����,�ڳ�ʼ��nand�󣬼�����н�����������ڲ�����Ч���ݴ����������
*               �����޸������û����Ч���ݣ��򽫽�����������
*               ���޸����������Դ��򽻻����Ϊ���飬���޸�����ʧ�ܣ��ļ�ϵͳ�����𻵡�
*   ��    �Σ���
*   �� �� ֵ: ִ�н����
*              - NAND_OK   �����ɹ�
*              - NAND_FAIL ����ʧ��
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
    /* ������н����� */
    for (i = MAX_REMAP_LOG_ADDR; i < MAX_SWAP_LOG_ADDR; i++)
    {
        if (NAND_IsBlockDataValid(i) == NAND_OK) //�ڽ������ڷ�����Ч���ݣ���Ҫ��д
        {
            /* ��ȡ����������Ч���ݶ�Ӧ���߼��� */
            ulPageNo = i * PAGES_NUM_PER_BLOCK; // ����ÿ��1��ҳ��ҳ�� 
            if (FSMC_NAND_ReadSpare(tempBuf, ulPageNo, LBN0_OFFSET, 2) == NAND_OK) // ��ȡ�߼���
            {
                logicNum = tempBuf[0] + ((uint16_t)tempBuf[1] << 8);
                if(logicNum != 0xFFFF)
                {   
                    /* ��ȡ���������� */
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

            /* ����Դ���ݿ� */
            if (FSMC_NAND_EraseBlock(tarBlock) != NAND_READY) 
            {
                NAND_MarkBadBlock(tarBlock); 
                return NAND_FAIL;
            }

            /* ���ݻ�д������������Ч����д�ص�Դ���ݿ��� */
            if (NAND_WriteToSrcBlock(i, tarBlock) == NAND_OK)
            {
                if (NAND_MarkErasedSwapBlock(i))//����֮ǰ����λ����ֹ��ʱ�ϵ�
                {
                    /* ���������� */
                    if (FSMC_NAND_EraseBlock(i) != NAND_READY) 
                    {
                        NAND_MarkBadBlock(i); 
                        return NAND_FAIL;
                    }
                    /* ִ�е��˴��Ѿ��ָ���ɣ���û�˳������ڼ��ʣ�ཻ���� */
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
*   �� �� ��: NAND_IsBlockDataValid
*   ����˵��: ���籣������һ���֣��ú������ڼ�⽻���������Ƿ������Ч���ݣ����������Ч�������ݣ��򽫸�
*               �������������֤�������Ǹɾ����Եġ�
*         ps��ֻ���ڵ�һҳ�����һҳ���߼�����ͬ�Ҳ�����0XFFFF��������ҳ�����ܾ���ecc��֤��
*               �����������Ϊ������Ч�������ÿ顣
*             
*   ��    �Σ�_ulBlockNo��Ҫ���Ŀ��
*   �� �� ֵ: ִ�н��  
*               - NAND_OK   ��������������Ч
*               - NAND_FAIL ��������������Ч
*********************************************************************************************************
*/
static uint8_t NAND_IsBlockDataValid(uint32_t _ulBlockNo)
{
    uint16_t logicNum1,logicNum2;
    uint8_t tempBuf[2];
    uint32_t ulPageNo;

    logicNum1 = 0xFFFF;
    if (!NAND_IsBadBlock(_ulBlockNo)) // ������������ǻ���
    {
        /* ���������־����λ����������Ч����ֱ�Ӳ��� */
        if (NAND_IsErasedSwapBlock(_ulBlockNo))
        {
            if (FSMC_NAND_EraseBlock(_ulBlockNo) != NAND_READY) 
            {
                NAND_MarkBadBlock(_ulBlockNo); 
            }
            return NAND_FAIL;
        }

    /* ��⽻�������ݺ��߼����Ƿ���Ч,������Ч״̬��������󷵻� */
        
        /* ����1ҳ�����ݺ��߼����Ƿ���Ч */
        ulPageNo = _ulBlockNo * PAGES_NUM_PER_BLOCK; // ����ÿ��1��ҳ��ҳ�� 
        if (FSMC_NAND_ReadSpare(tempBuf, ulPageNo, LBN0_OFFSET, 2) == NAND_OK) // ��ȡ�߼���
        {   
            logicNum1 = tempBuf[0] + ((uint16_t)tempBuf[1] << 8);
            if (logicNum1 != 0xFFFF)  
            {   /* ���߼�����Ч */

                /* ����1ҳ��������Ч */
                if (FSMC_NAND_ReadData(s_ucTempBuf, ulPageNo, 0, NAND_PAGE_SIZE) == NAND_OK)
                {
                    /* �˴�û���κβ������������ÿ������1ҳ */
                }
                else
                {
                    /* �����ý����� */
                    if (FSMC_NAND_EraseBlock(_ulBlockNo) != NAND_READY) 
                    {
                        NAND_MarkBadBlock(_ulBlockNo); 
                    }
                    return NAND_FAIL;
                }
            }
            else
            {
                /* �����ý����� */
                if (FSMC_NAND_EraseBlock(_ulBlockNo) != NAND_READY) 
                {
                    NAND_MarkBadBlock(_ulBlockNo); 
                }
                return NAND_FAIL; //�������߼���λFFFF��֤������δд�뵽�˾͵�����
            }
        }
        else
        {
            return NAND_FAIL; //����ҳ����ʧ�ܷ���
        }
        
        /* ִ�е��˴���֤����һҳ�����ݺ��߼�����Ч */

        /* ������1ҳ�����ݺ��߼����Ƿ���Ч */
        logicNum2 = 0xFFFF;
        ulPageNo = (_ulBlockNo + 1) * PAGES_NUM_PER_BLOCK - 1; // ����ÿ����1��ҳ��ҳ�� 
        if (FSMC_NAND_ReadSpare(tempBuf, ulPageNo, LBN0_OFFSET, 2) == NAND_OK) // ��ȡ�߼���
        {   
            logicNum2 = tempBuf[0] + ((uint16_t)tempBuf[1] << 8);
            if (logicNum2 != 0xFFFF) 
            {   /* ���߼�����Ч */
                
                /* ������1ҳ��������Ч */
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
                        return NAND_FAIL; //�����߼��Ų�ͬ������
                    }
                }
                else
                {
                    /* �����ý����� */
                    if (FSMC_NAND_EraseBlock(_ulBlockNo) != NAND_READY) 
                    {
                        NAND_MarkBadBlock(_ulBlockNo); 
                    }
                    return NAND_FAIL; //������Ч������
                }
            }
            else
            {
                /* �����ý����� */
                if (FSMC_NAND_EraseBlock(_ulBlockNo) != NAND_READY) 
                {
                    NAND_MarkBadBlock(_ulBlockNo); 
                }
                return NAND_FAIL; //�������߼���λFFFF��֤������δд�뵽�˾͵�����
            }
        }
        else
        {
            return NAND_FAIL; //����ҳ����ʧ�ܷ���
        }
    }

    return NAND_FAIL; //����ֱ�ӷ���
}

/*
*********************************************************************************************************
*   �� �� ��: NAND_WriteToSrcBlock
*   ����˵��: ��TarBlockNo�ڵ�����д�ص�SrcBlockNo��
*   ��    �Σ�  
*               _SrcBlockNo  ��Դ���ݿ��ַ
*               _TarBlockNo   ���������ݿ��ַ
*   �� �� ֵ: ִ�н����
*               - NAND_OK   ��ʾ�ɹ�
*               - NAND_FAIL ��ʾʧ��
*********************************************************************************************************
*/
static uint8_t NAND_WriteToSrcBlock(uint32_t _SrcBlockNo, uint32_t _TarBlockNo)
{
    int i;
    uint32_t _ulSrcPageNo, _ulTarPageNo;
    _ulSrcPageNo = _SrcBlockNo * PAGES_NUM_PER_BLOCK; // ����Դ���ݿ��1��ҳ��ҳ�� 
    _ulTarPageNo = _TarBlockNo * PAGES_NUM_PER_BLOCK; // ���㽻�����ݿ��1��ҳ��ҳ�� 

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

/***************************** �������� (END OF FILE) *********************************/

#endif
