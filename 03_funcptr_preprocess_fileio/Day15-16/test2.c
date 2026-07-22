//test2 ：调试宏
#include<stdio.h>
#define ENABLE_DEBUG

#ifdef ENABLE_DEBUG

//预处理器定义 DEG 宏，后续代码中所有 DEG(...) 调用都会被替换为 printf("[DEBUG] " ...)
#define DEG(fmt,...) printf("[DEBUG] " fmt "\n",##__VA_ARGS__)
#else
#define DEG(fmt,...)
#endif

int main(void)
{
    int x = 42;
    //printf("[DEBUG] x = %d\n", x);
    DEG("x = %d",x);
    printf("程序运行中...\n");
    return 0;
}