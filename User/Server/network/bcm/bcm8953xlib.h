#ifndef BCM8953XLIB_H
#define BCM8953XLIB_H

#include "types.h"
#include "bcm89530_rdb.h"
#include "bcm8953x.h"
#include "bcm8953x_spi.h"
#include "mgmt.h"
#include "acd_mgmt.h"

/* Byte swap macros for communicating with the uKernel */
#ifdef BE_HOST
#define uswap16(x)              bswap_16(x)
#define uswap32(x)              bswap_32(x)
#define uswap64(x)              bswap_64(x)
#else
#define uswap16(x)              (x)
#define uswap32(x)              (x)
#define uswap64(x)              (x)
#endif
#endif /* defined(LINUX) && !defined(__KERNEL__) */

#define ROUND_UP(x,y)           ((((uint32)(x)) + ((uint32)(y)) - 1) \
        & ~(uint32)((y) - 1))

#define MGMT_CMD_INT_PARSE_STATUS(status)  if(status == MGMT_AVB_FAILURE){  \
    goto done;                    \
}                                \

#ifndef TRUE
#define TRUE        1
#endif

#ifndef FALSE
#define FALSE       0
#endif

#define ERR_PARAM   (-1)
#define ERR_NONE    0

#define SRST_CHIP	0x8001
#define FW_EXECUTED 0x8000

#define INSTALL_MODE_ALL    (1)
#define INSTALL_MODE_FW     (2)
#define INSTALL_MODE_AVB    (3)
#define INSTALL_MODE_VPD    (4)
#define INSTALL_MODE_USRDATA (5)

/* To align to 64-bit */
#define CEIL_64(d) (8*(((uint32)(d)+7)/8))
#define CEIL_32(d) (4*(((uint32)(d)+3)/4))

#define TCM_START_ADDR	            (0x0)
#define TCM_SIZE	                (480 * 1024)
#define FLASH_PROGRAM_TIMEOUT_SEC   (5*60) /* 5minutes */
#define MSEC_PER_SEC                (1000)
#define USEC_PER_MSEC               (1000)

#define MGMT_BCM895XX_TIMEOUT_VAL  1000

#define MGMT_AVB_INTF_P0           0
#define MGMT_AVB_INTF_P1           1
#define MGMT_AVB_INTF_P2           2
#define MGMT_AVB_INTF_P3           3
#define MGMT_AVB_INTF_P4           4
#define MGMT_AVB_INTF_P5           5
#define MGMT_AVB_INTF_P6           6

/* VLAN min max values */
#define MGMT_AVB_VLAN_MIN   0x0001
#define MGMT_AVB_VLAN_MAX   0x0FFE
#define MGMT_AVB_VLAN_VALID(vlan) (((vlan) >= MGMT_AVB_VLAN_MIN) && ((vlan) <= MGMT_AVB_VLAN_MAX))

#define MGMT_BCM895XX_MAX_NUM_PORTS         BCM895XX_MAX_NUM_PORTS
#define MGMT_BCM895XX_ALL_PORTS             (MGMT_BCM895XX_MAX_NUM_PORTS + 1)
#define MGMT_AVB_MAX_INTERFACES             AVB_MAX_INTERFACES
#define MGMT_AVB_MAX_BR_PORT                AVB_MAX_BR_PORTS
#define MGMT_AVB_MAX_INTERFACES_PHYS        AVB_MAX_INTERFACES_PHYS   /* BR0-BR4, Port5 & 6 */
#define MGMT_AVB_INTF_P8_INTERNAL           AVB_INTF_P8_INTERNAL       /* Internal Port8 Interface */
#define MGMT_AVB_INTF_P8                    AVB_INTF_P8       /* External Port8 Interface */
#define MGMT_AVB_MAX_INTERFACES_PHYS_P8     AVB_MAX_INTERFACES_PHYS_P8 /* BR0-BR4, Port5 & 6, Port8 */
/* TRUE if the interface is a Valid AVB Physical Interface BR0-BR4, P5, P6, P8 */
#define AVB_PHYS_INTF_IS_VALID(intf) (((intf) < AVB_MAX_INTERFACES_PHYS) || ((intf) == AVB_INTF_P8))
#define MGMT_AVB_PHYS_INTF_IS_VALID(intf)   AVB_PHYS_INTF_IS_VALID(intf)
#define IS_EXTERNAL_INTF_INVALID(extIntf)   ((extIntf == MGMT_AVB_MAX_INTERFACES_PHYS) || (extIntf > MGMT_AVB_INTF_P8))

