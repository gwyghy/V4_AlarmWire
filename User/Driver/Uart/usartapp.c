/********************************************************************************
* �ļ�����	usartapp.c
* ���ߣ�    qx
* �汾��   	V1.0
* ���ڣ�    2020.07.23  
* ��������: ֧�ܿ������߼��������ʼ���ʹ��� 
* 			 
*			 
*			 
*			 
*			 
* �޸�˵����   
*
*       >>>>  �ڹ����е�λ��  <<<<
*             3-Ӧ�ò�
*             2-Э���
*             1-Ӳ��������
*********************************************************************************
* @copy
* <h2><center>&copy; COPYRIGHT ������������޹�˾ �з����� �����</center></h2>
*********************************************************************************/
/********************************************************************************
* .hͷ�ļ�
*********************************************************************************/
#include "usartapp.h"
#include "usart.h"
#include "GUIKey.h"
#include "Util_Timer.h"
#include "IR_Rx.h"
/********************************************************************************
* #define�궨��
*********************************************************************************/
#define  STFRAME_LS_MAX						255//��ˮ�ŵ����ֵ
#define  SENDTIMES   3
#define  KEY_HEART_ERR_TIME_SET     4000
#define  WIRELESS_HEART_ERR_TIME_SET     8000
#define RFID_RCV_FRM_SIZE			50						// �洢�س���
u8 ACKFrameType;
u8 ACKData;
u8 ACKFrameType1;
u8 ACKData1;
//����
stUARTKEY  USARTKEYTxBuf[USARTKEY_TX_BUF_MAX];						//���ͻ���
uint8_t	 USARTKEYTxWrtPtr = 0;			   						    //����дָ��
uint8_t	 USARTKEYTxRdPtr =  0;			   							//���Ͷ�ָ��

static u32 KeyHeartTime = KEY_HEART_ERR_TIME_SET;
extern OS_EVENT *s_stKEYUsartRecvd ;

extern OS_EVENT *s_stKEYUsartsend ;


extern OS_EVENT *s_stLEDRecvd ;

/********************************************************************************
* �������ܣ� ���ü���ģ���ʱ
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
* �������ܣ� ������ʱ�ݼ�
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
* ���������� �������usart���Ͷ���
* ��ڲ�����
* ����ֵ����
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
	/**��ֹ���������������**/
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
		u8ReturnFlag =  FALSE;//�����������������ټ��������
	}	
	return u8ReturnFlag;
}

/***************************************************************************************
** ��������������Ӧ������
** �������룺��
** ���������None
** ע�����
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
* ���������� ����LEDͨ��
* ��ڲ�����
* ����ֵ����
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
* ���������� ����LED����
* ��ڲ�����
* ����ֵ����
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
* ���������� ����������Ϣ
* ��ڲ�����
* ����ֵ����
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
* ���������� ���ͳ������֡
* ��ڲ�����
* ����ֵ����
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
* ���������� ����������ֵ��Ϣ
* ��ڲ�����
* ����ֵ����
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
* ���������� �����汾��Ϣ
* ��ڲ�����
* ����ֵ����
********************************************************************************/
void KEYGetHeartBeat(stUARTDATA pFrame)
{
	KeyVersion[0] = pFrame.SendBuf[5];
	KeyVersion[1] = pFrame.SendBuf[6];
	KeyVersion[2] = pFrame.SendBuf[7];
}

/********************************************************************************
** �������������ʹ�������
** �䡡�룺  ��
** �䡡����  ��
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
			//���ź���
			OSSemPend(s_stKEYUsartRecvd, 40, &err);
//			OSTimeDly(10/TICK_TIME);
			if(err == OS_ERR_NONE)//�������ݣ�����
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
		u8ReturnFlag =  0x00;//�����������������ټ��������
	}	
	
	return u8ReturnFlag;
}

/********************************************************************************
* ���������� ��ʱ����ʱ�ص�
* ��ڲ�����
* ����ֵ����
********************************************************************************/
void usartHeartTimeout_callback(uint16_t id)
{
	memset(KeyVersion,0,sizeof(KeyVersion));
}


/********************************************************************************
* ���������� ��ʼ��������ʱ��
* ��ڲ�����
* ����ֵ����
********************************************************************************/
static u8 usartHeartTimerId;
void usartHeartTimerInit(void)
{
	usartHeartTimerId = addTimerEvent(5000,0xFF,usartHeartTimeout_callback);
	startTimerEvent(usartHeartTimerId);	
}


/********************************************************************************
* ���������� �������ݴ���
* ��ڲ�����
* ����ֵ����
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
	startTimerEvent(usartHeartTimerId);//���¿�ʼ��ʱ
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
			//���հ���������
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
** �����������������ݴ���������
** �䡡�룺  ��
** �䡡����  ��
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
