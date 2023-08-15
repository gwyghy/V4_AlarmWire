#ifndef __JTBS_H__
#define __JTBS_H__

#include "bsp.h"


// ��ͣ�ӿ�
#define STOP_PORT			GPIOH
#define STOP_PIN			GPIO_Pin_10
#define STOP_GPIO_RCC		RCC_AHB1Periph_GPIOH

// �����ӿ�
#define LOCK_PORT			GPIOB
#define LOCK_PIN			GPIO_Pin_8
#define LOCK_GPIO_RCC		RCC_AHB1Periph_GPIOB

void StopLockInit(void);
u16 CheckBS(void);
u16 CheckJT(void);

#endif


