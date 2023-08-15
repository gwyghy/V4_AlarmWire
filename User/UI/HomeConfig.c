/*
*********************************************************************************************************
*	                                  
*	模块名称 : 
*	文件名称 : HomeConfig.c
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
#include "ProjectConfig.h"
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
static FIL file;
static GUI_GIF_INFO InfoGif;
static GUI_GIF_IMAGE_INFO ImagInfoGif;


GUI_MEMDEV_Handle  	BackGround_DEV,
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
					PowerOn_DEV2,
					PassWord_DEV[2],
					HomeFrame_DEV[5],
					Group_DEV[5];
					
/*
*********************************************************************************************************
*	                                     列表头定义
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*	                                     选择项表定义
*********************************************************************************************************
*/
static FIL ScrSortFile;  
void emwinbmp_new_pathname(uint8_t *pname)
{
    uint8_t res;
    uint16_t index=0;		
    while(index<0XFFFF)
    {
        sprintf((char*)pname,"屏幕截图%05d.bmp",index);
        res=f_open(&ScrSortFile,(const TCHAR*)pname,FA_READ);//尝试打开这个文件
        if(res==FR_NO_FILE)break;   //改文件名不存在=正是我们需要的
        else if(res==FR_OK) f_close(&ScrSortFile);//如果打开成功，就关闭掉
        index++;
    }
}
static void _WriteByte2File(U8 Data, void * p) 
{
	UINT nWritten;
	f_write((FIL*)p,&Data,1,&nWritten);
}
void create_bmppicture(uint8_t *filename,int x0,int y0,int Xsize,int Ysize)
{
	static FIL hFile;
	//创建一个文件，路径为filename,如果文件原本就存在的话会被新建的文件覆盖掉！
	f_open(&hFile,(const TCHAR*)filename,FA_READ|FA_WRITE|FA_CREATE_ALWAYS);
	GUI_BMP_SerializeEx(_WriteByte2File,x0,y0,Xsize,Ysize,&hFile);
	f_close(&hFile);	//关闭文件
}



void GUI_ShowGIF(const char * sFilename) 
{
	uint16_t i = 0;
	uint32_t t0, t1;
	char result;
	uint16_t bread;
	char *_acBuffer;
	GUI_HMEM hMem;

	/* 打开文件 */		
	result = f_open(&file, sFilename, FA_OPEN_EXISTING | FA_READ | FA_OPEN_ALWAYS);
	if (result != FR_OK)
	{
		return;
	}
	hMem = GUI_ALLOC_AllocZero(file.obj.objsize);
	_acBuffer = GUI_ALLOC_h2p(hMem);
	result = f_read(&file, _acBuffer, file.obj.objsize, (UINT *)&bread);
	if (result != FR_OK)
	{
		return;
	}
   GUI_GIF_GetInfo(_acBuffer, file.obj.objsize, &InfoGif);
	
   while(1)
   {
	   i = InfoGif.NumImages-1;
//		if(i < 50)//c)
	    {
	        GUI_GIF_GetImageInfo(_acBuffer, file.obj.objsize, &ImagInfoGif, i);
			if(ImagInfoGif.Delay == 0)
			{
				GUI_Delay(100);
			}
			else
			{
				t0 = GUI_GetTime();
				GUI_GIF_DrawSub(_acBuffer, 
								file.obj.objsize, 
								(LCD_GetXSize() - InfoGif.xSize)/2,
								(LCD_GetYSize() - InfoGif.ySize)/2, 
								i++);
				t1 = GUI_GetTime() - t0;
				if (t1 < ImagInfoGif.Delay * 10) 
				{
					GUI_Delay(ImagInfoGif.Delay * 10 - t1);
				}
			}			  
	    }
//	    else
	    {
			Home_Create_memdev();
			break;
	    }
	}	
 	GUI_ALLOC_Free(hMem);
 	f_close(&file);
}



