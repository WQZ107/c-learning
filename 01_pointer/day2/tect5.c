#include<stdio.h>

int my_strlen(const char *str)
{
    int len = 0;
    while(*str != '\0'){
        len++;
        str++;
    }
    return len;
}

int main(void)
{
    char str[] = "Hello World!";
    printf("strlen(\"%s\") = %d\n",str,my_strlen(str));
    return 0;
}