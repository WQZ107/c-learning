//第 3 题（位域）：ABS 故障标志
//场景：ABS 控制器需要一个 1 字节的故障标志，包含 4 个轮子故障位 + 1 个传感器异常位 + 3 个预留位。

#include<stdio.h>
#include<stdint.h>
#include<string.h>

#pragma pack(push, 1) // 设置结构体按 1 字节对齐
typedef struct {
    unsigned char flFault : 1; // 前左轮故障
    unsigned char frFault : 1; // 前右轮故障
    unsigned char rlFault : 1; // 后左轮故障
    unsigned char rrFault : 1; // 后右轮故障
    unsigned char sensorError : 1; // 传感器异常
    unsigned char reserved : 3; // 预留位,必须保持为0
}AbsFault_t;
#pragma pack(pop) // 恢复默认对齐方式

//统计四个轮子故障个数（不含sensorError）
int CountFaults(const AbsFault_t *pAbsFault) {
    return pAbsFault->flFault + pAbsFault->frFault + pAbsFault->rlFault + pAbsFault->rrFault;
}

//检查是否有任何故障（包括传感器异常）
int AnyFault(const AbsFault_t *pAbsFault) {
    return pAbsFault->flFault || pAbsFault->frFault || pAbsFault->rlFault || pAbsFault->rrFault || pAbsFault->sensorError;
}

//全部清零
void ClearAll(AbsFault_t *pAbsFault) {
    memset(pAbsFault,0,sizeof(AbsFault_t));
}

int main(void){
    AbsFault_t fault = {0}; // 初始化为全零

    printf("Initial fault status: %d\n", AnyFault(&fault)); // 输出初始状态

    //置位flFaultht和sensorErr
    fault.flFault = 1;
    fault.sensorError = 1;

    printf("CountFaults = %u\n",CountFaults(&fault));// 输出故障个数
    printf("AnyFault = %d\n",AnyFault(&fault));// 输出是否有故障

    ClearAll(&fault); // 清零所有故障标志

    printf("After clear: \n");
    printf("CountFaults = %u\n",CountFaults(&fault));// 输出故障个数
    printf("AnyFault = %d\n",AnyFault(&fault));// 输出是否有故障

    return 0;
}