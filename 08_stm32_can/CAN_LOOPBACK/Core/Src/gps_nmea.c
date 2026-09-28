#include "gps_nmea.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

GPS_Data g_gps = {0};

/* ★Day48-1:nmea_get_field(完整代码见昨天 Day48 步骤2,原样抄) */
/* 从 NMEA 语句里抽第 index 个字段(0=语句头) 到 out,返回字段长度 */
int nmea_get_field(const char *str, int index, char *out, int outlen)
{
    int i = 0, n = 0;

    /* 1) 跳过前 index 个逗号 */
    while (*str != '\0' && i < index) {
        if (*str == ',') i++;
        str++;
    }
    /* 2) 复制到 逗号 / '*' / 结尾 为止(坑:不能把 *校验和 当字段) */
    while (*str != ',' && *str != '*' && *str != '\0' && n < outlen - 1) {
        out[n++] = *str++;
    }
    out[n] = '\0';                       /* 3) 补字符串结束符 */
    return n;
}

/* ★Day48-2:nmea_to_deg(完整代码见 Day48 步骤3,原样抄) */
/* "2307.1234",2 -> 23.118723;  "11315.6789",3 -> 113.261315 */
double nmea_to_deg(const char *coord, int deg_len)
{
    char degbuf[8] = {0};
    int  deg;
    double minutes;

    strncpy(degbuf, coord, deg_len);     /* 度的部分:纬度2位/经度3位 */
    degbuf[deg_len] = '\0';
    deg     = atoi(degbuf);
    minutes = atof(coord + deg_len);     /* 分的部分:小数点后的字符串直接转浮点 */
    return deg + minutes / 60.0;
}

/* ★Day48-3:TestNmea(先抄 T1~T4 四段测试,原样抄) */
void TestNmea(void)
{
    char f[20];

    /* 测试用例必须来自真模块的语句格式(手册踩坑第4条) */
    char test[] = "$GNRMC,093000.000,A,2307.1234,N,11315.6789,E,0.5,30.1,150726,,,A*60";

    nmea_get_field(test, 3, f, sizeof(f));
    printf("[T1] field3=[%s]  expect [2307.1234]\r\n", f);

    nmea_get_field(test, 9, f, sizeof(f));
    printf("[T2] field9=[%s]  expect [150726]\r\n", f);

    printf("[T3] lat: %.6f  expect 23.118723\r\n", nmea_to_deg("2307.1234", 2));
    printf("[T4] lon: %.6f  expect 113.261315\r\n", nmea_to_deg("11315.6789", 3));
}
