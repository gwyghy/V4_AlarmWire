/*
*********************************************************************************************************
*
*	ģ������ : TFTҺ����ʾ������ģ��
*	�ļ����� : LCD_ILI9488.h
*	��    �� : V1.0
*	˵    �� : ͷ�ļ�
*
*	Copyright (C), 2014-2015, ���������� www.armfly.com
*
*********************************************************************************************************
*/


#ifndef _LCD_ILI9488_H
#define _LCD_ILI9488_H

#include "stm32f4xx.h"

#define ILI9488_BASE       ((uint32_t)(0x60000000 | 0x00000000))

#define ILI9488_DMA		(ILI9488_BASE + (1 << (18 + 1)))
#define ILI9488_REG		*(__IO uint16_t *)(ILI9488_BASE)
#define ILI9488_RAM		*(__IO uint16_t *)(ILI9488_BASE + (1 << (17)))	/* FSMC 16λ����ģʽ�£�FSMC_A18���߶�Ӧ������ַA19 */


/* �ɹ��ⲿģ����õĺ��� */
uint32_t ILI9488_ReadID(void);
void ILI9488_InitHard(uint8_t type);
void ILI9488_DispOn(void);
void ILI9488_DispOff(void);
void ILI9488_ClrScr(uint16_t _usColor);
void ILI9488_PutPixel(uint16_t _usX, uint16_t _usY, uint16_t _usColor);
uint16_t ILI9488_GetPixel(uint16_t _usX, uint16_t _usY);
void ILI9488_DrawLine(uint16_t _usX1 , uint16_t _usY1 , uint16_t _usX2 , uint16_t _usY2 , uint16_t _usColor);
void ILI9488_DrawHLine(uint16_t _usX1 , uint16_t _usY1 , uint16_t _usX2 , uint16_t _usColor);
void ILI9488_DrawHColorLine(uint16_t _usX1 , uint16_t _usY1, uint16_t _usWidth, const uint16_t *_pColor);
void ILI9488_DrawHTransLine(uint16_t _usX1 , uint16_t _usY1, uint16_t _usWidth, const uint16_t *_pColor);
void ILI9488_DrawRect(uint16_t _usX, uint16_t _usY, uint16_t _usHeight, uint16_t _usWidth, uint16_t _usColor);
void ILI9488_FillRect(uint16_t _usX, uint16_t _usY, uint16_t _usHeight, uint16_t _usWidth, uint16_t _usColor);
void ILI9488_DrawCircle(uint16_t _usX, uint16_t _usY, uint16_t _usRadius, uint16_t _usColor);
void ILI9488_DrawBMP(uint16_t _usX, uint16_t _usY, uint16_t _usHeight, uint16_t _usWidth, uint16_t *_ptr);

void ILI9488_SetBackLight(uint8_t _bright);
void ILI9488_SetDirection(uint8_t _ucDir);

void ILI9488_DrawVLine(uint16_t _usX1 , uint16_t _usY1 , uint16_t _usY2 , uint16_t _usColor);
#endif

/***************************** ���������� www.armfly.com (END OF FILE) *********************************/