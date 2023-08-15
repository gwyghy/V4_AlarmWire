/********************************************************************************
* �ļ����ƣ�	SpiCan_1.c
* ��	�ߣ�	������   
* ��ǰ�汾��   	V1.1
* ������ڣ�    2015.04.23
* ��������: 	���SPI CAN 1��������ʵ���豸��ʼ�������ݶ�д�Ȳ�����
* ��ʷ��Ϣ��   
*           	�汾��Ϣ     ���ʱ��      ԭ����        ע��
*
*       >>>>  �ڹ����е�λ��  <<<<
*          	  3-Ӧ�ò�
*             2-Э���
*          ��  1-Ӳ��������
*********************************************************************************
* Copyright (c) 2014,������������޹�˾ All rights reserved.
*********************************************************************************/

/********************************************************************************
* .hͷ�ļ�
*********************************************************************************/
#include "includes.h"
#include "NativeCan_Driver.h"
#include "SPICan_Driver.h"
#include "Can_Bus.h"
#include "mcp2515.h"
/********************************************************************************
* #define�궨��
*********************************************************************************/
/********************************************************************************
* ��������
*********************************************************************************/
#define SPI_DEV_ININT_FAIL_SET  		(200)//��ʼ�������У��������޶�����ֵ���ж��豸��ʼ��ʧ�ܻ�λʧ��
#define SPI_DEV_INTPROC_TIMESET		10//�жϴ����������
#define SPICAN_DEV_NUM  2

static u8 u8SPICanIntFlag = 0x00;
static uint8_t SPICanCheckSendFlag = 1;//�����־λ

static uint16_t bauds[] = {50,80,100,125,200,250,400,500};
/*******************************************************************************************
**�������ƣ�SpiDevSetStatus
**�������ã�spi���ùܽ�״̬
**������������
**�����������
**ע�������
*******************************************************************************************/
void SpiDevSetStatus(SPI_TypeDef * spi, uint8_t status)
{
	uint8_t i;
	
	for(i = 0; i < 10; i++)
	{
		if(status == RESET)
		{
			if(spi == SPI1)
				GPIO_ResetBits(GPIO_SPICAN_1_RST, PIN_SPICAN_1_RST);
			else
				GPIO_ResetBits(GPIO_SPICAN_2_RST, PIN_SPICAN_2_RST);
		}
		else
		{
			if(spi == SPI1)
				GPIO_SetBits(GPIO_SPICAN_1_RST, PIN_SPICAN_1_RST);
			else
				GPIO_SetBits(GPIO_SPICAN_2_RST, PIN_SPICAN_2_RST);	
		}
	}
}

/*******************************************************************************************
**�������ƣ�SpiDevSellectStatus
**�������ã�����spi�ܽ�״̬
**������������
**�����������
**ע�������
*******************************************************************************************/
void SpiDevSellectStatus(SPI_TypeDef * spi, uint8_t status)
{
	uint8_t i;
	
	for(i = 0; i < 10; i++)
	{
		if(status == RESET)
		{
			if(spi == SPI1)
				GPIO_ResetBits(GPIO_SPICAN_1_CS, PIN_SPICAN_1_CS);
			else
				GPIO_ResetBits(GPIO_SPICAN_2_CS, PIN_SPICAN_2_CS);
		}
		else
		{
			if(spi == SPI1)
				GPIO_SetBits(GPIO_SPICAN_1_CS, PIN_SPICAN_1_CS);
			else
				GPIO_SetBits(GPIO_SPICAN_2_CS, PIN_SPICAN_2_CS);
		}
	}
}

