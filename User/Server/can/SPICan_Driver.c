/********************************************************************************
* 文件名称：	SpiCan_1.c
* 作	者：	马如意   
* 当前版本：   	V1.1
* 完成日期：    2015.04.23
* 功能描述: 	完成SPI CAN 1的驱动，实现设备初始化、数据读写等操作。
* 历史信息：   
*           	版本信息     完成时间      原作者        注释
*
*       >>>>  在工程中的位置  <<<<
*          	  3-应用层
*             2-协议层
*          √  1-硬件驱动层
*********************************************************************************
* Copyright (c) 2014,天津华宁电子有限公司 All rights reserved.
*********************************************************************************/

/********************************************************************************
* .h头文件
*********************************************************************************/
#include "includes.h"
#include "NativeCan_Driver.h"
#include "SPICan_Driver.h"
#include "Can_Bus.h"
#include "mcp2515.h"
/********************************************************************************
* #define宏定义
*********************************************************************************/
/********************************************************************************
* 常量定义
*********************************************************************************/
#define SPI_DEV_ININT_FAIL_SET  		(200)//初始化过程中，超过此限定次数值，判定设备初始化失败或复位失败
#define SPI_DEV_INTPROC_TIMESET		10//中断处理的最大次数
#define SPICAN_DEV_NUM  2

static u8 u8SPICanIntFlag = 0x00;
static uint8_t SPICanCheckSendFlag = 1;//错误标志位

