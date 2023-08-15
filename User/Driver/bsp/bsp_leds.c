/***********************************************************
* 模块名：jtbs.c
* 作者：  HNDZ
*
*
************************************************************/

#include	"bsp_leds.h"


static u16	s_u16LedState = 0x00;	//通信收发、急停、闭锁LED指示灯的状态

void LedInit(void)
{
	GPIO_InitTypeDef			GPIO_InitStructure;

	/* GPIO Periph clock enable */
	RCC_AHB1PeriphClockCmd(LED_TRS_GPIO_RCC
	                       | LED_REV_GPIO_RCC
	                       | LED_STOP_GPIO_RCC
												 | LED_MATCH_GPIO_RCC
												 | LED_WL_GPIO_RCC,ENABLE);



	/* Configure LED_STOP_PIN in output pushpull mode */
	GPIO_InitStructure.GPIO_Pin = LED_STOP_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(LED_STOP_PORT, &GPIO_InitStructure);

	/* Set LED_STOP_PIN */
	GPIO_SetBits(LED_STOP_PORT, LED_STOP_PIN);

	/* Configure LED_TRS_PIN in output pushpull mode */
	GPIO_InitStructure.GPIO_Pin = LED_TRS_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(LED_TRS_PORT, &GPIO_InitStructure);

	/* Set LED_TRS_PIN */
	GPIO_SetBits(LED_TRS_PORT, LED_TRS_PIN);

	/* Configure LED_REV_PIN in output pushpull mode */
	GPIO_InitStructure.GPIO_Pin = LED_REV_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(LED_REV_PORT, &GPIO_InitStructure);

	/* Set LED_REV_PIN */
	GPIO_SetBits(LED_REV_PORT, LED_REV_PIN);

	/* Configure LED_MATCH_IDLE_PIN in output pushpull mode */
	GPIO_InitStructure.GPIO_Pin = LED_MATCH_IDLE_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(LED_MATCH_PORT, &GPIO_InitStructure);

	/* Set LED_MATCH_IDLE_PIN */
	GPIO_SetBits(LED_MATCH_PORT, LED_MATCH_IDLE_PIN);

	/* Configure LED_MATCH_BUSY_PIN in output pushpull mode */
	GPIO_InitStructure.GPIO_Pin = LED_MATCH_BUSY_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(LED_MATCH_PORT, &GPIO_InitStructure);

	/* Set LED_MATCH_BUSY_PIN */
	GPIO_SetBits(LED_MATCH_PORT, LED_MATCH_BUSY_PIN);

	/* Configure LED_WL_PIN in output pushpull mode */
	GPIO_InitStructure.GPIO_Pin = LED_WL_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(LED_WL_PORT, &GPIO_InitStructure);

	/* Set LED_WL_PIN */
	GPIO_SetBits(LED_WL_PORT, LED_WL_PIN);
	s_u16LedState = 0xffff;													// 通信收发、急停、闭锁LED指示灯的状态
	StateLed(0,LED_STOP);
}


////点亮和熄灭通信收发、急停、闭锁LED
//点亮和熄灭通信收发LED、红外对码指示、无线通信
void StateLed(u16 u16On, u16 u16Off)
{
	if (u16On)
	{
		s_u16LedState |= u16On;
		if (u16On & LED_COM_TRS)
			GPIO_ResetBits(LED_TRS_PORT, LED_TRS_PIN);
		if (u16On & LED_COM_REV)
			GPIO_ResetBits(LED_REV_PORT, LED_REV_PIN);
		if (s_u16LedState & (LED_STOP))
			GPIO_ResetBits(LED_STOP_PORT, LED_STOP_PIN);

		if (u16On & LED_MATCH_IDLE)
			GPIO_SetBits(LED_MATCH_PORT, LED_MATCH_IDLE_PIN|LED_MATCH_BUSY_PIN);
		if (u16On & LED_MATCH_IR)
			GPIO_ResetBits(LED_MATCH_PORT, LED_MATCH_IDLE_PIN);
		if (u16On & LED_MATCH_BUSY)
			GPIO_ResetBits(LED_MATCH_PORT, LED_MATCH_BUSY_PIN);
		if (u16On & LED_WL)
			GPIO_ResetBits(LED_WL_PORT, LED_WL_PIN);

	}
	if (u16Off)
	{
		s_u16LedState &= (~u16Off);
		if (u16Off & LED_COM_TRS)
			GPIO_SetBits(LED_TRS_PORT, LED_TRS_PIN);
		if (u16Off & LED_COM_REV)
			GPIO_SetBits(LED_REV_PORT, LED_REV_PIN);
		
		if (!(s_u16LedState & (LED_STOP)))
			GPIO_SetBits(LED_STOP_PORT, LED_STOP_PIN);

		if ((u16Off & LED_MATCH_IDLE) && !(u16On & (LED_MATCH_IR|LED_MATCH_BUSY)))
			GPIO_SetBits(LED_MATCH_PORT, LED_MATCH_IDLE_PIN|LED_MATCH_BUSY_PIN);
		if (u16Off & LED_MATCH_IR)
			GPIO_SetBits(LED_MATCH_PORT, LED_MATCH_IDLE_PIN);
		if (u16Off & LED_MATCH_BUSY)
			GPIO_SetBits(LED_MATCH_PORT, LED_MATCH_BUSY_PIN);
		if (u16Off & LED_WL)
			GPIO_SetBits(LED_WL_PORT, LED_WL_PIN);

	}
}
////点亮和熄灭通信收发、急停、闭锁LED
//点亮和熄灭通信收发LED
u16 IsStateLedOn(u16 u16StateLed)
{
	return(s_u16LedState&u16StateLed);
}
