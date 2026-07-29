//用指针遍历数组
#include<stdio.h>

int main()
{
    int arr[] = {1,2,3,4,5};
    int *p = arr;

    //方法1：用下标访问
    printf("方法1：用下标访问\n");

    for(int i = 0;i < 5;i++)
    {
        printf("%d",arr[i]);

    }
    printf("\n");

    //方法2：用指针+偏移
    printf("方法2：用指针+偏移\n");

    for(int i = 0;i < 5;i++)
    {
        printf("%d",*(p + i));
    }
    printf("\n");

    //方法3：移动指针本身
    printf("方法3：移动指针本身\n");

    for(p = arr;p < arr + 5;p++)
    {
        printf("%d",*p);
    }
    printf("\n");

    return 0;

}
