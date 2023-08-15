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
#include "usart.h"
#include "usartapp.h"
/********************************************************************************
* #define�궨��
*********************************************************************************/
OS_EVENT *s_stKEYUsartRecvd ;
OS_EVENT *s_stKEYUsartsend ;
OS_EVENT *s_stLEDRecvd ;
stTIMER  KEYTimer;


//����
extern stUARTKEY  USARTKEYTxBuf[USARTKEY_TX_BUF_MAX];						//���ͻ���
extern uint8_t	 USARTKEYTxWrtPtr;			   						    //����дָ��
extern uint8_t	 USARTKEYTxRdPtr;			   							//���Ͷ�ָ��



//����
#define  USARTKEY_TMP_BUF_MAX            140    //������ʱ�����������ֵ
#define  USARTKEY_RX_BUF_MAX              10    //���̽��ն������ֵ
uint8_t  KEYUsartReveiveDataBuf[USARTKEY_TMP_BUF_MAX];   //��ʱ��������
uint8_t  KEYtmpTxWrtPtr  = 0;  //��ʱ����дָ��
stUARTKEY  USARTKEYRxBuf[USARTKEY_RX_BUF_MAX];						//���ջ���
uint8_t	 USARTKEYRxWrtPtr = 0;			   						    //����дָ��
uint8_t	 USARTKEYRxRdPtr =  0;			   							//���ն�ָ��


