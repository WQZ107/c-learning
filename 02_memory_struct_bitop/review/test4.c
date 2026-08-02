//题 C1：修改外部变量。这道题考察的是 指针传参 与 无符号数减法下溢保护

#include<stdio.h>
#include<stdint.h>

//功能：将 *pSpeed 减少 amount，最少减到 0
void Brake(uint8_t *pSpeed, uint8_t amount) {
    if (*pSpeed >= amount) {
        *pSpeed -= amount; 

    }else {
        *pSpeed = 0;
    }
}

int main(void) {
    uint8_t speed = 50;

    printf("初始速度: %u\n",speed);

    //第一次刹车：减10
    Brake(&speed,10);
    printf("第一次刹车后速度: %u\n",speed);

    //第二次刹车：减50(会触底到0)
    Brake(&speed,50);
    printf("第二次刹车后速度: %u\n",speed);

    return 0;
}