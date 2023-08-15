/************************************************************
** 文約名:  ir_rx.c
** Copyright (c) 1998-1999 天津华宁电子技术有限公司技术开发部
** 创建人:  HNDZ
** 日　期:  2015.04
** GS改人:	
** 日　期:	
** 描　述:  红外接收串口（IR_USART）驱动与接收处理程序。
**          用于电液控开发v2.0项目，IR接收芯片BC7210A。
** 版　本:	V1.0A
**-----------------------------------------------------------
*************************************************************/

#include "includes.h"
#include "IR_Rx.h"
#include "V4_Wireless.h"

#include "Wireless_Alarm.h"
#include "sys_msg_def.h"


#define IR_DEV_MAX 		0x01	//红外发射设备的最大数量

#define IR_RX_INFO_HEAD		0x80	//IR红外接收遥控器对码数据的起始标志
#define IR_RX_MINE_HEAD		0x01	//IR红外接收红外发射器数据的起始标志,01
#define IR_RX_MINE_END		0xFE	//IR红外接收红外发射器数据的结束标志
#define IR_RX_INFO_LENGTH	0x03	//IR接收数据一个完整帧数据长度

#define IR_RX_BUF_MAX		64		//IR接收缓冲区长度
#define IR_TX_BUF_MAX		0		//IR发送缓冲区长度

#define IR_MINE_SN_MASK		0x10	//红外接收，模拟发送流水号

u8	IR_RxBuf[IR_RX_BUF_MAX];		//接收缓存
u32	IR_RxBufWpt = 0;				//接收写指针
u32	IR_RxBufRpt = 0;				//接收读指针
u32	IR_RxBufCnt = 0;				//接收计数

u32  u32IRId[IR_DEV_MAX]={0x00};			//红外发射的设备号
u32	 u32IRData[IR_DEV_MAX]={0x00};			//红外接收的用户数据，0x00正常，0x01不正常
u32	 u32IRDataCtr[IR_DEV_MAX]={0x00};		//红外接收计蔵

/***********************************************************
** 函数名：IrRxAddIrInf
** 输　入：id蔵组G蚝?status状态，times次数
** 输　出：None
** 功能描述：累加红外接收的接收次数和状态
************************************************************/
void IrRxAddIrInf(u32 id, u32 status, u32 times)
{
	#if OS_CRITICAL_METHOD == 3                      /* Allocate storage for CPU status register */
		OS_CPU_SR  cpu_sr = 0;
	#endif
	
	if(id > IR_DEV_MAX)
		return;
	
	//关全局中断？
	OS_ENTER_CRITICAL();
	u32IRId[id - 1] = id;
	u32IRData[id - 1] += status;//红外模块之间通讯正常与否，否为0x1，正常0x0;最终发送的u32IRData 是一次上报间隔期间不正确的通讯
	u32IRDataCtr[id - 1]++;
	//开全局諫断？
	OS_EXIT_CRITICAL();	
	
}
/***********************************************************
** 函数名：IrRxClearIrInf
** 输　入：id蔵组G蚝?
** 输　出：None
** 功能描述：清除红外接收的接收次数和状态
************************************************************/
void IrRxClearIrInf(u32 id)
{
	#if OS_CRITICAL_METHOD == 3                      /* Allocate storage for CPU status register */
		OS_CPU_SR  cpu_sr = 0;
	#endif
	
	if(id > IR_DEV_MAX)
		return;
	
	//关全局中断？
	OS_ENTER_CRITICAL();
	u32IRId[id - 1] = 0x00;
	u32IRData[id - 1] = 0x00;
	u32IRDataCtr[id - 1] = 0x00;
	//开全局中断？
	OS_EXIT_CRITICAL();		
}
/***********************************************************
** 函数名：IrRxGetIrDevId
** 输　入：id蔵组G蚝?1-3
** 输　出：None
** 功能描述：获取数组諫的设备ID
************************************************************/
u32 IrRxGetIrDevId(u8 id)
{
	if(id > IR_DEV_MAX)
		return 0xFF;
	
	return u32IRId[id - 1];
}

/***********************************************************
** 函数名：IrRxGetIrRevStatus
** 输　入：id蔵组G蚝?1-3
** 输　出：None
** 功能描述：获取数组諫的设备接收次数
************************************************************/
u32 IrRxGetIrRevStatus(u8 id)
{
	if(id > IR_DEV_MAX)
		return 0xFF;
	
	return u32IRData[id - 1];
}

/***********************************************************
** 函数名：IrRxGetIrRevTimes
** 输　入：id数组G蚝?1-3
** 输　出：None
** 功能描述：获取数组諫的设备接收次数
************************************************************/
u32 IrRxGetIrRevTimes(u8 id)
{
	if(id > IR_DEV_MAX)
		return 0xFF;
	
	return u32IRDataCtr[id - 1];
}

