/********************************************************************************
* 文件名：	usart.c
* 作者：	     
* 版本：   	V1.0
* 日期：    2015.04.24
* 功能描述:  实现UART的驱动，实现红外数据接收的底层驱动。

* 修改说明：   
*
*       >>>>  在工程中的位置  <<<<
*             3-应用层
*             2-协议层
*         √   1-硬件驱动层
*********************************************************************************
* @copy
* <h2><center>&copy; COPYRIGHT 天津华宁电子有限公司 研发中心 软件部</center></h2>
*********************************************************************************/
/********************************************************************************
* .h头文件
*********************************************************************************/
#include "includes.h"
#include "Wireless_Driver.h"
#include "Wireless_Bus.h"

#include "sys_msg_def.h"

/********************************************************************************
* #define宏定义
*********************************************************************************/
/********************************************************************************
* 常量定义
*********************************************************************************/
#define  USART_TMP_BUF_MAX            160    //临时接收数组最大值
uint8_t  tmpTxWrtPtr  = 0;  //临时变量写指针
uint8_t  UsartReveiveDataBuf[USART_TMP_BUF_MAX];   //临时接收数组


/***********************************************************************************************
** 功能描述：	TIM初始化函数
** 输　  入：	无
** 输　  出：	无
************************************************************************************************/
void TIM4_Int_Init(u32 arr,u32 psc)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); //时钟使能

	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 计数到5000为500ms
	TIM_TimeBaseStructure.TIM_Prescaler = psc; //设置用来作为TIMx时钟频率除数的预分频值  10Khz的计数频率  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
 
	TIM_ClearFlag(TIM4,TIM_IT_Update);
	TIM_ClearITPendingBit(TIM4,TIM_IT_Update);
	
	TIM_ITConfig(  //使能或者失能指定的TIM中断
		TIM4, //TIM2
		TIM_IT_Update ,
		ENABLE  //使能
		);
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;  //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器

	TIM_Cmd(TIM4, DISABLE);  //使能TIMx外设
}

/**********************************************************************************************************
*	函 数 名: WlUsartInit
*	功能说明: WlUsart设备初始化
*	形    参: 无
*	返 回 值: 无
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

	//Usart1 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;//串口3中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x00;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器

	if(SysParam.globalParam.SupportParam.Paring_Method == 0) //当对码方式选择为串口时
	{
		USART_Cmd(USART1, ENABLE);						//串口使能
		USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);	//中断使能
	}
	TIM4_Int_Init(199,1049);
}





/**********************************************************************************************************
*	函 数 名: WL_USART_IRQHandler
*	功能说明: 中断处理
*	形    参: 无
*	返 回 值: 无
**********************************************************************************************************/
void USART1_IRQHandler(void)
{
	#if OS_CRITICAL_METHOD == 3                      /* Allocate storage for CPU status register           */
			OS_CPU_SR  cpu_sr = 0;
	#endif

	OS_ENTER_CRITICAL();                         /* Tell uC/OS-II that we are starting an ISR          */
	OSIntEnter();
	OS_EXIT_CRITICAL();
	//usart接收
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
		UsartReveiveDataBuf[tmpTxWrtPtr] = (u8)USART1->DR;
		//接收数据第一个打开TIM4
		if(tmpTxWrtPtr == 0)
		{
			TIM_Cmd(TIM4, ENABLE);  //使能TIM4
		}
		//接收到的其他数据清零计数器
		else
		{
			TIM4->CNT = 0;//清零TIM4计时
		}
		tmpTxWrtPtr ++;
		tmpTxWrtPtr %= USART_TMP_BUF_MAX;
	}
	//usart接收异常
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

