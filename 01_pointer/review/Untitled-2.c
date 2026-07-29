#include<stdio.h>

int main()
{

int a = 42;
int *p = &a;
int **pp = &p;

*p = 20;

printf("a 的值: %d\n", a);
printf("&a (地址): %p\n", &a);
printf("p 的值 (存的地址): %p\n", p);
printf("&p (指针自己的地址): %zu\n", &p);
printf("*p (解引用): %d\n", *p);
printf("pp 的值: %p\n", pp);
printf("*pp: %p\n", *pp);
printf("**pp: %d\n", **pp);


return 0;
}