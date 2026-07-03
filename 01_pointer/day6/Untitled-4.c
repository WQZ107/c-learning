//回调函数

#include<stdio.h>

//回调函数类型：接收一个int。无返回值
typedef void(*Callback)(int);

//通用遍历函数：对数组每个元素执行回调函数
void foreach(int arr[],int n,Callback cd)
{
    for(int i = 0;i < n;i++)
    {
        cd(arr[i]);//调用回调函数
    }
}

//以下是不同的回调函数
void print_element(int x)
{
    printf("%d ",x);
}

void print_square(int x)
{
    printf("%d ",x * x);
}

void print_double(int x)
{
    printf("%d ",x * 2);
}

int main(void)
{
    int arr[] = {1,2,3,4,5};
    int n = 5;

    printf("原样打印：");
    foreach(arr,n,print_element);
    printf("\n");

    printf("打印平方：");
    foreach(arr,n,print_square);
    printf("\n");

    printf("打印两倍：");
    foreach(arr,n,print_double);
    printf("\n");

    return 0;
}