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
#include "RackContorTask.h"
#include "cJSON.h"
#include "sys_msg_def.h"
#include "SysFunc_API.h"
//#include "RackContorTask.h"
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

extern WM_HWIN HomeFrame_hWin;
NameAndId   ShowAuxiliaryAction[30] ;
NameAndId   ShowGroupAction[30] ;
NameAndId	ShowKeyboardAction[KeyboardActionMAX][2];
SensorShow   ShowSensorShow[SensorMAX];


NameAndId   SingleActionBUF[SingleActionMAX];
NameAndId   GroupActionBUF[GroupActionMAX];
SensorShow   SensorShowBUF[SensorShowMAX];

uint8_t  SinglePointnumber[50];
uint8_t  SensorFloatPointnumber[20];
uint8_t  SensorSelPointnumber[20];

SigActionParamType  ActionParamType;
CombActionParamType  BaseParamType;
/* The cJSON structure: */ 
/*
*********************************************************************************************************
*	�� �� ��: 
*	����˵��: GroupRack�Ĳ�������
*	��    ��: 
*	�� �� ֵ: 
*********************************************************************************************************
*/
void ActionControlOfGroupRack(GroupStartUp temp)
{
	uint8_t buf[4] = {0};
	buf[0] = temp.SrartNumber;
	buf[1] = temp.EndNumber;
	buf[2] = temp.Direction;
	buf[3] = temp.Groupdirection;
	SysNetCtrlFun(0,temp.ActionType,temp.GroupID,2,1,4,buf);
}
/*
*********************************************************************************************************
*	�� �� ��: 
*	����˵��: AdjacentRack�Ĳ�������
*	��    ��: 
*	�� �� ֵ: 
*********************************************************************************************************
*/
void ActionControlOfAdjacentRack(SingleStartUp temp,uint8_t cmd)
{
	uint8_t i = 0;
	if(temp.Action.id == 0)
	{
		return;
	}
	if(temp.Action.type != 0)
	{
		i = 1;
		if(cmd == 1)
			return;
		else
		  cmd = ActionCtrl_CmdType_WAITSTARTUP;	
	}
	SysNetCtrlFun(temp.targer,temp.Action.type,temp.Action.id,cmd,i,0,0);
}
/*
*********************************************************************************************************
*	�� �� ��: 
*	����˵��: ��ѯ�෴����
*	��    ��: 
*	�� �� ֵ: 
*********************************************************************************************************
*/
void QueryOppositeAction(SingleStartUp temp,uint8_t cmd)
{
	stActionInfo* delactionInfo = NULL;
	stActionInfo* delactionInfo1 = NULL;
	SingleStartUp Querytemp;
	Querytemp = temp;
	GUIGetDateFunAPI(API_Singel_Action,temp.Action.id,&ActionParamType,0);
	Querytemp.Action.id = ActionParamType.actionParam.mutexActionID1;
	delactionInfo = checkActionInfoQueue(&Querytemp.Action);
	Querytemp.Action.id = ActionParamType.actionParam.mutexActionID2;
	delactionInfo1 = checkActionInfoQueue(&Querytemp.Action);
	if((delactionInfo == NULL) && (delactionInfo1 == NULL))
	{
		ActionControlOfAdjacentRack(temp,cmd);
		InsActionInfoQueue(&temp.Action);
	}
	else	
	{
		;
	}
}




