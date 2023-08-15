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
*	�� �� ��: IsKeyDownX
*	����˵��: �жϰ����Ƿ���
*	��    ��: ��
*	�� �� ֵ: ����ֵ1 ��ʾ���£�0��ʾδ����
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
*	�� �� ��: BSP_InitKeyHard
*	����˵��: ���ð�����Ӧ��GPIO
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void BSP_InitKeyHard(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/* ��1������GPIOʱ�� */
	RCC_AHB1PeriphClockCmd(RCC_ALL_KEY, ENABLE);

	/* ��2�����������еİ���GPIOΪ��������ģʽ(ʵ����CPU��λ���������״̬) */
	
#if		BOARD_VERSION == ARMFLY_V5
	
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN;		/* ��Ϊ����� */
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		/* ��Ϊ����ģʽ */
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;	/* �������������� */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	/* IO������ٶ� */

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

	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN;		/* ��Ϊ�����     */
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		/* ��Ϊ����ģʽ   */
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;	/* �������������� */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	/* IO������ٶ�   */	

	GPIO_InitStructure.GPIO_Pin   = KEY_ROW1_PIN | KEY_ROW2_PIN | KEY_ROW3_PIN | KEY_ROW4_PIN;
	GPIO_Init(KEY_ROW_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;		/* ��Ϊ�����     */
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		/* ��Ϊ����ģʽ   */
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;		/* ��������       */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	/* IO������ٶ�   */	
	
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
*	�� �� ��: BSP_InitKeyVar
*	����˵��: ��ʼ����������
*	��    �Σ���
*	�� �� ֵ: ��
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
*	�� �� ��: bsp_InitKey
*	����˵��: ��ʼ������. �ú����� bsp_Init() ���á�
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
int BSP_InitKey(void)
{
	BSP_InitKeyVar();		/* ��ʼ���������� */
	BSP_InitKeyHard();		/* ��ʼ������Ӳ�� */
	
	return(1);
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_PutKey
*	����˵��: ��1����ֵѹ�밴��FIFO��������������ģ��һ��������
*	��    �Σ�KeyValue: ��ֵ   
*             KeyEvent: �����¼�����
*	�� �� ֵ: 
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
		/* �˴�Ӧ���ж��Ƿ���Ҫ��ʱ�ط���ͬʱ�Ƿ���Ҫд�������־ */ 
		return -1;
	}
	
	return -2;
}

/*
*********************************************************************************************************
*	�� �� ��: BSP_DetectKey
*	����˵��: ���һ��������������״̬�����뱻�����Եĵ��á�����ѭ����������Ϊ5ms
*	��    �Σ������ṹ����ָ��
*	�� �� ֵ: ��
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
*	�� �� ��: bsp_KeyScan
*	����˵��: ɨ�����а���������������systick�ж������Եĵ���
*	��    �Σ���
*	�� �� ֵ: ��
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
