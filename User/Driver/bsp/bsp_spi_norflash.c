/***********************************************************
** 文件名:  flash.c
** Copyright (c) 1998-1999 天津华宁电子技术有限公司技术开发部
** 描　述:  电液控项目UCOS_II系统应用之flash芯片(W25QXX)驱动。
**-----------------------------------------------------------
*************************************************************/
#include "includes.h"
/***********************************************************
** 函数名：W25QXX_LowLevel_Init
** 输　入：None 
** 输　出：None
** 功能描述：Constrution of spi API, and initialize it.
** 全局变量：None
** 调用模块：None
************************************************************/
void W25QXX_LowLevel_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/* FLASH_AD_GPIO, FLASDH_CS_GPIO, FLASH_WP_GPIO Periph clock enable */
	RCC_AHB1PeriphClockCmd(FLASH_AD_GPIO_RCC 
							| FLASH_CS_GPIO_RCC 
							| FLASH_WP_GPIO_RCC,
							ENABLE);

	/*!< FLASH_AD_SPI Periph clock enable */
#if	FLASH_AD_SPI_APB == APB1
	RCC_APB1PeriphClockCmd(FLASH_AD_SPI_RCC, ENABLE); 
#else
	RCC_APB2PeriphClockCmd(FLASH_AD_SPI_RCC, ENABLE); 
#endif

	//设置SPI引脚复用功能
	GPIO_PinAFConfig(FLASH_SPI_PORT, FLASH_SPI_SCK_SOURCE, FLASH_SPI_GPIO_AF);
	GPIO_PinAFConfig(FLASH_SPI_PORT, FLASH_SPI_MISO_SOURCE, FLASH_SPI_GPIO_AF);
	GPIO_PinAFConfig(FLASH_SPI_PORT, FLASH_SPI_MOSI_SOURCE, FLASH_SPI_GPIO_AF);

	/* Configure FLASH_SPI_SCK_PIN in output pushpull mode */
	GPIO_InitStructure.GPIO_Pin = FLASH_SPI_SCK_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;	//GPIO_PuPd_DOWN;
	GPIO_Init(FLASH_SPI_PORT, &GPIO_InitStructure);

	/* Configure FLASH_SPI_MISO_PIN in output pushpull mode */
	GPIO_InitStructure.GPIO_Pin = FLASH_SPI_MISO_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_Init(FLASH_SPI_PORT, &GPIO_InitStructure);

	/* Configure FLASH_SPI_MOSI_PIN in input no pull mode */
	GPIO_InitStructure.GPIO_Pin = FLASH_SPI_MOSI_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	//GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_Init(FLASH_SPI_PORT, &GPIO_InitStructure);

	/* Configure FLASH_SPI_CS_PIN in output nopull mode */
	GPIO_InitStructure.GPIO_Pin = FLASH_SPI_CS_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(FLASH_SPI_CS_PORT, &GPIO_InitStructure);

	/* Configure FLASH_SPI_WP_PIN in output nopull mode */
	GPIO_InitStructure.GPIO_Pin = FLASH_SPI_WP_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(FLASH_SPI_WP_PORT, &GPIO_InitStructure);
}
/***********************************************************
** 函数名：W25QXX_SPI_Init
** 输　入：None  
** 输　出：None
** 功能描述：Initialize the SPI controller for the flash.
** 全局变量：None
** 调用模块：None
************************************************************/
void W25QXX_SPI_Init(void)
{
	SPI_InitTypeDef  SPI_InitStructure;

	SPI_Cmd(FLASH_AD_SPI, DISABLE);

	/* SPI configuration */
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
#if FLASH_AD_SPI_APB == APB1
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;//SPI speed: 328.125KHz
#else
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;//SPI speed: 328.125KHz
#endif
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_Init(FLASH_AD_SPI, &SPI_InitStructure);

	/* Enable the W25QXX_SPI  */
	SPI_Cmd(FLASH_AD_SPI, ENABLE);
}


