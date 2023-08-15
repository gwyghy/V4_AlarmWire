//#include "pngdisplay.h"
//#include "bmpdisplay.h"
//#include "EmWinHZFont.h"
//#include "GUI.h"
//#include "malloc.h"
//#include "delay.h"
//#include "ff.h"
//#include "tftlcd.h"
//#include "DIALOG.h"

//#include "led.h"
//#include "pcf8574.h"
/*
*********************************************************************************************************
*	                                  
*	模块名称 : 
*	文件名称 : HomeTask.c
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
#include "includes.h"					//ucos 使用	  
#include <math.h>
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
extern NameAndId   ShowAuxiliaryAction[];
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
					
WM_HTIMER 	MasterContor_hTimer;
			
			
WM_HWIN 	MasterContor_hWin;
		

		
			
static void MasterContor_cbDialog(WM_MESSAGE * pMsg) 
{
	uint32_t t0, t1;
	static uint8_t sige[6] = {0};
	static uint8_t sige1[6] = {0};
	uint16_t i,j,k;
	uint16_t length,pdata;
	static int beepsta;
	static uint16_t targer;
	uint8_t number,direction,range,numberMAX,numberMIN;
	WM_HWIN hItem;
	int     NCode;
	int     Id;
	NameAndId showbuf[6];
	char buf[10];
	char buf1[10];
	uint8_t send[4] = {0,0,0,0};
	uint8_t send1[4] = {1,0,0,0};
	char Actoinbuf[20];
//	char sign[]
	static  SingleStartUp  TempActoin,CombinedActoin;
	switch (pMsg->MsgId) 
	{
		case WM_CREATE:
			Setstheinitialstate();
//			SendLEDNavigationData(KEY_MASTER,KEY_ALL);
			OvertimeOpen();
			CreateMasterContorTextShow(pMsg);
			beepsta = 0;
			targer = 0;
			Info_Action_Init();
			memset(&TempActoin,0,sizeof(TempActoin));
			memset(&CombinedActoin,0,sizeof(CombinedActoin));
			WM_SetFocus(pMsg->hWin);
		break;
		case MSG_CHANGE_TEXT:	
			GUIGetDateFunAPI(API_Support_Number,0,&number,&length);	//本架编号
			GUIGetDateFunAPI(API_Number_Increase,0,&direction,&length);	//架号增向
			GUIGetDateFunAPI(API_Control_Range,0,&range,&length);	//隔架架数
			GUIGetDateFunAPI(API_Number_MAX,0,&numberMAX,&length);	//最大架号
			GUIGetDateFunAPI(API_Number_MIN,0,&numberMIN,&length);	//最小架号
			if(direction == 0)
			{
				if(pMsg ->Data.v  == 0) //左邻架
				{
					if(abs(beepsta+1) > range)
						break;
					if(!++beepsta)
						beepsta++;
				}
				else//右成组API_Number_Increase
				{
					if(abs(beepsta-1) > range)
						break;
					if(!--beepsta)
						beepsta--;
				}
			}
			else
			{
				if(pMsg ->Data.v  == 0) //左邻架
				{
					if(abs(beepsta-1) > range)
					break;
					if(!--beepsta)
						beepsta--;
				}
				else//右成组API_Number_Increase
				{
					if(abs(beepsta+1) > range)
						break;
					if(!++beepsta)
						beepsta++;
				}			
			}
			targer = number+beepsta;
			TempActoin.Number = number;
			TempActoin.targer = targer;
			sprintf(buf,"%d",targer);
			TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT41),buf);	
//			if(targer < numberMIN)
//				targer = numberMIN;
//			if(targer < numberMAX)
//				targer = numberMAX;
			/*发送目标架号到逻辑*/
			if(direction == left)
			{
				if(beepsta < 0)
				{
					if(beepsta == -1)
					{
						TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT42),"右邻架控制");	
					}
					else
					{
						sprintf(buf,"右隔%d架控制",abs(beepsta)-1);
						TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT42),buf);	
					}
				}
				else if(beepsta > 0)
				{
					if(beepsta == 1)
					{
						TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT42),"左邻架控制");	
					}
					else
					{
						sprintf(buf,"左隔%d架控制",abs(beepsta)-1);
						TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT42),buf);
					}
				}
				else
				{
					TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT42),"急停");
				}
			}
			else if(direction == right)
			{
				if(beepsta > 0)
				{
					if(beepsta == 1)
					{
						TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT42),"右邻架控制");	
					}
					else
					{
						sprintf(buf,"右隔%d架控制",abs(beepsta)-1);
						TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT42),buf);	
					}
				}
				else if(beepsta < 0)
				{
					if(beepsta == -1)
					{
						TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT42),"左邻架控制");	
					}
					else
					{
						sprintf(buf,"左隔%d架控制",abs(beepsta)-1);
						TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT42),buf);
					}
				}
				else
				{
					TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT42),"急停");
				}
			}
		break;
		case WM_KEY:
			if((((WM_KEY_INFO*)(pMsg->Data.p))->PressedCnt) > 0) //按键按下
			{
				ChangeInfoKey(BackGround_hWin,((WM_KEY_INFO*)(pMsg->Data.p))->Key,1);
				switch (((WM_KEY_INFO*)(pMsg->Data.p))->Key) 
				{ 
					case USER_KEY_ESC:
						GUI_EndDialog(pMsg->hWin, 0);
						CreateHomeFrame();				
						break;
					case USER_KEY_UP:
						ChangeInfoText(pMsg->hWin,0);	
						break;
					case USER_KEY_DOWN:
						ChangeInfoText(pMsg->hWin,1);
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
					case USER_KEY_ENTER:
						ActionControlOfAdjacentRack(CombinedActoin,0);
						memset(&CombinedActoin,0,sizeof(CombinedActoin));
						break;

					case USER_KEY_LEFT1:
						if((1+(GetSinglePageValue()*6)) > ShowAuxiliaryAction[0].MAX)
							break;
						sige[0] = 1;
						TempActoin.Action = ShowAuxiliaryAction[((GetSinglePageValue()*6))];
						
						if(TempActoin.Action.type == Sing)
						{
							ActionControlOfAdjacentRack(TempActoin,0);
							InsActionInfoQueue(&TempActoin.Action);
							//发送TempActoin
						}
						else if(TempActoin.Action.type == Combin)
						{
							CombinedActoin = TempActoin;
							memset(showbuf,0,sizeof(showbuf));
							//发送全部抬起
						}
						break;
					case USER_KEY_LEFT2:
						if((3+(GetSinglePageValue()*6)) > ShowAuxiliaryAction[0].MAX)
							break;
						sige[1] = 1;
						 TempActoin.Action = ShowAuxiliaryAction[2+((GetSinglePageValue()*6))];
						
						if(TempActoin.Action.type == Sing)
						{
							ActionControlOfAdjacentRack(TempActoin,0);
							InsActionInfoQueue(&TempActoin.Action);
							//发送TempActoin
						}
						else if(TempActoin.Action.type == Combin)
						{
							CombinedActoin = TempActoin;
							memset(showbuf,0,sizeof(showbuf));
							//发送全部抬起
						}
						break;
					case USER_KEY_LEFT3:
						if((5+(GetSinglePageValue()*6)) > ShowAuxiliaryAction[0].MAX)
							break;
						sige[2] = 1;
						 TempActoin.Action = ShowAuxiliaryAction[4+((GetSinglePageValue()*6))];
						
						if(TempActoin.Action.type == Sing)
						{
							ActionControlOfAdjacentRack(TempActoin,0);
							InsActionInfoQueue(&TempActoin.Action);
							//发送TempActoin
						}
						else if(TempActoin.Action.type == Combin)
						{
							CombinedActoin = TempActoin;
							memset(showbuf,0,sizeof(showbuf));
							//发送全部抬起
						}
						break;
					case USER_KEY_RIGHT1:
						if((2+(GetSinglePageValue()*6)) > ShowAuxiliaryAction[0].MAX)
							break;
						sige[3] = 1;
						 TempActoin.Action = ShowAuxiliaryAction[1+((GetSinglePageValue()*6))];
						
						if(TempActoin.Action.type == Sing)
						{
							ActionControlOfAdjacentRack(TempActoin,0);
							InsActionInfoQueue(&TempActoin.Action);
							//发送TempActoin
						}
						else if(TempActoin.Action.type == Combin)
						{
							CombinedActoin = TempActoin;
							memset(showbuf,0,sizeof(showbuf));
							//发送全部抬起
						}
						break;
					case USER_KEY_RIGHT2:
						if((4+(GetSinglePageValue()*6)) > ShowAuxiliaryAction[0].MAX)
							break;
						sige[4] = 1;
						 TempActoin.Action = ShowAuxiliaryAction[3+((GetSinglePageValue()*6))];
						
						if(TempActoin.Action.type == Sing)
						{
							ActionControlOfAdjacentRack(TempActoin,0);
							InsActionInfoQueue(&TempActoin.Action);
							//发送TempActoin
						}
						else if(TempActoin.Action.type == Combin)
						{
							CombinedActoin = TempActoin;
							memset(showbuf,0,sizeof(showbuf));
							//发送全部抬起
						}
						break;
					case USER_KEY_RIGHT3:
						if((6+(GetSinglePageValue()*6)) > ShowAuxiliaryAction[0].MAX)
							break;
						sige[5] = 1;
						 TempActoin.Action = ShowAuxiliaryAction[5+((GetSinglePageValue()*6))];
						
						if(TempActoin.Action.type == Sing)
						{
							ActionControlOfAdjacentRack(TempActoin,0);
							InsActionInfoQueue(&TempActoin.Action);
							//发送TempActoin
						}
						else if(TempActoin.Action.type == Combin)
						{
							CombinedActoin = TempActoin;
							memset(showbuf,0,sizeof(showbuf));
							//发送全部抬起
						}
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
					case USER_KEY_LEFT1:
						if((1+(GetSinglePageValue()*6)) > ShowAuxiliaryAction[0].MAX)
							break;
						sige[0] = 0;
						 TempActoin.Action = ShowAuxiliaryAction[((GetSinglePageValue()*6))];
						
						if(TempActoin.Action.type == Sing)
						{
							ActionControlOfAdjacentRack(TempActoin,1);
							DelActionInfoQueue(&TempActoin.Action);
							//发送TempActoin
						}
						else if(TempActoin.Action.type == Combin)
						{
							CombinedActoin = TempActoin;
							memset(showbuf,0,sizeof(showbuf));
							//发送全部抬起
						}
						break;
					case USER_KEY_LEFT2:
						if((3+(GetSinglePageValue()*6)) > ShowAuxiliaryAction[0].MAX)
							break;
						sige[1] = 0;
						 TempActoin.Action = ShowAuxiliaryAction[2+((GetSinglePageValue()*6))];
						
						if(TempActoin.Action.type == Sing)
						{
							ActionControlOfAdjacentRack(TempActoin,1);
							DelActionInfoQueue(&TempActoin.Action);
							//发送TempActoin
						}
						else if(TempActoin.Action.type == Combin)
						{
							CombinedActoin = TempActoin;
							memset(showbuf,0,sizeof(showbuf));
							//发送全部抬起
						}
						break;
					case USER_KEY_LEFT3:
						if((5+(GetSinglePageValue()*6)) > ShowAuxiliaryAction[0].MAX)
							break;
						sige[2] = 0;
						 TempActoin.Action = ShowAuxiliaryAction[4+((GetSinglePageValue()*6))];
						
						if(TempActoin.Action.type == Sing)
						{
							ActionControlOfAdjacentRack(TempActoin,1);
							DelActionInfoQueue(&TempActoin.Action);
							//发送TempActoin
						}
						else if(TempActoin.Action.type == Combin)
						{
							CombinedActoin = TempActoin;
							memset(showbuf,0,sizeof(showbuf));
							//发送全部抬起
						}
						break;
					case USER_KEY_RIGHT1:
						if((2+(GetSinglePageValue()*6)) > ShowAuxiliaryAction[0].MAX)
							break;
						sige[3] = 0;
						 TempActoin.Action = ShowAuxiliaryAction[1+((GetSinglePageValue()*6))];
						
						if(TempActoin.Action.type == Sing)
						{
							ActionControlOfAdjacentRack(TempActoin,1);
							DelActionInfoQueue(&TempActoin.Action);
							//发送TempActoin
						}
						else if(TempActoin.Action.type == Combin)
						{
							CombinedActoin = TempActoin;
							memset(showbuf,0,sizeof(showbuf));
							//发送全部抬起
						}
						break;
					case USER_KEY_RIGHT2:
						if((4+(GetSinglePageValue()*6)) > ShowAuxiliaryAction[0].MAX)
							break;
						sige[4] = 0;
						 TempActoin.Action = ShowAuxiliaryAction[3+((GetSinglePageValue()*6))];
						
						if(TempActoin.Action.type == Sing)
						{
							ActionControlOfAdjacentRack(TempActoin,1);
							DelActionInfoQueue(&TempActoin.Action);
							//发送TempActoin
						}
						else if(TempActoin.Action.type == Combin)
						{
							CombinedActoin = TempActoin;
							memset(showbuf,0,sizeof(showbuf));
							//发送全部抬起
						}
						break;
					case USER_KEY_RIGHT3:
						if((6+(GetSinglePageValue()*6)) > ShowAuxiliaryAction[0].MAX)
							break;
						sige[5] = 0;
						 TempActoin.Action = ShowAuxiliaryAction[5+((GetSinglePageValue()*6))];
						
						if(TempActoin.Action.type == Sing)
						{
							ActionControlOfAdjacentRack(TempActoin,1);
							DelActionInfoQueue(&TempActoin.Action);
							//发送TempActoin
						}
						else if(TempActoin.Action.type == Combin)
						{
							CombinedActoin = TempActoin;
							memset(showbuf,0,sizeof(showbuf));
							//发送全部抬起
						}
						break;
				}
				
			}		
			memset(showbuf,0,sizeof(showbuf));
			k = ReadAction_API(showbuf);
			for(i=0,j=k-1;i<k/2;i++,j--)
			{
				NameAndId tem = showbuf[i];
				showbuf[i] = showbuf[j];
				showbuf[j] = tem;
			}
			for(i= 0 ;i<7;i++)
			{
				TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT43+i)," ");
			}
			TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT43),showbuf[0].name);
			TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT44),showbuf[1].name);
			TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT45),showbuf[2].name);
			TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT46),showbuf[3].name);
			TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT47),showbuf[4].name);
			TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT48),showbuf[5].name);
			TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT49),CombinedActoin.Action.name);
			break;		
		case WM_TIMER:	
			if(pMsg->hWin != WM_GetFocussedWindow())
				WM_SetFocus(pMsg->hWin);
			OvertimeReturn(pMsg);
			Determineneedtoreturninterface(pMsg,0);
			WM_RestartTimer(MasterContor_hTimer,100);
			break;
		case WM_PAINT:
			t0 = GUI_GetTime();
			GUI_MEMDEV_WriteAt(HomeFrame_DEV[1], 65, 50);
			GUI_MEMDEV_WriteAt(Group_DEV[2], 60, 270);
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
*	功能说明: 创建邻架画面
*	形    参: 
*	返 回 值: 
*********************************************************************************************************
*/
WM_HWIN CreateMasterContor(void) 
{		
	MasterContor_hWin = WM_CreateWindowAsChild(60,0,360,320,BackGround_hWin,WM_CF_SHOW|WM_CF_HASTRANS,MasterContor_cbDialog,0);
	
	MasterContor_hTimer = WM_CreateTimer(WM_GetClientWindow(MasterContor_hWin), 0, 10, 0);
	return MasterContor_hWin;
}
