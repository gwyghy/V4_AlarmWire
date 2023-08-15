/********************************************************************************
* 文件名：	usartapp.c
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
/********************************************************************************
* .h头文件
*********************************************************************************/
#include "usartapp.h"
#include "usart.h"
#include "GUIKey.h"
#include "Util_Timer.h"
#include "IR_Rx.h"
/********************************************************************************
* #define宏定义
*********************************************************************************/
#define  STFRAME_LS_MAX						255//流水号的最大值
#define  SENDTIMES   3
#define  KEY_HEART_ERR_TIME_SET     4000
#define  WIRELESS_HEART_ERR_TIME_SET     8000
#define RFID_RCV_FRM_SIZE			50						// 存储池长度
u8 ACKFrameType;
u8 ACKData;
u8 ACKFrameType1;
u8 ACKData1;
//键盘
stUARTKEY  USARTKEYTxBuf[USARTKEY_TX_BUF_MAX];						//发送缓存
uint8_t	 USARTKEYTxWrtPtr = 0;			   						    //发送写指针
uint8_t	 USARTKEYTxRdPtr =  0;			   							//发送读指针

static u32 KeyHeartTime = KEY_HEART_ERR_TIME_SET;
extern OS_EVENT *s_stKEYUsartRecvd ;

extern OS_EVENT *s_stKEYUsartsend ;


extern OS_EVENT *s_stLEDRecvd ;

/********************************************************************************
* 函数功能： 重置键盘模块计时
* 
* 
********************************************************************************/
void KeyHeartCommErrTimer(void)
{
	#if OS_CRITICAL_METHOD == 3                      /* Allocate storage for CPU status register           */
	    OS_CPU_SR  cpu_sr = 0;
	#endif

	OS_ENTER_CRITICAL();	
	KeyHeartTime = KEY_HEART_ERR_TIME_SET;
	OS_EXIT_CRITICAL();	
}
/********************************************************************************
* 函数功能： 心跳计时递减
* 
* 
********************************************************************************/
void HeartBeatCommErrTimer(void)
{
//	if((KeyHeartTime != TIMER_EXPIRED) && (KeyHeartTime != TIMER_CLOSED))
//	{
//		if(GET_PT_BITS(PT_TMP_NOTE_KEY_ERR) == TRUE)
//			{
//				SET_PT_BITS(PT_TMP_NOTE_KEY_ERR, FALSE);
//				CanTrs_BitChange(PT_TMP_NOTE_KEY_ERR);
//			}	
//		KeyHeartTime--;
//	}
//	if(KeyHeartTime == TIMER_EXPIRED)
//	{
//		KeyHeartTime = TIMER_CLOSED;
//		SET_PT_BITS(PT_TMP_NOTE_KEY_ERR,  TRUE);
//		CanTrs_BitChange(PT_TMP_NOTE_KEY_ERR);
//	}	
//	if((WirelessHeartTime != TIMER_EXPIRED) && (WirelessHeartTime != TIMER_CLOSED))
//	{
//		if(GET_PT_BITS(PT_TMP_NOTE_WIRELESS_ERR) == TRUE)
//			{
//				SET_PT_BITS(PT_TMP_NOTE_WIRELESS_ERR, FALSE);
//				CanTrs_BitChange(PT_TMP_NOTE_WIRELESS_ERR);
//			}	
//		WirelessHeartTime--;
//	}
//	if(WirelessHeartTime == TIMER_EXPIRED)
//	{
//		WirelessHeartTime = TIMER_CLOSED;
//		SET_PT_BITS(PT_TMP_NOTE_WIRELESS_ERR,  TRUE);
//		CanTrs_BitChange(PT_TMP_NOTE_WIRELESS_ERR);
//	}	
}


