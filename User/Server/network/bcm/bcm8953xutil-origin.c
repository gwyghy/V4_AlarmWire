/*
*********************************************************************************************************
*	                                            DoubleWires
*                           Commuication Controller For The Mining Industry
*
*                                  (c) Copyright 1994-2013  HNDZ 
*                       All rights reserved.  Protected by international copyright laws.
*
*
*    File    : bcm8953xuntil.c
*    Module  : user
*    Version : V1.1.0.0
*    Brief   : 
*    History : 
*   -----------------
*              Version  Date           By            Note
*              v1.0                    bting
*
*********************************************************************************************************
*/

#include  <includes.h>
#include "bsp.h"
#include "bcm89530_rdb.h"
#include "bcm8953xlib.h"
#include "bsp_shell.h"
#include "mgmt.h"

#include "types.h"
#include "parse.h"
#include "sys_msg_def.h"


#if (SERVER_NETWORK_BCM_EN==1)

#define DOT1AS_DEBUG_TIMESYNC_LOG_LEN  120


#define scan_start(scan)        ((scan)->scan_tos = -1)
#define scan_ungetc(scan)       ((scan)->scan_ptr[(scan)->scan_tos]--)

/*
 * Typedef:     cmd_result_t
 * Purpose:    Type retured from all commands indicating success, fail,
 *        or print usage.
 */
typedef enum cmd_result_e {
    CMD_OK   = 0,            /* Command completed successfully */
    CMD_FAIL = -1,            /* Command failed */
    CMD_USAGE= -2,            /* Command failed, print usage  */
    CMD_NFND = -3,            /* Command not found */
    CMD_EXIT = -4,            /* Exit current shell level */
    CMD_INTR = -5,            /* Command interrupted */
    CMD_NOTIMPL = -6,         /* Command not implemented */
    CMD_USAGE_PORT = -7,
    CMD_USAGE_VLAN = -9,
    CMD_USAGE_MSRP = -10,
    CMD_USAGE_MIRROR = -11,
    CMD_USAGE_BANDWIDTH = -12,
    CMD_USAGE_FDBENTRY = -13,
    CMD_USAGE_SWITCH  = -14,
    CMD_USAGE_APS    = -15,
    CMD_MGM_SESSION_FAIL = -16,
    CMD_SPI_INIT_FAIL = -17,
    CMD_SPI_OPEN_FAIL = -18,
    CMD_USAGE_FLASH = -22,
    CMD_USAGE_LOG = -23,
    CMD_USAGE_VPD = -24,
    CMD_USAGE_AVNU = -25,
    CMD_USAGE_ACD = -26,
    CMD_USAGE_HEARTBEAT = -27,
    CMD_USAGE_CONFIG = -28,
} cmd_result_t;
/*
 * Typedef:    cmd_func_t
 * Purpose:    Defines command function type
 */
typedef cmd_result_t (*cmd_func_t)(int32, args_t *);

/*
 * Typedef:    cmd_t
 * Purpose:    Table command match structure.
 */
typedef struct cmd_s {
    parse_key_t    c_cmd;         /* Command string */
    cmd_func_t    c_f;            /* Function to call */
    const int8    *c_usage;       /* Usage string */
    const int8    *c_help;        /* Help string */
} cmd_t;

struct {
    uint32    offset;
    int8     *name;
} 

mgmt_mib_registers [] = {
    { 0x00, "TX octets" },
    { 0x08, "TX drop packets" },
    { 0x0c, "TX Q0 packets" },
    { 0x10, "TX broadcast packets" },
    { 0x14, "TX multicast packets" },
    { 0x18, "TX unicast packets" },
    { 0x1c, "TX collisions" },
    { 0x20, "TX single collisions" },
    { 0x24, "TX multiple collisions" },
    { 0x28, "TX deferred transmit counter" },
    { 0x2c, "TX late collisions" },
    { 0x30, "TX excessive collisions" },
    { 0x34, "TX Frame in disc counter" },
    { 0x38, "TX pause packets" },
    { 0x3c, "TX Q1 packets" },
    { 0x40, "TX Q2 packets" },
    { 0x44, "TX Q3 packets" },
    { 0x48, "TX Q4 packets" },
    { 0x4c, "TX Q5 packets" },
    { 0x50, "RX octets" },
    { 0x58, "RX undersize packet octets" },
    { 0x5c, "RX pause packets" },
    { 0x60, "RX 64 bytes octets" },
    { 0x64, "RX 65-127 bytes octets" },
    { 0x68, "RX 128-255 bytes octets" },
    { 0x6c, "RX 256-511 bytes octets" },
    { 0x70, "RX 512-1023 bytes octets" },
    { 0x74, "RX 1024-maxbytes bytes octets" },
    { 0x78, "RX oversize packets" },
    { 0x7c, "RX jabber packets" },
    { 0x80, "RX alignment error counter" },
    { 0x84, "RX FCS error counter" },
    { 0x8c, "RX good packet octet counter" },
    { 0x90, "RX drop packet counter" },
    { 0x94, "RX unicast packets" },
    { 0x98, "RX multicast packets" },
    { 0x9c, "RX broadcast packets" },
    { 0xa0, "RX SA change counter" },
    { 0xa4, "RX fragment counter" },
    { 0xa8, "RX Jumbo packet counter" },
    { 0xac, "RX symbol error counter" },
    { 0xb0, "InRangeErrCount counter" },
    { 0xb4, "OutRangeErrCount counter" },
    { 0xb8, "EEE low-power idle event registers" },
    { 0xbc, "EEE low-power idle duration registers" },
    { 0xc0, "RX discard counter" },
    { 0xc8, "Tx Q6 Packet Counter" },
    { 0xcc, "Tx Q7 Packet Counter" },
    { 0xd0, "Tx 64 Bytes Octets Counter" },
    { 0xd4, "Tx 65 to 127 Bytes Octets Counter" },
    { 0xd8, "Tx 128 to 255 Bytes Octets Counter" },
    { 0xdc, "Tx 256 to 511 Bytes Octets Counter" },
    { 0xe0, "Tx 512 to 1023 Bytes Octets Counter" },
    { 0xe4, "Tx 1024 to MaxPkt Bytes Octets Counter" },
};

typedef struct mgmt_port_mib_s {
    uint64    value;
} mgmt_switch_port_mib_t;

#define N_MGMT_MIB_REGISTERS   (sizeof(mgmt_mib_registers)/sizeof(mgmt_mib_registers[0]))

mgmt_info_t info_g;

/* List of commands supported along with the required parameters */
int8 mgmt_cmd_usage[] =
"\treboot\r\n"
"\tversion\r\n"
"\tstatus\r\n"
"\tmacaddr\r\n"
"\twrite <addr> <width 8|16|32|64> <data> [<data64_low>]\r\n"
"\tread <addr> <width 8|16|32|64>\r\n"
"\tspi_id <id 0|1|2|3>\r\n"
"\r\n";

int8 mgmt_vpd_cmd_usage[] =
"\tvpd show [key]\r\n"
"\tvpd set <key> <data>\r\n"
"\r\n";

int8 mgmt_switch_cmd_usage[] =
"\tswitch mirror enable <port bitmap> <probe port> <all|ingress|egress>\r\n"
"\tswitch mirror disable\r\n"
"\tswitch mirror status\r\n"
"\tswitch age_time set <age_time>\r\n"
"\tswitch age_time get\r\n"
"\tswitch dumbfwd get\r\n"
"\tswitch dumbfwd set <disable|enable>\r\n"
"\tswitch port mibs <port> <counter|all>\r\n"
"\r\n";

int8 mgmt_port_cmd_usage[] =
"\tport info <port>\r\n"
"\tport admin_mode set <port> <disable|enable>\r\n"
"\tport admin_mode get <port>\r\n"
"\tport speed set <port> <auto|100|1000> \r\n"
"\tport speed get <port>\r\n"
"\tport master_slave set <port> <slave|master>\r\n"
"\tport master_slave get <port>\r\n"
"\tport phy_lb_mode set <port> <disable|enable>\r\n"
"\tport phy_lb_mode get <port>\r\n"
"\tport jumbo_frame set <port> <disable|enable>\r\n"
"\tport jumbo_frame get <port>\r\n"
"\tport link sqi [port]\r\n"
"\r\n";

int8 mgmt_avnu_cmd_usage[] =
"\tavnu mode <disable|enable>\r\n"
"\tavnu stats <port>\r\n"
"\tavnu clearstats <port>\r\n"
"\tavnu numpdelaylostresp <port> <val>\r\n"
"\tavnu pdelay\r\n"
"\tavnu status\r\n"
"\tavnu device state\r\n"
"\tavnu portstatus [port]\r\n"
"\tavnu clock state <gm|slave>\r\n"
"\tavnu sync absence timeout <seconds>\r\n"
"\tavnu port role <port> <master|slave>\r\n"
"\tavnu ascapable <port> <true|false>\r\n"
"\tavnu pdelay <port> <pdelay>\r\n"
"\tavnu interval <port> <sync|pdelay> <initialLogInterval>\r\n"
"\tavnu interval pdelay_oper <port> <logVal>\r\n"
"\r\n";

int8 mgmt_vlan_cmd_usage[] =
"\tvlan create <vlan id>\r\n"
"\tvlan delete <vlan id>\r\n"
"\tvlan get <vlan id>\r\n"
"\tvlan portadd <vlan id> <port> <tag|untag>\r\n"
"\tvlan portdel <vlan id> <port>\r\n"
"\tvlan pvidset <port> <vlan id>\r\n"
"\tvlan ifilterset <disable|enable>\r\n"
"\tvlan prioset <port> <priority>\r\n"
"\tvlan port show\r\n"
"\r\n";

int8 mgmt_msrp_cmd_usage[] =
"\tmsrp entry add <xx:xx:xx:xx:xx:xx> <vlan id> <port|mcast_port_bitmap> "
"[<cir_kbps> <cbs_bytes> <ingress_port_bitmap>]\r\n"
"\tmsrp entry delete <xx:xx:xx:xx:xx:xx> <vlan id>\r\n"
"\tmsrp entry bandwidth get <xx:xx:xx:xx:xx:xx> <vlan id>\r\n"
"\tmsrp entry show [vlan_id [port]]\r\n"
"\tmsrp bandwidth set <port> <classA|classB> <max_rate_kbps> <max_burst_kbits>\r\n"
"\tmsrp bandwidth get <port> <classA|classB>\r\n"
"\tmsrp pcpmapping get <classA|classB>\r\n"
"\tmsrp pcpmapping set <prioA> <remap_prioA> <prioB> <remap_prioB>\r\n"
"\tmsrp boundary get <port>\r\n"
"\tmsrp boundary set <port> <classA|classB> <is_boundary>\r\n"
"\r\n";

int8 mgmt_acd_cmd_usage[] =
"\tacd <port> [m|cm] [bcm8953x|bcm8950x]\r\n"
"\r\n";

int8 mgmt_heartbeat_cmd_usage[] =
"\theartbeat <value_ms)>\r\n"
"\r\n";

int8 mgmt_config_cmd_usage[] =
"\tconfig <save|show|clear>\r\n"
"\r\n";

int8 mgmt_log_cmd_usage[] =
"\tlog show\r\n"
"\r\n";


/*@api
 * mgmt_avb_global_async_events_display
 *
 * @brief
 * Show defined global occured async events.
 *
 * @param= event_mask - global async events mask
 *
 * @returns None
 *
 * @desc
 */

void mgmt_avb_global_async_events_display(uint32 event_mask)
{
    if(event_mask & AVB_ASYNC_EVENT_GLOBAL_ETH_READY_MASK){
        printk("Event : Eth Ready\r\n");
    }
    if(event_mask & AVB_ASYNC_EVENT_GLOBAL_AVB_SYNC_MASK){
        printk("Event : AVB Sync\r\n");
    }
    if(event_mask & AVB_ASYNC_EVENT_GLOBAL_SYNC_RECEIPT_TIMEOUT_MASK){
        printk("Event : AVB Sync Receipt timeout\r\n");
    }
    if(event_mask & AVB_ASYNC_EVENT_GLOBAL_STREAM_BLOCKED_MASK){
        printk("Event : Stream Blocked \r\n");
    }
}

/*@api
 * mgmt_avb_port_async_events_display
 *
 * @brief
 * Show defined per port occured async events.
 *
 * @param= port - port number
 * @param= event_mask - per port async events mask
 *
 * @returns None
 *
 * @desc
 */

void mgmt_avb_port_async_events_display(uint32 port, uint32 event_mask)
{
    if(event_mask & AVB_ASYNC_EVENT_PORT_LINK_DOWN_MASK){
        printk("Event : Port Link Down on Port %d\r\n", port);
    }
    if(event_mask & AVB_ASYNC_EVENT_PORT_LINK_UP_MASK){
        printk("Event : Port Link Up on Port %d\r\n", port);
    }
    if(event_mask & AVB_ASYNC_EVENT_PORT_PDELAY_LOST_RESPONSE_EXCEEDED_MASK){
        printk("Event : PDelay Lost Response Exceeded on Port %d\r\n", port);
    }
    if(event_mask & AVB_ASYNC_EVENT_PORT_PDELAY_LOST_RESPONSE_RECOVERED_MASK){
        printk("Event : PDelay Lost Response recovered on Port %d\r\n", port);
    }
    if(event_mask & AVB_ASYNC_EVENT_PORT_PDELAY_CONFIG_CHANGE_MASK){
        printk("Event : PDelay config change on Port %d\r\n", port);
    }
    if(event_mask & AVB_ASYNC_EVENT_PORT_STREAM_BLOCKED_MASK){
        printk("Event : Stream blocked on Port %d\r\n", port);
    }
}



/*@api
 * mgmt_avb_async_events_show
 *
 * @brief
 * get occured async events.
 *
 * @param= info - pointer to mgmt_info_t
 *
 * @returns None
 *
 * @desc
 */

void mgmt_avb_async_events_show(mgmt_info_t *info)
{
    mgmt_avb_async_events_get_t events;

    sal_memset(&events, 0x00, sizeof(mgmt_avb_async_events_get_t));
    mgmt_avb_async_events_get(info, &events);
    printk("\r\n");
    if(events.global_events != 0){
        mgmt_avb_global_async_events_display(events.global_events);
    }

    if(events.port0_events != 0){
        mgmt_avb_port_async_events_display(MGMT_AVB_INTF_P0, events.port0_events);
    }

    if(events.port1_events != 0){
        mgmt_avb_port_async_events_display(MGMT_AVB_INTF_P1, events.port1_events);
    }

    if(events.port2_events != 0){
        mgmt_avb_port_async_events_display(MGMT_AVB_INTF_P2, events.port2_events);
    }

    if(events.port3_events != 0){
        mgmt_avb_port_async_events_display(MGMT_AVB_INTF_P3, events.port3_events);
    }

    if(events.port4_events != 0){
        mgmt_avb_port_async_events_display(MGMT_AVB_INTF_P4, events.port4_events);
    }

    if(events.port5_events != 0){
        mgmt_avb_port_async_events_display(MGMT_AVB_INTF_P5, events.port5_events);
    }

    if(events.port6_events != 0){
        mgmt_avb_port_async_events_display(MGMT_AVB_INTF_P6, events.port6_events);
    }

    if(events.port8_events != 0){
        mgmt_avb_port_async_events_display(MGMT_AVB_INTF_P8, events.port8_events);
    }
}

void mgmt_switch_port_mibs_sample(mgmt_info_t *info, int32 port, mgmt_switch_port_mib_t *mibs)
{
    int32 i;
    uint32 page_address;
    uint64 reg_value;

    for (i = 0; i < N_MGMT_MIB_REGISTERS; ++i) {
        reg_value = 0;
        page_address = ((0xb0020 + port) << 8) + mgmt_mib_registers[i].offset;
        mgmt_read_switch_register(info, page_address, 8, &reg_value);
        mibs[i].value = reg_value;
    }
}

