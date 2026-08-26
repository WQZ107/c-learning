//迷你练习 4：条件编译 - 车型开关
#include<stdio.h>

#define PLATFORM_EV     1
#define PLATFORM_HEV    2
#define MY_PLATFORM PLATFORM_EV

#if MY_PLATFORM == PLATFORM_EV
    #define TIRE_SIZE "225/45R18"
#else
    #define TIRE_SIZE "205/55R16"
#endif

int main(void)
{
    printf("轮胎规格: %s\n",TIRE_SIZE);

    return 0;
}