/********************************************************************************
* 功能描述： 插入键盘usart发送队列
* 入口参数：
* 返回值：无
********************************************************************************/
static u8 KEYInsUsartTrsQueue(stUARTDATA *pFrame)
{
	static u8 u8LiuShuiNumb = 0x00;
	static u8 u8TxBufTxWrtPtr = 0x00;
	u16 u16CrcTemp = 0x00;
	u8 u8ReturnFlag = 0x01;
#if OS_CRITICAL_METHOD == 3
    OS_CPU_SR  cpu_sr = 0;
#endif
	/**防止队列满的情况出现**/
	u8TxBufTxWrtPtr = USARTKEYTxWrtPtr;
	u8TxBufTxWrtPtr++;
	u8TxBufTxWrtPtr %= USARTKEY_TX_BUF_MAX;		
	if((u8TxBufTxWrtPtr != USARTKEYTxRdPtr))
	{
		OS_ENTER_CRITICAL();
		u8LiuShuiNumb++;
		u8LiuShuiNumb %= STFRAME_LS_MAX;
		memset(&(USARTKEYTxBuf[USARTKEYTxWrtPtr].stFrame), 0x00  , sizeof(stUARTDATA));
		memcpy(&(USARTKEYTxBuf[USARTKEYTxWrtPtr].stFrame), pFrame, sizeof(stUARTDATA));
		USARTKEYTxBuf[USARTKEYTxWrtPtr].Address = SCDEVIC;
		USARTKEYTxBuf[USARTKEYTxWrtPtr].Serial = u8LiuShuiNumb;		
 		Crc16UsartSendChick((u8*)&(USARTKEYTxBuf[USARTKEYTxWrtPtr]),USARTKEYTxBuf[USARTKEYTxWrtPtr].stFrame.u8DLC+5,&u16CrcTemp);
		USARTKEYTxBuf[USARTKEYTxWrtPtr].stFrame.CRCHigh = (u8)((u16CrcTemp&0xFF00)>>8);
		USARTKEYTxBuf[USARTKEYTxWrtPtr].stFrame.CRCLow = (u8)(u16CrcTemp&0x00FF);
		USARTKEYTxWrtPtr ++;
		USARTKEYTxWrtPtr %= USARTKEY_TX_BUF_MAX;
		OS_EXIT_CRITICAL();
	}
	else
	{
		u8ReturnFlag =  FALSE;//缓冲区已满，不能再加入队列中
	}	
	return u8ReturnFlag;
}

/***************************************************************************************
** 功能描述：发送应答数据
** 函数输入：无
** 函数输出：None
** 注意事项：
******************************************************************************************/
void SendResponseData(stUARTDATA pFrame, u8 Data, u8 Devic)
{
	stUARTDATA stSendFrame;
	stSendFrame = pFrame;
	stSendFrame.ACK = 0x00;
	stSendFrame.u8DLC = 0x01;
	stSendFrame.SendBuf[0] = Data;
	if(Devic == KEYDEVIC)
	{
		KEYInsUsartTrsQueue(&stSendFrame);
	}
	
}
/********************************************************************************
* 功能描述： 发送LED通信
* 入口参数：
* 返回值：无
********************************************************************************/
void SendLEDCommunicationData(u16 u16On, u16 u16Off)
{
	stUARTDATA stSendFrame;
	memset(&stSendFrame,0x00,sizeof(stUARTDATA));
	stSendFrame.FrameType = COMMUNICATIONLED;//2
	stSendFrame.ACK = ACK;
	stSendFrame.u8DLC = 4;
	stSendFrame.SendBuf[0] = (u8)( u16On & 0x000000FF);
	stSendFrame.SendBuf[1] = (u8)((u16On & 0x0000FF00) >> 8);
	stSendFrame.SendBuf[2] = (u8)((u16Off & 0x000000FF) );
	stSendFrame.SendBuf[3] = (u8)((u16Off & 0x0000FF00) >> 8);
	
	KEYInsUsartTrsQueue(&stSendFrame);
}
/********************************************************************************
* 功能描述： 发送LED导航
* 入口参数：
* 返回值：无
********************************************************************************/
void SendLEDNavigationData(u32 u32OnKey, u32 u32OffKey)
{
	stUARTDATA stSendFrame;
	memset(&stSendFrame,0x00,sizeof(stUARTDATA));
	stSendFrame.FrameType = NAVIGATIONLED;//3
	stSendFrame.ACK = NOACK;
	stSendFrame.u8DLC = 8;
	stSendFrame.SendBuf[0] = (u8)( u32OnKey & 0x000000FF);
	stSendFrame.SendBuf[1] = (u8)((u32OnKey & 0x0000FF00) >> 8);
	stSendFrame.SendBuf[2] = (u8)((u32OnKey & 0x00FF0000) >> 16);
	stSendFrame.SendBuf[3] = (u8)((u32OnKey & 0xFF000000) >> 24);
	
	stSendFrame.SendBuf[4] = (u8)( u32OffKey & 0x000000FF);
	stSendFrame.SendBuf[5] = (u8)((u32OffKey & 0x0000FF00) >> 8);
	stSendFrame.SendBuf[6] = (u8)((u32OffKey & 0x00FF0000) >> 16);
	stSendFrame.SendBuf[7] = (u8)((u32OffKey & 0xFF000000) >> 24);
	
	KEYInsUsartTrsQueue(&stSendFrame);
}
/********************************************************************************
* 功能描述： 发送设置信息
* 入口参数：
* 返回值：无
********************************************************************************/
void SendSetInformationData(u16 val)
{
	u16 data = 1;
	stUARTDATA stSendFrame;
	memset(&stSendFrame,0x00,sizeof(stUARTDATA));
	stSendFrame.FrameType = RESTORATION;
	stSendFrame.ACK = ACK;
	stSendFrame.u8DLC = 4;
	stSendFrame.SendBuf[0] = (u8)( data & 0x000000FF);
	stSendFrame.SendBuf[1] = (u8)((data & 0x0000FF00) >> 8);
	stSendFrame.SendBuf[2] = (u8)((data & 0x00FF0000) >> 16);
	stSendFrame.SendBuf[3] = (u8)((data & 0xFF000000) >> 24);
	if(val == KEYVAL)
		KEYInsUsartTrsQueue(&stSendFrame);
}

