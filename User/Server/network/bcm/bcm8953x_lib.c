/*
*********************************************************************************************************
*	                                            DoubleWires
*                           Commuication Controller For The Mining Industry
*
*                                  (c) Copyright 1994-2013  HNDZ 
*                       All rights reserved.  Protected by international copyright laws.
*
*
*    File    : bcm8953x_lib.c
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
#include "mgmt.h"
#include "bcm89530_rdb.h"
#include "bcm8953xlib.h"

#if (SERVER_NETWORK_BCM_EN==1)

/******************************************************************************/

#define READ_POWER_STATE_FROM_APS    (0)
#define DEFAULT_DEVICE_ID            (0)
#define DEBUG                        (0)

#define TCM_BL_MAX_SZ               (32UL * 1024UL)
#define TCM_BL_ADDR                 (TCM_START_ADDR + TCM_SIZE - TCM_BL_MAX_SZ)
#define TCM_RESV_SZ                 (128)
#define TCM_DL_ADDR                 (TCM_START_ADDR + TCM_RESV_SZ)
#define OPCODE_BRANCH_BL_RESV       (0xEAUL << 24UL | ((TCM_BL_ADDR)/4UL - 2UL))
#define TCM_MAX_DL_SZ               (TCM_SIZE - TCM_BL_MAX_SZ - TCM_RESV_SZ)

#define DWNLD_HDR_SZ    (20)
#define BROM_HDR_SZ     (20)

#define BOOT_MODE_SPARE_REG         (MISC_SPARE_SW_REG0)
#define DWNLD_TARGET_SPARE_REG      (MISC_SPARE_SW_REG1)
#define DWNLD_HOST_SPARE_REG        (MISC_SPARE_SW_REG2)
#define BOOT_INFO_SPARE_REG         (MISC_SPARE_SW_REG3)
#define F1CFG_REG0                  (MISC_SPARE_SW_REG4)
#define F1CFG_REG1                  (MISC_SPARE_SW_REG5)
#define F1CFG_REG2                  (MISC_SPARE_SW_REG6)

#define MGMT_MODE_BOOT_SPI          (0x1)
#define MGMT_MODE_ONE_SHOT          (0x2)
#define MGMT_MODE_INCR_DL           (0x3)
#define MGMT_MODE_ERASE             (0x4)
#define MGMT_MODE_READ              (0x5)
#define MGMT_MODE_UNKNOWN           (0xFF)

/* BOOT_MODE */
#define DBG_SCRATCH_RAMDUMP_MODE_MASK   (0x80000000UL)
#define DBG_SCRATCH_SW_RESET_MASK       (0x1UL)
#define DBG_SCRATCH_WD_RESET_MASK       (0x2UL)
#define DBG_SCRATCH_DWNLD_MODE_MASK     (0x1CUL)
#define DBG_SCRATCH_DWNLD_MODE_SHIFT    (2UL)
#define DBG_SCRATCH_DWNLD_MODE_TFTP     (0x1UL)
#define DBG_SCRATCH_DWNLD_MODE_RAM      (0x2UL)
#define DBG_SCRATCH_DWNLD_MODE_PARTIAL_MASK     (0x20UL) /*< 0: virgin download mode 1: partial download mode */

/* BOOT STATUS */
#define BOOT_FW_IMG_NUM_MASK        (0x000FUL)
#define BOOT_FW_IMG_NUM_SHIFT       (0UL)
#define BOOT_FW_IMG_VER_MATCH_MASK  (0x10UL)
#define BOOT_FW_IMG_VER_MATCH_SHIFT (4UL)

/* F1CFG_REG0 */
#define F1CFG_REG0_SKIP_MASK        (0x0001UL)
#define F1CFG_REG0_SKIP_SHIFT       (0UL)
#define F1CFG_REG0_PARITY_MASK      (0x0002UL)
#define F1CFG_REG0_PARITY_SHIFT     (1UL)

/******************************************************************************/

uint16_t mgmtavbCfgxIntfNum[AVB_MAX_INTERFACES] = { 0, 1, 2, 3, 4, 5, 6, 8 };

#if 0
static int32 mgmt_spi_wr8(uint32 addr, uint8 data);
#endif
int32 mgmt_spi_wr16(uint32 addr, uint16 data);
int32 mgmt_spi_wr32(uint32 addr, uint32 data);
#if 0
static int32 mgmt_spi_wr64(uint32 addr, uint64 data);
static uint32 mgmt_spi_rd8(uint32 addr, uint8 *rd_val);
#endif
int32 mgmt_spi_rd16(uint32 addr, uint16 *rd_val);
int32 mgmt_spi_rd32(uint32 addr, uint32 *rd_val);
#if 0
static uint32 mgmt_spi_rd64(uint32 addr, uint64 *rd_val);
#endif

int32 mgmt_spi_wr8_ind(uint32 addr, uint8 data);
int32 mgmt_spi_wr16_ind(uint32 addr, uint16 data);
int32 mgmt_spi_wr32_ind(uint32 addr, uint32 data);
int32 mgmt_spi_wr64_ind(uint32 addr, uint64 data);
static int32 mgmt_spi_rd8_ind(uint32 addr, uint8 *rd_val);
int32 mgmt_spi_rd16_ind(uint32 addr, uint16 *rd_val);
static int32 mgmt_spi_rd32_ind(uint32 addr, uint32 *rd_val);
static int32 mgmt_spi_rd64_ind(uint32 addr, uint64 *rd_val);
static int32 speed_up_reset_target(uint32_t halt);

extern MGMT_AVB_RC_t mgmt_avb_port_next_get(MGMT_AVB_BOOL_t isFirst,
                                            uint32 bitmap, uint32 *port);

extern uint64_t get_time_ns(void);

/******************************************************************************/

static SPI_ID_t g_spi_id = SPI_ID_2;

void acd_write16(uint32 addr, uint16 data)
{
    mgmt_spi_wr16_ind(addr, data);
}

uint16 acd_read16(uint32 addr)
{
    uint16 rd_val;

    mgmt_spi_rd16_ind(addr, &rd_val);

    return rd_val;
}

#if 0
static int32 mgmt_spi_wr64_cont(uint32 addr, uint8 *buf, uint32 len);
static int32 mgmt_spi_rd64_cont(uint8 *buf, uint32 addr, uint32 len);
#endif
static int32 mgmt_spi_wr32_bulk(uint32 addr, uint8 *buf, uint32 len, uint32 swap);
static int32 mgmt_spi_rd32_bulk(uint8 *buf, uint32 addr, uint32 len, uint32 swap);
uint32 hex2uint(const char *h);

static void swap_str(char *dst, char *src, int len)
{
	uint32_t *s,*d;
	int loop ;
	
    if ((src == NULL) || (dst == NULL)) {
        printf("%s src/dst is NULL\r\n", __func__);
        return;
    }
    loop = (len + sizeof(uint32_t) - 1) / sizeof(uint32_t);
    s = (uint32_t *)src;
    d = (uint32_t *)dst;
    while (loop--) {
        *d = uswap32(*s);
        s++;
        d++;
    }
}

/*@api
 * mgmt_vpd_data_validate
 *
 * @brief
 * check allowable characters in vpd key.
 *
 * @param=data - pointer to vpd key string
 *
 * @returns MGMT_VPD_SUCCESS/MGMT_VPD_FAILURE
 *
 * @desc
 */

MGMT_VPD_RV_t mgmt_vpd_data_validate(char *data)
{
    int i;
    for ( i = 0; data[i] != '\0'; i++) {
        if(MGMT_VPD_DATA_CHAR_SET(data[i])) {
            continue;
        } else {
            return MGMT_VPD_FAILURE;
        }
    }
    return MGMT_VPD_SUCCESS;
}

/*@api
 * mgmt_vpd_key_validate
 *
 * @brief
 * check allowable characters in vpd key.
 *
 * @param=key - pointer to vpd key string
 *
 * @returns MGMT_VPD_SUCCESS/MGMT_VPD_FAILURE
 *
 * @desc
 */

MGMT_VPD_RV_t mgmt_vpd_key_validate(char *key)
{
    int i;
    for ( i = 0; key[i] != '\0'; i++) {
        if(MGMT_VPD_KEY_CHAR_SET(key[i])) {
            continue;
        }
        else {
            return MGMT_VPD_FAILURE;
        }
    }
    return MGMT_VPD_SUCCESS;
}


/*@api
 * mgmt_dump_buffer
 *
 * @brief
 * Print the contents of a buffer to stdout. For debugging.
 *
 * @param=name - name of the buffer
 * @param=data - pointer to the data
 * @param=len - length of the data
 * @returns void
 *
 * @desc
 */
void mgmt_dump_buffer(int8 *name, uint8 *data, int32 len)
{
    int32 i;
    printk("%s: %d bytes", name, len);
    for (i = 0; i < len; ++i) {
        printk("%c%02x", (i % 16) ? ' ' : '\n', data[i] & 0xff);
    }
    printk("\r\n");
}


/*@api
 * mgmt_convert_mac
 *
 * @brief
 * Print the contents of a buffer to stdout. For debugging.
 *
 * @param=mac_addr - pointer to mac address string
 * @param=data - pointer to the mac address hex
 * @param=status - status of the operation
 *
 * @returns void
 *
 * @desc
 */
void mgmt_convert_mac(int8 *mac_addr, uint8 *mac_hex, int32 *status)
{
  uint8 i;
  uint32 octet;
  int8 *ptr = mac_addr;

  *status = -1;

  if (ptr == NULL) {
    printk("%s :: mac_addr ptr is null\r\n", __FUNCTION__);
    return ;
  }

  if (mac_hex == NULL) {
    printk("%s :: mac_hex ptr is null\r\n", __FUNCTION__);
    return ;
  }

  if (strlen(mac_addr) != 17) {
      printf("Macaddr length mismatch, should be of the format xx:xx:xx:xx:xx:xx\r\n");
      return;
  }

  for (i = 0; i < 17; i++) {
      if ((i % 3) == 2) {
          if (mac_addr[i] != ':') {
              printf("Invalid characters in macaddr input\r\n");
              return;
          }
      } else {
          if ((mac_addr[i] < '0')
              || ((mac_addr[i] > '9') && (mac_addr[i] < 'A'))
              || ((mac_addr[i] > 'F') && (mac_addr[i] < 'a'))
              || (mac_addr[i] > 'f')) {
              printf("Invalid characters in macaddr input\r\n");
              return;
          }
      }
  }

  for (i = 0; i < MGMT_ETHER_ADDR_LEN; ++i) {
     if (!i) {
       octet = strtol(mac_addr, &ptr, 16);
     } else {
       octet = strtol(ptr+1, &ptr, 16);
     }

     if (i == (MGMT_ETHER_ADDR_LEN - 1)) {
       if ((*ptr != '\0') || (octet > 255)) {
         printk("%s :: Invalid mac address format\r\n", __FUNCTION__);
         return;
       }
     } else {
       if ((*ptr != ':') || (octet > 255)) {
         printk("%s :: Invalid macaddress\r\n", __FUNCTION__);
         return;
       }
     }

    mac_hex[i] = octet;
  }

  if ((mac_hex[0] == 0) && (mac_hex[1] == 0) && (mac_hex[2] == 0) &&
      (mac_hex[3] == 0) && (mac_hex[4] == 0) && (mac_hex[5] == 0)) {
    printk("\nInvalid MAC address\r\n");
    return;   /* Invalid mac address */
  }

  if ((mac_hex[0] == 0xff) && (mac_hex[1] == 0xff) && (mac_hex[2] == 0xff) &&
      (mac_hex[3] == 0xff) && (mac_hex[4] == 0xff) && (mac_hex[5] == 0xff)) {
      printk("\nBroadcast MAC address\r\n");
    return;   /* Broad cast mac address */
  }

  *status = 0;
}


/*@api
 * buffer_from_file
 *
 * @brief
 * Create a buffer holding the contents of a file.
 *
 * @param=name - file to read
 * @param=len - pointer to length to return
 * @returns pointer to the data, or NULL
 *
 * @desc
 */
void * buffer_from_file(int8 *name, int32 *len)
{
    FILE *fp;
    int32 alloc_len, read_len;
    void *buffer;
    fp = sal_fopen(name, "r");
    if (!fp) {
#if DEBUG
        printk("could not open %s\r\n", name);
#endif
        return NULL;
    }
    if (fseek(fp, 0, SEEK_END) < 0) {
#if DEBUG
        printk("error seeking to end of %s\r\n", name);
#endif
        sal_fclose(fp);
        return NULL;
    }

    *len = ftell(fp);
    if(*len < 0 ) {
#if DEBUG
        printk("error in getting the current position\r\n");
#endif
        sal_fclose(fp);
        return NULL;
    }
    if (fseek(fp, 0, SEEK_SET) < 0) {
#if DEBUG
        printk("error seeking to beginning of %s\r\n", name);
#endif
        sal_fclose(fp);
        return NULL;
    }

    alloc_len = ROUND_UP(*len, MGMT_MEMORY_PAGE_SIZE);
    buffer = sal_alloc(alloc_len, "Polar Host");
    if (!buffer) {
#if DEBUG
        printk("could not allocate %d bytes\r\n", *len);
#endif
        fclose(fp);
        return NULL;
    }

    read_len = fread(buffer, 1, *len, fp);
    sal_fclose(fp);
    if (read_len != (int32)*len) {
#if DEBUG
        printk("error reading %s\r\n", name);
#endif
        sal_free(buffer);
        return NULL;
    }

    return buffer;
}

/*
 * @api
 * mgmt_initialize_connection
 *
 * @brief
 * Initialize the management connection to the BCM8953x
 *
 * @param=info - pointer to the mgmt_info_t
 *
 * @returns 0 - success
 * @returns !0 - error
 */
static int32 mgmt_initialize_connection(mgmt_info_t *info)
{
#if 0
Todo: This expects that mgmt interface exist => SWSPARE14 & 15 should already be
          populated but this is not possible incase of first flash; moreover it is
          unnecessary for current implementation which does not expect ARM to intervene
          while programming flash memory
              int32 errno;
      mgmt_reply_version_t version;

      errno = mgmt_get_version(info, &version);
      if (errno) {
#if DEBUG
          printk("could not get version info, errno = %d\r\n", errno);
#endif
          return errno;
      }

#if DEBUG > 2
      printk("Model = 0x%08x\r\n", version.model_id);
#endif
      switch (version.model_id & ~0xf) {
          case 0x89530:
              break;

          default:
              break;
      }
#endif
      return 0;
}


/*@api
 * mgmt_open_spi
 *
 * @brief
 * Initialize a connection to the Broadcom Polar
 *
 * @param=info - pointer to mgmt_info_t to use for the connection
 * @returns 0 - success
 * @returns !0 - error
 *
 * @desc
 */
int32 mgmt_open_spi(mgmt_info_t *info)
{
    int32 errno = 0;

    if (info == NULL) {
      printk("%s :: mgmt_info_t pointer is NULL\r\n", __FUNCTION__);
      return ERR_PARAM;
    }

    sal_memset(info, 0, sizeof(mgmt_info_t));

    info->connection_type = MGMT_SPI_CONNECTION;

    /* Initialize the connection */
    errno = mgmt_initialize_connection(info);
    return errno;
}


/*@api
 * mgmt_spi_send
 *
 * @brief
 * Encode and send bytes out the SPI interface.
 *
 * @param=info - pointer to the mgmt_info_t
 * @param=out - pointer to the data to send
 * @param=len - length of the data.
 *
 * @returns 0 - success
 * @returns !0 - error
 *
 */
static int32 mgmt_spi_send(mgmt_info_t *info, uint8 *data, uint32 datalen)
{
    uint32 i;
    int32 errno = -1;
    uint8 chksum;
    uint32 dst;
    uint16 sw_spare14;
    uint32 retry;
    mgmt_command_t *cmd = (mgmt_command_t *) data;

    if (info == NULL) {
      printk("%s :: mgmt_info_t pointer is NULL\r\n", __FUNCTION__);
      return errno;
    }

    if (data == NULL) {
       printk("%s :: data is NULL\r\n", __FUNCTION__);
       return errno;
     }

    cmd->chksum = chksum = 0;
    /* Compute the checksum */
    for (i = 0; i < datalen; ++i) {
        chksum += data[i];
    }
    /* negate the checksum and install */
    chksum = -chksum;
    cmd->chksum = chksum;

#if DEBUG > 2
    mgmt_dump_buffer("spi send plaintext", data, datalen);
#endif

    retry = MGMT_SPI_SEND_MAX_RETRIES_SEC * MSEC_PER_SEC;
    do {
        /* Check shared registers to know the status of slave
           and also the location where the msg has to be stored */
        errno = mgmt_spi_rd16(MISC_SPARE_SW_REG14, &sw_spare14);
        if (errno) {
            printk("failed to read shared register, err=0x%x\r\n", errno);
            goto err_read;
        }
        if (!sw_spare14 || (sw_spare14 & MSG_QUEUED)) {
            PLATFORM_OS_TimeDly(1);
            continue;
        } else {
            break;
        }
    } while (--retry);

    if (!retry) {
        printk("Timed out, target might be inactive, sw_spare14=0x%x\r\n", sw_spare14);
        goto err_timeout;
    }
    dst = (sw_spare14 & MSG_BUFFER) << MSG_BUFFER_SHIFT;
    /* Write 64bit at a time. Since cmd buffer has 16byte padding,
       it is safe to ceil datalen to 64bit */
    errno = mgmt_spi_wr32_bulk(dst, data, CEIL_64(datalen), 0);

    if (errno) {
        printk("Failed to send cmd buffer! err=0x%x\r\n", errno);
        goto err_copy;
    } else {
        uint32 reg_val;
        mgmt_spi_wr16(MISC_SPARE_SW_REG14, sw_spare14 | MSG_QUEUED);
        mgmt_spi_rd32(CFG_CPU_INTR_MASK, &reg_val);
        if (!(reg_val & (1 << 7))) {
            printk("Failure:Mask - Not set,INTR will be set to host besides ARM\r\n");
           goto err_cfg;
        }
        /*INTR 7 is used for communication from host to ARM*/
        mgmt_spi_rd32(CFG_CPU_INTR_FORCE, &reg_val);
        mgmt_spi_wr32(CFG_CPU_INTR_FORCE, (reg_val|(1<<7)));
    }
    errno = 0;
err_cfg:
err_copy:
err_timeout:
err_read:
    return errno;
}

/* In multiples of 32bit and assumes that buf[] is so aligned & accessible */
static int32 mgmt_spi_wr32_bulk(uint32 addr, uint8 *buf, uint32 len, uint32 swap)
{
    uint32 *src = (uint32*)buf;
    int i;
    int end = addr + len;

#if DEBUG
    printk("mgmt_spi_wr32_bulk dest:0x%x len:%d swap:%u\r\n", addr, len, swap);
#endif
    if (swap) {
        for (i = addr; i < end; i = i + sizeof(uint32)) {
            mgmt_spi_wr32_ind(i, uswap32(*src));
            src++;
        }
    } else {
        for (i = addr; i < end; i = i + sizeof(uint32)) {
            mgmt_spi_wr32_ind(i, *src);
            src++;
        }
    }
    return 0;
}


/* In multiples of 32bit and assumes that buf[] is so aligned & accessible */
static int32 mgmt_spi_rd32_bulk(uint8 *buf, uint32 addr, uint32 len, uint32 swap)
{
    uint32 *data = (uint32*)buf;
    int i;
    int end = addr + len;
    uint32 temp;

#if DEBUG
    printk("mgmt_spi_rd32_bulk src:0x%x len:%d swap:%u\r\n", addr, len, swap);
#endif

    if (swap) {
        for (i = addr; i < end; i = i + sizeof(uint32)) {
            mgmt_spi_rd32_ind(i, &temp);
            *data = uswap32(temp);
            data++;
        }
    } else {
        for (i = addr; i < end; i = i + sizeof(uint32)) {
            mgmt_spi_rd32_ind(i, data);
            data++;
        }
    }
    return 0;
}

#if 0
/* In multiples of 64bit and assumes that buf[] is so aligned & accessible */
static int32 mgmt_spi_wr64_cont(uint32 addr, uint8 *buf, uint32 len)
{
    uint32 rd_val;
    uint32 wait;
    uint32 *data = (uint32*)buf;
    int i;
    int ret = -1;;

#if DEBUG
    printk("mgmt_spi_wr64_cont dest:0x%x len:%d\r\n", addr, len);
#endif
    mgmt_spi_wr32(IND_ACC_RDB_IND_REGS_ADDR_SER_L16, addr);
    mgmt_spi_wr16(IND_ACC_RDB_IND_REGS_CTRL_SER_L16, (IND_ACC_COMMIT_ON_RDWR
                | IND_ACC_AUTO_INC | IND_ACC_WR | IND_ACC_TX_SZ_64));
    for (i = 0; i < len; i = i + 8) {
        uint32_t d_l = *data++;
        uint32_t d_h = *data++;
        mgmt_spi_wr32(IND_ACC_RDB_IND_REGS_DATA_SER_H_L16, d_h);
        mgmt_spi_wr32(IND_ACC_RDB_IND_REGS_DATA_SER_L_L16, d_l);
        wait = IND_ACC_RETRY;
        do {
            mgmt_spi_rd32(IND_ACC_RDB_IND_REGS_CTRL_SER_L16, &rd_val);
        } while (--wait && !(rd_val & IND_ACC_DONE));
        if (!wait) {
            printk("Ind. Wr. timed out ctrl 0x%x\r\n", rd_val);
            goto done;
        }
    }
    ret = 0;
done:
    mgmt_spi_wr16(IND_ACC_RDB_IND_REGS_CTRL_SER_L16, 0);
    return ret;
}

/* In multiples of 64bit and assumes that buf[] is so aligned & accessible */
static int32 mgmt_spi_rd64_cont(uint8 *buf, uint32 addr, uint32 len)
{
    uint32 rd_val;
    uint32 wait;
    uint32 *data = (uint32*)buf;
    int i;
    int32 ret;

#if DEBUG
    printk("mgmt_spi_rd64_cont src:0x%x len:%d\r\n", addr, len);
#endif
    mgmt_spi_wr32(IND_ACC_RDB_IND_REGS_ADDR_SER_L16, addr);
    mgmt_spi_wr16(IND_ACC_RDB_IND_REGS_CTRL_SER_L16, (IND_ACC_COMMIT_ON_RDWR
                | IND_ACC_AUTO_INC | IND_ACC_RD | IND_ACC_TX_SZ_64));
    /* Read transaction starts only once we read the LS bits. Read & Ignore */
    mgmt_spi_rd32(IND_ACC_RDB_IND_REGS_DATA_SER_L_L16, &rd_val);

    for (i = 0; i < len; i = i + 8) {
        uint32_t d_l;
        uint32_t d_h;
        wait = IND_ACC_RETRY;
        do {
            mgmt_spi_rd32(IND_ACC_RDB_IND_REGS_CTRL_SER_L16, &rd_val);
        } while (--wait && !(rd_val & IND_ACC_DONE));
        if (!wait) {
            printk("Ind. Rd. timed out ctrl 0x%x\r\n", rd_val);
            goto done;
        }
        mgmt_spi_rd32(IND_ACC_RDB_IND_REGS_DATA_SER_H_L16, &d_h);
        mgmt_spi_rd32(IND_ACC_RDB_IND_REGS_DATA_SER_L_L16, &d_l);
        *data++ = d_l;
        *data++ = d_h;
    }
    ret = 0;
done:
    mgmt_spi_wr16(IND_ACC_RDB_IND_REGS_CTRL_SER_L16, 0);
    return ret;
}

static int32 mgmt_spi_wr8(uint32 addr, uint8 data)
{
    uint8 buf[MAX_BUF_SZ];
    int32 len = 0;
    //opcode
    buf[len++] = SPI_OPCODE_PHYADDR(g_spi_id) | SPI_OPCODE_WR | SPI_OPCODE_NO_INC | SPI_OPCODE_RD_WAIT_0 | SPI_OPCODE_TX_SZ_8;
    buf[len++] = (addr >> 24) & 0xff;
    buf[len++] = (addr >> 16) & 0xff;
    buf[len++] = (addr >> 8) & 0xff;
    buf[len++] = (addr & 0xff);
    buf[len++] = data;
    aps_spi_write(DEFAULT_DEVICE_ID, &buf[0], len);
    return 0;
}
#endif

