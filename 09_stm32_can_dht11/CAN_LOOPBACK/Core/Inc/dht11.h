#ifndef __DHT11_H
#define __DHT11_H

#include "main.h"

/* Pin: PG11 = ALIENTEK Elite board U4 1-wire socket (1WIRE_DQ) */
#define DHT11_DQ_GPIO_PORT          GPIOG
#define DHT11_DQ_GPIO_PIN           GPIO_PIN_11
#define DHT11_DQ_GPIO_CLK_ENABLE()  do{ __HAL_RCC_GPIOG_CLK_ENABLE(); }while(0)

/* Open-drain output + pull-up: writing 1 releases the bus (pulled high),
   and the pin can be read directly -- no direction switching at all */
#define DHT11_DQ_OUT(x)     do{ x ? \
                                HAL_GPIO_WritePin(DHT11_DQ_GPIO_PORT, DHT11_DQ_GPIO_PIN, GPIO_PIN_SET) : \
                                HAL_GPIO_WritePin(DHT11_DQ_GPIO_PORT, DHT11_DQ_GPIO_PIN, GPIO_PIN_RESET); \
                            }while(0)
#define DHT11_DQ_IN         HAL_GPIO_ReadPin(DHT11_DQ_GPIO_PORT, DHT11_DQ_GPIO_PIN)

void    dht11_init(void);
/* return 0 = ok, 1 = no response (line stayed high), 2 = checksum error,
   3 = line stuck low after start pulse. On failure *humi / *temp are left
   untouched (fail = defined output state) */
uint8_t dht11_read(uint8_t *humi, uint8_t *temp);

#endif
