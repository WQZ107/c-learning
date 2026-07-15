#include<stdio.h>
#include<stdlib.h>

int main(void)
{
    int *arr = malloc(5 * sizeof(int));

    if (arr == NULL) {
        printf("malloc 失败! \n");
        return 1;
    }

    for(int i = 0;i < 5;i++){
        arr[i] = (i + 1) * 10;
        printf("arr[%d] = %d\n",i,arr[i]);
    }

    free(arr);
    arr = NULL;

    return 0;
}