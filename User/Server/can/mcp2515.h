/********************************************************************************
* 文件名称：	mcp2515.h
* 作	者：	马如意   
* 当前版本：   	V1.0
* 完成日期：    2014.10.28
* 功能描述: 	定义MCP2515硬件的通用头文件
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
#ifndef _MCP2515_H_
#define _MCP2515_H_
/********************************************************************************
* .h头文件
*********************************************************************************/

/********************************************************************************
* #define宏定义
*********************************************************************************/
/******MCP寄存器定义*******************************/
#define MCP_REG_RXF0SIDH			    0x00
#define MCP_REG_RXF0SIDL				0x01
#define MCP_REG_RXF0EID8				0x02
#define MCP_REG_RXF0EID0				0x03
#define MCP_REG_RXF1SIDH				0x04
#define MCP_REG_RXF1SIDL				0x05
#define MCP_REG_RXF1EID8				0x06
#define MCP_REG_RXF1EID0				0x07
#define MCP_REG_RXF2SIDH				0x08
#define MCP_REG_RXF2SIDL				0x09
#define MCP_REG_RXF2EID8				0x0A
#define MCP_REG_RXF2EID0				0x0B
#define MCP_REG_BFPCTRL					0x0C//特殊说明，可以采用位修改指令进行修改。
#define MCP_REG_TXRTSCTRL				0x0D//特殊说明，可以采用位修改指令进行修改。
#define MCP_REG_CANSTAT					0x0E
#define MCP_REG_CANCTRL					0x0F//特殊说明，可以采用位修改指令进行修改。

#define MCP_REG_RXF3SIDH				0x10
#define MCP_REG_RXF3SIDL				0x11
#define MCP_REG_RXF3EID8				0x12
#define MCP_REG_RXF3EID0				0x13
#define MCP_REG_RXF4SIDH				0x14
#define MCP_REG_RXF4SIDL				0x15
#define MCP_REG_RXF4EID8				0x16
#define MCP_REG_RXF4EID0				0x17
#define MCP_REG_RXF5SIDH				0x18
#define MCP_REG_RXF5SIDL				0x19
#define MCP_REG_RXF5EID8				0x1A
#define MCP_REG_RXF5EID0				0x1B
#define MCP_REG_TEC						0x1C
#define MCP_REG_REC        				0x1D

#define MCP_REG_RXM0SIDH				0x20
#define MCP_REG_RXM0SIDL				0x21
#define MCP_REG_RXM0EID8				0x22
#define MCP_REG_RXM0EID0				0x23
#define MCP_REG_RXM1SIDH				0x24
#define MCP_REG_RXM1SIDL				0x25
#define MCP_REG_RXM1EID8				0x26
#define MCP_REG_RXM1EID0				0x27
#define MCP_REG_CNF3					  0x28//特殊说明，可以采用位修改指令进行修改。
#define MCP_REG_CNF2					  0x29//特殊说明，可以采用位修改指令进行修改。
#define MCP_REG_CNF1					  0x2A//特殊说明，可以采用位修改指令进行修改。
#define MCP_REG_CANINTE					0x2B//特殊说明，可以采用位修改指令进行修改。
#define MCP_REG_CANINTF					0x2C//特殊说明，可以采用位修改指令进行修改。
#define MCP_REG_EFLG				   	0x2D//特殊说明，可以采用位修改指令进行修改。

#define MCP_REG_TXB0CTRL				0x30//特殊说明，可以采用位修改指令进行修改。
#define MCP_REG_TXB0SIDH				0x31
#define MCP_REG_TXB0SIDL				0x32
#define MCP_REG_TXB0EID8				0x33
#define MCP_REG_TXB0EID0				0x34
#define MCP_REG_TXB0DLC					0x35
#define MCP_REG_TXB0D0					0x36
#define MCP_REG_TXB0D1					0x37
#define MCP_REG_TXB0D2					0x38
#define MCP_REG_TXB0D3					0x39
#define MCP_REG_TXB0D4					0x3A
#define MCP_REG_TXB0D5					0x3B
#define MCP_REG_TXB0D6					0x3C
#define MCP_REG_TXB0D7					0x3D

