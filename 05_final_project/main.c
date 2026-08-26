// main.c 测试
#include <stdio.h>
#include <stdlib.h>
#include "sensor.h"

int main(void)
{
    srand(66);   // 固定随机种子，方便复现

    for (int i = 0; i < 5; i++) {
        char buf[20];
        get_timestamp(buf,sizeof(buf));

        double temp  = sensor_read_value(SENSOR_TEMP);
        double volt  = sensor_read_value(SENSOR_VOLT);
        double speed = sensor_read_value(SENSOR_SPEED);

        printf("温度：%.1f, 电压：%.2f, 速度：%-4.0f, 现在时间：%s\n",
        temp,volt,speed,buf);
    }
    return 0;
}