/*******************************************************************************************
**�������ƣ�BspSpiPortInit
**�������ã�����SPI���ߵ�Ӳ���ӿڣ�ʱ�ӡ��ܽţ���Ӳ������
**������������
**�����������
**ע�������
*******************************************************************************************/
static u8 BspSpiPortInit(uint16_t device)
{	
	GPIO_InitTypeDef GPIO_InitStructure;
	SPI_InitTypeDef  SPI_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
 	NVIC_InitTypeDef NVIC_InitStructure;
	
	if(device == SPICAN1)
	{
		/********����ʱ��**********************************/
		RCC_SPICAN_1_APBxCmd();//ʹ��SPIʱ��
		RCC_AHB1PeriphClockCmd(RCC_SPICAN_1_SCLK | RCC_SPICAN_1_MOSI| RCC_SPICAN_1_MIS0| RCC_SPICAN_1_CS |RCC_SPICAN_1_RST | RCC_SPICAN_1_TX0RST | RCC_SPICAN_1_TX1RST | RCC_SPICAN_1_TX2RST,ENABLE); 
		RCC_AHB1PeriphClockCmd(RCC_SPICAN_1_INT | RCC_SPICAN_1_RX0BF | RCC_SPICAN_1_RX1BF, ENABLE); 
		RCC_SPICAN_1_AF_APBxCmd();	
		
		/*******����˿�����****************************/
		//SCLK:PA5  MISO:PA6  MOSI:PB5  	SPI�ڸ��ù��ܶ���
		GPIO_PinAFConfig(GPIO_SPICAN_1_SCLK, GPIO_SPICAN_1_PINSOURCE_SCLK, GPIO_SPICAN_1_AF_DEFINE);
		GPIO_PinAFConfig(GPIO_SPICAN_1_MISO, GPIO_SPICAN_1_PINSOURCE_MISO, GPIO_SPICAN_1_AF_DEFINE);
		GPIO_PinAFConfig(GPIO_SPICAN_1_MOSI, GPIO_SPICAN_1_PINSOURCE_MOSI, GPIO_SPICAN_1_AF_DEFINE);
	}
	else
	{
		/********����ʱ��**********************************/
		RCC_SPICAN_2_APBxCmd();//ʹ��SPIʱ��
		RCC_AHB1PeriphClockCmd(RCC_SPICAN_2_SCLK | RCC_SPICAN_2_MOSI| RCC_SPICAN_2_MIS0| RCC_SPICAN_2_CS |RCC_SPICAN_2_RST | RCC_SPICAN_2_TX0RST | RCC_SPICAN_2_TX1RST | RCC_SPICAN_2_TX2RST,ENABLE); 
		RCC_AHB1PeriphClockCmd(RCC_SPICAN_2_INT | RCC_SPICAN_2_RX0BF | RCC_SPICAN_2_RX1BF, ENABLE); 
		RCC_SPICAN_2_AF_APBxCmd();
		
		/*******����˿�����****************************/
		//SCLK:PA5  MISO:PA6  MOSI:PB5  	SPI�ڸ��ù��ܶ���
		GPIO_PinAFConfig(GPIO_SPICAN_2_SCLK, GPIO_SPICAN_2_PINSOURCE_SCLK, GPIO_SPICAN_2_AF_DEFINE);
		GPIO_PinAFConfig(GPIO_SPICAN_2_MISO, GPIO_SPICAN_2_PINSOURCE_MISO, GPIO_SPICAN_2_AF_DEFINE);
		GPIO_PinAFConfig(GPIO_SPICAN_2_MOSI, GPIO_SPICAN_2_PINSOURCE_MOSI, GPIO_SPICAN_2_AF_DEFINE);
	}
	

	/**SCLK***/
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;//GPIO_PuPd_UP
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	if(device == SPICAN1)
	{
		GPIO_InitStructure.GPIO_Pin = PIN_SPICAN_1_SCLK;
		GPIO_Init(GPIO_SPICAN_1_SCLK, &GPIO_InitStructure);
	}
	else
	{
		GPIO_InitStructure.GPIO_Pin = PIN_SPICAN_2_SCLK;
		GPIO_Init(GPIO_SPICAN_2_SCLK, &GPIO_InitStructure);	
	}

	/**MIS0***/
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	if(device == SPICAN1)
	{
		GPIO_InitStructure.GPIO_Pin = PIN_SPICAN_1_MISO;
		GPIO_Init(GPIO_SPICAN_1_MISO, &GPIO_InitStructure);
	}
	else
	{
		GPIO_InitStructure.GPIO_Pin = PIN_SPICAN_2_MISO;
		GPIO_Init(GPIO_SPICAN_2_MISO, &GPIO_InitStructure);
	}

	/**MOSI***/
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	if(device == SPICAN1)
	{	
		GPIO_InitStructure.GPIO_Pin   = PIN_SPICAN_1_MOSI;
		GPIO_Init(GPIO_SPICAN_1_MOSI, &GPIO_InitStructure);	
	}
	else
	{
		GPIO_InitStructure.GPIO_Pin   = PIN_SPICAN_2_MOSI;
		GPIO_Init(GPIO_SPICAN_2_MOSI, &GPIO_InitStructure);	
	}

	/*MCP2515-CS*/
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;//GPIO_PuPd_NOPULL;		
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	if(device == SPICAN1)
	{	
		GPIO_InitStructure.GPIO_Pin = PIN_SPICAN_1_CS;	
		GPIO_Init(GPIO_SPICAN_1_CS, &GPIO_InitStructure);

		GPIO_SetBits(GPIO_SPICAN_1_CS, PIN_SPICAN_1_CS);//cs ��ѡ��
	}
	else
	{
		GPIO_InitStructure.GPIO_Pin = PIN_SPICAN_2_CS;	
		GPIO_Init(GPIO_SPICAN_2_CS, &GPIO_InitStructure);

		GPIO_SetBits(GPIO_SPICAN_2_CS, PIN_SPICAN_2_CS);//cs ��ѡ��
	}

	 /**RESRT**/
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//�˿�ģʽΪ���������ʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		  
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;//GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//��ת�ٶ�
	if(device == SPICAN1)
	{	
	GPIO_InitStructure.GPIO_Pin =PIN_SPICAN_1_RST;		      //IO�˿�	  
	GPIO_Init(GPIO_SPICAN_1_RST, &GPIO_InitStructure); 	

	GPIO_SetBits(GPIO_SPICAN_1_RST, PIN_SPICAN_1_RST);//RST����λ
	}
	else
	{
		GPIO_InitStructure.GPIO_Pin =PIN_SPICAN_2_RST;		      //IO�˿�	  
		GPIO_Init(GPIO_SPICAN_2_RST, &GPIO_InitStructure); 	

		GPIO_SetBits(GPIO_SPICAN_2_RST, PIN_SPICAN_2_RST);//RST����λ
	}
	
	/***INT*****//**����Ŀ��ʹ�ô�������Ϊ�ⲿ�ж�****/
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;		  //��������	
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//GPIO_PuPd_NOPULL;//������
	if(device == SPICAN1)
	{		
		GPIO_InitStructure.GPIO_Pin = PIN_SPICAN_1_INT;	  //ѡ��ܽ�
		GPIO_Init(GPIO_SPICAN_1_INT, &GPIO_InitStructure);  //��ʼ��		

		/**��ʼ��INT��Ӧ���ⲿ�ж�****/
		/* Connect EXTI Line to INT Pin */
		SYSCFG_EXTILineConfig(SPICAN_1_INT_EXTI_PORT_SOURCE, SPICAN_1_INT_EXTI_PIN_SOURCE);
	}
	else
	{
		GPIO_InitStructure.GPIO_Pin = PIN_SPICAN_2_INT;	  //ѡ��ܽ�
		GPIO_Init(GPIO_SPICAN_2_INT, &GPIO_InitStructure);  //��ʼ��		

		/**��ʼ��INT��Ӧ���ⲿ�ж�****/
		/* Connect EXTI Line to INT Pin */
		SYSCFG_EXTILineConfig(SPICAN_2_INT_EXTI_PORT_SOURCE, SPICAN_2_INT_EXTI_PIN_SOURCE);
	}
	
	/* Configure EXTI line */	
	if(device == SPICAN1)
	{	
		EXTI_InitStructure.EXTI_Line    = SPICAN_1_INT_EXTI_LINE;
	}
	else
	{
		EXTI_InitStructure.EXTI_Line    = SPICAN_2_INT_EXTI_LINE;
	}
	
	EXTI_InitStructure.EXTI_Mode    = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;  
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);

	/* Enable and set the EXTI interrupt to priority 1*/
	if(device == SPICAN1)
	{	
		NVIC_InitStructure.NVIC_IRQChannel                   = SPICAN_1_INT_EXTI_IRQn;
	}
	else
	{
		NVIC_InitStructure.NVIC_IRQChannel                   = SPICAN_2_INT_EXTI_IRQn;
	}
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;//��ʹ���ж����ȼ�Ƕ�ס���ΪSysTick���ж����ȼ�Ϊ0x0f //1; 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;//0������SysTick //0;        //��Ӧ���ȼ�Ϊ 0
	NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
	NVIC_Init(&NVIC_InitStructure);  

	if(device == SPICAN1)
	 SPI_I2S_DeInit(SPICAN_1);
	else
	 SPI_I2S_DeInit(SPICAN_2);
	
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;   //ȫ˫��
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;  //8λ����ģʽ
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;         //����ģʽ��SCKΪ1
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;       //���ݲ����ӵ�2��ʱ����ؿ�ʼ
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;          //NSS�������
	SPI_InitStructure.SPI_BaudRatePrescaler = SPICAN_1_BAUNDRATE_PRESCALER;  //���÷�Ƶϵ��
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB; //���ģʽ
	SPI_InitStructure.SPI_CRCPolynomial = 7;           //CRC����ʽ
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;      //����ģʽ
	if(device == SPICAN1)
		SPI_Init(SPICAN_1, &SPI_InitStructure);
	else
		SPI_Init(SPICAN_2, &SPI_InitStructure);

	if(device == SPICAN1)
	{
	SPI_Cmd(SPICAN_1, DISABLE);			                       //�Ƚ�ֹSPI1	
	SPI_Cmd(SPICAN_1, ENABLE);
	}
	else
	{
		SPI_Cmd(SPICAN_2, DISABLE);			                       //�Ƚ�ֹSPI1	
		SPI_Cmd(SPICAN_2, ENABLE);
	}
	return 0x01;
}