int32 mgmt_spi_wr16(uint32 addr, uint16 data)
{
    uint8 buf[MAX_BUF_SZ];
    int32 len = 0;
    //opcode
    buf[len++] = SPI_OPCODE_PHYADDR(g_spi_id) | SPI_OPCODE_WR | SPI_OPCODE_NO_INC | SPI_OPCODE_RD_WAIT_0 | SPI_OPCODE_TX_SZ_16;
    buf[len++] = (addr >> 24) & 0xff;
    buf[len++] = (addr >> 16) & 0xff;
    buf[len++] = (addr >> 8) & 0xff;
    buf[len++] = (addr & 0xff);
    buf[len++] = (data >> 8) & 0xff;
    buf[len++] = (data & 0xff);
    aps_spi_write(DEFAULT_DEVICE_ID, &buf[0], len);
    return 0;
}

int32 mgmt_spi_wr32(uint32 addr, uint32 data)
{
    uint8 buf[MAX_BUF_SZ];
    int32 len = 0;
    //opcode
    buf[len++] = SPI_OPCODE_PHYADDR(g_spi_id) | SPI_OPCODE_WR | SPI_OPCODE_NO_INC | SPI_OPCODE_RD_WAIT_0 | SPI_OPCODE_TX_SZ_32;
    buf[len++] = (addr >> 24) & 0xff;
    buf[len++] = (addr >> 16) & 0xff;
    buf[len++] = (addr >> 8) & 0xff;
    buf[len++] = (addr & 0xff);
    buf[len++] = (data >> 24) & 0xff;
    buf[len++] = (data >> 16) & 0xff;
    buf[len++] = (data >> 8) & 0xff;
    buf[len++] = (data & 0xff);
    aps_spi_write(DEFAULT_DEVICE_ID, &buf[0], len);
    return 0;
}

#if 0
static int32 mgmt_spi_wr64(uint32 addr, uint64 data)
{
    uint8 buf[MAX_BUF_SZ];
    int32 len = 0;
    //opcode
    buf[len++] = SPI_OPCODE_PHYADDR(g_spi_id) | SPI_OPCODE_WR | SPI_OPCODE_NO_INC | SPI_OPCODE_RD_WAIT_0 | SPI_OPCODE_TX_SZ_64;
    buf[len++] = (addr >> 24) & 0xff;
    buf[len++] = (addr >> 16) & 0xff;
    buf[len++] = (addr >> 8) & 0xff;
    buf[len++] = (addr & 0xff);
    buf[len++] = (data >> 56) & 0xff;
    buf[len++] = (data >> 48) & 0xff;
    buf[len++] = (data >> 40) & 0xff;
    buf[len++] = (data >> 32) & 0xff;
    buf[len++] = (data >> 24) & 0xff;
    buf[len++] = (data >> 16) & 0xff;
    buf[len++] = (data >> 8) & 0xff;
    buf[len++] = (data & 0xff);
    aps_spi_write(DEFAULT_DEVICE_ID, &buf[0], len);
    return 0;
}

static uint32 mgmt_spi_rd8(uint32 addr, uint8 *rd_val)
{
    uint8 buf[MAX_BUF_SZ];
    int32 len = 0;
    int i, errno;
    //opcode
    buf[len++] = SPI_OPCODE_PHYADDR(g_spi_id) | SPI_OPCODE_RD | SPI_OPCODE_NO_INC | SPI_OPCODE_RD_WAIT_2 | SPI_OPCODE_TX_SZ_8;  //--WAIT_6
    buf[len++] = (addr >> 24) & 0xff;
    buf[len++] = (addr >> 16) & 0xff;
    buf[len++] = (addr >> 8) & 0xff;
    buf[len++] = (addr & 0xff);
    /* wait states as per opcode */
    for (i =0; i < ((buf[0] & SPI_OPCODE_RD_WAIT_MASK) >> SPI_OPCODE_RD_WAIT_SHIFT) * 2; i++)
        buf[len++] = 0x0;
    errno = (uint16)aps_spi_read(DEFAULT_DEVICE_ID, &buf[0], len, (uint8*)rd_val, sizeof(uint8));
    if (errno) {
        printk("aps_spi_read failed errno=%d\r\n", errno);
        return -1;
    }
    return 0;
}
#endif

int32 mgmt_spi_rd16(uint32 addr, uint16 *rd_val)
{
    uint8 buf[MAX_BUF_SZ];
	  uint8 recvbuf[MAX_BUF_SZ];
    int32 len = 0;
    int i, errno;
    //opcode
    buf[len++] = SPI_OPCODE_PHYADDR(g_spi_id) | SPI_OPCODE_RD | SPI_OPCODE_NO_INC | SPI_OPCODE_RD_WAIT_2 | SPI_OPCODE_TX_SZ_16;  //--WAIT_6
    buf[len++] = (addr >> 24) & 0xff;
    buf[len++] = (addr >> 16) & 0xff;
    buf[len++] = (addr >> 8) & 0xff;
    buf[len++] = (addr & 0xff);
    /* wait states as per opcode */
    for (i =0; i < ((buf[0] & SPI_OPCODE_RD_WAIT_MASK) >> SPI_OPCODE_RD_WAIT_SHIFT) * 2; i++)
        buf[len++] = 0x0;
    //errno = aps_spi_read(DEFAULT_DEVICE_ID, &buf[0], len, (uint8*)rd_val, sizeof(uint16));
	  errno = aps_spi_read(DEFAULT_DEVICE_ID, &buf[0], len, (uint8*)recvbuf, sizeof(uint16));
    if (errno) {
        printk("aps_spi_read failed errno=%d\r\n", errno);
        return -1;
    }
		*rd_val=((uint16)recvbuf[0]<<8)|(recvbuf[1]);
    return 0;
}


int32 mgmt_spi_rd32(uint32 addr, uint32 *rd_val)
{
    uint8 buf[MAX_BUF_SZ];
	  uint8 recvbuf[MAX_BUF_SZ];
    int32 len = 0;
    int i, errno;

    //opcode
    buf[len++] = SPI_OPCODE_PHYADDR(g_spi_id) | SPI_OPCODE_RD | SPI_OPCODE_NO_INC | SPI_OPCODE_RD_WAIT_2 | SPI_OPCODE_TX_SZ_32;  //--WAIT_6
    buf[len++] = (addr >> 24) & 0xff;
    buf[len++] = (addr >> 16) & 0xff;
    buf[len++] = (addr >> 8) & 0xff;
    buf[len++] = (addr & 0xff);
    /* wait states as per opcode */
    for (i =0; i < ((buf[0] & SPI_OPCODE_RD_WAIT_MASK) >> SPI_OPCODE_RD_WAIT_SHIFT) * 2; i++)
        buf[len++] = 0x0;
    //errno = aps_spi_read(DEFAULT_DEVICE_ID, &buf[0], len, (uint8*)rd_val, sizeof(uint32));
		errno = aps_spi_read(DEFAULT_DEVICE_ID, &buf[0], len, (uint8*)recvbuf, sizeof(uint32));
    if (errno) {
        printk("aps_spi_read failed errno=%d\r\n", errno);
        return -1;
    }
		*rd_val=((uint32)recvbuf[0]<<24)|((uint32)recvbuf[1]<<16)|((uint32)recvbuf[2]<<8)|(recvbuf[3]);
    return 0;
}

#if 0
static uint32 mgmt_spi_rd64(uint32 addr, uint64 *rd_val)
{
    uint8 buf[MAX_BUF_SZ];
    int32 len = 0;
    int i, errno;

    //opcode
    buf[len++] = SPI_OPCODE_PHYADDR(g_spi_id) | SPI_OPCODE_RD | SPI_OPCODE_NO_INC | SPI_OPCODE_RD_WAIT_2 | SPI_OPCODE_TX_SZ_64;  //--WAIT_6
    buf[len++] = (addr >> 24) & 0xff;
    buf[len++] = (addr >> 16) & 0xff;
    buf[len++] = (addr >> 8) & 0xff;
    buf[len++] = (addr & 0xff);
    /* wait states as per opcode */
    for (i =0; i < ((buf[0] & SPI_OPCODE_RD_WAIT_MASK) >> SPI_OPCODE_RD_WAIT_SHIFT) * 2; i++)
        buf[len++] = 0x0;
    errno = (uint16)aps_spi_read(DEFAULT_DEVICE_ID, &buf[0], len, (uint8*)rd_val, sizeof(uint64));
    if (errno) {
        printk("aps_spi_read failed errno=%d\r\n", errno);
        return -1;
    }
    printk("rd val = 0x%x 0x%x\r\n", (uint32)(*rd_val >> 32), (uint32)(*rd_val & 0xffffffff));
    return 0;
}
#endif

int32 mgmt_spi_wr8_ind(uint32 addr, uint8 data)
{
    uint32 rd_val;
    uint32 wait = IND_ACC_RETRY;
    mgmt_spi_wr32(IND_ACC_RDB_IND_REGS_ADDR_SER_L16, addr);
    mgmt_spi_wr32(IND_ACC_RDB_IND_REGS_DATA_SER_L_L16, (uint32)(data));
    mgmt_spi_wr16(IND_ACC_RDB_IND_REGS_CTRL_SER_L16, (IND_ACC_COMMIT | IND_ACC_NO_AUTO_INC | IND_ACC_WR | IND_ACC_TX_SZ_8));
    do {
        mgmt_spi_rd32(IND_ACC_RDB_IND_REGS_CTRL_SER_L16, &rd_val);
    } while (--wait && !(rd_val & IND_ACC_DONE));
    if (!wait) {
        printk("Ind. Wr. timed out ctrl 0x%x\r\n", rd_val);
        return -1;
    }
    return 0;
}

int32 mgmt_spi_wr16_ind(uint32 addr, uint16 data)
{
    uint32 rd_val;
    uint32 wait = IND_ACC_RETRY;
    mgmt_spi_wr32(IND_ACC_RDB_IND_REGS_ADDR_SER_L16, addr);
    mgmt_spi_wr32(IND_ACC_RDB_IND_REGS_DATA_SER_L_L16, (uint32)data);
    mgmt_spi_wr16(IND_ACC_RDB_IND_REGS_CTRL_SER_L16, (IND_ACC_COMMIT | IND_ACC_NO_AUTO_INC | IND_ACC_WR | IND_ACC_TX_SZ_16));
    do {
        mgmt_spi_rd32(IND_ACC_RDB_IND_REGS_CTRL_SER_L16, &rd_val);
    } while (--wait && !(rd_val & IND_ACC_DONE));
    if (!wait) {
        printk("Ind. Wr. timed out ctrl 0x%x\r\n", rd_val);
        return -1;
    }
    return 0;
}

int32 mgmt_spi_wr32_ind(uint32 addr, uint32 data)
{
    uint32 rd_val;
    uint32 wait = IND_ACC_RETRY;
    mgmt_spi_wr32(IND_ACC_RDB_IND_REGS_ADDR_SER_L16, addr);
    mgmt_spi_wr32(IND_ACC_RDB_IND_REGS_DATA_SER_L_L16, data);
    mgmt_spi_wr16(IND_ACC_RDB_IND_REGS_CTRL_SER_L16, (IND_ACC_COMMIT | IND_ACC_NO_AUTO_INC | IND_ACC_WR | IND_ACC_TX_SZ_32));
    do {
        mgmt_spi_rd32(IND_ACC_RDB_IND_REGS_CTRL_SER_L16, &rd_val);
    } while (--wait && !(rd_val & IND_ACC_DONE));
    if (!wait) {
        printk("Ind. Wr. timed out ctrl 0x%x\r\n", rd_val);
        return -1;
    }
    return 0;
}

static int32 mgmt_spi_wr64_ind(uint32 addr, uint64 data)
{
    uint32 rd_val;
    uint32 wait = IND_ACC_RETRY;
    mgmt_spi_wr32(IND_ACC_RDB_IND_REGS_ADDR_SER_L16, addr);
    mgmt_spi_wr32(IND_ACC_RDB_IND_REGS_DATA_SER_L_L16, (uint32)(data & 0xFfFfFfFf));
    mgmt_spi_wr32(IND_ACC_RDB_IND_REGS_DATA_SER_H_L16, (uint32)(data >> 32));
    mgmt_spi_wr16(IND_ACC_RDB_IND_REGS_CTRL_SER_L16, (IND_ACC_COMMIT | IND_ACC_NO_AUTO_INC | IND_ACC_WR | IND_ACC_TX_SZ_64));
    do {
        mgmt_spi_rd32(IND_ACC_RDB_IND_REGS_CTRL_SER_L16, &rd_val);
    } while (--wait && !(rd_val & IND_ACC_DONE));
    if (!wait) {
        printk("Ind. Wr. timed out ctrl 0x%x\r\n", rd_val);
        return -1;
    }
    return 0;
}

static int32 mgmt_spi_rd8_ind(uint32 addr, uint8 *data)
{
    uint32 rd_val;
    uint32 wait = IND_ACC_RETRY;
    mgmt_spi_wr32(IND_ACC_RDB_IND_REGS_ADDR_SER_L16, addr);
    mgmt_spi_wr16(IND_ACC_RDB_IND_REGS_CTRL_SER_L16, (IND_ACC_COMMIT | IND_ACC_NO_AUTO_INC | IND_ACC_RD | IND_ACC_TX_SZ_8));
    do {
        mgmt_spi_rd32(IND_ACC_RDB_IND_REGS_CTRL_SER_L16, &rd_val);
    } while (--wait && !(rd_val & IND_ACC_DONE));
    if (!wait) {
        printk("Ind. Wr. timed out ctrl 0x%x\r\n", rd_val);
        return -1;
    }
    mgmt_spi_rd32(IND_ACC_RDB_IND_REGS_DATA_SER_L_L16, &rd_val);
    *data = (uint8)(rd_val & 0xff);
    return 0;
}

int32 mgmt_spi_rd16_ind(uint32 addr, uint16 *data)
{
    uint32 rd_val;
    uint32 wait = IND_ACC_RETRY;
    mgmt_spi_wr32(IND_ACC_RDB_IND_REGS_ADDR_SER_L16, addr);
    mgmt_spi_wr16(IND_ACC_RDB_IND_REGS_CTRL_SER_L16, (IND_ACC_COMMIT | IND_ACC_NO_AUTO_INC | IND_ACC_RD | IND_ACC_TX_SZ_16));
    do {
        mgmt_spi_rd32(IND_ACC_RDB_IND_REGS_CTRL_SER_L16, &rd_val);
    } while (--wait && !(rd_val & IND_ACC_DONE));
    if (!wait) {
        printk("Ind. Wr. timed out ctrl 0x%x\r\n", rd_val);
        return -1;
    }
    mgmt_spi_rd16(IND_ACC_RDB_IND_REGS_DATA_SER_L_L16, data);
    return 0;
}

static int32 mgmt_spi_rd32_ind(uint32 addr, uint32 *data)
{
    uint32 rd_val;
    uint32 wait = IND_ACC_RETRY;
    mgmt_spi_wr32(IND_ACC_RDB_IND_REGS_ADDR_SER_L16, addr);
    mgmt_spi_wr16(IND_ACC_RDB_IND_REGS_CTRL_SER_L16, (IND_ACC_COMMIT | IND_ACC_NO_AUTO_INC | IND_ACC_RD | IND_ACC_TX_SZ_32));
    do {
        mgmt_spi_rd32(IND_ACC_RDB_IND_REGS_CTRL_SER_L16, &rd_val);
    } while (--wait && !(rd_val & IND_ACC_DONE));
    if (!wait) {
        printk("Ind. Wr. timed out ctrl 0x%x\r\n", rd_val);
        return -1;
    }
    mgmt_spi_rd32(IND_ACC_RDB_IND_REGS_DATA_SER_L_L16, data);
    return 0;
}

static int32 mgmt_spi_rd64_ind(uint32 addr, uint64 *data)
{
    uint32 rd_val;
    uint32 wait = IND_ACC_RETRY;
    mgmt_spi_wr32(IND_ACC_RDB_IND_REGS_ADDR_SER_L16, addr);
    mgmt_spi_wr16(IND_ACC_RDB_IND_REGS_CTRL_SER_L16, (IND_ACC_COMMIT | IND_ACC_NO_AUTO_INC | IND_ACC_RD | IND_ACC_TX_SZ_64));

    do {
        mgmt_spi_rd32(IND_ACC_RDB_IND_REGS_CTRL_SER_L16, &rd_val);
    } while (--wait && !(rd_val & IND_ACC_DONE));
    if (!wait) {
        printk("Ind. Wr. timed out ctrl 0x%x\r\n", rd_val);
        return -1;
    }
    /* BE host */
    mgmt_spi_rd32(IND_ACC_RDB_IND_REGS_DATA_SER_L_L16, &((uint32*)data)[1]);
    mgmt_spi_rd32(IND_ACC_RDB_IND_REGS_DATA_SER_H_L16, &((uint32*)data)[0]);
    return 0;
}



/*@api
 * mgmt_spi_recv
 *
 * @brief
 * Receive bytes on the SPI interface
 *
 * @param=info - pointer to the mgmt_info_t
 * @param=data - pointer to the buffer to place the data into
 * @param=datalen - pointer to where to store the length of the decoded data received
 *
 * @returns 0 - success
 * @returns !0 - error
 *
 */
static int32 mgmt_spi_recv(mgmt_info_t *info, uint8 *data, uint32 *datalen)
{
    uint16 sw_spare15;
    int32 errno = -1;
    uint32 retry;
    uint32 src;
	uint32 len_offset;

    if (info == NULL) {
      printk("%s :: info pointer is NULL\r\n", __FUNCTION__);
      return errno;
    }

    if (data == NULL || datalen == NULL) {
      printk("%s :: Invalid input parameters(data = 0x%x, datalen = 0x%x)\r\n", __FUNCTION__, (unsigned int)data, (unsigned int)datalen);
      return errno;
    }

    retry = MGMT_SPI_RECV_MAX_RETRIES_SEC * MSEC_PER_SEC;
    do {
        /* Check shared registers to know the status of slave
           and also the location where the msg is to be stored */
        errno = mgmt_spi_rd16(MISC_SPARE_SW_REG15, &sw_spare15);
        if (errno) {
            printk("failed to read shared register, err=0x%x\r\n", errno);
            goto err_read;
        }
        if (!(sw_spare15 & MSG_QUEUED)) {
            PLATFORM_OS_TimeDly(1);
            continue;
        } else {
            break;
        }
    } while (--retry);

    if (!retry) {
        printk("Timed out, target might be inactive, sw_spare15=0x%x\r\n", sw_spare15);
        goto err_timeout;
    }
    src = (sw_spare15 & MSG_BUFFER) << MSG_BUFFER_SHIFT;
    if (!src) {
        printk("Reply buffer addr is 0\r\n");
        goto err_src;
    }

    len_offset = (uint32)&((mgmt_reply_t *)0)->len;
    /* Get length and then the rest*/
    mgmt_spi_rd32_ind(src + len_offset, datalen);

    if (*datalen > REPLY_BUFFER_MAX) {
        errno = -1;
        printk("reply buffer size is incorrect! %u\r\n", *datalen);
        goto err_size;
    }
    /* Read 32bit at a time. Since reply buffer has 16byte padding,
       it is safe to ceil datalen to 32bit */
    errno = mgmt_spi_rd32_bulk(data, src, CEIL_32(*datalen), 0);

    if (errno) {
        printk("Failed to retrieve reply buffer! err=0x%x\r\n", errno);
        goto err_copy;
    } else {
        /* Clear queued reply in sw_spare15 register  */
        mgmt_spi_wr16(MISC_SPARE_SW_REG15, sw_spare15 & MSG_BUFFER);
    }
    errno = 0;
err_copy:
err_size:
err_src:
err_timeout:
err_read:
    return errno;
}

/*@api
 * mgmt_spi_cmd
 *
 * @brief
 * Issue a command to the Broadcom Polar
 *
 * @param=info - pointer to mgmt_info_t
 * @param=cmd - pointer to mgmt_command_t to send
 * @returns 0 - success
 * @returns !0 - error
 *
 * @desc
 */
static int32 mgmt_spi_cmd(mgmt_info_t *info, mgmt_command_t *cmd)
{
    int32 errno = -1;
    uint8 *tx = (uint8*)cmd;

    if (info == NULL) {
      printk("%s :: info pointer is NULL\r\n", __FUNCTION__);
      return errno;
    }

    if (cmd == NULL) {
      printk("%s :: cmd pointer is NULL\r\n", __FUNCTION__);
      return errno;
    }

    errno = mgmt_spi_send(info, tx, uswap32(cmd->len));
    if (errno) {
#ifdef DEBUG
        printk("mgmt_spi_cmd, mgmt_spi_send failed (CMD)\r\n");
#endif
        return errno;
    } else {
#if DEBUG
        printk("mgmt_spi_cmd succeeded\r\n");
#endif
    }
    return 0;
}

/*@api
 * mgmt_spi_reply
 *
 * @brief
 * Retrieve a reply to a command.
 *
 * @param=info - pointer to mgmt_info_t
 * @param=reply - pointer to mgmt_reply_t to fill in
 * @param=len - pointer to length to fill in
 * @returns 0 - success
 * @returns !0 - error
 *
 * @desc
 */
static int32 mgmt_spi_reply(mgmt_info_t *info, mgmt_reply_t *reply, int32 *rlen)
{
    /* Wait for reply ready */
    uint32 i;
    int32 errno = -1;
    uint8 cksum;
    uint8 *rx = (uint8_t *) reply;

    if (info == NULL) {
      printk("%s :: info pointer is NULL\r\n", __FUNCTION__);
      return errno;
    }

    if ((reply == NULL) || (rlen == NULL)) {
      printk("%s :: Invalid input parameter(reply = 0x%x and rlen = 0x%x)\r\n", __FUNCTION__, (unsigned int)reply, (unsigned int)rlen);
      return errno;
    }

    sal_memset(rx, 0, sizeof(rx));
    errno = mgmt_spi_recv(info, rx, &reply->len);
    if (errno) {
        goto done;
    }
#if DEBUG > 2
    mgmt_dump_buffer("SPI reply", rx, reply->len);
#endif
    /* verify the checksum and length */
    if (reply->len > REPLY_BUFFER_MAX) {
#if DEBUG > 2
        printk("mgmt_spi_reply: bad len %d\r\n",
                reply->len);
        mgmt_dump_buffer("SPI reply", rx, 32);
#endif
        errno = -1;
        goto done;
    }
    for (i = 0, cksum = 0; i < reply->len; ++i) {
        cksum += rx[i];
    }
    if (cksum) {
#ifdef DEBUG
        printk("mgmt_spi_reply: bad cksum 0x%02x\r\n", cksum);
#endif
        errno = -1;
        goto done;
    }
done:
    return errno;
}


/*@api
 * mgmt_cmd
 *
 * @brief
 * Issue a command to the Broadcom Polar
 *
 * @param=info - pointer to mgmt_info_t
 * @param=cmd - pointer to mgmt_command_t to send
 * @param=encrypt - flag to request encryption
 * @returns 0 - success
 * @returns !0 - error
 *
 * @desc
 */
static int32 mgmt_cmd(mgmt_info_t *info, mgmt_command_t *cmd, int32 encrypt)
{
    int32 errno = -1;

    if (info == NULL) {
      printk("%s :: info pointer is NULL\r\n", __FUNCTION__);
      return errno;
    }
    if (cmd == NULL) {
      printk("%s :: cmd pointer is NULL\r\n", __FUNCTION__);
      return errno;
    }

    /* Set the magic number */
    cmd->magic = uswap32(MGMT_COMMAND_MAGIC);

#if 0
    /* zero out unused bytes in the command structure */
    sal_memset(((uint8 *)cmd) + uswap32(cmd->len), 0, sizeof(mgmt_command_t) - uswap32(cmd->len));
#endif

#if DEBUG > 2
    mgmt_dump_buffer("cmd", (uint8 *)cmd, uswap32(cmd->len));
#endif

    errno = mgmt_spi_cmd(info, cmd);
    if (errno) {
#if DEBUG
        printk("mgmt_cmd: could not send SPI data, errno = %d\r\n", errno);
#endif
        return errno;
    }
    return 0;
}

/*@api
 * mgmt_reply
 *
 * @brief
 * Retrieve a reply to a command.
 *
 * @param=info - pointer to mgmt_info_t
 * @param=reply - pointer to mgmt_reply_t to fill in
 * @param=len - pointer to length to fill in
 * @param=decrypt - flag to request decryption
 * @returns 0 - success
 * @returns !0 - error
 *
 * @desc
 */
    static
