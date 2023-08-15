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
*	ģ������ : 
*	�ļ����� : HomeTask.c
*	��    �� : V1.0
*	˵    �� : ʵ�ְ������ֹ���
*              
*	�޸ļ�¼ :
*		�汾��   ����         ����          ˵��
*		V1.0    2016-07-16   Eric2013  	    �װ�    
*                                     
*	Copyright (C), 2015-2020, ���������� www.armfly.com
*
*********************************************************************************************************
*/	
#include "User_GUI.h"
#include "includes.h"					//ucos ʹ��	  
#include <math.h>
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
			GUIGetDateFunAPI(API_Support_Number,0,&number,&length);	//���ܱ��
			GUIGetDateFunAPI(API_Number_Increase,0,&direction,&length);	//�ܺ�����
			GUIGetDateFunAPI(API_Control_Range,0,&range,&length);	//���ܼ���
			GUIGetDateFunAPI(API_Number_MAX,0,&numberMAX,&length);	//���ܺ�
			GUIGetDateFunAPI(API_Number_MIN,0,&numberMIN,&length);	//��С�ܺ�
			if(direction == 0)
			{
				if(pMsg ->Data.v  == 0) //���ڼ�
				{
					if(abs(beepsta+1) > range)
						break;
					if(!++beepsta)
						beepsta++;
				}
				else//�ҳ���API_Number_Increase
				{
					if(abs(beepsta-1) > range)
						break;
					if(!--beepsta)
						beepsta--;
				}
			}
			else
			{
				if(pMsg ->Data.v  == 0) //���ڼ�
				{
					if(abs(beepsta-1) > range)
					break;
					if(!--beepsta)
						beepsta--;
				}
				else//�ҳ���API_Number_Increase
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
			/*����Ŀ��ܺŵ��߼�*/
			if(direction == left)
			{
				if(beepsta < 0)
				{
					if(beepsta == -1)
					{
						TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT42),"���ڼܿ���");	
					}
					else
					{
						sprintf(buf,"�Ҹ�%d�ܿ���",abs(beepsta)-1);
						TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT42),buf);	
					}
				}
				else if(beepsta > 0)
				{
					if(beepsta == 1)
					{
						TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT42),"���ڼܿ���");	
					}
					else
					{
						sprintf(buf,"���%d�ܿ���",abs(beepsta)-1);
						TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT42),buf);
					}
				}
				else
				{
					TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT42),"��ͣ");
				}
			}
			else if(direction == right)
			{
				if(beepsta > 0)
				{
					if(beepsta == 1)
					{
						TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT42),"���ڼܿ���");	
					}
					else
					{
						sprintf(buf,"�Ҹ�%d�ܿ���",abs(beepsta)-1);
						TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT42),buf);	
					}
				}
				else if(beepsta < 0)
				{
					if(beepsta == -1)
					{
						TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT42),"���ڼܿ���");	
					}
					else
					{
						sprintf(buf,"���%d�ܿ���",abs(beepsta)-1);
						TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT42),buf);
					}
				}
				else
				{
					TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT42),"��ͣ");
				}
			}
		break;
		case WM_KEY:
			if((((WM_KEY_INFO*)(pMsg->Data.p))->PressedCnt) > 0) //��������
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
							//����TempActoin
						}
						else if(TempActoin.Action.type == Combin)
						{
							CombinedActoin = TempActoin;
							memset(showbuf,0,sizeof(showbuf));
							//����ȫ��̧��
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
							//����TempActoin
						}
						else if(TempActoin.Action.type == Combin)
						{
							CombinedActoin = TempActoin;
							memset(showbuf,0,sizeof(showbuf));
							//����ȫ��̧��
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
							//����TempActoin
						}
						else if(TempActoin.Action.type == Combin)
						{
							CombinedActoin = TempActoin;
							memset(showbuf,0,sizeof(showbuf));
							//����ȫ��̧��
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
							//����TempActoin
						}
						else if(TempActoin.Action.type == Combin)
						{
							CombinedActoin = TempActoin;
							memset(showbuf,0,sizeof(showbuf));
							//����ȫ��̧��
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
							//����TempActoin
						}
						else if(TempActoin.Action.type == Combin)
						{
							CombinedActoin = TempActoin;
							memset(showbuf,0,sizeof(showbuf));
							//����ȫ��̧��
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
							//����TempActoin
						}
						else if(TempActoin.Action.type == Combin)
						{
							CombinedActoin = TempActoin;
							memset(showbuf,0,sizeof(showbuf));
							//����ȫ��̧��
						}
						break;
					
					default:
						break;			
				}
			}
			else //����̧��
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
							//����TempActoin
						}
						else if(TempActoin.Action.type == Combin)
						{
							CombinedActoin = TempActoin;
							memset(showbuf,0,sizeof(showbuf));
							//����ȫ��̧��
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
							//����TempActoin
						}
						else if(TempActoin.Action.type == Combin)
						{
							CombinedActoin = TempActoin;
							memset(showbuf,0,sizeof(showbuf));
							//����ȫ��̧��
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
							//����TempActoin
						}
						else if(TempActoin.Action.type == Combin)
						{
							CombinedActoin = TempActoin;
							memset(showbuf,0,sizeof(showbuf));
							//����ȫ��̧��
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
							//����TempActoin
						}
						else if(TempActoin.Action.type == Combin)
						{
							CombinedActoin = TempActoin;
							memset(showbuf,0,sizeof(showbuf));
							//����ȫ��̧��
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
							//����TempActoin
						}
						else if(TempActoin.Action.type == Combin)
						{
							CombinedActoin = TempActoin;
							memset(showbuf,0,sizeof(showbuf));
							//����ȫ��̧��
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
							//����TempActoin
						}
						else if(TempActoin.Action.type == Combin)
						{
							CombinedActoin = TempActoin;
							memset(showbuf,0,sizeof(showbuf));
							//����ȫ��̧��
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
*	�� �� ��: 
*	����˵��: �����ڼܻ���
*	��    ��: 
*	�� �� ֵ: 
*********************************************************************************************************
*/
WM_HWIN CreateMasterContor(void) 
{		
	MasterContor_hWin = WM_CreateWindowAsChild(60,0,360,320,BackGround_hWin,WM_CF_SHOW|WM_CF_HASTRANS,MasterContor_cbDialog,0);
	
	MasterContor_hTimer = WM_CreateTimer(WM_GetClientWindow(MasterContor_hWin), 0, 10, 0);
	return MasterContor_hWin;
}
