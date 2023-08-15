/*
*********************************************************************************************************
*	                                  
*	模块名称 : gui用户库
*	文件名称 : User_GUI.c
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


#include "User_GUI.h"
#include "sys_msg_def.h"
#include "FileLoad.h"
/*
*********************************************************************************************************
*	                                     宏定义
*********************************************************************************************************
*/

static USER_PARAM user_param;

/*
*********************************************************************************************************
*	函 数 名: 
*	功能说明: 设置当前页数
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void SetPageValue(uint8_t value)
{
	user_param.menupaga = value;
}
/*
*********************************************************************************************************
*	函 数 名: 
*	功能说明: 获取当前页数
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
uint8_t GetPageValue(void)
{
//	param->paga = user_param.paga;
	return user_param.menupaga;
}
/*
*********************************************************************************************************
*	函 数 名: 
*	功能说明: 设置当前页数
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void SetSinglePageValue(uint8_t value)
{
	user_param.Singlepaga = value;
}
/*
*********************************************************************************************************
*	函 数 名: 
*	功能说明: 获取当前页数
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
uint8_t GetSinglePageValue(void)
{
//	param->paga = user_param.paga;
	return user_param.Singlepaga;
}
/*
*********************************************************************************************************
*	函 数 名: 
*	功能说明: 设置当前页数
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void SetSingleMAXValue(uint8_t value)
{
	user_param.SingleMAX = value;
}
/*
*********************************************************************************************************
*	函 数 名: 
*	功能说明: 获取当前页数
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
uint8_t GetSingleMAXValue(void)
{
//	param->paga = user_param.paga;
	return user_param.SingleMAX;
}
/*
*********************************************************************************************************
*	函 数 名: 
*	功能说明: 设置当前页数
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void SetGroupPageValue(uint8_t value)
{
	user_param.Grouppaga = value;
}
/*
*********************************************************************************************************
*	函 数 名: 
*	功能说明: 获取当前页数
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
uint8_t GetGroupPageValue(void)
{
//	param->paga = user_param.paga;
	return user_param.Grouppaga;
}

/*
*********************************************************************************************************
*	函 数 名: 
*	功能说明: 设置当前页数
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void SetAuxiliaryPageValue(uint8_t value)
{
	user_param.Grouppaga = value;
}
/*
*********************************************************************************************************
*	函 数 名: 
*	功能说明: 获取当前页数
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
uint8_t GetAuxiliaryPageValue(void)
{
//	param->paga = user_param.paga;
	return user_param.Grouppaga;
}

/*
*********************************************************************************************************
*	函 数 名: 
*	功能说明: 设置当前页数
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void SetGroupMAXValue(uint8_t value)
{
	user_param.GroupMAX = value;
}
/*
*********************************************************************************************************
*	函 数 名: 
*	功能说明: 获取当前页数
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
uint8_t GetGroupMAXValue(void)
{
//	param->paga = user_param.paga;
	return user_param.GroupMAX;
}
/*
*********************************************************************************************************
*	函 数 名: 
*	功能说明: 设置当前行数
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void SetRowValue(uint8_t value)
{
	user_param.row = value;
}
/*
*********************************************************************************************************
*	函 数 名: 
*	功能说明: 获取当前行数
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
uint8_t GetRowValue(void)
{
	return user_param.row;
}
/*
*********************************************************************************************************
*	函 数 名: 
*	功能说明: 设置当前是否要修改参数
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void SetconfigState(uint8_t value)
{
	user_param.config = value;
}
/*
*********************************************************************************************************
*	函 数 名: 
*	功能说明: 获取当前是否要修改参数
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
uint8_t GetconfigState(void)
{
	return user_param.config;
}
/*
*********************************************************************************************************
*	函 数 名: 
*	功能说明: 设置当前页面
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void Setrankvalue(uint8_t i,uint8_t value)
{
	if(i > LEVEL_MAX)
		return;
	user_param.rank[i] = value;
}
/*
*********************************************************************************************************
*	函 数 名: 
*	功能说明: 获取当前页面
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
uint64_t Getrankvalue(uint8_t * arr)
{
	uint64_t temp = 0;
	temp |= ((uint64_t)user_param.rank[7]) ;
	temp |= ((uint64_t)user_param.rank[6] <<8);
	temp |= ((uint64_t)user_param.rank[5] <<16);
	temp |= ((uint64_t)user_param.rank[4] <<24);
	temp |= ((uint64_t)user_param.rank[3] <<32);
	temp |= ((uint64_t)user_param.rank[2] <<40);
	temp |= ((uint64_t)user_param.rank[1] <<48);
	temp |= ((uint64_t)user_param.rank[0] <<56);
	
	memcpy((arr),user_param.rank,sizeof(user_param.rank));
	return temp;
}
/*
*********************************************************************************************************
*	函 数 名: 
*	功能说明: 设置当前是否要修改参数
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void SetLevelValue(uint8_t value)
{
	user_param.level = value;
	SetPageValue(0);
}
/*
*********************************************************************************************************
*	函 数 名: 
*	功能说明: 获取当前是否要修改参数
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
uint8_t GetLevelValue(void)
{
	return user_param.level;
}
/*
*********************************************************************************************************
*	函 数 名: 
*	功能说明: 设置当前是否要修改参数
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void SetBufIndValue(uint8_t value)
{
	//1：连续输入。0.重新输入
	user_param.BufInd = value;
}
/*
*********************************************************************************************************
*	函 数 名: 
*	功能说明: 获取当前是否要修改参数
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
uint8_t GetBufIndValue(void)
{
	return user_param.BufInd;
}






void InitUserGui(void)
{
	memset(&user_param,0x00,sizeof(user_param));
}

/*
*********************************************************************************************************
*	函 数 名: USER_INIT_DIALOG
*	功能说明: 对话框的回调函数
*	形    参: pMsg 消息指针
*	返 回 值: 无
*********************************************************************************************************
*/
void ChangeInfoButton(WM_HWIN hWin,uint16_t  pStr)
{ 
	 WM_MESSAGE Message;
	 Message.MsgId = MSG_CHANGE_BUTTON;
	 Message.Data.v = pStr;
	 WM_SendMessage(hWin, &Message);
}

