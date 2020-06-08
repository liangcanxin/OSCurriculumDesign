#ifndef RESOURCE_H_INCLUDED
#define RESOURCE_H_INCLUDED

///以下源码部分参考网络资源
#include<iostream>
#include<stdlib.h>
#include<string.h>
#include<iomanip>
#include<stdio.h>
using namespace std;
int GetDistance(int currentPageID,int page);//按照页面编号获取在物理块内的页面最久未使用的时间（哪个距离当前pageID最远）
int GetDistanceToFuture(int currentPageID,int page);//按照页面编号获取在物理块内的页面最近最不可能用到的时间（哪个距离当前pageID最远）
class BLOCK{
public:
	int blockNum;	//物理块总数
	int pageNum;	//物理块中的页面数量
	int *pageID;	//页面号（大小为blockNum）
	int *stayTime;	//页面在物理块中的停留时间（与物理块ID对应）
	BLOCK(int num)
	{
		int i;
		pageNum=0;
		blockNum=num;
		pageID=new int[num];
		stayTime=new int[num];
		for(i=0;i<num;i++)
		{
			pageID[i]=-1;	//初始化每个物理块中没有放置，页面号表示为-1
			stayTime[i]=0;	//初始化停留时间为0
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
			pageID[i]=-1;	//初始化每个物理块中没有放置，页面号表示为-1
			stayTime[i]=0;	//初始化停留时间为0
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
	int GetLongestStay()	//获取在物理块中停留时间最长的页面所在物理块号
	{
		int i;
		int max_pos=0;
		for(i=0;i<pageNum;i++)
		if(stayTime[max_pos]<stayTime[i])
		max_pos=i;
		return max_pos;
	}
	int GetRencentNotUse(int currentPageID)	//获取在物理块中最近最久未使用的页面编号
	{
		//默认currentPageID一定大于等于BLOCKNUM
		int i;
		int DestID=0;
		for(i=0;i<blockNum;i++)
		{
			if(GetDistance(currentPageID,pageID[i])>GetDistance(currentPageID,pageID[DestID]))
			DestID=i;
		}
		return DestID;
	}
	int GetfutureNotUse(int currentPageID) //获取在物理块中最近最不可能使用到的页面编号
	{
	    //默认currentPageID一定大于等于BLOCKNUM
	    int i;
	    int DestID=0;
	    for(i=0;i<blockNum;i++)
		{
			if(GetDistanceToFuture(currentPageID,pageID[i])>GetDistance(currentPageID,pageID[DestID]))
			DestID=i;
		}
		return DestID;
	}
};	//物理块数据结构定义
//-----------------------全局变量-------------------------
int BLOCKNUM;		//物理块数
int *PVS;			//PageVisitSequence页面访问序列
int PVS_NUM;		//页面访问序列长度
int *replaceArray;	//页面置换标志数组（大小为访问页面的次数，存储每次访问是否进行页面置换）
int *lackArray;		//缺页中断标志数组（大小为访问页面的次数，存储每次访问是否存在缺页中断）
//-----------------------函数声明-------------------------
void InputAndInit();						//数据输入和变量初始化
void ReplaceFIFO(BLOCK block);				//FIFO页面置换算法
int FindPage(int pageID,BLOCK block);		//页面查找（按照页面编号在物理块中查找页面是否存在）
void ReplaceLRU(BLOCK block);				//LRU页面置换算法
void ReplaceOPT(BLOCK block);			//LRU页面置换算法
int GetReplaceTimes();						//获取页面置换总次数
int GetLackTimes();							//获取缺页中断总次数
//----------------------FIFO页面置换算法--------------------------
void ReplaceFIFO(BLOCK block)			//FIFO页面置换算法
{
	int i,j;
	block.Init();
	int replacePosition;				//待置换位置
	for(i=0;i<PVS_NUM;i++)				//依次对页面访问序列的每一个页面PVS[i]进行操作
	{
		for(j=0;j<block.pageNum;j++)
		block.stayTime[j]++;			//每循环一次，物理块（0~pageNum）停留时间自增
		if(block.pageNum<block.blockNum)
		{
			if(!FindPage(PVS[i],block))	//若页面PVS[i]不存在
			{
			    //lackArray[i]=1;         //由于访问页面不存在造成页面中断
				block.pageID[block.pageNum]=PVS[i];
				block.pageNum++;
			}
		}
		else 						//FIFO算法（置换停留时间最长的页面所在物理块位置）
		{

//			TODO:若页面已存在的情况（上述三条语句应该是页面不存在的情况，应加上if(页面在物理块中不存在)的判断）
			if(!FindPage(PVS[i],block))	//若页面PVS[i]不存在
			{
				replaceArray[i]=1;		//由于访问页面不存在且无空闲物理块造成页面置换
				lackArray[i]=1;			//由于访问页面不存在造成页面中断
				replacePosition=block.GetLongestStay();
				block.pageID[replacePosition]=PVS[i];	//选择停留时间最长的页面置换
				block.stayTime[replacePosition]=0;		//置换后，该页面所在物理位置停留时间清零
			}
		}
	}
}
int FindPage(int pageID,BLOCK block)		//页面查找（按照页面编号在以存放页面的物理块（长度为pageNum）中查找页面是否存在）
{
	int i=0;
	for(i=0;i<block.pageNum;i++){
        if(block.pageID[i]==pageID)
        break;
	}
	return !(i==block.pageNum);				//若页面存在，则返回1，否则返回0
}
//----------------------LRU页面置换算法--------------------------
void ReplaceLRU(BLOCK block)			//LRU页面置换算法
{
	int i,j;
	block.Init();
	int replacePosition;				//待置换位置
	for(i=0;i<PVS_NUM;i++)				//依次对页面访问序列的每一个页面PVS[i]进行操作
	{
		for(j=0;j<block.pageNum;j++)
		block.stayTime[j]++;			//每循环一次，物理块（0~pageNum）停留时间自增
		if(block.pageNum<block.blockNum)
		{
			if(!FindPage(PVS[i],block))	//若页面PVS[i]不存在
			{
			    //lackArray[i]=1;         //由于访问页面不存在造成页面中断
				block.pageID[block.pageNum]=PVS[i];
				block.pageNum++;
			}
		}
		else							//FIFO算法（置换停留时间最长的页面所在物理块位置）
		{
//			TODO:若页面已存在的情况（上述三条语句应该是页面不存在的情况，应加上if(页面在物理块中不存在)的判断）
			if(!FindPage(PVS[i],block))	//若页面PVS[i]不存在
			{
				replaceArray[i]=1;		//由于访问页面不存在且无空闲物理块造成页面置换
				lackArray[i]=1;			//由于访问页面不存在造成页面中断
				replacePosition=block.GetRencentNotUse(i);
				block.pageID[replacePosition]=PVS[i];	//选择停留时间最长的页面置换
				block.stayTime[replacePosition]=0;		//置换后，该页面所在物理位置停留时间清零
			}
		}
	}
}

void ReplaceOPT(BLOCK block)			//LRU页面置换算法
{
	int i,j;
	block.Init();
	int replacePosition;				//待置换位置
	for(i=0;i<PVS_NUM;i++)				//依次对页面访问序列的每一个页面PVS[i]进行操作
	{
		for(j=0;j<block.pageNum;j++)
		block.stayTime[j]++;			//每循环一次，物理块（0~pageNum）停留时间自增
		if(block.pageNum<block.blockNum)
		{
			if(!FindPage(PVS[i],block))	//若页面PVS[i]不存在
			{
			    //lackArray[i]=1;         //由于访问页面不存在造成页面中断
				block.pageID[block.pageNum]=PVS[i];
				block.pageNum++;
			}
		}
		else							//FIFO算法（置换停留时间最长的页面所在物理块位置）
		{
//			TODO:若页面已存在的情况（上述三条语句应该是页面不存在的情况，应加上if(页面在物理块中不存在)的判断）
			if(!FindPage(PVS[i],block))	//若页面PVS[i]不存在
			{
				replaceArray[i]=1;		//由于访问页面不存在且无空闲物理块造成页面置换
				lackArray[i]=1;			//由于访问页面不存在造成页面中断
				replacePosition=block.GetfutureNotUse(i);
				block.pageID[replacePosition]=PVS[i];	//选择停留时间最长的页面置换
				block.stayTime[replacePosition]=0;		//置换后，该页面所在物理位置停留时间清零
			}
		}
	}
}

void InputAndInit(int blockNum, int *a, int amount)					//数据输入和变量初始化
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
		replaceArray[i]=0; 					//页面置换标志数组初始化为0
		lackArray[i]=0; 					//缺页中断标志数组初始化为0
	}
}

int GetDistance(int currentPageID,int page)	//按照页面编号获取在物理块内的页面最久未使用的时间（哪个距离当前pageID最远）
{
	int distance=0;
	int i;
	for(i=currentPageID-1;i>=0;i--)
	if(PVS[i]!=page)
	distance++;
	else break;
	return distance;
}

int GetDistanceToFuture(int currentPageID,int page)	//按照页面编号获取在物理块内的页面最近最不可能用到的时间（哪个距离当前pageID最远）
{
	int distance=0;
	int i;
	for(i=currentPageID+1;i<=PVS_NUM;i++)
	if(PVS[i]!=page)
	distance++;
	else break;
	return distance;
}

int GetReplaceTimes()						//获取页面置换总次数
{
	int i;
	int num = 0;
	for(i=0;i<PVS_NUM;i++)
	num+=replaceArray[i];
	return num;
}
int GetLackTimes()							//获取页面中断总次数
{
	int i;
	int num = 0;
	for(i=0;i<PVS_NUM;i++)
	num+=lackArray[i];
	return num;
}

#endif // RESOURCE_H_INCLUDED
