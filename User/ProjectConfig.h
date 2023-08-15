/*********************************************************************************************************
*                                              ProjectConfig	
*                                                ��������
*********************************************************************************************************/
#ifndef  PROJECT_CONFIG
#define  PROJECT_CONFIG
/*********************************************************************************************************
*                                                ����汾��		
* ********************************************** �汾�Ź���***********************************************
*             �����׶Σ�                            ***            �����׶Σ�������JSON�ֿ��󣩣�
*             8��V4����1�������棩    +  XX         ***            8(V4) + 8(ͨ�ð�) + XX
*             8��V4����2����ǰ��      +  XX         ***
*             8��V4����3�����ƣ�      +  XX         ***
*             8��V4����4�������С����+  XX         ***
*             8��V4����5���豸�г���  +  XX         ***
*             8��V4����0������棩    +  XX         ***
*********************************************************************************************************/
#define  V4_CONTROLLER_VERSION_BYTE1    8
#define  V4_CONTROLLER_VERSION_BYTE2    8
#define  V4_CONTROLLER_VERSION_BYTE3    2
#define  V4_CONTROLLER_VERSION_BYTE4    0
/*********************************************************************************************************
*                                            FIRST_TURNON_FLAG			
*********************************************************************************************************/
//**�˺�ֻ�������ӣ����޸�Ӧ�ó������޸�JSON�ļ����뽫�˺�+1**//

#define  FIRST_TURNON_FLAG      0x12345611

//**�˺겻�����޸�//
#define  RESTORE_FASTORY_SETTING  0x66666666


/*********************************************************************************************************
*                                      APPLICATION_SCENARIO_SELECTION
*									        Ӧ�ó���ѡ��				
*********************************************************************************************************/
//**��ù�����**//
#define	LIVE_CHICKEN_AND_RABBIT_WORKING_FACE                   0
//**������ƻ�β**//
#define	LIVE_CHICKEN_AND_RABBIT_SELF_MOVING_TAIL               1
//**�������С��**//
#define	LIVE_CHICKEN_AND_RABBIT_CABLE_CAR                      2
//**��׼�����۷�**//
#define	STANDARD_FOUR_PILLAR_FULLY_MECHANIZED                  3
//**������������**//
#define	DALIUTA_WORKING_FACE                                   4
//**��ó�ǰ֧��**//
#define LIVE_CHICKEN_AND_RABBIT_LEAD_SUPPORT                   5
//**����31117������**//
#define JINJIE_31117_WORKING_FACE                              6

//**��Ҫ������ѡ��һ������,�޸ĺ�������޸� FIRST_TURNON_FLAG**// 
#define APPLICATION_SCENARIO_SELECTION   JINJIE_31117_WORKING_FACE

/*********************************************************************************************************
*                                      		FMC_CLOCK_DIVISION_SELECTION
*									            FMCʱ�ӷ�Ƶѡ��			
*********************************************************************************************************/
//**����Ƶ�����뻪����Ļ**//
#define	DUAL_FREQUENCY_HUATIAN_SCREEN                                   0
//**����Ƶ������������Ļ**//
#define	TRI_FREQUENCY_TIANMA_SCREEN                                     1
 
//**ѡ��FMCʱ�ӷ�Ƶѡ��**//
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


//ghy ָ������31117�����淢������֧�ܱ��
#define BISUO_SUPPORT_NUM   235

#endif
//#include "ProjectConfig.h"
