#include<stdio.h>

typedef struct {
    int year;
    int mouth;
    int day;
}Date;

typedef struct {
    char name[30];
    int id;
    Date birthday;
    char dept[30];
}Employee;

int main(void)
{
    Employee e = {
        .name = "张三",
        .id = 1001,
        .birthday = {2002,10,17},
        .dept = "研发部"
    };

    printf("姓名：%s\n出生日期：%d-%02d-%02d\n",e.name,e.birthday.year,e.birthday.mouth,e.birthday.day);
    printf("ID: %d\n",e.id);
    printf("DEFT: %s\n",e.dept);

    return 0;
}