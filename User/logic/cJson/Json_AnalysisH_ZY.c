/*
*********************************************************************************************************
*	                                            V4
*                           Commuication Controller For The Mining Industry
*
*                                  (c) Copyright 1994-2013  HNDZ 
*                       All rights reserved.  Protected by international copyright laws.
*
*
*    File    : Can_Bus.c
*    Module  : user
*    Version : V1.0
*    History :
*   -----------------
*              Version  Date           By            Note
*              v1.0         
*
*********************************************************************************************************
*/
#include "includes.h"
#include "Json_Analysis.h"
#include "cJSON.h"
#include "DoFatfsFiles.h"
#include "sys_msg_def.h"

#define jsonParaQueueSize 1024
#define jsonNameSize  40
uint8_t jsonParaQueue[jsonParaQueueSize] = {0};
char  jsonName[jsonNameSize];

const char *ssss;

///*
const char V4paraall[] = {"{\"V4����":\
	[\
		{\"ϵͳ����\":\
			[\
				{\"���ܲ���\":\
					[\
						[\"���ܱ��\",0,250,1,3,1,2,2,6,0],\
						[\"�ܺ�����\",16,1,0,1,1,1,2,1,0],\
						[\"��С�ܺ�\",0,250,1,1,1,2,2,1,0],\
						[\"���ܺ�\",0,250,1,1,1,2,2,250,0],\
						[\"ֹͣ��Χ\",0,100,0,1,1,2,2,1,0],\
						[\"���ܼ���\",0,5,0,1,1,2,2,5,0],\
						[\"������ʹ��\",1,1,0,1,1,1,2,1,0],\
						[\"����ʱ��\",0,180,1,1,1,2,2,30,1],\
						[\"֧������\",24,2,0,1,1,1,2,0,0]\
					]\
				},\
				{\"�������\":\
					[\
						[\"��λ������\",    23,1,0,4,1,100,2,0,0],\
						[\"��λ������\",    23,1,0,4,1,101,1,0,0],\
						[\"���ͽӿڱ�\",    17,2,0,4,1,102,1,0,0],\
						[\"�������ӿڱ�\",  17,2,0,4,1,103,1,0,0],\
						[\"����������\",    17,2,0,4,1,104,1,0,0],\
						[\"����������\",    17,2,0,4,1,105,1,0,0],\
						[\"����ȫ�ֲ���\",  17,2,0,4,1,106,2,0,0],\
						[\"�������в���\",  17,2,0,4,1,107,2,0,0],\
						[\"�ָ���������\",  23,1,0,4,1,108,2,0,0],\
						[\"�������\",      23,1,0,4,1,109,1,0,0],\
						[\"�ָ�����\",      23,1,0,4,1,110,1,0,0]\
					]\
				},\
				{\"��������\":\
					[\
						[\"2��-1\",241,60,0,3,1,1,2,1,0],\
						[\"2��-2\",241,60,0,3,1,1,2,0,0],\
						[\"3��-1\",241,60,0,3,1,1,2,2,0],\
						[\"3��-2\",241,60,0,3,1,1,2,0,0],\
						[\"5��-1\",241,60,0,3,1,1,2,3,0],\
						[\"5��-2\",241,60,0,3,1,1,2,0,0],\
						[\"6��-1\",241,60,0,3,1,1,2,4,0],\
						[\"6��-2\",241,60,0,3,1,1,2,0,0],\
						[\"8��-1\",241,60,0,3,1,1,2,5,0],\
						[\"8��-2\",241,60,0,3,1,1,2,0,0],\
						[\"9��-1\",241,60,0,3,1,1,2,6,0],\
						[\"9��-2\",241,60,0,3,1,1,2,0,0]\
					]\
				},\
				{\"��������\":\
					[\
						[\"1-L1\",240,60,0,3,1,1,2,7,0],\
						[\"1-R1\",240,60,0,3,1,1,2,8,0],\
						[\"1-L2\",240,60,0,3,1,1,2,9,0],\
						[\"1-R2\",240,60,0,3,1,1,2,10,0],\
						[\"1-L3\",240,60,0,3,1,1,2,11,0],\
						[\"1-R3\",240,60,0,3,1,1,2,12,0],\
						[\"2-L1\",240,60,0,3,1,1,2,13,0],\
						[\"2-R1\",240,60,0,3,1,1,2,14,0],\
						[\"2-L2\",240,60,0,3,1,1,2,15,0],\
						[\"2-R2\",240,60,0,3,1,1,2,16,0],\
						[\"2-L3\",240,60,0,3,1,1,2,0,0],\
						[\"2-R3\",240,60,0,3,1,1,2,0,0],\
						[\"3-L1\",240,60,0,3,1,1,2,0,0],\
						[\"3-R1\",240,60,0,3,1,1,2,0,0],\
						[\"3-L2\",240,60,0,3,1,1,2,0,0],\
						[\"3-R2\",240,60,0,3,1,1,2,0,0],\
						[\"3-L3\",240,60,0,3,1,1,2,0,0],\
						[\"3-R3\",240,60,0,3,1,1,2,0,0],\
						[\"4-L1\",240,60,0,3,1,1,2,0,0],\
						[\"4-R1\",240,60,0,3,1,1,2,0,0],\
						[\"4-L2\",240,60,0,3,1,1,2,0,0],\
						[\"4-R2\",240,60,0,3,1,1,2,0,0],\
						[\"4-L3\",240,60,0,3,1,1,2,0,0],\
						[\"4-R3\",240,60,0,3,1,1,2,0,0],\
						[\"5-L1\",240,60,0,3,1,1,2,0,0],\
						[\"5-R1\",240,60,0,3,1,1,2,0,0],\
						[\"5-L2\",240,60,0,3,1,1,2,0,0],\
						[\"5-R2\",240,60,0,3,1,1,2,0,0],\
						[\"5-L3\",240,60,0,3,1,1,2,0,0],\
						[\"5-R3\",240,60,0,3,1,1,2,0,0]\
					]\
				},\
				{\"������\":\
					[\
						[\"��ʾ��1\",242,32,0,3,1,1,2,0,0],\
						[\"��ʾ��2\",242,32,0,3,1,1,2,0,0],\
						[\"��ʾ��3\",242,32,0,3,1,1,2,0,0],\
						[\"��ʾ��4\",242,32,0,3,1,1,2,0,0],\
						[\"��ʾ��5\",242,32,0,3,1,1,2,0,0],\
						[\"��ʾ��6\",242,32,0,3,1,1,2,0,0]\
					]\
				},\
				{\"���ⱨ��\":\
					[\
						[\"���ⱨ��ʹ��\",1,1,0,3,1,1,2,1,0]\
					]\
				},\
				{\"��Ա��λ\":\
					[\
						[\"��Ա��λʹ��\",1,1,0,3,1,1,2,0,0],\
						[\"��Ա����ʹ��\",1,1,0,3,1,1,2,0,0],\
						[\"��������\",0,1500,20,1,1,2,2,150,4],\
						[\"������ֵ\",0,1500,20,1,1,2,2,20,4],\
						[\"����ʱ��\",0,300,0,1,1,3,2,200,1],\
						[\"������\",0,1500,20,1,1,2,2,500,4],\
						[\"�ϱ����\",0,255,1,1,1,3,2,10,1],\
						[\"���͹���\",18,11,0,1,1,1,2,11,5],\
						[\"��������ID1\",0,10,1,1,1,2,1,1,0],\
						[\"��������ID2\",0,10,1,1,1,2,1,2,0]\
					]\
				},\
				{\"�������\":\
					[\
						[\"����ģ�����\",1,1,0,3,1,1,2,0,0],\
						[\"����������\",1,1,0,3,1,1,2,1,0]\
					]\
				},\
				{\"��������\":\
					[\
						[\"��������ʹ��\",1,1,0,3,1,1,2,0,0]\
					]\
				}\
			]\
		},\
		{\"������\":\
			[\
				{\"��ǰ��\":\
					[\
						{\"��������\":\
							[\
								[\"��ǰ��\",0,0,0,0,0,0,3,0,0],\
								[\"����ID\",0,40,1,0,0,0,4,1,0],\
								[\"������\",6,3,1,3,1,1,1,2,0],\
								[\"�豸��\",7,6,0,3,1,1,1,1,0],\
								[\"���豸��\",0,2,1,3,1,2,1,1,0],\
								[\"���\",0,32,1,3,1,2,2,1,0],\
								[\"�Ƿ�ʹ��\",1,1,0,3,1,1,2,1,0],\
								[\"��������\",2,4,0,1,1,1,1,1,0],\
								[\"��������ʽ\",14,4,0,1,1,1,1,3,0],\
								[\"�ⱨ����ʽ\",15,5,0,1,1,1,1,1,0],\
								[\"Ԥ��ʱ��\",0,255,0,1,1,3,1,0,1],\
								[\"��ʱʱ��\",2,255,0,1,1,3,2,0,1],\
								[\"ִ��ʱ��\",0,255,0,1,1,3,2,0,1],\
								[\"���Ʒ�ʽ\",21,2,0,1,1,1,1,0,0],\
								[\"���ܿ��Ʒ�ʽ\",22,1,0,1,1,1,1,0,0],\
								[\"��ֹ����1\",0,40,0,1,1,2,1,2,0],\
								[\"��ֹ����2\",0,40,0,1,1,2,1,0,0]\
							]\
						}\
					]\
				},\
				{\"��ǰ��\":\
					[\
						{\"��������\":\
							[\
								[\"��ǰ��\",0,0,0,0,0,0,3,0,0],\
								[\"����ID\",0,40,1,0,0,0,4,2,0],\
								[\"������\",6,3,0,3,1,1,1,2,0],\
								[\"�豸��\",7,6,0,3,1,1,1,1,0],\
								[\"���豸��\",0,2,1,3,1,2,1,1,0],\
								[\"���\",0,32,1,3,1,2,2,2,0],\
								[\"�Ƿ�ʹ��\",1,1,0,3,1,1,2,1,0],\
								[\"��������\",2,4,0,1,1,1,1,1,0],\
								[\"��������ʽ\",14,4,0,1,1,1,1,3,0],\
								[\"�ⱨ����ʽ\",15,5,0,1,1,1,1,1,0],\
								[\"Ԥ��ʱ��\",0,255,0,1,1,3,1,0,1],\
								[\"��ʱʱ��\",0,255,0,1,1,3,2,0,1],\
								[\"ִ��ʱ��\",0,255,0,1,1,3,2,100,1],\
								[\"���Ʒ�ʽ\",21,2,0,1,1,1,1,0,0],\
								[\"���ܿ��Ʒ�ʽ\",22,1,0,1,1,1,1,0,0],\
								[\"��ֹ����1\",0,40,0,1,1,2,1,1,0],\
								[\"��ֹ����2\",0,40,0,1,1,2,1,0,0]\
							]\
						}\
					]\
				},\
				{\"�Һ���\":\
					[\
						{\"��������\":\
							[\
								[\"�Һ���\",0,0,0,0,0,0,3,0,0],\
								[\"����ID\",0,40,1,0,0,0,4,3,0],\
								[\"������\",6,3,0,3,1,1,1,2,0],\
								[\"�豸��\",7,6,0,3,1,1,1,1,0],\
								[\"���豸��\",0,2,1,3,1,2,1,1,0],\
								[\"���\",0,32,1,3,1,2,2,3,0],\
								[\"�Ƿ�ʹ��\",1,1,0,3,1,1,2,1,0],\
								[\"��������\",2,4,0,1,1,1,1,1,0],\
								[\"��������ʽ\",14,4,0,1,1,1,1,3,0],\
								[\"�ⱨ����ʽ\",15,5,0,1,1,1,1,1,0],\
								[\"Ԥ��ʱ��\",0,255,0,1,1,3,1,0,1],\
								[\"��ʱʱ��\",0,255,0,1,1,3,2,0,1],\
								[\"ִ��ʱ��\",0,255,0,1,1,3,2,200,1],\
								[\"���Ʒ�ʽ\",21,2,0,1,1,1,1,0,0],\
								[\"���ܿ��Ʒ�ʽ\",22,1,0,1,1,1,1,1,0],\
								[\"��ֹ����1\",0,40,0,1,1,2,1,4,0],\
								[\"��ֹ����2\",0,40,0,1,1,2,1,0,0]\
							]\
						}\
					]\
				},\
				{\"�Һ�\":\
					[\
						{\"��������\":\
							[\
								[\"�Һ�\",0,0,0,0,0,0,3,0,0],\
								[\"����ID\",0,40,1,0,0,0,4,4,0],\
								[\"������\",6,3,1,3,1,1,1,2,0],\
								[\"�豸��\",7,6,0,3,1,1,1,1,0],\
								[\"���豸��\",0,2,1,3,1,2,1,1,0],\
								[\"���\",0,32,1,3,1,2,2,4,0],\
								[\"�Ƿ�ʹ��\",1,1,0,3,1,1,2,1,0],\
								[\"��������\",2,4,0,1,1,1,1,1,0],\
								[\"��������ʽ\",14,4,0,1,1,1,1,3,0],\
								[\"�ⱨ����ʽ\",15,5,0,1,1,1,1,1,0],\
								[\"Ԥ��ʱ��\",0,255,0,1,1,3,1,0,1],\
								[\"��ʱʱ��\",2,255,0,1,1,3,2,0,1],\
								[\"ִ��ʱ��\",0,255,0,1,1,3,2,0,1],\
								[\"���Ʒ�ʽ\",21,2,0,1,1,1,1,0,0],\
								[\"���ܿ��Ʒ�ʽ\",22,1,0,1,1,1,1,0,0],\
								[\"��ֹ����1\",0,40,0,1,1,2,1,3,0],\
								[\"��ֹ����2\",0,40,0,1,1,2,1,0,0]\
							]\
						}\
					]\
				},\
				{\"ǰ��\":\
					[\
						{\"��������\":\
							[\
								[\"ǰ��\",0,0,0,0,0,0,3,0,0],\
								[\"����ID\",0,40,1,0,0,0,4,5,0],\
								[\"������\",6,3,0,3,1,1,1,2,0],\
								[\"�豸��\",7,6,0,3,1,1,1,1,0],\
								[\"���豸��\",0,2,1,3,1,2,1,1,0],\
								[\"���\",0,32,1,3,1,2,2,5,0],\
								[\"�Ƿ�ʹ��\",1,1,0,3,1,1,2,1,0],\
								[\"��������\",2,4,0,1,1,1,1,1,0],\
								[\"��������ʽ\",14,4,0,1,1,1,1,3,0],\
								[\"�ⱨ����ʽ\",15,5,0,1,1,1,1,1,0],\
								[\"Ԥ��ʱ��\",0,255,0,1,1,3,1,0,1],\
								[\"��ʱʱ��\",0,255,0,1,1,3,2,0,1],\
								[\"ִ��ʱ��\",0,255,0,1,1,3,2,0,1],\
								[\"���Ʒ�ʽ\",21,2,0,1,1,1,1,0,0],\
								[\"���ܿ��Ʒ�ʽ\",22,1,0,1,1,1,1,0,0],\
								[\"��ֹ����1\",0,40,0,1,1,2,1,6,0],\
								[\"��ֹ����2\",0,40,0,1,1,2,1,0,0]\
							]\
						}\
					]\
				},\
				{\"����\":\
					[\
						{\"��������\":\
							[\
								[\"����\",0,0,0,0,0,0,3,0,0],\
								[\"����ID\",0,40,1,0,0,0,4,6,0],\
								[\"������\",6,3,0,3,1,1,1,2,0],\
								[\"�豸��\",7,6,0,3,1,1,1,1,0],\
								[\"���豸��\",0,2,1,3,1,2,1,1,0],\
								[\"���\",0,32,1,3,1,2,2,6,0],\
								[\"�Ƿ�ʹ��\",1,1,0,3,1,1,2,1,0],\
								[\"��������\",2,4,0,1,1,1,1,1,0],\
								[\"��������ʽ\",14,4,0,1,1,1,1,3,0],\
								[\"�ⱨ����ʽ\",15,5,0,1,1,1,1,1,0],\
								[\"Ԥ��ʱ��\",0,255,0,1,1,3,1,0,1],\
								[\"��ʱʱ��\",0,255,0,1,1,3,2,0,1],\
								[\"ִ��ʱ��\",0,255,0,1,1,3,2,0,1],\
								[\"���Ʒ�ʽ\",21,2,0,1,1,1,1,0,0],\
								[\"���ܿ��Ʒ�ʽ\",22,1,0,1,1,1,1,0,0],\
								[\"��ֹ����1\",0,40,0,1,1,2,1,5,0],\
								[\"��ֹ����2\",0,40,0,1,1,2,1,0,0]\
							]\
						}\
					]\
				},\
				{\"��ǰ��\":\
					[\
						{\"��������\":\
							[\
								[\"��ǰ��\",0,0,0,0,0,0,3,0,0],\
								[\"����ID\",0,40,1,0,0,0,4,7,0],\
								[\"������\",6,3,0,3,1,1,1,2,0],\
								[\"�豸��\",7,6,0,3,1,1,1,1,0],\
								[\"���豸��\",0,2,1,3,1,2,1,1,0],\
								[\"���\",0,32,1,3,1,2,2,7,0],\
								[\"�Ƿ�ʹ��\",1,1,0,3,1,1,2,1,0],\
								[\"��������\",2,4,0,1,1,1,1,1,0],\
								[\"��������ʽ\",14,4,0,1,1,1,1,3,0],\
								[\"�ⱨ����ʽ\",15,5,0,1,1,1,1,1,0],\
								[\"Ԥ��ʱ��\",0,255,0,1,1,3,1,0,1],\
								[\"��ʱʱ��\",0,255,0,1,1,3,2,0,1],\
								[\"ִ��ʱ��\",0,255,0,1,1,3,2,60,1],\
								[\"���Ʒ�ʽ\",21,2,0,1,1,1,1,0,0],\
								[\"���ܿ��Ʒ�ʽ\",22,1,0,1,1,1,1,0,0],\
								[\"��ֹ����1\",0,40,0,1,1,2,1,8,0],\
								[\"��ֹ����2\",0,40,0,1,1,2,1,0,0]\
							]\
						}\
					]\
				},\
				{\"��ǰ��\":\
					[\
						{\"��������\":\
							[\
								[\"��ǰ��\",0,0,0,0,0,0,3,0,0],\
								[\"����ID\",0,40,1,0,0,0,4,8,0],\
								[\"������\",6,3,0,3,1,1,1,2,0],\
								[\"�豸��\",7,6,0,3,1,1,1,1,0],\
								[\"���豸��\",0,2,1,3,1,2,1,1,0],\
								[\"���\",0,32,1,3,1,2,2,8,0],\
								[\"�Ƿ�ʹ��\",1,1,0,3,1,1,2,1,0],\
								[\"��������\",2,4,0,1,1,1,1,1,0],\
								[\"��������ʽ\",14,4,0,1,1,1,1,3,0],\
								[\"�ⱨ����ʽ\",15,5,0,1,1,1,1,1,0],\
								[\"Ԥ��ʱ��\",0,255,0,1,1,3,1,0,1],\
								[\"��ʱʱ��\",0,255,0,1,1,3,2,0,1],\
								[\"ִ��ʱ��\",0,255,0,1,1,3,2,60,1],\
								[\"���Ʒ�ʽ\",21,2,0,1,1,1,1,0,0],\
								[\"���ܿ��Ʒ�ʽ\",22,1,0,1,1,1,1,0,0],\
								[\"��ֹ����1\",0,40,0,1,1,2,1,7,0],\
								[\"��ֹ����2\",0,40,0,1,1,2,1,0,0]\
							]\
						}\
					]\
				},\
				{\"�����\":\
					[\
						{\"��������\":\
							[\
								[\"�����\",0,0,0,0,0,0,3,0,0],\
								[\"����ID\",0,40,1,0,0,0,4,9,0],\
								[\"������\",6,3,0,3,1,1,1,2,0],\
								[\"�豸��\",7,6,0,3,1,1,1,1,0],\
								[\"���豸��\",0,2,1,3,1,2,1,1,0],\
								[\"���\",0,32,1,3,1,2,2,9,0],\
								[\"�Ƿ�ʹ��\",1,1,0,3,1,1,2,1,0],\
								[\"��������\",2,4,0,1,1,1,1,1,0],\
								[\"��������ʽ\",14,4,0,1,1,1,1,3,0],\
								[\"�ⱨ����ʽ\",15,5,0,1,1,1,1,1,0],\
								[\"Ԥ��ʱ��\",0,255,0,1,1,3,1,0,1],\
								[\"��ʱʱ��\",0,255,0,1,1,3,2,20,1],\
								[\"ִ��ʱ��\",0,255,0,1,1,3,2,80,1],\
								[\"���Ʒ�ʽ\",21,2,0,1,1,1,1,0,0],\
								[\"���ܿ��Ʒ�ʽ\",22,1,0,1,1,1,1,1,0],\
								[\"��ֹ����1\",0,40,0,1,1,2,1,10,0],\
								[\"��ֹ����2\",0,40,0,1,1,2,1,0,0]\
							]\
						}\
					]\
				},\
				{\"���\":\
					[\
						{\"��������\":\
							[\
								[\"���\",0,0,0,0,0,0,3,0,0],\
								[\"����ID\",0,40,1,0,0,0,4,10,0],\
								[\"������\",6,3,0,3,1,1,1,2,0],\
								[\"�豸��\",7,6,0,3,1,1,1,1,0],\
								[\"���豸��\",0,2,1,3,1,2,1,1,0],\
								[\"���\",0,32,1,3,1,2,2,10,0],\
								[\"�Ƿ�ʹ��\",1,1,0,3,1,1,2,1,0],\
								[\"��������\",2,4,0,1,1,1,1,1,0],\
								[\"��������ʽ\",14,4,0,1,1,1,1,3,0],\
								[\"�ⱨ����ʽ\",15,5,0,1,1,1,1,1,0],\
								[\"Ԥ��ʱ��\",0,255,0,1,1,3,1,0,1],\
								[\"��ʱʱ��\",0,255,0,1,1,3,2,0,1],\
								[\"ִ��ʱ��\",0,255,0,1,1,3,2,0,1],\
								[\"���Ʒ�ʽ\",21,2,0,1,1,1,1,0,0],\
								[\"���ܿ��Ʒ�ʽ\",22,1,0,1,1,1,1,0,0],\
								[\"��ֹ����1\",0,40,0,1,1,2,1,9,0],\
								[\"��ֹ����2\",0,40,0,1,1,2,1,0,0]\
							]\
						}\
					]\
				},\
				{\"ǰ����\":\
					[\
						{\"��������\":\
							[\
								[\"ǰ����\",0,0,0,0,0,0,3,0,0],\
								[\"����ID\",0,40,1,0,0,0,4,11,0],\
								[\"������\",6,3,0,3,1,1,1,2,0],\
								[\"�豸��\",7,6,0,3,1,1,1,1,0],\
								[\"���豸��\",0,2,1,3,1,2,1,1,0],\
								[\"���\",0,32,1,3,1,2,2,11,0],\
								[\"�Ƿ�ʹ��\",1,1,0,3,1,1,2,1,0],\
								[\"��������\",2,4,0,1,1,1,1,1,0],\
								[\"��������ʽ\",14,4,0,1,1,1,1,3,0],\
								[\"�ⱨ����ʽ\",15,5,0,1,1,1,1,1,0],\
								[\"Ԥ��ʱ��\",0,255,0,1,1,3,1,0,1],\
								[\"��ʱʱ��\",0,255,0,1,1,3,2,0,1],\
								[\"ִ��ʱ��\",0,255,0,1,1,3,2,60,1],\
								[\"���Ʒ�ʽ\",21,2,0,1,1,1,1,0,0],\
								[\"���ܿ��Ʒ�ʽ\",22,1,0,1,1,1,1,0,0],\
								[\"��ֹ����1\",0,40,0,1,1,2,1,12,0],\
								[\"��ֹ����2\",0,40,0,1,1,2,1,0,0]\
							]\
						}\
					]\
				},\
				{\"ǰ�ܽ�\":\
					[\
						{\"��������\":\
							[\
								[\"ǰ�ܽ�\",0,0,0,0,0,0,3,0,0],\
								[\"����ID\",0,40,1,0,0,0,4,12,0],\
								[\"������\",6,3,0,3,1,1,1,2,0],\
								[\"�豸��\",7,6,0,3,1,1,1,1,0],\
								[\"���豸��\",0,2,1,3,1,2,1,1,0],\
								[\"���\",0,32,1,3,1,2,2,12,0],\
								[\"�Ƿ�ʹ��\",1,1,0,3,1,1,2,1,0],\
								[\"��������\",2,4,0,1,1,1,1,1,0],\
								[\"��������ʽ\",14,4,0,1,1,1,1,3,0],\
								[\"�ⱨ����ʽ\",15,5,0,1,1,1,1,1,0],\
								[\"Ԥ��ʱ��\",0,255,0,1,1,3,1,0,1],\
								[\"��ʱʱ��\",0,255,0,1,1,3,2,0,1],\
								[\"ִ��ʱ��\",0,255,0,1,1,3,2,60,1],\
								[\"���Ʒ�ʽ\",21,2,0,1,1,1,1,0,0],\
								[\"���ܿ��Ʒ�ʽ\",22,1,0,1,1,1,1,0,0],\
								[\"��ֹ����1\",0,40,0,1,1,2,1,11,0],\
								[\"��ֹ����2\",0,40,0,1,1,2,1,0,0]\
							]\
						}\
					]\
				},\
				{\"ǰ���\":\
					[\
						{\"��������\":\
							[\
								[\"ǰ���\",0,0,0,0,0,0,3,0,0],\
								[\"����ID\",0,40,1,0,0,0,4,13,0],\
								[\"������\",6,3,0,3,1,1,1,2,0],\
								[\"�豸��\",7,6,0,3,1,1,1,1,0],\
								[\"���豸��\",0,2,1,3,1,2,1,1,0],\
								[\"���\",0,32,1,3,1,2,2,13,0],\
								[\"�Ƿ�ʹ��\",1,1,0,3,1,1,2,1,0],\
								[\"��������\",2,4,0,1,1,1,1,1,0],\
								[\"��������ʽ\",14,4,0,1,1,1,1,3,0],\
								[\"�ⱨ����ʽ\",15,5,0,1,1,1,1,1,0],\
								[\"Ԥ��ʱ��\",0,255,0,1,1,3,1,0,1],\
								[\"��ʱʱ��\",0,255,0,1,1,3,2,0,1],\
								[\"ִ��ʱ��\",0,255,0,1,1,3,2,200,1],\
								[\"���Ʒ�ʽ\",21,2,0,1,1,1,1,0,0],\
								[\"���ܿ��Ʒ�ʽ\",22,1,0,1,1,1,1,1,0],\
								[\"��ֹ����1\",0,40,0,1,1,2,1,14,0],\
								[\"��ֹ����2\",0,40,0,1,1,2,1,0,0]\
							]\
						}\
					]\
				},\
				{\"ǰ�Ұ�\":\
					[\
						{\"��������\":\
							[\
								[\"ǰ�Ұ�\",0,0,0,0,0,0,3,0,0],\
								[\"����ID\",0,40,1,0,0,0,4,14,0],\
								[\"������\",6,3,0,3,1,1,1,2,0],\
								[\"�豸��\",7,6,0,3,1,1,1,1,0],\
								[\"���豸��\",0,2,1,3,1,2,1,1,0],\
								[\"���\",0,32,1,3,1,2,2,14,0],\
								[\"�Ƿ�ʹ��\",1,1,0,3,1,1,2,1,0],\
								[\"��������\",2,4,0,1,1,1,1,1,0],\
								[\"��������ʽ\",14,4,0,1,1,1,1,3,0],\
								[\"�ⱨ����ʽ\",15,5,0,1,1,1,1,1,0],\
								[\"Ԥ��ʱ��\",0,255,0,1,1,3,1,0,1],\
								[\"��ʱʱ��\",0,255,0,1,1,3,2,0,1],\
								[\"ִ��ʱ��\",0,255,0,1,1,3,2,200,1],\
								[\"���Ʒ�ʽ\",21,2,0,1,1,1,1,0,0],\
								[\"���ܿ��Ʒ�ʽ\",22,1,0,1,1,1,1,1,0],\
								[\"��ֹ����1\",0,40,0,1,1,2,1,13,0],\
								[\"��ֹ����2\",0,40,0,1,1,2,1,0,0]\
							]\
						}\
					]\
				},\
				{\"�����\":\
					[\
						{\"��������\":\
							[\
								[\"�����\",0,0,0,0,0,0,3,0,0],\
								[\"����ID\",0,40,1,0,0,0,4,15,0],\
								[\"������\",6,3,0,3,1,1,1,2,0],\
								[\"�豸��\",7,6,0,3,1,1,1,1,0],\
								[\"���豸��\",0,2,1,3,1,2,1,1,0],\
								[\"���\",0,32,1,3,1,2,2,15,0],\
								[\"�Ƿ�ʹ��\",1,1,0,3,1,1,2,1,0],\
								[\"��������\",2,4,0,1,1,1,1,1,0],\
								[\"��������ʽ\",14,4,0,1,1,1,1,3,0],\
								[\"�ⱨ����ʽ\",15,5,0,1,1,1,1,1,0],\
								[\"Ԥ��ʱ��\",0,255,0,1,1,3,1,0,1],\
								[\"��ʱʱ��\",0,255,0,1,1,3,2,0,1],\
								[\"ִ��ʱ��\",0,255,0,1,1,3,2,200,1],\
								[\"���Ʒ�ʽ\",21,2,0,1,1,1,1,0,0],\
								[\"���ܿ��Ʒ�ʽ\",22,1,0,1,1,1,1,1,0],\
								[\"��ֹ����1\",0,40,0,1,1,2,1,16,0],\
								[\"��ֹ����2\",0,40,0,1,1,2,1,0,0]\
							]\
						}\
					]\
				},\
				{\"���Ұ�\":\
					[\
						{\"��������\":\
							[\
								[\"���Ұ�\",0,0,0,0,0,0,3,0,0],\
								[\"����ID\",0,40,1,0,0,0,4,16,0],\
								[\"������\",6,3,0,3,1,1,1,2,0],\
								[\"�豸��\",7,6,0,3,1,1,1,1,0],\
								[\"���豸��\",0,2,1,3,1,2,1,1,0],\
								[\"���\",0,32,1,3,1,2,2,16,0],\
								[\"�Ƿ�ʹ��\",1,1,0,3,1,1,2,1,0],\
								[\"��������\",2,4,0,1,1,1,1,1,0],\
								[\"��������ʽ\",14,4,0,1,1,1,1,3,0],\
								[\"�ⱨ����ʽ\",15,5,0,1,1,1,1,1,0],\
								[\"Ԥ��ʱ��\",0,255,0,1,1,3,1,0,1],\
								[\"��ʱʱ��\",0,255,0,1,1,3,2,0,1],\
								[\"ִ��ʱ��\",0,255,0,1,1,3,2,200,1],\
								[\"���Ʒ�ʽ\",21,2,0,1,1,1,1,0,0],\
								[\"���ܿ��Ʒ�ʽ\",22,1,0,1,1,1,1,1,0],\
								[\"��ֹ����1\",0,40,0,1,1,2,1,15,0],\
								[\"��ֹ����2\",0,40,0,1,1,2,1,0,0]\
							]\
						}\
					]\
				}\				
			]\
		}\
	]\
}"};
//*/
uint8_t jsonDataQueue[85000] __attribute__((at(MEM_SDRAM_CJSON_POS)));//����������
STR_V4PARA_NAME childName[40];
static uint32_t writept = 0;
static cJSON  *v4Json = NULL;



