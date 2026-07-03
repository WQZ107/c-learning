//用 `qsort` 对字符串数组排序（提示：比较函数里用 `strcmp`）

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int compare_strings(const void *a,const void *b)
{
    //a 和 b 都是char**（指向指针的指针）

    return strcmp(*(const char**)a, *(const char**)b);

}

int main(void)
{
    const char *names[] = {"Charlie", "Alice", "Bob", "Eve", "David"};
    int n = sizeof(names) / sizeof(names[0]);

    qsort(names,n,sizeof(char*),compare_strings);

    for(int i = 0;i < n;i++)
    {
        printf("%s\n",names[i]);
    }

    return 0;
}