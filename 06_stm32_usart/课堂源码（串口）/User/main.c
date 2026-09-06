#include "./SYSTEM/sys/sys.h"
#include "./SYSTEM/delay/delay.h"
#include "./SYSTEM/usart/usart.h"
#include "./BSP/LED/led.h"


// 全局变量声明（在 usart.h 里用 extern 声明过）
extern uint8_t cmd_buf[64];
extern volatile uint8_t cmd_ready;

void key_exti_init(void);

int main(void)
{
    HAL_Init();
    sys_stm32_clock_init(RCC_PLL_MUL9);
    delay_init(72);
    led_init();
    usart_init(115200);
    
    key_exti_init();
    
    printf("命令接受测试，发 hello+回车 试试\r\n");
    
    while(1)
    {
        if (cmd_ready)              //一条完整命令到了
        {
            cmd_ready = 0;
            printf("受到命令: [%s]\r\n",cmd_buf);       //块五这里换成 ProcessCommand(cmd_buf)
        }
        
        LED_Toggle();
        delay_ms(500);
    }
}


    //按键外部中断初始化：KEY_UP=PA0，下降沿触发
    static void key_exti_init(void)
    {
        GPIO_InitTypeDef gpio_init_struct;
        
        __HAL_RCC_GPIOA_CLK_ENABLE();               //开启GPIOE时钟
        
        gpio_init_struct.Pin  = GPIO_PIN_0;         //KEY_UP = PA0
        
//        gpio_init_struct.Mode = GPIO_MODE_IT_FALLING;   //IT = 中断，FALLING = 下降沿（按下瞬间高→低）
        gpio_init_struct.Mode = GPIO_MODE_IT_RISING;        //上升沿
        
//        gpio_init_struct.Pull = GPIO_PULLUP;            //上拉：没按时是高电平
        gpio_init_struct.Pull = GPIO_PULLDOWN;            //下拉：没按时是低电平
        
        HAL_GPIO_Init(GPIOA,&gpio_init_struct);
        
        HAL_NVIC_SetPriority(EXTI0_IRQn,2,0);             //设置中断优先级，使能 GPIOA 的时钟 → 打开A端口的电源开关
        HAL_NVIC_EnableIRQ(EXTI0_IRQn);                 //使能 EXTI0 中断 → 允许第0号中断线触发中断
    }
        //中断服务函数：硬件跳转的第一站（名字固定，不能改）
        void EXTI0_IRQHandler(void)
        {
            HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_0);       //HAL帮你清标志+调回调
        }
        
        //回调函数：中断真正干活的地点（所有EXTI共用这一个回调，靠参数区分引脚）
        void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
        {
            if (GPIO_Pin == GPIO_PIN_0)     //确定是KEY0（PE4）
            {
                delay_ms(20);               //延时20跳过抖动期
                if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0) == GPIO_PIN_SET)        //GPIO_PIN_RESET还在低=真按下, GPIO_PIN_SET高电平
                {
                LED_Toggle();
            }
        }
    }
