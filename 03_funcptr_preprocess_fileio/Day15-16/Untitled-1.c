//#ifdef 条件编译
#include<stdio.h>
//#define DEBUG

int main(void)
{
    #ifdef DEBUG
    printf("调试模式运行中\n");
    #endif
    printf("程序正常执行\n");

    #if VERSION >= 2
    printf("这是版本2的新功能\n");
    #endif

    return 0;
}