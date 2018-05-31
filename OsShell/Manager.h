#pragma once

#include <iostream>
#include <vector>
#include <string>
#include "PCB.h"
#include "RCB.h"
#include "PcbManager.h"
#include "RcbManager.h"

class Manager {
private:
	PcbManager pcbManager;
	RcbManager rcbManager;
	Pcb * running;
public:
	Manager(int pcbPrio = 3);
	Manager(int pcbPrio, int rcbNum, int * resourceNumber, std::vector<std::string> nm);
	~Manager();

	int begin();

	int createPcb(int prio, std::string nm = "Unknown");

	bool destroyPcb(int pid);

	bool requestRcbForPcb(int rid, int num = 1);
	bool requestRcbForPcbByName(std::string nm, int num = 1);

	bool releaseRcbForPcb(int rid);
	bool releaseRcbForPcbByName(std::string nm);

	int findPcbByName(std::string nm);

	void addRcb(std::string nm = "Unknown", int rcbResNum = 1);

	int searchRidByName(std::string nm);

	std::vector<PcbInfo> getTotalPcbInfo();

	std::vector<RcbInfo> getTotalRcbInfo();

	int pauseManager();

	int schedul();

	int timeOut();

private:
	void updateReadyList();
};
