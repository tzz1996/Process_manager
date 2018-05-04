#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "ReadyList.h"
#include "RCB.h"
#include "BlockList.h"

using namespace std;

class TestShell
{
private:
	string command;
	vector<string> word;   //将输入命令按空格转换成命令和参数
	vector<PCB*> pcb_manager;   //用于查找已创建进程
	vector<RCB*> rcb_manager;   //管理资源
	ReadyList ready_list;
	BlockList block_list;
public:
	TestShell();
	~TestShell();

	void Run();

	void CreateProcess(string pid, int priority);    //创建进程
	void TakeProcess(string pid);   //调度进程ready -> running / running ->ready
	void TimeOut(string pid);   //时钟中断函数running -> ready
	void DestroyProcess(string pid);   //销毁进程，并销毁此进程的所有子进程，从pcb_manager中删除
	void LookProcess();   //查看所有进程
	int RequestResource(string pid, string rid);  //请求资源
	int ReleaseResource(string pid, string rid);   //释放资源
};

