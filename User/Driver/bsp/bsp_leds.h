#ifndef __BSP_LEDS_H__
#define __BSP_LEDS_H__


#include "bsp.h"

#define LED_COM_TRS		1	//ͨ�ŷ���LED
#define LED_COM_REV		2	//ͨ�Ž���LED
#define LED_STOP		4	//��ͣLED
#define LED_MATCH_IDLE	16	//�������
#define LED_MATCH_IR	32	//�������ɹ�_���
#define LED_MATCH_BUSY	64	//���������С����̵�
#define LED_WL			128	//����ͨ��LED


// LED�ӿ�
//����ָʾ��
#define LED_TRS_PORT		GPIOH
#define LED_TRS_PIN			GPIO_Pin_15
#define LED_TRS_GPIO_RCC	RCC_AHB1Periph_GPIOH

//����ָʾ��
#define LED_REV_PORT		GPIOH
#define LED_REV_PIN			GPIO_Pin_14
#define LED_REV_GPIO_RCC	RCC_AHB1Periph_GPIOH

//��ͣ����ָʾ��
#define LED_STOP_PORT		GPIOH
#define LED_STOP_PIN		GPIO_Pin_4
#define LED_STOP_GPIO_RCC	RCC_AHB1Periph_GPIOH

//����ͨ��ָʾ��
#define LED_WL_PORT			GPIOH
#define LED_WL_GPIO_RCC		RCC_AHB1Periph_GPIOH
#define LED_WL_PIN			GPIO_Pin_13

//�������ָʾ��
#define LED_MATCH_PORT		GPIOI
#define LED_MATCH_GPIO_RCC	RCC_AHB1Periph_GPIOI
#define LED_MATCH_IDLE_PIN	GPIO_Pin_0
#define LED_MATCH_BUSY_PIN	GPIO_Pin_1



void LedInit(void);
//������Ϩ��ͨ���շ�LED���������ָʾ������ͨ��
void StateLed(u16 u16On, u16 u16Off);


#endif
