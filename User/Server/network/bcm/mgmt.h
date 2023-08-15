/*****************************************************************************
 Copyright 2016-2017 Broadcom Limited.  All rights reserved.

 This program is the proprietary software of Broadcom Limited and/or its
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
 File Name:  mgmt.h
 Description: Arm Processor Subsystem remote management library
******************************************************************************/

/*
 * Warning:
 * This file is included by both uKernel and host code. Be very careful
 * about what goes into this file, esp. with regards to including other
 * files.
 */

#ifndef APPL_DIAG_APS_MGMT_H
#define APPL_DIAG_APS_MGMT_H

#include "vpdcfg.h"
#include "bcm8953x.h"

#define MGMT_COMMAND_MAGIC      0xa5a5a5a5
#define MGMT_REPLY_MAGIC        0x5a5a5a5a

/* SPI command codes */
#define MGMT_SPI_CMD_READY              0xa1
#define MGMT_SPI_CMD                    0xa2
#define MGMT_SPI_REPLY                  0xa3

#define VERSION_STR_LEN                 100

/* reboot the chip completely including memories, phy etc */
#define MGMT_REBOOT_CHIP        0x00000001UL
/* reboot cpu alone and put it in incremental download mode */
#define MGMT_REBOOT_INCR_DL     0x00000002UL

typedef enum
{
    /*Port events*/
    AVB_ASYNC_EVENT_PORT_LINK_DOWN = 0,
    AVB_ASYNC_EVENT_PORT_LINK_UP =1,
    AVB_ASYNC_EVENT_PORT_PDELAY_LOST_RESPONSE_EXCEEDED = 2,
    AVB_ASYNC_EVENT_PORT_PDELAY_LOST_RESPONSE_RECOVERED = 3,
    AVB_ASYNC_EVENT_PORT_PDELAY_CONFIG_CHANGE = 4,
    AVB_ASYNC_EVENT_PORT_STREAM_BLOCKED = 5,
}avb_async_port_events_t;

typedef enum
{
    /*global events*/
    AVB_ASYNC_EVENT_GLOBAL_ETH_READY = 0,
    AVB_ASYNC_EVENT_GLOBAL_AVB_SYNC = 1,
    AVB_ASYNC_EVENT_GLOBAL_SYNC_RECEIPT_TIMEOUT =2,
    AVB_ASYNC_EVENT_GLOBAL_STREAM_BLOCKED =3,
}avb_async_global_events_t;

#define MIRROR_INGRESS (0)
#define MIRROR_EGRESS  (1)
#define MIRROR_BOTH    (2)

/*Port Event Masks*/
#define AVB_ASYNC_EVENT_PORT_LINK_DOWN_MASK                      (1 << AVB_ASYNC_EVENT_PORT_LINK_DOWN)
#define AVB_ASYNC_EVENT_PORT_LINK_UP_MASK                        (1 << AVB_ASYNC_EVENT_PORT_LINK_UP)
#define AVB_ASYNC_EVENT_PORT_PDELAY_LOST_RESPONSE_EXCEEDED_MASK  (1 << AVB_ASYNC_EVENT_PORT_PDELAY_LOST_RESPONSE_EXCEEDED)
#define AVB_ASYNC_EVENT_PORT_PDELAY_LOST_RESPONSE_RECOVERED_MASK (1 << AVB_ASYNC_EVENT_PORT_PDELAY_LOST_RESPONSE_RECOVERED)
#define AVB_ASYNC_EVENT_PORT_PDELAY_CONFIG_CHANGE_MASK           (1 << AVB_ASYNC_EVENT_PORT_PDELAY_CONFIG_CHANGE)
#define AVB_ASYNC_EVENT_PORT_STREAM_BLOCKED_MASK                 (1 << AVB_ASYNC_EVENT_PORT_STREAM_BLOCKED)

