/********************************************************************************
* �ļ����ƣ�	Wireless_Driver.h
* ��	�ߣ�  
* ��ǰ�汾��   	V1.0
* ������ڣ�    2015.04.24
* ��������: 	����bsp.hͷ�ļ�
* ��ʷ��Ϣ��   
*           	�汾��Ϣ     ���ʱ��      ԭ����        ע��
*
*       >>>>  �ڹ����е�λ��  <<<<
*             3-Ӧ�ò�
*          	  2-Э���
*          ��  1-Ӳ��������
*********************************************************************************
* Copyright (c) 2014,������������޹�˾ All rights reserved.
*********************************************************************************/
#ifndef __WIRELESS_DRIVER_H__ 
#define __WIRELESS_DRIVER_H__


#define WIRELESS_USART					USART1
#define GPIO_PORT_WIRELESS_TX  			GPIOA
#define GPIO_PORT_WIRELESS_RX  			GPIOA
#define GPIO_PIN_WIRELESS_TX				GPIO_Pin_9
#define GPIO_PIN_WIRELESS_RX				GPIO_Pin_10



#define  USARTSENDBUFSIZE          140


//�������ݽṹ��
typedef  struct
{
	uint8_t  Address;       //�豸��ַ
	uint8_t  Serial;        //��ˮ��
	uint8_t  FrameType;     //֡���
	uint8_t  ACK;           //Ӧ��
	uint8_t  u8DLC;         //���ݳ���
	uint8_t  SendBuf[USARTSENDBUFSIZE];      //����
	uint8_t  CRCHigh;         //CRCУ�� ���ֽ�
	uint8_t  CRCLow;       //CRCУ��  ���ֽ�
} STR_Usart_Frame;
void DriverUsart_Init(uint32_t bund);

#endif



