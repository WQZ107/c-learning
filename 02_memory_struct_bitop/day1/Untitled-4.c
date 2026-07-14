#include<stdio.h>
#include<stdlib.h>

int main(void)
{
    int *arr = malloc(10000000 * sizeof(int));
    if(arr == NULL) {
        printf("malloc 失败\n");
        return 1;
    }

    for (int i = 0; i < 10000000;i++){
        arr[i] = i;
    }

    printf("arr[9999999] = %d\n",arr[9999999]);

    free(arr);
    return 0;
}