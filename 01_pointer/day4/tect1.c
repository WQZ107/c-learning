#include<stdio.h>

//定义 `int x=7; int *p=&x; int **pp=&p;`，打印
int main(void)
{
    int x = 7;
    int *p = &x;
    int **pp = &p;

    printf("x = %d\n",x);
    printf("*p = %d\n",*p);
    printf("**pp = %d\n",**pp);
    printf("\n");
    
    printf("x = %p\n",(void*)&x);
    printf("p = %p\n",(void*)p);
    printf("*pp = %p\n",(void*)*pp);
    printf("\n");

    printf("&p = %p\n",(void*)&p);
    printf("pp = %p\n",(void*)pp);
    printf("&pp = %p\n",(void*)&pp);

    return 0;
}