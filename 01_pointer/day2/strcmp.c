#include <stdio.h>

int my_strcmp(const char *a, const char *b)
{
    while(*a == *b && *a != '\0'){
        a++;
        b++;
    }
    return *a - *b;
}

// 辅助函数：调用 my_strcmp 并打印易读的结果
void test(const char *a, const char *b) {
    int result = my_strcmp(a, b);
    printf("比较 \"%-8s\" 和 \"%-8s\" -> 返回值: %3d  ", a, b, result);
    
    if (result == 0)      printf("(a == b)\n");
    else if (result > 0)  printf("(a > b)\n");
    else                  printf("(a < b)\n");
}

int main() {
    printf("--- my_strcmp 字符串比较测试 ---\n\n");
    
    // 场景 1：完全相同
    test("hello", "hello");
    
    // 场景 2：a > b（第2个字符 'e' 的 ASCII码 大于 'a'）
    test("hello", "hallo"); 
    
    // 场景 3：a < b（第1个字符 'a' 的 ASCII码 小于 'b'）
    test("apple", "bpple");
    
    // 场景 4：前缀相同，但 a 更长（a 遇到 's'，b 遇到 '\0'，'s' > '\0'）
    test("apples", "apple");
    
    // 场景 5：前缀相同，但 a 更短（a 遇到 '\0'，b 遇到 'l'，'\0' < 'l'）
    test("app", "apple");

    return 0;
}