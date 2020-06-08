#ifndef RESOURCE_H_INCLUDED
#define RESOURCE_H_INCLUDED

///����Դ�벿�ֲο�������Դ
#include<iostream>
#include<stdlib.h>
#include<string.h>
#include<iomanip>
#include<stdio.h>
using namespace std;
int GetDistance(int currentPageID,int page);//����ҳ���Ż�ȡ��������ڵ�ҳ�����δʹ�õ�ʱ�䣨�ĸ����뵱ǰpageID��Զ��
int GetDistanceToFuture(int currentPageID,int page);//����ҳ���Ż�ȡ��������ڵ�ҳ�����������õ���ʱ�䣨�ĸ����뵱ǰpageID��Զ��
class BLOCK{
public:
	int blockNum;	//���������
	int pageNum;	//������е�ҳ������
	int *pageID;	//ҳ��ţ���СΪblockNum��
	int *stayTime;	//ҳ����������е�ͣ��ʱ�䣨�������ID��Ӧ��
	BLOCK(int num)
	{
		int i;
		pageNum=0;
		blockNum=num;
		pageID=new int[num];
		stayTime=new int[num];
		for(i=0;i<num;i++)
		{
			pageID[i]=-1;	//��ʼ��ÿ���������û�з��ã�ҳ��ű�ʾΪ-1
			stayTime[i]=0;	//��ʼ��ͣ��ʱ��Ϊ0
		}
	}
	void Init()
	{
		int i;
		int num=blockNum;
		pageNum=0;
		pageID=new int[num];
		stayTime=new int[num];
		for(i=0;i<num;i++)
		{
			pageID[i]=-1;	//��ʼ��ÿ���������û�з��ã�ҳ��ű�ʾΪ-1
			stayTime[i]=0;	//��ʼ��ͣ��ʱ��Ϊ0
		}
	}
	void ShowPage()
	{
		int i;
		for(i=0;i<blockNum;i++)
		cout<<"Page["<<i<<"]: "<<pageID[i]<<endl;
	}
	void ShowStayTime()
	{
		int i;
		for(i=0;i<blockNum;i++)
		cout<<"Stay["<<i<<"]: "<<stayTime[i]<<endl;
	}
	int GetLongestStay()	//��ȡ���������ͣ��ʱ�����ҳ������������
	{
		int i;
		int max_pos=0;
		for(i=0;i<pageNum;i++)
		if(stayTime[max_pos]<stayTime[i])
		max_pos=i;
		return max_pos;
	}
	int GetRencentNotUse(int currentPageID)	//��ȡ���������������δʹ�õ�ҳ����
	{
		//Ĭ��currentPageIDһ�����ڵ���BLOCKNUM
		int i;
		int DestID=0;
		for(i=0;i<blockNum;i++)
		{
			if(GetDistance(currentPageID,pageID[i])>GetDistance(currentPageID,pageID[DestID]))
			DestID=i;
		}
		return DestID;
	}
	int GetfutureNotUse(int currentPageID) //��ȡ�����������������ʹ�õ���ҳ����
	{
	    //Ĭ��currentPageIDһ�����ڵ���BLOCKNUM
	    int i;
	    int DestID=0;
	    for(i=0;i<blockNum;i++)
		{
			if(GetDistanceToFuture(currentPageID,pageID[i])>GetDistance(currentPageID,pageID[DestID]))
			DestID=i;
		}
		return DestID;
	}
};	//��������ݽṹ����
//-----------------------ȫ�ֱ���-------------------------
int BLOCKNUM;		//�������
int *PVS;			//PageVisitSequenceҳ���������
int PVS_NUM;		//ҳ��������г���
int *replaceArray;	//ҳ���û���־���飨��СΪ����ҳ��Ĵ������洢ÿ�η����Ƿ����ҳ���û���
int *lackArray;		//ȱҳ�жϱ�־���飨��СΪ����ҳ��Ĵ������洢ÿ�η����Ƿ����ȱҳ�жϣ�
//-----------------------��������-------------------------
void InputAndInit();						//��������ͱ�����ʼ��
void ReplaceFIFO(BLOCK block);				//FIFOҳ���û��㷨
int FindPage(int pageID,BLOCK block);		//ҳ����ң�����ҳ������������в���ҳ���Ƿ���ڣ�
void ReplaceLRU(BLOCK block);				//LRUҳ���û��㷨
void ReplaceOPT(BLOCK block);			//LRUҳ���û��㷨
int GetReplaceTimes();						//��ȡҳ���û��ܴ���
int GetLackTimes();							//��ȡȱҳ�ж��ܴ���
//----------------------FIFOҳ���û��㷨--------------------------
void ReplaceFIFO(BLOCK block)			//FIFOҳ���û��㷨
{
	int i,j;
	block.Init();
	int replacePosition;				//���û�λ��
	for(i=0;i<PVS_NUM;i++)				//���ζ�ҳ��������е�ÿһ��ҳ��PVS[i]���в���
	{
		for(j=0;j<block.pageNum;j++)
		block.stayTime[j]++;			//ÿѭ��һ�Σ�����飨0~pageNum��ͣ��ʱ������
		if(block.pageNum<block.blockNum)
		{
			if(!FindPage(PVS[i],block))	//��ҳ��PVS[i]������
			{
			    //lackArray[i]=1;         //���ڷ���ҳ�治�������ҳ���ж�
				block.pageID[block.pageNum]=PVS[i];
				block.pageNum++;
			}
		}
		else 						//FIFO�㷨���û�ͣ��ʱ�����ҳ�����������λ�ã�
		{

//			TODO:��ҳ���Ѵ��ڵ�����������������Ӧ����ҳ�治���ڵ������Ӧ����if(ҳ����������в�����)���жϣ�
			if(!FindPage(PVS[i],block))	//��ҳ��PVS[i]������
			{
				replaceArray[i]=1;		//���ڷ���ҳ�治�������޿�����������ҳ���û�
				lackArray[i]=1;			//���ڷ���ҳ�治�������ҳ���ж�
				replacePosition=block.GetLongestStay();
				block.pageID[replacePosition]=PVS[i];	//ѡ��ͣ��ʱ�����ҳ���û�
				block.stayTime[replacePosition]=0;		//�û��󣬸�ҳ����������λ��ͣ��ʱ������
			}
		}
	}
}
int FindPage(int pageID,BLOCK block)		//ҳ����ң�����ҳ�������Դ��ҳ�������飨����ΪpageNum���в���ҳ���Ƿ���ڣ�
{
	int i=0;
	for(i=0;i<block.pageNum;i++){
        if(block.pageID[i]==pageID)
        break;
	}
	return !(i==block.pageNum);				//��ҳ����ڣ��򷵻�1�����򷵻�0
}
//----------------------LRUҳ���û��㷨--------------------------
void ReplaceLRU(BLOCK block)			//LRUҳ���û��㷨
{
	int i,j;
	block.Init();
	int replacePosition;				//���û�λ��
	for(i=0;i<PVS_NUM;i++)				//���ζ�ҳ��������е�ÿһ��ҳ��PVS[i]���в���
	{
		for(j=0;j<block.pageNum;j++)
		block.stayTime[j]++;			//ÿѭ��һ�Σ�����飨0~pageNum��ͣ��ʱ������
		if(block.pageNum<block.blockNum)
		{
			if(!FindPage(PVS[i],block))	//��ҳ��PVS[i]������
			{
			    //lackArray[i]=1;         //���ڷ���ҳ�治�������ҳ���ж�
				block.pageID[block.pageNum]=PVS[i];
				block.pageNum++;
			}
		}
		else							//FIFO�㷨���û�ͣ��ʱ�����ҳ�����������λ�ã�
		{
//			TODO:��ҳ���Ѵ��ڵ�����������������Ӧ����ҳ�治���ڵ������Ӧ����if(ҳ����������в�����)���жϣ�
			if(!FindPage(PVS[i],block))	//��ҳ��PVS[i]������
			{
				replaceArray[i]=1;		//���ڷ���ҳ�治�������޿�����������ҳ���û�
				lackArray[i]=1;			//���ڷ���ҳ�治�������ҳ���ж�
				replacePosition=block.GetRencentNotUse(i);
				block.pageID[replacePosition]=PVS[i];	//ѡ��ͣ��ʱ�����ҳ���û�
				block.stayTime[replacePosition]=0;		//�û��󣬸�ҳ����������λ��ͣ��ʱ������
			}
		}
	}
}

