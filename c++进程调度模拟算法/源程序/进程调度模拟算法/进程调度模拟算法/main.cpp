#include <iostream>
#include <string>
using namespace std;
typedef struct PCB
{
	string name="";//���̱�ʶ��
	int prio_round=0;//����������/������תʱ��Ƭ��
	int cputime=0;//�����ۼ�ռ��cpu��
	int needtime=0;
	string state="";
	PCB *next=NULL;
}PCB;
void  initlist(PCB* &L)//��һ���ڵ�Ϊ��
{
	L = new PCB;
	L->next = NULL;
}
PCB readyHpfInput()
{
	PCB pcb;
	cin >> pcb.name;
	cin >> pcb.needtime;
	pcb.prio_round = 50 - pcb.needtime;
	pcb.state = "W";
	return pcb;
}
PCB readyTimeInput()
{
	PCB pcb;
	cin >> pcb.name;
	cin >> pcb.needtime;
	pcb.prio_round = 2;
	pcb.state = "W";
	return pcb;
}
//����ready����βָ��
void tailfind(PCB* &ready, PCB* &tail)
{
	PCB* s;
	s = ready;
	while (true)
	{
		if (s->next == NULL)
		{
			break;
		}
		s = s->next;
	}
	tail = s;
}
void readyadd(PCB* &ready, PCB* &tail,PCB pcb)//ready�������
{
	PCB* s;
	s = new PCB;
	*s = pcb;
	tail->next = s;
	tail = tail->next;
}

PCB checkHpfpcb(PCB* &ready,PCB* &run ,PCB* &tail)//�������ȼ���ߵ�����,�����������ͷָ��,runΪҪִ�е�����
{
	PCB* s;
	s = ready->next;
	int prio=0;
	int xh = 0,xz=0;//��¼ѭ���������ڼ�����ѡ��
	while (true)
	{
		if (s->prio_round >= prio)
		{
			prio = s->prio_round;
			run = s;
			xz = xh;
		}
		if (s->next==NULL)
		{
			break;
		}
		s = s->next;
		xh++;
	}
	//ɾ��ѡ������
	s = ready;
	for (int i = 0; i < xz; i++)//ָ�뵽ѡ�е�ǰһ��
	{
		s = s->next;
	}
	s->next = s->next->next;

	tailfind(ready, tail);
	//�޸�״̬
	run->state = "R";
	return *run;
}
PCB checkTimepcb(PCB* &ready, PCB* &run, PCB* &tail)//���Ҷ�ͷ����,�����������ͷָ��,runΪҪִ�е�����
{
	PCB* s;
	s = ready->next;//sȡֵ��һ����
	int prio = 0;
	run = s;//runָ��ȡֵs
	//ɾ��ѡ������
	s = ready;
	s->next = s->next->next;

	tailfind(ready, tail);
	//�޸�״̬
	run->state = "R";
	return *run;
}
void intoready(PCB* &run,PCB* &tail)//ִ�ж����������������ж�β
{
	run->state = "W";
	tail->next = run;
	//cout << tail->name << endl;
	tail = tail->next;
	//cout << tail->name << endl;
	run = NULL;
}
void intofinsh(PCB* &run ,PCB* &finish)//ִ�ж������������ɶ���
{
	run->state = "F";
	run->next=finish->next;
	finish->next = run;
	run = NULL;
}