static uint16_t bauds[] = {50,80,100,125,200,250,400,500};
/*******************************************************************************************
**函数名称：SpiDevSetStatus
**函数作用：spi设置管脚状态
**函数参数：无
**函数输出：无
**注意事项：无
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
**函数名称：SpiDevSellectStatus
**函数作用：设置spi管脚状态
**函数参数：无
**函数输出：无
**注意事项：无
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
**函数名称：BspSpiPortInit
**函数作用：配置SPI总线的硬件接口（时钟、管脚）和硬件参数
**函数参数：无
**函数输出：无
**注意事项：无
*******************************************************************************************/
static u8 BspSpiPortInit(uint16_t device)
{	
	GPIO_InitTypeDef GPIO_InitStructure;
	SPI_InitTypeDef  SPI_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
 	NVIC_InitTypeDef NVIC_InitStructure;
	
	if(device == SPICAN1)
	{
		/********配置时钟**********************************/
		RCC_SPICAN_1_APBxCmd();//使能SPI时钟
		RCC_AHB1PeriphClockCmd(RCC_SPICAN_1_SCLK | RCC_SPICAN_1_MOSI| RCC_SPICAN_1_MIS0| RCC_SPICAN_1_CS |RCC_SPICAN_1_RST | RCC_SPICAN_1_TX0RST | RCC_SPICAN_1_TX1RST | RCC_SPICAN_1_TX2RST,ENABLE); 
		RCC_AHB1PeriphClockCmd(RCC_SPICAN_1_INT | RCC_SPICAN_1_RX0BF | RCC_SPICAN_1_RX1BF, ENABLE); 
		RCC_SPICAN_1_AF_APBxCmd();	
		
		/*******输出端口配置****************************/
		//SCLK:PA5  MISO:PA6  MOSI:PB5  	SPI口复用功能定义
		GPIO_PinAFConfig(GPIO_SPICAN_1_SCLK, GPIO_SPICAN_1_PINSOURCE_SCLK, GPIO_SPICAN_1_AF_DEFINE);
		GPIO_PinAFConfig(GPIO_SPICAN_1_MISO, GPIO_SPICAN_1_PINSOURCE_MISO, GPIO_SPICAN_1_AF_DEFINE);
		GPIO_PinAFConfig(GPIO_SPICAN_1_MOSI, GPIO_SPICAN_1_PINSOURCE_MOSI, GPIO_SPICAN_1_AF_DEFINE);
	}
	else
	{
		/********配置时钟**********************************/
		RCC_SPICAN_2_APBxCmd();//使能SPI时钟
		RCC_AHB1PeriphClockCmd(RCC_SPICAN_2_SCLK | RCC_SPICAN_2_MOSI| RCC_SPICAN_2_MIS0| RCC_SPICAN_2_CS |RCC_SPICAN_2_RST | RCC_SPICAN_2_TX0RST | RCC_SPICAN_2_TX1RST | RCC_SPICAN_2_TX2RST,ENABLE); 
		RCC_AHB1PeriphClockCmd(RCC_SPICAN_2_INT | RCC_SPICAN_2_RX0BF | RCC_SPICAN_2_RX1BF, ENABLE); 
		RCC_SPICAN_2_AF_APBxCmd();
		
		/*******输出端口配置****************************/
		//SCLK:PA5  MISO:PA6  MOSI:PB5  	SPI口复用功能定义
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

		GPIO_SetBits(GPIO_SPICAN_1_CS, PIN_SPICAN_1_CS);//cs 不选中
	}
	else
	{
		GPIO_InitStructure.GPIO_Pin = PIN_SPICAN_2_CS;	
		GPIO_Init(GPIO_SPICAN_2_CS, &GPIO_InitStructure);

		GPIO_SetBits(GPIO_SPICAN_2_CS, PIN_SPICAN_2_CS);//cs 不选中
	}

	 /**RESRT**/
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//端口模式为推拉输出方式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		  
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;//GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//翻转速度
	if(device == SPICAN1)
	{	
	GPIO_InitStructure.GPIO_Pin =PIN_SPICAN_1_RST;		      //IO端口	  
	GPIO_Init(GPIO_SPICAN_1_RST, &GPIO_InitStructure); 	

	GPIO_SetBits(GPIO_SPICAN_1_RST, PIN_SPICAN_1_RST);//RST不复位
	}
	else
	{
		GPIO_InitStructure.GPIO_Pin =PIN_SPICAN_2_RST;		      //IO端口	  
		GPIO_Init(GPIO_SPICAN_2_RST, &GPIO_InitStructure); 	

		GPIO_SetBits(GPIO_SPICAN_2_RST, PIN_SPICAN_2_RST);//RST不复位
	}
	
	/***INT*****//**本项目中使用此引脚作为外部中断****/
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;		  //悬空输入	
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//GPIO_PuPd_NOPULL;//无上拉
	if(device == SPICAN1)
	{		
		GPIO_InitStructure.GPIO_Pin = PIN_SPICAN_1_INT;	  //选择管脚
		GPIO_Init(GPIO_SPICAN_1_INT, &GPIO_InitStructure);  //初始化		

		/**初始化INT对应的外部中断****/
		/* Connect EXTI Line to INT Pin */
		SYSCFG_EXTILineConfig(SPICAN_1_INT_EXTI_PORT_SOURCE, SPICAN_1_INT_EXTI_PIN_SOURCE);
	}
	else
	{
		GPIO_InitStructure.GPIO_Pin = PIN_SPICAN_2_INT;	  //选择管脚
		GPIO_Init(GPIO_SPICAN_2_INT, &GPIO_InitStructure);  //初始化		

		/**初始化INT对应的外部中断****/
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
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;//不使用中断优先级嵌套。因为SysTick的中断优先级为0x0f //1; 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;//0级用于SysTick //0;        //响应优先级为 0
	NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
	NVIC_Init(&NVIC_InitStructure);  

	if(device == SPICAN1)
	 SPI_I2S_DeInit(SPICAN_1);
	else
	 SPI_I2S_DeInit(SPICAN_2);
	
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;   //全双工
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;  //8位数据模式
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;         //空闲模式下SCK为1
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;       //数据采样从第2个时间边沿开始
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;          //NSS软件管理
	SPI_InitStructure.SPI_BaudRatePrescaler = SPICAN_1_BAUNDRATE_PRESCALER;  //设置分频系数
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB; //大端模式
	SPI_InitStructure.SPI_CRCPolynomial = 7;           //CRC多项式
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;      //主机模式
	if(device == SPICAN1)
		SPI_Init(SPICAN_1, &SPI_InitStructure);
	else
		SPI_Init(SPICAN_2, &SPI_InitStructure);

	if(device == SPICAN1)
	{
	SPI_Cmd(SPICAN_1, DISABLE);			                       //先禁止SPI1	
	SPI_Cmd(SPICAN_1, ENABLE);
	}
	else
	{
		SPI_Cmd(SPICAN_2, DISABLE);			                       //先禁止SPI1	
		SPI_Cmd(SPICAN_2, ENABLE);
	}
	return 0x01;
}


/*
*********************************************************************************************************
*	函 数 名: Spi1SendByte
*	功能说明: 向器件发送一个字节，同时从MISO口线采样器件返回的数据
*	形    参:  _ucByte : 发送的字节值
*	返 回 值: 从MISO口线采样器件返回的数据
*********************************************************************************************************
*/
static u16 SpiSendByte(SPI_TypeDef * spi, u8 u8Value)
{
	u32 i;
	
	/* 等待上个数据为发送完毕 */
	i = 0;
	while ((SPI_I2S_GetFlagStatus(spi, SPI_I2S_FLAG_TXE) == RESET) && (i++<65536));

	/* 通过SPI硬件发送1个字节 */
	SPI_I2S_SendData(spi, u8Value);

	/* 等待接收一个字节任务完成 */
	i = 0;
	while ((SPI_I2S_GetFlagStatus(spi, SPI_I2S_FLAG_RXNE) == RESET) && (i++<65536));

	/* 返回从SPI总线读到的数据 */
	return SPI_I2S_ReceiveData(spi);
}
/*
*********************************************************************************************************
*	函 数 名: Spi1ReadByte
*	功能说明: 从SPI器件读取一个字节数据
*	形    参: 无
*	返 回 值: 从MISO口线采样器件返回的数据
*********************************************************************************************************
*/
static u16 SpiReadByte(SPI_TypeDef * spi)
{	
	return SpiSendByte(spi,0x00);
}

/*
*********************************************************************************************************
*	函 数 名: Spi1WriteRegister
*	功能说明: 写MCP2515芯片指定寄存器的数值
*	形    参: 寄存器地址address 寄存器值value
*	返 回 值:
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
*	函 数 名: Spi1ReadRegister
*	功能说明: 读MCP2515芯片指定寄存器的数值
*	形    参: 寄存器地址address
*	返 回 值: ret
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
*	函 数 名: Spi1ModifyRegister
*	功能说明: 修改MCP2515芯片指定寄存器指定位的数值
*	形    参: 寄存器地址address 掩码mask 数据data
*	返 回 值: 
**********************************************************************************************************/
static u8 SpiModifyRegister(SPI_TypeDef * spi, const u8 u8Address, const u8 u8Mask, const u8 u8Data)//查询方式
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
*	函 数 名: Spi1SetTxb0Ctrl
*	功能说明: 设置MCP2515的发送缓冲器请求发送功能引脚的功能
*	形    参: 无
*	返 回 值: sta
*********************************************************************************************************
*/
static u8 SpiSetTxb0Ctrl(SPI_TypeDef * spi, s8 s8EnFlag)
{
	SpiWriteRegister(spi,MCP_REG_TXB0CTRL,s8EnFlag);     // 引脚寄存器第二功能	
	return 0x01;
}	


/*
*********************************************************************************************************
*	函 数 名: Spi1SetTxrstPinFun
*	功能说明: 设置MCP2515的发送缓冲器请求发送功能引脚的功能
*	形    参: 无
*	返 回 值: sta
*********************************************************************************************************
*/
static u8 SpiSetTxrstPinFun(SPI_TypeDef * spi, s8 s8EnFlag)
{
	SpiWriteRegister(spi, MCP_REG_TXRTSCTRL,s8EnFlag);     // 引脚寄存器第二功能	
	return 0x01;
}	

/*
*********************************************************************************************************
*	函 数 名: Spi1SetRxbufPinFun
*	功能说明: 设置MCP2515的接收缓冲器中断功能引脚的功能
*	形    参: 无
*	返 回 值: sta
*********************************************************************************************************
*/
static u8 SpiSetRxbufPinFun(SPI_TypeDef * spi, s8 s8Funtion)
{
	SpiWriteRegister(spi, MCP_REG_BFPCTRL,s8Funtion);      // 引脚控制功能数字输出	
	return 0x01;
}


/*
*********************************************************************************************************
*	函 数 名: Spi1SetIntEn
*	功能说明: 设置MCP2515的中断使能寄存器的数值
*	形    参: 无
*	返 回 值: sta
*********************************************************************************************************
*/
static u8 SpiSetIntEn(SPI_TypeDef * spi, s8 s8Interupt)
{
	SpiWriteRegister(spi, MCP_REG_CANINTE,s8Interupt);     //  
	return 0x01;
}


/*
*********************************************************************************************************
*	函 数 名: Spi1SetRxb0Ctrl
*	功能说明: 设置MCP2515的Rxb0Ctrl寄存器的数值
*	形    参: 无
*	返 回 值: sta
*********************************************************************************************************
*/
static u8 SpiSetRxb0Ctrl(SPI_TypeDef * spi, u8 u8Funtion)
{
	SpiWriteRegister(spi, MCP_REG_RXB0CTRL,u8Funtion);      // 引脚控制功能数字输出	
	return 0x01;
}

/*
*********************************************************************************************************
*	函 数 名: Spi1SetRxb1Ctrl
*	功能说明: 设置MCP2515的Rxb1Ctrl寄存器的数值
*	形    参: 无
*	返 回 值: sta
*********************************************************************************************************
*/
static u8 SpiSetRxb1Ctrl(SPI_TypeDef * spi, u8 uFuntion)
{
	SpiWriteRegister(spi, MCP_REG_RXB1CTRL,uFuntion);      // 引脚控制功能数字输出	
	return 0x01;
}

/*
*********************************************************************************************************
*	函 数 名: Spi1ModifyIntFlag
*	功能说明: 位修改MCP2515的中断标志(CANINTF是可读写的)
*	形    参: 无
*	返 回 值: sta
*********************************************************************************************************
*/
static u8 SpiModifyIntFlag(SPI_TypeDef * spi, const u8 u8Mask, const u8 u8Data)
{
	return SpiModifyRegister(spi, MCP_REG_CANINTF,u8Mask,u8Data);     //   
}

/*
*********************************************************************************************************
*	函 数 名: Spi1SetCantrlDeviceMode
*	功能说明: 设置MCP2515的工作模式
*	形    参: 无
*	返 回 值: 1：成功，0：失败
*********************************************************************************************************
*/
static u16 SpiSetCantrlDeviceMode(SPI_TypeDef * spi, MCP_CANCTRL_MODE_TYPE sMode)
{	
	u8 u8Mask = 0x00;//掩码
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
			SpiModifyRegister(spi, MCP_REG_CANCTRL,u8Mask,sMode);//以位修改的方式进行写操作，防止误写
			for(u16i = 0x00;u16i < 2000;u16i++)//设置完工作模式，需要一定的延时时间
				;	
			u16i = SpiReadRegister(spi, MCP_REG_CANCTRL);
		}while(((u16i&u8Mask) != sMode) && (u16times < SPI_DEV_ININT_FAIL_SET));//必须确认模式已经更改
		if(u16times >= SPI_DEV_ININT_FAIL_SET)//超过限定次数值
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
*	函 数 名: Spi1ModifyCantrl
*	功能说明: 设置Cantrl寄存器的其他内容
*	形    参: 无
*	返 回 值: sta
*********************************************************************************************************
*/
static u8 SpiModifyCantrl(SPI_TypeDef * spi, const u8 u8Mask,const u8 u8Data)
{
	SpiModifyRegister(spi, MCP_REG_CANCTRL,u8Mask,u8Data);//以位修改的方式进行写操作，防止误写
	
	return 0x01;
}

/*
*********************************************************************************************************
*	函 数 名: Spi1SetCanBaudRate
*	功能说明: 设置MCP2515的CAN发送速率
*	形    参: 无
*	返 回 值: sta
*********************************************************************************************************
*/
static u8 SpiSetCanBaudRate(SPI_TypeDef * spi, SPICAN_BAUNDRATE_TYPE sBaud)
{
	#if (SPICAN_1_USE_FOSC == SPICAN_1_FOSC_4M)
		/**依据晶振为8M Hz,设置波特率进行配置时的数组***/
		u8 CAN_BAUDRATE_BUF[SPICAN_BAUDRATE_MAX][3] = 
		{	
			/**默认一次采样**/
			{0x04,0x90,0x02},	 // 50Kbps,   BPR:4,  位时间8Tq,   SJW:1 Tq,传播段1 Tq:,   PS1: 3 Tq , PS2:3 Tq。采样点:62.5%
			{0x04,0x99,0x02},	 // 80Kbps,   BPR:4,  位时间10Tq,   SJW:1 Tq,传播段2 Tq:,   PS1: 4 Tq , PS2:3 Tq。采样点:70%
			{0x01,0x91,0x03},	 // 100Kbps  BPR:1,  位时间10Tq,   SJW:1 Tq,传播段2 Tq:,   PS1: 3 Tq , PS2:4 Tq。采样点:60%
			{0x01,0x90,0x02},	 // 125Kbps  BPR:1,  位时间8Tq,   SJW:1 Tq,传播段1 Tq:,   PS1: 3 Tq , PS2:3 Tq。采样点:62.5%

			{0x01,0x99,0x02},	 // 200Kbps  BPR:1,  位时间10Tq,   SJW:1 Tq,传播段2 Tq:,   PS1: 4 Tq , PS2:3 Tq。采样点:70%
			{0x00,0x90,0x02},	 // 250Kbps  BPR:0,  位时间8Tq,   SJW:1 Tq,传播段1 Tq:,   PS1: 3 Tq , PS2:3 Tq。采样点:62.5%
			{0x00,0x99,0x02},	 // 400Kbps  BPR:0,  位时间10Tq,   SJW:1 Tq,传播段2 Tq:,   PS1: 4 Tq , PS2:3 Tq。采样点:70%
			{0xFF,0xFF,0xFF}	 // 500Kbps 无效
		};		
	#elif  (SPICAN_1_USE_FOSC == SPICAN_1_FOSC_8M)
		/**依据晶振为8M Hz,设置波特率进行配置时的数组***/
		u8 CAN_BAUDRATE_BUF[SPICAN_BAUDRATE_MAX][3] = 
		{	
			/**默认一次采样**/
			{0x09,0x90,0x02},	 // 50Kbps,   BPR:9,  位时间8Tq,   SJW:1 Tq,传播段1 Tq:,   PS1: 3 Tq , PS2:3 Tq。采样点:62.5%
			{0x04,0x99,0x02},	 // 80Kbps,   BPR:4,  位时间10Tq,   SJW:1 Tq,传播段2 Tq:,   PS1: 4 Tq , PS2:3 Tq。采样点:70%
			{0x04,0x90,0x02},	 // 100Kbps  BPR:4,  位时间8Tq,   SJW:1 Tq,传播段1 Tq:,   PS1: 3 Tq , PS2:3 Tq。采样点:62.5%
			{0x03,0x90,0x02},	 // 125Kbps  BPR:3,  位时间8Tq,   SJW:1 Tq,传播段1 Tq:,   PS1: 3 Tq , PS2:3 Tq。采样点:62.5%

			{0x01,0x99,0x02},	 // 200Kbps  BPR:1,  位时间10Tq,   SJW:1 Tq,传播段2 Tq:,   PS1: 4 Tq , PS2:3 Tq。采样点:70%
			{0x01,0x90,0x02},	 // 250Kbps  BPR:1,  位时间8Tq,   SJW:1 Tq,传播段1 Tq:,   PS1: 3 Tq , PS2:3 Tq。采样点:62.5%
			{0x00,0x99,0x02},	 // 400Kbps  BPR:0,  位时间10Tq,   SJW:1 Tq,传播段2 Tq:,   PS1: 4 Tq , PS2:3 Tq。采样点:70%
			{0x00,0x90,0x02}	 // 500Kbps BPR:0,  位时间8Tq,   SJW:1 Tq,传播段1 Tq:,   PS1: 3 Tq , PS2:3 Tq。采样点:62.5%
		};	
	#elif (SPICAN_1_USE_FOSC == SPICAN_1_FOSC_16M)
		/**依据晶振为16M Hz,设置波特率进行配置时的数组***/
		u8 CAN_BAUDRATE_BUF[MCP_CAN_BAUDRATE_MAX][3] = 
		{	
			/**默认一次采样**/
			{0x13,0x90,0x02},	 // 50Kbps,   BPR:19,  位时间8Tq,   SJW:1 Tq,传播段1 Tq:,   PS1: 3 Tq , PS2:3 Tq。采样点:62.5%
			{0x09,0x99,0x02},	 // 80Kbps,   BPR:9,  位时间10Tq,   SJW:1 Tq,传播段2 Tq:,   PS1: 4 Tq , PS2:3 Tq。采样点:70%
			{0x09,0x90,0x02},	 // 100Kbps  BPR:9,  位时间8Tq,   SJW:1 Tq,传播段1 Tq:,   PS1: 3 Tq , PS2:3 Tq。采样点:62.5%
			{0x07,0x90,0x02},	 // 125Kbps  BPR:7,  位时间8Tq,   SJW:1 Tq,传播段1 Tq:,   PS1: 3 Tq , PS2:3 Tq。采样点:62.5%

			{0x03,0x99,0x02},	 // 200Kbps  BPR:3,  位时间10Tq,   SJW:1 Tq,传播段2 Tq:,   PS1: 4 Tq , PS2:3 Tq。采样点:70%
			{0x03,0x90,0x02},	 // 250Kbps  BPR:3,  位时间8Tq,   SJW:1 Tq,传播段1 Tq:,   PS1: 3 Tq , PS2:3 Tq。采样点:62.5%
			{0x01,0x99,0x02},	 // 400Kbps  BPR:1,  位时间10Tq,   SJW:1 Tq,传播段2 Tq:,   PS1: 4 Tq , PS2:3 Tq。采样点:70%
			{0x01,0x90,0x02}	 // 500Kbps BPR:1,  位时间8Tq,   SJW:1 Tq,传播段1 Tq:,   PS1: 3 Tq , PS2:3 Tq。采样点:62.5%
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
*	函 数 名: Spi1SetCanBaud
*	功能说明: 封装Spi1SetCanBaudRate函数
*	形    参: 无
*	返 回 值: sta
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
*	函 数 名: SpiCanMcp2515Init
*	功能说明: MCP2515器件初始化
*	形    参: 无
*	返 回 值: 1：成功，0：失败
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
	SpiDevSetStatus(spi,RESET);//复位
	for(u16i = 0x00;u16i < 5000;u16i++)//此处的延时是必须的，复位时间最小值为2us
		;
	SpiDevSetStatus(spi,SET);

	u16i = SpiSetCantrlDeviceMode(spi,MCP_CAN_MODE_SETUPE);	// 配置模式mcp2515_setCANCTRL_Mode(0X80);  
	if(u16i == 0x00)
		return u16i;
	
	/***启用单次发送，clkout禁止***/
	SpiModifyCantrl(spi, MCP_CANCTRL_OSM|MCP_CANCTRL_CLKOUT_ENABLE,  MCP_CANCTRL_OSM|MCP_CANCTRL_CLKOUT_DISABLE);
	SpiSetCanBaud(spi,sBaud); // 配置波特率
	/************接收配置*****************************************************************/
	/* RX0使能滚存功能，且关闭滤波功能，接收所有报文。*/
	SpiSetRxb0Ctrl(spi, MCP_RXBNCTRL_FILTER_CLOSE|MCP_RXBNCTRL_BUKE_ENABLE);
	/*RX1关闭滤波功能，接收所有报文。*/
	SpiSetRxb1Ctrl(spi, MCP_RXBNCTRL_FILTER_CLOSE);
	/**RXB0、RXB1接收中断引脚禁止，采用INT中断方式**/
	SpiSetRxbufPinFun(spi, MCP_RXNBF_RX01BF_DISABLE);

	/************发送配置*****************************************************************/
	/*不使用发送输入引脚进行数据发送,采用发送命令进行数据发送**/
	SpiSetTxrstPinFun(spi, MCP_TXRSTCTRL_TX012RST_DISABLE); 
	/**只使用发送缓冲区0进行数据发送，其他两个不使用***/
	SpiSetTxb0Ctrl(spi, MCP_TXBNCTRL_PRIOTY_HIGHEST);

	/**INT中断设置:接收1、接收0、错误中断、发送寄存器空****/
	SpiModifyIntFlag(spi, MCP_CANINTF_TX0IF,0x00);	
	SpiSetIntEn(spi, MCP_CANINTE_MERRE_ENABLE|MCP_CANINTE_RX1IE_ENABLE|MCP_CANINTE_RX0IE_ENABLE|MCP_CANINTE_ERRIE_ENABLE|MCP_CANINTE_TX0IE_ENABLE);
	//Spi1SetIntEn(MCP_CANINTE_RX1IE_ENABLE|MCP_CANINTE_RX0IE_ENABLE|MCP_CANINTE_ERRIE_ENABLE);
	
	u16i = SpiSetCantrlDeviceMode(spi, MCP_CAN_MODE_NORMAL);// 正常模式
	if(u16i == 0x00)
		return u16i;

	if(device == SPICAN1)
		NVIC_EnableIRQ(SPICAN_1_INT_EXTI_IRQn);//BSP_IntEn(SPICAN_1_INT_EXTI_IRQn);
	else
		NVIC_EnableIRQ(SPICAN_2_INT_EXTI_IRQn);//BSP_IntEn(SPICAN_2_INT_EXTI_IRQn);

	return 0x01;
}

/**********************************************************************************************************
*	函 数 名: SpiCanReadFrameAll
*	功能说明: HUB 读取接收缓冲区中的数据帧的全部信息(采用默认读取13个寄存器的方式)
*	形    参: sRegType:寄存器类型
*	返 回 值: 
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
	SpiSendByte(spi, MCP_CMD_RD_RX_BUFF | sRegType);//采用读取缓冲器模式，

	u8Rxbnsidh = SpiSendByte(spi, MCP_READBUF_ANY_STATUS);
	u8Rxbnsidl = SpiSendByte(spi, MCP_READBUF_ANY_STATUS);
	u8Rxbneid8 = SpiSendByte(spi, MCP_READBUF_ANY_STATUS);
	u8Rxbneid0 = SpiSendByte(spi, MCP_READBUF_ANY_STATUS);
	u8Rxbndlc = SpiSendByte(spi, MCP_READBUF_ANY_STATUS);	
	for (u8i=0; u8i<(u8Rxbndlc&0x0F); u8i++) //读取数据
	{
		CanRxMsgTemp->Data[u8i] = SpiSendByte(spi,MCP_READBUF_ANY_STATUS);
	}
	
	/**进行以上读取数据的处理***/
	CanRxMsgTemp->DLC = (SPICAN_FRAME_LENGTH_TYPE)(u8Rxbndlc&0x0F);
	CanRxMsgTemp->IDE = (SPICAN_FRAME_ID_TYPE)((u8Rxbnsidl&0x08)>>3);
	if(CanRxMsgTemp->IDE == SPICAN_CANFRAME_STAND)//标准帧
	{
		CanRxMsgTemp->StdId = (u32)(u8Rxbnsidh);
		CanRxMsgTemp->StdId <<= 0x03;
		CanRxMsgTemp->StdId |= (u32)((u8Rxbnsidl&0xE0)>>5);	
		CanRxMsgTemp->RTR = (u8Rxbnsidl&0x10)>>4;
	}
	else//扩展帧
	{
		CanRxMsgTemp->ExtId = u8Rxbneid0;
		CanRxMsgTemp->ExtId |= ((u32)(u8Rxbneid8)<<8);
		CanRxMsgTemp->ExtId |= (((u32)(u8Rxbnsidl)&0x03)<<16);
		CanRxMsgTemp->ExtId |= (((u32)(u8Rxbnsidl)&0xE0)<<13);
		CanRxMsgTemp->ExtId |= ((u32)(u8Rxbnsidh)<<21);
		CanRxMsgTemp->RTR = (u8Rxbndlc&0x40)>>6;
	}		
	SpiDevSellectStatus(spi,SET);
	/**在CS置高之后，会自动清除相应的中断标志****/
}

/**********************************************************************************************************
*	函 数 名: DriverSPICan_Read
*	功能说明: 封装SpiCanReadFrameAll函数
*	形    参: sRegType:寄存器类型
*	返 回 值: 
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
	
	if((u8SPICanIntFlag & MCP_CANINTF_RX1IF) == MCP_CANINTF_RX1IF)//1接收中断
		SpiCanReadFrameAll(spi, MCP_READBUF_BUF1,&CanRxMsgTemp);
	else if((u8SPICanIntFlag & MCP_CANINTF_RX1IF) == MCP_CANINTF_RX0IF)//0 接收中断
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
*	函 数 名: SpiCan1RtsTxBuffer
*	功能说明: 发送请求数据发送帧
*	形    参: sRtsType：请求发送的数据BUF
*	返 回 值: 0x01:需进行数据发送，0x00:无数据进行发送
**********************************************************************************************************/
static  u8 SpiCanRtsTxBuffer(SPI_TypeDef * spi, MCP_TXRTS_TYPE sRtsType)
{
	/**请求发送****/
	SpiDevSellectStatus(spi, RESET);
	SpiSendByte(spi, MCP_CMD_RTS | sRtsType );//采用读取缓冲器模式，
	SpiDevSellectStatus(spi, SET);
	SPICanCheckSendFlag = 1;

	return 0x01;
}

/**********************************************************************************************************
*	函 数 名: SpiCan1LoadSendTxBuffer
*	功能说明: 将缓冲区中的数据进行装载并进行发送
*	形    参: 
*	返 回 值: 0x01:需进行数据发送，0x00:无数据进行发送
**********************************************************************************************************/
//采用SPI查询处理方式时，在外部中断中直接进行数据的读取
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
	SpiSendByte(spi, MCP_CMD_LOAD_TX | MCP_TXLOAD_TXB0SIDH);//采用装载缓冲区模式，
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
**函数名称：DriverSPICan_IoCtl
**函数作用：原生canIO控制
**函数参数：无
**函数输出：无
**注意事项：无
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
			SpiModifyIntFlag(spi,MCP_CANINTF_TX0IF,MCP_CANINTF_TX0IF);	//写发送缓冲器空中断标志
			break;
		
		case CANCTL_CHECK_SEND_MAIL0:
						break;
			
		case CANCTL_ABORT_SEND_MAIL0:
			/**清除所有的中断标志以及错误标志***/
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
*	函 数 名: SpiCan1ErrIntProc
*	功能说明: 进行错误中断处理
*	形    参: u8EflgFlag:错误标志(中断处理方式有效)
*	返 回 值: 无
**********************************************************************************************************/
static void SpiCanErrIntProc(SPI_TypeDef * spi, u8 u8EflgFlag)
{
	u8 u8IntFlag = 0x00;
	
	//采用SPI查询处理方式时，在外部中断中直接进行数据的读取
	u8IntFlag = SpiReadRegister(spi, MCP_REG_EFLG);//读取错误标志寄存器

	
	if((u8IntFlag & (MCP_REG_EFLG_RX1OVR)) == (MCP_REG_EFLG_RX1OVR))//接收器溢出
	{
		/**清除错误标志***/
		SpiModifyRegister(spi, MCP_REG_EFLG, MCP_REG_EFLG_RX1OVR, 0x00);
		/**在进入错误处理之前，已经对接收中断标志进行了处理，无需再次读取***/
	}
	if((u8IntFlag & (MCP_REG_EFLG_RX0OVR)) == (MCP_REG_EFLG_RX0OVR))//接收器溢出
	{
		/**清除错误标志***/
		SpiModifyRegister(spi,MCP_REG_EFLG, MCP_REG_EFLG_RX0OVR, 0x00);
		/**在进入错误处理之前，已经对接收中断标志进行了处理，无需再次读取***/
	}	
	if(((u8IntFlag & (MCP_REG_EFLG_TXB0)) == (MCP_REG_EFLG_TXB0))    //总线关闭，重新对器件进行初始化
	   || ((u8IntFlag & (MCP_REG_EFLG_TXEP)) == (MCP_REG_EFLG_TXEP))   //发送被动错误
	   || ((u8IntFlag & (MCP_REG_EFLG_RXEP)) == (MCP_REG_EFLG_RXEP))   //接收被动错误
	   || ((u8IntFlag & (MCP_REG_EFLG_EWARN)) == (MCP_REG_EFLG_EWARN))   //错误警告
	   || ((u8IntFlag & (MCP_REG_EFLG_TXWAR)) == (MCP_REG_EFLG_TXWAR))   //发送错误警告
	   || ((u8IntFlag & (MCP_REG_EFLG_RXWAR)) == (MCP_REG_EFLG_RXWAR)))   //接收错误警告	        	      
	{
		/**清除错误标志***/
		SpiModifyRegister(spi, MCP_REG_EFLG, (MCP_REG_EFLG_TXB0|MCP_REG_EFLG_TXEP|MCP_REG_EFLG_RXEP|MCP_REG_EFLG_EWARN|MCP_REG_EFLG_TXWAR|MCP_REG_EFLG_RXWAR), 0x00);			
//		SpiCan1DeInt(SPICAN_1_CAN_BANDRATE_SET);   jhyjhy
	}
	
}



/**********************************************************************************************************
*	函 数 名: SpiCan1IntExtiCheckoutProc
*	功能说明: HUB 外部中断(INT)对应的处理过程
*	形    参: 寄存器地址address 寄存器数量n 数组values 
*	返 回 值: 
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
		/**读取中断标志***/
		u8SPICanIntFlag = SpiReadRegister(spi,MCP_REG_CANINTF);
		if(u8SPICanIntFlag == 0x00)
			break;			
		/**当同时发生多个中断时,INT将保持低电平直至复位所有的中断****/
		/**INT中断设置:报文错误、发送缓冲区0空、接收1、接收0、错误中断****/
		/**接收中断处理***/
		
		if((u8SPICanIntFlag & MCP_CANINTF_RX1IF) == MCP_CANINTF_RX1IF)//1接收中断
		{
			//底层直接调用读函数    注意MCP_READBUF_BUF1   jhyjhy
			res = DriverSPICan_Read(device,&RevBufTemp);
			//读数据判断结果
			if(res >= 0)
			{
				//存缓存
				CanBusDealFunc(device, CANBUS_MSG_InsCanRevQueue, &RevBufTemp);
			}
			//释放信号量
			
			//采用读缓冲区方式，在抬高CS电平后，会自动清除中断标志,无需手动清除
			SpiModifyIntFlag(spi, MCP_CANINTF_RX1IF,0x00);
		}
		
		if((u8SPICanIntFlag & MCP_CANINTF_RX0IF) == MCP_CANINTF_RX0IF)/*0接收中断***/
		{
			res = DriverSPICan_Read(device,&RevBufTemp);
			//读数据判断结果
			if(res >= 0)
			{
				//存缓存
				CanBusDealFunc(device, CANBUS_MSG_InsCanRevQueue, &RevBufTemp);	;
			}
			//释放信号量
			
			//采用读缓冲区方式，在抬高CS电平后，会自动清除中断标志,无需手动清除
			SpiModifyIntFlag(spi, MCP_CANINTF_RX0IF,0x00);
		}
		
		/*错误中断***/
		if((u8SPICanIntFlag & MCP_CANINTF_ERRIF) == MCP_CANINTF_ERRIF)
		{
			SpiModifyIntFlag(spi, MCP_CANINTF_ERRIF,0x00);
			SpiCanErrIntProc(spi,u8SPICanIntFlag);//进行错误中断处理	 
		}
		
		/*报文错误等其他情况*/
		if((u8SPICanIntFlag & MCP_CANINTF_MERRF) == MCP_CANINTF_MERRF)
		{
			SpiModifyIntFlag(spi, u8SPICanIntFlag|MCP_CANINTF_ERRIF,0x00);//2015.04.17,修改
			SpiCanErrIntProc(spi, u8SPICanIntFlag);//进行错误中断处理	，2015.04.17，新增加
		}
		
		/**唤醒中断等其他中断标志****/
		if( ((u8SPICanIntFlag & MCP_CANINTF_WAKEIF) == MCP_CANINTF_WAKEIF) \
		|| ((u8SPICanIntFlag & MCP_CANINTF_TX1IF) == MCP_CANINTF_TX1IF) \
		|| ((u8SPICanIntFlag & MCP_CANINTF_TX2IF) == MCP_CANINTF_TX2IF))
		{
			SpiModifyIntFlag(spi, u8SPICanIntFlag,0x00);
		}
		
		/**发送中断处理***/
		if((u8SPICanIntFlag & MCP_CANINTF_TX0IF) == MCP_CANINTF_TX0IF)
		{		
			CanDriverTxDealFun(device,res);
		}
	}while(++u8ProcTimes < SPI_DEV_INTPROC_TIMESET);
	
	if(u8ProcTimes >= SPI_DEV_INTPROC_TIMESET)//超过设定的最大次数，进行硬件复位
	{
		/**清除所有的中断标志以及错误标志***/
		SpiModifyRegister(spi, MCP_REG_CANINTF, 0xFF, 0x00);			
		SpiModifyRegister(spi, MCP_REG_EFLG, 0xFF, 0x00);
					
//		SpiCan1DeInt(SPICAN_1_CAN_BANDRATE_SET);    jhyjhy
	}

}
	
 /**********************************************************************************************************
 *	函 数 名: SPICAN_1_INT_EXTI_IRQn
 *	功能说明: HUB 外部中断(INT)对应的处理过程
 *	形    参: 寄存器地址address 寄存器数量n 数组values 
 *	返 回 值: 
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
 		//采用SPI查询处理方式时，在外部中断中直接进行数据的读取
 		SpiCanIntExtiCheckoutProc(SPI1);
 	}
 	OSIntExit();	
 }
 
 
  /**********************************************************************************************************
 *	函 数 名: SPICAN_2_INT_EXTI_IRQn
 *	功能说明: HUB 外部中断(INT)对应的处理过程
 *	形    参: 寄存器地址address 寄存器数量n 数组values 
 *	返 回 值: 
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
 		//采用SPI查询处理方式时，在外部中断中直接进行数据的读取
 		SpiCanIntExtiCheckoutProc(SPI2);
 	}
 	OSIntExit();	
 }
 
 
 
 
 