void mgmt_switch_port_mibs_print(int32 port, int32 zeros, mgmt_switch_port_mib_t *mibs)
{
    int32 i;
    uint64 value;

    printk("port %d:\r\n", port);
    for (i = 0; i < N_MGMT_MIB_REGISTERS; ++i) {
        value = mibs[i].value;
        if (value || zeros) {
            printk("%s = 0x%08x.%08x\r\n", mgmt_mib_registers[i].name,
                    (uint32)(value >> 32), (uint32)value);
        }
    }
}

void mgmt_port_mibs(mgmt_info_t *info, int32 port, int32 zeros, uint32 index)
{
    uint64 reg_value = 0;
    uint32 page_address;
    int32 errno;
    mgmt_switch_port_mib_t sample0[N_MGMT_MIB_REGISTERS];
    if (info == NULL) {
        printk("%s :: mgmt_info_t pointer is null\r\n", __FUNCTION__);
        return;
    }

    if (zeros != 1) {
        printk("%s :: zeros variable value shoud be 1\r\n", __FUNCTION__);
        return;
    }

    if ((!MGMT_AVB_PHYS_INTF_IS_VALID(port)) && (port != 7)) {
        printk("Invalid physical port %d\r\n", port);
        return;
    }

    if(index < N_MGMT_MIB_REGISTERS){
        page_address = ((0xb0020 + port) << 8) + mgmt_mib_registers[index].offset;
        errno = mgmt_read_switch_register(info, page_address, 8, &reg_value);
        if (errno) {
            printk("Accessing switch reg failed, errno = %d\r\n", errno);
            return;
        }
        if (reg_value || zeros) {
            printk("%s = 0x%08x.%08x\r\n", mgmt_mib_registers[index].name,
                    (uint32)(reg_value >> 32), (uint32)reg_value);
        }
    } else if (index == N_MGMT_MIB_REGISTERS) {
        mgmt_switch_port_mibs_sample(info, port, sample0);
        mgmt_switch_port_mibs_print(port, zeros, sample0);
    } else {
        printk("%s :: Invalid register offset\r\n", __FUNCTION__);
        return;
    }
}

/*@api
 * parse_integer
 *
 * @brief
 * parse the integer.
 *
 * @param=s - mask
 * @returns parsed value
 *
 * @desc
 */
int32 parse_integer(const int8 *s, int32 *status)
{
    uint32  n, neg, base = 10;
    int8   *ch;

    *status = MGMT_AVB_SUCCESS;
    s += (neg = (*s == '-'));
    if ((*s == '0') && ((*(s+1) == 'x') || (*(s+1) == 'X'))) {
        if (neg) {
            printk("Negative hex numbers are not supported\r\n");
            *status = MGMT_AVB_FAILURE;
            return -1;
        }
        else {
            base = 16;
            s = (s + 2);
        }
    }

    ch = (int8 *) s;
    if (*ch == '\0') {
        printk("Invalid user input\r\n");
        *status = MGMT_AVB_FAILURE;
        return -1;
    }

    if (base == 10) {
        while (*ch != '\0') {
            if (*ch < '0' || *ch > '9') {
                printk("Invalid decimal number \"%s\"\r\n", s);
                *status = MGMT_AVB_FAILURE;
                return -1;
            }
            ch++;
        }
    }
    else if (base == 16) {
        while (*ch !='\0') {
            if ((*ch >= '0' && *ch <= '9') || (*ch >= 'A' && *ch <= 'F') || (*ch >= 'a' && *ch <= 'f')){
                ch++;
                continue;
            }
            else {
                printk("Invalid hex number: \"0x%s\"\r\n", s);
                *status = MGMT_AVB_FAILURE;
                return -1;
            }
        }
    }

    for (n = 0; ((*s >= 'a' && *s <= 'f' && base > 10) ||
                (*s >= 'A' && *s <= 'F' && base > 10) ||
                (*s >= '0' && *s <= '9')); s++) {
        n = n * base +
            ((*s >= 'a' && *s <= 'f') ? *s - 'a' + 10 :
             (*s >= 'A' && *s <= 'F') ? *s - 'A' + 10 :
             *s - '0');
    }

    return (neg ? -n : n);
}

/*@api
 * mgmt_port_next_get
 *
 * @param=isFirst - Get first (MGMT_TRUE) or get next (MGMT_FALSE)
 * @param=bitmap  - bitmap representation of ports
 * @param=port    - port number
 *
 * @brief
 * Get first/next member port for the supplied bitmap
 *
 * @returnval AVB_RC_t
 */
MGMT_AVB_RC_t mgmt_avb_port_next_get(MGMT_AVB_BOOL_t isFirst, uint32 bitmap, uint32 *port)
{
    MGMT_AVB_RC_t rc = MGMT_AVB_FAILURE;
    uint32 next_port = 0;

    if (isFirst != MGMT_AVB_TRUE)
    {
        next_port = *port + 1;
    }

    while (next_port < MGMT_AVB_MAX_INTERFACES)
    {
        if (bitmap & (1 << next_port))
        {
            *port = next_port;
            rc = MGMT_AVB_SUCCESS;
            break;
        }
        next_port++;
    }
    return rc;
}

/*@api
 * mgmt_intf_internal_to_external
 *
 * @brief
 * Convert an internal AVB port number to an external port number
 *
 * @param=intf    - internal avb port number (0-8)
 * @param=extIntf - external port number (0-5, 8, 100, 101)
 *
 * @returnval=0 - OK
 * @returnval!=0 - failed
 */
MGMT_AVB_RC_t mgmt_avb_intf_internal_to_external(uint32 intf, uint32 *extIntf)
{
    MGMT_AVB_RC_t rc      = MGMT_AVB_SUCCESS;

    if (intf < MGMT_AVB_MAX_INTERFACES_PHYS)
    {
        *extIntf = intf;
    }
    else if (intf == MGMT_AVB_INTF_P8_INTERNAL)
    {
        *extIntf = MGMT_AVB_INTF_P8;
    }
    else
    {
        rc = MGMT_AVB_FAILURE; /* Invalid Interface */
    }
    return rc;
}

/*@api
 * avb_internal_mask_to_external_port_print
 *
 * @brief
 * Print external port numbers from an AVB internal portmask
 * eg output: "0 1 2 3 4 5 8 100 101 "
 *
 * @param=portmask - internal avb portmask
 *
 * @returnval=none
 */
void mgmt_avb_internal_mask_to_external_port_print(uint32 portmask)
{
    MGMT_AVB_RC_t  rc = MGMT_AVB_FAILURE;
    uint32  port;
    uint32  extIntf;

    rc = mgmt_avb_port_next_get(MGMT_AVB_TRUE, portmask, &port);
    while (rc == MGMT_AVB_SUCCESS)
    {
        if (mgmt_avb_intf_internal_to_external(port, &extIntf) == MGMT_AVB_SUCCESS)
        {
            printk("%d ", extIntf);
        }
        else
        {
            printk("%d* ", port);
        }
        rc = mgmt_avb_port_next_get(MGMT_AVB_FALSE, portmask, &port);
    }
}

/*@api
 *Display the MAC entries from the hardware ARL table
 */

static void mgmt_avb_l2_entry_show(uint64 regval, uint64 regval64)
{
    uint8  is_static;
    uint32 vid, index;
    uint8  mac[MGMT_ETHER_ADDR_LEN];

    vid = (uint32) (regval64 >> 48) & 0xFFF;
    mac[5] = (uint8)(regval64 & 0xFF);
    mac[4] = (uint8)((regval64 >> 8) & 0xFF);
    mac[3] = (uint8)((regval64 >> 16) & 0xFF);
    mac[2] = (uint8)((regval64 >> 24) & 0xFF);
    mac[1] = (uint8)((regval64 >> 32) & 0xFF);
    mac[0] = (uint8)((regval64 >> 40) & 0xFF);
    is_static = (regval & 0x8000) ? 1 : 0;
    printk("%02x:%02x:%02x:%02x:%02x:%02x\t%d\t%d",
            mac[0], mac[1], mac[2], mac[3], mac[4], mac[5], vid, is_static);

    if (mac[0] & 0x1)
    {
        /* Bitmap for multicast */
        printk("\t");
        for (index=0; index < MGMT_AVB_MAX_INTERFACES; index++)
        {
            if (regval & (1 << index))
            {
                printk("P%d ", index);
            }
        }
        printk("\r\n");
    }
    else
    {
        /* Port for unicast */
        printk("\tP%lld\r\n", regval & 0xF);
    }
}

/*
 * @api
 * mgmt_fdb_entry_get
 *
 * @brief
 * get  forwarding dabatase entry based on the given VLAN and port.
 *
 * @param=info - management info
 * @param=port - port number
 * @param=vlan - vlan_id
 *
 * @returns 0 on success, !0 on error
 */
int32 mgmt_fdb_entry_get(mgmt_info_t *info, uint32 vlan_id, uint32 port)
{
    uint32 index = 0;
    uint32 count = 0;
    uint8  entry_valid = 0, search_valid = 0;
    uint64 regval;
    uint64 regval64_N0;
    uint64 regval64_N1;

    if (info == NULL) {
        printk("%s :: mgmt_info_t pointer is null\r\n", __FUNCTION__);
        return -1;
    }

    if ((!MGMT_AVB_VLAN_VALID(vlan_id)) && (vlan_id != MGMT_AVB_ARL_MATCH_ALL_VLANS)){
        printk("Invalid VLAN ID %d specified\r\n", vlan_id);
        return -1;
    }

    if ((IS_EXTERNAL_INTF_INVALID(port)) && (port != MGMT_AVB_ARL_MATCH_ALL_PORTS)){
        printk("Invalid port number %d\r\n", port);
        return -1;
    }

    if ((vlan_id == MGMT_AVB_ARL_MATCH_ALL_VLANS) || (port == MGMT_AVB_ARL_MATCH_ALL_PORTS)){
        if (vlan_id == MGMT_AVB_ARL_MATCH_ALL_VLANS)
            printk("Stream entries for all VLANS will be displayed\r\n");
        if (port == MGMT_AVB_ARL_MATCH_ALL_PORTS)
            printk("Stream entries for all PORTS will be displayed\r\n");
    }

    /* this search function relies on ARL table search control register */
    /* Set the start bit in the ARL table search control register */
    mgmt_read_switch_register(info, SWITCH_PAGE_05_ARLA_SRCH_CTL, sizeof(uint32), &regval);
    regval |= (1 << 7);
    mgmt_write_switch_register(info, SWITCH_PAGE_05_ARLA_SRCH_CTL, sizeof(uint32), regval);

    /* Wait for operation to complete */
    for (index=0; index < MGMT_BCM895XX_TIMEOUT_VAL; index++)
    {
        mgmt_read_switch_register(info, SWITCH_PAGE_05_ARLA_SRCH_CTL, sizeof(uint32), &regval);
        if ((regval & 0x80) == 0)
        {
            break; /* If search is done, break */
        }

        if ((regval & 0x1) == 0x00)
        {
            continue; /* If search is not valid, continue */
        }
        else
        {
            search_valid = 1;
            break; /* Valid entry found */
        }
    }

    if ((index == MGMT_BCM895XX_TIMEOUT_VAL) || (search_valid == 0))
    {
        /* Done with the search */
        return 0;
    }

    do
    {
        /* Read ARL search result 0 register */
        entry_valid = 0;
        for (index=0; index < MGMT_BCM895XX_TIMEOUT_VAL; index++)
        {
            mgmt_read_switch_register(info, SWITCH_PAGE_05_ARLA_SRCH_RSLT_0, sizeof(uint32), &regval);
            if (regval & 0x10000)  /* Entry is valid */
            {
                entry_valid = 1;
                break;
            }
        }

        /* Read the VID, MAC details for Result 0 and Result 1 unconditionally.
           They'll only get used if the corresponding search result is valid   */
        mgmt_read_switch_register(info, SWITCH_PAGE_05_ARLA_SRCH_RSLT_0_MACVID, sizeof(uint64), &regval64_N0);
        mgmt_read_switch_register(info, SWITCH_PAGE_05_ARLA_SRCH_RSLT_1_MACVID, sizeof(uint64), &regval64_N1);

        if (entry_valid)
        {
            if (((port == MGMT_AVB_ARL_MATCH_ALL_PORTS) && (vlan_id == ((uint32) (regval64_N0 >> 48) & 0xFFF))) ||  /* Show all vlan matching entries */
                    ((port == (regval & 0xF))   && (vlan_id == ((uint32) (regval64_N0 >> 48) & 0xFFF))) ||  /* Show all vlan and port matching entries */
                    ((port == (regval & 0xF))   && (vlan_id == MGMT_AVB_ARL_MATCH_ALL_VLANS)) ||                                      /* Show all port matching entries */
                    ((port == MGMT_AVB_ARL_MATCH_ALL_PORTS) && (vlan_id == MGMT_AVB_ARL_MATCH_ALL_VLANS)))                                        /* Show everything */
            {
                count++;
                if (count == 1)
                {
                    printf("\nMac Address\t\tVlan\tStatic\tPort(s)\r\n");
                }
                mgmt_avb_l2_entry_show(regval, regval64_N0);
            }
        }
        else
        {
            entry_valid = 0;
        }

        /* Read ARL search result 1 register */
        regval = 0;
        mgmt_read_switch_register(info, SWITCH_PAGE_05_ARLA_SRCH_RSLT_1, sizeof(uint32), &regval);
        if (regval & 0x10000)  /* Entry is valid */
        {
            entry_valid = 1;
            if (((port == MGMT_AVB_ARL_MATCH_ALL_PORTS) && (vlan_id == ((uint32) (regval64_N1 >> 48) & 0xFFF))) ||  /* Show all vlan matching entries */
                    ((port == (regval & 0xF))   && (vlan_id == ((uint32) (regval64_N1 >> 48) & 0xFFF))) ||  /* Show all vlan and port matching entries */
                    ((port == (regval & 0xF))   && (vlan_id == MGMT_AVB_ARL_MATCH_ALL_VLANS)) ||                                      /* Show all port matching entries */
                    ((port == MGMT_AVB_ARL_MATCH_ALL_PORTS) && (vlan_id == MGMT_AVB_ARL_MATCH_ALL_VLANS)))                                        /* Show everything */
            {
                count++;
                mgmt_avb_l2_entry_show(regval, regval64_N1);
            }
        }
    } while (entry_valid);

    printf("\nTotal number of MAC entries %d\r\n", count);
    return 0;
}

/*@api
 * show_mgmt_vlan_port_info
 *
 * @brief
 * Displays the vlan information per port.
 *
 * @param = info - management info
 * @returns 0
 *
 * @desc
 */

void show_mgmt_vlan_port_info(mgmt_info_t *info)
{
    uint32 extIntf = 0;
    uint32 port;
    mgmt_vlan_port_info_t vlan_port_info;

    if (info == NULL) {
        printk("%s :: mgmt_info_t pointer is null\r\n", __FUNCTION__);
        return;
    }

    printk("\nPort  iFilter  Pvid  Prio \r\n");
    for(port = 0; port < MGMT_BCM895XX_MAX_NUM_PORTS; port++) {
        sal_memset(&vlan_port_info, 0x00, sizeof(mgmt_vlan_port_info_t));

        if (mgmt_avb_intf_internal_to_external(port, &extIntf) == MGMT_AVB_SUCCESS){
            printk("%u", extIntf);
        }else{
            printk("%u*", port);
        }
        port = (port == MGMT_AVB_INTF_P8_INTERNAL) ? MGMT_AVB_INTF_P8 : port;
        mgmt_vlan_port_info_get(info, port, &vlan_port_info);
        printk("%6u%9u%6u", vlan_port_info.iFilter, vlan_port_info.pvid, vlan_port_info.priority);
        printk("\r\n");
    }
}

