/*
*********************************************************************************************************
*	                                            DoubleWires
*                           Commuication Controller For The Mining Industry
*
*                                  (c) Copyright 1994-2013  HNDZ 
*                       All rights reserved.  Protected by international copyright laws.
*
*
*    File    : bsp_mcp2525.c
*    Module  : user
*    Version : V1.0
*    History :
*   -----------------
*              Version  Date           By            Note
*              v1.0     2013-12-17     
*
*********************************************************************************************************
*/
#include "bsp_mcp2515.h"
#include <string.h>

/*
*********************************************************************************************************
*	函 数 名: SPI1_Init
*	功能说明: 配置SPI总线的硬件接口（时钟、管脚）和硬件参数
*	形    参:  无
*	返 回 值: 无
*********************************************************************************************************
*/
void SPI1_Init(void)
{	
	GPIO_InitTypeDef GPIO_InitStructure;
	SPI_InitTypeDef  SPI_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);  
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOF | RCC_AHB1Periph_GPIOG ,ENABLE); 
	
	//SCLK:PA5  MISO:PA6  MOSI:PB5  	
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource5, GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource5, GPIO_AF_SPI1);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_5;
	GPIO_Init(GPIOB, &GPIO_InitStructure);	
		
  //PF5---INT-----IN 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;				  //选择管脚
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;		  //悬空输入
  GPIO_Init(GPIOF, &GPIO_InitStructure);					  //初始化		
  //PF3---RESRT---OUT					 
  GPIO_InitStructure.GPIO_Pin =GPIO_Pin_3;		      //IO端口
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//翻转速度
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;			//端口模式为推拉输出方式
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	
  GPIO_Init(GPIOF, &GPIO_InitStructure); 		
	//NOR_FLASH-CS: PB11	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;	
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_SetBits(GPIOB, GPIO_Pin_11);//不选中
  //MCP2515-CS:PF4
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;	
	GPIO_Init(GPIOF, &GPIO_InitStructure);
	GPIO_SetBits(GPIOF, GPIO_Pin_5);//不选中
  //LCD-CS:PG1
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;		
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOG, &GPIO_InitStructure);
	GPIO_SetBits(GPIOG, GPIO_Pin_1);//不选中			

	SPI_I2S_DeInit(SPI1);
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;   //全双工
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;  //8位数据模式
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;         //空闲模式下SCK为1
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;       //数据采样从第2个时间边沿开始
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;          //NSS软件管理
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_64;  //波特率
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB; //大端模式
	SPI_InitStructure.SPI_CRCPolynomial = 7;           //CRC多项式
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;      //主机模式
	SPI_Init(SPI1, &SPI_InitStructure);
	SPI_Cmd(SPI1, DISABLE);			                       //先禁止SPI1	
	SPI_Cmd(SPI1, ENABLE);		
}

/*
*********************************************************************************************************
*	函 数 名: SPI_SendByte
*	功能说明: 向器件发送一个字节，同时从MISO口线采样器件返回的数据
*	形    参:  _ucByte : 发送的字节值
*	返 回 值: 从MISO口线采样器件返回的数据
*********************************************************************************************************
*/
uint8_t SPI_SendByte(uint8_t _ucValue)
{
	/* 等待上个数据未发送完毕 */
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);

	/* 通过SPI硬件发送1个字节 */
	SPI_I2S_SendData(SPI1, _ucValue);

	/* 等待接收一个字节任务完成 */
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);

	/* 返回从SPI总线读到的数据 */
	return SPI_I2S_ReceiveData(SPI1);
}

/*
*********************************************************************************************************
*	函 数 名: SPI_ReadByte
*	功能说明: 从SPI器件读取一个字节数据
*	形    参: 无
*	返 回 值: 从MISO口线采样器件返回的数据
*********************************************************************************************************
*/

uint8_t SPI_ReadByte(void)
{	
	SPI_I2S_SendData(SPI1, 0x00);	
	PLATFORM_OS_TimeDly(10);
	return SPI_I2S_ReceiveData(SPI1);
}


