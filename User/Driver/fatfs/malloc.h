#ifndef __MALLOC_H
#define __MALLOC_H
#include "sys.h" 
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ���������ɣ��������������κ���;
//ALIENTEK STM32������
//�ڴ���� ��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2016/1/13
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) �������������ӿƼ����޹�˾ 2014-2024
//All rights reserved 
////////////////////////////////////////////////////////////////////////////////// 	 

 
#ifndef NULL
#define NULL 0
#endif

//���������ڴ��
#define SRAMIN	 0		//�ڲ��ڴ��
#define SRAMEX   1		//�ⲿ�ڴ��(SDRAM)
#define SRAMCCM  2		//CCM�ڴ��(�˲���SRAM����CPU���Է���!!!)


#define SRAMBANK 	3	//����֧�ֵ�SRAM����.	


//mem1�ڴ�����趨.mem1��ȫ�����ڲ�SRAM����.
#define MEM1_BLOCK_SIZE			64  	  						//�ڴ���СΪ64�ֽ�
#define MEM1_MAX_SIZE			1024//50*1024  						//�������ڴ� 160K
#define MEM1_ALLOC_TABLE_SIZE	MEM1_MAX_SIZE/MEM1_BLOCK_SIZE 	//�ڴ����С

//mem2�ڴ�����趨.mem2���ڴ�ش����ⲿSDRAM����
#define MEM2_BLOCK_SIZE			64  	  						//�ڴ���СΪ64�ֽ�
#define MEM2_MAX_SIZE			2*1024*1024  					//�������ڴ�20M
#define MEM2_ALLOC_TABLE_SIZE	MEM2_MAX_SIZE/MEM2_BLOCK_SIZE 	//�ڴ����С
		 
//mem3�ڴ�����趨.mem3����CCM,���ڹ���CCM(�ر�ע��,�ⲿ��SRAM,��CPU���Է���!!)
#define MEM3_BLOCK_SIZE			64  	  						//�ڴ���СΪ64�ֽ�
#define MEM3_MAX_SIZE			1024  	//20*					//�������ڴ�60K
#define MEM3_ALLOC_TABLE_SIZE	MEM3_MAX_SIZE/MEM3_BLOCK_SIZE 	//�ڴ����С
		 

//�ڴ����������
struct _m_mallco_dev
{
	void (*init)(u8);					//��ʼ��
	u16 (*perused)(u8);		  	    	//�ڴ�ʹ����
	u8 	*membase[SRAMBANK];				//�ڴ�� ����SRAMBANK��������ڴ�
	u32 *memmap[SRAMBANK]; 				//�ڴ����״̬��
	u8  memrdy[SRAMBANK]; 				//�ڴ�����Ƿ����
};
extern struct _m_mallco_dev mallco_dev;	 //��mallco.c���涨��

void mymemset(void *s,u8 c,u32 count);	//�����ڴ�
void mymemcpy(void *des,void *src,u32 n);//�����ڴ�     
void my_mem_init(u8 memx);				//�ڴ������ʼ������(��/�ڲ�����)
u32 my_mem_malloc(u8 memx,u32 size);	//�ڴ����(�ڲ�����)
u8 my_mem_free(u8 memx,u32 offset);		//�ڴ��ͷ�(�ڲ�����)
u16 my_mem_perused(u8 memx) ;			//����ڴ�ʹ����(��/�ڲ�����) 
////////////////////////////////////////////////////////////////////////////////
//�û����ú���
void myfree(u8 memx,void *ptr);  			//�ڴ��ͷ�(�ⲿ����)
void *mymalloc(u8 memx,u32 size);			//�ڴ����(�ⲿ����)
void *myrealloc(u8 memx,void *ptr,u32 size);//���·����ڴ�(�ⲿ����)
#endif












