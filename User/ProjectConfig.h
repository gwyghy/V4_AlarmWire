/*********************************************************************************************************
*                                              ProjectConfig	
*                                                工程配置
*********************************************************************************************************/
#ifndef  PROJECT_CONFIG
#define  PROJECT_CONFIG
/*********************************************************************************************************
*                                                程序版本号		
* ********************************************** 版本号规则***********************************************
*             开发阶段：                            ***            发布阶段（程序与JSON分开后）：
*             8（V4）＋1（工作面）    +  XX         ***            8(V4) + 8(通用版) + XX
*             8（V4）＋2（超前）      +  XX         ***
*             8（V4）＋3（自移）      +  XX         ***
*             8（V4）＋4（单轨吊小车）+  XX         ***
*             8（V4）＋5（设备列车）  +  XX         ***
*             8（V4）＋0（特殊版）    +  XX         ***
*********************************************************************************************************/
#define  V4_CONTROLLER_VERSION_BYTE1    8
#define  V4_CONTROLLER_VERSION_BYTE2    8
#define  V4_CONTROLLER_VERSION_BYTE3    2
#define  V4_CONTROLLER_VERSION_BYTE4    0
/*********************************************************************************************************
*                                            FIRST_TURNON_FLAG			
*********************************************************************************************************/
//**此宏只可以增加，在修改应用场景或修改JSON文件后请将此宏+1**//

#define  FIRST_TURNON_FLAG      0x12345611

//**此宏不可以修改//
#define  RESTORE_FASTORY_SETTING  0x66666666


/*********************************************************************************************************
*                                      APPLICATION_SCENARIO_SELECTION
*									        应用场景选择				
*********************************************************************************************************/
//**活鸡兔工作面**//
#define	LIVE_CHICKEN_AND_RABBIT_WORKING_FACE                   0
//**活鸡兔自移机尾**//
#define	LIVE_CHICKEN_AND_RABBIT_SELF_MOVING_TAIL               1
//**活鸡兔托缆小车**//
#define	LIVE_CHICKEN_AND_RABBIT_CABLE_CAR                      2
//**标准四柱综放**//
#define	STANDARD_FOUR_PILLAR_FULLY_MECHANIZED                  3
//**大柳塔工作面**//
#define	DALIUTA_WORKING_FACE                                   4
//**活鸡兔超前支架**//
#define LIVE_CHICKEN_AND_RABBIT_LEAD_SUPPORT                   5
//**锦界31117工作面**//
#define JINJIE_31117_WORKING_FACE                              6

//**需要在上面选择一种类型,修改后请务必修改 FIRST_TURNON_FLAG**// 
#define APPLICATION_SCENARIO_SELECTION   JINJIE_31117_WORKING_FACE

/*********************************************************************************************************
*                                      		FMC_CLOCK_DIVISION_SELECTION
*									            FMC时钟分频选择			
*********************************************************************************************************/
//**二分频适用与华田屏幕**//
#define	DUAL_FREQUENCY_HUATIAN_SCREEN                                   0
//**三分频适用与天马屏幕**//
#define	TRI_FREQUENCY_TIANMA_SCREEN                                     1
 
//**选择FMC时钟分频选择**//
#define FMC_CLOCK_DIVISION_SELECTION           TRI_FREQUENCY_TIANMA_SCREEN

//jhy 20220726   disable mainsceen animation  0 disable , 1  enable
#define MAIN_INTERFACE_ANIMATION    0

#define	HONGMENG       1 
#define	HNDZ           0 

#define BOOT_SCREEN_SELECTION    HONGMENG

//jhy 20220908  id and index apart    0 disable , 1  enable 
#define IDANDINDEXAPART  0


//jhy 20220908  json and program apart   0 disable , 1  enable 
#define JSONANDPROGRAMAPART   1


//ghy 指定锦界31117工作面发送特殊支架编号
#define BISUO_SUPPORT_NUM   235

#endif
//#include "ProjectConfig.h"
