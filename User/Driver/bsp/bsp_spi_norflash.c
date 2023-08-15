/***********************************************************
** �ļ���:  flash.c
** Copyright (c) 1998-1999 ��������Ӽ������޹�˾����������
** �衡��:  ��Һ����ĿUCOS_IIϵͳӦ��֮flashоƬ(W25QXX)������
**-----------------------------------------------------------
*************************************************************/
#include "includes.h"
/***********************************************************
** ��������W25QXX_LowLevel_Init
** �䡡�룺None 
** �䡡����None
** ����������Constrution of spi API, and initialize it.
** ȫ�ֱ�����None
** ����ģ�飺None
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

	//����SPI���Ÿ��ù���
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
** ��������W25QXX_SPI_Init
** �䡡�룺None  
** �䡡����None
** ����������Initialize the SPI controller for the flash.
** ȫ�ֱ�����None
** ����ģ�飺None
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


//��ȡW25QXX��״̬�Ĵ�����W25QXXһ����3��״̬�Ĵ���
//״̬�Ĵ���1��
//BIT7  6   5   4   3   2   1   0
//SPR   RV  TB BP2 BP1 BP0 WEL BUSY
//SPR:Ĭ��0,״̬�Ĵ�������λ,���WPʹ��
//TB,BP2,BP1,BP0:FLASH����д��������
//WEL:дʹ������
//BUSY:æ���λ(1,æ;0,����)
//Ĭ��:0x00
//״̬�Ĵ���2��
//BIT7  6   5   4   3   2   1   0
//SUS   CMP LB3 LB2 LB1 (R) QE  SRP1
//״̬�Ĵ���3��
//BIT7      6    5    4   3   2   1   0
//HOLD/RST  DRV1 DRV0 (R) (R) WPS ADP ADS
//regno:״̬�Ĵ����ţ���:1~3
//����ֵ:״̬�Ĵ���ֵ
u8 W25QXX_ReadSR(u8 regno)   
{  
	u8 byte=0,command=0; 
    switch(regno)
    {
        case 1:
            command=W25QXX_CMD_RDSR1;    //��״̬�Ĵ���1ָ��
            break;
        case 2:
            command=W25QXX_CMD_RDSR2;    //��״̬�Ĵ���2ָ��
            break;
        case 3:
            command=W25QXX_CMD_RDSR3;    //��״̬�Ĵ���3ָ��
            break;
        default:
            command=W25QXX_CMD_RDSR1;
            break;
    }    
	W25QXX_CS_LOW();                        //ʹ������   
	W25QXX_SendByte(command);            //���Ͷ�ȡ״̬�Ĵ�������    
	byte=W25QXX_SendByte(W25QXX_DUMMY_BYTE);          //��ȡһ���ֽ�  
	W25QXX_CS_HIGH();                           //ȡ��Ƭѡ     
	return byte;   
}


/***********************************************************
** ��������W25QXX_Init
** �䡡�룺None  
** �䡡����None
** ����������Initialize the flash.
** ȫ�ֱ�����None
** ����ģ�飺None
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
		if((temp&0X01)==0)			        //�������4�ֽڵ�ַģʽ,�����4�ֽڵ�ַģʽ
		{
			W25QXX_CS_LOW(); 			        //ѡ��
			W25QXX_SendByte(W25QXX_CMD_4BYTE);//���ͽ���4�ֽڵ�ַģʽָ��  
			W25QXX_CS_HIGH();       		        //ȡ��Ƭѡ   
		}
	}
}

//�ȴ�����
void W25QXX_Wait_Busy(void)   
{   
	while((W25QXX_ReadSR(1)&0x01)==0x01);   // �ȴ�BUSYλ���
}
/***********************************************************
** ��������W25QXX_EraseSector
** �䡡�룺SectorAddr ---Easer the adress of sector.  
** �䡡����Result
** ����������Easer a complete Sector (4KB) to 0xff.
** ȫ�ֱ�����None
** ����ģ�飺None
************************************************************/

