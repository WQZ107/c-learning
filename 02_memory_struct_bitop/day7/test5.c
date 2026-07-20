//示例5：结构体指针作函数参数
//功能：定义 Student 结构体，将结构体指针传入函数，实现修改和打印结构体成员
#include <stdio.h>

/* 定义 Student 结构体 */
typedef struct {
    char    name[32];   /* 姓名 */
    int     id;         /* 学号 */
    float   score;      /* 成绩 */
} Student;

/* 接受结构体指针，修改成绩 */
void update_score(Student *p, float new_score)
{
    if (p != NULL) {
        p->score = new_score;
    }
}

/* 接受结构体指针，只读打印（const 保护） */
void print_student(const Student *p)
{
    if (p != NULL) {
        printf("姓名: %s  学号: %d  成绩: %.1f\n",
               p->name, p->id, p->score);
    }
}

int main(void)
{
    Student stu = {"王五", 1003, 76.5f};

    printf("修改前:\n");
    print_student(&stu);

    /* 通过指针修改成绩 */
    update_score(&stu, 88.0f);

    printf("修改后:\n");
    print_student(&stu);

    return 0;
}