#define MCP_REG_TXB1CTRL				0x40//特殊说明，可以采用位修改指令进行修改。
#define MCP_REG_TXB1SIDH				0x41
#define MCP_REG_TXB1SIDL				0x42
#define MCP_REG_TXB1EID8				0x43
#define MCP_REG_TXB1EID0				0x44
#define MCP_REG_TXB1DLC					0x45
#define MCP_REG_TXB1D0					0x46
#define MCP_REG_TXB1D1					0x47
#define MCP_REG_TXB1D2					0x48
#define MCP_REG_TXB1D3					0x49
#define MCP_REG_TXB1D4					0x4A
#define MCP_REG_TXB1D5					0x4B
#define MCP_REG_TXB1D6					0x4C
#define MCP_REG_TXB1D7					0x4D

#define MCP_REG_TXB2CTRL				0x50//特殊说明，可以采用位修改指令进行修改。
#define MCP_REG_TXB2SIDH				0x51
#define MCP_REG_TXB2SIDL				0x52
#define MCP_REG_TXB2EID8				0x53
#define MCP_REG_TXB2EID0				0x54
#define MCP_REG_TXB2DLC					0x55
#define MCP_REG_TXB2D0					0x56
#define MCP_REG_TXB2D1					0x57
#define MCP_REG_TXB2D2					0x58
#define MCP_REG_TXB2D3					0x59
#define MCP_REG_TXB2D4					0x5A
#define MCP_REG_TXB2D5					0x5B
#define MCP_REG_TXB2D6					0x5C
#define MCP_REG_TXB2D7					0x5D

#define MCP_REG_RXB0CTRL				0x60//特殊说明，可以采用位修改指令进行修改。
#define MCP_REG_RXB0SIDH				0x61
#define MCP_REG_RXB0SIDL				0x62
#define MCP_REG_RXB0EID8				0x63
#define MCP_REG_RXB0EID0				0x64
#define MCP_REG_RXB0DLC					0x65
#define MCP_REG_RXB0D0					0x66
#define MCP_REG_RXB0D1					0x67
#define MCP_REG_RXB0D2					0x68
#define MCP_REG_RXB0D3					0x69
#define MCP_REG_RXB0D4					0x6A
#define MCP_REG_RXB0D5					0x6B
#define MCP_REG_RXB0D6					0x6C
#define MCP_REG_RXB0D7					0x6D

#define MCP_REG_RXB1CTRL				0x70//特殊说明，可以采用位修改指令进行修改。
#define MCP_REG_RXB1SIDH				0x71
#define MCP_REG_RXB1SIDL				0x72
#define MCP_REG_RXB1EID8				0x73
#define MCP_REG_RXB1EID0				0x74
#define MCP_REG_RXB1DLC					0x75
#define MCP_REG_RXB1D0					0x76
#define MCP_REG_RXB1D1					0x77
#define MCP_REG_RXB1D2					0x78
#define MCP_REG_RXB1D3					0x79
#define MCP_REG_RXB1D4					0x7A
#define MCP_REG_RXB1D5					0x7B
#define MCP_REG_RXB1D6					0x7C
#define MCP_REG_RXB1D7					0x7D

/***寄存器中位信息定义************************/
/*定义RX0BF引脚控制和状态寄存器BFPCTRL位信息*/
typedef enum
{
	MCP_REG_BFPCTRL_B1BFS	=	5,
	MCP_REG_BFPCTRL_B0BFS	=	4,
	MCP_REG_BFPCTRL_B1BFE	=	3,
	MCP_REG_BFPCTRL_B0BFE	=	2,
	MCP_REG_BFPCTRL_B1BFM	=	1,
	MCP_REG_BFPCTRL_B0BFM	=	0
}MCP_REG_BFPCTRL_BIT;

/*定义TXnRST引脚控制盒状态寄存器TXRTSCTRL位信息*/
typedef enum
{
	MCP_REG_B2RTS			=	5,
	MCP_REG_B1RTS			=	4,
	MCP_REG_B0RTS			=	3,
	MCP_REG_B2RTSM			=	2,
	MCP_REG_B1RTSM			=	1,
	MCP_REG_B0RTSM			=	0,
}MCP_REG_TXRTSCTR_BIT;