/*@api
 * show_mgmt_port_info
 *
 * @brief
 * Displays the port information.
 *
 * @param = info - management info
 * @param = port - port number to get
 * @returns 0
 *
 * @desc
 */
int32 show_mgmt_port_info(mgmt_info_t *info, uint32 port)
{
    int32 errno;
    mgmt_port_info_t port_info;
    sal_memset(&port_info, 0x00, sizeof(mgmt_port_info_t));

    errno  = mgmt_port_info_get(info, port, &port_info);
    if(errno) {
        return errno;
    }

    printk("Admin_mode               : %s\r\n", (port_info.admin_mode == ENABLED)?"enabled":"disabled");
    printk("Link status              : %s\r\n", (port_info.link_status == LINK_UP)?"up":"down");
    if(  port_info.speed == 0){
        printk("Speed                    : Auto\r\n");
    }else{
        printk("Speed                    : %d Mbps\r\n", port_info.speed);
    }
    printk("BR_mode                  : ");
    if( port_info.br_mode == SLAVE){
        printk("slave");
    }else if( port_info.br_mode ==  MASTER){
        printk("master");
    }else if( port_info.br_mode ==  AUTO){
        printk("auto");
    }else if( port_info.br_mode ==  NONE){
        printk("none");
    }
    printk("\r\n");
    printk("Phy Loop Back mode       : %s\r\n", (port_info.loop_back_mode == ENABLED) ? "enabled":"disabled");
    printk("Jumbo Frame_mode         : %s\r\n", (port_info.jumbo_frame_mode == ENABLED)? "enabled":"disabled");
    printk("Auto Negotiation         : %s\r\n", (port_info.autoneg == ENABLED)? "enabled":"disabled");
    printk("Auto Negotiation result  : %s\r\n", (port_info.autoneg_complete == AUTO_NEG_COMPLETE)? "complete":"pending");
    printk("Duplex Status            : %s\r\n", (port_info.duplex == HALF_DUPLEX) ? "half":"full");
    printk("LED status               : %s\r\n", port_info.led ? "ON": "OFF");
    printk("Link state change count  : %u\r\n", port_info.link_state_change_cnt);

    return errno;
}
/* @api
 * get_port_role
 *
 * @brief
 * return the port role.
 *
 * @param = info - management info
 * @param = port - port number to get port role
 *
 */
uint32 get_mgmt_port_role(mgmt_info_t *info, uint32 port)
{
    mgmt_port_info_t port_info;
    mgmt_port_info_get(info, port, &port_info);
    return port_info.br_mode;
}

/*@api
 * show_mgmt_port_stats
 *
 * @brief
 * Displays the port statistics.
 *
 * @param = info - management info
 * @param = port - port number to get the statistics
 * @returns 0
 *
 * @desc
 */
int32 show_mgmt_port_stats(mgmt_info_t *info, uint32 port)
{
    /* Read the page registers directly for stats */
    return CMD_NFND;
}

/*@api
 * show_mgmt_avnu_port_stats
 *
 * @brief
 * Displays the port dot1as stats.
 *
 * @param = info - management info
 * @param = port - port number to get the dot1as stats
 * @returns 0
 *
 * @desc
 */
int32 show_mgmt_avnu_port_stats(mgmt_info_t *info, uint32 port)
{
    mgmt_avnu_port_stats_t avnu_port_stats;
    int32 rv;
    sal_memset(&avnu_port_stats, 0x00, sizeof(mgmt_avnu_port_stats_t));

    rv = mgmt_avnu_port_stats_get(info, port, &avnu_port_stats);
    if (rv == 0){
        printk("\r\n");
        printk("Sync Rx                   : %d\r\n",avnu_port_stats.sync_rx);
        printk("Sync Tx                   : %d\r\n",avnu_port_stats.sync_tx);
        printk("Sync Tx Resends           : %d\r\n",avnu_port_stats.sync_tx_timeouts);
        printk("Followup Rx               : %d\r\n",avnu_port_stats.followup_rx);
        printk("Followup Tx               : %d\r\n",avnu_port_stats.followup_tx);
        printk("Sync Rx Timeouts          : %d\r\n",avnu_port_stats.sync_rx_timeouts);
        printk("FollowUp Rx Timeouts      : %d\r\n",avnu_port_stats.followup_rx_timeouts);
        printk("Sync Discards             : %d\r\n",avnu_port_stats.sync_discards);
        printk("FollowUp Rx Discards      : %d\r\n",avnu_port_stats.followup_rx_discards);

        printk("\r\n");
        printk("Pdelay Req Rx             : %d\r\n",avnu_port_stats.pdelay_req_rx);
        printk("Pdelay Req Tx             : %d\r\n",avnu_port_stats.pdelay_req_tx);
        printk("Pdelay Resp Rx            : %d\r\n",avnu_port_stats.pdelay_resp_rx);
        printk("Pdelay Resp Tx            : %d\r\n",avnu_port_stats.pdelay_resp_tx);
        printk("Pdelay Resp FollowUp Rx   : %d\r\n",avnu_port_stats.pdelay_followup_rx);
        printk("Pdelay Resp FollowUp Tx   : %d\r\n",avnu_port_stats.pdelay_followup_tx);
        printk("Pdelay Req Rx Discards    : %d\r\n",avnu_port_stats.pdelay_req_rx_discards);
        printk("Pdelay Resp Rx Discards   : %d\r\n",avnu_port_stats.pdelay_resp_rx_discards);
        printk("Pdelay Rx Timeouts        : %d\r\n",avnu_port_stats.pdelay_rx_timeouts);
        printk("Bad Pdelay Values         : %d\r\n",avnu_port_stats.bad_pdelay_values);
        printk("Pdelay Lost Resp Exceeded : %d\r\n",avnu_port_stats.pdelay_lost_resp_exceeded);
        printk("Pdelay Rx Discards        : %d\r\n",avnu_port_stats.pdelay_discards);

        printk("\r\n");
        printk("Signaling Rx              : %d\r\n",avnu_port_stats.signaling_rx_count);
        printk("Signaling Tx              : %d\r\n",avnu_port_stats.signaling_tx_count);
        printk("Signaling Discards        : %d\r\n",avnu_port_stats.signaling_rx_discards);

        printk("\r\n");
        printk("Sync Too Fast             : %d\r\n",avnu_port_stats.sync_too_fast);
        printk("Tx Errors                 : %d\r\n",avnu_port_stats.tx_errors);
        printk("Timestamp Errors          : %d\r\n",avnu_port_stats.ts_errors);
        printk("Bad headers               : %d\r\n",avnu_port_stats.bad_headers);
        printk("PTP Discards              : %d\r\n",avnu_port_stats.ptp_discards);
    }
    else{
        return rv;
    }
    return 0;
}

int32 show_mgmt_sqi_result(mgmt_info_t *info, uint32 port, uint32 *port_status)
{
   mgmt_port_info_t port_info;
   uint32 sqi = 0;
   int32 errno;

   sal_memset(&port_info, 0x00, sizeof(mgmt_port_info_t));
   mgmt_port_info_get(info, port, &port_info);
   *port_status = port_info.link_status;
   if(*port_status  == LINK_DOWN){
     return sqi;
   }

   errno = mgmt_port_link_sqi_get(info, port, &sqi);
   if (errno) {
      printk("Error: Could not able to get SQI value, errn = %d\r\n", errno);
      sqi = 0;
   }
   return sqi;
}

/* Function to convert BMCA port role to string */
uint8 *mgmtDot1asDebugRoleStringGet(MGMT_DOT1AS_PORT_ROLE_t role)
{
    uint8 *str;
    switch (role)
    {
        case MGMT_DOT1AS_ROLE_MASTER:
            str = (uint8 *)"Master";
            break;
        case MGMT_DOT1AS_ROLE_SLAVE:
            str = (uint8 *)"Slave";
            break;
        default:
            str = (uint8 *)"";
            break;
    }

    return str;
}

/* Function to convert AVnu AS Capable Status to string */
void mgmtPortAVnuASStatusString(uint32 as_capable_status, char *str)
{
    switch (as_capable_status)
    {
        case MGMT_AVNU_ASCAPABLE_STATUS_PORT_DOWN:
            sal_sprintf(str, "%s(%s)", "False", "Link Down");
            break;
        case MGMT_AVNU_ASCAPABLE_STATUS_FALSE:
            sal_sprintf(str, "%s(%s)", "False", "AVnu Config");
            break;
        case MGMT_AVNU_ASCAPABLE_STATUS_TRUE:
            sal_sprintf(str, "%s(%s)", "True", "AVnu Config");
            break;
        default:
            sal_sprintf(str,"%s", "");
            break;
    }
}

/* Function to convert AVnu dot1as Status to string */
void mgmt_avnu_dot1as_status_string(uint32 dot1as_status, char *str)
{
    switch(dot1as_status){
        case MGMT_DOT1AS_AVNU_DISABLED:
            sal_sprintf(str, "%s", "802.1as    : AVnu (802.1as Disabled)\r\n");
            break;
        case MGMT_DOT1AS_AVNU_AUTOMOTIVE:
            sal_sprintf(str, "%s", "802.1as    : AVnu Automotive Profile\r\n");
            break;
        default:
            break;

    }
}

/* Display AVnu global status */

void show_avnu_status(mgmt_info_t *info)
{
    mgmt_avnu_global_status_t status;
    int32 errno;
    char str[40];
    uint8_t *id_upper;
    uint8_t *id_lower;

    sal_memset(&status, 0x00, sizeof(mgmt_avnu_global_status_t));
    errno = mgmt_avnu_global_status(info, &status);
    if(errno){
        printk("Error: Could not able to get AVnu Global Status\r\n");
        return;
    }

    sal_memset(str, 0x00, sizeof(str));
    mgmt_avnu_dot1as_status_string(status.dot1as_status, str);
    printk("%s\r\n", str);

    id_upper = (uint8 *)&status.local_clk_id_upper;
    id_lower = (uint8 *)&status.local_clk_id_lower;
    printk("Local clock id    : %02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x\r\n",
            id_lower[3], id_lower[2], id_lower[1], id_lower[0],id_upper[3], id_upper[2], id_upper[1], id_upper[0]);

    if (status.clock_type == MGMT_AVNU_CLOCK_STATE_SLAVE){
        switch (status.gm_status){
            case MGMT_AVNU_GM_STARTUP_ABSENT:
                printk("GM         : Absent At Startup\r\n");
                break;
            case MGMT_AVNU_GM_OPERATIONAL_ABSENT:
                printk("GM         : Absent\r\n");
                break;
            case MGMT_AVNU_GM_UNDETERMINED:
                printk("GM         : Undetermined\r\n");
                break;
            case MGMT_AVNU_GM_DISABLED:
                printk("GM         : Disabled\r\n");
                break;
            case MGMT_AVNU_GM_NO_SLAVE_PORT:
                printk("GM         : No slave port\r\n");
                break;
            case MGMT_AVNU_GM_PRESENT:
                printk("GM         : Present\r\n");
                break;
            default:
                break;
        }

        if (status.slave_port != MGMT_BCM895XX_MAX_NUM_PORTS){
            printk("Slave Port : %d\r\n", status.slave_port);
        }
        printk("RateRatioIn :0x%x\r\n", status.rateRatioIn);
        printk("RateRatioOut:0x%x\r\n", status.rateRatioOut);
        printk("IsSyncRcvd  :%u\r\n", status.isSyncReceived);
        printk("BridgeLocalTime:%llu.%u",
            ((uint64_t)status.bridgeLocalTime.sec_u << 32)
            | (status.bridgeLocalTime.sec_l), status.bridgeLocalTime.ns);
    }
}

void show_avnu_port_status(mgmt_info_t *info, uint32 intIfNum)
{
    char str[40];
    int32 errno;
    mgmt_avnu_port_status_t avnu_port_status;
    mgmt_avnu_global_status_t avnu_gm_status;
	int i;

    sal_memset(&avnu_gm_status, 0x00, sizeof(mgmt_avnu_global_status_t));
    errno = mgmt_avnu_global_status(info, &avnu_gm_status);
    if(errno){
        printk("Error: Could not able to get AVnu Global Status\r\n");
        return;
    }

    sal_memset(str, 0x00, sizeof(str));
    mgmt_avnu_dot1as_status_string(avnu_gm_status.dot1as_status, str);
    printk("\n%s\r\n", str);

	if (intIfNum != MGMT_BCM895XX_ALL_PORTS){
        if (intIfNum == MGMT_AVB_INTF_P8_INTERNAL){
            intIfNum = MGMT_AVB_INTF_P8;
        }
		sal_memset(&avnu_port_status, 0x00, sizeof(mgmt_avnu_port_status_t));
		errno = mgmt_avnu_port_status_get(info, intIfNum, &avnu_port_status);
		if(errno){
			printk("Error: Could not able to get avnu port status\r\n");
			return;
		}

		printk("Dot1as capable            : %s\r\n",avnu_port_status.dot1as_capable?"True":"False");
		printk("Peer delay                : %d\r\n",avnu_port_status.peer_delay);
		printk("Rate ratio                : 0x%8x\r\n",avnu_port_status.nbr_rate_ratio);
		printk("Is measuring delay        : %s\r\n",avnu_port_status.is_measuring_pdelay?"True":"False");
		printk("Allowed lost responses    : %d\r\n",avnu_port_status.allowed_lost_responses);
        printk("Port role                 : ");
		if( avnu_port_status.port_role == MGMT_DOT1AS_ROLE_SLAVE){
			printk("slave\r\n");
		}else if( avnu_port_status.port_role ==  MGMT_DOT1AS_ROLE_MASTER){
			printk("master\r\n");
		}else {
			printk("unknown\r\n");
		}
		printk("Sync interval             : %d\r\n",atoi(avnu_port_status.sync_interval));
		printk("Pdelay interval           : %d\r\n",atoi(avnu_port_status.pdelay_interval));
		printk("Sync reciept timeout      : %d\r\n",avnu_port_status.sync_receipt_timeout);
		printk("Number of lost responses  : %u\r\n", avnu_port_status.num_lost_responses);
		printk("Peer delay request state  : %s\r\n", avnu_port_status.pdelay_req_state);
		printk("Sync receive state        : %s\r\n", avnu_port_status.sync_receive_state);
		printk("%-20s : %s\r\n", "Port Role", mgmtDot1asDebugRoleStringGet(avnu_port_status.port_role));
		printk("%-20s : 0x%x\r\n", "Rate Ratio", avnu_port_status.nbr_rate_ratio);
		if (avnu_port_status.port_role == MGMT_DOT1AS_ROLE_MASTER){
			printk("%-20s : %dms\r\n", "SyncTx Interval", MGMT_DOT1AS_INTL_US_TO_MS(avnu_port_status.sync_tx_intl));
		}
		else if (avnu_port_status.port_role == MGMT_DOT1AS_ROLE_SLAVE){
			printk("%-20s : %dms\r\n", "SyncRx Interval", MGMT_DOT1AS_INTL_US_TO_MS(avnu_port_status.sync_rx_intl));
		}
		printk("pDelaySeqId : %u\r\n", avnu_port_status.pDelaySeqId);
		printk("isAVnuPdelayConfigSaved : %u\r\n", avnu_port_status.isAVnuPdelayConfigSaved);
		printk("PartnerId Clock: ");
		for (i=0; i<8; i++)
			printk("%02x:", avnu_port_status.partnerIdClock[i]);
        printk("\r\n");
		printk("partnerIdPort : %u\r\n", avnu_port_status.partnerIdPort);
		printk("rxIntfIndex : %u\r\n", avnu_port_status.syncRxInfo.rxIntfIndex);
		printk("lastSeqId : %u\r\n", avnu_port_status.syncRxInfo.lastSeqId);
		printk("followupCorrectionField:%llu\r\n", ((int64_t)avnu_port_status.syncRxInfo.followupCorrectionField_u << 32) | avnu_port_status.syncRxInfo.followupCorrectionField_l);
		printk("PDU interval : %d\r\n", avnu_port_status.syncRxInfo.pduInterval);
		printk("PreciseOriginTimeStamp: %llu.%u\r\n", ((uint64_t)avnu_port_status.syncRxInfo.preciseOriginTimestamp.sec_u << 32) | avnu_port_status.syncRxInfo.preciseOriginTimestamp.sec_l, avnu_port_status.syncRxInfo.preciseOriginTimestamp.ns);
		printk("syncRxTimestamp: %llu.%u\r\n", ((uint64_t)avnu_port_status.syncRxInfo.syncRxTimestamp.sec_u << 32) | avnu_port_status.syncRxInfo.syncRxTimestamp.sec_l, avnu_port_status.syncRxInfo.syncRxTimestamp.ns);
		printk("gmTimeBaseIndicator : %u\r\n", avnu_port_status.syncRxInfo.gmTimeBaseIndicator);
		printk("lastGmPhaseChange:");
		for (i=0; i<12; i++)
			printk("%0x", avnu_port_status.syncRxInfo.lastGmPhaseChange[i]);
        printk("\r\n");
		printk("LastGMFreqChange :%d\r\n", avnu_port_status.syncRxInfo.lastGmFreqChange);
		printk("syncInfoAvailable : %u\r\n", avnu_port_status.syncInfoAvailable);
		printk("syncTxSeqId : %u\r\n", avnu_port_status.syncTxSeqId);
		printk("signalingSeqId : %u\r\n", avnu_port_status.signalingSeqId);
		return;
	}

    MGMT_AVB_TRACE_LINE(114);
    printf("%-5s %8s %10s %9s %12s %12s %12s\r\n", "", "", "", "", "", "syncTx", "syncRx");
    printf("%-5s %8s %10s %9s %12s %12s %12s\r\n", "Intf", "PortRole", "asCapable", "PDelay", "RateRatio", "Interval", "Interval");
    MGMT_AVB_TRACE_LINE(114);
    for (intIfNum = 0; intIfNum < MGMT_BCM895XX_MAX_NUM_PORTS; intIfNum++)
    {
        sal_memset(&avnu_port_status, 0x00, sizeof(mgmt_avnu_port_status_t));
        if (intIfNum == MGMT_AVB_INTF_P8_INTERNAL){
            intIfNum = MGMT_AVB_INTF_P8;
        }
        printk("%-6d", intIfNum);
        errno = mgmt_avnu_port_status_get(info, intIfNum, &avnu_port_status);
        if(errno){
            printk("Error: Could not able to get avnu port status\r\n");
            return;
        }
        sal_memset(str, 0x00, sizeof(str));
        printk("%8s",mgmtDot1asDebugRoleStringGet(avnu_port_status.port_role));
        printk("%10s", avnu_port_status.dot1as_capable ? "True" : "False");
        if (!avnu_port_status.dot1as_capable){
            printk("\r\n");
            continue;
        }

        printk("%11d", avnu_port_status.peer_delay);
        sal_memset(str, 0x00, sizeof(str));
        sal_sprintf(str, "0x%x", avnu_port_status.nbr_rate_ratio);
        printk("%13s", avnu_port_status.dot1as_capable ? str : "");
        if (avnu_port_status.port_role == MGMT_DOT1AS_ROLE_MASTER)
        {
            printk("%11dms", MGMT_DOT1AS_INTL_US_TO_MS(avnu_port_status.sync_tx_intl));
        }

        if (avnu_port_status.port_role == MGMT_DOT1AS_ROLE_SLAVE)
        {
            printk("%13s", "");
            printk("%10s", "");
            printk("%11dms", MGMT_DOT1AS_INTL_US_TO_MS(avnu_port_status.sync_rx_intl));
        }
        printk("\r\n");
    }
    MGMT_AVB_TRACE_LINE(114);

}

