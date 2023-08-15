/*
*********************************************************************************************************
*	                                            DoubleWires
*                           Commuication Controller For The Mining Industry
*
*                                  (c) Copyright 1994-2013  HNDZ 
*                       All rights reserved.  Protected by international copyright laws.
*
*
*    File    : udp_app_proc.c
*    Module  : user/udp
*    Version : V1.1.0.0
*    Brief   : STM32F4xx DCP
*    History : 
*   -----------------
*              Version  Date           By            Note
*              v1.0        
*
*********************************************************************************************************
 */

#include "udp_app_proc.h"
#include "udp_app_callback.h"
#include "udp_update_callback.h"
#include "memory.h"
#include "Util_Timer.h"
#include "netmain.h"
#include "FileLoad.h"
#include "sys_msg_def.h"
#include "ProjectConfig.h"

#if (SERVER_NETWORK_UDP_EN ==1)

static const UDPProcType AppProcInfo[]={
	
	/*******************数据采集控制********************/
	//------------------1 读状态请求（单一结构）---------
	{
		AppProcIDType_readState_request,
		APPPROC_TIMEOUT_BASE,
		APPPROC_REPEATNUM_BASE,
		udpAppCB_readState_request,
		udpAppCB_request_outtime
	},
	
	//------------------2 读状态应答（单一结构）---------
	{
		AppProcIDType_readState_confirm,
		0,
		0,
		udpAppCB_readState_confirm,
		NULL
	},
	
	//------------------3 读状态请求（复合结构）---------
	{
		AppProcIDType_readState_multiple_request,
		APPPROC_TIMEOUT_BASE,
		APPPROC_REPEATNUM_BASE,
		udpAppCB_readState_multiple_request,
		udpAppCB_request_outtime
	},
	
	//------------------4 读状态应答（复合结构）---------
	{
		AppProcIDType_readState_multiple_confirm,
		0,
		0,
		udpAppCB_readState_multiple_confirm,
		NULL
	},
	
	//------------------5 状态上报指示 -------------------
	{
		AppProcIDType_stateReport_indication,
		APPPROC_TIMEOUT_BASE,
		0, //APPPROC_REPEATNUM_BASE,
		udpAppCB_stateReport_indication,
		udpAppCB_request_outtime
	},
	
	//------------------6 状态上报应答 (略)-------------------
	{
		AppProcIDType_stateReport_confirm,
		APPPROC_TIMEOUT_BASE,
		0,//APPPROC_REPEATNUM_BASE,
		NULL,
		NULL
	},
	
	//------------------7 控制设备请求 -------------------
	{
		AppProcIDType_control_request,
		APPPROC_TIMEOUT_BASE,
		APPPROC_REPEATNUM_BASE,
		udpAppCB_controlDevice_request,
		udpAppCB_request_outtime
	},
	
	//------------------8 控制设备应答 -------------------
	{
		AppProcIDType_control_confirm,
		0,
		0,
		udpAppCB_request_ack,
		NULL
	},

	//------------------9 手动控制设备请求 -------------------
	{
		AppProcIDType_manualControl_request,
		0,//APPPROC_TIMEOUT_BASE,
		0,//APPPROC_REPEATNUM_BASE,
		udpAppCB_manualControlDevice_request,
		NULL
	},
	
	//------------------10 日志上报指示 -------------------
	{
		AppProcIDType_logReport_indication,
		APPPROC_TIMEOUT_BASE,
		0, //APPPROC_REPEATNUM_BASE,
		NULL,
		NULL
	},	
	
	//------------------11 写状态 -------------------
	{
		AppProcIDType_writeState_request,
		0,
		0, //APPPROC_REPEATNUM_BASE,
		udpAppCB_writeStateDevice_request,
		NULL
	},	
	
	//------------------12 跟机启动请求 -------------------
	{
		AppProcIDType_followControl_request,
		APPPROC_TIMEOUT_BASE,
		APPPROC_REPEATNUM_BASE,
		NULL,
		NULL
	},
	//------------------13 跟机启动应答 -------------------
	{
		AppProcIDType_followControl_confirm,
		0,
		0,
		udpAppCB_followControlDevice_confirm,
		NULL
	},
	
	//------------------14 遥控器解除对码请求 -------------------
	{
		AppProcIDType_ykqDecode_request,
		0,
		0,
		udpAppCB_ykqDecode_request,
		NULL
	},
	
		//------------------15 维护心跳请求 -------------------
	{
		AppProcIDType_arpNetMaintain_request,
		0,
		0,
		udpAppCB_arpNetMaintain_request,
		NULL
	},
	
		//------------------16 维护心跳请求 -------------------
	{
		AppProcIDType_arpNetMaintain_confirm,
		0,
		0,
		NULL,
		NULL
	},
		//------------------17 反冲洗启动请求 -------------------
	{
		AppProcIDType_RecoilControl_request,
		APPPROC_TIMEOUT_BASE,
		APPPROC_REPEATNUM_BASE,
		NULL,
		NULL
	},
		//------------------18 反冲洗应答请求 -------------------
	{
		AppProcIDType_RecoilControl_confirm,
		0,
		0,
		udpAppCB_recoilControlDevice_confirm,
		NULL
	},
	//------------------19 运输机电流请求 -------------------
	{
		AppProcIDType_TransportPlane_request,
		0,
		0,
		udpAppCB_TransportPlane_request,
		NULL
	},
	
	
	/*******************参数传输*************************/
	//------------------101 获取参数请求（单一结构）------
	{
		AppProcIDType_getParam_request,
		APPPROC_TIMEOUT_BASE,
		APPPROC_REPEATNUM_BASE,
		udpAppCB_readState_request,
		udpAppCB_request_outtime
	},
	
	//------------------102 获取参数应答（单一结构）------
	{
		AppProcIDType_getParam_confirm,
		0,
		0,
		udpAppCB_readState_confirm,
		NULL
	},
	
	//------------------103 获取参数请求（复合结构）------
	{
		AppProcIDType_getParam_multiple_request,
		APPPROC_TIMEOUT_BASE,
		APPPROC_REPEATNUM_BASE,
		udpAppCB_readState_multiple_request,
		udpAppCB_request_outtime
	},
	
	//------------------104 获取参数应答（复合结构）------
	{
		AppProcIDType_getParam_multiple_confirm,
		0,
		0,
		udpAppCB_readState_multiple_confirm,
		NULL
	},
	
	//------------------105 设置参数请求（单一结构）------
	{
		AppProcIDType_setParam_request,
		APPPROC_TIMEOUT_BASE,
		APPPROC_REPEATNUM_BASE,
		udpAppCB_setParam_request,
		udpAppCB_request_outtime
	},
	
	//------------------106 设置参数应答（单一结构）------
	{
		AppProcIDType_setParam_confirm,
		0,
		0,
		udpAppCB_request_ack,
		NULL
	},
	
	//------------------107 设置参数请求（复合结构）------
	{
		AppProcIDType_setParam_multiple_request,
		APPPROC_TIMEOUT_BASE,
		APPPROC_REPEATNUM_BASE,
		udpAppCB_setParam_multiple_request,
		udpAppCB_request_outtime
	},
	
	//------------------108 设置参数应答（复合结构）------
	{
		AppProcIDType_setParam_multiple_confirm,
		0,
		0,
		udpAppCB_request_ack,
		NULL
	},
	
	//------------------109 参数检查请求------------------
	{
		AppProcIDType_checkParam_request,
		APPPROC_TIMEOUT_BASE,
		APPPROC_REPEATNUM_BASE,
		udpAppCB_checkParam_request,
		udpAppCB_request_outtime
	},
	
	//------------------110 参数检查应答-------------------
	{
		AppProcIDType_checkParam_confirm,
		0,
		0,
		udpAppCB_checkParam_confirm,
		NULL
	},
	
    //------------------111 设置参数请求（单一结构）界面方式设置参数------
	{
		AppProcIDType_setParam_UIMode_request,
		APPPROC_TIMEOUT_BASE,//APPPROC_TIMEOUT_BASE,
		1,//APPPROC_REPEATNUM_BASE,
		udpAppCB_setParam_UIMode_request,
		NULL
	},
	
	//------------------112 服务类操作请求--恢复出厂默认参数------------------
	{
		AppProcIDType_serverOperate_request,
		0,//APPPROC_TIMEOUT_BASE,
		0,//APPPROC_REPEATNUM_BASE,
		udpAppCB_serverOperate_request,
		NULL
	},
	
	    //------------------113 显示服务器参数请求------
	{
		AppProcIDType_serverDisplays_request,
		0,//APPPROC_TIMEOUT_BASE,
		0,//APPPROC_REPEATNUM_BASE,
		NULL,
		NULL
	},
	
		//------------------114 显示服务器参数应答------------------
	{
		AppProcIDType_serverDisplays_confirm,
		0,//APPPROC_TIMEOUT_BASE,
		0,//APPPROC_REPEATNUM_BASE,
		udpAppCB_serverDisplays_confirm,
		NULL
	},
	    //------------------115 修改服务器参数请求------
	{
		AppProcIDType_serverRevise_request,
		0,//APPPROC_TIMEOUT_BASE,
		0,//APPPROC_REPEATNUM_BASE,
		NULL,
		NULL
	},	
	
		//------------------116 修改服务器参数应答------------------
	{
		AppProcIDType_serverRevise_confirm,
		0,//APPPROC_TIMEOUT_BASE,
		0,//APPPROC_REPEATNUM_BASE,
		udpAppCB_serverRevise_confirm,
		NULL
	},
		//------------------117 惯导找直数据下发与清除------------------
	{
		AppProcIDType_serverINS_request,
		0,//APPPROC_TIMEOUT_BASE,
		0,//APPPROC_REPEATNUM_BASE,
		udpAppCB_serverINS_request,
		NULL
	},
		//------------------118 惯导找直数据应答------------------
	{
		AppProcIDType_serverINS_confirm,
		0,//APPPROC_TIMEOUT_BASE,
		0,//APPPROC_REPEATNUM_BASE,
		NULL,
		NULL
	},
		//------------------119 服务器参数同步------------------
	{
		AppProcIDType_sysParamSynchronous_requset,
		0,//APPPROC_TIMEOUT_BASE,
		0,//APPPROC_REPEATNUM_BASE,
		udpAppCB_sysParamSynchronous_request,
		NULL
	},	
	
	

	
	/*******************网络维护*************************/
	//------------------201 强制进入配置模式-------------
	{
		AppProcIDType_enterCfgMode_request,
		100,//APPPROC_TIMEOUT_BASE,
		1,//APPPROC_REPEATNUM_BASE,
		udpAppCB_enterCfgMode_request,
		NULL
	},
	
	//------------------203 设置本架地址请求-----------------
	{
		AppProcIDType_setAddr_request,
//		APPPROC_TIMEOUT_BASE,
//		APPPROC_REPEATNUM_BASE,
		0,
		0,
		udpAppCB_setAddr_request,
		NULL
	},
	
	//------------------204 设置本架地址应答-----------------
	{
		AppProcIDType_setAddr_confirm,
		0,
		0,
		udpAppCB_setAddr_confirm,
		NULL
	},
	
	//------------------205 网络心跳请求-----------------
	{
		AppProcIDType_netHeart_request,
		APPPROC_TIMEOUT_BASE,
		APPPROC_REPEATNUM_BASE,
		udpAppCB_netHeart_request,
		udpAppCB_request_outtime
	},
	
	//------------------206 网络心跳应答-----------------
	{
		AppProcIDType_netHeart_confirm,
		0,
		0,
		udpAppCB_request_ack,
		0
	},
	
	//------------------207 时间同步请求-----------------
	{
		AppProcIDType_timeSync_request,
		APPPROC_TIMEOUT_BASE,
		APPPROC_REPEATNUM_BASE,
		udpAppCB_timeSync_request,
		NULL
	},
	
	//------------------208 时间同步应答-----------------
	{
		AppProcIDType_timeSync_confirm,
		0,
		0,
		udpAppCB_request_ack,
		NULL
	},
	
	//------------------209 邻架通信检测请求-----------------
	{
		AppProcIDType_commCheck_request,
		0,//APPPROC_TIMEOUT_BASE,
		0,//APPPROC_REPEATNUM_BASE,
		udpAppCB_commCheck_request,
		NULL//udpAppCB_request_outtime
	},
	
	//------------------210 邻架通信检测应答-----------------
	{
		AppProcIDType_commCheck_confirm,
		0,
		0,
		udpAppCB_commCheck_confirm,
		NULL
	},
};