/*定义状态寄存器CANSTAT位信息*/
typedef enum
{

	MCP_REG_CANSTAT_OPMOD2	=	7,
	MCP_REG_CANSTAT_OPMOD1	=	6,
	MCP_REG_CANSTAT_OPMOD0	=	5,
	MCP_REG_CANSTAT_ICOD2	=	3,
	MCP_REG_CANSTAT_ICOD1	=	2,
	MCP_REG_CANSTAT_ICOD0	=	1,
}MCP_REG_CANSTAT_BIT;

/*定义控制寄存器CANCTRL位信息*/
typedef enum
{
	MCP_REG_CANCTRL_REQOP2	=	7,
	MCP_REG_CANCTRL_REQOP1	=	6,
	MCP_REG_CANCTRL_REQOP0	=	5,
	MCP_REG_CANCTRL_ABAT	=	4,
	MCP_REG_CANCTRL_CLKEN	=	2,
	MCP_REG_CANCTRL_CLKPRE1	=	1,
	MCP_REG_CANCTRL_CLKPRE0	=	0
}MCP_REG_CANCTRL_BIT;

/*定义配置寄存器CNF3位信息*/
typedef enum
{
	MCP_REG_CNF3_SOF		=	7,
	MCP_REG_CNF3_WAKFIL		=	6,
	MCP_REG_CNF3_PHSEG22	=	2,
	MCP_REG_CNF3_PHSEG21	=	1,
	MCP_REG_CNF3_PHSEG20	=	0
}MCP_REG_CANF3_BIT;

/*定义配置寄存器CNF2位信息*/
typedef enum
{
	MCP_REG_CANF2_BTLMODE	=	7,
	MCP_REG_CANF2_SAM		=	6,
	MCP_REG_CANF2_PHSEG12	=	5,
	MCP_REG_CANF2_PHSEG11	=	4,
	MCP_REG_CANF2_PHSEG10	=	3,
	MCP_REG_CANF2_PHSEG2	=	2,
	MCP_REG_CANF2_PHSEG1	=	1,
	MCP_REG_CANF2_PHSEG0	=	0
}MCP_REG_CANF2_BIT;

/*定义配置寄存器CNF1位信息*/
typedef enum
{
	MCP_REG_CANF1_SJW1		=	7,
	MCP_REG_CANF1_SJW0		=	6,
	MCP_REG_CANF1_BRP5		=	5,
	MCP_REG_CANF1_BRP4		=	4,
	MCP_REG_CANF1_BRP3		=	3,
	MCP_REG_CANF1_BRP2		=	2,
	MCP_REG_CANF1_BRP1		=	1,
	MCP_REG_CANF1_BRP0		=	0
}MCP_REG_CANF1_BIT;

/*定义中断使能寄存器CANINTE位信息*/
typedef enum
{
	MCP_REG_CANINTE_MERRE	=	7,//报文错误中断使能位
	MCP_REG_CANINTE_WAKIE	=	6,//CAN总线唤醒中断使能位
	MCP_REG_CANINTE_ERRIE	=	5,//错误中断使能位
	MCP_REG_CANINTE_TX2IE	=	4,//发送缓冲器2空中断使能
	MCP_REG_CANINTE_TX1IE	=	3,//发送缓冲器1空中断使能
	MCP_REG_CANINTE_TX0IE	=	2,//发送缓冲器0空中断使能
	MCP_REG_CANINTE_RX1IE	=	1,//接收缓冲器1满中断使能
	MCP_REG_CANINTE_RX0IE	=	0//接收缓冲器0满中断使能
}MCP_REG_CANINTE_BIT;

/*定义中断标志寄存器CANINTF位信息*/
typedef enum
{
	MCP_REG_CANINTF_MERRF	=	7,//报文错误标志位
	MCP_REG_CANINTF_WAKIF	=	6,//CAN总线唤醒中断标志位
	MCP_REG_CANINTF_ERRIF	=	5,//错误中断标志位
	MCP_REG_CANINTF_TX2IF	=	4,//发送缓冲器2空中断标志位
	MCP_REG_CANINTF_TX1IF	=	3,//发送缓冲器1空中断标志位
	MCP_REG_CANINTF_TX0IF	=	2,//发送缓冲器0空中断标志位
	MCP_REG_CANINTF_RX1IF	=	1,//接收缓冲器1满中断标志位
	MCP_REG_CANINTF_RX0IF	=	0//接收缓冲器0满中断标志位
}MCP_REG_CANINTF_BIT;

