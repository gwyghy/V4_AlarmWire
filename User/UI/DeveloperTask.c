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
#include "includes.h"
#include "User_GUI.h"
#include "FileLoad.h"
#include "DoFatfsFiles.h"
#include "FileUpdate.h"
#include "sys_param_type.h"
#include "sys_msg_def.h"
#include "usartapp.h"
/*
*********************************************************************************************************
*	                                     宏定义
*********************************************************************************************************
*/
/*
*********************************************************************************************************
*	                                     列表头定义
*********************************************************************************************************
*/
WM_HTIMER DeveloperhTimer;
WM_HWIN DeveloperhDlg;
extern GUI_MEMDEV_Handle  	BackGround_DEV;
EDIT_Handle hEdit;
/*
*********************************************************************************************************
*	                       GUI_WIDGET_CREATE_INFO类型数组
*********************************************************************************************************
*/

char *Developer_Grope[] =
{
	"中间架",
	"网络",
	"下载程序",
	"更新程序",
};	
char *SupportType_Grope[] =
{
	"工作面",
	"超前架",
	"自移机尾",
	"平板列车",
};	
char *IPaddressg_Grope[] =
{
	"IP地址",
//	"子网掩码",
//	"默认网关",
};
char *ProgramType_Grope[] =
{
	"下载控制器",
	"下载电磁阀",
	"下载角  度",
	"下载人  员",
	"下载声  光",
	"下载高  度",
	"下载红  外",
	"下载键  盘",
	"下载无  线",
	"下载可编程",
	"下载人员BL",
	"下载声光BL",
	"下载高度BL",
	"下载多功能声光",   //ghy 增加下载多功能声光功能
	"下载MiniHUB"          //ghy 增加下载MiniHUB功能
};
char *Filefolder_Grope[] = 
{
	"更新控制器",
	"更新电磁阀",
	"更新角  度",
	"更新人  员",
	"更新声  光",
	"更新高  度",
	"更新红  外",
	"更新键  盘",
	"更新无  线",
	"更新可编程",
	"更新人员BL",
	"更新声光BL",
	"更新高度BL",
	"更新多功能声光", //ghy 增加更新多功能声光功能
	"更新MiniHUB"		 //ghy 增加更新MiniHUB功能
};


