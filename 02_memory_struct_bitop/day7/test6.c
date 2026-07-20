//示例6：4个位操作宏（SET / CLR / TOGGLE / GET）
//功能：定义四个常用位操作宏，并演示它们的用法
#include <stdio.h>
#include <stdint.h>

/* SET   —— 将 value 的第 bit 位置 1 */
#define SET(value, bit)     ((value) |= (1u << (bit)))

/* CLR   —— 将 value 的第 bit 位置 0 */
#define CLR(value, bit)     ((value) &= ~(1u << (bit)))

/* TOGGLE—— 将 value 的第 bit 位翻转 */
#define TOGGLE(value, bit)  ((value) ^= (1u << (bit)))

/* GET   —— 读取 value 的第 bit 位的值（0 或 1） */
#define GET(value, bit)     (((value) >> (bit)) & 1u)

int main(void)
{
    uint8_t reg = 0x00;     /* 初始值: 0000 0000 */

    printf("初始值: 0x%02X\n\n", reg);

    /* SET: 将第 2 位置 1 */
    SET(reg, 2);            /* reg = 0000 0100 = 0x04 */
    printf("SET(reg, 2)   ==> 0x%02X\n", reg);

    /* SET: 将第 4 位置 1 */
    SET(reg, 4);            /* reg = 0001 0100 = 0x14 */
    printf("SET(reg, 4)   ==> 0x%02X\n", reg);

    /* GET: 读取第 2 位 */
    printf("GET(reg, 2)   ==> %d\n", GET(reg, 2));

    /* CLR: 将第 2 位清 0 */
    CLR(reg, 2);            /* reg = 0001 0000 = 0x10 */
    printf("CLR(reg, 2)   ==> 0x%02X\n", reg);

    /* TOGGLE: 翻转第 4 位（1→0） */
    TOGGLE(reg, 4);         /* reg = 0000 0000 = 0x00 */
    printf("TOGGLE(reg,4) ==> 0x%02X\n", reg);

    /* TOGGLE: 再翻转第 4 位（0→1） */
    TOGGLE(reg, 4);         /* reg = 0001 0000 = 0x10 */
    printf("TOGGLE(reg,4) ==> 0x%02X\n", reg);

    return 0;
}