/*
*********************************************************************************************************
*	函 数 名: USER_INIT_DIALOG
*	功能说明: 对话框的回调函数
*	形    参: pMsg 消息指针
*	返 回 值: 无
*********************************************************************************************************
*/
void ChangeInfoText(WM_HWIN hWin,uint16_t  pStr)
{ 
	 WM_MESSAGE Message;
	 Message.MsgId = MSG_CHANGE_TEXT;
	 Message.Data.v = pStr;
	 WM_SendMessage(hWin, &Message);
}
/*
*********************************************************************************************************
*	函 数 名: USER_INIT_DIALOG
*	功能说明: 对话框的回调函数
*	形    参: pMsg 消息指针
*	返 回 值: 无
*********************************************************************************************************
*/
void ChangeInfoKey(WM_HWIN hWin,int  pKey,int pCnt)
{ 
	WM_MESSAGE Message;
	Message.MsgId = MSG_CHANGE_KEY;
	pKey = pKey |(pCnt<<28);
	Message.Data.v = pKey;
	WM_SendMessage(hWin, &Message);
}
int overtime;
void OvertimeClose(void)
{
	overtime = -1;
}
void OvertimeOpen(void)
{
	overtime = 0;
}
void OvertimeZero(void)
{
	if(overtime >= 0)
		overtime = 0;
}
extern WM_HWIN GroupLeft_hWin,MasterContor_hWin;
	
void OvertimeReturn(WM_MESSAGE * pMsg)
{
	uint16_t time = 500;
	//获取超时时间
	if(overtime >= 0)
		overtime++;
	if(overtime > time)
	{
		GUI_EndDialog(pMsg->hWin, 0);
		if((pMsg->hWin == GroupLeft_hWin) ||(pMsg->hWin == MasterContor_hWin))
			CreateHomeFrame();	
		else
			CreateBackGround();
	}
}
uint8_t Initialstate;
/*
*********************************************************************************************************
*	函 数 名: Setstheinitialstate
*	功能说明: 设置初始状态
*	形    参: pMsg 消息指针
*	返 回 值: 无
*********************************************************************************************************
*/
void Setstheinitialstate(void)
{
	uint8_t length;
	globalStateType  temp;
	GUIGetDateFunAPI(API_Support_Statur,0,&temp,&length);
	if((temp.emergencyState == 1) || (temp.bisuoState == 1) ||(temp.stopState == 1))
	{
		Initialstate = 1;//初始状态为急停闭锁
	}
	else
	{
		Initialstate = 0;//初始状态为空闲
	}
}
/*
*********************************************************************************************************
*	函 数 名: Setstheinitialstate
*	功能说明: 判断是否需要返回主界面
*	形    参: 0：急停闭锁返回，1：急停闭锁不返回
*	返 回 值: 无
*********************************************************************************************************
*/
void Determineneedtoreturninterface(WM_MESSAGE * pMsg,uint8_t val)
{	
	uint8_t RecvStatus,back = 0;
	uint8_t length;
	globalStateType  temp;
	RecvStatus = getFileRecvStatus();
	if((RecvStatus == FILEREADY) || (RecvStatus == FILEDATA))//正在接收程序或准备接收程序
	{
		back = 1;
	}
	if(!val)
	{
		if(!Initialstate)//原来不是急停闭锁状态
		{
			GUIGetDateFunAPI(API_Support_Statur,0,&temp,&length);
			if((temp.emergencyState == 1) || (temp.bisuoState == 1) ||(temp.stopState == 1))
			{
				back = 1;
			}	
		}
	}
	if(back)
	{
		GUI_EndDialog(pMsg->hWin, 0);
		if((pMsg->hWin == GroupLeft_hWin) ||(pMsg->hWin == MasterContor_hWin))
			CreateHomeFrame();	
		else
			CreateBackGround();
	}	
}
