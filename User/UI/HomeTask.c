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
#include "ProjectConfig.h"
#include "User_GUI.h"
#include "usartapp.h"
#include "includes.h"					//ucos 使用	  
#include "FileLoad.h"
#include "sys_msg_def.h"
#include "FileUpdate.h"
#include "udp_app_proc.h"
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


extern uint8_t  CurrentWindow;
uint8_t MoveFramedata;
static const char * Recoil[] = {"启动" ," " ,"停止"," ",""," "} ;
static const char * Empty[] = {""," ",""," ",""," "} ;
static const char * PassButton[] = {"","1","","4","","7"} ;
uint16_t  FollowButton[6] = {0} ;
extern u8  NumberOfOperations;
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
					PowerOn_DEV1,
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
			PassWord_hTimer,
			Warning_hTimer;
			
			
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
		PassWord_hWin,
		Warning_hWin,
		Warningtwo_hWin;
		
		
static void Warningtwo_cbDialog(WM_MESSAGE * pMsg) 
{
	switch (pMsg->MsgId) 
	{
		case WM_PAINT:
			GUI_MEMDEV_WriteAt(SuddenStop_DEV, 60, 50);
		
			break;
		default:
			WM_DefaultProc(pMsg);
			break;
	}
}	
static void Warning_cbDialog(WM_MESSAGE * pMsg) 
{
	switch (pMsg->MsgId) 
	{
		case WM_PAINT:
			GUI_MEMDEV_WriteAt(SuddenStop_DEV, 60, 50);
		
			break;
		default:
			WM_DefaultProc(pMsg);
			break;
	}
}		
		