void setParaDataProc(uint8_t * data,uint8_t len)
{
	
	if(writept<(85000-1))
	{
		memcpy(jsonDataQueue+writept,data,len);
	}
	writept += len;
}



/* ����������ݵĵ�ַλ��*/
uint32_t getParaAddress(void)
{
	return writept;
}

/*******************************************************************************************
**�������ƣ�getComAPI
**�������ã���ȡ������������
**������������
**�����������
**ע�������
*******************************************************************************************/
cJSON* getComAPI(uint64_t levels)
{
	uint8_t i = 0,j = 0,leveltmp = 0;
	cJSON *tmp = v4Json;
	if(levels == 0)
		return NULL;
	
	for(j = 0; j < 5; j++)
	{
		//����Ŀ¼
		leveltmp = ((levels>>((7-j)*8))&0xff);
		if(leveltmp == 0)
			break;
		if(!tmp->child)
			return NULL;
		
		tmp = tmp->child;
		if(tmp->type == cJSON_Array)//����array��cjson�ṹ��
		{
			if(!tmp->child) 
				return NULL;
			tmp = tmp->child;
		}
		//������Ӧ����
		for(i = 1; i<leveltmp; i++)
		{
			if(!tmp->next)
				return NULL;
			tmp = tmp->next;
		}
	}
	return tmp;
}


