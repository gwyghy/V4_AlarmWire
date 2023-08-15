/*
*********************************************************************************************************
*	                                            V4
*                           Commuication Controller For The Mining Industry
*
*                                  (c) Copyright 1994-2013  HNDZ 
*                       All rights reserved.  Protected by international copyright laws.
*
*
*    File    : Can_Bus.c
*    Module  : user
*    Version : V1.0
*    History :
*   -----------------
*              Version  Date           By            Note
*              v1.0         
*
*********************************************************************************************************
*/

#ifndef _LOG_DATA_TYPE_H_
#define _LOG_DATA_TYPE_H_

#define LOGDATASIZE    6


typedef uint8_t log_int;




/*故障日志*/
//需要往后递增
typedef enum
{
	FAULT_EVENT_FAULT,                             //传感器故障,
	FAULT_EVENT_OFFLINE,                           //设备离线,
	FAULT_EVENT_WORKFACE_COMMUNICATION_WRONG,      //工作面通信异常,
	FAULT_EVENT_SYSTEM_NUMBER_WRONG,               //系统编号错误,
	FAULT_EVENT_SERVER_WRONG,                      //SS位置或使能不符,
	FAULT_EVENT_NUMBER_ADD_DIRECT_WRONG,           //架号增向不符,
	FAULT_EVENT_NUMBER_RANGE_WRONG,                //架号范围参数错误,
	FAULT_EVENT_PARA_UPDATE_WRONG,				   //参数更新失败,
	FAULT_EVENT_ALL_PARA_NOT_SAME,                 //全局参数不一致 ,
	FAULT_EVENT_ALL_PARA_NOT_MATCH_LEFTNEIGHBOR,       //全局参数左邻架不符,
	FAULT_EVENT_ALL_PARA_NOT_MATCH_RIGHTNEIGHBOR,       //全局参数左邻架不符,
	FAULT_EVENT_PARGRAM_NOT_SAME,				   //程序版本不一致 ,
	FAULT_EVENT_PARGRAM_NOT_MATCH_NEIGHBOR,        //版本号与邻架不符,
	FAULT_EVENT_HARD_SOFT_NOT_MATCH,               //硬件与软件不符,
	FAULT_EVENT_LEFT_NEIGHBOR_COMMUNICATION_WRONG, //左邻架通信错误,
	FAULT_EVENT_RIGHT_NEIGHBOR_COMMUNICATION_WRONG,//右邻架通信错误,
	FAULT_EVENT_ABNORMAL_COMMUNICATION_WITH_SERVER,//与服务器通信异常,
	/*与服务器参数不一致故障需要连续*/
	FAULT_EVENT_SYSPARAM_MINSUPPORTNUM_NOTSAME,    //服务器最小架号不一致,
	FAULT_EVENT_SYSPARAM_MAXSUPPORTNUM_NOTSAME,    //服务器最大架号不一致,
	FAULT_EVENT_SYSPARAM_SUPPORTNUMINCREASE_NOTSAME,//服务器架号增向不一致,
	/*END*/
	FAULT_EVENT_MAX
} FAULT_EVENT;



typedef enum 
{
	FAULT_STATUS_FAULT,
	FAULT_STATUS_NORMAL,
} FAULT_STATUS;


typedef enum 
{
	FAULT_HOST_SENSOR,//传感器类型：模拟量，角度，高度
	FAULT_HOST_DEVICE,//声光，红外，人员 
	FAULT_HOST_HUB1,//HUB1
	FAULT_HOST_HUB2,//HUB2
	FAULT_HOST_HUB3,//HUB3
	FAULT_HOST_OTHER,//其他
} FAULT_HOST;
	

typedef struct
{
	uint32_t     time;
	log_int      host_type; //FAULT_HOST类型
	log_int      host_name; //id，左前压力
	log_int      event;     //FAULT_EVENT
	log_int      status;    //FAULT_STATUS
} STR_FAULT_INFO;


