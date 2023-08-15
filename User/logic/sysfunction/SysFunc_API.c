/*
*********************************************************************************************************
*	                                            DoubleWires
*                           Commuication Controller For The Mining Industry
*
*                                  (c) Copyright 1994-2013  HNDZ 
*                       All rights reserved.  Protected by international copyright laws.
*
*
*    File    : SysFunc_API.c
*    Module  : system function
*    Version : V1.0
*    History :
*   -----------------
*              ʵ�ֿ���������
*              Version  Date           By            Note
*              v1.0     
*
*********************************************************************************************************
*/

#define __SYSFUNC_API_C__

#include "sys_default.h"
#include "SysFunc_API.h"
#include "sysParamAnalysis.h"
#include "udp_app_proc.h"
#include "Util_Timer.h"
#include "FileUpdate.h"
#include "ProjectConfig.h"

#if (LOGIC_SYSFUNC_EN == 1)

static int deviceTick=0;
static int devicePersonTick=0;
static int hwstopTick=0;
static int IrReportTick=0;
static int RsetReasonTick = 0;
static int networkhearttimes = 0;
static SysCtrlInfoBufType    ctrlInfoMng;
static ActionInfoBufType	 actionInfoMng;
static ActionInfoBufType	 manualActionMng;
static u8 emergencyState=SYSSTOP_UNPRESS;
static u8 manualCtrlAddr =0;
static globalStateType globalStateBak;
static uint8_t sysInitFlag = 0;
static ParamUnit deviceState[SYSDEVICE_NUM] = {0};


u8 PersonLockStateTimerId;//2S�������
ParamUnit personLockState[PERSON_MAX];
u16 personLockTimer = 0;   //2����ǩ����ʱ��
ParamUnit personLockStateLast;
u8 UnlockTimerFlag = 0;
u16 UnlockTimer = 0;
u32 ClearPersonLockState = 0;


/***********************0����ǩ*************************/
typedef struct
{
	uint8_t ZeroPersonLockState;//����״̬
	uint8_t LastZeroPersonLockState;//��һ�α���״̬	
	uint8_t ZeroPersonLockNum;//�����ܺ�
}
stZeroPersonLock;
stZeroPersonLock zeroPersonLock[3];
u8 AllZeroPersonLockState = 0;
/***********************end*************************/


static void SysAllActionStop(u8 src);
static void netReportBisuoState(u8 stateType,u8 state);

/*******************************************************************************************
*��������������������Χ�豸��ʼ��
*��ڲ�������
*����ֵ��  ��
*******************************************************************************************/
void DeviceEnableInit(void)
{
	int i;
	u8 gap;
	
	gap = ((u8 *)(&SysParam.Sensor[0].enable) -(u8 *)(&SysParam.Sensor[0].sensorName))/2;
	for(i=1; i<=SENSOR_MAX;i++)
		ParamProcess(ENUM_SENSOR,i,gap,1);
	
	ParamProcess(ENUM_SYSTEM,globalParamEnum_DeviceAlarm,0,1);
	ParamProcess(ENUM_SYSTEM,globalParamEnum_DevicePeople,0,1);
	ParamProcess(ENUM_SYSTEM,globalParamEnum_DeviceInfirred,0,1);
	ParamProcess(ENUM_SYSTEM,globalParamEnum_DeviceWireless,0,1);
	ParamProcess(ENUM_SYSTEM,globalParamEnum_DeviceHub,0,1); //ghy ����HUB
	ParamProcess(ENUM_SYSTEM,globalParamEnum_DeviceTrans,0,1);
	for(i=0; i<SYSDEVICE_NUM;i++)
	{
		deviceState[i] = 1;
	}
}

/*******************************************************************************************
*��������������ģ�͡���־���豸������״̬��ʼ��
*��ڲ�������
*����ֵ��  ��
*******************************************************************************************/
void FuncInit(void)
{
	int i;
	memset(&SysState,0,sizeof(SysStateType));
	
	if(RCC->CSR &0x80000000)
		kzqTurnonReson = 5;//�͹���
	else if(RCC->CSR &0x40000000)
		kzqTurnonReson = 3;//���ڿ��Ź�
	else if(RCC->CSR &0x20000000)
		kzqTurnonReson = 4;//�������Ź�
	else if(RCC->CSR &0x10000000)
		kzqTurnonReson = 2;//���
	else
		kzqTurnonReson = 1;//����	

	
	Info_Data_Init();//��־��ʼ��
	ActionDataInit();//�ֶ����������ʼ��
	AlarmModuleInit();//����ģ�ͳ�ʼ��
	
	//������ģ�ͳ�ʼ��
	for(i=0; i<SENSOR_MAX;i++)
		SensorModuleInit(&SysParam.Sensor[i],&SysState.Sensor[i],&SysState.ManualSensor[i],i); 

	//������ģ�ͳ�ʼ��
	SigActionModuleInit();	
	for(i=0; i<SINGLEACTION_NUM_MAX;i++)
		SigActionModuleLoad(i,&SysParam.SigAction[i],&SysState.SigAction[i]);

	//��϶���ģ�ͳ�ʼ��
	CombActionModuleInit();
	for(i=0; i<COMBACTION_NUM_MAX;i++)
		CombActionModuleLoad(i,&SysParam.CombAction[i],&SysState.CombAction[i]);

	//���鶯��ģ�ͳ�ʼ��
	groupActionModuleInit();
	for(i=0; i<GROUPACTION_NUM_MAX;i++)
		groupActionModuleLoad(i,&SysParam.GroupAction[i],&SysState.GroupAction[i]);	
	
	//����ģ�ͳ�ʼ��	
	partModuleInit();
	for(i=0; i<PART_NUM_MAX;i++)
		partModuleLoad(i,&SysParam.Part[i],&SysState.Part[i]);	
	
	//�豸����ʼ��
	DeviceEnableInit();
	SysState.globalState.LeftCommState=1;
	SysState.globalState.RightCommState=1;
	SysState.globalState.LeftglobalCRCState = 1;
	SysState.globalState.RightglobalCRCState = 1;
	SysState.globalState.globalCRC = GlobalParamCRCCal();
	setUdpAppSelfParamInfo((u8 *)(&SysState.globalState.globalCRC),2);
	
	
	zeroPersonLock[0].ZeroPersonLockNum = SysParam.globalParam.SupportParam.currentSupportID-1;
	zeroPersonLock[1].ZeroPersonLockNum = SysParam.globalParam.SupportParam.currentSupportID;
	zeroPersonLock[2].ZeroPersonLockNum = SysParam.globalParam.SupportParam.currentSupportID+1;
	
	PLATFORM_OS_TimeDly(200);	
}


/*******************************************************************************************
*�����������豸�ṹ�帳ֵ����
*��ڲ�����deviceNumȡֵ��
*            �豸������
*          driver�����ţ�
*             ������
*          typeȡֵ��
*             �豸����
*          pointȡֵ��
*             ���
*����ֵ��  �豸�Ƿ�ʹ�ܣ�
*             0��δʹ��
*             1��ʹ��
*******************************************************************************************/
static u8 setDeviceMsg(u8 deviceNum,u16 driver,u16 type,u16 point)
{
	stDeviceStatusMsg deviceMsg;	
	GetCanDeviceStatusMsg(driver, type, point, &deviceMsg);
	if(type == ID_EMVD)
	{
		deviceMsg.isEnable = 1;
	}
	if(deviceMsg.isEnable)
	{
		SysState.Device[deviceNum].driverEnable = deviceMsg.isEnable;
		SysState.Device[deviceNum].driverID = driver+1;
		SysState.Device[deviceNum].deviceType = type;
		SysState.Device[deviceNum].deviceNum =point+1;
		SysState.Device[deviceNum].deviceState = deviceMsg.isOnline;
		SysState.Device[deviceNum].deviceValue =0;
		memcpy(&SysState.Device[deviceNum].deviceAppVer , &deviceMsg.deviceVersion,sizeof(deviceMsg.deviceVersion));
		memcpy(&SysState.Device[deviceNum].deviceBootVer , &deviceMsg.deviceBLVersion,sizeof(deviceMsg.deviceBLVersion));
		return 1;
	}
	return 0;
}

/*******************************************************************************************
*�����������豸״̬�ϱ�������
*��ڲ�����deviceIDȡֵ��
*            �豸������
*����ֵ��  ��ʵ�����壺
*******************************************************************************************/
static u8 serverReportDevice(u8 deviceID)
{
#ifdef SERVER_REPORT_ENABLE
	u8 temp[40];
	u8 serverID;
	u16 length;
	u16 serverStatus;
		
	if(getSeverStatus() == 0)
		return 1;
	
	if(deviceState[deviceID] != SysState.Device[deviceID].deviceState && SysState.Device[deviceID].driverEnable == 1)
	{
		memset(temp,0,sizeof(temp));
		
		temp[0] = REPORTENUM_DEVICE;
		temp[1] = ENUM_DEVICE;
		temp[2] = deviceID+1;
		length =sizeof(deviceStateType);
		memcpy(&temp[3],&length,2);
		memcpy(&temp[5],&SysState.Device[deviceID],length);
		
		serverID = getSysServerAddr();
		UdpAppProcCtrl(AppProcCtrl_stateReport_indication,temp,5+length,serverID);
		
		deviceState[deviceID] = SysState.Device[deviceID].deviceState;
	}
#endif
	return 1;
}

u8 resetReasonflag = 1;
static u8 ledState;
extern u8 turnonRes;