/* Maximum size of the VPD data */
#define MGMT_VPD_DATA_OFFSET                   2
#define MGMT_VPD_VALID                         1
#define MGMT_VPD_MAGIC                         0xd2b42d4b
#define MGMT_VPD_HEADER_VERSION                1
#define MGMT_ETHER_ADDR_LEN                    6
#define MGMT_SERIAL_NUMBER_DEFUALT_LEN         8
#define MGMT_SERIAL_NUMBER_DEFAULT_VAL         "0000000"
#define MGMT_VPD_ENTRIES_MAX                   10
#define MGMT_VPD_KEY_LENGTH_MAX                16
#define MGMT_VPD_DATA_LENGTH_MAX              (SPIFLASH_PAGE_SIZE - sizeof(uint16) - MGMT_VPD_KEY_LENGTH_MAX)
#define MGMT_MAC_ADDRESS_STRING                "macaddr"
#define MGMT_SERIAL_NUMBER_STRING              "serial"

#define MGMT_AVB_TRACE_LINE(n)                                \
    do {                                                        \
        uint32 i;                                                 \
        for (i=0; i < (n); i++) { printk("-"); }                  \
        printk("\n");                                             \
    } while(0)                                                  \


#define MGMT_AVNU_SYNC_INIT_TX_LOWER_LIMIT         -5
#define MGMT_AVNU_SYNC_INIT_TX_UPPER_LIMIT         -3
#define MGMT_AVNU_PDELAY_INIT_VAL                   0
#define MGMT_PDELAY_TX_DISABLE_VAL                  127
#define MGMT_SYNC_RX_LOWER_LIMIT                    2
#define MGMT_SYNC_RX_UPPER_LIMIT                    255
#define MGMT_AVB_VERSION_SUFFIX_STRING_MAX_LEN      8
#define SPIFLASH_PAGE_SIZE                          256
#define BIT(x)                                      (1 << (x))

#define MGMT_DOT1AS_INTL_US_TO_MS(us)               ((us) / 1000)

#define MGMT_CRASH_LOG_ENTRIES_MAX                   3
#define MGMT_CRASH_LOG_VALID                         0x05050505
#define MGMT_LOG_MAGIC                               0xf2d4b698


#define MGMT_VPD_KEY_CHAR_SET(ch)          ((ch == '-') || (ch == '_') || (ch == '.') || ((ch >= 'a') && (ch <= 'z')) || ((ch >= 'A') && (ch <= 'Z')) || ((ch >= '0') && (ch <= '9')))
#define MGMT_VPD_DATA_CHAR_SET(ch)         ((ch == '-') || (ch == '_') || (ch == '.') || ((ch >= 'a') && (ch <= 'z')) || ((ch >= 'A') && (ch <= 'Z')) || ((ch >= '0') && (ch <= '9')))
#define MGMT_AVNU_CLOCK_STATE_GM                     1
#define MGMT_AVNU_CLOCK_STATE_SLAVE                  3
#define MGMT_AVNU_AS_CAPABLE_TRUE                    1
#define MGMT_AVNU_AS_CAPABLE_FALSE                   0
#define MGMT_AVNU_PDELAY_INIT_INTERVAL_IS_NOT_VALID(val) ((val != MGMT_AVNU_PDELAY_INIT_VAL && val != MGMT_PDELAY_TX_DISABLE_VAL))
#define MGMT_AVNU_SYNC_INIT_INTERVAL_IS_NOT_VALID(val)   ((val < MGMT_AVNU_SYNC_INIT_TX_LOWER_LIMIT) || (val > MGMT_AVNU_SYNC_INIT_TX_UPPER_LIMIT))
#define MGMT_AVNU_SYNC_ABSENCE_TIMEOUT_MIN           1
#define MGMT_AVNU_SYNC_ABSENCE_TIMEOUT_MAX           20
#define MGMT_AVNU_INTF_OPER_PDELAY_INTL_MIN          0
#define MGMT_AVNU_INTF_OPER_PDELAY_INTL_MAX          3
#define MGMT_AVNU_OPERATIONAL_PDELAY_INTERVAL_IS_NOT_VALID(val) (((val < MGMT_AVNU_INTF_OPER_PDELAY_INTL_MIN) || (val > MGMT_AVNU_INTF_OPER_PDELAY_INTL_MAX)) && (val != MGMT_PDELAY_TX_DISABLE_VAL))

