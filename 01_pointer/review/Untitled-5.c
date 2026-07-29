//数组名在大多数情况下就是首元素的地址
#include<stdio.h>
int main()
{
    int arr[] = {1,2,3,4,5};
    printf("arr = %p\n",arr);//数组名
    printf("&arr0 = %p\n",&arr[0]);//第一个元素的地址
    printf("&arr = %p\n",&arr);//整个数组的地址
    // 三个打印出来的值是一样的！
    // 但注意：它们的类型不一样！
    // arr 是 int* 类型（指向第一个int）
    // &arr 是 int(*)[5]类型（指向整个数组）

    return 0;
}