int32 mgmt_reply(mgmt_info_t *info, mgmt_reply_t *reply, int32 *len, int32 decrypt)
{
    int32 errno = -1;

    if (info == NULL) {
      printk("%s :: info pointer is NULL\r\n", __FUNCTION__);
      return errno;
    }

    if ((reply == NULL) || (len == NULL)) {
      printk("%s :: Invalid input parameter(reply = 0x%x, len = 0x%x)\r\n", __FUNCTION__, (unsigned int)reply, (unsigned int)len);
      return errno;
    }

    errno = mgmt_spi_reply(info, reply, len);
    if (errno) {
#if DEBUG
        printk("mgmt_reply: could not receive SPI data, errno = %d\r\n", errno);
#endif
        return errno;
    }
#if DEBUG > 2
    printk("reply: status %d len %d magic 0x%08x\r\n",
            uswap32(reply->status), uswap32(reply->len), uswap32(reply->magic));
#endif
    if (uswap32(reply->len) < MGMT_STATUS_LEN) {
#if DEBUG
        printk("reply: truncated reply\r\n");
#endif
        return -1;
    }
    /* Detect unexpected plaintext replies (usually errors) */
    if (uswap32(reply->magic) == MGMT_REPLY_MAGIC) {
    } else {
#if DEBUG
        printk("bad magic number in reply, 0x%08x\r\n", uswap32(reply->magic));
#endif
        return -1;
    }

    return 0;
}

/*@api
 * mgmt_cmd_reply
 *
 * @brief
 * Issue a command to the Polar
 *
 * @param=info - pointer to mgmt_info_t
 * @param=cmd - pointer to mgmt_command_t to send
 * @param=reply - pointer to mgmt_reply_t to fill in
 * @param=encrypt - flag to request encryption/decryption
 * @param=need_reply - flag to indicate if reply is needed
 * @returns 0 - success
 * @returns !0 - error
 *
 * @desc
 */
    static
int32 mgmt_cmd_reply(mgmt_info_t *info, mgmt_command_t *cmd,
        mgmt_reply_t *reply, int32 encrypt)
{
    int32 len;
    int32 errno = -1;

    if (info == NULL) {
      printk("%s :: info pointer is NULL\r\n", __FUNCTION__);
      return errno;
    }

    if ((reply == NULL) || (cmd == NULL)) {
      printk("%s :: Invalid input parameter(reply = 0x%x, cmd = 0x%x)\r\n", __FUNCTION__, (unsigned int)reply, (unsigned int)cmd);
      return errno;
    }

    errno = mgmt_cmd(info, cmd, encrypt);
    if (errno) {
#if DEBUG
        printk("could not send command\r\n");
#endif
        return errno;
    }

    if (reply) {
        errno = mgmt_reply(info, reply, &len, encrypt);
        if (errno) {
#if  DEBUG > 1
            printk("could not get reply\r\n");
#endif
            return errno;
        }
        if (uswap32(reply->status) != MGMT_SUCCESS) {
#if DEBUG > 1
            printk("error in reply status=%d\r\n", uswap32(reply->status));
#endif
            return -1;
        }
    }

    return 0;
}

uint32 hex2uint(const char *h)
{
    uint32 val = 0;
    char c;
    while (((c = *h++) != '\0') && (c != '\n')) {
        if ((c >= 'a') && (c <= 'f'))
            val = (val << 4) | (c - 'a' + 10);
        if ((c >= 'A') && (c <= 'F'))
            val = (val << 4) | (c - 'A' + 10);
        if ((c >= '0') && (c <= '9'))
            val = (val << 4) | (c - '0');
    }
    return val;
}

enum flash_status_t {
    FLASH_NONE,
    FLASH_INIT,
    FLASH_ERASE,
    FLASH_WRITE,
    FLASH_VERIFY,
};

/* image in binary format: supports only 32 or 64 bit transfers */
static void tx_tcm_data(uint32 dst, uint8 *armbuf, int32 armlen, uint32 tx_sz)
{
    uint32 src;
    uint32 dst_end = dst + armlen;
    if (tx_sz == sizeof(uint32)) {
        for(src = 0; dst < dst_end; src += tx_sz, dst += tx_sz)
            mgmt_spi_wr32_ind(dst, uswap32(*(uint32_t *)(&armbuf[src])));
    } else if (tx_sz == sizeof(uint64)){
        for(src = 0; dst < dst_end; src += tx_sz, dst += tx_sz)
            mgmt_spi_wr64_ind(dst, (*(uint64_t *)(&armbuf[src])));
    } else {
        printf("Error: Unsupported transfer size %d\r\n", tx_sz);
    }
}

static int32 speed_up_reset_target(uint32_t halt)
{
    int32 errno = -1;
    uint32_t cpusys;

    /* Let switch know that P7 is going down soon */
    errno = mgmt_spi_wr16(0xa84002a, 0x01);
    if (errno) {
        printk("failed to bring down P7 err=%d\r\n", errno);
        goto rd_wr_err;
    }

    /*	Speed up cpu clocks	*/
    errno = mgmt_spi_wr32(0x00146044, 0x000c4059);
    if (errno) {
        printk("failed to set clocks err=%d\r\n", errno);
        goto rd_wr_err;
    }

    /* Reset override and hold reset */
    cpusys = 0x033f99ca;

    if (halt) {
        /* Boot from 0x0 */
        cpusys &= ~0x40;

        /* Halt override and halt */
        cpusys |= 0x20;
    } else {
        /* Clear F1 status */
        mgmt_spi_wr16(0xa800300, 0x0);
    }

    /*	Hold CPU in Halt mode	*/
    errno = mgmt_spi_wr32(0x00148004, cpusys);
    if (errno) {
        printk("failed to halt cpu err=%d\r\n", errno);
        goto rd_wr_err;
    }

    /* Release reset */
    cpusys = (cpusys & ~0xF) | 0x5;

    errno = mgmt_spi_wr32(0x00148004, cpusys);
    if (errno) {
        printk("failed to halt cpu err=%d\r\n", errno);
        goto rd_wr_err;
    }
rd_wr_err:
    return errno;
}

#define MAX_SEQ     (1)

/*@api
 * mgmt_read_switch_register
 *
 * @brief
 * Reads a switch register via SPI indirect mode
 *
 * @param=info - pointer to mgmt_info_t
 * @param=addr - switch register to read
 * @returns 0 - success
 * @returns !0 - error
 *
 * @desc
 */
int32 mgmt_read_switch_register(mgmt_info_t *info, uint32 addr,
        uint32 size, uint64 *regval)
{
    if (info == NULL) {
      printk("%s :: mgmt_info_t pointer is NULL\r\n", __FUNCTION__);
      return ERR_PARAM;
    }

    return mgmt_spi_rd64_ind(addr, regval);
}

/*@api
 * mgmt_write_switch_register
 *
 * @brief
 * Implements the MGMT_SWITCH_WRITE_REG command/reply exchange
 *
 * @param=info - pointer to mgmt_info_t
 * @param=addr - switch register to write
 * @param=regval - value to write
 * @returns 0 - success
 * @returns !0 - error
 *
 * @desc
 */
int32 mgmt_write_switch_register(mgmt_info_t *info, uint32 addr,
        uint32 size, uint64 regval)
{
    if (info == NULL) {
      printk("%s :: mgmt_info_t pointer is NULL\r\n", __FUNCTION__);
      return ERR_PARAM;
    }

    return mgmt_write(info, addr, RD_WR_MODE_64, regval);
}


/*
 * @api
 * mgmt_set_spi_id
 *
 * @brief
 *   Set the SPI_ID of BCM8953X to communicate
 *
 * @param=info - pointer to the mgmt_info_t
 * @param=id - ID to use
 * @returns 0 on success, !0 on error
 */
int32 mgmt_set_spi_id(mgmt_info_t *info, SPI_ID_t id)
{
    int32 ret;
    if (info == NULL) {
      printk("%s :: mgmt_info_t pointer is NULL\r\n", __FUNCTION__);
      return ERR_PARAM;
    }

    if (id > 3) {
        ret = MGMT_ERROR;
    } else {
        switch(id) {
        case 0:
            g_spi_id = SPI_ID_0;
            break;
        case 1:
            g_spi_id = SPI_ID_1;
            break;
        case 3:
            g_spi_id = SPI_ID_3;
            break;
        default:
            g_spi_id = SPI_ID_2;
            break;
        }
        ret = MGMT_SUCCESS;
    }
    return ret;
}

/*
 * @api
 * mgmt_reboot
 *
 * @brief
 * In case reason is other than MGMT_REBOOT_CHIP,
 *   send a command to BCM8953X, requesting it to reboot itself
 * else
 *   issue a soft-reset on complete BCM8953X system
 *
 * @param=info - pointer to the mgmt_info_t
 * @param=slot - slot to reboot
 * @returns 0 on success, !0 on error
 */
int32 mgmt_reboot(mgmt_info_t *info, uint32 reason)
{
    int32 errno = -1;
    mgmt_command_t cmd_packet;
    mgmt_command_t *cmd = &cmd_packet;
    mgmt_reply_t reply_packet;
    mgmt_reply_t *reply = &reply_packet;

    if (info == NULL) {
        printk("%s :: mgmt_info_t pointer is NULL\r\n", __FUNCTION__);
        return errno;
    }

    if (MGMT_REBOOT_CHIP == reason) {
        errno = mgmt_spi_wr16(CRG_RESET_CONFIG, SRST_CHIP);
    } else {
        sal_memset(cmd, 0, sizeof(mgmt_command_t));
        sal_memset(reply, 0, sizeof(mgmt_reply_t));

        cmd->cmd = uswap32(MGMT_REBOOT);
        cmd->len = uswap32(MGMT_COMMAND_LEN + sizeof(cmd->u.reboot));
        cmd->u.reboot.reason = uswap32(reason);

        errno = mgmt_cmd_reply(info, cmd, reply, 0);
    }

    return errno;
}

int32 mgmt_write(mgmt_info_t *info, uint32 addr, uint32 mode, uint64 data)
{
    /* Access ARM memspace only through indirect block */
    switch(mode) {
        case RD_WR_MODE_8:
            mgmt_spi_wr8_ind(addr, (uint8)(data & 0xff));
            break;
        case RD_WR_MODE_16:
            mgmt_spi_wr16_ind(addr, (uint16)(data & 0xffff));
            break;
        case RD_WR_MODE_32:
            mgmt_spi_wr32_ind(addr, (uint32)(data & 0xffffffff));
            break;
        case RD_WR_MODE_64:
            mgmt_spi_wr64_ind(addr, data);
            break;
        default:
            return -1;
    }
    return 0;
}


int32 mgmt_read(mgmt_info_t *info, uint32 addr, uint32 mode, uint64 *data)
{
    switch(mode) {
        case RD_WR_MODE_8:
            mgmt_spi_rd8_ind(addr, (uint8*)data);
            printk("0x%x\r\n", *(uint8 *)data);
            break;
        case RD_WR_MODE_16:
            mgmt_spi_rd16_ind(addr, (uint16*)data);
            printk("0x%x\r\n", *(uint16 *)data);
            break;
        case RD_WR_MODE_32:
            mgmt_spi_rd32_ind(addr, (uint32*)data);
            printk("0x%x\r\n", *(uint32 *)data);
            break;
        case RD_WR_MODE_64:
            mgmt_spi_rd64_ind(addr, (uint64*)data);
            printk("0x%llx\r\n", *data);
            break;
        default:
            return -1;
    }
    return 0;
}



/*
 * @api
 * mgmt_write_memory
 *
 * @brief
 * Write a page of memory to the target
 *
 * @param=info - pointer to the mgmt_info_t
 * @param=address - address to write
 * @param=data - buffer containing the data
 * @returns 0 on success, !0 on error
 */
int32 mgmt_write_memory(mgmt_info_t *info, uint32 address, uint8 *data)
{
    if ((info == NULL) || (data == NULL)) {
      printk("%s :: Atleast one of the input pointer argument is NULL\r\n", __FUNCTION__);
      return ERR_PARAM;
    }

    mgmt_spi_wr32_bulk(address, data, MGMT_MEMORY_PAGE_SIZE, 0);

    return 0;
}

/*
 * @api
 * mgmt_port_info_get
 *
 * @brief
 * Get the port information
 *
 * @param=info - management info
 * @param=port - port to get the information
 * @param=port_info - structure to fill port information
 *
 * @returns 0 on success, !0 on error
 */
int32 mgmt_port_info_get(mgmt_info_t *info, uint32 port, mgmt_port_info_t *port_info)
{
    int32 errno = -1;
    mgmt_command_t cmd_packet;
    mgmt_command_t *cmd = &cmd_packet;
    mgmt_reply_t reply_packet;
    mgmt_reply_t *reply = &reply_packet;

    if ((info == NULL) || (port_info == NULL)) {
      printk("%s :: Invalid input parameter(info = 0x%x and port_info = 0x%x)\r\n",
                                                    __FUNCTION__, (unsigned int)info, (unsigned int)port_info);
      return errno;
    }

    if ((IS_EXTERNAL_INTF_INVALID(port))) {
      printk("%s :: Invalid port number %d\r\n", __FUNCTION__, port);
      return errno;
    }

    sal_memset(cmd, 0, sizeof(mgmt_command_t));
    sal_memset(reply, 0, sizeof(mgmt_reply_t));

    cmd->cmd = uswap32(MGMT_AVB);
    cmd->u.avb.avb_cmd = uswap32(MGMT_AVB_PORT_INFO_GET);
    cmd->u.avb.u.port_info.num = uswap32(port);
    cmd->len = uswap32(MGMT_AVB_COMMAND_LEN + sizeof(cmd->u.avb.u.port_info));

    errno = mgmt_cmd_reply(info, cmd, reply, 0);
    if (errno){
        return errno;
    }

    port_info->admin_mode = uswap32(reply->u.avb.port_info.admin_mode);
    port_info->link_status = uswap32(reply->u.avb.port_info.link_status);
    port_info->speed = uswap32 (reply->u.avb.port_info.speed);
    port_info->br_mode = uswap32 (reply->u.avb.port_info.br_mode);
    port_info->jumbo_frame_mode = uswap32 (reply->u.avb.port_info.jumbo_frame_mode);
    port_info->loop_back_mode = uswap32 (reply->u.avb.port_info.loop_back_mode);
    port_info->autoneg = uswap32 (reply->u.avb.port_info.autoneg);
    port_info->autoneg_complete = uswap32 (reply->u.avb.port_info.autoneg_complete);
    port_info->duplex= uswap32 (reply->u.avb.port_info.duplex);
    port_info->led = uswap32 (reply->u.avb.port_info.led);
    port_info->link_state_change_cnt = uswap32 (reply->u.avb.port_info.link_state_change_cnt);

    return 0;
}


/*
 * @api
 * mgmt_port_admin_mode_set
 *
 * @brief
 * Set the port admin mode
 *
 * @param=info - management info
 * @param=port - port to get the information
 * @param=admin_mode - 0=disable, 1=enable
 *
 * @returns 0 on success, !0 on error
 */

int32 mgmt_port_admin_mode_set(mgmt_info_t *info, uint32 port, uint32 admin_mode)
{
    int32 errno = -1;
    mgmt_command_t cmd_packet;
    mgmt_command_t *cmd = &cmd_packet;
    mgmt_reply_t reply_packet;
    mgmt_reply_t *reply = &reply_packet;

    if (info == NULL) {
      printk("%s :: mgmt_info_t pointer is NULL\r\n", __FUNCTION__);
      return errno;
    }

    if ((IS_EXTERNAL_INTF_INVALID(port))) {
      printk("%s :: Invalid port number %d\r\n", __FUNCTION__, port);
      return errno;
    }

    if ((admin_mode != ENABLED) && (admin_mode != DISABLED)) {
      printk("%s :: Invalid setting for port admin mode\r\n", __FUNCTION__);
      return errno;
    }

    sal_memset(cmd, 0, sizeof(mgmt_command_t));
    sal_memset(reply, 0, sizeof(mgmt_reply_t));

    cmd->cmd = uswap32(MGMT_AVB);
    cmd->u.avb.avb_cmd = uswap32(MGMT_AVB_PORT_ADMIN_MODE_SET);
    cmd->u.avb.u.port_admin_mode.num = uswap32(port);
    cmd->u.avb.u.port_admin_mode.value = uswap32(admin_mode);
    cmd->len = uswap32(MGMT_AVB_COMMAND_LEN + sizeof(cmd->u.avb.u.port_admin_mode));

    errno = mgmt_cmd_reply(info, cmd, reply, 0);
    if (errno) {
        return errno;
    }
    return 0;
}

/*
 * @api
 * mgmt_admin_mode_get
 *
 * @brief
 * Get admin mode
 *
 * @param=info - management info
 * @param=port - port to get the information
 * @param=admin_mode - information to get
 *
 * @returns 0 on success, !0 on error
 */
int32 mgmt_port_admin_mode_get(mgmt_info_t *info, uint32 port, uint32 *admin_mode)
{
    int32 errno = -1;
    mgmt_command_t cmd_packet;
    mgmt_command_t *cmd = &cmd_packet;
    mgmt_reply_t reply_packet;
    mgmt_reply_t *reply = &reply_packet;

    if (info == NULL) {
      printk("%s :: mgmt_info_t pointer is NULL\r\n", __FUNCTION__);
      return errno;
    }

    if ((IS_EXTERNAL_INTF_INVALID(port))) {
      printk("%s :: Invalid port number %d\r\n", __FUNCTION__, port);
      return errno;
    }

    sal_memset(cmd, 0, sizeof(mgmt_command_t));
    sal_memset(reply, 0, sizeof(mgmt_reply_t));
    cmd->cmd = uswap32(MGMT_AVB);
    cmd->u.avb.avb_cmd = uswap32(MGMT_AVB_PORT_ADMIN_MODE_GET);
    cmd->u.avb.u.port_admin_mode.num = uswap32(port);
    cmd->len = uswap32(MGMT_AVB_COMMAND_LEN + sizeof(cmd->u.avb.u.port_admin_mode));

    errno = mgmt_cmd_reply(info, cmd, reply, 0);
    if (errno) {
        return errno;
    }
    *admin_mode = uswap32(reply->u.avb.port_admin_mode.value);
    return 0;
}


/*
 * @api
 * mgmt_port_speed_set
 *
 * @brief
 * Set the port speed
 *
 * @param=info - management info
 * @param=port - port to get the information
 * @param=speed - 0=auto, 10=10mpbs, 100=100mpbs, 1000=1G
 *
 * @returns 0 on success, !0 on error
 */
int32 mgmt_port_speed_set(mgmt_info_t *info,uint32 port, uint32 speed)
{
    int32 errno = -1;
    mgmt_command_t cmd_packet;
    mgmt_command_t *cmd = &cmd_packet;
    mgmt_reply_t reply_packet;
    mgmt_reply_t *reply = &reply_packet;

    if (info == NULL) {
      printk("%s :: mgmt_info_t pointer is NULL\r\n", __FUNCTION__);
      return errno;
    }

    if ((IS_EXTERNAL_INTF_INVALID(port))) {
      printk("%s :: Invalid port number %d\r\n", __FUNCTION__, port);
      return errno;
    }

    if ((speed != MGMT_AVB_PORT_SPEED_0) && (speed != MGMT_AVB_PORT_SPEED_100) &&
                                        (speed != MGMT_AVB_PORT_SPEED_1000)) {
      printk("%s :: Invalid port speed value %d\r\n", __FUNCTION__, speed);
      return errno;
    }

    sal_memset(cmd, 0, sizeof(mgmt_command_t));
    sal_memset(reply, 0, sizeof(mgmt_reply_t));

    cmd->cmd = uswap32(MGMT_AVB);
    cmd->u.avb.avb_cmd = uswap32(MGMT_AVB_PORT_SPEED_SET);
    cmd->u.avb.u.port_speed.num = uswap32(port);
    cmd->u.avb.u.port_speed.value = uswap32(speed);
    cmd->len = uswap32(MGMT_AVB_COMMAND_LEN + sizeof(cmd->u.avb.u.port_speed));

    errno = mgmt_cmd_reply(info, cmd, reply, 0);
    if (errno) {
        return errno;
    }
    return 0;
}

/*
 * @api
 * mgmt_port_speed_get
 *
 * @brief
 * Get the speed
 *
 * @param=info - management info
 * @param=port - port to get the information
 * @param=speed - information to get
 *
 * @returns 0 on success, !0 on error
 */
int32 mgmt_port_speed_get(mgmt_info_t *info, uint32 port, uint32 *speed)
{
    int32 errno = -1;
    mgmt_command_t cmd_packet;
    mgmt_command_t *cmd = &cmd_packet;
    mgmt_reply_t reply_packet;
    mgmt_reply_t *reply = &reply_packet;

    if (info == NULL) {
      printk("%s :: mgmt_info_t pointer is NULL\r\n", __FUNCTION__);
      return errno;
    }

    if ((IS_EXTERNAL_INTF_INVALID(port))) {
      printk("%s :: Invalid port number %d\r\n", __FUNCTION__, port);
      return errno;
    }

    sal_memset(cmd, 0, sizeof(mgmt_command_t));
    sal_memset(reply, 0, sizeof(mgmt_reply_t));
    cmd->cmd = uswap32(MGMT_AVB);
    cmd->u.avb.avb_cmd = uswap32(MGMT_AVB_PORT_SPEED_GET);
    cmd->u.avb.u.port_speed.num = uswap32(port);
    cmd->len = uswap32(MGMT_AVB_COMMAND_LEN + sizeof(cmd->u.avb.u.port_speed));

    errno = mgmt_cmd_reply(info, cmd, reply, 0);
    if (errno) {
        return errno;
    }
    *speed = uswap32(reply->u.avb.port_speed.value);
    return 0;
}


/*
 * @api
 * mgmt_port_master_slave_set
 *
 * @brief
 * Set the BR master/slave mode
 *
 * @param=info - management info
 * @param=port - port to get the information
 * @param=master_slave - 0=slave, 1=master
 *
 * @returns 0 on success, !0 on error
 */
int32 mgmt_port_master_slave_set(mgmt_info_t *info, uint32 port, uint32 master_slave)
{
    int32 errno = -1;
    mgmt_command_t cmd_packet;
    mgmt_command_t *cmd = &cmd_packet;
    mgmt_reply_t reply_packet;
    mgmt_reply_t *reply = &reply_packet;

    if (info == NULL) {
      printk("%s :: mgmt_info_t pointer is NULL\r\n", __FUNCTION__);
      return errno;
    }

    if ((IS_EXTERNAL_INTF_INVALID(port))) {
      printk("%s :: Invalid port number %d\r\n", __FUNCTION__, port);
      return errno;
    }

    if ((master_slave != MASTER) && (master_slave != SLAVE)) {
      printk("%s :: Invalid master_slave configuration\r\n", __FUNCTION__);
      return errno;
    }

    sal_memset(cmd, 0, sizeof(mgmt_command_t));
    sal_memset(reply, 0, sizeof(mgmt_reply_t));
    cmd->cmd = uswap32(MGMT_AVB);
    cmd->u.avb.avb_cmd = uswap32(MGMT_AVB_PORT_MASTER_SLAVE_SET);
    cmd->u.avb.u.br_mode.num = uswap32(port);
    cmd->u.avb.u.br_mode.value = uswap32(master_slave);
    cmd->len = uswap32(MGMT_AVB_COMMAND_LEN + sizeof(cmd->u.avb.u.br_mode));

    errno = mgmt_cmd_reply(info, cmd, reply, 0);
    if (errno) {
        return errno;
    }
    return 0;
}

/*
 * @api
 * mgmt_port_master_slave_get
 *
 * @brief
 * Get the BR master/slave mode
 *
 * @param=info - management info
 * @param=port - port to get the information
 * @param=master_slave - information to get
 *
 * @returns 0 on success, !0 on error
 */
int32 mgmt_port_master_slave_get(mgmt_info_t *info, uint32 port, uint32 *master_slave)
{
    int32 errno = -1;
    mgmt_command_t cmd_packet;
    mgmt_command_t *cmd = &cmd_packet;
    mgmt_reply_t reply_packet;
    mgmt_reply_t *reply = &reply_packet;

    if (info == NULL) {
      printk("%s :: mgmt_info_t pointer is NULL\r\n", __FUNCTION__);
      return errno;
    }

    if ((IS_EXTERNAL_INTF_INVALID(port))) {
      printk("%s :: Invalid port number %d\r\n", __FUNCTION__, port);
      return errno;
    }

    sal_memset(cmd, 0, sizeof(mgmt_command_t));
    sal_memset(reply, 0, sizeof(mgmt_reply_t));
    cmd->cmd = uswap32(MGMT_AVB);
    cmd->u.avb.avb_cmd = uswap32(MGMT_AVB_PORT_MASTER_SLAVE_GET);
    cmd->u.avb.u.br_mode.num = uswap32(port);
    cmd->len = uswap32(MGMT_AVB_COMMAND_LEN + sizeof(cmd->u.avb.u.br_mode));

    errno = mgmt_cmd_reply(info, cmd, reply, 0);
    if (errno) {
        return errno;
    }
    *master_slave = uswap32(reply->u.avb.br_mode.value);
    return 0;
}

