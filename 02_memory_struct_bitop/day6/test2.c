#include<stdio.h>

int main(void)
{
    int n;
    printf("输入一个整数:");
    scanf("%d",&n);

    if(n & 1) {
        printf("%d 是奇数\n",n);
    }else {
        printf("%d 是偶数\n",n);
    }

    return 0;
}