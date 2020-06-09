#ifndef RESOURCE_H_INCLUDED
#define RESOURCE_H_INCLUDED

///以下源码部分参考网络资源(https://blog.csdn.net/u012321457/article/details/80714640)
#include<iostream>
#include<stdlib.h>
#include<string.h>
#include<iomanip>
#include<stdio.h>
using namespace std;

///-----------------------全局变量-------------------------
int BLOCKNUM;		//页框数
int *PVS;			//PageVisitSequence页面访问序列（页地址流）
int PVS_NUM;		//页面访问序列长度（页地址流长度）
int *replaceArray;	//页面置换标志数组（大小为访问页面的次数，存储每次访问是否进行页面置换，当需要置换时，数据置为1，反之为0）
int *lackArray;		//缺页中断标志数组（大小为访问页面的次数，存储每次访问是否存在缺页中断，当发生中断时，数据置为1，反之为0）
///--------------------------------------------------------

class BLOCK;        //前置引用声明

///-----------------------函数声明-------------------------
void InputAndInit();						//数据输入和变量初始化
void ReplaceFIFO(BLOCK block);				//FIFO页面置换算法
int FindPage(int pageID,BLOCK block);		//页面查找（按照页面编号在物理块中查找页面是否存在）
void ReplaceLRU(BLOCK block);				//LRU页面置换算法
void ReplaceOPT(BLOCK block);			    //OPT页面置换算法
int GetReplaceTimes();						//获取页面置换总次数
int GetLackTimes();							//获取缺页中断总次数
///---------------------------------------------------------

/***** BLOCK *****
** function: 自定义C++类，在这里主要用于存储页地址流数据和其所在页框情况
*/
class BLOCK{

    public:         //为方便调用，这里的参数都设置为公有变量，规范编写应该设置为私有变量，然后采用getter()和setter()调用
	int blockNum;	//页框总数
	int pageNum;	//单个页框内已存在的数据数
	int *pageID;	//单个页框内已存储的数据的页面号（大小为blockNum）
	int *stayTime;	//页面在具体页框中的停留时间（与页面号对应）

	//初始化BLOCK的相关参数
	BLOCK(int num)
	{
		int i;
		pageNum=0;
		blockNum=num;
		pageID=new int[num];
		stayTime=new int[num];
		for(i=0;i<num;i++)
		{
			pageID[i]=-1;	//初始化每个页框中没有数据，页面号设置为-1
			stayTime[i]=0;	//初始化停留时间为0
		}
	}
	/*//获取页地址流数据
	void ShowPage()
	{
		int i;
		for(i=0;i<blockNum;i++)
		cout<<"Page["<<i<<"]: "<<pageID[i]<<endl;
	}*/

	/*//获取某个页面在页框中的停留时间
	void ShowStayTime()
	{
		int i;
		for(i=0;i<blockNum;i++)
            cout<<"Stay["<<i<<"]: "<<stayTime[i]<<endl;
	}*/

	//获取在页框中停留时间最长的页面所在页面号
	int GetLongestStay()
	{
		int i;
		int max_pos=0;
		for(i=0;i<pageNum;i++)
		if(stayTime[max_pos]<stayTime[i])
            max_pos=i;
		return max_pos;
	}

    //获取页面编号获取在页框内的页面最久未使用的时间（哪个距离当前pageID最远）
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

	//获取在页框中最近最久未使用的页面号
	int GetRencentNotUse(int currentPageID)
	{
		//默认currentPageID一定大于等于BLOCKNUM(也就是此时页框已填满)
		int i;
		int DestID=0;
		for(i=0;i<blockNum;i++)
		{
			if(GetDistance(currentPageID,pageID[i])>GetDistance(currentPageID,pageID[DestID]))
                DestID=i;
		}
		return DestID;
	}

	//按照页面编号获取在页框内的页面最近最不可能用到的时间（哪个距离当前pageID最远）
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

	//获取在页框中最近最不可能使用到的页面号
	int GetfutureNotUse(int currentPageID)
	{
	    //默认currentPageID一定大于等于BLOCKNUM(也就是此时页框已填满)
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
** function: 数据导入和全局变量初始化
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
	    //页面置换标志数组初始化为0
		replaceArray[i]=0;
		//缺页中断标志数组初始化为0
		lackArray[i]=0;
	}
}

/***** FindPage *****
** param: pageID, block
** return: 1/0
** function: 页面查找，按照页面号在对应页框（长度为pageNum）中查找页面是否存在
*/
int FindPage(int pageID,BLOCK block)
{
	int i=0;
	for(i=0;i<block.pageNum;i++){
        if(block.pageID[i]==pageID)
        break;
	}
	//若页面存在，则返回1，否则返回0
	return !(i==block.pageNum);
}

