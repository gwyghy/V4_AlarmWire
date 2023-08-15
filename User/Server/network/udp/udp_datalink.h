#ifndef __UDP_DATALINK_H
#define __UDP_DATALINK_H

#include "udp_server.h"
#include "chain.h"
#include "memory_pool.h"


#define UDP_DEVICE_ALL         0
#define UDP_DEVICE_CONTROLLER  1
#define UDP_DEVICE_SERVER      2
#define UDP_DEVICE_DATATRANS   3
#define UDP_DEVICE_APP		   4
//append device
#define UDP_DEVICE_CONTROLLER_ChaoQian   5
#define UDP_DEVICE_CONTROLLER_ZiYiJinWei 6

#define UDP_DEVICE_CONTROLLER_PingBanLieChe  7 //ghy ����ƽ���г��豸

#define UDP_DEVICE_MAX         8 
#define UDP_DEVICE_TYPE  UDP_DEVICE_CONTROLLER

#define UDP_FRAME_HEADER_LEN   8
#define UDP_FRAME_CRC_OFFSET   4
#define UDP_FRAME_INFO_LEN     (sizeof(struct ip_addr)+2)
#define UDP_FRAME_DATA_MAX     (10*(UDP_FRAME_LENGTH_MAX-UDP_FRAME_HEADER_LEN))
	
#define UDP_LINK_BROADCAST_ADDR      255


#define IP_ADDR(ipaddr, a,b,c,d) \
        (ipaddr)->addr = ((u32)((d) & 0xff) << 24) | \
                         ((u32)((c) & 0xff) << 16) | \
                         ((u32)((b) & 0xff) << 8)  | \
                          (u32)((a) & 0xff)

enum
{
	UDPProcess_ID_SupportCom=0,
	UDPProcess_ID_PHONE,
	UDPProcess_ID_Update,
	UDPProcess_ID_IPScaner,
	UDPProcess_ID_MAX
};

typedef struct
{
	u16	SN;		//���к�
	u16 crc;    //CRC
	union{
		struct{						
			u16		ACK   :1;				//Ӧ���ʶ
			u16		FIN   :1;				//��ɱ�ʶ
			u16		RST   :1;				//��λ
			u16     VLAN  :2;               //VLAN��־
			u16		Null  :3;				//Ԥ��
			u16     Type  :5;               //�豸����
			u16     FuncPort   :3;          //���ܶ˿�
		} FUNC;
		u16 FuncID;
	} u16FuncID;
	u8  TotalNum;  //�ܰ���
	u8  CurNum;    //��ǰ����
}UDPHeadType;

typedef	struct
{
	struct  ip_addr   addr;//IP��ַ
	u16     length;        //���ݳ���
	UDPHeadType  header;   //�Զ����ͷ
	u8      *pdata;        //����
}UDPFrameType;

typedef u8 (*udpLinkCBFunc)(UDPFrameType *pframe);

typedef struct
{
	u16		procID;        //ӳ��ID
	u16		overtime;      //��ʱʱ��
	u16  	repeatNum;	   //�ط�����
	udpLinkCBFunc	recvCBFun; //���ջص�
	udpLinkCBFunc	overCBFun; //��ʱ�ص�
}UDPProcType;

typedef u8 (*udpProcCBFunc)(UDPProcType *proc, UDPFrameType *pframe);

typedef struct
{
	ChainList 	 list;
	FreeDataFunc freeFunc;
	CompareFunc	 compareFunc;
}UdpLinkListType;

typedef struct
{
	u8 ipAddr0;
	u8 ipAddr1;
	u8 ipAddr2;
	u8 ipAddr3;
}UdpLinkParamType;

typedef struct
{
	u16		overtime;      //��ʱʱ��
	u16		currenttime;   //��ǰʱ��
	u16  	repeatNum;	   //�ط�����
}UdpLinkOverType;

typedef struct
{
	UdpLinkParamType	linkParam;
	u16                 linkSN;
	
	UdpLinkListType 	RecvList;  //UDPFrameType
	UdpLinkListType		TrsList;   //UDPFrameType
	UdpLinkListType		OverList;  //UdpLinkOverType + UDPFrameType
	UdpLinkListType		PollList;
	
	udpProcCBFunc       udpProcessFunc[UDPProcess_ID_MAX];
}UDPLinkLayerType;


void UdpLinkLayerInit(void);
void setUdpLinkParam(u8 addr0,u8 addr1,u8 addr2,u8 addr3,u8 type);
void UdpLinkRecv(u8 *pdata, u16 length, struct ip_addr *addr);
void UdpLinkLayerProcess(void);
int CompareFunc_OverList(void *p1, void *p2);
void freeFunc_OverList(void *p1);
u8 UdpLinkLayerSend(u8 *pdata,u16 length,u8 ack,u16 sn,u8 devtype,u8 funcport,u8 supportAddr,u8 vlan);
u8 UdpLinkLayerSend_ext(u8 *pdata,u16 length,u8 ack,u16 sn,u8 devtype,u8 funcport,u8 supportAddr,u8 totalNum,u8 curNum);
void UdpLinkLayerSendProcess(void);
void UdpLinkLayerCycleProcess(u16 delay);


#endif /* __UDP_DATALINK_H */