u16 const u16CrcUsarttab[256] = 
{
0x0000,0x1021,0x2042,0x3063,0x4084,0x50a5,0x60c6,0x70e7,
0x8108,0x9129,0xa14a,0xb16b,0xc18c,0xd1ad,0xe1ce,0xf1ef,
0x1231,0x0210,0x3273,0x2252,0x52b5,0x4294,0x72f7,0x62d6,
0x9339,0x8318,0xb37b,0xa35a,0xd3bd,0xc39c,0xf3ff,0xe3de,
0x2462,0x3443,0x0420,0x1401,0x64e6,0x74c7,0x44a4,0x5485,
0xa56a,0xb54b,0x8528,0x9509,0xe5ee,0xf5cf,0xc5ac,0xd58d,
0x3653,0x2672,0x1611,0x0630,0x76d7,0x66f6,0x5695,0x46b4,
0xb75b,0xa77a,0x9719,0x8738,0xf7df,0xe7fe,0xd79d,0xc7bc,
0x48c4,0x58e5,0x6886,0x78a7,0x0840,0x1861,0x2802,0x3823,
0xc9cc,0xd9ed,0xe98e,0xf9af,0x8948,0x9969,0xa90a,0xb92b,
0x5af5,0x4ad4,0x7ab7,0x6a96,0x1a71,0x0a50,0x3a33,0x2a12,
0xdbfd,0xcbdc,0xfbbf,0xeb9e,0x9b79,0x8b58,0xbb3b,0xab1a,
0x6ca6,0x7c87,0x4ce4,0x5cc5,0x2c22,0x3c03,0x0c60,0x1c41,
0xedae,0xfd8f,0xcdec,0xddcd,0xad2a,0xbd0b,0x8d68,0x9d49,
0x7e97,0x6eb6,0x5ed5,0x4ef4,0x3e13,0x2e32,0x1e51,0x0e70,
0xff9f,0xefbe,0xdfdd,0xcffc,0xbf1b,0xaf3a,0x9f59,0x8f78,
0x9188,0x81a9,0xb1ca,0xa1eb,0xd10c,0xc12d,0xf14e,0xe16f,
0x1080,0x00a1,0x30c2,0x20e3,0x5004,0x4025,0x7046,0x6067,
0x83b9,0x9398,0xa3fb,0xb3da,0xc33d,0xd31c,0xe37f,0xf35e,
0x02b1,0x1290,0x22f3,0x32d2,0x4235,0x5214,0x6277,0x7256,
0xb5ea,0xa5cb,0x95a8,0x8589,0xf56e,0xe54f,0xd52c,0xc50d,
0x34e2,0x24c3,0x14a0,0x0481,0x7466,0x6447,0x5424,0x4405,
0xa7db,0xb7fa,0x8799,0x97b8,0xe75f,0xf77e,0xc71d,0xd73c,
0x26d3,0x36f2,0x0691,0x16b0,0x6657,0x7676,0x4615,0x5634,
0xd94c,0xc96d,0xf90e,0xe92f,0x99c8,0x89e9,0xb98a,0xa9ab,
0x5844,0x4865,0x7806,0x6827,0x18c0,0x08e1,0x3882,0x28a3,
0xcb7d,0xdb5c,0xeb3f,0xfb1e,0x8bf9,0x9bd8,0xabbb,0xbb9a,
0x4a75,0x5a54,0x6a37,0x7a16,0x0af1,0x1ad0,0x2ab3,0x3a92,
0xfd2e,0xed0f,0xdd6c,0xcd4d,0xbdaa,0xad8b,0x9de8,0x8dc9,
0x7c26,0x6c07,0x5c64,0x4c45,0x3ca2,0x2c83,0x1ce0,0x0cc1,
0xef1f,0xff3e,0xcf5d,0xdf7c,0xaf9b,0xbfba,0x8fd9,0x9ff8,
0x6e17,0x7e36,0x4e55,0x5e74,0x2e93,0x3eb2,0x0ed1,0x1ef0
};
/********************************************************************************
** �������������ڳ�ʼ��
** �䡡�룺  ��
** �䡡����  ��
*********************************************************************************/
void UartKeyInit(void)
{
	   //GPIO�˿�����
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	DMA_InitTypeDef  DMA_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE); //ʹ��GPIOAʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);//ʹ��USART3ʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);
	//����1��Ӧ���Ÿ���ӳ��
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource10,GPIO_AF_USART3); //GPIOA10����ΪUSART1
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource11,GPIO_AF_USART3); //GPIOA11����ΪUSART1
	
	DMA_DeInit(DMA1_Stream1); //DMA1 ������ 3
	DMA_InitStructure.DMA_Channel = DMA_Channel_4;  
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&USART3->DR;  //DMA��a����ADC?����??��
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)KEYUsartReveiveDataBuf;//
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
	DMA_InitStructure.DMA_BufferSize = USARTKEY_TMP_BUF_MAX;//
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable ;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_Mode =  DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
	
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;         
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	DMA_Init(DMA1_Stream1, &DMA_InitStructure);
	USART_DMACmd(USART3,USART_DMAReq_Rx,ENABLE);   //��1?����??��1 DMA?����?
	DMA_Cmd(DMA1_Stream1, ENABLE);  //?y��??y?��DMA��?��?
	
	//USART1�˿�����
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11; //GPIOA11��GPIOA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//�ٶ�50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
	GPIO_Init(GPIOB,&GPIO_InitStructure); //��ʼ��PA11��PA10
	

   //USART1 ��ʼ������
	USART_InitStructure.USART_BaudRate = 115200;//����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
	USART_Init(USART3, &USART_InitStructure); //��ʼ������1
	

	//Usart1 NVIC ����
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;//����3�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x02;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���

	
	USART_ClearFlag(USART3, USART_FLAG_TC);
	USART_ClearFlag(USART3, USART_FLAG_RXNE);
	USART_ClearFlag(USART3, USART_FLAG_IDLE);
	USART_ClearFlag(USART3, USART_FLAG_ORE);
	USART_ITConfig(USART3, USART_IT_IDLE, ENABLE);//��������ж�
	USART_Cmd(USART3, ENABLE);  //ʹ�ܴ���1 
}
void MYDMA_Enable(DMA_Stream_TypeDef*DMA_CHx)
{ 
	DMA_Cmd(DMA_CHx, DISABLE );  //1?��?USART1 TX DMA1 ?��??��?��?�����̨�      
 	DMA_SetCurrDataCounter(DMA_CHx,USARTKEY_TMP_BUF_MAX);//DMA�����̨���?DMA?o��?��?�䨮D?
 	DMA_Cmd(DMA_CHx, ENABLE);  //��1?��USART1 TX DMA1 ?��??��?��?�����̨� 
}
/********************************************************************************
** �������������ڳ�ʼ��
** �䡡�룺  ��
** �䡡����  ��
*********************************************************************************/
void UartInit(void)
{
	UartKeyInit();
}

/********************************************************************************
** ��������������һ���ź���
** �䡡�룺  ��
** �䡡����  ��
*********************************************************************************/
u32 UsartRecvSemCreate(void)
{
	s_stKEYUsartRecvd  = OSSemCreate(0);	//�������������ź��� 
	s_stLEDRecvd  = OSSemCreate(0);	//�������������ź���
	s_stKEYUsartsend = OSSemCreate(0);
	return 0x01;
}
/********************************************************************************
** ��������������һ���ź���
** �䡡�룺  ��
** �䡡����  ��
*********************************************************************************/
void UsartMessageForRfidRcved(void)
{
	OSSemPost(s_stKEYUsartRecvd);	//RFID���յ�����
}