GUI_HMEM AddToJPEG(const char *sFilename,GUI_MEMDEV_Handle hMemBMP,uint16_t XSeat,uint16_t YSeat) 
{
	char *_acBuffer;
	char result;
	uint16_t bread;
	GUI_HMEM hMem;
//	GUI_MEMDEV_Handle hMemBMP;	
	result = f_open(&file, sFilename, FA_OPEN_EXISTING | FA_READ | FA_OPEN_ALWAYS);
	if (result != FR_OK)
	{
		return 0;
	}
	hMem = GUI_ALLOC_AllocZero(file.obj.objsize);
	_acBuffer = GUI_ALLOC_h2p(hMem);
	result = f_read(&file, _acBuffer, file.obj.objsize, (UINT *)&bread);
	if (result != FR_OK)
	{
		return 0;
	}
	GUI_MEMDEV_Select(hMemBMP);
	GUI_JPEG_Draw(_acBuffer, file.obj.objsize, XSeat, YSeat);
	GUI_MEMDEV_Select(0);
	GUI_ALLOC_Free(hMem);
	
	f_close(&file);
	
	return hMemBMP;
}



GUI_HMEM AddToBMP(const char *sFilename,GUI_MEMDEV_Handle hMemBMP,uint16_t XSeat,uint16_t YSeat) 
{
	char *_acBuffer;
	char result;
	uint16_t bread;
	GUI_HMEM hMem;
//	GUI_MEMDEV_Handle hMemBMP;	
	result = f_open(&file, sFilename, FA_OPEN_EXISTING | FA_READ | FA_OPEN_ALWAYS);
	if (result != FR_OK)
	{
		return 0;
	}
	hMem = GUI_ALLOC_AllocZero(file.obj.objsize);
	_acBuffer = GUI_ALLOC_h2p(hMem);
	result = f_read(&file, _acBuffer, file.obj.objsize, (UINT *)&bread);
	if (result != FR_OK)
	{
		return 0;
	}
	GUI_MEMDEV_Select(hMemBMP);
	GUI_BMP_Draw(_acBuffer, XSeat, YSeat);
	GUI_MEMDEV_Select(0);
	GUI_ALLOC_Free(hMem);
	
	f_close(&file);
	
	return hMemBMP;
}
GUI_HMEM AddToPNG(const char *sFilename,GUI_MEMDEV_Handle hMemPNG,uint16_t XSeat,uint16_t YSeat) 
{
	
	char *_acBuffer;
	char result;
	uint16_t bread;
	GUI_HMEM hMem;
//	GUI_MEMDEV_Handle hMemBMP;
	
	result = f_open(&file, sFilename, FA_OPEN_EXISTING | FA_READ /*| FA_OPEN_ALWAYS*/);
	if (result != FR_OK)
	{
		return 0;
	}
	hMem = GUI_ALLOC_AllocZero(file.obj.objsize);
	
	_acBuffer = GUI_ALLOC_h2p(hMem);

	result = f_read(&file, _acBuffer, file.obj.objsize, (UINT *)&bread);
	if (result != FR_OK)
	{
		return 0;
	}
	GUI_MEMDEV_Select(hMemPNG);
	GUI_SetBkColor(GUI_TRANSPARENT);
	GUI_Clear();
	GUI_PNG_Draw(_acBuffer, file.obj.objsize,XSeat,YSeat);
	GUI_MEMDEV_Select(0);

	GUI_ALLOC_Free(hMem);
	
	f_close(&file);
	
	return hMemPNG;
}



void Home_Create_memdev(void)
{
	PassWord_DEV[0] = GUI_MEMDEV_CreateFixed(0, 0, 332, 49,  GUI_MEMDEV_HASTRANS,GUI_MEMDEV_APILIST_32,GUI_COLOR_CONV_8888);
	AddToPNG("密码输入框.png",PassWord_DEV[0],0,0);
//	PassWord_DEV[1] = GUI_MEMDEV_CreateFixed(0,0, 264, 39,  GUI_MEMDEV_HASTRANS,GUI_MEMDEV_APILIST_32,GUI_COLOR_CONV_8888);
//	AddToPNG("华宁电子.png",PassWord_DEV[1],0,0);
	
}


