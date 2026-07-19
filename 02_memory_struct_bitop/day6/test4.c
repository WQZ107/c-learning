#include<stdio.h>

#define SET_BIT(reg,n)     ((reg) |= (1 << (n)))
#define CLR_BIT(reg,n)     ((reg) &= ~(1 << (n)))
#define TOGGLE_BIT(reg,n)  ((reg) ^= (1 << (n)))
#define GET_BIT(reg,n)     ((reg) >> (n) & 1)

void print_leds(unsigned char reg)
{
    printf("LED:");
    for (int i = 7;i >= 0;i--) {
        printf("[%s] ",GET_BIT(reg,i) ? "亮" : "灭");
    }
    printf("\n");
}

int main(void)
{
    unsigned char led_reg = 0;
    
    print_leds(led_reg);
    
    SET_BIT(led_reg, 0);    // 点亮 LED0
    SET_BIT(led_reg, 3);    // 点亮 LED3
    SET_BIT(led_reg, 7);    // 点亮 LED7
    print_leds(led_reg);
    
    CLR_BIT(led_reg, 3);    // 熄灭 LED3
    print_leds(led_reg);
    
    TOGGLE_BIT(led_reg, 0); // 翻转 LED0
    print_leds(led_reg);
    
    return 0;
}