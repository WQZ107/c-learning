#include "sensor.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* ========== 时间戳 ========== */
void get_timestamp(char *buf, int size)
{
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    strftime(buf, size, "%Y-%m-%d %H:%M:%S", t);
}

/* ========== 模拟传感器读数 ========== */
double sensor_read_value(SensorType type)
{
    switch (type) {
        case SENSOR_TEMP:
            return 25.0 + (rand() % 200) / 10.0;
        case SENSOR_VOLT:
            return 11.5 + (rand() % 150) / 100.0;
        case SENSOR_SPEED:
            return rand() % 130;
        default:
            return 0;
    }
    // 注意：如果 switch 里所有 case 都 return 了，编译器不会到这里
    // 但有些编译器会警告，可以加 return 0;
}

/* ========== 环形 Buffer ========== */

RingBuffer* rb_create(int size)
{
    RingBuffer *rb = (RingBuffer *)malloc(sizeof(RingBuffer));
    if (!rb) return NULL;
    rb->buffer = (SensorData *)malloc(size * sizeof(SensorData));
    if (!rb->buffer) {
        free(rb);
        return NULL;
    }
    rb->head = 0;
    rb->tail = 0;
    rb->size = size;
    return rb;
}

int rb_is_empty(RingBuffer *rb)
{
    return rb->head == rb->tail;
}

int rb_is_full(RingBuffer *rb)
{
    return (rb->tail + 1) % rb->size == rb->head;
}

int rb_write(RingBuffer *rb, SensorData *data)
{
    if (rb_is_full(rb)) return -1;
    rb->buffer[rb->tail] = *data;
    rb->tail = (rb->tail + 1) % rb->size;
    return 0;
}

int rb_read(RingBuffer *rb, SensorData *data)
{
    if (rb_is_empty(rb)) return -1;
    *data = rb->buffer[rb->head];
    rb->head = (rb->head + 1) % rb->size;
    return 0;
}

void rb_free(RingBuffer *rb)
{
    if (rb) {
        free(rb->buffer);
        free(rb);
    }
}
