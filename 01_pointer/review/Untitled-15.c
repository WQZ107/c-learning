//题 3：通过指针调用（补全代码）
#include<stdio.h>
#include<ctype.h>

char to_upper(char c) {
    if (c >= 'a' && c <= 'z') return c - 32;
    return c;
}

int main(){
    char ch = 'x';

    char (*fp)(char) = to_upper;
    printf("%c\n",fp(ch));

    return 0;
}