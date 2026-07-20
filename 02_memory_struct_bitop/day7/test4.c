//示例4：定义 Student 结构体 + 打印
//功能：定义一个学生结构体，创建变量并初始化，打印学生信息
#include <stdio.h>

/* 定义 Student 结构体 */
typedef struct {
    char    name[32];   /* 姓名 */
    int     id;         /* 学号 */
    float   score;      /* 成绩 */
} Student;

int main(void)
{
    /* 创建结构体变量并初始化 */
    Student stu1 = {"张三", 1001, 89.5f};
    Student stu2 = {"李四", 1002, 92.0f};

    /* 打印学生信息 */
    printf("===== 学生信息 =====\n");
    printf("姓名: %s  学号: %d  成绩: %.1f\n", stu1.name, stu1.id, stu1.score);
    printf("姓名: %s  学号: %d  成绩: %.1f\n", stu2.name, stu2.id, stu2.score);

    return 0;
}