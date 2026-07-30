//正确的交换函数（传址调用）

#include<stdio.h>

void swap(int *x,int *y)//接收地址
{
    int temp = *x;//根据地址找到a的值
    *x = *y;
    *y = temp;
}

int main()
{
    int a = 10,b = 20;
    swap(&a,&b);//把a和b的地址传过去
    printf("交换后: a = %d,b = %d\n",a,b);

    // 虽然x和y还是副本，但它们存的是a和b的地址！
    // 通过地址就能找到并修改原件！
    // 就像你把家里的钥匙给别人，别人就能直接进你家改东西

    return 0;
}
//要想改实参，就得传地址!