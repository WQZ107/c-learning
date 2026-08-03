// 练习 2：# 字符串化、## 拼接、可变参数宏（__VA_ARGS__）
//用途场景：DTC 故障码枚举转字符串表、寄存器映射、日志打印

#include <stdio.h>

/* ---- # 字符串化：参数 -> 字符串字面量 ---- */
#define STR(x)     #x

/* ---- ## 拼接：把两个标识符拼成一个 ---- */
#define CAT(a, b)  a ## b

/* ---- 可变参数宏：日志（## 的作用是参数为空时消掉逗号） ---- */
#define LOG(fmt, ...)  printf("[LOG] " fmt "\n", ##__VA_ARGS__)

/* ---- 车载场景：DTC 枚举转字符串 ---- */
#define DTC_STR(name)  #name

typedef enum {
    DTC_NONE = 0,
    DTC_MOTOR_OVERTEMP,
    DTC_BATTERY_LOW
} DtcCode_t;

static const char* const dtc_names[] = {
    DTC_STR(DTC_NONE),
    DTC_STR(DTC_MOTOR_OVERTEMP),
    DTC_STR(DTC_BATTERY_LOW)
};

int main(void)
{
    /* 1. 字符串化 */
    printf("1) STR(hello) -> %s\n", STR(hello));

    /* 2. ## 拼接：声明了两个变量 speed1 / speed2 */
    int CAT(speed, 1) = 100;
    int CAT(speed, 2) = 200;
    printf("2) speed1 = %d, speed2 = %d\n", speed1, speed2);

    /* 3. 可变参数宏 */
    LOG("启动完成，版本 = 0x%04X", 0x0301);
    LOG("无参数调用也能编译");

    /* 4. DTC 字符串表（枚举和字符串永不写错位） */
    printf("4) dtc_names[DTC_MOTOR_OVERTEMP] = %s\n", dtc_names[DTC_MOTOR_OVERTEMP]);

    /* 5. 挑战：用 CAT 拼出变量名来访问 speed1 */
    printf("5) CAT(speed,1) = %d\n", CAT(speed, 1));

    /* 6. 挑战：双层展开。想想 STR(CAT(1,2)) 会输出什么？ */
    printf("6) STR(CAT(1,2)) -> %s\n", STR(CAT(1, 2)));

    return 0;
}