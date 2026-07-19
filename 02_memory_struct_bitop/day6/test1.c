#include<stdio.h>

#define SET_BIT(reg,n)     ((reg) |= (1 << (n)))
#define CLR_BIT(reg,n)     ((reg) &= ~(1 << (n)))
#define TOGGLE_BIT(reg,n)  ((reg) ^= (1 << (n)))
#define GET_BIT(reg,n)     ((reg) >> (n) & 1)

void print_binary(unsigned int val)
{
    for (int i = 7;i >=0;i--) {
        printf("%d",(val >> i) & 1);
    }
}

int main(void)
{
    unsigned char reg = 0;

    printf("初始：   "); print_binary(reg); printf("\n");

    SET_BIT(reg,3);
    printf("SET 3:  "); print_binary(reg); printf("\n");

    SET_BIT(reg,5);
    printf("SET 5:  "); print_binary(reg); printf("\n");

    CLR_BIT(reg,3);
    printf("CLR 3:  "); print_binary(reg); printf("\n");

    TOGGLE_BIT(reg,5);
    printf("TOG 5:  "); print_binary(reg); printf("\n");

    printf("GET 5: %d\n",GET_BIT(reg,5));

    return 0;
}