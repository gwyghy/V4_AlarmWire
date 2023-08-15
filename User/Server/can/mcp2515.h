/********************************************************************************
* �ļ����ƣ�	mcp2515.h
* ��	�ߣ�	������   
* ��ǰ�汾��   	V1.0
* ������ڣ�    2014.10.28
* ��������: 	����MCP2515Ӳ����ͨ��ͷ�ļ�
* ��ʷ��Ϣ��   
*           	�汾��Ϣ     ���ʱ��      ԭ����        ע��
*
*       >>>>  �ڹ����е�λ��  <<<<
*          	  3-Ӧ�ò�
*             2-Э���
*          ��  1-Ӳ��������
*********************************************************************************
* Copyright (c) 2014,������������޹�˾ All rights reserved.
*********************************************************************************/
#ifndef _MCP2515_H_
#define _MCP2515_H_
/********************************************************************************
* .hͷ�ļ�
*********************************************************************************/

/********************************************************************************
* #define�궨��
*********************************************************************************/
/******MCP�Ĵ�������*******************************/
#define MCP_REG_RXF0SIDH			    0x00
#define MCP_REG_RXF0SIDL				0x01
#define MCP_REG_RXF0EID8				0x02
#define MCP_REG_RXF0EID0				0x03
#define MCP_REG_RXF1SIDH				0x04
#define MCP_REG_RXF1SIDL				0x05
#define MCP_REG_RXF1EID8				0x06
#define MCP_REG_RXF1EID0				0x07
#define MCP_REG_RXF2SIDH				0x08
#define MCP_REG_RXF2SIDL				0x09
#define MCP_REG_RXF2EID8				0x0A
#define MCP_REG_RXF2EID0				0x0B
#define MCP_REG_BFPCTRL					0x0C//����˵�������Բ���λ�޸�ָ������޸ġ�
#define MCP_REG_TXRTSCTRL				0x0D//����˵�������Բ���λ�޸�ָ������޸ġ�
#define MCP_REG_CANSTAT					0x0E
#define MCP_REG_CANCTRL					0x0F//����˵�������Բ���λ�޸�ָ������޸ġ�

#define MCP_REG_RXF3SIDH				0x10
#define MCP_REG_RXF3SIDL				0x11
#define MCP_REG_RXF3EID8				0x12
#define MCP_REG_RXF3EID0				0x13
#define MCP_REG_RXF4SIDH				0x14
#define MCP_REG_RXF4SIDL				0x15
#define MCP_REG_RXF4EID8				0x16
#define MCP_REG_RXF4EID0				0x17
#define MCP_REG_RXF5SIDH				0x18
#define MCP_REG_RXF5SIDL				0x19
#define MCP_REG_RXF5EID8				0x1A
#define MCP_REG_RXF5EID0				0x1B
#define MCP_REG_TEC						0x1C
#define MCP_REG_REC        				0x1D

#define MCP_REG_RXM0SIDH				0x20
#define MCP_REG_RXM0SIDL				0x21
#define MCP_REG_RXM0EID8				0x22
#define MCP_REG_RXM0EID0				0x23
#define MCP_REG_RXM1SIDH				0x24
#define MCP_REG_RXM1SIDL				0x25
#define MCP_REG_RXM1EID8				0x26
#define MCP_REG_RXM1EID0				0x27
#define MCP_REG_CNF3					  0x28//����˵�������Բ���λ�޸�ָ������޸ġ�
#define MCP_REG_CNF2					  0x29//����˵�������Բ���λ�޸�ָ������޸ġ�
#define MCP_REG_CNF1					  0x2A//����˵�������Բ���λ�޸�ָ������޸ġ�
#define MCP_REG_CANINTE					0x2B//����˵�������Բ���λ�޸�ָ������޸ġ�
#define MCP_REG_CANINTF					0x2C//����˵�������Բ���λ�޸�ָ������޸ġ�
#define MCP_REG_EFLG				   	0x2D//����˵�������Բ���λ�޸�ָ������޸ġ�

#define MCP_REG_TXB0CTRL				0x30//����˵�������Բ���λ�޸�ָ������޸ġ�
#define MCP_REG_TXB0SIDH				0x31
#define MCP_REG_TXB0SIDL				0x32
#define MCP_REG_TXB0EID8				0x33
#define MCP_REG_TXB0EID0				0x34
#define MCP_REG_TXB0DLC					0x35
#define MCP_REG_TXB0D0					0x36
#define MCP_REG_TXB0D1					0x37
#define MCP_REG_TXB0D2					0x38
#define MCP_REG_TXB0D3					0x39
#define MCP_REG_TXB0D4					0x3A
#define MCP_REG_TXB0D5					0x3B
#define MCP_REG_TXB0D6					0x3C
#define MCP_REG_TXB0D7					0x3D

