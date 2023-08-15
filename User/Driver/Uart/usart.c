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
#include "usart.h"
#include "usartapp.h"
/********************************************************************************
* #define宏定义
*********************************************************************************/
OS_EVENT *s_stKEYUsartRecvd ;
OS_EVENT *s_stKEYUsartsend ;
OS_EVENT *s_stLEDRecvd ;
stTIMER  KEYTimer;


//键盘
extern stUARTKEY  USARTKEYTxBuf[USARTKEY_TX_BUF_MAX];						//发送缓存
extern uint8_t	 USARTKEYTxWrtPtr;			   						    //发送写指针
extern uint8_t	 USARTKEYTxRdPtr;			   							//发送读指针



//键盘
#define  USARTKEY_TMP_BUF_MAX            140    //键盘临时接收数组最大值
#define  USARTKEY_RX_BUF_MAX              10    //键盘接收队列最大值
uint8_t  KEYUsartReveiveDataBuf[USARTKEY_TMP_BUF_MAX];   //临时接收数组
uint8_t  KEYtmpTxWrtPtr  = 0;  //临时变量写指针
stUARTKEY  USARTKEYRxBuf[USARTKEY_RX_BUF_MAX];						//接收缓存
uint8_t	 USARTKEYRxWrtPtr = 0;			   						    //接收写指针
uint8_t	 USARTKEYRxRdPtr =  0;			   							//接收读指针


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
** 功能描述：串口初始化
** 输　入：  无
** 输　出：  无
*********************************************************************************/
void UartKeyInit(void)
{
	   //GPIO端口设置
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	DMA_InitTypeDef  DMA_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE); //使能GPIOA时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);//使能USART3时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);
	//串口1对应引脚复用映射
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource10,GPIO_AF_USART3); //GPIOA10复用为USART1
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource11,GPIO_AF_USART3); //GPIOA11复用为USART1
	
	DMA_DeInit(DMA1_Stream1); //DMA1 数据流 3
	DMA_InitStructure.DMA_Channel = DMA_Channel_4;  
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&USART3->DR;  //DMAíaéèADC?ùμ??·
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
	USART_DMACmd(USART3,USART_DMAReq_Rx,ENABLE);   //ê1?ü′??ú1 DMA?óê?
	DMA_Cmd(DMA1_Stream1, ENABLE);  //?yê??y?ˉDMA′?ê?
	
	//USART1端口配置
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11; //GPIOA11与GPIOA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init(GPIOB,&GPIO_InitStructure); //初始化PA11，PA10
	

   //USART1 初始化设置
	USART_InitStructure.USART_BaudRate = 115200;//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
	USART_Init(USART3, &USART_InitStructure); //初始化串口1
	

	//Usart1 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;//串口3中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x02;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器

	
	USART_ClearFlag(USART3, USART_FLAG_TC);
	USART_ClearFlag(USART3, USART_FLAG_RXNE);
	USART_ClearFlag(USART3, USART_FLAG_IDLE);
	USART_ClearFlag(USART3, USART_FLAG_ORE);
	USART_ITConfig(USART3, USART_IT_IDLE, ENABLE);//开启相关中断
	USART_Cmd(USART3, ENABLE);  //使能串口1 
}
void MYDMA_Enable(DMA_Stream_TypeDef*DMA_CHx)
{ 
	DMA_Cmd(DMA_CHx, DISABLE );  //1?±?USART1 TX DMA1 ?ù??ê?μ?í¨μà      
 	DMA_SetCurrDataCounter(DMA_CHx,USARTKEY_TMP_BUF_MAX);//DMAí¨μàμ?DMA?o′?μ?′óD?
 	DMA_Cmd(DMA_CHx, ENABLE);  //ê1?üUSART1 TX DMA1 ?ù??ê?μ?í¨μà 
}
/********************************************************************************
** 功能描述：串口初始化
** 输　入：  无
** 输　出：  无
*********************************************************************************/
void UartInit(void)
{
	UartKeyInit();
}

