#include <stdio.h>
#include <stdlib.h>

void process(int count)
{
    int *arr = malloc(count * sizeof(int));
    if (arr == NULL) return;
    
    for (int i = 0; i < count; i++) {
        arr[i] = i;
    }
    
    if (count > 100) {
        free(arr);  // 提前返回前也要 free
        return;
    }
    
    // ... 正常处理 ...
    free(arr);
}

int main(void)
{
    for (int i = 0; i < 1000; i++) {
        process(200);
    }
    return 0;
}