/*******************************************************************************************
**�������ƣ�getJsonParaAddress
**�������ã���ȡ�����Ķ�Ӧ��ַ
**������������
**�����������
**ע�������
*******************************************************************************************/
uint8_t* getJsonParaAddress(uint64_t levels)
{
	cJSON *tmp  = NULL;
	uint32_t leveladdress = 0;
	
	tmp = getComAPI(levels);
	if(tmp == NULL)
		return NULL;
	leveladdress = tmp->valueint;
	
	return (jsonDataQueue+leveladdress);
}


/*******************************************************************************************
**�������ƣ�getJsonMenuName
**�������ã���ȡ�����Ķ�Ӧmenu����
**������������
**�����������
**ע�������
*******************************************************************************************/
uint8_t getJsonMenuName(uint64_t levels,char ** name)
{
	uint8_t res = 0;
	static char *menuName;
	cJSON *tmp  = NULL;
	tmp = getComAPI(levels);
	if(tmp == NULL)
		return res;
	
	menuName = tmp->child->name;
	*name = menuName;
	res = 1;
	return res;
}



/*******************************************************************************************
**�������ƣ�getJsonParaChildSize
**�������ã���ȡ��������ײ��child����
**������������
**�����������
**ע�������
*******************************************************************************************/
uint8_t getJsonParaChildSize(uint64_t levels)
{
	cJSON *tmp = NULL;
	uint8_t childSize = 0;
	
	tmp = getComAPI(levels);
	if(tmp == NULL)
		return 0;
	while(tmp->type != cJSON_String)
	{
		if(!tmp->child)
			return 0;
		tmp = tmp->child;
	}
	childSize = tmp->childSize;
	
	return childSize;
}