/********************************************************************************
* 功能描述： 发送程序更新帧
* 入口参数：
* 返回值：无
********************************************************************************/
void SendProgramTransmissionData(u8* pFrame,u8 len)
{
	stUARTDATA stSendFrame;
	memset(&stSendFrame,0x00,sizeof(stUARTDATA));
	stSendFrame.FrameType = UPDATAPROGRAM;//3
	stSendFrame.ACK = ACK;
	memcpy(&(stSendFrame.SendBuf[0]),pFrame,len);	
	stSendFrame.SendBuf[len] = len;
	stSendFrame.u8DLC = len+1;	
	KEYInsUsartTrsQueue(&stSendFrame);
	OSTimeDly(100);
}

/********************************************************************************
* 功能描述： 解析按键键值信息
* 入口参数：
* 返回值：无
********************************************************************************/
int GetKeyValue(stUARTDATA pFrame)
{
	u32 data;
	int flag;
	data = pFrame.SendBuf[0];
	data |= pFrame.SendBuf[1] << 8;
	data |= pFrame.SendBuf[2] << 16;
	data |= pFrame.SendBuf[3] << 24;
	flag = GUI_KeyUser(data);
}
uint8_t  KeyVersion[3] = {0};
/********************************************************************************
* 功能描述： 解析版本信息
* 入口参数：
* 返回值：无
********************************************************************************/
void KEYGetHeartBeat(stUARTDATA pFrame)
{
	KeyVersion[0] = pFrame.SendBuf[5];
	KeyVersion[1] = pFrame.SendBuf[6];
	KeyVersion[2] = pFrame.SendBuf[7];
}