void Init_Home_Create_memdev(void)
{
	uint8_t temp;
			//bejing
	BackGround_DEV = GUI_MEMDEV_CreateFixed(0, 0, 480, 320,  GUI_MEMDEV_HASTRANS,GUI_MEMDEV_APILIST_32,GUI_COLOR_CONV_8888);
	AddToPNG("主界面5.png",BackGround_DEV,0,0);	
	
	Group_DEV[0] = GUI_MEMDEV_CreateFixed(0, 0, 123, 38,  GUI_MEMDEV_HASTRANS,GUI_MEMDEV_APILIST_32,GUI_COLOR_CONV_8888);
	AddToPNG("成组左.png",Group_DEV[0],0,0);
	
	Group_DEV[1] = GUI_MEMDEV_CreateFixed(0, 0, 123, 38,  GUI_MEMDEV_HASTRANS,GUI_MEMDEV_APILIST_32,GUI_COLOR_CONV_8888);
	AddToPNG("成组右.png",Group_DEV[1],0,0);

	Group_DEV[2] = GUI_MEMDEV_CreateFixed(0, 0, 360, 4,  GUI_MEMDEV_HASTRANS,GUI_MEMDEV_APILIST_32,GUI_COLOR_CONV_8888);
	AddToPNG("成组装饰1.png",Group_DEV[2],0,0);	
	
	HomeFrame_DEV[0] = GUI_MEMDEV_CreateFixed(0, 0, 56, 4,  GUI_MEMDEV_HASTRANS,GUI_MEMDEV_APILIST_32,GUI_COLOR_CONV_8888);
	AddToPNG("主界面装饰1.png",HomeFrame_DEV[0],0,0);	
	HomeFrame_DEV[1] = GUI_MEMDEV_CreateFixed(0, 0, 349, 1,  GUI_MEMDEV_HASTRANS,GUI_MEMDEV_APILIST_32,GUI_COLOR_CONV_8888);
	AddToPNG("主界面装饰2.png",HomeFrame_DEV[1],0,0);	
	HomeFrame_DEV[2] = GUI_MEMDEV_CreateFixed(0, 0, 79, 37,  GUI_MEMDEV_HASTRANS,GUI_MEMDEV_APILIST_32,GUI_COLOR_CONV_8888);
	AddToPNG("主界面装饰3.png",HomeFrame_DEV[2],0,0);	
	HomeFrame_DEV[3] = GUI_MEMDEV_CreateFixed(0, 0, 360, 109,  GUI_MEMDEV_HASTRANS,GUI_MEMDEV_APILIST_32,GUI_COLOR_CONV_8888);
	HomeFrame_DEV[4] = GUI_MEMDEV_CreateFixed(0, 0, 341, 1,  GUI_MEMDEV_HASTRANS,GUI_MEMDEV_APILIST_32,GUI_COLOR_CONV_8888);
	AddToPNG("主界面装饰4.png",HomeFrame_DEV[4],0,0);
	
	SysDataFun(DATA_MSG_GET_DEVICETYPE_PARAM, 0,0,0,0,(void *)&temp,0);
	if(temp == 1)		
	{
		ShearerLeft_DEV1 = GUI_MEMDEV_CreateFixed(0,0,360,104, GUI_MEMDEV_HASTRANS,GUI_MEMDEV_APILIST_32,GUI_COLOR_CONV_8888);
		AddToPNG("liangzhuzhijia.png",ShearerLeft_DEV1,0,0);
		ShearerLeft_DEV2 = GUI_MEMDEV_CreateFixed(0,0,360,104, GUI_MEMDEV_HASTRANS,GUI_MEMDEV_APILIST_32,GUI_COLOR_CONV_8888);
		AddToPNG("liangzhuzhijia.png",ShearerLeft_DEV2,0,0);		
		ShearerRight_DEV1 = GUI_MEMDEV_CreateFixed(0,0,360,104, GUI_MEMDEV_HASTRANS,GUI_MEMDEV_APILIST_32,GUI_COLOR_CONV_8888);
		AddToPNG("liangzhuzhijia.png",ShearerRight_DEV1,0,0);
		ShearerRight_DEV2 = GUI_MEMDEV_CreateFixed(0,0,360,104, GUI_MEMDEV_HASTRANS,GUI_MEMDEV_APILIST_32,GUI_COLOR_CONV_8888);
		AddToPNG("liangzhuzhijia.png",ShearerRight_DEV2,0,0);
	}
	else if(temp == 2)		
	{
		ShearerLeft_DEV1 = GUI_MEMDEV_CreateFixed(0,0,360,104, GUI_MEMDEV_HASTRANS,GUI_MEMDEV_APILIST_32,GUI_COLOR_CONV_8888);
		AddToPNG("ziyijiwei.png",ShearerLeft_DEV1,0,0);
		ShearerLeft_DEV2 = GUI_MEMDEV_CreateFixed(0,0,360,104, GUI_MEMDEV_HASTRANS,GUI_MEMDEV_APILIST_32,GUI_COLOR_CONV_8888);
		AddToPNG("ziyijiwei.png",ShearerLeft_DEV2,0,0);		
		ShearerRight_DEV1 = GUI_MEMDEV_CreateFixed(0,0,360,104, GUI_MEMDEV_HASTRANS,GUI_MEMDEV_APILIST_32,GUI_COLOR_CONV_8888);
		AddToPNG("ziyijiwei.png",ShearerRight_DEV1,0,0);
		ShearerRight_DEV2 = GUI_MEMDEV_CreateFixed(0,0,360,104, GUI_MEMDEV_HASTRANS,GUI_MEMDEV_APILIST_32,GUI_COLOR_CONV_8888);
		AddToPNG("ziyijiwei.png",ShearerRight_DEV2,0,0);
	}
	else
	{
		ShearerLeft_DEV1 = GUI_MEMDEV_CreateFixed(0,0,360,104, GUI_MEMDEV_HASTRANS,GUI_MEMDEV_APILIST_32,GUI_COLOR_CONV_8888);
		AddToPNG("采煤机左1.png",ShearerLeft_DEV1,0,0);
		ShearerLeft_DEV2 = GUI_MEMDEV_CreateFixed(0,0,360,104, GUI_MEMDEV_HASTRANS,GUI_MEMDEV_APILIST_32,GUI_COLOR_CONV_8888);
		AddToPNG("采煤机左2.png",ShearerLeft_DEV2,0,0);		
		ShearerRight_DEV1 = GUI_MEMDEV_CreateFixed(0,0,360,104, GUI_MEMDEV_HASTRANS,GUI_MEMDEV_APILIST_32,GUI_COLOR_CONV_8888);
		AddToPNG("采煤机右1.png",ShearerRight_DEV1,0,0);
		ShearerRight_DEV2 = GUI_MEMDEV_CreateFixed(0,0,360,104, GUI_MEMDEV_HASTRANS,GUI_MEMDEV_APILIST_32,GUI_COLOR_CONV_8888);
		AddToPNG("采煤机右2.png",ShearerRight_DEV2,0,0);
	}
		
	Signal_DEV1 = GUI_MEMDEV_CreateFixed(0,0,32,23, GUI_MEMDEV_HASTRANS,GUI_MEMDEV_APILIST_32,GUI_COLOR_CONV_8888);
	AddToPNG("信号1.png",Signal_DEV1,0,0);
	Signal_DEV2 = GUI_MEMDEV_CreateFixed(0,0,32,23, GUI_MEMDEV_HASTRANS,GUI_MEMDEV_APILIST_32,GUI_COLOR_CONV_8888);
	AddToPNG("信号2.png",Signal_DEV2,0,0);
	Signal_DEV3 = GUI_MEMDEV_CreateFixed(0,0,32,23, GUI_MEMDEV_HASTRANS,GUI_MEMDEV_APILIST_32,GUI_COLOR_CONV_8888);
	AddToPNG("信号3.png",Signal_DEV3,0,0);
	
	DtrectionLeft_DEV0 = GUI_MEMDEV_CreateFixed(0,0,360,45, GUI_MEMDEV_HASTRANS,GUI_MEMDEV_APILIST_32,GUI_COLOR_CONV_8888);
	AddToPNG("方向左0.png",DtrectionLeft_DEV0,0,0);
	DtrectionLeft_DEV1 = GUI_MEMDEV_CreateFixed(0,0,360,45, GUI_MEMDEV_HASTRANS,GUI_MEMDEV_APILIST_32,GUI_COLOR_CONV_8888);
	AddToPNG("方向左1.png",DtrectionLeft_DEV1,0,0);
	DtrectionLeft_DEV2 = GUI_MEMDEV_CreateFixed(0,0,360,45, GUI_MEMDEV_HASTRANS,GUI_MEMDEV_APILIST_32,GUI_COLOR_CONV_8888);
	AddToPNG("方向左2.png",DtrectionLeft_DEV2,0,0);	
	DtrectionLeft_DEV3 = GUI_MEMDEV_CreateFixed(0,0,360,45, GUI_MEMDEV_HASTRANS,GUI_MEMDEV_APILIST_32,GUI_COLOR_CONV_8888);
	AddToPNG("方向左3.png",DtrectionLeft_DEV3,0,0);
	
	DtrectionRight_DEV0 = GUI_MEMDEV_CreateFixed(0,0,360,45, GUI_MEMDEV_HASTRANS,GUI_MEMDEV_APILIST_32,GUI_COLOR_CONV_8888);
	AddToPNG("方向右0.png",DtrectionRight_DEV0,0,0);
	DtrectionRight_DEV1 = GUI_MEMDEV_CreateFixed(0,0,360,45, GUI_MEMDEV_HASTRANS,GUI_MEMDEV_APILIST_32,GUI_COLOR_CONV_8888);
	AddToPNG("方向右1.png",DtrectionRight_DEV1,0,0);
	DtrectionRight_DEV2 = GUI_MEMDEV_CreateFixed(0,0,360,45, GUI_MEMDEV_HASTRANS,GUI_MEMDEV_APILIST_32,GUI_COLOR_CONV_8888);
	AddToPNG("方向右2.png",DtrectionRight_DEV2,0,0);	
	DtrectionRight_DEV3 = GUI_MEMDEV_CreateFixed(0,0,360,45, GUI_MEMDEV_HASTRANS,GUI_MEMDEV_APILIST_32,GUI_COLOR_CONV_8888);
	AddToPNG("方向右3.png",DtrectionRight_DEV3,0,0);
	
	
	TranSport_DEV = GUI_MEMDEV_CreateFixed(0,0,360,4, GUI_MEMDEV_HASTRANS,GUI_MEMDEV_APILIST_32,GUI_COLOR_CONV_8888);
	AddToPNG("前部运输机.png",TranSport_DEV,0,0);
	
	SuddenStop_DEV = GUI_MEMDEV_CreateFixed(0,0,360,163, GUI_MEMDEV_HASTRANS,GUI_MEMDEV_APILIST_32,GUI_COLOR_CONV_8888);
	AddToPNG("急停.png",SuddenStop_DEV,0,0);
}