/*******************************************************************************************
**�������ƣ�getJsonParaChildSize
**�������ã�������ȷ�ļ���
**������������
**�����������
**ע�������
*******************************************************************************************/
uint8_t ModifyTheLevel(uint64_t levels,uint8_t* sent)
{
	cJSON *tmp = NULL;
	uint64_t id = 0;
	uint8_t childSize = 0;
	uint8_t i = 0;
	uint8_t temp[8];
	if(levels == 0)
	{
		levels = 0x100000000000000;
	}
	tmp = getComAPI(levels);
	if(tmp == NULL)
		return 0;
	memset(temp,0,8);
	
	temp[0] = (uint8_t)(levels >> 56);
	temp[1] = (uint8_t)(levels >> 48);
	temp[2] = (uint8_t)(levels >> 40);
	temp[3] = (uint8_t)(levels >> 32);
	temp[4] = (uint8_t)(levels >> 24);
	temp[5] = (uint8_t)(levels >> 16);
	temp[6] = (uint8_t)(levels >> 8);
	temp[7] = (uint8_t)(levels );
	
	while(tmp->type != cJSON_String)
	{
		if(!tmp->child)
			return 0;
		tmp = tmp->child;
		if(tmp->type == cJSON_Array)
		{
			for(i=0;i<8;i++)
			{
				if(temp[i] == 0)
				{
					temp[i] = 0x01;
					break;
				}
			}
		}
		
	}	
	id |= ((uint64_t)temp[7]);
	id |= ((uint64_t)temp[6] <<8);
	id |= ((uint64_t)temp[5] <<16);
	id |= ((uint64_t)temp[4] <<24);
	id |= ((uint64_t)temp[3] <<32);
	id |= ((uint64_t)temp[2] <<40);
	id |= ((uint64_t)temp[1] <<48);
	id |= ((uint64_t)temp[0] <<56);
		
	
	memcpy(sent,(u8 *)&id,8);
}
/*******************************************************************************************
**�������ƣ�getJsonParaIsChilds
**�������ã���ȡ������û�к���
**������������
**�����������
**ע�������
*******************************************************************************************/
uint8_t getJsonParaIsChilds(uint64_t levels)
{
	cJSON *tmp = NULL;	
	tmp = getComAPI(levels);
	if(tmp == NULL)
		return 0;
	
	if(!tmp->child)
		return 0;
	
	if(tmp->child->type == cJSON_String)
		return 0;
	else 
		return 1;
}

