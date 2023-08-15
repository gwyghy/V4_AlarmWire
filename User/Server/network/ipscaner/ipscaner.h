/*
*********************************************************************************************************
*	                                            TK100+
*                           Commuication Controller For The Mining Industry
*
*                                  (c) Copyright 1994-2013  HNDZ 
*                       All rights reserved.  Protected by international copyright laws.
*
*
*    File    : ipscaner.h
*    Module  : user
*    Version : V1.0
*    History :
*   -----------------
*              Version  Date           By            Note
*              v1.0     2014-04-17    
*
*********************************************************************************************************
*/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __IPSCANER_H__
#define __IPSCANER_H__

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
#define IPSCAN_BYTE1	0x55
#define IPSCAN_BYTE2	0xAA
#define IPSCAN_BYTE3	0x00
#define IPSCAN_BYTE4	0x00

#define IPSET_BYTE1		0x11
#define IPSET_BYTE2		0x11
#define IPSET_BYTE3		0x11
#define IPSET_BYTE4		0x11


   
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void ipscanserver_init(void);



#endif 