static const UDPProcType UpdateProcInfo[]={
	
	//------------------1 文件传输开始---------
	{
		UpdateProcIDType_UpdateStart_request,
		0,
		0,
		udpUpdateCB_UpdateStart_request,
		NULL
	},
	//------------------2 文件数据传输---------
	{
		UpdateProcIDType_SendData_request,
//		UPDATEPROC_TIMEOUT_BASE,
//		UPDATEPROC_REPEATNUM_BASE,
		0,
		0,		
		udpUpdateCB_SendData_request,
		NULL
	},
	//------------------3 文件数据应答---------
	{
		UpdateProcIDType_RecvDataAck_request,
		0,
		0,
		udpUpdateCB_RecvDataAck_request,
		NULL
	},
	//------------------4 文件最后校验---------
	{
		UpdateProcIDType_DataCRC_request,
		0,
		0,
		udpUpdateCB_DataCRC_request,
		NULL
	},
	//------------------5 文件最后校验结果--------
	{
		UpdateProcIDType_DataCRC_confirm,
		0,
		0,
		udpUpdateCB_DataCRC_confirm,
		NULL
	},
	//------------------6 是否立即更新---------
	{
		UpdateProcIDType_UpdateNow_request,
		0,
		0,
		udpUpdateCB_UpdateNow_request,
		NULL
	},
};

