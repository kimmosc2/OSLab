/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include <stdio.h>
#include <cstring>
#include <stdlib.h>

#define SizeOfPage 100
#define SizeOfBlock 128

//最大页面数量
#define MaxPage 4 
struct info //页表信息结构体
{
    bool flag;  // 页标志，1表示该页已在内存，0表示该页不在内存
    long block; // 块号
    long disk;  // 在磁盘上的位置
    bool dirty; // 更新标志
} pagelist[SizeOfPage];

long po;   // 队列标记
long P[MaxPage]; // 假设内存中最多允许MaxPage个页面

void init_ex1()
{
    memset(pagelist, 0, sizeof(pagelist)); //内存空间初始化
    /*分页式虚拟存储系统初始化*/
    pagelist[0].flag = 1;
    pagelist[0].block = 5;
    pagelist[0].disk = 011;

    pagelist[1].flag = 1;
    pagelist[1].block = 8;
    pagelist[1].disk = 012;

    pagelist[2].flag = 1;
    pagelist[2].block = 9;
    pagelist[2].disk = 013;

    pagelist[3].flag = 1;
    pagelist[3].block = 1;
    pagelist[3].disk = 021;
}
void work_ex1()
{ //模拟分页式存储管理中硬件的地址转换和产生缺页中断过程
    bool stop = 0;
    long p, q;
    char s[128];
    do
    {
        printf("please input page number and unit number:\n");
        if (scanf("%d %d", &p, &q) != 2)
        {
            scanf("%s", s);
            if (strcmp(s, "exit") == 0)
            { //如果输入的为“exit”那么就退出，进入重选页面
                stop = 1;
            }
        }
        else
        {
            if (pagelist[p].flag)
            {                                                                  //如果该页flag标志位为1，说明该页在主存中
                printf("absolute address=%1d\n", pagelist[p].block * SizeOfBlock + q); //计算出绝对地址，绝对地址=块号*块长(默认128)+单元号
            }
            else
            {
                printf("*%1d\n", p); //如果该页flag标志位为0，表示该页不在主存中，则产生了一次缺页中断
            }
        }
    } while (!stop);
}
void init_ex2()
{
    /*以下部分为用先进先出(FIFO)页面调度算法处理缺页中断的初始化，其中也包含了对于当前的存储期内容的初始化*/
    po = 0;
    P[0] = 0;
    P[1] = 1;
    P[2] = 2;
    P[3] = 3;                             //对内存中的4个页面初始化并使目前排在第一位的为0
    memset(pagelist, 0, sizeof(pagelist)); //内存空间初始化
    pagelist[0].flag = 1;
    pagelist[0].block = 5;
    pagelist[0].disk = 011;
    pagelist[1].flag = 1;
    pagelist[1].block = 8;
    pagelist[1].disk = 012;
    pagelist[2].flag = 1;
    pagelist[2].block = 9;
    pagelist[2].disk = 013;
    pagelist[3].flag = 1;
    pagelist[3].block = 1;
    pagelist[3].disk = 021;
}
void work_ex2()
{
    // 页面号和单元号
    long p, q, i;
    char s[100];
    bool stop = 0;
    do
    {
        // printf("请输入指令的页号、单元号，以及是否为内存指令：\n");
        printf("please input page number and unit number:\n");
        if (scanf("%d %d", &p, &q) != 2)
        {
            scanf("%s", s);
            if (strcmp(s, "exit") == 0)
            { //如果输入为"exit"那么就退出，进入重选页面
                stop = 1;
            }
        }
        else
        {
            // scanf("%s", s);
            if (pagelist[p].flag)
            {                                                                  // 如果该页flag标志位为1，说明该页在主存中
                printf("absolute address=%1d\n", pagelist[p].block * SizeOfBlock + q); // 计算绝对地址，绝对地址=块号×块长(128)＋单元号
                // if (s[0] == 'Y' || s[0] == 'y')
                // {                          //内存指令
                //     pagelist[p].dirty = 1; //修改标志为1
                // }
            }
            else
            {
                if (pagelist[P[po]].dirty)
                { //当前页面被更新过，需把更新后的内容写回外存
                    pagelist[P[po]].dirty = 0;
                }
                pagelist[P[po]].flag = 0;                  // 将flag标志位置0，表示当前页面已被置换出去
                printf("out of page:%3d\n", P[po]);                 // 显示根据FIFO算法被置换出去的页面
                printf("inside page:%3d\n", p);                      // 显示根据FTFO算法被调入的页面
                pagelist[p].block = pagelist[P[po]].block; // 块号相同
                pagelist[p].flag = 1;                      // 将当前页面的标记置为1，表示已在主存中
                P[po] = p;                                 // 保存当前页面所在的位置
                po = (po + 1) % MaxPage;
            }
        }
    } while (!stop);
    printf("array value of p:\n");
    for (i = 0; i < MaxPage; i++)
    { //循环输出当前数组的数值，即当前在内存中的页面
        printf("P[%1d]=%1d\n", i, P[i]);
    }
}
void select()
{ //选择哪种方法进行
    long se;
    char s[128];
    do
    {
        printf("select (1/2):");
        if (scanf("%d", &se) != 1)
        {
            scanf("%s", &s);
            if (strcmp(s, "exit") == 0)
            { //如果输入为exit则退出整个程序
                return;
            }
        }
        else
        {
            if (se == 1)
            {               // 如果se=1说明选择的是模拟分页式存储管理中硬件的地址转换和产生缺页中断
                init_ex1(); // 初始化
                work_ex1(); // 进行模拟
            }
            if (se == 2)
            {               // 如果se=2说明选择的是FFO算法来实现页面的置换
                init_ex2(); // 初始化
                work_ex2(); // 进行模拟
            }
        }
        printf("%d",se);
    } while (1);
}
int main()
{
#if defined(_WIN16) || defined(_WIN32) || defined(_WIN64)
    // change code page，use utf-8
    system("chcp 936");
#endif
    select(); //选择题号
    return 0;
}
