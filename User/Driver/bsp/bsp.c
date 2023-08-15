/*
*********************************************************************************************************
*	                                            DoubleWires
*                           Commuication Controller For The Mining Industry
*
*                                  (c) Copyright 1994-2013  HNDZ 
*                       All rights reserved.  Protected by international copyright laws.
*
*
*    File    : bsp.c
*    Module  : user
*    Version : V1.0
*    History :
*   -----------------
*              Version  Date           By            Note
*              v1.0     
*
*********************************************************************************************************
*/

#define   BSP_MODULE

#include  <includes.h>
#include  <bsp.h>
#include  "usart.h"
#include  "timer.h"

/*
*********************************************************************************************************
*                                            FUNCTIONS DECLARE
*********************************************************************************************************
*/
void NVIC_Configuration(void);

/*
*********************************************************************************************************
* Description : Initialize the Board Support Package (BSP).
* Argument(s) : none.
* Return(s)   : none.
* Caller(s)   : Application.
* Note(s)     : (1) This function SHOULD be called before any other BSP function is called.
*               (2) CPU instruction / data tracing requires the use of the following pins :
*                   (a) (1) Aysynchronous     :  PB[3]
*                       (2) Synchronous 1-bit :  PE[3:2]
*                       (3) Synchronous 2-bit :  PE[4:2]
*                       (4) Synchronous 4-bit :  PE[6:2]
*
*                   (c) The application may wish to adjust the trace bus width depending on I/O
*                       requirements.
*********************************************************************************************************
*/
void BSP_Init(void)
{
//	int a = 0;
//	uint16_t  i= 0;
//	uint8_t j=0;
//	uint8_t buf[1000] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24};
//	uint8_t buf1[1000] = {0};
	int err;
	/* ʹ��CRC ��Ϊʹ��STemWinǰ����Ҫʹ�� */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_CRC, ENABLE);
	NVIC_Configuration();                                  /* �ж����ȼ��������� */
	BSP_IntInit();
	SystemCoreClockUpdate();	/* ����PLL���ø���ϵͳʱ��Ƶ�ʱ��� SystemCoreClock */
	bsp_InitDWT();

	UsartRecvSemCreate();
	UartInit();
	usartHeartTimerInit();

	
	TIM3_Int_Init(4999,17999);//100Khz��???��y?��?��????��y��?9?a0.1ms  
	bsp_DelayMS(200);
	bsp_InitExtSDRAM();
//	bsp_TestExtSDRAM1();
	LCD_InitHard();
	
	bsp_InitI2C();
	ee_CheckOk();
	
	
//	while(1)
//	{
//		i++;
//		i = i%999;
//		buf[i] = j++;
//		bsp_DelayMS(10);
//		ee_WriteBytes(buf,0,1000);
//		bsp_DelayMS(10);
//		ee_ReadBytes(buf1,0,1000);
//		if(memcmp(buf,buf1,1000) != 0)
//		{
//			a++;
//		}
//	}
	
//	err = BSP_ShellInit(); 	                               /* ��ʼ������ */
//	if (err > 0)
//	{
//		RUN_TRACE("\r\n\r\n\r\n");
//		RUN_TRACE("Embedded Framework Shell [%s]\r\n\r\n",SHELL_VERSION);
//		RUN_TRACE("OS     : %s \r\n",SHELL_OS);
//		RUN_TRACE("CPU    : %s \r\n",SHELL_CPU);
//		RUN_TRACE("Device : %s \r\n",SHELL_DEVICE);
//		RUN_TRACE("Board  : %s \r\n",SHELL_BOARD);	
//		RUN_TRACE("\r\n");
//		RUN_TRACE("System is booting....\r\n");
//		RUN_TRACE("Shell initialization ... [PASSED]\r\n");
//	}
	
// 	err = BSP_LedInit();
// 	if (err > 0)
// 	{
// 		RUN_TRACE_INFO("LED initialization ... [PASSED]\r\n");
// 	}
// 	else
// 	{
// 		RUN_TRACE_ERR(LED_ERROR,"LED initialization ... [FAILED]\r\n");
// 	}
// 	
// 	err = BSP_InitKey();
// 	if (err > 0)
// 	{
// 		RUN_TRACE_INFO("KEY initialization ... [PASSED]\r\n");
// 	}
// 	else
// 	{
// 		RUN_TRACE_ERR(KEY_ERROR,"KEY initialization ... [FAILED]\r\n");
// 	}
	
	err = BSP_BCMInit();
    if (err > 0)
	{
		RUN_TRACE("BCM initialization ... [PASSED]\r\n");
	}
	else
	{
		RUN_TRACE_ERR(FLASH_ERROR, "BCM initialization ... [FAILED]\r\n");
	}
	

//     err = bsp_NandFlashinit();
//     if (err > 0)
// 	{
// 		RUN_TRACE_INFO("NAND initialization ... [PASSED]\r\n");
// 	}
// 	else
// 	{
// 		RUN_TRACE_ERR(FLASH_ERROR, "NAND initialization ... [FAILED]\r\n");
// 	}

