#pragma once

#include "Manager.h"
#include <iostream>
#include <string>

#define __CORE_VERSION	"ZiQin Core V1.1"

class Core {
private:
	Manager manager;
	std::string version;
	bool run;
	bool on;

public:
	Core(int prio = 3);
	Core(int prio, int rcb, int *rcbNum, std::vector<std::string> nm);
	~Core();

	bool initial(int rcb, int * rcbResNum, std::vector<std::string> nm);

	int begin();

	int createPcb(int prio = 1, std::string nm = "Unknown");

	bool deletePcbByPid(int pid);
	bool deletePcbByName(std::string nm);

	void addRcb(std::string nm = "Unknown", int rcbResNum = 1);

	bool requestRcb(int rid, int num = 1);
	bool requestRcb(std::string nm, int num = 1);

	bool releaseRcb(int rid, int num = 1);
	bool releaseRcb(std::string nm, int num = 1);

	int suspendCore();

	std::vector<PcbInfo> getTotalPcbInfo();
	std::vector<RcbInfo> getTotalRcbInfo();

	int timeOut();

	void restart();

	void exit();

	std::string getVersion();

	bool isRunning();

	bool isOn();

	std::string getManagerErrorMsg();
	int getManagerErrorCode();

};
