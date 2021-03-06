# 操作系统课程设计

## 模拟场景：模拟实现 OPT（最佳置换）、FIFO 和 LRU 算法，并计算缺页率。

## 具体要求及提示
1. 首先用随机数生成函数产生一个“指令将要访问的地址序列”，然后将地址序列变换
成相应的页地址流（即页访问序列），再计算不同算法下的命中率。
2. 通过随机数产生一个地址序列，共产生 400 条。其中 50%的地址访问是顺序执行的，另外 50%就是非顺序执行。且地址在前半部地址空间和后半部地址空间均匀分布。具体产生方法如下：
1) 在前半部地址空间，即[0，199]中随机选一数 m，记录到地址流数组中（这是
非顺序执行）；
2) 接着“顺序执行一条指令”，即执行地址为 m+1 的指令，把 m+1 记录下来；
3) 在后半部地址空间，[200，399]中随机选一数 m’，作为新指令地址；
4) 顺序执行一条指令，其地址为 m’+1；
5) 重复步骤 1~4，直到产生 400 个指令地址。
3. 将指令地址流变换成页地址（页号）流，简化假设为：
1) 页面大小为 1K（这里 K 只是表示一个单位，不必是 1024B）；
2) 用户虚存容量为 40K；
3) 用户内存容量为 4 个页框到 40 个页框；
4) 用户虚存中，每 K 存放 10 条指令，所以那 400 条指令访问地址所对应的页地
址（页号）流为：指令访问地址为[0，9]的地址为第 0 页；指令访问地址为[10，
19]的地址为第 1 页；……。按这种方式，把 400 条指令组织进“40 页”，并
将“要访问的页号序列”记录到页地址流数组中。
4. 循环运行，使用户内存容量从 4 页框到 40 页框。计算每个内存容量下不同页面置换
算法的命中率，命中率=1-缺页率。输出结果可以为：
页框数 OPT 命中率 FIFO 命中率 LRU 命中率
[4] OPT：0.5566 FIFO：0.4455 LRU：0.5500 
[5] OPT：0.6644 FIFO：0.5544 LRU：0.5588 
 …… ……
 …… ……
[39] OPT：0.9000 FIFO：0.9000 LRU：0.9000 
[40] OPT：1.0000 FIFO：1.0000 LRU：1.0000 

注 1：在某一次实验中，可能 FIFO 比 LRU 性能更好，但足够多次的实验表明 LRU
的平均性能比 FIFO 更好。
注 2：计算缺页率时，以页框填满之后的缺页次数计算（也就是不包含填满前的缺页情况）。

## 运行环境：CodeBlocks

## 相关文件说明
* main.cpp：主程序
* OScurriculumDesign.h：随机数的相关设置
* Resouce.h：关于相关置换算法的具体实现（参考网络）

## 注意事项
本程序基本可以运行，但是在运行程序过程中，有时会出现计算结果错误。多数计算结果是正确的，但是还是有一两次出现错误（笔者能力有限，错误原因目前还未找出……）

## 鸣谢：感谢广大的网络资源，感谢网络大神的资源！

