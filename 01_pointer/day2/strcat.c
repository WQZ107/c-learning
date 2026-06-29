#include<stdio.h>

char *my_strcat(char *dst,const char *src)
{
    char *original_dst = dst;

    while(*dst != '\0'){
        dst++;
    }

    while(*src != '\0'){
        *dst++ = *src++;
    }
    *dst = '\0';
    return original_dst;
}

int main()
{
    char str1[100] = "Hello ";
    char str2[] = "World!";
    my_strcat(str1,str2);
    printf("%s\n",str1);
    return 0;
}