#define MCP_REG_TXB1CTRL				0x40//����˵�������Բ���λ�޸�ָ������޸ġ�
#define MCP_REG_TXB1SIDH				0x41
#define MCP_REG_TXB1SIDL				0x42
#define MCP_REG_TXB1EID8				0x43
#define MCP_REG_TXB1EID0				0x44
#define MCP_REG_TXB1DLC					0x45
#define MCP_REG_TXB1D0					0x46
#define MCP_REG_TXB1D1					0x47
#define MCP_REG_TXB1D2					0x48
#define MCP_REG_TXB1D3					0x49
#define MCP_REG_TXB1D4					0x4A
#define MCP_REG_TXB1D5					0x4B
#define MCP_REG_TXB1D6					0x4C
#define MCP_REG_TXB1D7					0x4D

#define MCP_REG_TXB2CTRL				0x50//����˵�������Բ���λ�޸�ָ������޸ġ�
#define MCP_REG_TXB2SIDH				0x51
#define MCP_REG_TXB2SIDL				0x52
#define MCP_REG_TXB2EID8				0x53
#define MCP_REG_TXB2EID0				0x54
#define MCP_REG_TXB2DLC					0x55
#define MCP_REG_TXB2D0					0x56
#define MCP_REG_TXB2D1					0x57
#define MCP_REG_TXB2D2					0x58
#define MCP_REG_TXB2D3					0x59
#define MCP_REG_TXB2D4					0x5A
#define MCP_REG_TXB2D5					0x5B
#define MCP_REG_TXB2D6					0x5C
#define MCP_REG_TXB2D7					0x5D

#define MCP_REG_RXB0CTRL				0x60//����˵�������Բ���λ�޸�ָ������޸ġ�
#define MCP_REG_RXB0SIDH				0x61
#define MCP_REG_RXB0SIDL				0x62
#define MCP_REG_RXB0EID8				0x63
#define MCP_REG_RXB0EID0				0x64
#define MCP_REG_RXB0DLC					0x65
#define MCP_REG_RXB0D0					0x66
#define MCP_REG_RXB0D1					0x67
#define MCP_REG_RXB0D2					0x68
#define MCP_REG_RXB0D3					0x69
#define MCP_REG_RXB0D4					0x6A
#define MCP_REG_RXB0D5					0x6B
#define MCP_REG_RXB0D6					0x6C
#define MCP_REG_RXB0D7					0x6D

#define MCP_REG_RXB1CTRL				0x70//����˵�������Բ���λ�޸�ָ������޸ġ�
#define MCP_REG_RXB1SIDH				0x71
#define MCP_REG_RXB1SIDL				0x72
#define MCP_REG_RXB1EID8				0x73
#define MCP_REG_RXB1EID0				0x74
#define MCP_REG_RXB1DLC					0x75
#define MCP_REG_RXB1D0					0x76
#define MCP_REG_RXB1D1					0x77
#define MCP_REG_RXB1D2					0x78
#define MCP_REG_RXB1D3					0x79
#define MCP_REG_RXB1D4					0x7A
#define MCP_REG_RXB1D5					0x7B
#define MCP_REG_RXB1D6					0x7C
#define MCP_REG_RXB1D7					0x7D

/***�Ĵ�����λ��Ϣ����************************/
/*����RX0BF���ſ��ƺ�״̬�Ĵ���BFPCTRLλ��Ϣ*/
typedef enum
{
	MCP_REG_BFPCTRL_B1BFS	=	5,
	MCP_REG_BFPCTRL_B0BFS	=	4,
	MCP_REG_BFPCTRL_B1BFE	=	3,
	MCP_REG_BFPCTRL_B0BFE	=	2,
	MCP_REG_BFPCTRL_B1BFM	=	1,
	MCP_REG_BFPCTRL_B0BFM	=	0
}MCP_REG_BFPCTRL_BIT;

