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
#include "cJSON.h"
#include "sys_msg_def.h"
#include "SysFunc_API.h"
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

NameAndId   ShowAuxiliaryAction[30] ;
NameAndId	ShowKeyboardAction[KeyboardActionMAX][2];
SensorShow   ShowSensorShow[SensorMAX];


NameAndId   SingleActionBUF[SingleActionMAX];
NameAndId   GroupActionBUF[GroupActionMAX];
SensorShow   SensorShowBUF[SensorShowMAX];


/* The cJSON structure: */ 
/*
*********************************************************************************************************
*	函 数 名: 
*	功能说明: GUI刷新cJSON获取函数
*	形    参: 
*	返 回 值: 
*********************************************************************************************************
*/
void ActionControlOfGroupRack(GroupStartUp temp)
{
	uint8_t buf[3] = {0};
	buf[0] = temp.SrartNumber;
	buf[1] = temp.EndNumber;
	buf[2] = temp.Direction;
	SysNetCtrlFun(0,temp.ActionType,temp.GroupID,0,1,3,buf);
//	SysDataFun(DATA_MSG_CTRL,temp.ActionType,temp.GroupID,3,0,buf,0);
}
/*
*********************************************************************************************************
*	函 数 名: 
*	功能说明: GUI刷新cJSON获取函数
*	形    参: 
*	返 回 值: 
*********************************************************************************************************
*/
void ActionControlOfAdjacentRack(SingleStartUp temp,uint8_t cmd)
{
	uint8_t i = 0;
	if(temp.Action.type != 0)
	{
		i = 1;
		if(cmd == 1)
			return;
	}
	SysNetCtrlFun(temp.targer,temp.Action.type,temp.Action.id,cmd,i,0,0);
}

/*
*********************************************************************************************************
*	函 数 名: 
*	功能说明: GUI刷新cJSON获取函数
*	形    参: 
*	返 回 值: 
*********************************************************************************************************
*/
void ActionControlOfThisRack(SingleStartUp temp,uint8_t * arr)
{
	if(temp.Action.type != 0)
	{
		if(*arr == 1)
			return;
	}
	SysDataFun(DATA_MSG_CTRL,temp.Action.type,temp.Action.id,4,0,arr,0);
}