/*定义错误标志寄存器EFLG位信息*/
typedef enum
{
	MCP_REG_EFLG_RX1OVR	=	0x80,
	MCP_REG_EFLG_RX0OVR	=	0x40,
	MCP_REG_EFLG_TXB0	=	0x20,
	MCP_REG_EFLG_TXEP	=	0x10,
	MCP_REG_EFLG_RXEP	=	0x08,
	MCP_REG_EFLG_TXWAR	=	0x04,
	MCP_REG_EFLG_RXWAR	=	0x02,
	MCP_REG_EFLG_EWARN	=	0x01
}MCP_REG_EFLG_BIT;

/*** 定义发送缓冲器n控制寄存器TXBnCTRL  ( n = 0, 1, 2 )位信息*/
typedef enum
{
	MCP_REG_TXB_ABTF_M      =   0x06,
	MCP_REG_TXB_MLOA_M      =   0x05,
	MCP_REG_TXB_TXERR_M     =   0x04,
	MCP_REG_TXB_TXREQ_M     =   0x03,
	MCP_REG_TXB_TXP1_M      =   0x01,
	MCP_REG_TXB_TXP0_M      =   0x00	
}MCP_REG_TXBnCTRL_BIT;/**TXB0CTRL、TXB1CTRL、TXB2CTRL**/

/*定义寄存器RXBnCTRL(n=0,1)位信息*/
typedef enum
{
	MCP_REG_RXBnCTRL_RXM1	=	6,
	MCP_REG_RXBnCTRL_RXM0	=	5,
	MCP_REG_RXBnCTRL_RXRTR	=	3,
	MCP_REG_RXBnCTRL_BUKT	=	2,
	MCP_REG_RXBnCTRL_BUKT1	=	1,
	MCP_REG_RXBnCTRL_FILHIT0=	0
}MCP_REG_RXBnCTRL_BIT;/**RXB0CTRL、RXB1CTRL**/

/*****定义SPI控制命令字*****************************/
typedef enum
{
	MCP_CMD_RESET			=  	0xC0,// 复位
	MCP_CMD_READ			=	0x03,// 从指定地址起始的寄存器读取数据
	MCP_CMD_RD_RX_BUFF		= 	0x90,// 读RX缓冲器
	MCP_CMD_WRITE			=	0x02,// 将数据写指定地址起始的寄存器
	MCP_CMD_LOAD_TX			=	0x40,// 装载TX缓冲器
	MCP_CMD_RTS				=	0x80,// 请求发送报文(具体分为0x81\82\84三个发送命令)
	MCP_CMD_RD_STATUS		=	0xA0,// 读状态
	MCP_CMD_RX_STATUS		=	0xB0,// RX状态
	MCP_CMD_BIT_MODIFY		=	0x05,// 位修改
	/**0x80命令的具体实现****/
	MCP_CMD_RTS_TXB0    	=	0x81,// 请求发送发送缓冲区0的数据
	MCP_CMD_RTS_TXB1    	=	0x82, // 请求发送发送缓冲区1的数据
	MCP_CMD_RTS_TXB2   	 	=	0x84// 请求发送发送缓冲区2的数据
}MCP_CMD_TYPE;

/**定义读取缓冲区时使用的定义****/
typedef enum
{
	MCP_READBUF_BUF0 = 0x00,//读取缓冲区0
	MCP_READBUF_BUF1 = 0x04//读取缓冲区1
	/**此处未定义仅读取数据的指令，防止误操作****/	
}MCP_READBUF_REG_TYPE;

enum 
{
	MCP_READBUF_ANY_STATUS = 0xFF,//读取缓冲器时使用的任意命令
};

