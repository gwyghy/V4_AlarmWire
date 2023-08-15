/***********************************************************
* ģ������jtbs.c
* ���ߣ�  HNDZ
*
*
************************************************************/
#include "bsp_jtbs.h"
#include "bsp_leds.h"

/********************************************************************************
* ��������
*********************************************************************************/
static u32 u32EmergencyStopEnsureCnt = 5;		//��ͣ״̬ȷ���˲�����
static u32 u32LockOnEnsureCnt = 5;				//����״̬ȷ���˲�����
/***********************************************************
* �������ƣ�JTBS_Init
* ����������
* ��ڲ�����
* ���ڲ�������
* ʹ��˵������
************************************************************/
void JTBS_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(STOP_GPIO_RCC | LOCK_GPIO_RCC , ENABLE);

	/* Configure STOP_PIN in input nopull mode */
	GPIO_InitStructure.GPIO_Pin = STOP_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	//GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		// for Output
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(STOP_PORT, &GPIO_InitStructure);

	/* Configure LOCK_PIN in input nopull mode */
	GPIO_InitStructure.GPIO_Pin = LOCK_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	//GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		// for Output
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(LOCK_PORT, &GPIO_InitStructure);

	StateLed(0x00, LED_STOP);
}
/***********************************************************
* �������ƣ�StopLockInit
* ����������
* ��ڲ�����
* ���ڲ�������
* ʹ��˵������
************************************************************/
void StopLockInit(void)
{
	JTBS_Init();
}

static u16 u16Lock = FALSE;
static u16 u16Stop = FALSE;
/***********************************************************
* �������ƣ�CheckBS
* ����������������
* ��ڲ�����
* ���ڲ�����TRUE����������
* ʹ��˵������
************************************************************/
u16 CheckBS(void)
{

	static u16 u16LockEnabledCount = 0;
	static u16 u16LockDisabledCount = 0;

	if (GPIO_ReadInputDataBit(LOCK_PORT, LOCK_PIN) == 1)			//�ߵ�ƽΪ����״̬
	{
		u16LockDisabledCount = 0;
		if (u16LockEnabledCount < u32LockOnEnsureCnt)
			u16LockEnabledCount++;
	}
	else
	{
		u16LockEnabledCount = 0;
		if (u16LockDisabledCount < u32LockOnEnsureCnt)
			u16LockDisabledCount++;
	}
	if ((u16LockEnabledCount == u32LockOnEnsureCnt) && (u16Lock == FALSE))
	{
		u16Lock = TRUE;
		StateLed(LED_STOP, 0x00);
	}
	else if((u16LockDisabledCount == u32LockOnEnsureCnt) && (u16Lock == TRUE))
	{
		u16Lock = FALSE;
	}
	else if((u16LockDisabledCount == u32LockOnEnsureCnt) && (u16Lock == FALSE) && (u16Stop == FALSE))
	{
		StateLed(0x00, LED_STOP);
	}

	return(u16Lock);
}
/***********************************************************
* �������ƣ�CheckJT
* ������������⼱ͣ
* ��ڲ�����
* ���ڲ�����TRUE����ͣ����
* ʹ��˵������
************************************************************/
u16 CheckJT(void)
{

	static u16 u16StopEnabledCount = 0;
	static u16 u16StopDisabledCount = 0;

	if (GPIO_ReadInputDataBit(STOP_PORT, STOP_PIN) == 0)			//�͵�ƽΪ��ͣ���״̬
	{
		u16StopEnabledCount = 0;
		if (u16StopDisabledCount < u32EmergencyStopEnsureCnt)
			u16StopDisabledCount++;
	}
	else															//�ߵ�ƽΪ��ͣ״̬
	{
		u16StopDisabledCount = 0;
		if (u16StopEnabledCount < u32EmergencyStopEnsureCnt)
			u16StopEnabledCount++;
	}

	if ((u16StopEnabledCount == u32EmergencyStopEnsureCnt) && (u16Stop == FALSE))
	{
		u16Stop = TRUE;
		StateLed(LED_STOP, 0x00);
	}
	else if((u16StopDisabledCount == u32EmergencyStopEnsureCnt) && (u16Stop == TRUE))
	{
		u16Stop = FALSE;
	}
	else if((u16StopDisabledCount == u32EmergencyStopEnsureCnt) && (u16Lock == FALSE) && (u16Stop == FALSE))
	{
		StateLed(0x00, LED_STOP);
	}

	return(u16Stop);
}
