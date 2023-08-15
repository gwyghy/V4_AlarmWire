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
#include "usartapp.h"
#include "includes.h"					//ucos 使用	  
#include "FileLoad.h"
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
TEXT_Handle 	hModeText[15];	

static const GUI_WIDGET_CREATE_INFO _aDialogCreate2[] = 
{
	{ WINDOW_CreateIndirect, "Window", GUI_ID_WINDOW2, 65,60, 360, 139, 0, 0x64, 0 },
	{ TEXT_CreateIndirect, " ", GUI_ID_TEXT21, 116-65,110-60,84 ,44, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, " ", GUI_ID_TEXT22, 201-65,110-60,105,44, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, " ", GUI_ID_TEXT23, 307-65,103-60,80 ,45, 0, 0x0, 0 },
};
static const GUI_WIDGET_CREATE_INFO _aDialogCreate1[] = 
{
	{ WINDOW_CreateIndirect, "Window", GUI_ID_WINDOW1, 0, 0, 480, 320, 0, 0x64, 0 },
	{ TEXT_CreateIndirect, " ", GUI_ID_TEXT0, 75, 125, 330, 50, 0, 0x0, 0 },

};
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = 
{
	{ WINDOW_CreateIndirect, "Window", GUI_ID_WINDOW0, 0, 0, 480, 320, 0, 0x64, 0 },
	{ BUTTON_CreateIndirect, " ", MENU_BUTTON_LEFT_ID1, 0, 0, 59, 101, 0, 0x0, 0 },
	{ BUTTON_CreateIndirect, " ", MENU_BUTTON_LEFT_ID2, 0, 106, 59, 110, 0, 0x0, 0 },
	{ BUTTON_CreateIndirect, " ", MENU_BUTTON_LEFT_ID3, 0, 220, 59, 101, 0, 0x0, 0 },
	{ BUTTON_CreateIndirect, " ", MENU_BUTTON_RIGHT_ID1, 422, 0, 59, 101, 0, 0x0, 0 },
	{ BUTTON_CreateIndirect, " ", MENU_BUTTON_RIGHT_ID2, 422, 106, 59, 110, 0, 0x0, 0 },
	{ BUTTON_CreateIndirect, " ", MENU_BUTTON_RIGHT_ID3, 422, 220, 59, 101, 0, 0x0, 0 },	
};
extern WM_HWIN Boot_hWinp;

extern WM_HWIN BackGround_hWin,GroupLeft_hWin;


extern GUI_MEMDEV_Handle  	BackGround_DEV,
					Button_DEV,
					Signal_DEV1,
					Signal_DEV2,
					Signal_DEV3,
					TranSport_DEV ,
					DtrectionLeft_DEV0,
					DtrectionLeft_DEV1,
					DtrectionLeft_DEV2,
					DtrectionLeft_DEV3,
					DtrectionRight_DEV0,
					DtrectionRight_DEV1,
					DtrectionRight_DEV2,
					DtrectionRight_DEV3,
					ShearerLeft_DEV1,
					ShearerLeft_DEV2,
					ShearerRight_DEV1,
					ShearerRight_DEV2,
					SuddenStop_DEV,
					PowerOn_DEV,
					PassWord_DEV[2],
					HomeFrame_DEV[5],
					Group_DEV[5];
					
WM_HTIMER 	ShearerLeft_hTimer,
			ShearerRight_hTimer,
			DtrectionLeft_hTimer,
			DtrectionRight_hTimer,
			SuddenStop_hTimer,
			TranSport_hTimer,
			Signal_hTimer,
			BackGround_hTimer,
			HomeFrame_hTimer,
			PassWord_hTimer;
			
			
WM_HWIN ShearerLeft_hWin,
		ShearerRight_hWin,
		DtrectionLeft_hWin,
		DtrectionRight_hWin,
		SuddenStop_hWin,
		TranSport_hWin,
		Button_hWin,
		Signal_hWin,
		BackGround_hWin,
		HomeFrame_hWin,
		PassWord_hWin;
		

		
		
