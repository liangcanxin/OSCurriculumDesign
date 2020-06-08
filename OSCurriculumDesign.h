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
** function: ͨ�����������һ����ַ���У�����50%�ĵ�ַ������˳��ִ�У�
**           50%�Ƿ�˳��ִ�У��ҵ�ַ��ǰ�벿�ֵ�ַ�ռ�ͺ�벿�ֵ�ַ
**           �ռ���ȷֲ���
*/
void DepositRandomAddr(int amount, int *a, int &minAddr, int &maxAddr)
{
    int m;              //�����������
    int n[amount];      //����������ַ�ռ�
    minAddr = (int)&n[0];
    maxAddr = (int)&n[amount-1];
    //printf("������ĵ�ַ��Χ��%d - %d\n",minAddr,maxAddr);
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
** function: ��ָ���ַ��ת��Ϊҳ��ַ��ҳ�ţ��������У���ҳ��СΪ
**           (maxAddr-minAddr)/(amount-1)*10��
*/
void DepositPageAddr(int amount, int *a, int minAddr, int maxAddr)
{
    //printf("\nÿ������ռ�ÿռ��С: %d\n",((maxAddr-minAddr)/(amount-1)));
    int i;
    for(i=0;i<amount;i++){
        a[i] = (a[i]-minAddr)/((maxAddr-minAddr)/(amount-1)*10);
    }
}

#endif // OSCURRICULUMDESIGN_H_INCLUDED
