/************************************************************
** �ļs��:  ir_rx.c
** Copyright (c) 1998-1999 ��������Ӽ������޹�˾����������
** ������:  HNDZ
** �ա���:  2015.04
** GS����:	
** �ա���:	
** �衡��:  ������մ��ڣ�IR_USART����������մ������
**          ���ڵ�Һ�ؿ���v2.0��Ŀ��IR����оƬBC7210A��
** �桡��:	V1.0A
**-----------------------------------------------------------
*************************************************************/

#include "includes.h"
#include "IR_Rx.h"
#include "V4_Wireless.h"

#include "Wireless_Alarm.h"
#include "sys_msg_def.h"


#define IR_DEV_MAX 		0x01	//���ⷢ���豸���������

#define IR_RX_INFO_HEAD		0x80	//IR�������ң�����������ݵ���ʼ��־
#define IR_RX_MINE_HEAD		0x01	//IR������պ��ⷢ�������ݵ���ʼ��־,01
#define IR_RX_MINE_END		0xFE	//IR������պ��ⷢ�������ݵĽ�����־
#define IR_RX_INFO_LENGTH	0x03	//IR��������һ������֡���ݳ���

#define IR_RX_BUF_MAX		64		//IR���ջ���������
#define IR_TX_BUF_MAX		0		//IR���ͻ���������

#define IR_MINE_SN_MASK		0x10	//������գ�ģ�ⷢ����ˮ��

u8	IR_RxBuf[IR_RX_BUF_MAX];		//���ջ���
u32	IR_RxBufWpt = 0;				//����дָ��
u32	IR_RxBufRpt = 0;				//���ն�ָ��
u32	IR_RxBufCnt = 0;				//���ռ���

u32  u32IRId[IR_DEV_MAX]={0x00};			//���ⷢ����豸��
u32	 u32IRData[IR_DEV_MAX]={0x00};			//������յ��û����ݣ�0x00������0x01������
u32	 u32IRDataCtr[IR_DEV_MAX]={0x00};		//������ռ��i

/***********************************************************
** ��������IrRxAddIrInf
** �䡡�룺id�i��G�?status״̬��times����
** �䡡����None
** �����������ۼӺ�����յĽ��մ�����״̬
************************************************************/
void IrRxAddIrInf(u32 id, u32 status, u32 times)
{
	#if OS_CRITICAL_METHOD == 3                      /* Allocate storage for CPU status register */
		OS_CPU_SR  cpu_sr = 0;
	#endif
	
	if(id > IR_DEV_MAX)
		return;
	
	//��ȫ���жϣ�
	OS_ENTER_CRITICAL();
	u32IRId[id - 1] = id;
	u32IRData[id - 1] += status;//����ģ��֮��ͨѶ������񣬷�Ϊ0x1������0x0;���շ��͵�u32IRData ��һ���ϱ�����ڼ䲻��ȷ��ͨѶ
	u32IRDataCtr[id - 1]++;
	//��ȫ���G�ϣ�
	OS_EXIT_CRITICAL();	
	
}
/***********************************************************
** ��������IrRxClearIrInf
** �䡡�룺id�i��G�?
** �䡡����None
** �������������������յĽ��մ�����״̬
************************************************************/
void IrRxClearIrInf(u32 id)
{
	#if OS_CRITICAL_METHOD == 3                      /* Allocate storage for CPU status register */
		OS_CPU_SR  cpu_sr = 0;
	#endif
	
	if(id > IR_DEV_MAX)
		return;
	
	//��ȫ���жϣ�
	OS_ENTER_CRITICAL();
	u32IRId[id - 1] = 0x00;
	u32IRData[id - 1] = 0x00;
	u32IRDataCtr[id - 1] = 0x00;
	//��ȫ���жϣ�
	OS_EXIT_CRITICAL();		
}
/***********************************************************
** ��������IrRxGetIrDevId
** �䡡�룺id�i��G�?1-3
** �䡡����None
** ������������ȡ�����G���豸ID
************************************************************/
u32 IrRxGetIrDevId(u8 id)
{
	if(id > IR_DEV_MAX)
		return 0xFF;
	
	return u32IRId[id - 1];
}

/***********************************************************
** ��������IrRxGetIrRevStatus
** �䡡�룺id�i��G�?1-3
** �䡡����None
** ������������ȡ�����G���豸���մ���
************************************************************/
u32 IrRxGetIrRevStatus(u8 id)
{
	if(id > IR_DEV_MAX)
		return 0xFF;
	
	return u32IRData[id - 1];
}

/***********************************************************
** ��������IrRxGetIrRevTimes
** �䡡�룺id����G�?1-3
** �䡡����None
** ������������ȡ�����G���豸���մ���
************************************************************/
u32 IrRxGetIrRevTimes(u8 id)
{
	if(id > IR_DEV_MAX)
		return 0xFF;
	
	return u32IRDataCtr[id - 1];
}

