/*
*********************************************************************************************************
*	                                  
*	模块名称 : 
*	文件名称 : MenuConfig.c
*	版    本 : V1.0
*	说    明 : 实现按键各种功能
*              
*	修改记录 :
*		版本号   日期         作者          说明
*		V1.0    2016-07-16   Eric2013  	    首版    
*                                     
*	Copyright (C), 2015-2020, 安富莱电子 www.armfly.com
*
*********************************************************************************************************
*/
#include "StateConfig.h"
#include "User_GUI.h"
#include "sys_msg_def.h"
#include "Log_Data_Mng.h"
#include "Can_Pro.h"
#include "ProjectConfig.h"
/*
*********************************************************************************************************
*	                                     宏定义
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*	                                    全局变量
*********************************************************************************************************
*/
extern char *SupportType_Grope[];
extern NameAndId   SingleActionBUF[];
extern NameAndId   GroupActionBUF[];
extern SensorShow   SensorShowBUF[];
extern uint8_t  KeyVersion[];
extern uint8_t WirelessVersion[];
/*
*********************************************************************************************************
*	                                     支架控制器版本号
*********************************************************************************************************
*/
//char *ControllerVersion = "V8.1.0.0";
static char ControllerVersion[20];
/*
*********************************************************************************************************
*	                                     ※※※※※※※※※※※
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*	                                     列表头定义
*********************************************************************************************************
*/
const LISTVIEW_HEADER State_LISTVIEW_HEADER1[] =
{
	{40,""},
	{125,"名称"},
	{85,"状态"},
	{110,"数值"},
};
const LISTVIEW_HEADER State_LISTVIEW_HEADER2[] =
{
	{35,""},
	{115,"动作名"},
	{105,"类型"},
	{105,"状态"},
};
const LISTVIEW_HEADER State_LISTVIEW_HEADER3[] =
{
	{40,""},
	{125,"ID"},
	{85,"距离"},
	{110,"权限"},
};
const LISTVIEW_HEADER State_LISTVIEW_HEADER4[] =
{
	{40,""},
	{160,"名称"},
	{160,"版本"},
};
const LISTVIEW_HEADER State_LISTVIEW_HEADER5[] =
{
	{40,""},
	{130,"时间"},
	{190,"名称"},
};
const LISTVIEW_HEADER State_LISTVIEW_HEADER6[] =
{
	{40,""},
	{160,"名称"},
	{160,"状态"},
};
/*
*********************************************************************************************************
*	                                     选择项表定义
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*	                                    按键名称定义
*********************************************************************************************************
*/

const char *StateButtonName[][6] = 
{
	{"传感器","日志","人员","版本","故障","运行"},
};