/*
 * @api
 * mgmt_port_phy_lb_set
 *
 * @brief
 * Set the port phy_lb mode
 *
 * @param=info - management info
 * @param=port - port to get the information
 * @param=phy_lb_mode - 0=disable, 1=enable
 *
 * @returns 0 on success, !0 on error
 */
int32 mgmt_port_phy_lb_set(mgmt_info_t *info, uint32 port, uint32 phy_lb_mode)
{
    int32 errno = -1;
    mgmt_command_t cmd_packet;
    mgmt_command_t *cmd = &cmd_packet;
    mgmt_reply_t reply_packet;
    mgmt_reply_t *reply = &reply_packet;

    if (info == NULL) {
      printk("%s :: mgmt_info_t pointer is NULL\r\n", __FUNCTION__);
      return errno;
    }

    if ((IS_EXTERNAL_INTF_INVALID(port))) {
      printk("%s :: Invalid port number %d\r\n", __FUNCTION__, port);
      return errno;
    }

    if ((phy_lb_mode != ENABLED) && (phy_lb_mode != DISABLED)) {
      printk("%s :: Invalid phy_lb_mode configuration\r\n", __FUNCTION__);
      return errno;
    }

    sal_memset(cmd, 0, sizeof(mgmt_command_t));
    sal_memset(reply, 0, sizeof(mgmt_reply_t));

    cmd->cmd = uswap32(MGMT_AVB);
    cmd->u.avb.avb_cmd = uswap32(MGMT_AVB_PORT_PHY_LB_SET);
    cmd->u.avb.u.port_phy_lb_mode.num = uswap32(port);
    cmd->u.avb.u.port_phy_lb_mode.value = uswap32(phy_lb_mode);
    cmd->len = uswap32(MGMT_AVB_COMMAND_LEN + sizeof(cmd->u.avb.u.port_phy_lb_mode));

    errno = mgmt_cmd_reply(info, cmd, reply, 0);
    if (errno) {
        return errno;
    }
    return 0;
}

/*
 * @api
 * mgmt_port_phy_lb_get
 *
 * @brief
 * Get the phy_lb mode
 *
 * @param=info - management info
 * @param=port - port to get the information
 * @param=phy_lb - information to get
 *
 * @returns 0 on success, !0 on error
 */
int32 mgmt_port_phy_lb_get(mgmt_info_t *info, uint32 port, uint32 *phy_lb)
{
    int32 errno = -1;
    mgmt_command_t cmd_packet;
    mgmt_command_t *cmd = &cmd_packet;
    mgmt_reply_t reply_packet;
    mgmt_reply_t *reply = &reply_packet;

    if (info == NULL) {
      printk("%s :: mgmt_info_t pointer is NULL\r\n", __FUNCTION__);
      return errno;
    }

    if ((IS_EXTERNAL_INTF_INVALID(port))) {
      printk("%s :: Invalid port number %d\r\n", __FUNCTION__, port);
      return errno;
    }

    sal_memset(cmd, 0, sizeof(mgmt_command_t));
    sal_memset(reply, 0, sizeof(mgmt_reply_t));
    cmd->cmd = uswap32(MGMT_AVB);
    cmd->u.avb.avb_cmd = uswap32(MGMT_AVB_PORT_PHY_LB_GET);
    cmd->u.avb.u.port_phy_lb_mode.num = uswap32(port);
    cmd->len = uswap32(MGMT_AVB_COMMAND_LEN + sizeof(cmd->u.avb.u.port_phy_lb_mode));

    errno = mgmt_cmd_reply(info, cmd, reply, 0);
    if (errno) {
        return errno;
    }
    *phy_lb = uswap32(reply->u.avb.port_phy_lb_mode.value);
    return 0;
}


/*
 * @api
 * mgmt_port_link_sqi_get
 *
 * @brief
 * Get the link signal quality index
 *
 * @param=info - management info
 * @param=port - port to get the information
 * @param=sqi_reg_val - pointer to fill the sqi value
 *
 * @returns 0 on success, !0 on error
 */

int32 mgmt_port_link_sqi_get(mgmt_info_t *info, uint32 port, uint32 *sqi)
{
    int32 errno = -1;
    mgmt_command_t cmd_packet;
    mgmt_command_t *cmd = &cmd_packet;
    mgmt_reply_t reply_packet;
    mgmt_reply_t *reply = &reply_packet;
    uint32 port_type;
    mgmt_port_info_t port_info;

    if ((info == NULL) || (sqi == NULL)) {
      printk("%s :: Atleast one of the input pointer argument is NULL\r\n", __FUNCTION__);
      return errno;
    }

    if(!MGMT_AVB_PHYS_INTF_IS_VALID(port)){
       printk("Error: Invalid port specified: %d\r\n", port);
       return errno;
    }
    mgmt_port_type_get(info, port, &port_type);
    if (port_type != MGMT_PORT_TYPE_BR) {
      printk("%s :: Port %d is not BR PHY port\r\n", __FUNCTION__, port);
      return errno;
    }

    sal_memset(&port_info, 0x00, sizeof(mgmt_port_info_t));
    mgmt_port_info_get(info, port, &port_info);

    if (port_info.link_status  == LINK_DOWN) {
      printk("%s :: Port %d is link down\r\n", __FUNCTION__, port);
      return errno;
    }

    sal_memset(cmd, 0, sizeof(mgmt_command_t));
    sal_memset(reply, 0, sizeof(mgmt_reply_t));

    cmd->cmd = uswap32(MGMT_AVB);
    cmd->u.avb.avb_cmd = uswap32(MGMT_AVB_PORT_LINK_SQI_GET);
    cmd->u.avb.u.mgmt_port_link_sqi.port = uswap32(port);
    cmd->len = uswap32(MGMT_AVB_COMMAND_LEN + sizeof(cmd->u.avb.u.mgmt_port_link_sqi));

    errno = mgmt_cmd_reply(info, cmd, reply, 0);
    if (errno){
        return errno;
    }

    *sqi = uswap32(reply->u.avb.mgmt_port_link_sqi.sqi_reg_val);
    return 0;
}

/*
 * @api
 * mgmt_switch_age_time_set
 *
 * @brief
 * Set the bridge age time
 *
 * @param=info - management info
 * @param=switch age time in seconds
 *
 * @returns 0 on success, !0 on error
 */
int32 mgmt_switch_age_time_set(mgmt_info_t *info, uint32 age_time)
{
    int32 errno = -1;
    mgmt_command_t cmd_packet;
    mgmt_command_t *cmd = &cmd_packet;
    mgmt_reply_t reply_packet;
    mgmt_reply_t *reply = &reply_packet;

    if (info == NULL) {
      printk("%s :: mgmt_info_t pointer is NULL\r\n", __FUNCTION__);
      return errno;
    }

    if (age_time > MGMT_SWITCH_AGE_TIME_MAX) {
      printk("%s :: Invalid switch age time set %d\r\n",__FUNCTION__,  age_time);
      return errno;
    }

    sal_memset(cmd, 0, sizeof(mgmt_command_t));
    sal_memset(reply, 0, sizeof(mgmt_reply_t));

    cmd->cmd = uswap32(MGMT_AVB);
    cmd->u.avb.avb_cmd = uswap32(MGMT_AVB_SWITCH_AGE_TIME_SET);
    cmd->u.avb.u.switch_age_time.value = uswap32 (age_time);
    cmd->len = uswap32(MGMT_AVB_COMMAND_LEN + sizeof(cmd->u.avb.u.switch_age_time));

    errno = mgmt_cmd_reply(info, cmd, reply, 0);
    if (errno) {
        return errno;
    }
    return 0;
}

/*
 * @api
 * mgmt_switch_age_time_get
 *
 * @brief
 * Get the bridge age time
 *
 * @param=info - management info
 * @param=switch age time in seconds
 *
 * @returns 0 on success, !0 on error
 */
int32 mgmt_switch_age_time_get(mgmt_info_t *info, uint32 *age_time)
{
    int32 errno = -1;
    mgmt_command_t cmd_packet;
    mgmt_command_t *cmd = &cmd_packet;
    mgmt_reply_t reply_packet;
    mgmt_reply_t *reply = &reply_packet;

    if ((info == NULL) || (age_time == NULL)) {
      printk("%s :: Invalid input param(info =0x%x and age_time = 0x%x)\r\n",
	                                __FUNCTION__, (unsigned int)info, (unsigned int)age_time);
      return errno;
    }

    sal_memset(cmd, 0, sizeof(mgmt_command_t));
    sal_memset(reply, 0, sizeof(mgmt_reply_t));

    cmd->cmd = uswap32(MGMT_AVB);
    cmd->u.avb.avb_cmd = uswap32(MGMT_AVB_SWITCH_AGE_TIME_GET);
    cmd->len = uswap32(MGMT_AVB_COMMAND_LEN);

    errno = mgmt_cmd_reply(info, cmd, reply, 0);
    if (errno) {
        return errno;
    }

    *age_time = uswap32(reply->u.avb.switch_age_time.value);
    return 0;
}

/*
 * @api
 * mgmt_port_jumbo_frame_mode_set
 *
 * @brief
 * Set the port jumbo frame mode
 *
 * @param=info - management info
 * @param=port - port to get the information
 * @param=jumboe frame mode  0=disable, 1=enable
 *
 * @returns 0 on success, !0 on error
 */
int32 mgmt_port_jumbo_frame_mode_set(mgmt_info_t *info, uint32 port, uint32 jumbo_frame_mode)
{
    int32 errno = -1;
    mgmt_command_t cmd_packet;
    mgmt_command_t *cmd = &cmd_packet;
    mgmt_reply_t reply_packet;
    mgmt_reply_t *reply = &reply_packet;

    if (info == NULL) {
      printk("%s :: mgmt_info_t pointer is NULL\r\n", __FUNCTION__);
      return errno;
    }

    if ((IS_EXTERNAL_INTF_INVALID(port))) {
      printk("%s :: Invalid port number %d\r\n", __FUNCTION__, port);
      return errno;
    }

    if ((jumbo_frame_mode != ENABLED) && (jumbo_frame_mode != DISABLED)) {
      printk("%s :: Invalid jumbo_frame_mode configuration\r\n", __FUNCTION__);
      return errno;
    }

    sal_memset(cmd, 0, sizeof(mgmt_command_t));
    sal_memset(reply, 0, sizeof(mgmt_reply_t));

    cmd->cmd = uswap32(MGMT_AVB);
    cmd->u.avb.avb_cmd = uswap32(MGMT_AVB_PORT_JUMBO_MODE_SET);
    cmd->u.avb.u.port_jumbo_frame_mode.num = uswap32(port);
    cmd->u.avb.u.port_jumbo_frame_mode.value = uswap32(jumbo_frame_mode);
    cmd->len = uswap32(MGMT_AVB_COMMAND_LEN + sizeof(cmd->u.avb.u.port_jumbo_frame_mode));

    errno = mgmt_cmd_reply(info, cmd, reply, 0);
    if (errno) {
        return errno;
    }
    return 0;
}
/*
 * @api
 * mgmt_port_dumbfwd_mode_get
 *
 * @brief
 * Get the dumbfwd mode
 *
 * @param=info - management info
 * @param=dumbfwd_mode - to get the information
 *
 * @returns 0 on success, !0 on error
 */
int32 mgmt_port_dumbfwd_mode_get(mgmt_info_t *info, uint32 *dumbfwd_mode)
{
    int32 errno = -1;
    mgmt_command_t cmd_packet;
    mgmt_command_t *cmd = &cmd_packet;
    mgmt_reply_t reply_packet;
    mgmt_reply_t *reply = &reply_packet;

    if ((info == NULL) || (dumbfwd_mode == NULL)) {
      printk("%s :: Invalid input parameters(info =0x%x and dumbfwd_mode =0x%x \r\n",
	                                 __FUNCTION__, (unsigned int)info, (unsigned int)dumbfwd_mode);
      return errno;
    }

    sal_memset(cmd, 0, sizeof(mgmt_command_t));
    sal_memset(reply, 0, sizeof(mgmt_reply_t));

    cmd->cmd = uswap32(MGMT_AVB);
    cmd->u.avb.avb_cmd = uswap32(MGMT_AVB_PORT_DUMBFWD_MODE_GET);
    cmd->len = uswap32(MGMT_AVB_COMMAND_LEN);

    errno = mgmt_cmd_reply(info, cmd, reply, 0);
    if (errno) {
        return errno;
    }
    *dumbfwd_mode = uswap32(reply->u.avb.dumbfwd_status.dumbfwd_mode);
    return 0;
}

/*
 * @api
 * mgmt_port_dumbfwd_mode_set
 *
 * @brief
 * Set the dumbfwd mode
 *
 * @param=info - management info
 * @param=dumbfwd_mode - to set dumbfwd mode
 *
 * @returns 0 on success, !0 on error
 */
int32 mgmt_port_dumbfwd_mode_set(mgmt_info_t *info, uint32 dumbfwd_mode)
{
    int32 errno = -1;
    mgmt_command_t cmd_packet;
    mgmt_command_t *cmd = &cmd_packet;
    mgmt_reply_t reply_packet;
    mgmt_reply_t *reply = &reply_packet;

    if (info == NULL) {
      printk("%s :: mgmt_info_t pointer is NULL\r\n", __FUNCTION__);
      return errno;
    }

    if ((dumbfwd_mode != ENABLED) && (dumbfwd_mode != DISABLED)) {
      printk("%s :: Invalid value for dumbfwd_mode\r\n", __FUNCTION__);
      return errno;
    }

    sal_memset(cmd, 0, sizeof(mgmt_command_t));
    sal_memset(reply, 0, sizeof(mgmt_reply_t));

    cmd->cmd = uswap32(MGMT_AVB);
    cmd->u.avb.avb_cmd = uswap32(MGMT_AVB_PORT_DUMBFWD_MODE_SET);
    cmd->len = uswap32(MGMT_AVB_COMMAND_LEN);
    cmd->u.avb.u.dumbfwd_config.dumbfwd_mode = uswap32(dumbfwd_mode);
    cmd->len = uswap32(MGMT_AVB_COMMAND_LEN + sizeof(cmd->u.avb.u.dumbfwd_config));

    errno = mgmt_cmd_reply(info, cmd, reply, 0);
    if (errno) {
        return errno;
    }
    return 0;
}


/*
 * @api
 * mgmt_port_jumbo_frame_mode_get
 *
 * @brief
 * Get the port jumbo frame mode
 *
 * @param=info - management info
 * @param=port - port to get the information
 * @param=jumboe frame mode  0=disable, 1=enable
 *
 * @returns 0 on success, !0 on error
 */
int32 mgmt_port_jumbo_frame_mode_get(mgmt_info_t *info, uint32 port, uint32 *jumbo_frame_mode)
{
    int32 errno = -1;
    mgmt_command_t cmd_packet;
    mgmt_command_t *cmd = &cmd_packet;
    mgmt_reply_t reply_packet;
    mgmt_reply_t *reply = &reply_packet;

    if ((info == NULL) || (jumbo_frame_mode == NULL)) {
      printk("%s :: Atleast one of the input pointer argument is NULL\r\n", __FUNCTION__);
      return errno;
    }

    if ((IS_EXTERNAL_INTF_INVALID(port))) {
      printk("%s :: Invalid port number %d\r\n", __FUNCTION__, port);
      return errno;
    }

    sal_memset(cmd, 0, sizeof(mgmt_command_t));
    sal_memset(reply, 0, sizeof(mgmt_reply_t));

    cmd->cmd = uswap32(MGMT_AVB);
    cmd->u.avb.avb_cmd = uswap32(MGMT_AVB_PORT_JUMBO_MODE_GET);
    cmd->u.avb.u.port_jumbo_frame_mode.num = uswap32(port);
    cmd->len = uswap32(MGMT_AVB_COMMAND_LEN + sizeof(cmd->u.avb.u.port_jumbo_frame_mode));

    errno = mgmt_cmd_reply(info, cmd, reply, 0);
    if (errno) {
        return errno;
    }
    *jumbo_frame_mode = uswap32(reply->u.avb.port_jumbo_frame_mode.value);
    return 0;
}

/*
 * @api
 * mgmt_buffer_info
 *
 * @brief
 * Retrieve information pertaining to a buffer
 *
 * @param=info - management info
 * @param=type - type of the buffer
 * @param=addr - address information to fetch
 * @param=size - size information to fetch
 *
 * @returns 0 on success, !0 on error
 */
int32 mgmt_buffer_info(mgmt_info_t *info, uint32_t type, uint32 *addr, uint32 *size)
{
    int32 errno = -1;
    mgmt_command_t cmd_packet;
    mgmt_command_t *cmd = &cmd_packet;
    mgmt_reply_t reply_packet;
    mgmt_reply_t *reply = &reply_packet;

    if (info == NULL) {
      printk("%s :: mgmt_info_t pointer is NULL\r\n", __FUNCTION__);
      return errno;
    }

    sal_memset(cmd, 0, sizeof(mgmt_command_t));
    sal_memset(reply, 0, sizeof(mgmt_reply_t));

    cmd->cmd = uswap32(MGMT_AVB);
    cmd->u.avb.avb_cmd = uswap32(MGMT_AVB_BUFF_INFO);
    cmd->u.avb.u.buff_info.type = uswap32(type);
    cmd->len = uswap32(MGMT_AVB_COMMAND_LEN);

    errno = mgmt_cmd_reply(info, cmd, reply, 0);
    if (errno) {
        return errno;
    }

    /* Get pointer and length */
    *addr = uswap32(reply->u.avb.buff_info.addr);
    *size = uswap32(reply->u.avb.buff_info.size);

    return 0;
}

/*
 * @api
 * mgmt_avnu_global_admin_mode_set
 *
 * @brief
 * Set the 802.1AS global admin mode
 *
 * @param=info - management info
 * @param=mode - mode to set
 *
 * @returns 0 on success, !0 on error
 */
int32 mgmt_avnu_global_admin_mode_set(mgmt_info_t *info, uint32 mode)
{
    int32 errno = -1;
    mgmt_command_t cmd_packet;
    mgmt_command_t *cmd = &cmd_packet;
    mgmt_reply_t reply_packet;
    mgmt_reply_t *reply = &reply_packet;

    if (info == NULL) {
      printk("%s :: mgmt_info_t pointer is NULL\r\n", __FUNCTION__);
      return errno;
    }

    if ((mode != MGMT_AVB_DOT1AS_ADMIN_ENABLE) &&
        (mode != MGMT_AVB_DOT1AS_ADMIN_DISABLE)) {
      printk("%s :: Invalid avnu global admin mode value %d\r\n", __FUNCTION__, mode);
      return errno;
    }

    sal_memset(cmd, 0, sizeof(mgmt_command_t));
    sal_memset(reply, 0, sizeof(mgmt_reply_t));

    cmd->cmd = uswap32(MGMT_AVB);
    cmd->u.avb.avb_cmd = uswap32( MGMT_AVB_AVNU_GLOBAL_ADMIN_MODE_SET);
    cmd->u.avb.u.avnu_global_admin_mode.mode = uswap32(mode);
    cmd->len = uswap32(MGMT_AVB_COMMAND_LEN + sizeof(cmd->u.avb.u.as_mode));

    errno = mgmt_cmd_reply(info, cmd, reply, 0);
    if (errno) {
        return errno;
    }
    return 0;
}

/*
 * @api
 * mgmt_avnu_port_status_get
 *
 * @brief
 * Get the 802.1AS port information
 *
 * @param=info - management info
 * @param=port - port number to get the info
 * @param=avnu_port_status - structure pointer to fill the info
 *
 * @returns 0 on success, !0 on error
 */
int32 mgmt_avnu_port_status_get(mgmt_info_t *info, uint32 port,
        mgmt_avnu_port_status_t *avnu_port_status)
{
	int32 errno = -1;
	mgmt_command_t cmd_packet;
	mgmt_command_t *cmd = &cmd_packet;
	mgmt_reply_t reply_packet;
	mgmt_reply_t *reply = &reply_packet;

	if ((info == NULL) || (avnu_port_status == NULL)) {
		printk("%s :: Invalid input parameter is NULL\r\n", __FUNCTION__);
		return errno;
	}

	if ((IS_EXTERNAL_INTF_INVALID(port))) {
		printk("%s :: Invalid port number %d\r\n", __FUNCTION__, port);
		return errno;
	}
	sal_memset(cmd, 0, sizeof(mgmt_command_t));
	sal_memset(reply, 0, sizeof(mgmt_reply_t));

	cmd->cmd = uswap32(MGMT_AVB);
	cmd->u.avb.avb_cmd = uswap32(MGMT_AVB_AVNU_PORT_STATUS);
	cmd->u.avb.u.as_port_info.num = uswap32(port);
	cmd->len = uswap32(MGMT_AVB_COMMAND_LEN + sizeof(cmd->u.avb.u.as_port_info));

	errno = mgmt_cmd_reply(info, cmd, reply, 0);
	if (errno) {
		return errno;
	}
	avnu_port_status->dot1as_capable = uswap32(reply->u.avb.avnu_port_status.dot1as_capable);  /* Is port 802.1AS capable? */
	avnu_port_status->peer_delay = uswap32(reply->u.avb.avnu_port_status.peer_delay);      /* Current peer delay */
	avnu_port_status->nbr_rate_ratio = uswap32(reply->u.avb.avnu_port_status.nbr_rate_ratio);  /* Current neighbor rate ratio */
	avnu_port_status->is_measuring_pdelay = uswap32(reply->u.avb.avnu_port_status.is_measuring_pdelay); /* Is port measuring PDELAY? */
	avnu_port_status->allowed_lost_responses = uswap32(reply->u.avb.avnu_port_status.allowed_lost_responses); /* Number PDELAY responses allowed */
	avnu_port_status->port_role = uswap32(reply->u.avb.avnu_port_status.port_role);       /* Port role determined by BMCA */
	strncpy(avnu_port_status->sync_interval,reply->u.avb.avnu_port_status.sync_interval,19);   /* Current SYNC TX interval */
	strncpy(avnu_port_status->pdelay_interval,reply->u.avb.avnu_port_status.pdelay_interval,19);   /* Current PDELAY TX interval */
	avnu_port_status->sync_receipt_timeout = uswap32(reply->u.avb.avnu_port_status.sync_receipt_timeout);  /* Current SYNC RX timeout */
	avnu_port_status->num_lost_responses = uswap32(reply->u.avb.avnu_port_status.num_lost_responses);
	swap_str(avnu_port_status->pdelay_req_state,reply->u.avb.avnu_port_status.pdelay_req_state,20);
	swap_str(avnu_port_status->sync_receive_state,reply->u.avb.avnu_port_status.sync_receive_state,20);
	avnu_port_status->sync_tx_intl = uswap32(reply->u.avb.avnu_port_status.sync_tx_intl);
	avnu_port_status->sync_rx_intl = uswap32(reply->u.avb.avnu_port_status.sync_rx_intl);
	avnu_port_status->pDelaySeqId = uswap32(reply->u.avb.avnu_port_status.pDelaySeqId);
	avnu_port_status->isAVnuPdelayConfigSaved = uswap32(reply->u.avb.avnu_port_status.isAVnuPdelayConfigSaved);
	memcpy(avnu_port_status->partnerIdClock,
           reply->u.avb.avnu_port_status.partnerIdClock,
           sizeof(avnu_port_status->partnerIdClock));
	avnu_port_status->partnerIdPort = uswap32(reply->u.avb.avnu_port_status.partnerIdPort);
	avnu_port_status->syncRxInfo.rxIntfIndex = uswap32(reply->u.avb.avnu_port_status.syncRxInfo.rxIntfIndex);
	avnu_port_status->syncRxInfo.lastSeqId = uswap32(reply->u.avb.avnu_port_status.syncRxInfo.lastSeqId);
	avnu_port_status->syncRxInfo.followupCorrectionField_u = uswap32(reply->u.avb.avnu_port_status.syncRxInfo.followupCorrectionField_u);
	avnu_port_status->syncRxInfo.followupCorrectionField_l = uswap32(reply->u.avb.avnu_port_status.syncRxInfo.followupCorrectionField_l);
	avnu_port_status->syncRxInfo.pduInterval = uswap32(reply->u.avb.avnu_port_status.syncRxInfo.pduInterval);
	avnu_port_status->syncRxInfo.preciseOriginTimestamp.sec_u = uswap32(reply->u.avb.avnu_port_status.syncRxInfo.preciseOriginTimestamp.sec_u);
	avnu_port_status->syncRxInfo.preciseOriginTimestamp.sec_l = uswap32(reply->u.avb.avnu_port_status.syncRxInfo.preciseOriginTimestamp.sec_l);
	avnu_port_status->syncRxInfo.preciseOriginTimestamp.ns = uswap32(reply->u.avb.avnu_port_status.syncRxInfo.preciseOriginTimestamp.ns);
	avnu_port_status->syncRxInfo.syncRxTimestamp.sec_u = uswap32(reply->u.avb.avnu_port_status.syncRxInfo.syncRxTimestamp.sec_u);
	avnu_port_status->syncRxInfo.syncRxTimestamp.sec_l = uswap32(reply->u.avb.avnu_port_status.syncRxInfo.syncRxTimestamp.sec_l);
	avnu_port_status->syncRxInfo.syncRxTimestamp.ns = uswap32(reply->u.avb.avnu_port_status.syncRxInfo.syncRxTimestamp.ns);
	avnu_port_status->syncRxInfo.gmTimeBaseIndicator = uswap32(reply->u.avb.avnu_port_status.syncRxInfo.gmTimeBaseIndicator);
	memcpy(avnu_port_status->syncRxInfo.lastGmPhaseChange,
           reply->u.avb.avnu_port_status.syncRxInfo.lastGmPhaseChange,
           sizeof(avnu_port_status->syncRxInfo.lastGmPhaseChange));
	avnu_port_status->syncRxInfo.lastGmFreqChange = uswap32(reply->u.avb.avnu_port_status.syncRxInfo.lastGmFreqChange);
	avnu_port_status->syncInfoAvailable = uswap32(reply->u.avb.avnu_port_status.syncInfoAvailable);
	avnu_port_status->syncTxSeqId = uswap32(reply->u.avb.avnu_port_status.syncTxSeqId);
	avnu_port_status->signalingSeqId = uswap32(reply->u.avb.avnu_port_status.signalingSeqId);
	return 0;
}