const LISTVIEW_HEADER Developer_LISTVIEW_HEADER[] =
{
	{280,""},

};
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = 
{
  { WINDOW_CreateIndirect, 	 "Window",   MENU_WINDOW_ID0, 0, 0, 480, 320, 0, 0x0, 0 },
  { TEXT_CreateIndirect, 	 "Text",     MENU_TEXT_ID0, 0, 0, 480, 50, 0, 0x0, 0 },
  { TEXT_CreateIndirect, 	 "Text",     GUI_ID_TEXT1, 90, 280, 84, 40, 0, 0x0, 0 },
  { TEXT_CreateIndirect, 	 "Text",     GUI_ID_TEXT2, 176, 280, 54, 40, 0, 0x0, 0 },
  { TEXT_CreateIndirect,     "Text",     GUI_ID_TEXT3, 202, 280, 246, 40, 0, 0x0, 0 },
  { LISTVIEW_CreateIndirect, "Listview", GUI_ID_LISTVIEW0, 60, 65, 140, 190, 0, 0x0, 10 },
  { LISTVIEW_CreateIndirect, "Listview", GUI_ID_LISTVIEW1, 220, 65, 200, 190, 0, 0x0, 10 },
};
/*
*********************************************************************************************************
*	函 数 名: 
*	功能说明: 选择型参数输入
*	形    参: 
*	返 回 值: 
*********************************************************************************************************
*/
void Developer_init(WM_MESSAGE * pMsg)
{
	WM_HWIN hItem;
	uint8_t  temp;
	char * sensor;
	char * addrow[10] = {"IP:","192",".","168",".","6",".","192"};
	uint8_t i =1;
	GUI_SetPenSize(10);
	GUI_SetColor(GUI_WHITE);
	TEXT_SetFont(WM_GetDialogItem(pMsg->hWin, MENU_TEXT_ID0), &GUI_FontHZ32);
	TEXT_SetText(WM_GetDialogItem(pMsg->hWin, MENU_TEXT_ID0),"开发者模式");
	TEXT_SetTextColor(WM_GetDialogItem(pMsg->hWin, MENU_TEXT_ID0),GUI_WHITE);
	TEXT_SetTextAlign(WM_GetDialogItem(pMsg->hWin, MENU_TEXT_ID0),GUI_TA_HCENTER | GUI_TA_VCENTER);

	TEXT_SetFont(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT1), &GUI_Font24_1);
	TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT1),"(C)2021");
	TEXT_SetTextColor(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT1),GUI_WHITE);
	TEXT_SetTextAlign(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT1),GUI_TA_HCENTER | GUI_TA_VCENTER);

	TEXT_SetFont(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT2), &GUI_Font24_1);
	TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT2),"HNDZ");
	TEXT_SetTextColor(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT2),GUI_RED);
	TEXT_SetTextAlign(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT2),GUI_TA_HCENTER | GUI_TA_VCENTER);

	TEXT_SetFont(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT3), &GUI_Font24_1);
	TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT3),", All Right Reserved");
	TEXT_SetTextColor(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT3),GUI_WHITE);
	TEXT_SetTextAlign(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT3),GUI_TA_HCENTER | GUI_TA_VCENTER);


	hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW0);
	LISTVIEW_SetHeaderHeight(hItem,0);         //设置标题的高度
	LISTVIEW_SetRowHeight(hItem,47);            //设置LISTVIEW的行高
	LISTVIEW_SetBkColor(hItem,LISTVIEW_CI_UNSEL,GUI_BLACK);    //设置背景颜色
	LISTVIEW_SetBkColor(hItem,LISTVIEW_CI_SEL,GUI_BLACK);    //设置背景颜色
	LISTVIEW_SetBkColor(hItem,LISTVIEW_CI_SELFOCUS,GUI_WHITE);    //设置背景颜色
	LISTVIEW_SetFont(hItem, &GUI_FontHZ24);
	LISTVIEW_SetTextColor(hItem,LISTVIEW_CI_UNSEL,GUI_WHITE);
	LISTVIEW_SetTextColor(hItem,LISTVIEW_CI_SEL,GUI_WHITE);
	LISTVIEW_SetTextColor(hItem,LISTVIEW_CI_SELFOCUS,GUI_BLACK);
	LISTVIEW_SetGridVis(hItem,1);
	LISTVIEW_AddColumn(hItem,140, " ",  GUI_TA_CENTER | GUI_TA_VCENTER); 
	
	SysDataFun(DATA_MSG_GET_DEVICETYPE_PARAM, 0,0,0,0,(void *)&temp,0);
	i = temp;
	Developer_Grope[0] = SupportType_Grope[i];
	for(i=0;i<GUI_COUNTOF(Developer_Grope);i++ )
	{
		sensor = Developer_Grope[i];
		LISTVIEW_AddRow(hItem,&sensor);
	}
	
	LISTVIEW_IncSel(hItem);
	WM_SetFocus(hItem);
	LISTVIEW_SetSel(hItem,0);
	
	
	hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW1);
	LISTVIEW_SetHeaderHeight(hItem,0);         //设置标题的高度
	LISTVIEW_SetRowHeight(hItem,47);            //设置LISTVIEW的行高
	LISTVIEW_SetBkColor(hItem,LISTVIEW_CI_UNSEL,GUI_BLACK);    //设置背景颜色
	LISTVIEW_SetBkColor(hItem,LISTVIEW_CI_SEL,GUI_BLACK);    //设置背景颜色
	LISTVIEW_SetBkColor(hItem,LISTVIEW_CI_SELFOCUS,GUI_WHITE);    //设置背景颜色
	LISTVIEW_SetFont(hItem, &GUI_FontHZ24);
	LISTVIEW_SetTextColor(hItem,LISTVIEW_CI_UNSEL,GUI_WHITE);
	LISTVIEW_SetTextColor(hItem,LISTVIEW_CI_SEL,GUI_WHITE);
	LISTVIEW_SetTextColor(hItem,LISTVIEW_CI_SELFOCUS,GUI_BLACK);
	LISTVIEW_SetGridVis(hItem,1);
	LISTVIEW_AddColumn(hItem,200, " ",  GUI_TA_LEFT | GUI_TA_VCENTER); 
	for(i=0;i<GUI_COUNTOF(SupportType_Grope);i++ )
	{
		sensor = SupportType_Grope[i];
		LISTVIEW_AddRow(hItem,&sensor);
	}
	TEXT_CreateEx(60,60,360,75,pMsg->hWin,WM_CF_HIDE,TEXT_CF_HCENTER|TEXT_CF_VCENTER,GUI_ID_TEXT21," ");
	TEXT_SetBkColor(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT21),GUI_BLACK);
	TEXT_CreateEx(60,185,360,75,pMsg->hWin,WM_CF_HIDE,TEXT_CF_HCENTER|TEXT_CF_VCENTER,GUI_ID_TEXT22," ");
	TEXT_SetBkColor(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT22),GUI_BLACK);
	TEXT_CreateEx(60,135,360,50,pMsg->hWin,WM_CF_HIDE,TEXT_CF_HCENTER|TEXT_CF_VCENTER,GUI_ID_TEXT23," ");
	TEXT_SetBkColor(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT23),GUI_BLACK);
	TEXT_SetTextColor(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT23),GUI_WHITE);
	TEXT_SetFont(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT23), &GUI_FontHZ32);
	LISTVIEW_CreateEx(60,135,360,50,pMsg->hWin,WM_CF_HIDE,TEXT_CF_HCENTER|TEXT_CF_VCENTER,GUI_ID_LISTVIEW2);
	hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW2);
	LISTVIEW_SetHeaderHeight(hItem,0);         //设置标题的高度
	LISTVIEW_SetRowHeight(hItem,50);            //设置LISTVIEW的行高
	LISTVIEW_SetBkColor(hItem,LISTVIEW_CI_UNSEL,GUI_WHITE);    //设置背景颜色
	LISTVIEW_SetBkColor(hItem,LISTVIEW_CI_SEL,GUI_WHITE);    //设置背景颜色
	LISTVIEW_SetBkColor(hItem,LISTVIEW_CI_SELFOCUS,GUI_WHITE);    //设置背景颜色
	LISTVIEW_SetFont(hItem, &GUI_FontHZ24);
	LISTVIEW_SetTextColor(hItem,LISTVIEW_CI_UNSEL,GUI_BLACK);
	LISTVIEW_SetTextColor(hItem,LISTVIEW_CI_SEL,GUI_BLACK);
	LISTVIEW_SetTextColor(hItem,LISTVIEW_CI_SELFOCUS,GUI_BLACK);
