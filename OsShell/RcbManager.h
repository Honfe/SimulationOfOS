/**
* FileName: RcbManager.h
* Author: ZiQin
* BuildDate: 2018-5-31
* Version: 1.0.0
*/

#pragma once

#include "RCB.h"
#include <vector>

/**
* 资源基本信息
*/
struct RcbInfo {
	int rid;
	std::string name;
	int validNum;
};

class RcbManager {
private:
	// 资源管理队列
	std::vector<Rcb*> rcbList;
	// 资源类型上限
	int rcbNumMax;

public:
	// 构造函数
	RcbManager();
	RcbManager(int rcbNum, int * rcbRscNum, const std::vector<std::string> & nm);
	~RcbManager();

	/**
	* 函数名：addResource
	* 功能：增加资源类型
	* 参数：
	*		nm：资源名
	*		rcbResourceNum：资源拥有的最大数量
	* 返回值：无
	*/
	void addResource(std::string nm = "Unknown", int rcbResourceNum = 1);
	
	/**
	* 函数名：findRcb
	* 功能：查找资源块
	* 参数：rid/nm：
	* 返回值：
	*/
	Rcb * findRcb(int rid);
	Rcb * findRcb(std::string nm);

	/**
	* 函数名：checkRcbVaild
	* 功能：检查rcb资源量有无剩余
	* 参数：rid：需要检查的rid
	* 返回值：true/false
	*/
	bool checkRcbVaild(int rid);

	/**
	* 函数名：getReadyPcb
	* 功能：获取已经取得资源但未被唤醒的pcb
	* 参数：无
	* 返回值：vector容器
	*/
	std::vector<Pcb*> getReadyPcb();

	/**
	* 函数名：getRcbInfoList
	* 功能：获取rcb基本信息
	* 参数：无
	* 返回值：vector容器
	*/
	std::vector<RcbInfo> getRcbInfoList();

	/**
	* 函数名：getRcbNumber
	* 功能：获取Rcb的类型总数
	* 参数：无
	* 返回值：rcb类型总数
	*/
	int getRcbNumber();
};