/****************************************************************
mcp2515 intialize;
contain clock intialize, reset, speed rate set.


*******************************************************************/

/*
*********************************************************************************************************
*	函 数 名: mcp2515_clock
*	功能说明: 配置MCP2515芯片的时钟（此处由开发板自带8M晶振提供时钟）
*	形    参: 无
*	返 回 值: 
*********************************************************************************************************
*/
unsigned char mcp2515_clock(void) 
{
//    GPIOA->CRH.B.CNF8  =2   ;
//    GPIOA->CRH.B.MODE8 =3  ;
//    RCC->CFGR.B.MCO=7; //SET MCO OUTPUT CLOCK FOR mcp2515,PLL/2;
	
  return 0;
}

/*
*********************************************************************************************************
*	函 数 名: mcp2515_reset
*	功能说明: MCP2515芯片的软件复位
*	形    参: 无
*	返 回 值: 
*********************************************************************************************************
*/
unsigned char mcp2515_reset(void)    //reset mcp2515;
{
	PLATFORM_OS_TimeDly(1);
	CS_OFF;
	SPI_SendByte(MCP_RESET);
	PLATFORM_OS_TimeDly(5);
	CS_ON;
	PLATFORM_OS_TimeDly(10);
	
	return 0;
}

/*
*********************************************************************************************************
*	函 数 名: mcp2515_config_rate
*	功能说明: 配置CAN2通信的波特率
*	形    参: 无
*	返 回 值: 
*********************************************************************************************************
*/
unsigned char mcp2515_config_rate(void)   //input Fosc=8MHz
{ 
	PLATFORM_OS_TimeDly(1);
	CS_OFF;
	mcp2515_set_register(0x2a,0x07);        //CNF1 配置寄存器1 地址0x2a
	mcp2515_set_register(0x29,0x12);			  //CNF2 配置寄存器2 地址0x29
	mcp2515_set_register(0x28,0x82);			  //CNF3 配置寄存器3 地址0x28
	CS_ON;
	PLATFORM_OS_TimeDly(10);

	return 0;
} 

/*
*********************************************************************************************************
*	函 数 名: mcp2515_read_register
*	功能说明: 读MCP2515芯片指定寄存器的数值
*	形    参: 寄存器地址address
*	返 回 值: ret
*********************************************************************************************************
*/
unsigned int mcp2515_read_register(const unsigned char address)
{
	unsigned int ret;

	PLATFORM_OS_TimeDly(1);
	CS_OFF;
	SPI_SendByte(MCP_READ);
	SPI_SendByte(address);
	ret = SPI_ReadByte();
	PLATFORM_OS_TimeDly(1);
	CS_ON;
	PLATFORM_OS_TimeDly(10);

	return ret;
}

/*
*********************************************************************************************************
*	函 数 名: mcp2515_read_registers
*	功能说明: 读MCP2515芯片多个寄存器的数值
*	形    参: 寄存器地址address 寄存器数量n 数组values 
*	返 回 值: 
*********************************************************************************************************
*/
unsigned char mcp2515_read_registers(const unsigned char address, 
	unsigned char values[], const unsigned char n)
{
	unsigned char i;	

	PLATFORM_OS_TimeDly(1);
	CS_OFF;
	SPI_SendByte(MCP_READ);
	SPI_SendByte(address);
	for (i=0; i<n; i++) 
	{
		values[i] = SPI_ReadByte();
		PLATFORM_OS_TimeDly(1);
	}
	CS_ON;
	PLATFORM_OS_TimeDly(10);
	
	return 0;
}

/*
*********************************************************************************************************
*	函 数 名: mcp2515_read_status
*	功能说明: 读MCP2515芯片状态寄存器
*	形    参: 无
*	返 回 值: sta
*********************************************************************************************************
*/
unsigned char mcp2515_read_status(void)
{
	unsigned char sta;

	PLATFORM_OS_TimeDly(1);
	CS_OFF;
	SPI_SendByte(MCP_READ_STATUS);
	sta = SPI_ReadByte();
	CS_ON;
	PLATFORM_OS_TimeDly(10);

	return sta;
}