/**定义读取缓冲区时使用的定义****/
typedef enum
{
	MCP_READBUF_LENTH_1 = 1,//读取1个缓冲区，数据
	MCP_READBUF_LENTH_2 = 2,//读取2个缓冲区，数据
	MCP_READBUF_LENTH_3 = 3,//读取3个缓冲区，数据
	MCP_READBUF_LENTH_4 = 4,//读取4个缓冲区，数据
	MCP_READBUF_LENTH_5 = 5,//读取5个缓冲区，数据
	MCP_READBUF_LENTH_6 = 6,//读取6个缓冲区，数据
	MCP_READBUF_LENTH_7 = 7,//读取7个缓冲区，数据
	/**较常用***/
	MCP_READBUF_LENTH_8 = 8,//读取8个缓冲区，数据

	MCP_READBUF_LENTH_9 = 9,//读取9个缓冲区，数据
	MCP_READBUF_LENTH_10 = 10,//读取10个缓冲区，数据
	MCP_READBUF_LENTH_11 = 11,//读取11个缓冲区，数据
	MCP_READBUF_LENTH_12 = 12,//读取12个缓冲区，数据
	/**较常用***/
	MCP_READBUF_LENTH_13 = 13,//读取13个缓冲区，标示符+长度+数据
}MCP_READBUF_LENGTH_TYPE;

/**SPI设备状态类型定义****/
typedef enum
{
	SPI_DEV_STATUS_INIT,		//设备处于初始化状态
	SPI_DEV_STATUS_INIT_FAIL,	//设备处于初始化失败状态
	SPI_DEV_STATUS_DEINIT_FAIL,	//设备处于复位失败状态	
	SPI_DEV_STATUS_NORMAL,		//设备处于正常状态	
	SPI_DEV_STATUS_DEINT_NORMAL,//设备处于复位正常状态	
	SPI_DEV_STATUS_MAX,			//设备状态最大值?
}SPI_DEV_STATUS_TYPE;

/**SPI命令类型定义****/
typedef enum
{
	SPI_PROC_WRITE,				//写数据方式
	SPI_PROC_READ				//读数据方式
}SPI_PROC_TYPE;

/**SPI数据帧类型定义****/
#define SPI_ONEFRAME_MAX_SIZE  20//SPI一桢数据的最大长度
__packed typedef struct
{
	u32 u32ProcType ;
	
	u8 u8TxBuf[SPI_ONEFRAME_MAX_SIZE];
	u8 u8TxLength;
	u8 u8TxRead;
	
	u8 u8RxBuf[SPI_ONEFRAME_MAX_SIZE];
	u8 u8RxWrite;
	u8 u8RxLength;	
}SPI_DATA_TYPE;


/**配置过程命令字定义****************************/   
/**工作模式设置***/
typedef enum
{
	MCP_CAN_MODE_NORMAL 	=  0x00,// 正常模式
	MCP_CAN_MODE_SLEEP   	=  0x20,//休眠模式	  
	MCP_CAN_MODE_LOOP   	=  0x40,// 回环模式
	MCP_CAN_MODE_LISTEN  	=  0x60,//监听模式 
	MCP_CAN_MODE_SETUPE	 	=  0x80,// 配置模式
	MCP_CAN_MODE_MAX 		=  0x81//最大值
}MCP_CANCTRL_MODE_TYPE;

/**CANCTRL 自动重传设置***/
typedef enum
{
	MCP_CANCTRL_AUTO_RESEND = 0x00,//自动重传
	MCP_CANCTRL_OSM = 0x08//单触发模式	
}MCP_CANCTRL_OSM_TYPE;

/**CANCTRL CLKOUT引脚使能设置***/
typedef enum
{
	MCP_CANCTRL_CLKOUT_DISABLE = 0x00,//CLKOUT引脚不使能
	MCP_CANCTRL_CLKOUT_ENABLE = 0x04//CLKOUT引脚使能	
}MCP_CANCTRL_CLKOUT_TYPE;

/**终止发送报文请求****/
enum
{
	MCP_CANCTRL_ABAT_ENABLE = 0x80,//终止发送
	MCP_CANCTRL_ABAT_DISABLE = 0x00//不终止发送
};

