//定义 `int *p[5]` 和 `int (*q)[5]`
//分别打印 `sizeof(p)` 和 `sizeof(q)`

#include<stdio.h>

int main(void)
{
    int *p[5];
    int (*q)[5];

    printf("sizeof(p) = %zu (指针数组,5个指针)\n", sizeof(p));
    printf("sizeof(q) = %zu (指向数组,1个指针)\n", sizeof(q));

    return 0;
}