/*
*********************************************************************************************************
*	函 数 名: mcp2515_set_register
*	功能说明: 写MCP2515芯片指定寄存器的数值
*	形    参: 寄存器地址address 寄存器值value
*	返 回 值:
*********************************************************************************************************
*/
unsigned char mcp2515_set_register(const unsigned char address,const unsigned char value)
{
	PLATFORM_OS_TimeDly(1);
	CS_OFF;
	SPI_SendByte(MCP_WRITE); 
	SPI_SendByte(address);
	SPI_SendByte(value);
	CS_ON;
	PLATFORM_OS_TimeDly(10);

	return 0;
}

/*
*********************************************************************************************************
*	函 数 名: mcp2515_set_registers
*	功能说明: 写MCP2515芯片多个寄存器的数值
*	形    参: 寄存器地址address 寄存器数量n 数组values 
*	返 回 值: 
*********************************************************************************************************
*/
unsigned char mcp2515_set_registers(const unsigned char address, 
	const unsigned char values[], const unsigned char n)
{
	unsigned char i;

	PLATFORM_OS_TimeDly(1);
	CS_OFF;
	SPI_SendByte(MCP_WRITE);
	SPI_SendByte(address);
	for (i=0; i<n; i++) 
	{ 
	  SPI_SendByte(values[i]);
	}
	CS_ON;
	PLATFORM_OS_TimeDly(10);

	return 0;
}

/*
*********************************************************************************************************
*	函 数 名: mcp2515_modify_register
*	功能说明: 修改MCP2515芯片指定寄存器指定位的数值
*	形    参: 寄存器地址address 掩码mask 数据data
*	返 回 值: 
*********************************************************************************************************
*/
unsigned char mcp2515_modify_register(const unsigned char address, 
	const unsigned char mask, const unsigned char data)
{

	PLATFORM_OS_TimeDly(1);
	CS_OFF;
	SPI_SendByte(MCP_BITMOD);
	SPI_SendByte(address);
	SPI_SendByte(mask);
	SPI_SendByte(data);
	CS_ON;
	PLATFORM_OS_TimeDly(10);

	return 0;
}

/*
*********************************************************************************************************
*	函 数 名: mcp2515_read_can_id
*	功能说明: 从MCP2515芯片指定寄存器读取CAN通信的ID
*	形    参: 寄存器地址：mcp_addr 扩展ID：ext 标准IDcan_id
*	返 回 值: 
*********************************************************************************************************
*/
unsigned char mcp2515_read_can_id( const unsigned char mcp_addr, 
	unsigned char* ext, unsigned int* can_id )
{
	unsigned char tbufdata[4];
	*ext = 0;
	*can_id = 0;

	mcp2515_read_registers( mcp_addr, tbufdata, 4 );
	*can_id = (tbufdata[0]<<3) + (tbufdata[1]>>5);
	// have no extend ID;
	
	return 0;
}

/*
*********************************************************************************************************
*	函 数 名: mcp2515_read_can_msg
*	功能说明: 从MCP2515芯片指定寄存器读取CAN通信的消息
*	形    参: 寄存器地址：mcp_addr 消息：msg
*	返 回 值: 
*********************************************************************************************************
*/
unsigned char mcp2515_read_can_msg( const unsigned char buffer_sidh_addr,
	CAN_MESSAGE* msg)
{

	unsigned char mcp_addr, ctrl;

	mcp_addr = buffer_sidh_addr;

	mcp2515_read_can_id( mcp_addr, &(msg->extended_identifier), 
	 &(msg->identifier) );

	ctrl = mcp2515_read_register( mcp_addr-1 );
	msg->dlc = (mcp2515_read_register( mcp_addr+4 )&0x0f);

	if ((msg->dlc & 0x40) || (ctrl & 0x08)) 
	{
	  msg->rtr = 1;
	}
	else 
	{
	  msg->rtr = 0;
	}
	msg->dlc &= 0x0F;	
	mcp2515_read_registers(mcp_addr+5,&(msg->dta[0]),msg->dlc) ;
	//msg->dta[0]=mcp2515_read_register( mcp_addr+5);
	//msg->dta[1]=mcp2515_read_register( mcp_addr+6);
	//msg->dta[2]=mcp2515_read_register( mcp_addr+7);
	//msg->dta[3]=mcp2515_read_register( mcp_addr+8);
	//msg->dta[4]=mcp2515_read_register( mcp_addr+9);
	//msg->dta[5]=mcp2515_read_register( mcp_addr+10);
	//msg->dta[6]=mcp2515_read_register( mcp_addr+11);
	//msg->dta[7]=mcp2515_read_register( mcp_addr+12);
	
	return 0;
}
	 
