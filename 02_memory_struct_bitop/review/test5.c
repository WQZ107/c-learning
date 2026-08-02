//题 C2：指针访问结构体
//写函数 void UpdateBattery(Battery_t *pB, float v, float i)，main 里定义电池并调用，然后打印。

#include<stdio.h>

typedef struct Battery {
    float voltage;  //电压
    float current;  //电流
}Battery_t;

//更新电池参数：通过指针修改结构体内容
void UpdateBattery(Battery_t *pB, float v, float i) {
    pB->voltage = v;  //等价于（*pB）.voltage = v;
    pB->current = i;  //等价于（*pB）.current = i;
}
//UpdateBattery 函数用于通过指针更新电池结构体 Battery_t 的电压和电流参数。
//该函数接受一个指向 Battery_t 的指针以及两个浮点数值，分别表示电池的新电压和电流，
//并直接修改结构体的相应字段。

int main(void) {
    //定义并初始化电池（初始值可设为0.0）
    Battery_t battery = {0.0f, 0.0f};

    //调用函数更新电池参数
    UpdateBattery(&battery, 3.4f, 2.3f);

    //打印结果
    printf("Battery voltage: %.2f V\n",battery.voltage);
    printf("Battery current: %.2f A\n",battery.current);
    return 0;
}