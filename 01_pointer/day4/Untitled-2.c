#include<stdio.h>

void change_pointer(int **pp,int *new_target)
{
    *pp = new_target;
}

int main(void)
{
    int a = 10,b = 20;
    int *p = &a;

    printf("*p = %d\n",*p);

    change_pointer(&p,&b);
    printf("*p = %d\n",*p);

    return 0;
}