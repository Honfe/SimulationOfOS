#pragma once

#include "PCB.h"
#include "RCB.h"
#include <vector>
#include <string>

class Pcb;

struct PcbInfo {
	int pid;
	std::string name;
	std::string status;
};

class PcbList {
private:
	std::vector<Pcb*> * list;
	int priorityMax;
public:
	PcbList(int prio = 3);
	~PcbList();

	void insertPcbList(Pcb * pcb);
	Pcb * findPcbByPid(int pid);
	Pcb * findPcbByName(std::string nm);
	bool removePcbList(int pid);
	int getHighestPcb(int prio = -1);

	std::vector<PcbInfo> getTotalPcbInfoInList();

	int getPcbNumber();
	int getPriorityPcbNumber(int prio);
};
