//示例1：malloc 动态创建数组 + free
//功能：用户输入数组大小，动态分配内存，赋值并打印，最后释放
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    int n;

    printf("请输入数组大小: ");
    scanf("%d", &n);

    //动态分配 n 个 int 类型的空间
    int *arr = (int *)malloc(n * sizeof(int));
    if (arr == NULL) {
        printf("内存分配失败！\n");
        return 1;
    }

    //赋值并打印
    for (int i = 0; i < n; i++) {
        arr[i] = i * 10;
        printf("arr[%d] = %d\n", i, arr[i]);
    }

    //释放内存，避免内存泄漏
    free(arr);
    arr = NULL;     //释放后将指针置空，防止野指针

    return 0;
}