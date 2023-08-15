/*
*********************************************************************************************************
* Filename      : includes.h
* Version       : V1.00
* Programmer(s) : EHS
*                 DC
*********************************************************************************************************
*/
#ifndef  INCLUDES_PRESENT
#define  INCLUDES_PRESENT

#define  V4_CONTROLLER_VERSION_LEN      4

/*
*********************************************************************************************************
*                                         STANDARD LIBRARIES
*********************************************************************************************************
*/
#include  <stdarg.h>
#include  <stdio.h>
#include  <stdlib.h>
#include  <math.h>
#include "stdio.h"
#include "string.h"
#include  <stm32f4xx.h>
#include <core_cm4.h>
#include  <app_cfg.h>
#include  <OsEvent.h>
#include  "os_api.h"
#include  <bsp.h>

/*
*********************************************************************************************************
*                                               SDRAM/CCM POS
*********************************************************************************************************
*/
#define MEM_SDRAM_ALIGNMENT    4
#define MEM_SDRAM_ALIGN_SIZE(size) (((size) + MEM_SDRAM_ALIGNMENT - 1) & ~(MEM_SDRAM_ALIGNMENT-1))

#define  MEM_CCM_START				(0X10000000+2048)
#define  MEM_CCM_UTIL_MEMORY_POS	MEM_CCM_START	//41K
#define  MEM_CCM_FILERECV_POS       (MEM_CCM_START+41*1024)   //10k
#define  MEM_CCM_END				(MEM_CCM_UTIL_MEMORY_POS +51*1024)


//#define  MEM_SDRAM_START			(0XC0000000+16*1024*1024)
//#define	 MEM_SDRAM_CJSON_POS		 MEM_SDRAM_START                          //50k
//#define	 MEM_SDRAM_CJSON_MALLOC_POS	(MEM_SDRAM_CJSON_POS +50*1024)	          //10k ?
//#define	 MEM_SDRAM_FATFS_MALLOC_POS	0XC1500000                                //(2M+2M/64) =3M
//#define	 MEM_SDRAM_END				(MEM_SDRAM_FATFS_MALLOC_POS +3*1024*1024) 


#define  MEM_SDRAM_START			(0XC0000000+16*1024*1024)
#define	 MEM_SDRAM_CJSON_POS		(0XC0000000+16*1024*1024)
#define	 MEM_SDRAM_CJSON_MALLOC_POS	(0XC0000000+16*1024*1024+100000)
#define  MEM_SDRAM_FILERECV_POS     (0XC0000000+16*1024*1024+140000)
#define  MEM_SDRAM_FILESAVE_POS     (0XC0000000+16*1024*1024+140000+10240)
#define	 MEM_SDRAM_SYSFUNC_POS	    (0XC0000000+16*1024*1024+140000+10240+2*1024*1024)
#define	 MEM_LWIP_MEMORY_POS	    0XC1600000
#define	 MEM_SDRAM_SYSJSON_POS	    0XC1800000
#define	 MEM_SDRAM_FATFS_MALLOC_POS	0XC1C00000
/*
*********************************************************************************************************
*                                               SERIAL
*********************************************************************************************************
*/

#if (APP_CFG_SERIAL_EN == DEF_ENABLED)
//#include  <app_serial.h>
#endif



#define USE_V4YKQ   1

/*
*********************************************************************************************************
*                                            INCLUDES END
*********************************************************************************************************
*/

typedef enum {FALSE = 0, TRUE = !FALSE} BOOL;
#define ParamUnit    u16
#define ParamUnit_SETHIGHTBIT(x,value)    ((x&0x7fff) | (value<<16))
#define ParamUnit_GETHIGHTBIT(x)          (x>>16)


#endif