/*******************************************************************************************   




**�������ã�����CRC16У��ֵ
**����������u8Buf:��ҪУ���������ʼ��ַ��u32Len:��ҪУ�����ݵĳ��ȣ�*u16CheckOld:������CRCУ��ֵ
**�����������
**ע�������
*******************************************************************************************/
void Crc16UsartSendChick(const u8 *u8Buf, u32 u32Len, u16 *u16CheckOld)
{
	u32 u32Cnt = 0x00;
	u16 u16Crc = *u16CheckOld;
	for( u32Cnt = 0; u32Cnt < u32Len; u32Cnt++)
		u16Crc = (u16Crc<<8) ^ u16CrcUsarttab[((u16Crc>>8) ^ *(u8 *)u8Buf++)&0x00FF];
	*u16CheckOld = u16Crc;
}
/*******************************************************************************************
**�������ã�����CRC16У��ֵ
**����������u8Buf:��ҪУ���������ʼ��ַ��u32Len:��ҪУ�����ݵĳ��ȣ�*u16CheckOld:������CRCУ��ֵ
**�����������
**ע�������
*******************************************************************************************/
void Crc16UsartReceiveChick(const u8 *u8Buf, u32 u32Len, u16 *u16CheckOld)
{
	u32 u32Cnt = 0x00;
	u16 u16Crc = *u16CheckOld;
	for( u32Cnt = 0; u32Cnt < u32Len; u32Cnt++)
		u16Crc = (u16Crc<<8) ^ u16CrcUsarttab[((u16Crc>>8) ^ *(u8 *)u8Buf++)&0x00FF];
	*u16CheckOld = u16Crc;
}



u8 getKeyDeleteFile(void)
{
	stUARTKEY  UsartRcvFrm;
	u32 data;
	int flag;
	if(KEYUsartFetchData(&UsartRcvFrm) == FALSE)
		return 0x00;
	if(UsartRcvFrm.stFrame.FrameType == KEYVALUEANDSTATUS)
	{

		data = UsartRcvFrm.stFrame.SendBuf[0];
		data |= UsartRcvFrm.stFrame.SendBuf[1] << 8;
		data |= UsartRcvFrm.stFrame.SendBuf[2] << 16;
		data |= UsartRcvFrm.stFrame.SendBuf[3] << 24;
		if(data == (0x00000008|0x00000004))
			return 1;
		else
			return 0;
	}
	else
		return 0;
}