/*运行日志*/
typedef enum 
{	
	LOG_STATUS_START=0,         //直接启动(启动)
	LOG_STATUS_STOP ,           //停止
	LOG_STATUS_DELAY_START ,    //延时启动
	LOG_STATUS_SUSPEND,         //暂停

	LOG_STATUS_INPLACE,         //到位	
	LOG_STATUS_DISABLE,         //禁止
	LOG_STATUS_OVERTIME,        //超时-暂时不加
	
	LOG_STATUS_SENSOR_NORMAL,   //传感器正常
	LOG_STATUS_SENSOR_ALARM,    //传感器报警
	LOG_STATUS_EMERGENCY_ALARM, //急停
	LOG_STATUS_EMERGENCY_NORMAL,//急停恢复
	LOG_STATUS_BISUO_ALARM,     //闭锁
	LOG_STATUS_BISUO_NORMAL,     //闭锁恢复
	
	LOG_STATUS_DOWNLOAD_SUCCESS, //下载成功
	LOG_STATUS_DOWNLOAD_FAIL,    //下载失败
	LOG_STATUS_UPDATE_PRO_SUCCESS,//更新程序成功
	LOG_STAUTS_UPDATE_PRO_FAIL,   //更新程序失败
	LOG_STATUS_UPDATE_PARA_SUCCESS,//更新参数成功
	LOG_STATUS_UPDATE_PARA_FAIL, //更新参数失败
	LOG_STATUS_RESET_LOWPAWER,//低功耗复位
	LOG_STATUS_RESET_WWATCHDOG,//窗口看门狗
	LOG_STATUS_RESET_IWATCHDOG,//独立看门狗
	LOG_STATUS_RESET_SOFTWARE , //软件复位
	LOG_STATUS_RESET_PORRSTF,  //掉电复位
	
	LOG_STATUS_PERSONLOCK_TIMEOUT,//人员闭锁超时
	
	LOG_STATUS_NET_REINIT,//网络重新初始化
	
	LOG_STATUS_START_OUT,  //外部启动
	LOG_STATUS_START_IN   //内部启动
	
} LOG_STATUS;



typedef enum 
{
	LOG_TYPE_HANDLE_SINGLE = 0, //单动作操作
	LOG_TYPE_HANDLE_COMB, 		//组合动作操作
	LOG_TYPE_HANDLE_GROUP,		//成组动作操作
	LOG_TYPE_HANDLE_SYSTEM,		//系统操作
	LOG_TYPE_SERVICE,    		//服务
	LOG_TYPE_ALARM,      		//传感器报警
	LOG_TYPE_SYSTEM_ALARM,      //系统报警
	LOG_TYPE_UPDATE_PROGRAM,  //更新程序
	LOG_TYPE_RESET_SYSTEM,   //系统复位
	LOG_TYPE_JSON_TEXT   //配置文件启动
} LOG_TYPE;

typedef struct
{
	uint32_t      time;
	log_int   	  host;	//id
	log_int   	  type;
	log_int       status;
	uint8_t       data[LOGDATASIZE];//动作操作类：0-控制源地址、1-控制源类型、2/3-用户ID、4-手动自动
									//报警类：0-部件id
} STR_LOG_INFO;


//运行提示信息字串
//const uint8_t u8FaultInfoMsg[FAULT_EVENT_MAX][17]	=
//{
//	"故障",
//	"离线",             
//	"工作面通信异常",
//  "系统编号错误",     
//	"SS位置或使能不符",
//	"架号增向不符",
//	"架号范围参数错误", 
//	"参数更新失败",
//	"全局参数不一致",   
//	"全局参数邻架不符",	
//	"程序版本不一致",   
//	"版本号与邻架不符", 
//	"硬件与软件不符",	  
//	"左邻架通信错误",   
//	"右邻架通信错误",	  
//};                                  


#endif