/*******************************************************************************************
*�����������豸ϵͳ״̬������
*��ڲ�����delayȡֵ��
*            ��ʱʱ��
*����ֵ��  ��ʵ�����壺
*******************************************************************************************/
static u8 serverReportSystemState(u16 delay)
{
#ifdef SERVER_REPORT_ENABLE
	u8 temp[100];
	u8 serverID;
	u16 length,cmpLen;
	u32 irRecTime = 0;
	u8 ret;
	u8 irdriver = 0;
	uint16_t reset;
	u8 JsonSuccess;
	static u8 Infraredtimer = 0;	
	
	IrReportTick +=delay;
	//������˸����ʾ���յ������ź�
	if(IrReportTick>300 && ledState ==1)
	{
		StateLed(0,LED_WL);
		ledState = 0;
	}
	SysState.globalState.IrRevTimes = 0;
    SysState.globalState.IrDevID = 0;
	if(IrReportTick >=1000)
	{
		IrReportTick=0;
		
		SysState.globalState.IrDevID = 1;//IrRxGetIrDevId(1);//Ŀǰֻ����һ���������
		SysState.globalState.IrRevTimes = 0;
//			SysState.globalState.IrRevStatus = IrRxGetIrRevStatus(1);
		SysState.globalState.IrRevTimes = IrRxGetIrRevTimes(1);
		IrRxClearIrInf(1);
		if(SysParam.globalParam.DeviceInfirredParam.InfirredRevEnable == EnableType_ENABLE)
		{	
			if(GetDeviceCanMsg(ID_IRRX,0,&irdriver))
			{
				GetCanDeviceAPPMsg(irdriver,ID_IRRX,0,0,&irRecTime);
				SetCanDeviceAPPMsg(irdriver,ID_IRRX,0,0,0);
				SysState.globalState.IrRevTimes+= irRecTime;
			}
		}	
	}
	
	if(getSeverStatus() == 0)
		return 1;
	
	if(RsetReasonTick < 60000)
		RsetReasonTick +=delay;
	if(RsetReasonTick >= 60000 && resetReasonflag == 1)
	{			
		resetReasonflag = 0;
		SysState.globalState.Resetreason = kzqTurnonReson;
		//��ȡ��һ�θ�λԭ��	
	}
	

	
	cmpLen = (u8 *)(&SysState.globalState.runMode)-(u8 *)(&SysState.globalState.emergencyState);
	ret = memcmp(&globalStateBak,&SysState.globalState,cmpLen);
	if(ret == 0)
	{
		if(SysState.globalState.IrRevTimes > 0)
		{
			Infraredtimer++;
			if(Infraredtimer > 20)
			{
				Infraredtimer = 0;
				if(ledState == 0)
				{
					StateLed(LED_WL,0);
					ledState = 1;
				}
			}
			else
			{
				return 0;
			}
		}
		else
		{
			return 0;
		}
		
	}
	else
	{
		if(SysState.globalState.IrRevTimes > 0)
		{
			if(ledState == 0)
			{
				StateLed(LED_WL,0);
				ledState = 1;
			}
		}
	}
	Infraredtimer = 0;
	
	memcpy(&globalStateBak,&SysState.globalState,cmpLen);
	memset(temp,0,sizeof(temp));
	
	temp[0] = REPORTENUM_SYSTEM;
	temp[1] = ENUM_SYSTEM;
	temp[2] = 0;
	length =cmpLen+2;
	memcpy(&temp[3],&length,2);
	memcpy(&temp[5],&SysState.globalState,length);
	
	serverID = getSysServerAddr();
	UdpAppProcCtrl(AppProcCtrl_stateReport_indication,temp,5+length,serverID);
	
	if(SysState.globalState.Resetreason != 0)   //jhy Reset Reason only report once
		SysState.globalState.Resetreason = 0;
		
#endif
	return 1;
}




u8 getPersonLockResumeTimer(void)
{
	return (5-UnlockTimer/1000);
}

void PersonCtrlAlarmProc(u8 state)
{
	u8 alar[7]= {0};
	alar[0] = state;
	alar[1] = 0x01;//frame->u8DT[4];	
	alar[2] = 0x00;
	alar[3] = 0xE2;
	alar[4] = 0x00;
	SysDataFun(DATA_MSG_CTRL,0x03,0x01,0x07,0,alar,0);	
}


void PersionLockSuspendCallBackTimer(uint16_t id)
{
	SysState.globalState.PersonLockState = 0;
	deleteTimerEvent(PersonLockStateTimerId);
}



//void zeroPersonLockPro(u8 localState)
//{
//	u8 i;
//	u8 actionFlag = 0;
//	if(localState)
//	{
//		for(i=0; i<SINGLEACTION_NUM_MAX;i++)
//		{
//			if(SysState.SigAction[i].RunState != RunStateType_IDLE)
//			{
//				actionFlag = 1;
//				break;
//			}
//		}
//		
//		for(i=0; i<COMBACTION_NUM_MAX;i++)
//		{
//			if(SysState.CombAction[i].RunState != RunStateType_IDLE)
//			{
//				actionFlag = 1;
//				break;
//			}
//		}
//		if(actionFlag)
//		{
//			zeroPersonLock[1].ZeroPersonLockState = 1;
//		}
//	}
//	else
//	{
//		zeroPersonLock[1].ZeroPersonLockState = 0;
//	}
//	
//}
/*******************************************************************************************
*������������Ա����������̺���
*��ڲ�����cmdȡֵ��
*            ActionCtrl_CmdType_STOP��   ����ֹͣ
*            ActionCtrl_CmdType_SUSPEND����������
*            ActionCtrl_CmdType_RESUME�� �����ָ�
*����ֵ��  �ޣ�
*******************************************************************************************/
static void PersonLockControllActionProc(u8 cmd)
{
	u8 temp[4] = {0};
	u8 i = 0;
	ParamUnit   LockActionID;
	ActionCtrlType type;
	//��ȡ��ǰ����ID״̬

	type.CtrlCmd = cmd;
	type.CtrlMode = ActionCtrl_ModeType_AUTO;
	type.CtrlSourceAddr = getSelfAddr();
	type.CtrlSourceType = ActionCtrl_SourceType_CONTROLLER;
	type.UserID = 0;
	
	for(i = 0; i<2;i++)
	{
		if(i == 0)
			LockActionID = SysParam.globalParam.DevicePeopleParam.LockActionID1;
		else
			LockActionID = SysParam.globalParam.DevicePeopleParam.LockActionID2;
		
		if(LockActionID == 0)
			continue;
		if(cmd == ActionCtrl_CmdType_STOP)
		{
			if(SysState.CombAction[LockActionID-1].RunState == RunStateType_STOP\
				 ||SysState.CombAction[LockActionID-1].RunState == RunStateType_IDLE)
			{
				continue;
			}
			else
			{
				PersonCtrlAlarmProc(1);
				CombActionCtrlFunc(type,LockActionID-1);
				if(personLockStateLast == 1)
				{
					SysState.globalState.PersonLockState = 1;
					PersonLockStateTimerId = addTimerEvent(5000,0X01,PersionLockSuspendCallBackTimer);//2000ms
					startTimerEvent(PersonLockStateTimerId);
				}
				else
				{
					SysState.globalState.PersonLockState = 0;
					WriteLog_API(LockActionID,LOG_TYPE_HANDLE_COMB,LOG_STATUS_PERSONLOCK_TIMEOUT,0,0);
					//����־  �Ƽ���Ա������ʱ
				}
			}
		}
		else if(cmd == ActionCtrl_CmdType_SUSPEND)//��ͣ
		{
			if(SysState.CombAction[LockActionID-1].RunState == RunStateType_STOP\
				 ||SysState.CombAction[LockActionID-1].RunState == RunStateType_IDLE\
				 ||SysState.CombAction[LockActionID-1].RunState == RunStateType_SUSPEND)
			{
				continue;
			}
			else
			{			
				UnlockTimer = 0;
				UnlockTimerFlag = 0;				
				CombActionCtrlFunc(type,LockActionID-1);
				personLockTimer = 1;
				SysState.globalState.PersonLockState = 1;
				PersonCtrlAlarmProc(1);
			}
		}
		else if(cmd == ActionCtrl_CmdType_RESUME)//�ָ�
		{
			if(SysState.CombAction[LockActionID-1].RunState == RunStateType_SUSPEND)
			{
				if(UnlockTimer)
				{
					PersonCtrlAlarmProc(1);
					CombActionCtrlFunc(type,LockActionID-1);
					SysState.globalState.PersonLockState = 0;
				}
				else
				{
					PersonCtrlAlarmProc(0);
					UnlockTimerFlag = 1;
					UnlockTimer = 0;
					SysState.globalState.PersonLockState = 2;
				}
			}
		}
	}
}

