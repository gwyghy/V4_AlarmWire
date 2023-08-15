/********************************************************************************
* 文件名称：	Wireless_Driver.h
* 作	者：  
* 当前版本：   	V1.0
* 完成日期：    2015.04.24
* 功能描述: 	定义bsp.h头文件
* 历史信息：   
*           	版本信息     完成时间      原作者        注释
*
*       >>>>  在工程中的位置  <<<<
*             3-应用层
*          	  2-协议层
*          √  1-硬件驱动层
*********************************************************************************
* Copyright (c) 2014,天津华宁电子有限公司 All rights reserved.
*********************************************************************************/
#ifndef __WIRELESS_DRIVER_H__ 
#define __WIRELESS_DRIVER_H__


#define WIRELESS_USART					USART1
#define GPIO_PORT_WIRELESS_TX  			GPIOA
#define GPIO_PORT_WIRELESS_RX  			GPIOA
#define GPIO_PIN_WIRELESS_TX				GPIO_Pin_9
#define GPIO_PIN_WIRELESS_RX				GPIO_Pin_10



#define  USARTSENDBUFSIZE          140


//串口数据结构体
typedef  struct
{
	uint8_t  Address;       //设备地址
	uint8_t  Serial;        //流水号
	uint8_t  FrameType;     //帧类别
	uint8_t  ACK;           //应答
	uint8_t  u8DLC;         //数据长度
	uint8_t  SendBuf[USARTSENDBUFSIZE];      //数据
	uint8_t  CRCHigh;         //CRC校验 高字节
	uint8_t  CRCLow;       //CRC校验  低字节
} STR_Usart_Frame;
void DriverUsart_Init(uint32_t bund);

#endif



