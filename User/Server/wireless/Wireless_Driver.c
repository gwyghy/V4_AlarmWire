/********************************************************************************
* �ļ�����	usart.c
* ���ߣ�	     
* �汾��   	V1.0
* ���ڣ�    2015.04.24
* ��������:  ʵ��UART��������ʵ�ֺ������ݽ��յĵײ�������

* �޸�˵����   
*
*       >>>>  �ڹ����е�λ��  <<<<
*             3-Ӧ�ò�
*             2-Э���
*         ��   1-Ӳ��������
*********************************************************************************
* @copy
* <h2><center>&copy; COPYRIGHT ������������޹�˾ �з����� �����</center></h2>
*********************************************************************************/
/********************************************************************************
* .hͷ�ļ�
*********************************************************************************/
#include "includes.h"
#include "Wireless_Driver.h"
#include "Wireless_Bus.h"

#include "sys_msg_def.h"

/********************************************************************************
* #define�궨��
*********************************************************************************/
/********************************************************************************
* ��������
*********************************************************************************/
#define  USART_TMP_BUF_MAX            160    //��ʱ�����������ֵ
uint8_t  tmpTxWrtPtr  = 0;  //��ʱ����дָ��
uint8_t  UsartReveiveDataBuf[USART_TMP_BUF_MAX];   //��ʱ��������


/***********************************************************************************************
** ����������	TIM��ʼ������
** �䡡  �룺	��
** �䡡  ����	��
************************************************************************************************/
void TIM4_Int_Init(u32 arr,u32 psc)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); //ʱ��ʹ��

	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	 ������5000Ϊ500ms
	TIM_TimeBaseStructure.TIM_Prescaler = psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  10Khz�ļ���Ƶ��  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
	TIM_ClearFlag(TIM4,TIM_IT_Update);
	TIM_ClearITPendingBit(TIM4,TIM_IT_Update);
	
	TIM_ITConfig(  //ʹ�ܻ���ʧ��ָ����TIM�ж�
		TIM4, //TIM2
		TIM_IT_Update ,
		ENABLE  //ʹ��
		);
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;  //TIM3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���

	TIM_Cmd(TIM4, DISABLE);  //ʹ��TIMx����
}

/**********************************************************************************************************
*	�� �� ��: WlUsartInit
*	����˵��: WlUsart�豸��ʼ��
*	��    ��: ��
*	�� �� ֵ: ��
**********************************************************************************************************/
void DriverUsart_Init(uint32_t bund)
{
	GPIO_InitTypeDef GPIO_InitStructure;   
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef  NVIC_InitStructure;
		
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	GPIO_PinAFConfig(GPIO_PORT_WIRELESS_TX, GPIO_PinSource9, GPIO_AF_USART1);
	GPIO_PinAFConfig(GPIO_PORT_WIRELESS_RX, GPIO_PinSource10, GPIO_AF_USART1);
	
 	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Pin   = GPIO_PIN_WIRELESS_TX;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIO_PORT_WIRELESS_TX, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Pin   = GPIO_PIN_WIRELESS_RX;
	GPIO_Init(GPIO_PORT_WIRELESS_RX, &GPIO_InitStructure);
	
	
	USART_InitStructure.USART_BaudRate            = 115200;
	USART_InitStructure.USART_WordLength          = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits            = USART_StopBits_1;
	USART_InitStructure.USART_Parity              = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1, &USART_InitStructure);

	//Usart1 NVIC ����
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;//����3�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x00;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���

	if(SysParam.globalParam.SupportParam.Paring_Method == 0) //�����뷽ʽѡ��Ϊ����ʱ
	{
		USART_Cmd(USART1, ENABLE);						//����ʹ��
		USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);	//�ж�ʹ��
	}
	TIM4_Int_Init(199,1049);
}





/**********************************************************************************************************
*	�� �� ��: WL_USART_IRQHandler
*	����˵��: �жϴ���
*	��    ��: ��
*	�� �� ֵ: ��
**********************************************************************************************************/
void USART1_IRQHandler(void)
{
	#if OS_CRITICAL_METHOD == 3                      /* Allocate storage for CPU status register           */
			OS_CPU_SR  cpu_sr = 0;
	#endif

	OS_ENTER_CRITICAL();                         /* Tell uC/OS-II that we are starting an ISR          */
	OSIntEnter();
	OS_EXIT_CRITICAL();
	//usart����
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
		UsartReveiveDataBuf[tmpTxWrtPtr] = (u8)USART1->DR;
		//�������ݵ�һ����TIM4
		if(tmpTxWrtPtr == 0)
		{
			TIM_Cmd(TIM4, ENABLE);  //ʹ��TIM4
		}
		//���յ��������������������
		else
		{
			TIM4->CNT = 0;//����TIM4��ʱ
		}
		tmpTxWrtPtr ++;
		tmpTxWrtPtr %= USART_TMP_BUF_MAX;
	}
	//usart�����쳣
	else if(USART_GetFlagStatus(USART1,USART_FLAG_ORE) != RESET)
	{
		USART_ClearFlag(USART1,USART_FLAG_ORE);
		USART1->DR;
		TIM4->CNT = 0;
	}
	else
	{
		UsartDriverTxDealFun();
	}
	OSIntExit();
}


void TIM4_IRQHandler(void) 
{   
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)
	{
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update ); 
		TIM_Cmd(TIM4, DISABLE);
	

		UsartRecvDataProc(UsartReveiveDataBuf,tmpTxWrtPtr);
						
		memset(&(UsartReveiveDataBuf[0]), 0x00  , USART_TMP_BUF_MAX);
		tmpTxWrtPtr = 0;		
	}
}

