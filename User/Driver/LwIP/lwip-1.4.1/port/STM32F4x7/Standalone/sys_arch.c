/*
*********************************************************************************************************
*	                                            TK100+
*                           Commuication Controller For The Mining Industry
*
*                                  (c) Copyright 1994-2014  HNDZ 
*                       All rights reserved.  Protected by international copyright laws.
*
*
*    File    : sys_arch.c
*    Module  : user
*    Version : V1.0
*    History :
*   -----------------
*              Version  Date           By            Note
*              v1.0     2014-05-15     FanHaichun
*
*********************************************************************************************************
*/
#include "lwip/opt.h"

#if  (NO_SYS == 0)
#define __SYS_ARCH_C__

#include "arch/cc.h"
#include "arch/sys_arch.h"
#include "lwip/sys.h"

static OS_STK       TCPIP_THREAD_STK[TCPIP_THREAD_PRIO_NUM][TCPIP_THREAD_STACKSIZE];

/******************************************************************************************************
                                      sys_init

  @Description : Is called to initialize the sys_arch layer
  @Arguments   : none
  @Returns     : none
 ******************************************************************************************************/
void sys_init(void)
{
	/*None Done*/
	return;
}

/******************************************************************************************************
                                      sys_sem_new

  @Description : Creates a new semaphore.
  @Arguments   : sem   - point to which can be both a pointer or the actual OS structure.
                 count - the initial state of the semaphore which is either 0 or 1
  @Returns     : error type
 ******************************************************************************************************/
err_t sys_sem_new(sys_sem_t *sem, u8_t count)
{
	*sem = OSSemCreate(count);
	if (*sem != NULL)
	{
		return ERR_OK;
	}
	else
	{
		return ERR_ARG;
	}
}

/******************************************************************************************************
                                      sys_sem_free

  @Description : Deallocates a semaphore. there none done
  @Arguments   : sem   - point to which can be both a pointer or the actual OS structure.
  @Returns     : none
 ******************************************************************************************************/
void sys_sem_free(sys_sem_t *sem)
{
	/* None done */
	return;
}

/******************************************************************************************************
                                      sys_sem_signal

  @Description : signals a semaphore
  @Arguments   : sem   - point to which can be both a pointer or the actual OS structure.
  @Returns     : none
 ******************************************************************************************************/
void sys_sem_signal(sys_sem_t *sem)
{
	if (*sem != NULL)
	{
		OSSemPost(*sem);
	}
	return;
}

/******************************************************************************************************
                                      sys_arch_sem_wait

  @Description : waiting for the semaphore to be signaled. If the "timeout" argument is non-zero, the
                 thread should only be blocked for the specified time(measured in millisiseconds).
  @Arguments   : sem   - point to which can be both a pointer or the actual OS structure.
  @Returns     : time(ms or timeout sign)
 ******************************************************************************************************/
u32_t sys_arch_sem_pend(sys_sem_t *sem, u32_t timeout)
{
	u8_t  err;
	
	OSSemPend(*sem,((u16_t)timeout),&err);
	if (err == OS_ERR_NONE)
	{
		return ERR_OK;
	}
	return err;
}
u32_t sys_arch_sem_wait(sys_sem_t *sem, u32_t timeout)
{
	u8_t  err;
	u32_t start_time = OSTimeGet();
	
	OSSemPend(*sem,((u16_t)timeout),&err);
	
	if (err == OS_ERR_NONE)
	{
		u32_t now_time = OSTimeGet();
		if (now_time >= start_time)
		{
			return (now_time - start_time);
		}
		else
		{
			return(now_time + 2); /* ticks=1ms时，49.7天会遇到1次ticks翻转情况，此情况下用此模糊算法 */
		}
	}
	
	return (SYS_ARCH_TIMEOUT);	
}

/******************************************************************************************************
                                      sys_sem_valid

  @Description : check the pointer for != NULL
  @Arguments   : sem   - point to which can be both a pointer or the actual OS structure.
  @Returns     : returns 1 if the semaphore is valid, 0 if it is not valid.
 ******************************************************************************************************/
