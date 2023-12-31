/*********************************************************************
*          Portions COPYRIGHT 2013 STMicroelectronics                *
*          Portions SEGGER Microcontroller GmbH & Co. KG             *
*        Solutions for real time microcontroller applications        *
**********************************************************************
*                                                                    *
*        (c) 1996 - 2013  SEGGER Microcontroller GmbH & Co. KG       *
*                                                                    *
*        Internet: www.segger.com    Support:  support@segger.com    *
*                                                                    *
**********************************************************************

** emWin V5.20 - Graphical user interface for embedded applications **
All  Intellectual Property rights  in the Software belongs to  SEGGER.
emWin is protected by  international copyright laws.  Knowledge of the
source code may not be used to write a similar product.  This file may
only be used in accordance with the following terms:

The  software has  been licensed  to STMicroelectronics International
N.V. a Dutch company with a Swiss branch and its headquarters in Plan-
les-Ouates, Geneva, 39 Chemin du Champ des Filles, Switzerland for the
purposes of creating libraries for ARM Cortex-M-based 32-bit microcon_
troller products commercialized by Licensee only, sublicensed and dis_
tributed under the terms and conditions of the End User License Agree_
ment supplied by STMicroelectronics International N.V.
Full source code is available at: www.segger.com

We appreciate your understanding and fairness.
----------------------------------------------------------------------
File        : GUIConf.c
Purpose     : Display controller initialization
---------------------------END-OF-HEADER------------------------------
*/

/**
  ******************************************************************************
  * @file    GUIConf.c
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    22-July-2013
  * @brief   Display controller initialization
  ******************************************************************************
  * @attention
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

#include "GUI.h"

/*********************************************************************
*
*       Defines, configurable
*
**********************************************************************
*/

//
// Define the available number of bytes available for the GUI
//

#if defined (STM32F429X)
 #define GUI_NUMBYTES  (1024) *  96    // x KByte
#elif defined (STM32F40XX) || defined (STM32F2XX) || defined (STM32F10X_HD_VL)
 #define GUI_NUMBYTES  (1024) *  512   // x KByte
#elif defined (STM32F10X_HD)
 #define GUI_NUMBYTES  (1024) *  53   // x KByte
#elif defined (STM32F10X_MD)
 #define GUI_NUMBYTES  (1024) *  10   // x KByte
#elif defined (STM32F10X_MD_VL) || defined (STM32F0XX)
 #define GUI_NUMBYTES  (1024) *  3   // x KByte
#elif defined (STM32F30X)
 #define GUI_NUMBYTES  (1024) *  25   // x KByte
#elif defined (STM32F37X)
 #define GUI_NUMBYTES  (1024) *  22   // x KByte
#elif defined (STM32L1XX_HD)
 #define GUI_NUMBYTES  (1024) *  35   // x KByte
#elif defined (STM32L1XX_MD)
 #define GUI_NUMBYTES  (1024) *  6   // x KByte
#else
 #define GUI_NUMBYTES  (1024) *  40   // x KByte
#endif

/*********************************************************************
*
*       Static data
*
**********************************************************************
*/


/* 32 bit aligned memory area */

#ifdef __ICCARM__
 #if defined (STM32F40XX) || defined (STM32F2XX)
  #pragma location=0x64000000
   static __no_init U32 extMem[GUI_NUMBYTES / 4];
 #elif defined (STM32F10X_HD_VL)
  #pragma location=0x68000000
   static __no_init U32 extMem[GUI_NUMBYTES / 4];
 #else
   U32 extMem[GUI_NUMBYTES / 4];
 #endif
#elif defined (__CC_ARM)
 #if defined (STM32F40XX) || defined (STM32F2XX)
   static U32 extMem[GUI_NUMBYTES / 4] __attribute__((at(0x64000000)));
 #elif defined (STM32F10X_HD_VL)
   static U32 extMem[GUI_NUMBYTES / 4] __attribute__((at(0x68000000)));
 #else
   U32 extMem[GUI_NUMBYTES / 4];
 #endif
#endif   



/*********************************************************************
*
*       Public code
*
**********************************************************************
*/
/*********************************************************************
*
*       GUI_X_Config
*
* Purpose:
*   Called during the initialization process in order to set up the
*   available memory for the GUI.
*/
void GUI_X_Config(void)
{

  GUI_ALLOC_AssignMemory(extMem, GUI_NUMBYTES);

}

/*************************** End of file ****************************/