#define TAR_HDR_MAGIC "ustar" /* ustar and a null */

int octal_string_to_int(uint8 *current_char, unsigned int size)
{
    unsigned int output = 0;
    while(size > 0){
        output = output * 8 + *current_char - '0';
        current_char++;
        size--;
    }
    return output;
}

static int32_t show_avnu_pdelay(mgmt_info_t *info)
{
    int32_t errno;
    uint32_t intIfNum;
    mgmt_avnu_pdelay_t avnu_pdelay;

    printk("%-6s%-12s%-12s%-12s%-12s%-12s%-12s%-14s\r\n",
            "Intf", "T1", "T2", "T3", "T4", "Pdelay",
            "asCapable", "nbrRatio");

    for (intIfNum = 0; intIfNum < MGMT_BCM895XX_MAX_NUM_PORTS; intIfNum++) {
        if (intIfNum == MGMT_AVB_INTF_P8_INTERNAL){
            intIfNum = MGMT_AVB_INTF_P8;
        }
        errno = mgmt_avnu_pdelay_get(info, intIfNum, &avnu_pdelay);
        if (errno) {
            printk("failed to get avnu delay for intf %u\r\n", intIfNum);
            break;
        }

        printk("%-6d%-12llu%-12llu%-12llu%-12llu%-12u%-12s0x%-12x\r\n",
                intIfNum,
                ((uint64_t)avnu_pdelay.pDelayT1.sec_u << 32) | avnu_pdelay.pDelayT1.sec_l,
                ((uint64_t)avnu_pdelay.pDelayT2.sec_u << 32) | avnu_pdelay.pDelayT2.sec_l,
                ((uint64_t)avnu_pdelay.pDelayT3.sec_u << 32) | avnu_pdelay.pDelayT3.sec_l,
                ((uint64_t)avnu_pdelay.pDelayT4.sec_u << 32) | avnu_pdelay.pDelayT4.sec_l,
                avnu_pdelay.nbrPDelay,
                avnu_pdelay.dot1asCapable,
                avnu_pdelay.nbrRR);

        printk("%-6s%-12u%-12u%-12u%-12u\n\r\n",
                "",
                avnu_pdelay.pDelayT1.ns,
                avnu_pdelay.pDelayT2.ns,
                avnu_pdelay.pDelayT3.ns,
                avnu_pdelay.pDelayT4.ns);

    }
    return errno;
}

/*@api
 * mgmt_cmd
 *
 * @brief
 * process the command.
 *
 * @param=args - pointer to args
 * @returns command result
 *
 * @desc
 */
uint64 spi_test_buf;

