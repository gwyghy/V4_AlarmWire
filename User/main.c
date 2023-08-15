/*
*********************************************************************************************************
*	                                            DoubleWires
*                           Commuication Controller For The Mining Industry
*
*                                  (c) Copyright 1994-2013  HNDZ 
*                       All rights reserved.  Protected by international copyright laws.
*
*
*    File    : main.c
*    Module  : user
*    Version : V1.0
*    History :
*   -----------------
*              Version  Date           By            Note
*              v1.0          
*
*********************************************************************************************************
*/
#define __MAIN_C__

#include "includes.h"

#include "sys_default.h"
#include "sys_data_type.h"
#include "sys_msg_def.h"
#include "SysFunc_API.h"
#include "devmng.h"
#include "sysParamAnalysis.h"

#include "stm32f4x7_eth.h"
#include "stm32f4x7_eth_bsp.h"
#include "netconf.h"
#include "udp_app_proc.h"
#include "memory_pool.h"
#include "memory.h"

#include "Can_Pro.h"
#include "V2_Can_Pro.h"
#include "FileLoad.h"
#include "Util_Timer.h"
#include "usartapp.h"
#include "fontupd.h"
#include "DoFatfsFiles.h"
#include "HomeTask.h"
#include "GUI_api.h"
#include "V4_Wireless.h"
/*
*********************************************************************************************************
*                                       LOCAL GLOBAL VARIABLES
*********************************************************************************************************
*/
static OS_STK		APPMng_TaskStack    [ APPMNG_TASK_STK_SIZE    ];

#if (LOGIC_SYSFUNC_EN == 1)
static OS_STK       SysFuncMng_TaskStack[ SYSFUNCMNG_TASK_STK_SIZE];
static OS_STK       SensorMng_TaskStack [ SENSORMNG_TASK_STK_SIZE ];
static OS_STK       SysCtrlMng_TaskStack[ SYSCTRLMNG_TASK_STK_SIZE ];
#endif

#if ((RUN_MODE == DEBUG_MODE)&&(SERVER_NETWORK_BCM_EN==1))
  static OS_STK     Shell_TaskStack     [SHELL_TASK_STK_SIZE      ];
#endif   

static OS_STK     GUI_TaskStack     [APP_TASK_GUI_STK_SIZE      ];

static OS_STK     usart_send_task_stk        [TASK_STK_SIZE_256      ];
static OS_STK     usart_receive_task_stk     [TASK_STK_SIZE_512      ];

#if ((SERVER_NETWORK_UDP_EN ==1)||(SERVER_NETWOEK_IPSCANER_EN==1))
static OS_STK       LwipMng_TaskStack    [ LWIPMNG_TASK_STK_SIZE     ];
static OS_STK       LwipProcess_TaskStack[ LWIPPROCESS_TASK_STK_SIZE ];
#endif

#if (SERVER_NETWORK_UDP_EN ==1)
static OS_STK       UdpMng_TaskStack     [ UDPMNG_TASK_STK_SIZE ];
static OS_STK       UdpSend_TaskStack    [ UDPSEND_TASK_STK_SIZE ];
static OS_STK       UdpCycle_TaskStack   [ UDPCYCLE_TASK_STK_SIZE ];
static OS_STK       UdpProc_TaskStack    [ UDPPROC_TASK_STK_SIZE ];
#endif

#if (SERVER_DEVICEMNG_EN ==1)
static OS_STK       DeviceMng_TaskStack  [ DEVICEMNG_TASK_STK_SIZE ];
#endif

static OS_STK       Util_Timer_TaskStack [ UTILTIMER_TASK_STK_SIZE  ];
static OS_STK       CanMng_TaskStack     [ CANMNG_TASK_STK_SIZE     ];
static OS_STK       FileLoad_TaskStack   [ FILELOAD_TASK_STK_SIZE   ];
static OS_STK       Fstfs_Save_File_TaskStack [FATFS_SAVE_FILE_STK_SIZE];

static OS_STK       UsartMng_TaskStack     [ USARTMNG_TASK_STK_SIZE     ];

static uint16_t     WDTEnableSign;
static uint8_t      WDTCounter[WDT_NUM];
/*
*********************************************************************************************************
*                                         FUNCTION PROTOTYPES
*********************************************************************************************************
*/
extern void  SysFuncMngTask(u16 delay);
extern void  DeviceMng_Task(void *);
extern void  SysCtrlMngTask(void);
extern uint32_t   PhyLinkStatus;

extern uint8_t gLogMark;