static void SuddenStop_cbDialog(WM_MESSAGE * pMsg) 
{
	switch (pMsg->MsgId) 
	{
		case WM_CREATE:
			CreateSuddenStopTextShow(pMsg);	
			SupportStopRefresh(pMsg);
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
				 #if (MAIN_INTERFACE_ANIMATION == 1)
					GUI_MEMDEV_WriteAt(DtrectionLeft_DEV0, 85, 60);
				#else
					GUI_MEMDEV_WriteAt(DtrectionLeft_DEV3, 85, 60);
				#endif
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
				#if (MAIN_INTERFACE_ANIMATION == 1)
				  GUI_MEMDEV_WriteAt(DtrectionRight_DEV0, 85, 60);//    jhy 20220726   DtrectionRight_DEV0
				#else
					GUI_MEMDEV_WriteAt(DtrectionRight_DEV3, 85, 60);
				#endif
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
extern WM_HWIN 	MasterContor_hWin;
extern NameAndId  SingleActionBUF[];
extern NameAndId  GroupActionBUF[];
extern NameAndId   ShowGroupAction[] ;
extern NameAndId  ShowAuxiliaryAction[];
extern NameAndId  ShowKeyboardAction[][2];
static void BackGround_cbDialog(WM_MESSAGE * pMsg) 
{
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
				USER_ReviseSingleActionButtonShow(pMsg,ShowGroupAction,1);
			}
			else if(pMsg ->Data.v  == 2)//成组
			{
				USER_ReviseSingleActionButtonShow(pMsg,ShowGroupAction,1);
			}
			else if(pMsg ->Data.v  == 3)//刷新
			{
				USER_ReviseSingleActionButtonShow(pMsg,ShowAuxiliaryAction,0);
			}
			else if(pMsg ->Data.v  == 4)//密码输入
			{
				USER_ReviseOnePageButtonShow(pMsg,PassButton);
			}
			else if(pMsg ->Data.v  == 5)//跟机输入工序号
			{
				USER_ReviseOnePageButtonShow(pMsg,Empty);
				USER_RevisefollowPageButtonShow(pMsg,FollowButton);
			}
			else if(pMsg ->Data.v  == 6)//自动反冲洗
			{
				USER_ReviseOnePageButtonShow(pMsg,Recoil);
			}
			WM_InvalidateWindow(pMsg->hWin);
		break;
		case WM_TIMER:	
			CreateHomeFrame();
			break;
		case WM_PAINT:
			GUI_MEMDEV_WriteAt(BackGround_DEV, 0, 0);
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
	globalStateType  temp;
	uint8_t follow[5];
	uint16_t direction;
	uint8_t pstate = 1;
	static uint8_t FollowCon;
	static uint8_t frist;
	static uint8_t start;
	static uint8_t sige[6] = {0};
	static uint8_t sige1[6] = {0};
	static uint8_t timer,timer1,timer3,sever;
	static  SingleStartUp  TempActoin;
	static uint8_t slaveModetimers = 0;
	char buf[20] ;
	
	switch (pMsg->MsgId) 
	{
		case WM_CREATE:
			frist = 0;
			start = 0;
			timer1 = 0;
			timer3 = 0;
			sever = 0;
			timer = 0;
			FollowCon = 0;
			SupportStatusdata = 0;
			SendLEDNavigationData(0,KEY_ALL);
			CreateHomeFrameTextShow(pMsg);		
			USER_ReviseTextStyle(pMsg);	
			ShearerLeft_hWin = 0;
			DtrectionLeft_hWin = 0;
			ShearerRight_hWin = 0;
			DtrectionRight_hWin = 0;
			SetSinglePageValue(0);
			ChangeInfoButton(BackGround_hWin,3);
			CurrentDataRefresh(pMsg);
			StatusRefresh(pMsg,0);
			SupportNumberRefresh(pMsg,0);
			DisplayItemRefresh(pMsg,0);
			TransportPlaneRefresh(pMsg);
			memset(sige,0,6);
			memset(sige1,0,6);
			SetSysFuncMode(SYSMODE_NORMAL);
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
						///接收程序时先暂停急停闭锁显示，否则不能显示接收文件进度
						if(SupportStatusdata == 1)
						{
							SupportStatusdata = 0;
							GUI_EndDialog(SuddenStop_hWin, 0);
						}						
						GUI_Delay (10);
						
						
						CreateWarning();
						TEXT_CreateEx(0,105,360,50,pMsg->hWin,WM_CF_SHOW,TEXT_CF_HCENTER|TEXT_CF_VCENTER,GUI_ID_TEXT22," ");
						TEXT_SetTextColor(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT22),GUI_WHITE);
						TEXT_SetFont(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT22), &GUI_FontHZ32);
					}	
					memset(buf,0,20);
					sprintf(buf,"正在接收文件:%d%%",UpdateTimerApi());
					TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT22),buf);
					goto loop;
				break;
				case FILESAVING:
					memset(buf,0,20);
					sprintf(buf,"正在保存文件:%d%%",UpdateTimerApi());
					TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT22),buf);
					goto loop;				
					break;
				case FILESENDING:
					memset(buf,0,20);
					sprintf(buf,"正在发送文件:%d%%",sendTimerAPI());
					TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT22),buf);	
					goto loop;				
					break;
				case FILEREVSUCCESS:
					
					TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT22),"保存成功");		
					if(frist == 1)
					{
						GUI_EndDialog(Warning_hWin, 0);
						WM_DeleteWindow(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT22));
					}
					setFileRecvStatus(0xFF);
					frist = 0;
					start = 0;
					goto loop;		
					break;
				case FILEREVFAIL:
					TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT22),"接收失败");			
					if(frist == 1)
					{
						GUI_EndDialog(Warning_hWin, 0);
						WM_DeleteWindow(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT22));
					}
					setFileRecvStatus(0xFF);
					frist = 0;
					start = 0;
					goto loop;		
					break;
				default:
					break;
			}
			GUIGetDateFunAPI(API_Support_Statur,0,&temp,0);
			if(temp.slaveMode == 2)
			{
				if(frist == 0)
				{
					slaveModetimers = 0;
					frist = 1;
					
					///配址时先暂停急停闭锁显示，否则不能显示配址
					if(SupportStatusdata == 1)
					{
						SupportStatusdata = 0;
						GUI_EndDialog(SuddenStop_hWin, 0);
					}						
					GUI_Delay (10);
					
					CreateWarning();
					TEXT_CreateEx(0,105,360,50,pMsg->hWin,WM_CF_SHOW,TEXT_CF_HCENTER|TEXT_CF_VCENTER,GUI_ID_TEXT23," ");
					TEXT_SetTextColor(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT23),GUI_WHITE);
					TEXT_SetFont(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT23), &GUI_FontHZ32);
					TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT23),"正在配址，请勿动");
					start = 1;					
				}
				else
				{
					slaveModetimers ++;
					if(slaveModetimers > 60)
					{
						SetSysFuncMode(SYSMODE_CONFIG_FIN);
						slaveModetimers = 0;				
					}
				}
			}
			else if(temp.slaveMode == 3)
			{
				if(frist == 1)
				{
					GUI_EndDialog(Warning_hWin, 0);
					WM_DeleteWindow(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT23));
				}
				SetSysFuncMode(SYSMODE_NORMAL);
				start = 0;
				frist = 0;
			}
			else
			{
				if(getFileRecvStatus() == 0xFF)
				{
					if(FollowCon)
					{
						timer++;
						if((NumberOfOperations != 0) && (NumberOfOperations != 0xFF))
						{
							if(FollowCon == 1)
							{											
								GUI_EndDialog(pMsg->hWin, 0);
								ChangeInfoFollow(CreateFollowContor(),0);
							}
							else if(FollowCon == 2)
							{													
								GUI_EndDialog(pMsg->hWin, 0);
								ChangeInfoFollow(CreateFollowContor(),1);
							}
							FollowCon = 0;
							timer = 0;
						}
						else
						{
							if(timer > 4)
							{
								FollowCon = 0;
								if(!SupportStatusdata)
								{
									sever = 1;
									timer = 0;
									CreateWarning();
									TEXT_CreateEx(0,105,360,50,pMsg->hWin,WM_CF_SHOW,TEXT_CF_HCENTER|TEXT_CF_VCENTER,GUI_ID_TEXT24," ");
									TEXT_SetTextColor(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT24),GUI_WHITE);
									TEXT_SetFont(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT24), &GUI_FontHZ32);
									if(NumberOfOperations == 0xFF)
										TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT24),"此位置无法开启跟机");
									else
										TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT24),"远端网络通信异常");//连接服务器失败
								}
							}
						}
						goto loop;	
					}
					if(sever)
					{
						timer1++;
						if(timer1 > 6)
						{
							GUI_EndDialog(Warning_hWin, 0);
							WM_DeleteWindow(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT24));	
							timer1 = 0;
							sever = 0;
						}
						goto loop;	
					}
					
					StatusRefresh(pMsg,1);
					SupportStatusRefresh(pMsg->hWin,1);
					RunningDirectionRefresh(pMsg);
					TransportPlaneRefresh(pMsg);
					DisplayItemRefresh(pMsg,1);
					if(GetAuxiliarySige())
					{
						ChangeInfoButton(BackGround_hWin,3);
						ResetAuxiliarySigereturn();
					}
				}
			}
			loop:
			if(timer3>10)
			{
				CurrentDataRefresh(pMsg);
				timer3 = 0;
			}
			timer3++;
			SupportNumberRefresh(pMsg,1);
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
					case USER_KEY_FOLLOW_LEFT:
						NumberOfOperations = 0;
						follow[0] = 0;
						follow[1] = 0;
						follow[2] = 0;
						follow[3] = 0;
						follow[4] = 1;
						UdpLinkLayerSend_app(follow,5,0,0 ,0,AppProcIDType_followControl_request);
						FollowCon = 1;
					break;
					case USER_KEY_FOLLOW_RIGHT:						
						NumberOfOperations = 0;
						follow[0] = 0;
						follow[1] = 0;
						follow[2] = 0;
						follow[3] = 0;
						follow[4] = 2;
						UdpLinkLayerSend_app(follow,5,0,0 ,0,AppProcIDType_followControl_request);
						FollowCon = 2;
					break;	
					case USER_KEY_RECOIL:						
						GUI_EndDialog(pMsg->hWin, 0);
						CreateRecoilContor();//反冲洗
					break;	
					case USER_KEY_ESC:
						SysDataFun(DATA_MSG_SetStopPress,ENUM_SYSTEM,0,0,0,&pstate,0);
						break;
					case USER_KEY_UP:
						GUI_EndDialog(pMsg->hWin, 0);
						ChangeInfoText(CreateMasterContor(),0);	
						break;
					case USER_KEY_DOWN:
						GUI_EndDialog(pMsg->hWin, 0);
						ChangeInfoText(CreateMasterContor(),1);
						break;
					case USER_KEY_LEFT:
						if(getJsonParaChildSize(0x600000000000000) == 1)
							break;
						else 
						{
							GUI_EndDialog(pMsg->hWin, 0);
							ChangeInfoText(CreateGroupLeft(),0);
							ChangeInfoButton(BackGround_hWin,2);
						}
						break;
					case USER_KEY_RIGHT:
						if(getJsonParaChildSize(0x600000000000000) == 1)
							break;
						else 
						{
							GUI_EndDialog(pMsg->hWin, 0);
							ChangeInfoText(CreateGroupLeft(),1);
							ChangeInfoButton(BackGround_hWin,2);					
							break;
						}
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
						ActionControlOfThisHome(TempActoin,0);
						break;
					case USER_KEY_LEFT2:
						if((3+(GetSinglePageValue()*6)) > ShowAuxiliaryAction[0].MAX)
							break;
						sige[1] = 1;
						TempActoin.Action = ShowAuxiliaryAction[2+((GetSinglePageValue()*6))];
						ActionControlOfThisHome(TempActoin,0);
						break;	
					case USER_KEY_LEFT3:
						if((5+(GetSinglePageValue()*6)) > ShowAuxiliaryAction[0].MAX)
							break;
						sige[2] = 1;
						TempActoin.Action = ShowAuxiliaryAction[4+((GetSinglePageValue()*6))];
						ActionControlOfThisHome(TempActoin,0);
						break;	
					case USER_KEY_RIGHT1:
						if((2+(GetSinglePageValue()*6)) > ShowAuxiliaryAction[0].MAX)
							break;
						sige[3] = 1;
						TempActoin.Action = ShowAuxiliaryAction[1+((GetSinglePageValue()*6))];
						ActionControlOfThisHome(TempActoin,0);
						break;	
					case USER_KEY_RIGHT2:
						if((4+(GetSinglePageValue()*6)) > ShowAuxiliaryAction[0].MAX)
							break;
						sige[4] = 1;
						TempActoin.Action = ShowAuxiliaryAction[3+((GetSinglePageValue()*6))];
						ActionControlOfThisHome(TempActoin,0);
						break;	
					case USER_KEY_RIGHT3:
						if((6+(GetSinglePageValue()*6)) > ShowAuxiliaryAction[0].MAX)
							break;
						sige[5] = 1;
						TempActoin.Action = ShowAuxiliaryAction[5+((GetSinglePageValue()*6))];
						ActionControlOfThisHome(TempActoin,0);
						break;							
					case USER_KEY_2:
						TempActoin.Action = ShowKeyboardAction[0][0];
						ActionControlOfThisHome(TempActoin,0);
						TempActoin.Action = ShowKeyboardAction[0][1];
						ActionControlOfThisHome(TempActoin,0);	
						break;	
					case USER_KEY_3:
						TempActoin.Action = ShowKeyboardAction[1][0];
						ActionControlOfThisHome(TempActoin,0);
						TempActoin.Action = ShowKeyboardAction[1][1];
						ActionControlOfThisHome(TempActoin,0);	
						break;	
					case USER_KEY_5:
						TempActoin.Action = ShowKeyboardAction[2][0];
						ActionControlOfThisHome(TempActoin,0);
						TempActoin.Action = ShowKeyboardAction[2][1];
						ActionControlOfThisHome(TempActoin,0);	
						break;	
					case USER_KEY_6:
						TempActoin.Action = ShowKeyboardAction[3][0];
						ActionControlOfThisHome(TempActoin,0);
						TempActoin.Action = ShowKeyboardAction[3][1];
						ActionControlOfThisHome(TempActoin,0);	
						break;	
					case USER_KEY_8:
						TempActoin.Action = ShowKeyboardAction[4][0];
						ActionControlOfThisHome(TempActoin,0);
						TempActoin.Action = ShowKeyboardAction[4][1];
						ActionControlOfThisHome(TempActoin,0);	
						break;	
					case USER_KEY_9:
						TempActoin.Action = ShowKeyboardAction[5][0];
						ActionControlOfThisHome(TempActoin,0);
						TempActoin.Action = ShowKeyboardAction[5][1];
						ActionControlOfThisHome(TempActoin,0);	
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
						ActionControlOfThisHome(TempActoin,1);
						break;
					case USER_KEY_LEFT2:
						if((3+(GetSinglePageValue()*6)) > ShowAuxiliaryAction[0].MAX)
							break;
						sige[1] = 0;
						TempActoin.Action = ShowAuxiliaryAction[2+((GetSinglePageValue()*6))];
						ActionControlOfThisHome(TempActoin,1);
						break;	
					case USER_KEY_LEFT3:
						if((5+(GetSinglePageValue()*6)) > ShowAuxiliaryAction[0].MAX)
							break;
						sige[2] = 0;
						TempActoin.Action = ShowAuxiliaryAction[4+((GetSinglePageValue()*6))];
						ActionControlOfThisHome(TempActoin,1);
						break;	
					case USER_KEY_RIGHT1:
						if((2+(GetSinglePageValue()*6)) > ShowAuxiliaryAction[0].MAX)
							break;
						sige[3] = 0;
						TempActoin.Action = ShowAuxiliaryAction[1+((GetSinglePageValue()*6))];
						ActionControlOfThisHome(TempActoin,1);
						break;	
					case USER_KEY_RIGHT2:
						if((4+(GetSinglePageValue()*6)) > ShowAuxiliaryAction[0].MAX)
							break;
						sige[4] = 0;
						TempActoin.Action = ShowAuxiliaryAction[3+((GetSinglePageValue()*6))];
						ActionControlOfThisHome(TempActoin,1);
						break;	
					case USER_KEY_RIGHT3:
						if((6+(GetSinglePageValue()*6)) > ShowAuxiliaryAction[0].MAX)
							break;
						sige[5] = 0;
						TempActoin.Action = ShowAuxiliaryAction[5+((GetSinglePageValue()*6))];
						ActionControlOfThisHome(TempActoin,1);
						break;							
					case USER_KEY_2:
						TempActoin.Action = ShowKeyboardAction[0][0];
						ActionControlOfThisHome(TempActoin,1);
						TempActoin.Action = ShowKeyboardAction[0][1];
						ActionControlOfThisHome(TempActoin,1);	
						break;	
					case USER_KEY_3:
						TempActoin.Action = ShowKeyboardAction[1][0];
						ActionControlOfThisHome(TempActoin,1);
						TempActoin.Action = ShowKeyboardAction[1][1];
						ActionControlOfThisHome(TempActoin,1);	
						break;	
					case USER_KEY_5:
						TempActoin.Action = ShowKeyboardAction[2][0];
						ActionControlOfThisHome(TempActoin,1);
						TempActoin.Action = ShowKeyboardAction[2][1];
						ActionControlOfThisHome(TempActoin,1);	
						break;	
					case USER_KEY_6:
						TempActoin.Action = ShowKeyboardAction[3][0];
						ActionControlOfThisHome(TempActoin,1);
						TempActoin.Action = ShowKeyboardAction[3][1];
						ActionControlOfThisHome(TempActoin,1);	
						break;	
					case USER_KEY_8:
						TempActoin.Action = ShowKeyboardAction[4][0];
						ActionControlOfThisHome(TempActoin,1);
						TempActoin.Action = ShowKeyboardAction[4][1];
						ActionControlOfThisHome(TempActoin,1);	
						break;	
					case USER_KEY_9:
						TempActoin.Action = ShowKeyboardAction[5][0];
						ActionControlOfThisHome(TempActoin,1);
						TempActoin.Action = ShowKeyboardAction[5][1];
						ActionControlOfThisHome(TempActoin,1);	
						break;	
					default:
						break;	
				}
				
			}
			break;

		case WM_PAINT:
			GUI_MEMDEV_WriteAt(HomeFrame_DEV[3], 60, 213);
			GUI_MEMDEV_WriteAt(HomeFrame_DEV[0], 212, 7);
			GUI_MEMDEV_WriteAt(HomeFrame_DEV[1], 65, 50);
			GUI_MEMDEV_WriteAt(HomeFrame_DEV[2], 200, 60);
			GUI_MEMDEV_WriteAt(HomeFrame_DEV[4], 69, 250);
			GUI_MEMDEV_WriteAt(HomeFrame_DEV[4], 69, 285);	
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
uint8_t Super_advanced_password;
Displayrefresh   Serviceparameters;
static void PassWord_cbDialog(WM_MESSAGE * pMsg) 
{
	WM_HWIN hItem;
	uint8_t temp[9];
	static uint8_t timer,sever;
	static uint16_t  pass ,count = 0;;
	switch (pMsg->MsgId) 
	{
		case WM_CREATE:
			sever = 0;
			timer = 0;
			Super_advanced_password = 0;
			memset(&Serviceparameters,0,sizeof(Serviceparameters));
			SendLEDNavigationData(KEY_PASS,KEY_ALL);
			OvertimeOpen();
			ChangeInfoButton(BackGround_hWin,4);
			Setstheinitialstate();
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
			OvertimeReturn(pMsg);
			Determineneedtoreturninterface(pMsg,1);
			if(sever)
			{
				timer++;
				if((Serviceparameters.ButtonDisplay.count)&&(Serviceparameters.ListviewDisplaye.count)&&(Serviceparameters.TextDisplay.count))
				{
					sever = 0;
					timer = 0;
					GUI_EndDialog(BackGround_hWin, 0);
					CreateServerMenuTask();
				}
				else
				{
					if(Serviceparameters.TextDisplay.count == 0)
					{
						memset(temp,0,9);
						temp[8] = 1;
						UdpLinkLayerSend_app(temp,9,0,0 ,0,AppProcIDType_serverDisplays_request);
					}
					if(Serviceparameters.ButtonDisplay.count == 0)
					{
						memset(temp,0,9);
						temp[8] = 2;
						UdpLinkLayerSend_app(temp,9,0,0 ,0,AppProcIDType_serverDisplays_request);
					}
					if(Serviceparameters.ListviewDisplaye.count == 0)
					{
						memset(temp,0,9);
						temp[8] = 3;
						UdpLinkLayerSend_app(temp,9,0,0 ,0,AppProcIDType_serverDisplays_request);
					}					
					if(timer > 4)
					{
						sever = 0;
						timer = 0;
						pass = 0;
						count = 0;
						TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT30), "远端网络通信异常");  //连接服务器失败
					}
				}
			}				
			WM_RestartTimer(PassWord_hTimer,100);
			break;	
		case WM_KEY:
			SendLEDNavigationData(KEY_PASS,KEY_ALL);
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
					case USER_KEY_WORK:
						if(count != 0)
						{
							pass = pass/10;
							count--;
						}
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
					if(pass == 6222)
					{
						Super_advanced_password = 1;
						GUI_EndDialog(BackGround_hWin, 0);
						CreateMenuTask();
					}
					else if(pass == SysParam.globalParam.ServerParam.ParameterPassword)
					{
						GUI_EndDialog(BackGround_hWin, 0);
						CreateMenuTask();
					}
					else if(pass == SysParam.globalParam.ServerParam.DeveloperPassword)
					{
						GUI_EndDialog(BackGround_hWin, 0);
						CreateDeveloperTask();
					}
					else if(pass == SysParam.globalParam.ServerParam.ServerPassword)
					{
						sever = 1;
						memset(&Serviceparameters,0,sizeof(Serviceparameters));
						memset(temp,0,9);
						temp[8] = 1;
						UdpLinkLayerSend_app(temp,9,0,0 ,0,AppProcIDType_serverDisplays_request);
						temp[8] = 2;
						UdpLinkLayerSend_app(temp,9,0,0 ,0,AppProcIDType_serverDisplays_request);
						temp[8] = 3;
						UdpLinkLayerSend_app(temp,9,0,0 ,0,AppProcIDType_serverDisplays_request);
						WM_RestartTimer(PassWord_hTimer,100);
					}
					else						
					{
						pass = 0;
						count = 0;
						TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT30), "密码错误,请重新输入");	
					}
				}
				else
				{
					TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT30), "输入密码");
				}
			}	
		case WM_PAINT:		
		GUI_MEMDEV_WriteAt(PowerOn_DEV, 109, 25);
