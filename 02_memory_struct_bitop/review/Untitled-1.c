/*
 * 练习 1：内存四区观察 —— 参考答案（观察题）
 * 运行：gcc -g -o ex01 ex01.c && ./ex01
 */
#include <stdio.h>
#include <stdlib.h>

/* 全局变量 */
int g_global_init = 100;        /* 答案：数据区 .data（已初始化） */
int g_global_uninit;            /* 答案：数据区 .bss（未初始化，运行时清零） */

/* 字符串字面量 */
const char *g_ptr = "literal!"; /* 指针 g_ptr 在 数据区；"literal!" 在 代码区 .rodata（只读） */

/* 静态变量 */
static int s_file_static = 7;   /* 答案：数据区 .data */

/* 函数地址 */
int my_func(int x) { return x + 1; }

int main(void)
{
    /* 局部变量 */
    int local_a = 1;             /* 答案：栈区 */
    int local_b = 2;             /* 答案：栈区 */
    char stack_arr[32];          /* 答案：栈区 */

    /* 静态局部变量 */
    static int s_local = 5;      /* 答案：数据区（静态变量不在栈上！） */

    /* 动态分配 */
    int *heap_a = malloc(sizeof(int));   /* 指针在 栈区，指向的块在 堆区 */
    int *heap_b = malloc(sizeof(int));

    /* 常量（const 修饰） */
    const int c_local = 3;       /* 答案：栈区（const 局部变量仍是自动变量，只是不可写） */

    printf("===== 地址输出 =====\n");
    printf("[代码区] my_func       : %p\n", (void *)my_func);
    printf("[代码区] main          : %p\n", (void *)main);
    printf("[数据区] g_global_init : %p\n", (void *)&g_global_init);
    printf("[数据区] g_global_uninit: %p\n", (void *)&g_global_uninit);
    printf("[数据区] g_ptr         : %p\n", (void *)&g_ptr);
    printf("[只读区] \"literal!\"    : %p\n", (void *)g_ptr);
    printf("[数据区] s_file_static : %p\n", (void *)&s_file_static);
    printf("[数据区] s_local       : %p\n", (void *)&s_local);
    printf("[堆区]   heap_a        : %p\n", (void *)heap_a);
    printf("[堆区]   heap_b        : %p\n", (void *)heap_b);
    printf("[栈区]   local_a       : %p\n", (void *)&local_a);
    printf("[栈区]   local_b       : %p\n", (void *)&local_b);
    printf("[栈区]   stack_arr     : %p\n", (void *)stack_arr);
    printf("[栈区]   c_local       : %p\n", (void *)&c_local);

    printf("\n===== 观察题答案 =====\n");
    printf("1. 堆区增长方向：向上（heap_a < heap_b）\n");
    printf("2. 栈区增长方向：向下（local_a > local_b）\n");
    printf("3. s_local 与数据区全局变量地址挨在一起 → 它属于数据区\n");
    printf("4. c_local 在栈上（const 只限制可写性，不改变存储位置）\n");

    free(heap_a);
    free(heap_b);
    return 0;
}