void ReplaceOPT(BLOCK block)			//LRUҳ���û��㷨
{
	int i,j;
	block.Init();
	int replacePosition;				//���û�λ��
	for(i=0;i<PVS_NUM;i++)				//���ζ�ҳ��������е�ÿһ��ҳ��PVS[i]���в���
	{
		for(j=0;j<block.pageNum;j++)
		block.stayTime[j]++;			//ÿѭ��һ�Σ�����飨0~pageNum��ͣ��ʱ������
		if(block.pageNum<block.blockNum)
		{
			if(!FindPage(PVS[i],block))	//��ҳ��PVS[i]������
			{
			    //lackArray[i]=1;         //���ڷ���ҳ�治�������ҳ���ж�
				block.pageID[block.pageNum]=PVS[i];
				block.pageNum++;
			}
		}
		else							//FIFO�㷨���û�ͣ��ʱ�����ҳ�����������λ�ã�
		{
//			TODO:��ҳ���Ѵ��ڵ�����������������Ӧ����ҳ�治���ڵ������Ӧ����if(ҳ����������в�����)���жϣ�
			if(!FindPage(PVS[i],block))	//��ҳ��PVS[i]������
			{
				replaceArray[i]=1;		//���ڷ���ҳ�治�������޿�����������ҳ���û�
				lackArray[i]=1;			//���ڷ���ҳ�治�������ҳ���ж�
				replacePosition=block.GetfutureNotUse(i);
				block.pageID[replacePosition]=PVS[i];	//ѡ��ͣ��ʱ�����ҳ���û�
				block.stayTime[replacePosition]=0;		//�û��󣬸�ҳ����������λ��ͣ��ʱ������
			}
		}
	}
}