static AppDevType AppDevController[APPPROC_DEV_CONTROLLER_NUM]={0};
static AppDevType AppDevServer	  [APPPROC_DEV_SERVER_NUM]={0};
static AppDevType AppDevDataTrans [APPPROC_DEV_DATATRANS_NUM]={0};
static u8 AppProcMode=APPPROC_MODE_NORMAL;
static u8 AppProcDeviceType=UDP_DEVICE_CONTROLLER;
AppProcInfoType AppProcData;
static u8 udpUpdateBuf[520];

extern void setVlanBcm(u8 vlanMode);

/**************************support comm *******************************************/
void udpAppProcInit(void)
{
	u8 addr[4];
	u8 selfid[DEVICE_SELFID_MAX];
	
	//memset(&AppProcData,0,sizeof(AppProcInfoType));
	memset(selfid,0,DEVICE_SELFID_MAX);
//	UtilTimerInit(TIM5);
	GetCurrentIPAddr(addr);
	setSelfAddr(addr[3],0);
	setBcmInfo_vlanType(SysNetParam.Vlan);
	setAppProcDeviceType(SysNetParam.deviceType);

	Get_MCU_ID(selfid);
	setSelfID(DEVICE_SELFID,selfid,0);
	setSelfID(DEVICE_SELFID_LEFT,SysNetParam.LeftSelfID,0);
	setSelfID(DEVICE_SELFID_RIGHT,SysNetParam.RightSelfID,0);
	setAppProcDirect(SysParam.globalParam.SupportParam.SupportDirect);
}

void setAppProcMode(u8 mode)
{
	if(mode >=APPPROC_MODE_MAX)
		return ;
	
	AppProcMode=mode;
}

u8 getAppProcMode(void)
{
	return AppProcMode;
}

void setAppProcDirect(u8 direct)
{
	if(direct ==0)
		AppProcData.direct =0;
	else
		AppProcData.direct =1;
}

void setAppProcDeviceType(u8 mode)
{
	if(mode >=UDP_DEVICE_MAX)
		return ;
	
	AppProcDeviceType=mode;
}

u8 getAppProcDeviceType(void)
{
	return AppProcDeviceType;
}

void setAppDevOnLine(u8 type,u8 addr)
{
	if(addr==0)
		return;
	
	if(type==UDP_DEVICE_SERVER)
	{
		if(addr <= APPPROC_DEV_SERVER_NUM)
		{
			AppDevServer[addr-1].devOnLine = APPDEV_STATE_ONLINE;
			AppDevServer[addr-1].errCnt =0;
		}
	}
	else if(type==UDP_DEVICE_DATATRANS)
	{
		if(addr <= APPPROC_DEV_DATATRANS_NUM)
		{
			AppDevDataTrans[addr-1].devOnLine = APPDEV_STATE_ONLINE;
			AppDevDataTrans[addr-1].errCnt =0;
		}
	}
	else if((type==UDP_DEVICE_CONTROLLER)||((type <UDP_DEVICE_MAX)&&(type > UDP_DEVICE_APP)))
	{
		if(addr <= APPPROC_DEV_CONTROLLER_NUM)
		{
			AppDevController[addr-1].devOnLine = APPDEV_STATE_ONLINE;
			AppDevController[addr-1].errCnt =0;
		}
	}
}

void setAppDevOffLine(u8 type,u8 addr)
{
	if(addr==0)
		return;
	
	if(type==UDP_DEVICE_SERVER)
	{
		if(addr <= APPPROC_DEV_SERVER_NUM)
		{
			AppDevServer[addr-1].errCnt++;
			if(AppDevServer[addr-1].errCnt >= APPPROC_DEV_OFFLINE_ERRNUM)
				AppDevServer[addr-1].devOnLine = APPDEV_STATE_ONLINE;
		}
	}
	else if(type==UDP_DEVICE_DATATRANS)
	{
		if(addr <= APPPROC_DEV_DATATRANS_NUM)
		{
			AppDevDataTrans[addr-1].errCnt++;
			if(AppDevDataTrans[addr-1].errCnt >= APPPROC_DEV_OFFLINE_ERRNUM)
				AppDevDataTrans[addr-1].devOnLine = APPDEV_STATE_ONLINE;
		}
	}
	else if((type==UDP_DEVICE_CONTROLLER)||((type <UDP_DEVICE_MAX)&&(type > UDP_DEVICE_APP)))
	{
		if(addr <= APPPROC_DEV_CONTROLLER_NUM)
		{
			AppDevController[addr-1].errCnt++;
			if(AppDevController[addr-1].errCnt >= APPPROC_DEV_OFFLINE_ERRNUM)
			{
				AppDevController[addr-1].devOnLine = APPDEV_STATE_ONLINE;
			}
		}
	}
}