static void SuddenStop_cbDialog(WM_MESSAGE * pMsg) 
{
	WM_HWIN hItem;
	int a,b ;
	static uint8_t beepsta;
	switch (pMsg->MsgId) 
	{
		case WM_CREATE:
			CreateSuddenStopTextShow(pMsg);	
			break;
		case WM_TIMER:
			SupportStopRefresh(pMsg);
			WM_RestartTimer(SuddenStop_hTimer,100);
			break;
		case WM_PAINT:
			GUI_MEMDEV_WriteAt(SuddenStop_DEV, 60, 50);
		
			break;
		default:
			WM_DefaultProc(pMsg);
			break;
	}
}		
static void DtrectionLeft_cbDialog(WM_MESSAGE * pMsg) 
{
	static uint8_t beepsta;
	switch (pMsg->MsgId) 
	{
		case WM_TIMER:
			if(beepsta == 0)
				beepsta = 1;
			else if(beepsta == 1)
				beepsta = 2; 
			else if(beepsta == 2)
				beepsta = 3;
			else
				beepsta = 0;
			WM_InvalidateWindow(DtrectionLeft_hWin);
			WM_RestartTimer(DtrectionLeft_hTimer,500);
			break;
		case WM_PAINT:
			if(beepsta == 0)
				GUI_MEMDEV_WriteAt(DtrectionLeft_DEV0, 85, 60);
			else if(beepsta == 1)
				GUI_MEMDEV_WriteAt(DtrectionLeft_DEV1, 85, 60);
			else if(beepsta == 2)
				GUI_MEMDEV_WriteAt(DtrectionLeft_DEV2, 85, 60);
			else 
				GUI_MEMDEV_WriteAt(DtrectionLeft_DEV3, 85, 60);
			break;
		default:
			WM_DefaultProc(pMsg);
			break;
	}
}
static void DtrectionRight_cbDialog(WM_MESSAGE * pMsg) 
{
	static uint8_t beepsta;
	switch (pMsg->MsgId) 
	{
		case WM_TIMER:
			if(beepsta == 0)
				beepsta = 1;
			else if(beepsta == 1)
				beepsta = 2; 
			else if(beepsta == 2)
				beepsta = 3;
			else
				beepsta = 0;
			WM_InvalidateWindow(DtrectionRight_hWin);
			WM_RestartTimer(DtrectionRight_hTimer,500);
			break;
		case WM_PAINT:
			if(beepsta == 0)
				GUI_MEMDEV_WriteAt(DtrectionRight_DEV0, 85, 60);
			else if(beepsta == 1)
				GUI_MEMDEV_WriteAt(DtrectionRight_DEV1, 85, 60);
			else if(beepsta == 2)
				GUI_MEMDEV_WriteAt(DtrectionRight_DEV2, 85, 60);
			else 
				GUI_MEMDEV_WriteAt(DtrectionRight_DEV3, 85, 60);
			break;
		default:
			WM_DefaultProc(pMsg);
			break;
	}
}
	

