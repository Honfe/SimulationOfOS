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
	// 检查指针是否存在
	if (pcb == nullptr)
		return;
	// 检查优先级合理性
	if (pcb->getPriority() >= priorityMax) {
		return;
	}
	list[pcb->getPriority()].push_back(pcb);
}

Pcb * PcbList::findPcbByPid(int pid)
{
	// 检查pid是否合理
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
	// 检查名字是否合理
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
	int prio = pcb->getPriority();		// 获取优先级，方便后续查找
	for (std::vector<Pcb*>::iterator i = list[prio].begin(); i != list[prio].end(); i++) {
		if ((*i)->getPid() == pid) {
			list[prio].erase(i);
			break;
		}
	}
	return true;
}

int PcbList::getHighestPcb()
{
	// 由最高优先级开始往下遍历查找
	for (int priority = priorityMax - 1; priority >= 0; priority--) {
		if (list[priority].size() == 0) {	// 队列为空则直接跳过
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