//读取W25QXX的状态寄存器，W25QXX一共有3个状态寄存器
//状态寄存器1：
//BIT7  6   5   4   3   2   1   0
//SPR   RV  TB BP2 BP1 BP0 WEL BUSY
//SPR:默认0,状态寄存器保护位,配合WP使用
//TB,BP2,BP1,BP0:FLASH区域写保护设置
//WEL:写使能锁定
//BUSY:忙标记位(1,忙;0,空闲)
//默认:0x00
//状态寄存器2：
//BIT7  6   5   4   3   2   1   0
//SUS   CMP LB3 LB2 LB1 (R) QE  SRP1
//状态寄存器3：
//BIT7      6    5    4   3   2   1   0
//HOLD/RST  DRV1 DRV0 (R) (R) WPS ADP ADS
//regno:状态寄存器号，范:1~3
//返回值:状态寄存器值
u8 W25QXX_ReadSR(u8 regno)   
{  
	u8 byte=0,command=0; 
    switch(regno)
    {
        case 1:
            command=W25QXX_CMD_RDSR1;    //读状态寄存器1指令
            break;
        case 2:
            command=W25QXX_CMD_RDSR2;    //读状态寄存器2指令
            break;
        case 3:
            command=W25QXX_CMD_RDSR3;    //读状态寄存器3指令
            break;
        default:
            command=W25QXX_CMD_RDSR1;
            break;
    }    
	W25QXX_CS_LOW();                        //使能器件   
	W25QXX_SendByte(command);            //发送读取状态寄存器命令    
	byte=W25QXX_SendByte(W25QXX_DUMMY_BYTE);          //读取一个字节  
	W25QXX_CS_HIGH();                           //取消片选     
	return byte;   
}


/***********************************************************
** 函数名：W25QXX_Init
** 输　入：None  
** 输　出：None
** 功能描述：Initialize the flash.
** 全局变量：None
** 调用模块：None
************************************************************/
//u8  tmp[16]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
void W25QXX_Init(void)
{
	uint16_t type = 0;
	uint8_t temp;
	W25QXX_LowLevel_Init();

	/*!< Deselect the FLASH: Chip Select high */
	W25QXX_WP_LOW();
	W25QXX_WP_LOW();
	W25QXX_WP_LOW();
	W25QXX_WP_LOW();
	W25QXX_CS_HIGH();
	W25QXX_CS_HIGH();
	W25QXX_CS_HIGH();
	W25QXX_CS_HIGH();

	// SPI controller configuration
	W25QXX_SPI_Init();
	type = W25QXX_ReadID();
	if(type == 0X4019)
	{
		temp = W25QXX_ReadSR(3);
		if((temp&0X01)==0)			        //如果不是4字节地址模式,则进入4字节地址模式
		{
			W25QXX_CS_LOW(); 			        //选中
			W25QXX_SendByte(W25QXX_CMD_4BYTE);//发送进入4字节地址模式指令  
			W25QXX_CS_HIGH();       		        //取消片选   
		}
	}
}

//等待空闲
void W25QXX_Wait_Busy(void)   
{   
	while((W25QXX_ReadSR(1)&0x01)==0x01);   // 等待BUSY位清空
}
/***********************************************************
** 函数名：W25QXX_EraseSector
** 输　入：SectorAddr ---Easer the adress of sector.  
** 输　出：Result
** 功能描述：Easer a complete Sector (4KB) to 0xff.
** 全局变量：None
** 调用模块：None
************************************************************/

void W25QXX_Erase_Sector(u32 Dst_Addr)   
{  
	//监视falsh擦除情况,测试用   
 	//printf("fe:%x\r\n",Dst_Addr);	  
		Dst_Addr*=4096;
    W25QXX_Write_Enable();                  //SET WEL 	 
    W25QXX_Wait_Busy();   
		W25QXX_CS_LOW();                         //使能器件   
    W25QXX_SendByte(W25QXX_CMD_SE);   //发送扇区擦除指令 
//    if(W25QXX_TYPE==W25Q256)                //如果是W25Q256的话地址为4字节的，要发送最高8位
    {
        W25QXX_SendByte((u8)((Dst_Addr)>>24)); 
    }
    W25QXX_SendByte((u8)((Dst_Addr)>>16));  //发送24bit地址    
    W25QXX_SendByte((u8)((Dst_Addr)>>8));   
    W25QXX_SendByte((u8)Dst_Addr);  
		W25QXX_CS_HIGH();                         //取消片选     	      
    W25QXX_Wait_Busy();   				    //等待擦除完成
}  