/***********************************************************
** ���i����IR_RxDataProc
** �䡡�룺None
** �䡡����None
** ��������������������ݴ������º���Ӧ��������������ִ�С�
************************************************************/
void IR_RxDataProc(void)
{
	u16	u16i, u16j;
	IR_INFO_u RxIrInfo;
	EXT_CAN_FRAME_s RxIrCan;
#if OS_CRITICAL_METHOD == 3                      /* Allocate storage for CPU status register */
    OS_CPU_SR  cpu_sr = 0;
#endif

	OS_ENTER_CRITICAL();
	u16j = IR_RxBufCnt;
	OS_EXIT_CRITICAL();
   	while (u16j >= IR_RX_INFO_LENGTH)
	{
		u16i = IR_RxBufRpt;
		if((((IR_RxBuf[u16i] & IR_RX_INFO_HEAD) == IR_RX_INFO_HEAD) && (IR_RxBuf[u16i] != IR_RX_MINE_END))\
			||((IR_RxBuf[u16i] & IR_RX_MINE_HEAD) == IR_RX_MINE_HEAD))
		{
			RxIrInfo.u8IrInfo[0] = IR_RxBuf[u16i++];		//��ַ��1
			u16i %= IR_RX_BUF_MAX;
			if ((IR_RxBuf[u16i] & IR_RX_INFO_HEAD) != IR_RX_INFO_HEAD)
			{
				RxIrInfo.u8IrInfo[1] = IR_RxBuf[u16i++];	//��ַ��2
				u16i %= IR_RX_BUF_MAX;
				if((((IR_RxBuf[u16i] & IR_RX_INFO_HEAD) != IR_RX_INFO_HEAD) && (IR_RxBuf[u16i] != 0x00))\
				  ||(IR_RxBuf[u16i] == IR_RX_MINE_END))
				{
					RxIrInfo.u8IrInfo[2] = IR_RxBuf[u16i++];//�û��ֽ�
					u16i %= IR_RX_BUF_MAX;
					IR_RxBufRpt = u16i;						//���¶�ȡָ��
					OS_ENTER_CRITICAL();
					IR_RxBufCnt = IR_RxBufCnt - IR_RX_INFO_LENGTH;
					u16j = IR_RxBufCnt;
					OS_EXIT_CRITICAL();
					//��Ӵ��롣ת��Ϊ��չCAN���ݸ�ʽ
					if (RxIrInfo.sIrInfo.Type < 9)
					{
						switch (RxIrInfo.sIrInfo.Type)
						{
							case 0x01://�����������֡ �����ú����λ���ݣ�����Ϊ0x80 �� 0x03
								if((RxIrInfo.u8IrInfo[0] & IR_RX_INFO_HEAD) == IR_RX_INFO_HEAD)//ң������������
								{
									RxIrCan.u8DT[0] = 0x0;
									RxIrCan.u8DT[1] = 0x0;
									RxIrCan.u8DT[2] = RxIrInfo.sIrInfo.ScNoIncDir<<7;
									RxIrCan.u8DT[3] = 0x0;
									RxIrCan.u8DT[4] = 0x0;
									RxIrCan.u8DT[5] = RxIrInfo.sIrInfo.RemoteID;
									RxIrCan.u8DT[6] = 0x0;
									RxIrCan.u8DT[7] = 0x0;
									RxIrCan.u16DLC = 8;
									RxIrCan.u32ID.ID.NC = 0x00;
									RxIrCan.u32ID.ID.RD = 0;
									RxIrCan.u32ID.ID.ACK = 0;
									RxIrCan.u32ID.ID.FT = V4YKQ_IR_CODEMATCH;
									RxIrCan.u32ID.ID.RID = 0;
									RxIrCan.u32ID.ID.TID = 3;
									RxIrCan.u32ID.ID.SUM = 0;
									RxIrCan.u32ID.ID.SUB = 0;
									RxIrCan.u32ID.ID.SN = 0;
								
									if(SysParam.globalParam.SupportParam.Paring_Method == 0)    //ʹ�ô���ͨ��ʱ������ģ��																						
										InsUsartIrRecvData_proc((STR_WLCAN_Frame *)&RxIrCan,sizeof(RxIrCan));
									else if(SysParam.globalParam.SupportParam.Paring_Method ==1) // ʹ��CANͨ��ʱ������ģ��
									    InsAlarmWirelessIrRecvData_proc((STR_WLCAN_Frame *)&RxIrCan,sizeof(RxIrCan));
								}
								else if((RxIrInfo.u8IrInfo[0] & IR_RX_MINE_HEAD) == IR_RX_MINE_HEAD)
								{
									u32 i = 0x00;
									i = (((u32)RxIrInfo.u8IrInfo[1])<<8) + (u32)RxIrInfo.u8IrInfo[0];
									if (i > 0 && i < 4)
									{
										IrRxAddIrInf(i,(u32)RxIrInfo.u8IrInfo[2],0x01);
									}
								}
								else
									;
								break;
							
							default:
								break;

						}
					}
					else
					{
						IR_RxBufRpt++;
						IR_RxBufRpt %= IR_RX_BUF_MAX;
						OS_ENTER_CRITICAL();
						if(IR_RxBufCnt > 0)//mry,20.07.23,IR_RxBufCnt�������ܳ���IR_RX_BUF_MAX
						{
							IR_RxBufCnt--;
							u16j = IR_RxBufCnt;
						}
						else
						{
							u16j = 0x00;
						}
						OS_EXIT_CRITICAL();
					}
					continue;
				}
			}
		}
		IR_RxBufRpt++;
		IR_RxBufRpt %= IR_RX_BUF_MAX;
		OS_ENTER_CRITICAL();
		if(IR_RxBufCnt > 0)//mry,20.07.23,IR_RxBufCnt�������ܳ���IR_RX_BUF_MAX
		{
			IR_RxBufCnt--;
			u16j = IR_RxBufCnt;
		}
		else
		{
			u16j = 0x00;
		}
		OS_EXIT_CRITICAL();
	}
}
/***********************************************************
** ��������IR_RxUartInit
** �䡡�룺None
** �䡡����None
** ���������� 
************************************************************/
void IR_RxUartInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;   
//#ifndef DYK_SS
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef  NVIC_InitStructure;
//#endif
	
	RCC_AHB1PeriphClockCmd(IR_RX_GPIO_RCC, ENABLE);			//���ڶ˿�ʱ��ʹ��

	RCC_APB2PeriphClockCmd(IR_RX_RCC, ENABLE);				//���⴮��ʱ��ʹ��
		
	/* Configure IR_TX_PIN in output pushpull mode */
	GPIO_InitStructure.GPIO_Pin = IR_RX_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(IR_RX_PORT, &GPIO_InitStructure);


	GPIO_PinAFConfig(IR_RX_PORT, IR_RX_SOURCE, IR_RX_GPIO_AF);
	
	USART_InitStructure.USART_BaudRate = 9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;		//��У��
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx;			// | USART_Mode_Tx; 
	USART_Init(IR_RX_USART, &USART_InitStructure);			//����

	USART_ITConfig(IR_RX_USART, USART_IT_RXNE, ENABLE);		//�ж�ʹ��


	/* Configure the NVIC Preemption Priority Bits */  
	/* Enable the USARTy Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = IR_RX_USART_IRQ;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x05;	//��ʹ���ж����ȼ�Ƕ�ס���ΪSysTick���ж����ȼ�Ϊ0x0f
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;//0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	USART_Cmd(IR_RX_USART, ENABLE);							//����ʹ��
}
/***********************************************************
** ��������IR_RxCmd
** �䡡�룺StateVal=ENABLE�����������գ�=DISABLE����ֹ�������
** �䡡����None
** ���������� �������������գ����ֹ�������
************************************************************/
void IR_RxCmd(FunctionalState StateVal)
{
	if (StateVal == DISABLE)
	{
		USART_ITConfig(IR_RX_USART, USART_IT_RXNE, DISABLE);//��ֹ�����ж�
		USART_ITConfig(IR_RX_USART, USART_IT_TXE, DISABLE);	//��ֹ�����ж�
		USART_ITConfig(IR_RX_USART, USART_IT_TC, DISABLE);	//��ֹ��������ж�
		while(USART_GetFlagStatus(IR_RX_USART, USART_FLAG_TC) == RESET);//�ȴ����ͽ���
		(void)IR_RX_USART->SR;								//���������־
		(void)IR_RX_USART->DR;								//���������־
	}else
	{
		(void)IR_RX_USART->SR;								//���������־
		(void)IR_RX_USART->DR;								//���������־
		USART_ITConfig(IR_RX_USART, USART_IT_RXNE, ENABLE);	//�����ж�ʹ��
	}
	USART_Cmd(IR_RX_USART, StateVal);						//��������
}
/***********************************************************
** ��������	 IR_RX_USART_IRQHandler
** �������������⴮�ڽ����жϳ���
************************************************************/
void IR_RX_USART_IRQHandler(void)
{
	OS_CPU_SR  cpu_sr;

	OS_ENTER_CRITICAL();						/* Tell uC/OS-II that we are starting an ISR */
	OSIntEnter();
	OS_EXIT_CRITICAL();


	if ((USART_GetITStatus(IR_RX_USART, USART_IT_RXNE) != RESET) 
		|| (USART_GetITStatus(IR_RX_USART, USART_IT_ORE_RX) != RESET))
	{
		u16 data;
		data = USART_ReceiveData(IR_RX_USART);
		
		OS_ENTER_CRITICAL();					/* Tell uC/OS-II that we are starting an ISR */
		if (IR_RxBufCnt < IR_RX_BUF_MAX)
		{
			IR_RxBuf[IR_RxBufWpt++] = (u8)(data & 0xff);
			IR_RxBufWpt %= IR_RX_BUF_MAX;
			IR_RxBufCnt++;
		}
		OS_EXIT_CRITICAL();
	}

	OSIntExit();
}
