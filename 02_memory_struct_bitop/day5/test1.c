#include<stdio.h>

typedef struct {
    char type[16];
    double value;
    char timetamp[20];
    int status;
}SensorData;

int main(void)
{
    SensorData s = {"GPS",23.123,"2026-6-10 15:40:00",1};
    printf("%s: %.3f [%s] %s\n",
            s.type,s.value,s.timetamp,s.status ? "警告" : "正常");

    return 0;
}