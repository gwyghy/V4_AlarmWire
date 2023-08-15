/*
*********************************************************************************************************
*	                                            DoubleWires
*                           Commuication Controller For The Mining Industry
*
*                                  (c) Copyright 1994-2013  HNDZ
*                       All rights reserved.  Protected by international copyright laws.
*
*
*    File    : bsp_fm24cl64.c
*    Module  : user
*    Version : V1.0
*    History :
*   -----------------
*              Version  Date           By            Note
*              v1.0     2013-09-17    辛鑫
*
*********************************************************************************************************
*/

/*********************************************************************************************/
/*                                          程序包含的头文件                                 */
/*********************************************************************************************/
#include "stm32f4xx.h"

#include "bsp_plat_data.h"
#include "bsp_i2c_gpio.h"
#include "bsp_fm24cl64.h"

#include <stdio.h>
/* Private function prototypes -----------------------------------------------*/

/*********************************************************************************************/
/*                                          宏定义                                         */
/*********************************************************************************************/
#define Wait                   1000
#define I2C_WP_PIN	     GPIO_Pin_2	
/*********************************************************************************************/
/*                                          全局变量                                         */
/*********************************************************************************************/
uint8_t CRC_TABLE[]=
{0x00,0x31,0x62,0x53,0xC4,0xF5,0xA6,0x97,0xB9,0x88,0xDB,0xEA,0x7D,0x4C,0x1F,0x2E};

static int8_t FM24CL64_SendAddress(uint16_t address);

static void FM24CL64_IoInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;		/* 设为输出口 */
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;		/* 设为开漏模式 */
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;	/* 上下拉电阻不使能 */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	/* IO口最大速度 */

	GPIO_InitStructure.GPIO_Pin = I2C_WP_PIN;
	GPIO_Init(GPIO_PORT_I2C, &GPIO_InitStructure);
	
	FM24CL64_WriteUnProtect();                          // 解除写保护
}

int BSP_InitFM24CL64(void)
{
    uint8_t res;
    
	FM24CL64_IoInit();
    if (i2c_CheckDevice(FM24_SLAVE_ADDRESS) == 0)	/* 这个函数会配置STM32的GPIO用于软件模拟I2C时序 */
	{
		res = 1;
	}
	else
	{
		res = 0;
	}

	return res;
}

/*********************************************************
** 函数名：uint8_t FM24CL64_CRC8(uint16_t PData)
** 输　入：uint16_t PData
** 输　出：uint8_t
** 功能描述：8位CRC校验函数
** 作　者：辛鑫
** 版本：V1.0
************************************************************/
uint8_t FM24CL64_CRC8(uint16_t PData)
{
   uint8_t CRC_Temp = 0;
   uint8_t Temp;
   uint8_t PData_H = 0;
   uint8_t PData_L = 0;

   PData_H = PData >> 8;
   PData_L = (uint8_t)PData;

   Temp = CRC_Temp >> 4;
   CRC_Temp <<= 4;
   CRC_Temp ^= CRC_TABLE[Temp^(PData_H>>4)];

   Temp = CRC_Temp >> 4;
   CRC_Temp <<= 4;
   CRC_Temp ^= CRC_TABLE[Temp^(PData_H&0x0F)];

   Temp = CRC_Temp >> 4;
   CRC_Temp <<= 4;
   CRC_Temp ^= CRC_TABLE[Temp^(PData_L>>4)];

   Temp = CRC_Temp >> 4;
   CRC_Temp <<= 4;
   CRC_Temp ^= CRC_TABLE[Temp^(PData_L&0x0F)];

   return (CRC_Temp);
}

int8_t FM24CL64_ReadByte(uint16_t Address, uint8_t *dat)
{
    int8_t  res;
	uint8_t ucAck;

    if (Address > FM24_LAST_ADDR)
    {
        return (ERR_OUTADDR);
    }

    res = FM24CL64_SendAddress(Address);
	if (res < 0)
	{
		return res;
	}
    
	/* 发送起始位 */
    i2c_Start();

    /* 发送设备地址+读写控制bit（0 = w， 1 = r) bit7 先传 */
	i2c_SendByte(FM24_SLAVE_ADDRESS | I2C_RD);	
	/* 检测ACK */
	ucAck = i2c_WaitAck();
	if (ucAck)
	{
		return (ERR_NOACK);
	}
	
    *dat = i2c_ReadByte();
    /* 发送STOP */
	i2c_Stop();

    return 1;
}

static int8_t FM24CL64_SendAddress(uint16_t address)
{
	uint8_t ucAck, addr_msb, addr_lsb;

    addr_msb = (uint8_t)(address>>8);
    addr_lsb = (uint8_t) address;
    /* 发送起始位 */
    i2c_Start();

    /* 发送设备地址+读写控制bit（0 = w， 1 = r) bit7 先传 */
	i2c_SendByte(FM24_SLAVE_ADDRESS | I2C_WR);
	/* 检测ACK */
	ucAck = i2c_WaitAck();
	if (ucAck)
	{
		return (ERR_NOACK);
	}

	i2c_SendByte(addr_msb);	                // 发送地址高字节
	/* 检测ACK */
	ucAck = i2c_WaitAck();
	if (ucAck)
	{
		return (ERR_NOACK);
	}
	
	i2c_SendByte(addr_lsb);					// 发送地址低字节
    /* 检测ACK */
	ucAck = i2c_WaitAck();
	if (ucAck)
	{
		return (ERR_NOACK);
	}
	
	return 1;
}

