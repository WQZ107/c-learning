//写一个函数 `redirect(int **pp, int *new_target)`
//让外面的指针指向新的变量

#include<stdio.h>

void redirect(int **pp,int *new_target)
{
    *pp = new_target;
}

int main(void)
{
    int a = 100,b = 200;
    int *p = &a;

    printf("*p = %d(指向a)\n", *p);

    redirect(&p,&b);
    printf("*p = %d(指向b)\n", *p);

    return 0;
}