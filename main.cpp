#include "OSCurriculumDesign.h"
#include "Resource.h"

//-----------------------ȫ�ֱ���-------------------------
extern int BLOCKNUM;		//�������
extern int *PVS;			//PageVisitSequenceҳ���������
extern int PVS_NUM;		//ҳ��������г���
extern int *replaceArray;	//ҳ���û���־���飨��СΪ����ҳ��Ĵ������洢ÿ�η����Ƿ����ҳ���û���
extern int *lackArray;		//ȱҳ�жϱ�־���飨��СΪ����ҳ��Ĵ������洢ÿ�η����Ƿ����ȱҳ�жϣ�

int main()
{
    int i=0;
    int amount = 400;       //���������������
    int a[amount];
    int minAddr=0,maxAddr=0;//���������͵�ַ����ߵ�ַ
    DepositRandomAddr(amount,a,minAddr,maxAddr);//��ʱ��a���������ڴ��������ĵ�ַ������

    /*for(int i=0;i<amount;i++)       //�������������ľ����ַ�ֲ����
    {
        if(i==0)printf("������������ַ�ֲ����:\n");
        printf("%d  ",a[i]);
        if((i+1)%10 == 0&&i!=amount-1)printf("\n");
    }*/

    DepositPageAddr(amount,a,minAddr,maxAddr);//��ʱ��a���������ڴ���������ҳ��ַ������

    /*for(int i=0;i<amount;i++)       //�������������ľ���ҳ��ַ�ֲ����
    {
        if(i==0)printf("��������������ҳ��ַ�ֲ����:\n");
        printf("%02d\t",a[i]);
        if((i+1)%10 == 0&&i!=amount-1)printf("\n");
    }*/
    printf("ҳ����\tOPT������\tFIFO������\tLRU������\n");
    int blockNum[40];
    for(i=4; i <= 40;i++){
        blockNum[i]=i;
        InputAndInit(blockNum[i], a, amount);
        BLOCK block(BLOCKNUM);	//��������飨ע����������ڱ��������ݳ�ʼ���󣬷���BLOCKNUMδ֪��

        ReplaceOPT(block);
        printf("[%02d]\t",i);
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

        delete[] PVS;
        delete[] replaceArray;
        delete[] lackArray;
    }
    return 0;
}