/*
*********************************************************************************************************
*	函 数 名: mcp2515_read_can_id
*	功能说明: 向MCP2515芯片指定寄存器写入CAN通信的ID
*	形    参: 寄存器地址：mcp_addr 扩展ID：ext 标准IDcan_id
*	返 回 值: 
*********************************************************************************************************
*/
unsigned char mcp2515_write_can_id( const unsigned char mcp_addr, 
	const unsigned char ext, const unsigned int can_id )   
{
	unsigned char tbufdata[4];

	tbufdata[0] = (unsigned char)(can_id>>3);
	tbufdata[1] = (unsigned char)(can_id<<5);
	tbufdata[2] = 0;
	tbufdata[3] = 0;	  
	mcp2515_set_registers( mcp_addr, tbufdata, 4 );	 //mcp_addr start from 31H ;

	return 0;
}

/*
*********************************************************************************************************
*	函 数 名: mcp2515_write_can_msg
*	功能说明: 向MCP2515芯片指定寄存器写入CAN通信的消息
*	形    参: 寄存器地址：mcp_addr 消息：msg
*	返 回 值: 
*********************************************************************************************************
*/
unsigned char mcp2515_write_can_msg( const unsigned char buffer_sidh_addr, 
	const CanTxMsg* msg)
{
	unsigned char mcp_addr, dlc;

	mcp_addr = buffer_sidh_addr;   //id register=31H
	dlc = msg->DLC;
	mcp2515_write_can_id( mcp_addr, msg->ExtId,msg->StdId );  	 // write CAN id
	mcp2515_set_register( (mcp_addr+4), (0x0F&dlc) );  // write the RTR and DLC	,RTR=0;
	mcp2515_set_registers((mcp_addr+5), &(msg->Data[0]), dlc );  // write data bytes

	return 0;
}

/*
*********************************************************************************************************
*	函 数 名: mcp2515_get_free_txbuf
*	功能说明: 获取当前空闲的发送缓冲区
*	形    参: 可用缓冲区 txbuf_n
*	返 回 值: res
*********************************************************************************************************
*/
unsigned char mcp2515_get_free_txbuf(unsigned char *txbuf_n)
{
	unsigned char res, i, ctrlval;
	unsigned char ctrlregs[3] = { MCP_TXB0CTRL, MCP_TXB1CTRL, MCP_TXB2CTRL };

	res = 2;//all TXB is in busy ;
	*txbuf_n = 0x00;

	for (i=0; i<3; i++) // check all 3 TX-Buffers
	{
		ctrlval = mcp2515_read_register( ctrlregs[i] );
		if ( (ctrlval & 0x08) == 0 ) 
		{
			*txbuf_n = ctrlregs[i]+1; // return SIDH-address of Buffer
			res = 1;
			return res; // ! function exit 
		}
	}

	return res;
}