/*����TXnRST���ſ��ƺ�״̬�Ĵ���TXRTSCTRLλ��Ϣ*/
typedef enum
{
	MCP_REG_B2RTS			=	5,
	MCP_REG_B1RTS			=	4,
	MCP_REG_B0RTS			=	3,
	MCP_REG_B2RTSM			=	2,
	MCP_REG_B1RTSM			=	1,
	MCP_REG_B0RTSM			=	0,
}MCP_REG_TXRTSCTR_BIT;

/*����״̬�Ĵ���CANSTATλ��Ϣ*/
typedef enum
{

	MCP_REG_CANSTAT_OPMOD2	=	7,
	MCP_REG_CANSTAT_OPMOD1	=	6,
	MCP_REG_CANSTAT_OPMOD0	=	5,
	MCP_REG_CANSTAT_ICOD2	=	3,
	MCP_REG_CANSTAT_ICOD1	=	2,
	MCP_REG_CANSTAT_ICOD0	=	1,
}MCP_REG_CANSTAT_BIT;

/*������ƼĴ���CANCTRLλ��Ϣ*/
typedef enum
{
	MCP_REG_CANCTRL_REQOP2	=	7,
	MCP_REG_CANCTRL_REQOP1	=	6,
	MCP_REG_CANCTRL_REQOP0	=	5,
	MCP_REG_CANCTRL_ABAT	=	4,
	MCP_REG_CANCTRL_CLKEN	=	2,
	MCP_REG_CANCTRL_CLKPRE1	=	1,
	MCP_REG_CANCTRL_CLKPRE0	=	0
}MCP_REG_CANCTRL_BIT;

/*�������üĴ���CNF3λ��Ϣ*/
typedef enum
{
	MCP_REG_CNF3_SOF		=	7,
	MCP_REG_CNF3_WAKFIL		=	6,
	MCP_REG_CNF3_PHSEG22	=	2,
	MCP_REG_CNF3_PHSEG21	=	1,
	MCP_REG_CNF3_PHSEG20	=	0
}MCP_REG_CANF3_BIT;

/*�������üĴ���CNF2λ��Ϣ*/
typedef enum
{
	MCP_REG_CANF2_BTLMODE	=	7,
	MCP_REG_CANF2_SAM		=	6,
	MCP_REG_CANF2_PHSEG12	=	5,
	MCP_REG_CANF2_PHSEG11	=	4,
	MCP_REG_CANF2_PHSEG10	=	3,
	MCP_REG_CANF2_PHSEG2	=	2,
	MCP_REG_CANF2_PHSEG1	=	1,
	MCP_REG_CANF2_PHSEG0	=	0
}MCP_REG_CANF2_BIT;

/*�������üĴ���CNF1λ��Ϣ*/
typedef enum
{
	MCP_REG_CANF1_SJW1		=	7,
	MCP_REG_CANF1_SJW0		=	6,
	MCP_REG_CANF1_BRP5		=	5,
	MCP_REG_CANF1_BRP4		=	4,
	MCP_REG_CANF1_BRP3		=	3,
	MCP_REG_CANF1_BRP2		=	2,
	MCP_REG_CANF1_BRP1		=	1,
	MCP_REG_CANF1_BRP0		=	0
}MCP_REG_CANF1_BIT;

/*�����ж�ʹ�ܼĴ���CANINTEλ��Ϣ*/
typedef enum
{
	MCP_REG_CANINTE_MERRE	=	7,//���Ĵ����ж�ʹ��λ
	MCP_REG_CANINTE_WAKIE	=	6,//CAN���߻����ж�ʹ��λ
	MCP_REG_CANINTE_ERRIE	=	5,//�����ж�ʹ��λ
	MCP_REG_CANINTE_TX2IE	=	4,//���ͻ�����2���ж�ʹ��
	MCP_REG_CANINTE_TX1IE	=	3,//���ͻ�����1���ж�ʹ��
	MCP_REG_CANINTE_TX0IE	=	2,//���ͻ�����0���ж�ʹ��
	MCP_REG_CANINTE_RX1IE	=	1,//���ջ�����1���ж�ʹ��
	MCP_REG_CANINTE_RX0IE	=	0//���ջ�����0���ж�ʹ��
}MCP_REG_CANINTE_BIT;

