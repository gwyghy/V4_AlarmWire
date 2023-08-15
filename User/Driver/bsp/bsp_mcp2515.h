/*
*********************************************************************************************************
*	                                            DoubleWires
*                           Commuication Controller For The Mining Industry
*
*                                  (c) Copyright 1994-2013  HNDZ 
*                       All rights reserved.  Protected by international copyright laws.
*
*
*    File    : bsp_mcp2515.h
*    Module  : user
*    Version : V1.0
*    History :
*   -----------------
*              Version  Date           By            Note
*              v1.0     2013-12-17     
*
*********************************************************************************************************
*/

#ifndef _BSP_MCP2515_H_
#define _BSP_MCP2515_H_
//-----------------Include files-------------------------//
#include "stm32f4xx.h"
#include  <bsp.h>
/*---------------------------------------------------------------  */
// Define MCP2515 register addresses

#define MCP_RXF0SIDH	0x00
#define MCP_RXF0SIDL	0x01
#define MCP_RXF0EID8	0x02
#define MCP_RXF0EID0	0x03
#define MCP_RXF1SIDH	0x04
#define MCP_RXF1SIDL	0x05
#define MCP_RXF1EID8	0x06
#define MCP_RXF1EID0	0x07
#define MCP_RXF2SIDH	0x08
#define MCP_RXF2SIDL	0x09
#define MCP_RXF2EID8	0x0A
#define MCP_RXF2EID0	0x0B
#define MCP_TXRTSCTRL 0x0D
#define MCP_CANSTAT		0x0E
#define MCP_CANCTRL		0x0F
#define MCP_RXF3SIDH	0x10
#define MCP_RXF3SIDL	0x11
#define MCP_RXF3EID8	0x12
#define MCP_RXF3EID0	0x13
#define MCP_RXF4SIDH	0x14
#define MCP_RXF4SIDL	0x15
#define MCP_RXF4EID8	0x16
#define MCP_RXF4EID0	0x17
#define MCP_RXF5SIDH	0x18
#define MCP_RXF5SIDL	0x19
#define MCP_RXF5EID8	0x1A
#define MCP_RXF5EID0	0x1B
#define MCP_TEC			0x1C
#define MCP_REC			0x1D
#define MCP_RXM0SIDH	0x20
#define MCP_RXM0SIDL	0x21
#define MCP_RXM0EID8	0x22
#define MCP_RXM0EID0	0x23
#define MCP_RXM1SIDH	0x24
#define MCP_RXM1SIDL	0x25
#define MCP_RXM1EID8	0x26
#define MCP_RXM1EID0	0x27
#define MCP_CNF3		  0x28
#define MCP_CNF2		  0x29
#define MCP_CNF1		  0x2A
#define MCP_CANINTE		0x2B
#define MCP_CANINTF		0x2C
#define MCP_EFLG		  0x2D
#define MCP_TXB0CTRL	0x30
#define MCP_TXB1CTRL	0x40
#define MCP_TXB2CTRL	0x50

#define MCP_TXB0SIDH	0x31
#define MCP_TXB0SIDL	0x32
#define MCP_TXB0DLC	  0x35
#define MCP_TXB0D0	  0x36
#define MCP_TXB0D1	  0x37
#define MCP_TXB0D2	  0x38
#define MCP_TXB0D3	  0x39
#define MCP_TXB0D4	  0x3A
#define MCP_TXB0D5	  0x3B
#define MCP_TXB0D6	  0x3C
#define MCP_TXB0D7	  0x3D

#define MCP_TXB2SIDH	0x41
#define MCP_TXB3SIDH	0x51

#define MCP_RXB0CTRL	0x60
#define MCP_RXB0SIDH	0x61
#define MCP_RXB1CTRL	0x70
#define MCP_RXB1SIDH	0x71


// Define SPI Instruction Set

#define MCP_WRITE		0x02

#define MCP_READ		0x03

#define MCP_BITMOD		0x05

#define MCP_LOAD_TX0	0x40
#define MCP_LOAD_TX1	0x42
#define MCP_LOAD_TX2	0x44

#define MCP_RTS_TX0		0x81
#define MCP_RTS_TX1		0x82
#define MCP_RTS_TX2		0x84
#define MCP_RTS_ALL		0x87

#define MCP_READ_RX0	0x90
#define MCP_READ_RX1	0x94

#define MCP_READ_STATUS	0xA0

#define MCP_RX_STATUS	0xB0

#define MCP_RESET		0xC0

// CANCTRL Register Values
#define MODE_NORMAL     0x00
#define MODE_SLEEP      0x20
#define MODE_LOOPBACK   0x40
#define MODE_LISTENONLY 0x60
#define MODE_CONFIG     0x80
#define MODE_POWERUP	0xE0
#define MODE_MASK		0xE0
#define ABORT_TX        0x10
#define MODE_ONESHOT	0x08
#define CLKOUT_ENABLE	0x04
#define CLKOUT_DISABLE	0x00
#define CLKOUT_PS1		0x00
#define CLKOUT_PS2		0x01
#define CLKOUT_PS4		0x02
#define CLKOUT_PS8		0x03

