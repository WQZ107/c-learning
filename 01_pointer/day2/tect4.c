#include<stdio.h>

int main(void)
{
    int arr[] = {100,200,300,400,500};
    int i = 2;

    printf("arr[%d]   =  %d\n",i,arr[i]);
    printf("*(arr + %d) = %d\n",i,*(arr + i));
    printf("两者%s\s",(arr[i] == *(arr+i))?"相等":"不等");

    printf("i[arr](=%d[arr]) = %d\n",i,i[arr]);
    return 0;
}