#ifndef __UART_H__ 
#define __UART_H__

/********************************************************************************
* #define�궨��
*********************************************************************************/
#define BAUD19200	19200
/********************************************************************************
* ȫ�ֱ�������
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


// IR���սӿڣ�485��
#define IR_RX_USART			USART6
#define IR_RX_PORT			USART6_PORT
#define IR_RX_APB			USART6_APB
#define IR_TX_PIN			USART6_TX_PIN			//IO����IR����
#define IR_RX_PIN			USART6_RX_PIN

#define IR_RX_GPIO_RCC		USART6_GPIO_RCC
#define IR_RX_RCC			USART6_RCC

#define IR_TX_SOURCE		USART6_TX_SOURCE		//IO����IR����
#define IR_RX_SOURCE		USART6_RX_SOURCE

#define IR_RX_GPIO_AF		USART6_GPIO_AF

#define IR_RX_USART_IRQ		USART6_IRQn
#define IR_RX_USART_IRQHandler	USART6_IRQHandler


// IR����ӿ�
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


//IR����Э��֡�ṹ
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
//IR����Э��֡����
typedef union{
	IR_INFO_s	sIrInfo;
	u8	u8IrInfo[3];
} IR_INFO_u;


//�ӿ�CAN֡IDλ�ṹ
__packed typedef struct
{
	unsigned int	RID:3;			//����ID
	unsigned int	TID:3;			//����ID
	unsigned int	FT:10;			//֡���
	unsigned int	SN:4;			//֡������ˮ��0x0-0xfѭ�����ط�֡��Ӧ��֡�����뷢����ˮ��
	unsigned int	SUM:5;			//SUB=0��SUM�����֡��=SUM+1��SUB=1��SUM����֡���=0��SUM-1
	unsigned int	SUB:1;			//0�����֡����֡��1����֡
	unsigned int	ACK:1;			//Ҫ��ȷ��λ��1-Ҫ��ȷ�ϣ�0-ȷ��֡��Ҫ��ȷ��
	unsigned int	RD:2;			//������չλ��0x00������֡��0x01�����򴫵ݲ���
	unsigned int	NC:3;			//��Чλ
} FID_s;
//�ӿ�CAN֡IDλ����
__packed typedef union
{
	FID_s	ID;
	u32		u32Id;
} FID_u; 
//��չCAN����֡�ṹ
__packed typedef struct
{
	FID_u u32ID; 
	u8	u8DT[8];					//�����ֽ�
	u16	u16DLC;						//�����ֽ���
} EXT_CAN_FRAME_s;




/********************************************************************************
* ��������
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