/********************************************************************************
** 功能描述：发送串口数据
** 输　入：  无
** 输　出：  无
*********************************************************************************/
static u8 KEYSendUsartDataProc(void)
{
	INT8U err;
	static u8 u8SendTime = 0x00;
	u8 u8ReturnFlag = 0x01;
#if OS_CRITICAL_METHOD == 3
    OS_CPU_SR  cpu_sr = 0;
#endif

	if((USARTKEYTxWrtPtr != USARTKEYTxRdPtr))
	{
		USART_SendData(USART3, *((u8*)&USARTKEYTxBuf[USARTKEYTxRdPtr]));
		USART_ITConfig(USART3, USART_IT_TXE, ENABLE);
		s_stKEYUsartsend->OSEventCnt = 0;
		OSSemPend(s_stKEYUsartsend,20,&err);
		
		if(USARTKEYTxBuf[USARTKEYTxRdPtr].stFrame.ACK == 1)
		{
			//等信号量
			OSSemPend(s_stKEYUsartRecvd, 40, &err);
//			OSTimeDly(10/TICK_TIME);
			if(err == OS_ERR_NONE)//接收数据，处理
			{
				if((USARTKEYTxBuf[USARTKEYTxRdPtr].stFrame.FrameType == ACKFrameType)
					&& ACKData == 0x01)
				{
					USARTKEYTxRdPtr ++;
					USARTKEYTxRdPtr %= USARTKEY_TX_BUF_MAX;
					u8SendTime = 0;
				}
				else
				{
					u8SendTime++;
				}	
			}
			else
			{
				u8SendTime++;
			}
			if(u8SendTime >= SENDTIMES)
			{
					USARTKEYTxRdPtr ++;
					USARTKEYTxRdPtr %= USARTKEY_TX_BUF_MAX;
					u8SendTime = 0;
			}
		}	
		else
		{
				USARTKEYTxRdPtr ++;
				USARTKEYTxRdPtr %= USARTKEY_TX_BUF_MAX;
				u8SendTime = 0;
		}			
	}
	else
	{
		u8ReturnFlag =  0x00;//缓冲区已满，不能再加入队列中
	}	
	
	return u8ReturnFlag;
}

/********************************************************************************
* 功能描述： 定时器超时回调
* 入口参数：
* 返回值：无
********************************************************************************/
void usartHeartTimeout_callback(uint16_t id)
{
	memset(KeyVersion,0,sizeof(KeyVersion));
}


/********************************************************************************
* 功能描述： 初始化创建定时器
* 入口参数：
* 返回值：无
********************************************************************************/
static u8 usartHeartTimerId;
void usartHeartTimerInit(void)
{
	usartHeartTimerId = addTimerEvent(5000,0xFF,usartHeartTimeout_callback);
	startTimerEvent(usartHeartTimerId);	
}


/********************************************************************************
* 功能描述： 接收数据处理
* 入口参数：
* 返回值：无
********************************************************************************/
int KEYRecvUsartDataProc(void)
{
	int flag = 0;
	stUARTKEY  UsartRcvFrm;
	static u8 u8DestScNumbBackup;
	if(KEYUsartFetchData(&UsartRcvFrm) == FALSE)
		return 0x00;
	if(UsartRcvFrm.Address == FALSE)
		return 0x00;
	startTimerEvent(usartHeartTimerId);//重新开始计时
	KeyHeartCommErrTimer();	
	if(u8DestScNumbBackup == UsartRcvFrm.Serial)
		return 0x00;	
	u8DestScNumbBackup = UsartRcvFrm.Serial;
	if((UsartRcvFrm.stFrame.ACK != 1) &&(UsartRcvFrm.stFrame.FrameType != 0)&& (UsartRcvFrm.stFrame.u8DLC == 1))
	{
		ACKData	= UsartRcvFrm.stFrame.SendBuf[0];
		ACKFrameType = UsartRcvFrm.stFrame.FrameType;
 		UsartMessageForRfidRcved();
	}
	switch(UsartRcvFrm.stFrame.FrameType)
	{
		case HEARTBEAT:
			KEYGetHeartBeat(UsartRcvFrm.stFrame);	
		break;	
		case KEYVALUEANDSTATUS:
			//接收按键处理函数
			flag = GetKeyValue(UsartRcvFrm.stFrame);	
		break;
		case RSUPDATAPROGRAM:
//			u32DownProgramType = KEY_DEV_TYPE;
//			CanResendData();
		break;
		default:

		break;
	}
	return flag;
}

/********************************************************************************
** 功能描述：串口数据处理主任务
** 输　入：  无
** 输　出：  无
*********************************************************************************/
void usart_send_task(void *p_arg)
{	
	while(1)
	{
		KEYSendUsartDataProc();
		PLATFORM_OS_TimeDly(10);
		IR_RxDataProc();
	}
}

void usart_receive_task(void *p_arg)
{	
	INT8U err;
	while(1)
	{
		OSSemPend(s_stLEDRecvd, 20, &err);
		KEYRecvUsartDataProc();
	}
}
