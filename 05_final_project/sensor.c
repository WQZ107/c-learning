#include "sensor.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* ========== 时间戳（Day 3） ========== */
void get_timestamp(char *buf, int size)
{
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    strftime(buf, size, "%Y-%m-%d %H:%M:%S", t);
}

/* ========== 模拟传感器读数（Day 2） ========== */
double sensor_read_value(SensorType type)
{
    switch (type) {
        case SENSOR_TEMP:
            return 25.0 + (rand() % 200) / 10.0;   // 25.0 ~ 44.9（温度）
        case SENSOR_VOLT:
            return 11.5 + (rand() % 150) / 100.0;  // 11.50 ~ 12.99（电压）
        case SENSOR_SPEED:
            return rand() % 130;                    // 0 ~ 129（速度 km/h）
        default:
            return 0;
    }
}

/* ========== 环形 Buffer（Day 4） ========== */

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

/* ========== 告警链表（Day 6 新增） ========== */

// 头插法：新告警插到链表最前面，返回新的头
AlertNode* alert_add(AlertNode *head, SensorType type, double value, double threshold)
{
    AlertNode *node = (AlertNode *)malloc(sizeof(AlertNode));
    if (!node) return head;                 // 内存不够，原来的链表原样返回

    char ts[20];
    get_timestamp(ts, sizeof(ts));          // 复用 Day 3 的时间戳函数
    strcpy(node->time, ts);
    node->type = type;
    node->value = value;
    node->threshold = threshold;

    node->next = head;                      // 新节点指向原来的头
    return node;                            // 新节点成为新的头
}

// 从头到尾遍历打印（最新的告警在最前面）
void alert_print(AlertNode *head)
{
    AlertNode *cur = head;
    while (cur) {
        printf("[%s] 类型%d = %.1f (阈值%.1f)\n",
               cur->time, cur->type, cur->value, cur->threshold);
        cur = cur->next;                    // 走到下一个节点
    }
}

// 逐个释放，防止内存泄漏
void alert_free(AlertNode *head)
{
    AlertNode *cur = head;
    while (cur) {
        AlertNode *tmp = cur;               // 先记住当前节点
        cur = cur->next;                    // 再移到下一个
        free(tmp);                          // 最后释放刚才那个
    }
}

/* ========== 告警判断（Day 7 新增） ========== */

// 返回 1 = 超阈值要告警，0 = 正常
int check_alert(SensorType type, double value)
{
    switch (type) {
        case SENSOR_TEMP:  return value > 80.0;    // 温度超80告警
        case SENSOR_VOLT:  return value < 11.0;    // 电压低于11告警
        case SENSOR_SPEED: return value > 100.0;   // 速度超100告警
        default: return 0;
    }
}

/* ========== CSV存储（Day 8 新增） ========== */

// 把环形buffer里剩的数据全部倒进CSV文件
// 返回 0 成功，-1 失败
int save_to_csv(RingBuffer *rb,const char *filename)
{
    FILE *fp = fopen(filename, "w");        // ① 打开文件（w=不存在就建，存在就清空）
    if (!fp) {                              //    打不开（路径错/文件被占用）直接失败
        return -1;
    }

    fprintf(fp,"type, value, timestamp\n"); // ② 先写表头

    SensorData d;
    while(rb_read(rb, &d) == 0) {           // ③ 把buffer逐条读空
        fprintf(fp,"%d,%.2f,%s\n",d.type,d.value,d.timestamp);
    }

    fclose(fp);                             // ④ 关闭（必须！）
    return 0;
}