/*******************************************************************************************
*������������Ա�������ڴ���
*��ڲ�����delayȡֵ��
*            ��ʱʱ��50ms
*����ֵ��  ��ʵ�����壺
*******************************************************************************************/
static void sysDevicePersionLock(u16 delay)
{
	int i;
	u8  driver=0;
	u32 data;
	u8 personGrades = 0;
	u16 personDis = 0;
	u16 personNum = 0;
	ParamUnit  Persontmp;
	ParamUnit   LockActionID;

	u8 serverID;
	u16 length;	
		
	//��Ա��λδʹ��
	if(SysParam.globalParam.DevicePeopleParam.PeoplePosEnable == EnableType_DISABLE)
	{
		SysState.globalState.PersonLockState = 0;
		return;
	}
	//��Ա����δʹ��
	if(SysParam.globalParam.DevicePeopleParam.PeopleBisuoEnable == EnableType_DISABLE)
	{
		SysState.globalState.PersonLockState = 0;		
		return;
	}

//	//��ȡ��Ա��λ��������
//	if(GetDeviceCanMsg(ID_PERSON, 0, &driver) == 0)
//	{
//		SysState.globalState.PersonLockState = 0;
//		return;
//	}
	//��ͣ����ֹͣ����Ա��������
	if(SysState.globalState.bisuoState == 1 || SysState.globalState.hwBisuo == 1 \
		||SysState.globalState.hwEmergency == 1|| SysState.globalState.emergencyState == 1\
		||SysState.globalState.hwStop == 1|| SysState.globalState.stopState == 1)
	{
		SysState.globalState.PersonLockState = 0;
	}

	//5���Զ��ָ�	
	if(UnlockTimerFlag)
	{
		UnlockTimer+=delay;
	}	

	if(UnlockTimer>=5000 && UnlockTimerFlag)
	{
		PersonLockControllActionProc(ActionCtrl_CmdType_RESUME);//�ָ�
		UnlockTimer = 0;
		UnlockTimerFlag = 0;
		//�����־
	}
	
	//����������Ա��λ��Ϣ
	for(i=0;i<PERSON_MAX;i++)
	{
		GetCanDeviceAPPMsg(driver, ID_PERSON, i, PERSONMSG, &data); 
		personGrades = (ParamUnit)(data >> 12);
		GetCanDeviceAPPMsg(driver, ID_PERSON, i, PERSONDIS, &data);
		personDis = (ParamUnit)data;
		
		GetCanDeviceAPPMsg(driver, ID_PERSON, i, PERSONNUM, &data); 
		personNum = (ParamUnit)data;
		
		personLockState[i] = 0xFF;
		
		//��Ա��λ��СΪ301��300һ�²�����
		if(personNum<UWBPERSONIDMIN)
			continue;
		
		//��Ա������ڱ�������+������Χ
		if(personDis > SysParam.globalParam.DevicePeopleParam.BisuoRange + SysParam.globalParam.DevicePeopleParam.BisuoDifference)
		{
			personLockState[i] = 0xFF;
		}		
		else if(personDis <= SysParam.globalParam.DevicePeopleParam.BisuoRange)
			personLockState[i] = personGrades;
		else 
			personLockState[i] = 0xFF;
	}
	//Ĭ��ֵ������
	Persontmp = PERSON_GRADES_15;
	//�����Ա��λ�Ƿ��б���
	for(i=0;i<PERSON_MAX;i++)
	{
		//��ѯ���б�ǩ����С�ȼ�
		if(Persontmp>personLockState[i]/*&& personLockState[i] != 0*/)
			Persontmp = personLockState[i];
	}
	//Ĭ��ֵ���������ȼ�Ϊ0
	if(Persontmp >= PERSON_GRADES_3)
		Persontmp = PERSON_GRADES_3;
	else
		ClearPersonLockState = 0;
	
	if(Persontmp == PERSON_GRADES_0)
	{
//		if(localZeroPersonLock == 0)
		{
			zeroPersonLock[1].ZeroPersonLockState = 1;
//			zeroPersonLockPro(localZeroPersonLock);
		}
	}
	else
	{
		zeroPersonLock[1].ZeroPersonLockState = 0;
//		zeroPersonLockPro(localZeroPersonLock);
		switch(Persontmp)
		{				
				//��ǰ��1
			case PERSON_GRADES_1:
				{
					personLockTimer = 0;
					//�ϴβ�Ϊ1��ͣ����
	//				if(personLockStateLast != PERSON_GRADES_1)
					{
						personLockStateLast = Persontmp;
						PersonLockControllActionProc(ActionCtrl_CmdType_STOP);//ͣ����
					}
				}
				break;
				//��ǰ��2
				case PERSON_GRADES_2:
					{
						switch(personLockStateLast)
						{
							//�ϴ�Ϊ3��Ҫ��ͣ
							case PERSON_GRADES_3:
								//��ͣ	
								personLockTimer = 0;
								PersonLockControllActionProc(ActionCtrl_CmdType_SUSPEND);//��ͣ
								break;
							case PERSON_GRADES_1:
								personLockTimer = 0;
								break;
							case PERSON_GRADES_2:
								if(personLockTimer == 0)
									PersonLockControllActionProc(ActionCtrl_CmdType_SUSPEND);//��ͣ
								else if(personLockTimer < SysParam.globalParam.DevicePeopleParam.BisuoTime*100)
								{
									personLockTimer +=delay;
	//								PersonLockControllActionProc(ActionCtrl_CmdType_SUSPEND);//��ͣ
								}
								else if(personLockTimer >= SysParam.globalParam.DevicePeopleParam.BisuoTime*100)
								{
									personLockTimer = 0;
									PersonLockControllActionProc(ActionCtrl_CmdType_STOP);//ͣ����
								}
								break;
						}
					}
					break;
				case PERSON_GRADES_3:
					{
						personLockTimer = 0;
						switch(personLockStateLast)
						{
							case PERSON_GRADES_1:
								break;
							case PERSON_GRADES_2:
								PersonLockControllActionProc(ActionCtrl_CmdType_RESUME);//�ָ�
								break;
							case PERSON_GRADES_3:
								//��������ʱ��ǿ�ƽ���
								if(SysState.globalState.PersonLockState != 0)
									ClearPersonLockState += delay;
								if(SysState.globalState.PersonLockState == 0)
									ClearPersonLockState = 0;
								if(ClearPersonLockState>SysParam.globalParam.DevicePeopleParam.BisuoTime*100*2 && SysState.globalState.PersonLockState != 0)
								{
									if(zeroPersonLock[0].ZeroPersonLockState == 0 && \
									   zeroPersonLock[1].ZeroPersonLockState == 0 &&\
									   zeroPersonLock[2].ZeroPersonLockState == 0)
									{
										SysState.globalState.PersonLockState = 0;
										ClearPersonLockState = 0;
									}
								}
								break;
						}
					}
					break;
		}
	}
	personLockStateLast = Persontmp;
}



	


static void sysZeroPersonLockProc(u16 delay)
{
	u8 i = 0;
	u8 localLock = 0;
	static u8 localLockLast = 0;
	if(zeroPersonLock[1].LastZeroPersonLockState != zeroPersonLock[1].ZeroPersonLockState)
	{
		zeroPersonLock[1].LastZeroPersonLockState = zeroPersonLock[1].ZeroPersonLockState;
		netReportBisuoState(DevIOInputPointType_PERSON_BISUO,zeroPersonLock[1].ZeroPersonLockState);
	}
	for(i = 0;i<3;i++)
	{
		localLock |= zeroPersonLock[i].ZeroPersonLockState;
	}
	if(localLock != localLockLast)
	{
		if(localLock)
		{
			AllZeroPersonLockState = 1;
			SysState.globalState.PersonLockState = 1;
			SysAllActionStop(ActionCtrl_SourceType_CONTROLLER);
		}
		else
		{
			AllZeroPersonLockState = 0;
			SysState.globalState.PersonLockState = 0;
		}
	}
	localLockLast = localLock;
}

void setZeroPersonLockState(u8 num, u8 state)
{
	u8 i;
	for(i = 0;i<3;i++)
	{
		if(zeroPersonLock[i].ZeroPersonLockNum == num)
			zeroPersonLock[i].ZeroPersonLockState = state;
	}
}


/*******************************************************************************************
*����������uwb��Ա��λ�ϱ�
*��ڲ�����delayȡֵ��
*            ��ʱʱ��50ms
*����ֵ��  �ޣ�
*******************************************************************************************/
static void sysDevicePerson(u16 delay)
{
#ifdef SERVER_REPORT_ENABLE
	int i;
	u8  driver=0;
	u32 data;
	u8 temp[40];
	u8 serverID;
	u16 length;
	static u8 personReportTimes = 0;
	
	if(getSeverStatus() == 0)
		return;
	
	 if(GetDeviceCanMsg(ID_PERSON, 0, &driver) == 0)
		return;
		for(i=0;i<PERSON_MAX;i++)
		{
			GetCanDeviceAPPMsg(driver, ID_PERSON, i, PERSONBATTRY, &data); 
			if((data&0xff)&0x80)
			{
				GetCanDeviceAPPMsg(driver, ID_PERSON, i, PERSONNUM, &data); 
				SysState.Person[i].personNum = (ParamUnit)data;
				if(SysState.Person[i].personNum == 0)
					continue;
				GetCanDeviceAPPMsg(driver, ID_PERSON, i, PERSONMSG, &data); 
				SysState.Person[i].personPer = (ParamUnit)(data >> 12);
				SysState.Person[i].personStar = (ParamUnit)((data >> 8)&0x0f);
				GetCanDeviceAPPMsg(driver, ID_PERSON, i, PERSONBATTRY, &data); 
				SysState.Person[i].personStar |= ((data<<8)&0xff00);
				GetCanDeviceAPPMsg(driver, ID_PERSON, i, PERSONDIS, &data);
				SysState.Person[i].personDis = (ParamUnit)data;
				memset(temp,0,sizeof(temp));
			
				temp[0] = REPORTENUM_PERSON;
				temp[1] = ENUM_DEVICE;
				temp[2] = i+1;
				length =sizeof(personStateType);
				memcpy(&temp[3],&length,2);
				memcpy(&temp[5],&SysState.Person[i],length);
				serverID = getSysServerAddr();
				UdpAppProcCtrl(AppProcCtrl_stateReport_indication,temp,5+length,serverID);		
			}
		}
		
	devicePersonTick +=delay;
	if(devicePersonTick <200)
		return;

	devicePersonTick =0;
	personReportTimes++;

	if(GetDeviceCanMsg(ID_PERSON, 0, &driver) == 0)
		return;
	for(i=0;i<PERSON_MAX;i++)
	{
		GetCanDeviceAPPMsg(driver, ID_PERSON, i, PERSONNUM, &data); 
		SysState.Person[i].personNum = (ParamUnit)data;
	
		if(SysState.Person[i].personNum == 0)
			continue;
		if(personReportTimes <5 &&(SysState.Person[i].personNum>=UWBPERSONIDMIN))
			continue;
		
		GetCanDeviceAPPMsg(driver, ID_PERSON, i, PERSONMSG, &data); 
		SysState.Person[i].personPer = (ParamUnit)(data >> 12);
		SysState.Person[i].personStar = (ParamUnit)((data >> 8)&0x0f);
		GetCanDeviceAPPMsg(driver, ID_PERSON, i, PERSONBATTRY, &data); 
		SysState.Person[i].personStar |= ((data<<8)&0xff00);
		GetCanDeviceAPPMsg(driver, ID_PERSON, i, PERSONDIS, &data);
		SysState.Person[i].personDis = (ParamUnit)data;
		memset(temp,0,sizeof(temp));
		
		temp[0] = REPORTENUM_PERSON;
		temp[1] = ENUM_DEVICE;
		temp[2] = i+1;
		length =sizeof(personStateType);
		memcpy(&temp[3],&length,2);
		memcpy(&temp[5],&SysState.Person[i],length);
		
		serverID = getSysServerAddr();
		
		UdpAppProcCtrl(AppProcCtrl_stateReport_indication,temp,5+length,serverID);		
	}
	if(personReportTimes>=5)
		personReportTimes = 0;
	
#endif
}