u8 getAppDevOnLine(u8 type,u8 addr,u8 *state)
{
	if(addr==0)
		return 0;
	
	if(type==UDP_DEVICE_SERVER)
	{
		if(addr <= APPPROC_DEV_SERVER_NUM)
		{
			*state = AppDevServer[addr-1].devOnLine;
			return 1;
		}
	}
	else if(type==UDP_DEVICE_DATATRANS)
	{
		if(addr <= APPPROC_DEV_DATATRANS_NUM)
		{
			*state = AppDevDataTrans[addr-1].devOnLine;
			return 1;
		}
	}
	else if((type==UDP_DEVICE_CONTROLLER)||((type <UDP_DEVICE_MAX)&&(type > UDP_DEVICE_APP)))
	{
		if(addr <= APPPROC_DEV_CONTROLLER_NUM)
		{
			*state = AppDevController[addr-1].devOnLine;
			return 1;
		}
	}
	return 0;
}

void TimerCallback_exitCfgMode(u16 data)
{
	if(GetSysFuncMode() == SYSMODE_CONFIG)
		SetSysFuncMode(SYSMODE_CONFIG_FIN);
	setAppProcMode(APPPROC_MODE_NORMAL);
	if(AppProcData.configClearTimerId !=0xff)
	{
		deleteTimerEvent(AppProcData.configClearTimerId);
		AppProcData.configClearTimerId =0xff;
	}

	if(AppProcData.configTimerId !=0xff)
	{
		deleteTimerEvent(AppProcData.configTimerId);
		AppProcData.configTimerId =0xff;
	}
	
	if(AppProcData.setAddrTimerId !=0xff)
	{
		deleteTimerEvent(AppProcData.setAddrTimerId);
		AppProcData.setAddrTimerId =0xff;
	}
}

void TimerCallback_enterCfgMode(u16 data)
{
	bcmApi_setLRCommBreak();
	if(AppProcData.configTimerId !=0xff)
	{
		deleteTimerEvent(AppProcData.configTimerId);
		AppProcData.configTimerId =0xff;
	}

	AppProcData.configClearTimerId = addTimerEvent(10000, 1, TimerCallback_exitCfgMode);
	if(AppProcData.configClearTimerId != 0xff)
	{		
		startTimerEvent(AppProcData.configClearTimerId);
	}	
}

void setSelfID(u8 device,u8 *id,u8 sel)
{
	if(id==NULL)
		return;
	
	if(device == DEVICE_SELFID)
		memcpy(AppProcData.selfID,id,DEVICE_SELFID_MAX);
	else if(device == DEVICE_SELFID_LEFT)
	{
		memcpy(AppProcData.selfID_Left,id,DEVICE_SELFID_MAX);
		if(sel)
		{
			SysDataFun(DATA_MSG_SET_LeftSelfID_PARAM,0,0,DEVICE_SELFID_MAX,0,AppProcData.selfID_Left,0);
		}
	}
	else if(device == DEVICE_SELFID_RIGHT)
	{
		memcpy(AppProcData.selfID_Right,id,DEVICE_SELFID_MAX);
		if(sel)
		{
			SysDataFun(DATA_MSG_SET_RightSelfID_PARAM,0,0,DEVICE_SELFID_MAX,0,AppProcData.selfID_Right,0);
		}		
	}
}

u8 * getSelfID(u8 device)
{
	if(device == DEVICE_SELFID)
		return AppProcData.selfID;
	else if(device == DEVICE_SELFID_LEFT)
		return AppProcData.selfID_Left;
	else if(device == DEVICE_SELFID_RIGHT)
		return AppProcData.selfID_Right;
	else
		return NULL;
}

