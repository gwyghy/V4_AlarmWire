/*
*********************************************************************************************************
*	                                  
*	模块名称 : 看门狗程序
*	文件名称 : bsp_iwdg.c
*	版    本 : V1.0
*	说    明 : IWDG例程。
*	修改记录 :
*		版本号  日期         作者    说明
*		v1.0    2012-12-17  Eric2013  ST固件库V1.0.2版本。
*	
*	Copyright (C), 2013-2014
*   QQ超级群：216681322
*   BLOG: http://blog.sina.com.cn/u/2565749395
*********************************************************************************************************
*/
#include "stm32f4xx.h"
#include "bsp_iwdg.h"
#include "stdio.h"


__IO uint32_t LsiFreq = 0;
__IO uint32_t CaptureNumber = 0, PeriodValue = 0;
uint32_t GetLSIFrequency(void);
/*
*********************************************************************************************************
*	函 数 名: bsp_InitIwdg
*	功能说明: 独立看门狗时间配置函数
*	形    参：IWDGTime: 0 ---- 0x0FFF
*			  独立看门狗时间设置,单位为ms,IWDGTime = 1000 大约就是一秒的时间
*		  	  超过IWDGTime不进行喂狗的话系统将会复位
*			  LSI = 32800左右
*	返 回 值: 无		        
*********************************************************************************************************
*/
void bsp_InitIwdg(uint32_t _ulIWDGTime)
{
		
	/* Check if the system has resumed from IWDG reset */
	if (RCC_GetFlagStatus(RCC_FLAG_IWDGRST) != RESET)
	{		
		/* Clear reset flags */
		RCC_ClearFlag();
	}
	else
	{
		/* IWDGRST flag is not set */

	}
	
	/* Get the LSI frequency:  TIM5 is used to measure the LSI frequency */
	/* 特别注意，这里没有精确计算，默认取32K*/
#if 0
	LsiFreq = GetLSIFrequency();
#else
	/* Enable the LSI oscillator ************************************************/
	RCC_LSICmd(ENABLE);
	
	/* Wait till LSI is ready */
	while (RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET)
	{}
	LsiFreq = 32000;
#endif
	/* 
	   IWDG timeout equal to _ulIWDGTime (the timeout may varies due to LSI frequency
	   dispersion) 
	   Enable write access to IWDG_PR and IWDG_RLR registers 
	   写入0x5555表示允许访问IWDG_PR 和IWDG_RLR寄存器 
	 */
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
	
	/* IWDG counter clock: LSI/32 */

	IWDG_SetPrescaler(IWDG_Prescaler_32);
	
	/*特别注意，由于这里_ulIWDGTime的最小单位是ms, 所以这里重装计数的
	  计数时 需要除以1000
	 Set counter reload value to obtain _ulIWDGTime IWDG TimeOut.
	 Counter Reload Value = (_ulIWDGTime / 1000) /(1 / IWDG counter clock period)
	                      = (_ulIWDGTime / 1000) / (32/LSI)
	                      = (_ulIWDGTime / 1000) / (32/LsiFreq)
	                      = LsiFreq * _ulIWDGTime / 32000
	 实际测试LsiFreq = 32800，所以这里取1的时候 大概就是1ms 
	*/
	IWDG_SetReload(_ulIWDGTime);
	
	/* Reload IWDG counter */
	IWDG_ReloadCounter();
	
	/* Enable IWDG (the LSI oscillator will be enabled by hardware) */
	IWDG_Enable();
		
}

/*
*********************************************************************************************************
*	函 数 名: IWDG_Feed
*	功能说明: 喂狗函数
*	形    参：无
*	返 回 值: 无		        
*********************************************************************************************************
*/
void IWDG_Feed(void)
{
	IWDG_ReloadCounter();
}

/*
*********************************************************************************************************
*	函 数 名: GetLSIFrequency
*	功能说明: Configures TIM5 to measure the LSI oscillator frequency
*	形    参：无
*	返 回 值: LSI Frequency	 实际测试LSI = 32800左右
*   注    意：这里的注释使用的官方的注释，不再翻译成中文，这里的英文注释能准确的表达意思	        
*********************************************************************************************************
*/
uint32_t GetLSIFrequency(void)
{
	NVIC_InitTypeDef   NVIC_InitStructure;
	TIM_ICInitTypeDef  TIM_ICInitStructure;
	RCC_ClocksTypeDef  RCC_ClockFreq;
	
	/* Enable the LSI oscillator ************************************************/
	RCC_LSICmd(ENABLE);
	
	/* Wait till LSI is ready */
	while (RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET)
	{}
	
	/* TIM5 configuration *******************************************************/ 
	/* Enable TIM5 clock */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
	
	/* Connect internally the TIM5_CH4 Input Capture to the LSI clock output */
	TIM_RemapConfig(TIM5, TIM5_LSI);
	
	/* Configure TIM5 presclaer */
	TIM_PrescalerConfig(TIM5, 0, TIM_PSCReloadMode_Immediate);
	
	/* TIM5 configuration: Input Capture mode ---------------------
	 The LSI oscillator is connected to TIM5 CH4
	 The Rising edge is used as active edge,
	 The TIM5 CCR4 is used to compute the frequency value 
	------------------------------------------------------------ */
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_4;
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV8;
	TIM_ICInitStructure.TIM_ICFilter = 0;
	TIM_ICInit(TIM5, &TIM_ICInitStructure);
	
	/* Enable TIM5 Interrupt channel */
	NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	/* Enable TIM5 counter */
	TIM_Cmd(TIM5, ENABLE);
	
	/* Reset the flags */
	TIM5->SR = 0;
	
	/* Enable the CC4 Interrupt Request */  
	TIM_ITConfig(TIM5, TIM_IT_CC4, ENABLE);
	
	
	/* Wait until the TIM5 get 2 LSI edges (refer to TIM5_IRQHandler() in 
	stm32f4xx_it.c file) ******************************************************/
	while(CaptureNumber != 2)
	{
	}
	/* Deinitialize the TIM5 peripheral registers to their default reset values */
	TIM_DeInit(TIM5);
		
	/* Compute the LSI frequency, depending on TIM5 input clock frequency (PCLK1)*/
	/* Get SYSCLK, HCLK and PCLKx frequency */
	RCC_GetClocksFreq(&RCC_ClockFreq);
	
	/* Get PCLK1 prescaler */
	if ((RCC->CFGR & RCC_CFGR_PPRE1) == 0)
	{ 
		/* PCLK1 prescaler equal to 1 => TIMCLK = PCLK1 */
		return ((RCC_ClockFreq.PCLK1_Frequency / PeriodValue) * 8);
	}
	else
	{ 
		/* PCLK1 prescaler different from 1 => TIMCLK = 2 * PCLK1 */
		return (((2 * RCC_ClockFreq.PCLK1_Frequency) / PeriodValue) * 8) ;
	}
}