/*
*********************************************************************************************************
*	函 数 名: mcp2515_init_can_buffers
*	功能说明: 初始化接收缓冲区
*	形    参: 无
*	返 回 值: 
*********************************************************************************************************
*/	
unsigned char mcp2515_init_can_buffers(void)
{
	unsigned char i, a1, a2, a3;

	// TODO: check why this is needed to receive extended 
	//   and standard frames
	// Mark all filter bits as don't care:
	mcp2515_write_can_id(MCP_RXM0SIDH, 0, 0);
	mcp2515_write_can_id(MCP_RXM1SIDH, 0, 0);
	// Anyway, set all filters to 0:
	mcp2515_write_can_id(MCP_RXF0SIDH, 1, 0); // RXB0: extended 
	mcp2515_write_can_id(MCP_RXF1SIDH, 0, 0); //       AND standard
	mcp2515_write_can_id(MCP_RXF2SIDH, 1, 0); // RXB1: extended 
	mcp2515_write_can_id(MCP_RXF3SIDH, 0, 0); //       AND standard
	mcp2515_write_can_id(MCP_RXF4SIDH, 0, 0);
	mcp2515_write_can_id(MCP_RXF5SIDH, 0, 0);

	// Clear, deactivate the three transmit buffers
	// TXBnCTRL -> TXBnD7
	a1 = MCP_TXB0CTRL;
	a2 = MCP_TXB1CTRL;
	a3 = MCP_TXB2CTRL;
	for (i = 0; i < 14; i++) 
	{ // in-buffer loop
		mcp2515_set_register(a1, 0);
		mcp2515_set_register(a2, 0);
		mcp2515_set_register(a3, 0);
		a1++;
		a2++;
		a3++;
	}

	// and clear, deactivate the two receive buffers.
	mcp2515_set_register(MCP_RXB0CTRL, 0);
	mcp2515_set_register(MCP_RXB1CTRL, 0);

	return 0;
}

/*
*********************************************************************************************************
*	函 数 名: mcp2515_rts
*	功能说明: 准备发送
*	形    参: 无
*	返 回 值: 
*********************************************************************************************************
*/
unsigned char mcp2515_rts(void)
{  
	CS_OFF;
	PLATFORM_OS_TimeDly(1);
	SPI_SendByte(0x81);
	CS_ON;
	PLATFORM_OS_TimeDly(1);

	return 0;
}
   
/*
*********************************************************************************************************
*	函 数 名: mcp2515_start_transmit
*	功能说明: 开始发送
*	形    参: 无
*	返 回 值: 
*********************************************************************************************************
*/
unsigned char mcp2515_start_transmit()
{
	mcp2515_modify_register( 0x30 , 0x08, 0x08 );//modify the reg. TXBnCTRL.TXREQ=1,wait for data sending;

	return 0;
}			

/*
*********************************************************************************************************
*	函 数 名: mcp2515_filter_intialize
*	功能说明: 对CAN接收滤波器进行初始化
*	形    参: 无
*	返 回 值: 
*********************************************************************************************************
*/
unsigned char mcp2515_filter_intialize(void)
{
	mcp2515_set_register(MCP_RXF0SIDH,0x05);   //ID=0x05 can be receive;
	mcp2515_set_register(MCP_RXF0SIDL,0x00);
	mcp2515_set_register(MCP_RXM0SIDH,0xFF);
	mcp2515_set_register(MCP_RXM0SIDL,0xE0);

  return 0;
}

/*
*********************************************************************************************************
*	函 数 名: mcp2515_setCANCTRL_Mode
*	功能说明: 设置CAN2的传输模式
*	形    参: CAN的新传输模式newmode
*	返 回 值: 
*********************************************************************************************************
*/
uint8_t mcp2515_setCANCTRL_Mode(uint8_t newmode)
{
	uint8_t i;
	
	i = mcp2515_read_register(MCP_CANCTRL);
	i &= ~(MODE_MASK);
	i |= newmode;
	mcp2515_set_register(MCP_CANCTRL, i);
	
	// verify as advised in datasheet
	i = mcp2515_read_register(MCP_CANCTRL);
	i &= MODE_MASK;
	if ( i == newmode ) 
	{
		return MCP2515_OK; 
	}
	else 
	{
		return MCP2515_FAIL;
	}
}

