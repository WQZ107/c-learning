#ifndef __SR04_H
#define __SR04_H

#include "main.h"

/* Pins: Trig=PE2(push-pull out), Echo=PG10(FT input, 5V echo) */
#define SR04_TRIG_PORT   GPIOE
#define SR04_TRIG_PIN    GPIO_PIN_2
#define SR04_TRIG_CLK()  do{ __HAL_RCC_GPIOE_CLK_ENABLE(); }while(0)
#define SR04_ECHO_PORT   GPIOG
#define SR04_ECHO_PIN    GPIO_PIN_10
#define SR04_ECHO_CLK()  do{ __HAL_RCC_GPIOG_CLK_ENABLE(); }while(0)

#define SR04_TRIG_OUT(x) do{ x ? \
                             HAL_GPIO_WritePin(SR04_TRIG_PORT, SR04_TRIG_PIN, GPIO_PIN_SET) : \
                             HAL_GPIO_WritePin(SR04_TRIG_PORT, SR04_TRIG_PIN, GPIO_PIN_RESET); \
                         }while(0)
#define SR04_ECHO_IN     (HAL_GPIO_ReadPin(SR04_ECHO_PORT, SR04_ECHO_PIN) != GPIO_PIN_RESET)

void sr04_init(void);
void sr04_set_temp10(int t10);   /* sound speed compensation, feed DHT11 temp_x10 */
int  sr04_read_mm(void);         /* mm; -1=no echo (wiring/target), -2=echo stuck high */

#endif