cmd_result_t mgmt_cmd(args_t *args)
{
    int32 i, errno, rv = CMD_OK;
    mgmt_info_t *info = &info_g;
    int8 *c = NULL;
    int8 *mode = NULL;
    int32 mode_val = 0;
    uint32 argCnt;
    int8 * mac_addr = NULL;
    uint8 mac_hex[MGMT_ETHER_ADDR_LEN];
    uint32 value1;
    uint32 value2;
    uint32 value3;
    uint32 value4;
    uint32 value5;
    char sw_version[VERSION_STR_LEN];
    int32 command_parse_status = MGMT_AVB_SUCCESS;
    uint32 sqi =0;
    mgmt_avnu_port_status_t avnu_port_status;
    int8 *vpd_data = NULL;
    ImgDesc imgs[MGMT_DOWNLOAD_MAX_IMGS];

    sal_memset(&avnu_port_status, 0x00, sizeof(mgmt_avnu_port_status_t));

    if (ARG_CNT(args) < 1) {
        return 0;
    }

    c = ARG_GET(args);                  /* Command or prefix */
    if (!sal_strcmp(c, "help")){
        if (ARG_CNT(args) == 1) {
            c = ARG_GET(args);
        }
        return CMD_USAGE;
    }

    /* Now parse and handle the commands */
    if (!sal_strcmp(c, "version")) {
        uint32 dev;
        uint16 rev;

        errno = mgmt_get_chip_info(&rev, &dev);
        if (errno){
            printk("could not get chip info, errno = %d\r\n", errno);
            goto done;
        }
        (void)printk("MCU: %x %x\r\n", dev, rev);

        errno = mgmt_os_version_get(info, sw_version);
        if (errno){
            printk("could not get OS version, errno = %d\r\n", errno);
            goto done;
        }
        printk("OS Version: %s\r\n", sw_version);

        errno = mgmt_avb_version_get(info, sw_version);
        if (errno){
            printk("could not get avb version, errno = %d\r\n", errno);
            goto done;
        }
        printk("AVB Version: %s\r\n", sw_version);

    } else if (!sal_strcmp(c, "status")) {
        uint16 rd_val;

        errno = mgmt_fw_status_get(info, &rd_val);
        if (errno) {
            printk("mgmt_fw_status_get failed, errno = %d\r\n", errno);
            rv = CMD_FAIL;
        } else {
            printk("Firmware status: 0x%04x\r\n", rd_val);
        }

        errno = mgmt_userdata_status_get(info, &rd_val);
        if (errno) {
            printk("mgmt_userdata_status_get failed, errno = %d\r\n", errno);
            rv = CMD_FAIL;
        } else {
            printk("AVB/VPD status: 0x%04x\r\n", rd_val);
        }
    }else if (!sal_strcmp(c, "macaddr")) {
        errno = mgmt_macaddr_get(info, mac_hex);
        if (errno){
            printk("could not get MAC Address of devcie, errno = %d\r\n", errno);
            goto done;
        }
        printk("\nMAC Address: %02x:%02x:%02x:%02x:%02x:%02x\r\n",
                        mac_hex[0],mac_hex[1],mac_hex[2],mac_hex[3],mac_hex[4],mac_hex[5]);
    }else if (!sal_strcmp(c, "reboot")) {
        if (ARG_CNT(args) != 0) {
            rv = CMD_USAGE_FLASH;
            goto done;
        }

        errno = mgmt_reboot(info, MGMT_REBOOT_CHIP);
        if (errno) {
            printk("could not reboot, errno = %d\r\n", errno);
            rv = CMD_FAIL;
        }
    } else if (!sal_strcmp(c, "spi_id")) {
        SPI_ID_t spi_id;
        if (ARG_CNT(args) != 1) {
            rv = CMD_USAGE_FLASH;
            goto done;
        }
        c = ARG_GET(args);
        spi_id = parse_integer(c, &command_parse_status);

        errno = mgmt_set_spi_id(info, spi_id);
        if (errno) {
            printk("could not set SPI ID, errno = %d\r\n", errno);
            rv = CMD_FAIL;
        }
    } else if (!sal_strcmp(c, "write")) {
        uint32 addr;
        uint32 mode;
        uint64 data;
        if (!((ARG_CNT(args) == 3) || (ARG_CNT(args) == 4))){
            rv = CMD_USAGE_FLASH;
            goto done;
        }
        c = ARG_GET(args);
        addr = parse_integer(c, &command_parse_status);
        c = ARG_GET(args);
        mode = parse_integer(c, &command_parse_status);
        c = ARG_GET(args);
        data = (uint64)parse_integer(c, &command_parse_status);
        if (ARG_CNT(args)!= 0) {
            c = ARG_GET(args);
            data = (data << 32) + ((uint64)parse_integer(c, &command_parse_status));
        }
        //printk("0x%x mode=%d 0x%x 0x%x\r\n", addr, mode, (uint32)(data & 0xffffffff), (uint32)(data >> 32));
        errno = mgmt_write(info, addr, mode, data);
        if (errno) {
            printk("mgmt write failed, errno = %d\r\n", errno);
            rv = CMD_FAIL;
        } else {
            //		printk("mgmt write succeeded\r\n");
        }
    } else if (!sal_strcmp(c, "read")) {
        uint32 addr;
        uint32 mode;
        uint64 rd_val;
        if (ARG_CNT(args) != 2) {
            rv = CMD_USAGE_FLASH;
            goto done;
        }
        c = ARG_GET(args);
        addr = parse_integer(c, &command_parse_status);
        c = ARG_GET(args);
        mode = parse_integer(c, &command_parse_status);
        //	printk("0x%x mode=%d\r\n", addr, mode);
        errno = mgmt_read(info, addr, mode, &rd_val);
				spi_test_buf =rd_val;
        if (errno) {
            printk("mgmt read failed, errno = %d\r\n", errno);
            rv = CMD_FAIL;
        } else {
            //		printk("mgmt read succeeded val=0x%x 0x%x\r\n", (uint32)(rd_val & 0xffffffff), (uint32)(rd_val >> 32));
        }
    } else if (!sal_strcmp(c, "avnu")) {
        if (ARG_CNT(args) < 1){
            rv = CMD_USAGE_AVNU;
            goto done;
        }
        c = ARG_GET(args);
        if ((!sal_strcmp(c, "mode")) && (ARG_CNT(args) == 1)) {
            c = ARG_GET(args);
            if(!sal_strcmp(c, "enable")) {
                value1 = MGMT_AVB_DOT1AS_ADMIN_ENABLE;
            }else if(!sal_strcmp(c, "disable")) {
                value1 = MGMT_AVB_DOT1AS_ADMIN_DISABLE;
            }else{
                printk("avnu mode <disable|enable>\r\n");
                goto done;
            }
            errno =  mgmt_avnu_global_admin_mode_set(info, value1);
            if (errno) {
                printk("avnu global admin mode set failed, errno = %d\r\n", errno);
                rv = CMD_FAIL;
            }
        }else if ((!sal_strcmp(c, "stats")) && (ARG_CNT(args) == 1)){
            c = ARG_GET(args);
            value1 = parse_integer(c, &command_parse_status); /* port */
            MGMT_CMD_INT_PARSE_STATUS(command_parse_status);
            errno = show_mgmt_avnu_port_stats(info, value1);
            if (errno) {
                printk("avnu stats get  failed, errno = %d\r\n", errno);
                rv = CMD_FAIL;
            }
        }else if ((!sal_strcmp(c, "clearstats")) && (ARG_CNT(args) == 1)) {
            c = ARG_GET(args);
            value1 = parse_integer(c, &command_parse_status); /* port */
            MGMT_CMD_INT_PARSE_STATUS(command_parse_status);
            errno = mgmt_avnu_port_stats_clear(info, value1);
            if (errno) {
                printk("avnu stats clear  failed, errno = %d\r\n", errno);
                rv = CMD_FAIL;
            }
        } else if ((!sal_strcmp(c, "numpdelaylostresp")) && (ARG_CNT(args) == 2)) {
            c = ARG_GET(args);
            value1 = parse_integer(c, &command_parse_status); /* port */
            MGMT_CMD_INT_PARSE_STATUS(command_parse_status);
            c = ARG_GET(args);
            value2 = parse_integer(c, &command_parse_status); /* last response */
            MGMT_CMD_INT_PARSE_STATUS(command_parse_status);
            errno = mgmt_avnu_port_num_lstresp_set(info, value1, value2);
            if (errno) {
                printk("avnu port num of lost responses set failed, errno = %d\r\n", errno);
                rv = CMD_FAIL;
            }
        }else if((!sal_strcmp(c, "pdelay")) && (ARG_CNT(args) == 0)) {
            errno = show_avnu_pdelay(info);
            if (errno) {
                printk("avnu pdelay get failed, errno = %d\r\n", errno);
                rv = CMD_FAIL;
            }
        } else if(!sal_strcmp(c, "status")){
            show_avnu_status(info);
        }else if ((!sal_strcmp(c, "device")) && (ARG_CNT(args) == 1)) {
            c = ARG_GET(args);
            if (!sal_strcmp(c, "state")) {
                errno = mgmt_avnu_device_state_get(info, &value1);
                if (errno) {
                    printk("AVnu device state get failed\r\n");
                    goto done;
                }

                switch(value1) {
                    case MGMT_AVNU_DEVICE_STATE_NOT_APPLICABLE :
                        break;
                    case MGMT_AVNU_DEVICE_STATE_ETH_READY :
                        printk("Device State: Ethernet is ready\r\n");
                        break;
                    case MGMT_AVNU_DEVICE_STATE_AVB_SYNC :
                        printk("Device State: AVB Sync\r\n");
                        break;
                    case MGMT_AVNU_DEVICE_STATE_SYNC_RECEIPT_TIMEOUT :
                        printk("Device State: Sync Receipt Timeout\r\n");
                        break;
                    default:
                        break;
                }
            } else {
                printk("avnu device state\r\n");
            }
        }else if(!sal_strcmp(c, "portstatus")){
            value1 = MGMT_BCM895XX_ALL_PORTS;
            if (ARG_CNT(args) == 1){
                c = ARG_GET(args);
                value1 = parse_integer(c, &command_parse_status); /* port number*/
                MGMT_CMD_INT_PARSE_STATUS(command_parse_status);
            }
            show_avnu_port_status(info, value1);
        }else if((!sal_strcmp(c, "clock")) && (ARG_CNT(args) == 2)){
            c = ARG_GET(args);
            if(!sal_strcmp(c, "state")){
                c = ARG_GET(args);
                if(!sal_strcmp(c, "gm")){
                    value1 = MGMT_AVNU_CLOCK_STATE_GM;
                }else if(!sal_strcmp(c, "slave")){
                    value1 = MGMT_AVNU_CLOCK_STATE_SLAVE;
                }else{
                    printk("avnu clock state <gm|slave>\r\n");
                    goto done;
                }
                errno = mgmt_avnu_clock_state_set(info, value1);
                if(errno){
                    printk("Error in configuring clock state, errno = %d \r\n", errno);
                    rv = CMD_FAIL;
                }
            }else{
                printk("avnu clock state <gm|slave>\r\n");
            }
        }else if((!sal_strcmp(c, "sync")) && (ARG_CNT(args) == 3)){
            c = ARG_GET(args);
            if(!sal_strcmp(c, "absence")){
                c = ARG_GET(args);
                if(!sal_strcmp(c, "timeout")){
                    c = ARG_GET(args);
                    value1 = parse_integer(c, &command_parse_status); /* sync absence timeout */
                    MGMT_CMD_INT_PARSE_STATUS(command_parse_status);
                    errno = mgmt_avnu_sync_absence_timeout_set(info, value1);
                    if(errno){
                        printk("Error in configuring sync absence timeout, errno = %d \r\n", errno);
                        rv = CMD_FAIL;
                    }
                }else{
                    printk("avnu sync absence timeout <seconds>\r\n");
                }
            }else{
                printk("avnu sync absence timeout <seconds>\r\n");
            }
        }else if((!sal_strcmp(c, "port")) && (ARG_CNT(args) == 3)){
            c = ARG_GET(args);
            if(!sal_strcmp(c, "role")){
                c = ARG_GET(args);
                value1 = parse_integer(c, &command_parse_status); /* port number */
                MGMT_CMD_INT_PARSE_STATUS(command_parse_status);
                c = ARG_GET(args);
                if(!sal_strcmp(c, "master")){
                    value2 = MGMT_DOT1AS_ROLE_MASTER;
                }else if(!sal_strcmp(c, "slave")){
                    value2 = MGMT_DOT1AS_ROLE_SLAVE;
                }else{
                    printk("avnu port role <port> <master|slave>\r\n");
                    goto done;
                }
                errno = mgmt_avnu_port_role_set(info, value1, value2);
                if(errno){
                    printk("Error in configuring port role, errno = %d\r\n", errno);
                    rv = CMD_FAIL;
                }
            }else{
                printk("avnu port role <port> <master|slave>\r\n");
            }
        } else if((!sal_strcmp(c, "ascapable")) && (ARG_CNT(args) == 2)){
            c = ARG_GET(args);
            value1 = parse_integer(c, &command_parse_status); /* port number */
            MGMT_CMD_INT_PARSE_STATUS(command_parse_status);
            c = ARG_GET(args);
            if(!sal_strcmp(c, "true")){
                value2 = MGMT_AVNU_AS_CAPABLE_TRUE;
            }else if(!sal_strcmp(c, "false")){
                value2 = MGMT_AVNU_AS_CAPABLE_FALSE;
            }else{
                printk("avnu ascapable <port> <true|false>\r\n");
                goto done;
            }
            errno = mgmt_avnu_as_capable_mode_set(info, value1, value2);
            if(errno){
                printk("Error in configuring AS capability, errno = %d \r\n", errno);
                rv = CMD_FAIL;
            }
        } else if((!sal_strcmp(c, "pdelay")) && (ARG_CNT(args) == 2)) {
            c = ARG_GET(args);
            value1 = parse_integer(c, &command_parse_status); /* port number */
            MGMT_CMD_INT_PARSE_STATUS(command_parse_status);
            c = ARG_GET(args);
            value2 = parse_integer(c, &command_parse_status);
            MGMT_CMD_INT_PARSE_STATUS(command_parse_status);
            errno = mgmt_avnu_nbr_pdelay_set(info, value1, value2);
            if(errno){
                printk("Error in configuring PDelay value, errno = %d \r\n", errno);
                rv = CMD_FAIL;
            }
        }else if((!sal_strcmp(c, "interval")) && (ARG_CNT(args) == 3)) {
            c = ARG_GET(args);
            if(!sal_strncmp("pdelay_oper", c, strlen("pdelay_oper"))){
                c = ARG_GET(args);
                value1 = parse_integer(c, &command_parse_status); /* port number */
                MGMT_CMD_INT_PARSE_STATUS(command_parse_status);
                c = ARG_GET(args);
                value2 = parse_integer(c, &command_parse_status); /*pdelay interval */
                MGMT_CMD_INT_PARSE_STATUS(command_parse_status);
                errno = mgmt_avnu_operational_log_pdelay_interval_set(info, value1, (int32)value2);
                if(errno){
                    printk("Error in configuring Operatonal Log Pdelay, errno = %d \r\n", errno);
                    rv = CMD_FAIL;
                    goto done;
                }
                goto done;
            }
            value1 = parse_integer(c, &command_parse_status); /* port number */
            MGMT_CMD_INT_PARSE_STATUS(command_parse_status);
            c = ARG_GET(args);
            if(!sal_strcmp(c, "pdelay")){
                c = ARG_GET(args);
                value2 = parse_integer(c, &command_parse_status); /*pdelay interval */
                MGMT_CMD_INT_PARSE_STATUS(command_parse_status);
                errno = mgmt_avnu_initial_log_pdelay_interval_set(info, value1, (int32)value2);
                if(errno){
                    printk("Error in configuring Initial Log Pdelay Interval, errno = %d \r\n", errno);
                    rv = CMD_FAIL;
                }
            }else if(!sal_strcmp(c, "sync")){
                c = ARG_GET(args);
                value2 = parse_integer(c, &command_parse_status); /*sync interval */
                MGMT_CMD_INT_PARSE_STATUS(command_parse_status);
                errno = mgmt_avnu_initial_log_sync_interval_set(info, value1, (int32)value2);
                if(errno){
                    printk("Error in configuring Initial Log SYNC Interval, errno = %d \r\n", errno);
                    rv = CMD_FAIL;
                }
            }
        }else {
            rv = CMD_USAGE_AVNU;
        }
    } else if (!sal_strcmp(c, "switch")) {
        if (ARG_CNT(args) < 1){
            rv = CMD_USAGE_SWITCH;
            goto done;
        }
        c = ARG_GET(args);
        if((!sal_strcmp(c, "age_time")) && (ARG_CNT(args) < 3)){
            c = ARG_GET(args);
            if ((!sal_strcmp(c,"set")) && (ARG_CNT(args) == 1)) {
                c = ARG_GET(args);
                value1 = parse_integer(c, &command_parse_status); /* AGE_TIME */
                MGMT_CMD_INT_PARSE_STATUS(command_parse_status);
                errno = mgmt_switch_age_time_set(info, value1);
                if (errno) {
                    printk("Switch age time set failed, errno = %d\r\n", errno);
                    rv = CMD_FAIL;
                }
            } else if ((!sal_strcmp(c,"get")) && (ARG_CNT(args) == 0)) {
                errno = mgmt_switch_age_time_get(info, &value1);
                if (errno) {
                    printk("Failed to get AGE time, errno = %d\r\n", errno);
                    rv = CMD_FAIL;
                }else {
                    printk("Switch FDB age timeout (secs) =%d\r\n",value1);
                }
            } else{
                rv = CMD_USAGE_SWITCH;
            }
        }else if((!sal_strcmp(c, "dumbfwd")) && (ARG_CNT(args) > 0)){
            c = ARG_GET(args);
            if((!sal_strcmp(c, "set")) && (ARG_CNT(args) == 1)){
                c = ARG_GET(args);
                if( !sal_strcmp(c, "disable")) {
                    value1 = DISABLED;
                }else if( !sal_strcmp(c, "enable")) {
                    value1 = ENABLED;
                }else{
                    printk("switch dumbfwd set <disable|enable>\r\n");
                    goto done;
                }
                errno = mgmt_port_dumbfwd_mode_set(info, value1);
                if(errno){
                    printk("Failed to set P8 dumb forwarding mode, errno = %d\r\n", errno);
                    rv = CMD_FAIL;
                }else{
                    printk("P8 Dumb Forwarding is %s\r\n", (value1) ? "Enabled" : "Disabled");
                }
            }else if((!sal_strcmp(c,"get")) && (ARG_CNT(args) == 0)){
                errno = mgmt_port_dumbfwd_mode_get(info, &value1);
                if(errno){
                    printk("Failed to get P8 Dumb Forwarding  Mode, errno = %d\r\n", errno);
                    rv = CMD_FAIL;
                }else{
                    printk("P8 Dumb Forwarding is %s", (value1) ? "Enabled" : "Disabled");
                }
            } else{
                rv = CMD_USAGE_SWITCH;
            }
        }else if (!sal_strcmp(c, "mirror")) {
            if (ARG_CNT(args) < 1){
                rv = CMD_USAGE_SWITCH;
                goto done;
            }
            c = ARG_GET(args);
            if ((!sal_strcmp(c, "enable"))  && (ARG_CNT(args) == 3)){
                c = ARG_GET(args);
                value1 = parse_integer(c, &command_parse_status); /* port bitmap */
                MGMT_CMD_INT_PARSE_STATUS(command_parse_status);
                c = ARG_GET(args);
                value2 = parse_integer(c, &command_parse_status); /* probe port */
                MGMT_CMD_INT_PARSE_STATUS(command_parse_status);

                c = ARG_GET(args); /* probe mode engress/igress/all */
                if(!sal_strcmp(c, "ingress")){
                    value3 = MGMT_INTF_MIRROR_INGRESS;
                }else if(!sal_strcmp(c, "egress")){
                    value3 = MGMT_INTF_MIRROR_EGRESS;
                }else if(!sal_strcmp(c, "all")){
                    value3 = MGMT_INTF_MIRROR_BOTH;
                }else{
                    printk("switch mirror enable <port bitmap> <probe port> <all|ingress|egress>\r\n");
                    goto done;
                }

                errno = mgmt_mirror_enable(info, value1, value2, value3);
                if (errno) {
                    printk("Failed to enable mirroring mode, errno = %d\r\n", errno);
                    rv = CMD_FAIL;
                }
            }else if (!sal_strcmp(c,"disable")) {
                errno = mgmt_mirror_disable(info);
                if (errno) {
                    printk("Failed to disable mirroring mode, errno = %d\r\n", errno);
                    rv = CMD_FAIL;
                }
            }else if (!sal_strcmp(c,"status")) {
                errno = mgmt_mirror_status(info, &value1, &value2, &value3, &value4);
                if (errno) {
                    printk("Failed to get mirror status, errno = %d\r\n", errno);
                    rv = CMD_FAIL;
                }
                if (value1 == ENABLED) {
                    printk("Mirroring is enabled for %s traffic with port_bitmap=0x%x and probe_port=%d\r\n",
                            value4 == MIRROR_INGRESS ? "ingress": (value4 == MIRROR_EGRESS ? "egress" : "all"),
                            value2, value3);
                } else {
                    printf("Mirroring is disabled\r\n");
                }
            }else{
                rv = CMD_USAGE_SWITCH;
            }
        }else if((!sal_strcmp(c, "port")) && (ARG_CNT(args) == 3)){
            c = ARG_GET(args);
            if(!sal_strcmp(c, "mibs")){
                c = ARG_GET(args);
                value1 = parse_integer(c, &command_parse_status); /* port */
                MGMT_CMD_INT_PARSE_STATUS(command_parse_status);
                c = ARG_GET(args);
                if(!sal_strcmp(c, "all")){
                    mgmt_port_mibs(info, value1, 1, N_MGMT_MIB_REGISTERS);
                }else{
                    value2 = parse_integer(c, &command_parse_status); /* mibs stat key val */
                    MGMT_CMD_INT_PARSE_STATUS(command_parse_status);
                    for(i = 0; i < N_MGMT_MIB_REGISTERS; i++){
                        if(value2 == mgmt_mib_registers[i].offset){
                            value2 = i;
                            break;
                        }
                    }
                    if(i == N_MGMT_MIB_REGISTERS){
                        printk("Invalid MIB counter. Counter = 0x%x\r\n", value2);
                        goto done;
                    }
                    mgmt_port_mibs(info, value1, 1, value2);
                }
            }else{
                printk("switch port mibs <port> <counter|all>\r\n");
            }
        }else{
            rv = CMD_USAGE_SWITCH;
        }
    } else if (!sal_strcmp(c, "port")) {
        if (ARG_CNT(args) < 1){
            rv = CMD_USAGE_PORT;
            goto done;
        }
        c = ARG_GET(args);
        if ((!sal_strcmp(c, "info")) && (ARG_CNT(args) == 1)){
            c = ARG_GET(args);
            value1 = parse_integer(c, &command_parse_status); /* port */
            MGMT_CMD_INT_PARSE_STATUS(command_parse_status);
            errno = show_mgmt_port_info(info, value1);
            if (errno) {
                printk("Failed to get port info , errno = %d\r\n", errno);
                rv = CMD_FAIL;
            }
        }else if ((!sal_strcmp(c, "stats")) && (ARG_CNT(args) == 1)){
            c = ARG_GET(args);
            value1 = parse_integer(c, &command_parse_status); /* port */
            MGMT_CMD_INT_PARSE_STATUS(command_parse_status);
            errno = show_mgmt_port_stats(info, value1);
            if (errno) {
                printk("Failed to get port stats , errno = %d\r\n", errno);
                rv = CMD_FAIL;
            }
        }else if(!sal_strcmp(c, "admin_mode")) {
            c = ARG_GET(args);
            if ((!sal_strcmp(c,"set")) && (ARG_CNT(args) == 2)) {
                c = ARG_GET(args);
                value1 = parse_integer(c, &command_parse_status); /* port */
                MGMT_CMD_INT_PARSE_STATUS(command_parse_status);
                c = ARG_GET(args);
                if(!sal_strcmp(c,"enable")) {
                    value2 = ENABLED;
                }else if(!sal_strcmp(c,"disable")) {
                    value2 = DISABLED;
                }else{
                    printk("port admin_mode set <port> <disable|enable>\r\n");
                    goto done;
                }
                errno = mgmt_port_admin_mode_set(info, value1, value2);
                if (errno) {
                    printk("Failed to set admin mode, errno = %d\r\n", errno);
                    rv = CMD_FAIL;
                }
            } else if ((!sal_strcmp(c,"get")) && (ARG_CNT(args) == 1)) {
                c = ARG_GET(args);
                value1 = parse_integer(c, &command_parse_status); /* port */
                MGMT_CMD_INT_PARSE_STATUS(command_parse_status);
                errno = mgmt_port_admin_mode_get(info, value1, &value2);
                if (errno) {
                    printk("Failed to get admin mode, errno = %d\r\n", errno);
                    rv = CMD_FAIL;
                }else{
                    printk("Admin mode: %s\r\n", (value2 == ENABLED) ? "Enabled" : "Disabled");
                    goto done;
                }
            } else{
                rv = CMD_USAGE_PORT;
                goto done;
            }
        }else if(!sal_strcmp(c, "speed")){
            c = ARG_GET(args);
            if ((!sal_strcmp(c,"set")) && (ARG_CNT(args) == 2)) {
                c = ARG_GET(args);
                value1 = parse_integer(c, &command_parse_status); /* port */
                MGMT_CMD_INT_PARSE_STATUS(command_parse_status);
                c = ARG_GET(args);
                value2 = -1;
                if (!sal_strcmp(c,"auto")){
                    value2 = 0;
                }else if (!sal_strcmp(c,"100")){
                    value2 = 100;
                }else if (!sal_strcmp(c,"1000")){
                    value2 = 1000;
                } else {
                    printk("port speed set <port> <auto|100|1000>\r\n");
                    goto done;
                }
                errno = mgmt_port_speed_set(info, value1, value2);
                if (errno) {
                    printk("Failed to set port speed, errno = %d\r\n", errno);
                    rv = CMD_FAIL;
                }
            } else if ((!sal_strcmp(c,"get")) && (ARG_CNT(args) == 1)) {
                c = ARG_GET(args);
                value1 = parse_integer(c, &command_parse_status); /* port */
                MGMT_CMD_INT_PARSE_STATUS(command_parse_status);
                errno = mgmt_port_speed_get(info, value1, &value2 );
                if (errno) {
                    printk("Failed to get speed, errno = %d\r\n", errno);
                    rv = CMD_FAIL;
                }else{
                    printk("Speed: %u\r\n", value2);
                    goto done;
                }
            }else{
                rv = CMD_USAGE_PORT;
                goto done;
            }
        }else if(!sal_strcmp(c, "master_slave")) {
            c = ARG_GET(args);
            if ((!sal_strcmp(c,"set")) && (ARG_CNT(args) == 2)){
                c = ARG_GET(args);
                value1 = parse_integer(c, &command_parse_status); /* port */
                MGMT_CMD_INT_PARSE_STATUS(command_parse_status);
                c = ARG_GET(args);
                if( !sal_strcmp(c,"slave")) {
                    value2 = SLAVE;
                }else if( !sal_strcmp(c,"master")) {
                    value2 = MASTER;
                }else{
                    printk("port master_slave set <port> <slave|master>\r\n");
                    goto done;
                }
                errno =  mgmt_port_master_slave_set(info, value1, value2 );
                if (errno) {
                    printk("Failed to set BR mode, errno = %d\r\n", errno);
                    rv = CMD_FAIL;
                }
            } else if ((!sal_strcmp(c,"get")) && (ARG_CNT(args) == 1)) {
                c = ARG_GET(args);
                value1 = parse_integer(c, &command_parse_status); /* port */
                MGMT_CMD_INT_PARSE_STATUS(command_parse_status);
                errno = mgmt_port_master_slave_get(info, value1, &value2 );
                if (errno) {
                    printk("Failed to get master/slave mode, errno = %d\r\n", errno);
                    rv = CMD_FAIL;
                }else{
                    printk("Master/Slave: %s\r\n", (value2 == MASTER) ? "Master" : "Slave");
                    goto done;
                }
            } else{
                rv = CMD_USAGE_PORT;
                goto done;
            }
        }else if(!sal_strcmp(c, "phy_lb_mode")) {
            c = ARG_GET(args);
            if ((!sal_strcmp(c,"set")) && (ARG_CNT(args) == 2)) {
                c = ARG_GET(args);
                value1 = parse_integer(c, &command_parse_status); /* port */
                MGMT_CMD_INT_PARSE_STATUS(command_parse_status);
                c = ARG_GET(args);
                if( !sal_strcmp(c,"disable")) {
                    value2 = DISABLED;
                }else if( !sal_strcmp(c,"enable")) {
                    value2 = ENABLED;
                }else{
                    printk("port phy_lb_mode set <port> <disable|enable>\r\n");
                    goto done;
                }
                errno = mgmt_port_phy_lb_set(info, value1, value2 );
                if (errno) {
                    printk("Failed to set phy loop back mode, errno = %d\r\n", errno);
                    rv = CMD_FAIL;
                }
            } else if ((!sal_strcmp(c,"get")) && (ARG_CNT(args) == 1)) {
                c = ARG_GET(args);
                value1 = parse_integer(c, &command_parse_status); /* port */
                MGMT_CMD_INT_PARSE_STATUS(command_parse_status);
                errno = mgmt_port_phy_lb_get(info, value1, &value2 );
                if (errno) {
                    printk("Failed to get phy_lb mode, errno = %d\r\n", errno);
                    rv = CMD_FAIL;
                }else{
                    printk("Phy_lb mode: %s\r\n", (value2 == ENABLED) ? "Enabled" : "Disabled");
                    goto done;
                }
            } else{
                rv = CMD_USAGE_PORT;
                goto done;
            }
        }else if(!sal_strcmp(c, "jumbo_frame")) {
            c = ARG_GET(args);
            if ((!sal_strcmp(c,"set")) && (ARG_CNT(args) == 2)) {
                c = ARG_GET(args);
                value1 = parse_integer(c, &command_parse_status); /* port */
                MGMT_CMD_INT_PARSE_STATUS(command_parse_status);
                c = ARG_GET(args);
                if( !sal_strcmp(c,"disable")) {
                    value2 = DISABLED;
                }else if( !sal_strcmp(c,"enable")) {
                    value2 = ENABLED;
                }else{
                    printk("port jumbo_frame set <port> <disable|enable>\r\n");
                    goto done;
                }
                errno = mgmt_port_jumbo_frame_mode_set(info, value1, value2 );
                if (errno) {
                    printk("Failed to set jumbo frame mode, errno = %d\r\n", errno);
                    rv = CMD_FAIL;
                }
            } else if ((!sal_strcmp(c,"get")) && (ARG_CNT(args) == 1)) {
                c = ARG_GET(args);
                value1 = parse_integer(c, &command_parse_status); /* port */
                MGMT_CMD_INT_PARSE_STATUS(command_parse_status);
                errno = mgmt_port_jumbo_frame_mode_get(info, value1, &value2 );
                if (errno) {
                    printk("Failed to get jumbo frame mode, errno = %d\r\n", errno);
                    rv = CMD_FAIL;
                }else{
                    printk("Jumbo frame mode: %s\r\n", (value2 == ENABLED) ? "Enabled" : "Disabled");
                    goto done;
                }
            } else {
                rv = CMD_USAGE_PORT;
                goto done;
            }
        }else if((!sal_strcmp(c, "link")) && (ARG_CNT(args) <= 2)){
            c = ARG_GET(args);
            if ((!sal_strcmp(c, "sqi")) && (ARG_CNT(args) < 2)) {
                if(ARG_CNT(args) == 1){
                    c = ARG_GET(args);
                    value1 = parse_integer(c, &command_parse_status);
                    MGMT_CMD_INT_PARSE_STATUS(command_parse_status);
                    if (!mgmt_port_link_sqi_get(info, value1, &sqi)) {
                        printk("Signal Quality Indicator(SQI) value of port %d is %d\r\n", value1, sqi);
                    }
                }else {
                    MGMT_AVB_TRACE_LINE(20);
                    printk("%7s %10s\r\n", "Port", "SQI");
                    MGMT_AVB_TRACE_LINE(20);
                    for(i = 0; i < MGMT_AVB_MAX_BR_PORT; i++){
                        mgmt_port_info_t port_info;
                        sal_memset(&port_info, 0x00, sizeof(mgmt_port_info_t));
                        mgmt_port_info_get(info, i, &port_info);
                        if (port_info.link_status  == LINK_DOWN) {
                            continue;
                        }

                        if (!mgmt_port_link_sqi_get(info, i, &sqi)) {
                            printk("%7d %10d\r\n", i, sqi);
                        }
                    }
                    MGMT_AVB_TRACE_LINE(20);
                }
            }else{
                printk("port link sqi [port]\r\n");
                goto done;
            }
        }else {
            rv = CMD_USAGE_PORT;
        }
    } else if (!sal_strcmp(c, "vlan")){
        if (ARG_CNT(args) < 1){
            rv = CMD_USAGE_VLAN;
            goto done;
        }
        c = ARG_GET(args);
        if ((!sal_strcmp(c, "create")) && (ARG_CNT(args) == 1)){
            c = ARG_GET(args);
            value1 = parse_integer(c, &command_parse_status); /* vlan */
            MGMT_CMD_INT_PARSE_STATUS(command_parse_status);
            errno =  mgmt_vlan_create(info, value1);
            if (errno) {
                printk("Failed to create the VLAN, errno = %d\r\n", errno);
                rv = CMD_FAIL;
            }
        }else if ((!sal_strcmp(c, "delete")) && (ARG_CNT(args) == 1)) {
            c = ARG_GET(args);
            value1 = parse_integer(c, &command_parse_status); /* vlan */
            MGMT_CMD_INT_PARSE_STATUS(command_parse_status);
            errno = mgmt_vlan_delete(info, value1);
            if (errno) {
                printk("Failed to delete the VLAN, errno = %d\r\n", errno);
                rv = CMD_FAIL;
            }
        }else if ((!sal_strcmp(c, "get"))&& (ARG_CNT(args) == 1)) {
            c = ARG_GET(args);
            value1 = parse_integer(c, &command_parse_status); /* vlan */
            MGMT_CMD_INT_PARSE_STATUS(command_parse_status);
            errno = mgmt_vlan_get(info, value1, &value2, &value3);
            if (errno) {
                printk("Invalid VLAN/ VLAN does not exist, errno = %d\r\n", errno);
                rv = CMD_FAIL;
            }else{
                printk("VLAN %d Tagged Ports  : ", value1);
                mgmt_avb_internal_mask_to_external_port_print(value2 & (~value3));
                printk("\r\n");
                printk("VLAN %d Untagged Ports: ", value1);
                mgmt_avb_internal_mask_to_external_port_print(value3);
                printk("\r\n");
            }
        }else if ((!sal_strcmp(c, "portadd")) && (ARG_CNT(args) == 3)) {
            c = ARG_GET(args);
            value1 = parse_integer(c, &command_parse_status); /* vlan */
            MGMT_CMD_INT_PARSE_STATUS(command_parse_status);
            c = ARG_GET(args);
            value2 = parse_integer(c, &command_parse_status); /* port */
            MGMT_CMD_INT_PARSE_STATUS(command_parse_status);
            c = ARG_GET(args);
            if(!sal_strcmp(c, "tag")) {
                value3 = MGMT_AVB_VLAN_TAG;
            }else if(!sal_strcmp(c, "untag")) {
                value3 = MGMT_AVB_VLAN_UNTAG;
            }else{
                printk("vlan portadd <vlan id> <port> <tag|untag>\r\n");
                goto done;
            }
            errno = mgmt_vlan_portadd(info, value1, value2, value3);
            if (errno) {
                printk("Failed to add the port to the VLAN, errno = %d\r\n", errno);
                rv = CMD_FAIL;
            }
        }else if ((!sal_strcmp(c, "portdel")) && (ARG_CNT(args) == 2)){
            c = ARG_GET(args);
            value1 = parse_integer(c, &command_parse_status); /* vlan */
            MGMT_CMD_INT_PARSE_STATUS(command_parse_status);
            c = ARG_GET(args);
            value2 = parse_integer(c, &command_parse_status); /* port */
            MGMT_CMD_INT_PARSE_STATUS(command_parse_status);
            errno = mgmt_vlan_portdel(info, value1, value2);
            if (errno) {
                printk("Failed to delete the port from the VLAN, errno = %d\r\n", errno);
                rv = CMD_FAIL;
            }
        }else if ((!sal_strcmp(c, "pvidset")) && (ARG_CNT(args) == 2)) {
            c = ARG_GET(args);
            value1 = parse_integer(c, &command_parse_status); /* port */
            MGMT_CMD_INT_PARSE_STATUS(command_parse_status);
            c = ARG_GET(args);
            value2 = parse_integer(c, &command_parse_status); /* pvid */
            MGMT_CMD_INT_PARSE_STATUS(command_parse_status);
            errno = mgmt_vlan_pvid_set(info, value1, value2);
            if (errno) {
                printk("Failed to set the PVID, errno = %d\r\n", errno);
                rv = CMD_FAIL;
            }
        }else if ((!sal_strcmp(c, "ifilterset"))&& (ARG_CNT(args) == 1)) {
            c = ARG_GET(args);
            if(!sal_strcmp(c, "enable")) {
                value1 = MGMT_AVB_INGRESS_FILTER_MODE_ENABLE;
            }else if(!sal_strcmp(c, "disable")) {
                value1 = MGMT_AVB_INGRESS_FILTER_MODE_DISABLE;
            }else{
                printk("vlan ifilterset <disable|enable>\r\n");
                goto done;
            }
            errno =  mgmt_vlan_ifilter_set(info, value1);
            if (errno) {
                printk("Failed to set the VLAN ingress filter, errno = %d\r\n", errno);
                rv = CMD_FAIL;
            }
        }else if ((!sal_strcmp(c,"prioset")) && (ARG_CNT(args) == 2)) {
            c = ARG_GET(args);
            value1 = parse_integer(c, &command_parse_status); /* port */
            MGMT_CMD_INT_PARSE_STATUS(command_parse_status);
            c = ARG_GET(args);
            value2 = parse_integer(c, &command_parse_status); /* priority */
            MGMT_CMD_INT_PARSE_STATUS(command_parse_status);
            errno = mgmt_vlan_priority_set(info, value1, value2);
            if (errno) {
                printk("Failed to set the untagged VLAN priority, errno = %d\r\n", errno);
                rv = CMD_FAIL;
            }
        }else if((!sal_strcmp(c, "port")) && (ARG_CNT(args) == 1)) {
            c = ARG_GET(args);
            if(!sal_strcmp(c, "show")){
                show_mgmt_vlan_port_info(info);
            }else{
                printk("vlan port show\r\n");
                goto done;
            }
        } else {
            rv = CMD_USAGE_VLAN;
        }
    } else if (!sal_strcmp(c, "msrp")) {
        if (ARG_CNT(args) < 1){
            rv = CMD_USAGE_MSRP;
            goto done;
        }
        c = ARG_GET(args);
        if (!sal_strcmp(c, "entry")) {
            if (ARG_CNT(args) < 1){
                rv = CMD_USAGE_MSRP;
                goto done;
            }
            c = ARG_GET(args);
            if ((!sal_strcmp(c, "add"))&& (ARG_CNT(args) >= 3)){
                mac_addr = ARG_GET(args); /* mac address string */
                c = ARG_GET(args);
                value1 = parse_integer(c, &command_parse_status); /* vlan id */
                MGMT_CMD_INT_PARSE_STATUS(command_parse_status);
                c = ARG_GET(args);
                value2 = parse_integer(c, &command_parse_status); /* port or port bitmap(set msb as 1) */
                MGMT_CMD_INT_PARSE_STATUS(command_parse_status);
                value3 = 0;
                value4 = 0;
                value5 = 0;

                if ((ARG_CNT(args) >= 2)) {
                    c = ARG_GET(args);
                    value3 = parse_integer(c, &command_parse_status); /* ingress CIR */
                    MGMT_CMD_INT_PARSE_STATUS(command_parse_status);
                    c = ARG_GET(args);
                    value4 = parse_integer(c, &command_parse_status); /* ingress CBS */
                    MGMT_CMD_INT_PARSE_STATUS(command_parse_status);
                    if ((ARG_CNT(args) > 0)) {
                        c = ARG_GET(args);
                        value5 = parse_integer(c, &command_parse_status); /* ingress port bitmap */
                        MGMT_CMD_INT_PARSE_STATUS(command_parse_status);
                    }
                }
                errno = mgmt_fdb_entry_add(info, mac_addr, value1, value2,
                                           value3, value4, value5);
                if (errno) {
                    printk("msrp entry add <xx:xx:xx:xx:xx:xx> <vlan id> <port|mcast_port_bitmap> "
                           "[<cir_kbps> <cbs_bytes> <ingress_port_bitmap>]\r\n");
                    printk("Failed to add stream entry, errno = %d\r\n", errno);
                    rv = CMD_FAIL;
                }
            }else if ((!sal_strcmp(c,"delete")) && (ARG_CNT(args) == 2)){
                mac_addr = ARG_GET(args); /* mac address string */
                c = ARG_GET(args);
                value1 = parse_integer(c, &command_parse_status); /* vlan id */
                MGMT_CMD_INT_PARSE_STATUS(command_parse_status);
                errno = mgmt_fdb_entry_delete(info, mac_addr, value1);
                if (errno) {
                    printk("msrp entry delete <xx:xx:xx:xx:xx:xx> <vlan id>\r\n");
                    printk("Failed to delete the msrp entry, errno = %d\r\n", errno);
                    rv = CMD_FAIL;
                }
            }else if ((!sal_strcmp(c,"bandwidth")) && (ARG_CNT(args) == 3)){
                c = ARG_GET(args);
                if (!sal_strcmp(c,"get")) {
                    mac_addr = ARG_GET(args); /* mac address string */
                    c = ARG_GET(args);
                    value1 = parse_integer(c, &command_parse_status); /* vlan id */
                    MGMT_CMD_INT_PARSE_STATUS(command_parse_status);
                    errno = mgmt_stream_bandwidth_get(info, mac_addr, value1, &value2, &value3);
                    if (errno) {
                        printk("Failed to get the stream bandwidth, errno = %d\r\n", errno);
                        rv = CMD_FAIL;
                    } else {
                        printk("Rate Kbps   : %d\r\n", value2);
                        printk("Burst Bytes : %d\r\n", value3);
                    }
                }
            }else if ((!sal_strcmp(c,"show")) && (ARG_CNT(args) < 3)){
                value1 = MGMT_AVB_ARL_MATCH_ALL_VLANS;
                value2 = MGMT_AVB_ARL_MATCH_ALL_PORTS;
                if (ARG_CNT(args) >= 1){
                    c = ARG_GET(args);
                    value1 = parse_integer(c, &command_parse_status);/*vlan*/
                    MGMT_CMD_INT_PARSE_STATUS(command_parse_status);
                }
                if (ARG_CNT(args) == 1){
                    c = ARG_GET(args);
                    value2 = parse_integer(c, &command_parse_status);/*port*/
                    MGMT_CMD_INT_PARSE_STATUS(command_parse_status);
                }
                errno = mgmt_fdb_entry_get(info, value1, value2);
                if (errno) {
                    printk("msrp entry get failed, errno = %d\r\n", errno);
                    rv = CMD_FAIL;
                }
            }else{
                rv = CMD_USAGE_MSRP;
            }
        }else if ((!sal_strcmp(c, "bandwidth")) && (ARG_CNT(args) >= 3)){
            c = ARG_GET(args);
            if ((!sal_strcmp(c, "get"))  && (ARG_CNT(args) == 2)) {
                c = ARG_GET(args);
                value1 = parse_integer(c, &command_parse_status); /* port */
                MGMT_CMD_INT_PARSE_STATUS(command_parse_status);
                c = ARG_GET(args);
                if (!sal_strcmp(c,"classA")) {
                    value2 = MGMT_AVB_STREAM_CLASS_TYPE_A;
                }else if (!sal_strcmp(c,"classB")) {
                    value2 = MGMT_AVB_STREAM_CLASS_TYPE_B;
                }else{
                    printk("Invalid stream class \r\n");
                    goto done;
                }
                errno = mgmt_port_bandwidth_get(info, value1, value2, &value3, &value4);
                if (errno == -1) {
                    printk("Failed to get bandwidth info, errno = %d\r\n", errno);
                    rv = CMD_FAIL;
                }else if( errno == 0){
                    printk("Rate Kbps   : %d\r\n", value3);
                    printk("Burst Kbits : %d\r\n", value4);
                }
            }else if ((!sal_strcmp(c,"set"))&& (ARG_CNT(args) == 4)) {
                c = ARG_GET(args);
                value1 = parse_integer(c, &command_parse_status); /* port */
                MGMT_CMD_INT_PARSE_STATUS(command_parse_status);
                c = ARG_GET(args);
                if (!sal_strcmp(c,"classA")) {
                    value2 = MGMT_AVB_STREAM_CLASS_TYPE_A;
                }else if (!sal_strcmp(c,"classB")) {
                    value2 = MGMT_AVB_STREAM_CLASS_TYPE_B;
                }else{
                    printk("Invalid stream class \r\n");
                    goto done;
                }
                c = ARG_GET(args);
                value3 = parse_integer(c, &command_parse_status); /* rate */
                MGMT_CMD_INT_PARSE_STATUS(command_parse_status);
                c = ARG_GET(args);
                value4 = parse_integer(c, &command_parse_status); /* burst */
                MGMT_CMD_INT_PARSE_STATUS(command_parse_status);
                errno = mgmt_port_bandwidth_set(info, value1, value2, value3, value4);
                if (errno) {
                    printk("Failed to set bandwidth, errno = %d\r\n", errno);
                    rv = CMD_FAIL;
                }
            }else{
                rv = CMD_USAGE_MSRP;
            }
        } else if ((!sal_strcmp(c, "pcpmapping")) && (ARG_CNT(args) > 1)){
            c = ARG_GET(args);
            if ((!sal_strcmp(c, "get")) && (ARG_CNT(args) == 1)){
                c = ARG_GET(args);
                if (!sal_strcmp(c,"classA")) {
                    value1 = MGMT_AVB_STREAM_CLASS_TYPE_A;
                }else if (!sal_strcmp(c,"classB")) {
                    value1 = MGMT_AVB_STREAM_CLASS_TYPE_B;
                }else{
                    printk("Invalid stream class \r\n");
                    goto done;
                }
                errno = mgmt_pcp_mapping_get(info, value1, &value2, &value3);
                if (errno) {
                    printk("Failed to get PCP mapping info, errno = %d\r\n", errno);
                    rv = CMD_FAIL;
                }else{
                    printk("PCP Prio  : %d\r\n", value2);
                    printk("PCP Remap : %d\r\n", value3);
                }
            }else if ((!sal_strcmp(c,"set")) && (ARG_CNT(args) == 4)) {
                c = ARG_GET(args);
                value1 = parse_integer(c, &command_parse_status); /* prioA */
                MGMT_CMD_INT_PARSE_STATUS(command_parse_status);
                c = ARG_GET(args);
                value2 = parse_integer(c, &command_parse_status); /* remap prioA */
                MGMT_CMD_INT_PARSE_STATUS(command_parse_status);
                c = ARG_GET(args);
                value3 = parse_integer(c, &command_parse_status); /* prioB */
                MGMT_CMD_INT_PARSE_STATUS(command_parse_status);
                c = ARG_GET(args);
                value4 = parse_integer(c, &command_parse_status); /* remap prioB */
                MGMT_CMD_INT_PARSE_STATUS(command_parse_status);
                errno = mgmt_pcp_mapping_set(info, MGMT_AVB_STREAM_CLASS_TYPE_A, value1, value2);
                if (errno) {
                    printk("Failed to set class A PCP mappings, errno = %d\r\n", errno);
                    rv = CMD_FAIL;
                } else {
                    errno = mgmt_pcp_mapping_set(info, MGMT_AVB_STREAM_CLASS_TYPE_B, value3, value4);
                    if (errno) {
                        printk("Failed to set class B PCP mappings, errno = %d\r\n", errno);
                        rv = CMD_FAIL;
                    }
                }
            }else{
                rv = CMD_USAGE_MSRP;
            }
        }else if((!sal_strcmp(c, "boundary")) && (ARG_CNT(args) > 1)){
            c = ARG_GET(args);
            if((!sal_strcmp(c, "get")) && (ARG_CNT(args) == 1)){
                c = ARG_GET(args);
                value1 = parse_integer(c, &command_parse_status); /* port number */
                MGMT_CMD_INT_PARSE_STATUS(command_parse_status);
                errno = mgmt_fdb_boundary_get(info, value1, &value2, &value3);
                if (errno) {
                    printk("Failed to get port boundary type\r\n");
                    rv = CMD_FAIL;
                    goto done;
                }
                printk("Port Number: %d, Class A Boundary Value: %d, Class B Boundary Value: %d\r\n",
				                          value1, value2, value3);
            } else if((!sal_strcmp(c, "set")) && (ARG_CNT(args) == 3)) {
                c = ARG_GET(args);
                value1 = parse_integer(c, &command_parse_status); /* port number */
                MGMT_CMD_INT_PARSE_STATUS(command_parse_status);
                c = ARG_GET(args); /* class type */
                if(!sal_strcmp(c, "classA")){
                    value2 = MGMT_AVB_STREAM_CLASS_TYPE_A;
                }else if(!sal_strcmp(c, "classB")){
                    value2 = MGMT_AVB_STREAM_CLASS_TYPE_B;
                }else{
                    printk("Usage: msrp boundary set <port> <classA|classB> <is_boundary>\r\n");
                    goto done;
                }
                c = ARG_GET(args); /* is boundary */
                value3 = parse_integer(c, &command_parse_status);
                MGMT_CMD_INT_PARSE_STATUS(command_parse_status);
                errno = mgmt_fdb_boundary_set(info, value1, value2, value3);
                if (errno) {
                    printk("Failed to set port boundary type\r\n");
                    rv = CMD_FAIL;
                    goto done;
                }
            }else{
                rv = CMD_USAGE_MSRP;
            }
        }else{
            rv = CMD_USAGE_MSRP;
        }
    }else if (!sal_strcmp(c, "heartbeat")) {
        if (ARG_CNT(args) != 1){
            rv = CMD_USAGE_HEARTBEAT;
            goto done;
        }
        c = ARG_GET(args);
        value1 = parse_integer(c, &command_parse_status); /* heart beat interval */
        MGMT_CMD_INT_PARSE_STATUS(command_parse_status);
        mgmt_heartbeat_set(info, value1);
    } else{
        printk("Unknown Command, Type: 'help'\r\n");
    }
done:
    return rv;
}

