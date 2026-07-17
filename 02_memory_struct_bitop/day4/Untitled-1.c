#include<stdio.h>
#include<string.h>

typedef struct {
    char name[32];
    int age;
    float score;
}Student;

int main(void)
{
    Student s1;
    strcpy(s1.name,"张三");
    s1.age = 20;
    s1.score = 66.4;

    Student s2 = {"李四",21,44.5};


    Student s3 = {
        .name = "王五",
        .age = 22,
        .score = 77.2
    };

    printf("%s %d %.1f\n",s1.name, s1.age, s1.score);
    printf("%s %d %.1f\n",s2.name, s2.age, s2.score);
    printf("%s %d %.1f\n",s3.name, s3.age, s3.score);

    return 0;
}