// CNF1 Register Values
#define SJW1            0x00
#define SJW2            0x40
#define SJW3            0x80
#define SJW4            0xC0

// CNF2 Register Values
#define BTLMODE			0x80
#define SAMPLE_1X       0x00
#define SAMPLE_3X       0x40

// CNF3 Register Values
#define SOF_ENABLE		0x80
#define SOF_DISABLE		0x00
#define WAKFIL_ENABLE	0x40
#define WAKFIL_DISABLE	0x00


// CANINTF Register Bits
#define MCP_RX0IF		0x01
#define MCP_RX1IF		0x02
#define MCP_TX0IF		0x04
#define MCP_TX1IF		0x08
#define MCP_TX2IF		0x10
#define MCP_ERRIF		0x20
#define MCP_WAKIF		0x40
#define MCP_MERRF		0x80


#define MCP2515_OK         (0)
#define MCP2515_FAIL       (1)
#define CANDEBUG   1

#define CANUSELOOP 0

#define CANSENDTIMEOUT (200) 

// #define SUCCESS         (0)
// #define FAIL           (!0)

#define CAN_MAX_CHAR_IN_MESSAGE (8)

#define CLK_OFF   GPIO_ResetBits(GPIOA,GPIO_Pin_5)
#define CLK_ON    GPIO_SetBits(GPIOA,GPIO_Pin_5)
#define MISO      GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_6)
#define MOSI_H    GPIO_SetBits(GPIOB,GPIO_Pin_5)
#define MOSI_L    GPIO_ResetBits(GPIOB,GPIO_Pin_5)
#define CS_OFF    GPIO_ResetBits(GPIOF,GPIO_Pin_4)                                   
#define CS_ON	    GPIO_SetBits(GPIOF,GPIO_Pin_4)
#define RST_OFF   GPIO_ResetBits(GPIOF,GPIO_Pin_3)                                   
#define RST_ON	  GPIO_SetBits(GPIOF,GPIO_Pin_3)


typedef struct {
	// identifier CAN_xxxID
	unsigned char  extended_identifier; 
	// either extended (the 18 LSB) or standard (the 11 LSB)
	unsigned int identifier; 
	// data length:	 29bit;
	unsigned char  dlc;
	unsigned char  dta[CAN_MAX_CHAR_IN_MESSAGE];
	
	// used for receive only:
	// Received Remote Transfer Bit 
	//  (0=no... 1=remote transfer request received)
	unsigned char  rtr;  
	// Acceptence Filter that enabled the reception
	unsigned char  filhit;
} CAN_MESSAGE;

/*-------------------------------------------------------------------------------*/

extern  unsigned char mcp2515_reset(void);
extern  unsigned char mcp2515_config_rate(void);
extern  unsigned char mcp2515_clock(void);
extern  unsigned int mcp2515_read_register(const unsigned char address);
extern  unsigned char mcp2515_read_registers(const unsigned char address, 
	        unsigned char values[], const unsigned char n);
extern  unsigned char mcp2515_read_status(void);
extern  unsigned char mcp2515_set_register(const unsigned char address, const unsigned char value);
extern  unsigned char mcp2515_modify_register(const unsigned char address, 
	        const unsigned char mask, const unsigned char data);
extern  unsigned char mcp2515_set_registers(const unsigned char address, 
        	const unsigned char values[], const unsigned char n);
extern  unsigned char mcp2515_read_can_id( const unsigned char mcp_addr, 
    	    unsigned char* ext, unsigned int* can_id );
extern  unsigned char mcp2515_read_can_msg( const unsigned char buffer_sidh_addr,
         	CAN_MESSAGE* msg);
extern  unsigned char mcp2515_write_can_id( const unsigned char mcp_addr, 
        	const unsigned char ext, const unsigned int can_id ) ;
extern  unsigned char mcp2515_write_can_msg( const unsigned char buffer_sidh_addr, 
	        const CanTxMsg* msg);
extern  unsigned char mcp2515_start_transmit(void);
extern  unsigned char mcp2515_init_can_buffers(void);
extern  unsigned char mcp2515_rts(void);
extern  unsigned char mcp2515_get_free_txbuf(unsigned char *txbuf_n);
extern  unsigned char mcp2515_filter_intialize(void);
uint8_t mcp2515_setCANCTRL_Mode(uint8_t newmode);
void SPI1_Init(void);
void init_message_struct(CanTxMsg* msg);
unsigned char can_send_message(const CanTxMsg* msg);
unsigned char can_read_message(CAN_MESSAGE *msg);
unsigned char can_test_transmit(void);
void can_test_receive(void);

#endif