void runHpfpcb(PCB* &run ,PCB* &tail ,PCB* &finish)//������ִ�ж��н��еļ��㡣�����Ҫʱ�����0��������ɶ��У������������Ķ��ж�β
{
	run->cputime += 1;
	run->needtime -= 1;
	run->prio_round -= 1;
	run->next = NULL;
	if (run->needtime <= 0)//�����Ҫʱ��С�ڵ���0.������ɶ���
	{
		intofinsh(run,finish);
	}
	else//����������ж�β
	{
		intoready(run,tail);
	}
}
void runTimepcb(PCB* &run, PCB* &tail, PCB* &finish)//������ִ�ж��н��еļ��㡣�����Ҫʱ�����0��������ɶ��У������������Ķ��ж�β
{
	for (int i = 0; i < run->prio_round; i++)
	{
		run->cputime += 1;
		run->needtime -= 1;
		if (run->needtime == 0)
		{
			break;
		}
	}
	run->next = NULL;
	if (run->needtime <= 0)//�����Ҫʱ��С�ڵ���0.������ɶ���
	{
		intofinsh(run, finish);
	}
	else//����������ж�β
	{
		intoready(run, tail);
	}
}
void pout(PCB* &run)//���runָ��
{
	PCB* s;
	if (run ==NULL)
	{

	}
	else if(run->name=="")
	{

	}
	else
	{
		s = run;
		cout << "name:" << s->name << ",cputime:" << s->cputime << ",needname:" << s->needtime << ",priority:" << s->prio_round << ",state:" << s->state << endl;

	}
}
void listout(PCB* &head)//�������
{
	PCB* s;
	s = head;
	while (true)
	{
		if (s->next==NULL)
		{
			break;
		}
		else if (s->next->name == "")
		{
			break;
		}
		else
		{
			s = s->next;
			cout << "name:" << s->name << ",cputime:" << s->cputime << ",needname:" << s->needtime << ",priority:" << s->prio_round << ",state:" << s->state << endl;
		}
	}
}
void allout(PCB* &ready, PCB* &run, PCB* &finish)
{
	listout(ready);
	pout(run);
	listout(finish);
}
void main() 
{
	PCB *run;//���н���ָ��
	PCB *ready;//��������ͷָ��
	PCB *tail;//��������βָ��
	PCB *finish;//��ɶ���ͷָ��
	PCB pcb;
	PCB zxrw;//����ִ�ж��е�����
	int xz=0,num;//ѡ���㷨����������
	initlist(ready);
	initlist(run);
	initlist(finish);
	tail = ready;
	cout << "ѡ���㷨(����1��2)��1.����Ȩ�㷨��2.ʱ��Ƭ��ת"<<endl;
	cin >> xz;
	if(xz ==1 ||xz == 2 )
	{
		cout << "�������" << endl;
		cin >> num;
		cout << "�����������ƺ���Ҫʱ��" << endl;
		if (xz == 1)
		{
			for (int i = 0; i < num; i++)
			{
				pcb = readyHpfInput();
				readyadd(ready, tail, pcb);//���������������
			}
			int cs = 0;
			while (true)
			{
				cs++;
				if (ready->next == NULL&&run == NULL)
				{
					break;
				}
				//ѭ��
				cout << "��" << cs << "�γ�ʼ" << endl;
				allout(ready, run, finish);
				checkHpfpcb(ready, run, tail);//�������ȼ���ߵ�����
				cout << "��" << cs << "��ִ��ǰ" << endl;
				allout(ready, run, finish);
				runHpfpcb(run, tail, finish);//�Խ���ִ�ж��е�������в���
				cout << "��" << cs << "��ִ�к�" << endl;
				allout(ready, run, finish);
				cout << "-------------------------------------------------------" << endl;
			}
		}
		else if (xz == 2)
		{
			for (int i = 0; i < num; i++)
			{
				pcb = readyTimeInput();
				readyadd(ready, tail, pcb);//���������������
			}
			int cs = 0;
			while (true)
			{
				cs++;
				if (ready->next == NULL&&run == NULL)
				{
					break;
				}
				//ѭ��
				cout << "��" << cs << "�γ�ʼ" << endl;
				allout(ready, run, finish);
				checkTimepcb(ready, run, tail);//�������ȼ���ߵ�����
				cout << "��" << cs << "��ִ��ǰ" << endl;
				allout(ready, run, finish);
				runTimepcb(run, tail, finish);//�Խ���ִ�ж��е�������в���
				cout << "��" << cs << "��ִ�к�" << endl;
				allout(ready, run, finish);
				cout << "-------------------------------------------------------" << endl;
			}
		}
		else
		{
			cout << "�������";
		}
	}
	else
	{
		cout << "�������" << endl;
		
	}
	
}
