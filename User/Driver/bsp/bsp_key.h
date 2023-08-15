/*
*********************************************************************************************************
*	                                            DoubleWires
*                           Commuication Controller For The Mining Industry
*
*                                  (c) Copyright 1994-2013  HNDZ 
*                       All rights reserved.  Protected by international copyright laws.
*
*
*    File    : bsp_key.h
*    Module  : user
*    Version : V1.0
*    History :
*   -----------------
*              Version  Date           By            Note
*              v1.0     2013-08-15     
*
*********************************************************************************************************
*/
#ifndef __BSP_KEY_H
#define __BSP_KEY_H


/****************************************************************************************************
 * 根据板卡类型定义按键GPIO PIN
 ***************************************************************************************************/
#define RCC_ALL_KEY 			(RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOF | RCC_AHB1Periph_GPIOH | RCC_AHB1Periph_GPIOI | RCC_AHB1Periph_GPIOG)
#if		BOARD_VERSION == ARMFLY_V5

	/*
		STM32-V5 按键口线分配：
			K1 键      : PI8   (低电平表示按下)
			K2 键      : PC13  (低电平表示按下)
			K3 键      : PI11  (低电平表示按下)
			摇杆UP键   : PH2   (低电平表示按下)
			摇杆DOWN键 : PH3   (低电平表示按下)
			摇杆LEFT键 : PF11  (低电平表示按下)
			摇杆RIGHT键: PG7   (低电平表示按下)
			摇杆OK键   : PH15  (低电平表示按下)
	*/

	#define GPIO_PORT_K1    		GPIOI
	#define GPIO_PIN_K1	    		GPIO_Pin_8

	#define GPIO_PORT_K2    		GPIOC
	#define GPIO_PIN_K2	    		GPIO_Pin_13

	#define GPIO_PORT_K3    		GPIOI
	#define GPIO_PIN_K3	    		GPIO_Pin_11

	#define GPIO_PORT_K4    		GPIOH
	#define GPIO_PIN_K4	    		GPIO_Pin_2

	#define GPIO_PORT_K5    		GPIOH
	#define GPIO_PIN_K5	    		GPIO_Pin_3

	#define GPIO_PORT_K6    		GPIOF
	#define GPIO_PIN_K6	    		GPIO_Pin_11

	#define GPIO_PORT_K7    		GPIOG
	#define GPIO_PIN_K7	    		GPIO_Pin_7

	#define GPIO_PORT_K8    		GPIOH
	#define GPIO_PIN_K8	    		GPIO_Pin_15

	/* 根据应用程序的功能重命名按键宏 */
	#define KEY_DOWN_K1				KEY_1_DOWN
	#define KEY_UP_K1				KEY_1_UP
	#define KEY_LONG_K1				KEY_1_LONG

	#define KEY_DOWN_K2				KEY_2_DOWN
	#define KEY_UP_K2				KEY_2_UP
	#define KEY_LONG_K2				KEY_2_LONG

	#define KEY_DOWN_K3				KEY_3_DOWN
	#define KEY_UP_K3				KEY_3_UP
	#define KEY_LONG_K3				KEY_3_LONG

	#define JOY_DOWN_U				KEY_4_DOWN		/* 上 */
	#define JOY_UP_U				KEY_4_UP
	#define JOY_LONG_U				KEY_4_LONG

	#define JOY_DOWN_D				KEY_5_DOWN		/* 下 */
	#define JOY_UP_D				KEY_5_UP
	#define JOY_LONG_D				KEY_5_LONG

	#define JOY_DOWN_L				KEY_6_DOWN		/* 左 */
	#define JOY_UP_L				KEY_6_UP
	#define JOY_LONG_L				KEY_6_LONG

	#define JOY_DOWN_R				KEY_7_DOWN		/* 右 */
	#define JOY_UP_R				KEY_7_UP
	#define JOY_LONG_R				KEY_7_LONG

	#define JOY_DOWN_OK				KEY_8_DOWN		/* ok */
	#define JOY_UP_OK				KEY_8_UP
	#define JOY_LONG_OK				KEY_8_LONG

	
