/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include <stdio.h>
//#include<cstdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

#define bool _Bool

#define N 3  // 假设系统有3类设备
#define M 10 // 假设系统有5个设备
struct
{
    char type[10]; // 设备类名
    int count;     // 拥有设备数量
    int remain;    // 现存的可用设备数量
    int address;   // 该类设备在设备表中的起始地址
} equip_type[N];   // 设备类表定义，假设系统有n个设备类型
struct
{
    int number;       // 设备绝对号
    bool status;      // 设备状态可否使用
    bool IsRemain;    // 设备是否已分配
    char jobname[10]; // 占有设备的作业名称
    int lnumber;      // 设备相对号
} equipment[M];       // 设备表定义，假设系统有m个设备

// *********************************函数说明*************************************
//  设备分配函数
// *****************************************************************************
bool allocate(char *job, char *type, int mm)
{
    int i = 0, t;
    // 查询该类设备
    while (i < N && strcmp(equip_type[i].type, type) != 0)
        i++;
    // 没有找到该类设备
    if (i >= N)
    {
        printf("无该类设备，设备分配请求失败");
        return false;
    }
    // 所需设备现存的可用数量不足
    if (equip_type[i].remain < 1)
    {
        printf("该类设备数量不足，设备分配请求失败");
        return false;
    }
    // 得到该类设备在设备表中的起始地址
    t = equip_type[i].address;
    while (!(equipment[t].status == true && equipment[t].IsRemain == false))
        t++;
    // 填写作业名、设备相对号，状态
    equip_type[i].remain--;
    equipment[t].IsRemain = true;
    strcpy(equipment[t].jobname, job);
    equipment[t].lnumber = mm;
    return true;
}
//*********************************函数说明************************************
//设备回收函数
//*****************************************************************************
bool reclaim(char *job, char *type)
{
    int i = 0, t, j, k = 0, nn;
    while (i < N && strcmp(equip_type[i].type, type) != 0)
        i++;
    //没有找到该类设备
    if (i >= N)
    {
        printf("无该类设备，设备分配请求失败");
        return (false);
    }
    //得到该类设备在设备表中的起始地址
    t = equip_type[i].address;
    //得到该设备的数量
    j = equip_type[i].count;
    nn = t + j;
    //修改设备为可使用状态和该类型设备可用数量
    for (; t < nn; t++)
    {
        if (strcmp(equipment[t].jobname, job) == 0 && equipment[t].IsRemain == true)
        {
            equipment[t].IsRemain = false;
            k++;
        }
    }
    equip_type[i].remain = equip_type[i].remain + k;
    if (k == 0)
        printf("作业没有使用该类设备");
    return true;
}
int main()
{

    // 作业名
    char job[10];
    // mm 设备相对号,choose 菜单选项
    int i, mm, choose;
    // 作业所需设备类
    char type[10];
    strcpy(equip_type[0].type, "input"); //设备类型：输入设备
    equip_type[0].count = 2;
    equip_type[0].remain = 2;
    equip_type[0].address = 0;
    strcpy(equip_type[1].type, "printer");
    equip_type[1].count = 3;
    equip_type[1].remain = 3;
    equip_type[1].address = 2;
    strcpy(equip_type[2].type, "disk");
    equip_type[2].count = 4;
    equip_type[2].remain = 4;
    equip_type[2].address = 5;
    for (i = 0; i < M; i++)
    {
        equipment[i].number = i;
        equipment[i].status = 1;
        equipment[i].IsRemain = false;
    }
    while (true)
    {
        printf("\n0---退出，1---分配，2---回收，3---显示");
        printf("\n请选择功能项：");
        scanf("%d", &choose);
        switch (choose)
        {
        case 0:
            return 0;
        case 1:
            printf("请输入作业名、作业所需设备类和设备相对号");
            scanf("%s %s %d", job, type, &mm);
            allocate(job, type, mm); // 分配设备
            break;
        case 2:
            printf("请输入作业名和作业要归还的设备类型");
            scanf("%s %s", job, type);
            reclaim(job, type); // 回收设备
            break;
        case 3:
            printf("\n系统设备表(SDT)：\n");
            printf("设备类型\t拥有设备数量\t可分配设备数量\t起始地址\n");
            for (i = 0; i < N; i++)
                printf("%-8s%12d%18d%14d\n", equip_type[i].type, equip_type[i].count,
                       equip_type[i].remain, equip_type[i].address);
            printf("-----------------------------------------------\n");
            printf("设备控制表(DCT)：\n");
            printf("绝对号\t设备状态\t是否分配\t占用作业名\t相对号\n");
            for (i = 0; i < M; i++)
            {
                printf("%3d%12d%14d%18s%14d\n", equipment[i].number, equipment[i].status,
                       equipment[i].IsRemain, equipment[i].jobname, equipment[i].lnumber);
            }
        default:
            continue;
        }
    }
}
