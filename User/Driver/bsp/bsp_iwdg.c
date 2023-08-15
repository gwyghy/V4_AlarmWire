/*
*********************************************************************************************************
*	                                  
*	ģ������ : ���Ź�����
*	�ļ����� : bsp_iwdg.c
*	��    �� : V1.0
*	˵    �� : IWDG���̡�
*	�޸ļ�¼ :
*		�汾��  ����         ����    ˵��
*		v1.0    2012-12-17  Eric2013  ST�̼���V1.0.2�汾��
*	
*	Copyright (C), 2013-2014
*   QQ����Ⱥ��216681322
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
*	�� �� ��: bsp_InitIwdg
*	����˵��: �������Ź�ʱ�����ú���
*	��    �Σ�IWDGTime: 0 ---- 0x0FFF
*			  �������Ź�ʱ������,��λΪms,IWDGTime = 1000 ��Լ����һ���ʱ��
*		  	  ����IWDGTime������ι���Ļ�ϵͳ���Ḵλ
*			  LSI = 32800����
*	�� �� ֵ: ��		        
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
	/* �ر�ע�⣬����û�о�ȷ���㣬Ĭ��ȡ32K*/
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
	   д��0x5555��ʾ�������IWDG_PR ��IWDG_RLR�Ĵ��� 
	 */
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
	
	/* IWDG counter clock: LSI/32 */

	IWDG_SetPrescaler(IWDG_Prescaler_32);
	
	/*�ر�ע�⣬��������_ulIWDGTime����С��λ��ms, ����������װ������
	  ����ʱ ��Ҫ����1000
	 Set counter reload value to obtain _ulIWDGTime IWDG TimeOut.
	 Counter Reload Value = (_ulIWDGTime / 1000) /(1 / IWDG counter clock period)
	                      = (_ulIWDGTime / 1000) / (32/LSI)
	                      = (_ulIWDGTime / 1000) / (32/LsiFreq)
	                      = LsiFreq * _ulIWDGTime / 32000
	 ʵ�ʲ���LsiFreq = 32800����������ȡ1��ʱ�� ��ž���1ms 
	*/
	IWDG_SetReload(_ulIWDGTime);
	
	/* Reload IWDG counter */
	IWDG_ReloadCounter();
	
	/* Enable IWDG (the LSI oscillator will be enabled by hardware) */
	IWDG_Enable();
		
}

/*
*********************************************************************************************************
*	�� �� ��: IWDG_Feed
*	����˵��: ι������
*	��    �Σ���
*	�� �� ֵ: ��		        
*********************************************************************************************************
*/
void IWDG_Feed(void)
{
	IWDG_ReloadCounter();
}

/*
*********************************************************************************************************
*	�� �� ��: GetLSIFrequency
*	����˵��: Configures TIM5 to measure the LSI oscillator frequency
*	��    �Σ���
*	�� �� ֵ: LSI Frequency	 ʵ�ʲ���LSI = 32800����
*   ע    �⣺�����ע��ʹ�õĹٷ���ע�ͣ����ٷ�������ģ������Ӣ��ע����׼ȷ�ı����˼	        
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