/*******************************************************************************************
*����������ϵͳ�豸�ϱ�
*��ڲ�����delayȡֵ��
*            ��ʱʱ��50ms
*����ֵ��  �ޣ�
*******************************************************************************************/
static void sysDeviceMng(u16 delay)
{
	int i,j;
	u8  ret;
	u8  deviceNum=0;
	u32 data;
	
	deviceTick +=delay;
	if(deviceTick <2000)   //2000   jhy  220625  more data test
		return;
	
	deviceTick =0;
	
	for(i=0; i<2; i++)
	{
		ret = setDeviceMsg(deviceNum,i,ID_EMVD, 0);
		if(ret)
		{
			deviceNum++;
			break;
		}
	}
	for(i=0; i<2; i++)
	{		
		ret = setDeviceMsg(deviceNum,i,ID_ALARM, 0);
		if(ret)
		{
			deviceNum++;
			break;
		}
	}	
	for(i=0; i<2; i++)
	{		
		ret = setDeviceMsg(deviceNum,i,ID_PERSON, 0);
		if(ret)
		{
			GetCanDeviceAPPMsg(i, ID_PERSON, 0, PERSONNUM, &data); //point:PERSONNUM
			SysState.Device[deviceNum].deviceValue = (ParamUnit)data;
			deviceNum++;
			break;
		}
	}
	for(i=0; i<2; i++)
	{	
		ret = setDeviceMsg(deviceNum,i,ID_IRRX, 0);
		if(ret)
		{
			GetCanDeviceAPPMsg(i, ID_IRRX, 0, 0, &data); 
			SysState.Device[deviceNum].deviceValue = (ParamUnit)data;
			deviceNum++;
			break;
		}		
	}
	
	for(i=deviceNum;i<SYSDEVICE_NUM;i++)
	{
		memset(&SysState.Device[deviceNum].driverEnable ,0,sizeof(deviceStateType));		
	}
	
	for(i=0;i<deviceNum;i++)
		serverReportDevice(i);
}


/*******************************************************************************************
*����������ϵͳ��ͣ�ϱ�
*��ڲ�������
*����ֵ��  �ޣ�
*******************************************************************************************/
void SysEmergencyStateUpdate(void)
{
	int i;
	u8  flag=0;
	u8  hostaddr;
	
	for(i =0;i<256;i++)
	{
		if(SysStopState[i].SYSStop.EMERGENCY == SYSSTOP_PRESS)
		{
			flag=1;
			break;
		}			
	}
	
	if(flag)
		SysState.globalState.emergencyState = SYSSTOP_PRESS;
	else
		SysState.globalState.emergencyState = SYSSTOP_UNPRESS;
	
//	hostaddr = getSelfAddr();
//	
//	SysStopState[hostaddr].SYSStop.EMERGENCY = SysState.globalState.emergencyState;
}

static void SysAllActionStop(u8 src)
{
	int i;
	
	for(i=0; i<SINGLEACTION_NUM_MAX;i++)
	{
		SigActionAllStop(i);
	}
	
	for(i=0; i<COMBACTION_NUM_MAX;i++)
	{
		CombActionAllStop(i);
	}

	for(i=0; i<GROUPACTION_NUM_MAX;i++)
	{
		groupActionAllStop(i);
	}	
	if(src == ActionCtrl_SourceType_CONTROLLER)
		AlarmAllStop();
	ActionDataInit();
	SigActionTrggerTimerInit();
}

static void netReportBisuoState(u8 stateType,u8 state)
{
	u8 temp[5];
	
	memset(temp,0,sizeof(temp));

	if(stateType == DevIOInputPointType_EMERGENCY)
	{	
		temp[0] = stateType;
		temp[1] = ENUM_SYSTEM;
		temp[2] = state;
		
		UdpAppProcCtrl(AppProcCtrl_stateReport_indication,temp,5,UDP_LINK_BROADCAST_ADDR);
		UdpAppProcCtrl(AppProcCtrl_stateReport_indication,temp,5,UDP_LINK_BROADCAST_ADDR);
		if(SysParam.globalParam.DeviceWirelessParam.WirelessEnable == EnableType_ENABLE)
		    SendWirelessBusData(state,getSelfAddr());
	}
	else if(stateType == DevIOInputPointType_BISUO)
	{	
		temp[0] = stateType;
		temp[1] = ENUM_SYSTEM;
		temp[2] = state;
		
		
		UdpAppProcCtrl(AppProcCtrl_stateReport_indication,temp,5,UDP_LINK_BROADCAST_ADDR);
		UdpAppProcCtrl(AppProcCtrl_stateReport_indication,temp,5,UDP_LINK_BROADCAST_ADDR);
		
//		if(SysParam.globalParam.SupportParam.currentSupportID <SysParam.globalParam.SupportParam.MaxSupportID)
//			UdpAppProcCtrl(AppProcCtrl_stateReport_indication,temp,5,SysParam.globalParam.SupportParam.currentSupportID+1);
//		
//		if(SysParam.globalParam.SupportParam.currentSupportID >SysParam.globalParam.SupportParam.MinSupportID)
//			UdpAppProcCtrl(AppProcCtrl_stateReport_indication,temp,5,SysParam.globalParam.SupportParam.currentSupportID-1);
	}
	else if(stateType == DevIOInputPointType_STOP)
	{
		temp[0] = stateType;
		temp[1] = ENUM_SYSTEM;
		temp[2] = state;
		
		UdpAppProcCtrl(AppProcCtrl_stateReport_indication,temp,5,UDP_LINK_BROADCAST_ADDR);
	}
	else if(stateType == DevIOInputPointType_PERSON_BISUO)
	{
		temp[0] = stateType;
		temp[1] = ENUM_SYSTEM;
		temp[2] = state;
		
		if(SysParam.globalParam.SupportParam.currentSupportID <SysParam.globalParam.SupportParam.MaxSupportID)
			UdpAppProcCtrl(AppProcCtrl_stateReport_indication,temp,5,SysParam.globalParam.SupportParam.currentSupportID+1);
		
		if(SysParam.globalParam.SupportParam.currentSupportID >SysParam.globalParam.SupportParam.MinSupportID)
			UdpAppProcCtrl(AppProcCtrl_stateReport_indication,temp,5,SysParam.globalParam.SupportParam.currentSupportID-1);
		
	}
}

static void SysStopProcess(u16 delay)
{
	u8  hostaddr,flag,min,max,i,state;
	
	hostaddr = getSelfAddr();
	
	if(SysState.globalState.hwStop == SYSSTOP_PRESS)
	{
		hwstopTick += delay;
		if(hwstopTick > 2000)
		{
			SysState.globalState.hwStop = SYSSTOP_UNPRESS;
			hwstopTick=0;
		}
	}
	
	if(SysStopState[hostaddr].SYSStop.STOP != SysState.globalState.hwStop)
	{
		netReportBisuoState(DevIOInputPointType_STOP,SysState.globalState.hwStop);
		
		SysStopState[hostaddr].SYSStop.STOP = SysState.globalState.hwStop;		
	}
	
	if((hostaddr+ SysParam.globalParam.SupportParam.StopScope )<=SysParam.globalParam.SupportParam.MaxSupportID)
		max = hostaddr+ SysParam.globalParam.SupportParam.StopScope;
	else
		max = SysParam.globalParam.SupportParam.MaxSupportID;
	
	if((SysParam.globalParam.SupportParam.MinSupportID+SysParam.globalParam.SupportParam.StopScope)<=hostaddr)
		min = hostaddr- SysParam.globalParam.SupportParam.StopScope ;
	else
		min = SysParam.globalParam.SupportParam.MinSupportID;
	
	flag=0;
	for(i=min;i<=max;i++)
	{
		flag |=SysStopState[i].SYSStop.STOP;
	}
	flag |= SysStopState[getSelfAddr()].SYSStop.STOP;   //stop mode need add self state  jhy 20220823
	
	if(SysState.globalState.stopState !=flag)
	{
		SysState.globalState.stopState =flag;
		if(SysState.globalState.stopState ==SYSSTOP_PRESS)
		{
			SysAllActionStop(SysState.globalState.stopSrc);
			state = BaseCtrlType_STARTUP;
			write_data (SUPPORT_DEFAULT_ADDR,DRIVER_IO,0,0,DevIOOutputPointType_BEEP_END_PRESSED,&state);			
		}
	}	
}

