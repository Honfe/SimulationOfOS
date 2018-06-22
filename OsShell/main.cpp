#include <iostream>
#include <cstring>
#include <vector>
#include <cstdio>
#include <map>
#include <cstdio>
#include <conio.h>
#include "Core.h"

#define ERR_ENTER "Command is wrong, Please reenter what you want~"

using namespace std;

enum {
	cr = 1,
	de,
	depid,
	req,
	reqrid,
	rel,
	relrid,
	to,
	listpcb,
	listrcb,
	shutdown,
	clear,
	sleep,
	help,
	version,
	restart
};

map<string, int> mapFunc = {
	{"-cr", cr}, {"-de", de}, {"-depid", depid}, {"-req", req},
	{"-reqrid", reqrid}, {"-rel", rel }, {"-relrid", relrid}, {"-to", to}	,
	{"-listpcb", listpcb}, {"-listrcb", listrcb}, {"-exit", shutdown},
	{"-clear", clear}, {"-sleep", sleep}, {"-help", help},
	{"-version", version}, {"-restart", restart}
};

std::vector<std::string> split(std::string str, std::string pattern);

void printSystemMsg(string msg);

bool str2num(const string & numStr, int & num);

bool checkCmdValid(const vector<string> & item, int num);

void createPcb(Core & core, vector<string> item);

void deletePcbByName(Core & core, vector<string> item);

void deletePcbByPid(Core & core, vector<string> item);

void requestRcbByName(Core & core, vector<string> item);

void requestRcbByRid(Core & core, vector<string> item);

void releaseRcbByName(Core & core, vector<string> item);

void releaseRcbByRid(Core & core, vector<string> item);

void timeOut(Core & core, vector<string> item);

void listAllPcb(Core & core, vector<string> item);

void listAllRcb(Core & core, vector<string> item);

void shutdownCore(Core & core, vector<string> item);

void clearScreen(Core & core, vector<string> item);

void sleepCore(Core & core, vector<string> item);

void restartCore(Core & core, vector<string> item);

void commandHelp(Core & core, vector<string> item);

void showCoreVersion(Core & core, vector<string> item);

int main()
{
	Core core(3);
	vector<string> nm;
	nm.push_back("Printer");
	nm.push_back("IO");
	nm.push_back("KeyBoard");
	nm.push_back("Disk");
	int num[4] = { 1, 2, 3, 4 };
	core.initial(4, num, nm);
	int pid = core.begin();
	cout << "---------" << core.getVersion() << " begin--------" << endl;
	cout << "pid = " << pid << " is running!" << endl << endl;
	string cmd;
	while (true) {
		if (!core.isRunning()) {
			printSystemMsg("OS is sleeping...\n");
			system("pause");
			core.begin();
		}
		cout << "ZiQin shell>";
		getline(cin, cmd);
		if (cmd.size() == 0) {
			continue;
		}
		vector<string> item = split(cmd, " ");
		switch (mapFunc[item[0]]) {
		case cr:
			createPcb(core, item);
			break;
		case de:
			deletePcbByName(core, item);
			break;
		case depid:
			deletePcbByPid(core, item);
			break;
		case req:
			requestRcbByName(core, item);
			break;
		case reqrid:
			requestRcbByRid(core, item);
			break;
		case rel:
			releaseRcbByName(core, item);
			break;
		case relrid:
			releaseRcbByRid(core, item);
			break;
		case to:
			timeOut(core, item);
			break;
		case listpcb:
			listAllPcb(core, item);
			break;
		case listrcb:
			listAllRcb(core, item);
			break;
		case clear:
			clearScreen(core, item);
			break;
		case sleep:
			sleepCore(core, item);
			break;
		case help:
			commandHelp(core, item);
			break;
		case version:
			showCoreVersion(core, item);
			break;
		case shutdown:
			shutdownCore(core, item);
			return 0;
		case restart:
			restartCore(core, item);
			break;
		default:
			printSystemMsg(ERR_ENTER);
		}
	}

	return 0;
}

