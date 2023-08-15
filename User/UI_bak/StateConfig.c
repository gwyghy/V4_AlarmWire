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
extern NameAndId   SingleActionBUF[];
extern NameAndId   GroupActionBUF[];
extern SensorShow   SensorShowBUF[];
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
	{40,""},
	{125,"动作名"},
	{110,"类型"},
	{85,"状态"},
};
const LISTVIEW_HEADER State_LISTVIEW_HEADER3[] =
{
	{40,""},
	{125,"ID"},
	{85,"距离"},
	{110,"权限"},
};
const LISTVIEW_HEADER State_LISTVIEW_HEADER5[] =
{
	{40,""},
	{150,"时间"},
	{170,"名称"},
};
const LISTVIEW_HEADER State_LISTVIEW_HEADER6[] =
{
	{40,""},
	{160,"名称"},
	{160,"版本"},
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
	{"传感器","日志","人员","版本","故障"," "},
};

static const char *ListViewTable1[][4]={
	{"1",	"前柱压力","正常","15.0Mpa"},
	{"2",	"后柱压力","正常","16.0Mpa"},
	{"3",	"推移行程","正常","900mm"},
	{"4",	"支架高度","异常","***mm"},
	{"5",	"顶梁倾角","正常","7.0°"},
	{"6",	"底座倾角","正常","3.5°"},
	{"7",	"尾梁行程","正常","200mm"},
	{"8",	"护帮压力","正常","5.8Mpa"},
};
static const char *ListViewTable2[][4]={
	{"1",	"前柱压力","正常","15.0Mpa"},
	{"2",	"后柱压力","正常","16.0Mpa"},
	{"3",	"推移行程","正常","900mm"},
	{"4",	"支架高度","异常","***mm"},
	{"5",	"顶梁倾角","正常","7.0°"},
	{"6",	"底座倾角","正常","3.5°"},
	{"7",	"尾梁行程","正常","200mm"},
	{"8",	"护帮压力","正常","5.8Mpa"},
};
static const char *ListViewTable3[][4]={
	{"1",	"311","150cm","1级"},
	{"2",	"322","160cm","5级"},
	{"3",	"323","250cm","2级"},
	{"4",	"324","288cm","8级"},
	{"5",	"325","582cm","4级"},
	{"6",	"326","522cm","1级"},
	{"7",	"327","782cm","9级"},
	{"8",	"328","412cm","15级"},
};
static const char *ListViewTable5[][4]={
	{"1",	"高度传感器故障"},
};
static const char *ListViewTable6[][4]={
	{"1",	"架型","中间架"},
	{"2",	"IP","192.168.25.123"},
	{"3",	"版本","V4.21.11"},
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

	switch(val.host_type)
	{
		case FAULT_HOST_SENSOR:
			for(i=1;i<SensorShowBUF[0].MAX+1;i++)
			{
				if(val.host_name == SensorShowBUF[i].id)
				{
					show[2] = SensorShowBUF[i].name;
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
				case FAULT_EVENT_ALL_PARA_NOT_MATCH_NEIGHBOR:
					show[2] = "全局参数邻架不符";
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
			show[2] = "报警";
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
			show[3] = "传感器正常";
			break;
		case LOG_STATUS_SENSOR_ALARM:
			show[3] = "传感器报警";
			break;
		default:
			break;
	}
}



void show_person_refresh(WM_MESSAGE * pMsg)
{
 uint8_t i,j=0;
 uint8_t length;
 uint32_t person_temp;
 static char * sensor[4];
 SensorStateType SensorState;
 char buf[15];
 char value[13];
 char state[13];
 char numberbuf[3];
 LISTVIEW_DeleteAllRows(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW0));
 GetDeviceCanMsg(ID_PERSON,0,&length); 
 for(i=0;i<11;i++)
 {
  memset(sensor,0x00,sizeof(sensor));
  GetCanDeviceAPPMsg(length,ID_PERSON,i,1,&person_temp);
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
  GetCanDeviceAPPMsg(length,ID_PERSON,i,0,&person_temp);//权限
  person_temp  = (person_temp>>12)&0x0000FF;
  sprintf(value, "%d级", person_temp);
  sensor[3] = value;
  GetCanDeviceAPPMsg(length,ID_PERSON,i,2,&person_temp);//距离
  sprintf(state, "%dcm", person_temp);
  sensor[2] = state; 
  LISTVIEW_AddRow(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW0),sensor);
 }
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
	uint8_t i,j=0;
	uint8_t length;
	uint32_t person_temp;
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
	State_ShowButtonForPage(pMsg,0);
	LISTVIEW_IncSel(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW0));
	WM_SetFocus(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW0));
	LISTVIEW_SetSel(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW0),0);
	switch (count) 
	{ 
		case 1:
			person_refresh = 0;
			USER_ReviseListviewHeader(pMsg,State_LISTVIEW_HEADER1,sizeof(State_LISTVIEW_HEADER1));
			for(i=0;i<SensorShowBUF[0].MAX;i++)
			{
				memset(buf,0x00,sizeof(buf));
				memset(numberbuf,0x00,sizeof(numberbuf));
				sprintf(numberbuf, "%d", i+1);
				sensor[0] = numberbuf;
				sensor[1] = SensorShowBUF[i+1].name;
				GUIGetDateFunAPI(API_Display_Item,SensorShowBUF[i+1].id,&SensorState,&length);
				if(SensorState.state_online == 1)
				{
					if(SensorState.state_fail == 1)
						sprintf(state,"%s","正常");
					else 
						sprintf(state,"%s","故障");
				}
				else
					sprintf(state,"%s","离线");
				sensor[2] = state;
				if(SensorShowBUF[i+1].acc == 3)
					sprintf(value,"%d.%d",SensorState.calculateValue/10,SensorState.calculateValue%10);
				else if(SensorShowBUF[i+1].acc == 4)
					sprintf(value,"%d.%02d",SensorState.calculateValue/100,SensorState.calculateValue%100);
				else
					sprintf(value,"%d",SensorState.calculateValue);
				AddUnitDispiay(value,SensorShowBUF[i+1].Nuit);
				sensor[3] = value;				
				LISTVIEW_AddRow(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW0),sensor);
				memset(value,0,sizeof(value));
				memset(state,0,sizeof(state));
			}
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
		break;	
		case 3:

		 person_refresh = 1;
		 USER_ReviseListviewHeader(pMsg,State_LISTVIEW_HEADER3,sizeof(State_LISTVIEW_HEADER3));
		 show_person_refresh(pMsg);
		 break;

		case 4://版本
			person_refresh = 0;
			USER_ReviseListviewHeader(pMsg,State_LISTVIEW_HEADER6,sizeof(State_LISTVIEW_HEADER6));
