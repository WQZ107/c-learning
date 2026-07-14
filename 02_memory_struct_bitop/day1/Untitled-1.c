#include <stdio.h>
#include <stdlib.h>

int global_init = 100;      // Data段
int global_no_init;          // BSS段
static int static_var = 5;   // Data段

int main(void)
{
    int local_var = 10;                // 栈
    int *heap_var = malloc(sizeof(int)); // 堆
    *heap_var = 99;
    
    printf("代码区地址: %p\n", (void*)main);
    printf("Data段地址: %p\n", (void*)&global_init);
    printf("BSS段地址:  %p\n", (void*)&global_no_init);
    printf("栈区地址:   %p\n", (void*)&local_var);
    printf("堆区地址:   %p\n", (void*)heap_var);
    
    free(heap_var);
    return 0;
}