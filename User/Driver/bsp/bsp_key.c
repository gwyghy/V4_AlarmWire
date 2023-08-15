/*
*********************************************************************************************************
*	                                            DoubleWires
*                           Commuication Controller For The Mining Industry
*
*                                  (c) Copyright 1994-2013  HNDZ 
*                       All rights reserved.  Protected by international copyright laws.
*
*
*    File    : bsp_key.c
*    Module  : user
*    Version : V1.0
*    History :
*   -----------------
*              Version  Date           By            Note
*              v1.0     2013-08-15     
*
*********************************************************************************************************
*/
#include "includes.h"
#include "bsp.h"



static KEY_T   s_tBtn[KID_MAX];
static STR_KEY gKey;

/*+
*********************************************************************************************************
*	函 数 名: IsKeyDownX
*	功能说明: 判断按键是否按下
*	形    参: 无
*	返 回 值: 返回值1 表示按下，0表示未按下
*********************************************************************************************************
*/
#if		BOARD_VERSION == ARMFLY_V5

static uint8_t IsKey1_Down1   (void) {if ((GPIO_PORT_K1->IDR & GPIO_PIN_K1) == 0) return 1;else return 0;}
static uint8_t IsKey2_Down2   (void) {if ((GPIO_PORT_K2->IDR & GPIO_PIN_K2) == 0) return 1;else return 0;}
static uint8_t IsKey3_Down3   (void) {if ((GPIO_PORT_K3->IDR & GPIO_PIN_K3) == 0) return 1;else return 0;}
static uint8_t IsJoy_U_Down4  (void) {if ((GPIO_PORT_K4->IDR & GPIO_PIN_K4) == 0) return 1;else return 0;}
static uint8_t IsJoy_D_Down5  (void) {if ((GPIO_PORT_K5->IDR & GPIO_PIN_K5) == 0) return 1;else return 0;}
static uint8_t IsJoy_L_Down6  (void) {if ((GPIO_PORT_K6->IDR & GPIO_PIN_K6) == 0) return 1;else return 0;}
static uint8_t IsJoy_R_Down7  (void) {if ((GPIO_PORT_K7->IDR & GPIO_PIN_K7) == 0) return 1;else return 0;}
static uint8_t IsJoy_OK_Down8 (void) {if ((GPIO_PORT_K8->IDR & GPIO_PIN_K8) == 0) return 1;else return 0;}

#else

static uint8_t IsKey_Down(GPIO_TypeDef *row_port,int row_pin,GPIO_TypeDef *line_port,int line_pin) 
{
	uint8_t rtn;
	
	GPIO_WriteBit(line_port, line_pin, Bit_SET);
	PLATFORM_OS_TimeDly(2);
	
	if ((row_port->IDR & row_pin) == 0)	
		rtn = 0;
	else
		rtn = 1;

	GPIO_WriteBit(line_port, line_pin, Bit_RESET);
	return rtn;
}

static uint8_t IsKeyLeft1_Down  (void) {return(IsKey_Down( KEY_LEFT1_ROW_PORT,  KEY_LEFT1_ROW_PIN,  KEY_LEFT1_LINE_PORT,  KEY_LEFT1_LINE_PIN));}
static uint8_t IsKeyLeft2_Down  (void) {return(IsKey_Down( KEY_LEFT2_ROW_PORT,  KEY_LEFT2_ROW_PIN,  KEY_LEFT2_LINE_PORT,  KEY_LEFT2_LINE_PIN));}
static uint8_t IsKeyLeft3_Down  (void) {return(IsKey_Down( KEY_LEFT3_ROW_PORT,  KEY_LEFT3_ROW_PIN,  KEY_LEFT3_LINE_PORT,  KEY_LEFT3_LINE_PIN));}
static uint8_t IsKeyLeft4_Down  (void) {return(IsKey_Down( KEY_LEFT4_ROW_PORT,  KEY_LEFT4_ROW_PIN,  KEY_LEFT4_LINE_PORT,  KEY_LEFT4_LINE_PIN));}

static uint8_t IsKeyRight1_Down (void) {return(IsKey_Down(KEY_RIGHT1_ROW_PORT, KEY_RIGHT1_ROW_PIN, KEY_RIGHT1_LINE_PORT, KEY_RIGHT1_LINE_PIN));}
static uint8_t IsKeyRight2_Down (void) {return(IsKey_Down(KEY_RIGHT2_ROW_PORT, KEY_RIGHT2_ROW_PIN, KEY_RIGHT2_LINE_PORT, KEY_RIGHT2_LINE_PIN));}
static uint8_t IsKeyRight3_Down (void) {return(IsKey_Down(KEY_RIGHT3_ROW_PORT, KEY_RIGHT3_ROW_PIN, KEY_RIGHT3_LINE_PORT, KEY_RIGHT3_LINE_PIN));}
static uint8_t IsKeyRight4_Down (void) {return(IsKey_Down(KEY_RIGHT4_ROW_PORT, KEY_RIGHT4_ROW_PIN, KEY_RIGHT4_LINE_PORT, KEY_RIGHT4_LINE_PIN));}

