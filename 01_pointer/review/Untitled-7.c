#include <stdio.h>

int main() {
    int arr[5] = {10, 20, 30, 40, 50};
    
    printf("=== 验证：数组名就是首元素地址 ===\n\n");
    
    printf("arr 的值（地址）:     %p\n", arr);
    printf("&arr[0] 的值（地址）: %p\n", &arr[0]);
    printf("两者相等吗？         %s\n\n", arr == &arr[0] ? "是 ✅" : "否 ❌");
    
    printf("=== 验证：数组名可以像指针一样用 ===\n\n");
    
    printf("*arr          = %d\n", *arr);          // 等价于 arr[0]
    printf("*(arr + 1)    = %d\n", *(arr + 1));    // 等价于 arr[1]
    printf("*(arr + 2)    = %d\n", *(arr + 2));    // 等价于 arr[2]
    
    printf("\n=== 验证：下标操作本质是指针运算 ===\n\n");
    
    printf("arr[0]  = %d\n", arr[0]);
    printf("0[arr]  = %d\n", 0[arr]);   // ← 这居然也能工作！
    // 因为 arr[0] 被编译器翻译成 *(arr + 0)
    // 而 0[arr] 被翻译成 *(0 + arr)，完全一样！
    
    return 0;
}