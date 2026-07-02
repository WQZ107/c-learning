//定义一个指针数组存星期一到星期日，用 `for` 循环打印

#include<stdio.h>

int main(void)
{
    const char *days[] = {"Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"};

    int n = (sizeof(days) / sizeof(days[0]));

    for(int i = 0;i < n;i++)
    {
        printf("days[%d] = %s\n",i,days[i]);
    }
    return 0;
}