int sys_sem_valid(sys_sem_t *sem)
{
	if (*sem != NULL)
	{
		return 1;
	}	
	else
	{
		return 0;
	}
}

/******************************************************************************************************
                                      sys_sem_set_invalid

  @Description : for anything abnormity,there none done
  @Arguments   : sem   - point to which can be both a pointer or the actual OS structure.
  @Returns     : none
 ******************************************************************************************************/
void sys_sem_set_invalid(sys_sem_t *sem)
{
	/* None Done*/
	return;	
}

/******************************************************************************************************
                                      sys_mutex_new

  @Description : 
  @Arguments   : 
  @Returns     : none
 ******************************************************************************************************/
err_t sys_mutex_new(sys_mutex_t *mutex)
{
    u8_t err;
    
	*mutex = OSMutexCreate(1,&err);
	if (err == OS_ERR_NONE)
	{
        if (*mutex != NULL)
        {
            OSMutexPost(*mutex);
            return ERR_OK;
        }
	}
	return ERR_ARG;
}

/******************************************************************************************************
                                      sys_sem_valid

  @Description : 
  @Arguments   : 
  @Returns     : none
 ******************************************************************************************************/
void sys_mutex_lock(sys_mutex_t *mutex)
{	
    u8_t err;
    
	OSMutexPend(*mutex,0,&err);
}

/******************************************************************************************************
                                      sys_sem_valid

  @Description : 
  @Arguments   : 
  @Returns     : none
 ******************************************************************************************************/
void sys_mutex_unlock(sys_mutex_t *mutex)
{
    OSMutexPost(*mutex);
}

/******************************************************************************************************
                                      sys_sem_valid

  @Description : 
  @Arguments   : 
  @Returns     : none
 ******************************************************************************************************/
void sys_mutex_free(sys_mutex_t *mutex)
{
	/* None Done*/
	return;	
}

/******************************************************************************************************
                                      sys_sem_valid

  @Description : 
  @Arguments   : 
  @Returns     : none
 ******************************************************************************************************/
#ifndef sys_mutex_valid
int sys_mutex_valid(sys_mutex_t *mutex)
{
	if (*mutex != NULL)
	{
		return 1;
	}	
	else
	{
		return 0;
	}
}
#endif

/******************************************************************************************************
                                      sys_sem_valid

  @Description : 
  @Arguments   : 
  @Returns     : none
 ******************************************************************************************************/
#ifndef sys_mutex_set_invalid
void sys_mutex_set_invalid(sys_mutex_t *mutex)
{

}
#endif

/******************************************************************************************************
                                      sys_mbox_new

  @Description : creates an mailbox for maximum size elements. 
  @Arguments   : mbox - point to mailbox
  @Returns     : if the mailbox has been created, ERR_OK should be returned. 
                 returning any other error will provide a hint what went wrong.
 ******************************************************************************************************/
err_t sys_mbox_new(sys_mbox_t *mbox, int size)
{	
	(*mbox)->mbox = OSQCreate((*mbox)->msgs[0],size);
	
	return ERR_OK; 
}

/******************************************************************************************************
                                      sys_mbox_free

  @Description : dealocates a mailbox.
  @Arguments   : mbox - point to mailbox
  @Returns     : none
 ******************************************************************************************************/
void sys_mbox_free(sys_mbox_t *mbox)
{
	OSQFlush((*mbox)->mbox);   
	
	/* None Done*/
	return;	
}

/******************************************************************************************************
                                      sys_mbox_post

  @Description : post the msg to the mailbox. this function have to block until the msg is really posted.
  @Arguments   : mbox - point to the mailbox
                 msg  - the message waiting to send
  @Returns     : none
 ******************************************************************************************************/
void sys_mbox_post(sys_mbox_t *mbox, void *msg)
{
	while(OSQPost((*mbox)->mbox, msg) == OS_ERR_Q_FULL)
	{
		OSTimeDlyHMSM(0, 0, 0, 100);
	}
}

