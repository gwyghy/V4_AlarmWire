/********************************************************************************
* �ļ�����	timer.h
* ���ߣ�    qx
* �汾��   	V1.0
* ���ڣ�    2020.07.23  
* ��������: ֧�ܿ������߼��������ʼ���ʹ��� 
* 			 
*			 
*			 
*			 
*			 
* �޸�˵����   
*
*       >>>>  �ڹ����е�λ��  <<<<
*             3-Ӧ�ò�
*             2-Э���
*             1-Ӳ��������
*********************************************************************************
* @copy
* <h2><center>&copy; COPYRIGHT ������������޹�˾ �з����� ������</center></h2>
*********************************************************************************/
#ifndef __TIMER_H__
#define __TIMER_H__
/********************************************************************************
* .hͷ�ļ�
*********************************************************************************/
#include "includes.h"
/********************************************************************************
* #define�궨��
*********************************************************************************/

void TIM3_Int_Init(u16 arr,u16 psc);
void TIM4_Int_Init(u32 arr,u32 psc);
#endif