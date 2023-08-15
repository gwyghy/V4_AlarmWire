#ifndef BCM89530_H__
#define BCM89530_H__

/**
 * m = memory, c = core, r = register, f = field, d = data.
 */
#if !defined(GET_FIELD) && !defined(SET_FIELD)
#define BRCM_ALIGN(c,r,f)   c##_##r##_##f##_ALIGN
#define BRCM_BITS(c,r,f)    c##_##r##_##f##_BITS
#define BRCM_MASK(c,r,f)    c##_##r##_##f##_MASK
#define BRCM_SHIFT(c,r,f)   c##_##r##_##f##_SHIFT

#define GET_FIELD(m,c,r,f) \
	((((m) & BRCM_MASK(c,r,f)) >> BRCM_SHIFT(c,r,f)) << BRCM_ALIGN(c,r,f))

#define SET_FIELD(m,c,r,f,d) \
	((m) = (((m) & ~BRCM_MASK(c,r,f)) | ((((d) >> BRCM_ALIGN(c,r,f)) << \
	 BRCM_SHIFT(c,r,f)) & BRCM_MASK(c,r,f))) \
	)

#define SET_TYPE_FIELD(m,c,r,f,d) SET_FIELD(m,c,r,f,c##_##d)
#define SET_NAME_FIELD(m,c,r,f,d) SET_FIELD(m,c,r,f,c##_##r##_##f##_##d)
#define SET_VALUE_FIELD(m,c,r,f,d) SET_FIELD(m,c,r,f,d)

#endif /* GET & SET */

/****************************************************************************
 * Core Enums.
 ***************************************************************************/
/****************************************************************************
 * bcm89530_sys_cfg_ETH
 ***************************************************************************/
#define ETH_ETH_CTRL                   0x00106000 /* Eth General Control */
#define ETH_INTR_MASK                  0x00106004 /* Interrupt Mask */
#define ETH_INTR                       0x00106008 /* Masked Interrupts */
#define ETH_INTR_RAW                   0x0010600c /* Raw Interrupts */
#define ETH_INTR_CLR                   0x00106010 /* Clear Interrupts (both RAW and INTR) */
#define ETH_MCADDRF0                   0x00106020 /* Multicast Address Filter 0 */
#define ETH_MCADDRF1                   0x00106024 /* Multicast Address Filter 1 */
#define ETH_LSADDR0                    0x00106028 /* Local Station Address Low */
#define ETH_LSCADDR1                   0x0010602c /* Local Station Address High */
#define ETH_OUIADDR0                   0x00106030 /* OUI Address Low */
#define ETH_OUIADDR1                   0x00106034 /* Local Station Address High */
#define ETH_PHYCTRL                    0x00106040 /* Onboard PHY control */
#define ETH_RBUFFCTRL                  0x00106104 /* Receiver Buffer Control */
#define ETH_RBASE                      0x00106110 /* Rx Descriptor Ring Base */
#define ETH_RBCFG                      0x00106114 /* Rx Descriptor Ring Configuration */
#define ETH_RBDPTR                     0x00106118 /* Pointer to Current HW Rx Descriptor */
#define ETH_RSWPTR                     0x0010611c /* Pointer to Current SW Rx Descriptor */
#define ETH_RBCFG_EXT                  0x00106120 /* Rx Descriptor Ring Configuration Extension */
#define ETH_TBASE                      0x00106210 /* Tx Descriptor Ring Base */
#define ETH_TBCFG                      0x00106214 /* Tx Descriptor Ring Configuration */
#define ETH_TBDPTR                     0x00106218 /* Pointer to Current HW Tx Descriptor */
#define ETH_TSWPTR                     0x0010621c /* Pointer to Current SW Tx Descriptor */
#define ETH_MACBP                      0x00106404 /* MAC HD Control */
#define ETH_MACCFG                     0x00106408 /* UniMAC Configuration */
#define ETH_MACADDR0                   0x0010640c /* Ethernet MAC Address Low */
#define ETH_MACADDR1                   0x00106410 /* Ethernet MAC Address Low */
#define ETH_MAXFRM                     0x00106414 /* Maximum allowed frame size */
#define ETH_MACPQ                      0x00106418 /* Ethernet Pause Quanta */
#define ETH_RXFIFO_EMPTY               0x0010641c /* MAC RX FIFO empty watermark */
#define ETH_RXFIFO_FULL                0x00106420 /* MAC RX FIFO full watermark */
#define ETH_TXFIFO_EMPTY               0x00106424 /* MAC TX FIFO empty watermark */
#define ETH_TXFIFO_FULL                0x00106428 /* MAC TX FIFO full watermark */
#define ETH_RXFIFO_AEMPTY              0x0010642c /* MAC RX FIFO almost empty watermark */
#define ETH_RXFIFO_AFULL               0x00106430 /* MAC RX FIFO almost full watermark */
#define ETH_TXFIFO_AEMPTY              0x00106434 /* MAC TX FIFO almost empty watermark */
#define ETH_TXFIFO_AFULL               0x00106438 /* MAC TX FIFO almost full watermark */
#define ETH_MACMODE                    0x00106444 /* Ethernet Mac Status */
#define ETH_VLANTAG0                   0x00106448 /* Outer VLAN Tag */
#define ETH_TXIPG                      0x0010645c /* Transmit Inter Packet Gap */
#define ETH_TXPCTRL                    0x00106730 /* Transmit Pause Control */
#define ETH_TXFIFOF                    0x00106734 /* Transmit Pause Control */
#define ETH_RXFIFOSTAT                 0x00106738 /* Rx FIFO Status */
#define ETH_TXFIFOSTAT                 0x0010673c /* Tx FIFO Status */
#define ETH_TXOCTGOOD                  0x00106800 /* Total Good Octets Transmitted */
#define ETH_TXFRMGOOD                  0x00106804 /* Total Good Frames Transmitted */
#define ETH_TXOCTTOTAL                 0x00106808 /* Total Octets Transmitted */
#define ETH_TXFRMTOTAL                 0x0010680c /* Total Frames Transmitted */
#define ETH_TXBCASTGOOD                0x00106810 /* Total Good Broadcast Frames Transmitted */
#define ETH_TXMCASTGOOD                0x00106814 /* Total Good Multicast Frames Transmitted */
#define ETH_TX64                       0x00106818 /* Total 64B Frames Transmitted */
#define ETH_TX65_127                   0x0010681c /* Total 65-127B Frames Transmitted */
#define ETH_TX128_255                  0x00106820 /* Total 128-255B Frames Transmitted */
#define ETH_TX256_511                  0x00106824 /* Total 256-511B Frames Transmitted */
#define ETH_TX512_1023                 0x00106828 /* Total 512-1023B Frames Transmitted */
#define ETH_TX1024_MAX                 0x0010682c /* Total 1024B-MAXFRM Frames Transmitted */
#define ETH_TXJABBER                   0x00106830 /* Total Frames > MAXFRM with bad CRC */
#define ETH_TXJUMBO                    0x00106834 /* Total Frames > MAXFRM with good CRC */
#define ETH_TXFRAG                     0x00106838 /* Total Frames < 64B with bad CRC */
#define ETH_TXUNDERRUN                 0x0010683c /* Total Frames that experienced an underrun */
#define ETH_TXCOLTOTAL                 0x00106840 /* Total number of collisions seen */
#define ETH_TX1COL                     0x00106844 /* Total Frames that saw exactly one collision */
#define ETH_TXMCOL                     0x00106848 /* Total Frames that saw more than one collision */
#define ETH_TXEXCOL                    0x0010684c /* Total frames that saw excessive collisions */
#define ETH_TXLATE                     0x00106850 /* Total frames that saw late collisions */
#define ETH_TXDEFER                    0x00106854 /* Total frames that were defered */
#define ETH_TXNOCRS                    0x00106858 /* Total frames that saw loss of carrier */
#define ETH_TXPAUSE                    0x0010685c /* Total pause frames sent */
#define ETH_TXCNTOF                    0x0010687c /* Tx Counter Overflow Reporting */
#define ETH_RXOCTGOOD                  0x00106880 /* Total Good Octets Received */
#define ETH_RXFRMGOOD                  0x00106884 /* Total Good Frames Received */
#define ETH_RXOCTTOTAL                 0x00106888 /* Total Octets Received */
#define ETH_RXFRMTOTAL                 0x0010688c /* Total Frames Received */
#define ETH_RXBCASTGOOD                0x00106890 /* Total Good Broadcast Frames Received */
#define ETH_RXMCASTGOOD                0x00106894 /* Total Good Multicast Frames Received */
#define ETH_RX64                       0x00106898 /* Total 64B Frames Received */
#define ETH_RX65_127                   0x0010689c /* Total 65-127B Frames Received */
#define ETH_RX128_255                  0x001068a0 /* Total 128-255B Frames Received */
#define ETH_RX256_511                  0x001068a4 /* Total 256-511B Frames Received */
#define ETH_RX512_1023                 0x001068a8 /* Total 512-1023B Frames Received */
#define ETH_RX1024_MAX                 0x001068ac /* Total 1024B-MAXFRM Frames Received */
#define ETH_RXJABBER                   0x001068b0 /* Total Frames > MAXFRM with bad CRC */
#define ETH_RXJUMBO                    0x001068b4 /* Total Frames > MAXFRM with good CRC */
#define ETH_RXFRAG                     0x001068b8 /* Total Frames < 64B with bad CRC */
#define ETH_RXOVERRUN                  0x001068bc /* Total Frames that experienced an overrun */
#define ETH_RXCRCALIGN                 0x001068c0 /* Rx Frames with bad CRC or alignment */
#define ETH_RXUSIZE                    0x001068c4 /* Total Frames < 64B with good CRC */
#define ETH_RXCRC                      0x001068c8 /* Rx Frames with bad CRC but even alignment */
#define ETH_RXALIGN                    0x001068cc /* Rx Frames with bad CRC and odd alignment */
#define ETH_RXCDERR                    0x001068d0 /* Rx Frames with Code Error (Rx_ER) */
#define ETH_RXPAUSE                    0x001068d4 /* Rx Pause Frames received */
#define ETH_RXCTRLFM                   0x001068d8 /* Rx Control Frames received */
#define ETH_RXCNTOF                    0x001068fc /* Rx Counter Overflow Reporting */

/****************************************************************************
 * bcm89530_sys_cfg_MEM
 ***************************************************************************/
#define MEM_STATUS                     0x0010d000 /* Mem Status register */
#define MEM_CNT                        0x0010d004 /* Single bit error counter and threshold */
#define MEM_CTRL                       0x0010d008 /* Mem main control register */
#define MEM_BAD_ADDR                   0x0010d00c /* Error snapshot:address */
#define MEM_BAD_CMD                    0x0010d010 /* Error snapshot:side band and error type */
#define MEM_BAD_DATA                   0x0010d014 /* Error snapshot: lower 32 bits of data */
#define MEM_BAD_DATA1                  0x0010d018 /* Error snapshot: upper 32 bits of data */
#define MEM_BAD_ECC                    0x0010d01c /* Error snapshot: parity and ECC */


/****************************************************************************
 * bcm89530_sys_cfg_SPI1
 ***************************************************************************/
#define SPI1_SSPCR0                    0x00141000 /* Control Register 0. (See PL022 TRM for details) */
#define SPI1_SSPCR1                    0x00141004 /* Control Register 1. (See PL022 TRM for details) */
#define SPI1_SSPDR                     0x00141008 /* Receive FIFO (read) and transmit FIFO (write) data register. (See PL022 TRM for details) */
#define SPI1_SSPSR                     0x0014100c /* Status register. (See PL022 TRM for details) */
#define SPI1_SSPCPSR                   0x00141010 /* Clock prescale register. (See PL022 TRM for details) */
#define SPI1_SSPIMSC                   0x00141014 /* Interrupt mask set and clear register. (See PL022 TRM for details) */
#define SPI1_SSPRIS                    0x00141018 /* Raw interrupt status register. (See PL022 TRM for details) */
#define SPI1_SSPMIS                    0x0014101c /* Masked interrupt status register. (See PL022 TRM for details) */
#define SPI1_SSPICR                    0x00141020 /* Interrupt clear register. (See PL022 TRM for details) */
#define SPI1_SSPDMACR                  0x00141024 /* DMA control register. (See PL022 TRM for details) */
#define SPI1_SSPPERIPHID0              0x00141fe0 /* "Peripheral identification register bits [7:0]. (See PL022 TRM for details)" */
#define SPI1_SSPPERIPHID1              0x00141fe4 /* "Peripheral identification register bits [15:8]. (See PL022 TRM for details)" */
#define SPI1_SSPPERIPHID2              0x00141fe8 /* "Peripheral identification register bits [23:16]. (See PL022 TRM for details)" */
#define SPI1_SSPPERIPHID3              0x00141fec /* "Peripheral identification register bits [31:24]. (See PL022 TRM for details)" */
#define SPI1_SSPPCELLID0               0x00141ff0 /* "PrimeCell identification register bits [7:0]. (See PL022 TRM for details)" */
#define SPI1_SSPPCELLID1               0x00141ff4 /* "PrimeCell identification register bits [15:8]. (See PL022 TRM for details)" */
#define SPI1_SSPPCELLID2               0x00141ff8 /* "PrimeCell identification register bits [23:16]. (See PL022 TRM for details)" */
#define SPI1_SSPPCELLID3               0x00141ffc /* "PrimeCell identification register bits [31:24]. (See PL022 TRM for details)" */

/****************************************************************************
 * bcm89530_sys_cfg_I2C
 ***************************************************************************/
#define I2C_BSCCS                      0x00147020 /* I2C Control and Status Register */
#define I2C_BSCTIM                     0x00147024 /* I2C Time Register */
#define I2C_BSCDAT                     0x00147028 /* I2C Data Register */
#define I2C_BSCTOUT                    0x0014702c /* I2C Timeout Register */
#define I2C_BSCFCR                     0x0014703c /* I2C FIFO Control Register */
#define I2C_BSCFIFORDOUT               0x00147040 /* I2C FIFO Read Out Register */
#define I2C_BSCIER                     0x00147044 /* I2C FIFO Interrupt Enable Register */
#define I2C_BSCISR                     0x00147048 /* I2C FIFO Interrupt Status Register */
#define I2C_BSCCLKEN                   0x0014704c /* I2C clock enable */


/****************************************************************************
 * bcm89530_sys_cfg_CFG
 ***************************************************************************/
#define CFG_SR                         0x00148000 /* Status register */
#define CFG_CFG_CPUSYS_MISC            0x00148004 /* CPUSYS misc config registers */
#define CFG_TM_0                       0x00148008 /* TM Bits [31:0] */
#define CFG_TM_1                       0x0014800c /* TM Bits [63:32] */
#define CFG_DEBUG_EN                   0x00148010 /* Debug enable register */
#define CFG_PARITY_DISABLE             0x00148020 /* Disables ECC and parity */
#define CFG_SRAB_CMDSTAT               0x0014802c /* Switch Register Access Bridge command status */
#define CFG_SRAB_WDH                   0x00148030 /* "Switch register access ,write data, high order word" */
#define CFG_SRAB_WDL                   0x00148034 /* "Switch register access ,write data, low order word" */
#define CFG_SRAB_RDH                   0x00148038 /* "Switch register access read data, high order word" */
#define CFG_SRAB_RDL                   0x0014803c /* "Switch register access read data, low order word" */
#define CFG_SW_IF                      0x00148040 /* Switch interface and control */
#define CFG_SW_INTR_CLR                0x00148044 /* Switch interrupt clear register */
#define CFG_QSPI_IO_STATUS             0x00148048 /* QSPI IO status register */
#define CFG_QSPI_IO_CONTROL            0x0014804c /* QSPI IO control direct access register */
#define CFG_QSPI_IP_REVID              0x00148050 /* QSPI Revision identifier access register */
#define CFG_SPI_CRC_CONTROL            0x00148054 /* SPI CRC control register */
#define CFG_SPI_CRC_STATUS             0x00148058 /* SPI CRC result status register */
#define CFG_CPU_INTR_RAW               0x0014805c /* VIC raw interrupt register */
#define CFG_CPU_INTR_STAT              0x00148060 /* VIC Status interrupt register */
#define CFG_CPU_INTR_MASK              0x00148064 /* VIC MASK interrupt register */
#define CFG_CPU_INTR_FORCE             0x00148068 /* VIC FORCE interrupt register */
#define CFG_CPU_INTR_CFG               0x0014806c /* PAD Output interrupt configuration register */
#define CFG_SPI_CRC_IDLE_CYCLE_COUNT   0x00148070 /* SPI CRC idle cycle count */
#define CFG_AHB2RDB_TIMEOUT            0x00148074 /* AHB2RDB timeout counter and status */

/****************************************************************************
 * bcm89530_sys_cfg_SPI0
 ***************************************************************************/
#define SPI0_SSPCR0                    0x0014a000 /* Control Register 0. (See PL022 TRM for details) */
#define SPI0_SSPCR1                    0x0014a004 /* Control Register 1. (See PL022 TRM for details) */
#define SPI0_SSPDR                     0x0014a008 /* Receive FIFO (read) and transmit FIFO (write) data register. (See PL022 TRM for details) */
#define SPI0_SSPSR                     0x0014a00c /* Status register. (See PL022 TRM for details) */
#define SPI0_SSPCPSR                   0x0014a010 /* Clock prescale register. (See PL022 TRM for details) */
#define SPI0_SSPIMSC                   0x0014a014 /* Interrupt mask set and clear register. (See PL022 TRM for details) */
#define SPI0_SSPRIS                    0x0014a018 /* Raw interrupt status register. (See PL022 TRM for details) */
#define SPI0_SSPMIS                    0x0014a01c /* Masked interrupt status register. (See PL022 TRM for details) */
#define SPI0_SSPICR                    0x0014a020 /* Interrupt clear register. (See PL022 TRM for details) */
#define SPI0_SSPDMACR                  0x0014a024 /* DMA control register. (See PL022 TRM for details) */
#define SPI0_SSPPERIPHID0              0x0014afe0 /* "Peripheral identification register bits [7:0]. (See PL022 TRM for details)" */
#define SPI0_SSPPERIPHID1              0x0014afe4 /* "Peripheral identification register bits [15:8]. (See PL022 TRM for details)" */
#define SPI0_SSPPERIPHID2              0x0014afe8 /* "Peripheral identification register bits [23:16]. (See PL022 TRM for details)" */
#define SPI0_SSPPERIPHID3              0x0014afec /* "Peripheral identification register bits [31:24]. (See PL022 TRM for details)" */
#define SPI0_SSPPCELLID0               0x0014aff0 /* "PrimeCell identification register bits [7:0]. (See PL022 TRM for details)" */
#define SPI0_SSPPCELLID1               0x0014aff4 /* "PrimeCell identification register bits [15:8]. (See PL022 TRM for details)" */
#define SPI0_SSPPCELLID2               0x0014aff8 /* "PrimeCell identification register bits [23:16]. (See PL022 TRM for details)" */
#define SPI0_SSPPCELLID3               0x0014affc /* "PrimeCell identification register bits [31:24]. (See PL022 TRM for details)" */

/****************************************************************************
 * bcm89530_sys_cfg_MMI
 ***************************************************************************/
#define MMI_MMI_CTRL                   0x0014d000 /* Eth General Control */
#define MMI_MMI_CMD                    0x0014d004 /* Interrupt Mask */

/****************************************************************************
 * bcm89530_top_bridge_brphys_brphys0_BRPHY0_CL45DEV1
 ***************************************************************************/
#define BRPHY0_CL45DEV1_PMD_IEEE_CTL1  0x09020000 /* IEEE PMA/PMD CONTROL 1 REGISTER (REG 1.0) */
#define BRPHY0_CL45DEV1_PMD_IEEE_ST1   0x09020002 /* IEEE PMA/PMD STATUS 1 REGISTER  (REG 1.1) */
#define BRPHY0_CL45DEV1_PMD_IEEE_DEV_ID0 0x09020004 /* IEEE PMA/PMD DEVICE IDENTIFIER PART 0 (REG 1.2) */
#define BRPHY0_CL45DEV1_PMD_IEEE_DEV_ID1 0x09020006 /* IEEE PMA/PMD DEVICE IDENTIFIER PART 1 (REG 1.3) */
#define BRPHY0_CL45DEV1_PMD_IEEE_DEV0  0x0902000a /* DEVICES IN PACKAGE REGISTER PART 0 (REG 1.5) */
#define BRPHY0_CL45DEV1_PMD_IEEE_DEV1  0x0902000c /* DEVICES IN PACKAGE REGISTER PART 1 (REG 1.6) */
#define BRPHY0_CL45DEV1_PMD_IEEE_PKG_ID0 0x0902001c /* PMA/PMD PACKAGE IDENTIFIER (REG 1.14) */
#define BRPHY0_CL45DEV1_PMD_IEEE_PKG_ID1 0x0902001e /* PMA/PMD PACKAGE IDENTIFIER (REG 1.15) */
#define BRPHY0_CL45DEV1_PMD_IEEE_TSYNC_CAP 0x09020e10 /* TimeSync PMA/PMD capability (REG 1.1800) */
#define BRPHY0_CL45DEV1_PMD_IEEE_TX_TSYNC_MAX_DELAY_LOWER 0x09020e12 /* Maximum PMA/PMD transmit path data delay, lower (REG 1.1801) */
#define BRPHY0_CL45DEV1_PMD_IEEE_TX_TSYNC_MAX_DELAY_UPPER 0x09020e14 /* Maximum PMA/PMD transmit path data delay, upper (REG 1.1802) */
#define BRPHY0_CL45DEV1_PMD_IEEE_TX_TSYNC_MIN_DELAY_LOWER 0x09020e16 /* Minimum PMA/PMD transmit path data delay, lower (REG 1.1803) */
#define BRPHY0_CL45DEV1_PMD_IEEE_TX_TSYNC_MIN_DELAY_UPPER 0x09020e18 /* Minimum PMA/PMD transmit path data delay, upper (REG 1.1804) */
#define BRPHY0_CL45DEV1_PMD_IEEE_RX_TSYNC_MAX_DELAY_LOWER 0x09020e1a /* Maximum PMA/PMD receive path data delay, lower (REG 1.1805) */
#define BRPHY0_CL45DEV1_PMD_IEEE_RX_TSYNC_MAX_DELAY_UPPER 0x09020e1c /* Maximum PMA/PMD receive path data delay, upper (REG 1.1806) */
#define BRPHY0_CL45DEV1_PMD_IEEE_RX_TSYNC_MIN_DELAY_LOWER 0x09020e1e /* Minimum PMA/PMD receive path data delay, lower (REG 1.1807) */
#define BRPHY0_CL45DEV1_PMD_IEEE_RX_TSYNC_MIN_DELAY_UPPER 0x09020e20 /* Minimum PMA/PMD receive path data delay, upper (REG 1.1808) */


/****************************************************************************
 * bcm89530_top_bridge_brphys_brphys0_BRPHY0_CL45DEV3
 ***************************************************************************/
#define BRPHY0_CL45DEV3_PCS_IEEE_CTL1  0x09060000 /* IEEE PCS CONTROL 1 REGISTER (REG 3.0) */
#define BRPHY0_CL45DEV3_PCS_IEEE_ST1   0x09060002 /* IEEE PCS STATUS 1 REGISTER  (REG 3.1) */
#define BRPHY0_CL45DEV3_PCS_IEEE_DEV_ID0 0x09060004 /* IEEE PCS DEVICE IDENTIFIER PART 0 (REG 3.2) */
#define BRPHY0_CL45DEV3_PCS_IEEE_DEV_ID1 0x09060006 /* IEEE PCS DEVICE IDENTIFIER PART 1 (REG 3.3) */
#define BRPHY0_CL45DEV3_PCS_IEEE_DEV0  0x0906000a /* DEVICES IN PACKAGE REGISTER PART 0 (REG 3.5) */
#define BRPHY0_CL45DEV3_PCS_IEEE_DEV1  0x0906000c /* DEVICES IN PACKAGE REGISTER PART 1 (REG 3.6) */
#define BRPHY0_CL45DEV3_PCS_IEEE_PKG_ID0 0x0906001c /* PCS PACKAGE IDENTIFIER (REG 3.14) */
#define BRPHY0_CL45DEV3_PCS_IEEE_PKG_ID1 0x0906001e /* PCS PACKAGE IDENTIFIER (REG 3.15) */
#define BRPHY0_CL45DEV3_PCS_EEE_CAP    0x09060028 /* PCS_EEE_CAP(REG 3.20) */
#define BRPHY0_CL45DEV3_PCS_EEE_WAKE_ERR_CNT 0x0906002c /* PCS_EEE_Wake_Err_Cnt(REG 3.22) */
#define BRPHY0_CL45DEV3_PCS_IEEE_TSYNC_CAP 0x09060e10 /* TimeSync PCS capability (REG 3.1800) */
#define BRPHY0_CL45DEV3_PCS_IEEE_TX_TSYNC_MAX_DELAY_LOWER 0x09060e12 /* Maximum PCS transmit path data delay, lower (REG 3.1801) */
#define BRPHY0_CL45DEV3_PCS_IEEE_TX_TSYNC_MAX_DELAY_UPPER 0x09060e14 /* Maximum PCS transmit path data delay, upper (REG 3.1802) */
#define BRPHY0_CL45DEV3_PCS_IEEE_TX_TSYNC_MIN_DELAY_LOWER 0x09060e16 /* Minimum PCS transmit path data delay, lower (REG 3.1803) */
#define BRPHY0_CL45DEV3_PCS_IEEE_TX_TSYNC_MIN_DELAY_UPPER 0x09060e18 /* Minimum PCS transmit path data delay, upper (REG 3.1804) */
#define BRPHY0_CL45DEV3_PCS_IEEE_RX_TSYNC_MAX_DELAY_LOWER 0x09060e1a /* Maximum PCS receive path data delay, lower (REG 3.1805) */
#define BRPHY0_CL45DEV3_PCS_IEEE_RX_TSYNC_MAX_DELAY_UPPER 0x09060e1c /* Maximum PCS receive path data delay, upper (REG 3.1806) */
#define BRPHY0_CL45DEV3_PCS_IEEE_RX_TSYNC_MIN_DELAY_LOWER 0x09060e1e /* Minimum PCS receive path data delay, lower (REG 3.1807) */
#define BRPHY0_CL45DEV3_PCS_IEEE_RX_TSYNC_MIN_DELAY_UPPER 0x09060e20 /* Minimum PCS receive path data delay, upper (REG 3.1808) */


/****************************************************************************
 * bcm89530_top_bridge_brphys_brphys0_BRPHY0_CL45DEV7
 ***************************************************************************/
#define BRPHY0_CL45DEV7_AN_CTRL        0x090e0000 /* Auto Neg Extended Next Page Control     (0x0000) (REG 7.0) */
#define BRPHY0_CL45DEV7_AN_STAT        0x090e0002 /* AN Status (0x0001) (REG 7.1) */
#define BRPHY0_CL45DEV7_AN_DEV_ID_LSB  0x090e0004 /* Auto Neg Device Identifier Lower 16 bit (0x0002) (REG 7.2) */
#define BRPHY0_CL45DEV7_AN_DEV_ID_MSB  0x090e0006 /* Auto Neg Device Identifier Upper 16 bit (0x0003) (REG 7.3) */
#define BRPHY0_CL45DEV7_AN_DEV_IN_PKG_LSB 0x090e000a /* Auto Neg Device In Package Lower 16 bit (0x0005) (REG 7.5) */
#define BRPHY0_CL45DEV7_AN_DEV_IN_PKG_MSB 0x090e000c /* Auto Neg Device In Package Upper 16 bit (0x0006) (REG 7.6) */
#define BRPHY0_CL45DEV7_AN_DEV_PKG_ID_LSB 0x090e001c /* Auto Neg Package ID Lower 16 bit(0x000e) (REG 7.14) */
#define BRPHY0_CL45DEV7_AN_DEV_PKG_ID_MSB 0x090e001e /* Auto Neg Package ID Upper 16 bit(0x000f) (REG 7.15) */
#define BRPHY0_CL45DEV7_AN_AD          0x090e0020 /* Auto Neg AD(0x0010) (REG 7.16) */
#define BRPHY0_CL45DEV7_AN_LPA         0x090e0026 /* AN LP base page ability (0x0013) (REG 7.19) */
#define BRPHY0_CL45DEV7_AN_XNPA        0x090e002c /* AN XNP transmit A (0x0016) (REG 7.22) */
#define BRPHY0_CL45DEV7_AN_XNPB        0x090e002e /* AN XNP transmit B (0x0017) (REG 7.23) */
#define BRPHY0_CL45DEV7_AN_XNPC        0x090e0030 /* AN XNP transmit C (0x0018) (REG 7.24) */
#define BRPHY0_CL45DEV7_LP_XNPA        0x090e0032 /* AN LP XNP ability A (0x0019) (REG 7.25) */
#define BRPHY0_CL45DEV7_LP_XNPB        0x090e0034 /* AN LP XNP ability B (0x001a) (REG 7.26) */
#define BRPHY0_CL45DEV7_LP_XNPC        0x090e0036 /* AN LP XNP ability C (0x001b) (REG 7.27) */
#define BRPHY0_CL45DEV7_TENG_AN_CTRL   0x090e0040 /* 10G Base-T AN Control Register (0x0020) (REG 7.32) */
#define BRPHY0_CL45DEV7_TENG_AN_STAT   0x090e0042 /* 10G Base-T AN Status Register  (0x0021) (REG 7.33) */
#define BRPHY0_CL45DEV7_EEE_ADV        0x090e0078 /* EEE Advertisement (0x003C) (REG 7.60 ???) */
#define BRPHY0_CL45DEV7_EEE_LP_ADV     0x090e007a /* EEE Link Partner Advertisement (0x003D) (REG 7.61 ???) */
#define BRPHY0_CL45DEV7_EEE_MODE_CTL   0x090e007c /* EEE Mode Control (0x003E) (REG 7.62 ???) */


/****************************************************************************
 * bcm89530_top_bridge_brphys_brphys0_BRPHY0_CL45VEN
 ***************************************************************************/
#define BRPHY0_CL45VEN_FORCE_LINK      0x090f0000 /* Force Link Register */
#define BRPHY0_CL45VEN_SELECTIVE_RESET 0x090f0002 /* Selective Reset Register */
#define BRPHY0_CL45VEN_TEST_FSM_EXT_NXT_PGS 0x090f0004 /* Test State Machine For Extended Next Pages Register --mvadkert */
#define BRPHY0_CL45VEN_TEST_FSM_NXT_PGS 0x090f0006 /* Test State Machine For Next Pages Register --mvadkert */
#define BRPHY0_CL45VEN_AN_MAN_TEST     0x090f0032 /* Auto Negotiation Manual Test Register */
#define BRPHY0_CL45VEN_AN_MAN_LNK_PARTNR_ABI_A 0x090f0034 /* Auto Negotiation Manual Link Partners Abilities Register A */
#define BRPHY0_CL45VEN_AN_MAN_LNK_PARTNR_ABI_B 0x090f0036 /* Auto Negotiation Manual Link Partners Abilities Register B */
#define BRPHY0_CL45VEN_LNK_PARTNR_NXT_PG_A 0x090f0038 /* Link Partner Next Page */
#define BRPHY0_CL45VEN_LNK_PARTNR_NXT_PG_B 0x090f003a /* Link Partner Next Page (cont.) */
#define BRPHY0_CL45VEN_LNK_PARTNR_NXT_PG_C 0x090f003c /* Link Partner Next Page (cont.) */
#define BRPHY0_CL45VEN_LNK_PARTNR_NXT_PG_D 0x090f003e /* Link Partner Next Page (cont.) */
#define BRPHY0_CL45VEN_LNK_PARTNR_NXT_PG_E 0x090f0040 /* Link Partner Next Page (cont.) */
#define BRPHY0_CL45VEN_LNK_PARTNR_NXT_PG_F 0x090f0042 /* Link Partner Next Page (cont.) */
#define BRPHY0_CL45VEN_EPON_CTRL_REG   0x090f0046 /* EPON mode control register */
#define BRPHY0_CL45VEN_EEE_TEST_CTRL_A 0x090f0060 /* EEE Test Control Register A eee_test_control_bus[15:0] */
#define BRPHY0_CL45VEN_EEE_TEST_CTRL_B 0x090f0062 /* EEE Test Control Register B eee_test_control_bus[31:16] */
#define BRPHY0_CL45VEN_EEE_TEST_CTRL_C 0x090f0064 /* EEE Test Control Register C eee_test_control_bus[47:32] */
#define BRPHY0_CL45VEN_EEE_SPARE_1     0x090f0076 /* EEE Spare Register 1 */
#define BRPHY0_CL45VEN_EEE_SPARE_2     0x090f0078 /* EEE Spare Register 2 */
#define BRPHY0_CL45VEN_EEE_CONTROL     0x090f007a /* EEE Control Register */
#define BRPHY0_CL45VEN_EEE_RES_STAT    0x090f007c /* EEE Resolution Status Register */
#define BRPHY0_CL45VEN_LPI_MODE_CNTR   0x090f007e /* LPI Mode Counter Register */
#define BRPHY0_CL45VEN_LOC_DEV_MSG_5_A 0x090f0080 /* Local Device Message 5 */
#define BRPHY0_CL45VEN_LOC_DEV_MSG_5_B 0x090f0082 /* Local Device Message 5 cont. */
#define BRPHY0_CL45VEN_LOC_DEV_MSG_5_C 0x090f0084 /* Local Device Message 5 cont. */
#define BRPHY0_CL45VEN_LOC_DEV_MSG_5_D 0x090f0086 /* Local Device Message 5 cont. */
#define BRPHY0_CL45VEN_LNK_PARTNR_MSG_5_A 0x090f0088 /* Link Partner Message 5 */
#define BRPHY0_CL45VEN_LNK_PARTNR_MSG_5_B 0x090f008a /* Link Partner Message 5 cont. */
#define BRPHY0_CL45VEN_LNK_PARTNR_MSG_5_C 0x090f008c /* Link Partner Message 5 cont. */
#define BRPHY0_CL45VEN_LNK_PARTNR_MSG_5_D 0x090f008e /* Link Partner Message 5 cont. */
#define BRPHY0_CL45VEN_LOC_DEV_MSG_6_A 0x090f0090 /* Local Device Message 6 */
#define BRPHY0_CL45VEN_LOC_DEV_MSG_6_B 0x090f0092 /* Local Device Message 6 cont. */
#define BRPHY0_CL45VEN_LOC_DEV_MSG_6_C 0x090f0094 /* Local Device Message 6 cont. */
#define BRPHY0_CL45VEN_LOC_DEV_MSG_6_D 0x090f0096 /* Local Device Message 6 cont. */
#define BRPHY0_CL45VEN_LNK_PARTNR_MSG_6_A 0x090f0098 /* Link Partner Message 6 */
#define BRPHY0_CL45VEN_LNK_PARTNR_MSG_6_B 0x090f009a /* Link Partner Message 6 cont. */
#define BRPHY0_CL45VEN_LNK_PARTNR_MSG_6_C 0x090f009c /* Link Partner Message 6 cont. */
#define BRPHY0_CL45VEN_LNK_PARTNR_MSG_6_D 0x090f009e /* Link Partner Message 6 cont. */


/****************************************************************************
 * bcm89530_top_bridge_brphys_brphys0_BRPHY0_GPHY_CORE
 ***************************************************************************/
#define BRPHY0_GPHY_CORE_BASE10        0x090f2000 /* PHY_Extended_ctl_Register */
#define BRPHY0_GPHY_CORE_BASE11        0x090f2002 /* PHY_Extended_Status_Register (copper side only) */
#define BRPHY0_GPHY_CORE_BASE12        0x090f2004 /* Receive_Error_Cntr_Register */
#define BRPHY0_GPHY_CORE_BASE13        0x090f2006 /* False_Carrier_Sense_Cntr_Register */
#define BRPHY0_GPHY_CORE_BASE14        0x090f2008 /* Local_Remote_Receiver_NOT_OK_Cntrs_Register */
#define BRPHY0_GPHY_CORE_EXP45         0x090f200a /* Pattern Generator Control Register */
#define BRPHY0_GPHY_CORE_EXP46         0x090f200b /* Pattern Generator Status Register */
#define BRPHY0_GPHY_CORE_BASE19        0x090f2012 /* Auxiliary Status Summary (copper side only) */
#define BRPHY0_GPHY_CORE_BASE1A        0x090f2014 /* Interrupt Status Register (copper side only) */
#define BRPHY0_GPHY_CORE_BASE1B        0x090f2016 /* Interrupt Mask Register */
#define BRPHY0_GPHY_CORE_BASE1D_SHD    0x090f2018 /* HCD Status Register */
#define BRPHY0_GPHY_CORE_BASE1D        0x090f201a /* Master/Slave Seed Register */
#define BRPHY0_GPHY_CORE_BASE1E        0x090f201c /* Test1_Register */
#define BRPHY0_GPHY_CORE_BASE1F        0x090f201e /* Test2_Register */
#define BRPHY0_GPHY_CORE_SHD1C_00      0x090f2020 /* Cabletron LED Register (Shadow Register Selector = "00h") */
#define BRPHY0_GPHY_CORE_SHD1C_01      0x090f2022 /* TVCO Selection Register (Shadow Register Selector = "01h") */
#define BRPHY0_GPHY_CORE_SHD1C_02      0x090f2024 /* reserved Control 1 Register (Shadow Register Selector = "02h") */
#define BRPHY0_GPHY_CORE_SHD1C_03      0x090f2026 /* Clock Alignment Control Regsiter (Shadow Register Selector = "03h") */
#define BRPHY0_GPHY_CORE_SHD1C_04      0x090f2028 /* reserved Control 2 Register (Shadow Register Selector = "04h") */
#define BRPHY0_GPHY_CORE_SHD1C_05      0x090f202a /* reserved Control 3 Register (Shadow Register Selector = "05h") */
#define BRPHY0_GPHY_CORE_SHD1C_06      0x090f202c /* Tdr Control 1 Register (Shadow Register Selector = "06h") */
#define BRPHY0_GPHY_CORE_SHD1C_07      0x090f202e /* Tdr Control 2 Register (Shadow Register Selector = "07h") */
#define BRPHY0_GPHY_CORE_SHD1C_08      0x090f2030 /* Led Status Register (Shadow Register Selector = "08h") */
#define BRPHY0_GPHY_CORE_SHD1C_09      0x090f2032 /* Led Control Register (Shadow Register Selector = "09h") */
#define BRPHY0_GPHY_CORE_SHD1C_0A      0x090f2034 /* Auto-Power Down Register (Shadow Register Selector = "0ah") */
#define BRPHY0_GPHY_CORE_SHD1C_0B      0x090f2036 /* reserved Control 4 Register (Shadow Register Selector = "0bh") */
#define BRPHY0_GPHY_CORE_SHD1C_0D      0x090f203a /* LED Selector 1 Register (Shadow Register Selector = "0dh") */
#define BRPHY0_GPHY_CORE_SHD1C_0E      0x090f203c /* LED Selector 2 Register (Shadow Register Selector = "0eh") */
#define BRPHY0_GPHY_CORE_SHD1C_0F      0x090f203e /* LED GPIO Control/Status Register (Shadow Register Selector = "0fh") */
#define BRPHY0_GPHY_CORE_SHD1C_10      0x090f2040 /* Cisco Enhanced Linkstatus Mode Control Register (Shadow Register Selector = "10h") */
#define BRPHY0_GPHY_CORE_SHD1C_1F      0x090f2042 /* Mode Control Register (Shadow Register Selector = "1fh") */
#define BRPHY0_GPHY_CORE_SHD18_000     0x090f2050 /* Auxiliary Control Register (Shadow Register Selector = "000") */
#define BRPHY0_GPHY_CORE_SHD18_001     0x090f2052 /* 10 Base-T Register (Shadow Register Selector = "001") */
#define BRPHY0_GPHY_CORE_SHD18_010     0x090f2054 /* Power/MII Control Register (Shadow Register Selector = "010") */
#define BRPHY0_GPHY_CORE_SHD18_011     0x090f2056 /* IP Phone Register  (Shadow Register Selector = "011") */
#define BRPHY0_GPHY_CORE_SHD18_100     0x090f2058 /* Misc Test Register  1 (Shadow Register Selector = "100") */
#define BRPHY0_GPHY_CORE_SHD18_101     0x090f205a /* Misc Test Register  2 (Shadow Register Selector = "101") */
#define BRPHY0_GPHY_CORE_SHD18_110     0x090f205c /* Manual IP Phone Seed Register (Shadow Register Selector = "110") */
#define BRPHY0_GPHY_CORE_SHD18_111     0x090f205e /* Miscellanous Control Register (Shadow Register Selector = "111") */
#define BRPHY0_GPHY_CORE_EXP00         0x090f2060 /* Transmit Packet Counter */
#define BRPHY0_GPHY_CORE_EXP01         0x090f2062 /* Expansion Interrupt Status */
#define BRPHY0_GPHY_CORE_EXP02         0x090f2064 /* Expansion Interrupt Mask */
#define BRPHY0_GPHY_CORE_EXP03         0x090f2066 /* Spare Registers */
#define BRPHY0_GPHY_CORE_EXP04         0x090f2068 /* Bicolor LED Selectors */
#define BRPHY0_GPHY_CORE_EXP05         0x090f206a /* Bicolor LED Flash Rate Controls */
#define BRPHY0_GPHY_CORE_EXP06         0x090f206c /* Bicolor LED Programmable Blink Controls */
#define BRPHY0_GPHY_CORE_EXP07         0x090f206e /* Far End Fault */
#define BRPHY0_GPHY_CORE_EXP08         0x090f2070 /* 10BT Controls */
#define BRPHY0_GPHY_CORE_EXP09         0x090f2072 /* AMRR Controls */
#define BRPHY0_GPHY_CORE_EXP0A         0x090f2074 /* DAC TEMPLATE Controls */
#define BRPHY0_GPHY_CORE_EXP0B         0x090f2076 /* External Status */
#define BRPHY0_GPHY_CORE_EXP0C         0x090f2078 /* Spare Registers */
#define BRPHY0_GPHY_CORE_EXP30         0x090f2080 /* Late Collision Counters Status Register */
#define BRPHY0_GPHY_CORE_EXP31         0x090f2082 /* Late Collision Counter [64:95] */
#define BRPHY0_GPHY_CORE_EXP32         0x090f2084 /* Late Collision Counter [96:127] */
#define BRPHY0_GPHY_CORE_EXP33         0x090f2086 /* Late Collision Counter [128:191] */
#define BRPHY0_GPHY_CORE_EXP34         0x090f2088 /* Late Collision Counter [192:319] */
#define BRPHY0_GPHY_CORE_EXP35         0x090f208a /* Late Collision Counter Threshold Register */
#define BRPHY0_GPHY_CORE_EXP36         0x090f208c /* Clock PPM Detection between Recovery and Transmit Clocks */
#define BRPHY0_GPHY_CORE_EXP37         0x090f208e /* Clock PPM Detection between GTX_CLK and Transmit Clocks */
#define BRPHY0_GPHY_CORE_EXP38         0x090f2090 /* IP PHONE Cable Length Status Register */
#define BRPHY0_GPHY_CORE_EXP42         0x090f20a2 /* Operating Mode Status */
#define BRPHY0_GPHY_CORE_EXP5F         0x090f20be /* PLL Frequency Offset Testmode Control */
#define BRPHY0_GPHY_CORE_EXP70         0x090f20e0 /* SOFT-RESET */
#define BRPHY0_GPHY_CORE_EXP71         0x090f20e2 /* Serial LED Control 1 */
#define BRPHY0_GPHY_CORE_EXP72         0x090f20e4 /* Serial LED Control 2 */
#define BRPHY0_GPHY_CORE_EXP73         0x090f20e6 /* LED Gating 2 (Used for dual-media applications) */
#define BRPHY0_GPHY_CORE_EXP74         0x090f20e8 /* LED Programmable Current Mode Control */
#define BRPHY0_GPHY_CORE_EXP75         0x090f20ea /* CED LED Error Mask */
#define BRPHY0_GPHY_CORE_EXP78         0x090f20f0 /* Misc Extended Control */
#define BRPHY0_GPHY_CORE_EXP7B         0x090f20f6 /* I2C Control */
#define BRPHY0_GPHY_CORE_EXP7C         0x090f20f8 /* I2C Status */
#define BRPHY0_GPHY_CORE_EXP7F         0x090f20fe /* External MACSec Interface Control */
#define BRPHY0_GPHY_CORE_ALIAS_18      0x090f2100 /* Alias to MII Reg 18 */
#define BRPHY0_GPHY_CORE_ALIAS_19      0x090f2102 /* Alias to MII Reg 19 */
#define BRPHY0_GPHY_CORE_ALIAS_1A      0x090f2104 /* Alias to MII Reg 1a */
#define BRPHY0_GPHY_CORE_ALIAS_1B      0x090f2106 /* Alias to MII Reg 1b */
#define BRPHY0_GPHY_CORE_ALIAS_1C      0x090f2108 /* Alias to MII Reg 1c */
#define BRPHY0_GPHY_CORE_ALIAS_1D      0x090f210a /* Alias to MII Reg 1d */
#define BRPHY0_GPHY_CORE_REG_MAP_CTL   0x090f210e /* MII Registers 10-1D mapping control */
#define BRPHY0_GPHY_CORE_EXP98         0x090f2130 /* First Slice of Quad-GPHY only): CAL-BIAS Status */
#define BRPHY0_GPHY_CORE_EXP9C         0x090f2138 /* SMII Control */
#define BRPHY0_GPHY_CORE_BT_LINK_FIX   0x090f214a /* 10BT LINK FIX Register */
#define BRPHY0_GPHY_CORE_SYNCE_PLUS_DBG 0x090f214c /* SyncE+ Debug */
#define BRPHY0_GPHY_CORE_SYNCE_PLUS    0x090f214e /* SyncE+ Status and Control */
#define BRPHY0_GPHY_CORE_EXPA8         0x090f2150 /* ADAPTIVE BIAS CONTROL */
#define BRPHY0_GPHY_CORE_EXPA9         0x090f2152 /* spare register */
#define BRPHY0_GPHY_CORE_EXPAA         0x090f2154 /* EEE Statistic timer 12hours lpi */
#define BRPHY0_GPHY_CORE_EXPAB         0x090f2156 /* EEE Statistic timer 12hours local */
#define BRPHY0_GPHY_CORE_EXPAC         0x090f2158 /* EEE Statistic loc lpi req 0_to_1 counter */
#define BRPHY0_GPHY_CORE_EXPAD         0x090f215a /* EEE Statistic rem lpi_req 0_to_1 counter */
#define BRPHY0_GPHY_CORE_EXPAE         0x090f215c /* spare register */
#define BRPHY0_GPHY_CORE_EXPAF         0x090f215e /* EEE Statistic counters ctrl/status */
#define BRPHY0_GPHY_CORE_EXPB0         0x090f2160 /* Bias Control 0 */
#define BRPHY0_GPHY_CORE_EXPB1         0x090f2162 /* Bias Control 1 */
#define BRPHY0_GPHY_CORE_EXPB2         0x090f2164 /* Bias Control 2 */
#define BRPHY0_GPHY_CORE_EXPE3         0x090f2166 /* TX PCS Delay 1000BT and 100TX (copper side) */
#define BRPHY0_GPHY_CORE_EXPE4         0x090f2168 /* TX PCS Delay 10BT (copper side) */
#define BRPHY0_GPHY_CORE_EXPE5         0x090f216a /* TX PCS Delay (fiber side) */
#define BRPHY0_GPHY_CORE_EXPE6         0x090f216c /* RX PCS Delay 1000BT and 100TX (copper side) */
#define BRPHY0_GPHY_CORE_EXPE7         0x090f216e /* RX PCS Delay 10BT (copper side) */
#define BRPHY0_GPHY_CORE_EXPE8         0x090f2170 /* RX PCS Delay (fiber side) */
#define BRPHY0_GPHY_CORE_EXPE9         0x090f2172 /* P1588 TX/RX Cycle Delay */
#define BRPHY0_GPHY_CORE_EXPE0         0x090f2174 /* TX PMA/PMD Delay (copper side) */
#define BRPHY0_GPHY_CORE_EXPE1         0x090f2176 /* TX PMA/PMD Delay (fiber side) */
#define BRPHY0_GPHY_CORE_EXPE2         0x090f2178 /* RX PMA/PMD Delay (copper side) */
#define BRPHY0_GPHY_CORE_EXPEA         0x090f217a /* TX/RX Adjustable Cycle Delay */
#define BRPHY0_GPHY_CORE_LED_PRA_MODE  0x090f2180 /* LED Proportional Rate Activity Control */
#define BRPHY0_GPHY_CORE_FIFO_CTL      0x090f2182 /* FIFO Control Register */
#define BRPHY0_GPHY_CORE_EXPD8         0x090f21b0 /* Halting agc/enc ctrl reg */
#define BRPHY0_GPHY_CORE_EXPF0         0x090f21e0 /* RGMII IBS Control */
#define BRPHY0_GPHY_CORE_EXPF5         0x090f21ea /* Time Sync */
#define BRPHY0_GPHY_CORE_EXPF6         0x090f21ec /* Analog Power Control Status */
#define BRPHY0_GPHY_CORE_EXPF7         0x090f21ee /* Auto-power Down Control Status */
#define BRPHY0_GPHY_CORE_EXPF8         0x090f21f0 /* Trim Settings from Fuse & to Bias Block */
#define BRPHY0_GPHY_CORE_EXPF9         0x090f21f2 /* reserved Register Bits */
#define BRPHY0_GPHY_CORE_EXPFA         0x090f21f4 /* Hidden Identifier */
#define BRPHY0_GPHY_CORE_EXPFB         0x090f21f6 /* TDR Override Values */
#define BRPHY0_GPHY_CORE_EXPFC         0x090f21f8 /*  */
#define BRPHY0_GPHY_CORE_EXPFD         0x090f21fa /* Clock gating control override value */
#define BRPHY0_GPHY_CORE_EXPFE         0x090f21fc /* Clock gating control override enable */
#define BRPHY0_GPHY_CORE_EXPFF         0x090f21fe /* Analog power control override */


/****************************************************************************
 * bcm89530_top_bridge_brphys_brphys0_BRPHY0_DSP_TAP
 ***************************************************************************/
#define BRPHY0_DSP_TAP_TAP0_C0         0x090f2200 /* AGC Control/Status Register A (x4) */
#define BRPHY0_DSP_TAP_TAP0_C1         0x090f2202 /* AGC Control/Status Register A (x4) */
#define BRPHY0_DSP_TAP_TAP0_C2         0x090f2204 /* AGC Control/Status Register A (x4) */
#define BRPHY0_DSP_TAP_TAP0_C3         0x090f2206 /* AGC Control/Status Register A (x4) */
#define BRPHY0_DSP_TAP_TAP1            0x090f2208 /* IPRF Control register (x1) */
#define BRPHY0_DSP_TAP_TAP2_C0         0x090f2210 /* MSE Status Register (x4) */
#define BRPHY0_DSP_TAP_TAP2_C1         0x090f2212 /* MSE Status Register (x4) */
#define BRPHY0_DSP_TAP_TAP2_C2         0x090f2214 /* MSE Status Register (x4) */
#define BRPHY0_DSP_TAP_TAP2_C3         0x090f2216 /* MSE Status Register (x4) */
#define BRPHY0_DSP_TAP_TAP3_C0         0x090f2218 /* Soft Decision (From DFE) Status Register (x4) */
#define BRPHY0_DSP_TAP_TAP3_C1         0x090f221a /* Soft Decision (From DFE) Status Register (x4) */
#define BRPHY0_DSP_TAP_TAP3_C2         0x090f221c /* Soft Decision (From DFE) Status Register (x4) */
#define BRPHY0_DSP_TAP_TAP3_C3         0x090f221e /* Soft Decision (From DFE) Status Register (x4) */
#define BRPHY0_DSP_TAP_TAP4_C0         0x090f2220 /* Phase Status Register and Coarse AGC High Gear (x4) */
#define BRPHY0_DSP_TAP_TAP4_C1         0x090f2222 /* Phase Status Register and Coarse AGC High Gear (x4) */
#define BRPHY0_DSP_TAP_TAP4_C2         0x090f2224 /* Phase Status Register and Coarse AGC High Gear (x4) */
#define BRPHY0_DSP_TAP_TAP4_C3         0x090f2226 /* Phase Status Register and Coarse AGC High Gear (x4) */
#define BRPHY0_DSP_TAP_TAP5_C0         0x090f2228 /* Wire Map/Skew Control/Status and Echo/NEXT Control and Transmitter Control and ADC Control Register and Slicer Zero (x4) */
#define BRPHY0_DSP_TAP_TAP5_C1         0x090f222a /* Wire Map/Skew Control/Status and Echo/NEXT Control and Transmitter Control and ADC Control Register and Slicer Zero (x4) */
#define BRPHY0_DSP_TAP_TAP5_C2         0x090f222c /* Wire Map/Skew Control/Status and Echo/NEXT Control and Transmitter Control and ADC Control Register and Slicer Zero (x4) */
#define BRPHY0_DSP_TAP_TAP5_C3         0x090f222e /* Wire Map/Skew Control/Status and Echo/NEXT Control and Transmitter Control and ADC Control Register and Slicer Zero (x4) */
#define BRPHY0_DSP_TAP_TAP6            0x090f2230 /* CFC Deadman Disable */
#define BRPHY0_DSP_TAP_TAP7_C0         0x090f2238 /* BIST TEST 0 */
#define BRPHY0_DSP_TAP_TAP7_C1         0x090f223a /* BIST TEST 1 */
#define BRPHY0_DSP_TAP_TAP7_C2         0x090f223c /* BIST TEST 2 */
#define BRPHY0_DSP_TAP_TAP8_C0         0x090f2240 /* ABIST TEST 0 */
#define BRPHY0_DSP_TAP_TAP8_C1         0x090f2242 /* ABIST TEST 1 */
#define BRPHY0_DSP_TAP_TAP8_C2         0x090f2244 /* ABIST TEST 2 */
#define BRPHY0_DSP_TAP_TAP8_C3         0x090f2246 /* BR HPF Control */
#define BRPHY0_DSP_TAP_TAP9            0x090f2248 /* Frequency Control/Status Register LSBs (x1) */
#define BRPHY0_DSP_TAP_TAP10           0x090f224a /* PLL Bandwidth Control/Status and Path Metric Reset Register (x1) */
#define BRPHY0_DSP_TAP_TAP11           0x090f224c /* PLL RCLK and TCLK Offset Freeze Register (x1) */
#define BRPHY0_DSP_TAP_TAP12_C0        0x090f2250 /* PLL ACLK Offset/Freeze Register (x4) */
#define BRPHY0_DSP_TAP_TAP12_C1        0x090f2252 /* PLL ACLK Offset/Freeze Register (x4) */
#define BRPHY0_DSP_TAP_TAP12_C2        0x090f2254 /* PLL ACLK Offset/Freeze Register (x4) */
#define BRPHY0_DSP_TAP_TAP12_C3        0x090f2256 /* PLL ACLK Offset/Freeze Register (x4) */
#define BRPHY0_DSP_TAP_TAP13           0x090f2258 /* HPF Bandwidth Control and Disable ADC LSBs (x1) */
#define BRPHY0_DSP_TAP_TAP14           0x090f225a /* MSE Threshold Register  #1 (x1) */
#define BRPHY0_DSP_TAP_TAP15           0x090f225c /* MSE Threshold Register #2 (x1) */
#define BRPHY0_DSP_TAP_TAP16_C0        0x090f2260 /* Logic Analyzer trigger delay (x1) */
#define BRPHY0_DSP_TAP_TAP16_C1        0x090f2262 /* BIST CRC Monitor (x4) */
#define BRPHY0_DSP_TAP_TAP16_C2        0x090f2264 /* BIST CRC Monitor (x4) */
#define BRPHY0_DSP_TAP_TAP16_C3        0x090f2266 /* BIST CRC Monitor (x4) */
#define BRPHY0_DSP_TAP_TAP17_C0        0x090f2268 /* Testmode testvalue (aliased with logic analyzer state selects) */
#define BRPHY0_DSP_TAP_TAP17_C1        0x090f226a /* Testmode and logic analyzer controls #1 */
#define BRPHY0_DSP_TAP_TAP17_C2        0x090f226c /* Logic analyzer controls #2 */
#define BRPHY0_DSP_TAP_TAP17_C3        0x090f226e /* Testmode and logic analyzer controls #3 */
#define BRPHY0_DSP_TAP_TAP18_C0        0x090f2270 /* Peak Noise detector (x4) */
#define BRPHY0_DSP_TAP_TAP18_C1        0x090f2272 /* Peak Noise detector (x4) */
#define BRPHY0_DSP_TAP_TAP18_C2        0x090f2274 /* Peak Noise detector (x4) */
#define BRPHY0_DSP_TAP_TAP18_C3        0x090f2276 /* Peak Noise detector (x4) */
#define BRPHY0_DSP_TAP_TAP20           0x090f2278 /* Echo Minimum Length and LMS/FIR delay adjustments (x1) */
#define BRPHY0_DSP_TAP_TAP21           0x090f227a /* Phy Control Monitors #1 (x1) */
#define BRPHY0_DSP_TAP_TAP22           0x090f227c /* Phy Control Monitors #2 (x1) */
#define BRPHY0_DSP_TAP_TAP23           0x090f227e /* Phy Control Monitors #3 (x1) */
#define BRPHY0_DSP_TAP_TAP24           0x090f2280 /* Phy Control Output Overrides #1 (x1) */
#define BRPHY0_DSP_TAP_TAP25           0x090f2282 /* Phy Control Output Overrides #2 (x1) */
#define BRPHY0_DSP_TAP_TAP26           0x090f2284 /* Phy Control Input Overrides #1 (x1) */
#define BRPHY0_DSP_TAP_TAP27           0x090f2286 /* Phy Control Input Overrides #2 (x1) */
#define BRPHY0_DSP_TAP_TAP28           0x090f2288 /* Phy Control Output Overrides #3 (x1) */
#define BRPHY0_DSP_TAP_TAP29           0x090f228a /* Phy Control Force State/Timers/Alternate Behaviour Register #1 (x1) */
#define BRPHY0_DSP_TAP_TAP30           0x090f228c /* Phy Control Force State/Timers/Alternate Behaviour Register #2 (x1) */
#define BRPHY0_DSP_TAP_TAP31_C0        0x090f2290 /* Channel Swap Override */
#define BRPHY0_DSP_TAP_TAP32_C0        0x090f2298 /* Transmit Testmode Sync Generation (x1) */
#define BRPHY0_DSP_TAP_FDFE_OV_RD      0x090f229a /* FDFE Override/Read Control Register */
#define BRPHY0_DSP_TAP_FDFE_COEFF      0x090f229c /* FDFE Coefficient Read Back Register */
#define BRPHY0_DSP_TAP_FDFE_BETA_THRESHOLD 0x090f229e /* FDFE Beta Threshold Control */
#define BRPHY0_DSP_TAP_TAP33_C0        0x090f22a0 /* eee dsp test */
#define BRPHY0_DSP_TAP_TAP33_C1        0x090f22a2 /* eee sigdet */
#define BRPHY0_DSP_TAP_TAP33_C2        0x090f22a4 /* eee_lpi_timers */
#define BRPHY0_DSP_TAP_TAP33_C3        0x090f22a6 /* spare register */
#define BRPHY0_DSP_TAP_TAP34_C0        0x090f22a8 /* eee frequency control */
#define BRPHY0_DSP_TAP_TAP34_C1        0x090f22aa /* eee Gigabit Mode BW control */
#define BRPHY0_DSP_TAP_TAP34_C2        0x090f22ac /* eee 100TX Mode BW control */
#define BRPHY0_DSP_TAP_TAP34_C3        0x090f22ae /* phasectl TPO monitor */
#define BRPHY0_DSP_TAP_TAP35_C0        0x090f22b0 /* eee 100Base-tx timer control 1 */
#define BRPHY0_DSP_TAP_TAP35_C1        0x090f22b2 /* eee 100Base-tx timer control 2 */
#define BRPHY0_DSP_TAP_TAP35_C2        0x090f22b4 /* eee 100Base-tx timer misc control */
#define BRPHY0_DSP_TAP_TAP35_C3        0x090f22b6 /* pcs_lpi_test */
#define BRPHY0_DSP_TAP_FILTER_CTL_CH0  0x090f22b8 /* Filter Freeze/Disable per channel Control */
#define BRPHY0_DSP_TAP_FILTER_CTL_CH1  0x090f22ba /* Filter Freeze/Disable per channel Control */
#define BRPHY0_DSP_TAP_FILTER_CTL_CH2  0x090f22bc /* Filter Freeze/Disable per channel Control */
#define BRPHY0_DSP_TAP_FILTER_CTL_CH3  0x090f22be /* Filter Freeze/Disable per channel Control */
#define BRPHY0_DSP_TAP_FILTER_CTL_FFEX_CH0 0x090f22c0 /* FFEX Freeze/Disable per channel Control */
#define BRPHY0_DSP_TAP_FILTER_CTL_FFEX_CH1 0x090f22c2 /* FFEX Freeze/Disable per channel Control */
#define BRPHY0_DSP_TAP_FILTER_CTL_FFEX_CH2 0x090f22c4 /* FFEX Freeze/Disable per channel Control */
#define BRPHY0_DSP_TAP_FILTER_CTL_FFEX_CH3 0x090f22c6 /* FFEX Freeze/Disable per channel Control */
#define BRPHY0_DSP_TAP_EMI_DATAPATH_CTL 0x090f22c8 /* EMI Datapath Control */
#define BRPHY0_DSP_TAP_EMI_DATAPATH_CTL2 0x090f22ca /* EMI Datapath Control2 */
#define BRPHY0_DSP_TAP_FFEX_CTL        0x090f22cc /* FFE X-tap Control */
#define BRPHY0_DSP_TAP_PHYC_BREAKPOINT_CTL0 0x090f22ce /* Phycontrol Breakpoint Control 0 */
#define BRPHY0_DSP_TAP_PHYC_BREAKPOINT_CTL1 0x090f22d0 /* Phycontrol Breakpoint Control 1 */
#define BRPHY0_DSP_TAP_FILTER_ADDR     0x090f2360 /* DSP Coefficient Address Register */
#define BRPHY0_DSP_TAP_FILTER_CTL      0x090f2362 /* DSP Control Register */
#define BRPHY0_DSP_TAP_FILTER_DATA     0x090f2364 /* DSP Coefficient Read/Write Port */


/****************************************************************************
 * bcm89530_top_bridge_brphys_brphys0_BRPHY0_PLL_CTRL
 ***************************************************************************/
#define BRPHY0_PLL_CTRL_PLLCTRL_0      0x090f2390 /* Analog pll control 0 */
#define BRPHY0_PLL_CTRL_PLLCTRL_1      0x090f2392 /* Analog pll control 1 */
#define BRPHY0_PLL_CTRL_PLLCTRL_2      0x090f2394 /* Analog pll control 2 */
#define BRPHY0_PLL_CTRL_PLLCTRL_3      0x090f2396 /* Analog pll control 3 */
#define BRPHY0_PLL_CTRL_PLLCTRL_4      0x090f2398 /* Analog pll control 4 */
#define BRPHY0_PLL_CTRL_PLLCTRL_5      0x090f239a /* Analog pll control 5 */
#define BRPHY0_PLL_CTRL_PLLCTRL_6      0x090f239c /* Analog pll control 6 */
#define BRPHY0_PLL_CTRL_PLL_STATUS_0   0x090f23a0 /* Analog PLL Status 0 */
#define BRPHY0_PLL_CTRL_PLL_STATUS_1   0x090f23a2 /* Analog PLL Status 1 */
#define BRPHY0_PLL_CTRL_AFE_SIGDET_STATUS 0x090f23a4 /* AFE Signal detect */
#define BRPHY0_PLL_CTRL_PLLCTRL_7      0x090f23a6 /* Analog pll control 7 */
#define BRPHY0_PLL_CTRL_PLLCTRL_8      0x090f23a8 /* Analog pll control 8 */


/****************************************************************************
 * bcm89530_top_bridge_brphys_brphys0_BRPHY0_AFE_CTRL
 ***************************************************************************/
#define BRPHY0_AFE_CTRL_RXCONFIG_0     0x090f23c0 /* RXCONFIG 15:0 */
#define BRPHY0_AFE_CTRL_RXCONFIG_1     0x090f23c2 /* RXCONFIG 31:16 */
#define BRPHY0_AFE_CTRL_RXCONFIG_2     0x090f23c4 /* RXCONFIG 47:32 */
#define BRPHY0_AFE_CTRL_RXCONFIG_3     0x090f23c6 /* RXCONFIG 63:48 */
#define BRPHY0_AFE_CTRL_RXCONFIG_4     0x090f23c8 /* RXCONFIG 79:64 */
#define BRPHY0_AFE_CTRL_RXCONFIG5_LP   0x090f23ca /* RXCONFIG 86:80 and LP tuning */
#define BRPHY0_AFE_CTRL_TX_CONFIG_0    0x090f23cc /* TXCONFIG 15:0 */
#define BRPHY0_AFE_CTRL_TX_CONFIG_1    0x090f23ce /* TXCONFIG 31:16 */
#define BRPHY0_AFE_CTRL_VDAC_ICTRL_0   0x090f23d0 /* VDAC CURRENT Control 15:0 */
#define BRPHY0_AFE_CTRL_VDAC_ICTRL_1   0x090f23d2 /* VDAC CURRENT Control 31:16 */
#define BRPHY0_AFE_CTRL_VDAC_ICTRL_2   0x090f23d4 /* VDAC CURRENT Control 51:36 */
#define BRPHY0_AFE_CTRL_VDAC_OTHERS_0  0x090f23d6 /* VDAC CURRENT 35:32 and others */
#define BRPHY0_AFE_CTRL_HPF_TRIM_OTHERS 0x090f23d8 /* HPF trim and reserved bits */
#define BRPHY0_AFE_CTRL_TX_EXTRA_CONFIG_0 0x090f23da /* TXCONFIG 15:0 */
#define BRPHY0_AFE_CTRL_TX_EXTRA_CONFIG_1 0x090f23dc /* TXCONFIG 15:0 */
#define BRPHY0_AFE_CTRL_TX_EXTRA_CONFIG_2 0x090f23de /* TXCONFIG 15:0 */
#define BRPHY0_AFE_CTRL_TEMPSEN_OTHERS 0x090f23e0 /* TEMPSEN_OTHERS 15:0 */
#define BRPHY0_AFE_CTRL_FUTURE_RSV     0x090f23e2 /* FUTURE_RSV 15:0 */


/****************************************************************************
 * bcm89530_top_bridge_brphys_brphys0_BRPHY0_ECD_CTRL
 ***************************************************************************/
#define BRPHY0_ECD_CTRL_EXPC0          0x090f2540 /* ECD Control and Status */
#define BRPHY0_ECD_CTRL_EXPC1          0x090f2542 /* ECD Fault Type */
#define BRPHY0_ECD_CTRL_EXPC2          0x090f2544 /* ECD Pair A Length Results */
#define BRPHY0_ECD_CTRL_EXPC3          0x090f2546 /* ECD Pair B Length Results */
#define BRPHY0_ECD_CTRL_EXPC4          0x090f2548 /* ECD Pair C Length Results */
#define BRPHY0_ECD_CTRL_EXPC5          0x090f254a /* ECD Pair D Length Results */
#define BRPHY0_ECD_CTRL_EXPC6          0x090f254c /* ECD XTALK Map */
#define BRPHY0_ECD_CTRL_EXPC7          0x090f254e /* ECD EXTRA RESERVED REGISTER */
#define BRPHY0_ECD_CTRL_EXPC8          0x090f2550 /* ECD EXTRA RESERVED REGISTER */
#define BRPHY0_ECD_CTRL_EXPC9          0x090f2552 /* ECD EXTRA RESERVED REGISTER */
#define BRPHY0_ECD_CTRL_EXPCA          0x090f2554 /* ECD EXTRA RESERVED REGISTER */
#define BRPHY0_ECD_CTRL_EXPCB          0x090f2556 /* ECD EXTRA RESERVED REGISTER */
#define BRPHY0_ECD_CTRL_EXPCC          0x090f2558 /* ECD EXTRA RESERVED REGISTER */
#define BRPHY0_ECD_CTRL_EXPCD          0x090f255a /* ECD EXTRA RESERVED REGISTER */
#define BRPHY0_ECD_CTRL_EXPCE          0x090f255c /* ECD EXTRA RESERVED REGISTER */
#define BRPHY0_ECD_CTRL_EXPCF          0x090f255e /* ECD EXTRA RESERVED REGISTER */
#define BRPHY0_ECD_CTRL_EXPE0          0x090f2560 /* ECD EXTRA RESERVED REGISTER */
#define BRPHY0_ECD_CTRL_EXPE1          0x090f2562 /* ECD EXTRA RESERVED REGISTER */
#define BRPHY0_ECD_CTRL_EXPE2          0x090f2564 /* ECD EXTRA RESERVED REGISTER */
#define BRPHY0_ECD_CTRL_EXPE3          0x090f2566 /* ECD EXTRA RESERVED REGISTER */
#define BRPHY0_ECD_CTRL_EXPE4          0x090f2568 /* ECD EXTRA RESERVED REGISTER */
#define BRPHY0_ECD_CTRL_EXPE5          0x090f256a /* ECD EXTRA RESERVED REGISTER */
#define BRPHY0_ECD_CTRL_EXPE6          0x090f256c /* ECD EXTRA RESERVED REGISTER */
#define BRPHY0_ECD_CTRL_EXPE7          0x090f256e /* ECD EXTRA RESERVED REGISTER */
#define BRPHY0_ECD_CTRL_EXPE8          0x090f2570 /* ECD EXTRA RESERVED REGISTER */
#define BRPHY0_ECD_CTRL_EXPE9          0x090f2572 /* ECD EXTRA RESERVED REGISTER */
#define BRPHY0_ECD_CTRL_EXPEA          0x090f2574 /* ECD EXTRA RESERVED REGISTER */
#define BRPHY0_ECD_CTRL_EXPEB          0x090f2576 /* ECD EXTRA RESERVED REGISTER */
#define BRPHY0_ECD_CTRL_EXPEC          0x090f2578 /* ECD EXTRA RESERVED REGISTER */
#define BRPHY0_ECD_CTRL_EXPED          0x090f257a /* ECD EXTRA RESERVED REGISTER */
#define BRPHY0_ECD_CTRL_EXPEE          0x090f257c /* ECD EXTRA RESERVED REGISTER */
#define BRPHY0_ECD_CTRL_EXPEF          0x090f257e /* ECD EXTRA RESERVED REGISTER */


/****************************************************************************
 * bcm89530_top_bridge_brphys_brphys0_BRPHY0_BR_CTRL
 ***************************************************************************/
#define BRPHY0_BR_CTRL_EXP90           0x090f2600 /* BroadReach LRE Misc Control */
#define BRPHY0_BR_CTRL_EXP91           0x090f2602 /* BroadReach LRE Misc Control */
#define BRPHY0_BR_CTRL_EXP92           0x090f2604 /* BroadReach LRE Misc Control */
#define BRPHY0_BR_CTRL_EXP93           0x090f2606 /* BroadReach LDS  Control */
#define BRPHY0_BR_CTRL_EXP94           0x090f2608 /* BroadReach LDS RX Control */
#define BRPHY0_BR_CTRL_EXP95           0x090f260a /* BroadReach LDS RX Control */
#define BRPHY0_BR_CTRL_EXP96           0x090f260c /* BroadReach LDS Scan, ARB and TX Status */
#define BRPHY0_BR_CTRL_EXP97           0x090f260e /* BroadReach LDS Scan, ARB and TX Status */
#define BRPHY0_BR_CTRL_EXP99           0x090f2612 /* BroadReach LDS Timer Control */
#define BRPHY0_BR_CTRL_EXP9A           0x090f2614 /* LDS Status */
#define BRPHY0_BR_CTRL_EXP9B           0x090f2616 /* BroadR-Reach PLL Control */
#define BRPHY0_BR_CTRL_EXP9D           0x090f261a /* EoC Internal Control 1 */
#define BRPHY0_BR_CTRL_EXP9E           0x090f261c /* LDS Length Threshold 0 */
#define BRPHY0_BR_CTRL_EXP9F           0x090f261e /* LDS Length Threshold 1 */
#define BRPHY0_BR_CTRL_EXPA0           0x090f2620 /* HLDS register, LDS extend advertisement register */
#define BRPHY0_BR_CTRL_EXPA1           0x090f2622 /* HLDS register, LDS link partner extend ability register */
#define BRPHY0_BR_CTRL_EXPA2           0x090f2624 /* HLDS Register */
#define BRPHY0_BR_CTRL_BR_MISC_CONTROL_STATUS 0x090f2626 /* Broadreach Misc Status */
#define BRPHY0_BR_CTRL_BR250_CTL       0x090f263c /* BR250 Control */


/****************************************************************************
 * bcm89530_top_bridge_brphys_brphys0_BRPHY_TOP_MISC_0
 ***************************************************************************/
#define BRPHY_TOP_MISC_0_SPARE_REG_0   0x090f3000 /* spare registers 16 bits */
#define BRPHY_TOP_MISC_0_SPARE_REG_1   0x090f3002 /* spare registers 16 bits */
#define BRPHY_TOP_MISC_0_AFE_OVR       0x090f3004 /* AFE Override Control Register */
#define BRPHY_TOP_MISC_0_TEST_REG0     0x090f3006 /* Test control register 0 for iddq etc. mode */
#define BRPHY_TOP_MISC_0_TEST_REG1     0x090f3008 /* Test control register 1 for test bus selection */
#define BRPHY_TOP_MISC_0_TOP_GLOBAL_RESET_REG 0x090f300a /* Global Top level soft reset register */
#define BRPHY_TOP_MISC_0_INTERRUPT_STATUS_REG 0x090f300c /* Top interrupt status register */


/****************************************************************************
 * bcm89530_top_bridge_brphys_brphys0_BRPHY_WOL
 ***************************************************************************/
#define BRPHY_WOL_TB_P0_CTRL           0x090f3200 /* Wake On LAN Main Control Register */
#define BRPHY_WOL_TB_P0_ITPID          0x090f3202 /* Inner Tag Protocol ID Register */
#define BRPHY_WOL_TB_P0_OTPID          0x090f3204 /* Outer Tag Protocol ID Register */
#define BRPHY_WOL_TB_P0_OTPID2         0x090f3206 /* Outer Tag Protocol ID 2 Register */
#define BRPHY_WOL_TB_P0_PKT1_15_00     0x090f3208 /* Magic Packet1 Data bit [15:0] Register */
#define BRPHY_WOL_TB_P0_PKT1_31_16     0x090f320a /* Magic Packet1 Data bit [31:16] Register */
#define BRPHY_WOL_TB_P0_PKT1_47_32     0x090f320c /* Magic Packet1 Data bit [47:32] Register */
#define BRPHY_WOL_TB_P0_PKT2_15_00     0x090f320e /* Magic Packet2/SecureKey Data bit [15:0] Register */
#define BRPHY_WOL_TB_P0_PKT2_31_16     0x090f3210 /* Magic Packet2/SecureKey Data bit [31:16] Register */
#define BRPHY_WOL_TB_P0_PKT2_47_32     0x090f3212 /* Magic Packet2/SecureKey Data bit [47:32] Register */
#define BRPHY_WOL_TB_P0_PKT2_63_48     0x090f3214 /* SecureKey Data bit [63:48] Register(SecureKey 8 byte mode) */
#define BRPHY_WOL_TB_P0_MSKCTR_15_00   0x090f3216 /* Wake On LAN Packet Mask Control Register [15:0] */
#define BRPHY_WOL_TB_P0_MSKCTR_31_16   0x090f3218 /* Wake On LAN Packet Mask Control Register [31:16] */
#define BRPHY_WOL_TB_P0_MSKCTR_47_32   0x090f321a /* Wake On LAN Packet Mask Control Register [47:32] */
#define BRPHY_WOL_TB_P0_SECKEY_15_00   0x090f321c /* Wake On LAN Secure Key Data Storage Register [15:0] */
#define BRPHY_WOL_TB_P0_SECKEY_31_16   0x090f321e /* Wake On LAN Secure Key Data Storage Register [31:16] */
#define BRPHY_WOL_TB_P0_SECKEY_47_32   0x090f3220 /* Wake On LAN Secure Key Data Storage Register [47:32] */
#define BRPHY_WOL_TB_P0_SECKEY_63_48   0x090f3222 /* Wake On LAN Secure Key Data Storage Register [63:48] */
#define BRPHY_WOL_TB_P0_PKT_CNT        0x090f3224 /* Magic Packet 1/2 Shared Counter Register */
#define BRPHY_WOL_TB_P1_CTRL           0x090f3226 /* Wake On LAN Main Control Register */
#define BRPHY_WOL_TB_P1_ITPID          0x090f3228 /* Inner Tag Protocol ID Register */
#define BRPHY_WOL_TB_P1_OTPID          0x090f322a /* Outer Tag Protocol ID Register */
#define BRPHY_WOL_TB_P1_OTPID2         0x090f322c /* Outer Tag Protocol ID 2 Register */
#define BRPHY_WOL_TB_P1_PKT1_15_00     0x090f322e /* Magic Packet1 Data bit [15:0] Register */
#define BRPHY_WOL_TB_P1_PKT1_31_16     0x090f3230 /* Magic Packet1 Data bit [31:16] Register */
#define BRPHY_WOL_TB_P1_PKT1_47_32     0x090f3232 /* Magic Packet1 Data bit [47:32] Register */
#define BRPHY_WOL_TB_P1_PKT2_15_00     0x090f3234 /* Magic Packet2/SecureKey Data bit [15:0] Register */
#define BRPHY_WOL_TB_P1_PKT2_31_16     0x090f3236 /* Magic Packet2/SecureKey Data bit [31:16] Register */
#define BRPHY_WOL_TB_P1_PKT2_47_32     0x090f3238 /* Magic Packet2/SecureKey Data bit [47:32] Register */
#define BRPHY_WOL_TB_P1_PKT2_63_48     0x090f323a /* SecureKey Data bit [63:48] Register(SecureKey 8 byte mode) */
#define BRPHY_WOL_TB_P1_MSKCTR_15_00   0x090f323c /* Wake On LAN Packet Mask Control Register [15:0] */
#define BRPHY_WOL_TB_P1_MSKCTR_31_16   0x090f323e /* Wake On LAN Packet Mask Control Register [31:16] */
#define BRPHY_WOL_TB_P1_MSKCTR_47_32   0x090f3240 /* Wake On LAN Packet Mask Control Register [47:32] */
#define BRPHY_WOL_TB_P1_SECKEY_15_00   0x090f3242 /* Wake On LAN Secure Key Data Storage Register [15:0] */
#define BRPHY_WOL_TB_P1_SECKEY_31_16   0x090f3244 /* Wake On LAN Secure Key Data Storage Register [31:16] */
#define BRPHY_WOL_TB_P1_SECKEY_47_32   0x090f3246 /* Wake On LAN Secure Key Data Storage Register [47:32] */
#define BRPHY_WOL_TB_P1_SECKEY_63_48   0x090f3248 /* Wake On LAN Secure Key Data Storage Register [63:48] */
#define BRPHY_WOL_TB_P1_PKT_CNT        0x090f324a /* Magic Packet 1/2 Shared Counter Register */
#define BRPHY_WOL_TB_P2_CTRL           0x090f324c /* Wake On LAN Main Control Register */
#define BRPHY_WOL_TB_P2_ITPID          0x090f324e /* Inner Tag Protocol ID Register */
#define BRPHY_WOL_TB_P2_OTPID          0x090f3250 /* Outer Tag Protocol ID Register */
#define BRPHY_WOL_TB_P2_OTPID2         0x090f3252 /* Outer Tag Protocol ID 2 Register */
#define BRPHY_WOL_TB_P2_PKT1_15_00     0x090f3254 /* Magic Packet1 Data bit [15:0] Register */
#define BRPHY_WOL_TB_P2_PKT1_31_16     0x090f3256 /* Magic Packet1 Data bit [31:16] Register */
#define BRPHY_WOL_TB_P2_PKT1_47_32     0x090f3258 /* Magic Packet1 Data bit [47:32] Register */
#define BRPHY_WOL_TB_P2_PKT2_15_00     0x090f325a /* Magic Packet2/SecureKey Data bit [15:0] Register */
#define BRPHY_WOL_TB_P2_PKT2_31_16     0x090f325c /* Magic Packet2/SecureKey Data bit [31:16] Register */
#define BRPHY_WOL_TB_P2_PKT2_47_32     0x090f325e /* Magic Packet2/SecureKey Data bit [47:32] Register */
#define BRPHY_WOL_TB_P2_PKT2_63_48     0x090f3260 /* SecureKey Data bit [63:48] Register(SecureKey 8 byte mode) */
#define BRPHY_WOL_TB_P2_MSKCTR_15_00   0x090f3262 /* Wake On LAN Packet Mask Control Register [15:0] */
#define BRPHY_WOL_TB_P2_MSKCTR_31_16   0x090f3264 /* Wake On LAN Packet Mask Control Register [31:16] */
#define BRPHY_WOL_TB_P2_MSKCTR_47_32   0x090f3266 /* Wake On LAN Packet Mask Control Register [47:32] */
#define BRPHY_WOL_TB_P2_SECKEY_15_00   0x090f3268 /* Wake On LAN Secure Key Data Storage Register [15:0] */
#define BRPHY_WOL_TB_P2_SECKEY_31_16   0x090f326a /* Wake On LAN Secure Key Data Storage Register [31:16] */
#define BRPHY_WOL_TB_P2_SECKEY_47_32   0x090f326c /* Wake On LAN Secure Key Data Storage Register [47:32] */
#define BRPHY_WOL_TB_P2_SECKEY_63_48   0x090f326e /* Wake On LAN Secure Key Data Storage Register [63:48] */
#define BRPHY_WOL_TB_P2_PKT_CNT        0x090f3270 /* Magic Packet 1/2 Shared Counter Register */
#define BRPHY_WOL_TB_P3_CTRL           0x090f3272 /* Wake On LAN Main Control Register */
#define BRPHY_WOL_TB_P3_ITPID          0x090f3274 /* Inner Tag Protocol ID Register */
#define BRPHY_WOL_TB_P3_OTPID          0x090f3276 /* Outer Tag Protocol ID Register */
#define BRPHY_WOL_TB_P3_OTPID2         0x090f3278 /* Outer Tag Protocol ID 2 Register */
#define BRPHY_WOL_TB_P3_PKT1_15_00     0x090f327a /* Magic Packet1 Data bit [15:0] Register */
#define BRPHY_WOL_TB_P3_PKT1_31_16     0x090f327c /* Magic Packet1 Data bit [31:16] Register */
#define BRPHY_WOL_TB_P3_PKT1_47_32     0x090f327e /* Magic Packet1 Data bit [47:32] Register */
#define BRPHY_WOL_TB_P3_PKT2_15_00     0x090f3280 /* Magic Packet2/SecureKey Data bit [15:0] Register */
#define BRPHY_WOL_TB_P3_PKT2_31_16     0x090f3282 /* Magic Packet2/SecureKey Data bit [31:16] Register */
#define BRPHY_WOL_TB_P3_PKT2_47_32     0x090f3284 /* Magic Packet2/SecureKey Data bit [47:32] Register */
#define BRPHY_WOL_TB_P3_PKT2_63_48     0x090f3286 /* SecureKey Data bit [63:48] Register(SecureKey 8 byte mode) */
#define BRPHY_WOL_TB_P3_MSKCTR_15_00   0x090f3288 /* Wake On LAN Packet Mask Control Register [15:0] */
#define BRPHY_WOL_TB_P3_MSKCTR_31_16   0x090f328a /* Wake On LAN Packet Mask Control Register [31:16] */
#define BRPHY_WOL_TB_P3_MSKCTR_47_32   0x090f328c /* Wake On LAN Packet Mask Control Register [47:32] */
#define BRPHY_WOL_TB_P3_SECKEY_15_00   0x090f328e /* Wake On LAN Secure Key Data Storage Register [15:0] */
#define BRPHY_WOL_TB_P3_SECKEY_31_16   0x090f3290 /* Wake On LAN Secure Key Data Storage Register [31:16] */
#define BRPHY_WOL_TB_P3_SECKEY_47_32   0x090f3292 /* Wake On LAN Secure Key Data Storage Register [47:32] */
#define BRPHY_WOL_TB_P3_SECKEY_63_48   0x090f3294 /* Wake On LAN Secure Key Data Storage Register [63:48] */
#define BRPHY_WOL_TB_P3_PKT_CNT        0x090f3296 /* Magic Packet 1/2 Shared Counter Register */
#define BRPHY_WOL_TB_P4_CTRL           0x090f3298 /* Wake On LAN Main Control Register */
#define BRPHY_WOL_TB_P4_ITPID          0x090f329a /* Inner Tag Protocol ID Register */
#define BRPHY_WOL_TB_P4_OTPID          0x090f329c /* Outer Tag Protocol ID Register */
#define BRPHY_WOL_TB_P4_OTPID2         0x090f329e /* Outer Tag Protocol ID 2 Register */
#define BRPHY_WOL_TB_P4_PKT1_15_00     0x090f32a0 /* Magic Packet1 Data bit [15:0] Register */
#define BRPHY_WOL_TB_P4_PKT1_31_16     0x090f32a2 /* Magic Packet1 Data bit [31:16] Register */
#define BRPHY_WOL_TB_P4_PKT1_47_32     0x090f32a4 /* Magic Packet1 Data bit [47:32] Register */
#define BRPHY_WOL_TB_P4_PKT2_15_00     0x090f32a6 /* Magic Packet2/SecureKey Data bit [15:0] Register */
#define BRPHY_WOL_TB_P4_PKT2_31_16     0x090f32a8 /* Magic Packet2/SecureKey Data bit [31:16] Register */
#define BRPHY_WOL_TB_P4_PKT2_47_32     0x090f32aa /* Magic Packet2/SecureKey Data bit [47:32] Register */
#define BRPHY_WOL_TB_P4_PKT2_63_48     0x090f32ac /* SecureKey Data bit [63:48] Register(SecureKey 8 byte mode) */
#define BRPHY_WOL_TB_P4_MSKCTR_15_00   0x090f32ae /* Wake On LAN Packet Mask Control Register [15:0] */
#define BRPHY_WOL_TB_P4_MSKCTR_31_16   0x090f32b0 /* Wake On LAN Packet Mask Control Register [31:16] */
#define BRPHY_WOL_TB_P4_MSKCTR_47_32   0x090f32b2 /* Wake On LAN Packet Mask Control Register [47:32] */
#define BRPHY_WOL_TB_P4_SECKEY_15_00   0x090f32b4 /* Wake On LAN Secure Key Data Storage Register [15:0] */
#define BRPHY_WOL_TB_P4_SECKEY_31_16   0x090f32b6 /* Wake On LAN Secure Key Data Storage Register [31:16] */
#define BRPHY_WOL_TB_P4_SECKEY_47_32   0x090f32b8 /* Wake On LAN Secure Key Data Storage Register [47:32] */
#define BRPHY_WOL_TB_P4_SECKEY_63_48   0x090f32ba /* Wake On LAN Secure Key Data Storage Register [63:48] */
#define BRPHY_WOL_TB_P4_PKT_CNT        0x090f32bc /* Magic Packet 1/2 Shared Counter Register */
#define BRPHY_WOL_TB_INTR_MSK0         0x090f32be /* Magic Packet Interrupt Mask Register 0 */
#define BRPHY_WOL_TB_INTR_STS0         0x090f32c0 /* Magic Packet Status Interrupt Register 0 */
#define BRPHY_WOL_TB_WOL_TPO_SEL       0x090f32c2 /* wol tpo port sel */
#define BRPHY_WOL_TB_WOL_SPARE         0x090f33fc /* Spare Register */


/****************************************************************************
 * bcm89530_top_bridge_brphys_brphys0_BRPHY_TOP_1588
 ***************************************************************************/
#define BRPHY_TOP_1588_SLICE_ENABLE    0x090f3420 /* P1588 Slice Enable Control Register */
#define BRPHY_TOP_1588_TX_MODE_PORT_0  0x090f3422 /* P1588 Port 0 TX Event Message Mode1 and Mode2 Selection Register */
#define BRPHY_TOP_1588_TX_MODE_PORT_1  0x090f3424 /* P1588 Port 1 TX Event Message Mode1 and Mode2 Selection Register */
#define BRPHY_TOP_1588_TX_MODE_PORT_2  0x090f3426 /* P1588 Port 2 TX Event Message Mode1 and Mode2 Selection Register */
#define BRPHY_TOP_1588_TX_MODE_PORT_3  0x090f3428 /* P1588 Port 3 TX Event Message Mode1 and Mode2 Selection Register */
#define BRPHY_TOP_1588_TX_MODE_PORT_4  0x090f342a /* P1588 Port 4 TX Event Message Mode1 and Mode2 Selection Register */
#define BRPHY_TOP_1588_TX_MODE_PORT_5  0x090f342c /* P1588 Port 5 TX Event Message Mode1 and Mode2 Selection Register */
#define BRPHY_TOP_1588_TX_MODE_PORT_6  0x090f342e /* P1588 Port 6 TX Event Message Mode1 and Mode2 Selection Register */
#define BRPHY_TOP_1588_TX_MODE_PORT_7  0x090f3430 /* P1588 Port 7 TX Event Message Mode1 and Mode2 Selection Register */
#define BRPHY_TOP_1588_RX_MODE_PORT_0  0x090f3432 /* P1588 Port 0 RX Event Message Mode1 and Mode2 Selection Register */
#define BRPHY_TOP_1588_RX_MODE_PORT_1  0x090f3434 /* P1588 Port 1 RX Event Message Mode1 and Mode2 Selection Register */
#define BRPHY_TOP_1588_RX_MODE_PORT_2  0x090f3436 /* P1588 Port 2 RX Event Message Mode1 and Mode2 Selection Register */
#define BRPHY_TOP_1588_RX_MODE_PORT_3  0x090f3438 /* P1588 Port 3 RX Event Message Mode1 and Mode2 Selection Register */
#define BRPHY_TOP_1588_RX_MODE_PORT_4  0x090f343a /* P1588 Port 4 RX Event Message Mode1 and Mode2 Selection Register */
#define BRPHY_TOP_1588_RX_MODE_PORT_5  0x090f343c /* P1588 Port 5 RX Event Message Mode1 and Mode2 Selection Register */
#define BRPHY_TOP_1588_RX_MODE_PORT_6  0x090f343e /* P1588 Port 6 RX Event Message Mode1 and Mode2 Selection Register */
#define BRPHY_TOP_1588_RX_MODE_PORT_7  0x090f3440 /* P1588 Port 7 RX Event Message Mode1 and Mode2 Selection Register */
#define BRPHY_TOP_1588_TX_TS_CAP       0x090f3442 /* P1588 TX SOP Timestamp Capture Enable */
#define BRPHY_TOP_1588_RX_TS_CAP       0x090f3444 /* P1588 RX SOP Timestamp Capture Enable Register */
#define BRPHY_TOP_1588_RX_TX_OPTION    0x090f3446 /* P1588 RX and TX Option Register */
#define BRPHY_TOP_1588_RX_PORT_0_LINK_DELAY_LSB 0x090f3448 /* P1588 Port 0 RX PORT Link delay LSB Register */
#define BRPHY_TOP_1588_RX_PORT_0_LINK_DELAY_MSB 0x090f344a /* P1588 Port 0 RX PORT Link delay MSB Register */
#define BRPHY_TOP_1588_RX_PORT_1_LINK_DELAY_LSB 0x090f344c /* P1588 Port 1 RX PORT Link delay LSB Register */
#define BRPHY_TOP_1588_RX_PORT_1_LINK_DELAY_MSB 0x090f344e /* P1588 Port 1 RX PORT Link delay MSB Register */
#define BRPHY_TOP_1588_RX_PORT_2_LINK_DELAY_LSB 0x090f3450 /* P1588 Port 2 RX PORT Link delay LSB Register */
#define BRPHY_TOP_1588_RX_PORT_2_LINK_DELAY_MSB 0x090f3452 /* P1588 Port 2 RX PORT Link delay MSB Register */
#define BRPHY_TOP_1588_RX_PORT_3_LINK_DELAY_LSB 0x090f3454 /* P1588 Port 3 RX PORT Link delay LSB Register */
#define BRPHY_TOP_1588_RX_PORT_3_LINK_DELAY_MSB 0x090f3456 /* P1588 Port 3 RX PORT Link delay MSB Register */
#define BRPHY_TOP_1588_RX_PORT_4_LINK_DELAY_LSB 0x090f3458 /* P1588 Port 4 RX PORT Link delay LSB Register */
#define BRPHY_TOP_1588_RX_PORT_4_LINK_DELAY_MSB 0x090f345a /* P1588 Port 4 RX PORT Link delay MSB Register */
#define BRPHY_TOP_1588_RX_PORT_5_LINK_DELAY_LSB 0x090f345c /* P1588 Port 5 RX PORT Link delay LSB Register */
#define BRPHY_TOP_1588_RX_PORT_5_LINK_DELAY_MSB 0x090f345e /* P1588 Port 5 RX PORT Link delay MSB Register */
#define BRPHY_TOP_1588_RX_PORT_6_LINK_DELAY_LSB 0x090f3460 /* P1588 Port 6 RX PORT Link delay LSB Register */
#define BRPHY_TOP_1588_RX_PORT_6_LINK_DELAY_MSB 0x090f3462 /* P1588 Port 6 RX PORT Link delay MSB Register */
#define BRPHY_TOP_1588_RX_PORT_7_LINK_DELAY_LSB 0x090f3464 /* P1588 Port 7 RX PORT Link delay LSB Register */
#define BRPHY_TOP_1588_RX_PORT_7_LINK_DELAY_MSB 0x090f3466 /* P1588 Port 7 RX PORT Link delay MSB Register */
#define BRPHY_TOP_1588_TX_PORT_0_TS_OFFSET_LSB 0x090f3468 /* P1588 Port 0 TX Timestamp Offset LSB Register */
#define BRPHY_TOP_1588_TX_PORT_0_TS_OFFSET_MSB 0x090f346a /* P1588 Port 0 TX Timestamp Offset MSB Register */
#define BRPHY_TOP_1588_TX_PORT_1_TS_OFFSET_LSB 0x090f346c /* P1588 Port 1 TX Timestamp Offset LSB Register */
#define BRPHY_TOP_1588_TX_PORT_1_TS_OFFSET_MSB 0x090f346e /* P1588 Port 1 TX Timestamp Offset MSB Register */
#define BRPHY_TOP_1588_TX_PORT_2_TS_OFFSET_LSB 0x090f3470 /* P1588 Port 2 TX Timestamp Offset LSB Register */
#define BRPHY_TOP_1588_TX_PORT_2_TS_OFFSET_MSB 0x090f3472 /* P1588 Port 2 TX Timestamp Offset MSB Register */
#define BRPHY_TOP_1588_TX_PORT_3_TS_OFFSET_LSB 0x090f3474 /* P1588 Port 3 TX Timestamp Offset LSB Register */
#define BRPHY_TOP_1588_TX_PORT_3_TS_OFFSET_MSB 0x090f3476 /* P1588 Port 3 TX Timestamp Offset MSB Register */
#define BRPHY_TOP_1588_TX_PORT_4_TS_OFFSET_LSB 0x090f3478 /* P1588 Port 4 TX Timestamp Offset LSB Register */
#define BRPHY_TOP_1588_TX_PORT_4_TS_OFFSET_MSB 0x090f347a /* P1588 Port 4 TX Timestamp Offset MSB Register */
#define BRPHY_TOP_1588_TX_PORT_5_TS_OFFSET_LSB 0x090f347c /* P1588 Port 5 TX Timestamp Offset LSB Register */
#define BRPHY_TOP_1588_TX_PORT_5_TS_OFFSET_MSB 0x090f347e /* P1588 Port 5 TX Timestamp Offset MSB Register */
#define BRPHY_TOP_1588_TX_PORT_6_TS_OFFSET_LSB 0x090f3480 /* P1588 Port 6 TX Timestamp Offset LSB Register */
#define BRPHY_TOP_1588_TX_PORT_6_TS_OFFSET_MSB 0x090f3482 /* P1588 Port 6 TX Timestamp Offset MSB Register */
#define BRPHY_TOP_1588_TX_PORT_7_TS_OFFSET_LSB 0x090f3484 /* P1588 Port 7 TX Timestamp Offset LSB Register */
#define BRPHY_TOP_1588_TX_PORT_7_TS_OFFSET_MSB 0x090f3486 /* P1588 Port 7 TX Timestamp Offset MSB Register */
#define BRPHY_TOP_1588_RX_PORT_0_TS_OFFSET_LSB 0x090f3488 /* P1588 Port 0 RX Timestamp Offset LSB Register */
#define BRPHY_TOP_1588_RX_PORT_0_TS_OFFSET_MSB 0x090f348a /* P1588 Port 0 RX Timestamp Offset MSB Register */
#define BRPHY_TOP_1588_RX_PORT_1_TS_OFFSET_LSB 0x090f348c /* P1588 Port 1 RX Timestamp Offset LSB Register */
#define BRPHY_TOP_1588_RX_PORT_1_TS_OFFSET_MSB 0x090f348e /* P1588 Port 1 RX Timestamp Offset MSB Register */
#define BRPHY_TOP_1588_RX_PORT_2_TS_OFFSET_LSB 0x090f3490 /* P1588 Port 2 RX Timestamp Offset LSB Register */
#define BRPHY_TOP_1588_RX_PORT_2_TS_OFFSET_MSB 0x090f3492 /* P1588 Port 2 RX Timestamp Offset MSB Register */
#define BRPHY_TOP_1588_RX_PORT_3_TS_OFFSET_LSB 0x090f3494 /* P1588 Port 3 RX Timestamp Offset LSB Register */
#define BRPHY_TOP_1588_RX_PORT_3_TS_OFFSET_MSB 0x090f3496 /* P1588 Port 3 RX Timestamp Offset MSB Register */
#define BRPHY_TOP_1588_RX_PORT_4_TS_OFFSET_LSB 0x090f3498 /* P1588 Port 4 RX Timestamp Offset LSB Register */
#define BRPHY_TOP_1588_RX_PORT_4_TS_OFFSET_MSB 0x090f349a /* P1588 Port 4 RX Timestamp Offset MSB Register */
#define BRPHY_TOP_1588_RX_PORT_5_TS_OFFSET_LSB 0x090f349c /* P1588 Port 5 RX Timestamp Offset LSB Register */
#define BRPHY_TOP_1588_RX_PORT_5_TS_OFFSET_MSB 0x090f349e /* P1588 Port 5 RX Timestamp Offset MSB Register */
#define BRPHY_TOP_1588_RX_PORT_6_TS_OFFSET_LSB 0x090f34a0 /* P1588 Port 6 RX Timestamp Offset LSB Register */
#define BRPHY_TOP_1588_RX_PORT_6_TS_OFFSET_MSB 0x090f34a2 /* P1588 Port 6 RX Timestamp Offset MSB Register */
#define BRPHY_TOP_1588_RX_PORT_7_TS_OFFSET_LSB 0x090f34a4 /* P1588 Port 7 RX Timestamp Offset LSB Register */
#define BRPHY_TOP_1588_RX_PORT_7_TS_OFFSET_MSB 0x090f34a6 /* P1588 Port 7 RX Timestamp Offset MSB Register */
#define BRPHY_TOP_1588_TIME_CODE_0     0x090f34a8 /* P1588 Original Time Code 0 Register */
#define BRPHY_TOP_1588_TIME_CODE_1     0x090f34aa /* P1588 Original Time Code 1 Register */
#define BRPHY_TOP_1588_TIME_CODE_2     0x090f34ac /* P1588 Original Time Code 2 Register */
#define BRPHY_TOP_1588_TIME_CODE_3     0x090f34ae /* P1588 Original Time Code 3 Register */
#define BRPHY_TOP_1588_TIME_CODE_4     0x090f34b0 /* P1588 Original Time Code 4 Register */
#define BRPHY_TOP_1588_DPLL_DB_LSB     0x090f34b2 /* P1588 DPLL Debug LSB Register */
#define BRPHY_TOP_1588_DPLL_DB_MSB     0x090f34b4 /* P1588 DPLL Debug MSB Register */
#define BRPHY_TOP_1588_DPLL_DB_SEL     0x090f34b6 /* P1588 DPLL Debug Select Register */
#define BRPHY_TOP_1588_SHD_CTL         0x090f34b8 /* P1588 Shadow Register Control */
#define BRPHY_TOP_1588_SHD_LD          0x090f34ba /* P1588 Shadow Register Load */
#define BRPHY_TOP_1588_INT_MASK        0x090f34bc /* P1588 Interrupt Mask Register */
#define BRPHY_TOP_1588_INT_STAT        0x090f34be /* P1588 Interrupt Status Register */
#define BRPHY_TOP_1588_TX_CTL          0x090f34c0 /* P1588 Transmit Control Register */
#define BRPHY_TOP_1588_RX_CTL          0x090f34c2 /* P1588 Receive Control Register */
#define BRPHY_TOP_1588_RX_TX_CTL       0x090f34c4 /* P1588 Receive/Transmit Control Register */
#define BRPHY_TOP_1588_VLAN_ITPID      0x090f34c6 /* P1588 VLAN 1tags ITPID Register */
#define BRPHY_TOP_1588_VLAN_OTPID      0x090f34c8 /* P1588 VLAN 2tags OTPID Register */
#define BRPHY_TOP_1588_OTHER_OTPID     0x090f34ca /* P1588 VLAN 2Tags Other OTPID Register */
#define BRPHY_TOP_1588_NSE_DPLL_1      0x090f34cc /* P1588 NSE DPLL Register 1 */
#define BRPHY_TOP_1588_NSE_DPLL_2_0    0x090f34ce /* P1588 NSE DPLL Register 2(0) */
#define BRPHY_TOP_1588_NSE_DPLL_2_1    0x090f34d0 /* P1588 NSE DPLL Register 2(1) */
#define BRPHY_TOP_1588_NSE_DPLL_2_2    0x090f34d2 /* P1588 NSE DPLL Register 2(2) */
#define BRPHY_TOP_1588_NSE_DPLL_3_LSB  0x090f34d4 /* P1588 NSE DPLL Register 3 LSB */
#define BRPHY_TOP_1588_NSE_DPLL_3_MSB  0x090f34d6 /* P1588 NSE DPLL Register 3 MSB */
#define BRPHY_TOP_1588_NSE_DPLL_4      0x090f34d8 /* P1588 NSE DPLL Register 4 */
#define BRPHY_TOP_1588_NSE_DPLL_5      0x090f34da /* P1588 NSE DPLL Register 5 */
#define BRPHY_TOP_1588_NSE_DPLL_6      0x090f34dc /* P1588 NSE DPLL Register 6 */
#define BRPHY_TOP_1588_NSE_DPLL_7_0    0x090f34de /* P1588 NSE DPLL Register 7(0) */
#define BRPHY_TOP_1588_NSE_DPLL_7_1    0x090f34e0 /* P1588 NSE DPLL Register 7(1) */
#define BRPHY_TOP_1588_NSE_DPLL_7_2    0x090f34e2 /* P1588 NSE DPLL Register 7(2) */
#define BRPHY_TOP_1588_NSE_DPLL_7_3    0x090f34e4 /* P1588 NSE DPLL Register 7(3) */
#define BRPHY_TOP_1588_NSE_NCO_1_LSB   0x090f34e6 /* P1588 NSE DPLL NCO Register 1 LSB */
#define BRPHY_TOP_1588_NSE_NCO_1_MSB   0x090f34e8 /* P1588 NSE DPLL NCO Register 1 MSB */
#define BRPHY_TOP_1588_NSE_NCO_2_0     0x090f34ea /* P1588 NSE DPLL NCO Register 2(0) */
#define BRPHY_TOP_1588_NSE_NCO_2_1     0x090f34ec /* P1588 NSE DPLL NCO Register 2(1) */
#define BRPHY_TOP_1588_NSE_NCO_2_2     0x090f34ee /* P1588 NSE DPLL NCO Register 2(2) */
#define BRPHY_TOP_1588_NSE_NCO_3_0     0x090f34f0 /* P1588 NSE DPLL NCO Register 3(0) */
#define BRPHY_TOP_1588_NSE_NCO_3_1     0x090f34f2 /* P1588 NSE DPLL NCO Register 3(1) */
#define BRPHY_TOP_1588_NSE_NCO_3_2     0x090f34f4 /* P1588 NSE DPLL NCO Register 3(2) */
#define BRPHY_TOP_1588_NSE_NCO_4       0x090f34f6 /* P1588 NSE DPLL NCO Register 4 */
#define BRPHY_TOP_1588_NSE_NCO_5_0     0x090f34f8 /* P1588 NSE DPLL NCO Register 5(0) */
#define BRPHY_TOP_1588_NSE_NCO_5_1     0x090f34fa /* P1588 NSE DPLL NCO Register 5(1) */
#define BRPHY_TOP_1588_NSE_NCO_5_2     0x090f34fc /* P1588 NSE DPLL NCO Register 5(2) */
#define BRPHY_TOP_1588_NSE_NCO_6       0x090f34fe /* P1588 NSE DPLL NCO Register 6 */
#define BRPHY_TOP_1588_NSE_NCO_7_0     0x090f3500 /* P1588 NSE DPLL NCO Register 7(0) */
#define BRPHY_TOP_1588_NSE_NCO_7_1     0x090f3502 /* P1588 NSE DPLL NCO Register 7(1) */
#define BRPHY_TOP_1588_TX_COUNTER      0x090f3504 /* P1588 TX Counter */
#define BRPHY_TOP_1588_RX_COUNTER      0x090f3506 /* P1588 RX Counter */
#define BRPHY_TOP_1588_RX_TX_1588_COUNTER 0x090f3508 /* P1588 RX TX 1588 Counter */
#define BRPHY_TOP_1588_TS_READ_START_END 0x090f350a /* P1588 Timestamp READ START/END Register */
#define BRPHY_TOP_1588_HEARTBEAT_0     0x090f350c /* P1588 Heartbeat Register(0) */
#define BRPHY_TOP_1588_HEARTBEAT_1     0x090f350e /* P1588 Heartbeat Register(1) */
#define BRPHY_TOP_1588_HEARTBEAT_2     0x090f3510 /* P1588 Heartbeat Register(2) */
#define BRPHY_TOP_1588_TIME_STAMP_0    0x090f3512 /* P1588 Time Stamp Register(0) */
#define BRPHY_TOP_1588_TIME_STAMP_1    0x090f3514 /* P1588 Time Stamp Register(1) */
#define BRPHY_TOP_1588_TIME_STAMP_2    0x090f3516 /* P1588 Time Stamp Register(2) */
#define BRPHY_TOP_1588_TIME_STAMP_INFO_1 0x090f3518 /* P1588 Time Stamp Register Info (1) */
#define BRPHY_TOP_1588_TIME_STAMP_INFO_2 0x090f351a /* P1588 Time Stamp Register Info (2) */
#define BRPHY_TOP_1588_CNTR_DBG        0x090f351c /* P1588 Control/Debug Register */
#define BRPHY_TOP_1588_MPLS_SPARE1     0x090f351e /* P1588 CPU TX and RX Port Enable Registers */
#define BRPHY_TOP_1588_MPLS_SPARE2     0x090f3520 /* P1588 DA1 Registers */
#define BRPHY_TOP_1588_MPLS_SPARE3     0x090f3522 /* P1588 DA2 Registers */
#define BRPHY_TOP_1588_MPLS_SPARE4     0x090f3524 /* P1588 DA3 Registers */
#define BRPHY_TOP_1588_MPLS_SPARE5     0x090f3526 /* P1588 MPLS Special Label LSB Registers */
#define BRPHY_TOP_1588_MPLS_SPARE6     0x090f3528 /* P1588 MPLS SPECIAL lABEL 4 MSB Registers */
#define BRPHY_TOP_1588_MPLS_TX_CNTL    0x090f352a /* P1588 MPLS TX Enable */
#define BRPHY_TOP_1588_MPLS_RX_CNTL    0x090f352c /* P1588 MPLS RX Enable */
#define BRPHY_TOP_1588_MPLS_LABEL1_LSB_MASK 0x090f352e /* P1588 MPLS label1 mask lsb bit */
#define BRPHY_TOP_1588_MPLS_LABEL1_MSB_MASK 0x090f3530 /* P1588 MPLS label1 mask msb bit (*Inband P0 Control) */
#define BRPHY_TOP_1588_MPLS_LABEL1_LSB_VALUE 0x090f3532 /* P1588 MPLS label1 value lsb bit */
#define BRPHY_TOP_1588_MPLS_LABEL1_MSB_VALUE 0x090f3534 /* P1588 MPLS label1 value msb bit (*HSR P0 Offset) */
#define BRPHY_TOP_1588_MPLS_LABEL2_LSB_MASK 0x090f3536 /* P1588 MPLS label2 mask lsb bit */
#define BRPHY_TOP_1588_MPLS_LABEL2_MSB_MASK 0x090f3538 /* P1588 MPLS label2 mask msb bit (*Inband P1 Control) */
#define BRPHY_TOP_1588_MPLS_LABEL2_LSB_VALUE 0x090f353a /* P1588 MPLS label2 value lsb bit */
#define BRPHY_TOP_1588_MPLS_LABEL2_MSB_VALUE 0x090f353c /* P1588 MPLS label2 value msb bit (*HSR P1 Offset) */
#define BRPHY_TOP_1588_MPLS_LABEL3_LSB_MASK 0x090f353e /* P1588 MPLS label3 mask lsb bit */
#define BRPHY_TOP_1588_MPLS_LABEL3_MSB_MASK 0x090f3540 /* P1588 MPLS label3 mask msb bit (*Inband P2 Control) */
#define BRPHY_TOP_1588_MPLS_LABEL3_LSB_VALUE 0x090f3542 /* P1588 MPLS label3 value lsb bit */
#define BRPHY_TOP_1588_MPLS_LABEL3_MSB_VALUE 0x090f3544 /* P1588 MPLS label3 value msb bit (*HSR P2 Offset) */
#define BRPHY_TOP_1588_MPLS_LABEL4_LSB_MASK 0x090f3546 /* P1588 MPLS label4 mask lsb bit */
#define BRPHY_TOP_1588_MPLS_LABEL4_MSB_MASK 0x090f3548 /* P1588 MPLS label4 mask msb bit (*Inband P3 Control) */
#define BRPHY_TOP_1588_MPLS_LABEL4_LSB_VALUE 0x090f354a /* P1588 MPLS label4 value lsb bit */
#define BRPHY_TOP_1588_MPLS_LABEL4_MSB_VALUE 0x090f354c /* P1588 MPLS label4 value msb bit (*HSR P3 Offset) */
#define BRPHY_TOP_1588_MPLS_LABEL5_LSB_MASK 0x090f354e /* P1588 MPLS label5 mask lsb bit */
#define BRPHY_TOP_1588_MPLS_LABEL5_MSB_MASK 0x090f3550 /* P1588 MPLS label5 mask msb bit (*Inband P4 Control) */
#define BRPHY_TOP_1588_MPLS_LABEL5_LSB_VALUE 0x090f3552 /* P1588 MPLS label5 value lsb bit */
#define BRPHY_TOP_1588_MPLS_LABEL5_MSB_VALUE 0x090f3554 /* P1588 MPLS label5 value msb bit (*HSR P4 Offset) */
#define BRPHY_TOP_1588_MPLS_LABEL6_LSB_MASK 0x090f3556 /* P1588 MPLS label6 mask lsb bit */
#define BRPHY_TOP_1588_MPLS_LABEL6_MSB_MASK 0x090f3558 /* P1588 MPLS label6 mask msb bit (*Inband P5 Control) */
#define BRPHY_TOP_1588_MPLS_LABEL6_LSB_VALUE 0x090f355a /* P1588 MPLS label6 value lsb bit */
#define BRPHY_TOP_1588_MPLS_LABEL6_MSB_VALUE 0x090f355c /* P1588 MPLS label6 value msb bit (*HSR P5 Offset) */
#define BRPHY_TOP_1588_MPLS_LABEL7_LSB_MASK 0x090f355e /* P1588 MPLS label7 mask lsb bit */
#define BRPHY_TOP_1588_MPLS_LABEL7_MSB_MASK 0x090f3560 /* P1588 MPLS label7 mask msb bit (*Inband P6 Control) */
#define BRPHY_TOP_1588_MPLS_LABEL7_LSB_VALUE 0x090f3562 /* P1588 MPLS label7 value lsb bit */
#define BRPHY_TOP_1588_MPLS_LABEL7_MSB_VALUE 0x090f3564 /* P1588 MPLS label7 value msb bit (*HSR P6 Offset) */
#define BRPHY_TOP_1588_MPLS_LABEL8_LSB_MASK 0x090f3566 /* P1588 MPLS label8 mask lsb bit */
#define BRPHY_TOP_1588_MPLS_LABEL8_MSB_MASK 0x090f3568 /* P1588 MPLS label8 mask msb bit (*Inband P7 Control) */
#define BRPHY_TOP_1588_MPLS_LABEL8_LSB_VALUE 0x090f356a /* P1588 MPLS label8 value lsb bit */
#define BRPHY_TOP_1588_MPLS_LABEL8_MSB_VALUE 0x090f356c /* P1588 MPLS label8 value msb bit (*HSR P7 Offset) */
#define BRPHY_TOP_1588_MPLS_LABEL9_LSB_MASK 0x090f356e /* P1588 MPLS label9 mask lsb bit */
#define BRPHY_TOP_1588_MPLS_LABEL9_MSB_MASK 0x090f3570 /* P1588 MPLS label9 mask msb bit */
#define BRPHY_TOP_1588_MPLS_LABEL9_LSB_VALUE 0x090f3572 /* P1588 MPLS label9 value lsb bit */
#define BRPHY_TOP_1588_MPLS_LABEL9_MSB_VALUE 0x090f3574 /* P1588 MPLS label9 value msb bit */
#define BRPHY_TOP_1588_MPLS_LABEL10_LSB_MASK 0x090f3576 /* P1588 MPLS label10 mask lsb bit (*HSR Enable) */
#define BRPHY_TOP_1588_MPLS_LABEL10_MSB_MASK 0x090f3578 /* P1588 MPLS label10 mask msb bit (*HSR Ethertype) */
#define BRPHY_TOP_1588_MPLS_LABEL10_LSB_VALUE 0x090f357a /* P1588 MPLS label10 value lsb bit (*HSR SNAP/LLC Control) */
#define BRPHY_TOP_1588_MPLS_LABEL10_MSB_VALUE 0x090f357c /* P1588 MPLS label10 value msb bit */
#define BRPHY_TOP_1588_RX_TX_1588_COUNTER1 0x090f357e /* P1588 RX TX CPU 1588 Counter */
#define BRPHY_TOP_1588_RX_CF_SPEC      0x090f3580 /* P1588 RX CF + Insertion */
#define BRPHY_TOP_1588_TX_CF_SPEC      0x090f3582 /* P1588 TX CS Update */
#define BRPHY_TOP_1588_MPLS_PACKET_ENABLE 0x090f3584 /* P1588 MPLS_PACKET ENABLE */
#define BRPHY_TOP_1588_TIMECODE_SEL    0x090f3586 /* P1588 TIMECODE SEL */
#define BRPHY_TOP_1588_TIME_STAMP_3    0x090f3588 /* P1588 Time Stamp Register(3) */
#define BRPHY_TOP_1588_TIME_STAMP      0x090f358a /* P1588 Control/Debug Register */
#define BRPHY_TOP_1588_DM_TX_CNTL      0x090f358c /* P1588 Delay Measurment Control Register */
#define BRPHY_TOP_1588_DM_RX_CNTL      0x090f358e /* P1588 Delay Measurment Control Register */
#define BRPHY_TOP_1588_DM_ETHTYPE1     0x090f3590 /* P1588 Delay Measurment Ethtype1 Register */
#define BRPHY_TOP_1588_DM_ETHTYPE2     0x090f3592 /* P1588 Delay Measurment Ethtype2 Register */
#define BRPHY_TOP_1588_DM_ETHTYPE3     0x090f3594 /* P1588 Delay Measurment Ethtype3 Register */
#define BRPHY_TOP_1588_DM_ETHTYPE4     0x090f3596 /* P1588 Delay Measurment Ethtype4 Register */
#define BRPHY_TOP_1588_DM_ETHTYPE5     0x090f3598 /* P1588 Delay Measurment Ethtype5 Register */
#define BRPHY_TOP_1588_DM_ETHTYPE6     0x090f359a /* P1588 Delay Measurment Ethtype6 Register */
#define BRPHY_TOP_1588_DM_ETHTYPE7     0x090f359c /* P1588 Delay Measurment Ethtype7 Register */
#define BRPHY_TOP_1588_DM_ETHTYPE8     0x090f359e /* P1588 Delay Measurment Ethtype8 Register */
#define BRPHY_TOP_1588_DM_ETHTYPE9     0x090f35a0 /* P1588 Delay Measurment Ethtype9 Register */
#define BRPHY_TOP_1588_DM_ETHTYPE10    0x090f35a2 /* P1588 Delay Measurment Ethtype10 Register */
#define BRPHY_TOP_1588_DM_ETHTYPE11    0x090f35a4 /* P1588 Delay Measurment Ethtype11 Register */
#define BRPHY_TOP_1588_DM_ETHTYPE12    0x090f35a6 /* P1588 Delay Measurment Ethtype12 Register */
#define BRPHY_TOP_1588_DM_ETHTYPE13    0x090f35a8 /* P1588 Delay Measurment Ethtype13 Register */
#define BRPHY_TOP_1588_DM_IETF_OFFSET  0x090f35aa /* P1588 Delay Measurment IETF Offfset Register */
#define BRPHY_TOP_1588_NTP_TIME_STAMP_0 0x090f35ac /* P1588 NTP Counter Time Stamp0  Register */
#define BRPHY_TOP_1588_NTP_TIME_STAMP_1 0x090f35ae /* P1588 NTP Counter Time Stamp1  Register */
#define BRPHY_TOP_1588_NTP_TIME_STAMP_2 0x090f35b0 /* P1588 NTP Counter Time Stamp2  Register */
#define BRPHY_TOP_1588_NTP_TIME_STAMP_3 0x090f35b2 /* P1588 NTP Counter Time Stamp3  Register */
#define BRPHY_TOP_1588_NTP_NCO_FREQ_0  0x090f35b4 /* P1588 NTP NCO Frequency0  Register */
#define BRPHY_TOP_1588_NTP_NCO_FREQ_1  0x090f35b6 /* P1588 NTP NCO Frequency1  Register */
#define BRPHY_TOP_1588_NTP_DOWN_CNTER_0 0x090f35b8 /* P1588 NTP Down Counter 0  Register */
#define BRPHY_TOP_1588_NTP_DOWN_CNTER_1 0x090f35ba /* P1588 NTP Down Counter 1  Register */
#define BRPHY_TOP_1588_NTP_ERR_LSB     0x090f35bc /* P1588 NTP ERR LSB  Register */
#define BRPHY_TOP_1588_NTP_ERR_MSB     0x090f35be /* P1588 NTP ERR MSB  Register */
#define BRPHY_TOP_1588_DM_MAC_L1_0     0x090f35c0 /* P1588 DM MAC Address Local1 0 Register */
#define BRPHY_TOP_1588_DM_MAC_L1_1     0x090f35c2 /* P1588 DM MAC Address Local1 0 Register */
#define BRPHY_TOP_1588_DM_MAC_L1_2     0x090f35c4 /* P1588 DM MAC Address Local1 0 Register */
#define BRPHY_TOP_1588_DM_MAC_L2_0     0x090f35c6 /* P1588 DM MAC Address Local2 0 Register */
#define BRPHY_TOP_1588_DM_MAC_L2_1     0x090f35c8 /* P1588 DM MAC Address Local2 0 Register */
#define BRPHY_TOP_1588_DM_MAC_L2_2     0x090f35ca /* P1588 DM MAC Address Local2 0 Register */
#define BRPHY_TOP_1588_DM_MAC_L3_0     0x090f35cc /* P1588 DM MAC Address Local3 0 Register */
#define BRPHY_TOP_1588_DM_MAC_L3_1     0x090f35ce /* P1588 DM MAC Address Local3 0 Register */
#define BRPHY_TOP_1588_DM_MAC_L3_2     0x090f35d0 /* P1588 DM MAC Address Local3 0 Register */
#define BRPHY_TOP_1588_DM_MAC_CTL_0    0x090f35d2 /* P1588 DM MAC cONTROL 0 Register */
#define BRPHY_TOP_1588_DM_MAC_CTL_1    0x090f35d4 /* P1588 DM MAC CONTROL 1 Register */
#define BRPHY_TOP_1588_DM_MAC_CTL_2    0x090f35d6 /* P1588 DM MAC CONTROL 2 Register */
#define BRPHY_TOP_1588_HEARTBEAT_3     0x090f35d8 /* P1588 Heartbeat Register(3) */
#define BRPHY_TOP_1588_HEARTBEAT_4     0x090f35da /* P1588 Heartbeat Register(4) */
#define BRPHY_TOP_1588_INBAND_CNTL_0   0x090f35dc /* P1588 Inband Control Port0 Register */
#define BRPHY_TOP_1588_INBAND_CNTL_1   0x090f35de /* P1588 Inband Control Port1 Register */
#define BRPHY_TOP_1588_INBAND_CNTL_2   0x090f35e0 /* P1588 Inband Control Port2 Register */
#define BRPHY_TOP_1588_INBAND_CNTL_3   0x090f35e2 /* P1588 Inband Control Port3 Register */
#define BRPHY_TOP_1588_INBAND_CNTL_4   0x090f35e4 /* P1588 Inband Control Port4 Register */
#define BRPHY_TOP_1588_INBAND_CNTL_5   0x090f35e6 /* P1588 Inband Control Port5 Register */
#define BRPHY_TOP_1588_INBAND_CNTL_6   0x090f35e8 /* P1588 Inband Control Port6 Register */
#define BRPHY_TOP_1588_INBAND_CNTL_7   0x090f35ea /* P1588 Inband Control Port7 Register */
#define BRPHY_TOP_1588_MEM_COUNTER     0x090f35ec /* P1588 Memory Counter Register */
#define BRPHY_TOP_1588_TIMESTAMP_DELTA 0x090f35ee /* P1588 Timestamp Delta Register */
#define BRPHY_TOP_1588_SOP_SEL         0x090f35f0 /* P1588 SOP Selection Register */
#define BRPHY_TOP_1588_TIME_STAMP_INFO_3 0x090f35f2 /* P1588 Time Stamp Register Info (3) */
#define BRPHY_TOP_1588_TIME_STAMP_INFO_4 0x090f35f4 /* P1588 Time Stamp Register Info (4) */
#define BRPHY_TOP_1588_TIME_STAMP_INFO_5 0x090f35f6 /* P1588 Time Stamp Register Info (5) */
#define BRPHY_TOP_1588_TIME_STAMP_INFO_6 0x090f35f8 /* P1588 Time Stamp Register Info (6) */
#define BRPHY_TOP_1588_TIME_STAMP_INFO_7 0x090f35fa /* P1588 Time Stamp Register Info (7) */
#define BRPHY_TOP_1588_TIME_STAMP_INFO_8 0x090f35fc /* P1588 Time Stamp Register Info (8) */
#define BRPHY_TOP_1588_INBAND_SPARE1   0x090f35fe /* P1588 Inband Spare1 Register */


/****************************************************************************
 * bcm89530_top_bridge_brphys_brphys0_BRPHY0_BR_CL22_IEEE
 ***************************************************************************/
#define BRPHY0_BR_CL22_IEEE_MII_CTRL   0x090fffc0 /* BR_LRE_Control_Register */
#define BRPHY0_BR_CL22_IEEE_MII_STAT   0x090fffc2 /* BR_LRE_Status_Register */
#define BRPHY0_BR_CL22_IEEE_PHY_ID_MSB 0x090fffc4 /* PHY_Identifier_MSB_Register */
#define BRPHY0_BR_CL22_IEEE_PHY_ID_LSB 0x090fffc6 /* PHY_Identifier_LSB_Register */
#define BRPHY0_BR_CL22_IEEE_LDS_ADV_ABILITY_BP 0x090fffc8 /* LDS_Advertised_Ability_Register (Base Page) */
#define BRPHY0_BR_CL22_IEEE_LDS_ADV_CONTROL 0x090fffca /* LDS_Advertised_Control_Register */
#define BRPHY0_BR_CL22_IEEE_LDS_ABILITY 0x090fffcc /* LDS_Ability_Register (Next Page) */
#define BRPHY0_BR_CL22_IEEE_LDS_LP_ABILITY_BP 0x090fffce /* LDS_Link_Partner_Ability_Base_Page_Register (Base Page) */
#define BRPHY0_BR_CL22_IEEE_LDS_LP_MSG_NXTP 0x090fffd0 /* LDS_Link_Partners_Nxt_Pg_Msg_Register (Next Page) */
#define BRPHY0_BR_CL22_IEEE_LDS_LP_ABILITY_NXTP 0x090fffd2 /* LDS_Link_Partner_Ability_Nxt_Pg_Register (Next Page) */
#define BRPHY0_BR_CL22_IEEE_LDS_LDS_EXP 0x090fffd4 /* LDS_Expansion_Register */
#define BRPHY0_BR_CL22_IEEE_LRE_EXTENDED_STAT 0x090fffd6 /* IEEE_Extended_Status_Register */


/****************************************************************************
 * bcm89530_top_bridge_brphys_brphys1_BRPHY1_CL45DEV1
 ***************************************************************************/
#define BRPHY1_CL45DEV1_PMD_IEEE_CTL1  0x09420000 /* IEEE PMA/PMD CONTROL 1 REGISTER (REG 1.0) */
#define BRPHY1_CL45DEV1_PMD_IEEE_ST1   0x09420002 /* IEEE PMA/PMD STATUS 1 REGISTER  (REG 1.1) */
#define BRPHY1_CL45DEV1_PMD_IEEE_DEV_ID0 0x09420004 /* IEEE PMA/PMD DEVICE IDENTIFIER PART 0 (REG 1.2) */
#define BRPHY1_CL45DEV1_PMD_IEEE_DEV_ID1 0x09420006 /* IEEE PMA/PMD DEVICE IDENTIFIER PART 1 (REG 1.3) */
#define BRPHY1_CL45DEV1_PMD_IEEE_DEV0  0x0942000a /* DEVICES IN PACKAGE REGISTER PART 0 (REG 1.5) */
#define BRPHY1_CL45DEV1_PMD_IEEE_DEV1  0x0942000c /* DEVICES IN PACKAGE REGISTER PART 1 (REG 1.6) */
#define BRPHY1_CL45DEV1_PMD_IEEE_PKG_ID0 0x0942001c /* PMA/PMD PACKAGE IDENTIFIER (REG 1.14) */
#define BRPHY1_CL45DEV1_PMD_IEEE_PKG_ID1 0x0942001e /* PMA/PMD PACKAGE IDENTIFIER (REG 1.15) */
#define BRPHY1_CL45DEV1_PMD_IEEE_TSYNC_CAP 0x09420e10 /* TimeSync PMA/PMD capability (REG 1.1800) */
#define BRPHY1_CL45DEV1_PMD_IEEE_TX_TSYNC_MAX_DELAY_LOWER 0x09420e12 /* Maximum PMA/PMD transmit path data delay, lower (REG 1.1801) */
#define BRPHY1_CL45DEV1_PMD_IEEE_TX_TSYNC_MAX_DELAY_UPPER 0x09420e14 /* Maximum PMA/PMD transmit path data delay, upper (REG 1.1802) */
#define BRPHY1_CL45DEV1_PMD_IEEE_TX_TSYNC_MIN_DELAY_LOWER 0x09420e16 /* Minimum PMA/PMD transmit path data delay, lower (REG 1.1803) */
#define BRPHY1_CL45DEV1_PMD_IEEE_TX_TSYNC_MIN_DELAY_UPPER 0x09420e18 /* Minimum PMA/PMD transmit path data delay, upper (REG 1.1804) */
#define BRPHY1_CL45DEV1_PMD_IEEE_RX_TSYNC_MAX_DELAY_LOWER 0x09420e1a /* Maximum PMA/PMD receive path data delay, lower (REG 1.1805) */
#define BRPHY1_CL45DEV1_PMD_IEEE_RX_TSYNC_MAX_DELAY_UPPER 0x09420e1c /* Maximum PMA/PMD receive path data delay, upper (REG 1.1806) */
#define BRPHY1_CL45DEV1_PMD_IEEE_RX_TSYNC_MIN_DELAY_LOWER 0x09420e1e /* Minimum PMA/PMD receive path data delay, lower (REG 1.1807) */
#define BRPHY1_CL45DEV1_PMD_IEEE_RX_TSYNC_MIN_DELAY_UPPER 0x09420e20 /* Minimum PMA/PMD receive path data delay, upper (REG 1.1808) */


/****************************************************************************
 * bcm89530_top_bridge_brphys_brphys1_BRPHY1_CL45DEV3
 ***************************************************************************/
#define BRPHY1_CL45DEV3_PCS_IEEE_CTL1  0x09460000 /* IEEE PCS CONTROL 1 REGISTER (REG 3.0) */
#define BRPHY1_CL45DEV3_PCS_IEEE_ST1   0x09460002 /* IEEE PCS STATUS 1 REGISTER  (REG 3.1) */
#define BRPHY1_CL45DEV3_PCS_IEEE_DEV_ID0 0x09460004 /* IEEE PCS DEVICE IDENTIFIER PART 0 (REG 3.2) */
#define BRPHY1_CL45DEV3_PCS_IEEE_DEV_ID1 0x09460006 /* IEEE PCS DEVICE IDENTIFIER PART 1 (REG 3.3) */
#define BRPHY1_CL45DEV3_PCS_IEEE_DEV0  0x0946000a /* DEVICES IN PACKAGE REGISTER PART 0 (REG 3.5) */
#define BRPHY1_CL45DEV3_PCS_IEEE_DEV1  0x0946000c /* DEVICES IN PACKAGE REGISTER PART 1 (REG 3.6) */
#define BRPHY1_CL45DEV3_PCS_IEEE_PKG_ID0 0x0946001c /* PCS PACKAGE IDENTIFIER (REG 3.14) */
#define BRPHY1_CL45DEV3_PCS_IEEE_PKG_ID1 0x0946001e /* PCS PACKAGE IDENTIFIER (REG 3.15) */
#define BRPHY1_CL45DEV3_PCS_EEE_CAP    0x09460028 /* PCS_EEE_CAP(REG 3.20) */
#define BRPHY1_CL45DEV3_PCS_EEE_WAKE_ERR_CNT 0x0946002c /* PCS_EEE_Wake_Err_Cnt(REG 3.22) */
#define BRPHY1_CL45DEV3_PCS_IEEE_TSYNC_CAP 0x09460e10 /* TimeSync PCS capability (REG 3.1800) */
#define BRPHY1_CL45DEV3_PCS_IEEE_TX_TSYNC_MAX_DELAY_LOWER 0x09460e12 /* Maximum PCS transmit path data delay, lower (REG 3.1801) */
#define BRPHY1_CL45DEV3_PCS_IEEE_TX_TSYNC_MAX_DELAY_UPPER 0x09460e14 /* Maximum PCS transmit path data delay, upper (REG 3.1802) */
#define BRPHY1_CL45DEV3_PCS_IEEE_TX_TSYNC_MIN_DELAY_LOWER 0x09460e16 /* Minimum PCS transmit path data delay, lower (REG 3.1803) */
#define BRPHY1_CL45DEV3_PCS_IEEE_TX_TSYNC_MIN_DELAY_UPPER 0x09460e18 /* Minimum PCS transmit path data delay, upper (REG 3.1804) */
#define BRPHY1_CL45DEV3_PCS_IEEE_RX_TSYNC_MAX_DELAY_LOWER 0x09460e1a /* Maximum PCS receive path data delay, lower (REG 3.1805) */
#define BRPHY1_CL45DEV3_PCS_IEEE_RX_TSYNC_MAX_DELAY_UPPER 0x09460e1c /* Maximum PCS receive path data delay, upper (REG 3.1806) */
#define BRPHY1_CL45DEV3_PCS_IEEE_RX_TSYNC_MIN_DELAY_LOWER 0x09460e1e /* Minimum PCS receive path data delay, lower (REG 3.1807) */
#define BRPHY1_CL45DEV3_PCS_IEEE_RX_TSYNC_MIN_DELAY_UPPER 0x09460e20 /* Minimum PCS receive path data delay, upper (REG 3.1808) */


/****************************************************************************
 * bcm89530_top_bridge_brphys_brphys1_BRPHY1_CL45DEV7
 ***************************************************************************/
#define BRPHY1_CL45DEV7_AN_CTRL        0x094e0000 /* Auto Neg Extended Next Page Control     (0x0000) (REG 7.0) */
#define BRPHY1_CL45DEV7_AN_STAT        0x094e0002 /* AN Status (0x0001) (REG 7.1) */
#define BRPHY1_CL45DEV7_AN_DEV_ID_LSB  0x094e0004 /* Auto Neg Device Identifier Lower 16 bit (0x0002) (REG 7.2) */
#define BRPHY1_CL45DEV7_AN_DEV_ID_MSB  0x094e0006 /* Auto Neg Device Identifier Upper 16 bit (0x0003) (REG 7.3) */
#define BRPHY1_CL45DEV7_AN_DEV_IN_PKG_LSB 0x094e000a /* Auto Neg Device In Package Lower 16 bit (0x0005) (REG 7.5) */
#define BRPHY1_CL45DEV7_AN_DEV_IN_PKG_MSB 0x094e000c /* Auto Neg Device In Package Upper 16 bit (0x0006) (REG 7.6) */
#define BRPHY1_CL45DEV7_AN_DEV_PKG_ID_LSB 0x094e001c /* Auto Neg Package ID Lower 16 bit(0x000e) (REG 7.14) */
#define BRPHY1_CL45DEV7_AN_DEV_PKG_ID_MSB 0x094e001e /* Auto Neg Package ID Upper 16 bit(0x000f) (REG 7.15) */
#define BRPHY1_CL45DEV7_AN_AD          0x094e0020 /* Auto Neg AD(0x0010) (REG 7.16) */
#define BRPHY1_CL45DEV7_AN_LPA         0x094e0026 /* AN LP base page ability (0x0013) (REG 7.19) */
#define BRPHY1_CL45DEV7_AN_XNPA        0x094e002c /* AN XNP transmit A (0x0016) (REG 7.22) */
#define BRPHY1_CL45DEV7_AN_XNPB        0x094e002e /* AN XNP transmit B (0x0017) (REG 7.23) */
#define BRPHY1_CL45DEV7_AN_XNPC        0x094e0030 /* AN XNP transmit C (0x0018) (REG 7.24) */
#define BRPHY1_CL45DEV7_LP_XNPA        0x094e0032 /* AN LP XNP ability A (0x0019) (REG 7.25) */
#define BRPHY1_CL45DEV7_LP_XNPB        0x094e0034 /* AN LP XNP ability B (0x001a) (REG 7.26) */
#define BRPHY1_CL45DEV7_LP_XNPC        0x094e0036 /* AN LP XNP ability C (0x001b) (REG 7.27) */
#define BRPHY1_CL45DEV7_TENG_AN_CTRL   0x094e0040 /* 10G Base-T AN Control Register (0x0020) (REG 7.32) */
#define BRPHY1_CL45DEV7_TENG_AN_STAT   0x094e0042 /* 10G Base-T AN Status Register  (0x0021) (REG 7.33) */
#define BRPHY1_CL45DEV7_EEE_ADV        0x094e0078 /* EEE Advertisement (0x003C) (REG 7.60 ???) */
#define BRPHY1_CL45DEV7_EEE_LP_ADV     0x094e007a /* EEE Link Partner Advertisement (0x003D) (REG 7.61 ???) */
#define BRPHY1_CL45DEV7_EEE_MODE_CTL   0x094e007c /* EEE Mode Control (0x003E) (REG 7.62 ???) */


/****************************************************************************
 * bcm89530_top_bridge_brphys_brphys1_BRPHY1_CL45VEN
 ***************************************************************************/
#define BRPHY1_CL45VEN_FORCE_LINK      0x094f0000 /* Force Link Register */
#define BRPHY1_CL45VEN_SELECTIVE_RESET 0x094f0002 /* Selective Reset Register */
#define BRPHY1_CL45VEN_TEST_FSM_EXT_NXT_PGS 0x094f0004 /* Test State Machine For Extended Next Pages Register --mvadkert */
#define BRPHY1_CL45VEN_TEST_FSM_NXT_PGS 0x094f0006 /* Test State Machine For Next Pages Register --mvadkert */
#define BRPHY1_CL45VEN_AN_MAN_TEST     0x094f0032 /* Auto Negotiation Manual Test Register */
#define BRPHY1_CL45VEN_AN_MAN_LNK_PARTNR_ABI_A 0x094f0034 /* Auto Negotiation Manual Link Partners Abilities Register A */
#define BRPHY1_CL45VEN_AN_MAN_LNK_PARTNR_ABI_B 0x094f0036 /* Auto Negotiation Manual Link Partners Abilities Register B */
#define BRPHY1_CL45VEN_LNK_PARTNR_NXT_PG_A 0x094f0038 /* Link Partner Next Page */
#define BRPHY1_CL45VEN_LNK_PARTNR_NXT_PG_B 0x094f003a /* Link Partner Next Page (cont.) */
#define BRPHY1_CL45VEN_LNK_PARTNR_NXT_PG_C 0x094f003c /* Link Partner Next Page (cont.) */
#define BRPHY1_CL45VEN_LNK_PARTNR_NXT_PG_D 0x094f003e /* Link Partner Next Page (cont.) */
#define BRPHY1_CL45VEN_LNK_PARTNR_NXT_PG_E 0x094f0040 /* Link Partner Next Page (cont.) */
#define BRPHY1_CL45VEN_LNK_PARTNR_NXT_PG_F 0x094f0042 /* Link Partner Next Page (cont.) */
#define BRPHY1_CL45VEN_EPON_CTRL_REG   0x094f0046 /* EPON mode control register */
#define BRPHY1_CL45VEN_EEE_TEST_CTRL_A 0x094f0060 /* EEE Test Control Register A eee_test_control_bus[15:0] */
#define BRPHY1_CL45VEN_EEE_TEST_CTRL_B 0x094f0062 /* EEE Test Control Register B eee_test_control_bus[31:16] */
#define BRPHY1_CL45VEN_EEE_TEST_CTRL_C 0x094f0064 /* EEE Test Control Register C eee_test_control_bus[47:32] */
#define BRPHY1_CL45VEN_EEE_SPARE_1     0x094f0076 /* EEE Spare Register 1 */
#define BRPHY1_CL45VEN_EEE_SPARE_2     0x094f0078 /* EEE Spare Register 2 */
#define BRPHY1_CL45VEN_EEE_CONTROL     0x094f007a /* EEE Control Register */
#define BRPHY1_CL45VEN_EEE_RES_STAT    0x094f007c /* EEE Resolution Status Register */
#define BRPHY1_CL45VEN_LPI_MODE_CNTR   0x094f007e /* LPI Mode Counter Register */
#define BRPHY1_CL45VEN_LOC_DEV_MSG_5_A 0x094f0080 /* Local Device Message 5 */
#define BRPHY1_CL45VEN_LOC_DEV_MSG_5_B 0x094f0082 /* Local Device Message 5 cont. */
#define BRPHY1_CL45VEN_LOC_DEV_MSG_5_C 0x094f0084 /* Local Device Message 5 cont. */
#define BRPHY1_CL45VEN_LOC_DEV_MSG_5_D 0x094f0086 /* Local Device Message 5 cont. */
#define BRPHY1_CL45VEN_LNK_PARTNR_MSG_5_A 0x094f0088 /* Link Partner Message 5 */
#define BRPHY1_CL45VEN_LNK_PARTNR_MSG_5_B 0x094f008a /* Link Partner Message 5 cont. */
#define BRPHY1_CL45VEN_LNK_PARTNR_MSG_5_C 0x094f008c /* Link Partner Message 5 cont. */
#define BRPHY1_CL45VEN_LNK_PARTNR_MSG_5_D 0x094f008e /* Link Partner Message 5 cont. */
#define BRPHY1_CL45VEN_LOC_DEV_MSG_6_A 0x094f0090 /* Local Device Message 6 */
#define BRPHY1_CL45VEN_LOC_DEV_MSG_6_B 0x094f0092 /* Local Device Message 6 cont. */
#define BRPHY1_CL45VEN_LOC_DEV_MSG_6_C 0x094f0094 /* Local Device Message 6 cont. */
#define BRPHY1_CL45VEN_LOC_DEV_MSG_6_D 0x094f0096 /* Local Device Message 6 cont. */
#define BRPHY1_CL45VEN_LNK_PARTNR_MSG_6_A 0x094f0098 /* Link Partner Message 6 */
#define BRPHY1_CL45VEN_LNK_PARTNR_MSG_6_B 0x094f009a /* Link Partner Message 6 cont. */
#define BRPHY1_CL45VEN_LNK_PARTNR_MSG_6_C 0x094f009c /* Link Partner Message 6 cont. */
#define BRPHY1_CL45VEN_LNK_PARTNR_MSG_6_D 0x094f009e /* Link Partner Message 6 cont. */


/****************************************************************************
 * bcm89530_top_bridge_brphys_brphys1_BRPHY1_GPHY_CORE
 ***************************************************************************/
#define BRPHY1_GPHY_CORE_BASE10        0x094f2000 /* PHY_Extended_ctl_Register */
#define BRPHY1_GPHY_CORE_BASE11        0x094f2002 /* PHY_Extended_Status_Register (copper side only) */
#define BRPHY1_GPHY_CORE_BASE12        0x094f2004 /* Receive_Error_Cntr_Register */
#define BRPHY1_GPHY_CORE_BASE13        0x094f2006 /* False_Carrier_Sense_Cntr_Register */
#define BRPHY1_GPHY_CORE_BASE14        0x094f2008 /* Local_Remote_Receiver_NOT_OK_Cntrs_Register */
#define BRPHY1_GPHY_CORE_EXP45         0x094f200a /* Pattern Generator Control Register */
#define BRPHY1_GPHY_CORE_EXP46         0x094f200b /* Pattern Generator Status Register */
#define BRPHY1_GPHY_CORE_BASE19        0x094f2012 /* Auxiliary Status Summary (copper side only) */
#define BRPHY1_GPHY_CORE_BASE1A        0x094f2014 /* Interrupt Status Register (copper side only) */
#define BRPHY1_GPHY_CORE_BASE1B        0x094f2016 /* Interrupt Mask Register */
#define BRPHY1_GPHY_CORE_BASE1D_SHD    0x094f2018 /* HCD Status Register */
#define BRPHY1_GPHY_CORE_BASE1D        0x094f201a /* Master/Slave Seed Register */
#define BRPHY1_GPHY_CORE_BASE1E        0x094f201c /* Test1_Register */
#define BRPHY1_GPHY_CORE_BASE1F        0x094f201e /* Test2_Register */
#define BRPHY1_GPHY_CORE_SHD1C_00      0x094f2020 /* Cabletron LED Register (Shadow Register Selector = "00h") */
#define BRPHY1_GPHY_CORE_SHD1C_01      0x094f2022 /* TVCO Selection Register (Shadow Register Selector = "01h") */
#define BRPHY1_GPHY_CORE_SHD1C_02      0x094f2024 /* reserved Control 1 Register (Shadow Register Selector = "02h") */
#define BRPHY1_GPHY_CORE_SHD1C_03      0x094f2026 /* Clock Alignment Control Regsiter (Shadow Register Selector = "03h") */
#define BRPHY1_GPHY_CORE_SHD1C_04      0x094f2028 /* reserved Control 2 Register (Shadow Register Selector = "04h") */
#define BRPHY1_GPHY_CORE_SHD1C_05      0x094f202a /* reserved Control 3 Register (Shadow Register Selector = "05h") */
#define BRPHY1_GPHY_CORE_SHD1C_06      0x094f202c /* Tdr Control 1 Register (Shadow Register Selector = "06h") */
#define BRPHY1_GPHY_CORE_SHD1C_07      0x094f202e /* Tdr Control 2 Register (Shadow Register Selector = "07h") */
#define BRPHY1_GPHY_CORE_SHD1C_08      0x094f2030 /* Led Status Register (Shadow Register Selector = "08h") */
#define BRPHY1_GPHY_CORE_SHD1C_09      0x094f2032 /* Led Control Register (Shadow Register Selector = "09h") */
#define BRPHY1_GPHY_CORE_SHD1C_0A      0x094f2034 /* Auto-Power Down Register (Shadow Register Selector = "0ah") */
#define BRPHY1_GPHY_CORE_SHD1C_0B      0x094f2036 /* reserved Control 4 Register (Shadow Register Selector = "0bh") */
#define BRPHY1_GPHY_CORE_SHD1C_0D      0x094f203a /* LED Selector 1 Register (Shadow Register Selector = "0dh") */
#define BRPHY1_GPHY_CORE_SHD1C_0E      0x094f203c /* LED Selector 2 Register (Shadow Register Selector = "0eh") */
#define BRPHY1_GPHY_CORE_SHD1C_0F      0x094f203e /* LED GPIO Control/Status Register (Shadow Register Selector = "0fh") */
#define BRPHY1_GPHY_CORE_SHD1C_10      0x094f2040 /* Cisco Enhanced Linkstatus Mode Control Register (Shadow Register Selector = "10h") */
#define BRPHY1_GPHY_CORE_SHD1C_1F      0x094f2042 /* Mode Control Register (Shadow Register Selector = "1fh") */
#define BRPHY1_GPHY_CORE_SHD18_000     0x094f2050 /* Auxiliary Control Register (Shadow Register Selector = "000") */
#define BRPHY1_GPHY_CORE_SHD18_001     0x094f2052 /* 10 Base-T Register (Shadow Register Selector = "001") */
#define BRPHY1_GPHY_CORE_SHD18_010     0x094f2054 /* Power/MII Control Register (Shadow Register Selector = "010") */
#define BRPHY1_GPHY_CORE_SHD18_011     0x094f2056 /* IP Phone Register  (Shadow Register Selector = "011") */
#define BRPHY1_GPHY_CORE_SHD18_100     0x094f2058 /* Misc Test Register  1 (Shadow Register Selector = "100") */
#define BRPHY1_GPHY_CORE_SHD18_101     0x094f205a /* Misc Test Register  2 (Shadow Register Selector = "101") */
#define BRPHY1_GPHY_CORE_SHD18_110     0x094f205c /* Manual IP Phone Seed Register (Shadow Register Selector = "110") */
#define BRPHY1_GPHY_CORE_SHD18_111     0x094f205e /* Miscellanous Control Register (Shadow Register Selector = "111") */
#define BRPHY1_GPHY_CORE_EXP00         0x094f2060 /* Transmit Packet Counter */
#define BRPHY1_GPHY_CORE_EXP01         0x094f2062 /* Expansion Interrupt Status */
#define BRPHY1_GPHY_CORE_EXP02         0x094f2064 /* Expansion Interrupt Mask */
#define BRPHY1_GPHY_CORE_EXP03         0x094f2066 /* Spare Registers */
#define BRPHY1_GPHY_CORE_EXP04         0x094f2068 /* Bicolor LED Selectors */
#define BRPHY1_GPHY_CORE_EXP05         0x094f206a /* Bicolor LED Flash Rate Controls */
#define BRPHY1_GPHY_CORE_EXP06         0x094f206c /* Bicolor LED Programmable Blink Controls */
#define BRPHY1_GPHY_CORE_EXP07         0x094f206e /* Far End Fault */
#define BRPHY1_GPHY_CORE_EXP08         0x094f2070 /* 10BT Controls */
#define BRPHY1_GPHY_CORE_EXP09         0x094f2072 /* AMRR Controls */
#define BRPHY1_GPHY_CORE_EXP0A         0x094f2074 /* DAC TEMPLATE Controls */
#define BRPHY1_GPHY_CORE_EXP0B         0x094f2076 /* External Status */
#define BRPHY1_GPHY_CORE_EXP0C         0x094f2078 /* Spare Registers */
#define BRPHY1_GPHY_CORE_EXP30         0x094f2080 /* Late Collision Counters Status Register */
#define BRPHY1_GPHY_CORE_EXP31         0x094f2082 /* Late Collision Counter [64:95] */
#define BRPHY1_GPHY_CORE_EXP32         0x094f2084 /* Late Collision Counter [96:127] */
#define BRPHY1_GPHY_CORE_EXP33         0x094f2086 /* Late Collision Counter [128:191] */
#define BRPHY1_GPHY_CORE_EXP34         0x094f2088 /* Late Collision Counter [192:319] */
#define BRPHY1_GPHY_CORE_EXP35         0x094f208a /* Late Collision Counter Threshold Register */
#define BRPHY1_GPHY_CORE_EXP36         0x094f208c /* Clock PPM Detection between Recovery and Transmit Clocks */
#define BRPHY1_GPHY_CORE_EXP37         0x094f208e /* Clock PPM Detection between GTX_CLK and Transmit Clocks */
#define BRPHY1_GPHY_CORE_EXP38         0x094f2090 /* IP PHONE Cable Length Status Register */
#define BRPHY1_GPHY_CORE_EXP42         0x094f20a2 /* Operating Mode Status */
#define BRPHY1_GPHY_CORE_EXP5F         0x094f20be /* PLL Frequency Offset Testmode Control */
#define BRPHY1_GPHY_CORE_EXP70         0x094f20e0 /* SOFT-RESET */
#define BRPHY1_GPHY_CORE_EXP71         0x094f20e2 /* Serial LED Control 1 */
#define BRPHY1_GPHY_CORE_EXP72         0x094f20e4 /* Serial LED Control 2 */
#define BRPHY1_GPHY_CORE_EXP73         0x094f20e6 /* LED Gating 2 (Used for dual-media applications) */
#define BRPHY1_GPHY_CORE_EXP74         0x094f20e8 /* LED Programmable Current Mode Control */
#define BRPHY1_GPHY_CORE_EXP75         0x094f20ea /* CED LED Error Mask */
#define BRPHY1_GPHY_CORE_EXP78         0x094f20f0 /* Misc Extended Control */
#define BRPHY1_GPHY_CORE_EXP7B         0x094f20f6 /* I2C Control */
#define BRPHY1_GPHY_CORE_EXP7C         0x094f20f8 /* I2C Status */
#define BRPHY1_GPHY_CORE_EXP7F         0x094f20fe /* External MACSec Interface Control */
#define BRPHY1_GPHY_CORE_ALIAS_18      0x094f2100 /* Alias to MII Reg 18 */
#define BRPHY1_GPHY_CORE_ALIAS_19      0x094f2102 /* Alias to MII Reg 19 */
#define BRPHY1_GPHY_CORE_ALIAS_1A      0x094f2104 /* Alias to MII Reg 1a */
#define BRPHY1_GPHY_CORE_ALIAS_1B      0x094f2106 /* Alias to MII Reg 1b */
#define BRPHY1_GPHY_CORE_ALIAS_1C      0x094f2108 /* Alias to MII Reg 1c */
#define BRPHY1_GPHY_CORE_ALIAS_1D      0x094f210a /* Alias to MII Reg 1d */
#define BRPHY1_GPHY_CORE_REG_MAP_CTL   0x094f210e /* MII Registers 10-1D mapping control */
#define BRPHY1_GPHY_CORE_EXP98         0x094f2130 /* First Slice of Quad-GPHY only): CAL-BIAS Status */
#define BRPHY1_GPHY_CORE_EXP9C         0x094f2138 /* SMII Control */
#define BRPHY1_GPHY_CORE_BT_LINK_FIX   0x094f214a /* 10BT LINK FIX Register */
#define BRPHY1_GPHY_CORE_SYNCE_PLUS_DBG 0x094f214c /* SyncE+ Debug */
#define BRPHY1_GPHY_CORE_SYNCE_PLUS    0x094f214e /* SyncE+ Status and Control */
#define BRPHY1_GPHY_CORE_EXPA8         0x094f2150 /* ADAPTIVE BIAS CONTROL */
#define BRPHY1_GPHY_CORE_EXPA9         0x094f2152 /* spare register */
#define BRPHY1_GPHY_CORE_EXPAA         0x094f2154 /* EEE Statistic timer 12hours lpi */
#define BRPHY1_GPHY_CORE_EXPAB         0x094f2156 /* EEE Statistic timer 12hours local */
#define BRPHY1_GPHY_CORE_EXPAC         0x094f2158 /* EEE Statistic loc lpi req 0_to_1 counter */
#define BRPHY1_GPHY_CORE_EXPAD         0x094f215a /* EEE Statistic rem lpi_req 0_to_1 counter */
#define BRPHY1_GPHY_CORE_EXPAE         0x094f215c /* spare register */
#define BRPHY1_GPHY_CORE_EXPAF         0x094f215e /* EEE Statistic counters ctrl/status */
#define BRPHY1_GPHY_CORE_EXPB0         0x094f2160 /* Bias Control 0 */
#define BRPHY1_GPHY_CORE_EXPB1         0x094f2162 /* Bias Control 1 */
#define BRPHY1_GPHY_CORE_EXPB2         0x094f2164 /* Bias Control 2 */
#define BRPHY1_GPHY_CORE_EXPE3         0x094f2166 /* TX PCS Delay 1000BT and 100TX (copper side) */
#define BRPHY1_GPHY_CORE_EXPE4         0x094f2168 /* TX PCS Delay 10BT (copper side) */
#define BRPHY1_GPHY_CORE_EXPE5         0x094f216a /* TX PCS Delay (fiber side) */
#define BRPHY1_GPHY_CORE_EXPE6         0x094f216c /* RX PCS Delay 1000BT and 100TX (copper side) */
#define BRPHY1_GPHY_CORE_EXPE7         0x094f216e /* RX PCS Delay 10BT (copper side) */
#define BRPHY1_GPHY_CORE_EXPE8         0x094f2170 /* RX PCS Delay (fiber side) */
#define BRPHY1_GPHY_CORE_EXPE9         0x094f2172 /* P1588 TX/RX Cycle Delay */
#define BRPHY1_GPHY_CORE_EXPE0         0x094f2174 /* TX PMA/PMD Delay (copper side) */
#define BRPHY1_GPHY_CORE_EXPE1         0x094f2176 /* TX PMA/PMD Delay (fiber side) */
#define BRPHY1_GPHY_CORE_EXPE2         0x094f2178 /* RX PMA/PMD Delay (copper side) */
#define BRPHY1_GPHY_CORE_EXPEA         0x094f217a /* TX/RX Adjustable Cycle Delay */
#define BRPHY1_GPHY_CORE_LED_PRA_MODE  0x094f2180 /* LED Proportional Rate Activity Control */
#define BRPHY1_GPHY_CORE_FIFO_CTL      0x094f2182 /* FIFO Control Register */
#define BRPHY1_GPHY_CORE_EXPD8         0x094f21b0 /* Halting agc/enc ctrl reg */
#define BRPHY1_GPHY_CORE_EXPF0         0x094f21e0 /* RGMII IBS Control */
#define BRPHY1_GPHY_CORE_EXPF5         0x094f21ea /* Time Sync */
#define BRPHY1_GPHY_CORE_EXPF6         0x094f21ec /* Analog Power Control Status */
#define BRPHY1_GPHY_CORE_EXPF7         0x094f21ee /* Auto-power Down Control Status */
#define BRPHY1_GPHY_CORE_EXPF8         0x094f21f0 /* Trim Settings from Fuse & to Bias Block */
#define BRPHY1_GPHY_CORE_EXPF9         0x094f21f2 /* reserved Register Bits */
#define BRPHY1_GPHY_CORE_EXPFA         0x094f21f4 /* Hidden Identifier */
#define BRPHY1_GPHY_CORE_EXPFB         0x094f21f6 /* TDR Override Values */
#define BRPHY1_GPHY_CORE_EXPFC         0x094f21f8 /*  */
#define BRPHY1_GPHY_CORE_EXPFD         0x094f21fa /* Clock gating control override value */
#define BRPHY1_GPHY_CORE_EXPFE         0x094f21fc /* Clock gating control override enable */
#define BRPHY1_GPHY_CORE_EXPFF         0x094f21fe /* Analog power control override */


/****************************************************************************
 * bcm89530_top_bridge_brphys_brphys1_BRPHY1_DSP_TAP
 ***************************************************************************/
#define BRPHY1_DSP_TAP_TAP0_C0         0x094f2200 /* AGC Control/Status Register A (x4) */
#define BRPHY1_DSP_TAP_TAP0_C1         0x094f2202 /* AGC Control/Status Register A (x4) */
#define BRPHY1_DSP_TAP_TAP0_C2         0x094f2204 /* AGC Control/Status Register A (x4) */
#define BRPHY1_DSP_TAP_TAP0_C3         0x094f2206 /* AGC Control/Status Register A (x4) */
#define BRPHY1_DSP_TAP_TAP1            0x094f2208 /* IPRF Control register (x1) */
#define BRPHY1_DSP_TAP_TAP2_C0         0x094f2210 /* MSE Status Register (x4) */
#define BRPHY1_DSP_TAP_TAP2_C1         0x094f2212 /* MSE Status Register (x4) */
#define BRPHY1_DSP_TAP_TAP2_C2         0x094f2214 /* MSE Status Register (x4) */
#define BRPHY1_DSP_TAP_TAP2_C3         0x094f2216 /* MSE Status Register (x4) */
#define BRPHY1_DSP_TAP_TAP3_C0         0x094f2218 /* Soft Decision (From DFE) Status Register (x4) */
#define BRPHY1_DSP_TAP_TAP3_C1         0x094f221a /* Soft Decision (From DFE) Status Register (x4) */
#define BRPHY1_DSP_TAP_TAP3_C2         0x094f221c /* Soft Decision (From DFE) Status Register (x4) */
#define BRPHY1_DSP_TAP_TAP3_C3         0x094f221e /* Soft Decision (From DFE) Status Register (x4) */
#define BRPHY1_DSP_TAP_TAP4_C0         0x094f2220 /* Phase Status Register and Coarse AGC High Gear (x4) */
#define BRPHY1_DSP_TAP_TAP4_C1         0x094f2222 /* Phase Status Register and Coarse AGC High Gear (x4) */
#define BRPHY1_DSP_TAP_TAP4_C2         0x094f2224 /* Phase Status Register and Coarse AGC High Gear (x4) */
#define BRPHY1_DSP_TAP_TAP4_C3         0x094f2226 /* Phase Status Register and Coarse AGC High Gear (x4) */
#define BRPHY1_DSP_TAP_TAP5_C0         0x094f2228 /* Wire Map/Skew Control/Status and Echo/NEXT Control and Transmitter Control and ADC Control Register and Slicer Zero (x4) */
#define BRPHY1_DSP_TAP_TAP5_C1         0x094f222a /* Wire Map/Skew Control/Status and Echo/NEXT Control and Transmitter Control and ADC Control Register and Slicer Zero (x4) */
#define BRPHY1_DSP_TAP_TAP5_C2         0x094f222c /* Wire Map/Skew Control/Status and Echo/NEXT Control and Transmitter Control and ADC Control Register and Slicer Zero (x4) */
#define BRPHY1_DSP_TAP_TAP5_C3         0x094f222e /* Wire Map/Skew Control/Status and Echo/NEXT Control and Transmitter Control and ADC Control Register and Slicer Zero (x4) */
#define BRPHY1_DSP_TAP_TAP6            0x094f2230 /* CFC Deadman Disable */
#define BRPHY1_DSP_TAP_TAP7_C0         0x094f2238 /* BIST TEST 0 */
#define BRPHY1_DSP_TAP_TAP7_C1         0x094f223a /* BIST TEST 1 */
#define BRPHY1_DSP_TAP_TAP7_C2         0x094f223c /* BIST TEST 2 */
#define BRPHY1_DSP_TAP_TAP8_C0         0x094f2240 /* ABIST TEST 0 */
#define BRPHY1_DSP_TAP_TAP8_C1         0x094f2242 /* ABIST TEST 1 */
#define BRPHY1_DSP_TAP_TAP8_C2         0x094f2244 /* ABIST TEST 2 */
#define BRPHY1_DSP_TAP_TAP8_C3         0x094f2246 /* BR HPF Control */
#define BRPHY1_DSP_TAP_TAP9            0x094f2248 /* Frequency Control/Status Register LSBs (x1) */
#define BRPHY1_DSP_TAP_TAP10           0x094f224a /* PLL Bandwidth Control/Status and Path Metric Reset Register (x1) */
#define BRPHY1_DSP_TAP_TAP11           0x094f224c /* PLL RCLK and TCLK Offset Freeze Register (x1) */
#define BRPHY1_DSP_TAP_TAP12_C0        0x094f2250 /* PLL ACLK Offset/Freeze Register (x4) */
#define BRPHY1_DSP_TAP_TAP12_C1        0x094f2252 /* PLL ACLK Offset/Freeze Register (x4) */
#define BRPHY1_DSP_TAP_TAP12_C2        0x094f2254 /* PLL ACLK Offset/Freeze Register (x4) */
#define BRPHY1_DSP_TAP_TAP12_C3        0x094f2256 /* PLL ACLK Offset/Freeze Register (x4) */
#define BRPHY1_DSP_TAP_TAP13           0x094f2258 /* HPF Bandwidth Control and Disable ADC LSBs (x1) */
#define BRPHY1_DSP_TAP_TAP14           0x094f225a /* MSE Threshold Register  #1 (x1) */
#define BRPHY1_DSP_TAP_TAP15           0x094f225c /* MSE Threshold Register #2 (x1) */
#define BRPHY1_DSP_TAP_TAP16_C0        0x094f2260 /* Logic Analyzer trigger delay (x1) */
#define BRPHY1_DSP_TAP_TAP16_C1        0x094f2262 /* BIST CRC Monitor (x4) */
#define BRPHY1_DSP_TAP_TAP16_C2        0x094f2264 /* BIST CRC Monitor (x4) */
#define BRPHY1_DSP_TAP_TAP16_C3        0x094f2266 /* BIST CRC Monitor (x4) */
#define BRPHY1_DSP_TAP_TAP17_C0        0x094f2268 /* Testmode testvalue (aliased with logic analyzer state selects) */
#define BRPHY1_DSP_TAP_TAP17_C1        0x094f226a /* Testmode and logic analyzer controls #1 */
#define BRPHY1_DSP_TAP_TAP17_C2        0x094f226c /* Logic analyzer controls #2 */
#define BRPHY1_DSP_TAP_TAP17_C3        0x094f226e /* Testmode and logic analyzer controls #3 */
#define BRPHY1_DSP_TAP_TAP18_C0        0x094f2270 /* Peak Noise detector (x4) */
#define BRPHY1_DSP_TAP_TAP18_C1        0x094f2272 /* Peak Noise detector (x4) */
#define BRPHY1_DSP_TAP_TAP18_C2        0x094f2274 /* Peak Noise detector (x4) */
#define BRPHY1_DSP_TAP_TAP18_C3        0x094f2276 /* Peak Noise detector (x4) */
#define BRPHY1_DSP_TAP_TAP20           0x094f2278 /* Echo Minimum Length and LMS/FIR delay adjustments (x1) */
#define BRPHY1_DSP_TAP_TAP21           0x094f227a /* Phy Control Monitors #1 (x1) */
#define BRPHY1_DSP_TAP_TAP22           0x094f227c /* Phy Control Monitors #2 (x1) */
#define BRPHY1_DSP_TAP_TAP23           0x094f227e /* Phy Control Monitors #3 (x1) */
#define BRPHY1_DSP_TAP_TAP24           0x094f2280 /* Phy Control Output Overrides #1 (x1) */
#define BRPHY1_DSP_TAP_TAP25           0x094f2282 /* Phy Control Output Overrides #2 (x1) */
#define BRPHY1_DSP_TAP_TAP26           0x094f2284 /* Phy Control Input Overrides #1 (x1) */
#define BRPHY1_DSP_TAP_TAP27           0x094f2286 /* Phy Control Input Overrides #2 (x1) */
#define BRPHY1_DSP_TAP_TAP28           0x094f2288 /* Phy Control Output Overrides #3 (x1) */
#define BRPHY1_DSP_TAP_TAP29           0x094f228a /* Phy Control Force State/Timers/Alternate Behaviour Register #1 (x1) */
#define BRPHY1_DSP_TAP_TAP30           0x094f228c /* Phy Control Force State/Timers/Alternate Behaviour Register #2 (x1) */
#define BRPHY1_DSP_TAP_TAP31_C0        0x094f2290 /* Channel Swap Override */
#define BRPHY1_DSP_TAP_TAP32_C0        0x094f2298 /* Transmit Testmode Sync Generation (x1) */
#define BRPHY1_DSP_TAP_FDFE_OV_RD      0x094f229a /* FDFE Override/Read Control Register */
#define BRPHY1_DSP_TAP_FDFE_COEFF      0x094f229c /* FDFE Coefficient Read Back Register */
#define BRPHY1_DSP_TAP_FDFE_BETA_THRESHOLD 0x094f229e /* FDFE Beta Threshold Control */
#define BRPHY1_DSP_TAP_TAP33_C0        0x094f22a0 /* eee dsp test */
#define BRPHY1_DSP_TAP_TAP33_C1        0x094f22a2 /* eee sigdet */
#define BRPHY1_DSP_TAP_TAP33_C2        0x094f22a4 /* eee_lpi_timers */
#define BRPHY1_DSP_TAP_TAP33_C3        0x094f22a6 /* spare register */
#define BRPHY1_DSP_TAP_TAP34_C0        0x094f22a8 /* eee frequency control */
#define BRPHY1_DSP_TAP_TAP34_C1        0x094f22aa /* eee Gigabit Mode BW control */
#define BRPHY1_DSP_TAP_TAP34_C2        0x094f22ac /* eee 100TX Mode BW control */
#define BRPHY1_DSP_TAP_TAP34_C3        0x094f22ae /* phasectl TPO monitor */
#define BRPHY1_DSP_TAP_TAP35_C0        0x094f22b0 /* eee 100Base-tx timer control 1 */
#define BRPHY1_DSP_TAP_TAP35_C1        0x094f22b2 /* eee 100Base-tx timer control 2 */
#define BRPHY1_DSP_TAP_TAP35_C2        0x094f22b4 /* eee 100Base-tx timer misc control */
#define BRPHY1_DSP_TAP_TAP35_C3        0x094f22b6 /* pcs_lpi_test */
#define BRPHY1_DSP_TAP_FILTER_CTL_CH0  0x094f22b8 /* Filter Freeze/Disable per channel Control */
#define BRPHY1_DSP_TAP_FILTER_CTL_CH1  0x094f22ba /* Filter Freeze/Disable per channel Control */
#define BRPHY1_DSP_TAP_FILTER_CTL_CH2  0x094f22bc /* Filter Freeze/Disable per channel Control */
#define BRPHY1_DSP_TAP_FILTER_CTL_CH3  0x094f22be /* Filter Freeze/Disable per channel Control */
#define BRPHY1_DSP_TAP_FILTER_CTL_FFEX_CH0 0x094f22c0 /* FFEX Freeze/Disable per channel Control */
#define BRPHY1_DSP_TAP_FILTER_CTL_FFEX_CH1 0x094f22c2 /* FFEX Freeze/Disable per channel Control */
#define BRPHY1_DSP_TAP_FILTER_CTL_FFEX_CH2 0x094f22c4 /* FFEX Freeze/Disable per channel Control */
#define BRPHY1_DSP_TAP_FILTER_CTL_FFEX_CH3 0x094f22c6 /* FFEX Freeze/Disable per channel Control */
#define BRPHY1_DSP_TAP_EMI_DATAPATH_CTL 0x094f22c8 /* EMI Datapath Control */
#define BRPHY1_DSP_TAP_EMI_DATAPATH_CTL2 0x094f22ca /* EMI Datapath Control2 */
#define BRPHY1_DSP_TAP_FFEX_CTL        0x094f22cc /* FFE X-tap Control */
#define BRPHY1_DSP_TAP_PHYC_BREAKPOINT_CTL0 0x094f22ce /* Phycontrol Breakpoint Control 0 */
#define BRPHY1_DSP_TAP_PHYC_BREAKPOINT_CTL1 0x094f22d0 /* Phycontrol Breakpoint Control 1 */
#define BRPHY1_DSP_TAP_FILTER_ADDR     0x094f2360 /* DSP Coefficient Address Register */
#define BRPHY1_DSP_TAP_FILTER_CTL      0x094f2362 /* DSP Control Register */
#define BRPHY1_DSP_TAP_FILTER_DATA     0x094f2364 /* DSP Coefficient Read/Write Port */


/****************************************************************************
 * bcm89530_top_bridge_brphys_brphys1_BRPHY1_PLL_CTRL
 ***************************************************************************/
#define BRPHY1_PLL_CTRL_PLLCTRL_0      0x094f2390 /* Analog pll control 0 */
#define BRPHY1_PLL_CTRL_PLLCTRL_1      0x094f2392 /* Analog pll control 1 */
#define BRPHY1_PLL_CTRL_PLLCTRL_2      0x094f2394 /* Analog pll control 2 */
#define BRPHY1_PLL_CTRL_PLLCTRL_3      0x094f2396 /* Analog pll control 3 */
#define BRPHY1_PLL_CTRL_PLLCTRL_4      0x094f2398 /* Analog pll control 4 */
#define BRPHY1_PLL_CTRL_PLLCTRL_5      0x094f239a /* Analog pll control 5 */
#define BRPHY1_PLL_CTRL_PLLCTRL_6      0x094f239c /* Analog pll control 6 */
#define BRPHY1_PLL_CTRL_PLL_STATUS_0   0x094f23a0 /* Analog PLL Status 0 */
#define BRPHY1_PLL_CTRL_PLL_STATUS_1   0x094f23a2 /* Analog PLL Status 1 */
#define BRPHY1_PLL_CTRL_AFE_SIGDET_STATUS 0x094f23a4 /* AFE Signal detect */
#define BRPHY1_PLL_CTRL_PLLCTRL_7      0x094f23a6 /* Analog pll control 7 */
#define BRPHY1_PLL_CTRL_PLLCTRL_8      0x094f23a8 /* Analog pll control 8 */


/****************************************************************************
 * bcm89530_top_bridge_brphys_brphys1_BRPHY1_AFE_CTRL
 ***************************************************************************/
#define BRPHY1_AFE_CTRL_RXCONFIG_0     0x094f23c0 /* RXCONFIG 15:0 */
#define BRPHY1_AFE_CTRL_RXCONFIG_1     0x094f23c2 /* RXCONFIG 31:16 */
#define BRPHY1_AFE_CTRL_RXCONFIG_2     0x094f23c4 /* RXCONFIG 47:32 */
#define BRPHY1_AFE_CTRL_RXCONFIG_3     0x094f23c6 /* RXCONFIG 63:48 */
#define BRPHY1_AFE_CTRL_RXCONFIG_4     0x094f23c8 /* RXCONFIG 79:64 */
#define BRPHY1_AFE_CTRL_RXCONFIG5_LP   0x094f23ca /* RXCONFIG 86:80 and LP tuning */
#define BRPHY1_AFE_CTRL_TX_CONFIG_0    0x094f23cc /* TXCONFIG 15:0 */
#define BRPHY1_AFE_CTRL_TX_CONFIG_1    0x094f23ce /* TXCONFIG 31:16 */
#define BRPHY1_AFE_CTRL_VDAC_ICTRL_0   0x094f23d0 /* VDAC CURRENT Control 15:0 */
#define BRPHY1_AFE_CTRL_VDAC_ICTRL_1   0x094f23d2 /* VDAC CURRENT Control 31:16 */
#define BRPHY1_AFE_CTRL_VDAC_ICTRL_2   0x094f23d4 /* VDAC CURRENT Control 51:36 */
#define BRPHY1_AFE_CTRL_VDAC_OTHERS_0  0x094f23d6 /* VDAC CURRENT 35:32 and others */
#define BRPHY1_AFE_CTRL_HPF_TRIM_OTHERS 0x094f23d8 /* HPF trim and reserved bits */
#define BRPHY1_AFE_CTRL_TX_EXTRA_CONFIG_0 0x094f23da /* TXCONFIG 15:0 */
#define BRPHY1_AFE_CTRL_TX_EXTRA_CONFIG_1 0x094f23dc /* TXCONFIG 15:0 */
#define BRPHY1_AFE_CTRL_TX_EXTRA_CONFIG_2 0x094f23de /* TXCONFIG 15:0 */
#define BRPHY1_AFE_CTRL_TEMPSEN_OTHERS 0x094f23e0 /* TEMPSEN_OTHERS 15:0 */
#define BRPHY1_AFE_CTRL_FUTURE_RSV     0x094f23e2 /* FUTURE_RSV 15:0 */


/****************************************************************************
 * bcm89530_top_bridge_brphys_brphys1_BRPHY1_ECD_CTRL
 ***************************************************************************/
#define BRPHY1_ECD_CTRL_EXPC0          0x094f2540 /* ECD Control and Status */
#define BRPHY1_ECD_CTRL_EXPC1          0x094f2542 /* ECD Fault Type */
#define BRPHY1_ECD_CTRL_EXPC2          0x094f2544 /* ECD Pair A Length Results */
#define BRPHY1_ECD_CTRL_EXPC3          0x094f2546 /* ECD Pair B Length Results */
#define BRPHY1_ECD_CTRL_EXPC4          0x094f2548 /* ECD Pair C Length Results */
#define BRPHY1_ECD_CTRL_EXPC5          0x094f254a /* ECD Pair D Length Results */
#define BRPHY1_ECD_CTRL_EXPC6          0x094f254c /* ECD XTALK Map */
#define BRPHY1_ECD_CTRL_EXPC7          0x094f254e /* ECD EXTRA RESERVED REGISTER */
#define BRPHY1_ECD_CTRL_EXPC8          0x094f2550 /* ECD EXTRA RESERVED REGISTER */
#define BRPHY1_ECD_CTRL_EXPC9          0x094f2552 /* ECD EXTRA RESERVED REGISTER */
#define BRPHY1_ECD_CTRL_EXPCA          0x094f2554 /* ECD EXTRA RESERVED REGISTER */
#define BRPHY1_ECD_CTRL_EXPCB          0x094f2556 /* ECD EXTRA RESERVED REGISTER */
#define BRPHY1_ECD_CTRL_EXPCC          0x094f2558 /* ECD EXTRA RESERVED REGISTER */
#define BRPHY1_ECD_CTRL_EXPCD          0x094f255a /* ECD EXTRA RESERVED REGISTER */
#define BRPHY1_ECD_CTRL_EXPCE          0x094f255c /* ECD EXTRA RESERVED REGISTER */
#define BRPHY1_ECD_CTRL_EXPCF          0x094f255e /* ECD EXTRA RESERVED REGISTER */
#define BRPHY1_ECD_CTRL_EXPE0          0x094f2560 /* ECD EXTRA RESERVED REGISTER */
#define BRPHY1_ECD_CTRL_EXPE1          0x094f2562 /* ECD EXTRA RESERVED REGISTER */
#define BRPHY1_ECD_CTRL_EXPE2          0x094f2564 /* ECD EXTRA RESERVED REGISTER */
#define BRPHY1_ECD_CTRL_EXPE3          0x094f2566 /* ECD EXTRA RESERVED REGISTER */
#define BRPHY1_ECD_CTRL_EXPE4          0x094f2568 /* ECD EXTRA RESERVED REGISTER */
#define BRPHY1_ECD_CTRL_EXPE5          0x094f256a /* ECD EXTRA RESERVED REGISTER */
#define BRPHY1_ECD_CTRL_EXPE6          0x094f256c /* ECD EXTRA RESERVED REGISTER */
#define BRPHY1_ECD_CTRL_EXPE7          0x094f256e /* ECD EXTRA RESERVED REGISTER */
#define BRPHY1_ECD_CTRL_EXPE8          0x094f2570 /* ECD EXTRA RESERVED REGISTER */
#define BRPHY1_ECD_CTRL_EXPE9          0x094f2572 /* ECD EXTRA RESERVED REGISTER */
#define BRPHY1_ECD_CTRL_EXPEA          0x094f2574 /* ECD EXTRA RESERVED REGISTER */
#define BRPHY1_ECD_CTRL_EXPEB          0x094f2576 /* ECD EXTRA RESERVED REGISTER */
#define BRPHY1_ECD_CTRL_EXPEC          0x094f2578 /* ECD EXTRA RESERVED REGISTER */
#define BRPHY1_ECD_CTRL_EXPED          0x094f257a /* ECD EXTRA RESERVED REGISTER */
#define BRPHY1_ECD_CTRL_EXPEE          0x094f257c /* ECD EXTRA RESERVED REGISTER */
#define BRPHY1_ECD_CTRL_EXPEF          0x094f257e /* ECD EXTRA RESERVED REGISTER */


/****************************************************************************
 * bcm89530_top_bridge_brphys_brphys1_BRPHY1_BR_CTRL
 ***************************************************************************/
#define BRPHY1_BR_CTRL_EXP90           0x094f2600 /* BroadReach LRE Misc Control */
#define BRPHY1_BR_CTRL_EXP91           0x094f2602 /* BroadReach LRE Misc Control */
#define BRPHY1_BR_CTRL_EXP92           0x094f2604 /* BroadReach LRE Misc Control */
#define BRPHY1_BR_CTRL_EXP93           0x094f2606 /* BroadReach LDS  Control */
#define BRPHY1_BR_CTRL_EXP94           0x094f2608 /* BroadReach LDS RX Control */
#define BRPHY1_BR_CTRL_EXP95           0x094f260a /* BroadReach LDS RX Control */
#define BRPHY1_BR_CTRL_EXP96           0x094f260c /* BroadReach LDS Scan, ARB and TX Status */
#define BRPHY1_BR_CTRL_EXP97           0x094f260e /* BroadReach LDS Scan, ARB and TX Status */
#define BRPHY1_BR_CTRL_EXP99           0x094f2612 /* BroadReach LDS Timer Control */
#define BRPHY1_BR_CTRL_EXP9A           0x094f2614 /* LDS Status */
#define BRPHY1_BR_CTRL_EXP9B           0x094f2616 /* BroadR-Reach PLL Control */
#define BRPHY1_BR_CTRL_EXP9D           0x094f261a /* EoC Internal Control 1 */
#define BRPHY1_BR_CTRL_EXP9E           0x094f261c /* LDS Length Threshold 0 */
#define BRPHY1_BR_CTRL_EXP9F           0x094f261e /* LDS Length Threshold 1 */
#define BRPHY1_BR_CTRL_EXPA0           0x094f2620 /* HLDS register, LDS extend advertisement register */
#define BRPHY1_BR_CTRL_EXPA1           0x094f2622 /* HLDS register, LDS link partner extend ability register */
#define BRPHY1_BR_CTRL_EXPA2           0x094f2624 /* HLDS Register */
#define BRPHY1_BR_CTRL_BR_MISC_CONTROL_STATUS 0x094f2626 /* Broadreach Misc Status */
#define BRPHY1_BR_CTRL_BR250_CTL       0x094f263c /* BR250 Control */


/****************************************************************************
 * bcm89530_top_bridge_brphys_brphys1_IND_ACC
 ***************************************************************************/
#define IND_ACC_RDB_IND_REGS_CTRL_SER_L16 0x094f3000 /* Indirect Access SPI/MDIO CTRL/STATUS reg low  16 bits */
#define IND_ACC_RDB_IND_REGS_ADDR_SER_L16 0x094f3004 /* Indirect Access SPI/MDIO Address reg low  16 bits */
#define IND_ACC_RDB_IND_REGS_ADDR_SER_H16 0x094f3006 /* Indirect Access SPI/MDIO Address reg high 16 bits */
#define IND_ACC_RDB_IND_REGS_DATA_SER_L_L16 0x094f3008 /* Indirect Access SPI/MDIO Data reg 15-0 bits */
#define IND_ACC_RDB_IND_REGS_DATA_SER_L_H16 0x094f300a /* Indirect Access SPI/MDIO Data reg 31-16 bits */
#define IND_ACC_RDB_IND_REGS_DATA_SER_H_L16 0x094f300c /* Indirect Access SPI/MDIO Data reg 47-32 bits */
#define IND_ACC_RDB_IND_REGS_DATA_SER_H_H16 0x094f300e /* Indirect Access SPI/MDIO Data reg 63-48 bits */
#define IND_ACC_RDB_IND_REGS_CTRL_CPU_L16 0x094f3010 /* Indirect Access CPU CTRL/STATUS reg low  16 bits */
#define IND_ACC_RDB_IND_REGS_ADDR_CPU_L16 0x094f3014 /* Indirect Access CPU Address reg low  16 bits */
#define IND_ACC_RDB_IND_REGS_ADDR_CPU_H16 0x094f3016 /* Indirect Access CPU Address reg high 16 bits */
#define IND_ACC_RDB_IND_REGS_DATA_CPU_L_L16 0x094f3018 /* Indirect Access CPU Data reg 15-0 bits */
#define IND_ACC_RDB_IND_REGS_DATA_CPU_L_H16 0x094f301a /* Indirect Access CPU Data reg 31-16 bits */
#define IND_ACC_RDB_IND_REGS_DATA_CPU_H_L16 0x094f301c /* Indirect Access CPU Data reg 47-32 bits */
#define IND_ACC_RDB_IND_REGS_DATA_CPU_H_H16 0x094f301e /* Indirect Access CPU Data reg 63-48 bits */


/****************************************************************************
 * bcm89530_top_bridge_brphys_brphys1_BRPHY1_BR_CL22_IEEE
 ***************************************************************************/
#define BRPHY1_BR_CL22_IEEE_MII_CTRL   0x094fffc0 /* BR_LRE_Control_Register */
#define BRPHY1_BR_CL22_IEEE_MII_STAT   0x094fffc2 /* BR_LRE_Status_Register */
#define BRPHY1_BR_CL22_IEEE_PHY_ID_MSB 0x094fffc4 /* PHY_Identifier_MSB_Register */
#define BRPHY1_BR_CL22_IEEE_PHY_ID_LSB 0x094fffc6 /* PHY_Identifier_LSB_Register */
#define BRPHY1_BR_CL22_IEEE_LDS_ADV_ABILITY_BP 0x094fffc8 /* LDS_Advertised_Ability_Register (Base Page) */
#define BRPHY1_BR_CL22_IEEE_LDS_ADV_CONTROL 0x094fffca /* LDS_Advertised_Control_Register */
#define BRPHY1_BR_CL22_IEEE_LDS_ABILITY 0x094fffcc /* LDS_Ability_Register (Next Page) */
#define BRPHY1_BR_CL22_IEEE_LDS_LP_ABILITY_BP 0x094fffce /* LDS_Link_Partner_Ability_Base_Page_Register (Base Page) */
#define BRPHY1_BR_CL22_IEEE_LDS_LP_MSG_NXTP 0x094fffd0 /* LDS_Link_Partners_Nxt_Pg_Msg_Register (Next Page) */
#define BRPHY1_BR_CL22_IEEE_LDS_LP_ABILITY_NXTP 0x094fffd2 /* LDS_Link_Partner_Ability_Nxt_Pg_Register (Next Page) */
#define BRPHY1_BR_CL22_IEEE_LDS_LDS_EXP 0x094fffd4 /* LDS_Expansion_Register */
#define BRPHY1_BR_CL22_IEEE_LRE_EXTENDED_STAT 0x094fffd6 /* IEEE_Extended_Status_Register */


/****************************************************************************
 * bcm89530_top_bridge_brphys_brphys2_BRPHY2_CL45DEV1
 ***************************************************************************/
#define BRPHY2_CL45DEV1_PMD_IEEE_CTL1  0x09820000 /* IEEE PMA/PMD CONTROL 1 REGISTER (REG 1.0) */
#define BRPHY2_CL45DEV1_PMD_IEEE_ST1   0x09820002 /* IEEE PMA/PMD STATUS 1 REGISTER  (REG 1.1) */
#define BRPHY2_CL45DEV1_PMD_IEEE_DEV_ID0 0x09820004 /* IEEE PMA/PMD DEVICE IDENTIFIER PART 0 (REG 1.2) */
#define BRPHY2_CL45DEV1_PMD_IEEE_DEV_ID1 0x09820006 /* IEEE PMA/PMD DEVICE IDENTIFIER PART 1 (REG 1.3) */
#define BRPHY2_CL45DEV1_PMD_IEEE_DEV0  0x0982000a /* DEVICES IN PACKAGE REGISTER PART 0 (REG 1.5) */
#define BRPHY2_CL45DEV1_PMD_IEEE_DEV1  0x0982000c /* DEVICES IN PACKAGE REGISTER PART 1 (REG 1.6) */
#define BRPHY2_CL45DEV1_PMD_IEEE_PKG_ID0 0x0982001c /* PMA/PMD PACKAGE IDENTIFIER (REG 1.14) */
#define BRPHY2_CL45DEV1_PMD_IEEE_PKG_ID1 0x0982001e /* PMA/PMD PACKAGE IDENTIFIER (REG 1.15) */
#define BRPHY2_CL45DEV1_PMD_IEEE_TSYNC_CAP 0x09820e10 /* TimeSync PMA/PMD capability (REG 1.1800) */
#define BRPHY2_CL45DEV1_PMD_IEEE_TX_TSYNC_MAX_DELAY_LOWER 0x09820e12 /* Maximum PMA/PMD transmit path data delay, lower (REG 1.1801) */
#define BRPHY2_CL45DEV1_PMD_IEEE_TX_TSYNC_MAX_DELAY_UPPER 0x09820e14 /* Maximum PMA/PMD transmit path data delay, upper (REG 1.1802) */
#define BRPHY2_CL45DEV1_PMD_IEEE_TX_TSYNC_MIN_DELAY_LOWER 0x09820e16 /* Minimum PMA/PMD transmit path data delay, lower (REG 1.1803) */
#define BRPHY2_CL45DEV1_PMD_IEEE_TX_TSYNC_MIN_DELAY_UPPER 0x09820e18 /* Minimum PMA/PMD transmit path data delay, upper (REG 1.1804) */
#define BRPHY2_CL45DEV1_PMD_IEEE_RX_TSYNC_MAX_DELAY_LOWER 0x09820e1a /* Maximum PMA/PMD receive path data delay, lower (REG 1.1805) */
#define BRPHY2_CL45DEV1_PMD_IEEE_RX_TSYNC_MAX_DELAY_UPPER 0x09820e1c /* Maximum PMA/PMD receive path data delay, upper (REG 1.1806) */
#define BRPHY2_CL45DEV1_PMD_IEEE_RX_TSYNC_MIN_DELAY_LOWER 0x09820e1e /* Minimum PMA/PMD receive path data delay, lower (REG 1.1807) */
#define BRPHY2_CL45DEV1_PMD_IEEE_RX_TSYNC_MIN_DELAY_UPPER 0x09820e20 /* Minimum PMA/PMD receive path data delay, upper (REG 1.1808) */


/****************************************************************************
 * bcm89530_top_bridge_brphys_brphys2_BRPHY2_CL45DEV3
 ***************************************************************************/
#define BRPHY2_CL45DEV3_PCS_IEEE_CTL1  0x09860000 /* IEEE PCS CONTROL 1 REGISTER (REG 3.0) */
#define BRPHY2_CL45DEV3_PCS_IEEE_ST1   0x09860002 /* IEEE PCS STATUS 1 REGISTER  (REG 3.1) */
#define BRPHY2_CL45DEV3_PCS_IEEE_DEV_ID0 0x09860004 /* IEEE PCS DEVICE IDENTIFIER PART 0 (REG 3.2) */
#define BRPHY2_CL45DEV3_PCS_IEEE_DEV_ID1 0x09860006 /* IEEE PCS DEVICE IDENTIFIER PART 1 (REG 3.3) */
#define BRPHY2_CL45DEV3_PCS_IEEE_DEV0  0x0986000a /* DEVICES IN PACKAGE REGISTER PART 0 (REG 3.5) */
#define BRPHY2_CL45DEV3_PCS_IEEE_DEV1  0x0986000c /* DEVICES IN PACKAGE REGISTER PART 1 (REG 3.6) */
#define BRPHY2_CL45DEV3_PCS_IEEE_PKG_ID0 0x0986001c /* PCS PACKAGE IDENTIFIER (REG 3.14) */
#define BRPHY2_CL45DEV3_PCS_IEEE_PKG_ID1 0x0986001e /* PCS PACKAGE IDENTIFIER (REG 3.15) */
#define BRPHY2_CL45DEV3_PCS_EEE_CAP    0x09860028 /* PCS_EEE_CAP(REG 3.20) */
#define BRPHY2_CL45DEV3_PCS_EEE_WAKE_ERR_CNT 0x0986002c /* PCS_EEE_Wake_Err_Cnt(REG 3.22) */
#define BRPHY2_CL45DEV3_PCS_IEEE_TSYNC_CAP 0x09860e10 /* TimeSync PCS capability (REG 3.1800) */
#define BRPHY2_CL45DEV3_PCS_IEEE_TX_TSYNC_MAX_DELAY_LOWER 0x09860e12 /* Maximum PCS transmit path data delay, lower (REG 3.1801) */
#define BRPHY2_CL45DEV3_PCS_IEEE_TX_TSYNC_MAX_DELAY_UPPER 0x09860e14 /* Maximum PCS transmit path data delay, upper (REG 3.1802) */
#define BRPHY2_CL45DEV3_PCS_IEEE_TX_TSYNC_MIN_DELAY_LOWER 0x09860e16 /* Minimum PCS transmit path data delay, lower (REG 3.1803) */
#define BRPHY2_CL45DEV3_PCS_IEEE_TX_TSYNC_MIN_DELAY_UPPER 0x09860e18 /* Minimum PCS transmit path data delay, upper (REG 3.1804) */
#define BRPHY2_CL45DEV3_PCS_IEEE_RX_TSYNC_MAX_DELAY_LOWER 0x09860e1a /* Maximum PCS receive path data delay, lower (REG 3.1805) */
#define BRPHY2_CL45DEV3_PCS_IEEE_RX_TSYNC_MAX_DELAY_UPPER 0x09860e1c /* Maximum PCS receive path data delay, upper (REG 3.1806) */
#define BRPHY2_CL45DEV3_PCS_IEEE_RX_TSYNC_MIN_DELAY_LOWER 0x09860e1e /* Minimum PCS receive path data delay, lower (REG 3.1807) */
#define BRPHY2_CL45DEV3_PCS_IEEE_RX_TSYNC_MIN_DELAY_UPPER 0x09860e20 /* Minimum PCS receive path data delay, upper (REG 3.1808) */


/****************************************************************************
 * bcm89530_top_bridge_brphys_brphys2_BRPHY2_CL45DEV7
 ***************************************************************************/
#define BRPHY2_CL45DEV7_AN_CTRL        0x098e0000 /* Auto Neg Extended Next Page Control     (0x0000) (REG 7.0) */
#define BRPHY2_CL45DEV7_AN_STAT        0x098e0002 /* AN Status (0x0001) (REG 7.1) */
#define BRPHY2_CL45DEV7_AN_DEV_ID_LSB  0x098e0004 /* Auto Neg Device Identifier Lower 16 bit (0x0002) (REG 7.2) */
#define BRPHY2_CL45DEV7_AN_DEV_ID_MSB  0x098e0006 /* Auto Neg Device Identifier Upper 16 bit (0x0003) (REG 7.3) */
#define BRPHY2_CL45DEV7_AN_DEV_IN_PKG_LSB 0x098e000a /* Auto Neg Device In Package Lower 16 bit (0x0005) (REG 7.5) */
#define BRPHY2_CL45DEV7_AN_DEV_IN_PKG_MSB 0x098e000c /* Auto Neg Device In Package Upper 16 bit (0x0006) (REG 7.6) */
#define BRPHY2_CL45DEV7_AN_DEV_PKG_ID_LSB 0x098e001c /* Auto Neg Package ID Lower 16 bit(0x000e) (REG 7.14) */
#define BRPHY2_CL45DEV7_AN_DEV_PKG_ID_MSB 0x098e001e /* Auto Neg Package ID Upper 16 bit(0x000f) (REG 7.15) */
#define BRPHY2_CL45DEV7_AN_AD          0x098e0020 /* Auto Neg AD(0x0010) (REG 7.16) */
#define BRPHY2_CL45DEV7_AN_LPA         0x098e0026 /* AN LP base page ability (0x0013) (REG 7.19) */
#define BRPHY2_CL45DEV7_AN_XNPA        0x098e002c /* AN XNP transmit A (0x0016) (REG 7.22) */
#define BRPHY2_CL45DEV7_AN_XNPB        0x098e002e /* AN XNP transmit B (0x0017) (REG 7.23) */
#define BRPHY2_CL45DEV7_AN_XNPC        0x098e0030 /* AN XNP transmit C (0x0018) (REG 7.24) */
#define BRPHY2_CL45DEV7_LP_XNPA        0x098e0032 /* AN LP XNP ability A (0x0019) (REG 7.25) */
#define BRPHY2_CL45DEV7_LP_XNPB        0x098e0034 /* AN LP XNP ability B (0x001a) (REG 7.26) */
#define BRPHY2_CL45DEV7_LP_XNPC        0x098e0036 /* AN LP XNP ability C (0x001b) (REG 7.27) */
#define BRPHY2_CL45DEV7_TENG_AN_CTRL   0x098e0040 /* 10G Base-T AN Control Register (0x0020) (REG 7.32) */
#define BRPHY2_CL45DEV7_TENG_AN_STAT   0x098e0042 /* 10G Base-T AN Status Register  (0x0021) (REG 7.33) */
#define BRPHY2_CL45DEV7_EEE_ADV        0x098e0078 /* EEE Advertisement (0x003C) (REG 7.60 ???) */
#define BRPHY2_CL45DEV7_EEE_LP_ADV     0x098e007a /* EEE Link Partner Advertisement (0x003D) (REG 7.61 ???) */
#define BRPHY2_CL45DEV7_EEE_MODE_CTL   0x098e007c /* EEE Mode Control (0x003E) (REG 7.62 ???) */


/****************************************************************************
 * bcm89530_top_bridge_brphys_brphys2_BRPHY2_CL45VEN
 ***************************************************************************/
#define BRPHY2_CL45VEN_FORCE_LINK      0x098f0000 /* Force Link Register */
#define BRPHY2_CL45VEN_SELECTIVE_RESET 0x098f0002 /* Selective Reset Register */
#define BRPHY2_CL45VEN_TEST_FSM_EXT_NXT_PGS 0x098f0004 /* Test State Machine For Extended Next Pages Register --mvadkert */
#define BRPHY2_CL45VEN_TEST_FSM_NXT_PGS 0x098f0006 /* Test State Machine For Next Pages Register --mvadkert */
#define BRPHY2_CL45VEN_AN_MAN_TEST     0x098f0032 /* Auto Negotiation Manual Test Register */
#define BRPHY2_CL45VEN_AN_MAN_LNK_PARTNR_ABI_A 0x098f0034 /* Auto Negotiation Manual Link Partners Abilities Register A */
#define BRPHY2_CL45VEN_AN_MAN_LNK_PARTNR_ABI_B 0x098f0036 /* Auto Negotiation Manual Link Partners Abilities Register B */
#define BRPHY2_CL45VEN_LNK_PARTNR_NXT_PG_A 0x098f0038 /* Link Partner Next Page */
#define BRPHY2_CL45VEN_LNK_PARTNR_NXT_PG_B 0x098f003a /* Link Partner Next Page (cont.) */
#define BRPHY2_CL45VEN_LNK_PARTNR_NXT_PG_C 0x098f003c /* Link Partner Next Page (cont.) */
#define BRPHY2_CL45VEN_LNK_PARTNR_NXT_PG_D 0x098f003e /* Link Partner Next Page (cont.) */
#define BRPHY2_CL45VEN_LNK_PARTNR_NXT_PG_E 0x098f0040 /* Link Partner Next Page (cont.) */
#define BRPHY2_CL45VEN_LNK_PARTNR_NXT_PG_F 0x098f0042 /* Link Partner Next Page (cont.) */
#define BRPHY2_CL45VEN_EPON_CTRL_REG   0x098f0046 /* EPON mode control register */
#define BRPHY2_CL45VEN_EEE_TEST_CTRL_A 0x098f0060 /* EEE Test Control Register A eee_test_control_bus[15:0] */
#define BRPHY2_CL45VEN_EEE_TEST_CTRL_B 0x098f0062 /* EEE Test Control Register B eee_test_control_bus[31:16] */
#define BRPHY2_CL45VEN_EEE_TEST_CTRL_C 0x098f0064 /* EEE Test Control Register C eee_test_control_bus[47:32] */
#define BRPHY2_CL45VEN_EEE_SPARE_1     0x098f0076 /* EEE Spare Register 1 */
#define BRPHY2_CL45VEN_EEE_SPARE_2     0x098f0078 /* EEE Spare Register 2 */
#define BRPHY2_CL45VEN_EEE_CONTROL     0x098f007a /* EEE Control Register */
#define BRPHY2_CL45VEN_EEE_RES_STAT    0x098f007c /* EEE Resolution Status Register */
#define BRPHY2_CL45VEN_LPI_MODE_CNTR   0x098f007e /* LPI Mode Counter Register */
#define BRPHY2_CL45VEN_LOC_DEV_MSG_5_A 0x098f0080 /* Local Device Message 5 */
#define BRPHY2_CL45VEN_LOC_DEV_MSG_5_B 0x098f0082 /* Local Device Message 5 cont. */
#define BRPHY2_CL45VEN_LOC_DEV_MSG_5_C 0x098f0084 /* Local Device Message 5 cont. */
#define BRPHY2_CL45VEN_LOC_DEV_MSG_5_D 0x098f0086 /* Local Device Message 5 cont. */
#define BRPHY2_CL45VEN_LNK_PARTNR_MSG_5_A 0x098f0088 /* Link Partner Message 5 */
#define BRPHY2_CL45VEN_LNK_PARTNR_MSG_5_B 0x098f008a /* Link Partner Message 5 cont. */
#define BRPHY2_CL45VEN_LNK_PARTNR_MSG_5_C 0x098f008c /* Link Partner Message 5 cont. */
#define BRPHY2_CL45VEN_LNK_PARTNR_MSG_5_D 0x098f008e /* Link Partner Message 5 cont. */
#define BRPHY2_CL45VEN_LOC_DEV_MSG_6_A 0x098f0090 /* Local Device Message 6 */
#define BRPHY2_CL45VEN_LOC_DEV_MSG_6_B 0x098f0092 /* Local Device Message 6 cont. */
#define BRPHY2_CL45VEN_LOC_DEV_MSG_6_C 0x098f0094 /* Local Device Message 6 cont. */
#define BRPHY2_CL45VEN_LOC_DEV_MSG_6_D 0x098f0096 /* Local Device Message 6 cont. */
#define BRPHY2_CL45VEN_LNK_PARTNR_MSG_6_A 0x098f0098 /* Link Partner Message 6 */
#define BRPHY2_CL45VEN_LNK_PARTNR_MSG_6_B 0x098f009a /* Link Partner Message 6 cont. */
#define BRPHY2_CL45VEN_LNK_PARTNR_MSG_6_C 0x098f009c /* Link Partner Message 6 cont. */
#define BRPHY2_CL45VEN_LNK_PARTNR_MSG_6_D 0x098f009e /* Link Partner Message 6 cont. */


/****************************************************************************
 * bcm89530_top_bridge_brphys_brphys2_BRPHY2_GPHY_CORE
 ***************************************************************************/
#define BRPHY2_GPHY_CORE_BASE10        0x098f2000 /* PHY_Extended_ctl_Register */
#define BRPHY2_GPHY_CORE_BASE11        0x098f2002 /* PHY_Extended_Status_Register (copper side only) */
#define BRPHY2_GPHY_CORE_BASE12        0x098f2004 /* Receive_Error_Cntr_Register */
#define BRPHY2_GPHY_CORE_BASE13        0x098f2006 /* False_Carrier_Sense_Cntr_Register */
#define BRPHY2_GPHY_CORE_BASE14        0x098f2008 /* Local_Remote_Receiver_NOT_OK_Cntrs_Register */
#define BRPHY2_GPHY_CORE_EXP45         0x098f200a /* Pattern Generator Control Register */
#define BRPHY2_GPHY_CORE_EXP46         0x098f200b /* Pattern Generator Status Register */
#define BRPHY2_GPHY_CORE_BASE19        0x098f2012 /* Auxiliary Status Summary (copper side only) */
#define BRPHY2_GPHY_CORE_BASE1A        0x098f2014 /* Interrupt Status Register (copper side only) */
#define BRPHY2_GPHY_CORE_BASE1B        0x098f2016 /* Interrupt Mask Register */
#define BRPHY2_GPHY_CORE_BASE1D_SHD    0x098f2018 /* HCD Status Register */
#define BRPHY2_GPHY_CORE_BASE1D        0x098f201a /* Master/Slave Seed Register */
#define BRPHY2_GPHY_CORE_BASE1E        0x098f201c /* Test1_Register */
#define BRPHY2_GPHY_CORE_BASE1F        0x098f201e /* Test2_Register */
#define BRPHY2_GPHY_CORE_SHD1C_00      0x098f2020 /* Cabletron LED Register (Shadow Register Selector = "00h") */
#define BRPHY2_GPHY_CORE_SHD1C_01      0x098f2022 /* TVCO Selection Register (Shadow Register Selector = "01h") */
#define BRPHY2_GPHY_CORE_SHD1C_02      0x098f2024 /* reserved Control 1 Register (Shadow Register Selector = "02h") */
#define BRPHY2_GPHY_CORE_SHD1C_03      0x098f2026 /* Clock Alignment Control Regsiter (Shadow Register Selector = "03h") */
#define BRPHY2_GPHY_CORE_SHD1C_04      0x098f2028 /* reserved Control 2 Register (Shadow Register Selector = "04h") */
#define BRPHY2_GPHY_CORE_SHD1C_05      0x098f202a /* reserved Control 3 Register (Shadow Register Selector = "05h") */
#define BRPHY2_GPHY_CORE_SHD1C_06      0x098f202c /* Tdr Control 1 Register (Shadow Register Selector = "06h") */
#define BRPHY2_GPHY_CORE_SHD1C_07      0x098f202e /* Tdr Control 2 Register (Shadow Register Selector = "07h") */
#define BRPHY2_GPHY_CORE_SHD1C_08      0x098f2030 /* Led Status Register (Shadow Register Selector = "08h") */
#define BRPHY2_GPHY_CORE_SHD1C_09      0x098f2032 /* Led Control Register (Shadow Register Selector = "09h") */
#define BRPHY2_GPHY_CORE_SHD1C_0A      0x098f2034 /* Auto-Power Down Register (Shadow Register Selector = "0ah") */
#define BRPHY2_GPHY_CORE_SHD1C_0B      0x098f2036 /* reserved Control 4 Register (Shadow Register Selector = "0bh") */
#define BRPHY2_GPHY_CORE_SHD1C_0D      0x098f203a /* LED Selector 1 Register (Shadow Register Selector = "0dh") */
#define BRPHY2_GPHY_CORE_SHD1C_0E      0x098f203c /* LED Selector 2 Register (Shadow Register Selector = "0eh") */
#define BRPHY2_GPHY_CORE_SHD1C_0F      0x098f203e /* LED GPIO Control/Status Register (Shadow Register Selector = "0fh") */
#define BRPHY2_GPHY_CORE_SHD1C_10      0x098f2040 /* Cisco Enhanced Linkstatus Mode Control Register (Shadow Register Selector = "10h") */
#define BRPHY2_GPHY_CORE_SHD1C_1F      0x098f2042 /* Mode Control Register (Shadow Register Selector = "1fh") */
#define BRPHY2_GPHY_CORE_SHD18_000     0x098f2050 /* Auxiliary Control Register (Shadow Register Selector = "000") */
#define BRPHY2_GPHY_CORE_SHD18_001     0x098f2052 /* 10 Base-T Register (Shadow Register Selector = "001") */
#define BRPHY2_GPHY_CORE_SHD18_010     0x098f2054 /* Power/MII Control Register (Shadow Register Selector = "010") */
#define BRPHY2_GPHY_CORE_SHD18_011     0x098f2056 /* IP Phone Register  (Shadow Register Selector = "011") */
#define BRPHY2_GPHY_CORE_SHD18_100     0x098f2058 /* Misc Test Register  1 (Shadow Register Selector = "100") */
#define BRPHY2_GPHY_CORE_SHD18_101     0x098f205a /* Misc Test Register  2 (Shadow Register Selector = "101") */
#define BRPHY2_GPHY_CORE_SHD18_110     0x098f205c /* Manual IP Phone Seed Register (Shadow Register Selector = "110") */
#define BRPHY2_GPHY_CORE_SHD18_111     0x098f205e /* Miscellanous Control Register (Shadow Register Selector = "111") */
#define BRPHY2_GPHY_CORE_EXP00         0x098f2060 /* Transmit Packet Counter */
#define BRPHY2_GPHY_CORE_EXP01         0x098f2062 /* Expansion Interrupt Status */
#define BRPHY2_GPHY_CORE_EXP02         0x098f2064 /* Expansion Interrupt Mask */
#define BRPHY2_GPHY_CORE_EXP03         0x098f2066 /* Spare Registers */
#define BRPHY2_GPHY_CORE_EXP04         0x098f2068 /* Bicolor LED Selectors */
#define BRPHY2_GPHY_CORE_EXP05         0x098f206a /* Bicolor LED Flash Rate Controls */
#define BRPHY2_GPHY_CORE_EXP06         0x098f206c /* Bicolor LED Programmable Blink Controls */
#define BRPHY2_GPHY_CORE_EXP07         0x098f206e /* Far End Fault */
#define BRPHY2_GPHY_CORE_EXP08         0x098f2070 /* 10BT Controls */
#define BRPHY2_GPHY_CORE_EXP09         0x098f2072 /* AMRR Controls */
#define BRPHY2_GPHY_CORE_EXP0A         0x098f2074 /* DAC TEMPLATE Controls */
#define BRPHY2_GPHY_CORE_EXP0B         0x098f2076 /* External Status */
#define BRPHY2_GPHY_CORE_EXP0C         0x098f2078 /* Spare Registers */
#define BRPHY2_GPHY_CORE_EXP30         0x098f2080 /* Late Collision Counters Status Register */
#define BRPHY2_GPHY_CORE_EXP31         0x098f2082 /* Late Collision Counter [64:95] */
#define BRPHY2_GPHY_CORE_EXP32         0x098f2084 /* Late Collision Counter [96:127] */
#define BRPHY2_GPHY_CORE_EXP33         0x098f2086 /* Late Collision Counter [128:191] */
#define BRPHY2_GPHY_CORE_EXP34         0x098f2088 /* Late Collision Counter [192:319] */
#define BRPHY2_GPHY_CORE_EXP35         0x098f208a /* Late Collision Counter Threshold Register */
#define BRPHY2_GPHY_CORE_EXP36         0x098f208c /* Clock PPM Detection between Recovery and Transmit Clocks */
#define BRPHY2_GPHY_CORE_EXP37         0x098f208e /* Clock PPM Detection between GTX_CLK and Transmit Clocks */
#define BRPHY2_GPHY_CORE_EXP38         0x098f2090 /* IP PHONE Cable Length Status Register */
#define BRPHY2_GPHY_CORE_EXP42         0x098f20a2 /* Operating Mode Status */
#define BRPHY2_GPHY_CORE_EXP5F         0x098f20be /* PLL Frequency Offset Testmode Control */
#define BRPHY2_GPHY_CORE_EXP70         0x098f20e0 /* SOFT-RESET */
#define BRPHY2_GPHY_CORE_EXP71         0x098f20e2 /* Serial LED Control 1 */
#define BRPHY2_GPHY_CORE_EXP72         0x098f20e4 /* Serial LED Control 2 */
#define BRPHY2_GPHY_CORE_EXP73         0x098f20e6 /* LED Gating 2 (Used for dual-media applications) */
#define BRPHY2_GPHY_CORE_EXP74         0x098f20e8 /* LED Programmable Current Mode Control */
#define BRPHY2_GPHY_CORE_EXP75         0x098f20ea /* CED LED Error Mask */
#define BRPHY2_GPHY_CORE_EXP78         0x098f20f0 /* Misc Extended Control */
#define BRPHY2_GPHY_CORE_EXP7B         0x098f20f6 /* I2C Control */
#define BRPHY2_GPHY_CORE_EXP7C         0x098f20f8 /* I2C Status */
#define BRPHY2_GPHY_CORE_EXP7F         0x098f20fe /* External MACSec Interface Control */
#define BRPHY2_GPHY_CORE_ALIAS_18      0x098f2100 /* Alias to MII Reg 18 */
#define BRPHY2_GPHY_CORE_ALIAS_19      0x098f2102 /* Alias to MII Reg 19 */
#define BRPHY2_GPHY_CORE_ALIAS_1A      0x098f2104 /* Alias to MII Reg 1a */
#define BRPHY2_GPHY_CORE_ALIAS_1B      0x098f2106 /* Alias to MII Reg 1b */
#define BRPHY2_GPHY_CORE_ALIAS_1C      0x098f2108 /* Alias to MII Reg 1c */
#define BRPHY2_GPHY_CORE_ALIAS_1D      0x098f210a /* Alias to MII Reg 1d */
#define BRPHY2_GPHY_CORE_REG_MAP_CTL   0x098f210e /* MII Registers 10-1D mapping control */
#define BRPHY2_GPHY_CORE_EXP98         0x098f2130 /* First Slice of Quad-GPHY only): CAL-BIAS Status */
#define BRPHY2_GPHY_CORE_EXP9C         0x098f2138 /* SMII Control */
#define BRPHY2_GPHY_CORE_BT_LINK_FIX   0x098f214a /* 10BT LINK FIX Register */
#define BRPHY2_GPHY_CORE_SYNCE_PLUS_DBG 0x098f214c /* SyncE+ Debug */
#define BRPHY2_GPHY_CORE_SYNCE_PLUS    0x098f214e /* SyncE+ Status and Control */
#define BRPHY2_GPHY_CORE_EXPA8         0x098f2150 /* ADAPTIVE BIAS CONTROL */
#define BRPHY2_GPHY_CORE_EXPA9         0x098f2152 /* spare register */
#define BRPHY2_GPHY_CORE_EXPAA         0x098f2154 /* EEE Statistic timer 12hours lpi */
#define BRPHY2_GPHY_CORE_EXPAB         0x098f2156 /* EEE Statistic timer 12hours local */
#define BRPHY2_GPHY_CORE_EXPAC         0x098f2158 /* EEE Statistic loc lpi req 0_to_1 counter */
#define BRPHY2_GPHY_CORE_EXPAD         0x098f215a /* EEE Statistic rem lpi_req 0_to_1 counter */
#define BRPHY2_GPHY_CORE_EXPAE         0x098f215c /* spare register */
#define BRPHY2_GPHY_CORE_EXPAF         0x098f215e /* EEE Statistic counters ctrl/status */
#define BRPHY2_GPHY_CORE_EXPB0         0x098f2160 /* Bias Control 0 */
#define BRPHY2_GPHY_CORE_EXPB1         0x098f2162 /* Bias Control 1 */
#define BRPHY2_GPHY_CORE_EXPB2         0x098f2164 /* Bias Control 2 */
#define BRPHY2_GPHY_CORE_EXPE3         0x098f2166 /* TX PCS Delay 1000BT and 100TX (copper side) */
#define BRPHY2_GPHY_CORE_EXPE4         0x098f2168 /* TX PCS Delay 10BT (copper side) */
#define BRPHY2_GPHY_CORE_EXPE5         0x098f216a /* TX PCS Delay (fiber side) */
#define BRPHY2_GPHY_CORE_EXPE6         0x098f216c /* RX PCS Delay 1000BT and 100TX (copper side) */
#define BRPHY2_GPHY_CORE_EXPE7         0x098f216e /* RX PCS Delay 10BT (copper side) */
#define BRPHY2_GPHY_CORE_EXPE8         0x098f2170 /* RX PCS Delay (fiber side) */
#define BRPHY2_GPHY_CORE_EXPE9         0x098f2172 /* P1588 TX/RX Cycle Delay */
#define BRPHY2_GPHY_CORE_EXPE0         0x098f2174 /* TX PMA/PMD Delay (copper side) */
#define BRPHY2_GPHY_CORE_EXPE1         0x098f2176 /* TX PMA/PMD Delay (fiber side) */
#define BRPHY2_GPHY_CORE_EXPE2         0x098f2178 /* RX PMA/PMD Delay (copper side) */
#define BRPHY2_GPHY_CORE_EXPEA         0x098f217a /* TX/RX Adjustable Cycle Delay */
#define BRPHY2_GPHY_CORE_LED_PRA_MODE  0x098f2180 /* LED Proportional Rate Activity Control */
#define BRPHY2_GPHY_CORE_FIFO_CTL      0x098f2182 /* FIFO Control Register */
#define BRPHY2_GPHY_CORE_EXPD8         0x098f21b0 /* Halting agc/enc ctrl reg */
#define BRPHY2_GPHY_CORE_EXPF0         0x098f21e0 /* RGMII IBS Control */
#define BRPHY2_GPHY_CORE_EXPF5         0x098f21ea /* Time Sync */
#define BRPHY2_GPHY_CORE_EXPF6         0x098f21ec /* Analog Power Control Status */
#define BRPHY2_GPHY_CORE_EXPF7         0x098f21ee /* Auto-power Down Control Status */
#define BRPHY2_GPHY_CORE_EXPF8         0x098f21f0 /* Trim Settings from Fuse & to Bias Block */
#define BRPHY2_GPHY_CORE_EXPF9         0x098f21f2 /* reserved Register Bits */
#define BRPHY2_GPHY_CORE_EXPFA         0x098f21f4 /* Hidden Identifier */
#define BRPHY2_GPHY_CORE_EXPFB         0x098f21f6 /* TDR Override Values */
#define BRPHY2_GPHY_CORE_EXPFC         0x098f21f8 /*  */
#define BRPHY2_GPHY_CORE_EXPFD         0x098f21fa /* Clock gating control override value */
#define BRPHY2_GPHY_CORE_EXPFE         0x098f21fc /* Clock gating control override enable */
#define BRPHY2_GPHY_CORE_EXPFF         0x098f21fe /* Analog power control override */


/****************************************************************************
 * bcm89530_top_bridge_brphys_brphys2_BRPHY2_DSP_TAP
 ***************************************************************************/
#define BRPHY2_DSP_TAP_TAP0_C0         0x098f2200 /* AGC Control/Status Register A (x4) */
#define BRPHY2_DSP_TAP_TAP0_C1         0x098f2202 /* AGC Control/Status Register A (x4) */
#define BRPHY2_DSP_TAP_TAP0_C2         0x098f2204 /* AGC Control/Status Register A (x4) */
#define BRPHY2_DSP_TAP_TAP0_C3         0x098f2206 /* AGC Control/Status Register A (x4) */
#define BRPHY2_DSP_TAP_TAP1            0x098f2208 /* IPRF Control register (x1) */
#define BRPHY2_DSP_TAP_TAP2_C0         0x098f2210 /* MSE Status Register (x4) */
#define BRPHY2_DSP_TAP_TAP2_C1         0x098f2212 /* MSE Status Register (x4) */
#define BRPHY2_DSP_TAP_TAP2_C2         0x098f2214 /* MSE Status Register (x4) */
#define BRPHY2_DSP_TAP_TAP2_C3         0x098f2216 /* MSE Status Register (x4) */
#define BRPHY2_DSP_TAP_TAP3_C0         0x098f2218 /* Soft Decision (From DFE) Status Register (x4) */
#define BRPHY2_DSP_TAP_TAP3_C1         0x098f221a /* Soft Decision (From DFE) Status Register (x4) */
#define BRPHY2_DSP_TAP_TAP3_C2         0x098f221c /* Soft Decision (From DFE) Status Register (x4) */
#define BRPHY2_DSP_TAP_TAP3_C3         0x098f221e /* Soft Decision (From DFE) Status Register (x4) */
#define BRPHY2_DSP_TAP_TAP4_C0         0x098f2220 /* Phase Status Register and Coarse AGC High Gear (x4) */
#define BRPHY2_DSP_TAP_TAP4_C1         0x098f2222 /* Phase Status Register and Coarse AGC High Gear (x4) */
#define BRPHY2_DSP_TAP_TAP4_C2         0x098f2224 /* Phase Status Register and Coarse AGC High Gear (x4) */
#define BRPHY2_DSP_TAP_TAP4_C3         0x098f2226 /* Phase Status Register and Coarse AGC High Gear (x4) */
#define BRPHY2_DSP_TAP_TAP5_C0         0x098f2228 /* Wire Map/Skew Control/Status and Echo/NEXT Control and Transmitter Control and ADC Control Register and Slicer Zero (x4) */
#define BRPHY2_DSP_TAP_TAP5_C1         0x098f222a /* Wire Map/Skew Control/Status and Echo/NEXT Control and Transmitter Control and ADC Control Register and Slicer Zero (x4) */
#define BRPHY2_DSP_TAP_TAP5_C2         0x098f222c /* Wire Map/Skew Control/Status and Echo/NEXT Control and Transmitter Control and ADC Control Register and Slicer Zero (x4) */
#define BRPHY2_DSP_TAP_TAP5_C3         0x098f222e /* Wire Map/Skew Control/Status and Echo/NEXT Control and Transmitter Control and ADC Control Register and Slicer Zero (x4) */
#define BRPHY2_DSP_TAP_TAP6            0x098f2230 /* CFC Deadman Disable */
#define BRPHY2_DSP_TAP_TAP7_C0         0x098f2238 /* BIST TEST 0 */
#define BRPHY2_DSP_TAP_TAP7_C1         0x098f223a /* BIST TEST 1 */
#define BRPHY2_DSP_TAP_TAP7_C2         0x098f223c /* BIST TEST 2 */
#define BRPHY2_DSP_TAP_TAP8_C0         0x098f2240 /* ABIST TEST 0 */
#define BRPHY2_DSP_TAP_TAP8_C1         0x098f2242 /* ABIST TEST 1 */
#define BRPHY2_DSP_TAP_TAP8_C2         0x098f2244 /* ABIST TEST 2 */
#define BRPHY2_DSP_TAP_TAP8_C3         0x098f2246 /* BR HPF Control */
#define BRPHY2_DSP_TAP_TAP9            0x098f2248 /* Frequency Control/Status Register LSBs (x1) */
#define BRPHY2_DSP_TAP_TAP10           0x098f224a /* PLL Bandwidth Control/Status and Path Metric Reset Register (x1) */
#define BRPHY2_DSP_TAP_TAP11           0x098f224c /* PLL RCLK and TCLK Offset Freeze Register (x1) */
#define BRPHY2_DSP_TAP_TAP12_C0        0x098f2250 /* PLL ACLK Offset/Freeze Register (x4) */
#define BRPHY2_DSP_TAP_TAP12_C1        0x098f2252 /* PLL ACLK Offset/Freeze Register (x4) */
#define BRPHY2_DSP_TAP_TAP12_C2        0x098f2254 /* PLL ACLK Offset/Freeze Register (x4) */
#define BRPHY2_DSP_TAP_TAP12_C3        0x098f2256 /* PLL ACLK Offset/Freeze Register (x4) */
#define BRPHY2_DSP_TAP_TAP13           0x098f2258 /* HPF Bandwidth Control and Disable ADC LSBs (x1) */
#define BRPHY2_DSP_TAP_TAP14           0x098f225a /* MSE Threshold Register  #1 (x1) */
#define BRPHY2_DSP_TAP_TAP15           0x098f225c /* MSE Threshold Register #2 (x1) */
#define BRPHY2_DSP_TAP_TAP16_C0        0x098f2260 /* Logic Analyzer trigger delay (x1) */
#define BRPHY2_DSP_TAP_TAP16_C1        0x098f2262 /* BIST CRC Monitor (x4) */
#define BRPHY2_DSP_TAP_TAP16_C2        0x098f2264 /* BIST CRC Monitor (x4) */
#define BRPHY2_DSP_TAP_TAP16_C3        0x098f2266 /* BIST CRC Monitor (x4) */
#define BRPHY2_DSP_TAP_TAP17_C0        0x098f2268 /* Testmode testvalue (aliased with logic analyzer state selects) */
#define BRPHY2_DSP_TAP_TAP17_C1        0x098f226a /* Testmode and logic analyzer controls #1 */
#define BRPHY2_DSP_TAP_TAP17_C2        0x098f226c /* Logic analyzer controls #2 */
#define BRPHY2_DSP_TAP_TAP17_C3        0x098f226e /* Testmode and logic analyzer controls #3 */
#define BRPHY2_DSP_TAP_TAP18_C0        0x098f2270 /* Peak Noise detector (x4) */
#define BRPHY2_DSP_TAP_TAP18_C1        0x098f2272 /* Peak Noise detector (x4) */
#define BRPHY2_DSP_TAP_TAP18_C2        0x098f2274 /* Peak Noise detector (x4) */
#define BRPHY2_DSP_TAP_TAP18_C3        0x098f2276 /* Peak Noise detector (x4) */
#define BRPHY2_DSP_TAP_TAP20           0x098f2278 /* Echo Minimum Length and LMS/FIR delay adjustments (x1) */
#define BRPHY2_DSP_TAP_TAP21           0x098f227a /* Phy Control Monitors #1 (x1) */
#define BRPHY2_DSP_TAP_TAP22           0x098f227c /* Phy Control Monitors #2 (x1) */
#define BRPHY2_DSP_TAP_TAP23           0x098f227e /* Phy Control Monitors #3 (x1) */
#define BRPHY2_DSP_TAP_TAP24           0x098f2280 /* Phy Control Output Overrides #1 (x1) */
#define BRPHY2_DSP_TAP_TAP25           0x098f2282 /* Phy Control Output Overrides #2 (x1) */
#define BRPHY2_DSP_TAP_TAP26           0x098f2284 /* Phy Control Input Overrides #1 (x1) */
#define BRPHY2_DSP_TAP_TAP27           0x098f2286 /* Phy Control Input Overrides #2 (x1) */
#define BRPHY2_DSP_TAP_TAP28           0x098f2288 /* Phy Control Output Overrides #3 (x1) */
#define BRPHY2_DSP_TAP_TAP29           0x098f228a /* Phy Control Force State/Timers/Alternate Behaviour Register #1 (x1) */
#define BRPHY2_DSP_TAP_TAP30           0x098f228c /* Phy Control Force State/Timers/Alternate Behaviour Register #2 (x1) */
#define BRPHY2_DSP_TAP_TAP31_C0        0x098f2290 /* Channel Swap Override */
#define BRPHY2_DSP_TAP_TAP32_C0        0x098f2298 /* Transmit Testmode Sync Generation (x1) */
#define BRPHY2_DSP_TAP_FDFE_OV_RD      0x098f229a /* FDFE Override/Read Control Register */
#define BRPHY2_DSP_TAP_FDFE_COEFF      0x098f229c /* FDFE Coefficient Read Back Register */
#define BRPHY2_DSP_TAP_FDFE_BETA_THRESHOLD 0x098f229e /* FDFE Beta Threshold Control */
#define BRPHY2_DSP_TAP_TAP33_C0        0x098f22a0 /* eee dsp test */
#define BRPHY2_DSP_TAP_TAP33_C1        0x098f22a2 /* eee sigdet */
#define BRPHY2_DSP_TAP_TAP33_C2        0x098f22a4 /* eee_lpi_timers */
#define BRPHY2_DSP_TAP_TAP33_C3        0x098f22a6 /* spare register */
#define BRPHY2_DSP_TAP_TAP34_C0        0x098f22a8 /* eee frequency control */
#define BRPHY2_DSP_TAP_TAP34_C1        0x098f22aa /* eee Gigabit Mode BW control */
#define BRPHY2_DSP_TAP_TAP34_C2        0x098f22ac /* eee 100TX Mode BW control */
#define BRPHY2_DSP_TAP_TAP34_C3        0x098f22ae /* phasectl TPO monitor */
#define BRPHY2_DSP_TAP_TAP35_C0        0x098f22b0 /* eee 100Base-tx timer control 1 */
#define BRPHY2_DSP_TAP_TAP35_C1        0x098f22b2 /* eee 100Base-tx timer control 2 */
#define BRPHY2_DSP_TAP_TAP35_C2        0x098f22b4 /* eee 100Base-tx timer misc control */
#define BRPHY2_DSP_TAP_TAP35_C3        0x098f22b6 /* pcs_lpi_test */
#define BRPHY2_DSP_TAP_FILTER_CTL_CH0  0x098f22b8 /* Filter Freeze/Disable per channel Control */
#define BRPHY2_DSP_TAP_FILTER_CTL_CH1  0x098f22ba /* Filter Freeze/Disable per channel Control */
#define BRPHY2_DSP_TAP_FILTER_CTL_CH2  0x098f22bc /* Filter Freeze/Disable per channel Control */
#define BRPHY2_DSP_TAP_FILTER_CTL_CH3  0x098f22be /* Filter Freeze/Disable per channel Control */
#define BRPHY2_DSP_TAP_FILTER_CTL_FFEX_CH0 0x098f22c0 /* FFEX Freeze/Disable per channel Control */
#define BRPHY2_DSP_TAP_FILTER_CTL_FFEX_CH1 0x098f22c2 /* FFEX Freeze/Disable per channel Control */
#define BRPHY2_DSP_TAP_FILTER_CTL_FFEX_CH2 0x098f22c4 /* FFEX Freeze/Disable per channel Control */
#define BRPHY2_DSP_TAP_FILTER_CTL_FFEX_CH3 0x098f22c6 /* FFEX Freeze/Disable per channel Control */
#define BRPHY2_DSP_TAP_EMI_DATAPATH_CTL 0x098f22c8 /* EMI Datapath Control */
#define BRPHY2_DSP_TAP_EMI_DATAPATH_CTL2 0x098f22ca /* EMI Datapath Control2 */
#define BRPHY2_DSP_TAP_FFEX_CTL        0x098f22cc /* FFE X-tap Control */
#define BRPHY2_DSP_TAP_PHYC_BREAKPOINT_CTL0 0x098f22ce /* Phycontrol Breakpoint Control 0 */
#define BRPHY2_DSP_TAP_PHYC_BREAKPOINT_CTL1 0x098f22d0 /* Phycontrol Breakpoint Control 1 */
#define BRPHY2_DSP_TAP_FILTER_ADDR     0x098f2360 /* DSP Coefficient Address Register */
#define BRPHY2_DSP_TAP_FILTER_CTL      0x098f2362 /* DSP Control Register */
#define BRPHY2_DSP_TAP_FILTER_DATA     0x098f2364 /* DSP Coefficient Read/Write Port */


/****************************************************************************
 * bcm89530_top_bridge_brphys_brphys2_BRPHY2_PLL_CTRL
 ***************************************************************************/
#define BRPHY2_PLL_CTRL_PLLCTRL_0      0x098f2390 /* Analog pll control 0 */
#define BRPHY2_PLL_CTRL_PLLCTRL_1      0x098f2392 /* Analog pll control 1 */
#define BRPHY2_PLL_CTRL_PLLCTRL_2      0x098f2394 /* Analog pll control 2 */
#define BRPHY2_PLL_CTRL_PLLCTRL_3      0x098f2396 /* Analog pll control 3 */
#define BRPHY2_PLL_CTRL_PLLCTRL_4      0x098f2398 /* Analog pll control 4 */
#define BRPHY2_PLL_CTRL_PLLCTRL_5      0x098f239a /* Analog pll control 5 */
#define BRPHY2_PLL_CTRL_PLLCTRL_6      0x098f239c /* Analog pll control 6 */
#define BRPHY2_PLL_CTRL_PLL_STATUS_0   0x098f23a0 /* Analog PLL Status 0 */
#define BRPHY2_PLL_CTRL_PLL_STATUS_1   0x098f23a2 /* Analog PLL Status 1 */
#define BRPHY2_PLL_CTRL_AFE_SIGDET_STATUS 0x098f23a4 /* AFE Signal detect */
#define BRPHY2_PLL_CTRL_PLLCTRL_7      0x098f23a6 /* Analog pll control 7 */
#define BRPHY2_PLL_CTRL_PLLCTRL_8      0x098f23a8 /* Analog pll control 8 */


/****************************************************************************
 * bcm89530_top_bridge_brphys_brphys2_BRPHY2_AFE_CTRL
 ***************************************************************************/
#define BRPHY2_AFE_CTRL_RXCONFIG_0     0x098f23c0 /* RXCONFIG 15:0 */
#define BRPHY2_AFE_CTRL_RXCONFIG_1     0x098f23c2 /* RXCONFIG 31:16 */
#define BRPHY2_AFE_CTRL_RXCONFIG_2     0x098f23c4 /* RXCONFIG 47:32 */
#define BRPHY2_AFE_CTRL_RXCONFIG_3     0x098f23c6 /* RXCONFIG 63:48 */
#define BRPHY2_AFE_CTRL_RXCONFIG_4     0x098f23c8 /* RXCONFIG 79:64 */
#define BRPHY2_AFE_CTRL_RXCONFIG5_LP   0x098f23ca /* RXCONFIG 86:80 and LP tuning */
#define BRPHY2_AFE_CTRL_TX_CONFIG_0    0x098f23cc /* TXCONFIG 15:0 */
#define BRPHY2_AFE_CTRL_TX_CONFIG_1    0x098f23ce /* TXCONFIG 31:16 */
#define BRPHY2_AFE_CTRL_VDAC_ICTRL_0   0x098f23d0 /* VDAC CURRENT Control 15:0 */
#define BRPHY2_AFE_CTRL_VDAC_ICTRL_1   0x098f23d2 /* VDAC CURRENT Control 31:16 */
#define BRPHY2_AFE_CTRL_VDAC_ICTRL_2   0x098f23d4 /* VDAC CURRENT Control 51:36 */
#define BRPHY2_AFE_CTRL_VDAC_OTHERS_0  0x098f23d6 /* VDAC CURRENT 35:32 and others */
#define BRPHY2_AFE_CTRL_HPF_TRIM_OTHERS 0x098f23d8 /* HPF trim and reserved bits */
#define BRPHY2_AFE_CTRL_TX_EXTRA_CONFIG_0 0x098f23da /* TXCONFIG 15:0 */
#define BRPHY2_AFE_CTRL_TX_EXTRA_CONFIG_1 0x098f23dc /* TXCONFIG 15:0 */
#define BRPHY2_AFE_CTRL_TX_EXTRA_CONFIG_2 0x098f23de /* TXCONFIG 15:0 */
#define BRPHY2_AFE_CTRL_TEMPSEN_OTHERS 0x098f23e0 /* TEMPSEN_OTHERS 15:0 */
#define BRPHY2_AFE_CTRL_FUTURE_RSV     0x098f23e2 /* FUTURE_RSV 15:0 */


/****************************************************************************
 * bcm89530_top_bridge_brphys_brphys2_BRPHY2_ECD_CTRL
 ***************************************************************************/
#define BRPHY2_ECD_CTRL_EXPC0          0x098f2540 /* ECD Control and Status */
#define BRPHY2_ECD_CTRL_EXPC1          0x098f2542 /* ECD Fault Type */
#define BRPHY2_ECD_CTRL_EXPC2          0x098f2544 /* ECD Pair A Length Results */
#define BRPHY2_ECD_CTRL_EXPC3          0x098f2546 /* ECD Pair B Length Results */
#define BRPHY2_ECD_CTRL_EXPC4          0x098f2548 /* ECD Pair C Length Results */
#define BRPHY2_ECD_CTRL_EXPC5          0x098f254a /* ECD Pair D Length Results */
#define BRPHY2_ECD_CTRL_EXPC6          0x098f254c /* ECD XTALK Map */
#define BRPHY2_ECD_CTRL_EXPC7          0x098f254e /* ECD EXTRA RESERVED REGISTER */
#define BRPHY2_ECD_CTRL_EXPC8          0x098f2550 /* ECD EXTRA RESERVED REGISTER */
#define BRPHY2_ECD_CTRL_EXPC9          0x098f2552 /* ECD EXTRA RESERVED REGISTER */
#define BRPHY2_ECD_CTRL_EXPCA          0x098f2554 /* ECD EXTRA RESERVED REGISTER */
#define BRPHY2_ECD_CTRL_EXPCB          0x098f2556 /* ECD EXTRA RESERVED REGISTER */
#define BRPHY2_ECD_CTRL_EXPCC          0x098f2558 /* ECD EXTRA RESERVED REGISTER */
#define BRPHY2_ECD_CTRL_EXPCD          0x098f255a /* ECD EXTRA RESERVED REGISTER */
#define BRPHY2_ECD_CTRL_EXPCE          0x098f255c /* ECD EXTRA RESERVED REGISTER */
#define BRPHY2_ECD_CTRL_EXPCF          0x098f255e /* ECD EXTRA RESERVED REGISTER */
#define BRPHY2_ECD_CTRL_EXPE0          0x098f2560 /* ECD EXTRA RESERVED REGISTER */
#define BRPHY2_ECD_CTRL_EXPE1          0x098f2562 /* ECD EXTRA RESERVED REGISTER */
#define BRPHY2_ECD_CTRL_EXPE2          0x098f2564 /* ECD EXTRA RESERVED REGISTER */
#define BRPHY2_ECD_CTRL_EXPE3          0x098f2566 /* ECD EXTRA RESERVED REGISTER */
#define BRPHY2_ECD_CTRL_EXPE4          0x098f2568 /* ECD EXTRA RESERVED REGISTER */
#define BRPHY2_ECD_CTRL_EXPE5          0x098f256a /* ECD EXTRA RESERVED REGISTER */
#define BRPHY2_ECD_CTRL_EXPE6          0x098f256c /* ECD EXTRA RESERVED REGISTER */
#define BRPHY2_ECD_CTRL_EXPE7          0x098f256e /* ECD EXTRA RESERVED REGISTER */
#define BRPHY2_ECD_CTRL_EXPE8          0x098f2570 /* ECD EXTRA RESERVED REGISTER */
#define BRPHY2_ECD_CTRL_EXPE9          0x098f2572 /* ECD EXTRA RESERVED REGISTER */
#define BRPHY2_ECD_CTRL_EXPEA          0x098f2574 /* ECD EXTRA RESERVED REGISTER */
#define BRPHY2_ECD_CTRL_EXPEB          0x098f2576 /* ECD EXTRA RESERVED REGISTER */
#define BRPHY2_ECD_CTRL_EXPEC          0x098f2578 /* ECD EXTRA RESERVED REGISTER */
#define BRPHY2_ECD_CTRL_EXPED          0x098f257a /* ECD EXTRA RESERVED REGISTER */
#define BRPHY2_ECD_CTRL_EXPEE          0x098f257c /* ECD EXTRA RESERVED REGISTER */
#define BRPHY2_ECD_CTRL_EXPEF          0x098f257e /* ECD EXTRA RESERVED REGISTER */


/****************************************************************************
 * bcm89530_top_bridge_brphys_brphys2_BRPHY2_BR_CTRL
 ***************************************************************************/
#define BRPHY2_BR_CTRL_EXP90           0x098f2600 /* BroadReach LRE Misc Control */
#define BRPHY2_BR_CTRL_EXP91           0x098f2602 /* BroadReach LRE Misc Control */
#define BRPHY2_BR_CTRL_EXP92           0x098f2604 /* BroadReach LRE Misc Control */
#define BRPHY2_BR_CTRL_EXP93           0x098f2606 /* BroadReach LDS  Control */
#define BRPHY2_BR_CTRL_EXP94           0x098f2608 /* BroadReach LDS RX Control */
#define BRPHY2_BR_CTRL_EXP95           0x098f260a /* BroadReach LDS RX Control */
#define BRPHY2_BR_CTRL_EXP96           0x098f260c /* BroadReach LDS Scan, ARB and TX Status */
#define BRPHY2_BR_CTRL_EXP97           0x098f260e /* BroadReach LDS Scan, ARB and TX Status */
#define BRPHY2_BR_CTRL_EXP99           0x098f2612 /* BroadReach LDS Timer Control */
#define BRPHY2_BR_CTRL_EXP9A           0x098f2614 /* LDS Status */
#define BRPHY2_BR_CTRL_EXP9B           0x098f2616 /* BroadR-Reach PLL Control */
#define BRPHY2_BR_CTRL_EXP9D           0x098f261a /* EoC Internal Control 1 */
#define BRPHY2_BR_CTRL_EXP9E           0x098f261c /* LDS Length Threshold 0 */
#define BRPHY2_BR_CTRL_EXP9F           0x098f261e /* LDS Length Threshold 1 */
#define BRPHY2_BR_CTRL_EXPA0           0x098f2620 /* HLDS register, LDS extend advertisement register */
#define BRPHY2_BR_CTRL_EXPA1           0x098f2622 /* HLDS register, LDS link partner extend ability register */
#define BRPHY2_BR_CTRL_EXPA2           0x098f2624 /* HLDS Register */
#define BRPHY2_BR_CTRL_BR_MISC_CONTROL_STATUS 0x098f2626 /* Broadreach Misc Status */
#define BRPHY2_BR_CTRL_BR250_CTL       0x098f263c /* BR250 Control */


/****************************************************************************
 * bcm89530_top_bridge_brphys_brphys2_BRPHY2_BR_CL22_IEEE
 ***************************************************************************/
#define BRPHY2_BR_CL22_IEEE_MII_CTRL   0x098fffc0 /* BR_LRE_Control_Register */
#define BRPHY2_BR_CL22_IEEE_MII_STAT   0x098fffc2 /* BR_LRE_Status_Register */
#define BRPHY2_BR_CL22_IEEE_PHY_ID_MSB 0x098fffc4 /* PHY_Identifier_MSB_Register */
#define BRPHY2_BR_CL22_IEEE_PHY_ID_LSB 0x098fffc6 /* PHY_Identifier_LSB_Register */
#define BRPHY2_BR_CL22_IEEE_LDS_ADV_ABILITY_BP 0x098fffc8 /* LDS_Advertised_Ability_Register (Base Page) */
#define BRPHY2_BR_CL22_IEEE_LDS_ADV_CONTROL 0x098fffca /* LDS_Advertised_Control_Register */
#define BRPHY2_BR_CL22_IEEE_LDS_ABILITY 0x098fffcc /* LDS_Ability_Register (Next Page) */
#define BRPHY2_BR_CL22_IEEE_LDS_LP_ABILITY_BP 0x098fffce /* LDS_Link_Partner_Ability_Base_Page_Register (Base Page) */
#define BRPHY2_BR_CL22_IEEE_LDS_LP_MSG_NXTP 0x098fffd0 /* LDS_Link_Partners_Nxt_Pg_Msg_Register (Next Page) */
#define BRPHY2_BR_CL22_IEEE_LDS_LP_ABILITY_NXTP 0x098fffd2 /* LDS_Link_Partner_Ability_Nxt_Pg_Register (Next Page) */
#define BRPHY2_BR_CL22_IEEE_LDS_LDS_EXP 0x098fffd4 /* LDS_Expansion_Register */
#define BRPHY2_BR_CL22_IEEE_LRE_EXTENDED_STAT 0x098fffd6 /* IEEE_Extended_Status_Register */


/****************************************************************************
 * bcm89530_top_bridge_brphys_brphys3_BRPHY3_CL45DEV1
 ***************************************************************************/
#define BRPHY3_CL45DEV1_PMD_IEEE_CTL1  0x09c20000 /* IEEE PMA/PMD CONTROL 1 REGISTER (REG 1.0) */
#define BRPHY3_CL45DEV1_PMD_IEEE_ST1   0x09c20002 /* IEEE PMA/PMD STATUS 1 REGISTER  (REG 1.1) */
#define BRPHY3_CL45DEV1_PMD_IEEE_DEV_ID0 0x09c20004 /* IEEE PMA/PMD DEVICE IDENTIFIER PART 0 (REG 1.2) */
#define BRPHY3_CL45DEV1_PMD_IEEE_DEV_ID1 0x09c20006 /* IEEE PMA/PMD DEVICE IDENTIFIER PART 1 (REG 1.3) */
#define BRPHY3_CL45DEV1_PMD_IEEE_DEV0  0x09c2000a /* DEVICES IN PACKAGE REGISTER PART 0 (REG 1.5) */
#define BRPHY3_CL45DEV1_PMD_IEEE_DEV1  0x09c2000c /* DEVICES IN PACKAGE REGISTER PART 1 (REG 1.6) */
#define BRPHY3_CL45DEV1_PMD_IEEE_PKG_ID0 0x09c2001c /* PMA/PMD PACKAGE IDENTIFIER (REG 1.14) */
#define BRPHY3_CL45DEV1_PMD_IEEE_PKG_ID1 0x09c2001e /* PMA/PMD PACKAGE IDENTIFIER (REG 1.15) */
#define BRPHY3_CL45DEV1_PMD_IEEE_TSYNC_CAP 0x09c20e10 /* TimeSync PMA/PMD capability (REG 1.1800) */
#define BRPHY3_CL45DEV1_PMD_IEEE_TX_TSYNC_MAX_DELAY_LOWER 0x09c20e12 /* Maximum PMA/PMD transmit path data delay, lower (REG 1.1801) */
#define BRPHY3_CL45DEV1_PMD_IEEE_TX_TSYNC_MAX_DELAY_UPPER 0x09c20e14 /* Maximum PMA/PMD transmit path data delay, upper (REG 1.1802) */
#define BRPHY3_CL45DEV1_PMD_IEEE_TX_TSYNC_MIN_DELAY_LOWER 0x09c20e16 /* Minimum PMA/PMD transmit path data delay, lower (REG 1.1803) */
#define BRPHY3_CL45DEV1_PMD_IEEE_TX_TSYNC_MIN_DELAY_UPPER 0x09c20e18 /* Minimum PMA/PMD transmit path data delay, upper (REG 1.1804) */
#define BRPHY3_CL45DEV1_PMD_IEEE_RX_TSYNC_MAX_DELAY_LOWER 0x09c20e1a /* Maximum PMA/PMD receive path data delay, lower (REG 1.1805) */
#define BRPHY3_CL45DEV1_PMD_IEEE_RX_TSYNC_MAX_DELAY_UPPER 0x09c20e1c /* Maximum PMA/PMD receive path data delay, upper (REG 1.1806) */
#define BRPHY3_CL45DEV1_PMD_IEEE_RX_TSYNC_MIN_DELAY_LOWER 0x09c20e1e /* Minimum PMA/PMD receive path data delay, lower (REG 1.1807) */
#define BRPHY3_CL45DEV1_PMD_IEEE_RX_TSYNC_MIN_DELAY_UPPER 0x09c20e20 /* Minimum PMA/PMD receive path data delay, upper (REG 1.1808) */


/****************************************************************************
 * bcm89530_top_bridge_brphys_brphys3_BRPHY3_CL45DEV3
 ***************************************************************************/
#define BRPHY3_CL45DEV3_PCS_IEEE_CTL1  0x09c60000 /* IEEE PCS CONTROL 1 REGISTER (REG 3.0) */
#define BRPHY3_CL45DEV3_PCS_IEEE_ST1   0x09c60002 /* IEEE PCS STATUS 1 REGISTER  (REG 3.1) */
#define BRPHY3_CL45DEV3_PCS_IEEE_DEV_ID0 0x09c60004 /* IEEE PCS DEVICE IDENTIFIER PART 0 (REG 3.2) */
#define BRPHY3_CL45DEV3_PCS_IEEE_DEV_ID1 0x09c60006 /* IEEE PCS DEVICE IDENTIFIER PART 1 (REG 3.3) */
#define BRPHY3_CL45DEV3_PCS_IEEE_DEV0  0x09c6000a /* DEVICES IN PACKAGE REGISTER PART 0 (REG 3.5) */
#define BRPHY3_CL45DEV3_PCS_IEEE_DEV1  0x09c6000c /* DEVICES IN PACKAGE REGISTER PART 1 (REG 3.6) */
#define BRPHY3_CL45DEV3_PCS_IEEE_PKG_ID0 0x09c6001c /* PCS PACKAGE IDENTIFIER (REG 3.14) */
#define BRPHY3_CL45DEV3_PCS_IEEE_PKG_ID1 0x09c6001e /* PCS PACKAGE IDENTIFIER (REG 3.15) */
#define BRPHY3_CL45DEV3_PCS_EEE_CAP    0x09c60028 /* PCS_EEE_CAP(REG 3.20) */
#define BRPHY3_CL45DEV3_PCS_EEE_WAKE_ERR_CNT 0x09c6002c /* PCS_EEE_Wake_Err_Cnt(REG 3.22) */
#define BRPHY3_CL45DEV3_PCS_IEEE_TSYNC_CAP 0x09c60e10 /* TimeSync PCS capability (REG 3.1800) */
#define BRPHY3_CL45DEV3_PCS_IEEE_TX_TSYNC_MAX_DELAY_LOWER 0x09c60e12 /* Maximum PCS transmit path data delay, lower (REG 3.1801) */
#define BRPHY3_CL45DEV3_PCS_IEEE_TX_TSYNC_MAX_DELAY_UPPER 0x09c60e14 /* Maximum PCS transmit path data delay, upper (REG 3.1802) */
#define BRPHY3_CL45DEV3_PCS_IEEE_TX_TSYNC_MIN_DELAY_LOWER 0x09c60e16 /* Minimum PCS transmit path data delay, lower (REG 3.1803) */
#define BRPHY3_CL45DEV3_PCS_IEEE_TX_TSYNC_MIN_DELAY_UPPER 0x09c60e18 /* Minimum PCS transmit path data delay, upper (REG 3.1804) */
#define BRPHY3_CL45DEV3_PCS_IEEE_RX_TSYNC_MAX_DELAY_LOWER 0x09c60e1a /* Maximum PCS receive path data delay, lower (REG 3.1805) */
#define BRPHY3_CL45DEV3_PCS_IEEE_RX_TSYNC_MAX_DELAY_UPPER 0x09c60e1c /* Maximum PCS receive path data delay, upper (REG 3.1806) */
#define BRPHY3_CL45DEV3_PCS_IEEE_RX_TSYNC_MIN_DELAY_LOWER 0x09c60e1e /* Minimum PCS receive path data delay, lower (REG 3.1807) */
#define BRPHY3_CL45DEV3_PCS_IEEE_RX_TSYNC_MIN_DELAY_UPPER 0x09c60e20 /* Minimum PCS receive path data delay, upper (REG 3.1808) */


/****************************************************************************
 * bcm89530_top_bridge_brphys_brphys3_BRPHY3_CL45DEV7
 ***************************************************************************/
#define BRPHY3_CL45DEV7_AN_CTRL        0x09ce0000 /* Auto Neg Extended Next Page Control     (0x0000) (REG 7.0) */
#define BRPHY3_CL45DEV7_AN_STAT        0x09ce0002 /* AN Status (0x0001) (REG 7.1) */
#define BRPHY3_CL45DEV7_AN_DEV_ID_LSB  0x09ce0004 /* Auto Neg Device Identifier Lower 16 bit (0x0002) (REG 7.2) */
#define BRPHY3_CL45DEV7_AN_DEV_ID_MSB  0x09ce0006 /* Auto Neg Device Identifier Upper 16 bit (0x0003) (REG 7.3) */
#define BRPHY3_CL45DEV7_AN_DEV_IN_PKG_LSB 0x09ce000a /* Auto Neg Device In Package Lower 16 bit (0x0005) (REG 7.5) */
#define BRPHY3_CL45DEV7_AN_DEV_IN_PKG_MSB 0x09ce000c /* Auto Neg Device In Package Upper 16 bit (0x0006) (REG 7.6) */
#define BRPHY3_CL45DEV7_AN_DEV_PKG_ID_LSB 0x09ce001c /* Auto Neg Package ID Lower 16 bit(0x000e) (REG 7.14) */
#define BRPHY3_CL45DEV7_AN_DEV_PKG_ID_MSB 0x09ce001e /* Auto Neg Package ID Upper 16 bit(0x000f) (REG 7.15) */
#define BRPHY3_CL45DEV7_AN_AD          0x09ce0020 /* Auto Neg AD(0x0010) (REG 7.16) */
#define BRPHY3_CL45DEV7_AN_LPA         0x09ce0026 /* AN LP base page ability (0x0013) (REG 7.19) */
#define BRPHY3_CL45DEV7_AN_XNPA        0x09ce002c /* AN XNP transmit A (0x0016) (REG 7.22) */
#define BRPHY3_CL45DEV7_AN_XNPB        0x09ce002e /* AN XNP transmit B (0x0017) (REG 7.23) */
#define BRPHY3_CL45DEV7_AN_XNPC        0x09ce0030 /* AN XNP transmit C (0x0018) (REG 7.24) */
#define BRPHY3_CL45DEV7_LP_XNPA        0x09ce0032 /* AN LP XNP ability A (0x0019) (REG 7.25) */
#define BRPHY3_CL45DEV7_LP_XNPB        0x09ce0034 /* AN LP XNP ability B (0x001a) (REG 7.26) */
#define BRPHY3_CL45DEV7_LP_XNPC        0x09ce0036 /* AN LP XNP ability C (0x001b) (REG 7.27) */
#define BRPHY3_CL45DEV7_TENG_AN_CTRL   0x09ce0040 /* 10G Base-T AN Control Register (0x0020) (REG 7.32) */
#define BRPHY3_CL45DEV7_TENG_AN_STAT   0x09ce0042 /* 10G Base-T AN Status Register  (0x0021) (REG 7.33) */
#define BRPHY3_CL45DEV7_EEE_ADV        0x09ce0078 /* EEE Advertisement (0x003C) (REG 7.60 ???) */
#define BRPHY3_CL45DEV7_EEE_LP_ADV     0x09ce007a /* EEE Link Partner Advertisement (0x003D) (REG 7.61 ???) */
#define BRPHY3_CL45DEV7_EEE_MODE_CTL   0x09ce007c /* EEE Mode Control (0x003E) (REG 7.62 ???) */


/****************************************************************************
 * bcm89530_top_bridge_brphys_brphys3_BRPHY3_CL45VEN
 ***************************************************************************/
#define BRPHY3_CL45VEN_FORCE_LINK      0x09cf0000 /* Force Link Register */
#define BRPHY3_CL45VEN_SELECTIVE_RESET 0x09cf0002 /* Selective Reset Register */
#define BRPHY3_CL45VEN_TEST_FSM_EXT_NXT_PGS 0x09cf0004 /* Test State Machine For Extended Next Pages Register --mvadkert */
#define BRPHY3_CL45VEN_TEST_FSM_NXT_PGS 0x09cf0006 /* Test State Machine For Next Pages Register --mvadkert */
#define BRPHY3_CL45VEN_AN_MAN_TEST     0x09cf0032 /* Auto Negotiation Manual Test Register */
#define BRPHY3_CL45VEN_AN_MAN_LNK_PARTNR_ABI_A 0x09cf0034 /* Auto Negotiation Manual Link Partners Abilities Register A */
#define BRPHY3_CL45VEN_AN_MAN_LNK_PARTNR_ABI_B 0x09cf0036 /* Auto Negotiation Manual Link Partners Abilities Register B */
#define BRPHY3_CL45VEN_LNK_PARTNR_NXT_PG_A 0x09cf0038 /* Link Partner Next Page */
#define BRPHY3_CL45VEN_LNK_PARTNR_NXT_PG_B 0x09cf003a /* Link Partner Next Page (cont.) */
#define BRPHY3_CL45VEN_LNK_PARTNR_NXT_PG_C 0x09cf003c /* Link Partner Next Page (cont.) */
#define BRPHY3_CL45VEN_LNK_PARTNR_NXT_PG_D 0x09cf003e /* Link Partner Next Page (cont.) */
#define BRPHY3_CL45VEN_LNK_PARTNR_NXT_PG_E 0x09cf0040 /* Link Partner Next Page (cont.) */
#define BRPHY3_CL45VEN_LNK_PARTNR_NXT_PG_F 0x09cf0042 /* Link Partner Next Page (cont.) */
#define BRPHY3_CL45VEN_EPON_CTRL_REG   0x09cf0046 /* EPON mode control register */
#define BRPHY3_CL45VEN_EEE_TEST_CTRL_A 0x09cf0060 /* EEE Test Control Register A eee_test_control_bus[15:0] */
#define BRPHY3_CL45VEN_EEE_TEST_CTRL_B 0x09cf0062 /* EEE Test Control Register B eee_test_control_bus[31:16] */
#define BRPHY3_CL45VEN_EEE_TEST_CTRL_C 0x09cf0064 /* EEE Test Control Register C eee_test_control_bus[47:32] */
#define BRPHY3_CL45VEN_EEE_SPARE_1     0x09cf0076 /* EEE Spare Register 1 */
#define BRPHY3_CL45VEN_EEE_SPARE_2     0x09cf0078 /* EEE Spare Register 2 */
#define BRPHY3_CL45VEN_EEE_CONTROL     0x09cf007a /* EEE Control Register */
#define BRPHY3_CL45VEN_EEE_RES_STAT    0x09cf007c /* EEE Resolution Status Register */
#define BRPHY3_CL45VEN_LPI_MODE_CNTR   0x09cf007e /* LPI Mode Counter Register */
#define BRPHY3_CL45VEN_LOC_DEV_MSG_5_A 0x09cf0080 /* Local Device Message 5 */
#define BRPHY3_CL45VEN_LOC_DEV_MSG_5_B 0x09cf0082 /* Local Device Message 5 cont. */
#define BRPHY3_CL45VEN_LOC_DEV_MSG_5_C 0x09cf0084 /* Local Device Message 5 cont. */
#define BRPHY3_CL45VEN_LOC_DEV_MSG_5_D 0x09cf0086 /* Local Device Message 5 cont. */
#define BRPHY3_CL45VEN_LNK_PARTNR_MSG_5_A 0x09cf0088 /* Link Partner Message 5 */
#define BRPHY3_CL45VEN_LNK_PARTNR_MSG_5_B 0x09cf008a /* Link Partner Message 5 cont. */
#define BRPHY3_CL45VEN_LNK_PARTNR_MSG_5_C 0x09cf008c /* Link Partner Message 5 cont. */
#define BRPHY3_CL45VEN_LNK_PARTNR_MSG_5_D 0x09cf008e /* Link Partner Message 5 cont. */
#define BRPHY3_CL45VEN_LOC_DEV_MSG_6_A 0x09cf0090 /* Local Device Message 6 */
#define BRPHY3_CL45VEN_LOC_DEV_MSG_6_B 0x09cf0092 /* Local Device Message 6 cont. */
#define BRPHY3_CL45VEN_LOC_DEV_MSG_6_C 0x09cf0094 /* Local Device Message 6 cont. */
#define BRPHY3_CL45VEN_LOC_DEV_MSG_6_D 0x09cf0096 /* Local Device Message 6 cont. */
#define BRPHY3_CL45VEN_LNK_PARTNR_MSG_6_A 0x09cf0098 /* Link Partner Message 6 */
#define BRPHY3_CL45VEN_LNK_PARTNR_MSG_6_B 0x09cf009a /* Link Partner Message 6 cont. */
#define BRPHY3_CL45VEN_LNK_PARTNR_MSG_6_C 0x09cf009c /* Link Partner Message 6 cont. */
#define BRPHY3_CL45VEN_LNK_PARTNR_MSG_6_D 0x09cf009e /* Link Partner Message 6 cont. */


/****************************************************************************
 * bcm89530_top_bridge_brphys_brphys3_BRPHY3_GPHY_CORE
 ***************************************************************************/
#define BRPHY3_GPHY_CORE_BASE10        0x09cf2000 /* PHY_Extended_ctl_Register */
#define BRPHY3_GPHY_CORE_BASE11        0x09cf2002 /* PHY_Extended_Status_Register (copper side only) */
#define BRPHY3_GPHY_CORE_BASE12        0x09cf2004 /* Receive_Error_Cntr_Register */
#define BRPHY3_GPHY_CORE_BASE13        0x09cf2006 /* False_Carrier_Sense_Cntr_Register */
#define BRPHY3_GPHY_CORE_BASE14        0x09cf2008 /* Local_Remote_Receiver_NOT_OK_Cntrs_Register */
#define BRPHY3_GPHY_CORE_EXP45         0x09cf200a /* Pattern Generator Control Register */
#define BRPHY3_GPHY_CORE_EXP46         0x09cf200b /* Pattern Generator Status Register */
#define BRPHY3_GPHY_CORE_BASE19        0x09cf2012 /* Auxiliary Status Summary (copper side only) */
#define BRPHY3_GPHY_CORE_BASE1A        0x09cf2014 /* Interrupt Status Register (copper side only) */
#define BRPHY3_GPHY_CORE_BASE1B        0x09cf2016 /* Interrupt Mask Register */
#define BRPHY3_GPHY_CORE_BASE1D_SHD    0x09cf2018 /* HCD Status Register */
#define BRPHY3_GPHY_CORE_BASE1D        0x09cf201a /* Master/Slave Seed Register */
#define BRPHY3_GPHY_CORE_BASE1E        0x09cf201c /* Test1_Register */
#define BRPHY3_GPHY_CORE_BASE1F        0x09cf201e /* Test2_Register */
#define BRPHY3_GPHY_CORE_SHD1C_00      0x09cf2020 /* Cabletron LED Register (Shadow Register Selector = "00h") */
#define BRPHY3_GPHY_CORE_SHD1C_01      0x09cf2022 /* TVCO Selection Register (Shadow Register Selector = "01h") */
#define BRPHY3_GPHY_CORE_SHD1C_02      0x09cf2024 /* reserved Control 1 Register (Shadow Register Selector = "02h") */
#define BRPHY3_GPHY_CORE_SHD1C_03      0x09cf2026 /* Clock Alignment Control Regsiter (Shadow Register Selector = "03h") */
#define BRPHY3_GPHY_CORE_SHD1C_04      0x09cf2028 /* reserved Control 2 Register (Shadow Register Selector = "04h") */
#define BRPHY3_GPHY_CORE_SHD1C_05      0x09cf202a /* reserved Control 3 Register (Shadow Register Selector = "05h") */
#define BRPHY3_GPHY_CORE_SHD1C_06      0x09cf202c /* Tdr Control 1 Register (Shadow Register Selector = "06h") */
#define BRPHY3_GPHY_CORE_SHD1C_07      0x09cf202e /* Tdr Control 2 Register (Shadow Register Selector = "07h") */
#define BRPHY3_GPHY_CORE_SHD1C_08      0x09cf2030 /* Led Status Register (Shadow Register Selector = "08h") */
#define BRPHY3_GPHY_CORE_SHD1C_09      0x09cf2032 /* Led Control Register (Shadow Register Selector = "09h") */
#define BRPHY3_GPHY_CORE_SHD1C_0A      0x09cf2034 /* Auto-Power Down Register (Shadow Register Selector = "0ah") */
#define BRPHY3_GPHY_CORE_SHD1C_0B      0x09cf2036 /* reserved Control 4 Register (Shadow Register Selector = "0bh") */
#define BRPHY3_GPHY_CORE_SHD1C_0D      0x09cf203a /* LED Selector 1 Register (Shadow Register Selector = "0dh") */
#define BRPHY3_GPHY_CORE_SHD1C_0E      0x09cf203c /* LED Selector 2 Register (Shadow Register Selector = "0eh") */
#define BRPHY3_GPHY_CORE_SHD1C_0F      0x09cf203e /* LED GPIO Control/Status Register (Shadow Register Selector = "0fh") */
#define BRPHY3_GPHY_CORE_SHD1C_10      0x09cf2040 /* Cisco Enhanced Linkstatus Mode Control Register (Shadow Register Selector = "10h") */
#define BRPHY3_GPHY_CORE_SHD1C_1F      0x09cf2042 /* Mode Control Register (Shadow Register Selector = "1fh") */
#define BRPHY3_GPHY_CORE_SHD18_000     0x09cf2050 /* Auxiliary Control Register (Shadow Register Selector = "000") */
#define BRPHY3_GPHY_CORE_SHD18_001     0x09cf2052 /* 10 Base-T Register (Shadow Register Selector = "001") */
#define BRPHY3_GPHY_CORE_SHD18_010     0x09cf2054 /* Power/MII Control Register (Shadow Register Selector = "010") */
#define BRPHY3_GPHY_CORE_SHD18_011     0x09cf2056 /* IP Phone Register  (Shadow Register Selector = "011") */
#define BRPHY3_GPHY_CORE_SHD18_100     0x09cf2058 /* Misc Test Register  1 (Shadow Register Selector = "100") */
#define BRPHY3_GPHY_CORE_SHD18_101     0x09cf205a /* Misc Test Register  2 (Shadow Register Selector = "101") */
#define BRPHY3_GPHY_CORE_SHD18_110     0x09cf205c /* Manual IP Phone Seed Register (Shadow Register Selector = "110") */
#define BRPHY3_GPHY_CORE_SHD18_111     0x09cf205e /* Miscellanous Control Register (Shadow Register Selector = "111") */
#define BRPHY3_GPHY_CORE_EXP00         0x09cf2060 /* Transmit Packet Counter */
#define BRPHY3_GPHY_CORE_EXP01         0x09cf2062 /* Expansion Interrupt Status */
#define BRPHY3_GPHY_CORE_EXP02         0x09cf2064 /* Expansion Interrupt Mask */
#define BRPHY3_GPHY_CORE_EXP03         0x09cf2066 /* Spare Registers */
#define BRPHY3_GPHY_CORE_EXP04         0x09cf2068 /* Bicolor LED Selectors */
#define BRPHY3_GPHY_CORE_EXP05         0x09cf206a /* Bicolor LED Flash Rate Controls */
#define BRPHY3_GPHY_CORE_EXP06         0x09cf206c /* Bicolor LED Programmable Blink Controls */
#define BRPHY3_GPHY_CORE_EXP07         0x09cf206e /* Far End Fault */
#define BRPHY3_GPHY_CORE_EXP08         0x09cf2070 /* 10BT Controls */
#define BRPHY3_GPHY_CORE_EXP09         0x09cf2072 /* AMRR Controls */
#define BRPHY3_GPHY_CORE_EXP0A         0x09cf2074 /* DAC TEMPLATE Controls */
#define BRPHY3_GPHY_CORE_EXP0B         0x09cf2076 /* External Status */
#define BRPHY3_GPHY_CORE_EXP0C         0x09cf2078 /* Spare Registers */
#define BRPHY3_GPHY_CORE_EXP30         0x09cf2080 /* Late Collision Counters Status Register */
#define BRPHY3_GPHY_CORE_EXP31         0x09cf2082 /* Late Collision Counter [64:95] */
#define BRPHY3_GPHY_CORE_EXP32         0x09cf2084 /* Late Collision Counter [96:127] */
#define BRPHY3_GPHY_CORE_EXP33         0x09cf2086 /* Late Collision Counter [128:191] */
#define BRPHY3_GPHY_CORE_EXP34         0x09cf2088 /* Late Collision Counter [192:319] */
#define BRPHY3_GPHY_CORE_EXP35         0x09cf208a /* Late Collision Counter Threshold Register */
#define BRPHY3_GPHY_CORE_EXP36         0x09cf208c /* Clock PPM Detection between Recovery and Transmit Clocks */
#define BRPHY3_GPHY_CORE_EXP37         0x09cf208e /* Clock PPM Detection between GTX_CLK and Transmit Clocks */
#define BRPHY3_GPHY_CORE_EXP38         0x09cf2090 /* IP PHONE Cable Length Status Register */
#define BRPHY3_GPHY_CORE_EXP42         0x09cf20a2 /* Operating Mode Status */
#define BRPHY3_GPHY_CORE_EXP5F         0x09cf20be /* PLL Frequency Offset Testmode Control */
#define BRPHY3_GPHY_CORE_EXP70         0x09cf20e0 /* SOFT-RESET */
#define BRPHY3_GPHY_CORE_EXP71         0x09cf20e2 /* Serial LED Control 1 */
#define BRPHY3_GPHY_CORE_EXP72         0x09cf20e4 /* Serial LED Control 2 */
#define BRPHY3_GPHY_CORE_EXP73         0x09cf20e6 /* LED Gating 2 (Used for dual-media applications) */
#define BRPHY3_GPHY_CORE_EXP74         0x09cf20e8 /* LED Programmable Current Mode Control */
#define BRPHY3_GPHY_CORE_EXP75         0x09cf20ea /* CED LED Error Mask */
#define BRPHY3_GPHY_CORE_EXP78         0x09cf20f0 /* Misc Extended Control */
#define BRPHY3_GPHY_CORE_EXP7B         0x09cf20f6 /* I2C Control */
#define BRPHY3_GPHY_CORE_EXP7C         0x09cf20f8 /* I2C Status */
#define BRPHY3_GPHY_CORE_EXP7F         0x09cf20fe /* External MACSec Interface Control */
#define BRPHY3_GPHY_CORE_ALIAS_18      0x09cf2100 /* Alias to MII Reg 18 */
#define BRPHY3_GPHY_CORE_ALIAS_19      0x09cf2102 /* Alias to MII Reg 19 */
#define BRPHY3_GPHY_CORE_ALIAS_1A      0x09cf2104 /* Alias to MII Reg 1a */
#define BRPHY3_GPHY_CORE_ALIAS_1B      0x09cf2106 /* Alias to MII Reg 1b */
#define BRPHY3_GPHY_CORE_ALIAS_1C      0x09cf2108 /* Alias to MII Reg 1c */
#define BRPHY3_GPHY_CORE_ALIAS_1D      0x09cf210a /* Alias to MII Reg 1d */
#define BRPHY3_GPHY_CORE_REG_MAP_CTL   0x09cf210e /* MII Registers 10-1D mapping control */
#define BRPHY3_GPHY_CORE_EXP98         0x09cf2130 /* First Slice of Quad-GPHY only): CAL-BIAS Status */
#define BRPHY3_GPHY_CORE_EXP9C         0x09cf2138 /* SMII Control */
#define BRPHY3_GPHY_CORE_BT_LINK_FIX   0x09cf214a /* 10BT LINK FIX Register */
#define BRPHY3_GPHY_CORE_SYNCE_PLUS_DBG 0x09cf214c /* SyncE+ Debug */
#define BRPHY3_GPHY_CORE_SYNCE_PLUS    0x09cf214e /* SyncE+ Status and Control */
#define BRPHY3_GPHY_CORE_EXPA8         0x09cf2150 /* ADAPTIVE BIAS CONTROL */
#define BRPHY3_GPHY_CORE_EXPA9         0x09cf2152 /* spare register */
#define BRPHY3_GPHY_CORE_EXPAA         0x09cf2154 /* EEE Statistic timer 12hours lpi */
#define BRPHY3_GPHY_CORE_EXPAB         0x09cf2156 /* EEE Statistic timer 12hours local */
#define BRPHY3_GPHY_CORE_EXPAC         0x09cf2158 /* EEE Statistic loc lpi req 0_to_1 counter */
#define BRPHY3_GPHY_CORE_EXPAD         0x09cf215a /* EEE Statistic rem lpi_req 0_to_1 counter */
#define BRPHY3_GPHY_CORE_EXPAE         0x09cf215c /* spare register */
#define BRPHY3_GPHY_CORE_EXPAF         0x09cf215e /* EEE Statistic counters ctrl/status */
#define BRPHY3_GPHY_CORE_EXPB0         0x09cf2160 /* Bias Control 0 */
#define BRPHY3_GPHY_CORE_EXPB1         0x09cf2162 /* Bias Control 1 */
#define BRPHY3_GPHY_CORE_EXPB2         0x09cf2164 /* Bias Control 2 */
#define BRPHY3_GPHY_CORE_EXPE3         0x09cf2166 /* TX PCS Delay 1000BT and 100TX (copper side) */
#define BRPHY3_GPHY_CORE_EXPE4         0x09cf2168 /* TX PCS Delay 10BT (copper side) */
#define BRPHY3_GPHY_CORE_EXPE5         0x09cf216a /* TX PCS Delay (fiber side) */
#define BRPHY3_GPHY_CORE_EXPE6         0x09cf216c /* RX PCS Delay 1000BT and 100TX (copper side) */
#define BRPHY3_GPHY_CORE_EXPE7         0x09cf216e /* RX PCS Delay 10BT (copper side) */
#define BRPHY3_GPHY_CORE_EXPE8         0x09cf2170 /* RX PCS Delay (fiber side) */
#define BRPHY3_GPHY_CORE_EXPE9         0x09cf2172 /* P1588 TX/RX Cycle Delay */
#define BRPHY3_GPHY_CORE_EXPE0         0x09cf2174 /* TX PMA/PMD Delay (copper side) */
#define BRPHY3_GPHY_CORE_EXPE1         0x09cf2176 /* TX PMA/PMD Delay (fiber side) */
#define BRPHY3_GPHY_CORE_EXPE2         0x09cf2178 /* RX PMA/PMD Delay (copper side) */
#define BRPHY3_GPHY_CORE_EXPEA         0x09cf217a /* TX/RX Adjustable Cycle Delay */
#define BRPHY3_GPHY_CORE_LED_PRA_MODE  0x09cf2180 /* LED Proportional Rate Activity Control */
#define BRPHY3_GPHY_CORE_FIFO_CTL      0x09cf2182 /* FIFO Control Register */
#define BRPHY3_GPHY_CORE_EXPD8         0x09cf21b0 /* Halting agc/enc ctrl reg */
#define BRPHY3_GPHY_CORE_EXPF0         0x09cf21e0 /* RGMII IBS Control */
#define BRPHY3_GPHY_CORE_EXPF5         0x09cf21ea /* Time Sync */
#define BRPHY3_GPHY_CORE_EXPF6         0x09cf21ec /* Analog Power Control Status */
#define BRPHY3_GPHY_CORE_EXPF7         0x09cf21ee /* Auto-power Down Control Status */
#define BRPHY3_GPHY_CORE_EXPF8         0x09cf21f0 /* Trim Settings from Fuse & to Bias Block */
#define BRPHY3_GPHY_CORE_EXPF9         0x09cf21f2 /* reserved Register Bits */
#define BRPHY3_GPHY_CORE_EXPFA         0x09cf21f4 /* Hidden Identifier */
#define BRPHY3_GPHY_CORE_EXPFB         0x09cf21f6 /* TDR Override Values */
#define BRPHY3_GPHY_CORE_EXPFC         0x09cf21f8 /*  */
#define BRPHY3_GPHY_CORE_EXPFD         0x09cf21fa /* Clock gating control override value */
#define BRPHY3_GPHY_CORE_EXPFE         0x09cf21fc /* Clock gating control override enable */
#define BRPHY3_GPHY_CORE_EXPFF         0x09cf21fe /* Analog power control override */


/****************************************************************************
 * bcm89530_top_bridge_brphys_brphys3_BRPHY3_DSP_TAP
 ***************************************************************************/
#define BRPHY3_DSP_TAP_TAP0_C0         0x09cf2200 /* AGC Control/Status Register A (x4) */
#define BRPHY3_DSP_TAP_TAP0_C1         0x09cf2202 /* AGC Control/Status Register A (x4) */
#define BRPHY3_DSP_TAP_TAP0_C2         0x09cf2204 /* AGC Control/Status Register A (x4) */
#define BRPHY3_DSP_TAP_TAP0_C3         0x09cf2206 /* AGC Control/Status Register A (x4) */
#define BRPHY3_DSP_TAP_TAP1            0x09cf2208 /* IPRF Control register (x1) */
#define BRPHY3_DSP_TAP_TAP2_C0         0x09cf2210 /* MSE Status Register (x4) */
#define BRPHY3_DSP_TAP_TAP2_C1         0x09cf2212 /* MSE Status Register (x4) */
#define BRPHY3_DSP_TAP_TAP2_C2         0x09cf2214 /* MSE Status Register (x4) */
#define BRPHY3_DSP_TAP_TAP2_C3         0x09cf2216 /* MSE Status Register (x4) */
#define BRPHY3_DSP_TAP_TAP3_C0         0x09cf2218 /* Soft Decision (From DFE) Status Register (x4) */
#define BRPHY3_DSP_TAP_TAP3_C1         0x09cf221a /* Soft Decision (From DFE) Status Register (x4) */
#define BRPHY3_DSP_TAP_TAP3_C2         0x09cf221c /* Soft Decision (From DFE) Status Register (x4) */
#define BRPHY3_DSP_TAP_TAP3_C3         0x09cf221e /* Soft Decision (From DFE) Status Register (x4) */
#define BRPHY3_DSP_TAP_TAP4_C0         0x09cf2220 /* Phase Status Register and Coarse AGC High Gear (x4) */
#define BRPHY3_DSP_TAP_TAP4_C1         0x09cf2222 /* Phase Status Register and Coarse AGC High Gear (x4) */
#define BRPHY3_DSP_TAP_TAP4_C2         0x09cf2224 /* Phase Status Register and Coarse AGC High Gear (x4) */
#define BRPHY3_DSP_TAP_TAP4_C3         0x09cf2226 /* Phase Status Register and Coarse AGC High Gear (x4) */
#define BRPHY3_DSP_TAP_TAP5_C0         0x09cf2228 /* Wire Map/Skew Control/Status and Echo/NEXT Control and Transmitter Control and ADC Control Register and Slicer Zero (x4) */
#define BRPHY3_DSP_TAP_TAP5_C1         0x09cf222a /* Wire Map/Skew Control/Status and Echo/NEXT Control and Transmitter Control and ADC Control Register and Slicer Zero (x4) */
#define BRPHY3_DSP_TAP_TAP5_C2         0x09cf222c /* Wire Map/Skew Control/Status and Echo/NEXT Control and Transmitter Control and ADC Control Register and Slicer Zero (x4) */
#define BRPHY3_DSP_TAP_TAP5_C3         0x09cf222e /* Wire Map/Skew Control/Status and Echo/NEXT Control and Transmitter Control and ADC Control Register and Slicer Zero (x4) */
#define BRPHY3_DSP_TAP_TAP6            0x09cf2230 /* CFC Deadman Disable */
#define BRPHY3_DSP_TAP_TAP7_C0         0x09cf2238 /* BIST TEST 0 */
#define BRPHY3_DSP_TAP_TAP7_C1         0x09cf223a /* BIST TEST 1 */
#define BRPHY3_DSP_TAP_TAP7_C2         0x09cf223c /* BIST TEST 2 */
#define BRPHY3_DSP_TAP_TAP8_C0         0x09cf2240 /* ABIST TEST 0 */
#define BRPHY3_DSP_TAP_TAP8_C1         0x09cf2242 /* ABIST TEST 1 */
#define BRPHY3_DSP_TAP_TAP8_C2         0x09cf2244 /* ABIST TEST 2 */
#define BRPHY3_DSP_TAP_TAP8_C3         0x09cf2246 /* BR HPF Control */
#define BRPHY3_DSP_TAP_TAP9            0x09cf2248 /* Frequency Control/Status Register LSBs (x1) */
#define BRPHY3_DSP_TAP_TAP10           0x09cf224a /* PLL Bandwidth Control/Status and Path Metric Reset Register (x1) */
#define BRPHY3_DSP_TAP_TAP11           0x09cf224c /* PLL RCLK and TCLK Offset Freeze Register (x1) */
#define BRPHY3_DSP_TAP_TAP12_C0        0x09cf2250 /* PLL ACLK Offset/Freeze Register (x4) */
#define BRPHY3_DSP_TAP_TAP12_C1        0x09cf2252 /* PLL ACLK Offset/Freeze Register (x4) */
#define BRPHY3_DSP_TAP_TAP12_C2        0x09cf2254 /* PLL ACLK Offset/Freeze Register (x4) */
#define BRPHY3_DSP_TAP_TAP12_C3        0x09cf2256 /* PLL ACLK Offset/Freeze Register (x4) */
#define BRPHY3_DSP_TAP_TAP13           0x09cf2258 /* HPF Bandwidth Control and Disable ADC LSBs (x1) */
#define BRPHY3_DSP_TAP_TAP14           0x09cf225a /* MSE Threshold Register  #1 (x1) */
#define BRPHY3_DSP_TAP_TAP15           0x09cf225c /* MSE Threshold Register #2 (x1) */
#define BRPHY3_DSP_TAP_TAP16_C0        0x09cf2260 /* Logic Analyzer trigger delay (x1) */
#define BRPHY3_DSP_TAP_TAP16_C1        0x09cf2262 /* BIST CRC Monitor (x4) */
#define BRPHY3_DSP_TAP_TAP16_C2        0x09cf2264 /* BIST CRC Monitor (x4) */
#define BRPHY3_DSP_TAP_TAP16_C3        0x09cf2266 /* BIST CRC Monitor (x4) */
#define BRPHY3_DSP_TAP_TAP17_C0        0x09cf2268 /* Testmode testvalue (aliased with logic analyzer state selects) */
#define BRPHY3_DSP_TAP_TAP17_C1        0x09cf226a /* Testmode and logic analyzer controls #1 */
#define BRPHY3_DSP_TAP_TAP17_C2        0x09cf226c /* Logic analyzer controls #2 */
#define BRPHY3_DSP_TAP_TAP17_C3        0x09cf226e /* Testmode and logic analyzer controls #3 */
#define BRPHY3_DSP_TAP_TAP18_C0        0x09cf2270 /* Peak Noise detector (x4) */
#define BRPHY3_DSP_TAP_TAP18_C1        0x09cf2272 /* Peak Noise detector (x4) */
#define BRPHY3_DSP_TAP_TAP18_C2        0x09cf2274 /* Peak Noise detector (x4) */
#define BRPHY3_DSP_TAP_TAP18_C3        0x09cf2276 /* Peak Noise detector (x4) */
#define BRPHY3_DSP_TAP_TAP20           0x09cf2278 /* Echo Minimum Length and LMS/FIR delay adjustments (x1) */
#define BRPHY3_DSP_TAP_TAP21           0x09cf227a /* Phy Control Monitors #1 (x1) */
#define BRPHY3_DSP_TAP_TAP22           0x09cf227c /* Phy Control Monitors #2 (x1) */
#define BRPHY3_DSP_TAP_TAP23           0x09cf227e /* Phy Control Monitors #3 (x1) */
#define BRPHY3_DSP_TAP_TAP24           0x09cf2280 /* Phy Control Output Overrides #1 (x1) */
#define BRPHY3_DSP_TAP_TAP25           0x09cf2282 /* Phy Control Output Overrides #2 (x1) */
#define BRPHY3_DSP_TAP_TAP26           0x09cf2284 /* Phy Control Input Overrides #1 (x1) */
#define BRPHY3_DSP_TAP_TAP27           0x09cf2286 /* Phy Control Input Overrides #2 (x1) */
#define BRPHY3_DSP_TAP_TAP28           0x09cf2288 /* Phy Control Output Overrides #3 (x1) */
#define BRPHY3_DSP_TAP_TAP29           0x09cf228a /* Phy Control Force State/Timers/Alternate Behaviour Register #1 (x1) */
#define BRPHY3_DSP_TAP_TAP30           0x09cf228c /* Phy Control Force State/Timers/Alternate Behaviour Register #2 (x1) */
#define BRPHY3_DSP_TAP_TAP31_C0        0x09cf2290 /* Channel Swap Override */
#define BRPHY3_DSP_TAP_TAP32_C0        0x09cf2298 /* Transmit Testmode Sync Generation (x1) */
#define BRPHY3_DSP_TAP_FDFE_OV_RD      0x09cf229a /* FDFE Override/Read Control Register */
#define BRPHY3_DSP_TAP_FDFE_COEFF      0x09cf229c /* FDFE Coefficient Read Back Register */
#define BRPHY3_DSP_TAP_FDFE_BETA_THRESHOLD 0x09cf229e /* FDFE Beta Threshold Control */
#define BRPHY3_DSP_TAP_TAP33_C0        0x09cf22a0 /* eee dsp test */
#define BRPHY3_DSP_TAP_TAP33_C1        0x09cf22a2 /* eee sigdet */
#define BRPHY3_DSP_TAP_TAP33_C2        0x09cf22a4 /* eee_lpi_timers */
#define BRPHY3_DSP_TAP_TAP33_C3        0x09cf22a6 /* spare register */
#define BRPHY3_DSP_TAP_TAP34_C0        0x09cf22a8 /* eee frequency control */
#define BRPHY3_DSP_TAP_TAP34_C1        0x09cf22aa /* eee Gigabit Mode BW control */
#define BRPHY3_DSP_TAP_TAP34_C2        0x09cf22ac /* eee 100TX Mode BW control */
#define BRPHY3_DSP_TAP_TAP34_C3        0x09cf22ae /* phasectl TPO monitor */
#define BRPHY3_DSP_TAP_TAP35_C0        0x09cf22b0 /* eee 100Base-tx timer control 1 */
#define BRPHY3_DSP_TAP_TAP35_C1        0x09cf22b2 /* eee 100Base-tx timer control 2 */
#define BRPHY3_DSP_TAP_TAP35_C2        0x09cf22b4 /* eee 100Base-tx timer misc control */
#define BRPHY3_DSP_TAP_TAP35_C3        0x09cf22b6 /* pcs_lpi_test */
#define BRPHY3_DSP_TAP_FILTER_CTL_CH0  0x09cf22b8 /* Filter Freeze/Disable per channel Control */
#define BRPHY3_DSP_TAP_FILTER_CTL_CH1  0x09cf22ba /* Filter Freeze/Disable per channel Control */
#define BRPHY3_DSP_TAP_FILTER_CTL_CH2  0x09cf22bc /* Filter Freeze/Disable per channel Control */
#define BRPHY3_DSP_TAP_FILTER_CTL_CH3  0x09cf22be /* Filter Freeze/Disable per channel Control */
#define BRPHY3_DSP_TAP_FILTER_CTL_FFEX_CH0 0x09cf22c0 /* FFEX Freeze/Disable per channel Control */
#define BRPHY3_DSP_TAP_FILTER_CTL_FFEX_CH1 0x09cf22c2 /* FFEX Freeze/Disable per channel Control */
#define BRPHY3_DSP_TAP_FILTER_CTL_FFEX_CH2 0x09cf22c4 /* FFEX Freeze/Disable per channel Control */
#define BRPHY3_DSP_TAP_FILTER_CTL_FFEX_CH3 0x09cf22c6 /* FFEX Freeze/Disable per channel Control */
#define BRPHY3_DSP_TAP_EMI_DATAPATH_CTL 0x09cf22c8 /* EMI Datapath Control */
#define BRPHY3_DSP_TAP_EMI_DATAPATH_CTL2 0x09cf22ca /* EMI Datapath Control2 */
#define BRPHY3_DSP_TAP_FFEX_CTL        0x09cf22cc /* FFE X-tap Control */
#define BRPHY3_DSP_TAP_PHYC_BREAKPOINT_CTL0 0x09cf22ce /* Phycontrol Breakpoint Control 0 */
#define BRPHY3_DSP_TAP_PHYC_BREAKPOINT_CTL1 0x09cf22d0 /* Phycontrol Breakpoint Control 1 */
#define BRPHY3_DSP_TAP_FILTER_ADDR     0x09cf2360 /* DSP Coefficient Address Register */
#define BRPHY3_DSP_TAP_FILTER_CTL      0x09cf2362 /* DSP Control Register */
#define BRPHY3_DSP_TAP_FILTER_DATA     0x09cf2364 /* DSP Coefficient Read/Write Port */


/****************************************************************************
 * bcm89530_top_bridge_brphys_brphys3_BRPHY3_PLL_CTRL
 ***************************************************************************/
#define BRPHY3_PLL_CTRL_PLLCTRL_0      0x09cf2390 /* Analog pll control 0 */
#define BRPHY3_PLL_CTRL_PLLCTRL_1      0x09cf2392 /* Analog pll control 1 */
#define BRPHY3_PLL_CTRL_PLLCTRL_2      0x09cf2394 /* Analog pll control 2 */
#define BRPHY3_PLL_CTRL_PLLCTRL_3      0x09cf2396 /* Analog pll control 3 */
#define BRPHY3_PLL_CTRL_PLLCTRL_4      0x09cf2398 /* Analog pll control 4 */
#define BRPHY3_PLL_CTRL_PLLCTRL_5      0x09cf239a /* Analog pll control 5 */
#define BRPHY3_PLL_CTRL_PLLCTRL_6      0x09cf239c /* Analog pll control 6 */
#define BRPHY3_PLL_CTRL_PLL_STATUS_0   0x09cf23a0 /* Analog PLL Status 0 */
#define BRPHY3_PLL_CTRL_PLL_STATUS_1   0x09cf23a2 /* Analog PLL Status 1 */
#define BRPHY3_PLL_CTRL_AFE_SIGDET_STATUS 0x09cf23a4 /* AFE Signal detect */
#define BRPHY3_PLL_CTRL_PLLCTRL_7      0x09cf23a6 /* Analog pll control 7 */
#define BRPHY3_PLL_CTRL_PLLCTRL_8      0x09cf23a8 /* Analog pll control 8 */


/****************************************************************************
 * bcm89530_top_bridge_brphys_brphys3_BRPHY3_AFE_CTRL
 ***************************************************************************/
#define BRPHY3_AFE_CTRL_RXCONFIG_0     0x09cf23c0 /* RXCONFIG 15:0 */
#define BRPHY3_AFE_CTRL_RXCONFIG_1     0x09cf23c2 /* RXCONFIG 31:16 */
#define BRPHY3_AFE_CTRL_RXCONFIG_2     0x09cf23c4 /* RXCONFIG 47:32 */
#define BRPHY3_AFE_CTRL_RXCONFIG_3     0x09cf23c6 /* RXCONFIG 63:48 */
#define BRPHY3_AFE_CTRL_RXCONFIG_4     0x09cf23c8 /* RXCONFIG 79:64 */
#define BRPHY3_AFE_CTRL_RXCONFIG5_LP   0x09cf23ca /* RXCONFIG 86:80 and LP tuning */
#define BRPHY3_AFE_CTRL_TX_CONFIG_0    0x09cf23cc /* TXCONFIG 15:0 */
#define BRPHY3_AFE_CTRL_TX_CONFIG_1    0x09cf23ce /* TXCONFIG 31:16 */
#define BRPHY3_AFE_CTRL_VDAC_ICTRL_0   0x09cf23d0 /* VDAC CURRENT Control 15:0 */
#define BRPHY3_AFE_CTRL_VDAC_ICTRL_1   0x09cf23d2 /* VDAC CURRENT Control 31:16 */
#define BRPHY3_AFE_CTRL_VDAC_ICTRL_2   0x09cf23d4 /* VDAC CURRENT Control 51:36 */
#define BRPHY3_AFE_CTRL_VDAC_OTHERS_0  0x09cf23d6 /* VDAC CURRENT 35:32 and others */
#define BRPHY3_AFE_CTRL_HPF_TRIM_OTHERS 0x09cf23d8 /* HPF trim and reserved bits */
#define BRPHY3_AFE_CTRL_TX_EXTRA_CONFIG_0 0x09cf23da /* TXCONFIG 15:0 */
#define BRPHY3_AFE_CTRL_TX_EXTRA_CONFIG_1 0x09cf23dc /* TXCONFIG 15:0 */
#define BRPHY3_AFE_CTRL_TX_EXTRA_CONFIG_2 0x09cf23de /* TXCONFIG 15:0 */
#define BRPHY3_AFE_CTRL_TEMPSEN_OTHERS 0x09cf23e0 /* TEMPSEN_OTHERS 15:0 */
#define BRPHY3_AFE_CTRL_FUTURE_RSV     0x09cf23e2 /* FUTURE_RSV 15:0 */


/****************************************************************************
 * bcm89530_top_bridge_brphys_brphys3_BRPHY3_ECD_CTRL
 ***************************************************************************/
#define BRPHY3_ECD_CTRL_EXPC0          0x09cf2540 /* ECD Control and Status */
#define BRPHY3_ECD_CTRL_EXPC1          0x09cf2542 /* ECD Fault Type */
#define BRPHY3_ECD_CTRL_EXPC2          0x09cf2544 /* ECD Pair A Length Results */
#define BRPHY3_ECD_CTRL_EXPC3          0x09cf2546 /* ECD Pair B Length Results */
#define BRPHY3_ECD_CTRL_EXPC4          0x09cf2548 /* ECD Pair C Length Results */
#define BRPHY3_ECD_CTRL_EXPC5          0x09cf254a /* ECD Pair D Length Results */
#define BRPHY3_ECD_CTRL_EXPC6          0x09cf254c /* ECD XTALK Map */
#define BRPHY3_ECD_CTRL_EXPC7          0x09cf254e /* ECD EXTRA RESERVED REGISTER */
#define BRPHY3_ECD_CTRL_EXPC8          0x09cf2550 /* ECD EXTRA RESERVED REGISTER */
#define BRPHY3_ECD_CTRL_EXPC9          0x09cf2552 /* ECD EXTRA RESERVED REGISTER */
#define BRPHY3_ECD_CTRL_EXPCA          0x09cf2554 /* ECD EXTRA RESERVED REGISTER */
#define BRPHY3_ECD_CTRL_EXPCB          0x09cf2556 /* ECD EXTRA RESERVED REGISTER */
#define BRPHY3_ECD_CTRL_EXPCC          0x09cf2558 /* ECD EXTRA RESERVED REGISTER */
#define BRPHY3_ECD_CTRL_EXPCD          0x09cf255a /* ECD EXTRA RESERVED REGISTER */
#define BRPHY3_ECD_CTRL_EXPCE          0x09cf255c /* ECD EXTRA RESERVED REGISTER */
#define BRPHY3_ECD_CTRL_EXPCF          0x09cf255e /* ECD EXTRA RESERVED REGISTER */
#define BRPHY3_ECD_CTRL_EXPE0          0x09cf2560 /* ECD EXTRA RESERVED REGISTER */
#define BRPHY3_ECD_CTRL_EXPE1          0x09cf2562 /* ECD EXTRA RESERVED REGISTER */
#define BRPHY3_ECD_CTRL_EXPE2          0x09cf2564 /* ECD EXTRA RESERVED REGISTER */
#define BRPHY3_ECD_CTRL_EXPE3          0x09cf2566 /* ECD EXTRA RESERVED REGISTER */
#define BRPHY3_ECD_CTRL_EXPE4          0x09cf2568 /* ECD EXTRA RESERVED REGISTER */
#define BRPHY3_ECD_CTRL_EXPE5          0x09cf256a /* ECD EXTRA RESERVED REGISTER */
#define BRPHY3_ECD_CTRL_EXPE6          0x09cf256c /* ECD EXTRA RESERVED REGISTER */
#define BRPHY3_ECD_CTRL_EXPE7          0x09cf256e /* ECD EXTRA RESERVED REGISTER */
#define BRPHY3_ECD_CTRL_EXPE8          0x09cf2570 /* ECD EXTRA RESERVED REGISTER */
#define BRPHY3_ECD_CTRL_EXPE9          0x09cf2572 /* ECD EXTRA RESERVED REGISTER */
#define BRPHY3_ECD_CTRL_EXPEA          0x09cf2574 /* ECD EXTRA RESERVED REGISTER */
#define BRPHY3_ECD_CTRL_EXPEB          0x09cf2576 /* ECD EXTRA RESERVED REGISTER */
#define BRPHY3_ECD_CTRL_EXPEC          0x09cf2578 /* ECD EXTRA RESERVED REGISTER */
#define BRPHY3_ECD_CTRL_EXPED          0x09cf257a /* ECD EXTRA RESERVED REGISTER */
#define BRPHY3_ECD_CTRL_EXPEE          0x09cf257c /* ECD EXTRA RESERVED REGISTER */
#define BRPHY3_ECD_CTRL_EXPEF          0x09cf257e /* ECD EXTRA RESERVED REGISTER */


/****************************************************************************
 * bcm89530_top_bridge_brphys_brphys3_BRPHY3_BR_CTRL
 ***************************************************************************/
#define BRPHY3_BR_CTRL_EXP90           0x09cf2600 /* BroadReach LRE Misc Control */
#define BRPHY3_BR_CTRL_EXP91           0x09cf2602 /* BroadReach LRE Misc Control */
#define BRPHY3_BR_CTRL_EXP92           0x09cf2604 /* BroadReach LRE Misc Control */
#define BRPHY3_BR_CTRL_EXP93           0x09cf2606 /* BroadReach LDS  Control */
#define BRPHY3_BR_CTRL_EXP94           0x09cf2608 /* BroadReach LDS RX Control */
#define BRPHY3_BR_CTRL_EXP95           0x09cf260a /* BroadReach LDS RX Control */
#define BRPHY3_BR_CTRL_EXP96           0x09cf260c /* BroadReach LDS Scan, ARB and TX Status */
#define BRPHY3_BR_CTRL_EXP97           0x09cf260e /* BroadReach LDS Scan, ARB and TX Status */
#define BRPHY3_BR_CTRL_EXP99           0x09cf2612 /* BroadReach LDS Timer Control */
#define BRPHY3_BR_CTRL_EXP9A           0x09cf2614 /* LDS Status */
#define BRPHY3_BR_CTRL_EXP9B           0x09cf2616 /* BroadR-Reach PLL Control */
#define BRPHY3_BR_CTRL_EXP9D           0x09cf261a /* EoC Internal Control 1 */
#define BRPHY3_BR_CTRL_EXP9E           0x09cf261c /* LDS Length Threshold 0 */
#define BRPHY3_BR_CTRL_EXP9F           0x09cf261e /* LDS Length Threshold 1 */
#define BRPHY3_BR_CTRL_EXPA0           0x09cf2620 /* HLDS register, LDS extend advertisement register */
#define BRPHY3_BR_CTRL_EXPA1           0x09cf2622 /* HLDS register, LDS link partner extend ability register */
#define BRPHY3_BR_CTRL_EXPA2           0x09cf2624 /* HLDS Register */
#define BRPHY3_BR_CTRL_BR_MISC_CONTROL_STATUS 0x09cf2626 /* Broadreach Misc Status */
#define BRPHY3_BR_CTRL_BR250_CTL       0x09cf263c /* BR250 Control */


/****************************************************************************
 * bcm89530_top_bridge_brphys_brphys3_BRPHY3_BR_CL22_IEEE
 ***************************************************************************/
#define BRPHY3_BR_CL22_IEEE_MII_CTRL   0x09cfffc0 /* BR_LRE_Control_Register */
#define BRPHY3_BR_CL22_IEEE_MII_STAT   0x09cfffc2 /* BR_LRE_Status_Register */
#define BRPHY3_BR_CL22_IEEE_PHY_ID_MSB 0x09cfffc4 /* PHY_Identifier_MSB_Register */
#define BRPHY3_BR_CL22_IEEE_PHY_ID_LSB 0x09cfffc6 /* PHY_Identifier_LSB_Register */
#define BRPHY3_BR_CL22_IEEE_LDS_ADV_ABILITY_BP 0x09cfffc8 /* LDS_Advertised_Ability_Register (Base Page) */
#define BRPHY3_BR_CL22_IEEE_LDS_ADV_CONTROL 0x09cfffca /* LDS_Advertised_Control_Register */
#define BRPHY3_BR_CL22_IEEE_LDS_ABILITY 0x09cfffcc /* LDS_Ability_Register (Next Page) */
#define BRPHY3_BR_CL22_IEEE_LDS_LP_ABILITY_BP 0x09cfffce /* LDS_Link_Partner_Ability_Base_Page_Register (Base Page) */
#define BRPHY3_BR_CL22_IEEE_LDS_LP_MSG_NXTP 0x09cfffd0 /* LDS_Link_Partners_Nxt_Pg_Msg_Register (Next Page) */
#define BRPHY3_BR_CL22_IEEE_LDS_LP_ABILITY_NXTP 0x09cfffd2 /* LDS_Link_Partner_Ability_Nxt_Pg_Register (Next Page) */
#define BRPHY3_BR_CL22_IEEE_LDS_LDS_EXP 0x09cfffd4 /* LDS_Expansion_Register */
#define BRPHY3_BR_CL22_IEEE_LRE_EXTENDED_STAT 0x09cfffd6 /* IEEE_Extended_Status_Register */


/****************************************************************************
 * bcm89530_top_bridge_brphys_brphys4_BRPHY4_CL45DEV1
 ***************************************************************************/
#define BRPHY4_CL45DEV1_PMD_IEEE_CTL1  0x0a020000 /* IEEE PMA/PMD CONTROL 1 REGISTER (REG 1.0) */
#define BRPHY4_CL45DEV1_PMD_IEEE_ST1   0x0a020002 /* IEEE PMA/PMD STATUS 1 REGISTER  (REG 1.1) */
#define BRPHY4_CL45DEV1_PMD_IEEE_DEV_ID0 0x0a020004 /* IEEE PMA/PMD DEVICE IDENTIFIER PART 0 (REG 1.2) */
#define BRPHY4_CL45DEV1_PMD_IEEE_DEV_ID1 0x0a020006 /* IEEE PMA/PMD DEVICE IDENTIFIER PART 1 (REG 1.3) */
#define BRPHY4_CL45DEV1_PMD_IEEE_DEV0  0x0a02000a /* DEVICES IN PACKAGE REGISTER PART 0 (REG 1.5) */
#define BRPHY4_CL45DEV1_PMD_IEEE_DEV1  0x0a02000c /* DEVICES IN PACKAGE REGISTER PART 1 (REG 1.6) */
#define BRPHY4_CL45DEV1_PMD_IEEE_PKG_ID0 0x0a02001c /* PMA/PMD PACKAGE IDENTIFIER (REG 1.14) */
#define BRPHY4_CL45DEV1_PMD_IEEE_PKG_ID1 0x0a02001e /* PMA/PMD PACKAGE IDENTIFIER (REG 1.15) */
#define BRPHY4_CL45DEV1_PMD_IEEE_TSYNC_CAP 0x0a020e10 /* TimeSync PMA/PMD capability (REG 1.1800) */
#define BRPHY4_CL45DEV1_PMD_IEEE_TX_TSYNC_MAX_DELAY_LOWER 0x0a020e12 /* Maximum PMA/PMD transmit path data delay, lower (REG 1.1801) */
#define BRPHY4_CL45DEV1_PMD_IEEE_TX_TSYNC_MAX_DELAY_UPPER 0x0a020e14 /* Maximum PMA/PMD transmit path data delay, upper (REG 1.1802) */
#define BRPHY4_CL45DEV1_PMD_IEEE_TX_TSYNC_MIN_DELAY_LOWER 0x0a020e16 /* Minimum PMA/PMD transmit path data delay, lower (REG 1.1803) */
#define BRPHY4_CL45DEV1_PMD_IEEE_TX_TSYNC_MIN_DELAY_UPPER 0x0a020e18 /* Minimum PMA/PMD transmit path data delay, upper (REG 1.1804) */
#define BRPHY4_CL45DEV1_PMD_IEEE_RX_TSYNC_MAX_DELAY_LOWER 0x0a020e1a /* Maximum PMA/PMD receive path data delay, lower (REG 1.1805) */
#define BRPHY4_CL45DEV1_PMD_IEEE_RX_TSYNC_MAX_DELAY_UPPER 0x0a020e1c /* Maximum PMA/PMD receive path data delay, upper (REG 1.1806) */
#define BRPHY4_CL45DEV1_PMD_IEEE_RX_TSYNC_MIN_DELAY_LOWER 0x0a020e1e /* Minimum PMA/PMD receive path data delay, lower (REG 1.1807) */
#define BRPHY4_CL45DEV1_PMD_IEEE_RX_TSYNC_MIN_DELAY_UPPER 0x0a020e20 /* Minimum PMA/PMD receive path data delay, upper (REG 1.1808) */


/****************************************************************************
 * bcm89530_top_bridge_brphys_brphys4_BRPHY4_CL45DEV3
 ***************************************************************************/
#define BRPHY4_CL45DEV3_PCS_IEEE_CTL1  0x0a060000 /* IEEE PCS CONTROL 1 REGISTER (REG 3.0) */
#define BRPHY4_CL45DEV3_PCS_IEEE_ST1   0x0a060002 /* IEEE PCS STATUS 1 REGISTER  (REG 3.1) */
#define BRPHY4_CL45DEV3_PCS_IEEE_DEV_ID0 0x0a060004 /* IEEE PCS DEVICE IDENTIFIER PART 0 (REG 3.2) */
#define BRPHY4_CL45DEV3_PCS_IEEE_DEV_ID1 0x0a060006 /* IEEE PCS DEVICE IDENTIFIER PART 1 (REG 3.3) */
#define BRPHY4_CL45DEV3_PCS_IEEE_DEV0  0x0a06000a /* DEVICES IN PACKAGE REGISTER PART 0 (REG 3.5) */
#define BRPHY4_CL45DEV3_PCS_IEEE_DEV1  0x0a06000c /* DEVICES IN PACKAGE REGISTER PART 1 (REG 3.6) */
#define BRPHY4_CL45DEV3_PCS_IEEE_PKG_ID0 0x0a06001c /* PCS PACKAGE IDENTIFIER (REG 3.14) */
#define BRPHY4_CL45DEV3_PCS_IEEE_PKG_ID1 0x0a06001e /* PCS PACKAGE IDENTIFIER (REG 3.15) */
#define BRPHY4_CL45DEV3_PCS_EEE_CAP    0x0a060028 /* PCS_EEE_CAP(REG 3.20) */
#define BRPHY4_CL45DEV3_PCS_EEE_WAKE_ERR_CNT 0x0a06002c /* PCS_EEE_Wake_Err_Cnt(REG 3.22) */
#define BRPHY4_CL45DEV3_PCS_IEEE_TSYNC_CAP 0x0a060e10 /* TimeSync PCS capability (REG 3.1800) */
#define BRPHY4_CL45DEV3_PCS_IEEE_TX_TSYNC_MAX_DELAY_LOWER 0x0a060e12 /* Maximum PCS transmit path data delay, lower (REG 3.1801) */
#define BRPHY4_CL45DEV3_PCS_IEEE_TX_TSYNC_MAX_DELAY_UPPER 0x0a060e14 /* Maximum PCS transmit path data delay, upper (REG 3.1802) */
#define BRPHY4_CL45DEV3_PCS_IEEE_TX_TSYNC_MIN_DELAY_LOWER 0x0a060e16 /* Minimum PCS transmit path data delay, lower (REG 3.1803) */
#define BRPHY4_CL45DEV3_PCS_IEEE_TX_TSYNC_MIN_DELAY_UPPER 0x0a060e18 /* Minimum PCS transmit path data delay, upper (REG 3.1804) */
#define BRPHY4_CL45DEV3_PCS_IEEE_RX_TSYNC_MAX_DELAY_LOWER 0x0a060e1a /* Maximum PCS receive path data delay, lower (REG 3.1805) */
#define BRPHY4_CL45DEV3_PCS_IEEE_RX_TSYNC_MAX_DELAY_UPPER 0x0a060e1c /* Maximum PCS receive path data delay, upper (REG 3.1806) */
#define BRPHY4_CL45DEV3_PCS_IEEE_RX_TSYNC_MIN_DELAY_LOWER 0x0a060e1e /* Minimum PCS receive path data delay, lower (REG 3.1807) */
#define BRPHY4_CL45DEV3_PCS_IEEE_RX_TSYNC_MIN_DELAY_UPPER 0x0a060e20 /* Minimum PCS receive path data delay, upper (REG 3.1808) */


/****************************************************************************
 * bcm89530_top_bridge_brphys_brphys4_BRPHY4_CL45DEV7
 ***************************************************************************/
#define BRPHY4_CL45DEV7_AN_CTRL        0x0a0e0000 /* Auto Neg Extended Next Page Control     (0x0000) (REG 7.0) */
#define BRPHY4_CL45DEV7_AN_STAT        0x0a0e0002 /* AN Status (0x0001) (REG 7.1) */
#define BRPHY4_CL45DEV7_AN_DEV_ID_LSB  0x0a0e0004 /* Auto Neg Device Identifier Lower 16 bit (0x0002) (REG 7.2) */
#define BRPHY4_CL45DEV7_AN_DEV_ID_MSB  0x0a0e0006 /* Auto Neg Device Identifier Upper 16 bit (0x0003) (REG 7.3) */
#define BRPHY4_CL45DEV7_AN_DEV_IN_PKG_LSB 0x0a0e000a /* Auto Neg Device In Package Lower 16 bit (0x0005) (REG 7.5) */
#define BRPHY4_CL45DEV7_AN_DEV_IN_PKG_MSB 0x0a0e000c /* Auto Neg Device In Package Upper 16 bit (0x0006) (REG 7.6) */
#define BRPHY4_CL45DEV7_AN_DEV_PKG_ID_LSB 0x0a0e001c /* Auto Neg Package ID Lower 16 bit(0x000e) (REG 7.14) */
#define BRPHY4_CL45DEV7_AN_DEV_PKG_ID_MSB 0x0a0e001e /* Auto Neg Package ID Upper 16 bit(0x000f) (REG 7.15) */
#define BRPHY4_CL45DEV7_AN_AD          0x0a0e0020 /* Auto Neg AD(0x0010) (REG 7.16) */
#define BRPHY4_CL45DEV7_AN_LPA         0x0a0e0026 /* AN LP base page ability (0x0013) (REG 7.19) */
#define BRPHY4_CL45DEV7_AN_XNPA        0x0a0e002c /* AN XNP transmit A (0x0016) (REG 7.22) */
#define BRPHY4_CL45DEV7_AN_XNPB        0x0a0e002e /* AN XNP transmit B (0x0017) (REG 7.23) */
#define BRPHY4_CL45DEV7_AN_XNPC        0x0a0e0030 /* AN XNP transmit C (0x0018) (REG 7.24) */
#define BRPHY4_CL45DEV7_LP_XNPA        0x0a0e0032 /* AN LP XNP ability A (0x0019) (REG 7.25) */
#define BRPHY4_CL45DEV7_LP_XNPB        0x0a0e0034 /* AN LP XNP ability B (0x001a) (REG 7.26) */
#define BRPHY4_CL45DEV7_LP_XNPC        0x0a0e0036 /* AN LP XNP ability C (0x001b) (REG 7.27) */
#define BRPHY4_CL45DEV7_TENG_AN_CTRL   0x0a0e0040 /* 10G Base-T AN Control Register (0x0020) (REG 7.32) */
#define BRPHY4_CL45DEV7_TENG_AN_STAT   0x0a0e0042 /* 10G Base-T AN Status Register  (0x0021) (REG 7.33) */
#define BRPHY4_CL45DEV7_EEE_ADV        0x0a0e0078 /* EEE Advertisement (0x003C) (REG 7.60 ???) */
#define BRPHY4_CL45DEV7_EEE_LP_ADV     0x0a0e007a /* EEE Link Partner Advertisement (0x003D) (REG 7.61 ???) */
#define BRPHY4_CL45DEV7_EEE_MODE_CTL   0x0a0e007c /* EEE Mode Control (0x003E) (REG 7.62 ???) */


/****************************************************************************
 * bcm89530_top_bridge_brphys_brphys4_BRPHY4_CL45VEN
 ***************************************************************************/
#define BRPHY4_CL45VEN_FORCE_LINK      0x0a0f0000 /* Force Link Register */
#define BRPHY4_CL45VEN_SELECTIVE_RESET 0x0a0f0002 /* Selective Reset Register */
#define BRPHY4_CL45VEN_TEST_FSM_EXT_NXT_PGS 0x0a0f0004 /* Test State Machine For Extended Next Pages Register --mvadkert */
#define BRPHY4_CL45VEN_TEST_FSM_NXT_PGS 0x0a0f0006 /* Test State Machine For Next Pages Register --mvadkert */
#define BRPHY4_CL45VEN_AN_MAN_TEST     0x0a0f0032 /* Auto Negotiation Manual Test Register */
#define BRPHY4_CL45VEN_AN_MAN_LNK_PARTNR_ABI_A 0x0a0f0034 /* Auto Negotiation Manual Link Partners Abilities Register A */
#define BRPHY4_CL45VEN_AN_MAN_LNK_PARTNR_ABI_B 0x0a0f0036 /* Auto Negotiation Manual Link Partners Abilities Register B */
#define BRPHY4_CL45VEN_LNK_PARTNR_NXT_PG_A 0x0a0f0038 /* Link Partner Next Page */
#define BRPHY4_CL45VEN_LNK_PARTNR_NXT_PG_B 0x0a0f003a /* Link Partner Next Page (cont.) */
#define BRPHY4_CL45VEN_LNK_PARTNR_NXT_PG_C 0x0a0f003c /* Link Partner Next Page (cont.) */
#define BRPHY4_CL45VEN_LNK_PARTNR_NXT_PG_D 0x0a0f003e /* Link Partner Next Page (cont.) */
#define BRPHY4_CL45VEN_LNK_PARTNR_NXT_PG_E 0x0a0f0040 /* Link Partner Next Page (cont.) */
#define BRPHY4_CL45VEN_LNK_PARTNR_NXT_PG_F 0x0a0f0042 /* Link Partner Next Page (cont.) */
#define BRPHY4_CL45VEN_EPON_CTRL_REG   0x0a0f0046 /* EPON mode control register */
#define BRPHY4_CL45VEN_EEE_TEST_CTRL_A 0x0a0f0060 /* EEE Test Control Register A eee_test_control_bus[15:0] */
#define BRPHY4_CL45VEN_EEE_TEST_CTRL_B 0x0a0f0062 /* EEE Test Control Register B eee_test_control_bus[31:16] */
#define BRPHY4_CL45VEN_EEE_TEST_CTRL_C 0x0a0f0064 /* EEE Test Control Register C eee_test_control_bus[47:32] */
#define BRPHY4_CL45VEN_EEE_SPARE_1     0x0a0f0076 /* EEE Spare Register 1 */
#define BRPHY4_CL45VEN_EEE_SPARE_2     0x0a0f0078 /* EEE Spare Register 2 */
#define BRPHY4_CL45VEN_EEE_CONTROL     0x0a0f007a /* EEE Control Register */
#define BRPHY4_CL45VEN_EEE_RES_STAT    0x0a0f007c /* EEE Resolution Status Register */
#define BRPHY4_CL45VEN_LPI_MODE_CNTR   0x0a0f007e /* LPI Mode Counter Register */
#define BRPHY4_CL45VEN_LOC_DEV_MSG_5_A 0x0a0f0080 /* Local Device Message 5 */
#define BRPHY4_CL45VEN_LOC_DEV_MSG_5_B 0x0a0f0082 /* Local Device Message 5 cont. */
#define BRPHY4_CL45VEN_LOC_DEV_MSG_5_C 0x0a0f0084 /* Local Device Message 5 cont. */
#define BRPHY4_CL45VEN_LOC_DEV_MSG_5_D 0x0a0f0086 /* Local Device Message 5 cont. */
#define BRPHY4_CL45VEN_LNK_PARTNR_MSG_5_A 0x0a0f0088 /* Link Partner Message 5 */
#define BRPHY4_CL45VEN_LNK_PARTNR_MSG_5_B 0x0a0f008a /* Link Partner Message 5 cont. */
#define BRPHY4_CL45VEN_LNK_PARTNR_MSG_5_C 0x0a0f008c /* Link Partner Message 5 cont. */
#define BRPHY4_CL45VEN_LNK_PARTNR_MSG_5_D 0x0a0f008e /* Link Partner Message 5 cont. */
#define BRPHY4_CL45VEN_LOC_DEV_MSG_6_A 0x0a0f0090 /* Local Device Message 6 */
#define BRPHY4_CL45VEN_LOC_DEV_MSG_6_B 0x0a0f0092 /* Local Device Message 6 cont. */
#define BRPHY4_CL45VEN_LOC_DEV_MSG_6_C 0x0a0f0094 /* Local Device Message 6 cont. */
#define BRPHY4_CL45VEN_LOC_DEV_MSG_6_D 0x0a0f0096 /* Local Device Message 6 cont. */
#define BRPHY4_CL45VEN_LNK_PARTNR_MSG_6_A 0x0a0f0098 /* Link Partner Message 6 */
#define BRPHY4_CL45VEN_LNK_PARTNR_MSG_6_B 0x0a0f009a /* Link Partner Message 6 cont. */
#define BRPHY4_CL45VEN_LNK_PARTNR_MSG_6_C 0x0a0f009c /* Link Partner Message 6 cont. */
#define BRPHY4_CL45VEN_LNK_PARTNR_MSG_6_D 0x0a0f009e /* Link Partner Message 6 cont. */


/****************************************************************************
 * bcm89530_top_bridge_brphys_brphys4_BRPHY4_GPHY_CORE
 ***************************************************************************/
#define BRPHY4_GPHY_CORE_BASE10        0x0a0f2000 /* PHY_Extended_ctl_Register */
#define BRPHY4_GPHY_CORE_BASE11        0x0a0f2002 /* PHY_Extended_Status_Register (copper side only) */
#define BRPHY4_GPHY_CORE_BASE12        0x0a0f2004 /* Receive_Error_Cntr_Register */
#define BRPHY4_GPHY_CORE_BASE13        0x0a0f2006 /* False_Carrier_Sense_Cntr_Register */
#define BRPHY4_GPHY_CORE_BASE14        0x0a0f2008 /* Local_Remote_Receiver_NOT_OK_Cntrs_Register */
#define BRPHY4_GPHY_CORE_EXP45         0x0a0f200a /* Pattern Generator Control Register */
#define BRPHY4_GPHY_CORE_EXP46         0x0a0f200b /* Pattern Generator Status Register */
#define BRPHY4_GPHY_CORE_BASE19        0x0a0f2012 /* Auxiliary Status Summary (copper side only) */
#define BRPHY4_GPHY_CORE_BASE1A        0x0a0f2014 /* Interrupt Status Register (copper side only) */
#define BRPHY4_GPHY_CORE_BASE1B        0x0a0f2016 /* Interrupt Mask Register */
#define BRPHY4_GPHY_CORE_BASE1D_SHD    0x0a0f2018 /* HCD Status Register */
#define BRPHY4_GPHY_CORE_BASE1D        0x0a0f201a /* Master/Slave Seed Register */
#define BRPHY4_GPHY_CORE_BASE1E        0x0a0f201c /* Test1_Register */
#define BRPHY4_GPHY_CORE_BASE1F        0x0a0f201e /* Test2_Register */
#define BRPHY4_GPHY_CORE_SHD1C_00      0x0a0f2020 /* Cabletron LED Register (Shadow Register Selector = "00h") */
#define BRPHY4_GPHY_CORE_SHD1C_01      0x0a0f2022 /* TVCO Selection Register (Shadow Register Selector = "01h") */
#define BRPHY4_GPHY_CORE_SHD1C_02      0x0a0f2024 /* reserved Control 1 Register (Shadow Register Selector = "02h") */
#define BRPHY4_GPHY_CORE_SHD1C_03      0x0a0f2026 /* Clock Alignment Control Regsiter (Shadow Register Selector = "03h") */
#define BRPHY4_GPHY_CORE_SHD1C_04      0x0a0f2028 /* reserved Control 2 Register (Shadow Register Selector = "04h") */
#define BRPHY4_GPHY_CORE_SHD1C_05      0x0a0f202a /* reserved Control 3 Register (Shadow Register Selector = "05h") */
#define BRPHY4_GPHY_CORE_SHD1C_06      0x0a0f202c /* Tdr Control 1 Register (Shadow Register Selector = "06h") */
#define BRPHY4_GPHY_CORE_SHD1C_07      0x0a0f202e /* Tdr Control 2 Register (Shadow Register Selector = "07h") */
#define BRPHY4_GPHY_CORE_SHD1C_08      0x0a0f2030 /* Led Status Register (Shadow Register Selector = "08h") */
#define BRPHY4_GPHY_CORE_SHD1C_09      0x0a0f2032 /* Led Control Register (Shadow Register Selector = "09h") */
#define BRPHY4_GPHY_CORE_SHD1C_0A      0x0a0f2034 /* Auto-Power Down Register (Shadow Register Selector = "0ah") */
#define BRPHY4_GPHY_CORE_SHD1C_0B      0x0a0f2036 /* reserved Control 4 Register (Shadow Register Selector = "0bh") */
#define BRPHY4_GPHY_CORE_SHD1C_0D      0x0a0f203a /* LED Selector 1 Register (Shadow Register Selector = "0dh") */
#define BRPHY4_GPHY_CORE_SHD1C_0E      0x0a0f203c /* LED Selector 2 Register (Shadow Register Selector = "0eh") */
#define BRPHY4_GPHY_CORE_SHD1C_0F      0x0a0f203e /* LED GPIO Control/Status Register (Shadow Register Selector = "0fh") */
#define BRPHY4_GPHY_CORE_SHD1C_10      0x0a0f2040 /* Cisco Enhanced Linkstatus Mode Control Register (Shadow Register Selector = "10h") */
#define BRPHY4_GPHY_CORE_SHD1C_1F      0x0a0f2042 /* Mode Control Register (Shadow Register Selector = "1fh") */
#define BRPHY4_GPHY_CORE_SHD18_000     0x0a0f2050 /* Auxiliary Control Register (Shadow Register Selector = "000") */
#define BRPHY4_GPHY_CORE_SHD18_001     0x0a0f2052 /* 10 Base-T Register (Shadow Register Selector = "001") */
#define BRPHY4_GPHY_CORE_SHD18_010     0x0a0f2054 /* Power/MII Control Register (Shadow Register Selector = "010") */
#define BRPHY4_GPHY_CORE_SHD18_011     0x0a0f2056 /* IP Phone Register  (Shadow Register Selector = "011") */
#define BRPHY4_GPHY_CORE_SHD18_100     0x0a0f2058 /* Misc Test Register  1 (Shadow Register Selector = "100") */
#define BRPHY4_GPHY_CORE_SHD18_101     0x0a0f205a /* Misc Test Register  2 (Shadow Register Selector = "101") */
#define BRPHY4_GPHY_CORE_SHD18_110     0x0a0f205c /* Manual IP Phone Seed Register (Shadow Register Selector = "110") */
#define BRPHY4_GPHY_CORE_SHD18_111     0x0a0f205e /* Miscellanous Control Register (Shadow Register Selector = "111") */
#define BRPHY4_GPHY_CORE_EXP00         0x0a0f2060 /* Transmit Packet Counter */
#define BRPHY4_GPHY_CORE_EXP01         0x0a0f2062 /* Expansion Interrupt Status */
#define BRPHY4_GPHY_CORE_EXP02         0x0a0f2064 /* Expansion Interrupt Mask */
#define BRPHY4_GPHY_CORE_EXP03         0x0a0f2066 /* Spare Registers */
#define BRPHY4_GPHY_CORE_EXP04         0x0a0f2068 /* Bicolor LED Selectors */
#define BRPHY4_GPHY_CORE_EXP05         0x0a0f206a /* Bicolor LED Flash Rate Controls */
#define BRPHY4_GPHY_CORE_EXP06         0x0a0f206c /* Bicolor LED Programmable Blink Controls */
#define BRPHY4_GPHY_CORE_EXP07         0x0a0f206e /* Far End Fault */
#define BRPHY4_GPHY_CORE_EXP08         0x0a0f2070 /* 10BT Controls */
#define BRPHY4_GPHY_CORE_EXP09         0x0a0f2072 /* AMRR Controls */
#define BRPHY4_GPHY_CORE_EXP0A         0x0a0f2074 /* DAC TEMPLATE Controls */
#define BRPHY4_GPHY_CORE_EXP0B         0x0a0f2076 /* External Status */
#define BRPHY4_GPHY_CORE_EXP0C         0x0a0f2078 /* Spare Registers */
#define BRPHY4_GPHY_CORE_EXP30         0x0a0f2080 /* Late Collision Counters Status Register */
#define BRPHY4_GPHY_CORE_EXP31         0x0a0f2082 /* Late Collision Counter [64:95] */
#define BRPHY4_GPHY_CORE_EXP32         0x0a0f2084 /* Late Collision Counter [96:127] */
#define BRPHY4_GPHY_CORE_EXP33         0x0a0f2086 /* Late Collision Counter [128:191] */
#define BRPHY4_GPHY_CORE_EXP34         0x0a0f2088 /* Late Collision Counter [192:319] */
#define BRPHY4_GPHY_CORE_EXP35         0x0a0f208a /* Late Collision Counter Threshold Register */
#define BRPHY4_GPHY_CORE_EXP36         0x0a0f208c /* Clock PPM Detection between Recovery and Transmit Clocks */
#define BRPHY4_GPHY_CORE_EXP37         0x0a0f208e /* Clock PPM Detection between GTX_CLK and Transmit Clocks */
#define BRPHY4_GPHY_CORE_EXP38         0x0a0f2090 /* IP PHONE Cable Length Status Register */
#define BRPHY4_GPHY_CORE_EXP42         0x0a0f20a2 /* Operating Mode Status */
#define BRPHY4_GPHY_CORE_EXP5F         0x0a0f20be /* PLL Frequency Offset Testmode Control */
#define BRPHY4_GPHY_CORE_EXP70         0x0a0f20e0 /* SOFT-RESET */
#define BRPHY4_GPHY_CORE_EXP71         0x0a0f20e2 /* Serial LED Control 1 */
#define BRPHY4_GPHY_CORE_EXP72         0x0a0f20e4 /* Serial LED Control 2 */
#define BRPHY4_GPHY_CORE_EXP73         0x0a0f20e6 /* LED Gating 2 (Used for dual-media applications) */
#define BRPHY4_GPHY_CORE_EXP74         0x0a0f20e8 /* LED Programmable Current Mode Control */
#define BRPHY4_GPHY_CORE_EXP75         0x0a0f20ea /* CED LED Error Mask */
#define BRPHY4_GPHY_CORE_EXP78         0x0a0f20f0 /* Misc Extended Control */
#define BRPHY4_GPHY_CORE_EXP7B         0x0a0f20f6 /* I2C Control */
#define BRPHY4_GPHY_CORE_EXP7C         0x0a0f20f8 /* I2C Status */
#define BRPHY4_GPHY_CORE_EXP7F         0x0a0f20fe /* External MACSec Interface Control */
#define BRPHY4_GPHY_CORE_ALIAS_18      0x0a0f2100 /* Alias to MII Reg 18 */
#define BRPHY4_GPHY_CORE_ALIAS_19      0x0a0f2102 /* Alias to MII Reg 19 */
#define BRPHY4_GPHY_CORE_ALIAS_1A      0x0a0f2104 /* Alias to MII Reg 1a */
#define BRPHY4_GPHY_CORE_ALIAS_1B      0x0a0f2106 /* Alias to MII Reg 1b */
#define BRPHY4_GPHY_CORE_ALIAS_1C      0x0a0f2108 /* Alias to MII Reg 1c */
#define BRPHY4_GPHY_CORE_ALIAS_1D      0x0a0f210a /* Alias to MII Reg 1d */
#define BRPHY4_GPHY_CORE_REG_MAP_CTL   0x0a0f210e /* MII Registers 10-1D mapping control */
#define BRPHY4_GPHY_CORE_EXP98         0x0a0f2130 /* First Slice of Quad-GPHY only): CAL-BIAS Status */
#define BRPHY4_GPHY_CORE_EXP9C         0x0a0f2138 /* SMII Control */
#define BRPHY4_GPHY_CORE_BT_LINK_FIX   0x0a0f214a /* 10BT LINK FIX Register */
#define BRPHY4_GPHY_CORE_SYNCE_PLUS_DBG 0x0a0f214c /* SyncE+ Debug */
#define BRPHY4_GPHY_CORE_SYNCE_PLUS    0x0a0f214e /* SyncE+ Status and Control */
#define BRPHY4_GPHY_CORE_EXPA8         0x0a0f2150 /* ADAPTIVE BIAS CONTROL */
#define BRPHY4_GPHY_CORE_EXPA9         0x0a0f2152 /* spare register */
#define BRPHY4_GPHY_CORE_EXPAA         0x0a0f2154 /* EEE Statistic timer 12hours lpi */
#define BRPHY4_GPHY_CORE_EXPAB         0x0a0f2156 /* EEE Statistic timer 12hours local */
#define BRPHY4_GPHY_CORE_EXPAC         0x0a0f2158 /* EEE Statistic loc lpi req 0_to_1 counter */
#define BRPHY4_GPHY_CORE_EXPAD         0x0a0f215a /* EEE Statistic rem lpi_req 0_to_1 counter */
#define BRPHY4_GPHY_CORE_EXPAE         0x0a0f215c /* spare register */
#define BRPHY4_GPHY_CORE_EXPAF         0x0a0f215e /* EEE Statistic counters ctrl/status */
#define BRPHY4_GPHY_CORE_EXPB0         0x0a0f2160 /* Bias Control 0 */
#define BRPHY4_GPHY_CORE_EXPB1         0x0a0f2162 /* Bias Control 1 */
#define BRPHY4_GPHY_CORE_EXPB2         0x0a0f2164 /* Bias Control 2 */
#define BRPHY4_GPHY_CORE_EXPE3         0x0a0f2166 /* TX PCS Delay 1000BT and 100TX (copper side) */
#define BRPHY4_GPHY_CORE_EXPE4         0x0a0f2168 /* TX PCS Delay 10BT (copper side) */
#define BRPHY4_GPHY_CORE_EXPE5         0x0a0f216a /* TX PCS Delay (fiber side) */
#define BRPHY4_GPHY_CORE_EXPE6         0x0a0f216c /* RX PCS Delay 1000BT and 100TX (copper side) */
#define BRPHY4_GPHY_CORE_EXPE7         0x0a0f216e /* RX PCS Delay 10BT (copper side) */
#define BRPHY4_GPHY_CORE_EXPE8         0x0a0f2170 /* RX PCS Delay (fiber side) */
#define BRPHY4_GPHY_CORE_EXPE9         0x0a0f2172 /* P1588 TX/RX Cycle Delay */
#define BRPHY4_GPHY_CORE_EXPE0         0x0a0f2174 /* TX PMA/PMD Delay (copper side) */
#define BRPHY4_GPHY_CORE_EXPE1         0x0a0f2176 /* TX PMA/PMD Delay (fiber side) */
#define BRPHY4_GPHY_CORE_EXPE2         0x0a0f2178 /* RX PMA/PMD Delay (copper side) */
#define BRPHY4_GPHY_CORE_EXPEA         0x0a0f217a /* TX/RX Adjustable Cycle Delay */
#define BRPHY4_GPHY_CORE_LED_PRA_MODE  0x0a0f2180 /* LED Proportional Rate Activity Control */
#define BRPHY4_GPHY_CORE_FIFO_CTL      0x0a0f2182 /* FIFO Control Register */
#define BRPHY4_GPHY_CORE_EXPD8         0x0a0f21b0 /* Halting agc/enc ctrl reg */
#define BRPHY4_GPHY_CORE_EXPF0         0x0a0f21e0 /* RGMII IBS Control */
#define BRPHY4_GPHY_CORE_EXPF5         0x0a0f21ea /* Time Sync */
#define BRPHY4_GPHY_CORE_EXPF6         0x0a0f21ec /* Analog Power Control Status */
#define BRPHY4_GPHY_CORE_EXPF7         0x0a0f21ee /* Auto-power Down Control Status */
#define BRPHY4_GPHY_CORE_EXPF8         0x0a0f21f0 /* Trim Settings from Fuse & to Bias Block */
#define BRPHY4_GPHY_CORE_EXPF9         0x0a0f21f2 /* reserved Register Bits */
#define BRPHY4_GPHY_CORE_EXPFA         0x0a0f21f4 /* Hidden Identifier */
#define BRPHY4_GPHY_CORE_EXPFB         0x0a0f21f6 /* TDR Override Values */
#define BRPHY4_GPHY_CORE_EXPFC         0x0a0f21f8 /*  */
#define BRPHY4_GPHY_CORE_EXPFD         0x0a0f21fa /* Clock gating control override value */
#define BRPHY4_GPHY_CORE_EXPFE         0x0a0f21fc /* Clock gating control override enable */
#define BRPHY4_GPHY_CORE_EXPFF         0x0a0f21fe /* Analog power control override */


/****************************************************************************
 * bcm89530_top_bridge_brphys_brphys4_BRPHY4_DSP_TAP
 ***************************************************************************/
#define BRPHY4_DSP_TAP_TAP0_C0         0x0a0f2200 /* AGC Control/Status Register A (x4) */
#define BRPHY4_DSP_TAP_TAP0_C1         0x0a0f2202 /* AGC Control/Status Register A (x4) */
#define BRPHY4_DSP_TAP_TAP0_C2         0x0a0f2204 /* AGC Control/Status Register A (x4) */
#define BRPHY4_DSP_TAP_TAP0_C3         0x0a0f2206 /* AGC Control/Status Register A (x4) */
#define BRPHY4_DSP_TAP_TAP1            0x0a0f2208 /* IPRF Control register (x1) */
#define BRPHY4_DSP_TAP_TAP2_C0         0x0a0f2210 /* MSE Status Register (x4) */
#define BRPHY4_DSP_TAP_TAP2_C1         0x0a0f2212 /* MSE Status Register (x4) */
#define BRPHY4_DSP_TAP_TAP2_C2         0x0a0f2214 /* MSE Status Register (x4) */
#define BRPHY4_DSP_TAP_TAP2_C3         0x0a0f2216 /* MSE Status Register (x4) */
#define BRPHY4_DSP_TAP_TAP3_C0         0x0a0f2218 /* Soft Decision (From DFE) Status Register (x4) */
#define BRPHY4_DSP_TAP_TAP3_C1         0x0a0f221a /* Soft Decision (From DFE) Status Register (x4) */
#define BRPHY4_DSP_TAP_TAP3_C2         0x0a0f221c /* Soft Decision (From DFE) Status Register (x4) */
#define BRPHY4_DSP_TAP_TAP3_C3         0x0a0f221e /* Soft Decision (From DFE) Status Register (x4) */
#define BRPHY4_DSP_TAP_TAP4_C0         0x0a0f2220 /* Phase Status Register and Coarse AGC High Gear (x4) */
#define BRPHY4_DSP_TAP_TAP4_C1         0x0a0f2222 /* Phase Status Register and Coarse AGC High Gear (x4) */
#define BRPHY4_DSP_TAP_TAP4_C2         0x0a0f2224 /* Phase Status Register and Coarse AGC High Gear (x4) */
#define BRPHY4_DSP_TAP_TAP4_C3         0x0a0f2226 /* Phase Status Register and Coarse AGC High Gear (x4) */
#define BRPHY4_DSP_TAP_TAP5_C0         0x0a0f2228 /* Wire Map/Skew Control/Status and Echo/NEXT Control and Transmitter Control and ADC Control Register and Slicer Zero (x4) */
#define BRPHY4_DSP_TAP_TAP5_C1         0x0a0f222a /* Wire Map/Skew Control/Status and Echo/NEXT Control and Transmitter Control and ADC Control Register and Slicer Zero (x4) */
#define BRPHY4_DSP_TAP_TAP5_C2         0x0a0f222c /* Wire Map/Skew Control/Status and Echo/NEXT Control and Transmitter Control and ADC Control Register and Slicer Zero (x4) */
#define BRPHY4_DSP_TAP_TAP5_C3         0x0a0f222e /* Wire Map/Skew Control/Status and Echo/NEXT Control and Transmitter Control and ADC Control Register and Slicer Zero (x4) */
#define BRPHY4_DSP_TAP_TAP6            0x0a0f2230 /* CFC Deadman Disable */
#define BRPHY4_DSP_TAP_TAP7_C0         0x0a0f2238 /* BIST TEST 0 */
#define BRPHY4_DSP_TAP_TAP7_C1         0x0a0f223a /* BIST TEST 1 */
#define BRPHY4_DSP_TAP_TAP7_C2         0x0a0f223c /* BIST TEST 2 */
#define BRPHY4_DSP_TAP_TAP8_C0         0x0a0f2240 /* ABIST TEST 0 */
#define BRPHY4_DSP_TAP_TAP8_C1         0x0a0f2242 /* ABIST TEST 1 */
#define BRPHY4_DSP_TAP_TAP8_C2         0x0a0f2244 /* ABIST TEST 2 */
#define BRPHY4_DSP_TAP_TAP8_C3         0x0a0f2246 /* BR HPF Control */
#define BRPHY4_DSP_TAP_TAP9            0x0a0f2248 /* Frequency Control/Status Register LSBs (x1) */
#define BRPHY4_DSP_TAP_TAP10           0x0a0f224a /* PLL Bandwidth Control/Status and Path Metric Reset Register (x1) */
#define BRPHY4_DSP_TAP_TAP11           0x0a0f224c /* PLL RCLK and TCLK Offset Freeze Register (x1) */
#define BRPHY4_DSP_TAP_TAP12_C0        0x0a0f2250 /* PLL ACLK Offset/Freeze Register (x4) */
#define BRPHY4_DSP_TAP_TAP12_C1        0x0a0f2252 /* PLL ACLK Offset/Freeze Register (x4) */
#define BRPHY4_DSP_TAP_TAP12_C2        0x0a0f2254 /* PLL ACLK Offset/Freeze Register (x4) */
#define BRPHY4_DSP_TAP_TAP12_C3        0x0a0f2256 /* PLL ACLK Offset/Freeze Register (x4) */
#define BRPHY4_DSP_TAP_TAP13           0x0a0f2258 /* HPF Bandwidth Control and Disable ADC LSBs (x1) */
#define BRPHY4_DSP_TAP_TAP14           0x0a0f225a /* MSE Threshold Register  #1 (x1) */
#define BRPHY4_DSP_TAP_TAP15           0x0a0f225c /* MSE Threshold Register #2 (x1) */
#define BRPHY4_DSP_TAP_TAP16_C0        0x0a0f2260 /* Logic Analyzer trigger delay (x1) */
#define BRPHY4_DSP_TAP_TAP16_C1        0x0a0f2262 /* BIST CRC Monitor (x4) */
#define BRPHY4_DSP_TAP_TAP16_C2        0x0a0f2264 /* BIST CRC Monitor (x4) */
#define BRPHY4_DSP_TAP_TAP16_C3        0x0a0f2266 /* BIST CRC Monitor (x4) */
#define BRPHY4_DSP_TAP_TAP17_C0        0x0a0f2268 /* Testmode testvalue (aliased with logic analyzer state selects) */
#define BRPHY4_DSP_TAP_TAP17_C1        0x0a0f226a /* Testmode and logic analyzer controls #1 */
#define BRPHY4_DSP_TAP_TAP17_C2        0x0a0f226c /* Logic analyzer controls #2 */
#define BRPHY4_DSP_TAP_TAP17_C3        0x0a0f226e /* Testmode and logic analyzer controls #3 */
#define BRPHY4_DSP_TAP_TAP18_C0        0x0a0f2270 /* Peak Noise detector (x4) */
#define BRPHY4_DSP_TAP_TAP18_C1        0x0a0f2272 /* Peak Noise detector (x4) */
#define BRPHY4_DSP_TAP_TAP18_C2        0x0a0f2274 /* Peak Noise detector (x4) */
#define BRPHY4_DSP_TAP_TAP18_C3        0x0a0f2276 /* Peak Noise detector (x4) */
#define BRPHY4_DSP_TAP_TAP20           0x0a0f2278 /* Echo Minimum Length and LMS/FIR delay adjustments (x1) */
#define BRPHY4_DSP_TAP_TAP21           0x0a0f227a /* Phy Control Monitors #1 (x1) */
#define BRPHY4_DSP_TAP_TAP22           0x0a0f227c /* Phy Control Monitors #2 (x1) */
#define BRPHY4_DSP_TAP_TAP23           0x0a0f227e /* Phy Control Monitors #3 (x1) */
#define BRPHY4_DSP_TAP_TAP24           0x0a0f2280 /* Phy Control Output Overrides #1 (x1) */
#define BRPHY4_DSP_TAP_TAP25           0x0a0f2282 /* Phy Control Output Overrides #2 (x1) */
#define BRPHY4_DSP_TAP_TAP26           0x0a0f2284 /* Phy Control Input Overrides #1 (x1) */
#define BRPHY4_DSP_TAP_TAP27           0x0a0f2286 /* Phy Control Input Overrides #2 (x1) */
#define BRPHY4_DSP_TAP_TAP28           0x0a0f2288 /* Phy Control Output Overrides #3 (x1) */
#define BRPHY4_DSP_TAP_TAP29           0x0a0f228a /* Phy Control Force State/Timers/Alternate Behaviour Register #1 (x1) */
#define BRPHY4_DSP_TAP_TAP30           0x0a0f228c /* Phy Control Force State/Timers/Alternate Behaviour Register #2 (x1) */
#define BRPHY4_DSP_TAP_TAP31_C0        0x0a0f2290 /* Channel Swap Override */
#define BRPHY4_DSP_TAP_TAP32_C0        0x0a0f2298 /* Transmit Testmode Sync Generation (x1) */
#define BRPHY4_DSP_TAP_FDFE_OV_RD      0x0a0f229a /* FDFE Override/Read Control Register */
#define BRPHY4_DSP_TAP_FDFE_COEFF      0x0a0f229c /* FDFE Coefficient Read Back Register */
#define BRPHY4_DSP_TAP_FDFE_BETA_THRESHOLD 0x0a0f229e /* FDFE Beta Threshold Control */
#define BRPHY4_DSP_TAP_TAP33_C0        0x0a0f22a0 /* eee dsp test */
#define BRPHY4_DSP_TAP_TAP33_C1        0x0a0f22a2 /* eee sigdet */
#define BRPHY4_DSP_TAP_TAP33_C2        0x0a0f22a4 /* eee_lpi_timers */
#define BRPHY4_DSP_TAP_TAP33_C3        0x0a0f22a6 /* spare register */
#define BRPHY4_DSP_TAP_TAP34_C0        0x0a0f22a8 /* eee frequency control */
#define BRPHY4_DSP_TAP_TAP34_C1        0x0a0f22aa /* eee Gigabit Mode BW control */
#define BRPHY4_DSP_TAP_TAP34_C2        0x0a0f22ac /* eee 100TX Mode BW control */
#define BRPHY4_DSP_TAP_TAP34_C3        0x0a0f22ae /* phasectl TPO monitor */
#define BRPHY4_DSP_TAP_TAP35_C0        0x0a0f22b0 /* eee 100Base-tx timer control 1 */
#define BRPHY4_DSP_TAP_TAP35_C1        0x0a0f22b2 /* eee 100Base-tx timer control 2 */
#define BRPHY4_DSP_TAP_TAP35_C2        0x0a0f22b4 /* eee 100Base-tx timer misc control */
#define BRPHY4_DSP_TAP_TAP35_C3        0x0a0f22b6 /* pcs_lpi_test */
#define BRPHY4_DSP_TAP_FILTER_CTL_CH0  0x0a0f22b8 /* Filter Freeze/Disable per channel Control */
#define BRPHY4_DSP_TAP_FILTER_CTL_CH1  0x0a0f22ba /* Filter Freeze/Disable per channel Control */
#define BRPHY4_DSP_TAP_FILTER_CTL_CH2  0x0a0f22bc /* Filter Freeze/Disable per channel Control */
#define BRPHY4_DSP_TAP_FILTER_CTL_CH3  0x0a0f22be /* Filter Freeze/Disable per channel Control */
#define BRPHY4_DSP_TAP_FILTER_CTL_FFEX_CH0 0x0a0f22c0 /* FFEX Freeze/Disable per channel Control */
#define BRPHY4_DSP_TAP_FILTER_CTL_FFEX_CH1 0x0a0f22c2 /* FFEX Freeze/Disable per channel Control */
#define BRPHY4_DSP_TAP_FILTER_CTL_FFEX_CH2 0x0a0f22c4 /* FFEX Freeze/Disable per channel Control */
#define BRPHY4_DSP_TAP_FILTER_CTL_FFEX_CH3 0x0a0f22c6 /* FFEX Freeze/Disable per channel Control */
#define BRPHY4_DSP_TAP_EMI_DATAPATH_CTL 0x0a0f22c8 /* EMI Datapath Control */
#define BRPHY4_DSP_TAP_EMI_DATAPATH_CTL2 0x0a0f22ca /* EMI Datapath Control2 */
#define BRPHY4_DSP_TAP_FFEX_CTL        0x0a0f22cc /* FFE X-tap Control */
#define BRPHY4_DSP_TAP_PHYC_BREAKPOINT_CTL0 0x0a0f22ce /* Phycontrol Breakpoint Control 0 */
#define BRPHY4_DSP_TAP_PHYC_BREAKPOINT_CTL1 0x0a0f22d0 /* Phycontrol Breakpoint Control 1 */
#define BRPHY4_DSP_TAP_FILTER_ADDR     0x0a0f2360 /* DSP Coefficient Address Register */
#define BRPHY4_DSP_TAP_FILTER_CTL      0x0a0f2362 /* DSP Control Register */
#define BRPHY4_DSP_TAP_FILTER_DATA     0x0a0f2364 /* DSP Coefficient Read/Write Port */


/****************************************************************************
 * bcm89530_top_bridge_brphys_brphys4_BRPHY4_PLL_CTRL
 ***************************************************************************/
#define BRPHY4_PLL_CTRL_PLLCTRL_0      0x0a0f2390 /* Analog pll control 0 */
#define BRPHY4_PLL_CTRL_PLLCTRL_1      0x0a0f2392 /* Analog pll control 1 */
#define BRPHY4_PLL_CTRL_PLLCTRL_2      0x0a0f2394 /* Analog pll control 2 */
#define BRPHY4_PLL_CTRL_PLLCTRL_3      0x0a0f2396 /* Analog pll control 3 */
#define BRPHY4_PLL_CTRL_PLLCTRL_4      0x0a0f2398 /* Analog pll control 4 */
#define BRPHY4_PLL_CTRL_PLLCTRL_5      0x0a0f239a /* Analog pll control 5 */
#define BRPHY4_PLL_CTRL_PLLCTRL_6      0x0a0f239c /* Analog pll control 6 */
#define BRPHY4_PLL_CTRL_PLL_STATUS_0   0x0a0f23a0 /* Analog PLL Status 0 */
#define BRPHY4_PLL_CTRL_PLL_STATUS_1   0x0a0f23a2 /* Analog PLL Status 1 */
#define BRPHY4_PLL_CTRL_AFE_SIGDET_STATUS 0x0a0f23a4 /* AFE Signal detect */
#define BRPHY4_PLL_CTRL_PLLCTRL_7      0x0a0f23a6 /* Analog pll control 7 */
#define BRPHY4_PLL_CTRL_PLLCTRL_8      0x0a0f23a8 /* Analog pll control 8 */


/****************************************************************************
 * bcm89530_top_bridge_brphys_brphys4_BRPHY4_AFE_CTRL
 ***************************************************************************/
#define BRPHY4_AFE_CTRL_RXCONFIG_0     0x0a0f23c0 /* RXCONFIG 15:0 */
#define BRPHY4_AFE_CTRL_RXCONFIG_1     0x0a0f23c2 /* RXCONFIG 31:16 */
#define BRPHY4_AFE_CTRL_RXCONFIG_2     0x0a0f23c4 /* RXCONFIG 47:32 */
#define BRPHY4_AFE_CTRL_RXCONFIG_3     0x0a0f23c6 /* RXCONFIG 63:48 */
#define BRPHY4_AFE_CTRL_RXCONFIG_4     0x0a0f23c8 /* RXCONFIG 79:64 */
#define BRPHY4_AFE_CTRL_RXCONFIG5_LP   0x0a0f23ca /* RXCONFIG 86:80 and LP tuning */
#define BRPHY4_AFE_CTRL_TX_CONFIG_0    0x0a0f23cc /* TXCONFIG 15:0 */
#define BRPHY4_AFE_CTRL_TX_CONFIG_1    0x0a0f23ce /* TXCONFIG 31:16 */
#define BRPHY4_AFE_CTRL_VDAC_ICTRL_0   0x0a0f23d0 /* VDAC CURRENT Control 15:0 */
#define BRPHY4_AFE_CTRL_VDAC_ICTRL_1   0x0a0f23d2 /* VDAC CURRENT Control 31:16 */
#define BRPHY4_AFE_CTRL_VDAC_ICTRL_2   0x0a0f23d4 /* VDAC CURRENT Control 51:36 */
#define BRPHY4_AFE_CTRL_VDAC_OTHERS_0  0x0a0f23d6 /* VDAC CURRENT 35:32 and others */
#define BRPHY4_AFE_CTRL_HPF_TRIM_OTHERS 0x0a0f23d8 /* HPF trim and reserved bits */
#define BRPHY4_AFE_CTRL_TX_EXTRA_CONFIG_0 0x0a0f23da /* TXCONFIG 15:0 */
#define BRPHY4_AFE_CTRL_TX_EXTRA_CONFIG_1 0x0a0f23dc /* TXCONFIG 15:0 */
#define BRPHY4_AFE_CTRL_TX_EXTRA_CONFIG_2 0x0a0f23de /* TXCONFIG 15:0 */
#define BRPHY4_AFE_CTRL_TEMPSEN_OTHERS 0x0a0f23e0 /* TEMPSEN_OTHERS 15:0 */
#define BRPHY4_AFE_CTRL_FUTURE_RSV     0x0a0f23e2 /* FUTURE_RSV 15:0 */


/****************************************************************************
 * bcm89530_top_bridge_brphys_brphys4_BRPHY4_ECD_CTRL
 ***************************************************************************/
#define BRPHY4_ECD_CTRL_EXPC0          0x0a0f2540 /* ECD Control and Status */
#define BRPHY4_ECD_CTRL_EXPC1          0x0a0f2542 /* ECD Fault Type */
#define BRPHY4_ECD_CTRL_EXPC2          0x0a0f2544 /* ECD Pair A Length Results */
#define BRPHY4_ECD_CTRL_EXPC3          0x0a0f2546 /* ECD Pair B Length Results */
#define BRPHY4_ECD_CTRL_EXPC4          0x0a0f2548 /* ECD Pair C Length Results */
#define BRPHY4_ECD_CTRL_EXPC5          0x0a0f254a /* ECD Pair D Length Results */
#define BRPHY4_ECD_CTRL_EXPC6          0x0a0f254c /* ECD XTALK Map */
#define BRPHY4_ECD_CTRL_EXPC7          0x0a0f254e /* ECD EXTRA RESERVED REGISTER */
#define BRPHY4_ECD_CTRL_EXPC8          0x0a0f2550 /* ECD EXTRA RESERVED REGISTER */
#define BRPHY4_ECD_CTRL_EXPC9          0x0a0f2552 /* ECD EXTRA RESERVED REGISTER */
#define BRPHY4_ECD_CTRL_EXPCA          0x0a0f2554 /* ECD EXTRA RESERVED REGISTER */
#define BRPHY4_ECD_CTRL_EXPCB          0x0a0f2556 /* ECD EXTRA RESERVED REGISTER */
#define BRPHY4_ECD_CTRL_EXPCC          0x0a0f2558 /* ECD EXTRA RESERVED REGISTER */
#define BRPHY4_ECD_CTRL_EXPCD          0x0a0f255a /* ECD EXTRA RESERVED REGISTER */
#define BRPHY4_ECD_CTRL_EXPCE          0x0a0f255c /* ECD EXTRA RESERVED REGISTER */
#define BRPHY4_ECD_CTRL_EXPCF          0x0a0f255e /* ECD EXTRA RESERVED REGISTER */
#define BRPHY4_ECD_CTRL_EXPE0          0x0a0f2560 /* ECD EXTRA RESERVED REGISTER */
#define BRPHY4_ECD_CTRL_EXPE1          0x0a0f2562 /* ECD EXTRA RESERVED REGISTER */
#define BRPHY4_ECD_CTRL_EXPE2          0x0a0f2564 /* ECD EXTRA RESERVED REGISTER */
#define BRPHY4_ECD_CTRL_EXPE3          0x0a0f2566 /* ECD EXTRA RESERVED REGISTER */
#define BRPHY4_ECD_CTRL_EXPE4          0x0a0f2568 /* ECD EXTRA RESERVED REGISTER */
#define BRPHY4_ECD_CTRL_EXPE5          0x0a0f256a /* ECD EXTRA RESERVED REGISTER */
#define BRPHY4_ECD_CTRL_EXPE6          0x0a0f256c /* ECD EXTRA RESERVED REGISTER */
#define BRPHY4_ECD_CTRL_EXPE7          0x0a0f256e /* ECD EXTRA RESERVED REGISTER */
#define BRPHY4_ECD_CTRL_EXPE8          0x0a0f2570 /* ECD EXTRA RESERVED REGISTER */
#define BRPHY4_ECD_CTRL_EXPE9          0x0a0f2572 /* ECD EXTRA RESERVED REGISTER */
#define BRPHY4_ECD_CTRL_EXPEA          0x0a0f2574 /* ECD EXTRA RESERVED REGISTER */
#define BRPHY4_ECD_CTRL_EXPEB          0x0a0f2576 /* ECD EXTRA RESERVED REGISTER */
#define BRPHY4_ECD_CTRL_EXPEC          0x0a0f2578 /* ECD EXTRA RESERVED REGISTER */
#define BRPHY4_ECD_CTRL_EXPED          0x0a0f257a /* ECD EXTRA RESERVED REGISTER */
#define BRPHY4_ECD_CTRL_EXPEE          0x0a0f257c /* ECD EXTRA RESERVED REGISTER */
#define BRPHY4_ECD_CTRL_EXPEF          0x0a0f257e /* ECD EXTRA RESERVED REGISTER */


/****************************************************************************
 * bcm89530_top_bridge_brphys_brphys4_BRPHY4_BR_CTRL
 ***************************************************************************/
#define BRPHY4_BR_CTRL_EXP90           0x0a0f2600 /* BroadReach LRE Misc Control */
#define BRPHY4_BR_CTRL_EXP91           0x0a0f2602 /* BroadReach LRE Misc Control */
#define BRPHY4_BR_CTRL_EXP92           0x0a0f2604 /* BroadReach LRE Misc Control */
#define BRPHY4_BR_CTRL_EXP93           0x0a0f2606 /* BroadReach LDS  Control */
#define BRPHY4_BR_CTRL_EXP94           0x0a0f2608 /* BroadReach LDS RX Control */
#define BRPHY4_BR_CTRL_EXP95           0x0a0f260a /* BroadReach LDS RX Control */
#define BRPHY4_BR_CTRL_EXP96           0x0a0f260c /* BroadReach LDS Scan, ARB and TX Status */
#define BRPHY4_BR_CTRL_EXP97           0x0a0f260e /* BroadReach LDS Scan, ARB and TX Status */
#define BRPHY4_BR_CTRL_EXP99           0x0a0f2612 /* BroadReach LDS Timer Control */
#define BRPHY4_BR_CTRL_EXP9A           0x0a0f2614 /* LDS Status */
#define BRPHY4_BR_CTRL_EXP9B           0x0a0f2616 /* BroadR-Reach PLL Control */
#define BRPHY4_BR_CTRL_EXP9D           0x0a0f261a /* EoC Internal Control 1 */
#define BRPHY4_BR_CTRL_EXP9E           0x0a0f261c /* LDS Length Threshold 0 */
#define BRPHY4_BR_CTRL_EXP9F           0x0a0f261e /* LDS Length Threshold 1 */
#define BRPHY4_BR_CTRL_EXPA0           0x0a0f2620 /* HLDS register, LDS extend advertisement register */
#define BRPHY4_BR_CTRL_EXPA1           0x0a0f2622 /* HLDS register, LDS link partner extend ability register */
#define BRPHY4_BR_CTRL_EXPA2           0x0a0f2624 /* HLDS Register */
#define BRPHY4_BR_CTRL_BR_MISC_CONTROL_STATUS 0x0a0f2626 /* Broadreach Misc Status */
#define BRPHY4_BR_CTRL_BR250_CTL       0x0a0f263c /* BR250 Control */


/****************************************************************************
 * bcm89530_top_bridge_brphys_brphys4_BRPHY4_BR_CL22_IEEE
 ***************************************************************************/
#define BRPHY4_BR_CL22_IEEE_MII_CTRL   0x0a0fffc0 /* BR_LRE_Control_Register */
#define BRPHY4_BR_CL22_IEEE_MII_STAT   0x0a0fffc2 /* BR_LRE_Status_Register */
#define BRPHY4_BR_CL22_IEEE_PHY_ID_MSB 0x0a0fffc4 /* PHY_Identifier_MSB_Register */
#define BRPHY4_BR_CL22_IEEE_PHY_ID_LSB 0x0a0fffc6 /* PHY_Identifier_LSB_Register */
#define BRPHY4_BR_CL22_IEEE_LDS_ADV_ABILITY_BP 0x0a0fffc8 /* LDS_Advertised_Ability_Register (Base Page) */
#define BRPHY4_BR_CL22_IEEE_LDS_ADV_CONTROL 0x0a0fffca /* LDS_Advertised_Control_Register */
#define BRPHY4_BR_CL22_IEEE_LDS_ABILITY 0x0a0fffcc /* LDS_Ability_Register (Next Page) */
#define BRPHY4_BR_CL22_IEEE_LDS_LP_ABILITY_BP 0x0a0fffce /* LDS_Link_Partner_Ability_Base_Page_Register (Base Page) */
#define BRPHY4_BR_CL22_IEEE_LDS_LP_MSG_NXTP 0x0a0fffd0 /* LDS_Link_Partners_Nxt_Pg_Msg_Register (Next Page) */
#define BRPHY4_BR_CL22_IEEE_LDS_LP_ABILITY_NXTP 0x0a0fffd2 /* LDS_Link_Partner_Ability_Nxt_Pg_Register (Next Page) */
#define BRPHY4_BR_CL22_IEEE_LDS_LDS_EXP 0x0a0fffd4 /* LDS_Expansion_Register */
#define BRPHY4_BR_CL22_IEEE_LRE_EXTENDED_STAT 0x0a0fffd6 /* IEEE_Extended_Status_Register */


/****************************************************************************
 * bcm89530_top_bridge_sgmii_x3_SGMII_LANE0_SGMII0_CL22_B0
 ***************************************************************************/
#define SGMII0_CL22_B0_MIICNTL         0x0a400000 /* IEEE MII control register */
#define SGMII0_CL22_B0_MIISTAT         0x0a400002 /* IEEE MII status register */
#define SGMII0_CL22_B0_ID1             0x0a400004 /* IEEE phy ID LSByte register */
#define SGMII0_CL22_B0_ID2             0x0a400006 /* IEEE phy ID MSByte register */
#define SGMII0_CL22_B0_AUTONEGADV      0x0a400008 /* IEEE auto-negotiation advertised abilities register */
#define SGMII0_CL22_B0_AUTONEGLPABIL   0x0a40000a /* IEEE auto-negotiation link partner abilities register */
#define SGMII0_CL22_B0_AUTONEGEXP      0x0a40000c /* IEEE auto-negotiation expansion register */
#define SGMII0_CL22_B0_AUTONEGNP       0x0a40000e /* IEEE auto-negotiation next page register */
#define SGMII0_CL22_B0_AUTONEGLPABIL2  0x0a400010 /* IEEE auto-negotiation link partner next page register */
#define SGMII0_CL22_B0_MIIEXTSTAT      0x0a40001e /* IEEE MII extended status register */


/****************************************************************************
 * bcm89530_top_bridge_sgmii_x3_SGMII_LANE0_SGMII_Blk0
 ***************************************************************************/
#define SGMII_BLK0_XGXSCONTROL         0x0a410000 /* XGXS control register */
#define SGMII_BLK0_MMDSELECT           0x0a41001a /* MMD select register */
#define SGMII_BLK0_MISCCONTROL1        0x0a41001c /* Miscellaneous control 1 register */
#define SGMII_BLK0_BLOCKADDRESS        0x0a41001e /* Block Address register */


/****************************************************************************
 * bcm89530_top_bridge_sgmii_x3_SGMII_LANE0_SGMII_Blk1
 ***************************************************************************/
#define SGMII_BLK1_LANECTRL0           0x0a41002a /* Lane control 0 register */
#define SGMII_BLK1_LANECTRL1           0x0a41002c /* Lane control 1 register */
#define SGMII_BLK1_LANECTRL2           0x0a41002e /* Lane control 2 register */
#define SGMII_BLK1_LANECTRL3           0x0a410030 /* Lane control 3 register */
#define SGMII_BLK1_LANEPRBS            0x0a410032 /* Lane PRBS control register */
#define SGMII_BLK1_LANETEST            0x0a410034 /* Lane test control register */
#define SGMII_BLK1_BLOCKADDRESS        0x0a41003e /* Block Address register */


/****************************************************************************
 * bcm89530_top_bridge_sgmii_x3_SGMII_LANE0_SGMII_PLL_afe
 ***************************************************************************/
#define SGMII_PLL_AFE_CTRL0            0x0a4100a0 /* Analog PLL Control0 Register */
#define SGMII_PLL_AFE_CTRL1            0x0a4100a2 /* Analog PLL Control1 Register */
#define SGMII_PLL_AFE_CTRL2            0x0a4100a4 /* Analog PLL Control2 Register */
#define SGMII_PLL_AFE_CTRL3            0x0a4100a6 /* Analog PLL Control3 Register */
#define SGMII_PLL_AFE_CTRL4            0x0a4100a8 /* Analog PLL Control4 Register */
#define SGMII_PLL_AFE_CTRL5            0x0a4100aa /* Analog PLL Control5 Register */
#define SGMII_PLL_AFE_CTRL6            0x0a4100ac /* Analog PLL Control6 Register */
#define SGMII_PLL_AFE_CTRL7            0x0a4100ae /* Analog PLL Control7 Register */
#define SGMII_PLL_AFE_CTRL8            0x0a4100b0 /* Analog PLL Control8 Register */


/****************************************************************************
 * bcm89530_top_bridge_sgmii_x3_SGMII_LANE0_SGMII0_TX_afe
 ***************************************************************************/
#define SGMII0_TX_AFE_ANATXASTATUS0    0x0a4100c0 /* Tx analog status 0 register */
#define SGMII0_TX_AFE_ANATXACONTROL0   0x0a4100c2 /* Tx analog control 0 register */
#define SGMII0_TX_AFE_ANATXMDATA0      0x0a4100c4 /* Tx test mux data 0 register */
#define SGMII0_TX_AFE_ANATXMDATA1      0x0a4100c6 /* Tx test mux data 1 register */
#define SGMII0_TX_AFE_CONTROL0         0x0a4100ca /* Analog TX Control 0 Register */
#define SGMII0_TX_AFE_CONTROL1         0x0a4100cc /* Analog TX Control 1 Register */
#define SGMII0_TX_AFE_CONTROL2         0x0a4100ce /* Analog TX Control 2 Register */
#define SGMII0_TX_AFE_CONTROL3         0x0a4100d0 /* Analog TX Control 3 Register */
#define SGMII0_TX_AFE_INTERP           0x0a4100d2 /* txinterp Control Register */


/****************************************************************************
 * bcm89530_top_bridge_sgmii_x3_SGMII_LANE0_SGMII0_RX_afe
 ***************************************************************************/
#define SGMII0_RX_AFE_ANARXSTATUS      0x0a410160 /* Rx lane status register */
#define SGMII0_RX_AFE_ANARXCONTROL     0x0a410162 /* Rx lane control register */
#define SGMII0_RX_AFE_CTRL0            0x0a410164 /* AFE Control Register 15:0 */
#define SGMII0_RX_AFE_CTRL1            0x0a410166 /* AFE Control Register 31:16 */
#define SGMII0_RX_AFE_ANARXSIGDET      0x0a410168 /* Rx Sigdet Control */
#define SGMII0_RX_AFE_CTRL2            0x0a41016a /* AFE Control Register 47:32 */
#define SGMII0_RX_AFE_CTRL3            0x0a41016c /* AFE Control Register 63:48 */
#define SGMII0_RX_AFE_CTRL4            0x0a41016e /* AFE Control Register 79:64 */
#define SGMII0_RX_AFE_ANARXTEST        0x0a410170 /* Rx lane control register */
#define SGMII0_RX_AFE_ANARXCONTROL1G   0x0a410172 /* Rx 1G Control register */
#define SGMII0_RX_AFE_ANARXCONTROLPCI  0x0a410174 /* Rx PCI Control register */
#define SGMII0_RX_AFE_ANARXASTATUS     0x0a410176 /* Rx analog status register */
#define SGMII0_RX_AFE_CTRL5            0x0a410178 /* AFE Control Register 95:80 */
#define SGMII0_RX_AFE_CTRL6            0x0a41017a /* AFE Control Register 111:96 */


/****************************************************************************
 * bcm89530_top_bridge_sgmii_x3_SGMII_LANE0_SGMII_Blk2
 ***************************************************************************/
#define SGMII_BLK2_TESTMODELANE        0x0a41020c /* Test mode lane select register */
#define SGMII_BLK2_TESTMODECOMBO       0x0a41020e /* Test mode monitor control register */
#define SGMII_BLK2_TESTMODEMUX         0x0a410210 /* Test mode mux control register */
#define SGMII_BLK2_CX4SIGDETCNT        0x0a410212 /* 10GBASE-CX4 signal detect timeout value */
#define SGMII_BLK2_LANERESET           0x0a410214 /* Lane reset register */


/****************************************************************************
 * bcm89530_top_bridge_sgmii_x3_SGMII_LANE0_SGMII_Blk4
 ***************************************************************************/
#define SGMII_BLK4_XGXSSTATUS1         0x0a410244 /* status 1 register */
#define SGMII_BLK4_XGXSSTATUS2         0x0a410246 /* status 2 register */
#define SGMII_BLK4_STATUS1000X1        0x0a410248 /* 1000X status 1 register */


/****************************************************************************
 * bcm89530_top_bridge_sgmii_x3_SGMII_LANE0_SGMII_Blk7
 ***************************************************************************/
#define SGMII_BLK7_PRBS_DECOUPLE       0x0a4102b4 /* prbs monitor decouple control */


/****************************************************************************
 * bcm89530_top_bridge_sgmii_x3_SGMII_LANE0_SGMII_PLL2
 ***************************************************************************/
#define SGMII_PLL2_STAT0               0x0a410300 /* Analog PLL Status0 Register */
#define SGMII_PLL2_CTRL1               0x0a410302 /* Analog PLL Control1 Register */
#define SGMII_PLL2_CTRL2               0x0a410304 /* Analog PLL Control2 Register */
#define SGMII_PLL2_CTRL3               0x0a410306 /* Analog PLL Control3 Register */
#define SGMII_PLL2_CTRL4               0x0a410308 /* Analog PLL Control4 Register */
#define SGMII_PLL2_CTRL5               0x0a41030a /* Analog PLL Control5 Register */
#define SGMII_PLL2_CTRL6               0x0a41030c /* Analog PLL Control6 Register */


/****************************************************************************
 * bcm89530_top_bridge_sgmii_x3_SGMII_LANE0_SGMII0_Digital
 ***************************************************************************/
#define SGMII0_DIGITAL_CONTROL1000X1   0x0a410600 /* 1000X control 1 register */
#define SGMII0_DIGITAL_CONTROL1000X2   0x0a410602 /* 1000X control 2 register */
#define SGMII0_DIGITAL_CONTROL1000X3   0x0a410604 /* 1000X control 3 register */
#define SGMII0_DIGITAL_CONTROL1000X4   0x0a410606 /* 1000X control 4 register */
#define SGMII0_DIGITAL_STATUS1000X1    0x0a410608 /* 1000X status 1 register */
#define SGMII0_DIGITAL_STATUS1000X2    0x0a41060a /* 1000X status 2 register */
#define SGMII0_DIGITAL_STATUS1000X3    0x0a41060c /* 1000X status 3 register */
#define SGMII0_DIGITAL_BADCODEGROUP    0x0a41060e /* Invalid code group count register */
#define SGMII0_DIGITAL_MISC1           0x0a410610 /* Miscellaneous 1 control register */
#define SGMII0_DIGITAL_MISC2           0x0a410612 /* Miscellaneous 2 control register */
#define SGMII0_DIGITAL_PATGENCTRL      0x0a410614 /* Pattern generator control register */
#define SGMII0_DIGITAL_PATGENSTAT      0x0a410616 /* Pattern generator status register */
#define SGMII0_DIGITAL_TESTMODE        0x0a410618 /* Test mode register */
#define SGMII0_DIGITAL_TXPKTCNT        0x0a41061a /* Tx packet count register */
#define SGMII0_DIGITAL_RXPKTCNT        0x0a41061c /* Rx packet count register */


/****************************************************************************
 * bcm89530_top_bridge_sgmii_x3_SGMII_LANE0_SGMII0_serdesID
 ***************************************************************************/
#define SGMII0_SERDESID_SERDESID0      0x0a410620 /* Serdes ID 0 register */
#define SGMII0_SERDESID_SERDESID1      0x0a410622 /* Serdes ID 1 register */
#define SGMII0_SERDESID_SERDESID2      0x0a410624 /* Serdes ID 2 register */
#define SGMII0_SERDESID_SERDESID3      0x0a410626 /* Serdes ID 3 register */


/****************************************************************************
 * bcm89530_top_bridge_sgmii_x3_SGMII_LANE0_SGMII0_Digital3
 ***************************************************************************/
#define SGMII0_DIGITAL3_TPOUT_1        0x0a41064e /* Test port out bits 15:0, tpout[15:0] */
#define SGMII0_DIGITAL3_TPOUT_2        0x0a410650 /* Test port out bits 23:8, tpout[23:8] */


/****************************************************************************
 * bcm89530_top_bridge_sgmii_x3_SGMII_LANE0_SGMII0_Digital4
 ***************************************************************************/
#define SGMII0_DIGITAL4_MISC3          0x0a410678 /* Miscellaneous 3 control register */
#define SGMII0_DIGITAL4_MISC5          0x0a41067c /* Miscelaneous 5 control register */


/****************************************************************************
 * bcm89530_top_bridge_sgmii_x3_SGMII_LANE0_SGMII0_FX100
 ***************************************************************************/
#define SGMII0_FX100_CONTROL1          0x0a410800 /* 100FX control register 1 */
#define SGMII0_FX100_CONTROL2          0x0a410802 /* 100FX control register 2 */
#define SGMII0_FX100_CONTROL3          0x0a410804 /* 100FX control register 3 */
#define SGMII0_FX100_STATUS1           0x0a410806 /* 100FX status register 1 */
#define SGMII0_FX100_STATUS3           0x0a41080a /* 100FX status register 3 */
#define SGMII0_FX100_STATUS4           0x0a41080c /* 100FX status register 4 */
#define SGMII0_FX100_FX100IDLE1        0x0a41080e /* 100FX idle pattern register 1 */
#define SGMII0_FX100_FX100IDLE2        0x0a410810 /* 100FX idle pattern register 2 */
#define SGMII0_FX100_FX100IDLESTATUS   0x0a410812 /* 100FX idle status register */
#define SGMII0_FX100_FX100IDLETHRES    0x0a410814 /* 100FX idle threshold register */
#define SGMII0_FX100_FX100LOCKTMR      0x0a410816 /* 100FX lock timer register */
#define SGMII0_FX100_FX100LINKTMR      0x0a410818 /* 100FX link timer register */


/****************************************************************************
 * bcm89530_top_bridge_sgmii_x3_SGMII_LANE0_SGMII0_RX2
 ***************************************************************************/
#define SGMII0_RX2_RXSEQ0              0x0a4108e0 /* rxseq0 Register */
#define SGMII0_RX2_RXSEQ1              0x0a4108e2 /* rxseq1 Register */
#define SGMII0_RX2_RXCDR0              0x0a4108e4 /* rxcdr0 Register */
#define SGMII0_RX2_RXCDR1              0x0a4108e6 /* rxcdr1 Register */
#define SGMII0_RX2_RXCDR2              0x0a4108e8 /* rxcdr2 Register */
#define SGMII0_RX2_RXCDR3              0x0a4108ea /* rxcdr3 Register */
#define SGMII0_RX2_RXCDR4              0x0a4108ec /* rxcdr4 Register */
#define SGMII0_RX2_STATUS0             0x0a4108ee /* rxstatus 0 Register */
#define SGMII0_RX2_STATUS1             0x0a4108f0 /* rxstatus 1 Register */
#define SGMII0_RX2_STATUS2             0x0a4108f2 /* rxstatus 2 Register */
#define SGMII0_RX2_STATUS3             0x0a4108f4 /* rxstatus 3 Register */
#define SGMII0_RX2_STATUS4             0x0a4108f6 /* rxstatus 4 Register */
#define SGMII0_RX2_STATUS5             0x0a4108f8 /* rxstatus 5 Register */
#define SGMII0_RX2_STATUS6             0x0a4108fa /* rxstatus 6 Register */


/****************************************************************************
 * bcm89530_top_bridge_sgmii_x3_SGMII_LANE0_SGMII0_RX3
 ***************************************************************************/
#define SGMII0_RX3_CONTROL0            0x0a410900 /* rx slice 0 Register */
#define SGMII0_RX3_CONTROL1            0x0a410902 /* rx slice 1 Register */
#define SGMII0_RX3_CONTROL2            0x0a410904 /* rx slice 2 Register */
#define SGMII0_RX3_CONTROL3            0x0a410906 /* rx slice 3 Register */
#define SGMII0_RX3_CONTROL4            0x0a410908 /* rx slice 4 Register */
#define SGMII0_RX3_CONTROL5            0x0a41090a /* rx slice 5 Register */
#define SGMII0_RX3_CONTROL6            0x0a41090c /* rx slice 6 Register */
#define SGMII0_RX3_CONTROL7            0x0a41090e /* rx slice 7 Register */
#define SGMII0_RX3_CONTROL8            0x0a410910 /* rx slice 8 Register */
#define SGMII0_RX3_CONTROL9            0x0a410912 /* rx slice 9 Register */
#define SGMII0_RX3_CONTROL10           0x0a410914 /* rx slice 10 Register */
#define SGMII0_RX3_CONTROL11           0x0a410916 /* rx slice 11 Register */


/****************************************************************************
 * bcm89530_top_bridge_sgmii_x3_SGMII_LANE0_SGMII_aerBlk
 ***************************************************************************/
#define SGMII_AERBLK_AER               0x0a41ffbc /* Address Expansion Register */


/****************************************************************************
 * bcm89530_top_bridge_sgmii_x3_SGMII_LANE0_SGMII0_Combo_IEEE0
 ***************************************************************************/
#define SGMII0_COMBO_IEEE0_MIICNTL     0x0a41ffc0 /* IEEE MII control register */
#define SGMII0_COMBO_IEEE0_MIISTAT     0x0a41ffc2 /* IEEE MII status register */
#define SGMII0_COMBO_IEEE0_ID1         0x0a41ffc4 /* IEEE phy ID LSByte register */
#define SGMII0_COMBO_IEEE0_ID2         0x0a41ffc6 /* IEEE phy ID MSByte register */
#define SGMII0_COMBO_IEEE0_AUTONEGADV  0x0a41ffc8 /* IEEE auto-negotiation advertised abilities register */
#define SGMII0_COMBO_IEEE0_AUTONEGLPABIL 0x0a41ffca /* IEEE auto-negotiation link partner abilities register */
#define SGMII0_COMBO_IEEE0_AUTONEGEXP  0x0a41ffcc /* IEEE auto-negotiation expansion register */
#define SGMII0_COMBO_IEEE0_AUTONEGNP   0x0a41ffce /* IEEE auto-negotiation next page register */
#define SGMII0_COMBO_IEEE0_AUTONEGLPABIL2 0x0a41ffd0 /* IEEE auto-negotiation link partner next page register */
#define SGMII0_COMBO_IEEE0_MIIEXTSTAT  0x0a41ffde /* IEEE MII extended status register */


/****************************************************************************
 * bcm89530_top_bridge_sgmii_x3_SGMII_LANE1_SGMII1_CL22_B0
 ***************************************************************************/
#define SGMII1_CL22_B0_MIICNTL         0x0a420000 /* IEEE MII control register */
#define SGMII1_CL22_B0_MIISTAT         0x0a420002 /* IEEE MII status register */
#define SGMII1_CL22_B0_ID1             0x0a420004 /* IEEE phy ID LSByte register */
#define SGMII1_CL22_B0_ID2             0x0a420006 /* IEEE phy ID MSByte register */
#define SGMII1_CL22_B0_AUTONEGADV      0x0a420008 /* IEEE auto-negotiation advertised abilities register */
#define SGMII1_CL22_B0_AUTONEGLPABIL   0x0a42000a /* IEEE auto-negotiation link partner abilities register */
#define SGMII1_CL22_B0_AUTONEGEXP      0x0a42000c /* IEEE auto-negotiation expansion register */
#define SGMII1_CL22_B0_AUTONEGNP       0x0a42000e /* IEEE auto-negotiation next page register */
#define SGMII1_CL22_B0_AUTONEGLPABIL2  0x0a420010 /* IEEE auto-negotiation link partner next page register */
#define SGMII1_CL22_B0_MIIEXTSTAT      0x0a42001e /* IEEE MII extended status register */


/****************************************************************************
 * bcm89530_top_bridge_sgmii_x3_SGMII_LANE1_SGMII1_TX_afe
 ***************************************************************************/
#define SGMII1_TX_AFE_ANATXASTATUS0    0x0a4300c0 /* Tx analog status 0 register */
#define SGMII1_TX_AFE_ANATXACONTROL0   0x0a4300c2 /* Tx analog control 0 register */
#define SGMII1_TX_AFE_ANATXMDATA0      0x0a4300c4 /* Tx test mux data 0 register */
#define SGMII1_TX_AFE_ANATXMDATA1      0x0a4300c6 /* Tx test mux data 1 register */
#define SGMII1_TX_AFE_CONTROL0         0x0a4300ca /* Analog TX Control 0 Register */
#define SGMII1_TX_AFE_CONTROL1         0x0a4300cc /* Analog TX Control 1 Register */
#define SGMII1_TX_AFE_CONTROL2         0x0a4300ce /* Analog TX Control 2 Register */
#define SGMII1_TX_AFE_CONTROL3         0x0a4300d0 /* Analog TX Control 3 Register */
#define SGMII1_TX_AFE_INTERP           0x0a4300d2 /* txinterp Control Register */


/****************************************************************************
 * bcm89530_top_bridge_sgmii_x3_SGMII_LANE1_SGMII1_RX_afe
 ***************************************************************************/
#define SGMII1_RX_AFE_ANARXSTATUS      0x0a430160 /* Rx lane status register */
#define SGMII1_RX_AFE_ANARXCONTROL     0x0a430162 /* Rx lane control register */
#define SGMII1_RX_AFE_CTRL0            0x0a430164 /* AFE Control Register 15:0 */
#define SGMII1_RX_AFE_CTRL1            0x0a430166 /* AFE Control Register 31:16 */
#define SGMII1_RX_AFE_ANARXSIGDET      0x0a430168 /* Rx Sigdet Control */
#define SGMII1_RX_AFE_CTRL2            0x0a43016a /* AFE Control Register 47:32 */
#define SGMII1_RX_AFE_CTRL3            0x0a43016c /* AFE Control Register 63:48 */
#define SGMII1_RX_AFE_CTRL4            0x0a43016e /* AFE Control Register 79:64 */
#define SGMII1_RX_AFE_ANARXTEST        0x0a430170 /* Rx lane control register */
#define SGMII1_RX_AFE_ANARXCONTROL1G   0x0a430172 /* Rx 1G Control register */
#define SGMII1_RX_AFE_ANARXCONTROLPCI  0x0a430174 /* Rx PCI Control register */
#define SGMII1_RX_AFE_ANARXASTATUS     0x0a430176 /* Rx analog status register */
#define SGMII1_RX_AFE_CTRL5            0x0a430178 /* AFE Control Register 95:80 */
#define SGMII1_RX_AFE_CTRL6            0x0a43017a /* AFE Control Register 111:96 */


/****************************************************************************
 * bcm89530_top_bridge_sgmii_x3_SGMII_LANE1_SGMII1_Digital
 ***************************************************************************/
#define SGMII1_DIGITAL_CONTROL1000X1   0x0a430600 /* 1000X control 1 register */
#define SGMII1_DIGITAL_CONTROL1000X2   0x0a430602 /* 1000X control 2 register */
#define SGMII1_DIGITAL_CONTROL1000X3   0x0a430604 /* 1000X control 3 register */
#define SGMII1_DIGITAL_CONTROL1000X4   0x0a430606 /* 1000X control 4 register */
#define SGMII1_DIGITAL_STATUS1000X1    0x0a430608 /* 1000X status 1 register */
#define SGMII1_DIGITAL_STATUS1000X2    0x0a43060a /* 1000X status 2 register */
#define SGMII1_DIGITAL_STATUS1000X3    0x0a43060c /* 1000X status 3 register */
#define SGMII1_DIGITAL_BADCODEGROUP    0x0a43060e /* Invalid code group count register */
#define SGMII1_DIGITAL_MISC1           0x0a430610 /* Miscellaneous 1 control register */
#define SGMII1_DIGITAL_MISC2           0x0a430612 /* Miscellaneous 2 control register */
#define SGMII1_DIGITAL_PATGENCTRL      0x0a430614 /* Pattern generator control register */
#define SGMII1_DIGITAL_PATGENSTAT      0x0a430616 /* Pattern generator status register */
#define SGMII1_DIGITAL_TESTMODE        0x0a430618 /* Test mode register */
#define SGMII1_DIGITAL_TXPKTCNT        0x0a43061a /* Tx packet count register */
#define SGMII1_DIGITAL_RXPKTCNT        0x0a43061c /* Rx packet count register */


/****************************************************************************
 * bcm89530_top_bridge_sgmii_x3_SGMII_LANE1_SGMII1_serdesID
 ***************************************************************************/
#define SGMII1_SERDESID_SERDESID0      0x0a430620 /* Serdes ID 0 register */
#define SGMII1_SERDESID_SERDESID1      0x0a430622 /* Serdes ID 1 register */
#define SGMII1_SERDESID_SERDESID2      0x0a430624 /* Serdes ID 2 register */
#define SGMII1_SERDESID_SERDESID3      0x0a430626 /* Serdes ID 3 register */


/****************************************************************************
 * bcm89530_top_bridge_sgmii_x3_SGMII_LANE1_SGMII1_Digital3
 ***************************************************************************/
#define SGMII1_DIGITAL3_TPOUT_1        0x0a43064e /* Test port out bits 15:0, tpout[15:0] */
#define SGMII1_DIGITAL3_TPOUT_2        0x0a430650 /* Test port out bits 23:8, tpout[23:8] */


/****************************************************************************
 * bcm89530_top_bridge_sgmii_x3_SGMII_LANE1_SGMII1_Digital4
 ***************************************************************************/
#define SGMII1_DIGITAL4_MISC3          0x0a430678 /* Miscellaneous 3 control register */
#define SGMII1_DIGITAL4_MISC5          0x0a43067c /* Miscelaneous 5 control register */


/****************************************************************************
 * bcm89530_top_bridge_sgmii_x3_SGMII_LANE1_SGMII1_FX100
 ***************************************************************************/
#define SGMII1_FX100_CONTROL1          0x0a430800 /* 100FX control register 1 */
#define SGMII1_FX100_CONTROL2          0x0a430802 /* 100FX control register 2 */
#define SGMII1_FX100_CONTROL3          0x0a430804 /* 100FX control register 3 */
#define SGMII1_FX100_STATUS1           0x0a430806 /* 100FX status register 1 */
#define SGMII1_FX100_STATUS3           0x0a43080a /* 100FX status register 3 */
#define SGMII1_FX100_STATUS4           0x0a43080c /* 100FX status register 4 */
#define SGMII1_FX100_FX100IDLE1        0x0a43080e /* 100FX idle pattern register 1 */
#define SGMII1_FX100_FX100IDLE2        0x0a430810 /* 100FX idle pattern register 2 */
#define SGMII1_FX100_FX100IDLESTATUS   0x0a430812 /* 100FX idle status register */
#define SGMII1_FX100_FX100IDLETHRES    0x0a430814 /* 100FX idle threshold register */
#define SGMII1_FX100_FX100LOCKTMR      0x0a430816 /* 100FX lock timer register */
#define SGMII1_FX100_FX100LINKTMR      0x0a430818 /* 100FX link timer register */


/****************************************************************************
 * bcm89530_top_bridge_sgmii_x3_SGMII_LANE1_SGMII1_RX2
 ***************************************************************************/
#define SGMII1_RX2_RXSEQ0              0x0a4308e0 /* rxseq0 Register */
#define SGMII1_RX2_RXSEQ1              0x0a4308e2 /* rxseq1 Register */
#define SGMII1_RX2_RXCDR0              0x0a4308e4 /* rxcdr0 Register */
#define SGMII1_RX2_RXCDR1              0x0a4308e6 /* rxcdr1 Register */
#define SGMII1_RX2_RXCDR2              0x0a4308e8 /* rxcdr2 Register */
#define SGMII1_RX2_RXCDR3              0x0a4308ea /* rxcdr3 Register */
#define SGMII1_RX2_RXCDR4              0x0a4308ec /* rxcdr4 Register */
#define SGMII1_RX2_STATUS0             0x0a4308ee /* rxstatus 0 Register */
#define SGMII1_RX2_STATUS1             0x0a4308f0 /* rxstatus 1 Register */
#define SGMII1_RX2_STATUS2             0x0a4308f2 /* rxstatus 2 Register */
#define SGMII1_RX2_STATUS3             0x0a4308f4 /* rxstatus 3 Register */
#define SGMII1_RX2_STATUS4             0x0a4308f6 /* rxstatus 4 Register */
#define SGMII1_RX2_STATUS5             0x0a4308f8 /* rxstatus 5 Register */
#define SGMII1_RX2_STATUS6             0x0a4308fa /* rxstatus 6 Register */


/****************************************************************************
 * bcm89530_top_bridge_sgmii_x3_SGMII_LANE1_SGMII1_RX3
 ***************************************************************************/
#define SGMII1_RX3_CONTROL0            0x0a430900 /* rx slice 0 Register */
#define SGMII1_RX3_CONTROL1            0x0a430902 /* rx slice 1 Register */
#define SGMII1_RX3_CONTROL2            0x0a430904 /* rx slice 2 Register */
#define SGMII1_RX3_CONTROL3            0x0a430906 /* rx slice 3 Register */
#define SGMII1_RX3_CONTROL4            0x0a430908 /* rx slice 4 Register */
#define SGMII1_RX3_CONTROL5            0x0a43090a /* rx slice 5 Register */
#define SGMII1_RX3_CONTROL6            0x0a43090c /* rx slice 6 Register */
#define SGMII1_RX3_CONTROL7            0x0a43090e /* rx slice 7 Register */
#define SGMII1_RX3_CONTROL8            0x0a430910 /* rx slice 8 Register */
#define SGMII1_RX3_CONTROL9            0x0a430912 /* rx slice 9 Register */
#define SGMII1_RX3_CONTROL10           0x0a430914 /* rx slice 10 Register */
#define SGMII1_RX3_CONTROL11           0x0a430916 /* rx slice 11 Register */


/****************************************************************************
 * bcm89530_top_bridge_sgmii_x3_SGMII_LANE1_SGMII1_Combo_IEEE0
 ***************************************************************************/
#define SGMII1_COMBO_IEEE0_MIICNTL     0x0a43ffc0 /* IEEE MII control register */
#define SGMII1_COMBO_IEEE0_MIISTAT     0x0a43ffc2 /* IEEE MII status register */
#define SGMII1_COMBO_IEEE0_ID1         0x0a43ffc4 /* IEEE phy ID LSByte register */
#define SGMII1_COMBO_IEEE0_ID2         0x0a43ffc6 /* IEEE phy ID MSByte register */
#define SGMII1_COMBO_IEEE0_AUTONEGADV  0x0a43ffc8 /* IEEE auto-negotiation advertised abilities register */
#define SGMII1_COMBO_IEEE0_AUTONEGLPABIL 0x0a43ffca /* IEEE auto-negotiation link partner abilities register */
#define SGMII1_COMBO_IEEE0_AUTONEGEXP  0x0a43ffcc /* IEEE auto-negotiation expansion register */
#define SGMII1_COMBO_IEEE0_AUTONEGNP   0x0a43ffce /* IEEE auto-negotiation next page register */
#define SGMII1_COMBO_IEEE0_AUTONEGLPABIL2 0x0a43ffd0 /* IEEE auto-negotiation link partner next page register */
#define SGMII1_COMBO_IEEE0_MIIEXTSTAT  0x0a43ffde /* IEEE MII extended status register */


/****************************************************************************
 * bcm89530_top_bridge_sgmii_x3_SGMII_LANE2_SGMII2_CL22_B0
 ***************************************************************************/
#define SGMII2_CL22_B0_MIICNTL         0x0a440000 /* IEEE MII control register */
#define SGMII2_CL22_B0_MIISTAT         0x0a440002 /* IEEE MII status register */
#define SGMII2_CL22_B0_ID1             0x0a440004 /* IEEE phy ID LSByte register */
#define SGMII2_CL22_B0_ID2             0x0a440006 /* IEEE phy ID MSByte register */
#define SGMII2_CL22_B0_AUTONEGADV      0x0a440008 /* IEEE auto-negotiation advertised abilities register */
#define SGMII2_CL22_B0_AUTONEGLPABIL   0x0a44000a /* IEEE auto-negotiation link partner abilities register */
#define SGMII2_CL22_B0_AUTONEGEXP      0x0a44000c /* IEEE auto-negotiation expansion register */
#define SGMII2_CL22_B0_AUTONEGNP       0x0a44000e /* IEEE auto-negotiation next page register */
#define SGMII2_CL22_B0_AUTONEGLPABIL2  0x0a440010 /* IEEE auto-negotiation link partner next page register */
#define SGMII2_CL22_B0_MIIEXTSTAT      0x0a44001e /* IEEE MII extended status register */


/****************************************************************************
 * bcm89530_top_bridge_sgmii_x3_SGMII_LANE2_SGMII2_TX_afe
 ***************************************************************************/
#define SGMII2_TX_AFE_ANATXASTATUS0    0x0a4500c0 /* Tx analog status 0 register */
#define SGMII2_TX_AFE_ANATXACONTROL0   0x0a4500c2 /* Tx analog control 0 register */
#define SGMII2_TX_AFE_ANATXMDATA0      0x0a4500c4 /* Tx test mux data 0 register */
#define SGMII2_TX_AFE_ANATXMDATA1      0x0a4500c6 /* Tx test mux data 1 register */
#define SGMII2_TX_AFE_CONTROL0         0x0a4500ca /* Analog TX Control 0 Register */
#define SGMII2_TX_AFE_CONTROL1         0x0a4500cc /* Analog TX Control 1 Register */
#define SGMII2_TX_AFE_CONTROL2         0x0a4500ce /* Analog TX Control 2 Register */
#define SGMII2_TX_AFE_CONTROL3         0x0a4500d0 /* Analog TX Control 3 Register */
#define SGMII2_TX_AFE_INTERP           0x0a4500d2 /* txinterp Control Register */


/****************************************************************************
 * bcm89530_top_bridge_sgmii_x3_SGMII_LANE2_SGMII2_RX_afe
 ***************************************************************************/
#define SGMII2_RX_AFE_ANARXSTATUS      0x0a450160 /* Rx lane status register */
#define SGMII2_RX_AFE_ANARXCONTROL     0x0a450162 /* Rx lane control register */
#define SGMII2_RX_AFE_CTRL0            0x0a450164 /* AFE Control Register 15:0 */
#define SGMII2_RX_AFE_CTRL1            0x0a450166 /* AFE Control Register 31:16 */
#define SGMII2_RX_AFE_ANARXSIGDET      0x0a450168 /* Rx Sigdet Control */
#define SGMII2_RX_AFE_CTRL2            0x0a45016a /* AFE Control Register 47:32 */
#define SGMII2_RX_AFE_CTRL3            0x0a45016c /* AFE Control Register 63:48 */
#define SGMII2_RX_AFE_CTRL4            0x0a45016e /* AFE Control Register 79:64 */
#define SGMII2_RX_AFE_ANARXTEST        0x0a450170 /* Rx lane control register */
#define SGMII2_RX_AFE_ANARXCONTROL1G   0x0a450172 /* Rx 1G Control register */
#define SGMII2_RX_AFE_ANARXCONTROLPCI  0x0a450174 /* Rx PCI Control register */
#define SGMII2_RX_AFE_ANARXASTATUS     0x0a450176 /* Rx analog status register */
#define SGMII2_RX_AFE_CTRL5            0x0a450178 /* AFE Control Register 95:80 */
#define SGMII2_RX_AFE_CTRL6            0x0a45017a /* AFE Control Register 111:96 */


/****************************************************************************
 * bcm89530_top_bridge_sgmii_x3_SGMII_LANE2_SGMII2_Digital
 ***************************************************************************/
#define SGMII2_DIGITAL_CONTROL1000X1   0x0a450600 /* 1000X control 1 register */
#define SGMII2_DIGITAL_CONTROL1000X2   0x0a450602 /* 1000X control 2 register */
#define SGMII2_DIGITAL_CONTROL1000X3   0x0a450604 /* 1000X control 3 register */
#define SGMII2_DIGITAL_CONTROL1000X4   0x0a450606 /* 1000X control 4 register */
#define SGMII2_DIGITAL_STATUS1000X1    0x0a450608 /* 1000X status 1 register */
#define SGMII2_DIGITAL_STATUS1000X2    0x0a45060a /* 1000X status 2 register */
#define SGMII2_DIGITAL_STATUS1000X3    0x0a45060c /* 1000X status 3 register */
#define SGMII2_DIGITAL_BADCODEGROUP    0x0a45060e /* Invalid code group count register */
#define SGMII2_DIGITAL_MISC1           0x0a450610 /* Miscellaneous 1 control register */
#define SGMII2_DIGITAL_MISC2           0x0a450612 /* Miscellaneous 2 control register */
#define SGMII2_DIGITAL_PATGENCTRL      0x0a450614 /* Pattern generator control register */
#define SGMII2_DIGITAL_PATGENSTAT      0x0a450616 /* Pattern generator status register */
#define SGMII2_DIGITAL_TESTMODE        0x0a450618 /* Test mode register */
#define SGMII2_DIGITAL_TXPKTCNT        0x0a45061a /* Tx packet count register */
#define SGMII2_DIGITAL_RXPKTCNT        0x0a45061c /* Rx packet count register */


/****************************************************************************
 * bcm89530_top_bridge_sgmii_x3_SGMII_LANE2_SGMII2_serdesID
 ***************************************************************************/
#define SGMII2_SERDESID_SERDESID0      0x0a450620 /* Serdes ID 0 register */
#define SGMII2_SERDESID_SERDESID1      0x0a450622 /* Serdes ID 1 register */
#define SGMII2_SERDESID_SERDESID2      0x0a450624 /* Serdes ID 2 register */
#define SGMII2_SERDESID_SERDESID3      0x0a450626 /* Serdes ID 3 register */


/****************************************************************************
 * bcm89530_top_bridge_sgmii_x3_SGMII_LANE2_SGMII2_Digital3
 ***************************************************************************/
#define SGMII2_DIGITAL3_TPOUT_1        0x0a45064e /* Test port out bits 15:0, tpout[15:0] */
#define SGMII2_DIGITAL3_TPOUT_2        0x0a450650 /* Test port out bits 23:8, tpout[23:8] */


/****************************************************************************
 * bcm89530_top_bridge_sgmii_x3_SGMII_LANE2_SGMII2_Digital4
 ***************************************************************************/
#define SGMII2_DIGITAL4_MISC3          0x0a450678 /* Miscellaneous 3 control register */
#define SGMII2_DIGITAL4_MISC5          0x0a45067c /* Miscelaneous 5 control register */


/****************************************************************************
 * bcm89530_top_bridge_sgmii_x3_SGMII_LANE2_SGMII2_FX100
 ***************************************************************************/
#define SGMII2_FX100_CONTROL1          0x0a450800 /* 100FX control register 1 */
#define SGMII2_FX100_CONTROL2          0x0a450802 /* 100FX control register 2 */
#define SGMII2_FX100_CONTROL3          0x0a450804 /* 100FX control register 3 */
#define SGMII2_FX100_STATUS1           0x0a450806 /* 100FX status register 1 */
#define SGMII2_FX100_STATUS3           0x0a45080a /* 100FX status register 3 */
#define SGMII2_FX100_STATUS4           0x0a45080c /* 100FX status register 4 */
#define SGMII2_FX100_FX100IDLE1        0x0a45080e /* 100FX idle pattern register 1 */
#define SGMII2_FX100_FX100IDLE2        0x0a450810 /* 100FX idle pattern register 2 */
#define SGMII2_FX100_FX100IDLESTATUS   0x0a450812 /* 100FX idle status register */
#define SGMII2_FX100_FX100IDLETHRES    0x0a450814 /* 100FX idle threshold register */
#define SGMII2_FX100_FX100LOCKTMR      0x0a450816 /* 100FX lock timer register */
#define SGMII2_FX100_FX100LINKTMR      0x0a450818 /* 100FX link timer register */


/****************************************************************************
 * bcm89530_top_bridge_sgmii_x3_SGMII_LANE2_SGMII2_RX2
 ***************************************************************************/
#define SGMII2_RX2_RXSEQ0              0x0a4508e0 /* rxseq0 Register */
#define SGMII2_RX2_RXSEQ1              0x0a4508e2 /* rxseq1 Register */
#define SGMII2_RX2_RXCDR0              0x0a4508e4 /* rxcdr0 Register */
#define SGMII2_RX2_RXCDR1              0x0a4508e6 /* rxcdr1 Register */
#define SGMII2_RX2_RXCDR2              0x0a4508e8 /* rxcdr2 Register */
#define SGMII2_RX2_RXCDR3              0x0a4508ea /* rxcdr3 Register */
#define SGMII2_RX2_RXCDR4              0x0a4508ec /* rxcdr4 Register */
#define SGMII2_RX2_STATUS0             0x0a4508ee /* rxstatus 0 Register */
#define SGMII2_RX2_STATUS1             0x0a4508f0 /* rxstatus 1 Register */
#define SGMII2_RX2_STATUS2             0x0a4508f2 /* rxstatus 2 Register */
#define SGMII2_RX2_STATUS3             0x0a4508f4 /* rxstatus 3 Register */
#define SGMII2_RX2_STATUS4             0x0a4508f6 /* rxstatus 4 Register */
#define SGMII2_RX2_STATUS5             0x0a4508f8 /* rxstatus 5 Register */
#define SGMII2_RX2_STATUS6             0x0a4508fa /* rxstatus 6 Register */


/****************************************************************************
 * bcm89530_top_bridge_sgmii_x3_SGMII_LANE2_SGMII2_RX3
 ***************************************************************************/
#define SGMII2_RX3_CONTROL0            0x0a450900 /* rx slice 0 Register */
#define SGMII2_RX3_CONTROL1            0x0a450902 /* rx slice 1 Register */
#define SGMII2_RX3_CONTROL2            0x0a450904 /* rx slice 2 Register */
#define SGMII2_RX3_CONTROL3            0x0a450906 /* rx slice 3 Register */
#define SGMII2_RX3_CONTROL4            0x0a450908 /* rx slice 4 Register */
#define SGMII2_RX3_CONTROL5            0x0a45090a /* rx slice 5 Register */
#define SGMII2_RX3_CONTROL6            0x0a45090c /* rx slice 6 Register */
#define SGMII2_RX3_CONTROL7            0x0a45090e /* rx slice 7 Register */
#define SGMII2_RX3_CONTROL8            0x0a450910 /* rx slice 8 Register */
#define SGMII2_RX3_CONTROL9            0x0a450912 /* rx slice 9 Register */
#define SGMII2_RX3_CONTROL10           0x0a450914 /* rx slice 10 Register */
#define SGMII2_RX3_CONTROL11           0x0a450916 /* rx slice 11 Register */


/****************************************************************************
 * bcm89530_top_bridge_sgmii_x3_SGMII_LANE2_SGMII2_Combo_IEEE0
 ***************************************************************************/
#define SGMII2_COMBO_IEEE0_MIICNTL     0x0a45ffc0 /* IEEE MII control register */
#define SGMII2_COMBO_IEEE0_MIISTAT     0x0a45ffc2 /* IEEE MII status register */
#define SGMII2_COMBO_IEEE0_ID1         0x0a45ffc4 /* IEEE phy ID LSByte register */
#define SGMII2_COMBO_IEEE0_ID2         0x0a45ffc6 /* IEEE phy ID MSByte register */
#define SGMII2_COMBO_IEEE0_AUTONEGADV  0x0a45ffc8 /* IEEE auto-negotiation advertised abilities register */
#define SGMII2_COMBO_IEEE0_AUTONEGLPABIL 0x0a45ffca /* IEEE auto-negotiation link partner abilities register */
#define SGMII2_COMBO_IEEE0_AUTONEGEXP  0x0a45ffcc /* IEEE auto-negotiation expansion register */
#define SGMII2_COMBO_IEEE0_AUTONEGNP   0x0a45ffce /* IEEE auto-negotiation next page register */
#define SGMII2_COMBO_IEEE0_AUTONEGLPABIL2 0x0a45ffd0 /* IEEE auto-negotiation link partner next page register */
#define SGMII2_COMBO_IEEE0_MIIEXTSTAT  0x0a45ffde /* IEEE MII extended status register */


/****************************************************************************
 * bcm89530_top_bridge_MISC
 ***************************************************************************/
#define MISC_MODEL_REV_NUM             0x0a800000 /* track the model and revision numbers */
#define MISC_DEVICEID_LO               0x0a800002 /* track device ID [11:0] */
#define MISC_DEVICEID_HI               0x0a800004 /* track device ID [19:12] */
#define MISC_SWITCH_MISC_CTRL          0x0a800006 /* miscallenaous switch config form chip level */
#define MISC_LDO_PWRDN                 0x0a800100 /* LDO Power Down register */
#define MISC_LDO_VREGCNTL_1            0x0a800102 /* LDO Voltage Regulator Control register 1 */
#define MISC_LDO_VREGCNTL_2            0x0a800104 /* LDO Voltage Regulator Control register 2 */
#define MISC_LDO_VREGCNTLEN            0x0a800106 /* LDO Voltage Regulator Control Enable register */
#define MISC_SWREG_CTRL_REG0           0x0a800110 /* Switching regulator control register0 */
#define MISC_SWREG_CTRL_REG1           0x0a800112 /* Switching regulator control register1 */
#define MISC_SWREG_CTRL_REG2           0x0a800114 /* Switching regulator control register2 */
#define MISC_SWREG_CTRL_REG3           0x0a800116 /* Switching regulator control register3 */
#define MISC_SWREG_CTRL_REG4           0x0a800118 /* Switching regulator control register4 */
#define MISC_SWREG_CTRL_REG5           0x0a80011a /* Switching regulator control register5 */
#define MISC_SWREG_CTRL_REG6           0x0a80011c /* Switching regulator control register6 */
#define MISC_SWREG_CTRL_REG7           0x0a80011e /* Switching regulator control register7 */
#define MISC_SWREG_CTRL_REG8           0x0a800120 /* Switching regulator control register8 */
#define MISC_SWREG_CTRL_REG9           0x0a800122 /* Switching regulator control register9 */
#define MISC_SWREG_STAT_REG12          0x0a800124 /* Switching regulator status register0 */
#define MISC_SWREG_STAT_REG13          0x0a800126 /* Switching regulator status register1 */
#define MISC_SWREG_STAT_REG14          0x0a800128 /* Switching regulator status register2 */
#define MISC_SWREG_STAT_REG15          0x0a80012a /* Switching regulator status register3 */
#define MISC_SWREG_ACCESS_CTRL_1       0x0a80012c /* Switching regulator access control register1 */
#define MISC_SWREG_ACCESS_CTRL_2       0x0a80012e /* Switching regulator access control register2 */
#define MISC_SWREG_CONTROL_STATUS      0x0a800130 /* Switching regulator miscellaneous control and status */
#define MISC_SGMII_PWRDWN              0x0a800140 /* SGMII PowerDown */
#define MISC_SGMII_HW_RST_DLY_VAL      0x0a800142 /* SGMII Hardware Reset Delay Value */
#define MISC_SGMII_MDIO_RST_DLY_VAL    0x0a800144 /* SGMII MDIO Reset Delay Value */
#define MISC_SGMII_PLL_RST_DLY_VAL     0x0a800146 /* SGMII PLL Reset Delay Value */
#define MISC_SGMII_EXT_CTL             0x0a800148 /* SGMII External Control */
#define MISC_SGMII_AN0                 0x0a80014a /* SGMII AutoNeg 0 */
#define MISC_SGMII_BASE_PAGE0          0x0a80014c /* SGMII Base Page 0 */
#define MISC_SGMII_AN1                 0x0a80014e /* SGMII AutoNeg 1 */
#define MISC_SGMII_BASE_PAGE1          0x0a800150 /* SGMII Base Page 1 */
#define MISC_SGMII_AN2                 0x0a800152 /* SGMII AutoNeg 2 */
#define MISC_SGMII_BASE_PAGE2          0x0a800154 /* SGMII Base Page 2 */
#define MISC_SGMII_MDIO_CTL            0x0a800156 /* SGMII MDIO Control */
#define MISC_OTP_CPU_COMMAND           0x0a800170 /* OTP CPU Command Register */
#define MISC_OTP_CPU_WRDATA_H          0x0a800172 /* CPU Write Data Register High */
#define MISC_OTP_CPU_WRDATA_L          0x0a800174 /* OTP CPU Write Data Register Low */
#define MISC_OTP_CONFIG                0x0a800176 /* OTP Configuration Register */
#define MISC_OTP_ADDRESS               0x0a800178 /* OTP Address Register */
#define MISC_OTP_STATUS_1              0x0a80017a /* OTP Status Register 1 */
#define MISC_OTP_STATUS_0              0x0a80017c /* OTP Status Register 0 */
#define MISC_OTP_RDATA_H               0x0a800180 /* OTP Read Data Register High */
#define MISC_OTP_RDATA_L               0x0a800182 /* OTP Read Data Register Low */
#define MISC_BISR_STATUS               0x0a800184 /* BISR Status Register */
#define MISC_PVTMON_CTRL               0x0a800200 /* PVTMON general controls */
#define MISC_PVTMON_SAMPLE_NUM         0x0a800202 /* number of temp & voltage samples to be collected in each measurement cycle in auto mode */
#define MISC_PVTMON_TMON_PERIOD        0x0a800204 /* period of occurence of tmon cycle w.r.t vmon cycles */
#define MISC_PVTMON_I_CTRL_31_16       0x0a800206 /* PVTMON i_ctrl[31:16] - to be used in EXPERT mode only */
#define MISC_PVTMON_I_CTRL_15_0        0x0a800208 /* PVTMON i_ctrl[15:0] - to be used in EXPERT mode only */
#define MISC_PVTMON_ADC_DATA           0x0a80020a /* 10-bit output from the SAR ADC - to be used when auto_mode=0 (OR) to be used in EXPERT mode */
#define MISC_PVTMON_DAC_DATA           0x0a80020c /* DAC input data from user - to be used in EXPERT mode only */
#define MISC_PVTMON_TMON_THRESH1_CTRL  0x0a80020e /* thresh1 controls for tmon */
#define MISC_PVTMON_TMON_THRESH2_CTRL  0x0a800210 /* thresh2 controls for tmon */
#define MISC_PVTMON_VMON_1V_H_THRESH1_CTRL 0x0a800212 /* thresh1 controls for vmon_1v_h */
#define MISC_PVTMON_VMON_1V_H_THRESH2_CTRL 0x0a800214 /* thresh2 controls for vmon_1v_h */
#define MISC_PVTMON_VMON_1V_L_THRESH1_CTRL 0x0a800216 /* thresh1 controls for vmon_1v_l */
#define MISC_PVTMON_VMON_1V_L_THRESH2_CTRL 0x0a800218 /* thresh2 controls for vmon_1v_l */
#define MISC_PVTMON_VMON_1P8V_H_THRESH1_CTRL 0x0a80021a /* thresh1 controls for vmon_1p8v_h */
#define MISC_PVTMON_VMON_1P8V_H_THRESH2_CTRL 0x0a80021c /* thresh2 controls for vmon_1p8v_h */
#define MISC_PVTMON_VMON_1P8V_L_THRESH1_CTRL 0x0a80021e /* thresh1 controls for vmon_1p8v_l */
#define MISC_PVTMON_VMON_1P8V_L_THRESH2_CTRL 0x0a800220 /* thresh2 controls for vmon_1p8v_l */
#define MISC_PVTMON_VMON_3P3V_H_THRESH1_CTRL 0x0a800222 /* thresh1 controls for vmon_3p3v_h */
#define MISC_PVTMON_VMON_3P3V_H_THRESH2_CTRL 0x0a800224 /* thresh2 controls for vmon_3p3v_h */
#define MISC_PVTMON_VMON_3P3V_L_THRESH1_CTRL 0x0a800226 /* thresh1 controls for vmon_3p3v_l */
#define MISC_PVTMON_VMON_3P3V_L_THRESH2_CTRL 0x0a800228 /* thresh2 controls for vmon_3p3v_l */
#define MISC_PVTMON_THRESHOLD_FILTER   0x0a80022a /* number of continous threshold breaches that will lead to "violation" */
#define MISC_PVTMON_VIOL_RAWSTS        0x0a80022c /* raw status showing whether we have a brownout violation */
#define MISC_PVTMON_INTR_STATUS        0x0a80022e /* latched status register for all interrupts from pvtmon */
#define MISC_PVTMON_INTSTS_CLEAR       0x0a800230 /* clear the interrupt status */
#define MISC_PVTMON_INTERRUPT_COUNT    0x0a800232 /* number of brownout interrupts until now */
#define MISC_PVTMON_TMON_SAMPLE        0x0a800234 /* last temp sample */
#define MISC_PVTMON_VMON_1V_SAMPLE     0x0a800236 /* last 1V sample */
#define MISC_PVTMON_VMON_1P8V_SAMPLE   0x0a800238 /* last 1.8V sample */
#define MISC_PVTMON_VMON_3P3V_SAMPLE   0x0a80023a /* last 3.3V sample */
#define MISC_F1_IMAGE_STATUS           0x0a800300 /* F1 Image Status */
#define MISC_F1_IMAGE_VERSION          0x0a800302 /* F1 Image Version */
#define MISC_F2_IMAGE_STATUS           0x0a800304 /* F2 Image Status */
#define MISC_F2_IMAGE_VERSION          0x0a800306 /* F2 Image Version */
#define MISC_SPARE_HW_REG4             0x0a800308 /* Spare H/W Register 4 */
#define MISC_SPARE_HW_REG5             0x0a80030a /* Spare H/W Register 5 */
#define MISC_SPARE_HW_REG6             0x0a80030c /* Spare H/W Register 6 */
#define MISC_SPARE_HW_REG7             0x0a80030e /* Spare H/W Register 7 */
#define MISC_SPARE_HW_REG8             0x0a800310 /* Spare H/W Register 8 */
#define MISC_SPARE_HW_REG9             0x0a800312 /* Spare H/W Register 9 */
#define MISC_SPARE_HW_REG10            0x0a800314 /* Spare H/W Register 10 */
#define MISC_SPARE_HW_REG11            0x0a800316 /* Spare H/W Register 11 */
#define MISC_SPARE_HW_REG12            0x0a800318 /* Spare H/W Register 12 */
#define MISC_SPARE_HW_REG13            0x0a80031a /* Spare H/W Register 13 */
#define MISC_SPARE_HW_REG14            0x0a80031c /* Spare H/W Register 14 */
#define MISC_SPARE_HW_REG15            0x0a80031e /* Spare H/W Register 15 */
#define MISC_SPARE_SW_REG0             0x0a800320 /* Spare S/W Register 0 */
#define MISC_SPARE_SW_REG1             0x0a800322 /* Spare S/W Register 1 */
#define MISC_SPARE_SW_REG2             0x0a800324 /* Spare S/W Register 2 */
#define MISC_SPARE_SW_REG3             0x0a800326 /* Spare S/W Register 3 */
#define MISC_SPARE_SW_REG4             0x0a800328 /* Spare S/W Register 4 */
#define MISC_SPARE_SW_REG5             0x0a80032a /* Spare S/W Register 5 */
#define MISC_SPARE_SW_REG6             0x0a80032c /* Spare S/W Register 6 */
#define MISC_SPARE_SW_REG7             0x0a80032e /* Spare S/W Register 7 */
#define MISC_SPARE_SW_REG8             0x0a800330 /* Spare S/W Register 8 */
#define MISC_SPARE_SW_REG9             0x0a800332 /* Spare S/W Register 9 */
#define MISC_SPARE_SW_REG10            0x0a800334 /* Spare S/W Register 10 */
#define MISC_SPARE_SW_REG11            0x0a800336 /* Spare S/W Register 11 */
#define MISC_SPARE_SW_REG12            0x0a800338 /* Spare S/W Register 12 */
#define MISC_SPARE_SW_REG13            0x0a80033a /* Spare S/W Register 13 */
#define MISC_SPARE_SW_REG14            0x0a80033c /* Spare S/W Register 14 */
#define MISC_SPARE_SW_REG15            0x0a80033e /* Spare S/W Register 15 */
#define MISC_CPUSYS_MISC               0x0a800340 /* Misc CPUSYS ctrl Register */
#define MISC_BRPHYS_CLEAR_ON_READ_REG  0x0a800342 /* Misc BRPHYS_CLEAR_ON_READ_REG ctrl Register */
#define MISC_SCRATCH_REG               0x0a800ffe /* Scratch reg */


/****************************************************************************
 * bcm89530_top_bridge_CRG
 ***************************************************************************/
#define CRG_XTAL_CONFIG                0x0a820000 /* XTAL Configuration register */
#define CRG_PLL_CONFIG1                0x0a820002 /* PLL configuration1 regsiter */
#define CRG_PLL_CONFIG2                0x0a820004 /* PLL configuration2 regsiter */
#define CRG_PLL_NDIV                   0x0a820006 /* PLL NDIV integer value */
#define CRG_PLL_CTRL0                  0x0a820008 /* PLL control regsiter0 */
#define CRG_PLL_CTRL1                  0x0a82000a /* PLL control regsiter1 */
#define CRG_PLL_CTRL2                  0x0a82000c /* PLL control regsiter2 */
#define CRG_PLL_CTRL3                  0x0a82000e /* PLL control regsiter3 */
#define CRG_PLL_MDIV_CH01              0x0a820010 /* MDIV for channel 0 and 1 */
#define CRG_PLL_MDIV_CH23              0x0a820012 /* MDIV for channel 2 and 3 */
#define CRG_PLL_SSC_CONFIG1            0x0a820014 /* PLL SSC config1 */
#define CRG_PLL_SSC_CONFIG2            0x0a820016 /* PLL SSC config2 */
#define CRG_PLL_SSC_STEP               0x0a820018 /* PLL SSC Step size */
#define CRG_PLL_STATUS                 0x0a82001a /* PLL Status register */
#define CRG_CLOCK_CONFIG1              0x0a82001c /* Clock config regsiter */
#define CRG_IDDQ_CHIP                  0x0a82001e /* Chip IDDQ register */
#define CRG_IDDQ_CONFIG                0x0a820020 /* Individual Block IDDQ configuration. */
#define CRG_RESET_CONFIG               0x0a820022 /* Reset Configuration */
#define CRG_SCRATCH_REG                0x0a820ffe /* Scratch reg */


/****************************************************************************
 * bcm89530_top_bridge_IO
 ***************************************************************************/
#define IO_MII1_CONFIG                 0x0a840000 /* MII1_CONFIG */
#define IO_MII2_CONFIG                 0x0a840002 /* MII2_CONFIG */
#define IO_IO_HYSTERESIS               0x0a840004 /* IO_HYSTERESIS */
#define IO_IO_SOURCE                   0x0a840006 /* IO_SOURCE */
#define IO_IO_SEL                      0x0a840008 /* IO_SEL */
#define IO_IO_MII1_MODEHV              0x0a84000a /* MII1_MODEHV */
#define IO_IO_MII2_MODEHV              0x0a84000c /* MII2_MODEHV */
#define IO_RGMII1_CTL                  0x0a840020 /* RGMII1_CTL */
#define IO_RGMII2_CTL                  0x0a840022 /* RGMII2_CTL */
#define IO_SGMII_RGMII_CTL             0x0a840024 /* SGMII_RGMII_CTL */
#define IO_RGMII1_GMII_CTL             0x0a840026 /* RGMII1_GMII_CTL */
#define IO_RGMII2_GMII_CTL             0x0a840028 /* RGMII2_GMII_CTL */
#define IO_CPU_GMII_CTL                0x0a84002a /* CPU_GMII_CTL */
#define IO_STRAPS_RAW                  0x0a840040 /* STRAPS_RAW */
#define IO_STRAPS_OV                   0x0a840042 /* STRAPS_OV */
#define IO_SW_OVRD0                    0x0a840050 /* SW_OVRD */
#define IO_SW_OVRD1                    0x0a840052 /* SW_OVRD */
#define IO_SW_OVRD2                    0x0a840054 /* SW_OVRD */
#define IO_SW_OVRD3                    0x0a840056 /* SW_OVRD */
#define IO_SW_OVRD4                    0x0a840058 /* SW_OVRD */
#define IO_SW_OVRD5                    0x0a84005a /* SW_OVRD */
#define IO_SW_OVRD6                    0x0a84005c /* SW_OVRD */
#define IO_SW_OVRD7                    0x0a84005e /* SW_OVRD */
#define IO_SW_OVRD8                    0x0a840060 /* SW_OVRD */
#define IO_TEST_BUS_SELECT             0x0a840070 /* TEST_BUS_SELECT */
#define IO_P1588_CONFIG                0x0a840072 /* Top level P1588 Config */
#define IO_P1588_SYNC_GEN              0x0a840074 /* P1588 Sync In pulse generator */
#define IO_IOFF                        0x0a840ffe /* IOFF */


/****************************************************************************
 * bcm89530_top_bridge_TOP_1588
 ***************************************************************************/
#define TOP_1588_SLICE_ENABLE          0x0a860020 /* P1588 Slice Enable Control Register */
#define TOP_1588_TX_MODE_PORT_0        0x0a860022 /* P1588 Port 0 TX Event Message Mode1 and Mode2 Selection Register */
#define TOP_1588_TX_MODE_PORT_1        0x0a860024 /* P1588 Port 1 TX Event Message Mode1 and Mode2 Selection Register */
#define TOP_1588_TX_MODE_PORT_2        0x0a860026 /* P1588 Port 2 TX Event Message Mode1 and Mode2 Selection Register */
#define TOP_1588_TX_MODE_PORT_3        0x0a860028 /* P1588 Port 3 TX Event Message Mode1 and Mode2 Selection Register */
#define TOP_1588_TX_MODE_PORT_4        0x0a86002a /* P1588 Port 4 TX Event Message Mode1 and Mode2 Selection Register */
#define TOP_1588_TX_MODE_PORT_5        0x0a86002c /* P1588 Port 5 TX Event Message Mode1 and Mode2 Selection Register */
#define TOP_1588_TX_MODE_PORT_6        0x0a86002e /* P1588 Port 6 TX Event Message Mode1 and Mode2 Selection Register */
#define TOP_1588_TX_MODE_PORT_7        0x0a860030 /* P1588 Port 7 TX Event Message Mode1 and Mode2 Selection Register */
#define TOP_1588_RX_MODE_PORT_0        0x0a860032 /* P1588 Port 0 RX Event Message Mode1 and Mode2 Selection Register */
#define TOP_1588_RX_MODE_PORT_1        0x0a860034 /* P1588 Port 1 RX Event Message Mode1 and Mode2 Selection Register */
#define TOP_1588_RX_MODE_PORT_2        0x0a860036 /* P1588 Port 2 RX Event Message Mode1 and Mode2 Selection Register */
#define TOP_1588_RX_MODE_PORT_3        0x0a860038 /* P1588 Port 3 RX Event Message Mode1 and Mode2 Selection Register */
#define TOP_1588_RX_MODE_PORT_4        0x0a86003a /* P1588 Port 4 RX Event Message Mode1 and Mode2 Selection Register */
#define TOP_1588_RX_MODE_PORT_5        0x0a86003c /* P1588 Port 5 RX Event Message Mode1 and Mode2 Selection Register */
#define TOP_1588_RX_MODE_PORT_6        0x0a86003e /* P1588 Port 6 RX Event Message Mode1 and Mode2 Selection Register */
#define TOP_1588_RX_MODE_PORT_7        0x0a860040 /* P1588 Port 7 RX Event Message Mode1 and Mode2 Selection Register */
#define TOP_1588_TX_TS_CAP             0x0a860042 /* P1588 TX SOP Timestamp Capture Enable */
#define TOP_1588_RX_TS_CAP             0x0a860044 /* P1588 RX SOP Timestamp Capture Enable Register */
#define TOP_1588_RX_TX_OPTION          0x0a860046 /* P1588 RX and TX Option Register */
#define TOP_1588_RX_PORT_0_LINK_DELAY_LSB 0x0a860048 /* P1588 Port 0 RX PORT Link delay LSB Register */
#define TOP_1588_RX_PORT_0_LINK_DELAY_MSB 0x0a86004a /* P1588 Port 0 RX PORT Link delay MSB Register */
#define TOP_1588_RX_PORT_1_LINK_DELAY_LSB 0x0a86004c /* P1588 Port 1 RX PORT Link delay LSB Register */
#define TOP_1588_RX_PORT_1_LINK_DELAY_MSB 0x0a86004e /* P1588 Port 1 RX PORT Link delay MSB Register */
#define TOP_1588_RX_PORT_2_LINK_DELAY_LSB 0x0a860050 /* P1588 Port 2 RX PORT Link delay LSB Register */
#define TOP_1588_RX_PORT_2_LINK_DELAY_MSB 0x0a860052 /* P1588 Port 2 RX PORT Link delay MSB Register */
#define TOP_1588_RX_PORT_3_LINK_DELAY_LSB 0x0a860054 /* P1588 Port 3 RX PORT Link delay LSB Register */
#define TOP_1588_RX_PORT_3_LINK_DELAY_MSB 0x0a860056 /* P1588 Port 3 RX PORT Link delay MSB Register */
#define TOP_1588_RX_PORT_4_LINK_DELAY_LSB 0x0a860058 /* P1588 Port 4 RX PORT Link delay LSB Register */
#define TOP_1588_RX_PORT_4_LINK_DELAY_MSB 0x0a86005a /* P1588 Port 4 RX PORT Link delay MSB Register */
#define TOP_1588_RX_PORT_5_LINK_DELAY_LSB 0x0a86005c /* P1588 Port 5 RX PORT Link delay LSB Register */
#define TOP_1588_RX_PORT_5_LINK_DELAY_MSB 0x0a86005e /* P1588 Port 5 RX PORT Link delay MSB Register */
#define TOP_1588_RX_PORT_6_LINK_DELAY_LSB 0x0a860060 /* P1588 Port 6 RX PORT Link delay LSB Register */
#define TOP_1588_RX_PORT_6_LINK_DELAY_MSB 0x0a860062 /* P1588 Port 6 RX PORT Link delay MSB Register */
#define TOP_1588_RX_PORT_7_LINK_DELAY_LSB 0x0a860064 /* P1588 Port 7 RX PORT Link delay LSB Register */
#define TOP_1588_RX_PORT_7_LINK_DELAY_MSB 0x0a860066 /* P1588 Port 7 RX PORT Link delay MSB Register */
#define TOP_1588_TX_PORT_0_TS_OFFSET_LSB 0x0a860068 /* P1588 Port 0 TX Timestamp Offset LSB Register */
#define TOP_1588_TX_PORT_0_TS_OFFSET_MSB 0x0a86006a /* P1588 Port 0 TX Timestamp Offset MSB Register */
#define TOP_1588_TX_PORT_1_TS_OFFSET_LSB 0x0a86006c /* P1588 Port 1 TX Timestamp Offset LSB Register */
#define TOP_1588_TX_PORT_1_TS_OFFSET_MSB 0x0a86006e /* P1588 Port 1 TX Timestamp Offset MSB Register */
#define TOP_1588_TX_PORT_2_TS_OFFSET_LSB 0x0a860070 /* P1588 Port 2 TX Timestamp Offset LSB Register */
#define TOP_1588_TX_PORT_2_TS_OFFSET_MSB 0x0a860072 /* P1588 Port 2 TX Timestamp Offset MSB Register */
#define TOP_1588_TX_PORT_3_TS_OFFSET_LSB 0x0a860074 /* P1588 Port 3 TX Timestamp Offset LSB Register */
#define TOP_1588_TX_PORT_3_TS_OFFSET_MSB 0x0a860076 /* P1588 Port 3 TX Timestamp Offset MSB Register */
#define TOP_1588_TX_PORT_4_TS_OFFSET_LSB 0x0a860078 /* P1588 Port 4 TX Timestamp Offset LSB Register */
#define TOP_1588_TX_PORT_4_TS_OFFSET_MSB 0x0a86007a /* P1588 Port 4 TX Timestamp Offset MSB Register */
#define TOP_1588_TX_PORT_5_TS_OFFSET_LSB 0x0a86007c /* P1588 Port 5 TX Timestamp Offset LSB Register */
#define TOP_1588_TX_PORT_5_TS_OFFSET_MSB 0x0a86007e /* P1588 Port 5 TX Timestamp Offset MSB Register */
#define TOP_1588_TX_PORT_6_TS_OFFSET_LSB 0x0a860080 /* P1588 Port 6 TX Timestamp Offset LSB Register */
#define TOP_1588_TX_PORT_6_TS_OFFSET_MSB 0x0a860082 /* P1588 Port 6 TX Timestamp Offset MSB Register */
#define TOP_1588_TX_PORT_7_TS_OFFSET_LSB 0x0a860084 /* P1588 Port 7 TX Timestamp Offset LSB Register */
#define TOP_1588_TX_PORT_7_TS_OFFSET_MSB 0x0a860086 /* P1588 Port 7 TX Timestamp Offset MSB Register */
#define TOP_1588_RX_PORT_0_TS_OFFSET_LSB 0x0a860088 /* P1588 Port 0 RX Timestamp Offset LSB Register */
#define TOP_1588_RX_PORT_0_TS_OFFSET_MSB 0x0a86008a /* P1588 Port 0 RX Timestamp Offset MSB Register */
#define TOP_1588_RX_PORT_1_TS_OFFSET_LSB 0x0a86008c /* P1588 Port 1 RX Timestamp Offset LSB Register */
#define TOP_1588_RX_PORT_1_TS_OFFSET_MSB 0x0a86008e /* P1588 Port 1 RX Timestamp Offset MSB Register */
#define TOP_1588_RX_PORT_2_TS_OFFSET_LSB 0x0a860090 /* P1588 Port 2 RX Timestamp Offset LSB Register */
#define TOP_1588_RX_PORT_2_TS_OFFSET_MSB 0x0a860092 /* P1588 Port 2 RX Timestamp Offset MSB Register */
#define TOP_1588_RX_PORT_3_TS_OFFSET_LSB 0x0a860094 /* P1588 Port 3 RX Timestamp Offset LSB Register */
#define TOP_1588_RX_PORT_3_TS_OFFSET_MSB 0x0a860096 /* P1588 Port 3 RX Timestamp Offset MSB Register */
#define TOP_1588_RX_PORT_4_TS_OFFSET_LSB 0x0a860098 /* P1588 Port 4 RX Timestamp Offset LSB Register */
#define TOP_1588_RX_PORT_4_TS_OFFSET_MSB 0x0a86009a /* P1588 Port 4 RX Timestamp Offset MSB Register */
#define TOP_1588_RX_PORT_5_TS_OFFSET_LSB 0x0a86009c /* P1588 Port 5 RX Timestamp Offset LSB Register */
#define TOP_1588_RX_PORT_5_TS_OFFSET_MSB 0x0a86009e /* P1588 Port 5 RX Timestamp Offset MSB Register */
#define TOP_1588_RX_PORT_6_TS_OFFSET_LSB 0x0a8600a0 /* P1588 Port 6 RX Timestamp Offset LSB Register */
#define TOP_1588_RX_PORT_6_TS_OFFSET_MSB 0x0a8600a2 /* P1588 Port 6 RX Timestamp Offset MSB Register */
#define TOP_1588_RX_PORT_7_TS_OFFSET_LSB 0x0a8600a4 /* P1588 Port 7 RX Timestamp Offset LSB Register */
#define TOP_1588_RX_PORT_7_TS_OFFSET_MSB 0x0a8600a6 /* P1588 Port 7 RX Timestamp Offset MSB Register */
#define TOP_1588_TIME_CODE_0           0x0a8600a8 /* P1588 Original Time Code 0 Register */
#define TOP_1588_TIME_CODE_1           0x0a8600aa /* P1588 Original Time Code 1 Register */
#define TOP_1588_TIME_CODE_2           0x0a8600ac /* P1588 Original Time Code 2 Register */
#define TOP_1588_TIME_CODE_3           0x0a8600ae /* P1588 Original Time Code 3 Register */
#define TOP_1588_TIME_CODE_4           0x0a8600b0 /* P1588 Original Time Code 4 Register */
#define TOP_1588_DPLL_DB_LSB           0x0a8600b2 /* P1588 DPLL Debug LSB Register */
#define TOP_1588_DPLL_DB_MSB           0x0a8600b4 /* P1588 DPLL Debug MSB Register */
#define TOP_1588_DPLL_DB_SEL           0x0a8600b6 /* P1588 DPLL Debug Select Register */
#define TOP_1588_SHD_CTL               0x0a8600b8 /* P1588 Shadow Register Control */
#define TOP_1588_SHD_LD                0x0a8600ba /* P1588 Shadow Register Load */
#define TOP_1588_INT_MASK              0x0a8600bc /* P1588 Interrupt Mask Register */
#define TOP_1588_INT_STAT              0x0a8600be /* P1588 Interrupt Status Register */
#define TOP_1588_TX_CTL                0x0a8600c0 /* P1588 Transmit Control Register */
#define TOP_1588_RX_CTL                0x0a8600c2 /* P1588 Receive Control Register */
#define TOP_1588_RX_TX_CTL             0x0a8600c4 /* P1588 Receive/Transmit Control Register */
#define TOP_1588_VLAN_ITPID            0x0a8600c6 /* P1588 VLAN 1tags ITPID Register */
#define TOP_1588_VLAN_OTPID            0x0a8600c8 /* P1588 VLAN 2tags OTPID Register */
#define TOP_1588_OTHER_OTPID           0x0a8600ca /* P1588 VLAN 2Tags Other OTPID Register */
#define TOP_1588_NSE_DPLL_1            0x0a8600cc /* P1588 NSE DPLL Register 1 */
#define TOP_1588_NSE_DPLL_2_0          0x0a8600ce /* P1588 NSE DPLL Register 2(0) */
#define TOP_1588_NSE_DPLL_2_1          0x0a8600d0 /* P1588 NSE DPLL Register 2(1) */
#define TOP_1588_NSE_DPLL_2_2          0x0a8600d2 /* P1588 NSE DPLL Register 2(2) */
#define TOP_1588_NSE_DPLL_3_LSB        0x0a8600d4 /* P1588 NSE DPLL Register 3 LSB */
#define TOP_1588_NSE_DPLL_3_MSB        0x0a8600d6 /* P1588 NSE DPLL Register 3 MSB */
#define TOP_1588_NSE_DPLL_4            0x0a8600d8 /* P1588 NSE DPLL Register 4 */
#define TOP_1588_NSE_DPLL_5            0x0a8600da /* P1588 NSE DPLL Register 5 */
#define TOP_1588_NSE_DPLL_6            0x0a8600dc /* P1588 NSE DPLL Register 6 */
#define TOP_1588_NSE_DPLL_7_0          0x0a8600de /* P1588 NSE DPLL Register 7(0) */
#define TOP_1588_NSE_DPLL_7_1          0x0a8600e0 /* P1588 NSE DPLL Register 7(1) */
#define TOP_1588_NSE_DPLL_7_2          0x0a8600e2 /* P1588 NSE DPLL Register 7(2) */
#define TOP_1588_NSE_DPLL_7_3          0x0a8600e4 /* P1588 NSE DPLL Register 7(3) */
#define TOP_1588_NSE_NCO_1_LSB         0x0a8600e6 /* P1588 NSE DPLL NCO Register 1 LSB */
#define TOP_1588_NSE_NCO_1_MSB         0x0a8600e8 /* P1588 NSE DPLL NCO Register 1 MSB */
#define TOP_1588_NSE_NCO_2_0           0x0a8600ea /* P1588 NSE DPLL NCO Register 2(0) */
#define TOP_1588_NSE_NCO_2_1           0x0a8600ec /* P1588 NSE DPLL NCO Register 2(1) */
#define TOP_1588_NSE_NCO_2_2           0x0a8600ee /* P1588 NSE DPLL NCO Register 2(2) */
#define TOP_1588_NSE_NCO_3_0           0x0a8600f0 /* P1588 NSE DPLL NCO Register 3(0) */
#define TOP_1588_NSE_NCO_3_1           0x0a8600f2 /* P1588 NSE DPLL NCO Register 3(1) */
#define TOP_1588_NSE_NCO_3_2           0x0a8600f4 /* P1588 NSE DPLL NCO Register 3(2) */
#define TOP_1588_NSE_NCO_4             0x0a8600f6 /* P1588 NSE DPLL NCO Register 4 */
#define TOP_1588_NSE_NCO_5_0           0x0a8600f8 /* P1588 NSE DPLL NCO Register 5(0) */
#define TOP_1588_NSE_NCO_5_1           0x0a8600fa /* P1588 NSE DPLL NCO Register 5(1) */
#define TOP_1588_NSE_NCO_5_2           0x0a8600fc /* P1588 NSE DPLL NCO Register 5(2) */
#define TOP_1588_NSE_NCO_6             0x0a8600fe /* P1588 NSE DPLL NCO Register 6 */
#define TOP_1588_NSE_NCO_7_0           0x0a860100 /* P1588 NSE DPLL NCO Register 7(0) */
#define TOP_1588_NSE_NCO_7_1           0x0a860102 /* P1588 NSE DPLL NCO Register 7(1) */
#define TOP_1588_TX_COUNTER            0x0a860104 /* P1588 TX Counter */
#define TOP_1588_RX_COUNTER            0x0a860106 /* P1588 RX Counter */
#define TOP_1588_RX_TX_1588_COUNTER    0x0a860108 /* P1588 RX TX 1588 Counter */
#define TOP_1588_TS_READ_START_END     0x0a86010a /* P1588 Timestamp READ START/END Register */
#define TOP_1588_HEARTBEAT_0           0x0a86010c /* P1588 Heartbeat Register(0) */
#define TOP_1588_HEARTBEAT_1           0x0a86010e /* P1588 Heartbeat Register(1) */
#define TOP_1588_HEARTBEAT_2           0x0a860110 /* P1588 Heartbeat Register(2) */
#define TOP_1588_TIME_STAMP_0          0x0a860112 /* P1588 Time Stamp Register(0) */
#define TOP_1588_TIME_STAMP_1          0x0a860114 /* P1588 Time Stamp Register(1) */
#define TOP_1588_TIME_STAMP_2          0x0a860116 /* P1588 Time Stamp Register(2) */
#define TOP_1588_TIME_STAMP_INFO_1     0x0a860118 /* P1588 Time Stamp Register Info (1) */
#define TOP_1588_TIME_STAMP_INFO_2     0x0a86011a /* P1588 Time Stamp Register Info (2) */
#define TOP_1588_CNTR_DBG              0x0a86011c /* P1588 Control/Debug Register */
#define TOP_1588_MPLS_SPARE1           0x0a86011e /* P1588 CPU TX and RX Port Enable Registers */
#define TOP_1588_MPLS_SPARE2           0x0a860120 /* P1588 DA1 Registers */
#define TOP_1588_MPLS_SPARE3           0x0a860122 /* P1588 DA2 Registers */
#define TOP_1588_MPLS_SPARE4           0x0a860124 /* P1588 DA3 Registers */
#define TOP_1588_MPLS_SPARE5           0x0a860126 /* P1588 MPLS Special Label LSB Registers */
#define TOP_1588_MPLS_SPARE6           0x0a860128 /* P1588 MPLS SPECIAL lABEL 4 MSB Registers */
#define TOP_1588_MPLS_TX_CNTL          0x0a86012a /* P1588 MPLS TX Enable */
#define TOP_1588_MPLS_RX_CNTL          0x0a86012c /* P1588 MPLS RX Enable */
#define TOP_1588_MPLS_LABEL1_LSB_MASK  0x0a86012e /* P1588 MPLS label1 mask lsb bit */
#define TOP_1588_MPLS_LABEL1_MSB_MASK  0x0a860130 /* P1588 MPLS label1 mask msb bit (*Inband P0 Control) */
#define TOP_1588_MPLS_LABEL1_LSB_VALUE 0x0a860132 /* P1588 MPLS label1 value lsb bit */
#define TOP_1588_MPLS_LABEL1_MSB_VALUE 0x0a860134 /* P1588 MPLS label1 value msb bit (*HSR P0 Offset) */
#define TOP_1588_MPLS_LABEL2_LSB_MASK  0x0a860136 /* P1588 MPLS label2 mask lsb bit */
#define TOP_1588_MPLS_LABEL2_MSB_MASK  0x0a860138 /* P1588 MPLS label2 mask msb bit (*Inband P1 Control) */
#define TOP_1588_MPLS_LABEL2_LSB_VALUE 0x0a86013a /* P1588 MPLS label2 value lsb bit */
#define TOP_1588_MPLS_LABEL2_MSB_VALUE 0x0a86013c /* P1588 MPLS label2 value msb bit (*HSR P1 Offset) */
#define TOP_1588_MPLS_LABEL3_LSB_MASK  0x0a86013e /* P1588 MPLS label3 mask lsb bit */
#define TOP_1588_MPLS_LABEL3_MSB_MASK  0x0a860140 /* P1588 MPLS label3 mask msb bit (*Inband P2 Control) */
#define TOP_1588_MPLS_LABEL3_LSB_VALUE 0x0a860142 /* P1588 MPLS label3 value lsb bit */
#define TOP_1588_MPLS_LABEL3_MSB_VALUE 0x0a860144 /* P1588 MPLS label3 value msb bit (*HSR P2 Offset) */
#define TOP_1588_MPLS_LABEL4_LSB_MASK  0x0a860146 /* P1588 MPLS label4 mask lsb bit */
#define TOP_1588_MPLS_LABEL4_MSB_MASK  0x0a860148 /* P1588 MPLS label4 mask msb bit (*Inband P3 Control) */
#define TOP_1588_MPLS_LABEL4_LSB_VALUE 0x0a86014a /* P1588 MPLS label4 value lsb bit */
#define TOP_1588_MPLS_LABEL4_MSB_VALUE 0x0a86014c /* P1588 MPLS label4 value msb bit (*HSR P3 Offset) */
#define TOP_1588_MPLS_LABEL5_LSB_MASK  0x0a86014e /* P1588 MPLS label5 mask lsb bit */
#define TOP_1588_MPLS_LABEL5_MSB_MASK  0x0a860150 /* P1588 MPLS label5 mask msb bit (*Inband P4 Control) */
#define TOP_1588_MPLS_LABEL5_LSB_VALUE 0x0a860152 /* P1588 MPLS label5 value lsb bit */
#define TOP_1588_MPLS_LABEL5_MSB_VALUE 0x0a860154 /* P1588 MPLS label5 value msb bit (*HSR P4 Offset) */
#define TOP_1588_MPLS_LABEL6_LSB_MASK  0x0a860156 /* P1588 MPLS label6 mask lsb bit */
#define TOP_1588_MPLS_LABEL6_MSB_MASK  0x0a860158 /* P1588 MPLS label6 mask msb bit (*Inband P5 Control) */
#define TOP_1588_MPLS_LABEL6_LSB_VALUE 0x0a86015a /* P1588 MPLS label6 value lsb bit */
#define TOP_1588_MPLS_LABEL6_MSB_VALUE 0x0a86015c /* P1588 MPLS label6 value msb bit (*HSR P5 Offset) */
#define TOP_1588_MPLS_LABEL7_LSB_MASK  0x0a86015e /* P1588 MPLS label7 mask lsb bit */
#define TOP_1588_MPLS_LABEL7_MSB_MASK  0x0a860160 /* P1588 MPLS label7 mask msb bit (*Inband P6 Control) */
#define TOP_1588_MPLS_LABEL7_LSB_VALUE 0x0a860162 /* P1588 MPLS label7 value lsb bit */
#define TOP_1588_MPLS_LABEL7_MSB_VALUE 0x0a860164 /* P1588 MPLS label7 value msb bit (*HSR P6 Offset) */
#define TOP_1588_MPLS_LABEL8_LSB_MASK  0x0a860166 /* P1588 MPLS label8 mask lsb bit */
#define TOP_1588_MPLS_LABEL8_MSB_MASK  0x0a860168 /* P1588 MPLS label8 mask msb bit (*Inband P7 Control) */
#define TOP_1588_MPLS_LABEL8_LSB_VALUE 0x0a86016a /* P1588 MPLS label8 value lsb bit */
#define TOP_1588_MPLS_LABEL8_MSB_VALUE 0x0a86016c /* P1588 MPLS label8 value msb bit (*HSR P7 Offset) */
#define TOP_1588_MPLS_LABEL9_LSB_MASK  0x0a86016e /* P1588 MPLS label9 mask lsb bit */
#define TOP_1588_MPLS_LABEL9_MSB_MASK  0x0a860170 /* P1588 MPLS label9 mask msb bit */
#define TOP_1588_MPLS_LABEL9_LSB_VALUE 0x0a860172 /* P1588 MPLS label9 value lsb bit */
#define TOP_1588_MPLS_LABEL9_MSB_VALUE 0x0a860174 /* P1588 MPLS label9 value msb bit */
#define TOP_1588_MPLS_LABEL10_LSB_MASK 0x0a860176 /* P1588 MPLS label10 mask lsb bit (*HSR Enable) */
#define TOP_1588_MPLS_LABEL10_MSB_MASK 0x0a860178 /* P1588 MPLS label10 mask msb bit (*HSR Ethertype) */
#define TOP_1588_MPLS_LABEL10_LSB_VALUE 0x0a86017a /* P1588 MPLS label10 value lsb bit (*HSR SNAP/LLC Control) */
#define TOP_1588_MPLS_LABEL10_MSB_VALUE 0x0a86017c /* P1588 MPLS label10 value msb bit */
#define TOP_1588_RX_TX_1588_COUNTER1   0x0a86017e /* P1588 RX TX CPU 1588 Counter */
#define TOP_1588_RX_CF_SPEC            0x0a860180 /* P1588 RX CF + Insertion */
#define TOP_1588_TX_CF_SPEC            0x0a860182 /* P1588 TX CS Update */
#define TOP_1588_MPLS_PACKET_ENABLE    0x0a860184 /* P1588 MPLS_PACKET ENABLE */
#define TOP_1588_TIMECODE_SEL          0x0a860186 /* P1588 TIMECODE SEL */
#define TOP_1588_TIME_STAMP_3          0x0a860188 /* P1588 Time Stamp Register(3) */
#define TOP_1588_TIME_STAMP            0x0a86018a /* P1588 Control/Debug Register */
#define TOP_1588_DM_TX_CNTL            0x0a86018c /* P1588 Delay Measurment Control Register */
#define TOP_1588_DM_RX_CNTL            0x0a86018e /* P1588 Delay Measurment Control Register */
#define TOP_1588_DM_ETHTYPE1           0x0a860190 /* P1588 Delay Measurment Ethtype1 Register */
#define TOP_1588_DM_ETHTYPE2           0x0a860192 /* P1588 Delay Measurment Ethtype2 Register */
#define TOP_1588_DM_ETHTYPE3           0x0a860194 /* P1588 Delay Measurment Ethtype3 Register */
#define TOP_1588_DM_ETHTYPE4           0x0a860196 /* P1588 Delay Measurment Ethtype4 Register */
#define TOP_1588_DM_ETHTYPE5           0x0a860198 /* P1588 Delay Measurment Ethtype5 Register */
#define TOP_1588_DM_ETHTYPE6           0x0a86019a /* P1588 Delay Measurment Ethtype6 Register */
#define TOP_1588_DM_ETHTYPE7           0x0a86019c /* P1588 Delay Measurment Ethtype7 Register */
#define TOP_1588_DM_ETHTYPE8           0x0a86019e /* P1588 Delay Measurment Ethtype8 Register */
#define TOP_1588_DM_ETHTYPE9           0x0a8601a0 /* P1588 Delay Measurment Ethtype9 Register */
#define TOP_1588_DM_ETHTYPE10          0x0a8601a2 /* P1588 Delay Measurment Ethtype10 Register */
#define TOP_1588_DM_ETHTYPE11          0x0a8601a4 /* P1588 Delay Measurment Ethtype11 Register */
#define TOP_1588_DM_ETHTYPE12          0x0a8601a6 /* P1588 Delay Measurment Ethtype12 Register */
#define TOP_1588_DM_ETHTYPE13          0x0a8601a8 /* P1588 Delay Measurment Ethtype13 Register */
#define TOP_1588_DM_IETF_OFFSET        0x0a8601aa /* P1588 Delay Measurment IETF Offfset Register */
#define TOP_1588_NTP_TIME_STAMP_0      0x0a8601ac /* P1588 NTP Counter Time Stamp0  Register */
#define TOP_1588_NTP_TIME_STAMP_1      0x0a8601ae /* P1588 NTP Counter Time Stamp1  Register */
#define TOP_1588_NTP_TIME_STAMP_2      0x0a8601b0 /* P1588 NTP Counter Time Stamp2  Register */
#define TOP_1588_NTP_TIME_STAMP_3      0x0a8601b2 /* P1588 NTP Counter Time Stamp3  Register */
#define TOP_1588_NTP_NCO_FREQ_0        0x0a8601b4 /* P1588 NTP NCO Frequency0  Register */
#define TOP_1588_NTP_NCO_FREQ_1        0x0a8601b6 /* P1588 NTP NCO Frequency1  Register */
#define TOP_1588_NTP_DOWN_CNTER_0      0x0a8601b8 /* P1588 NTP Down Counter 0  Register */
#define TOP_1588_NTP_DOWN_CNTER_1      0x0a8601ba /* P1588 NTP Down Counter 1  Register */
#define TOP_1588_NTP_ERR_LSB           0x0a8601bc /* P1588 NTP ERR LSB  Register */
#define TOP_1588_NTP_ERR_MSB           0x0a8601be /* P1588 NTP ERR MSB  Register */
#define TOP_1588_DM_MAC_L1_0           0x0a8601c0 /* P1588 DM MAC Address Local1 0 Register */
#define TOP_1588_DM_MAC_L1_1           0x0a8601c2 /* P1588 DM MAC Address Local1 0 Register */
#define TOP_1588_DM_MAC_L1_2           0x0a8601c4 /* P1588 DM MAC Address Local1 0 Register */
#define TOP_1588_DM_MAC_L2_0           0x0a8601c6 /* P1588 DM MAC Address Local2 0 Register */
#define TOP_1588_DM_MAC_L2_1           0x0a8601c8 /* P1588 DM MAC Address Local2 0 Register */
#define TOP_1588_DM_MAC_L2_2           0x0a8601ca /* P1588 DM MAC Address Local2 0 Register */
#define TOP_1588_DM_MAC_L3_0           0x0a8601cc /* P1588 DM MAC Address Local3 0 Register */
#define TOP_1588_DM_MAC_L3_1           0x0a8601ce /* P1588 DM MAC Address Local3 0 Register */
#define TOP_1588_DM_MAC_L3_2           0x0a8601d0 /* P1588 DM MAC Address Local3 0 Register */
#define TOP_1588_DM_MAC_CTL_0          0x0a8601d2 /* P1588 DM MAC cONTROL 0 Register */
#define TOP_1588_DM_MAC_CTL_1          0x0a8601d4 /* P1588 DM MAC CONTROL 1 Register */
#define TOP_1588_DM_MAC_CTL_2          0x0a8601d6 /* P1588 DM MAC CONTROL 2 Register */
#define TOP_1588_HEARTBEAT_3           0x0a8601d8 /* P1588 Heartbeat Register(3) */
#define TOP_1588_HEARTBEAT_4           0x0a8601da /* P1588 Heartbeat Register(4) */
#define TOP_1588_INBAND_CNTL_0         0x0a8601dc /* P1588 Inband Control Port0 Register */
#define TOP_1588_INBAND_CNTL_1         0x0a8601de /* P1588 Inband Control Port1 Register */
#define TOP_1588_INBAND_CNTL_2         0x0a8601e0 /* P1588 Inband Control Port2 Register */
#define TOP_1588_INBAND_CNTL_3         0x0a8601e2 /* P1588 Inband Control Port3 Register */
#define TOP_1588_INBAND_CNTL_4         0x0a8601e4 /* P1588 Inband Control Port4 Register */
#define TOP_1588_INBAND_CNTL_5         0x0a8601e6 /* P1588 Inband Control Port5 Register */
#define TOP_1588_INBAND_CNTL_6         0x0a8601e8 /* P1588 Inband Control Port6 Register */
#define TOP_1588_INBAND_CNTL_7         0x0a8601ea /* P1588 Inband Control Port7 Register */
#define TOP_1588_MEM_COUNTER           0x0a8601ec /* P1588 Memory Counter Register */
#define TOP_1588_TIMESTAMP_DELTA       0x0a8601ee /* P1588 Timestamp Delta Register */
#define TOP_1588_SOP_SEL               0x0a8601f0 /* P1588 SOP Selection Register */
#define TOP_1588_TIME_STAMP_INFO_3     0x0a8601f2 /* P1588 Time Stamp Register Info (3) */
#define TOP_1588_TIME_STAMP_INFO_4     0x0a8601f4 /* P1588 Time Stamp Register Info (4) */
#define TOP_1588_TIME_STAMP_INFO_5     0x0a8601f6 /* P1588 Time Stamp Register Info (5) */
#define TOP_1588_TIME_STAMP_INFO_6     0x0a8601f8 /* P1588 Time Stamp Register Info (6) */
#define TOP_1588_TIME_STAMP_INFO_7     0x0a8601fa /* P1588 Time Stamp Register Info (7) */
#define TOP_1588_TIME_STAMP_INFO_8     0x0a8601fc /* P1588 Time Stamp Register Info (8) */
#define TOP_1588_INBAND_SPARE1         0x0a8601fe /* P1588 Inband Spare1 Register */


/****************************************************************************
 * bcm89530_swsys_switch
 ***************************************************************************/
#define SWITCH_PAGE_00_G_PCTL0         0x0b000000 /* Port 0 10/100/1000 Control Register */
#define SWITCH_PAGE_00_G_PCTL1         0x0b000001 /* Port 1 10/100/1000 Control Register */
#define SWITCH_PAGE_00_G_PCTL2         0x0b000002 /* Port 2 10/100/1000 Control Register */
#define SWITCH_PAGE_00_G_PCTL3         0x0b000003 /* Port 3 10/100/1000 Control Register */
#define SWITCH_PAGE_00_G_PCTL4         0x0b000004 /* Port 4 10/100/1000 Control Register */
#define SWITCH_PAGE_00_G_PCTL5         0x0b000005 /* Port 5 10/100/1000 Control Register */
#define SWITCH_PAGE_00_G_PCTL6         0x0b000006 /* Port 6 10/100/1000 Control Register */
#define SWITCH_PAGE_00_P7_CTL          0x0b000007 /* Port 7 Control Register */
#define SWITCH_PAGE_00_IMP_CTL         0x0b000008 /* IMP Port (Port 8) Control Register */
#define SWITCH_PAGE_00_RX_GLOBAL_CTL   0x0b00000a /* RX Global Control register(Not2Release) */
#define SWITCH_PAGE_00_SWMODE          0x0b00000b /* Switch Mode Register */
#define SWITCH_PAGE_00_LED_REFLSH_CTL  0x0b00000f /* LED Configuration Register */
#define SWITCH_PAGE_00_LED_FUNC0_CTL   0x0b000010 /* LED Function 0 control register */
#define SWITCH_PAGE_00_LED_FUNC1_CTL   0x0b000012 /* LED Function 1 control register */
#define SWITCH_PAGE_00_LED_FUNC_MAP    0x0b000014 /* LED Function Map register */
#define SWITCH_PAGE_00_LED_EN_MAP      0x0b000016 /* LED Enable Map register */
#define SWITCH_PAGE_00_LED_MODE_MAP_0  0x0b000018 /* LED Mode map 0 register */
#define SWITCH_PAGE_00_LED_MODE_MAP_1  0x0b00001a /* LED Mode map 1 register */
#define SWITCH_PAGE_00_POST_LED_CTRL   0x0b00001d /* Post LED Control Register */
#define SWITCH_PAGE_00_DEBUG_REG       0x0b00001e /* Debug Control Register(Not2Release) */
#define SWITCH_PAGE_00_NEW_CTRL        0x0b000021 /* New Control Register */
#define SWITCH_PAGE_00_SWITCH_CTRL     0x0b000022 /* Switch Control Register (Not2Release) */
#define SWITCH_PAGE_00_PROTECTED_SEL   0x0b000024 /* Protected Port Select Register */
#define SWITCH_PAGE_00_WAN_PORT_SEL    0x0b000026 /* WAN Port select Register */
#define SWITCH_PAGE_00_RSV_MCAST_CTRL  0x0b00002f /* Reserved Multicast Register */
#define SWITCH_PAGE_00_TXQ_FLUSH_MODE  0x0b000031 /* TxQ Flush Mode Control Register(Not2Release) */
#define SWITCH_PAGE_00_ULF_DROP_MAP    0x0b000032 /* Unicast Lookup Failed Forward Map Register */
#define SWITCH_PAGE_00_MLF_DROP_MAP    0x0b000034 /* Multicast Lookup Failed Forward Map Register */
#define SWITCH_PAGE_00_MLF_IPMC_FWD_MAP 0x0b000036 /* IPMC Forward Map Register */
#define SWITCH_PAGE_00_RX_PAUSE_PASS   0x0b000038 /* Pause pass Through for RX Register */
#define SWITCH_PAGE_00_TX_PAUSE_PASS   0x0b00003a /* Pause pass Through for TX Register */
#define SWITCH_PAGE_00_DIS_LEARN       0x0b00003c /* Disable Learning Register */
#define SWITCH_PAGE_00_SFT_LRN_CTL     0x0b00003e /* Software Learning Control */
#define SWITCH_PAGE_00_LOW_POWER_EXP1  0x0b000040 /* Low Power Expansion I Register */
#define SWITCH_PAGE_00_PHY_INT_STS     0x0b000044 /* PHY Interrupt Status Register (Not2Release) */
#define SWITCH_PAGE_00_CTLREG_REG_SPARE 0x0b000054 /* Spare Register (Not2Release) */
#define SWITCH_PAGE_00_WATCH_DOG_RPT1  0x0b00007a /* Watch Dog Report 1 Register(Not2Release) */
#define SWITCH_PAGE_00_WATCH_DOG_RPT2  0x0b00007c /* Watch Dog Report 2 Register(Not2Release) */
#define SWITCH_PAGE_00_WATCH_DOG_RPT3  0x0b00007e /* Watch Dog Report 3 Register(Not2Release) */
#define SWITCH_PAGE_00_PAUSE_FRM_CTRL  0x0b000080 /* Pause Frame Detection Control Register */
#define SWITCH_PAGE_00_PAUSE_ST_ADDR   0x0b000081 /* PAUSE Frame DA Address */
#define SWITCH_PAGE_00_FAST_AGE_CTRL   0x0b000088 /* Fast Ageing Control Register */
#define SWITCH_PAGE_00_FAST_AGE_PORT   0x0b000089 /* Fast Ageing Port Control Register */
#define SWITCH_PAGE_00_FAST_AGE_VID    0x0b00008a /* Fast Ageing VID Control Register */
#define SWITCH_PAGE_00_LED_FUNC0_EXTD_CTL 0x0b000090 /* LED Function 0 Extended Control Register */
#define SWITCH_PAGE_00_LED_FUNC1_EXTD_CTL 0x0b000092 /* LED Function 1 Extended Control Register */
#define SWITCH_PAGE_00_LOW_POWER_CTRL  0x0b0000de /* Core-Level LOW Power Control Register */
#define SWITCH_PAGE_00_TCAM_CTRL       0x0b0000e8 /* TCAM Control Register */
#define SWITCH_PAGE_00_TCAM_CHKSUM_STS 0x0b0000ea /* TCAM Checksum Status Register */
#define SWITCH_PAGE_00_LIGHTSTACK_CTRL 0x0b0000ec /* Light Stacking Control Register */
#define SWITCH_PAGE_01_LNKSTS          0x0b000100 /* Link Status Summary Register */
#define SWITCH_PAGE_01_LNKSTSCHG       0x0b000102 /* Link Status Change Register */
#define SWITCH_PAGE_01_SPDSTS          0x0b000104 /* Port Speed Summary Register */
#define SWITCH_PAGE_01_DUPSTS          0x0b000108 /* Duplex status Summary Register */
#define SWITCH_PAGE_01_PAUSESTS        0x0b00010a /* Pause Status Summary Register */
#define SWITCH_PAGE_01_SRCADRCHG       0x0b00010e /* Source Address Change Register */
#define SWITCH_PAGE_01_LSA_PORT0       0x0b000110 /* Port 0 Last Source Address */
#define SWITCH_PAGE_01_LSA_PORT1       0x0b000116 /* Port 1 Last Source Address */
#define SWITCH_PAGE_01_LSA_PORT2       0x0b00011c /* Port 2 Last Source Address */
#define SWITCH_PAGE_01_LSA_PORT3       0x0b000122 /* Port 3 Last Source Address */
#define SWITCH_PAGE_01_LSA_PORT4       0x0b000128 /* Port 4 Last Source Address */
#define SWITCH_PAGE_01_LSA_PORT5       0x0b00012e /* Port 5 Last Source Address */
#define SWITCH_PAGE_01_LSA_PORT6       0x0b000134 /* Port 6 Last Source Address */
#define SWITCH_PAGE_01_LSA_PORT7       0x0b00013a /* Port 7 Last Source Address */
#define SWITCH_PAGE_01_LSA_MII_PORT    0x0b000140 /* Port 8 Last Source Address */
#define SWITCH_PAGE_01_BIST_STS0       0x0b000146 /* BIST Status Register 0 */
#define SWITCH_PAGE_01_BIST_STS1       0x0b00014c /* BIST Status Register 1 */
#define SWITCH_PAGE_01_PBPTRFIFO_0     0x0b000150 /* PBPTRFIFO Status Register 0(Not2Release) */
#define SWITCH_PAGE_01_PBPTRFIFO_1     0x0b000156 /* PBPTRFIFO Status Register 1(Not2Release) */
#define SWITCH_PAGE_01_RESET_STATUS    0x0b000190 /* Reset Status Register */
#define SWITCH_PAGE_01_STREG_REG_SPARE0 0x0b0001a0 /* Spare 0 Register (Not2Release) */
#define SWITCH_PAGE_01_STREG_REG_SPARE1 0x0b0001a4 /* Spare 1 Register (Not2Release) */
#define SWITCH_PAGE_02_GMNGCFG         0x0b000200 /* Global Management Configuration Register */
#define SWITCH_PAGE_02_IMP0_PRT_ID     0x0b000201 /* IMP/IMP0 Port ID Register */
#define SWITCH_PAGE_02_IMP1_PRT_ID     0x0b000202 /* IMP1 Port ID Register */
#define SWITCH_PAGE_02_BRCM_HDR_CTRL   0x0b000203 /* BRCM Header Control Register */
#define SWITCH_PAGE_02_SPTAGT          0x0b000206 /* Aging Time Control Register */
#define SWITCH_PAGE_02_BRCM_HDR_CTRL2  0x0b00020a /* BRCM Header Control 2 Register */
#define SWITCH_PAGE_02_IPG_SHRNK_CTRL  0x0b00020c /* IPG Shrink Control Register */
#define SWITCH_PAGE_02_MIRCAPCTL       0x0b000210 /* Mirror Capture Control Register */
#define SWITCH_PAGE_02_IGMIRCTL        0x0b000212 /* Ingress Mirror Control Register */
#define SWITCH_PAGE_02_IGMIRDIV        0x0b000214 /* Ingress Mirror Divider Register */
#define SWITCH_PAGE_02_IGMIRMAC        0x0b000216 /* Ingress Mirror Mac Address Register */
#define SWITCH_PAGE_02_EGMIRCTL        0x0b00021c /* Egress Mirror Control Register */
#define SWITCH_PAGE_02_EGMIRDIV        0x0b00021e /* Egress Mirror Divider Register */
#define SWITCH_PAGE_02_EGMIRMAC        0x0b000220 /* Egress Mirror MAC Address Register */
#define SWITCH_PAGE_02_SPANCTL         0x0b000226 /* SPAN RSPAN Control Register */
#define SWITCH_PAGE_02_RSPANVLAN       0x0b00022a /* RSPAN VLAN ID Register */
#define SWITCH_PAGE_02_MODEL_ID        0x0b000230 /* Model ID Register (Not2Release) */
#define SWITCH_PAGE_02_CHIP_REVID      0x0b000240 /* Chip Version ID Register */
#define SWITCH_PAGE_02_HL_PRTC_CTRL    0x0b000250 /* High Level Protocol Control Register */
#define SWITCH_PAGE_02_RST_MIB_CNT_EN  0x0b000254 /* Reset MIB Counter Enable Register */
#define SWITCH_PAGE_02_IPG_SHRINK_2G_WA 0x0b000258 /* IPG Shrink 2G Workaround Register (Not2Release) */
#define SWITCH_PAGE_02_MNGMODE_REG_SPARE0 0x0b000270 /* Spare 0 Register (Not2Release) */
#define SWITCH_PAGE_02_MNGMODE_REG_SPARE1 0x0b000274 /* Spare 1 Register (Not2Release) */
#define SWITCH_PAGE_03_INT_STS         0x0b000300 /* External Host Raw Interrupt Status Register */
#define SWITCH_PAGE_03_INT_EN          0x0b000308 /* External Host Interrupt Enable Register */
#define SWITCH_PAGE_03_IMP_SLEEP_TIMER 0x0b000310 /* IMP Port(port 8) Sleep Timer Register */
#define SWITCH_PAGE_03_PORT7_SLEEP_TIMER 0x0b000312 /* Port 7 Sleep Timer Register */
#define SWITCH_PAGE_03_WAN_SLEEP_TIMER 0x0b000314 /* WAN Port Sleep Timer Register */
#define SWITCH_PAGE_03_PORT_SLEEP_STS  0x0b000318 /* Port Sleep Status Register */
#define SWITCH_PAGE_03_INT_TRIGGER     0x0b000320 /* Interrupt Trigger Register */
#define SWITCH_PAGE_03_LINK_STS_INT_EN 0x0b000324 /* Link Status Interrupt Enable Register */
#define SWITCH_PAGE_03_ENG_DET_INT_EN  0x0b000328 /* Energy Detection Interrupt Enable Register */
#define SWITCH_PAGE_03_LPI_STS_CHG_INT_EN 0x0b00032a /* LPI Status Change Interrupt Enable Register */
#define SWITCH_PAGE_03_CPU_RESOURCE_ARBITER 0x0b000340 /* CPU Resource Arbitor Register */
#define SWITCH_PAGE_03_CPU_DATA_SHARE  0x0b000350 /* CPU Data Share Register */
#define SWITCH_PAGE_03_CPU_DATA_SHARE_1 0x0b000358 /* CPU Data Share 1 Register */
#define SWITCH_PAGE_03_MEM_ECC_ERR_INT_STS 0x0b000360 /* Memory ECC Double-Error-Detection Interrupt Status (Not2Release) */
#define SWITCH_PAGE_03_MEM_ECC_ERR_INT_EN 0x0b000362 /* Memory ECC Double-Error-Detection Interrupt Enable (Not2Release) */
#define SWITCH_PAGE_03_PORT_EVT_ECC_ERR_STS 0x0b000364 /* Per Port EVT Table ECC Double-Error-Detection Error Status (Not2Release) */
#define SWITCH_PAGE_03_PORT_MIB_ECC_ERR_STS 0x0b000366 /* Per Port MIB Counter ECC Double-Error-Detection Error Status (Not2Release) */
#define SWITCH_PAGE_03_PORT_TXQ_ECC_ERR_STS 0x0b000368 /* Per Port TXQ ECC Double-Error-Detection Error Status (Not2Release) */
#define SWITCH_PAGE_03_PROBE_BUS_CTL   0x0b000370 /* Probe Bus Control Registers(Not2Release) */
#define SWITCH_PAGE_03_MDC_EXTEND_CTRL 0x0b000374 /* MDC Extend Clock Control Register (Not2Release) */
#define SWITCH_PAGE_03_PPPOE_SESSION_PARSE_EN 0x0b000380 /* PPPoE Session Packet Parsing Enable Register */
#define SWITCH_PAGE_03_CTLREG_1_REG_SPARE0 0x0b000390 /* Spare 0 Register (Not2Release) */
#define SWITCH_PAGE_03_CTLREG_1_REG_SPARE1 0x0b000394 /* Spare 1 Register (Not2Release) */
#define SWITCH_PAGE_04_GARLCFG         0x0b000400 /* Global ARL Configuration Register */
#define SWITCH_PAGE_04_BPDU_MCADDR     0x0b000404 /* BPDU Multicast Address Register */
#define SWITCH_PAGE_04_MULTI_PORT_CTL  0x0b00040e /* Multiport Control Register */
#define SWITCH_PAGE_04_MULTIPORT_ADDR0 0x0b000410 /* Multiport Address 0 Register (Default for TS) */
#define SWITCH_PAGE_04_MPORTVEC0       0x0b000418 /* Multiport Vector 0 Register */
#define SWITCH_PAGE_04_MULTIPORT_ADDR1 0x0b000420 /* Multiport Address 1 Register */
#define SWITCH_PAGE_04_MPORTVEC1       0x0b000428 /* Multiport Vector 1 Register */
#define SWITCH_PAGE_04_MULTIPORT_ADDR2 0x0b000430 /* Multiport Address 2 Register */
#define SWITCH_PAGE_04_MPORTVEC2       0x0b000438 /* Multiport Vector 2 Register */
#define SWITCH_PAGE_04_MULTIPORT_ADDR3 0x0b000440 /* Multiport Address 3 Register */
#define SWITCH_PAGE_04_MPORTVEC3       0x0b000448 /* Multiport Vector 3 Register */
#define SWITCH_PAGE_04_MULTIPORT_ADDR4 0x0b000450 /* Multiport Address 4 Register */
#define SWITCH_PAGE_04_MPORTVEC4       0x0b000458 /* Multiport Vector 4 Register */
#define SWITCH_PAGE_04_MULTIPORT_ADDR5 0x0b000460 /* Multiport Address 5 Register */
#define SWITCH_PAGE_04_MPORTVEC5       0x0b000468 /* Multiport Vector 5 Register */
#define SWITCH_PAGE_04_ARL_BIN_FULL_CNTR 0x0b000470 /* ARL Bin Full Counter Register */
#define SWITCH_PAGE_04_ARL_BIN_FULL_FWD 0x0b000474 /* ARL Biin Full Forward Enable Register */
#define SWITCH_PAGE_04_ARL_SEED        0x0b000476 /* ARL Programmable seed */
#define SWITCH_PAGE_04_ARLCTL_REG_SPARE0 0x0b000480 /* Spare 0 Register (Not2Release) */
#define SWITCH_PAGE_04_ARLCTL_REG_SPARE1 0x0b000484 /* Spare 1 Register (Not2Release) */
#define SWITCH_PAGE_04_ARL_TCAM_CTRL   0x0b000490 /* ARL TCAM Control Register */
#define SWITCH_PAGE_04_ARL_TCAM_STS    0x0b000494 /* ARL TCAM Status Register */
#define SWITCH_PAGE_04_ARL_TCAM_FULL_CNTR 0x0b000498 /* ARL TCAM Full Counter Register */
#define SWITCH_PAGE_05_ARLA_RWCTL      0x0b000500 /* ARL Read/Write Control Register */
#define SWITCH_PAGE_05_ARLA_MAC        0x0b000502 /* MAC Address Index Register */
#define SWITCH_PAGE_05_ARLA_VID        0x0b000508 /* VID Index Register */
#define SWITCH_PAGE_05_ARLA_MACVID_ENTRY0 0x0b000510 /* ARL MAC/VID Entry 0 Register */
#define SWITCH_PAGE_05_ARLA_FWD_ENTRY0 0x0b000518 /* ARL FWD Entry 0 Register */
#define SWITCH_PAGE_05_ARLA_MACVID_ENTRY1 0x0b000520 /* ARL MAC/VID Entry 1 Register */
#define SWITCH_PAGE_05_ARLA_FWD_ENTRY1 0x0b000528 /* ARL FWD Entry 1 Register */
#define SWITCH_PAGE_05_ARLA_MACVID_ENTRY2 0x0b000530 /* ARL MAC/VID Entry 2 Register */
#define SWITCH_PAGE_05_ARLA_FWD_ENTRY2 0x0b000538 /* ARL FWD Entry 2 Register */
#define SWITCH_PAGE_05_ARLA_MACVID_ENTRY3 0x0b000540 /* ARL MAC/VID Entry 3 Register */
#define SWITCH_PAGE_05_ARLA_FWD_ENTRY3 0x0b000548 /* ARL FWD Entry 3 Register */
#define SWITCH_PAGE_05_ARLA_SRCH_CTL   0x0b000550 /* ARL Search Control Register */
#define SWITCH_PAGE_05_ARLA_SRCH_ADR   0x0b000551 /* ARL Search Address Register */
#define SWITCH_PAGE_05_ARLA_SRCH_RSLT_0_MACVID 0x0b000560 /* ARL Search MAC/VID Result 0 Register */
#define SWITCH_PAGE_05_ARLA_SRCH_RSLT_0 0x0b000568 /* ARL Search Result 0 Register */
#define SWITCH_PAGE_05_ARLA_SRCH_RSLT_1_MACVID 0x0b000570 /* ARL Search MAC/VID Result 1 Register */
#define SWITCH_PAGE_05_ARLA_SRCH_RSLT_1 0x0b000578 /* ARL Search Result 1 Register */
#define SWITCH_PAGE_05_ARLA_VTBL_RWCTRL 0x0b000580 /* VTBL Read/Write/Clear Control Register */
#define SWITCH_PAGE_05_ARLA_VTBL_ADDR  0x0b000581 /* VTBL Address Index Register */
#define SWITCH_PAGE_05_ARLA_VTBL_ENTRY 0x0b000583 /* VTBL Entry Register */
#define SWITCH_PAGE_05_ARLACCS_REG_SPARE0 0x0b000590 /* Spare 0 Register (Not2Release) */
#define SWITCH_PAGE_05_ARLACCS_REG_SPARE1 0x0b000594 /* Spare 1 Register (Not2Release) */
#define SWITCH_PAGE_08_MEM_CTRL        0x0b000800 /* Memory Debug Control Register */
#define SWITCH_PAGE_08_MEM_ADDR        0x0b000801 /* Memory Debug Address Register */
#define SWITCH_PAGE_08_MEM_DEBUG_DATA_0_0 0x0b000808 /* Memory Debug Data 0_0 Register */
#define SWITCH_PAGE_08_MEM_DEBUG_DATA_0_1 0x0b000810 /* Memory Debug Data 0_1 Register */
#define SWITCH_PAGE_08_MEM_DEBUG_DATA_1_0 0x0b000812 /* Memory Debug Data 1_0 Register */
#define SWITCH_PAGE_08_MEM_DEBUG_DATA_1_1 0x0b00081a /* Memory Debug Data 1_1 Register */
#define SWITCH_PAGE_08_MEM_FRM_ADDR    0x0b000820 /* Frame Memory Address Register */
#define SWITCH_PAGE_08_MEM_FRM_DATA0   0x0b000830 /* Frame Memory Data 1st Register */
#define SWITCH_PAGE_08_MEM_FRM_DATA1   0x0b000838 /* Frame Memory Data 2st Register */
#define SWITCH_PAGE_08_MEM_FRM_DATA2   0x0b000840 /* Frame Memory Data 3st Register */
#define SWITCH_PAGE_08_MEM_FRM_DATA3   0x0b000848 /* Frame Memory Data 4th Register */
#define SWITCH_PAGE_08_MEM_BTM_DATA0   0x0b000850 /* Buffer Tag Memory Register 0 */
#define SWITCH_PAGE_08_MEM_BTM_DATA1   0x0b000858 /* Buffer Tag Memory Register 1 */
#define SWITCH_PAGE_08_MEM_BFC_ADDR    0x0b000860 /* Buffer Control Memory Address Register */
#define SWITCH_PAGE_08_MEM_BFC_DATA    0x0b000862 /* Buffer Control Memory Data Register */
#define SWITCH_PAGE_08_PRS_FIFO_DEBUG_CTRL 0x0b000870 /* PRS_FIFO Debug Control Register(Not2Release) */
#define SWITCH_PAGE_08_PRS_FIFO_DEBUG_DATA 0x0b000871 /* PRS_FIFO Debug Data Register(Not2Release) */
#define SWITCH_PAGE_08_MEM_REG_SPARE0  0x0b0008a8 /* Spare 0 Register (Not2Release) */
#define SWITCH_PAGE_08_MEM_REG_SPARE1  0x0b0008ac /* Spare 1 Register (Not2Release) */
#define SWITCH_PAGE_08_MEM_MISC_CTRL   0x0b0008b0 /* Memory Misc Control Register */
#define SWITCH_PAGE_08_MEM_TEST_CTRL0  0x0b0008b4 /* Memory Test Control 0 Register */
#define SWITCH_PAGE_08_MEM_TEST_CTRL1  0x0b0008b8 /* Memory Test Control 1 Register */
#define SWITCH_PAGE_08_MEM_TEST_CTRL2  0x0b0008bc /* Memory Test Control 2 Register */
#define SWITCH_PAGE_08_MEM_TEST_CTRL3  0x0b0008c0 /* Memory Test Control 3 Register */
#define SWITCH_PAGE_08_MEM_TEST_CTRL4  0x0b0008c4 /* Memory Test Control 4 Register */
#define SWITCH_PAGE_08_MEM_TEST_CTRL5  0x0b0008c8 /* Memory Test Control 5 Register */
#define SWITCH_PAGE_08_MEM_PSM_VDD_CTRL 0x0b0008e0 /* Memory PSM_VDD Pin Control register */
#define SWITCH_PAGE_09_PORT0_DEBUG     0x0b000900 /* PORT0 DEBUG */
#define SWITCH_PAGE_09_PORT1_DEBUG     0x0b000910 /* PORT1 DEBUG */
#define SWITCH_PAGE_09_PORT2_DEBUG     0x0b000920 /* PORT2 DEBUG */
#define SWITCH_PAGE_09_PORT3_DEBUG     0x0b000930 /* PORT3 DEBUG */
#define SWITCH_PAGE_09_PORT4_DEBUG     0x0b000940 /* PORT4 DEBUG */
#define SWITCH_PAGE_09_PORT5_DEBUG     0x0b000950 /* PORT5 DEBUG */
#define SWITCH_PAGE_09_PORT6_DEBUG     0x0b000960 /* PORT6 DEBUG */
#define SWITCH_PAGE_09_PORT7_DEBUG     0x0b000970 /* PORT7 DEBUG */
#define SWITCH_PAGE_09_PORT8_DEBUG     0x0b000980 /* PORT8 DEBUG */
#define SWITCH_PAGE_0A_FC_DIAG_CTRL    0x0b000a00 /* Flowcon Diagnosis Control Register */
#define SWITCH_PAGE_0A_FC_CTRL_MODE    0x0b000a02 /* Flow Control Mode Selection Register */
#define SWITCH_PAGE_0A_FC_CTRL_PORT    0x0b000a03 /* Flow Control Port Selection Register */
#define SWITCH_PAGE_0A_FC_OOB_PAUSE_EN 0x0b000a04 /* OOB Pause Signal Enable Register (Release2Customer) */
#define SWITCH_PAGE_0A_PAUSE_TIME_MAX  0x0b000a10 /* MAX Quantum Pause Time Register */
#define SWITCH_PAGE_0A_PAUSE_TIME_MIN  0x0b000a12 /* MIN Quantum Pause Time Register */
#define SWITCH_PAGE_0A_PAUSE_TIME_RESET_THD 0x0b000a14 /* Quantum Pause Threshold Register */
#define SWITCH_PAGE_0A_PAUSE_TIME_UPDATE_PERIOD 0x0b000a16 /* Quantum Pause Update Period Register */
#define SWITCH_PAGE_0A_PAUSE_TIME_DEFAULT 0x0b000a18 /* Default Quantum Pause Time Register */
#define SWITCH_PAGE_0A_FC_MCAST_DROP_CTRL 0x0b000a1a /* Multicast Drop Control Register */
#define SWITCH_PAGE_0A_FC_PAUSE_DROP_CTRL 0x0b000a1c /* Pause/Drop Control Register */
#define SWITCH_PAGE_0A_FC_TXQ_THD_PAUSE_OFF 0x0b000a1e /* TXQ Pause Off Threshold Register */
#define SWITCH_PAGE_0A_FC_RX_RUNOFF    0x0b000a20 /* RX-Based Run-Off Register */
#define SWITCH_PAGE_0A_FC_RX_RSV_THD   0x0b000a22 /* RX-Based Reserved Register */
#define SWITCH_PAGE_0A_FC_RX_HYST_THD  0x0b000a24 /* RX-Based Hysteresis Register */
#define SWITCH_PAGE_0A_FC_RX_MAX_PTR   0x0b000a26 /* RX-Based Maximum Buffer Remap Register */
#define SWITCH_PAGE_0A_FC_SPARE_ZERO_REG 0x0b000a28 /* Flow Control Spare Zero Register */
#define SWITCH_PAGE_0A_FC_SPARE_ONE_REG 0x0b000a2a /* Flow Control Spare One Register */
#define SWITCH_PAGE_0A_FC_MON_TXQ0     0x0b000a30 /* Monitored TXQ 0 Register */
#define SWITCH_PAGE_0A_FC_MON_TXQ1     0x0b000a32 /* Monitored TXQ 1 Register */
#define SWITCH_PAGE_0A_FC_MON_TXQ2     0x0b000a34 /* Monitored TXQ 2 Register */
#define SWITCH_PAGE_0A_FC_MON_TXQ3     0x0b000a36 /* Monitored TXQ 3 Register */
#define SWITCH_PAGE_0A_FC_MON_TXQ4     0x0b000a38 /* Monitored TXQ 4 Register */
#define SWITCH_PAGE_0A_FC_MON_TXQ5     0x0b000a3a /* Monitored TXQ 5 Register */
#define SWITCH_PAGE_0A_FC_MON_TXQ6     0x0b000a3c /* Monitored TXQ 6 Register */
#define SWITCH_PAGE_0A_FC_MON_TXQ7     0x0b000a3e /* Monitored TXQ 7 Register */
#define SWITCH_PAGE_0A_FC_PEAK_TXQ0    0x0b000a40 /* Peak TXQ 0 Counter Register */
#define SWITCH_PAGE_0A_FC_PEAK_TXQ1    0x0b000a42 /* Peak TXQ 1 Counter Register */
#define SWITCH_PAGE_0A_FC_PEAK_TXQ2    0x0b000a44 /* Peak TXQ 2 Counter Register */
#define SWITCH_PAGE_0A_FC_PEAK_TXQ3    0x0b000a46 /* Peak TXQ 3 Counter Register */
#define SWITCH_PAGE_0A_FC_PEAK_TXQ4    0x0b000a48 /* Peak TXQ 4 Counter Register */
#define SWITCH_PAGE_0A_FC_PEAK_TXQ5    0x0b000a4a /* Peak TXQ 5 Counter Register */
#define SWITCH_PAGE_0A_FC_PEAK_TXQ6    0x0b000a4c /* Peak TXQ 6 Counter Register */
#define SWITCH_PAGE_0A_FC_PEAK_TXQ7    0x0b000a4e /* Peak TXQ 7 Counter Register */
#define SWITCH_PAGE_0A_FC_PEAK_TOTAL_USED 0x0b000a50 /* Peak Total Used Count Register */
#define SWITCH_PAGE_0A_FC_TOTAL_USED   0x0b000a52 /* Total Used Count Register */
#define SWITCH_PAGE_0A_FC_PEAK_RX_CNT  0x0b000a54 /* Peak RX Counter Register */
#define SWITCH_PAGE_0A_FC_LINK_PORTMAP 0x0b000a56 /* PHY Link Information Register */
#define SWITCH_PAGE_0A_FC_GIGA_PORTMAP 0x0b000a58 /* Giga Speed Information Register */
#define SWITCH_PAGE_0A_FC_CONG_PORTMAP_PN_PORT0 0x0b000a60 /* Port N (0 to 6) Congested PortMap Register */
#define SWITCH_PAGE_0A_FC_CONG_PORTMAP_PN_PORT1 0x0b000a62 /* Port N (0 to 6) Congested PortMap Register */
#define SWITCH_PAGE_0A_FC_CONG_PORTMAP_PN_PORT2 0x0b000a64 /* Port N (0 to 6) Congested PortMap Register */
#define SWITCH_PAGE_0A_FC_CONG_PORTMAP_PN_PORT3 0x0b000a66 /* Port N (0 to 6) Congested PortMap Register */
#define SWITCH_PAGE_0A_FC_CONG_PORTMAP_PN_PORT4 0x0b000a68 /* Port N (0 to 6) Congested PortMap Register */
#define SWITCH_PAGE_0A_FC_CONG_PORTMAP_PN_PORT5 0x0b000a6a /* Port N (0 to 6) Congested PortMap Register */
#define SWITCH_PAGE_0A_FC_CONG_PORTMAP_PN_PORT6 0x0b000a6c /* Port N (0 to 6) Congested PortMap Register */
#define SWITCH_PAGE_0A_FC_CONG_PORTMAP_P7 0x0b000a6e /* Port 7 Congested PortMap Register */
#define SWITCH_PAGE_0A_FC_CONG_PORTMAP_P8 0x0b000a70 /* Port 8 Congested PortMap Register */
#define SWITCH_PAGE_0A_FC_PAUSE_HIS    0x0b000a78 /* Pause History Register */
#define SWITCH_PAGE_0A_FC_TX_QUANTUM_PAUSE_HIS 0x0b000a7a /* TX Quantum Pause History Register */
#define SWITCH_PAGE_0A_FC_RX_PAUSE_HIS 0x0b000a7c /* RX Based Pause History Register */
#define SWITCH_PAGE_0A_FC_RXBUF_ERR_HIS 0x0b000a7e /* RX Buffer Error History Register */
#define SWITCH_PAGE_0A_FC_TXQ_CONG_PORTMAP_PN_PORT0 0x0b000a80 /* Port N (0 to 6) TXQ Congested PortMap Register */
#define SWITCH_PAGE_0A_FC_TXQ_CONG_PORTMAP_PN_PORT1 0x0b000a82 /* Port N (0 to 6) TXQ Congested PortMap Register */
#define SWITCH_PAGE_0A_FC_TXQ_CONG_PORTMAP_PN_PORT2 0x0b000a84 /* Port N (0 to 6) TXQ Congested PortMap Register */
#define SWITCH_PAGE_0A_FC_TXQ_CONG_PORTMAP_PN_PORT3 0x0b000a86 /* Port N (0 to 6) TXQ Congested PortMap Register */
#define SWITCH_PAGE_0A_FC_TXQ_CONG_PORTMAP_PN_PORT4 0x0b000a88 /* Port N (0 to 6) TXQ Congested PortMap Register */
#define SWITCH_PAGE_0A_FC_TXQ_CONG_PORTMAP_PN_PORT5 0x0b000a8a /* Port N (0 to 6) TXQ Congested PortMap Register */
#define SWITCH_PAGE_0A_FC_TXQ_CONG_PORTMAP_PN_PORT6 0x0b000a8c /* Port N (0 to 6) TXQ Congested PortMap Register */
#define SWITCH_PAGE_0A_FC_TXQ_CONG_PORTMAP_P7 0x0b000a8e /* Port 7 TXQ Congested PortMap Register */
#define SWITCH_PAGE_0A_FC_TXQ_CONG_PORTMAP_P8 0x0b000a90 /* Port 8 TXQ Congested PortMap Register */
#define SWITCH_PAGE_0A_FC_TOTAL_CONG_PORTMAP_PN_PORT0 0x0b000a9a /* Port N (0 to 6) Total Congested PortMap Register */
#define SWITCH_PAGE_0A_FC_TOTAL_CONG_PORTMAP_PN_PORT1 0x0b000a9c /* Port N (0 to 6) Total Congested PortMap Register */
#define SWITCH_PAGE_0A_FC_TOTAL_CONG_PORTMAP_PN_PORT2 0x0b000a9e /* Port N (0 to 6) Total Congested PortMap Register */
#define SWITCH_PAGE_0A_FC_TOTAL_CONG_PORTMAP_PN_PORT3 0x0b000aa0 /* Port N (0 to 6) Total Congested PortMap Register */
#define SWITCH_PAGE_0A_FC_TOTAL_CONG_PORTMAP_PN_PORT4 0x0b000aa2 /* Port N (0 to 6) Total Congested PortMap Register */
#define SWITCH_PAGE_0A_FC_TOTAL_CONG_PORTMAP_PN_PORT5 0x0b000aa4 /* Port N (0 to 6) Total Congested PortMap Register */
#define SWITCH_PAGE_0A_FC_TOTAL_CONG_PORTMAP_PN_PORT6 0x0b000aa6 /* Port N (0 to 6) Total Congested PortMap Register */
#define SWITCH_PAGE_0A_FC_TOTAL_CONG_PORTMAP_P7 0x0b000aa8 /* Port 7 Total Congested PortMap Register */
#define SWITCH_PAGE_0A_FC_TOTAL_CONG_PORTMAP_P8 0x0b000aaa /* Port 8 Total Congested PortMap Register */
#define SWITCH_PAGE_0B_FC_LAN_TXQ_THD_RSV_Q0 0x0b000b00 /* LAN Port Queue 0 Reserved Threshold Register */
#define SWITCH_PAGE_0B_FC_LAN_TXQ_THD_RSV_Q1 0x0b000b02 /* LAN Port Queue 1 Reserved Threshold Register */
#define SWITCH_PAGE_0B_FC_LAN_TXQ_THD_RSV_Q2 0x0b000b04 /* LAN Port Queue 2 Reserved Threshold Register */
#define SWITCH_PAGE_0B_FC_LAN_TXQ_THD_RSV_Q3 0x0b000b06 /* LAN Port Queue 3 Reserved Threshold Register */
#define SWITCH_PAGE_0B_FC_LAN_TXQ_THD_RSV_Q4 0x0b000b08 /* LAN Port Queue 4 Reserved Threshold Register */
#define SWITCH_PAGE_0B_FC_LAN_TXQ_THD_RSV_Q5 0x0b000b0a /* LAN Port Queue 5 Reserved Threshold Register */
#define SWITCH_PAGE_0B_FC_LAN_TXQ_THD_RSV_Q6 0x0b000b0c /* LAN Port Queue 6 Reserved Threshold Register */
#define SWITCH_PAGE_0B_FC_LAN_TXQ_THD_RSV_Q7 0x0b000b0e /* LAN Port Queue 7 Reserved Threshold Register */
#define SWITCH_PAGE_0B_FC_LAN_TXQ_THD_HYST_Q0 0x0b000b10 /* LAN Port Queue 0 Hysteresis Threshold Register */
#define SWITCH_PAGE_0B_FC_LAN_TXQ_THD_HYST_Q1 0x0b000b12 /* LAN Port Queue 1 Hysteresis Threshold Register */
#define SWITCH_PAGE_0B_FC_LAN_TXQ_THD_HYST_Q2 0x0b000b14 /* LAN Port Queue 2 Hysteresis Threshold Register */
#define SWITCH_PAGE_0B_FC_LAN_TXQ_THD_HYST_Q3 0x0b000b16 /* LAN Port Queue 3 Hysteresis Threshold Register */
#define SWITCH_PAGE_0B_FC_LAN_TXQ_THD_HYST_Q4 0x0b000b18 /* LAN Port Queue 4 Hysteresis Threshold Register */
#define SWITCH_PAGE_0B_FC_LAN_TXQ_THD_HYST_Q5 0x0b000b1a /* LAN Port Queue 5 Hysteresis Threshold Register */
#define SWITCH_PAGE_0B_FC_LAN_TXQ_THD_HYST_Q6 0x0b000b1c /* LAN Port Queue 6 Hysteresis Threshold Register */
#define SWITCH_PAGE_0B_FC_LAN_TXQ_THD_HYST_Q7 0x0b000b1e /* LAN Port Queue 7 Hysteresis Threshold Register */
#define SWITCH_PAGE_0B_FC_LAN_TXQ_THD_PAUSE_Q0 0x0b000b20 /* LAN Port Queue 0 Pause Threshold Register */
#define SWITCH_PAGE_0B_FC_LAN_TXQ_THD_PAUSE_Q1 0x0b000b22 /* LAN Port Queue 1 Pause Threshold Register */
#define SWITCH_PAGE_0B_FC_LAN_TXQ_THD_PAUSE_Q2 0x0b000b24 /* LAN Port Queue 2 Pause Threshold Register */
#define SWITCH_PAGE_0B_FC_LAN_TXQ_THD_PAUSE_Q3 0x0b000b26 /* LAN Port Queue 3 Pause Threshold Register */
#define SWITCH_PAGE_0B_FC_LAN_TXQ_THD_PAUSE_Q4 0x0b000b28 /* LAN Port Queue 4 Pause Threshold Register */
#define SWITCH_PAGE_0B_FC_LAN_TXQ_THD_PAUSE_Q5 0x0b000b2a /* LAN Port Queue 5 Pause Threshold Register */
#define SWITCH_PAGE_0B_FC_LAN_TXQ_THD_PAUSE_Q6 0x0b000b2c /* LAN Port Queue 6 Pause Threshold Register */
#define SWITCH_PAGE_0B_FC_LAN_TXQ_THD_PAUSE_Q7 0x0b000b2e /* LAN Port Queue 0 Pause Threshold Register */
#define SWITCH_PAGE_0B_FC_LAN_TXQ_THD_DROP_Q0 0x0b000b30 /* LAN Port Queue 0 DROP Threshold Register */
#define SWITCH_PAGE_0B_FC_LAN_TXQ_THD_DROP_Q1 0x0b000b32 /* LAN Port Queue 1 DROP Threshold Register */
#define SWITCH_PAGE_0B_FC_LAN_TXQ_THD_DROP_Q2 0x0b000b34 /* LAN Port Queue 2 DROP Threshold Register */
#define SWITCH_PAGE_0B_FC_LAN_TXQ_THD_DROP_Q3 0x0b000b36 /* LAN Port Queue 3 DROP Threshold Register */
#define SWITCH_PAGE_0B_FC_LAN_TXQ_THD_DROP_Q4 0x0b000b38 /* LAN Port Queue 4 DROP Threshold Register */
#define SWITCH_PAGE_0B_FC_LAN_TXQ_THD_DROP_Q5 0x0b000b3a /* LAN Port Queue 5 DROP Threshold Register */
#define SWITCH_PAGE_0B_FC_LAN_TXQ_THD_DROP_Q6 0x0b000b3c /* LAN Port Queue 6 DROP Threshold Register */
#define SWITCH_PAGE_0B_FC_LAN_TXQ_THD_DROP_Q7 0x0b000b3e /* LAN Port Queue 7 DROP Threshold Register */
#define SWITCH_PAGE_0B_FC_LAN_TOTAL_THD_HYST_Q0 0x0b000b40 /* LAN Port Queue 0 Total Hysteresis Threshold Register */
#define SWITCH_PAGE_0B_FC_LAN_TOTAL_THD_HYST_Q1 0x0b000b42 /* LAN Port Queue 2 Total Hysteresis Threshold Register */
#define SWITCH_PAGE_0B_FC_LAN_TOTAL_THD_HYST_Q2 0x0b000b44 /* LAN Port Queue 2 Total Hysteresis Threshold Register */
#define SWITCH_PAGE_0B_FC_LAN_TOTAL_THD_HYST_Q3 0x0b000b46 /* LAN Port Queue 3 Total Hysteresis Threshold Register */
#define SWITCH_PAGE_0B_FC_LAN_TOTAL_THD_HYST_Q4 0x0b000b48 /* LAN Port Queue 4 Total Hysteresis Threshold Register */
#define SWITCH_PAGE_0B_FC_LAN_TOTAL_THD_HYST_Q5 0x0b000b4a /* LAN Port Queue 5 Total Hysteresis Threshold Register */
#define SWITCH_PAGE_0B_FC_LAN_TOTAL_THD_HYST_Q6 0x0b000b4c /* LAN Port Queue 6 Total Hysteresis Threshold Register */
#define SWITCH_PAGE_0B_FC_LAN_TOTAL_THD_HYST_Q7 0x0b000b4e /* LAN Port Queue 7 Total Hysteresis Threshold Register */
#define SWITCH_PAGE_0B_FC_LAN_TOTAL_THD_PAUSE_Q0 0x0b000b50 /* LAN Port Queue 0 Total Pause Threshold Register */
#define SWITCH_PAGE_0B_FC_LAN_TOTAL_THD_PAUSE_Q1 0x0b000b52 /* LAN Port Queue 1 Total Pause Threshold Register */
#define SWITCH_PAGE_0B_FC_LAN_TOTAL_THD_PAUSE_Q2 0x0b000b54 /* LAN Port Queue 2 Total Pause Threshold Register */
#define SWITCH_PAGE_0B_FC_LAN_TOTAL_THD_PAUSE_Q3 0x0b000b56 /* LAN Port Queue 3 Total Pause Threshold Register */
#define SWITCH_PAGE_0B_FC_LAN_TOTAL_THD_PAUSE_Q4 0x0b000b58 /* LAN Port Queue 4 Total Pause Threshold Register */
#define SWITCH_PAGE_0B_FC_LAN_TOTAL_THD_PAUSE_Q5 0x0b000b5a /* LAN Port Queue 5 Total Pause Threshold Register */
#define SWITCH_PAGE_0B_FC_LAN_TOTAL_THD_PAUSE_Q6 0x0b000b5c /* LAN Port Queue 6 Total Pause Threshold Register */
#define SWITCH_PAGE_0B_FC_LAN_TOTAL_THD_PAUSE_Q7 0x0b000b5e /* LAN Port Queue 7 Total Pause Threshold Register */
#define SWITCH_PAGE_0B_FC_LAN_TOTAL_THD_DROP_Q0 0x0b000b60 /* LAN Port Queue 0 Total DROP Threshold Register */
#define SWITCH_PAGE_0B_FC_LAN_TOTAL_THD_DROP_Q1 0x0b000b62 /* LAN Port Queue 1 Total DROP Threshold Register */
#define SWITCH_PAGE_0B_FC_LAN_TOTAL_THD_DROP_Q2 0x0b000b64 /* LAN Port Queue 2 Total DROP Threshold Register */
#define SWITCH_PAGE_0B_FC_LAN_TOTAL_THD_DROP_Q3 0x0b000b66 /* LAN Port Queue 3 Total DROP Threshold Register */
#define SWITCH_PAGE_0B_FC_LAN_TOTAL_THD_DROP_Q4 0x0b000b68 /* LAN Port Queue 4 Total DROP Threshold Register */
#define SWITCH_PAGE_0B_FC_LAN_TOTAL_THD_DROP_Q5 0x0b000b6a /* LAN Port Queue 5 Total DROP Threshold Register */
#define SWITCH_PAGE_0B_FC_LAN_TOTAL_THD_DROP_Q6 0x0b000b6c /* LAN Port Queue 6 Total DROP Threshold Register */
#define SWITCH_PAGE_0B_FC_LAN_TOTAL_THD_DROP_Q7 0x0b000b6e /* LAN Port Queue 0 Total DROP Threshold Register */
#define SWITCH_PAGE_0C_P0_DEBUG_MUX    0x0b000c00 /* P0 DEBUG MUX */
#define SWITCH_PAGE_0C_P1_DEBUG_MUX    0x0b000c04 /* P1 DEBUG MUX */
#define SWITCH_PAGE_0C_P2_DEBUG_MUX    0x0b000c08 /* P2 DEBUG MUX */
#define SWITCH_PAGE_0C_P3_DEBUG_MUX    0x0b000c0c /* P3 DEBUG MUX */
#define SWITCH_PAGE_0C_P4_DEBUG_MUX    0x0b000c10 /* P4 DEBUG MUX */
#define SWITCH_PAGE_0C_P5_DEBUG_MUX    0x0b000c14 /* P5 DEBUG MUX */
#define SWITCH_PAGE_0C_P6_DEBUG_MUX    0x0b000c18 /* P6 DEBUG MUX */
#define SWITCH_PAGE_0C_P7_DEBUG_MUX    0x0b000c1c /* P7 DEBUG MUX */
#define SWITCH_PAGE_0C_IMP_DEBUG_MUX   0x0b000c20 /* IMP DEBUG MUX */
#define SWITCH_PAGE_0C_CFP_DEBUG_BUS_0 0x0b000c24 /* CFP DEBUG BUS 0 */
#define SWITCH_PAGE_0C_CFP_DEBUG_BUS_1 0x0b000c28 /* CFP DEBUG BUS 1 */
#define SWITCH_PAGE_0C_WRED_DEBUG_0    0x0b000c2c /* WRED DEBUG 0 */
#define SWITCH_PAGE_0C_WRED_DEBUG_1    0x0b000c30 /* WRED DEBUG 1 */
#define SWITCH_PAGE_0C_TOP_MISC_DEBUG_0 0x0b000c34 /* TOP MISC DEBUG 0 */
#define SWITCH_PAGE_0C_TOP_MISC_DEBUG_1 0x0b000c38 /* TOP MISC DEBUG 1 */
#define SWITCH_PAGE_0C_DIAGREG_BUFCON  0x0b000c3c /* DIAGREG BUFCON */
#define SWITCH_PAGE_0C_TESTBUS_P1588   0x0b000c40 /* TESTBUS P1588 */
#define SWITCH_PAGE_0C_FLOWCON_DEBUG_BUS 0x0b000c44 /* FLOWCON DEBUG BUS */
#define SWITCH_PAGE_0D_FC_IMP0_TXQ_THD_RSV_Q0 0x0b000d00 /* IMP0 Port Queue 0 Reserved Threshold Register */
#define SWITCH_PAGE_0D_FC_IMP0_TXQ_THD_RSV_Q1 0x0b000d02 /* IMP0 Port Queue 1 Reserved Threshold Register */
#define SWITCH_PAGE_0D_FC_IMP0_TXQ_THD_RSV_Q2 0x0b000d04 /* IMP0 Port Queue 2 Reserved Threshold Register */
#define SWITCH_PAGE_0D_FC_IMP0_TXQ_THD_RSV_Q3 0x0b000d06 /* IMP0 Port Queue 3 Reserved Threshold Register */
#define SWITCH_PAGE_0D_FC_IMP0_TXQ_THD_RSV_Q4 0x0b000d08 /* IMP0 Port Queue 4 Reserved Threshold Register */
#define SWITCH_PAGE_0D_FC_IMP0_TXQ_THD_RSV_Q5 0x0b000d0a /* IMP0 Port Queue 5 Reserved Threshold Register */
#define SWITCH_PAGE_0D_FC_IMP0_TXQ_THD_RSV_Q6 0x0b000d0c /* IMP0 Port Queue 6 Reserved Threshold Register */
#define SWITCH_PAGE_0D_FC_IMP0_TXQ_THD_RSV_Q7 0x0b000d0e /* IMP0 Port Queue 7 Reserved Threshold Register */
#define SWITCH_PAGE_0D_FC_IMP0_TXQ_THD_HYST_Q0 0x0b000d10 /* IMP0 Port Queue 0 Hysteresis Threshold Register */
#define SWITCH_PAGE_0D_FC_IMP0_TXQ_THD_HYST_Q1 0x0b000d12 /* IMP0 Port Queue 1 Hysteresis Threshold Register */
#define SWITCH_PAGE_0D_FC_IMP0_TXQ_THD_HYST_Q2 0x0b000d14 /* IMP0 Port Queue 2 Hysteresis Threshold Register */
#define SWITCH_PAGE_0D_FC_IMP0_TXQ_THD_HYST_Q3 0x0b000d16 /* IMP0 Port Queue 3 Hysteresis Threshold Register */
#define SWITCH_PAGE_0D_FC_IMP0_TXQ_THD_HYST_Q4 0x0b000d18 /* IMP0 Port Queue 4 Hysteresis Threshold Register */
#define SWITCH_PAGE_0D_FC_IMP0_TXQ_THD_HYST_Q5 0x0b000d1a /* IMP0 Port Queue 5 Hysteresis Threshold Register */
#define SWITCH_PAGE_0D_FC_IMP0_TXQ_THD_HYST_Q6 0x0b000d1c /* IMP0 Port Queue 6 Hysteresis Threshold Register */
#define SWITCH_PAGE_0D_FC_IMP0_TXQ_THD_HYST_Q7 0x0b000d1e /* IMP0 Port Queue 7 Hysteresis Threshold Register */
#define SWITCH_PAGE_0D_FC_IMP0_TXQ_THD_PAUSE_Q0 0x0b000d20 /* IMP0 Port Queue 0 Pause Threshold Register */
#define SWITCH_PAGE_0D_FC_IMP0_TXQ_THD_PAUSE_Q1 0x0b000d22 /* IMP0 Port Queue 1 Pause Threshold Register */
#define SWITCH_PAGE_0D_FC_IMP0_TXQ_THD_PAUSE_Q2 0x0b000d24 /* IMP0 Port Queue 2 Pause Threshold Register */
#define SWITCH_PAGE_0D_FC_IMP0_TXQ_THD_PAUSE_Q3 0x0b000d26 /* IMP0 Port Queue 3 Pause Threshold Register */
#define SWITCH_PAGE_0D_FC_IMP0_TXQ_THD_PAUSE_Q4 0x0b000d28 /* IMP0 Port Queue 4 Pause Threshold Register */
#define SWITCH_PAGE_0D_FC_IMP0_TXQ_THD_PAUSE_Q5 0x0b000d2a /* IMP0 Port Queue 5 Pause Threshold Register */
#define SWITCH_PAGE_0D_FC_IMP0_TXQ_THD_PAUSE_Q6 0x0b000d2c /* IMP0 Port Queue 6 Pause Threshold Register */
#define SWITCH_PAGE_0D_FC_IMP0_TXQ_THD_PAUSE_Q7 0x0b000d2e /* IMP0 Port Queue 0 Pause Threshold Register */
#define SWITCH_PAGE_0D_FC_IMP0_TXQ_THD_DROP_Q0 0x0b000d30 /* IMP0 Port Queue 0 DROP Threshold Register */
#define SWITCH_PAGE_0D_FC_IMP0_TXQ_THD_DROP_Q1 0x0b000d32 /* IMP0 Port Queue 1 DROP Threshold Register */
#define SWITCH_PAGE_0D_FC_IMP0_TXQ_THD_DROP_Q2 0x0b000d34 /* IMP0 Port Queue 2 DROP Threshold Register */
#define SWITCH_PAGE_0D_FC_IMP0_TXQ_THD_DROP_Q3 0x0b000d36 /* IMP0 Port Queue 3 DROP Threshold Register */
#define SWITCH_PAGE_0D_FC_IMP0_TXQ_THD_DROP_Q4 0x0b000d38 /* IMP0 Port Queue 4 DROP Threshold Register */
#define SWITCH_PAGE_0D_FC_IMP0_TXQ_THD_DROP_Q5 0x0b000d3a /* IMP0 Port Queue 5 DROP Threshold Register */
#define SWITCH_PAGE_0D_FC_IMP0_TXQ_THD_DROP_Q6 0x0b000d3c /* IMP0 Port Queue 6 DROP Threshold Register */
#define SWITCH_PAGE_0D_FC_IMP0_TXQ_THD_DROP_Q7 0x0b000d3e /* IMP0 Port Queue 7 DROP Threshold Register */
#define SWITCH_PAGE_0D_FC_IMP0_TOTAL_THD_HYST_Q0 0x0b000d40 /* IMP0 Port Queue 0 Total Hysteresis Threshold Register */
#define SWITCH_PAGE_0D_FC_IMP0_TOTAL_THD_HYST_Q1 0x0b000d42 /* IMP0 Port Queue 2 Total Hysteresis Threshold Register */
#define SWITCH_PAGE_0D_FC_IMP0_TOTAL_THD_HYST_Q2 0x0b000d44 /* IMP0 Port Queue 2 Total Hysteresis Threshold Register */
#define SWITCH_PAGE_0D_FC_IMP0_TOTAL_THD_HYST_Q3 0x0b000d46 /* IMP0 Port Queue 3 Total Hysteresis Threshold Register */
#define SWITCH_PAGE_0D_FC_IMP0_TOTAL_THD_HYST_Q4 0x0b000d48 /* IMP0 Port Queue 4 Total Hysteresis Threshold Register */
#define SWITCH_PAGE_0D_FC_IMP0_TOTAL_THD_HYST_Q5 0x0b000d4a /* IMP0 Port Queue 5 Total Hysteresis Threshold Register */
#define SWITCH_PAGE_0D_FC_IMP0_TOTAL_THD_HYST_Q6 0x0b000d4c /* IMP0 Port Queue 6 Total Hysteresis Threshold Register */
#define SWITCH_PAGE_0D_FC_IMP0_TOTAL_THD_HYST_Q7 0x0b000d4e /* IMP0 Port Queue 7 Total Hysteresis Threshold Register */
#define SWITCH_PAGE_0D_FC_IMP0_TOTAL_THD_PAUSE_Q0 0x0b000d50 /* IMP0 Port Queue 0 Total Pause Threshold Register */
#define SWITCH_PAGE_0D_FC_IMP0_TOTAL_THD_PAUSE_Q1 0x0b000d52 /* IMP0 Port Queue 1 Total Pause Threshold Register */
#define SWITCH_PAGE_0D_FC_IMP0_TOTAL_THD_PAUSE_Q2 0x0b000d54 /* IMP0 Port Queue 2 Total Pause Threshold Register */
#define SWITCH_PAGE_0D_FC_IMP0_TOTAL_THD_PAUSE_Q3 0x0b000d56 /* IMP0 Port Queue 3 Total Pause Threshold Register */
#define SWITCH_PAGE_0D_FC_IMP0_TOTAL_THD_PAUSE_Q4 0x0b000d58 /* IMP0 Port Queue 4 Total Pause Threshold Register */
#define SWITCH_PAGE_0D_FC_IMP0_TOTAL_THD_PAUSE_Q5 0x0b000d5a /* IMP0 Port Queue 5 Total Pause Threshold Register */
#define SWITCH_PAGE_0D_FC_IMP0_TOTAL_THD_PAUSE_Q6 0x0b000d5c /* IMP0 Port Queue 6 Total Pause Threshold Register */
#define SWITCH_PAGE_0D_FC_IMP0_TOTAL_THD_PAUSE_Q7 0x0b000d5e /* IMP0 Port Queue 7 Total Pause Threshold Register */
#define SWITCH_PAGE_0D_FC_IMP0_TOTAL_THD_DROP_Q0 0x0b000d60 /* IMP0 Port Queue 0 Total DROP Threshold Register */
#define SWITCH_PAGE_0D_FC_IMP0_TOTAL_THD_DROP_Q1 0x0b000d62 /* IMP0 Port Queue 1 Total DROP Threshold Register */
#define SWITCH_PAGE_0D_FC_IMP0_TOTAL_THD_DROP_Q2 0x0b000d64 /* IMP0 Port Queue 2 Total DROP Threshold Register */
#define SWITCH_PAGE_0D_FC_IMP0_TOTAL_THD_DROP_Q3 0x0b000d66 /* IMP0 Port Queue 3 Total DROP Threshold Register */
#define SWITCH_PAGE_0D_FC_IMP0_TOTAL_THD_DROP_Q4 0x0b000d68 /* IMP0 Port Queue 4 Total DROP Threshold Register */
#define SWITCH_PAGE_0D_FC_IMP0_TOTAL_THD_DROP_Q5 0x0b000d6a /* IMP0 Port Queue 5 Total DROP Threshold Register */
#define SWITCH_PAGE_0D_FC_IMP0_TOTAL_THD_DROP_Q6 0x0b000d6c /* IMP0 Port Queue 6 Total DROP Threshold Register */
#define SWITCH_PAGE_0D_FC_IMP0_TOTAL_THD_DROP_Q7 0x0b000d6e /* IMP0 Port Queue 0 Total DROP Threshold Register */
#define SWITCH_PAGE_0D_FC_IMP0_REG_SPARE0 0x0b000d70 /* Spare 0 Register (Not2Release) */
#define SWITCH_PAGE_0D_FC_IMP0_REG_SPARE1 0x0b000d72 /* Spare 1 Register (Not2Release) */
#define SWITCH_PAGE_0E_FC_WAN_IMP1_TXQ_THD_RSV_Q0 0x0b000e00 /* WAN/IMP1 Port Queue 0 Reserved Threshold Register */
#define SWITCH_PAGE_0E_FC_WAN_IMP1_TXQ_THD_RSV_Q1 0x0b000e02 /* WAN/IMP1 Port Queue 1 Reserved Threshold Register */
#define SWITCH_PAGE_0E_FC_WAN_IMP1_TXQ_THD_RSV_Q2 0x0b000e04 /* WAN/IMP1 Port Queue 2 Reserved Threshold Register */
#define SWITCH_PAGE_0E_FC_WAN_IMP1_TXQ_THD_RSV_Q3 0x0b000e06 /* WAN/IMP1 Port Queue 3 Reserved Threshold Register */
#define SWITCH_PAGE_0E_FC_WAN_IMP1_TXQ_THD_RSV_Q4 0x0b000e08 /* WAN/IMP1 Port Queue 4 Reserved Threshold Register */
#define SWITCH_PAGE_0E_FC_WAN_IMP1_TXQ_THD_RSV_Q5 0x0b000e0a /* WAN/IMP1 Port Queue 5 Reserved Threshold Register */
#define SWITCH_PAGE_0E_FC_WAN_IMP1_TXQ_THD_RSV_Q6 0x0b000e0c /* WAN/IMP1 Port Queue 6 Reserved Threshold Register */
#define SWITCH_PAGE_0E_FC_WAN_IMP1_TXQ_THD_RSV_Q7 0x0b000e0e /* WAN/IMP1 Port Queue 7 Reserved Threshold Register */
#define SWITCH_PAGE_0E_FC_WAN_IMP1_TXQ_THD_HYST_Q0 0x0b000e10 /* WAN/IMP1 Port Queue 0 Hysteresis Threshold Register */
#define SWITCH_PAGE_0E_FC_WAN_IMP1_TXQ_THD_HYST_Q1 0x0b000e12 /* WAN/IMP1 Port Queue 1 Hysteresis Threshold Register */
#define SWITCH_PAGE_0E_FC_WAN_IMP1_TXQ_THD_HYST_Q2 0x0b000e14 /* WAN/IMP1 Port Queue 2 Hysteresis Threshold Register */
#define SWITCH_PAGE_0E_FC_WAN_IMP1_TXQ_THD_HYST_Q3 0x0b000e16 /* WAN/IMP1 Port Queue 3 Hysteresis Threshold Register */
#define SWITCH_PAGE_0E_FC_WAN_IMP1_TXQ_THD_HYST_Q4 0x0b000e18 /* WAN/IMP1 Port Queue 4 Hysteresis Threshold Register */
#define SWITCH_PAGE_0E_FC_WAN_IMP1_TXQ_THD_HYST_Q5 0x0b000e1a /* WAN/IMP1 Port Queue 5 Hysteresis Threshold Register */
#define SWITCH_PAGE_0E_FC_WAN_IMP1_TXQ_THD_HYST_Q6 0x0b000e1c /* WAN/IMP1 Port Queue 6 Hysteresis Threshold Register */
#define SWITCH_PAGE_0E_FC_WAN_IMP1_TXQ_THD_HYST_Q7 0x0b000e1e /* WAN/IMP1 Port Queue 7 Hysteresis Threshold Register */
#define SWITCH_PAGE_0E_FC_WAN_IMP1_TXQ_THD_PAUSE_Q0 0x0b000e20 /* WAN/IMP1 Port Queue 0 Pause Threshold Register */
#define SWITCH_PAGE_0E_FC_WAN_IMP1_TXQ_THD_PAUSE_Q1 0x0b000e22 /* WAN/IMP1 Port Queue 1 Pause Threshold Register */
#define SWITCH_PAGE_0E_FC_WAN_IMP1_TXQ_THD_PAUSE_Q2 0x0b000e24 /* WAN/IMP1 Port Queue 2 Pause Threshold Register */
#define SWITCH_PAGE_0E_FC_WAN_IMP1_TXQ_THD_PAUSE_Q3 0x0b000e26 /* WAN/IMP1 Port Queue 3 Pause Threshold Register */
#define SWITCH_PAGE_0E_FC_WAN_IMP1_TXQ_THD_PAUSE_Q4 0x0b000e28 /* WAN/IMP1 Port Queue 4 Pause Threshold Register */
#define SWITCH_PAGE_0E_FC_WAN_IMP1_TXQ_THD_PAUSE_Q5 0x0b000e2a /* WAN/IMP1 Port Queue 5 Pause Threshold Register */
#define SWITCH_PAGE_0E_FC_WAN_IMP1_TXQ_THD_PAUSE_Q6 0x0b000e2c /* WAN/IMP1 Port Queue 6 Pause Threshold Register */
#define SWITCH_PAGE_0E_FC_WAN_IMP1_TXQ_THD_PAUSE_Q7 0x0b000e2e /* WAN/IMP1 Port Queue 0 Pause Threshold Register */
#define SWITCH_PAGE_0E_FC_WAN_IMP1_TXQ_THD_DROP_Q0 0x0b000e30 /* WAN/IMP1 Port Queue 0 DROP Threshold Register */
#define SWITCH_PAGE_0E_FC_WAN_IMP1_TXQ_THD_DROP_Q1 0x0b000e32 /* WAN/IMP1 Port Queue 1 DROP Threshold Register */
#define SWITCH_PAGE_0E_FC_WAN_IMP1_TXQ_THD_DROP_Q2 0x0b000e34 /* WAN/IMP1 Port Queue 2 DROP Threshold Register */
#define SWITCH_PAGE_0E_FC_WAN_IMP1_TXQ_THD_DROP_Q3 0x0b000e36 /* WAN/IMP1 Port Queue 3 DROP Threshold Register */
#define SWITCH_PAGE_0E_FC_WAN_IMP1_TXQ_THD_DROP_Q4 0x0b000e38 /* WAN/IMP1 Port Queue 4 DROP Threshold Register */
#define SWITCH_PAGE_0E_FC_WAN_IMP1_TXQ_THD_DROP_Q5 0x0b000e3a /* WAN/IMP1 Port Queue 5 DROP Threshold Register */
#define SWITCH_PAGE_0E_FC_WAN_IMP1_TXQ_THD_DROP_Q6 0x0b000e3c /* WAN/IMP1 Port Queue 6 DROP Threshold Register */
#define SWITCH_PAGE_0E_FC_WAN_IMP1_TXQ_THD_DROP_Q7 0x0b000e3e /* WAN/IMP1 Port Queue 7 DROP Threshold Register */
#define SWITCH_PAGE_0E_FC_WAN_IMP1_TOTAL_THD_HYST_Q0 0x0b000e40 /* WAN/IMP1 Port Queue 0 Total Hysteresis Threshold Register */
#define SWITCH_PAGE_0E_FC_WAN_IMP1_TOTAL_THD_HYST_Q1 0x0b000e42 /* WAN/IMP1 Port Queue 2 Total Hysteresis Threshold Register */
#define SWITCH_PAGE_0E_FC_WAN_IMP1_TOTAL_THD_HYST_Q2 0x0b000e44 /* WAN/IMP1 Port Queue 2 Total Hysteresis Threshold Register */
#define SWITCH_PAGE_0E_FC_WAN_IMP1_TOTAL_THD_HYST_Q3 0x0b000e46 /* WAN/IMP1 Port Queue 3 Total Hysteresis Threshold Register */
#define SWITCH_PAGE_0E_FC_WAN_IMP1_TOTAL_THD_HYST_Q4 0x0b000e48 /* WAN/IMP1 Port Queue 4 Total Hysteresis Threshold Register */
#define SWITCH_PAGE_0E_FC_WAN_IMP1_TOTAL_THD_HYST_Q5 0x0b000e4a /* WAN/IMP1 Port Queue 5 Total Hysteresis Threshold Register */
#define SWITCH_PAGE_0E_FC_WAN_IMP1_TOTAL_THD_HYST_Q6 0x0b000e4c /* WAN/IMP1 Port Queue 6 Total Hysteresis Threshold Register */
#define SWITCH_PAGE_0E_FC_WAN_IMP1_TOTAL_THD_HYST_Q7 0x0b000e4e /* WAN/IMP1 Port Queue 7 Total Hysteresis Threshold Register */
#define SWITCH_PAGE_0E_FC_WAN_IMP1_TOTAL_THD_PAUSE_Q0 0x0b000e50 /* WAN/IMP1 Port Queue 0 Total Pause Threshold Register */
#define SWITCH_PAGE_0E_FC_WAN_IMP1_TOTAL_THD_PAUSE_Q1 0x0b000e52 /* WAN/IMP1 Port Queue 1 Total Pause Threshold Register */
#define SWITCH_PAGE_0E_FC_WAN_IMP1_TOTAL_THD_PAUSE_Q2 0x0b000e54 /* WAN/IMP1 Port Queue 2 Total Pause Threshold Register */
#define SWITCH_PAGE_0E_FC_WAN_IMP1_TOTAL_THD_PAUSE_Q3 0x0b000e56 /* WAN/IMP1 Port Queue 3 Total Pause Threshold Register */
#define SWITCH_PAGE_0E_FC_WAN_IMP1_TOTAL_THD_PAUSE_Q4 0x0b000e58 /* WAN/IMP1 Port Queue 4 Total Pause Threshold Register */
#define SWITCH_PAGE_0E_FC_WAN_IMP1_TOTAL_THD_PAUSE_Q5 0x0b000e5a /* WAN/IMP1 Port Queue 5 Total Pause Threshold Register */
#define SWITCH_PAGE_0E_FC_WAN_IMP1_TOTAL_THD_PAUSE_Q6 0x0b000e5c /* WAN/IMP1 Port Queue 6 Total Pause Threshold Register */
#define SWITCH_PAGE_0E_FC_WAN_IMP1_TOTAL_THD_PAUSE_Q7 0x0b000e5e /* WAN/IMP1 Port Queue 7 Total Pause Threshold Register */
#define SWITCH_PAGE_0E_FC_WAN_IMP1_TOTAL_THD_DROP_Q0 0x0b000e60 /* WAN/IMP1 Port Queue 0 Total DROP Threshold Register */
#define SWITCH_PAGE_0E_FC_WAN_IMP1_TOTAL_THD_DROP_Q1 0x0b000e62 /* WAN/IMP1 Port Queue 1 Total DROP Threshold Register */
#define SWITCH_PAGE_0E_FC_WAN_IMP1_TOTAL_THD_DROP_Q2 0x0b000e64 /* WAN/IMP1 Port Queue 2 Total DROP Threshold Register */
#define SWITCH_PAGE_0E_FC_WAN_IMP1_TOTAL_THD_DROP_Q3 0x0b000e66 /* WAN/IMP1 Port Queue 3 Total DROP Threshold Register */
#define SWITCH_PAGE_0E_FC_WAN_IMP1_TOTAL_THD_DROP_Q4 0x0b000e68 /* WAN/IMP1 Port Queue 4 Total DROP Threshold Register */
#define SWITCH_PAGE_0E_FC_WAN_IMP1_TOTAL_THD_DROP_Q5 0x0b000e6a /* WAN/IMP1 Port Queue 5 Total DROP Threshold Register */
#define SWITCH_PAGE_0E_FC_WAN_IMP1_TOTAL_THD_DROP_Q6 0x0b000e6c /* WAN/IMP1 Port Queue 6 Total DROP Threshold Register */
#define SWITCH_PAGE_0E_FC_WAN_IMP1_TOTAL_THD_DROP_Q7 0x0b000e6e /* WAN/IMP1 Port Queue 0 Total DROP Threshold Register */
#define SWITCH_PAGE_0E_FC_WAN_IMP1_REG_SPARE0 0x0b000e70 /* Spare 0 Register (Not2Release) */
#define SWITCH_PAGE_0E_FC_WAN_IMP1_REG_SPARE1 0x0b000e72 /* Spare 1 Register (Not2Release) */
#define SWITCH_PAGE_20_TXOCTETS        0x0b002000 /* Tx Octets */
#define SWITCH_PAGE_20_TXDROPPKTS      0x0b002008 /* Tx Drop Packet Counter */
#define SWITCH_PAGE_20_TXQPKTQ0        0x0b00200c /* Tx Q0 Packet Counter */
#define SWITCH_PAGE_20_TXBROADCASTPKTS 0x0b002010 /* Tx Broadcast Packet Counter */
#define SWITCH_PAGE_20_TXMULTICASTPKTS 0x0b002014 /* Tx Multicast Packet Counter */
#define SWITCH_PAGE_20_TXUNICASTPKTS   0x0b002018 /* Tx Unicast Packet Counter */
#define SWITCH_PAGE_20_TXCOLLISIONS    0x0b00201c /* Tx Collision Counter */
#define SWITCH_PAGE_20_TXSINGLECOLLISION 0x0b002020 /* Tx Single Collision Counter */
#define SWITCH_PAGE_20_TXMULTIPLECOLLISION 0x0b002024 /* Tx Multiple collsion Counter */
#define SWITCH_PAGE_20_TXDEFERREDTRANSMIT 0x0b002028 /* Tx Deferred Transmit Counter */
#define SWITCH_PAGE_20_TXLATECOLLISION 0x0b00202c /* Tx Late Collision Counter */
#define SWITCH_PAGE_20_TXEXCESSIVECOLLISION 0x0b002030 /* Tx Excessive Collision Counter */
#define SWITCH_PAGE_20_TXFRAMEINDISC   0x0b002034 /* Tx Fram IN Disc Counter */
#define SWITCH_PAGE_20_TXPAUSEPKTS     0x0b002038 /* Tx Pause Packet Counter */
#define SWITCH_PAGE_20_TXQPKTQ1        0x0b00203c /* Tx Q1 Packet Counter */
#define SWITCH_PAGE_20_TXQPKTQ2        0x0b002040 /* Tx Q2 Packet Counter */
#define SWITCH_PAGE_20_TXQPKTQ3        0x0b002044 /* Tx Q3 Packet Counter */
#define SWITCH_PAGE_20_TXQPKTQ4        0x0b002048 /* Tx Q4 Packet Counter */
#define SWITCH_PAGE_20_TXQPKTQ5        0x0b00204c /* Tx Q5 Packet Counter */
#define SWITCH_PAGE_20_RXOCTETS        0x0b002050 /* Rx Packet Octets Counter */
#define SWITCH_PAGE_20_RXUNDERSIZEPKTS 0x0b002058 /* Rx Under Size Packet Octets Counter */
#define SWITCH_PAGE_20_RXPAUSEPKTS     0x0b00205c /* Rx Pause Packet Counter */
#define SWITCH_PAGE_20_RXPKTS64OCTETS  0x0b002060 /* Rx 64 Bytes Octets Counter */
#define SWITCH_PAGE_20_RXPKTS65TO127OCTETS 0x0b002064 /* Rx 65 to 127 Bytes Octets Counter */
#define SWITCH_PAGE_20_RXPKTS128TO255OCTETS 0x0b002068 /* Rx 128 to 255 Bytes Octets Counter */
#define SWITCH_PAGE_20_RXPKTS256TO511OCTETS 0x0b00206c /* Rx 256 to 511 Bytes Octets Counter */
#define SWITCH_PAGE_20_RXPKTS512TO1023OCTETS 0x0b002070 /* Rx 512 to 1023 Bytes Octets Counter */
#define SWITCH_PAGE_20_RXPKTS1024TOMAXPKTOCTETS 0x0b002074 /* Rx 1024 to MaxPkt Bytes Octets Counter */
#define SWITCH_PAGE_20_RXOVERSIZEPKTS  0x0b002078 /* Rx Over Size Packet Counter */
#define SWITCH_PAGE_20_RXJABBERS       0x0b00207c /* Rx Jabber Packet Counter */
#define SWITCH_PAGE_20_RXALIGNMENTERRORS 0x0b002080 /* Rx Alignment Error Counter */
#define SWITCH_PAGE_20_RXFCSERRORS     0x0b002084 /* Rx FCS Error Counter */
#define SWITCH_PAGE_20_RXGOODOCTETS    0x0b002088 /* Rx Good Packet Octet Counter */
#define SWITCH_PAGE_20_RXDROPPKTS      0x0b002090 /* Rx Drop Packet Counter */
#define SWITCH_PAGE_20_RXUNICASTPKTS   0x0b002094 /* Rx Unicast Packet Counter */
#define SWITCH_PAGE_20_RXMULTICASTPKTS 0x0b002098 /* Rx Multicast Packet Counter */
#define SWITCH_PAGE_20_RXBROADCASTPKTS 0x0b00209c /* Rx Broadcast Packet Counter */
#define SWITCH_PAGE_20_RXSACHANGES     0x0b0020a0 /* Rx SA Change Counter */
#define SWITCH_PAGE_20_RXFRAGMENTS     0x0b0020a4 /* Rx Fragment Counter */
#define SWITCH_PAGE_20_RXJUMBOPKT      0x0b0020a8 /* Jumbo Packet Counter */
#define SWITCH_PAGE_20_RXSYMBLERR      0x0b0020ac /* Rx Symbol Error Counter */
#define SWITCH_PAGE_20_INRANGEERRCOUNT 0x0b0020b0 /* InRangeErrCount Counter */
#define SWITCH_PAGE_20_OUTRANGEERRCOUNT 0x0b0020b4 /* OutRangeErrCount Counter */
#define SWITCH_PAGE_20_EEE_LPI_EVENT   0x0b0020b8 /* EEE Low-Power Idle Event Registers */
#define SWITCH_PAGE_20_EEE_LPI_DURATION 0x0b0020bc /* EEE Low-Power Idle Duration Registers */
#define SWITCH_PAGE_20_RXDISCARD       0x0b0020c0 /* Rx Discard Counter */
#define SWITCH_PAGE_20_TXQPKTQ6        0x0b0020c8 /* Tx Q6 Packet Counter */
#define SWITCH_PAGE_20_TXQPKTQ7        0x0b0020cc /* Tx Q7 Packet Counter */
#define SWITCH_PAGE_20_TXPKTS64OCTETS  0x0b0020d0 /* Tx 64 Bytes Octets Counter */
#define SWITCH_PAGE_20_TXPKTS65TO127OCTETS 0x0b0020d4 /* Tx 65 to 127 Bytes Octets Counter */
#define SWITCH_PAGE_20_TXPKTS128TO255OCTETS 0x0b0020d8 /* Tx 128 to 255 Bytes Octets Counter */
#define SWITCH_PAGE_20_TXPKTS256TO511OCTETS 0x0b0020dc /* Tx 256 to 511 Bytes Octets Counter */
#define SWITCH_PAGE_20_TXPKTS512TO1023OCTETS 0x0b0020e0 /* Tx 512 to 1023 Bytes Octets Counter */
#define SWITCH_PAGE_20_TXPKTS1024TOMAXPKTOCTETS 0x0b0020e4 /* Tx 1024 to MaxPkt Bytes Octets Counter */
#define SWITCH_PAGE_21_TXOCTETS        0x0b002100 /* Tx Octets */
#define SWITCH_PAGE_21_TXDROPPKTS      0x0b002108 /* Tx Drop Packet Counter */
#define SWITCH_PAGE_21_TXQPKTQ0        0x0b00210c /* Tx Q0 Packet Counter */
#define SWITCH_PAGE_21_TXBROADCASTPKTS 0x0b002110 /* Tx Broadcast Packet Counter */
#define SWITCH_PAGE_21_TXMULTICASTPKTS 0x0b002114 /* Tx Multicast Packet Counter */
#define SWITCH_PAGE_21_TXUNICASTPKTS   0x0b002118 /* Tx Unicast Packet Counter */
#define SWITCH_PAGE_21_TXCOLLISIONS    0x0b00211c /* Tx Collision Counter */
#define SWITCH_PAGE_21_TXSINGLECOLLISION 0x0b002120 /* Tx Single Collision Counter */
#define SWITCH_PAGE_21_TXMULTIPLECOLLISION 0x0b002124 /* Tx Multiple collsion Counter */
#define SWITCH_PAGE_21_TXDEFERREDTRANSMIT 0x0b002128 /* Tx Deferred Transmit Counter */
#define SWITCH_PAGE_21_TXLATECOLLISION 0x0b00212c /* Tx Late Collision Counter */
#define SWITCH_PAGE_21_TXEXCESSIVECOLLISION 0x0b002130 /* Tx Excessive Collision Counter */
#define SWITCH_PAGE_21_TXFRAMEINDISC   0x0b002134 /* Tx Fram IN Disc Counter */
#define SWITCH_PAGE_21_TXPAUSEPKTS     0x0b002138 /* Tx Pause Packet Counter */
#define SWITCH_PAGE_21_TXQPKTQ1        0x0b00213c /* Tx Q1 Packet Counter */
#define SWITCH_PAGE_21_TXQPKTQ2        0x0b002140 /* Tx Q2 Packet Counter */
#define SWITCH_PAGE_21_TXQPKTQ3        0x0b002144 /* Tx Q3 Packet Counter */
#define SWITCH_PAGE_21_TXQPKTQ4        0x0b002148 /* Tx Q4 Packet Counter */
#define SWITCH_PAGE_21_TXQPKTQ5        0x0b00214c /* Tx Q5 Packet Counter */
#define SWITCH_PAGE_21_RXOCTETS        0x0b002150 /* Rx Packet Octets Counter */
#define SWITCH_PAGE_21_RXUNDERSIZEPKTS 0x0b002158 /* Rx Under Size Packet Octets Counter */
#define SWITCH_PAGE_21_RXPAUSEPKTS     0x0b00215c /* Rx Pause Packet Counter */
#define SWITCH_PAGE_21_RXPKTS64OCTETS  0x0b002160 /* Rx 64 Bytes Octets Counter */
#define SWITCH_PAGE_21_RXPKTS65TO127OCTETS 0x0b002164 /* Rx 65 to 127 Bytes Octets Counter */
#define SWITCH_PAGE_21_RXPKTS128TO255OCTETS 0x0b002168 /* Rx 128 to 255 Bytes Octets Counter */
#define SWITCH_PAGE_21_RXPKTS256TO511OCTETS 0x0b00216c /* Rx 256 to 511 Bytes Octets Counter */
#define SWITCH_PAGE_21_RXPKTS512TO1023OCTETS 0x0b002170 /* Rx 512 to 1023 Bytes Octets Counter */
#define SWITCH_PAGE_21_RXPKTS1024TOMAXPKTOCTETS 0x0b002174 /* Rx 1024 to MaxPkt Bytes Octets Counter */
#define SWITCH_PAGE_21_RXOVERSIZEPKTS  0x0b002178 /* Rx Over Size Packet Counter */
#define SWITCH_PAGE_21_RXJABBERS       0x0b00217c /* Rx Jabber Packet Counter */
#define SWITCH_PAGE_21_RXALIGNMENTERRORS 0x0b002180 /* Rx Alignment Error Counter */
#define SWITCH_PAGE_21_RXFCSERRORS     0x0b002184 /* Rx FCS Error Counter */
#define SWITCH_PAGE_21_RXGOODOCTETS    0x0b002188 /* Rx Good Packet Octet Counter */
#define SWITCH_PAGE_21_RXDROPPKTS      0x0b002190 /* Rx Drop Packet Counter */
#define SWITCH_PAGE_21_RXUNICASTPKTS   0x0b002194 /* Rx Unicast Packet Counter */
#define SWITCH_PAGE_21_RXMULTICASTPKTS 0x0b002198 /* Rx Multicast Packet Counter */
#define SWITCH_PAGE_21_RXBROADCASTPKTS 0x0b00219c /* Rx Broadcast Packet Counter */
#define SWITCH_PAGE_21_RXSACHANGES     0x0b0021a0 /* Rx SA Change Counter */
#define SWITCH_PAGE_21_RXFRAGMENTS     0x0b0021a4 /* Rx Fragment Counter */
#define SWITCH_PAGE_21_RXJUMBOPKT      0x0b0021a8 /* Jumbo Packet Counter */
#define SWITCH_PAGE_21_RXSYMBLERR      0x0b0021ac /* Rx Symbol Error Counter */
#define SWITCH_PAGE_21_INRANGEERRCOUNT 0x0b0021b0 /* InRangeErrCount Counter */
#define SWITCH_PAGE_21_OUTRANGEERRCOUNT 0x0b0021b4 /* OutRangeErrCount Counter */
#define SWITCH_PAGE_21_EEE_LPI_EVENT   0x0b0021b8 /* EEE Low-Power Idle Event Registers */
#define SWITCH_PAGE_21_EEE_LPI_DURATION 0x0b0021bc /* EEE Low-Power Idle Duration Registers */
#define SWITCH_PAGE_21_RXDISCARD       0x0b0021c0 /* Rx Discard Counter */
#define SWITCH_PAGE_21_TXQPKTQ6        0x0b0021c8 /* Tx Q6 Packet Counter */
#define SWITCH_PAGE_21_TXQPKTQ7        0x0b0021cc /* Tx Q7 Packet Counter */
#define SWITCH_PAGE_21_TXPKTS64OCTETS  0x0b0021d0 /* Tx 64 Bytes Octets Counter */
#define SWITCH_PAGE_21_TXPKTS65TO127OCTETS 0x0b0021d4 /* Tx 65 to 127 Bytes Octets Counter */
#define SWITCH_PAGE_21_TXPKTS128TO255OCTETS 0x0b0021d8 /* Tx 128 to 255 Bytes Octets Counter */
#define SWITCH_PAGE_21_TXPKTS256TO511OCTETS 0x0b0021dc /* Tx 256 to 511 Bytes Octets Counter */
#define SWITCH_PAGE_21_TXPKTS512TO1023OCTETS 0x0b0021e0 /* Tx 512 to 1023 Bytes Octets Counter */
#define SWITCH_PAGE_21_TXPKTS1024TOMAXPKTOCTETS 0x0b0021e4 /* Tx 1024 to MaxPkt Bytes Octets Counter */
#define SWITCH_PAGE_22_TXOCTETS        0x0b002200 /* Tx Octets */
#define SWITCH_PAGE_22_TXDROPPKTS      0x0b002208 /* Tx Drop Packet Counter */
#define SWITCH_PAGE_22_TXQPKTQ0        0x0b00220c /* Tx Q0 Packet Counter */
#define SWITCH_PAGE_22_TXBROADCASTPKTS 0x0b002210 /* Tx Broadcast Packet Counter */
#define SWITCH_PAGE_22_TXMULTICASTPKTS 0x0b002214 /* Tx Multicast Packet Counter */
#define SWITCH_PAGE_22_TXUNICASTPKTS   0x0b002218 /* Tx Unicast Packet Counter */
#define SWITCH_PAGE_22_TXCOLLISIONS    0x0b00221c /* Tx Collision Counter */
#define SWITCH_PAGE_22_TXSINGLECOLLISION 0x0b002220 /* Tx Single Collision Counter */
#define SWITCH_PAGE_22_TXMULTIPLECOLLISION 0x0b002224 /* Tx Multiple collsion Counter */
#define SWITCH_PAGE_22_TXDEFERREDTRANSMIT 0x0b002228 /* Tx Deferred Transmit Counter */
#define SWITCH_PAGE_22_TXLATECOLLISION 0x0b00222c /* Tx Late Collision Counter */
#define SWITCH_PAGE_22_TXEXCESSIVECOLLISION 0x0b002230 /* Tx Excessive Collision Counter */
#define SWITCH_PAGE_22_TXFRAMEINDISC   0x0b002234 /* Tx Fram IN Disc Counter */
#define SWITCH_PAGE_22_TXPAUSEPKTS     0x0b002238 /* Tx Pause Packet Counter */
#define SWITCH_PAGE_22_TXQPKTQ1        0x0b00223c /* Tx Q1 Packet Counter */
#define SWITCH_PAGE_22_TXQPKTQ2        0x0b002240 /* Tx Q2 Packet Counter */
#define SWITCH_PAGE_22_TXQPKTQ3        0x0b002244 /* Tx Q3 Packet Counter */
#define SWITCH_PAGE_22_TXQPKTQ4        0x0b002248 /* Tx Q4 Packet Counter */
#define SWITCH_PAGE_22_TXQPKTQ5        0x0b00224c /* Tx Q5 Packet Counter */
#define SWITCH_PAGE_22_RXOCTETS        0x0b002250 /* Rx Packet Octets Counter */
#define SWITCH_PAGE_22_RXUNDERSIZEPKTS 0x0b002258 /* Rx Under Size Packet Octets Counter */
#define SWITCH_PAGE_22_RXPAUSEPKTS     0x0b00225c /* Rx Pause Packet Counter */
#define SWITCH_PAGE_22_RXPKTS64OCTETS  0x0b002260 /* Rx 64 Bytes Octets Counter */
#define SWITCH_PAGE_22_RXPKTS65TO127OCTETS 0x0b002264 /* Rx 65 to 127 Bytes Octets Counter */
#define SWITCH_PAGE_22_RXPKTS128TO255OCTETS 0x0b002268 /* Rx 128 to 255 Bytes Octets Counter */
#define SWITCH_PAGE_22_RXPKTS256TO511OCTETS 0x0b00226c /* Rx 256 to 511 Bytes Octets Counter */
#define SWITCH_PAGE_22_RXPKTS512TO1023OCTETS 0x0b002270 /* Rx 512 to 1023 Bytes Octets Counter */
#define SWITCH_PAGE_22_RXPKTS1024TOMAXPKTOCTETS 0x0b002274 /* Rx 1024 to MaxPkt Bytes Octets Counter */
#define SWITCH_PAGE_22_RXOVERSIZEPKTS  0x0b002278 /* Rx Over Size Packet Counter */
#define SWITCH_PAGE_22_RXJABBERS       0x0b00227c /* Rx Jabber Packet Counter */
#define SWITCH_PAGE_22_RXALIGNMENTERRORS 0x0b002280 /* Rx Alignment Error Counter */
#define SWITCH_PAGE_22_RXFCSERRORS     0x0b002284 /* Rx FCS Error Counter */
#define SWITCH_PAGE_22_RXGOODOCTETS    0x0b002288 /* Rx Good Packet Octet Counter */
#define SWITCH_PAGE_22_RXDROPPKTS      0x0b002290 /* Rx Drop Packet Counter */
#define SWITCH_PAGE_22_RXUNICASTPKTS   0x0b002294 /* Rx Unicast Packet Counter */
#define SWITCH_PAGE_22_RXMULTICASTPKTS 0x0b002298 /* Rx Multicast Packet Counter */
#define SWITCH_PAGE_22_RXBROADCASTPKTS 0x0b00229c /* Rx Broadcast Packet Counter */
#define SWITCH_PAGE_22_RXSACHANGES     0x0b0022a0 /* Rx SA Change Counter */
#define SWITCH_PAGE_22_RXFRAGMENTS     0x0b0022a4 /* Rx Fragment Counter */
#define SWITCH_PAGE_22_RXJUMBOPKT      0x0b0022a8 /* Jumbo Packet Counter */
#define SWITCH_PAGE_22_RXSYMBLERR      0x0b0022ac /* Rx Symbol Error Counter */
#define SWITCH_PAGE_22_INRANGEERRCOUNT 0x0b0022b0 /* InRangeErrCount Counter */
#define SWITCH_PAGE_22_OUTRANGEERRCOUNT 0x0b0022b4 /* OutRangeErrCount Counter */
#define SWITCH_PAGE_22_EEE_LPI_EVENT   0x0b0022b8 /* EEE Low-Power Idle Event Registers */
#define SWITCH_PAGE_22_EEE_LPI_DURATION 0x0b0022bc /* EEE Low-Power Idle Duration Registers */
#define SWITCH_PAGE_22_RXDISCARD       0x0b0022c0 /* Rx Discard Counter */
#define SWITCH_PAGE_22_TXQPKTQ6        0x0b0022c8 /* Tx Q6 Packet Counter */
#define SWITCH_PAGE_22_TXQPKTQ7        0x0b0022cc /* Tx Q7 Packet Counter */
#define SWITCH_PAGE_22_TXPKTS64OCTETS  0x0b0022d0 /* Tx 64 Bytes Octets Counter */
#define SWITCH_PAGE_22_TXPKTS65TO127OCTETS 0x0b0022d4 /* Tx 65 to 127 Bytes Octets Counter */
#define SWITCH_PAGE_22_TXPKTS128TO255OCTETS 0x0b0022d8 /* Tx 128 to 255 Bytes Octets Counter */
#define SWITCH_PAGE_22_TXPKTS256TO511OCTETS 0x0b0022dc /* Tx 256 to 511 Bytes Octets Counter */
#define SWITCH_PAGE_22_TXPKTS512TO1023OCTETS 0x0b0022e0 /* Tx 512 to 1023 Bytes Octets Counter */
#define SWITCH_PAGE_22_TXPKTS1024TOMAXPKTOCTETS 0x0b0022e4 /* Tx 1024 to MaxPkt Bytes Octets Counter */
#define SWITCH_PAGE_23_TXOCTETS        0x0b002300 /* Tx Octets */
#define SWITCH_PAGE_23_TXDROPPKTS      0x0b002308 /* Tx Drop Packet Counter */
#define SWITCH_PAGE_23_TXQPKTQ0        0x0b00230c /* Tx Q0 Packet Counter */
#define SWITCH_PAGE_23_TXBROADCASTPKTS 0x0b002310 /* Tx Broadcast Packet Counter */
#define SWITCH_PAGE_23_TXMULTICASTPKTS 0x0b002314 /* Tx Multicast Packet Counter */
#define SWITCH_PAGE_23_TXUNICASTPKTS   0x0b002318 /* Tx Unicast Packet Counter */
#define SWITCH_PAGE_23_TXCOLLISIONS    0x0b00231c /* Tx Collision Counter */
#define SWITCH_PAGE_23_TXSINGLECOLLISION 0x0b002320 /* Tx Single Collision Counter */
#define SWITCH_PAGE_23_TXMULTIPLECOLLISION 0x0b002324 /* Tx Multiple collsion Counter */
#define SWITCH_PAGE_23_TXDEFERREDTRANSMIT 0x0b002328 /* Tx Deferred Transmit Counter */
#define SWITCH_PAGE_23_TXLATECOLLISION 0x0b00232c /* Tx Late Collision Counter */
#define SWITCH_PAGE_23_TXEXCESSIVECOLLISION 0x0b002330 /* Tx Excessive Collision Counter */
#define SWITCH_PAGE_23_TXFRAMEINDISC   0x0b002334 /* Tx Fram IN Disc Counter */
#define SWITCH_PAGE_23_TXPAUSEPKTS     0x0b002338 /* Tx Pause Packet Counter */
#define SWITCH_PAGE_23_TXQPKTQ1        0x0b00233c /* Tx Q1 Packet Counter */
#define SWITCH_PAGE_23_TXQPKTQ2        0x0b002340 /* Tx Q2 Packet Counter */
#define SWITCH_PAGE_23_TXQPKTQ3        0x0b002344 /* Tx Q3 Packet Counter */
#define SWITCH_PAGE_23_TXQPKTQ4        0x0b002348 /* Tx Q4 Packet Counter */
#define SWITCH_PAGE_23_TXQPKTQ5        0x0b00234c /* Tx Q5 Packet Counter */
#define SWITCH_PAGE_23_RXOCTETS        0x0b002350 /* Rx Packet Octets Counter */
#define SWITCH_PAGE_23_RXUNDERSIZEPKTS 0x0b002358 /* Rx Under Size Packet Octets Counter */
#define SWITCH_PAGE_23_RXPAUSEPKTS     0x0b00235c /* Rx Pause Packet Counter */
#define SWITCH_PAGE_23_RXPKTS64OCTETS  0x0b002360 /* Rx 64 Bytes Octets Counter */
#define SWITCH_PAGE_23_RXPKTS65TO127OCTETS 0x0b002364 /* Rx 65 to 127 Bytes Octets Counter */
#define SWITCH_PAGE_23_RXPKTS128TO255OCTETS 0x0b002368 /* Rx 128 to 255 Bytes Octets Counter */
#define SWITCH_PAGE_23_RXPKTS256TO511OCTETS 0x0b00236c /* Rx 256 to 511 Bytes Octets Counter */
#define SWITCH_PAGE_23_RXPKTS512TO1023OCTETS 0x0b002370 /* Rx 512 to 1023 Bytes Octets Counter */
#define SWITCH_PAGE_23_RXPKTS1024TOMAXPKTOCTETS 0x0b002374 /* Rx 1024 to MaxPkt Bytes Octets Counter */
#define SWITCH_PAGE_23_RXOVERSIZEPKTS  0x0b002378 /* Rx Over Size Packet Counter */
#define SWITCH_PAGE_23_RXJABBERS       0x0b00237c /* Rx Jabber Packet Counter */
#define SWITCH_PAGE_23_RXALIGNMENTERRORS 0x0b002380 /* Rx Alignment Error Counter */
#define SWITCH_PAGE_23_RXFCSERRORS     0x0b002384 /* Rx FCS Error Counter */
#define SWITCH_PAGE_23_RXGOODOCTETS    0x0b002388 /* Rx Good Packet Octet Counter */
#define SWITCH_PAGE_23_RXDROPPKTS      0x0b002390 /* Rx Drop Packet Counter */
#define SWITCH_PAGE_23_RXUNICASTPKTS   0x0b002394 /* Rx Unicast Packet Counter */
#define SWITCH_PAGE_23_RXMULTICASTPKTS 0x0b002398 /* Rx Multicast Packet Counter */
#define SWITCH_PAGE_23_RXBROADCASTPKTS 0x0b00239c /* Rx Broadcast Packet Counter */
#define SWITCH_PAGE_23_RXSACHANGES     0x0b0023a0 /* Rx SA Change Counter */
#define SWITCH_PAGE_23_RXFRAGMENTS     0x0b0023a4 /* Rx Fragment Counter */
#define SWITCH_PAGE_23_RXJUMBOPKT      0x0b0023a8 /* Jumbo Packet Counter */
#define SWITCH_PAGE_23_RXSYMBLERR      0x0b0023ac /* Rx Symbol Error Counter */
#define SWITCH_PAGE_23_INRANGEERRCOUNT 0x0b0023b0 /* InRangeErrCount Counter */
#define SWITCH_PAGE_23_OUTRANGEERRCOUNT 0x0b0023b4 /* OutRangeErrCount Counter */
#define SWITCH_PAGE_23_EEE_LPI_EVENT   0x0b0023b8 /* EEE Low-Power Idle Event Registers */
#define SWITCH_PAGE_23_EEE_LPI_DURATION 0x0b0023bc /* EEE Low-Power Idle Duration Registers */
#define SWITCH_PAGE_23_RXDISCARD       0x0b0023c0 /* Rx Discard Counter */
#define SWITCH_PAGE_23_TXQPKTQ6        0x0b0023c8 /* Tx Q6 Packet Counter */
#define SWITCH_PAGE_23_TXQPKTQ7        0x0b0023cc /* Tx Q7 Packet Counter */
#define SWITCH_PAGE_23_TXPKTS64OCTETS  0x0b0023d0 /* Tx 64 Bytes Octets Counter */
#define SWITCH_PAGE_23_TXPKTS65TO127OCTETS 0x0b0023d4 /* Tx 65 to 127 Bytes Octets Counter */
#define SWITCH_PAGE_23_TXPKTS128TO255OCTETS 0x0b0023d8 /* Tx 128 to 255 Bytes Octets Counter */
#define SWITCH_PAGE_23_TXPKTS256TO511OCTETS 0x0b0023dc /* Tx 256 to 511 Bytes Octets Counter */
#define SWITCH_PAGE_23_TXPKTS512TO1023OCTETS 0x0b0023e0 /* Tx 512 to 1023 Bytes Octets Counter */
#define SWITCH_PAGE_23_TXPKTS1024TOMAXPKTOCTETS 0x0b0023e4 /* Tx 1024 to MaxPkt Bytes Octets Counter */
#define SWITCH_PAGE_24_TXOCTETS        0x0b002400 /* Tx Octets */
#define SWITCH_PAGE_24_TXDROPPKTS      0x0b002408 /* Tx Drop Packet Counter */
#define SWITCH_PAGE_24_TXQPKTQ0        0x0b00240c /* Tx Q0 Packet Counter */
#define SWITCH_PAGE_24_TXBROADCASTPKTS 0x0b002410 /* Tx Broadcast Packet Counter */
#define SWITCH_PAGE_24_TXMULTICASTPKTS 0x0b002414 /* Tx Multicast Packet Counter */
#define SWITCH_PAGE_24_TXUNICASTPKTS   0x0b002418 /* Tx Unicast Packet Counter */
#define SWITCH_PAGE_24_TXCOLLISIONS    0x0b00241c /* Tx Collision Counter */
#define SWITCH_PAGE_24_TXSINGLECOLLISION 0x0b002420 /* Tx Single Collision Counter */
#define SWITCH_PAGE_24_TXMULTIPLECOLLISION 0x0b002424 /* Tx Multiple collsion Counter */
#define SWITCH_PAGE_24_TXDEFERREDTRANSMIT 0x0b002428 /* Tx Deferred Transmit Counter */
#define SWITCH_PAGE_24_TXLATECOLLISION 0x0b00242c /* Tx Late Collision Counter */
#define SWITCH_PAGE_24_TXEXCESSIVECOLLISION 0x0b002430 /* Tx Excessive Collision Counter */
#define SWITCH_PAGE_24_TXFRAMEINDISC   0x0b002434 /* Tx Fram IN Disc Counter */
#define SWITCH_PAGE_24_TXPAUSEPKTS     0x0b002438 /* Tx Pause Packet Counter */
#define SWITCH_PAGE_24_TXQPKTQ1        0x0b00243c /* Tx Q1 Packet Counter */
#define SWITCH_PAGE_24_TXQPKTQ2        0x0b002440 /* Tx Q2 Packet Counter */
#define SWITCH_PAGE_24_TXQPKTQ3        0x0b002444 /* Tx Q3 Packet Counter */
#define SWITCH_PAGE_24_TXQPKTQ4        0x0b002448 /* Tx Q4 Packet Counter */
#define SWITCH_PAGE_24_TXQPKTQ5        0x0b00244c /* Tx Q5 Packet Counter */
#define SWITCH_PAGE_24_RXOCTETS        0x0b002450 /* Rx Packet Octets Counter */
#define SWITCH_PAGE_24_RXUNDERSIZEPKTS 0x0b002458 /* Rx Under Size Packet Octets Counter */
#define SWITCH_PAGE_24_RXPAUSEPKTS     0x0b00245c /* Rx Pause Packet Counter */
#define SWITCH_PAGE_24_RXPKTS64OCTETS  0x0b002460 /* Rx 64 Bytes Octets Counter */
#define SWITCH_PAGE_24_RXPKTS65TO127OCTETS 0x0b002464 /* Rx 65 to 127 Bytes Octets Counter */
#define SWITCH_PAGE_24_RXPKTS128TO255OCTETS 0x0b002468 /* Rx 128 to 255 Bytes Octets Counter */
#define SWITCH_PAGE_24_RXPKTS256TO511OCTETS 0x0b00246c /* Rx 256 to 511 Bytes Octets Counter */
#define SWITCH_PAGE_24_RXPKTS512TO1023OCTETS 0x0b002470 /* Rx 512 to 1023 Bytes Octets Counter */
#define SWITCH_PAGE_24_RXPKTS1024TOMAXPKTOCTETS 0x0b002474 /* Rx 1024 to MaxPkt Bytes Octets Counter */
#define SWITCH_PAGE_24_RXOVERSIZEPKTS  0x0b002478 /* Rx Over Size Packet Counter */
#define SWITCH_PAGE_24_RXJABBERS       0x0b00247c /* Rx Jabber Packet Counter */
#define SWITCH_PAGE_24_RXALIGNMENTERRORS 0x0b002480 /* Rx Alignment Error Counter */
#define SWITCH_PAGE_24_RXFCSERRORS     0x0b002484 /* Rx FCS Error Counter */
#define SWITCH_PAGE_24_RXGOODOCTETS    0x0b002488 /* Rx Good Packet Octet Counter */
#define SWITCH_PAGE_24_RXDROPPKTS      0x0b002490 /* Rx Drop Packet Counter */
#define SWITCH_PAGE_24_RXUNICASTPKTS   0x0b002494 /* Rx Unicast Packet Counter */
#define SWITCH_PAGE_24_RXMULTICASTPKTS 0x0b002498 /* Rx Multicast Packet Counter */
#define SWITCH_PAGE_24_RXBROADCASTPKTS 0x0b00249c /* Rx Broadcast Packet Counter */
#define SWITCH_PAGE_24_RXSACHANGES     0x0b0024a0 /* Rx SA Change Counter */
#define SWITCH_PAGE_24_RXFRAGMENTS     0x0b0024a4 /* Rx Fragment Counter */
#define SWITCH_PAGE_24_RXJUMBOPKT      0x0b0024a8 /* Jumbo Packet Counter */
#define SWITCH_PAGE_24_RXSYMBLERR      0x0b0024ac /* Rx Symbol Error Counter */
#define SWITCH_PAGE_24_INRANGEERRCOUNT 0x0b0024b0 /* InRangeErrCount Counter */
#define SWITCH_PAGE_24_OUTRANGEERRCOUNT 0x0b0024b4 /* OutRangeErrCount Counter */
#define SWITCH_PAGE_24_EEE_LPI_EVENT   0x0b0024b8 /* EEE Low-Power Idle Event Registers */
#define SWITCH_PAGE_24_EEE_LPI_DURATION 0x0b0024bc /* EEE Low-Power Idle Duration Registers */
#define SWITCH_PAGE_24_RXDISCARD       0x0b0024c0 /* Rx Discard Counter */
#define SWITCH_PAGE_24_TXQPKTQ6        0x0b0024c8 /* Tx Q6 Packet Counter */
#define SWITCH_PAGE_24_TXQPKTQ7        0x0b0024cc /* Tx Q7 Packet Counter */
#define SWITCH_PAGE_24_TXPKTS64OCTETS  0x0b0024d0 /* Tx 64 Bytes Octets Counter */
#define SWITCH_PAGE_24_TXPKTS65TO127OCTETS 0x0b0024d4 /* Tx 65 to 127 Bytes Octets Counter */
#define SWITCH_PAGE_24_TXPKTS128TO255OCTETS 0x0b0024d8 /* Tx 128 to 255 Bytes Octets Counter */
#define SWITCH_PAGE_24_TXPKTS256TO511OCTETS 0x0b0024dc /* Tx 256 to 511 Bytes Octets Counter */
#define SWITCH_PAGE_24_TXPKTS512TO1023OCTETS 0x0b0024e0 /* Tx 512 to 1023 Bytes Octets Counter */
#define SWITCH_PAGE_24_TXPKTS1024TOMAXPKTOCTETS 0x0b0024e4 /* Tx 1024 to MaxPkt Bytes Octets Counter */
#define SWITCH_PAGE_25_TXOCTETS        0x0b002500 /* Tx Octets */
#define SWITCH_PAGE_25_TXDROPPKTS      0x0b002508 /* Tx Drop Packet Counter */
#define SWITCH_PAGE_25_TXQPKTQ0        0x0b00250c /* Tx Q0 Packet Counter */
#define SWITCH_PAGE_25_TXBROADCASTPKTS 0x0b002510 /* Tx Broadcast Packet Counter */
#define SWITCH_PAGE_25_TXMULTICASTPKTS 0x0b002514 /* Tx Multicast Packet Counter */
#define SWITCH_PAGE_25_TXUNICASTPKTS   0x0b002518 /* Tx Unicast Packet Counter */
#define SWITCH_PAGE_25_TXCOLLISIONS    0x0b00251c /* Tx Collision Counter */
#define SWITCH_PAGE_25_TXSINGLECOLLISION 0x0b002520 /* Tx Single Collision Counter */
#define SWITCH_PAGE_25_TXMULTIPLECOLLISION 0x0b002524 /* Tx Multiple collsion Counter */
#define SWITCH_PAGE_25_TXDEFERREDTRANSMIT 0x0b002528 /* Tx Deferred Transmit Counter */
#define SWITCH_PAGE_25_TXLATECOLLISION 0x0b00252c /* Tx Late Collision Counter */
#define SWITCH_PAGE_25_TXEXCESSIVECOLLISION 0x0b002530 /* Tx Excessive Collision Counter */
#define SWITCH_PAGE_25_TXFRAMEINDISC   0x0b002534 /* Tx Fram IN Disc Counter */
#define SWITCH_PAGE_25_TXPAUSEPKTS     0x0b002538 /* Tx Pause Packet Counter */
#define SWITCH_PAGE_25_TXQPKTQ1        0x0b00253c /* Tx Q1 Packet Counter */
#define SWITCH_PAGE_25_TXQPKTQ2        0x0b002540 /* Tx Q2 Packet Counter */
#define SWITCH_PAGE_25_TXQPKTQ3        0x0b002544 /* Tx Q3 Packet Counter */
#define SWITCH_PAGE_25_TXQPKTQ4        0x0b002548 /* Tx Q4 Packet Counter */
#define SWITCH_PAGE_25_TXQPKTQ5        0x0b00254c /* Tx Q5 Packet Counter */
#define SWITCH_PAGE_25_RXOCTETS        0x0b002550 /* Rx Packet Octets Counter */
#define SWITCH_PAGE_25_RXUNDERSIZEPKTS 0x0b002558 /* Rx Under Size Packet Octets Counter */
#define SWITCH_PAGE_25_RXPAUSEPKTS     0x0b00255c /* Rx Pause Packet Counter */
#define SWITCH_PAGE_25_RXPKTS64OCTETS  0x0b002560 /* Rx 64 Bytes Octets Counter */
#define SWITCH_PAGE_25_RXPKTS65TO127OCTETS 0x0b002564 /* Rx 65 to 127 Bytes Octets Counter */
#define SWITCH_PAGE_25_RXPKTS128TO255OCTETS 0x0b002568 /* Rx 128 to 255 Bytes Octets Counter */
#define SWITCH_PAGE_25_RXPKTS256TO511OCTETS 0x0b00256c /* Rx 256 to 511 Bytes Octets Counter */
#define SWITCH_PAGE_25_RXPKTS512TO1023OCTETS 0x0b002570 /* Rx 512 to 1023 Bytes Octets Counter */
#define SWITCH_PAGE_25_RXPKTS1024TOMAXPKTOCTETS 0x0b002574 /* Rx 1024 to MaxPkt Bytes Octets Counter */
#define SWITCH_PAGE_25_RXOVERSIZEPKTS  0x0b002578 /* Rx Over Size Packet Counter */
#define SWITCH_PAGE_25_RXJABBERS       0x0b00257c /* Rx Jabber Packet Counter */
#define SWITCH_PAGE_25_RXALIGNMENTERRORS 0x0b002580 /* Rx Alignment Error Counter */
#define SWITCH_PAGE_25_RXFCSERRORS     0x0b002584 /* Rx FCS Error Counter */
#define SWITCH_PAGE_25_RXGOODOCTETS    0x0b002588 /* Rx Good Packet Octet Counter */
#define SWITCH_PAGE_25_RXDROPPKTS      0x0b002590 /* Rx Drop Packet Counter */
#define SWITCH_PAGE_25_RXUNICASTPKTS   0x0b002594 /* Rx Unicast Packet Counter */
#define SWITCH_PAGE_25_RXMULTICASTPKTS 0x0b002598 /* Rx Multicast Packet Counter */
#define SWITCH_PAGE_25_RXBROADCASTPKTS 0x0b00259c /* Rx Broadcast Packet Counter */
#define SWITCH_PAGE_25_RXSACHANGES     0x0b0025a0 /* Rx SA Change Counter */
#define SWITCH_PAGE_25_RXFRAGMENTS     0x0b0025a4 /* Rx Fragment Counter */
#define SWITCH_PAGE_25_RXJUMBOPKT      0x0b0025a8 /* Jumbo Packet Counter */
#define SWITCH_PAGE_25_RXSYMBLERR      0x0b0025ac /* Rx Symbol Error Counter */
#define SWITCH_PAGE_25_INRANGEERRCOUNT 0x0b0025b0 /* InRangeErrCount Counter */
#define SWITCH_PAGE_25_OUTRANGEERRCOUNT 0x0b0025b4 /* OutRangeErrCount Counter */
#define SWITCH_PAGE_25_EEE_LPI_EVENT   0x0b0025b8 /* EEE Low-Power Idle Event Registers */
#define SWITCH_PAGE_25_EEE_LPI_DURATION 0x0b0025bc /* EEE Low-Power Idle Duration Registers */
#define SWITCH_PAGE_25_RXDISCARD       0x0b0025c0 /* Rx Discard Counter */
#define SWITCH_PAGE_25_TXQPKTQ6        0x0b0025c8 /* Tx Q6 Packet Counter */
#define SWITCH_PAGE_25_TXQPKTQ7        0x0b0025cc /* Tx Q7 Packet Counter */
#define SWITCH_PAGE_25_TXPKTS64OCTETS  0x0b0025d0 /* Tx 64 Bytes Octets Counter */
#define SWITCH_PAGE_25_TXPKTS65TO127OCTETS 0x0b0025d4 /* Tx 65 to 127 Bytes Octets Counter */
#define SWITCH_PAGE_25_TXPKTS128TO255OCTETS 0x0b0025d8 /* Tx 128 to 255 Bytes Octets Counter */
#define SWITCH_PAGE_25_TXPKTS256TO511OCTETS 0x0b0025dc /* Tx 256 to 511 Bytes Octets Counter */
#define SWITCH_PAGE_25_TXPKTS512TO1023OCTETS 0x0b0025e0 /* Tx 512 to 1023 Bytes Octets Counter */
#define SWITCH_PAGE_25_TXPKTS1024TOMAXPKTOCTETS 0x0b0025e4 /* Tx 1024 to MaxPkt Bytes Octets Counter */
#define SWITCH_PAGE_26_TXOCTETS        0x0b002600 /* Tx Octets */
#define SWITCH_PAGE_26_TXDROPPKTS      0x0b002608 /* Tx Drop Packet Counter */
#define SWITCH_PAGE_26_TXQPKTQ0        0x0b00260c /* Tx Q0 Packet Counter */
#define SWITCH_PAGE_26_TXBROADCASTPKTS 0x0b002610 /* Tx Broadcast Packet Counter */
#define SWITCH_PAGE_26_TXMULTICASTPKTS 0x0b002614 /* Tx Multicast Packet Counter */
#define SWITCH_PAGE_26_TXUNICASTPKTS   0x0b002618 /* Tx Unicast Packet Counter */
#define SWITCH_PAGE_26_TXCOLLISIONS    0x0b00261c /* Tx Collision Counter */
#define SWITCH_PAGE_26_TXSINGLECOLLISION 0x0b002620 /* Tx Single Collision Counter */
#define SWITCH_PAGE_26_TXMULTIPLECOLLISION 0x0b002624 /* Tx Multiple collsion Counter */
#define SWITCH_PAGE_26_TXDEFERREDTRANSMIT 0x0b002628 /* Tx Deferred Transmit Counter */
#define SWITCH_PAGE_26_TXLATECOLLISION 0x0b00262c /* Tx Late Collision Counter */
#define SWITCH_PAGE_26_TXEXCESSIVECOLLISION 0x0b002630 /* Tx Excessive Collision Counter */
#define SWITCH_PAGE_26_TXFRAMEINDISC   0x0b002634 /* Tx Fram IN Disc Counter */
#define SWITCH_PAGE_26_TXPAUSEPKTS     0x0b002638 /* Tx Pause Packet Counter */
#define SWITCH_PAGE_26_TXQPKTQ1        0x0b00263c /* Tx Q1 Packet Counter */
#define SWITCH_PAGE_26_TXQPKTQ2        0x0b002640 /* Tx Q2 Packet Counter */
#define SWITCH_PAGE_26_TXQPKTQ3        0x0b002644 /* Tx Q3 Packet Counter */
#define SWITCH_PAGE_26_TXQPKTQ4        0x0b002648 /* Tx Q4 Packet Counter */
#define SWITCH_PAGE_26_TXQPKTQ5        0x0b00264c /* Tx Q5 Packet Counter */
#define SWITCH_PAGE_26_RXOCTETS        0x0b002650 /* Rx Packet Octets Counter */
#define SWITCH_PAGE_26_RXUNDERSIZEPKTS 0x0b002658 /* Rx Under Size Packet Octets Counter */
#define SWITCH_PAGE_26_RXPAUSEPKTS     0x0b00265c /* Rx Pause Packet Counter */
#define SWITCH_PAGE_26_RXPKTS64OCTETS  0x0b002660 /* Rx 64 Bytes Octets Counter */
#define SWITCH_PAGE_26_RXPKTS65TO127OCTETS 0x0b002664 /* Rx 65 to 127 Bytes Octets Counter */
#define SWITCH_PAGE_26_RXPKTS128TO255OCTETS 0x0b002668 /* Rx 128 to 255 Bytes Octets Counter */
#define SWITCH_PAGE_26_RXPKTS256TO511OCTETS 0x0b00266c /* Rx 256 to 511 Bytes Octets Counter */
#define SWITCH_PAGE_26_RXPKTS512TO1023OCTETS 0x0b002670 /* Rx 512 to 1023 Bytes Octets Counter */
#define SWITCH_PAGE_26_RXPKTS1024TOMAXPKTOCTETS 0x0b002674 /* Rx 1024 to MaxPkt Bytes Octets Counter */
#define SWITCH_PAGE_26_RXOVERSIZEPKTS  0x0b002678 /* Rx Over Size Packet Counter */
#define SWITCH_PAGE_26_RXJABBERS       0x0b00267c /* Rx Jabber Packet Counter */
#define SWITCH_PAGE_26_RXALIGNMENTERRORS 0x0b002680 /* Rx Alignment Error Counter */
#define SWITCH_PAGE_26_RXFCSERRORS     0x0b002684 /* Rx FCS Error Counter */
#define SWITCH_PAGE_26_RXGOODOCTETS    0x0b002688 /* Rx Good Packet Octet Counter */
#define SWITCH_PAGE_26_RXDROPPKTS      0x0b002690 /* Rx Drop Packet Counter */
#define SWITCH_PAGE_26_RXUNICASTPKTS   0x0b002694 /* Rx Unicast Packet Counter */
#define SWITCH_PAGE_26_RXMULTICASTPKTS 0x0b002698 /* Rx Multicast Packet Counter */
#define SWITCH_PAGE_26_RXBROADCASTPKTS 0x0b00269c /* Rx Broadcast Packet Counter */
#define SWITCH_PAGE_26_RXSACHANGES     0x0b0026a0 /* Rx SA Change Counter */
#define SWITCH_PAGE_26_RXFRAGMENTS     0x0b0026a4 /* Rx Fragment Counter */
#define SWITCH_PAGE_26_RXJUMBOPKT      0x0b0026a8 /* Jumbo Packet Counter */
#define SWITCH_PAGE_26_RXSYMBLERR      0x0b0026ac /* Rx Symbol Error Counter */
#define SWITCH_PAGE_26_INRANGEERRCOUNT 0x0b0026b0 /* InRangeErrCount Counter */
#define SWITCH_PAGE_26_OUTRANGEERRCOUNT 0x0b0026b4 /* OutRangeErrCount Counter */
#define SWITCH_PAGE_26_EEE_LPI_EVENT   0x0b0026b8 /* EEE Low-Power Idle Event Registers */
#define SWITCH_PAGE_26_EEE_LPI_DURATION 0x0b0026bc /* EEE Low-Power Idle Duration Registers */
#define SWITCH_PAGE_26_RXDISCARD       0x0b0026c0 /* Rx Discard Counter */
#define SWITCH_PAGE_26_TXQPKTQ6        0x0b0026c8 /* Tx Q6 Packet Counter */
#define SWITCH_PAGE_26_TXQPKTQ7        0x0b0026cc /* Tx Q7 Packet Counter */
#define SWITCH_PAGE_26_TXPKTS64OCTETS  0x0b0026d0 /* Tx 64 Bytes Octets Counter */
#define SWITCH_PAGE_26_TXPKTS65TO127OCTETS 0x0b0026d4 /* Tx 65 to 127 Bytes Octets Counter */
#define SWITCH_PAGE_26_TXPKTS128TO255OCTETS 0x0b0026d8 /* Tx 128 to 255 Bytes Octets Counter */
#define SWITCH_PAGE_26_TXPKTS256TO511OCTETS 0x0b0026dc /* Tx 256 to 511 Bytes Octets Counter */
#define SWITCH_PAGE_26_TXPKTS512TO1023OCTETS 0x0b0026e0 /* Tx 512 to 1023 Bytes Octets Counter */
#define SWITCH_PAGE_26_TXPKTS1024TOMAXPKTOCTETS 0x0b0026e4 /* Tx 1024 to MaxPkt Bytes Octets Counter */
#define SWITCH_PAGE_27_TXOCTETS_P7     0x0b002700 /* Tx Octets */
#define SWITCH_PAGE_27_TXDROPPKTS_P7   0x0b002708 /* Tx Drop Packet Counter */
#define SWITCH_PAGE_27_TXQPKTQ0_P7     0x0b00270c /* Tx Q0 Packet Counter */
#define SWITCH_PAGE_27_TXBROADCASTPKTS_P7 0x0b002710 /* Tx Broadcast Packet Counter */
#define SWITCH_PAGE_27_TXMULTICASTPKTS_P7 0x0b002714 /* Tx Multicast Packet Counter */
#define SWITCH_PAGE_27_TXUNICASTPKTS_P7 0x0b002718 /* Tx Unicast Packet Counter */
#define SWITCH_PAGE_27_TXCOLLISIONS_P7 0x0b00271c /* Tx Collision Counter */
#define SWITCH_PAGE_27_TXSINGLECOLLISION_P7 0x0b002720 /* Tx Single Collision Counter */
#define SWITCH_PAGE_27_TXMULTIPLECOLLISION_P7 0x0b002724 /* Tx Multiple collsion Counter */
#define SWITCH_PAGE_27_TXDEFERREDTRANSMIT_P7 0x0b002728 /* Tx Deferred Transmit Counter */
#define SWITCH_PAGE_27_TXLATECOLLISION_P7 0x0b00272c /* Tx Late Collision Counter */
#define SWITCH_PAGE_27_TXEXCESSIVECOLLISION_P7 0x0b002730 /* Tx Excessive Collision Counter */
#define SWITCH_PAGE_27_TXFRAMEINDISC_P7 0x0b002734 /* Tx Fram IN Disc Counter */
#define SWITCH_PAGE_27_TXPAUSEPKTS_P7  0x0b002738 /* Tx Pause Packet Counter */
#define SWITCH_PAGE_27_TXQPKTQ1_P7     0x0b00273c /* Tx Q1 Packet Counter */
#define SWITCH_PAGE_27_TXQPKTQ2_P7     0x0b002740 /* Tx Q2 Packet Counter */
#define SWITCH_PAGE_27_TXQPKTQ3_P7     0x0b002744 /* Tx Q3 Packet Counter */
#define SWITCH_PAGE_27_TXQPKTQ4_P7     0x0b002748 /* Tx Q4 Packet Counter */
#define SWITCH_PAGE_27_TXQPKTQ5_P7     0x0b00274c /* Tx Q5 Packet Counter */
#define SWITCH_PAGE_27_RXOCTETS_P7     0x0b002750 /* Rx Packet Octets Counter */
#define SWITCH_PAGE_27_RXUNDERSIZEPKTS_P7 0x0b002758 /* Rx Under Size Packet Octets Counter */
#define SWITCH_PAGE_27_RXPAUSEPKTS_P7  0x0b00275c /* Rx Pause Packet Counter */
#define SWITCH_PAGE_27_RXPKTS64OCTETS_P7 0x0b002760 /* Rx 64 Bytes Octets Counter */
#define SWITCH_PAGE_27_RXPKTS65TO127OCTETS_P7 0x0b002764 /* Rx 65 to 127 Bytes Octets Counter */
#define SWITCH_PAGE_27_RXPKTS128TO255OCTETS_P7 0x0b002768 /* Rx 128 to 255 Bytes Octets Counter */
#define SWITCH_PAGE_27_RXPKTS256TO511OCTETS_P7 0x0b00276c /* Rx 256 to 511 Bytes Octets Counter */
#define SWITCH_PAGE_27_RXPKTS512TO1023OCTETS_P7 0x0b002770 /* Rx 512 to 1023 Bytes Octets Counter */
#define SWITCH_PAGE_27_RXPKTS1024TOMAXPKTOCTETS_P7 0x0b002774 /* Rx 1024 to MaxPkt Bytes Octets Counter */
#define SWITCH_PAGE_27_RXOVERSIZEPKTS_P7 0x0b002778 /* Rx Over Size Packet Counter */
#define SWITCH_PAGE_27_RXJABBERS_P7    0x0b00277c /* Rx Jabber Packet Counter */
#define SWITCH_PAGE_27_RXALIGNMENTERRORS_P7 0x0b002780 /* Rx Alignment Error Counter */
#define SWITCH_PAGE_27_RXFCSERRORS_P7  0x0b002784 /* Rx FCS Error Counter */
#define SWITCH_PAGE_27_RXGOODOCTETS_P7 0x0b002788 /* Rx Good Packet Octet Counter */
#define SWITCH_PAGE_27_RXDROPPKTS_P7   0x0b002790 /* Rx Drop Packet Counter */
#define SWITCH_PAGE_27_RXUNICASTPKTS_P7 0x0b002794 /* Rx Unicast Packet Counter */
#define SWITCH_PAGE_27_RXMULTICASTPKTS_P7 0x0b002798 /* Rx Multicast Packet Counter */
#define SWITCH_PAGE_27_RXBROADCASTPKTS_P7 0x0b00279c /* Rx Broadcast Packet Counter */
#define SWITCH_PAGE_27_RXSACHANGES_P7  0x0b0027a0 /* Rx SA Change Counter */
#define SWITCH_PAGE_27_RXFRAGMENTS_P7  0x0b0027a4 /* Rx Fragment Counter */
#define SWITCH_PAGE_27_RXJUMBOPKT_P7   0x0b0027a8 /* Jumbo Packet Counter */
#define SWITCH_PAGE_27_RXSYMBLERR_P7   0x0b0027ac /* Rx Symbol Error Counter */
#define SWITCH_PAGE_27_INRANGEERRCOUNT_P7 0x0b0027b0 /* InRangeErrCount Counter */
#define SWITCH_PAGE_27_OUTRANGEERRCOUNT_P7 0x0b0027b4 /* OutRangeErrCount Counter */
#define SWITCH_PAGE_27_EEE_LPI_EVENT_P7 0x0b0027b8 /* EEE Low-Power Idle Event Registers */
#define SWITCH_PAGE_27_EEE_LPI_DURATION_P7 0x0b0027bc /* EEE Low-Power Idle Duration Registers */
#define SWITCH_PAGE_27_RXDISCARD_P7    0x0b0027c0 /* Rx Discard Counter */
#define SWITCH_PAGE_27_TXQPKTQ6_P7     0x0b0027c8 /* Tx Q6 Packet Counter */
#define SWITCH_PAGE_27_TXQPKTQ7_P7     0x0b0027cc /* Tx Q7 Packet Counter */
#define SWITCH_PAGE_27_TXPKTS64OCTETS_P7 0x0b0027d0 /* Tx 64 Bytes Octets Counter */
#define SWITCH_PAGE_27_TXPKTS65TO127OCTETS_P7 0x0b0027d4 /* Tx 65 to 127 Bytes Octets Counter */
#define SWITCH_PAGE_27_TXPKTS128TO255OCTETS_P7 0x0b0027d8 /* Tx 128 to 255 Bytes Octets Counter */
#define SWITCH_PAGE_27_TXPKTS256TO511OCTETS_P7 0x0b0027dc /* Tx 256 to 511 Bytes Octets Counter */
#define SWITCH_PAGE_27_TXPKTS512TO1023OCTETS_P7 0x0b0027e0 /* Tx 512 to 1023 Bytes Octets Counter */
#define SWITCH_PAGE_27_TXPKTS1024TOMAXPKTOCTETS_P7 0x0b0027e4 /* Tx 1024 to MaxPkt Bytes Octets Counter */
#define SWITCH_PAGE_28_TXOCTETS_IMP    0x0b002800 /* Tx Octets */
#define SWITCH_PAGE_28_TXDROPPKTS_IMP  0x0b002808 /* Tx Drop Packet Counter */
#define SWITCH_PAGE_28_TXQPKTQ0_IMP    0x0b00280c /* Tx Q0 Packet Counter */
#define SWITCH_PAGE_28_TXBROADCASTPKTS_IMP 0x0b002810 /* Tx Broadcast Packet Counter */
#define SWITCH_PAGE_28_TXMULTICASTPKTS_IMP 0x0b002814 /* Tx Multicast Packet Counter */
#define SWITCH_PAGE_28_TXUNICASTPKTS_IMP 0x0b002818 /* Tx Unicast Packet Counter */
#define SWITCH_PAGE_28_TXCOLLISIONS_IMP 0x0b00281c /* Tx Collision Counter */
#define SWITCH_PAGE_28_TXSINGLECOLLISION_IMP 0x0b002820 /* Tx Single Collision Counter */
#define SWITCH_PAGE_28_TXMULTIPLECOLLISION_IMP 0x0b002824 /* Tx Multiple collsion Counter */
#define SWITCH_PAGE_28_TXDEFERREDTRANSMIT_IMP 0x0b002828 /* Tx Deferred Transmit Counter */
#define SWITCH_PAGE_28_TXLATECOLLISION_IMP 0x0b00282c /* Tx Late Collision Counter */
#define SWITCH_PAGE_28_TXEXCESSIVECOLLISION_IMP 0x0b002830 /* Tx Excessive Collision Counter */
#define SWITCH_PAGE_28_TXFRAMEINDISC_IMP 0x0b002834 /* Tx Fram IN Disc Counter */
#define SWITCH_PAGE_28_TXPAUSEPKTS_IMP 0x0b002838 /* Tx Pause Packet Counter */
#define SWITCH_PAGE_28_TXQPKTQ1_IMP    0x0b00283c /* Tx Q1 Packet Counter */
#define SWITCH_PAGE_28_TXQPKTQ2_IMP    0x0b002840 /* Tx Q2 Packet Counter */
#define SWITCH_PAGE_28_TXQPKTQ3_IMP    0x0b002844 /* Tx Q3 Packet Counter */
#define SWITCH_PAGE_28_TXQPKTQ4_IMP    0x0b002848 /* Tx Q4 Packet Counter */
#define SWITCH_PAGE_28_TXQPKTQ5_IMP    0x0b00284c /* Tx Q5 Packet Counter */
#define SWITCH_PAGE_28_RXOCTETS_IMP    0x0b002850 /* Rx Packet Octets Counter */
#define SWITCH_PAGE_28_RXUNDERSIZEPKTS_IMP 0x0b002858 /* Rx Under Size Packet Octets Counter */
#define SWITCH_PAGE_28_RXPAUSEPKTS_IMP 0x0b00285c /* Rx Pause Packet Counter */
#define SWITCH_PAGE_28_RXPKTS64OCTETS_IMP 0x0b002860 /* Rx 64 Bytes Octets Counter */
#define SWITCH_PAGE_28_RXPKTS65TO127OCTETS_IMP 0x0b002864 /* Rx 65 to 127 Bytes Octets Counter */
#define SWITCH_PAGE_28_RXPKTS128TO255OCTETS_IMP 0x0b002868 /* Rx 128 to 255 Bytes Octets Counter */
#define SWITCH_PAGE_28_RXPKTS256TO511OCTETS_IMP 0x0b00286c /* Rx 256 to 511 Bytes Octets Counter */
#define SWITCH_PAGE_28_RXPKTS512TO1023OCTETS_IMP 0x0b002870 /* Rx 512 to 1023 Bytes Octets Counter */
#define SWITCH_PAGE_28_RXPKTS1024TOMAXPKTOCTETS_IMP 0x0b002874 /* Rx 1024 to MaxPkt Bytes Octets Counter */
#define SWITCH_PAGE_28_RXOVERSIZEPKTS_IMP 0x0b002878 /* Rx Over Size Packet Counter */
#define SWITCH_PAGE_28_RXJABBERS_IMP   0x0b00287c /* Rx Jabber Packet Counter */
#define SWITCH_PAGE_28_RXALIGNMENTERRORS_IMP 0x0b002880 /* Rx Alignment Error Counter */
#define SWITCH_PAGE_28_RXFCSERRORS_IMP 0x0b002884 /* Rx FCS Error Counter */
#define SWITCH_PAGE_28_RXGOODOCTETS_IMP 0x0b002888 /* Rx Good Packet Octet Counter */
#define SWITCH_PAGE_28_RXDROPPKTS_IMP  0x0b002890 /* Rx Drop Packet Counter */
#define SWITCH_PAGE_28_RXUNICASTPKTS_IMP 0x0b002894 /* Rx Unicast Packet Counter */
#define SWITCH_PAGE_28_RXMULTICASTPKTS_IMP 0x0b002898 /* Rx Multicast Packet Counter */
#define SWITCH_PAGE_28_RXBROADCASTPKTS_IMP 0x0b00289c /* Rx Broadcast Packet Counter */
#define SWITCH_PAGE_28_RXSACHANGES_IMP 0x0b0028a0 /* Rx SA Change Counter */
#define SWITCH_PAGE_28_RXFRAGMENTS_IMP 0x0b0028a4 /* Rx Fragment Counter */
#define SWITCH_PAGE_28_RXJUMBOPKT_IMP  0x0b0028a8 /* Jumbo Packet Counter */
#define SWITCH_PAGE_28_RXSYMBLERR_IMP  0x0b0028ac /* Rx Symbol Error Counter */
#define SWITCH_PAGE_28_INRANGEERRCOUNT_IMP 0x0b0028b0 /* InRangeErrCount Counter */
#define SWITCH_PAGE_28_OUTRANGEERRCOUNT_IMP 0x0b0028b4 /* OutRangeErrCount Counter */
#define SWITCH_PAGE_28_EEE_LPI_EVENT_IMP 0x0b0028b8 /* EEE Low-Power Idle Event Registers */
#define SWITCH_PAGE_28_EEE_LPI_DURATION_IMP 0x0b0028bc /* EEE Low-Power Idle Duration Registers */
#define SWITCH_PAGE_28_RXDISCARD_IMP   0x0b0028c0 /* Rx Discard Counter */
#define SWITCH_PAGE_28_TXQPKTQ6_IMP    0x0b0028c8 /* Tx Q6 Packet Counter */
#define SWITCH_PAGE_28_TXQPKTQ7_IMP    0x0b0028cc /* Tx Q7 Packet Counter */
#define SWITCH_PAGE_28_TXPKTS64OCTETS_IMP 0x0b0028d0 /* Tx 64 Bytes Octets Counter */
#define SWITCH_PAGE_28_TXPKTS65TO127OCTETS_IMP 0x0b0028d4 /* Tx 65 to 127 Bytes Octets Counter */
#define SWITCH_PAGE_28_TXPKTS128TO255OCTETS_IMP 0x0b0028d8 /* Tx 128 to 255 Bytes Octets Counter */
#define SWITCH_PAGE_28_TXPKTS256TO511OCTETS_IMP 0x0b0028dc /* Tx 256 to 511 Bytes Octets Counter */
#define SWITCH_PAGE_28_TXPKTS512TO1023OCTETS_IMP 0x0b0028e0 /* Tx 512 to 1023 Bytes Octets Counter */
#define SWITCH_PAGE_28_TXPKTS1024TOMAXPKTOCTETS_IMP 0x0b0028e4 /* Tx 1024 to MaxPkt Bytes Octets Counter */
#define SWITCH_PAGE_30_QOS_GLOBAL_CTRL 0x0b003000 /* QOS Global Control Register */
#define SWITCH_PAGE_30_QOS_1P_EN       0x0b003004 /* QoS 802.1P Enable Register */
#define SWITCH_PAGE_30_QOS_EN_DIFFSERV 0x0b003006 /* QOS DiffServ Enable Register */
#define SWITCH_PAGE_30_PN_PCP2TC_DEI0_PORT0 0x0b003010 /* Port N (0 to 6) PCP to TC Map for DEI 0 Register */
#define SWITCH_PAGE_30_PN_PCP2TC_DEI0_PORT1 0x0b003014 /* Port N (0 to 6) PCP to TC Map for DEI 0 Register */
#define SWITCH_PAGE_30_PN_PCP2TC_DEI0_PORT2 0x0b003018 /* Port N (0 to 6) PCP to TC Map for DEI 0 Register */
#define SWITCH_PAGE_30_PN_PCP2TC_DEI0_PORT3 0x0b00301c /* Port N (0 to 6) PCP to TC Map for DEI 0 Register */
#define SWITCH_PAGE_30_PN_PCP2TC_DEI0_PORT4 0x0b003020 /* Port N (0 to 6) PCP to TC Map for DEI 0 Register */
#define SWITCH_PAGE_30_PN_PCP2TC_DEI0_PORT5 0x0b003024 /* Port N (0 to 6) PCP to TC Map for DEI 0 Register */
#define SWITCH_PAGE_30_PN_PCP2TC_DEI0_PORT6 0x0b003028 /* Port N (0 to 6) PCP to TC Map for DEI 0 Register */
#define SWITCH_PAGE_30_P7_PCP2TC_DEI0  0x0b00302c /* Port 7 PCP to TC Map for DEI 0 Register */
#define SWITCH_PAGE_30_IMP_PCP2TC_DEI0 0x0b003030 /* Port 8 (IMP) PCP to TC Map for DEI 0 Register */
#define SWITCH_PAGE_30_QOS_DIFF_DSCP0  0x0b003034 /* DiffServ Priority Map 0 Register */
#define SWITCH_PAGE_30_QOS_DIFF_DSCP1  0x0b00303a /* DiffServ Priority Map 1 Register */
#define SWITCH_PAGE_30_QOS_DIFF_DSCP2  0x0b003040 /* DiffServ Priority Map 2 Register */
#define SWITCH_PAGE_30_QOS_DIFF_DSCP3  0x0b003046 /* DiffServ Priority Map 3 Register */
#define SWITCH_PAGE_30_PID2TC          0x0b00304c /* Port ID to TC Map Register */
#define SWITCH_PAGE_30_TC_SEL_TABLE_PORT0 0x0b003054 /* Port N (0 to 6) TC Select Table Register */
#define SWITCH_PAGE_30_TC_SEL_TABLE_PORT1 0x0b003056 /* Port N (0 to 6) TC Select Table Register */
#define SWITCH_PAGE_30_TC_SEL_TABLE_PORT2 0x0b003058 /* Port N (0 to 6) TC Select Table Register */
#define SWITCH_PAGE_30_TC_SEL_TABLE_PORT3 0x0b00305a /* Port N (0 to 6) TC Select Table Register */
#define SWITCH_PAGE_30_TC_SEL_TABLE_PORT4 0x0b00305c /* Port N (0 to 6) TC Select Table Register */
#define SWITCH_PAGE_30_TC_SEL_TABLE_PORT5 0x0b00305e /* Port N (0 to 6) TC Select Table Register */
#define SWITCH_PAGE_30_TC_SEL_TABLE_PORT6 0x0b003060 /* Port N (0 to 6) TC Select Table Register */
#define SWITCH_PAGE_30_P7_TC_SEL_TABLE 0x0b003062 /* Port 7 TC Select Table Register */
#define SWITCH_PAGE_30_IMP_TC_SEL_TABLE 0x0b003064 /* Port 8 TC Select Table Register */
#define SWITCH_PAGE_30_CPU2COS_MAP     0x0b003068 /* CPU to COS Mapping Register */
#define SWITCH_PAGE_30_PN_TC2COS_MAP_PORT0 0x0b003070 /* Port N (0 to 6) TC to COS Mapping Register */
#define SWITCH_PAGE_30_PN_TC2COS_MAP_PORT1 0x0b003074 /* Port N (0 to 6) TC to COS Mapping Register */
#define SWITCH_PAGE_30_PN_TC2COS_MAP_PORT2 0x0b003078 /* Port N (0 to 6) TC to COS Mapping Register */
#define SWITCH_PAGE_30_PN_TC2COS_MAP_PORT3 0x0b00307c /* Port N (0 to 6) TC to COS Mapping Register */
#define SWITCH_PAGE_30_PN_TC2COS_MAP_PORT4 0x0b003080 /* Port N (0 to 6) TC to COS Mapping Register */
#define SWITCH_PAGE_30_PN_TC2COS_MAP_PORT5 0x0b003084 /* Port N (0 to 6) TC to COS Mapping Register */
#define SWITCH_PAGE_30_PN_TC2COS_MAP_PORT6 0x0b003088 /* Port N (0 to 6) TC to COS Mapping Register */
#define SWITCH_PAGE_30_P7_TC2COS_MAP   0x0b00308c /* Port 7 TC to COS Mapping Register */
#define SWITCH_PAGE_30_IMP_TC2COS_MAP  0x0b003090 /* Port 8 TC to COS Mapping Register */
#define SWITCH_PAGE_30_QOS_REG_SPARE0  0x0b0030a8 /* Spare 0 Register (Not2Release) */
#define SWITCH_PAGE_30_QOS_REG_SPARE1  0x0b0030ac /* Spare 1 Register (Not2Release) */
#define SWITCH_PAGE_30_PN_PCP2TC_DEI1_PORT0 0x0b0030b0 /* Port N (0 to 6) PCP to TC Map for DEI 1 Register */
#define SWITCH_PAGE_30_PN_PCP2TC_DEI1_PORT1 0x0b0030b4 /* Port N (0 to 6) PCP to TC Map for DEI 1 Register */
#define SWITCH_PAGE_30_PN_PCP2TC_DEI1_PORT2 0x0b0030b8 /* Port N (0 to 6) PCP to TC Map for DEI 1 Register */
#define SWITCH_PAGE_30_PN_PCP2TC_DEI1_PORT3 0x0b0030bc /* Port N (0 to 6) PCP to TC Map for DEI 1 Register */
#define SWITCH_PAGE_30_PN_PCP2TC_DEI1_PORT4 0x0b0030c0 /* Port N (0 to 6) PCP to TC Map for DEI 1 Register */
#define SWITCH_PAGE_30_PN_PCP2TC_DEI1_PORT5 0x0b0030c4 /* Port N (0 to 6) PCP to TC Map for DEI 1 Register */
#define SWITCH_PAGE_30_PN_PCP2TC_DEI1_PORT6 0x0b0030c8 /* Port N (0 to 6) PCP to TC Map for DEI 1 Register */
#define SWITCH_PAGE_30_P7_PCP2TC_DEI1  0x0b0030cc /* Port 7 PCP to TC Map for DEI 1 Register */
#define SWITCH_PAGE_30_IMP_PCP2TC_DEI1 0x0b0030d0 /* Port 8 (IMP) PCP to TC Map for DEI 1 Register */
#define SWITCH_PAGE_31_PORT_VLAN_CTL_PORT0 0x0b003100 /* PORT N (0 to 6) VLAN Control Register */
#define SWITCH_PAGE_31_PORT_VLAN_CTL_PORT1 0x0b003102 /* PORT N (0 to 6) VLAN Control Register */
#define SWITCH_PAGE_31_PORT_VLAN_CTL_PORT2 0x0b003104 /* PORT N (0 to 6) VLAN Control Register */
#define SWITCH_PAGE_31_PORT_VLAN_CTL_PORT3 0x0b003106 /* PORT N (0 to 6) VLAN Control Register */
#define SWITCH_PAGE_31_PORT_VLAN_CTL_PORT4 0x0b003108 /* PORT N (0 to 6) VLAN Control Register */
#define SWITCH_PAGE_31_PORT_VLAN_CTL_PORT5 0x0b00310a /* PORT N (0 to 6) VLAN Control Register */
#define SWITCH_PAGE_31_PORT_VLAN_CTL_PORT6 0x0b00310c /* PORT N (0 to 6) VLAN Control Register */
#define SWITCH_PAGE_31_PORT_VLAN_CTL_P7 0x0b00310e /* PORT 7 VLAN Control Register */
#define SWITCH_PAGE_31_PORT_VLAN_CTL_IMP 0x0b003110 /* PORT 8 VLAN Control Register */
#define SWITCH_PAGE_31_VLAN_REG_SPARE0 0x0b003120 /* Spare 0 Register (Not2Release) */
#define SWITCH_PAGE_31_VLAN_REG_SPARE1 0x0b003124 /* Spare 1 Register (Not2Release) */
#define SWITCH_PAGE_32_MAC_TRUNK_CTL   0x0b003200 /* MAC Trunk Control Register */
#define SWITCH_PAGE_32_TRUNK_GRP_CTL0  0x0b003210 /* Trunk 0 Group Control Register */
#define SWITCH_PAGE_32_TRUNK_GRP_CTL1  0x0b003212 /* Trunk 1 Group Control Register */
#define SWITCH_PAGE_32_TRUNK_GRP_CTL2  0x0b003214 /* Trunk 2 Group Control Register */
#define SWITCH_PAGE_32_TRUNK_GRP_CTL3  0x0b003216 /* Trunk 3 Group Control Register */
#define SWITCH_PAGE_32_TRUNK_REG_SPARE0 0x0b003220 /* Spare 0 Register (Not2Release) */
#define SWITCH_PAGE_32_TRUNK_REG_SPARE1 0x0b003224 /* Spare 1 Register (Not2Release) */
#define SWITCH_PAGE_34_VLAN_CTRL0      0x0b003400 /* 802.1Q VLAN Control 0 Registers */
#define SWITCH_PAGE_34_VLAN_CTRL1      0x0b003401 /* 802.1Q VLAN Control 1 Registers */
#define SWITCH_PAGE_34_VLAN_CTRL2      0x0b003402 /* 802.1Q VLAN Control 2 Registers */
#define SWITCH_PAGE_34_VLAN_CTRL3      0x0b003403 /* 802.1Q VLAN Control 3 Registers */
#define SWITCH_PAGE_34_VLAN_CTRL4      0x0b003405 /* 802.1Q VLAN Control 4 Registers */
#define SWITCH_PAGE_34_VLAN_CTRL5      0x0b003406 /* 802.1Q VLAN Control 5 Registers */
#define SWITCH_PAGE_34_VLAN_CTRL6      0x0b003407 /* 802.1Q VLAN Control 6 Registers */
#define SWITCH_PAGE_34_VLAN_MULTI_PORT_ADDR_CTL 0x0b00340a /* VLAN Multiport Address Control Register */
#define SWITCH_PAGE_34_DEFAULT_1Q_TAG_PORT0 0x0b003410 /* Port N (0 to 6) 802.1Q Default Tag Registers */
#define SWITCH_PAGE_34_DEFAULT_1Q_TAG_PORT1 0x0b003412 /* Port N (0 to 6) 802.1Q Default Tag Registers */
#define SWITCH_PAGE_34_DEFAULT_1Q_TAG_PORT2 0x0b003414 /* Port N (0 to 6) 802.1Q Default Tag Registers */
#define SWITCH_PAGE_34_DEFAULT_1Q_TAG_PORT3 0x0b003416 /* Port N (0 to 6) 802.1Q Default Tag Registers */
#define SWITCH_PAGE_34_DEFAULT_1Q_TAG_PORT4 0x0b003418 /* Port N (0 to 6) 802.1Q Default Tag Registers */
#define SWITCH_PAGE_34_DEFAULT_1Q_TAG_PORT5 0x0b00341a /* Port N (0 to 6) 802.1Q Default Tag Registers */
#define SWITCH_PAGE_34_DEFAULT_1Q_TAG_PORT6 0x0b00341c /* Port N (0 to 6) 802.1Q Default Tag Registers */
#define SWITCH_PAGE_34_DEFAULT_1Q_TAG_P7 0x0b00341e /* Port 7 802.1Q Default Tag Registers */
#define SWITCH_PAGE_34_DEFAULT_1Q_TAG_IMP 0x0b003420 /* Port 8 802.1Q Default Tag Registers */
#define SWITCH_PAGE_34_DTAG_TPID       0x0b003430 /* Double Tagging TPID Registers */
#define SWITCH_PAGE_34_ISP_SEL_PORTMAP 0x0b003432 /* ISP Port Selection Portmap Registers */
#define SWITCH_PAGE_34_EGRESS_VID_RMK_TBL_ACS 0x0b003440 /* Egress VID Remarking Table Access Register */
#define SWITCH_PAGE_34_EGRESS_VID_RMK_TBL_DATA 0x0b003444 /* Egress VID Remarking Table Data Register */
#define SWITCH_PAGE_34_JOIN_ALL_VLAN_EN 0x0b003450 /* Join All VLAN Enable Register */
#define SWITCH_PAGE_34_PORT_IVL_SVL_CTRL 0x0b003452 /* Port IVL or SVL Control Register */
#define SWITCH_PAGE_34_BCM8021Q_REG_SPARE0 0x0b003460 /* Spare 0 Register (Not2Release) */
#define SWITCH_PAGE_34_BCM8021Q_REG_SPARE1 0x0b003464 /* Spare 1 Register (Not2Release) */
#define SWITCH_PAGE_36_DOS_CTRL        0x0b003600 /* DoS Control RegisterRegister */
#define SWITCH_PAGE_36_MINIMUM_TCP_HDR_SZ 0x0b003604 /* Minimum TCP Header Size Register */
#define SWITCH_PAGE_36_MAX_ICMPV4_SIZE_REG 0x0b003608 /* Maximum ICMPv4 Size Register */
#define SWITCH_PAGE_36_MAX_ICMPV6_SIZE_REG 0x0b00360c /* Maximum ICMPv6 Size Register */
#define SWITCH_PAGE_36_DOS_DIS_LRN_REG 0x0b003610 /* DoS Disable Learn Register */
#define SWITCH_PAGE_36_DOS_REG_SPARE0  0x0b003620 /* Spare 0 Register (Not2Release) */
#define SWITCH_PAGE_36_DOS_REG_SPARE1  0x0b003624 /* Spare 1 Register (Not2Release) */
#define SWITCH_PAGE_40_JUMBO_PORT_MASK 0x0b004001 /* Jumbo Frame Port Mask Registers */
#define SWITCH_PAGE_40_MIB_GD_FM_MAX_SIZE 0x0b004005 /* Jumbo MIB Good Frame Max Size Registers */
#define SWITCH_PAGE_40_JUMBO_CTRL_REG_SPARE0 0x0b004010 /* Spare 0 Register (Not2Release) */
#define SWITCH_PAGE_40_JUMBO_CTRL_REG_SPARE1 0x0b004014 /* Spare 1 Register (Not2Release) */
#define SWITCH_PAGE_41_COMM_IRC_CON    0x0b004100 /* Common Ingress rate Control Configuration Registers */
#define SWITCH_PAGE_41_IRC_VIRTUAL_ZERO_THD 0x0b004104 /* Ingress Rate Control Virtual Zero Threshold Register (Not2Release) */
#define SWITCH_PAGE_41_IRC_ALARM_THD   0x0b004106 /* Ingress Rate Control Alarm Threshold Register (Not2Release) */
#define SWITCH_PAGE_41_BC_SUP_RATECTRL_P_PORT0 0x0b004110 /* Port N (0 to 6) Receive Rate Control Registers */
#define SWITCH_PAGE_41_BC_SUP_RATECTRL_P_PORT1 0x0b004114 /* Port N (0 to 6) Receive Rate Control Registers */
#define SWITCH_PAGE_41_BC_SUP_RATECTRL_P_PORT2 0x0b004118 /* Port N (0 to 6) Receive Rate Control Registers */
#define SWITCH_PAGE_41_BC_SUP_RATECTRL_P_PORT3 0x0b00411c /* Port N (0 to 6) Receive Rate Control Registers */
#define SWITCH_PAGE_41_BC_SUP_RATECTRL_P_PORT4 0x0b004120 /* Port N (0 to 6) Receive Rate Control Registers */
#define SWITCH_PAGE_41_BC_SUP_RATECTRL_P_PORT5 0x0b004124 /* Port N (0 to 6) Receive Rate Control Registers */
#define SWITCH_PAGE_41_BC_SUP_RATECTRL_P_PORT6 0x0b004128 /* Port N (0 to 6) Receive Rate Control Registers */
#define SWITCH_PAGE_41_BC_SUP_RATECTRL_P7 0x0b00412c /* Port 7 Receive Rate Control Registers */
#define SWITCH_PAGE_41_BC_SUP_RATECTRL_IMP 0x0b004130 /* Port 8 Receive Rate Control Registers */
#define SWITCH_PAGE_41_BC_SUP_RATECTRL_1_P_PORT0 0x0b004134 /* Port N (0 to 6) Receive Rate Control 1 Registers */
#define SWITCH_PAGE_41_BC_SUP_RATECTRL_1_P_PORT1 0x0b004136 /* Port N (0 to 6) Receive Rate Control 1 Registers */
#define SWITCH_PAGE_41_BC_SUP_RATECTRL_1_P_PORT2 0x0b004138 /* Port N (0 to 6) Receive Rate Control 1 Registers */
#define SWITCH_PAGE_41_BC_SUP_RATECTRL_1_P_PORT3 0x0b00413a /* Port N (0 to 6) Receive Rate Control 1 Registers */
#define SWITCH_PAGE_41_BC_SUP_RATECTRL_1_P_PORT4 0x0b00413c /* Port N (0 to 6) Receive Rate Control 1 Registers */
#define SWITCH_PAGE_41_BC_SUP_RATECTRL_1_P_PORT5 0x0b00413e /* Port N (0 to 6) Receive Rate Control 1 Registers */
#define SWITCH_PAGE_41_BC_SUP_RATECTRL_1_P_PORT6 0x0b004140 /* Port N (0 to 6) Receive Rate Control 1 Registers */
#define SWITCH_PAGE_41_BC_SUP_RATECTRL_1_P7 0x0b004142 /* Port 7 Receive Rate Control 1 Register */
#define SWITCH_PAGE_41_BC_SUP_RATECTRL_1_IMP 0x0b004144 /* Port 8 Receive Rate Control 1 Register */
#define SWITCH_PAGE_41_BC_SUP_PKTDROP_CNT_P_PORT0 0x0b004150 /* Port N (0 to 6) Suppressed Packet Drop Counter Register */
#define SWITCH_PAGE_41_BC_SUP_PKTDROP_CNT_P_PORT1 0x0b004154 /* Port N (0 to 6) Suppressed Packet Drop Counter Register */
#define SWITCH_PAGE_41_BC_SUP_PKTDROP_CNT_P_PORT2 0x0b004158 /* Port N (0 to 6) Suppressed Packet Drop Counter Register */
#define SWITCH_PAGE_41_BC_SUP_PKTDROP_CNT_P_PORT3 0x0b00415c /* Port N (0 to 6) Suppressed Packet Drop Counter Register */
#define SWITCH_PAGE_41_BC_SUP_PKTDROP_CNT_P_PORT4 0x0b004160 /* Port N (0 to 6) Suppressed Packet Drop Counter Register */
#define SWITCH_PAGE_41_BC_SUP_PKTDROP_CNT_P_PORT5 0x0b004164 /* Port N (0 to 6) Suppressed Packet Drop Counter Register */
#define SWITCH_PAGE_41_BC_SUP_PKTDROP_CNT_P_PORT6 0x0b004168 /* Port N (0 to 6) Suppressed Packet Drop Counter Register */
#define SWITCH_PAGE_41_BC_SUP_PKTDROP_CNT_P7 0x0b00416c /* Port 7 Suppressed Packet Drop Counter Register */
#define SWITCH_PAGE_41_BC_SUP_PKTDROP_CNT_IMP 0x0b004170 /* Port 8 Suppressed Packet Drop Counter Register */
#define SWITCH_PAGE_41_BC_SUPPRESS_REG_SPARE0 0x0b0041d0 /* Spare 0 Register (Not2Release) */
#define SWITCH_PAGE_41_BC_SUPPRESS_REG_SPARE1 0x0b0041d4 /* Spare 1 Register (Not2Release) */
#define SWITCH_PAGE_42_EAP_GLO_CON     0x0b004200 /* EAP Global Configuration Registers */
#define SWITCH_PAGE_42_EAP_MULTI_ADDR_CTRL 0x0b004201 /* EAP Multiport Address Control Register */
#define SWITCH_PAGE_42_EAP_DIP0        0x0b004202 /* EAP Destination IP Registers */
#define SWITCH_PAGE_42_EAP_DIP1        0x0b00420a /* EAP Destination IP Registers */
#define SWITCH_PAGE_42_PORT_EAP_CON_P0 0x0b004220 /* Port 0 EAP Configuration Registers */
#define SWITCH_PAGE_42_PORT_EAP_CON_P1 0x0b004228 /* Port 1 EAP Configuration Registers */
#define SWITCH_PAGE_42_PORT_EAP_CON_P2 0x0b004230 /* Port 2 EAP Configuration Registers */
#define SWITCH_PAGE_42_PORT_EAP_CON_P3 0x0b004238 /* Port 3 EAP Configuration Registers */
#define SWITCH_PAGE_42_PORT_EAP_CON_P4 0x0b004240 /* Port 4 EAP Configuration Registers */
#define SWITCH_PAGE_42_PORT_EAP_CON_P5 0x0b004248 /* Port 5 EAP Configuration Registers */
#define SWITCH_PAGE_42_PORT_EAP_CON_P6 0x0b004250 /* Port 6 EAP Configuration Registers */
#define SWITCH_PAGE_42_PORT_EAP_CON_P7 0x0b004258 /* Port 7 EAP Configuration Registers */
#define SWITCH_PAGE_42_PORT_EAP_CON_IMP 0x0b004260 /* IMP EAP Configuration Registers */
#define SWITCH_PAGE_42_IEEE8021X_REG_SPARE0 0x0b004270 /* Spare 0 Register (Not2Release) */
#define SWITCH_PAGE_42_IEEE8021X_REG_SPARE1 0x0b004274 /* Spare 1 Register (Not2Release) */
#define SWITCH_PAGE_43_MST_CON         0x0b004300 /* MST Control Registers */
#define SWITCH_PAGE_43_MST_AGE         0x0b004302 /* MST Ageing Control Register */
#define SWITCH_PAGE_43_MST_TAB_0       0x0b004310 /* Multiple Spanning Tree Table 0 Enable Registers */
#define SWITCH_PAGE_43_MST_TAB_1       0x0b004314 /* Multiple Spanning Tree Table 1 Enable Registers */
#define SWITCH_PAGE_43_MST_TAB_2       0x0b004318 /* Multiple Spanning Tree Table 2 Enable Registers */
#define SWITCH_PAGE_43_MST_TAB_3       0x0b00431c /* Multiple Spanning Tree Table 3 Enable Registers */
#define SWITCH_PAGE_43_MST_TAB_4       0x0b004320 /* Multiple Spanning Tree Table 0 Enable Registers */
#define SWITCH_PAGE_43_MST_TAB_5       0x0b004324 /* Multiple Spanning Tree Table 5 Enable Registers */
#define SWITCH_PAGE_43_MST_TAB_6       0x0b004328 /* Multiple Spanning Tree Table 6 Enable Registers */
#define SWITCH_PAGE_43_MST_TAB_7       0x0b00432c /* Multiple Spanning Tree Table 0 Enable Registers */
#define SWITCH_PAGE_43_SPT_MULTI_ADDR_BPS_CTRL 0x0b004350 /* STP Multiport Address Bypass Control Register */
#define SWITCH_PAGE_43_IEEE8021S_REG_SPARE0 0x0b004360 /* Spare 0 Register (Not2Release) */
#define SWITCH_PAGE_43_IEEE8021S_REG_SPARE1 0x0b004364 /* Spare 1 Register (Not2Release) */
#define SWITCH_PAGE_45_SA_LIMIT_ENABLE 0x0b004500 /* SA Limit Enable Register */
#define SWITCH_PAGE_45_SA_LRN_CNTR_RST 0x0b004502 /* SA Learned Counters Reset Register */
#define SWITCH_PAGE_45_SA_OVERLIMIT_CNTR_RST 0x0b004504 /* SA Over Limit Counters Reset Register */
#define SWITCH_PAGE_45_TOTAL_SA_LIMIT_CTL 0x0b004510 /* Total SA Limit Control Register */
#define SWITCH_PAGE_45_PORT_N_SA_LIMIT_CTL_PORT0 0x0b004512 /* Port N (0 to 6) SA Limit Control Register */
#define SWITCH_PAGE_45_PORT_N_SA_LIMIT_CTL_PORT1 0x0b004514 /* Port N (0 to 6) SA Limit Control Register */
#define SWITCH_PAGE_45_PORT_N_SA_LIMIT_CTL_PORT2 0x0b004516 /* Port N (0 to 6) SA Limit Control Register */
#define SWITCH_PAGE_45_PORT_N_SA_LIMIT_CTL_PORT3 0x0b004518 /* Port N (0 to 6) SA Limit Control Register */
#define SWITCH_PAGE_45_PORT_N_SA_LIMIT_CTL_PORT4 0x0b00451a /* Port N (0 to 6) SA Limit Control Register */
#define SWITCH_PAGE_45_PORT_N_SA_LIMIT_CTL_PORT5 0x0b00451c /* Port N (0 to 6) SA Limit Control Register */
#define SWITCH_PAGE_45_PORT_N_SA_LIMIT_CTL_PORT6 0x0b00451e /* Port N (0 to 6) SA Limit Control Register */
#define SWITCH_PAGE_45_PORT_7_SA_LIMIT_CTL 0x0b004520 /* Port 7 SA Limit Control Register */
#define SWITCH_PAGE_45_PORT_8_SA_LIMIT_CTL 0x0b004522 /* Port 8 SA Limit Control Register */
#define SWITCH_PAGE_45_TOTAL_SA_LRN_CNTR 0x0b004530 /* Total SA Learned Counter Register */
#define SWITCH_PAGE_45_PORT_N_SA_LRN_CNTR_PORT0 0x0b004532 /* Port N (0 to 6) SA Learned Counter Register */
#define SWITCH_PAGE_45_PORT_N_SA_LRN_CNTR_PORT1 0x0b004534 /* Port N (0 to 6) SA Learned Counter Register */
#define SWITCH_PAGE_45_PORT_N_SA_LRN_CNTR_PORT2 0x0b004536 /* Port N (0 to 6) SA Learned Counter Register */
#define SWITCH_PAGE_45_PORT_N_SA_LRN_CNTR_PORT3 0x0b004538 /* Port N (0 to 6) SA Learned Counter Register */
#define SWITCH_PAGE_45_PORT_N_SA_LRN_CNTR_PORT4 0x0b00453a /* Port N (0 to 6) SA Learned Counter Register */
#define SWITCH_PAGE_45_PORT_N_SA_LRN_CNTR_PORT5 0x0b00453c /* Port N (0 to 6) SA Learned Counter Register */
#define SWITCH_PAGE_45_PORT_N_SA_LRN_CNTR_PORT6 0x0b00453e /* Port N (0 to 6) SA Learned Counter Register */
#define SWITCH_PAGE_45_PORT_7_SA_LRN_CNTR 0x0b004540 /* Port 7 SA Learned Counter Register */
#define SWITCH_PAGE_45_PORT_8_SA_LRN_CNTR 0x0b004542 /* Port 8 SA Learned Counter Register */
#define SWITCH_PAGE_45_PORT_N_SA_OVERLIMIT_CNTR_PORT0 0x0b004550 /* Port N (0 to 6) SA Over Limit Counter Register */
#define SWITCH_PAGE_45_PORT_N_SA_OVERLIMIT_CNTR_PORT1 0x0b004554 /* Port N (0 to 6) SA Over Limit Counter Register */
#define SWITCH_PAGE_45_PORT_N_SA_OVERLIMIT_CNTR_PORT2 0x0b004558 /* Port N (0 to 6) SA Over Limit Counter Register */
#define SWITCH_PAGE_45_PORT_N_SA_OVERLIMIT_CNTR_PORT3 0x0b00455c /* Port N (0 to 6) SA Over Limit Counter Register */
#define SWITCH_PAGE_45_PORT_N_SA_OVERLIMIT_CNTR_PORT4 0x0b004560 /* Port N (0 to 6) SA Over Limit Counter Register */
#define SWITCH_PAGE_45_PORT_N_SA_OVERLIMIT_CNTR_PORT5 0x0b004564 /* Port N (0 to 6) SA Over Limit Counter Register */
#define SWITCH_PAGE_45_PORT_N_SA_OVERLIMIT_CNTR_PORT6 0x0b004568 /* Port N (0 to 6) SA Over Limit Counter Register */
#define SWITCH_PAGE_45_PORT_7_SA_OVERLIMIT_CNTR 0x0b00456c /* Port 7 SA Over Limit Counter Register */
#define SWITCH_PAGE_45_PORT_8_SA_OVERLIMIT_CNTR 0x0b004570 /* Port 8 SA Over Limit Counter Register */
#define SWITCH_PAGE_45_SA_OVER_LIMIT_COPY_REDIRECT 0x0b004574 /* SA Over Limit Actions Config Register */
#define SWITCH_PAGE_45_MAC_LIMIT_REG_SPARE0 0x0b004580 /* Spare 0 Register (Not2Release) */
#define SWITCH_PAGE_45_MAC_LIMIT_REG_SPARE1 0x0b004584 /* Spare 1 Register (Not2Release) */
#define SWITCH_PAGE_46_PN_QOS_PRI_CTL_PORT0 0x0b004600 /* "Port N (0 to 6), QOS Priority Control Register" */
#define SWITCH_PAGE_46_PN_QOS_PRI_CTL_PORT1 0x0b004601 /* "Port N (0 to 6), QOS Priority Control Register" */
#define SWITCH_PAGE_46_PN_QOS_PRI_CTL_PORT2 0x0b004602 /* "Port N (0 to 6), QOS Priority Control Register" */
#define SWITCH_PAGE_46_PN_QOS_PRI_CTL_PORT3 0x0b004603 /* "Port N (0 to 6), QOS Priority Control Register" */
#define SWITCH_PAGE_46_PN_QOS_PRI_CTL_PORT4 0x0b004604 /* "Port N (0 to 6), QOS Priority Control Register" */
#define SWITCH_PAGE_46_PN_QOS_PRI_CTL_PORT5 0x0b004605 /* "Port N (0 to 6), QOS Priority Control Register" */
#define SWITCH_PAGE_46_PN_QOS_PRI_CTL_PORT6 0x0b004606 /* "Port N (0 to 6), QOS Priority Control Register" */
#define SWITCH_PAGE_46_P7_QOS_PRI_CTL  0x0b004607 /* "Port 7, QOS Priority Control Register" */
#define SWITCH_PAGE_46_IMP_QOS_PRI_CTL 0x0b004608 /* "Port 8, QOS Priority Control Register" */
#define SWITCH_PAGE_46_PN_QOS_WEIGHT_PORT0 0x0b004610 /* "Port N (0 to 6), QOS Weight Register" */
#define SWITCH_PAGE_46_PN_QOS_WEIGHT_PORT1 0x0b004618 /* "Port N (0 to 6), QOS Weight Register" */
#define SWITCH_PAGE_46_PN_QOS_WEIGHT_PORT2 0x0b004620 /* "Port N (0 to 6), QOS Weight Register" */
#define SWITCH_PAGE_46_PN_QOS_WEIGHT_PORT3 0x0b004628 /* "Port N (0 to 6), QOS Weight Register" */
#define SWITCH_PAGE_46_PN_QOS_WEIGHT_PORT4 0x0b004630 /* "Port N (0 to 6), QOS Weight Register" */
#define SWITCH_PAGE_46_PN_QOS_WEIGHT_PORT5 0x0b004638 /* "Port N (0 to 6), QOS Weight Register" */
#define SWITCH_PAGE_46_PN_QOS_WEIGHT_PORT6 0x0b004640 /* "Port N (0 to 6), QOS Weight Register" */
#define SWITCH_PAGE_46_P7_QOS_WEIGHT   0x0b004648 /* "Port 7, QOS Weight Register" */
#define SWITCH_PAGE_46_IMP_QOS_WEIGHT  0x0b004650 /* "Port 8, QOS Weight Register" */
#define SWITCH_PAGE_46_PN_WDRR_PENALTY_PORT0 0x0b004660 /* "Port N (0 to 6), WDRR Weight-Scaling Penalty Register (Not2Release)" */
#define SWITCH_PAGE_46_PN_WDRR_PENALTY_PORT1 0x0b004662 /* "Port N (0 to 6), WDRR Weight-Scaling Penalty Register (Not2Release)" */
#define SWITCH_PAGE_46_PN_WDRR_PENALTY_PORT2 0x0b004664 /* "Port N (0 to 6), WDRR Weight-Scaling Penalty Register (Not2Release)" */
#define SWITCH_PAGE_46_PN_WDRR_PENALTY_PORT3 0x0b004666 /* "Port N (0 to 6), WDRR Weight-Scaling Penalty Register (Not2Release)" */
#define SWITCH_PAGE_46_PN_WDRR_PENALTY_PORT4 0x0b004668 /* "Port N (0 to 6), WDRR Weight-Scaling Penalty Register (Not2Release)" */
#define SWITCH_PAGE_46_PN_WDRR_PENALTY_PORT5 0x0b00466a /* "Port N (0 to 6), WDRR Weight-Scaling Penalty Register (Not2Release)" */
#define SWITCH_PAGE_46_PN_WDRR_PENALTY_PORT6 0x0b00466c /* "Port N (0 to 6), WDRR Weight-Scaling Penalty Register (Not2Release)" */
#define SWITCH_PAGE_46_P7_WDRR_PENALTY 0x0b004670 /* "Port 7, WDRR Weight-Scaling Penalty Register (Not2Release)" */
#define SWITCH_PAGE_46_P8_WDRR_PENALTY 0x0b004672 /* "Port 8, WDRR Weight-Scaling Penalty Register (Not2Release)" */
#define SWITCH_PAGE_46_SCHEDULER_REG_SPARE0 0x0b004680 /* Spare 0 Register (Not2Release) */
#define SWITCH_PAGE_46_SCHEDULER_REG_SPARE1 0x0b004684 /* Spare 1 Register (Not2Release) */
#define SWITCH_PAGE_47_PN_PORT_SHAPER_BYTE_BASED_MAX_REFRESH_PORT0 0x0b004700 /* "Port N (0 to 6), Byte-Based, Port Shaper Shaping Rate Configure Register" */
#define SWITCH_PAGE_47_PN_PORT_SHAPER_BYTE_BASED_MAX_REFRESH_PORT1 0x0b004704 /* "Port N (0 to 6), Byte-Based, Port Shaper Shaping Rate Configure Register" */
#define SWITCH_PAGE_47_PN_PORT_SHAPER_BYTE_BASED_MAX_REFRESH_PORT2 0x0b004708 /* "Port N (0 to 6), Byte-Based, Port Shaper Shaping Rate Configure Register" */
#define SWITCH_PAGE_47_PN_PORT_SHAPER_BYTE_BASED_MAX_REFRESH_PORT3 0x0b00470c /* "Port N (0 to 6), Byte-Based, Port Shaper Shaping Rate Configure Register" */
#define SWITCH_PAGE_47_PN_PORT_SHAPER_BYTE_BASED_MAX_REFRESH_PORT4 0x0b004710 /* "Port N (0 to 6), Byte-Based, Port Shaper Shaping Rate Configure Register" */
#define SWITCH_PAGE_47_PN_PORT_SHAPER_BYTE_BASED_MAX_REFRESH_PORT5 0x0b004714 /* "Port N (0 to 6), Byte-Based, Port Shaper Shaping Rate Configure Register" */
#define SWITCH_PAGE_47_PN_PORT_SHAPER_BYTE_BASED_MAX_REFRESH_PORT6 0x0b004718 /* "Port N (0 to 6), Byte-Based, Port Shaper Shaping Rate Configure Register" */
#define SWITCH_PAGE_47_P7_PORT_SHAPER_BYTE_BASED_MAX_REFRESH 0x0b00471c /* "Port 7, Byte-Based, Port Shaper Shaping Rate Configure Register" */
#define SWITCH_PAGE_47_IMP_PORT_SHAPER_BYTE_BASED_MAX_REFRESH 0x0b004720 /* "Port 8, Byte-Based, Port Shaper Shaping Rate Configure Register" */
#define SWITCH_PAGE_47_PN_PORT_SHAPER_BYTE_BASED_MAX_THD_SEL_PORT0 0x0b004730 /* "Port N (0 to 6), Byte-Based, Port Shaper Burst Size Configure Register" */
#define SWITCH_PAGE_47_PN_PORT_SHAPER_BYTE_BASED_MAX_THD_SEL_PORT1 0x0b004734 /* "Port N (0 to 6), Byte-Based, Port Shaper Burst Size Configure Register" */
#define SWITCH_PAGE_47_PN_PORT_SHAPER_BYTE_BASED_MAX_THD_SEL_PORT2 0x0b004738 /* "Port N (0 to 6), Byte-Based, Port Shaper Burst Size Configure Register" */
#define SWITCH_PAGE_47_PN_PORT_SHAPER_BYTE_BASED_MAX_THD_SEL_PORT3 0x0b00473c /* "Port N (0 to 6), Byte-Based, Port Shaper Burst Size Configure Register" */
#define SWITCH_PAGE_47_PN_PORT_SHAPER_BYTE_BASED_MAX_THD_SEL_PORT4 0x0b004740 /* "Port N (0 to 6), Byte-Based, Port Shaper Burst Size Configure Register" */
#define SWITCH_PAGE_47_PN_PORT_SHAPER_BYTE_BASED_MAX_THD_SEL_PORT5 0x0b004744 /* "Port N (0 to 6), Byte-Based, Port Shaper Burst Size Configure Register" */
#define SWITCH_PAGE_47_PN_PORT_SHAPER_BYTE_BASED_MAX_THD_SEL_PORT6 0x0b004748 /* "Port N (0 to 6), Byte-Based, Port Shaper Burst Size Configure Register" */
#define SWITCH_PAGE_47_P7_PORT_SHAPER_BYTE_BASED_MAX_THD_SEL 0x0b00474c /* "Port 7, Byte-Based, Port Shaper Burst Size Configure Register" */
#define SWITCH_PAGE_47_IMP_PORT_SHAPER_BYTE_BASED_MAX_THD_SEL 0x0b004750 /* "Port 8, Byte-Based, Port Shaper Burst Size Configure Register" */
#define SWITCH_PAGE_47_PN_PORT_SHAPER_STS_PORT0 0x0b004760 /* "Port N (0 to 6), PORT Shaper Status Register" */
#define SWITCH_PAGE_47_PN_PORT_SHAPER_STS_PORT1 0x0b004764 /* "Port N (0 to 6), PORT Shaper Status Register" */
#define SWITCH_PAGE_47_PN_PORT_SHAPER_STS_PORT2 0x0b004768 /* "Port N (0 to 6), PORT Shaper Status Register" */
#define SWITCH_PAGE_47_PN_PORT_SHAPER_STS_PORT3 0x0b00476c /* "Port N (0 to 6), PORT Shaper Status Register" */
#define SWITCH_PAGE_47_PN_PORT_SHAPER_STS_PORT4 0x0b004770 /* "Port N (0 to 6), PORT Shaper Status Register" */
#define SWITCH_PAGE_47_PN_PORT_SHAPER_STS_PORT5 0x0b004774 /* "Port N (0 to 6), PORT Shaper Status Register" */
#define SWITCH_PAGE_47_PN_PORT_SHAPER_STS_PORT6 0x0b004778 /* "Port N (0 to 6), PORT Shaper Status Register" */
#define SWITCH_PAGE_47_P7_PORT_SHAPER_STS 0x0b00477c /* "Port 7, PORT Shaper Status Register" */
#define SWITCH_PAGE_47_IMP_PORT_SHAPER_STS 0x0b004780 /* "Port 8, PORT Shaper Status Register" */
#define SWITCH_PAGE_47_PN_PORT_SHAPER_PACKET_BASED_MAX_REFRESH_PORT0 0x0b004790 /* "Port N (0 to 6), Packet-Based, Port Shaper Shaping Rate Configure Register" */
#define SWITCH_PAGE_47_PN_PORT_SHAPER_PACKET_BASED_MAX_REFRESH_PORT1 0x0b004794 /* "Port N (0 to 6), Packet-Based, Port Shaper Shaping Rate Configure Register" */
#define SWITCH_PAGE_47_PN_PORT_SHAPER_PACKET_BASED_MAX_REFRESH_PORT2 0x0b004798 /* "Port N (0 to 6), Packet-Based, Port Shaper Shaping Rate Configure Register" */
#define SWITCH_PAGE_47_PN_PORT_SHAPER_PACKET_BASED_MAX_REFRESH_PORT3 0x0b00479c /* "Port N (0 to 6), Packet-Based, Port Shaper Shaping Rate Configure Register" */
#define SWITCH_PAGE_47_PN_PORT_SHAPER_PACKET_BASED_MAX_REFRESH_PORT4 0x0b0047a0 /* "Port N (0 to 6), Packet-Based, Port Shaper Shaping Rate Configure Register" */
#define SWITCH_PAGE_47_PN_PORT_SHAPER_PACKET_BASED_MAX_REFRESH_PORT5 0x0b0047a4 /* "Port N (0 to 6), Packet-Based, Port Shaper Shaping Rate Configure Register" */
#define SWITCH_PAGE_47_PN_PORT_SHAPER_PACKET_BASED_MAX_REFRESH_PORT6 0x0b0047a8 /* "Port N (0 to 6), Packet-Based, Port Shaper Shaping Rate Configure Register" */
#define SWITCH_PAGE_47_P7_PORT_SHAPER_PACKET_BASED_MAX_REFRESH 0x0b0047ac /* "Port 7, Packet-Based, Port Shaper Shaping Rate Configure Register" */
#define SWITCH_PAGE_47_IMP_PORT_SHAPER_PACKET_BASED_MAX_REFRESH 0x0b0047b0 /* "Port 8, Packet-Based, Port Shaper Shaping Rate Configure Register" */
#define SWITCH_PAGE_47_EGRESS_SHAPER_CTLREG_REG_SPARE0 0x0b0047b8 /* Spare 0 Register (Not2Release) */
#define SWITCH_PAGE_47_EGRESS_SHAPER_CTLREG_REG_SPARE1 0x0b0047bc /* Spare 1 Register (Not2Release) */
#define SWITCH_PAGE_47_PN_PORT_SHAPER_PACKET_BASED_MAX_THD_SEL_PORT0 0x0b0047c0 /* "Port N (0 to 6), Packet-Based, Port Shaper Burst Size Configure Register" */
#define SWITCH_PAGE_47_PN_PORT_SHAPER_PACKET_BASED_MAX_THD_SEL_PORT1 0x0b0047c4 /* "Port N (0 to 6), Packet-Based, Port Shaper Burst Size Configure Register" */
#define SWITCH_PAGE_47_PN_PORT_SHAPER_PACKET_BASED_MAX_THD_SEL_PORT2 0x0b0047c8 /* "Port N (0 to 6), Packet-Based, Port Shaper Burst Size Configure Register" */
#define SWITCH_PAGE_47_PN_PORT_SHAPER_PACKET_BASED_MAX_THD_SEL_PORT3 0x0b0047cc /* "Port N (0 to 6), Packet-Based, Port Shaper Burst Size Configure Register" */
#define SWITCH_PAGE_47_PN_PORT_SHAPER_PACKET_BASED_MAX_THD_SEL_PORT4 0x0b0047d0 /* "Port N (0 to 6), Packet-Based, Port Shaper Burst Size Configure Register" */
#define SWITCH_PAGE_47_PN_PORT_SHAPER_PACKET_BASED_MAX_THD_SEL_PORT5 0x0b0047d4 /* "Port N (0 to 6), Packet-Based, Port Shaper Burst Size Configure Register" */
#define SWITCH_PAGE_47_PN_PORT_SHAPER_PACKET_BASED_MAX_THD_SEL_PORT6 0x0b0047d8 /* "Port N (0 to 6), Packet-Based, Port Shaper Burst Size Configure Register" */
#define SWITCH_PAGE_47_P7_PORT_SHAPER_PACKET_BASED_MAX_THD_SEL 0x0b0047dc /* "Port 7, Packet-Based, Port Shaper Burst Size Configure Register" */
#define SWITCH_PAGE_47_IMP_PORT_SHAPER_PACKET_BASED_MAX_THD_SEL 0x0b0047e0 /* "Port 8, Packet-Based, Port Shaper Burst Size Configure Register" */
#define SWITCH_PAGE_47_PORT_SHAPER_AVB_SHAPING_MODE 0x0b0047e4 /* Port Shaper AVB Shaping Mode Control Register */
#define SWITCH_PAGE_47_PORT_SHAPER_ENABLE 0x0b0047e6 /* Port Shaper Enable Register */
#define SWITCH_PAGE_47_PORT_SHAPER_BUCKET_COUNT_SELECT 0x0b0047e8 /* Port Shaper Bucket Count Select Register */
#define SWITCH_PAGE_47_PORT_SHAPER_BLOCKING 0x0b0047ea /* Port Shaper Blocking Control Register */
#define SWITCH_PAGE_47_IFG_BYTES       0x0b0047ee /* IFG Correction Control Register */
#define SWITCH_PAGE_48_PN_QUEUE0_MAX_REFRESH_PORT0 0x0b004800 /* "Port N (0 to 6), Byte-based Queue 0 Shaping Rate Configure Register" */
#define SWITCH_PAGE_48_PN_QUEUE0_MAX_REFRESH_PORT1 0x0b004804 /* "Port N (0 to 6), Byte-based Queue 0 Shaping Rate Configure Register" */
#define SWITCH_PAGE_48_PN_QUEUE0_MAX_REFRESH_PORT2 0x0b004808 /* "Port N (0 to 6), Byte-based Queue 0 Shaping Rate Configure Register" */
#define SWITCH_PAGE_48_PN_QUEUE0_MAX_REFRESH_PORT3 0x0b00480c /* "Port N (0 to 6), Byte-based Queue 0 Shaping Rate Configure Register" */
#define SWITCH_PAGE_48_PN_QUEUE0_MAX_REFRESH_PORT4 0x0b004810 /* "Port N (0 to 6), Byte-based Queue 0 Shaping Rate Configure Register" */
#define SWITCH_PAGE_48_PN_QUEUE0_MAX_REFRESH_PORT5 0x0b004814 /* "Port N (0 to 6), Byte-based Queue 0 Shaping Rate Configure Register" */
#define SWITCH_PAGE_48_PN_QUEUE0_MAX_REFRESH_PORT6 0x0b004818 /* "Port N (0 to 6), Byte-based Queue 0 Shaping Rate Configure Register" */
#define SWITCH_PAGE_48_P7_QUEUE0_MAX_REFRESH 0x0b00481c /* "Port 7, Byte-based Queue 0 Shaping Rate Configure Register" */
#define SWITCH_PAGE_48_IMP_QUEUE0_MAX_REFRESH 0x0b004820 /* "Port 8, Byte-based Queue 0 Shaping Rate Configure Register" */
#define SWITCH_PAGE_48_PN_QUEUE0_MAX_THD_SEL_PORT0 0x0b004830 /* "Port N (0 to 6), Byte-based Queue 0 Burst Size Configure Register" */
#define SWITCH_PAGE_48_PN_QUEUE0_MAX_THD_SEL_PORT1 0x0b004834 /* "Port N (0 to 6), Byte-based Queue 0 Burst Size Configure Register" */
#define SWITCH_PAGE_48_PN_QUEUE0_MAX_THD_SEL_PORT2 0x0b004838 /* "Port N (0 to 6), Byte-based Queue 0 Burst Size Configure Register" */
#define SWITCH_PAGE_48_PN_QUEUE0_MAX_THD_SEL_PORT3 0x0b00483c /* "Port N (0 to 6), Byte-based Queue 0 Burst Size Configure Register" */
#define SWITCH_PAGE_48_PN_QUEUE0_MAX_THD_SEL_PORT4 0x0b004840 /* "Port N (0 to 6), Byte-based Queue 0 Burst Size Configure Register" */
#define SWITCH_PAGE_48_PN_QUEUE0_MAX_THD_SEL_PORT5 0x0b004844 /* "Port N (0 to 6), Byte-based Queue 0 Burst Size Configure Register" */
#define SWITCH_PAGE_48_PN_QUEUE0_MAX_THD_SEL_PORT6 0x0b004848 /* "Port N (0 to 6), Byte-based Queue 0 Burst Size Configure Register" */
#define SWITCH_PAGE_48_P7_QUEUE0_MAX_THD_SEL 0x0b00484c /* "Port 7, Byte-based Queue 0 Burst Size Configure Register" */
#define SWITCH_PAGE_48_IMP_QUEUE0_MAX_THD_SEL 0x0b004850 /* "Port 8, Byte-based Queue 0 Burst Size Configure Register" */
#define SWITCH_PAGE_48_PN_QUEUE0_SHAPER_STS_PORT0 0x0b004860 /* "Port N (0 to 6), Queue 0 Shaper Status Register" */
#define SWITCH_PAGE_48_PN_QUEUE0_SHAPER_STS_PORT1 0x0b004864 /* "Port N (0 to 6), Queue 0 Shaper Status Register" */
#define SWITCH_PAGE_48_PN_QUEUE0_SHAPER_STS_PORT2 0x0b004868 /* "Port N (0 to 6), Queue 0 Shaper Status Register" */
#define SWITCH_PAGE_48_PN_QUEUE0_SHAPER_STS_PORT3 0x0b00486c /* "Port N (0 to 6), Queue 0 Shaper Status Register" */
#define SWITCH_PAGE_48_PN_QUEUE0_SHAPER_STS_PORT4 0x0b004870 /* "Port N (0 to 6), Queue 0 Shaper Status Register" */
#define SWITCH_PAGE_48_PN_QUEUE0_SHAPER_STS_PORT5 0x0b004874 /* "Port N (0 to 6), Queue 0 Shaper Status Register" */
#define SWITCH_PAGE_48_PN_QUEUE0_SHAPER_STS_PORT6 0x0b004878 /* "Port N (0 to 6), Queue 0 Shaper Status Register" */
#define SWITCH_PAGE_48_P7_QUEUE0_SHAPER_STS 0x0b00487c /* "Port 7, Queue 0 Shaper Status Register" */
#define SWITCH_PAGE_48_IMP_QUEUE0_SHAPER_STS 0x0b004880 /* "Port 8, Queue 0 Shaper Status Register" */
#define SWITCH_PAGE_48_PN_QUEUE0_MAX_PACKET_REFRESH_PORT0 0x0b004890 /* "Port N (0 to 6), Packet-based Queue 0 Shaping Rate Configure Register" */
#define SWITCH_PAGE_48_PN_QUEUE0_MAX_PACKET_REFRESH_PORT1 0x0b004894 /* "Port N (0 to 6), Packet-based Queue 0 Shaping Rate Configure Register" */
#define SWITCH_PAGE_48_PN_QUEUE0_MAX_PACKET_REFRESH_PORT2 0x0b004898 /* "Port N (0 to 6), Packet-based Queue 0 Shaping Rate Configure Register" */
#define SWITCH_PAGE_48_PN_QUEUE0_MAX_PACKET_REFRESH_PORT3 0x0b00489c /* "Port N (0 to 6), Packet-based Queue 0 Shaping Rate Configure Register" */
#define SWITCH_PAGE_48_PN_QUEUE0_MAX_PACKET_REFRESH_PORT4 0x0b0048a0 /* "Port N (0 to 6), Packet-based Queue 0 Shaping Rate Configure Register" */
#define SWITCH_PAGE_48_PN_QUEUE0_MAX_PACKET_REFRESH_PORT5 0x0b0048a4 /* "Port N (0 to 6), Packet-based Queue 0 Shaping Rate Configure Register" */
#define SWITCH_PAGE_48_PN_QUEUE0_MAX_PACKET_REFRESH_PORT6 0x0b0048a8 /* "Port N (0 to 6), Packet-based Queue 0 Shaping Rate Configure Register" */
#define SWITCH_PAGE_48_P7_QUEUE0_MAX_PACKET_REFRESH 0x0b0048ac /* "Port 7, Packet-based Queue 0 Shaping Rate Configure Register" */
#define SWITCH_PAGE_48_IMP_QUEUE0_MAX_PACKET_REFRESH 0x0b0048b0 /* "Port 8, Packet-based Queue 0 Shaping Rate Configure Register" */
#define SWITCH_PAGE_48_EGRESS_SHAPER_Q0_CONFIG_REG_SPARE0 0x0b0048b8 /* Spare 0 Register (Not2Release) */
#define SWITCH_PAGE_48_EGRESS_SHAPER_Q0_CONFIG_REG_SPARE1 0x0b0048bc /* Spare 1 Register (Not2Release) */
#define SWITCH_PAGE_48_PN_QUEUE0_MAX_PACKET_THD_SEL_PORT0 0x0b0048c0 /* "Port N (0 to 6), Packet-based Queue 0 Burst Size Configure Register" */
#define SWITCH_PAGE_48_PN_QUEUE0_MAX_PACKET_THD_SEL_PORT1 0x0b0048c4 /* "Port N (0 to 6), Packet-based Queue 0 Burst Size Configure Register" */
#define SWITCH_PAGE_48_PN_QUEUE0_MAX_PACKET_THD_SEL_PORT2 0x0b0048c8 /* "Port N (0 to 6), Packet-based Queue 0 Burst Size Configure Register" */
#define SWITCH_PAGE_48_PN_QUEUE0_MAX_PACKET_THD_SEL_PORT3 0x0b0048cc /* "Port N (0 to 6), Packet-based Queue 0 Burst Size Configure Register" */
#define SWITCH_PAGE_48_PN_QUEUE0_MAX_PACKET_THD_SEL_PORT4 0x0b0048d0 /* "Port N (0 to 6), Packet-based Queue 0 Burst Size Configure Register" */
#define SWITCH_PAGE_48_PN_QUEUE0_MAX_PACKET_THD_SEL_PORT5 0x0b0048d4 /* "Port N (0 to 6), Packet-based Queue 0 Burst Size Configure Register" */
#define SWITCH_PAGE_48_PN_QUEUE0_MAX_PACKET_THD_SEL_PORT6 0x0b0048d8 /* "Port N (0 to 6), Packet-based Queue 0 Burst Size Configure Register" */
#define SWITCH_PAGE_48_P7_QUEUE0_MAX_PACKET_THD_SEL 0x0b0048dc /* "Port 7, Packet-based Queue 0 Burst Size Configure Register" */
#define SWITCH_PAGE_48_IMP_QUEUE0_MAX_PACKET_THD_SEL 0x0b0048e0 /* "Port 8, Packet-based Queue 0 Burst Size Configure Register" */
#define SWITCH_PAGE_48_QUEUE0_AVB_SHAPING_MODE 0x0b0048e4 /* Queue 0 AVB Shaping Mode Control Register */
#define SWITCH_PAGE_48_QUEUE0_SHAPER_ENABLE 0x0b0048e6 /* Queue 0 Shaper Enable Register */
#define SWITCH_PAGE_48_QUEUE0_SHAPER_BUCKET_COUNT_SELECT 0x0b0048e8 /* Queue 0 Bucket Count Select Register */
#define SWITCH_PAGE_48_QUEUE0_SHAPER_BLOCKING 0x0b0048ea /* Queue 0 Shaper Blocking Control Register */
#define SWITCH_PAGE_49_PN_QUEUE1_MAX_REFRESH_PORT0 0x0b004900 /* "Port N (0 to 6), Byte-based Queue 1 Shaping Rate Configure Register" */
#define SWITCH_PAGE_49_PN_QUEUE1_MAX_REFRESH_PORT1 0x0b004904 /* "Port N (0 to 6), Byte-based Queue 1 Shaping Rate Configure Register" */
#define SWITCH_PAGE_49_PN_QUEUE1_MAX_REFRESH_PORT2 0x0b004908 /* "Port N (0 to 6), Byte-based Queue 1 Shaping Rate Configure Register" */
#define SWITCH_PAGE_49_PN_QUEUE1_MAX_REFRESH_PORT3 0x0b00490c /* "Port N (0 to 6), Byte-based Queue 1 Shaping Rate Configure Register" */
#define SWITCH_PAGE_49_PN_QUEUE1_MAX_REFRESH_PORT4 0x0b004910 /* "Port N (0 to 6), Byte-based Queue 1 Shaping Rate Configure Register" */
#define SWITCH_PAGE_49_PN_QUEUE1_MAX_REFRESH_PORT5 0x0b004914 /* "Port N (0 to 6), Byte-based Queue 1 Shaping Rate Configure Register" */
#define SWITCH_PAGE_49_PN_QUEUE1_MAX_REFRESH_PORT6 0x0b004918 /* "Port N (0 to 6), Byte-based Queue 1 Shaping Rate Configure Register" */
#define SWITCH_PAGE_49_P7_QUEUE1_MAX_REFRESH 0x0b00491c /* "Port 7, Byte-based Queue 1 Shaping Rate Configure Register" */
#define SWITCH_PAGE_49_IMP_QUEUE1_MAX_REFRESH 0x0b004920 /* "Port 8, Byte-based Queue 1 Shaping Rate Configure Register" */
#define SWITCH_PAGE_49_PN_QUEUE1_MAX_THD_SEL_PORT0 0x0b004930 /* "Port N (0 to 6), Byte-based Queue 1 Burst Size Configure Register" */
#define SWITCH_PAGE_49_PN_QUEUE1_MAX_THD_SEL_PORT1 0x0b004934 /* "Port N (0 to 6), Byte-based Queue 1 Burst Size Configure Register" */
#define SWITCH_PAGE_49_PN_QUEUE1_MAX_THD_SEL_PORT2 0x0b004938 /* "Port N (0 to 6), Byte-based Queue 1 Burst Size Configure Register" */
#define SWITCH_PAGE_49_PN_QUEUE1_MAX_THD_SEL_PORT3 0x0b00493c /* "Port N (0 to 6), Byte-based Queue 1 Burst Size Configure Register" */
#define SWITCH_PAGE_49_PN_QUEUE1_MAX_THD_SEL_PORT4 0x0b004940 /* "Port N (0 to 6), Byte-based Queue 1 Burst Size Configure Register" */
#define SWITCH_PAGE_49_PN_QUEUE1_MAX_THD_SEL_PORT5 0x0b004944 /* "Port N (0 to 6), Byte-based Queue 1 Burst Size Configure Register" */
#define SWITCH_PAGE_49_PN_QUEUE1_MAX_THD_SEL_PORT6 0x0b004948 /* "Port N (0 to 6), Byte-based Queue 1 Burst Size Configure Register" */
#define SWITCH_PAGE_49_P7_QUEUE1_MAX_THD_SEL 0x0b00494c /* "Port 7, Byte-based Queue 1 Burst Size Configure Register" */
#define SWITCH_PAGE_49_IMP_QUEUE1_MAX_THD_SEL 0x0b004950 /* "Port 8, Byte-based Queue 1 Burst Size Configure Register" */
#define SWITCH_PAGE_49_PN_QUEUE1_SHAPER_STS_PORT0 0x0b004960 /* "Port N (0 to 6), Queue 1 Shaper Status Register" */
#define SWITCH_PAGE_49_PN_QUEUE1_SHAPER_STS_PORT1 0x0b004964 /* "Port N (0 to 6), Queue 1 Shaper Status Register" */
#define SWITCH_PAGE_49_PN_QUEUE1_SHAPER_STS_PORT2 0x0b004968 /* "Port N (0 to 6), Queue 1 Shaper Status Register" */
#define SWITCH_PAGE_49_PN_QUEUE1_SHAPER_STS_PORT3 0x0b00496c /* "Port N (0 to 6), Queue 1 Shaper Status Register" */
#define SWITCH_PAGE_49_PN_QUEUE1_SHAPER_STS_PORT4 0x0b004970 /* "Port N (0 to 6), Queue 1 Shaper Status Register" */
#define SWITCH_PAGE_49_PN_QUEUE1_SHAPER_STS_PORT5 0x0b004974 /* "Port N (0 to 6), Queue 1 Shaper Status Register" */
#define SWITCH_PAGE_49_PN_QUEUE1_SHAPER_STS_PORT6 0x0b004978 /* "Port N (0 to 6), Queue 1 Shaper Status Register" */
#define SWITCH_PAGE_49_P7_QUEUE1_SHAPER_STS 0x0b00497c /* "Port 7, Queue 1 Shaper Status Register" */
#define SWITCH_PAGE_49_IMP_QUEUE1_SHAPER_STS 0x0b004980 /* "Port 8, Queue 1 Shaper Status Register" */
#define SWITCH_PAGE_49_PN_QUEUE1_MAX_PACKET_REFRESH_PORT0 0x0b004990 /* "Port N (0 to 6), Packet-based Queue 1 Shaping Rate Configure Register" */
#define SWITCH_PAGE_49_PN_QUEUE1_MAX_PACKET_REFRESH_PORT1 0x0b004994 /* "Port N (0 to 6), Packet-based Queue 1 Shaping Rate Configure Register" */
#define SWITCH_PAGE_49_PN_QUEUE1_MAX_PACKET_REFRESH_PORT2 0x0b004998 /* "Port N (0 to 6), Packet-based Queue 1 Shaping Rate Configure Register" */
#define SWITCH_PAGE_49_PN_QUEUE1_MAX_PACKET_REFRESH_PORT3 0x0b00499c /* "Port N (0 to 6), Packet-based Queue 1 Shaping Rate Configure Register" */
#define SWITCH_PAGE_49_PN_QUEUE1_MAX_PACKET_REFRESH_PORT4 0x0b0049a0 /* "Port N (0 to 6), Packet-based Queue 1 Shaping Rate Configure Register" */
#define SWITCH_PAGE_49_PN_QUEUE1_MAX_PACKET_REFRESH_PORT5 0x0b0049a4 /* "Port N (0 to 6), Packet-based Queue 1 Shaping Rate Configure Register" */
#define SWITCH_PAGE_49_PN_QUEUE1_MAX_PACKET_REFRESH_PORT6 0x0b0049a8 /* "Port N (0 to 6), Packet-based Queue 1 Shaping Rate Configure Register" */
#define SWITCH_PAGE_49_P7_QUEUE1_MAX_PACKET_REFRESH 0x0b0049ac /* "Port 7, Packet-based Queue 1 Shaping Rate Configure Register" */
#define SWITCH_PAGE_49_IMP_QUEUE1_MAX_PACKET_REFRESH 0x0b0049b0 /* "Port 8, Packet-based Queue 1 Shaping Rate Configure Register" */
#define SWITCH_PAGE_49_EGRESS_SHAPER_Q1_CONFIG_REG_SPARE0 0x0b0049b8 /* Spare 0 Register (Not2Release) */
#define SWITCH_PAGE_49_EGRESS_SHAPER_Q1_CONFIG_REG_SPARE1 0x0b0049bc /* Spare 1 Register (Not2Release) */
#define SWITCH_PAGE_49_PN_QUEUE1_MAX_PACKET_THD_SEL_PORT0 0x0b0049c0 /* "Port N (0 to 6), Packet-based Queue 1 Burst Size Configure Register" */
#define SWITCH_PAGE_49_PN_QUEUE1_MAX_PACKET_THD_SEL_PORT1 0x0b0049c4 /* "Port N (0 to 6), Packet-based Queue 1 Burst Size Configure Register" */
#define SWITCH_PAGE_49_PN_QUEUE1_MAX_PACKET_THD_SEL_PORT2 0x0b0049c8 /* "Port N (0 to 6), Packet-based Queue 1 Burst Size Configure Register" */
#define SWITCH_PAGE_49_PN_QUEUE1_MAX_PACKET_THD_SEL_PORT3 0x0b0049cc /* "Port N (0 to 6), Packet-based Queue 1 Burst Size Configure Register" */
#define SWITCH_PAGE_49_PN_QUEUE1_MAX_PACKET_THD_SEL_PORT4 0x0b0049d0 /* "Port N (0 to 6), Packet-based Queue 1 Burst Size Configure Register" */
#define SWITCH_PAGE_49_PN_QUEUE1_MAX_PACKET_THD_SEL_PORT5 0x0b0049d4 /* "Port N (0 to 6), Packet-based Queue 1 Burst Size Configure Register" */
#define SWITCH_PAGE_49_PN_QUEUE1_MAX_PACKET_THD_SEL_PORT6 0x0b0049d8 /* "Port N (0 to 6), Packet-based Queue 1 Burst Size Configure Register" */
#define SWITCH_PAGE_49_P7_QUEUE1_MAX_PACKET_THD_SEL 0x0b0049dc /* "Port 7, Packet-based Queue 1 Burst Size Configure Register" */
#define SWITCH_PAGE_49_IMP_QUEUE1_MAX_PACKET_THD_SEL 0x0b0049e0 /* "Port 8, Packet-based Queue 1 Burst Size Configure Register" */
#define SWITCH_PAGE_49_QUEUE1_AVB_SHAPING_MODE 0x0b0049e4 /* Queue 1 AVB Shaping Mode Control Register */
#define SWITCH_PAGE_49_QUEUE1_SHAPER_ENABLE 0x0b0049e6 /* Queue 1 Shaper Enable Register */
#define SWITCH_PAGE_49_QUEUE1_SHAPER_BUCKET_COUNT_SELECT 0x0b0049e8 /* Queue 1 Bucket Count Select Register */
#define SWITCH_PAGE_49_QUEUE1_SHAPER_BLOCKING 0x0b0049ea /* Queue 1 Shaper Blocking Control Register */
#define SWITCH_PAGE_4A_PN_QUEUE2_MAX_REFRESH_PORT0 0x0b004a00 /* "Port N (0 to 6), Byte-based Queue 2 Shaping Rate Configure Register" */
#define SWITCH_PAGE_4A_PN_QUEUE2_MAX_REFRESH_PORT1 0x0b004a04 /* "Port N (0 to 6), Byte-based Queue 2 Shaping Rate Configure Register" */
#define SWITCH_PAGE_4A_PN_QUEUE2_MAX_REFRESH_PORT2 0x0b004a08 /* "Port N (0 to 6), Byte-based Queue 2 Shaping Rate Configure Register" */
#define SWITCH_PAGE_4A_PN_QUEUE2_MAX_REFRESH_PORT3 0x0b004a0c /* "Port N (0 to 6), Byte-based Queue 2 Shaping Rate Configure Register" */
#define SWITCH_PAGE_4A_PN_QUEUE2_MAX_REFRESH_PORT4 0x0b004a10 /* "Port N (0 to 6), Byte-based Queue 2 Shaping Rate Configure Register" */
#define SWITCH_PAGE_4A_PN_QUEUE2_MAX_REFRESH_PORT5 0x0b004a14 /* "Port N (0 to 6), Byte-based Queue 2 Shaping Rate Configure Register" */
#define SWITCH_PAGE_4A_PN_QUEUE2_MAX_REFRESH_PORT6 0x0b004a18 /* "Port N (0 to 6), Byte-based Queue 2 Shaping Rate Configure Register" */
#define SWITCH_PAGE_4A_P7_QUEUE2_MAX_REFRESH 0x0b004a1c /* "Port 7, Byte-based Queue 2 Shaping Rate Configure Register" */
#define SWITCH_PAGE_4A_IMP_QUEUE2_MAX_REFRESH 0x0b004a20 /* "Port 8, Byte-based Queue 2 Shaping Rate Configure Register" */
#define SWITCH_PAGE_4A_PN_QUEUE2_MAX_THD_SEL_PORT0 0x0b004a30 /* "Port N (0 to 6), Byte-based Queue 2 Burst Size Configure Register" */
#define SWITCH_PAGE_4A_PN_QUEUE2_MAX_THD_SEL_PORT1 0x0b004a34 /* "Port N (0 to 6), Byte-based Queue 2 Burst Size Configure Register" */
#define SWITCH_PAGE_4A_PN_QUEUE2_MAX_THD_SEL_PORT2 0x0b004a38 /* "Port N (0 to 6), Byte-based Queue 2 Burst Size Configure Register" */
#define SWITCH_PAGE_4A_PN_QUEUE2_MAX_THD_SEL_PORT3 0x0b004a3c /* "Port N (0 to 6), Byte-based Queue 2 Burst Size Configure Register" */
#define SWITCH_PAGE_4A_PN_QUEUE2_MAX_THD_SEL_PORT4 0x0b004a40 /* "Port N (0 to 6), Byte-based Queue 2 Burst Size Configure Register" */
#define SWITCH_PAGE_4A_PN_QUEUE2_MAX_THD_SEL_PORT5 0x0b004a44 /* "Port N (0 to 6), Byte-based Queue 2 Burst Size Configure Register" */
#define SWITCH_PAGE_4A_PN_QUEUE2_MAX_THD_SEL_PORT6 0x0b004a48 /* "Port N (0 to 6), Byte-based Queue 2 Burst Size Configure Register" */
#define SWITCH_PAGE_4A_P7_QUEUE2_MAX_THD_SEL 0x0b004a4c /* "Port 7, Byte-based Queue 2 Burst Size Configure Register" */
#define SWITCH_PAGE_4A_IMP_QUEUE2_MAX_THD_SEL 0x0b004a50 /* "Port 8, Byte-based Queue 2 Burst Size Configure Register" */
#define SWITCH_PAGE_4A_PN_QUEUE2_SHAPER_STS_PORT0 0x0b004a60 /* "Port N (0 to 6), Queue 2 Shaper Status Register" */
#define SWITCH_PAGE_4A_PN_QUEUE2_SHAPER_STS_PORT1 0x0b004a64 /* "Port N (0 to 6), Queue 2 Shaper Status Register" */
#define SWITCH_PAGE_4A_PN_QUEUE2_SHAPER_STS_PORT2 0x0b004a68 /* "Port N (0 to 6), Queue 2 Shaper Status Register" */
#define SWITCH_PAGE_4A_PN_QUEUE2_SHAPER_STS_PORT3 0x0b004a6c /* "Port N (0 to 6), Queue 2 Shaper Status Register" */
#define SWITCH_PAGE_4A_PN_QUEUE2_SHAPER_STS_PORT4 0x0b004a70 /* "Port N (0 to 6), Queue 2 Shaper Status Register" */
#define SWITCH_PAGE_4A_PN_QUEUE2_SHAPER_STS_PORT5 0x0b004a74 /* "Port N (0 to 6), Queue 2 Shaper Status Register" */
#define SWITCH_PAGE_4A_PN_QUEUE2_SHAPER_STS_PORT6 0x0b004a78 /* "Port N (0 to 6), Queue 2 Shaper Status Register" */
#define SWITCH_PAGE_4A_P7_QUEUE2_SHAPER_STS 0x0b004a7c /* "Port 7, Queue 2 Shaper Status Register" */
#define SWITCH_PAGE_4A_IMP_QUEUE2_SHAPER_STS 0x0b004a80 /* "Port 8, Queue 2 Shaper Status Register" */
#define SWITCH_PAGE_4A_PN_QUEUE2_MAX_PACKET_REFRESH_PORT0 0x0b004a90 /* "Port N (0 to 6), Packet-based Queue 2 Shaping Rate Configure Register" */
#define SWITCH_PAGE_4A_PN_QUEUE2_MAX_PACKET_REFRESH_PORT1 0x0b004a94 /* "Port N (0 to 6), Packet-based Queue 2 Shaping Rate Configure Register" */
#define SWITCH_PAGE_4A_PN_QUEUE2_MAX_PACKET_REFRESH_PORT2 0x0b004a98 /* "Port N (0 to 6), Packet-based Queue 2 Shaping Rate Configure Register" */
#define SWITCH_PAGE_4A_PN_QUEUE2_MAX_PACKET_REFRESH_PORT3 0x0b004a9c /* "Port N (0 to 6), Packet-based Queue 2 Shaping Rate Configure Register" */
#define SWITCH_PAGE_4A_PN_QUEUE2_MAX_PACKET_REFRESH_PORT4 0x0b004aa0 /* "Port N (0 to 6), Packet-based Queue 2 Shaping Rate Configure Register" */
#define SWITCH_PAGE_4A_PN_QUEUE2_MAX_PACKET_REFRESH_PORT5 0x0b004aa4 /* "Port N (0 to 6), Packet-based Queue 2 Shaping Rate Configure Register" */
#define SWITCH_PAGE_4A_PN_QUEUE2_MAX_PACKET_REFRESH_PORT6 0x0b004aa8 /* "Port N (0 to 6), Packet-based Queue 2 Shaping Rate Configure Register" */
#define SWITCH_PAGE_4A_P7_QUEUE2_MAX_PACKET_REFRESH 0x0b004aac /* "Port 7, Packet-based Queue 2 Shaping Rate Configure Register" */
#define SWITCH_PAGE_4A_IMP_QUEUE2_MAX_PACKET_REFRESH 0x0b004ab0 /* "Port 8, Packet-based Queue 2 Shaping Rate Configure Register" */
#define SWITCH_PAGE_4A_EGRESS_SHAPER_Q2_CONFIG_REG_SPARE0 0x0b004ab8 /* Spare 0 Register (Not2Release) */
#define SWITCH_PAGE_4A_EGRESS_SHAPER_Q2_CONFIG_REG_SPARE1 0x0b004abc /* Spare 1 Register (Not2Release) */
#define SWITCH_PAGE_4A_PN_QUEUE2_MAX_PACKET_THD_SEL_PORT0 0x0b004ac0 /* "Port N (0 to 6), Packet-based Queue 2 Burst Size Configure Register" */
#define SWITCH_PAGE_4A_PN_QUEUE2_MAX_PACKET_THD_SEL_PORT1 0x0b004ac4 /* "Port N (0 to 6), Packet-based Queue 2 Burst Size Configure Register" */
#define SWITCH_PAGE_4A_PN_QUEUE2_MAX_PACKET_THD_SEL_PORT2 0x0b004ac8 /* "Port N (0 to 6), Packet-based Queue 2 Burst Size Configure Register" */
#define SWITCH_PAGE_4A_PN_QUEUE2_MAX_PACKET_THD_SEL_PORT3 0x0b004acc /* "Port N (0 to 6), Packet-based Queue 2 Burst Size Configure Register" */
#define SWITCH_PAGE_4A_PN_QUEUE2_MAX_PACKET_THD_SEL_PORT4 0x0b004ad0 /* "Port N (0 to 6), Packet-based Queue 2 Burst Size Configure Register" */
#define SWITCH_PAGE_4A_PN_QUEUE2_MAX_PACKET_THD_SEL_PORT5 0x0b004ad4 /* "Port N (0 to 6), Packet-based Queue 2 Burst Size Configure Register" */
#define SWITCH_PAGE_4A_PN_QUEUE2_MAX_PACKET_THD_SEL_PORT6 0x0b004ad8 /* "Port N (0 to 6), Packet-based Queue 2 Burst Size Configure Register" */
#define SWITCH_PAGE_4A_P7_QUEUE2_MAX_PACKET_THD_SEL 0x0b004adc /* "Port 7, Packet-based Queue 2 Burst Size Configure Register" */
#define SWITCH_PAGE_4A_IMP_QUEUE2_MAX_PACKET_THD_SEL 0x0b004ae0 /* "Port 8, Packet-based Queue 2 Burst Size Configure Register" */
#define SWITCH_PAGE_4A_QUEUE2_AVB_SHAPING_MODE 0x0b004ae4 /* Queue 2 AVB Shaping Mode Control Register */
#define SWITCH_PAGE_4A_QUEUE2_SHAPER_ENABLE 0x0b004ae6 /* Queue 2 Shaper Enable Register */
#define SWITCH_PAGE_4A_QUEUE2_SHAPER_BUCKET_COUNT_SELECT 0x0b004ae8 /* Queue 2 Bucket Count Select Register */
#define SWITCH_PAGE_4A_QUEUE2_SHAPER_BLOCKING 0x0b004aea /* Queue 2 Shaper Blocking Control Register */
#define SWITCH_PAGE_4B_PN_QUEUE3_MAX_REFRESH_PORT0 0x0b004b00 /* "Port N (0 to 6), Byte-based Queue 3 Shaping Rate Configure Register" */
#define SWITCH_PAGE_4B_PN_QUEUE3_MAX_REFRESH_PORT1 0x0b004b04 /* "Port N (0 to 6), Byte-based Queue 3 Shaping Rate Configure Register" */
#define SWITCH_PAGE_4B_PN_QUEUE3_MAX_REFRESH_PORT2 0x0b004b08 /* "Port N (0 to 6), Byte-based Queue 3 Shaping Rate Configure Register" */
#define SWITCH_PAGE_4B_PN_QUEUE3_MAX_REFRESH_PORT3 0x0b004b0c /* "Port N (0 to 6), Byte-based Queue 3 Shaping Rate Configure Register" */
#define SWITCH_PAGE_4B_PN_QUEUE3_MAX_REFRESH_PORT4 0x0b004b10 /* "Port N (0 to 6), Byte-based Queue 3 Shaping Rate Configure Register" */
#define SWITCH_PAGE_4B_PN_QUEUE3_MAX_REFRESH_PORT5 0x0b004b14 /* "Port N (0 to 6), Byte-based Queue 3 Shaping Rate Configure Register" */
#define SWITCH_PAGE_4B_PN_QUEUE3_MAX_REFRESH_PORT6 0x0b004b18 /* "Port N (0 to 6), Byte-based Queue 3 Shaping Rate Configure Register" */
#define SWITCH_PAGE_4B_P7_QUEUE3_MAX_REFRESH 0x0b004b1c /* "Port 7, Byte-based Queue 3 Shaping Rate Configure Register" */
#define SWITCH_PAGE_4B_IMP_QUEUE3_MAX_REFRESH 0x0b004b20 /* "Port 8, Byte-based Queue 3 Shaping Rate Configure Register" */
#define SWITCH_PAGE_4B_PN_QUEUE3_MAX_THD_SEL_PORT0 0x0b004b30 /* "Port N (0 to 6), Byte-based Queue 3 Burst Size Configure Register" */
#define SWITCH_PAGE_4B_PN_QUEUE3_MAX_THD_SEL_PORT1 0x0b004b34 /* "Port N (0 to 6), Byte-based Queue 3 Burst Size Configure Register" */
#define SWITCH_PAGE_4B_PN_QUEUE3_MAX_THD_SEL_PORT2 0x0b004b38 /* "Port N (0 to 6), Byte-based Queue 3 Burst Size Configure Register" */
#define SWITCH_PAGE_4B_PN_QUEUE3_MAX_THD_SEL_PORT3 0x0b004b3c /* "Port N (0 to 6), Byte-based Queue 3 Burst Size Configure Register" */
#define SWITCH_PAGE_4B_PN_QUEUE3_MAX_THD_SEL_PORT4 0x0b004b40 /* "Port N (0 to 6), Byte-based Queue 3 Burst Size Configure Register" */
#define SWITCH_PAGE_4B_PN_QUEUE3_MAX_THD_SEL_PORT5 0x0b004b44 /* "Port N (0 to 6), Byte-based Queue 3 Burst Size Configure Register" */
#define SWITCH_PAGE_4B_PN_QUEUE3_MAX_THD_SEL_PORT6 0x0b004b48 /* "Port N (0 to 6), Byte-based Queue 3 Burst Size Configure Register" */
#define SWITCH_PAGE_4B_P7_QUEUE3_MAX_THD_SEL 0x0b004b4c /* "Port 7, Byte-based Queue 3 Burst Size Configure Register" */
#define SWITCH_PAGE_4B_IMP_QUEUE3_MAX_THD_SEL 0x0b004b50 /* "Port 8, Byte-based Queue 3 Burst Size Configure Register" */
#define SWITCH_PAGE_4B_PN_QUEUE3_SHAPER_STS_PORT0 0x0b004b60 /* "Port N (0 to 6), Queue 3 Shaper Status Register" */
#define SWITCH_PAGE_4B_PN_QUEUE3_SHAPER_STS_PORT1 0x0b004b64 /* "Port N (0 to 6), Queue 3 Shaper Status Register" */
#define SWITCH_PAGE_4B_PN_QUEUE3_SHAPER_STS_PORT2 0x0b004b68 /* "Port N (0 to 6), Queue 3 Shaper Status Register" */
#define SWITCH_PAGE_4B_PN_QUEUE3_SHAPER_STS_PORT3 0x0b004b6c /* "Port N (0 to 6), Queue 3 Shaper Status Register" */
#define SWITCH_PAGE_4B_PN_QUEUE3_SHAPER_STS_PORT4 0x0b004b70 /* "Port N (0 to 6), Queue 3 Shaper Status Register" */
#define SWITCH_PAGE_4B_PN_QUEUE3_SHAPER_STS_PORT5 0x0b004b74 /* "Port N (0 to 6), Queue 3 Shaper Status Register" */
#define SWITCH_PAGE_4B_PN_QUEUE3_SHAPER_STS_PORT6 0x0b004b78 /* "Port N (0 to 6), Queue 3 Shaper Status Register" */
#define SWITCH_PAGE_4B_P7_QUEUE3_SHAPER_STS 0x0b004b7c /* "Port 7, Queue 3 Shaper Status Register" */
#define SWITCH_PAGE_4B_IMP_QUEUE3_SHAPER_STS 0x0b004b80 /* "Port 8, Queue 3 Shaper Status Register" */
#define SWITCH_PAGE_4B_PN_QUEUE3_MAX_PACKET_REFRESH_PORT0 0x0b004b90 /* "Port N (0 to 6), Packet-based Queue 3 Shaping Rate Configure Register" */
#define SWITCH_PAGE_4B_PN_QUEUE3_MAX_PACKET_REFRESH_PORT1 0x0b004b94 /* "Port N (0 to 6), Packet-based Queue 3 Shaping Rate Configure Register" */
#define SWITCH_PAGE_4B_PN_QUEUE3_MAX_PACKET_REFRESH_PORT2 0x0b004b98 /* "Port N (0 to 6), Packet-based Queue 3 Shaping Rate Configure Register" */
#define SWITCH_PAGE_4B_PN_QUEUE3_MAX_PACKET_REFRESH_PORT3 0x0b004b9c /* "Port N (0 to 6), Packet-based Queue 3 Shaping Rate Configure Register" */
#define SWITCH_PAGE_4B_PN_QUEUE3_MAX_PACKET_REFRESH_PORT4 0x0b004ba0 /* "Port N (0 to 6), Packet-based Queue 3 Shaping Rate Configure Register" */
#define SWITCH_PAGE_4B_PN_QUEUE3_MAX_PACKET_REFRESH_PORT5 0x0b004ba4 /* "Port N (0 to 6), Packet-based Queue 3 Shaping Rate Configure Register" */
#define SWITCH_PAGE_4B_PN_QUEUE3_MAX_PACKET_REFRESH_PORT6 0x0b004ba8 /* "Port N (0 to 6), Packet-based Queue 3 Shaping Rate Configure Register" */
#define SWITCH_PAGE_4B_P7_QUEUE3_MAX_PACKET_REFRESH 0x0b004bac /* "Port 7, Packet-based Queue 3 Shaping Rate Configure Register" */
#define SWITCH_PAGE_4B_IMP_QUEUE3_MAX_PACKET_REFRESH 0x0b004bb0 /* "Port 8, Packet-based Queue 3 Shaping Rate Configure Register" */
#define SWITCH_PAGE_4B_EGRESS_SHAPER_Q3_CONFIG_REG_SPARE0 0x0b004bb8 /* Spare 0 Register (Not2Release) */
#define SWITCH_PAGE_4B_EGRESS_SHAPER_Q3_CONFIG_REG_SPARE1 0x0b004bbc /* Spare 1 Register (Not2Release) */
#define SWITCH_PAGE_4B_PN_QUEUE3_MAX_PACKET_THD_SEL_PORT0 0x0b004bc0 /* "Port N (0 to 6), Packet-based Queue 3 Burst Size Configure Register" */
#define SWITCH_PAGE_4B_PN_QUEUE3_MAX_PACKET_THD_SEL_PORT1 0x0b004bc4 /* "Port N (0 to 6), Packet-based Queue 3 Burst Size Configure Register" */
#define SWITCH_PAGE_4B_PN_QUEUE3_MAX_PACKET_THD_SEL_PORT2 0x0b004bc8 /* "Port N (0 to 6), Packet-based Queue 3 Burst Size Configure Register" */
#define SWITCH_PAGE_4B_PN_QUEUE3_MAX_PACKET_THD_SEL_PORT3 0x0b004bcc /* "Port N (0 to 6), Packet-based Queue 3 Burst Size Configure Register" */
#define SWITCH_PAGE_4B_PN_QUEUE3_MAX_PACKET_THD_SEL_PORT4 0x0b004bd0 /* "Port N (0 to 6), Packet-based Queue 3 Burst Size Configure Register" */
#define SWITCH_PAGE_4B_PN_QUEUE3_MAX_PACKET_THD_SEL_PORT5 0x0b004bd4 /* "Port N (0 to 6), Packet-based Queue 3 Burst Size Configure Register" */
#define SWITCH_PAGE_4B_PN_QUEUE3_MAX_PACKET_THD_SEL_PORT6 0x0b004bd8 /* "Port N (0 to 6), Packet-based Queue 3 Burst Size Configure Register" */
#define SWITCH_PAGE_4B_P7_QUEUE3_MAX_PACKET_THD_SEL 0x0b004bdc /* "Port 7, Packet-based Queue 3 Burst Size Configure Register" */
#define SWITCH_PAGE_4B_IMP_QUEUE3_MAX_PACKET_THD_SEL 0x0b004be0 /* "Port 8, Packet-based Queue 3 Burst Size Configure Register" */
#define SWITCH_PAGE_4B_QUEUE3_AVB_SHAPING_MODE 0x0b004be4 /* Queue 3 AVB Shaping Mode Control Register */
#define SWITCH_PAGE_4B_QUEUE3_SHAPER_ENABLE 0x0b004be6 /* Queue 3 Shaper Enable Register */
#define SWITCH_PAGE_4B_QUEUE3_SHAPER_BUCKET_COUNT_SELECT 0x0b004be8 /* Queue 3 Bucket Count Select Register */
#define SWITCH_PAGE_4B_QUEUE3_SHAPER_BLOCKING 0x0b004bea /* Queue 3 Shaper Blocking Control Register */
#define SWITCH_PAGE_4C_PN_QUEUE4_MAX_REFRESH_PORT0 0x0b004c00 /* "Port N (0 to 6), Byte-based Queue 4 Shaping Rate Configure Register" */
#define SWITCH_PAGE_4C_PN_QUEUE4_MAX_REFRESH_PORT1 0x0b004c04 /* "Port N (0 to 6), Byte-based Queue 4 Shaping Rate Configure Register" */
#define SWITCH_PAGE_4C_PN_QUEUE4_MAX_REFRESH_PORT2 0x0b004c08 /* "Port N (0 to 6), Byte-based Queue 4 Shaping Rate Configure Register" */
#define SWITCH_PAGE_4C_PN_QUEUE4_MAX_REFRESH_PORT3 0x0b004c0c /* "Port N (0 to 6), Byte-based Queue 4 Shaping Rate Configure Register" */
#define SWITCH_PAGE_4C_PN_QUEUE4_MAX_REFRESH_PORT4 0x0b004c10 /* "Port N (0 to 6), Byte-based Queue 4 Shaping Rate Configure Register" */
#define SWITCH_PAGE_4C_PN_QUEUE4_MAX_REFRESH_PORT5 0x0b004c14 /* "Port N (0 to 6), Byte-based Queue 4 Shaping Rate Configure Register" */
#define SWITCH_PAGE_4C_PN_QUEUE4_MAX_REFRESH_PORT6 0x0b004c18 /* "Port N (0 to 6), Byte-based Queue 4 Shaping Rate Configure Register" */
#define SWITCH_PAGE_4C_P7_QUEUE4_MAX_REFRESH 0x0b004c1c /* "Port 7, Byte-based Queue 4 Shaping Rate Configure Register" */
#define SWITCH_PAGE_4C_IMP_QUEUE4_MAX_REFRESH 0x0b004c20 /* "Port 8, Byte-based Queue 4 Shaping Rate Configure Register" */
#define SWITCH_PAGE_4C_PN_QUEUE4_MAX_THD_SEL_PORT0 0x0b004c30 /* "Port N (0 to 6), Byte-based Queue 4 Burst Size Configure Register" */
#define SWITCH_PAGE_4C_PN_QUEUE4_MAX_THD_SEL_PORT1 0x0b004c34 /* "Port N (0 to 6), Byte-based Queue 4 Burst Size Configure Register" */
#define SWITCH_PAGE_4C_PN_QUEUE4_MAX_THD_SEL_PORT2 0x0b004c38 /* "Port N (0 to 6), Byte-based Queue 4 Burst Size Configure Register" */
#define SWITCH_PAGE_4C_PN_QUEUE4_MAX_THD_SEL_PORT3 0x0b004c3c /* "Port N (0 to 6), Byte-based Queue 4 Burst Size Configure Register" */
#define SWITCH_PAGE_4C_PN_QUEUE4_MAX_THD_SEL_PORT4 0x0b004c40 /* "Port N (0 to 6), Byte-based Queue 4 Burst Size Configure Register" */
#define SWITCH_PAGE_4C_PN_QUEUE4_MAX_THD_SEL_PORT5 0x0b004c44 /* "Port N (0 to 6), Byte-based Queue 4 Burst Size Configure Register" */
#define SWITCH_PAGE_4C_PN_QUEUE4_MAX_THD_SEL_PORT6 0x0b004c48 /* "Port N (0 to 6), Byte-based Queue 4 Burst Size Configure Register" */
#define SWITCH_PAGE_4C_P7_QUEUE4_MAX_THD_SEL 0x0b004c4c /* "Port 7, Byte-based Queue 4 Burst Size Configure Register" */
#define SWITCH_PAGE_4C_IMP_QUEUE4_MAX_THD_SEL 0x0b004c50 /* "Port 8, Byte-based Queue 4 Burst Size Configure Register" */
#define SWITCH_PAGE_4C_PN_QUEUE4_SHAPER_STS_PORT0 0x0b004c60 /* "Port N (0 to 6), Queue 4 Shaper Status Register" */
#define SWITCH_PAGE_4C_PN_QUEUE4_SHAPER_STS_PORT1 0x0b004c64 /* "Port N (0 to 6), Queue 4 Shaper Status Register" */
#define SWITCH_PAGE_4C_PN_QUEUE4_SHAPER_STS_PORT2 0x0b004c68 /* "Port N (0 to 6), Queue 4 Shaper Status Register" */
#define SWITCH_PAGE_4C_PN_QUEUE4_SHAPER_STS_PORT3 0x0b004c6c /* "Port N (0 to 6), Queue 4 Shaper Status Register" */
#define SWITCH_PAGE_4C_PN_QUEUE4_SHAPER_STS_PORT4 0x0b004c70 /* "Port N (0 to 6), Queue 4 Shaper Status Register" */
#define SWITCH_PAGE_4C_PN_QUEUE4_SHAPER_STS_PORT5 0x0b004c74 /* "Port N (0 to 6), Queue 4 Shaper Status Register" */
#define SWITCH_PAGE_4C_PN_QUEUE4_SHAPER_STS_PORT6 0x0b004c78 /* "Port N (0 to 6), Queue 4 Shaper Status Register" */
#define SWITCH_PAGE_4C_P7_QUEUE4_SHAPER_STS 0x0b004c7c /* "Port 7, Queue 4 Shaper Status Register" */
#define SWITCH_PAGE_4C_IMP_QUEUE4_SHAPER_STS 0x0b004c80 /* "Port 8, Queue 4 Shaper Status Register" */
#define SWITCH_PAGE_4C_PN_QUEUE4_MAX_PACKET_REFRESH_PORT0 0x0b004c90 /* "Port N (0 to 6), Packet-based Queue 4 Shaping Rate Configure Register" */
#define SWITCH_PAGE_4C_PN_QUEUE4_MAX_PACKET_REFRESH_PORT1 0x0b004c94 /* "Port N (0 to 6), Packet-based Queue 4 Shaping Rate Configure Register" */
#define SWITCH_PAGE_4C_PN_QUEUE4_MAX_PACKET_REFRESH_PORT2 0x0b004c98 /* "Port N (0 to 6), Packet-based Queue 4 Shaping Rate Configure Register" */
#define SWITCH_PAGE_4C_PN_QUEUE4_MAX_PACKET_REFRESH_PORT3 0x0b004c9c /* "Port N (0 to 6), Packet-based Queue 4 Shaping Rate Configure Register" */
#define SWITCH_PAGE_4C_PN_QUEUE4_MAX_PACKET_REFRESH_PORT4 0x0b004ca0 /* "Port N (0 to 6), Packet-based Queue 4 Shaping Rate Configure Register" */
#define SWITCH_PAGE_4C_PN_QUEUE4_MAX_PACKET_REFRESH_PORT5 0x0b004ca4 /* "Port N (0 to 6), Packet-based Queue 4 Shaping Rate Configure Register" */
#define SWITCH_PAGE_4C_PN_QUEUE4_MAX_PACKET_REFRESH_PORT6 0x0b004ca8 /* "Port N (0 to 6), Packet-based Queue 4 Shaping Rate Configure Register" */
#define SWITCH_PAGE_4C_P7_QUEUE4_MAX_PACKET_REFRESH 0x0b004cac /* "Port 7, Packet-based Queue 4 Shaping Rate Configure Register" */
#define SWITCH_PAGE_4C_IMP_QUEUE4_MAX_PACKET_REFRESH 0x0b004cb0 /* "Port 8, Packet-based Queue 4 Shaping Rate Configure Register" */
#define SWITCH_PAGE_4C_EGRESS_SHAPER_Q4_CONFIG_REG_SPARE0 0x0b004cb8 /* Spare 0 Register (Not2Release) */
#define SWITCH_PAGE_4C_EGRESS_SHAPER_Q4_CONFIG_REG_SPARE1 0x0b004cbc /* Spare 1 Register (Not2Release) */
#define SWITCH_PAGE_4C_PN_QUEUE4_MAX_PACKET_THD_SEL_PORT0 0x0b004cc0 /* "Port N (0 to 6), Packet-based Queue 4 Burst Size Configure Register" */
#define SWITCH_PAGE_4C_PN_QUEUE4_MAX_PACKET_THD_SEL_PORT1 0x0b004cc4 /* "Port N (0 to 6), Packet-based Queue 4 Burst Size Configure Register" */
#define SWITCH_PAGE_4C_PN_QUEUE4_MAX_PACKET_THD_SEL_PORT2 0x0b004cc8 /* "Port N (0 to 6), Packet-based Queue 4 Burst Size Configure Register" */
#define SWITCH_PAGE_4C_PN_QUEUE4_MAX_PACKET_THD_SEL_PORT3 0x0b004ccc /* "Port N (0 to 6), Packet-based Queue 4 Burst Size Configure Register" */
#define SWITCH_PAGE_4C_PN_QUEUE4_MAX_PACKET_THD_SEL_PORT4 0x0b004cd0 /* "Port N (0 to 6), Packet-based Queue 4 Burst Size Configure Register" */
#define SWITCH_PAGE_4C_PN_QUEUE4_MAX_PACKET_THD_SEL_PORT5 0x0b004cd4 /* "Port N (0 to 6), Packet-based Queue 4 Burst Size Configure Register" */
#define SWITCH_PAGE_4C_PN_QUEUE4_MAX_PACKET_THD_SEL_PORT6 0x0b004cd8 /* "Port N (0 to 6), Packet-based Queue 4 Burst Size Configure Register" */
#define SWITCH_PAGE_4C_P7_QUEUE4_MAX_PACKET_THD_SEL 0x0b004cdc /* "Port 7, Packet-based Queue 4 Burst Size Configure Register" */
#define SWITCH_PAGE_4C_IMP_QUEUE4_MAX_PACKET_THD_SEL 0x0b004ce0 /* "Port 8, Packet-based Queue 4 Burst Size Configure Register" */
#define SWITCH_PAGE_4C_QUEUE4_AVB_SHAPING_MODE 0x0b004ce4 /* Queue 4 AVB Shaping Mode Control Register */
#define SWITCH_PAGE_4C_QUEUE4_SHAPER_ENABLE 0x0b004ce6 /* Queue 4 Shaper Enable Register */
#define SWITCH_PAGE_4C_QUEUE4_SHAPER_BUCKET_COUNT_SELECT 0x0b004ce8 /* Queue 4 Bucket Count Select Register */
#define SWITCH_PAGE_4C_QUEUE4_SHAPER_BLOCKING 0x0b004cea /* Queue 4 Shaper Blocking Control Register */
#define SWITCH_PAGE_4D_PN_QUEUE5_MAX_REFRESH_PORT0 0x0b004d00 /* "Port N (0 to 6), Byte-based Queue 5 Shaping Rate Configure Register" */
#define SWITCH_PAGE_4D_PN_QUEUE5_MAX_REFRESH_PORT1 0x0b004d04 /* "Port N (0 to 6), Byte-based Queue 5 Shaping Rate Configure Register" */
#define SWITCH_PAGE_4D_PN_QUEUE5_MAX_REFRESH_PORT2 0x0b004d08 /* "Port N (0 to 6), Byte-based Queue 5 Shaping Rate Configure Register" */
#define SWITCH_PAGE_4D_PN_QUEUE5_MAX_REFRESH_PORT3 0x0b004d0c /* "Port N (0 to 6), Byte-based Queue 5 Shaping Rate Configure Register" */
#define SWITCH_PAGE_4D_PN_QUEUE5_MAX_REFRESH_PORT4 0x0b004d10 /* "Port N (0 to 6), Byte-based Queue 5 Shaping Rate Configure Register" */
#define SWITCH_PAGE_4D_PN_QUEUE5_MAX_REFRESH_PORT5 0x0b004d14 /* "Port N (0 to 6), Byte-based Queue 5 Shaping Rate Configure Register" */
#define SWITCH_PAGE_4D_PN_QUEUE5_MAX_REFRESH_PORT6 0x0b004d18 /* "Port N (0 to 6), Byte-based Queue 5 Shaping Rate Configure Register" */
#define SWITCH_PAGE_4D_P7_QUEUE5_MAX_REFRESH 0x0b004d1c /* "Port 7, Byte-based Queue 5 Shaping Rate Configure Register" */
#define SWITCH_PAGE_4D_IMP_QUEUE5_MAX_REFRESH 0x0b004d20 /* "Port 8, Byte-based Queue 5 Shaping Rate Configure Register" */
#define SWITCH_PAGE_4D_PN_QUEUE5_MAX_THD_SEL_PORT0 0x0b004d30 /* "Port N (0 to 6), Byte-based Queue 5 Burst Size Configure Register" */
#define SWITCH_PAGE_4D_PN_QUEUE5_MAX_THD_SEL_PORT1 0x0b004d34 /* "Port N (0 to 6), Byte-based Queue 5 Burst Size Configure Register" */
#define SWITCH_PAGE_4D_PN_QUEUE5_MAX_THD_SEL_PORT2 0x0b004d38 /* "Port N (0 to 6), Byte-based Queue 5 Burst Size Configure Register" */
#define SWITCH_PAGE_4D_PN_QUEUE5_MAX_THD_SEL_PORT3 0x0b004d3c /* "Port N (0 to 6), Byte-based Queue 5 Burst Size Configure Register" */
#define SWITCH_PAGE_4D_PN_QUEUE5_MAX_THD_SEL_PORT4 0x0b004d40 /* "Port N (0 to 6), Byte-based Queue 5 Burst Size Configure Register" */
#define SWITCH_PAGE_4D_PN_QUEUE5_MAX_THD_SEL_PORT5 0x0b004d44 /* "Port N (0 to 6), Byte-based Queue 5 Burst Size Configure Register" */
#define SWITCH_PAGE_4D_PN_QUEUE5_MAX_THD_SEL_PORT6 0x0b004d48 /* "Port N (0 to 6), Byte-based Queue 5 Burst Size Configure Register" */
#define SWITCH_PAGE_4D_P7_QUEUE5_MAX_THD_SEL 0x0b004d4c /* "Port 7, Byte-based Queue 5 Burst Size Configure Register" */
#define SWITCH_PAGE_4D_IMP_QUEUE5_MAX_THD_SEL 0x0b004d50 /* "Port 8, Byte-based Queue 5 Burst Size Configure Register" */
#define SWITCH_PAGE_4D_PN_QUEUE5_SHAPER_STS_PORT0 0x0b004d60 /* "Port N (0 to 6), Queue 5 Shaper Status Register" */
#define SWITCH_PAGE_4D_PN_QUEUE5_SHAPER_STS_PORT1 0x0b004d64 /* "Port N (0 to 6), Queue 5 Shaper Status Register" */
#define SWITCH_PAGE_4D_PN_QUEUE5_SHAPER_STS_PORT2 0x0b004d68 /* "Port N (0 to 6), Queue 5 Shaper Status Register" */
#define SWITCH_PAGE_4D_PN_QUEUE5_SHAPER_STS_PORT3 0x0b004d6c /* "Port N (0 to 6), Queue 5 Shaper Status Register" */
#define SWITCH_PAGE_4D_PN_QUEUE5_SHAPER_STS_PORT4 0x0b004d70 /* "Port N (0 to 6), Queue 5 Shaper Status Register" */
#define SWITCH_PAGE_4D_PN_QUEUE5_SHAPER_STS_PORT5 0x0b004d74 /* "Port N (0 to 6), Queue 5 Shaper Status Register" */
#define SWITCH_PAGE_4D_PN_QUEUE5_SHAPER_STS_PORT6 0x0b004d78 /* "Port N (0 to 6), Queue 5 Shaper Status Register" */
#define SWITCH_PAGE_4D_P7_QUEUE5_SHAPER_STS 0x0b004d7c /* "Port 7, Queue 5 Shaper Status Register" */
#define SWITCH_PAGE_4D_IMP_QUEUE5_SHAPER_STS 0x0b004d80 /* "Port 8, Queue 5 Shaper Status Register" */
#define SWITCH_PAGE_4D_PN_QUEUE5_MAX_PACKET_REFRESH_PORT0 0x0b004d90 /* "Port N (0 to 6), Packet-based Queue 5 Shaping Rate Configure Register" */
#define SWITCH_PAGE_4D_PN_QUEUE5_MAX_PACKET_REFRESH_PORT1 0x0b004d94 /* "Port N (0 to 6), Packet-based Queue 5 Shaping Rate Configure Register" */
#define SWITCH_PAGE_4D_PN_QUEUE5_MAX_PACKET_REFRESH_PORT2 0x0b004d98 /* "Port N (0 to 6), Packet-based Queue 5 Shaping Rate Configure Register" */
#define SWITCH_PAGE_4D_PN_QUEUE5_MAX_PACKET_REFRESH_PORT3 0x0b004d9c /* "Port N (0 to 6), Packet-based Queue 5 Shaping Rate Configure Register" */
#define SWITCH_PAGE_4D_PN_QUEUE5_MAX_PACKET_REFRESH_PORT4 0x0b004da0 /* "Port N (0 to 6), Packet-based Queue 5 Shaping Rate Configure Register" */
#define SWITCH_PAGE_4D_PN_QUEUE5_MAX_PACKET_REFRESH_PORT5 0x0b004da4 /* "Port N (0 to 6), Packet-based Queue 5 Shaping Rate Configure Register" */
#define SWITCH_PAGE_4D_PN_QUEUE5_MAX_PACKET_REFRESH_PORT6 0x0b004da8 /* "Port N (0 to 6), Packet-based Queue 5 Shaping Rate Configure Register" */
#define SWITCH_PAGE_4D_P7_QUEUE5_MAX_PACKET_REFRESH 0x0b004dac /* "Port 7, Packet-based Queue 5 Shaping Rate Configure Register" */
#define SWITCH_PAGE_4D_IMP_QUEUE5_MAX_PACKET_REFRESH 0x0b004db0 /* "Port 8, Packet-based Queue 5 Shaping Rate Configure Register" */
#define SWITCH_PAGE_4D_EGRESS_SHAPER_Q5_CONFIG_REG_SPARE0 0x0b004db8 /* Spare 0 Register (Not2Release) */
#define SWITCH_PAGE_4D_EGRESS_SHAPER_Q5_CONFIG_REG_SPARE1 0x0b004dbc /* Spare 1 Register (Not2Release) */
#define SWITCH_PAGE_4D_PN_QUEUE5_MAX_PACKET_THD_SEL_PORT0 0x0b004dc0 /* "Port N (0 to 6), Packet-based Queue 5 Burst Size Configure Register" */
#define SWITCH_PAGE_4D_PN_QUEUE5_MAX_PACKET_THD_SEL_PORT1 0x0b004dc4 /* "Port N (0 to 6), Packet-based Queue 5 Burst Size Configure Register" */
#define SWITCH_PAGE_4D_PN_QUEUE5_MAX_PACKET_THD_SEL_PORT2 0x0b004dc8 /* "Port N (0 to 6), Packet-based Queue 5 Burst Size Configure Register" */
#define SWITCH_PAGE_4D_PN_QUEUE5_MAX_PACKET_THD_SEL_PORT3 0x0b004dcc /* "Port N (0 to 6), Packet-based Queue 5 Burst Size Configure Register" */
#define SWITCH_PAGE_4D_PN_QUEUE5_MAX_PACKET_THD_SEL_PORT4 0x0b004dd0 /* "Port N (0 to 6), Packet-based Queue 5 Burst Size Configure Register" */
#define SWITCH_PAGE_4D_PN_QUEUE5_MAX_PACKET_THD_SEL_PORT5 0x0b004dd4 /* "Port N (0 to 6), Packet-based Queue 5 Burst Size Configure Register" */
#define SWITCH_PAGE_4D_PN_QUEUE5_MAX_PACKET_THD_SEL_PORT6 0x0b004dd8 /* "Port N (0 to 6), Packet-based Queue 5 Burst Size Configure Register" */
#define SWITCH_PAGE_4D_P7_QUEUE5_MAX_PACKET_THD_SEL 0x0b004ddc /* "Port 7, Packet-based Queue 5 Burst Size Configure Register" */
#define SWITCH_PAGE_4D_IMP_QUEUE5_MAX_PACKET_THD_SEL 0x0b004de0 /* "Port 8, Packet-based Queue 5 Burst Size Configure Register" */
#define SWITCH_PAGE_4D_QUEUE5_AVB_SHAPING_MODE 0x0b004de4 /* Queue 5 AVB Shaping Mode Control Register */
#define SWITCH_PAGE_4D_QUEUE5_SHAPER_ENABLE 0x0b004de6 /* Queue 5 Shaper Enable Register */
#define SWITCH_PAGE_4D_QUEUE5_SHAPER_BUCKET_COUNT_SELECT 0x0b004de8 /* Queue 5 Bucket Count Select Register */
#define SWITCH_PAGE_4D_QUEUE5_SHAPER_BLOCKING 0x0b004dea /* Queue 5 Shaper Blocking Control Register */
#define SWITCH_PAGE_4E_PN_QUEUE6_MAX_REFRESH_PORT0 0x0b004e00 /* "Port N (0 to 6), Byte-based Queue 6 Shaping Rate Configure Register" */
#define SWITCH_PAGE_4E_PN_QUEUE6_MAX_REFRESH_PORT1 0x0b004e04 /* "Port N (0 to 6), Byte-based Queue 6 Shaping Rate Configure Register" */
#define SWITCH_PAGE_4E_PN_QUEUE6_MAX_REFRESH_PORT2 0x0b004e08 /* "Port N (0 to 6), Byte-based Queue 6 Shaping Rate Configure Register" */
#define SWITCH_PAGE_4E_PN_QUEUE6_MAX_REFRESH_PORT3 0x0b004e0c /* "Port N (0 to 6), Byte-based Queue 6 Shaping Rate Configure Register" */
#define SWITCH_PAGE_4E_PN_QUEUE6_MAX_REFRESH_PORT4 0x0b004e10 /* "Port N (0 to 6), Byte-based Queue 6 Shaping Rate Configure Register" */
#define SWITCH_PAGE_4E_PN_QUEUE6_MAX_REFRESH_PORT5 0x0b004e14 /* "Port N (0 to 6), Byte-based Queue 6 Shaping Rate Configure Register" */
#define SWITCH_PAGE_4E_PN_QUEUE6_MAX_REFRESH_PORT6 0x0b004e18 /* "Port N (0 to 6), Byte-based Queue 6 Shaping Rate Configure Register" */
#define SWITCH_PAGE_4E_P7_QUEUE6_MAX_REFRESH 0x0b004e1c /* "Port 7, Byte-based Queue 6 Shaping Rate Configure Register" */
#define SWITCH_PAGE_4E_IMP_QUEUE6_MAX_REFRESH 0x0b004e20 /* "Port 8, Byte-based Queue 6 Shaping Rate Configure Register" */
#define SWITCH_PAGE_4E_PN_QUEUE6_MAX_THD_SEL_PORT0 0x0b004e30 /* "Port N (0 to 6), Byte-based Queue 6 Burst Size Configure Register" */
#define SWITCH_PAGE_4E_PN_QUEUE6_MAX_THD_SEL_PORT1 0x0b004e34 /* "Port N (0 to 6), Byte-based Queue 6 Burst Size Configure Register" */
#define SWITCH_PAGE_4E_PN_QUEUE6_MAX_THD_SEL_PORT2 0x0b004e38 /* "Port N (0 to 6), Byte-based Queue 6 Burst Size Configure Register" */
#define SWITCH_PAGE_4E_PN_QUEUE6_MAX_THD_SEL_PORT3 0x0b004e3c /* "Port N (0 to 6), Byte-based Queue 6 Burst Size Configure Register" */
#define SWITCH_PAGE_4E_PN_QUEUE6_MAX_THD_SEL_PORT4 0x0b004e40 /* "Port N (0 to 6), Byte-based Queue 6 Burst Size Configure Register" */
#define SWITCH_PAGE_4E_PN_QUEUE6_MAX_THD_SEL_PORT5 0x0b004e44 /* "Port N (0 to 6), Byte-based Queue 6 Burst Size Configure Register" */
#define SWITCH_PAGE_4E_PN_QUEUE6_MAX_THD_SEL_PORT6 0x0b004e48 /* "Port N (0 to 6), Byte-based Queue 6 Burst Size Configure Register" */
#define SWITCH_PAGE_4E_P7_QUEUE6_MAX_THD_SEL 0x0b004e4c /* "Port 7, Byte-based Queue 6 Burst Size Configure Register" */
#define SWITCH_PAGE_4E_IMP_QUEUE6_MAX_THD_SEL 0x0b004e50 /* "Port 8, Byte-based Queue 6 Burst Size Configure Register" */
#define SWITCH_PAGE_4E_PN_QUEUE6_SHAPER_STS_PORT0 0x0b004e60 /* "Port N (0 to 6), Queue 6 Shaper Status Register" */
#define SWITCH_PAGE_4E_PN_QUEUE6_SHAPER_STS_PORT1 0x0b004e64 /* "Port N (0 to 6), Queue 6 Shaper Status Register" */
#define SWITCH_PAGE_4E_PN_QUEUE6_SHAPER_STS_PORT2 0x0b004e68 /* "Port N (0 to 6), Queue 6 Shaper Status Register" */
#define SWITCH_PAGE_4E_PN_QUEUE6_SHAPER_STS_PORT3 0x0b004e6c /* "Port N (0 to 6), Queue 6 Shaper Status Register" */
#define SWITCH_PAGE_4E_PN_QUEUE6_SHAPER_STS_PORT4 0x0b004e70 /* "Port N (0 to 6), Queue 6 Shaper Status Register" */
#define SWITCH_PAGE_4E_PN_QUEUE6_SHAPER_STS_PORT5 0x0b004e74 /* "Port N (0 to 6), Queue 6 Shaper Status Register" */
#define SWITCH_PAGE_4E_PN_QUEUE6_SHAPER_STS_PORT6 0x0b004e78 /* "Port N (0 to 6), Queue 6 Shaper Status Register" */
#define SWITCH_PAGE_4E_P7_QUEUE6_SHAPER_STS 0x0b004e7c /* "Port 7, Queue 6 Shaper Status Register" */
#define SWITCH_PAGE_4E_IMP_QUEUE6_SHAPER_STS 0x0b004e80 /* "Port 8, Queue 6 Shaper Status Register" */
#define SWITCH_PAGE_4E_PN_QUEUE6_MAX_PACKET_REFRESH_PORT0 0x0b004e90 /* "Port N (0 to 6), Packet-based Queue 6 Shaping Rate Configure Register" */
#define SWITCH_PAGE_4E_PN_QUEUE6_MAX_PACKET_REFRESH_PORT1 0x0b004e94 /* "Port N (0 to 6), Packet-based Queue 6 Shaping Rate Configure Register" */
#define SWITCH_PAGE_4E_PN_QUEUE6_MAX_PACKET_REFRESH_PORT2 0x0b004e98 /* "Port N (0 to 6), Packet-based Queue 6 Shaping Rate Configure Register" */
#define SWITCH_PAGE_4E_PN_QUEUE6_MAX_PACKET_REFRESH_PORT3 0x0b004e9c /* "Port N (0 to 6), Packet-based Queue 6 Shaping Rate Configure Register" */
#define SWITCH_PAGE_4E_PN_QUEUE6_MAX_PACKET_REFRESH_PORT4 0x0b004ea0 /* "Port N (0 to 6), Packet-based Queue 6 Shaping Rate Configure Register" */
#define SWITCH_PAGE_4E_PN_QUEUE6_MAX_PACKET_REFRESH_PORT5 0x0b004ea4 /* "Port N (0 to 6), Packet-based Queue 6 Shaping Rate Configure Register" */
#define SWITCH_PAGE_4E_PN_QUEUE6_MAX_PACKET_REFRESH_PORT6 0x0b004ea8 /* "Port N (0 to 6), Packet-based Queue 6 Shaping Rate Configure Register" */
#define SWITCH_PAGE_4E_P7_QUEUE6_MAX_PACKET_REFRESH 0x0b004eac /* "Port 7, Packet-based Queue 6 Shaping Rate Configure Register" */
#define SWITCH_PAGE_4E_IMP_QUEUE6_MAX_PACKET_REFRESH 0x0b004eb0 /* "Port 8, Packet-based Queue 6 Shaping Rate Configure Register" */
#define SWITCH_PAGE_4E_EGRESS_SHAPER_Q6_CONFIG_REG_SPARE0 0x0b004eb8 /* Spare 0 Register (Not2Release) */
#define SWITCH_PAGE_4E_EGRESS_SHAPER_Q6_CONFIG_REG_SPARE1 0x0b004ebc /* Spare 1 Register (Not2Release) */
#define SWITCH_PAGE_4E_PN_QUEUE6_MAX_PACKET_THD_SEL_PORT0 0x0b004ec0 /* "Port N (0 to 6), Packet-based Queue 6 Burst Size Configure Register" */
#define SWITCH_PAGE_4E_PN_QUEUE6_MAX_PACKET_THD_SEL_PORT1 0x0b004ec4 /* "Port N (0 to 6), Packet-based Queue 6 Burst Size Configure Register" */
#define SWITCH_PAGE_4E_PN_QUEUE6_MAX_PACKET_THD_SEL_PORT2 0x0b004ec8 /* "Port N (0 to 6), Packet-based Queue 6 Burst Size Configure Register" */
#define SWITCH_PAGE_4E_PN_QUEUE6_MAX_PACKET_THD_SEL_PORT3 0x0b004ecc /* "Port N (0 to 6), Packet-based Queue 6 Burst Size Configure Register" */
#define SWITCH_PAGE_4E_PN_QUEUE6_MAX_PACKET_THD_SEL_PORT4 0x0b004ed0 /* "Port N (0 to 6), Packet-based Queue 6 Burst Size Configure Register" */
#define SWITCH_PAGE_4E_PN_QUEUE6_MAX_PACKET_THD_SEL_PORT5 0x0b004ed4 /* "Port N (0 to 6), Packet-based Queue 6 Burst Size Configure Register" */
#define SWITCH_PAGE_4E_PN_QUEUE6_MAX_PACKET_THD_SEL_PORT6 0x0b004ed8 /* "Port N (0 to 6), Packet-based Queue 6 Burst Size Configure Register" */
#define SWITCH_PAGE_4E_P7_QUEUE6_MAX_PACKET_THD_SEL 0x0b004edc /* "Port 7, Packet-based Queue 6 Burst Size Configure Register" */
#define SWITCH_PAGE_4E_IMP_QUEUE6_MAX_PACKET_THD_SEL 0x0b004ee0 /* "Port 8, Packet-based Queue 6 Burst Size Configure Register" */
#define SWITCH_PAGE_4E_QUEUE6_AVB_SHAPING_MODE 0x0b004ee4 /* Queue 6 AVB Shaping Mode Control Register */
#define SWITCH_PAGE_4E_QUEUE6_SHAPER_ENABLE 0x0b004ee6 /* Queue 6 Shaper Enable Register */
#define SWITCH_PAGE_4E_QUEUE6_SHAPER_BUCKET_COUNT_SELECT 0x0b004ee8 /* Queue 6 Bucket Count Select Register */
#define SWITCH_PAGE_4E_QUEUE6_SHAPER_BLOCKING 0x0b004eea /* Queue 6 Shaper Blocking Control Register */
#define SWITCH_PAGE_4F_PN_QUEUE7_MAX_REFRESH_PORT0 0x0b004f00 /* "Port N (0 to 6), Byte-based Queue 7 Shaping Rate Configure Register" */
#define SWITCH_PAGE_4F_PN_QUEUE7_MAX_REFRESH_PORT1 0x0b004f04 /* "Port N (0 to 6), Byte-based Queue 7 Shaping Rate Configure Register" */
#define SWITCH_PAGE_4F_PN_QUEUE7_MAX_REFRESH_PORT2 0x0b004f08 /* "Port N (0 to 6), Byte-based Queue 7 Shaping Rate Configure Register" */
#define SWITCH_PAGE_4F_PN_QUEUE7_MAX_REFRESH_PORT3 0x0b004f0c /* "Port N (0 to 6), Byte-based Queue 7 Shaping Rate Configure Register" */
#define SWITCH_PAGE_4F_PN_QUEUE7_MAX_REFRESH_PORT4 0x0b004f10 /* "Port N (0 to 6), Byte-based Queue 7 Shaping Rate Configure Register" */
#define SWITCH_PAGE_4F_PN_QUEUE7_MAX_REFRESH_PORT5 0x0b004f14 /* "Port N (0 to 6), Byte-based Queue 7 Shaping Rate Configure Register" */
#define SWITCH_PAGE_4F_PN_QUEUE7_MAX_REFRESH_PORT6 0x0b004f18 /* "Port N (0 to 6), Byte-based Queue 7 Shaping Rate Configure Register" */
#define SWITCH_PAGE_4F_P7_QUEUE7_MAX_REFRESH 0x0b004f1c /* "Port 7, Byte-based Queue 7 Shaping Rate Configure Register" */
#define SWITCH_PAGE_4F_IMP_QUEUE7_MAX_REFRESH 0x0b004f20 /* "Port 8, Byte-based Queue 7 Shaping Rate Configure Register" */
#define SWITCH_PAGE_4F_PN_QUEUE7_MAX_THD_SEL_PORT0 0x0b004f30 /* "Port N (0 to 6), Byte-based Queue 7 Burst Size Configure Register" */
#define SWITCH_PAGE_4F_PN_QUEUE7_MAX_THD_SEL_PORT1 0x0b004f34 /* "Port N (0 to 6), Byte-based Queue 7 Burst Size Configure Register" */
#define SWITCH_PAGE_4F_PN_QUEUE7_MAX_THD_SEL_PORT2 0x0b004f38 /* "Port N (0 to 6), Byte-based Queue 7 Burst Size Configure Register" */
#define SWITCH_PAGE_4F_PN_QUEUE7_MAX_THD_SEL_PORT3 0x0b004f3c /* "Port N (0 to 6), Byte-based Queue 7 Burst Size Configure Register" */
#define SWITCH_PAGE_4F_PN_QUEUE7_MAX_THD_SEL_PORT4 0x0b004f40 /* "Port N (0 to 6), Byte-based Queue 7 Burst Size Configure Register" */
#define SWITCH_PAGE_4F_PN_QUEUE7_MAX_THD_SEL_PORT5 0x0b004f44 /* "Port N (0 to 6), Byte-based Queue 7 Burst Size Configure Register" */
#define SWITCH_PAGE_4F_PN_QUEUE7_MAX_THD_SEL_PORT6 0x0b004f48 /* "Port N (0 to 6), Byte-based Queue 7 Burst Size Configure Register" */
#define SWITCH_PAGE_4F_P7_QUEUE7_MAX_THD_SEL 0x0b004f4c /* "Port 7, Byte-based Queue 7 Burst Size Configure Register" */
#define SWITCH_PAGE_4F_IMP_QUEUE7_MAX_THD_SEL 0x0b004f50 /* "Port 8, Byte-based Queue 7 Burst Size Configure Register" */
#define SWITCH_PAGE_4F_PN_QUEUE7_SHAPER_STS_PORT0 0x0b004f60 /* "Port N (0 to 6), Queue 7 Shaper Status Register" */
#define SWITCH_PAGE_4F_PN_QUEUE7_SHAPER_STS_PORT1 0x0b004f64 /* "Port N (0 to 6), Queue 7 Shaper Status Register" */
#define SWITCH_PAGE_4F_PN_QUEUE7_SHAPER_STS_PORT2 0x0b004f68 /* "Port N (0 to 6), Queue 7 Shaper Status Register" */
#define SWITCH_PAGE_4F_PN_QUEUE7_SHAPER_STS_PORT3 0x0b004f6c /* "Port N (0 to 6), Queue 7 Shaper Status Register" */
#define SWITCH_PAGE_4F_PN_QUEUE7_SHAPER_STS_PORT4 0x0b004f70 /* "Port N (0 to 6), Queue 7 Shaper Status Register" */
#define SWITCH_PAGE_4F_PN_QUEUE7_SHAPER_STS_PORT5 0x0b004f74 /* "Port N (0 to 6), Queue 7 Shaper Status Register" */
#define SWITCH_PAGE_4F_PN_QUEUE7_SHAPER_STS_PORT6 0x0b004f78 /* "Port N (0 to 6), Queue 7 Shaper Status Register" */
#define SWITCH_PAGE_4F_P7_QUEUE7_SHAPER_STS 0x0b004f7c /* "Port 7, Queue 7 Shaper Status Register" */
#define SWITCH_PAGE_4F_IMP_QUEUE7_SHAPER_STS 0x0b004f80 /* "Port 8, Queue 7 Shaper Status Register" */
#define SWITCH_PAGE_4F_PN_QUEUE7_MAX_PACKET_REFRESH_PORT0 0x0b004f90 /* "Port N (0 to 6), Packet-based Queue 7 Shaping Rate Configure Register" */
#define SWITCH_PAGE_4F_PN_QUEUE7_MAX_PACKET_REFRESH_PORT1 0x0b004f94 /* "Port N (0 to 6), Packet-based Queue 7 Shaping Rate Configure Register" */
#define SWITCH_PAGE_4F_PN_QUEUE7_MAX_PACKET_REFRESH_PORT2 0x0b004f98 /* "Port N (0 to 6), Packet-based Queue 7 Shaping Rate Configure Register" */
#define SWITCH_PAGE_4F_PN_QUEUE7_MAX_PACKET_REFRESH_PORT3 0x0b004f9c /* "Port N (0 to 6), Packet-based Queue 7 Shaping Rate Configure Register" */
#define SWITCH_PAGE_4F_PN_QUEUE7_MAX_PACKET_REFRESH_PORT4 0x0b004fa0 /* "Port N (0 to 6), Packet-based Queue 7 Shaping Rate Configure Register" */
#define SWITCH_PAGE_4F_PN_QUEUE7_MAX_PACKET_REFRESH_PORT5 0x0b004fa4 /* "Port N (0 to 6), Packet-based Queue 7 Shaping Rate Configure Register" */
#define SWITCH_PAGE_4F_PN_QUEUE7_MAX_PACKET_REFRESH_PORT6 0x0b004fa8 /* "Port N (0 to 6), Packet-based Queue 7 Shaping Rate Configure Register" */
#define SWITCH_PAGE_4F_P7_QUEUE7_MAX_PACKET_REFRESH 0x0b004fac /* "Port 7, Packet-based Queue 7 Shaping Rate Configure Register" */
#define SWITCH_PAGE_4F_IMP_QUEUE7_MAX_PACKET_REFRESH 0x0b004fb0 /* "Port 8, Packet-based Queue 7 Shaping Rate Configure Register" */
#define SWITCH_PAGE_4F_EGRESS_SHAPER_Q7_CONFIG_REG_SPARE0 0x0b004fb8 /* Spare 0 Register (Not2Release) */
#define SWITCH_PAGE_4F_EGRESS_SHAPER_Q7_CONFIG_REG_SPARE1 0x0b004fbc /* Spare 1 Register (Not2Release) */
#define SWITCH_PAGE_4F_PN_QUEUE7_MAX_PACKET_THD_SEL_PORT0 0x0b004fc0 /* "Port N (0 to 6), Packet-based Queue 7 Burst Size Configure Register" */
#define SWITCH_PAGE_4F_PN_QUEUE7_MAX_PACKET_THD_SEL_PORT1 0x0b004fc4 /* "Port N (0 to 6), Packet-based Queue 7 Burst Size Configure Register" */
#define SWITCH_PAGE_4F_PN_QUEUE7_MAX_PACKET_THD_SEL_PORT2 0x0b004fc8 /* "Port N (0 to 6), Packet-based Queue 7 Burst Size Configure Register" */
#define SWITCH_PAGE_4F_PN_QUEUE7_MAX_PACKET_THD_SEL_PORT3 0x0b004fcc /* "Port N (0 to 6), Packet-based Queue 7 Burst Size Configure Register" */
#define SWITCH_PAGE_4F_PN_QUEUE7_MAX_PACKET_THD_SEL_PORT4 0x0b004fd0 /* "Port N (0 to 6), Packet-based Queue 7 Burst Size Configure Register" */
#define SWITCH_PAGE_4F_PN_QUEUE7_MAX_PACKET_THD_SEL_PORT5 0x0b004fd4 /* "Port N (0 to 6), Packet-based Queue 7 Burst Size Configure Register" */
#define SWITCH_PAGE_4F_PN_QUEUE7_MAX_PACKET_THD_SEL_PORT6 0x0b004fd8 /* "Port N (0 to 6), Packet-based Queue 7 Burst Size Configure Register" */
#define SWITCH_PAGE_4F_P7_QUEUE7_MAX_PACKET_THD_SEL 0x0b004fdc /* "Port 7, Packet-based Queue 7 Burst Size Configure Register" */
#define SWITCH_PAGE_4F_IMP_QUEUE7_MAX_PACKET_THD_SEL 0x0b004fe0 /* "Port 8, Packet-based Queue 7 Burst Size Configure Register" */
#define SWITCH_PAGE_4F_QUEUE7_AVB_SHAPING_MODE 0x0b004fe4 /* Queue 7 AVB Shaping Mode Control Register */
#define SWITCH_PAGE_4F_QUEUE7_SHAPER_ENABLE 0x0b004fe6 /* Queue 7 Shaper Enable Register */
#define SWITCH_PAGE_4F_QUEUE7_SHAPER_BUCKET_COUNT_SELECT 0x0b004fe8 /* Queue 7 Bucket Count Select Register */
#define SWITCH_PAGE_4F_QUEUE7_SHAPER_BLOCKING 0x0b004fea /* Queue 7 Shaper Blocking Control Register */
#define SWITCH_PAGE_70_MIB_SNAPSHOT_CTL 0x0b007000 /* MIB Snapshot Control Register */
#define SWITCH_PAGE_71_S_TXOCTETS      0x0b007100 /* Tx Octets */
#define SWITCH_PAGE_71_S_TXDROPPKTS    0x0b007108 /* Tx Drop Packet Counter */
#define SWITCH_PAGE_71_S_TXQPKTQ0      0x0b00710c /* Tx Q0 Packet Counter */
#define SWITCH_PAGE_71_S_TXBROADCASTPKTS 0x0b007110 /* Tx Broadcast Packet Counter */
#define SWITCH_PAGE_71_S_TXMULTICASTPKTS 0x0b007114 /* Tx Multicast Packet Counter */
#define SWITCH_PAGE_71_S_TXUNICASTPKTS 0x0b007118 /* Tx Unicast Packet Counter */
#define SWITCH_PAGE_71_S_TXCOLLISIONS  0x0b00711c /* Tx Collision Counter */
#define SWITCH_PAGE_71_S_TXSINGLECOLLISION 0x0b007120 /* Tx Single Collision Counter */
#define SWITCH_PAGE_71_S_TXMULTIPLECOLLISION 0x0b007124 /* Tx Multiple collsion Counter */
#define SWITCH_PAGE_71_S_TXDEFERREDTRANSMIT 0x0b007128 /* Tx Deferred Transmit Counter */
#define SWITCH_PAGE_71_S_TXLATECOLLISION 0x0b00712c /* Tx Late Collision Counter */
#define SWITCH_PAGE_71_S_TXEXCESSIVECOLLISION 0x0b007130 /* Tx Excessive Collision Counter */
#define SWITCH_PAGE_71_S_TXFRAMEINDISC 0x0b007134 /* Tx Fram IN Disc Counter */
#define SWITCH_PAGE_71_S_TXPAUSEPKTS   0x0b007138 /* Tx Pause Packet Counter */
#define SWITCH_PAGE_71_S_TXQPKTQ1      0x0b00713c /* Tx Q1 Packet Counter */
#define SWITCH_PAGE_71_S_TXQPKTQ2      0x0b007140 /* Tx Q2 Packet Counter */
#define SWITCH_PAGE_71_S_TXQPKTQ3      0x0b007144 /* Tx Q3 Packet Counter */
#define SWITCH_PAGE_71_S_TXQPKTQ4      0x0b007148 /* Tx Q4 Packet Counter */
#define SWITCH_PAGE_71_S_TXQPKTQ5      0x0b00714c /* Tx Q5 Packet Counter */
#define SWITCH_PAGE_71_S_RXOCTETS      0x0b007150 /* Rx Packet Octets Counter */
#define SWITCH_PAGE_71_S_RXUNDERSIZEPKTS 0x0b007158 /* Rx Under Size Packet Octets Counter */
#define SWITCH_PAGE_71_S_RXPAUSEPKTS   0x0b00715c /* Rx Pause Packet Counter */
#define SWITCH_PAGE_71_S_RXPKTS64OCTETS 0x0b007160 /* Rx 64 Bytes Octets Counter */
#define SWITCH_PAGE_71_S_RXPKTS65TO127OCTETS 0x0b007164 /* Rx 65 to 127 Bytes Octets Counter */
#define SWITCH_PAGE_71_S_RXPKTS128TO255OCTETS 0x0b007168 /* Rx 128 to 255 Bytes Octets Counter */
#define SWITCH_PAGE_71_S_RXPKTS256TO511OCTETS 0x0b00716c /* Rx 256 to 511 Bytes Octets Counter */
#define SWITCH_PAGE_71_S_RXPKTS512TO1023OCTETS 0x0b007170 /* Rx 512 to 1023 Bytes Octets Counter */
#define SWITCH_PAGE_71_S_RXPKTS1024TOMAXPKTOCTETS 0x0b007174 /* Rx 1024 to MaxPkt Bytes Octets Counter */
#define SWITCH_PAGE_71_S_RXOVERSIZEPKTS 0x0b007178 /* Rx Over Size Packet Counter */
#define SWITCH_PAGE_71_S_RXJABBERS     0x0b00717c /* Rx Jabber Packet Counter */
#define SWITCH_PAGE_71_S_RXALIGNMENTERRORS 0x0b007180 /* Rx Alignment Error Counter */
#define SWITCH_PAGE_71_S_RXFCSERRORS   0x0b007184 /* Rx FCS Error Counter */
#define SWITCH_PAGE_71_S_RXGOODOCTETS  0x0b007188 /* Rx Good Packet Octet Counter */
#define SWITCH_PAGE_71_S_RXDROPPKTS    0x0b007190 /* Rx Drop Packet Counter */
#define SWITCH_PAGE_71_S_RXUNICASTPKTS 0x0b007194 /* Rx Unicast Packet Counter */
#define SWITCH_PAGE_71_S_RXMULTICASTPKTS 0x0b007198 /* Rx Multicast Packet Counter */
#define SWITCH_PAGE_71_S_RXBROADCASTPKTS 0x0b00719c /* Rx Broadcast Packet Counter */
#define SWITCH_PAGE_71_S_RXSACHANGES   0x0b0071a0 /* Rx SA Change Counter */
#define SWITCH_PAGE_71_S_RXFRAGMENTS   0x0b0071a4 /* Rx Fragment Counter */
#define SWITCH_PAGE_71_S_RXJUMBOPKT    0x0b0071a8 /* Jumbo Packet Counter */
#define SWITCH_PAGE_71_S_RXSYMBLERR    0x0b0071ac /* Rx Symbol Error Counter */
#define SWITCH_PAGE_71_S_INRANGEERRCOUNT 0x0b0071b0 /* InRangeErrCount Counter */
#define SWITCH_PAGE_71_S_OUTRANGEERRCOUNT 0x0b0071b4 /* OutRangeErrCount Counter */
#define SWITCH_PAGE_71_S_EEE_LPI_EVENT 0x0b0071b8 /* EEE Low-Power Idle Event Registers */
#define SWITCH_PAGE_71_S_EEE_LPI_DURATION 0x0b0071bc /* EEE Low-Power Idle Duration Registers */
#define SWITCH_PAGE_71_S_RXDISCARD     0x0b0071c0 /* Rx Discard Counter */
#define SWITCH_PAGE_71_S_TXQPKTQ6      0x0b0071c8 /* Tx Q6 Packet Counter */
#define SWITCH_PAGE_71_S_TXQPKTQ7      0x0b0071cc /* Tx Q7 Packet Counter */
#define SWITCH_PAGE_71_S_TXPKTS64OCTETS 0x0b0071d0 /* Tx 64 Bytes Octets Counter */
#define SWITCH_PAGE_71_S_TXPKTS65TO127OCTETS 0x0b0071d4 /* Tx 65 to 127 Bytes Octets Counter */
#define SWITCH_PAGE_71_S_TXPKTS128TO255OCTETS 0x0b0071d8 /* Tx 128 to 255 Bytes Octets Counter */
#define SWITCH_PAGE_71_S_TXPKTS256TO511OCTETS 0x0b0071dc /* Tx 256 to 511 Bytes Octets Counter */
#define SWITCH_PAGE_71_S_TXPKTS512TO1023OCTETS 0x0b0071e0 /* Tx 512 to 1023 Bytes Octets Counter */
#define SWITCH_PAGE_71_S_TXPKTS1024TOMAXPKTOCTETS 0x0b0071e4 /* Tx 1024 to MaxPkt Bytes Octets Counter */
#define SWITCH_PAGE_72_LPDET_CFG       0x0b007200 /* Loop Detection Configuration Registers */
#define SWITCH_PAGE_72_DF_TIMER        0x0b007202 /* Discovery Frame Timer Registers */
#define SWITCH_PAGE_72_LED_PORTMAP     0x0b007203 /* LED Waming Portmap Registers */
#define SWITCH_PAGE_72_MODULE_ID0      0x0b007205 /* Module ID 0 Registers */
#define SWITCH_PAGE_72_MODULE_ID1      0x0b00720b /* Module ID 1 Registers */
#define SWITCH_PAGE_72_LPDET_SA        0x0b007211 /* Loop Detect Frame SA Registers */
#define SWITCH_PAGE_72_LPDET_REG_SPARE0 0x0b007220 /* Spare 0 Register (Not2Release) */
#define SWITCH_PAGE_72_LPDET_REG_SPARE1 0x0b007224 /* Spare 1 Register (Not2Release) */
#define SWITCH_PAGE_73_BPM_CTRL        0x0b007300 /* BPM Power Switching Control Register */
#define SWITCH_PAGE_73_BPM_PSM_OVR_CTRL 0x0b007301 /* BPM Power Switching SW Override Register */
#define SWITCH_PAGE_73_BPM_PSM_TIME_CFG 0x0b007302 /* PSM_VDD Timing Parameter Configuration Register */
#define SWITCH_PAGE_73_BPM_PSM_THD_CFG 0x0b007304 /* PSM_VDD Switching Threshold Configuration Register */
#define SWITCH_PAGE_73_ROW_VMASK_OVR_CTRL 0x0b007308 /* BUFCON Row Status Valid Mask SW Override Control Register */
#define SWITCH_PAGE_73_BPM_STS         0x0b00730c /* BPM Status Register */
#define SWITCH_PAGE_73_BPM_PDA_OVR_CTRL 0x0b007310 /* BPM PDA Switching SW Override Control Register */
#define SWITCH_PAGE_73_PDA_TIMEOUT_CFG 0x0b007312 /* BPM PDA Switching Timeout Counter Register */
#define SWITCH_PAGE_73_PDA_SETUP_TIME_CFG 0x0b007314 /* BPM PDA Switching Setup Time Register */
#define SWITCH_PAGE_73_PDA_HOLD_TIME_CFG 0x0b007316 /* BPM PDA Switching Hold Time Register */
#define SWITCH_PAGE_73_PBB_VBUFCNT_0   0x0b007318 /* Packet Buffer Block 0 Valid Buffer Count Register */
#define SWITCH_PAGE_73_PBB_VBUFCNT_1   0x0b00731a /* Packet Buffer Block 1 Valid Buffer Count Register */
#define SWITCH_PAGE_73_PBB_VBUFCNT_2   0x0b00731c /* Packet Buffer Block 2 Valid Buffer Count Register */
#define SWITCH_PAGE_73_RCY_TIME_CFG    0x0b00731e /* Recycling Check Pulse Period Counter Register */
#define SWITCH_PAGE_73_PBB_PWRDWN_MON_CTRL 0x0b007320 /* PBB Powerdown Monitor Control Register */
#define SWITCH_PAGE_73_PBB_PWRDWN_MON_0 0x0b007328 /* PBB Powerdown Time Monitor 0 Register */
#define SWITCH_PAGE_73_PBB_PWRDWN_MON_1 0x0b007330 /* PBB Powerdown Time Monitor 1 Register */
#define SWITCH_PAGE_73_PBB_PWRDWN_MON_2 0x0b007338 /* PBB Powerdown Time Monitor 2 Register */
#define SWITCH_PAGE_73_BPM_REG_SPARE0  0x0b007360 /* Spare 0 Register (Not2Release) */
#define SWITCH_PAGE_73_BPM_REG_SPARE1  0x0b007364 /* Spare 1 Register (Not2Release) */
#define SWITCH_PAGE_90_AVB_TIME_STAMP_EN 0x0b009000 /* AVB Time Stamp Enable Register (Not2Release) */
#define SWITCH_PAGE_90_TM_STAMP_RPT_CTRL 0x0b009002 /* AVB Time Stamp Report Control Register (Not2Release) */
#define SWITCH_PAGE_90_AVB_TM_BASE     0x0b009010 /* AVB Time Base Register (Not2Release) */
#define SWITCH_PAGE_90_AVB_TM_ADJ      0x0b009014 /* AVB Time Base Adjustment Register (Not2Release) */
#define SWITCH_PAGE_90_AVB_SLOT_TICK   0x0b009018 /* AVB Slot Number and Tick Counter Register (Not2Release) */
#define SWITCH_PAGE_90_AVB_SLOT_ADJ    0x0b00901c /* AVB Slot Adjustment Register (Not2Release) */
#define SWITCH_PAGE_90_AVB_EGRESS_TM_STAMP_PORT0 0x0b009090 /* Port N AVB Egress Time Stamp Register (Not2Release) */
#define SWITCH_PAGE_90_AVB_EGRESS_TM_STAMP_PORT1 0x0b009094 /* Port N AVB Egress Time Stamp Register (Not2Release) */
#define SWITCH_PAGE_90_AVB_EGRESS_TM_STAMP_PORT2 0x0b009098 /* Port N AVB Egress Time Stamp Register (Not2Release) */
#define SWITCH_PAGE_90_AVB_EGRESS_TM_STAMP_PORT3 0x0b00909c /* Port N AVB Egress Time Stamp Register (Not2Release) */
#define SWITCH_PAGE_90_AVB_EGRESS_TM_STAMP_PORT4 0x0b0090a0 /* Port N AVB Egress Time Stamp Register (Not2Release) */
#define SWITCH_PAGE_90_AVB_EGRESS_TM_STAMP_PORT5 0x0b0090a4 /* Port N AVB Egress Time Stamp Register (Not2Release) */
#define SWITCH_PAGE_90_AVB_EGRESS_TM_STAMP_PORT6 0x0b0090a8 /* Port N AVB Egress Time Stamp Register (Not2Release) */
#define SWITCH_PAGE_90_TM_STAMP_STATUS 0x0b0090af /* AVB Egress Time Stamp Status Register (Not2Release) */
#define SWITCH_PAGE_90_EAV_LNK_STATUS  0x0b0090b0 /* AVB Port AVB Link Status Register */
#define SWITCH_PAGE_90_P1588_CTRL      0x0b0090c0 /* P1588 Control Register */
#define SWITCH_PAGE_90_AVB_TICK_CTRL   0x0b0090c2 /* AVB tick control */
#define SWITCH_PAGE_90_AVB_REG_SPARE0  0x0b0090d0 /* Spare 0 Register (Not2Release) */
#define SWITCH_PAGE_90_AVB_REG_SPARE1  0x0b0090d4 /* Spare 1 Register (Not2Release) */
#define SWITCH_PAGE_91_TRREG_CTRL0     0x0b009100 /* Traffic Remarking Control 0 Register */
#define SWITCH_PAGE_91_TRREG_CTRL1     0x0b009104 /* Traffic Remarking Control 1 Register */
#define SWITCH_PAGE_91_TRREG_CTRL2     0x0b009108 /* Traffic Remarking Control 2 Register */
#define SWITCH_PAGE_91_PN_EGRESS_PKT_TC2PCP_MAP_PORT0 0x0b009110 /* "Port N (0 to 6), Egress TC to PCP mapping Register" */
#define SWITCH_PAGE_91_PN_EGRESS_PKT_TC2PCP_MAP_PORT1 0x0b009118 /* "Port N (0 to 6), Egress TC to PCP mapping Register" */
#define SWITCH_PAGE_91_PN_EGRESS_PKT_TC2PCP_MAP_PORT2 0x0b009120 /* "Port N (0 to 6), Egress TC to PCP mapping Register" */
#define SWITCH_PAGE_91_PN_EGRESS_PKT_TC2PCP_MAP_PORT3 0x0b009128 /* "Port N (0 to 6), Egress TC to PCP mapping Register" */
#define SWITCH_PAGE_91_PN_EGRESS_PKT_TC2PCP_MAP_PORT4 0x0b009130 /* "Port N (0 to 6), Egress TC to PCP mapping Register" */
#define SWITCH_PAGE_91_PN_EGRESS_PKT_TC2PCP_MAP_PORT5 0x0b009138 /* "Port N (0 to 6), Egress TC to PCP mapping Register" */
#define SWITCH_PAGE_91_PN_EGRESS_PKT_TC2PCP_MAP_PORT6 0x0b009140 /* "Port N (0 to 6), Egress TC to PCP mapping Register" */
#define SWITCH_PAGE_91_P7_EGRESS_PKT_TC2PCP_MAP 0x0b009148 /* "Port 7, Egress TC to PCP mapping Register" */
#define SWITCH_PAGE_91_IMP_EGRESS_PKT_TC2PCP_MAP 0x0b009150 /* "Port 8, Egress TC to PCP mapping Register" */
#define SWITCH_PAGE_91_PN_EGRESS_PKT_TC2CPCP_MAP_PORT0 0x0b009160 /* "Port N (0 to 6), Egress TC to CPCP mapping Register" */
#define SWITCH_PAGE_91_PN_EGRESS_PKT_TC2CPCP_MAP_PORT1 0x0b009168 /* "Port N (0 to 6), Egress TC to CPCP mapping Register" */
#define SWITCH_PAGE_91_PN_EGRESS_PKT_TC2CPCP_MAP_PORT2 0x0b009170 /* "Port N (0 to 6), Egress TC to CPCP mapping Register" */
#define SWITCH_PAGE_91_PN_EGRESS_PKT_TC2CPCP_MAP_PORT3 0x0b009178 /* "Port N (0 to 6), Egress TC to CPCP mapping Register" */
#define SWITCH_PAGE_91_PN_EGRESS_PKT_TC2CPCP_MAP_PORT4 0x0b009180 /* "Port N (0 to 6), Egress TC to CPCP mapping Register" */
#define SWITCH_PAGE_91_PN_EGRESS_PKT_TC2CPCP_MAP_PORT5 0x0b009188 /* "Port N (0 to 6), Egress TC to CPCP mapping Register" */
#define SWITCH_PAGE_91_PN_EGRESS_PKT_TC2CPCP_MAP_PORT6 0x0b009190 /* "Port N (0 to 6), Egress TC to CPCP mapping Register" */
#define SWITCH_PAGE_91_P7_EGRESS_PKT_TC2CPCP_MAP 0x0b009198 /* "Port 7, Egress TC to CPCP mapping Register" */
#define SWITCH_PAGE_91_IMP_EGRESS_PKT_TC2CPCP_MAP 0x0b0091a0 /* "Port 8, Egress TC to CPCP mapping Register" */
#define SWITCH_PAGE_91_TRREG_REG_SPARE0 0x0b0091b0 /* Spare 0 Register (Not2Release) */
#define SWITCH_PAGE_91_TRREG_REG_SPARE1 0x0b0091b4 /* Spare 1 Register (Not2Release) */
#define SWITCH_PAGE_92_EEE_EN_CTRL     0x0b009200 /* EEE Enable Control Registers */
#define SWITCH_PAGE_92_EEE_LPI_ASSERT  0x0b009202 /* EEE Low Power Assert Status Registers */
#define SWITCH_PAGE_92_EEE_LPI_INDICATE 0x0b009204 /* EEE Low Power Indicate Status Registers */
#define SWITCH_PAGE_92_EEE_RX_IDLE_SYMBOL 0x0b009206 /* EEE Receiving Idle Symbols Status Registers */
#define SWITCH_PAGE_92_EEE_LPI_SYMBOL_TX_DISABLE 0x0b009208 /* EEE LPI Symbol Transmit Disable Registers(Not2Release) */
#define SWITCH_PAGE_92_EEE_PIPELINE_TIMER 0x0b00920c /* EEE Pipeline Delay Timer Registers */
#define SWITCH_PAGE_92_EEE_SLEEP_TIMER_G_PORT0 0x0b009210 /* EEE Port N (0 to 6) Sleep Delay Timer - 1G Registers */
#define SWITCH_PAGE_92_EEE_SLEEP_TIMER_G_PORT1 0x0b009214 /* EEE Port N (0 to 6) Sleep Delay Timer - 1G Registers */
#define SWITCH_PAGE_92_EEE_SLEEP_TIMER_G_PORT2 0x0b009218 /* EEE Port N (0 to 6) Sleep Delay Timer - 1G Registers */
#define SWITCH_PAGE_92_EEE_SLEEP_TIMER_G_PORT3 0x0b00921c /* EEE Port N (0 to 6) Sleep Delay Timer - 1G Registers */
#define SWITCH_PAGE_92_EEE_SLEEP_TIMER_G_PORT4 0x0b009220 /* EEE Port N (0 to 6) Sleep Delay Timer - 1G Registers */
#define SWITCH_PAGE_92_EEE_SLEEP_TIMER_G_PORT5 0x0b009224 /* EEE Port N (0 to 6) Sleep Delay Timer - 1G Registers */
#define SWITCH_PAGE_92_EEE_SLEEP_TIMER_G_PORT6 0x0b009228 /* EEE Port N (0 to 6) Sleep Delay Timer - 1G Registers */
#define SWITCH_PAGE_92_EEE_SLEEP_TIMER_G_P7 0x0b00922c /* EEE Port 7 Sleep Delay Timer - 1G Registers */
#define SWITCH_PAGE_92_EEE_SLEEP_TIMER_G_IMP 0x0b009230 /* EEE Port 8(IMP) Sleep Delay Timer - 1G Registers */
#define SWITCH_PAGE_92_EEE_SLEEP_TIMER_H_PORT0 0x0b009234 /* EEE Port N (0 to 6) Sleep Delay Timer - 100M Registers */
#define SWITCH_PAGE_92_EEE_SLEEP_TIMER_H_PORT1 0x0b009238 /* EEE Port N (0 to 6) Sleep Delay Timer - 100M Registers */
#define SWITCH_PAGE_92_EEE_SLEEP_TIMER_H_PORT2 0x0b00923c /* EEE Port N (0 to 6) Sleep Delay Timer - 100M Registers */
#define SWITCH_PAGE_92_EEE_SLEEP_TIMER_H_PORT3 0x0b009240 /* EEE Port N (0 to 6) Sleep Delay Timer - 100M Registers */
#define SWITCH_PAGE_92_EEE_SLEEP_TIMER_H_PORT4 0x0b009244 /* EEE Port N (0 to 6) Sleep Delay Timer - 100M Registers */
#define SWITCH_PAGE_92_EEE_SLEEP_TIMER_H_PORT5 0x0b009248 /* EEE Port N (0 to 6) Sleep Delay Timer - 100M Registers */
#define SWITCH_PAGE_92_EEE_SLEEP_TIMER_H_PORT6 0x0b00924c /* EEE Port N (0 to 6) Sleep Delay Timer - 100M Registers */
#define SWITCH_PAGE_92_EEE_SLEEP_TIMER_H_P7 0x0b009250 /* EEE Port 7 Sleep Delay Timer - 100M Registers */
#define SWITCH_PAGE_92_EEE_SLEEP_TIMER_H_IMP 0x0b009254 /* EEE Port 8(IMP) Sleep Delay Timer - 100M Registers */
#define SWITCH_PAGE_92_EEE_MIN_LP_TIMER_G_PORT0 0x0b009258 /* EEE Port (0 to 6) Minimum Low-Power Duration Timer - 1G Registers */
#define SWITCH_PAGE_92_EEE_MIN_LP_TIMER_G_PORT1 0x0b00925c /* EEE Port (0 to 6) Minimum Low-Power Duration Timer - 1G Registers */
#define SWITCH_PAGE_92_EEE_MIN_LP_TIMER_G_PORT2 0x0b009260 /* EEE Port (0 to 6) Minimum Low-Power Duration Timer - 1G Registers */
#define SWITCH_PAGE_92_EEE_MIN_LP_TIMER_G_PORT3 0x0b009264 /* EEE Port (0 to 6) Minimum Low-Power Duration Timer - 1G Registers */
#define SWITCH_PAGE_92_EEE_MIN_LP_TIMER_G_PORT4 0x0b009268 /* EEE Port (0 to 6) Minimum Low-Power Duration Timer - 1G Registers */
#define SWITCH_PAGE_92_EEE_MIN_LP_TIMER_G_PORT5 0x0b00926c /* EEE Port (0 to 6) Minimum Low-Power Duration Timer - 1G Registers */
#define SWITCH_PAGE_92_EEE_MIN_LP_TIMER_G_PORT6 0x0b009270 /* EEE Port (0 to 6) Minimum Low-Power Duration Timer - 1G Registers */
#define SWITCH_PAGE_92_EEE_MIN_LP_TIMER_G_P7 0x0b009274 /* EEE Port 7 Minimum Low-Power Duration Timer Registers */
#define SWITCH_PAGE_92_EEE_MIN_LP_TIMER_G_IMP 0x0b009278 /* EEE Port 8(IMP) Minimum Low-Power Duration Timer Registers */
#define SWITCH_PAGE_92_EEE_MIN_LP_TIMER_H_PORT0 0x0b00927c /* EEE Port (0 to 6) Minimum Low-Power Duration Timer - 100M Registers */
#define SWITCH_PAGE_92_EEE_MIN_LP_TIMER_H_PORT1 0x0b009280 /* EEE Port (0 to 6) Minimum Low-Power Duration Timer - 100M Registers */
#define SWITCH_PAGE_92_EEE_MIN_LP_TIMER_H_PORT2 0x0b009284 /* EEE Port (0 to 6) Minimum Low-Power Duration Timer - 100M Registers */
#define SWITCH_PAGE_92_EEE_MIN_LP_TIMER_H_PORT3 0x0b009288 /* EEE Port (0 to 6) Minimum Low-Power Duration Timer - 100M Registers */
#define SWITCH_PAGE_92_EEE_MIN_LP_TIMER_H_PORT4 0x0b00928c /* EEE Port (0 to 6) Minimum Low-Power Duration Timer - 100M Registers */
#define SWITCH_PAGE_92_EEE_MIN_LP_TIMER_H_PORT5 0x0b009290 /* EEE Port (0 to 6) Minimum Low-Power Duration Timer - 100M Registers */
#define SWITCH_PAGE_92_EEE_MIN_LP_TIMER_H_PORT6 0x0b009294 /* EEE Port (0 to 6) Minimum Low-Power Duration Timer - 100M Registers */
#define SWITCH_PAGE_92_EEE_MIN_LP_TIMER_H_P7 0x0b009298 /* EEE Port 7 Minimum Low-Power Duration Timer - 100M Registers */
#define SWITCH_PAGE_92_EEE_MIN_LP_TIMER_H_IMP 0x0b00929c /* EEE Port 8(IMP) Minimum Low-Power Duration Timer - 100M Registers */
#define SWITCH_PAGE_92_EEE_WAKE_TIMER_G_PORT0 0x0b0092a0 /* EEE Port N (0 to 6) Wake Transition Timer - 1G Registers */
#define SWITCH_PAGE_92_EEE_WAKE_TIMER_G_PORT1 0x0b0092a2 /* EEE Port N (0 to 6) Wake Transition Timer - 1G Registers */
#define SWITCH_PAGE_92_EEE_WAKE_TIMER_G_PORT2 0x0b0092a4 /* EEE Port N (0 to 6) Wake Transition Timer - 1G Registers */
#define SWITCH_PAGE_92_EEE_WAKE_TIMER_G_PORT3 0x0b0092a6 /* EEE Port N (0 to 6) Wake Transition Timer - 1G Registers */
#define SWITCH_PAGE_92_EEE_WAKE_TIMER_G_PORT4 0x0b0092a8 /* EEE Port N (0 to 6) Wake Transition Timer - 1G Registers */
#define SWITCH_PAGE_92_EEE_WAKE_TIMER_G_PORT5 0x0b0092aa /* EEE Port N (0 to 6) Wake Transition Timer - 1G Registers */
#define SWITCH_PAGE_92_EEE_WAKE_TIMER_G_PORT6 0x0b0092ac /* EEE Port N (0 to 6) Wake Transition Timer - 1G Registers */
#define SWITCH_PAGE_92_EEE_WAKE_TIMER_G_P7 0x0b0092ae /* EEE Port 7 Wake Transition Timer - 1G Registers */
#define SWITCH_PAGE_92_EEE_WAKE_TIMER_G_IMP 0x0b0092b0 /* EEE Port 8(IMP) Wake Transition Timer - 1G Registers */
#define SWITCH_PAGE_92_EEE_WAKE_TIMER_H_PORT0 0x0b0092b2 /* EEE Port N (0 to 6) Wake Transition Timer - 100M Registers */
#define SWITCH_PAGE_92_EEE_WAKE_TIMER_H_PORT1 0x0b0092b4 /* EEE Port N (0 to 6) Wake Transition Timer - 100M Registers */
#define SWITCH_PAGE_92_EEE_WAKE_TIMER_H_PORT2 0x0b0092b6 /* EEE Port N (0 to 6) Wake Transition Timer - 100M Registers */
#define SWITCH_PAGE_92_EEE_WAKE_TIMER_H_PORT3 0x0b0092b8 /* EEE Port N (0 to 6) Wake Transition Timer - 100M Registers */
#define SWITCH_PAGE_92_EEE_WAKE_TIMER_H_PORT4 0x0b0092ba /* EEE Port N (0 to 6) Wake Transition Timer - 100M Registers */
#define SWITCH_PAGE_92_EEE_WAKE_TIMER_H_PORT5 0x0b0092bc /* EEE Port N (0 to 6) Wake Transition Timer - 100M Registers */
#define SWITCH_PAGE_92_EEE_WAKE_TIMER_H_PORT6 0x0b0092be /* EEE Port N (0 to 6) Wake Transition Timer - 100M Registers */
#define SWITCH_PAGE_92_EEE_WAKE_TIMER_H_P7 0x0b0092c0 /* EEE Port 7 Wake Transition Timer - 100M Registers */
#define SWITCH_PAGE_92_EEE_WAKE_TIMER_H_IMP 0x0b0092c2 /* EEE Port 8(IMP) Wake Transition Timer - 100M Registers */
#define SWITCH_PAGE_92_EEE_GLB_CONG_TH 0x0b0092c4 /* EEE Global Congestion Threshold Registers */
#define SWITCH_PAGE_92_EEE_TXQ_CONG_TH0 0x0b0092c6 /* EEE TXQ 0 Congestion Threshold Registers */
#define SWITCH_PAGE_92_EEE_TXQ_CONG_TH1 0x0b0092c8 /* EEE TXQ 1 Congestion Threshold Registers */
#define SWITCH_PAGE_92_EEE_TXQ_CONG_TH2 0x0b0092ca /* EEE TXQ 2 Congestion Threshold Registers */
#define SWITCH_PAGE_92_EEE_TXQ_CONG_TH3 0x0b0092cc /* EEE TXQ 3 Congestion Threshold Registers */
#define SWITCH_PAGE_92_EEE_TXQ_CONG_TH4 0x0b0092ce /* EEE TXQ 4 Congestion Threshold Registers */
#define SWITCH_PAGE_92_EEE_TXQ_CONG_TH5 0x0b0092d0 /* EEE TXQ 5 Congestion Threshold Registers */
#define SWITCH_PAGE_92_EEE_TXQ_CONG_TH6 0x0b0092d3 /* EEE TXQ 6 Congestion Threshold Registers */
#define SWITCH_PAGE_92_EEE_TXQ_CONG_TH7 0x0b0092d5 /* EEE TXQ 7 Congestion Threshold Registers */
#define SWITCH_PAGE_92_EEE_CTL_REG_SPARE0 0x0b0092db /* Spare 0 Register (Not2Release) */
#define SWITCH_PAGE_92_EEE_CTL_REG_SPARE1 0x0b0092e0 /* Spare 1 Register (Not2Release) */
#define SWITCH_PAGE_92_EEE_DEBUG       0x0b0092e7 /* EEE Debug Registers(Not2Release) */
#define SWITCH_PAGE_92_EEE_LINK_DLY_TIMER 0x0b0092e8 /* EEE Link Delay Timer Registers(Not2Release) */
#define SWITCH_PAGE_92_EEE_STATE       0x0b0092ec /* EEE Control Policy State Registers(Not2Release) */
#define SWITCH_PAGE_95_RED_CONTROL     0x0b009500 /* RED Control Register */
#define SWITCH_PAGE_95_TC2RED_PROFILE_TABLE 0x0b009502 /* RED Table Configuration Register */
#define SWITCH_PAGE_95_RED_EGRESS_BYPASS 0x0b009504 /* RED Egress Bypass Register */
#define SWITCH_PAGE_95_RED_AQD_CONTROL 0x0b009506 /* RED AQD Control Register */
#define SWITCH_PAGE_95_RED_EXPONENT    0x0b009508 /* RED AQD Weighted Factor Register */
#define SWITCH_PAGE_95_RED_DROP_ADD_TO_MIB 0x0b00950a /* RED Drop Add to MIB Register */
#define SWITCH_PAGE_95_RED_PROFILE_DEFAULT 0x0b009510 /* Default RED profile Register */
#define SWITCH_PAGE_95_WRED_REG_SPARE0 0x0b009514 /* Spare 0 Register (Not2Release) */
#define SWITCH_PAGE_95_WRED_REG_SPARE1 0x0b009518 /* Spare 1 Register (Not2Release) */
#define SWITCH_PAGE_95_RED_PROFILE_0   0x0b009520 /* RED profile 0 Register */
#define SWITCH_PAGE_95_RED_PROFILE_1   0x0b009524 /* RED profile 1 Register */
#define SWITCH_PAGE_95_RED_PROFILE_2   0x0b009528 /* RED profile 2 Register */
#define SWITCH_PAGE_95_RED_PROFILE_3   0x0b00952c /* RED profile 3 Register */
#define SWITCH_PAGE_95_RED_PROFILE_4   0x0b009530 /* RED profile 4 Register */
#define SWITCH_PAGE_95_RED_PROFILE_5   0x0b009534 /* RED profile 5 Register */
#define SWITCH_PAGE_95_RED_PROFILE_6   0x0b009538 /* RED profile 6 Register */
#define SWITCH_PAGE_95_RED_PROFILE_7   0x0b00953c /* RED profile 7 Register */
#define SWITCH_PAGE_95_RED_PROFILE_8   0x0b009540 /* RED profile 8 Register */
#define SWITCH_PAGE_95_RED_PROFILE_9   0x0b009544 /* RED profile 9 Register */
#define SWITCH_PAGE_95_RED_PROFILE_10  0x0b009548 /* RED profile 10 Register */
#define SWITCH_PAGE_95_RED_PROFILE_11  0x0b00954c /* RED profile 11 Register */
#define SWITCH_PAGE_95_RED_PROFILE_12  0x0b009550 /* RED profile 12 Register */
#define SWITCH_PAGE_95_RED_PROFILE_13  0x0b009554 /* RED profile 13 Register */
#define SWITCH_PAGE_95_RED_PROFILE_14  0x0b009558 /* RED profile 14 Register */
#define SWITCH_PAGE_95_RED_PROFILE_15  0x0b00955c /* RED profile 15 Register */
#define SWITCH_PAGE_95_RED_DROP_CNTR_RST 0x0b00956c /* RED Drop Counter Reset Register */
#define SWITCH_PAGE_95_PN_PORT_RED_PKT_DROP_CNTR_PORT0 0x0b009570 /* PORT N (0 to 6) RED Packet Drop Counter Register */
#define SWITCH_PAGE_95_PN_PORT_RED_PKT_DROP_CNTR_PORT1 0x0b009574 /* PORT N (0 to 6) RED Packet Drop Counter Register */
#define SWITCH_PAGE_95_PN_PORT_RED_PKT_DROP_CNTR_PORT2 0x0b009578 /* PORT N (0 to 6) RED Packet Drop Counter Register */
#define SWITCH_PAGE_95_PN_PORT_RED_PKT_DROP_CNTR_PORT3 0x0b00957c /* PORT N (0 to 6) RED Packet Drop Counter Register */
#define SWITCH_PAGE_95_PN_PORT_RED_PKT_DROP_CNTR_PORT4 0x0b009580 /* PORT N (0 to 6) RED Packet Drop Counter Register */
#define SWITCH_PAGE_95_PN_PORT_RED_PKT_DROP_CNTR_PORT5 0x0b009584 /* PORT N (0 to 6) RED Packet Drop Counter Register */
#define SWITCH_PAGE_95_PN_PORT_RED_PKT_DROP_CNTR_PORT6 0x0b009588 /* PORT N (0 to 6) RED Packet Drop Counter Register */
#define SWITCH_PAGE_95_P7_PORT_RED_PKT_DROP_CNTR 0x0b00958c /* PORT 7 RED Packet Drop Counter Register */
#define SWITCH_PAGE_95_IMP_PORT_RED_PKT_DROP_CNTR 0x0b009590 /* PORT 8 RED Packet Drop Counter Register */
#define SWITCH_PAGE_95_PN_PORT_RED_BYTE_DROP_CNTR_PORT0 0x0b0095a0 /* PORT N (0 to 6) RED Byte Drop Counter Register */
#define SWITCH_PAGE_95_PN_PORT_RED_BYTE_DROP_CNTR_PORT1 0x0b0095a8 /* PORT N (0 to 6) RED Byte Drop Counter Register */
#define SWITCH_PAGE_95_PN_PORT_RED_BYTE_DROP_CNTR_PORT2 0x0b0095b0 /* PORT N (0 to 6) RED Byte Drop Counter Register */
#define SWITCH_PAGE_95_PN_PORT_RED_BYTE_DROP_CNTR_PORT3 0x0b0095b8 /* PORT N (0 to 6) RED Byte Drop Counter Register */
#define SWITCH_PAGE_95_PN_PORT_RED_BYTE_DROP_CNTR_PORT4 0x0b0095c0 /* PORT N (0 to 6) RED Byte Drop Counter Register */
#define SWITCH_PAGE_95_PN_PORT_RED_BYTE_DROP_CNTR_PORT5 0x0b0095c8 /* PORT N (0 to 6) RED Byte Drop Counter Register */
#define SWITCH_PAGE_95_PN_PORT_RED_BYTE_DROP_CNTR_PORT6 0x0b0095d0 /* PORT N (0 to 6) RED Byte Drop Counter Register */
#define SWITCH_PAGE_95_P7_PORT_RED_BYTE_DROP_CNTR 0x0b0095d8 /* PORT 7 RED Byte Drop Counter Register */
#define SWITCH_PAGE_95_IMP_PORT_RED_BYTE_DROP_CNTR 0x0b0095e0 /* PORT 8 RED Byte Drop Counter Register */
#define SWITCH_PAGE_A0_CFP_ACC         0x0b00a000 /* CFP Access Registers */
#define SWITCH_PAGE_A0_RATE_METER_GLOBAL_CTL 0x0b00a004 /* CFP RATE METER Global Control Registers */
#define SWITCH_PAGE_A0_CFP_DATA0       0x0b00a010 /* CFP TCAM Data 0 Registers */
#define SWITCH_PAGE_A0_CFP_DATA1       0x0b00a014 /* CFP TCAM Data 1 Registers */
#define SWITCH_PAGE_A0_CFP_DATA2       0x0b00a018 /* CFP TCAM Data 2 Registers */
#define SWITCH_PAGE_A0_CFP_DATA3       0x0b00a01c /* CFP TCAM Data 3 Registers */
#define SWITCH_PAGE_A0_CFP_DATA4       0x0b00a020 /* CFP TCAM Data 4 Registers */
#define SWITCH_PAGE_A0_CFP_DATA5       0x0b00a024 /* CFP TCAM Data 5 Registers */
#define SWITCH_PAGE_A0_CFP_DATA6       0x0b00a028 /* CFP TCAM Data 6 Registers */
#define SWITCH_PAGE_A0_CFP_DATA7       0x0b00a02c /* CFP TCAM Data 7 Registers */
#define SWITCH_PAGE_A0_CFP_MASK0       0x0b00a030 /* CFP TCAM Mask 0 Registers */
#define SWITCH_PAGE_A0_CFP_MASK1       0x0b00a034 /* CFP TCAM Mask 1 Registers */
#define SWITCH_PAGE_A0_CFP_MASK2       0x0b00a038 /* CFP TCAM Mask 2 Registers */
#define SWITCH_PAGE_A0_CFP_MASK3       0x0b00a03c /* CFP TCAM Mask 3 Registers */
#define SWITCH_PAGE_A0_CFP_MASK4       0x0b00a040 /* CFP TCAM Mask 4 Registers */
#define SWITCH_PAGE_A0_CFP_MASK5       0x0b00a044 /* CFP TCAM Mask 5 Registers */
#define SWITCH_PAGE_A0_CFP_MASK6       0x0b00a048 /* CFP TCAM Mask 6 Registers */
#define SWITCH_PAGE_A0_CFP_MASK7       0x0b00a04c /* CFP TCAM Mask 7 Registers */
#define SWITCH_PAGE_A0_ACT_POL_DATA0   0x0b00a050 /* CFP Action/Policy Data 0 Registers */
#define SWITCH_PAGE_A0_ACT_POL_DATA1   0x0b00a054 /* CFP Action/Policy Data 1 Registers */
#define SWITCH_PAGE_A0_ACT_POL_DATA2   0x0b00a058 /* CFP Action/Policy Data 2 Registers */
#define SWITCH_PAGE_A0_RATE_METER0     0x0b00a060 /* CFP RATE METER DATA 0 Registers */
#define SWITCH_PAGE_A0_RATE_METER1     0x0b00a064 /* CFP RATE METER DATA 1 Registers */
#define SWITCH_PAGE_A0_RATE_METER2     0x0b00a068 /* CFP RATE METER DATA 2 Registers */
#define SWITCH_PAGE_A0_RATE_METER3     0x0b00a06c /* CFP RATE METER DATA 3 Registers */
#define SWITCH_PAGE_A0_RATE_METER4     0x0b00a070 /* CFP RATE METER DATA 4 Registers */
#define SWITCH_PAGE_A0_RATE_METER5     0x0b00a074 /* CFP RATE METER DATA 5 Registers */
#define SWITCH_PAGE_A0_RATE_METER6     0x0b00a078 /* CFP RATE METER DATA 6 Registers */
#define SWITCH_PAGE_A0_TC2COLOR        0x0b00a07c /* TC to COLOR Mapping Registers */
#define SWITCH_PAGE_A0_STAT_GREEN_CNTR 0x0b00a080 /* Policer Green color statistic counter */
#define SWITCH_PAGE_A0_STAT_YELLOW_CNTR 0x0b00a084 /* Policer Yellow color statistic counter */
#define SWITCH_PAGE_A0_STAT_RED_CNTR   0x0b00a088 /* Policer RED color statistic counter */
#define SWITCH_PAGE_A0_TCAM_BIST_CONTROL 0x0b00a0a0 /* TCAM BIST Control Registers (Not2Release) */
#define SWITCH_PAGE_A0_TCAM_BIST_STATUS 0x0b00a0a4 /* TCAM BIST Status Registers (Not2Release) */
#define SWITCH_PAGE_A0_TCAM_TEST_COMPARE_STATUS 0x0b00a0a8 /* TCAM Test Compare Status Registers (Not2Release) */
#define SWITCH_PAGE_A0_CFP_REG_SPARE0  0x0b00a0b0 /* Spare 0 Register (Not2Release) */
#define SWITCH_PAGE_A0_CFP_REG_SPARE1  0x0b00a0b4 /* Spare 1 Register (Not2Release) */
#define SWITCH_PAGE_A1_CFP_CTL_REG     0x0b00a100 /* CFP Control Registers */
#define SWITCH_PAGE_A1_UDF_0_A_0       0x0b00a110 /* UDFs of slice 0 for IPv4 packet Registers */
#define SWITCH_PAGE_A1_UDF_0_A_1       0x0b00a111 /* UDFs of slice 0 for IPv4 packet Registers */
#define SWITCH_PAGE_A1_UDF_0_A_2       0x0b00a112 /* UDFs of slice 0 for IPv4 packet Registers */
#define SWITCH_PAGE_A1_UDF_0_A_3       0x0b00a113 /* UDFs of slice 0 for IPv4 packet Registers */
#define SWITCH_PAGE_A1_UDF_0_A_4       0x0b00a114 /* UDFs of slice 0 for IPv4 packet Registers */
#define SWITCH_PAGE_A1_UDF_0_A_5       0x0b00a115 /* UDFs of slice 0 for IPv4 packet Registers */
#define SWITCH_PAGE_A1_UDF_0_A_6       0x0b00a116 /* UDFs of slice 0 for IPv4 packet Registers */
#define SWITCH_PAGE_A1_UDF_0_A_7       0x0b00a117 /* UDFs of slice 0 for IPv4 packet Registers */
#define SWITCH_PAGE_A1_UDF_0_A_8       0x0b00a118 /* UDFs of slice 0 for IPv4 packet Registers */
#define SWITCH_PAGE_A1_UDF_1_A_0       0x0b00a120 /* UDFs of slice 1 for IPv4 packet Registers */
#define SWITCH_PAGE_A1_UDF_1_A_1       0x0b00a121 /* UDFs of slice 1 for IPv4 packet Registers */
#define SWITCH_PAGE_A1_UDF_1_A_2       0x0b00a122 /* UDFs of slice 1 for IPv4 packet Registers */
#define SWITCH_PAGE_A1_UDF_1_A_3       0x0b00a123 /* UDFs of slice 1 for IPv4 packet Registers */
#define SWITCH_PAGE_A1_UDF_1_A_4       0x0b00a124 /* UDFs of slice 1 for IPv4 packet Registers */
#define SWITCH_PAGE_A1_UDF_1_A_5       0x0b00a125 /* UDFs of slice 1 for IPv4 packet Registers */
#define SWITCH_PAGE_A1_UDF_1_A_6       0x0b00a126 /* UDFs of slice 1 for IPv4 packet Registers */
#define SWITCH_PAGE_A1_UDF_1_A_7       0x0b00a127 /* UDFs of slice 1 for IPv4 packet Registers */
#define SWITCH_PAGE_A1_UDF_1_A_8       0x0b00a128 /* UDFs of slice 1 for IPv4 packet Registers */
#define SWITCH_PAGE_A1_UDF_2_A_0       0x0b00a130 /* UDFs of slice 2 for IPv4 packet Registers */
#define SWITCH_PAGE_A1_UDF_2_A_1       0x0b00a131 /* UDFs of slice 2 for IPv4 packet Registers */
#define SWITCH_PAGE_A1_UDF_2_A_2       0x0b00a132 /* UDFs of slice 2 for IPv4 packet Registers */
#define SWITCH_PAGE_A1_UDF_2_A_3       0x0b00a133 /* UDFs of slice 2 for IPv4 packet Registers */
#define SWITCH_PAGE_A1_UDF_2_A_4       0x0b00a134 /* UDFs of slice 2 for IPv4 packet Registers */
#define SWITCH_PAGE_A1_UDF_2_A_5       0x0b00a135 /* UDFs of slice 2 for IPv4 packet Registers */
#define SWITCH_PAGE_A1_UDF_2_A_6       0x0b00a136 /* UDFs of slice 2 for IPv4 packet Registers */
#define SWITCH_PAGE_A1_UDF_2_A_7       0x0b00a137 /* UDFs of slice 2 for IPv4 packet Registers */
#define SWITCH_PAGE_A1_UDF_2_A_8       0x0b00a138 /* UDFs of slice 2 for IPv4 packet Registers */
#define SWITCH_PAGE_A1_UDF_0_B_0       0x0b00a140 /* UDFs of slice 0 for IPv6 packet Registers */
#define SWITCH_PAGE_A1_UDF_0_B_1       0x0b00a141 /* UDFs of slice 0 for IPv6 packet Registers */
#define SWITCH_PAGE_A1_UDF_0_B_2       0x0b00a142 /* UDFs of slice 0 for IPv6 packet Registers */
#define SWITCH_PAGE_A1_UDF_0_B_3       0x0b00a143 /* UDFs of slice 0 for IPv6 packet Registers */
#define SWITCH_PAGE_A1_UDF_0_B_4       0x0b00a144 /* UDFs of slice 0 for IPv6 packet Registers */
#define SWITCH_PAGE_A1_UDF_0_B_5       0x0b00a145 /* UDFs of slice 0 for IPv6 packet Registers */
#define SWITCH_PAGE_A1_UDF_0_B_6       0x0b00a146 /* UDFs of slice 0 for IPv6 packet Registers */
#define SWITCH_PAGE_A1_UDF_0_B_7       0x0b00a147 /* UDFs of slice 0 for IPv6 packet Registers */
#define SWITCH_PAGE_A1_UDF_0_B_8       0x0b00a148 /* UDFs of slice 0 for IPv6 packet Registers */
#define SWITCH_PAGE_A1_UDF_1_B_0       0x0b00a150 /* UDFs of slice 1 for IPv6 packet Registers */
#define SWITCH_PAGE_A1_UDF_1_B_1       0x0b00a151 /* UDFs of slice 1 for IPv6 packet Registers */
#define SWITCH_PAGE_A1_UDF_1_B_2       0x0b00a152 /* UDFs of slice 1 for IPv6 packet Registers */
#define SWITCH_PAGE_A1_UDF_1_B_3       0x0b00a153 /* UDFs of slice 1 for IPv6 packet Registers */
#define SWITCH_PAGE_A1_UDF_1_B_4       0x0b00a154 /* UDFs of slice 1 for IPv6 packet Registers */
#define SWITCH_PAGE_A1_UDF_1_B_5       0x0b00a155 /* UDFs of slice 1 for IPv6 packet Registers */
#define SWITCH_PAGE_A1_UDF_1_B_6       0x0b00a156 /* UDFs of slice 1 for IPv6 packet Registers */
#define SWITCH_PAGE_A1_UDF_1_B_7       0x0b00a157 /* UDFs of slice 1 for IPv6 packet Registers */
#define SWITCH_PAGE_A1_UDF_1_B_8       0x0b00a158 /* UDFs of slice 1 for IPv6 packet Registers */
#define SWITCH_PAGE_A1_UDF_2_B_0       0x0b00a160 /* UDFs of slice 2 for IPv6 packet Registers */
#define SWITCH_PAGE_A1_UDF_2_B_1       0x0b00a161 /* UDFs of slice 2 for IPv6 packet Registers */
#define SWITCH_PAGE_A1_UDF_2_B_2       0x0b00a162 /* UDFs of slice 2 for IPv6 packet Registers */
#define SWITCH_PAGE_A1_UDF_2_B_3       0x0b00a163 /* UDFs of slice 2 for IPv6 packet Registers */
#define SWITCH_PAGE_A1_UDF_2_B_4       0x0b00a164 /* UDFs of slice 2 for IPv6 packet Registers */
#define SWITCH_PAGE_A1_UDF_2_B_5       0x0b00a165 /* UDFs of slice 2 for IPv6 packet Registers */
#define SWITCH_PAGE_A1_UDF_2_B_6       0x0b00a166 /* UDFs of slice 2 for IPv6 packet Registers */
#define SWITCH_PAGE_A1_UDF_2_B_7       0x0b00a167 /* UDFs of slice 2 for IPv6 packet Registers */
#define SWITCH_PAGE_A1_UDF_2_B_8       0x0b00a168 /* UDFs of slice 2 for IPv6 packet Registers */
#define SWITCH_PAGE_A1_UDF_0_C_0       0x0b00a170 /* UDFs of slice 0 for non-IP packet Registers */
#define SWITCH_PAGE_A1_UDF_0_C_1       0x0b00a171 /* UDFs of slice 0 for non-IP packet Registers */
#define SWITCH_PAGE_A1_UDF_0_C_2       0x0b00a172 /* UDFs of slice 0 for non-IP packet Registers */
#define SWITCH_PAGE_A1_UDF_0_C_3       0x0b00a173 /* UDFs of slice 0 for non-IP packet Registers */
#define SWITCH_PAGE_A1_UDF_0_C_4       0x0b00a174 /* UDFs of slice 0 for non-IP packet Registers */
#define SWITCH_PAGE_A1_UDF_0_C_5       0x0b00a175 /* UDFs of slice 0 for non-IP packet Registers */
#define SWITCH_PAGE_A1_UDF_0_C_6       0x0b00a176 /* UDFs of slice 0 for non-IP packet Registers */
#define SWITCH_PAGE_A1_UDF_0_C_7       0x0b00a177 /* UDFs of slice 0 for non-IP packet Registers */
#define SWITCH_PAGE_A1_UDF_0_C_8       0x0b00a178 /* UDFs of slice 0 for non-IP packet Registers */
#define SWITCH_PAGE_A1_UDF_1_C_0       0x0b00a180 /* UDFs of slice 1 for non-IP packet Registers */
#define SWITCH_PAGE_A1_UDF_1_C_1       0x0b00a181 /* UDFs of slice 1 for non-IP packet Registers */
#define SWITCH_PAGE_A1_UDF_1_C_2       0x0b00a182 /* UDFs of slice 1 for non-IP packet Registers */
#define SWITCH_PAGE_A1_UDF_1_C_3       0x0b00a183 /* UDFs of slice 1 for non-IP packet Registers */
#define SWITCH_PAGE_A1_UDF_1_C_4       0x0b00a184 /* UDFs of slice 1 for non-IP packet Registers */
#define SWITCH_PAGE_A1_UDF_1_C_5       0x0b00a185 /* UDFs of slice 1 for non-IP packet Registers */
#define SWITCH_PAGE_A1_UDF_1_C_6       0x0b00a186 /* UDFs of slice 1 for non-IP packet Registers */
#define SWITCH_PAGE_A1_UDF_1_C_7       0x0b00a187 /* UDFs of slice 1 for non-IP packet Registers */
#define SWITCH_PAGE_A1_UDF_1_C_8       0x0b00a188 /* UDFs of slice 1 for non-IP packet Registers */
#define SWITCH_PAGE_A1_UDF_2_C_0       0x0b00a190 /* UDFs of slice 2 for non-IP packet Registers */
#define SWITCH_PAGE_A1_UDF_2_C_1       0x0b00a191 /* UDFs of slice 2 for non-IP packet Registers */
#define SWITCH_PAGE_A1_UDF_2_C_2       0x0b00a192 /* UDFs of slice 2 for non-IP packet Registers */
#define SWITCH_PAGE_A1_UDF_2_C_3       0x0b00a193 /* UDFs of slice 2 for non-IP packet Registers */
#define SWITCH_PAGE_A1_UDF_2_C_4       0x0b00a194 /* UDFs of slice 2 for non-IP packet Registers */
#define SWITCH_PAGE_A1_UDF_2_C_5       0x0b00a195 /* UDFs of slice 2 for non-IP packet Registers */
#define SWITCH_PAGE_A1_UDF_2_C_6       0x0b00a196 /* UDFs of slice 2 for non-IP packet Registers */
#define SWITCH_PAGE_A1_UDF_2_C_7       0x0b00a197 /* UDFs of slice 2 for non-IP packet Registers */
#define SWITCH_PAGE_A1_UDF_2_C_8       0x0b00a198 /* UDFs of slice 2 for non-IP packet Registers */
#define SWITCH_PAGE_A1_UDF_0_D_0       0x0b00a1a0 /* UDF 0 for IPv6 Chain Rule Registers */
#define SWITCH_PAGE_A1_UDF_0_D_1       0x0b00a1a1 /* UDF 1 for IPv6 Chain Rule Registers */
#define SWITCH_PAGE_A1_UDF_0_D_2       0x0b00a1a2 /* UDF 2 for IPv6 Chain Rule Registers */
#define SWITCH_PAGE_A1_UDF_0_D_3       0x0b00a1a3 /* UDF 3 for IPv6 Chain Rule Registers */
#define SWITCH_PAGE_A1_UDF_0_D_4       0x0b00a1a4 /* UDF 4 for IPv6 Chain Rule Registers */
#define SWITCH_PAGE_A1_UDF_0_D_5       0x0b00a1a5 /* UDF 5 for IPv6 Chain Rule Registers */
#define SWITCH_PAGE_A1_UDF_0_D_6       0x0b00a1a6 /* UDF 6 for IPv6 Chain Rule Registers */
#define SWITCH_PAGE_A1_UDF_0_D_7       0x0b00a1a7 /* UDF 7 for IPv6 Chain Rule Registers */
#define SWITCH_PAGE_A1_UDF_0_D_8       0x0b00a1a8 /* UDF 8 for IPv6 Chain Rule Registers */
#define SWITCH_PAGE_A1_UDF_0_D_9       0x0b00a1a9 /* UDF 9 for IPv6 Chain Rule Registers */
#define SWITCH_PAGE_A1_UDF_0_D_A       0x0b00a1aa /* UDF 10 for IPv6 Chain Rule Registers */
#define SWITCH_PAGE_A1_UDF_0_D_B       0x0b00a1ab /* UDF 12 for IPv6 Chain Rule Registers */
#define SWITCH_PAGE_B0_ARL_TCAM_ACC    0x0b00b000 /* ARL TCAM and Secondary SRAM Access Register */
#define SWITCH_PAGE_B0_ARL_TCAM_DATA_0 0x0b00b004 /* ARL TCAM Data Register */
#define SWITCH_PAGE_B0_ARL_TCAM_DATA_1 0x0b00b008 /* ARL TCAM Data Register */
#define SWITCH_PAGE_B0_ARL_SMEM_DATA   0x0b00b014 /* ARL SMEM Data Register */
#define SWITCH_PAGE_B0_ARL_TCAM_BIST_CTRL 0x0b00b020 /* TCAM BIST Control Registers (Not2Release) */
#define SWITCH_PAGE_B0_ARL_TCAM_BIST_STS 0x0b00b024 /* TCAM BIST Status Registers (Not2Release) */

/****************************************************************************
 * bcm89530_sys_cfg_ETH
 ***************************************************************************/
/****************************************************************************
 * ETH :: eth_ctrl
 ***************************************************************************/
/* ETH :: eth_ctrl :: reserved0 [31:16] */
#define ETH_ETH_CTRL_RESERVED0_MASK                                0xffff0000
#define ETH_ETH_CTRL_RESERVED0_ALIGN                               0
#define ETH_ETH_CTRL_RESERVED0_BITS                                16
#define ETH_ETH_CTRL_RESERVED0_SHIFT                               16

/* ETH :: eth_ctrl :: oobbpe [15:15] */
#define Wr_ETH_eth_ctrl_oobbpe(x) WriteRegBits(ETH_ETH_CTRL,0x8000,15,x)
#define Rd_ETH_eth_ctrl_oobbpe(x) ReadRegBits(ETH_ETH_CTRL,0x8000,15)
#define ETH_ETH_CTRL_OOBBPE_MASK                                   0x00008000
#define ETH_ETH_CTRL_OOBBPE_ALIGN                                  0
#define ETH_ETH_CTRL_OOBBPE_BITS                                   1
#define ETH_ETH_CTRL_OOBBPE_SHIFT                                  15

/* ETH :: eth_ctrl :: be [14:14] */
#define Wr_ETH_eth_ctrl_be(x) WriteRegBits(ETH_ETH_CTRL,0x4000,14,x)
#define Rd_ETH_eth_ctrl_be(x) ReadRegBits(ETH_ETH_CTRL,0x4000,14)
#define ETH_ETH_CTRL_BE_MASK                                       0x00004000
#define ETH_ETH_CTRL_BE_ALIGN                                      0
#define ETH_ETH_CTRL_BE_BITS                                       1
#define ETH_ETH_CTRL_BE_SHIFT                                      14

/* ETH :: eth_ctrl :: fbp [13:13] */
#define Wr_ETH_eth_ctrl_fbp(x) WriteRegBits(ETH_ETH_CTRL,0x2000,13,x)
#define Rd_ETH_eth_ctrl_fbp(x) ReadRegBits(ETH_ETH_CTRL,0x2000,13)
#define ETH_ETH_CTRL_FBP_MASK                                      0x00002000
#define ETH_ETH_CTRL_FBP_ALIGN                                     0
#define ETH_ETH_CTRL_FBP_BITS                                      1
#define ETH_ETH_CTRL_FBP_SHIFT                                     13

/* ETH :: eth_ctrl :: bcr [12:12] */
#define Wr_ETH_eth_ctrl_bcr(x) WriteRegBits(ETH_ETH_CTRL,0x1000,12,x)
#define Rd_ETH_eth_ctrl_bcr(x) ReadRegBits(ETH_ETH_CTRL,0x1000,12)
#define ETH_ETH_CTRL_BCR_MASK                                      0x00001000
#define ETH_ETH_CTRL_BCR_ALIGN                                     0
#define ETH_ETH_CTRL_BCR_BITS                                      1
#define ETH_ETH_CTRL_BCR_SHIFT                                     12

/* ETH :: eth_ctrl :: reserved1 [11:11] */
#define ETH_ETH_CTRL_RESERVED1_MASK                                0x00000800
#define ETH_ETH_CTRL_RESERVED1_ALIGN                               0
#define ETH_ETH_CTRL_RESERVED1_BITS                                1
#define ETH_ETH_CTRL_RESERVED1_SHIFT                               11

/* ETH :: eth_ctrl :: led1 [10:08] */
#define Wr_ETH_eth_ctrl_led1(x) WriteRegBits(ETH_ETH_CTRL,0x700,8,x)
#define Rd_ETH_eth_ctrl_led1(x) ReadRegBits(ETH_ETH_CTRL,0x700,8)
#define ETH_ETH_CTRL_LED1_MASK                                     0x00000700
#define ETH_ETH_CTRL_LED1_ALIGN                                    0
#define ETH_ETH_CTRL_LED1_BITS                                     3
#define ETH_ETH_CTRL_LED1_SHIFT                                    8

/* ETH :: eth_ctrl :: reserved2 [07:07] */
#define ETH_ETH_CTRL_RESERVED2_MASK                                0x00000080
#define ETH_ETH_CTRL_RESERVED2_ALIGN                               0
#define ETH_ETH_CTRL_RESERVED2_BITS                                1
#define ETH_ETH_CTRL_RESERVED2_SHIFT                               7

/* ETH :: eth_ctrl :: led0 [06:04] */
#define Wr_ETH_eth_ctrl_led0(x) WriteRegBits(ETH_ETH_CTRL,0x70,4,x)
#define Rd_ETH_eth_ctrl_led0(x) ReadRegBits(ETH_ETH_CTRL,0x70,4)
#define ETH_ETH_CTRL_LED0_MASK                                     0x00000070
#define ETH_ETH_CTRL_LED0_ALIGN                                    0
#define ETH_ETH_CTRL_LED0_BITS                                     3
#define ETH_ETH_CTRL_LED0_SHIFT                                    4

/* ETH :: eth_ctrl :: men [03:03] */
#define Wr_ETH_eth_ctrl_men(x) WriteRegBits(ETH_ETH_CTRL,0x8,3,x)
#define Rd_ETH_eth_ctrl_men(x) ReadRegBits(ETH_ETH_CTRL,0x8,3)
#define ETH_ETH_CTRL_MEN_MASK                                      0x00000008
#define ETH_ETH_CTRL_MEN_ALIGN                                     0
#define ETH_ETH_CTRL_MEN_BITS                                      1
#define ETH_ETH_CTRL_MEN_SHIFT                                     3

/* ETH :: eth_ctrl :: clronrd [02:02] */
#define Wr_ETH_eth_ctrl_clronrd(x) WriteRegBits(ETH_ETH_CTRL,0x4,2,x)
#define Rd_ETH_eth_ctrl_clronrd(x) ReadRegBits(ETH_ETH_CTRL,0x4,2)
#define ETH_ETH_CTRL_CLRONRD_MASK                                  0x00000004
#define ETH_ETH_CTRL_CLRONRD_ALIGN                                 0
#define ETH_ETH_CTRL_CLRONRD_BITS                                  1
#define ETH_ETH_CTRL_CLRONRD_SHIFT                                 2

/* ETH :: eth_ctrl :: grs [01:01] */
#define Wr_ETH_eth_ctrl_grs(x) WriteRegBits(ETH_ETH_CTRL,0x2,1,x)
#define Rd_ETH_eth_ctrl_grs(x) ReadRegBits(ETH_ETH_CTRL,0x2,1)
#define ETH_ETH_CTRL_GRS_MASK                                      0x00000002
#define ETH_ETH_CTRL_GRS_ALIGN                                     0
#define ETH_ETH_CTRL_GRS_BITS                                      1
#define ETH_ETH_CTRL_GRS_SHIFT                                     1

/* ETH :: eth_ctrl :: gts [00:00] */
#define Wr_ETH_eth_ctrl_gts(x) WriteRegBits(ETH_ETH_CTRL,0x1,0,x)
#define Rd_ETH_eth_ctrl_gts(x) ReadRegBits(ETH_ETH_CTRL,0x1,0)
#define ETH_ETH_CTRL_GTS_MASK                                      0x00000001
#define ETH_ETH_CTRL_GTS_ALIGN                                     0
#define ETH_ETH_CTRL_GTS_BITS                                      1
#define ETH_ETH_CTRL_GTS_SHIFT                                     0


/****************************************************************************
 * ETH :: intr_mask
 ***************************************************************************/
/* ETH :: intr_mask :: reserved0 [31:17] */
#define ETH_INTR_MASK_RESERVED0_MASK                               0xfffe0000
#define ETH_INTR_MASK_RESERVED0_ALIGN                              0
#define ETH_INTR_MASK_RESERVED0_BITS                               15
#define ETH_INTR_MASK_RESERVED0_SHIFT                              17

/* ETH :: intr_mask :: phy_mask [16:16] */
#define Wr_ETH_intr_mask_phy_mask(x) WriteRegBits(ETH_INTR_MASK,0x10000,16,x)
#define Rd_ETH_intr_mask_phy_mask(x) ReadRegBits(ETH_INTR_MASK,0x10000,16)
#define ETH_INTR_MASK_PHY_MASK_MASK                                0x00010000
#define ETH_INTR_MASK_PHY_MASK_ALIGN                               0
#define ETH_INTR_MASK_PHY_MASK_BITS                                1
#define ETH_INTR_MASK_PHY_MASK_SHIFT                               16

/* ETH :: intr_mask :: rthr_mask [15:15] */
#define Wr_ETH_intr_mask_rthr_mask(x) WriteRegBits(ETH_INTR_MASK,0x8000,15,x)
#define Rd_ETH_intr_mask_rthr_mask(x) ReadRegBits(ETH_INTR_MASK,0x8000,15)
#define ETH_INTR_MASK_RTHR_MASK_MASK                               0x00008000
#define ETH_INTR_MASK_RTHR_MASK_ALIGN                              0
#define ETH_INTR_MASK_RTHR_MASK_BITS                               1
#define ETH_INTR_MASK_RTHR_MASK_SHIFT                              15

/* ETH :: intr_mask :: tthr_mask [14:14] */
#define Wr_ETH_intr_mask_tthr_mask(x) WriteRegBits(ETH_INTR_MASK,0x4000,14,x)
#define Rd_ETH_intr_mask_tthr_mask(x) ReadRegBits(ETH_INTR_MASK,0x4000,14)
#define ETH_INTR_MASK_TTHR_MASK_MASK                               0x00004000
#define ETH_INTR_MASK_TTHR_MASK_ALIGN                              0
#define ETH_INTR_MASK_TTHR_MASK_BITS                               1
#define ETH_INTR_MASK_TTHR_MASK_SHIFT                              14

/* ETH :: intr_mask :: rhlt_mask [13:13] */
#define Wr_ETH_intr_mask_rhlt_mask(x) WriteRegBits(ETH_INTR_MASK,0x2000,13,x)
#define Rd_ETH_intr_mask_rhlt_mask(x) ReadRegBits(ETH_INTR_MASK,0x2000,13)
#define ETH_INTR_MASK_RHLT_MASK_MASK                               0x00002000
#define ETH_INTR_MASK_RHLT_MASK_ALIGN                              0
#define ETH_INTR_MASK_RHLT_MASK_BITS                               1
#define ETH_INTR_MASK_RHLT_MASK_SHIFT                              13

/* ETH :: intr_mask :: thlt_mask [12:12] */
#define Wr_ETH_intr_mask_thlt_mask(x) WriteRegBits(ETH_INTR_MASK,0x1000,12,x)
#define Rd_ETH_intr_mask_thlt_mask(x) ReadRegBits(ETH_INTR_MASK,0x1000,12)
#define ETH_INTR_MASK_THLT_MASK_MASK                               0x00001000
#define ETH_INTR_MASK_THLT_MASK_ALIGN                              0
#define ETH_INTR_MASK_THLT_MASK_BITS                               1
#define ETH_INTR_MASK_THLT_MASK_SHIFT                              12

/* ETH :: intr_mask :: rov_mask [11:11] */
#define Wr_ETH_intr_mask_rov_mask(x) WriteRegBits(ETH_INTR_MASK,0x800,11,x)
#define Rd_ETH_intr_mask_rov_mask(x) ReadRegBits(ETH_INTR_MASK,0x800,11)
#define ETH_INTR_MASK_ROV_MASK_MASK                                0x00000800
#define ETH_INTR_MASK_ROV_MASK_ALIGN                               0
#define ETH_INTR_MASK_ROV_MASK_BITS                                1
#define ETH_INTR_MASK_ROV_MASK_SHIFT                               11

/* ETH :: intr_mask :: tun_mask [10:10] */
#define Wr_ETH_intr_mask_tun_mask(x) WriteRegBits(ETH_INTR_MASK,0x400,10,x)
#define Rd_ETH_intr_mask_tun_mask(x) ReadRegBits(ETH_INTR_MASK,0x400,10)
#define ETH_INTR_MASK_TUN_MASK_MASK                                0x00000400
#define ETH_INTR_MASK_TUN_MASK_ALIGN                               0
#define ETH_INTR_MASK_TUN_MASK_BITS                                1
#define ETH_INTR_MASK_TUN_MASK_SHIFT                               10

/* ETH :: intr_mask :: tec_mask [09:09] */
#define Wr_ETH_intr_mask_tec_mask(x) WriteRegBits(ETH_INTR_MASK,0x200,9,x)
#define Rd_ETH_intr_mask_tec_mask(x) ReadRegBits(ETH_INTR_MASK,0x200,9)
#define ETH_INTR_MASK_TEC_MASK_MASK                                0x00000200
#define ETH_INTR_MASK_TEC_MASK_ALIGN                               0
#define ETH_INTR_MASK_TEC_MASK_BITS                                1
#define ETH_INTR_MASK_TEC_MASK_SHIFT                               9

/* ETH :: intr_mask :: tlc_mask [08:08] */
#define Wr_ETH_intr_mask_tlc_mask(x) WriteRegBits(ETH_INTR_MASK,0x100,8,x)
#define Rd_ETH_intr_mask_tlc_mask(x) ReadRegBits(ETH_INTR_MASK,0x100,8)
#define ETH_INTR_MASK_TLC_MASK_MASK                                0x00000100
#define ETH_INTR_MASK_TLC_MASK_ALIGN                               0
#define ETH_INTR_MASK_TLC_MASK_BITS                                1
#define ETH_INTR_MASK_TLC_MASK_SHIFT                               8

/* ETH :: intr_mask :: rxb_mask [07:07] */
#define Wr_ETH_intr_mask_rxb_mask(x) WriteRegBits(ETH_INTR_MASK,0x80,7,x)
#define Rd_ETH_intr_mask_rxb_mask(x) ReadRegBits(ETH_INTR_MASK,0x80,7)
#define ETH_INTR_MASK_RXB_MASK_MASK                                0x00000080
#define ETH_INTR_MASK_RXB_MASK_ALIGN                               0
#define ETH_INTR_MASK_RXB_MASK_BITS                                1
#define ETH_INTR_MASK_RXB_MASK_SHIFT                               7

/* ETH :: intr_mask :: txb_mask [06:06] */
#define Wr_ETH_intr_mask_txb_mask(x) WriteRegBits(ETH_INTR_MASK,0x40,6,x)
#define Rd_ETH_intr_mask_txb_mask(x) ReadRegBits(ETH_INTR_MASK,0x40,6)
#define ETH_INTR_MASK_TXB_MASK_MASK                                0x00000040
#define ETH_INTR_MASK_TXB_MASK_ALIGN                               0
#define ETH_INTR_MASK_TXB_MASK_BITS                                1
#define ETH_INTR_MASK_TXB_MASK_SHIFT                               6

/* ETH :: intr_mask :: rxf_mask [05:05] */
#define Wr_ETH_intr_mask_rxf_mask(x) WriteRegBits(ETH_INTR_MASK,0x20,5,x)
#define Rd_ETH_intr_mask_rxf_mask(x) ReadRegBits(ETH_INTR_MASK,0x20,5)
#define ETH_INTR_MASK_RXF_MASK_MASK                                0x00000020
#define ETH_INTR_MASK_RXF_MASK_ALIGN                               0
#define ETH_INTR_MASK_RXF_MASK_BITS                                1
#define ETH_INTR_MASK_RXF_MASK_SHIFT                               5

/* ETH :: intr_mask :: txf_mask [04:04] */
#define Wr_ETH_intr_mask_txf_mask(x) WriteRegBits(ETH_INTR_MASK,0x10,4,x)
#define Rd_ETH_intr_mask_txf_mask(x) ReadRegBits(ETH_INTR_MASK,0x10,4)
#define ETH_INTR_MASK_TXF_MASK_MASK                                0x00000010
#define ETH_INTR_MASK_TXF_MASK_ALIGN                               0
#define ETH_INTR_MASK_TXF_MASK_BITS                                1
#define ETH_INTR_MASK_TXF_MASK_SHIFT                               4

/* ETH :: intr_mask :: berr_mask [03:03] */
#define Wr_ETH_intr_mask_berr_mask(x) WriteRegBits(ETH_INTR_MASK,0x8,3,x)
#define Rd_ETH_intr_mask_berr_mask(x) ReadRegBits(ETH_INTR_MASK,0x8,3)
#define ETH_INTR_MASK_BERR_MASK_MASK                               0x00000008
#define ETH_INTR_MASK_BERR_MASK_ALIGN                              0
#define ETH_INTR_MASK_BERR_MASK_BITS                               1
#define ETH_INTR_MASK_BERR_MASK_SHIFT                              3

/* ETH :: intr_mask :: reserved1 [02:02] */
#define ETH_INTR_MASK_RESERVED1_MASK                               0x00000004
#define ETH_INTR_MASK_RESERVED1_ALIGN                              0
#define ETH_INTR_MASK_RESERVED1_BITS                               1
#define ETH_INTR_MASK_RESERVED1_SHIFT                              2

/* ETH :: intr_mask :: grsc_mask [01:01] */
#define Wr_ETH_intr_mask_grsc_mask(x) WriteRegBits(ETH_INTR_MASK,0x2,1,x)
#define Rd_ETH_intr_mask_grsc_mask(x) ReadRegBits(ETH_INTR_MASK,0x2,1)
#define ETH_INTR_MASK_GRSC_MASK_MASK                               0x00000002
#define ETH_INTR_MASK_GRSC_MASK_ALIGN                              0
#define ETH_INTR_MASK_GRSC_MASK_BITS                               1
#define ETH_INTR_MASK_GRSC_MASK_SHIFT                              1

/* ETH :: intr_mask :: gtsc_mask [00:00] */
#define Wr_ETH_intr_mask_gtsc_mask(x) WriteRegBits(ETH_INTR_MASK,0x1,0,x)
#define Rd_ETH_intr_mask_gtsc_mask(x) ReadRegBits(ETH_INTR_MASK,0x1,0)
#define ETH_INTR_MASK_GTSC_MASK_MASK                               0x00000001
#define ETH_INTR_MASK_GTSC_MASK_ALIGN                              0
#define ETH_INTR_MASK_GTSC_MASK_BITS                               1
#define ETH_INTR_MASK_GTSC_MASK_SHIFT                              0


/****************************************************************************
 * ETH :: intr
 ***************************************************************************/
/* ETH :: intr :: reserved0 [31:17] */
#define ETH_INTR_RESERVED0_MASK                                    0xfffe0000
#define ETH_INTR_RESERVED0_ALIGN                                   0
#define ETH_INTR_RESERVED0_BITS                                    15
#define ETH_INTR_RESERVED0_SHIFT                                   17

/* ETH :: intr :: phy [16:16] */
#define Wr_ETH_intr_phy(x) WriteRegBits(ETH_INTR,0x10000,16,x)
#define Rd_ETH_intr_phy(x) ReadRegBits(ETH_INTR,0x10000,16)
#define ETH_INTR_PHY_MASK                                          0x00010000
#define ETH_INTR_PHY_ALIGN                                         0
#define ETH_INTR_PHY_BITS                                          1
#define ETH_INTR_PHY_SHIFT                                         16

/* ETH :: intr :: rthr [15:15] */
#define Wr_ETH_intr_rthr(x) WriteRegBits(ETH_INTR,0x8000,15,x)
#define Rd_ETH_intr_rthr(x) ReadRegBits(ETH_INTR,0x8000,15)
#define ETH_INTR_RTHR_MASK                                         0x00008000
#define ETH_INTR_RTHR_ALIGN                                        0
#define ETH_INTR_RTHR_BITS                                         1
#define ETH_INTR_RTHR_SHIFT                                        15

/* ETH :: intr :: tthr [14:14] */
#define Wr_ETH_intr_tthr(x) WriteRegBits(ETH_INTR,0x4000,14,x)
#define Rd_ETH_intr_tthr(x) ReadRegBits(ETH_INTR,0x4000,14)
#define ETH_INTR_TTHR_MASK                                         0x00004000
#define ETH_INTR_TTHR_ALIGN                                        0
#define ETH_INTR_TTHR_BITS                                         1
#define ETH_INTR_TTHR_SHIFT                                        14

/* ETH :: intr :: rhlt [13:13] */
#define Wr_ETH_intr_rhlt(x) WriteRegBits(ETH_INTR,0x2000,13,x)
#define Rd_ETH_intr_rhlt(x) ReadRegBits(ETH_INTR,0x2000,13)
#define ETH_INTR_RHLT_MASK                                         0x00002000
#define ETH_INTR_RHLT_ALIGN                                        0
#define ETH_INTR_RHLT_BITS                                         1
#define ETH_INTR_RHLT_SHIFT                                        13

/* ETH :: intr :: thlt [12:12] */
#define Wr_ETH_intr_thlt(x) WriteRegBits(ETH_INTR,0x1000,12,x)
#define Rd_ETH_intr_thlt(x) ReadRegBits(ETH_INTR,0x1000,12)
#define ETH_INTR_THLT_MASK                                         0x00001000
#define ETH_INTR_THLT_ALIGN                                        0
#define ETH_INTR_THLT_BITS                                         1
#define ETH_INTR_THLT_SHIFT                                        12

/* ETH :: intr :: rov [11:11] */
#define Wr_ETH_intr_rov(x) WriteRegBits(ETH_INTR,0x800,11,x)
#define Rd_ETH_intr_rov(x) ReadRegBits(ETH_INTR,0x800,11)
#define ETH_INTR_ROV_MASK                                          0x00000800
#define ETH_INTR_ROV_ALIGN                                         0
#define ETH_INTR_ROV_BITS                                          1
#define ETH_INTR_ROV_SHIFT                                         11

/* ETH :: intr :: tun [10:10] */
#define Wr_ETH_intr_tun(x) WriteRegBits(ETH_INTR,0x400,10,x)
#define Rd_ETH_intr_tun(x) ReadRegBits(ETH_INTR,0x400,10)
#define ETH_INTR_TUN_MASK                                          0x00000400
#define ETH_INTR_TUN_ALIGN                                         0
#define ETH_INTR_TUN_BITS                                          1
#define ETH_INTR_TUN_SHIFT                                         10

/* ETH :: intr :: tec [09:09] */
#define Wr_ETH_intr_tec(x) WriteRegBits(ETH_INTR,0x200,9,x)
#define Rd_ETH_intr_tec(x) ReadRegBits(ETH_INTR,0x200,9)
#define ETH_INTR_TEC_MASK                                          0x00000200
#define ETH_INTR_TEC_ALIGN                                         0
#define ETH_INTR_TEC_BITS                                          1
#define ETH_INTR_TEC_SHIFT                                         9

/* ETH :: intr :: tlc [08:08] */
#define Wr_ETH_intr_tlc(x) WriteRegBits(ETH_INTR,0x100,8,x)
#define Rd_ETH_intr_tlc(x) ReadRegBits(ETH_INTR,0x100,8)
#define ETH_INTR_TLC_MASK                                          0x00000100
#define ETH_INTR_TLC_ALIGN                                         0
#define ETH_INTR_TLC_BITS                                          1
#define ETH_INTR_TLC_SHIFT                                         8

/* ETH :: intr :: rxb [07:07] */
#define Wr_ETH_intr_rxb(x) WriteRegBits(ETH_INTR,0x80,7,x)
#define Rd_ETH_intr_rxb(x) ReadRegBits(ETH_INTR,0x80,7)
#define ETH_INTR_RXB_MASK                                          0x00000080
#define ETH_INTR_RXB_ALIGN                                         0
#define ETH_INTR_RXB_BITS                                          1
#define ETH_INTR_RXB_SHIFT                                         7

/* ETH :: intr :: txb [06:06] */
#define Wr_ETH_intr_txb(x) WriteRegBits(ETH_INTR,0x40,6,x)
#define Rd_ETH_intr_txb(x) ReadRegBits(ETH_INTR,0x40,6)
#define ETH_INTR_TXB_MASK                                          0x00000040
#define ETH_INTR_TXB_ALIGN                                         0
#define ETH_INTR_TXB_BITS                                          1
#define ETH_INTR_TXB_SHIFT                                         6

/* ETH :: intr :: rxf [05:05] */
#define Wr_ETH_intr_rxf(x) WriteRegBits(ETH_INTR,0x20,5,x)
#define Rd_ETH_intr_rxf(x) ReadRegBits(ETH_INTR,0x20,5)
#define ETH_INTR_RXF_MASK                                          0x00000020
#define ETH_INTR_RXF_ALIGN                                         0
#define ETH_INTR_RXF_BITS                                          1
#define ETH_INTR_RXF_SHIFT                                         5

/* ETH :: intr :: txf [04:04] */
#define Wr_ETH_intr_txf(x) WriteRegBits(ETH_INTR,0x10,4,x)
#define Rd_ETH_intr_txf(x) ReadRegBits(ETH_INTR,0x10,4)
#define ETH_INTR_TXF_MASK                                          0x00000010
#define ETH_INTR_TXF_ALIGN                                         0
#define ETH_INTR_TXF_BITS                                          1
#define ETH_INTR_TXF_SHIFT                                         4

/* ETH :: intr :: berr [03:03] */
#define Wr_ETH_intr_berr(x) WriteRegBits(ETH_INTR,0x8,3,x)
#define Rd_ETH_intr_berr(x) ReadRegBits(ETH_INTR,0x8,3)
#define ETH_INTR_BERR_MASK                                         0x00000008
#define ETH_INTR_BERR_ALIGN                                        0
#define ETH_INTR_BERR_BITS                                         1
#define ETH_INTR_BERR_SHIFT                                        3

/* ETH :: intr :: reserved1 [02:02] */
#define ETH_INTR_RESERVED1_MASK                                    0x00000004
#define ETH_INTR_RESERVED1_ALIGN                                   0
#define ETH_INTR_RESERVED1_BITS                                    1
#define ETH_INTR_RESERVED1_SHIFT                                   2

/* ETH :: intr :: grsc [01:01] */
#define Wr_ETH_intr_grsc(x) WriteRegBits(ETH_INTR,0x2,1,x)
#define Rd_ETH_intr_grsc(x) ReadRegBits(ETH_INTR,0x2,1)
#define ETH_INTR_GRSC_MASK                                         0x00000002
#define ETH_INTR_GRSC_ALIGN                                        0
#define ETH_INTR_GRSC_BITS                                         1
#define ETH_INTR_GRSC_SHIFT                                        1

/* ETH :: intr :: gtsc [00:00] */
#define Wr_ETH_intr_gtsc(x) WriteRegBits(ETH_INTR,0x1,0,x)
#define Rd_ETH_intr_gtsc(x) ReadRegBits(ETH_INTR,0x1,0)
#define ETH_INTR_GTSC_MASK                                         0x00000001
#define ETH_INTR_GTSC_ALIGN                                        0
#define ETH_INTR_GTSC_BITS                                         1
#define ETH_INTR_GTSC_SHIFT                                        0


/****************************************************************************
 * ETH :: intr_raw
 ***************************************************************************/
/* ETH :: intr_raw :: reserved0 [31:17] */
#define ETH_INTR_RAW_RESERVED0_MASK                                0xfffe0000
#define ETH_INTR_RAW_RESERVED0_ALIGN                               0
#define ETH_INTR_RAW_RESERVED0_BITS                                15
#define ETH_INTR_RAW_RESERVED0_SHIFT                               17

/* ETH :: intr_raw :: phy_raw [16:16] */
#define Wr_ETH_intr_raw_phy_raw(x) WriteRegBits(ETH_INTR_RAW,0x10000,16,x)
#define Rd_ETH_intr_raw_phy_raw(x) ReadRegBits(ETH_INTR_RAW,0x10000,16)
#define ETH_INTR_RAW_PHY_RAW_MASK                                  0x00010000
#define ETH_INTR_RAW_PHY_RAW_ALIGN                                 0
#define ETH_INTR_RAW_PHY_RAW_BITS                                  1
#define ETH_INTR_RAW_PHY_RAW_SHIFT                                 16

/* ETH :: intr_raw :: rthr_raw [15:15] */
#define Wr_ETH_intr_raw_rthr_raw(x) WriteRegBits(ETH_INTR_RAW,0x8000,15,x)
#define Rd_ETH_intr_raw_rthr_raw(x) ReadRegBits(ETH_INTR_RAW,0x8000,15)
#define ETH_INTR_RAW_RTHR_RAW_MASK                                 0x00008000
#define ETH_INTR_RAW_RTHR_RAW_ALIGN                                0
#define ETH_INTR_RAW_RTHR_RAW_BITS                                 1
#define ETH_INTR_RAW_RTHR_RAW_SHIFT                                15

/* ETH :: intr_raw :: tthr_raw [14:14] */
#define Wr_ETH_intr_raw_tthr_raw(x) WriteRegBits(ETH_INTR_RAW,0x4000,14,x)
#define Rd_ETH_intr_raw_tthr_raw(x) ReadRegBits(ETH_INTR_RAW,0x4000,14)
#define ETH_INTR_RAW_TTHR_RAW_MASK                                 0x00004000
#define ETH_INTR_RAW_TTHR_RAW_ALIGN                                0
#define ETH_INTR_RAW_TTHR_RAW_BITS                                 1
#define ETH_INTR_RAW_TTHR_RAW_SHIFT                                14

/* ETH :: intr_raw :: rhlt_raw [13:13] */
#define Wr_ETH_intr_raw_rhlt_raw(x) WriteRegBits(ETH_INTR_RAW,0x2000,13,x)
#define Rd_ETH_intr_raw_rhlt_raw(x) ReadRegBits(ETH_INTR_RAW,0x2000,13)
#define ETH_INTR_RAW_RHLT_RAW_MASK                                 0x00002000
#define ETH_INTR_RAW_RHLT_RAW_ALIGN                                0
#define ETH_INTR_RAW_RHLT_RAW_BITS                                 1
#define ETH_INTR_RAW_RHLT_RAW_SHIFT                                13

/* ETH :: intr_raw :: thlt_raw [12:12] */
#define Wr_ETH_intr_raw_thlt_raw(x) WriteRegBits(ETH_INTR_RAW,0x1000,12,x)
#define Rd_ETH_intr_raw_thlt_raw(x) ReadRegBits(ETH_INTR_RAW,0x1000,12)
#define ETH_INTR_RAW_THLT_RAW_MASK                                 0x00001000
#define ETH_INTR_RAW_THLT_RAW_ALIGN                                0
#define ETH_INTR_RAW_THLT_RAW_BITS                                 1
#define ETH_INTR_RAW_THLT_RAW_SHIFT                                12

/* ETH :: intr_raw :: rov_raw [11:11] */
#define Wr_ETH_intr_raw_rov_raw(x) WriteRegBits(ETH_INTR_RAW,0x800,11,x)
#define Rd_ETH_intr_raw_rov_raw(x) ReadRegBits(ETH_INTR_RAW,0x800,11)
#define ETH_INTR_RAW_ROV_RAW_MASK                                  0x00000800
#define ETH_INTR_RAW_ROV_RAW_ALIGN                                 0
#define ETH_INTR_RAW_ROV_RAW_BITS                                  1
#define ETH_INTR_RAW_ROV_RAW_SHIFT                                 11

/* ETH :: intr_raw :: tun_raw [10:10] */
#define Wr_ETH_intr_raw_tun_raw(x) WriteRegBits(ETH_INTR_RAW,0x400,10,x)
#define Rd_ETH_intr_raw_tun_raw(x) ReadRegBits(ETH_INTR_RAW,0x400,10)
#define ETH_INTR_RAW_TUN_RAW_MASK                                  0x00000400
#define ETH_INTR_RAW_TUN_RAW_ALIGN                                 0
#define ETH_INTR_RAW_TUN_RAW_BITS                                  1
#define ETH_INTR_RAW_TUN_RAW_SHIFT                                 10

/* ETH :: intr_raw :: tec_raw [09:09] */
#define Wr_ETH_intr_raw_tec_raw(x) WriteRegBits(ETH_INTR_RAW,0x200,9,x)
#define Rd_ETH_intr_raw_tec_raw(x) ReadRegBits(ETH_INTR_RAW,0x200,9)
#define ETH_INTR_RAW_TEC_RAW_MASK                                  0x00000200
#define ETH_INTR_RAW_TEC_RAW_ALIGN                                 0
#define ETH_INTR_RAW_TEC_RAW_BITS                                  1
#define ETH_INTR_RAW_TEC_RAW_SHIFT                                 9

/* ETH :: intr_raw :: tlc_raw [08:08] */
#define Wr_ETH_intr_raw_tlc_raw(x) WriteRegBits(ETH_INTR_RAW,0x100,8,x)
#define Rd_ETH_intr_raw_tlc_raw(x) ReadRegBits(ETH_INTR_RAW,0x100,8)
#define ETH_INTR_RAW_TLC_RAW_MASK                                  0x00000100
#define ETH_INTR_RAW_TLC_RAW_ALIGN                                 0
#define ETH_INTR_RAW_TLC_RAW_BITS                                  1
#define ETH_INTR_RAW_TLC_RAW_SHIFT                                 8

/* ETH :: intr_raw :: rxb_raw [07:07] */
#define Wr_ETH_intr_raw_rxb_raw(x) WriteRegBits(ETH_INTR_RAW,0x80,7,x)
#define Rd_ETH_intr_raw_rxb_raw(x) ReadRegBits(ETH_INTR_RAW,0x80,7)
#define ETH_INTR_RAW_RXB_RAW_MASK                                  0x00000080
#define ETH_INTR_RAW_RXB_RAW_ALIGN                                 0
#define ETH_INTR_RAW_RXB_RAW_BITS                                  1
#define ETH_INTR_RAW_RXB_RAW_SHIFT                                 7

/* ETH :: intr_raw :: txb_raw [06:06] */
#define Wr_ETH_intr_raw_txb_raw(x) WriteRegBits(ETH_INTR_RAW,0x40,6,x)
#define Rd_ETH_intr_raw_txb_raw(x) ReadRegBits(ETH_INTR_RAW,0x40,6)
#define ETH_INTR_RAW_TXB_RAW_MASK                                  0x00000040
#define ETH_INTR_RAW_TXB_RAW_ALIGN                                 0
#define ETH_INTR_RAW_TXB_RAW_BITS                                  1
#define ETH_INTR_RAW_TXB_RAW_SHIFT                                 6

/* ETH :: intr_raw :: rxf_raw [05:05] */
#define Wr_ETH_intr_raw_rxf_raw(x) WriteRegBits(ETH_INTR_RAW,0x20,5,x)
#define Rd_ETH_intr_raw_rxf_raw(x) ReadRegBits(ETH_INTR_RAW,0x20,5)
#define ETH_INTR_RAW_RXF_RAW_MASK                                  0x00000020
#define ETH_INTR_RAW_RXF_RAW_ALIGN                                 0
#define ETH_INTR_RAW_RXF_RAW_BITS                                  1
#define ETH_INTR_RAW_RXF_RAW_SHIFT                                 5

/* ETH :: intr_raw :: txf_raw [04:04] */
#define Wr_ETH_intr_raw_txf_raw(x) WriteRegBits(ETH_INTR_RAW,0x10,4,x)
#define Rd_ETH_intr_raw_txf_raw(x) ReadRegBits(ETH_INTR_RAW,0x10,4)
#define ETH_INTR_RAW_TXF_RAW_MASK                                  0x00000010
#define ETH_INTR_RAW_TXF_RAW_ALIGN                                 0
#define ETH_INTR_RAW_TXF_RAW_BITS                                  1
#define ETH_INTR_RAW_TXF_RAW_SHIFT                                 4

/* ETH :: intr_raw :: berr_raw [03:03] */
#define Wr_ETH_intr_raw_berr_raw(x) WriteRegBits(ETH_INTR_RAW,0x8,3,x)
#define Rd_ETH_intr_raw_berr_raw(x) ReadRegBits(ETH_INTR_RAW,0x8,3)
#define ETH_INTR_RAW_BERR_RAW_MASK                                 0x00000008
#define ETH_INTR_RAW_BERR_RAW_ALIGN                                0
#define ETH_INTR_RAW_BERR_RAW_BITS                                 1
#define ETH_INTR_RAW_BERR_RAW_SHIFT                                3

/* ETH :: intr_raw :: reserved1 [02:02] */
#define ETH_INTR_RAW_RESERVED1_MASK                                0x00000004
#define ETH_INTR_RAW_RESERVED1_ALIGN                               0
#define ETH_INTR_RAW_RESERVED1_BITS                                1
#define ETH_INTR_RAW_RESERVED1_SHIFT                               2

/* ETH :: intr_raw :: grsc_raw [01:01] */
#define Wr_ETH_intr_raw_grsc_raw(x) WriteRegBits(ETH_INTR_RAW,0x2,1,x)
#define Rd_ETH_intr_raw_grsc_raw(x) ReadRegBits(ETH_INTR_RAW,0x2,1)
#define ETH_INTR_RAW_GRSC_RAW_MASK                                 0x00000002
#define ETH_INTR_RAW_GRSC_RAW_ALIGN                                0
#define ETH_INTR_RAW_GRSC_RAW_BITS                                 1
#define ETH_INTR_RAW_GRSC_RAW_SHIFT                                1

/* ETH :: intr_raw :: gtsc_raw [00:00] */
#define Wr_ETH_intr_raw_gtsc_raw(x) WriteRegBits(ETH_INTR_RAW,0x1,0,x)
#define Rd_ETH_intr_raw_gtsc_raw(x) ReadRegBits(ETH_INTR_RAW,0x1,0)
#define ETH_INTR_RAW_GTSC_RAW_MASK                                 0x00000001
#define ETH_INTR_RAW_GTSC_RAW_ALIGN                                0
#define ETH_INTR_RAW_GTSC_RAW_BITS                                 1
#define ETH_INTR_RAW_GTSC_RAW_SHIFT                                0


/****************************************************************************
 * ETH :: intr_clr
 ***************************************************************************/
/* ETH :: intr_clr :: reserved0 [31:17] */
#define ETH_INTR_CLR_RESERVED0_MASK                                0xfffe0000
#define ETH_INTR_CLR_RESERVED0_ALIGN                               0
#define ETH_INTR_CLR_RESERVED0_BITS                                15
#define ETH_INTR_CLR_RESERVED0_SHIFT                               17

/* ETH :: intr_clr :: phy_clr [16:16] */
#define Wr_ETH_intr_clr_phy_clr(x) WriteRegBits(ETH_INTR_CLR,0x10000,16,x)
#define Rd_ETH_intr_clr_phy_clr(x) ReadRegBits(ETH_INTR_CLR,0x10000,16)
#define ETH_INTR_CLR_PHY_CLR_MASK                                  0x00010000
#define ETH_INTR_CLR_PHY_CLR_ALIGN                                 0
#define ETH_INTR_CLR_PHY_CLR_BITS                                  1
#define ETH_INTR_CLR_PHY_CLR_SHIFT                                 16

/* ETH :: intr_clr :: rthr_clr [15:15] */
#define Wr_ETH_intr_clr_rthr_clr(x) WriteRegBits(ETH_INTR_CLR,0x8000,15,x)
#define Rd_ETH_intr_clr_rthr_clr(x) ReadRegBits(ETH_INTR_CLR,0x8000,15)
#define ETH_INTR_CLR_RTHR_CLR_MASK                                 0x00008000
#define ETH_INTR_CLR_RTHR_CLR_ALIGN                                0
#define ETH_INTR_CLR_RTHR_CLR_BITS                                 1
#define ETH_INTR_CLR_RTHR_CLR_SHIFT                                15

/* ETH :: intr_clr :: tthr_clr [14:14] */
#define Wr_ETH_intr_clr_tthr_clr(x) WriteRegBits(ETH_INTR_CLR,0x4000,14,x)
#define Rd_ETH_intr_clr_tthr_clr(x) ReadRegBits(ETH_INTR_CLR,0x4000,14)
#define ETH_INTR_CLR_TTHR_CLR_MASK                                 0x00004000
#define ETH_INTR_CLR_TTHR_CLR_ALIGN                                0
#define ETH_INTR_CLR_TTHR_CLR_BITS                                 1
#define ETH_INTR_CLR_TTHR_CLR_SHIFT                                14

/* ETH :: intr_clr :: rhlt_clr [13:13] */
#define Wr_ETH_intr_clr_rhlt_clr(x) WriteRegBits(ETH_INTR_CLR,0x2000,13,x)
#define Rd_ETH_intr_clr_rhlt_clr(x) ReadRegBits(ETH_INTR_CLR,0x2000,13)
#define ETH_INTR_CLR_RHLT_CLR_MASK                                 0x00002000
#define ETH_INTR_CLR_RHLT_CLR_ALIGN                                0
#define ETH_INTR_CLR_RHLT_CLR_BITS                                 1
#define ETH_INTR_CLR_RHLT_CLR_SHIFT                                13

/* ETH :: intr_clr :: thlt_clr [12:12] */
#define Wr_ETH_intr_clr_thlt_clr(x) WriteRegBits(ETH_INTR_CLR,0x1000,12,x)
#define Rd_ETH_intr_clr_thlt_clr(x) ReadRegBits(ETH_INTR_CLR,0x1000,12)
#define ETH_INTR_CLR_THLT_CLR_MASK                                 0x00001000
#define ETH_INTR_CLR_THLT_CLR_ALIGN                                0
#define ETH_INTR_CLR_THLT_CLR_BITS                                 1
#define ETH_INTR_CLR_THLT_CLR_SHIFT                                12

/* ETH :: intr_clr :: rov_clr [11:11] */
#define Wr_ETH_intr_clr_rov_clr(x) WriteRegBits(ETH_INTR_CLR,0x800,11,x)
#define Rd_ETH_intr_clr_rov_clr(x) ReadRegBits(ETH_INTR_CLR,0x800,11)
#define ETH_INTR_CLR_ROV_CLR_MASK                                  0x00000800
#define ETH_INTR_CLR_ROV_CLR_ALIGN                                 0
#define ETH_INTR_CLR_ROV_CLR_BITS                                  1
#define ETH_INTR_CLR_ROV_CLR_SHIFT                                 11

/* ETH :: intr_clr :: tun_clr [10:10] */
#define Wr_ETH_intr_clr_tun_clr(x) WriteRegBits(ETH_INTR_CLR,0x400,10,x)
#define Rd_ETH_intr_clr_tun_clr(x) ReadRegBits(ETH_INTR_CLR,0x400,10)
#define ETH_INTR_CLR_TUN_CLR_MASK                                  0x00000400
#define ETH_INTR_CLR_TUN_CLR_ALIGN                                 0
#define ETH_INTR_CLR_TUN_CLR_BITS                                  1
#define ETH_INTR_CLR_TUN_CLR_SHIFT                                 10

/* ETH :: intr_clr :: tec_clr [09:09] */
#define Wr_ETH_intr_clr_tec_clr(x) WriteRegBits(ETH_INTR_CLR,0x200,9,x)
#define Rd_ETH_intr_clr_tec_clr(x) ReadRegBits(ETH_INTR_CLR,0x200,9)
#define ETH_INTR_CLR_TEC_CLR_MASK                                  0x00000200
#define ETH_INTR_CLR_TEC_CLR_ALIGN                                 0
#define ETH_INTR_CLR_TEC_CLR_BITS                                  1
#define ETH_INTR_CLR_TEC_CLR_SHIFT                                 9

/* ETH :: intr_clr :: tlc_clr [08:08] */
#define Wr_ETH_intr_clr_tlc_clr(x) WriteRegBits(ETH_INTR_CLR,0x100,8,x)
#define Rd_ETH_intr_clr_tlc_clr(x) ReadRegBits(ETH_INTR_CLR,0x100,8)
#define ETH_INTR_CLR_TLC_CLR_MASK                                  0x00000100
#define ETH_INTR_CLR_TLC_CLR_ALIGN                                 0
#define ETH_INTR_CLR_TLC_CLR_BITS                                  1
#define ETH_INTR_CLR_TLC_CLR_SHIFT                                 8

/* ETH :: intr_clr :: rxb_clr [07:07] */
#define Wr_ETH_intr_clr_rxb_clr(x) WriteRegBits(ETH_INTR_CLR,0x80,7,x)
#define Rd_ETH_intr_clr_rxb_clr(x) ReadRegBits(ETH_INTR_CLR,0x80,7)
#define ETH_INTR_CLR_RXB_CLR_MASK                                  0x00000080
#define ETH_INTR_CLR_RXB_CLR_ALIGN                                 0
#define ETH_INTR_CLR_RXB_CLR_BITS                                  1
#define ETH_INTR_CLR_RXB_CLR_SHIFT                                 7

/* ETH :: intr_clr :: txb_clr [06:06] */
#define Wr_ETH_intr_clr_txb_clr(x) WriteRegBits(ETH_INTR_CLR,0x40,6,x)
#define Rd_ETH_intr_clr_txb_clr(x) ReadRegBits(ETH_INTR_CLR,0x40,6)
#define ETH_INTR_CLR_TXB_CLR_MASK                                  0x00000040
#define ETH_INTR_CLR_TXB_CLR_ALIGN                                 0
#define ETH_INTR_CLR_TXB_CLR_BITS                                  1
#define ETH_INTR_CLR_TXB_CLR_SHIFT                                 6

/* ETH :: intr_clr :: rxf_clr [05:05] */
#define Wr_ETH_intr_clr_rxf_clr(x) WriteRegBits(ETH_INTR_CLR,0x20,5,x)
#define Rd_ETH_intr_clr_rxf_clr(x) ReadRegBits(ETH_INTR_CLR,0x20,5)
#define ETH_INTR_CLR_RXF_CLR_MASK                                  0x00000020
#define ETH_INTR_CLR_RXF_CLR_ALIGN                                 0
#define ETH_INTR_CLR_RXF_CLR_BITS                                  1
#define ETH_INTR_CLR_RXF_CLR_SHIFT                                 5

/* ETH :: intr_clr :: txf_clr [04:04] */
#define Wr_ETH_intr_clr_txf_clr(x) WriteRegBits(ETH_INTR_CLR,0x10,4,x)
#define Rd_ETH_intr_clr_txf_clr(x) ReadRegBits(ETH_INTR_CLR,0x10,4)
#define ETH_INTR_CLR_TXF_CLR_MASK                                  0x00000010
#define ETH_INTR_CLR_TXF_CLR_ALIGN                                 0
#define ETH_INTR_CLR_TXF_CLR_BITS                                  1
#define ETH_INTR_CLR_TXF_CLR_SHIFT                                 4

/* ETH :: intr_clr :: berr_clr [03:03] */
#define Wr_ETH_intr_clr_berr_clr(x) WriteRegBits(ETH_INTR_CLR,0x8,3,x)
#define Rd_ETH_intr_clr_berr_clr(x) ReadRegBits(ETH_INTR_CLR,0x8,3)
#define ETH_INTR_CLR_BERR_CLR_MASK                                 0x00000008
#define ETH_INTR_CLR_BERR_CLR_ALIGN                                0
#define ETH_INTR_CLR_BERR_CLR_BITS                                 1
#define ETH_INTR_CLR_BERR_CLR_SHIFT                                3

/* ETH :: intr_clr :: reserved1 [02:02] */
#define ETH_INTR_CLR_RESERVED1_MASK                                0x00000004
#define ETH_INTR_CLR_RESERVED1_ALIGN                               0
#define ETH_INTR_CLR_RESERVED1_BITS                                1
#define ETH_INTR_CLR_RESERVED1_SHIFT                               2

/* ETH :: intr_clr :: grsc_clr [01:01] */
#define Wr_ETH_intr_clr_grsc_clr(x) WriteRegBits(ETH_INTR_CLR,0x2,1,x)
#define Rd_ETH_intr_clr_grsc_clr(x) ReadRegBits(ETH_INTR_CLR,0x2,1)
#define ETH_INTR_CLR_GRSC_CLR_MASK                                 0x00000002
#define ETH_INTR_CLR_GRSC_CLR_ALIGN                                0
#define ETH_INTR_CLR_GRSC_CLR_BITS                                 1
#define ETH_INTR_CLR_GRSC_CLR_SHIFT                                1

/* ETH :: intr_clr :: gtsc_clr [00:00] */
#define Wr_ETH_intr_clr_gtsc_clr(x) WriteRegBits(ETH_INTR_CLR,0x1,0,x)
#define Rd_ETH_intr_clr_gtsc_clr(x) ReadRegBits(ETH_INTR_CLR,0x1,0)
#define ETH_INTR_CLR_GTSC_CLR_MASK                                 0x00000001
#define ETH_INTR_CLR_GTSC_CLR_ALIGN                                0
#define ETH_INTR_CLR_GTSC_CLR_BITS                                 1
#define ETH_INTR_CLR_GTSC_CLR_SHIFT                                0


/****************************************************************************
 * ETH :: mcaddrf0
 ***************************************************************************/
/* ETH :: mcaddrf0 :: mcaddrf0 [31:00] */
#define Wr_ETH_mcaddrf0_mcaddrf0(x) WriteReg(ETH_MCADDRF0,x)
#define Rd_ETH_mcaddrf0_mcaddrf0(x) ReadReg(ETH_MCADDRF0)
#define ETH_MCADDRF0_MCADDRF0_MASK                                 0xffffffff
#define ETH_MCADDRF0_MCADDRF0_ALIGN                                0
#define ETH_MCADDRF0_MCADDRF0_BITS                                 32
#define ETH_MCADDRF0_MCADDRF0_SHIFT                                0


/****************************************************************************
 * ETH :: mcaddrf1
 ***************************************************************************/
/* ETH :: mcaddrf1 :: mcaddrf1 [31:00] */
#define Wr_ETH_mcaddrf1_mcaddrf1(x) WriteReg(ETH_MCADDRF1,x)
#define Rd_ETH_mcaddrf1_mcaddrf1(x) ReadReg(ETH_MCADDRF1)
#define ETH_MCADDRF1_MCADDRF1_MASK                                 0xffffffff
#define ETH_MCADDRF1_MCADDRF1_ALIGN                                0
#define ETH_MCADDRF1_MCADDRF1_BITS                                 32
#define ETH_MCADDRF1_MCADDRF1_SHIFT                                0


/****************************************************************************
 * ETH :: lsaddr0
 ***************************************************************************/
/* ETH :: lsaddr0 :: lssabyte0 [31:24] */
#define Wr_ETH_lsaddr0_lssabyte0(x) WriteRegBits(ETH_LSADDR0,0xff000000,24,x)
#define Rd_ETH_lsaddr0_lssabyte0(x) ReadRegBits(ETH_LSADDR0,0xff000000,24)
#define ETH_LSADDR0_LSSABYTE0_MASK                                 0xff000000
#define ETH_LSADDR0_LSSABYTE0_ALIGN                                0
#define ETH_LSADDR0_LSSABYTE0_BITS                                 8
#define ETH_LSADDR0_LSSABYTE0_SHIFT                                24

/* ETH :: lsaddr0 :: lssabyte1 [23:16] */
#define Wr_ETH_lsaddr0_lssabyte1(x) WriteRegBits(ETH_LSADDR0,0xff0000,16,x)
#define Rd_ETH_lsaddr0_lssabyte1(x) ReadRegBits(ETH_LSADDR0,0xff0000,16)
#define ETH_LSADDR0_LSSABYTE1_MASK                                 0x00ff0000
#define ETH_LSADDR0_LSSABYTE1_ALIGN                                0
#define ETH_LSADDR0_LSSABYTE1_BITS                                 8
#define ETH_LSADDR0_LSSABYTE1_SHIFT                                16

/* ETH :: lsaddr0 :: lssabyte2 [15:08] */
#define Wr_ETH_lsaddr0_lssabyte2(x) WriteRegBits(ETH_LSADDR0,0xff00,8,x)
#define Rd_ETH_lsaddr0_lssabyte2(x) ReadRegBits(ETH_LSADDR0,0xff00,8)
#define ETH_LSADDR0_LSSABYTE2_MASK                                 0x0000ff00
#define ETH_LSADDR0_LSSABYTE2_ALIGN                                0
#define ETH_LSADDR0_LSSABYTE2_BITS                                 8
#define ETH_LSADDR0_LSSABYTE2_SHIFT                                8

/* ETH :: lsaddr0 :: lssabyte3 [07:00] */
#define Wr_ETH_lsaddr0_lssabyte3(x) WriteRegBits(ETH_LSADDR0,0xff,0,x)
#define Rd_ETH_lsaddr0_lssabyte3(x) ReadRegBits(ETH_LSADDR0,0xff,0)
#define ETH_LSADDR0_LSSABYTE3_MASK                                 0x000000ff
#define ETH_LSADDR0_LSSABYTE3_ALIGN                                0
#define ETH_LSADDR0_LSSABYTE3_BITS                                 8
#define ETH_LSADDR0_LSSABYTE3_SHIFT                                0


/****************************************************************************
 * ETH :: lscaddr1
 ***************************************************************************/
/* ETH :: lscaddr1 :: reserved0 [31:16] */
#define ETH_LSCADDR1_RESERVED0_MASK                                0xffff0000
#define ETH_LSCADDR1_RESERVED0_ALIGN                               0
#define ETH_LSCADDR1_RESERVED0_BITS                                16
#define ETH_LSCADDR1_RESERVED0_SHIFT                               16

/* ETH :: lscaddr1 :: lssabyte4 [15:08] */
#define Wr_ETH_lscaddr1_lssabyte4(x) WriteRegBits(ETH_LSCADDR1,0xff00,8,x)
#define Rd_ETH_lscaddr1_lssabyte4(x) ReadRegBits(ETH_LSCADDR1,0xff00,8)
#define ETH_LSCADDR1_LSSABYTE4_MASK                                0x0000ff00
#define ETH_LSCADDR1_LSSABYTE4_ALIGN                               0
#define ETH_LSCADDR1_LSSABYTE4_BITS                                8
#define ETH_LSCADDR1_LSSABYTE4_SHIFT                               8

/* ETH :: lscaddr1 :: lssabyte5 [07:00] */
#define Wr_ETH_lscaddr1_lssabyte5(x) WriteRegBits(ETH_LSCADDR1,0xff,0,x)
#define Rd_ETH_lscaddr1_lssabyte5(x) ReadRegBits(ETH_LSCADDR1,0xff,0)
#define ETH_LSCADDR1_LSSABYTE5_MASK                                0x000000ff
#define ETH_LSCADDR1_LSSABYTE5_ALIGN                               0
#define ETH_LSCADDR1_LSSABYTE5_BITS                                8
#define ETH_LSCADDR1_LSSABYTE5_SHIFT                               0


/****************************************************************************
 * ETH :: ouiaddr0
 ***************************************************************************/
/* ETH :: ouiaddr0 :: ouisabyte0 [31:24] */
#define Wr_ETH_ouiaddr0_ouisabyte0(x) WriteRegBits(ETH_OUIADDR0,0xff000000,24,x)
#define Rd_ETH_ouiaddr0_ouisabyte0(x) ReadRegBits(ETH_OUIADDR0,0xff000000,24)
#define ETH_OUIADDR0_OUISABYTE0_MASK                               0xff000000
#define ETH_OUIADDR0_OUISABYTE0_ALIGN                              0
#define ETH_OUIADDR0_OUISABYTE0_BITS                               8
#define ETH_OUIADDR0_OUISABYTE0_SHIFT                              24

/* ETH :: ouiaddr0 :: ouisabyte1 [23:16] */
#define Wr_ETH_ouiaddr0_ouisabyte1(x) WriteRegBits(ETH_OUIADDR0,0xff0000,16,x)
#define Rd_ETH_ouiaddr0_ouisabyte1(x) ReadRegBits(ETH_OUIADDR0,0xff0000,16)
#define ETH_OUIADDR0_OUISABYTE1_MASK                               0x00ff0000
#define ETH_OUIADDR0_OUISABYTE1_ALIGN                              0
#define ETH_OUIADDR0_OUISABYTE1_BITS                               8
#define ETH_OUIADDR0_OUISABYTE1_SHIFT                              16

/* ETH :: ouiaddr0 :: ouisabyte2 [15:08] */
#define Wr_ETH_ouiaddr0_ouisabyte2(x) WriteRegBits(ETH_OUIADDR0,0xff00,8,x)
#define Rd_ETH_ouiaddr0_ouisabyte2(x) ReadRegBits(ETH_OUIADDR0,0xff00,8)
#define ETH_OUIADDR0_OUISABYTE2_MASK                               0x0000ff00
#define ETH_OUIADDR0_OUISABYTE2_ALIGN                              0
#define ETH_OUIADDR0_OUISABYTE2_BITS                               8
#define ETH_OUIADDR0_OUISABYTE2_SHIFT                              8

/* ETH :: ouiaddr0 :: ouisabyte3 [07:00] */
#define Wr_ETH_ouiaddr0_ouisabyte3(x) WriteRegBits(ETH_OUIADDR0,0xff,0,x)
#define Rd_ETH_ouiaddr0_ouisabyte3(x) ReadRegBits(ETH_OUIADDR0,0xff,0)
#define ETH_OUIADDR0_OUISABYTE3_MASK                               0x000000ff
#define ETH_OUIADDR0_OUISABYTE3_ALIGN                              0
#define ETH_OUIADDR0_OUISABYTE3_BITS                               8
#define ETH_OUIADDR0_OUISABYTE3_SHIFT                              0


/****************************************************************************
 * ETH :: ouiaddr1
 ***************************************************************************/
/* ETH :: ouiaddr1 :: reserved0 [31:16] */
#define ETH_OUIADDR1_RESERVED0_MASK                                0xffff0000
#define ETH_OUIADDR1_RESERVED0_ALIGN                               0
#define ETH_OUIADDR1_RESERVED0_BITS                                16
#define ETH_OUIADDR1_RESERVED0_SHIFT                               16

/* ETH :: ouiaddr1 :: ouisabyte4 [15:08] */
#define Wr_ETH_ouiaddr1_ouisabyte4(x) WriteRegBits(ETH_OUIADDR1,0xff00,8,x)
#define Rd_ETH_ouiaddr1_ouisabyte4(x) ReadRegBits(ETH_OUIADDR1,0xff00,8)
#define ETH_OUIADDR1_OUISABYTE4_MASK                               0x0000ff00
#define ETH_OUIADDR1_OUISABYTE4_ALIGN                              0
#define ETH_OUIADDR1_OUISABYTE4_BITS                               8
#define ETH_OUIADDR1_OUISABYTE4_SHIFT                              8

/* ETH :: ouiaddr1 :: ouisabyte5 [07:00] */
#define Wr_ETH_ouiaddr1_ouisabyte5(x) WriteRegBits(ETH_OUIADDR1,0xff,0,x)
#define Rd_ETH_ouiaddr1_ouisabyte5(x) ReadRegBits(ETH_OUIADDR1,0xff,0)
#define ETH_OUIADDR1_OUISABYTE5_MASK                               0x000000ff
#define ETH_OUIADDR1_OUISABYTE5_ALIGN                              0
#define ETH_OUIADDR1_OUISABYTE5_BITS                               8
#define ETH_OUIADDR1_OUISABYTE5_SHIFT                              0


/****************************************************************************
 * ETH :: phyctrl
 ***************************************************************************/
/* ETH :: phyctrl :: ext [31:31] */
#define Wr_ETH_phyctrl_ext(x) WriteRegBits(ETH_PHYCTRL,0x80000000,31,x)
#define Rd_ETH_phyctrl_ext(x) ReadRegBits(ETH_PHYCTRL,0x80000000,31)
#define ETH_PHYCTRL_EXT_MASK                                       0x80000000
#define ETH_PHYCTRL_EXT_ALIGN                                      0
#define ETH_PHYCTRL_EXT_BITS                                       1
#define ETH_PHYCTRL_EXT_SHIFT                                      31

/* ETH :: phyctrl :: reserved0 [30:12] */
#define ETH_PHYCTRL_RESERVED0_MASK                                 0x7ffff000
#define ETH_PHYCTRL_RESERVED0_ALIGN                                0
#define ETH_PHYCTRL_RESERVED0_BITS                                 19
#define ETH_PHYCTRL_RESERVED0_SHIFT                                12

/* ETH :: phyctrl :: trim [11:08] */
#define Wr_ETH_phyctrl_trim(x) WriteRegBits(ETH_PHYCTRL,0xf00,8,x)
#define Rd_ETH_phyctrl_trim(x) ReadRegBits(ETH_PHYCTRL,0xf00,8)
#define ETH_PHYCTRL_TRIM_MASK                                      0x00000f00
#define ETH_PHYCTRL_TRIM_ALIGN                                     0
#define ETH_PHYCTRL_TRIM_BITS                                      4
#define ETH_PHYCTRL_TRIM_SHIFT                                     8

/* ETH :: phyctrl :: reserved1 [07:06] */
#define ETH_PHYCTRL_RESERVED1_MASK                                 0x000000c0
#define ETH_PHYCTRL_RESERVED1_ALIGN                                0
#define ETH_PHYCTRL_RESERVED1_BITS                                 2
#define ETH_PHYCTRL_RESERVED1_SHIFT                                6

/* ETH :: phyctrl :: ttst [05:05] */
#define Wr_ETH_phyctrl_ttst(x) WriteRegBits(ETH_PHYCTRL,0x20,5,x)
#define Rd_ETH_phyctrl_ttst(x) ReadRegBits(ETH_PHYCTRL,0x20,5)
#define ETH_PHYCTRL_TTST_MASK                                      0x00000020
#define ETH_PHYCTRL_TTST_ALIGN                                     0
#define ETH_PHYCTRL_TTST_BITS                                      1
#define ETH_PHYCTRL_TTST_SHIFT                                     5

/* ETH :: phyctrl :: idq [04:04] */
#define Wr_ETH_phyctrl_idq(x) WriteRegBits(ETH_PHYCTRL,0x10,4,x)
#define Rd_ETH_phyctrl_idq(x) ReadRegBits(ETH_PHYCTRL,0x10,4)
#define ETH_PHYCTRL_IDQ_MASK                                       0x00000010
#define ETH_PHYCTRL_IDQ_ALIGN                                      0
#define ETH_PHYCTRL_IDQ_BITS                                       1
#define ETH_PHYCTRL_IDQ_SHIFT                                      4

/* ETH :: phyctrl :: det [03:03] */
#define Wr_ETH_phyctrl_det(x) WriteRegBits(ETH_PHYCTRL,0x8,3,x)
#define Rd_ETH_phyctrl_det(x) ReadRegBits(ETH_PHYCTRL,0x8,3)
#define ETH_PHYCTRL_DET_MASK                                       0x00000008
#define ETH_PHYCTRL_DET_ALIGN                                      0
#define ETH_PHYCTRL_DET_BITS                                       1
#define ETH_PHYCTRL_DET_SHIFT                                      3

/* ETH :: phyctrl :: pdb [02:02] */
#define Wr_ETH_phyctrl_pdb(x) WriteRegBits(ETH_PHYCTRL,0x4,2,x)
#define Rd_ETH_phyctrl_pdb(x) ReadRegBits(ETH_PHYCTRL,0x4,2)
#define ETH_PHYCTRL_PDB_MASK                                       0x00000004
#define ETH_PHYCTRL_PDB_ALIGN                                      0
#define ETH_PHYCTRL_PDB_BITS                                       1
#define ETH_PHYCTRL_PDB_SHIFT                                      2

/* ETH :: phyctrl :: pdd [01:01] */
#define Wr_ETH_phyctrl_pdd(x) WriteRegBits(ETH_PHYCTRL,0x2,1,x)
#define Rd_ETH_phyctrl_pdd(x) ReadRegBits(ETH_PHYCTRL,0x2,1)
#define ETH_PHYCTRL_PDD_MASK                                       0x00000002
#define ETH_PHYCTRL_PDD_ALIGN                                      0
#define ETH_PHYCTRL_PDD_BITS                                       1
#define ETH_PHYCTRL_PDD_SHIFT                                      1

/* ETH :: phyctrl :: pdp [00:00] */
#define Wr_ETH_phyctrl_pdp(x) WriteRegBits(ETH_PHYCTRL,0x1,0,x)
#define Rd_ETH_phyctrl_pdp(x) ReadRegBits(ETH_PHYCTRL,0x1,0)
#define ETH_PHYCTRL_PDP_MASK                                       0x00000001
#define ETH_PHYCTRL_PDP_ALIGN                                      0
#define ETH_PHYCTRL_PDP_BITS                                       1
#define ETH_PHYCTRL_PDP_SHIFT                                      0


/****************************************************************************
 * ETH :: rbuffctrl
 ***************************************************************************/
/* ETH :: rbuffctrl :: rbuffpad [31:31] */
#define Wr_ETH_rbuffctrl_rbuffpad(x) WriteRegBits(ETH_RBUFFCTRL,0x80000000,31,x)
#define Rd_ETH_rbuffctrl_rbuffpad(x) ReadRegBits(ETH_RBUFFCTRL,0x80000000,31)
#define ETH_RBUFFCTRL_RBUFFPAD_MASK                                0x80000000
#define ETH_RBUFFCTRL_RBUFFPAD_ALIGN                               0
#define ETH_RBUFFCTRL_RBUFFPAD_BITS                                1
#define ETH_RBUFFCTRL_RBUFFPAD_SHIFT                               31

/* ETH :: rbuffctrl :: reserved0 [30:16] */
#define ETH_RBUFFCTRL_RESERVED0_MASK                               0x7fff0000
#define ETH_RBUFFCTRL_RESERVED0_ALIGN                              0
#define ETH_RBUFFCTRL_RESERVED0_BITS                               15
#define ETH_RBUFFCTRL_RESERVED0_SHIFT                              16

/* ETH :: rbuffctrl :: rbuffsz [15:05] */
#define Wr_ETH_rbuffctrl_rbuffsz(x) WriteRegBits(ETH_RBUFFCTRL,0xffe0,5,x)
#define Rd_ETH_rbuffctrl_rbuffsz(x) ReadRegBits(ETH_RBUFFCTRL,0xffe0,5)
#define ETH_RBUFFCTRL_RBUFFSZ_MASK                                 0x0000ffe0
#define ETH_RBUFFCTRL_RBUFFSZ_ALIGN                                0
#define ETH_RBUFFCTRL_RBUFFSZ_BITS                                 11
#define ETH_RBUFFCTRL_RBUFFSZ_SHIFT                                5

/* ETH :: rbuffctrl :: reserved1 [04:00] */
#define ETH_RBUFFCTRL_RESERVED1_MASK                               0x0000001f
#define ETH_RBUFFCTRL_RESERVED1_ALIGN                              0
#define ETH_RBUFFCTRL_RESERVED1_BITS                               5
#define ETH_RBUFFCTRL_RESERVED1_SHIFT                              0


/****************************************************************************
 * ETH :: rbase
 ***************************************************************************/
/* ETH :: rbase :: rbase [31:05] */
#define Wr_ETH_rbase_rbase(x) WriteRegBits(ETH_RBASE,0xffffffe0,5,x)
#define Rd_ETH_rbase_rbase(x) ReadRegBits(ETH_RBASE,0xffffffe0,5)
#define ETH_RBASE_RBASE_MASK                                       0xffffffe0
#define ETH_RBASE_RBASE_ALIGN                                      0
#define ETH_RBASE_RBASE_BITS                                       27
#define ETH_RBASE_RBASE_SHIFT                                      5

/* ETH :: rbase :: reserved0 [04:00] */
#define ETH_RBASE_RESERVED0_MASK                                   0x0000001f
#define ETH_RBASE_RESERVED0_ALIGN                                  0
#define ETH_RBASE_RESERVED0_BITS                                   5
#define ETH_RBASE_RESERVED0_SHIFT                                  0


/****************************************************************************
 * ETH :: rbcfg
 ***************************************************************************/
/* ETH :: rbcfg :: rfbwmrk [31:16] */
#define Wr_ETH_rbcfg_rfbwmrk(x) WriteRegBits(ETH_RBCFG,0xffff0000,16,x)
#define Rd_ETH_rbcfg_rfbwmrk(x) ReadRegBits(ETH_RBCFG,0xffff0000,16)
#define ETH_RBCFG_RFBWMRK_MASK                                     0xffff0000
#define ETH_RBCFG_RFBWMRK_ALIGN                                    0
#define ETH_RBCFG_RFBWMRK_BITS                                     16
#define ETH_RBCFG_RFBWMRK_SHIFT                                    16

/* ETH :: rbcfg :: rlen [15:00] */
#define Wr_ETH_rbcfg_rlen(x) WriteRegBits(ETH_RBCFG,0xffff,0,x)
#define Rd_ETH_rbcfg_rlen(x) ReadRegBits(ETH_RBCFG,0xffff,0)
#define ETH_RBCFG_RLEN_MASK                                        0x0000ffff
#define ETH_RBCFG_RLEN_ALIGN                                       0
#define ETH_RBCFG_RLEN_BITS                                        16
#define ETH_RBCFG_RLEN_SHIFT                                       0


/****************************************************************************
 * ETH :: rbdptr
 ***************************************************************************/
/* ETH :: rbdptr :: rbdptr [31:03] */
#define Wr_ETH_rbdptr_rbdptr(x) WriteRegBits(ETH_RBDPTR,0xfffffff8,3,x)
#define Rd_ETH_rbdptr_rbdptr(x) ReadRegBits(ETH_RBDPTR,0xfffffff8,3)
#define ETH_RBDPTR_RBDPTR_MASK                                     0xfffffff8
#define ETH_RBDPTR_RBDPTR_ALIGN                                    0
#define ETH_RBDPTR_RBDPTR_BITS                                     29
#define ETH_RBDPTR_RBDPTR_SHIFT                                    3

/* ETH :: rbdptr :: reserved0 [02:00] */
#define ETH_RBDPTR_RESERVED0_MASK                                  0x00000007
#define ETH_RBDPTR_RESERVED0_ALIGN                                 0
#define ETH_RBDPTR_RESERVED0_BITS                                  3
#define ETH_RBDPTR_RESERVED0_SHIFT                                 0


/****************************************************************************
 * ETH :: rswptr
 ***************************************************************************/
/* ETH :: rswptr :: rswptr [31:03] */
#define Wr_ETH_rswptr_rswptr(x) WriteRegBits(ETH_RSWPTR,0xfffffff8,3,x)
#define Rd_ETH_rswptr_rswptr(x) ReadRegBits(ETH_RSWPTR,0xfffffff8,3)
#define ETH_RSWPTR_RSWPTR_MASK                                     0xfffffff8
#define ETH_RSWPTR_RSWPTR_ALIGN                                    0
#define ETH_RSWPTR_RSWPTR_BITS                                     29
#define ETH_RSWPTR_RSWPTR_SHIFT                                    3

/* ETH :: rswptr :: reserved0 [02:00] */
#define ETH_RSWPTR_RESERVED0_MASK                                  0x00000007
#define ETH_RSWPTR_RESERVED0_ALIGN                                 0
#define ETH_RSWPTR_RESERVED0_BITS                                  3
#define ETH_RSWPTR_RESERVED0_SHIFT                                 0


/****************************************************************************
 * ETH :: rbcfg_ext
 ***************************************************************************/
/* ETH :: rbcfg_ext :: rfbwmrk_bp_disable [31:16] */
#define Wr_ETH_rbcfg_ext_rfbwmrk_bp_disable(x) WriteRegBits(ETH_RBCFG_EXT,0xffff0000,16,x)
#define Rd_ETH_rbcfg_ext_rfbwmrk_bp_disable(x) ReadRegBits(ETH_RBCFG_EXT,0xffff0000,16)
#define ETH_RBCFG_EXT_RFBWMRK_BP_DISABLE_MASK                      0xffff0000
#define ETH_RBCFG_EXT_RFBWMRK_BP_DISABLE_ALIGN                     0
#define ETH_RBCFG_EXT_RFBWMRK_BP_DISABLE_BITS                      16
#define ETH_RBCFG_EXT_RFBWMRK_BP_DISABLE_SHIFT                     16

/* ETH :: rbcfg_ext :: reserved0 [15:01] */
#define ETH_RBCFG_EXT_RESERVED0_MASK                               0x0000fffe
#define ETH_RBCFG_EXT_RESERVED0_ALIGN                              0
#define ETH_RBCFG_EXT_RESERVED0_BITS                               15
#define ETH_RBCFG_EXT_RESERVED0_SHIFT                              1

/* ETH :: rbcfg_ext :: rfbwmrk_bp_disable_vld [00:00] */
#define Wr_ETH_rbcfg_ext_rfbwmrk_bp_disable_vld(x) WriteRegBits(ETH_RBCFG_EXT,0x1,0,x)
#define Rd_ETH_rbcfg_ext_rfbwmrk_bp_disable_vld(x) ReadRegBits(ETH_RBCFG_EXT,0x1,0)
#define ETH_RBCFG_EXT_RFBWMRK_BP_DISABLE_VLD_MASK                  0x00000001
#define ETH_RBCFG_EXT_RFBWMRK_BP_DISABLE_VLD_ALIGN                 0
#define ETH_RBCFG_EXT_RFBWMRK_BP_DISABLE_VLD_BITS                  1
#define ETH_RBCFG_EXT_RFBWMRK_BP_DISABLE_VLD_SHIFT                 0


/****************************************************************************
 * ETH :: tbase
 ***************************************************************************/
/* ETH :: tbase :: tbase [31:05] */
#define Wr_ETH_tbase_tbase(x) WriteRegBits(ETH_TBASE,0xffffffe0,5,x)
#define Rd_ETH_tbase_tbase(x) ReadRegBits(ETH_TBASE,0xffffffe0,5)
#define ETH_TBASE_TBASE_MASK                                       0xffffffe0
#define ETH_TBASE_TBASE_ALIGN                                      0
#define ETH_TBASE_TBASE_BITS                                       27
#define ETH_TBASE_TBASE_SHIFT                                      5

/* ETH :: tbase :: reserved0 [04:00] */
#define ETH_TBASE_RESERVED0_MASK                                   0x0000001f
#define ETH_TBASE_RESERVED0_ALIGN                                  0
#define ETH_TBASE_RESERVED0_BITS                                   5
#define ETH_TBASE_RESERVED0_SHIFT                                  0


/****************************************************************************
 * ETH :: tbcfg
 ***************************************************************************/
/* ETH :: tbcfg :: trbwmrk [31:16] */
#define Wr_ETH_tbcfg_trbwmrk(x) WriteRegBits(ETH_TBCFG,0xffff0000,16,x)
#define Rd_ETH_tbcfg_trbwmrk(x) ReadRegBits(ETH_TBCFG,0xffff0000,16)
#define ETH_TBCFG_TRBWMRK_MASK                                     0xffff0000
#define ETH_TBCFG_TRBWMRK_ALIGN                                    0
#define ETH_TBCFG_TRBWMRK_BITS                                     16
#define ETH_TBCFG_TRBWMRK_SHIFT                                    16

/* ETH :: tbcfg :: tlen [15:00] */
#define Wr_ETH_tbcfg_tlen(x) WriteRegBits(ETH_TBCFG,0xffff,0,x)
#define Rd_ETH_tbcfg_tlen(x) ReadRegBits(ETH_TBCFG,0xffff,0)
#define ETH_TBCFG_TLEN_MASK                                        0x0000ffff
#define ETH_TBCFG_TLEN_ALIGN                                       0
#define ETH_TBCFG_TLEN_BITS                                        16
#define ETH_TBCFG_TLEN_SHIFT                                       0


/****************************************************************************
 * ETH :: tbdptr
 ***************************************************************************/
/* ETH :: tbdptr :: tbdptr [31:03] */
#define Wr_ETH_tbdptr_tbdptr(x) WriteRegBits(ETH_TBDPTR,0xfffffff8,3,x)
#define Rd_ETH_tbdptr_tbdptr(x) ReadRegBits(ETH_TBDPTR,0xfffffff8,3)
#define ETH_TBDPTR_TBDPTR_MASK                                     0xfffffff8
#define ETH_TBDPTR_TBDPTR_ALIGN                                    0
#define ETH_TBDPTR_TBDPTR_BITS                                     29
#define ETH_TBDPTR_TBDPTR_SHIFT                                    3

/* ETH :: tbdptr :: reserved0 [02:00] */
#define ETH_TBDPTR_RESERVED0_MASK                                  0x00000007
#define ETH_TBDPTR_RESERVED0_ALIGN                                 0
#define ETH_TBDPTR_RESERVED0_BITS                                  3
#define ETH_TBDPTR_RESERVED0_SHIFT                                 0


/****************************************************************************
 * ETH :: tswptr
 ***************************************************************************/
/* ETH :: tswptr :: tswptr [31:03] */
#define Wr_ETH_tswptr_tswptr(x) WriteRegBits(ETH_TSWPTR,0xfffffff8,3,x)
#define Rd_ETH_tswptr_tswptr(x) ReadRegBits(ETH_TSWPTR,0xfffffff8,3)
#define ETH_TSWPTR_TSWPTR_MASK                                     0xfffffff8
#define ETH_TSWPTR_TSWPTR_ALIGN                                    0
#define ETH_TSWPTR_TSWPTR_BITS                                     29
#define ETH_TSWPTR_TSWPTR_SHIFT                                    3

/* ETH :: tswptr :: reserved0 [02:00] */
#define ETH_TSWPTR_RESERVED0_MASK                                  0x00000007
#define ETH_TSWPTR_RESERVED0_ALIGN                                 0
#define ETH_TSWPTR_RESERVED0_BITS                                  3
#define ETH_TSWPTR_RESERVED0_SHIFT                                 0


/****************************************************************************
 * ETH :: macbp
 ***************************************************************************/
/* ETH :: macbp :: reserved0 [31:07] */
#define ETH_MACBP_RESERVED0_MASK                                   0xffffff80
#define ETH_MACBP_RESERVED0_ALIGN                                  0
#define ETH_MACBP_RESERVED0_BITS                                   25
#define ETH_MACBP_RESERVED0_SHIFT                                  7

/* ETH :: macbp :: ipg_cfg [06:02] */
#define Wr_ETH_macbp_ipg_cfg(x) WriteRegBits(ETH_MACBP,0x7c,2,x)
#define Rd_ETH_macbp_ipg_cfg(x) ReadRegBits(ETH_MACBP,0x7c,2)
#define ETH_MACBP_IPG_CFG_MASK                                     0x0000007c
#define ETH_MACBP_IPG_CFG_ALIGN                                    0
#define ETH_MACBP_IPG_CFG_BITS                                     5
#define ETH_MACBP_IPG_CFG_SHIFT                                    2

/* ETH :: macbp :: bken [01:01] */
#define Wr_ETH_macbp_bken(x) WriteRegBits(ETH_MACBP,0x2,1,x)
#define Rd_ETH_macbp_bken(x) ReadRegBits(ETH_MACBP,0x2,1)
#define ETH_MACBP_BKEN_MASK                                        0x00000002
#define ETH_MACBP_BKEN_ALIGN                                       0
#define ETH_MACBP_BKEN_BITS                                        1
#define ETH_MACBP_BKEN_SHIFT                                       1

/* ETH :: macbp :: fcen [00:00] */
#define Wr_ETH_macbp_fcen(x) WriteRegBits(ETH_MACBP,0x1,0,x)
#define Rd_ETH_macbp_fcen(x) ReadRegBits(ETH_MACBP,0x1,0)
#define ETH_MACBP_FCEN_MASK                                        0x00000001
#define ETH_MACBP_FCEN_ALIGN                                       0
#define ETH_MACBP_FCEN_BITS                                        1
#define ETH_MACBP_FCEN_SHIFT                                       0


/****************************************************************************
 * ETH :: maccfg
 ***************************************************************************/
/* ETH :: maccfg :: reserved0 [31:31] */
#define ETH_MACCFG_RESERVED0_MASK                                  0x80000000
#define ETH_MACCFG_RESERVED0_ALIGN                                 0
#define ETH_MACCFG_RESERVED0_BITS                                  1
#define ETH_MACCFG_RESERVED0_SHIFT                                 31

/* ETH :: maccfg :: ipg_cfgrsf [30:30] */
#define Wr_ETH_maccfg_ipg_cfgrsf(x) WriteRegBits(ETH_MACCFG,0x40000000,30,x)
#define Rd_ETH_maccfg_ipg_cfgrsf(x) ReadRegBits(ETH_MACCFG,0x40000000,30)
#define ETH_MACCFG_IPG_CFGRSF_MASK                                 0x40000000
#define ETH_MACCFG_IPG_CFGRSF_ALIGN                                0
#define ETH_MACCFG_IPG_CFGRSF_BITS                                 1
#define ETH_MACCFG_IPG_CFGRSF_SHIFT                                30

/* ETH :: maccfg :: txrx [29:29] */
#define Wr_ETH_maccfg_txrx(x) WriteRegBits(ETH_MACCFG,0x20000000,29,x)
#define Rd_ETH_maccfg_txrx(x) ReadRegBits(ETH_MACCFG,0x20000000,29)
#define ETH_MACCFG_TXRX_MASK                                       0x20000000
#define ETH_MACCFG_TXRX_ALIGN                                      0
#define ETH_MACCFG_TXRX_BITS                                       1
#define ETH_MACCFG_TXRX_SHIFT                                      29

/* ETH :: maccfg :: tpd [28:28] */
#define Wr_ETH_maccfg_tpd(x) WriteRegBits(ETH_MACCFG,0x10000000,28,x)
#define Rd_ETH_maccfg_tpd(x) ReadRegBits(ETH_MACCFG,0x10000000,28)
#define ETH_MACCFG_TPD_MASK                                        0x10000000
#define ETH_MACCFG_TPD_ALIGN                                       0
#define ETH_MACCFG_TPD_BITS                                        1
#define ETH_MACCFG_TPD_SHIFT                                       28

/* ETH :: maccfg :: reserved1 [27:26] */
#define ETH_MACCFG_RESERVED1_MASK                                  0x0c000000
#define ETH_MACCFG_RESERVED1_ALIGN                                 0
#define ETH_MACCFG_RESERVED1_BITS                                  2
#define ETH_MACCFG_RESERVED1_SHIFT                                 26

/* ETH :: maccfg :: rlb [25:25] */
#define Wr_ETH_maccfg_rlb(x) WriteRegBits(ETH_MACCFG,0x2000000,25,x)
#define Rd_ETH_maccfg_rlb(x) ReadRegBits(ETH_MACCFG,0x2000000,25)
#define ETH_MACCFG_RLB_MASK                                        0x02000000
#define ETH_MACCFG_RLB_ALIGN                                       0
#define ETH_MACCFG_RLB_BITS                                        1
#define ETH_MACCFG_RLB_SHIFT                                       25

/* ETH :: maccfg :: nolc [24:24] */
#define Wr_ETH_maccfg_nolc(x) WriteRegBits(ETH_MACCFG,0x1000000,24,x)
#define Rd_ETH_maccfg_nolc(x) ReadRegBits(ETH_MACCFG,0x1000000,24)
#define ETH_MACCFG_NOLC_MASK                                       0x01000000
#define ETH_MACCFG_NOLC_ALIGN                                      0
#define ETH_MACCFG_NOLC_BITS                                       1
#define ETH_MACCFG_NOLC_SHIFT                                      24

/* ETH :: maccfg :: cfa [23:23] */
#define Wr_ETH_maccfg_cfa(x) WriteRegBits(ETH_MACCFG,0x800000,23,x)
#define Rd_ETH_maccfg_cfa(x) ReadRegBits(ETH_MACCFG,0x800000,23)
#define ETH_MACCFG_CFA_MASK                                        0x00800000
#define ETH_MACCFG_CFA_ALIGN                                       0
#define ETH_MACCFG_CFA_BITS                                        1
#define ETH_MACCFG_CFA_SHIFT                                       23

/* ETH :: maccfg :: acfg [22:22] */
#define Wr_ETH_maccfg_acfg(x) WriteRegBits(ETH_MACCFG,0x400000,22,x)
#define Rd_ETH_maccfg_acfg(x) ReadRegBits(ETH_MACCFG,0x400000,22)
#define ETH_MACCFG_ACFG_MASK                                       0x00400000
#define ETH_MACCFG_ACFG_ALIGN                                      0
#define ETH_MACCFG_ACFG_BITS                                       1
#define ETH_MACCFG_ACFG_SHIFT                                      22

/* ETH :: maccfg :: reserved2 [21:16] */
#define ETH_MACCFG_RESERVED2_MASK                                  0x003f0000
#define ETH_MACCFG_RESERVED2_ALIGN                                 0
#define ETH_MACCFG_RESERVED2_BITS                                  6
#define ETH_MACCFG_RESERVED2_SHIFT                                 16

/* ETH :: maccfg :: llb [15:15] */
#define Wr_ETH_maccfg_llb(x) WriteRegBits(ETH_MACCFG,0x8000,15,x)
#define Rd_ETH_maccfg_llb(x) ReadRegBits(ETH_MACCFG,0x8000,15)
#define ETH_MACCFG_LLB_MASK                                        0x00008000
#define ETH_MACCFG_LLB_ALIGN                                       0
#define ETH_MACCFG_LLB_BITS                                        1
#define ETH_MACCFG_LLB_SHIFT                                       15

/* ETH :: maccfg :: crpt [14:14] */
#define Wr_ETH_maccfg_crpt(x) WriteRegBits(ETH_MACCFG,0x4000,14,x)
#define Rd_ETH_maccfg_crpt(x) ReadRegBits(ETH_MACCFG,0x4000,14)
#define ETH_MACCFG_CRPT_MASK                                       0x00004000
#define ETH_MACCFG_CRPT_ALIGN                                      0
#define ETH_MACCFG_CRPT_BITS                                       1
#define ETH_MACCFG_CRPT_SHIFT                                      14

/* ETH :: maccfg :: srst [13:13] */
#define Wr_ETH_maccfg_srst(x) WriteRegBits(ETH_MACCFG,0x2000,13,x)
#define Rd_ETH_maccfg_srst(x) ReadRegBits(ETH_MACCFG,0x2000,13)
#define ETH_MACCFG_SRST_MASK                                       0x00002000
#define ETH_MACCFG_SRST_ALIGN                                      0
#define ETH_MACCFG_SRST_BITS                                       1
#define ETH_MACCFG_SRST_SHIFT                                      13

/* ETH :: maccfg :: ofen [12:12] */
#define Wr_ETH_maccfg_ofen(x) WriteRegBits(ETH_MACCFG,0x1000,12,x)
#define Rd_ETH_maccfg_ofen(x) ReadRegBits(ETH_MACCFG,0x1000,12)
#define ETH_MACCFG_OFEN_MASK                                       0x00001000
#define ETH_MACCFG_OFEN_ALIGN                                      0
#define ETH_MACCFG_OFEN_BITS                                       1
#define ETH_MACCFG_OFEN_SHIFT                                      12

/* ETH :: maccfg :: reserved3 [11:11] */
#define ETH_MACCFG_RESERVED3_MASK                                  0x00000800
#define ETH_MACCFG_RESERVED3_ALIGN                                 0
#define ETH_MACCFG_RESERVED3_BITS                                  1
#define ETH_MACCFG_RESERVED3_SHIFT                                 11

/* ETH :: maccfg :: hden [10:10] */
#define Wr_ETH_maccfg_hden(x) WriteRegBits(ETH_MACCFG,0x400,10,x)
#define Rd_ETH_maccfg_hden(x) ReadRegBits(ETH_MACCFG,0x400,10)
#define ETH_MACCFG_HDEN_MASK                                       0x00000400
#define ETH_MACCFG_HDEN_ALIGN                                      0
#define ETH_MACCFG_HDEN_BITS                                       1
#define ETH_MACCFG_HDEN_SHIFT                                      10

/* ETH :: maccfg :: txad [09:09] */
#define Wr_ETH_maccfg_txad(x) WriteRegBits(ETH_MACCFG,0x200,9,x)
#define Rd_ETH_maccfg_txad(x) ReadRegBits(ETH_MACCFG,0x200,9)
#define ETH_MACCFG_TXAD_MASK                                       0x00000200
#define ETH_MACCFG_TXAD_ALIGN                                      0
#define ETH_MACCFG_TXAD_BITS                                       1
#define ETH_MACCFG_TXAD_SHIFT                                      9

/* ETH :: maccfg :: pdis [08:08] */
#define Wr_ETH_maccfg_pdis(x) WriteRegBits(ETH_MACCFG,0x100,8,x)
#define Rd_ETH_maccfg_pdis(x) ReadRegBits(ETH_MACCFG,0x100,8)
#define ETH_MACCFG_PDIS_MASK                                       0x00000100
#define ETH_MACCFG_PDIS_ALIGN                                      0
#define ETH_MACCFG_PDIS_BITS                                       1
#define ETH_MACCFG_PDIS_SHIFT                                      8

/* ETH :: maccfg :: pfwd [07:07] */
#define Wr_ETH_maccfg_pfwd(x) WriteRegBits(ETH_MACCFG,0x80,7,x)
#define Rd_ETH_maccfg_pfwd(x) ReadRegBits(ETH_MACCFG,0x80,7)
#define ETH_MACCFG_PFWD_MASK                                       0x00000080
#define ETH_MACCFG_PFWD_ALIGN                                      0
#define ETH_MACCFG_PFWD_BITS                                       1
#define ETH_MACCFG_PFWD_SHIFT                                      7

/* ETH :: maccfg :: cfwd [06:06] */
#define Wr_ETH_maccfg_cfwd(x) WriteRegBits(ETH_MACCFG,0x40,6,x)
#define Rd_ETH_maccfg_cfwd(x) ReadRegBits(ETH_MACCFG,0x40,6)
#define ETH_MACCFG_CFWD_MASK                                       0x00000040
#define ETH_MACCFG_CFWD_ALIGN                                      0
#define ETH_MACCFG_CFWD_BITS                                       1
#define ETH_MACCFG_CFWD_SHIFT                                      6

/* ETH :: maccfg :: rxpad [05:05] */
#define Wr_ETH_maccfg_rxpad(x) WriteRegBits(ETH_MACCFG,0x20,5,x)
#define Rd_ETH_maccfg_rxpad(x) ReadRegBits(ETH_MACCFG,0x20,5)
#define ETH_MACCFG_RXPAD_MASK                                      0x00000020
#define ETH_MACCFG_RXPAD_ALIGN                                     0
#define ETH_MACCFG_RXPAD_BITS                                      1
#define ETH_MACCFG_RXPAD_SHIFT                                     5

/* ETH :: maccfg :: prom [04:04] */
#define Wr_ETH_maccfg_prom(x) WriteRegBits(ETH_MACCFG,0x10,4,x)
#define Rd_ETH_maccfg_prom(x) ReadRegBits(ETH_MACCFG,0x10,4)
#define ETH_MACCFG_PROM_MASK                                       0x00000010
#define ETH_MACCFG_PROM_ALIGN                                      0
#define ETH_MACCFG_PROM_BITS                                       1
#define ETH_MACCFG_PROM_SHIFT                                      4

/* ETH :: maccfg :: espd [03:02] */
#define Wr_ETH_maccfg_espd(x) WriteRegBits(ETH_MACCFG,0xc,2,x)
#define Rd_ETH_maccfg_espd(x) ReadRegBits(ETH_MACCFG,0xc,2)
#define ETH_MACCFG_ESPD_MASK                                       0x0000000c
#define ETH_MACCFG_ESPD_ALIGN                                      0
#define ETH_MACCFG_ESPD_BITS                                       2
#define ETH_MACCFG_ESPD_SHIFT                                      2

/* ETH :: maccfg :: rxen [01:01] */
#define Wr_ETH_maccfg_rxen(x) WriteRegBits(ETH_MACCFG,0x2,1,x)
#define Rd_ETH_maccfg_rxen(x) ReadRegBits(ETH_MACCFG,0x2,1)
#define ETH_MACCFG_RXEN_MASK                                       0x00000002
#define ETH_MACCFG_RXEN_ALIGN                                      0
#define ETH_MACCFG_RXEN_BITS                                       1
#define ETH_MACCFG_RXEN_SHIFT                                      1

/* ETH :: maccfg :: txen [00:00] */
#define Wr_ETH_maccfg_txen(x) WriteRegBits(ETH_MACCFG,0x1,0,x)
#define Rd_ETH_maccfg_txen(x) ReadRegBits(ETH_MACCFG,0x1,0)
#define ETH_MACCFG_TXEN_MASK                                       0x00000001
#define ETH_MACCFG_TXEN_ALIGN                                      0
#define ETH_MACCFG_TXEN_BITS                                       1
#define ETH_MACCFG_TXEN_SHIFT                                      0


/****************************************************************************
 * ETH :: macaddr0
 ***************************************************************************/
/* ETH :: macaddr0 :: sabyte0 [31:24] */
#define Wr_ETH_macaddr0_sabyte0(x) WriteRegBits(ETH_MACADDR0,0xff000000,24,x)
#define Rd_ETH_macaddr0_sabyte0(x) ReadRegBits(ETH_MACADDR0,0xff000000,24)
#define ETH_MACADDR0_SABYTE0_MASK                                  0xff000000
#define ETH_MACADDR0_SABYTE0_ALIGN                                 0
#define ETH_MACADDR0_SABYTE0_BITS                                  8
#define ETH_MACADDR0_SABYTE0_SHIFT                                 24

/* ETH :: macaddr0 :: sabyte1 [23:16] */
#define Wr_ETH_macaddr0_sabyte1(x) WriteRegBits(ETH_MACADDR0,0xff0000,16,x)
#define Rd_ETH_macaddr0_sabyte1(x) ReadRegBits(ETH_MACADDR0,0xff0000,16)
#define ETH_MACADDR0_SABYTE1_MASK                                  0x00ff0000
#define ETH_MACADDR0_SABYTE1_ALIGN                                 0
#define ETH_MACADDR0_SABYTE1_BITS                                  8
#define ETH_MACADDR0_SABYTE1_SHIFT                                 16

/* ETH :: macaddr0 :: sabyte2 [15:08] */
#define Wr_ETH_macaddr0_sabyte2(x) WriteRegBits(ETH_MACADDR0,0xff00,8,x)
#define Rd_ETH_macaddr0_sabyte2(x) ReadRegBits(ETH_MACADDR0,0xff00,8)
#define ETH_MACADDR0_SABYTE2_MASK                                  0x0000ff00
#define ETH_MACADDR0_SABYTE2_ALIGN                                 0
#define ETH_MACADDR0_SABYTE2_BITS                                  8
#define ETH_MACADDR0_SABYTE2_SHIFT                                 8

/* ETH :: macaddr0 :: sabyte3 [07:00] */
#define Wr_ETH_macaddr0_sabyte3(x) WriteRegBits(ETH_MACADDR0,0xff,0,x)
#define Rd_ETH_macaddr0_sabyte3(x) ReadRegBits(ETH_MACADDR0,0xff,0)
#define ETH_MACADDR0_SABYTE3_MASK                                  0x000000ff
#define ETH_MACADDR0_SABYTE3_ALIGN                                 0
#define ETH_MACADDR0_SABYTE3_BITS                                  8
#define ETH_MACADDR0_SABYTE3_SHIFT                                 0


/****************************************************************************
 * ETH :: macaddr1
 ***************************************************************************/
/* ETH :: macaddr1 :: reserved0 [31:16] */
#define ETH_MACADDR1_RESERVED0_MASK                                0xffff0000
#define ETH_MACADDR1_RESERVED0_ALIGN                               0
#define ETH_MACADDR1_RESERVED0_BITS                                16
#define ETH_MACADDR1_RESERVED0_SHIFT                               16

/* ETH :: macaddr1 :: sabyte4 [15:08] */
#define Wr_ETH_macaddr1_sabyte4(x) WriteRegBits(ETH_MACADDR1,0xff00,8,x)
#define Rd_ETH_macaddr1_sabyte4(x) ReadRegBits(ETH_MACADDR1,0xff00,8)
#define ETH_MACADDR1_SABYTE4_MASK                                  0x0000ff00
#define ETH_MACADDR1_SABYTE4_ALIGN                                 0
#define ETH_MACADDR1_SABYTE4_BITS                                  8
#define ETH_MACADDR1_SABYTE4_SHIFT                                 8

/* ETH :: macaddr1 :: sabyte5 [07:00] */
#define Wr_ETH_macaddr1_sabyte5(x) WriteRegBits(ETH_MACADDR1,0xff,0,x)
#define Rd_ETH_macaddr1_sabyte5(x) ReadRegBits(ETH_MACADDR1,0xff,0)
#define ETH_MACADDR1_SABYTE5_MASK                                  0x000000ff
#define ETH_MACADDR1_SABYTE5_ALIGN                                 0
#define ETH_MACADDR1_SABYTE5_BITS                                  8
#define ETH_MACADDR1_SABYTE5_SHIFT                                 0


/****************************************************************************
 * ETH :: maxfrm
 ***************************************************************************/
/* ETH :: maxfrm :: reserved0 [31:14] */
#define ETH_MAXFRM_RESERVED0_MASK                                  0xffffc000
#define ETH_MAXFRM_RESERVED0_ALIGN                                 0
#define ETH_MAXFRM_RESERVED0_BITS                                  18
#define ETH_MAXFRM_RESERVED0_SHIFT                                 14

/* ETH :: maxfrm :: maxfrm [13:00] */
#define Wr_ETH_maxfrm_maxfrm(x) WriteRegBits(ETH_MAXFRM,0x3fff,0,x)
#define Rd_ETH_maxfrm_maxfrm(x) ReadRegBits(ETH_MAXFRM,0x3fff,0)
#define ETH_MAXFRM_MAXFRM_MASK                                     0x00003fff
#define ETH_MAXFRM_MAXFRM_ALIGN                                    0
#define ETH_MAXFRM_MAXFRM_BITS                                     14
#define ETH_MAXFRM_MAXFRM_SHIFT                                    0


/****************************************************************************
 * ETH :: macpq
 ***************************************************************************/
/* ETH :: macpq :: reserved0 [31:16] */
#define ETH_MACPQ_RESERVED0_MASK                                   0xffff0000
#define ETH_MACPQ_RESERVED0_ALIGN                                  0
#define ETH_MACPQ_RESERVED0_BITS                                   16
#define ETH_MACPQ_RESERVED0_SHIFT                                  16

/* ETH :: macpq :: macpq [15:00] */
#define Wr_ETH_macpq_macpq(x) WriteRegBits(ETH_MACPQ,0xffff,0,x)
#define Rd_ETH_macpq_macpq(x) ReadRegBits(ETH_MACPQ,0xffff,0)
#define ETH_MACPQ_MACPQ_MASK                                       0x0000ffff
#define ETH_MACPQ_MACPQ_ALIGN                                      0
#define ETH_MACPQ_MACPQ_BITS                                       16
#define ETH_MACPQ_MACPQ_SHIFT                                      0


/****************************************************************************
 * ETH :: rxfifo_empty
 ***************************************************************************/
/* ETH :: rxfifo_empty :: reserved0 [31:04] */
#define ETH_RXFIFO_EMPTY_RESERVED0_MASK                            0xfffffff0
#define ETH_RXFIFO_EMPTY_RESERVED0_ALIGN                           0
#define ETH_RXFIFO_EMPTY_RESERVED0_BITS                            28
#define ETH_RXFIFO_EMPTY_RESERVED0_SHIFT                           4

/* ETH :: rxfifo_empty :: rxfe [03:00] */
#define Wr_ETH_rxfifo_empty_rxfe(x) WriteRegBits(ETH_RXFIFO_EMPTY,0xf,0,x)
#define Rd_ETH_rxfifo_empty_rxfe(x) ReadRegBits(ETH_RXFIFO_EMPTY,0xf,0)
#define ETH_RXFIFO_EMPTY_RXFE_MASK                                 0x0000000f
#define ETH_RXFIFO_EMPTY_RXFE_ALIGN                                0
#define ETH_RXFIFO_EMPTY_RXFE_BITS                                 4
#define ETH_RXFIFO_EMPTY_RXFE_SHIFT                                0


/****************************************************************************
 * ETH :: rxfifo_full
 ***************************************************************************/
/* ETH :: rxfifo_full :: reserved0 [31:04] */
#define ETH_RXFIFO_FULL_RESERVED0_MASK                             0xfffffff0
#define ETH_RXFIFO_FULL_RESERVED0_ALIGN                            0
#define ETH_RXFIFO_FULL_RESERVED0_BITS                             28
#define ETH_RXFIFO_FULL_RESERVED0_SHIFT                            4

/* ETH :: rxfifo_full :: rxff [03:00] */
#define Wr_ETH_rxfifo_full_rxff(x) WriteRegBits(ETH_RXFIFO_FULL,0xf,0,x)
#define Rd_ETH_rxfifo_full_rxff(x) ReadRegBits(ETH_RXFIFO_FULL,0xf,0)
#define ETH_RXFIFO_FULL_RXFF_MASK                                  0x0000000f
#define ETH_RXFIFO_FULL_RXFF_ALIGN                                 0
#define ETH_RXFIFO_FULL_RXFF_BITS                                  4
#define ETH_RXFIFO_FULL_RXFF_SHIFT                                 0


/****************************************************************************
 * ETH :: txfifo_empty
 ***************************************************************************/
/* ETH :: txfifo_empty :: reserved0 [31:04] */
#define ETH_TXFIFO_EMPTY_RESERVED0_MASK                            0xfffffff0
#define ETH_TXFIFO_EMPTY_RESERVED0_ALIGN                           0
#define ETH_TXFIFO_EMPTY_RESERVED0_BITS                            28
#define ETH_TXFIFO_EMPTY_RESERVED0_SHIFT                           4

/* ETH :: txfifo_empty :: txfe [03:00] */
#define Wr_ETH_txfifo_empty_txfe(x) WriteRegBits(ETH_TXFIFO_EMPTY,0xf,0,x)
#define Rd_ETH_txfifo_empty_txfe(x) ReadRegBits(ETH_TXFIFO_EMPTY,0xf,0)
#define ETH_TXFIFO_EMPTY_TXFE_MASK                                 0x0000000f
#define ETH_TXFIFO_EMPTY_TXFE_ALIGN                                0
#define ETH_TXFIFO_EMPTY_TXFE_BITS                                 4
#define ETH_TXFIFO_EMPTY_TXFE_SHIFT                                0


/****************************************************************************
 * ETH :: txfifo_full
 ***************************************************************************/
/* ETH :: txfifo_full :: reserved0 [31:04] */
#define ETH_TXFIFO_FULL_RESERVED0_MASK                             0xfffffff0
#define ETH_TXFIFO_FULL_RESERVED0_ALIGN                            0
#define ETH_TXFIFO_FULL_RESERVED0_BITS                             28
#define ETH_TXFIFO_FULL_RESERVED0_SHIFT                            4

/* ETH :: txfifo_full :: txff [03:00] */
#define Wr_ETH_txfifo_full_txff(x) WriteRegBits(ETH_TXFIFO_FULL,0xf,0,x)
#define Rd_ETH_txfifo_full_txff(x) ReadRegBits(ETH_TXFIFO_FULL,0xf,0)
#define ETH_TXFIFO_FULL_TXFF_MASK                                  0x0000000f
#define ETH_TXFIFO_FULL_TXFF_ALIGN                                 0
#define ETH_TXFIFO_FULL_TXFF_BITS                                  4
#define ETH_TXFIFO_FULL_TXFF_SHIFT                                 0


/****************************************************************************
 * ETH :: rxfifo_aempty
 ***************************************************************************/
/* ETH :: rxfifo_aempty :: reserved0 [31:04] */
#define ETH_RXFIFO_AEMPTY_RESERVED0_MASK                           0xfffffff0
#define ETH_RXFIFO_AEMPTY_RESERVED0_ALIGN                          0
#define ETH_RXFIFO_AEMPTY_RESERVED0_BITS                           28
#define ETH_RXFIFO_AEMPTY_RESERVED0_SHIFT                          4

/* ETH :: rxfifo_aempty :: rxfae [03:00] */
#define Wr_ETH_rxfifo_aempty_rxfae(x) WriteRegBits(ETH_RXFIFO_AEMPTY,0xf,0,x)
#define Rd_ETH_rxfifo_aempty_rxfae(x) ReadRegBits(ETH_RXFIFO_AEMPTY,0xf,0)
#define ETH_RXFIFO_AEMPTY_RXFAE_MASK                               0x0000000f
#define ETH_RXFIFO_AEMPTY_RXFAE_ALIGN                              0
#define ETH_RXFIFO_AEMPTY_RXFAE_BITS                               4
#define ETH_RXFIFO_AEMPTY_RXFAE_SHIFT                              0


/****************************************************************************
 * ETH :: rxfifo_afull
 ***************************************************************************/
/* ETH :: rxfifo_afull :: reserved0 [31:04] */
#define ETH_RXFIFO_AFULL_RESERVED0_MASK                            0xfffffff0
#define ETH_RXFIFO_AFULL_RESERVED0_ALIGN                           0
#define ETH_RXFIFO_AFULL_RESERVED0_BITS                            28
#define ETH_RXFIFO_AFULL_RESERVED0_SHIFT                           4

/* ETH :: rxfifo_afull :: rxfaf [03:00] */
#define Wr_ETH_rxfifo_afull_rxfaf(x) WriteRegBits(ETH_RXFIFO_AFULL,0xf,0,x)
#define Rd_ETH_rxfifo_afull_rxfaf(x) ReadRegBits(ETH_RXFIFO_AFULL,0xf,0)
#define ETH_RXFIFO_AFULL_RXFAF_MASK                                0x0000000f
#define ETH_RXFIFO_AFULL_RXFAF_ALIGN                               0
#define ETH_RXFIFO_AFULL_RXFAF_BITS                                4
#define ETH_RXFIFO_AFULL_RXFAF_SHIFT                               0


/****************************************************************************
 * ETH :: txfifo_aempty
 ***************************************************************************/
/* ETH :: txfifo_aempty :: reserved0 [31:04] */
#define ETH_TXFIFO_AEMPTY_RESERVED0_MASK                           0xfffffff0
#define ETH_TXFIFO_AEMPTY_RESERVED0_ALIGN                          0
#define ETH_TXFIFO_AEMPTY_RESERVED0_BITS                           28
#define ETH_TXFIFO_AEMPTY_RESERVED0_SHIFT                          4

/* ETH :: txfifo_aempty :: txfae [03:00] */
#define Wr_ETH_txfifo_aempty_txfae(x) WriteRegBits(ETH_TXFIFO_AEMPTY,0xf,0,x)
#define Rd_ETH_txfifo_aempty_txfae(x) ReadRegBits(ETH_TXFIFO_AEMPTY,0xf,0)
#define ETH_TXFIFO_AEMPTY_TXFAE_MASK                               0x0000000f
#define ETH_TXFIFO_AEMPTY_TXFAE_ALIGN                              0
#define ETH_TXFIFO_AEMPTY_TXFAE_BITS                               4
#define ETH_TXFIFO_AEMPTY_TXFAE_SHIFT                              0


/****************************************************************************
 * ETH :: txfifo_afull
 ***************************************************************************/
/* ETH :: txfifo_afull :: reserved0 [31:04] */
#define ETH_TXFIFO_AFULL_RESERVED0_MASK                            0xfffffff0
#define ETH_TXFIFO_AFULL_RESERVED0_ALIGN                           0
#define ETH_TXFIFO_AFULL_RESERVED0_BITS                            28
#define ETH_TXFIFO_AFULL_RESERVED0_SHIFT                           4

/* ETH :: txfifo_afull :: txfaf [03:00] */
#define Wr_ETH_txfifo_afull_txfaf(x) WriteRegBits(ETH_TXFIFO_AFULL,0xf,0,x)
#define Rd_ETH_txfifo_afull_txfaf(x) ReadRegBits(ETH_TXFIFO_AFULL,0xf,0)
#define ETH_TXFIFO_AFULL_TXFAF_MASK                                0x0000000f
#define ETH_TXFIFO_AFULL_TXFAF_ALIGN                               0
#define ETH_TXFIFO_AFULL_TXFAF_BITS                                4
#define ETH_TXFIFO_AFULL_TXFAF_SHIFT                               0


/****************************************************************************
 * ETH :: macmode
 ***************************************************************************/
/* ETH :: macmode :: reserved0 [31:06] */
#define ETH_MACMODE_RESERVED0_MASK                                 0xffffffc0
#define ETH_MACMODE_RESERVED0_ALIGN                                0
#define ETH_MACMODE_RESERVED0_BITS                                 26
#define ETH_MACMODE_RESERVED0_SHIFT                                6

/* ETH :: macmode :: lnk [05:05] */
#define Wr_ETH_macmode_lnk(x) WriteRegBits(ETH_MACMODE,0x20,5,x)
#define Rd_ETH_macmode_lnk(x) ReadRegBits(ETH_MACMODE,0x20,5)
#define ETH_MACMODE_LNK_MASK                                       0x00000020
#define ETH_MACMODE_LNK_ALIGN                                      0
#define ETH_MACMODE_LNK_BITS                                       1
#define ETH_MACMODE_LNK_SHIFT                                      5

/* ETH :: macmode :: txp [04:04] */
#define Wr_ETH_macmode_txp(x) WriteRegBits(ETH_MACMODE,0x10,4,x)
#define Rd_ETH_macmode_txp(x) ReadRegBits(ETH_MACMODE,0x10,4)
#define ETH_MACMODE_TXP_MASK                                       0x00000010
#define ETH_MACMODE_TXP_ALIGN                                      0
#define ETH_MACMODE_TXP_BITS                                       1
#define ETH_MACMODE_TXP_SHIFT                                      4

/* ETH :: macmode :: rsp [03:03] */
#define Wr_ETH_macmode_rsp(x) WriteRegBits(ETH_MACMODE,0x8,3,x)
#define Rd_ETH_macmode_rsp(x) ReadRegBits(ETH_MACMODE,0x8,3)
#define ETH_MACMODE_RSP_MASK                                       0x00000008
#define ETH_MACMODE_RSP_ALIGN                                      0
#define ETH_MACMODE_RSP_BITS                                       1
#define ETH_MACMODE_RSP_SHIFT                                      3

/* ETH :: macmode :: dp [02:02] */
#define Wr_ETH_macmode_dp(x) WriteRegBits(ETH_MACMODE,0x4,2,x)
#define Rd_ETH_macmode_dp(x) ReadRegBits(ETH_MACMODE,0x4,2)
#define ETH_MACMODE_DP_MASK                                        0x00000004
#define ETH_MACMODE_DP_ALIGN                                       0
#define ETH_MACMODE_DP_BITS                                        1
#define ETH_MACMODE_DP_SHIFT                                       2

/* ETH :: macmode :: mspd [01:00] */
#define Wr_ETH_macmode_mspd(x) WriteRegBits(ETH_MACMODE,0x3,0,x)
#define Rd_ETH_macmode_mspd(x) ReadRegBits(ETH_MACMODE,0x3,0)
#define ETH_MACMODE_MSPD_MASK                                      0x00000003
#define ETH_MACMODE_MSPD_ALIGN                                     0
#define ETH_MACMODE_MSPD_BITS                                      2
#define ETH_MACMODE_MSPD_SHIFT                                     0


/****************************************************************************
 * ETH :: vlantag0
 ***************************************************************************/
/* ETH :: vlantag0 :: reserved0 [31:16] */
#define ETH_VLANTAG0_RESERVED0_MASK                                0xffff0000
#define ETH_VLANTAG0_RESERVED0_ALIGN                               0
#define ETH_VLANTAG0_RESERVED0_BITS                                16
#define ETH_VLANTAG0_RESERVED0_SHIFT                               16

/* ETH :: vlantag0 :: vlantag0 [15:00] */
#define Wr_ETH_vlantag0_vlantag0(x) WriteRegBits(ETH_VLANTAG0,0xffff,0,x)
#define Rd_ETH_vlantag0_vlantag0(x) ReadRegBits(ETH_VLANTAG0,0xffff,0)
#define ETH_VLANTAG0_VLANTAG0_MASK                                 0x0000ffff
#define ETH_VLANTAG0_VLANTAG0_ALIGN                                0
#define ETH_VLANTAG0_VLANTAG0_BITS                                 16
#define ETH_VLANTAG0_VLANTAG0_SHIFT                                0


/****************************************************************************
 * ETH :: txipg
 ***************************************************************************/
/* ETH :: txipg :: reserved0 [31:05] */
#define ETH_TXIPG_RESERVED0_MASK                                   0xffffffe0
#define ETH_TXIPG_RESERVED0_ALIGN                                  0
#define ETH_TXIPG_RESERVED0_BITS                                   27
#define ETH_TXIPG_RESERVED0_SHIFT                                  5

/* ETH :: txipg :: txipg [04:00] */
#define Wr_ETH_txipg_txipg(x) WriteRegBits(ETH_TXIPG,0x1f,0,x)
#define Rd_ETH_txipg_txipg(x) ReadRegBits(ETH_TXIPG,0x1f,0)
#define ETH_TXIPG_TXIPG_MASK                                       0x0000001f
#define ETH_TXIPG_TXIPG_ALIGN                                      0
#define ETH_TXIPG_TXIPG_BITS                                       5
#define ETH_TXIPG_TXIPG_SHIFT                                      0


/****************************************************************************
 * ETH :: txpctrl
 ***************************************************************************/
/* ETH :: txpctrl :: reserved0 [31:18] */
#define ETH_TXPCTRL_RESERVED0_MASK                                 0xfffc0000
#define ETH_TXPCTRL_RESERVED0_ALIGN                                0
#define ETH_TXPCTRL_RESERVED0_BITS                                 14
#define ETH_TXPCTRL_RESERVED0_SHIFT                                18

/* ETH :: txpctrl :: en [17:17] */
#define Wr_ETH_txpctrl_en(x) WriteRegBits(ETH_TXPCTRL,0x20000,17,x)
#define Rd_ETH_txpctrl_en(x) ReadRegBits(ETH_TXPCTRL,0x20000,17)
#define ETH_TXPCTRL_EN_MASK                                        0x00020000
#define ETH_TXPCTRL_EN_ALIGN                                       0
#define ETH_TXPCTRL_EN_BITS                                        1
#define ETH_TXPCTRL_EN_SHIFT                                       17

/* ETH :: txpctrl :: ptimer [16:00] */
#define Wr_ETH_txpctrl_ptimer(x) WriteRegBits(ETH_TXPCTRL,0x1ffff,0,x)
#define Rd_ETH_txpctrl_ptimer(x) ReadRegBits(ETH_TXPCTRL,0x1ffff,0)
#define ETH_TXPCTRL_PTIMER_MASK                                    0x0001ffff
#define ETH_TXPCTRL_PTIMER_ALIGN                                   0
#define ETH_TXPCTRL_PTIMER_BITS                                    17
#define ETH_TXPCTRL_PTIMER_SHIFT                                   0


/****************************************************************************
 * ETH :: txfifof
 ***************************************************************************/
/* ETH :: txfifof :: reserved0 [31:01] */
#define ETH_TXFIFOF_RESERVED0_MASK                                 0xfffffffe
#define ETH_TXFIFOF_RESERVED0_ALIGN                                0
#define ETH_TXFIFOF_RESERVED0_BITS                                 31
#define ETH_TXFIFOF_RESERVED0_SHIFT                                1

/* ETH :: txfifof :: flush [00:00] */
#define Wr_ETH_txfifof_flush(x) WriteRegBits(ETH_TXFIFOF,0x1,0,x)
#define Rd_ETH_txfifof_flush(x) ReadRegBits(ETH_TXFIFOF,0x1,0)
#define ETH_TXFIFOF_FLUSH_MASK                                     0x00000001
#define ETH_TXFIFOF_FLUSH_ALIGN                                    0
#define ETH_TXFIFOF_FLUSH_BITS                                     1
#define ETH_TXFIFOF_FLUSH_SHIFT                                    0


/****************************************************************************
 * ETH :: rxfifostat
 ***************************************************************************/
/* ETH :: rxfifostat :: reserved0 [31:02] */
#define ETH_RXFIFOSTAT_RESERVED0_MASK                              0xfffffffc
#define ETH_RXFIFOSTAT_RESERVED0_ALIGN                             0
#define ETH_RXFIFOSTAT_RESERVED0_BITS                              30
#define ETH_RXFIFOSTAT_RESERVED0_SHIFT                             2

/* ETH :: rxfifostat :: rxor [01:01] */
#define Wr_ETH_rxfifostat_rxor(x) WriteRegBits(ETH_RXFIFOSTAT,0x2,1,x)
#define Rd_ETH_rxfifostat_rxor(x) ReadRegBits(ETH_RXFIFOSTAT,0x2,1)
#define ETH_RXFIFOSTAT_RXOR_MASK                                   0x00000002
#define ETH_RXFIFOSTAT_RXOR_ALIGN                                  0
#define ETH_RXFIFOSTAT_RXOR_BITS                                   1
#define ETH_RXFIFOSTAT_RXOR_SHIFT                                  1

/* ETH :: rxfifostat :: rxur [00:00] */
#define Wr_ETH_rxfifostat_rxur(x) WriteRegBits(ETH_RXFIFOSTAT,0x1,0,x)
#define Rd_ETH_rxfifostat_rxur(x) ReadRegBits(ETH_RXFIFOSTAT,0x1,0)
#define ETH_RXFIFOSTAT_RXUR_MASK                                   0x00000001
#define ETH_RXFIFOSTAT_RXUR_ALIGN                                  0
#define ETH_RXFIFOSTAT_RXUR_BITS                                   1
#define ETH_RXFIFOSTAT_RXUR_SHIFT                                  0


/****************************************************************************
 * ETH :: txfifostat
 ***************************************************************************/
/* ETH :: txfifostat :: reserved0 [31:02] */
#define ETH_TXFIFOSTAT_RESERVED0_MASK                              0xfffffffc
#define ETH_TXFIFOSTAT_RESERVED0_ALIGN                             0
#define ETH_TXFIFOSTAT_RESERVED0_BITS                              30
#define ETH_TXFIFOSTAT_RESERVED0_SHIFT                             2

/* ETH :: txfifostat :: txor [01:01] */
#define Wr_ETH_txfifostat_txor(x) WriteRegBits(ETH_TXFIFOSTAT,0x2,1,x)
#define Rd_ETH_txfifostat_txor(x) ReadRegBits(ETH_TXFIFOSTAT,0x2,1)
#define ETH_TXFIFOSTAT_TXOR_MASK                                   0x00000002
#define ETH_TXFIFOSTAT_TXOR_ALIGN                                  0
#define ETH_TXFIFOSTAT_TXOR_BITS                                   1
#define ETH_TXFIFOSTAT_TXOR_SHIFT                                  1

/* ETH :: txfifostat :: txur [00:00] */
#define Wr_ETH_txfifostat_txur(x) WriteRegBits(ETH_TXFIFOSTAT,0x1,0,x)
#define Rd_ETH_txfifostat_txur(x) ReadRegBits(ETH_TXFIFOSTAT,0x1,0)
#define ETH_TXFIFOSTAT_TXUR_MASK                                   0x00000001
#define ETH_TXFIFOSTAT_TXUR_ALIGN                                  0
#define ETH_TXFIFOSTAT_TXUR_BITS                                   1
#define ETH_TXFIFOSTAT_TXUR_SHIFT                                  0


/****************************************************************************
 * ETH :: txoctgood
 ***************************************************************************/
/* ETH :: txoctgood :: txoctgood [31:00] */
#define Wr_ETH_txoctgood_txoctgood(x) WriteReg(ETH_TXOCTGOOD,x)
#define Rd_ETH_txoctgood_txoctgood(x) ReadReg(ETH_TXOCTGOOD)
#define ETH_TXOCTGOOD_TXOCTGOOD_MASK                               0xffffffff
#define ETH_TXOCTGOOD_TXOCTGOOD_ALIGN                              0
#define ETH_TXOCTGOOD_TXOCTGOOD_BITS                               32
#define ETH_TXOCTGOOD_TXOCTGOOD_SHIFT                              0


/****************************************************************************
 * ETH :: txfrmgood
 ***************************************************************************/
/* ETH :: txfrmgood :: reserved0 [31:16] */
#define ETH_TXFRMGOOD_RESERVED0_MASK                               0xffff0000
#define ETH_TXFRMGOOD_RESERVED0_ALIGN                              0
#define ETH_TXFRMGOOD_RESERVED0_BITS                               16
#define ETH_TXFRMGOOD_RESERVED0_SHIFT                              16

/* ETH :: txfrmgood :: txfrmgood [15:00] */
#define Wr_ETH_txfrmgood_txfrmgood(x) WriteRegBits(ETH_TXFRMGOOD,0xffff,0,x)
#define Rd_ETH_txfrmgood_txfrmgood(x) ReadRegBits(ETH_TXFRMGOOD,0xffff,0)
#define ETH_TXFRMGOOD_TXFRMGOOD_MASK                               0x0000ffff
#define ETH_TXFRMGOOD_TXFRMGOOD_ALIGN                              0
#define ETH_TXFRMGOOD_TXFRMGOOD_BITS                               16
#define ETH_TXFRMGOOD_TXFRMGOOD_SHIFT                              0


/****************************************************************************
 * ETH :: txocttotal
 ***************************************************************************/
/* ETH :: txocttotal :: txocttotal [31:00] */
#define Wr_ETH_txocttotal_txocttotal(x) WriteReg(ETH_TXOCTTOTAL,x)
#define Rd_ETH_txocttotal_txocttotal(x) ReadReg(ETH_TXOCTTOTAL)
#define ETH_TXOCTTOTAL_TXOCTTOTAL_MASK                             0xffffffff
#define ETH_TXOCTTOTAL_TXOCTTOTAL_ALIGN                            0
#define ETH_TXOCTTOTAL_TXOCTTOTAL_BITS                             32
#define ETH_TXOCTTOTAL_TXOCTTOTAL_SHIFT                            0


/****************************************************************************
 * ETH :: txfrmtotal
 ***************************************************************************/
/* ETH :: txfrmtotal :: reserved0 [31:16] */
#define ETH_TXFRMTOTAL_RESERVED0_MASK                              0xffff0000
#define ETH_TXFRMTOTAL_RESERVED0_ALIGN                             0
#define ETH_TXFRMTOTAL_RESERVED0_BITS                              16
#define ETH_TXFRMTOTAL_RESERVED0_SHIFT                             16

/* ETH :: txfrmtotal :: txfrmtotal [15:00] */
#define Wr_ETH_txfrmtotal_txfrmtotal(x) WriteRegBits(ETH_TXFRMTOTAL,0xffff,0,x)
#define Rd_ETH_txfrmtotal_txfrmtotal(x) ReadRegBits(ETH_TXFRMTOTAL,0xffff,0)
#define ETH_TXFRMTOTAL_TXFRMTOTAL_MASK                             0x0000ffff
#define ETH_TXFRMTOTAL_TXFRMTOTAL_ALIGN                            0
#define ETH_TXFRMTOTAL_TXFRMTOTAL_BITS                             16
#define ETH_TXFRMTOTAL_TXFRMTOTAL_SHIFT                            0


/****************************************************************************
 * ETH :: txbcastgood
 ***************************************************************************/
/* ETH :: txbcastgood :: reserved0 [31:16] */
#define ETH_TXBCASTGOOD_RESERVED0_MASK                             0xffff0000
#define ETH_TXBCASTGOOD_RESERVED0_ALIGN                            0
#define ETH_TXBCASTGOOD_RESERVED0_BITS                             16
#define ETH_TXBCASTGOOD_RESERVED0_SHIFT                            16

/* ETH :: txbcastgood :: txbcastgood [15:00] */
#define Wr_ETH_txbcastgood_txbcastgood(x) WriteRegBits(ETH_TXBCASTGOOD,0xffff,0,x)
#define Rd_ETH_txbcastgood_txbcastgood(x) ReadRegBits(ETH_TXBCASTGOOD,0xffff,0)
#define ETH_TXBCASTGOOD_TXBCASTGOOD_MASK                           0x0000ffff
#define ETH_TXBCASTGOOD_TXBCASTGOOD_ALIGN                          0
#define ETH_TXBCASTGOOD_TXBCASTGOOD_BITS                           16
#define ETH_TXBCASTGOOD_TXBCASTGOOD_SHIFT                          0


/****************************************************************************
 * ETH :: txmcastgood
 ***************************************************************************/
/* ETH :: txmcastgood :: reserved0 [31:16] */
#define ETH_TXMCASTGOOD_RESERVED0_MASK                             0xffff0000
#define ETH_TXMCASTGOOD_RESERVED0_ALIGN                            0
#define ETH_TXMCASTGOOD_RESERVED0_BITS                             16
#define ETH_TXMCASTGOOD_RESERVED0_SHIFT                            16

/* ETH :: txmcastgood :: txmcastgood [15:00] */
#define Wr_ETH_txmcastgood_txmcastgood(x) WriteRegBits(ETH_TXMCASTGOOD,0xffff,0,x)
#define Rd_ETH_txmcastgood_txmcastgood(x) ReadRegBits(ETH_TXMCASTGOOD,0xffff,0)
#define ETH_TXMCASTGOOD_TXMCASTGOOD_MASK                           0x0000ffff
#define ETH_TXMCASTGOOD_TXMCASTGOOD_ALIGN                          0
#define ETH_TXMCASTGOOD_TXMCASTGOOD_BITS                           16
#define ETH_TXMCASTGOOD_TXMCASTGOOD_SHIFT                          0


/****************************************************************************
 * ETH :: tx64
 ***************************************************************************/
/* ETH :: tx64 :: reserved0 [31:16] */
#define ETH_TX64_RESERVED0_MASK                                    0xffff0000
#define ETH_TX64_RESERVED0_ALIGN                                   0
#define ETH_TX64_RESERVED0_BITS                                    16
#define ETH_TX64_RESERVED0_SHIFT                                   16

/* ETH :: tx64 :: tx64 [15:00] */
#define Wr_ETH_tx64_tx64(x) WriteRegBits(ETH_TX64,0xffff,0,x)
#define Rd_ETH_tx64_tx64(x) ReadRegBits(ETH_TX64,0xffff,0)
#define ETH_TX64_TX64_MASK                                         0x0000ffff
#define ETH_TX64_TX64_ALIGN                                        0
#define ETH_TX64_TX64_BITS                                         16
#define ETH_TX64_TX64_SHIFT                                        0


/****************************************************************************
 * ETH :: tx65_127
 ***************************************************************************/
/* ETH :: tx65_127 :: reserved0 [31:16] */
#define ETH_TX65_127_RESERVED0_MASK                                0xffff0000
#define ETH_TX65_127_RESERVED0_ALIGN                               0
#define ETH_TX65_127_RESERVED0_BITS                                16
#define ETH_TX65_127_RESERVED0_SHIFT                               16

/* ETH :: tx65_127 :: tx65_127 [15:00] */
#define Wr_ETH_tx65_127_tx65_127(x) WriteRegBits(ETH_TX65_127,0xffff,0,x)
#define Rd_ETH_tx65_127_tx65_127(x) ReadRegBits(ETH_TX65_127,0xffff,0)
#define ETH_TX65_127_TX65_127_MASK                                 0x0000ffff
#define ETH_TX65_127_TX65_127_ALIGN                                0
#define ETH_TX65_127_TX65_127_BITS                                 16
#define ETH_TX65_127_TX65_127_SHIFT                                0


/****************************************************************************
 * ETH :: tx128_255
 ***************************************************************************/
/* ETH :: tx128_255 :: reserved0 [31:16] */
#define ETH_TX128_255_RESERVED0_MASK                               0xffff0000
#define ETH_TX128_255_RESERVED0_ALIGN                              0
#define ETH_TX128_255_RESERVED0_BITS                               16
#define ETH_TX128_255_RESERVED0_SHIFT                              16

/* ETH :: tx128_255 :: tx128_255 [15:00] */
#define Wr_ETH_tx128_255_tx128_255(x) WriteRegBits(ETH_TX128_255,0xffff,0,x)
#define Rd_ETH_tx128_255_tx128_255(x) ReadRegBits(ETH_TX128_255,0xffff,0)
#define ETH_TX128_255_TX128_255_MASK                               0x0000ffff
#define ETH_TX128_255_TX128_255_ALIGN                              0
#define ETH_TX128_255_TX128_255_BITS                               16
#define ETH_TX128_255_TX128_255_SHIFT                              0


/****************************************************************************
 * ETH :: tx256_511
 ***************************************************************************/
/* ETH :: tx256_511 :: reserved0 [31:16] */
#define ETH_TX256_511_RESERVED0_MASK                               0xffff0000
#define ETH_TX256_511_RESERVED0_ALIGN                              0
#define ETH_TX256_511_RESERVED0_BITS                               16
#define ETH_TX256_511_RESERVED0_SHIFT                              16

/* ETH :: tx256_511 :: tx256_511 [15:00] */
#define Wr_ETH_tx256_511_tx256_511(x) WriteRegBits(ETH_TX256_511,0xffff,0,x)
#define Rd_ETH_tx256_511_tx256_511(x) ReadRegBits(ETH_TX256_511,0xffff,0)
#define ETH_TX256_511_TX256_511_MASK                               0x0000ffff
#define ETH_TX256_511_TX256_511_ALIGN                              0
#define ETH_TX256_511_TX256_511_BITS                               16
#define ETH_TX256_511_TX256_511_SHIFT                              0


/****************************************************************************
 * ETH :: tx512_1023
 ***************************************************************************/
/* ETH :: tx512_1023 :: reserved0 [31:16] */
#define ETH_TX512_1023_RESERVED0_MASK                              0xffff0000
#define ETH_TX512_1023_RESERVED0_ALIGN                             0
#define ETH_TX512_1023_RESERVED0_BITS                              16
#define ETH_TX512_1023_RESERVED0_SHIFT                             16

/* ETH :: tx512_1023 :: tx512_1023 [15:00] */
#define Wr_ETH_tx512_1023_tx512_1023(x) WriteRegBits(ETH_TX512_1023,0xffff,0,x)
#define Rd_ETH_tx512_1023_tx512_1023(x) ReadRegBits(ETH_TX512_1023,0xffff,0)
#define ETH_TX512_1023_TX512_1023_MASK                             0x0000ffff
#define ETH_TX512_1023_TX512_1023_ALIGN                            0
#define ETH_TX512_1023_TX512_1023_BITS                             16
#define ETH_TX512_1023_TX512_1023_SHIFT                            0


/****************************************************************************
 * ETH :: tx1024_MAX
 ***************************************************************************/
/* ETH :: tx1024_MAX :: reserved0 [31:16] */
#define ETH_TX1024_MAX_RESERVED0_MASK                              0xffff0000
#define ETH_TX1024_MAX_RESERVED0_ALIGN                             0
#define ETH_TX1024_MAX_RESERVED0_BITS                              16
#define ETH_TX1024_MAX_RESERVED0_SHIFT                             16

/* ETH :: tx1024_MAX :: tx1024_MAX [15:00] */
#define Wr_ETH_tx1024_MAX_tx1024_MAX(x) WriteRegBits(ETH_TX1024_MAX,0xffff,0,x)
#define Rd_ETH_tx1024_MAX_tx1024_MAX(x) ReadRegBits(ETH_TX1024_MAX,0xffff,0)
#define ETH_TX1024_MAX_TX1024_MAX_MASK                             0x0000ffff
#define ETH_TX1024_MAX_TX1024_MAX_ALIGN                            0
#define ETH_TX1024_MAX_TX1024_MAX_BITS                             16
#define ETH_TX1024_MAX_TX1024_MAX_SHIFT                            0


/****************************************************************************
 * ETH :: txjabber
 ***************************************************************************/
/* ETH :: txjabber :: reserved0 [31:16] */
#define ETH_TXJABBER_RESERVED0_MASK                                0xffff0000
#define ETH_TXJABBER_RESERVED0_ALIGN                               0
#define ETH_TXJABBER_RESERVED0_BITS                                16
#define ETH_TXJABBER_RESERVED0_SHIFT                               16

/* ETH :: txjabber :: txjabber [15:00] */
#define Wr_ETH_txjabber_txjabber(x) WriteRegBits(ETH_TXJABBER,0xffff,0,x)
#define Rd_ETH_txjabber_txjabber(x) ReadRegBits(ETH_TXJABBER,0xffff,0)
#define ETH_TXJABBER_TXJABBER_MASK                                 0x0000ffff
#define ETH_TXJABBER_TXJABBER_ALIGN                                0
#define ETH_TXJABBER_TXJABBER_BITS                                 16
#define ETH_TXJABBER_TXJABBER_SHIFT                                0


/****************************************************************************
 * ETH :: txjumbo
 ***************************************************************************/
/* ETH :: txjumbo :: reserved0 [31:16] */
#define ETH_TXJUMBO_RESERVED0_MASK                                 0xffff0000
#define ETH_TXJUMBO_RESERVED0_ALIGN                                0
#define ETH_TXJUMBO_RESERVED0_BITS                                 16
#define ETH_TXJUMBO_RESERVED0_SHIFT                                16

/* ETH :: txjumbo :: txjumbo [15:00] */
#define Wr_ETH_txjumbo_txjumbo(x) WriteRegBits(ETH_TXJUMBO,0xffff,0,x)
#define Rd_ETH_txjumbo_txjumbo(x) ReadRegBits(ETH_TXJUMBO,0xffff,0)
#define ETH_TXJUMBO_TXJUMBO_MASK                                   0x0000ffff
#define ETH_TXJUMBO_TXJUMBO_ALIGN                                  0
#define ETH_TXJUMBO_TXJUMBO_BITS                                   16
#define ETH_TXJUMBO_TXJUMBO_SHIFT                                  0


/****************************************************************************
 * ETH :: txfrag
 ***************************************************************************/
/* ETH :: txfrag :: reserved0 [31:16] */
#define ETH_TXFRAG_RESERVED0_MASK                                  0xffff0000
#define ETH_TXFRAG_RESERVED0_ALIGN                                 0
#define ETH_TXFRAG_RESERVED0_BITS                                  16
#define ETH_TXFRAG_RESERVED0_SHIFT                                 16

/* ETH :: txfrag :: txfrag [15:00] */
#define Wr_ETH_txfrag_txfrag(x) WriteRegBits(ETH_TXFRAG,0xffff,0,x)
#define Rd_ETH_txfrag_txfrag(x) ReadRegBits(ETH_TXFRAG,0xffff,0)
#define ETH_TXFRAG_TXFRAG_MASK                                     0x0000ffff
#define ETH_TXFRAG_TXFRAG_ALIGN                                    0
#define ETH_TXFRAG_TXFRAG_BITS                                     16
#define ETH_TXFRAG_TXFRAG_SHIFT                                    0


/****************************************************************************
 * ETH :: txunderrun
 ***************************************************************************/
/* ETH :: txunderrun :: reserved0 [31:16] */
#define ETH_TXUNDERRUN_RESERVED0_MASK                              0xffff0000
#define ETH_TXUNDERRUN_RESERVED0_ALIGN                             0
#define ETH_TXUNDERRUN_RESERVED0_BITS                              16
#define ETH_TXUNDERRUN_RESERVED0_SHIFT                             16

/* ETH :: txunderrun :: txunderrun [15:00] */
#define Wr_ETH_txunderrun_txunderrun(x) WriteRegBits(ETH_TXUNDERRUN,0xffff,0,x)
#define Rd_ETH_txunderrun_txunderrun(x) ReadRegBits(ETH_TXUNDERRUN,0xffff,0)
#define ETH_TXUNDERRUN_TXUNDERRUN_MASK                             0x0000ffff
#define ETH_TXUNDERRUN_TXUNDERRUN_ALIGN                            0
#define ETH_TXUNDERRUN_TXUNDERRUN_BITS                             16
#define ETH_TXUNDERRUN_TXUNDERRUN_SHIFT                            0


/****************************************************************************
 * ETH :: txcoltotal
 ***************************************************************************/
/* ETH :: txcoltotal :: reserved0 [31:16] */
#define ETH_TXCOLTOTAL_RESERVED0_MASK                              0xffff0000
#define ETH_TXCOLTOTAL_RESERVED0_ALIGN                             0
#define ETH_TXCOLTOTAL_RESERVED0_BITS                              16
#define ETH_TXCOLTOTAL_RESERVED0_SHIFT                             16

/* ETH :: txcoltotal :: txcoltotal [15:00] */
#define Wr_ETH_txcoltotal_txcoltotal(x) WriteRegBits(ETH_TXCOLTOTAL,0xffff,0,x)
#define Rd_ETH_txcoltotal_txcoltotal(x) ReadRegBits(ETH_TXCOLTOTAL,0xffff,0)
#define ETH_TXCOLTOTAL_TXCOLTOTAL_MASK                             0x0000ffff
#define ETH_TXCOLTOTAL_TXCOLTOTAL_ALIGN                            0
#define ETH_TXCOLTOTAL_TXCOLTOTAL_BITS                             16
#define ETH_TXCOLTOTAL_TXCOLTOTAL_SHIFT                            0


/****************************************************************************
 * ETH :: tx1col
 ***************************************************************************/
/* ETH :: tx1col :: reserved0 [31:16] */
#define ETH_TX1COL_RESERVED0_MASK                                  0xffff0000
#define ETH_TX1COL_RESERVED0_ALIGN                                 0
#define ETH_TX1COL_RESERVED0_BITS                                  16
#define ETH_TX1COL_RESERVED0_SHIFT                                 16

/* ETH :: tx1col :: tx1col [15:00] */
#define Wr_ETH_tx1col_tx1col(x) WriteRegBits(ETH_TX1COL,0xffff,0,x)
#define Rd_ETH_tx1col_tx1col(x) ReadRegBits(ETH_TX1COL,0xffff,0)
#define ETH_TX1COL_TX1COL_MASK                                     0x0000ffff
#define ETH_TX1COL_TX1COL_ALIGN                                    0
#define ETH_TX1COL_TX1COL_BITS                                     16
#define ETH_TX1COL_TX1COL_SHIFT                                    0


/****************************************************************************
 * ETH :: txmcol
 ***************************************************************************/
/* ETH :: txmcol :: reserved0 [31:16] */
#define ETH_TXMCOL_RESERVED0_MASK                                  0xffff0000
#define ETH_TXMCOL_RESERVED0_ALIGN                                 0
#define ETH_TXMCOL_RESERVED0_BITS                                  16
#define ETH_TXMCOL_RESERVED0_SHIFT                                 16

/* ETH :: txmcol :: txmcol [15:00] */
#define Wr_ETH_txmcol_txmcol(x) WriteRegBits(ETH_TXMCOL,0xffff,0,x)
#define Rd_ETH_txmcol_txmcol(x) ReadRegBits(ETH_TXMCOL,0xffff,0)
#define ETH_TXMCOL_TXMCOL_MASK                                     0x0000ffff
#define ETH_TXMCOL_TXMCOL_ALIGN                                    0
#define ETH_TXMCOL_TXMCOL_BITS                                     16
#define ETH_TXMCOL_TXMCOL_SHIFT                                    0


/****************************************************************************
 * ETH :: txexcol
 ***************************************************************************/
/* ETH :: txexcol :: reserved0 [31:16] */
#define ETH_TXEXCOL_RESERVED0_MASK                                 0xffff0000
#define ETH_TXEXCOL_RESERVED0_ALIGN                                0
#define ETH_TXEXCOL_RESERVED0_BITS                                 16
#define ETH_TXEXCOL_RESERVED0_SHIFT                                16

/* ETH :: txexcol :: txexcol [15:00] */
#define Wr_ETH_txexcol_txexcol(x) WriteRegBits(ETH_TXEXCOL,0xffff,0,x)
#define Rd_ETH_txexcol_txexcol(x) ReadRegBits(ETH_TXEXCOL,0xffff,0)
#define ETH_TXEXCOL_TXEXCOL_MASK                                   0x0000ffff
#define ETH_TXEXCOL_TXEXCOL_ALIGN                                  0
#define ETH_TXEXCOL_TXEXCOL_BITS                                   16
#define ETH_TXEXCOL_TXEXCOL_SHIFT                                  0


/****************************************************************************
 * ETH :: txlate
 ***************************************************************************/
/* ETH :: txlate :: reserved0 [31:16] */
#define ETH_TXLATE_RESERVED0_MASK                                  0xffff0000
#define ETH_TXLATE_RESERVED0_ALIGN                                 0
#define ETH_TXLATE_RESERVED0_BITS                                  16
#define ETH_TXLATE_RESERVED0_SHIFT                                 16

/* ETH :: txlate :: txlate [15:00] */
#define Wr_ETH_txlate_txlate(x) WriteRegBits(ETH_TXLATE,0xffff,0,x)
#define Rd_ETH_txlate_txlate(x) ReadRegBits(ETH_TXLATE,0xffff,0)
#define ETH_TXLATE_TXLATE_MASK                                     0x0000ffff
#define ETH_TXLATE_TXLATE_ALIGN                                    0
#define ETH_TXLATE_TXLATE_BITS                                     16
#define ETH_TXLATE_TXLATE_SHIFT                                    0


/****************************************************************************
 * ETH :: txdefer
 ***************************************************************************/
/* ETH :: txdefer :: reserved0 [31:16] */
#define ETH_TXDEFER_RESERVED0_MASK                                 0xffff0000
#define ETH_TXDEFER_RESERVED0_ALIGN                                0
#define ETH_TXDEFER_RESERVED0_BITS                                 16
#define ETH_TXDEFER_RESERVED0_SHIFT                                16

/* ETH :: txdefer :: txdefer [15:00] */
#define Wr_ETH_txdefer_txdefer(x) WriteRegBits(ETH_TXDEFER,0xffff,0,x)
#define Rd_ETH_txdefer_txdefer(x) ReadRegBits(ETH_TXDEFER,0xffff,0)
#define ETH_TXDEFER_TXDEFER_MASK                                   0x0000ffff
#define ETH_TXDEFER_TXDEFER_ALIGN                                  0
#define ETH_TXDEFER_TXDEFER_BITS                                   16
#define ETH_TXDEFER_TXDEFER_SHIFT                                  0


/****************************************************************************
 * ETH :: txnocrs
 ***************************************************************************/
/* ETH :: txnocrs :: reserved0 [31:16] */
#define ETH_TXNOCRS_RESERVED0_MASK                                 0xffff0000
#define ETH_TXNOCRS_RESERVED0_ALIGN                                0
#define ETH_TXNOCRS_RESERVED0_BITS                                 16
#define ETH_TXNOCRS_RESERVED0_SHIFT                                16

/* ETH :: txnocrs :: txnocrs [15:00] */
#define Wr_ETH_txnocrs_txnocrs(x) WriteRegBits(ETH_TXNOCRS,0xffff,0,x)
#define Rd_ETH_txnocrs_txnocrs(x) ReadRegBits(ETH_TXNOCRS,0xffff,0)
#define ETH_TXNOCRS_TXNOCRS_MASK                                   0x0000ffff
#define ETH_TXNOCRS_TXNOCRS_ALIGN                                  0
#define ETH_TXNOCRS_TXNOCRS_BITS                                   16
#define ETH_TXNOCRS_TXNOCRS_SHIFT                                  0


/****************************************************************************
 * ETH :: txpause
 ***************************************************************************/
/* ETH :: txpause :: reserved0 [31:16] */
#define ETH_TXPAUSE_RESERVED0_MASK                                 0xffff0000
#define ETH_TXPAUSE_RESERVED0_ALIGN                                0
#define ETH_TXPAUSE_RESERVED0_BITS                                 16
#define ETH_TXPAUSE_RESERVED0_SHIFT                                16

/* ETH :: txpause :: txpause [15:00] */
#define Wr_ETH_txpause_txpause(x) WriteRegBits(ETH_TXPAUSE,0xffff,0,x)
#define Rd_ETH_txpause_txpause(x) ReadRegBits(ETH_TXPAUSE,0xffff,0)
#define ETH_TXPAUSE_TXPAUSE_MASK                                   0x0000ffff
#define ETH_TXPAUSE_TXPAUSE_ALIGN                                  0
#define ETH_TXPAUSE_TXPAUSE_BITS                                   16
#define ETH_TXPAUSE_TXPAUSE_SHIFT                                  0


/****************************************************************************
 * ETH :: txcntof
 ***************************************************************************/
/* ETH :: txcntof :: reserved0 [31:24] */
#define ETH_TXCNTOF_RESERVED0_MASK                                 0xff000000
#define ETH_TXCNTOF_RESERVED0_ALIGN                                0
#define ETH_TXCNTOF_RESERVED0_BITS                                 8
#define ETH_TXCNTOF_RESERVED0_SHIFT                                24

/* ETH :: txcntof :: txoctgood_of [23:23] */
#define Wr_ETH_txcntof_txoctgood_of(x) WriteRegBits(ETH_TXCNTOF,0x800000,23,x)
#define Rd_ETH_txcntof_txoctgood_of(x) ReadRegBits(ETH_TXCNTOF,0x800000,23)
#define ETH_TXCNTOF_TXOCTGOOD_OF_MASK                              0x00800000
#define ETH_TXCNTOF_TXOCTGOOD_OF_ALIGN                             0
#define ETH_TXCNTOF_TXOCTGOOD_OF_BITS                              1
#define ETH_TXCNTOF_TXOCTGOOD_OF_SHIFT                             23

/* ETH :: txcntof :: txfrmgood_of [22:22] */
#define Wr_ETH_txcntof_txfrmgood_of(x) WriteRegBits(ETH_TXCNTOF,0x400000,22,x)
#define Rd_ETH_txcntof_txfrmgood_of(x) ReadRegBits(ETH_TXCNTOF,0x400000,22)
#define ETH_TXCNTOF_TXFRMGOOD_OF_MASK                              0x00400000
#define ETH_TXCNTOF_TXFRMGOOD_OF_ALIGN                             0
#define ETH_TXCNTOF_TXFRMGOOD_OF_BITS                              1
#define ETH_TXCNTOF_TXFRMGOOD_OF_SHIFT                             22

/* ETH :: txcntof :: txocttotal_of [21:21] */
#define Wr_ETH_txcntof_txocttotal_of(x) WriteRegBits(ETH_TXCNTOF,0x200000,21,x)
#define Rd_ETH_txcntof_txocttotal_of(x) ReadRegBits(ETH_TXCNTOF,0x200000,21)
#define ETH_TXCNTOF_TXOCTTOTAL_OF_MASK                             0x00200000
#define ETH_TXCNTOF_TXOCTTOTAL_OF_ALIGN                            0
#define ETH_TXCNTOF_TXOCTTOTAL_OF_BITS                             1
#define ETH_TXCNTOF_TXOCTTOTAL_OF_SHIFT                            21

/* ETH :: txcntof :: txfrmtotal_of [20:20] */
#define Wr_ETH_txcntof_txfrmtotal_of(x) WriteRegBits(ETH_TXCNTOF,0x100000,20,x)
#define Rd_ETH_txcntof_txfrmtotal_of(x) ReadRegBits(ETH_TXCNTOF,0x100000,20)
#define ETH_TXCNTOF_TXFRMTOTAL_OF_MASK                             0x00100000
#define ETH_TXCNTOF_TXFRMTOTAL_OF_ALIGN                            0
#define ETH_TXCNTOF_TXFRMTOTAL_OF_BITS                             1
#define ETH_TXCNTOF_TXFRMTOTAL_OF_SHIFT                            20

/* ETH :: txcntof :: txbcastgood_of [19:19] */
#define Wr_ETH_txcntof_txbcastgood_of(x) WriteRegBits(ETH_TXCNTOF,0x80000,19,x)
#define Rd_ETH_txcntof_txbcastgood_of(x) ReadRegBits(ETH_TXCNTOF,0x80000,19)
#define ETH_TXCNTOF_TXBCASTGOOD_OF_MASK                            0x00080000
#define ETH_TXCNTOF_TXBCASTGOOD_OF_ALIGN                           0
#define ETH_TXCNTOF_TXBCASTGOOD_OF_BITS                            1
#define ETH_TXCNTOF_TXBCASTGOOD_OF_SHIFT                           19

/* ETH :: txcntof :: txmcastgood_of [18:18] */
#define Wr_ETH_txcntof_txmcastgood_of(x) WriteRegBits(ETH_TXCNTOF,0x40000,18,x)
#define Rd_ETH_txcntof_txmcastgood_of(x) ReadRegBits(ETH_TXCNTOF,0x40000,18)
#define ETH_TXCNTOF_TXMCASTGOOD_OF_MASK                            0x00040000
#define ETH_TXCNTOF_TXMCASTGOOD_OF_ALIGN                           0
#define ETH_TXCNTOF_TXMCASTGOOD_OF_BITS                            1
#define ETH_TXCNTOF_TXMCASTGOOD_OF_SHIFT                           18

/* ETH :: txcntof :: tx64_of [17:17] */
#define Wr_ETH_txcntof_tx64_of(x) WriteRegBits(ETH_TXCNTOF,0x20000,17,x)
#define Rd_ETH_txcntof_tx64_of(x) ReadRegBits(ETH_TXCNTOF,0x20000,17)
#define ETH_TXCNTOF_TX64_OF_MASK                                   0x00020000
#define ETH_TXCNTOF_TX64_OF_ALIGN                                  0
#define ETH_TXCNTOF_TX64_OF_BITS                                   1
#define ETH_TXCNTOF_TX64_OF_SHIFT                                  17

/* ETH :: txcntof :: tx65_127_of [16:16] */
#define Wr_ETH_txcntof_tx65_127_of(x) WriteRegBits(ETH_TXCNTOF,0x10000,16,x)
#define Rd_ETH_txcntof_tx65_127_of(x) ReadRegBits(ETH_TXCNTOF,0x10000,16)
#define ETH_TXCNTOF_TX65_127_OF_MASK                               0x00010000
#define ETH_TXCNTOF_TX65_127_OF_ALIGN                              0
#define ETH_TXCNTOF_TX65_127_OF_BITS                               1
#define ETH_TXCNTOF_TX65_127_OF_SHIFT                              16

/* ETH :: txcntof :: tx128_255_of [15:15] */
#define Wr_ETH_txcntof_tx128_255_of(x) WriteRegBits(ETH_TXCNTOF,0x8000,15,x)
#define Rd_ETH_txcntof_tx128_255_of(x) ReadRegBits(ETH_TXCNTOF,0x8000,15)
#define ETH_TXCNTOF_TX128_255_OF_MASK                              0x00008000
#define ETH_TXCNTOF_TX128_255_OF_ALIGN                             0
#define ETH_TXCNTOF_TX128_255_OF_BITS                              1
#define ETH_TXCNTOF_TX128_255_OF_SHIFT                             15

/* ETH :: txcntof :: tx256_511_of [14:14] */
#define Wr_ETH_txcntof_tx256_511_of(x) WriteRegBits(ETH_TXCNTOF,0x4000,14,x)
#define Rd_ETH_txcntof_tx256_511_of(x) ReadRegBits(ETH_TXCNTOF,0x4000,14)
#define ETH_TXCNTOF_TX256_511_OF_MASK                              0x00004000
#define ETH_TXCNTOF_TX256_511_OF_ALIGN                             0
#define ETH_TXCNTOF_TX256_511_OF_BITS                              1
#define ETH_TXCNTOF_TX256_511_OF_SHIFT                             14

/* ETH :: txcntof :: tx512_1023_of [13:13] */
#define Wr_ETH_txcntof_tx512_1023_of(x) WriteRegBits(ETH_TXCNTOF,0x2000,13,x)
#define Rd_ETH_txcntof_tx512_1023_of(x) ReadRegBits(ETH_TXCNTOF,0x2000,13)
#define ETH_TXCNTOF_TX512_1023_OF_MASK                             0x00002000
#define ETH_TXCNTOF_TX512_1023_OF_ALIGN                            0
#define ETH_TXCNTOF_TX512_1023_OF_BITS                             1
#define ETH_TXCNTOF_TX512_1023_OF_SHIFT                            13

/* ETH :: txcntof :: tx1024_max_of [12:12] */
#define Wr_ETH_txcntof_tx1024_max_of(x) WriteRegBits(ETH_TXCNTOF,0x1000,12,x)
#define Rd_ETH_txcntof_tx1024_max_of(x) ReadRegBits(ETH_TXCNTOF,0x1000,12)
#define ETH_TXCNTOF_TX1024_MAX_OF_MASK                             0x00001000
#define ETH_TXCNTOF_TX1024_MAX_OF_ALIGN                            0
#define ETH_TXCNTOF_TX1024_MAX_OF_BITS                             1
#define ETH_TXCNTOF_TX1024_MAX_OF_SHIFT                            12

/* ETH :: txcntof :: txjabber_of [11:11] */
#define Wr_ETH_txcntof_txjabber_of(x) WriteRegBits(ETH_TXCNTOF,0x800,11,x)
#define Rd_ETH_txcntof_txjabber_of(x) ReadRegBits(ETH_TXCNTOF,0x800,11)
#define ETH_TXCNTOF_TXJABBER_OF_MASK                               0x00000800
#define ETH_TXCNTOF_TXJABBER_OF_ALIGN                              0
#define ETH_TXCNTOF_TXJABBER_OF_BITS                               1
#define ETH_TXCNTOF_TXJABBER_OF_SHIFT                              11

/* ETH :: txcntof :: txjumbo_of [10:10] */
#define Wr_ETH_txcntof_txjumbo_of(x) WriteRegBits(ETH_TXCNTOF,0x400,10,x)
#define Rd_ETH_txcntof_txjumbo_of(x) ReadRegBits(ETH_TXCNTOF,0x400,10)
#define ETH_TXCNTOF_TXJUMBO_OF_MASK                                0x00000400
#define ETH_TXCNTOF_TXJUMBO_OF_ALIGN                               0
#define ETH_TXCNTOF_TXJUMBO_OF_BITS                                1
#define ETH_TXCNTOF_TXJUMBO_OF_SHIFT                               10

/* ETH :: txcntof :: txfrag_of [09:09] */
#define Wr_ETH_txcntof_txfrag_of(x) WriteRegBits(ETH_TXCNTOF,0x200,9,x)
#define Rd_ETH_txcntof_txfrag_of(x) ReadRegBits(ETH_TXCNTOF,0x200,9)
#define ETH_TXCNTOF_TXFRAG_OF_MASK                                 0x00000200
#define ETH_TXCNTOF_TXFRAG_OF_ALIGN                                0
#define ETH_TXCNTOF_TXFRAG_OF_BITS                                 1
#define ETH_TXCNTOF_TXFRAG_OF_SHIFT                                9

/* ETH :: txcntof :: txunderrun_of [08:08] */
#define Wr_ETH_txcntof_txunderrun_of(x) WriteRegBits(ETH_TXCNTOF,0x100,8,x)
#define Rd_ETH_txcntof_txunderrun_of(x) ReadRegBits(ETH_TXCNTOF,0x100,8)
#define ETH_TXCNTOF_TXUNDERRUN_OF_MASK                             0x00000100
#define ETH_TXCNTOF_TXUNDERRUN_OF_ALIGN                            0
#define ETH_TXCNTOF_TXUNDERRUN_OF_BITS                             1
#define ETH_TXCNTOF_TXUNDERRUN_OF_SHIFT                            8

/* ETH :: txcntof :: txcoltotal_of [07:07] */
#define Wr_ETH_txcntof_txcoltotal_of(x) WriteRegBits(ETH_TXCNTOF,0x80,7,x)
#define Rd_ETH_txcntof_txcoltotal_of(x) ReadRegBits(ETH_TXCNTOF,0x80,7)
#define ETH_TXCNTOF_TXCOLTOTAL_OF_MASK                             0x00000080
#define ETH_TXCNTOF_TXCOLTOTAL_OF_ALIGN                            0
#define ETH_TXCNTOF_TXCOLTOTAL_OF_BITS                             1
#define ETH_TXCNTOF_TXCOLTOTAL_OF_SHIFT                            7

/* ETH :: txcntof :: tx1col_of [06:06] */
#define Wr_ETH_txcntof_tx1col_of(x) WriteRegBits(ETH_TXCNTOF,0x40,6,x)
#define Rd_ETH_txcntof_tx1col_of(x) ReadRegBits(ETH_TXCNTOF,0x40,6)
#define ETH_TXCNTOF_TX1COL_OF_MASK                                 0x00000040
#define ETH_TXCNTOF_TX1COL_OF_ALIGN                                0
#define ETH_TXCNTOF_TX1COL_OF_BITS                                 1
#define ETH_TXCNTOF_TX1COL_OF_SHIFT                                6

/* ETH :: txcntof :: txmcol_of [05:05] */
#define Wr_ETH_txcntof_txmcol_of(x) WriteRegBits(ETH_TXCNTOF,0x20,5,x)
#define Rd_ETH_txcntof_txmcol_of(x) ReadRegBits(ETH_TXCNTOF,0x20,5)
#define ETH_TXCNTOF_TXMCOL_OF_MASK                                 0x00000020
#define ETH_TXCNTOF_TXMCOL_OF_ALIGN                                0
#define ETH_TXCNTOF_TXMCOL_OF_BITS                                 1
#define ETH_TXCNTOF_TXMCOL_OF_SHIFT                                5

/* ETH :: txcntof :: txexcol_of [04:04] */
#define Wr_ETH_txcntof_txexcol_of(x) WriteRegBits(ETH_TXCNTOF,0x10,4,x)
#define Rd_ETH_txcntof_txexcol_of(x) ReadRegBits(ETH_TXCNTOF,0x10,4)
#define ETH_TXCNTOF_TXEXCOL_OF_MASK                                0x00000010
#define ETH_TXCNTOF_TXEXCOL_OF_ALIGN                               0
#define ETH_TXCNTOF_TXEXCOL_OF_BITS                                1
#define ETH_TXCNTOF_TXEXCOL_OF_SHIFT                               4

/* ETH :: txcntof :: txlate_of [03:03] */
#define Wr_ETH_txcntof_txlate_of(x) WriteRegBits(ETH_TXCNTOF,0x8,3,x)
#define Rd_ETH_txcntof_txlate_of(x) ReadRegBits(ETH_TXCNTOF,0x8,3)
#define ETH_TXCNTOF_TXLATE_OF_MASK                                 0x00000008
#define ETH_TXCNTOF_TXLATE_OF_ALIGN                                0
#define ETH_TXCNTOF_TXLATE_OF_BITS                                 1
#define ETH_TXCNTOF_TXLATE_OF_SHIFT                                3

/* ETH :: txcntof :: txdefer_of [02:02] */
#define Wr_ETH_txcntof_txdefer_of(x) WriteRegBits(ETH_TXCNTOF,0x4,2,x)
#define Rd_ETH_txcntof_txdefer_of(x) ReadRegBits(ETH_TXCNTOF,0x4,2)
#define ETH_TXCNTOF_TXDEFER_OF_MASK                                0x00000004
#define ETH_TXCNTOF_TXDEFER_OF_ALIGN                               0
#define ETH_TXCNTOF_TXDEFER_OF_BITS                                1
#define ETH_TXCNTOF_TXDEFER_OF_SHIFT                               2

/* ETH :: txcntof :: txnocrs_of [01:01] */
#define Wr_ETH_txcntof_txnocrs_of(x) WriteRegBits(ETH_TXCNTOF,0x2,1,x)
#define Rd_ETH_txcntof_txnocrs_of(x) ReadRegBits(ETH_TXCNTOF,0x2,1)
#define ETH_TXCNTOF_TXNOCRS_OF_MASK                                0x00000002
#define ETH_TXCNTOF_TXNOCRS_OF_ALIGN                               0
#define ETH_TXCNTOF_TXNOCRS_OF_BITS                                1
#define ETH_TXCNTOF_TXNOCRS_OF_SHIFT                               1

/* ETH :: txcntof :: txpause_of [00:00] */
#define Wr_ETH_txcntof_txpause_of(x) WriteRegBits(ETH_TXCNTOF,0x1,0,x)
#define Rd_ETH_txcntof_txpause_of(x) ReadRegBits(ETH_TXCNTOF,0x1,0)
#define ETH_TXCNTOF_TXPAUSE_OF_MASK                                0x00000001
#define ETH_TXCNTOF_TXPAUSE_OF_ALIGN                               0
#define ETH_TXCNTOF_TXPAUSE_OF_BITS                                1
#define ETH_TXCNTOF_TXPAUSE_OF_SHIFT                               0


/****************************************************************************
 * ETH :: rxoctgood
 ***************************************************************************/
/* ETH :: rxoctgood :: rxoctgood [31:00] */
#define Wr_ETH_rxoctgood_rxoctgood(x) WriteReg(ETH_RXOCTGOOD,x)
#define Rd_ETH_rxoctgood_rxoctgood(x) ReadReg(ETH_RXOCTGOOD)
#define ETH_RXOCTGOOD_RXOCTGOOD_MASK                               0xffffffff
#define ETH_RXOCTGOOD_RXOCTGOOD_ALIGN                              0
#define ETH_RXOCTGOOD_RXOCTGOOD_BITS                               32
#define ETH_RXOCTGOOD_RXOCTGOOD_SHIFT                              0


/****************************************************************************
 * ETH :: rxfrmgood
 ***************************************************************************/
/* ETH :: rxfrmgood :: reserved0 [31:16] */
#define ETH_RXFRMGOOD_RESERVED0_MASK                               0xffff0000
#define ETH_RXFRMGOOD_RESERVED0_ALIGN                              0
#define ETH_RXFRMGOOD_RESERVED0_BITS                               16
#define ETH_RXFRMGOOD_RESERVED0_SHIFT                              16

/* ETH :: rxfrmgood :: rxfrmgood [15:00] */
#define Wr_ETH_rxfrmgood_rxfrmgood(x) WriteRegBits(ETH_RXFRMGOOD,0xffff,0,x)
#define Rd_ETH_rxfrmgood_rxfrmgood(x) ReadRegBits(ETH_RXFRMGOOD,0xffff,0)
#define ETH_RXFRMGOOD_RXFRMGOOD_MASK                               0x0000ffff
#define ETH_RXFRMGOOD_RXFRMGOOD_ALIGN                              0
#define ETH_RXFRMGOOD_RXFRMGOOD_BITS                               16
#define ETH_RXFRMGOOD_RXFRMGOOD_SHIFT                              0


/****************************************************************************
 * ETH :: rxocttotal
 ***************************************************************************/
/* ETH :: rxocttotal :: rxocttotal [31:00] */
#define Wr_ETH_rxocttotal_rxocttotal(x) WriteReg(ETH_RXOCTTOTAL,x)
#define Rd_ETH_rxocttotal_rxocttotal(x) ReadReg(ETH_RXOCTTOTAL)
#define ETH_RXOCTTOTAL_RXOCTTOTAL_MASK                             0xffffffff
#define ETH_RXOCTTOTAL_RXOCTTOTAL_ALIGN                            0
#define ETH_RXOCTTOTAL_RXOCTTOTAL_BITS                             32
#define ETH_RXOCTTOTAL_RXOCTTOTAL_SHIFT                            0


/****************************************************************************
 * ETH :: rxfrmtotal
 ***************************************************************************/
/* ETH :: rxfrmtotal :: reserved0 [31:16] */
#define ETH_RXFRMTOTAL_RESERVED0_MASK                              0xffff0000
#define ETH_RXFRMTOTAL_RESERVED0_ALIGN                             0
#define ETH_RXFRMTOTAL_RESERVED0_BITS                              16
#define ETH_RXFRMTOTAL_RESERVED0_SHIFT                             16

/* ETH :: rxfrmtotal :: rxfrmtotal [15:00] */
#define Wr_ETH_rxfrmtotal_rxfrmtotal(x) WriteRegBits(ETH_RXFRMTOTAL,0xffff,0,x)
#define Rd_ETH_rxfrmtotal_rxfrmtotal(x) ReadRegBits(ETH_RXFRMTOTAL,0xffff,0)
#define ETH_RXFRMTOTAL_RXFRMTOTAL_MASK                             0x0000ffff
#define ETH_RXFRMTOTAL_RXFRMTOTAL_ALIGN                            0
#define ETH_RXFRMTOTAL_RXFRMTOTAL_BITS                             16
#define ETH_RXFRMTOTAL_RXFRMTOTAL_SHIFT                            0


/****************************************************************************
 * ETH :: rxbcastgood
 ***************************************************************************/
/* ETH :: rxbcastgood :: reserved0 [31:16] */
#define ETH_RXBCASTGOOD_RESERVED0_MASK                             0xffff0000
#define ETH_RXBCASTGOOD_RESERVED0_ALIGN                            0
#define ETH_RXBCASTGOOD_RESERVED0_BITS                             16
#define ETH_RXBCASTGOOD_RESERVED0_SHIFT                            16

/* ETH :: rxbcastgood :: rxbcastgood [15:00] */
#define Wr_ETH_rxbcastgood_rxbcastgood(x) WriteRegBits(ETH_RXBCASTGOOD,0xffff,0,x)
#define Rd_ETH_rxbcastgood_rxbcastgood(x) ReadRegBits(ETH_RXBCASTGOOD,0xffff,0)
#define ETH_RXBCASTGOOD_RXBCASTGOOD_MASK                           0x0000ffff
#define ETH_RXBCASTGOOD_RXBCASTGOOD_ALIGN                          0
#define ETH_RXBCASTGOOD_RXBCASTGOOD_BITS                           16
#define ETH_RXBCASTGOOD_RXBCASTGOOD_SHIFT                          0


/****************************************************************************
 * ETH :: rxmcastgood
 ***************************************************************************/
/* ETH :: rxmcastgood :: reserved0 [31:16] */
#define ETH_RXMCASTGOOD_RESERVED0_MASK                             0xffff0000
#define ETH_RXMCASTGOOD_RESERVED0_ALIGN                            0
#define ETH_RXMCASTGOOD_RESERVED0_BITS                             16
#define ETH_RXMCASTGOOD_RESERVED0_SHIFT                            16

/* ETH :: rxmcastgood :: rxmcastgood [15:00] */
#define Wr_ETH_rxmcastgood_rxmcastgood(x) WriteRegBits(ETH_RXMCASTGOOD,0xffff,0,x)
#define Rd_ETH_rxmcastgood_rxmcastgood(x) ReadRegBits(ETH_RXMCASTGOOD,0xffff,0)
#define ETH_RXMCASTGOOD_RXMCASTGOOD_MASK                           0x0000ffff
#define ETH_RXMCASTGOOD_RXMCASTGOOD_ALIGN                          0
#define ETH_RXMCASTGOOD_RXMCASTGOOD_BITS                           16
#define ETH_RXMCASTGOOD_RXMCASTGOOD_SHIFT                          0


/****************************************************************************
 * ETH :: rx64
 ***************************************************************************/
/* ETH :: rx64 :: reserved0 [31:16] */
#define ETH_RX64_RESERVED0_MASK                                    0xffff0000
#define ETH_RX64_RESERVED0_ALIGN                                   0
#define ETH_RX64_RESERVED0_BITS                                    16
#define ETH_RX64_RESERVED0_SHIFT                                   16

/* ETH :: rx64 :: rx64 [15:00] */
#define Wr_ETH_rx64_rx64(x) WriteRegBits(ETH_RX64,0xffff,0,x)
#define Rd_ETH_rx64_rx64(x) ReadRegBits(ETH_RX64,0xffff,0)
#define ETH_RX64_RX64_MASK                                         0x0000ffff
#define ETH_RX64_RX64_ALIGN                                        0
#define ETH_RX64_RX64_BITS                                         16
#define ETH_RX64_RX64_SHIFT                                        0


/****************************************************************************
 * ETH :: rx65_127
 ***************************************************************************/
/* ETH :: rx65_127 :: reserved0 [31:16] */
#define ETH_RX65_127_RESERVED0_MASK                                0xffff0000
#define ETH_RX65_127_RESERVED0_ALIGN                               0
#define ETH_RX65_127_RESERVED0_BITS                                16
#define ETH_RX65_127_RESERVED0_SHIFT                               16

/* ETH :: rx65_127 :: rx65_127 [15:00] */
#define Wr_ETH_rx65_127_rx65_127(x) WriteRegBits(ETH_RX65_127,0xffff,0,x)
#define Rd_ETH_rx65_127_rx65_127(x) ReadRegBits(ETH_RX65_127,0xffff,0)
#define ETH_RX65_127_RX65_127_MASK                                 0x0000ffff
#define ETH_RX65_127_RX65_127_ALIGN                                0
#define ETH_RX65_127_RX65_127_BITS                                 16
#define ETH_RX65_127_RX65_127_SHIFT                                0


/****************************************************************************
 * ETH :: rx128_255
 ***************************************************************************/
/* ETH :: rx128_255 :: reserved0 [31:16] */
#define ETH_RX128_255_RESERVED0_MASK                               0xffff0000
#define ETH_RX128_255_RESERVED0_ALIGN                              0
#define ETH_RX128_255_RESERVED0_BITS                               16
#define ETH_RX128_255_RESERVED0_SHIFT                              16

/* ETH :: rx128_255 :: rx128_255 [15:00] */
#define Wr_ETH_rx128_255_rx128_255(x) WriteRegBits(ETH_RX128_255,0xffff,0,x)
#define Rd_ETH_rx128_255_rx128_255(x) ReadRegBits(ETH_RX128_255,0xffff,0)
#define ETH_RX128_255_RX128_255_MASK                               0x0000ffff
#define ETH_RX128_255_RX128_255_ALIGN                              0
#define ETH_RX128_255_RX128_255_BITS                               16
#define ETH_RX128_255_RX128_255_SHIFT                              0


/****************************************************************************
 * ETH :: rx256_511
 ***************************************************************************/
/* ETH :: rx256_511 :: reserved0 [31:16] */
#define ETH_RX256_511_RESERVED0_MASK                               0xffff0000
#define ETH_RX256_511_RESERVED0_ALIGN                              0
#define ETH_RX256_511_RESERVED0_BITS                               16
#define ETH_RX256_511_RESERVED0_SHIFT                              16

/* ETH :: rx256_511 :: rx256_511 [15:00] */
#define Wr_ETH_rx256_511_rx256_511(x) WriteRegBits(ETH_RX256_511,0xffff,0,x)
#define Rd_ETH_rx256_511_rx256_511(x) ReadRegBits(ETH_RX256_511,0xffff,0)
#define ETH_RX256_511_RX256_511_MASK                               0x0000ffff
#define ETH_RX256_511_RX256_511_ALIGN                              0
#define ETH_RX256_511_RX256_511_BITS                               16
#define ETH_RX256_511_RX256_511_SHIFT                              0


/****************************************************************************
 * ETH :: rx512_1023
 ***************************************************************************/
/* ETH :: rx512_1023 :: reserved0 [31:16] */
#define ETH_RX512_1023_RESERVED0_MASK                              0xffff0000
#define ETH_RX512_1023_RESERVED0_ALIGN                             0
#define ETH_RX512_1023_RESERVED0_BITS                              16
#define ETH_RX512_1023_RESERVED0_SHIFT                             16

/* ETH :: rx512_1023 :: rx512_1023 [15:00] */
#define Wr_ETH_rx512_1023_rx512_1023(x) WriteRegBits(ETH_RX512_1023,0xffff,0,x)
#define Rd_ETH_rx512_1023_rx512_1023(x) ReadRegBits(ETH_RX512_1023,0xffff,0)
#define ETH_RX512_1023_RX512_1023_MASK                             0x0000ffff
#define ETH_RX512_1023_RX512_1023_ALIGN                            0
#define ETH_RX512_1023_RX512_1023_BITS                             16
#define ETH_RX512_1023_RX512_1023_SHIFT                            0


/****************************************************************************
 * ETH :: rx1024_MAX
 ***************************************************************************/
/* ETH :: rx1024_MAX :: reserved0 [31:16] */
#define ETH_RX1024_MAX_RESERVED0_MASK                              0xffff0000
#define ETH_RX1024_MAX_RESERVED0_ALIGN                             0
#define ETH_RX1024_MAX_RESERVED0_BITS                              16
#define ETH_RX1024_MAX_RESERVED0_SHIFT                             16

/* ETH :: rx1024_MAX :: rx1024_MAX [15:00] */
#define Wr_ETH_rx1024_MAX_rx1024_MAX(x) WriteRegBits(ETH_RX1024_MAX,0xffff,0,x)
#define Rd_ETH_rx1024_MAX_rx1024_MAX(x) ReadRegBits(ETH_RX1024_MAX,0xffff,0)
#define ETH_RX1024_MAX_RX1024_MAX_MASK                             0x0000ffff
#define ETH_RX1024_MAX_RX1024_MAX_ALIGN                            0
#define ETH_RX1024_MAX_RX1024_MAX_BITS                             16
#define ETH_RX1024_MAX_RX1024_MAX_SHIFT                            0


/****************************************************************************
 * ETH :: rxjabber
 ***************************************************************************/
/* ETH :: rxjabber :: reserved0 [31:16] */
#define ETH_RXJABBER_RESERVED0_MASK                                0xffff0000
#define ETH_RXJABBER_RESERVED0_ALIGN                               0
#define ETH_RXJABBER_RESERVED0_BITS                                16
#define ETH_RXJABBER_RESERVED0_SHIFT                               16

/* ETH :: rxjabber :: rxjabber [15:00] */
#define Wr_ETH_rxjabber_rxjabber(x) WriteRegBits(ETH_RXJABBER,0xffff,0,x)
#define Rd_ETH_rxjabber_rxjabber(x) ReadRegBits(ETH_RXJABBER,0xffff,0)
#define ETH_RXJABBER_RXJABBER_MASK                                 0x0000ffff
#define ETH_RXJABBER_RXJABBER_ALIGN                                0
#define ETH_RXJABBER_RXJABBER_BITS                                 16
#define ETH_RXJABBER_RXJABBER_SHIFT                                0


/****************************************************************************
 * ETH :: rxjumbo
 ***************************************************************************/
/* ETH :: rxjumbo :: reserved0 [31:16] */
#define ETH_RXJUMBO_RESERVED0_MASK                                 0xffff0000
#define ETH_RXJUMBO_RESERVED0_ALIGN                                0
#define ETH_RXJUMBO_RESERVED0_BITS                                 16
#define ETH_RXJUMBO_RESERVED0_SHIFT                                16

/* ETH :: rxjumbo :: rxjumbo [15:00] */
#define Wr_ETH_rxjumbo_rxjumbo(x) WriteRegBits(ETH_RXJUMBO,0xffff,0,x)
#define Rd_ETH_rxjumbo_rxjumbo(x) ReadRegBits(ETH_RXJUMBO,0xffff,0)
#define ETH_RXJUMBO_RXJUMBO_MASK                                   0x0000ffff
#define ETH_RXJUMBO_RXJUMBO_ALIGN                                  0
#define ETH_RXJUMBO_RXJUMBO_BITS                                   16
#define ETH_RXJUMBO_RXJUMBO_SHIFT                                  0


/****************************************************************************
 * ETH :: rxfrag
 ***************************************************************************/
/* ETH :: rxfrag :: reserved0 [31:16] */
#define ETH_RXFRAG_RESERVED0_MASK                                  0xffff0000
#define ETH_RXFRAG_RESERVED0_ALIGN                                 0
#define ETH_RXFRAG_RESERVED0_BITS                                  16
#define ETH_RXFRAG_RESERVED0_SHIFT                                 16

/* ETH :: rxfrag :: rxfrag [15:00] */
#define Wr_ETH_rxfrag_rxfrag(x) WriteRegBits(ETH_RXFRAG,0xffff,0,x)
#define Rd_ETH_rxfrag_rxfrag(x) ReadRegBits(ETH_RXFRAG,0xffff,0)
#define ETH_RXFRAG_RXFRAG_MASK                                     0x0000ffff
#define ETH_RXFRAG_RXFRAG_ALIGN                                    0
#define ETH_RXFRAG_RXFRAG_BITS                                     16
#define ETH_RXFRAG_RXFRAG_SHIFT                                    0


/****************************************************************************
 * ETH :: rxoverrun
 ***************************************************************************/
/* ETH :: rxoverrun :: reserved0 [31:16] */
#define ETH_RXOVERRUN_RESERVED0_MASK                               0xffff0000
#define ETH_RXOVERRUN_RESERVED0_ALIGN                              0
#define ETH_RXOVERRUN_RESERVED0_BITS                               16
#define ETH_RXOVERRUN_RESERVED0_SHIFT                              16

/* ETH :: rxoverrun :: rxoverrun [15:00] */
#define Wr_ETH_rxoverrun_rxoverrun(x) WriteRegBits(ETH_RXOVERRUN,0xffff,0,x)
#define Rd_ETH_rxoverrun_rxoverrun(x) ReadRegBits(ETH_RXOVERRUN,0xffff,0)
#define ETH_RXOVERRUN_RXOVERRUN_MASK                               0x0000ffff
#define ETH_RXOVERRUN_RXOVERRUN_ALIGN                              0
#define ETH_RXOVERRUN_RXOVERRUN_BITS                               16
#define ETH_RXOVERRUN_RXOVERRUN_SHIFT                              0


/****************************************************************************
 * ETH :: rxcrcalign
 ***************************************************************************/
/* ETH :: rxcrcalign :: reserved0 [31:16] */
#define ETH_RXCRCALIGN_RESERVED0_MASK                              0xffff0000
#define ETH_RXCRCALIGN_RESERVED0_ALIGN                             0
#define ETH_RXCRCALIGN_RESERVED0_BITS                              16
#define ETH_RXCRCALIGN_RESERVED0_SHIFT                             16

/* ETH :: rxcrcalign :: rxcrcalign [15:00] */
#define Wr_ETH_rxcrcalign_rxcrcalign(x) WriteRegBits(ETH_RXCRCALIGN,0xffff,0,x)
#define Rd_ETH_rxcrcalign_rxcrcalign(x) ReadRegBits(ETH_RXCRCALIGN,0xffff,0)
#define ETH_RXCRCALIGN_RXCRCALIGN_MASK                             0x0000ffff
#define ETH_RXCRCALIGN_RXCRCALIGN_ALIGN                            0
#define ETH_RXCRCALIGN_RXCRCALIGN_BITS                             16
#define ETH_RXCRCALIGN_RXCRCALIGN_SHIFT                            0


/****************************************************************************
 * ETH :: rxusize
 ***************************************************************************/
/* ETH :: rxusize :: reserved0 [31:16] */
#define ETH_RXUSIZE_RESERVED0_MASK                                 0xffff0000
#define ETH_RXUSIZE_RESERVED0_ALIGN                                0
#define ETH_RXUSIZE_RESERVED0_BITS                                 16
#define ETH_RXUSIZE_RESERVED0_SHIFT                                16

/* ETH :: rxusize :: rxusize [15:00] */
#define Wr_ETH_rxusize_rxusize(x) WriteRegBits(ETH_RXUSIZE,0xffff,0,x)
#define Rd_ETH_rxusize_rxusize(x) ReadRegBits(ETH_RXUSIZE,0xffff,0)
#define ETH_RXUSIZE_RXUSIZE_MASK                                   0x0000ffff
#define ETH_RXUSIZE_RXUSIZE_ALIGN                                  0
#define ETH_RXUSIZE_RXUSIZE_BITS                                   16
#define ETH_RXUSIZE_RXUSIZE_SHIFT                                  0


/****************************************************************************
 * ETH :: rxcrc
 ***************************************************************************/
/* ETH :: rxcrc :: reserved0 [31:16] */
#define ETH_RXCRC_RESERVED0_MASK                                   0xffff0000
#define ETH_RXCRC_RESERVED0_ALIGN                                  0
#define ETH_RXCRC_RESERVED0_BITS                                   16
#define ETH_RXCRC_RESERVED0_SHIFT                                  16

/* ETH :: rxcrc :: rxcrc [15:00] */
#define Wr_ETH_rxcrc_rxcrc(x) WriteRegBits(ETH_RXCRC,0xffff,0,x)
#define Rd_ETH_rxcrc_rxcrc(x) ReadRegBits(ETH_RXCRC,0xffff,0)
#define ETH_RXCRC_RXCRC_MASK                                       0x0000ffff
#define ETH_RXCRC_RXCRC_ALIGN                                      0
#define ETH_RXCRC_RXCRC_BITS                                       16
#define ETH_RXCRC_RXCRC_SHIFT                                      0


/****************************************************************************
 * ETH :: rxalign
 ***************************************************************************/
/* ETH :: rxalign :: reserved0 [31:16] */
#define ETH_RXALIGN_RESERVED0_MASK                                 0xffff0000
#define ETH_RXALIGN_RESERVED0_ALIGN                                0
#define ETH_RXALIGN_RESERVED0_BITS                                 16
#define ETH_RXALIGN_RESERVED0_SHIFT                                16

/* ETH :: rxalign :: rxalign [15:00] */
#define Wr_ETH_rxalign_rxalign(x) WriteRegBits(ETH_RXALIGN,0xffff,0,x)
#define Rd_ETH_rxalign_rxalign(x) ReadRegBits(ETH_RXALIGN,0xffff,0)
#define ETH_RXALIGN_RXALIGN_MASK                                   0x0000ffff
#define ETH_RXALIGN_RXALIGN_ALIGN                                  0
#define ETH_RXALIGN_RXALIGN_BITS                                   16
#define ETH_RXALIGN_RXALIGN_SHIFT                                  0


/****************************************************************************
 * ETH :: rxcderr
 ***************************************************************************/
/* ETH :: rxcderr :: reserved0 [31:16] */
#define ETH_RXCDERR_RESERVED0_MASK                                 0xffff0000
#define ETH_RXCDERR_RESERVED0_ALIGN                                0
#define ETH_RXCDERR_RESERVED0_BITS                                 16
#define ETH_RXCDERR_RESERVED0_SHIFT                                16

/* ETH :: rxcderr :: rxcderr [15:00] */
#define Wr_ETH_rxcderr_rxcderr(x) WriteRegBits(ETH_RXCDERR,0xffff,0,x)
#define Rd_ETH_rxcderr_rxcderr(x) ReadRegBits(ETH_RXCDERR,0xffff,0)
#define ETH_RXCDERR_RXCDERR_MASK                                   0x0000ffff
#define ETH_RXCDERR_RXCDERR_ALIGN                                  0
#define ETH_RXCDERR_RXCDERR_BITS                                   16
#define ETH_RXCDERR_RXCDERR_SHIFT                                  0


/****************************************************************************
 * ETH :: rxpause
 ***************************************************************************/
/* ETH :: rxpause :: reserved0 [31:16] */
#define ETH_RXPAUSE_RESERVED0_MASK                                 0xffff0000
#define ETH_RXPAUSE_RESERVED0_ALIGN                                0
#define ETH_RXPAUSE_RESERVED0_BITS                                 16
#define ETH_RXPAUSE_RESERVED0_SHIFT                                16

/* ETH :: rxpause :: rxpause [15:00] */
#define Wr_ETH_rxpause_rxpause(x) WriteRegBits(ETH_RXPAUSE,0xffff,0,x)
#define Rd_ETH_rxpause_rxpause(x) ReadRegBits(ETH_RXPAUSE,0xffff,0)
#define ETH_RXPAUSE_RXPAUSE_MASK                                   0x0000ffff
#define ETH_RXPAUSE_RXPAUSE_ALIGN                                  0
#define ETH_RXPAUSE_RXPAUSE_BITS                                   16
#define ETH_RXPAUSE_RXPAUSE_SHIFT                                  0


/****************************************************************************
 * ETH :: rxctrlfm
 ***************************************************************************/
/* ETH :: rxctrlfm :: reserved0 [31:16] */
#define ETH_RXCTRLFM_RESERVED0_MASK                                0xffff0000
#define ETH_RXCTRLFM_RESERVED0_ALIGN                               0
#define ETH_RXCTRLFM_RESERVED0_BITS                                16
#define ETH_RXCTRLFM_RESERVED0_SHIFT                               16

/* ETH :: rxctrlfm :: rxctrlfm [15:00] */
#define Wr_ETH_rxctrlfm_rxctrlfm(x) WriteRegBits(ETH_RXCTRLFM,0xffff,0,x)
#define Rd_ETH_rxctrlfm_rxctrlfm(x) ReadRegBits(ETH_RXCTRLFM,0xffff,0)
#define ETH_RXCTRLFM_RXCTRLFM_MASK                                 0x0000ffff
#define ETH_RXCTRLFM_RXCTRLFM_ALIGN                                0
#define ETH_RXCTRLFM_RXCTRLFM_BITS                                 16
#define ETH_RXCTRLFM_RXCTRLFM_SHIFT                                0


/****************************************************************************
 * ETH :: rxcntof
 ***************************************************************************/
/* ETH :: rxcntof :: reserved0 [31:23] */
#define ETH_RXCNTOF_RESERVED0_MASK                                 0xff800000
#define ETH_RXCNTOF_RESERVED0_ALIGN                                0
#define ETH_RXCNTOF_RESERVED0_BITS                                 9
#define ETH_RXCNTOF_RESERVED0_SHIFT                                23

/* ETH :: rxcntof :: rxoctgood_of [22:22] */
#define Wr_ETH_rxcntof_rxoctgood_of(x) WriteRegBits(ETH_RXCNTOF,0x400000,22,x)
#define Rd_ETH_rxcntof_rxoctgood_of(x) ReadRegBits(ETH_RXCNTOF,0x400000,22)
#define ETH_RXCNTOF_RXOCTGOOD_OF_MASK                              0x00400000
#define ETH_RXCNTOF_RXOCTGOOD_OF_ALIGN                             0
#define ETH_RXCNTOF_RXOCTGOOD_OF_BITS                              1
#define ETH_RXCNTOF_RXOCTGOOD_OF_SHIFT                             22

/* ETH :: rxcntof :: rxfrmgood_of [21:21] */
#define Wr_ETH_rxcntof_rxfrmgood_of(x) WriteRegBits(ETH_RXCNTOF,0x200000,21,x)
#define Rd_ETH_rxcntof_rxfrmgood_of(x) ReadRegBits(ETH_RXCNTOF,0x200000,21)
#define ETH_RXCNTOF_RXFRMGOOD_OF_MASK                              0x00200000
#define ETH_RXCNTOF_RXFRMGOOD_OF_ALIGN                             0
#define ETH_RXCNTOF_RXFRMGOOD_OF_BITS                              1
#define ETH_RXCNTOF_RXFRMGOOD_OF_SHIFT                             21

/* ETH :: rxcntof :: rxocttotal_of [20:20] */
#define Wr_ETH_rxcntof_rxocttotal_of(x) WriteRegBits(ETH_RXCNTOF,0x100000,20,x)
#define Rd_ETH_rxcntof_rxocttotal_of(x) ReadRegBits(ETH_RXCNTOF,0x100000,20)
#define ETH_RXCNTOF_RXOCTTOTAL_OF_MASK                             0x00100000
#define ETH_RXCNTOF_RXOCTTOTAL_OF_ALIGN                            0
#define ETH_RXCNTOF_RXOCTTOTAL_OF_BITS                             1
#define ETH_RXCNTOF_RXOCTTOTAL_OF_SHIFT                            20

/* ETH :: rxcntof :: rxfrmtotal_of [19:19] */
#define Wr_ETH_rxcntof_rxfrmtotal_of(x) WriteRegBits(ETH_RXCNTOF,0x80000,19,x)
#define Rd_ETH_rxcntof_rxfrmtotal_of(x) ReadRegBits(ETH_RXCNTOF,0x80000,19)
#define ETH_RXCNTOF_RXFRMTOTAL_OF_MASK                             0x00080000
#define ETH_RXCNTOF_RXFRMTOTAL_OF_ALIGN                            0
#define ETH_RXCNTOF_RXFRMTOTAL_OF_BITS                             1
#define ETH_RXCNTOF_RXFRMTOTAL_OF_SHIFT                            19

/* ETH :: rxcntof :: rxbcastgood_of [18:18] */
#define Wr_ETH_rxcntof_rxbcastgood_of(x) WriteRegBits(ETH_RXCNTOF,0x40000,18,x)
#define Rd_ETH_rxcntof_rxbcastgood_of(x) ReadRegBits(ETH_RXCNTOF,0x40000,18)
#define ETH_RXCNTOF_RXBCASTGOOD_OF_MASK                            0x00040000
#define ETH_RXCNTOF_RXBCASTGOOD_OF_ALIGN                           0
#define ETH_RXCNTOF_RXBCASTGOOD_OF_BITS                            1
#define ETH_RXCNTOF_RXBCASTGOOD_OF_SHIFT                           18

/* ETH :: rxcntof :: rxmcastgood_of [17:17] */
#define Wr_ETH_rxcntof_rxmcastgood_of(x) WriteRegBits(ETH_RXCNTOF,0x20000,17,x)
#define Rd_ETH_rxcntof_rxmcastgood_of(x) ReadRegBits(ETH_RXCNTOF,0x20000,17)
#define ETH_RXCNTOF_RXMCASTGOOD_OF_MASK                            0x00020000
#define ETH_RXCNTOF_RXMCASTGOOD_OF_ALIGN                           0
#define ETH_RXCNTOF_RXMCASTGOOD_OF_BITS                            1
#define ETH_RXCNTOF_RXMCASTGOOD_OF_SHIFT                           17

/* ETH :: rxcntof :: rx64_of [16:16] */
#define Wr_ETH_rxcntof_rx64_of(x) WriteRegBits(ETH_RXCNTOF,0x10000,16,x)
#define Rd_ETH_rxcntof_rx64_of(x) ReadRegBits(ETH_RXCNTOF,0x10000,16)
#define ETH_RXCNTOF_RX64_OF_MASK                                   0x00010000
#define ETH_RXCNTOF_RX64_OF_ALIGN                                  0
#define ETH_RXCNTOF_RX64_OF_BITS                                   1
#define ETH_RXCNTOF_RX64_OF_SHIFT                                  16

/* ETH :: rxcntof :: rx65_127_of [15:15] */
#define Wr_ETH_rxcntof_rx65_127_of(x) WriteRegBits(ETH_RXCNTOF,0x8000,15,x)
#define Rd_ETH_rxcntof_rx65_127_of(x) ReadRegBits(ETH_RXCNTOF,0x8000,15)
#define ETH_RXCNTOF_RX65_127_OF_MASK                               0x00008000
#define ETH_RXCNTOF_RX65_127_OF_ALIGN                              0
#define ETH_RXCNTOF_RX65_127_OF_BITS                               1
#define ETH_RXCNTOF_RX65_127_OF_SHIFT                              15

/* ETH :: rxcntof :: rx128_255_of [14:14] */
#define Wr_ETH_rxcntof_rx128_255_of(x) WriteRegBits(ETH_RXCNTOF,0x4000,14,x)
#define Rd_ETH_rxcntof_rx128_255_of(x) ReadRegBits(ETH_RXCNTOF,0x4000,14)
#define ETH_RXCNTOF_RX128_255_OF_MASK                              0x00004000
#define ETH_RXCNTOF_RX128_255_OF_ALIGN                             0
#define ETH_RXCNTOF_RX128_255_OF_BITS                              1
#define ETH_RXCNTOF_RX128_255_OF_SHIFT                             14

/* ETH :: rxcntof :: rx256_511_of [13:13] */
#define Wr_ETH_rxcntof_rx256_511_of(x) WriteRegBits(ETH_RXCNTOF,0x2000,13,x)
#define Rd_ETH_rxcntof_rx256_511_of(x) ReadRegBits(ETH_RXCNTOF,0x2000,13)
#define ETH_RXCNTOF_RX256_511_OF_MASK                              0x00002000
#define ETH_RXCNTOF_RX256_511_OF_ALIGN                             0
#define ETH_RXCNTOF_RX256_511_OF_BITS                              1
#define ETH_RXCNTOF_RX256_511_OF_SHIFT                             13

/* ETH :: rxcntof :: rx512_1023_of [12:12] */
#define Wr_ETH_rxcntof_rx512_1023_of(x) WriteRegBits(ETH_RXCNTOF,0x1000,12,x)
#define Rd_ETH_rxcntof_rx512_1023_of(x) ReadRegBits(ETH_RXCNTOF,0x1000,12)
#define ETH_RXCNTOF_RX512_1023_OF_MASK                             0x00001000
#define ETH_RXCNTOF_RX512_1023_OF_ALIGN                            0
#define ETH_RXCNTOF_RX512_1023_OF_BITS                             1
#define ETH_RXCNTOF_RX512_1023_OF_SHIFT                            12

/* ETH :: rxcntof :: rx1024_max_of [11:11] */
#define Wr_ETH_rxcntof_rx1024_max_of(x) WriteRegBits(ETH_RXCNTOF,0x800,11,x)
#define Rd_ETH_rxcntof_rx1024_max_of(x) ReadRegBits(ETH_RXCNTOF,0x800,11)
#define ETH_RXCNTOF_RX1024_MAX_OF_MASK                             0x00000800
#define ETH_RXCNTOF_RX1024_MAX_OF_ALIGN                            0
#define ETH_RXCNTOF_RX1024_MAX_OF_BITS                             1
#define ETH_RXCNTOF_RX1024_MAX_OF_SHIFT                            11

/* ETH :: rxcntof :: rxjabber_of [10:10] */
#define Wr_ETH_rxcntof_rxjabber_of(x) WriteRegBits(ETH_RXCNTOF,0x400,10,x)
#define Rd_ETH_rxcntof_rxjabber_of(x) ReadRegBits(ETH_RXCNTOF,0x400,10)
#define ETH_RXCNTOF_RXJABBER_OF_MASK                               0x00000400
#define ETH_RXCNTOF_RXJABBER_OF_ALIGN                              0
#define ETH_RXCNTOF_RXJABBER_OF_BITS                               1
#define ETH_RXCNTOF_RXJABBER_OF_SHIFT                              10

/* ETH :: rxcntof :: rxjumbo_of [09:09] */
#define Wr_ETH_rxcntof_rxjumbo_of(x) WriteRegBits(ETH_RXCNTOF,0x200,9,x)
#define Rd_ETH_rxcntof_rxjumbo_of(x) ReadRegBits(ETH_RXCNTOF,0x200,9)
#define ETH_RXCNTOF_RXJUMBO_OF_MASK                                0x00000200
#define ETH_RXCNTOF_RXJUMBO_OF_ALIGN                               0
#define ETH_RXCNTOF_RXJUMBO_OF_BITS                                1
#define ETH_RXCNTOF_RXJUMBO_OF_SHIFT                               9

/* ETH :: rxcntof :: rxfrag_of [08:08] */
#define Wr_ETH_rxcntof_rxfrag_of(x) WriteRegBits(ETH_RXCNTOF,0x100,8,x)
#define Rd_ETH_rxcntof_rxfrag_of(x) ReadRegBits(ETH_RXCNTOF,0x100,8)
#define ETH_RXCNTOF_RXFRAG_OF_MASK                                 0x00000100
#define ETH_RXCNTOF_RXFRAG_OF_ALIGN                                0
#define ETH_RXCNTOF_RXFRAG_OF_BITS                                 1
#define ETH_RXCNTOF_RXFRAG_OF_SHIFT                                8

/* ETH :: rxcntof :: rxoverrun_of [07:07] */
#define Wr_ETH_rxcntof_rxoverrun_of(x) WriteRegBits(ETH_RXCNTOF,0x80,7,x)
#define Rd_ETH_rxcntof_rxoverrun_of(x) ReadRegBits(ETH_RXCNTOF,0x80,7)
#define ETH_RXCNTOF_RXOVERRUN_OF_MASK                              0x00000080
#define ETH_RXCNTOF_RXOVERRUN_OF_ALIGN                             0
#define ETH_RXCNTOF_RXOVERRUN_OF_BITS                              1
#define ETH_RXCNTOF_RXOVERRUN_OF_SHIFT                             7

/* ETH :: rxcntof :: rxcrcalign_of [06:06] */
#define Wr_ETH_rxcntof_rxcrcalign_of(x) WriteRegBits(ETH_RXCNTOF,0x40,6,x)
#define Rd_ETH_rxcntof_rxcrcalign_of(x) ReadRegBits(ETH_RXCNTOF,0x40,6)
#define ETH_RXCNTOF_RXCRCALIGN_OF_MASK                             0x00000040
#define ETH_RXCNTOF_RXCRCALIGN_OF_ALIGN                            0
#define ETH_RXCNTOF_RXCRCALIGN_OF_BITS                             1
#define ETH_RXCNTOF_RXCRCALIGN_OF_SHIFT                            6

/* ETH :: rxcntof :: rxusize_of [05:05] */
#define Wr_ETH_rxcntof_rxusize_of(x) WriteRegBits(ETH_RXCNTOF,0x20,5,x)
#define Rd_ETH_rxcntof_rxusize_of(x) ReadRegBits(ETH_RXCNTOF,0x20,5)
#define ETH_RXCNTOF_RXUSIZE_OF_MASK                                0x00000020
#define ETH_RXCNTOF_RXUSIZE_OF_ALIGN                               0
#define ETH_RXCNTOF_RXUSIZE_OF_BITS                                1
#define ETH_RXCNTOF_RXUSIZE_OF_SHIFT                               5

/* ETH :: rxcntof :: rxcrc_of [04:04] */
#define Wr_ETH_rxcntof_rxcrc_of(x) WriteRegBits(ETH_RXCNTOF,0x10,4,x)
#define Rd_ETH_rxcntof_rxcrc_of(x) ReadRegBits(ETH_RXCNTOF,0x10,4)
#define ETH_RXCNTOF_RXCRC_OF_MASK                                  0x00000010
#define ETH_RXCNTOF_RXCRC_OF_ALIGN                                 0
#define ETH_RXCNTOF_RXCRC_OF_BITS                                  1
#define ETH_RXCNTOF_RXCRC_OF_SHIFT                                 4

/* ETH :: rxcntof :: rxalign_of [03:03] */
#define Wr_ETH_rxcntof_rxalign_of(x) WriteRegBits(ETH_RXCNTOF,0x8,3,x)
#define Rd_ETH_rxcntof_rxalign_of(x) ReadRegBits(ETH_RXCNTOF,0x8,3)
#define ETH_RXCNTOF_RXALIGN_OF_MASK                                0x00000008
#define ETH_RXCNTOF_RXALIGN_OF_ALIGN                               0
#define ETH_RXCNTOF_RXALIGN_OF_BITS                                1
#define ETH_RXCNTOF_RXALIGN_OF_SHIFT                               3

/* ETH :: rxcntof :: rxcderr_of [02:02] */
#define Wr_ETH_rxcntof_rxcderr_of(x) WriteRegBits(ETH_RXCNTOF,0x4,2,x)
#define Rd_ETH_rxcntof_rxcderr_of(x) ReadRegBits(ETH_RXCNTOF,0x4,2)
#define ETH_RXCNTOF_RXCDERR_OF_MASK                                0x00000004
#define ETH_RXCNTOF_RXCDERR_OF_ALIGN                               0
#define ETH_RXCNTOF_RXCDERR_OF_BITS                                1
#define ETH_RXCNTOF_RXCDERR_OF_SHIFT                               2

/* ETH :: rxcntof :: rxpausefm_of [01:01] */
#define Wr_ETH_rxcntof_rxpausefm_of(x) WriteRegBits(ETH_RXCNTOF,0x2,1,x)
#define Rd_ETH_rxcntof_rxpausefm_of(x) ReadRegBits(ETH_RXCNTOF,0x2,1)
#define ETH_RXCNTOF_RXPAUSEFM_OF_MASK                              0x00000002
#define ETH_RXCNTOF_RXPAUSEFM_OF_ALIGN                             0
#define ETH_RXCNTOF_RXPAUSEFM_OF_BITS                              1
#define ETH_RXCNTOF_RXPAUSEFM_OF_SHIFT                             1

/* ETH :: rxcntof :: rxctrlfm_of [00:00] */
#define Wr_ETH_rxcntof_rxctrlfm_of(x) WriteRegBits(ETH_RXCNTOF,0x1,0,x)
#define Rd_ETH_rxcntof_rxctrlfm_of(x) ReadRegBits(ETH_RXCNTOF,0x1,0)
#define ETH_RXCNTOF_RXCTRLFM_OF_MASK                               0x00000001
#define ETH_RXCNTOF_RXCTRLFM_OF_ALIGN                              0
#define ETH_RXCNTOF_RXCTRLFM_OF_BITS                               1
#define ETH_RXCNTOF_RXCTRLFM_OF_SHIFT                              0

/****************************************************************************
 * bcm89530_sys_cfg_SPI1
 ***************************************************************************/
/****************************************************************************
 * SPI1 :: SSPCR0
 ***************************************************************************/
/* SPI1 :: SSPCR0 :: reserved0 [31:16] */
#define SPI1_SSPCR0_RESERVED0_MASK                                 0xffff0000
#define SPI1_SSPCR0_RESERVED0_ALIGN                                0
#define SPI1_SSPCR0_RESERVED0_BITS                                 16
#define SPI1_SSPCR0_RESERVED0_SHIFT                                16

/* SPI1 :: SSPCR0 :: SCR [15:08] */
#define Wr_SPI1_SSPCR0_SCR(x) WriteRegBits(SPI1_SSPCR0,0xff00,8,x)
#define Rd_SPI1_SSPCR0_SCR(x) ReadRegBits(SPI1_SSPCR0,0xff00,8)
#define SPI1_SSPCR0_SCR_MASK                                       0x0000ff00
#define SPI1_SSPCR0_SCR_ALIGN                                      0
#define SPI1_SSPCR0_SCR_BITS                                       8
#define SPI1_SSPCR0_SCR_SHIFT                                      8

/* SPI1 :: SSPCR0 :: SPH [07:07] */
#define Wr_SPI1_SSPCR0_SPH(x) WriteRegBits(SPI1_SSPCR0,0x80,7,x)
#define Rd_SPI1_SSPCR0_SPH(x) ReadRegBits(SPI1_SSPCR0,0x80,7)
#define SPI1_SSPCR0_SPH_MASK                                       0x00000080
#define SPI1_SSPCR0_SPH_ALIGN                                      0
#define SPI1_SSPCR0_SPH_BITS                                       1
#define SPI1_SSPCR0_SPH_SHIFT                                      7

/* SPI1 :: SSPCR0 :: SPO [06:06] */
#define Wr_SPI1_SSPCR0_SPO(x) WriteRegBits(SPI1_SSPCR0,0x40,6,x)
#define Rd_SPI1_SSPCR0_SPO(x) ReadRegBits(SPI1_SSPCR0,0x40,6)
#define SPI1_SSPCR0_SPO_MASK                                       0x00000040
#define SPI1_SSPCR0_SPO_ALIGN                                      0
#define SPI1_SSPCR0_SPO_BITS                                       1
#define SPI1_SSPCR0_SPO_SHIFT                                      6

/* SPI1 :: SSPCR0 :: FRF [05:04] */
#define Wr_SPI1_SSPCR0_FRF(x) WriteRegBits(SPI1_SSPCR0,0x30,4,x)
#define Rd_SPI1_SSPCR0_FRF(x) ReadRegBits(SPI1_SSPCR0,0x30,4)
#define SPI1_SSPCR0_FRF_MASK                                       0x00000030
#define SPI1_SSPCR0_FRF_ALIGN                                      0
#define SPI1_SSPCR0_FRF_BITS                                       2
#define SPI1_SSPCR0_FRF_SHIFT                                      4

/* SPI1 :: SSPCR0 :: DSS [03:00] */
#define Wr_SPI1_SSPCR0_DSS(x) WriteRegBits(SPI1_SSPCR0,0xf,0,x)
#define Rd_SPI1_SSPCR0_DSS(x) ReadRegBits(SPI1_SSPCR0,0xf,0)
#define SPI1_SSPCR0_DSS_MASK                                       0x0000000f
#define SPI1_SSPCR0_DSS_ALIGN                                      0
#define SPI1_SSPCR0_DSS_BITS                                       4
#define SPI1_SSPCR0_DSS_SHIFT                                      0


/****************************************************************************
 * SPI1 :: SSPCR1
 ***************************************************************************/
/* SPI1 :: SSPCR1 :: reserved0 [31:04] */
#define SPI1_SSPCR1_RESERVED0_MASK                                 0xfffffff0
#define SPI1_SSPCR1_RESERVED0_ALIGN                                0
#define SPI1_SSPCR1_RESERVED0_BITS                                 28
#define SPI1_SSPCR1_RESERVED0_SHIFT                                4

/* SPI1 :: SSPCR1 :: SOD [03:03] */
#define Wr_SPI1_SSPCR1_SOD(x) WriteRegBits(SPI1_SSPCR1,0x8,3,x)
#define Rd_SPI1_SSPCR1_SOD(x) ReadRegBits(SPI1_SSPCR1,0x8,3)
#define SPI1_SSPCR1_SOD_MASK                                       0x00000008
#define SPI1_SSPCR1_SOD_ALIGN                                      0
#define SPI1_SSPCR1_SOD_BITS                                       1
#define SPI1_SSPCR1_SOD_SHIFT                                      3

/* SPI1 :: SSPCR1 :: MS [02:02] */
#define Wr_SPI1_SSPCR1_MS(x) WriteRegBits(SPI1_SSPCR1,0x4,2,x)
#define Rd_SPI1_SSPCR1_MS(x) ReadRegBits(SPI1_SSPCR1,0x4,2)
#define SPI1_SSPCR1_MS_MASK                                        0x00000004
#define SPI1_SSPCR1_MS_ALIGN                                       0
#define SPI1_SSPCR1_MS_BITS                                        1
#define SPI1_SSPCR1_MS_SHIFT                                       2

/* SPI1 :: SSPCR1 :: SSE [01:01] */
#define Wr_SPI1_SSPCR1_SSE(x) WriteRegBits(SPI1_SSPCR1,0x2,1,x)
#define Rd_SPI1_SSPCR1_SSE(x) ReadRegBits(SPI1_SSPCR1,0x2,1)
#define SPI1_SSPCR1_SSE_MASK                                       0x00000002
#define SPI1_SSPCR1_SSE_ALIGN                                      0
#define SPI1_SSPCR1_SSE_BITS                                       1
#define SPI1_SSPCR1_SSE_SHIFT                                      1

/* SPI1 :: SSPCR1 :: reserved1 [00:00] */
#define SPI1_SSPCR1_RESERVED1_MASK                                 0x00000001
#define SPI1_SSPCR1_RESERVED1_ALIGN                                0
#define SPI1_SSPCR1_RESERVED1_BITS                                 1
#define SPI1_SSPCR1_RESERVED1_SHIFT                                0


/****************************************************************************
 * SPI1 :: SSPDR
 ***************************************************************************/
/* SPI1 :: SSPDR :: reserved0 [31:16] */
#define SPI1_SSPDR_RESERVED0_MASK                                  0xffff0000
#define SPI1_SSPDR_RESERVED0_ALIGN                                 0
#define SPI1_SSPDR_RESERVED0_BITS                                  16
#define SPI1_SSPDR_RESERVED0_SHIFT                                 16

/* SPI1 :: SSPDR :: DATA [15:00] */
#define Wr_SPI1_SSPDR_DATA(x) WriteRegBits(SPI1_SSPDR,0xffff,0,x)
#define Rd_SPI1_SSPDR_DATA(x) ReadRegBits(SPI1_SSPDR,0xffff,0)
#define SPI1_SSPDR_DATA_MASK                                       0x0000ffff
#define SPI1_SSPDR_DATA_ALIGN                                      0
#define SPI1_SSPDR_DATA_BITS                                       16
#define SPI1_SSPDR_DATA_SHIFT                                      0


/****************************************************************************
 * SPI1 :: SSPSR
 ***************************************************************************/
/* SPI1 :: SSPSR :: reserved0 [31:05] */
#define SPI1_SSPSR_RESERVED0_MASK                                  0xffffffe0
#define SPI1_SSPSR_RESERVED0_ALIGN                                 0
#define SPI1_SSPSR_RESERVED0_BITS                                  27
#define SPI1_SSPSR_RESERVED0_SHIFT                                 5

/* SPI1 :: SSPSR :: BSY [04:04] */
#define Wr_SPI1_SSPSR_BSY(x) WriteRegBits(SPI1_SSPSR,0x10,4,x)
#define Rd_SPI1_SSPSR_BSY(x) ReadRegBits(SPI1_SSPSR,0x10,4)
#define SPI1_SSPSR_BSY_MASK                                        0x00000010
#define SPI1_SSPSR_BSY_ALIGN                                       0
#define SPI1_SSPSR_BSY_BITS                                        1
#define SPI1_SSPSR_BSY_SHIFT                                       4

/* SPI1 :: SSPSR :: RFF [03:03] */
#define Wr_SPI1_SSPSR_RFF(x) WriteRegBits(SPI1_SSPSR,0x8,3,x)
#define Rd_SPI1_SSPSR_RFF(x) ReadRegBits(SPI1_SSPSR,0x8,3)
#define SPI1_SSPSR_RFF_MASK                                        0x00000008
#define SPI1_SSPSR_RFF_ALIGN                                       0
#define SPI1_SSPSR_RFF_BITS                                        1
#define SPI1_SSPSR_RFF_SHIFT                                       3

/* SPI1 :: SSPSR :: RNE [02:02] */
#define Wr_SPI1_SSPSR_RNE(x) WriteRegBits(SPI1_SSPSR,0x4,2,x)
#define Rd_SPI1_SSPSR_RNE(x) ReadRegBits(SPI1_SSPSR,0x4,2)
#define SPI1_SSPSR_RNE_MASK                                        0x00000004
#define SPI1_SSPSR_RNE_ALIGN                                       0
#define SPI1_SSPSR_RNE_BITS                                        1
#define SPI1_SSPSR_RNE_SHIFT                                       2

/* SPI1 :: SSPSR :: TNF [01:01] */
#define Wr_SPI1_SSPSR_TNF(x) WriteRegBits(SPI1_SSPSR,0x2,1,x)
#define Rd_SPI1_SSPSR_TNF(x) ReadRegBits(SPI1_SSPSR,0x2,1)
#define SPI1_SSPSR_TNF_MASK                                        0x00000002
#define SPI1_SSPSR_TNF_ALIGN                                       0
#define SPI1_SSPSR_TNF_BITS                                        1
#define SPI1_SSPSR_TNF_SHIFT                                       1

/* SPI1 :: SSPSR :: TFE [00:00] */
#define Wr_SPI1_SSPSR_TFE(x) WriteRegBits(SPI1_SSPSR,0x1,0,x)
#define Rd_SPI1_SSPSR_TFE(x) ReadRegBits(SPI1_SSPSR,0x1,0)
#define SPI1_SSPSR_TFE_MASK                                        0x00000001
#define SPI1_SSPSR_TFE_ALIGN                                       0
#define SPI1_SSPSR_TFE_BITS                                        1
#define SPI1_SSPSR_TFE_SHIFT                                       0


/****************************************************************************
 * SPI1 :: SSPCPSR
 ***************************************************************************/
/* SPI1 :: SSPCPSR :: reserved0 [31:08] */
#define SPI1_SSPCPSR_RESERVED0_MASK                                0xffffff00
#define SPI1_SSPCPSR_RESERVED0_ALIGN                               0
#define SPI1_SSPCPSR_RESERVED0_BITS                                24
#define SPI1_SSPCPSR_RESERVED0_SHIFT                               8

/* SPI1 :: SSPCPSR :: CPSDVSR [07:00] */
#define Wr_SPI1_SSPCPSR_CPSDVSR(x) WriteRegBits(SPI1_SSPCPSR,0xff,0,x)
#define Rd_SPI1_SSPCPSR_CPSDVSR(x) ReadRegBits(SPI1_SSPCPSR,0xff,0)
#define SPI1_SSPCPSR_CPSDVSR_MASK                                  0x000000ff
#define SPI1_SSPCPSR_CPSDVSR_ALIGN                                 0
#define SPI1_SSPCPSR_CPSDVSR_BITS                                  8
#define SPI1_SSPCPSR_CPSDVSR_SHIFT                                 0


/****************************************************************************
 * SPI1 :: SSPIMSC
 ***************************************************************************/
/* SPI1 :: SSPIMSC :: reserved0 [31:04] */
#define SPI1_SSPIMSC_RESERVED0_MASK                                0xfffffff0
#define SPI1_SSPIMSC_RESERVED0_ALIGN                               0
#define SPI1_SSPIMSC_RESERVED0_BITS                                28
#define SPI1_SSPIMSC_RESERVED0_SHIFT                               4

/* SPI1 :: SSPIMSC :: TXIM [03:03] */
#define Wr_SPI1_SSPIMSC_TXIM(x) WriteRegBits(SPI1_SSPIMSC,0x8,3,x)
#define Rd_SPI1_SSPIMSC_TXIM(x) ReadRegBits(SPI1_SSPIMSC,0x8,3)
#define SPI1_SSPIMSC_TXIM_MASK                                     0x00000008
#define SPI1_SSPIMSC_TXIM_ALIGN                                    0
#define SPI1_SSPIMSC_TXIM_BITS                                     1
#define SPI1_SSPIMSC_TXIM_SHIFT                                    3

/* SPI1 :: SSPIMSC :: RXIM [02:02] */
#define Wr_SPI1_SSPIMSC_RXIM(x) WriteRegBits(SPI1_SSPIMSC,0x4,2,x)
#define Rd_SPI1_SSPIMSC_RXIM(x) ReadRegBits(SPI1_SSPIMSC,0x4,2)
#define SPI1_SSPIMSC_RXIM_MASK                                     0x00000004
#define SPI1_SSPIMSC_RXIM_ALIGN                                    0
#define SPI1_SSPIMSC_RXIM_BITS                                     1
#define SPI1_SSPIMSC_RXIM_SHIFT                                    2

/* SPI1 :: SSPIMSC :: RTIM [01:01] */
#define Wr_SPI1_SSPIMSC_RTIM(x) WriteRegBits(SPI1_SSPIMSC,0x2,1,x)
#define Rd_SPI1_SSPIMSC_RTIM(x) ReadRegBits(SPI1_SSPIMSC,0x2,1)
#define SPI1_SSPIMSC_RTIM_MASK                                     0x00000002
#define SPI1_SSPIMSC_RTIM_ALIGN                                    0
#define SPI1_SSPIMSC_RTIM_BITS                                     1
#define SPI1_SSPIMSC_RTIM_SHIFT                                    1

/* SPI1 :: SSPIMSC :: RORIM [00:00] */
#define Wr_SPI1_SSPIMSC_RORIM(x) WriteRegBits(SPI1_SSPIMSC,0x1,0,x)
#define Rd_SPI1_SSPIMSC_RORIM(x) ReadRegBits(SPI1_SSPIMSC,0x1,0)
#define SPI1_SSPIMSC_RORIM_MASK                                    0x00000001
#define SPI1_SSPIMSC_RORIM_ALIGN                                   0
#define SPI1_SSPIMSC_RORIM_BITS                                    1
#define SPI1_SSPIMSC_RORIM_SHIFT                                   0


/****************************************************************************
 * SPI1 :: SSPRIS
 ***************************************************************************/
/* SPI1 :: SSPRIS :: reserved0 [31:04] */
#define SPI1_SSPRIS_RESERVED0_MASK                                 0xfffffff0
#define SPI1_SSPRIS_RESERVED0_ALIGN                                0
#define SPI1_SSPRIS_RESERVED0_BITS                                 28
#define SPI1_SSPRIS_RESERVED0_SHIFT                                4

/* SPI1 :: SSPRIS :: TXRIS [03:03] */
#define Wr_SPI1_SSPRIS_TXRIS(x) WriteRegBits(SPI1_SSPRIS,0x8,3,x)
#define Rd_SPI1_SSPRIS_TXRIS(x) ReadRegBits(SPI1_SSPRIS,0x8,3)
#define SPI1_SSPRIS_TXRIS_MASK                                     0x00000008
#define SPI1_SSPRIS_TXRIS_ALIGN                                    0
#define SPI1_SSPRIS_TXRIS_BITS                                     1
#define SPI1_SSPRIS_TXRIS_SHIFT                                    3

/* SPI1 :: SSPRIS :: RXRIS [02:02] */
#define Wr_SPI1_SSPRIS_RXRIS(x) WriteRegBits(SPI1_SSPRIS,0x4,2,x)
#define Rd_SPI1_SSPRIS_RXRIS(x) ReadRegBits(SPI1_SSPRIS,0x4,2)
#define SPI1_SSPRIS_RXRIS_MASK                                     0x00000004
#define SPI1_SSPRIS_RXRIS_ALIGN                                    0
#define SPI1_SSPRIS_RXRIS_BITS                                     1
#define SPI1_SSPRIS_RXRIS_SHIFT                                    2

/* SPI1 :: SSPRIS :: RTRIS [01:01] */
#define Wr_SPI1_SSPRIS_RTRIS(x) WriteRegBits(SPI1_SSPRIS,0x2,1,x)
#define Rd_SPI1_SSPRIS_RTRIS(x) ReadRegBits(SPI1_SSPRIS,0x2,1)
#define SPI1_SSPRIS_RTRIS_MASK                                     0x00000002
#define SPI1_SSPRIS_RTRIS_ALIGN                                    0
#define SPI1_SSPRIS_RTRIS_BITS                                     1
#define SPI1_SSPRIS_RTRIS_SHIFT                                    1

/* SPI1 :: SSPRIS :: RORRIS [00:00] */
#define Wr_SPI1_SSPRIS_RORRIS(x) WriteRegBits(SPI1_SSPRIS,0x1,0,x)
#define Rd_SPI1_SSPRIS_RORRIS(x) ReadRegBits(SPI1_SSPRIS,0x1,0)
#define SPI1_SSPRIS_RORRIS_MASK                                    0x00000001
#define SPI1_SSPRIS_RORRIS_ALIGN                                   0
#define SPI1_SSPRIS_RORRIS_BITS                                    1
#define SPI1_SSPRIS_RORRIS_SHIFT                                   0


/****************************************************************************
 * SPI1 :: SSPMIS
 ***************************************************************************/
/* SPI1 :: SSPMIS :: reserved0 [31:04] */
#define SPI1_SSPMIS_RESERVED0_MASK                                 0xfffffff0
#define SPI1_SSPMIS_RESERVED0_ALIGN                                0
#define SPI1_SSPMIS_RESERVED0_BITS                                 28
#define SPI1_SSPMIS_RESERVED0_SHIFT                                4

/* SPI1 :: SSPMIS :: TXMIS [03:03] */
#define Wr_SPI1_SSPMIS_TXMIS(x) WriteRegBits(SPI1_SSPMIS,0x8,3,x)
#define Rd_SPI1_SSPMIS_TXMIS(x) ReadRegBits(SPI1_SSPMIS,0x8,3)
#define SPI1_SSPMIS_TXMIS_MASK                                     0x00000008
#define SPI1_SSPMIS_TXMIS_ALIGN                                    0
#define SPI1_SSPMIS_TXMIS_BITS                                     1
#define SPI1_SSPMIS_TXMIS_SHIFT                                    3

/* SPI1 :: SSPMIS :: RXMIS [02:02] */
#define Wr_SPI1_SSPMIS_RXMIS(x) WriteRegBits(SPI1_SSPMIS,0x4,2,x)
#define Rd_SPI1_SSPMIS_RXMIS(x) ReadRegBits(SPI1_SSPMIS,0x4,2)
#define SPI1_SSPMIS_RXMIS_MASK                                     0x00000004
#define SPI1_SSPMIS_RXMIS_ALIGN                                    0
#define SPI1_SSPMIS_RXMIS_BITS                                     1
#define SPI1_SSPMIS_RXMIS_SHIFT                                    2

/* SPI1 :: SSPMIS :: RTMIS [01:01] */
#define Wr_SPI1_SSPMIS_RTMIS(x) WriteRegBits(SPI1_SSPMIS,0x2,1,x)
#define Rd_SPI1_SSPMIS_RTMIS(x) ReadRegBits(SPI1_SSPMIS,0x2,1)
#define SPI1_SSPMIS_RTMIS_MASK                                     0x00000002
#define SPI1_SSPMIS_RTMIS_ALIGN                                    0
#define SPI1_SSPMIS_RTMIS_BITS                                     1
#define SPI1_SSPMIS_RTMIS_SHIFT                                    1

/* SPI1 :: SSPMIS :: RORMIS [00:00] */
#define Wr_SPI1_SSPMIS_RORMIS(x) WriteRegBits(SPI1_SSPMIS,0x1,0,x)
#define Rd_SPI1_SSPMIS_RORMIS(x) ReadRegBits(SPI1_SSPMIS,0x1,0)
#define SPI1_SSPMIS_RORMIS_MASK                                    0x00000001
#define SPI1_SSPMIS_RORMIS_ALIGN                                   0
#define SPI1_SSPMIS_RORMIS_BITS                                    1
#define SPI1_SSPMIS_RORMIS_SHIFT                                   0


/****************************************************************************
 * SPI1 :: SSPICR
 ***************************************************************************/
/* SPI1 :: SSPICR :: reserved0 [31:02] */
#define SPI1_SSPICR_RESERVED0_MASK                                 0xfffffffc
#define SPI1_SSPICR_RESERVED0_ALIGN                                0
#define SPI1_SSPICR_RESERVED0_BITS                                 30
#define SPI1_SSPICR_RESERVED0_SHIFT                                2

/* SPI1 :: SSPICR :: RTIC [01:01] */
#define Wr_SPI1_SSPICR_RTIC(x) WriteRegBits(SPI1_SSPICR,0x2,1,x)
#define Rd_SPI1_SSPICR_RTIC(x) ReadRegBits(SPI1_SSPICR,0x2,1)
#define SPI1_SSPICR_RTIC_MASK                                      0x00000002
#define SPI1_SSPICR_RTIC_ALIGN                                     0
#define SPI1_SSPICR_RTIC_BITS                                      1
#define SPI1_SSPICR_RTIC_SHIFT                                     1

/* SPI1 :: SSPICR :: RORIC [00:00] */
#define Wr_SPI1_SSPICR_RORIC(x) WriteRegBits(SPI1_SSPICR,0x1,0,x)
#define Rd_SPI1_SSPICR_RORIC(x) ReadRegBits(SPI1_SSPICR,0x1,0)
#define SPI1_SSPICR_RORIC_MASK                                     0x00000001
#define SPI1_SSPICR_RORIC_ALIGN                                    0
#define SPI1_SSPICR_RORIC_BITS                                     1
#define SPI1_SSPICR_RORIC_SHIFT                                    0


/****************************************************************************
 * SPI1 :: SSPDMACR
 ***************************************************************************/
/* SPI1 :: SSPDMACR :: reserved0 [31:02] */
#define SPI1_SSPDMACR_RESERVED0_MASK                               0xfffffffc
#define SPI1_SSPDMACR_RESERVED0_ALIGN                              0
#define SPI1_SSPDMACR_RESERVED0_BITS                               30
#define SPI1_SSPDMACR_RESERVED0_SHIFT                              2

/* SPI1 :: SSPDMACR :: TXDMAE [01:01] */
#define Wr_SPI1_SSPDMACR_TXDMAE(x) WriteRegBits(SPI1_SSPDMACR,0x2,1,x)
#define Rd_SPI1_SSPDMACR_TXDMAE(x) ReadRegBits(SPI1_SSPDMACR,0x2,1)
#define SPI1_SSPDMACR_TXDMAE_MASK                                  0x00000002
#define SPI1_SSPDMACR_TXDMAE_ALIGN                                 0
#define SPI1_SSPDMACR_TXDMAE_BITS                                  1
#define SPI1_SSPDMACR_TXDMAE_SHIFT                                 1

/* SPI1 :: SSPDMACR :: RXDMAE [00:00] */
#define Wr_SPI1_SSPDMACR_RXDMAE(x) WriteRegBits(SPI1_SSPDMACR,0x1,0,x)
#define Rd_SPI1_SSPDMACR_RXDMAE(x) ReadRegBits(SPI1_SSPDMACR,0x1,0)
#define SPI1_SSPDMACR_RXDMAE_MASK                                  0x00000001
#define SPI1_SSPDMACR_RXDMAE_ALIGN                                 0
#define SPI1_SSPDMACR_RXDMAE_BITS                                  1
#define SPI1_SSPDMACR_RXDMAE_SHIFT                                 0


/****************************************************************************
 * SPI1 :: SSPPeriphID0
 ***************************************************************************/
/* SPI1 :: SSPPeriphID0 :: reserved0 [31:08] */
#define SPI1_SSPPERIPHID0_RESERVED0_MASK                           0xffffff00
#define SPI1_SSPPERIPHID0_RESERVED0_ALIGN                          0
#define SPI1_SSPPERIPHID0_RESERVED0_BITS                           24
#define SPI1_SSPPERIPHID0_RESERVED0_SHIFT                          8

/* SPI1 :: SSPPeriphID0 :: PartNumber0 [07:00] */
#define Wr_SPI1_SSPPeriphID0_PartNumber0(x) WriteRegBits(SPI1_SSPPERIPHID0,0xff,0,x)
#define Rd_SPI1_SSPPeriphID0_PartNumber0(x) ReadRegBits(SPI1_SSPPERIPHID0,0xff,0)
#define SPI1_SSPPERIPHID0_PARTNUMBER0_MASK                         0x000000ff
#define SPI1_SSPPERIPHID0_PARTNUMBER0_ALIGN                        0
#define SPI1_SSPPERIPHID0_PARTNUMBER0_BITS                         8
#define SPI1_SSPPERIPHID0_PARTNUMBER0_SHIFT                        0


/****************************************************************************
 * SPI1 :: SSPPeriphID1
 ***************************************************************************/
/* SPI1 :: SSPPeriphID1 :: reserved0 [31:08] */
#define SPI1_SSPPERIPHID1_RESERVED0_MASK                           0xffffff00
#define SPI1_SSPPERIPHID1_RESERVED0_ALIGN                          0
#define SPI1_SSPPERIPHID1_RESERVED0_BITS                           24
#define SPI1_SSPPERIPHID1_RESERVED0_SHIFT                          8

/* SPI1 :: SSPPeriphID1 :: Designer0 [07:04] */
#define Wr_SPI1_SSPPeriphID1_Designer0(x) WriteRegBits(SPI1_SSPPERIPHID1,0xf0,4,x)
#define Rd_SPI1_SSPPeriphID1_Designer0(x) ReadRegBits(SPI1_SSPPERIPHID1,0xf0,4)
#define SPI1_SSPPERIPHID1_DESIGNER0_MASK                           0x000000f0
#define SPI1_SSPPERIPHID1_DESIGNER0_ALIGN                          0
#define SPI1_SSPPERIPHID1_DESIGNER0_BITS                           4
#define SPI1_SSPPERIPHID1_DESIGNER0_SHIFT                          4

/* SPI1 :: SSPPeriphID1 :: PartNumber1 [03:00] */
#define Wr_SPI1_SSPPeriphID1_PartNumber1(x) WriteRegBits(SPI1_SSPPERIPHID1,0xf,0,x)
#define Rd_SPI1_SSPPeriphID1_PartNumber1(x) ReadRegBits(SPI1_SSPPERIPHID1,0xf,0)
#define SPI1_SSPPERIPHID1_PARTNUMBER1_MASK                         0x0000000f
#define SPI1_SSPPERIPHID1_PARTNUMBER1_ALIGN                        0
#define SPI1_SSPPERIPHID1_PARTNUMBER1_BITS                         4
#define SPI1_SSPPERIPHID1_PARTNUMBER1_SHIFT                        0


/****************************************************************************
 * SPI1 :: SSPPeriphID2
 ***************************************************************************/
/* SPI1 :: SSPPeriphID2 :: reserved0 [31:08] */
#define SPI1_SSPPERIPHID2_RESERVED0_MASK                           0xffffff00
#define SPI1_SSPPERIPHID2_RESERVED0_ALIGN                          0
#define SPI1_SSPPERIPHID2_RESERVED0_BITS                           24
#define SPI1_SSPPERIPHID2_RESERVED0_SHIFT                          8

/* SPI1 :: SSPPeriphID2 :: Revision [07:04] */
#define Wr_SPI1_SSPPeriphID2_Revision(x) WriteRegBits(SPI1_SSPPERIPHID2,0xf0,4,x)
#define Rd_SPI1_SSPPeriphID2_Revision(x) ReadRegBits(SPI1_SSPPERIPHID2,0xf0,4)
#define SPI1_SSPPERIPHID2_REVISION_MASK                            0x000000f0
#define SPI1_SSPPERIPHID2_REVISION_ALIGN                           0
#define SPI1_SSPPERIPHID2_REVISION_BITS                            4
#define SPI1_SSPPERIPHID2_REVISION_SHIFT                           4

/* SPI1 :: SSPPeriphID2 :: Designer1 [03:00] */
#define Wr_SPI1_SSPPeriphID2_Designer1(x) WriteRegBits(SPI1_SSPPERIPHID2,0xf,0,x)
#define Rd_SPI1_SSPPeriphID2_Designer1(x) ReadRegBits(SPI1_SSPPERIPHID2,0xf,0)
#define SPI1_SSPPERIPHID2_DESIGNER1_MASK                           0x0000000f
#define SPI1_SSPPERIPHID2_DESIGNER1_ALIGN                          0
#define SPI1_SSPPERIPHID2_DESIGNER1_BITS                           4
#define SPI1_SSPPERIPHID2_DESIGNER1_SHIFT                          0


/****************************************************************************
 * SPI1 :: SSPPeriphID3
 ***************************************************************************/
/* SPI1 :: SSPPeriphID3 :: reserved0 [31:08] */
#define SPI1_SSPPERIPHID3_RESERVED0_MASK                           0xffffff00
#define SPI1_SSPPERIPHID3_RESERVED0_ALIGN                          0
#define SPI1_SSPPERIPHID3_RESERVED0_BITS                           24
#define SPI1_SSPPERIPHID3_RESERVED0_SHIFT                          8

/* SPI1 :: SSPPeriphID3 :: Configuration [07:00] */
#define Wr_SPI1_SSPPeriphID3_Configuration(x) WriteRegBits(SPI1_SSPPERIPHID3,0xff,0,x)
#define Rd_SPI1_SSPPeriphID3_Configuration(x) ReadRegBits(SPI1_SSPPERIPHID3,0xff,0)
#define SPI1_SSPPERIPHID3_CONFIGURATION_MASK                       0x000000ff
#define SPI1_SSPPERIPHID3_CONFIGURATION_ALIGN                      0
#define SPI1_SSPPERIPHID3_CONFIGURATION_BITS                       8
#define SPI1_SSPPERIPHID3_CONFIGURATION_SHIFT                      0


/****************************************************************************
 * SPI1 :: SSPPCellID0
 ***************************************************************************/
/* SPI1 :: SSPPCellID0 :: reserved0 [31:08] */
#define SPI1_SSPPCELLID0_RESERVED0_MASK                            0xffffff00
#define SPI1_SSPPCELLID0_RESERVED0_ALIGN                           0
#define SPI1_SSPPCELLID0_RESERVED0_BITS                            24
#define SPI1_SSPPCELLID0_RESERVED0_SHIFT                           8

/* SPI1 :: SSPPCellID0 :: SSPPCellID0 [07:00] */
#define Wr_SPI1_SSPPCellID0_SSPPCellID0(x) WriteRegBits(SPI1_SSPPCELLID0,0xff,0,x)
#define Rd_SPI1_SSPPCellID0_SSPPCellID0(x) ReadRegBits(SPI1_SSPPCELLID0,0xff,0)
#define SPI1_SSPPCELLID0_SSPPCELLID0_MASK                          0x000000ff
#define SPI1_SSPPCELLID0_SSPPCELLID0_ALIGN                         0
#define SPI1_SSPPCELLID0_SSPPCELLID0_BITS                          8
#define SPI1_SSPPCELLID0_SSPPCELLID0_SHIFT                         0


/****************************************************************************
 * SPI1 :: SSPPCellID1
 ***************************************************************************/
/* SPI1 :: SSPPCellID1 :: reserved0 [31:08] */
#define SPI1_SSPPCELLID1_RESERVED0_MASK                            0xffffff00
#define SPI1_SSPPCELLID1_RESERVED0_ALIGN                           0
#define SPI1_SSPPCELLID1_RESERVED0_BITS                            24
#define SPI1_SSPPCELLID1_RESERVED0_SHIFT                           8

/* SPI1 :: SSPPCellID1 :: SSPPCellID1 [07:00] */
#define Wr_SPI1_SSPPCellID1_SSPPCellID1(x) WriteRegBits(SPI1_SSPPCELLID1,0xff,0,x)
#define Rd_SPI1_SSPPCellID1_SSPPCellID1(x) ReadRegBits(SPI1_SSPPCELLID1,0xff,0)
#define SPI1_SSPPCELLID1_SSPPCELLID1_MASK                          0x000000ff
#define SPI1_SSPPCELLID1_SSPPCELLID1_ALIGN                         0
#define SPI1_SSPPCELLID1_SSPPCELLID1_BITS                          8
#define SPI1_SSPPCELLID1_SSPPCELLID1_SHIFT                         0


/****************************************************************************
 * SPI1 :: SSPPCellID2
 ***************************************************************************/
/* SPI1 :: SSPPCellID2 :: reserved0 [31:08] */
#define SPI1_SSPPCELLID2_RESERVED0_MASK                            0xffffff00
#define SPI1_SSPPCELLID2_RESERVED0_ALIGN                           0
#define SPI1_SSPPCELLID2_RESERVED0_BITS                            24
#define SPI1_SSPPCELLID2_RESERVED0_SHIFT                           8

/* SPI1 :: SSPPCellID2 :: SSPPCellID2 [07:00] */
#define Wr_SPI1_SSPPCellID2_SSPPCellID2(x) WriteRegBits(SPI1_SSPPCELLID2,0xff,0,x)
#define Rd_SPI1_SSPPCellID2_SSPPCellID2(x) ReadRegBits(SPI1_SSPPCELLID2,0xff,0)
#define SPI1_SSPPCELLID2_SSPPCELLID2_MASK                          0x000000ff
#define SPI1_SSPPCELLID2_SSPPCELLID2_ALIGN                         0
#define SPI1_SSPPCELLID2_SSPPCELLID2_BITS                          8
#define SPI1_SSPPCELLID2_SSPPCELLID2_SHIFT                         0


/****************************************************************************
 * SPI1 :: SSPPCellID3
 ***************************************************************************/
/* SPI1 :: SSPPCellID3 :: reserved0 [31:08] */
#define SPI1_SSPPCELLID3_RESERVED0_MASK                            0xffffff00
#define SPI1_SSPPCELLID3_RESERVED0_ALIGN                           0
#define SPI1_SSPPCELLID3_RESERVED0_BITS                            24
#define SPI1_SSPPCELLID3_RESERVED0_SHIFT                           8

/* SPI1 :: SSPPCellID3 :: SSPPCellID3 [07:00] */
#define Wr_SPI1_SSPPCellID3_SSPPCellID3(x) WriteRegBits(SPI1_SSPPCELLID3,0xff,0,x)
#define Rd_SPI1_SSPPCellID3_SSPPCellID3(x) ReadRegBits(SPI1_SSPPCELLID3,0xff,0)
#define SPI1_SSPPCELLID3_SSPPCELLID3_MASK                          0x000000ff
#define SPI1_SSPPCELLID3_SSPPCELLID3_ALIGN                         0
#define SPI1_SSPPCELLID3_SSPPCELLID3_BITS                          8
#define SPI1_SSPPCELLID3_SSPPCELLID3_SHIFT                         0

/****************************************************************************
 * MISC :: deviceid_lo
 ***************************************************************************/
/* MISC :: deviceid_lo :: reserved0 [15:12] */
#define MISC_DEVICEID_LO_RESERVED0_MASK                            0xf000
#define MISC_DEVICEID_LO_RESERVED0_ALIGN                           0
#define MISC_DEVICEID_LO_RESERVED0_BITS                            4
#define MISC_DEVICEID_LO_RESERVED0_SHIFT                           12

/* MISC :: deviceid_lo :: device_id_lo [11:00] */
#define Wr_MISC_deviceid_lo_device_id_lo(x) WriteRegBits16(MISC_DEVICEID_LO,0xfff,0,x)
#define Rd_MISC_deviceid_lo_device_id_lo(x) ReadRegBits16(MISC_DEVICEID_LO,0xfff,0)
#define MISC_DEVICEID_LO_DEVICE_ID_LO_MASK                         0x0fff
#define MISC_DEVICEID_LO_DEVICE_ID_LO_ALIGN                        0
#define MISC_DEVICEID_LO_DEVICE_ID_LO_BITS                         12
#define MISC_DEVICEID_LO_DEVICE_ID_LO_SHIFT                        0

/****************************************************************************
 * MISC :: deviceid_hi
 ***************************************************************************/
/* MISC :: deviceid_hi :: reserved0 [15:08] */
#define MISC_DEVICEID_HI_RESERVED0_MASK                            0xff00
#define MISC_DEVICEID_HI_RESERVED0_ALIGN                           0
#define MISC_DEVICEID_HI_RESERVED0_BITS                            8
#define MISC_DEVICEID_HI_RESERVED0_SHIFT                           8

/* MISC :: deviceid_hi :: device_id_hi [07:00] */
#define Wr_MISC_deviceid_hi_device_id_hi(x) WriteRegBits16(MISC_DEVICEID_HI,0xff,0,x)
#define Rd_MISC_deviceid_hi_device_id_hi(x) ReadRegBits16(MISC_DEVICEID_HI,0xff,0)
#define MISC_DEVICEID_HI_DEVICE_ID_HI_MASK                         0x00ff
#define MISC_DEVICEID_HI_DEVICE_ID_HI_ALIGN                        0
#define MISC_DEVICEID_HI_DEVICE_ID_HI_BITS                         8
#define MISC_DEVICEID_HI_DEVICE_ID_HI_SHIFT                        0

#endif /* #ifndef BCM89530_H__ */

/* End of File */

