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
	int number;     //��Դ����
	int remain;      //������Դ��
	string status = "allocated";	     //��Դ״̬
	list<PCB*> waiting_list;   //�ȴ�����
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

