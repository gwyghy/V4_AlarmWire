/*
*********************************************************************************************************
*	                                            V4
*                           Commuication Controller For The Mining Industry
*
*                                  (c) Copyright 1994-2013  HNDZ 
*                       All rights reserved.  Protected by international copyright laws.
*
*
*    File    : Can_Driver.h
*    Module  : user
*    Version : V1.0
*    History :
*   -----------------
*              Version  Date           By            Note
*              v1.0         
*
*********************************************************************************************************
*/

#ifndef __DOFATFSFILE_H__
#define __DOFATFSFILE_H__

uint8_t Fatfs_Mount(void);
uint8_t Fatfs_UnMount(void);
uint8_t Fatfs_Mkfs(void);
void Fatfs_CreateDir(char *name);
uint8_t Fatfs_openDir(char *name);
uint8_t Fatfs_FindDir(char* name, char *res[], uint8_t fileordir,uint8_t type);
void Fatfs_readFile(char *name, uint8_t *data, uint32_t size);
uint8_t Fatfs_createFile(char *name);
uint32_t Fatfs_openFile(char *name,uint8_t isWrite);
uint8_t Fatfs_writeFile(uint8_t *data, uint32_t size);
void Fatfs_closeFile(void);
void DeleteDirFile(char *name);
int8_t Fatfs_isExistDir(char* path,char* name);
uint8_t checkBinFileName(char *name);
char* getFatfsFileName(uint8_t num);

#endif


