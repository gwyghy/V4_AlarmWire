#ifndef __BSP_LEDS_H__
#define __BSP_LEDS_H__


#include "bsp.h"

#define LED_COM_TRS		1	//通信发送LED
#define LED_COM_REV		2	//通信接收LED
#define LED_STOP		4	//急停LED
#define LED_MATCH_IDLE	16	//对码空闲
#define LED_MATCH_IR	32	//红外对码成功_红灯
#define LED_MATCH_BUSY	64	//红外对码进行——绿灯
#define LED_WL			128	//无线通信LED


// LED接口
//发送指示灯
#define LED_TRS_PORT		GPIOH
#define LED_TRS_PIN			GPIO_Pin_15
#define LED_TRS_GPIO_RCC	RCC_AHB1Periph_GPIOH

//接收指示灯
#define LED_REV_PORT		GPIOH
#define LED_REV_PIN			GPIO_Pin_14
#define LED_REV_GPIO_RCC	RCC_AHB1Periph_GPIOH

//急停闭锁指示灯
#define LED_STOP_PORT		GPIOH
#define LED_STOP_PIN		GPIO_Pin_4
#define LED_STOP_GPIO_RCC	RCC_AHB1Periph_GPIOH

//无线通信指示灯
#define LED_WL_PORT			GPIOH
#define LED_WL_GPIO_RCC		RCC_AHB1Periph_GPIOH
#define LED_WL_PIN			GPIO_Pin_13

//红外对码指示灯
#define LED_MATCH_PORT		GPIOI
#define LED_MATCH_GPIO_RCC	RCC_AHB1Periph_GPIOI
#define LED_MATCH_IDLE_PIN	GPIO_Pin_0
#define LED_MATCH_BUSY_PIN	GPIO_Pin_1



void LedInit(void);
//点亮和熄灭通信收发LED、红外对码指示、无线通信
void StateLed(u16 u16On, u16 u16Off);


#endif
