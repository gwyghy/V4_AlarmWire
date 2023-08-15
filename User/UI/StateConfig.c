/*
*********************************************************************************************************
*	                                  
*	ģ������ : 
*	�ļ����� : MenuConfig.c
*	��    �� : V1.0
*	˵    �� : ʵ�ְ������ֹ���
*              
*	�޸ļ�¼ :
*		�汾��   ����         ����          ˵��
*		V1.0    2016-07-16   Eric2013  	    �װ�    
*                                     
*	Copyright (C), 2015-2020, ���������� www.armfly.com
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
*	                                     �궨��
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*	                                    ȫ�ֱ���
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
*	                                     ֧�ܿ������汾��
*********************************************************************************************************
*/
//char *ControllerVersion = "V8.1.0.0";
static char ControllerVersion[20];
/*
*********************************************************************************************************
*	                                     ����������������������
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*	                                     �б�ͷ����
*********************************************************************************************************
*/
const LISTVIEW_HEADER State_LISTVIEW_HEADER1[] =
{
	{40,""},
	{125,"����"},
	{85,"״̬"},
	{110,"��ֵ"},
};
const LISTVIEW_HEADER State_LISTVIEW_HEADER2[] =
{
	{35,""},
	{115,"������"},
	{105,"����"},
	{105,"״̬"},
};
const LISTVIEW_HEADER State_LISTVIEW_HEADER3[] =
{
	{40,""},
	{125,"ID"},
	{85,"����"},
	{110,"Ȩ��"},
};
const LISTVIEW_HEADER State_LISTVIEW_HEADER4[] =
{
	{40,""},
	{160,"����"},
	{160,"�汾"},
};
const LISTVIEW_HEADER State_LISTVIEW_HEADER5[] =
{
	{40,""},
	{130,"ʱ��"},
	{190,"����"},
};
const LISTVIEW_HEADER State_LISTVIEW_HEADER6[] =
{
	{40,""},
	{160,"����"},
	{160,"״̬"},
};
/*
*********************************************************************************************************
*	                                     ѡ�������
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*	                                    �������ƶ���
*********************************************************************************************************
*/

const char *StateButtonName[][6] = 
{
	{"������","��־","��Ա","�汾","����","����"},
};

