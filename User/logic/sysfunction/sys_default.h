/*
*********************************************************************************************************
*	                                            DoubleWires
*                           Commuication Controller For The Mining Industry
*
*                                  (c) Copyright 1994-2013  HNDZ 
*                       All rights reserved.  Protected by international copyright laws.
*
*
*    File    : sys_default.h
*    Module  : system function
*    Version : V1.0
*    History :
*   -----------------
*              系统出厂设定及系统默认值宏定义
*              Version  Date           By            Note
*              v1.0     2013-08-08     xxx
*
*********************************************************************************************************
*/
#ifndef __SYS_DEFAULT_H__
#define __SYS_DEFAULT_H__

/* Macro */
#define IPV4(a,b,c,d)				((d<<24)|(c<<16)|(b<<8)|(a))

#define RELEASEID					20130809u
#define IMAGEID                     23060808u
#define DEVICE_TYPE                 0xE1020000

/**备注:铁电存储器地址使用说明(2014.07.01)*********/
/*
 0x0000~0x03FF(1024字节),存储系统参数
 0x0400~0x0FFF(3072字节),存储MODBUS系统相关参数
 0x1000~0x1BFF(3072字节),存储系统日志,相关地址的定义参见Log_Data_Mng.h
 0x1C00~0x1FFF(1024字节),存储网络设置参数，相关地址的定义参见NetParamSet.h
*/
/**系统参数的起始地址定义**/
#define FRAM_SYS_PARAM_STARTADDR  	0x0000//系统参数的起始地址
#define FRAM_SYS_PARAM_ENDADDR  	0x03FF//系统参数的起始地址


#endif   /* __SYS_DEFAULT_H__ */
