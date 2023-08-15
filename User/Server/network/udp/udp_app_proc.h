#ifndef __UDP_APP_PROC_H
#define __UDP_APP_PROC_H

#include "udp_datalink.h"
#include "udp_app_bcm.h" 

#define APPPROC_TIMEOUT_BASE      500  //200ms
#define APPPROC_REPEATNUM_BASE    2  
#define APPPROC_FRAME_IDLENGTH    2

#define IP_ADDR_SUPPORT(ipaddr)        (u8)(ipaddr.addr >>24)

#define APPPROC_DEV_CONTROLLER_NUM    254
#define APPPROC_DEV_SERVER_NUM		  1
#define APPPROC_DEV_DATATRANS_NUM	  32
#define APPPROC_DEV_OFFLINE_ERRNUM	  10
#define APPPROC_USER_FRAME_LEN        (UDP_FRAME_LENGTH_MAX-UDP_FRAME_HEADER_LEN -APPPROC_FRAME_IDLENGTH)

#define DEVICE_SELFID_MAX             12

#define APPPROC_CHECK_TIME			  6000
#define APPPROC_SELF_PARAM_LEN        2

enum
{
	/*********************���ݲɼ�����*************************/
	AppProcIDType_readState_request = 1,       //��״̬���󣨵�һ�ṹ��
	AppProcIDType_readState_confirm,           //��״̬Ӧ�𣨵�һ�ṹ��
	AppProcIDType_readState_multiple_request,  //��״̬���󣨸��Ͻṹ��
	AppProcIDType_readState_multiple_confirm,  //��״̬Ӧ�𣨸��Ͻṹ��
	AppProcIDType_stateReport_indication,      //״̬�ϱ�ָʾ
	AppProcIDType_stateReport_confirm,	       //״̬�ϱ�Ӧ��	
	AppProcIDType_control_request,             //�����豸����
	AppProcIDType_control_confirm,             //�����豸Ӧ��
	AppProcIDType_manualControl_request,       //�ֶ������豸����    
	AppProcIDType_logReport_indication,        //��־�ϱ�ָʾ   
	AppProcIDType_writeState_request,          //д״̬    
	AppProcIDType_followControl_request,       //������������
	AppProcIDType_followControl_confirm,       //��������Ӧ��
	AppProcIDType_ykqDecode_request,           //ң���������������
	AppProcIDType_arpNetMaintain_request,      //ά����������-ά��arp��map��
	AppProcIDType_arpNetMaintain_confirm,      //ά����������
	AppProcIDType_RecoilControl_request,	   //����ϴ��������
	AppProcIDType_RecoilControl_confirm,	   //����ϴӦ������
    AppProcIDType_TransportPlane_request,
	/*********************��������*************************/	
	AppProcIDType_getParam_request = 101,        //��ȡ�������󣨵�һ�ṹ��
	AppProcIDType_getParam_confirm,              //��ȡ����Ӧ�𣨵�һ�ṹ��
	AppProcIDType_getParam_multiple_request,     //��ȡ�������󣨸��Ͻṹ��
	AppProcIDType_getParam_multiple_confirm,     //��ȡ����Ӧ�𣨸��Ͻṹ��
	AppProcIDType_setParam_request,              //���ò������󣨵�һ�ṹ��
	AppProcIDType_setParam_confirm,              //���ò���Ӧ�𣨵�һ�ṹ��
	AppProcIDType_setParam_multiple_request,     //���ò������󣨸��Ͻṹ��
	AppProcIDType_setParam_multiple_confirm,     //���ò���Ӧ�𣨸��Ͻṹ��
	AppProcIDType_checkParam_request,            //�����������
	AppProcIDType_checkParam_confirm,            //�������Ӧ��
	AppProcIDType_setParam_UIMode_request,       //���ò������󣨵�һ�ṹ�����淽ʽ���ò���
	AppProcIDType_serverOperate_request,         //�������������--�ָ�����Ĭ�ϲ���
	AppProcIDType_serverDisplays_request,        //��ʾ��������������
	AppProcIDType_serverDisplays_confirm,	       //��ʾ����������Ӧ��
	AppProcIDType_serverRevise_request,          //�޸ķ�������������
	AppProcIDType_serverRevise_confirm,          //�޸ķ���������Ӧ��
	AppProcIDType_serverINS_request,             //�ߵ���ֱ�����·������
	AppProcIDType_serverINS_confirm,             //�ߵ���ֱ����Ӧ��
	AppProcIDType_sysParamSynchronous_requset,    //����������ͬ��
	
	
	/*********************����ά��*************************/		
	AppProcIDType_enterCfgMode_request = 201,    //ǿ�ƽ�������ģʽָʾ
	AppProcIDType_enterCfgMode_confirm,          //ǿ�ƽ�������ģʽӦ��
	AppProcIDType_setAddr_request,               //���ñ��ܵ�ַָʾ
	AppProcIDType_setAddr_confirm,               //���ñ��ܵ�ַӦ��
	AppProcIDType_netHeart_request,              //��������ָʾ
	AppProcIDType_netHeart_confirm,              //��������Ӧ��
	AppProcIDType_timeSync_request,              //����ʱ��ͬ��ָʾ
	AppProcIDType_timeSync_confirm,	             //����ʱ��ͬ��Ӧ��
	AppProcIDType_commCheck_request,             //�ڼ�ͨ�ż��ָʾ
	AppProcIDType_commCheck_confirm,             //�ڼ�ͨ�ż��Ӧ��
	
