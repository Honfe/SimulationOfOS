#include "RCB.h"

int Rcb::id = 0;

Rcb::Rcb(std::string nm, int num)
{
	this->rid = id++;
	this->name = nm;
	this->validNumber = num;
	this->validNumberTotal = this->validNumber;
}

Rcb::~Rcb()
{
}

bool Rcb::checkRequestRcb(int num)
{
	if (this->validNumber - num > 0) {
		return true;
	}
	else {
		return false;
	}
}

bool Rcb::requestRcb(Pcb * pcb, int num)
{
	if (num < 0 || num > this->validNumberTotal) 
		return false;
	if (this->validNumber - num >= 0) {
		this->validNumber -= num;
		// load resource to pcb
		return true;
	}
	else {
		WaitingPcb wp{ pcb, num };
		waitingPcb.push_back(wp);
		return false;
	}
}

bool Rcb::releaseRcb(Pcb * pcb, int num)
{
	if (num < 0)
		return false;
	if (this->validNumber + num > this->validNumberTotal)
		return false;
	if (num == 0) {
		for (std::vector<WaitingPcb>::iterator i = waitingPcb.begin(); i != waitingPcb.end(); i++) {
			if ((*i).pcb->getPid() == pcb->getPid()) {
				waitingPcb.erase(i);
				break;
			}
		}
	}
	else {
		this->validNumber += num;
		if (!this->waitingPcb.empty()) {
			for (std::vector<WaitingPcb>::iterator i = waitingPcb.begin(); i != waitingPcb.end(); i++) {
				if (checkRequestRcb(i->requestNum)) {
					i->pcb->removeWaitingRcb(this);
					i->pcb->requestRcb(this, i->requestNum);
					readyPcb.push_back(i->pcb);
					waitingPcb.erase(i);
					break;
				}
			}
		}
	}
	return true;
}

int Rcb::getRid()
{
	return this->rid;
}

int Rcb::getValidNumber()
{
	return this->validNumber;
}

int Rcb::getValidTotal()
{
	return this->validNumberTotal;
}

int * Rcb::getWaitingPid()
{
	int size = this->waitingPcb.size();
	int *pidList = new int[size];
	for (int i = 0; i < this->waitingPcb.size(); i++) {
		// registor pid of pcb of waiting resources to waitingList
	}
	return pidList;
}

Pcb * Rcb::getOneReadyPcb()
{
	if (readyPcb.size() > 0) {
		Pcb * rdyPcb = readyPcb[0];
		readyPcb.erase(readyPcb.begin());
		return rdyPcb;
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
	return this->name;
}