//	LISTVIEW_SetGridVis(hItem,1);
	LISTVIEW_AddColumn(hItem,90, " ",  GUI_TA_RIGHT | GUI_TA_VCENTER); 
	LISTVIEW_AddColumn(hItem,40, " ",  GUI_TA_CENTER | GUI_TA_VCENTER); 
	LISTVIEW_AddColumn(hItem,14, " ",  GUI_TA_CENTER | GUI_TA_VCENTER); 
	LISTVIEW_AddColumn(hItem,40, " ",  GUI_TA_CENTER | GUI_TA_VCENTER); 
	LISTVIEW_AddColumn(hItem,14, " ",  GUI_TA_CENTER | GUI_TA_VCENTER); 
	LISTVIEW_AddColumn(hItem,40, " ",  GUI_TA_CENTER | GUI_TA_VCENTER); 
	LISTVIEW_AddColumn(hItem,14, " ",  GUI_TA_CENTER | GUI_TA_VCENTER); 
	LISTVIEW_AddColumn(hItem,40, " ",  GUI_TA_CENTER | GUI_TA_VCENTER); 
	LISTVIEW_AddRow(hItem,addrow);
	LISTVIEW_EnableCellSelect(hItem,1);
}
/*
*********************************************************************************************************
*	函 数 名: 
*	功能说明: 选择型参数输入
*	形    参: 
*	返 回 值: 
*********************************************************************************************************
*/

void USER_GUI_Filefolder(WM_MESSAGE * pMsg)
{
	WM_HWIN hItem;
	uint8_t i;
	char * p;
	hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW1);
	for(i=0;i<GUI_COUNTOF(Filefolder_Grope);i++ )
	{
		p = Filefolder_Grope[i];
		LISTVIEW_AddRow(hItem,&p);
	}
}
/*
*********************************************************************************************************
*	函 数 名: 
*	功能说明: 选择型参数输入
*	形    参: 
*	返 回 值: 
*********************************************************************************************************
*/
void USER_GUI_ID_LISTVIEW1(WM_MESSAGE * pMsg)
{
	WM_HWIN hItem;
	uint8_t row,i;
	char * p;
	
	if(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW0) != WM_GetFocussedWindow())
		return;
	row = LISTVIEW_GetSel(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW0));
	hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW1);
	LISTVIEW_DeleteAllRows(hItem);
	switch (row) 
	{ 
		case 0:
			for(i=0;i<GUI_COUNTOF(SupportType_Grope);i++ )
			{
				p = SupportType_Grope[i];
				LISTVIEW_AddRow(hItem,&p);
			}
			break;
		case 1:
			
			for(i=0;i<GUI_COUNTOF(IPaddressg_Grope);i++ )
			{
				p = IPaddressg_Grope[i];
				LISTVIEW_AddRow(hItem,&p);
			}
			break;
		case 2:
			for(i=0;i<GUI_COUNTOF(ProgramType_Grope);i++ )
			{
				p = ProgramType_Grope[i];
				LISTVIEW_AddRow(hItem,&p);
			}
			break;
		case 3:
			for(i=0;i<GUI_COUNTOF(Filefolder_Grope);i++ )
			{
				p = Filefolder_Grope[i];
				LISTVIEW_AddRow(hItem,&p);
			}
			break;
		default:
		/* 其它的键值不处理 */
			break;
	}
}

static void _usercbeditaddkey(EDIT_Handle hobj ,int key) 
{
	switch (key) 
	{ 
		case USER_KEY_RIGHT1:
			EDIT_SetpfAddKeyEx(hobj,_usercbeditaddkey);
			EDIT_AddKey(hobj,'1');
			break ;
		case USER_KEY_RIGHT2:
			EDIT_SetpfAddKeyEx(hobj,_usercbeditaddkey);
			EDIT_AddKey(hobj,GUI_KEY_BACKSPACE);
			break ;
	}
}

