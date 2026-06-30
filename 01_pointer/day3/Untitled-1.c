#include <stdio.h>

void swap(int *a, int *b)   // 注意：参数是指针，存的是地址
{
    int temp = *a;   // *a 取出 a 指向的值（就是原来的 x）
    *a = *b;         // 把 b 指向的值赋给 a 指向的位置
    *b = temp;
}

int main(void)
{
    int x = 10, y = 20;
    printf("调用前: x=%d, y=%d\n", x, y);
    
    swap(&x, &y);    // 传入 x 和 y 的地址！
    
    printf("调用后: x=%d, y=%d\n", x, y);  // 这次变了！
    return 0;
}