void TimerCallback_setAddr(u16 data)
{
	u8 temp[28];
	u8 addr,direct,vlan;
	
	#ifdef VLAN_AUTO_ENABLE
	
		deleteTimerEvent(AppProcData.setAddrTimerId);
	
		addr = AppProcData.selfAddr;		
		setBcmInfo_ipaddr(AppProcData.selfAddr,0,NULL);
		setVlanBcm(VLANType_V3V2);
		setBcmInfo_vlanType(VLANType_V3V2);

		if(AppProcData.direct)
		{
			if(addr!=APPPROC_DEV_CONTROLLER_NUM)
			{
				addr ++;
				vlan = VLANType_V2V3;
				direct = DEVICE_DIRECT_RIGHT;
				
				temp[0] = addr;
				temp[1] = direct;
				temp[2] = vlan;
				memcpy(&temp[3],SysNetParam.NetParam_IPAddress,4);
				memcpy(&temp[7],SysNetParam.NetParam_NetMask,4);
				memcpy(&temp[11],SysNetParam.NetParam_GateWay,4);
				temp[6] = addr;
				vlan = getBcmInfo_vlanType();
				
				memcpy(&temp[15],getSelfID(DEVICE_SELFID),DEVICE_SELFID_MAX);
				UdpLinkLayerSend_app_vlan(temp,15+DEVICE_SELFID_MAX,0,0 ,UDP_LINK_BROADCAST_ADDR,AppProcIDType_setAddr_request,(vlan==VLANType_V2V3)?VLANID_V3:VLANID_V2);
			}	

			addr = AppProcData.selfAddr;
			if(addr>=2)
			{
				addr --;
				vlan = VLANType_V2V3;
				direct = DEVICE_DIRECT_LEFT;
				
				temp[0] = addr;
				temp[1] = direct;
				temp[2] = vlan;
				memcpy(&temp[3],SysNetParam.NetParam_IPAddress,4);
				memcpy(&temp[7],SysNetParam.NetParam_NetMask,4);
				memcpy(&temp[11],SysNetParam.NetParam_GateWay,4);
				temp[6] = addr;
				vlan = getBcmInfo_vlanType();
				
				memcpy(&temp[15],getSelfID(DEVICE_SELFID),DEVICE_SELFID_MAX);
				UdpLinkLayerSend_app_vlan(temp,15+DEVICE_SELFID_MAX,0,0 ,UDP_LINK_BROADCAST_ADDR,AppProcIDType_setAddr_request,(vlan==VLANType_V2V3)?VLANID_V2:VLANID_V3);
			}	
		}
		else
		{
			if(addr!=APPPROC_DEV_CONTROLLER_NUM)
			{
				addr ++;
				vlan = VLANType_V2V3;
				direct = DEVICE_DIRECT_LEFT;
				
				temp[0] = addr;
				temp[1] = direct;
				temp[2] = vlan;
				memcpy(&temp[3],SysNetParam.NetParam_IPAddress,4);
				memcpy(&temp[7],SysNetParam.NetParam_NetMask,4);
				memcpy(&temp[11],SysNetParam.NetParam_GateWay,4);
				temp[6] = addr;
				vlan = getBcmInfo_vlanType();
				
				memcpy(&temp[15],getSelfID(DEVICE_SELFID),DEVICE_SELFID_MAX);
				UdpLinkLayerSend_app_vlan(temp,15+DEVICE_SELFID_MAX,0,0 ,UDP_LINK_BROADCAST_ADDR,AppProcIDType_setAddr_request,(vlan==VLANType_V2V3)?VLANID_V2:VLANID_V3);
			}	

			addr = AppProcData.selfAddr;
			if(addr>=2)
			{
				addr --;
				vlan = VLANType_V2V3;
				direct = DEVICE_DIRECT_RIGHT;
				
				temp[0] = addr;
				temp[1] = direct;
				temp[2] = vlan;
				memcpy(&temp[3],SysNetParam.NetParam_IPAddress,4);
				memcpy(&temp[7],SysNetParam.NetParam_NetMask,4);
				memcpy(&temp[11],SysNetParam.NetParam_GateWay,4);
				temp[6] = addr;
				vlan = getBcmInfo_vlanType();
				
				memcpy(&temp[15],getSelfID(DEVICE_SELFID),DEVICE_SELFID_MAX);
				UdpLinkLayerSend_app_vlan(temp,15+DEVICE_SELFID_MAX,0,0 ,UDP_LINK_BROADCAST_ADDR,AppProcIDType_setAddr_request,(vlan==VLANType_V2V3)?VLANID_V3:VLANID_V2);
			}		
			
		}
		
		//setAppProcMode(APPPROC_MODE_IPRESQUEST);
	#endif	
}

void setSelfAddr(u8 selfaddr,u8 sel)
{
	u8 temp[16];
	u8 addr,direct,vlan;
	
	AppProcData.selfAddr = selfaddr;
	setAppProcDirect(SysParam.globalParam.SupportParam.SupportDirect);
	
	if(sel)
	{
	#ifdef VLAN_AUTO_ENABLE
		UdpAppProcCtrl(AppProcCtrl_enterCfgMode_request,0,0,UDP_LINK_BROADCAST_ADDR);
		//PLATFORM_OS_TimeDly(2000);
		AppProcData.setAddrTimerId = addTimerEvent(2000, 1, TimerCallback_setAddr);
		if(AppProcData.setAddrTimerId != 0xff)
		{
			startTimerEvent(AppProcData.setAddrTimerId);
			//setAppProcMode(APPPROC_MODE_CONFIG);
		}
		else
			AppProcData.setAddrTimerId=0;
	#endif
		
	#ifndef VLAN_AUTO_ENABLE
		
		addr = selfaddr;		
		setBcmInfo_ipaddr(selfaddr,0,NULL);
		
		if(addr!=APPPROC_DEV_CONTROLLER_NUM)
		{
			addr ++;
			vlan = getBcmInfo_vlanType();
			vlan = (vlan==VLANType_V2V3)? VLANType_V3V2 :VLANType_V2V3;
			direct = DEVICE_DIRECT_RIGHT;
			
			temp[0] = addr;
			temp[1] = direct;
			temp[2] = vlan;
			memcpy(&temp[3],SysNetParam.NetParam_IPAddress,4);
			memcpy(&temp[7],SysNetParam.NetParam_NetMask,4);
			memcpy(&temp[11],SysNetParam.NetParam_GateWay,4);
			temp[6] = addr;
			vlan = getBcmInfo_vlanType();
			UdpLinkLayerSend_app_vlan(temp,15,0,0 ,UDP_LINK_BROADCAST_ADDR,AppProcIDType_setAddr_request,(vlan==VLANType_V2V3)?VLANID_V3:VLANID_V2);
			//UdpLinkLayerSend_app(temp,15,0,0 ,UDP_LINK_BROADCAST_ADDR,AppProcIDType_setAddr_request);
		}

	
		addr = selfaddr;
		if(addr>=2)
		{
			addr --;
			vlan = getBcmInfo_vlanType();
			vlan = (vlan==VLANType_V2V3)? VLANType_V3V2 :VLANType_V2V3;
			direct = DEVICE_DIRECT_LEFT;
			
			temp[0] = addr;
			temp[1] = direct;
			temp[2] = vlan;
			memcpy(&temp[3],SysNetParam.NetParam_IPAddress,4);
			memcpy(&temp[7],SysNetParam.NetParam_NetMask,4);
			memcpy(&temp[11],SysNetParam.NetParam_GateWay,4);
			temp[6] = addr;
			vlan = getBcmInfo_vlanType();
			UdpLinkLayerSend_app_vlan(temp,15,0,0 ,UDP_LINK_BROADCAST_ADDR,AppProcIDType_setAddr_request,(vlan==VLANType_V2V3)?VLANID_V2:VLANID_V3);
		  //UdpLinkLayerSend_app(temp,15,0,0 ,UDP_LINK_BROADCAST_ADDR,AppProcIDType_setAddr_request);
		}
	#endif
	}
}

u8 getSelfAddr(void)
{
	return AppProcData.selfAddr;
}

void clearUdpAppProcTimer(u8 direct)
{
	if(direct==DEVICE_SELFID_LEFT)
		AppProcData.lefttimer =0;
	else
		AppProcData.righttimer =0;
}

