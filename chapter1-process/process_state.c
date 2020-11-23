#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

void block();
void wakeup();
void show();
void create();
void kill();
// simple process model
struct process_type
{
    // pid
    int pid;

    // process priority
    int priority;

    // process
    int size;

    // process state, 0-not in memory, 1-running, 2-block
    int state;

    // process content
    char content[10];
};

// memory
struct process_type neicun[20];

// current number of process
int shumu = 0;

// number of blocked process
int zuses = 0;

int pid;
int flag = 0;

int main()
{
#if defined(_WIN16) || defined(_WIN32) || defined(_WIN64)
    // change code page，use utf-8
    system("chcp 65001");
#endif
    int num;
    for (int i = 0; i < 20; i++)
        neicun[i].state = 0;
    while (true)
    {
        printf("\n\n\n");
        printf("\n***********************************************************");
        printf("\n*进程演示系统*");
        printf("\n***********************************************************");
        printf("\n*1.创建新的进程2．查看运行进程*");
        printf("\n*3．阻塞某个进程4．杀死运行进程*");
        printf("\n*5．唤醒某个进程6．退出系统*");
        printf("\n************************************************************");
        printf("\n请选择(1～6)\n");
        scanf("%d", &num);
        switch (num)
        {
        case 1:
            create();
            break;
        case 2:
            show();
            break;
        case 3:
            block();
            break;
        case 4:
            kill();
            break;
        case 5:
            wakeup();
            break;
        case 6:
            exit(0);
        default:
            break;
        }
        flag = 0; //恢复标记
    }
}

// create mock the process creation
void create()
{
    // check process number
    if (shumu >= 20)
        printf("\n内存已满，请先唤醒或杀死进程n");
    else
    {
        // watch process list
        for (int i = 0; i < 20; i++)
        {

            // 定位，找到可以还未创建的进程
            if (neicun[i].state != 0)
                continue;
            printf("\n请输入新进程pid\n");
            scanf("%d", &(neicun[i].pid));

            for (int j = 0; j < i; j++)
            {
                if (neicun[i].pid == neicun[j].pid)
                {
                    printf("\n该进程已存在n");
                    return;
                }
            }

            printf("\n请输入新进程优先级\n");
            scanf("%d", &(neicun[i].priority));
            printf("\n请输入新进程大小\n");
            scanf("%d", &(neicun[i].size));
            printf("\n请输入新进程内容\n");
            scanf("%s", &(neicun[i].content));

            //创建进程，使标记位为1
            neicun[i].state = 1;
            shumu++;
            break;
        }
    }
}
void show()
{
    for (int i = 0; i < 20; i++)
    {
        if (neicun[i].state == 1 || neicun[i].state == 2)
        {
            // printf running process information
            printf("\npid=%d\t", neicun[i].pid);
            printf("priority=%d\t", neicun[i].priority);
            printf("size=%d\t", neicun[i].size);
            printf("state=%d\t", neicun[i].state);
            printf("content=%s", neicun[i].content);
            flag = 1;
        }
    }
    if (!flag)
        printf("\n当前没有运行进程\n");
}

void wakeup()
{
    if (!shumu)
    {
        printf("当前没有运行进程\n");
        return;
    }
    if (!zuses)
    {
        printf("\n当前没有阻塞进程\n");
        return;
    }
    printf("\n输人唤醒进程的ID值");
    scanf("%d", &pid);
    for (int i = 0; i < 20; i++)
    {
        //定位，找到所要唤醒的进程，根据其状态做相应处理
        if (pid == neicun[i].pid)
        { // if block
            if (neicun[i].state == 2)
            {
                // wake up
                neicun[i].state = 1;
                // reduce block queue
                zuses--;
                printf("\n已经成功唤醒进程\n");
            }
            else if (neicun[i].state == 0)
                printf("\n要唤醒的进程不存在\"n");
            else
                printf("\n要唤醒的进程已在运行\n");
            flag = 1;
            break;
            // 找不到，则说明进程不存在
            if (flag == 0)
                printf("\n要唤醒的进程不存在\n");
        }
    }
}
void kill()
{
    if (!shumu)
    {
        printf("当前没有运行进程\n");
        return;
    }
    printf("\n输入杀死进程的PID值");
    scanf("%d", &pid);
    for (int i = 0; i < 20; i++)
    {
        //定位，找到所要杀死的进程，根据其状态做相应处理
        if (pid == neicun[i].pid)
        {
            if (neicun[i].state == 1)
            {
                neicun[i].state = 0;
                shumu--;
                printf("\n已成功杀死进程\n");
            }
            else if (neicun[i].state == 0)
                printf("\n:要杀死的进程不存在\n");
            else
                printf("\n要杀死的进程已被阻塞\n");
            flag = 1;
            break;
        }
    }
    //找不到，则说明进程不存在
    if (!flag)
        printf("\n要杀死的进程不存在\n");
}

void block()
{
    if (!shumu)
    {
        printf("\n当前没有运行进程\n");
        return;
    }
    printf("\n输人pid：\n");
    scanf("%d", &pid);
    for (int i = 0; i < 20; i++)
    {
        //定位，找到所要block的进程
        if (pid == neicun[i].pid)
        {
            flag = false;
            if (neicun[i].state == 1)
            {
                neicun[i].state = 2;
                zuses++;
                printf("\n已经成功阻塞进程\n");
            }
            else if (neicun[i].state == 0)
                printf("\n要阻塞的进程不存在\n");
            else
            {

                printf("\n要阻塞的进程已被阻塞\n");
                break;
            }
            //找不到，则说明进程不存在
            if (flag)
                printf("\n要阻塞的进程不存在\n");
        }
    }
}