//			for(i=0;i<GUI_COUNTOF(ListViewTable6);i++)
//			{
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
					if(Sensorversion.isOnline)
					{
						memset(buf,0x00,sizeof(buf));
						sprintf(buf, "V%d.%d.%d", Sensorversion.deviceVersion[0],Sensorversion.deviceVersion[1],Sensorversion.deviceVersion[2]);
						sensor[2] = buf;
						sensor[1] = SensorShowBUF[i].name;
					}
					else
					{
						sensor[2] = "设备离线";
						sensor[1] = SensorShowBUF[i].name;
					}
					LISTVIEW_AddRow(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW0),sensor);
					if((Sensorversion.deviceBLVersion[0] != 0) || (Sensorversion.deviceBLVersion[0] != 0)||(Sensorversion.deviceBLVersion[0] != 0))
					{
						memset(numberbuf,0x00,sizeof(numberbuf));
						sprintf(numberbuf, "%d", ++j);
						sensor[0] = numberbuf;	
						memset(buf,0x00,sizeof(buf));
						sprintf(buf, "V%d.%d.%d", Sensorversion.deviceBLVersion[0],Sensorversion.deviceBLVersion[1],Sensorversion.deviceBLVersion[2]);
						sensor[2] = buf;
						sensor[1] = SensorShowBUF[i].name;
						strcat(sensor[1],"BL");
						LISTVIEW_AddRow(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW0),sensor);
					}
				}
			}
			GetDeviceCanMsg(ID_PERSON,0,&length);	
			GetCanDeviceStatusMsg(length,ID_PERSON,0,&Sensorversion);
			{
				if(Sensorversion.isOnline)
				{
					memset(numberbuf,0x00,sizeof(numberbuf));
					sprintf(numberbuf, "%d", ++j);
					sensor[0] = numberbuf;
					if(Sensorversion.isOnline)
					{
						memset(buf,0x00,sizeof(buf));
						sprintf(buf, "V%d.%d.%d", Sensorversion.deviceVersion[0],Sensorversion.deviceVersion[1],Sensorversion.deviceVersion[2]);
						sensor[2] = buf;
						sensor[1] = "人员定位";
					}
					else
					{
						sensor[2] = "设备离线";
						sensor[1] = SensorShowBUF[i].name;
					}
					LISTVIEW_AddRow(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW0),sensor);
					if((Sensorversion.deviceBLVersion[0] != 0) || (Sensorversion.deviceBLVersion[0] != 0)||(Sensorversion.deviceBLVersion[0] != 0))
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
			GetDeviceCanMsg(ID_ALARM,0,&length);	
			GetCanDeviceStatusMsg(length,ID_ALARM,0,&Sensorversion);
			{
				if(Sensorversion.isOnline)
				{
					memset(numberbuf,0x00,sizeof(numberbuf));
					sprintf(numberbuf, "%d", ++j);
					sensor[0] = numberbuf;
					if(Sensorversion.isOnline)
					{
						memset(buf,0x00,sizeof(buf));
						sprintf(buf, "V%d.%d.%d", Sensorversion.deviceVersion[0],Sensorversion.deviceVersion[1],Sensorversion.deviceVersion[2]);
						sensor[2] = buf;
						sensor[1] = "声光报警";
					}
					else
					{
						sensor[2] = "设备离线";
						sensor[1] = SensorShowBUF[i].name;
					}
					LISTVIEW_AddRow(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW0),sensor);
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
			GetDeviceCanMsg(ID_IRRX,0,&length);	
			GetCanDeviceStatusMsg(length,ID_IRRX,0,&Sensorversion);
			{
				if(Sensorversion.isOnline)
				{
					memset(numberbuf,0x00,sizeof(numberbuf));
					sprintf(numberbuf, "%d", ++j);
					sensor[0] = numberbuf;
					if(Sensorversion.isOnline)
					{
						memset(buf,0x00,sizeof(buf));
						sprintf(buf, "V%d.%d.%d", Sensorversion.deviceVersion[0],Sensorversion.deviceVersion[1],Sensorversion.deviceVersion[2]);
						sensor[2] = buf;
						sensor[1] = "红外接收";
					}
					else
					{
						sensor[2] = "设备离线";
						sensor[1] = SensorShowBUF[i].name;
					}
					LISTVIEW_AddRow(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW0),sensor);
					if((Sensorversion.deviceBLVersion[0] != 0) || (Sensorversion.deviceBLVersion[0] != 0)||(Sensorversion.deviceBLVersion[0] != 0))
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
			GetDeviceCanMsg(ID_EMVD,0,&length);	
			GetCanDeviceStatusMsg(length,ID_EMVD,0,&Sensorversion);
			{
				if(Sensorversion.isOnline)
				{
					memset(numberbuf,0x00,sizeof(numberbuf));
					sprintf(numberbuf, "%d", ++j);
					sensor[0] = numberbuf;
					if(Sensorversion.isOnline)
					{
						memset(buf,0x00,sizeof(buf));
						sprintf(buf, "V%d.%d.%d", Sensorversion.deviceVersion[0],Sensorversion.deviceVersion[1],Sensorversion.deviceVersion[2]);
						sensor[2] = buf;
						sensor[1] = "电磁阀";
					}
					else
					{
						sensor[2] = "设备离线";
						sensor[1] = SensorShowBUF[i].name;
					}
					LISTVIEW_AddRow(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW0),sensor);
					if((Sensorversion.deviceBLVersion[0] != 0) || (Sensorversion.deviceBLVersion[0] != 0)||(Sensorversion.deviceBLVersion[0] != 0))
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
		break;
			
		case 6:

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

























