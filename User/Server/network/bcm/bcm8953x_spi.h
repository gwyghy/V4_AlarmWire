/*
 * $Id$
 * $Copyright: Copyright 2016 Broadcom Limited.
 * This program is the proprietary software of Broadcom Limited
 * and/or its licensors, and may only be used, duplicated, modified
 * or distributed pursuant to the terms and conditions of a separate,
 * written license agreement executed between you and Broadcom
 * (an "Authorized License").  Except as set forth in an Authorized
 * License, Broadcom grants no license (express or implied), right
 * to use, or waiver of any kind with respect to the Software, and
 * Broadcom expressly reserves all rights in and to the Software
 * and all intellectual property rights therein.  IF YOU HAVE
 * NO AUTHORIZED LICENSE, THEN YOU HAVE NO RIGHT TO USE THIS SOFTWARE
 * IN ANY WAY, AND SHOULD IMMEDIATELY NOTIFY BROADCOM AND DISCONTINUE
 * ALL USE OF THE SOFTWARE.
 *
 * Except as expressly set forth in the Authorized License,
 *
 * 1.     This program, including its structure, sequence and organization,
 * constitutes the valuable trade secrets of Broadcom, and you shall use
 * all reasonable efforts to protect the confidentiality thereof,
 * and to use this information only in connection with your use of
 * Broadcom integrated circuit products.
 *
 * 2.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS
 * PROVIDED "AS IS" AND WITH ALL FAULTS AND BROADCOM MAKES NO PROMISES,
 * REPRESENTATIONS OR WARRANTIES, EITHER EXPRESS, IMPLIED, STATUTORY,
 * OR OTHERWISE, WITH RESPECT TO THE SOFTWARE.  BROADCOM SPECIFICALLY
 * DISCLAIMS ANY AND ALL IMPLIED WARRANTIES OF TITLE, MERCHANTABILITY,
 * NONINFRINGEMENT, FITNESS FOR A PARTICULAR PURPOSE, LACK OF VIRUSES,
 * ACCURACY OR COMPLETENESS, QUIET ENJOYMENT, QUIET POSSESSION OR
 * CORRESPONDENCE TO DESCRIPTION. YOU ASSUME THE ENTIRE RISK ARISING
 * OUT OF USE OR PERFORMANCE OF THE SOFTWARE.
 *
 * 3.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, IN NO EVENT SHALL
 * BROADCOM OR ITS LICENSORS BE LIABLE FOR (i) CONSEQUENTIAL,
 * INCIDENTAL, SPECIAL, INDIRECT, OR EXEMPLARY DAMAGES WHATSOEVER
 * ARISING OUT OF OR IN ANY WAY RELATING TO YOUR USE OF OR INABILITY
 * TO USE THE SOFTWARE EVEN IF BROADCOM HAS BEEN ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGES; OR (ii) ANY AMOUNT IN EXCESS OF
 * THE AMOUNT ACTUALLY PAID FOR THE SOFTWARE ITSELF OR USD 1.00,
 * WHICHEVER IS GREATER. THESE LIMITATIONS SHALL APPLY NOTWITHSTANDING
 * ANY FAILURE OF ESSENTIAL PURPOSE OF ANY LIMITED REMEDY.$
 *
 * SPI specific to BCM8953X implementation
 *
 */

#ifndef BCM8953X_SPI
#define BCM8953X_SPI

#define MGMT_SPI_SEND_MAX_RETRIES_SEC   10
#define MGMT_SPI_RECV_MAX_RETRIES_SEC   10

/* Macros for SPI opcode generation */
#define MAX_BUF_SZ                      16
#define SPI_OPCODE_PHYADDR_SHIFT        6
#define SPI_OPCODE_PHYADDR(a)           ((a) << SPI_OPCODE_PHYADDR_SHIFT)
#define SPI_OPCODE_RDWR_SHIFT           5
#define SPI_OPCODE_WR                   (1 << SPI_OPCODE_RDWR_SHIFT)
#define SPI_OPCODE_RD                   (0 << SPI_OPCODE_RDWR_SHIFT)
#define SPI_OPCODE_INC_SHIFT            4
#define SPI_OPCODE_AUTO_INC             (1 << SPI_OPCODE_INC_SHIFT)
#define SPI_OPCODE_NO_INC               (0 << SPI_OPCODE_INC_SHIFT)
#define SPI_OPCODE_RD_WAIT_SHIFT        2
#define SPI_OPCODE_RD_WAIT_MASK         (0x3 << SPI_OPCODE_RD_WAIT_SHIFT)
#define SPI_OPCODE_RD_WAIT_0            (0 << SPI_OPCODE_RD_WAIT_SHIFT)
#define SPI_OPCODE_RD_WAIT_2            (1 << SPI_OPCODE_RD_WAIT_SHIFT)
#define SPI_OPCODE_RD_WAIT_4            (2 << SPI_OPCODE_RD_WAIT_SHIFT)
#define SPI_OPCODE_RD_WAIT_6            (3 << SPI_OPCODE_RD_WAIT_SHIFT)
#define SPI_OPCODE_TZ_SZ_MASK           0x3
#define SPI_OPCODE_TX_SZ_8              (0)
#define SPI_OPCODE_TX_SZ_16             (1)
#define SPI_OPCODE_TX_SZ_32             (2)
#define SPI_OPCODE_TX_SZ_64             (3)

#define IND_ACC_RETRY                   10
#define IND_ACC_TX_SZ_8                 0
#define IND_ACC_TX_SZ_16                1
#define IND_ACC_TX_SZ_32                2
#define IND_ACC_TX_SZ_64                3
#define IND_ACC_RD                      (0 << 2)
#define IND_ACC_WR                      (1 << 2)
#define IND_ACC_AUTO_INC                (1 << 3)
#define IND_ACC_NO_AUTO_INC             (0 << 3)
#define IND_ACC_COMMIT                  (1 << 4)
#define IND_ACC_COMMIT_ON_RDWR          (1 << 5)
#define IND_ACC_DONE                    (1 << 7)

#define RD_WR_MODE_8                    8
#define RD_WR_MODE_16                   16
#define RD_WR_MODE_32                   32
#define RD_WR_MODE_64                   64


typedef enum {
    SPI_ID_0 = 0,
    SPI_ID_1 = 1,
    SPI_ID_2 = 2,
    SPI_ID_3 = 3
} SPI_ID_t;

#endif /* BCM8953X_SPI */
