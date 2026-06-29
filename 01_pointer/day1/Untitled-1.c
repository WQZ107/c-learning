#include <stdio.h>
#include <stddef.h> // 1. 引入标准头文件，确保 size_t 可用

// 2. 将返回类型从 int 改为 size_t
size_t my_strlen(const char *s)
{
    const char *p = s;
    while(*p) p++;
    return p - s; // 指针差值会自动转换为 size_t
}

int main()
{
    const char *s = "Hello";
    printf("字符串： \"%s\"\n", s);
    
    // 3. 将 %d 改为 %zu
    printf("长度: %zu\n", my_strlen(s));
    
    return 0;
}