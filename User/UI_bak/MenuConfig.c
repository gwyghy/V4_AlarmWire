/*
*********************************************************************************************************
*	                                  
*	模块名称 : 
*	文件名称 : MenuConfig.c
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
#include "MenuConfig.h"
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


/*
*********************************************************************************************************
*	                                     列表头定义
*********************************************************************************************************
*/
const LISTVIEW_HEADER Meum_LISTVIEW_HEADER[] =
{
	{39,""},
	{200,"名称"},
	{103,"数值"},
};
/*
*********************************************************************************************************
*	                                     选择项表定义
*********************************************************************************************************
*/

const char *SelectItem[][15] =
{/*禁止修改*/

{"无"},
{"禁止","允许"},	
{"电话","声光","声","光","无"},
{"模拟量","常开","常闭"},
{"模拟量","常开","常闭"},
{"1","10","100","1000"},
{"can1","can2","模拟量接口"},
{"电磁阀驱动器","角度","高度","声光","红外","人员"},
{"模拟1","模拟2","模拟3","模拟4"},
{"本架","它架"},
{"动作","声光报警","某状态或方法"},
{"启动","运行","结束"},
{"左","右","左右"},
{"直接启动","停止","延时启动","暂停","恢复"},
{"NO","ALWAYS","2040","1020"},
{"NO","ALWAYS","BLUE","RED","2020"},
{"左","右"},
{"左","右","全面"},
{"1db","3db","6db","9db","12db","18db","21db","24db","27db","30db","33db"},








};/*禁止修改*/
/*
*********************************************************************************************************
*	                                     参数属性属性定义
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*	                                    按键名称定义
*********************************************************************************************************
*/
const char *ButtonName[][12] = 
{
	{"系统参数","单动作","传感器","组合动作","部件","成组动作",
	 " "," "," "," "," "," "},
};


/*
*********************************************************************************************************
*	                                    文本名称定义
*********************************************************************************************************
*/

const char TextName[] = 
{
	"菜单设置"
};

/*
*********************************************************************************************************
*	函 数 名: 
*	功能说明: 获取指针和长度
*	形    参: 
*	返 回 值: 
*********************************************************************************************************
*/
void GetPointerFromLogicalPast(uint64_t id,MENU_PARA **address ,uint8_t *length)
{

}


/*
*********************************************************************************************************
*	函 数 名: 
*	功能说明: 创建菜单列表
*	形    参: 
*	返 回 值: 
*********************************************************************************************************
*/
void Creator_Menuwindow(WM_MESSAGE * pMsg)
{
	USER_ReviseListview(pMsg);
	USER_ReviseListviewHeader(pMsg,Meum_LISTVIEW_HEADER,sizeof(Meum_LISTVIEW_HEADER));
	Setrankvalue(0,0);
	Setrankvalue(1,0);
	Setrankvalue(2,0);
	Setrankvalue(3,0);
	Setrankvalue(4,0);
	Setrankvalue(5,0);
	Setrankvalue(6,0);
	Setrankvalue(7,0);
	SetPageValue(0);
	SetLevelValue(0);
	SetRowValue(0);
	SetconfigState(0);
	USER_ReviseButtonStyle(pMsg);
	Menu_ShowButtonForPage(pMsg,PAGE_0_0_0);	
	InitMenuText(pMsg);		
	Menu_ShowListviewForPage(pMsg,0x100000000000000);	

}

