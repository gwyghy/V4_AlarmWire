/*
*********************************************************************************************************
*	                                  
*	模块名称 : GUI界面主函数
*	文件名称 : MainTask.c
*	版    本 : V1.0
*	说    明 : GUI界面主函数
*		版本号   日期         作者            说明
*		v1.0    2015-08-05  Eric2013  	      首版
*
*	Copyright (C), 2015-2020, 安富莱电子 www.armfly.com
*
*********************************************************************************************************
*/

#ifndef __GUIKey_H
#define __GUIKey_H
#include <stdio.h>
#include <stdint.h>
/*
*********************************************************************************************************
*	                                     宏定义
*********************************************************************************************************
*/

#define DOWN 1
#define UP 0
//按键名称定义
#define  KEY_LEFT1  KID_RIGHT1
#define  KEY_LEFT2
#define  KEY_LEFT3

#define  KEY_RIGHT1
#define  KEY_RIGHT2
#define  KEY_RIGHT3

//#define  KEY_UP     KID_MENU
//#define  KEY_DOWN   KID_LEFT4


//GUI按键名称定义
#define USER_KEY_LEFT1     0x25
#define USER_KEY_LEFT2     0x26
#define USER_KEY_LEFT3     0x27
#define USER_KEY_RIGHT1    0x28
#define USER_KEY_RIGHT2    0x29
#define USER_KEY_RIGHT3    0x2A
#define USER_KEY_ASSIST    0x2B
#define USER_KEY_ESC   	  0x2C
#define USER_KEY_ENTER   	  0x2D

#define USER_KEY_UP   0x2E 
#define USER_KEY_DOWN   0x2F
#define USER_KEY_LEFT   0x36 
#define USER_KEY_RIGHT   0x37

#define USER_KEY_WORK   0x38

#define USER_KEY_MENU   0x39


#define USER_KEY_FOLLOW_LEFT   0x3A
#define USER_KEY_FOLLOW_RIGHT   0x3B

#define USER_KEY_RECOIL   0x3C

#define USER_KEY_2   0x30
#define USER_KEY_3   0x31
#define USER_KEY_5   0x32
#define USER_KEY_6   0x33
#define USER_KEY_8   0x34
#define USER_KEY_9   0x35

#define	KEY_L1				0x00000001
#define	KEY_L2				0x00000002
#define	KEY_L3				0x00000004
#define	KEY_R1				0x00000008
#define	KEY_R2				0x00000010
#define	KEY_R3				0x00000020

#define	KEY_2				0x00000040//
#define	KEY_3				0x00000080
#define	KEY_5				0x00000100
#define	KEY_6				0x00000200
#define	KEY_8				0x00000400
#define	KEY_9				0x00000800

#define	KEY_STOP			0x00001000
#define	KEY_UP				0x00002000
#define	KEY_DOWN			0x00004000//

#define	KEY_LEFT			0x00008000
#define	KEY_RIGHT			0x00010000
#define	KEY_START			0x00020000
#define	KEY_0				0x00040000
#define	KEY_WORK			0x00080000	
#define	KEY_MENU		    0x00100000

#define KEY_PASS  KEY_R1|KEY_R2|KEY_R3|KEY_2|KEY_3|KEY_5|KEY_6|KEY_8|KEY_9|KEY_0
#define KEY_ALL  KEY_R1|KEY_R2|KEY_R3|KEY_2|KEY_3|KEY_5|KEY_6|KEY_8|KEY_9|KEY_0 \
 |KEY_L1|KEY_L2|KEY_L3|KEY_R3|KEY_STOP|KEY_UP|KEY_DOWN|KEY_LEFT|KEY_RIGHT|KEY_START|KEY_WORK|KEY_MENU
#define KEY_MASTER  KEY_R1|KEY_R2|KEY_R3|KEY_2|KEY_3|KEY_5|KEY_6|KEY_8|KEY_9|KEY_0 \
 |KEY_L1|KEY_L2|KEY_L3|KEY_R3|KEY_STOP|KEY_UP|KEY_DOWN|KEY_LEFT|KEY_RIGHT|KEY_START|KEY_MENU
#define KEY_GROUP  KEY_R1|KEY_R2|KEY_R3|KEY_0 \
 |KEY_L1|KEY_L2|KEY_L3|KEY_R3|KEY_STOP|KEY_UP|KEY_DOWN|KEY_LEFT|KEY_RIGHT|KEY_START|KEY_MENU
 
#define KEY_CHOOSE  KEY_UP|KEY_DOWN
#define KEY_NUMBER  KEY_UP|KEY_DOWN|KEY_PASS
#define KEY_LOCAL   KEY_L1|KEY_L2
#define KEY_OVERALL  KEY_L1
#define KEY_RECOIL   KEY_LOCAL| KEY_START
int GUI_KeyUser(uint32_t data);
uint16_t DigitKeyToChr(uint16_t Key);
#endif