/*Global Event Masks*/
#define AVB_ASYNC_EVENT_GLOBAL_ETH_READY_MASK                    (1 << AVB_ASYNC_EVENT_GLOBAL_ETH_READY)
#define AVB_ASYNC_EVENT_GLOBAL_AVB_SYNC_MASK                     (1 << AVB_ASYNC_EVENT_GLOBAL_AVB_SYNC)
#define AVB_ASYNC_EVENT_GLOBAL_SYNC_RECEIPT_TIMEOUT_MASK         (1 << AVB_ASYNC_EVENT_GLOBAL_SYNC_RECEIPT_TIMEOUT)
#define AVB_ASYNC_EVENT_GLOBAL_STREAM_BLOCKED_MASK               (1 << AVB_ASYNC_EVENT_GLOBAL_STREAM_BLOCKED)

typedef enum
{
    INVALID = 0,
    PAIR_OK,
    PAIR_OPEN,
    INTRA_PAIR_SHORT,
    PAIR_BUSY = 9
}CABLE_HEALTH_STATUS_t;

typedef enum {
    MGMT_INTF_MIRROR_INGRESS = 0,
    MGMT_INTF_MIRROR_EGRESS,
    MGMT_INTF_MIRROR_BOTH
}MGMT_INTF_MIRROR_MODE_t;

#define MGMT_AVB_BUFF_TYPE_AVBCFG       (0x1UL)
#define MGMT_AVB_BUFF_TYPE_TIMESYNC     (0x2UL)
#define MGMT_AVB_BUFF_TYPE_LOG          (0x3UL)

typedef enum {
    MGMT_OS_SW_VERSION = 0x0,           /* Version of Operating System */
    MGMT_AVB = 0x1,                     /* Audio Video Bridging command */
    MGMT_REBOOT = 0x2,                  /* Reboot request */
} mgmt_cmd_t;

typedef enum {
    MGMT_SUCCESS,                       /* successful operation */
    MGMT_ERROR                          /* generic error */
} mgmt_status_t;

