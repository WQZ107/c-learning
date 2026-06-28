#include<stdio.h>

int main() {
    int n = 100;
    int *p1 = &n;
    int *p2 = &n;

    *p2 = 200;
    
    printf("*p1= %d\n", *p1);
    return 0;
}