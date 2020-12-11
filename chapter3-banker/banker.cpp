/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */
 
#include <iostream>

using namespace std;

int Available[100];      // 可利用资源数组
int Max[50][100];        // 最大需求矩阵
int Allocation[50][100]; // 已分配资源矩阵
int Need[50][100];       // 剩下所需资源矩阵
int Request[50][100];    // 请求的资源
int Finish[50];          // Finish[i]标志一个进程是否完成
int p[50];               // 安全序列
int process;             // 进程数量
int resourceNum;         // 资源种类数量

int IsSafe();

int main()
{
#if defined(_WIN16) || defined(_WIN32) || defined(_WIN64)
    // change code page，use utf-8
    system("chcp 65001");
#endif
    //银行家算法
    int i, j, mi;
    cout << "输人进程的数目：\n";
    cin >> process;
    cout << "输人资源种类的数量：\n";
    cin >> resourceNum;
    cout << "输入每个进程最多所需的各资源数,按照" << process << " x " << resourceNum << "矩阵输入\n";
    // 每个进程的所需资源(总)赋值,减去下面的已分配就等于需要的资源
    for (i = 0; i < process; i++)
        for (j = 0; j < resourceNum; j++)
            cin >> Max[i][j];
    cout << "输入每个进程已分配的各资源数,也按照" << process << "x" << resourceNum << "矩阵输入\n";
    // 已分配资源赋值,用进程所需资源总数(Max)减去这个已分配(Allocation)的就等于还需要的(Need)
    for (i = 0; i < process; i++)
    {
        for (j = 0; j < resourceNum; j++)
        {
            cin >> Allocation[i][j];
            // 计算剩下所需资源
            Need[i][j] = Max[i][j] - Allocation[i][j];
            if (Need[i][j] < 0)
            {
                cout << "你输入的第" << i + 1 << "个进程所拥有的第" << j + 1 << "个资源数错误,请重新输入：\n";
                j--;
                continue;
            }
        }
    }
    cout << "请输入各个资源现有的数目：\n";
    // 可用资源数目
    for (i = 0; i < resourceNum; i++)
        cin >> Available[i];
    IsSafe();
    while (1)
    {
        
        cout << "输入要申请资源的进程号(注：第1个进程号为0,以此类推)\n";
        cin >> mi;
        cout << "输入进程所请求的各资源的数量\n";
        for (i = 0; i < resourceNum; i++)
            cin >> Request[mi][i];

        for (i = 0; i < resourceNum; i++)
        {
            if (Request[mi][i] > Need[mi][i])
            {
                cout << "你输入的请求数超过进程的需求量!\n";
                return 0;
            }
            if (Request[mi][i] > Available[i])
            {
                cout << "你输人的请求数超过系统有的资源数！\n";
                return 0;
            }
        }
        for (i = 0; i < resourceNum; i++)
        {
            Available[i] -= Request[mi][i];
            Allocation[mi][i] += Request[mi][i];
            Need[mi][i] -= Request[mi][i];
        }
        if (IsSafe())
        {

            cout << "同意分配请求！\n";
            for (i = 0; i < resourceNum; i++)
            {

                Available[i] += Request[mi][i] + Allocation[mi][i];
                Finish[i] = 1;
                Need[mi][i] = 0;
            }
        }
        else
        {
            cout << "你的请求被拒绝\n";
            for (i = 0; i < resourceNum; i++)
            {
                Available[i] += Request[mi][i];
                Allocation[mi][i] -= Request[mi][i];
                Need[mi][i] += Request[mi][i];
            }
            for (i = 0; i < process; i++)
                Finish[i] = 0;
            char YesOrNo;
            cout << "你还想再次请求分配吗？是请按yY,否按n／N,再确定\n";
            while (1)
            {
                cin >> YesOrNo;
                if (YesOrNo == 'y' || YesOrNo == 'Y' || YesOrNo == 'n' || YesOrNo == 'N')
                    break;
                else
                {
                    cout << "请按要求输入！\n";
                    continue;
                }
            }
            // if (YesOrNo == 'y' || YesOrNo == 'Y')
            //     continue;
            // else
            //     break;
        }
    }
}

int IsSafe()
{
    int i, j, l = 0;
    int Work[100]; // 可利用资源数组,这里用work是为了避免对真正的可用资源进程更改，毕竟IsSafe的功能仅仅是检查
    for (i = 0; i < resourceNum; i++)
        Work[i] = Available[i];
    for (i = 0; i < process; i++)
        Finish[i] = 0;
    // 判断是否安全
    // i代表进程号
    for (i = 0; i < process; i++)
    {
        if (Finish[i] == 1)
            continue;
        else
        {
            // j 代表资源号 {0,1,2}
            for (j = 0; j < resourceNum; j++)
            {
                // 如果i进程的第j个资源 大于 可用资源数组
                if (Need[i][j] > Work[j])
                    break;
            }
            // 如果资源都已检查过，并且可以分配
            if (j == resourceNum)
            {
                Finish[i] = 1;
                for (int k = 0; k < resourceNum; k++)
                    // 资源回收,将进程i的k个资源全部回收到Work里,work是可利用的资源数组
                    Work[k] += Allocation[i][k];
                p[l++] = i;
                // i置为-1，再来一遍
                i = -1;
            }
            else
                continue;
            // 如果所有的进程都检查过
            if (l == process)
            {
                cout << "系统是安全的"
                     << "\n";
                cout << "安全序列是：\n";
                for (i = 0; i < l; i++)
                {
                    cout << p[i];
                    if (i != l - 1)
                        cout << "-->";
                }
                cout << "\n";
                return 1;
            }
        }
    }
    return 0;
}
