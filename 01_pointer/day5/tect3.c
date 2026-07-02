//定义一个 `3×4` 的二维数组，用数组指针遍历并打印每个元素

#include<stdio.h>

int main(void)
{
    int arr[3][4] = {
        {1, 2, 3, 4},
        {5, 6, 7, 8},
        {9, 10, 11, 12}
    };

    int (*p)[4] = arr;

    for(int i = 0;i < 3;i++)
    {
        for(int j = 0;j < 4;j++)
        {
            printf("%3d ",p[i][j]);
        }
        printf("\n");
    }
    return 0;
}