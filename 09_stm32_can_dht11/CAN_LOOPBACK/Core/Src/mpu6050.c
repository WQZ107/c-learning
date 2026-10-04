#include "mpu6050.h"
#include "i2c.h"
#include <stdio.h>

/* I2C 总线普查:1~127 挨个敲门,应答的报出地址 */
void I2C_Scan(void)
{
    printf("[SCAN] I2C bus online:");
    for (uint8_t addr = 1; addr < 128; addr++) {
        if (HAL_I2C_IsDeviceReady(&hi2c1, addr << 1, 2, 100) == HAL_OK)
            printf(" 0x%02X", addr);
    }
    printf("\r\n");
}

/* ============ Day52:三步曲驱动 ============ */

/* 三步曲:确认在线 → 唤醒 → 配量程。返回 0=成功,-1=没找到 */
int MPU6050_Init(void)
{
    uint8_t who = 0, zero = 0;

    /* —— 第一步:读身份证(0x75)—— */
    if (HAL_I2C_Mem_Read(&hi2c1, MPU6050_ADDR, MPU6050_WHO_AM_I,
                         I2C_MEMADD_SIZE_8BIT, &who, 1, 100) != HAL_OK)
        return -1;                          /* 总线上无人应答=接线/地址错 */
    printf("[MPU] WHO_AM_I=0x%02X ", who);
    if      (who == 0x68) printf("(genuine MPU-60x0)\r\n");
    else if (who == 0x70) printf("(MPU-6500 family clone, regs compatible, go on)\r\n");
    else if (who == 0x98) printf("(fake chip, regs compatible, go on)\r\n");
    else                  printf("(unknown, try anyway)\r\n");
    /* 手册:兼容芯片寄存器通用,校验失败不死磕,直接往下走 */

    /* —— 第二步:叫醒(0x6B 写 0)—— */
    if (HAL_I2C_Mem_Write(&hi2c1, MPU6050_ADDR, MPU6050_PWR_MGMT,
                          I2C_MEMADD_SIZE_8BIT, &zero, 1, 100) != HAL_OK)
        return -1;
    HAL_Delay(50);                          /* 醒了给 50ms 内部时钟稳定 */

    /* —— 第三步:定规矩(量程)—— */
    HAL_I2C_Mem_Write(&hi2c1, MPU6050_ADDR, MPU6050_ACCEL_CFG,
                      I2C_MEMADD_SIZE_8BIT, &zero, 1, 100);   /* 0x1C=0 → ±2g */
    printf("[MPU] init OK (awake, accel +-2g)\r\n");
    return 0;
}

/* 从 0x3B 连读 6 字节 = X高 X低 Y高 Y低 Z高 Z低 */
void MPU6050_ReadAccel(int16_t *ax, int16_t *ay, int16_t *az)
{
    uint8_t buf[6];
    if (HAL_I2C_Mem_Read(&hi2c1, MPU6050_ADDR, MPU6050_ACCEL_XOUT,
                         I2C_MEMADD_SIZE_8BIT, buf, 6, 100) != HAL_OK) {
        *ax = *ay = *az = 0;   /* ★读失败清零:调用方局部变量未初始化,
                                   不清零会把栈垃圾写进 g_vehicle(块I 实测bug) */
        return;
    }
    *ax = (int16_t)((buf[0] << 8) | buf[1]);
    *ay = (int16_t)((buf[2] << 8) | buf[3]);
    *az = (int16_t)((buf[4] << 8) | buf[5]);
}
