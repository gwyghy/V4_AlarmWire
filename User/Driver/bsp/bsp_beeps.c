#include	"bsp_beeps.h"


#define BEEP_ON		ToBeep()
#define BEEP_OFF	StopBeep()

static vu16 vu16BeepCase = 0;				//�澯���� =0������
static vu16 vu16BeepState = 0;				//�澯�׶Σ�=0���澯�ڣ�=1����Ъ
static vu16 vu16BeepOffTimer = 0xffff;		//��Ъ��ʱ��x*10ms��
static vu16 vu16BeepOnTimer = 0xffff;		//�澯��ʱ��x*10ms��
static vu16 vu16BeepCnt = 0;				//�澯����

void Beep_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(BEEP_GPIO_RCC, ENABLE);

	/* Configure BEEP_PIN in output pushpull mode */
	GPIO_InitStructure.GPIO_Pin = BEEP_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;

	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;

	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;

	GPIO_Init(BEEP_PORT, &GPIO_InitStructure);
	
	GPIO_SetBits(BEEP_PORT, BEEP_PIN);
	
	GPIO_ResetBits(BEEP_PORT, BEEP_PIN);

}
//����
void ToBeep(void)
{
	#if OS_CRITICAL_METHOD == 3                      /* Allocate storage for CPU status register           */
	    OS_CPU_SR  cpu_sr = 0;
	#endif
	
	OS_ENTER_CRITICAL();
	GPIO_SetBits(BEEP_PORT, BEEP_PIN);
 	OS_EXIT_CRITICAL();
}
//ֹͣ����
void StopBeep(void)
{
	#if OS_CRITICAL_METHOD == 3                      /* Allocate storage for CPU status register           */
	    OS_CPU_SR  cpu_sr = 0;
	#endif
	
	OS_ENTER_CRITICAL();
	GPIO_ResetBits(BEEP_PORT, BEEP_PIN);
 	OS_EXIT_CRITICAL();
}

//���ݲ�ͬ��������Ĳ�ͬ�׶Σ�������Ӧ�ķ��������ʱ
static u16 GetBeepOffTimer(u16 u16Case, u16 u16Order,u8 val)
{
	switch (u16Case)
	{
		case BEEP_KEY_PRESSED:
			return (BEEP_KEY_OFF_TIMER);

		case BEEP_NEAR_SELECTED:
			if(val == 1)
				return (BEEP_NEAR_OFF_TIMER);
			else
				return (BEEP_NEAR_OFF_TIMER0);

		case BEEP_AUTO_ACTION:
			return (BEEP_AUTO_OFF_TIMER);

		case BEEP_LOCKED:
			return (BEEP_LOCKED_OFF_TIMER);

		case BEEP_UNLOCKED:
			return (BEEP_UNLOCKED_OFF_TIMER);

		case BEEP_END_PRESSED:
			return (BEEP_END_OFF_TIMER);
	}
	return(0x0000);
}

//���ݲ�ͬ��������Ĳ�ͬ�׶Σ�������Ӧ�ķ�����ʱ
static u16 GetBeepOnTimer(u16 u16Case, u16 u16Order,u8 val)
{
	switch (u16Case)
	{
		case BEEP_KEY_PRESSED:
			return(BEEP_KEY_ON_TIMER);

		case BEEP_NEAR_SELECTED:
			if(val == 1)
				return (BEEP_NEAR_OFF_TIMER);
			else
				return (BEEP_NEAR_OFF_TIMER0);

		case BEEP_AUTO_ACTION:
			return (BEEP_AUTO_ON_TIMER);

		case BEEP_LOCKED:
			return (BEEP_LOCKED_ON_TIMER);

		case BEEP_UNLOCKED:
			return (BEEP_UNLOCKED_ON_TIMER);

		case BEEP_END_PRESSED:
			return (BEEP_END_ON_TIMER);
	}
	return(0x0000);
}

