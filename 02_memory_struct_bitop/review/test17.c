//迷你练习 3：陷阱观察
#include<stdio.h>

#define MUL(a,b) a * b
#define MULOK(a,b) ((a) * (b))

int main(void)
{
    printf("MUL(2+3,4) = %d\n",MUL(2+3,4));
    printf("MULOK(2+3,4) = %d\n",MULOK(2+3,4));

    return 0;
}