#define MGMT_AVB_VLAN_TAG                             1
#define MGMT_AVB_VLAN_UNTAG                           0
#define MGMT_AVB_INGRESS_FILTER_MODE_ENABLE           1
#define MGMT_AVB_INGRESS_FILTER_MODE_DISABLE          0
#define MGMT_AVB_DOT1Q_INTF_PRIO_MIN                  0
#define MGMT_AVB_DOT1Q_INTF_PRIO_MAX                  7


#define MGMT_AVB_PORT_SPEED_0                         0
#define MGMT_AVB_PORT_SPEED_100                       100
#define MGMT_AVB_PORT_SPEED_1000                      1000
#define MGMT_AVB_MAX_BURST                            0x7FFFE00
#define MGMT_AVB_RECOMMENDED_BANDWIDTH(speed)         ((speed/100) * 75 * 1000)
#define MGMT_SWITCH_AGE_TIME_MAX                      1048575
#define MGMT_AVB_PDELAY_ALLOWED_LOST_RESP_MAX         65535
#define MGMT_GLOBAL_HEARTBEAT_INTERVAL_MIN            100   /* in milli seconds */
#define MGMT_GLOBAL_HEARTBEAT_INTERVAL_MAX            20000 /* in milli seconds */
#define MGMT_AVNU_SYNC_INTERVAL_IS_NOT_VALID(val)     ((val < MGMT_AVNU_SYNC_INIT_TX_LOWER_LIMIT) || (val > MGMT_AVNU_SYNC_INIT_TX_UPPER_LIMIT))
#define MGMT_AVB_PORT_BOUNDARY                        1
#define MGMT_AVB_PORT_CORE                            0

#define MGMT_AVB_ARL_MATCH_ALL_PORTS     0xFF
#define MGMT_AVB_ARL_MATCH_ALL_VLANS     0x0

#define MGMT_DOWNLOAD_MAX_IMGS (16UL)

extern uint16_t mgmtavbCfgxIntfNum[AVB_MAX_INTERFACES];

typedef enum {
  MGMT_AVB_DOT1AS_ADMIN_DISABLE = 0,
  MGMT_AVB_DOT1AS_ADMIN_ENABLE  = 1
}MGMT_AVB_DOT1AS_ADMIN_MODE_t;

/*avnu device status*/
typedef enum {
  MGMT_AVNU_DEVICE_STATE_NOT_APPLICABLE        = 0,
  MGMT_AVNU_DEVICE_STATE_ETH_READY             = 1,
  MGMT_AVNU_DEVICE_STATE_AVB_SYNC              = 2,
  MGMT_AVNU_DEVICE_STATE_SYNC_RECEIPT_TIMEOUT  = 3,
} MGMT_AVB_AVNU_DEVICE_STATE_t;

/* avnu 8021as Status */
typedef enum
{
    MGMT_DOT1AS_AVNU_DISABLED = 0,
    MGMT_DOT1AS_AVNU_AUTOMOTIVE = 1
}MGMT_DOT1AS_AVNU_STATUS_t;

