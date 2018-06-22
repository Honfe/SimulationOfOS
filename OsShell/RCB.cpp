#include "RCB.h"

int Rcb::id = 0;

Rcb::Rcb(std::string nm, int num)
{
	this->name = nm;
	this->resNumberTotal = this->resNumber = num;
	this->rid = id++;
}

Rcb::~Rcb()
{
	// need to release rcb which pcb are using.
}

int Rcb::requestRcb(Pcb * pcb, int num)
{
	// 检查是否超出总量
	if (num > this->resNumberTotal)
		return __rcb_OUT_OF_RES;
	// 检查当前资源是否够用
	if (num > this->resNumber) {
		// 检查是否该pcb已经有使用该资源
		int pos = findWaitingListPcb(pcb);
		if (pos != -1) {
			waitingPcb[pos].requestNum += num;	// 直接增加原来占用数量
		}
		else {
			WaitingPcb wpcb{ pcb, num };
			waitingPcb.push_back(wpcb);		// 压入等待队列
		}
		return __rcb_OUT_OF_VAL;
	}
	else {
		// 检查是否该pcb已经有使用该资源
		int pos = findUsingListPcb(pcb);		
		if (pos != -1) {
			usingPcb[pos].requestNum += num;	// 直接增加原来占用数量
		}
		else {
			UsingPcb upcb{ pcb, num };
			usingPcb.push_back(upcb);			// 压入使用队列
		}
		this->resNumber -= num;
		return __rcb_NO_ERR;
	}
}

int Rcb::releaseRcb(Pcb * pcb, int num)
{
	// 检查请求释放量是否超出资源总量
	if (num > this->resNumberTotal) {
		return __rcb_OUT_OF_RES;
	}
	// 首先遍历等待列表
	int pos = findWaitingListPcb(pcb);
	if (pos != -1) {
		if (waitingPcb[pos].requestNum >= num) {
			waitingPcb[pos].requestNum -= num;
			// 如果等待队列中的pcb申请的资源被要求释放为0，则移出
			if (waitingPcb[pos].requestNum == 0) {
				waitingPcb.erase(waitingPcb.begin() + pos);
			}
			return __rcb_NO_ERR;
		}
		else {
			// 释放相应资源数目
			num -= waitingPcb[pos].requestNum;
			waitingPcb.erase(waitingPcb.begin() + pos);
		}
	}
	// 再遍历占有资源的列表
	pos = findUsingListPcb(pcb);
	if (pos != -1) {
		int usingNum = usingPcb[pos].requestNum;
		if (usingNum <= num) {		// 申请释放的量大于或等于实际占有量时，需要移除该pcb在using表
			this->resNumber += usingNum;
			usingPcb.erase(usingPcb.begin() + pos);
			updateReadyList();			// 由于释放了部分资源，此时应该及时让其他阻塞等待的进程获得资源并加入就绪队列中
			return  (usingNum == num) ? __rcb_NO_ERR : __rcb_OUT_OF_HAVE;
		}
		else {
			this->resNumber += num;
			usingPcb[pos].requestNum -= num;
			updateReadyList();			// 由于释放了部分资源，此时应该及时让其他阻塞等待的进程获得资源并加入就绪队列中
			return __rcb_NO_ERR;
		}
	}
	return __rcb_NOT_FOUND;
}

void Rcb::releaseAll(Pcb * pcb)
{
	int pos = findWaitingListPcb(pcb);
	if (pos != -1) {
		this->resNumber += waitingPcb[pos].requestNum;
		waitingPcb.erase(waitingPcb.begin() + pos);
	}
	pos = findUsingListPcb(pcb);
	if (pos != -1) {
		this->resNumber += usingPcb[pos].requestNum;
		usingPcb.erase(usingPcb.begin() + pos);
	}
	// 由于进程已经被删除，故假若进程在ready表中，也应该移除
	for (int i = 0; i < readyPcb.size(); i++) {
		if (readyPcb[i]->getPid() == pcb->getPid()) {
			readyPcb.erase(readyPcb.begin() + i);
			break;
		}
	}
	updateReadyList();
}

int Rcb::consultWaitRcb(Pcb * pcb)
{
	int pos = findWaitingListPcb(pcb);
	if (pos == -1) {
		return 0;
	}
	else {
		return waitingPcb[pos].requestNum;
	}
}

int Rcb::consultUsingRcb(Pcb * pcb)
{
	int pos = findUsingListPcb(pcb);
	if (pos == -1) {
		return 0;
	}
	else {
		return usingPcb[pos].requestNum;
	}
}

int Rcb::getRid()
{
	return rid;
}

int Rcb::getresNumber()
{
	return resNumber;
}

int Rcb::getResTotalNumber()
{
	return resNumberTotal;
}

Pcb * Rcb::getOneReadyPcb()
{	
	if (readyPcb.size() > 0) {
		Pcb * pcb = readyPcb[0];
		readyPcb.erase(readyPcb.begin());	// 删除已经取出的就绪pcb
		return pcb;
	}
	else {
		return nullptr;
	}
}

int Rcb::getReadyListSize()
{
	return readyPcb.size();
}

std::string Rcb::getName()
{
	return name;
}

int Rcb::findWaitingListPcb(Pcb * pcb)
{
	for (int i = 0; i < waitingPcb.size(); i++) {
		if (waitingPcb[i].pcb->getPid() == pcb->getPid()) {
			return i;
		}
	}
	return -1;
}

int Rcb::findUsingListPcb(Pcb * pcb)
{
	for (int i = 0; i < usingPcb.size(); i++) {
		if (usingPcb[i].pcb->getPid() == pcb->getPid()) {
			return i;
		}
	}
	return -1;
}

void Rcb::updateReadyList()
{
	// 遍历等待资源的pcb列表中是否有pcb可以使用资源
	for (int i = 0; i < waitingPcb.size(); i++) {
		if (this->resNumber >= waitingPcb[i].requestNum) {
			waitingPcb[i].pcb->requestRcb(this, waitingPcb[i].requestNum);	// 使pcb重新申请资源
			readyPcb.push_back(waitingPcb[i].pcb);
			waitingPcb.erase(waitingPcb.begin() + i);
			i = 0; // 由于vector容器在删除了其中一个元素后内部会重新组织，此时下标失效，因此需要重新开始遍历
		}
	}
}

