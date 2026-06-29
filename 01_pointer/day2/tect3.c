#include<stdio.h>

int main(void)
{
    int arr[] = {2,34,1,4,2};
    int *p = arr;
    int *end = arr + 5;
    int max = *p;


    p++;
    while(p < end)
    {
        if(*p >max)
        {
            max = *p;
        }
        p++;
    }
    printf("最大值： %d\n",max);
    return 0;
}