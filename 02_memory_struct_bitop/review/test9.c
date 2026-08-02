// 题 F2：数组嵌套在结构体里
// typedef struct {
//     uint8_t id;        // 传感器ID
//     int16_t value;     // 采样值（可负）
// } Sensor_t;

// typedef struct {
//     Sensor_t sensors[3];   // 3个传感器
//     uint8_t  count;
// } SensorBank_t;
// 初始化 3 个传感器：id 1/2/3，值 100/-50/25
// 写函数 int16_t TotalValue(const SensorBank_t *pBank)：求和返回
// 打印总和

#include<stdio.h>
#include<stdint.h>

typedef struct {
    uint8_t id;
    int16_t value;
}Sensor_t;

typedef struct {
    Sensor_t sensors[3];
    uint8_t count;
}SensorBank_t;

int16_t TotalValue(const SensorBank_t *pBank) {
    int32_t sum = 0;
    for (int i = 0; i < pBank->count; i++) {
        sum += pBank->sensors[i].value;
    }
    return (int16_t)sum;

}

int main(void) {
    SensorBank_t bank = {
        .sensors = {
            [0] = {.id = 1,.value = 100},
            [1] = {.id = 2,.value = -50},
            [2] = {.id = 3,.value = 25}
        },
        .count = 3
    };

    int16_t total = TotalValue(&bank);
    printf("传感器值总和:%d\n",total);

    return 0;
}