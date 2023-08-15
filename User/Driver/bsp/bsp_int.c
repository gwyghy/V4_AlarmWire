/*
*********************************************************************************************************
*	                                            DoubleWires
*                           Commuication Controller For The Mining Industry
*
*                                  (c) Copyright 1994-2013  HNDZ 
*                       All rights reserved.  Protected by international copyright laws.
*
*
*    File    : bsp_init.c
*    Module  : user
*    Version : V1.0
*    brief   : Main Interrupt Service Routines.
*              This file provides template for all exceptions handler and peripherals interrupt service routine.
*    History :
*   -----------------
*              Version  Date           By            Note
*              v1.0     2013-08-19     
*
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/
#include  "includes.h"

#include  "bsp.h"

/*
*********************************************************************************************************
*                                            LOCAL TABLES
*********************************************************************************************************
*/
static  CPU_FNCT_VOID  BSP_IntVectTbl[IRQn_NUM];    // IRQn_NUM need add to IRQn_Type <stm32fxx.h> 


/********************************************************************************************************
  * Description : This function handles NMI exception.
  * Argument(s) : none.
  * Return(s)   : none.
  * Caller(s)   : startup_stm32f4xx.s
  * Note(s)     : 
*********************************************************************************************************/
void NMI_Handler(void)
{
    while (1)
	{
	    ;
	}
}

/********************************************************************************************************
  * Description : This function handles Hard Fault exception.
  * Argument(s) : none.
  * Return(s)   : none.
  * Caller(s)   : startup_stm32f4xx.s
  * Note(s)     : 
*********************************************************************************************************/
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
	  ;
  }
}

/********************************************************************************************************
  * Description : This function handles Memory Manage exception.
  * Argument(s) : none.
  * Return(s)   : none.
  * Caller(s)   : startup_stm32f4xx.s
  * Note(s)     : 
*********************************************************************************************************/
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
	  ;
  }
}

/********************************************************************************************************
  * Description : This function handles Bus Fault exception.
  * Argument(s) : none.
  * Return(s)   : none.
  * Caller(s)   : startup_stm32f4xx.s
  * Note(s)     : 
*********************************************************************************************************/
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
	  ;
  }
}

/********************************************************************************************************
  * Description : This function handles Usage Fault exception.
  * Argument(s) : none.
  * Return(s)   : none.
  * Caller(s)   : startup_stm32f4xx.s
  * Note(s)     : 
*********************************************************************************************************/
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {  	
  }
}

/********************************************************************************************************
  * Description : This function handles SVCall exception.
  * Argument(s) : none.
  * Return(s)   : none.
  * Caller(s)   : startup_stm32f4xx.s
  * Note(s)     : 
*********************************************************************************************************/
void SVC_Handler(void)
{
	
}

/********************************************************************************************************
  * Description : This function handles Debug Monitor exception.
  * Argument(s) : none.
  * Return(s)   : none.
  * Caller(s)   : startup_stm32f4xx.s
  * Note(s)     : 
*********************************************************************************************************/
void DebugMon_Handler(void)
{
	
}

/********************************************************************************************************
  * Description : Clear interrupt.
  * Argument(s) : int_id      Interrupt to clear.
  * Return(s)   : none.
  * Caller(s)   : Application.
  * Note(s)     : (1) An interrupt does not need to be cleared within the interrupt controller.
*********************************************************************************************************/
void  BSP_IntClr (IRQn_Type  int_id)
{

}

/********************************************************************************************************
 Description : Disable interrupt.
 Argument(s) : int_id      Interrupt to disable.
 Return(s)   : none.
 Caller(s)   : Application.
 Note(s)     : none.
*********************************************************************************************************/
void  BSP_IntDis (IRQn_Type  int_id)
{
	NVIC_DisableIRQ(int_id);
}

/********************************************************************************************************
 Description : Enable interrupt.
 Argument(s) : int_id      Interrupt to enable.
 Return(s)   : none.
 Caller(s)   : Application.
 Note(s)     : none.
*********************************************************************************************************/
void  BSP_IntEn (IRQn_Type  int_id)
{
    NVIC_EnableIRQ(int_id);
}

