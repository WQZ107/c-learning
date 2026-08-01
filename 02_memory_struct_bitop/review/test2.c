//定义+初始化
#include<stdio.h>
#include<stdint.h>

typedef struct {
    uint16_t rpm;          //转速
    uint8_t temp;         //温度
    uint8_t fuelLevel;     //油量
} Engine_t;

int main(void){
    //使用指定初始化器（C99 特性）初始化
    Engine_t engine = {
        .rpm = 3000,
        .temp = 95,
        .fuelLevel = 40
    };

    //打印原始发动机数据
    printf("原始发动机: rpm = %u,temp = %u,fuelLevel = %u\n",
           engine.rpm, engine.temp, engine.fuelLevel);

    // 整体复制结构体（浅拷贝，但对于基础类型结构体完全适用）
    Engine_t engine_copy = engine;

    // 修改副本的rpm值
    engine_copy.rpm = 3500;

    //打印原始和副本的rpm
    printf("原始 rpm: %u\n",engine.rpm);
    printf("副本 rpm: %u\n",engine_copy.rpm);

    return 0;
}