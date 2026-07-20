//示例3：动态创建二维数组
//功能：动态分配一个 rows x cols 的二维数组，赋值并打印，最后正确释放所有内存
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    int rows = 3, cols = 4;

    /* 第一步：分配行指针数组（rows 个 int*） */
    int **matrix = (int **)malloc(rows * sizeof(int *));
    if (matrix == NULL) {
        printf("行指针分配失败！\n");
        return 1;
    }

    /* 第二步：为每一行分配 cols 个 int */
    for (int i = 0; i < rows; i++) {
        matrix[i] = (int *)malloc(cols * sizeof(int));
        if (matrix[i] == NULL) {
            printf("第 %d 行分配失败！\n", i);
            /* 释放已分配的行，防止泄漏 */
            for (int k = 0; k < i; k++) free(matrix[k]);
            free(matrix);
            return 1;
        }
    }

    /* 赋值并打印 */
    int count = 0;
    printf("动态创建的 %d x %d 二维数组:\n", rows, cols);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            matrix[i][j] = count++;
            printf("%3d ", matrix[i][j]);
        }
        printf("\n");
    }

    /* 释放：先释放每一行，再释放行指针数组 */
    for (int i = 0; i < rows; i++) {
        free(matrix[i]);
    }
    free(matrix);
    matrix = NULL;

    return 0;
}