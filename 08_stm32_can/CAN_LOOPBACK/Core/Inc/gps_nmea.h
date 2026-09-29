#ifndef __GPS_NMEA_H
#define __GPS_NMEA_H
#include "main.h"

typedef struct {
    double  latitude;    /* 十进制度,南纬为负 */
    double  longitude;   /* 十进制度,西经为负 */
    double  speed_kmh;   /* 节×1.852 换算 km/h */
    uint8_t satellites;  /* GGA 卫星数 */
    uint8_t valid;       /* 1=定位有效(RMC 状态A) */
    uint8_t hour, min, sec;
} GPS_Data;

extern GPS_Data g_gps;

void GPS_ParseLine(const char *line);
void TestNmea(void);
void GPS_UART_Init(UART_HandleTypeDef *huart);  /* Day50 用 */
uint8_t GPS_PopLine(char *out, int outlen);

#endif
