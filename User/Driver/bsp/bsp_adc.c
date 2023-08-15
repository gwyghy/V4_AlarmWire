/***********************************************************
* ģ������bsp_adc.c
* ���ߣ�  
*
*
************************************************************/
#include	"bsp.h"

/*
*********************************************************************************************************
*	�� �� ��: ADC_Configuration
*	����˵��: ����ADC, PC4��ΪADCͨ������
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void ADC_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	//__IO uint16_t ADCConvertedValue;

    /* ʹ�� ADC1 and GPIOC clock */
	RCC_AHB1PeriphClockCmd(ADC3_V_GPIO_RCC|ADC1_A_GPIO_RCC, ENABLE);
	RCC_APB2PeriphClockCmd(ADC3_V_RCC|ADC1_A_RCC, ENABLE);
	
	/* ����PC2Ϊģ������(ADC Channel12) */
	GPIO_InitStructure.GPIO_Pin = ADC3_V_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(ADC3_V_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = ADC1_A_PIN;
	GPIO_Init(ADC1_A_PORT, &GPIO_InitStructure);

  /****************************************************************************   
	  PCLK2 = HCLK / 2 
	  ����ѡ�����2��Ƶ
	  ADCCLK = PCLK2 /8 = HCLK / 8 = 168 / 8 = 21M
      ADC����Ƶ�ʣ� Sampling Time + Conversion Time = 480 + 12 cycles = 492cyc
                    Conversion Time = 21MHz / 492cyc = 42.6ksps. 
	*****************************************************************************/
	RCC_APB2PeriphResetCmd(ADC3_V_RCC|ADC1_A_RCC,ENABLE);	    //ADC1��λ
	RCC_APB2PeriphResetCmd(ADC3_V_RCC|ADC1_A_RCC,DISABLE);	//��λ����	 
	
	/* ADC Common ���� ----------------------------------------------------------*/
	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_20Cycles;
	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;		//����DMA
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div8;
	ADC_CommonInit(&ADC_CommonInitStructure);
	
	/* ADC1 regular channel 12 configuration ************************************/
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfConversion = 1;	/* 1��ת���ڹ��������� Ҳ����ֻת����������1  */
	ADC_Init(ADC3_V, &ADC_InitStructure);
	
	ADC_Init(ADC1_A, &ADC_InitStructure);

	/* ����ADC1 ����ͨ��12 channel12 configuration */
	ADC_RegularChannelConfig(ADC3_V, ADC3_V_CHANNEL, 1, ADC_SampleTime_56Cycles);

	ADC_RegularChannelConfig(ADC1_A, ADC1_A_CHANNEL, 1, ADC_SampleTime_56Cycles);
	/* ʹ�� ADC1 */
	ADC_Cmd(ADC3_V, ENABLE);
	ADC_Cmd(ADC1_A, ENABLE);

	/* �������ADCת�� */
//	ADC_SoftwareStartConv(ADC1);
}


uint16_t adc_vol_buf[SAMP_COUNT] = {0};
uint8_t adc_vol_buf_pt = 0;
uint16_t adc_cur_buf[SAMP_COUNT] = {0};
uint8_t adc_cur_buf_pt = 0;
/*
*********************************************************************************************************
*	�� �� ��: AdcPro
*	����˵��: ADC������������1ms systick �жϽ��е���
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
uint16_t  AdcPro(uint8_t adcNum)
{
	uint16_t maxValue = 0, minValue = 0;
	uint32_t sum = 0;
	uint8_t i;
	uint16_t adcValue = 0;
	
	switch(adcNum)
	{
		case adc_vol:
			ADC_SoftwareStartConv(ADC3_V);		//ʹ��ָ����ADC1�����ת����������	
			if(adc_vol_buf_pt >= SAMP_COUNT)
				adc_vol_buf_pt = 0;
			adc_vol_buf[adc_vol_buf_pt] =  ADC_GetConversionValue(ADC3_V);
			adc_vol_buf_pt++;	
			sum = 0;
			maxValue = adc_vol_buf[0];
			minValue = adc_vol_buf[0];
			for (i = 0; i < SAMP_COUNT; i++)
			{
				sum += adc_vol_buf[i];
				if(maxValue<adc_vol_buf[i])
					maxValue = adc_vol_buf[i];
				if(minValue>adc_vol_buf[i])
					minValue = adc_vol_buf[i];
			}
			break;
		case adc_cur:
			ADC_SoftwareStartConv(ADC1_A);		//ʹ��ָ����ADC1�����ת����������	
			if(adc_cur_buf_pt >= SAMP_COUNT)
				adc_cur_buf_pt = 0;
			adc_cur_buf[adc_cur_buf_pt] =  ADC_GetConversionValue(ADC1_A);
			adc_cur_buf_pt++;
			sum = 0;
			maxValue = adc_cur_buf[0];
			minValue = adc_cur_buf[0];
			for (i = 0; i < SAMP_COUNT; i++)
			{
				sum += adc_cur_buf[i];
				if(maxValue<adc_cur_buf[i])
					maxValue = adc_cur_buf[i];
				if(minValue>adc_cur_buf[i])
					minValue = adc_cur_buf[i];
			}
			break;
	}
	
	adcValue = (sum-maxValue-minValue) / (SAMP_COUNT-2);	/* ADC����ֵ�����ɴβ���ֵƽ�� */
	return adcValue;
}


