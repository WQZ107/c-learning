#include <stdio.h>
#include <stdlib.h>
#include "sensor.h"

// ── 菜单对应的操作函数（Day 9 先是桩子，Day 10 换成真功能）──
void op_start(void) { printf("== 开始采集 ==\n"); }
void op_stop(void)  { printf("== 停止采集 ==\n"); }
void op_show(void)  { printf("== 显示当前数据 ==\n"); }
void op_alert(void) { printf("== 显示告警 ==\n"); }
void op_save(void)  { printf("== 保存CSV ==\n"); }
void op_exit(void)  { printf("== 退出 ==\n"); }

int main(void)
{
    // ★ 函数指针数组：下标0放op_start的地址，1放op_stop的地址
    void (*ops[])(void) = {op_start,op_stop,op_show,op_alert,op_save,op_exit};
    const char *name[] = {"开始采集","停止采集","显示数据","查看警告","保存CSV","退出"};

    int count = sizeof(ops) / sizeof(ops[0]);        // 自动算出菜单有几项(6)

    while (1)
    {
        printf("\n===车载传感器采集器===\n");
        for (int i = 0; i < count; i++) {
            printf("  %d. %s\n",i,name[i]);
        }
        printf("输入编号:");

        int choice;
        if (scanf("%d",&choice) != 1) {     //输入的不是数字（比如abc）
            while(getchar() != '\n');       //把垃圾输入清掉
            printf("请输入数字! \n");
            continue;
        }

        if (choice < 0 || choice >= count) {    //数字越界
            printf("无效选择\n");
            continue;
        }
        if (choice  == count - 1) break;        //最后一项 = 退出

        ops[choice]();                          // ★ 核心：按下标取出函数地址，直接调用

    }
    return 0;
    
}