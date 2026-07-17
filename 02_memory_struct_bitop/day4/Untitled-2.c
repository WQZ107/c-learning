#include<stdio.h>
#include<string.h>

typedef struct {
    char name[20];
    int age;
    float score;
}Student;

int main(void)
{
    Student class[3] = {
        {"张三", 20, 85.5},
        {"李四", 21, 92.0},
        {"王五", 19, 78.5}
    };


    printf("用下标遍历\n");
    for(int i = 0;i < 3;i++){
        printf("%s: %.1f %d\n",class[i].name,class[i].score,class[i].age);
    }

    printf("\n用指针遍历\n");
    Student *p = class;
    for (int i = 0;i < 3;i++){
        printf("%s: %.1f %d\n",p->name,p->score,p->age);
        p++;
    }

    return 0;
}