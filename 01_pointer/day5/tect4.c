//用指针数组实现字符串排序

#include<stdio.h>
#include<string.h>

void sort_strings(const char *arr[],int n)
{
    for(int i = 0;i < n - 1;i++){
        for(int j = 0;j < n - i - 1;j++){
            if(strcmp(arr[j],arr[j+1]) > 0){
                const char *temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

int main(void)
{
    const char *names[] = {"Charlie", "Alice", "Bob", "Eve", "David"};
    int n = (sizeof(names) / sizeof(names[0]));

    sort_strings(names,n);

    for(int i = 0;i < n;i++){
        printf("%s\n",names[i]);

    }

    return 0;
}