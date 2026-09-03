/**
 ****************************************************************************************************
 * @file        main.c
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.0
 * @date        2020-04-20
 * @brief       跑马灯 实验
 * @license     Copyright (c) 2020-2032, 广州市星翼电子科技有限公司
 ****************************************************************************************************
 * @attention
 *
 * 实验平台:正点原子 STM32F103开发板
 * 在线视频:www.yuanzige.com
 * 技术论坛:www.openedv.com
 * 公司网址:www.alientek.com
 * 购买地址:openedv.taobao.com
 *
 ****************************************************************************************************
 */

#include "./SYSTEM/sys/sys.h"
#include "./SYSTEM/delay/delay.h"
#include "./SYSTEM/usart/usart.h"
#include "./BSP/LED/led.h"


int main(void)
{
    HAL_Init();                                 /* 初始化HAL库 */
    sys_stm32_clock_init(RCC_PLL_MUL9);         /* 设置时钟,72M */
    delay_init(72);                             /* 初始化延时函数 */
    led_init();                                 /* 初始化LED */
    usart_init(115200);                         /* 波特率设为115200 */
    
//    uint8_t rx_byte;    /* 存收到的1个字节 */

    while (1)
    {
        /* 阻塞等待收1字节：没收到就在这等100ms，返回值!=HAL_OK就是超时 */
//        if (HAL_UART_Receive(&g_uart1_handle, &rx_byte, 1, 100) == HAL_OK)
//        {
//            printf("收到字符: %c (ASCII: %d)\r\n", rx_byte, rx_byte);
//        }
        
        LED_Toggle();
        delay_ms(3000);
    }
}
