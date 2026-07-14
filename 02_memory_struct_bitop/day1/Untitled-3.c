#include <stdio.h>

// 有终止条件的递归
int factorial(int n)
{
    if (n <= 1) return 1;   // 终止条件
    return n * factorial(n - 1);
}

int main(void)
{
    printf("5! = %d\n", factorial(5));   // 120
    return 0;
}