void W25QXX_Erase_Sector(u32 Dst_Addr)   
{  
	//����falsh�������,������   
 	//printf("fe:%x\r\n",Dst_Addr);	  
		Dst_Addr*=4096;
    W25QXX_Write_Enable();                  //SET WEL 	 
    W25QXX_Wait_Busy();   
		W25QXX_CS_LOW();                         //ʹ������   
    W25QXX_SendByte(W25QXX_CMD_SE);   //������������ָ�� 
//    if(W25QXX_TYPE==W25Q256)                //�����W25Q256�Ļ���ַΪ4�ֽڵģ�Ҫ�������8λ
    {
        W25QXX_SendByte((u8)((Dst_Addr)>>24)); 
    }
    W25QXX_SendByte((u8)((Dst_Addr)>>16));  //����24bit��ַ    
    W25QXX_SendByte((u8)((Dst_Addr)>>8));   
    W25QXX_SendByte((u8)Dst_Addr);  
		W25QXX_CS_HIGH();                         //ȡ��Ƭѡ     	      
    W25QXX_Wait_Busy();   				    //�ȴ��������
}  

/***********************************************************
** ��������W25QXX_WritePage
** �䡡�룺pBuffer ---The buffer containing the data to be writing to flash.
**         WriteAddr ---Address in flash for paunching the instrution of write.
**         NumByteToWrite ---The mount of bytes to be writed  
** �䡡����None
** ����������Write an entire page of data to flash.
** ȫ�ֱ�����None
** ����ģ�飺None
************************************************************/

void W25QXX_Write_Page(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite)
{
		u16 i;  
    W25QXX_Write_Enable();                  //SET WEL 
		W25QXX_CS_LOW();                        //ʹ������   
    W25QXX_SendByte(W25QXX_CMD_WRITE);   //����дҳ����   
//    if(W25QXX_TYPE==W25Q256)                //�����W25Q256�Ļ���ַΪ4�ֽڵģ�Ҫ�������8λ
    {
        W25QXX_SendByte((u8)((WriteAddr)>>24)); 
    }
    W25QXX_SendByte((u8)((WriteAddr)>>16)); //����24bit��ַ    
    W25QXX_SendByte((u8)((WriteAddr)>>8));   
    W25QXX_SendByte((u8)WriteAddr);   
    for(i=0;i<NumByteToWrite;i++)W25QXX_SendByte(pBuffer[i]);//ѭ��д��  
		W25QXX_CS_HIGH();                        //ȡ��Ƭѡ 
		W25QXX_Wait_Busy();					   //�ȴ�д�����
} 

//�޼���дSPI FLASH 
//����ȷ����д�ĵ�ַ��Χ�ڵ�����ȫ��Ϊ0XFF,�����ڷ�0XFF��д������ݽ�ʧ��!
//�����Զ���ҳ���� 
//��ָ����ַ��ʼд��ָ�����ȵ�����,����Ҫȷ����ַ��Խ��!
//pBuffer:���ݴ洢��
//WriteAddr:��ʼд��ĵ�ַ(24bit)
//NumByteToWrite:Ҫд����ֽ���(���65535)
//CHECK OK
void W25QXX_Write_NoCheck(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite)   
{ 			 		 
	u16 pageremain;	   
	pageremain=256-WriteAddr%256; //��ҳʣ����ֽ���		 	    
	if(NumByteToWrite<=pageremain)pageremain=NumByteToWrite;//������256���ֽ�
	while(1)
	{	   
		W25QXX_Write_Page(pBuffer,WriteAddr,pageremain);
		if(NumByteToWrite==pageremain)break;//д�������
	 	else //NumByteToWrite>pageremain
		{
			pBuffer+=pageremain;
			WriteAddr+=pageremain;	

			NumByteToWrite-=pageremain;			  //��ȥ�Ѿ�д���˵��ֽ���
			if(NumByteToWrite>256)pageremain=256; //һ�ο���д��256���ֽ�
			else pageremain=NumByteToWrite; 	  //����256���ֽ���
		}
	};	    
} 


