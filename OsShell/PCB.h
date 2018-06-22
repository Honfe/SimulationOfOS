/**
* FileName: PCB.h
* Author: ZiQin
* BuildDate: 2018-6-22
* Version: 1.1.0
*/

#pragma once

#include <iostream>
#include <vector>
#include <string>
#include "RCB.h"
#include "PcbList.h"

class Rcb;
class PcbList;

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
	std::vector<Rcb *> usingRcb;
	unsigned char status;
	PcbList * listType;
	Pcb * parent;
	std::vector<Pcb *> children;
	int priority;
	static int id;

public:
	Pcb(Pcb * par, int prio, std::string nm = "Unknown");
	~Pcb();

	int requestRcb(Rcb *rcb, int num = 1);
	int releaseRcb(Rcb * rcb, int num = 1);

	std::vector<Pcb*> destroy();

	int getPid();
	std::string getName();

	void changeList(PcbList * list);
	PcbList * getList();

	int getParentPid();
	Pcb * getParentPcb();

	int * getChilrendPid();
	std::vector<Pcb *> getChilrenPcb();
	int getChildrenNum();

	int getPriority();

	unsigned char getStatus();
	void changeStatus(unsigned char status);

	int getWaitingRcbNum();

private:
	void addChild(Pcb * child);
	void deleteChild(Pcb * child);

	int findUsingRcb(Rcb * rcb);
};
