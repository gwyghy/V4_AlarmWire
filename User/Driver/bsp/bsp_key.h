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
 * ���ݰ忨���Ͷ��尴��GPIO PIN
 ***************************************************************************************************/
#define RCC_ALL_KEY 			(RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOF | RCC_AHB1Periph_GPIOH | RCC_AHB1Periph_GPIOI | RCC_AHB1Periph_GPIOG)
#if		BOARD_VERSION == ARMFLY_V5

	/*
		STM32-V5 �������߷��䣺
			K1 ��      : PI8   (�͵�ƽ��ʾ����)
			K2 ��      : PC13  (�͵�ƽ��ʾ����)
			K3 ��      : PI11  (�͵�ƽ��ʾ����)
			ҡ��UP��   : PH2   (�͵�ƽ��ʾ����)
			ҡ��DOWN�� : PH3   (�͵�ƽ��ʾ����)
			ҡ��LEFT�� : PF11  (�͵�ƽ��ʾ����)
			ҡ��RIGHT��: PG7   (�͵�ƽ��ʾ����)
			ҡ��OK��   : PH15  (�͵�ƽ��ʾ����)
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

	/* ����Ӧ�ó���Ĺ��������������� */
	#define KEY_DOWN_K1				KEY_1_DOWN
	#define KEY_UP_K1				KEY_1_UP
	#define KEY_LONG_K1				KEY_1_LONG

	#define KEY_DOWN_K2				KEY_2_DOWN
	#define KEY_UP_K2				KEY_2_UP
	#define KEY_LONG_K2				KEY_2_LONG

	#define KEY_DOWN_K3				KEY_3_DOWN
	#define KEY_UP_K3				KEY_3_UP
	#define KEY_LONG_K3				KEY_3_LONG

	#define JOY_DOWN_U				KEY_4_DOWN		/* �� */
	#define JOY_UP_U				KEY_4_UP
	#define JOY_LONG_U				KEY_4_LONG

	#define JOY_DOWN_D				KEY_5_DOWN		/* �� */
	#define JOY_UP_D				KEY_5_UP
	#define JOY_LONG_D				KEY_5_LONG

	#define JOY_DOWN_L				KEY_6_DOWN		/* �� */
	#define JOY_UP_L				KEY_6_UP
	#define JOY_LONG_L				KEY_6_LONG

	#define JOY_DOWN_R				KEY_7_DOWN		/* �� */
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
 * �����µ��������Ͱ���ID����Ҫע�ⰴ��ID��0��ʼ���ڳ�����������жϣ�Ӧ����ע�����
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
 * �����µ��������Ͱ����¼�
 ***************************************************************************************************/
typedef enum
{
	tKeyDown = 1,
	tKeyUp,
	tKeyDoubleClick,
	tKeyLongDown
}KeyEvent_t;

/****************************************************************************************************
 * �������ṹ�壬ÿ��������Ӧһ�������Ľṹ�壬�ڰ�����������в��԰���˫���ͳ������м��
 ***************************************************************************************************/
#define KEY_FILTER_TIME             2 

#if 0
	#define KEY_LONG_TIME           100		/* ��λms�� ����1�룬��Ϊ�����¼�                      */
	#define KEY_TYPE_DOUBLE	        0x0001  /* ���˫���¼�                                        */
	#define KEY_TYPE_LONG           0x0002  /* ��ⳤ���¼�                                        */
#endif

typedef struct
{
	uint8_t   (*IsKeyDownFunc)(void);       /* �������µ��жϺ���,1��ʾ����                        */
	uint16_t  Count;					    /* �˲���������                                        */
	uint8_t   State;					    /* ������ǰ״̬�����»��ǵ��𣩣�1-����  0-����        */
	
#if 0
	uint16_t  LongCount;				    /* ����������                                          */
	uint16_t  LongTime;				        /* �������³���ʱ��, 0��ʾ����ⳤ��                   */
	uint8_t   Type;                         /* �Ƿ���˫���������¼�,Ĭ�Ͻ���ⵥ���¼�           */
	uint8_t   RepeatSpeed;			        /* ������������                                        */
	uint8_t   RepeatCount;			        /* ��������������                                      */
#endif
}KEY_T;

/****************************************************************************************************
 * ���ڰ���FIFO����������������¼����������¼��������ڴ˲����ð���FIFO
 * �ڳ�������Ҫע��ĵط��ǣ������¼����������һ��Ҫ�Ȱ������������
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
	STR_KEY Buf[KEY_FIFO_SIZE];		        /* ��ֵ������                                          */
	uint8_t Read;					        /* ��������ָ��                                        */
	uint8_t Write;					        /* ������дָ��                                        */
}KEY_FIFO_T;
#endif

/* ���ⲿ���õĺ������� */
int  BSP_InitKey	(void);
void BSP_KeyScan	(void);
int BSP_PutKey(uint32_t KeyValue, KeyEvent_t KeyEvent);
#endif

