/*
*********************************************************************************************************
*	                                            DoubleWires
*                           Commuication Controller For The Mining Industry
*
*                                  (c) Copyright 1994-2013  HNDZ
*                       All rights reserved.  Protected by international copyright laws.
*
*
*    File    : bsp_fm24cl64.h
*    Module  : user
*    Version : V1.0
*    History :
*   -----------------
*              Version  Date           By            Note
*              v1.0     2013-09-05    辛鑫
*
*********************************************************************************************************
*/
#ifndef __BSP_FM24CL64_H
#define __BSP_FM24CL64_H

#include "stm32f4xx.h"

#define FM24_SLAVE_ADDRESS    0xA0	             /* FM24CL64 I2C从机地址 */
#define FM24_LAST_ADDR      0x1FFF               // 最大地址8192

#define ERR_NOACK               -1               // 无ACK应答
#define ERR_OUTADDR             -2               // 地址越界

int  BSP_InitFM24CL64(void);
void FM24CL64_WriteProtect(void);
void FM24CL64_WriteUnProtect(void);

int8_t FM24CL64_ReadByte(uint16_t Address, uint8_t *dat);
int8_t FM24CL64_WriteByte(uint16_t Address, uint8_t dat);
int8_t FM24CL64_ReadMultBytes(uint16_t Address, uint8_t *dat, uint16_t num);
int8_t FM24CL64_WriteMultBytes(uint16_t Address, uint8_t *dat, uint16_t num);

int8_t FM24CL64_CurReadByte(uint8_t *dat);
int8_t FM24CL64_CurReadMultBytes(uint8_t *dat, uint16_t num);

#endif

