//结构体数组
#include<stdio.h>
#include<stdint.h>

typedef struct {
    uint8_t pressure;      // 胎压 bar，放大10倍存储（如 25 → 2.5 bar）
    uint8_t temperature;   // 胎温 ℃，放大10倍存储（如 25 → 2.5 ℃）
}Tire_t;

int main(void) {
    //用指定初始化器初始化 4 个轮胎：左前、右前、左后、右后（顺序可自定）
    // 前轮（索引0,1）：压力 25（2.5bar），温度 30
    // 后轮（索引2,3）：压力 26（2.6bar），温度 32
    Tire_t tires[4] = {
        [0] = {.pressure = 25,.temperature = 30}, // 左前
        [1] = {.pressure = 25,.temperature = 30}, // 右前
        [2] = {.pressure = 26,.temperature = 32}, // 左后
        [3] = {.pressure = 26,.temperature = 32}  // 右后
    };

    //循环打印 4 个轮胎的压力和温度（格式：Tire1:2.5bar,30℃）
    for (int i = 0; i < 4; i++) {
        // 将压力和温度除以 10.0 转换为浮点数
        printf("Tire%d: %.1fbar,%d℃\n",i + 1, tires[i].pressure /10.0,tires[i].temperature);

    }

    //修改第 2 个轮胎（右前）的压力为 27（2.7bar），温度为 31
    tires[1].pressure = 27;
    tires[1].temperature = 31;

    //再次打印验证修改
    printf("\nAfter modification:\n");
    for (int i = 0; i < 4; i++) {
        printf("Tire%d: %.1fbar,%d℃\n",i + 1, tires[i].pressure /10.0,tires[i].temperature);
    }

    return 0;
}