/* AVB sub-commands */
typedef enum {
    MGMT_AVB_PORT_INFO_GET = 0x1,
    MGMT_AVB_PORT_ADMIN_MODE_SET = 0x2,
    MGMT_AVB_PORT_SPEED_SET = 0x3,
    MGMT_AVB_PORT_MASTER_SLAVE_SET = 0x4,
    MGMT_AVB_PORT_JUMBO_MODE_SET = 0x5,
    MGMT_AVB_PORT_PHY_LB_SET = 0x6,
    MGMT_AVB_PORT_DUMBFWD_MODE_SET = 0X7,
    MGMT_AVB_PORT_DUMBFWD_MODE_GET = 0X8,
    MGMT_AVB_PORT_JUMBO_MODE_GET = 0x9,
    MGMT_AVB_PORT_TYPE_GET = 0xa,
    MGMT_AVB_PORT_ADMIN_MODE_GET = 0xb,
    MGMT_AVB_PORT_SPEED_GET = 0xc,
    MGMT_AVB_PORT_MASTER_SLAVE_GET = 0xd,
    MGMT_AVB_PORT_PHY_LB_GET = 0xe,
    MGMT_AVB_PORT_LINK_SQI_GET = 0xf,

    MGMT_AVB_VLAN_CREATE = 0x40,
    MGMT_AVB_VLAN_DELETE = 0x41,
    MGMT_AVB_VLAN_PORT_ADD = 0x42,
    MGMT_AVB_VLAN_PORT_REMOVE = 0x43,
    MGMT_AVB_VLAN_PVID_SET = 0x44,
    MGMT_AVB_VLAN_IFILTER_SET = 0x45,
    MGMT_AVB_VLAN_PRIO_SET = 0x46,
    MGMT_AVB_VLAN_GET = 0x47,
    MGMT_AVB_VLAN_PORT_GET = 0x48,

    MGMT_AVB_MSRP_ENTRY_ADD = 0x60,
    MGMT_AVB_MSRP_ENTRY_DELETE = 0x61,
    MGMT_AVB_MSRP_STREAM_BANDWIDTH_GET = 0x62,
    MGMT_AVB_MSRP_PORT_BANDWIDTH_GET = 0x63,
    MGMT_AVB_MSRP_PORT_BANDWIDTH_SET = 0x64,
    MGMT_AVB_MSRP_PCP_MAPPING_GET = 0x65,
    MGMT_AVB_MSRP_PCP_MAPPING_SET = 0x66,
    MGMT_AVB_MSRP_BOUNDARY_SET = 0x67,
    MGMT_AVB_MSRP_BOUNDARY_GET = 0x68,

    MGMT_AVB_SWITCH_MIRROR_ENABLE = 0x80,
    MGMT_AVB_SWITCH_MIRROR_DISABLE = 0x81,
    MGMT_AVB_SWITCH_MIRROR_STATUS = 0x82,
    MGMT_AVB_SWITCH_AGE_TIME_SET = 0x83,
    MGMT_AVB_SWITCH_AGE_TIME_GET = 0x84,

    MGMT_AVB_CONFIG_SAVE = 0x90,
    MGMT_AVB_CONFIG_CLEAR = 0x92,

    MGMT_AVB_AVNU_GLOBAL_CONFIG_GET = 0xb0,
    MGMT_AVB_AVNU_INTERFACE_CONFIG_GET = 0xb1,
    MGMT_AVB_AVNU_CLOCK_STATE_SET = 0xb3,
    MGMT_AVB_AVNU_ASCAPABLE_MODE_SET = 0xb4,
    MGMT_AVB_AVNU_PORT_ROLE_SET = 0xb5,
    MGMT_AVB_AVNU_NBR_PDELAY_SET = 0xb6,
    MGMT_AVB_AVNU_INITIAL_LOG_PDELAY_INTL_SET = 0xb7,
    MGMT_AVB_AVNU_INITIAL_LOG_SYNC_INTL_SET = 0xb8,
    MGMT_AVB_AVNU_SYNC_ABSENCE_TIMEOUT_SET = 0xb9,
    MGMT_AVB_AVNU_OPERATIONAL_PDELAY_INTERVAL_SET = 0xba,
    MGMT_AVB_AVNU_GLOBAL_STATUS = 0xbb,
    MGMT_AVB_AVNU_PORT_STATUS = 0xbc,
    MGMT_AVB_AVNU_DEVICE_STATE_GET = 0xbd,
    MGMT_AVB_AVNU_GLOBAL_ADMIN_MODE_SET = 0xbf,
    MGMT_AVB_AVNU_STATS_GET = 0xc1,
    MGMT_AVB_AVNU_STATS_CLEAR = 0xc2,
    MGMT_AVB_AVNU_NUM_LSTRESP_SET = 0xc3,
    MGMT_AVB_AVNU_PDELAY = 0xc4,

    MGMT_AVB_VPD_GET = 0xd0,
#ifdef ALLOW_VPD_SAVE
    MGMT_AVB_VPD_ERASE = 0xd1,
    MGMT_AVB_VPD_SET = 0xd2,
#endif
    MGMT_AVB_HEARTBEAT_INTERVAL_SET = 0xfa,
    MGMT_AVB_BUFF_INFO = 0xfb,
    MGMT_AVB_ASYNC_EVENTS_GET = 0xfd,
    MGMT_AVB_SW_VERSION = 0xfe,
} mgmt_avb_cmd_t;


#define MGMT_COMMAND_LEN        16      /* mgmt command w/ no data */
#define MGMT_STATUS_LEN         16      /* mgmt reply w/ no data */

#define MGMT_AVB_COMMAND_CODE_LEN    4       /* 4 bytes enum is 4 bytes even on 64 bit machine*/
#define MGMT_AVB_COMMAND_LEN    (MGMT_COMMAND_LEN + MGMT_AVB_COMMAND_CODE_LEN)      /* MGMT_COMMAND_LEN + MGMT_AVB_COMMAND_CODE_LEN*/

#define MGMT_NUM_ARGS           4

#define MGMT_FLASH_PAGE_SIZE    256     /* Flash page size */
#define MGMT_MEMORY_PAGE_SIZE   256     /* Memory page size */

#define MGMT_NUM_BUFFERS        4       /* Max number of buffers */
#define MGMT_BUFFER_PAGE_SIZE   256     /* Buffer page size */
#define MGMT_BUFFER_FLAG_PROT   1       /* Buffer is protected */

#define MGMT_SHELL_CMD_LEN      256     /* Max length of shell command */

