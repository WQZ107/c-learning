// // main.c 测试
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sensor.h"

int main(void)
{
    srand(66);
    RingBuffer *rb = rb_create(64);   // Day4: 采集数据缓存
    AlertNode *alerts = NULL;         // Day6: 告警链表头

    printf("=== 采集 15 个传感器数据点 ===\n");

    for (int i = 0; i < 15; i++) {
        // 三种传感器各采一次
        for (int t = 0; t < SENSOR_MAX; t++) {
            SensorData d;
            d.type = (SensorType)t;
            d.value = sensor_read_value(d.type);        // 用正确函数，避开优先级坑
            get_timestamp(d.timestamp, sizeof(d.timestamp));

            rb_write(rb, &d);                            // Day4: 存进环形buffer

            // 判断超阈值（Day7），各自用对应阈值
            if (check_alert(d.type, d.value)) {
                double thresh;
                switch (d.type) {
                    case SENSOR_TEMP:  thresh = 80.0; break;
                    case SENSOR_VOLT:  thresh = 11.0; break;
                    case SENSOR_SPEED: thresh = 100.0; break;
                    default:           thresh = 0.0;  break;
                }
                alerts = alert_add(alerts, d.type, d.value, thresh);   // Day6: 记入链表
                printf("⚠️ 告警: 类型%d=%.1f (阈值%.1f)\n", d.type, d.value, thresh);
            }
        }
    }

    printf("\n=== 打印所有告警记录（最新的在前）===\n");
    alert_print(alerts);

    alert_free(alerts);     // Day6: 释放链表
    rb_free(rb);            // Day4: 释放环形buffer
    return 0;
}