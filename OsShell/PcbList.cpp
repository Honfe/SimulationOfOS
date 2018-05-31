#include "PcbList.h"

PcbList::PcbList(int prio)
{
	list = new std::vector<Pcb*>[prio];
	priorityMax = prio;
}

PcbList::~PcbList()
{
	delete[] list;
}

void PcbList::insertPcbList(Pcb * pcb)
{
	if (pcb == nullptr)
		return;
	if (pcb->getPriority() >= priorityMax) {
		return;
	}
	list[pcb->getPriority()].push_back(pcb);
}

Pcb * PcbList::findPcbByPid(int pid)
{
	if (pid < 0) {
		return nullptr;
	}
	for (int prio = 0; prio < priorityMax; prio++) {
		for (int i = 0; i < list[prio].size(); i++) {
			if (list[prio][i]->getPid() == pid) {
				return list[prio][i];
			}
		}
	}
	return nullptr;
}

Pcb * PcbList::findPcbByName(std::string nm)
{
	if (nm.size() == 0) 
		return nullptr;
	for (int prio = 0; prio < priorityMax; prio++) {
		for (int i = 0; i < list[prio].size(); i++) {
			if (list[prio][i]->getName() == nm) {
				return list[prio][i];
			}
		}
	}
	return nullptr;
}

bool PcbList::removePcbList(int pid)
{
	Pcb * pcb = findPcbByPid(pid);
	if (pcb == nullptr) return false;
	int prio = pcb->getPriority();
	for (std::vector<Pcb*>::iterator i = list[prio].begin(); i != list[prio].end(); i++) {
		if ((*i)->getPid() == pid) {
			list[prio].erase(i);
			break;
		}
	}
	return true;
}

int PcbList::getHighestPcb(int prio)
{
	for (int priority = priorityMax - 1; priority >= 0; priority--) {
		if (list[priority].size() == 0) {
			continue;
		}
		int pid = list[priority][0]->getPid();
		return pid;
	}
	return -1;
}

std::vector<PcbInfo> PcbList::getTotalPcbInfoInList()
{
	PcbInfo pcf;
	std::vector<PcbInfo> pcbInfoList;
	for (int prio = 0; prio < priorityMax; prio++) {
		for (int i = 0; i < list[prio].size(); i++) {
			pcf.name = list[prio][i]->getName();
			pcf.pid = list[prio][i]->getPid();
			int pcbStatus = list[prio][i]->getStatus();
			if (pcbStatus == run) {
				pcf.status = "run";
			}
			else if (pcbStatus == block) {
				pcf.status = "block";
			}
			else {
				pcf.status = "ready";
			}
			pcbInfoList.push_back(pcf);
		}
	}
	return pcbInfoList;
}

int PcbList::getPcbNumber()
{
	int total = 0;
	for (int i = 0; i < priorityMax; i++) {
		total += getPriorityPcbNumber(i);
	}
	return total;
}

int PcbList::getPriorityPcbNumber(int prio)
{
	return list[prio].size();
}