/*
*********************************************************************************************************
*	                                    文本名称定义
*********************************************************************************************************
*/
extern SensorShow   SensorShowBUF[];
void show_fault_type(char * show[],STR_FAULT_INFO val)
{
	uint16_t i;
	static char buf[20];
	switch(val.host_type)
	{
		case FAULT_HOST_SENSOR:
			for(i=1;i<SensorShowBUF[0].MAX+1;i++)
			{
				if(val.host_name == SensorShowBUF[i].id)
				{
					memset(buf,0,sizeof(buf));
					memcpy(buf,SensorShowBUF[i].name,sizeof(SensorShowBUF[i].name));					
					show[2] = buf;
					strcat(show[2],"故障");
				}
			}
			break;	
		case FAULT_HOST_DEVICE:
			switch(val.host_name)
			{
				case ID_EMVD:
					show[2] = "电磁阀故障";
					break;
				case ID_ALARM:
					show[2] = "声光报警故障";
					break;
				case ID_PERSON:
					show[2] = "人员定位故障";
					break;
				case ID_IRRX:
					show[2] = "红外接收故障";
					break;
				default:
					break;
			}
			break;	
		//ghy 添加HUB故障信息记录
		case  FAULT_HOST_HUB1:	
			switch(val.host_name)
			{
				case ID_HUB1:
					show[2] = "HUB1故障";
				break;
				default:
				break;
			}
			break;
		 case  FAULT_HOST_HUB2:	
			switch(val.host_name)
			{
				case ID_HUB2:
					show[2] = "HUB2故障";
				break;
				default:
				break;
			}
			break;	
		 case  FAULT_HOST_HUB3:	
			switch(val.host_name)
			{
				case ID_HUB3:
					show[2] = "HUB3故障";
				break;
				default:
				break;
			}
			break;
			
		case FAULT_HOST_OTHER:
			switch(val.event)
			{
				case FAULT_EVENT_FAULT:
					show[2] = "传感器故障";
					break;
				case FAULT_EVENT_OFFLINE:
					show[2] = "设备离线";
					break;
				case FAULT_EVENT_WORKFACE_COMMUNICATION_WRONG:
					show[2] = "工作面通信异常";
					break;
				case FAULT_EVENT_SYSTEM_NUMBER_WRONG:
					show[2] = "系统编号错误";
					break;
				case FAULT_EVENT_SERVER_WRONG:
					show[2] = "SS位置或使能不符";
					break;
				case FAULT_EVENT_NUMBER_ADD_DIRECT_WRONG:
					show[2] = "架号增向不符";
					break;
				case FAULT_EVENT_NUMBER_RANGE_WRONG:
					show[2] = "架号范围参数错误";
					break;
				case FAULT_EVENT_PARA_UPDATE_WRONG:
					show[2] = "参数更新失败";
					break;	
				case FAULT_EVENT_ALL_PARA_NOT_SAME:
					show[2] = "全局参数不一致";
					break;
				case FAULT_EVENT_ALL_PARA_NOT_MATCH_LEFTNEIGHBOR:
					show[2] = "参数左邻架不符";
					break;
				case FAULT_EVENT_ALL_PARA_NOT_MATCH_RIGHTNEIGHBOR:
					show[2] = "参数右邻架不符";
					break;
				case FAULT_EVENT_PARGRAM_NOT_SAME:
					show[2] = "程序版本不一致";
					break;
				case FAULT_EVENT_PARGRAM_NOT_MATCH_NEIGHBOR:
					show[2] = "版本号与邻架不符";
					break;
				case FAULT_EVENT_HARD_SOFT_NOT_MATCH:
					show[2] = "硬件与软件不符";
					break;
				case FAULT_EVENT_LEFT_NEIGHBOR_COMMUNICATION_WRONG:
					show[2] = "左邻架通信错误";
					break;	
				case FAULT_EVENT_RIGHT_NEIGHBOR_COMMUNICATION_WRONG:
					show[2] = "右邻架通信错误";
					break;	
				case FAULT_EVENT_ABNORMAL_COMMUNICATION_WITH_SERVER:
					show[2] = "远端网络通信异常";//与服务器通信异常
					break;
				case FAULT_EVENT_SYSPARAM_MAXSUPPORTNUM_NOTSAME:
					show[2] = "最大架号不一致";
					break;	
				case FAULT_EVENT_SYSPARAM_MINSUPPORTNUM_NOTSAME:
					show[2] = "最小架号不一致";
					break;	
				case FAULT_EVENT_SYSPARAM_SUPPORTNUMINCREASE_NOTSAME:
					show[2] = "架号增向不一致";
					break;					
				default:
					break;				
			}
			break;
		default:
			break;
	}
}
void show_log_type(char * show[],STR_LOG_INFO val)
{
	uint16_t i;
	show[1] = " ";
	switch(val.type)
	{
		case LOG_TYPE_HANDLE_SINGLE:
			show[2] = "单动作";
			for(i=1;i<SingleActionBUF[1].MAX+1;i++)
			{
				if(val.host == SingleActionBUF[i].id)
				{
					show[1] = SingleActionBUF[i].name;
				}
			}
			
			break;
		case LOG_TYPE_HANDLE_COMB:
			show[2] = "组合动作";
			for(i=SingleActionBUF[1].MAX+1;i<SingleActionBUF[0].MAX+1;i++)
			{
				if(val.host == SingleActionBUF[i].id)
				{
					show[1] = SingleActionBUF[i].name;
				}
			}
			break;
		case LOG_TYPE_HANDLE_GROUP:
			show[2] = "成组动作";
			for(i=1;i<GroupActionBUF[0].MAX+1;i++)
			{
				if(val.host == GroupActionBUF[i].id)
				{
					show[1] = GroupActionBUF[i].name;
				}
			}
			break;
		case LOG_TYPE_HANDLE_SYSTEM:
			show[2] = "系统操作";
			break;
		case LOG_TYPE_SERVICE:
			show[2] = "服务";
			break;
		case LOG_TYPE_ALARM:
			show[2] = "传感器";
			for(i=1;i<SensorShowBUF[0].MAX+1;i++)
			{
				if(val.host == SensorShowBUF[i].id)
				{
					show[1] = SensorShowBUF[i].name;
				}
			}
			break;
		case LOG_TYPE_SYSTEM_ALARM:
			show[2] = "报警";
			show[1] = "系统";
			break;
		case LOG_TYPE_UPDATE_PROGRAM:
			show[2] = "更新";
			show[1] = "程序";
			break;
		case LOG_TYPE_RESET_SYSTEM:
			show[2] = "复位";
			show[3] = " ";
			break;
		case LOG_TYPE_JSON_TEXT:
			show[2] = "配置文件";
			show[3] = "启动";
			break;
		default:
			break;
	}
	switch(val.status)
	{
		case LOG_STATUS_START:
			show[3] = "启动";
			break;
		case LOG_STATUS_STOP:
			show[3] = "停止";
			break;
		case LOG_STATUS_DELAY_START:
			show[3] = "延时启动";
			break;
		case LOG_STATUS_SUSPEND:
			show[3] = "暂停";
			break;
		case LOG_STATUS_INPLACE:
			show[3] = "到位";
			break;
		case LOG_STATUS_DISABLE:
			show[3] = "禁止";
			break;
		case LOG_STATUS_OVERTIME:
			show[3] = "超时";
			break;
		case LOG_STATUS_SENSOR_NORMAL:
			show[3] = "正常";
			break;
		case LOG_STATUS_SENSOR_ALARM:
			show[3] = "报警";
			break;
		case LOG_STATUS_EMERGENCY_ALARM:
			show[3] = "急停";
			break;
		case LOG_STATUS_EMERGENCY_NORMAL:
			show[3] = "急停恢复";
			break;
		case LOG_STATUS_BISUO_ALARM:
			show[3] = "闭锁";
			break;
		case LOG_STATUS_BISUO_NORMAL:
			show[3] = "闭锁恢复";
			break;
		case LOG_STATUS_DOWNLOAD_SUCCESS:
			show[3] = "下载成功";
			break;
		case LOG_STATUS_DOWNLOAD_FAIL:
			show[3] = "下载失败";
			break;
		case LOG_STATUS_UPDATE_PRO_SUCCESS:
			show[3] = "程序成功";
			break;
		case LOG_STAUTS_UPDATE_PRO_FAIL:
			show[3] = "程序失败";
			break;
		case LOG_STATUS_UPDATE_PARA_SUCCESS:
			show[3] = "参数成功";
			break;
		case LOG_STATUS_UPDATE_PARA_FAIL:
			show[3] = "参数失败";
			break;
		case LOG_STATUS_RESET_LOWPAWER:
			show[1] = "低功耗";
			break;
		case LOG_STATUS_RESET_WWATCHDOG:
			show[1] = "窗口看门狗";
			break;
		case LOG_STATUS_RESET_IWATCHDOG:
			show[1] = "独立看门狗";
			break;
		case LOG_STATUS_RESET_SOFTWARE:
			show[1] = "软件";
			break;
		case LOG_STATUS_RESET_PORRSTF:
			show[1] = "掉电";
			break;
		case LOG_STATUS_PERSONLOCK_TIMEOUT:
			show[3] = "人员闭锁";
			break;
		case LOG_STATUS_NET_REINIT:
			show[3] = "网络重连";
			break;
		case LOG_STATUS_START_OUT:
			show[1] = "外部";
			break;		
		case LOG_STATUS_START_IN:
			show[1] = "内部";
			break;
		default:
			break;
	}
}
void show_senson_refresh(WM_MESSAGE * pMsg)
{
	uint8_t i,length,k=1;
	uint8_t row=0;
	uint32_t person_temp;
	static char * sensor[4];
	static uint16_t back[10];
	SensorStateType SensorState;
	char buf[15];
	char value[13];
	char state[13];
	char numberbuf[3];
	SensorParamType Sensorpara;
	stDeviceStatusMsg  Sensorversion;
	row = LISTVIEW_GetSel(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW0));
	LISTVIEW_DeleteAllRows(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW0));
	for(i=0;i<SensorShowBUF[0].MAX;i++)
	{
		memset(buf,0x00,sizeof(buf));
		memset(numberbuf,0x00,sizeof(numberbuf));
		sensor[1] = SensorShowBUF[i+1].name;
		GUIGetDateFunAPI(API_Display_Item,SensorShowBUF[i+1].id,&SensorState,&length);	
		GUIGetDateFunAPI(API_Sensor_Version,SensorShowBUF[i+1].id,&Sensorpara,&length);	
		GetCanDeviceStatusMsg(Sensorpara.pointID.driver,Sensorpara.pointID.device,Sensorpara.pointID.subDevice,&Sensorversion);	
		if(Sensorpara.enable == 0)
			continue;
		else 
		{
			sprintf(numberbuf, "%d", k++);
			sensor[0] = numberbuf;
			if(SensorState.state_online == OnlineType_ONLINE)
			{
				if(SensorState.state_fail == FailType_NORMAL)
					sprintf(state,"%s","正常");
				else 
					sprintf(state,"%s","故障");
			}
			else
			{
				sprintf(state,"%s","离线");																
			}					
			sensor[2] = state;
			
			if((SensorState.state_fail == FailType_NORMAL) && (SensorState.state_online == OnlineType_ONLINE))
			{
				if(back[i] > SensorState.calculateValue)
				{
					if((back[i] - SensorState.calculateValue) <4)
					{
						SensorState.calculateValue = back[i];
					}	
					else
					{
						back[i] = SensorState.calculateValue;
					}
				}
				else
				{
					if((SensorState.calculateValue - back[i]) <4)
					{
						SensorState.calculateValue = back[i];
					}
					else
					{
						back[i] = SensorState.calculateValue;
					}
				}
				if(SensorShowBUF[i+1].acc == 3)
					sprintf(value,"%d.%d",SensorState.calculateValue/10,SensorState.calculateValue%10);
				else if(SensorShowBUF[i+1].acc == 4)
					sprintf(value,"%d.%02d",SensorState.calculateValue/100,SensorState.calculateValue%100);
				else if(SensorShowBUF[i+1].acc == 9)
					sprintf(value,"%d.%d",(int16_t)SensorState.calculateValue/10,SensorState.calculateValue%10);
				else					
					sprintf(value,"%d",SensorState.calculateValue);
			}
			else
			{
				sprintf(value,"%s","***");
			}
			AddUnitDispiay(value,SensorShowBUF[i+1].Nuit);
			sensor[3] = value;				
			LISTVIEW_AddRow(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW0),sensor);
			memset(value,0,sizeof(value));
			memset(state,0,sizeof(state));
		}	
		
	}
	LISTVIEW_IncSel(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW0));
	WM_SetFocus(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW0));
	LISTVIEW_SetSel(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW0), row);
}
/*
*********************************************************************************************************
*	函 数 名: 
*	功能说明: 创建菜单列表
*	形    参: 
*	返 回 值: 
*********************************************************************************************************
*/
void show_person_refresh(WM_MESSAGE * pMsg)
{
	uint8_t i,j=0;
	uint8_t row=0;
	uint8_t device;
	uint32_t person_temp;
	static char * sensor[4];
	char buf[15];
	char value[13];
	char state[13];
	char numberbuf[3];
	row = LISTVIEW_GetSel(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW0));
	LISTVIEW_DeleteAllRows(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW0));
	if(GetDeviceCanMsg(ID_PERSON,0,&device) == 0)
	return;
	for(i=0;i<11;i++)
	{
		memset(sensor,0x00,sizeof(sensor));
		GetCanDeviceAPPMsg(device,ID_PERSON,i,1,&person_temp);
		if(person_temp == 0)
		continue;
		memset(buf,0x00,sizeof(buf));
		memset(value,0x00,sizeof(value));
		memset(state,0x00,sizeof(state));
		memset(numberbuf,0x00,sizeof(numberbuf));
		sprintf(numberbuf, "%d", ++j);
		sensor[0] = numberbuf;
		sprintf(buf, "%d", person_temp);
		sensor[1] = buf;
		GetCanDeviceAPPMsg(device,ID_PERSON,i,0,&person_temp);//权限
		person_temp  = (person_temp>>12)&0x0000FF;
		sprintf(value, "%d级", person_temp);
		sensor[3] = value;
		GetCanDeviceAPPMsg(device,ID_PERSON,i,2,&person_temp);//距离
		sprintf(state, "%dcm", person_temp);
		sensor[2] = state; 
		LISTVIEW_AddRow(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW0),sensor);
	}
	LISTVIEW_SetSel(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW0), row);
}
extern u16 InsServer;
extern u16  RecoilState;
extern u16  RecoilNumber;

