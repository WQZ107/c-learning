#include "sensor.h"
#include <string.h>
#include <stdlib.h>
#include <time.h>

void get_timestamp(char *buf, int size)
{
    time_t now = time(NULL);        //当前时间（秒数）
    struct tm *t = localtime(&now); //转成本地时间结构
    strftime(buf, size, "%Y-%m-%d %H:%M:%S",t);
}

// 模拟传感器读到的值（真实项目里这里会换成读真实硬件）
double sensor_read_value(SensorType type)
{
    switch (type) {
        case SENSOR_TEMP:
            return 25.0 + (rand() % 200) / 10.0;  // 25.0 ~ 44.9（温度）
        case SENSOR_VOLT:
            return 11.5 + (rand() % 150) / 100.0;  // 11.50 ~ 12.99（电压）
        case SENSOR_SPEED:
            return rand() % 130;                    // 0 ~ 129（速度 km/h）
        default:
            return 0;
    }
}