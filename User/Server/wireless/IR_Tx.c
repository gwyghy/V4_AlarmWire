/************************************************************
** 文件名:  ir_emit.c
** Copyright (c) 1998-1999 天津华宁电子技术有限公司技术开发部
** 创建人:  HNDZ
** 日　期:  2015.04
** 修改人:	
** 日　期:	
** 描　述:  红外发射驱动与处理程序。
**          用于电液控开发v2.0项目，IR发送编码为NEC格式。
** 版　本:	V1.0A
**-----------------------------------------------------------
*************************************************************/
#ifndef __IR_EMIT_C__
#define __IR_EMIT_C__
#endif

#include "includes.h"
#include "IR_Rx.h"
#include "V4_Wireless.h"


#define TOGGLE_STEP			52									//输出TOGGLE步进值，与分频后的时钟有关

static vu32 s_vu32EmitCompleted = TRUE;						//IR发射完成
static vu32 s_vu32PWMCount = 0;								//PWM发射计数
static vu32 s_vu32ArrayPt = 0;									//发射数组指针
static vu16 s_vu16EmitTimeArray[80];							//IR发送Carrier和idle所占PWM周期计数数组
/***********************************************************
** 函数名：IR_EmitInit
** 输　入：None
** 输　出：None
** 功能描述：IR发射外围设备初始化 
************************************************************/
void IR_EmitInit(void)
{
//#ifndef DYK_SS
	GPIO_InitTypeDef			GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef		TIM_Structure;					//定义定时器结构体
	TIM_OCInitTypeDef			TIM_OCInitStructure;
	NVIC_InitTypeDef			NVIC_InitStructure; 
	
	RCC_AHB1PeriphClockCmd(IR_OUT_GPIO_RCC, ENABLE);			//IR发射端口时钟使能

	/* Configure IR_OUT_GPIO_PIN in output pushpull mode */
	GPIO_InitStructure.GPIO_Pin = IR_OUT_GPIO_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(IR_OUT_GPIO_PORT, &GPIO_InitStructure);

	//设置IR发射引脚复用功能
	GPIO_PinAFConfig(IR_OUT_GPIO_PORT, IR_OUT_SOURCE, IR_OUT_AF);

	TIM_TimeBaseStructInit(&TIM_Structure);						//初始化定时器结构体
	TIM_OCStructInit(&TIM_OCInitStructure);						//初始化定时器通道结构体
	TIM_DeInit(IR_TIM);											//设置TIM默认模式


	RCC_APB2PeriphClockCmd(IR_TIM_RCC, ENABLE);					//IR发射定时器时钟使能

	//禁止定时器和主输出
	TIM_Cmd(IR_TIM, DISABLE);									//关闭定时器
	TIM_CtrlPWMOutputs(IR_TIM, DISABLE);						//关闭脉冲输出端口

#if 1	//使用数PWM方法或使用重复计数器方法
	//初始化IR发射所使用的定时器
	TIM_Structure.TIM_Period = 14;								//设置初始值,定时26.25us中断，决定PWM周期(15/571.43KHz)

	TIM_Structure.TIM_Prescaler = 293;						//设置定时器3的分频值，时钟为571.43KHz(168M/294)
	TIM_Structure.TIM_ClockDivision = TIM_CKD_DIV1;				//设置时钟分割
	TIM_Structure.TIM_CounterMode = TIM_CounterMode_Up;			//TIM向上计数模式
    TIM_Structure.TIM_RepetitionCounter = 0;					//重复计数器
	TIM_TimeBaseInit(IR_TIM, &TIM_Structure);					//按结构体的值，初始化定时器
	TIM_InternalClockConfig(IR_TIM);							//设置TIM的时钟为内部时钟

	//初始化IR发射所使用的定时器的PWM输出通道
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;			//PWM1模式 
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;//输出使能
	TIM_OCInitStructure.TIM_Pulse = (TIM_Structure.TIM_Period+1)/3;//设置PWM占空比。
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;	//设置极性。
	TIM_OC1Init(IR_TIM, &TIM_OCInitStructure);					//初始化通道1输出
	TIM_OC1PreloadConfig(IR_TIM, TIM_OCPreload_Enable);			//使能自动加载比较参数

	//禁止通道中断
	TIM_ITConfig(IR_TIM, TIM_IT_CC1 , DISABLE);					//禁止捕获比较模式中断

	TIM_ClearFlag(IR_TIM, TIM_FLAG_Update);						//清除更新标识
	//初始化定时器溢出中断
    TIM_ITConfig(IR_TIM, TIM_IT_Update, ENABLE);				//使能IR_TIM溢出中断源

	//使能定时器ARR自动重装
	TIM_ARRPreloadConfig(IR_TIM, ENABLE);						//使能自动重装

	//初始化定时器中断
    NVIC_InitStructure.NVIC_IRQChannel = IR_TIM_IRQ;			//使能或者失能指定的IRQ通道 TIM全局中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0f;//不使用中断优先级嵌套。因为SysTick的中断优先级为0x0f
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;	//1;	//设置成员 NVIC_IRQChannel中的从优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				//使能 NVIC_IRQChannel
	NVIC_Init(&NVIC_InitStructure);    
#endif


	//初始化计数器
	IR_TIM->CNT = 0x0000;										//定时器的计数器清零
	//禁止定时器
	TIM_Cmd(IR_TIM, DISABLE);
	//禁止定时器主输出
	TIM_CtrlPWMOutputs(IR_TIM, DISABLE);
//#endif
}
/***********************************************************
** 函数名：IREmitEnabled
** 输　入：None
** 输　出：None
** 功能描述：使能IR发射定时器
************************************************************/
static void IREmitEnabled(void)
{
//#ifndef DYK_SS
	u32 i = 0;
	
	s_vu32PWMCount = 0;
	s_vu32ArrayPt = 1;											//重复计数器装载的维数
	s_vu32EmitCompleted = FALSE;								//IR发射未完成

	//立即装入
	TIM_ARRPreloadConfig(IR_TIM, DISABLE);						//使能立即装入
	TIM_SetAutoreload(IR_TIM, 14);								//设置初始值,定时26.25us中断，决定PWM周期(15/571.43KHz)

	TIM_PrescalerConfig(IR_TIM, 293, TIM_PSCReloadMode_Update);	//设置定时器3的分频值，时钟为571.43KHz(168M/294)
	//装载重复计数值
	IR_TIM->RCR = s_vu16EmitTimeArray[0];
	//自动重装
	TIM_ARRPreloadConfig(IR_TIM, ENABLE);						//使能自动重装
	
	//关闭红外接收
	IR_RxCmd(DISABLE);

	//初始化计数器
	IR_TIM->CNT = 0x0000;										//定时器的计数器清零
	TIM_ClearFlag(IR_TIM, TIM_FLAG_Update);						//清除更新标识
	//使能定时器主输出
	TIM_CtrlPWMOutputs(IR_TIM, ENABLE);							//开始生成脉冲信号
	//使能定时器
	TIM_Cmd(IR_TIM, ENABLE);									//使能定时器
	while(!IR_TIM->CNT && ((++i)<100));							//等待计数开始
	//装载下一次的重复计数值，产生更新事件后重载
	IR_TIM->RCR = s_vu16EmitTimeArray[s_vu32ArrayPt];
//#endif
}
/***********************************************************
** 函数名：	 IR_TIM_CC_IRQHandler
** 功能描述：红外发射中断程序。使用数Toggle方法
************************************************************/
void IR_TIM_CC_IRQHandler(void)
{
#if OS_CRITICAL_METHOD == 3                      /* Allocate storage for CPU status register */
    OS_CPU_SR  cpu_sr = 0;
#endif
//#ifndef DYK_SS
	u32 capture;
//#endif
	
	OS_ENTER_CRITICAL();                         /* Tell uC/OS-II that we are starting an ISR */
	OSIntEnter();
	OS_EXIT_CRITICAL();

//#ifndef DYK_SS
	if(TIM_GetITStatus(IR_TIM,TIM_IT_CC1) == SET)				//判断是否发生定时器溢出中断
	{
		TIM_ClearITPendingBit(IR_TIM, TIM_IT_CC1);			//定时器溢出中断待处理标记清零
		// 获得IR_TIM输入捕获1的值
		capture = TIM_GetCapture1(IR_TIM);
		// 设置IR_TIM捕获比较1寄存器值
		// IR_TIM计数器值向上计数
		TIM_SetCompare1(IR_TIM, capture + TOGGLE_STEP);
		// 累计Toggle计数
		s_vu32PWMCount++;
		// 发送0或者1的一半 
		if ((s_vu16EmitTimeArray[s_vu32ArrayPt] * 2) == s_vu32PWMCount)
		{
			s_vu32PWMCount = 0;
			s_vu32ArrayPt++;
			TIM_CtrlPWMOutputs(IR_TIM, (s_vu32ArrayPt & 0x1) ? DISABLE : ENABLE);// 偶数关,即不发送脉冲
			if (s_vu32ArrayPt >= 69) 
			{
				TIM_Cmd(IR_TIM, DISABLE);
				s_vu32EmitCompleted = TRUE;					//IR发射完成
				//使能红外接收
				IR_RxCmd(ENABLE);
			}	
		}
	}
//#endif
	OSIntExit();
}
/***********************************************************
** 函数名：	 IR_TIM_IRQHandler
** 功能描述：红外发射中断程序。使用数PWM的方法，或者使用重复计数器方法
************************************************************/
void IR_TIM_IRQHandler(void)
{
#if OS_CRITICAL_METHOD == 3                      /* Allocate storage for CPU status register */
    OS_CPU_SR  cpu_sr = 0;
#endif

	OS_ENTER_CRITICAL();                         /* Tell uC/OS-II that we are starting an ISR */
	OSIntEnter();
	OS_EXIT_CRITICAL();

//#ifndef DYK_SS
///* 使用重复计数器方法
	if(TIM_GetITStatus(IR_TIM,TIM_IT_Update) == SET)				//判断是否发生定时器溢出中断
	{
		TIM_ClearITPendingBit(IR_TIM, TIM_IT_Update);			//定时器溢出中断待处理标记清零

 		if ((s_vu32ArrayPt == 0) 
		|| (s_vu32ArrayPt == 68) 
		|| ((s_vu32ArrayPt & 0x1) == 1))
			TIM_CtrlPWMOutputs(IR_TIM, ENABLE);					//单数数组成员生成脉冲
		else
			TIM_CtrlPWMOutputs(IR_TIM, DISABLE);				//双数数组成员不生成脉冲
		if(s_vu32ArrayPt >= 72)									//完成1次数据输出（最后1个idle态不需要输出）
		{
			TIM_Cmd(IR_TIM, DISABLE);							//信号生成完毕,关闭
			IR_TIM->CNT = 0x0000;								//定时器的计数器清零
			s_vu32EmitCompleted = TRUE;							//IR发射完成
			//使能红外接收
			IR_RxCmd(ENABLE);
		}else
		{
			u32 i=0;
			s_vu32ArrayPt++;
			while(!IR_TIM->CNT && ((++i)<100));					//等待计数开始
			//装载下一次的重复计数值，产生更新事件后重载
			IR_TIM->RCR = s_vu16EmitTimeArray[s_vu32ArrayPt];
		}
	}
	OSIntExit();
}
/***********************************************************
** 函数名：IsCompletedForIREmit
** 输　入：None
** 输　出：TRUE=IR发送完成；FALSE=IR发送中
** 功能描述：获取IR发射完成标志
************************************************************/
u32 IsCompletedForIREmit(void)
{
	u32	i;
#if OS_CRITICAL_METHOD == 3                      /* Allocate storage for CPU status register */
    OS_CPU_SR  cpu_sr = 0;
#endif

	OS_ENTER_CRITICAL();                         /* Tell uC/OS-II that we are starting an ISR */
	i = s_vu32EmitCompleted;
	OS_EXIT_CRITICAL();
	return(i);
}
/***********************************************************
** 函数名：IR_EmitProc
** 输　入：Wl_EmitCan：扩展CAN格式发射数据
** 输　出：TRUE=发送；FALSE=错误
** 功能描述：IR发射数据处理。应该在任务中周期调用该函数
************************************************************/
u32 IR_EmitProc(EXT_CAN_FRAME_s *IR_EmitCan)
{
//#ifndef DYK_SS
	u32	i,j,k;
	IR_INFO_u	data;
	u8	c;

	if ((IR_EmitCan->u32ID.ID.FT != V4YKQ_IR_CODEMATCH_ACK) 
		&& (IR_EmitCan->u32ID.ID.FT != V4YKQ_IR_CODEMATCH))
		return(FALSE);
	if (IR_EmitCan->u32ID.ID.FT == V4YKQ_IR_CODEMATCH_ACK) 
	{
		data.sIrInfo.Type = 2;									//命令类型
		data.sIrInfo.ScNoLSB3 = (((u32)IR_EmitCan->u8DT[4] << 8) + IR_EmitCan->u8DT[3]) & 0x7;//对码架号低3位
		data.sIrInfo.Sign1 = 1;									//起始标识
		data.sIrInfo.ScNoMSB6 = ((((u32)IR_EmitCan->u8DT[4] << 8) + IR_EmitCan->u8DT[3]) >> 3) & 0x3f;//对码架号低6位
		data.sIrInfo.Dir = 1;									//发送方向
		data.sIrInfo.Sign2 = 0;									//中间字节标识
		data.sIrInfo.Result = IR_EmitCan->u8DT[2] & 0x01;		//对码结果
		data.sIrInfo.ACK = 0;									//应答标识
		data.sIrInfo.RemoteID = IR_EmitCan->u8DT[5] & 0xf;		//遥控器ID
		data.sIrInfo.ScNoIncDir = IR_EmitCan->u8DT[1] & 0x01;	//支架控制器架号增向
		data.sIrInfo.Sign3 = 0;									//中间字节标识
	}
	else
	{
///* for test
		data.sIrInfo.Type = 1;									//命令类型
		data.sIrInfo.ScNoLSB3 = 0;								//对码架号低3位
		data.sIrInfo.Sign1 = 1;									//起始标识
		data.sIrInfo.ScNoMSB6 = 0;								//对码架号低6位
		data.sIrInfo.Dir = 0;									//发送方向
		data.sIrInfo.Sign2 = 0;									//中间字节标识
		data.sIrInfo.Result = 0;								//对码结果
		data.sIrInfo.ACK = 0;									//应答标识
		data.sIrInfo.RemoteID = 1;								//遥控器ID
		data.sIrInfo.ScNoIncDir = 0;							//支架控制器架号增向
		data.sIrInfo.Sign3 = 0;									//中间字节标识
//*/
	}

	k = 0;
///* 重复计数器法
 	s_vu16EmitTimeArray[k++] = 171;								//k=0,k=1脉冲数组成员生成9ms的连续脉冲
 	s_vu16EmitTimeArray[k++] = 170;								//k=0,k=1脉冲数组成员生成9ms的连续脉冲

	s_vu16EmitTimeArray[k++] = 170;								//第3个脉冲数组成员生成4.5ms的低电平
	for (i = 0; i < 4; i++)
	{
		if (i == 3)												//红外发射数据只能是4个字节，其中，最后一个是倒数第2个的反码
		{
			c = data.u8IrInfo[i - 1];
			c = ~c;
		}
		else
			c = data.u8IrInfo[i];
		for (j = 0; j < 8; j++)
		{
			if((c >> j) & 1)									//Bit=1时：Carrier=0.56ms, Idle=1.68ms
			{
				s_vu16EmitTimeArray[k++] = 20;
				s_vu16EmitTimeArray[k++] = 63;
			}else												//Bit=0时：Carrier=0.56ms, Idle=0.56ms
			{
				s_vu16EmitTimeArray[k++] = 20;
				s_vu16EmitTimeArray[k++] = 20;
			}
		}
	}
///* 重复计数器法
 	s_vu16EmitTimeArray[k++] = 120;
 	s_vu16EmitTimeArray[k++] = 110;
 	s_vu16EmitTimeArray[k++] = 110;
//*/
/* 数PWM法或数TOGGLE法
	s_vu16EmitTimeArray[k++]=342;								//发送结束码
*/
	s_vu16EmitTimeArray[k++] = 85;
	s_vu16EmitTimeArray[k++] = 21;
	s_vu16EmitTimeArray[k++] = 100;								//额外的
	s_vu32PWMCount = 0;
	s_vu32ArrayPt = 0;
	
///* 重复计数器法
	IREmitEnabled();											//启动发送

	return(TRUE);
}

