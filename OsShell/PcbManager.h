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
	PcbList totalList;
	PcbList readyList;
	PcbList blockList;
	int priorityMax;
public:
	PcbManager(int prio = 3);
	~PcbManager();

	int createPcb(Pcb * par, std::string nm = "Unknown",  int prio = 1);
	int destroyPcb(int pid);
	Pcb * findPcbByPid(int pid);
	Pcb * findPcbByName(std::string nm);
	int checkPcbStatus(int pid);
	int getHighestPcb(int prio = -1);

	bool insertReadyPcb(int pid);
	bool insertBlockPcb(int pid);

	bool removeReadyPcb(int pid);
	bool removeBlockPcb(int pid);

	bool comparePcbPriority(int pid1, int pid2);

	std::vector<PcbInfo> getTotalPcbInfo();

	int getPcbNumber();
	int getPrioMax();

private:
	void removePcbFromList(Pcb * pcb);
};
