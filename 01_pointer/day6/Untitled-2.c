#include<stdio.h>

typedef int (*MathFunc)(int, int);
int add(int a,int b) {return a + b;}
int sub(int a,int b) {return a - b;}
int mul(int a,int b) {return a * b;}

int main(void)
{
//定义函数指针，指向add
    //int (*fp)(int,int) = add;
    MathFunc fp = add;
//调用函数指针
    printf("add(3,4) = %d\n",fp(3,4));

    //改变指向
    fp = sub;
    printf("sub(3,4) = %d\n",fp(3,4));   

    fp = mul;
    printf("mul(3,4) = %d\n",fp(3,4));

    return 0;
}