/*�����жϱ�־�Ĵ���CANINTFλ��Ϣ*/
typedef enum
{
	MCP_REG_CANINTF_MERRF	=	7,//���Ĵ����־λ
	MCP_REG_CANINTF_WAKIF	=	6,//CAN���߻����жϱ�־λ
	MCP_REG_CANINTF_ERRIF	=	5,//�����жϱ�־λ
	MCP_REG_CANINTF_TX2IF	=	4,//���ͻ�����2���жϱ�־λ
	MCP_REG_CANINTF_TX1IF	=	3,//���ͻ�����1���жϱ�־λ
	MCP_REG_CANINTF_TX0IF	=	2,//���ͻ�����0���жϱ�־λ
	MCP_REG_CANINTF_RX1IF	=	1,//���ջ�����1���жϱ�־λ
	MCP_REG_CANINTF_RX0IF	=	0//���ջ�����0���жϱ�־λ
}MCP_REG_CANINTF_BIT;

/*��������־�Ĵ���EFLGλ��Ϣ*/
typedef enum
{
	MCP_REG_EFLG_RX1OVR	=	0x80,
	MCP_REG_EFLG_RX0OVR	=	0x40,
	MCP_REG_EFLG_TXB0	=	0x20,
	MCP_REG_EFLG_TXEP	=	0x10,
	MCP_REG_EFLG_RXEP	=	0x08,
	MCP_REG_EFLG_TXWAR	=	0x04,
	MCP_REG_EFLG_RXWAR	=	0x02,
	MCP_REG_EFLG_EWARN	=	0x01
}MCP_REG_EFLG_BIT;

/*** ���巢�ͻ�����n���ƼĴ���TXBnCTRL  ( n = 0, 1, 2 )λ��Ϣ*/
typedef enum
{
	MCP_REG_TXB_ABTF_M      =   0x06,
	MCP_REG_TXB_MLOA_M      =   0x05,
	MCP_REG_TXB_TXERR_M     =   0x04,
	MCP_REG_TXB_TXREQ_M     =   0x03,
	MCP_REG_TXB_TXP1_M      =   0x01,
	MCP_REG_TXB_TXP0_M      =   0x00	
}MCP_REG_TXBnCTRL_BIT;/**TXB0CTRL��TXB1CTRL��TXB2CTRL**/

/*����Ĵ���RXBnCTRL(n=0,1)λ��Ϣ*/
typedef enum
{
	MCP_REG_RXBnCTRL_RXM1	=	6,
	MCP_REG_RXBnCTRL_RXM0	=	5,
	MCP_REG_RXBnCTRL_RXRTR	=	3,
	MCP_REG_RXBnCTRL_BUKT	=	2,
	MCP_REG_RXBnCTRL_BUKT1	=	1,
	MCP_REG_RXBnCTRL_FILHIT0=	0
}MCP_REG_RXBnCTRL_BIT;/**RXB0CTRL��RXB1CTRL**/

/*****����SPI����������*****************************/
typedef enum
{
	MCP_CMD_RESET			=  	0xC0,// ��λ
	MCP_CMD_READ			=	0x03,// ��ָ����ַ��ʼ�ļĴ�����ȡ����
	MCP_CMD_RD_RX_BUFF		= 	0x90,// ��RX������
	MCP_CMD_WRITE			=	0x02,// ������дָ����ַ��ʼ�ļĴ���
	MCP_CMD_LOAD_TX			=	0x40,// װ��TX������
	MCP_CMD_RTS				=	0x80,// �����ͱ���(�����Ϊ0x81\82\84������������)
	MCP_CMD_RD_STATUS		=	0xA0,// ��״̬
	MCP_CMD_RX_STATUS		=	0xB0,// RX״̬
	MCP_CMD_BIT_MODIFY		=	0x05,// λ�޸�
	/**0x80����ľ���ʵ��****/
	MCP_CMD_RTS_TXB0    	=	0x81,// �����ͷ��ͻ�����0������
	MCP_CMD_RTS_TXB1    	=	0x82, // �����ͷ��ͻ�����1������
	MCP_CMD_RTS_TXB2   	 	=	0x84// �����ͷ��ͻ�����2������
}MCP_CMD_TYPE;

/**�����ȡ������ʱʹ�õĶ���****/
typedef enum
{
	MCP_READBUF_BUF0 = 0x00,//��ȡ������0
	MCP_READBUF_BUF1 = 0x04//��ȡ������1
	/**�˴�δ�������ȡ���ݵ�ָ���ֹ�����****/	
}MCP_READBUF_REG_TYPE;

enum 
{
	MCP_READBUF_ANY_STATUS = 0xFF,//��ȡ������ʱʹ�õ���������
};

