// // main.c 测试
// #include <stdio.h>
// #include <stdlib.h>
// #include "sensor.h"

// int main(void)
// {
//     srand(66);   // 固定随机种子，方便复现

//     for (int i = 0; i < 5; i++) {
//         char buf[20];
//         get_timestamp(buf,sizeof(buf));

//         double temp  = sensor_read_value(SENSOR_TEMP);
//         double volt  = sensor_read_value(SENSOR_VOLT);
//         double speed = sensor_read_value(SENSOR_SPEED);

//         printf("温度：%.1f, 电压：%.2f, 速度：%-4.0f, 现在时间：%s\n",
//         temp,volt,speed,buf);
//     }
//     return 0;
// }
#include <stdio.h>
#include <stdlib.h>
#include "sensor.h"

int main(void)
{
    RingBuffer *rb = rb_create(5);   // 容量5（实际最多存4个）
    SensorData d;

    // 写3个
    for (int i = 0; i < 3; i++) {
        d.type = SENSOR_TEMP;
        d.value = 30 + i;
        rb_write(rb, &d);
    }

    // 读（注意：这里会一口气读完，不是只读2个）
    while (rb_read(rb, &d) == 0) {
        printf("读到: %.1f\n", d.value);
    }

    // 再写2个（会绕圈）
    for (int i = 0; i < 2; i++) {
        d.value = 40 + i;
        rb_write(rb, &d);
    }

    // 把剩余的读完
    while (rb_read(rb, &d) == 0) {
        printf("再读到: %.1f\n", d.value);
    }

    rb_free(rb);
    return 0;
}