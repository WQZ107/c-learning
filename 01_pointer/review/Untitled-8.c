//指针与函数
//错误的交流函数，代码示例7：错误的交换函数（传值调用）

#include<stdio.h>

void swap(int x,int y)
{
    int temp = x;
    x = y;
    y = temp;
    printf("函数内: x = %d,y = %d\n",x,y);//函数内确实是交换了
}

int main()
{
    int a = 10,b = 20;
    swap(a,b);
    printf("函数外: a = %d,b = %d\n",a,b);//外面没变化，还是10，20

    //因为传值调用是“拷贝一份”
    //x和y是a和b的副本，修改副本不影响原件
    // 就像你把文件复印一份给别人，别人在复印件上乱画，你的原件不受影响

    return 0;
}