/*
 * @api
 * mgmt_avnu_port_stats_get
 *
 * @brief
 * Get the 802.1AS statistics for a port
 *
 * @param=info - management info
 * @param=port - port number
 * @param=avnu_port_stats - structure pointer to fill the statistics
 *
 * @returns 0 on success, !0 on error
 */
int32 mgmt_avnu_port_stats_get(mgmt_info_t *info, uint32 port,
        mgmt_avnu_port_stats_t *avnu_port_stats)
{
    int32 errno = -1;
    mgmt_command_t cmd_packet;
    mgmt_command_t *cmd = &cmd_packet;
    mgmt_reply_t reply_packet;
    mgmt_reply_t *reply = &reply_packet;

    if ((info == NULL) || (avnu_port_stats == NULL)) {
      printk("%s :: Invalid input param(info = 0x%x,avnu_port_stats = 0x%x)\r\n",
	                                          __FUNCTION__, (unsigned int)info, (unsigned int)avnu_port_stats);
      return errno;
    }

    if (IS_EXTERNAL_INTF_INVALID(port)){
      printk("%s :: Invalid port number %d\r\n", __FUNCTION__, port);
      return errno;
    }

    sal_memset(cmd, 0, sizeof(mgmt_command_t));
    sal_memset(reply, 0, sizeof(mgmt_reply_t));

    cmd->cmd = uswap32(MGMT_AVB);
    cmd->u.avb.avb_cmd = uswap32(MGMT_AVB_AVNU_STATS_GET);
    cmd->u.avb.u.as_port_stats.num = uswap32(port);
    cmd->len = uswap32(MGMT_AVB_COMMAND_LEN + sizeof(cmd->u.avb.u.as_port_stats));
    errno = mgmt_cmd_reply(info, cmd, reply, 0);
    if (errno) {
        return errno;
    }

    avnu_port_stats->sync_tx = uswap32(reply->u.avb.avnu_port_stats.sync_tx);
    avnu_port_stats->sync_rx = uswap32(reply->u.avb.avnu_port_stats.sync_rx);
    avnu_port_stats->followup_tx = uswap32(reply->u.avb.avnu_port_stats.followup_tx);
    avnu_port_stats->followup_rx = uswap32(reply->u.avb.avnu_port_stats.followup_rx);
    avnu_port_stats->pdelay_req_tx = uswap32(reply->u.avb.avnu_port_stats.pdelay_req_tx);
    avnu_port_stats->pdelay_req_rx = uswap32(reply->u.avb.avnu_port_stats.pdelay_req_rx);
    avnu_port_stats->pdelay_resp_tx = uswap32(reply->u.avb.avnu_port_stats.pdelay_resp_tx);
    avnu_port_stats->pdelay_resp_rx = uswap32(reply->u.avb.avnu_port_stats.pdelay_resp_rx);
    avnu_port_stats->pdelay_followup_tx = uswap32(reply->u.avb.avnu_port_stats.pdelay_followup_tx);
    avnu_port_stats->pdelay_followup_rx = uswap32(reply->u.avb.avnu_port_stats.pdelay_followup_rx);
    avnu_port_stats->sync_rx_timeouts = uswap32(reply->u.avb.avnu_port_stats.sync_rx_timeouts);
    avnu_port_stats->sync_discards = uswap32(reply->u.avb.avnu_port_stats.sync_discards);
    avnu_port_stats->pdelay_rx_timeouts = uswap32(reply->u.avb.avnu_port_stats.pdelay_rx_timeouts);
    avnu_port_stats->pdelay_discards = uswap32(reply->u.avb.avnu_port_stats.pdelay_discards);
    avnu_port_stats->bad_headers = uswap32(reply->u.avb.avnu_port_stats.bad_headers);
    avnu_port_stats->pdelay_lost_resp_exceeded = uswap32(reply->u.avb.avnu_port_stats.pdelay_lost_resp_exceeded);
    avnu_port_stats->followup_rx_timeouts = uswap32(reply->u.avb.avnu_port_stats.followup_rx_timeouts);
    avnu_port_stats->followup_rx_discards = uswap32(reply->u.avb.avnu_port_stats.followup_rx_discards);
    avnu_port_stats->sync_tx_timeouts = uswap32(reply->u.avb.avnu_port_stats.sync_tx_timeouts);
    avnu_port_stats->pdelay_req_rx_discards = uswap32(reply->u.avb.avnu_port_stats.pdelay_req_rx_discards);
    avnu_port_stats->pdelay_resp_rx_discards = uswap32(reply->u.avb.avnu_port_stats.pdelay_resp_rx_discards);
    avnu_port_stats->bad_pdelay_values = uswap32(reply->u.avb.avnu_port_stats.bad_pdelay_values);
    avnu_port_stats->signaling_rx_count = uswap32(reply->u.avb.avnu_port_stats.signaling_rx_count);
    avnu_port_stats->signaling_tx_count = uswap32(reply->u.avb.avnu_port_stats.signaling_tx_count);
    avnu_port_stats->signaling_rx_discards = uswap32(reply->u.avb.avnu_port_stats.signaling_rx_discards);
    avnu_port_stats->sync_too_fast = uswap32(reply->u.avb.avnu_port_stats.sync_too_fast);
    avnu_port_stats->tx_errors = uswap32(reply->u.avb.avnu_port_stats.tx_errors);
    avnu_port_stats->ts_errors = uswap32(reply->u.avb.avnu_port_stats.ts_errors);
    avnu_port_stats->ptp_discards = uswap32(reply->u.avb.avnu_port_stats.ptp_discards);

    return 0;
}

/*
   @api
 * mgmt_avnu_port_stats_clear
 *
 * @brief
 * Clear the 802.1AS stats for a port
 *
 * @param=info - management info
 * @param=port - port number
 *
 * @returns 0 on success, !0 on error
 */
int32 mgmt_avnu_port_stats_clear(mgmt_info_t *info, uint32 port)
{
    int32 errno = -1;
    mgmt_command_t cmd_packet;
    mgmt_command_t *cmd = &cmd_packet;
    mgmt_reply_t reply_packet;
    mgmt_reply_t *reply = &reply_packet;

    if (info == NULL) {
      printk("%s :: mgmt_info_t pointer is NULL\r\n", __FUNCTION__);
      return errno;
    }

    if (IS_EXTERNAL_INTF_INVALID(port)){
      printk("%s :: Invalid port number %d\r\n", __FUNCTION__, port);
      return errno;
    }

    sal_memset(cmd, 0, sizeof(mgmt_command_t));
    sal_memset(reply, 0, sizeof(mgmt_reply_t));

    cmd->cmd = uswap32(MGMT_AVB);
    cmd->u.avb.avb_cmd = uswap32(MGMT_AVB_AVNU_STATS_CLEAR);
    cmd->u.avb.u.as_port_stats.num = uswap32(port);
    cmd->len = uswap32(MGMT_AVB_COMMAND_LEN + sizeof(cmd->u.avb.u.as_port_stats));
    errno = mgmt_cmd_reply(info, cmd, reply, 0);
    if (errno) {
        return errno;
    }
    return 0;
}

/*
 * @api
 * mgmt_avnu_port_num_lstresp_set
 *
 * @brief
 * Set the value of number of allowed lost PDELAY responses
 *
 * @param=info - management info
 * @param=port - port number to set
 * @param=last_resp - value
 *
 * @returns 0 on success, !0 on error
 */
int32 mgmt_avnu_port_num_lstresp_set(mgmt_info_t *info, uint32 port, uint32 lst_resp)
{
    int32 errno = -1;
    mgmt_command_t cmd_packet;
    mgmt_command_t *cmd = &cmd_packet;
    mgmt_reply_t reply_packet;
    mgmt_reply_t *reply = &reply_packet;

    if (info == NULL) {
      printk("%s :: mgmt_info_t pointer is NULL\r\n", __FUNCTION__);
      return errno;
    }

    if (IS_EXTERNAL_INTF_INVALID(port)){
      printk("%s :: Invalid port number %d\r\n", __FUNCTION__, port);
      return errno;
    }

    if (lst_resp > MGMT_AVB_PDELAY_ALLOWED_LOST_RESP_MAX) {
      printk("%s :: Exceeds pdelay allowed responce max value %d\r\n", __FUNCTION__, lst_resp);
      return errno;
    }

    sal_memset(cmd, 0, sizeof(mgmt_command_t));
    sal_memset(reply, 0, sizeof(mgmt_reply_t));

    cmd->cmd = uswap32(MGMT_AVB);
    cmd->u.avb.avb_cmd = uswap32(MGMT_AVB_AVNU_NUM_LSTRESP_SET);
    cmd->u.avb.u.as_allowed_lst_resp.num =  uswap32(port);
    cmd->u.avb.u.as_allowed_lst_resp.value = uswap32(lst_resp);
    cmd->len = uswap32(MGMT_AVB_COMMAND_LEN + sizeof(cmd->u.avb.u.as_allowed_lst_resp));

    errno = mgmt_cmd_reply(info, cmd, reply, 0);
    if (errno) {
        return errno;
    }
    return 0;
}


/*
 * @api
 * mgmt_vlan_create
 *
 * @brief
 * Create the given VLAN
 *
 * @param=info - management info
 * @param=vlan - vlan id to create
 *
 * @returns 0 on success, !0 on error
 */
int32 mgmt_vlan_create(mgmt_info_t *info, uint32 vlan)
{
    int32 errno = -1;
    mgmt_command_t cmd_packet;
    mgmt_command_t *cmd = &cmd_packet;
    mgmt_reply_t reply_packet;
    mgmt_reply_t *reply = &reply_packet;

    if (info == NULL) {
      printk("%s :: mgmt_info_t pointer is NULL\r\n", __FUNCTION__);
      return errno;
    }

    if (!MGMT_AVB_VLAN_VALID(vlan)){
        printk("Invalid VLAN id %d\r\n", vlan);
        return errno;
    }

    sal_memset(cmd, 0, sizeof(mgmt_command_t));
    sal_memset(reply, 0, sizeof(mgmt_reply_t));

    cmd->cmd = uswap32(MGMT_AVB);
    cmd->u.avb.avb_cmd = uswap32(MGMT_AVB_VLAN_CREATE);
    cmd->u.avb.u.vlan_create.id = uswap32(vlan);
    cmd->len = uswap32(MGMT_AVB_COMMAND_LEN + sizeof(cmd->u.avb.u.vlan_create));

    errno = mgmt_cmd_reply(info, cmd, reply, 0);
    if (errno) {
        return errno;
    }
    return 0;
}

/*
 * @api
 * mgmt_vlan_delete
 *
 * @brief
 * Delete the given VLAN
 *
 * @param=info - management info
 * @param=vlan - vlan id to create
 *
 * @returns 0 on success, !0 on error
 */
int32 mgmt_vlan_delete(mgmt_info_t *info, uint32 vlan)
{
    int32 errno = -1;
    mgmt_command_t cmd_packet;
    mgmt_command_t *cmd = &cmd_packet;
    mgmt_reply_t reply_packet;
    mgmt_reply_t *reply = &reply_packet;

    if (info == NULL) {
      printk("%s :: mgmt_info_t pointer is NULL\r\n", __FUNCTION__);
      return errno;
    }

    if (!MGMT_AVB_VLAN_VALID(vlan)){
      printk("Invalid VLAN id %d\r\n", vlan);
      return errno;
    }

    sal_memset(cmd, 0, sizeof(mgmt_command_t));
    sal_memset(reply, 0, sizeof(mgmt_reply_t));

    cmd->cmd = uswap32(MGMT_AVB);
    cmd->u.avb.avb_cmd = uswap32(MGMT_AVB_VLAN_DELETE);
    cmd->u.avb.u.vlan_delete.id = uswap32(vlan);
    cmd->len = uswap32(MGMT_AVB_COMMAND_LEN + sizeof(cmd->u.avb.u.vlan_delete));

    errno = mgmt_cmd_reply(info, cmd, reply, 0);
    if (errno) {
        return errno;
    }
    return 0;
}

/*
 * @api
 * mgmt_vlan_portadd
 *
 * @brief
 * Add the given port to the given VLAN, and enable/disable tagging on it
 *
 * @param=info - management info
 * @param=vlan - vlan id
 * @param=port - port number to add
 * @param=tag - tag
 *
 * @returns 0 on success, !0 on error
 */
int32 mgmt_vlan_portadd(mgmt_info_t *info, uint32 vlan, uint32 port, uint32 tag)
{
    int32 errno = -1;
    mgmt_command_t cmd_packet;
    mgmt_command_t *cmd = &cmd_packet;
    mgmt_reply_t reply_packet;
    mgmt_reply_t *reply = &reply_packet;

    if (info == NULL) {
      printk("%s :: mgmt_info_t pointer is NULL\r\n", __FUNCTION__);
      return errno;
    }

    if (!MGMT_AVB_VLAN_VALID(vlan)){
      printk("Invalid VLAN id %d\r\n", vlan);
      return errno;
    }
    if (IS_EXTERNAL_INTF_INVALID(port)){
      printk("Invalid port number %d\r\n", port);
      return errno;
    }
    if ((tag != MGMT_AVB_VLAN_UNTAG) && (tag != MGMT_AVB_VLAN_TAG)) {
      printk("Invalid tag/untag id %d\r\n", tag);
      return errno;
    }

    sal_memset(cmd, 0, sizeof(mgmt_command_t));
    sal_memset(reply, 0, sizeof(mgmt_reply_t));

    cmd->cmd = uswap32(MGMT_AVB);
    cmd->u.avb.avb_cmd = uswap32(MGMT_AVB_VLAN_PORT_ADD);
    cmd->u.avb.u.vlan_port_add.id = uswap32(vlan);
    cmd->u.avb.u.vlan_port_add.num = uswap32(port);
    cmd->u.avb.u.vlan_port_add.is_tagged = uswap32(tag);
    cmd->len = uswap32(MGMT_AVB_COMMAND_LEN + sizeof(cmd->u.avb.u.vlan_port_add));

    errno = mgmt_cmd_reply(info, cmd, reply, 0);
    if (errno) {
        return errno;
    }
    return 0;
}


/*
 * @api
 * mgmt_vlan_portdel
 *
 * @brief
 * Delete the given port from the given VLAN
 *
 * @param=info - management info
 * @param=vlan - vlan id to create
 * @param=port - port to remove
 *
 * @returns 0 on success, !0 on error
 */
int32 mgmt_vlan_portdel(mgmt_info_t *info, uint32 vlan, uint32 port)
{
    int32 errno = -1;
    mgmt_command_t cmd_packet;
    mgmt_command_t *cmd = &cmd_packet;
    mgmt_reply_t reply_packet;
    mgmt_reply_t *reply = &reply_packet;

    if (info == NULL) {
      printk("%s :: mgmt_info_t pointer is NULL\r\n", __FUNCTION__);
      return errno;
    }

    sal_memset(cmd, 0, sizeof(mgmt_command_t));
    sal_memset(reply, 0, sizeof(mgmt_reply_t));

    if (!MGMT_AVB_VLAN_VALID(vlan)){
      printk("Invalid VLAN id %d\r\n", vlan);
      return errno;
    }

    if (IS_EXTERNAL_INTF_INVALID(port)){
      printk("Invalid port number %d\r\n", port);
      return errno;
    }

    cmd->cmd = uswap32(MGMT_AVB);
    cmd->u.avb.avb_cmd = uswap32(MGMT_AVB_VLAN_PORT_REMOVE);
    cmd->u.avb.u.vlan_port_del.id = uswap32(vlan);
    cmd->u.avb.u.vlan_port_del.num = uswap32(port);
    cmd->len = uswap32(MGMT_AVB_COMMAND_LEN + sizeof(cmd->u.avb.u.vlan_port_del));


    errno = mgmt_cmd_reply(info, cmd, reply, 0);
    if (errno) {
        return errno;
    }
    return 0;
}

/*
 * @api
 * mgmt_vlan_pvid_set
 *
 * @brief
 * Configure the untagged VLAN for the given port.
 *
 * @param=info - management info
 * @param=port - port number
 * @param=pvid - pvid
 *
 * @returns 0 on success, !0 on error
 */
int32 mgmt_vlan_pvid_set(mgmt_info_t *info, uint32 port, uint32 pvid)
{
    int32 errno = -1;
    mgmt_command_t cmd_packet;
    mgmt_command_t *cmd = &cmd_packet;
    mgmt_reply_t reply_packet;
    mgmt_reply_t *reply = &reply_packet;

    if (info == NULL) {
      printk("%s :: mgmt_info_t pointer is NULL\r\n", __FUNCTION__);
      return errno;
    }

    if (!MGMT_AVB_VLAN_VALID(pvid)){
      printk("Invalid PVID %d\r\n", pvid);
      return errno;
    }

    if (IS_EXTERNAL_INTF_INVALID(port)){
      printk("Invalid port number %d\r\n", port);
      return errno;
    }

    sal_memset(cmd, 0, sizeof(mgmt_command_t));
    sal_memset(reply, 0, sizeof(mgmt_reply_t));

    cmd->cmd = uswap32(MGMT_AVB);
    cmd->u.avb.avb_cmd = uswap32(MGMT_AVB_VLAN_PVID_SET);
    cmd->u.avb.u.vlan_pvid.num = uswap32(port);
    cmd->u.avb.u.vlan_pvid.value = uswap32(pvid);
    cmd->len = uswap32(MGMT_AVB_COMMAND_LEN + sizeof(cmd->u.avb.u.vlan_pvid));

    errno = mgmt_cmd_reply(info, cmd, reply, 0);
    if (errno) {
        return errno;
    }
    return 0;
}

/*
 * @api
 * mgmt_vlan_ifilter_set
 *
 * @brief
 * Enable/Disbale VLAN ingress filtering mode for all ports
 *
 * @param=info - management info
 * @param=mode - mode
 *
 * @returns 0 on success, !0 on error
 */
int32 mgmt_vlan_ifilter_set(mgmt_info_t *info, uint32 mode)
{
    int32 errno = -1;
    mgmt_command_t cmd_packet;
    mgmt_command_t *cmd = &cmd_packet;
    mgmt_reply_t reply_packet;
    mgmt_reply_t *reply = &reply_packet;

    if (info == NULL) {
      printk("%s :: mgmt_info_t pointer is NULL\r\n", __FUNCTION__);
      return errno;
    }
    if ((mode != MGMT_AVB_INGRESS_FILTER_MODE_ENABLE) &&
      (mode != MGMT_AVB_INGRESS_FILTER_MODE_DISABLE)) {
        printk("Invalid mode for ingress filtering %d\r\n", mode);
        return errno;
    }

    sal_memset(cmd, 0, sizeof(mgmt_command_t));
    sal_memset(reply, 0, sizeof(mgmt_reply_t));

    cmd->cmd = uswap32(MGMT_AVB);
    cmd->u.avb.avb_cmd = uswap32(MGMT_AVB_VLAN_IFILTER_SET);
    cmd->u.avb.u.vlan_ifilter.num = 0; /* Unused but leave it in case in the future */
    cmd->u.avb.u.vlan_ifilter.value = uswap32(mode);
    cmd->len = uswap32(MGMT_AVB_COMMAND_LEN + sizeof(cmd->u.avb.u.vlan_ifilter));

    errno = mgmt_cmd_reply(info, cmd, reply, 0);
    if (errno) {
        return errno;
    }
    return 0;
}


/*
 * @api
 * mgmt_vlan_priority_set
 *
 * @brief
 * Configure the untagged priority on the given port.
 *
 * @param=info - management info
 * @param=vlan - vlan id to create
 *
 * @returns 0 on success, !0 on error
 */
int32 mgmt_vlan_priority_set(mgmt_info_t *info, uint32 port, uint32 priority)
{
    int32 errno = -1;
    mgmt_command_t cmd_packet;
    mgmt_command_t *cmd = &cmd_packet;
    mgmt_reply_t reply_packet;
    mgmt_reply_t *reply = &reply_packet;

    if (info == NULL) {
      printk("%s :: mgmt_info_t pointer is NULL\r\n", __FUNCTION__);
      return errno;
    }

    if (IS_EXTERNAL_INTF_INVALID(port)){
      printk("Invalid port number %d\r\n", port);
      return errno;
    }

    /*Since priority is unsigned , just checking the upperlimit would suffice*/
    if (priority > MGMT_AVB_DOT1Q_INTF_PRIO_MAX){
      printk("Invalid vlan priority %d\r\n", priority);
      return errno;
    }

    sal_memset(cmd, 0, sizeof(mgmt_command_t));
    sal_memset(reply, 0, sizeof(mgmt_reply_t));

    cmd->cmd = uswap32(MGMT_AVB);
    cmd->u.avb.avb_cmd = uswap32(MGMT_AVB_VLAN_PRIO_SET);
    cmd->u.avb.u.vlan_untag_prio.num = uswap32(port);
    cmd->u.avb.u.vlan_untag_prio.value = uswap32(priority);
    cmd->len = uswap32(MGMT_AVB_COMMAND_LEN + sizeof(cmd->u.avb.u.vlan_untag_prio));

    errno = mgmt_cmd_reply(info, cmd, reply, 0);
    if (errno) {
        return errno;
    }
    return 0;
}

/*
 * @api
 * mgmt_vlan_get
 *
 * @brief
 * Get the VLAN info
 *
 * @param=info - management info
 * @param=vlan - vlan id
 * @param=bitmap - VLAN port membership
 * @param=tag - tagged bitmap
 *
 * @returns 0 on success, !0 on error
 */
