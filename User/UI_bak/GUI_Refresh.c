/*
*********************************************************************************************************
*	                                            DoubleWires
*                           Commuication Controller For The Mining Industry
*
*                                  (c) Copyright 1994-2021  HNDZ 
*                       All rights reserved.  Protected by international copyright laws.
*
*
*    File    : 
*    Module  : user
*    Version : V1.0
*    History :
*   -----------------
*              Version  Date           By            Note
*              v1.0     2021-10   
*
*********************************************************************************************************
*/	
#include "User_GUI.h"
#include "sys_msg_def.h"
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
extern WM_HTIMER 	ShearerLeft_hTimer,
			ShearerRight_hTimer,
			DtrectionLeft_hTimer,
			DtrectionRight_hTimer;
extern WM_HWIN  ShearerLeft_hWin,
		ShearerRight_hWin,
		DtrectionLeft_hWin,
		DtrectionRight_hWin;




extern WM_HWIN SuddenStop_hWin;
extern NameAndId   ShowAuxiliaryAction[AuxiliaryActionMAX];
extern SensorShow   ShowSensorShow[SensorMAX];


ACTOIN_INFO  action_info;
void Info_Action_Init()
{
	//����
	uint8_t i = 0;
	//����
	action_info.action_info_count = 0;
	memset(action_info.action_info_queue, 0 ,sizeof(action_info.action_info_queue));
	
	action_info.pUsed = NULL;
	action_info.pFree =&action_info.action_info_queue[0];
	for(i=0 ; i<ACTION_INFO_SIZE - 1; i++)
	{
		action_info.action_info_queue[i].pNext = &action_info.action_info_queue[i+1];
	}	
	action_info.action_info_queue[ACTION_INFO_SIZE-1].pNext = NULL;
}
//��ѯ��������
static stActionInfo * checkActionInfoQueue(NameAndId *actionInfo)
{
	stActionInfo* resactionInfo = NULL;
	stActionInfo* tmpactionInfo = action_info.pUsed;
	
	if(action_info.action_info_count == 0)
		return resactionInfo;
	do
	{
		if(tmpactionInfo->actionInfo.id == actionInfo->id)
		{
			resactionInfo = tmpactionInfo;
			break;
		}
		tmpactionInfo = tmpactionInfo->pNext;
	}while(tmpactionInfo != NULL);

	return resactionInfo;
}
 void InsActionInfoQueue(NameAndId *actionInfo)
{
	stActionInfo * tmpInfo;
	if(action_info.action_info_count <ACTION_INFO_SIZE)
	{
		memcpy(&(action_info.pFree->actionInfo),actionInfo,sizeof(NameAndId));
		tmpInfo = action_info.pFree;
		action_info.pFree = action_info.pFree->pNext;
		
		tmpInfo->pNext =  action_info.pUsed;
		action_info.pUsed = tmpInfo;
		action_info.action_info_count ++;
	}
}
//ɾ����������
 void DelActionInfoQueue(NameAndId *actionInfo)
{
	uint8_t i;
	stActionInfo* delactionInfo = NULL;
	stActionInfo* tmpactionInfo = action_info.pUsed;
	delactionInfo = checkActionInfoQueue(actionInfo);
	if(delactionInfo != NULL)
	{	
		if(delactionInfo == tmpactionInfo)
		{
			action_info.pUsed = action_info.pUsed->pNext;
			action_info.action_info_count--;
			tmpactionInfo->pNext = action_info.pFree;
			action_info.pFree = tmpactionInfo;
		}
		else
		{
			do
			{
				if(tmpactionInfo->pNext == delactionInfo)
				{
					tmpactionInfo->pNext = delactionInfo->pNext;
					delactionInfo->pNext = action_info.pFree;
					action_info.pFree = delactionInfo;
					action_info.action_info_count--;
					break;
				}
				tmpactionInfo = tmpactionInfo->pNext;
			}while(tmpactionInfo != NULL);
		}
	}
}
//�������������api
uint8_t ReadAction_API(NameAndId *info)
{
	uint8_t i = 0;
	uint8_t index = 0;
	uint8_t res = 0;
	stActionInfo* tmpactionInfo = action_info.pUsed;
	NameAndId tmpinfo;

	for(i = 0; i<6; i++)
	{
		if(tmpactionInfo == NULL)
			break;
		memcpy(info,&(tmpactionInfo->actionInfo),sizeof(NameAndId));
		tmpactionInfo = tmpactionInfo->pNext;
		info++;
		index++;
	}
	return index;
}