extern uint16_t transportplane[]; //运输机电流值数组
/*
*********************************************************************************************************
*	函 数 名: 
*	功能说明: 创建菜单列表
*	形    参: 
*	返 回 值: 
*********************************************************************************************************
*/
void show_function_refresh(WM_MESSAGE * pMsg)
{
	uint8_t i,j=0;
	static char * sensor[4];
	char buf[15];
	char numberbuf[3];
	uint16_t transportp[4];
	uint8_t length;
    //获取是否显示
	GUIGetDateFunAPI(API_Transport_Plane,0,&transportp,&length);
	
	uint8_t row;
	row = LISTVIEW_GetSel(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW0));
	LISTVIEW_DeleteAllRows(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW0));
	//写惯导数据
	memset(sensor,0x00,sizeof(sensor));
	memset(numberbuf,0x00,sizeof(numberbuf));
	sprintf(numberbuf, "%d", ++j);
	sensor[0] = numberbuf;
	sensor[1] = "惯导找直";
	memset(buf,0x00,sizeof(buf));
	if(InsServer)
		sprintf(buf, "%dmm", InsServer);	
	else
		sprintf(buf, "%s", "未启动");
	sensor[2] = buf;
	LISTVIEW_AddRow(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW0),sensor);
	//写反冲洗状态
	memset(sensor,0x00,sizeof(sensor));
	memset(numberbuf,0x00,sizeof(numberbuf));
	sprintf(numberbuf, "%d", ++j);
	sensor[0] = numberbuf;
	sensor[1] = "自动反冲洗";
	memset(buf,0x00,sizeof(buf));
	if(RecoilState)
		sprintf(buf, "%s", "启动");	
	else
		sprintf(buf, "%s", "未启动");
	sensor[2] = buf;
	LISTVIEW_AddRow(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW0),sensor);
	//写反冲洗架号
	if(RecoilState)
	{
		memset(sensor,0x00,sizeof(sensor));
		memset(numberbuf,0x00,sizeof(numberbuf));
		sprintf(numberbuf, "%d", ++j);
		sensor[0] = numberbuf;
		sensor[1] = "正在冲洗";
		memset(buf,0x00,sizeof(buf));
		sprintf(buf, "%d号架", RecoilNumber);	
		sensor[2] = buf;
		LISTVIEW_AddRow(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW0),sensor);
	}
	//写运输机机头1电流值
	if(transportp[0] == 1)
	{
		memset(sensor,0x00,sizeof(sensor));
		memset(numberbuf,0x00,sizeof(numberbuf));
		sprintf(numberbuf, "%d", ++j);
		sensor[0] = numberbuf;
		sensor[1] = "机头1";
		memset(buf,0x00,sizeof(buf));
		if(transportplane[0] != 0)
		{
			sprintf(buf,"%d",transportplane[0]);
		}
		else
		{
			sprintf(buf,"%s","***");
		}
		strcat(buf,"A");
		sensor[2] = buf;
		LISTVIEW_AddRow(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW0),sensor);
	}
	//写运输机机头2电流值
	if(transportp[1] == 1)
	{
		memset(sensor,0x00,sizeof(sensor));
		memset(numberbuf,0x00,sizeof(numberbuf));
		sprintf(numberbuf, "%d", ++j);
		sensor[0] = numberbuf;
		sensor[1] = "机头2";
		memset(buf,0x00,sizeof(buf));
		if(transportplane[1] != 0)
		{
			sprintf(buf,"%d",transportplane[1]);
		}
		else
		{
			sprintf(buf,"%s","***");
		}
		strcat(buf,"A");
		sensor[2] = buf;
		LISTVIEW_AddRow(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW0),sensor);
	}
	//写运输机机尾1电流值
	if(transportp[2] == 1)
	{
		memset(sensor,0x00,sizeof(sensor));
		memset(numberbuf,0x00,sizeof(numberbuf));
		sprintf(numberbuf, "%d", ++j);
		sensor[0] = numberbuf;
		sensor[1] = "机尾1";
		memset(buf,0x00,sizeof(buf));
		if(transportplane[2] != 0)
		{
			sprintf(buf,"%d",transportplane[2]);
		}
		else
		{
			sprintf(buf,"%s","***");
		}
		strcat(buf,"A");
		sensor[2] = buf;
		LISTVIEW_AddRow(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW0),sensor);
	}
	//写运输机机尾2电流值
	if(transportp[3] == 1)
	{
		memset(sensor,0x00,sizeof(sensor));
		memset(numberbuf,0x00,sizeof(numberbuf));
		sprintf(numberbuf, "%d", ++j);
		sensor[0] = numberbuf;
		sensor[1] = "机尾2";
		memset(buf,0x00,sizeof(buf));
		if(transportplane[3] != 0)
		{
			sprintf(buf,"%d",transportplane[3]);
		}
		else
		{
			sprintf(buf,"%s","***");
		}
		strcat(buf,"A");
		sensor[2] = buf;
		LISTVIEW_AddRow(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW0),sensor);
	}
	LISTVIEW_SetSel(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW0), row);
}
/*
*********************************************************************************************************
*	函 数 名: 
*	功能说明: 创建菜单列表
*	形    参: 
*	返 回 值: 
*********************************************************************************************************
*/