#define GUI_LWIP_DELAY_TIME   250

void BootPrintf(const char * s)
{

}

/**********************************************************************************************************
*                                          WDT manage function
*
* @Description : WDT manage function
* @Arguments   : none 
* @Returns     : none
**********************************************************************************************************/
void WDTEnable(void)
{
    uint32_t i;
    
    for(i=0;i<WDT_NUM;i++)
    {
        WDTCounter[i] = WDT_NO_USE;
    }
    
	IWDG_WriteAccessCmd(0x5555);
	IWDG_SetPrescaler(IWDG_Prescaler_128);
	IWDG_SetReload(0xFFF);
	IWDG_Enable();
	IWDG_ReloadCounter();
	WDTEnableSign = 1;
}
void WDTEnReload(void)
{
	WDTEnableSign = 1;
	IWDG_ReloadCounter();
}
void WDTNoReload(void)
{
	WDTEnableSign = 0;
}
void WDTReload(uint8_t wdt)
{
    uint32_t i;
    
    if (wdt > WDT_NUM) return;
    
	if (WDTEnableSign==1)
	{
        if (wdt == 0)
        {
            for (i=0;i<WDT_NUM;i++)
            {
                if (WDTCounter[i] != WDT_NO_USE)
                {
                    if (WDTCounter[i]++ > WDT_RST_VALUE)
                    {
                        return;
                    }
                }
            }
            IWDG_ReloadCounter();
        }
        else
        {
            WDTCounter[wdt] = 0;
        }
	}
}
/*
*********************************************************************************************************
*                                          SysFunc manage TASK
*
* Description : 
* Arguments   : p_arg 
* Returns     : none
* Notes       : (void) p_arg is used to prevent a compiler warning because 'p_arg' is not
*                used.  The compiler should not generate any code for this statement.
*********************************************************************************************************
*/
#if (LOGIC_SYSFUNC_EN == 1)
static  void  SysFuncMng_Task(void *p_arg)
{
	uint8_t err;	
	u16     delay=20;
	
	(void)p_arg;
	
	FuncInit();
	err =PLATFORM_OS_TaskCreate((void (*)(void *)) SensorModuleMngTask,
						 (PLATFORM_OS_STK * ) SensorMng_TaskStack,
						 SENSORMNG_TASK_PRIO,
						 SENSORMNG_TASK_STK_SIZE,
						 "sensor Mng Stack Task");
	
	err =PLATFORM_OS_TaskCreate((void (*)(void *)) SysCtrlMngTask,
						 (PLATFORM_OS_STK * ) SysCtrlMng_TaskStack,
						 SYSCTRLMNG_TASK_PRIO,
						 SYSCTRLMNG_TASK_STK_SIZE,
						 "Sys ctrl Mng Stack Task");
	PLATFORM_OS_TimeDly(GUI_LWIP_DELAY_TIME);
	
	PLATFORM_OS_TimeDly(getSelfAddr()*5);  //jhy    Reduce concurrency 20220628
	
	while (1) 
	{
		SysFuncMngTask(delay);
		PLATFORM_OS_TimeDly(delay);
	}
}
#endif
/*********************************************************************************************************
*                                          Lwip Task
*
* @Description : LwipProcess_Task
* @Arguments   : none
* @Returns     : none
**********************************************************************************************************/
#if ((SERVER_NETWORK_UDP_EN ==1)||(SERVER_NETWOEK_IPSCANER_EN==1))
extern void    relow_level_init(uint8_t init_sign);
extern void    reETH_BSP_Config(void);


void LwipProcess_Trigger(void)
{
	PLATFORM_OS_SemPost(&EthPktSem);
}
static  void  LwipProcess_Task(void *p_arg)
{
	uint8_t err;
	uint32_t timeout;
	
	(void)p_arg;
	
	PLATFORM_OS_TimeDly(100);
	
	while(1)
	{
		timeout = (get_phylinkstatus() == 0) ? 0 : timeout;
		if(timeout != 0)
		{
			timeout = timeout/3 + 1;
		}
		err = PLATFORM_OS_SemWait(&EthPktSem, timeout*1000);
		if (err == DEF_OK)
		{
			LwIP_Pkt_Handle();
		}
		else if(err == DEF_TIMEOUT)
		{
			if(EthStatus_Get(ETH_LINK_FLAG))
			{
				Set_LwipTimeover(1);          //lwip 超时
				set_phy_linkstatus(0);       //清除lwip link标志
			}						
		}
		
		//UpdateIPConfig();
	}
}
#endif
/*********************************************************************************************************
*                                        UdpMng_Task
*
* @Description : UdpMng_Task
* @Arguments   : none
* @Returns     : none
**********************************************************************************************************/
#if (SERVER_NETWORK_UDP_EN ==1)
static  void   UdpSend_Task(void *p_arg)
{
	 uint8_t err;
	(void)p_arg;
	
	while(1)
	{
 		err = PLATFORM_OS_SemWait(&UDPPktSendSem, 0);
 		if(err == DEF_OK)         //进行数据处理
 		{
			UdpLinkLayerSendProcess();
 		}
	}
}

