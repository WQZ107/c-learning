//题 4：函数指针数组（入门）
//有一个函数数组，请通过循环依次调用每个函数
#include<stdio.h>

int add(int a,int b) {return a + b;}
int sub(int a,int b) {return a - b;}
int mul(int a,int b) {return a * b;}
int divide(int a,int b) {return a ? b / b : 0;}

int main(){
    int x = 10,y = 20;

    // 请定义一个函数指针数组，包含以上 4 个函数
    int (*ops[4])(int,int) = {add,sub,mul,divide};
    char symbols[] = {'+','-','*','/'};

    // 用循环依次调用，打印
    for (int i = 0;i < 4;i++) {
        printf("%d %c %d = %d\n",x,symbols[i],y,ops[i](x,y));
    }
    return 0;
}