static void ShearerLeft_cbDialog(WM_MESSAGE * pMsg) 
{ 
	static uint8_t beepsta;
	switch (pMsg->MsgId) 
	{
		case WM_INIT_DIALOG:
			break;
		case WM_TIMER:
			 beepsta=!beepsta;
			WM_InvalidateWindow(ShearerLeft_hWin);
			WM_RestartTimer(ShearerLeft_hTimer,600);

			break;
		case WM_PAINT:
			if(beepsta == 0)
				GUI_MEMDEV_WriteAt(ShearerLeft_DEV1, 90, 100);
			else
				GUI_MEMDEV_WriteAt(ShearerLeft_DEV2, 90, 100);
			break;
		default:
			WM_DefaultProc(pMsg);
			break;
	}
}
static void TranSport_cbDialog(WM_MESSAGE * pMsg) 
{ 
	static uint8_t beepsta;
	switch (pMsg->MsgId) 
	{
		case WM_INIT_DIALOG:
			break;
		case WM_TIMER:
			 beepsta=!beepsta;
			WM_InvalidateWindow(TranSport_hWin);
			WM_RestartTimer(TranSport_hTimer,600);

			break;
		case WM_PAINT:
			if(beepsta == 0)
				GUI_MEMDEV_WriteAt(TranSport_DEV, 60, 206);
			else
				GUI_MEMDEV_WriteAt(TranSport_DEV, 68, 206);
			break;
		default:
			WM_DefaultProc(pMsg);
			break;
	}
}
static void ShearerRight_cbDialog(WM_MESSAGE * pMsg) 
{ 
	static uint8_t beepsta;
	switch (pMsg->MsgId) 
	{
		case WM_INIT_DIALOG:
			break;
		case WM_TIMER:
			 beepsta=!beepsta;
			WM_InvalidateWindow(ShearerRight_hWin);
			WM_RestartTimer(ShearerRight_hTimer,600);
			break;
		case WM_PAINT:
			if(beepsta == 0)
				GUI_MEMDEV_WriteAt(ShearerRight_DEV1, 90, 100);
			else
				GUI_MEMDEV_WriteAt(ShearerRight_DEV2, 90, 100);
			break;
		default:
			WM_DefaultProc(pMsg);
			break;
	}
} 
uint32_t temp;
extern WM_HWIN 	MasterContor_hWin;
extern NameAndId  SingleActionBUF[];
extern NameAndId  GroupActionBUF[];
extern NameAndId  ShowAuxiliaryAction[];
extern NameAndId  ShowKeyboardAction[][2];
static void BackGround_cbDialog(WM_MESSAGE * pMsg) 
{
	uint32_t t0, t1;
	static uint8_t beepsta;
	WM_HWIN hItem;
	int     NCode;
	int     Id;
	switch (pMsg->MsgId) 
	{
		case WM_INIT_DIALOG:	
			USER_ReviseButtonStyle(pMsg);
			SetSinglePageValue(0);
			USER_ReviseSingleActionButtonShow(pMsg,ShowAuxiliaryAction,0);
		break;
		case MSG_CHANGE_KEY:
			if((pMsg->Data.v >> 28) > 0) //按键按下
			{
				USER_DownButtonShow(pMsg,(pMsg ->Data.v & 0x0FFF));
			}
			else
			{
				USER_UpButtonShow(pMsg,(pMsg ->Data.v & 0x0FFF));
			}
		break;	
		case MSG_CHANGE_BUTTON:			
			if(pMsg ->Data.v  == 0) //单动作
			{
				if(GetMAXpage(ShowAuxiliaryAction[0].MAX) <= GetSinglePageValue())
				{
					SetSinglePageValue(0);
				}
				else
				{
					SetSinglePageValue(GetSinglePageValue()+1);
				}
				USER_ReviseSingleActionButtonShow(pMsg,ShowAuxiliaryAction,0);
			}
			else if(pMsg ->Data.v  == 1)//成组
			{
				if(GetMAXpage(GetGroupMAXValue()) <= GetGroupPageValue())
				{
					SetGroupPageValue(0);
				}
				else
				{
					SetGroupPageValue(GetGroupPageValue()+1);
				}
				USER_ReviseSingleActionButtonShow(pMsg,&GroupActionBUF[1],1);
			}
			else if(pMsg ->Data.v  == 2)//成组
			{
				USER_ReviseSingleActionButtonShow(pMsg,&GroupActionBUF[1],1);
			}
			else if(pMsg ->Data.v  == 3)//单
			{
				USER_ReviseSingleActionButtonShow(pMsg,ShowAuxiliaryAction,0);
			}
			WM_InvalidateWindow(pMsg->hWin);
		break;
		case WM_TIMER:	
			CreateHomeFrame();
			break;
		case WM_PAINT:
			t0 = GUI_GetTime();
			GUI_MEMDEV_WriteAt(BackGround_DEV, 0, 0);
			t1 = GUI_GetTime() - t0;
			break;
		default:
			WM_DefaultProc(pMsg);
			break;
	}
}
uint8_t Dir;
extern uint8_t SupportStatusdata;
static void HomeFrame_cbDialog(WM_MESSAGE * pMsg) 
{
	uint32_t t0, t1;
	static uint8_t  frist;
	static uint8_t start;
	static uint8_t sige[6] = {0};
	static uint8_t sige1[6] = {0};
	static uint8_t beepsta;
	static uint8_t tage3,tage1,tage2;
	WM_HWIN hItem;
	int     NCode;
	int     Id;
	static  SingleStartUp  TempActoin;
	uint8_t buf[4] = {0,1,0,0};
	uint8_t buf1[4] = {1,1,0,0};
	switch (pMsg->MsgId) 
	{
		case WM_CREATE:
			SupportStatusdata = 0;
//			SendLEDNavigationData(0,KEY_ALL);
			CreateHomeFrameTextShow(pMsg);		
			USER_ReviseTextStyle(pMsg);	
			ShearerLeft_hWin = 0;
			DtrectionLeft_hWin = 0;
			ShearerRight_hWin = 0;
			DtrectionRight_hWin = 0;
			SetSinglePageValue(0);
			ChangeInfoButton(BackGround_hWin,3);
			WM_SetFocus(pMsg->hWin);
		break;
		case WM_TIMER:	
			if(pMsg->hWin != WM_GetFocussedWindow())
				WM_SetFocus(pMsg->hWin);
			
				switch(getFileRecvStatus())
				{
					case FILEDATA:
						if(frist == 0)
						{
							frist = 1;
							start = 1;
							TEXT_CreateEx(0,60,360,75,pMsg->hWin,WM_CF_SHOW,TEXT_CF_HCENTER|TEXT_CF_VCENTER,GUI_ID_TEXT21," ");
							TEXT_SetBkColor(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT21),GUI_BLACK);
							TEXT_SetTextColor(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT21),GUI_WHITE);
							TEXT_SetFont(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT21), &GUI_FontHZ32);
							TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT21),"正在接收程序，请勿动");
							TEXT_CreateEx(0,185,360,75,pMsg->hWin,WM_CF_SHOW,TEXT_CF_HCENTER|TEXT_CF_VCENTER,GUI_ID_TEXT22," ");
							TEXT_SetBkColor(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT22),GUI_BLACK);							
							PROGBAR_CreateEx(0,135,360,50,pMsg->hWin,WM_CF_SHOW,PROGBAR_CF_HORIZONTAL,GUI_ID_PROGBAR0);
							hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_PROGBAR0);
							PROGBAR_SetFont(hItem, &GUI_FontHZ24);
							PROGBAR_SetMinMax(hItem, 0, 100); 
							OvertimeClose();
							
						}						
						PROGBAR_SetValue(WM_GetDialogItem(pMsg->hWin, GUI_ID_PROGBAR0),UpdateTimerApi());
					break;
					case FILEREVSUCCESS:
						TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT21),"接收成功");
					
						WM_DeleteWindow(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT21));
						WM_DeleteWindow(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT22));
						WM_DeleteWindow(WM_GetDialogItem(pMsg->hWin, GUI_ID_PROGBAR0));
						setFileRecvStatus(0xFF);
						frist = 0;
						start = 0;
						OvertimeOpen();
						break;
					case FILEREVFAIL:
						TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT21),"接收失败");
					
						WM_DeleteWindow(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT21));
						WM_DeleteWindow(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT22));
						WM_DeleteWindow(WM_GetDialogItem(pMsg->hWin, GUI_ID_PROGBAR0));
						setFileRecvStatus(0xFF);
						frist = 0;
						start = 0;
						OvertimeOpen();
						break;
					default:
						break;
				}
			
			
			
			
			
			
			SupportStatusRefresh(pMsg);
			SupportNumberRefresh(pMsg);
			RunningDirectionRefresh(pMsg);
			DisplayItemRefresh(pMsg);
			WM_RestartTimer(HomeFrame_hTimer,100);
			break;
		case WM_KEY:
			if(start)
				break;
			if((((WM_KEY_INFO*)(pMsg->Data.p))->PressedCnt) > 0) //按键按下
			{
				ChangeInfoKey(BackGround_hWin,((WM_KEY_INFO*)(pMsg->Data.p))->Key,1);
				switch (((WM_KEY_INFO*)(pMsg->Data.p))->Key) 
				{ 
					case USER_KEY_UP:
						GUI_EndDialog(pMsg->hWin, 0);
						ChangeInfoText(CreateMasterContor(),0);	
						break;
					case USER_KEY_DOWN:
						GUI_EndDialog(pMsg->hWin, 0);
						ChangeInfoText(CreateMasterContor(),1);
						break;
					case USER_KEY_LEFT:
						GUI_EndDialog(pMsg->hWin, 0);
						ChangeInfoText(CreateGroupLeft(),0);
						ChangeInfoButton(BackGround_hWin,2);
						break;
					case USER_KEY_RIGHT:
						GUI_EndDialog(pMsg->hWin, 0);
						ChangeInfoText(CreateGroupLeft(),1);
						ChangeInfoButton(BackGround_hWin,2);					
						break;
					case USER_KEY_ASSIST:
						if(memcmp(sige,sige1,6) == 0)
							ChangeInfoButton(BackGround_hWin,0);
						break;
					case USER_KEY_MENU:	
						GUI_EndDialog(pMsg->hWin, 0);
						CreatePassWordInPut();
						break;
					case USER_KEY_LEFT1:
						if((1+(GetSinglePageValue()*6)) > ShowAuxiliaryAction[0].MAX)
							break;
						sige[0] = 1;
						TempActoin.Action = ShowAuxiliaryAction[((GetSinglePageValue()*6))];
						ActionControlOfThisRack(TempActoin,buf);
						break;
					case USER_KEY_LEFT2:
						if((3+(GetSinglePageValue()*6)) > ShowAuxiliaryAction[0].MAX)
							break;
						sige[1] = 1;
						TempActoin.Action = ShowAuxiliaryAction[2+((GetSinglePageValue()*6))];
						ActionControlOfThisRack(TempActoin,buf);
						break;	
					case USER_KEY_LEFT3:
						if((5+(GetSinglePageValue()*6)) > ShowAuxiliaryAction[0].MAX)
							break;
						sige[2] = 1;
						TempActoin.Action = ShowAuxiliaryAction[4+((GetSinglePageValue()*6))];
						ActionControlOfThisRack(TempActoin,buf);
						break;	
					case USER_KEY_RIGHT1:
						if((2+(GetSinglePageValue()*6)) > ShowAuxiliaryAction[0].MAX)
							break;
						sige[3] = 1;
						TempActoin.Action = ShowAuxiliaryAction[1+((GetSinglePageValue()*6))];
						ActionControlOfThisRack(TempActoin,buf);
						break;	
					case USER_KEY_RIGHT2:
						if((4+(GetSinglePageValue()*6)) > ShowAuxiliaryAction[0].MAX)
							break;
						sige[4] = 1;
						TempActoin.Action = ShowAuxiliaryAction[3+((GetSinglePageValue()*6))];
						ActionControlOfThisRack(TempActoin,buf);
						break;	
					case USER_KEY_RIGHT3:
						if((6+(GetSinglePageValue()*6)) > ShowAuxiliaryAction[0].MAX)
							break;
						sige[5] = 1;
						TempActoin.Action = ShowAuxiliaryAction[5+((GetSinglePageValue()*6))];
						ActionControlOfThisRack(TempActoin,buf);
						break;							
					case USER_KEY_2:
						TempActoin.Action = ShowKeyboardAction[0][0];
						ActionControlOfThisRack(TempActoin,buf);
						TempActoin.Action = ShowKeyboardAction[0][1];
						ActionControlOfThisRack(TempActoin,buf);	
						break;	
					case USER_KEY_3:
						TempActoin.Action = ShowKeyboardAction[1][0];
						ActionControlOfThisRack(TempActoin,buf);
						TempActoin.Action = ShowKeyboardAction[1][1];
						ActionControlOfThisRack(TempActoin,buf);	
						break;	
					case USER_KEY_5:
						TempActoin.Action = ShowKeyboardAction[2][0];
						ActionControlOfThisRack(TempActoin,buf);
						TempActoin.Action = ShowKeyboardAction[2][1];
						ActionControlOfThisRack(TempActoin,buf);	
						break;	
					case USER_KEY_6:
						TempActoin.Action = ShowKeyboardAction[3][0];
						ActionControlOfThisRack(TempActoin,buf);
						TempActoin.Action = ShowKeyboardAction[3][1];
						ActionControlOfThisRack(TempActoin,buf);	
						break;	
					case USER_KEY_8:
						TempActoin.Action = ShowKeyboardAction[4][0];
						ActionControlOfThisRack(TempActoin,buf);
						TempActoin.Action = ShowKeyboardAction[4][1];
						ActionControlOfThisRack(TempActoin,buf);	
						break;	
					case USER_KEY_9:
						TempActoin.Action = ShowKeyboardAction[5][0];
						ActionControlOfThisRack(TempActoin,buf);
						TempActoin.Action = ShowKeyboardAction[5][1];
						ActionControlOfThisRack(TempActoin,buf);	
						break;						
					default:
						break;			
				}
				
			}
			else //按键抬起
			{				
				ChangeInfoKey(BackGround_hWin,((WM_KEY_INFO*)(pMsg->Data.p))->Key,0);
				switch (((WM_KEY_INFO*)(pMsg->Data.p))->Key) 
				{ 
					case USER_KEY_WORK:	
						GUI_EndDialog(BackGround_hWin, 0);
						CreateStateTask();
						break;
					case USER_KEY_LEFT1:
						if((1+(GetSinglePageValue()*6)) > ShowAuxiliaryAction[0].MAX)
							break;
						sige[0] = 0;
						TempActoin.Action = ShowAuxiliaryAction[((GetSinglePageValue()*6))];
						ActionControlOfThisRack(TempActoin,buf1);
						break;
					case USER_KEY_LEFT2:
						if((3+(GetSinglePageValue()*6)) > ShowAuxiliaryAction[0].MAX)
							break;
						sige[1] = 0;
						TempActoin.Action = ShowAuxiliaryAction[2+((GetSinglePageValue()*6))];
						ActionControlOfThisRack(TempActoin,buf1);
						break;	
					case USER_KEY_LEFT3:
						if((5+(GetSinglePageValue()*6)) > ShowAuxiliaryAction[0].MAX)
							break;
						sige[2] = 0;
						TempActoin.Action = ShowAuxiliaryAction[4+((GetSinglePageValue()*6))];
						ActionControlOfThisRack(TempActoin,buf1);
						break;	
					case USER_KEY_RIGHT1:
						if((2+(GetSinglePageValue()*6)) > ShowAuxiliaryAction[0].MAX)
							break;
						sige[3] = 0;
						TempActoin.Action = ShowAuxiliaryAction[1+((GetSinglePageValue()*6))];
						ActionControlOfThisRack(TempActoin,buf1);
						break;	
					case USER_KEY_RIGHT2:
						if((4+(GetSinglePageValue()*6)) > ShowAuxiliaryAction[0].MAX)
							break;
						sige[4] = 0;
						TempActoin.Action = ShowAuxiliaryAction[3+((GetSinglePageValue()*6))];
						ActionControlOfThisRack(TempActoin,buf1);
						break;	
					case USER_KEY_RIGHT3:
						if((6+(GetSinglePageValue()*6)) > ShowAuxiliaryAction[0].MAX)
							break;
						sige[5] = 0;
						TempActoin.Action = ShowAuxiliaryAction[5+((GetSinglePageValue()*6))];
						ActionControlOfThisRack(TempActoin,buf1);
						break;							
					case USER_KEY_2:
						TempActoin.Action = ShowKeyboardAction[0][0];
						ActionControlOfThisRack(TempActoin,buf1);
						TempActoin.Action = ShowKeyboardAction[0][1];
						ActionControlOfThisRack(TempActoin,buf1);	
						break;	
					case USER_KEY_3:
						TempActoin.Action = ShowKeyboardAction[1][0];
						ActionControlOfThisRack(TempActoin,buf1);
						TempActoin.Action = ShowKeyboardAction[1][1];
						ActionControlOfThisRack(TempActoin,buf1);	
						break;	
					case USER_KEY_5:
						TempActoin.Action = ShowKeyboardAction[2][0];
						ActionControlOfThisRack(TempActoin,buf1);
						TempActoin.Action = ShowKeyboardAction[2][1];
						ActionControlOfThisRack(TempActoin,buf1);	
						break;	
					case USER_KEY_6:
						TempActoin.Action = ShowKeyboardAction[3][0];
						ActionControlOfThisRack(TempActoin,buf1);
						TempActoin.Action = ShowKeyboardAction[3][1];
						ActionControlOfThisRack(TempActoin,buf1);	
						break;	
					case USER_KEY_8:
						TempActoin.Action = ShowKeyboardAction[4][0];
						ActionControlOfThisRack(TempActoin,buf1);
						TempActoin.Action = ShowKeyboardAction[4][1];
						ActionControlOfThisRack(TempActoin,buf1);	
						break;	
					case USER_KEY_9:
						TempActoin.Action = ShowKeyboardAction[5][0];
						ActionControlOfThisRack(TempActoin,buf1);
						TempActoin.Action = ShowKeyboardAction[5][1];
						ActionControlOfThisRack(TempActoin,buf1);	
						break;	
					default:
						break;	
				}
				
			}
			break;

		case WM_PAINT:
			t0 = GUI_GetTime();
			GUI_MEMDEV_WriteAt(HomeFrame_DEV[3], 60, 213);
			GUI_MEMDEV_WriteAt(HomeFrame_DEV[0], 212, 7);
			GUI_MEMDEV_WriteAt(HomeFrame_DEV[1], 65, 50);
			GUI_MEMDEV_WriteAt(HomeFrame_DEV[2], 200, 60);
			GUI_MEMDEV_WriteAt(HomeFrame_DEV[4], 69, 250);
			GUI_MEMDEV_WriteAt(HomeFrame_DEV[4], 69, 285);		
			t1 = GUI_GetTime() - t0;
			break;
		default:
			WM_DefaultProc(pMsg);
			break;
	}
}
static void Signal_cbDialog(WM_MESSAGE * pMsg) 
{
	static uint8_t beepsta;
	static uint16_t lastdata;
	uint16_t length,pdata;
	switch (pMsg->MsgId) 
	{
		case WM_TIMER:
			GUIGetDateFunAPI(API_Signal_Intensity,0,&pdata,&length);
			if(pdata != lastdata)
			{
				beepsta = pdata;
				lastdata = pdata;
				WM_InvalidateWindow(Signal_hWin);
			}
			WM_RestartTimer(Signal_hTimer,500);
			break;
		case WM_PAINT:
			if(beepsta == 1)
				GUI_MEMDEV_WriteAt(Signal_DEV1, 70, 17);
			else if(beepsta == 2)
				GUI_MEMDEV_WriteAt(Signal_DEV2, 70, 17);
			else if(beepsta == 3)
				GUI_MEMDEV_WriteAt(Signal_DEV3, 70, 17);
			else
				;
			break;
		default:
			WM_DefaultProc(pMsg);
			break;
	}
}

