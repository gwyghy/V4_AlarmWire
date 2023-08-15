/*
*********************************************************************************************************
*	                                  
*	模块名称 : GUI界面主函数
*	文件名称 : HomeTask.c
*	版    本 : V1.0
*	说    明 : GUI界面主函数
*		版本号   日期         作者            说明
*		v1.0    2015-08-05  Eric2013  	      首版
*
*	Copyright (C), 2015-2020, 安富莱电子 www.armfly.com
*
*********************************************************************************************************
*/

#ifndef __GUI_API_H
#define __GUI_API_H

#include "MenuConfig.h"

/*
*********************************************************************************************************
*	                                     宏定义
*********************************************************************************************************
*/
#define   SingleActionMAX  60
//#define   CombinedActionMAX  16
#define   GroupActionMAX  42
#define   SensorShowMAX   40

#define   AuxiliaryActionMAX  30
#define   KeyboardActionMAX  6
#define   SensorMAX   6




//typedef struct
//{
//	NameAndId name;
//	uint16_t max;	
//}NameAndId;

enum
{
	Sing,
	Combin,
	Groupa,
};


enum
{
	API_Support_Statur,//支架状态
	API_Support_Number,//支架编号
	API_Current_Data,//当前日期
	API_Current_Timr,//当前时间
	API_Machine_Position,//煤机位置
	API_Running_Direction,//运行方向
	API_Signal_Intensity,//信号强度
	API_Display_Item,//显示项
	API_Number_Increase,//架号增向
	API_Number_MAX,//最大架号
	API_Number_MIN,//最小架号
	API_Control_Range,//隔架架数
	API_Auxiliary_Action,//辅助动作	
	API_Keyboard_Action,//键盘动作
	API_GroupAuxiliary_Action,//成组辅助动作
	API_Sensor_Show,//传感器显示
	API_Group_Action,//成组动作
	API_Support_Slave,
	API_Sensor_Version,
	API_Singel_Action,
	API_Comblnation_Action,
	API_Transport_Plane,
	
};

/*
************************************************************************
*						  FatFs
************************************************************************
*/



/*
************************************************************************
*						供外部文件调用
************************************************************************
*/
uint16_t GUIGetDateFunAPI(uint16_t Type,uint16_t Id,void* pdata,void *length);
void GUIInitShow(void);
void ActionControlOfThisRack(SingleStartUp temp,uint8_t  arr);
void ActionControlOfThisSlve(SingleStartUp temp,uint8_t  arr);
void ActionControlOfThisHome(SingleStartUp temp,uint8_t  arr);
void AuxiliaryActionShow(char** arr ,uint16_t pSelItem,uint16_t Value);
void SetMenuListtempMaxValue(SETLIST*  arr);
void AddUnitDispiay(char* buf,uint16_t val);
void AddUnitDispiaySETLIST(char* buf,SETLIST*  arr);
void AddUnitDispiayINITLIST(char* buf,INITLIST*  arr);
void SetMenuListtempValue(SETLIST*  arr);
void AuxiliaryActionShowEnter(char** arr ,uint16_t pSelItem,uint16_t Value);
void GetShowAgain(uint8_t val);
void ActionControlOfAdjacentRack(SingleStartUp temp,uint8_t cmd);
void ActionControlOfGroupRack(GroupStartUp temp);
uint8_t GetAuxiliarySige(void);
void SetAuxiliarySigereturn(void);
void ResetAuxiliarySigereturn(void);
void QueryOppositeAction(SingleStartUp temp,uint8_t cmd);
#endif

/*****************************(END OF FILE) *********************************/
