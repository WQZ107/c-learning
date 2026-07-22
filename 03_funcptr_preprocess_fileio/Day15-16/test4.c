//test4 : 带参宏的副作用实验
#include<stdio.h>

#define MAX(a,b) ((a) > (b) ? (a) : (b))

int main(void)
{
    int x = 5,y = 3;
    int z = MAX(x++,y++);
    printf("x=%d y=%d z=%d\n",x,y,z);

    return 0;
}