/**�����ȡ������ʱʹ�õĶ���****/
typedef enum
{
	MCP_READBUF_LENTH_1 = 1,//��ȡ1��������������
	MCP_READBUF_LENTH_2 = 2,//��ȡ2��������������
	MCP_READBUF_LENTH_3 = 3,//��ȡ3��������������
	MCP_READBUF_LENTH_4 = 4,//��ȡ4��������������
	MCP_READBUF_LENTH_5 = 5,//��ȡ5��������������
	MCP_READBUF_LENTH_6 = 6,//��ȡ6��������������
	MCP_READBUF_LENTH_7 = 7,//��ȡ7��������������
	/**�ϳ���***/
	MCP_READBUF_LENTH_8 = 8,//��ȡ8��������������

	MCP_READBUF_LENTH_9 = 9,//��ȡ9��������������
	MCP_READBUF_LENTH_10 = 10,//��ȡ10��������������
	MCP_READBUF_LENTH_11 = 11,//��ȡ11��������������
	MCP_READBUF_LENTH_12 = 12,//��ȡ12��������������
	/**�ϳ���***/
	MCP_READBUF_LENTH_13 = 13,//��ȡ13������������ʾ��+����+����
}MCP_READBUF_LENGTH_TYPE;

/**SPI�豸״̬���Ͷ���****/
typedef enum
{
	SPI_DEV_STATUS_INIT,		//�豸���ڳ�ʼ��״̬
	SPI_DEV_STATUS_INIT_FAIL,	//�豸���ڳ�ʼ��ʧ��״̬
	SPI_DEV_STATUS_DEINIT_FAIL,	//�豸���ڸ�λʧ��״̬	
	SPI_DEV_STATUS_NORMAL,		//�豸��������״̬	
	SPI_DEV_STATUS_DEINT_NORMAL,//�豸���ڸ�λ����״̬	
	SPI_DEV_STATUS_MAX,			//�豸״̬���ֵ?
}SPI_DEV_STATUS_TYPE;

/**SPI�������Ͷ���****/
typedef enum
{
	SPI_PROC_WRITE,				//д���ݷ�ʽ
	SPI_PROC_READ				//�����ݷ�ʽ
}SPI_PROC_TYPE;

/**SPI����֡���Ͷ���****/
#define SPI_ONEFRAME_MAX_SIZE  20//SPIһ�����ݵ���󳤶�
__packed typedef struct
{
	u32 u32ProcType ;
	
	u8 u8TxBuf[SPI_ONEFRAME_MAX_SIZE];
	u8 u8TxLength;
	u8 u8TxRead;
	
	u8 u8RxBuf[SPI_ONEFRAME_MAX_SIZE];
	u8 u8RxWrite;
	u8 u8RxLength;	
}SPI_DATA_TYPE;


/**���ù��������ֶ���****************************/   
/**����ģʽ����***/
typedef enum
{
	MCP_CAN_MODE_NORMAL 	=  0x00,// ����ģʽ
	MCP_CAN_MODE_SLEEP   	=  0x20,//����ģʽ	  
	MCP_CAN_MODE_LOOP   	=  0x40,// �ػ�ģʽ
	MCP_CAN_MODE_LISTEN  	=  0x60,//����ģʽ 
	MCP_CAN_MODE_SETUPE	 	=  0x80,// ����ģʽ
	MCP_CAN_MODE_MAX 		=  0x81//���ֵ
}MCP_CANCTRL_MODE_TYPE;

/**CANCTRL �Զ��ش�����***/
typedef enum
{
	MCP_CANCTRL_AUTO_RESEND = 0x00,//�Զ��ش�
	MCP_CANCTRL_OSM = 0x08//������ģʽ	
}MCP_CANCTRL_OSM_TYPE;

/**CANCTRL CLKOUT����ʹ������***/
typedef enum
{
	MCP_CANCTRL_CLKOUT_DISABLE = 0x00,//CLKOUT���Ų�ʹ��
	MCP_CANCTRL_CLKOUT_ENABLE = 0x04//CLKOUT����ʹ��	
}MCP_CANCTRL_CLKOUT_TYPE;

/**��ֹ���ͱ�������****/
enum
{
	MCP_CANCTRL_ABAT_ENABLE = 0x80,//��ֹ����
	MCP_CANCTRL_ABAT_DISABLE = 0x00//����ֹ����
};

