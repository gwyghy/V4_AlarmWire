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

#ifndef _LOG_DATA_TYPE_H_
#define _LOG_DATA_TYPE_H_

#define LOGDATASIZE    6


typedef uint8_t log_int;




/*������־*/
//��Ҫ�������
typedef enum
{
	FAULT_EVENT_FAULT,                             //����������,
	FAULT_EVENT_OFFLINE,                           //�豸����,
	FAULT_EVENT_WORKFACE_COMMUNICATION_WRONG,      //������ͨ���쳣,
	FAULT_EVENT_SYSTEM_NUMBER_WRONG,               //ϵͳ��Ŵ���,
	FAULT_EVENT_SERVER_WRONG,                      //SSλ�û�ʹ�ܲ���,
	FAULT_EVENT_NUMBER_ADD_DIRECT_WRONG,           //�ܺ����򲻷�,
	FAULT_EVENT_NUMBER_RANGE_WRONG,                //�ܺŷ�Χ��������,
	FAULT_EVENT_PARA_UPDATE_WRONG,				   //��������ʧ��,
	FAULT_EVENT_ALL_PARA_NOT_SAME,                 //ȫ�ֲ�����һ�� ,
	FAULT_EVENT_ALL_PARA_NOT_MATCH_LEFTNEIGHBOR,       //ȫ�ֲ������ڼܲ���,
	FAULT_EVENT_ALL_PARA_NOT_MATCH_RIGHTNEIGHBOR,       //ȫ�ֲ������ڼܲ���,
	FAULT_EVENT_PARGRAM_NOT_SAME,				   //����汾��һ�� ,
	FAULT_EVENT_PARGRAM_NOT_MATCH_NEIGHBOR,        //�汾�����ڼܲ���,
	FAULT_EVENT_HARD_SOFT_NOT_MATCH,               //Ӳ�����������,
	FAULT_EVENT_LEFT_NEIGHBOR_COMMUNICATION_WRONG, //���ڼ�ͨ�Ŵ���,
	FAULT_EVENT_RIGHT_NEIGHBOR_COMMUNICATION_WRONG,//���ڼ�ͨ�Ŵ���,
	FAULT_EVENT_ABNORMAL_COMMUNICATION_WITH_SERVER,//�������ͨ���쳣,
	/*�������������һ�¹�����Ҫ����*/
	FAULT_EVENT_SYSPARAM_MINSUPPORTNUM_NOTSAME,    //��������С�ܺŲ�һ��,
	FAULT_EVENT_SYSPARAM_MAXSUPPORTNUM_NOTSAME,    //���������ܺŲ�һ��,
	FAULT_EVENT_SYSPARAM_SUPPORTNUMINCREASE_NOTSAME,//�������ܺ�����һ��,
	/*END*/
	FAULT_EVENT_MAX
} FAULT_EVENT;



typedef enum 
{
	FAULT_STATUS_FAULT,
	FAULT_STATUS_NORMAL,
} FAULT_STATUS;


typedef enum 
{
	FAULT_HOST_SENSOR,//���������ͣ�ģ�������Ƕȣ��߶�
	FAULT_HOST_DEVICE,//���⣬���⣬��Ա 
	FAULT_HOST_HUB1,//HUB1
	FAULT_HOST_HUB2,//HUB2
	FAULT_HOST_HUB3,//HUB3
	FAULT_HOST_OTHER,//����
} FAULT_HOST;
	

typedef struct
{
	uint32_t     time;
	log_int      host_type; //FAULT_HOST����
	log_int      host_name; //id����ǰѹ��
	log_int      event;     //FAULT_EVENT
	log_int      status;    //FAULT_STATUS
} STR_FAULT_INFO;