//		#if (BOOT_SCREEN_SELECTION == HONGMENG)
//			GUI_MEMDEV_WriteAt(PowerOn_DEV1, 0, 125+65);
//		#endif
			GUI_MEMDEV_WriteAt(PassWord_DEV[0], 60+15, 125);
//			GUI_MEMDEV_WriteAt(PassWord_DEV[1], 60+49, 285);
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
	CurrentWindow = passwin;
	return PassWord_hWin;
}

/*
*********************************************************************************************************
*	函 数 名: 
*	功能说明: 创建急停画面
*	形    参: 
*	返 回 值: 
*********************************************************************************************************
*/
WM_HWIN CreateWarning(void)
{
	Warning_hWin = WM_CreateWindowAsChild(0,50,360,163,HomeFrame_hWin,WM_CF_SHOW|WM_CF_HASTRANS,Warning_cbDialog,0);
//	Warning_hTimer = WM_CreateTimer(WM_GetClientWindow(Warning_hWin), 0, 100, 0);
	return Warning_hWin;
}
/*
*********************************************************************************************************
*	函 数 名: 
*	功能说明: 创建急停画面
*	形    参: 
*	返 回 值: 
*********************************************************************************************************
*/
WM_HWIN CreateSuddenStop(WM_HWIN  pMsg)
{
	SuddenStop_hWin = WM_CreateWindowAsChild(0,50,360,163,pMsg,WM_CF_SHOW|WM_CF_HASTRANS,SuddenStop_cbDialog,0);
	SuddenStop_hTimer = WM_CreateTimer(WM_GetClientWindow(SuddenStop_hWin), 0, 100, 0);
	return SuddenStop_hWin;
}

