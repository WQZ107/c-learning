#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char **create_string_array(int count)
{
    char **arr = (char**)malloc(count * sizeof(char*));
    if (arr == NULL) return NULL;
    
    for (int i = 0; i < count; i++) {
        arr[i] = (char*)malloc(64 * sizeof(char));
        if (arr[i] == NULL) {
            // 分配失败，释放已分配的
            for (int j = 0; j < i; j++) free(arr[j]);
            free(arr);
            return NULL;
        }
        arr[i][0] = '\0';
    }
    
    return arr;
}

void free_string_array(char **arr, int count)
{
    for (int i = 0; i < count; i++) {
        free(arr[i]);
    }
    free(arr);
}

int main(void)
{
    int n = 3;
    char **names = create_string_array(n);
    
    strcpy(names[0], "张三");
    strcpy(names[1], "李四");
    strcpy(names[2], "王五");
    
    for (int i = 0; i < n; i++) {
        printf("names[%d] = %s\n", i, names[i]);
    }
    
    free_string_array(names, n);
    return 0;
}