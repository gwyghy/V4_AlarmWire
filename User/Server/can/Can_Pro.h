/*
*********************************************************************************************************
*	                                            V4
*                           Commuication Controller For The Mining Industry
*
*                                  (c) Copyright 1994-2013  HNDZ 
*                       All rights reserved.  Protected by international copyright laws.
*
*
*    File    : Can_Pro.c
*    Module  : user
*    Version : V1.0
*    History :
*   -----------------
*              Version  Date           By            Note
*              v1.0         
*
*********************************************************************************************************
*/

#ifndef __CAN_PRO_H__
#define __CAN_PRO_H__
#include "NativeCan_Driver.h"
#include "Can_Bus.h"

#define CanLoopSendQueueNum  10

#define V2TYPEDEVICE   0
#define V4TYPEDEVICE   1

#define V4CANPROFRAME          0x10000000    
#define V2CANPROFRAME_UPDATE   0x08000000
#define V2CANPROACK            0x04000000

#define  UWBPERSONIDMIN    300
#define  UWBMINEMIN   289
#define  UWBMINEMAX   299
#define  UWBDYKMIN    250

typedef uint16_t (*CanRecAckCallback)(STR_Can_Frame *);
typedef STR_Can_Frame (*LoopSendIfFun)(STR_Can_Frame *); 


//�豸RID,TID
enum
{
	ID_SC,
	ID_EMVD = 1,						//��ŷ�������1
	ID_CXB = 2,							//�����2
	ID_ANGLE = 3,						//�Ƕȴ�����5
	ID_HIGH = 4,            //�߶�4
	ID_ALARM = 5,			      //���ⱨ����6
	ID_PERSON = 6,          //��Ա7
	ID_IRRX = 7,            //�������
	ID_KEYBOARD,
	ID_WIRELESS,
	ID_HUB1,				//HUB1  0x0A
	ID_HUB2,				//HUB2	0x0B
	ID_HUB3,				//HUB3  0x0C
	ID_ANGLE_HIGH,   //�Ǹ�һ��
	ID_ANGLE_ALARM, //  �๦������
	ID_DEVICEMAX
};


/**V2��Χ�豸can֡��ʽ***/
__packed typedef struct
{
	__packed union
	{
		__packed struct
		{
			unsigned int	RxID:3;				//���շ�ID
			unsigned int	TxID:3;				//���ͷ�ID
			unsigned int	FrameType:10;		//֡���	
			unsigned int	LiuShuiNumb:4;		//��ˮ���кţ�0x0-0xfѭ����ACK=1Ӧ��֡����ˮ�����ñ�Ӧ��֡����ˮ
			unsigned int	Sum:5;		//��֡��Ż�
			unsigned int	Sub:1;		//���֡/��֡��0�����֡����֡��1����֡
			unsigned int	ACK:1;				//Ӧ���־��1:Ӧ��0:����Ӧ��			
			unsigned int	Reservd:2;			//Ԥ���������ڴ�����:01,����:00	
			unsigned int	NoUsed:3;			//����������
		} ID;//֡ID
		uint32_t u32Id;//֡ID
	} u32ID;	//֡ID
	uint8_t	u8DT[8];			//֡����
	uint16_t	u16DLC;			//֡����
} 
STR_V2CAN_Frame;



typedef struct
{
	union{
		struct{			
			unsigned int  TTL   :7;       //��������
			unsigned int  Addr  :5;       //Ŀ�ĵ�ַ
			unsigned int	Rese  :3;				//Դ��ַ
			unsigned int	Dev   :5;				//ack��־
			unsigned int	Dir   :1;
			unsigned int	Ack   :1;				//next��־
			unsigned int	Next  :1;
			unsigned int	Fun   :5;				//������
		  unsigned int	Ver   :1;
        } ID;
		uint32_t u32Id;
	} u32ID;
	uint8_t	     u8DT[8];			            //�����ֽ�
	uint16_t	 u16DLC;				        //�����ֽ���
}
STR_V4CAN_Frame;


typedef struct
{
	uint8_t   devicetype;         //�豸����
	uint16_t  frametype;        	//֡����
	uint16_t  frameid;            //ӳ���
	uint16_t  overtime;						//��ʱʱ��
	uint16_t  count;              //���ʹ���
	uint8_t   ack;                //Ӧ��
	uint16_t  (*rececallbacfun)(void * , STR_Can_Frame* );     //���ջص�����
	uint16_t  (*overcallbacfun)(void * , STR_Can_Frame* );     //��ʱ�ص�����
	uint16_t  (*sendiffun)(void *);                            //���������ص�����
	uint16_t  (*sendprofun)(void *);                           //����ǰ����ص�����
}
CANPROHEAD;


