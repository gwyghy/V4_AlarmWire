/*****************************************************************************
 Copyright 2015 Broadcom Corporation.  All rights reserved.

 This program is the proprietary software of Broadcom Corporation and/or its
 licensors, and may only be used, duplicated, modified or distributed pursuant
 to the terms and conditions of a separate, written license agreement executed
 between you and Broadcom (an "Authorized License").

 Except as set forth in an Authorized License, Broadcom grants no license
 (express or implied), right to use, or waiver of any kind with respect to the
 Software, and Broadcom expressly reserves all rights in and to the Software
 and all intellectual property rights therein.  IF YOU HAVE NO AUTHORIZED
 LICENSE, THEN YOU HAVE NO RIGHT TO USE THIS SOFTWARE IN ANY WAY, AND SHOULD
 IMMEDIATELY NOTIFY BROADCOM AND DISCONTINUE ALL USE OF THE SOFTWARE.

  Except as expressly set forth in the Authorized License,
 1. This program, including its structure, sequence and organization,
    constitutes the valuable trade secrets of Broadcom, and you shall use all
    reasonable efforts to protect the confidentiality thereof, and to use this
    information only in connection with your use of Broadcom integrated
    circuit products.

 2. TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS PROVIDED "AS IS"
    AND WITH ALL FAULTS AND BROADCOM MAKES NO PROMISES, REPRESENTATIONS OR
    WARRANTIES, EITHER EXPRESS, IMPLIED, STATUTORY, OR OTHERWISE, WITH RESPECT
    TO THE SOFTWARE.  BROADCOM SPECIFICALLY DISCLAIMS ANY AND ALL IMPLIED
    WARRANTIES OF TITLE, MERCHANTABILITY, NONINFRINGEMENT, FITNESS FOR A
    PARTICULAR PURPOSE, LACK OF VIRUSES, ACCURACY OR COMPLETENESS,
    QUIET ENJOYMENT, QUIET POSSESSION OR CORRESPONDENCE TO DESCRIPTION.
    YOU ASSUME THE ENTIRE RISK ARISING OUT OF USE OR PERFORMANCE OF THE
    SOFTWARE.

 3. TO THE MAXIMUM EXTENT PERMITTED BY LAW, IN NO EVENT SHALL BROADCOM OR ITS
    LICENSORS BE LIABLE FOR (i) CONSEQUENTIAL, INCIDENTAL, SPECIAL, INDIRECT,
    OR EXEMPLARY DAMAGES WHATSOEVER ARISING OUT OF OR IN ANY WAY RELATING TO
    YOUR USE OF OR INABILITY TO USE THE SOFTWARE EVEN IF BROADCOM HAS BEEN
    ADVISED OF THE POSSIBILITY OF SUCH DAMAGES; OR (ii) ANY AMOUNT IN EXCESS
    OF THE AMOUNT ACTUALLY PAID FOR THE SOFTWARE ITSELF OR U.S. $1, WHICHEVER
    IS GREATER. THESE LIMITATIONS SHALL APPLY NOTWITHSTANDING ANY FAILURE OF
    ESSENTIAL PURPOSE OF ANY LIMITED REMEDY.
******************************************************************************/


/******************************************************************************
 File Name:   bcm8953x.h
 Description: Board specific macros
******************************************************************************/


#ifndef __BCM8953X_H__
#define __BCM8953X_H__

/*
 * Int   HW   Port
 * P0 -- P0 -- BR0
 * P1 -- P1 -- BR1
 * P2 -- P2 -- BR2
 * P3 -- P3 -- BR3
 * P4 -- P4 -- BR4
 * P5 -- P5 -- SGMII
 * P6 -- P6 -- SGMII
 *       P7 -- ARM
 * P7--- P8 -- IMP(SGMII)
 *
 */


/* Max AVB parameters */
#define AVB_MAX_BR_ONLY_PORTS       4
#define AVB_MAX_BRFE_PORT           1
/* maximum number of avb BR ports */
#define AVB_MAX_BR_PORTS            (AVB_MAX_BR_ONLY_PORTS + AVB_MAX_BRFE_PORT)
#define AVB_MAX_INTERFACES_SGMII    2  /* Port5 & Port6 */
#define AVB_MAX_INTERFACES_PHYS     (AVB_MAX_BR_PORTS + AVB_MAX_INTERFACES_SGMII)   /* Add SGMII */
#define AVB_MAX_INTERFACES_PHYS_P8  (AVB_MAX_INTERFACES_PHYS + 1)   /* Add Port8 */
#define AVB_INTF_INVALID            0xFFFF /* Invalid interface number */

#define SWITCH_AGE_TIME_MAX         0xFFFFF

