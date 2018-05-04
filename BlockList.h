#pragma once
#include <list>
#include <string>
#include "PCB.h"

class BlockList
{
private:
	list<PCB*> init;
	list<PCB*> user;
	list<PCB*> system;
	list<PCB*>::iterator it;
public:
	BlockList();
	~BlockList();

	int AddPcbToBL(PCB* test);
	int DelePcbFromBL(PCB* test);
	int InBL(PCB* test);
};

