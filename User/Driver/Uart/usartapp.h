/********************************************************************************
* �ļ�����	usartapp.h
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
#ifndef __USARTAPP_H__
#define __USARTAPP_H__
/********************************************************************************
* .hͷ�ļ�
*********************************************************************************/
#include "includes.h"
/********************************************************************************
* #define�궨��
*********************************************************************************/
 #define  SendHeartTimerMAX     200
 #define  SCDEVIC        0
 #define  KEYDEVIC       2
 #define  WIRELESSDEVIC        3
 #define  USARTSENDBUFSIZE          129
 
  #define  KEYRESET   1

/**֡���Ͷ���**/
enum
{
	TYPE_HEART=0,
	TYPE_KEY_VALUE
};
/**Ӧ����**/
 enum
 {
 	NOACK=0,
 	ACK
 };
/**֡���ö��**///����
enum
{
	HEARTBEAT,//��������
	KEYVALUEANDSTATUS=1,//������Ϣ
	COMMUNICATIONLED,//LED��Ϣ
	NAVIGATIONLED,//LED����
	SETINFORMAION,//������Ϣ
	UPDATAPROGRAM,//��������
	RSUPDATAPROGRAM,//CHONGXINSHENGJI
	RESTORATION//FUWIE
};


//����
enum
{
	HEARTBEATWL,//��������
	WIRELESSDATA=1,//��������
	STARTHINT,//�ϵ���ʾ
	RESERVED,//Ԥ��
	PARAMETERSETTING,//��������
	UPDATAPROGRAMWL,//��������
	RSUPDATAPROGRAMWL,//CHONGXINSHENGJI
	RESTORATIONWL //FUWEI
};
enum
{
	KEYVAL,
	WRIELESSVAL
};




//�������ݽṹ��8
typedef  struct
{
	uint8_t  FrameType;     //֡���
	uint8_t  ACK;           //Ӧ��
	uint8_t  u8DLC;         //���ݳ���
	uint8_t  SendBuf[USARTSENDBUFSIZE];      //����
	uint8_t  CRCHigh;         //CRCУ�� ���ֽ�
	uint8_t  CRCLow;       //CRCУ��  ���ֽ�
} stUARTDATA;

//���ڷ��ͽṹ��
typedef  struct
{
	uint8_t  Address;       //�豸��ַ
 	uint8_t  Serial;        //��ˮ��
    stUARTDATA  stFrame;	
} stUARTKEY;

//������Ϣ�ṹ��
// typedef  struct
// {
// 	uint64_t  u64Key;       //����ֵ
//  	uint8_t   u8Status ;        //����״̬
// } stKEY;

void SendProgramTransmissionData(u8* pFrame,u8 len);
void SendLEDCommunicationData(u16 u16On, u16 u16Off);
void SendLEDNavigationData(u32 u32OnKey, u32 u32OffKey);
void SendResponseData(stUARTDATA pFrame, u8 Data, u8 Devic);
u32 UsartRecvSemCreate(void);
void usart_send_task(void *p_arg);
void usart_sendwl_task(void *p_arg);
void usart_receive_task(void *p_arg);
void usart_receivewl_task(void *p_arg);
void SendWirelessData(unsigned char *buf,u32 data);
void SendSetInformationData(u16 data);
void HeartBeatCommErrTimer(void);
int KEYRecvUsartDataProc(void);
#endif
