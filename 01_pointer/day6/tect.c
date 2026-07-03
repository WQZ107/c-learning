//练习 1：基本函数指针

#include<stdio.h>

int arr(int a,int b) {return a+b;}
int sub(int a,int b) {return a-b;}
int mul(int a,int b) {return a*b;}
int div(int a,int b) {return b ? a / b : 0;}

int main() 
{
    int (*fp)(int,int);

    fp = arr;
    printf("add = %d\n",fp(10,20));

    fp = sub;
    printf("sub = %d\n",fp(10,20));

    fp = mul;
    printf("mul = %d\n",fp(10,20));

    fp = div;
    printf("div = %d\n",fp(10,20));

    return 0;
}