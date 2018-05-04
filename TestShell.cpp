#include "TestShell.h"
#include "PCB.h"
#include "ReadyList.h"
#include "BlockList.h"
#include <sstream>

TestShell::TestShell()
{
}


TestShell::~TestShell()
{
}

void TestShell::Run() {
	/*创建资源*/
	RCB* r1 = new RCB("r1", 1);
	RCB* r2 = new RCB("r2", 5);
	RCB* r3 = new RCB("r3", 2);
	rcb_manager.push_back(r1);
	rcb_manager.push_back(r2);
	rcb_manager.push_back(r3);

	while (1) {
		cout << "tzzshell>";
		getline(cin, command);
		stringstream input(command);
		string result;
		while (input >> result)
			word.push_back(result);
	
		if (word[0] == "cr") {                          //创建进程
			if (word.size() != 3) {
				cout << "缺少参数或参数错误" << endl;
			}
			stringstream temp(word[2]);
			int temp_priority;
			temp >> temp_priority;
			CreateProcess(word[1], temp_priority);
		}
		else if (word[0] == "take") {   
			if (word.size() != 2) {
				cout << "缺少参数或参数错误" << endl;
			}
			else
				TakeProcess(word[1]);
		}
		else if (word[0] == "de") {
			if (word.size() != 2) {
				cout << "缺少参数或参数错误" << endl;
			}
			DestroyProcess(word[1]);
		}
		else if (word[0] == "req") {
			if (word.size() != 3) {
				cout << "缺少参数或参数错误" << endl;
			}
			RequestResource(word[1], word[2]);
		}
		else if (word[0] == "rel") {
			if (word.size() != 3) {
				cout << "缺少参数或参数错误" << endl;
			}
			ReleaseResource(word[1], word[2]);
		}
		else if (word[0] == "to") {
			TimeOut(word[1]);
		}
		else if (word[0] == "look") {
		
		}
		else if (word[0] == "quit" || word[0] == "q" || word[0] == "exit")
			break;
		else
			cout << "输入错误！" << endl;

		word.swap(vector<string>());     //清空word容器的内容
	}
}


void TestShell::CreateProcess(string pid, int priority) {
	PCB* that_pcb = new PCB(pid, priority);
	pcb_manager.push_back(that_pcb);   //将新建PCB添加到PCB容器
	if (ready_list.AddPcbToRL(that_pcb)) {      //将新建PCB添加到ReadyList
		for (int i = 0; i < pcb_manager.size(); i++) {      //如果有正在运行的进程，则将新建进程添加到运行进程的children里
			if (pcb_manager[i]->GetType() == "running") {
				pcb_manager[i]->AddChildren(that_pcb);
				(*that_pcb).AddParent(pcb_manager[i]);     //并将运行的进程添加到新建进程的parent里
			}
		}
		cout << "process " + pid + " is ready" << endl;
	}
}


void TestShell::TakeProcess(string pid) {
	bool find = false;
	for (int i = 0; i < pcb_manager.size(); i++) {
		if (pcb_manager[i]->GetPid() == pid) {
			if (pcb_manager[i]->GetType() == "running") {
				pcb_manager[i]->SetType("ready");
				cout << "process " + pid + " is ready now" << endl;
				find = true;
				break;
			}
			else if (pcb_manager[i]->GetType() == "ready") {
				pcb_manager[i]->SetType("running");
				cout << "process " + pid + " is running now" << endl;
				find = true;
				break;
			}
		}
	}
	if (!find)
		cout << "没有该进程" << endl;
}


void TestShell::TimeOut(string pid) {
	for (int i = 0; i < pcb_manager.size(); i++) {
		if (pcb_manager[i]->GetPid() == pid) {
			if (pcb_manager[i]->GetType() == "running") {
				pcb_manager[i]->SetType("ready");
				cout << "process " + pid + " is ready now" << endl;
			}
			else if (pcb_manager[i]->GetType() == "ready") {
				cout << "process " + pid + " is ready already" << endl;
			}
			else if (pcb_manager[i]->GetType() == "blocked") {
				cout << "process " + pid + " is blocked now" << endl;
			}
		}
	}
}


