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

#define  LWIPPROCESS_TASK_PRIO                   7u	    /*LWIP��Ϣ��������                          */
#define  LWIPMNG_TASK_PRIO                       8u	  	/*LWIP����������							*/                       
#define  UDPMNG_TASK_PRIO                        9u     /*UDP����������								*/
#define  UDPSEND_TASK_PRIO                       11u    /*UDP��������								*/
#define  UDPCYCLE_TASK_PRIO                      10u    /*UDP��������								*/
#define  UDPPROC_TASK_PRIO                       12u    /*UDPЭ��ջ����								*/
#define  SYSFUNCMNG_TASK_PRIO                    15u    /* ϵͳ���ܹ�������                         */ 
#define  SENSORMNG_TASK_PRIO                     14u    /* ��������������                           */
#define  UTILTIMER_TASK_PRIO                     13u
#define  CANPROCESS_TASK_PRIO                    16u	/*can��Ϣ��������                           */
#define  USARTPROCESS_TASK_PRIO                  17u	/*can��Ϣ��������                           */
#define  FILELOAD_TASK_PRIO                      19u    /*�ļ�������������                          */
#define  DEVICEMNG_TASK_PRIO                     22u    /* �豸��������                             */ 
#define  SYSCTRLMNG_TASK_PRIO                    24u    /* ϵͳ���ܿ��ƹ�������                     */ 
#define  USART_SEND_TASK_PRIO       	           20u 
#define  USART_RECEIVE_TASK_PRIO       	         21u 
#define  APP_TASK_GUI_PRIO       	               23u    /*GUI��������                               */  

#define  FATFS_SAVE_FILE_PRIO                    18u

#define  APPMNG_TASK_START_PRIO                  27u    /* ϵͳ�������                             */
#define  SHELL_TASK_START_PRIO                   29u    /* SHELL���������                        */    


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
