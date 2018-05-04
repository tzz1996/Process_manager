#include "ReadyList.h"



ReadyList::ReadyList()
{
}


ReadyList::~ReadyList()
{
}


int ReadyList::AddPcbToRL(PCB* test) {
	switch ((*test).GetPriority())
	{
	case 0:
		init.push_back(test);
		return 1;
	case 1:
		user.push_back(test);
		return 1;
	case 2:
		system.push_back(test);
		return 1;
	default:
		cout << "没有该权限" << endl;
		return 0;
	}
}

int ReadyList::DelePcbFromRL(PCB* test) {
	switch((*test).GetPriority()) {
	case 0:
		it = init.begin();
		for (it; it != init.end(); it++) {
			if ((*it)->GetPid() == (*test).GetPid()) {
				init.erase(it);
				return 1;
			}
		}
	case 1:
		it = user.begin();
		for (it; it != user.end(); it++) {
			if ((*it)->GetPid() == (*test).GetPid()) {
				user.erase(it);
				return 1;
			}
		}
	case 2:
		it = system.begin();
		for (it; it != system.end(); it++) {
			if ((*it)->GetPid() == (*test).GetPid()) {
				system.erase(it);
				return 1;
			}
		}
	default:
		cout << "没有该权限" << endl;
		return 0;
	}
}

int ReadyList::InRL(PCB* test) {
	switch ((*test).GetPriority())
	{
	case 0:
		it = init.begin();
		for (it; it != init.end(); it++) {
			if ((*it)->GetPid() == (*test).GetPid())
				return 1;
		}
	case 1:
		it = user.begin();
		for (it; it != user.end(); it++) {
			if ((*it)->GetPid() == (*test).GetPid())
				return 1;
		}
	case 2:
		it = system.begin();
		for (it; it != system.end(); it++) {
			if ((*it)->GetPid() == (*test).GetPid())
				return 1;
		}
	default:
		break;
	}
}
