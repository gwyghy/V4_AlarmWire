/*
*********************************************************************************************************
*	                                  
*	ģ������ : 
*	�ļ����� : MenuConfig.h
*	��    �� : V1.0
*	˵    �� : ʵ�ְ������ֹ���
*              
*	�޸ļ�¼ :
*		�汾��   ����         ����          ˵��
*		V1.0    2016-07-16   Eric2013  	    �װ�    
*                                     
*	Copyright (C), 2015-2020, ���������� www.armfly.com
*
*********************************************************************************************************
*/


#ifndef __Menu_Config_H
#define __Menu_Config_H

//#include "User_GUI.h"
#include <stdio.h>
#include <stdint.h>
#include "WM.h"
#define PAGE_0_0_0     0x000000
typedef struct INITLIST INITLIST;	  
typedef struct SETLIST SETLIST;
/**�����ṹ��**/

typedef struct
{
	char         Para_Name[13];	//��������
	uint8_t	 pSelItem;	//ѡ�����
	uint16_t     Max_Value;	//���ֵ,
	uint16_t     Min_Value;	//��Сֵ
	uint8_t 	 Modift_Type;	//�޸�����			0-ȫ�������1-ȫ��&����		����
	uint8_t 	 Pass_Word;   //����Ȩ��    
	uint8_t 	 Input_Type;	//��������
	uint8_t 	 Show_Noshow;	//��ʾ����ʾ   	0-����ʾ��1-��ʾ	
	uint16_t 	 Para_Value;	//��ֵ
	uint16_t   	 Unit_Name; 	//��λ
}MENU_PARA;

struct INITLIST
{
	MENU_PARA  *Parameter;
	uint16_t   Para_Value;
	uint16_t  Deviation;	//ƫ��
};
struct SETLIST
{
	MENU_PARA  Parameter;
	uint16_t  Deviation;	//ƫ��
	uint8_t  Scope;  //�޸ķ�Χ
};

/*
************************************************************************
*						���ⲿ�ļ�����
************************************************************************
*/

//extern const char *ButtonName[];
//extern char TextName[];
void Creator_Menuwindow(WM_MESSAGE * pMsg);
void Menu_SetCurrentPage(uint16_t key);
void Menu_ShowButtonForPage(WM_MESSAGE * pMsg,uint64_t id);
void Menu_ShowListviewForPage(WM_MESSAGE * pMsg,uint64_t id);
void Menu_ShowTextForPage(WM_MESSAGE * pMsg,uint64_t id);
void GetPointerFromLogicalPast(uint64_t id,MENU_PARA **address ,uint8_t *length);
uint8_t GetMAXpage(uint8_t id);
#endif
