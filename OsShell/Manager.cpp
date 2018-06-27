#include "Manager.h"

Manager::Manager(int pcbPrio)
	: pcbManager(pcbPrio), rcbManager()
{
	running = nullptr;
	int pid = pcbManager.createPcb(nullptr, "Manager Task", 0);
	Pcb * pcb = pcbManager.findPcbByPid(pid);
	pcb->changeStatus(ready);
	err = __manager_NO_ERR;
}

Manager::Manager(int pcbPrio, int rcbNum, int * resourceNumber, std::vector<std::string> nm)
	: pcbManager(pcbPrio), rcbManager(rcbNum, resourceNumber, nm)
{
	running = nullptr;
	int pid = pcbManager.createPcb(nullptr, "Manager Task", 0);
	Pcb * pcb = pcbManager.findPcbByPid(pid);
	pcb->changeStatus(ready);
	err = __manager_NO_ERR;
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
	// 检查优先级是否合法，prio=0的进程只能由系统创建，不能由用户创建
	if (prio <= 0) {
		err = __manager_PCB_PRIO_ERR;
		return -1;
	}
	else {
		int pid = pcbManager.createPcb(running, nm, prio);
		err = (pid > 0) ? __manager_NO_ERR : __manager_PCB_CREATE_FAIL;
		schedulForCreate();
		return pid;
	}
}

bool Manager::destroyPcb(int pid)
{
	switch (pcbManager.destroyPcb(pid)) {
	case __pcbManager_NO_ERR:
		err = __manager_NO_ERR;
		schedul();		// 进程删除成功后还需要重新调度
		return true;
	case __pcbManager_PID_INVAILD:
		err = __manager_PCB_PID_INVAILD;
		return false;
	case __pcbManager_PCB_NOTFOUND:
		err = __manager_PCB_NOT_FOUND;
		return false;
	case __pcbManager_PRIO_INVAILD:
		err = __manager_PCB_PRIO_ERR;
		return false;
	default:
		err = __manager_OP_ERR;
		return false;
	}
}

bool Manager::requestRcbForPcb(int rid, int num)
{
	Rcb * rcb = rcbManager.findRcb(rid);
	switch (running->requestRcb(rcb, num)) {
	case __rcb_NO_ERR:
		err = __manager_NO_ERR;
		return true;
	case __rcb_OUT_OF_RES:
		err = __manager_RES_OUT_TOTAL;
		return false;
	case __rcb_OUT_OF_VAL:
		err = __manager_RES_OUT_NUM;
	default:
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

bool Manager::releaseRcbForPcb(int rid, int num)
{
	Rcb * rcb = rcbManager.findRcb(rid);
	int res = running->releaseRcb(rcb, num);
	// schedul();
	switch (res) {
	case __rcb_OUT_OF_RES:
		err = __manager_RES_OUT_TOTAL;
		return false;
	case __rcb_NO_ERR:
		err = __manager_NO_ERR;
		return true;
	case __rcb_OUT_OF_HAVE:
		err = __manager_RES_OUT_HAVE;
		return false;
	case __rcb_NOT_FOUND:
		err = __manager_RES_NOT_FOUND;
		return false;
	default:
		err = __manager_OP_ERR;
		return false;
	}
}

bool Manager::releaseRcbForPcbByName(std::string nm, int num)
{
	int rid = searchRidByName(nm);
	if (rid == -1)
		return false;
	return releaseRcbForPcb(rid, num);
}

int Manager::findPcbByName(std::string nm)
{
	Pcb * pcb = pcbManager.findPcbByName(nm);
	if (pcb == nullptr) {
		err = __manager_PCB_NOT_FOUND;
		return -1;
	}
	err = __manager_NO_ERR;
	return pcb->getPid();
}

void Manager::addRcb(std::string nm, int rcbResNum)
{
	rcbManager.addResource(nm, rcbResNum);
}

int Manager::searchRidByName(std::string nm)
{
	Rcb * rcb = rcbManager.findRcb(nm);
	if (rcb == nullptr) {
		err = __manager_RES_NOT_FOUND;
		return -1;
	}
	else {
		err = __manager_NO_ERR;
		return rcb->getRid();
	}
}

std::vector<PcbInfo> Manager::getTotalPcbInfo()
{
	err = __manager_NO_ERR;
	return pcbManager.getTotalPcbInfo();
}

std::vector<RcbInfo> Manager::getTotalRcbInfo()
{
	err = __manager_NO_ERR;
	return rcbManager.getRcbInfoList();
}

int Manager::pauseManager()
{
	if (running == nullptr) {
		err = __manager_NO_RUNNING;
		return -1;
	}
	int pid = running->getPid();
	running->changeStatus(block);
	pcbManager.insertReadyPcb(running->getPid());
	running = nullptr;
	err = __manager_NO_ERR;
	return pid;
}

int Manager::schedul()
{
	// 将获得资源但未被唤醒的pcb加入就绪队列中
	updateReadyList();
	// 获取最高优先级的pid
	int pid = pcbManager.getHighestPcb();
	if (pid != -1) {
		// 如果当前运行的进程仍在运行
		if (running->getStatus() == run) {
			// 比对当前运行的进程优先级和刚取出的进程优先级，如果运行的进程优先级高则不进行调度
			if (!pcbManager.comparePcbPriority(running->getPid(), pid)) {
				running->changeStatus(ready);
				pcbManager.insertReadyPcb(running->getPid());
				running = pcbManager.findPcbByPid(pid);
				running->changeStatus(run);
				pcbManager.removeReadyPcb(pid);
			}
		}
		else {	// 当前运行的进程可能因为等待资源而被阻塞
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
	err = __manager_NO_ERR;
	return schedul();
}

void Manager::restart()
{
	Pcb * pcb = pcbManager.findPcbByPid(0);
	int * arr = pcb->getChilrendPid();
	for (int i = 0; i < pcb->getChildrenNum(); i++) {
		pcbManager.destroyPcb(arr[i]);
	}
	delete[] arr;
}

std::string Manager::getLatestErrStr()
{
	switch (err) {
	case __manager_NO_ERR:
		return "No error";
	case __manager_RES_OUT_TOTAL:
		return "Resource total number is not enough";
	case __manager_RES_OUT_NUM:
		return "Resource value number is not enough";
	case __manager_RES_OUT_HAVE:
		return "Apply too much resource";
	case __manager_RES_NOT_FOUND:
		return "Resource is not found";
	case __manager_PCB_CREATE_FAIL:
		return "Create pcb fail";
	case __manager_PCB_PRIO_ERR:
		return "Pcb's priority is not vaild";
	case __manager_PCB_NOT_FOUND:
		return "Pcb is not found";
	case __manager_PCB_PID_INVAILD:
		return "pid is not vaild";
	case __manager_NO_RUNNING:
		return "Manager is not running";
	case __manager_OP_ERR:
	default:
		return "Op error! Find the programmer";
	}
}

int Manager::getLatestErrCode()
{
	return err;
}

int Manager::prioMax()
{
	return pcbManager.getPrioMax();
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

int Manager::schedulForCreate()
{
	int pid = pcbManager.getHighestPcb();
	if (pid != -1) {
		// 比对当前运行的进程优先级和刚取出的进程优先级，如果运行的进程优先级高则不进行调度
		if (pcbManager.comparePcbPriority(pid, running->getPid())) {
			running->changeStatus(ready);
			pcbManager.insertReadyPcb(running->getPid());
			running = pcbManager.findPcbByPid(pid);
			running->changeStatus(run);
			pcbManager.removeReadyPcb(pid);
		}
	}
	return running->getPid();
}