/**CANCTRL CLKOUT����ʹ������***/
typedef enum
{
	MCP_CANCTRL_CLKOUTPER_1 = 0x00,//CLKOUT����ʹ��
	MCP_CANCTRL_CLKOUTPER_2 = 0x01,//CLKOUT���Ų�ʹ��
	MCP_CANCTRL_CLKOUTPER_4 = 0x02,//CLKOUT���Ų�ʹ��
	MCP_CANCTRL_CLKOUTPER_8 = 0x03,//CLKOUT���Ų�ʹ��	
}MCP_CANCTRL_CLKOUTPER_TYPE;


/**���ջ�����ƼĴ����У��˲���ʽ��ö��***/
typedef enum
{
	MCP_RXBNCTRL_FILTER_ALL 	= 0x00,//���շ����˲������ı�׼֡�Լ���չ֡	
	MCP_RXBNCTRL_FILTER_STAND 	= 0x20,//���շ����˲������ı�׼֡
	MCP_RXBNCTRL_FILTER_EXT 	= 0x40,//���շ����˲���������չ֡
	MCP_RXBNCTRL_FILTER_CLOSE 	= 0x60,//�ر��˲����ܣ��������й���
}MCP_RXBUF_FILTERMODE_TYPE;

/**���ջ�����ƼĴ����У����淽ʽ��ö��***/
typedef enum
{
	MCP_RXBNCTRL_BUKE_ENABLE 	= 0x04,//���淽ʽʹ�ܣ�buf0�����Զ����浽buf1
	MCP_RXBNCTRL_BUKE_DISABLE 	= 0x00,//���淽ʽ��ֹ		
}MCP_RXBUF_BUKTMODE_TYPE;

/**�����ж��������RXNBF������ʹ�õ�ö��***/
typedef enum
{
	MCP_RXNBF_RX1BF_ENABLE 		= 0x08,//RX0BF���Ź���ʹ��
	MCP_RXNBF_RX0BF_ENABLE 		= 0x04,//RX1BF���Ź���ʹ��
	MCP_RXNBF_RX01BF_DISABLE 	= 0x00,//RX0BF��RX1BF���Ź��ܽ�ֹ
	
	MCP_RXNBF_RX1BF_INT_EN 		= 0x02,//RX1BF������RXBF1������ʱ�������ж�
	MCP_RXNBF_RX0BF_INT_EN 		= 0x01,//RX0BF������RXBF0������ʱ�������ж�
}MCP_RXNBF_RXPINTMODE_TYPE;

/**�����ж��������TXRSTCTRL ������ʹ�õ�ö��***/
typedef enum
{
	MCP_TXRSTCTRL_TX2RST_ENABLE = 0x04,//TX2RST�����������ʹ��λ
	MCP_TXRSTCTRL_TX1RST_ENABLE = 0x02,//TX1RST�����������ʹ��λ
	MCP_TXRSTCTRL_TX0RST_ENABLE = 0x01,//TX0RST�����������ʹ��λ
	
	MCP_TXRSTCTRL_TX012RST_DISABLE = 0x00//TX0RST��TX0RST��TX2RST���Ź��ܽ�ֹ
}MCP_TXRSTCTRL_TXPINTMODE_TYPE;

/**���Ϳ��ƼĴ����У����ȼ�***/
enum
{
	MCP_TXBNCTRL_PRIOTY_LOWEST = 0x00,//��͵ķ������ȼ�
	MCP_TXBNCTRL_PRIOTY_LOW = 0x01,//�͵ķ������ȼ�
	MCP_TXBNCTRL_PRIOTY_HIGH = 0x02,//�ߵķ������ȼ�
	MCP_TXBNCTRL_PRIOTY_HIGHEST = 0x03,//��ߵķ������ȼ�
};

/**���Ϳ��ƣ�������***/
enum
{
	MCP_TXBNCTRL_CLEAR_TXBUF = 0x00,
	MCP_TXBNCTRL_TXREQ = 0x08
};

/**���ͣ�װ��TX������ָ���У�����������***/
typedef enum
{
	MCP_TXLOAD_TXB0SIDH = 0x00,//��TXB0SIDH��ʼװ��(ȫ��֡��Ϣ)
	MCP_TXLOAD_TXB1SIDH = 0x02,//��TXB1SIDH��ʼװ��(ȫ��֡��Ϣ)	
	MCP_TXLOAD_TXB2SIDH = 0x04//��TXB2SIDH��ʼװ��(ȫ��֡��Ϣ)	
	/**�˴�δ�����װ�����ݵ�ָ���ֹ�����****/
}MCP_TXLOAD_TYPE;

