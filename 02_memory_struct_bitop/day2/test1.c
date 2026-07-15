#include<stdio.h>
#include<stdlib.h>

int main(void)
{
    int n;
    printf("输入数组大小：");
    scanf("%d",&n);

    int *arr = malloc(n * sizeof(int));
    if(arr == NULL){
        printf("malloc 失败\n");
        return 1;
    }

    for(int i = 0;i < n;i++)
    {
        arr[i] = i * 10;
    }
    
    for(int i = 0;i < n;i++)
    {
        printf("arr[%d] = %d\n",i,arr[i]);
    }

    free(arr);
    arr = NULL;
    return 0;
}