void TestShell::DestroyProcess(string pid) {
	bool find = false;
	vector<PCB*>::iterator it;
	it = pcb_manager.begin();
	for (it; it != pcb_manager.end(); it++) {
		if ((*it)->GetPid() == pid)
			find = true;
		if (find)
			break;
	}
	if (!find)
		cout << "没有该进程" << endl;
	else {           
		/*删除该进程子进程*/
		list<PCB*> temp = (*it)->GetChildren();
		list<PCB*>::iterator it2 = temp.begin();
		if (!temp.empty()) {
			for (it2; it2 != temp.end(); it2++) {
				DestroyProcess((*it2)->GetPid());
			}
		}
		/*删除RL或BL中的该进程*/
		if (ready_list.InRL(*it))
			ready_list.DelePcbFromRL(*it);
		else if (block_list.InBL(*it))
			block_list.DelePcbFromBL(*it);
		/*释放该进程的资源*/
		list<RCB*> temp2 = (*it)->GetResourceList();
		list<RCB*>::iterator it3 = temp2.begin();
		if (!temp2.empty()) {
			for (it3; it3 != temp2.end(); it3++) {
				ReleaseResource((*it)->GetPid(), (*it3)->GetRid());
			}
		}
		/*将该进程从可能存在于资源waiting_list中的指针删除*/
		vector<RCB*>::iterator it4 = rcb_manager.begin();
		for (it4; it4 != rcb_manager.end(); it4++) {
			list<PCB*> temp3 = (*it4)->GetWaitingList();
			list<PCB*>::iterator it5 = temp3.begin();
			if (!temp3.empty()) {
				for (it5; it5 != temp3.end(); it5++) {
					if ((*it5)->GetPid() == (*it)->GetPid()) {
						(*it4)->DeleFromWList(*it);
					}
				}
			}
		}
		delete *it;
		*it = NULL;
		pcb_manager.erase(it);
	}
}


int TestShell::RequestResource(string pid, string rid) {
	vector<PCB*>::iterator pit;
	bool exit_pid = false;
	pit = pcb_manager.begin();
	for (pit; pit != pcb_manager.end(); pit++) {
		if ((*pit)->GetPid() == pid) {
			exit_pid = true;
			break;
		}
	}
	if (!exit_pid) {
		cout << "没有该进程" << endl;
		return 0;
	}

	vector<RCB*>::iterator rit;
	bool exit_rid = false;
	rit = rcb_manager.begin();
	for (rit; rit != rcb_manager.end(); rit++) {
		if ((*rit)->GetRid() == rid) {
			exit_rid = true;
			break;
		}
	}
	if (!exit_rid) {
		cout << "没有该资源" << endl;
		return 0;
	}

	/*申请资源*/
	if ((*rit)->GetStatus() == "allocated") {
		(*pit)->AddRCB(*rit);
		(*rit)->SetRemain((*rit)->GetRemain() - 1);
		if ((*rit)->GetRemain() == 0)
			(*rit)->SetStatus();
		cout <<(*pit)->GetPid() + " request " + (*rit)->GetRid() + " success!" << endl;
	}
	else if ((*rit)->GetStatus() == "blocked") {
		(*rit)->AddIntoWList(*pit);
		if (ready_list.DelePcbFromRL(*pit))
			block_list.AddPcbToBL(*pit);
		cout << (*pit)->GetPid() + " request " + (*rit)->GetRid() + " failed! " + (*pit)->GetPid() + " is added to " + (*rit)->GetRid() + "'s waiting list!" << endl;
	}
}


int TestShell::ReleaseResource(string pid, string rid) {
	vector<PCB*>::iterator pit;
	bool exit_pid = false;
	pit = pcb_manager.begin();
	for (pit; pit != pcb_manager.end(); pit++) {
		if ((*pit)->GetPid() == pid) {
			exit_pid = true;
			break;
		}
	}
	if (!exit_pid) {
		cout << "没有该进程" << endl;
		return 0;
	}

	vector<RCB*>::iterator rit;
	bool exit_rid = false;
	rit = rcb_manager.begin();
	for (rit; rit != rcb_manager.end(); rit++) {
		if ((*rit)->GetRid() == rid) {
			exit_rid = true;
			break;
		}
	}
	if (!exit_rid) {
		cout << "没有该资源" << endl;
		return 0;
	}

	/*释放资源*/
	list<RCB*> temp = (*pit)->GetResourceList();
	list<RCB*>::iterator it = temp.begin();
	if (!temp.empty()) {
		for (it; it != temp.end(); it++) {
			bool find = false;
			if ((*it)->GetRid() == (*rit)->GetRid()) {
				/*资源剩余量+1*/
				(*rit)->SetRemain((*rit)->GetRemain() + 1);
				/*将此资源从进程的resource_list中删除*/
				(*pit)->DeleRCB(*rit);
				cout << (*pit)->GetPid() + " release " + (*rit)->GetRid() + " success!" << endl;
				/*若此资源从blocked变为allocated,从此资源的waiting list中查找权限最高的进程并给他*/
				if ((*rit)->GetRemain() == 1 && (*rit)->GetStatus() == "blocked") {
					(*rit)->SetStatus();
					list<PCB*> temp2 = (*rit)->GetWaitingList();
					list<PCB*>::iterator it2 = temp2.begin();
					if (!temp2.empty()) {
						PCB* biggist_process = *it2;
						for (it2; it2 != temp2.end(); it2++) {
							if ((*it2)->GetPriority() > biggist_process->GetPriority())
								biggist_process = *it2;
						}
						if (RequestResource(biggist_process->GetPid(), (*rit)->GetRid())) {
							(*rit)->DeleFromWList(biggist_process);
							if (block_list.DelePcbFromBL(*pit))
								ready_list.AddPcbToRL(*pit);
						}
					}
				}
				find = true;
			}
			if (find)
				break;
		}
	}
	else
		cout << (*pit)->GetPid() + " has no " + (*rit)->GetRid() + " resources!" << endl;
	return 1;
}