/**CANCTRL CLKOUT引脚使能设置***/
typedef enum
{
	MCP_CANCTRL_CLKOUTPER_1 = 0x00,//CLKOUT引脚使能
	MCP_CANCTRL_CLKOUTPER_2 = 0x01,//CLKOUT引脚不使能
	MCP_CANCTRL_CLKOUTPER_4 = 0x02,//CLKOUT引脚不使能
	MCP_CANCTRL_CLKOUTPER_8 = 0x03,//CLKOUT引脚不使能	
}MCP_CANCTRL_CLKOUTPER_TYPE;


/**接收缓冲控制寄存器中，滤波方式的枚举***/
typedef enum
{
	MCP_RXBNCTRL_FILTER_ALL 	= 0x00,//接收符合滤波条件的标准帧以及扩展帧	
	MCP_RXBNCTRL_FILTER_STAND 	= 0x20,//接收符合滤波条件的标准帧
	MCP_RXBNCTRL_FILTER_EXT 	= 0x40,//接收符合滤波条件的扩展帧
	MCP_RXBNCTRL_FILTER_CLOSE 	= 0x60,//关闭滤波功能，接收所有功能
}MCP_RXBUF_FILTERMODE_TYPE;

/**接收缓冲控制寄存器中，滚存方式的枚举***/
typedef enum
{
	MCP_RXBNCTRL_BUKE_ENABLE 	= 0x04,//滚存方式使能，buf0满，自动滚存到buf1
	MCP_RXBNCTRL_BUKE_DISABLE 	= 0x00,//滚存方式禁止		
}MCP_RXBUF_BUKTMODE_TYPE;

/**接收中断输出引脚RXNBF配置中使用的枚举***/
typedef enum
{
	MCP_RXNBF_RX1BF_ENABLE 		= 0x08,//RX0BF引脚功能使能
	MCP_RXNBF_RX0BF_ENABLE 		= 0x04,//RX1BF引脚功能使能
	MCP_RXNBF_RX01BF_DISABLE 	= 0x00,//RX0BF、RX1BF引脚功能禁止
	
	MCP_RXNBF_RX1BF_INT_EN 		= 0x02,//RX1BF引脚在RXBF1有数据时，产生中断
	MCP_RXNBF_RX0BF_INT_EN 		= 0x01,//RX0BF引脚在RXBF0有数据时，产生中断
}MCP_RXNBF_RXPINTMODE_TYPE;

/**发送中断输出引脚TXRSTCTRL 配置中使用的枚举***/
typedef enum
{
	MCP_TXRSTCTRL_TX2RST_ENABLE = 0x04,//TX2RST输出控制引脚使能位
	MCP_TXRSTCTRL_TX1RST_ENABLE = 0x02,//TX1RST输出控制引脚使能位
	MCP_TXRSTCTRL_TX0RST_ENABLE = 0x01,//TX0RST输出控制引脚使能位
	
	MCP_TXRSTCTRL_TX012RST_DISABLE = 0x00//TX0RST、TX0RST、TX2RST引脚功能禁止
}MCP_TXRSTCTRL_TXPINTMODE_TYPE;

/**发送控制寄存器中，优先级***/
enum
{
	MCP_TXBNCTRL_PRIOTY_LOWEST = 0x00,//最低的发送优先级
	MCP_TXBNCTRL_PRIOTY_LOW = 0x01,//低的发送优先级
	MCP_TXBNCTRL_PRIOTY_HIGH = 0x02,//高的发送优先级
	MCP_TXBNCTRL_PRIOTY_HIGHEST = 0x03,//最高的发送优先级
};

/**发送控制，请求发送***/
enum
{
	MCP_TXBNCTRL_CLEAR_TXBUF = 0x00,
	MCP_TXBNCTRL_TXREQ = 0x08
};

/**发送，装载TX缓冲区指令中，缓冲区定义***/
typedef enum
{
	MCP_TXLOAD_TXB0SIDH = 0x00,//从TXB0SIDH开始装载(全部帧信息)
	MCP_TXLOAD_TXB1SIDH = 0x02,//从TXB1SIDH开始装载(全部帧信息)	
	MCP_TXLOAD_TXB2SIDH = 0x04//从TXB2SIDH开始装载(全部帧信息)	
	/**此处未定义仅装载数据的指令，防止误操作****/
}MCP_TXLOAD_TYPE;