/* avnu GM Status */
typedef enum
{
    MGMT_AVNU_GM_STARTUP_ABSENT = 0,
    MGMT_AVNU_GM_OPERATIONAL_ABSENT = 1,
    MGMT_AVNU_GM_UNDETERMINED = 2,
    MGMT_AVNU_GM_DISABLED = 3,
    MGMT_AVNU_GM_NO_SLAVE_PORT = 4,
    MGMT_AVNU_GM_PRESENT = 5
}MGMT_AVNU_GM_STATUS_t;

/*avnu interface status*/
typedef enum
{
    MGMT_AVNU_ASCAPABLE_STATUS_PORT_DOWN = 0,
    MGMT_AVNU_ASCAPABLE_STATUS_FALSE = 1,
    MGMT_AVNU_ASCAPABLE_STATUS_TRUE = 2
}MGMT_AVNU_ASCAPABLE_STATUS_t;

typedef struct mgmt_log_header_s {
    uint32  magic;
    uint32  version;
    uint32  crashlog_data_offset;
    uint32  crashlog_data_length;
    uint32  crashlog_entries_max;
    uint32  crashlog_cksum;
    uint32  log_data_offset;
    uint32  log_data_length;
    uint32  log_entries;
    uint32  log_cksum;
    uint32  hdr_cksum;
} __attribute__ ((packed)) mgmt_log_header_t;


typedef struct mgmt_crash_log_data_s {
    uint32  valid_entry_indicator;
    uint32  exception_type;
    uint32  link_register_value;
    uint64  uptime;
    uint32  code_version;
} __attribute__ ((packed)) mgmt_crash_log_data_t;


typedef enum {
    MGMT_PORT_TYPE_BR         = BIT(0),
    MGMT_PORT_TYPE_FE         = BIT(1),
    MGMT_PORT_TYPE_GE_SERDES  = BIT(2),
    MGMT_PORT_TYPE_IMP        = BIT(3),
}mgmt_port_type_t;

typedef enum {
    MGMT_AVB_FALSE = 0,
    MGMT_AVB_TRUE = 1
}MGMT_AVB_BOOL_t;

typedef enum {
    MGMT_AVB_SUCCESS = 0,
    MGMT_AVB_FAILURE = 1
}MGMT_AVB_RC_t;


typedef enum {
    DISABLED = 0,
    ENABLED = 1
}MGMT_MODE_t;

typedef enum {
    SLAVE = 0,
    MASTER = 1,
    AUTO = 2,
    NONE = 3
} MGMT_BR_MODE_t;

typedef enum {
    LINK_DOWN = 0,
    LINK_UP  = 1
}MGMT_LINK_STATUS_t;

typedef enum {
    AUTO_NEG_COMPLETE = 0,
    AUTO_NEG_PENDING  = 1
}MGMT_AUTO_NEG_STATUS_t;

typedef enum {
    HALF_DUPLEX = 0,
    FULL_DUPLEX = 1
}MGMT_DUPLEX_STATUS_t;

/* AVB stream class */
typedef enum MGMT_AVB_STREAM_CLASS_TYPE_s {
    MGMT_AVB_STREAM_CLASS_TYPE_A = 1,      /* AVB Class A */
    MGMT_AVB_STREAM_CLASS_TYPE_B,          /* AVB Class B */
} MGMT_AVB_STREAM_CLASS_TYPE_t;

typedef enum {
    MGMT_NO_CONNECTION,
    MGMT_SPI_CONNECTION,
} mgmt_connection_t;

/* 802.1AS Port Roles */
typedef enum
{
    MGMT_DOT1AS_ROLE_MASTER = 6,
    MGMT_DOT1AS_ROLE_SLAVE = 9
} MGMT_DOT1AS_PORT_ROLE_t;

