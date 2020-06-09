#include "OSCurriculumDesign.h"
#include "Resource.h"

int main()
{
    int i=0;
    int amount = 400;                           //���������������
    int a[amount];
    int minAddr=0,maxAddr=0;                    //���������͵�ַ����ߵ�ַ
    DepositRandomAddr(amount,a,minAddr,maxAddr);//��ʱ��a���������ڴ��������ĵ�ַ������
    //printf("������ĵ�ַ��Χ��%d - %d\n",minAddr,maxAddr);//����1��������ĵ�ַ����Χ
    /*for(int i=0;i<amount;i++)                 //����2���������������ľ����ַ�ֲ����
    {
        if(i==0)printf("������������ַ�ֲ����:\n");
        printf("%d  ",a[i]);
        if((i+1)%10 == 0&&i!=amount-1)printf("\n");
    }*/

    DepositPageAddr(amount,a,minAddr,maxAddr);  //��ʱ��a���������ڴ���������ҳ��ַ������
    //printf("ÿ������ռ�ÿռ��С: %d\n",((maxAddr-minAddr)/(amount-1)));//����3���������ҳ��ַ��С
    /*for(int i=0;i<amount;i++)                 //����4���������������ľ���ҳ��ַ�ֲ����
    {
        if(i==0)printf("��������������ҳ��ַ�ֲ����:\n");
        printf("%02d\t",a[i]);
        if((i+1)%10 == 0&&i!=amount-1)printf("\n");
    }*/
    printf("ҳ����\tOPT������\tFIFO������\tLRU������\n");
    int blockNum[40];
    for(i=4; i <= 40;i++){
        blockNum[i]=i;
        InputAndInit(blockNum[i], a, amount);   //��ʼ�����ȫ�ֱ�������
        BLOCK block(BLOCKNUM);	                //���岢��ֵҳ��ע����������ڱ��������ݳ�ʼ���󣬷���BLOCKNUMδ֪��

        ReplaceOPT(block);
        printf("[%02d]\t",i);
        //����5�����ҳ����û�������ȱҳ�жϴ���
        //printf("ҳ���û�����:%03d  ",GetReplaceTimes());
        //printf("ȱҳ�жϴ���:%03d  ",GetLackTimes());
        printf("OPT:%.4lf\t",1-((GetLackTimes()*1.0)/amount));

        ReplaceFIFO(block);
        //printf("ҳ���û�����:%03d  ",GetReplaceTimes());
        //printf("ȱҳ�жϴ���:%03d  ",GetLackTimes());
        printf("FIFO:%.4lf\t",1-((GetLackTimes()*1.0)/amount));

        ReplaceLRU(block);
        //printf("ҳ���û�����:%03d  ",GetReplaceTimes());
        //printf("ȱҳ�жϴ���:%03d  ",GetLackTimes());
        printf("LRU:%.4lf\n",1-((GetLackTimes()*1.0)/amount));
    }

    //ɾ��ҳ�����س�ʼ������
    delete[] PVS;
    delete[] replaceArray;
    delete[] lackArray;
    return 0;
}
