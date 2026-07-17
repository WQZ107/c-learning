#include<stdio.h>
#include<string.h>

typedef struct {
    char name[20];
    int age;
    float score;
}Student;

int main(void){
    Student s = {"张三",20,88.5};
    Student *p = &s;

    printf("通过指针访问:\n");
    printf("名字：%s\n",p->name);
    printf("年龄：%d\n",p->age);
    printf("成绩：%.1f\n",p->score);

    p->age = 21;
    printf("修改后的年龄：%d\n",s.age);//验证s也被修改了

    return 0;
}