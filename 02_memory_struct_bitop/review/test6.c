// 题 E1：多功能方向盘按键
// // 用位域表示方向盘上的按键状态，1字节搞定
// typedef struct {
//     uint8_t volumeUp   : 1;
//     uint8_t volumeDown : 1;
//     uint8_t cruiseOn   : 1;
//     uint8_t cruiseSet  : 1;
//     uint8_t horn       : 1;   // 喇叭
//     uint8_t reserved   : 3;
// } SteeringBtn_t;
// 定义变量，置位 volumeUp 和 horn
// 写函数 int AnyButtonPressed(const SteeringBtn_t *pB)：任一按键按下（1）返回 1
// 打印 sizeof(SteeringBtn_t)
// 验收：AnyButtonPressed 返回 1；sizeof == 1；
//注释说明如果 horn 也松开，结果变为什么。

#include<stdio.h>
#include<stdint.h>

// 使用 #pragma pack 强制 1 字节对齐，确保 sizeof == 1
#pragma pack(push,1)
typedef struct {
    uint8_t volumeUp   : 1;
    uint8_t volumeDown : 1;
    uint8_t cruiseOn   : 1;
    uint8_t cruiseSet  : 1;
    uint8_t horn       : 1;   // 喇叭
    uint8_t reserved   : 3;   // 保留位
}SteeringBtn_t;
#pragma pack(pop)

//检查是否有任何按键被按下(任一有效位为1)
int AnyButtonPressed(const SteeringBtn_t *pB) {
    return pB->volumeUp || pB->volumeDown || pB->cruiseOn ||
              pB->cruiseSet || pB->horn;
}

int main(void) {
    // 定义变量并初始化为 0（所有按键松开）
    SteeringBtn_t buttons = {0};

    // 置位 volumeUp 和 horn
    buttons.volumeUp = 1;
    buttons.horn = 1;

    // 打印 sizeof（应输出 1）
    printf("sizeof(SteeringBtn_t) = %zu\n",sizeof(SteeringBtn_t));

    // 检查是否有按键被按下
    printf("AnyButtonPressed = %d\n", AnyButtonPressed(&buttons));

    //模拟 horn 松开
    buttons.horn = 0;

    // 再次检查是否有按键被按下
    printf("After horn released, AnyButtonPressed = %d\n", AnyButtonPressed(&buttons));

    return 0;
}