/*
*********************************************************************************************************
*	函 数 名: 
*	功能说明: 创建向右运行画面
*	形    参: 
*	返 回 值: 
*********************************************************************************************************
*/

WM_HWIN CreateMoveFrame_Right(void) 
{
	ShearerRight_hWin = WM_CreateWindowAsChild(0,101,360,104,HomeFrame_hWin,WM_CF_SHOW|WM_CF_HASTRANS,ShearerRight_cbDialog,0);
	
	DtrectionRight_hWin = WM_CreateWindowAsChild(0,55,360,45,HomeFrame_hWin,WM_CF_SHOW|WM_CF_HASTRANS,DtrectionRight_cbDialog,0);
	
#if (MAIN_INTERFACE_ANIMATION == 1)
	ShearerRight_hTimer = WM_CreateTimer(WM_GetClientWindow(ShearerRight_hWin), 0, 100, 0);
	DtrectionRight_hTimer = WM_CreateTimer(WM_GetClientWindow(DtrectionRight_hWin), 0, 100, 0);
#endif
	
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
#if (MAIN_INTERFACE_ANIMATION == 1)
//	ShearerLeft_hTimer = WM_CreateTimer(WM_GetClientWindow(ShearerLeft_hWin), 0, 100, 0);
//	DtrectionLeft_hTimer = WM_CreateTimer(WM_GetClientWindow(DtrectionLeft_hWin), 0, 100, 0);
#endif
	GUI_EndDialog(ShearerRight_hWin, 0);
	GUI_EndDialog(DtrectionRight_hWin, 0);
	MoveFramedata = 2;
	return ShearerLeft_hWin;
}


