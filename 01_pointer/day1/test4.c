#include <stdio.h>

int main(void)
{
    printf("char*   = %zu\n", sizeof(char*));
    printf("int*    = %zu\n", sizeof(int*));
    printf("double* = %zu\n", sizeof(double*));
    printf("int**   = %zu\n", sizeof(int**));
    return 0;
}