int32 mgmt_vlan_get(mgmt_info_t *info, uint32 vlan, uint32 *bitmap, uint32 *tag)
{
    int32 errno = -1;
    mgmt_command_t cmd_packet;
    mgmt_command_t *cmd = &cmd_packet;
    mgmt_reply_t reply_packet;
    mgmt_reply_t *reply = &reply_packet;

    if ((info == NULL) || (tag == NULL) || (bitmap == NULL)) {
      printk("%s :: Invalid input param(info = 0x%x, tag = 0x%x, bitmap = 0x%x)\r\n",
                                        __FUNCTION__, (unsigned int)info, (unsigned int)tag, (unsigned int)bitmap);
      return errno;
    }

    if (!MGMT_AVB_VLAN_VALID(vlan)){
      printk("Invalid VLAN id %d\r\n", vlan);
      return errno;
    }

    sal_memset(cmd, 0, sizeof(mgmt_command_t));
    sal_memset(reply, 0, sizeof(mgmt_reply_t));

    cmd->cmd = uswap32(MGMT_AVB);
    cmd->u.avb.avb_cmd = uswap32(MGMT_AVB_VLAN_GET);
    cmd->u.avb.u.vlan_get.id = uswap32(vlan);
    cmd->len = uswap32(MGMT_AVB_COMMAND_LEN + sizeof(cmd->u.avb.u.vlan_get));

    errno = mgmt_cmd_reply(info, cmd, reply, 0);
    if (errno) {
        return errno;
    }

    *bitmap = uswap32(reply->u.avb.vlan_info.port_bitmap);
    *tag = uswap32(reply->u.avb.vlan_info.tag_bitmap);
    return 0;
}
/*
 * @api
 * mgmt_vlan_port_info
 *
 * @brief
 * Get the vlan per port information
 *
 * @param=info - management info
 * @param=port - port to get the information
 * @param=vlan_info - structure to fill vlan information
 *
 * @returns 0 on success, !0 on error
 */
int32 mgmt_vlan_port_info_get(mgmt_info_t *info, uint32 port, mgmt_vlan_port_info_t *vlan_port_info)
{
    int32 errno = -1;
    mgmt_command_t cmd_packet;
    mgmt_command_t *cmd = &cmd_packet;
    mgmt_reply_t reply_packet;
    mgmt_reply_t *reply = &reply_packet;

    if ((info == NULL) || (vlan_port_info == NULL)) {
      printk("%s :: Invalid input parameter(info = 0x%x,vlan_port_info = 0x%x)\r\n", __FUNCTION__, (unsigned int)info, (unsigned int)vlan_port_info);
      return errno;
    }

    if (IS_EXTERNAL_INTF_INVALID(port)){
      printk("Invalid port number %d\r\n", port);
      return errno;
    }

    sal_memset(cmd, 0, sizeof(mgmt_command_t));
    sal_memset(reply, 0, sizeof(mgmt_reply_t));

    cmd->cmd = uswap32(MGMT_AVB);
    cmd->u.avb.avb_cmd = uswap32(MGMT_AVB_VLAN_PORT_GET);
    cmd->u.avb.u.vlan_port_info.port = uswap32(port);
    cmd->len = uswap32(MGMT_AVB_COMMAND_LEN + sizeof(cmd->u.avb.u.vlan_port_info));

    errno = mgmt_cmd_reply(info, cmd, reply, 0);
    if (errno){
        return errno;
    }

    vlan_port_info->iFilter = uswap32(reply->u.avb.vlan_port_info.iFilter);
    vlan_port_info->pvid = uswap32(reply->u.avb.vlan_port_info.pvid);
    vlan_port_info->priority = uswap32 (reply->u.avb.vlan_port_info.priority);

    return 0;
}

/*
 * @api
 * mgmt_fdb_entry_add
 *
 * @brief
 * Add a forwarding database entry with given MAC/VLAN parameters and set the
 * destination port/bitmap and the policer (optional).
 *
 * @param=info - management info
 * @param=mac_addr - mac address
 * @param=vlan - vlan
 * @param=dest_port_bmp - port number/bitmap
 * @param=cir_kbps - ingress bandwidth
 * @param=cir_bytes - ingress burst size
 * @param=ingress_port_bmp - ingress port bitmap
 *
 * @returns 0 on success, !0 on error
 */
int32 mgmt_fdb_entry_add(mgmt_info_t *info, int8 *mac_addr, uint32 vlan,
                         uint32 dest_port_bmp, uint32 cir_kbps,
                         uint32 cbs_bytes, uint32 ingress_port_bmp)
{
    int32 errno = -1;
    mgmt_command_t cmd_packet;
    mgmt_command_t *cmd = &cmd_packet;
    mgmt_reply_t reply_packet;
    mgmt_reply_t *reply = &reply_packet;
    uint8 mac_hex[MGMT_ETHER_ADDR_LEN];
    int32 status;
    uint32 max_rate, port = 0;

    if ((info == NULL) || (mac_addr == NULL)) {
      printk("%s :: Invalid input parameter(info = 0x%x,mac_addr = 0x%x)\r\n",
               __FUNCTION__, (unsigned int)info, (unsigned int)mac_addr);
      return errno;
    }

    if (!MGMT_AVB_VLAN_VALID(vlan)){
        printk("Invalid VLAN id %d\r\n", vlan);
        return errno;
    }

    sal_memset(mac_hex, 0x00, MGMT_ETHER_ADDR_LEN);

    mgmt_convert_mac(mac_addr, mac_hex, &status);
    if (status == -1){
        printk("Issue with MAC\r\n");
        return errno;
    }

    if(!(mac_hex[0] & (0x01)) && IS_EXTERNAL_INTF_INVALID(dest_port_bmp)) {
        printk("Invalid port number\r\n");
        return errno;
    }else if((mac_hex[0] & 0x01)&& ((dest_port_bmp & (~AVB_VALID_PORT_BITMASK))
              || dest_port_bmp == 0)){
        printk("Invalid port bitmap\r\n");
        return errno;
    }

    if (cir_kbps > 1000 * MGMT_AVB_PORT_SPEED_1000) {
        printk("Invalid Ingress Max Bandwidth \r\n");
        return -1;
    }

    if ((ingress_port_bmp != 0) && (ingress_port_bmp & (~AVB_VALID_PORT_BITMASK))) {
        printk("Invalid ingress port bitmap  %d\r\n", ingress_port_bmp);
            return -1;
    }

    if (((mac_hex[0] & 0x01) && (dest_port_bmp & ingress_port_bmp)) ||
          (!(mac_hex[0] & (0x01)) && ((1 << dest_port_bmp) & ingress_port_bmp)))
    {
        printk("Conflicts in destination and ingress ports\r\n");
        return -1;
    }

    errno = mgmt_avb_port_next_get(MGMT_AVB_TRUE, ingress_port_bmp, &port);
    while (!errno) {
        /* Assuming P8 running at 100Mbps */
        max_rate = AVB_INT_PORT_IS_SGMII(port) ?
           (MGMT_AVB_PORT_SPEED_1000 * 1000) : (MGMT_AVB_PORT_SPEED_100 * 1000);
        if (cir_kbps > max_rate) {
            printk("Invalid Ingress Max Bandwidth configured on port %d\r\n", port);
            return errno;
        }
        errno = mgmt_avb_port_next_get(MGMT_AVB_FALSE, ingress_port_bmp, &port);
    }


    sal_memset(cmd, 0, sizeof(mgmt_command_t));
    sal_memset(reply, 0, sizeof(mgmt_reply_t));

    cmd->cmd = uswap32(MGMT_AVB);
    cmd->u.avb.avb_cmd = uswap32(MGMT_AVB_MSRP_ENTRY_ADD);
    cmd->u.avb.u.fdb_add.id = uswap32(vlan);
    swap_str((char *)cmd->u.avb.u.fdb_add.addr,(char *)mac_hex, MGMT_ETHER_ADDR_LEN);
    cmd->u.avb.u.fdb_add.port_bmp = uswap32(dest_port_bmp);
    cmd->u.avb.u.fdb_add.cir_kbps = uswap32(cir_kbps);
    cmd->u.avb.u.fdb_add.cbs_bytes = uswap32(cbs_bytes);
    cmd->u.avb.u.fdb_add.ingressPortMask = uswap32(ingress_port_bmp);
    cmd->len = uswap32(MGMT_AVB_COMMAND_LEN + sizeof(cmd->u.avb.u.fdb_add));
    errno = mgmt_cmd_reply(info, cmd, reply, 0);
    if (errno) {
        return errno;
    }
    return 0;
}

/*
 * @api
 * mgmt_fdb_entry_delete
 *
 * @brief
 * Delete a forwarding dabatase entry based on the given VLAN/MAC parameters.
 *
 * @param=info - management info
 * @param=mac_addr - mac address
 * @param=vlan - vlan
 *
 * @returns 0 on success, !0 on error
 */
int32 mgmt_fdb_entry_delete(mgmt_info_t *info, int8 *mac_addr, uint32 vlan)
{
    int32 errno = -1;
    mgmt_command_t cmd_packet;
    mgmt_command_t *cmd = &cmd_packet;
    mgmt_reply_t reply_packet;
    mgmt_reply_t *reply = &reply_packet;
    uint8 mac_hex[MGMT_ETHER_ADDR_LEN];
    int32 status;

    if ((info == NULL) || (mac_addr == NULL)) {
      printk("%s :: Invalid input parameter(info = 0x%x, mac_addr = 0x%x) \r\n",
                                            __FUNCTION__, (unsigned int)info, (unsigned int)mac_addr);
      return errno;
    }

    if (!MGMT_AVB_VLAN_VALID(vlan)){
        printk("Invalid VLAN id %d\r\n", vlan);
        return errno;
    }
    sal_memset(mac_hex, 0x00, MGMT_ETHER_ADDR_LEN);
    mgmt_convert_mac(mac_addr, mac_hex, &status);
    if (status == -1){
        return errno;
    }

    sal_memset(cmd, 0, sizeof(mgmt_command_t));
    sal_memset(reply, 0, sizeof(mgmt_reply_t));

    cmd->cmd = uswap32(MGMT_AVB);
    cmd->u.avb.avb_cmd = uswap32(MGMT_AVB_MSRP_ENTRY_DELETE);
    cmd->u.avb.u.fdb_del.id = uswap32(vlan);
    swap_str((char *)cmd->u.avb.u.fdb_del.addr, (char *)mac_hex, MGMT_ETHER_ADDR_LEN);
    cmd->len = uswap32(MGMT_AVB_COMMAND_LEN + sizeof(cmd->u.avb.u.fdb_del));

    errno = mgmt_cmd_reply(info, cmd, reply, 0);
    if (errno) {
        return errno;
    }
    return 0;
}

/*
 * @api
 * mgmt_fdb_boundary_set
 *
 * @brief
 * set the boundary value of port.
 *
 * @param=info - management info
 * @param=port - port number
 * @param=sr_class - stream class
 * @param=is_boundary - port type
 *
 * @returns 0 on success, !0 on error
 */

int32 mgmt_fdb_boundary_set(mgmt_info_t *info, uint32 port, uint32 sr_class, uint32 is_boundary)
{
    int32 errno = -1;
    mgmt_command_t cmd_packet;
    mgmt_command_t *cmd = &cmd_packet;
    mgmt_reply_t reply_packet;
    mgmt_reply_t *reply = &reply_packet;

    if (info == NULL) {
      printk("%s :: mgmt_info_t pointer is NULL\r\n", __FUNCTION__);
      return errno;
    }

    if (IS_EXTERNAL_INTF_INVALID(port)){
      printk("Invalid port number %d\r\n", port);
      return errno;
    }

    if ((sr_class != MGMT_AVB_STREAM_CLASS_TYPE_A) &&
        (sr_class != MGMT_AVB_STREAM_CLASS_TYPE_B)) {
      printk("%s :: Invalid stream class type\r\n", __FUNCTION__);
      return errno;
    }

    if ((is_boundary != MGMT_AVB_PORT_BOUNDARY) && (is_boundary != MGMT_AVB_PORT_CORE)) {
      printk("Invalid value of is_bondary parameter\r\n");
      return errno;
    }

    sal_memset(cmd, 0, sizeof(mgmt_command_t));
    sal_memset(reply, 0, sizeof(mgmt_reply_t));
    cmd->cmd = uswap32(MGMT_AVB);
    cmd->u.avb.avb_cmd = uswap32(MGMT_AVB_MSRP_BOUNDARY_SET);
    cmd->u.avb.u.port_boundary_set.port = uswap32(port);
    cmd->u.avb.u.port_boundary_set.sr_class = uswap32(sr_class);
    cmd->u.avb.u.port_boundary_set.is_boundary = uswap32(is_boundary);
    cmd->len = uswap32(MGMT_AVB_COMMAND_LEN + sizeof(cmd->u.avb.u.port_boundary_set));
    errno = mgmt_cmd_reply(info, cmd, reply, 0);
    if (errno) {
        return errno;
    }
    return 0;
}

/*
 * @api
 * mgmt_fdb_boundary_get
 *
 * @brief
 * get the bondary value of port.
 *
 * @param=info - management info
 * @param=port - port number
 * @param=sr_class - stream class
 * @param=is_boundary - port type
 *
 * @returns 0 on success, !0 on error
 */

int32 mgmt_fdb_boundary_get(mgmt_info_t *info, uint32 port, uint32 *bdyA, uint32 *bdyB)
{
    int32 errno = -1;
    mgmt_command_t cmd_packet;
    mgmt_command_t *cmd = &cmd_packet;
    mgmt_reply_t reply_packet;
    mgmt_reply_t *reply = &reply_packet;

    if ((info == NULL) || (bdyA == NULL) || (bdyB == NULL)) {
      printk("%s :: Invalid input parameter(info = 0x%x, bdyA = 0x%x, bdyB = 0x%x)\r\n",
                                    __FUNCTION__, (unsigned int)info, (unsigned int)bdyA, (unsigned int)bdyB);
      return errno;
    }

    if (IS_EXTERNAL_INTF_INVALID(port)){
      printk("Invalid port number %d\r\n", port);
      return errno;
    }

    sal_memset(cmd, 0, sizeof(mgmt_command_t));
    sal_memset(reply, 0, sizeof(mgmt_reply_t));
    cmd->cmd = uswap32(MGMT_AVB);
    cmd->u.avb.avb_cmd = uswap32(MGMT_AVB_MSRP_BOUNDARY_GET);
    cmd->u.avb.u.port_boundary_get.port = uswap32(port);
    cmd->len = uswap32(MGMT_AVB_COMMAND_LEN + sizeof(cmd->u.avb.u.port_boundary_get));
    errno = mgmt_cmd_reply(info, cmd, reply, 0);
    if (errno) {
        return errno;
    }
    *bdyA = uswap32(reply->u.avb.port_boundary_get.bdyA);
    *bdyB = uswap32(reply->u.avb.port_boundary_get.bdyB);
    return 0;
}

/*
 * @api
 * mgmt_stream_bandwidth_get
 *
 * @brief
 * Get the bandwidth for the stream
 *
 * @param=mac_addr - mac address
 * @param=vlan - vlan
 * @param=rate - bandwidth rate in kbps
 * @param=burst - bandwidth burst in bytes
 *
 * @returns 0 on success, !0 on error
 */
int32 mgmt_stream_bandwidth_get(mgmt_info_t *info, int8 *mac_addr, uint32 vlan,
                               uint32 *rate, uint32 *burst)
{
    int32 errno = -1;
    uint8 mac_hex[MGMT_ETHER_ADDR_LEN];
    mgmt_command_t cmd_packet;
    mgmt_command_t *cmd = &cmd_packet;
    mgmt_reply_t reply_packet;
    mgmt_reply_t *reply = &reply_packet;
    int32 status;

    if ((info == NULL) || (mac_addr == NULL) || (rate == NULL) || (burst == NULL)) {
      printk("%s :: Invalid input param(info = 0x%x, rate = 0x%x, burst = 0x%x)\r\n",
                                __FUNCTION__, (unsigned int)info, (unsigned int)rate, (unsigned int)burst);
      return errno;
    }

    if (!MGMT_AVB_VLAN_VALID(vlan)){
        printk("Invalid VLAN id %d\r\n", vlan);
        return errno;
    }
    sal_memset(mac_hex, 0x00, MGMT_ETHER_ADDR_LEN);
    mgmt_convert_mac(mac_addr, mac_hex, &status);
    if (status == -1){
        return errno;
    }

    sal_memset(cmd, 0, sizeof(mgmt_command_t));
    sal_memset(reply, 0, sizeof(mgmt_reply_t));

    cmd->cmd = uswap32(MGMT_AVB);
    cmd->u.avb.avb_cmd = uswap32(MGMT_AVB_MSRP_STREAM_BANDWIDTH_GET);
    cmd->u.avb.u.stream_bandwidth_get.id = uswap32(vlan);
    swap_str((char *)cmd->u.avb.u.stream_bandwidth_get.addr, (char *)mac_hex, MGMT_ETHER_ADDR_LEN);
    cmd->len = uswap32(MGMT_AVB_COMMAND_LEN + sizeof(cmd->u.avb.u.stream_bandwidth_get));

    errno = mgmt_cmd_reply(info, cmd, reply, 0);
    if (errno) {
        return errno;
    }

    *rate = uswap32(reply->u.avb.bw_info.rate_kbps);
    *burst = uswap32(reply->u.avb.bw_info.burst_kbits);

    return 0;
}


/*
 * @api
 * mgmt_port_bandwidth_get
 *
 * @brief
 * Get the current bandwidth reserved for the given SR_class on the given port
 *
 * @param=port - port
 * @param=sr_class - stream class
 * @param=rate - bandwidth rate in kbps
 * @param=burst - bandwidth burst in kbits
 *
 * @returns 0 on success, !0 on error
 */
int32 mgmt_port_bandwidth_get(mgmt_info_t *info, uint32 port, uint32 sr_class,
        uint32 *rate, uint32 *burst)
{
    int32 errno = -1;
    mgmt_command_t cmd_packet;
    mgmt_command_t *cmd = &cmd_packet;
    mgmt_reply_t reply_packet;
    mgmt_reply_t *reply = &reply_packet;

    if ((info == NULL) || (rate == NULL) || (burst == NULL)) {
      printk("%s :: Invalid input param(info = 0x%x, rate = 0x%x, burst = 0x%x)\r\n",
                                __FUNCTION__, (unsigned int)info, (unsigned int)rate, (unsigned int)burst);
      return errno;
    }

    if (IS_EXTERNAL_INTF_INVALID(port)){
      printk("Invalid port number %d\r\n", port);
      return errno;
    }

    if ((sr_class != MGMT_AVB_STREAM_CLASS_TYPE_A) &&
        (sr_class != MGMT_AVB_STREAM_CLASS_TYPE_B)) {
      printk("%s :: Invalid stream class type\r\n", __FUNCTION__);
      return errno;
    }

    sal_memset(cmd, 0, sizeof(mgmt_command_t));
    sal_memset(reply, 0, sizeof(mgmt_reply_t));

    cmd->cmd = uswap32(MGMT_AVB);
    cmd->u.avb.avb_cmd = uswap32(MGMT_AVB_MSRP_PORT_BANDWIDTH_GET);
    cmd->u.avb.u.port_bandwidth_get.num = uswap32(port);
    cmd->u.avb.u.port_bandwidth_get.sr_class = uswap32(sr_class);
    cmd->len = uswap32(MGMT_AVB_COMMAND_LEN + sizeof(cmd->u.avb.u.port_bandwidth_get));

    errno = mgmt_cmd_reply(info, cmd, reply, 0);
    if (errno) {
        return errno;
    }

    *rate = uswap32(reply->u.avb.bw_info.rate_kbps);
    *burst = uswap32(reply->u.avb.bw_info.burst_kbits);

    return 0;
}

/*
 * @api
 * mgmt_port_bandwidth_set
 *
 * @brief
 * Configure the bandwidth on the given SR_Class on the given port.
 *
 * @param=port - port
 * @param=sr_class - stream class
 * @param=rate - bandwidth rate in kbps
 * @param=burst - bandwidth burst in kbits
 *
 * @returns 0 on success, !0 on error
 */
int32 mgmt_port_bandwidth_set(mgmt_info_t *info, uint32 port, uint32 sr_class,
        uint32 rate, uint32 burst)
{
    int32 errno = -1;
    mgmt_command_t cmd_packet;
    mgmt_command_t *cmd = &cmd_packet;
    mgmt_reply_t reply_packet;
    mgmt_reply_t *reply = &reply_packet;
    uint32 classConfig;
    uint32 rateConfig = 0;
    uint32 burstConfig = 0;
    uint32 max_rate;
    uint32 max_burst = MGMT_AVB_MAX_BURST;
    uint32 port_speed_max;
    uint32 avb_bw;

    if (info == NULL) {
      printk("%s :: mgmt_info_t pointer is NULL\r\n", __FUNCTION__);
      return errno;
    }

    if (IS_EXTERNAL_INTF_INVALID(port)){
      printk("Invalid port number %d\r\n", port);
      return errno;
    }

    if ((sr_class != MGMT_AVB_STREAM_CLASS_TYPE_A) &&
      (sr_class != MGMT_AVB_STREAM_CLASS_TYPE_B)) {
      printk("Invalid stream class %d\r\n", sr_class);
      return errno;
    }

    /* get the configration details */
     classConfig = MGMT_AVB_STREAM_CLASS_TYPE_A + MGMT_AVB_STREAM_CLASS_TYPE_B - sr_class;
     mgmt_port_bandwidth_get(info, port, classConfig, &rateConfig, &burstConfig);
     port_speed_max = AVB_INT_PORT_IS_SGMII(port) ? MGMT_AVB_PORT_SPEED_1000 : MGMT_AVB_PORT_SPEED_100;
     max_rate = (port_speed_max * 1000); /* in kbps */
     if ((rate + rateConfig) > max_rate){
       printk("Maximum allowed rate configuration (class A and class B) is %d\r\n",
           max_rate);
       return errno;
     }

     if ((burst + burstConfig) > max_burst){
       printk("Maximum allowed burst configuration (class A and class B) is %d\r\n",
           max_burst);
       return errno;
     }

     /* recommended bandwidth for avb trafiic is 75% of total bandwidth */
     avb_bw = MGMT_AVB_RECOMMENDED_BANDWIDTH(port_speed_max);
     if ((rate + rateConfig) > avb_bw){
       printk("Warning :: More than 75%% bandwidth for AVB traffic (class A & class B). BW = %d\r\n",
           (rate + rateConfig));
     }

    sal_memset(cmd, 0, sizeof(mgmt_command_t));
    sal_memset(reply, 0, sizeof(mgmt_reply_t));

    cmd->cmd = uswap32(MGMT_AVB);
    cmd->u.avb.avb_cmd = uswap32(MGMT_AVB_MSRP_PORT_BANDWIDTH_SET);
    cmd->u.avb.u.port_bandwidth_set.num = uswap32(port);
    cmd->u.avb.u.port_bandwidth_set.sr_class = uswap32(sr_class);
    cmd->u.avb.u.port_bandwidth_set.rate_kbps = uswap32(rate);
    cmd->u.avb.u.port_bandwidth_set.burst_kbits = uswap32(burst);
    cmd->len = uswap32(MGMT_AVB_COMMAND_LEN + sizeof(cmd->u.avb.u.port_bandwidth_set));

    errno = mgmt_cmd_reply(info, cmd, reply, 0);
    if (errno) {
        return errno;
    }
    return 0;
}

/*
 * @api
 * mgmt_pcp_mapping_get
 *
 * @brief
 * Get the current values of SR_class priority and remap-priority
 *
 * @param=sr_class - stream class
 * @param=prio - PCP of the stream class
 * @param=remap - PCP for the regenerated priority
 *
 * @returns 0 on success, !0 on error
 */
int32 mgmt_pcp_mapping_get(mgmt_info_t *info, uint32 sr_class, uint32 *prio, uint32 *remap)
{
    int32 errno = -1;
    mgmt_command_t cmd_packet;
    mgmt_command_t *cmd = &cmd_packet;
    mgmt_reply_t reply_packet;
    mgmt_reply_t *reply = &reply_packet;

    if ((info == NULL) || (prio == NULL) || (remap == NULL)) {
      printk("%s :: Invalid input parameter(info = 0x%x, prio=0x%x, remap= 0x%x)\r\n",
                                        __FUNCTION__, (unsigned int)info, (unsigned int)prio, (unsigned int)remap);
      return errno;
    }

    if ((sr_class != MGMT_AVB_STREAM_CLASS_TYPE_A) &&
        (sr_class != MGMT_AVB_STREAM_CLASS_TYPE_B)) {
      printk("%s :: Invalid stream class type\r\n", __FUNCTION__);
      return errno;
    }

    sal_memset(cmd, 0, sizeof(mgmt_command_t));
    sal_memset(reply, 0, sizeof(mgmt_reply_t));

    cmd->cmd = uswap32(MGMT_AVB);
    cmd->u.avb.avb_cmd = uswap32(MGMT_AVB_MSRP_PCP_MAPPING_GET);
    cmd->u.avb.u.pcp_mapping_get.sr_class = uswap32( sr_class);
    cmd->len = uswap32(MGMT_AVB_COMMAND_LEN + sizeof(cmd->u.avb.u.pcp_mapping_get));

    errno = mgmt_cmd_reply(info, cmd, reply, 0);
    if (errno) {
        return errno;
    }

    *prio = uswap32(reply->u.avb.pcp_mapping.prio);
    *remap = uswap32(reply->u.avb.pcp_mapping.remap_prio);
    return 0;
}

