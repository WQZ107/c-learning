// 题 F1：两层嵌套
// typedef struct {
//     float longitude;   // 经度
//     float latitude;    // 纬度
// } GpsPos_t;

// typedef struct {
//     GpsPos_t position;
//     uint8_t  satellites;   // 卫星数
// } Gps_t;

// typedef struct {
//     Gps_t gps;
//     uint8_t speed;
// } Car_t;
// 初始化：经度 116.4、纬度 39.9、卫星 8、车速 60
// 用变量 car 访问并打印纬度
// 定义指针 Car_t *pCar = &car;，用 pCar 修改卫星数为 10，打印
// 把"经度+0.1"这个操作放进函数：void MoveGps(Car_t *pCar, float dx, float dy)，调用后打印新经度

#include <stdio.h>
#include<stdint.h>

typedef struct {
    float longitude;  //经度
    float latitude;   //纬度
}GpsPos_t;

typedef struct {
    GpsPos_t position; // 位置
    uint8_t  satellites; // 卫星数
} Gps_t;

typedef struct {
    Gps_t gps;         // GPS 数据
    uint8_t speed;     // 车速（km/h）
} Car_t;

// 移动 GPS 位置：经度增加 dx，纬度增加 dy
void MoveGps(Car_t *pCar,float dx,float dy) {
    pCar->gps.position.longitude += dx;
    pCar->gps.position.latitude  += dy;
}

int main(void) {
    // 使用指定初始化器初始化（顺序可任意，未指定的成员自动为 0）
    Car_t car = {
        .gps = {
            .position = {
                .longitude = 118.5f,
                .latitude  = 45.3f
            },
            .satellites = 8
        },
        .speed = 60
    };

    // 1. 用变量 car 访问并打印纬度
    printf("初始纬度: %.2f\n",car.gps.position.latitude);

    // 2. 定义指针并修改卫星数
    Car_t *pCar = &car;
    pCar->gps.satellites = 10;

    // 3. 调用 MoveGps 修改经度
    MoveGps(pCar,0.1f,0.0f);

    // 4. 打印最终经度和卫星数
    printf("修改后经度: %.2f\n", car.gps.position.longitude);
    printf("卫星数: %u\n", car.gps.satellites);

    return 0;
}