/*
*********************************************************************************************************
*	                                  
*	ģ������ : GUI����������
*	�ļ����� : HomeTask.c
*	��    �� : V1.0
*	˵    �� : GUI����������
*		�汾��   ����         ����            ˵��
*		v1.0    2015-08-05  Eric2013  	      �װ�
*
*	Copyright (C), 2015-2020, ���������� www.armfly.com
*
*********************************************************************************************************
*/

#ifndef __GROUPTASK_H
#define __GROUPTASK_H

#include "User_GUI.h"

/*
*********************************************************************************************************
*	                                     �궨��
*********************************************************************************************************
*/


/*
************************************************************************
*						  FatFs
************************************************************************
*/


enum
{
	left,
	right,
};
enum
{
	Single,
	Combined,
	Group,
};


typedef struct
{
	uint16_t  GroupID;
	uint16_t  ActionType;
	uint16_t  Direction;	//����
	uint16_t  number;
	uint16_t  SrartNumber;
	uint16_t  EndNumber;
}GroupStartUp;





/*
************************************************************************
*						���ⲿ�ļ�����
************************************************************************
*/

WM_HWIN CreateGroupLeft(void) ;

#endif

/*****************************(END OF FILE) *********************************/