/*******************************************************************************************
**�������ƣ�getJsonParaStruct
**�������ã���ȡ����ָ���ṹ��
**������������
**�����������
**ע�������
*******************************************************************************************/
uint8_t getJsonParaStruct(uint64_t levels,STR_V4PARA_NAME** childname)
{
	uint8_t i = 0,res = 0;
	cJSON *tmp = NULL;	
	tmp = getComAPI(levels);
	
	tmp = tmp->child;
	if(tmp->type == cJSON_Array)//����array��cjson�ṹ��
	{
		if(!tmp->child) 
			return NULL;
		tmp = tmp->child;
	}
		
	memset(childName,0,sizeof(childName));
	
	if(tmp == NULL)
		return 0;
	
	if(!tmp->child)
		return 0;
	if(tmp->type == cJSON_Object&& tmp->child!= NULL)
	{
		memcpy(&childName[0],tmp->child->name,sizeof(STR_V4PARA_NAME));
		res++;
	}
	for(i = 1; i<40; i++)
	{
		if(tmp->type == cJSON_Object&& tmp->child!= NULL && tmp->next!= NULL)
		{
			tmp = tmp->next;
			memcpy(&childName[i],tmp->child->name,sizeof(STR_V4PARA_NAME));
			res++;
		}
		else
			break;
		
	}	
	*childname = childName;
	return res;
}

