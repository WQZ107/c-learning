// 题 E2：位域存数字
// typedef struct {
//     uint8_t temp : 6;     // 6位，能存 0~63
//     uint8_t unit : 1;     // 0=℃ 1=℉
//     uint8_t on    : 1;
// } AcSetting_t;
// 温度存 35（35 < 64，没问题），unit = 0，on = 1
// 打印三项
// 坑题：尝试给 temp 赋 100，打印结果——观察发生了什么，解释为什么
// 验收：能说出"位域溢出被截断"这个现象，并说明这就是为什么位域赋值前要查范围。

#include <stdio.h>
#include <stdint.h>

#pragma pack(push,1);
typedef struct {
    uint8_t temp : 6;     // 6位，能存 0~63
    uint8_t unit : 1;     // 0=℃ 1=℉
    uint8_t on    : 1;
}AcSetting_t;
#pragma pack(pop);

int main(void) {
    AcSetting_t ac = {0};

    //1.正常赋值（35 < 64）
    ac.temp = 35;
    ac.unit = 0;
    ac.on = 1;

    printf("正常赋值\n");
    printf("temp = %u,unit = %u,on = %u\n",ac.temp,ac.unit,ac.on);
    printf("sizeof(AcSetting_t) = %zu\n",sizeof(AcSetting_t));

    // 2. 坑题：尝试给 temp 赋 100（超出 6 位范围）
    ac.temp = 100; // 100 的二进制表示为 1100100，截断后只保留低 6 位，即 001100，等于 36

    printf("坑题赋值\n");
    printf("temp = %u (0x%X),unit = %u,on = %u\n",ac.temp,ac.temp,ac.unit,ac.on);

    //额外观察整体字节值
    uint8_t *pByte = (uint8_t *)&ac;
    printf("整体字节值 = 0x%02X\n",*pByte);

    return 0;
}