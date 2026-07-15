#include<stdio.h>
#include<stdlib.h>

int main(void)
{
    int *p1 = malloc(5 * sizeof(int));
    int *p2 = calloc(5,sizeof(int));

    printf("malloc 分配（未初始化）:\n");
    for(int i = 0;i < 5;i++) printf("p1[%d] = %d\n",i,p1[i]);

    printf("calloc 分配（已初始化）:\n");
    for(int i = 0;i < 5;i++)printf("p2[%d] = %d\n",i,p2[i]);

    free(p1);
    free(p2);
    return 0;
}