/**
* FileName: PcbManager.h
* Author: ZiQin
* BuildDate: 2018-6-22
* Version: 1.0.1
*/

#pragma once

#include "PCB.h"
#include "PcbList.h"
#include <vector>
#include <string>

#define __pcbManager_NO_ERR					0		// 无错误
#define __pcbManager_PID_INVAILD			-1		// 无效pid
#define __pcbManager_PCB_NOTFOUND	-2		// 未找到pcb
#define __pcbManager_PRIO_INVAILD		-3		// 无效优先级或非法优先级（0）

class PcbManager {
private:
	// 所有进程管理队列
	PcbList totalList;
	// 就绪进程队列
	PcbList readyList;
	// 挂起进程队列
	PcbList blockList;
	// 所有进程中允许的最高优先级-1
	int priorityMax;

public:
	// 构造函数
	PcbManager(int prio = 3);
	~PcbManager();

	/**
	* 函数名：createPcb
	* 功能：创建一个pcb
	* 参数：
	*		par：父进程
	*		nm：进程名
	*		prio：进程优先级
	* 返回值：新建进程的pid
	*/
	int createPcb(Pcb * par, std::string nm = "Unknown",  int prio = 1);
	
	/**
	* 函数名：destroyPcb
	* 功能：删除一个进程
	* 参数：pid：要删除的进程号
	* 返回值：状态码
	*/
	int destroyPcb(int pid);

	/**
	* 函数名：findPcbByPid/Name
	* 功能：通过pid/name查找进程
	* 参数：pid：要查找的进程号
	* 返回值：进程控制块（未找到则返回nullptr）
	*/
	Pcb * findPcbByPid(int pid);
	Pcb * findPcbByName(std::string nm);

	/**
	* 函数名：checkPcbStatus
	* 功能：查询进程当前状态
	* 参数：pid：被查询的进程号
	* 返回值：进程状态码
	*/
	int checkPcbStatus(int pid);

	/**
	* 函数名：getHighestPcb
	* 功能：获取当前最高优先级Pcb
	* 参数：无
	* 返回值：pid
	*/
	int getHighestPcb();

	/**
	* 函数名：insertReady/BlockPcb
	* 功能：将pid插入就绪/等待队列
	* 参数：pid：进程标识符
	* 返回值：true/false
	*/
	bool insertReadyPcb(int pid);
	bool insertBlockPcb(int pid);

	/**
	* 函数名：removeReady/BlockPcb
	* 功能：将pid从就绪/等待队列中移除
	* 参数：pid：进程标识符
	* 返回值：true/false
	*/
	bool removeReadyPcb(int pid);
	bool removeBlockPcb(int pid);

	/**
	* 函数名：comparePcbPriority
	* 功能：比较两个进程之间的优先级
	* 参数：pid1/pid2：参与比较的两个进程pid
	* 返回值：如果pid1优先级大于pid2，则返回true，否则false
	*/
	bool comparePcbPriority(int pid1, int pid2);

	/**
	* 函数名：getTotalPcbInfo
	* 功能：获取所有pcb的基本信息
	* 参数：无
	* 返回值：装有pcb基本信息的vector容器
	*/
	std::vector<PcbInfo> getTotalPcbInfo();

	/**
	* 函数名：getPcbNumber
	* 功能：获取pcb的总数
	* 参数：无
	* 返回值：pcb总数
	*/
	int getPcbNumber();
	
	/**
	* 函数名：getPrioMax
	* 功能：获取pcbManager中允许的pcb最大优先级
	* 参数：无
	* 返回值：允许的最大优先级
	*/
	int getPrioMax();

private:
	/**
	* 函数名：removePcbFromList
	* 功能：将对应的pcb从所有队列中移除
	* 参数：pcb：要被移除的控制块
	* 返回值：无
	*/
	void removePcbFromList(Pcb * pcb);
};