extern void bttest_eth(void);
extern void reinit_551_br(void);
extern void reinit_port4(void);

u32 netRuntimer = 0;
u8 netRuntimeflag = 0;
u16 netCheckTimer = 11000;
u8 reinitport4flag = 0;
u16 reinitport4timer = 0;
void reInitNetProc(u8 delay)
{
	if(netRuntimeflag == 1)
		netRuntimer	+= delay;
	if(reinitport4flag)
		reinitport4timer += delay;
	if(netRuntimer>= netCheckTimer)
	{
		netRuntimer = 0;
		if(SysParam.globalParam.SupportParam.MaxSupportID != SysParam.globalParam.SupportParam.MinSupportID &&\
			SysParam.globalParam.SupportParam.ServerEnable == EnableType_ENABLE)
		{
			reinitport4flag = 1;
			WriteLog_API(0,LOG_TYPE_SYSTEM_ALARM,LOG_STATUS_NET_REINIT,0,0);
			reinit_551_br();
			bttest_eth();
		}
	}
	if(reinitport4timer>2000)
	{
		reinitport4flag = 0;
		reinitport4timer = 0;
		reinit_port4();
	}
}



static  void   UdpCycle_Task(void *p_arg)
{
	u16 delay=10;
	(void)p_arg;
	
	if((RCC->CSR &0xF0000000) != 0)
		netCheckTimer = 11000;
	else
		netCheckTimer = 20000;
	
	netRuntimeflag = 1;
	
	while(1)
	{
		PLATFORM_OS_TimeDly(delay);
		UdpLinkLayerCycleProcess(delay);
		reInitNetProc(delay);
	}
}


void UdpSendMsg_map(void)
{
	u8 i,j = 4;
	i = getSelfAddr();
	u8 temp[10] = {0};
	
	while(j>0)
	{
		j--;
		i--;
		if(i>= SysParam.globalParam.SupportParam.MaxSupportID)
			break;
    UdpAppProcCtrl(AppProcCtrl_arpNetMaintain_request,temp,4,i);
		OSTimeDly(100);
	}
}


u8 commNum = 2;
void UdpSendHeartMsg_map(void)
{
	u8 i;
	i = getSelfAddr();
	u8 temp[10] = {0};
	if(i == SysParam.globalParam.SupportParam.MinSupportID)
		return;
	if(commNum == 7)
	{
		commNum = 1;
		return;
	}
	if(commNum>=i)
	{
		commNum = 1;
		return;
	}
	if((i-commNum)<SysParam.globalParam.SupportParam.MinSupportID)
	{
		commNum = 1;
		return;
	}
	UdpAppProcCtrl(AppProcCtrl_arpNetMaintain_request,temp,4,i-commNum);
	commNum++;
}

static void UdpProc_Task(void *p_arg)
{
	u16 delay=2000;
	u8 temp[2];
	u8 commTimes = 0;
	(void)p_arg;
	
	udpAppProcInit();
	setUdpLinkParam(SysNetParam.NetParam_IPAddress[0], SysNetParam.NetParam_IPAddress[1], SysNetParam.NetParam_IPAddress[2], SysNetParam.NetParam_IPAddress[3],SysNetParam.deviceType);
	
	while(1)
	{
		PLATFORM_OS_TimeDly(delay);
		
		if(SysState.globalState.slaveMode !=SYSMODE_CONFIG)
		{
			UdpAppCommCheckFunc(delay);
			UdpAppProcTimerProcess(delay);
		}
		if(commTimes<6)
			commTimes++;
		if(commTimes == 5)
			UdpSendMsg_map();
		if(commTimes >=6)
			UdpSendHeartMsg_map();
	}
}

