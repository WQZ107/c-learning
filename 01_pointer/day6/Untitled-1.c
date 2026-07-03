#include<stdio.h>

void say_hello(void)
{
    printf("Hello, World!\n");
}

int main(void){

    printf("say_hello 的地址: = %p\n",(void*)say_hello);

    return 0;
}