#ifndef __UART_H__ 
#define __UART_H__

/********************************************************************************
* #define宏定义
*********************************************************************************/
#define BAUD19200	19200
/********************************************************************************
* 全局变量声明
*********************************************************************************/

// USART6
#define USART6_PORT			GPIOC
#define USART6_TX_PIN		GPIO_Pin_6
#define USART6_RX_PIN		GPIO_Pin_7

#define USART6_GPIO_RCC		RCC_AHB1Periph_GPIOC
#define USART6_APB			APB2
#define USART6_RCC			RCC_APB2Periph_USART6

#define USART6_TX_SOURCE	GPIO_PinSource6
#define USART6_RX_SOURCE	GPIO_PinSource7

#define USART6_GPIO_AF		GPIO_AF_USART6


// IR接收接口（485）
#define IR_RX_USART			USART6
#define IR_RX_PORT			USART6_PORT
#define IR_RX_APB			USART6_APB
#define IR_TX_PIN			USART6_TX_PIN			//IO用于IR发送
#define IR_RX_PIN			USART6_RX_PIN

#define IR_RX_GPIO_RCC		USART6_GPIO_RCC
#define IR_RX_RCC			USART6_RCC

#define IR_TX_SOURCE		USART6_TX_SOURCE		//IO用于IR发送
#define IR_RX_SOURCE		USART6_RX_SOURCE

#define IR_RX_GPIO_AF		USART6_GPIO_AF

#define IR_RX_USART_IRQ		USART6_IRQn
#define IR_RX_USART_IRQHandler	USART6_IRQHandler


// IR发射接口
#define IR_TIM				TIM8
#define IR_TIM_RCC			RCC_APB2Periph_TIM8
#define IR_TIM_APB			APB2
#define IR_TIM_IRQ			TIM8_UP_TIM13_IRQn
#define IR_TIM_IRQHandler	TIM8_UP_TIM13_IRQHandler
#define IR_TIM_CC_IRQ		TIM8_CC_IRQn
#define IR_TIM_CC_IRQHandler	TIM8_CC_IRQHandler
#define IR_OUT_AF			GPIO_AF_TIM8


#define IR_OUT_GPIO_PORT	GPIOC
#define IR_OUT_GPIO_RCC		RCC_AHB1Periph_GPIOC
#define IR_OUT_GPIO_PIN		GPIO_Pin_6

#define IR_OUT_SOURCE		GPIO_PinSource6


//IR红外协议帧结构
typedef struct{
	u32 Type:4;
	u32 ScNoLSB3:3;
	u32 Sign1:1;
	u32 ScNoMSB6:6;
	u32 Dir:1;
	u32 Sign2:1;
	u32 Result:1;
	u32 ACK:1;
	u32 RemoteID:4;
	u32 ScNoIncDir:1;
	u32 Sign3:1;
} IR_INFO_s;
//IR红外协议帧联合
typedef union{
	IR_INFO_s	sIrInfo;
	u8	u8IrInfo[3];
} IR_INFO_u;


//接口CAN帧ID位结构
__packed typedef struct
{
	unsigned int	RID:3;			//接收ID
	unsigned int	TID:3;			//发送ID
	unsigned int	FT:10;			//帧类别
	unsigned int	SN:4;			//帧发送流水，0x0-0xf循环，重发帧和应答帧不参与发送流水。
	unsigned int	SUM:5;			//SUB=0：SUM：组合帧数=SUM+1，SUB=1：SUM：子帧序号=0至SUM-1
	unsigned int	SUB:1;			//0：组合帧总数帧：1：子帧
	unsigned int	ACK:1;			//要求确认位：1-要求确认；0-确认帧或不要求确认
	unsigned int	RD:2;			//保留扩展位：0x00：数据帧；0x01：程序传递操作
	unsigned int	NC:3;			//无效位
} FID_s;
//接口CAN帧ID位联合
__packed typedef union
{
	FID_s	ID;
	u32		u32Id;
} FID_u; 
//扩展CAN数据帧结构
__packed typedef struct
{
	FID_u u32ID; 
	u8	u8DT[8];					//数据字节
	u16	u16DLC;						//数据字节数
} EXT_CAN_FRAME_s;




/********************************************************************************
* 函数声明
*********************************************************************************/
void IrRxAddIrInf(u32 id,u32 status,u32 times);
void IrRxClearIrInf(u32 id);
u32 IrRxGetIrDevId(u8 id);
u32 IrRxGetIrRevStatus(u8 id);
u32 IrRxGetIrRevTimes(u8 id);
void IR_RxCmd(FunctionalState StateVal);
void IR_RxUartInit(void);
void IR_EmitInit(void);
u32 IR_EmitProc(EXT_CAN_FRAME_s *IR_EmitCan);
#endif //__UART_H__