uint8_t getJsonParaStructSize(uint64_t levels)
{
	uint8_t i = 0,res = 0;
	cJSON *tmp = NULL;	
	tmp = getComAPI(levels);

	tmp = tmp->child;
	if(tmp->type == cJSON_Array)//����array��cjson�ṹ��
	{
		if(!tmp->child) 
			return NULL;
		tmp = tmp->child;
	}
	
	if(tmp == NULL)
		return 0;
	
	if(!tmp->child)
		return 0;
	if(tmp->type == cJSON_Object&& tmp->child!= NULL)
	{
		res++;
	}
	for(i = 1; i<40; i++)
	{
		if(tmp->type == cJSON_Object&& tmp->child!= NULL && tmp->next!= NULL)
		{
			tmp = tmp->next;
			res++;
		}
		else
			break;
		
	}	
	return res;
}

/*******************************************************************************************
**�������ƣ�getParaBufferAddress
**�������ã���ȡ������ȡ��������һ��ָ��
**������������
**�����������
**ע�������
*******************************************************************************************/
uint8_t* getParaBufferAddress()
{
	return jsonParaQueue+jsonParaQueueSize-1;
}


/*******************************************************************************************
**�������ƣ�setParaBufferAddress
**�������ã���ȡjson�ļ�������ͷָ��
**������������
**�����������
**ע�������
*******************************************************************************************/
uint8_t* setParaBufferAddress()
{
	Fatfs_readFile(jsonName,jsonParaQueue,jsonParaQueueSize);
	return jsonParaQueue;
}

