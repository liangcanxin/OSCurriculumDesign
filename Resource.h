#ifndef RESOURCE_H_INCLUDED
#define RESOURCE_H_INCLUDED

///����Դ�벿�ֲο�������Դ(https://blog.csdn.net/u012321457/article/details/80714640)
#include<iostream>
#include<stdlib.h>
#include<string.h>
#include<iomanip>
#include<stdio.h>
using namespace std;

///-----------------------ȫ�ֱ���-------------------------
int BLOCKNUM;		//ҳ����
int *PVS;			//PageVisitSequenceҳ��������У�ҳ��ַ����
int PVS_NUM;		//ҳ��������г��ȣ�ҳ��ַ�����ȣ�
int *replaceArray;	//ҳ���û���־���飨��СΪ����ҳ��Ĵ������洢ÿ�η����Ƿ����ҳ���û�������Ҫ�û�ʱ��������Ϊ1����֮Ϊ0��
int *lackArray;		//ȱҳ�жϱ�־���飨��СΪ����ҳ��Ĵ������洢ÿ�η����Ƿ����ȱҳ�жϣ��������ж�ʱ��������Ϊ1����֮Ϊ0��
///--------------------------------------------------------

class BLOCK;        //ǰ����������

///-----------------------��������-------------------------
void InputAndInit();						//��������ͱ�����ʼ��
void ReplaceFIFO(BLOCK block);				//FIFOҳ���û��㷨
int FindPage(int pageID,BLOCK block);		//ҳ����ң�����ҳ������������в���ҳ���Ƿ���ڣ�
void ReplaceLRU(BLOCK block);				//LRUҳ���û��㷨
void ReplaceOPT(BLOCK block);			    //OPTҳ���û��㷨
int GetReplaceTimes();						//��ȡҳ���û��ܴ���
int GetLackTimes();							//��ȡȱҳ�ж��ܴ���
///---------------------------------------------------------

/***** BLOCK *****
** function: �Զ���C++�࣬��������Ҫ���ڴ洢ҳ��ַ�����ݺ�������ҳ�����
*/
class BLOCK{

    public:         //Ϊ������ã�����Ĳ���������Ϊ���б������淶��дӦ������Ϊ˽�б�����Ȼ�����getter()��setter()����
	int blockNum;	//ҳ������
	int pageNum;	//����ҳ�����Ѵ��ڵ�������
	int *pageID;	//����ҳ�����Ѵ洢�����ݵ�ҳ��ţ���СΪblockNum��
	int *stayTime;	//ҳ���ھ���ҳ���е�ͣ��ʱ�䣨��ҳ��Ŷ�Ӧ��

	//��ʼ��BLOCK����ز���
	BLOCK(int num)
	{
		int i;
		pageNum=0;
		blockNum=num;
		pageID=new int[num];
		stayTime=new int[num];
		for(i=0;i<num;i++)
		{
			pageID[i]=-1;	//��ʼ��ÿ��ҳ����û�����ݣ�ҳ�������Ϊ-1
			stayTime[i]=0;	//��ʼ��ͣ��ʱ��Ϊ0
		}
	}
	/*//��ȡҳ��ַ������
	void ShowPage()
	{
		int i;
		for(i=0;i<blockNum;i++)
		cout<<"Page["<<i<<"]: "<<pageID[i]<<endl;
	}*/

	/*//��ȡĳ��ҳ����ҳ���е�ͣ��ʱ��
	void ShowStayTime()
	{
		int i;
		for(i=0;i<blockNum;i++)
            cout<<"Stay["<<i<<"]: "<<stayTime[i]<<endl;
	}*/

	//��ȡ��ҳ����ͣ��ʱ�����ҳ������ҳ���
	int GetLongestStay()
	{
		int i;
		int max_pos=0;
		for(i=0;i<pageNum;i++)
		if(stayTime[max_pos]<stayTime[i])
            max_pos=i;
		return max_pos;
	}

    //��ȡҳ���Ż�ȡ��ҳ���ڵ�ҳ�����δʹ�õ�ʱ�䣨�ĸ����뵱ǰpageID��Զ��
	int GetDistance(int currentPageID,int page)
    {
        int distance=0;
        int i;
        for(i=currentPageID-1;i>=0;i--)
            if(PVS[i]!=page)
                distance++;
            else break;
        return distance;
    }