uint8_t person_refresh= 0;
void Creator_Statewindow(WM_MESSAGE * pMsg,uint8_t count)
{
	uint8_t i,j = 0,k=0;
	uint8_t temp;
	static NetParamType  iptemp;
	uint8_t length;
	static char * sensor[4];
	SensorStateType SensorState;
	char buf[15];
	char value[13];
	char state[13];
	char numberbuf[3];
	static	STR_FAULT_INFO fault_temp;
	static STR_LOG_INFO log_temp;
	SensorParamType Sensorpara;
	stDeviceStatusMsg  Sensorversion;
//	LISTVIEW_CreateEx(59,50,362,270,pMsg->hWin,WM_CF_MEMDEV_ON_REDRAW,0,MENU_LISTVIEW_ID0);
	
	USER_ReviseListview(pMsg);
	SetPageValue(0);
	State_ShowButtonForPage(pMsg,0);

	switch (count) 
	{ 
		case 1:
			person_refresh = 2;
			k = 1;
			USER_ReviseListviewHeader(pMsg,State_LISTVIEW_HEADER1,sizeof(State_LISTVIEW_HEADER1));
			for(i=0;i<SensorShowBUF[0].MAX;i++)
			{
				memset(buf,0x00,sizeof(buf));
				memset(numberbuf,0x00,sizeof(numberbuf));
				sensor[1] = SensorShowBUF[i+1].name;
				GUIGetDateFunAPI(API_Display_Item,SensorShowBUF[i+1].id,&SensorState,&length);	
				GUIGetDateFunAPI(API_Sensor_Version,SensorShowBUF[i+1].id,&Sensorpara,&length);	
				GetCanDeviceStatusMsg(Sensorpara.pointID.driver,Sensorpara.pointID.device,Sensorpara.pointID.subDevice,&Sensorversion);	
				if(Sensorpara.enable == 0)
					continue;
				else 
				{
					sprintf(numberbuf, "%d", k++);
					sensor[0] = numberbuf;
					if(SensorState.state_online == OnlineType_ONLINE)
					{
						if(SensorState.state_fail == FailType_NORMAL)
							sprintf(state,"%s","正常");
						else 
							sprintf(state,"%s","故障");
					}
					else
					{
						sprintf(state,"%s","离线");																
					}					
					sensor[2] = state;
					
					if((SensorState.state_fail == FailType_NORMAL) && (SensorState.state_online == OnlineType_ONLINE))
					{
						if(SensorShowBUF[i+1].acc == 3)
							sprintf(value,"%d.%d",SensorState.calculateValue/10,SensorState.calculateValue%10);
						else if(SensorShowBUF[i+1].acc == 4)
							sprintf(value,"%d.%02d",SensorState.calculateValue/100,SensorState.calculateValue%100);
						else if(SensorShowBUF[i+1].acc == 9)
							sprintf(value,"%d.%d",(int16_t)SensorState.calculateValue/10,SensorState.calculateValue%10);
						else					
							sprintf(value,"%d",SensorState.calculateValue);
					}
					else
					{
						sprintf(value,"%s","***");
					}
					AddUnitDispiay(value,SensorShowBUF[i+1].Nuit);
					sensor[3] = value;				
					LISTVIEW_AddRow(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW0),sensor);
					memset(value,0,sizeof(value));
					memset(state,0,sizeof(state));
				}				
			}
			LISTVIEW_IncSel(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW0));
			WM_SetFocus(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW0));
			LISTVIEW_SetSel(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW0),0);
		break;
		case 2:
			person_refresh = 0;
			USER_ReviseListviewHeader(pMsg,State_LISTVIEW_HEADER2,sizeof(State_LISTVIEW_HEADER2));
		
			length = getLogLength_API();
			for(i=1;i<length+1;i++)
			{
				memset(sensor,0x00,sizeof(sensor));
				memset(buf,0x00,sizeof(buf));
				memset(numberbuf,0x00,sizeof(numberbuf));
				sprintf(numberbuf, "%d", i);
				sensor[0] = numberbuf;
				ReadLog_API(i,1,&log_temp);
				show_log_type(sensor,log_temp);
				LISTVIEW_AddRow(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW0),sensor);
			}
			LISTVIEW_IncSel(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW0));
			WM_SetFocus(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW0));
			LISTVIEW_SetSel(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW0),0);
		break;	
		
		case 3:

		 person_refresh = 1;
		 USER_ReviseListviewHeader(pMsg,State_LISTVIEW_HEADER3,sizeof(State_LISTVIEW_HEADER3));
		 show_person_refresh(pMsg);
	   	 LISTVIEW_SetSel(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW0), 0);
		 break;

		case 4://版本
			person_refresh = 0;
			USER_ReviseListviewHeader(pMsg,State_LISTVIEW_HEADER4,sizeof(State_LISTVIEW_HEADER4));	
			//写支架类型
			memset(sensor,0x00,sizeof(sensor));
			memset(numberbuf,0x00,sizeof(numberbuf));
			sprintf(numberbuf, "%d", ++j);
			sensor[0] = numberbuf;
			SysDataFun(DATA_MSG_GET_DEVICETYPE_PARAM, 0,0,0,0,(void *)&temp,0);
			i = temp;
			memset(ControllerVersion,0,sizeof(ControllerVersion));
			sprintf(ControllerVersion,"%s%d%s%d%s%d%s%d", "V",V4_CONTROLLER_VERSION_BYTE1,"."
					,V4_CONTROLLER_VERSION_BYTE2,".",V4_CONTROLLER_VERSION_BYTE3,"."
		            ,V4_CONTROLLER_VERSION_BYTE4);
			sensor[1] = SupportType_Grope[i];
			sensor[2] = ControllerVersion;
			LISTVIEW_AddRow(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW0),sensor);
			//写IP
			memset(sensor,0x00,sizeof(sensor));
			memset(numberbuf,0x00,sizeof(numberbuf));
			sprintf(numberbuf, "%d", ++j);
			sensor[0] = numberbuf;
			SysDataFun(DATA_MSG_GET_IP_PARAM, 0,0,0,0,(void *)&iptemp,0);
			memset(buf,0x00,sizeof(buf));
			sprintf(buf, "%d.%d.%d.%d", iptemp.NetParam_IPAddress[0],iptemp.NetParam_IPAddress[1],iptemp.NetParam_IPAddress[2],iptemp.NetParam_IPAddress[3]);	
			sensor[2] = buf;
			sensor[1] = "IP地址";
			LISTVIEW_AddRow(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW0),sensor);
			//写惯导数据