typedef struct {
    uint32_t avb_cmd;  /* AVB command type */
    union {
        struct {
            uint32_t value;   /* Switch age time */
        } switch_age_time;
        struct {
            uint32_t value;   /* Switch age time */
        } switch_mtu;
        struct {
            uint32_t num;   /* Port number */
        } port_info;
        struct {
            uint32_t num;   /* Port number */
            uint32_t value; /* 0=Disabled, 1=Enabled */
        } port_admin_mode;
        struct {
            uint32_t num;   /* Port number */
            uint32_t value; /* 0=Autoneg, 10, 100, 1000 */
        } port_speed;
        struct {
            uint32_t num;   /* Port number */
            uint32_t value; /* 0=slave, 1=master, 2=auto 3=none */
        } br_mode;
        struct {
            uint32_t num;   /* Port number */
        } port_stat;
        struct {
            uint32_t num;   /* Port number */
            uint32_t value; /* 0=Disabled, 1=Enabled */
        } port_phy_lb_mode;
        struct {
            uint32_t num;   /* Port number */
            uint32_t value; /* 0=Disabled, 1=Enabled */
        } port_jumbo_frame_mode;
        struct {
            uint32_t port;  /* port number */
        } mgmt_port_link_sqi;
        struct {
            uint32_t  value; /* 0=Disabled, 1=Enabled */
        } as_mode;
        struct {
            uint32_t num;   /* Port number */
        } as_port_info;
        struct {
            uint32_t num;   /* Port number */
        } as_port_stats;
        struct {
            uint32_t num;   /* Port number */
            uint32_t value;
        } as_allowed_lst_resp;
        struct {
            uint32_t id;   /* VLAN ID */
        } vlan_create;
        struct {
            uint32_t id;   /* VLAN ID */
        } vlan_delete;
        struct {
            uint32_t id;    /* VLAN ID */
            uint32_t num;   /* Port number */
            uint32_t is_tagged;   /* tagged or untagged */
        } vlan_port_add;
        struct {
            uint32_t id;   /* VLAN ID */
            uint32_t num;   /* Port number */
        } vlan_port_del;
        struct {
            uint32_t num;   /* Port number */
            uint32_t value; /* Value */
        } vlan_pvid;
        struct {
            uint32_t num;   /* Port number */
            uint32_t value; /* Value */
        } vlan_ifilter;
        struct {
            uint32_t num;   /* Port number */
            uint32_t value; /* Value */
        } vlan_untag_prio;
        struct {
            uint32_t id;   /* VLAN ID */
        } vlan_get;
        struct {
            uint32_t port; /* vlan port info */
        } vlan_port_info;
        struct {
            uint32_t id;        /* VLAN ID */
            uint8_t  addr[6];   /* MAC address */
            uint8_t  pad[2];
            uint32_t port_bmp;  /* Port or bitmap */
            uint32_t cir_kbps;       /* Committed ingress rate */
            uint32_t cbs_bytes;       /* Committed burst size */
            uint32_t ingressPortMask;  /* Ingress port bitmap */

        } fdb_add;
        struct {
            uint32_t id;        /* VLAN ID */
            uint8_t  addr[6];   /* MAC address */
            uint8_t  pad[2];
        } fdb_del;
        struct {
            uint32_t id;        /* VLAN ID */
            uint8_t  addr[6];   /* MAC address */
            uint8_t  pad[2];
        } stream_bandwidth_get;
        struct {
            uint32_t id;        /* VLAN ID */
            uint32_t value;     /* port */
        } fdb_get;
        struct {
            uint32_t num;   /* Port number */
            uint32_t sr_class;  /* Stream class */
        } port_bandwidth_get;
        struct {
            uint32_t num;   /* Port number */
            uint32_t sr_class;  /* Stream Class */
            uint32_t rate_kbps;    /* Rate in kpbs */
            uint32_t burst_kbits;  /* Burst in kbits */
        } port_bandwidth_set;
        struct {
            uint32_t sr_class;  /* AVB class */
        } pcp_mapping_get;
        struct {
            uint32_t sr_class;   /* AVB class */
            uint32_t prio;       /* Priority of class */
            uint32_t remap_prio; /* Remap priority of class */
        } pcp_mapping_set;
        struct {
            uint32_t port;        /* Port Number */
            uint32_t sr_class;    /* Stream Class */
            uint32_t is_boundary; /* core port type */
        } port_boundary_set;
        struct {
            uint32_t port; /* Port Number */
        } port_boundary_get;
        struct {
            uint32_t bmp;        /* Mirror bitmap */
            uint32_t probe_port; /* MTP port */
            uint32_t mode;       /* Mirror mode - ingres/egress/both */
        } mirror_enable;
        struct {
            uint32_t dumbfwd_mode;  /* configure dumb forwarding mode */
        } dumbfwd_config;
        struct {
            uint32_t      port;
        } mgmt_phy_init;
        struct {
            uint32_t      port;
        } mgmt_port_type;
        struct {
            uint32_t     mode;
        } avnu_global_admin_mode;
        struct {
            uint32_t     clock_state;
        } avnu_clock_state;
        struct {
            uint32_t     timeout;
        } avnu_sync_absence;
        struct {
            uint32_t     port;
            uint32_t     mode;
        } avnu_ascapable;
        struct {
            uint32_t     port;
            uint32_t     role;
        } avnu_port_role;
        struct {
            uint32_t     port;
            uint32_t     pdelay_val;
        } avnu_nbr_pdelay;
        struct {
            uint32_t     port;
            int32_t      pdelay_intl_val;
        } avnu_pdelay_intl;
        struct {
            uint32_t     port;
            int32_t      sync_intl_val;
        } avnu_sync_intl;
        struct {
            uint32_t     port;
            int32_t      pdelay_oper_intl_val;
        } avnu_pdelay_oper;
        struct {
            uint32_t     port;
        }avnu_interface_config;
        struct {
            uint32_t     port;
        }avnu_port_status;
        struct {
            uint32_t port;
        } avnu_pdelay;
        struct {
            uint32_t idx;
        }vpd_get;
#ifdef ALLOW_VPD_SAVE
        struct {
            char key[VPD_KEY_LENGTH_MAX];
        }vpd_erase;
        struct {
            char key[VPD_KEY_LENGTH_MAX];
            char data[VPD_DATA_LENGTH_MAX];
            uint32_t length;
        }vpd_set;
#endif
        struct {
            uint32_t heartbeat_val;
        } heartbeat_interval;
        struct {
            uint32_t type;
        } buff_info;
    }u;
} mgmt_avb_request_t;