std::vector<std::string> split(std::string str, std::string pattern)
{
	std::string::size_type pos;
	std::vector<std::string> result;
	str += pattern;//扩展字符串以方便操作
	int size = str.size();

	for (int i = 0; i<size; i++)
	{
		pos = str.find(pattern, i);
		if (pos<size)
		{
			std::string s = str.substr(i, pos - i);
			result.push_back(s);
			i = pos + pattern.size() - 1;
		}
	}
	return result;
}

void printSystemMsg(string msg)
{
	cout << msg << endl << endl;
}

bool str2num(const string & numStr, int & num)
{
	try {
		num = stoi(numStr);
		return true;
	}
	catch (...) {
		return false;
	}
}

bool checkCmdValid(const vector<string>& item, int num)
{
	return (item.size() == num) ? true: false;
}

void createPcb(Core & core, vector<string> item)
{
	if (!checkCmdValid(item, 3)) {
		printSystemMsg(ERR_ENTER);
		return;
	}
	int prio, pid;
	if (!str2num(item[2], prio)) {
		printSystemMsg(ERR_ENTER);
		return;
	}
	pid = core.createPcb(prio, item[1]);
	if (core.getManagerErrorCode() != __manager_NO_ERR) {
		printSystemMsg(core.getManagerErrorMsg());
	}
	else {
		cout << "Create pcb successful, pid = " << pid << endl << endl;
	}
}

void deletePcbByName(Core & core, vector<string> item)
{
	if (!checkCmdValid(item, 2)) {
		printSystemMsg(ERR_ENTER);
		return;
	}
	int pid = core.deletePcbByName(item[1]);
	if (pid == -1) {
		printSystemMsg(core.getManagerErrorMsg());
	}
	else {
		printSystemMsg("Delete pcb successful!");
	}
}

void deletePcbByPid(Core & core, vector<string> item)
{
	if (!checkCmdValid(item, 2)) {
		printSystemMsg(ERR_ENTER);
		return;
	}
	int pid;
	if (!str2num(item[1], pid)) {
		printSystemMsg(ERR_ENTER);
		return;
	}
	if (core.deletePcbByPid(pid)) {
		printSystemMsg("Delete pcb successful!");
	}
	else {
		printSystemMsg(core.getManagerErrorMsg());
	}
}

void requestRcbByName(Core & core, vector<string> item)
{
	if (!checkCmdValid(item, 3)) {
		printSystemMsg(ERR_ENTER);
		return;
	}
	int num;
	if (!str2num(item[2], num)) {
		printSystemMsg(ERR_ENTER);
		return;
	}
	if (core.requestRcb(item[1], num)) {
		printSystemMsg("Request rcb successful!");
	}
	else {
		printSystemMsg(core.getManagerErrorMsg());
	}
}

void requestRcbByRid(Core & core, vector<string> item)
{
	if (!checkCmdValid(item, 3)) {
		printSystemMsg(ERR_ENTER);
		return;
	}
	int rid, num;
	if (!(str2num(item[1], rid))) {
		printSystemMsg(ERR_ENTER);
		return;
	}
	if (!(str2num(item[2], num))) {
		printSystemMsg(ERR_ENTER);
		return;
	}
	if (core.requestRcb(rid, num)) {
		printSystemMsg("Request rcb successful!");
	}
	else {
		printSystemMsg(core.getManagerErrorMsg());
	}
}

void releaseRcbByName(Core & core, vector<string> item)
{
	if (!checkCmdValid(item, 3)) {
		printSystemMsg(ERR_ENTER);
		return;
	}
	int num;
	if (!str2num(item[2], num)) {
		printSystemMsg(ERR_ENTER);
		return;
	}
	if (core.releaseRcb(item[1], num)) {
		printSystemMsg("Release rcb successful!");
	}
	else {
		printSystemMsg(core.getManagerErrorMsg());
	}
}

