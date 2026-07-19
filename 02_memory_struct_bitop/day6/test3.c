#include<stdio.h>

int main(void){
    int a = 10, b = 20;
    printf("交换前:a = %d, b = %d\n",a,b);

    a ^= b;
    b ^= a;
    a ^= b;

    printf("交换前:a = %d,b = %d",a,b);

    return 0;
}