static u8 Bisuocount =0;
static void SysBisuoProcess(void)
{
	u16 value;
	u8  hostaddr,state,flag;
	
	Bisuocount ++;
	if(Bisuocount >2)
		Bisuocount = 0;
	else 
		return;
	read_data (SUPPORT_DEFAULT_ADDR,DRIVER_IO,0,0,DevIOInputPointType_BISUO,&value);
	if(value == SYSSTOP_PRESS)
	{
		if(SysState.globalState.hwBisuo != SYSSTOP_PRESS)
		{
//			reinit_551_br();
//			bttest_eth();
//			OSTimeDly(2000);
//			reinit_port4();
//			SendFile_API(FILETYPE,"��ͣ.png",0,UPDATERIGHT);
//			OSTimeDly(4000);
//			SendFile_API(FILETYPE,"������5.png",0,UPDATEALL);
//			OSTimeDly(4000);
//			SendFile_API(FILETYPE,"��������ͼ��.png",0,UPDATERIGHT);
//			OSTimeDly(4000);
//			SendFile_API(FILETYPE,"������װ��3.png",0,UPDATERIGHT);
//			OSTimeDly(4000);
//			SendFile_API(FILETYPE,"��ú����1.png",0,UPDATERIGHT);
//			OSTimeDly(4000);
//			SendFile_API(FILETYPE,"��ú����2.png",0,UPDATERIGHT);
//			OSTimeDly(4000);
//			SendFile_API(FILETYPE,"��ú����1.png",0,UPDATERIGHT);
//			OSTimeDly(4000);
//			SendFile_API(FILETYPE,"��ú����2.png",0,UPDATERIGHT);
//			OSTimeDly(4000);
			
			
		}
		state = SYSSTOP_PRESS;
		SysState.globalState.hwBisuo = SYSSTOP_PRESS;
	}
	else if(value == SYSSTOP_UNPRESS)
	{
		state = SYSSTOP_UNPRESS;
		SysState.globalState.hwBisuo = SYSSTOP_UNPRESS;
	}
	
	hostaddr = getSelfAddr();
	
	if(SysStopState[hostaddr].SYSStop.BISUO != state)
	{
		netReportBisuoState(DevIOInputPointType_BISUO,state);
		
		SysStopState[hostaddr].SYSStop.BISUO = state;
		
	}
	
	flag=0;
	if(SysParam.globalParam.SupportParam.currentSupportID <SysParam.globalParam.SupportParam.MaxSupportID)
		flag |=SysStopState[hostaddr+1].SYSStop.BISUO;
	if(SysParam.globalParam.SupportParam.currentSupportID >SysParam.globalParam.SupportParam.MinSupportID)
		flag |=SysStopState[hostaddr-1].SYSStop.BISUO;
	flag |=SysStopState[hostaddr].SYSStop.BISUO;
	
	//ghy ��������ܱ�����ش����߼�
	if(SysParam.globalParam.SupportParam.DangganEnable)
	{
		if((getSelfAddr() >= SysParam.globalParam.SupportParam.MinBisuoID) && \
		   (getSelfAddr() <= SysParam.globalParam.SupportParam.MaxBisuoID))
		{
			flag |= SysStopState[BISUO_SUPPORT_NUM].SYSStop.BISUO;
		}
	}
	
	if(SysState.globalState.bisuoState !=flag)
	{
		SysState.globalState.bisuoState =flag;
		if(SysState.globalState.bisuoState ==SYSSTOP_PRESS)
		{
			SysAllActionStop(ActionCtrl_SourceType_CONTROLLER);
			state = BaseCtrlType_STARTUP;
			write_data (SUPPORT_DEFAULT_ADDR,DRIVER_IO,0,0,DevIOOutputPointType_BEEP_LOCKED,&state);
			WriteLog_API(0,LOG_TYPE_SYSTEM_ALARM,LOG_STATUS_BISUO_ALARM,0,0);
		}
		else
		{
			state = BaseCtrlType_STARTUP;
			write_data (SUPPORT_DEFAULT_ADDR,DRIVER_IO,0,0,DevIOOutputPointType_BEEP_UNLOCKED,&state);	
			WriteLog_API(0,LOG_TYPE_SYSTEM_ALARM,LOG_STATUS_BISUO_NORMAL,0,0);
		}		
	}	
}

static u8 Emergencycount = 0;
static void SysEmergencyProcess(void)
{
	u16 value;
	u8  hostaddr,state,emergency;
	
	Emergencycount ++;
	if(Emergencycount >2) 
		Emergencycount = 0;
	else
		return;		
		
	read_data (SUPPORT_DEFAULT_ADDR,DRIVER_IO,0,0,DevIOInputPointType_EMERGENCY,&value);
	if(value == SYSSTOP_PRESS)
	{
		SysState.globalState.hwEmergency = SYSSTOP_PRESS;
	}
	else if(value == SYSSTOP_UNPRESS)
	{		
		SysState.globalState.hwEmergency = SYSSTOP_UNPRESS;
	}
	
	hostaddr = getSelfAddr();
	
	if(SysStopState[hostaddr].SYSStop.HWEMERGENCY != SysState.globalState.hwEmergency)
	{
		netReportBisuoState(DevIOInputPointType_EMERGENCY,SysState.globalState.hwEmergency);
		SysStopState[hostaddr].SYSStop.HWEMERGENCY = SysState.globalState.hwEmergency;
		SysStopState[hostaddr].SYSStop.EMERGENCY = SysState.globalState.hwEmergency;
		SysEmergencyStateUpdate();
	}
	
	if(emergencyState != SysState.globalState.emergencyState)
	{	
		if(SysState.globalState.emergencyState ==SYSSTOP_PRESS)
		{
			SysAllActionStop(ActionCtrl_SourceType_CONTROLLER);
			state = BaseCtrlType_STARTUP;
			write_data (SUPPORT_DEFAULT_ADDR,DRIVER_IO,0,0,DevIOOutputPointType_BEEP_LOCKED,&state);	
			WriteLog_API(0,LOG_TYPE_SYSTEM_ALARM,LOG_STATUS_EMERGENCY_ALARM,0,0);
		}
		else
		{
			state = BaseCtrlType_STARTUP;
			write_data (SUPPORT_DEFAULT_ADDR,DRIVER_IO,0,0,DevIOOutputPointType_BEEP_UNLOCKED,&state);
			WriteLog_API(0,LOG_TYPE_SYSTEM_ALARM,LOG_STATUS_EMERGENCY_NORMAL,0,0);
		}
		emergencyState = SysState.globalState.emergencyState;
	}
}

static void SysB12VProcess(void)
{
	u8 flag,state,hostaddr;
	
	hostaddr = getSelfAddr();
	flag=0;
	if(SysParam.globalParam.SupportParam.currentSupportID <SysParam.globalParam.SupportParam.MaxSupportID)
	{
		flag |=SysStopState[hostaddr+1].SYSStop.BISUO;
		flag |=SysStopState[hostaddr+1].SYSStop.EMERGENCY;
	}
	if(SysParam.globalParam.SupportParam.currentSupportID >SysParam.globalParam.SupportParam.MinSupportID)
	{
		flag |=SysStopState[hostaddr-1].SYSStop.BISUO;
		flag |=SysStopState[hostaddr-1].SYSStop.EMERGENCY;
	}
	flag |=SysStopState[hostaddr].SYSStop.BISUO;
	flag |=SysStopState[hostaddr].SYSStop.EMERGENCY;
	
	
    //ghy ��������֧�ܶ�B12V�����߼�
	if(SysParam.globalParam.SupportParam.DangganEnable)
	{
		if((getSelfAddr() >= SysParam.globalParam.SupportParam.MinBisuoID) && \
		   (getSelfAddr() <= SysParam.globalParam.SupportParam.MaxBisuoID))
		{
			flag |= SysStopState[BISUO_SUPPORT_NUM].SYSStop.BISUO;
		}
	}
	
	if(SysState.globalState.B12VState !=flag)
	{
		SysState.globalState.B12VState =flag;
		if(SysState.globalState.B12VState ==SYSSTOP_PRESS)
		{
			state = BaseCtrlType_STOP;
			write_data (SUPPORT_DEFAULT_ADDR,DRIVER_IO,0,0,DevIOOutputPointType_B12V,&state);			
		}
		else
		{
			state = BaseCtrlType_STARTUP;
			write_data (SUPPORT_DEFAULT_ADDR,DRIVER_IO,0,0,DevIOOutputPointType_B12V,&state);		
		}		
	}	
	
}