//			memset(sensor,0x00,sizeof(sensor));
//			memset(numberbuf,0x00,sizeof(numberbuf));
//			sprintf(numberbuf, "%d", ++j);
//			sensor[0] = numberbuf;
//			sensor[1] = "惯导找直";
//			memset(buf,0x00,sizeof(buf));
//			if(InsServer)
//				sprintf(buf, "%dmm", InsServer);	
//			else
//				sprintf(buf, "%s", "未启动");
//			sensor[2] = buf;
//			LISTVIEW_AddRow(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW0),sensor);
//			//写控制器版本
//			for(i=0;i<GUI_COUNTOF(ListViewTable6);i++)
//			{
//				++j;
//				LISTVIEW_AddRow(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW0),ListViewTable6[i]);
//			}
			for(i=1;i<SensorShowBUF[0].MAX+1;i++)
			{
				GUIGetDateFunAPI(API_Sensor_Version,SensorShowBUF[i].id,&Sensorpara,&length);
				Sensorpara.pointID.driver -=1;
				if((Sensorpara.pointID.driver == 0) ||(Sensorpara.pointID.driver == 1))
				{
					memset(sensor,0x00,sizeof(sensor));
					memset(numberbuf,0x00,sizeof(numberbuf));
					sprintf(numberbuf, "%d", ++j);
					sensor[0] = numberbuf;
					GetCanDeviceStatusMsg(Sensorpara.pointID.driver,Sensorpara.pointID.device,Sensorpara.pointID.subDevice,&Sensorversion);
					if((Sensorversion.isOnline) &&(Sensorversion.isEnable))
					{
						memset(buf,0x00,sizeof(buf));
						sprintf(buf, "V%d.%d.%d", Sensorversion.deviceVersion[0],Sensorversion.deviceVersion[1],Sensorversion.deviceVersion[2]);
						sensor[2] = buf;
						sensor[1] = SensorShowBUF[i].name;
					}
					else if(Sensorversion.isOnline == 0)
					{
						sensor[2] = "设备离线";
						sensor[1] = SensorShowBUF[i].name;
					}
					else if((Sensorversion.isOnline) &&(Sensorversion.isEnable == 0))
					{
						sensor[2] = "未使能";
						sensor[1] = SensorShowBUF[i].name;
					}
					LISTVIEW_AddRow(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW0),sensor);
					if((Sensorversion.isOnline) &&(Sensorversion.isEnable))
					{
						if((Sensorversion.deviceBLVersion[0] != 0) || (Sensorversion.deviceBLVersion[1] != 0)||(Sensorversion.deviceBLVersion[2] != 0))
						{
							memset(numberbuf,0x00,sizeof(numberbuf));
							sprintf(numberbuf, "%d", ++j);
							sensor[0] = numberbuf;	
							memset(buf,0x00,sizeof(buf));
							sprintf(buf, "V%d.%d.%d", Sensorversion.deviceBLVersion[0],Sensorversion.deviceBLVersion[1],Sensorversion.deviceBLVersion[2]);
							sensor[2] = buf;		
							memset(value,0,sizeof(value));
							memcpy(value,SensorShowBUF[i].name,sizeof(SensorShowBUF[i].name));					
							sensor[1] = value;
							strcat(sensor[1],"BL");						
							LISTVIEW_AddRow(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW0),sensor);
						}
			    	}
				}
			}
			if(GetDeviceCanMsg(ID_PERSON,0,&length))
			{
				GetCanDeviceStatusMsg(length,ID_PERSON,0,&Sensorversion);
				if(Sensorversion.isOnline)
				{
					memset(numberbuf,0x00,sizeof(numberbuf));
					sprintf(numberbuf, "%d", ++j);
					sensor[0] = numberbuf;
					if((Sensorversion.isOnline) &&(Sensorversion.isEnable))
					{
						memset(buf,0x00,sizeof(buf));
						sprintf(buf, "V%d.%d.%d", Sensorversion.deviceVersion[0],Sensorversion.deviceVersion[1],Sensorversion.deviceVersion[2]);
						sensor[2] = buf;
						sensor[1] = "人员定位";
					}
					else
					{
						sensor[2] = "未使能";
						sensor[1] = "人员定位";
					}
					LISTVIEW_AddRow(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW0),sensor);
					if((Sensorversion.isOnline) &&(Sensorversion.isEnable))
					{
						if((Sensorversion.deviceBLVersion[0] != 0) || (Sensorversion.deviceBLVersion[1] != 0)||(Sensorversion.deviceBLVersion[2] != 0))
						{
							memset(numberbuf,0x00,sizeof(numberbuf));
							sprintf(numberbuf, "%d", ++j);
							sensor[0] = numberbuf;	
							memset(buf,0x00,sizeof(buf));
							sprintf(buf, "V%d.%d.%d", Sensorversion.deviceBLVersion[0],Sensorversion.deviceBLVersion[1],Sensorversion.deviceBLVersion[2]);
							sensor[2] = buf;
							sensor[1] = "人员定位BL";
							LISTVIEW_AddRow(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW0),sensor);
						}
					}
				}
			}
			if(GetDeviceCanMsg(ID_ALARM,0,&length))
			{
				GetCanDeviceStatusMsg(length,ID_ALARM,0,&Sensorversion);
				if(Sensorversion.isOnline)
				{
					memset(numberbuf,0x00,sizeof(numberbuf));
					sprintf(numberbuf, "%d", ++j);
					sensor[0] = numberbuf;
					if((Sensorversion.isOnline) &&(Sensorversion.isEnable))
					{
						memset(buf,0x00,sizeof(buf));
						sprintf(buf, "V%d.%d.%d", Sensorversion.deviceVersion[0],Sensorversion.deviceVersion[1],Sensorversion.deviceVersion[2]);
						sensor[2] = buf;
						sensor[1] = "声光报警";
					}
					else 
					{
						sensor[2] = "未使能";
						sensor[1] = "声光报警";
					}
					LISTVIEW_AddRow(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW0),sensor);
					if((Sensorversion.isOnline) &&(Sensorversion.isEnable))
					{					
						if((Sensorversion.deviceBLVersion[0] != 0) || (Sensorversion.deviceBLVersion[0] != 0)||(Sensorversion.deviceBLVersion[0] != 0))
						{
							memset(numberbuf,0x00,sizeof(numberbuf));
							sprintf(numberbuf, "%d", ++j);
							sensor[0] = numberbuf;	
							memset(buf,0x00,sizeof(buf));
							sprintf(buf, "V%d.%d.%d", Sensorversion.deviceBLVersion[0],Sensorversion.deviceBLVersion[1],Sensorversion.deviceBLVersion[2]);
							sensor[2] = buf;
							sensor[1] = "声光报警BL";
							LISTVIEW_AddRow(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW0),sensor);
						}
				    }
				}
			}
			if(GetDeviceCanMsg(ID_IRRX,0,&length))
			{
				GetCanDeviceStatusMsg(length,ID_IRRX,0,&Sensorversion);
				if(Sensorversion.isOnline)
				{
					memset(numberbuf,0x00,sizeof(numberbuf));
					sprintf(numberbuf, "%d", ++j);
					sensor[0] = numberbuf;
					if((Sensorversion.isOnline) &&(Sensorversion.isEnable))
					{
						memset(buf,0x00,sizeof(buf));
						sprintf(buf, "V%d.%d.%d", Sensorversion.deviceVersion[0],Sensorversion.deviceVersion[1],Sensorversion.deviceVersion[2]);
						sensor[2] = buf;
						sensor[1] = "红外接收";
					}
					else
					{
						sensor[2] = "未使能";
						sensor[1] = "红外接收";
					}
					LISTVIEW_AddRow(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW0),sensor);
					if((Sensorversion.isOnline) &&(Sensorversion.isEnable))
					{
						if((Sensorversion.deviceBLVersion[0] != 0) || (Sensorversion.deviceBLVersion[1] != 0)||(Sensorversion.deviceBLVersion[2] != 0))
						{
							memset(numberbuf,0x00,sizeof(numberbuf));
							sprintf(numberbuf, "%d", ++j);
							sensor[0] = numberbuf;	
							memset(buf,0x00,sizeof(buf));
							sprintf(buf, "V%d.%d.%d", Sensorversion.deviceBLVersion[0],Sensorversion.deviceBLVersion[1],Sensorversion.deviceBLVersion[2]);
							sensor[2] = buf;
							sensor[1] = "红外接收BL";
							LISTVIEW_AddRow(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW0),sensor);
						}
					}
				}
			}
			if(GetDeviceCanMsg(ID_EMVD,0,&length))
			{
				GetCanDeviceStatusMsg(length,ID_EMVD,0,&Sensorversion);
				if(Sensorversion.isOnline)
				{
					memset(numberbuf,0x00,sizeof(numberbuf));
					sprintf(numberbuf, "%d", ++j);
					sensor[0] = numberbuf;
					if((Sensorversion.isOnline) &&(Sensorversion.isEnable))
					{
						memset(buf,0x00,sizeof(buf));
						sprintf(buf, "V%d.%d.%d", Sensorversion.deviceVersion[0],Sensorversion.deviceVersion[1],Sensorversion.deviceVersion[2]);
						sensor[2] = buf;
						sensor[1] = "电磁阀";
					}
					else
					{
						sensor[2] = "未使能";
						sensor[1] = "电磁阀";
					}
					LISTVIEW_AddRow(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW0),sensor);
					if((Sensorversion.isOnline) &&(Sensorversion.isEnable))
					{
						if((Sensorversion.deviceBLVersion[0] != 0) || (Sensorversion.deviceBLVersion[1] != 0)||(Sensorversion.deviceBLVersion[2] != 0))
						{
							memset(numberbuf,0x00,sizeof(numberbuf));
							sprintf(numberbuf, "%d", ++j);
							sensor[0] = numberbuf;	
							memset(buf,0x00,sizeof(buf));
							sprintf(buf, "V%d.%d.%d", Sensorversion.deviceBLVersion[0],Sensorversion.deviceBLVersion[1],Sensorversion.deviceBLVersion[2]);
							sensor[2] = buf;
							sensor[1] = "电磁阀BL";
							LISTVIEW_AddRow(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW0),sensor);
						}
					}
				}
			}
			//ghy 增加HUB版本号显示
			for(i = 0;i < 3;i++)
			{
				if(GetDeviceCanMsg(ID_HUB1 + i,  i*5 , &length))
				{
					GetCanDeviceStatusMsg(length,ID_HUB1 + i,0,&Sensorversion);
					if(Sensorversion.isOnline)
					{
						memset(numberbuf,0x00,sizeof(numberbuf));
						sprintf(numberbuf, "%d", ++j);
						sensor[0] = numberbuf;
						if((Sensorversion.isOnline) &&(Sensorversion.isEnable))
						{
							memset(buf,0x00,sizeof(buf));
							sprintf(buf, "V%d.%d.%d", Sensorversion.deviceVersion[0],Sensorversion.deviceVersion[1],Sensorversion.deviceVersion[2]);
							sensor[2] = buf;
							switch(i)
							{
								case 0x00:
									sensor[1] = "HUB1";
									break;
								case 0x01:
									sensor[1] = "HUB2";
									break;
								case 0x02:
									sensor[1] = "HUB3";
									break;
								default:
									break;
							}
						}
						else
						{
							sensor[2] = "未使能";							
							switch(i)
							{
								case 0x00:
									sensor[1] = "HUB1";
									break;
								case 0x01:
									sensor[1] = "HUB2";
									break;
								case 0x02:
									sensor[1] = "HUB3";
									break;
								default:
									break;
							}
						}
						LISTVIEW_AddRow(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW0),sensor);
					}
				}
			}
			memset(numberbuf,0x00,sizeof(numberbuf));
			sprintf(numberbuf, "%d", ++j);
			sensor[0] = numberbuf;	
			sprintf(buf, "V%d.%d.%d", KeyVersion[0],KeyVersion[1],KeyVersion[2]);
			sensor[2] = buf;
			sensor[1] = "键盘模块";
			LISTVIEW_AddRow(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW0),sensor);
			
			
			memset(numberbuf,0x00,sizeof(numberbuf));
			sprintf(numberbuf, "%d", ++j);
			sensor[0] = numberbuf;
			sprintf(buf, "V%d.%d.%d", WirelessVersion[0],WirelessVersion[1],WirelessVersion[2]);
			sensor[2] = buf;
			sensor[1] = "无线模块";
			LISTVIEW_AddRow(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW0),sensor);
			
			
			LISTVIEW_IncSel(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW0));
			WM_SetFocus(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW0));
			LISTVIEW_SetSel(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW0),0);
		break;
		case 5:
			person_refresh = 0;
			USER_ReviseListviewHeader(pMsg,State_LISTVIEW_HEADER5,sizeof(State_LISTVIEW_HEADER5));
			length = getFaultLength_API();
			for(i=0;i<length;i++)
			{
				memset(sensor,0x00,sizeof(sensor));
				memset(buf,0x00,sizeof(buf));
				memset(numberbuf,0x00,sizeof(numberbuf));
				sprintf(numberbuf, "%d", i+1);
				sensor[0] = numberbuf;
				ReadFault_API(i,1,&fault_temp);
				show_fault_type(sensor,fault_temp);
				sprintf(buf, "%d:%d:%d", (uint8_t)(fault_temp.time>>16),(uint8_t)(fault_temp.time>>8),(uint8_t)(fault_temp.time));
				sensor[1] = buf;
				LISTVIEW_AddRow(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW0),sensor);
			}
			LISTVIEW_IncSel(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW0));
			WM_SetFocus(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW0));
			LISTVIEW_SetSel(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW0),0);
		break;
			
		case 6:
			person_refresh = 3;
			USER_ReviseListviewHeader(pMsg,State_LISTVIEW_HEADER6,sizeof(State_LISTVIEW_HEADER6));	
			//写跟机状态
			show_function_refresh(pMsg);
			LISTVIEW_SetSel(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW0), 0);

		break;	
	}	
			
			
			
