#include<stdio.h>

#define PERM_READ   0
#define PERM_WRITE  1
#define PERM_EXEC   2

#define SET_BIT(reg,n)  ((reg) |= (1 << (n)))
#define CLR_BIT(reg,n)  ((reg) &= ~(1 << (n)))
#define GET_BIT(reg,n)  (((reg) >> (n)) & 1 )

void print_perms(int perm)
{
    printf("权限: [读:%s] [写:%s] [执行:%s]\n",
            GET_BIT(perm,PERM_READ)  ? "是" : "否",
            GET_BIT(perm,PERM_WRITE) ? "是" : "否",
            GET_BIT(perm,PERM_EXEC)  ? "是" : "否");
}

int main(void)
{
    int perm = 0;

    SET_BIT(perm,PERM_READ);
    SET_BIT(perm,PERM_WRITE);
    print_perms(perm);

    CLR_BIT(perm,PERM_WRITE);
    print_perms(perm);

    return 0;
}