void releaseRcbByRid(Core & core, vector<string> item)
{
	if (!checkCmdValid(item, 3)) {
		printSystemMsg(ERR_ENTER);
		return;
	}
	int rid;
	if (!str2num(item[1], rid)) {
		printSystemMsg(ERR_ENTER);
		return;
	}
	int num;
	if (!str2num(item[2], num)) {
		printSystemMsg(ERR_ENTER);
		return;
	}
	if (core.releaseRcb(rid, num)) {
		printSystemMsg("Release rcb successful!");
	}
	else {
		printSystemMsg(core.getManagerErrorMsg());
	}
}

void timeOut(Core & core, vector<string> item)
{
	if (!checkCmdValid(item, 1)) {
		printSystemMsg(ERR_ENTER);
		return;
	}
	int pid = core.timeOut();
	cout << "Now running pcb is pid = " << pid << endl << endl;
}

void listAllPcb(Core & core, vector<string> item)
{
	if (!checkCmdValid(item, 1)) {
		printSystemMsg(ERR_ENTER);
		return;
	}
	vector<PcbInfo> list = core.getTotalPcbInfo();
	cout << "--------------PCB LIST----------------" << endl;
	cout << "pid\tstatus\t\tname" << endl;
	for (int i = 0; i < list.size(); i++) {
		printf("%-8d%s\t\t%s\n", list[i].pid, list[i].status.data(), list[i].name.data());
	}
	cout << endl;
}

void listAllRcb(Core & core, vector<string> item)
{
	if (!checkCmdValid(item, 1)) {
		printSystemMsg(ERR_ENTER);
		return;
	}
	vector<RcbInfo> list = core.getTotalRcbInfo();
	cout << "--------------RCB LIST----------------" << endl;
	cout << "rid\tvaliable\tname" << endl;
	for (int i = 0; i < list.size(); i++) {
		printf("%-8d%-13d\t%s\n", list[i].rid, list[i].validNum, list[i].name.data());
	}
	cout << endl;
}

void shutdownCore(Core & core, vector<string> item)
{
	if (!checkCmdValid(item, 1)) {
		printSystemMsg(ERR_ENTER);
		return;
	}
	core.exit();
}

void clearScreen(Core & core, vector<string> item)
{
	system("cls");
}

void sleepCore(Core & core, vector<string> item)
{
	int pid = core.suspendCore();
	cout << "Running pcb has been stopped(pid = " << pid <<")" << endl;
}

void restartCore(Core & core, vector<string> item)
{
	core.restart();
	printSystemMsg("Core restart complete!");
}

void commandHelp(Core & core, vector<string> item)
{
	printSystemMsg("--------------------Command Help---------------------");
	printSystemMsg(" -cr <name> <priority=1-2>\tCreate a new pcb");
	printSystemMsg(" -de <name>\t\t\tDelete pcb by name");
	printSystemMsg(" -depid <pid>\t\t\tDelete pcb by pid");
	printSystemMsg(" -req <name> <number>\t\tRequest rcb by name");
	printSystemMsg(" -reqrid <rid> <number>\t\tRequest rcb by rid");
	printSystemMsg(" -rel <name>\t\t\tRelease rcb by name");
	printSystemMsg(" -relrid <rid> <number>\t\tRelease rcb by rid");
	printSystemMsg(" -to\t\t\t\tTime out");
	printSystemMsg(" -listpcb\t\t\tList all pcb");
	printSystemMsg(" -listrcb\t\t\tList all rcb");
	printSystemMsg(" -clear\t\t\t\tClear screen");
	printSystemMsg(" -version\t\t\tShow core version");
	printSystemMsg(" -sleep\t\t\t\tSleep core");
	printSystemMsg(" -exit\t\t\t\tExit core(shutdown)");
}

void showCoreVersion(Core & core, vector<string> item)
{
	printSystemMsg(core.getVersion());
}