static void SysFailProcess(void)
{
	u8  versionflag,paraflag,min,max,i,state;
	static u8 lastparaflag = 0;	
	
	max = SysParam.globalParam.SupportParam.MaxSupportID;
	min = SysParam.globalParam.SupportParam.MinSupportID;

	versionflag = 0;
	paraflag = 0;
	for(i=min;i<=max;i++)
	{
		if(i == getSelfAddr())
			continue;
		if(i == (getSelfAddr()-1))
			continue;
		if(i == (getSelfAddr()+1))
			continue;
		versionflag |=SysStopState[i].SYSStop.VERSION;
		paraflag |= SysStopState[i].SYSStop.OVERALLPARA;
	}
	
	if(SysState.globalState.globalVersionState !=versionflag)
	{
		SysState.globalState.globalVersionState = versionflag;
		if(versionflag==0)
		{
			WriteFault_API(FAULT_HOST_OTHER,0,FAULT_EVENT_PARGRAM_NOT_SAME,FAULT_STATUS_NORMAL);				
		}
		else
		{
			WriteFault_API(FAULT_HOST_OTHER,0,FAULT_EVENT_PARGRAM_NOT_SAME,FAULT_STATUS_FAULT);
		}
	}
	if(lastparaflag != paraflag)
	{
		lastparaflag = paraflag;

		if(paraflag == 0)
		{
			WriteFault_API(FAULT_HOST_OTHER,0,FAULT_EVENT_ALL_PARA_NOT_SAME,FAULT_STATUS_NORMAL);				
		}
		else
		{
			WriteFault_API(FAULT_HOST_OTHER,0,FAULT_EVENT_ALL_PARA_NOT_SAME,FAULT_STATUS_FAULT);
		}
	}
}


void NetworkHeartbeatTimesZero(void)
{
	networkhearttimes = 0;
	if(SysState.globalState.ServerState == OnlineType_OFFLINE)
	{
		WriteFault_API(FAULT_HOST_OTHER,0,FAULT_EVENT_ABNORMAL_COMMUNICATION_WITH_SERVER,FAULT_STATUS_NORMAL);
		SysState.globalState.ServerState = OnlineType_ONLINE;
	}
}	
void NetworkHeartbeatTimesOut(u16 delay)
{
	if(SysParam.globalParam.SupportParam.ServerEnable != EnableType_ENABLE) //ghy ��������ֹ
	{
		if(SysState.globalState.ServerState == OnlineType_ONLINE)
		{
			WriteFault_API(FAULT_HOST_OTHER,0,FAULT_EVENT_ABNORMAL_COMMUNICATION_WITH_SERVER,FAULT_STATUS_NORMAL);
			SysState.globalState.ServerState = OnlineType_OFFLINE;
		}
	}
	else if(SysParam.globalParam.SupportParam.ServerEnable == EnableType_ENABLE)//ghy ������ʹ��
	{
		//����������ʱ�����κδ���ֻд��һ��������Ϣ		
		if(SysState.globalState.ServerState == OnlineType_ONLINE)
		{
			networkhearttimes += delay;
			if(networkhearttimes > 16000)
			{
				WriteFault_API(FAULT_HOST_OTHER,0,FAULT_EVENT_ABNORMAL_COMMUNICATION_WITH_SERVER,FAULT_STATUS_FAULT);
				SysState.globalState.ServerState = OnlineType_OFFLINE;
			}
		}
	}
}



void reportManualSingleAction(u16 delay)
{
	u8 i;
	u8 sensorId;
	u8 temp[20];
	u8 length;
	SensorStateType value;
	
	if(SysState.globalState.slaveMode != SYSMODE_SLAVE_CTRL)
	{
		ManualActionSupportNum = 0;
		return;
	}
	if(ManualActionSupportNum == getSysServerAddr())
		return;
	
	for(i=0;i<MANUALACTIONINFO_LEN;i++)
	{
		if(ManualActionClear[i] != 0)
		{
			sensorId = getPartInSingleActionOfSensorId(ManualActionClear[i]);
			if(sensorId!= 0)
			{
				getSensorState(sensorId-1,&value);
				memset(temp,0,sizeof(temp));
	
				temp[0] = REPORTENUM_SENSOR;
				temp[1] = ENUM_SENSOR;
				temp[2] = sensorId;
				length =sizeof(SensorStateType);
				memcpy(&temp[3],&length,2);
				memcpy(&temp[5],&value,length);
				
				UdpAppProcCtrl(AppProcCtrl_stateReport_indication,temp,5+length,ManualActionSupportNum);
			}
		}
	}
}

/*********************************************************
** ��������SysFuncMngTask
** �䡡�룺None
** �䡡����None
** ����������������ϵͳ��������
************************************************************/
//extern u16_t getTestMemValue(void);
//u16_t TestMemValue=0;

u16 SysInitTimer = 0;
u8  SysInitFlag = 0;
void SysFuncMngTask(u16 delay)
{	
	int i;
	u8	err,len;
	u8  manualActionID[MANUALACTIONINFO_LEN];
	
	if(SysInitFlag == 0)
		SysInitTimer += delay;
	if(SysInitTimer >16500)
		SysInitFlag = 1;

	err = PLATFORM_OS_SemWait(&SYSMutex, 0);
		
	if(SysState.globalState.slaveMode ==SYSMODE_CONFIG)
	{
		PLATFORM_OS_SemPost(&SYSMutex);
		return;	
	}
	serverReportSystemState(delay);
	NetworkHeartbeatTimesOut(delay);
	if(SysInitFlag)
	{
		SysEmergencyProcess();
		SysBisuoProcess();
		sysDeviceMng(delay);
	}
	SysB12VProcess();
	SysStopProcess(delay);

	sysDevicePersionLock(delay);
	sysZeroPersonLockProc(delay);
	sysDevicePerson(delay);
	SysFailProcess();
	
	AlarmMng(delay);	
	TiggerDelayCycle(delay);
	reportManualSingleAction(delay);
	
	for(i=0; i<SINGLEACTION_NUM_MAX;i++)
	{
		SigActionStepProcess(delay,i);
	}
	
	for(i=0; i<COMBACTION_NUM_MAX;i++)
	{
		CombActionStepProcess(delay,i);
	}

	for(i=0; i<GROUPACTION_NUM_MAX;i++)
	{
		groupActionStepProcess(delay,i);
	}	
	
	for(i=0; i<PART_NUM_MAX;i++)
	{
		partCycleProcess(i,delay);
	}	
	
	if(getManualActionInfoBufSzie() >0)
	{
		len = CheckManualActionList(manualActionID);
		if(len)
		{		
			UdpAppProcCtrl(AppProcCtrl_manualControl_request,manualActionID,len,manualCtrlAddr);			
		}			
	}
	
//	TestMemValue = getTestMemValue();
	
	PLATFORM_OS_SemPost(&SYSMutex);
}

/*********************************************************
** ��������SysCtrlMngTask
** �䡡�룺None
** �䡡����None
** ����������������ϵͳ���ܿ����·�����
************************************************************/
static void SysCtrlBufInit(void)
{
	int i;
	
	memset(ctrlInfoMng.buf, 0 ,sizeof(ctrlInfoMng.buf));
	ctrlInfoMng.Cnt=0;
	ctrlInfoMng.pHead = &ctrlInfoMng.buf[0];
	ctrlInfoMng.pEnd  = &ctrlInfoMng.buf[0];
	for(i=0 ; i<CTRL_BUF_LEN - 1; i++)
	{
		ctrlInfoMng.buf[i].pNext = &ctrlInfoMng.buf[i+1];
	}	
	ctrlInfoMng.buf[CTRL_BUF_LEN-1].pNext = &ctrlInfoMng.buf[0];
}

void InsCtrlBufQueue(SysCtrlInfoType *pdata)
{	
	if(ctrlInfoMng.Cnt <CTRL_BUF_LEN)
	{
		memcpy(ctrlInfoMng.pHead,pdata,sizeof(SysCtrlInfoType)-4);
		ctrlInfoMng.pHead = ctrlInfoMng.pHead->pNext;
		ctrlInfoMng.Cnt++;
	}
}

void DelCtrlBufQueue(void)
{
	if(ctrlInfoMng.Cnt)
	{	
		ctrlInfoMng.Cnt --;
		ctrlInfoMng.pEnd = ctrlInfoMng.pEnd->pNext;	
	}
}

static u8 IsEmergencyStopAction(SysCtrlInfoType *pctrl)
{
	u8 leftid=0;
	u8 rightid=0;
	u8 localid=0;
	
	if((pctrl->actionType ==ENUM_COMBINATION_ACTION)||(pctrl->actionType ==ENUM_GROUP_ACTION)
	  ||((pctrl->actionType ==ENUM_SINGLE_ACTION)&&(pctrl->ctrl.CtrlMode==ActionCtrl_ModeType_AUTO)))
	{
		if(SysState.globalState.emergencyState == SYSSTOP_PRESS)
			return 1;
		else
			return 0;
	}
	else
	{	
		if(SysParam.globalParam.SupportParam.currentSupportID <SysParam.globalParam.SupportParam.MaxSupportID)
			rightid =SysParam.globalParam.SupportParam.currentSupportID+1;
			
		if(SysParam.globalParam.SupportParam.currentSupportID >SysParam.globalParam.SupportParam.MinSupportID)
			leftid =SysParam.globalParam.SupportParam.currentSupportID-1;
		
		localid = SysParam.globalParam.SupportParam.currentSupportID;
		if((leftid!=0)&&(SysStopState[leftid].SYSStop.EMERGENCY == SYSSTOP_PRESS))
			return 1;
		if((rightid!=0)&&(SysStopState[rightid].SYSStop.EMERGENCY == SYSSTOP_PRESS))
			return 1;		
		if((localid!=0)&&(SysStopState[localid].SYSStop.EMERGENCY == SYSSTOP_PRESS))
			return 1;		
		
		return 0;
	}
}

uint8_t getSysInitState(void)
{
	return sysInitFlag;
}


