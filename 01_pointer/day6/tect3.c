//实现一个'foreach'函数
//接收数组和回调数组，对每个元素执行回调

#include<stdio.h>

typedef void (*Callback)(int);

void foreach(int arr[],int n,Callback cd)
{
    for(int i = 0;i < n;i++)
    {
        cd(arr[i]);
    }
}

//全局变量用于累加（简单起见）
static int g_sum = 0;

void printf_val(int x) {printf("%d ",x);}
void printf_sq(int x) {printf("%d ",x * x);}
void accumulate(int x) {g_sum += x;}

int main(void)
{
    int arr[] = {1,2,3,4,5};
    int n = 5;

    printf("打印: "); foreach(arr,n,printf_val); printf("\n");
    printf("平方: "); foreach(arr,n,printf_sq);printf("\n");

    g_sum = 0;
    foreach(arr, n, accumulate);
    printf("求和: %d\n",g_sum);

    return 0;
}