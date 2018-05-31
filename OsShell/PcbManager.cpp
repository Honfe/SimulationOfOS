#include "PcbManager.h"

PcbManager::PcbManager(int prio)
	: totalList(prio), readyList(prio), blockList(prio)
{
	priorityMax = prio;
}

PcbManager::~PcbManager()
{
}

int PcbManager::createPcb(Pcb * par, std::string nm, int prio)
{
	Pcb * pcb = new Pcb(par, nm, prio);
	pcb->changeStatus(ready);
	pcb->changeList(&readyList);
	readyList.insertPcbList(pcb);
	totalList.insertPcbList(pcb);
	return pcb->getPid();
}

bool PcbManager::destroyPcb(int pid)
{
	if (pid < 0) 
		return false;
	Pcb * pcb = findPcbByPid(pid);
	if (pcb == nullptr)
		return false;
	if (pcb->getPriority() == 0)
		return false;
	std::vector<Pcb*> children = pcb->getChildrenPcb();
	for (int i = 0; i < children.size(); i++) {
		destroyPcb(children[i]->getPid());
	}
	int status = pcb->getStatus();
	if (status == ready) {
		readyList.removePcbList(pid);
	}
	else {
		blockList.removePcbList(pid);
	}
	totalList.removePcbList(pid);
	pcb->destroy();
	delete pcb;
	return true;
}

Pcb * PcbManager::findPcbByPid(int pid)
{
	if (pid < 0)
		return nullptr;
	Pcb * pcb = totalList.findPcbByPid(pid);
	if (pcb == nullptr)
		return nullptr;
	else
		return pcb;
}

Pcb * PcbManager::findPcbByName(std::string nm)
{
	return totalList.findPcbByName(nm);
}

int PcbManager::checkPcbStatus(int pid)
{
	Pcb * pcb = totalList.findPcbByPid(pid);
	if (pcb == nullptr)
		return -1;
	else
		return pcb->getStatus();
}

int PcbManager::getHighestPcb(int prio)
{
	return readyList.getHighestPcb(prio);
}

bool PcbManager::insertReadyPcb(int pid)
{
	Pcb * pcb = totalList.findPcbByPid(pid);
	if (pcb) {
		pcb->changeList(&readyList);
		pcb->changeStatus(ready);
		readyList.insertPcbList(pcb);
		return true;
	}
	else {
		return false;
	}
}

bool PcbManager::insertBlockPcb(int pid)
{
	Pcb * pcb = blockList.findPcbByPid(pid);
	if (pcb) {
		pcb->changeList(&blockList);
		pcb->changeStatus(block);
		blockList.insertPcbList(pcb);
		return true;
	}
	else {
		return false;
	}
}

bool PcbManager::removeReadyPcb(int pid)
{
	if (readyList.removePcbList(pid)) {
		return true;
	}
	else {
		return false;
	}
}

bool PcbManager::removeBlockPcb(int pid)
{
	if (blockList.removePcbList(pid)) {
		return true;
	}
	else {
		return false;
	}
}

bool PcbManager::comparePcbPriority(int pid1, int pid2)
{
	Pcb * pcb1 = findPcbByPid(pid1);
	Pcb * pcb2 = findPcbByPid(pid2);
	return pcb1->getPriority() > pcb2->getPriority();
}

std::vector<PcbInfo> PcbManager::getTotalPcbInfo()
{
	return totalList.getTotalPcbInfoInList();
}

int PcbManager::getPcbNumber()
{
	return totalList.getPcbNumber();
}

int PcbManager::getPrioMax()
{
	return priorityMax;
}
