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


//设备RID,TID
enum
{
	ID_SC,
	ID_EMVD = 1,						//电磁阀驱动器1
	ID_CXB = 2,							//程序棒2
	ID_ANGLE = 3,						//角度传感器5
	ID_HIGH = 4,            //高度4
	ID_ALARM = 5,			      //声光报警器6
	ID_PERSON = 6,          //人员7
	ID_IRRX = 7,            //红外接收
	ID_KEYBOARD,
	ID_WIRELESS,
	ID_HUB1,				//HUB1  0x0A
	ID_HUB2,				//HUB2	0x0B
	ID_HUB3,				//HUB3  0x0C
	ID_ANGLE_HIGH,   //角高一体
	ID_ANGLE_ALARM, //  多功能声光
	ID_DEVICEMAX
};


/**V2外围设备can帧格式***/
__packed typedef struct
{
	__packed union
	{
		__packed struct
		{
			unsigned int	RxID:3;				//接收方ID
			unsigned int	TxID:3;				//发送方ID
			unsigned int	FrameType:10;		//帧类别	
			unsigned int	LiuShuiNumb:4;		//流水序列号，0x0-0xf循环，ACK=1应答帧的流水，采用被应答帧的流水
			unsigned int	Sum:5;		//子帧序号或
			unsigned int	Sub:1;		//组合帧/子帧，0：组合帧总数帧：1：子帧
			unsigned int	ACK:1;				//应答标志，1:应答，0:无需应答			
			unsigned int	Reservd:2;			//预留区域。用于传程序:01,其他:00	
			unsigned int	NoUsed:3;			//不存在区域
		} ID;//帧ID
		uint32_t u32Id;//帧ID
	} u32ID;	//帧ID
	uint8_t	u8DT[8];			//帧数据
	uint16_t	u16DLC;			//帧长度
} 
STR_V2CAN_Frame;



typedef struct
{
	union{
		struct{			
			unsigned int  TTL   :7;       //生存周期
			unsigned int  Addr  :5;       //目的地址
			unsigned int	Rese  :3;				//源地址
			unsigned int	Dev   :5;				//ack标志
			unsigned int	Dir   :1;
			unsigned int	Ack   :1;				//next标志
			unsigned int	Next  :1;
			unsigned int	Fun   :5;				//功能码
		  unsigned int	Ver   :1;
        } ID;
		uint32_t u32Id;
	} u32ID;
	uint8_t	     u8DT[8];			            //数据字节
	uint16_t	 u16DLC;				        //数据字节数
}
STR_V4CAN_Frame;


typedef struct
{
	uint8_t   devicetype;         //设备类型
	uint16_t  frametype;        	//帧类型
	uint16_t  frameid;            //映射号
	uint16_t  overtime;						//超时时间
	uint16_t  count;              //发送次数
	uint8_t   ack;                //应答
	uint16_t  (*rececallbacfun)(void * , STR_Can_Frame* );     //接收回调函数
	uint16_t  (*overcallbacfun)(void * , STR_Can_Frame* );     //超时回调函数
	uint16_t  (*sendiffun)(void *);                            //发送条件回调函数
	uint16_t  (*sendprofun)(void *);                           //发送前处理回调函数
}
CANPROHEAD;


//循环发送队列相关结构体
typedef struct
{
	STR_Can_Frame      SendFrame;
	CANBUSHEAD         head;
	uint16_t           timer;						//超时时间
	uint16_t           backTimer;
	uint16_t           count;           //发送次数
	uint16_t           device;
	LoopSendIfFun      sendiffun;            //发送条件回调函数
	
}
stCanLoopSend;


typedef struct
{
	stCanLoopSend      CanLoopSend[CanLoopSendQueueNum];
	uint16_t           u16CanLoopSendCnt;					            //发送链表项数			              //发送链表首项
	
}
stCanLoopSendQueue;




//外围设备相关结构体
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
	uint8_t  person;//编号
	uint8_t  personPer;//权限
	uint16_t personNum;//id
	uint16_t personDis;//距离
	uint16_t personTimeout;//数据超时时间
	uint8_t  personBattry; //电池电量
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
	uint8_t device;//设备
	uint8_t subDevice;//子设备
	uint8_t isOnline;//是否在线
	uint8_t isEnable;//是否使能
	uint8_t status;//电磁阀驱动器状态
	uint8_t HeartTimer;
	uint8_t backTimer;
	uint8_t deviceStatus;//外设当前状态
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

//ghy  增加声光上433无线模块的CAN数据处理
void  CanProSendParam_WirelessAlarm(uint16_t driver, uint16_t device, uint16_t subdevice,uint8_t imd);	
#endif


