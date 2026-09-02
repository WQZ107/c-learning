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
    int count = 0;
    float temperature = 25.5f;
    char name[] = "STM32F103ZET6";
    
    HAL_Init();                                 /* 初始化HAL库 */
    sys_stm32_clock_init(RCC_PLL_MUL9);         /* 设置时钟,72M */
    delay_init(72);                             /* 初始化延时函数 */
    led_init();                                 /* 初始化LED */
    usart_init(115200);                         /* 波特率设为115200 */
    
    //printf("请输入一个英文字符：\r\n\r\n");
    
    while(1)
    {
        printf("=== 第%d次上报 ===\r\n",    ++count);
        printf("板子：%s\r\n",name);
        printf("温度：%.1f 度\r\n",temperature);
        printf("LED引脚电平：%d\r\n",HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_5));
        printf("\r\n");
        delay_ms(2000);
    }
}