/*
*********************************************************************************************************
*	�� �� ��: Spi1SendByte
*	����˵��: ����������һ���ֽڣ�ͬʱ��MISO���߲����������ص�����
*	��    ��:  _ucByte : ���͵��ֽ�ֵ
*	�� �� ֵ: ��MISO���߲����������ص�����
*********************************************************************************************************
*/
static u16 SpiSendByte(SPI_TypeDef * spi, u8 u8Value)
{
	u32 i;
	
	/* �ȴ��ϸ�����Ϊ������� */
	i = 0;
	while ((SPI_I2S_GetFlagStatus(spi, SPI_I2S_FLAG_TXE) == RESET) && (i++<65536));

	/* ͨ��SPIӲ������1���ֽ� */
	SPI_I2S_SendData(spi, u8Value);

	/* �ȴ�����һ���ֽ�������� */
	i = 0;
	while ((SPI_I2S_GetFlagStatus(spi, SPI_I2S_FLAG_RXNE) == RESET) && (i++<65536));

	/* ���ش�SPI���߶��������� */
	return SPI_I2S_ReceiveData(spi);
}
/*
*********************************************************************************************************
*	�� �� ��: Spi1ReadByte
*	����˵��: ��SPI������ȡһ���ֽ�����
*	��    ��: ��
*	�� �� ֵ: ��MISO���߲����������ص�����
*********************************************************************************************************
*/
static u16 SpiReadByte(SPI_TypeDef * spi)
{	
	return SpiSendByte(spi,0x00);
}

/*
*********************************************************************************************************
*	�� �� ��: Spi1WriteRegister
*	����˵��: дMCP2515оƬָ���Ĵ�������ֵ
*	��    ��: �Ĵ�����ַaddress �Ĵ���ֵvalue
*	�� �� ֵ:
*********************************************************************************************************
*/
static u8 SpiWriteRegister(SPI_TypeDef * spi, const u8 u8Address,const u8 u8Value)
{

	SpiDevSellectStatus(spi,RESET);
	SpiSendByte(spi, MCP_CMD_WRITE); 
	SpiSendByte(spi,u8Address);
	SpiSendByte(spi,u8Value);
	SpiDevSellectStatus(spi,SET);
	return 0x01;
}

/*
*********************************************************************************************************
*	�� �� ��: Spi1ReadRegister
*	����˵��: ��MCP2515оƬָ���Ĵ�������ֵ
*	��    ��: �Ĵ�����ַaddress
*	�� �� ֵ: ret
*********************************************************************************************************
*/
static u16 SpiReadRegister(SPI_TypeDef * spi, const u8 u8Address)
{
	unsigned int ret;
	
	SpiDevSellectStatus(spi,RESET);
	SpiSendByte(spi,MCP_CMD_READ);
	SpiSendByte(spi,u8Address);
	ret = SpiReadByte(spi);
	SpiDevSellectStatus(spi,SET);

	return ret;
}

/**********************************************************************************************************
*	�� �� ��: Spi1ModifyRegister
*	����˵��: �޸�MCP2515оƬָ���Ĵ���ָ��λ����ֵ
*	��    ��: �Ĵ�����ַaddress ����mask ����data
*	�� �� ֵ: 
**********************************************************************************************************/
static u8 SpiModifyRegister(SPI_TypeDef * spi, const u8 u8Address, const u8 u8Mask, const u8 u8Data)//��ѯ��ʽ
{
	SpiDevSellectStatus(spi,RESET);	
	SpiSendByte(spi,MCP_CMD_BIT_MODIFY);
	SpiSendByte(spi,u8Address);
	SpiSendByte(spi,u8Mask);
	SpiSendByte(spi,u8Data);
	SpiDevSellectStatus(spi,SET);
	
	return 0x01;
}


/*
*********************************************************************************************************
*	�� �� ��: Spi1SetTxb0Ctrl
*	����˵��: ����MCP2515�ķ��ͻ����������͹������ŵĹ���
*	��    ��: ��
*	�� �� ֵ: sta
*********************************************************************************************************
*/
static u8 SpiSetTxb0Ctrl(SPI_TypeDef * spi, s8 s8EnFlag)
{
	SpiWriteRegister(spi,MCP_REG_TXB0CTRL,s8EnFlag);     // ���żĴ����ڶ�����	
	return 0x01;
}	


/*
*********************************************************************************************************
*	�� �� ��: Spi1SetTxrstPinFun
*	����˵��: ����MCP2515�ķ��ͻ����������͹������ŵĹ���
*	��    ��: ��
*	�� �� ֵ: sta
*********************************************************************************************************
*/
static u8 SpiSetTxrstPinFun(SPI_TypeDef * spi, s8 s8EnFlag)
{
	SpiWriteRegister(spi, MCP_REG_TXRTSCTRL,s8EnFlag);     // ���żĴ����ڶ�����	
	return 0x01;
}	

/*
*********************************************************************************************************
*	�� �� ��: Spi1SetRxbufPinFun
*	����˵��: ����MCP2515�Ľ��ջ������жϹ������ŵĹ���
*	��    ��: ��
*	�� �� ֵ: sta
*********************************************************************************************************
*/
static u8 SpiSetRxbufPinFun(SPI_TypeDef * spi, s8 s8Funtion)
{
	SpiWriteRegister(spi, MCP_REG_BFPCTRL,s8Funtion);      // ���ſ��ƹ����������	
	return 0x01;
}


/*
*********************************************************************************************************
*	�� �� ��: Spi1SetIntEn
*	����˵��: ����MCP2515���ж�ʹ�ܼĴ�������ֵ
*	��    ��: ��
*	�� �� ֵ: sta
*********************************************************************************************************
*/
static u8 SpiSetIntEn(SPI_TypeDef * spi, s8 s8Interupt)
{
	SpiWriteRegister(spi, MCP_REG_CANINTE,s8Interupt);     //  
	return 0x01;
}


/*
*********************************************************************************************************
*	�� �� ��: Spi1SetRxb0Ctrl
*	����˵��: ����MCP2515��Rxb0Ctrl�Ĵ�������ֵ
*	��    ��: ��
*	�� �� ֵ: sta
*********************************************************************************************************
*/
static u8 SpiSetRxb0Ctrl(SPI_TypeDef * spi, u8 u8Funtion)
{
	SpiWriteRegister(spi, MCP_REG_RXB0CTRL,u8Funtion);      // ���ſ��ƹ����������	
	return 0x01;
}

/*
*********************************************************************************************************
*	�� �� ��: Spi1SetRxb1Ctrl
*	����˵��: ����MCP2515��Rxb1Ctrl�Ĵ�������ֵ
*	��    ��: ��
*	�� �� ֵ: sta
*********************************************************************************************************
*/
static u8 SpiSetRxb1Ctrl(SPI_TypeDef * spi, u8 uFuntion)
{
	SpiWriteRegister(spi, MCP_REG_RXB1CTRL,uFuntion);      // ���ſ��ƹ����������	
	return 0x01;
}