//	USER_ShwoListviewData(pMsg,GetListviewData(id));

}
/*
*********************************************************************************************************
*	函 数 名: 
*	功能说明: 根据当前页面显示按键
*	形    参: 
*	返 回 值: 
*********************************************************************************************************
*/
void State_ShowButtonForPage(WM_MESSAGE * pMsg,uint32_t id)
{
	USER_ReviseAllButtonShow(pMsg,StateButtonName[0]);
}
/*
*********************************************************************************************************
*	函 数 名: 
*	功能说明: 根据当前页面显示列表
*	形    参: 
*	返 回 值: 
*********************************************************************************************************
*/
void State_ShowListviewForPage(WM_MESSAGE * pMsg,uint32_t id)
{
	USER_ShwoListviewData(pMsg,GetListviewData(id));
}
/*
*********************************************************************************************************
*	函 数 名: 
*	功能说明: 初始化菜单文本
*	形    参: 
*	返 回 值: 
*********************************************************************************************************
*/	
void InitStateText(WM_MESSAGE * pMsg)
{
	
	WM_HWIN hItem;
	hItem = WM_GetDialogItem(pMsg->hWin, MENU_TEXT_ID0);
	TEXT_SetFont(hItem, &GUI_FontHZ24);
	TEXT_SetText(hItem,"状态界面");
	TEXT_SetTextColor(hItem,GUI_WHITE);
	TEXT_SetTextAlign(hItem,GUI_TA_HCENTER | GUI_TA_VCENTER);
}

