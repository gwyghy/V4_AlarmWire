/********************************************************************************
* 文件名：	usart.h
* 作者：    qx
* 版本：   	V1.0
* 日期：    2020.07.23  
* 功能描述: 支架控制器逻辑层任务初始化和创建 
* 			 
*			 
*			 
*			 
*			 
* 修改说明：   
*
*       >>>>  在工程中的位置  <<<<
*             3-应用层
*             2-协议层
*             1-硬件驱动层
*********************************************************************************
* @copy
* <h2><center>&copy; COPYRIGHT 天津华宁电子有限公司 研发中心 软件部</center></h2>
*********************************************************************************/
#ifndef __USART_H__
#define __USART_H__
/********************************************************************************
* .h头文件
*********************************************************************************/
#include "includes.h"
#include "usartapp.h"
/********************************************************************************
* #define宏定义
*********************************************************************************/
#define  USARTKEY_TX_BUF_MAX              10   //发送队列最大值
#define  USARTWIRELESS_TX_BUF_MAX              10   //发送队列最大值
//timer信息结构体
typedef  struct
{
	uint16_t  u16Count;       //计数
 	uint8_t   u8Enable;        //使能
} stTIMER;

void UsartMessageForRfidRcved(void);
u8 getKeyDeleteFile(void);
u32 KEYUsartFetchData(stUARTKEY  *UsartRcvFrm);
u32 WIRELESSUsartFetchData(stUARTKEY *UsartRcvFrm);
void UartInit(void);
void Crc16UsartSendChick(const u8 *u8Buf, u32 u32Len, u16 *u16CheckOld);
void Crc16UsartReceiveChick(const u8 *u8Buf, u32 u32Len, u16 *u16CheckOld);

#endif