/*
*********************************************************************************************************
*	函 数 名: can_read_message
*	功能说明: 读取接收到的消息
*	形    参: 消息msg
*	返 回 值: res
*********************************************************************************************************
*/ 
unsigned char can_read_message(CAN_MESSAGE *msg)
{
	unsigned char stat, res;

	stat = mcp2515_read_status();
	if ( stat & 0x01 ) 
	{
		// Msg in Buffer 0
		mcp2515_read_can_msg( MCP_RXB0SIDH, msg);	//
		mcp2515_modify_register(MCP_CANINTF, 0x01, 0);		//clear the receive interrupt identify;
		res = 1;
	}
	else if ( stat & 0x02 ) 
	{
		// Msg in Buffer 1
		mcp2515_read_can_msg( MCP_RXB1SIDH, msg);
		mcp2515_modify_register(MCP_CANINTF, 0x02, 0);
		res = 1;
	}
	else 
	{
		res = 0;
	}
	
	return res;
}

/*
*********************************************************************************************************
*	函 数 名: can_send_message
*	功能说明: 发送消息
*	形    参: 消息msg
*	返 回 值: 
*********************************************************************************************************
*/ 
unsigned char can_send_message(const CanTxMsg* msg)
{
  unsigned char free_txbuf;

	mcp2515_set_register(MCP_CANINTE,0x00); //clear the INT	
	mcp2515_get_free_txbuf(&free_txbuf);    //get the free buffer for sending the data;
	mcp2515_write_can_msg(free_txbuf, msg);	//buffer 31h;
	mcp2515_start_transmit();  //start the transmit;

	return 0;	
}

/*
*********************************************************************************************************
*	函 数 名: intit_message_struct
*	功能说明: 初始化发送消息的结构
*	形    参: 消息msg
*	返 回 值: 
*********************************************************************************************************
*/
void init_message_struct(CanTxMsg* msg)
{
	memset(msg,0,sizeof(CanTxMsg));
}

/*
*********************************************************************************************************
*	函 数 名: can_test_transmit
*	功能说明: 发送指定消息
*	形    参: 无
*	返 回 值: 
*********************************************************************************************************
*/
unsigned char can_test_transmit(void)
{
  CanTxMsg msg;			    //发送缓冲区
	
	init_message_struct(&msg);
	msg.StdId =  0x00;		//ID;
	msg.IDE = CAN_ID_STD;
	msg.DLC = 8;				  // the width of the data;
	msg.Data[0] = 0x48;		//data;
	msg.Data[1] = 0x4e;
	msg.Data[2] = 0x44;
	msg.Data[3] = 0x5a;
	msg.Data[4] = 0x4b;
	msg.Data[5] = 0x54;
	msg.Data[6] = 0X43;
	msg.Data[7] = 0x31;		
	RUN_TRACE("CAN2 Tx data is %x，%x，%x，%x，%x，%x，%x，%x \r\n",msg.Data[0],msg.Data[1],msg.Data[2],msg.Data[3],msg.Data[4],msg.Data[5],msg.Data[6],msg.Data[7]);	
	can_send_message(&msg);
	PLATFORM_OS_TimeDly(5);	

	return 0;
}

/*
*********************************************************************************************************
*	函 数 名: can_test_receive
*	功能说明: 接收函数
*	形    参: 无
*	返 回 值: 
*********************************************************************************************************
*/
void can_test_receive(void)
{
	CAN_MESSAGE  RxMessage;
	unsigned char buf[30];

	can_read_message(&RxMessage);
	buf[0]=RxMessage.dta[0];
	buf[1]=RxMessage.dta[1];
	buf[2]=RxMessage.dta[2];
	buf[3]=RxMessage.dta[3];
	buf[4]=RxMessage.dta[4];
	buf[5]=RxMessage.dta[5];
	buf[6]=RxMessage.dta[6];
	buf[7]=RxMessage.dta[7];

	RUN_TRACE("CAN2 Rx data is %x，%x，%x，%x，%x，%x，%x，%x \r\n ",buf[0],buf[1],buf[2],buf[3],buf[4],buf[5],buf[6],buf[7]);
}
