

1. Project Components

   OSII-STM32F429
     |- User ----------------- 程序的主函数
	 |- User/OSA-------------- OS API
	 |- Logic/SysFunction----- 逻辑程序
	 |- Logic/FileLoad-------- 文件或程序下载
	 |- Logic/FileUpdate------ 文件或程序更新
	 |- Server/Network/udp---- 架间通信协议栈
	 |- Server/Network/ipscaner架间通信-IP搜索服务
	 |- Server/Network/bcm---- 架间通信-网络交换机功能服务
	 |- Server/Can------------ 设备间通信CAN协议栈
	 |- Server/Devicemng------ 设备管理服务
	 |- Server/Log------------ 日志服务
	 |- Server/Util----------- 通用服务库，如内存分配等
	 |- UI/MainWin------------ 主画面及动作画面
	 |- UI/StateWin----------- 状态界面
	 |- UI/ParamWin----------- 参数设置界面
	 |- UI/SysSetupWin-------- 系统维护界面
	 |- Driver/Lwip----------- TCPIP 协议栈
	 |- Driver/FatFs---------- 文件系统API
	 |- Driver/BSP------------ 底层驱动
	 |- Driver/GUI------------ GUI驱动或接口
	 |- User/Startup---------- 设备启动文件	 
	 |- lib/StdPeriph_Driver-- 外围设备库 
	 |- lib/DSP_Lib----------- DSP库函数
	 |- lib/Eth_Driver-------- 以太网库函数
	 |- lib/emwin------------- emwin库函数
	 |- uC/OSII-Source-------- ucos	 
	 |- uC/OSII-Port---------- ucos


2.  发布清单路径为：2022-v4-controller/ 程序发布清单.xlsx
     1）发布程序时请按照《程序发布清单.xlsx》中模板的Sheet页填写
	 《程序发布清单.xlsx》，不要删除模板Sheet页。
     2）提交《程序测试申请单》时，请将以下信息写入备注
	“程序发布清单路径为：2022-v4-controller/ 程序发布清单.xlsx。
	请将《程序发布清单.xlsx》与编译后的.bin文件一并交给测试，
	用笑脸工具校验时将输出文件名称填写为“kzq+程序版本号+_+提测日期”，
	如2022年8月6日提交了一版版本号为8146的控制器程序，
	则输出文件名称为kzq8146_0806”