void InputAndInit(int blockNum, int *a, int amount)					//��������ͱ�����ʼ��
{
	int i=0;
    BLOCKNUM = blockNum;
    PVS_NUM = amount;
    PVS = new int[PVS_NUM];
    for(i=0;i<PVS_NUM;i++)PVS[i]=a[i];
    replaceArray=new int[PVS_NUM];
    lackArray=new int[PVS_NUM];
	for(i=0;i<PVS_NUM;i++)
	{
		replaceArray[i]=0; 					//ҳ���û���־�����ʼ��Ϊ0
		lackArray[i]=0; 					//ȱҳ�жϱ�־�����ʼ��Ϊ0
	}
}

int GetDistance(int currentPageID,int page)	//����ҳ���Ż�ȡ��������ڵ�ҳ�����δʹ�õ�ʱ�䣨�ĸ����뵱ǰpageID��Զ��
{
	int distance=0;
	int i;
	for(i=currentPageID-1;i>=0;i--)
	if(PVS[i]!=page)
	distance++;
	else break;
	return distance;
}

int GetDistanceToFuture(int currentPageID,int page)	//����ҳ���Ż�ȡ��������ڵ�ҳ�����������õ���ʱ�䣨�ĸ����뵱ǰpageID��Զ��
{
	int distance=0;
	int i;
	for(i=currentPageID+1;i<=PVS_NUM;i++)
	if(PVS[i]!=page)
	distance++;
	else break;
	return distance;
}

int GetReplaceTimes()						//��ȡҳ���û��ܴ���
{
	int i;
	int num = 0;
	for(i=0;i<PVS_NUM;i++)
	num+=replaceArray[i];
	return num;
}
int GetLackTimes()							//��ȡҳ���ж��ܴ���
{
	int i;
	int num = 0;
	for(i=0;i<PVS_NUM;i++)
	num+=lackArray[i];
	return num;
}

#endif // RESOURCE_H_INCLUDED
