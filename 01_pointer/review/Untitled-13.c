//通过函数指针调用函数
#include<stdio.h>

int add(int a,int b) {return a + b;}
int sub(int a,int b) {return a - b;}

int main()
{
    int x = 5,y = 6;
    int (*fp)(int,int);
    fp = add;
    printf("%d + %d = %d\n",x,y,fp(x,y));

    fp = sub;
    printf("%d - %d = %d\n",x,y,fp(x,y));

    return 0;
}