#include "includes.h"
#include "bsp_spi_551.h"


/*********************************************************
** ��������BCM_Init
** �䡡�룺None  
** �䡡����None
** ����������Initialize the flash.
************************************************************/
static void BCM_CfgSPI(void)
{
	SPI_InitTypeDef  SPI_InitStructure;

	/* ��SPIʱ�� */
	ENABLE_BCMSPI_RCC();

	SPI_I2S_DeInit(SPI4);
	/* ����SPIӲ������ */
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;	/* ���ݷ���2��ȫ˫�� */
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		                    /* STM32��SPI����ģʽ ������ģʽ */
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;	                  /* ����λ���� �� 8λ */
	/* SPI_CPOL��SPI_CPHA���ʹ�þ���ʱ�Ӻ����ݲ��������λ��ϵ��
	   ��������: ���߿����ǵ͵�ƽ,��1�����أ������ز�������)
	*/
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;			                    /* ʱ�������ز������� */
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;		                    /* ʱ�ӵĵ�1�����ز������� */
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;			                      /* Ƭѡ���Ʒ�ʽ��������� */

	/* ���ò�����Ԥ��Ƶϵ�� */
	SPI_InitStructure.SPI_BaudRatePrescaler = BCM_SPI_CLK;

	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	                /* ����λ������򣺸�λ�ȴ� */
	SPI_InitStructure.SPI_CRCPolynomial = 7;			                      /* CRC����ʽ�Ĵ�������λ��Ϊ7�������̲��� */
	SPI_Init(BCM_SPI, &SPI_InitStructure);

	SPI_Cmd(BCM_SPI, DISABLE);			/* �Ƚ�ֹSPI  */

	SPI_Cmd(BCM_SPI, ENABLE);				/* ʹ��SPI  */
}

int8_t BSP_BCMInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/* ʹ��GPIO ʱ�� */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE|RCC_AHB1Periph_GPIOI|RCC_AHB1Periph_GPIOH, ENABLE);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;

	GPIO_InitStructure.GPIO_Pin = BCM_SPI_SCK_PIN | BCM_SPI_MISO_PIN | BCM_SPI_MOSI_PIN ;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	
		/* ���� SCK, MISO �� MOSI Ϊ���ù��� */
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource2, GPIO_AF_SPI4);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource5, GPIO_AF_SPI4);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource6, GPIO_AF_SPI4);
	
	/* ����Ƭѡ����Ϊ�������ģʽ */	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Pin = BCM_CS_PIN  ;
	GPIO_Init(GPIOI, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = BCM_551CS_PIN;
	GPIO_Init(BCM_551CS_GPIO_PORT, &GPIO_InitStructure);
	
	BCM_CS_HIGH();		/* Ƭѡ�øߣ���ѡ�� */
	BCM_551CS_HIGH();

	/* ����SPIӲ���������ڷ��ʴ���Flash */
	BCM_CfgSPI();

	return 1;
}

/*********************************************************
** ��������BCM_WriteByte
** �䡡�룺byte --- 8-bit data being sent. 
** �䡡����8-bit data being received.
** ����������Sending the 8-bit data.
************************************************************/
u8 BCM_WriteByte(u8 *byte,u32 wr_len)
{
	int i=0;
	
	for(i=0;i<wr_len;i++)
	{
		/*!< Loop while DR register in not emplty */
		u32 spitimeover = 0;
		while (SPI_I2S_GetFlagStatus(BCM_SPI, SPI_I2S_FLAG_TXE) == RESET && spitimeover <= SPI_WAIT_TIMEDLY)
		{
			spitimeover++;
		}
		if(spitimeover > SPI_WAIT_TIMEDLY)
			return 0;
		/*!< Send byte through the SPI1 peripheral */
		SPI_I2S_SendData(BCM_SPI, *(byte+i));
		
		/*!< Wait to receive a byte */
		spitimeover = 0;
		while (SPI_I2S_GetFlagStatus(BCM_SPI, SPI_I2S_FLAG_RXNE) == RESET && spitimeover <= SPI_WAIT_TIMEDLY)
		{
			spitimeover++;
		}

		if(spitimeover > SPI_WAIT_TIMEDLY)
			return 0;
		/*!< Return the byte read from the SPI bus */
		SPI_I2S_ReceiveData(BCM_SPI);
	}
	return 1;
}

/*********************************************************
** ��������BCM_ReadByte
** �䡡�룺byte --- 8-bit data being sent. 
** �䡡����8-bit data being received.
** ����������Sending the 8-bit data.
************************************************************/
u8 BCM_ReadByte(u8 *byte,u32 len)
{
	int i=0;
	u16 data;
	
	for(i=0;i<len;i++)
	{
		u32 spitimeover = 0;
		while (SPI_I2S_GetFlagStatus(BCM_SPI, SPI_I2S_FLAG_TXE) == RESET && spitimeover <= SPI_WAIT_TIMEDLY)
		{
			spitimeover++;
		}
		if(spitimeover > SPI_WAIT_TIMEDLY)
			return 0;
		/*!< Send byte through the SPI1 peripheral */
		SPI_I2S_SendData(BCM_SPI, 0x00);
		
		/*!< Wait to receive a byte */
		spitimeover = 0;
		while (SPI_I2S_GetFlagStatus(BCM_SPI, SPI_I2S_FLAG_RXNE) == RESET && spitimeover <= SPI_WAIT_TIMEDLY)
		{
			spitimeover++;
		}
		//while (SPI_I2S_GetFlagStatus(BCM_SPI, SPI_I2S_FLAG_RXNE) == RESET);

		if(spitimeover > SPI_WAIT_TIMEDLY)
			return 0;
		/*!< Return the byte read from the SPI bus */
		data = SPI_I2S_ReceiveData(BCM_SPI);
		*(byte+i)=(u8)(data&0xff);
	}
	return (i);
}


/*********************************************************
** ��������aps_spi_read
** �䡡�룺None  
** �䡡����None
** ����������
************************************************************/
int32_t aps_spi_read(int32_t d,uint8_t *wr_buf,int32_t wr_len,uint8_t *rd_buf,int32_t rd_len)
{
	u8 ret=0;
	int i;
	
  /*!< Select the FLASH: Chip Select low */
  BCM_CS_LOW();
	//OSTimeDly(1);
	PLATFORM_OS_ENTER_CRITICAL();
	ret=BCM_WriteByte(wr_buf,wr_len);
// 	for( i=0;i<240;i++)
// 	{
// 		;
// 	}
	if(ret>0)
		BCM_ReadByte(rd_buf,rd_len);
	PLATFORM_OS_EXIT_CRITICAL();
  /*!< Deselect the FLASH: Chip Select high */
  BCM_CS_HIGH();
	//OSTimeDly(1);
	if(ret==0)
		return 1;
	else
		return 0;
}

/*********************************************************
** ��������aps_spi_write
** �䡡�룺None  
** �䡡����None
** ����������
************************************************************/
int32_t aps_spi_write(int32_t d,uint8_t *wr_buf,int32_t wr_len)
{
	u8 ret=0;
  /*!< Select the FLASH: Chip Select low */
  BCM_CS_LOW();
	//OSTimeDly(1);
	PLATFORM_OS_ENTER_CRITICAL();
	ret = BCM_WriteByte(wr_buf,wr_len);
	PLATFORM_OS_EXIT_CRITICAL();
  /*!< Deselect the FLASH: Chip Select high */
  BCM_CS_HIGH();
	//OSTimeDly(1);
	if(ret==0)
		return 1;
	else
		return 0;
}

