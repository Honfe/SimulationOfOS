/**
* FileName: Core.h
* Author: ZiQin
* BuildDate: 2018-6-22
* Version: 1.1.0
*/

#pragma once

#include "Manager.h"
#include <iostream>
#include <string>

#define __CORE_VERSION	"ZiQin Core V1.1"

class Core {
private:
	// 进程资源管理器
	Manager manager;
	// 内核版本信息
	std::string version;
	// 内核是否正在运行
	bool run;
	// 内核是否开启（模拟开关）
	bool on;

public:
	// 构造函数
	Core(int prio = 3);
	Core(int prio, int rcb, int *rcbNum, std::vector<std::string> nm);
	~Core();

	/**
	* 函数名：initial
	* 功能：初始化Core
	* 参数：
	*		rcb：资源数量
	*		rcbResNum：每种资源的最大资源量
	*		nm：每种资源的名字
	* 返回值：
	*/
	bool initial(int rcb, int * rcbResNum, std::vector<std::string> nm);

	/**
	* 函数名：begin
	* 功能：开始运行Core
	* 参数：无
	* 返回值：当前运行的pid
	*/
	int begin();

	/**
	* 函数名：createPcb
	* 功能：创建Pcb
	* 参数：
	*		prio：优先级
	*		nm：pcb的名字
	* 返回值：新建pcb的pid
	*/
	int createPcb(int prio = 1, std::string nm = "Unknown");

	/**
	* 函数名：deletePcbByPid/Name
	* 功能：通过pid/name删除pcb
	* 参数：pid/name：要删除的pcb的pid/name
	* 返回值：true/false
	*/
	bool deletePcbByPid(int pid);
	bool deletePcbByName(std::string nm);

	/**
	* 函数名：addRcb
	* 功能：增加资源
	* 参数：
	*		nm：新增的资源名
	*		rcbResNum：资源的允许最大资源量
	* 返回值：无
	*/
	void addRcb(std::string nm = "Unknown", int rcbResNum = 1);

	/**
	* 函数名：requestRcb
	* 功能：为当前运行的进程申请资源
	* 参数：
	*		rid/name：资源rid/name
	*		num：申请的资源数量
	* 返回值：true/false
	*/
	bool requestRcb(int rid, int num = 1);
	bool requestRcb(std::string nm, int num = 1);

	/**
	* 函数名：releaseRcb
	* 功能：为当前运行的进程释放资源
	* 参数：
	*		rid/name：资源rid/name
	*		num：希望释放的资源量
	* 返回值：true/false
	*/
	bool releaseRcb(int rid, int num = 1);
	bool releaseRcb(std::string nm, int num = 1);

	/**
	* 函数名：suspendCore
	* 功能：挂起（暂停）Core
	* 参数：无
	* 返回值：被挂起的进程pid
	*/
	int suspendCore();

	/**
	* 函数名：getTotalPcbInfo
	* 功能：获取所有pcb的基本信息
	* 参数：无
	* 返回值：vector容器
	*/
	std::vector<PcbInfo> getTotalPcbInfo();
	std::vector<RcbInfo> getTotalRcbInfo();

	/**
	* 函数名：timeOut
	* 功能：超时
	* 参数：无
	* 返回值：调度后运行的pcb的pid
	*/
	int timeOut();

	/**
	* 函数名：restart
	* 功能：重启Core
	* 参数：无
	* 返回值：无
	*/
	void restart();

	/**
	* 函数名：exit
	* 功能：关闭Core
	* 参数：无
	* 返回值：无
	*/
	void exit();

	/**
	* 函数名：getVersion
	* 功能：获取Core版本
	* 参数：无
	* 返回值：Core版本
	*/
	std::string getVersion();

	/**
	* 函数名：isRunning
	* 功能：检查Core是否在运行
	* 参数：无
	* 返回值：true/false
	*/
	bool isRunning();

	/**
	* 函数名：isOn
	* 功能：检查Core是否开启
	* 参数：无
	* 返回值：true/false
	*/
	bool isOn();

	/**
	* 函数名：getManagerErrorMsg/Code
	* 功能：获取Manager的错误消息/错误码
	* 参数：无
	* 返回值：错误消息/错误码
	*/
	std::string getManagerErrorMsg();
	int getManagerErrorCode();

};
