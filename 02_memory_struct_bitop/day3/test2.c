#include<stdio.h>
#include<stdlib.h>

int main(void)
{
    int *p = malloc(sizeof(int));
    *p = 77;

    printf("*p = %d\n",*p);

    free(p);
    p = NULL;
    return 0;
}