static void PassWord_cbDialog(WM_MESSAGE * pMsg) 
{
	static uint8_t beepsta;
	uint32_t t0, t1;
	WM_HWIN hItem;
	int     NCode;
	int     Id;	
	static uint16_t  pass ,count;
	switch (pMsg->MsgId) 
	{
		case WM_CREATE:
			TEXT_CreateEx(25, 125, 330, 50,pMsg->hWin,WM_CF_SHOW,TEXT_CF_HCENTER|TEXT_CF_VCENTER,GUI_ID_TEXT30," ");
			hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT30);	
			TEXT_SetFont(hItem, &GUI_FontHZ32);
			TEXT_SetText(hItem, "输入密码");
			TEXT_SetTextColor(hItem,GUI_LIGHTBLUE);
			TEXT_SetTextAlign(hItem,GUI_TA_LEFT | GUI_TA_VCENTER);
			count = 0;
			pass = 0;
			WM_SetFocus(pMsg->hWin);
			break;
		case WM_TIMER:
			if(pMsg->hWin != WM_GetFocussedWindow())
				WM_SetFocus(pMsg->hWin);
			WM_RestartTimer(PassWord_hTimer,500);
			break;	
		case WM_KEY:
			if((((WM_KEY_INFO*)(pMsg->Data.p))->PressedCnt) > 0) //按键按下
			{
				switch (((WM_KEY_INFO*)(pMsg->Data.p))->Key) 
				{ 
					case USER_KEY_ESC:
						GUI_EndDialog(pMsg->hWin, 0);
						CreateHomeFrame();				
						break;
					case USER_KEY_RIGHT1:	
					case USER_KEY_RIGHT2:	
					case USER_KEY_RIGHT3:	
					case USER_KEY_2:	
					case USER_KEY_3:	
					case USER_KEY_5:	
					case USER_KEY_6:	
					case USER_KEY_8:	
					case USER_KEY_9:	
					case USER_KEY_ASSIST:
					pass = pass*10+	DigitKeyToChr(((WM_KEY_INFO*)(pMsg->Data.p))->Key);
					count++;
					break;
					
					default:
						break;			
				}
				if(count == 1)
				{
					TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT30), "*");
				}
				else if(count == 2)
				{
					TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT30), "**");
				}
				else if(count == 3)
				{
					TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT30), "***");
				}
				else if(count == 4)	//四位数开始判断输入的密码对不对				
				{
					if(pass == 1111)
					{
						GUI_EndDialog(BackGround_hWin, 0);
						CreateMenuTask();
					}
					else if(pass == 2222)
					{
						GUI_EndDialog(BackGround_hWin, 0);
						CreateDeveloperTask();
					}
					else						
					{
						GUI_EndDialog(pMsg->hWin, 0);
						CreateHomeFrame();	
					}
				}
				else
					;
			}	
		case WM_PAINT:		
			t0 = GUI_GetTime();
			GUI_MEMDEV_WriteAt(PowerOn_DEV, 109, 25);
			t1 = GUI_GetTime() - t0;
			GUI_MEMDEV_WriteAt(PassWord_DEV[0], 60+15, 125);
			t1 = GUI_GetTime() - t0;
			GUI_MEMDEV_WriteAt(PassWord_DEV[1], 60+49, 285);
			t1 = GUI_GetTime() - t0;
			break;
		default:
			WM_DefaultProc(pMsg);
			break;
	}
}

