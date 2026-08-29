// sensor.h —— 项目的数据结构定义
#ifndef __SENSOR_H__
#define __SENSOR_H__

#include <stdio.h>

// 传感器类型
typedef enum {
    SENSOR_TEMP = 0,    // 温度
    SENSOR_VOLT,        // 电压
    SENSOR_SPEED,       // 速度
    SENSOR_MAX          // 用于遍历（不实际使用）
} SensorType;

// 一个传感器数据点
typedef struct {
    SensorType type;    // 是什么传感器
    double value;       // 数值
    char timestamp[20]; // 时间 "2026-08-01 10:30:00"
} SensorData;

// 告警记录节点（用链表存）
typedef struct AlertNode {
    char time[20];
    SensorType type;
    double value;
    double threshold;
    struct AlertNode *next;
} AlertNode;

// 环形buffer结构
typedef struct {
    SensorData *buffer;
    int head;       // 读位置
    int tail;       // 写位置
    int size;       // 容量
} RingBuffer;

// 读取传感器数值
double sensor_read_value(SensorType type);

// 获取当前时间字符串
void get_timestamp(char *buf, int size);

//环形buffer操作（Day4新增）
RingBuffer* rb_create(int size);                //创建
int rb_is_empty(RingBuffer *rb);                //判空
int rb_is_full(RingBuffer *rb);                 //判满
int rb_write(RingBuffer *rb,SensorData *data);  //写入
int rb_read(RingBuffer *rb,SensorData *data);   //读出
void rb_free(RingBuffer *rb);                   //释放

// ── 告警链表（Day 6）──
AlertNode* alert_add(AlertNode *head, SensorType type, double value, double threshold);
void alert_print(AlertNode *head);
void alert_free(AlertNode *head);

// ── 告警判断（Day 7）──
int check_alert(SensorType type, double value);

//CSV存储（Day8）
int save_to_csv(RingBuffer *rb,const char *filename);

#endif