void UdpAppProcTimerProcess(u16 delay)
{
	u8 temp[2];

	AppProcData.lefttimer +=delay;
	AppProcData.righttimer+=delay;
	if(AppProcData.lefttimer > APPPROC_CHECK_TIME)
	{
		if(((AppProcData.direct)&&(AppProcData.selfAddr!=SysParam.globalParam.SupportParam.MinSupportID))
		  ||((AppProcData.direct==0)&&(AppProcData.selfAddr!=SysParam.globalParam.SupportParam.MaxSupportID)))
		{
			temp[0] = 0;		
			temp[1] = DEVICE_SELFID_LEFT;
			SysDataFun(DATA_MSG_SetCommCheckState,0,0,2,0,temp,0);			
		}
		AppProcData.lefttimer =0;
	}
	
	if(AppProcData.righttimer > APPPROC_CHECK_TIME)
	{
		if(((AppProcData.direct)&&(AppProcData.selfAddr!=SysParam.globalParam.SupportParam.MaxSupportID))
		  ||((AppProcData.direct==0)&&(AppProcData.selfAddr!=SysParam.globalParam.SupportParam.MinSupportID)))
		{
			temp[0] = 0;
			temp[1] = DEVICE_SELFID_RIGHT;
			SysDataFun(DATA_MSG_SetCommCheckState,0,0,2,0,temp,0);
		}
		AppProcData.righttimer =0;
	}	
}

u8 UdpAppProcFunc(UDPProcType *proc, UDPFrameType *pframe)
{
	u16 id;
	u8  i;
	u8  findFlag=0;
	
	if(pframe->length <APPPROC_FRAME_IDLENGTH)
		return 0;
	
	id = *((u16*)(pframe->pdata));
	for(i=0 ; i< (sizeof(AppProcInfo)/sizeof(UDPProcType));i++)
	{
		if(AppProcInfo[i].procID == id)
		{
			findFlag = 1;
			id = i;
			break;
		}
	}
	
	if(findFlag)
	{
		proc ->procID = AppProcInfo[id].procID;
		proc ->overtime = AppProcInfo[id].overtime;
		proc ->repeatNum = AppProcInfo[id].repeatNum;
		proc ->recvCBFun = AppProcInfo[id].recvCBFun;
		proc ->overCBFun = AppProcInfo[id].overCBFun;
		return 1;
	}
	
	return 0;
}

u8 UdpLinkLayerSend_app(u8 *pdata,u16 length,u8 ack,u16 sn,u8 supportAddr,u16 funcid)
{
	u8 ret;
	u8 *ptemp =NULL;
#if OS_CRITICAL_METHOD == 3                      
    OS_CPU_SR  cpu_sr = 0;
#endif	
	
	OS_ENTER_CRITICAL();
	
	ptemp = memory_malloc(length+APPPROC_FRAME_IDLENGTH);
	if(ptemp ==NULL)
	{
		OS_EXIT_CRITICAL();
		return 0;
	}
	
	memcpy(ptemp,&funcid,APPPROC_FRAME_IDLENGTH);
	memcpy(ptemp+APPPROC_FRAME_IDLENGTH,pdata,length);
	ret = UdpLinkLayerSend(ptemp,length+APPPROC_FRAME_IDLENGTH,ack,sn ,getAppProcDeviceType(),UDPProcess_ID_SupportCom,supportAddr,0);
	memory_free((void *)ptemp);
	
	OS_EXIT_CRITICAL();
	
	return ret;
}

u8 UdpLinkLayerSend_app_vlan(u8 *pdata,u16 length,u8 ack,u16 sn,u8 supportAddr,u16 funcid,u8 vlan)
{
	u8 ret;
	u8 *ptemp =NULL;
#if OS_CRITICAL_METHOD == 3                      
    OS_CPU_SR  cpu_sr = 0;
#endif	
	
	OS_ENTER_CRITICAL();	
	
	ptemp = memory_malloc(length+APPPROC_FRAME_IDLENGTH);
	if(ptemp ==NULL)
	{
		OS_EXIT_CRITICAL();
		return 0;
	}
	
	memcpy(ptemp,&funcid,APPPROC_FRAME_IDLENGTH);
	memcpy(ptemp+APPPROC_FRAME_IDLENGTH,pdata,length);
	ret = UdpLinkLayerSend(ptemp,length+APPPROC_FRAME_IDLENGTH,ack,sn ,getAppProcDeviceType(),UDPProcess_ID_SupportCom,supportAddr,vlan);
	memory_free((void *)ptemp);
	
	OS_EXIT_CRITICAL();
	
	return ret;
}