void SysCtrlMngTask(void)
{	
	u8	err,ret,temp[2];
	SysCtrlInfoType *pctrl;
	u16 mutexAction[2];
	u8 alarmMode[2];	
	int i;
	SysCtrlBufInit();
	
	sysInitFlag = 1;
	
	while(1)
	{	
		err = PLATFORM_OS_SemWait(&SYSCtrlSem, 0);
		if (err == DEF_OK)
		{
			PLATFORM_OS_SemWait(&SYSMutex, 0);
			
			if(ctrlInfoMng.Cnt)
			{
				pctrl =ctrlInfoMng.pEnd;
				if((pctrl->actionID !=0)&&(SysState.globalState.slaveMode !=SYSMODE_CONFIG)
					&&(IsEmergencyStopAction(pctrl)==0)
				    &&(SysState.globalState.bisuoState != SYSSTOP_PRESS)
				    &&(SysState.globalState.stopState != SYSSTOP_PRESS))
				{
					if((SysParam.globalParam.SupportParam.ManualCtrlFirst == EnableType_ENABLE && \
						(SysState.globalState.slaveMode ==SYSMODE_MASTER_CTRL ||\
						(SysState.globalState.slaveMode == SYSMODE_SLAVE_CTRL && pctrl->ctrl.CtrlSourceType == ActionCtrl_SourceType_SERVER)))
					     ||(AllZeroPersonLockState == 1))
					{	
						;
					}
					else
					{
						if(pctrl->actionType ==ENUM_SINGLE_ACTION)
						{
							ret = SigActionPerformJudge(pctrl->ctrl,pctrl->actionID-1,mutexAction);
							if(ret)
							{
								if(pctrl->actionType == ENUM_SINGLE_ACTION && pctrl->ctrl.CtrlMode ==ActionCtrl_ModeType_MANUAL && SysParam.globalParam.SupportParam.ManualCtrlFirst == EnableType_ENABLE)
								{
									for(i=0; i<SINGLEACTION_NUM_MAX;i++)
									{
										AutoSigActionAllStop(i);
									}
									
									for(i=0; i<COMBACTION_NUM_MAX;i++)
									{
										CombActionAllStop(i);
									}

									for(i=0; i<GROUPACTION_NUM_MAX;i++)
									{
										groupActionAllStop(i);
									}	
								}
								SigActionRunStateJudge(pctrl->ctrl,pctrl->actionID-1);
		//						SigActionModuleLoad(pctrl->actionID-1,&SysParam.SigAction[pctrl->actionID-1],0);
								SigActionCtrlFunc(pctrl->ctrl,pctrl->actionID-1,0);
							}
							else
								WriteLog_API(pctrl->actionID,LOG_TYPE_HANDLE_SINGLE,LOG_STATUS_DISABLE,0,0);
						}
						else if(pctrl->actionType ==ENUM_COMBINATION_ACTION)
						{
							ret= CombActionPerformJudge(pctrl->ctrl,pctrl->actionID-1);
							if(ret)
							{
								ret= checkCombActionPriorityState(pctrl->actionID-1);
								if(ret)
									CombActionCtrlFunc(pctrl->ctrl,pctrl->actionID-1);
							}
							else
								WriteLog_API(pctrl->actionID,LOG_TYPE_HANDLE_COMB,LOG_STATUS_DISABLE,0,0);
						}
						else if(pctrl->actionType ==ENUM_GROUP_ACTION)
						{
							groupActionCtrlFunc(pctrl->ctrl,pctrl->actionID-1,pctrl->ParamBuf,pctrl->ParamLength);
						}
						else if(pctrl->actionType ==ENUM_GROUP_ACTION_KZQ)
						{
							groupActionJudge(pctrl->ctrl,pctrl->actionID-1);
						}
						else if(pctrl->actionType ==ENUM_ALARM)
						{
//							if(pctrl->ctrl.CtrlSourceType != ActionCtrl_SourceType_SERVER)
//							{
//								if(pctrl->ctrl.CtrlCmd == ActionCtrl_CmdType_STARTUP)
//									SysState.globalState.slaveMode = SYSMODE_SLAVE_CTRL;
////								else
////									SysState.globalState.slaveMode = SYSMODE_NORMAL;
//							}
							if(pctrl->ParamLength >=3)
							{
								memcpy(&mutexAction[0],&(pctrl->ParamBuf[1]),2);
								temp[1] = pctrl->ParamBuf[0];
							}
							else
							{
								temp[1]=AlarmType_SoundLight;
								mutexAction[0]=SINGLEACTION_ALARM_ALWAY;
							}
							
							alarmMode[0] = SINGLEACTION_ALARM_MODE_SOUND_DEFAULT;
							alarmMode[1] = SINGLEACTION_ALARM_MODE_LIGHT_DEFAULT;
							if(pctrl->ctrl.CtrlCmd == ActionCtrl_CmdType_STARTUP)
							{
								temp[0] = BaseCtrlType_STARTUP;
								write_data (SUPPORT_DEFAULT_ADDR,DRIVER_IO,0,0,DevIOOutputPointType_BEEP_NEAR_SELECTED,&temp[0]);
							}
							else
							{
								temp[0] = BaseCtrlType_STOP;
								write_data (SUPPORT_DEFAULT_ADDR,DRIVER_IO,0,0,DevIOOutputPointType_BEEP_NEAR_SELECTED,&temp[0]);
							}
							CtrlAlarm(temp[1],mutexAction[0],temp[0],alarmMode,ENUM_ALARM,pctrl->ctrl.CtrlSourceAddr,ALARM_MODE_1);
						}					
					}
				}
				DelCtrlBufQueue();
			}
			
			PLATFORM_OS_SemPost(&SYSMutex);
		}			
	}
}

int SysNetCtrlFun(u8 addr,u8 Type,u8 Id,u8 cmd,u8 mode,u16 Length,u8 *pdata)
{
	u8 temp[12];
	
	memset(temp,0,sizeof(temp));
	
	if((Type >ENUM_GROUP_ACTION)||(cmd >=ActionCtrl_CmdTypeMAX)||(mode > ActionCtrl_ModeType_AUTO))
		return 0;
	
	switch(Type)
	{
		case ENUM_SINGLE_ACTION:
		{
			if(Id >SINGLEACTION_NUM_MAX)
				break;
			
			temp[0] = Type;
			temp[1] = Id;
			temp[2] = cmd;
			temp[3] = mode;
			temp[4] = 0;
			temp[5] = getSelfAddr();
			temp[6] = ActionCtrl_SourceType_CONTROLLER;
			
			if(mode == ActionCtrl_ModeType_MANUAL)
			{
				manualCtrlAddr =addr;
				if(cmd == ActionCtrl_CmdType_STARTUP)
					AddManualActionInfo(ENUM_SINGLE_ACTION,Id,RunStateType_RUN_MANUAL);
				else
					AddManualActionInfo(ENUM_SINGLE_ACTION,Id,RunStateType_IDLE);
			}
			
			UdpAppProcCtrl(AppProcCtrl_control_request,temp,9,addr);
		}
			break;
		
		case ENUM_COMBINATION_ACTION:
		{
			if(Id >COMBACTION_NUM_MAX)
				break;
			
			temp[0] = Type;
			temp[1] = Id;
			temp[2] = cmd;
			temp[3] = mode;
			temp[4] = 0;
			temp[5] = getSelfAddr();
			temp[6] = ActionCtrl_SourceType_CONTROLLER;
			
			UdpAppProcCtrl(AppProcCtrl_control_request,temp,9,addr);
		}			
			break;
		
		case ENUM_GROUP_ACTION:
		{
			if((Id >GROUPACTION_NUM_MAX)||(Length !=4)||(pdata ==NULL))
				break;
			
			temp[0] = Type;
			temp[1] = Id;
			temp[2] = cmd;
			temp[3] = mode;
			temp[4] = 3;
			temp[5] = getSelfAddr();
			temp[6] = ActionCtrl_SourceType_CONTROLLER;
			temp[9] = pdata[0];
			temp[10] = pdata[1];
			temp[11] = (pdata[2]>1)? 0:pdata[2];
			
			if(temp[5] == temp[9])
				SysDataFun(DATA_MSG_CTRL,Type,Id,10,0,&temp[2],0);
			else
			{
				if(pdata[2] != pdata[3])
				{
					temp[9] = pdata[1];
					temp[10] = pdata[0];
				}					
				UdpAppProcCtrl(AppProcCtrl_control_request,temp,12,temp[9]);
			}
		}
			break;
		
		default:
			break;
	}
	
	return 1;
}

/*********************************************************
** ��������
** �䡡�룺None
** �䡡����None
** ����������������Ϣά��
************************************************************/
void ActionDataInit(void)
{
	uint8_t i;
	
	memset(actionInfoMng.buf,0 ,sizeof(actionInfoMng.buf));
	actionInfoMng.pHead = actionInfoMng.pEnd =NULL;	
	
	actionInfoMng.Cnt = 0;
	for(i=0 ; i<ACTIONINFO_BUF_LEN - 1; i++)
	{
		actionInfoMng.buf[i].pNext = &actionInfoMng.buf[i+1];
	}	
	actionInfoMng.buf[ACTIONINFO_BUF_LEN-1].pNext = NULL;
	actionInfoMng.pFree = &actionInfoMng.buf[0];
	
	manualActionMng.Cnt = 0;
	memset(manualActionMng.buf,0 ,sizeof(manualActionMng.buf));
	manualActionMng.pHead = manualActionMng.pEnd =NULL;	
	
	for(i=0 ; i<ACTIONINFO_BUF_LEN - 1; i++)
	{
		manualActionMng.buf[i].pNext = &manualActionMng.buf[i+1];
	}	
	manualActionMng.buf[ACTIONINFO_BUF_LEN-1].pNext = NULL;
	manualActionMng.pFree = &manualActionMng.buf[0];	
}

