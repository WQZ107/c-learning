//写一个函数 `create_array(int **pp, int size) 
//函数内 malloc 一个 int 数组，通过二级指针把地址带出来
//主函数里使用这个数组（赋值、打印、free）

#include<stdio.h>
#include<stdlib.h>

int create_array(int **pp, int size)
{
    if (pp == NULL || size <= 0) return -1;
    
    *pp = (int*)malloc(size * sizeof(int));
    if (*pp == NULL) return -1;
    
    return 0;  // 成功
}

int main(void)
{
    int *arr = NULL;
    int n = 5;

    create_array(&arr, n);
    if (arr){

        for(int i = 0;i < n;i++){
            arr[i] = (i + 1) * 10;

        }

        for(int i = 0;i < n;i++){
            printf("arr[%d] = %d\n",i,arr[i]);
        }

        free(arr);
        arr = NULL;
    }

    return 0;
}