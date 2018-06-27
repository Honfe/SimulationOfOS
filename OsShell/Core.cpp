#include "Core.h"

Core::Core(int prio)
	: manager(prio)
{
	version = __CORE_VERSION;
	run = false;
	on = false;
}

Core::Core(int prio, int rcb, int * rcbNum, std::vector<std::string> nm)
	: manager(prio, rcb, rcbNum, nm)
{
	version = __CORE_VERSION;
	run = false;
	on = false;
}

Core::~Core()
{
}

bool Core::initial(int rcb, int * rcbResNum, std::vector<std::string> nm)
{
	on = true; // 假装打开了内核，并初始化系统所需要的资源
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

bool Core::releaseRcb(int rid, int num)
{
	return manager.releaseRcbForPcb(rid, num);
}

bool Core::releaseRcb(std::string nm, int num)
{
	return manager.releaseRcbForPcbByName(nm, num);
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

void Core::restart()
{
	exit();
	manager.restart();
	begin();
}

void Core::exit()
{
	run = false;
	on = false;
	// deletePcbByPid(0);
}

std::string Core::getVersion()
{
	return version;
}

bool Core::isRunning()
{
	return run;
}

bool Core::isOn()
{
	return on;
}

std::string Core::getManagerErrorMsg()
{
	return manager.getLatestErrStr();
}

int Core::getManagerErrorCode()
{
	return manager.getLatestErrCode();
}