/***** ReplaceFIFO *****
** param: block
** return: null
** function: FIFO页面置换算法
*/
void ReplaceFIFO(BLOCK block)
{
	int i,j;
	int replacePosition;				                //待置换位置
	for(i=0;i<PVS_NUM;i++)				                //依次对页面访问序列的每一个页面PVS[i]进行操作
	{
		for(j=0;j<block.pageNum;j++)
		block.stayTime[j]++;			                //每循环一次，页框（0~pageNum）停留时间自增
		if(block.pageNum<block.blockNum)
		{
			if(!FindPage(PVS[i],block))	                //若页面PVS[i]不存在
			{
			    //lackArray[i]=1;                       //由于访问页面不存在造成页面中断(本程序默认页框未满不算入中断中)
				block.pageID[block.pageNum]=PVS[i];
				block.pageNum++;
			}
		}
		else 						                    //FIFO算法（置换在页框中停留时间最长的页面所在页面号）
		{
			if(!FindPage(PVS[i],block))	                //若页面PVS[i]不存在
			{
				replaceArray[i]=1;		                //由于访问页面不存在且无空闲页框位置造成页面置换
				lackArray[i]=1;			                //由于访问页面不存在造成页面中断
				replacePosition=block.GetLongestStay();
				block.pageID[replacePosition]=PVS[i];	//选择在页框中停留时间最长的页面所在页面号置换
				block.stayTime[replacePosition]=0;		//置换后，该页面所在页框位置停留时间清零
			}
		}
	}
}

/***** ReplaceLRU *****
** param: block
** return: null
** function: LRU页面置换算法
*/
void ReplaceLRU(BLOCK block)
{
	int i,j;
	int replacePosition;				                //待置换位置
	for(i=0;i<PVS_NUM;i++)				                //依次对页面访问序列的每一个页面PVS[i]进行操作
	{
		for(j=0;j<block.pageNum;j++)
		block.stayTime[j]++;			                //每循环一次，页框（0~pageNum）停留时间自增
		if(block.pageNum<block.blockNum)
		{
			if(!FindPage(PVS[i],block))	                //若页面PVS[i]不存在
			{
			    //lackArray[i]=1;                       //由于访问页面不存在造成页面中断(本程序默认页框未满不算入中断中)
				block.pageID[block.pageNum]=PVS[i];
				block.pageNum++;
			}
		}
		else							                //LRU算法（置换在页框中最近最久未使用的页面号）
		{
			if(!FindPage(PVS[i],block))	                //若页面PVS[i]不存在
			{
				replaceArray[i]=1;		                //由于访问页面不存在且无空闲页框位置造成页面置换
				lackArray[i]=1;			                //由于访问页面不存在造成页面中断
				replacePosition=block.GetRencentNotUse(i);
				block.pageID[replacePosition]=PVS[i];	//选择在页框中最近最久未使用的页面号置换
				block.stayTime[replacePosition]=0;		//置换后，该页面所在页框位置停留时间清零
			}
		}
	}
}

/***** ReplaceOPT *****
** param: block
** return: null
** function: OPT页面置换算法
*/
void ReplaceOPT(BLOCK block)
{
	int i,j;
	int replacePosition;				                //待置换位置
	for(i=0;i<PVS_NUM;i++)				                //依次对页面访问序列的每一个页面PVS[i]进行操作
	{
		for(j=0;j<block.pageNum;j++)
		block.stayTime[j]++;			                //每循环一次，页框（0~pageNum）停留时间自增
		if(block.pageNum<block.blockNum)
		{
			if(!FindPage(PVS[i],block))	                //若页面PVS[i]不存在
			{
			    //lackArray[i]=1;                       //由于访问页面不存在造成页面中断(本程序默认页框未满不算入中断中)
				block.pageID[block.pageNum]=PVS[i];
				block.pageNum++;
			}
		}
		else							                //OPT算法（置换在页框中最近最不可能使用到的页面号）
		{
			if(!FindPage(PVS[i],block))	                //若页面PVS[i]不存在
			{
				replaceArray[i]=1;		                //由于访问页面不存在且无空闲页框位置造成页面置换
				lackArray[i]=1;			                //由于访问页面不存在造成页面中断
				replacePosition=block.GetfutureNotUse(i);
				block.pageID[replacePosition]=PVS[i];	//选择在页框中最近最不可能使用到的页面号置换
				block.stayTime[replacePosition]=0;		//置换后，该页面所在页框位置停留时间清零
			}
		}
	}
}

/***** GetReplaceTimes *****
** param: null
** return: num
** function: 获取页面置换总次数
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
** function: 获取页面中断总次数
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