void Boot_Sceen_Create(void)
{

	uint8_t temp;
	ee_ReadBytes(&temp, 0x1000-1, 1);
	if(temp == 0x0E)
	{
		temp = 0;
		ee_WriteBytes(&temp, 0x1000-1, 1);
		BackGround_DEV = GUI_MEMDEV_CreateFixed(0, 0, 480, 320,  GUI_MEMDEV_HASTRANS,GUI_MEMDEV_APILIST_32,GUI_COLOR_CONV_8888);
		GUI_MEMDEV_Select(BackGround_DEV);
		GUI_SetBkColor(GUI_BLUE);
		GUI_Clear();
		GUI_MEMDEV_Select(0);
	}
	else
	{
		PowerOn_DEV = GUI_MEMDEV_CreateFixed(0, 0, 264, 87,  GUI_MEMDEV_HASTRANS,GUI_MEMDEV_APILIST_32,GUI_COLOR_CONV_8888);
		AddToPNG("华宁电子图标.png",PowerOn_DEV,0,0);
		#if (BOOT_SCREEN_SELECTION == HNDZ)
		GUI_MEMDEV_WriteAt(PowerOn_DEV, 109, 100);
		#elif (BOOT_SCREEN_SELECTION == HONGMENG)
		PowerOn_DEV1 = GUI_MEMDEV_CreateFixed(0, 0, 480, 320,  GUI_MEMDEV_HASTRANS,GUI_MEMDEV_APILIST_32,GUI_COLOR_CONV_8888);
		if(AddToPNG("hongmeng.png",PowerOn_DEV1,0,0) == 0)
		{
			GUI_MEMDEV_WriteAt(PowerOn_DEV, 109, 100);
		}
		else
		{
			GUI_MEMDEV_WriteAt(PowerOn_DEV1, 0, 0);
		}

		#else
		#endif
		

		Home_Create_memdev();

		Init_Home_Create_memdev();
	}

	CreateBackGround();	

}
