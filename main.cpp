#include "OSCurriculumDesign.h"
#include "Resource.h"

int main()
{
    int i=0;
    int amount = 400;                           //产生的随机数数量
    int a[amount];
    int minAddr=0,maxAddr=0;                    //随机数的最低地址和最高地址
    DepositRandomAddr(amount,a,minAddr,maxAddr);//此时，a数组是用于存放随机数的地址流数组
    //printf("随机数的地址范围：%d - %d\n",minAddr,maxAddr);//调试1：随机数的地址流范围
    /*for(int i=0;i<amount;i++)                 //调试2：输出所有随机数的具体地址分布情况
    {
        if(i==0)printf("具体的随机数地址分布情况:\n");
        printf("%d  ",a[i]);
        if((i+1)%10 == 0&&i!=amount-1)printf("\n");
    }*/

    DepositPageAddr(amount,a,minAddr,maxAddr);  //此时，a数组是用于存放随机数的页地址流数组
    //printf("每个数据占用空间大小: %d\n",((maxAddr-minAddr)/(amount-1)));//调试3：随机数的页地址大小
    /*for(int i=0;i<amount;i++)                 //调试4：输出所有随机数的具体页地址分布情况
    {
        if(i==0)printf("具体的随机数所在页地址分布情况:\n");
        printf("%02d\t",a[i]);
        if((i+1)%10 == 0&&i!=amount-1)printf("\n");
    }*/
    printf("页框数\tOPT命中率\tFIFO命中率\tLRU命中率\n");
    int blockNum[40];
    for(i=4; i <= 40;i++){
        blockNum[i]=i;
        InputAndInit(blockNum[i], a, amount);   //初始化相关全局变量数据
        BLOCK block(BLOCKNUM);	                //定义并赋值页框（注意该语句必须在变量和数据初始化后，否则BLOCKNUM未知）

        ReplaceOPT(block);
        printf("[%02d]\t",i);
        //调试5：输出页面的置换次数和缺页中断次数
        //printf("页面置换次数:%03d  ",GetReplaceTimes());
        //printf("缺页中断次数:%03d  ",GetLackTimes());
        printf("OPT:%.4lf\t",1-((GetLackTimes()*1.0)/amount));

        ReplaceFIFO(block);
        //printf("页面置换次数:%03d  ",GetReplaceTimes());
        //printf("缺页中断次数:%03d  ",GetLackTimes());
        printf("FIFO:%.4lf\t",1-((GetLackTimes()*1.0)/amount));

        ReplaceLRU(block);
        //printf("页面置换次数:%03d  ",GetReplaceTimes());
        //printf("缺页中断次数:%03d  ",GetLackTimes());
        printf("LRU:%.4lf\n",1-((GetLackTimes()*1.0)/amount));
    }

    //删除页框的相关初始化数据
    delete[] PVS;
    delete[] replaceArray;
    delete[] lackArray;
    return 0;
}
