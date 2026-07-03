#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 比较函数：升序
int compare_asc(const void *a, const void *b)
{
    return *(int*)a - *(int*)b;
}

// 比较函数：降序
int compare_desc(const void *a, const void *b)
{
    return *(int*)b - *(int*)a;
}

int main(void)
{
    int arr[] = {5, 2, 8, 1, 9, 3};
    int n = 6;
    
    // 升序排序
    qsort(arr, n, sizeof(int), compare_asc);
    
    printf("升序: ");
    for (int i = 0; i < n; i++) printf("%d ", arr[i]);
    printf("\n");
    
    // 降序排序
    qsort(arr, n, sizeof(int), compare_desc);
    
    printf("降序: ");
    for (int i = 0; i < n; i++) printf("%d ", arr[i]);
    printf("\n");
    
    return 0;
}