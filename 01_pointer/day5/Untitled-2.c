//数组指针

#include<stdio.h>

int main(void)
{
    int matrix[3][5] = {
        {1,2,3,4,5},
        {6,7,8,9,10},
        {11,12,13,14,15}
    };

    int (*p)[5] = matrix; //定义一个指向数组的指针

    for(int i = 0;i < 3;i++){
        for(int j = 0;j < 5;j++){
            printf("%3d ",p[i][j]);
        }
        printf("\n");
    }
    return 0;
}