/*
*********************************************************************************************************
*	                                    �ı����ƶ���
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
					strcat(show[2],"����");
				}
			}
			break;	
		case FAULT_HOST_DEVICE:
			switch(val.host_name)
			{
				case ID_EMVD:
					show[2] = "��ŷ�����";
					break;
				case ID_ALARM:
					show[2] = "���ⱨ������";
					break;
				case ID_PERSON:
					show[2] = "��Ա��λ����";
					break;
				case ID_IRRX:
					show[2] = "������չ���";
					break;
				default:
					break;
			}
			break;	
		//ghy ���HUB������Ϣ��¼
		case  FAULT_HOST_HUB1:	
			switch(val.host_name)
			{
				case ID_HUB1:
					show[2] = "HUB1����";
				break;
				default:
				break;
			}
			break;
		 case  FAULT_HOST_HUB2:	
			switch(val.host_name)
			{
				case ID_HUB2:
					show[2] = "HUB2����";
				break;
				default:
				break;
			}
			break;	
		 case  FAULT_HOST_HUB3:	
			switch(val.host_name)
			{
				case ID_HUB3:
					show[2] = "HUB3����";
				break;
				default:
				break;
			}
			break;
			
		case FAULT_HOST_OTHER:
			switch(val.event)
			{
				case FAULT_EVENT_FAULT:
					show[2] = "����������";
					break;
				case FAULT_EVENT_OFFLINE:
					show[2] = "�豸����";
					break;
				case FAULT_EVENT_WORKFACE_COMMUNICATION_WRONG:
					show[2] = "������ͨ���쳣";
					break;
				case FAULT_EVENT_SYSTEM_NUMBER_WRONG:
					show[2] = "ϵͳ��Ŵ���";
					break;
				case FAULT_EVENT_SERVER_WRONG:
					show[2] = "SSλ�û�ʹ�ܲ���";
					break;
				case FAULT_EVENT_NUMBER_ADD_DIRECT_WRONG:
					show[2] = "�ܺ����򲻷�";
					break;
				case FAULT_EVENT_NUMBER_RANGE_WRONG:
					show[2] = "�ܺŷ�Χ��������";
					break;
				case FAULT_EVENT_PARA_UPDATE_WRONG:
					show[2] = "��������ʧ��";
					break;	
				case FAULT_EVENT_ALL_PARA_NOT_SAME:
					show[2] = "ȫ�ֲ�����һ��";
					break;
				case FAULT_EVENT_ALL_PARA_NOT_MATCH_LEFTNEIGHBOR:
					show[2] = "�������ڼܲ���";
					break;
				case FAULT_EVENT_ALL_PARA_NOT_MATCH_RIGHTNEIGHBOR:
					show[2] = "�������ڼܲ���";
					break;
				case FAULT_EVENT_PARGRAM_NOT_SAME:
					show[2] = "����汾��һ��";
					break;
				case FAULT_EVENT_PARGRAM_NOT_MATCH_NEIGHBOR:
					show[2] = "�汾�����ڼܲ���";
					break;
				case FAULT_EVENT_HARD_SOFT_NOT_MATCH:
					show[2] = "Ӳ�����������";
					break;
				case FAULT_EVENT_LEFT_NEIGHBOR_COMMUNICATION_WRONG:
					show[2] = "���ڼ�ͨ�Ŵ���";
					break;	
				case FAULT_EVENT_RIGHT_NEIGHBOR_COMMUNICATION_WRONG:
					show[2] = "���ڼ�ͨ�Ŵ���";
					break;	
				case FAULT_EVENT_ABNORMAL_COMMUNICATION_WITH_SERVER:
					show[2] = "Զ������ͨ���쳣";//�������ͨ���쳣
					break;
				case FAULT_EVENT_SYSPARAM_MAXSUPPORTNUM_NOTSAME:
					show[2] = "���ܺŲ�һ��";
					break;	
				case FAULT_EVENT_SYSPARAM_MINSUPPORTNUM_NOTSAME:
					show[2] = "��С�ܺŲ�һ��";
					break;	
				case FAULT_EVENT_SYSPARAM_SUPPORTNUMINCREASE_NOTSAME:
					show[2] = "�ܺ�����һ��";
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
			show[2] = "������";
			for(i=1;i<SingleActionBUF[1].MAX+1;i++)
			{
				if(val.host == SingleActionBUF[i].id)
				{
					show[1] = SingleActionBUF[i].name;
				}
			}
			
			break;
		case LOG_TYPE_HANDLE_COMB:
			show[2] = "��϶���";
			for(i=SingleActionBUF[1].MAX+1;i<SingleActionBUF[0].MAX+1;i++)
			{
				if(val.host == SingleActionBUF[i].id)
				{
					show[1] = SingleActionBUF[i].name;
				}
			}
			break;
		case LOG_TYPE_HANDLE_GROUP:
			show[2] = "���鶯��";
			for(i=1;i<GroupActionBUF[0].MAX+1;i++)
			{
				if(val.host == GroupActionBUF[i].id)
				{
					show[1] = GroupActionBUF[i].name;
				}
			}
			break;
		case LOG_TYPE_HANDLE_SYSTEM:
			show[2] = "ϵͳ����";
			break;
		case LOG_TYPE_SERVICE:
			show[2] = "����";
			break;
		case LOG_TYPE_ALARM:
			show[2] = "������";
			for(i=1;i<SensorShowBUF[0].MAX+1;i++)
			{
				if(val.host == SensorShowBUF[i].id)
				{
					show[1] = SensorShowBUF[i].name;
				}
			}
			break;
		case LOG_TYPE_SYSTEM_ALARM:
			show[2] = "����";
			show[1] = "ϵͳ";
			break;
		case LOG_TYPE_UPDATE_PROGRAM:
			show[2] = "����";
			show[1] = "����";
			break;
		case LOG_TYPE_RESET_SYSTEM:
			show[2] = "��λ";
			show[3] = " ";
			break;
		case LOG_TYPE_JSON_TEXT:
			show[2] = "�����ļ�";
			show[3] = "����";
			break;
		default:
			break;
	}
	switch(val.status)
	{
		case LOG_STATUS_START:
			show[3] = "����";
			break;
		case LOG_STATUS_STOP:
			show[3] = "ֹͣ";
			break;
		case LOG_STATUS_DELAY_START:
			show[3] = "��ʱ����";
			break;
		case LOG_STATUS_SUSPEND:
			show[3] = "��ͣ";
			break;
		case LOG_STATUS_INPLACE:
			show[3] = "��λ";
			break;
		case LOG_STATUS_DISABLE:
			show[3] = "��ֹ";
			break;
		case LOG_STATUS_OVERTIME:
			show[3] = "��ʱ";
			break;
		case LOG_STATUS_SENSOR_NORMAL:
			show[3] = "����";
			break;
		case LOG_STATUS_SENSOR_ALARM:
			show[3] = "����";
			break;
		case LOG_STATUS_EMERGENCY_ALARM:
			show[3] = "��ͣ";
			break;
		case LOG_STATUS_EMERGENCY_NORMAL:
			show[3] = "��ͣ�ָ�";
			break;
		case LOG_STATUS_BISUO_ALARM:
			show[3] = "����";
			break;
		case LOG_STATUS_BISUO_NORMAL:
			show[3] = "�����ָ�";
			break;
		case LOG_STATUS_DOWNLOAD_SUCCESS:
			show[3] = "���سɹ�";
			break;
		case LOG_STATUS_DOWNLOAD_FAIL:
			show[3] = "����ʧ��";
			break;
		case LOG_STATUS_UPDATE_PRO_SUCCESS:
			show[3] = "����ɹ�";
			break;
		case LOG_STAUTS_UPDATE_PRO_FAIL:
			show[3] = "����ʧ��";
			break;
		case LOG_STATUS_UPDATE_PARA_SUCCESS:
			show[3] = "�����ɹ�";
			break;
		case LOG_STATUS_UPDATE_PARA_FAIL:
			show[3] = "����ʧ��";
			break;
		case LOG_STATUS_RESET_LOWPAWER:
			show[1] = "�͹���";
			break;
		case LOG_STATUS_RESET_WWATCHDOG:
			show[1] = "���ڿ��Ź�";
			break;
		case LOG_STATUS_RESET_IWATCHDOG:
			show[1] = "�������Ź�";
			break;
		case LOG_STATUS_RESET_SOFTWARE:
			show[1] = "���";
			break;
		case LOG_STATUS_RESET_PORRSTF:
			show[1] = "����";
			break;
		case LOG_STATUS_PERSONLOCK_TIMEOUT:
			show[3] = "��Ա����";
			break;
		case LOG_STATUS_NET_REINIT:
			show[3] = "��������";
			break;
		case LOG_STATUS_START_OUT:
			show[1] = "�ⲿ";
			break;		
		case LOG_STATUS_START_IN:
			show[1] = "�ڲ�";
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
					sprintf(state,"%s","����");
				else 
					sprintf(state,"%s","����");
			}
			else
			{
				sprintf(state,"%s","����");																
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
*	�� �� ��: 
*	����˵��: �����˵��б�
*	��    ��: 
*	�� �� ֵ: 
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
		GetCanDeviceAPPMsg(device,ID_PERSON,i,0,&person_temp);//Ȩ��
		person_temp  = (person_temp>>12)&0x0000FF;
		sprintf(value, "%d��", person_temp);
		sensor[3] = value;
		GetCanDeviceAPPMsg(device,ID_PERSON,i,2,&person_temp);//����
		sprintf(state, "%dcm", person_temp);
		sensor[2] = state; 
		LISTVIEW_AddRow(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW0),sensor);
	}
	LISTVIEW_SetSel(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW0), row);
}
extern u16 InsServer;
extern u16  RecoilState;
extern u16  RecoilNumber;

extern uint16_t transportplane[]; //���������ֵ����
/*
*********************************************************************************************************
*	�� �� ��: 
*	����˵��: �����˵��б�
*	��    ��: 
*	�� �� ֵ: 
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
    //��ȡ�Ƿ���ʾ
	GUIGetDateFunAPI(API_Transport_Plane,0,&transportp,&length);
	
	uint8_t row;
	row = LISTVIEW_GetSel(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW0));
	LISTVIEW_DeleteAllRows(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW0));
	//д�ߵ�����
	memset(sensor,0x00,sizeof(sensor));
	memset(numberbuf,0x00,sizeof(numberbuf));
	sprintf(numberbuf, "%d", ++j);
	sensor[0] = numberbuf;
	sensor[1] = "�ߵ���ֱ";
	memset(buf,0x00,sizeof(buf));
	if(InsServer)
		sprintf(buf, "%dmm", InsServer);	
	else
		sprintf(buf, "%s", "δ����");
	sensor[2] = buf;
	LISTVIEW_AddRow(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW0),sensor);
	//д����ϴ״̬
	memset(sensor,0x00,sizeof(sensor));
	memset(numberbuf,0x00,sizeof(numberbuf));
	sprintf(numberbuf, "%d", ++j);
	sensor[0] = numberbuf;
	sensor[1] = "�Զ�����ϴ";
	memset(buf,0x00,sizeof(buf));
	if(RecoilState)
		sprintf(buf, "%s", "����");	
	else
		sprintf(buf, "%s", "δ����");
	sensor[2] = buf;
	LISTVIEW_AddRow(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW0),sensor);
	//д����ϴ�ܺ�
	if(RecoilState)
	{
		memset(sensor,0x00,sizeof(sensor));
		memset(numberbuf,0x00,sizeof(numberbuf));
		sprintf(numberbuf, "%d", ++j);
		sensor[0] = numberbuf;
		sensor[1] = "���ڳ�ϴ";
		memset(buf,0x00,sizeof(buf));
		sprintf(buf, "%d�ż�", RecoilNumber);	
		sensor[2] = buf;
		LISTVIEW_AddRow(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW0),sensor);
	}
	//д�������ͷ1����ֵ
	if(transportp[0] == 1)
	{
		memset(sensor,0x00,sizeof(sensor));
		memset(numberbuf,0x00,sizeof(numberbuf));
		sprintf(numberbuf, "%d", ++j);
		sensor[0] = numberbuf;
		sensor[1] = "��ͷ1";
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
	//д�������ͷ2����ֵ
	if(transportp[1] == 1)
	{
		memset(sensor,0x00,sizeof(sensor));
		memset(numberbuf,0x00,sizeof(numberbuf));
		sprintf(numberbuf, "%d", ++j);
		sensor[0] = numberbuf;
		sensor[1] = "��ͷ2";
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
	//д�������β1����ֵ
	if(transportp[2] == 1)
	{
		memset(sensor,0x00,sizeof(sensor));
		memset(numberbuf,0x00,sizeof(numberbuf));
		sprintf(numberbuf, "%d", ++j);
		sensor[0] = numberbuf;
		sensor[1] = "��β1";
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
	//д�������β2����ֵ
	if(transportp[3] == 1)
	{
		memset(sensor,0x00,sizeof(sensor));
		memset(numberbuf,0x00,sizeof(numberbuf));
		sprintf(numberbuf, "%d", ++j);
		sensor[0] = numberbuf;
		sensor[1] = "��β2";
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
*	�� �� ��: 
*	����˵��: �����˵��б�
*	��    ��: 
*	�� �� ֵ: 
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
							sprintf(state,"%s","����");
						else 
							sprintf(state,"%s","����");
					}
					else
					{
						sprintf(state,"%s","����");																
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

		case 4://�汾
			person_refresh = 0;
			USER_ReviseListviewHeader(pMsg,State_LISTVIEW_HEADER4,sizeof(State_LISTVIEW_HEADER4));	
			//д֧������
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
			//дIP
			memset(sensor,0x00,sizeof(sensor));
			memset(numberbuf,0x00,sizeof(numberbuf));
			sprintf(numberbuf, "%d", ++j);
			sensor[0] = numberbuf;
			SysDataFun(DATA_MSG_GET_IP_PARAM, 0,0,0,0,(void *)&iptemp,0);
			memset(buf,0x00,sizeof(buf));
			sprintf(buf, "%d.%d.%d.%d", iptemp.NetParam_IPAddress[0],iptemp.NetParam_IPAddress[1],iptemp.NetParam_IPAddress[2],iptemp.NetParam_IPAddress[3]);	
			sensor[2] = buf;
			sensor[1] = "IP��ַ";
			LISTVIEW_AddRow(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW0),sensor);
			//д�ߵ�����
//			memset(sensor,0x00,sizeof(sensor));
//			memset(numberbuf,0x00,sizeof(numberbuf));
//			sprintf(numberbuf, "%d", ++j);
//			sensor[0] = numberbuf;
//			sensor[1] = "�ߵ���ֱ";
//			memset(buf,0x00,sizeof(buf));
//			if(InsServer)
//				sprintf(buf, "%dmm", InsServer);	
//			else
//				sprintf(buf, "%s", "δ����");
//			sensor[2] = buf;
//			LISTVIEW_AddRow(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW0),sensor);
//			//д�������汾
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
						sensor[2] = "�豸����";
						sensor[1] = SensorShowBUF[i].name;
					}
					else if((Sensorversion.isOnline) &&(Sensorversion.isEnable == 0))
					{
						sensor[2] = "δʹ��";
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
						sensor[1] = "��Ա��λ";
					}
					else
					{
						sensor[2] = "δʹ��";
						sensor[1] = "��Ա��λ";
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
							sensor[1] = "��Ա��λBL";
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
						sensor[1] = "���ⱨ��";
					}
					else 
					{
						sensor[2] = "δʹ��";
						sensor[1] = "���ⱨ��";
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
							sensor[1] = "���ⱨ��BL";
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
						sensor[1] = "�������";
					}
					else
					{
						sensor[2] = "δʹ��";
						sensor[1] = "�������";
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
							sensor[1] = "�������BL";
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
						sensor[1] = "��ŷ�";
					}
					else
					{
						sensor[2] = "δʹ��";
						sensor[1] = "��ŷ�";
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
							sensor[1] = "��ŷ�BL";
							LISTVIEW_AddRow(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW0),sensor);
						}
					}
				}
			}
			//ghy ����HUB�汾����ʾ
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
							sensor[2] = "δʹ��";							
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
			sensor[1] = "����ģ��";
			LISTVIEW_AddRow(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW0),sensor);
			
			
			memset(numberbuf,0x00,sizeof(numberbuf));
			sprintf(numberbuf, "%d", ++j);
			sensor[0] = numberbuf;
			sprintf(buf, "V%d.%d.%d", WirelessVersion[0],WirelessVersion[1],WirelessVersion[2]);
			sensor[2] = buf;
			sensor[1] = "����ģ��";
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
			//д����״̬
			show_function_refresh(pMsg);
			LISTVIEW_SetSel(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW0), 0);

		break;	
	}	
			
			
			
//	USER_ShwoListviewData(pMsg,GetListviewData(id));

}
/*
*********************************************************************************************************
*	�� �� ��: 
*	����˵��: ���ݵ�ǰҳ����ʾ����
*	��    ��: 
*	�� �� ֵ: 
*********************************************************************************************************
*/
void State_ShowButtonForPage(WM_MESSAGE * pMsg,uint32_t id)
{
	USER_ReviseAllButtonShow(pMsg,StateButtonName[0]);
}
/*
*********************************************************************************************************
*	�� �� ��: 
*	����˵��: ���ݵ�ǰҳ����ʾ�б�
*	��    ��: 
*	�� �� ֵ: 
*********************************************************************************************************
*/
void State_ShowListviewForPage(WM_MESSAGE * pMsg,uint32_t id)
{
	USER_ShwoListviewData(pMsg,GetListviewData(id));
}
/*
*********************************************************************************************************
*	�� �� ��: 
*	����˵��: ��ʼ���˵��ı�
*	��    ��: 
*	�� �� ֵ: 
*********************************************************************************************************
*/	
void InitStateText(WM_MESSAGE * pMsg)
{
	
	WM_HWIN hItem;
	hItem = WM_GetDialogItem(pMsg->hWin, MENU_TEXT_ID0);
	TEXT_SetFont(hItem, &GUI_FontHZ24);
	TEXT_SetText(hItem,"״̬����");
	TEXT_SetTextColor(hItem,GUI_WHITE);
	TEXT_SetTextAlign(hItem,GUI_TA_HCENTER | GUI_TA_VCENTER);
}