/*
*********************************************************************************************************
*	函 数 名: 
*	功能说明: 创建输入密码画面
*	形    参: 
*	返 回 值: 
*********************************************************************************************************
*/
WM_HWIN CreatePassWordInPut(void)
{
	PassWord_hWin = WM_CreateWindowAsChild(60,0,360,320,BackGround_hWin,WM_CF_SHOW|WM_CF_HASTRANS,PassWord_cbDialog,0);
	PassWord_hTimer = WM_CreateTimer(WM_GetClientWindow(PassWord_hWin), 0, 500, 0);
}
/*
*********************************************************************************************************
*	函 数 名: 
*	功能说明: 创建急停画面
*	形    参: 
*	返 回 值: 
*********************************************************************************************************
*/
WM_HWIN CreateSuddenStop(void)
{
	SuddenStop_hWin = WM_CreateWindowAsChild(0,50,360,163,HomeFrame_hWin,WM_CF_SHOW|WM_CF_HASTRANS,SuddenStop_cbDialog,0);
	SuddenStop_hTimer = WM_CreateTimer(WM_GetClientWindow(SuddenStop_hWin), 0, 100, 0);
}

/*
*********************************************************************************************************
*	函 数 名: 
*	功能说明: 创建向右运行画面
*	形    参: 
*	返 回 值: 
*********************************************************************************************************
*/
uint8_t MoveFramedata;
WM_HWIN CreateMoveFrame_Right(void) 
{
	ShearerRight_hWin = WM_CreateWindowAsChild(0,101,360,104,HomeFrame_hWin,WM_CF_SHOW|WM_CF_HASTRANS,ShearerRight_cbDialog,0);
	
	DtrectionRight_hWin = WM_CreateWindowAsChild(0,55,360,45,HomeFrame_hWin,WM_CF_SHOW|WM_CF_HASTRANS,DtrectionRight_cbDialog,0);
	
	ShearerRight_hTimer = WM_CreateTimer(WM_GetClientWindow(ShearerRight_hWin), 0, 100, 0);

	DtrectionRight_hTimer = WM_CreateTimer(WM_GetClientWindow(DtrectionRight_hWin), 0, 100, 0);

	
	GUI_EndDialog(ShearerLeft_hWin, 0);
	GUI_EndDialog(DtrectionLeft_hWin, 0);
	
	MoveFramedata = 1;
	return ShearerRight_hWin;
}