/***********************************************************************************************
** ����������	�Ӽ��̴��ڽ��ն�����ȡ����
** �䡡  �룺	��
** �䡡  ����	��
************************************************************************************************/
u32 KEYUsartFetchData(stUARTKEY *UsartRcvFrm)
{
	if(USARTKEYRxWrtPtr != USARTKEYRxRdPtr) 
	{
		#if OS_CRITICAL_METHOD == 3                      /* Allocate storage for CPU status register           */
			OS_CPU_SR  cpu_sr = 0;
		#endif
		OS_ENTER_CRITICAL();//��ȫ���ж�
		memset((UsartRcvFrm), 0x00  , sizeof(stUARTKEY));
		memcpy((UsartRcvFrm),&USARTKEYRxBuf[USARTKEYRxRdPtr],sizeof(stUARTKEY) );	
		
		USARTKEYRxRdPtr++;
		USARTKEYRxRdPtr %= USARTKEY_RX_BUF_MAX;
		OS_EXIT_CRITICAL();//��ȫ���ж�
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}
/***********************************************************************************************
** ����������	���̴����жϷ�����
** �䡡  �룺	��
** �䡡  ����	��
************************************************************************************************/
void USART3_IRQHandler(void)
{	
	static uint32_t  z,j;
	static uint16_t  i;
	u8 Clear = Clear;
    u16 num = 0;
	u16 u16CrcTemp = 0x00;
	if(USART_GetITStatus(USART3, USART_IT_IDLE) != RESET)
	{  
		Clear = USART3->SR;  //��SR�Ĵ������                                                            
       
        Clear = USART3->DR;  //��DR�Ĵ������                                                        
        DMA_Cmd(DMA1_Stream1,DISABLE);   //�ر�DMA
		DMA_ClearFlag(DMA1_Stream1,DMA_FLAG_TCIF2);
        num = DMA_GetCurrDataCounter(DMA1_Stream1); //�õ������������ݸ���
        DMA_SetCurrDataCounter(DMA1_Stream1,USARTKEY_TMP_BUF_MAX);//����DMA���䳤��                                                        
        DMA_Cmd(DMA1_Stream1,ENABLE);  //����DMA  
		Crc16UsartReceiveChick(KEYUsartReveiveDataBuf,KEYUsartReveiveDataBuf[4]+5,&u16CrcTemp);		
		//У����յ������Ƿ���ȷ
		if(KEYUsartReveiveDataBuf[KEYUsartReveiveDataBuf[4]+5] == (u8)((u16CrcTemp&0xFF00)>>8)  		//��ȷ
			&& KEYUsartReveiveDataBuf[KEYUsartReveiveDataBuf[4]+6] ==  (u8)(u16CrcTemp&0x00FF))
		{
			memset(&(USARTKEYRxBuf[USARTKEYRxWrtPtr]), 0x00  , sizeof(stUARTKEY));
			memcpy(&(USARTKEYRxBuf[USARTKEYRxWrtPtr]),KEYUsartReveiveDataBuf,KEYUsartReveiveDataBuf[4]+5 );
			if(KEYUsartReveiveDataBuf[3] == 1)
			{
				SendResponseData(USARTKEYRxBuf[USARTKEYRxWrtPtr].stFrame,1,KEYDEVIC);
			}
			USARTKEYRxWrtPtr ++;
			USARTKEYRxWrtPtr %= USARTKEY_RX_BUF_MAX;
			OSSemPost(s_stLEDRecvd);
		}
	}
	else if(USART_GetFlagStatus(USART3, USART_FLAG_ORE) != RESET)
	{
		USART_ReceiveData(USART3);
		USART_ClearFlag(USART3,USART_FLAG_ORE);
		TIM3->CNT = 0;
	}
	else if ((USART3->SR & USART_FLAG_TXE) != RESET)
	{
		i++;
		if(i == (sizeof(stUARTKEY)-1))
		{
			USART_SendData(USART3, *((u8*)&USARTKEYTxBuf[USARTKEYTxRdPtr]+i));
			USART_ITConfig(USART3, USART_IT_TXE, DISABLE);
			USART_ClearFlag(USART3, USART_FLAG_TC); 
			USART_ITConfig(USART3, USART_IT_TC, ENABLE);
		}
		else if(i < (sizeof(stUARTKEY)-1))
		{
			if(i == (USARTKEYTxBuf[USARTKEYTxRdPtr].stFrame.u8DLC + 5))
				i = sizeof(stUARTKEY) - 2;
			USART_SendData(USART3, *((u8*)&USARTKEYTxBuf[USARTKEYTxRdPtr]+i));
			USART_ClearFlag(USART3, USART_FLAG_TC); 
		}
	}
	if ((USART3->SR & USART_FLAG_TC) != 0)
	{		
		if (i >= (sizeof(stUARTKEY)-1))
		{
			USART_ClearFlag(USART3, USART_FLAG_TC); 
			USART_ITConfig(USART3, USART_IT_TC, DISABLE);	
			i = 0;
			OSSemPost(s_stKEYUsartsend);
		}
	}
}
#ifndef INIT_FLASH
/***********************************************************************************************
** ����������	TIM3�жϷ�����
** �䡡  �룺	��
** �䡡  ����	��
************************************************************************************************/
void TIM3_IRQHandler(void)   //TIM3�ж�
{   

	u16 u16CrcTemp = 0x00;
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) //���ָ����TIM�жϷ������:TIM �ж�Դ 
	{
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update );
		GUI_KeyUser(0);
		TIM_Cmd(TIM3, DISABLE); 
		
//		TIM_ClearITPendingBit(TIM3, TIM_IT_Update );  //���TIMx���жϴ�����λ:TIM �ж�Դ 
//		TIM_Cmd(TIM3, DISABLE); 

//		/*���մ����߼�*/					
//		Crc16UsartReceiveChick(KEYUsartReveiveDataBuf,KEYUsartReveiveDataBuf[4]+5,&u16CrcTemp);		
//		//У����յ������Ƿ���ȷ
//		if(KEYUsartReveiveDataBuf[KEYUsartReveiveDataBuf[4]+5] == (u8)((u16CrcTemp&0xFF00)>>8)  		//��ȷ
//		   && KEYUsartReveiveDataBuf[KEYUsartReveiveDataBuf[4]+6] ==  (u8)(u16CrcTemp&0x00FF))
//		{
//			memset(&(USARTKEYRxBuf[USARTKEYRxWrtPtr]), 0x00  , sizeof(stUARTKEY));
//			memcpy(&(USARTKEYRxBuf[USARTKEYRxWrtPtr]),KEYUsartReveiveDataBuf,KEYUsartReveiveDataBuf[4]+5 );
//			if(KEYUsartReveiveDataBuf[3] == 1)
//			{
//				SendResponseData(USARTKEYRxBuf[USARTKEYRxWrtPtr].stFrame,1,KEYDEVIC);
//			}
//			USARTKEYRxWrtPtr ++;
//			USARTKEYRxWrtPtr %= USARTKEY_RX_BUF_MAX;
//			OSSemPost(s_stLEDRecvd);
//		}
//		else
//		{
//			;
//		}
//		memset(&(KEYUsartReveiveDataBuf[0]), 0x00  , USARTKEY_TMP_BUF_MAX);
//		KEYtmpTxWrtPtr = 0;
	}
}

#endif

