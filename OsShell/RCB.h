/**
* FileName: RCB.h
* Author: ZiQin
* BuildDate: 2018-6-22
* Version: v2.0.0
*/

#pragma once

#include <iostream>
#include <vector>
#include <string>
#include "PCB.h"

#define __rcb_NO_ERR			 0		// 无错误
#define __rcb_OUT_OF_RES		-1		// 超出资源总量
#define __rcb_OUT_OF_VAL	-2		// 超出当前可用资源数量
#define __rcb_OUT_OF_HAVE	-5		// 超出当前PCB占有的资源量
#define __rcb_NOT_FOUND	-6		// 未找到资源

class Pcb;

/**
* 进程占用资源的记录
*/
struct WaitingPcb {
	Pcb * pcb;
	int requestNum;
};
typedef WaitingPcb UsingPcb;

class Rcb {
private:
	// RCB标识符
	int rid;
	// RCB名称
	std::string name;
	// rcb当前剩余可用资源数量
	int resNumber;
	// rcb总共拥有资源量
	int resNumberTotal;
	// 因为等待资源而被阻塞的pcb
	std::vector<WaitingPcb> waitingPcb;
	// 本来阻塞，最近获得资源的pcb
	std::vector<Pcb*> readyPcb;
	// 占用资源的pcb
	std::vector<UsingPcb> usingPcb;
	// 静态成员，为每一个新资源唯一标识一个id
	static int id;

public:
	// 构造函数
	Rcb(std::string nm = "Unknown", int num = 1);
	~Rcb();

	/**
	* 函数名：requestRcb
	* 功能：申请资源
	* 参数：
	*		pcb：申请资源的控制块
	*		num：申请的资源量
	* 返回值：状态码
	*/
	int requestRcb(Pcb * pcb, int num = 1);

	/**
	* 函数名：releaseRcb
	* 功能：释放资源
	* 参数：
	*		pcb：释放资源的控制块
	*		num：释放的资源量
	* 返回值：状态码
	*/
	int releaseRcb(Pcb * pcb, int num = 1);

	/**
	* 函数名：releaseAll
	* 功能：进程释放所有资源
	* 参数：pcb：要释放所有资源的进程
	* 返回值：无
	*/
	void releaseAll(Pcb * pcb);

	/**
	* 函数名：consultWait/UsingRcb
	* 功能：查询对应pcb等待/占用Rcb的资源量
	* 参数：pcb：要查询的pcb
	* 返回值：等待/占用的Rcb数量
	*/
	int consultWaitRcb(Pcb * pcb);
	int consultUsingRcb(Pcb * pcb);

	// 以下都是访问成员变量的方法

	int getRid();
	int getresNumber();
	int getResTotalNumber();
	Pcb * getOneReadyPcb();
	int getReadyListSize();
	std::string getName();

private:
	/**
	* 函数名：findWaitingListPcb
	* 功能：查询该pcb是否有等待/占用该资源
	* 参数：pcb：要查询的pcb
	* 返回值：等待/占用的资源数
	*/
	int findWaitingListPcb(Pcb * pcb);
	int findUsingListPcb(Pcb * pcb);

	/**
	* 函数名：updateReadyList
	* 功能：更新已经获得资源但未被唤醒的pcb列表
	* 参数：无
	* 返回值：无
	*/
	void updateReadyList();
};
