// main.c —— Day 10-12 整合版：把9天的零件串成完整采集器
#include <stdio.h>
#include <stdlib.h>
#include "sensor.h"

//全局资源（整个程序共享的"公共财产"，g_前缀=global）
RingBuffer *g_rb = NULL;        //数据暂缓（Day 4）
AlertNode *g_alerts = NULL;     //告警链表头（Day 6），NULL = 空链表

//按类型查阈值：全项目唯一的阈值表
double get_threshold(SensorType type)
{
    switch (type) {
        case SENSOR_TEMP:  return 80.0;      //温度超80告警
        case SENSOR_VOLT:  return 11.0;      //电压低于11告警
        case SENSOR_SPEED: return 100.0;     //速度超100告警
        default:           return 0.0;
    }
}

//采集一轮：3个传感器各采一次（开机采集和菜单采集共用它）
void collect_once(void)
{
    for (int t = 0; t < SENSOR_MAX; t++) {
        SensorData d;
        d.type = (SensorType)t;
        d.value = sensor_read_value(d.type);
        get_timestamp(d.timestamp,sizeof(d.timestamp));     //每条数据自己的时间戳

        rb_write(g_rb,&d);      //存进环形buffer

        if (check_alert(d.type,d.value)) {      //超阈值？
            g_alerts = alert_add(g_alerts,d.type,d.value,get_threshold(d.type));
        }
    }
}

// 菜单操作（桩子 → 真功能）
void op_start(void)
{
    collect_once();
    printf("已采集1轮，新增3条数据\n");
}

//偷看式查看：局部游标走一圈，不动head——看完数据还在缓存里
void op_show(void)
{
    int count = 0;
    printf("=== 当前缓存 ===\n");
    for (int i = g_rb->head; i != g_rb->tail; i = (i + 1) % g_rb->size) {
        SensorData *d = &g_rb->buffer[i];       //只看不取
        printf("    类型%d = %.2f (%s)\n",d->type,d->value,d->timestamp);
        count++;
    }
    if (count == 0) printf("(缓存为空，先采集)\n");
    else            printf("共 %d 条\n",count);
}

void op_alert(void)
{
    printf("=== 告警记录 （最新在最前） ===\n");
    if (g_alerts == NULL) printf(" （暂无告警）\n");
    alert_print(g_alerts);
}

void op_save(void)
{
    if (rb_is_empty(g_rb)) {
        printf("缓存为空，没有可保存的数据，先采集！\n");
        return;
    }
    if (save_to_csv(g_rb, "sensor_data.csv") == 0)
        printf("已保存到 sensor_data.csv（缓存已清空）\n");
    else
        printf("保存失败！检查文件是否被Excel占用\n");
}

void op_exit(void) { printf("再见！\n");}

int main(void)
{
    srand(66);
    g_rb = rb_create(64);           // 出生：申请缓存

    printf("=== 车载传感器数据采集器 ===\n");
    for (int i = 0; i < 5; i++) {
        collect_once();
    }
    printf("开机自检：已预采集5轮（15条）\n");

    void (*ops[])(void) = {op_start,op_show,op_alert,op_save,op_exit};
    const char *name[] = {"采集一轮", "查看数据", "查看告警", "保存CSV", "退出"};
    int count = sizeof(ops) / sizeof(ops[0]);
    printf(">>> 菜单项数: %d\n", count);   // 诊断用，查完删

    while(1)
    {
        printf("\n===车载传感器采集器===\n");
        for (int i = 0; i < count; i++) {
            printf("  %d. %s\n",i,name[i]);
        }
        printf("输入编号:");

        int choice;
        if (scanf("%d",&choice) != 1) {
            while (getchar() != '\n');
            printf("请输入数字!\n");
            continue;
        }
        if (choice < 0 || choice >= count) {
            printf("无效选择\n");
            continue;
        }
        ops[choice]();
        if (choice == count - 1) break;
    }

    alert_free(g_alerts);       // 死亡前：还清所有借的内存
    rb_free(g_rb);
    printf("程序结束\n");
    return 0;
}