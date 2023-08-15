
#ifndef  PLATFORM_OS
#define  PLATFORM_OS


/*
*********************************************************************************************************
*                                              INCLUDE FILES
*********************************************************************************************************
*/

#include  "ucos_ii.h"
#include  <stdint.h>

/*
*********************************************************************************************************
*                                                 EXTERNS
*********************************************************************************************************
*/

#ifdef   PLATFORM_OS_MODULE
#define  PLATFORM_OS_EXT
#else
#define  PLATFORM_OS_EXT  extern
#endif


/*
*********************************************************************************************************
*                                               DEFINES
*********************************************************************************************************
*/

#define DEF_OK   		0
#define DEF_FAIL 		1
#define DEF_TIMEOUT 	2
#define DEF_MBOX_FULL 	3


/*
*********************************************************************************************************
*                                             DATA TYPES
*********************************************************************************************************
*/

typedef   OS_EVENT    *PLATFORM_OS_SEM;
typedef   uint16_t    PLATFORM_OS_SEM_VAL;
typedef   OS_EVENT    *PLATFORM_OS_MBOX;
typedef   uint16_t    PLATFORM_OS_MBOX_VAL;
typedef   uint32_t    PLATFORM_OS_STK;
typedef   OS_CPU_SR   PLATFORM_OS_CPU_SR;

/*
*********************************************************************************************************
*                                          GLOBAL VARIABLES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                               MACRO'S
*********************************************************************************************************
*/
#define PLATFORM_OS_ENTER_INT()		OSIntEnter()
#define PLATFORM_OS_EXIT_INT()		OSIntExit()

/*
*********************************************************************************************************
*                                            FUNCTION PROTOTYPES
*********************************************************************************************************
*/

uint8_t  PLATFORM_OS_SemCreate          (PLATFORM_OS_SEM     *p_sem,
										PLATFORM_OS_SEM_VAL  sem_val,
										char       *p_sem_name);

uint8_t  PLATFORM_OS_SemWait            (PLATFORM_OS_SEM     *p_sem,
										uint32_t      dly_ms);

uint8_t  PLATFORM_OS_SemPost            (PLATFORM_OS_SEM     *p_sem);

uint8_t  PLATFORM_OS_SemClear 			(PLATFORM_OS_SEM * p_sem);

void  PLATFORM_OS_TimeDly            	(uint32_t      dly_tick);

uint8_t  PLATFORM_OS_MboxCreate 		(PLATFORM_OS_MBOX  *p_mbox,
										void *p_msg,
										char *p_mbox_name);
								 
uint8_t PLATFORM_OS_MboxWait 			(PLATFORM_OS_MBOX  *p_mbox,
										uint32_t  dly_ms,
										void *p_msg);
										
uint8_t  PLATFORM_OS_MboxPost 			(PLATFORM_OS_MBOX  *p_mbox,
										void  *p_msg);
										
uint8_t  PLATFORM_OS_TaskCreate 		(void   (*task)(void *p_arg),
										PLATFORM_OS_STK  *p_stk,
										uint8_t    prio,
										uint32_t   stk_size,
										uint8_t *p_task_name);										

void  PLATFORM_OS_Init (void);
void  PLATFORM_OS_Start (void);
void  PLATFORM_OS_ENTER_CRITICAL(void);
void  PLATFORM_OS_EXIT_CRITICAL(void);
PLATFORM_OS_CPU_SR PLATFORM_OS_GetSR(void);

/*
*********************************************************************************************************
*                                          CONFIGURATION ERRORS
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                             MODULE END
*********************************************************************************************************
*/

#endif                                                          
/* End of module include.                               */
