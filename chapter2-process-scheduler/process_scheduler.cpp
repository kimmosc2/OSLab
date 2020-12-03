#include <stdio.h>
#include <dos.h>
#include <stdlib.h>
#include <conio.h>
#include <iostream>
#include <windows.h>
#include <iomanip>

// 进程数量
#define P_NUM 5
// 优先数(在优先级调度模型下,进程优先级等于P_TIME - 所需时间,所以如果进程所需时间过长,会导致
// priority为负数,然后程序则永远不会执行,更改P_STEP为1可以一定程度上减小这种情况,然而在P_TIME
// 固定的情况下,这种问题依旧会发生,除非更改算法)
#define P_TIME 50
// 优先级降低步长(老师给的3)
#define P_STEP 1

using namespace std;

//定义进程状态
enum state
{
    ready,
    execute,
    block,
    finish
};

// 定义进程PCB
struct pcb
{
    char name[4];  // 进程名
    int priority;  // 优先权
    int cputime;   // CPU运行时间
    int needtime;  // 进程运行所需时间
    int count;     // 进程执行次数
    int round;     // 时间片轮转轮次
    state process; // 进程状态
    pcb *next;
};

// 创建进程
pcb *getprocess();

// 显示pcb信息(从传入的pcb开始一直到结束,是个链表)
void display(pcb *p);

// 进程是否完成
int process_finish(pcb *q);

// 显示菜单
void display_menu();

//计算进程优先级
void prioritycal();

//时间片轮转调度算法输出调度信息
void round_cal();

//设置队列中进程执行状态
void setstate(pcb *p);

//获取下一个进程
pcb *get_next(pcb *k, pcb *head);

//采用时间片轮转调度算法执行某一进程
void cpu_round(pcb *q);

//时间片轮转调度算法创建就绪进程队列
pcb *getprocess_round();

//显示调度算法菜单，可供用户选择优先权调度算法和时间片轮转调度算法
void display_menu();

//选择某一进程，给它分配CPU
void cpuexe(pcb *q);

//结束进程，即将队列中各进程的所需时间设置为0
int process_finish(pcb *q);

int main()
{
#if defined(_WIN16) || defined(_WIN32) || defined(_WIN64)
    // change code page，use utf-8
    system("chcp 65001");
#endif
    display_menu();
    int k;
    scanf("%d", &k);
    switch (k)
    {
    case 1:
        prioritycal();
        break;
    case 2:
        round_cal();
        break;
    case 3:
        break;
    }
    return 0;
}

// gerprocess 创建P_NUM个进程,他们是链式的,
// 所以返回第一个进程,这个进程之后连接着其他进程
pcb *getprocess()
{
    // 创建的进程
    pcb *q;
    // 临时变量,用于迭代赋值
    pcb *t;
    // 返回的进程,即链表的首个节点
    pcb *p;
    int i = 0;

    cout << "input name and time" << endl;
    while (i < P_NUM)
    {
        q = (struct pcb *)malloc(sizeof(pcb));
        cin >> q->name;
        cin >> q->needtime;
        q->cputime = 0;
        // 进程优先级 = 优先数 - 该进程所需的时间(所需时间越少,优先级越高)
        q->priority = P_TIME - q->needtime,
        q->process = ready;
        q->next = NULL;
        // 首次赋值
        if (i == 0)
        {
            // 链表头确定并不再更改
            p = q;
            // 临时变量更改
            t = q;
        }
        else
        {
            t->next = q; //创建就绪进程队列
            t = q;
        }
        i++;
    }
    return p;
}

// 显示进程信息
void display(pcb *p)
{
    cout << "name"
         << " | "
         << "cputime"
         << " | "
         << "needtime"
         << " | "
         << "priority"
         << " | "
         << "state   |" << endl;
    while (p)
    {
        cout << setw(4) << p->name;
        cout << " | ";
        cout << setw(7) << p->cputime;
        cout << " | ";
        cout << setw(8) << p->needtime;
        cout << " | ";
        cout << setw(8) << p->priority;
        cout << " | ";
        switch (p->process)
        {
        case ready:
            cout << "ready"
                 << "   |" << endl;
            break;
        case execute:
            cout << "execute"
                 << " |" << endl;
            break;
        case block:
            cout << "block"
                 << "  |" << endl;
            break;
        case finish:
            cout << "finish"
                 << "  |" << endl;
            break;
        }
        p = p->next;
    } //显示模拟结果，包含进程名、CPU时间、运行所需时间以及优先级
}

// process_finish 返回是否进程链内的所有进程都已执行完毕
// 通过判断needtime而非process,如果还有未完成的则返回0,否则返回1
int process_finish(pcb *q)
{
    int bl = 1;
    while (bl && q)
    {
        // 判断当前进程的needtime是否为0，如果为0则判断下一个,否则退出循环
        bl = bl && q->needtime == 0;
        q = q->next;
    }
    return bl;
}

