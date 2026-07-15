#include<stdio.h>
#include<stdlib.h>

int main(void)
{
    int *arr = malloc(4 * sizeof(int));
    arr[0] = 10; arr[1] = 20; arr[2] = 30; arr[3] = 40;

    printf("扩展前：\n");
    for(int i = 0;i < 4;i++) printf("arr[%d] = %d\n",i,arr[i]);

    arr = realloc(arr,7 * sizeof(int));
    arr[4] = 50; arr[5] = 60; arr[6] = 70;

    printf("扩展后：\n");
    for (int i = 0;i < 7;i++) printf("arr[%d] = %d\n",i,arr[i]);

    free(arr);
    return 0;
}