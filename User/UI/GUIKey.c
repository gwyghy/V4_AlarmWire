/*
*********************************************************************************************************
*	                                  
*	模块名称 : GUI界面主函数
*	文件名称 : MainTask.c
*	版    本 : V1.0
*	说    明 : 本实验主要学习外部实体按键操作控件。
*              
*	修改记录 :
*		版本号   日期         作者          说明
*		V1.0    2016-07-16   Eric2013  	    首版    
*                                     
*	Copyright (C), 2015-2020, 安富莱电子 www.armfly.com
*
*********************************************************************************************************
*/

#include "bsp.h"
#include "User_GUI.h"
#include	"bsp_beeps.h"

/*
*********************************************************************************************************
*	函 数 名: 
*	功能说明: 将GUI按键转换成数值
*	形    参: 
*	返 回 值: 
*********************************************************************************************************
*/ 
uint16_t DigitKeyToChr(uint16_t Key)
{
	uint16_t count = 0xFFFF;
	switch (Key)
	{
		case USER_KEY_RIGHT1:
			count = 1;
			break;
		case USER_KEY_RIGHT2:
			count = 4;
			break;
		case USER_KEY_RIGHT3:
			count = 7;
			break;
		case USER_KEY_2:
			count = 2;
			break;
		case USER_KEY_3:
			count = 3;
			break;
		case USER_KEY_5:
			count = 5;
			break;
		case USER_KEY_6:
			count = 6;
			break;
		case USER_KEY_8:
			count = 8;
			break;
		case USER_KEY_9:
			count = 9;
			break;
		case USER_KEY_ASSIST:
			count = 0;
			break;	
		default:
			break;	
	}
	return count;
}
#if 1
int GUI_KeyUser(uint32_t data)
{
	static uint32_t keyold;
	static uint8_t tage = 0;
	uint32_t key,keytemp;
	int flag = -1;
	OvertimeZero();
//	if(data != 0)
//	{
//		if(keyold == 0)
//		{
//			TIM_ClearITPendingBit(TIM3, TIM_IT_Update );
//			TIM_Cmd(TIM3, ENABLE);
//			
//			TIM3->CNT = 0;
//		}
//		else
//		{
//			TIM3->CNT = 0;
//		}
//	}
//	else
//	{
//		TIM_Cmd(TIM3, DISABLE); 
//	}
	if(data != keyold)
	{
		flag = 0;
		keytemp = keyold ^ data;	
		if((keytemp & keyold) > 0)
		{
			key = keytemp & keyold;
			if((key & KEY_L1) == KEY_L1)
			{
				GUI_SendKeyMsg(USER_KEY_LEFT1, UP);
			}
			if((key & KEY_L2) == KEY_L2)
			{
				GUI_SendKeyMsg(USER_KEY_LEFT2, UP);
			}
			if((key & KEY_L3) == KEY_L3)
			{
				GUI_SendKeyMsg(USER_KEY_LEFT3, UP);
			}
			if((key & KEY_R1) == KEY_R1)
			{
				GUI_SendKeyMsg(USER_KEY_RIGHT1, UP);
			}
			if((key & KEY_R2) == KEY_R2)
			{
				GUI_SendKeyMsg(USER_KEY_RIGHT2, UP);
			}
			if((key & KEY_R3) == KEY_R3)
			{
				GUI_SendKeyMsg(USER_KEY_RIGHT3, UP);
			}
			if((key & KEY_2) == KEY_2)
			{
				GUI_SendKeyMsg(USER_KEY_2, UP);
			}
			if((key & KEY_3) == KEY_3)
			{
				GUI_SendKeyMsg(USER_KEY_3, UP);
			}
			if((key & KEY_5) == KEY_5)
			{
				GUI_SendKeyMsg(USER_KEY_5, UP);
			}
			if((key & KEY_6) == KEY_6)
			{
				GUI_SendKeyMsg(USER_KEY_6, UP);
			}
			if((key & KEY_8) == KEY_8)
			{
				GUI_SendKeyMsg(USER_KEY_8, UP);
			}
			if((key & KEY_9) == KEY_9)
			{
				GUI_SendKeyMsg(USER_KEY_9, UP);
			}
			if((key & KEY_STOP) == KEY_STOP)
			{
				GUI_SendKeyMsg(USER_KEY_ESC, UP);
			}
			if((key & KEY_UP) == KEY_UP)
			{
				GUI_SendKeyMsg(USER_KEY_UP, UP);
			}
			if((key & KEY_DOWN) == KEY_DOWN)
			{
				GUI_SendKeyMsg(USER_KEY_DOWN, UP);
			}
			if((key & KEY_LEFT) == KEY_LEFT)
			{
				GUI_SendKeyMsg(USER_KEY_LEFT, UP);
			}
			if((key & KEY_RIGHT) == KEY_RIGHT)
			{
				GUI_SendKeyMsg(USER_KEY_RIGHT, UP);
			}
			if((key & KEY_START) == KEY_START)
			{
				GUI_SendKeyMsg(USER_KEY_ENTER, UP);
			}
			if((key & KEY_0) == KEY_0)
			{
				GUI_SendKeyMsg(USER_KEY_ASSIST, UP);
			}
			if((key & KEY_WORK) == KEY_WORK)
			{
				if(tage == 1)
					GUI_SendKeyMsg(USER_KEY_WORK, UP);
				tage = 0;
			}
		}
		if((keytemp & data) > 0)
		{
			BeepOn(BEEP_KEY_PRESSED);
			key = keytemp & data;
			if((key & KEY_L1) == KEY_L1)
			{
				GUI_SendKeyMsg(USER_KEY_LEFT1, DOWN);
			}
			if((key & KEY_L2) == KEY_L2)
			{
				GUI_SendKeyMsg(USER_KEY_LEFT2, DOWN);
			}
			if((key & KEY_L3) == KEY_L3)
			{
				GUI_SendKeyMsg(USER_KEY_LEFT3, DOWN);
			}
			if((key & KEY_R1) == KEY_R1)
			{
				GUI_SendKeyMsg(USER_KEY_RIGHT1, DOWN);
			}
			if((key & KEY_R2) == KEY_R2)
			{
				GUI_SendKeyMsg(USER_KEY_RIGHT2, DOWN);
			}
			if((key & KEY_R3) == KEY_R3)
			{
				GUI_SendKeyMsg(USER_KEY_RIGHT3, DOWN);
			}
			if((key & KEY_2) == KEY_2)
			{
				GUI_SendKeyMsg(USER_KEY_2, DOWN);
			}
			if((key & KEY_3) == KEY_3)
			{
				GUI_SendKeyMsg(USER_KEY_3, DOWN);
			}
			if((key & KEY_5) == KEY_5)
			{
				GUI_SendKeyMsg(USER_KEY_5, DOWN);
			}
			if((key & KEY_6) == KEY_6)
			{
				GUI_SendKeyMsg(USER_KEY_6, DOWN);
			}
			if((key & KEY_8) == KEY_8)
			{
				GUI_SendKeyMsg(USER_KEY_8, DOWN);
			}
			if((key & KEY_9) == KEY_9)
			{
				GUI_SendKeyMsg(USER_KEY_9, DOWN);
			}
			if((key & KEY_STOP) == KEY_STOP)
			{
				GUI_SendKeyMsg(USER_KEY_ESC, DOWN);
			}
			if((key & KEY_UP) == KEY_UP)
			{
				GUI_SendKeyMsg(USER_KEY_UP, DOWN);
			}
			if((key & KEY_DOWN) == KEY_DOWN)
			{
				if(tage == 1)
				{
					GUI_SendKeyMsg(USER_KEY_RECOIL, DOWN);
					tage = 0;
				}
				else
					GUI_SendKeyMsg(USER_KEY_DOWN, DOWN);
			}
			if((key & KEY_LEFT) == KEY_LEFT)
			{
				if(tage == 1)
				{
					GUI_SendKeyMsg(USER_KEY_FOLLOW_LEFT, DOWN);
					tage = 0;
				}
				else
					GUI_SendKeyMsg(USER_KEY_LEFT, DOWN);
			}
			if((key & KEY_RIGHT) == KEY_RIGHT)
			{
				if(tage == 1)
				{
					GUI_SendKeyMsg(USER_KEY_FOLLOW_RIGHT, DOWN);
					tage = 0;
				}
				else
					GUI_SendKeyMsg(USER_KEY_RIGHT, DOWN);
			}
			if((key & KEY_START) == KEY_START)
			{
				GUI_SendKeyMsg(USER_KEY_ENTER, DOWN);
			}
			if((key & KEY_0) == KEY_0)
			{
				if(tage == 1)
				{
					GUI_SendKeyMsg(USER_KEY_MENU, DOWN);
					tage = 0;
				}
				else
					GUI_SendKeyMsg(USER_KEY_ASSIST, DOWN);				
			}
			if((key & KEY_WORK) == KEY_WORK)
			{
				GUI_SendKeyMsg(USER_KEY_WORK, DOWN);
				tage = 1;
			}
		}
		
		keyold = data;
	}
	return flag;
}
#else
int GUI_KeyUser(uint32_t data)
{
	uint8_t err;
	err = OSMboxPost(KeyMbox,(void *)data);
	if (err == DEF_OK)
	{
		return 1;
	}else
	if (err == DEF_MBOX_FULL)
	{
		/* 此处应加判断是否需要延时重发，同时是否需要写入错误日志 */ 
		return -1;
	}
	
	return -2;
}
#endif
#if 1
/*
*********************************************************************************************************
*	函 数 名: 
*	功能说明: 将按键转换成GUI按键
*	形    参: 
*	返 回 值: 
*********************************************************************************************************
*/
uint8_t GUI_Key(void)
{
	uint8_t  err;
	uint32_t data; 
	static uint32_t keyold;
	static uint8_t tage = 0;
	uint32_t key,keytemp;
	data = (uint32_t)(OSMboxPend(KeyMbox,50,&err));	
	if(err == OS_ERR_TIMEOUT)
		return 0;
	if(data != keyold)
	{
		keytemp = keyold ^ data;	

		if((keytemp & keyold) > 0)
		{
			key = keytemp & keyold;
			if((key & KEY_L1) == KEY_L1)
			{
				GUI_SendKeyMsg(USER_KEY_LEFT1, UP);
			}
			if((key & KEY_L2) == KEY_L2)
			{
				GUI_SendKeyMsg(USER_KEY_LEFT2, UP);
			}
			if((key & KEY_L3) == KEY_L3)
			{
				GUI_SendKeyMsg(USER_KEY_LEFT3, UP);
			}
			if((key & KEY_R1) == KEY_R1)
			{
				GUI_SendKeyMsg(USER_KEY_RIGHT1, UP);
			}
			if((key & KEY_R2) == KEY_R2)
			{
				GUI_SendKeyMsg(USER_KEY_RIGHT2, UP);
			}
			if((key & KEY_R3) == KEY_R3)
			{
				GUI_SendKeyMsg(USER_KEY_RIGHT3, UP);
			}
			if((key & KEY_2) == KEY_2)
			{
				GUI_SendKeyMsg(USER_KEY_2, UP);
			}
			if((key & KEY_3) == KEY_3)
			{
				GUI_SendKeyMsg(USER_KEY_3, UP);
			}
			if((key & KEY_5) == KEY_5)
			{
				GUI_SendKeyMsg(USER_KEY_5, UP);
			}
			if((key & KEY_6) == KEY_6)
			{
				GUI_SendKeyMsg(USER_KEY_6, UP);
			}
			if((key & KEY_8) == KEY_8)
			{
				GUI_SendKeyMsg(USER_KEY_8, UP);
			}
			if((key & KEY_9) == KEY_9)
			{
				GUI_SendKeyMsg(USER_KEY_9, UP);
			}
			if((key & KEY_STOP) == KEY_STOP)
			{
				GUI_SendKeyMsg(USER_KEY_ESC, UP);
			}
			if((key & KEY_UP) == KEY_UP)
			{
				GUI_SendKeyMsg(USER_KEY_UP, UP);
			}
			if((key & KEY_DOWN) == KEY_DOWN)
			{
				GUI_SendKeyMsg(USER_KEY_DOWN, UP);
			}
			if((key & KEY_LEFT) == KEY_LEFT)
			{
				GUI_SendKeyMsg(USER_KEY_LEFT, UP);
			}
			if((key & KEY_RIGHT) == KEY_RIGHT)
			{
				GUI_SendKeyMsg(USER_KEY_RIGHT, UP);
			}
			if((key & KEY_START) == KEY_START)
			{
				GUI_SendKeyMsg(USER_KEY_ENTER, UP);
			}
			if((key & KEY_0) == KEY_0)
			{
				GUI_SendKeyMsg(USER_KEY_ASSIST, UP);
			}
			if((key & KEY_WORK) == KEY_WORK)
			{
				if(tage == 1)
					GUI_SendKeyMsg(USER_KEY_WORK, UP);
				tage = 0;
			}
		}
		if((keytemp & data) > 0)
		{
			key = keytemp & data;
			if((key & KEY_L1) == KEY_L1)
			{
				GUI_SendKeyMsg(USER_KEY_LEFT1, DOWN);
			}
			if((key & KEY_L2) == KEY_L2)
			{
				GUI_SendKeyMsg(USER_KEY_LEFT2, DOWN);
			}
			if((key & KEY_L3) == KEY_L3)
			{
				GUI_SendKeyMsg(USER_KEY_LEFT3, DOWN);
			}
			if((key & KEY_R1) == KEY_R1)
			{
				GUI_SendKeyMsg(USER_KEY_RIGHT1, DOWN);
			}
			if((key & KEY_R2) == KEY_R2)
			{
				GUI_SendKeyMsg(USER_KEY_RIGHT2, DOWN);
			}
			if((key & KEY_R3) == KEY_R3)
			{
				GUI_SendKeyMsg(USER_KEY_RIGHT3, DOWN);
			}
			if((key & KEY_2) == KEY_2)
			{
				GUI_SendKeyMsg(USER_KEY_2, DOWN);
			}
			if((key & KEY_3) == KEY_3)
			{
				GUI_SendKeyMsg(USER_KEY_3, DOWN);
			}
			if((key & KEY_5) == KEY_5)
			{
				GUI_SendKeyMsg(USER_KEY_5, DOWN);
			}
			if((key & KEY_6) == KEY_6)
			{
				GUI_SendKeyMsg(USER_KEY_6, DOWN);
			}
			if((key & KEY_8) == KEY_8)
			{
				GUI_SendKeyMsg(USER_KEY_8, DOWN);
			}
			if((key & KEY_9) == KEY_9)
			{
				GUI_SendKeyMsg(USER_KEY_9, DOWN);
			}
			if((key & KEY_STOP) == KEY_STOP)
			{
				GUI_SendKeyMsg(USER_KEY_ESC, DOWN);
			}
			if((key & KEY_UP) == KEY_UP)
			{
				GUI_SendKeyMsg(USER_KEY_UP, DOWN);
			}
			if((key & KEY_DOWN) == KEY_DOWN)
			{
				GUI_SendKeyMsg(USER_KEY_DOWN, DOWN);
			}
			if((key & KEY_LEFT) == KEY_LEFT)
			{
				GUI_SendKeyMsg(USER_KEY_LEFT, DOWN);
			}
			if((key & KEY_RIGHT) == KEY_RIGHT)
			{
				GUI_SendKeyMsg(USER_KEY_RIGHT, DOWN);
			}
			if((key & KEY_START) == KEY_START)
			{
				GUI_SendKeyMsg(USER_KEY_ENTER, DOWN);
			}
			if((key & KEY_0) == KEY_0)
			{
				if(tage == 1)
				{
					GUI_SendKeyMsg(USER_KEY_MENU, DOWN);
					tage = 0;
				}
				else
					GUI_SendKeyMsg(USER_KEY_ASSIST, DOWN);				
			}
			if((key & KEY_WORK) == KEY_WORK)
			{
//				GUI_SendKeyMsg(USER_KEY_WORK, DOWN);
				tage = 1;
			}
		}
	


		keyold = data;
	}
}
#endif
