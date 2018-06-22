#include "RcbManager.h"

RcbManager::RcbManager()
{
	rcbNumMax = 0;
}

RcbManager::RcbManager(int rcbNum, int * rcbRscNum, const std::vector<std::string>&  nm)
{
	for (int i = 0; i < rcbNum; i++) {
		addResource(nm[i], rcbRscNum[i]);
	}
	rcbNumMax = rcbNum;
}

RcbManager::~RcbManager()
{
	for (int i = 0; i < rcbList.size(); i++) {
		delete rcbList[i];
	}
}

void RcbManager::addResource(std::string nm, int rcbResourceNum)
{
	Rcb * rcb = new Rcb(nm, rcbResourceNum);
	rcbList.push_back(rcb);
	++rcbNumMax;
}

Rcb * RcbManager::findRcb(int rid)
{
	if (rid < 0)
		return nullptr;
	for (int i = 0; i < rcbList.size(); i++) {
		if (rcbList[i]->getRid() == rid) {
			return rcbList[i];
		}
	}
	return nullptr;
}

Rcb * RcbManager::findRcb(std::string nm)
{
	for (int i = 0; i < rcbList.size(); i++) {
		if (rcbList[i]->getName() == nm) {
			return rcbList[i];
		}
	}
	return nullptr;
}

bool RcbManager::checkRcbValid(int rid)
{
	Rcb * rcb = findRcb(rid);
	if (rcb == nullptr)
		return false;
	if (rcb->getresNumber() > 0)
		return true;
	else
		return false;
}

std::vector<Pcb*> RcbManager::getReadyPcb()
{
	std::vector<Pcb*> readyPcbList;
	for (int i = 0; i < this->rcbNumMax; i++) {
		while (rcbList[i]->getReadyListSize()) {
			readyPcbList.push_back(rcbList[i]->getOneReadyPcb());
		}
	}
	return readyPcbList;
}

std::vector<RcbInfo> RcbManager::getRcbInfoList()
{
	RcbInfo rif;
	std::vector<RcbInfo> rcbInfo;
	for (int i = 0; i < rcbList.size(); i++) {
		rif.name = rcbList[i]->getName();
		rif.rid = rcbList[i]->getRid();
		rif.validNum = rcbList[i]->getresNumber();
		rcbInfo.push_back(rif);
	}
	return rcbInfo;
}

int RcbManager::getRcbNumber()
{
	return rcbNumMax;
}
