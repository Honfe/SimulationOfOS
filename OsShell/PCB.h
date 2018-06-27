/**
* FileName: PCB.h
* Author: ZiQin
* BuildDate: 2018-6-22
* Version: 1.1.1
*/

#pragma once

#include <iostream>
#include <vector>
#include <string>
#include "RCB.h"
#include "PcbList.h"

class Rcb;
class PcbList;

/**
* 进程状态
*/
enum PcbStatus {
	run,
	block,
	ready,
	none,
	unknown
};

class Pcb {
private:
	// 进程描述符
	int pid;
	// 进程名字
	std::string name;
	// 进程占用或阻塞等待的资源块
	std::vector<Rcb *> usingRcb;
	// 进程状态
	unsigned char status;
	// 进程关联的状态队列
	PcbList * listType;
	// 父进程
	Pcb * parent;
	// 子进程
	std::vector<Pcb *> children;
	// 进程优先级
	int priority;
	// 静态成员，为每一个新进程唯一标识一个id
	static int id;

public:
	// 构造函数
	Pcb(Pcb * par, int prio, std::string nm = "Unknown");
	~Pcb();

	/**
	* 函数名：requestRcb 
	* 功能：申请rcb资源
	* 参数：
	*		rcb：想要申请的资源
	*		num：申请的资源数量
	* 返回值：状态码
	*/
	int requestRcb(Rcb *rcb, int num = 1);

	/**
	* 函数名：releaseRcb
	* 功能：释放rcb资源
	* 参数：
	*		rcb：要释放的资源
	*		num：释放的资源数量
	* 返回值：状态码
	*/
	int releaseRcb(Rcb * rcb, int num = 1);

	/**
	* 函数名：destroy
	* 功能：删除进程及其由该进程派生出来的所有子进程
	* 参数：无
	* 返回值：被删除进程的进程块（类似僵尸进程），交由manager进行回收
	*/
	std::vector<Pcb*> destroy();

	// 以下都是访问成员变量的方法

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
	/**
	* 函数名：addChild
	* 功能：添加子进程
	* 参数：child：子进程控制块
	* 返回值：无
	*/
	void addChild(Pcb * child);

	/**
	* 函数名：deleteChild
	* 功能：删除子进程
	* 参数：child：子进程控制块
	* 返回值：无
	*/
	void deleteChild(Pcb * child);

	/**
	* 函数名：findUsingRcb
	* 功能：在占用的资源列表中查找指定资源
	* 参数：rcb：要查找的资源
	* 返回值：rcb在列表中的位置下标，未找到则返回-1
	*/
	int findUsingRcb(Rcb * rcb);
};
