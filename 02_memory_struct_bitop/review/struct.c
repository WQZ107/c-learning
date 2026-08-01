//第 2 题（结构体 + typedef）：轮速传感器
#include<stdio.h>
#include<stdint.h>

//定义结构体类型
typedef struct {
    uint16_t rawCount; //原始计数值
    float speedKmh; //速度值，单位 km/h
    uint8_t isActive; //传感器是否激活，1 表示激活，0 表示未激活
} Wheel_t;

//计算速度并更新isActive
void CalcSpeed(Wheel_t *pWheel) {
    pWheel->speedKmh = pWheel->rawCount * 0.125f; //假设每个计数对应 0.125 km/h
    pWheel->isActive = (pWheel->rawCount > 0) ? 1 : 0;
}

//打印轮速传感器信息（const 指针，只读）
void PrintWheel(const Wheel_t *pWheel) {
    printf("rawCount = %u,speed = %.2f km/h,isActive = %u\n",
           pWheel->rawCount, pWheel->speedKmh, pWheel->isActive);
}

int main() {
    //初始化四个车轮，rawCount 分别为 100， 500， 8， 0
    Wheel_t wheels[4] = {
        {100, 0.0f, 0},
        {500, 0.0f, 0},
        {8, 0.0f, 0},
        {0, 0.0f, 0}
    };

    //计算速度并打印信息
    for (int i = 0; i < 4; i++) {
        CalcSpeed(&wheels[i]);
        printf("Wheel %d:",i + 1);
        PrintWheel(&wheels[i]);
    }

    return 0;
}