u8 UdpAppProcCtrl(AppProcCtrl_t ctrlMsg,void *pdata,u16 length,u8 addr)
{
	u8 type;
	u8 id;
	u16 len =0;
	u16 offset =0;
	
	if (ctrlMsg> AppProcCtrl_MSG_MAX)//||(pdata ==NULL))		
		return 0;
	
	switch(ctrlMsg)
    {
		case AppProcCtrl_readState_request:  //u8 type+u8 id+(u16 length+u16 offset)
		case AppProcCtrl_getParam_request:
		{
			if((length <2)||(length >6))
				return 0;
			
			UdpLinkLayerSend_app(pdata,length,0,0 ,addr,(ctrlMsg ==AppProcCtrl_readState_request)? AppProcIDType_readState_request:AppProcIDType_getParam_request);
		}
		break;
		
		case AppProcCtrl_readState_multiple_request:  //u8 type+u8 startid+u8 endid+(u16 length+u16 offset)
		case AppProcCtrl_getParam_multiple_request:
		{
			if((length <3)||(length >7))
				return 0;
			
			UdpLinkLayerSend_app(pdata,length,0,0 ,addr,(ctrlMsg ==AppProcCtrl_getParam_multiple_request)?AppProcIDType_readState_multiple_request:AppProcIDType_getParam_multiple_request);
		}
		break;
		
		case AppProcCtrl_stateReport_indication:  //u8 statetype+u8 device+u8 state+u16 length+(u8 data)
		{
			if(length <5)
				return 0;
			
			UdpLinkLayerSend_app(pdata,length,0,0 ,addr,AppProcIDType_stateReport_indication);//qinxi
		}
		break;
		
		case AppProcCtrl_logReport_indication:  
		{
			if(length <3)
				return 0;
			
			UdpLinkLayerSend_app(pdata,length,0,0 ,addr,AppProcIDType_logReport_indication);
		}
		break;
		
		case AppProcCtrl_control_request:  
		//u8 Type+u8 Id+u8 ControlCommand+u8 ControlType+u8 ControlParamLength+(u8 SourceAddr+u8 SourceType+u16 UserId+u8 ControlParamData[])
		{
			if(length <5)
				return 0;
			
			UdpLinkLayerSend_app(pdata,length,0,0 ,addr,AppProcIDType_control_request);
		}
		break;
		
		case AppProcCtrl_setParam_request: //u8 type+u8 id+u16 length+u16 offset+u8 data[]
		{
			if(length <6)
				return 0;
			
			UdpLinkLayerSend_app(pdata,length,0,0 ,addr,AppProcIDType_setParam_request);
		}
		break;
		
		case AppProcCtrl_setParam_multiple_request: //u8 type+u8 startid+u8 endid+u16 length+u16 offset+u8 data[]
		{
			if(length <7)
				return 0;
			
			UdpLinkLayerSend_app(pdata,length,0,0 ,addr,AppProcIDType_setParam_multiple_request);
		}
		break;
		
		case AppProcCtrl_checkParam_request: //(u8 type+u8 startid+u8 endid)
		{
			if(length >3)
				return 0;
			
			UdpLinkLayerSend_app(pdata,length,0,0 ,addr,AppProcIDType_checkParam_request);
		}
		break;
		
		case AppProcCtrl_setParam_UIMode_request://u64 levels+u16 global+u16 length+u16 offset+(u8*n data)
		{
			if(length <14)
				return 0;
			
			UdpLinkLayerSend_app(pdata,length,0,0 ,addr,AppProcIDType_setParam_UIMode_request);  
		}
		break;		
		
		case AppProcCtrl_enterCfgMode_request://none
		{
			if(length !=0)
				return 0;
			
			UdpLinkLayerSend_app(pdata,length,0,0 ,UDP_LINK_BROADCAST_ADDR,AppProcIDType_enterCfgMode_request);
		}
		break;	

		case AppProcCtrl_setAddr_request://u8 StartIP+u8 Direction+u8 VLAN
		{
			if((length !=3)||(AppProcMode==APPPROC_MODE_NORMAL))
				return 0;
			
			UdpLinkLayerSend_app(pdata,length,0,0 ,UDP_LINK_BROADCAST_ADDR,AppProcIDType_setAddr_request);  //@@todo tag
		}
		break;		

		case AppProcCtrl_netHeart_request://none
		{
			if(length !=0)
				return 0;
			
			UdpLinkLayerSend_app(pdata,length,0,0 ,UDP_LINK_BROADCAST_ADDR,AppProcIDType_netHeart_request);
		}
		break;	

		case AppProcCtrl_timeSync_request://u8 Year+u8 Month+u8 Day+u8 Hour+u8 Minute+u8 Second
		{
			if(length !=6)
				return 0;
			
			UdpLinkLayerSend_app(pdata,length,0,0 ,UDP_LINK_BROADCAST_ADDR,AppProcIDType_timeSync_request);
		}
		break;	

		case AppProcCtrl_commCheck_request://Octet string(12bytes) SelfID
		{
			if(length !=12)
				return 0;
			
			UdpLinkLayerSend_app(pdata,length,0,0 ,addr,AppProcIDType_commCheck_request); //@@todo tag
		}
		break;

		case AppProcCtrl_serverOperate_request:
		{
			if(length !=2)
				return 0;
			
			UdpLinkLayerSend_app(pdata,length,0,0 ,addr,AppProcIDType_serverOperate_request);			
		}
		break;

		case AppProcCtrl_manualControl_request:  
		//u8 Id1+u8 Id2....(max 10)
		{
			if(pdata==NULL)
				return 0;
			
			UdpLinkLayerSend_app(pdata,length,0,0 ,addr,AppProcIDType_manualControl_request);
		}
		break;	

		case AppProcCtrl_ykqDecode_request:
			if(pdata==NULL)
				return 0;		
			UdpLinkLayerSend_app(pdata,length,0,0 ,addr,AppProcIDType_ykqDecode_request);
			break;
		
		case AppProcCtrl_arpNetMaintain_request:
			UdpLinkLayerSend_app(pdata,length,0,0 ,addr,AppProcIDType_arpNetMaintain_request);
		break;
		
		default:
			break;
	}
	
    return 1;		
}

u8 getUdpAppSelfParamInfo(u8 *pParam,u8 len)
{
	if(len >APPPROC_SELF_PARAM_LEN)
		return 0;
	
	memcpy(pParam,AppProcData.selfParam,len);
	return 1;
}

u8 setUdpAppSelfParamInfo(u8 *pParam,u8 len)
{
	if(len >APPPROC_SELF_PARAM_LEN)
		return 0;
	
	memcpy(AppProcData.selfParam,pParam,len);
	return 1;
}

