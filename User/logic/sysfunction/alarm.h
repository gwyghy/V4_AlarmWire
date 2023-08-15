#ifndef __ALARM_H__
#define __ALARM_H__

#include "includes.h"
#include "devmng_api.h"
#include "Log_Data_Mng.h"

#define  LOGIC_TIME_MS      100
#define  ALARMCTRL_BUF_LEN	40

/*------------------------param define----------------------*/
enum
{
	ALARM_UNIT_SOUND=0,
	ALARM_UNIT_LIGHT,
	ALARM_UNIT_PHONE,
	ALARM_UNIT_MAX
};

enum
{
	ALARM_MODE_NONE =0,
	ALARM_MODE_1 ,   //红灯常亮
	ALARM_MODE_2,    //红蓝闪烁
	ALARM_MODE_MAX
};

typedef enum
{
	EnableType_DISABLE = 0,
	EnableType_ENABLE 
}EnableType;

typedef enum
{
	AlarmType_Phone =0,
	AlarmType_SoundLight,
	AlarmType_Sound,
	AlarmType_Light,
	AlarmTypeMAX
}AlarmType;

typedef enum
{
//	BaseCtrlType_NONE=0,
	BaseCtrlType_STARTUP=1,
	BaseCtrlType_STOP=0
}BaseCtrlType;

typedef enum
{
	BaseRunType_STOP =0,
	BaseRunType_RUN
}BaseRunType;

typedef struct
{
	ParamUnit driver;
	ParamUnit device;
	ParamUnit subDevice;
	ParamUnit point;
}PointIDType;

/*------------------------state define----------------------*/


/*------------------------part module define----------------------*/
typedef u8 (*alarmCBFunc)(PointIDType point,u8 phoneAddr,void *pdata,u8 alarmMode,u8 beepflag);

typedef struct
{
	EnableType  enable;
	AlarmType	type;
	PointIDType point;
	u8			phoneAddr;
	int			alarmTime;
	int			ticks;
	BaseRunType state;
	u8      alarmMode;
	u8			AlarmEventType;
	u8			AlarmEventID;
	alarmCBFunc alarmFunc;
}AlarmUnitType;

typedef struct
{
	AlarmUnitType	alarmDev[ALARM_UNIT_MAX];
}AlarmModuleType;

typedef struct
{
	u8				  eventType;			//0:单动作 1：组合动作  2：成组动作  
	u8				  eventID;	
	u8                use;
	u8				  eventMode;            //报警方式
	int				  alarmTime;
}AlarmCtrlType;

typedef struct
{
	AlarmCtrlType    buf[ALARMCTRL_BUF_LEN];	//buf
	u8               Cnt;					    //项数
}AlarmCtrlBufType;


/*------------------------other define----------------------*/
u8 alarmFunc(PointIDType point,u8 phoneAddr,void *pdata,u8 alarmMode,u8 beepflag);
u8 alarmFunc_phone(PointIDType point,u8 phoneAddr,void *pdata,u8 alarmMode,u8 beepflag);

void AlarmModuleInit(void);
void AlarmModuleConfig(uint8_t type,uint8_t DriverId,uint8_t DeviceId,uint8_t PointId);
void AlarmModuleConfig_phone(uint8_t PhoneId);
u8 AlarmModuleEnable(EnableType enable);
u8 AlarmModuleEnable_phone(EnableType enable);
u8 CtrlAlarm(AlarmType	type,u16 alarmTime,BaseCtrlType cmd,void *pdata,u8 ctrlEventType,u8 ctrlEventID,u8 alarmMode);
void AlarmMng(u16 delay);
void AlarmAllStop(void);

#endif
