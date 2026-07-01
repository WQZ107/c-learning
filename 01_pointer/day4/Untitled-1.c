#include<stdio.h>
#include<stdlib.h>

void alloc_right(int **pp, int size)
{
    *pp = (int*)malloc(size * sizeof(int));  // 通过 *pp 改外面的指针
}

int main(void)
{
    int *ptr = NULL;
    alloc_right(&ptr, 10);     // 传 ptr 的地址
    printf("%p\n", ptr);       // 不是 NULL 了！拿到了 malloc 的地址
    
    ptr[0] = 999;
    printf("%d\n", ptr[0]);    // 999
    
    free(ptr);
    return 0;
}