/*
*********************************************************************************************************
*	�� �� ��: 
*	����˵��: ThisHome�Ĳ�������
*	��    ��: 
*	�� �� ֵ: 
*********************************************************************************************************
*/
void ActionControlOfThisHome(SingleStartUp temp,uint8_t  arr)
{	
	uint8_t buf[4];//buf��0�����£�̧��buf��1���ֶ����Զ�
	memset(buf,0,4);
	//������ֶ������Ͳ�����ʱ�������÷�ֹͣ��������Զ������Ͳ��÷�ֹͣ
	if(temp.Action.type != 0)
	{
		if(arr == 1)//̧�𰴼����Զ��������ܷ���ֹͣ
			return;
	}
	buf[0] = arr;
	if(temp.Action.type == 0)//������ֶ�����
	{
		GUIGetDateFunAPI(API_Singel_Action,temp.Action.id,&ActionParamType,0);
		if(((ActionParamType.actionParam.controlMode != 0) && (ActionParamType.actionParam.controlMode != 1))
			||(ActionParamType.baseParam.enable == 0))//
		{
			return;
		}
		buf[1] = ActionParamType.actionParam.localControlMode;
		if(buf[1] == 1)
		{
			buf[0] = ActionCtrl_CmdType_WAITSTARTUP;
		}
	}
	else if(temp.Action.type == 1)
	{
		GUIGetDateFunAPI(API_Comblnation_Action,temp.Action.id,&BaseParamType,0);
		if(((BaseParamType.baseParam.controlMode != 0) && (BaseParamType.baseParam.controlMode != 1))
			||(BaseParamType.baseParam.enable == 0))
		{
			return;
		}
		GUI_EndDialog(HomeFrame_hWin, 0);
		ChangeInfoRack(CreateRackContor(),temp.Action.id);
		return;
		buf[0] = ActionCtrl_CmdType_WAITSTARTUP;
		buf[1] = 1;
	}
	SysDataFun(DATA_MSG_CTRL,temp.Action.type,temp.Action.id,4,0,buf,0);
}