/***********************************************************
** ��������W25QXX_WriteBuffer
** �䡡�룺pBuffer ---The buffer containing the data to be writing to flash.
**         WriteAddr ---Address in flash for paunching the instrution of write.
**         NumByteToWrite ---The mount of bytes to be writed  
** �䡡����None
** ����������Write a certain mount of data to flash.
** ȫ�ֱ�����None
** ����ģ�飺None
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
 	secpos=WriteAddr/4096;//������ַ  
	secoff=WriteAddr%4096;//�������ڵ�ƫ��
	secremain=4096-secoff;//����ʣ��ռ��С   
 	//printf("ad:%X,nb:%X\r\n",WriteAddr,NumByteToWrite);//������
 	if(NumByteToWrite<=secremain)secremain=NumByteToWrite;//������4096���ֽ�
	while(1) 
	{	
		W25QXX_Read(W25QXX_BUF,secpos*4096,4096);//������������������
		for(i=0;i<secremain;i++)//У������
		{
			if(W25QXX_BUF[secoff+i]!=0XFF)break;//��Ҫ����  	  
		}
		if(i<secremain)//��Ҫ����
		{
			W25QXX_Erase_Sector(secpos);//�����������
			for(i=0;i<secremain;i++)	   //����
			{
				W25QXX_BUF[i+secoff]=pBuffer[i];	  
			}
			W25QXX_Write_NoCheck(W25QXX_BUF,secpos*4096,4096);//д����������  

		}else W25QXX_Write_NoCheck(pBuffer,WriteAddr,secremain);//д�Ѿ������˵�,ֱ��д������ʣ������. 				   
		if(NumByteToWrite==secremain)break;//д�������
		else//д��δ����
		{
			secpos++;//������ַ��1
			secoff=0;//ƫ��λ��Ϊ0 	 

		   	pBuffer+=secremain;  //ָ��ƫ��
			WriteAddr+=secremain;//д��ַƫ��	   
		   	NumByteToWrite-=secremain;				//�ֽ����ݼ�
			if(NumByteToWrite>4096)secremain=4096;	//��һ����������д����
			else secremain=NumByteToWrite;			//��һ����������д����
		}	 
	};	 
}



/***********************************************************
** ��������W25QXX_ReadBuffer
** �䡡�룺pBuffer ---The buffer containing the data read from flash.
**         ReadAddr ---Address in flash for paunching the instrution of read.
**         NumByteToRead ---The mount of bytes to be read  
** �䡡����None
** ����������Read a certain mount of content in flash and write them to a fixed memory.
** ȫ�ֱ�����None
** ����ģ�飺None
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
** ��������W25QXX_ReadID
** �䡡�룺pBuffer1 ---None  
** �䡡����JEDEC ID
** ����������Read the JEDEC ID(manufacturer id and device id)
** ȫ�ֱ�����None
** ����ģ�飺None
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
** ��������W25QXX_ReadByte
** �䡡�룺None  
** �䡡����Byte which is read.
** ����������Read a byte data from the spi bus.
** ȫ�ֱ�����None
** ����ģ�飺None
************************************************************/
u8 W25QXX_ReadByte(void)
{
	return (W25QXX_SendByte(W25QXX_DUMMY_BYTE));
}
/***********************************************************
** ��������W25QXX_SendByte
** �䡡�룺byte --- 8-bit data being sent. 
** �䡡����8-bit data being received.
** ����������Sending the 8-bit data.
** ȫ�ֱ�����None
** ����ģ�飺None
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
** ��������W25QXX_WriteEnable
** �䡡�룺None  
** �䡡����None
** ����������Send the writing command byte to prepare for writing.(Demonstrated in the protocol of SPI standard in Manual of W25QXX)
** ȫ�ֱ�����None
** ����ģ�飺None
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
** ��������W25QXX_WaitForWriteEnd
** �䡡�룺none  
** �䡡����none
** ����������Waiting for the last writing operation finished.
** ȫ�ֱ�����None
** ����ģ�飺None
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

