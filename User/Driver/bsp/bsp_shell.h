/*
*********************************************************************************************************
*	                                            DoubleWires
*                           Commuication Controller For The Mining Industry
*
*                                  (c) Copyright 1994-2013  HNDZ
*                       All rights reserved.  Protected by international copyright laws.
*
*
*    File    : bsp_shell.h
*    Module  : user
*    Version : V1.0
*    History :
*   -----------------
*              Version  Date           By            Note
*              v1.0     2013-08-28     fanhaichun
*
*********************************************************************************************************
*/

#ifndef __BSP_SHELL_H__
#define __BSP_SHELL_H__

#include "includes.h"
#define SHELL_USE_LIB					1		/* 0- 支持中断输入  1-不支持中断输入 */

#define SHELL_BPS					    9600//115200
#define SHELL_S_SIZE					128
#define SHELL_R_SIZE					32

//#if BOARD_VERSION == ARMFLY_V5

  #define SHELL_USART					USART2
  #define SHELL_IRQn					USART2_IRQn
  #define GPIO_PORT_SHELL_TX  			GPIOC
  #define GPIO_PORT_SHELL_RX  			GPIOC
  #define GPIO_PIN_SHELL_TX				GPIO_Pin_6
  #define GPIO_PIN_SHELL_RX				GPIO_Pin_7
  #define GPIO_PIN_SHELL_CONFIG()		{\
										RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);\
										RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6, ENABLE);\
										GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_USART6);\
										GPIO_PinAFConfig(GPIOC, GPIO_PinSource7, GPIO_AF_USART6);\
										}

// #elif BOARD_VERSION == TK100_V1
//     #define DEBUG_485B                      // 使用485B调试

//     #ifdef  DEBUG_485B
//       #define SHELL_USART				UART5
//       #define GPIO_PORT_SHELL_TX  		GPIOC
//       #define GPIO_PORT_SHELL_RX  		GPIOD
//       #define GPIO_PORT_SHELL_EN  		GPIOD
//       #define GPIO_PIN_SHELL_TX			GPIO_Pin_12
//       #define GPIO_PIN_SHELL_RX			GPIO_Pin_2
//       #define GPIO_PIN_SHELL_EN			GPIO_Pin_6
//       #define GPIO_PIN_SHELL_CONFIG()	{\
// 										RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);\
// 	                                    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);\
// 										RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, ENABLE);\
// 										GPIO_PinAFConfig(GPIOC, GPIO_PinSource12, GPIO_AF_UART5);\
// 										GPIO_PinAFConfig(GPIOD, GPIO_PinSource2, GPIO_AF_UART5);\
// 										}

//       #define RS485B_SendEnable()       GPIOD->BSRRH = GPIO_Pin_6         //modify by bting   2014.2.19(因硬件变更新板调试，临时改)
//       #define RS485B_SendDisable()      GPIOD->BSRRL = GPIO_Pin_6
//     #else
//       #define SHELL_USART				USART4
//       #define GPIO_PORT_SHELL_TX  		GPIOC
//       #define GPIO_PORT_SHELL_RX  		GPIOC
//       #define GPIO_PIN_SHELL_TX			GPIO_Pin_10
//       #define GPIO_PIN_SHELL_RX			GPIO_Pin_11
//       #define GPIO_PIN_SHELL_CONFIG()	{\
// 										RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);\
// 										RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);\
// 										GPIO_PinAFConfig(GPIOC, GPIO_PinSource10, GPIO_AF_UART4);\
// 										GPIO_PinAFConfig(GPIOC, GPIO_PinSource11, GPIO_AF_UART4);\
// 										}
//     #endif   /* DEBUG_485B */
// #endif   /* ARMFLY_V5 */

#if RUN_MODE == DEBUG_MODE


#define RUN_TRACE						//printf
#define printk		                    //printf								
#define RUN_TRACE_INFO( ...)		//printf("INFO  :" __VA_ARGS__ )
#define RUN_TRACE_ERR(ID, ...)			//printf("ERROR "#ID" :" __VA_ARGS__ )
//#define TIME_TRACE                      {\
//											RTC_TimeData  rtcValue;\
//											Get_Data_Time(&rtcValue);\
//											printf("TIME: %d (M) %d (S)",rtcValue.sys_time.RTC_Minutes,rtcValue.sys_time.RTC_Seconds);\
//										}
#define TIME_TRACE                      //time_trace();

#define RUN_SHELL_PROMPT()              //printf("double wires:>")   // 命令提示符
#define SHELL_CMD_SIZE                  300                  // 支持的命令长度
#define MAX_ARGS_NUM                     3                  // 最大参数个数
										
void ShellCmd_Task(void *p_arg);
void GetInputString(char *buffP);
void time_trace(void);
#else

#define RUN_TRACE( ...)
#define RUN_TRACE_INFO( ...)
#define RUN_TRACE_ERR(ID, ...)

#endif    /* RUN_MODE == DEBUG_MODE */

typedef struct{
	uint32_t		length;
	uint32_t        sp;
	uint32_t        wp;
}STR_SHELL_BUFSIGN;

#if RUN_MODE == DEBUG_MODE
typedef struct{
	const u8 Command_Name[SHELL_CMD_SIZE];
	const u8 args_num;
	void (*Cmd_Handle)(const u8 name[], u8 *argv[]);
}CMD_STRUCT;

uint8_t input_Command_Analyse(CMD_STRUCT *s_cmdMatch, uint8_t *buffP, uint8_t maxCmdNum);
#endif    /* RUN_MODE == DEBUG_MODE */

#endif   /* __BSP_SHELL_H__ */

