//指针数组
#include<stdio.h>

int main(void)
{
    const char *days[] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

    for (int i = 0;i <7;i++)
    {
        printf("days[%d] = %s\n",i,days[i]);
    }
    return 0;
}