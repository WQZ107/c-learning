#include <stdio.h>

int add(int a, int b) { return a + b; }
int sub(int a, int b) { return a - b; }
int mul(int a, int b) { return a * b; }
int div_op(int a, int b) { return b ? a / b : 0; }

int main(void)
{
    // 函数指针数组
    int (*ops[])(int, int) = {add, sub, mul, div_op};
    const char *names[] = {"+", "-", "*", "/"};
    
    int a = 12, b = 4;
    
    for (int i = 0; i < 4; i++) {
        printf("%d %s %d = %d\n", a, names[i], b, ops[i](a, b));
    }
    
    return 0;
}