/*������־*/
typedef enum 
{	
	LOG_STATUS_START=0,         //ֱ������(����)
	LOG_STATUS_STOP ,           //ֹͣ
	LOG_STATUS_DELAY_START ,    //��ʱ����
	LOG_STATUS_SUSPEND,         //��ͣ

	LOG_STATUS_INPLACE,         //��λ	
	LOG_STATUS_DISABLE,         //��ֹ
	LOG_STATUS_OVERTIME,        //��ʱ-��ʱ����
	
	LOG_STATUS_SENSOR_NORMAL,   //����������
	LOG_STATUS_SENSOR_ALARM,    //����������
	LOG_STATUS_EMERGENCY_ALARM, //��ͣ
	LOG_STATUS_EMERGENCY_NORMAL,//��ͣ�ָ�
	LOG_STATUS_BISUO_ALARM,     //����
	LOG_STATUS_BISUO_NORMAL,     //�����ָ�
	
	LOG_STATUS_DOWNLOAD_SUCCESS, //���سɹ�
	LOG_STATUS_DOWNLOAD_FAIL,    //����ʧ��
	LOG_STATUS_UPDATE_PRO_SUCCESS,//���³���ɹ�
	LOG_STAUTS_UPDATE_PRO_FAIL,   //���³���ʧ��
	LOG_STATUS_UPDATE_PARA_SUCCESS,//���²����ɹ�
	LOG_STATUS_UPDATE_PARA_FAIL, //���²���ʧ��
	LOG_STATUS_RESET_LOWPAWER,//�͹��ĸ�λ
	LOG_STATUS_RESET_WWATCHDOG,//���ڿ��Ź�
	LOG_STATUS_RESET_IWATCHDOG,//�������Ź�
	LOG_STATUS_RESET_SOFTWARE , //�����λ
	LOG_STATUS_RESET_PORRSTF,  //���縴λ
	
	LOG_STATUS_PERSONLOCK_TIMEOUT,//��Ա������ʱ
	
	LOG_STATUS_NET_REINIT,//�������³�ʼ��
	
	LOG_STATUS_START_OUT,  //�ⲿ����
	LOG_STATUS_START_IN   //�ڲ�����
	
} LOG_STATUS;



typedef enum 
{
	LOG_TYPE_HANDLE_SINGLE = 0, //����������
	LOG_TYPE_HANDLE_COMB, 		//��϶�������
	LOG_TYPE_HANDLE_GROUP,		//���鶯������
	LOG_TYPE_HANDLE_SYSTEM,		//ϵͳ����
	LOG_TYPE_SERVICE,    		//����
	LOG_TYPE_ALARM,      		//����������
	LOG_TYPE_SYSTEM_ALARM,      //ϵͳ����
	LOG_TYPE_UPDATE_PROGRAM,  //���³���
	LOG_TYPE_RESET_SYSTEM,   //ϵͳ��λ
	LOG_TYPE_JSON_TEXT   //�����ļ�����
} LOG_TYPE;

typedef struct
{
	uint32_t      time;
	log_int   	  host;	//id
	log_int   	  type;
	log_int       status;
	uint8_t       data[LOGDATASIZE];//���������ࣺ0-����Դ��ַ��1-����Դ���͡�2/3-�û�ID��4-�ֶ��Զ�
									//�����ࣺ0-����id
} STR_LOG_INFO;


//������ʾ��Ϣ�ִ�
//const uint8_t u8FaultInfoMsg[FAULT_EVENT_MAX][17]	=
//{
//	"����",
//	"����",             
//	"������ͨ���쳣",
//  "ϵͳ��Ŵ���",     
//	"SSλ�û�ʹ�ܲ���",
//	"�ܺ����򲻷�",
//	"�ܺŷ�Χ��������", 
//	"��������ʧ��",
//	"ȫ�ֲ�����һ��",   
//	"ȫ�ֲ����ڼܲ���",	
//	"����汾��һ��",   
//	"�汾�����ڼܲ���", 
//	"Ӳ�����������",	  
//	"���ڼ�ͨ�Ŵ���",   
//	"���ڼ�ͨ�Ŵ���",	  
//};                                  


#endif