/*
 * @api
 * mgmt_pcp_mapping_set
 *
 * @brief
 * Configure the values of SR_class priority and remap-priority
 *
 * @param=sr_class - stream class
 * @param=prio - PCP of the stream class
 * @param=remap - PCP for the regenerated priority
 *
 * @returns 0 on success, !0 on error
 */
int32 mgmt_pcp_mapping_set(mgmt_info_t *info, uint32 sr_class, uint32 prio, uint32 remap)
{
    int32 errno = -1;
    mgmt_command_t cmd_packet;
    mgmt_command_t *cmd = &cmd_packet;
    mgmt_reply_t reply_packet;
    mgmt_reply_t *reply = &reply_packet;

    if (info == NULL) {
      printk("%s :: mgmt_info_t pointer is NULL\r\n", __FUNCTION__);
      return errno;
    }

    if ((sr_class != MGMT_AVB_STREAM_CLASS_TYPE_A) &&
        (sr_class != MGMT_AVB_STREAM_CLASS_TYPE_B)) {
      printk("%s :: Invalid stream class type\r\n", __FUNCTION__);
      return errno;
    }

    if (prio > MGMT_AVB_DOT1Q_INTF_PRIO_MAX) {
      printk("%s :: Invalid priority value %d\r\n", __FUNCTION__,  prio);
      return errno;
    }

    if (remap > MGMT_AVB_DOT1Q_INTF_PRIO_MAX) {
      printk("%s :: Invalid priority value %d\r\n", __FUNCTION__, remap);
      return errno;
    }

    sal_memset(cmd, 0, sizeof(mgmt_command_t));
    sal_memset(reply, 0, sizeof(mgmt_reply_t));

    cmd->cmd = uswap32(MGMT_AVB);
    cmd->u.avb.avb_cmd = uswap32(MGMT_AVB_MSRP_PCP_MAPPING_SET);
    cmd->u.avb.u.pcp_mapping_set.sr_class = uswap32(sr_class);
    cmd->u.avb.u.pcp_mapping_set.prio = uswap32(prio);
    cmd->u.avb.u.pcp_mapping_set.remap_prio = uswap32(remap);
    cmd->len = uswap32(MGMT_AVB_COMMAND_LEN + sizeof(cmd->u.avb.u.pcp_mapping_set));

    errno = mgmt_cmd_reply(info, cmd, reply, 0);
    if (errno) {
        return errno;
    }
    return 0;
}

/*
 * @api
 * mgmt_mirror_enable
 *
 * @brief
 * Enable mirroring
 *
 * @param=info - management info
 * @param=port_bitmap - port bitmap
 * @param=probe_port - probe port
 *
 * @returns 0 on success, !0 on error
 */
int32 mgmt_mirror_enable(mgmt_info_t *info, uint32 port_bitmap,
                                    uint32 probe_port, MGMT_INTF_MIRROR_MODE_t mode)
{
    int32 errno = -1;
    mgmt_command_t cmd_packet;
    mgmt_command_t *cmd = &cmd_packet;
    mgmt_reply_t reply_packet;
    mgmt_reply_t *reply = &reply_packet;

    if (info == NULL) {
      printk("%s :: mgmt_info_t pointer is NULL\r\n", __FUNCTION__);
      return errno;
    }

    if ((mode != MGMT_INTF_MIRROR_INGRESS) && (mode != MGMT_INTF_MIRROR_EGRESS) &&
        (mode != MGMT_INTF_MIRROR_BOTH)) {
      printk("%s :: Invalid mirroring mode %d\r\n", __FUNCTION__, mode);
      return errno;
    }

    if (((1 << probe_port) & (~AVB_VALID_PORT_BITMASK)) ||
        (!MGMT_AVB_PHYS_INTF_IS_VALID(probe_port))) {
      printk("%s :: Invalid probe port number %d\r\n",__FUNCTION__, probe_port);
      return errno;
    }

    /* accept port bitmap of (0,1,2,3,4,5,6,8) leo switch ports
       make sure the port bitmap is non-zero and
       don't allow the probe port in the port bitmap
       */
    if ((port_bitmap & (~AVB_VALID_PORT_BITMASK)) ||
        (port_bitmap == 0x0) || ((port_bitmap & (1 << probe_port)) != 0x0)){
      printk("Invalid port bitmap or conflicting probe port\r\n");
      return errno;
    }

    sal_memset(cmd, 0, sizeof(mgmt_command_t));
    sal_memset(reply, 0, sizeof(mgmt_reply_t));

    cmd->cmd = uswap32(MGMT_AVB);
    cmd->u.avb.avb_cmd = uswap32(MGMT_AVB_SWITCH_MIRROR_ENABLE);
    cmd->u.avb.u.mirror_enable.bmp = uswap32(port_bitmap);
    cmd->u.avb.u.mirror_enable.probe_port = uswap32(probe_port);
    cmd->u.avb.u.mirror_enable.mode = uswap32(mode);
    cmd->len = uswap32(MGMT_AVB_COMMAND_LEN + sizeof(cmd->u.avb.u.mirror_enable));

    errno = mgmt_cmd_reply(info, cmd, reply, 0);
    if (errno) {
        return errno;
    }
    return 0;
}

/*
 * @api
 * mgmt_mirror_disable
 *
 * @brief
 * Disable mirroring
 *
 * @param=info - management info
 *
 * @returns 0 on success, !0 on error
 */
int32 mgmt_mirror_disable(mgmt_info_t *info)
{
    int32 errno = -1;
    mgmt_command_t cmd_packet;
    mgmt_command_t *cmd = &cmd_packet;
    mgmt_reply_t reply_packet;
    mgmt_reply_t *reply = &reply_packet;

    if (info == NULL) {
      printk("%s :: mgmt_info_t pointer is NULL\r\n", __FUNCTION__);
      return errno;
    }

    sal_memset(cmd, 0, sizeof(mgmt_command_t));
    sal_memset(reply, 0, sizeof(mgmt_reply_t));

    cmd->cmd = uswap32(MGMT_AVB);
    cmd->u.avb.avb_cmd = uswap32(MGMT_AVB_SWITCH_MIRROR_DISABLE);
    cmd->len = uswap32(MGMT_AVB_COMMAND_LEN);

    errno = mgmt_cmd_reply(info, cmd, reply, 0);
    if (errno) {
        return errno;
    }
    return 0;
}

/*
 * @api
 * mgmt_mirror_status
 *
 * @brief
 * Get the status of mirroring
 *
 * @param=info - management info
 * @param=mode -     enabled or disabled
 * @param=port_mask- bitmap of ports to mirror
 * @param=probe  -   probe port
 * @param=traffic-   Traffic direction ingress, egress or all
 *
 * @returns 0 on success, !0 on error
 */
int32 mgmt_mirror_status(mgmt_info_t *info, uint32_t *mode, uint32_t *port_mask, uint32_t *probe, uint32_t *traffic)
{
    int32 errno = -1;
    mgmt_command_t cmd_packet;
    mgmt_command_t *cmd = &cmd_packet;
    mgmt_reply_t reply_packet;
    mgmt_reply_t *reply = &reply_packet;

    if (info == NULL) {
      printk("%s :: mgmt_info_t pointer is NULL\r\n", __FUNCTION__);
      return errno;
    }

    sal_memset(cmd, 0, sizeof(mgmt_command_t));
    sal_memset(reply, 0, sizeof(mgmt_reply_t));

    cmd->cmd = uswap32(MGMT_AVB);
    cmd->u.avb.avb_cmd = uswap32(MGMT_AVB_SWITCH_MIRROR_STATUS);
    cmd->len = uswap32(MGMT_AVB_COMMAND_LEN);

    errno = mgmt_cmd_reply(info, cmd, reply, 0);
    if (errno) {
        return errno;
    }

    *mode = uswap32(reply->u.avb.mirror_status.mode);
    *port_mask = uswap32(reply->u.avb.mirror_status.port_mask);
    *probe = uswap32(reply->u.avb.mirror_status.probe);
    *traffic = uswap32(reply->u.avb.mirror_status.traffic);

    return 0;
}

/*
 * @api
 * mgmt_port_type_get
 *
 * @brief
 * get port type
 *
 * @param=info - pointer to the mgmt_info_t
 * @port = port number
 * @returns 0 on success, !0 on error
 */
int32 mgmt_port_type_get(mgmt_info_t *info, uint32 port, uint32 *port_type)
{
    int32 errno = -1;
    mgmt_command_t cmd_packet;
    mgmt_command_t *cmd = &cmd_packet;
    mgmt_reply_t reply_packet;
    mgmt_reply_t *reply = &reply_packet;

    if ((info == NULL) || (port_type == NULL)) {
      printk("%s :: Invalid input parameter(info = 0x%x,port_type = 0x%x)\r\n", __FUNCTION__, (unsigned int)info, (unsigned int)port_type);
      return errno;
    }

    if (!MGMT_AVB_PHYS_INTF_IS_VALID(port)) {
      printk("Error: Invalid port specified: %d\r\n", port);
      return errno;
    }

    sal_memset(cmd, 0, sizeof(mgmt_command_t));
    sal_memset(reply, 0, sizeof(mgmt_reply_t));

    cmd->cmd = uswap32(MGMT_AVB);
    cmd->u.avb.avb_cmd = uswap32(MGMT_AVB_PORT_TYPE_GET);
    cmd->len = uswap32(MGMT_AVB_COMMAND_LEN + sizeof(cmd->u.avb.u.mgmt_port_type));
    cmd->u.avb.u.mgmt_port_type.port = uswap32(port);

    errno = mgmt_cmd_reply(info, cmd, reply, 0);
    if (errno) {
        return errno;
    }
    *port_type = uswap32(reply->u.avb.mgmt_port_type.port_type);
    return 0;
}

/*
 * @api
 * mgmt_macaddr_get
 *
 * @brief
 * get mac address
 *
 * @param=info - pointer to the mgmt_info_t
 * @mac_addr= get mac address
 * @returns 0 on success, !0 on error
 */
int32 mgmt_macaddr_get(mgmt_info_t *info, uint8 *mac_addr)
{
    uint32 macaddr0;
    uint32 macaddr1;
    mgmt_spi_rd32(ETH_MACADDR0, &macaddr0);
    mgmt_spi_rd32(ETH_MACADDR1, &macaddr1);
    mac_addr[0] = (uint8_t)(macaddr0 >> 24);
    mac_addr[1] = (uint8_t)(macaddr0 >> 16);
    mac_addr[2] = (uint8_t)(macaddr0 >> 8);
    mac_addr[3] = (uint8_t)(macaddr0);
    mac_addr[4] = (uint8_t)(macaddr1 >> 8);
    mac_addr[5] = (uint8_t)(macaddr1);
    return 0;
}
/*
 * @api
 * mgmt_avb_version_get
 *
 * @brief
 * get software version
 *
 * @param=info - pointer to the mgmt_info_t
 * @ver = get avb version
 * @returns 0 on success, !0 on error
 */

int32 mgmt_avb_version_get(mgmt_info_t *info, char *ver)
{
    int32 errno = -1;
    mgmt_command_t cmd_packet;
    mgmt_command_t *cmd = &cmd_packet;
    mgmt_reply_t reply_packet;
    mgmt_reply_t *reply = &reply_packet;

    if ((info == NULL) || (ver == NULL)) {
      printk("%s :: Invalid input parameter(info = 0x%x, ver = 0x%x)\r\n",
                                            __FUNCTION__, (unsigned int)info, (unsigned int)ver);
      return errno;
    }

    sal_memset(cmd, 0, sizeof(mgmt_command_t));
    sal_memset(reply, 0, sizeof(mgmt_reply_t));

    cmd->cmd = uswap32(MGMT_AVB);
    cmd->u.avb.avb_cmd = uswap32(MGMT_AVB_SW_VERSION);
    cmd->len = uswap32(MGMT_AVB_COMMAND_LEN);

    errno = mgmt_cmd_reply(info, cmd, reply, 0);
    if (errno) {
        return errno;
    }

    swap_str(ver, reply->u.avb.avb_sw_version.avb_version, sizeof(reply->u.avb.avb_sw_version.avb_version));

    return 0;
}

/*
 * @api
 * mgmt_os_version_get
 *
 * @brief
 * get software version
 *
 * @param=info - pointer to the mgmt_info_t
 * @ver = get OS version
 * @returns 0 on success, !0 on error
 */

int32 mgmt_os_version_get(mgmt_info_t *info, char *ver)
{
    int32 errno = -1;
    mgmt_command_t cmd_packet;
    mgmt_command_t *cmd = &cmd_packet;
    mgmt_reply_t reply_packet;
    mgmt_reply_t *reply = &reply_packet;

    if ((info == NULL) || (ver == NULL))  {
        printk("%s :: Invalid input parameter(info = 0x%x, ver = 0x%x\r\n",
                                    __FUNCTION__, (unsigned int)info, (unsigned int)ver);
        return errno;
    }

    sal_memset(cmd, 0, sizeof(mgmt_command_t));
    sal_memset(reply, 0, sizeof(mgmt_reply_t));

    cmd->cmd = uswap32(MGMT_OS_SW_VERSION);
    cmd->len = uswap32(MGMT_COMMAND_LEN);

    errno = mgmt_cmd_reply(info, cmd, reply, 0);
    if (errno) {
        return errno;
    }

    swap_str(ver, reply->u.os_sw_version.os_version, sizeof(reply->u.os_sw_version.os_version));

    return 0;
}

/*@api
 * mgmt_fw_status_get
 *
 * @brief
 * Get the status of firmware execution and versions
 *
 * @param=info - management info
 * @param=val - pointer to uint16
 *
 * @returns int32
 *
 * @desc
 */

int32 mgmt_fw_status_get(mgmt_info_t *info, uint16 *val)
{
    return mgmt_spi_rd16_ind(MISC_SPARE_SW_REG3, val);
}


/*@api
 * mgmt_userdata_status_get
 *
 * @brief
 * Get the status of userdata partitions
 *
 * @param=info - management info
 * @param=val - pointer to uint16
 *
 * @returns int32
 *
 * @desc
 */
int32 mgmt_userdata_status_get(mgmt_info_t *info, uint16 *val)
{
    return mgmt_spi_rd16_ind(MISC_SPARE_SW_REG13, val);
}

/*
 * @api
 * mgmt_avnu_clock_state_set
 *
 * @brief
 * configure the avnu avnu clock state
 *
 * @param=state - GM/Slave
 *
 * @returns 0 on success, !0 on error
 */
int32 mgmt_avnu_clock_state_set(mgmt_info_t *info, uint32 state)
{
    int32 errno = -1;
    mgmt_command_t cmd_packet;
    mgmt_command_t *cmd = &cmd_packet;
    mgmt_reply_t reply_packet;
    mgmt_reply_t *reply = &reply_packet;

    if (info == NULL) {
      printk("%s :: mgmt_info_t pointer is NULL\r\n", __FUNCTION__);
      return errno;
    }

    if ((state != MGMT_AVNU_CLOCK_STATE_GM) && (state != MGMT_AVNU_CLOCK_STATE_SLAVE)) {
      printk("%s :: Invalid AVnu clock state value %d\r\n", __FUNCTION__, state);
      return errno;
    }

    sal_memset(cmd, 0, sizeof(mgmt_command_t));
    sal_memset(reply, 0, sizeof(mgmt_reply_t));

    cmd->cmd = uswap32(MGMT_AVB);
    cmd->u.avb.avb_cmd = uswap32(MGMT_AVB_AVNU_CLOCK_STATE_SET);
    cmd->u.avb.u.avnu_clock_state.clock_state = uswap32(state);
    cmd->len = uswap32(MGMT_AVB_COMMAND_LEN + sizeof(cmd->u.avb.u.avnu_clock_state));
    errno = mgmt_cmd_reply(info, cmd, reply, 0);
    if (errno) {
        return errno;
    }

    return 0;
}

int32 mgmt_avnu_sync_absence_timeout_set(mgmt_info_t *info, uint32 syncAbsenceTimeout)
{
    int32 errno = -1;
    mgmt_command_t cmd_packet;
    mgmt_command_t *cmd = &cmd_packet;
    mgmt_reply_t reply_packet;
    mgmt_reply_t *reply = &reply_packet;

    if (info == NULL) {
      printk("%s :: mgmt_info_t pointer is NULL\r\n", __FUNCTION__);
      return errno;
    }

    if ((syncAbsenceTimeout < MGMT_AVNU_SYNC_ABSENCE_TIMEOUT_MIN ) ||
        (syncAbsenceTimeout > MGMT_AVNU_SYNC_ABSENCE_TIMEOUT_MAX)) {
      printk("%s :: Invalid value for sync absence timeout %d\r\n", __FUNCTION__, syncAbsenceTimeout);
      return errno;
    }

    sal_memset(cmd, 0, sizeof(mgmt_command_t));
    sal_memset(reply, 0, sizeof(mgmt_reply_t));

    cmd->cmd = uswap32(MGMT_AVB);
    cmd->u.avb.avb_cmd = uswap32(MGMT_AVB_AVNU_SYNC_ABSENCE_TIMEOUT_SET);
    cmd->u.avb.u.avnu_sync_absence.timeout = uswap32(syncAbsenceTimeout);
    cmd->len = uswap32(MGMT_AVB_COMMAND_LEN + sizeof(cmd->u.avb.u.avnu_sync_absence));
    errno = mgmt_cmd_reply(info, cmd, reply, 0);
    if (errno) {
        return errno;
    }

    return 0;
}
/*
 * @api
 * mgmt_avnu_as_capable_mode_set
 *
 * @brief
 * configure the avnu AS capability
 *
 * @param=ascapable - TRUE/FALSE
 * @param=port - port number
 *
 * @returns 0 on success, !0 on error
 */
int32 mgmt_avnu_as_capable_mode_set(mgmt_info_t *info, uint32 port, uint32 ascapable)
{
    int32 errno = -1;
    mgmt_command_t cmd_packet;
    mgmt_command_t *cmd = &cmd_packet;
    mgmt_reply_t reply_packet;
    mgmt_reply_t *reply = &reply_packet;
    mgmt_avnu_interface_config_t intf_config;

    if (info == NULL) {
      printk("%s :: mgmt_info_t pointer is NULL\r\n", __FUNCTION__);
      return errno;
    }

    if (IS_EXTERNAL_INTF_INVALID(port)) {
      printk("%s :: Invalid port number %d\r\n", __FUNCTION__, port);
      return errno;
    }

    if ((ascapable != MGMT_AVNU_AS_CAPABLE_TRUE) && (ascapable != MGMT_AVNU_AS_CAPABLE_FALSE)) {
      printk("%s :: Invalid configuration value for ascapable %d\r\n", __FUNCTION__, ascapable);
      return errno;
    }

    sal_memset(&intf_config, 0x00, sizeof(mgmt_avnu_interface_config_t));
    mgmt_avnu_interface_config_get(info, port, &intf_config);

    sal_memset(cmd, 0, sizeof(mgmt_command_t));
    sal_memset(reply, 0, sizeof(mgmt_reply_t));

    cmd->cmd = uswap32(MGMT_AVB);
    cmd->u.avb.avb_cmd = uswap32(MGMT_AVB_AVNU_ASCAPABLE_MODE_SET);
    cmd->u.avb.u.avnu_ascapable.port = uswap32(port);
    cmd->u.avb.u.avnu_ascapable.mode = uswap32(ascapable);
    cmd->len = uswap32(MGMT_AVB_COMMAND_LEN + sizeof(cmd->u.avb.u.avnu_ascapable));
    errno = mgmt_cmd_reply(info, cmd, reply, 0);
    if (errno) {
        return errno;
    }

    return 0;
}

/*
 * @api
 * mgmt_avnu_port_role_set
 *
 * @brief
 * configure the avnu port role
 *
 * @param=role - master/slave
 * @param=port - port number
 *
 * @returns 0 on success, !0 on error
 */
int32 mgmt_avnu_port_role_set(mgmt_info_t *info, uint32 port, uint32 role)
{
    int32 errno = -1;
    mgmt_command_t cmd_packet;
    mgmt_command_t *cmd = &cmd_packet;
    mgmt_reply_t reply_packet;
    mgmt_reply_t *reply = &reply_packet;
    mgmt_avnu_global_config_t global_config;
    mgmt_avnu_interface_config_t intf_config;
    uint16 intIntf = AVB_INTF_INVALID;
    uint16 extIntf = AVB_INTF_INVALID;

    if (info == NULL) {
      printk("%s :: mgmt_info_t pointer is NULL\r\n", __FUNCTION__);
      return errno;
    }

    if (IS_EXTERNAL_INTF_INVALID(port)) {
      printk("%s :: Invalid port number %d\r\n", __FUNCTION__, port);
      return errno;
    }

    if ((role != MGMT_DOT1AS_ROLE_MASTER) && (role != MGMT_DOT1AS_ROLE_SLAVE))
    {
      printk("%s :: Invalid AVnu port role %d\r\n", __FUNCTION__, role);
      return errno;
    }

    if (role == MGMT_DOT1AS_ROLE_SLAVE) {
      mgmt_avnu_global_config_get(info, &global_config);
      if (global_config.clock_state == MGMT_AVNU_CLOCK_STATE_GM) {
        printk("Error:Configuration of port as slave is not allowed on a Grand Master Device\r\n");
        return errno;
      }

      if (global_config.clock_state == MGMT_AVNU_CLOCK_STATE_SLAVE) {
        for (intIntf = 0; intIntf < AVB_MAX_INTERFACES_PHYS; intIntf++) {
          if (port == mgmtavbCfgxIntfNum[intIntf]) {
            continue;
          }

          extIntf = mgmtavbCfgxIntfNum[intIntf];
          sal_memset(&intf_config, 0x00, sizeof(mgmt_avnu_interface_config_t));
          mgmt_avnu_interface_config_get(info, extIntf, &intf_config);
          if (intf_config.port_role == MGMT_DOT1AS_ROLE_SLAVE) {
            printk("Error: Only one port can be configured as slave, "
                    "Port %d is currently configured as slave\r\n", extIntf);
            return errno;
          }
        }
      }

      sal_memset(&intf_config, 0x00, sizeof(mgmt_avnu_interface_config_t));
      mgmt_avnu_interface_config_get(info, port, &intf_config);
      if (intf_config.pdelay_init_interval == MGMT_PDELAY_TX_DISABLE_VAL) {
        printk("Error: Port Role can't be changed to slave on port %d "
                "unless the initial pdelay interval %d is changed\r\n",
               port, intf_config.pdelay_init_interval);
        return errno;
      }

      if (intf_config.pdelay_oper_interval == MGMT_PDELAY_TX_DISABLE_VAL) {
        printk("Error: Port Role can't be changed to slave on port %d "
                "unless the operational pdelay interval %d is changed\r\n",
                port, intf_config.pdelay_oper_interval);
        return errno;
      }
    }

    sal_memset(cmd, 0, sizeof(mgmt_command_t));
    sal_memset(reply, 0, sizeof(mgmt_reply_t));

    cmd->cmd = uswap32(MGMT_AVB);
    cmd->u.avb.avb_cmd = uswap32(MGMT_AVB_AVNU_PORT_ROLE_SET);
    cmd->u.avb.u.avnu_port_role.port = uswap32(port);
    cmd->u.avb.u.avnu_port_role.role = uswap32(role);
    cmd->len = uswap32(MGMT_AVB_COMMAND_LEN + sizeof(cmd->u.avb.u.avnu_port_role));
    errno = mgmt_cmd_reply(info, cmd, reply, 0);
    if (errno) {
        return errno;
    }

    return 0;
}

/*
 * @api
 * mgmt_avnu_nbr_pdelay_set
 *
 * @brief
 * configure the avnu pdelay value
 *
 * @param=pdelay_val - pdelay value
 * @param=port - port number
 *
 * @returns 0 on success, !0 on error
 */
