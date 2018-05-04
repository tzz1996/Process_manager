#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "ReadyList.h"
#include "RCB.h"
#include "BlockList.h"

using namespace std;

class TestShell
{
private:
	string command;
	vector<string> word;   //����������ո�ת��������Ͳ���
	vector<PCB*> pcb_manager;   //���ڲ����Ѵ�������
	vector<RCB*> rcb_manager;   //������Դ
	ReadyList ready_list;
	BlockList block_list;
public:
	TestShell();
	~TestShell();

	void Run();

	void CreateProcess(string pid, int priority);    //��������
	void TakeProcess(string pid);   //���Ƚ���ready -> running / running ->ready
	void TimeOut(string pid);   //ʱ���жϺ���running -> ready
	void DestroyProcess(string pid);   //���ٽ��̣������ٴ˽��̵������ӽ��̣���pcb_manager��ɾ��
	void LookProcess();   //�鿴���н���
	int RequestResource(string pid, string rid);  //������Դ
	int ReleaseResource(string pid, string rid);   //�ͷ���Դ
};

