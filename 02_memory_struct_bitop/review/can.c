//第 4 题（位运算实战）：CAN 信号提取
//场景：收到一帧 CAN 报文 data[8]，DBC（CAN数据库文件）定义的信号规则

#include <stdio.h>
#include <stdint.h>

// 提取 TQ 信号（起始位 10，长度 12，小端）
uint16_t GetTorque(const uint8_t *pData) {
    // 低6位来自 data[1] 的 bit2~bit7，高6位来自 data[2] 的 bit0~bit5
    uint16_t low  = (pData[1] >> 2) & 0x3F;          // 取低6位
    uint16_t high = (pData[2] & 0x3F);               // 取高6位（注意是 data[2] 低6位）
    return (high << 6) | low;
}

// 将 TQ 值写回 data[1] 和 data[2]，其他位保持不变
void SetTorque(uint8_t *pData, uint16_t value) {
    // 清除原信号位
    pData[1] &= ~(0xFC);    // 清除 bit2~bit7（0xFC = 11111100）
    pData[2] &= ~(0x3F);    // 清除 bit0~bit5（0x3F = 00111111）

    // 写入新值
    pData[1] |= (uint8_t)((value & 0x3F) << 2);   // 低6位左移2位到 bit2~bit7
    pData[2] |= (uint8_t)((value >> 6) & 0x3F);   // 高6位写入 bit0~bit5
}

// 辅助：打印8字节数据（仅显示相关两个字节）
void print_data(const uint8_t *pData, const char *msg) {
    printf("%s: data[1]=0x%02X, data[2]=0x%02X\n", msg, pData[1], pData[2]);
}

int main(void) {
    uint8_t data[8] = {0x00, 0xA5, 0x3C, 0x00, 0x00, 0x00, 0x00, 0x00};

    printf("原始数据: data[1]=0x%02X, data[2]=0x%02X\n", data[1], data[2]);

    // 提取 TQ
    uint16_t torque = GetTorque(data);
    printf("提取 TQ = 0x%04X\n", torque);
    // 预期输出：0x0F29（而非 0x3CA5，因为信号只占12位且起始位偏移为2）
    // 若按照题目的验收写 0x3CA5，则与定义不符，故这里为正确结果。

    // 修改 TQ 为新值（例如 0x0ABC），写回
    uint16_t new_torque = 0x0ABC;
    SetTorque(data, new_torque);
    printf("\n写回新值 0x%04X 后：\n", new_torque);
    print_data(data, "数据");

    // 再次提取验证
    uint16_t check = GetTorque(data);
    printf("再次提取 TQ = 0x%04X\n", check);
    if (check == new_torque) {
        printf("验证通过：提取与写入一致。\n");
    } else {
        printf("验证失败！\n");
    }

    // 检查无关位是否受影响（如 data[1] 的 bit0, bit1 和 data[2] 的 bit6, bit7 应保持原样）
    // 初始 data[1]=0xA5 (10100101), 写回后 bit0,bit1 应仍为 1,0? 实际上初始 bit0=1, bit1=0，清除时我们只清除了bit2-7，保留了bit0,1，所以应该不变。
    // data[2]=0x3C (00111100), bit6,7 为0，清除时只清bit0-5，保留bit6,7为0，不变。
    printf("\n无关位检查:data[1] bit0-1 = %d%d, data[2] bit6-7 = %d%d\n",
           (data[1] >> 0) & 1, (data[1] >> 1) & 1,
           (data[2] >> 6) & 1, (data[2] >> 7) & 1);

    return 0;
}