/********************************************************************************
** 功能描述：创建一个信号量
** 输　入：  无
** 输　出：  无
*********************************************************************************/
u32 UsartRecvSemCreate(void)
{
	s_stKEYUsartRecvd  = OSSemCreate(0);	//创建接收数据信号量 
	s_stLEDRecvd  = OSSemCreate(0);	//创建接收数据信号量
	s_stKEYUsartsend = OSSemCreate(0);
	return 0x01;
}
/********************************************************************************
** 功能描述：发送一个信号量
** 输　入：  无
** 输　出：  无
*********************************************************************************/
void UsartMessageForRfidRcved(void)
{
	OSSemPost(s_stKEYUsartRecvd);	//RFID接收到数据
}

/*******************************************************************************************   




**函数作用：计算CRC16校验值
**函数参数：u8Buf:需要校验的数据起始地址，u32Len:需要校验数据的长度，*u16CheckOld:计算后的CRC校验值
**函数输出：无
**注意事项：无
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
**函数作用：计算CRC16校验值
**函数参数：u8Buf:需要校验的数据起始地址，u32Len:需要校验数据的长度，*u16CheckOld:计算后的CRC校验值
**函数输出：无
**注意事项：无
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
** 功能描述：	从键盘串口接收队列中取数据
** 输　  入：	无
** 输　  出：	无
************************************************************************************************/
u32 KEYUsartFetchData(stUARTKEY *UsartRcvFrm)
{
	if(USARTKEYRxWrtPtr != USARTKEYRxRdPtr) 
	{
		#if OS_CRITICAL_METHOD == 3                      /* Allocate storage for CPU status register           */
			OS_CPU_SR  cpu_sr = 0;
		#endif
		OS_ENTER_CRITICAL();//关全局中断
		memset((UsartRcvFrm), 0x00  , sizeof(stUARTKEY));
		memcpy((UsartRcvFrm),&USARTKEYRxBuf[USARTKEYRxRdPtr],sizeof(stUARTKEY) );	
		
		USARTKEYRxRdPtr++;
		USARTKEYRxRdPtr %= USARTKEY_RX_BUF_MAX;
		OS_EXIT_CRITICAL();//开全局中断
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}
/***********************************************************************************************
** 功能描述：	键盘串口中断服务函数
** 输　  入：	无
** 输　  出：	无
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
		Clear = USART3->SR;  //读SR寄存器清除                                                            
       
        Clear = USART3->DR;  //读DR寄存器清除                                                        
        DMA_Cmd(DMA1_Stream1,DISABLE);   //关闭DMA
		DMA_ClearFlag(DMA1_Stream1,DMA_FLAG_TCIF2);
        num = DMA_GetCurrDataCounter(DMA1_Stream1); //得到真正接收数据个数
        DMA_SetCurrDataCounter(DMA1_Stream1,USARTKEY_TMP_BUF_MAX);//设置DMA传输长度                                                        
        DMA_Cmd(DMA1_Stream1,ENABLE);  //开启DMA  
		Crc16UsartReceiveChick(KEYUsartReveiveDataBuf,KEYUsartReveiveDataBuf[4]+5,&u16CrcTemp);		
		//校验接收的数据是否正确
		if(KEYUsartReveiveDataBuf[KEYUsartReveiveDataBuf[4]+5] == (u8)((u16CrcTemp&0xFF00)>>8)  		//正确
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
** 功能描述：	TIM3中断服务函数
** 输　  入：	无
** 输　  出：	无
************************************************************************************************/
void TIM3_IRQHandler(void)   //TIM3中断
{   

	u16 u16CrcTemp = 0x00;
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) //检查指定的TIM中断发生与否:TIM 中断源 
	{
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update );
		GUI_KeyUser(0);
		TIM_Cmd(TIM3, DISABLE); 
		
//		TIM_ClearITPendingBit(TIM3, TIM_IT_Update );  //清除TIMx的中断待处理位:TIM 中断源 
//		TIM_Cmd(TIM3, DISABLE); 

//		/*接收处理逻辑*/					
//		Crc16UsartReceiveChick(KEYUsartReveiveDataBuf,KEYUsartReveiveDataBuf[4]+5,&u16CrcTemp);		
//		//校验接收的数据是否正确
//		if(KEYUsartReveiveDataBuf[KEYUsartReveiveDataBuf[4]+5] == (u8)((u16CrcTemp&0xFF00)>>8)  		//正确
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