	//��ȡ��ҳ����������δʹ�õ�ҳ���
	int GetRencentNotUse(int currentPageID)
	{
		//Ĭ��currentPageIDһ�����ڵ���BLOCKNUM(Ҳ���Ǵ�ʱҳ��������)
		int i;
		int DestID=0;
		for(i=0;i<blockNum;i++)
		{
			if(GetDistance(currentPageID,pageID[i])>GetDistance(currentPageID,pageID[DestID]))
                DestID=i;
		}
		return DestID;
	}

	//����ҳ���Ż�ȡ��ҳ���ڵ�ҳ�����������õ���ʱ�䣨�ĸ����뵱ǰpageID��Զ��
	int GetDistanceToFuture(int currentPageID,int page)
    {
        int distance=0;
        int i;
        for(i=currentPageID+1;i<=PVS_NUM;i++)
            if(PVS[i]!=page)
                distance++;
            else break;
        return distance;
    }

	//��ȡ��ҳ������������ʹ�õ���ҳ���
	int GetfutureNotUse(int currentPageID)
	{
	    //Ĭ��currentPageIDһ�����ڵ���BLOCKNUM(Ҳ���Ǵ�ʱҳ��������)
	    int i;
	    int DestID=0;
	    for(i=0;i<blockNum;i++)
		{
			if(GetDistanceToFuture(currentPageID,pageID[i])>GetDistance(currentPageID,pageID[DestID]))
                DestID=i;
		}
		return DestID;
	}
};

/***** InputAndInit *****
** param: blockNum, *a, amount
** return: null
** function: ���ݵ����ȫ�ֱ�����ʼ��
*/
void InputAndInit(int blockNum, int *a, int amount)
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
	    //ҳ���û���־�����ʼ��Ϊ0
		replaceArray[i]=0;
		//ȱҳ�жϱ�־�����ʼ��Ϊ0
		lackArray[i]=0;
	}
}

/***** FindPage *****
** param: pageID, block
** return: 1/0
** function: ҳ����ң�����ҳ����ڶ�Ӧҳ�򣨳���ΪpageNum���в���ҳ���Ƿ����
*/
int FindPage(int pageID,BLOCK block)
{
	int i=0;
	for(i=0;i<block.pageNum;i++){
        if(block.pageID[i]==pageID)
        break;
	}
	//��ҳ����ڣ��򷵻�1�����򷵻�0
	return !(i==block.pageNum);
}

/***** ReplaceFIFO *****
** param: block
** return: null
** function: FIFOҳ���û��㷨
*/
void ReplaceFIFO(BLOCK block)
{
	int i,j;
	int replacePosition;				                //���û�λ��
	for(i=0;i<PVS_NUM;i++)				                //���ζ�ҳ��������е�ÿһ��ҳ��PVS[i]���в���
	{
		for(j=0;j<block.pageNum;j++)
		block.stayTime[j]++;			                //ÿѭ��һ�Σ�ҳ��0~pageNum��ͣ��ʱ������
		if(block.pageNum<block.blockNum)
		{
			if(!FindPage(PVS[i],block))	                //��ҳ��PVS[i]������
			{
			    //lackArray[i]=1;                       //���ڷ���ҳ�治�������ҳ���ж�(������Ĭ��ҳ��δ���������ж���)
				block.pageID[block.pageNum]=PVS[i];
				block.pageNum++;
			}
		}
		else 						                    //FIFO�㷨���û���ҳ����ͣ��ʱ�����ҳ������ҳ��ţ�
		{
			if(!FindPage(PVS[i],block))	                //��ҳ��PVS[i]������
			{
				replaceArray[i]=1;		                //���ڷ���ҳ�治�������޿���ҳ��λ�����ҳ���û�
				lackArray[i]=1;			                //���ڷ���ҳ�治�������ҳ���ж�
				replacePosition=block.GetLongestStay();
				block.pageID[replacePosition]=PVS[i];	//ѡ����ҳ����ͣ��ʱ�����ҳ������ҳ����û�
				block.stayTime[replacePosition]=0;		//�û��󣬸�ҳ������ҳ��λ��ͣ��ʱ������
			}
		}
	}
}

