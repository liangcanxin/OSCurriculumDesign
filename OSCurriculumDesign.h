#ifndef OSCURRICULUMDESIGN_H_INCLUDED
#define OSCURRICULUMDESIGN_H_INCLUDED

#include <iostream>

using namespace std;

#include<time.h>
#include<stdlib.h>
#include<stdio.h>

void DepositRandomAddr(int amount, int *a, int &minAddr, int &maxAddr);
void DepositPageAddr(int amount, int *a, int minAddr, int maxAddr);

/***** DepositRandomAddr *****
** param: amount, *a, minAddr, maxAddr
** return: null
** function: 通过随机数产生一个地址序列，其中50%的地址访问是顺序执行，
**           50%是非顺序执行，且地址在前半部分地址空间和后半部分地址
**           空间均匀分布。
*/
void DepositRandomAddr(int amount, int *a, int &minAddr, int &maxAddr)
{
    int m;              //产生的随机数
    int n[amount];      //用来产生地址空间
    minAddr = (int)&n[0];
    maxAddr = (int)&n[amount-1];
    //printf("随机数的地址范围：%d - %d\n",minAddr,maxAddr);
    int i;
    srand((unsigned)time( NULL ));
    for(i=0;i<amount;i=i+4)
    {
        m=rand()%200;
        a[i] = (int)&n[m];
        a[i+1] = (int)&n[m+1];
        m=rand()%200+200;
        a[i+2] = (int)&n[m];
        a[i+3] = (int)&n[m+1];
    }
}

/***** DepositPageAddr *****
** param: amount, *a, minAddr, maxAddr
** return: null
** function: 将指令地址流转变为页地址（页号）流，其中，单页大小为
**           (maxAddr-minAddr)/(amount-1)*10。
*/
void DepositPageAddr(int amount, int *a, int minAddr, int maxAddr)
{
    //printf("\n每个数据占用空间大小: %d\n",((maxAddr-minAddr)/(amount-1)));
    int i;
    for(i=0;i<amount;i++){
        a[i] = (a[i]-minAddr)/((maxAddr-minAddr)/(amount-1)*10);
    }
}

#endif // OSCURRICULUMDESIGN_H_INCLUDED
