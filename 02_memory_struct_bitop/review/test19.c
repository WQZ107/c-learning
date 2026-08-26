//中级练习 1：寄存器位操作宏（ST HAL 风格）

#include<stdio.h>
#include<stdint.h>

//ST风格位操作宏家族
#define WRITE_REG(REG, VAL)      ((REG) = (VAL))
#define READ_REG(REG)           ((REG))
#define SET_BIT(REG, BIT)        ((REG) |= (BIT))
#define CLEAR_BIT(REG, BIT)      ((REG) &= ~(BIT))
#define READ_BIT(REG, BIT)       ((REG) & (BIT))
#define TOGGLE_BIT(REG, BIT)     ((REG) ^= (BIT))
#define MODIFY_REG(REG, CLEARMASK, SETMASK) \
    WRITE_REG((REG), (((READ_REG(REG)) & (~(CLEARMASK))) | (SETMASK)))

//模拟的外设寄存器（MCU 上真实写法：*(volatile uint32_t*)0x40020000）
static uint32_t sim_MODER = 0x00000000u;    //GPIO模式寄存器：每位引脚占2位
static uint32_t sim_ODR   = 0x00000000u;    //输出数据寄存器：每引脚1位

int main(void)
{
    /* 1. 配置引脚 5 为输出模式（MODER 位10~11：00=输入 01=输出 10=复用） */
    MODIFY_REG(sim_MODER, (3u << 10), (1u << 10));
    printf("1) MODER = 0x%08X  (引脚5=01 输出模式)\n", sim_MODER);

    /* 2. 读取引脚 5 的模式字段并解读 */
    uint32_t mode5 = READ_BIT(sim_MODER, (3u << 10)) >> 10;
    printf("2) 引脚5模式字段 = %u  (1=输出)\n", mode5);

    /* 3. 置位 / 清位 / 翻转 ODR 的引脚 5 */
    SET_BIT(sim_ODR, (1u << 5));
    printf("3) 置位后   ODR = 0x%08X\n", sim_ODR);
    CLEAR_BIT(sim_ODR, (1u << 5));
    printf("   清位后   ODR = 0x%08X\n", sim_ODR);
    TOGGLE_BIT(sim_ODR, (1u << 5));
    printf("   翻转后   ODR = 0x%08X\n", sim_ODR);

    /* 4. 挑战：把引脚 8 的 2 位字段改成 10（复用功能） */
    MODIFY_REG(sim_MODER, (3u << 16), (2u << 16));
    printf("4) MODER = 0x%08X  (引脚8=10 复用功能)\n", sim_MODER);

    /* 5. 挑战：一次修改两个不连续字段（引脚5清位 + 引脚8置位） */
    MODIFY_REG(sim_MODER, (3u << 10) | (3u << 16), (1u << 10) | (2u << 16));
    printf("5) MODER = 0x%08X  (两处同时改)\n", sim_MODER);

    return 0;
}