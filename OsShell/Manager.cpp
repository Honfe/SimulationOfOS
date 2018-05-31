#include "Manager.h"

Manager::Manager(int pcbPrio)
	: pcbManager(pcbPrio), rcbManager()
{
	running = nullptr;
	int pid = pcbManager.createPcb(nullptr, "Manager Task", 0);
	Pcb * pcb = pcbManager.findPcbByPid(pid);
	pcb->changeStatus(ready);
}

Manager::Manager(int pcbPrio, int rcbNum, int * resourceNumber, std::vector<std::string> nm)
	: pcbManager(pcbPrio), rcbManager(rcbNum, resourceNumber, nm)
{
	running = nullptr;
	int pid = pcbManager.createPcb(nullptr, "Manager Task", 0);
	Pcb * pcb = pcbManager.findPcbByPid(pid);
	pcb->changeStatus(ready);
}

Manager::~Manager()
{
	delete running;
}

int Manager::begin()
{
	int pid = pcbManager.getHighestPcb();
	if (pid == -1) 
		return -1;
	running = pcbManager.findPcbByPid(pid);
	running->changeStatus(run);
	pcbManager.removeReadyPcb(pid);
	return pid;
}

int Manager::createPcb(int prio, std::string nm)
{
	if (prio <= 0) 
		return -1;
	else 
		return pcbManager.createPcb(running, nm, prio);
}

bool Manager::destroyPcb(int pid)
{
	if (pcbManager.destroyPcb(pid)) {
		schedul();
		return true;
	}
	else {
		return false;
	}
}

bool Manager::requestRcbForPcb(int rid, int num)
{
	Rcb * rcb = rcbManager.findRcbByRid(rid);
	if (running->requestRcb(rcb, num)) {
		return true;
	}
	else {
		running->changeStatus(block);
		schedul();
		return false;
	}
}

bool Manager::requestRcbForPcbByName(std::string nm, int num)
{
	int rid = searchRidByName(nm);
	if (rid == -1)
		return false;
	return requestRcbForPcb(rid, num);
}

bool Manager::releaseRcbForPcb(int rid)
{
	return running->releaseRcb(rid);
}

bool Manager::releaseRcbForPcbByName(std::string nm)
{
	int rid = searchRidByName(nm);
	if (rid == -1)
		return false;
	return releaseRcbForPcb(rid);
}

int Manager::findPcbByName(std::string nm)
{
	Pcb * pcb = pcbManager.findPcbByName(nm);
	if (pcb == nullptr)
		return -1;
	return pcb->getPid();
}

void Manager::addRcb(std::string nm, int rcbResNum)
{
	rcbManager.addResource(nm, rcbResNum);
}

int Manager::searchRidByName(std::string nm)
{
	Rcb * rcb = rcbManager.findRcbByName(nm);
	if (rcb == nullptr) {
		return -1;
	}
	else {
		return rcb->getRid();
	}
}

std::vector<PcbInfo> Manager::getTotalPcbInfo()
{
	return pcbManager.getTotalPcbInfo();
}

std::vector<RcbInfo> Manager::getTotalRcbInfo()
{
	return rcbManager.getRcbInfoList();
}

int Manager::pauseManager()
{
	if (running == nullptr) {
		return -1;
	}
	int pid = running->getPid();
	running->changeStatus(block);
	pcbManager.insertReadyPcb(running->getPid());
	running = nullptr;
	return pid;
}

int Manager::schedul()
{
	updateReadyList();
	int pid = pcbManager.getHighestPcb(running->getPriority());
	if (pid != -1) {
		if (running->getStatus() == run) {
			if (!pcbManager.comparePcbPriority(running->getPid(), pid)) {
				running->changeStatus(ready);
				pcbManager.insertReadyPcb(running->getPid());
				running = pcbManager.findPcbByPid(pid);
				running->changeStatus(run);
				pcbManager.removeReadyPcb(pid);
			}
		}
		else {
			pcbManager.insertBlockPcb(running->getPid());
			running = pcbManager.findPcbByPid(pid);
			running->changeStatus(run);
			pcbManager.removeReadyPcb(pid);
		}
	}
	return running->getPid();
}

int Manager::timeOut()
{
	return schedul();
}

void Manager::updateReadyList()
{
	std::vector<Pcb*> list = rcbManager.getReadyPcb();
	for (int i = 0; i < list.size(); i++) {
		if (list[i]->getWaitingRcbNum() == 0) {
			list[i]->changeStatus(ready);
			pcbManager.removeBlockPcb(list[i]->getPid());
			pcbManager.insertReadyPcb(list[i]->getPid());
		}
	}
}


