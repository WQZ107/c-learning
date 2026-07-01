//写一个函数 `create_string_array(char ***ppp, int count)`
//动态创建一个字符串数组（每个字符串也是动态分配的）
//主函数里赋值、打印、释放

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

char **create_string_array(int count)
{
    char **arr = (char **)malloc(count * sizeof(char *));
    if (arr == NULL) return NULL;

    for (int i = 0;i < count;i++){
        arr[i] = (char *)malloc(64 * sizeof(char));
        if (arr[i] == NULL){

            for (int j = 0;j < i;j++) free(arr[j]);

            free(arr);
            return NULL;
        }
        arr[i][0] = '\0';
    }
    
    return arr;
}

void free_string_array(char **arr,int count)
{
    for (int i = 0;i < count;i++){
        free(arr[i]);
    }
    free(arr);
}

int main(void)
{
    int n = 3;
    char **names = create_string_array(n);

    strcpy(names[0],"wu");
    strcpy(names[1],"qi");
    strcpy(names[2],"zhen");

    for (int i = 0;i < n;i++){
        printf("names[%d] = %s\n",i,names[i]);
    }

    free_string_array(names,n);
    return 0;
}