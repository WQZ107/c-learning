//示例2：函数内 malloc（二级指针）
//功能：通过二级指针在函数内部修改调用方的一级指针，实现「函数内部分配内存，外部使用」
#include <stdio.h>
#include <stdlib.h>

/* 通过二级指针在函数内部 malloc，外部拿到分配好的内存 */
void create_array(int **pp, int size)
{
    *pp = (int *)malloc(size * sizeof(int));
    if (*pp == NULL) {
        printf("内存分配失败！\n");
        return;
    }
    for (int i = 0; i < size; i++) {
        (*pp)[i] = i + 100;
    }
}

int main(void)
{
    int *p = NULL;      /* 一级指针，初始为空 */
    int size = 5;

    /* 传入 &p（二级指针），函数内部 malloc 并赋值 */
    create_array(&p, size);

    if (p != NULL) {
        printf("函数内 malloc 创建的数组:\n");
        for (int i = 0; i < size; i++) {
            printf("p[%d] = %d\n", i, p[i]);
        }
        /* 使用完毕后释放 */
        free(p);
        p = NULL;
    }

    return 0;
}