/*
 * Function:    parse_start_word
 * Purpose:     Start a new parsed word, checking for argv overflow.
 * Parameters:  a - pointer to args structure
 *              w - pointer to word to start.
 * Returns:     0 - OK
 *              -1 - failed (to many parameters).
 */
static int32 parse_start_word(args_t *a, int8 *w)
{
    if (a->a_argc >= ARGS_CNT) {
        return(-1);
    } else {
        a->a_argv[a->a_argc++] = w;
        return(0);
    }
}

static void scan_push(scan_t *scan, const int8 *s)
{
    if (scan->scan_tos < SCAN_DEPTH - 1) {
        scan->scan_stack[++scan->scan_tos] = s;
        scan->scan_ptr[scan->scan_tos] = s;
    }
}

static int32 scan_getc(scan_t *scan)
{
    int32  c;

    for (c = 0; scan->scan_tos >= 0; scan->scan_tos--)
        if ((c = *scan->scan_ptr[scan->scan_tos]++) != 0)
            break;

    return c;
}

/*
 * Function:    parse_args
 * Purpose:     Break up a command line into argv/argc format.
 * Parameters:  s - string to break up
 *              s_ret - updated to where processing finished (e.g. semicolon)
 *              a - argument structure to fill in.
 * Returns:     0 - success
 *              -1 failed (message printed)
 */