/*
*********************************************************************************************************
*	�� �� ��: 
*	����˵��: ֧��״̬ˢ��
*	��    ��: 
*	�� �� ֵ: 
*********************************************************************************************************
*/
void SupporthomeRefresh(WM_MESSAGE * pMsg)
{
	static uint8_t lastdata;
	uint8_t length,pdata;
	globalStateType  temp;
	GUIGetDateFunAPI(API_Support_Slave,0,&temp,&length);
	
	if(temp.slaveMode == 0)
	{
		lastdata++;
		if(lastdata > 2)
		{
			lastdata = 0;
			GUI_EndDialog(pMsg->hWin, 0);
			CreateHomeFrame();
		}
	}
	else
	{
		lastdata = 0;
	}
}
/*
*********************************************************************************************************
*	�� �� ��: 
*	����˵��: ֧��״̬ˢ��
*	��    ��: 
*	�� �� ֵ: 
*********************************************************************************************************
*/
void SupportStopRefresh(WM_MESSAGE * pMsg)
{
	static uint8_t lastdata;
	uint8_t length,pdata;
	globalStateType  temp;
	char buf[10];
	GUIGetDateFunAPI(API_Support_Statur,0,&temp,&length);
//	SysDataFun(DATA_MSG_GetStopInfo,0,0,0,0,&pdata,(uint16_t*)&length);
	if(temp.emergencyState == 1)
	{
		SysDataFun(DATA_MSG_GetStopInfo,0,0,0,0,&pdata,(uint16_t*)&length);
		sprintf(buf,"%d",pdata);
		TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT23),buf);	
	}
	
//	
//	
//	
//	
//	
//	if(((temp.emergencyState == 1) || (temp.bisuoState == 1) ||(temp.stopState == 1)) && lastdata ==0)
//	{
//		lastdata = 1;//�Ѿ�����
//		CreateSuddenStop();
//	}
//	else if(((temp.emergencyState == 0) && (temp.bisuoState == 0) &&(temp.stopState == 0)) && lastdata ==1)
//	{
//		lastdata = 0;//�Ѿ�����
//		GUI_EndDialog(SuddenStop_hWin, 0);
//	}
}
///*
//*********************************************************************************************************
//*	�� �� ��: 
//*	����˵��: ֧��״̬ˢ��
//*	��    ��: 
//*	�� �� ֵ: 
//*********************************************************************************************************
//*/
//void SupportSlaveRefresh(WM_MESSAGE * pMsg)
//{
//	static uint8_t lastdata;
//	uint8_t length,pdata;
//	globalStateType  temp;
//	GUIGetDateFunAPI(API_Support_Slave,0,&temp,&length);
//	
//	if(temp.slaveMode == 1)
//	{
//		GUI_EndDialog(pMsg->hWin, 0);
//		CreateSlveContor();
//	}
//}
/*
*********************************************************************************************************
*	�� �� ��: 
*	����˵��: ֧��״̬ˢ��
*	��    ��: 
*	�� �� ֵ: 
*********************************************************************************************************
*/
uint8_t InitGetstopstate(void)
{
	uint8_t stop = 0;
	uint8_t length;
	globalStateType  temp;
	GUIGetDateFunAPI(API_Support_Statur,0,&temp,&length);
	if((temp.emergencyState == 1) || (temp.bisuoState == 1) )
	{
		stop = 1;
	}
	return stop;
}
/*
*********************************************************************************************************
*	�� �� ��: 
*	����˵��: ֧��״̬ˢ��
*	��    ��: 
*	�� �� ֵ: 
*********************************************************************************************************
*/
void Getstopstate(WM_MESSAGE * pMsg)
{
	static uint8_t lastdata;
	uint8_t length;
	globalStateType  temp;
	GUIGetDateFunAPI(API_Support_Statur,0,&temp,&length);
	if((temp.emergencyState == 1) || (temp.bisuoState == 1) ||(temp.stopState == 1))
	{
		GUI_EndDialog(pMsg->hWin, 0);
		CreateHomeFrame();
	}
}
uint8_t SupportStatusdata;
extern uint8_t MoveFramedata;
/*
*********************************************************************************************************
*	�� �� ��: 
*	����˵��: ֧��״̬ˢ��
*	��    ��: 
*	�� �� ֵ: 
*********************************************************************************************************
*/
void SupportStatusRefresh(WM_MESSAGE * pMsg)
{
	uint8_t length,pdata;
	globalStateType  temp;
	GUIGetDateFunAPI(API_Support_Statur,0,&temp,&length);
	pdata = temp.runMode;
	if(pdata == 0)
	{
		USER_ReviseTextShow(pMsg,GUI_ID_TEXT3,"֧�ܿ���");
	}
	else if(pdata == 1)
	{
		USER_ReviseTextShow(pMsg,GUI_ID_TEXT3,"������");
	}
	else
		;
	if(((temp.emergencyState == 1) || (temp.bisuoState == 1) ||(temp.stopState == 1)) && SupportStatusdata ==0)
	{
		SupportStatusdata = 1;//�Ѿ�����
		CreateSuddenStop();
		if(MoveFramedata ==1)
		{
			WM_DeleteTimer(ShearerRight_hTimer);
			WM_DeleteTimer(DtrectionRight_hTimer);
		}
		else if(MoveFramedata ==2)
		{
			WM_DeleteTimer(ShearerLeft_hTimer);
			WM_DeleteTimer(DtrectionLeft_hTimer);
		}
	}
	else if(((temp.emergencyState == 0) && (temp.bisuoState == 0) &&(temp.stopState == 0)) && SupportStatusdata ==1)
	{
		SupportStatusdata = 0;//�Ѿ�����		
		GUI_EndDialog(SuddenStop_hWin, 0);
		if(MoveFramedata ==1)
		{
			ShearerRight_hTimer = WM_CreateTimer(WM_GetClientWindow(ShearerRight_hWin), 0, 100, 0);
			DtrectionRight_hTimer = WM_CreateTimer(WM_GetClientWindow(DtrectionRight_hWin), 0, 100, 0);
		}
		else if(MoveFramedata ==2)
		{
			ShearerLeft_hTimer = WM_CreateTimer(WM_GetClientWindow(ShearerLeft_hWin), 0, 100, 0);
			DtrectionLeft_hTimer = WM_CreateTimer(WM_GetClientWindow(DtrectionLeft_hWin), 0, 100, 0);
		}
	}
	if(temp.slaveMode == 1)
	{
		GUI_EndDialog(pMsg->hWin, 0);
		CreateSlveContor();
	}
}
/*
*********************************************************************************************************
*	�� �� ��: 
*	����˵��: ֧�ܱ��ˢ��
*	��    ��: 
*	�� �� ֵ: 
*********************************************************************************************************
*/
void SupportNumberRefresh(WM_MESSAGE * pMsg)
{
	static uint8_t lastdata;
	uint8_t length,pdata;
	char  buf[3];
	GUIGetDateFunAPI(API_Support_Number,0,&pdata,&length);
//	if(pdata != lastdata)
//	{
		sprintf(buf, "%d", pdata);
		TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT0),buf);