/***********************************************************
** 函数名：W25QXX_WritePage
** 输　入：pBuffer ---The buffer containing the data to be writing to flash.
**         WriteAddr ---Address in flash for paunching the instrution of write.
**         NumByteToWrite ---The mount of bytes to be writed  
** 输　出：None
** 功能描述：Write an entire page of data to flash.
** 全局变量：None
** 调用模块：None
************************************************************/

void W25QXX_Write_Page(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite)
{
		u16 i;  
    W25QXX_Write_Enable();                  //SET WEL 
		W25QXX_CS_LOW();                        //使能器件   
    W25QXX_SendByte(W25QXX_CMD_WRITE);   //发送写页命令   
//    if(W25QXX_TYPE==W25Q256)                //如果是W25Q256的话地址为4字节的，要发送最高8位
    {
        W25QXX_SendByte((u8)((WriteAddr)>>24)); 
    }
    W25QXX_SendByte((u8)((WriteAddr)>>16)); //发送24bit地址    
    W25QXX_SendByte((u8)((WriteAddr)>>8));   
    W25QXX_SendByte((u8)WriteAddr);   
    for(i=0;i<NumByteToWrite;i++)W25QXX_SendByte(pBuffer[i]);//循环写数  
		W25QXX_CS_HIGH();                        //取消片选 
		W25QXX_Wait_Busy();					   //等待写入结束
} 

//无检验写SPI FLASH 
//必须确保所写的地址范围内的数据全部为0XFF,否则在非0XFF处写入的数据将失败!
//具有自动换页功能 
//在指定地址开始写入指定长度的数据,但是要确保地址不越界!
//pBuffer:数据存储区
//WriteAddr:开始写入的地址(24bit)
//NumByteToWrite:要写入的字节数(最大65535)
//CHECK OK
void W25QXX_Write_NoCheck(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite)   
{ 			 		 
	u16 pageremain;	   
	pageremain=256-WriteAddr%256; //单页剩余的字节数		 	    
	if(NumByteToWrite<=pageremain)pageremain=NumByteToWrite;//不大于256个字节
	while(1)
	{	   
		W25QXX_Write_Page(pBuffer,WriteAddr,pageremain);
		if(NumByteToWrite==pageremain)break;//写入结束了
	 	else //NumByteToWrite>pageremain
		{
			pBuffer+=pageremain;
			WriteAddr+=pageremain;	

			NumByteToWrite-=pageremain;			  //减去已经写入了的字节数
			if(NumByteToWrite>256)pageremain=256; //一次可以写入256个字节
			else pageremain=NumByteToWrite; 	  //不够256个字节了
		}
	};	    
} 