// cpuexe 选择某一进程，给它分配CPU
void cpuexe(pcb *q)
{
    // t就是要执行的进程,下面的while会遍历进程链表,然后选择一个优先级最高的进程
    // 将其赋值给t,然后执行t
    pcb *t = q;
    int tp = 0;
    while (q)
    {
        // 如果进程状态非完成状态,则将其更改为就绪态
        if (q->process != finish)
        {
            q->process = ready;
            // 如果所需时间为0,则直接完成
            if (q->needtime == 0)
            {
                q->process = finish;
            }
        }

        // 如果当前进程的优先级>tp,并且q的状态不等于finish
        // 则将tp设置为当前进程的优先级,并更改t为当前进程
        // 这部分算法会选择进程链表中优先级最高的进程，将其赋值给
        // t,并执行它
        if (tp < q->priority && q->process != finish)
        {
            tp = q->priority;
            t = q;
        }
        q = q->next;
    }
    // 模拟执行
    if (t->needtime > 0)
    {
        // 降低优先级
        t->priority -= P_STEP;
        // 减少需要时间
        t->needtime--;
        // 更改进程状态
        t->process = execute;
        // cpu执行时间增加
        t->cputime++;
    }
}

// 计算进程优先级
void prioritycal()
{
    pcb *p;
    system("cls");

    // p为进程链表的头结点
    p = getprocess();
    int cpu = 0;
    // 如果还有进程没有完成
    while (!process_finish(p))
    {
        system("cls");
        cpu++;
        cout << "\ncputime:" << cpu << endl;
        cpuexe(p);
        display(p);
        Sleep(500);
    }
    printf("All processes have finished,press any key to exit");
    getch();
}

//显示调度算法菜单，可供用户选择优先权调度算法和时间片轮转调度算法
void display_menu()
{
    cout << "CHOOSE THE ALGORITHM:" << endl;
    cout << "1 PRIORITY" << endl;
    cout << "2 ROUNDROBIN" << endl;
    cout << "3 EXIT" << endl;
}

// 时间片轮转调度算法创建就绪进程队列
// 详细注释可以参考getprocess(),两者大同小异
pcb *getprocess_round()
{
    // 临时变量,用来做分配内存初始化等工作
    pcb *q;
    // 临时变量(cursor),保存当前进程结点信息,用来循环增至p后
    pcb *t;
    // p为返回的链式进程首节点
    pcb *p;
    int i = 0;
    cout << "input name and time" << endl;
    while (i < P_NUM)
    {
        q = (struct pcb *)malloc(sizeof(pcb));
        cin >> q->name;
        cin >> q->needtime;
        q->cputime = 0;
        q->round = 0;
        q->count = 0;
        q->process = ready;
        q->next = NULL;
        if (i == 0)
        {
            p = q;
            t = q;
        }
        else
        {
            t->next = q;
            t = q;
        }
        i++;
    }
    return p;
}

// 采用时间片轮转调度算法执行某一进程
void cpu_round(pcb *q)
{
    q->cputime += 2;
    q->needtime -= 2;
    if (q->needtime < 0)
    {
        q->needtime = 0;
    }
    q->count++;
    q->round++;
    q->process = execute;
}

// 获取下一个需要执行的进程
// k当前进程,head,头结点进程
pcb *get_next(pcb *k, pcb *head)
{
    // 临时变量
    pcb *t;
    t = k;
    do
    {
        // 更改t为t的下一个进程
        t = t->next;
        // 如果t不为空并且t的状态==finish
    } while (t && t->process == finish);
    // 如果t为空
    if (t == NULL)
    {
        // 则t改为头结点
        t = head;
        // 判断t的下一个结点是否为k(传进来的当前结点) && t的状态==finish
        // 退出本循环的条件有两个:要么t==k,要么t的状态不为finish
        // t==k表示已经找了一圈了,t的状态为finish表示当前进程可以执行
        while (t->next != k && t->process == finish)
        {
            t = t->next;
        }
    }
    return t;
}

// 设置队列中进程执行状态
void setstate(pcb *p)
{
    while (p)
    {
        if (p->needtime == 0)
        {
            p->process = finish; //如果所需执行时间为0，则设置运行状态为结東
        }
        if (p->process == execute)
        {
            p->process = ready; //如果为执行状态则设置为就绪
        }
        p = p->next;
    }
}
void display_round(pcb *p)
{
    cout << "NAME"
         << " | "
         << "CPUTIME"
         << " | "
         << "NEEDTIME"
         << " | "
         << "COUNT"
         << " | "
         << "ROUTND"
         << " | "
         << "STATE   |" << endl;
    while (p)
    {
        cout << setw(4) << p->name;
        cout << " | ";
        cout << setw(7) << p->cputime;
        cout << " | ";
        cout << setw(8) << p->needtime;
        cout << " | ";
        cout << setw(5) << p->count;
        cout << " | ";
        cout << setw(6) << p->round;
        cout << " | ";
        switch (p->process)
        {
        case ready:
            cout << "ready   |" << endl;
            break;
        case execute:
            cout << "execute |" << endl;
            break;
        case finish:
            cout << "finish  |" << endl;
            break;
        }
        p = p->next;
    }
}

// 时间片轮转调度算法输出调度信息
void round_cal()
{
    // p是进程链表的首节点
    pcb *p;
    pcb *r;
    system("cls");

    p = getprocess_round();
    system("cls");

    int cpu = 0;
    r = p;
    while (!process_finish(p))
    {
        system("cls");
        cpu += 2;
        // 模拟执行r
        cpu_round(r);
        // 获取r的下一个进程

        r = get_next(r, p);
        cout << "\ncpu" << cpu << endl;
        display_round(p);
        // 更改进程状态
        setstate(p);
        Sleep(500);
    }
}