/***********************************************************
** 函蔵名：IR_RxDataProc
** 输　入：None
** 输　出：None
** 功能描述：红外接收数据处理。以下函数应该在任务内周期执行。
************************************************************/
void IR_RxDataProc(void)
{
	u16	u16i, u16j;
	IR_INFO_u RxIrInfo;
	EXT_CAN_FRAME_s RxIrCan;
#if OS_CRITICAL_METHOD == 3                      /* Allocate storage for CPU status register */
    OS_CPU_SR  cpu_sr = 0;
#endif

	OS_ENTER_CRITICAL();
	u16j = IR_RxBufCnt;
	OS_EXIT_CRITICAL();
   	while (u16j >= IR_RX_INFO_LENGTH)
	{
		u16i = IR_RxBufRpt;
		if((((IR_RxBuf[u16i] & IR_RX_INFO_HEAD) == IR_RX_INFO_HEAD) && (IR_RxBuf[u16i] != IR_RX_MINE_END))\
			||((IR_RxBuf[u16i] & IR_RX_MINE_HEAD) == IR_RX_MINE_HEAD))
		{
			RxIrInfo.u8IrInfo[0] = IR_RxBuf[u16i++];		//地址码1
			u16i %= IR_RX_BUF_MAX;
			if ((IR_RxBuf[u16i] & IR_RX_INFO_HEAD) != IR_RX_INFO_HEAD)
			{
				RxIrInfo.u8IrInfo[1] = IR_RxBuf[u16i++];	//地址码2
				u16i %= IR_RX_BUF_MAX;
				if((((IR_RxBuf[u16i] & IR_RX_INFO_HEAD) != IR_RX_INFO_HEAD) && (IR_RxBuf[u16i] != 0x00))\
				  ||(IR_RxBuf[u16i] == IR_RX_MINE_END))
				{
					RxIrInfo.u8IrInfo[2] = IR_RxBuf[u16i++];//用户字节
					u16i %= IR_RX_BUF_MAX;
					IR_RxBufRpt = u16i;						//更新读取指针
					OS_ENTER_CRITICAL();
					IR_RxBufCnt = IR_RxBufCnt - IR_RX_INFO_LENGTH;
					u16j = IR_RxBufCnt;
					OS_EXIT_CRITICAL();
					//添加代码。转换为扩展CAN数据格式
					if (RxIrInfo.sIrInfo.Type < 9)
					{
						switch (RxIrInfo.sIrInfo.Type)
						{
							case 0x01://红外对码数据帧 或红外煤机定位数据，掩码为0x80 或 0x03
								if((RxIrInfo.u8IrInfo[0] & IR_RX_INFO_HEAD) == IR_RX_INFO_HEAD)//遥控器对码数据
								{
									RxIrCan.u8DT[0] = 0x0;
									RxIrCan.u8DT[1] = 0x0;
									RxIrCan.u8DT[2] = RxIrInfo.sIrInfo.ScNoIncDir<<7;
									RxIrCan.u8DT[3] = 0x0;
									RxIrCan.u8DT[4] = 0x0;
									RxIrCan.u8DT[5] = RxIrInfo.sIrInfo.RemoteID;
									RxIrCan.u8DT[6] = 0x0;
									RxIrCan.u8DT[7] = 0x0;
									RxIrCan.u16DLC = 8;
									RxIrCan.u32ID.ID.NC = 0x00;
									RxIrCan.u32ID.ID.RD = 0;
									RxIrCan.u32ID.ID.ACK = 0;
									RxIrCan.u32ID.ID.FT = V4YKQ_IR_CODEMATCH;
									RxIrCan.u32ID.ID.RID = 0;
									RxIrCan.u32ID.ID.TID = 3;
									RxIrCan.u32ID.ID.SUM = 0;
									RxIrCan.u32ID.ID.SUB = 0;
									RxIrCan.u32ID.ID.SN = 0;
								
									if(SysParam.globalParam.SupportParam.Paring_Method == 0)    //使用串口通信时的无线模块																						
										InsUsartIrRecvData_proc((STR_WLCAN_Frame *)&RxIrCan,sizeof(RxIrCan));
									else if(SysParam.globalParam.SupportParam.Paring_Method ==1) // 使用CAN通信时的无线模块
									    InsAlarmWirelessIrRecvData_proc((STR_WLCAN_Frame *)&RxIrCan,sizeof(RxIrCan));
								}
								else if((RxIrInfo.u8IrInfo[0] & IR_RX_MINE_HEAD) == IR_RX_MINE_HEAD)
								{
									u32 i = 0x00;
									i = (((u32)RxIrInfo.u8IrInfo[1])<<8) + (u32)RxIrInfo.u8IrInfo[0];
									if (i > 0 && i < 4)
									{
										IrRxAddIrInf(i,(u32)RxIrInfo.u8IrInfo[2],0x01);
									}
								}
								else
									;
								break;
							
							default:
								break;

						}
					}
					else
					{
						IR_RxBufRpt++;
						IR_RxBufRpt %= IR_RX_BUF_MAX;
						OS_ENTER_CRITICAL();
						if(IR_RxBufCnt > 0)//mry,20.07.23,IR_RxBufCnt计数不能超过IR_RX_BUF_MAX
						{
							IR_RxBufCnt--;
							u16j = IR_RxBufCnt;
						}
						else
						{
							u16j = 0x00;
						}
						OS_EXIT_CRITICAL();
					}
					continue;
				}
			}
		}
		IR_RxBufRpt++;
		IR_RxBufRpt %= IR_RX_BUF_MAX;
		OS_ENTER_CRITICAL();
		if(IR_RxBufCnt > 0)//mry,20.07.23,IR_RxBufCnt计数不能超过IR_RX_BUF_MAX
		{
			IR_RxBufCnt--;
			u16j = IR_RxBufCnt;
		}
		else
		{
			u16j = 0x00;
		}
		OS_EXIT_CRITICAL();
	}
}
/***********************************************************
** 函数名：IR_RxUartInit
** 输　入：None
** 输　出：None
** 功能描述： 
************************************************************/
void IR_RxUartInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;   
//#ifndef DYK_SS
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef  NVIC_InitStructure;
//#endif
	
	RCC_AHB1PeriphClockCmd(IR_RX_GPIO_RCC, ENABLE);			//串口端口时钟使能

	RCC_APB2PeriphClockCmd(IR_RX_RCC, ENABLE);				//红外串口时钟使能
		
	/* Configure IR_TX_PIN in output pushpull mode */
	GPIO_InitStructure.GPIO_Pin = IR_RX_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(IR_RX_PORT, &GPIO_InitStructure);


	GPIO_PinAFConfig(IR_RX_PORT, IR_RX_SOURCE, IR_RX_GPIO_AF);
	
	USART_InitStructure.USART_BaudRate = 9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;		//无校验
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx;			// | USART_Mode_Tx; 
	USART_Init(IR_RX_USART, &USART_InitStructure);			//红外

	USART_ITConfig(IR_RX_USART, USART_IT_RXNE, ENABLE);		//中断使能


	/* Configure the NVIC Preemption Priority Bits */  
	/* Enable the USARTy Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = IR_RX_USART_IRQ;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x05;	//不使用中断优先级嵌套。因为SysTick的中断优先级为0x0f
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;//0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	USART_Cmd(IR_RX_USART, ENABLE);							//串口使能
}
/***********************************************************
** 函数名：IR_RxCmd
** 输　入：StateVal=ENABLE：允许红外接收；=DISABLE：禁止红外接收
** 输　出：None
** 功能描述： 设置允许红外接收，或禁止红外接收
************************************************************/
void IR_RxCmd(FunctionalState StateVal)
{
	if (StateVal == DISABLE)
	{
		USART_ITConfig(IR_RX_USART, USART_IT_RXNE, DISABLE);//禁止接收中断
		USART_ITConfig(IR_RX_USART, USART_IT_TXE, DISABLE);	//禁止发空中断
		USART_ITConfig(IR_RX_USART, USART_IT_TC, DISABLE);	//禁止发送完成中断
		while(USART_GetFlagStatus(IR_RX_USART, USART_FLAG_TC) == RESET);//等待发送结束
		(void)IR_RX_USART->SR;								//用于清除标志
		(void)IR_RX_USART->DR;								//用于清除标志
	}else
	{
		(void)IR_RX_USART->SR;								//用于清除标志
		(void)IR_RX_USART->DR;								//用于清除标志
		USART_ITConfig(IR_RX_USART, USART_IT_RXNE, ENABLE);	//接收中断使能
	}
	USART_Cmd(IR_RX_USART, StateVal);						//串口设置
}
/***********************************************************
** 函数名：	 IR_RX_USART_IRQHandler
** 功能描述：红外串口接收中断程序。
************************************************************/
void IR_RX_USART_IRQHandler(void)
{
	OS_CPU_SR  cpu_sr;

	OS_ENTER_CRITICAL();						/* Tell uC/OS-II that we are starting an ISR */
	OSIntEnter();
	OS_EXIT_CRITICAL();


	if ((USART_GetITStatus(IR_RX_USART, USART_IT_RXNE) != RESET) 
		|| (USART_GetITStatus(IR_RX_USART, USART_IT_ORE_RX) != RESET))
	{
		u16 data;
		data = USART_ReceiveData(IR_RX_USART);
		
		OS_ENTER_CRITICAL();					/* Tell uC/OS-II that we are starting an ISR */
		if (IR_RxBufCnt < IR_RX_BUF_MAX)
		{
			IR_RxBuf[IR_RxBufWpt++] = (u8)(data & 0xff);
			IR_RxBufWpt %= IR_RX_BUF_MAX;
			IR_RxBufCnt++;
		}
		OS_EXIT_CRITICAL();
	}

	OSIntExit();
}
