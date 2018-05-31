#pragma once

#include <iostream>
#include <vector>
#include <string>
#include "RCB.h"
#include "PcbList.h"

class Rcb;
class PcbList;

struct OccupyRcb {
	Rcb *Rcb;
	int requestNum;
};

enum PcbStatus {
	run,
	block,
	ready,
	none,
	unknown
};

class Pcb {
private:
	int pid;
	std::string name;
	std::vector<OccupyRcb> rcbList;
	std::vector<OccupyRcb> rcbWaiting;
	unsigned char status;
	PcbList * listType;
	Pcb * parent;
	std::vector<Pcb*> children;
	int priority;
	static int id;

public:
	Pcb(Pcb * par, std::string nm = "Unknown", int prio =	1);
	~Pcb();

	bool requestRcb(Rcb * targetRcb, int num = 1);
	bool releaseRcb(int rid);

	bool removeWaitingRcb(Rcb * rcb);
	bool removeListRcb(Rcb * rcb);

	void destroy();

	int getPid();
	std::string getName();
	int * getOccupyRidList();
	unsigned char getStatus();
	PcbList * getListType();
	int getParentPid();
	Pcb * getParentPcb();
	int * getChildrenPid();
	std::vector<Pcb*> getChildrenPcb();
	int getPriority();
	void changeStatus(int status);
	void changeList(PcbList * list);
	int getWaitingRcbNum();

private:
	void addChild(Pcb * child);
	void deleteChild(Pcb * child);
};
