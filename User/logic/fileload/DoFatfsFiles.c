/*
*********************************************************************************************************
*	                                            V4
*                           Commuication Controller For The Mining Industry
*
*                                  (c) Copyright 1994-2013  HNDZ 
*                       All rights reserved.  Protected by international copyright laws.
*
*
*    File    : DoFatfsFiles.c
*    Module  : user
*    Version : V1.0
*    History :
*   -----------------
*              Version  Date           By            Note
*              v1.0         
*
*********************************************************************************************************
*/
#include "includes.h"
#include "bsp.h"
#include "ff.h"			/* FatFS�ļ�ϵͳģ��*/
#include "DoFatfsFiles.h"
#include "V2_Can_Pro.h"

char *v4FatfsfileName[] = {"kzq","dcf","jd","ry","sg","gd","hw","jp","wx","area","rybl","sgbl","gdbl","dgns","Mhub"};//ghy ���Ӷ๦����������
static FATFS fs;
static FRESULT fr;
static DIR dir;
static FILINFO fno;
static DWORD fr_clust, fr_sect, tot_sect;
static FATFS *fs1;
static FIL fil;
static UINT byteMode = 0;
static UINT readfbyteMode = 0;
static uint32_t pointsread = 0;


uint8_t Fatfs_Mount(void)
{
	fr = f_mount(&fs,"",1);
	if (fr == 0)
		return 1;//printf("\nFatFs���سɹ�\n");
	else
		return 0;//printf("\nFatFs����ʧ��,�������(%d)\n", fr);
}

uint8_t Fatfs_UnMount(void)
{
	fr = f_mount(NULL,"",1);
	if (fr == 0)
		return 1;//printf("\nFatFs���سɹ�\n");
	else
		return 0;//printf("\nFatFs����ʧ��,�������(%d)\n", fr);
}



uint8_t Fatfs_Mkfs(void)
{
	fr = f_mkfs("0:",1,512);
		
	if (fr == 0)
		return 1;//printf("\n��ʽ���ɹ�\n");
	else
		return 0;//printf("\n��ʽ��ʧ��,�������(%d)\n", fr);
}



void Fatfs_Getfree(void)
{
	/* ��ȡ�豸��Ϣ�Ϳմش�С */
	fr = f_getfree("0:", &fr_clust, &fs1);

	/* ����õ��ܵ����������Ϳ��������� */
	tot_sect = (fs1->n_fatent - 2) * fs1->csize/2;
	fr_sect = fr_clust * fs1->csize/2;

	;//printf("\n�豸�ܿռ䣺%10lu KB��\n���ÿռ䣺  %10lu KB��\n",
		   //(unsigned long)tot_sect, (unsigned long)fr_sect);
}

void Fatfs_CreateDir(char *name)
{
	fr = f_opendir(&dir,"0:");

	fr = f_mkdir(name);
	if (fr == FR_OK)
	{
		;//printf("f_mkdir Dir Ok\r\n");
	}
	else if (fr == FR_EXIST)
	{
		;//printf("Dir Ŀ¼�Ѿ�����(%d)\r\n",  fr);
	}
	else
	{
		;//printf("f_mkdir Dir ʧ�� (%d)\r\n",  fr);
		return;
	}
}



uint8_t checkBinFileName(char *name)
{
	uint8_t i;
	for(i = 0; i<(sizeof(v4FatfsfileName)/sizeof(char *));i++)
	{
		if(name[0] == v4FatfsfileName[i][0])
			break;
	}
	if(i<16)
		return 1;
	else
		return 0;
}


char* getFatfsFileName(uint8_t num)
{
	uint8_t i;
	i=(sizeof(v4FatfsfileName)/sizeof(char *));
	if(num>i)
		return NULL;
	return v4FatfsfileName[num];
}


char resFileName[4][20];
uint8_t Fatfs_FindDir(char* name, char *res[], uint8_t fileordir,uint8_t type)
{
	uint8_t i = 0;
	uint8_t j = 0;
	fr = f_opendir(&dir,name);
	if(fr == FR_NO_PATH)
	{
		Fatfs_UnMount();
		Fatfs_Mount();
		fr = f_opendir(&dir,name);
	}
	memset(resFileName,0,sizeof(resFileName));
	for(;;)
	{
		fr = f_readdir(&dir,&fno);
		if(fr != FR_OK || fno.fname[0] == 0)
			break;		
		/* �ж����ļ�������Ŀ¼ */
		if (fno.fattrib & AM_DIR)
		{
			if(fileordir == 0)
			{
				memcpy(resFileName[i],fno.fname,sizeof(fno.fname));//printf("(0x%02d)Ŀ¼  ", fno.fattrib);
				res[i] = resFileName[i];
				i++;
			}
		}
		else
		{
			if(fileordir == 1)
			{
				for(j = 0; j<4; j++)
				{
					if(fno.fname[j] != *(v4FatfsfileName[type]+j))
						break;
					if(fno.fname[j]>0x30 && fno.fname[j]<0x39)
						break;
				}
				if(j >= 4)
				{
					memcpy(resFileName[i],fno.fname,sizeof(resFileName[i]));//printf("(0x%02d)�ļ�  ", fno.fattrib);
					res[i] = resFileName[i];
					i++;
				}
				else if(j == 3)
				{
					if((*(v4FatfsfileName[type]+2)) == 'q' || (*(v4FatfsfileName[type]+2)) == 'f')
					{
						memcpy(resFileName[i],fno.fname,sizeof(resFileName[i]));//printf("(0x%02d)�ļ�  ", fno.fattrib);
						res[i] = resFileName[i];
						i++;
					}
				}
				else if(j == 2)
				{
					if((*(v4FatfsfileName[type]+2)) == 'b')
					{
						if(fno.fname[2]!= 'b')
							continue;
					}
					else if(fno.fname[2] == 'b')
					{
						if((*(v4FatfsfileName[type]+2))!= 'b')
							continue;
					}						
					memcpy(resFileName[i],fno.fname,sizeof(resFileName[i]));//printf("(0x%02d)�ļ�  ", fno.fattrib);
					res[i] = resFileName[i];
					i++;
				}
			}
		}
	}
 f_closedir(&dir);
	return i;
}