/* 802.1AS transmit and receive intervals */
typedef enum
{
    MGMT_DOT1AS_INITIAL_SYNC_TX_INTERVAL = 0,
    MGMT_DOT1AS_CURRENT_SYNC_TX_INTERVAL,
    MGMT_DOT1AS_INITIAL_PDELAY_TX_INTERVAL,
    MGMT_DOT1AS_CURRENT_PDELAY_TX_INTERVAL,
    MGMT_DOT1AS_SYNC_RX_TIMEOUT,
    MGMT_DOT1AS_PDELAY_RX_TIMEOUT,
    MGMT_DOT1AS_SYNC_RX_TIMEOUT_INTERVAL,
} MGMT_DOT1AS_INTF_INTERVAL_t;

/* Return codes for VPD API */
typedef enum {
    MGMT_VPD_SUCCESS = 0,
    MGMT_VPD_FAILURE,
    MGMT_VPD_CHECKSUM_FAIL,
    MGMT_VPD_FLASH_READ_FAIL,
    MGMT_VPD_FLASH_WRITE_FAIL
} MGMT_VPD_RV_t;

typedef struct mgmt_vpd_entry_s{
    uint8 key[MGMT_VPD_KEY_LENGTH_MAX];          /* vpd key . a key of all zeros indicates an empty/available vpd entry*/
    uint16 length;                               /* length of the vpd data written */
    uint8 data[MGMT_VPD_DATA_LENGTH_MAX];       /* vpd data */
} mgmt_vpd_entry_t;

typedef struct mgmt_info_s {
    /* Indicator of connection type */
    mgmt_connection_t  connection_type;

    /* SPI connection datastructures */
    int32  spifd;
    int32  arl_mode;
    int32  linkscan;
    int32  counter;
    uint32  counter_flags;
    uint32  counter_pbmp;
} mgmt_info_t;

typedef struct _ImgDesc {
    uint8 *buf;
    int32 sz;
} ImgDesc;

int32 mgmt_get_chip_info(uint16 *rev, uint32 *dev);
int32 mgmt_open_spi(mgmt_info_t *info);

int32 mgmt_write_memory(mgmt_info_t *info, uint32 address, uint8 *data);
int32 mgmt_reboot(mgmt_info_t *info, uint32 reason);
int32 mgmt_set_spi_id(mgmt_info_t *info, SPI_ID_t id);
int32 mgmt_write(mgmt_info_t *info, uint32 addr, uint32 mode, uint64 data);
int32 mgmt_read(mgmt_info_t *info, uint32 addr, uint32 mode, uint64 *data);
int32 mgmt_pll_spread(mgmt_info_t *info, uint32 spread);

void *buffer_from_file(int8 *name, int32 *len);
int32 mgmt_read_switch_register(mgmt_info_t *info, uint32 addr,
        uint32 size, uint64 *regval);
int32 mgmt_write_switch_register(mgmt_info_t *info, uint32 addr,
        uint32 size, uint64 regval);

/* AVB management */
int32 mgmt_port_info_get(mgmt_info_t *info, uint32 port, mgmt_port_info_t *port_info);
int32 mgmt_port_admin_mode_set(mgmt_info_t *info,uint32 port, uint32 mode);
int32 mgmt_port_admin_mode_get(mgmt_info_t *info,uint32 port, uint32 *mode);
int32 mgmt_port_speed_set(mgmt_info_t *info, uint32 port, uint32 speed);
int32 mgmt_port_speed_get(mgmt_info_t *info, uint32 port, uint32 *speed);
int32 mgmt_port_master_slave_set(mgmt_info_t *info,uint32 port, uint32 master_slave);
int32 mgmt_port_master_slave_get(mgmt_info_t *info,uint32 port, uint32 *master_slave);
int32 mgmt_port_phy_lb_set(mgmt_info_t *info, uint32 port, uint32 phy_lb_mode);
int32 mgmt_port_phy_lb_get(mgmt_info_t *info, uint32 port, uint32 *phy_lb_mode);
int32 mgmt_port_jumbo_frame_mode_set(mgmt_info_t *info, uint32 port, uint32 jumbo_frame_mode);
int32 mgmt_port_jumbo_frame_mode_get(mgmt_info_t *info, uint32 port, uint32 *jumbo_frame_mode);
int32 mgmt_port_dumbfwd_mode_get(mgmt_info_t *info, uint32 *dumbfwd_mode);
int32 mgmt_port_dumbfwd_mode_set(mgmt_info_t *info, uint32 dumbfwd_mode);
int32 mgmt_port_type_get(mgmt_info_t *info, uint32 port, uint32 *port_type);
int32 mgmt_port_link_sqi_get(mgmt_info_t *info, uint32 port, uint32 *sqi);