/*******************************************************************************************
**�������ƣ�getJsonVersion
**�������ã���ȡjson�İ汾��
**������������
**�����������
**ע�������
*******************************************************************************************/
uint8_t* getJsonVersion()
{
	cJSON *tmp = v4Json;
	return (uint8_t*)tmp->child->name;
}



void saveJsonToFatfs(void)
{
//	Fatfs_CreateDir("update");
	DeleteDirFile("0:/update/�ɱ��_V0001.txt");	
	Fatfs_createFile("0:/update/�ɱ��_V0001.txt");
	Fatfs_openFile("0:/update/�ɱ��_V0001.txt");
	Fatfs_writeFile((uint8_t *)V4paraall,sizeof(V4paraall));
	Fatfs_closeFile();
//	ee_WriteBytes("0:/update/�ɱ��_V0001.txt", JSON_PARAM_USING_START, 30);
//	AnalysisJsonProc("0:/update/�ɱ��_V0001.txt");
}


/*******************************************************************************************
**�������ƣ�AnalysisJsonProc
**�������ã�����json����
**������������
**�����������
**ע�������
*******************************************************************************************/
uint8_t AnalysisJsonProc(char * name)
{
	uint32_t i = 0;
	
	for(i = 0; i<jsonNameSize; i++)
	{
		jsonName[i] = *(name+i);
		if((name+i) == '\0')
		{
			break;
		}
	}
	
	i = Fatfs_openFile(jsonName,0);
	if(i == 0)
	{
		Fatfs_closeFile();
		return 0;
	}
	Fatfs_readFile(jsonName,jsonParaQueue,jsonParaQueueSize);
	
	v4Json=cJSON_Parse(jsonParaQueue);//(jsonParaQueue); //�ײ����
	Fatfs_closeFile();
	ssss = cJSON_GetErrorPtr();
	
	if(ssss == NULL)
		return 1;//success
	else
		return 0;//fault	
}


/*******************************************************************************************
**�������ƣ�AnalysisJsonProc
**�������ã�����json����
**������������
**�����������
**ע�������
*******************************************************************************************/
uint8_t AnalysisInsideJsonProc(void)
{
		
	v4Json=cJSON_Parse(V4paraall);//�ײ����
	ssss = cJSON_GetErrorPtr();
	
	if(ssss == NULL)
		return 1;//success
	else
		return 0;//fault	
}















