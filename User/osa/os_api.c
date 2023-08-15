
/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/
#include "os_api.h"

static PLATFORM_OS_CPU_SR  cpu_sr;

uint8_t  PLATFORM_OS_SemCreate (PLATFORM_OS_SEM       *p_sem,
							    PLATFORM_OS_SEM_VAL    sem_val,
							    char         *p_sem_name)
{
    OS_EVENT    *p_event;

#if (OS_EVENT_NAME_EN > 0)
    uint8_t  err;
#endif

    p_event = OSSemCreate(sem_val);

    if (p_event == (OS_EVENT *)0) 
	{
        return (DEF_FAIL);
    }

    *p_sem = (PLATFORM_OS_SEM)(p_event);

#if (OS_EVENT_NAME_EN > 0)
    OSEventNameSet((OS_EVENT *)p_event,
                   (INT8U    *)p_sem_name,
                   (INT8U    *)&err);
#endif


    return (DEF_OK);
}


uint8_t  PLATFORM_OS_SemWait (PLATFORM_OS_SEM *p_sem,
							  uint32_t  dly_ms)
{
    uint8_t  err;

    OSSemPend((OS_EVENT   *)*p_sem,
              (uint32_t  )dly_ms,
              (uint8_t *)&err);

    if (err == OS_ERR_NONE) {
       return (DEF_OK);
    }
	else if(err == OS_ERR_TIMEOUT) {
		return (DEF_TIMEOUT);
	}

    return (DEF_FAIL);
}


uint8_t  PLATFORM_OS_SemPost (PLATFORM_OS_SEM * p_sem)
{
    uint8_t  err;

    err = OSSemPost((OS_EVENT *)*p_sem);

    if (err != OS_ERR_NONE) {
        return (DEF_FAIL);
    }

    return (DEF_OK);
}

uint8_t  PLATFORM_OS_SemClear (PLATFORM_OS_SEM * p_sem)
{
	OS_EVENT    *p_event = (OS_EVENT *)*p_sem;
	
	if (p_event == (OS_EVENT *)0) 
	{
        return (DEF_FAIL);
    }

    p_event->OSEventCnt = 0;

    return (DEF_OK);
}


void   PLATFORM_OS_TimeDly (uint32_t  dly_tick)
{
    OSTimeDly(dly_tick);
}

uint8_t  PLATFORM_OS_MboxCreate (PLATFORM_OS_MBOX  *p_mbox,
							     void *p_msg,
							     char *p_mbox_name)
{
    OS_EVENT    *p_event;

#if (OS_EVENT_NAME_EN > 0)
    uint8_t  err;
#endif

    p_event = OSMboxCreate(p_msg);

    if (p_event == (OS_EVENT *)0) 
	{
        return (DEF_FAIL);
    }

    *p_mbox = (PLATFORM_OS_MBOX)(p_event);

#if (OS_EVENT_NAME_EN > 0)
    OSEventNameSet((OS_EVENT *)p_event,
                   (INT8U    *)p_mbox_name,
                   (INT8U    *)&err);
#endif

    return (DEF_OK);
}

uint8_t PLATFORM_OS_MboxWait (PLATFORM_OS_MBOX  *p_mbox,
							  uint32_t  dly_ms,
                              void *p_msg)
{
	uint8_t  err;

    p_msg = OSMboxPend((OS_EVENT   *)*p_mbox,
						(uint32_t  )dly_ms,
						(uint8_t *)&err);

    if (err == OS_ERR_NONE) {
       return (DEF_OK);
    }
	else if(err == OS_ERR_TIMEOUT) {
		return (DEF_TIMEOUT);
	}

    return (DEF_FAIL);
}

uint8_t  PLATFORM_OS_MboxPost (PLATFORM_OS_MBOX  *p_mbox,
								void  *p_msg)
{
    uint8_t  err;

	err = OSMboxPost((OS_EVENT *)*p_mbox,p_msg);

    if (err == OS_ERR_NONE) {
        return (DEF_OK);
    }
	if (err == OS_ERR_MBOX_FULL){
		return (DEF_MBOX_FULL);
	}

    return (DEF_FAIL);
}

uint8_t  PLATFORM_OS_TaskCreate (void   (*task)(void *p_arg),
								PLATFORM_OS_STK  *p_stk,
								uint8_t    prio,
								uint32_t   stk_size,
								uint8_t *p_task_name)
{
	uint8_t  err;
	
	err = OSTaskCreateExt(task,
			        (void          * ) 0,
					(OS_STK        * )&p_stk[stk_size-1],
			        (uint8_t         ) prio,
			        (uint16_t        ) prio,
			        (OS_STK        * )&p_stk[0],
			        (INT32U          ) stk_size,
			        (void          * )0,
			        (uint16_t        )(OS_TASK_OPT_STK_CLR | OS_TASK_OPT_STK_CHK));
	if (err == OS_ERR_NONE)
	{
		OSTaskNameSet(prio, p_task_name, &err);
		return (DEF_OK);
	}
	return (DEF_FAIL);
}

void  PLATFORM_OS_Init (void)
{
	OSInit(); 
}

void  PLATFORM_OS_Start (void)
{
	OSStart();
}

void PLATFORM_OS_ENTER_CRITICAL(void)
{
	OS_ENTER_CRITICAL();
}

void PLATFORM_OS_EXIT_CRITICAL(void)
{
	OS_EXIT_CRITICAL();
}

PLATFORM_OS_CPU_SR PLATFORM_OS_GetSR(void)
{
	return cpu_sr;
}