/*
*********************************************************************************************************
*	函 数 名: 
*	功能说明: GUI刷新cJSON获取函数
*	形    参: 
*	返 回 值: 
*********************************************************************************************************
*/
uint16_t GUIGetcJSONFunAPI(uint16_t id,cJSON* pdata,uint16_t *length)
{
//	count = getJsonParaStruct(id,&temp);
}
/*
*********************************************************************************************************
*	函 数 名: 
*	功能说明: 初始化单动作到指定数组
*	形    参: 无
*	返 回 值: 无
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
			strcat(buf,"°");
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
*	函 数 名: 
*	功能说明: 初始化单动作到指定数组
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void SetMenuListtempMaxValue(SETLIST*  arr)
{
	uint16_t i;
	switch(arr->Parameter.pSelItem)
	{
		case 0xF0:// 辅助动作
			arr->Parameter.Max_Value = SingleActionBUF[0].MAX;//单动作+组合动作的最大值
			for(i=0;i<SingleActionBUF[0].MAX;i++)
			{
				if(((SingleActionBUF[i].type<<8) | (SingleActionBUF[i].id)) == arr->Parameter.Para_Value)
				{
					arr->Parameter.Para_Value = i;
					break;
				}
			}
		break;
		case 0xF1:// 键盘动作
			arr->Parameter.Max_Value = SingleActionBUF[1].MAX;//.id临时用于存储单动作的最大值
			for(i=0;i<SingleActionBUF[1].MAX;i++)
			{
				if(SingleActionBUF[i].id == arr->Parameter.Para_Value)
				{
					arr->Parameter.Para_Value = i;
					break;
				}
			}
		break;
		case 0xF2:// 传感器显示
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
		default:
			break;
	}
}
/*
*********************************************************************************************************
*	函 数 名: 
*	功能说明: 初始化单动作到指定数组
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void SetMenuListtempValue(SETLIST*  arr)
{
	uint16_t i;
	switch(arr->Parameter.pSelItem)
	{
		case 0xF0:// 辅助动作
			arr->Parameter.Para_Value = (SingleActionBUF[arr->Parameter.Para_Value].type<<8) |(SingleActionBUF[arr->Parameter.Para_Value].id);
		break;
		case 0xF1:// 键盘动作
			arr->Parameter.Para_Value = SingleActionBUF[arr->Parameter.Para_Value].id;
		break;
		case 0xF2:// 传感器显示
			arr->Parameter.Para_Value = SensorShowBUF[arr->Parameter.Para_Value].id;
		break;	
		default:
			break;
	}
}
/*
*********************************************************************************************************
*	函 数 名: 
*	功能说明: 初始化单动作到指定数组
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void AuxiliaryActionShowEnter(char** arr ,uint16_t pSelItem,uint16_t Value)
{
	uint16_t i;
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
	}
}
/*
*********************************************************************************************************
*	函 数 名: 
*	功能说明: 初始化单动作到指定数组
*	形    参: 无
*	返 回 值: 无
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
	}
}
/*
*********************************************************************************************************
*	函 数 名: 
*	功能说明: 初始化单动作到指定数组
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void InitSingleAction(void)
{
	uint8_t  *temp;
	uint64_t id = 0x200000000000000;
	MENU_PARA temp_value;
	uint16_t length,pdata,count,count1,i = 1;
	//获取单动作1的起始地址 id 3.1.0
	count = getJsonParaStructSize(id);
	for(i= 1;i<count+1;i++)
	{
		temp = getJsonParaAddress(((id >> 48 )+i)<<48);	
		SingleActionBUF[i].name = (char *)temp;		
		memcpy(&SingleActionBUF[i].id,temp+48,2);		
		SingleActionBUF[i].type = Sing;
	}
	SingleActionBUF[1].MAX = i-1;
	id = 0x400000000000000;
	count1 = getJsonParaStructSize(id);
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
*	函 数 名: 
*	功能说明: 初始化单动作到指定数组
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void InitGroupAction(void)
{
	uint8_t  *temp;
	MENU_PARA * menu;
	uint64_t id = 0x600000000000000;
	uint16_t length,pdata,count,i = 1;
	//获取单动作1的起始地址 id 3.1.0
	count = getJsonParaStructSize(id);
	for(i= 1;i<count+1;i++)
	{
		temp = getJsonParaAddress(((id >> 48 )+i)<<48);	
		GroupActionBUF[i].name = (char *)temp;		
		memcpy(&GroupActionBUF[i].id,temp+48,2);		
		GroupActionBUF[i].type = Groupa;
	}
	GroupActionBUF[0].MAX = i-1;
	SetGroupMAXValue(i);
}

/*
*********************************************************************************************************
*	函 数 名: 
*	功能说明: 初始化单动作到指定数组
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void InitSensorShow(void)
{
	uint8_t  *temp;
	STR_V4PARA_NAME *temp1;
	uint8_t* pstate=NULL;
	SensorParamType SensorParam;
	uint64_t id = 0x300000000000000;
	uint16_t length,count,len,i = 1;
	//获取单动作1的起始地址 id 3.1.0
	count = getJsonParaStructSize(id);
	for(i= 1;i<count+1;i++)
	{
		temp = getJsonParaAddress(((id >> 48 )+i)<<48);	
		SensorShowBUF[i].nameshow = (char*)temp;		
		memcpy(&SensorShowBUF[i].id,temp+48,2);
		memcpy(&SensorShowBUF[i].Nuit,temp+24,2);
		memcpy(&SensorShowBUF[i].acc,temp+20,1);
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
*	函 数 名: 
*	功能说明: 获取辅助动作放进一个数组
*	形    参: 无
*	返 回 值: 无
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
*	函 数 名: 
*	功能说明: 获取键盘动作放进一个数组
*	形    参: 无
*	返 回 值: 无
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
*	函 数 名: 
*	功能说明: 获取传感器显示放进一个数组
*	形    参: 无
*	返 回 值: 无
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
*	函 数 名: 
*	功能说明: GUI刷新数据获取函数
*	形    参: 
*	返 回 值: 
*********************************************************************************************************
*/
extern uint8_t Dir;
uint16_t GUIGetDateFunAPI(uint16_t Type,uint16_t Id,void* pdata,void *length)
{
	uint16_t  text[30] = {1,2,3,4,5,6,7,8,9,10,0x0101};
	static uint8_t beepsta;
	static uint8_t time;
	uint8_t data;
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
		case API_Support_Number://获取编号
			SysDataFun(DATA_MSG_READ_OBJECT_PARAM,ENUM_SYSTEM,globalParamEnum_Support,0,0,&pstate,&len);
			memcpy((uint8_t*)&prar,pstate,len);
			*((uint8_t *)pdata) = prar.currentSupportID;
		break;
		case API_Current_Data:

		break;
		case API_Current_Timr:

		break;
		case API_Machine_Position:

		break;
		case API_Running_Direction:
			*((uint8_t *)pdata) = Dir;

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
		default:
		break;
		
	}
	
	return 0;
}
/*
*********************************************************************************************************
*	函 数 名: 
*	功能说明: 逻辑层调用函数函数
*	形    参: 
*	返 回 值: 
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
}
/*
*********************************************************************************************************
*	函 数 名: 
*	功能说明: 逻辑层调用函数函数
*	形    参: 
*	返 回 值: 
*********************************************************************************************************
*/
void GetShowAgain(void)
{
	GetAuxiliaryActionIntoArray();
	GetKeyboardActionIntoArray();
	GetSensorShowIntoArray();
}



/*
*********************************************************************************************************
*	函 数 名: 
*	功能说明: 逻辑层调用函数函数
*	形    参: 
*	返 回 值: 
*********************************************************************************************************
*/
//急停闭锁停止；按键设置变化；辅助按键设置变化；主界面显示传感器变化
void GUIInPutFun(void)
{
	
}








