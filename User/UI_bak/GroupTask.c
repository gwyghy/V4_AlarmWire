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


extern WM_HWIN BackGround_hWin;


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
					
WM_HTIMER 	GroupLeft_hTimer,
			GroupRight_hTimer;
			
			
WM_HWIN GroupLeft_hWin,
		GroupRight_hWin;
		
extern NameAndId  GroupActionBUF[];
static void GroupLeft_cbDialog(WM_MESSAGE * pMsg) 
{
	uint32_t t0, t1;
	uint8_t length;
	static uint8_t Groupdirection,direction,maxnum,minnum;
	static  GroupStartUp  TempAction;
	WM_HWIN hItem;
	GUI_RECT  Rect;
	int     NCode;
	int     Id;
	NameAndId  temp = {"移架",1,1,1};
	NameAndId  temp1 = {"推溜",1,1,1};
	NameAndId  temp2 = {"伸护帮",1,1,1};
	NameAndId  temp3 = {"收护帮",1,1,1};
	char  showbuf[15];
	switch (pMsg->MsgId) 
	{
		case WM_CREATE:
			SendLEDNavigationData(KEY_GROUP,KEY_ALL);
			OvertimeOpen();
			CreateGroupTextShow(pMsg);	
			SetGroupPageValue(0);
			WM_SetFocus(pMsg->hWin);
		break;		
		case MSG_CHANGE_TEXT:			
			if(pMsg ->Data.v  == 0) //左成组
			{
				Groupdirection = 0;//成组方向
				TempAction.Direction = left; //执行方向
			}
			else//右成组
			{
				Groupdirection = 1;
				TempAction.Direction = right;
			}
			GUIGetDateFunAPI(API_Number_Increase,0,&direction,&length);	//架号增向
			GUIGetDateFunAPI(API_Number_MAX,0,&maxnum,&length);
			GUIGetDateFunAPI(API_Number_MIN,0,&minnum,&length);
			GroupRefresh(pMsg,&GroupActionBUF[1+(GetGroupPageValue()*6)],Groupdirection,&TempAction);
			WM_InvalidateWindow(pMsg->hWin);
		break;
		case WM_KEY:
			SendLEDNavigationData(KEY_GROUP,KEY_ALL);
			if((((WM_KEY_INFO*)(pMsg->Data.p))->PressedCnt) > 0) //按键按下
			{
				WM_SetFocus(pMsg->hWin);
				ChangeInfoKey(BackGround_hWin,((WM_KEY_INFO*)(pMsg->Data.p))->Key,1);
				switch (((WM_KEY_INFO*)(pMsg->Data.p))->Key) 
				{ 
					case USER_KEY_ESC:
						GUI_EndDialog(pMsg->hWin, 0);
						CreateHomeFrame();				
						break;
					case USER_KEY_ENTER:
						ActionControlOfGroupRack(TempAction);
						GUI_EndDialog(pMsg->hWin, 0);
						CreateHomeFrame();				
						break;
					case USER_KEY_UP:
						TempAction.Direction = left;
						WM_InvalidateWindow(pMsg->hWin);					
						break;
					case USER_KEY_DOWN:
						TempAction.Direction = right;
						WM_InvalidateWindow(pMsg->hWin);					
						break;	
					case USER_KEY_LEFT:
						if(Groupdirection == 1)
						{
							if(direction == Groupdirection)
							{
								if(TempAction.EndNumber > TempAction.SrartNumber)
									TempAction.EndNumber --; 
							}
							else
							{
								
								if(TempAction.EndNumber < TempAction.SrartNumber)
									TempAction.EndNumber ++; 
							}
							sprintf(showbuf, "%d", TempAction.EndNumber);
							TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT34),showbuf);
						}
						if(Groupdirection == 0)
						{
							if(direction == Groupdirection)
							{
								if(TempAction.EndNumber < maxnum)
									TempAction.EndNumber ++; 
							}
							else
							{
								if(TempAction.EndNumber > minnum)
									TempAction.EndNumber --; 
							}
						
							sprintf(showbuf, "%d", TempAction.EndNumber);
							TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT33),showbuf);
						}
						break;
					case USER_KEY_RIGHT:
						if(Groupdirection == 1)
						{
							if(direction == Groupdirection)
							{
								if(TempAction.EndNumber < maxnum)
									TempAction.EndNumber ++; 
							}
							else
							{
								if(TempAction.EndNumber > minnum)
									TempAction.EndNumber --; 
							}
							sprintf(showbuf, "%d", TempAction.EndNumber);
							TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT34),showbuf);
						}
						if(Groupdirection == 0)
						{
							if(direction == Groupdirection)
							{
								if(TempAction.EndNumber > TempAction.SrartNumber)
									TempAction.EndNumber --; 
							}
							else
							{
								if(TempAction.EndNumber < TempAction.SrartNumber)
									TempAction.EndNumber ++; 
							}
						
							sprintf(showbuf, "%d", TempAction.EndNumber);
							TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT33),showbuf);
						}
						break;
					case USER_KEY_ASSIST:		
						ChangeInfoButton(BackGround_hWin,1);
						break;
					case USER_KEY_LEFT1:
						if((1+(GetGroupPageValue()*6)) > GetGroupMAXValue())
							break;						
						GroupRefresh(pMsg,&GroupActionBUF[1+(GetGroupPageValue()*6)],Groupdirection,&TempAction);
						break;	
					case USER_KEY_RIGHT1:
						if((2+(GetGroupPageValue()*6)) > GetGroupMAXValue())
							break;						
						GroupRefresh(pMsg,&GroupActionBUF[2+(GetGroupPageValue()*6)],Groupdirection,&TempAction);					
						break;	
					case USER_KEY_LEFT2:
						if((3+(GetGroupPageValue()*6)) > GetGroupMAXValue())
							break;						
						GroupRefresh(pMsg,&GroupActionBUF[3+(GetGroupPageValue()*6)],Groupdirection,&TempAction);
						break;	
					case USER_KEY_RIGHT2:
						if((4+(GetGroupPageValue()*6)) > GetGroupMAXValue())
							break;						
						GroupRefresh(pMsg,&GroupActionBUF[4+(GetGroupPageValue()*6)],Groupdirection,&TempAction);					
						break;
					case USER_KEY_LEFT3:
						if((5+(GetGroupPageValue()*6)) > GetGroupMAXValue())
							break;						
						GroupRefresh(pMsg,&GroupActionBUF[5+(GetGroupPageValue()*6)],Groupdirection,&TempAction);						
						break;	
					case USER_KEY_RIGHT3:
						if((6+(GetGroupPageValue()*6)) > GetGroupMAXValue())
							break;						
						GroupRefresh(pMsg,&GroupActionBUF[6+(GetGroupPageValue()*6)],Groupdirection,&TempAction);					
						break;						
				}
			}
			else
			{
				ChangeInfoKey(BackGround_hWin,((WM_KEY_INFO*)(pMsg->Data.p))->Key,0);
			}		
			break;
		case WM_TIMER:	
			if(pMsg->hWin != WM_GetFocussedWindow())
				WM_SetFocus(pMsg->hWin);
			OvertimeReturn(pMsg);
			WM_RestartTimer(GroupLeft_hTimer,100);
			break;
		case WM_PAINT:
			t0 = GUI_GetTime();
			GUI_MEMDEV_WriteAt(HomeFrame_DEV[1], 65, 50);
			GUI_MEMDEV_WriteAt(PassWord_DEV[0], 75, 83);
			GUI_MEMDEV_WriteAt(HomeFrame_DEV[2], 95, 145);
			GUI_MEMDEV_WriteAt(HomeFrame_DEV[2], 306, 145);
			GUI_MEMDEV_WriteAt(Group_DEV[2], 60, 215);
			if(TempAction.Direction == 1)
				GUI_MEMDEV_WriteAt(Group_DEV[1], 181, 145);//右成组
			if(TempAction.Direction == 0)
				GUI_MEMDEV_WriteAt(Group_DEV[0], 181, 145);//左成组
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
*	功能说明: 创建左成组画面
*	形    参: 
*	返 回 值: 
*********************************************************************************************************
*/
WM_HWIN CreateGroupLeft(void) 
{		
	GroupLeft_hWin = WM_CreateWindowAsChild(60,0,360,320,BackGround_hWin,WM_CF_SHOW|WM_CF_HASTRANS,GroupLeft_cbDialog,0);
	
	GroupLeft_hTimer = WM_CreateTimer(WM_GetClientWindow(GroupLeft_hWin), 0, 500, 0);
	return GroupLeft_hWin;
}
/*
*********************************************************************************************************
*	函 数 名: 
*	功能说明: 创建右成组画面
*	形    参: 
*	返 回 值: 
*********************************************************************************************************
*/
WM_HWIN CreateGroupRight(void) 
{		
	GroupLeft_hWin = WM_CreateWindowAsChild(60,0,360,320,BackGround_hWin,WM_CF_SHOW|WM_CF_HASTRANS,GroupLeft_cbDialog,0);
	
	GroupLeft_hTimer = WM_CreateTimer(WM_GetClientWindow(GroupLeft_hWin), 0, 10, 0);
	return GroupLeft_hWin;
}
