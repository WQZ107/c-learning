//示例7：用位运算操作虚拟寄存器
//功能：模拟一个 32 位虚拟寄存器，按位域划分四个 8-bit 字段(控制/状态/地址/数据)，用位运算读写各字段
#include <stdio.h>
#include <stdint.h>

/*位域掩码定义*/
#define REG_CTRL_MASK   0xFF000000u     /* 控制字段 bit[31:24] */
#define REG_STAT_MASK   0x00FF0000u     /* 状态字段 bit[23:16] */
#define REG_ADDR_MASK   0x0000FF00u     /* 地址字段 bit[15: 8] */
#define REG_DATA_MASK   0x000000FFu     /* 数据字段 bit[ 7: 0] */

/*读写字段的辅助宏*/
/* 写入字段：先清空目标位域，再写入新值 */
#define REG_WRITE(reg, mask, shift, val) \
    ((reg) = ((reg) & ~(mask)) | (((uint32_t)(val) << (shift)) & (mask)))

/* 读取字段：掩码提取后右移对齐 */
#define REG_READ(reg, mask, shift) \
    (((reg) & (mask)) >> (shift))

/*通用位操作宏（复用)*/
#define SET(value, bit)     ((value) |= (1u << (bit)))
#define CLR(value, bit)     ((value) &= ~(1u << (bit)))
#define TOGGLE(value, bit)  ((value) ^= (1u << (bit)))
#define GET(value, bit)     (((value) >> (bit)) & 1u)

int main(void)
{
    uint32_t virt_reg = 0;      /* 32 位虚拟寄存器 */

    /* 分别写入四个字段（顺序随意） */
    REG_WRITE(virt_reg, REG_CTRL_MASK, 24, 0xA5);   /* 控制字段 */
    printf("写入控制字段 0xA5  ==> 0x%08X\n", virt_reg);

    REG_WRITE(virt_reg, REG_DATA_MASK, 0, 0x3C);    /* 数据字段 */
    printf("写入数据字段 0x3C  ==> 0x%08X\n", virt_reg);

    REG_WRITE(virt_reg, REG_ADDR_MASK, 8, 0x4E);    /* 地址字段 */
    printf("写入地址字段 0x4E  ==> 0x%08X\n", virt_reg);

    REG_WRITE(virt_reg, REG_STAT_MASK, 16, 0x07);   /* 状态字段 */
    printf("写入状态字段 0x07  ==> 0x%08X\n", virt_reg);

    /* 读取各字段 */
    printf("\n读取各字段\n");
    printf("控制字段 = 0x%02X\n", REG_READ(virt_reg, REG_CTRL_MASK, 24));
    printf("状态字段 = 0x%02X\n", REG_READ(virt_reg, REG_STAT_MASK, 16));
    printf("地址字段 = 0x%02X\n", REG_READ(virt_reg, REG_ADDR_MASK, 8));
    printf("数据字段 = 0x%02X\n", REG_READ(virt_reg, REG_DATA_MASK, 0));

    /* 用 SET/CLR/TOGGLE 修改状态字段的第 0 位（对应寄存器 bit16） */
    printf("\n用位宏修改状态字段 bit0\n");
    SET(virt_reg, 16);          /* 置 1 */
    printf("SET bit16   ==> 0x%08X (状态=0x%02X)\n",
           virt_reg, REG_READ(virt_reg, REG_STAT_MASK, 16));

    CLR(virt_reg, 16);          /* 清 0 */
    printf("CLR bit16   ==> 0x%08X (状态=0x%02X)\n",
           virt_reg, REG_READ(virt_reg, REG_STAT_MASK, 16));

    TOGGLE(virt_reg, 16);       /* 翻转 0→1 */
    printf("TOGGLE bit16 ==> 0x%08X (状态=0x%02X)\n",
           virt_reg, REG_READ(virt_reg, REG_STAT_MASK, 16));

    return 0;
}