typedef struct mgmt_command_s {
    uint32_t            cmd;                    /* command */
    uint32_t            len;                    /* Length of entire packet */
    uint32_t            magic;                  /* MGMT_COMMAND_MAGIC */
    uint32_t            chksum;                 /* checksum of the whole struct */
    union {
        struct {
            uint32_t    number;             /* ping number */
        } ping;
        struct {
            uint32_t    state;              /* power state */
        } power;
        struct {
            uint32_t    spread;             /* PLL spread-spectrum param */
        } spread;
        struct {
            uint32_t    reason;             /* reason for reboot */
            uint32_t    delay_ms;           /* delay in milliseconds before rebooting */
        } reboot;
        struct {
            char        cmd[MGMT_SHELL_CMD_LEN];   /* shell command to execute */
        } shell_cmd;
        struct {
            uint32_t    size;               /* 1, 2, 4 or 8 bytes */
            uint32_t    address;            /* switch reg addr */
        } switch_read;
        struct {
            uint32_t    size;               /* 1, 2, 4 or 8 bytes */
            uint32_t    address;            /* switch reg addr */
            uint32_t    upper_regval;       /* switch reg value, upper bits */
            uint32_t    lower_regval;       /* switch reg value, lower bits */
        } switch_write;
#ifdef ALLOW_VPD_SAVE
        struct {
            uint8_t     addr[6];            /* MAC address */
            uint8_t     pad[2];
        } macaddr;
#endif
        struct {
            uint32_t    spi;                /* SPI flash # */
        } flash_info;
        struct {
            uint32_t      hz;
            uint32_t      hclk_freq;
            uint32_t      hclk_sel;
            uint32_t      pclk_freq;
            uint32_t      pclk_sel;
            uint32_t      p1div;
            uint32_t      p2div;
            uint32_t      ndiv;
            uint32_t      m1div;
            uint32_t      m2div;
            uint32_t      m3div;
            uint32_t      m4div;
            uint32_t      pll_num;
            uint32_t      frac;
            uint32_t      bclk_sel;
        } dmu;
        mgmt_avb_request_t avb;
    } u;
    char        pad[16];                                /* encryption padding */
} mgmt_command_t;

