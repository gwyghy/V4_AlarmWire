#include "includes.h"
#include "bsp_spi_551.h"


/*********************************************************
** 函数名：BCM_Init
** 输　入：None  
** 输　出：None
** 功能描述：Initialize the flash.
************************************************************/
static void BCM_CfgSPI(void)
{
	SPI_InitTypeDef  SPI_InitStructure;

	/* 打开SPI时钟 */
	ENABLE_BCMSPI_RCC();

	SPI_I2S_DeInit(SPI4);
	/* 配置SPI硬件参数 */
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;	/* 数据方向：2线全双工 */
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		                    /* STM32的SPI工作模式 ：主机模式 */
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;	                  /* 数据位长度 ： 8位 */
	/* SPI_CPOL和SPI_CPHA结合使用决定时钟和数据采样点的相位关系、
	   本例配置: 总线空闲是低电平,第1个边沿（上升沿采样数据)
	*/
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;			                    /* 时钟上升沿采样数据 */
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;		                    /* 时钟的第1个边沿采样数据 */
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;			                      /* 片选控制方式：软件控制 */

	/* 设置波特率预分频系数 */
	SPI_InitStructure.SPI_BaudRatePrescaler = BCM_SPI_CLK;

	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	                /* 数据位传输次序：高位先传 */
	SPI_InitStructure.SPI_CRCPolynomial = 7;			                      /* CRC多项式寄存器，复位后为7。本例程不用 */
	SPI_Init(BCM_SPI, &SPI_InitStructure);

	SPI_Cmd(BCM_SPI, DISABLE);			/* 先禁止SPI  */

	SPI_Cmd(BCM_SPI, ENABLE);				/* 使能SPI  */
}

int8_t BSP_BCMInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/* 使能GPIO 时钟 */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE|RCC_AHB1Periph_GPIOI|RCC_AHB1Periph_GPIOH, ENABLE);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;

	GPIO_InitStructure.GPIO_Pin = BCM_SPI_SCK_PIN | BCM_SPI_MISO_PIN | BCM_SPI_MOSI_PIN ;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	
		/* 配置 SCK, MISO 、 MOSI 为复用功能 */
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource2, GPIO_AF_SPI4);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource5, GPIO_AF_SPI4);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource6, GPIO_AF_SPI4);
	
	/* 配置片选口线为推挽输出模式 */	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Pin = BCM_CS_PIN  ;
	GPIO_Init(GPIOI, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = BCM_551CS_PIN;
	GPIO_Init(BCM_551CS_GPIO_PORT, &GPIO_InitStructure);
	
	BCM_CS_HIGH();		/* 片选置高，不选中 */
	BCM_551CS_HIGH();

	/* 配置SPI硬件参数用于访问串行Flash */
	BCM_CfgSPI();

	return 1;
}

/*********************************************************
** 函数名：BCM_WriteByte
** 输　入：byte --- 8-bit data being sent. 
** 输　出：8-bit data being received.
** 功能描述：Sending the 8-bit data.
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
** 函数名：BCM_ReadByte
** 输　入：byte --- 8-bit data being sent. 
** 输　出：8-bit data being received.
** 功能描述：Sending the 8-bit data.
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
** 函数名：aps_spi_read
** 输　入：None  
** 输　出：None
** 功能描述：
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
** 函数名：aps_spi_write
** 输　入：None  
** 输　出：None
** 功能描述：
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