static  void   UdpMng_Task(void *p_arg)
{
	uint8_t err;

	(void)p_arg;

	/*UDP服务器初始化**/
	UdpLinkLayerInit();	
	
	err =PLATFORM_OS_TaskCreate((void (*)(void *)) UdpSend_Task,
						 (PLATFORM_OS_STK * ) UdpSend_TaskStack,
						 UDPSEND_TASK_PRIO,
						 UDPSEND_TASK_STK_SIZE,
						 "udp send Stack Task");
	
	err =PLATFORM_OS_TaskCreate((void (*)(void *)) UdpCycle_Task,
						 (PLATFORM_OS_STK * ) UdpCycle_TaskStack,
						 UDPCYCLE_TASK_PRIO,
						 UDPCYCLE_TASK_STK_SIZE,
						 "udp cycle Stack Task");
	
	err =PLATFORM_OS_TaskCreate((void (*)(void *)) UdpProc_Task,
						 (PLATFORM_OS_STK * ) UdpProc_TaskStack,
						 UDPPROC_TASK_PRIO,
						 UDPPROC_TASK_STK_SIZE,
						 "udp proc Stack Task");

	PLATFORM_OS_TimeDly(1000); /* 在上电1秒内不响应UDP任务 */	

	while(1)
	{
 		err = PLATFORM_OS_SemWait(&UDPPktRecvSem, 0);
 		if(err == DEF_OK)         //进行数据处理
 		{
			UdpLinkLayerProcess();
 		}
	}

}

#endif

/*********************************************************************************************************
*                                        LwipMng_Task
*
* @Description : LwipMng_Task
* @Arguments   : none
* @Returns     : none
**********************************************************************************************************/
#if ((SERVER_NETWORK_UDP_EN ==1)||(SERVER_NETWOEK_IPSCANER_EN==1))

#define TCPIP_PERIOD	251
extern struct tcp_pcb *tcp_active_pcbs;

static  void  LwipMng_Task(void *p_arg)
{
	uint8_t err;

	(void)p_arg;
	


	PLATFORM_OS_TimeDly(GUI_LWIP_DELAY_TIME);	
	/* configure ethernet (GPIOs, clocks, MAC, DMA) */
	ETH_BSP_Config();

	/* Initilaize the LwIP stack */
	LwIP_Init();
	
	#if (SERVER_NETWOEK_IPSCANER_EN==1)
	ipscanserver_init();
	#endif
	
	err =PLATFORM_OS_TaskCreate((void (*)(void *)) LwipProcess_Task,
                             (PLATFORM_OS_STK * ) LwipProcess_TaskStack,
                             LWIPPROCESS_TASK_PRIO,
                             LWIPPROCESS_TASK_STK_SIZE,
                             "Lwip Stack Task");
	
	#if (SERVER_NETWORK_UDP_EN ==1)
	err =PLATFORM_OS_TaskCreate((void (*)(void *)) UdpMng_Task,
                             (PLATFORM_OS_STK * ) UdpMng_TaskStack,
                             UDPMNG_TASK_PRIO,
                             UDPMNG_TASK_STK_SIZE,
                             "udp Stack Task");
	#endif

	EthStatus_Config(ETH_TASK_OK_FLAG,ENABLE);
	PLATFORM_OS_TimeDly(1000); /* 在上电1秒内不响应Modbus Tcp任务 */
	
	while (1) 
	{		
		LwIP_Periodic_Handle(TCPIP_PERIOD);  /* handle periodic timers for LwIP */
     
		if( GetLwip_Timemark())
		{
			Eth_Link_Stop();
		}
		Eth_Link_Check(DP83848_PHY_ADDRESS);
		
		UpdateIPConfig();
		
		PLATFORM_OS_TimeDly(TCPIP_PERIOD);
	}
}
#endif

/*
*********************************************************************************************************
* Description : Create the application Events
*
* Argument(s) : none.
*
* Return(s)   : none.
*********************************************************************************************************
*/

static  void  AppEventCreate (void)
{	
	PLATFORM_OS_SemCreate(&SYSMutex,1,"SYSMutex");
	PLATFORM_OS_SemCreate(&SYSCtrlSem,0,"SYSCtrlSem");
	PLATFORM_OS_SemCreate(&EthPktSem,0,"EthPktSem");		
	PLATFORM_OS_SemCreate(&UDPPktRecvSem,0,"UDPPktRecvSem");		
	PLATFORM_OS_SemCreate(&UDPPktSendSem,0,"UDPPktSendSem");

	CanRTxTaskSendSem = OSMboxCreate((void*)0);
	revProgramSem = OSMboxCreate((void*)0);	
	KeyMbox    = OSMboxCreate((void*)0);
	fatfsSaveFileSem = OSSemCreate(0);
	UsartRTxTaskSendSem = OSMboxCreate((void*)0);
}