/********************************************************************************************************
 Description : Assign ISR handler.
 Argument(s) : int_id      Interrupt for which vector will be set.
               isr         Handler to assign
 Return(s)   : none.
 Caller(s)   : Application.
 Note(s)     : none.
*********************************************************************************************************/
void  BSP_IntVectSet (IRQn_Type int_id, CPU_FNCT_VOID  isr)
{
    if (int_id < IRQn_NUM) 
	{
        PLATFORM_OS_ENTER_CRITICAL();
        BSP_IntVectTbl[int_id] = isr;
        PLATFORM_OS_EXIT_CRITICAL();
    }
}

/********************************************************************************************************
 Description : Assign ISR priority.
 Argument(s) : int_id      Interrupt for which vector will be set.
               prio        Priority to assign
 Return(s)   : none.
 Caller(s)   : Application.
 Note(s)     : none.
*********************************************************************************************************/
void  BSP_IntPrioSet (IRQn_Type  int_id, uint32_t  prio)
{
    if (int_id < IRQn_NUM) 
	{
        PLATFORM_OS_ENTER_CRITICAL();
        NVIC_SetPriority(int_id, prio);
        PLATFORM_OS_EXIT_CRITICAL();
    }
}

/********************************************************************************************************
 Description : Dummy interrupt handler.
 Argument(s) : none.
 Return(s)   : none.
 Caller(s)   : BSP_IntHandler().
 Note(s)     : none.
*********************************************************************************************************/
static  void  BSP_IntHandlerDummy (void)
{

}

/********************************************************************************************************
 Description : Initialize interrupts:
 Argument(s) : none.
 Return(s)   : none.
 Caller(s)   : BSP_Init().
 Note(s)     : none.
*********************************************************************************************************/
void  BSP_IntInit (void)
{
    IRQn_Type  int_id;

    for (int_id = (IRQn_Type)0; int_id < IRQn_NUM; int_id++) 
	{
        BSP_IntVectSet(int_id, BSP_IntHandlerDummy);
    }
}

/********************************************************************************************************
 Description : Central interrupt handler.
 Argument(s) : int_id          Interrupt that will be handled.
 Return(s)   : none.
 Caller(s)   : ISR handlers.
 Note(s)     : none.
*********************************************************************************************************/
static  void  BSP_IntHandler (IRQn_Type  int_id)
{
	CPU_FNCT_VOID  isr;

	PLATFORM_OS_ENTER_CRITICAL();                                       

	PLATFORM_OS_ENTER_INT();                                               /* Tell the OS that we are starting an ISR            */

    PLATFORM_OS_EXIT_CRITICAL();

    if (int_id < IRQn_NUM) 
	{
        isr = BSP_IntVectTbl[int_id];
        if (isr != (CPU_FNCT_VOID)0) 
		{
            isr();
        }
    }

    PLATFORM_OS_EXIT_INT();                                                /* Tell the OS that we are leaving the ISR            */
}

