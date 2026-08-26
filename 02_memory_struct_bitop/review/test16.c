//迷你练习 2：函数宏 - 单位换算（12 行） 车速 ADC 值换算：
#include<stdio.h>

//ADC 12位，0~4095 对应 0~100km/h
#define ADC_TO_SPEED(adc) (((adc) * 100) / 4095)

int main(void) {
    printf("ADC = 2048 -> %d km/h\n",ADC_TO_SPEED(2048));
    printf("ADC = 4095 -> %d km/h\n",ADC_TO_SPEED(4033));
    printf("ADC = 0    -> %d km/h\n",ADC_TO_SPEED(0));

    return 0;
}