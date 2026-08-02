// 题 G1：结构体 + 位运算混用
// typedef struct {
//     uint8_t gear;      // 挡位 0~6
//     uint8_t status;    // 状态字节：bit0=ABS介入 bit1=ESP介入 bit2=坡道辅助
// } GearStatus_t;
// 置位 ABS 介入和 ESP 介入（用位运算，定义掩码常量）
// 写函数 void ClutchControl(GearStatus_t *pG, uint8_t pressed)：pressed==1 时把挡位降到 0（不管当前几挡），同时清掉 ESP 位
// 打印结果

#include<stdio.h>
#include<stdint.h>

#define BIT_ABS     (1 << 0)    //bit0：ABS介入
#define BIT_ESP     (1 << 1)    //bit1：ESP介入
#define BIT_HILL    (1 << 2)    //bit2：坡道辅助

typedef struct {
    uint8_t gear;       //挡位 0~6
    uint8_t status;     //状态字节：bit0=ABS, bit1=ESP, bit2=坡道辅助
}GearStatus_t;

//离合器控制：pressed==1时挡位归零，并清除ESP介入标志
void ClutchControl(GearStatus_t *pG,uint8_t pressed) {
    if (pressed) {
        pG->gear = 0;           //挡位降到0
        pG->status &= ~BIT_ESP; //清除ESP位
    }
}

//辅助打印：二进制显示status字节
void print_status(const GearStatus_t *pG) {
    printf("gear = %u, status = 0x%02X (二进制：",pG->gear,pG->status);
    for (int i = 7; i >= 0; i--) {
        putchar((pG->status & (1 << i)) ? '1' : '0');
    }
    printf(")\n");
}

int main(void) {
    //初始化：挡位3，状态默认0
    GearStatus_t gs = {.gear = 3, .status = 0};

    //置位ABS和ESP（用位运算）
    gs.status |= (BIT_ABS | BIT_ESP);

    printf("置位 ABS 和 ESP 后：\n");
    print_status(&gs);

    // 调用 ClutchControl，pressed=1 执行降挡和清 ESP
    ClutchControl(&gs,1);

    printf("\n执行 ClutchControl(pressed=1) 后：\n");
    print_status(&gs);

    return 0;
}