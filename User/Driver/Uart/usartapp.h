/********************************************************************************
* 文件名：	usartapp.h
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
#ifndef __USARTAPP_H__
#define __USARTAPP_H__
/********************************************************************************
* .h头文件
*********************************************************************************/
#include "includes.h"
/********************************************************************************
* #define宏定义
*********************************************************************************/
 #define  SendHeartTimerMAX     200
 #define  SCDEVIC        0
 #define  KEYDEVIC       2
 #define  WIRELESSDEVIC        3
 #define  USARTSENDBUFSIZE          129
 
  #define  KEYRESET   1

/**帧类型定义**/
enum
{
	TYPE_HEART=0,
	TYPE_KEY_VALUE
};
/**应答定义**/
 enum
 {
 	NOACK=0,
 	ACK
 };
/**帧类别枚举**///键盘
enum
{
	HEARTBEAT,//心跳数据
	KEYVALUEANDSTATUS=1,//按键信息
	COMMUNICATIONLED,//LED信息
	NAVIGATIONLED,//LED导航
	SETINFORMAION,//设置信息
	UPDATAPROGRAM,//程序升级
	RSUPDATAPROGRAM,//CHONGXINSHENGJI
	RESTORATION//FUWIE
};


//无线
enum
{
	HEARTBEATWL,//心跳数据
	WIRELESSDATA=1,//无线数据
	STARTHINT,//上电提示
	RESERVED,//预留
	PARAMETERSETTING,//参数设置
	UPDATAPROGRAMWL,//程序升级
	RSUPDATAPROGRAMWL,//CHONGXINSHENGJI
	RESTORATIONWL //FUWEI
};
enum
{
	KEYVAL,
	WRIELESSVAL
};




//串口数据结构体8
typedef  struct
{
	uint8_t  FrameType;     //帧类别
	uint8_t  ACK;           //应答
	uint8_t  u8DLC;         //数据长度
	uint8_t  SendBuf[USARTSENDBUFSIZE];      //数据
	uint8_t  CRCHigh;         //CRC校验 高字节
	uint8_t  CRCLow;       //CRC校验  低字节
} stUARTDATA;

//串口发送结构体
typedef  struct
{
	uint8_t  Address;       //设备地址
 	uint8_t  Serial;        //流水号
    stUARTDATA  stFrame;	
} stUARTKEY;

//按键信息结构体
// typedef  struct
// {
// 	uint64_t  u64Key;       //按键值
//  	uint8_t   u8Status ;        //按键状态
// } stKEY;

void SendProgramTransmissionData(u8* pFrame,u8 len);
void SendLEDCommunicationData(u16 u16On, u16 u16Off);
void SendLEDNavigationData(u32 u32OnKey, u32 u32OffKey);
void SendResponseData(stUARTDATA pFrame, u8 Data, u8 Devic);
u32 UsartRecvSemCreate(void);
void usart_send_task(void *p_arg);
void usart_sendwl_task(void *p_arg);
void usart_receive_task(void *p_arg);
void usart_receivewl_task(void *p_arg);
void SendWirelessData(unsigned char *buf,u32 data);
void SendSetInformationData(u16 data);
void HeartBeatCommErrTimer(void);
int KEYRecvUsartDataProc(void);
#endif