typedef struct mgmt_timestamp_s {
    uint32_t sec_u;
    uint32_t sec_l;
    uint32_t ns;
} mgmt_timestamp_t;

typedef struct mgmt_port_info_s {
    uint32_t   admin_mode;   /* Admin mode of the port - enabled/disabled */
    uint32_t   link_status;  /* Link status of the port - up/down */
    uint32_t   speed;        /* Current speed of the port - 10/100/Autoneg */
    uint32_t   br_mode;      /* BroadR-Reach mode - master/slave */
    uint32_t   jumbo_frame_mode;      /* Jumbo frmae mode - enabled/disabled*/
    uint32_t   loop_back_mode;        /* Loop back mode - enabled/disabled*/
    uint32_t   autoneg;/* Auto Neg - enabled/disabled*/
    uint32_t   autoneg_complete;/* Auto Neg - complete/pending*/
    uint32_t   duplex;/* duplex - full/half*/
    uint32_t   led;   /* led - on/off */
    uint32_t   link_state_change_cnt; /* no. of state changes */
} mgmt_port_info_t;

typedef struct
{
  uint32_t         rxIntfIndex; /* Port number on which sync was received */
  uint32_t         lastSeqId;  /* Seq Id of the rx sync message */
  uint32_t         followupCorrectionField_u; /* Incoming Correction field- upper 32bits */
  uint32_t         followupCorrectionField_l; /* Incoming Correction field- lower 32bits */
  int32_t          pduInterval; /* Interval value of rx sync msg */
  mgmt_timestamp_t preciseOriginTimestamp; /* GM timestamp */
  mgmt_timestamp_t syncRxTimestamp; /* Sync ingress timestamp */
  uint32_t         gmTimeBaseIndicator;
  char             lastGmPhaseChange[12];
  int32_t          lastGmFreqChange;
} mgmt_sync_info_t;

typedef struct mgmt_avnu_port_status_s {
    uint32_t   dot1as_capable;  /* Is port 802.1AS capable? */
    uint32_t   peer_delay;      /* Current peer delay */
    uint32_t   nbr_rate_ratio;  /* Current neighbor rate ratio */
    uint32_t   is_measuring_pdelay; /* Is port measuring PDELAY? */
    uint32_t   allowed_lost_responses; /* Number PDELAY responses allowed */
    uint32_t   port_role;       /* Port role determined by BMCA */
    uint32_t   sync_receipt_timeout;  /* Current SYNC RX timeout */
    char       sync_interval[20];   /* Current SYNC TX interval */
    char       pdelay_interval[20];   /* Current PDELAY TX interval */
    uint32_t   num_lost_responses; /* number of lost responses */
    char       pdelay_req_state[20];   /* pdelay request state */
    char       sync_receive_state[20]; /* sync receive state- enabled/disabled */
    uint32_t sync_tx_intl;
    uint32_t sync_rx_intl;
    uint32_t pDelaySeqId;
    uint32_t isAVnuPdelayConfigSaved;
    char partnerIdClock[8];
    uint32_t partnerIdPort;
    mgmt_sync_info_t syncRxInfo;
    uint32_t syncInfoAvailable;
    uint32_t syncTxSeqId;
    uint32_t signalingSeqId;
} mgmt_avnu_port_status_t;