/*
*********************************************************************************************************
*	函 数 名: 
*	功能说明: 获取页面最大值
*	形    参: 
*	返 回 值: 
*********************************************************************************************************
*/
uint8_t GetMAXpage(uint8_t id)
{
	uint8_t page = 0;
	if(id > 6)
	{
		if(id%6)
			page = (id/6);
		else
			page = (id/6)-1;
	}
	return page;
}
/*
*********************************************************************************************************
*	函 数 名: 
*	功能说明: 设置当前页面
*	形    参: 
*	返 回 值: 
*********************************************************************************************************
*/
void Menu_SetCurrentPage(uint16_t key)
{
	uint8_t	rank[LEVEL_MAX] = {0};
	uint8_t level;
	uint64_t id;
	static uint8_t  MAXbutton = 6;
	level = GetLevelValue();
	id = Getrankvalue(rank);
	switch (key) 	 
	{
		case USER_KEY_LEFT1:		
			if((1+(GetPageValue()*6)) > MAXbutton)
				break;
			Setrankvalue(level,1+(GetPageValue()*6));
			if((getJsonParaIsChilds(Getrankvalue(rank))) && (level<LEVEL_MAX))//如果有孩子
			{
				SetLevelValue(level+1);
				MAXbutton = GetMAXbutton(Getrankvalue(rank));
			}
			break;
		case USER_KEY_LEFT2:
			if((3+(GetPageValue()*6)) > MAXbutton)
				break;
			Setrankvalue(level,3+(GetPageValue()*6));
			if((getJsonParaIsChilds(Getrankvalue(rank))) && (level<LEVEL_MAX))//如果有孩子
			{
				SetLevelValue(level+1);
				MAXbutton = GetMAXbutton(Getrankvalue(rank));
			}
			break;	
		case USER_KEY_LEFT3:
			if((5+(GetPageValue()*6)) > MAXbutton)
				break;					
			Setrankvalue(level,5+(GetPageValue()*6));
			if((getJsonParaIsChilds(Getrankvalue(rank))) && (level<LEVEL_MAX))//如果有孩子
			{
				SetLevelValue(level+1);
				MAXbutton = GetMAXbutton(Getrankvalue(rank));
			}
			break;
		case USER_KEY_RIGHT1:
			if((2+(GetPageValue()*6)) > MAXbutton)
				break;					
			Setrankvalue(level,2+(GetPageValue()*6));
			if((getJsonParaIsChilds(Getrankvalue(rank))) && (level<LEVEL_MAX))//如果有孩子
			{
				SetLevelValue(level+1);
				MAXbutton = GetMAXbutton(Getrankvalue(rank));
			}
			break;					
		case USER_KEY_RIGHT2:
			if((4+(GetPageValue()*6)) > MAXbutton)
				break;					
			Setrankvalue(level,4+(GetPageValue()*6));
			if((getJsonParaIsChilds(Getrankvalue(rank))) && (level<LEVEL_MAX))//如果有孩子
			{
				SetLevelValue(level+1);
				MAXbutton = GetMAXbutton(Getrankvalue(rank));
			}
			break;
		case USER_KEY_RIGHT3:
			if((6+(GetPageValue()*6)) > MAXbutton)
				break;					
			Setrankvalue(level,6+(GetPageValue()*6));
			if((getJsonParaIsChilds(Getrankvalue(rank))) && (level<LEVEL_MAX))//如果有孩子
			{
				SetLevelValue(level+1);
				MAXbutton = GetMAXbutton(Getrankvalue(rank));
			}
			break;
		case USER_KEY_ASSIST:
			if(id == 0)
				break ;
			if(GetMAXpage(MAXbutton) <= GetPageValue())
			{
				SetPageValue(0);
				Setrankvalue(level,1+(GetPageValue()*6));
			}
			else
			{
				SetPageValue(GetPageValue()+1);
				Setrankvalue(level,1+(GetPageValue()*6));
			}
			break;
		case USER_KEY_LEFT:	
			Setrankvalue(level,0);
			if(level > 0)
			{
				Setrankvalue(level-1,0);
				SetLevelValue(level-1);
			}
			SetPageValue(0);			
			MAXbutton = GetMAXbutton(Getrankvalue(rank));
			break;
			default:
				break;			
	}
}
/*
*********************************************************************************************************
*	函 数 名: 
*	功能说明: 根据当前页面显示按键
*	形    参: 
*	返 回 值: 
*********************************************************************************************************
*/
void Menu_ShowButtonForPage(WM_MESSAGE * pMsg,uint64_t id)
{
	static uint8_t leve,leveold;
	uint8_t  count,i;
	STR_V4PARA_NAME *temp = NULL;
	static char * ButtonOptions[40];
	if(id == 0)		
	{
		USER_ReviseAllButtonShow(pMsg,ButtonName[0]);
		leveold = 0;
	}
	else
	{		
		leve = GetLevelValue();
		if(leve != leveold)
		{
			memset(ButtonOptions,0,sizeof(ButtonOptions));
			count = getJsonParaStruct(id,&temp);
			if(count != 0)
			{	
				for(i=0;i<count;i++)
				{
					ButtonOptions[i] = (char *)temp;
					temp = temp+1;
				}
			}
			leveold = leve;
		}
		USER_ReviseAllButtonShow(pMsg,ButtonOptions);
	}
}
	

/*
*********************************************************************************************************
*	函 数 名: 
*	功能说明: 根据当前页面显示列表
*	形    参: 
*	返 回 值: 
*********************************************************************************************************
*/
void Menu_ShowListviewForPage(WM_MESSAGE * pMsg,uint64_t id)
{
	if(id == 0)
		id = 0x100000000000000;
	USER_ShwoListviewData(pMsg,GetListviewData(id));
}
/*
*********************************************************************************************************
*	函 数 名: 
*	功能说明: 根据当前页面显示文本
*	形    参: 
*	返 回 值: 
*********************************************************************************************************
*/
void Menu_ShowTextForPage(WM_MESSAGE * pMsg,uint64_t id)
{
uint8_t i;
uint64_t send = 0;
char * TextOptions;
char buf[9];
char show[30];
uint64_t temp;
uint8_t	rank[LEVEL_MAX] = {0};
temp = Getrankvalue(rank);
memset(buf,0,sizeof(buf));
memset(show,0,sizeof(show));
// sprintf((char *)buf, "%d""%d""%d""%d""%d""%d""%d""%d",rank[0],rank[1],rank[2],rank[3],rank[4],rank[5],rank[6],rank[7]);

for(i=0;i<8;i++)
{
	if(rank[i] != 0)
	{
		send |= ((uint64_t)rank[i] << ((7-i)*8));
		sprintf((char *)buf,"%x",(int)(send>>32));
		strcat(show,buf);
		strcat(show,"/");
	}
}


if(id == 0)
{
	USER_ReviseTextShow(pMsg,MENU_TEXT_ID0,"菜单设置");
}	
else
{
//	show(strlen(show));
	USER_ReviseTextShow(pMsg,MENU_TEXT_ID0,(char *)show);
}
return;
}
/*
*********************************************************************************************************
*	函 数 名: 
*	功能说明: 开始设置参数
*	形    参: 
*	返 回 值: 
*********************************************************************************************************
*/
void Menu_SetValueListviewData(WM_MESSAGE * pMsg,uint16_t key)
{
	
}


























