/* ============================================================
 * 练习 4：#pragma pack 与 CAN 报文结构体映射（重点！）
 *
 * 编译运行：
 *   gcc -o ex04 exercise_04_packed_can.c
 *   .\ex04.exe
 *
 * 场景：收到一帧 8 字节 CAN 报文，直接用结构体指针解析。
 * ============================================================ */
#include <stdio.h>
#include <stdint.h>

/* ---- 反面教材：默认对齐（有填充，布局和报文对不上） ---- */
typedef struct {
    uint8_t  msg_id;      /* 偏移 0         */
    uint32_t speed;       /* 偏移 4 !!! 前面垫了 3 字节填充 */
    uint16_t rpm;         /* 偏移 8 !!!     */
} BadMsg_t;               /* 大小 12 字节   */

/* ---- 正确做法：pack(1)，布局逐字节对应报文 ---- */
#pragma pack(push, 1)
typedef struct {
    uint8_t  msg_id;      /* Byte0           */
    uint16_t rpm;         /* Byte1~2         */
    uint16_t speed;       /* Byte3~4         */
    uint8_t  gear;        /* Byte5           */
    uint8_t  reserved[2]; /* Byte6~7         */
} EngineMsg_t;            /* 必须正好 8 字节 */
#pragma pack(pop)

/* 编译期检查：大小不对直接编译失败（写错立即报错） */
_Static_assert(sizeof(EngineMsg_t) == 8, "EngineMsg_t 必须是 8 字节！");

/* 模拟收到的 CAN 报文（12 字节缓冲，报文占前 8 字节，后面清零防止越界读）：
 *   msg_id=0x01, rpm=3000(0x0BB8), speed=80(0x0050), gear=2
 *   uint16 按小端存放：低字节在前 */
static const uint8_t can_frame[12] = {
    0x01,                     /* Byte0: msg_id  */
    0xB8, 0x0B,               /* Byte1~2: rpm = 0x0BB8 = 3000 */
    0x50, 0x00,               /* Byte3~4: speed = 0x0050 = 80 */
    0x02,                     /* Byte5: gear */
    0x00, 0x00,               /* Byte6~7: reserved */
    0x00, 0x00, 0x00, 0x00    /* 多留 4 字节，防止 BadMsg_t 越界读 */
};

int main(void)
{
    /* 1. 看大小差异 */
    printf("1) sizeof(BadMsg_t)    = %zu 字节 (有填充)\n", sizeof(BadMsg_t));
    printf("   sizeof(EngineMsg_t) = %zu 字节 (pack(1) 后与报文一致)\n", sizeof(EngineMsg_t));

    /* 2. 正确映射：结构体指针直接指向报文缓冲区 */
    const EngineMsg_t* msg = (const EngineMsg_t*)can_frame;
    printf("2) 正确解析：\n");
    printf("   msg_id = 0x%02X, rpm = %u, speed = %u km/h, gear = %u\n",
           msg->msg_id, msg->rpm, msg->speed, msg->gear);

    /* 3. 错误映射：BadMsg_t 有填充，字段全部错位 */
    const BadMsg_t* bad = (const BadMsg_t*)can_frame;
    printf("3) 错误解析 BadMsg_t：speed = %u (错位了！)\n", bad->speed);

    /* 4. 挑战：把 gear 字段改成 5，重新构造报文并验证
     *    提示：复制 can_frame 到可写数组，改 can_frame[5]，再映射 */
    uint8_t my_frame[8];
    for (int i = 0; i < 8; i++) my_frame[i] = can_frame[i];
    my_frame[5] = 0x05;                          /* 手动改 gear 字节 */
    const EngineMsg_t* my_msg = (const EngineMsg_t*)my_frame;
    printf("4) 修改 gear 字节后解析：gear = %u (应为 5)\n", my_msg->gear);

    return 0;
}