int32 mgmt_switch_age_time_set(mgmt_info_t *info, uint32 age_time);
int32 mgmt_switch_age_time_get(mgmt_info_t *info, uint32 *age_time);

int32 mgmt_buffer_info(mgmt_info_t *info, uint32_t type, uint32 *addr, uint32 *size);

/* VLAN */
int32 mgmt_vlan_create(mgmt_info_t *info, uint32 vlan);
int32 mgmt_vlan_delete(mgmt_info_t *info, uint32 vlan);
int32 mgmt_vlan_get(mgmt_info_t *info, uint32 vlan, uint32 *bitmap, uint32 *tag);
int32 mgmt_vlan_portadd(mgmt_info_t *info, uint32 vlan, uint32 port, uint32 tag);
int32 mgmt_vlan_portdel(mgmt_info_t *info, uint32 vlan, uint32 port);
int32 mgmt_vlan_pvid_set(mgmt_info_t *info, uint32 port, uint32 pvid);
int32 mgmt_vlan_ifilter_set(mgmt_info_t *info, uint32 ifilter);
int32 mgmt_vlan_priority_set(mgmt_info_t *info, uint32 port, uint32 priority);
int32 mgmt_vlan_port_info_get(mgmt_info_t *info, uint32 port, mgmt_vlan_port_info_t *vlan_port_info);

/* FDB */
int32 mgmt_fdb_entry_add(mgmt_info_t *info, int8 *mac_addr, uint32 vlan,
                         uint32 dest_port_bmp, uint32 cir_kbps, uint32 cbs_bytes,
                         uint32 ingress_port_bmp);
int32 mgmt_fdb_entry_delete(mgmt_info_t *info, int8 *mac_addr, uint32 vlan);
int32 mgmt_fdb_entry_get(mgmt_info_t *info,uint32 vlan,uint32 port);
int32 mgmt_fdb_boundary_set(mgmt_info_t *info, uint32 port, uint32 sr_class, uint32 is_boundary);
int32 mgmt_fdb_boundary_get(mgmt_info_t *info, uint32 port, uint32 *bdyA, uint32 *bdyB);
/* Mirroring */
int32 mgmt_mirror_enable(mgmt_info_t *info, uint32 port_bitmap, uint32 probe_port, MGMT_INTF_MIRROR_MODE_t mode);
int32 mgmt_mirror_disable(mgmt_info_t *info);
int32 mgmt_mirror_status(mgmt_info_t *info, uint32_t *mode, uint32_t *port_mask, uint32_t *probe, uint32_t *traffic);

/* Stream */
int32 mgmt_stream_bandwidth_get(mgmt_info_t *info, int8 *mac_addr, uint32 vlan,
                               uint32 *rate, uint32 *burst);
int32 mgmt_port_bandwidth_get(mgmt_info_t *info, uint32 port, uint32 sr_class,
        uint32 *rate, uint32 *burst);
int32 mgmt_port_bandwidth_set(mgmt_info_t *info, uint32 port, uint32 sr_class,
        uint32 rate, uint32 burst);
int32 mgmt_pcp_mapping_get(mgmt_info_t *info, uint32 sr_class, uint32 *prio, uint32 *remap);
int32 mgmt_pcp_mapping_set(mgmt_info_t *info, uint32 sr_class, uint32 prio, uint32 remap);

