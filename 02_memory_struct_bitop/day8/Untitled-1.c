// main.c —— 源文件：使用头文件
#include <stdio.h>
#include "math.h"   // 这一行实际上就是把 math.h 的内容"粘贴"到这里

int main(void) {
    printf("%d\n", add(1, 2));
    return 0;
}