/*
*********************************************************************************************************
*	�� �� ��: 
*	����˵��: ����
*	��    ��: 
*	�� �� ֵ: 
*********************************************************************************************************
*/
void ActionControlOfThisSlve(SingleStartUp temp,uint8_t  arr)
{	
	uint8_t buf[4];//buf��0�����£�̧��buf��1���ֶ����Զ�
	memset(buf,0,4);
	if(temp.Action.type != 0)
	{
		if(arr == 1)//̧�𰴼����Զ��������ܷ���ֹͣ
			return;
	}
	buf[0] = arr;
	if(temp.Action.type == 0)//������ֶ�����
	{
		GUIGetDateFunAPI(API_Singel_Action,temp.Action.id,&ActionParamType,0);
		if((ActionParamType.actionParam.controlMode != 0) && (ActionParamType.actionParam.controlMode != 1))//
		{
			return;
		}
	}
	else if(temp.Action.type == 1)
	{
		return;//������ʱ����ִ���Զ�����
		GUIGetDateFunAPI(API_Comblnation_Action,temp.Action.id,&BaseParamType,0);
		if((BaseParamType.baseParam.controlMode != 0) && (BaseParamType.baseParam.controlMode != 1))
		{
			return;
		}
		buf[0] = ActionCtrl_CmdType_WAITSTARTUP;
		buf[1] = 1;
	}
	SysDataFun(DATA_MSG_CTRL,temp.Action.type,temp.Action.id,4,0,buf,0);
}
/*
*********************************************************************************************************
*	�� �� ��: 
*	����˵��: ���ܿ���
*	��    ��: 
*	�� �� ֵ: 
*********************************************************************************************************
*/
void ActionControlOfThisRack(SingleStartUp temp,uint8_t  arr)
{	
	uint8_t buf[4];//buf��0�����£�̧��buf��1���ֶ����Զ�
	memset(buf,0,4);
	//������ֶ������Ͳ�����ʱ�������÷�ֹͣ��������Զ������Ͳ��÷�ֹͣ
	if(arr == 1)//̧�𰴼����Զ��������ܷ���ֹͣ
		return;
	buf[0] = arr;
	if(temp.Action.type == 0)//������ֶ�����
	{
		GUIGetDateFunAPI(API_Singel_Action,temp.Action.id,&ActionParamType,0);
		if((ActionParamType.actionParam.controlMode != 0) && (ActionParamType.actionParam.controlMode != 1))//
		{
			return;
		}
		buf[1] = ActionParamType.actionParam.localControlMode;
		if(buf[1] == 1)
		{
			buf[0] = ActionCtrl_CmdType_WAITSTARTUP;
		}
	}
	else if(temp.Action.type == 1)
	{
		GUIGetDateFunAPI(API_Comblnation_Action,temp.Action.id,&BaseParamType,0);
		if((BaseParamType.baseParam.controlMode != 0) && (BaseParamType.baseParam.controlMode != 1))
		{
			return;
		}
		buf[0] = ActionCtrl_CmdType_WAITSTARTUP;
		buf[1] = 1;
	}
	SysDataFun(DATA_MSG_CTRL,temp.Action.type,temp.Action.id,4,0,buf,0);
}
/*
*********************************************************************************************************
*	�� �� ��: 
*	����˵��: ��ӵ�λ
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void AddUnitDispiay(char* buf,uint16_t val)
{
	switch(val)
	{
		case 0x1:
			strcat(buf,"s");
		break;
		case 0x2:
			strcat(buf,"Mpa");
		break;
		case 0x3:
			strcat(buf,"mm");
		break;
		case 0x4:
			strcat(buf,"cm");
		break;
		case 0x5:
			strcat(buf,"db");
		break;
		case 0x6:
			strcat(buf,"��");
		break;
		case 0x7:
			strcat(buf,"V");
		break;
		case 0x8:
			strcat(buf,"%");
		break;
		case 0x9:
			strcat(buf,"m");
		break;
		default:
			break;
	}
}
void AddUnitDispiaySETLIST(char* buf,SETLIST*  arr)
{
	AddUnitDispiay(buf,arr->Parameter.Unit_Name);
}
void AddUnitDispiayINITLIST(char* buf,INITLIST*  arr)
{
	AddUnitDispiay(buf,arr->Parameter->Unit_Name);
}
/*
*********************************************************************************************************
*	�� �� ��: 
*	����˵��: ��ȡ��ʾ����
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void SetMenuListtempMaxValue(SETLIST*  arr)
{
	uint16_t i;
	switch(arr->Parameter.pSelItem)
	{
		case 0xF0:// ��������
			arr->Parameter.Max_Value = SingleActionBUF[0].MAX;//������+��϶��������ֵ
			for(i=0;i<SingleActionBUF[0].MAX+1;i++)
			{
				if(((SingleActionBUF[i].type<<8) | (SingleActionBUF[i].id)) == arr->Parameter.Para_Value)
				{
					arr->Parameter.Para_Value = i;
					break;
				}
			}
		break;
		case 0xF1:// ���̶���
			arr->Parameter.Max_Value = SingleActionBUF[1].MAX;//.id��ʱ���ڴ洢�����������ֵ
			for(i=0;i<SingleActionBUF[1].MAX+1;i++)
			{
				if(SingleActionBUF[i].id == arr->Parameter.Para_Value)
				{
					arr->Parameter.Para_Value = i;
					break;
				}
			}
		break;
		case 0xF2:// ��������ʾ
			arr->Parameter.Max_Value = SensorShowBUF[0].MAX;
			for(i=0;i<SensorShowBUF[0].id;i++)
			{
				if(SensorShowBUF[i].id == arr->Parameter.Para_Value)
				{
					arr->Parameter.Para_Value = i;
					break;
				}
			}
		break;
		case 0xF3:// ��������ʾ
			arr->Parameter.Max_Value = GroupActionBUF[0].MAX;
			for(i=0;i<GroupActionBUF[0].id;i++)
			{
				if(GroupActionBUF[i].id == arr->Parameter.Para_Value)
				{
					arr->Parameter.Para_Value = i;
					break;
				}
			}
		break;			
		default:
			break;
	}
}
/*
*********************************************************************************************************
*	�� �� ��: 
*	����˵��: ��ȡ��ʾ����
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void SetMenuListtempValue(SETLIST*  arr)
{
	switch(arr->Parameter.pSelItem)
	{
		case 0xF0:// ��������
			arr->Parameter.Para_Value = (SingleActionBUF[arr->Parameter.Para_Value].type<<8) |(SingleActionBUF[arr->Parameter.Para_Value].id);
		break;
		case 0xF1:// ���̶���
			arr->Parameter.Para_Value = SingleActionBUF[arr->Parameter.Para_Value].id;
		break;
		case 0xF2:// ��������ʾ
			arr->Parameter.Para_Value = SensorShowBUF[arr->Parameter.Para_Value].id;
		break;	
		case 0xF3:// ��������ʾ
			arr->Parameter.Para_Value = GroupActionBUF[arr->Parameter.Para_Value].id;
		break;
		default:
			break;
	}
}
/*
*********************************************************************************************************
*	�� �� ��: 
*	����˵��: ��ȡ��ʾ����
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void AuxiliaryActionShowEnter(char** arr ,uint16_t pSelItem,uint16_t Value)
{
	switch(pSelItem)
	{
		case 0xF0:
			*arr = SingleActionBUF[Value].name;
		break;
		case 0xF1:
			*arr = SingleActionBUF[Value].name;
		break;
		case 0xF2:
			*arr = SensorShowBUF[Value].name;
		break;
		case 0xF3:
			*arr = GroupActionBUF[Value].name;
		break;
	}
}
/*
*********************************************************************************************************
*	�� �� ��: 
*	����˵��: ��ȡ��ʾ����
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void AuxiliaryActionShow(char** arr ,uint16_t pSelItem,uint16_t Value)
{
	uint16_t i;
	switch(pSelItem)
	{
		case 0xF0:
			for(i=0;i<SingleActionBUF[0].MAX+1;i++)
			{
				if(((SingleActionBUF[i].type<<8) | (SingleActionBUF[i].id)) == Value)	
				{
					*arr = SingleActionBUF[i].name;
					break;
				}
			}
		break;
		case 0xF1:
			for(i=0;i<SingleActionBUF[1].MAX+1;i++)
			{
				if(((SingleActionBUF[i].type<<8) | (SingleActionBUF[i].id)) == Value)	
				{
					*arr = SingleActionBUF[i].name;
					break;
				}
			}
		break;
		case 0xF2:
			for(i=0;i<SensorShowBUF[0].MAX+1;i++)
			{
				if((SensorShowBUF[i].id) == Value)	
				{
					*arr = SensorShowBUF[i].name;
					break;
				}
			}
		break;
		case 0xF3:
			for(i=0;i<GroupActionBUF[0].MAX+1;i++)
			{
				if((GroupActionBUF[i].id) == Value)	
				{
					*arr = GroupActionBUF[i].name;
					break;
				}
			}
		break;
	}
}
/*
*********************************************************************************************************
*	�� �� ��: 
*	����˵��: ��ʼ����������ָ������
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void InitSingleAction(void)
{
	uint8_t  *temp;
	uint64_t id = 0x200000000000000;
	uint16_t count,count1,i = 1;
	//��ȡ������1����ʼ��ַ id 3.1.0
	memset(SinglePointnumber,0,sizeof(SinglePointnumber));
	count = getJsonParaStructSize(id);
	
	count = count >= SingleActionMAX ? SingleActionMAX-2:count;
	
	for(i= 1;i<count+1;i++)
	{
		temp = getJsonParaAddress(((id >> 48 )+i)<<48);	
		SingleActionBUF[i].name = (char *)temp;		
		memcpy(&SingleActionBUF[i].id,temp+48,2);		
		SingleActionBUF[i].type = Sing;
		if((uint8_t)( *(temp+176)) == EDIT_INT_DANDONGZUO)
		{
			SinglePointnumber[i] = 1;
		}
	}
	SingleActionBUF[1].MAX = i-1;
	
//	if(count == (SingleActionMAX-2))
//	{
//		return;
//	}
		
	id = 0x400000000000000;
	count1 = getJsonParaStructSize(id);
	
	count1 = (count1+count) >= SingleActionMAX ? 0:count1;
	
	for(;i<count1+count+1;i++)
	{
		temp = getJsonParaAddress(((id >> 48 )+(i-count))<<48);	
		SingleActionBUF[i].name = (char *)temp;		
		memcpy(&SingleActionBUF[i].id,temp+48,2);	
//		SingleActionBUF[i].id += 256;
		SingleActionBUF[i].type = Combin;
	}

	SingleActionBUF[0].MAX = i-1;
	SetSingleMAXValue(i);
}
/*
*********************************************************************************************************
*	�� �� ��: 
*	����˵��: ��ʼ����������ָ������
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void InitGroupAction(void)
{
	uint8_t  *temp;
	uint64_t id = 0x600000000000000;
	uint16_t count,i = 1;
	//��ȡ������1����ʼ��ַ id 3.1.0
	count = getJsonParaStructSize(id);
	
	count = count >= GroupActionMAX ? GroupActionMAX-2:count;
	
	for(i= 1;i<count+1;i++)
	{
		temp = getJsonParaAddress(((id >> 48 )+i)<<48);	
		GroupActionBUF[i].name = (char *)temp;		
		memcpy(&GroupActionBUF[i].id,temp+48,2);		
		GroupActionBUF[i].type = Groupa;
	}
	GroupActionBUF[0].MAX = i-1;
	SetGroupMAXValue(i-1);
}

/*
*********************************************************************************************************
*	�� �� ��: 
*	����˵��: ��ʼ����������ָ������
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void InitSensorShow(void)
{
	uint8_t  *temp;
	STR_V4PARA_NAME *temp1;
	uint64_t id = 0x300000000000000;
	uint16_t count,i = 1;
	//��ȡ������1����ʼ��ַ id 3.1.0
	memset(SensorFloatPointnumber,0,sizeof(SensorFloatPointnumber));
	memset(SensorSelPointnumber,0,sizeof(SensorSelPointnumber));
	count = getJsonParaStructSize(id);
	
	count = count >= SensorShowMAX ? SensorShowMAX-2:count;
	
	for(i= 1;i<count+1;i++)
	{
		temp = getJsonParaAddress(((id >> 48 )+i)<<48);	
		SensorShowBUF[i].nameshow = (char*)temp;		
		memcpy(&SensorShowBUF[i].id,temp+48,2);
		memcpy(&SensorShowBUF[i].Nuit,temp+24,2);
		memcpy(&SensorShowBUF[i].acc,temp+20,1);
		if((uint8_t)( *(temp+176)) == EDIT_FLOAT_CHUANGANQI)
		{
			if(i >= 19)
			{
				break;
			}
			SensorFloatPointnumber[i] = 1;
		}
		if((uint8_t)( *(temp+176-26)) == EDIT_SEL_CHUANGANQI)
		{
			if(i >= 19)
			{
				break;
			}
			SensorSelPointnumber[i] = 1;
		}
	}
	SensorShowBUF[0].MAX = i-1;
	count = getJsonParaStruct(id,&temp1);
	if(count != 0)
	{	
		for(i=1;i<count+1;i++)
		{
			memcpy(SensorShowBUF[i].name,(char *)temp1,12);
			temp1 = temp1+1;
		}
	}
}


/*
*********************************************************************************************************
*	�� �� ��: 
*	����˵��: ��ȡ���������Ž�һ������
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void GetGroupActionIntoArray(void)
{
	uint16_t i,j;
	uint16_t length,buf[30] = {0};
	GUIGetDateFunAPI(API_GroupAuxiliary_Action,0,buf,&length);	
	memset(ShowGroupAction,0,sizeof(ShowGroupAction));
	for(i= 0; i<30;i++)
	{
		for(j=0;j<=GroupActionMAX;j++)
		{
			if((GroupActionBUF[j].id) == buf[i])
			{
				ShowGroupAction[i] = GroupActionBUF[j];
				break;
			}
		}
	}
	for(i= 0; i<30;i++)
	{
		if(ShowGroupAction[(uint8_t)(29-i)].id != 0 )
		{
			break;
		}
	}
	ShowGroupAction[0].MAX = 30-i;
}
/*
*********************************************************************************************************
*	�� �� ��: 
*	����˵��: ��ȡ���������Ž�һ������
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void GetAuxiliaryActionIntoArray(void)
{
	uint16_t i,j;
	uint16_t length,buf[30] = {0};
	GUIGetDateFunAPI(API_Auxiliary_Action,0,buf,&length);	
	memset(ShowAuxiliaryAction,0,sizeof(ShowAuxiliaryAction));
	for(i= 0; i<30;i++)
	{
		for(j=0;j<=SingleActionMAX;j++)
		{
			if(((SingleActionBUF[j].type <<8) | (SingleActionBUF[j].id)) == buf[i])
			{
				ShowAuxiliaryAction[i] = SingleActionBUF[j];
				break;
			}
		}
	}
	for(i= 0; i<30;i++)
	{
		if(ShowAuxiliaryAction[(uint8_t)(29-i)].id != 0 )
		{
			break;
		}
	}
	ShowAuxiliaryAction[0].MAX = 30-i;
}
	
/*
*********************************************************************************************************
*	�� �� ��: 
*	����˵��: ��ȡ���̶����Ž�һ������
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void GetKeyboardActionIntoArray(void)
{
	uint16_t i,j,k;
	uint16_t length,buf[12];
	GUIGetDateFunAPI(API_Keyboard_Action,0,buf,&length);
	k = 0;
	for(i= 0; i<12;i+=2)
	{
		for(j=0;j<=SingleActionMAX;j++)
		{
			if(((SingleActionBUF[j].type <<8) | (SingleActionBUF[j].id)) == buf[i])
			{			
				ShowKeyboardAction[k][0] = SingleActionBUF[j];
				k++;
				break;
			}
		}
	}
	k = 0;
	for(i= 1; i<12+1;i+=2)
	{
		for(j=0;j<=SingleActionMAX;j++)
		{
			if(((SingleActionBUF[j].type <<8) | (SingleActionBUF[j].id)) == buf[i])
			{			
				ShowKeyboardAction[k][1] = SingleActionBUF[j];
				k++;
				break;
			}
		}
	}
}
/*
*********************************************************************************************************
*	�� �� ��: 
*	����˵��: ��ȡ��������ʾ�Ž�һ������
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void GetSensorShowIntoArray(void)
{
	uint16_t i,j;
	uint16_t length,buf[6];
	GUIGetDateFunAPI(API_Sensor_Show,0,buf,&length);
	for(i= 0; i<6;i++)
	{
		for(j=0;j<=SensorShowBUF[0].MAX;j++)
		{
			if(SensorShowBUF[j].id == buf[i])
			{			
				ShowSensorShow[i] = SensorShowBUF[j];
				break;
			}
		}
	}
	ShowSensorShow[0].MAX = i;
}
/*
*********************************************************************************************************
*	�� �� ��: 
*	����˵��: GUIˢ�����ݻ�ȡ����
*	��    ��: 
*	�� �� ֵ: 
*********************************************************************************************************
*/
extern uint8_t Dir;
uint16_t GUIGetDateFunAPI(uint16_t Type,uint16_t Id,void* pdata,void *length)
{
	uint8_t* pstate=NULL;
	uint16_t len=0;
	globalParam_SupportType prar;
	switch(Type)
	{
		case API_Support_Statur:
			SysDataFun(DATA_MSG_READ_OBJECT,ENUM_SYSTEM,0,0,0,&pstate,&len);
		    memcpy((uint8_t*)pdata,pstate,len);
		break;
		case API_Support_Slave:
			SysDataFun(DATA_MSG_READ_OBJECT,ENUM_SYSTEM,0,0,0,&pstate,&len);
		    memcpy((uint8_t*)pdata,pstate,len);
		break;
		case API_Support_Number://��ȡ���
			SysDataFun(DATA_MSG_READ_OBJECT_PARAM,ENUM_SYSTEM,globalParamEnum_Support,0,0,&pstate,&len);
			memcpy((uint8_t*)&prar,pstate,len);
			*((uint8_t *)pdata) = prar.currentSupportID;
		break;
		case API_Current_Data:

		break;
		case API_Current_Timr:
			SysDataFun(DATA_MSG_READ_OBJECT_PARAM,ENUM_SYSTEM,globalParamEnum_Support,0,0,&pstate,&len);
			memcpy((uint8_t*)&prar,pstate,len);
			*((uint8_t *)pdata) = prar.MasterCtrlTime;

		break;
		case API_Machine_Position:

		break;
		case API_Running_Direction:
			SysDataFun(DATA_MSG_READ_OBJECT,ENUM_SYSTEM,0,0,0,&pstate,&len);
		    memcpy((uint8_t*)pdata,pstate,len);

		break;
	    case API_Signal_Intensity:
			*((uint8_t *)pdata) = 3;
		break;
		case API_Display_Item:
			SysDataFun(DATA_MSG_READ_OBJECT,ENUM_SENSOR,Id,0,0,&pstate,&len);
		    memcpy((uint8_t*)pdata,pstate,len);
		break;
		case API_Number_Increase://
			SysDataFun(DATA_MSG_READ_OBJECT_PARAM,ENUM_SYSTEM,globalParamEnum_Support,0,0,&pstate,&len);
			memcpy((uint8_t*)&prar,pstate,len);
			*((uint8_t *)pdata) = prar.SupportDirect;
		break;	
		case API_Number_MAX://
			SysDataFun(DATA_MSG_READ_OBJECT_PARAM,ENUM_SYSTEM,globalParamEnum_Support,0,0,&pstate,&len);
			memcpy((uint8_t*)&prar,pstate,len);
			*((uint8_t *)pdata) = prar.MaxSupportID;
		break;	
		case API_Number_MIN://
			SysDataFun(DATA_MSG_READ_OBJECT_PARAM,ENUM_SYSTEM,globalParamEnum_Support,0,0,&pstate,&len);
			memcpy((uint8_t*)&prar,pstate,len);
			*((uint8_t *)pdata) = prar.MinSupportID;
		break;	
		case API_Control_Range://
			SysDataFun(DATA_MSG_READ_OBJECT_PARAM,ENUM_SYSTEM,globalParamEnum_Support,0,0,&pstate,&len);
			memcpy((uint8_t*)&prar,pstate,len);
			*((uint8_t *)pdata) = prar.GapSupportNum;
		break;
		case API_Auxiliary_Action:
			SysDataFun(DATA_MSG_READ_OBJECT_PARAM,ENUM_SYSTEM,globalParamEnum_AssistKey,0,0,&pstate,&len);
			memcpy((uint8_t*)pdata,pstate,len);
		break;	
		case API_Keyboard_Action:
			SysDataFun(DATA_MSG_READ_OBJECT_PARAM,ENUM_SYSTEM,globalParamEnum_Key,0,0,&pstate,&len);
			memcpy((uint8_t*)pdata,pstate,len);
		break;
		case API_GroupAuxiliary_Action:
			SysDataFun(DATA_MSG_READ_OBJECT_PARAM,ENUM_SYSTEM,globalParamEnum_GroupAssistKey,0,0,&pstate,&len);
			memcpy((uint8_t*)pdata,pstate,len);
		break;
		case API_Sensor_Show:
			SysDataFun(DATA_MSG_READ_OBJECT_PARAM,ENUM_SYSTEM,globalParamEnum_UIConfig,0,0,&pstate,&len);
			memcpy((uint8_t*)pdata,pstate,len);
		break;	
		case API_Group_Action:
			SysDataFun(DATA_MSG_READ_OBJECT_PARAM,ENUM_GROUP_ACTION,Id,0,0,&pstate,&len);
			memcpy((uint8_t*)pdata,pstate,len);
		break;	
		case API_Sensor_Version:
			SysDataFun(DATA_MSG_READ_OBJECT_PARAM,ENUM_SENSOR,Id,0,0,&pstate,&len);
		    memcpy((uint8_t*)pdata,pstate,len);
		break;
		
		case API_Singel_Action:
			SysDataFun(DATA_MSG_READ_OBJECT_PARAM,ENUM_SINGLE_ACTION,Id,0,0,&pstate,&len);
		    memcpy((uint8_t*)pdata,pstate,len);
		break;
		case API_Comblnation_Action:
			SysDataFun(DATA_MSG_READ_OBJECT_PARAM,ENUM_COMBINATION_ACTION,Id,0,0,&pstate,&len);
		    memcpy((uint8_t*)pdata,pstate,len);
		break;
		case API_Transport_Plane://
			SysDataFun(DATA_MSG_READ_OBJECT_PARAM,ENUM_SYSTEM,globalParamEnum_DeviceTrans,0,0,&pstate,&len);
			memcpy((uint8_t*)pdata,pstate,len);
		break;	
		default:
		break;
		
	}
	
	return 0;
}
/*
*********************************************************************************************************
*	�� �� ��: 
*	����˵��: �߼�����ú�������
*	��    ��: 
*	�� �� ֵ: 
*********************************************************************************************************
*/
void GUIInitShow(void)
{
	InitSingleAction();
	InitSensorShow();
	InitGroupAction();
	GetAuxiliaryActionIntoArray();
	GetKeyboardActionIntoArray();
	GetSensorShowIntoArray();
	GetGroupActionIntoArray();
}
/*
*********************************************************************************************************
*	�� �� ��: 
*	����˵��: ��ȡ��ʾ
*	��    ��: 0:ȫ����1������������2�����̶�����3��������
*	�� �� ֵ: 
*********************************************************************************************************
*/
uint8_t AuxiliarySige = 0;

uint8_t GetAuxiliarySige(void)
{
	return AuxiliarySige;
}
void SetAuxiliarySigereturn(void)
{
	AuxiliarySige = 1;
}
void ResetAuxiliarySigereturn(void)
{
	AuxiliarySige = 0;
}
void GetShowAgain(uint8_t val)
{
	switch(val)
	{
		case 0:
			GetAuxiliaryActionIntoArray();
			GetKeyboardActionIntoArray();
			GetSensorShowIntoArray();
			GetGroupActionIntoArray();
			SetAuxiliarySigereturn();
			break;
		case 1:
			GetAuxiliaryActionIntoArray();
			SetAuxiliarySigereturn();
			break;
		case 2:
			GetKeyboardActionIntoArray();
			break;
		case 3:
			GetSensorShowIntoArray();
			break;
		case 4:
			GetGroupActionIntoArray();
			break;
		default:
			break;
	}
}