int32 mgmt_macaddr_get(mgmt_info_t *info, uint8 *mac_addr);
int32 mgmt_os_version_get(mgmt_info_t *info, char *ver);
int32 mgmt_avb_version_get(mgmt_info_t *info, char *ver);
int32 mgmt_userdata_status_get(mgmt_info_t *info, uint16 *val);
int32 mgmt_fw_status_get(mgmt_info_t *info, uint16 *val);
MGMT_VPD_RV_t mgmt_vpd_get(mgmt_info_t *info, char *key);
MGMT_VPD_RV_t mgmt_vpd_erase(mgmt_info_t *info, char *key);
MGMT_VPD_RV_t mgmt_vpd_set(mgmt_info_t *info, char *key, uint16 length, char *data);
void mgmt_convert_mac(int8 *mac_addr, uint8 *mac_hex, int32 *status);

/* AVNU */
int32 mgmt_avnu_global_admin_mode_set(mgmt_info_t *info, uint32 mode);
int32 mgmt_avnu_port_status_get(mgmt_info_t *info, uint32 port,
        mgmt_avnu_port_status_t *status);
int32 mgmt_avnu_port_stats_get(mgmt_info_t *info, uint32 port,
        mgmt_avnu_port_stats_t *stats);
int32 mgmt_avnu_port_stats_clear(mgmt_info_t *info, uint32 port);
int32 mgmt_avnu_port_num_lstresp_set(mgmt_info_t *info, uint32 port, uint32 last_resp);

int32 mgmt_avnu_clock_state_set(mgmt_info_t *info, uint32 state);
int32 mgmt_avnu_sync_absence_timeout_set(mgmt_info_t *info, uint32 syncAbsenceTimeout);
int32 mgmt_avnu_as_capable_mode_set(mgmt_info_t *info, uint32 port, uint32 ascapable);
int32 mgmt_avnu_port_role_set(mgmt_info_t *info, uint32 port, uint32 role);
int32 mgmt_avnu_nbr_pdelay_set(mgmt_info_t *info, uint32 port, uint32 pdelay_val);
int32 mgmt_avnu_initial_log_pdelay_interval_set(mgmt_info_t *info, uint32 port, int32 interval);
int32 mgmt_avnu_initial_log_sync_interval_set(mgmt_info_t *info, uint32 port, int32 interval);
int32 mgmt_avnu_global_config_get(mgmt_info_t *info, mgmt_avnu_global_config_t *config);
int32 mgmt_avnu_interface_config_get(mgmt_info_t *info,uint32 port, mgmt_avnu_interface_config_t *config);
int32 mgmt_avnu_operational_log_pdelay_interval_set(mgmt_info_t *info, uint32 port, int32 log_val);
int32 mgmt_avnu_global_status(mgmt_info_t *info, mgmt_avnu_global_status_t *avnu_status);
int32 mgmt_avnu_device_state_get(mgmt_info_t *info, uint32 *state);
int32 mgmt_heartbeat_set(mgmt_info_t *info, uint32 heartbeat);
int32 mgmt_avb_async_events_get(mgmt_info_t *info, mgmt_avb_async_events_get_t *events);
int32 mgmt_avnu_pdelay_get(mgmt_info_t *info, uint32_t port, mgmt_avnu_pdelay_t *avnu_pdelay);

void acd_write16(uint32 addr, uint16 data);
uint16 acd_read16(uint32 addr);
MGMT_VPD_RV_t mgmt_vpd_data_validate(char *data);
MGMT_VPD_RV_t mgmt_vpd_key_validate(char *key);

int32 mgmt_spi_wr16(uint32 addr, uint16 data);
int32 mgmt_spi_wr32(uint32 addr, uint32 data);
int32 mgmt_spi_rd16(uint32 addr, uint16 *rd_val);
int32 mgmt_spi_rd32(uint32 addr, uint32 *rd_val);
int32 mgmt_spi_wr16_ind(uint32 addr, uint16 data);
int32 mgmt_spi_wr32_ind(uint32 addr, uint32 data);
int32 mgmt_spi_wr64_ind(uint32 addr, uint64 data);
int32 mgmt_spi_wr8_ind(uint32 addr, uint8 data);