/*
*********************************************************************************************************
* Description : Create the application tasks.
*
* Argument(s) : none.
*
* Return(s)   : none.
*********************************************************************************************************
*/
static  void  AppTaskCreate (void)
{
	uint8_t err;
	
/**
 Lwip Task	
*/
#if ((SERVER_NETWORK_UDP_EN ==1)||(SERVER_NETWOEK_IPSCANER_EN==1))
	err =PLATFORM_OS_TaskCreate((void (*)(void *)) LwipMng_Task,
                             (PLATFORM_OS_STK * ) LwipMng_TaskStack,
                             LWIPMNG_TASK_PRIO,
                             LWIPMNG_TASK_STK_SIZE,
                             "Lwip Stack Periodic Task");
#endif
	
/**
 SysFunc Main Task
*/
#if (LOGIC_SYSFUNC_EN == 1)
	err =PLATFORM_OS_TaskCreate((void (*)(void *)) SysFuncMng_Task,
                             (PLATFORM_OS_STK * ) SysFuncMng_TaskStack,
                             SYSFUNCMNG_TASK_PRIO,
                             SYSFUNCMNG_TASK_STK_SIZE,
                             "SysFunc Manage Task");
#endif
	
/**
 Device Mng Task
*/
#if (SERVER_DEVICEMNG_EN == 1)
	err =PLATFORM_OS_TaskCreate((void (*)(void *)) DeviceMng_Task,
                             (PLATFORM_OS_STK * ) DeviceMng_TaskStack,
                             DEVICEMNG_TASK_PRIO,
                             DEVICEMNG_TASK_STK_SIZE,
                             "Device Manage Task");
#endif

/**
 SHELL command Task
*/	
#if ((RUN_MODE == DEBUG_MODE)&&(SERVER_NETWORK_BCM_EN==1))
	err =PLATFORM_OS_TaskCreate((void (*)(void *)) ShellCmd_Task,
                             (PLATFORM_OS_STK * ) Shell_TaskStack,
                             SHELL_TASK_START_PRIO,
                             SHELL_TASK_STK_SIZE,
                             "SHELL command Task");
#endif 	
	
	
	err =PLATFORM_OS_TaskCreate((void (*)(void *)) AppTaskGUI,
                             (PLATFORM_OS_STK * ) GUI_TaskStack,
                             APP_TASK_GUI_PRIO,
                             APP_TASK_GUI_STK_SIZE,
                             "gui main Task");
	
	
	err =PLATFORM_OS_TaskCreate((void (*)(void *)) usart_send_task,
                             (PLATFORM_OS_STK * ) usart_send_task_stk,
                             USART_SEND_TASK_PRIO,
                             TASK_STK_SIZE_256,
                             "uart send Task");
	
//	err =PLATFORM_OS_TaskCreate((void (*)(void *)) usart_receive_task,
//                             (PLATFORM_OS_STK * ) usart_receive_task_stk,
//                             USART_RECEIVE_TASK_PRIO,
//                             TASK_STK_SIZE_512,
//                             "uart receive Task");

	err =PLATFORM_OS_TaskCreate((void (*)(void *)) Util_Timer_Task,
                             (PLATFORM_OS_STK * ) Util_Timer_TaskStack,
                             UTILTIMER_TASK_PRIO,
                             UTILTIMER_TASK_STK_SIZE,
                             "util timer Task");

	err =PLATFORM_OS_TaskCreate((void (*)(void *)) CanRec_task,
                             (PLATFORM_OS_STK * ) CanMng_TaskStack,
                             CANPROCESS_TASK_PRIO,
                             CANMNG_TASK_STK_SIZE,
                             "CAN command Task");
	
	err =PLATFORM_OS_TaskCreate((void (*)(void *)) FileLoad_task,
                             (PLATFORM_OS_STK * ) FileLoad_TaskStack,
                             FILELOAD_TASK_PRIO,
                             FILELOAD_TASK_STK_SIZE,
                             "CAN command Task");

#ifdef USE_V4YKQ
	err =PLATFORM_OS_TaskCreate((void (*)(void *)) UsartRec_task,
                             (PLATFORM_OS_STK * ) UsartMng_TaskStack,
                             USARTPROCESS_TASK_PRIO,
                             USARTMNG_TASK_STK_SIZE,
                             "CAN command Task");
#endif	
}


