/*
*********************************************************************************************************
*
*                                      APPLICATION CONFIGURATION
*
*                                     ST Microelectronics STM32
*                                              on the
*
*                                           STM3240G-EVAL
*                                         Evaluation Board
*
* Filename      : app_cfg.h
* Version       : V1.00
* Programmer(s) : FT
*                 DC
*********************************************************************************************************
*/

#ifndef  APP_CFG_MODULE_PRESENT
#define  APP_CFG_MODULE_PRESENT

/*
*********************************************************************************************************
*                                       MODULE ENABLE / DISABLE
*********************************************************************************************************
*/
#define  LOGIC_SYSFUNC_EN                     1
#define  LOGIC_FILELOAD_EN                    1
#define  LOGIC_FILEUPDATE_EN                  1
#define  SERVER_NETWORK_UDP_EN                1
#define  SERVER_NETWOEK_IPSCANER_EN           0
#define  SERVER_NETWORK_BCM_EN                1
#define  SERVER_CAN_EN                        1
#define  SERVER_DEVICEMNG_EN                  1
#define  SERVER_LOG_EN                        1
#define  SERVER_UTIL_EN                       1
#define  APP_UI_EN                            1
#define  DRIVER_FATFS_EN                      1

#define  SERVER_WIRELESS_EN                   0


/*
*********************************************************************************************************
*                                            TASK PRIORITIES
*********************************************************************************************************
*/

#define  LWIPPROCESS_TASK_PRIO                   7u	    /*LWIP信息处理任务                          */
#define  LWIPMNG_TASK_PRIO                       8u	  	/*LWIP处理主任务							*/                       
#define  UDPMNG_TASK_PRIO                        9u     /*UDP处理主任务								*/
#define  UDPSEND_TASK_PRIO                       11u    /*UDP发送任务								*/
#define  UDPCYCLE_TASK_PRIO                      10u    /*UDP周期任务								*/
#define  UDPPROC_TASK_PRIO                       12u    /*UDP协议栈任务								*/
#define  SYSFUNCMNG_TASK_PRIO                    15u    /* 系统功能管理任务                         */ 
#define  SENSORMNG_TASK_PRIO                     14u    /* 传感器管理任务                           */
#define  UTILTIMER_TASK_PRIO                     13u
#define  CANPROCESS_TASK_PRIO                    16u	/*can信息处理任务                           */
#define  USARTPROCESS_TASK_PRIO                  17u	/*can信息处理任务                           */
#define  FILELOAD_TASK_PRIO                      19u    /*文件程序下载任务                          */
#define  DEVICEMNG_TASK_PRIO                     22u    /* 设备管理任务                             */ 
#define  SYSCTRLMNG_TASK_PRIO                    24u    /* 系统功能控制管理任务                     */ 
#define  USART_SEND_TASK_PRIO       	           20u 
#define  USART_RECEIVE_TASK_PRIO       	         21u 
#define  APP_TASK_GUI_PRIO       	               23u    /*GUI处理任务                               */  

#define  FATFS_SAVE_FILE_PRIO                    18u

#define  APPMNG_TASK_START_PRIO                  27u    /* 系统监控任务                             */
#define  SHELL_TASK_START_PRIO                   29u    /* SHELL命令处理任务                        */    


/*
*********************************************************************************************************
*                                            TASK STACK SIZES
*                             Size of the task stacks (# of OS_STK entries)
*********************************************************************************************************
*/
#define  LWIPPROCESS_TASK_STK_SIZE               512u
#define  APPMNG_TASK_STK_SIZE                    512u
#define  SHELL_TASK_STK_SIZE                     1024u
#define  LWIPMNG_TASK_STK_SIZE                   256u
#define  UDPMNG_TASK_STK_SIZE					 256u
#define  UDPSEND_TASK_STK_SIZE					 256u
#define  UDPCYCLE_TASK_STK_SIZE					 256u
#define  UDPPROC_TASK_STK_SIZE					 256u
#define  SYSFUNCMNG_TASK_STK_SIZE                512u
#define  SENSORMNG_TASK_STK_SIZE                 256u
#define  DEVICEMNG_TASK_STK_SIZE			     128u
#define  SYSCTRLMNG_TASK_STK_SIZE                256u
#define	 APP_TASK_GUI_STK_SIZE					 2048u
#define  UTILTIMER_TASK_STK_SIZE                 256u
#define  CANMNG_TASK_STK_SIZE                    1024u
#define  FILELOAD_TASK_STK_SIZE                  256u
#define  FATFS_SAVE_FILE_STK_SIZE                256u

#define  USARTMNG_TASK_STK_SIZE                    512u


#define  TASK_STK_SIZE_256                       256u
#define  TASK_STK_SIZE_512                       1024u
#endif