int32 diag_parse_args(const int8 *s, int8 **s_ret, args_t *a)
{
    int32   inDQ = FALSE;           /* True if in double quote */
    int32   inSQ = FALSE;           /* True if in single quote */
    int32   inW = FALSE;            /* True if in word */
    int8    *d;                     /* Destination of copy */
    int8    *e;                     /* End of buffer (last char) */
    int32   c;
    scan_t  scan;

    d = a->a_buffer;
    e = a->a_buffer + sizeof (a->a_buffer) - 1;

    a->a_argc = 0;                      /* Start at 0 please */
    a->a_arg  = 0;

    if (!s) {                           /* Handle NULL string */
        if (s_ret) {
            *s_ret = 0;
        }
        return 0;
    }

    scan_start(&scan);
    scan_push(&scan, s);

    while (1) {
        c = scan_getc(&scan);
        if ('\\' == c) {        /* Escape char */
            c = scan_getc(&scan);
            if (c == '\0') {
                printk("ERROR: Can't escape EOL\r\n");
                return(-1);
            } else {
                if (!inW) {
                    if (parse_start_word(a, d)) {
                        return(-1);
                    }
                    inW = TRUE;
                }
                if (d < e) {
                    *d++ = c;
                }
            }
        } else if (isspace(c) || (';' == c) || (0 == c)) {
            if (inDQ || inSQ) {         /* In quote - copy */
                if (c == 0) {
                    printk("ERROR: Command line ended "
                            "while in a quoted string\r\n");
                    return(-1);
                }
                if (d < e) {
                    *d++ = c;
                }
                continue;
            } else if (inW) {           /* In word - break */
                *d = 0;
                if (d < e) {
                    d++;
                }
                inW = FALSE;
            }
            if (';' == c || 0 == c) {
                /*
                 * If end of line or statement, then update return
                 * pointer and finish up
                 */
                break;
            }
        } else {
            /*
             * Nothing special, if not in a word, then start new arg.
             */
            if (!inW) {
                if (parse_start_word(a, d)) {
                    return(-1);
                }
                inW = TRUE;
            }
            if ('"' == c && !inSQ) {
                inDQ = !inDQ;
            } else if ('\'' == c && !inDQ) {
                inSQ = !inSQ;
            } else {
                if (d < e) {
                    *d++ = c;
                }
            }
        }
    }

    if (s_ret) {        /* Update s_ret if non-NULL */
        *s_ret = ((c != 0 && scan.scan_tos == 0) ?
                (int8 *) scan.scan_ptr[scan.scan_tos] :
                NULL);
    }

    return(0);
}
/*@api
 * parse_arg_dump
 *
 * @brief
 * displays the args.
 *
 * @param=args - pointer to args
 * @returns none
 *
 * @desc
 */
void parse_arg_dump(args_t *a)
{
    int32  i;

    printk("parse_arg_dump: a=%p argc=%d next=%d\r\n",
            (void *)a, a->a_argc, a->a_arg);

    for (i = 0; i < a->a_argc; i++)
        printk("parse_arg_dump: arg[%d] = <%s>\r\n", i, a->a_argv[i]);
}


/*@api
 * clean_up
 *
 * @brief
 * removes "backspace" in string
 *
 * @param=args - pointer to string
 * @returns none
 *
 * @desc
 */
static void clean_up(char *s)
{
    char *d;
    d = s;
    while (*s) {
        if (*s == 8) {
            /* adjust for backspace */
            s++;
            d--;
        } else {
            *d++ = *s++;
        }
    }
    *d = 0;
}

void setVlanBcm(u8 vlanMode)
{
//	//使能802.1q
//	mgmt_spi_wr8_ind(0x0b003400, 0xe3);
//		
//	//0\1\4\8 -->1
//	mgmt_spi_wr32_ind(0x0b000583, 0x22713);
//	mgmt_spi_wr16_ind(0x0b000581, 0x1);
//	mgmt_spi_wr8_ind(0x0b000580, 0x80);
	
	if(vlanMode == 0)
	{
		//0\8 -->2
		mgmt_spi_wr32_ind(0x0b000583, 0x20301);
		mgmt_spi_wr16_ind(0x0b000581, 0x2);
		mgmt_spi_wr8_ind(0x0b000580, 0x80);
		
		//1\8 -->3
		mgmt_spi_wr32_ind(0x0b000583, 0x20502);
		mgmt_spi_wr16_ind(0x0b000581, 0x3);
		mgmt_spi_wr8_ind(0x0b000580, 0x80);
	}
	else
	{
		//0\8 -->3
		mgmt_spi_wr32_ind(0x0b000583, 0x20301);
		mgmt_spi_wr16_ind(0x0b000581, 0x3);
		mgmt_spi_wr8_ind(0x0b000580, 0x80);
		
		//1\8 -->2
		mgmt_spi_wr32_ind(0x0b000583, 0x20502);
		mgmt_spi_wr16_ind(0x0b000581, 0x2);
		mgmt_spi_wr8_ind(0x0b000580, 0x80);			
	}	
}