void Tim7Init(void)
{
	
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;				             //定时器初始化结构
	NVIC_InitTypeDef  NVIC_InitStructure;  
	RCC_ClocksTypeDef  rcc_clocks; 
	
	RCC_APB1PeriphClockCmd (RCC_APB1Periph_TIM7, ENABLE);

	
	TIM_DeInit(TIM7);
	RCC_GetClocksFreq(&rcc_clocks);					//调用标准库函数，获取系统时钟。

 	TIM_TimeBaseStructure.TIM_Period =99;     					         
	TIM_TimeBaseStructure.TIM_Prescaler = 859;       				         //时钟分频
	TIM_TimeBaseStructure.TIM_ClockDivision =TIM_CKD_DIV1;   		         //时钟分割
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;		         //模式,向上计数模式
	TIM_TimeBaseInit(TIM7, &TIM_TimeBaseStructure);					         //按结构体的值，初始化定时器
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM7_IRQn;                          
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0;                // 设置成员 NVIC_IRQChannel中的先占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;                       // 设置成员 NVIC_IRQChannel中的从优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = (FunctionalState)ENABLE;         // 使能 NVIC_IRQChannel
	NVIC_Init(&NVIC_InitStructure);                                          // 初始化外设 NVIC 寄存器
	
	TIM7->CNT=0;

	TIM_ClearITPendingBit(TIM7, TIM_IT_Update);
	TIM_ClearFlag(TIM7, TIM_FLAG_Update);	
	TIM_SetCounter(TIM7,0);
	TIM_ITConfig(TIM7, TIM_IT_Update, (FunctionalState)ENABLE);				// 使能中断请求
	TIM_Cmd(TIM7, (FunctionalState)ENABLE);
}


u8 spitestflag = 0;
u32 spitestmitmer = 0;
/*******************************************************************************************
**函数名称：TIM5_IRQHandler
**函数作用：TIM5定时中断
**函数参数：无
**函数输出：无
**注意事项：无
*******************************************************************************************/
void TIM7_IRQHandler(void)
{
	OSIntEnter();
	if(TIM_GetITStatus(TIM7,TIM_IT_Update )!= RESET)
	{
		TIM_ClearITPendingBit(TIM7, TIM_IT_Update);
		TIM_ClearFlag(TIM7, TIM_FLAG_Update);	
		
			if(spitestflag)
				spitestmitmer++;
	}
	OSIntExit();
}
/*
*********************************************************************************************************
*                                          STARTUP TASK
*
* Description : This is an example of a startup task.  As mentioned in the book's text, you MUST
*               initialize the ticker only once multitasking has started.
*
* Arguments   : p_arg   is the argument passed to 'AppTaskStart()' by 'OSTaskCreate()'.
*
* Returns     : none
*
* Notes       : (void) p_arg is used to prevent a compiler warning because 'p_arg' is not
*                used.  The compiler should not generate any code for this statement.
*********************************************************************************************************
*/
static void APPMng_Task(void *p_arg) 
{	
	uint16_t device[2] = {0x00,0x01};
	uint16_t bund[2]   = {200 ,200 };	
	(void) p_arg;
	
	BSP_Init();
	
//	Tim7Init();  //1ms
	font_init();
	mempool_init();
	memory_init();
	
	OS_CPU_SysTickInit();
	Fatfs_Mount();

	RUN_TRACE_INFO("OS Running...\r\n");

	AppEventCreate();
	WDTEnable();         //标准版不适用自动重启
	sysParamInit();
	GUIInitShow();
	UtilTimerInit(TIM5);
	CanProInit(device,bund,2);
	Wireless_Pro_Init();
	
	AppTaskCreate();
	
	while(1)
	{		
		PLATFORM_OS_TimeDly(1000);
		WDTReload(WDT_MANAGE);   
	}
}	

/*
*********************************************************************************************************
* Description : main function
*
* Arguments   : none
*
* Returns     : none
*********************************************************************************************************
*/
int main(void)
{	
	uint8_t os_err;
	
	PLATFORM_OS_Init();

	os_err = PLATFORM_OS_TaskCreate((void (*)(void *)) APPMng_Task,
                             (PLATFORM_OS_STK * ) APPMng_TaskStack,
                             APPMNG_TASK_START_PRIO,
                         APPMNG_TASK_STK_SIZE,
                             "Startup Task");
	
	if (os_err == DEF_OK)
	{
		PLATFORM_OS_Start();
	} 
	
	return 0;
	
	
}





