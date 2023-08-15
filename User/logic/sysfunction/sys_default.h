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
*              ϵͳ�����趨��ϵͳĬ��ֵ�궨��
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

/**��ע:����洢����ַʹ��˵��(2014.07.01)*********/
/*
 0x0000~0x03FF(1024�ֽ�),�洢ϵͳ����
 0x0400~0x0FFF(3072�ֽ�),�洢MODBUSϵͳ��ز���
 0x1000~0x1BFF(3072�ֽ�),�洢ϵͳ��־,��ص�ַ�Ķ���μ�Log_Data_Mng.h
 0x1C00~0x1FFF(1024�ֽ�),�洢�������ò�������ص�ַ�Ķ���μ�NetParamSet.h
*/
/**ϵͳ��������ʼ��ַ����**/
#define FRAM_SYS_PARAM_STARTADDR  	0x0000//ϵͳ��������ʼ��ַ
#define FRAM_SYS_PARAM_ENDADDR  	0x03FF//ϵͳ��������ʼ��ַ


#endif   /* __SYS_DEFAULT_H__ */
