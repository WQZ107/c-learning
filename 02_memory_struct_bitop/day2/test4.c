#include<stdio.h>
#include<stdlib.h>

int main(void){
    int rows = 3,cols = 4;

    //分配：先分配行指针数组，再分配每行
    int **matrix = malloc(rows * sizeof(int*));
    for(int i = 0;i < rows;i++) {
        matrix[i] = malloc(cols * sizeof(int));
    }

    //幅值
    int val = 1;
    for(int i = 0;i < rows;i++){
        for(int j = 0;j < cols;j++){
            matrix[i][j] = val++;
        }
    }

    //打印
    for(int i = 0;i < rows;i++){
        for(int j = 0;j < cols;j++){
            printf("%3d ",matrix[i][j]);
        }
        printf("\n");
    }

    //释放：先释放每行，再释放行指针数组
    for(int i = 0;i < rows;i++){
        free(matrix[i]);
    }
    free(matrix);
    return 0;
}