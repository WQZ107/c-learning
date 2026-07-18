#include <stdio.h>

typedef struct {
    char type[16];       // 传感器类型：GPS / ACCEL / TEMP
    double value;        // 数值
    char timestamp[20];  // 时间戳
    int status;          // 0=正常, 1=告警
} SensorData;

void print_sensor(const SensorData *s)
{
    printf("[%s] %s = %.2f (%s)\n",
           s->timestamp, s->type, s->value,
           s->status ? "告警" : "正常");
}

int main(void)
{
    SensorData data[] = {
        {"GPS",   23.123456, "2026-06-30 15:30:00", 0},
        {"ACCEL", 1.5,       "2026-06-30 15:30:01", 0},
        {"TEMP",  28.5,      "2026-06-30 15:30:02", 0},
        {"ACCEL", 24.0,      "2026-06-30 15:30:03", 1}  // 急刹车告警
    };

    for (int i = 0; i < 4; i++) {
        print_sensor(&data[i]);
    }

    return 0;
}