//数组传参的本质
#include<stdio.h>

void print_size(int arr[])
{
    printf("函数内sizeof(arr) = %zu\n",sizeof(arr));

}

int main()
{
    int arr[10] = {0};
    printf("函数外:sizeof(arr) = %zu\n",sizeof(arr));
    print_size(arr);

    return 0;
}