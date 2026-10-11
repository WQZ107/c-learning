/**
 * sr04.c - HC-SR04 ultrasonic rangefinder (Block M, 2026-10)
 * Trigger: 12us pulse on PE2. Echo: PG10 high time = round-trip time.
 * distance(mm) = echo_us * v(mm/us) / 2,  v = 331.4 + 0.6*T (m/s)
 * Temperature comes from DHT11 via sr04_set_temp10() (cross-module fusion).
 * DWT us-timer: same technique as dht11.c -- SysTick untouched.
 * Blocking budget: worst 30ms per read @2s period = 1.5% duty (OK, Block-K style).
 */
#include "sr04.h"

/* ---------- DWT microsecond timer (same as dht11.c) ---------- */
#define DWT_CTRL    (*(volatile uint32_t *)0xE0001000)
#define DWT_CYCCNT  (*(volatile uint32_t *)0xE0001004)
#define DEMCR       (*(volatile uint32_t *)0xE000EDFC)

static void sr04_dwt_init(void)
{
    DEMCR      |= (1UL << 24);
    DWT_CYCCNT  = 0;
    DWT_CTRL   |= 1UL;
}

static uint32_t sr04_now_us(void)
{
    return DWT_CYCCNT / (SystemCoreClock / 1000000U);
}

static int s_temp10 = 300;   /* default 30.0C until DHT11 feeds real value */

void sr04_set_temp10(int t10) { s_temp10 = t10; }

void sr04_init(void)
{
    GPIO_InitTypeDef g = {0};

    SR04_TRIG_CLK();
    SR04_ECHO_CLK();

    g.Pin = SR04_TRIG_PIN;                       /* Trig: push-pull output */
    g.Mode = GPIO_MODE_OUTPUT_PP;
    g.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(SR04_TRIG_PORT, &g);
    HAL_GPIO_WritePin(SR04_TRIG_PORT, SR04_TRIG_PIN, GPIO_PIN_RESET);
    /* ★ Block-K rule: explicit initial level (never trust ODR reset value) */

    g.Pin = SR04_ECHO_PIN;                       /* Echo: floating input (5V-tolerant pin) */
    g.Mode = GPIO_MODE_INPUT;
    g.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(SR04_ECHO_PORT, &g);

    sr04_dwt_init();
}

/* one measurement, blocking; returns mm or -1 on echo timeout */
int sr04_read_mm(void)
{
    uint32_t t0;

    /* trigger: 12us high pulse (DWT-measured, same style as dht11) */
    SR04_TRIG_OUT(1);
    t0 = sr04_now_us();
    while (sr04_now_us() - t0 < 12) { }
    SR04_TRIG_OUT(0);

    /* wait echo rising edge (sensor fires within a few hundred us) */
    t0 = sr04_now_us();
    while (!SR04_ECHO_IN) {
        if (sr04_now_us() - t0 > 30000) return -1;   /* -1: no echo at all (wiring/target) */
    }

    /* measure echo high width (max ~23.5ms @ 4m round trip) */
    t0 = sr04_now_us();
    while (SR04_ECHO_IN) {
        if (sr04_now_us() - t0 > 30000) return -2;   /* -2: echo stuck high (module fault) */
    }

    /* distance(mm) = echo_us * v(mm/us) / 2 ;  v(m/s) = 331.4 + 0.06*t10 */
    /* unit chain: v(m/s) -> /1000 -> mm/us -> /2 (round trip) -> multiply us */
    {
        uint32_t w = sr04_now_us() - t0;
        float v_mm_per_us = (331.4f + 0.06f * s_temp10) / 1000.0f;   /* 30C -> 0.3494 mm/us */
        return (int)((float)w * v_mm_per_us / 2.0f);
    }
}
