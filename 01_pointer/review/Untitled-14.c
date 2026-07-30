//请为每个函数声明一个合适的函数指针变量，并让指针指向该函数。
#include <stdio.h>

// 函数定义
int square(int x) { 
    return x * x; 
}

void hello() { 
    printf("Hello!\n"); 
}

double circle_area(double r) { 
    return 3.14 * r * r; 
}

char get_grade(int score) {
    if (score >= 90) return 'A';
    if (score >= 80) return 'B';
    return 'C';
}

int main() {
    // 函数指针声明并赋值
    int (*fp1)(int) = square;
    void (*fp2)() = hello;
    double (*fp3)(double) = circle_area;
    char (*fp4)(int) = get_grade;

    // 通过函数指针调用函数
    int num = 5;
    printf("square(%d) = %d\n", num, fp1(num));

    fp2();  // 输出 Hello!

    double radius = 2.5;
    printf("circle_area(%.1f) = %.2f\n", radius, fp3(radius));

    int score = 85;
    printf("get_grade(%d) = %c\n", score, fp4(score));

    return 0;
}