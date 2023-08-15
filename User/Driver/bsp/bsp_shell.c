/*
*********************************************************************************************************
*	                                            DoubleWires
*                           Commuication Controller For The Mining Industry
*
*                                  (c) Copyright 1994-2013  HNDZ
*                       All rights reserved.  Protected by international copyright laws.
*
*
*    File    : bsp_shell.c
*    Module  : user
*    Version : V1.0
*    History :
*   -----------------
*              Version  Date           By            Note
*              v1.0     2013-08-28     
*
*********************************************************************************************************
*/
#include "bsp.h"

//#include <string.h>
#define  MAX_COMMAND_NUM           13   // 最大的命令条数
#define  USARTTIMEOUT         6000000   /* 等待串口时间超时 */

#if RUN_MODE == DEBUG_MODE

static void UnSupportCommand(const u8 name[], u8 *argv[]);
char ReceiveCommand[SHELL_CMD_SIZE];

static CMD_STRUCT s_cmdMatch[] = 
{
	{    "test-io", 0,    UnSupportCommand},
	{   "test-led", 0,    UnSupportCommand},
	{   "test-lcd", 0,    UnSupportCommand},
    {   "test-key", 0,    UnSupportCommand},
#ifdef NAND_TEST
	{  "test-nand", 0,  GetNandTestCommand},
#else
	{  "test-nand", 0,    UnSupportCommand},
#endif    /* NAND_TEST */
	{   "test-can", 0,    UnSupportCommand},
	{   "test-485", 0,    UnSupportCommand},
	{"test-sflash", 0,    UnSupportCommand},
	{"test-otgout", 0,    UnSupportCommand}
  
};

#endif    /* RUN_MODE == DEBUG_MODE */


#if SHELL_USE_LIB == 0
static uint8_t            Shell_SBuf[SHELL_S_SIZE];
static uint8_t            Shell_RBuf[SHELL_R_SIZE];
static STR_SHELL_BUFSIGN  Shell_BufSign;

/*
*********************************************************************************************************
*	函 数 名: USART1_IRQHandler
*	功能说明:
*	形    参：
*	返 回 值:
*********************************************************************************************************
*/
void Shell_IRQHandler(void)
{
	uint8_t rdata;

	if ((SHELL_USART->SR & USART_FLAG_RXNE) != 0)		//if (USART_GetITStatus(SHELL_USART, USART_IT_RXNE) != RESET) //接收RXNE中断置位
	{
		rdata = (uint8_t)(USART_ReceiveData(SHELL_USART));
		USART_SendData(SHELL_USART, rdata);
	}

	if ((SHELL_USART->SR & USART_FLAG_TXE) != 0)		//if (USART_GetITStatus(SHELL_USART, USART_IT_TXE) != RESET) //发送TXE中断置位
	{
		STR_SHELL_BUFSIGN *bufsign = &Shell_BufSign;

		if (bufsign->length > 0)
		{
			USART_SendData(SHELL_USART, Shell_SBuf[bufsign->sp]);
			bufsign->sp++;
			bufsign->sp %= SHELL_S_SIZE;

			bufsign->length--;
		}
		else
		{
			USART_ITConfig(SHELL_USART, USART_IT_TXE, DISABLE);
		}
	}
}

static void ShellSend(uint8_t *sbuf, uint32_t len)
{

}

#endif
/*
*********************************************************************************************************
*	函 数 名: bsp_InitUart
*	功能说明: 初始化CPU的USART1串口硬件设备。未启用中断。
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
int BSP_ShellInit(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

#if SHELL_USE_LIB == 0
	NVIC_InitTypeDef  NVIC_InitStructure;
#endif

    /* 第1步： 配置GPIO                                        */

	GPIO_PIN_SHELL_CONFIG();

	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Pin   = GPIO_PIN_SHELL_TX;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIO_PORT_SHELL_TX, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Pin   = GPIO_PIN_SHELL_RX;
	GPIO_Init(GPIO_PORT_SHELL_RX, &GPIO_InitStructure);

#ifdef  DEBUG_485B
    GPIO_InitStructure.GPIO_Pin   = GPIO_PIN_SHELL_EN;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;//GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_Init(GPIO_PORT_SHELL_EN, &GPIO_InitStructure);
	RS485B_SendDisable();					      // 置为低电平，接收状态
#endif   /* DEBUG_485B */
	/* 第2步： 配置串口硬件参数                                */

	USART_InitStructure.USART_BaudRate            = SHELL_BPS;
	USART_InitStructure.USART_WordLength          = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits            = USART_StopBits_1;
	USART_InitStructure.USART_Parity              = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(SHELL_USART, &USART_InitStructure);