#elif	BOARD_VERSION == TK100_V1

	#define KEY_ROW_PORT			GPIOF
	#define KEY_ROW1_PIN			GPIO_Pin_12
	#define KEY_ROW2_PIN			GPIO_Pin_13
	#define KEY_ROW3_PIN			GPIO_Pin_14
	#define KEY_ROW4_PIN			GPIO_Pin_15
	
	#define KEY_LINE_PORT			GPIOG
	#define KEY_LINE1_PIN			GPIO_Pin_2
	#define KEY_LINE2_PIN			GPIO_Pin_3
	#define KEY_LINE3_PIN			GPIO_Pin_4
	#define KEY_LINE4_PIN			GPIO_Pin_5
	
	#define KEY_LEFT1_ROW_PORT		KEY_ROW_PORT
	#define KEY_LEFT1_ROW_PIN		KEY_ROW1_PIN
	#define KEY_LEFT1_LINE_PORT		KEY_LINE_PORT
	#define KEY_LEFT1_LINE_PIN		KEY_LINE4_PIN
	
	#define KEY_LEFT2_ROW_PORT		KEY_ROW_PORT
	#define KEY_LEFT2_ROW_PIN		KEY_ROW1_PIN
	#define KEY_LEFT2_LINE_PORT		KEY_LINE_PORT
	#define KEY_LEFT2_LINE_PIN		KEY_LINE3_PIN
	
	#define KEY_LEFT3_ROW_PORT		KEY_ROW_PORT
	#define KEY_LEFT3_ROW_PIN		KEY_ROW1_PIN
	#define KEY_LEFT3_LINE_PORT		KEY_LINE_PORT
	#define KEY_LEFT3_LINE_PIN		KEY_LINE2_PIN
	
	#define KEY_LEFT4_ROW_PORT		KEY_ROW_PORT
	#define KEY_LEFT4_ROW_PIN		KEY_ROW1_PIN
	#define KEY_LEFT4_LINE_PORT		KEY_LINE_PORT
	#define KEY_LEFT4_LINE_PIN		KEY_LINE1_PIN
	
	#define KEY_RIGHT1_ROW_PORT		KEY_ROW_PORT
	#define KEY_RIGHT1_ROW_PIN		KEY_ROW2_PIN
	#define KEY_RIGHT1_LINE_PORT	KEY_LINE_PORT
	#define KEY_RIGHT1_LINE_PIN		KEY_LINE4_PIN
	
	#define KEY_RIGHT2_ROW_PORT		KEY_ROW_PORT
	#define KEY_RIGHT2_ROW_PIN		KEY_ROW2_PIN
	#define KEY_RIGHT2_LINE_PORT	KEY_LINE_PORT
	#define KEY_RIGHT2_LINE_PIN		KEY_LINE3_PIN
	
	#define KEY_RIGHT3_ROW_PORT		KEY_ROW_PORT
	#define KEY_RIGHT3_ROW_PIN		KEY_ROW2_PIN
	#define KEY_RIGHT3_LINE_PORT	KEY_LINE_PORT
	#define KEY_RIGHT3_LINE_PIN		KEY_LINE2_PIN
	
	#define KEY_RIGHT4_ROW_PORT		KEY_ROW_PORT
	#define KEY_RIGHT4_ROW_PIN		KEY_ROW2_PIN
	#define KEY_RIGHT4_LINE_PORT	KEY_LINE_PORT
	#define KEY_RIGHT4_LINE_PIN		KEY_LINE1_PIN
	
	#define KEY_FUNC1_ROW_PORT		KEY_ROW_PORT
	#define KEY_FUNC1_ROW_PIN		KEY_ROW3_PIN
	#define KEY_FUNC1_LINE_PORT		KEY_LINE_PORT
	#define KEY_FUNC1_LINE_PIN		KEY_LINE4_PIN
	
	#define KEY_FUNC2_ROW_PORT		KEY_ROW_PORT
	#define KEY_FUNC2_ROW_PIN		KEY_ROW3_PIN
	#define KEY_FUNC2_LINE_PORT		KEY_LINE_PORT
	#define KEY_FUNC2_LINE_PIN		KEY_LINE3_PIN
	
	#define KEY_FUNC3_ROW_PORT		KEY_ROW_PORT
	#define KEY_FUNC3_ROW_PIN		KEY_ROW3_PIN
	#define KEY_FUNC3_LINE_PORT		KEY_LINE_PORT
	#define KEY_FUNC3_LINE_PIN		KEY_LINE2_PIN
	
	#define KEY_FUNC4_ROW_PORT		KEY_ROW_PORT
	#define KEY_FUNC4_ROW_PIN		KEY_ROW3_PIN
	#define KEY_FUNC4_LINE_PORT		KEY_LINE_PORT
	#define KEY_FUNC4_LINE_PIN		KEY_LINE1_PIN
	
	#define KEY_MENU_ROW_PORT		KEY_ROW_PORT
	#define KEY_MENU_ROW_PIN		KEY_ROW4_PIN
	#define KEY_MENU_LINE_PORT		KEY_LINE_PORT
	#define KEY_MENU_LINE_PIN		KEY_LINE1_PIN
	
	

