#ifndef __MPU6050_H
#define __MPU6050_H
#include "main.h"

#define MPU6050_ADDR       (0x68 << 1)   /* 7位地址0x68,HAL要8位格式=0xD0 */
#define MPU6050_WHO_AM_I   0x75
#define MPU6050_PWR_MGMT   0x6B
#define MPU6050_ACCEL_CFG  0x1C
#define MPU6050_ACCEL_XOUT 0x3B
#define MPU6050_GYRO_CFG   0x1B      /* 陀螺量程寄存器,0x00=±250°/s ★Block L */
#define MPU6050_GYRO_XOUT  0x43      /* 陀螺数据首地址(块H 笔记:"0x43起未读"→块L 补上) */
#define MPU6050_GYRO_SENS  131.0f    /* ±250°/s: 131 LSB/(°/s) 手册值, Day② 实测验证 */

int  MPU6050_Init(void);                    /* 0=成功 -1=没找到 */
void MPU6050_ReadAccel(int16_t *ax, int16_t *ay, int16_t *az);
void MPU6050_ReadGyro(int16_t *gx, int16_t *gy, int16_t *gz);   /* ★Block L */
void I2C_Scan(void);

#endif