/********************************************************************************************************
 Description : Handle an interrupt.
 Argument(s) : none.
 Return(s)   : none.
 Caller(s)   : This is an ISR.
 Note(s)     : none.
*********************************************************************************************************/
void  WWDG_IRQHandler                   (void)  { BSP_IntHandler(WWDG_IRQn);                }
void  PVD_IRQHandler                    (void)  { BSP_IntHandler(PVD_IRQn);                 }
void  TAMP_STAMP_IRQHandler             (void)  { BSP_IntHandler(TAMP_STAMP_IRQn);          }
void  RTC_WKUP_IRQHandler               (void)  { BSP_IntHandler(RTC_WKUP_IRQn);            }
void  FLASH_IRQHandler                  (void)  { BSP_IntHandler(FLASH_IRQn);               }
void  RCC_IRQHandler                    (void)  { BSP_IntHandler(RCC_IRQn);                 }
void  EXTI0_IRQHandler                  (void)  { BSP_IntHandler(EXTI0_IRQn);               }
//void  EXTI1_IRQHandler                  (void)  { BSP_IntHandler(EXTI1_IRQn);               }
//void  EXTI2_IRQHandler                  (void)  { BSP_IntHandler(EXTI2_IRQn);               }
void  EXTI3_IRQHandler                  (void)  { BSP_IntHandler(EXTI3_IRQn);               }
void  EXTI4_IRQHandler                  (void)  { BSP_IntHandler(EXTI4_IRQn);               }
void  DMA1_Stream0_IRQHandler           (void)  { BSP_IntHandler(DMA1_Stream0_IRQn);        }
void  DMA1_Stream1_IRQHandler           (void)  { BSP_IntHandler(DMA1_Stream1_IRQn);        }
void  DMA1_Stream2_IRQHandler           (void)  { BSP_IntHandler(DMA1_Stream2_IRQn);        }
void  DMA1_Stream3_IRQHandler           (void)  { BSP_IntHandler(DMA1_Stream3_IRQn);        }
void  DMA1_Stream4_IRQHandler           (void)  { BSP_IntHandler(DMA1_Stream4_IRQn);        }
void  DMA1_Stream5_IRQHandler           (void)  { BSP_IntHandler(DMA1_Stream5_IRQn);        }
void  DMA1_Stream6_IRQHandler           (void)  { BSP_IntHandler(DMA1_Stream6_IRQn);        }
void  ADC_IRQHandler                    (void)  { BSP_IntHandler(ADC_IRQn);                 }
//void  CAN1_TX_IRQHandler                (void)  { BSP_IntHandler(CAN1_TX_IRQn);             }
//void  CAN1_RX0_IRQHandler               (void)  { BSP_IntHandler(CAN1_RX0_IRQn);            }
void  CAN1_RX1_IRQHandler               (void)  { BSP_IntHandler(CAN1_RX1_IRQn);            }
void  CAN1_SCE_IRQHandler               (void)  { BSP_IntHandler(CAN1_SCE_IRQn);            }
void  EXTI9_5_IRQHandler                (void)  { BSP_IntHandler(EXTI9_5_IRQn);             }
void  TIM1_BRK_TIM9_IRQHandler          (void)  { BSP_IntHandler(TIM1_BRK_TIM9_IRQn);       }
void  TIM1_UP_TIM10_IRQHandler          (void)  { BSP_IntHandler(TIM1_UP_TIM10_IRQn);       }
void  TIM1_TRG_COM_TIM11_IRQHandler     (void)  { BSP_IntHandler(TIM1_TRG_COM_TIM11_IRQn);  }
void  TIM1_CC_IRQHandler                (void)  { BSP_IntHandler(TIM1_CC_IRQn);             }
void  TIM2_IRQHandler                   (void)  { BSP_IntHandler(TIM2_IRQn);                }
//void  TIM3_IRQHandler                   (void)  { BSP_IntHandler(TIM3_IRQn);                }
//void  TIM4_IRQHandler                   (void)  { BSP_IntHandler(TIM4_IRQn);                }
void  I2C1_EV_IRQHandler                (void)  { BSP_IntHandler(I2C1_EV_IRQn);             }
void  I2C1_ER_IRQHandler                (void)  { BSP_IntHandler(I2C1_ER_IRQn);             }
void  I2C2_EV_IRQHandler                (void)  { BSP_IntHandler(I2C2_EV_IRQn);             }
void  I2C2_ER_IRQHandler                (void)  { BSP_IntHandler(I2C2_ER_IRQn);             }
void  SPI1_IRQHandler                   (void)  { BSP_IntHandler(SPI1_IRQn);                }
void  SPI2_IRQHandler                   (void)  { BSP_IntHandler(SPI2_IRQn);                }
//void  USART1_IRQHandler                 (void)  { BSP_IntHandler(USART1_IRQn);              } //bting
//void  USART2_IRQHandler                 (void)  { BSP_IntHandler(USART2_IRQn);              }
//void  USART3_IRQHandler                 (void)  { BSP_IntHandler(USART3_IRQn);              }
void  EXTI15_10_IRQHandler              (void)  { BSP_IntHandler(EXTI15_10_IRQn);           }
void  RTC_Alarm_IRQHandler              (void)  { BSP_IntHandler(RTC_Alarm_IRQn);           }
void  OTG_FS_WKUP_IRQHandler            (void)  { BSP_IntHandler(OTG_FS_WKUP_IRQn);         }
//void  TIM8_BRK_TIM12_IRQHandler         (void)  { BSP_IntHandler(TIM8_BRK_TIM12_IRQn);      }
//void  TIM8_UP_TIM13_IRQHandler          (void)  { BSP_IntHandler(TIM8_UP_TIM13_IRQn);       }
//void  TIM8_TRG_COM_TIM14_IRQHandler     (void)  { BSP_IntHandler(TIM8_TRG_COM_TIM14_IRQn);  }
//void  TIM8_CC_IRQHandler                (void)  { BSP_IntHandler(TIM8_CC_IRQn);             }
void  DMA1_Stream7_IRQHandler           (void)  { BSP_IntHandler(DMA1_Stream7_IRQn);        }
void  FMC_IRQHandler                   (void)  { BSP_IntHandler(FMC_IRQn);                }
void  SDIO_IRQHandler                   (void)  { BSP_IntHandler(SDIO_IRQn);                }
//void  TIM5_IRQHandler                   (void)  { BSP_IntHandler(TIM5_IRQn);                }
void  SPI3_IRQHandler                   (void)  { BSP_IntHandler(SPI3_IRQn);                }
//void  UART4_IRQHandler                  (void)  { BSP_IntHandler(UART4_IRQn);               }
//void  UART5_IRQHandler                  (void)  { BSP_IntHandler(UART5_IRQn);               }
void  TIM6_DAC_IRQHandler               (void)  { BSP_IntHandler(TIM6_DAC_IRQn);            }
//void  TIM7_IRQHandler                   (void)  { BSP_IntHandler(TIM7_IRQn);                }
void  DMA2_Stream0_IRQHandler           (void)  { BSP_IntHandler(DMA2_Stream0_IRQn);        }
void  DMA2_Stream1_IRQHandler           (void)  { BSP_IntHandler(DMA2_Stream1_IRQn);        }
void  DMA2_Stream2_IRQHandler           (void)  { BSP_IntHandler(DMA2_Stream2_IRQn);        }
void  DMA2_Stream3_IRQHandler           (void)  { BSP_IntHandler(DMA2_Stream3_IRQn);        }
void  DMA2_Stream4_IRQHandler           (void)  { BSP_IntHandler(DMA2_Stream4_IRQn);        }
void  ETH_IRQHandler                    (void)  { BSP_IntHandler(ETH_IRQn);                 }
void  ETH_WKUP_IRQHandler               (void)  { BSP_IntHandler(ETH_WKUP_IRQn);            }
//void  CAN2_TX_IRQHandler                (void)  { BSP_IntHandler(CAN2_TX_IRQn);             }
//void  CAN2_RX0_IRQHandler               (void)  { BSP_IntHandler(CAN2_RX0_IRQn);            }
void  CAN2_RX1_IRQHandler               (void)  { BSP_IntHandler(CAN2_RX1_IRQn);            }
void  CAN2_SCE_IRQHandler               (void)  { BSP_IntHandler(CAN2_SCE_IRQn);            }
void  OTG_FS_IRQHandler                 (void)  { BSP_IntHandler(OTG_FS_IRQn);              }
void  DMA2_Stream5_IRQHandler           (void)  { BSP_IntHandler(DMA2_Stream5_IRQn);        }
void  DMA2_Stream6_IRQHandler           (void)  { BSP_IntHandler(DMA2_Stream6_IRQn);        }
void  DMA2_Stream7_IRQHandler           (void)  { BSP_IntHandler(DMA2_Stream7_IRQn);        }
//void  USART6_IRQHandler                 (void)  { BSP_IntHandler(USART6_IRQn);              }
void  I2C3_EV_IRQHandler                (void)  { BSP_IntHandler(I2C3_EV_IRQn);             }
void  I2C3_ER_IRQHandler                (void)  { BSP_IntHandler(I2C3_ER_IRQn);             }
void  OTG_HS_EP1_OUT_IRQHandler         (void)  { BSP_IntHandler(OTG_HS_EP1_OUT_IRQn);      }
void  OTG_HS_EP1_IN_IRQHandler          (void)  { BSP_IntHandler(OTG_HS_EP1_IN_IRQn);       }
void  OTG_HS_WKUP_IRQHandler            (void)  { BSP_IntHandler(OTG_HS_WKUP_IRQn);         }
void  OTG_HS_IRQHandler                 (void)  { BSP_IntHandler(OTG_HS_IRQn);              }
void  DCMI_IRQHandler                   (void)  { BSP_IntHandler(DCMI_IRQn);                }
void  CRYP_IRQHandler                   (void)  { BSP_IntHandler(CRYP_IRQn);                }
void  HASH_RNG_IRQHandler               (void)  { BSP_IntHandler(HASH_RNG_IRQn);            }
void  FPU_IRQHandler                    (void)  { BSP_IntHandler(FPU_IRQn);                 }
