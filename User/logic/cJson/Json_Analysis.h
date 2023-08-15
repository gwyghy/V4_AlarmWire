/*
*********************************************************************************************************
*	                                            V4
*                           Commuication Controller For The Mining Industry
*
*                                  (c) Copyright 1994-2013  HNDZ 
*                       All rights reserved.  Protected by international copyright laws.
*
*
*    File    : Can_Pro.c
*    Module  : user
*    Version : V1.0
*    History :
*   -----------------
*              Version  Date           By            Note
*              v1.0         
*
*********************************************************************************************************
*/

#ifndef __JSON_ANALYSIS_H__
#define __JSON_ANALYSIS_H__

#include "includes.h"
#include "cJSON.h"

#define namesize  12

typedef struct 
{
	uint8_t  name[namesize];
}STR_V4PARA_NAME;


void setParaDataProc(uint8_t * data,uint8_t len);
uint32_t getParaAddress(void);
uint8_t* getParaBufferAddress(void);
uint8_t* setParaBufferAddress(void);
uint8_t* getJsonParaAddress(uint64_t levels);
uint8_t getJsonMenuName(uint64_t levels,char ** name);
uint8_t getJsonParaStruct(uint64_t levels,STR_V4PARA_NAME** childname);
uint8_t getJsonParaChildSize(uint64_t levels);
uint8_t AnalysisJsonProc(char * name);
uint8_t getJsonParaStructSize(uint64_t levels);
uint8_t getJsonParaIsChilds(uint64_t levels);
uint8_t AnalysisInsideJsonProc(void);

void saveJsonToFatfs(void);
uint8_t ModifyTheLevel(uint64_t levels,uint8_t* sent);
#endif

