#pragma once

#include "RCB.h"
#include <vector>

struct RcbInfo {
	int rid;
	std::string name;
	int validNum;
};

class RcbManager {
private:
	std::vector<Rcb*> rcbList;
	int rcbNumMax;
public:
	RcbManager();
	RcbManager(int rcbNum, int * rcbRscNum, const std::vector<std::string> & nm);
	~RcbManager();

	void addResource(std::string nm = "Unknown", int rcbResourceNum = 1);
	Rcb * findRcbByRid(int rid);
	Rcb * findRcbByName(std::string nm);
	bool checkRcbValid(int rid);
	std::vector<Pcb*> getReadyPcb();
	std::vector<RcbInfo> getRcbInfoList();

	int getRcbNumber();
};