/*@api
 * main
 *
 * @brief
 * Main function for handling CLI commands.
 *
 * @param=none
 * @returns 0
 *
 * @desc
 */
extern char ReceiveCommand[SHELL_CMD_SIZE];
char exsw_version[VERSION_STR_LEN];
char teststring_tx[]="read 0x0b002800 64";
char teststring_rx[]="read 0x0b002850 64"; //"read 0x0b002850 64";

char teststring_1[]="read 0x0a840026 16";
char teststring_2[]="read 0x0a840060 16";
char teststring_3[]="read 0x0a840042 16";
char teststring_4[]="read 0x0a840020 16";
char teststring_5[]="read 0x0a0f2600 16";
char teststring_6[]="read 0x090fffc0 16";

char vlanstring_create_1[]="vlan create 2";
char vlanstring_addport_1[]="vlan portadd 2 0 tag";
char vlanstring_addmcu_1[]="vlan portadd 2 8 tag";
char vlanstring_create_2[]="vlan create 3";
char vlanstring_addport_2[]="vlan portadd 3 1 tag";
char vlanstring_addmcu_2[]="vlan portadd 3 8 tag";

char vlan2string_create_1[]="vlan create 2";
char vlan2string_addport_1[]="vlan portadd 2 1 tag";
char vlan2string_addmcu_1[]="vlan portadd 2 8 tag";
char vlan2string_create_2[]="vlan create 3";
char vlan2string_addport_2[]="vlan portadd 3 0 tag";
char vlan2string_addmcu_2[]="vlan portadd 3 8 tag";


//test pcb 25MHZ,v4 pcb 16MHz
void ShellCmd_Task(void *p_arg)
{   
	args_t   a;
    int8   *c_next;
    cmd_result_t rv;
    mgmt_info_t *info = &info_g;	  
    int32 errno;
    int32 error;
	(void)p_arg;
	
	  BCM_551CS_LOW();
	  PLATFORM_OS_TimeDly(500);
	  BCM_551CS_HIGH();
	  PLATFORM_OS_TimeDly(1000);

	/* Open the SPI interface */
    errno = mgmt_open_spi(info);
    if (errno != 0) {
        printk("Could not open SPI interface\r\n");
        rv = CMD_SPI_OPEN_FAIL;
        return;
    }
		
		PLATFORM_OS_TimeDly(2000);
		mgmt_spi_wr16_ind(0x0a840026, 0x5);
		PLATFORM_OS_TimeDly(100);
		mgmt_spi_wr16_ind(0x0a840060, 0x305);
		PLATFORM_OS_TimeDly(100);
		mgmt_spi_wr16_ind(0x0a840042, 0x136);
		PLATFORM_OS_TimeDly(100);
		mgmt_spi_wr16_ind(0x0a840020, 0x8);
	
		//设置Port4为100Tx
		PLATFORM_OS_TimeDly(100);
		mgmt_spi_wr16_ind(0x0a0f2600, 0x0);
		
		//设置PORT 0 为主模式，用于br直连
		PLATFORM_OS_TimeDly(100);
		mgmt_spi_wr16_ind(0x090fffc0, 0x208);	

		//使能802.1q
		mgmt_spi_wr8_ind(0x0b003400, 0xe3);
			
		//0\1\4\8 -->1
		mgmt_spi_wr32_ind(0x0b000583, 0x22713);
		mgmt_spi_wr16_ind(0x0b000581, 0x1);
		mgmt_spi_wr8_ind(0x0b000580, 0x80);
		
		if(SysNetParam.Vlan == 0)
		{
			//0\8 -->2
			mgmt_spi_wr32_ind(0x0b000583, 0x20301);
			mgmt_spi_wr16_ind(0x0b000581, 0x2);
			mgmt_spi_wr8_ind(0x0b000580, 0x80);
			
			//1\8 -->3
			mgmt_spi_wr32_ind(0x0b000583, 0x20502);
			mgmt_spi_wr16_ind(0x0b000581, 0x3);
			mgmt_spi_wr8_ind(0x0b000580, 0x80);
		}
		else
		{
			//0\8 -->3
			mgmt_spi_wr32_ind(0x0b000583, 0x20301);
			mgmt_spi_wr16_ind(0x0b000581, 0x3);
			mgmt_spi_wr8_ind(0x0b000580, 0x80);
			
			//1\8 -->2
			mgmt_spi_wr32_ind(0x0b000583, 0x20502);
			mgmt_spi_wr16_ind(0x0b000581, 0x2);
			mgmt_spi_wr8_ind(0x0b000580, 0x80);			
		}

#if 0	
		if(SysNetParam.NetParam_IPAddress[3]%2)
		{
			printk(" set vlan V2V3\r\r\n");
			clean_up(vlanstring_create_1);
			if (diag_parse_args(vlanstring_create_1, &c_next, &a)) {    /* Parses up to ; or EOL */
				printk(" command failed\r\r\n");
			}
			rv=mgmt_cmd(&a);
			
			clean_up(vlanstring_addport_1);
			if (diag_parse_args(vlanstring_addport_1, &c_next, &a)) {    /* Parses up to ; or EOL */
				printk(" command failed\r\r\n");
			}
			rv=mgmt_cmd(&a);	

			clean_up(vlanstring_addmcu_1);
			if (diag_parse_args(vlanstring_addmcu_1, &c_next, &a)) {    /* Parses up to ; or EOL */
				printk(" command failed\r\r\n");
			}
			rv=mgmt_cmd(&a);
			PLATFORM_OS_TimeDly(100);		

			clean_up(vlanstring_create_2);
			if (diag_parse_args(vlanstring_create_2, &c_next, &a)) {    /* Parses up to ; or EOL */
				printk(" command failed\r\r\n");
			}
			rv=mgmt_cmd(&a);
			
			clean_up(vlanstring_addport_2);
			if (diag_parse_args(vlanstring_addport_2, &c_next, &a)) {    /* Parses up to ; or EOL */
				printk(" command failed\r\r\n");
			}
			rv=mgmt_cmd(&a);	

			clean_up(vlanstring_addmcu_2);
			if (diag_parse_args(vlanstring_addmcu_2, &c_next, &a)) {    /* Parses up to ; or EOL */
				printk(" command failed\r\r\n");
			}
			rv=mgmt_cmd(&a);
			PLATFORM_OS_TimeDly(100);	
			printk(" set vlan V2V3 finish\r\r\n");			
		}
		else
		{
			printk(" set vlan V3V2\r\r\n");
			clean_up(vlan2string_create_1);
			if (diag_parse_args(vlan2string_create_1, &c_next, &a)) {    /* Parses up to ; or EOL */
				printk(" command failed\r\r\n");
			}
			rv=mgmt_cmd(&a);
			
			clean_up(vlan2string_addport_1);
			if (diag_parse_args(vlan2string_addport_1, &c_next, &a)) {    /* Parses up to ; or EOL */
				printk(" command failed\r\r\n");
			}
			rv=mgmt_cmd(&a);	

			clean_up(vlan2string_addmcu_1);
			if (diag_parse_args(vlan2string_addmcu_1, &c_next, &a)) {    /* Parses up to ; or EOL */
				printk(" command failed\r\r\n");
			}
			rv=mgmt_cmd(&a);
			PLATFORM_OS_TimeDly(100);		

			clean_up(vlan2string_create_2);
			if (diag_parse_args(vlan2string_create_2, &c_next, &a)) {    /* Parses up to ; or EOL */
				printk(" command failed\r\r\n");
			}
			rv=mgmt_cmd(&a);
			
			clean_up(vlan2string_addport_2);
			if (diag_parse_args(vlan2string_addport_2, &c_next, &a)) {    /* Parses up to ; or EOL */
				printk(" command failed\r\r\n");
			}
			rv=mgmt_cmd(&a);	

			clean_up(vlan2string_addmcu_2);
			if (diag_parse_args(vlan2string_addmcu_2, &c_next, &a)) {    /* Parses up to ; or EOL */
				printk(" command failed\r\r\n");
			}
			rv=mgmt_cmd(&a);
			PLATFORM_OS_TimeDly(100);	
			printk(" set vlan V3V2 finish\r\r\n");				
		}
#endif

    while (1)
    {
		PLATFORM_OS_TimeDly(3000);
			
//		printk(" teststring_tx\r\r\n");
//		clean_up(teststring_tx);
//		if (diag_parse_args(teststring_tx, &c_next, &a)) {    /* Parses up to ; or EOL */
//            //printk(" command failed\r\r\n");
//        }

//        /* Execute the management command */
//        rv=mgmt_cmd(&a);
		
//		printk(" teststring_rx\r\r\n");
//		clean_up(teststring_rx);
//		if (diag_parse_args(teststring_rx, &c_next, &a)) {    /* Parses up to ; or EOL */
//            //printk(" command failed\r\r\n");
//        }

//        /* Execute the management command */
//        rv=mgmt_cmd(&a);
#if 0
				uint16 rd_val;

        errno = mgmt_fw_status_get(info, &rd_val);
        if (errno) {
            printk("mgmt_fw_status_get failed, errno = %d\r\n", errno);
            rv = CMD_FAIL;
        } else {
            printk("Firmware status: 0x%04x\r\n", rd_val);
        }
#endif
		
#if 0	
		printk(" teststring_1\r\r\n");
		clean_up(teststring_1);
		if (diag_parse_args(teststring_1, &c_next, &a)) {    /* Parses up to ; or EOL */
            printk(" command failed\r\r\n");
        }

        /* Execute the management command */
        rv=mgmt_cmd(&a);
		PLATFORM_OS_TimeDly(500);
				printk(" teststring_2\r\r\n");
		clean_up(teststring_2);
		if (diag_parse_args(teststring_2, &c_next, &a)) {    /* Parses up to ; or EOL */
            printk(" command failed\r\r\n");
        }

        /* Execute the management command */
        rv=mgmt_cmd(&a);
		PLATFORM_OS_TimeDly(500);
				printk(" teststring_3\r\r\n");
		clean_up(teststring_3);
		if (diag_parse_args(teststring_3, &c_next, &a)) {    /* Parses up to ; or EOL */
            printk(" command failed\r\r\n");
        }

        /* Execute the management command */
        rv=mgmt_cmd(&a);
		PLATFORM_OS_TimeDly(500);
				printk(" teststring_4\r\r\n");
		clean_up(teststring_4);
		if (diag_parse_args(teststring_4, &c_next, &a)) {    /* Parses up to ; or EOL */
            printk(" command failed\r\r\n");
        }

        /* Execute the management command */
        rv=mgmt_cmd(&a);
		PLATFORM_OS_TimeDly(500);
				printk(" teststring_5\r\r\n");
		clean_up(teststring_5);
		if (diag_parse_args(teststring_5, &c_next, &a)) {    /* Parses up to ; or EOL */
            printk(" command failed\r\r\n");
        }

        /* Execute the management command */
        rv=mgmt_cmd(&a);
		PLATFORM_OS_TimeDly(500);
				printk(" teststring_6\r\r\n");
		clean_up(teststring_6);
		if (diag_parse_args(teststring_6, &c_next, &a)) {    /* Parses up to ; or EOL */
            printk(" command failed\r\r\n");
        }

        /* Execute the management command */
        rv=mgmt_cmd(&a);
		PLATFORM_OS_TimeDly(500);
		
		printk(" teststring_tx\r\r\n");
		clean_up(teststring_tx);
		if (diag_parse_args(teststring_tx, &c_next, &a)) {    /* Parses up to ; or EOL */
            printk(" command failed\r\r\n");
        }

        /* Execute the management command */
        rv=mgmt_cmd(&a);
		PLATFORM_OS_TimeDly(3000);
		
		printk(" teststring_rx\r\r\n");
		clean_up(teststring_rx);
		if (diag_parse_args(teststring_rx, &c_next, &a)) {    /* Parses up to ; or EOL */
            printk(" command failed\r\r\n");
        }

        /* Execute the management command */
        rv=mgmt_cmd(&a);
		PLATFORM_OS_TimeDly(3000);
#endif
	
#if 0			
        uint32 dev=0;
        uint16 rev=0;
				
        errno = mgmt_get_chip_info(&rev, &dev);
        if (errno){
            printk("could not get chip info, errno = %d\r\r\n", errno);
        }
        (void)printk("MCU: %x %x\r\r\n", dev, rev);
				//PLATFORM_OS_TimeDly(1000);

        errno = mgmt_os_version_get(info, exsw_version);
        if (errno){
            printk("could not get OS version, errno = %d\r\r\n", errno);
        }
				else
            printk("OS Version: %s\r\n", exsw_version);

        errno = mgmt_avb_version_get(info, exsw_version);
        if (errno){
            printk("could not get avb version, errno = %d\r\r\n", errno);
        }
				else
            printk("AVB Version: %s\r\n", exsw_version);
				
			  PLATFORM_OS_TimeDly(1000);
#endif
			
#if 0
		RUN_TRACE("   \r\r\n");
		RUN_SHELL_PROMPT();

		GetInputString(ReceiveCommand);                         // 收不到字符将阻塞
        //input_Command_Analyse(s_cmdMatch, ReceiveCommand, MAX_COMMAND_NUM); // 命令解析
		
		clean_up(ReceiveCommand);
		if (diag_parse_args(ReceiveCommand, &c_next, &a)) {    /* Parses up to ; or EOL */
            printk(" command failed\r\r\n");
        }

        /* Execute the management command */
        rv = mgmt_cmd(&a);
        if (rv == CMD_USAGE){
            printk(mgmt_cmd_usage);
            printk(mgmt_switch_cmd_usage);
            printk(mgmt_port_cmd_usage);
            printk(mgmt_avnu_cmd_usage);
            printk(mgmt_vlan_cmd_usage);
            printk(mgmt_msrp_cmd_usage);
            printk(mgmt_acd_cmd_usage);
            printk(mgmt_heartbeat_cmd_usage);
            printk(mgmt_config_cmd_usage);
            printk(mgmt_log_cmd_usage);
        }else if( rv == CMD_USAGE_FLASH){
            printk(mgmt_cmd_usage);
        }else if (rv == CMD_USAGE_SWITCH){
            printk(mgmt_switch_cmd_usage);
        }else if (rv == CMD_USAGE_PORT){
            printk(mgmt_port_cmd_usage);
        }else if (rv == CMD_USAGE_VLAN){
            printk(mgmt_vlan_cmd_usage);
        }else if (rv == CMD_USAGE_MSRP){
            printk(mgmt_msrp_cmd_usage);
        }else if (rv == CMD_USAGE_AVNU){
            printk(mgmt_avnu_cmd_usage);
        }else if (rv == CMD_USAGE_ACD) {
            printk(mgmt_acd_cmd_usage);
        }else if(rv == CMD_USAGE_HEARTBEAT) {
            printk(mgmt_heartbeat_cmd_usage);
        }else if(rv == CMD_USAGE_CONFIG) {
            printk(mgmt_config_cmd_usage);
        }else if(rv == CMD_USAGE_LOG) {
            printk(mgmt_log_cmd_usage);
        }
#endif

#if 0
//        uint32 dev=0;
//        uint16 rev=0;
//				
//        errno = mgmt_get_chip_info(&rev, &dev);
//        if (errno){
//            printk("could not get chip info, errno = %d\r\r\n", errno);
//        }
        (void)printk("0x55555555\r\r\n");
		PLATFORM_OS_TimeDly(3000);
#endif
    }
}

#endif
