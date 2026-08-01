#include<stdio.h>

//定义掩码常量（仅此处允许使用数字）
#define BIT_IGNITION_OK      (1 << 0)  //bit0
#define BIT_WATER_TEMP_HIGH  (1 << 2)  //bit2
#define BIT_OIL_PRESSURE_LOW (1 << 4)  //bit4
#define BIT_COLD_START       (1 << 7)  //bit7

//打印8位二进制数（循环控制数字不参与status的位操作）
void print_binary(unsigned char status){
    for(int i = 7;i >= 0;i--){
        putchar((status & (1 << i)) ? '1' : '0');
    }
    putchar('\n');
}

int main(){
    unsigned char status = 0; //初始化状态为0

    //1.初始化后打印
    printf("Initial status:   ");
    print_binary(status);

    //2.置位：点火正常、冷启动中
    status |= BIT_IGNITION_OK | BIT_COLD_START;
    printf("After setting ignition OK and cold start: ");
    print_binary(status);

    //3.读位：判断点火是否正常
    printf("Is ignition OK? %s\n", (status & BIT_IGNITION_OK) ? "Yes" : "No");

    //4.清位：清除冷启动中
    status &= ~BIT_COLD_START;
    printf("After clearing cold start: ");
    print_binary(status);

    //5.翻转：水温过高位
    status ^= BIT_WATER_TEMP_HIGH;
    printf("After toggling water temperature high: ");
    print_binary(status);

    //6.再置位机油压力过低
    status |= BIT_OIL_PRESSURE_LOW;
    printf("After setting oil pressure low: ");
    print_binary(status);

    //7.一次清掉bit4和bit2
    status &= ~(BIT_OIL_PRESSURE_LOW | BIT_WATER_TEMP_HIGH);
    printf("After clearing oil pressure low and water temperature high: ");
    print_binary(status);

    return 0;
}