/*
*********************************************************************************************************
*	�� �� ��: Spi1ModifyIntFlag
*	����˵��: λ�޸�MCP2515���жϱ�־(CANINTF�ǿɶ�д��)
*	��    ��: ��
*	�� �� ֵ: sta
*********************************************************************************************************
*/
static u8 SpiModifyIntFlag(SPI_TypeDef * spi, const u8 u8Mask, const u8 u8Data)
{
	return SpiModifyRegister(spi, MCP_REG_CANINTF,u8Mask,u8Data);     //   
}

/*
*********************************************************************************************************
*	�� �� ��: Spi1SetCantrlDeviceMode
*	����˵��: ����MCP2515�Ĺ���ģʽ
*	��    ��: ��
*	�� �� ֵ: 1���ɹ���0��ʧ��
*********************************************************************************************************
*/
static u16 SpiSetCantrlDeviceMode(SPI_TypeDef * spi, MCP_CANCTRL_MODE_TYPE sMode)
{	
	u8 u8Mask = 0x00;//����
	u16 u16i = 0x00;
	u16 u16times = 0x00;
	
	u8Mask = (1<< MCP_REG_CANCTRL_REQOP2);
	u8Mask |= (1<< MCP_REG_CANCTRL_REQOP1);
	u8Mask |= (1<< MCP_REG_CANCTRL_REQOP0);
	
	if(sMode < MCP_CAN_MODE_MAX )
	{
		do
		{
			u16times ++;
			SpiModifyRegister(spi, MCP_REG_CANCTRL,u8Mask,sMode);//��λ�޸ĵķ�ʽ����д��������ֹ��д
			for(u16i = 0x00;u16i < 2000;u16i++)//�����깤��ģʽ����Ҫһ������ʱʱ��
				;	
			u16i = SpiReadRegister(spi, MCP_REG_CANCTRL);
		}while(((u16i&u8Mask) != sMode) && (u16times < SPI_DEV_ININT_FAIL_SET));//����ȷ��ģʽ�Ѿ�����
		if(u16times >= SPI_DEV_ININT_FAIL_SET)//�����޶�����ֵ
			return 0x00;
		else
			return 0x01;
	}
	else
	{
		return 0x00;
	}
}


/*
*********************************************************************************************************
*	�� �� ��: Spi1ModifyCantrl
*	����˵��: ����Cantrl�Ĵ�������������
*	��    ��: ��
*	�� �� ֵ: sta
*********************************************************************************************************
*/
static u8 SpiModifyCantrl(SPI_TypeDef * spi, const u8 u8Mask,const u8 u8Data)
{
	SpiModifyRegister(spi, MCP_REG_CANCTRL,u8Mask,u8Data);//��λ�޸ĵķ�ʽ����д��������ֹ��д
	
	return 0x01;
}

/*
*********************************************************************************************************
*	�� �� ��: Spi1SetCanBaudRate
*	����˵��: ����MCP2515��CAN��������
*	��    ��: ��
*	�� �� ֵ: sta
*********************************************************************************************************
*/
static u8 SpiSetCanBaudRate(SPI_TypeDef * spi, SPICAN_BAUNDRATE_TYPE sBaud)
{
	#if (SPICAN_1_USE_FOSC == SPICAN_1_FOSC_4M)
		/**���ݾ���Ϊ8M Hz,���ò����ʽ�������ʱ������***/
		u8 CAN_BAUDRATE_BUF[SPICAN_BAUDRATE_MAX][3] = 
		{	
			/**Ĭ��һ�β���**/
			{0x04,0x90,0x02},	 // 50Kbps,   BPR:4,  λʱ��8Tq,   SJW:1 Tq,������1 Tq:,   PS1: 3 Tq , PS2:3 Tq��������:62.5%
			{0x04,0x99,0x02},	 // 80Kbps,   BPR:4,  λʱ��10Tq,   SJW:1 Tq,������2 Tq:,   PS1: 4 Tq , PS2:3 Tq��������:70%
			{0x01,0x91,0x03},	 // 100Kbps  BPR:1,  λʱ��10Tq,   SJW:1 Tq,������2 Tq:,   PS1: 3 Tq , PS2:4 Tq��������:60%
			{0x01,0x90,0x02},	 // 125Kbps  BPR:1,  λʱ��8Tq,   SJW:1 Tq,������1 Tq:,   PS1: 3 Tq , PS2:3 Tq��������:62.5%

			{0x01,0x99,0x02},	 // 200Kbps  BPR:1,  λʱ��10Tq,   SJW:1 Tq,������2 Tq:,   PS1: 4 Tq , PS2:3 Tq��������:70%
			{0x00,0x90,0x02},	 // 250Kbps  BPR:0,  λʱ��8Tq,   SJW:1 Tq,������1 Tq:,   PS1: 3 Tq , PS2:3 Tq��������:62.5%
			{0x00,0x99,0x02},	 // 400Kbps  BPR:0,  λʱ��10Tq,   SJW:1 Tq,������2 Tq:,   PS1: 4 Tq , PS2:3 Tq��������:70%
			{0xFF,0xFF,0xFF}	 // 500Kbps ��Ч
		};		
	#elif  (SPICAN_1_USE_FOSC == SPICAN_1_FOSC_8M)
		/**���ݾ���Ϊ8M Hz,���ò����ʽ�������ʱ������***/
		u8 CAN_BAUDRATE_BUF[SPICAN_BAUDRATE_MAX][3] = 
		{	
			/**Ĭ��һ�β���**/
			{0x09,0x90,0x02},	 // 50Kbps,   BPR:9,  λʱ��8Tq,   SJW:1 Tq,������1 Tq:,   PS1: 3 Tq , PS2:3 Tq��������:62.5%
			{0x04,0x99,0x02},	 // 80Kbps,   BPR:4,  λʱ��10Tq,   SJW:1 Tq,������2 Tq:,   PS1: 4 Tq , PS2:3 Tq��������:70%
			{0x04,0x90,0x02},	 // 100Kbps  BPR:4,  λʱ��8Tq,   SJW:1 Tq,������1 Tq:,   PS1: 3 Tq , PS2:3 Tq��������:62.5%
			{0x03,0x90,0x02},	 // 125Kbps  BPR:3,  λʱ��8Tq,   SJW:1 Tq,������1 Tq:,   PS1: 3 Tq , PS2:3 Tq��������:62.5%

			{0x01,0x99,0x02},	 // 200Kbps  BPR:1,  λʱ��10Tq,   SJW:1 Tq,������2 Tq:,   PS1: 4 Tq , PS2:3 Tq��������:70%
			{0x01,0x90,0x02},	 // 250Kbps  BPR:1,  λʱ��8Tq,   SJW:1 Tq,������1 Tq:,   PS1: 3 Tq , PS2:3 Tq��������:62.5%
			{0x00,0x99,0x02},	 // 400Kbps  BPR:0,  λʱ��10Tq,   SJW:1 Tq,������2 Tq:,   PS1: 4 Tq , PS2:3 Tq��������:70%
			{0x00,0x90,0x02}	 // 500Kbps BPR:0,  λʱ��8Tq,   SJW:1 Tq,������1 Tq:,   PS1: 3 Tq , PS2:3 Tq��������:62.5%
		};	
	#elif (SPICAN_1_USE_FOSC == SPICAN_1_FOSC_16M)
		/**���ݾ���Ϊ16M Hz,���ò����ʽ�������ʱ������***/
		u8 CAN_BAUDRATE_BUF[MCP_CAN_BAUDRATE_MAX][3] = 
		{	
			/**Ĭ��һ�β���**/
			{0x13,0x90,0x02},	 // 50Kbps,   BPR:19,  λʱ��8Tq,   SJW:1 Tq,������1 Tq:,   PS1: 3 Tq , PS2:3 Tq��������:62.5%
			{0x09,0x99,0x02},	 // 80Kbps,   BPR:9,  λʱ��10Tq,   SJW:1 Tq,������2 Tq:,   PS1: 4 Tq , PS2:3 Tq��������:70%
			{0x09,0x90,0x02},	 // 100Kbps  BPR:9,  λʱ��8Tq,   SJW:1 Tq,������1 Tq:,   PS1: 3 Tq , PS2:3 Tq��������:62.5%
			{0x07,0x90,0x02},	 // 125Kbps  BPR:7,  λʱ��8Tq,   SJW:1 Tq,������1 Tq:,   PS1: 3 Tq , PS2:3 Tq��������:62.5%

			{0x03,0x99,0x02},	 // 200Kbps  BPR:3,  λʱ��10Tq,   SJW:1 Tq,������2 Tq:,   PS1: 4 Tq , PS2:3 Tq��������:70%
			{0x03,0x90,0x02},	 // 250Kbps  BPR:3,  λʱ��8Tq,   SJW:1 Tq,������1 Tq:,   PS1: 3 Tq , PS2:3 Tq��������:62.5%
			{0x01,0x99,0x02},	 // 400Kbps  BPR:1,  λʱ��10Tq,   SJW:1 Tq,������2 Tq:,   PS1: 4 Tq , PS2:3 Tq��������:70%
			{0x01,0x90,0x02}	 // 500Kbps BPR:1,  λʱ��8Tq,   SJW:1 Tq,������1 Tq:,   PS1: 3 Tq , PS2:3 Tq��������:62.5%
		};		
	#else
		return 0x00;
 	#endif
		
 	if(sBaud < SPICAN_BAUDRATE_MAX)
 	{
 		SpiWriteRegister(spi, MCP_REG_CNF1,CAN_BAUDRATE_BUF[(u8)(sBaud)][0]);
 		SpiWriteRegister(spi, MCP_REG_CNF2,CAN_BAUDRATE_BUF[(u8)(sBaud)][1]);
 		SpiWriteRegister(spi, MCP_REG_CNF3,CAN_BAUDRATE_BUF[(u8)(sBaud)][2]);	
 		return 0x01;
 	}
 	else
 		return 0x00;
}