/*********************************************************
** 函数名：void AT24C08_WriteByte(unsigned short Address,unsigned char dat)
** 输　入：unsigned short Address,unsigned char dat
** 输　出：void
** 功能描述：向指定地址写入一个字节数据
** 作　者：辛鑫
** 日　期：2012-01-12
************************************************************/
int8_t FM24CL64_WriteByte(uint16_t Address, uint8_t dat)
{
    int8_t  res;
	uint8_t ucAck;

	if (Address > FM24_LAST_ADDR)
    {
        return (ERR_OUTADDR);
    }
	
    res = FM24CL64_SendAddress(Address);
	if (res < 0)
	{
		return res;
	}

	i2c_SendByte(dat); 						// 发送数据字节
	/* 检测ACK */
	ucAck = i2c_WaitAck();
	if (ucAck)
	{
		return (ERR_NOACK);
	}

	/* 发送STOP */
	i2c_Stop();
    return 1;
}

/*********************************************************
** 函数名：int8_t FM24CL64_WriteMultBytes(uint16_t Address, uint8_t *dat, uint16_t num)
** 输　入：uint16_t Address, uint8_t *dat, uint16_t num
** 输　出：int8_t
** 功能描述：向指定地址写入num个字节(对多16个字节)数据
** 作　者：辛鑫
** 日　期：2012-01-12
************************************************************/
int8_t FM24CL64_WriteMultBytes(uint16_t Address, uint8_t *dat, uint16_t num)
{
    int8_t  res;
	uint8_t ucAck;

    if (Address+num > FM24_LAST_ADDR)
    {
        return (ERR_OUTADDR);
    }

    res = FM24CL64_SendAddress(Address);
	if (res < 0)
	{
		return res;
	}

	while (num)
    {
        i2c_SendByte(*dat); 			    // 发送数据字节
	    /* 检测ACK */
	    ucAck = i2c_WaitAck();
	    if (ucAck)
	    {
		    return (ERR_NOACK);
	    }

        num--;
		dat++;
	}

    /* 发送STOP */
	i2c_Stop();
    return 1;
}

int8_t FM24CL64_CurReadByte(uint8_t *dat)
{
    uint8_t ucAck;

    /* 发送起始位 */
    i2c_Start();

    /* 发送设备地址+读写控制bit（0 = w， 1 = r) bit7 先传 */
	i2c_SendByte(FM24_SLAVE_ADDRESS | I2C_RD);

	/* 检测ACK */
	ucAck = i2c_WaitAck();
	if (ucAck)
	{
		return (ERR_NOACK);
	}

    *dat = i2c_ReadByte();
    /* 发送STOP */
	i2c_Stop();

    return 1;
}

int8_t FM24CL64_CurReadMultBytes(uint8_t *dat, uint16_t num)
{
    uint8_t ucAck;

    /* 发送起始位 */
    i2c_Start();

    /* 发送设备地址+读写控制bit（0 = w， 1 = r) bit7 先传 */
	i2c_SendByte(FM24_SLAVE_ADDRESS | I2C_RD);

	/* 检测ACK */
	ucAck = i2c_WaitAck();
	if (ucAck)
	{
		return (ERR_NOACK);
	}

    while (num)
    {
        *dat = i2c_ReadByte();
        if (num != 1)                 // 最后一个字节没有ACK
        {
            /* 给出应答ACK */
	        i2c_Ack();
        }

        num--;
		dat++;
    }
    /* 发送STOP */
	i2c_Stop();

    return 1;
}

/*********************************************************
** 函数名：unsigned char AT24C08_ReadBytes(unsigned short n,unsigned short Address,unsigned char *dat)
** 输　入：unsigned char n,unsigned short Address,unsigned char *dat
** 输　出：unsigned char
** 功能描述：从指定地址读取n个字节数据
** 作　者：辛鑫
** 日　期：2012-01-12
************************************************************/
int8_t FM24CL64_ReadMultBytes(uint16_t Address, uint8_t *dat, uint16_t num)
{
	int8_t  res;
	uint8_t ucAck;

    if (Address+num > FM24_LAST_ADDR)
    {
        return (ERR_OUTADDR);
    }

    res = FM24CL64_SendAddress(Address);
	if (res < 0)
	{
		return res;
	}
    
	/* 发送起始位 */
    i2c_Start();

    /* 发送设备地址+读写控制bit（0 = w， 1 = r) bit7 先传 */
	i2c_SendByte(FM24_SLAVE_ADDRESS | I2C_RD);	
	/* 检测ACK */
	ucAck = i2c_WaitAck();
	if (ucAck)
	{
		return (ERR_NOACK);
	}
	
	while (num)
    {
        *dat = i2c_ReadByte();
        if (num != 1)                 // 最后一个字节没有ACK
        {
            /* 给出应答ACK */
	        i2c_Ack();
        }

        num--;
		dat++;
    }
    /* 发送STOP */
	i2c_Stop();
	
	return 1;
}

void FM24CL64_WriteProtect(void)
{
	GPIO_PORT_I2C->BSRRL = I2C_WP_PIN;
}

void FM24CL64_WriteUnProtect(void)
{
	GPIO_PORT_I2C->BSRRH = I2C_WP_PIN;
}
