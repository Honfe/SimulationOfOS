#pragma once

#include <iostream>
#include <vector>
#include <string>
#include "PCB.h"

class Pcb;

struct WaitingPcb {
	Pcb * pcb;
	int requestNum;
};

class Rcb {
private:
	int rid;
	std::string name;
	int validNumber;
	int validNumberTotal;
	std::vector<WaitingPcb> waitingPcb;
	std::vector<Pcb*> readyPcb;
	static int id;
public:
	Rcb(std::string nm = "Unknown", int num = 1);
	~Rcb();

	bool checkRequestRcb(int num = 1);
	bool requestRcb(Pcb * pcb, int num = 1);
	bool releaseRcb(Pcb * pcb, int num = 1);

	int getRid();
	int getValidNumber();
	int getValidTotal();
	int * getWaitingPid();
	Pcb * getOneReadyPcb();
	int getReadyListSize();
	std::string getName();
};
