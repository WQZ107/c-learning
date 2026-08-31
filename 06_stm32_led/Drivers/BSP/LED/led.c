#include "./BSP/LED/led.h"


void led_init(void)
{
    GPIO_InitTypeDef gpio_init_struct;
    
    __HAL_RCC_GPIOB_CLK_ENABLE();       //先打开GPIOB的时钟（外设用前必开时钟）
    
    gpio_init_struct.Pin = GPIO_PIN_5;  //5号引脚
    gpio_init_struct.Mode = GPIO_MODE_OUTPUT_PP;    //推挽输出
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOB, &gpio_init_struct);

    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_SET); //写高电平=灭（低电平点亮）
}
//记住三步曲：**开时钟 → 配模式 → 用**。所有外设初始化都是这个套路

//LED打开：拉低点亮（低电平点亮）
void LED_On(void)
{
    HAL_GPIO_WritePin(GPIOB,GPIO_PIN_5,GPIO_PIN_RESET);
}

//LED关闭：拉高熄灭
void LED_off(void)
{
    HAL_GPIO_WritePin(GPIOB,GPIO_PIN_5,GPIO_PIN_SET);
}

//LED翻转
void LED_Toggle(void)
{
    HAL_GPIO_TogglePin(GPIOB,GPIO_PIN_5);
}

//读取LED状态：返回1=亮
int LED_Getstate(void)
{
    return(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_5) == GPIO_PIN_RESET) ? 1 : 0;
}