int32 mgmt_avnu_nbr_pdelay_set(mgmt_info_t *info, uint32 port, uint32 pdelay_val)
{
    int32 errno = -1;
    mgmt_command_t cmd_packet;
    mgmt_command_t *cmd = &cmd_packet;
    mgmt_reply_t reply_packet;
    mgmt_reply_t *reply = &reply_packet;

    if (info == NULL) {
      printk("%s :: mgmt_info_t pointer is NULL\r\n", __FUNCTION__);
      return errno;
    }

    if (IS_EXTERNAL_INTF_INVALID(port)) {
      printk("%s :: Invalid port number %d\r\n", __FUNCTION__, port);
      return errno;
    }

    sal_memset(cmd, 0, sizeof(mgmt_command_t));
    sal_memset(reply, 0, sizeof(mgmt_reply_t));

    cmd->cmd = uswap32(MGMT_AVB);
    cmd->u.avb.avb_cmd = uswap32(MGMT_AVB_AVNU_NBR_PDELAY_SET);
    cmd->u.avb.u.avnu_nbr_pdelay.port = uswap32(port);
    cmd->u.avb.u.avnu_nbr_pdelay.pdelay_val = uswap32(pdelay_val);
    cmd->len = uswap32(MGMT_AVB_COMMAND_LEN + sizeof(cmd->u.avb.u.avnu_nbr_pdelay));
    errno = mgmt_cmd_reply(info, cmd, reply, 0);
    if (errno) {
        return errno;
    }

    return 0;
}

/*
 * @api
 * mgmt_avnu_initial_log_pdelay_interval_set
 *
 * @brief
 * configure the avnu pdelay interval value
 *
 * @param=interval - pdelay interval
 * @param=port - port number
 *
 * @returns 0 on success, !0 on error
 */
int32 mgmt_avnu_initial_log_pdelay_interval_set(mgmt_info_t *info, uint32 port, int32 interval)
{
    int32 errno = -1;
    mgmt_command_t cmd_packet;
    mgmt_command_t *cmd = &cmd_packet;
    mgmt_reply_t reply_packet;
    mgmt_reply_t *reply = &reply_packet;
    mgmt_avnu_interface_config_t intf_config;

    if (info == NULL) {
      printk("%s :: mgmt_info_t pointer is NULL\r\n", __FUNCTION__);
      return errno;
    }

    if(IS_EXTERNAL_INTF_INVALID(port)){
      printk("Invalid port number %d\r\n", port);
      return errno;
    }

    sal_memset(&intf_config, 0x00, sizeof(mgmt_avnu_interface_config_t));
    mgmt_avnu_interface_config_get(info, port, &intf_config);
    if ((interval == MGMT_PDELAY_TX_DISABLE_VAL) && (intf_config.port_role == MGMT_DOT1AS_ROLE_SLAVE)) {
      printk("Error: PDelay can't be disabled unless the port role is configured as master\r\n");
      return errno;
    }

    if (MGMT_AVNU_PDELAY_INIT_INTERVAL_IS_NOT_VALID(interval)) {
      printk("Invalid pdelay interval, val = %d\r\n", interval);
      return errno;
    }

    sal_memset(cmd, 0, sizeof(mgmt_command_t));
    sal_memset(reply, 0, sizeof(mgmt_reply_t));

    cmd->cmd = uswap32(MGMT_AVB);
    cmd->u.avb.avb_cmd = uswap32(MGMT_AVB_AVNU_INITIAL_LOG_PDELAY_INTL_SET);
    cmd->u.avb.u.avnu_pdelay_intl.port = uswap32(port);
    cmd->u.avb.u.avnu_pdelay_intl.pdelay_intl_val = uswap32(interval);
    cmd->len = uswap32(MGMT_AVB_COMMAND_LEN + sizeof(cmd->u.avb.u.avnu_pdelay_intl));
    errno = mgmt_cmd_reply(info, cmd, reply, 0);
    if (errno) {
        return errno;
    }

    return 0;
}

/*
 * @api
 * mgmt_avnu_ioperational_log_pdelay_interval_set
 *
 * @brief
 * configure the avnu operational pdelay interval value
 *
 * @param=interval - operational pdelay interval
 * @param=port - port number
 *
 * @returns 0 on success, !0 on error
 */
int32 mgmt_avnu_operational_log_pdelay_interval_set(mgmt_info_t *info, uint32 port, int32 log_val)
{
    int32 errno = -1;
    mgmt_command_t cmd_packet;
    mgmt_command_t *cmd = &cmd_packet;
    mgmt_reply_t reply_packet;
    mgmt_reply_t *reply = &reply_packet;
    mgmt_avnu_interface_config_t intf_config;

    if (info == NULL) {
      printk("%s :: mgmt_info_t pointer is NULL\r\n", __FUNCTION__);
      return errno;
    }

    if (IS_EXTERNAL_INTF_INVALID(port)) {
      printk("%s :: Invalid port number %d\r\n", __FUNCTION__, port);
      return errno;
    }

    sal_memset(&intf_config, 0x00, sizeof(mgmt_avnu_interface_config_t));
    mgmt_avnu_interface_config_get(info, port, &intf_config);
    if ((log_val == MGMT_PDELAY_TX_DISABLE_VAL) && (intf_config.port_role == MGMT_DOT1AS_ROLE_SLAVE)) {
      printk("Error: PDelay can't be disabled unless the port role is configured as master\r\n");
      return errno;
    }

    if (MGMT_AVNU_OPERATIONAL_PDELAY_INTERVAL_IS_NOT_VALID(log_val)) {
      printk("Invalid opearational pdelay interval, val = %d\r\n", log_val);
      return errno;
    }

    sal_memset(cmd, 0, sizeof(mgmt_command_t));
    sal_memset(reply, 0, sizeof(mgmt_reply_t));

    cmd->cmd = uswap32(MGMT_AVB);
    cmd->u.avb.avb_cmd = uswap32(MGMT_AVB_AVNU_OPERATIONAL_PDELAY_INTERVAL_SET);
    cmd->u.avb.u.avnu_pdelay_oper.port = uswap32(port);
    cmd->u.avb.u.avnu_pdelay_oper.pdelay_oper_intl_val = uswap32(log_val);
    cmd->len = uswap32(MGMT_AVB_COMMAND_LEN + sizeof(cmd->u.avb.u.avnu_pdelay_oper));
    errno = mgmt_cmd_reply(info, cmd, reply, 0);
    if (errno) {
        return errno;
    }

    return 0;
}

/*
 * @api
 * mgmt_avnu_initial_log_sync_interval_set
 *
 * @brief
 * configure the avnu sync interval value
 *
 * @param=interval - sync interval
 * @param=port - port number
 *
 * @returns 0 on success, !0 on error
 */
int32 mgmt_avnu_initial_log_sync_interval_set(mgmt_info_t *info, uint32 port, int32 interval)
{
    int32 errno = -1;
    mgmt_command_t cmd_packet;
    mgmt_command_t *cmd = &cmd_packet;
    mgmt_reply_t reply_packet;
    mgmt_reply_t *reply = &reply_packet;
    if (info == NULL) {
      printk("%s :: mgmt_info_t pointer is NULL\r\n", __FUNCTION__);
      return errno;
    }

    if (IS_EXTERNAL_INTF_INVALID(port)) {
      printk("Invalid port number %d\r\n", port);
      return errno;
    }

    if (MGMT_AVNU_SYNC_INTERVAL_IS_NOT_VALID(interval)) {
      printk("Invalid sync interval, val = %d\r\n", interval);
      return errno;
    }

    sal_memset(cmd, 0, sizeof(mgmt_command_t));
    sal_memset(reply, 0, sizeof(mgmt_reply_t));

    cmd->cmd = uswap32(MGMT_AVB);
    cmd->u.avb.avb_cmd = uswap32(MGMT_AVB_AVNU_INITIAL_LOG_SYNC_INTL_SET);
    cmd->u.avb.u.avnu_sync_intl.port = uswap32(port);
    cmd->u.avb.u.avnu_sync_intl.sync_intl_val = uswap32(interval);
    cmd->len = uswap32(MGMT_AVB_COMMAND_LEN + sizeof(cmd->u.avb.u.avnu_sync_intl));
    errno = mgmt_cmd_reply(info, cmd, reply, 0);
    if (errno) {
        return errno;
    }

    return 0;
}

/*
 * @api
 * mgmt_avnu_global_config_get
 *
 * @brief
 * get global configuration of avnu mode
 *
 * @param=config - pointer to global avnu configuration
 *
 * @returns 0 on success, !0 on error
 */
int32 mgmt_avnu_global_config_get(mgmt_info_t *info, mgmt_avnu_global_config_t *config)
{
    int32 errno = -1;
    mgmt_command_t cmd_packet;
    mgmt_command_t *cmd = &cmd_packet;
    mgmt_reply_t reply_packet;
    mgmt_reply_t *reply = &reply_packet;
    if (info == NULL) {
      printk("%s :: mgmt_info_t pointer is NULL\r\n", __FUNCTION__);
      return errno;
    }

    if (config == NULL) {
      printk("%s :: mgmt_avnu_global_config_t pointer is null\r\n", __FUNCTION__);
      return errno;
    }

    sal_memset(cmd, 0, sizeof(mgmt_command_t));
    sal_memset(reply, 0, sizeof(mgmt_reply_t));

    cmd->cmd = uswap32(MGMT_AVB);
    cmd->u.avb.avb_cmd = uswap32(MGMT_AVB_AVNU_GLOBAL_CONFIG_GET);
    cmd->len = uswap32(MGMT_AVB_COMMAND_LEN);
    errno = mgmt_cmd_reply(info, cmd, reply, 0);
    if (errno) {
        return errno;
    }

    config->clock_state = uswap32(reply->u.avb.avnu_global_config.clock_state);
    config->sync_absence_timeout = uswap32(reply->u.avb.avnu_global_config.sync_absence_timeout);
    config->admin_mode = uswap32(reply->u.avb.avnu_global_config.admin_mode);
    config->syncPropTime = uswap32(reply->u.avb.avnu_global_config.syncPropTime);
    config->pdelayRespTime = uswap32(reply->u.avb.avnu_global_config.pdelayRespTime);
    return 0;
}

/*
 * @api
 * mgmt_avnu_interface_config_get
 *
 * @brief
 * get interface configuration of avnu mode
 *
 * @param=config - pointer to interface avnu configuration
 * @param=port - port number
 *
 * @returns 0 on success, !0 on error
 */
int32 mgmt_avnu_interface_config_get(mgmt_info_t *info,uint32 port, mgmt_avnu_interface_config_t *config)
{
    int32 errno = -1;
    mgmt_command_t cmd_packet;
    mgmt_command_t *cmd = &cmd_packet;
    mgmt_reply_t reply_packet;
    mgmt_reply_t *reply = &reply_packet;
    if ((info == NULL) || (config == NULL)){
      printk("%s :: Invalid input parameter(info = 0x%x,config = 0x%x)\r\n", __FUNCTION__, (unsigned int)info, (unsigned int)config);
      return errno;
    }

    if ((IS_EXTERNAL_INTF_INVALID(port))) {
      printk("%s :: Invalid port number %d\r\n", __FUNCTION__, port);
      return errno;
    }

    sal_memset(cmd, 0, sizeof(mgmt_command_t));
    sal_memset(reply, 0, sizeof(mgmt_reply_t));

    cmd->cmd = uswap32(MGMT_AVB);
    cmd->u.avb.avb_cmd = uswap32(MGMT_AVB_AVNU_INTERFACE_CONFIG_GET);
    cmd->u.avb.u.avnu_interface_config.port = uswap32(port);
    cmd->len = uswap32(MGMT_AVB_COMMAND_LEN + sizeof(cmd->u.avb.u.avnu_interface_config));
    errno = mgmt_cmd_reply(info, cmd, reply, 0);
    if (errno) {
        return errno;
    }

    config->port_role = uswap32(reply->u.avb.avnu_interface_config.port_role);
    config->asCapable_mode = uswap32(reply->u.avb.avnu_interface_config.asCapable_mode);
    config->pdelay_val = uswap32(reply->u.avb.avnu_interface_config.pdelay_val);
    config->pdelay_init_interval = (int8)uswap32(reply->u.avb.avnu_interface_config.pdelay_init_interval);
    config->sync_init_interval = (int8)uswap32(reply->u.avb.avnu_interface_config.sync_init_interval);
    config->pdelay_oper_interval = (int8)uswap32(reply->u.avb.avnu_interface_config.pdelay_oper_interval);
    config->syncTimeout = uswap32(reply->u.avb.avnu_interface_config.syncTimeout);
    config->lostResponse = uswap32(reply->u.avb.avnu_interface_config.lostResponse);
    config->avnuNbrRateRatio = uswap32(reply->u.avb.avnu_interface_config.avnuNbrRateRatio);
    return 0;
}

/*
 * @api
 * mgmt_avnu_global_status
 *
 * @brief
 * Global status of avnu mode
 *
 * @returns 0 on success, !0 on error
 */

int32 mgmt_avnu_global_status(mgmt_info_t *info, mgmt_avnu_global_status_t *avnu_status)
{
    int32 errno = -1;
    mgmt_command_t cmd_packet;
    mgmt_command_t *cmd = &cmd_packet;
    mgmt_reply_t reply_packet;
    mgmt_reply_t *reply = &reply_packet;

    if ((info == NULL) || (avnu_status == NULL)) {
      printk("%s :: Invalid input parameter(info = 0x%x, avnu_status = 0x%x)\r\n", __FUNCTION__, (unsigned int)info, (unsigned int)avnu_status);
      return errno;
    }

    sal_memset(cmd, 0, sizeof(mgmt_command_t));
    sal_memset(reply, 0, sizeof(mgmt_reply_t));

    cmd->cmd = uswap32(MGMT_AVB);
    cmd->u.avb.avb_cmd = uswap32(MGMT_AVB_AVNU_GLOBAL_STATUS);
    cmd->len = uswap32(MGMT_AVB_COMMAND_LEN);
    errno = mgmt_cmd_reply(info, cmd, reply, 0);
    if (errno) {
        return errno;
    }

    avnu_status->clock_type = uswap32(reply->u.avb.avnu_global_status.clock_type);
    avnu_status->dot1as_status = uswap32(reply->u.avb.avnu_global_status.dot1as_status);
    avnu_status->gm_status = uswap32(reply->u.avb.avnu_global_status.gm_status);
    avnu_status->slave_port = (int8)uswap32(reply->u.avb.avnu_global_status.slave_port);
    avnu_status->rateRatioIn = uswap32(reply->u.avb.avnu_global_status.rateRatioIn);
    avnu_status->rateRatioOut = uswap32(reply->u.avb.avnu_global_status.rateRatioOut);
    avnu_status->isSyncReceived = uswap32(reply->u.avb.avnu_global_status.isSyncReceived);
    avnu_status->bridgeLocalTime.sec_u = uswap32(reply->u.avb.avnu_global_status.bridgeLocalTime.sec_u);
    avnu_status->bridgeLocalTime.sec_l = uswap32(reply->u.avb.avnu_global_status.bridgeLocalTime.sec_l);
    avnu_status->bridgeLocalTime.ns = uswap32(reply->u.avb.avnu_global_status.bridgeLocalTime.ns);
    avnu_status->local_clk_id_upper = uswap32(reply->u.avb.avnu_global_status.local_clk_id_upper);
    avnu_status->local_clk_id_lower = uswap32(reply->u.avb.avnu_global_status.local_clk_id_lower);

    return 0;
}


/*
 * @api
 * mgmt_avnu_device_state_get
 *
 * @brief
 * AVnu Device State
 *
 * @returns 0 on success, !0 on error
 */

int32 mgmt_avnu_device_state_get(mgmt_info_t *info, uint32 *state)
{
  int32 errno = -1;
  mgmt_command_t cmd_packet;
  mgmt_command_t *cmd = &cmd_packet;
  mgmt_reply_t reply_packet;
  mgmt_reply_t *reply = &reply_packet;

  if ((info == NULL) || (state == NULL)) {
    printk("%s :: Invalid input parameter(info =0x%x, state = 0x%x)\r\n",
                                __FUNCTION__, (unsigned int)info, (unsigned int)state);
    return errno;
  }

  sal_memset(cmd, 0, sizeof(mgmt_command_t));
  sal_memset(reply, 0, sizeof(mgmt_reply_t));

  cmd->cmd = uswap32(MGMT_AVB);
  cmd->u.avb.avb_cmd = uswap32(MGMT_AVB_AVNU_DEVICE_STATE_GET);
  cmd->len = uswap32(MGMT_AVB_COMMAND_LEN);
  errno = mgmt_cmd_reply(info, cmd, reply, 0);
  if (errno) {
    return errno;
  }

  *state = uswap32(reply->u.avb.avnu_device_state_get.state);
  return 0;
}

int32 mgmt_avnu_pdelay_get(mgmt_info_t *info, uint32_t port, mgmt_avnu_pdelay_t *avnu_pdelay)
{
  int32 errno = -1;
  mgmt_command_t cmd_packet;
  mgmt_command_t *cmd = &cmd_packet;
  mgmt_reply_t reply_packet;
  mgmt_reply_t *reply = &reply_packet;

  if ((info == NULL) || (avnu_pdelay == NULL)) {
      printk("%s :: Invalid input parameter(info =0x%x, state = 0x%x)\r\n",
              __FUNCTION__, (unsigned int)info, (unsigned int)avnu_pdelay);
      return errno;
  }

  sal_memset(cmd, 0, sizeof(mgmt_command_t));
  sal_memset(reply, 0, sizeof(mgmt_reply_t));

  cmd->cmd = uswap32(MGMT_AVB);
  cmd->u.avb.avb_cmd = uswap32(MGMT_AVB_AVNU_PDELAY);
  cmd->u.avb.u.avnu_pdelay.port = uswap32(port);
  cmd->len = uswap32(MGMT_AVB_COMMAND_LEN) + sizeof(cmd->u.avb.u.avnu_pdelay);
  errno = mgmt_cmd_reply(info, cmd, reply, 0);
  if (errno) {
    return errno;
  }

  avnu_pdelay->pDelayT1.sec_u = uswap32(reply->u.avb.avnu_pdelay.pDelayT1.sec_u);
  avnu_pdelay->pDelayT1.sec_l =  uswap32(reply->u.avb.avnu_pdelay.pDelayT1.sec_l);
  avnu_pdelay->pDelayT2.sec_u = uswap32(reply->u.avb.avnu_pdelay.pDelayT2.sec_u);
  avnu_pdelay->pDelayT2.sec_l =  uswap32(reply->u.avb.avnu_pdelay.pDelayT2.sec_l);
  avnu_pdelay->pDelayT3.sec_u = uswap32(reply->u.avb.avnu_pdelay.pDelayT3.sec_u);
  avnu_pdelay->pDelayT3.sec_l =  uswap32(reply->u.avb.avnu_pdelay.pDelayT3.sec_l);
  avnu_pdelay->pDelayT4.sec_u = uswap32(reply->u.avb.avnu_pdelay.pDelayT4.sec_u);
  avnu_pdelay->pDelayT4.sec_l =  uswap32(reply->u.avb.avnu_pdelay.pDelayT4.sec_l);
  avnu_pdelay->nbrPDelay = uswap32(reply->u.avb.avnu_pdelay.nbrPDelay);
  avnu_pdelay->nbrRR = uswap32(reply->u.avb.avnu_pdelay.nbrRR);
  avnu_pdelay->pDelayT1.ns = uswap32(reply->u.avb.avnu_pdelay.pDelayT1.ns);
  avnu_pdelay->pDelayT2.ns = uswap32(reply->u.avb.avnu_pdelay.pDelayT2.ns);
  avnu_pdelay->pDelayT3.ns = uswap32(reply->u.avb.avnu_pdelay.pDelayT3.ns);
  avnu_pdelay->pDelayT4.ns = uswap32(reply->u.avb.avnu_pdelay.pDelayT4.ns);
  swap_str(avnu_pdelay->dot1asCapable, reply->u.avb.avnu_pdelay.dot1asCapable, sizeof(avnu_pdelay->dot1asCapable));

  return 0;
}

/*
 * @api
 * mgmt_get_chip_info
 *
 * @brief
 * Gets chip info
 *
 * @returns none
 */

int32 mgmt_get_chip_info(uint16 *rev, uint32 *dev)
{
    uint16 dev_lo;
    uint16 dev_hi;
    if (rev == NULL || dev == NULL) {
      printk("%s :: Invalid input param(rev = 0x%x, dev = 0x%x)\r\n",
                                                __FUNCTION__, (unsigned int)rev, (unsigned int)dev);
      return ERR_PARAM;
    }
    mgmt_spi_rd16(MISC_MODEL_REV_NUM, rev);
    mgmt_spi_rd16(MISC_DEVICEID_LO, &dev_lo);
    /* Dev_ID[19:0] = {device_id_hi[7:0], device_id_lo[11:0]} */
    dev_lo &= MISC_DEVICEID_LO_DEVICE_ID_LO_MASK;
    mgmt_spi_rd16(MISC_DEVICEID_HI, &dev_hi);
    *dev  = (uint32)(dev_hi & MISC_DEVICEID_HI_DEVICE_ID_HI_MASK) << MISC_DEVICEID_LO_DEVICE_ID_LO_BITS;
    *dev |= dev_lo ;
    return 0;
}

/*
 *  @api
 * mgmt_heartbeat_set
 *
 * @brief
 * Heartbeat interval set
 *
 * @returns 0 on success, !0 on error
 */

int32 mgmt_heartbeat_set(mgmt_info_t *info, uint32 heartbeat)
{
    int32 errno = -1;
    mgmt_command_t cmd_packet;
    mgmt_command_t *cmd = &cmd_packet;
    mgmt_reply_t reply_packet;
    mgmt_reply_t *reply = &reply_packet;

    if (info == NULL) {
      printk("%s :: mgmt_info_t pointer is NULL\r\n", __FUNCTION__);
      return errno;
    }

    if (heartbeat < MGMT_GLOBAL_HEARTBEAT_INTERVAL_MIN ||
                        heartbeat > MGMT_GLOBAL_HEARTBEAT_INTERVAL_MAX) {
      printk("Invalid Heartbeat value %d min=%u max=%u\r\n", heartbeat,
      MGMT_GLOBAL_HEARTBEAT_INTERVAL_MIN, MGMT_GLOBAL_HEARTBEAT_INTERVAL_MAX);
      return errno;
    }

    sal_memset(cmd, 0, sizeof(mgmt_command_t));
    sal_memset(reply, 0, sizeof(mgmt_reply_t));

    cmd->cmd = uswap32(MGMT_AVB);
    cmd->u.avb.avb_cmd = uswap32(MGMT_AVB_HEARTBEAT_INTERVAL_SET);
    cmd->u.avb.u.heartbeat_interval.heartbeat_val = uswap32(heartbeat);
    cmd->len = uswap32(MGMT_AVB_COMMAND_LEN + sizeof(cmd->u.avb.u.heartbeat_interval));
    errno = mgmt_cmd_reply(info, cmd, reply, 0);
    if (errno) {
        return errno;
    }
    return 0;
}

/* mgmt_avb_async_events_get
 *
 * @brief
 * get occured events from polar since last read
 *
 * @param = info - management info
 * @param = logEntry - pointer to mgmt_avb_async_events_get_t
 * @returns 0 on success, !0 on error
 */

int32 mgmt_avb_async_events_get(mgmt_info_t *info, mgmt_avb_async_events_get_t *events)
{
  int32 errno = -1;
  mgmt_command_t cmd_packet;
  mgmt_command_t *cmd = &cmd_packet;
  mgmt_reply_t reply_packet;
  mgmt_reply_t *reply = &reply_packet;

  if ((info == NULL) || (events == NULL)) {
    printk("%s :: Invalid input parameter(info= x%x,events = 0x%x)\r\n",
                                __FUNCTION__, (unsigned int)info, (unsigned int)events);
    return errno;
  }
  sal_memset(cmd, 0, sizeof(mgmt_command_t));
  sal_memset(reply, 0, sizeof(mgmt_reply_t));

  cmd->cmd = uswap32(MGMT_AVB);
  cmd->u.avb.avb_cmd = uswap32(MGMT_AVB_ASYNC_EVENTS_GET);
  cmd->len = uswap32(MGMT_AVB_COMMAND_LEN);
  errno = mgmt_cmd_reply(info, cmd, reply, 0);
  if (errno) {
    return errno;
  }

  events->port0_events = uswap32(reply->u.avb.avb_async_events.port0_events);
  events->port1_events = uswap32(reply->u.avb.avb_async_events.port1_events);
  events->port2_events = uswap32(reply->u.avb.avb_async_events.port2_events);
  events->port3_events = uswap32(reply->u.avb.avb_async_events.port3_events);
  events->port4_events = uswap32(reply->u.avb.avb_async_events.port4_events);
  events->port5_events = uswap32(reply->u.avb.avb_async_events.port5_events);
  events->port6_events = uswap32(reply->u.avb.avb_async_events.port6_events);
  events->port8_events = uswap32(reply->u.avb.avb_async_events.port8_events);
  events->global_events = uswap32(reply->u.avb.avb_async_events.global_events);
  return 0;
}

#endif