//ѭ�����Ͷ�����ؽṹ��
typedef struct
{
	STR_Can_Frame      SendFrame;
	CANBUSHEAD         head;
	uint16_t           timer;						//��ʱʱ��
	uint16_t           backTimer;
	uint16_t           count;           //���ʹ���
	uint16_t           device;
	LoopSendIfFun      sendiffun;            //���������ص�����
	
}
stCanLoopSend;


typedef struct
{
	stCanLoopSend      CanLoopSend[CanLoopSendQueueNum];
	uint16_t           u16CanLoopSendCnt;					            //������������			              //������������
	
}
stCanLoopSendQueue;




//��Χ�豸��ؽṹ��
typedef struct
{
	uint16_t angle_x;
	uint16_t angle_y;
	uint16_t angle_z;
}
stAngleAppMsg;


enum
{
	ANALOG_1 =0,
	ANALOG_2,
	ANALOG_3,
	ANALOG_4
};

enum
{
	ANGLE_X,
	ANGLE_Y,
	ANGLE_Z
};

typedef struct
{
	uint16_t high;
}
stHighAppMsg;

typedef struct
{
	uint8_t  person;//���
	uint8_t  personPer;//Ȩ��
	uint16_t personNum;//id
	uint16_t personDis;//����
	uint16_t personTimeout;//���ݳ�ʱʱ��
	uint8_t  personBattry; //��ص���
}
stPersonAppMsg;

enum
{
	PERSONMSG,
	PERSONNUM,
	PERSONDIS,
	PERSONBATTRY,
	PERSONTIMEOUT
};

enum
{
	PERSON_GRADES_0 = 0,	
	PERSON_GRADES_1 = 1,
	PERSON_GRADES_2 = 2,
	PERSON_GRADES_3 = 3,
	PERSON_GRADES_15 = 15,
};

enum
{
	PERSONINTO = 1,
	PROSONLEFT = 2,
	PERSONFAULT = 3
};

enum
{
	EMVD_AMARM_STOP = 0,
	EMVD_AMARM_ACTION = 1
};


typedef struct
{
	uint16_t irCounts;
}
stIRAppMsg;


typedef struct
{
	uint32_t iovalue;
	uint8_t status;
	uint8_t flag;
}
stEmvdAppMsg;

enum
{
	EMVDIO = 1,
	EMVDSTATUS = 2,
	EMVDFLAG = 3
};

enum
{
	ALARM_NO = 0,
	ALARM_BEEP = 1,
	ALARM_LED = 2,
	ALARM_BEEP_LED = 3
};

enum
{
	BEEP_NEVER  = 0,
	BEEP_ALWAYS = 1,
	BEEP_200400 = 0x2040,
	BEEP_100200 = 0x1020,
};
enum
{
	BLUE_RED_NEVER  = 0,
	BLUE_RED_ALWAYS = 1,
	RED_ALWAYS      = 2,
	BLUE_ALWAYS     = 3,
	RED20_BLUE20    = 0x2020,
};

typedef struct
{
	uint32_t alarmMode[3];
}
stAlarmAppMsg;


typedef struct
{
	uint16_t angle_x;
	uint16_t angle_y;
	uint16_t angle_z;
}
stAngleAlarmAppMsg;

typedef struct
{
	uint16_t angle_x;
	uint16_t angle_y;
	uint16_t angle_z;
}stHighAngleAppMsg;

typedef struct
{
	uint16_t analog1;
	uint16_t analog2;
	uint16_t analog3;
	uint16_t analog4;
	stAngleAppMsg hubangle[4];
}
stMiniHubAppMsg;


typedef struct
{
	stEmvdAppMsg    emvdappmsg[1];
	stAngleAppMsg   angleappmsg[6];
	stHighAppMsg    hgihappmsg[1];
	stPersonAppMsg  personappmsg[11]; 
	stIRAppMsg      irappmsg[1];
	stAlarmAppMsg   alarmappmsg[1];
	stAngleAlarmAppMsg anglealarmappMsg[1];
	stHighAngleAppMsg  highangleappMsg[2]; 
	stMiniHubAppMsg    minihubappmsg[3];
}
stDeviceAppMsg;