/*
*********************************************************************************************************
*	函 数 名: _cbDialog
*	功能说明: 对话框的回调函数
*	形    参: pMsg 消息指针
*	返 回 值: 无
*********************************************************************************************************
*/
static void _DeveloperDialog(WM_MESSAGE * pMsg) 
{
	WM_HWIN hItem;
	char buf[30];
	uint8_t length;
	globalStateType  StateTypetemp;
	static NetParamType  temp;
	static uint8_t flag;
	static uint8_t sige;
	static uint8_t rowtemp;
	static uint8_t rowtemp2;
	static uint8_t start;
	static uint8_t colu;
	uint8_t i;
	char * p;
	char* res[10];
	uint8_t count;
	static uint16_t ipadd[10] ;
	static uint8_t  BufInd;
	static uint8_t  frist;
	switch (pMsg->MsgId) 
	{
		case WM_INIT_DIALOG:		
			//初始化列表控件
			SendLEDNavigationData(0,KEY_ALL);
			flag = 0;
			sige = 0;
			rowtemp = 0;
			rowtemp2 = 0;
			start = 0;
			frist = 0;
			OvertimeOpen();
			Developer_init(pMsg);	
			break;
		case WM_PAINT:
			GUI_SetBkColor(GUI_BLACK);
			GUI_Clear();
			GUI_DrawLine(30,50,450,50);   //绘制线条
			GUI_DrawLine(30,54,450,54);   //绘制线条
		
			GUI_DrawLine(30,270,450,270);   //绘制线条
			GUI_DrawLine(30,266,450,266);   //绘制线条
			break;
		case WM_TIMER:
			if(pMsg->hWin != WM_GetFocussedWindow())
				WM_SetFocus(pMsg->hWin);
			OvertimeReturn(pMsg);			
			if(start == 1)
			{
				switch(getProLoadStatus())
				{
					case CXBUNCONNECT:
						TEXT_SetText(WM_GetDialogItem(pMsg->hWin, MENU_TEXT_ID0),"程序棒未连接");
						setProLoadStatus(0xFF);
						start = 0;
						frist = 0;
						OvertimeOpen();
						break;
					case NOPROTYPE:
						TEXT_SetText(WM_GetDialogItem(pMsg->hWin, MENU_TEXT_ID0),"没有该类型程序");
						setProLoadStatus(0xFF);
						start = 0;
						frist = 0;
						OvertimeOpen();
						break;
					case PROLOADBREAK:
						TEXT_SetText(WM_GetDialogItem(pMsg->hWin, MENU_TEXT_ID0),"下载中断");
						WM_HideWindow(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT21));
						WM_HideWindow(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT22));
						WM_DeleteWindow(WM_GetDialogItem(pMsg->hWin, GUI_ID_PROGBAR0));
						setProLoadStatus(0xFF);
						start = 0;
						frist = 0;
						OvertimeOpen();
						break;
					case PROLOADCRCERROR:
						TEXT_SetText(WM_GetDialogItem(pMsg->hWin, MENU_TEXT_ID0),"接收错误");
						WM_HideWindow(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT21));
						WM_HideWindow(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT22));
						WM_DeleteWindow(WM_GetDialogItem(pMsg->hWin, GUI_ID_PROGBAR0));
						setProLoadStatus(0xFF);
						start = 0;
						frist = 0;
						OvertimeOpen();
						break;
					case PROLOADSUCCESS:
						TEXT_SetText(WM_GetDialogItem(pMsg->hWin, MENU_TEXT_ID0),"接收成功");
						WM_HideWindow(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT21));
						WM_HideWindow(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT22));
						WM_DeleteWindow(WM_GetDialogItem(pMsg->hWin, GUI_ID_PROGBAR0));
						setProLoadStatus(0xFF);
						start = 0;
						frist = 0;
						OvertimeOpen();
						break;
					case PROLOADING:
						if(frist == 0)
						{
							frist = 1;
							WM_ShowWindow(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT21));
							WM_ShowWindow(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT22));
							PROGBAR_CreateEx(60,135,360,50,pMsg->hWin,WM_CF_SHOW,PROGBAR_CF_HORIZONTAL,GUI_ID_PROGBAR0);
							hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_PROGBAR0);
							PROGBAR_SetFont(hItem, &GUI_FontHZ24);
							PROGBAR_SetMinMax(hItem, 0, 100); 
							OvertimeClose();
						}						
						PROGBAR_SetValue(WM_GetDialogItem(pMsg->hWin, GUI_ID_PROGBAR0),UpdateTimerApi());	
						break;
					default:
						break;	
				}				
			}
			else if(start == 2)
			{
				if(sendTimerAPI() != 100)
				{
					PROGBAR_SetValue(WM_GetDialogItem(pMsg->hWin, GUI_ID_PROGBAR0),sendTimerAPI());
				}
				else
				{
					TEXT_SetText(WM_GetDialogItem(pMsg->hWin, MENU_TEXT_ID0),"发送完成");
					GUI_Delay(1000);
					TEXT_SetText(WM_GetDialogItem(pMsg->hWin, MENU_TEXT_ID0),"L2:本架R2:全面L3:左架R3:右架"); //ghy 
					WM_HideWindow(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT21));
					WM_HideWindow(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT22));
					WM_DeleteWindow(WM_GetDialogItem(pMsg->hWin, GUI_ID_PROGBAR0));
					frist = 0;  //ghy 
					start = 0;
					OvertimeOpen();
				}
			}
			else
			{
				if(getFileRecvStatus() == FILESENDING)
				{
					if(frist == 0)
					{
						frist = 1;
						TEXT_SetText(WM_GetDialogItem(pMsg->hWin, MENU_TEXT_ID0),"正在发送，请稍等");
						WM_ShowWindow(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT21));
						WM_ShowWindow(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT22));
						PROGBAR_CreateEx(60,135,360,50,pMsg->hWin,WM_CF_SHOW,PROGBAR_CF_HORIZONTAL,GUI_ID_PROGBAR0);
						hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_PROGBAR0);
						PROGBAR_SetFont(hItem, &GUI_FontHZ24);
						PROGBAR_SetMinMax(hItem, 0, 100); 
						OvertimeClose();
					}
					else
					{
						if(sendTimerAPI() != 100)
						{
							PROGBAR_SetValue(WM_GetDialogItem(pMsg->hWin, GUI_ID_PROGBAR0),sendTimerAPI());
						}
						else
						{
							TEXT_SetText(WM_GetDialogItem(pMsg->hWin, MENU_TEXT_ID0),"发送完成");
							GUI_Delay(1000);
							TEXT_SetText(WM_GetDialogItem(pMsg->hWin, MENU_TEXT_ID0),"L2:本架R2:全面L3:左架R3:右架"); //ghy 
							WM_HideWindow(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT21));
							WM_HideWindow(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT22));
							WM_DeleteWindow(WM_GetDialogItem(pMsg->hWin, GUI_ID_PROGBAR0));
							setFileRecvStatus(0xFF);
							start = 0;
							OvertimeOpen();
						}
					}				
				}
				
				else
				{
					GUIGetDateFunAPI(API_Support_Statur,0,&StateTypetemp,0);
					if(StateTypetemp.slaveMode == 2)
					{
						if(frist == 0)
						{
							frist = 1;

							WM_ShowWindow(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT21));
							WM_ShowWindow(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT22));
							WM_ShowWindow(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT23));
							TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT23),"正在配址，请勿动");
							start = 3;					
						}
					}
					else if(StateTypetemp.slaveMode == 3)
					{

						WM_HideWindow(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT21));
						WM_HideWindow(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT22));
						WM_HideWindow(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT23));
						SetSysFuncMode(SYSMODE_NORMAL);
						start = 0;
						frist = 0;
					}
				}
			}
			WM_RestartTimer(DeveloperhTimer,100);
            break;
		case WM_KEY:
			if(start)
				break;
			if((((WM_KEY_INFO*)(pMsg->Data.p))->PressedCnt) > 0) //按键按下
			{
				switch (((WM_KEY_INFO*)(pMsg->Data.p))->Key) 
				{ 
					case USER_KEY_ESC:
						GUI_EndDialog(pMsg->hWin, 0);
						CreateBackGround();
					break;					
					case USER_KEY_UP:		
						rowtemp = LISTVIEW_GetSel(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW0));	
						USER_MoveListviewRow(pMsg,0);
						USER_GUI_ID_LISTVIEW1(pMsg);
						if((rowtemp == 2)&&(flag==1))
						{ 
							TEXT_SetText(WM_GetDialogItem(pMsg->hWin, MENU_TEXT_ID0),"L1:修改");
						}	
						break;
					case USER_KEY_DOWN:
						rowtemp = LISTVIEW_GetSel(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW0));
						USER_MoveListviewRow(pMsg,1);
						USER_GUI_ID_LISTVIEW1(pMsg);
						if((rowtemp == 2)&&(flag==1))
						{
							TEXT_SetText(WM_GetDialogItem(pMsg->hWin, MENU_TEXT_ID0),"L1:修改");
						}	
						break;
					case USER_KEY_RIGHT:
						if(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW2) == WM_GetFocussedWindow())
						{
							BufInd = 0;
							if(colu < 2)
								colu = colu+2;
							else
								colu = 1;
							for(i=0;i<8;i++)
							{
								LISTVIEW_SetItemTextColor(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW2), (uint8_t)i, (uint8_t)0, LISTVIEW_CI_UNSEL, GUI_BLACK);
								LISTVIEW_SetItemTextColor(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW2), (uint8_t)i, (uint8_t)0, LISTVIEW_CI_SEL, GUI_BLACK);
								LISTVIEW_SetItemTextColor(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW2), (uint8_t)i, (uint8_t)0, LISTVIEW_CI_SELFOCUS, GUI_BLACK);
								LISTVIEW_SetItemBkColor(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW2), (uint8_t)i, (uint8_t)0, LISTVIEW_CI_UNSEL, GUI_WHITE);
								LISTVIEW_SetItemBkColor(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW2), (uint8_t)i, (uint8_t)0, LISTVIEW_CI_SEL, GUI_WHITE);
								LISTVIEW_SetItemBkColor(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW2), (uint8_t)i, (uint8_t)0, LISTVIEW_CI_SELFOCUS, GUI_WHITE);
							}
							
							
							LISTVIEW_SetItemTextColor(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW2), (uint8_t)colu, (uint8_t)0, LISTVIEW_CI_UNSEL, GUI_WHITE);
							LISTVIEW_SetItemTextColor(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW2), (uint8_t)colu, (uint8_t)0, LISTVIEW_CI_SEL, GUI_WHITE);
							LISTVIEW_SetItemTextColor(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW2), (uint8_t)colu, (uint8_t)0, LISTVIEW_CI_SELFOCUS, GUI_WHITE);
							LISTVIEW_SetItemBkColor(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW2), (uint8_t)colu, (uint8_t)0, LISTVIEW_CI_UNSEL, GUI_LIGHTBLUE);
							LISTVIEW_SetItemBkColor(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW2), (uint8_t)colu, (uint8_t)0, LISTVIEW_CI_SEL, GUI_LIGHTBLUE);
							LISTVIEW_SetItemBkColor(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW2), (uint8_t)colu, (uint8_t)0, LISTVIEW_CI_SELFOCUS, GUI_LIGHTBLUE);
						}
						if(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW0) == WM_GetFocussedWindow())
						{
							LISTVIEW_IncSel(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW1));
							WM_SetFocus(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW1));
							LISTVIEW_SetSel(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW1),0);
							rowtemp = LISTVIEW_GetSel(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW0));
							if((rowtemp == 0) || (rowtemp == 2))
							{
								flag = 1;
								TEXT_SetText(WM_GetDialogItem(pMsg->hWin, MENU_TEXT_ID0),"L1:修改");
							}
							else if(rowtemp == 3)
							{
								flag = 2;
								TEXT_SetText(WM_GetDialogItem(pMsg->hWin, MENU_TEXT_ID0),"L1:打开");
							}
							else if(rowtemp == 1)
							{
								flag = 3;
								TEXT_SetText(WM_GetDialogItem(pMsg->hWin, MENU_TEXT_ID0),"L1:修改");
							}
						}
						break;
					case USER_KEY_LEFT:
						if(sige == 0)
						{
							if(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW1) == WM_GetFocussedWindow())
							{
								LISTVIEW_IncSel(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW0));
								WM_SetFocus(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW0));
								LISTVIEW_SetSel(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW0),rowtemp);
								TEXT_SetText(WM_GetDialogItem(pMsg->hWin, MENU_TEXT_ID0),"开发者模式");	
								flag = 0;								
							}
						}
						else if((sige == 1) || (sige == 3))
						{
														
							if(sige == 3)
								flag = 2;
							if(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW1) == WM_GetFocussedWindow())
							{
								sige = 0;
								LISTVIEW_DeleteAllRows(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW1));
								USER_GUI_Filefolder(pMsg);
								LISTVIEW_IncSel(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW1));
								WM_SetFocus(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW1));
								LISTVIEW_SetSel(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW1),rowtemp2);
								TEXT_SetText(WM_GetDialogItem(pMsg->hWin, MENU_TEXT_ID0),"L1:打开");
							}

						}
						else if(sige == 2)
						{
								sige = 0;
								LISTVIEW_IncSel(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW1));
								WM_SetFocus(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW1));
								LISTVIEW_SetSel(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW1),rowtemp2);
								TEXT_SetText(WM_GetDialogItem(pMsg->hWin, MENU_TEXT_ID0),"L1:修改");
								WM_HideWindow(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW2));
								WM_HideWindow(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT21));
								WM_HideWindow(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT22));
						}
						break;
					case USER_KEY_LEFT1:
						if((flag == 0) || (sige == 1))//ghy 无程序或者已选择其他设备
							break;
						else if(flag == 1)
						{
							rowtemp2 = LISTVIEW_GetSel(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW1));	
							if(rowtemp == 2)
							{
								CanRequestProgramProc(rowtemp2);
								start = 1;
								TEXT_SetText(WM_GetDialogItem(pMsg->hWin, MENU_TEXT_ID0),"正在下载，请稍等");
							}
							else if(rowtemp == 0)
							{
								SysDataFun(DATA_MSG_SET_DEVICETYPE_PARAM, 0,0,0,0,(void *)&rowtemp2,0);
//								CreateDeveloperTask(); //ghy 选完设备之后显示
								//修改后复位控制器,//qinxi
								ETH_Stop();
								BCM_551CS_LOW();
								NVIC_SystemReset(); 
							}
						}
						else if(flag == 2)//
						{
							sige = 1;
							
							rowtemp2 = LISTVIEW_GetSel(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW1));
							LISTVIEW_DeleteAllRows(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW1));
							count = Fatfs_FindDir("0:/update",res,1,rowtemp2);
							if(count ==0)								
							{
								p = "无程序";
								sige = 3;
								flag = 0;
								LISTVIEW_AddRow(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW1),&p);
							}
							for(i=0;i<count;i++ )
							{
								p = res[i];
								LISTVIEW_AddRow(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW1),&p);
							}
							TEXT_SetText(WM_GetDialogItem(pMsg->hWin, MENU_TEXT_ID0),"L2:本架R2:全面L3:左架R3:右架");
							LISTVIEW_IncSel(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW1));
							WM_SetFocus(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW1));
							LISTVIEW_SetSel(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW1),0);
						}
						else if(flag == 3)
						{
							BufInd = 0;
							sige = 2;
							colu = 1;
							TEXT_SetText(WM_GetDialogItem(pMsg->hWin, MENU_TEXT_ID0),"L2:保存");
							SysDataFun(DATA_MSG_GET_IP_PARAM, 0,0,0,0,(void *)&temp,0);
							
							
							
							memset(buf,0,sizeof(buf));
							sprintf(buf, "%d",temp.NetParam_IPAddress[0]);	
							p = buf;
							LISTVIEW_SetItemText(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW2), 1, 0, p);
							
							memset(buf,0,sizeof(buf));
							sprintf(buf, "%d",temp.NetParam_IPAddress[1]);	
							p = buf;
							LISTVIEW_SetItemText(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW2), 3, 0, p);
							
							memset(buf,0,sizeof(buf));
							sprintf(buf, "%d",temp.NetParam_IPAddress[2]);	
							p = buf;
							LISTVIEW_SetItemText(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW2), 5, 0, p);
							
							memset(buf,0,sizeof(buf));
							sprintf(buf, "%d",temp.NetParam_IPAddress[3]);	
							p = buf;
							LISTVIEW_SetItemText(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW2), 7, 0, p);
							
							for(i=0;i<8;i++)
							{
								LISTVIEW_SetItemTextColor(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW2), (uint8_t)i, (uint8_t)0, LISTVIEW_CI_UNSEL, GUI_BLACK);
								LISTVIEW_SetItemTextColor(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW2), (uint8_t)i, (uint8_t)0, LISTVIEW_CI_SEL, GUI_BLACK);
								LISTVIEW_SetItemTextColor(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW2), (uint8_t)i, (uint8_t)0, LISTVIEW_CI_SELFOCUS, GUI_BLACK);
								LISTVIEW_SetItemBkColor(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW2), (uint8_t)i, (uint8_t)0, LISTVIEW_CI_UNSEL, GUI_WHITE);
								LISTVIEW_SetItemBkColor(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW2), (uint8_t)i, (uint8_t)0, LISTVIEW_CI_SEL, GUI_WHITE);
								LISTVIEW_SetItemBkColor(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW2), (uint8_t)i, (uint8_t)0, LISTVIEW_CI_SELFOCUS, GUI_WHITE);
							}
							
							
							LISTVIEW_SetItemTextColor(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW2), (uint8_t)colu, (uint8_t)0, LISTVIEW_CI_UNSEL, GUI_WHITE);
							LISTVIEW_SetItemTextColor(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW2), (uint8_t)colu, (uint8_t)0, LISTVIEW_CI_SEL, GUI_WHITE);
							LISTVIEW_SetItemTextColor(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW2), (uint8_t)colu, (uint8_t)0, LISTVIEW_CI_SELFOCUS, GUI_WHITE);
							LISTVIEW_SetItemBkColor(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW2), (uint8_t)colu, (uint8_t)0, LISTVIEW_CI_UNSEL, GUI_LIGHTBLUE);
							LISTVIEW_SetItemBkColor(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW2), (uint8_t)colu, (uint8_t)0, LISTVIEW_CI_SEL, GUI_LIGHTBLUE);
							LISTVIEW_SetItemBkColor(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW2), (uint8_t)colu, (uint8_t)0, LISTVIEW_CI_SELFOCUS, GUI_LIGHTBLUE);
							
							WM_ShowWindow(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW2));
							WM_ShowWindow(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT21));
							WM_ShowWindow(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT22));
							WM_SetFocus(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW2));

						}
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
						if(sige == 2)
						{
							if(BufInd == 1)//连续输入
							{
								ipadd[colu] = ipadd[colu]*10+DigitKeyToChr(((WM_KEY_INFO*)(pMsg->Data.p))->Key);
								if(colu == 1)
								{
									if(ipadd[colu]>223)
										ipadd[colu] = 223;
								}
								else
								{
									if(ipadd[colu]>255)
										ipadd[colu] = 255;
								}
							}
							else//重新输入
							{
								ipadd[colu] = DigitKeyToChr(((WM_KEY_INFO*)(pMsg->Data.p))->Key);
								BufInd = 1;//连续输入
							}
							memset(buf,0,sizeof(buf));
							sprintf(buf, "%d",ipadd[colu]);	
							p = buf;
							LISTVIEW_SetItemText(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW2), colu, 0, p);
						}
						else if(sige == 1)
						{
							if((((WM_KEY_INFO*)(pMsg->Data.p))->Key) == USER_KEY_RIGHT2)
							{
								start = 2;
								TEXT_SetText(WM_GetDialogItem(pMsg->hWin, MENU_TEXT_ID0),"正在发送，请稍等");
								WM_ShowWindow(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT21));
								WM_ShowWindow(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT22));
								PROGBAR_CreateEx(60,135,360,50,pMsg->hWin,WM_CF_SHOW,PROGBAR_CF_HORIZONTAL,GUI_ID_PROGBAR0);
								hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_PROGBAR0);
								PROGBAR_SetFont(hItem, &GUI_FontHZ24);
								PROGBAR_SetMinMax(hItem, 0, 100); 
								OvertimeClose();
								LISTVIEW_GetItemText(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW1),0,LISTVIEW_GetSel(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW1)),buf,30);
								SendFile_API(FILETYPE,buf,rowtemp2,UPDATEALL);
							}
							else if((((WM_KEY_INFO*)(pMsg->Data.p))->Key) == USER_KEY_RIGHT3)
							{
								start = 2;
								TEXT_SetText(WM_GetDialogItem(pMsg->hWin, MENU_TEXT_ID0),"正在发送，请稍等");
								WM_ShowWindow(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT21));
								WM_ShowWindow(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT22));
								PROGBAR_CreateEx(60,135,360,50,pMsg->hWin,WM_CF_SHOW,PROGBAR_CF_HORIZONTAL,GUI_ID_PROGBAR0);
								hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_PROGBAR0);
								PROGBAR_SetFont(hItem, &GUI_FontHZ24);
								PROGBAR_SetMinMax(hItem, 0, 100); 
								OvertimeClose();
								LISTVIEW_GetItemText(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW1),0,LISTVIEW_GetSel(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW1)),buf,30);
								SendFile_API(FILETYPE,buf,rowtemp2,UPDATERIGHT);
							}
						}
						break;	
						case USER_KEY_LEFT2:	
							if(sige == 1)
							{
								start = 2;
								TEXT_SetText(WM_GetDialogItem(pMsg->hWin, MENU_TEXT_ID0),"正在发送，请稍等");
								WM_ShowWindow(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT21));
								WM_ShowWindow(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT22));
								PROGBAR_CreateEx(60,135,360,50,pMsg->hWin,WM_CF_SHOW,PROGBAR_CF_HORIZONTAL,GUI_ID_PROGBAR0);
								hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_PROGBAR0);
								PROGBAR_SetFont(hItem, &GUI_FontHZ24);
								PROGBAR_SetMinMax(hItem, 0, 100); 
								OvertimeClose();
								LISTVIEW_GetItemText(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW1),0,LISTVIEW_GetSel(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW1)),buf,30);
								SendFile_API(FILETYPE,buf,rowtemp2,UPDATESEIF);
							}
							if(sige == 2)
							{
								sige = 0;
								
								
								memset(buf,0,sizeof(buf));
								LISTVIEW_GetItemTextSorted(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW2),1,0,(char*)buf,4);
								if(buf[2] != 0)
								{
									length = (buf[0]-0x30)*100 + (buf[1]-0x30)*10 + (buf[2]-0x30);
								}
								else if(buf[1] != 0)
								{
									length = (buf[0]-0x30)*10 + (buf[1]-0x30);
								}
								else
								{
									length = (buf[0]-0x30);
								}
								

								temp.NetParam_IPAddress[0] = length;
								temp.NetParam_GateWay[0] = length;
								
								memset(buf,0,sizeof(buf));
								LISTVIEW_GetItemTextSorted(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW2),3,0,(char*)buf,4);
								if(buf[2] != 0)
								{
									length = (buf[0]-0x30)*100 + (buf[1]-0x30)*10 + (buf[2]-0x30);
								}
								else if(buf[1] != 0)
								{
									length = (buf[0]-0x30)*10 + (buf[1]-0x30);
								}
								else
								{
									length = (buf[0]-0x30);
								}
								temp.NetParam_IPAddress[1] = length;
								temp.NetParam_GateWay[1] = length;
								
								SysDataFun(DATA_MSG_SET_IP_PARAM_GLOBAL, 0,0,0,0,(void *)&temp,0);
								
								LISTVIEW_IncSel(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW1));
								WM_SetFocus(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW1));
								LISTVIEW_SetSel(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW1),rowtemp2);
								TEXT_SetText(WM_GetDialogItem(pMsg->hWin, MENU_TEXT_ID0),"L1:修改");
								WM_HideWindow(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW2));
								WM_HideWindow(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT21));
								WM_HideWindow(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT22));
							}
							break;	
						case USER_KEY_LEFT3:	
							if(sige == 1)
							{
								start = 2;
								TEXT_SetText(WM_GetDialogItem(pMsg->hWin, MENU_TEXT_ID0),"正在发送，请稍等");
								WM_ShowWindow(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT21));
								WM_ShowWindow(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT22));
								PROGBAR_CreateEx(60,135,360,50,pMsg->hWin,WM_CF_SHOW,PROGBAR_CF_HORIZONTAL,GUI_ID_PROGBAR0);
								hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_PROGBAR0);
								PROGBAR_SetFont(hItem, &GUI_FontHZ24);
								PROGBAR_SetMinMax(hItem, 0, 100); 
								OvertimeClose();
								LISTVIEW_GetItemText(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW1),0,LISTVIEW_GetSel(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW1)),buf,30);
								SendFile_API(FILETYPE,buf,rowtemp2,UPDATELEFT);
							}
							break;		
					default:
						break;			
				}				
			}
			else //按键抬起
			{

			}
		break;		

		default:
			WM_DefaultProc(pMsg);
			break;
	}
}
/*
*********************************************************************************************************
*	函 数 名: MainTask
*	功能说明: GUI主函数
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
extern uint8_t  CurrentWindow;
void CreateDeveloperTask(void) 
{
	/*
	 关于多缓冲和窗口内存设备的设置说明
	   1. 使能多缓冲是调用的如下函数，用户要在LCDConf_Lin_Template.c文件中配置了多缓冲，调用此函数才有效：
		  WM_MULTIBUF_Enable(1);
	   2. 窗口使能使用内存设备是调用函数：WM_SetCreateFlags(WM_CF_MEMDEV);
	   3. 如果emWin的配置多缓冲和窗口内存设备都支持，二选一即可，且务必优先选择使用多缓冲，实际使用
		  STM32F429BIT6 + 32位SDRAM + RGB565/RGB888平台测试，多缓冲可以有效的降低窗口移动或者滑动时的撕裂
		  感，并有效的提高流畅性，通过使能窗口使用内存设备是做不到的。
	   4. 所有emWin例子默认是开启三缓冲。
	*/
//	WM_MULTIBUF_Enable(1);
//	WM_SetCreateFlags(WM_CF_MEMDEV);

	DeveloperhDlg = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), &_DeveloperDialog, 0, 0, 0);
//	GUI_ExecCreatedDialog(hDlg);

	DeveloperhTimer = WM_CreateTimer(WM_GetClientWindow(DeveloperhDlg), 0, 500, 0);
	CurrentWindow = developerwin;
}

/***************************** 安富莱电子 www.armfly.com (END OF FILE) *********************************/