static uint8_t IsKeyFunc1_Down  (void) {return(IsKey_Down( KEY_FUNC1_ROW_PORT,  KEY_FUNC1_ROW_PIN,  KEY_FUNC1_LINE_PORT,  KEY_FUNC1_LINE_PIN));}
static uint8_t IsKeyFunc2_Down  (void) {return(IsKey_Down( KEY_FUNC2_ROW_PORT,  KEY_FUNC2_ROW_PIN,  KEY_FUNC2_LINE_PORT,  KEY_FUNC2_LINE_PIN));}
static uint8_t IsKeyFunc3_Down  (void) {return(IsKey_Down( KEY_FUNC3_ROW_PORT,  KEY_FUNC3_ROW_PIN,  KEY_FUNC3_LINE_PORT,  KEY_FUNC3_LINE_PIN));}
static uint8_t IsKeyFunc4_Down  (void) {return(IsKey_Down( KEY_FUNC4_ROW_PORT,  KEY_FUNC4_ROW_PIN,  KEY_FUNC4_LINE_PORT,  KEY_FUNC4_LINE_PIN));}

static uint8_t IsKeyMenu_Down   (void) {return(IsKey_Down(  KEY_MENU_ROW_PORT,    KEY_MENU_ROW_PIN,  KEY_MENU_LINE_PORT,   KEY_MENU_LINE_PIN));}


#endif
	
/*
*********************************************************************************************************
*	函 数 名: BSP_InitKeyHard
*	功能说明: 配置按键对应的GPIO
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
static void BSP_InitKeyHard(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/* 第1步：打开GPIO时钟 */
	RCC_AHB1PeriphClockCmd(RCC_ALL_KEY, ENABLE);

	/* 第2步：配置所有的按键GPIO为浮动输入模式(实际上CPU复位后就是输入状态) */
	
