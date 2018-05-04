#include "RCB.h"



RCB::RCB(string rid, int number)
{
	this->rid = rid;
	this->number = number;
	remain = this->number;
}


RCB::~RCB()
{
}


string RCB::GetRid() {
	return this->rid;
}

int RCB::GetNumber() {
	return this->number;
}

int RCB::GetRemain() {
	return this->remain;
}

void RCB::SetRemain(int remain) {
	this->remain = remain;
}

string RCB::GetStatus() {
	return this->status;
}

void RCB::SetStatus() {
	if (status == "allocated")
		status = "blocked";
	else if (status == "blocked")
		status = "allocated";
}

void RCB::AddIntoWList(PCB* test) {
	waiting_list.push_back(test);
}

void RCB::DeleFromWList(PCB* test) {
	list<PCB*>::iterator it = waiting_list.begin();
	for (it; it != waiting_list.end(); it++) {
		if ((*it)->GetPid() == test->GetPid()) {
			waiting_list.erase(it);
			break;
		}
	}
}

list<PCB*> RCB::GetWaitingList() {
	return this->waiting_list;
}