/*
*********************************************************************************************************
*	函 数 名: 
*	功能说明: 创建向左运行画面
*	形    参: 
*	返 回 值: 
*********************************************************************************************************
*/

WM_HWIN CreateMoveFrame_Left(void) 
{
	ShearerLeft_hWin = WM_CreateWindowAsChild(0,101,360,104,HomeFrame_hWin,WM_CF_SHOW|WM_CF_HASTRANS,ShearerLeft_cbDialog,0);
	
	DtrectionLeft_hWin = WM_CreateWindowAsChild(0,55,360,45,HomeFrame_hWin,WM_CF_SHOW|WM_CF_HASTRANS,DtrectionLeft_cbDialog,0);

	ShearerLeft_hTimer = WM_CreateTimer(WM_GetClientWindow(ShearerLeft_hWin), 0, 100, 0);

	DtrectionLeft_hTimer = WM_CreateTimer(WM_GetClientWindow(DtrectionLeft_hWin), 0, 100, 0);

	GUI_EndDialog(ShearerRight_hWin, 0);
	GUI_EndDialog(DtrectionRight_hWin, 0);
	MoveFramedata = 2;
	return ShearerLeft_hWin;
}
/*
*********************************************************************************************************
*	函 数 名: 
*	功能说明: 创建运输机画面
*	形    参: 
*	返 回 值: 
*********************************************************************************************************
*/
WM_HWIN CreateTranSport(void) 
{		
	TranSport_hWin =  WM_CreateWindowAsChild(0,206,360,4,HomeFrame_hWin,WM_CF_SHOW|WM_CF_HASTRANS,TranSport_cbDialog,0);
	
	TranSport_hTimer = WM_CreateTimer(WM_GetClientWindow(TranSport_hWin), 0, 1000, 0);

	return TranSport_hWin;
}
/*
*********************************************************************************************************
*	函 数 名: 
*	功能说明: 创建信号画面
*	形    参: 
*	返 回 值: 
*********************************************************************************************************
*/
WM_HWIN CreateSignal(void) 
{		
	Signal_hWin = WM_CreateWindowAsChild(10,17,32,23,HomeFrame_hWin,WM_CF_SHOW|WM_CF_HASTRANS,Signal_cbDialog,0);
	
	Signal_hTimer = WM_CreateTimer(WM_GetClientWindow(Signal_hWin), 0, 50, 0);

	return Signal_hWin;
}
/*
*********************************************************************************************************
*	函 数 名: 
*	功能说明: 创建主界面画面
*	形    参: 
*	返 回 值: 
*********************************************************************************************************
*/
WM_HWIN CreateHomeFrame(void) 
{
	HomeFrame_hWin = WM_CreateWindowAsChild(60,0,360,320,BackGround_hWin,WM_CF_SHOW|WM_CF_HASTRANS,HomeFrame_cbDialog,0);
	CreateSignal();
	CreateTranSport();
	CreateMoveFrame_Left();
	HomeFrame_hTimer = WM_CreateTimer(WM_GetClientWindow(HomeFrame_hWin), 0, 0, 0);
	return HomeFrame_hWin;
}

