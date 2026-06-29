#include<stdio.h>

char *my_strcpy_v2(char *dst,const char *src)
{
    char *original_dst = dst;
    while((*dst++ = *src++) != '\0');
    return original_dst;
}

int main(){
    char str1[20] = "Hello";
    char str2[20];

    my_strcpy_v2(str2,str1);
    printf("复制后的字符串： \"%s\"\n", str2);
    return 0;
}