#elif	BOARD_VERSION == TK100_V2


#endif

/****************************************************************************************************
 * 定义新的数据类型按键ID，需要注意按键ID从0开始，在程序中如果做判断，应格外注意这点
 ***************************************************************************************************/
typedef enum
{
#if		BOARD_VERSION == ARMFLY_V5
	KID_K1 = 0,
	KID_K2,
	KID_K3,
	KID_JOY_U,
	KID_JOY_D,
	KID_JOY_L,
	KID_JOY_R,
	KID_JOY_OK,
#else
	KID_LEFT1 = 0,
	KID_LEFT2,
	KID_LEFT3,
	KID_LEFT4,
	KID_RIGHT1,
	KID_RIGHT2,
	KID_RIGHT3,
	KID_RIGHT4,	
	KID_FUNC1,
	KID_FUNC2,
	KID_FUNC3,
	KID_FUNC4,
	KID_MENU,
#endif
	KID_MAX
}KeyValue_t;

/****************************************************************************************************
 * 定义新的数据类型按键事件
 ***************************************************************************************************/
typedef enum
{
	tKeyDown = 1,
	tKeyUp,
	tKeyDoubleClick,
	tKeyLongDown
}KeyEvent_t;

/****************************************************************************************************
 * 按键检测结构体，每个按键对应一个这样的结构体，在按键基础检测中不对按键双击和长按进行检测
 ***************************************************************************************************/
#define KEY_FILTER_TIME             2 

#if 0
	#define KEY_LONG_TIME           100		/* 单位ms， 持续1秒，认为长按事件                      */
	#define KEY_TYPE_DOUBLE	        0x0001  /* 检测双击事件                                        */
	#define KEY_TYPE_LONG           0x0002  /* 检测长按事件                                        */
#endif

typedef struct
{
	uint8_t   (*IsKeyDownFunc)(void);       /* 按键按下的判断函数,1表示按下                        */
	uint16_t  Count;					    /* 滤波器计数器                                        */
	uint8_t   State;					    /* 按键当前状态（按下还是弹起），1-按下  0-弹起        */
	
#if 0
	uint16_t  LongCount;				    /* 长按计数器                                          */
	uint16_t  LongTime;				        /* 按键按下持续时间, 0表示不检测长按                   */
	uint8_t   Type;                         /* 是否检测双击、长按事件,默认仅检测单击事件           */
	uint8_t   RepeatSpeed;			        /* 连续按键周期                                        */
	uint8_t   RepeatCount;			        /* 连续按键计数器                                      */
#endif
}KEY_T;

/****************************************************************************************************
 * 关于按键FIFO，按键相对于其他事件而言是慢事件，所以在此不采用按键FIFO
 * 在程序中需要注意的地方是：按键事件处理的任务一定要比按键检测的任务高
 ***************************************************************************************************/
typedef struct
{
	uint32_t   keyValue;
	KeyEvent_t   keyEvent;
}STR_KEY;

#if 0
#define KEY_FIFO_SIZE	            4
typedef struct
{
	STR_KEY Buf[KEY_FIFO_SIZE];		        /* 键值缓冲区                                          */
	uint8_t Read;					        /* 缓冲区读指针                                        */
	uint8_t Write;					        /* 缓冲区写指针                                        */
}KEY_FIFO_T;
#endif

/* 供外部调用的函数声明 */
int  BSP_InitKey	(void);
void BSP_KeyScan	(void);
int BSP_PutKey(uint32_t KeyValue, KeyEvent_t KeyEvent);
#endif

