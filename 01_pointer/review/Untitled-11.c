//什么是函数指针：函数也有地址！代码编译后存在内存中，函数名就是函数的入口地址
#include<stdio.h>

int add(int a,int b)
{
    return a * b;
}

int main()
{
    printf("add 函数的地址:%p\n",add);
    printf("add 函数的地址:%p\n",&add);

    int (*fp)(int,int);
    fp = add;

    int result = fp(3,4);
    printf("%d\n",result);

    return 0;
}