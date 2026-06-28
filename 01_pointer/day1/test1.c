#include<stdio.h>

int main() {
    int X = 42;
    int *p = &X;

    printf("X 的值： %d\n",X);
    printf("X 的地址； %p\n",(void*)&X);
    printf("p 存的值： %p\n",(void*)p);
    printf("p 指向的值： %d\n",*p);
    printf("p 自己的地址：%p\n",(void*)&p);

    return 0;
}