/*
*********************************************************************************************************
*	�� �� ��: Spi1SetCanBaud
*	����˵��: ��װSpi1SetCanBaudRate����
*	��    ��: ��
*	�� �� ֵ: sta
*********************************************************************************************************
*/
static void SpiSetCanBaud(SPI_TypeDef * spi, uint16_t sBaud)
{
	uint8_t i;
	for(i = 0;i<sizeof(bauds);i++)
	{
		if(sBaud == bauds[i])
			break;
	}
	SpiSetCanBaudRate(spi, i);
}


/**********************************************************************************************************
*	�� �� ��: SpiCanMcp2515Init
*	����˵��: MCP2515������ʼ��
*	��    ��: ��
*	�� �� ֵ: 1���ɹ���0��ʧ��
**********************************************************************************************************/
uint16_t DriverSPICan_Init(uint16_t device, uint16_t sBaud)
{
	u16 u16i = 0x00;
	SPI_TypeDef * spi;
	if(device == SPICAN1)
		spi = SPI1;
	else
		spi = SPI2;
	
	BspSpiPortInit(device);
	
	SpiDevSetStatus(spi,SET);
	for(u16i = 0x00;u16i < 100;u16i++)
		;	
	SpiDevSetStatus(spi,RESET);//��λ
	for(u16i = 0x00;u16i < 5000;u16i++)//�˴�����ʱ�Ǳ���ģ���λʱ����СֵΪ2us
		;
	SpiDevSetStatus(spi,SET);

	u16i = SpiSetCantrlDeviceMode(spi,MCP_CAN_MODE_SETUPE);	// ����ģʽmcp2515_setCANCTRL_Mode(0X80);  
	if(u16i == 0x00)
		return u16i;
	
	/***���õ��η��ͣ�clkout��ֹ***/
	SpiModifyCantrl(spi, MCP_CANCTRL_OSM|MCP_CANCTRL_CLKOUT_ENABLE,  MCP_CANCTRL_OSM|MCP_CANCTRL_CLKOUT_DISABLE);
	SpiSetCanBaud(spi,sBaud); // ���ò�����
	/************��������*****************************************************************/
	/* RX0ʹ�ܹ��湦�ܣ��ҹر��˲����ܣ��������б��ġ�*/
	SpiSetRxb0Ctrl(spi, MCP_RXBNCTRL_FILTER_CLOSE|MCP_RXBNCTRL_BUKE_ENABLE);
	/*RX1�ر��˲����ܣ��������б��ġ�*/
	SpiSetRxb1Ctrl(spi, MCP_RXBNCTRL_FILTER_CLOSE);
	/**RXB0��RXB1�����ж����Ž�ֹ������INT�жϷ�ʽ**/
	SpiSetRxbufPinFun(spi, MCP_RXNBF_RX01BF_DISABLE);

	/************��������*****************************************************************/
	/*��ʹ�÷����������Ž������ݷ���,���÷�������������ݷ���**/
	SpiSetTxrstPinFun(spi, MCP_TXRSTCTRL_TX012RST_DISABLE); 
	/**ֻʹ�÷��ͻ�����0�������ݷ��ͣ�����������ʹ��***/
	SpiSetTxb0Ctrl(spi, MCP_TXBNCTRL_PRIOTY_HIGHEST);

	/**INT�ж�����:����1������0�������жϡ����ͼĴ�����****/
	SpiModifyIntFlag(spi, MCP_CANINTF_TX0IF,0x00);	
	SpiSetIntEn(spi, MCP_CANINTE_MERRE_ENABLE|MCP_CANINTE_RX1IE_ENABLE|MCP_CANINTE_RX0IE_ENABLE|MCP_CANINTE_ERRIE_ENABLE|MCP_CANINTE_TX0IE_ENABLE);
	//Spi1SetIntEn(MCP_CANINTE_RX1IE_ENABLE|MCP_CANINTE_RX0IE_ENABLE|MCP_CANINTE_ERRIE_ENABLE);
	
	u16i = SpiSetCantrlDeviceMode(spi, MCP_CAN_MODE_NORMAL);// ����ģʽ
	if(u16i == 0x00)
		return u16i;

	if(device == SPICAN1)
		NVIC_EnableIRQ(SPICAN_1_INT_EXTI_IRQn);//BSP_IntEn(SPICAN_1_INT_EXTI_IRQn);
	else
		NVIC_EnableIRQ(SPICAN_2_INT_EXTI_IRQn);//BSP_IntEn(SPICAN_2_INT_EXTI_IRQn);

	return 0x01;
}