//	lastdata = pdata;		
//	}
}
/*
*********************************************************************************************************
*	�� �� ��: 
*	����˵��: ˢ�µ�ǰ����
*	��    ��: 
*	�� �� ֵ: 
*********************************************************************************************************
*/
void CurrentDataRefresh(WM_MESSAGE * pMsg)
{

}
/*
*********************************************************************************************************
*	�� �� ��: 
*	����˵��: ˢ�µ�ǰʱ��
*	��    ��: 
*	�� �� ֵ: 
*********************************************************************************************************
*/
void CurrentTimrRefresh(WM_MESSAGE * pMsg)
{

}
/*
*********************************************************************************************************
*	�� �� ��: 
*	����˵��: ˢ��ú��λ��
*	��    ��: 
*	�� �� ֵ: 
*********************************************************************************************************
*/
void MachinePositionRefresh(WM_MESSAGE * pMsg)
{
	static uint8_t lastdata;
	uint8_t length,pdata;
	char  buf[3];
	GUIGetDateFunAPI(API_Machine_Position,0,&pdata,&length);
	if(pdata != lastdata)
	{
		sprintf(buf, "%d", pdata);
		TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT10),buf);	
		lastdata = pdata;
	}
}
/*
*********************************************************************************************************
*	�� �� ��: 
*	����˵��: ˢ�����з���
*	��    ��: 
*	�� �� ֵ: 
*********************************************************************************************************
*/
void RunningDirectionRefresh(WM_MESSAGE * pMsg)
{
	static uint8_t lastdata;
	uint8_t length,pdata;
	GUIGetDateFunAPI(API_Running_Direction,0,&pdata,&length);
	if(pdata != lastdata)
	{
		if(pdata == 0)
		{
			CreateMoveFrame_Left();
		}
		else if(pdata == 1)
		{
			CreateMoveFrame_Right();
		}
		else
			;	
		lastdata = pdata;
	}
}
/*
*********************************************************************************************************
*	�� �� ��: 
*	����˵��: ˢ����ʾ��1
*	��    ��: 
*	�� �� ֵ: 
*********************************************************************************************************
*/
void DisplayItemRefresh(WM_MESSAGE * pMsg)
{
	static uint8_t lastdata;
	uint8_t length,i;
	static  char buf[15];
	char buf1[15];
	char show[13];
	SensorStateType SensorState;
	for(i=0;i<6;i++)
	{
		if(ShowSensorShow[i].id == 0)
			continue;
		memcpy(buf,ShowSensorShow[i].nameshow,GUI__strlen(ShowSensorShow[i].nameshow));
		strcat(buf," ");
		GUIGetDateFunAPI(API_Display_Item,ShowSensorShow[i].id,&SensorState,&length);
		if(ShowSensorShow[i].acc == 3)
			sprintf(buf1,"%d.%d",SensorState.calculateValue/10,SensorState.calculateValue%10);
		else if(ShowSensorShow[i].acc == 4)
			sprintf(buf1,"%d.%02d",SensorState.calculateValue/100,SensorState.calculateValue%100);
		else
			sprintf(buf1,"%d",SensorState.calculateValue);
		strcat(buf,buf1);
		AddUnitDispiay(buf,ShowSensorShow[i].Nuit);
		TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT4+i),buf);	
		memset(buf,0,sizeof(buf));
	}
}
/*
*********************************************************************************************************
*	�� �� ��: 
*	����˵��: ˢ�³������
*	��    ��: 
*	�� �� ֵ: 
*********************************************************************************************************
*/
void GroupRefresh(WM_MESSAGE * pMsg,NameAndId * pGroupAction,uint16_t Direction,GroupStartUp * pdata)
{
	char  showbuf[15];
	uint8_t length;
	uint8_t number,direction;
	uint8_t SrartNumber,EndNumber;
	uint8_t SafeInte,ScopExec;
	groupActionParamType  grouptemp;
	GUIGetDateFunAPI(API_Support_Number,0,&number,&length);	//���ܱ��
	GUIGetDateFunAPI(API_Number_Increase,0,&direction,&length);	//�ܺ�����
	
	GUIGetDateFunAPI(API_Group_Action,pGroupAction->id,&grouptemp,&length);
	SafeInte = grouptemp.actionParam.safeGap;
	ScopExec = grouptemp.actionParam.scope;
	//�ܺ���������鷽��һ�£���ʼ��=���ܼܺ�+��ȫ�����������=���ܼܺ�+��ȫ���+ִ�з�Χ
	if(Direction == direction)
	{
		SrartNumber = number+SafeInte+1;
		EndNumber = number+SafeInte+ScopExec;
	}	
	//�ܺ�������������෴����ʼ��=���ܼܺ�-��ȫ�����������=���ܼܺ�-��ȫ���-ִ�з�Χ
	else
	{
		SrartNumber = number-SafeInte-1;
		EndNumber = number-SafeInte-ScopExec;
	}
	pdata->GroupID = pGroupAction->id;
	pdata->ActionType = Group;
	pdata->number = number;
	pdata->SrartNumber = SrartNumber;
	pdata->EndNumber = EndNumber;
	memset(showbuf, 0x00  , sizeof(showbuf));
	sprintf(showbuf, "����%s", pGroupAction->name);
	TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT32),showbuf);
	memset(showbuf, 0x00  , sizeof(showbuf));
	if(Direction == 1)
	{
		sprintf(showbuf, "%d", SrartNumber);
		TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT33),showbuf);
		memset(showbuf, 0x00  , sizeof(showbuf));
		sprintf(showbuf, "%d", EndNumber);
		TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT34),showbuf);
	}
	if(Direction == 0)
	{
		sprintf(showbuf, "%d", SrartNumber);
		TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT34),showbuf);
		memset(showbuf, 0x00  , sizeof(showbuf));
		sprintf(showbuf, "%d", EndNumber);
		TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT33),showbuf);
	}
}
/*
*********************************************************************************************************
*	�� �� ��: 
*	����˵��: ˢ�³�����������
*	��    ��: 
*	�� �� ֵ: 
*********************************************************************************************************
*/
void GroupAddRefresh(WM_MESSAGE * pMsg,GroupStartUp * pdata)
{
	char  showbuf[15];
	uint16_t length;
	uint16_t number,direction;
	uint16_t SrartNumber,EndNumber;
	GUIGetDateFunAPI(API_Support_Number,0,&number,&length);	//���ܱ��
	GUIGetDateFunAPI(API_Number_Increase,0,&direction,&length);	//�ܺ�����
	
	
	
	if(pdata->Direction == direction)
	{
		pdata->EndNumber++;
	}	
	
	else
	{
		pdata->EndNumber--;
	}
	memset(showbuf, 0x00  , sizeof(showbuf));
	sprintf(showbuf, "%d", EndNumber);
	TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT34),showbuf);
}



