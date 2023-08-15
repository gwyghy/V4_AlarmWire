/********************************************************************************
* �ļ�����	usart.h
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
* <h2><center>&copy; COPYRIGHT ������������޹�˾ �з����� �����</center></h2>
*********************************************************************************/
#ifndef __USART_H__
#define __USART_H__
/********************************************************************************
* .hͷ�ļ�
*********************************************************************************/
#include "includes.h"
#include "usartapp.h"
/********************************************************************************
* #define�궨��
*********************************************************************************/
#define  USARTKEY_TX_BUF_MAX              10   //���Ͷ������ֵ
#define  USARTWIRELESS_TX_BUF_MAX              10   //���Ͷ������ֵ
//timer��Ϣ�ṹ��
typedef  struct
{
	uint16_t  u16Count;       //����
 	uint8_t   u8Enable;        //ʹ��
} stTIMER;

void UsartMessageForRfidRcved(void);
u8 getKeyDeleteFile(void);
u32 KEYUsartFetchData(stUARTKEY  *UsartRcvFrm);
u32 WIRELESSUsartFetchData(stUARTKEY *UsartRcvFrm);
void UartInit(void);
void Crc16UsartSendChick(const u8 *u8Buf, u32 u32Len, u16 *u16CheckOld);
void Crc16UsartReceiveChick(const u8 *u8Buf, u32 u32Len, u16 *u16CheckOld);

#endif