/***** ReplaceLRU *****
** param: block
** return: null
** function: LRUҳ���û��㷨
*/
void ReplaceLRU(BLOCK block)
{
	int i,j;
	int replacePosition;				                //���û�λ��
	for(i=0;i<PVS_NUM;i++)				                //���ζ�ҳ��������е�ÿһ��ҳ��PVS[i]���в���
	{
		for(j=0;j<block.pageNum;j++)
		block.stayTime[j]++;			                //ÿѭ��һ�Σ�ҳ��0~pageNum��ͣ��ʱ������
		if(block.pageNum<block.blockNum)
		{
			if(!FindPage(PVS[i],block))	                //��ҳ��PVS[i]������
			{
			    //lackArray[i]=1;                       //���ڷ���ҳ�治�������ҳ���ж�(������Ĭ��ҳ��δ���������ж���)
				block.pageID[block.pageNum]=PVS[i];
				block.pageNum++;
			}
		}
		else							                //LRU�㷨���û���ҳ����������δʹ�õ�ҳ��ţ�
		{
			if(!FindPage(PVS[i],block))	                //��ҳ��PVS[i]������
			{
				replaceArray[i]=1;		                //���ڷ���ҳ�治�������޿���ҳ��λ�����ҳ���û�
				lackArray[i]=1;			                //���ڷ���ҳ�治�������ҳ���ж�
				replacePosition=block.GetRencentNotUse(i);
				block.pageID[replacePosition]=PVS[i];	//ѡ����ҳ����������δʹ�õ�ҳ����û�
				block.stayTime[replacePosition]=0;		//�û��󣬸�ҳ������ҳ��λ��ͣ��ʱ������
			}
		}
	}
}

/***** ReplaceOPT *****
** param: block
** return: null
** function: OPTҳ���û��㷨
*/
void ReplaceOPT(BLOCK block)
{
	int i,j;
	int replacePosition;				                //���û�λ��
	for(i=0;i<PVS_NUM;i++)				                //���ζ�ҳ��������е�ÿһ��ҳ��PVS[i]���в���
	{
		for(j=0;j<block.pageNum;j++)
		block.stayTime[j]++;			                //ÿѭ��һ�Σ�ҳ��0~pageNum��ͣ��ʱ������
		if(block.pageNum<block.blockNum)
		{
			if(!FindPage(PVS[i],block))	                //��ҳ��PVS[i]������
			{
			    //lackArray[i]=1;                       //���ڷ���ҳ�治�������ҳ���ж�(������Ĭ��ҳ��δ���������ж���)
				block.pageID[block.pageNum]=PVS[i];
				block.pageNum++;
			}
		}
		else							                //OPT�㷨���û���ҳ������������ʹ�õ���ҳ��ţ�
		{
			if(!FindPage(PVS[i],block))	                //��ҳ��PVS[i]������
			{
				replaceArray[i]=1;		                //���ڷ���ҳ�治�������޿���ҳ��λ�����ҳ���û�
				lackArray[i]=1;			                //���ڷ���ҳ�治�������ҳ���ж�
				replacePosition=block.GetfutureNotUse(i);
				block.pageID[replacePosition]=PVS[i];	//ѡ����ҳ������������ʹ�õ���ҳ����û�
				block.stayTime[replacePosition]=0;		//�û��󣬸�ҳ������ҳ��λ��ͣ��ʱ������
			}
		}
	}
}

/***** GetReplaceTimes *****
** param: null
** return: num
** function: ��ȡҳ���û��ܴ���
*/
int GetReplaceTimes()
{
	int i;
	int num = 0;
	for(i=0;i<PVS_NUM;i++)
	num+=replaceArray[i];
	return num;
}

/***** GetLackTimes *****
** param: null
** return: num
** function: ��ȡҳ���ж��ܴ���
*/
int GetLackTimes()
{
	int i;
	int num = 0;
	for(i=0;i<PVS_NUM;i++)
	num+=lackArray[i];
	return num;
}

#endif // RESOURCE_H_INCLUDED