#if SHELL_USE_LIB == 0
	/* 第3步： 配置中断                                        */
	USART_ITConfig(SHELL_USART, USART_IT_RXNE, ENABLE);
	BSP_IntVectSet(SHELL_IRQn, Shell_IRQHadler);

	NVIC_InitStructure.NVIC_IRQChannel            = SHELL_IRQn;//USART1通道
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd         = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
#endif

	/* 第4步： 使能串口 */
	USART_Cmd(SHELL_USART, ENABLE);

/**
	CPU的小缺陷：串口配置好，如果直接Send，则第1个字节发送不出去,如下语句解决第1个字节无法正确发送出去的问题
*/
	USART_ClearFlag(SHELL_USART, USART_FLAG_TC);     /* 清发送完成标志，Transmission Complete flag */

#if SHELL_USE_LIB == 0
	Shell_BufSign.length = 0;
	Shell_BufSign.sp     = 0;
	Shell_BufSign.wp     = 0;
#endif

	return(1);
}

#if RUN_MODE == DEBUG_MODE
/**
  * @brief  从调试串口接收数据
  * @param  key: The key pressed
  * @retval 1: Correct
  *         0: Error
  */
uint8_t SerialGetChar()
{
    uint8_t ch;
    ch = getchar();

    return ch;
}

/**
  * @brief  Get Input string from the HyperTerminal
  * @param  buffP: The input string
  * @retval None
  */
void GetInputString(char *buffP)
{
    uint32_t bytes_read = 0;
    char  c = 0;

    do
    {
        c = SerialGetChar();			   // 接收一个字符

        if (c == '\r')                     // 是回车
            break;
		else if (c == '\n')
			break;

        if (c == '\b')                     /* Backspace */
        {
            if (bytes_read > 0)			   // 删除符
            {
                bytes_read--;
            }
            break;
        }
        if (bytes_read >= SHELL_CMD_SIZE)
        {
            RUN_TRACE("  \r\n");
			RUN_TRACE("Command string size overflow\r\n");
			RUN_SHELL_PROMPT();
            bytes_read = 0;
            continue;
        }
        if (c >= 0x20 && c <= 0x7E)		   // 有效字符
        {
            buffP[bytes_read++] = c;
        }
    }
    while (1);

    buffP[bytes_read] = '\0';
}

static void UnSupportCommand(const u8 name[], u8 *argv[])
{
	RUN_TRACE("  \r\nReceived %s command!\r\n", name);
	RUN_TRACE("  But current not support!\r\n");
}

/********************************************************************************************
* 函数名称：findArgs()
* 功能描述：找出各个参数函数
* 入口参数：输入命令字符串
* 出口参数：参数个数
* 使用说明：无
********************************************************************************************/
u8 findArgs(u8 *input, u8 *argv[])
{
	u8 nargs = 0;

	while (nargs < MAX_ARGS_NUM) 
	{
		/* skip any white space */
		while ((*input == ' ') || (*input == '\t')) 
		{
			++input;
		}

		if (*input == '\0') 
		{	/* end of line, no more args	*/
			argv[nargs] = NULL;
			
			return (nargs);
		}

		argv[nargs++] = input;	 /* begin of argument string	*/

		/* find end of string */
		while (*input && (*input != ' ') && (*input != '\t')) \
		{
			++input;
		}

		if (*input == '\0') 
		{	/* end of line, no more args	*/
			argv[nargs] = NULL;
			if (nargs > 1)
				RUN_TRACE("    \r\nThe args %d is %s\r\n", nargs-1, argv[nargs-1]);
			
			return (nargs);
		}

		*input++ = '\0';		/* terminate current arg	 */
		if (nargs > 1)
			RUN_TRACE("    \r\nThe args %d is %s\r\n", nargs-1, argv[nargs-1]);
	}

	/* skip any white space */
	while ((*input == ' ') || (*input == '\t')) 
	{
		++input;
	}

	if (*input != '\0') 
		RUN_TRACE("\r\n** Too many args (max. %d) **\r\n", MAX_ARGS_NUM-1);
	
	return (nargs);
}

