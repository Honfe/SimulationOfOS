/**
* FileName: PcbList
* Author: ZiQin
* BuildDate:2018-5-31
* Version:	1.0.0
*/

#pragma once

#include "PCB.h"
#include "RCB.h"
#include <vector>
#include <string>

class Pcb;

/**
* 进程块的基本信息
*/
struct PcbInfo {
	int pid;
	std::string name;
	std::string status;
};

class PcbList {
private:
	// 保存进程的队列
	std::vector<Pcb*> * list;
	// 该队列中允许的最大优先级-1
	int priorityMax;

public:
	// 构造函数
	PcbList(int prio = 3);
	~PcbList();

	/**
	* 函数名：insertPcbList
	* 功能：将pcb插入队列中
	* 参数：pcb：将要插入的进程控制块
	* 返回值：无
	*/
	void insertPcbList(Pcb * pcb);

	/**
	* 函数名：findPcbByPid
	* 功能：通过pid查找进程控制块
	* 参数：pid：要查找的pcb的pid
	* 返回值：进程控制块（未找到返回nullptr）
	*/
	Pcb * findPcbByPid(int pid);

	/**
	* 函数名：findPcbByName
	* 功能：通过名字查找进程控制块
	* 参数：nm：要查找的pcb的name
	* 返回值：进程控制块（未找到返回nullptr）
	*/
	Pcb * findPcbByName(std::string nm);

	/**
	* 函数名：removePcbList
	* 功能：将pcb从队列中移出
	* 参数：要移出进程pid
	* 返回值：true/false
	*/
	bool removePcbList(int pid);

	/**
	* 函数名：getHighestPcb
	* 功能：获取当前队列中最高优先级的pcb
	* 参数：无
	* 返回值：当前队列中最高优先级
	*/
	int getHighestPcb();

	/**
	* 函数名：getTotalPchInfoInList
	* 功能：获取当前队列中的所有进程控制块
	* 参数：无
	* 返回值：含有进程基本信息的vector容器
	*/
	std::vector<PcbInfo> getTotalPcbInfoInList();

	/**
	* 函数名：getPcbNumber
	* 功能：获取队列中pcb数量
	* 参数：无
	* 返回值：队列中pcb的数量
	*/
	int getPcbNumber();

	/**
	* 函数名：getPriorityPcbNumber
	* 功能：获取该优先级下Pcb的数量
	* 参数：prio：要查询的优先级
	* 返回值：pcb数量
	*/
	int getPriorityPcbNumber(int prio);
};
