/**
* FileName: Manager.h
* Author: ZiQin
* BuildDate: 2018-6-22
* Version: 1.1.0
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
	PcbManager pcbManager;
	RcbManager rcbManager;
	Pcb * running;
	int err;
public:
	Manager(int pcbPrio = 3);
	Manager(int pcbPrio, int rcbNum, int * resourceNumber, std::vector<std::string> nm);
	~Manager();

	int begin();

	int createPcb(int prio, std::string nm = "Unknown");

	bool destroyPcb(int pid);

	bool requestRcbForPcb(int rid, int num = 1);
	bool requestRcbForPcbByName(std::string nm, int num = 1);

	bool releaseRcbForPcb(int rid, int num = 1);
	bool releaseRcbForPcbByName(std::string nm, int num = 1);

	int findPcbByName(std::string nm);

	void addRcb(std::string nm = "Unknown", int rcbResNum = 1);

	int searchRidByName(std::string nm);

	std::vector<PcbInfo> getTotalPcbInfo();

	std::vector<RcbInfo> getTotalRcbInfo();

	int pauseManager();

	int schedul();

	int timeOut();

	void restart();

	std::string getLatestErrStr();

	int getLatestErrCode();

	int prioMax();

private:
	void updateReadyList();
};
