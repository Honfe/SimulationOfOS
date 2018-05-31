#include "Core.h"

Core::Core(int prio, std::string ver)
	: manager(prio)
{
	version = ver;
	run = false;
}

Core::Core(int prio, int rcb, int * rcbNum, std::vector<std::string> nm, std::string ver)
	: manager(prio, rcb, rcbNum, nm)
{
	version = ver;
	run = false;
}

Core::~Core()
{
}

bool Core::initial(int rcb, int * rcbResNum, std::vector<std::string> nm)
{
	for (int i = 0; i < rcb; i++) {
		manager.addRcb(nm[i], rcbResNum[i]);
	}
	return true;
}

int Core::begin()
{
	if (run == false) {
		run = true;
		return manager.begin();
	}
	else {
		return -2;
	}
}

int Core::createPcb(int prio, std::string nm)
{
	 return manager.createPcb(prio, nm);
}

bool Core::deletePcbByPid(int pid)
{
	return manager.destroyPcb(pid);
}

bool Core::deletePcbByName(std::string nm)
{
	int pid = manager.findPcbByName(nm);
	if (pid == -1)
		return false;
	return manager.destroyPcb(pid);
}

void Core::addRcb(std::string nm, int rcbResNum)
{
	manager.addRcb(nm, rcbResNum);
}

bool Core::requestRcb(int rid, int num)
{
	if (manager.requestRcbForPcb(rid, num)) {
		return true;
	}
	else {
		return false;
	}
}

bool Core::requestRcb(std::string nm, int num)
{
	return manager.requestRcbForPcbByName(nm, num);
}

bool Core::releaseRcb(int rid)
{
	return manager.releaseRcbForPcb(rid);
}

bool Core::releaseRcb(std::string nm)
{
	return manager.releaseRcbForPcbByName(nm);
}

int Core::suspendCore()
{
	if (run) {
		run = false;
		return manager.pauseManager();
	}
	else {
		return -2;
	}
}

std::vector<PcbInfo> Core::getTotalPcbInfo()
{
	return manager.getTotalPcbInfo();
}

std::vector<RcbInfo> Core::getTotalRcbInfo()
{
	return manager.getTotalRcbInfo();
}

int Core::timeOut()
{
	return manager.timeOut();
}

void Core::exit()
{
	
}

std::string Core::getVersion()
{
	return version;
}

bool Core::isRunning()
{
	return run;
}