/**发送，请求发送指令中，缓冲区定义***/
typedef enum
{
	MCP_TXRTS_TXB0 = 0x01,//发送缓冲区0
	MCP_TXRTS_TXB1 = 0x02,//发送缓冲区1
	MCP_TXRTS_TXB2 = 0x04//发送缓冲区2	
}MCP_TXRTS_TYPE;

/**中断使能寄存器CANINTE配置中使用的枚举****/
typedef enum
{
	MCP_CANINTE_INT_DISABLE 	= 0x00,//关闭所有的中断使能
	
	MCP_CANINTE_RX0IE_ENABLE 	= 0x01,//接收缓冲区0中断使能
	MCP_CANINTE_RX1IE_ENABLE 	= 0x02,//接收缓冲区1中断使能
	MCP_CANINTE_TX0IE_ENABLE 	= 0x04,//发送缓冲区0空中断使能
	MCP_CANINTE_TX1IE_ENABLE 	= 0x08,//发送缓冲区1空中断使能
	
	MCP_CANINTE_TX2IE_ENABLE 	= 0x10,//发送缓冲区2空中断使能
	MCP_CANINTE_ERRIE_ENABLE 	= 0x20,//错误中断使能
	MCP_CANINTE_WAKEIE_ENABLE 	= 0x40,//唤醒中断使能
	MCP_CANINTE_MERRE_ENABLE 	= 0x80,//报文错误中断使能(接收或发送报文错误)
}MCP_CANINTE_ENABLE_TYPE;

/**发送缓冲区0空中断标志类型定义****/
typedef enum
{
	MCP_TXINTF_DISABLE = 0x00,//中断标志设置为1
	MCP_TXINTF_ENABLE = 0x01//中断标志设置为0
}MCP_TXINTF_FLAG_TYPE;

/**中断状态寄存器CANINTF配置中使用的枚举****/
typedef enum
{
	MCP_CANINTF_RX0IF		  	= 0x01,//接收缓冲区0中断标志
	MCP_CANINTF_RX1IF 			= 0x02,//接收缓冲区1中断标志
	MCP_CANINTF_TX0IF 			= 0x04,//发送缓冲区0空中断标志
	MCP_CANINTF_TX1IF 			= 0x08,//发送缓冲区1空中断标志
	 
	MCP_CANINTF_TX2IF		  	= 0x10,//发送缓冲区2空中断标志
	MCP_CANINTF_ERRIF 			= 0x20,//错误中断标志
	MCP_CANINTF_WAKEIF	 		= 0x40,//唤醒中断标志
	MCP_CANINTF_MERRF 			= 0x80,//报文错误中断标志
}MCP_CANINTF_FLAG_TYPE;;

/**当使用SPI中断方式时，定义的包类型***/
typedef enum
{
	MCP_EVENT_INFORMATION_RX0_RECVD ,//缓冲区0接收
	MCP_EVENT_INFORMATION_RX1_RECVD ,//缓冲区1接收
	MCP_EVENT_INFORMATION_TX0_EMPTY ,//发送缓冲区0空
	MCP_EVENT_INFORMATION_TX1_EMPTY ,//发送缓冲区1空		
	MCP_EVENT_INFORMATION_TX2_EMPTY ,//发送缓冲区2空	
	MCP_EVENT_INFORMATION_MESSAGE_ERR,//报文错误	
	MCP_EVENT_INFORMATION_CAN_RXERR,		//CAN总线错误，接收错误
	MCP_EVENT_INFORMATION_CAN_TXERR,		//CAN总线错误，发送错误
	MCP_EVENT_INFORMATION_CAN_CLOSED,		//CAN总线错误，总线关闭	
}MCP_EVENT_INFORMATION_TYPE;



/**预编译指令，本项目中是否使用RXnBF引脚进行中断接收****/
#define MCP2515_RXNBF_PIN_ENABLE		0x01
#define MCP2515_RXNBF_PIN_DISABLE		0x00


#endif