/***********************************************************
** 函数名：W25QXX_WriteBuffer
** 输　入：pBuffer ---The buffer containing the data to be writing to flash.
**         WriteAddr ---Address in flash for paunching the instrution of write.
**         NumByteToWrite ---The mount of bytes to be writed  
** 输　出：None
** 功能描述：Write a certain mount of data to flash.
** 全局变量：None
** 调用模块：None
************************************************************/
u8 W25QXX_BUFFER[4096];		
void W25QXX_Write(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite)   
{ 
	u32 secpos;
	u16 secoff;
	u16 secremain;	   
 	u16 i;    
	u8 * W25QXX_BUF;	  
	W25QXX_BUF=W25QXX_BUFFER;	     
 	secpos=WriteAddr/4096;//扇区地址  
	secoff=WriteAddr%4096;//在扇区内的偏移
	secremain=4096-secoff;//扇区剩余空间大小   
 	//printf("ad:%X,nb:%X\r\n",WriteAddr,NumByteToWrite);//测试用
 	if(NumByteToWrite<=secremain)secremain=NumByteToWrite;//不大于4096个字节
	while(1) 
	{	
		W25QXX_Read(W25QXX_BUF,secpos*4096,4096);//读出整个扇区的内容
		for(i=0;i<secremain;i++)//校验数据
		{
			if(W25QXX_BUF[secoff+i]!=0XFF)break;//需要擦除  	  
		}
		if(i<secremain)//需要擦除
		{
			W25QXX_Erase_Sector(secpos);//擦除这个扇区
			for(i=0;i<secremain;i++)	   //复制
			{
				W25QXX_BUF[i+secoff]=pBuffer[i];	  
			}
			W25QXX_Write_NoCheck(W25QXX_BUF,secpos*4096,4096);//写入整个扇区  

		}else W25QXX_Write_NoCheck(pBuffer,WriteAddr,secremain);//写已经擦除了的,直接写入扇区剩余区间. 				   
		if(NumByteToWrite==secremain)break;//写入结束了
		else//写入未结束
		{
			secpos++;//扇区地址增1
			secoff=0;//偏移位置为0 	 

		   	pBuffer+=secremain;  //指针偏移
			WriteAddr+=secremain;//写地址偏移	   
		   	NumByteToWrite-=secremain;				//字节数递减
			if(NumByteToWrite>4096)secremain=4096;	//下一个扇区还是写不完
			else secremain=NumByteToWrite;			//下一个扇区可以写完了
		}	 
	};	 
}



/***********************************************************
** 函数名：W25QXX_ReadBuffer
** 输　入：pBuffer ---The buffer containing the data read from flash.
**         ReadAddr ---Address in flash for paunching the instrution of read.
**         NumByteToRead ---The mount of bytes to be read  
** 输　出：None
** 功能描述：Read a certain mount of content in flash and write them to a fixed memory.
** 全局变量：None
** 调用模块：None
************************************************************/
u8 W25QXX_Read(u8* pBuffer, u32 ReadAddr, u16 NumByteToRead)
{
//	if((ReadAddr+NumByteToRead) + NumByteToRead > FLASH_ADDRESS_MAX) return ERROR_UP_OVERFLOW;


	/*!< Select the FLASH: Chip Select low */
//	W25QXX_WP_HIGH();
//	W25QXX_WP_HIGH();
//	W25QXX_WP_HIGH();
//	W25QXX_WP_HIGH();
	W25QXX_CS_LOW();
	W25QXX_CS_LOW();
	W25QXX_CS_LOW();
	W25QXX_CS_LOW();

	/*!< Send "Read from Memory " instruction */
	W25QXX_SendByte(W25QXX_CMD_READ);
	/*!< Send ReadAddr high nibble address byte to read from */
	W25QXX_SendByte((ReadAddr & 0xFF000000) >> 24);
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
	W25QXX_CS_HIGH();
	W25QXX_CS_HIGH();
	W25QXX_CS_HIGH();
//	W25QXX_WP_LOW();
//	W25QXX_WP_LOW();
//	W25QXX_WP_LOW();
//	W25QXX_WP_LOW();

	return SUCCESS;
}
/***********************************************************
** 函数名：W25QXX_ReadID
** 输　入：pBuffer1 ---None  
** 输　出：JEDEC ID
** 功能描述：Read the JEDEC ID(manufacturer id and device id)
** 全局变量：None
** 调用模块：None
************************************************************/
u32 W25QXX_ReadID(void)
{
  u32 Temp = 0, Temp0 = 0, Temp1 = 0, Temp2 = 0;

	/*!< Select the FLASH: Chip Select low */
	W25QXX_WP_HIGH();
	W25QXX_WP_HIGH();
	W25QXX_WP_HIGH();
	W25QXX_WP_HIGH();
	W25QXX_CS_LOW();
	W25QXX_CS_LOW();
	W25QXX_CS_LOW();
	W25QXX_CS_LOW();

	/*!< Send "RDID " instruction */
	W25QXX_SendByte(W25QXX_CMD_JEDECID);

	/*!< Read a byte from the FLASH */
	Temp0 = W25QXX_SendByte(0X00);

	/*!< Read a byte from the FLASH */
	Temp1 = W25QXX_SendByte(0XFF);

	/*!< Read a byte from the FLASH */
	Temp2 = W25QXX_SendByte(0XFF);

	/*!< Deselect the FLASH: Chip Select high */
	W25QXX_CS_HIGH();
	W25QXX_CS_HIGH();
	W25QXX_CS_HIGH();
	W25QXX_CS_HIGH();
	W25QXX_WP_LOW();
	W25QXX_WP_LOW();
	W25QXX_WP_LOW();
	W25QXX_WP_LOW();


	Temp = (Temp0 << 16) | (Temp1 << 8) | Temp2;
	return Temp;
}
/***********************************************************
** 函数名：W25QXX_ReadByte
** 输　入：None  
** 输　出：Byte which is read.
** 功能描述：Read a byte data from the spi bus.
** 全局变量：None
** 调用模块：None
************************************************************/
u8 W25QXX_ReadByte(void)
{
	return (W25QXX_SendByte(W25QXX_DUMMY_BYTE));
}
/***********************************************************
** 函数名：W25QXX_SendByte
** 输　入：byte --- 8-bit data being sent. 
** 输　出：8-bit data being received.
** 功能描述：Sending the 8-bit data.
** 全局变量：None
** 调用模块：None
************************************************************/
u8 W25QXX_SendByte(u8 byte)
{
	u32 i;
	
	/*!< Loop while DR register in not emplty */
	i = 0;
	while ((SPI_I2S_GetFlagStatus(FLASH_AD_SPI, SPI_I2S_FLAG_TXE) == RESET) && (i++<65536));

	/*!< Send byte through the SPI peripheral */
	SPI_I2S_SendData(FLASH_AD_SPI, byte);

	/*!< Wait to receive a byte */
	i = 0;
	while ((SPI_I2S_GetFlagStatus(FLASH_AD_SPI, SPI_I2S_FLAG_RXNE) == RESET) && (i++<65536));

	/*!< Return the byte read from the SPI bus */
	return SPI_I2S_ReceiveData(FLASH_AD_SPI);
}

