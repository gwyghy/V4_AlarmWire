#ifndef __BSP_BEEPS_H__
#define __BSP_BEEPS_H__

#include "bsp.h"

// 蜂鸣器接口
#define BEEP_PORT			GPIOB
#define BEEP_PIN			GPIO_Pin_9
#define BEEP_GPIO_RCC		RCC_AHB1Periph_GPIOB


#define	BEEP_KEY_PRESSED		(u16)1	//按键音
#define BEEP_KEY_COUNT			(u16)1	//发声次数
#define BEEP_KEY_ON_TIMER		(u16)15	//发声时长（*10ms）
#define BEEP_KEY_OFF_TIMER		(u16)15	//0	//间歇时长（*10ms）	//改变发声逻辑后，不能为0

#define	BEEP_NEAR_SELECTED		(u16)2	//邻/隔架选中
#define BEEP_NEAR_COUNT			(u16)0xffff	//发声次数
#define BEEP_NEAR_ON_TIMER_1ST	(u16)37	//第一次发声时长（*10ms）
#define BEEP_NEAR_ON_TIMER		(u16)40	//发声时长（*10ms）
#define BEEP_NEAR_ON_TIMER0		(u16)40	//发声时长（*10ms
#define BEEP_NEAR_OFF_TIMER		(u16)20	//间歇时长（*10ms）		//改变发声逻辑后，不能为0
#define BEEP_NEAR_OFF_TIMER0  (u16)40	//间歇时长（*10ms）		//改变发声逻辑后，不能为0
#define	BEEP_AUTO_ACTION		(u16)3	//自动动作
#define BEEP_AUTO_COUNT			(u16)0xffff	//发声次数
#define BEEP_AUTO_ON_TIMER		(u16)105	//发声时长（*10ms）
#define BEEP_AUTO_OFF_TIMER		(u16)35	//间歇时长（*10ms）		//改变发声逻辑后，不能为0

#define	BEEP_LOCKED				(u16)4	//闭锁急停
#define BEEP_LOCKED_COUNT		(u16)2	//发声次数
#define BEEP_LOCKED_ON_TIMER	(u16)45	//发声时长（*10ms）
#define BEEP_LOCKED_OFF_TIMER	(u16)45	//间歇时长（*10ms）		//改变发声逻辑后，不能为0

#define	BEEP_UNLOCKED			(u16)5	//闭锁急停解锁
#define BEEP_UNLOCKED_COUNT		(u16)3	//发声次数
#define BEEP_UNLOCKED_ON_TIMER	(u16)45	//发声时长（*10ms）
#define BEEP_UNLOCKED_OFF_TIMER	(u16)45	//间歇时长（*10ms）		//改变发声逻辑后，不能为0

#define	BEEP_END_PRESSED		(u16)6	//停止音
#define BEEP_END_COUNT			(u16)1	//发声次数
#define BEEP_END_ON_TIMER		(u16)25	//发声时长（*10ms）
#define BEEP_END_OFF_TIMER		(u16)15	//0	//间歇时长（*10ms）	//改变发声逻辑后，不能为0


void Beep_Init(void);

void ToBeep(void);
void StopBeep(void);
void BeepProc(void);
void BeepOn(u16 u16Case);
void BeepOff(void);
void BeepResume(void);
void Beep(u32 u32ms);

#endif