/*
*********************************************************************************************************
*	函 数 名: 
*	功能说明: 创建向右运行画面
*	形    参: 
*	返 回 值: 
*********************************************************************************************************
*/

WM_HWIN CreateMoveFrame_RightTemp(void) 
{
	ShearerRight_hWin = WM_CreateWindowAsChild(0,101,360,104,HomeFrame_hWin,WM_CF_SHOW|WM_CF_HASTRANS,ShearerRight_cbDialog,0);
	
	DtrectionRight_hWin = WM_CreateWindowAsChild(0,55,360,45,HomeFrame_hWin,WM_CF_SHOW|WM_CF_HASTRANS,DtrectionRight_cbDialog,0);
#if (MAIN_INTERFACE_ANIMATION == 1) 	
//	ShearerRight_hTimer = WM_CreateTimer(WM_GetClientWindow(ShearerRight_hWin), 0, 100, 0);
//	DtrectionRight_hTimer = WM_CreateTimer(WM_GetClientWindow(DtrectionRight_hWin), 0, 100, 0);
#endif	
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

WM_HWIN CreateMoveFrame_LeftTemp(void) 
{
	ShearerLeft_hWin = WM_CreateWindowAsChild(0,101,360,104,HomeFrame_hWin,WM_CF_SHOW|WM_CF_HASTRANS,ShearerLeft_cbDialog,0);
	
	DtrectionLeft_hWin = WM_CreateWindowAsChild(0,55,360,45,HomeFrame_hWin,WM_CF_SHOW|WM_CF_HASTRANS,DtrectionLeft_cbDialog,0);
#if (MAIN_INTERFACE_ANIMATION == 1)
	ShearerLeft_hTimer = WM_CreateTimer(WM_GetClientWindow(ShearerLeft_hWin), 0, 100, 0); 
	DtrectionLeft_hTimer = WM_CreateTimer(WM_GetClientWindow(DtrectionLeft_hWin), 0, 100, 0);
#endif
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
WM_HWIN window;
WM_HWIN CreateHomeFrame(void) 
{
	globalStateType  temp;
	uint8_t length;
	uint16_t transportp[4];
	GUIGetDateFunAPI(API_Support_Statur,0,&temp,&length);
	HomeFrame_hWin = WM_CreateWindowAsChild(60,0,360,320,BackGround_hWin,WM_CF_SHOW|WM_CF_HASTRANS,HomeFrame_cbDialog,0);
	CreateSignal();
	CreateTranSport();
	
	if(temp.DirectionOperation == 1)
	{
		window = CreateMoveFrame_RightTemp();
		//获取是否显示
		GUIGetDateFunAPI(API_Transport_Plane,0,&transportp,&length);
		if((transportp[0] == 0) && (transportp[1] == 0) && (transportp[2] == 0) && (transportp[3] == 0))
		{
			//隐藏窗口
			WM_HideWindow(window);
		}
		else
		{
			//显示窗口
			WM_ShowWindow (window);
		}
	}
	else
	{
		window = CreateMoveFrame_LeftTemp();
		//获取是否显示
		GUIGetDateFunAPI(API_Transport_Plane,0,&transportp,&length);
		if((transportp[0] == 0) && (transportp[1] == 0) && (transportp[2] == 0) && (transportp[3] == 0))
		{

			//显示窗口
			WM_ShowWindow (window);
		}
		else
		{
			//隐藏窗口
			WM_HideWindow(window);
		}
	}

	SupportStatusRefresh(HomeFrame_hWin,0);
	HomeFrame_hTimer = WM_CreateTimer(WM_GetClientWindow(HomeFrame_hWin), 0, 0, 0);
	CurrentWindow = homewin;
	return HomeFrame_hWin;
}
/*
*********************************************************************************************************
*	函 数 名: 
*	功能说明: 创建急停画面
*	形    参: 
*	返 回 值: 
*********************************************************************************************************
*/
WM_HWIN CreateWarningtwo(WM_HWIN  pMsg)
{
	Warningtwo_hWin = WM_CreateWindowAsChild(0,50,360,163,pMsg,WM_CF_SHOW|WM_CF_HASTRANS,Warningtwo_cbDialog,0);
	return Warningtwo_hWin;
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
