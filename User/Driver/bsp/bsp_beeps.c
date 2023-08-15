#include	"bsp_beeps.h"


#define BEEP_ON		ToBeep()
#define BEEP_OFF	StopBeep()

static vu16 vu16BeepCase = 0;				//告警种类 =0：空闲
static vu16 vu16BeepState = 0;				//告警阶段；=0：告警期；=1：间歇
static vu16 vu16BeepOffTimer = 0xffff;		//间歇计时（x*10ms）
static vu16 vu16BeepOnTimer = 0xffff;		//告警计时（x*10ms）
static vu16 vu16BeepCnt = 0;				//告警次数

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
//发声
void ToBeep(void)
{
	#if OS_CRITICAL_METHOD == 3                      /* Allocate storage for CPU status register           */
	    OS_CPU_SR  cpu_sr = 0;
	#endif
	
	OS_ENTER_CRITICAL();
	GPIO_SetBits(BEEP_PORT, BEEP_PIN);
 	OS_EXIT_CRITICAL();
}
//停止发声
void StopBeep(void)
{
	#if OS_CRITICAL_METHOD == 3                      /* Allocate storage for CPU status register           */
	    OS_CPU_SR  cpu_sr = 0;
	#endif
	
	OS_ENTER_CRITICAL();
	GPIO_ResetBits(BEEP_PORT, BEEP_PIN);
 	OS_EXIT_CRITICAL();
}

//依据不同发声种类的不同阶段，返回相应的发声间隔延时
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

//依据不同发声种类的不同阶段，返回相应的发声延时
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

//发声处理，called 10ms once
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
			case 0:	//发声阶段
				if (!vu16BeepOnTimer)	//计时结束
				{
					BEEP_OFF;
					vu16BeepOnTimer = 0xffff;
					if (vu16BeepCnt)
					{
						//如果是多次发声，则，依据vu16BeepCnt不同，调整发声间歇时间

						vu16BeepOffTimer = GetBeepOffTimer(vu16BeepCase, vu16BeepCnt,temp);
						if(temp == 0)
							temp = 1;
						else
							temp = 0;
						vu16BeepState = 1;
					}
					else	//结束发声
					{
						vu16BeepOffTimer = 0xffff;
						vu16BeepState = 0;
						vu16BeepCase = 0;
					}
				}
				else
					vu16BeepOnTimer --;
			break;
				
			case 1:	//发声间歇
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
						//如果是多次发声，则，依据vu16BeepCnt不同，调整发声时间
						vu16BeepOnTimer = GetBeepOnTimer(vu16BeepCase, vu16BeepCnt,temp);
//						if(temp == 0)
//							temp = 1;
//						else
//							temp = 0;
					}
					else	//结束发声
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
//启动发声
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

//停止告警
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

//恢复上次报警种类
void BeepResume(void)
{
	BeepOn(vu16BeepCase);
}
// 发声u32ms毫秒，u32ms必须与10ms是倍数关系
void Beep(u32 u32ms)
{
	if (u32ms)
	{
		BEEP_ON;
		OSTimeDly(u32ms / 10);
		BEEP_OFF;
	}
}

