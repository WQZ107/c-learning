#include<stdio.h>
#include<string.h>

typedef struct {
    char name[32];
    int age;
    float score;
}Student;

void update_score(Student *s,float new_score)
{
    s->score = new_score;
}

void print_student(const Student *s)
{
    printf("%s %d %.1f\n",s->name,s->age,s->score);
}

int main(void)
{
    Student s = {"张三",20,88.4};

    printf("修改前：");print_student(&s);
    update_score(&s,95.5);
    printf("修改后：");print_student(&s);

    return 0;
}