typedef struct mgmt_avnu_port_stats_s {
    uint32_t   sync_tx;
    uint32_t   sync_rx;
    uint32_t   followup_tx;
    uint32_t   followup_rx;
    uint32_t   pdelay_req_tx;
    uint32_t   pdelay_req_rx;
    uint32_t   pdelay_resp_tx;
    uint32_t   pdelay_resp_rx;
    uint32_t   pdelay_followup_tx;
    uint32_t   pdelay_followup_rx;
    uint32_t   sync_rx_timeouts;
    uint32_t   sync_discards;
    uint32_t   pdelay_rx_timeouts;
    uint32_t   pdelay_discards;
    uint32_t   pdelay_lost_resp_exceeded;
    uint32_t   bad_headers;
    uint32_t   followup_rx_timeouts;
    uint32_t   followup_rx_discards;
    uint32_t   sync_tx_timeouts;
    uint32_t   pdelay_req_rx_discards;
    uint32_t   pdelay_resp_rx_discards;
    uint32_t   bad_pdelay_values;
    uint32_t   signaling_rx_count;
    uint32_t   signaling_tx_count;
    uint32_t   signaling_rx_discards;
    uint32_t   sync_too_fast;
    uint32_t   tx_errors;
    uint32_t   ts_errors;
    uint32_t   ptp_discards;
} mgmt_avnu_port_stats_t;

typedef struct mgmt_acd_results_s {
    uint32_t      fault;                  /* ECD cable fault register */
    uint32_t      length;
    uint32_t      status;
} mgmt_acd_results_t;

typedef struct mgmt_reply_avb_sw_version_s {
    char      avb_version[VERSION_STR_LEN];
} mgmt_reply_avb_sw_version_t;

typedef struct mgmt_reply_sdk_sw_version_s {
    char      os_version[VERSION_STR_LEN];
} mgmt_reply_os_sw_version_t;

typedef struct mgmt_vlan_port_info_s {
    uint32_t iFilter;  /* ingress filter value */
    uint32_t pvid;     /* untageed vlan get */
    uint32_t priority; /* priority */
} mgmt_vlan_port_info_t;

typedef struct mgmt_avnu_interface_config_s {
    uint32_t port_role;
    uint32_t asCapable_mode;
    uint32_t pdelay_val;
    int32_t  pdelay_init_interval;
    int32_t  sync_init_interval;
    int32_t  pdelay_oper_interval;
    uint32_t syncTimeout;
    uint32_t lostResponse;
    int32_t avnuNbrRateRatio;
} mgmt_avnu_interface_config_t;

typedef struct mgmt_avnu_global_config_s {
    uint32_t clock_state; /* clock state GM/Slave */
    uint32_t sync_absence_timeout; /* sync absence timeout*/
    uint32_t admin_mode;    /* Global admin mode */
    uint32_t syncPropTime;  /* Sync Propagation time */
    uint32_t pdelayRespTime;/* Pdelay response time */
} mgmt_avnu_global_config_t;

typedef struct mgmt_avnu_global_status_s {
    uint32_t clock_type;
    uint32_t dot1as_status;
    uint32_t gm_status;
    uint32_t slave_port;
    uint32_t rateRatioIn;
    uint32_t rateRatioOut;
    uint32_t isSyncReceived;
    mgmt_timestamp_t bridgeLocalTime;
    uint32_t   local_clk_id_upper; /* Local clock identity */
    uint32_t   local_clk_id_lower;
} mgmt_avnu_global_status_t;

typedef struct mgmt_avnu_pdelay_s {
    mgmt_timestamp_t pDelayT1;
    mgmt_timestamp_t pDelayT2;
    mgmt_timestamp_t pDelayT3;
    mgmt_timestamp_t pDelayT4;
    uint32_t nbrPDelay;
    uint32_t nbrRR;
    char dot1asCapable[8];
} mgmt_avnu_pdelay_t;

typedef struct mgmt_avb_async_events_get_s {
   uint32_t port0_events;
   uint32_t port1_events;
   uint32_t port2_events;
   uint32_t port3_events;
   uint32_t port4_events;
   uint32_t port5_events;
   uint32_t port6_events;
   uint32_t port8_events;
   uint32_t global_events;
} mgmt_avb_async_events_get_t;

