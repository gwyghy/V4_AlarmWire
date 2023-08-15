/*
*********************************************************************************************************
*	                                            DoubleWires
*                           Commuication Controller For The Mining Industry
*
*                                  (c) Copyright 1994-2013  HNDZ 
*                       All rights reserved.  Protected by international copyright laws.
*
*
*    File    : bsp.h
*    Module  : user
*    Version : V1.0
*    History :
*   -----------------
*              Version  Date           By            Note
*              v1.0     2013-08-13     
*
*********************************************************************************************************
*/

#ifndef _BSP_H_
#define _BSP_H_

#include "stm32f4xx.h"

#include <stdio.h>
#include <stdint.h>
#include <mem.h>
#include <string.h>

#include "os_cpu.h"
#include "os_api.h"

/*
*********************************************************************************************************
*                                            EXTERNS
*********************************************************************************************************
*/
#ifdef   BSP_MODULE
#define  BSP_EXT
#else
#define  BSP_EXT								extern
#endif

#include "bsp_plat_data.h"

#include "bsp_shell.h"
#include "bsp_timer.h"
#include "bsp_key.h"
#include "bsp_beep.h"
#include "bsp_iwdg.h"
#include "bsp_fsmc_flash.h"
#include "bsp_nand_flash.h"
#include "bsp_fm24cl64.h"
#include "bsp_touch.h"
#include "bsp_rs485.h"
#include "bsp_spi_551.h"
#include "bsp_time.h"
#include "bsp_rtc.h"
#include "bsp_spi_norflash.h"
#include "bsp_spi_adc.h"
#include "bsp_adc.h"

#include "LCD_RA8875.h"
#include "LCD_SPFD5420.h"
#include "LCD_ILI9488.h"
#include "bsp_ra8875_port.h"
#include "bsp_tft_lcd.h"
#include "bsp_ra8875_flash.h"
#include  "bsp_dwt.h"
#include "bsp_tim_pwm.h"
#include "bsp_spi_bus.h"
#include "bsp_sdram.h"
#include "OsEvent.h"
#include "bsp_eeprom_24xx.h"
#include "bsp_i2c_gpio.h"
#include "bsp_jtbs.h"
#include "bsp_leds.h"
#include "bsp_nor_flash.h"


/*
*********************************************************************************************************
*                                           SHELL DEFINE
*********************************************************************************************************
*/
#define  SHELL_VERSION							"Ver 1.0"
#define  SHELL_OS								"OSII-V2.91"
#define  SHELL_CPU								"STM32F429"
#define  SHELL_DEVICE							"Double Wires"
#define  SHELL_BOARD							"CPU Ver1.0"

/*
*********************************************************************************************************
*                                           ERROR TYPE DEFINE
*********************************************************************************************************
*/
#define  LED_ERROR								01
#define  KEY_ERROR								02
#define  RTC_ERROR                              03
#define  FLASH_ERROR							04
#define  WM8978_ERROR                           05
#define  FM24_ERROR                             06
#define	 RS485_ERROR							07
/*
*********************************************************************************************************
*                                           TYPE DEFINES
*********************************************************************************************************
*/

typedef            void        CPU_VOID;
typedef            char        CPU_CHAR;                        /*  8-bit character                                     */
typedef  unsigned  char        CPU_BOOLEAN;                     /*  8-bit boolean or logical                            */
typedef  unsigned  char        CPU_INT08U;                      /*  8-bit unsigned integer                              */
typedef    signed  char        CPU_INT08S;                      /*  8-bit   signed integer                              */
typedef  unsigned  short       CPU_INT16U;                      /* 16-bit unsigned integer                              */
typedef    signed  short       CPU_INT16S;                      /* 16-bit   signed integer                              */
typedef  unsigned  int         CPU_INT32U;                      /* 32-bit unsigned integer                              */
typedef    signed  int         CPU_INT32S;                      /* 32-bit   signed integer                              */
typedef  unsigned  long  long  CPU_INT64U;                      /* 64-bit unsigned integer                              */
typedef    signed  long  long  CPU_INT64S;                      /* 64-bit   signed integer                              */

typedef            float       CPU_FP32;                        /* 32-bit floating point                                */
typedef            double      CPU_FP64;                        /* 64-bit floating point                                */


typedef  volatile  CPU_INT08U  CPU_REG08;                       /*  8-bit register                                      */
typedef  volatile  CPU_INT16U  CPU_REG16;                       /* 16-bit register                                      */
typedef  volatile  CPU_INT32U  CPU_REG32;                       /* 32-bit register                                      */
typedef  volatile  CPU_INT64U  CPU_REG64;                       /* 64-bit register                                      */

typedef  CPU_INT32U  CPU_DATA;
typedef  CPU_INT32U  CPU_SR; 


typedef            void      (*CPU_FNCT_VOID)(void);            /* See Note #2a.                                        */
typedef            void      (*CPU_FNCT_PTR )(void *p_obj);     /* See Note #2b.                                        */

/*
*********************************************************************************************************
*                                           FUNCTION PROTOTYPES
*********************************************************************************************************
*/
void        BSP_Init                            (void);

/**
	bsp_int.c
*/
void        NMI_Handler                         (void);
void        HardFault_Handler                   (void);
void        MemManage_Handler                   (void);
void        BusFault_Handler                    (void);
void        UsageFault_Handler                  (void);
void        SVC_Handler                         (void);
void        DebugMon_Handler                    (void);
void        SDIO_IRQHandler                     (void);
void        SD_SDIO_DMA_IRQHANDLER              (void);

