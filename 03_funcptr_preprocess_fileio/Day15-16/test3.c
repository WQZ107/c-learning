//test3 : 条件编译
#include<stdio.h>

#define PLATFORM_LINUX 1
#define PLATFORM_WIN   2

#define PLATFORM PLATFORM_LINUX

int main(void)
{
    #if PLATFORM == PLATFORM_LINUX
        printf("Linux平台\n");
    #elif PLATFORM == PLATFORM_WIN
        printf("Windows平台\n");
    #else
        printf("未知平台\n");
    #endif

        return 0;
}