/* Number of front panel ports on the device */
#define BCM895XX_MAX_NUM_PORTS      AVB_MAX_INTERFACES_PHYS_P8
#define AVB_MAX_INTERFACES          AVB_MAX_INTERFACES_PHYS_P8
#define AVB_MAX_TRAFFIC_CLASSES     8   /* TC 0 through 7 */
#define AVB_MAX_TRAFFIC_CLASSES_CFG 2   /* TC 7 cosQ-A and 6 cosQ-B are user configurable */
#define AVB_MAX_VLANS               16  /* Max number of static VLANs */
#define AVB_MAX_MSRP_STREAMS        32  /* Max number of static streams */

/* Mapping between port numbers and hardware index. In some cases this can be
 * one-to-one straight mapping. Port 7 is used by internal ARM CPU (hence not
 * defined below). Port numbers used by applications are sequential, 0 through 6
 */
#define BCM895XX_INT_CPU_PORT_NUM   7   /* Internal CPU port number */
#define BCM895XX_IMP_PORT_NUM       8   /* IMP port number */
#define AVB_INTF_P8                 BCM895XX_IMP_PORT_NUM   /* External Port8 Interface */
#define AVB_INTF_P8_INTERNAL        (AVB_MAX_INTERFACES_PHYS_P8-1)       /* Internal Port8 Interface */

/* Macros to identify type of port based on INTERNAL port index */
/* P0-3 */
#define AVB_INT_PORT_IS_BR_ONLY(x)  ((x)<AVB_MAX_BR_ONLY_PORTS)
/* P0-4 */
#define AVB_INT_PORT_IS_BR(x)       ((x)<AVB_MAX_BR_PORTS)
/* P4 */
#define AVB_INT_PORT_IS_BRFE(x)     ((x)==(AVB_MAX_BR_PORTS-1))
/* P5-6*/
#define AVB_INT_PORT_IS_SGMII(x)    (((x)>=AVB_MAX_BR_PORTS) && ((x)<AVB_MAX_INTERFACES_PHYS_P8))
/* P8 */
#define AVB_INT_PORT_IS_IMP(x)      ((x)==AVB_INTF_P8_INTERNAL)

/* Sets bit positions 'msb till 'lsb', both inclusive */
#define SET_BITS(msb, lsb)          (((1<<((msb)-(lsb)+1))-1) << (lsb))
/* Sets bits from position '0' upto and including position 'msb' */
#define SET_BITS_UPTO(msb)          SET_BITS(msb, 0)

#define AVB_INTF_MASK_ONLY_BR       SET_BITS_UPTO(AVB_MAX_BR_ONLY_PORTS - 1) //0xF
#define AVB_INTF_MASK_BR            SET_BITS_UPTO(AVB_MAX_BR_PORTS - 1) //0x1F
#define AVB_INTF_MASK_PHYS          SET_BITS_UPTO(AVB_MAX_INTERFACES_PHYS - 1) //0x7F
#define AVB_INTF_MASK_PHYS_P8       SET_BITS_UPTO(AVB_MAX_INTERFACES_PHYS_P8 - 1) //0xFF

#define BCM895XX_PORT0_ADDRESS      BRPHY0_CL45DEV1_PMD_IEEE_CTL1
#define BCM895XX_PORT1_ADDRESS      BRPHY1_CL45DEV1_PMD_IEEE_CTL1
#define BCM895XX_PORT2_ADDRESS      BRPHY2_CL45DEV1_PMD_IEEE_CTL1
#define BCM895XX_PORT3_ADDRESS      BRPHY3_CL45DEV1_PMD_IEEE_CTL1
#define BCM895XX_PORT4_ADDRESS      BRPHY4_CL45DEV1_PMD_IEEE_CTL1
#define BCM895XX_PORT5_ADDRESS      SGMII2_CL22_B0_MIICNTL  //SGMII2 or IMP1
#define BCM895XX_PORT6_ADDRESS      SGMII1_CL22_B0_MIICNTL  //SGMII1
#define BCM895XX_PORT_IMP_ADDRESS   SGMII0_CL22_B0_MIICNTL  //SGMII0 or IMP0

#define AVB_VALID_PORT_BITMASK         0x17F /* Ports 0-5,6,8 */

/* Enums for chip revision */
typedef enum {
    BCM8953X_A0 = 0,
    BCM8953X_B0 = 1,
    BCM8953X_B1 = 2,
} BCM8953X_CHIPREV_t;

extern uint32_t hwPortMap[BCM895XX_MAX_NUM_PORTS];
extern BCM8953X_CHIPREV_t chipRev;

#define CHIP_LACKS_HW_TS_ON_SGMII_PORTS (chipRev == BCM8953X_A0 || chipRev == BCM8953X_B0)
#define INTF_LACKS_HW_TS(port) (CHIP_LACKS_HW_TS_ON_SGMII_PORTS && AVB_INT_PORT_IS_SGMII(port))

#endif /* __BCM8953X_H__ */