//��������called 10ms once
void BeepProc(void)
{
	static u8 temp = 0;
	//INT8U err;
	#if OS_CRITICAL_METHOD == 3                      /* Allocate storage for CPU status register */
	    OS_CPU_SR  cpu_sr = 0;
	#endif
	
	OS_ENTER_CRITICAL();

	if (vu16BeepCase)
	{
		switch (vu16BeepState)
		{
			case 0:	//�����׶�
				if (!vu16BeepOnTimer)	//��ʱ����
				{
					BEEP_OFF;
					vu16BeepOnTimer = 0xffff;
					if (vu16BeepCnt)
					{
						//����Ƕ�η�����������vu16BeepCnt��ͬ������������Ъʱ��

						vu16BeepOffTimer = GetBeepOffTimer(vu16BeepCase, vu16BeepCnt,temp);
						if(temp == 0)
							temp = 1;
						else
							temp = 0;
						vu16BeepState = 1;
					}
					else	//��������
					{
						vu16BeepOffTimer = 0xffff;
						vu16BeepState = 0;
						vu16BeepCase = 0;
					}
				}
				else
					vu16BeepOnTimer --;
			break;
				
			case 1:	//������Ъ
				if (!vu16BeepOffTimer)
				{
					vu16BeepOffTimer = 0xffff;
					if ((vu16BeepCnt != 0xffff) && vu16BeepCnt)
						vu16BeepCnt --;
					if (vu16BeepCnt)
					{
//						if (!(vu16BeepCase == BEEP_NEAR_SELECTED))
							BEEP_ON;
						vu16BeepState = 0;
						//����Ƕ�η�����������vu16BeepCnt��ͬ����������ʱ��
						vu16BeepOnTimer = GetBeepOnTimer(vu16BeepCase, vu16BeepCnt,temp);
//						if(temp == 0)
//							temp = 1;
//						else
//							temp = 0;
					}
					else	//��������
					{
						vu16BeepOnTimer = 0xffff;
						vu16BeepState = 0;
						vu16BeepCase = 0;
					}
				}
				else
					vu16BeepOffTimer --;
			break;
				case 2:	
					BEEP_ON;
					temp = 0;
					vu16BeepState = 0;
					break;
		}
	}

 	OS_EXIT_CRITICAL();
}


//----------------------------------------------------------
//��������
void BeepOn(u16 u16Case)
{
	//INT8U err;
	#if OS_CRITICAL_METHOD == 3			/* Allocate storage for CPU status register */
	    OS_CPU_SR  cpu_sr = 0;
	#endif

	if (vu16BeepCase != 0 && u16Case == BEEP_KEY_PRESSED)
		return;
	if ((vu16BeepCase == BEEP_NEAR_SELECTED || vu16BeepCase == BEEP_AUTO_ACTION) 
		&& (u16Case == BEEP_NEAR_SELECTED || u16Case == BEEP_AUTO_ACTION))
		return;

	OS_ENTER_CRITICAL();

	
	vu16BeepCase = u16Case;
	vu16BeepState = 2;
	vu16BeepOffTimer = 0xffff;
	switch(u16Case)
	{
		case BEEP_KEY_PRESSED:
			vu16BeepOnTimer = BEEP_KEY_ON_TIMER;
			vu16BeepCnt = BEEP_KEY_COUNT;
		break;

		case BEEP_NEAR_SELECTED:
			vu16BeepOnTimer = BEEP_NEAR_ON_TIMER_1ST;
			vu16BeepCnt = BEEP_NEAR_COUNT;
		break;

		case BEEP_AUTO_ACTION:
			vu16BeepOnTimer = BEEP_AUTO_ON_TIMER;
			vu16BeepCnt = BEEP_AUTO_COUNT;
		break;

		case BEEP_LOCKED:
			vu16BeepOnTimer = BEEP_LOCKED_ON_TIMER;
			vu16BeepCnt = BEEP_LOCKED_COUNT;
		break;

		case BEEP_UNLOCKED:
			vu16BeepOnTimer = BEEP_UNLOCKED_ON_TIMER;
			vu16BeepCnt = BEEP_UNLOCKED_COUNT;
		break;

		case BEEP_END_PRESSED:
			vu16BeepOnTimer = BEEP_END_ON_TIMER;
			vu16BeepCnt = BEEP_END_COUNT;
	}
	if (!(u16Case == BEEP_NEAR_SELECTED))
		BEEP_ON;

 	OS_EXIT_CRITICAL();
}

//ֹͣ�澯
void BeepOff(void)
{
	#if OS_CRITICAL_METHOD == 3			/* Allocate storage for CPU status register */
	    OS_CPU_SR  cpu_sr = 0;
	#endif

	OS_ENTER_CRITICAL();


	vu16BeepCase = 0;
	vu16BeepState = 0;
	vu16BeepOffTimer = 0xffff;
	vu16BeepOnTimer = 0xffff;
	vu16BeepCnt = 0xffff;
	BEEP_OFF;

 	OS_EXIT_CRITICAL();
}

//�ָ��ϴα�������
void BeepResume(void)
{
	BeepOn(vu16BeepCase);
}
// ����u32ms���룬u32ms������10ms�Ǳ�����ϵ
void Beep(u32 u32ms)
{
	if (u32ms)
	{
		BEEP_ON;
		OSTimeDly(u32ms / 10);
		BEEP_OFF;
	}
}