/******************************************************************************************************
                                      sys_mbox_trypost

  @Description : Try to post the "msg" to the mailbox. Returns ERR_MEM if this one is full, 
                 else, ERR_OK if the "msg" is posted.
  @Arguments   : mbox - point to the mailbox
                 msg  - the message waiting to send
  @Returns     : ERR type
 ******************************************************************************************************/
err_t sys_mbox_trypost(sys_mbox_t *mbox, void *msg)
{
	u8_t err;
	
	err = OSQPost((*mbox)->mbox, msg);
	
	if (err == OS_ERR_NONE)
	{
		return ERR_OK;
	}
	else if (err == OS_ERR_Q_FULL)
	{
		OSTimeDly(50);
		err = OSQPost((*mbox)->mbox, msg);
		if (err == OS_ERR_NONE)
		{
			return ERR_OK;
		}
		else 
		{
			return ERR_TIMEOUT;
		}
	}
	return ERR_ARG;
	
}

/******************************************************************************************************
                                      sys_arch_mbox_fetch

  @Description : Blocks the thread until a message arrives in the mailbox, but does
                 not block the thread longer than "timeout" milliseconds (similar to
                 the sys_arch_sem_wait() function). If "timeout" is 0, the thread should
                 be blocked until a message arrives. The "msg" argument is a result
                 parameter that is set by the function (i.e., by doing "*msg =
                 ptr"). The "msg" parameter maybe NULL to indicate that the message
                 should be dropped.
  @Arguments   : mbox    - point to the mailbox
                 msg     - the received message 
                 timeout - waiting time
  @Returns     : the return values are the same as for the sys_arch_sem_wait() function:
                 Number of milliseconds spent waiting or SYS_ARCH_TIMEOUT if there was a timeout
 ******************************************************************************************************/
u32_t sys_arch_mbox_fetch(sys_mbox_t *mbox, void **msg, u32_t timeout)
{
	u8_t err;   
	u32_t start_time = OSTimeGet();

	if (*msg != NULL)   
	{
		*msg = OSQPend((*mbox)->mbox, timeout, &err);   
	}
	else    
	{
		OSQPend((*mbox)->mbox, timeout, &err);   
	}

	if (err == OS_ERR_NONE)
	{
		u32_t now_time = OSTimeGet();
		if (now_time >= start_time)
		{
			return (now_time - start_time);
		}
		else
		{
			return(now_time + 2); /* ticks=1ms时，49.7天会遇到1次ticks翻转情况，此情况下用此模糊算法 */
		} 
	}
	else   
	{
		return SYS_ARCH_TIMEOUT;   
	}
}

/******************************************************************************************************
                                      sys_arch_mbox_tryfetch

  @Description : This is similar to sys_arch_mbox_fetch, however if a message is not
                 present in the mailbox, it immediately returns with the code
                 SYS_MBOX_EMPTY. On success 0 is returned.
  @Arguments   : mbox - point to the mailbox
                 msg  - the received message 
  @Returns     : none
 ******************************************************************************************************/
u32_t sys_arch_mbox_tryfetch(sys_mbox_t *mbox, void **msg)
{
	u8_t err;   
	u32_t start_time = OSTimeGet();

	if (msg != NULL)   
	{
		*msg = OSQPend((*mbox)->mbox, 0, &err);   
	}
	else    
	{
		OSQPend((*mbox)->mbox, 0, &err);   
	}

	if (err == OS_ERR_NONE)
	{
		u32_t now_time = OSTimeGet();
		if (now_time >= start_time)
		{
			return (now_time - start_time);
		}
		else
		{
			return(now_time + 2); /* ticks=1ms时，49.7天会遇到1次ticks翻转情况，此情况下用此模糊算法 */
		} 
	}
	else   
	{
		return SYS_ARCH_TIMEOUT;   
	} 
}

/******************************************************************************************************
                                      sys_mbox_valid

  @Description : when using pointers, a simple way is to check the pointer for != NULL.
                 when directly using OS structures, implementing this may be more complex.
                 this may also be a define, in which case the function is not prototyped.
  @Arguments   : mbox - point to the mailbox
  @Returns     : Returns 1 if the mailbox is valid, 0 if it is not valid.
 ******************************************************************************************************/
