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
extern NameAndId   SingleActionBUF[];
extern NameAndId   GroupActionBUF[];
extern SensorShow   SensorShowBUF[];
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
	{40,""},
	{125,"������"},
	{110,"����"},
	{85,"״̬"},
};
const LISTVIEW_HEADER State_LISTVIEW_HEADER3[] =
{
	{40,""},
	{125,"ID"},
	{85,"����"},
	{110,"Ȩ��"},
};
const LISTVIEW_HEADER State_LISTVIEW_HEADER5[] =
{
	{40,""},
	{150,"ʱ��"},
	{170,"����"},
};
const LISTVIEW_HEADER State_LISTVIEW_HEADER6[] =
{
	{40,""},
	{160,"����"},
	{160,"�汾"},
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
	{"������","��־","��Ա","�汾","����"," "},
};

static const char *ListViewTable1[][4]={
	{"1",	"ǰ��ѹ��","����","15.0Mpa"},
	{"2",	"����ѹ��","����","16.0Mpa"},
	{"3",	"�����г�","����","900mm"},
	{"4",	"֧�ܸ߶�","�쳣","***mm"},
	{"5",	"�������","����","7.0��"},
	{"6",	"�������","����","3.5��"},
	{"7",	"β���г�","����","200mm"},
	{"8",	"����ѹ��","����","5.8Mpa"},
};
static const char *ListViewTable2[][4]={
	{"1",	"ǰ��ѹ��","����","15.0Mpa"},
	{"2",	"����ѹ��","����","16.0Mpa"},
	{"3",	"�����г�","����","900mm"},
	{"4",	"֧�ܸ߶�","�쳣","***mm"},
	{"5",	"�������","����","7.0��"},
	{"6",	"�������","����","3.5��"},
	{"7",	"β���г�","����","200mm"},
	{"8",	"����ѹ��","����","5.8Mpa"},
};
static const char *ListViewTable3[][4]={
	{"1",	"311","150cm","1��"},
	{"2",	"322","160cm","5��"},
	{"3",	"323","250cm","2��"},
	{"4",	"324","288cm","8��"},
	{"5",	"325","582cm","4��"},
	{"6",	"326","522cm","1��"},
	{"7",	"327","782cm","9��"},
	{"8",	"328","412cm","15��"},
};
static const char *ListViewTable5[][4]={
	{"1",	"�߶ȴ���������"},
};
static const char *ListViewTable6[][4]={
	{"1",	"����","�м��"},
	{"2",	"IP","192.168.25.123"},
	{"3",	"�汾","V4.21.11"},
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

	switch(val.host_type)
	{
		case FAULT_HOST_SENSOR:
			for(i=1;i<SensorShowBUF[0].MAX+1;i++)
			{
				if(val.host_name == SensorShowBUF[i].id)
				{
					show[2] = SensorShowBUF[i].name;
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
				case FAULT_EVENT_ALL_PARA_NOT_MATCH_NEIGHBOR:
					show[2] = "ȫ�ֲ����ڼܲ���";
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
			show[2] = "����";
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
			show[3] = "����������";
			break;
		case LOG_STATUS_SENSOR_ALARM:
			show[3] = "����������";
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
  GetCanDeviceAPPMsg(length,ID_PERSON,i,0,&person_temp);//Ȩ��
  person_temp  = (person_temp>>12)&0x0000FF;
  sprintf(value, "%d��", person_temp);
  sensor[3] = value;
  GetCanDeviceAPPMsg(length,ID_PERSON,i,2,&person_temp);//����
  sprintf(state, "%dcm", person_temp);
  sensor[2] = state; 
  LISTVIEW_AddRow(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW0),sensor);
 }
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
						sprintf(state,"%s","����");
					else 
						sprintf(state,"%s","����");
				}
				else
					sprintf(state,"%s","����");
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

		case 4://�汾
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
						sensor[2] = "�豸����";
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
						sensor[1] = "��Ա��λ";
					}
					else
					{
						sensor[2] = "�豸����";
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
						sensor[1] = "��Ա��λBL";
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
						sensor[1] = "���ⱨ��";
					}
					else
					{
						sensor[2] = "�豸����";
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
						sensor[1] = "���ⱨ��BL";
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
						sensor[1] = "�������";
					}
					else
					{
						sensor[2] = "�豸����";
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
						sensor[1] = "�������BL";
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
						sensor[1] = "��ŷ�";
					}
					else
					{
						sensor[2] = "�豸����";
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
						sensor[1] = "��ŷ�BL";
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

























