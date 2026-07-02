//字符串排序
#include<stdio.h>
#include<string.h>

void sort_strings(const char *arr[],int n)
{
    for(int i = 0;i < n - 1;i++)
    {
        for(int j = 0;j < n - 1 - i;j++)
        {
            if(strcmp(arr[j],arr[j + 1]) > 0)
            {
                const char *temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

int main(void)
{
    const char *fruits[] = {"Banana", "Apple", "Cherry", "Date", "Elderberry"};
    int n = sizeof(fruits) / sizeof(fruits[0]);

    printf("排序前：\n");
    for(int i = 0;i < n;i++) printf("%s\n", fruits[i]);

    sort_strings(fruits,n);

    printf("排序后：\n");
    for(int i = 0;i < n;i++) printf("%s\n", fruits[i]);

    return 0;
}