/*
*********************************************************************************************************
*	函 数 名: 
*	功能说明: 创建背景画面
*	形    参: 
*	返 回 值: 
*********************************************************************************************************
*/
extern OS_EVENT *s_stLEDRecvd ;
WM_HWIN CreateBackGround(void) 
{		
	BackGround_hWin = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), BackGround_cbDialog, WM_HBKWIN, 0, 0);
	BackGround_hTimer = WM_CreateTimer(WM_GetClientWindow(BackGround_hWin), 0, 0, 0);
	return BackGround_hWin;
}
//PNG图片显示例程
void AppTaskGUI(void)
{
	INT8U err;
	GUI_Init();  			//STemWin初始化
    WM_SetCreateFlags(WM_CF_MEMDEV); //启动所有窗口的存储设备
//	WM_MULTIBUF_Enable(1);  //开启STemWin多缓冲,RGB屏可能会用到
	Boot_Sceen_Create();
//	CreateDeveloperTask();
	while(1)
	{
		OSSemPend(s_stLEDRecvd, 100, &err);
		if(err == OS_ERR_NONE)
		{
			if(KEYRecvUsartDataProc() == 0)
			{
				GUI_Delay(50);
			}			
		}
		else if(err == OS_ERR_TIMEOUT)
		{
			GUI_Delay(50);
		}
	}
}
