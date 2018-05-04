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
	/*������Դ*/
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
	
		if (word[0] == "cr") {                          //��������
			if (word.size() != 3) {
				cout << "ȱ�ٲ������������" << endl;
			}
			stringstream temp(word[2]);
			int temp_priority;
			temp >> temp_priority;
			CreateProcess(word[1], temp_priority);
		}
		else if (word[0] == "take") {   
			if (word.size() != 2) {
				cout << "ȱ�ٲ������������" << endl;
			}
			else
				TakeProcess(word[1]);
		}
		else if (word[0] == "de") {
			if (word.size() != 2) {
				cout << "ȱ�ٲ������������" << endl;
			}
			DestroyProcess(word[1]);
		}
		else if (word[0] == "req") {
			if (word.size() != 3) {
				cout << "ȱ�ٲ������������" << endl;
			}
			RequestResource(word[1], word[2]);
		}
		else if (word[0] == "rel") {
			if (word.size() != 3) {
				cout << "ȱ�ٲ������������" << endl;
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
			cout << "�������" << endl;

		word.swap(vector<string>());     //���word����������
	}
}


void TestShell::CreateProcess(string pid, int priority) {
	PCB* that_pcb = new PCB(pid, priority);
	pcb_manager.push_back(that_pcb);   //���½�PCB��ӵ�PCB����
	if (ready_list.AddPcbToRL(that_pcb)) {      //���½�PCB��ӵ�ReadyList
		for (int i = 0; i < pcb_manager.size(); i++) {      //������������еĽ��̣����½�������ӵ����н��̵�children��
			if (pcb_manager[i]->GetType() == "running") {
				pcb_manager[i]->AddChildren(that_pcb);
				(*that_pcb).AddParent(pcb_manager[i]);     //�������еĽ�����ӵ��½����̵�parent��
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
		cout << "û�иý���" << endl;
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
		cout << "û�иý���" << endl;
	else {           
		/*ɾ���ý����ӽ���*/
		list<PCB*> temp = (*it)->GetChildren();
		list<PCB*>::iterator it2 = temp.begin();
		if (!temp.empty()) {
			for (it2; it2 != temp.end(); it2++) {
				DestroyProcess((*it2)->GetPid());
			}
		}
		/*ɾ��RL��BL�еĸý���*/
		if (ready_list.InRL(*it))
			ready_list.DelePcbFromRL(*it);
		else if (block_list.InBL(*it))
			block_list.DelePcbFromBL(*it);
		/*�ͷŸý��̵���Դ*/
		list<RCB*> temp2 = (*it)->GetResourceList();
		list<RCB*>::iterator it3 = temp2.begin();
		if (!temp2.empty()) {
			for (it3; it3 != temp2.end(); it3++) {
				ReleaseResource((*it)->GetPid(), (*it3)->GetRid());
			}
		}
		/*���ý��̴ӿ��ܴ�������Դwaiting_list�е�ָ��ɾ��*/
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
		cout << "û�иý���" << endl;
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
		cout << "û�и���Դ" << endl;
		return 0;
	}

	/*������Դ*/
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
		cout << "û�иý���" << endl;
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
		cout << "û�и���Դ" << endl;
		return 0;
	}

	/*�ͷ���Դ*/
	list<RCB*> temp = (*pit)->GetResourceList();
	list<RCB*>::iterator it = temp.begin();
	if (!temp.empty()) {
		for (it; it != temp.end(); it++) {
			bool find = false;
			if ((*it)->GetRid() == (*rit)->GetRid()) {
				/*��Դʣ����+1*/
				(*rit)->SetRemain((*rit)->GetRemain() + 1);
				/*������Դ�ӽ��̵�resource_list��ɾ��*/
				(*pit)->DeleRCB(*rit);
				cout << (*pit)->GetPid() + " release " + (*rit)->GetRid() + " success!" << endl;
				/*������Դ��blocked��Ϊallocated,�Ӵ���Դ��waiting list�в���Ȩ����ߵĽ��̲�����*/
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