int sys_mbox_valid(sys_mbox_t *mbox)
{
	if (*mbox != NULL)
	{
		return 1;
	}	
	else
	{
		return 0;
	}	
}

/******************************************************************************************************
                                      sys_mbox_set_invalid

  @Description : 
  @Arguments   : 
  @Returns     : none
 ******************************************************************************************************/
void sys_mbox_set_invalid(sys_mbox_t *mbox)
{
	/*None Done*/
	return;
}

/******************************************************************************************************
                                      sys_thread_new

  @Description :  Starts a new thread named "name" with priority "prio" that will begin its
                  execution in the function "thread()". The "arg" argument will be passed as an
                  argument to the thread() function. The stack size to used for this thread is
                  the "stacksize" parameter. The id of the new thread is returned. Both the id
                  and the priority are system dependent.
 ******************************************************************************************************/
sys_thread_t sys_thread_new(const char *name, lwip_thread_fn thread, void *arg, int stacksize, int prio)
{    
    return (sys_thread_t)ERR_ARG; 
}

sys_thread_t sys_thread_create(const char *name, lwip_thread_fn thread, int prio)
{
	u8_t err;
	uint32_t stack_num;
	
    if (prio >= TCPIP_THREAD_PRIO_STA && prio < (TCPIP_THREAD_PRIO_STA + TCPIP_THREAD_PRIO_NUM))   
    {  
		stack_num = prio - TCPIP_THREAD_PRIO_STA;
		err = OSTaskCreateExt(thread,
						(void          * ) 0,
						(OS_STK        * )&TCPIP_THREAD_STK[stack_num][TCPIP_THREAD_STACKSIZE-1],
						(uint8_t         ) prio,
						(uint16_t        ) prio,
						(OS_STK        * )&TCPIP_THREAD_STK[stack_num][0],
						(INT32U          ) TCPIP_THREAD_STACKSIZE,
						(void          * )0,
						(uint16_t        )(OS_TASK_OPT_STK_CLR | OS_TASK_OPT_STK_CHK));
		if (err == OS_ERR_NONE)
		{
			OSTaskNameSet(prio, (INT8U*)name, &err);
			return (sys_thread_t)ERR_OK;
		}		
    }   
    
    return (sys_thread_t)ERR_ARG; 
}

int sys_thread_check(int prio)
{
	uint8_t err;
	OS_TCB      ptcb;
	
	err = OSTaskQuery((INT8U)prio,&ptcb);
	if (err == OS_ERR_NONE)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}



/******************************************************************************************************
                                      sys_arch_protect

  @Description : This optional function does a "fast" critical region protection and returns
                 the previous protection level. This function is only called during very short
                 critical regions. An embedded system which supports ISR-based drivers might
                 want to implement this function by disabling interrupts. Task-based systems
                 might want to implement this by using a mutex or disabling tasking. This
                 function should support recursive calls from the same task or interrupt. In
                 other words, sys_arch_protect() could be called while already protected. In
                 that case the return value indicates that it is already protected.

                 sys_arch_protect() is only required if your port is supporting an operating  system.
 ******************************************************************************************************/
sys_prot_t sys_arch_protect(void)
{
	OSSchedLock();
	return 0;
}

/******************************************************************************************************
                                      sys_arch_unprotect

  @Description : This optional function does a "fast" set of critical region protection to the
                 value specified by pval. See the documentation for sys_arch_protect() for
                 more information. This function is only required if your port is supporting
                 an operating system.
 ******************************************************************************************************/
void sys_arch_unprotect(sys_prot_t pval)
{
	OSSchedUnlock();
}

/******************************************************************************************************
                                      sys_now

  @Description : This optional function returns the current time in milliseconds (don't care
                 for wraparound, this is only used for time diffs).
                 Not implementing this function means you cannot use some modules (e.g. TCP
                 timestamps, internal timeouts for NO_SYS==1).
 ******************************************************************************************************/
u32_t sys_now(void)
{
	return(OSTimeGet());
}

#endif /*(NO_SYS == 0)*/
