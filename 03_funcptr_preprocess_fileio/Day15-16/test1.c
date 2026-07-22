//test1:MAX/MIN宏
#include<stdio.h>

#define MAX(a,b) ((a) > (b) ? (a) : (b))
#define MIN(a,b) ((a) < (b) ? (a) : (b))

int main(void)
{
    printf("MAX(10,20) = %d\n",MAX(10,20));
    printf("MIN(3.5,2.8) = %.1f",MIN(3.5,2.8));

    return 0;
}