#include "PCB.h"

int Pcb::id = 0;

Pcb::Pcb(Pcb * par, int prio, std::string nm)
{
	this->pid = id++;
	this->name = nm;
	this->priority = prio;
	this->status = ready;
	this->parent = par;
	// 排除根节点的情况，根节点没有父亲
	if (this->parent != nullptr)
		this->parent->addChild(this);
	listType = nullptr;
}

Pcb::~Pcb()
{
}

int Pcb::requestRcb(Rcb * rcb, int num)
{
	int res = rcb->requestRcb(this, num);
	if (res == __rcb_NO_ERR) {
		if (findUsingRcb(rcb) == -1) {
			usingRcb.push_back(rcb);
		}
	}
	return res;
}

int Pcb::releaseRcb(Rcb * rcb, int num)
{
	int res = rcb->releaseRcb(this, num);
	switch (res) {
	case __rcb_NO_ERR:
	case __rcb_OUT_OF_HAVE:
		if ((rcb->consultUsingRcb(this) + rcb->consultWaitRcb(this)) <= 0) {
			usingRcb.erase(usingRcb.begin() + findUsingRcb(rcb));
		}
		break;
	case __rcb_NOT_FOUND:
	case __rcb_OUT_OF_RES:
	default:
		;
	}
	return res;
}

std::vector<Pcb*> Pcb::destroy()
{
	// 进程被删除前，释放所有资源
	for (int i = 0; i < usingRcb.size(); i++) {
		usingRcb[i]->releaseAll(this);
	}
	std::vector<Pcb*> ch;
	// 自杀
	ch.push_back(this);
	// 父进程结束，子进程跟着结束
	for (int i = 0; i < children.size(); i++) {
		std::vector<Pcb*> temp = children[i]->destroy();
		ch.insert(ch.end(), temp.begin(), temp.end());
	}
	// 通知父进程自己挂掉了
	this->parent->deleteChild(this);
	return ch;
}

int Pcb::getPid()
{
	return this->pid;
}

std::string Pcb::getName()
{
	return this->name;
}

void Pcb::changeList(PcbList * list)
{
	this->listType = list;
}

PcbList * Pcb::getList()
{
	return this->listType;
}

int Pcb::getParentPid()
{
	return this->parent->getPid();
}

Pcb * Pcb::getParentPcb()
{
	return this->parent;
}

int * Pcb::getChilrendPid()
{
	int * childrenPid = new int[this->children.size()];
	for (int i = 0; i < children.size(); i++) {
		childrenPid[i] = children[i]->getPid();
	}
	return childrenPid;
}

std::vector<Pcb*> Pcb::getChilrenPcb()
{
	std::vector<Pcb*> chPcb;
	for (int i = 0; i < this->children.size(); i++) {
		chPcb.push_back(this->children[i]);
	}
	return chPcb;
}

int Pcb::getChildrenNum()
{
	return this->children.size();
}

int Pcb::getPriority()
{
	return this->priority;
}

unsigned char Pcb::getStatus()
{
	return this->status;
}

void Pcb::changeStatus(unsigned char status)
{
	this->status = status;
}

int Pcb::getWaitingRcbNum()
{
	int num = 0;
	for (int i = 0; i < usingRcb.size(); i++) {
		if (usingRcb[i]->consultWaitRcb(this) > 0) {
			++num;
		}
	}
	return num;
}

void Pcb::addChild(Pcb * child)
{
	this->children.push_back(child);
}

void Pcb::deleteChild(Pcb * child)
{
	for (std::vector<Pcb*>::iterator i = children.begin(); i != children.end(); i++) {
		if ((*i)->getPid() == child->getPid()) {
			this->children.erase(i);
			break;
		}
	}
}

int Pcb::findUsingRcb(Rcb * rcb)
{
	for (int i = 0; i < usingRcb.size(); i++) {
		if (rcb->getRid() == usingRcb[i]->getRid())
			return i;
	}
	return -1;
}
