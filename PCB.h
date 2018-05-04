#pragma once
#include <iostream>
#include <string>
#include <list>
#include "RCB.h"

using namespace std;
class RCB;

class PCB
{
private:
	string pid;
	list<RCB*> resourse_list;
	string type;
	PCB* parent = NULL;
	list<PCB *> children;
	int priority;
public:
	PCB(string pid, int priority);
	~PCB();

	void AddParent(PCB *parent);
	void AddChildren(PCB* children);
	void DeleteChildren(PCB* children);
	void AddRCB(RCB* test);
	void DeleRCB(RCB* test);

	int GetPriority();
	string GetPid();
	string GetType();
	void SetType(string type);
	string GetParent();
	list<PCB*> GetChildren();
	list<RCB*> GetResourceList();
};

