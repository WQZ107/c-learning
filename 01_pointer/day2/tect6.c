#include<stdio.h>

char *my_strcpy(char *dst,const char *src)
{
    char *original_dst = dst;
    while((*dst++ = *src++) != '\0');
    return original_dst;
}

int main(void)
{
    char src[] = "Wolrd!";
    char dst[10];
    
    my_strcpy(dst,src);
    printf("dst = \"%s\"\n",dst);

    return 0;
}