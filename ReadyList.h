#pragma once
#include <list>
#include <string>
#include "PCB.h"

using namespace std;

class ReadyList
{
private:
	list<PCB*> init;
	list<PCB*> user;
	list<PCB*> system;
	list<PCB*>::iterator it;

public:
	ReadyList();
	~ReadyList();

	int AddPcbToRL(PCB* test);
	int DelePcbFromRL(PCB* test);
	int InRL(PCB* test);
};

