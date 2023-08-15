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
	/*********************数据采集控制*************************/
	AppProcIDType_readState_request = 1,       //读状态请求（单一结构）
	AppProcIDType_readState_confirm,           //读状态应答（单一结构）
	AppProcIDType_readState_multiple_request,  //读状态请求（复合结构）
	AppProcIDType_readState_multiple_confirm,  //读状态应答（复合结构）
	AppProcIDType_stateReport_indication,      //状态上报指示
	AppProcIDType_stateReport_confirm,	       //状态上报应答	
	AppProcIDType_control_request,             //控制设备请求
	AppProcIDType_control_confirm,             //控制设备应答
	AppProcIDType_manualControl_request,       //手动控制设备请求    
	AppProcIDType_logReport_indication,        //日志上报指示   
	AppProcIDType_writeState_request,          //写状态    
	AppProcIDType_followControl_request,       //跟机启动请求
	AppProcIDType_followControl_confirm,       //跟机启动应答
	AppProcIDType_ykqDecode_request,           //遥控器解除对码请求
	AppProcIDType_arpNetMaintain_request,      //维护心跳请求-维护arp的map表
	AppProcIDType_arpNetMaintain_confirm,      //维护心跳请求
	AppProcIDType_RecoilControl_request,	   //反冲洗启动请求
	AppProcIDType_RecoilControl_confirm,	   //反冲洗应答请求
    AppProcIDType_TransportPlane_request,
	/*********************参数传输*************************/	
	AppProcIDType_getParam_request = 101,        //获取参数请求（单一结构）
	AppProcIDType_getParam_confirm,              //获取参数应答（单一结构）
	AppProcIDType_getParam_multiple_request,     //获取参数请求（复合结构）
	AppProcIDType_getParam_multiple_confirm,     //获取参数应答（复合结构）
	AppProcIDType_setParam_request,              //设置参数请求（单一结构）
	AppProcIDType_setParam_confirm,              //设置参数应答（单一结构）
	AppProcIDType_setParam_multiple_request,     //设置参数请求（复合结构）
	AppProcIDType_setParam_multiple_confirm,     //设置参数应答（复合结构）
	AppProcIDType_checkParam_request,            //参数检查请求
	AppProcIDType_checkParam_confirm,            //参数检查应答
	AppProcIDType_setParam_UIMode_request,       //设置参数请求（单一结构）界面方式设置参数
	AppProcIDType_serverOperate_request,         //服务类操作请求--恢复出厂默认参数
	AppProcIDType_serverDisplays_request,        //显示服务器参数请求
	AppProcIDType_serverDisplays_confirm,	       //显示服务器参数应答
	AppProcIDType_serverRevise_request,          //修改服务器参数请求
	AppProcIDType_serverRevise_confirm,          //修改服务器参数应答
	AppProcIDType_serverINS_request,             //惯导找直数据下发与清除
	AppProcIDType_serverINS_confirm,             //惯导找直数据应答
	AppProcIDType_sysParamSynchronous_requset,    //服务器参数同步
	
	
	/*********************网络维护*************************/		
	AppProcIDType_enterCfgMode_request = 201,    //强制进入配置模式指示
	AppProcIDType_enterCfgMode_confirm,          //强制进入配置模式应答
	AppProcIDType_setAddr_request,               //设置本架地址指示
	AppProcIDType_setAddr_confirm,               //设置本架地址应答
	AppProcIDType_netHeart_request,              //网络心跳指示
	AppProcIDType_netHeart_confirm,              //网络心跳应答
	AppProcIDType_timeSync_request,              //网络时间同步指示
	AppProcIDType_timeSync_confirm,	             //网络时间同步应答
	AppProcIDType_commCheck_request,             //邻架通信检测指示
	AppProcIDType_commCheck_confirm,             //邻架通信检测应答
	
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
