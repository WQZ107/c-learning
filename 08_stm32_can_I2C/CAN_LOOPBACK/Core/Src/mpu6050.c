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
