/**
 * dht11.c - DHT11 single-bus temp/humidity driver (Block K, 2026-10)
 * Base: ALIENTEK example Drivers/BSP/DHT11/dht11.c (V1.0 2020-04-26)
 * Three fixes vs the example (all confirmed in its source):
 *   1) dht11_check() return value is now honored (example ignores it)
 *   2) on failure the output variables are untouched (defined output state)
 *   3) every retry counter is explicitly initialized (example had UB)
 * us-delay via DWT CYCCNT: SysTick/HAL time base is never touched
 * (example delay.c reconfigures SysTick -- not portable to HAL projects)
 * Datasheet: Aosong DHT11 v1.3_20170331 (timing table = its Table 10)
 */
#include "dht11.h"

/* ---------- DWT microsecond delay (does not touch SysTick) ---------- */
#define DWT_CTRL    (*(volatile uint32_t *)0xE0001000)
#define DWT_CYCCNT  (*(volatile uint32_t *)0xE0001004)
#define DEMCR       (*(volatile uint32_t *)0xE000EDFC)

static void dht11_dwt_init(void)
{
    DEMCR      |= (1UL << 24);            /* TRCENA: enable DWT counter */
    DWT_CYCCNT  = 0;
    DWT_CTRL   |= 1UL;                    /* CYCCNTENA */
}

static void dht11_delay_us(uint32_t us)   /* 72MHz: 1us = 72 cycles */
{
    uint32_t start = DWT_CYCCNT;
    uint32_t ticks = us * (SystemCoreClock / 1000000U);
    while ((DWT_CYCCNT - start) < ticks) { }  /* unsigned diff: overflow-safe */
}

/* ---------- init: PG11 open-drain + internal pull-up, wait 1s power-up ---------- */
void dht11_init(void)
{
    GPIO_InitTypeDef gpio_init_struct = {0};

    DHT11_DQ_GPIO_CLK_ENABLE();
    gpio_init_struct.Pin   = DHT11_DQ_GPIO_PIN;
    gpio_init_struct.Mode  = GPIO_MODE_OUTPUT_OD;   /* open-drain, see dht11.h */
    gpio_init_struct.Pull  = GPIO_PULLUP;           /* internal pull-up as backup */
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(DHT11_DQ_GPIO_PORT, &gpio_init_struct);

    dht11_dwt_init();
    DHT11_DQ_OUT(1);      /* release bus: ODR resets to 0 = OD output would hold low! */
    HAL_Delay(1000);      /* datasheet: 1s power-up unstable period, boot only */
}

/* Start signal: pull low >=18ms (Tbe), release 10~35us (Tgo) */
static void dht11_reset(void)
{
    DHT11_DQ_OUT(0);
    HAL_Delay(20);        /* 20ms inside 18~30ms window */
    DHT11_DQ_OUT(1);
    dht11_delay_us(13);   /* example value = datasheet typical 13us */
}

/* Response: DHT11 pulls low ~83us (Trel) then high ~87us (Treh) = "I'm here".
   return 0 = detected, 1 = timeout (fix #1: caller MUST honor this) */
static uint8_t dht11_check(void)
{
    uint8_t retry = 0;    /* fix #3: explicit init (example relied on garbage) */

    while (DHT11_DQ_IN && (retry < 100)) {  /* wait for sensor to pull low */
        retry++;
        dht11_delay_us(1);
    }
    if (retry >= 100) return 1;

    retry = 0;
    while ((!DHT11_DQ_IN) && (retry < 100)) { /* wait for it to release high */
        retry++;
        dht11_delay_us(1);
    }
    if (retry >= 100) return 3;   /* diag 3: pulled low but never released = stuck */

    return 0;
}

/* Read 1 bit: every bit starts with 50us low (TLOW), then high level
   23~27us = "0" (TH0) / 68~74us = "1" (TH1). Sample at 40us: 13us margin
   to both sides. Same approach as the example. */
static uint8_t dht11_read_bit(void)
{
    uint8_t retry = 0;

    while (DHT11_DQ_IN && (retry < 100)) { retry++; dht11_delay_us(1); }
    retry = 0;
    while ((!DHT11_DQ_IN) && (retry < 100)) { retry++; dht11_delay_us(1); }

    dht11_delay_us(40);
    return (DHT11_DQ_IN) ? 1 : 0;
}

static uint8_t dht11_read_byte(void)
{
    uint8_t i, data = 0;

    for (i = 0; i < 8; i++) {
        data <<= 1;                  /* MSB first (datasheet p4) */
        data |= dht11_read_bit();
    }
    return data;
}

/* Read temp/humi: 0=ok 1=no response 2=checksum error.On failure 
    *humi/ *temp stay untouched (fix #2, Block-J rule). */
uint8_t dht11_read(uint8_t *humi, uint8_t *temp)
{
    uint8_t buf[5] = {0};
    uint8_t i;
    uint8_t chk_rval;

    dht11_reset();
    chk_rval = dht11_check();
    if (chk_rval != 0)               /* fix #1: honor the response check */
        return chk_rval;             /* 1 = no answer, 3 = stuck low */

    for (i = 0; i < 5; i++)
        buf[i] = dht11_read_byte();

    if (buf[4] != (uint8_t)(buf[0] + buf[1] + buf[2] + buf[3]))
        return 2;                    /* fix #2: checksum fail touches nothing */

    *humi = buf[0];                  /* humidity integer %RH (decimal is always 0) */
    *temp = buf[2];                  /* temperature integer degC */
    return 0;
}