	AppProcIDType_MAX
};

enum
{
	AppProcServerType_reset_controller=1,
	AppProcServerType_reset_defaultParam,
	AppProcServerType_MAX
};

enum
{
	APPDEV_STATE_OFFLINE=0,
	APPDEV_STATE_ONLINE =1
};

enum
{
	APPPROC_MODE_NORMAL=0,
	APPPROC_MODE_CONFIG,
	APPPROC_MODE_IPRESQUEST,
	APPPROC_MODE_MAX
};

enum
{
	DEVICE_SELFID=0,
	DEVICE_SELFID_LEFT,
	DEVICE_SELFID_RIGHT
};

enum
{
	DEVICE_DIRECT_LEFT =0,
	DEVICE_DIRECT_RIGHT
};

typedef enum 
{	
	AppProcCtrl_readState_request = 0,
	AppProcCtrl_readState_multiple_request,
	AppProcCtrl_stateReport_indication,
	AppProcCtrl_control_request,
	AppProcCtrl_manualControl_request,
	AppProcCtrl_logReport_indication,
	
	AppProcCtrl_getParam_request,
	AppProcCtrl_getParam_multiple_request,
	AppProcCtrl_setParam_request,
	AppProcCtrl_setParam_multiple_request,
	AppProcCtrl_checkParam_request,
	AppProcCtrl_setParam_UIMode_request,
	AppProcCtrl_serverOperate_request,
	
	AppProcCtrl_enterCfgMode_request,
	AppProcCtrl_setAddr_request,
	AppProcCtrl_netHeart_request,
	AppProcCtrl_timeSync_request,
	AppProcCtrl_commCheck_request,
	
	AppProcCtrl_ykqDecode_request,
	AppProcCtrl_arpNetMaintain_request,
	
	AppProcCtrl_MSG_MAX
}AppProcCtrl_t;

typedef struct 
{
	u8 devOnLine;
	u8 devEnable;
	u8 errCnt;
	u8 devType;
}AppDevType;

typedef struct
{
	u16 configTimerId;
	u16 setAddrTimerId;
	u16 configClearTimerId;
	u8  selfID[DEVICE_SELFID_MAX];
	u8  selfID_Left [DEVICE_SELFID_MAX];
	u8  selfID_Right[DEVICE_SELFID_MAX];
	u8  selfAddr;
	int lefttimer;
	int righttimer;
	u8  selfParam[APPPROC_SELF_PARAM_LEN];
	u8  direct;
}AppProcInfoType;


/**************************support comm *******************************************/
u8 UdpAppProcFunc(UDPProcType *proc, UDPFrameType *pframe);
u8 UdpAppProcCtrl(AppProcCtrl_t ctrlMsg,void *pdata,u16 length,u8 addr);
u8 UdpLinkLayerSend_app(u8 *pdata,u16 length,u8 ack,u16 sn,u8 supportAddr,u16 funcid);
u8 UdpLinkLayerSend_app_vlan(u8 *pdata,u16 length,u8 ack,u16 sn,u8 supportAddr,u16 funcid,u8 vlan);
void setAppDevOnLine(u8 type,u8 addr);
void setAppDevOffLine(u8 type,u8 addr);
void udpAppProcInit(void);
void setAppProcMode(u8 mode);
u8 getAppProcMode(void);
void setSelfID(u8 device,u8 *id,u8 sel);
u8 * getSelfID(u8 device);
void setSelfAddr(u8 addr,u8 sel);
u8 getSelfAddr(void);
void UdpAppCommCheckFunc(u16 delay);
u8 getAppDevOnLine(u8 type,u8 addr,u8 *state);
void setAppProcDirect(u8 direct);

void TimerCallback_enterCfgMode(u16 data);
void setAppProcDeviceType(u8 mode);
u8 getAppProcDeviceType(void);
void TimerCallback_setAddr(u16 data);
void clearUdpAppProcTimer(u8 direct);
void UdpAppProcTimerProcess(u16 delay);
u8 getUdpAppSelfParamInfo(u8 *pParam,u8 len);
u8 setUdpAppSelfParamInfo(u8 *pParam,u8 len);

/**************************update /phone *******************************************/
#define UPDATEPROC_TIMEOUT_BASE      2  
#define UPDATEPROC_REPEATNUM_BASE    1 

enum
{
	UpdateProcIDType_UpdateStart_request = 1,
	UpdateProcIDType_SendData_request,
	UpdateProcIDType_RecvDataAck_request,
	UpdateProcIDType_DataCRC_request,
	UpdateProcIDType_DataCRC_confirm,
	UpdateProcIDType_UpdateNow_request,
	
	UpdateProcIDType_MAX
};

typedef struct 
{	
	u8 devtype;
	u8 ack;
	u8 totalNum;
	u8 curNum;	
}udpUpdateApiType;

u8 UdpUpdateProcFunc(UDPProcType *proc, UDPFrameType *pframe);
u8 UdpUpdateProcCtrl(u16 ctrlMsg,void *pdata,u16 length,u8 addr,udpUpdateApiType frameInfo);

#endif