/**���ͣ�������ָ���У�����������***/
typedef enum
{
	MCP_TXRTS_TXB0 = 0x01,//���ͻ�����0
	MCP_TXRTS_TXB1 = 0x02,//���ͻ�����1
	MCP_TXRTS_TXB2 = 0x04//���ͻ�����2	
}MCP_TXRTS_TYPE;

/**�ж�ʹ�ܼĴ���CANINTE������ʹ�õ�ö��****/
typedef enum
{
	MCP_CANINTE_INT_DISABLE 	= 0x00,//�ر����е��ж�ʹ��
	
	MCP_CANINTE_RX0IE_ENABLE 	= 0x01,//���ջ�����0�ж�ʹ��
	MCP_CANINTE_RX1IE_ENABLE 	= 0x02,//���ջ�����1�ж�ʹ��
	MCP_CANINTE_TX0IE_ENABLE 	= 0x04,//���ͻ�����0���ж�ʹ��
	MCP_CANINTE_TX1IE_ENABLE 	= 0x08,//���ͻ�����1���ж�ʹ��
	
	MCP_CANINTE_TX2IE_ENABLE 	= 0x10,//���ͻ�����2���ж�ʹ��
	MCP_CANINTE_ERRIE_ENABLE 	= 0x20,//�����ж�ʹ��
	MCP_CANINTE_WAKEIE_ENABLE 	= 0x40,//�����ж�ʹ��
	MCP_CANINTE_MERRE_ENABLE 	= 0x80,//���Ĵ����ж�ʹ��(���ջ��ͱ��Ĵ���)
}MCP_CANINTE_ENABLE_TYPE;

/**���ͻ�����0���жϱ�־���Ͷ���****/
typedef enum
{
	MCP_TXINTF_DISABLE = 0x00,//�жϱ�־����Ϊ1
	MCP_TXINTF_ENABLE = 0x01//�жϱ�־����Ϊ0
}MCP_TXINTF_FLAG_TYPE;

/**�ж�״̬�Ĵ���CANINTF������ʹ�õ�ö��****/
typedef enum
{
	MCP_CANINTF_RX0IF		  	= 0x01,//���ջ�����0�жϱ�־
	MCP_CANINTF_RX1IF 			= 0x02,//���ջ�����1�жϱ�־
	MCP_CANINTF_TX0IF 			= 0x04,//���ͻ�����0���жϱ�־
	MCP_CANINTF_TX1IF 			= 0x08,//���ͻ�����1���жϱ�־
	 
	MCP_CANINTF_TX2IF		  	= 0x10,//���ͻ�����2���жϱ�־
	MCP_CANINTF_ERRIF 			= 0x20,//�����жϱ�־
	MCP_CANINTF_WAKEIF	 		= 0x40,//�����жϱ�־
	MCP_CANINTF_MERRF 			= 0x80,//���Ĵ����жϱ�־
}MCP_CANINTF_FLAG_TYPE;;

/**��ʹ��SPI�жϷ�ʽʱ������İ�����***/
typedef enum
{
	MCP_EVENT_INFORMATION_RX0_RECVD ,//������0����
	MCP_EVENT_INFORMATION_RX1_RECVD ,//������1����
	MCP_EVENT_INFORMATION_TX0_EMPTY ,//���ͻ�����0��
	MCP_EVENT_INFORMATION_TX1_EMPTY ,//���ͻ�����1��		
	MCP_EVENT_INFORMATION_TX2_EMPTY ,//���ͻ�����2��	
	MCP_EVENT_INFORMATION_MESSAGE_ERR,//���Ĵ���	
	MCP_EVENT_INFORMATION_CAN_RXERR,		//CAN���ߴ��󣬽��մ���
	MCP_EVENT_INFORMATION_CAN_TXERR,		//CAN���ߴ��󣬷��ʹ���
	MCP_EVENT_INFORMATION_CAN_CLOSED,		//CAN���ߴ������߹ر�	
}MCP_EVENT_INFORMATION_TYPE;



/**Ԥ����ָ�����Ŀ���Ƿ�ʹ��RXnBF���Ž����жϽ���****/
#define MCP2515_RXNBF_PIN_ENABLE		0x01
#define MCP2515_RXNBF_PIN_DISABLE		0x00


#endif
