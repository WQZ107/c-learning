/**
 ******************************************************************************
 * @file     main.c
 * @author   正点原子团队(ALIENTEK)
 * @version  V1.0
 * @date     2020-08-20
 * @brief    新建工程实验-HAL库版本 实验
 * @license  Copyright (c) 2020-2032, 广州市星翼电子科技有限公司
 ******************************************************************************
 * @attention
 * 
 * 实验平台:正点原子 STM32F103 开发板
 * 在线视频:www.yuanzige.com
 * 技术论坛:www.openedv.com
 * 公司网址:www.alientek.com
 * 购买地址:openedv.taobao.com
 ******************************************************************************
 */

#include "./SYSTEM/sys/sys.h"
#include "./SYSTEM/usart/usart.h"
#include "./SYSTEM/delay/delay.h"
#include "./BSP/LED/led.h"

/* LED 闪烁 N 次，每次间隔 interval 毫秒 */
void led_blink_times(int times,int interval)
{
    for(int i = 0;i < times;i++)
    {
        HAL_GPIO_TogglePin(GPIOB,GPIO_PIN_5);  //Toggle为翻转
        delay_ms(interval);
    }
}

int main(void)
{
    HAL_Init();
    sys_stm32_clock_init(RCC_PLL_MUL9);
    delay_init(72);
    led_init();
    
    while(1)
    {
        led_blink_times(6,100);     //6次翻转=亮3下，结束在灭
        delay_ms(300);             //灭着停顿
        led_blink_times(4,1000);    //4次翻转=亮2下，结束在灭
        delay_ms(1000);             //全灭1秒  真 停在灭
    }
}
