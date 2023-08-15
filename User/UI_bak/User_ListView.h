/*
*********************************************************************************************************
*	                                  
*	ģ������ : User_Listview�û���
*	�ļ����� : User_Listview.h
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


#ifndef __User_Listview_H
#define __User_Listview_H

#include "User_GUI.h"

enum	
{
	DISPLAY_NULL,
	DISPLAY_NO_SHOW,
	DISPLAY_SHOW,
	DISPLAY_SPECAL_SHOW,
	DISPLAY_SPECAL_SHOW1,
	
};
enum	
{

    EDIT_SEL		=	1,		//ѡ��������
	EDIT_INT		=	2,		//��������
	EDIT_FLOAT		=	3,		//������-������
	
	EDIT_DOUBLE		=	4,		//������-˫����
    EDIT_NOTE		=	5,		//��ʾ��������
    EDIT_CMD		=	6,		//������

    EDIT_SEL_CMD	=	7,		//����ѡ����
	
	EDIT_SIGNED_INT  = 8,		//�з�������		
    EDIT_SIGNED_FLOAT	=	9,	//�з��Ÿ�����

};
//�б�ͷ�ṹ��
typedef struct
{
	uint16_t wide;
	char u8headerMame[9];				
}LISTVIEW_HEADER;
/*
************************************************************************
*						���ⲿ�ļ�����
************************************************************************
*/

void USER_ReviseListview(WM_MESSAGE * pMsg);
void USER_ReviseListviewHeader(WM_MESSAGE * pMsg,const LISTVIEW_HEADER *hader,uint8_t count);
void USER_ReviseListviewRow(WM_MESSAGE * pMsg);
void USER_ChooseListviewStartRow(WM_MESSAGE * pMsg);
void USER_MoveListviewRow(WM_MESSAGE * pMsg,uint8_t Direction);
void USER_EnterListviewRow(WM_MESSAGE * pMsg);
void USER_ExitListviewRow(WM_MESSAGE * pMsg);
void USER_SetValueListviewData(WM_MESSAGE * pMsg,uint16_t key);
void USER_ShwoListviewData(WM_MESSAGE * pMsg,uint8_t count);
uint8_t USER_SaveValueListviewData(WM_MESSAGE * pMsg,uint16_t key,uint64_t id);
uint8_t GetListviewData(uint64_t id);
#endif