/**********************************************************************************************************
*	�� �� ��: SpiCanReadFrameAll
*	����˵��: HUB ��ȡ���ջ������е�����֡��ȫ����Ϣ(����Ĭ�϶�ȡ13���Ĵ����ķ�ʽ)
*	��    ��: sRegType:�Ĵ�������
*	�� �� ֵ: 
**********************************************************************************************************/
static void SpiCanReadFrameAll(SPI_TypeDef * spi, MCP_READBUF_REG_TYPE sRegType,CanRxMsg *CanRxMsgTemp)
{
	u8 u8i = 0x00;
	u8 u8Rxbnsidh = 0x00;
	u8 u8Rxbnsidl = 0x00;
	u8 u8Rxbneid8 = 0x00;
	u8 u8Rxbneid0 = 0x00;
	u8 u8Rxbndlc = 0x00;
	
	SpiDevSellectStatus(spi,RESET);
	SpiSendByte(spi, MCP_CMD_RD_RX_BUFF | sRegType);//���ö�ȡ������ģʽ��

	u8Rxbnsidh = SpiSendByte(spi, MCP_READBUF_ANY_STATUS);
	u8Rxbnsidl = SpiSendByte(spi, MCP_READBUF_ANY_STATUS);
	u8Rxbneid8 = SpiSendByte(spi, MCP_READBUF_ANY_STATUS);
	u8Rxbneid0 = SpiSendByte(spi, MCP_READBUF_ANY_STATUS);
	u8Rxbndlc = SpiSendByte(spi, MCP_READBUF_ANY_STATUS);	
	for (u8i=0; u8i<(u8Rxbndlc&0x0F); u8i++) //��ȡ����
	{
		CanRxMsgTemp->Data[u8i] = SpiSendByte(spi,MCP_READBUF_ANY_STATUS);
	}
	
	/**�������϶�ȡ���ݵĴ���***/
	CanRxMsgTemp->DLC = (SPICAN_FRAME_LENGTH_TYPE)(u8Rxbndlc&0x0F);
	CanRxMsgTemp->IDE = (SPICAN_FRAME_ID_TYPE)((u8Rxbnsidl&0x08)>>3);
	if(CanRxMsgTemp->IDE == SPICAN_CANFRAME_STAND)//��׼֡
	{
		CanRxMsgTemp->StdId = (u32)(u8Rxbnsidh);
		CanRxMsgTemp->StdId <<= 0x03;
		CanRxMsgTemp->StdId |= (u32)((u8Rxbnsidl&0xE0)>>5);	
		CanRxMsgTemp->RTR = (u8Rxbnsidl&0x10)>>4;
	}
	else//��չ֡
	{
		CanRxMsgTemp->ExtId = u8Rxbneid0;
		CanRxMsgTemp->ExtId |= ((u32)(u8Rxbneid8)<<8);
		CanRxMsgTemp->ExtId |= (((u32)(u8Rxbnsidl)&0x03)<<16);
		CanRxMsgTemp->ExtId |= (((u32)(u8Rxbnsidl)&0xE0)<<13);
		CanRxMsgTemp->ExtId |= ((u32)(u8Rxbnsidh)<<21);
		CanRxMsgTemp->RTR = (u8Rxbndlc&0x40)>>6;
	}		
	SpiDevSellectStatus(spi,SET);
	/**��CS�ø�֮�󣬻��Զ������Ӧ���жϱ�־****/
}

/**********************************************************************************************************
*	�� �� ��: DriverSPICan_Read
*	����˵��: ��װSpiCanReadFrameAll����
*	��    ��: sRegType:�Ĵ�������
*	�� �� ֵ: 
**********************************************************************************************************/
uint16_t DriverSPICan_Read(uint16_t device, STR_Can_Frame* RxCan)
{
	uint16_t i;
	CanRxMsg  CanRxMsgTemp;
	SPI_TypeDef * spi;
	if(device == SPICAN1)
		spi = SPI1;
	else
		spi = SPI2;
	
	if((u8SPICanIntFlag & MCP_CANINTF_RX1IF) == MCP_CANINTF_RX1IF)//1�����ж�
		SpiCanReadFrameAll(spi, MCP_READBUF_BUF1,&CanRxMsgTemp);
	else if((u8SPICanIntFlag & MCP_CANINTF_RX1IF) == MCP_CANINTF_RX0IF)//0 �����ж�
		SpiCanReadFrameAll(spi, MCP_READBUF_BUF0,&CanRxMsgTemp);
	
	RxCan->ID = CanRxMsgTemp.ExtId;
	RxCan->DLC   = CanRxMsgTemp.DLC;
	if(RxCan->DLC >8)
	{
		return -1;
	}
	
	for(i = 0; i < CanRxMsgTemp.DLC; i++)
	{
		RxCan->Data[i] = CanRxMsgTemp.Data[i];
	} 
	return (RxCan->DLC);		
}




/**********************************************************************************************************
*	�� �� ��: SpiCan1RtsTxBuffer
*	����˵��: �����������ݷ���֡
*	��    ��: sRtsType�������͵�����BUF
*	�� �� ֵ: 0x01:��������ݷ��ͣ�0x00:�����ݽ��з���
**********************************************************************************************************/
static  u8 SpiCanRtsTxBuffer(SPI_TypeDef * spi, MCP_TXRTS_TYPE sRtsType)
{
	/**������****/
	SpiDevSellectStatus(spi, RESET);
	SpiSendByte(spi, MCP_CMD_RTS | sRtsType );//���ö�ȡ������ģʽ��
	SpiDevSellectStatus(spi, SET);
	SPICanCheckSendFlag = 1;

	return 0x01;
}

