#include <iostream>
#include <string>
using namespace std;
typedef struct PCB
{
	string name="";//进程标识符
	int prio_round=0;//进程优先数/进程轮转时间片数
	int cputime=0;//进程累计占用cpu的
	int needtime=0;
	string state="";
	PCB *next=NULL;
}PCB;
void  initlist(PCB* &L)//第一个节点为空
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
//查找ready队列尾指针
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
void readyadd(PCB* &ready, PCB* &tail,PCB pcb)//ready队列添加
{
	PCB* s;
	s = new PCB;
	*s = pcb;
	tail->next = s;
	tail = tail->next;
}

PCB checkHpfpcb(PCB* &ready,PCB* &run ,PCB* &tail)//查找优先级最高的任务,输入就绪队列头指针,run为要执行的任务
{
	PCB* s;
	s = ready->next;
	int prio=0;
	int xh = 0,xz=0;//记录循环次数，第几个被选中
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
	//删除选中任务
	s = ready;
	for (int i = 0; i < xz; i++)//指针到选中的前一个
	{
		s = s->next;
	}
	s->next = s->next->next;

	tailfind(ready, tail);
	//修改状态
	run->state = "R";
	return *run;
}
PCB checkTimepcb(PCB* &ready, PCB* &run, PCB* &tail)//查找队头任务,输入就绪队列头指针,run为要执行的任务
{
	PCB* s;
	s = ready->next;//s取值第一个数
	int prio = 0;
	run = s;//run指针取值s
	//删除选中任务
	s = ready;
	s->next = s->next->next;

	tailfind(ready, tail);
	//修改状态
	run->state = "R";
	return *run;
}
void intoready(PCB* &run,PCB* &tail)//执行队列任务进入就绪队列队尾
{
	run->state = "W";
	tail->next = run;
	//cout << tail->name << endl;
	tail = tail->next;
	//cout << tail->name << endl;
	run = NULL;
}
void intofinsh(PCB* &run ,PCB* &finish)//执行队列任务进入完成队列
{
	run->state = "F";
	run->next=finish->next;
	finish->next = run;
	run = NULL;
}

void runHpfpcb(PCB* &run ,PCB* &tail ,PCB* &finish)//任务在执行队列进行的计算。如果需要时间等于0，放入完成队列，否则放入就绪的队列队尾
{
	run->cputime += 1;
	run->needtime -= 1;
	run->prio_round -= 1;
	run->next = NULL;
	if (run->needtime <= 0)//如果需要时间小于等于0.放入完成队列
	{
		intofinsh(run,finish);
	}
	else//放入就绪队列队尾
	{
		intoready(run,tail);
	}
}
void runTimepcb(PCB* &run, PCB* &tail, PCB* &finish)//任务在执行队列进行的计算。如果需要时间等于0，放入完成队列，否则放入就绪的队列队尾
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
	if (run->needtime <= 0)//如果需要时间小于等于0.放入完成队列
	{
		intofinsh(run, finish);
	}
	else//放入就绪队列队尾
	{
		intoready(run, tail);
	}
}
void pout(PCB* &run)//输出run指针
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
void listout(PCB* &head)//输出链表
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
	PCB *run;//运行进程指针
	PCB *ready;//就绪队列头指针
	PCB *tail;//就绪队列尾指针
	PCB *finish;//完成队列头指针
	PCB pcb;
	PCB zxrw;//进入执行队列的任务
	int xz=0,num;//选择算法，输入数量
	initlist(ready);
	initlist(run);
	initlist(finish);
	tail = ready;
	cout << "选择算法(输入1或2)：1.优先权算法，2.时间片轮转"<<endl;
	cin >> xz;
	if(xz ==1 ||xz == 2 )
	{
		cout << "输入个数" << endl;
		cin >> num;
		cout << "依次输入名称和需要时间" << endl;
		if (xz == 1)
		{
			for (int i = 0; i < num; i++)
			{
				pcb = readyHpfInput();
				readyadd(ready, tail, pcb);//就绪队列添加任务
			}
			int cs = 0;
			while (true)
			{
				cs++;
				if (ready->next == NULL&&run == NULL)
				{
					break;
				}
				//循环
				cout << "第" << cs << "次初始" << endl;
				allout(ready, run, finish);
				checkHpfpcb(ready, run, tail);//查找优先级最高的任务
				cout << "第" << cs << "次执行前" << endl;
				allout(ready, run, finish);
				runHpfpcb(run, tail, finish);//对进入执行队列的任务进行操作
				cout << "第" << cs << "次执行后" << endl;
				allout(ready, run, finish);
				cout << "-------------------------------------------------------" << endl;
			}
		}
		else if (xz == 2)
		{
			for (int i = 0; i < num; i++)
			{
				pcb = readyTimeInput();
				readyadd(ready, tail, pcb);//就绪队列添加任务
			}
			int cs = 0;
			while (true)
			{
				cs++;
				if (ready->next == NULL&&run == NULL)
				{
					break;
				}
				//循环
				cout << "第" << cs << "次初始" << endl;
				allout(ready, run, finish);
				checkTimepcb(ready, run, tail);//查找优先级最高的任务
				cout << "第" << cs << "次执行前" << endl;
				allout(ready, run, finish);
				runTimepcb(run, tail, finish);//对进入执行队列的任务进行操作
				cout << "第" << cs << "次执行后" << endl;
				allout(ready, run, finish);
				cout << "-------------------------------------------------------" << endl;
			}
		}
		else
		{
			cout << "输入错误";
		}
	}
	else
	{
		cout << "输入错误" << endl;
		
	}
	
}