typedef union  {
    mgmt_port_info_t port_info;
    mgmt_avnu_port_status_t avnu_port_status;
    mgmt_avnu_port_stats_t avnu_port_stats;
    mgmt_avnu_pdelay_t avnu_pdelay;
    mgmt_acd_results_t mgmt_acd;           /* ACD results */
    mgmt_reply_avb_sw_version_t avb_sw_version; /* AVB software version */
    mgmt_vlan_port_info_t vlan_port_info;  /* per port vlan properties */
    mgmt_avnu_interface_config_t avnu_interface_config;
    mgmt_avnu_global_config_t avnu_global_config;
    mgmt_avnu_global_status_t avnu_global_status;
    mgmt_avb_async_events_get_t avb_async_events;

    struct {
      uint32_t sqi_reg_val; /* SQI reg val */
    }mgmt_port_link_sqi;

    struct {
        uint32_t value;   /* Switch age time */
    } switch_age_time;

    struct {
        uint32_t value;   /* Switch age time */
    } switch_mtu;
    struct {
        uint32_t value; /* 0=Disabled, 1=Enabled */
    } port_admin_mode;
    struct {
        uint32_t value; /* 0=Autoneg, 10, 100, 1000 */
    } port_speed;
    struct {
        uint32_t value; /* 0=slave, 1=master, 2=auto 3=none */
    } br_mode;
    struct {
        uint32_t value; /* 0=Disabled, 1=Enabled */
    } port_phy_lb_mode;
    struct {
        uint32_t value; /* 0=Disabled, 1=Enabled */
    } port_jumbo_frame_mode;

    struct {
        uint32_t   port_bitmap;  /* VLAN membership */
        uint32_t   tag_bitmap;   /* Tagged bitmap */
    } vlan_info;

    struct {
        uint32_t   rate_kbps;    /* Rate in kpbs */
        uint32_t   burst_kbits;  /* Burst in kbits */
    } bw_info;

    struct {
        uint32_t prio;       /* Priority of class */
        uint32_t remap_prio; /* Remap priority of class */
    } pcp_mapping;
    struct {
        uint32_t hash_index;  /* hash index value */
    } hash_info_get;
    struct {
        uint32_t dumbfwd_mode; /*  get dumbfwd mode */
    } dumbfwd_status;
    struct {
        uint32_t     port_type; /* get physical port type*/
    } mgmt_port_type;
    struct {
        uint32_t bdyA;  /* Boundary value A */
        uint32_t bdyB;  /* Boundary value B */
    } port_boundary_get;
    struct {
        char key[VPD_KEY_LENGTH_MAX];
        char data[VPD_DATA_LENGTH_MAX];
        uint32_t length;
    } vpd_get;
    struct {
        uint32_t state;
    } avnu_device_state_get;
    struct {
        uint32_t addr;  /* location of the buffer in TCM */
        uint32_t size;  /* size of the buffer */
    } buff_info;
    struct {
        uint32_t mode;      /* enabled or disabled */
        uint32_t port_mask; /* bitmap of ports to mirror */
        uint32_t probe;     /* probe port */
        uint32_t traffic;   /* Traffic direction ingress, egress or all */
    } mirror_status;
} mgmt_reply_avb_t;

typedef struct mgmt_reply_s {
    uint32_t            status;                 /* Used in replies */
    uint32_t            len;                    /* Length of entire packet */
    uint32_t            magic;                  /* MGMT_REPLY_MAGIC */
    uint32_t             chksum;                 /* negated checksum */
    union {
        mgmt_reply_os_sw_version_t os_sw_version; /* OS software version */
        mgmt_reply_avb_t avb;  /* Command response for AVB */
    } u;
    char        pad[16];                        /* AES encryption padding */
} mgmt_reply_t;

/*
 * SW spare registers 14 & 15 are used to exchange command and reply buffer
 * information respectively between Host and BCM8953X.
 * SW_SPARE14/SW_SPARE15 field description:
 *      [15:1]- Buffer Address[19:5]
 *              Addressable range : 0x0000_0020 - 0x000f_0000
 *              Alignment : 32byte
 *      [0]- Queued status
 *              1 : Queued
 *              0 : Available
 */
#define MSG_QUEUED          0x1
#define MSG_BUFFER          0xfffe
#define MSG_BUFFER_SHIFT    0x4
#define CMD_BUFFER_MAX      512
#define REPLY_BUFFER_MAX    512

#endif