static void InsActionBufQueue(ActionInfoType *pdata)
{	
	ActionInfoType *ptemp;
#if OS_CRITICAL_METHOD == 3                      
    OS_CPU_SR  cpu_sr = 0;
#endif

	OS_ENTER_CRITICAL();
	
	if(actionInfoMng.Cnt <ACTIONINFO_BUF_LEN)
	{
		ptemp = actionInfoMng.pFree;
		actionInfoMng.pFree = ptemp->pNext;
		ptemp->pNext = NULL;
		
		ptemp->actionType = pdata->actionType;
		ptemp->actionID = pdata->actionID;
		ptemp->runstate = pdata->runstate;

		if (actionInfoMng.pEnd != NULL)
		{
			actionInfoMng.pEnd->pNext = ptemp;
		}

		actionInfoMng.pEnd = ptemp;
		if(actionInfoMng.pHead == NULL)
		{
			actionInfoMng.pHead = ptemp;
		}
		actionInfoMng.Cnt ++;
	}
	
	OS_EXIT_CRITICAL();
}

static int compareActionBuf(ActionInfoType *pdata,ActionInfoType *pNode)
{
	if((pdata ==NULL)||(pNode ==NULL))
		return -1;
	
	if((pNode->actionType == pdata->actionType)&&(pNode->actionID == pdata->actionID))
		return 0;
			
	return -1;
}

void DelActionBufQueue(ActionInfoType *pdata)
{	
	ActionInfoType *pPrevious;
	ActionInfoType *pNode,*pDeleted;
	int nCompareRes;
#if OS_CRITICAL_METHOD == 3                      
    OS_CPU_SR  cpu_sr = 0;
#endif

	OS_ENTER_CRITICAL();
	
	if(actionInfoMng.Cnt >0)
	{
		pPrevious = NULL;
		pNode = actionInfoMng.pHead;
		while (pNode != NULL)
		{
			nCompareRes = compareActionBuf(pdata, pNode);
			if (nCompareRes == 0)
			{
				pDeleted = pNode;
				pNode = pNode->pNext;

				if (pDeleted == actionInfoMng.pHead)
				{
					actionInfoMng.pHead = pDeleted->pNext;
				}
				else
				{
					pPrevious->pNext = pDeleted->pNext;
				}

				if (pDeleted == actionInfoMng.pEnd)
				{
					actionInfoMng.pEnd = pPrevious;
				}
				
				pDeleted->pNext =actionInfoMng.pFree;
				actionInfoMng.pFree = pDeleted;
				actionInfoMng.Cnt --;
				
				break;
			}

			pPrevious = pNode;
			pNode = pNode->pNext;
		}	
	}
	
	if(actionInfoMng.Cnt == 0)
	{
		memset(&ManualActionClear,0,sizeof(ManualActionClear));
	}
	
	OS_EXIT_CRITICAL();
}

static u8 UpdateActionBufQueue(ActionInfoType *pdata)
{	
	ActionInfoType *pPrevious;
	ActionInfoType *pNode;
	int nCompareRes;
#if OS_CRITICAL_METHOD == 3                      
    OS_CPU_SR  cpu_sr = 0;
#endif

	OS_ENTER_CRITICAL();
	
	if(actionInfoMng.Cnt >0)
	{
		pPrevious = NULL;
		pNode = actionInfoMng.pHead;
		while (pNode != NULL)
		{
			nCompareRes = compareActionBuf(pdata, pNode);
			if (nCompareRes == 0)
			{
				pNode->runstate = pdata->runstate;

				OS_EXIT_CRITICAL();
				return 1;
			}

			pPrevious = pNode;
			pNode = pNode->pNext;
		}	
	}
	
	OS_EXIT_CRITICAL();
	
	return 0;
}

void AddActionInfo(u8 type,u8 id,u8 runState)
{
	ActionInfoType action;
	u8 ret;
	
	action.actionID = id;
	action.actionType = type;
	action.runstate = runState;
	action.pNext =NULL;
	
	if((runState == RunStateType_IDLE)||(runState == RunStateType_STOP))
	{
		DelActionBufQueue(&action);
		if((actionInfoMng.Cnt==0)&&(SysState.globalState.slaveMode!=SYSMODE_CONFIG_FIN))
			SysState.globalState.slaveMode = SYSMODE_NORMAL;
	}
	else
	{
		ret = UpdateActionBufQueue(&action);
		if(ret == 0)
			InsActionBufQueue(&action);
		
		if(actionInfoMng.Cnt>0)
			SysState.globalState.slaveMode = SYSMODE_SLAVE_CTRL;
	}
}

ActionInfoType * getActionInfoHead(void)
{
	return actionInfoMng.pHead;
}

u8  getActionInfoBufSzie(void)
{
	return actionInfoMng.Cnt;
}

//----------------manual action mng-------------------
static void InsManualActionBufQueue(ActionInfoType *pdata)
{	
	ActionInfoType *ptemp;
#if OS_CRITICAL_METHOD == 3                      
    OS_CPU_SR  cpu_sr = 0;
#endif

	OS_ENTER_CRITICAL();
	
	if(manualActionMng.Cnt <ACTIONINFO_BUF_LEN)
	{
		ptemp = manualActionMng.pFree;
		manualActionMng.pFree = ptemp->pNext;
		ptemp->pNext = NULL;
		
		ptemp->actionType = pdata->actionType;
		ptemp->actionID = pdata->actionID;
		ptemp->runstate = pdata->runstate;

		if (manualActionMng.pEnd != NULL)
		{
			manualActionMng.pEnd->pNext = ptemp;
		}

		manualActionMng.pEnd = ptemp;
		if(manualActionMng.pHead == NULL)
		{
			manualActionMng.pHead = ptemp;
		}
		manualActionMng.Cnt ++;
	}
	
	OS_EXIT_CRITICAL();
}

static void DelManualActionBufQueue(ActionInfoType *pdata)
{	
	ActionInfoType *pPrevious;
	ActionInfoType *pNode,*pDeleted;
	int nCompareRes;
#if OS_CRITICAL_METHOD == 3                      
    OS_CPU_SR  cpu_sr = 0;
#endif

	OS_ENTER_CRITICAL();
	
	if(manualActionMng.Cnt >0)
	{
		pPrevious = NULL;
		pNode = manualActionMng.pHead;
		while (pNode != NULL)
		{
			nCompareRes = compareActionBuf(pdata, pNode);
			if (nCompareRes == 0)
			{
				pDeleted = pNode;
				pNode = pNode->pNext;

				if (pDeleted == manualActionMng.pHead)
				{
					manualActionMng.pHead = pDeleted->pNext;
				}
				else
				{
					pPrevious->pNext = pDeleted->pNext;
				}

				if (pDeleted == manualActionMng.pEnd)
				{
					manualActionMng.pEnd = pPrevious;
				}
				
				pDeleted->pNext =manualActionMng.pFree;
				manualActionMng.pFree = pDeleted;
				manualActionMng.Cnt --;
				
				break;
			}

			pPrevious = pNode;
			pNode = pNode->pNext;
		}	
	}
	
	OS_EXIT_CRITICAL();
}

static u8 UpdateManualActionBufQueue(ActionInfoType *pdata)
{	
	ActionInfoType *pPrevious;
	ActionInfoType *pNode;
	int nCompareRes;
#if OS_CRITICAL_METHOD == 3                      
    OS_CPU_SR  cpu_sr = 0;
#endif

	OS_ENTER_CRITICAL();
	
	if(manualActionMng.Cnt >0)
	{
		pPrevious = NULL;
		pNode = manualActionMng.pHead;
		while (pNode != NULL)
		{
			nCompareRes = compareActionBuf(pdata, pNode);
			if (nCompareRes == 0)
			{
				pNode->runstate = pdata->runstate;

				OS_EXIT_CRITICAL();
				return 1;
			}

			pPrevious = pNode;
			pNode = pNode->pNext;
		}	
	}
	
	OS_EXIT_CRITICAL();
	
	return 0;
}

u8 CheckManualActionList(u8 *pactionList)
{	
	ActionInfoType *pNode;
	u8 len=0;
	
	if(pactionList==NULL)
		return 0;
	
	if(manualActionMng.Cnt >0)
	{
		pNode = manualActionMng.pHead;
		while (pNode != NULL)
		{
			if((pNode->runstate == RunStateType_RUN_MANUAL)&&( pNode->actionType ==0))
			{
				pactionList[len] = pNode->actionID;			
				if(len<MANUALACTIONINFO_LEN)
					len++;
			}
			pNode = pNode->pNext;
		}	
	}
	return len;
}

void AddManualActionInfo(u8 type,u8 id,u8 runState)
{
	ActionInfoType action;
	u8 ret;
	
	action.actionID = id;
	action.actionType = type;
	action.runstate = runState;
	action.pNext =NULL;
	
	if((runState == RunStateType_IDLE)||(runState == RunStateType_STOP))
	{
		DelManualActionBufQueue(&action);
	}
	else
	{
		ret = UpdateManualActionBufQueue(&action);
		if(ret == 0)
			InsManualActionBufQueue(&action);
	}
}

u8  getManualActionInfoBufSzie(void)
{
	return manualActionMng.Cnt;
}

#endif
