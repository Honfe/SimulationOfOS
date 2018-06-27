/**
* FileName: Manager.h
* Author: ZiQin
* BuildDate: 2018-6-22
* Version: 1.1.1
*/

#pragma once

#include <iostream>
#include <vector>
#include <string>
#include "PCB.h"
#include "RCB.h"
#include "PcbManager.h"
#include "RcbManager.h"

#define __manager_NO_ERR					0		// 无错误
#define __manager_RES_OUT_TOTAL		-1		// 超出资源总量
#define __manager_RES_OUT_NUM		-2		// 超出当前可用资源数量
#define __manager_RES_OUT_HAVE		-3		// 超出当前PCB占有的资源量
#define __manager_RES_NOT_FOUND	-4		// 未找到资源
#define __manager_OP_ERR					-5		// 操作错误
#define __manager_PCB_CREATE_FAIL	-6		// pcb创建失败
#define __manager_PCB_PRIO_ERR			-7		// pcb优先级错误
#define __manager_PCB_NOT_FOUND	-8		// 未找到pcb
#define __manager_PCB_PID_INVAILD	-9		// 非法pid
#define __manager_NO_RUNNING			-10	// manager并未运行

class Manager {
private:
	// pcb管理器
	PcbManager pcbManager;
	// rcb管理器
	RcbManager rcbManager;
	// 正在运行的pcb
	Pcb * running;
	// 管理器运行记录器
	int err;

public:
	// 构造函数
	Manager(int pcbPrio = 3);
	Manager(int pcbPrio, int rcbNum, int * resourceNumber, std::vector<std::string> nm);
	~Manager();

	/**
	* 函数名：begin
	* 功能：Manager开始运行
	* 参数：无
	* 返回值：当前运行的进程的pid，若没有则返回-1
	*/
	int begin();

	/**
	* 函数名：createPcb
	* 功能：创建pcb
	* 参数：
	*		prio：创建进程的优先级
	*		nm：创建进程的名字
	* 返回值：成功创建的pcb的pid，创建失败返回-1
	*/
	int createPcb(int prio, std::string nm = "Unknown");

	/**
	* 函数名：destroyPcb
	* 功能：删除进程
	* 参数：pid：要删除的pcb的pid
	* 返回值：true/false
	*/
	bool destroyPcb(int pid);

	/**
	* 函数名：requsetRcbForPcb(ByName)
	* 功能：为进程申请资源
	* 参数：
	*		rid/nm：资源id号/资源名字
	*		num：申请的资源数量
	* 返回值：true/false
	*/
	bool requestRcbForPcb(int rid, int num = 1);
	bool requestRcbForPcbByName(std::string nm, int num = 1);

	/**
	* 函数名：releaseRcbForPcb(ByName)
	* 功能：为进程释放资源
	* 参数：
	*		rid/nm：资源id号/资源名字
	*		num：释放的资源数量
	* 返回值：true/false
	*/
	bool releaseRcbForPcb(int rid, int num = 1);
	bool releaseRcbForPcbByName(std::string nm, int num = 1);

	/**
	* 函数名：findPcbByName
	* 功能：通过进程名找到pid
	* 参数：nm：进程名
	* 返回值：找到的进程pid，未找到返回-1
	*/
	int findPcbByName(std::string nm);

	/**
	* 函数名：addRcb
	* 功能：添加Rcb
	* 参数：
	*		nm：添加的资源名字
	*		rcbResNum：资源拥有的总数
	* 返回值：无
	*/
	void addRcb(std::string nm = "Unknown", int rcbResNum = 1);

	/**
	* 函数名：searchRidByName
	* 功能：通过资源名查找rid
	* 参数：nm：资源名
	* 返回值：rid，未找到返回-1
	*/
	int searchRidByName(std::string nm);

	/**
	* 函数名：getTotalPcbInfo
	* 功能：获取所有pcb的基本信息
	* 参数：无
	* 返回值：vector容器
	*/
	std::vector<PcbInfo> getTotalPcbInfo();

	/**
	* 函数名：getTotalRcbInfo
	* 功能：获取所有rcb的基本信息
	* 参数：无
	* 返回值：vector容器
	*/
	std::vector<RcbInfo> getTotalRcbInfo();

	/**
	* 函数名：pauseManager
	* 功能：暂停管理器
	* 参数：无
	* 返回值：当前执行的pid
	*/
	int pauseManager();

	/**
	* 函数名：schedul
	* 功能：调度
	* 参数：无
	* 返回值：调度后执行的pid
	*/
	int schedul();

	/**
	* 函数名：timeOut
	* 功能：超时
	* 参数：无
	* 返回值：调度后执行的pid
	*/
	int timeOut();

	/**
	* 函数名：restart
	* 功能：重启Manager
	* 参数：无
	* 返回值：无
	*/
	void restart();

	/**
	* 函数名：getLatestErrStr
	* 功能：获取最近一次执行的错误信息
	* 参数：无
	* 返回值：错误信息
	*/
	std::string getLatestErrStr();

	/**
	* 函数名：getLatestErrCode
	* 功能：获取最近一次的错误码
	* 参数：无
	* 返回值：错误码
	*/
	int getLatestErrCode();

	/**
	* 函数名：prioMax
	* 功能：获取最大允许的优先级
	* 参数：无
	* 返回值：允许的最大优先级
	*/
	int prioMax();

private:
	/**
	* 函数名：updateReadyList
	* 功能：更新就绪队列
	* 参数：无
	* 返回值：无
	*/
	void updateReadyList();

	/**
	* 函数名：schedulForCreate
	* 功能：创建进程后的调度
	* 参数：无
	* 返回值：调度后运行的pid
	*/
	int schedulOnlyPriority();
};