/**********************************************************************************************************
*	�� �� ��: SpiCan1LoadSendTxBuffer
*	����˵��: ���������е����ݽ���װ�ز����з���
*	��    ��: 
*	�� �� ֵ: 0x01:��������ݷ��ͣ�0x00:�����ݽ��з���
**********************************************************************************************************/
//����SPI��ѯ����ʽʱ�����ⲿ�ж���ֱ�ӽ������ݵĶ�ȡ
uint16_t DriverSPICan_Write(uint16_t device, STR_Can_Frame* TxCan)
{
	u8 u8i;
	u8 u8Txbnsidh;
	u8 u8Txbnsidl;
	u8 u8Txbneid8;
	u8 u8Txbneid0;
	u8 u8Txbndlc;
	
	SPI_TypeDef * spi;
	if(device == SPICAN1)
		spi = SPI1;
	else
		spi = SPI2;

	SPICanCheckSendFlag = 0;
	
	u8Txbnsidh = (u8)((TxCan->ID & 0x1FE00000)>>21);
	u8Txbnsidl = (u8)((TxCan->ID & 0x00030000)>>16);
	u8Txbnsidl |= (u8)((TxCan->ID & 0x001C0000)>>13);
	u8Txbnsidl |= (u8)(SPICAN_CANFRAME_EXTEND)<<3;
	u8Txbneid8 = (u8)((TxCan->ID & 0x0000FF00)>>8);
	u8Txbneid0 = (u8)(TxCan->ID & 0x000000FF);

	
	u8Txbndlc &= 0xF0;
	u8Txbndlc |= (TxCan->DLC&0x0F);
	
	SpiDevSellectStatus(spi, RESET);
	SpiModifyRegister(spi, MCP_REG_TXB0CTRL, MCP_TXBNCTRL_TXREQ,MCP_TXBNCTRL_CLEAR_TXBUF);
	SpiDevSellectStatus(spi, SET);
	
	SpiDevSellectStatus(spi, RESET);
	SpiSendByte(spi, MCP_CMD_LOAD_TX | MCP_TXLOAD_TXB0SIDH);//����װ�ػ�����ģʽ��
	SpiSendByte(spi, u8Txbnsidh);
	SpiSendByte(spi, u8Txbnsidl);
	SpiSendByte(spi, u8Txbneid8);
	SpiSendByte(spi, u8Txbneid0);
	SpiSendByte(spi, u8Txbndlc);	
	for(u8i = 0x00;u8i < (TxCan->DLC&0x0F); u8i++)
	{
		SpiSendByte(spi, TxCan->Data[u8i]);
	}
	SpiDevSellectStatus(spi, SET);
	
	SpiCanRtsTxBuffer(spi, MCP_TXRTS_TXB0);
	return 0x01;

}



/*******************************************************************************************
**�������ƣ�DriverSPICan_IoCtl
**�������ã�ԭ��canIO����
**������������
**�����������
**ע�������
*******************************************************************************************/
uint16_t  DriverSPICan_IoCtl(uint16_t device, uint8_t   func, void   *p_arg)
{
	SPI_TypeDef * spi;
	if(device == SPICAN1)
		spi = SPI1;
	else
		spi = SPI2;	
	
	
 	switch (func)
	{
		case CANCTL_SET_SEND_INT:
			SpiModifyIntFlag(spi,MCP_CANINTF_TX0IF,MCP_CANINTF_TX0IF);	//д���ͻ��������жϱ�־
			break;
		
		case CANCTL_CHECK_SEND_MAIL0:
						break;
			
		case CANCTL_ABORT_SEND_MAIL0:
			/**������е��жϱ�־�Լ������־***/
			SpiModifyRegister(spi,MCP_REG_CANINTF, 0xFF, 0x00);			
			SpiModifyRegister(spi,MCP_REG_EFLG, 0xFF, 0x00);
		
			break;

		case CANCTL_CHECK_SEND_TXOK0:

			if((SPICanCheckSendFlag) == 0)
				*(uint16_t *)p_arg = 0;
			else
				*(uint16_t *)p_arg = 1;
			break;
		
		default:
			break;
	}
	return(1);
}

/**********************************************************************************************************
*	�� �� ��: SpiCan1ErrIntProc
*	����˵��: ���д����жϴ���
*	��    ��: u8EflgFlag:�����־(�жϴ���ʽ��Ч)
*	�� �� ֵ: ��
**********************************************************************************************************/
static void SpiCanErrIntProc(SPI_TypeDef * spi, u8 u8EflgFlag)
{
	u8 u8IntFlag = 0x00;
	
	//����SPI��ѯ����ʽʱ�����ⲿ�ж���ֱ�ӽ������ݵĶ�ȡ
	u8IntFlag = SpiReadRegister(spi, MCP_REG_EFLG);//��ȡ�����־�Ĵ���

	
	if((u8IntFlag & (MCP_REG_EFLG_RX1OVR)) == (MCP_REG_EFLG_RX1OVR))//���������
	{
		/**��������־***/
		SpiModifyRegister(spi, MCP_REG_EFLG, MCP_REG_EFLG_RX1OVR, 0x00);
		/**�ڽ��������֮ǰ���Ѿ��Խ����жϱ�־�����˴��������ٴζ�ȡ***/
	}
	if((u8IntFlag & (MCP_REG_EFLG_RX0OVR)) == (MCP_REG_EFLG_RX0OVR))//���������
	{
		/**��������־***/
		SpiModifyRegister(spi,MCP_REG_EFLG, MCP_REG_EFLG_RX0OVR, 0x00);
		/**�ڽ��������֮ǰ���Ѿ��Խ����жϱ�־�����˴��������ٴζ�ȡ***/
	}	
	if(((u8IntFlag & (MCP_REG_EFLG_TXB0)) == (MCP_REG_EFLG_TXB0))    //���߹رգ����¶��������г�ʼ��
	   || ((u8IntFlag & (MCP_REG_EFLG_TXEP)) == (MCP_REG_EFLG_TXEP))   //���ͱ�������
	   || ((u8IntFlag & (MCP_REG_EFLG_RXEP)) == (MCP_REG_EFLG_RXEP))   //���ձ�������
	   || ((u8IntFlag & (MCP_REG_EFLG_EWARN)) == (MCP_REG_EFLG_EWARN))   //���󾯸�
	   || ((u8IntFlag & (MCP_REG_EFLG_TXWAR)) == (MCP_REG_EFLG_TXWAR))   //���ʹ��󾯸�
	   || ((u8IntFlag & (MCP_REG_EFLG_RXWAR)) == (MCP_REG_EFLG_RXWAR)))   //���մ��󾯸�	        	      
	{
		/**��������־***/
		SpiModifyRegister(spi, MCP_REG_EFLG, (MCP_REG_EFLG_TXB0|MCP_REG_EFLG_TXEP|MCP_REG_EFLG_RXEP|MCP_REG_EFLG_EWARN|MCP_REG_EFLG_TXWAR|MCP_REG_EFLG_RXWAR), 0x00);			
//		SpiCan1DeInt(SPICAN_1_CAN_BANDRATE_SET);   jhyjhy
	}
	
}



