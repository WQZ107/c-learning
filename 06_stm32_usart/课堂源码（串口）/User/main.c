#include "./SYSTEM/sys/sys.h"
#include "./SYSTEM/delay/delay.h"
#include "./SYSTEM/usart/usart.h"
#include "./BSP/LED/led.h"
#include <string.h>


//LED控制（块一，低电平点亮）
//void LED_On(void)       { HAL_GPIO_WritePin(GPIOB,GPIO_PIN_5,GPIO_PIN_RESET);}
//void LED_Off(void)      { HAL_GPIO_WritePin(GPIOB,GPIO_PIN_5,GPIO_PIN_SET);}
//void LED_Toggle(void)   { HAL_GPIO_TogglePin(GPIOB,GPIO_PIN_5);}
//int LED_GetState(void)  { return (HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_5) == GPIO_PIN_RESET) ? 1 : 0;} //1=亮

//KEY0 外部中断 （块四，PE4）
static void key_exti_init(void)
{
    GPIO_InitTypeDef gpio_init_struct;
    __HAL_RCC_GPIOE_CLK_ENABLE();
    
    gpio_init_struct.Pin  = GPIO_PIN_4;
    gpio_init_struct.Mode = GPIO_MODE_IT_FALLING;
    gpio_init_struct.Pull = GPIO_PULLUP;
    
    HAL_GPIO_Init(GPIOE,&gpio_init_struct);
    HAL_NVIC_SetPriority(EXTI4_IRQn,2,0);
    HAL_NVIC_EnableIRQ(EXTI4_IRQn);
}

void EXTI4_IRQHandler(void)
{
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_4);
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if (GPIO_Pin == GPIO_PIN_4)
    {
        delay_ms(20);
        if (HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_4) == GPIO_PIN_RESET)
        {
            LED_Toggle();
        }
    }
}

//命令解析（块三拼好的命令在这里执行）
void ProcessCommand(uint8_t *cmd)
{
    if (strcmp((char *)cmd,"LED_ON") == 0)         //strcmp相同返回0
    {
        LED_On();
        printf("LED已打开\r\n");
    }
    else if (strcmp((char *)cmd,"LED_OFF") == 0)
    {
        LED_Off();
        printf("LED已关闭\r\n");
    }
    else if (strcmp((char *)cmd,"STATUS") == 0)
    {
        printf("LED状态：%s,运行：%u秒\r\n",
        LED_GetState() ? "开" : "关",             //GetState已修正：1=亮=开 
        (uint32_t)HAL_GetTick() / 1000);                   //HAL_GetTick=开机毫秒数
    }
    else
    {
        printf("未知命令：[%s]\r\n",cmd);
    }
}

extern uint8_t cmd_buf[64];                //定义在usart.c（上面注释里的代码）
extern volatile uint8_t cmd_ready;      //完整命令就绪标志

void poll_command(void)
{
    if (cmd_ready)
    {
        cmd_ready = 0;
        ProcessCommand(cmd_buf);
    }
}

int main(void)
{
    HAL_Init();
    sys_stm32_clock_init(RCC_PLL_MUL9);
    delay_init(72);
    led_init();
    usart_init(115200);
    key_exti_init();                        //驱动内部已开启中断接收+自动续
    LED_Off();                              //KEY0外部中断
    
    printf("=== 串口命令控制系统启动 ===\r\n");
    printf("支持命令: LED_ON / LED_OFF / STATUS\r\n");
    
    uint32_t last_heartbeat = 0;        //上次心跳时刻
    
    while(1)
    {
        poll_command();
        
        if (HAL_GetTick() - last_heartbeat >= 10000)
        {
            printf("ALIVE,运行:%u秒\r\n",(uint32_t)HAL_GetTick() / 1000u);
            last_heartbeat = HAL_GetTick();
        }
    }
}
