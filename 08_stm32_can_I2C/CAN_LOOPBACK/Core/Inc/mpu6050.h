#ifndef __MPU6050_H
#define __MPU6050_H
#include "main.h"

#define MPU6050_ADDR       (0x68 << 1)   /* 7位地址0x68,HAL要8位格式=0xD0 */
#define MPU6050_WHO_AM_I   0x75
#define MPU6050_PWR_MGMT   0x6B
#define MPU6050_ACCEL_CFG  0x1C
#define MPU6050_ACCEL_XOUT 0x3B

int  MPU6050_Init(void);                    /* 0=成功 -1=没找到 */
void MPU6050_ReadAccel(int16_t *ax, int16_t *ay, int16_t *az);
void I2C_Scan(void);

#endif
