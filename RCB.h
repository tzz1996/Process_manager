#pragma once
#include <iostream>
#include <string>
#include <list>
#include "PCB.h"

using namespace std;
class PCB;

class RCB
{
private:
	string rid;
	int number;     //资源总数
	int remain;      //可用资源数
	string status = "allocated";	     //资源状态
	list<PCB*> waiting_list;   //等待队列
public:
	RCB(string rid, int number);
	~RCB();

	string GetRid();
	int GetNumber();
	int GetRemain();
	void SetRemain(int remain);
	string GetStatus();
	void SetStatus();
	list<PCB*> GetWaitingList();

	void AddIntoWList(PCB* test);
	void DeleFromWList(PCB* test);
};