void UdpAppCommCheckFunc(u16 delay)
{
	u8 *selfid=NULL;
	u8 addr,vlan,sendvlan;
	u8 senddata[DEVICE_SELFID_MAX+APPPROC_SELF_PARAM_LEN+V4_CONTROLLER_VERSION_LEN];
	
	selfid = getSelfID(DEVICE_SELFID);
	if(AppProcData.selfAddr >SysParam.globalParam.SupportParam.MinSupportID)
	{
		addr= AppProcData.selfAddr-1;
		vlan = getBcmInfo_vlanType();
		if(AppProcData.direct)
			sendvlan = (vlan==VLANType_V2V3)?VLANID_V2:VLANID_V3;
		else
			sendvlan = (vlan==VLANType_V2V3)?VLANID_V3:VLANID_V2;
		memcpy(senddata,selfid,DEVICE_SELFID_MAX);
		getUdpAppSelfParamInfo(senddata+DEVICE_SELFID_MAX,APPPROC_SELF_PARAM_LEN);
		senddata[DEVICE_SELFID_MAX+APPPROC_SELF_PARAM_LEN] = V4_CONTROLLER_VERSION_BYTE1;
		senddata[DEVICE_SELFID_MAX+APPPROC_SELF_PARAM_LEN+1] = V4_CONTROLLER_VERSION_BYTE2;
		senddata[DEVICE_SELFID_MAX+APPPROC_SELF_PARAM_LEN+2] = V4_CONTROLLER_VERSION_BYTE3;
		senddata[DEVICE_SELFID_MAX+APPPROC_SELF_PARAM_LEN+3] = V4_CONTROLLER_VERSION_BYTE4;
		
		UdpLinkLayerSend_app_vlan(senddata,DEVICE_SELFID_MAX+APPPROC_SELF_PARAM_LEN+V4_CONTROLLER_VERSION_LEN,0,0 ,addr,AppProcIDType_commCheck_request,sendvlan); //@@todo tag
		//UdpAppProcCtrl(AppProcCtrl_commCheck_request,selfid,DEVICE_SELFID_MAX,addr);
	}
	
	if(AppProcData.selfAddr <SysParam.globalParam.SupportParam.MaxSupportID)
	{
		addr= AppProcData.selfAddr+1;
		vlan = getBcmInfo_vlanType();
		if(AppProcData.direct)
			sendvlan = (vlan==VLANType_V2V3)?VLANID_V3:VLANID_V2;	
		else
			sendvlan = (vlan==VLANType_V2V3)?VLANID_V2:VLANID_V3;
		memcpy(senddata,selfid,DEVICE_SELFID_MAX);
		getUdpAppSelfParamInfo(senddata+DEVICE_SELFID_MAX,APPPROC_SELF_PARAM_LEN);
		senddata[DEVICE_SELFID_MAX+APPPROC_SELF_PARAM_LEN] = V4_CONTROLLER_VERSION_BYTE1;
		senddata[DEVICE_SELFID_MAX+APPPROC_SELF_PARAM_LEN+1] = V4_CONTROLLER_VERSION_BYTE2;
		senddata[DEVICE_SELFID_MAX+APPPROC_SELF_PARAM_LEN+2] = V4_CONTROLLER_VERSION_BYTE3;
		senddata[DEVICE_SELFID_MAX+APPPROC_SELF_PARAM_LEN+3] = V4_CONTROLLER_VERSION_BYTE4;		
		
		UdpLinkLayerSend_app_vlan(senddata,DEVICE_SELFID_MAX+APPPROC_SELF_PARAM_LEN+V4_CONTROLLER_VERSION_LEN,0,0 ,addr,AppProcIDType_commCheck_request,sendvlan);
		//UdpAppProcCtrl(AppProcCtrl_commCheck_request,selfid,DEVICE_SELFID_MAX,addr);
	}	
}

/**************************update /phone *******************************************/

u8 UdpUpdateProcFunc(UDPProcType *proc, UDPFrameType *pframe)
{
	u16 id;
	u8  i;
	u8  findFlag=0;
	
	if(pframe->length <APPPROC_FRAME_IDLENGTH)
		return 0;
	
	id = *((u16*)(pframe->pdata));
	for(i=0 ; i< (sizeof(UpdateProcInfo)/sizeof(UDPProcType));i++)
	{
		if(UpdateProcInfo[i].procID == id)
		{
			findFlag = 1;
			id = i;
			break;
		}
	}
	
	if(findFlag)
	{
		proc ->procID = UpdateProcInfo[id].procID;
		proc ->overtime = UpdateProcInfo[id].overtime;
		proc ->repeatNum = UpdateProcInfo[id].repeatNum;
		proc ->recvCBFun = UpdateProcInfo[id].recvCBFun;
		proc ->overCBFun = UpdateProcInfo[id].overCBFun;
		return 1;
	}
	
	return 0;
}

u8 UdpUpdateProcCtrl(u16 ctrlMsg,void *pdata,u16 length,u8 addr,udpUpdateApiType frameInfo)
{		
	if ((ctrlMsg>= UpdateProcIDType_MAX)||(pdata ==NULL))		
		return 0;
	
	frameInfo.devtype =getAppProcDeviceType();
	switch(ctrlMsg)
    {
		case UpdateProcIDType_UpdateStart_request:  //u16 FileType+u32 FileSize+u8*4 FileSuffixType+u8*20 FileName
		{
			if(length != sizeof(UDP_UPDATE_READY))
				return 0;
			
			memcpy(udpUpdateBuf,(u8 *)(&ctrlMsg),2);
			memcpy(udpUpdateBuf+2,(u8 *)((UDP_UPDATE_READY *)pdata)+2,sizeof(UDP_UPDATE_READY)-2);
			UdpLinkLayerSend_ext(udpUpdateBuf,sizeof(UDP_UPDATE_READY),frameInfo.ack,0,frameInfo.devtype,UDPProcess_ID_Update,addr,frameInfo.totalNum,frameInfo.curNum);
		}
		break;

		case UpdateProcIDType_SendData_request:  //u8*512 data
		{			
			memcpy(udpUpdateBuf,(u8 *)(&ctrlMsg),2);
			memcpy(udpUpdateBuf+2,(u8 *)(&length),2);
			memcpy(udpUpdateBuf+4,(u8 *)(pdata),length);
			UdpLinkLayerSend_ext(udpUpdateBuf,length +4,frameInfo.ack,0,frameInfo.devtype,UDPProcess_ID_Update,addr,frameInfo.totalNum,frameInfo.curNum);
		}
		break;			

		case UpdateProcIDType_RecvDataAck_request:  //u16 CRCData/result
		case UpdateProcIDType_DataCRC_request:
		case UpdateProcIDType_DataCRC_confirm:
		case UpdateProcIDType_UpdateNow_request:
		{			
			memcpy(udpUpdateBuf,(u8 *)(&ctrlMsg),2);
			memcpy(udpUpdateBuf+2,(u8 *)((u16 *)pdata),2);
			UdpLinkLayerSend_ext(udpUpdateBuf,4,frameInfo.ack,0,frameInfo.devtype,UDPProcess_ID_Update,addr,frameInfo.totalNum,frameInfo.curNum);
		}
		break;		
		
		default:
			break;
	}
	
    return 1;		
}

#endif
