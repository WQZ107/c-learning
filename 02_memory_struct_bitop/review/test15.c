#include<stdio.h>

#define BATTERY_MAX_VOLTAGE 4200
#define BATTERY_MIN_VOLTAGE 2500
#define CELL_COUNT 96

int main(void)
{
    int total_max = BATTERY_MAX_VOLTAGE * CELL_COUNT;
    int total_min = BATTERY_MIN_VOLTAGE * CELL_COUNT;
    printf("总电压上限: %d mV\n",total_max);
    printf("总电压上限: %d mV\n",total_min);


    return 0;
}