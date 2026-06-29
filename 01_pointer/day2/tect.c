#include<stdio.h>

int main()
{
    int arr[] = {10,20,30,40,50};
    int *p = arr;
    int *end = arr + 5;
    printf("遍历数组\n");
    while(p < end){
        printf("%d\n",*p);
        p++;
    }
    return 0;
}