#if		BOARD_VERSION == ARMFLY_V5
	
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN;		/* 设为输入口 */
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		/* 设为推挽模式 */
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;	/* 无需上下拉电阻 */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	/* IO口最大速度 */

	GPIO_InitStructure.GPIO_Pin   = GPIO_PIN_K1;
	GPIO_Init(GPIO_PORT_K1, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin   = GPIO_PIN_K2;
	GPIO_Init(GPIO_PORT_K2, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin   = GPIO_PIN_K3;
	GPIO_Init(GPIO_PORT_K3, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin   = GPIO_PIN_K4;
	GPIO_Init(GPIO_PORT_K4, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin   = GPIO_PIN_K5;
	GPIO_Init(GPIO_PORT_K5, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin   = GPIO_PIN_K6;
	GPIO_Init(GPIO_PORT_K6, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin   = GPIO_PIN_K7;
	GPIO_Init(GPIO_PORT_K7, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin   = GPIO_PIN_K8;
	GPIO_Init(GPIO_PORT_K8, &GPIO_InitStructure);

#elif BOARD_VERSION == TK100_V1

	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN;		/* 设为输入口     */
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		/* 设为推挽模式   */
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;	/* 无需上下拉电阻 */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	/* IO口最大速度   */	

	GPIO_InitStructure.GPIO_Pin   = KEY_ROW1_PIN | KEY_ROW2_PIN | KEY_ROW3_PIN | KEY_ROW4_PIN;
	GPIO_Init(KEY_ROW_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;		/* 设为输出口     */
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		/* 设为推挽模式   */
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;		/* 上拉电阻       */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	/* IO口最大速度   */	
	
	GPIO_InitStructure.GPIO_Pin   = KEY_LINE1_PIN | KEY_LINE2_PIN | KEY_LINE3_PIN | KEY_LINE4_PIN;
	GPIO_Init(KEY_LINE_PORT, &GPIO_InitStructure);
	
	GPIO_WriteBit(KEY_LINE_PORT, KEY_LINE1_PIN, Bit_RESET);
	GPIO_WriteBit(KEY_LINE_PORT, KEY_LINE2_PIN, Bit_RESET);
	GPIO_WriteBit(KEY_LINE_PORT, KEY_LINE3_PIN, Bit_RESET);
	GPIO_WriteBit(KEY_LINE_PORT, KEY_LINE4_PIN, Bit_RESET);

#endif
}

/*
*********************************************************************************************************
*	函 数 名: BSP_InitKeyVar
*	功能说明: 初始化按键变量
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
static void BSP_InitKeyVar(void)
{
	KeyValue_t i;

	for (i = (KeyValue_t)0; i < KID_MAX; i++)
	{		
		s_tBtn[i].Count              = 0;
		//s_tBtn[i].LongCount          = 0;
		//s_tBtn[i].LongTime           = 0;
		//s_tBtn[i].Type               = 0;
		s_tBtn[i].State              = 0;
		//s_tBtn[i].RepeatSpeed        = 0;
		//s_tBtn[i].RepeatCount        = 0;
	}

#if		BOARD_VERSION == ARMFLY_V5
	s_tBtn[ KID_K1   ].IsKeyDownFunc   = IsKey1_Down1;
	s_tBtn[ KID_K2   ].IsKeyDownFunc   = IsKey2_Down2;
	s_tBtn[ KID_K3   ].IsKeyDownFunc   = IsKey3_Down3;
	s_tBtn[ KID_JOY_U].IsKeyDownFunc   = IsJoy_U_Down4;
	s_tBtn[ KID_JOY_D].IsKeyDownFunc   = IsJoy_D_Down5;
	s_tBtn[ KID_JOY_L].IsKeyDownFunc   = IsJoy_L_Down6;
	s_tBtn[ KID_JOY_R].IsKeyDownFunc   = IsJoy_R_Down7;
	s_tBtn[ KID_JOY_OK].IsKeyDownFunc  = IsJoy_OK_Down8;
#else
	s_tBtn[ KID_LEFT1 ].IsKeyDownFunc  = IsKeyLeft1_Down;
	s_tBtn[ KID_LEFT2 ].IsKeyDownFunc  = IsKeyLeft2_Down;
	s_tBtn[ KID_LEFT3 ].IsKeyDownFunc  = IsKeyLeft3_Down;
	s_tBtn[ KID_LEFT4 ].IsKeyDownFunc  = IsKeyLeft4_Down;
	s_tBtn[ KID_RIGHT1].IsKeyDownFunc  = IsKeyRight1_Down;
	s_tBtn[ KID_RIGHT2].IsKeyDownFunc  = IsKeyRight2_Down;
	s_tBtn[ KID_RIGHT3].IsKeyDownFunc  = IsKeyRight3_Down;
	s_tBtn[ KID_RIGHT4].IsKeyDownFunc  = IsKeyRight4_Down;
	s_tBtn[ KID_FUNC1 ].IsKeyDownFunc  = IsKeyFunc1_Down;
	s_tBtn[ KID_FUNC2 ].IsKeyDownFunc  = IsKeyFunc2_Down;
	s_tBtn[ KID_FUNC3 ].IsKeyDownFunc  = IsKeyFunc3_Down;
	s_tBtn[ KID_FUNC4 ].IsKeyDownFunc  = IsKeyFunc4_Down;
	s_tBtn[ KID_MENU  ].IsKeyDownFunc  = IsKeyMenu_Down;
	
#endif
}

/*
*********************************************************************************************************
*	函 数 名: bsp_InitKey
*	功能说明: 初始化按键. 该函数被 bsp_Init() 调用。
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
int BSP_InitKey(void)
{
	BSP_InitKeyVar();		/* 初始化按键变量 */
	BSP_InitKeyHard();		/* 初始化按键硬件 */
	
	return(1);
}

/*
*********************************************************************************************************
*	函 数 名: bsp_PutKey
*	功能说明: 将1个键值压入按键FIFO缓冲区。可用于模拟一个按键。
*	形    参：KeyValue: 键值   
*             KeyEvent: 按键事件代码
*	返 回 值: 
*********************************************************************************************************
*/
int BSP_PutKey(uint32_t KeyValue, KeyEvent_t KeyEvent)
{
	uint8_t err;
	
	gKey.keyValue = KeyValue;
	gKey.keyEvent = KeyEvent;
	
	err = OSMboxPost(KeyMbox,(void *)&gKey);
	
	if (err == DEF_OK)
	{
		return 1;
	}else
	if (err == DEF_MBOX_FULL)
	{
		/* 此处应加判断是否需要延时重发，同时是否需要写入错误日志 */ 
		return -1;
	}
	
	return -2;
}

/*
*********************************************************************************************************
*	函 数 名: BSP_DetectKey
*	功能说明: 检测一个按键。非阻塞状态，必须被周期性的调用。建议循环调用周期为5ms
*	形    参：按键结构变量指针
*	返 回 值: 无
*********************************************************************************************************
*/
static void BSP_DetectKey(KeyValue_t i)
{
	KEY_T *pBtn = &s_tBtn[i];
	
	if (pBtn->IsKeyDownFunc == NULL)
	{
		RUN_TRACE_ERR(LED_ERROR,"DetectButton(), s_tBtn[i].IsKeyDownFunc undefine");
	}
	
	if (pBtn->IsKeyDownFunc())
	{
		if (pBtn->Count < KEY_FILTER_TIME)
		{
			pBtn->Count++;
		}
		else 
		{
			if (pBtn->State == 0)
			{
				pBtn->State = 1;	
				BSP_PutKey(i, tKeyDown);
			}
			pBtn->Count =0;
		}
	}
	else
	{
		if (pBtn->Count < KEY_FILTER_TIME)
		{
			pBtn->Count++;
		}
		else
		{
			if (pBtn->State == 1)
			{
				pBtn->State = 0;
				BSP_PutKey(i,tKeyUp);
			}
			pBtn->Count =0;
		}
	}
}

/*
*********************************************************************************************************
*	函 数 名: bsp_KeyScan
*	功能说明: 扫描所有按键。非阻塞，被systick中断周期性的调用
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void BSP_KeyScan(void)
{
	KeyValue_t i;

	for (i = (KeyValue_t)0; i < KID_MAX; i++)
	{
		BSP_DetectKey(i);
	}
}