/**********************************************************************************************************
*	�� �� ��: SpiCan1IntExtiCheckoutProc
*	����˵��: HUB �ⲿ�ж�(INT)��Ӧ�Ĵ������
*	��    ��: �Ĵ�����ַaddress �Ĵ�������n ����values 
*	�� �� ֵ: 
**********************************************************************************************************/
static void SpiCanIntExtiCheckoutProc(SPI_TypeDef * spi)
{
	u8 u8ProcTimes;
	int16_t   res; 
	STR_Can_Frame RevBufTemp;
	uint16_t device;
	u8ProcTimes = 0x00;
	
	if(spi == SPI1)
		device = SPICAN1;
	else if(spi == SPI1)
		device = SPICAN2;
	
	do
	{
		/**��ȡ�жϱ�־***/
		u8SPICanIntFlag = SpiReadRegister(spi,MCP_REG_CANINTF);
		if(u8SPICanIntFlag == 0x00)
			break;			
		/**��ͬʱ��������ж�ʱ,INT�����ֵ͵�ƽֱ����λ���е��ж�****/
		/**INT�ж�����:���Ĵ��󡢷��ͻ�����0�ա�����1������0�������ж�****/
		/**�����жϴ���***/
		
		if((u8SPICanIntFlag & MCP_CANINTF_RX1IF) == MCP_CANINTF_RX1IF)//1�����ж�
		{
			//�ײ�ֱ�ӵ��ö�����    ע��MCP_READBUF_BUF1   jhyjhy
			res = DriverSPICan_Read(device,&RevBufTemp);
			//�������жϽ��
			if(res >= 0)
			{
				//�滺��
				CanBusDealFunc(device, CANBUS_MSG_InsCanRevQueue, &RevBufTemp);
			}
			//�ͷ��ź���
			
			//���ö���������ʽ����̧��CS��ƽ�󣬻��Զ�����жϱ�־,�����ֶ����
			SpiModifyIntFlag(spi, MCP_CANINTF_RX1IF,0x00);
		}
		
		if((u8SPICanIntFlag & MCP_CANINTF_RX0IF) == MCP_CANINTF_RX0IF)/*0�����ж�***/
		{
			res = DriverSPICan_Read(device,&RevBufTemp);
			//�������жϽ��
			if(res >= 0)
			{
				//�滺��
				CanBusDealFunc(device, CANBUS_MSG_InsCanRevQueue, &RevBufTemp);	;
			}
			//�ͷ��ź���
			
			//���ö���������ʽ����̧��CS��ƽ�󣬻��Զ�����жϱ�־,�����ֶ����
			SpiModifyIntFlag(spi, MCP_CANINTF_RX0IF,0x00);
		}
		
		/*�����ж�***/
		if((u8SPICanIntFlag & MCP_CANINTF_ERRIF) == MCP_CANINTF_ERRIF)
		{
			SpiModifyIntFlag(spi, MCP_CANINTF_ERRIF,0x00);
			SpiCanErrIntProc(spi,u8SPICanIntFlag);//���д����жϴ���	 
		}
		
		/*���Ĵ�����������*/
		if((u8SPICanIntFlag & MCP_CANINTF_MERRF) == MCP_CANINTF_MERRF)
		{
			SpiModifyIntFlag(spi, u8SPICanIntFlag|MCP_CANINTF_ERRIF,0x00);//2015.04.17,�޸�
			SpiCanErrIntProc(spi, u8SPICanIntFlag);//���д����жϴ���	��2015.04.17��������
		}
		
		/**�����жϵ������жϱ�־****/
		if( ((u8SPICanIntFlag & MCP_CANINTF_WAKEIF) == MCP_CANINTF_WAKEIF) \
		|| ((u8SPICanIntFlag & MCP_CANINTF_TX1IF) == MCP_CANINTF_TX1IF) \
		|| ((u8SPICanIntFlag & MCP_CANINTF_TX2IF) == MCP_CANINTF_TX2IF))
		{
			SpiModifyIntFlag(spi, u8SPICanIntFlag,0x00);
		}
		
		/**�����жϴ���***/
		if((u8SPICanIntFlag & MCP_CANINTF_TX0IF) == MCP_CANINTF_TX0IF)
		{		
			CanDriverTxDealFun(device,res);
		}
	}while(++u8ProcTimes < SPI_DEV_INTPROC_TIMESET);
	
	if(u8ProcTimes >= SPI_DEV_INTPROC_TIMESET)//�����趨��������������Ӳ����λ
	{
		/**������е��жϱ�־�Լ������־***/
		SpiModifyRegister(spi, MCP_REG_CANINTF, 0xFF, 0x00);			
		SpiModifyRegister(spi, MCP_REG_EFLG, 0xFF, 0x00);
					
//		SpiCan1DeInt(SPICAN_1_CAN_BANDRATE_SET);    jhyjhy
	}

}
	
 /**********************************************************************************************************
 *	�� �� ��: SPICAN_1_INT_EXTI_IRQn
 *	����˵��: HUB �ⲿ�ж�(INT)��Ӧ�Ĵ������
 *	��    ��: �Ĵ�����ַaddress �Ĵ�������n ����values 
 *	�� �� ֵ: 
 **********************************************************************************************************/
 void SPICAN_1_INT_EXTI_IRQHandler(void)
 {
 #if OS_CRITICAL_METHOD == 3                      /* Allocate storage for CPU status register */
 	OS_CPU_SR  cpu_sr = 0;
 #endif
 		
 	OS_ENTER_CRITICAL();
  	OSIntEnter();
 	OS_EXIT_CRITICAL();
 	
 	if(EXTI_GetITStatus(SPICAN_1_INT_EXTI_LINE) != RESET)
 	{
 		EXTI_ClearITPendingBit(SPICAN_1_INT_EXTI_LINE);
 		//����SPI��ѯ����ʽʱ�����ⲿ�ж���ֱ�ӽ������ݵĶ�ȡ
 		SpiCanIntExtiCheckoutProc(SPI1);
 	}
 	OSIntExit();	
 }
 
 
  /**********************************************************************************************************
 *	�� �� ��: SPICAN_2_INT_EXTI_IRQn
 *	����˵��: HUB �ⲿ�ж�(INT)��Ӧ�Ĵ������
 *	��    ��: �Ĵ�����ַaddress �Ĵ�������n ����values 
 *	�� �� ֵ: 
 **********************************************************************************************************/
 void SPICAN_2_INT_EXTI_IRQHandler(void)
 {
 #if OS_CRITICAL_METHOD == 3                      /* Allocate storage for CPU status register */
 	OS_CPU_SR  cpu_sr = 0;
 #endif
 		
 	OS_ENTER_CRITICAL();
  	OSIntEnter();
 	OS_EXIT_CRITICAL();
 	
 	if(EXTI_GetITStatus(SPICAN_2_INT_EXTI_LINE) != RESET)
 	{
 		EXTI_ClearITPendingBit(SPICAN_2_INT_EXTI_LINE);
 		//����SPI��ѯ����ʽʱ�����ⲿ�ж���ֱ�ӽ������ݵĶ�ȡ
 		SpiCanIntExtiCheckoutProc(SPI2);
 	}
 	OSIntExit();	
 }
 
 
 
 
 