int8_t Fatfs_isExistDir(char* path,char* name)
{
	fr = f_opendir(&dir,path);

	fr = f_mkdir(name);
	if (fr == FR_OK)
	{
		return 1;
	}
	else if (fr == FR_EXIST)
	{
		return 0;
	}
	else
	{		
		return -1;
	}
}


void Fatfs_readFile(char *name, uint8_t *data, uint32_t size)
{
	fr = f_lseek(&fil, pointsread);
	if (fr == 0)
		;//printf("\nƫ�ƹ��ɹ�,ƫ���� %d \n", (int)f_size(&fil));
	else
		;//printf("\nƫ�ƹ��ʧ��\n");
	pointsread+=size;
	/* ��ȡ�ļ� */
	fr = f_read(&fil, data, size, &readfbyteMode);
	if (readfbyteMode > 0)
	{
		;//printf("\r\narmfly.txt �ļ����� : \r\n%d\r\n", fr);
	}
	else
	{
		;//printf("\r\narmfly.txt �ļ����� : \r\n");
	}
}

uint8_t Fatfs_openDir(char *name)
{
	fr = f_opendir(&dir,name);
	return 0;
}
uint8_t Fatfs_createFile(char *name)
{
	pointsread = 0;
	memset(&fil,0,sizeof(fil));
	byteMode = 0;
	readfbyteMode = 0;
	fr = f_open(&fil, name, FA_CREATE_ALWAYS);
	if (fr == 0)
		return 1;//printf("\n�ļ��򿪳ɹ�\n");
	else
		return 0;//printf("\n�ļ���ʧ��,�������(%d)\n", fr);
}

uint32_t Fatfs_openFile(char *name,uint8_t isWrite)
{
	if(isWrite)
	{
		fr = f_open(&fil, name, FA_OPEN_EXISTING | FA_WRITE);
		if (fr == 0)
			return 1;//printf("\n�ļ��򿪳ɹ�\n");
		else
			return 0;//printf("\n�ļ���ʧ��,�������(%d)\n", fr);
	}
	else
	{
		fr = f_open(&fil, name, FA_OPEN_EXISTING | FA_READ);
		if (fr == 0)
			return f_size(&fil);//printf("\n�ļ��򿪳ɹ�\n");
		else
			return 0;//printf("\n�ļ���ʧ��,�������(%d)\n", fr);
	}
}


uint8_t Fatfs_writeFile(uint8_t *data, uint32_t size)
{
	fr = f_lseek(&fil, f_size(&fil));	
	fr = f_write(&fil, data, size, &byteMode);
	if (fr == 0)
		return 1;//printf("\n�ļ��򿪳ɹ�\n");
	else
		return 0;//printf("\n�ļ���ʧ��,�������(%d)\n", fr);
}

void Fatfs_closeFile()
{
	pointsread = 0;
	f_close(&fil);
}



void DeleteDirFile(char* name)
{
//	fr = f_opendir(&dir,name);
//	for(;;)
//	{
//		fr = f_readdir(&dir,&fno);
//		if(fr != FR_OK || fno.fname[0] == 0)
//			break;		
//		/* �ж����ļ�������Ŀ¼ */
//		if (fno.fattrib & AM_DIR)
//		{
//			fr = f_unlink(fno.fname);
//		}
//		else
//		{
				fr = f_unlink(name);
//		}
//	}	
}

char ceshiname[50];
uint8_t Fatfs_DeleteDirFile(char* name)
{
	uint8_t i = 0;
	uint8_t j = 0;
	fr = f_opendir(&dir,name);
	if(fr == FR_NO_PATH)
	{
		Fatfs_UnMount();
		Fatfs_Mount();
		fr = f_opendir(&dir,name);
	}
	memset(resFileName,0,sizeof(resFileName));
	for(;;)
	{
		fr = f_readdir(&dir,&fno);
		if(fr != FR_OK || fno.fname[0] == 0)
			break;		
		/* �ж����ļ�������Ŀ¼ */
		if (fno.fattrib & AM_DIR)
		{
			;
		}
		else
		{
			sprintf(ceshiname,"%s%s","0:update/",fno.fname);
			fr = f_unlink(ceshiname);
		}
	}
 f_closedir(&dir);
	return i;
}