typedef struct
{
	uint8_t driver;
	stDeviceAppMsg deviceAppMsg;
}
stDeviceAppMsgQueue;

enum{
	DEVICENORMAL = 0,
	DEVICEINIT = 1,
	DEVICEPROGRAM = 2,
};


enum{
	DELETEDATA,
	INSERTADATA,
	MODIFYDATA,
	NODEALDATA,
	DELETEEMVDDATA
};

typedef struct
{
	uint8_t device;//�豸
	uint8_t subDevice;//���豸
	uint8_t isOnline;//�Ƿ�����
	uint8_t isEnable;//�Ƿ�ʹ��
	uint8_t status;//��ŷ�������״̬
	uint8_t HeartTimer;
	uint8_t backTimer;
	uint8_t deviceStatus;//���赱ǰ״̬
	uint8_t deviceVersion[3];
	uint8_t deviceBLVersion[3];
}
stDeviceStatusMsg;


enum
{
	EMVD_Version = 0,
	ANGLE0_Version = 1,
	ANGLE1_Version = 2,
	ANGLE2_Version = 3,
	ANGLE3_Version = 4,
	ANGLE4_Version = 5,
	ANGLE5_Version = 6,
	HIGH_Verson = 7,
	ALARM_Version = 8,
	PERSON_Version = 9,
	IR_Verson = 10,
	CXB_Version = 11,
	KEYBOARD_Version = 12,
	WIRELESS_Version = 13,
	ALARMANGLE_Version =14,
	HIGHANGLEBOTTOM_Version =15,
	HIGHANGLETOP_Version =16,
	HUB1_Version =17,
	HUB1_ANGLE1_Version =18,
	HUB1_ANGLE2_Version =19,
	HUB1_ANGLE3_Version =20,
	HUB1_ANGLE4_Version =21,
	HUB2_Version = 22,
	HUB2_ANGLE1_Version =23,
	HUB2_ANGLE2_Version =24,
	HUB2_ANGLE3_Version =25,
	HUB2_ANGLE4_Version =26,
	HUB3_Version =27,
	HUB3_ANGLE1_Version =28,
	HUB3_ANGLE2_Version =29,
	HUB3_ANGLE3_Version =30,
	HUB3_ANGLE4_Version =31,
	DEVICE_MAX_Version = 32
	
};

typedef struct
{
	uint8_t driver;
	stDeviceStatusMsg deviceStatusMsgQueue[DEVICE_MAX_Version];
}
stDeviceStatusMsgQueue;







void CanRevTimeoutProc(STR_Can_Frame* RevBufTemp);
void CanProInit(uint16_t* device, uint16_t* bunds,uint8_t num);
void CanRec_task(void);
void CanProSend(uint16_t driver,uint16_t device, uint16_t subDev, uint16_t point, uint8_t data, uint8_t flag);
void CanProDirectSend(uint16_t driver, uint16_t device, uint16_t subDev, uint16_t point, uint8_t* data, uint8_t len, uint32_t packetNum);
void CanProLoopSend(uint16_t driver,uint16_t device, uint16_t subDev, uint16_t point, uint16_t status, uint8_t interval, uint8_t count, LoopSendIfFun sendiffun);
void CanProSetParam(uint16_t device, void * param,uint8_t isNow);
void CanProSendParam(uint16_t driver, uint16_t device, uint16_t subdevice);
void SetCanDeviceAPPMsg(uint16_t driver, uint16_t device, uint16_t subDev, uint16_t point, uint32_t data);
void GetCanDeviceAPPMsg(uint16_t driver, uint16_t device, uint16_t subDev, uint16_t point, uint32_t* data);
void SetCanDeviceStatusMsg(uint16_t driver, uint16_t device, uint16_t subdevice, stDeviceStatusMsg* deviceMsg);
void GetCanDeviceStatusMsg(uint16_t driver, uint16_t device, uint16_t subdevice, stDeviceStatusMsg* deviceMsg);
uint8_t GetDeviceCanMsg(uint16_t device, uint16_t subDev, uint8_t *driver);

//ghy  ����������433����ģ���CAN���ݴ���
void  CanProSendParam_WirelessAlarm(uint16_t driver, uint16_t device, uint16_t subdevice,uint8_t imd);	
#endif


