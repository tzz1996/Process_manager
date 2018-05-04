#include "PCB.h"



PCB::PCB(string pid, int priority)
{
	this->pid = pid;
	this->priority = priority;
	type = "ready";
}


PCB::~PCB()
{
}

void PCB::AddParent(PCB *parent) {
	this->parent = parent;
}

void PCB::AddChildren(PCB* children) {
	this->children.push_back(children);
}

void PCB::DeleteChildren(PCB* children) {
	this->children.remove(children);
}

int PCB::GetPriority() {
	return this->priority;
}

string PCB::GetPid() {
	return this->pid;
}

string PCB::GetType() {
	return this->type;
}

void PCB::SetType(string type) {
	this->type = type;
}

string PCB::GetParent() {
	if (parent == NULL)
		return "null";
	else
		return parent->GetPid();
}

list<PCB*> PCB::GetChildren() {
	return this->children;
}

void PCB::AddRCB(RCB* test) {
	resourse_list.push_back(test);
}

void PCB::DeleRCB(RCB* test) {
	list<RCB*>::iterator it = resourse_list.begin();
	for (it; it != resourse_list.end(); it++) {
		if ((*it)->GetRid() == test->GetRid()) {
			resourse_list.erase(it);
			break;
		}
	}
}

list<RCB*> PCB::GetResourceList() {
	return this->resourse_list;
}
