/* ============================================================
 * 练习 5：临界区宏 ATOMIC_BLOCK（PC 模拟版）
 *
 * 编译运行：
 *   gcc -o ex05 exercise_05_critical.c
 *   .\ex05.exe
 *
 * 真实 MCU 上用的 CMSIS 函数是 __get_PRIMASK() / __disable_irq() /
 * __set_PRIMASK()；PC 上我们用全局标志模拟"关中断"。
 * 把下面的 SIM 宏换成 CMSIS 函数，这套代码就能上板子。
 * ============================================================ */
#include <stdio.h>
#include <stdint.h>

/* ---- 模拟"中断开关"（PC 版） ---- */
static volatile uint32_t sim_irq_enabled = 1u;   /* 1 = 中断开启 */

static uint32_t sim_get_primask(void) { return sim_irq_enabled ? 0u : 1u; }
static void     sim_disable_irq(void) { sim_irq_enabled = 0u; }
static void     sim_set_primask(uint32_t p) { sim_irq_enabled = (p == 0u); }

/* 真实 MCU 上替换为 CMSIS：
 *   #define GET_PRIMASK()  __get_PRIMASK()
 *   #define DISABLE_IRQ()  __disable_irq()
 *   #define SET_PRIMASK(v) __set_PRIMASK(v)   */
#define GET_PRIMASK()    sim_get_primask()
#define DISABLE_IRQ()    sim_disable_irq()
#define SET_PRIMASK(v)   sim_set_primask(v)

/* ---- 核心：临界区块宏（保存->关中断->执行->恢复） ---- */
#define ATOMIC_BLOCK(...)                                        \
    do {                                                         \
        uint32_t _saved_prm = GET_PRIMASK();   /* 1.保存状态 */  \
        DISABLE_IRQ();                          /* 2.关中断   */ \
        __VA_ARGS__                             /* 3.临界区代码*/ \
        SET_PRIMASK(_saved_prm);                /* 4.恢复状态 */ \
    } while (0)

/* ---- 主循环与"中断"共享的计数器 ---- */
static volatile uint32_t counter = 0u;

/* 模拟中断服务函数：每次触发累加 100 次 */
void sim_irq_handler(void)
{
    for (int i = 0; i < 100; i++) {
        counter++;          /* 读-改-写，三步，非原子！ */
    }
}

/* 模拟"读-改-写被打断"：读之后、写之前插入了中断 */
void sim_rmw_add(void)
{
    uint32_t tmp = counter;   /* ① 读 */
    sim_irq_handler();        /* ② 被打断！中断把 counter 加到 tmp+100 */
    counter = tmp + 1u;       /* ③ 写旧值+1 → 中断的 100 次累加全丢了 */
}

int main(void)
{
    /* 场景 1：裸奔累加（没有保护，中断插入导致计数丢失） */
    counter = 0u;
    for (int round = 0; round < 100; round++) {
        sim_rmw_add();                    /* 每次都被中断"劫走" 100 次 */
    }
    printf("1) 裸奔累加  结果 = %u   (理论值 10100，丢了 10000！)\n", counter);

    /* 场景 2：临界区保护（原子块内不会被打断）
     * ⚠️ 重要：do{...}while(0) 宏的调用处【必须加分号】！
     *    ATOMIC_BLOCK(counter++;);   ← 注意括号后面还有一个 ;
     *    分号恰好变成 while(0) 的结束符，所以 if/else 场景依然安全。 */
    counter = 0u;
    for (int round = 0; round < 100; round++) {
        ATOMIC_BLOCK(counter++;);         /* 关键：宏调用，展开见注释 */
        sim_irq_handler();                /* 中断只能在原子块外插入 */
    }
    printf("2) 临界区累加结果 = %u   (理论值 10100，一个不丢)\n", counter);

    /* 场景 3：ATOMIC_BLOCK 里放多句代码（同样记得末尾加 ;） */
    uint32_t status = 0u;
    ATOMIC_BLOCK(
        status = 0xA5u;
        counter += 10u;
    );
    printf("3) status = 0x%02X, counter = %u\n", status, counter);

    /* 场景 4：挑战——把宏体从 do{...}while(0) 改成 {...} 试试编译，
     *         下面的 if/else 立刻报错（分号悬空 + else 失配）。
     *         这就是 do{...}while(0) 存在的意义。 */
    if (counter > 0u)
        ATOMIC_BLOCK(counter += 1u;);
    else
        counter = 0u;
    printf("4) if/else + ATOMIC_BLOCK 正常编译运行，counter = %u\n", counter);

    return 0;
}
