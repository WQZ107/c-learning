#include<stdio.h>
#include<stdint.h>

//打印8位二进制
void print_binary(uint8_t x) {
    for (int i = 7; i >= 0; i--) {
        putchar((x >> i) & 1 ? '1' : '0');
    }
}

int main(void) {
    uint8_t reg = 0xB2; // 10110010

    printf("原始 reg = : ");
    print_binary(reg);
    printf("(0x%02X)\n", reg);

    //1.置位 bit5
    uint8_t r1 = reg | (1 << 5);
    printf("置位 bit5 后 r1 = : ");
    print_binary(r1);
    printf("(0x%02X)\n", r1);

    //2.清位 bit1
    uint8_t r2 = reg & ~(1 << 1);
    printf("清位 bit1 后 r2 = : ");
    print_binary(r2);
    printf("(0x%02X)\n", r2);

    //翻转 bit7
    uint8_t r3 = reg ^ (1 << 7);
    printf("翻转 bit7 后 r3 = : ");
    print_binary(r3);
    printf("(0x%02X)\n", r3);

    //4.读 bit2 的值
    int bit2 = (reg >> 2) & 1;
    printf("读 bit2 的值 = %d\n", bit2);

    return 0;
}