/***********************************************************
** 函数名：W25QXX_WriteEnable
** 输　入：None  
** 输　出：None
** 功能描述：Send the writing command byte to prepare for writing.(Demonstrated in the protocol of SPI standard in Manual of W25QXX)
** 全局变量：None
** 调用模块：None
************************************************************/
void W25QXX_Write_Enable(void)
{
	/*!< Select the FLASH: Chip Select low */
	W25QXX_CS_LOW();

	/*!< Send "Write Enable" instruction */
	W25QXX_SendByte(W25QXX_CMD_WREN);

	/*!< Deselect the FLASH: Chip Select high */
	W25QXX_CS_HIGH();
}
/***********************************************************
** 函数名：W25QXX_WaitForWriteEnd
** 输　入：none  
** 输　出：none
** 功能描述：Waiting for the last writing operation finished.
** 全局变量：None
** 调用模块：None
************************************************************/
void W25QXX_WaitForWriteEnd(void)
{
	u8 flashstatus = 0;

	/*!< Select the FLASH: Chip Select low */
	W25QXX_CS_LOW();
	W25QXX_CS_LOW();
	W25QXX_CS_LOW();
	W25QXX_CS_LOW();

	/*!< Send "Read Status Register" instruction */
	W25QXX_SendByte(W25QXX_CMD_RDSR1);

	/*!< Loop as long as the memory is busy with a write cycle */
	do
	{
		/*!< Send a dummy byte to generate the clock needed by the FLASH
		and put the value of the status register in FLASH_Status variable */
		flashstatus = W25QXX_SendByte(W25QXX_DUMMY_BYTE);
	}
	while ((flashstatus & W25QXX_WIP_FLAG) == SET); /* Write in progress */

	/*!< Deselect the FLASH: Chip Select high */
	W25QXX_CS_HIGH();
	W25QXX_CS_HIGH();
	W25QXX_CS_HIGH();
	W25QXX_CS_HIGH();
}