//     err = BSP_InitFM24CL64();
//     if (err > 0)
// 	{
// 		RUN_TRACE_INFO("FM24CL64 initialization ... [PASSED]\r\n");
// 	}
// 	else
// 	{
// 		RUN_TRACE_ERR(FM24_ERROR, "FM24CL64 initialization ... [FAILED]\r\n");
// 	}
// 	err = BSP_RS485A_Init();//RS485�ײ��ʼ��
// 	if (err > 0)
// 	{
// 		RUN_TRACE_INFO("RS485 initialization ... [PASSED]\r\n");
// 	}
// 	else
// 	{
// 		RUN_TRACE_ERR(RS485_ERROR, "RS485 initialization ... [FAILED]\r\n");
// 	}
	/**2014.07.14�����ӵ�Nor Flash��ʼ��**/
// 	err = BSP_W25QXXInit();
// 	if (err > 0)
// 	{
// 		RUN_TRACE_INFO("W25Q32 initialization ... [PASSED]\r\n");
// 	}
// 	else
// 	{
// 		RUN_TRACE_ERR(RS485_ERROR, "W25Q32 initialization ... [FAILED]\r\n");
// 	}
// 	
// 	//bsp_InitTimer();	                                   /* ��ʼ��ϵͳ�δ�ʱ�� */

	MCP3204_Init();
	ADC_Configuration();
	BSP_InitRtc();
	JTBS_Init();
	LedInit();
	Beep_Init();
	
	RUN_TRACE("System boot success.\r\n");
}

/*
*********************************************************************************************************
* Description : NVIC_Configuration
* Argument(s) : none.
* Return(s)   : none.
* Caller(s)   : BSP_Init.
* Note(s)     : �ж����ȼ����ã������Ȱ����ȼ��������ã�Ȼ���ھ����ģ������ʵ�����ȼ����� 
*********************************************************************************************************
*/
void NVIC_Configuration(void)
{				
	//NVIC_InitTypeDef NVIC_InitStructure;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);        /* ����NVIC���ȼ�����ΪGroup2��0-3��ռʽ���ȼ���0-3����Ӧʽ���ȼ� */
	
	//NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;//USART1ͨ��
	//NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	//NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	//NVIC_Init(&NVIC_InitStructure);
}

/*******************************************************************************
* Function Name  : Get_MCU_ID
* Description    : ��ȡMCU��ΨһID
* Input          : pMcuID -- ID�����ָ��
* Output         : McuID
* Return         : ȫ��UID
* Attention		   : STM32 96λ(12�ֽ�)�Ĳ�ƷΨһ��ݱ�ʶ ����ַ��0x1FFF F7E8
************************************************************************************/           
u32 Get_MCU_ID(uint8_t* pMcuID)
{
	u32 CpuID[3] = {0};
		
	//��ȡCPUΨһID
	#if 0//STM32F1ϵ��
	CpuID[0] =*(vu32*)(0x1ffff7e8); //��ȫ�֣�32λ����ȡ
	CpuID[1] =*(vu32*)(0x1ffff7ec);
	CpuID[2] =*(vu32*)(0x1ffff7f0);
	#endif
	
	#if 1//STM32F4ϵ��
	CpuID[0]=*(vu32*)(0x1fff7a10);
	CpuID[1]=*(vu32*)(0x1fff7a14);
	CpuID[2]=*(vu32*)(0x1fff7a18);
	#endif
	
	//���ֽڣ�8λ����ȡ
	pMcuID[0] = (uint8_t)(CpuID[0] & 0x000000FF);
	pMcuID[1] = (uint8_t)((CpuID[0] & 0xFF00) >>8);
	pMcuID[2] = (uint8_t)((CpuID[0] & 0xFF0000) >>16);
	pMcuID[3] = (uint8_t)((CpuID[0] & 0xFF000000) >>24);
	
	pMcuID[4] = (uint8_t)(CpuID[1] & 0xFF);
	pMcuID[5] = (uint8_t)((CpuID[1] & 0xFF00) >>8);
	pMcuID[6] = (uint8_t)((CpuID[1] & 0xFF0000) >>16);
	pMcuID[7] = (uint8_t)((CpuID[1] & 0xFF000000) >>24);
	
	pMcuID[8] = (uint8_t)(CpuID[2] & 0xFF);
	pMcuID[9] = (uint8_t)((CpuID[2] & 0xFF00) >>8);
	pMcuID[10] = (uint8_t)((CpuID[2] & 0xFF0000) >>16);
	pMcuID[11] = (uint8_t)((CpuID[2] & 0xFF000000) >>24);

	return (CpuID[0]>>1)+(CpuID[1]>>2)+(CpuID[2]>>3);
}

/***********************************************************************************************
* Function Name  : CreateSysMAC
* Description	 : ��ȡMCU��MAC��ַ
* Input 		 : None
* Output		 : MAC
* Return		 : None
***********************************************************************************************/
void CreateSysMAC(uint8_t* pMacBuf)
{
	u32 uiMcuId = 0;
	uint8_t McuID[15] = {0};
	int i = 0;

	uiMcuId = Get_MCU_ID(McuID);

	for(i=0; i<12; i++) //��ȡMcuID[12]
	{
		McuID[12] += McuID[i];	
	}
	for(i=0; i<12; i++)	//��ȡMcuID[13]
	{
		McuID[13] ^= McuID[i];	
	}

	pMacBuf[0] = (uint8_t)(uiMcuId & 0xF0);
	pMacBuf[1] = (uint8_t)((uiMcuId & 0xFF00) >>8);
	pMacBuf[2] = (uint8_t)((uiMcuId & 0xFF0000) >>16);
	pMacBuf[3] = (uint8_t)((uiMcuId & 0xFF000000) >>24);
	pMacBuf[4] = McuID[12];
	pMacBuf[5] = McuID[13];
}

