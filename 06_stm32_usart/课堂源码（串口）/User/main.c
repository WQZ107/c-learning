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


extern uint8_t g_usart1_rx_flag;
extern uint8_t g_rx_buffer[1];

int main(void)
{
    HAL_Init();
    sys_stm32_clock_init(RCC_PLL_MUL9);
    delay_init(72);
    led_init();
    usart_init(115200);
    
    printf("中断接收实验（变体B）：发字符试试\r\n");
    
    while(1)
    {
        if (g_usart1_rx_flag == 1)       //查标志：有字节到了
        {
            g_usart1_rx_flag = 0;
            HAL_UART_Transmit(&g_uart1_handle,(uint8_t*)g_rx_buffer,1,100);     //回显放主循环
        }
        LED_Toggle();
        delay_ms(200);      //LED照常闪
    }
}