/********************************************************************************************
* 函数名称：input_Command_Analyse()
* 功能描述：输入命令分析函数
* 入口参数：输入命令字符串
* 出口参数：是否有效
* 使用说明：无
********************************************************************************************/
uint8_t input_Command_Analyse(CMD_STRUCT *s_cmdMatch, uint8_t *buffP, uint8_t maxCmdNum)
{
	u16 bytes_read = 0;
	u8  command_strp = 0;
	u8  invalid = 0;
	u8  argc = 0;                         // 参数个数
    u8 *argv[MAX_ARGS_NUM];	              // 参数数组
	
//    if (buffP[0] == '\0')                 // 没有收到命令
//    {
//        return invalid;
//    }

	argc = findArgs(buffP, argv);
	
	if (argc == 0)
	{
        return invalid;
    }
	
    for (; command_strp < maxCmdNum; command_strp++)
	{
		invalid = 0;
        bytes_read = 0;
		//while (buffP[bytes_read])
		while (s_cmdMatch[command_strp].Command_Name[bytes_read])
		{
			if (argv[0][bytes_read] == s_cmdMatch[command_strp].Command_Name[bytes_read])
			{
				bytes_read++;
			}
			else
			{
				invalid = 1;
				break;                    // 跳出本次匹配
			}
		}

		if (!invalid)                     // 已经匹配命令了
		{
			if (buffP[bytes_read])        // 命令结尾不为\0
			{
				invalid = 1;
				continue;
			}
			break;
		}
	}

    if (invalid)
    {
		RUN_TRACE("   \r\nReceived invalid command!\r\n");
    }
    else
    {
        if (command_strp < MAX_COMMAND_NUM)
		{
			if (argc-1 > s_cmdMatch[command_strp].args_num)
			{
				invalid = 1;
				RUN_TRACE("\r\nArgs too many!\r\n");
			}
			else
				s_cmdMatch[command_strp].Cmd_Handle(s_cmdMatch[command_strp].Command_Name, argv); // 执行命令
		}
    }

    buffP[0] = '\0';
	return invalid;
}
#if 0
void ShellCmd_Task(void *p_arg)
{
    (void)p_arg;

    while (1)
    {
        RUN_TRACE("   \r\n");
		RUN_SHELL_PROMPT();

		//GetInputString(ReceiveCommand);                         // 收不到字符将阻塞
        //input_Command_Analyse(s_cmdMatch, ReceiveCommand, MAX_COMMAND_NUM); // 命令解析
    }
}
#endif

#endif   /* RUN_MODE == DEBUG_MODE */

/**
	说    明 :	实现printf和scanf函数重定向到串口1，即支持printf信息到USART1
				实现重定向，只需要添加2个函数:
				int fputc(int ch, FILE *f);
				int fgetc(FILE *f);
				对于KEIL MDK编译器，编译选项中需要在MicorLib前面打钩，否则不会有数据打印到USART1。
*/

/*
*********************************************************************************************************
*	函 数 名: fputc
*	功能说明: 重定义putc函数，这样可以使用printf函数从串口1打印输出
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
//#define PRINTF_MODE_ITM
int fputc(int ch, FILE *f)
{
	__IO uint32_t timeOut = 0;
#ifdef  DEBUG_485B
	while (USART_GetFlagStatus(SHELL_USART, USART_FLAG_RXNE) != RESET) // 等待接收结束
    {
		timeOut++;
		if (timeOut > USARTTIMEOUT)
		{
			timeOut = 0;
			break;
		}
    }
	RS485B_SendEnable();                      // 使能485-2发送
#endif   /* DEBUG_485B */

#ifdef PRINTF_MODE_ITM
	ITM_SendChar(ch);
#else
	/* 写一个字节到USART1 */
	USART_SendData(SHELL_USART, (uint8_t) ch);

	/* 等待发送结束 */
	while (USART_GetFlagStatus(SHELL_USART, USART_FLAG_TC) == RESET)
	{
		timeOut++;
		if (timeOut > USARTTIMEOUT)
		{
			break;
		}
	}
#endif

#ifdef  DEBUG_485B
	RS485B_SendDisable();                      // 失能485-2发送
#endif   /* DEBUG_485B */
	return ch;
}

/*
*********************************************************************************************************
*	函 数 名: fgetc
*	功能说明: 重定义getc函数，这样可以使用scanff函数从串口1输入数据
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
int fgetc(FILE *f)
{
	/* 等待串口1输入数据 */
	while (USART_GetFlagStatus(SHELL_USART, USART_FLAG_RXNE) == RESET)
	{
		;
	}

	return (int)USART_ReceiveData(SHELL_USART);
}

void time_trace(void)
{
	RTC_TimeData  rtcValue;
	
	Get_Data_Time(&rtcValue);
	printf("TIME: %d (M) %d (S)",rtcValue.sys_time.RTC_Minutes,rtcValue.sys_time.RTC_Seconds);
}