void        BSP_IntClr                          (IRQn_Type int_id);
void        BSP_IntDis                          (IRQn_Type int_id);
void        BSP_IntEn                           (IRQn_Type int_id);
void        BSP_IntVectSet                      (IRQn_Type int_id,  CPU_FNCT_VOID  isr);
void        BSP_IntPrioSet                      (IRQn_Type int_id,  uint32_t       prio);
void        BSP_IntInit                         (void);

void        WWDG_IRQHandler                     (void);
void        PVD_IRQHandler                      (void);
void        TAMP_STAMP_IRQHandler               (void);
void        RTC_WKUP_IRQHandler                 (void);
void        FLASH_IRQHandler                    (void);
void        RCC_IRQHandler                      (void);
void        EXTI0_IRQHandler                    (void);
void        EXTI1_IRQHandler                    (void);
void        EXTI2_IRQHandler                    (void);
void        EXTI3_IRQHandler                    (void);
void        EXTI4_IRQHandler                    (void);
void        DMA1_Stream0_IRQHandler             (void);
void        DMA1_Stream1_IRQHandler             (void);
void        DMA1_Stream2_IRQHandler             (void);
void        DMA1_Stream3_IRQHandler             (void);
void        DMA1_Stream4_IRQHandler             (void);
void        DMA1_Stream5_IRQHandler             (void);
void        DMA1_Stream6_IRQHandler             (void);
void        ADC_IRQHandler                      (void);
void        CAN1_TX_IRQHandler                  (void);
void        CAN1_RX0_IRQHandler                 (void);
void        CAN1_RX1_IRQHandler                 (void);
void        CAN1_SCE_IRQHandler                 (void);
void        EXTI9_5_IRQHandler                  (void);
void        TIM1_BRK_TIM9_IRQHandler            (void);
void        TIM1_UP_TIM10_IRQHandler            (void);
void        TIM1_TRG_COM_TIM11_IRQHandler       (void);
void        TIM1_CC_IRQHandler                  (void);
void        TIM2_IRQHandler                     (void);
void        TIM3_IRQHandler                     (void);
void        TIM4_IRQHandler                     (void);
void        I2C1_EV_IRQHandler                  (void);
void        I2C1_ER_IRQHandler                  (void);
void        I2C2_EV_IRQHandler                  (void);
void        I2C2_ER_IRQHandler                  (void);
void        SPI1_IRQHandler                     (void);
void        SPI2_IRQHandler                     (void);
void        USART1_IRQHandler                   (void);
void        USART2_IRQHandler                   (void);
void        USART3_IRQHandler                   (void);
void        EXTI15_10_IRQHandler                (void);
void        RTC_Alarm_IRQHandler                (void);
void        OTG_FS_WKUP_IRQHandler              (void);
void        TIM8_BRK_TIM12_IRQHandler           (void);
void        TIM8_UP_TIM13_IRQHandler            (void);
void        TIM8_TRG_COM_TIM14_IRQHandler       (void);
void        TIM8_CC_IRQHandler                  (void);
void        DMA1_Stream7_IRQHandler             (void);
void        FSMC_IRQHandler                     (void);
void        SDIO_IRQHandler                     (void);
void        TIM5_IRQHandler                     (void);
void        SPI3_IRQHandler                     (void);
void        UART4_IRQHandler                    (void);
void        UART5_IRQHandler                    (void);
void        TIM6_DAC_IRQHandler                 (void);
void        TIM7_IRQHandler                     (void);
void        DMA2_Stream0_IRQHandler             (void);
void        DMA2_Stream1_IRQHandler             (void);
void        DMA2_Stream2_IRQHandler             (void);
void        DMA2_Stream3_IRQHandler             (void);
void        DMA2_Stream4_IRQHandler             (void);
void        ETH_IRQHandler                      (void);
void        ETH_WKUP_IRQHandler                 (void);
void        CAN2_TX_IRQHandler                  (void);
void        CAN2_RX0_IRQHandler                 (void);
void        CAN2_RX1_IRQHandler                 (void);
void        CAN2_SCE_IRQHandler                 (void);
void        OTG_FS_IRQHandler                   (void);
void        DMA2_Stream5_IRQHandler             (void);
void        DMA2_Stream6_IRQHandler             (void);
void        DMA2_Stream7_IRQHandler             (void);
void        USART6_IRQHandler                   (void);
void        I2C3_EV_IRQHandler                  (void);
void        I2C3_ER_IRQHandler                  (void);
void        OTG_HS_EP1_OUT_IRQHandler           (void);
void        OTG_HS_EP1_IN_IRQHandler            (void);
void        OTG_HS_WKUP_IRQHandler              (void);
void        OTG_HS_IRQHandler                   (void);
void        DCMI_IRQHandler                     (void);
void        CRYP_IRQHandler                     (void);
void        HASH_RNG_IRQHandler                 (void);
void        FPU_IRQHandler                      (void);

/**
	bsp_led.c
*/
int         BSP_LedInit                         (void);
void        BSP_LedOn                           (uint8_t _no);
void        BSP_LedOff                          (uint8_t _no);
void        BSP_LedToggle                       (uint8_t _no);
uint8_t     BSP_IsLedOn                         (uint8_t _no);

/**
	bsp_shell.c
*/
int         BSP_ShellInit                        (void);

/**
	bsp_key.c
*/

u32 Get_MCU_ID(uint8_t* pMcuID);
void CreateSysMAC(uint8_t* pMacBuf);

#endif
