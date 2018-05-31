#include "PCB.h"

int Pcb::id = 0;

Pcb::Pcb(Pcb * par, std::string nm, int prio)
{
	this->pid = id++;
	this->name = nm;
	this->status = unknown;
	this->listType = nullptr;
	this->parent = par;
	this->priority = prio;
	if (par != nullptr)
		par->addChild(this);
}

Pcb::~Pcb()
{
}

bool Pcb::requestRcb(Rcb * targetRcb, int num)
{
	OccupyRcb rcb{ targetRcb, num };
	if (targetRcb->requestRcb(this, num)) {
		rcbList.push_back(rcb);
		return true;
	}
	else {
		rcbWaiting.push_back(rcb);
		status = block;
		return false;
	}
}

bool Pcb::releaseRcb(int rid)
{
	if (rcbList.size() == 0 && rcbWaiting.size() == 0) {
		return false;
	}
	for (int i = 0; i < rcbList.size(); i++) {
		if (rcbList[i].Rcb->getRid() == rid) {
			if (!rcbList[i].Rcb->releaseRcb(this, rcbList[i].requestNum))
				return  false;
			else {
				removeListRcb(rcbList[i].Rcb);
				return true;
			}
		}
	}
	for (int i = 0; i < rcbWaiting.size(); i++) {
		if (rcbWaiting[i].Rcb->getRid() == rid) {
			if (!rcbWaiting[i].Rcb->releaseRcb(this, rcbWaiting[i].requestNum))
				return false;
			else
				return true;
		}
	}
	return true;
}

bool Pcb::removeWaitingRcb(Rcb * rcb)
{
	for (std::vector<OccupyRcb>::iterator i = rcbWaiting.begin(); i != rcbWaiting.end(); i++) {
		if (i->Rcb->getRid() == rcb->getRid()) {
			OccupyRcb or = *i;
			rcbWaiting.erase(i);
			// rcbList.push_back(or);
			return true;
		}
	}
	return false;
}

bool Pcb::removeListRcb(Rcb * rcb)
{
	for (std::vector<OccupyRcb>::iterator i = rcbList.begin(); i != rcbList.end(); i++) {
		if (i->Rcb->getRid() == rcb->getRid()) {
			OccupyRcb or = *i;
			rcbList.erase(i);
			return true;
		}
	}
	return false;
}



void Pcb::destroy()
{
	// About pcb of chilren, I destroy it by Manager
	/**
	for (int i = 0; i < children.size(); i++) {
		children[i]->destroy();
	}
	children.clear();
	*/
	for (int i = 0; i < rcbList.size(); i++) {
		rcbList[i].Rcb->releaseRcb(this, rcbList[i].requestNum);
	}
	rcbList.clear();
	for (int i = 0; i < rcbWaiting.size(); i++) {
		rcbWaiting[i].Rcb->releaseRcb(this, 0);
	}
	rcbWaiting.clear();
	parent->deleteChild(this);
	parent = nullptr;
	status = none;
}

int Pcb::getPid()
{
	return pid;
}

std::string Pcb::getName()
{
	return this->name;
}

int * Pcb::getOccupyRidList()
{
	int size = rcbList.size();
	int * ridList = new int[size];
	for (int i = 0; i < size; i++) {
		ridList[i] = rcbList[i].Rcb->getRid();
	}
	return ridList;
}

unsigned char Pcb::getStatus()
{
	return status;
}

PcbList * Pcb::getListType()
{
	return listType;
}

int Pcb::getParentPid()
{
	return parent->getPid();
}

Pcb * Pcb::getParentPcb()
{
	return parent;
}

int * Pcb::getChildrenPid()
{
	return nullptr;
}

std::vector<Pcb*> Pcb::getChildrenPcb()
{
	return std::vector<Pcb*>(children);
}

int Pcb::getPriority()
{
	return priority;
}

void Pcb::changeStatus(int status)
{
	this->status = status;
}

void Pcb::changeList(PcbList * list)
{
	listType = list;
}

int Pcb::getWaitingRcbNum()
{
	return rcbWaiting.size();
}

void Pcb::addChild(Pcb * child)
{
	this->children.push_back(child);
}

void Pcb::deleteChild(Pcb * child)
{
	for (std::vector<Pcb*>::iterator i = children.begin(); i != children.end(); i++) {
		if ((*i)->getPid() == child->getPid()) {
			children.erase(i);
			return;
		}
	}
}
