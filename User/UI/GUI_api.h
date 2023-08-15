/*
*********************************************************************************************************
*	                                  
*	ģ������ : GUI����������
*	�ļ����� : HomeTask.c
*	��    �� : V1.0
*	˵    �� : GUI����������
*		�汾��   ����         ����            ˵��
*		v1.0    2015-08-05  Eric2013  	      �װ�
*
*	Copyright (C), 2015-2020, ���������� www.armfly.com
*
*********************************************************************************************************
*/

#ifndef __GUI_API_H
#define __GUI_API_H

#include "MenuConfig.h"

/*
*********************************************************************************************************
*	                                     �궨��
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
	API_Support_Statur,//֧��״̬
	API_Support_Number,//֧�ܱ��
	API_Current_Data,//��ǰ����
	API_Current_Timr,//��ǰʱ��
	API_Machine_Position,//ú��λ��
	API_Running_Direction,//���з���
	API_Signal_Intensity,//�ź�ǿ��
	API_Display_Item,//��ʾ��
	API_Number_Increase,//�ܺ�����
	API_Number_MAX,//���ܺ�
	API_Number_MIN,//��С�ܺ�
	API_Control_Range,//���ܼ���
	API_Auxiliary_Action,//��������	
	API_Keyboard_Action,//���̶���
	API_GroupAuxiliary_Action,//���鸨������
	API_Sensor_Show,//��������ʾ
	API_Group_Action,//���鶯��
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
*						���ⲿ�ļ�����
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
