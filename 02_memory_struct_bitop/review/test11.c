// 练习 1：对象宏与函数宏基础（热身 + 陷阱）
#include<stdio.h>

/* ---- 对象宏：给常量起名字 ---- */
#define N        5
#define MAX_NUM  10
#define PI       3.14f

/* ---- 陷阱 1：替换列表里的运算符没加括号 ---- */
#define X      3
#define Y      X + 1    /* 注意：Y 的替换列表是 "X + 1"，不是 4！ */

/* ---- 陷阱 2：函数宏参数没加括号 ---- */
#define BAD_SQR(x)  x * x
#define GOOD_SQR(x) ((x) * (x))

/* ---- 正确姿势：参数和整个表达式都套括号 ---- */
#define ADD(a, b)   ((a) + (b))
#define MAX(a, b)   ((a) > (b) ? (a) : (b))

int main(void)
{
    /* 1. 对象宏 */
    int arr[MAX_NUM];              /* 数组有几个元素？ */
    arr[0] = N;                    /* N 是几？ */
    printf("1) N = %d, MAX_NUM = %d, arr[0] = %d\n", N, MAX_NUM, arr[0]);
    printf("   PI = %.2f\n", PI);

    /* 2. 展开陷阱：Y * 2 是多少？别猜，先展开！ */
    int r1 = Y * 2;                /* 展开成 X + 1 * 2，结果？ */
    printf("2) Y * 2 = %d  (展开过程: Y*2 -> X+1*2 -> 3+1*2)\n", r1);

    /* 3. 括号陷阱：BAD_SQR 和 GOOD_SQR 天差地别 */
    int r2 = BAD_SQR(2 + 3);       /* 猜：25 还是 11？ */
    int r3 = GOOD_SQR(2 + 3);      /* 应该是 25 */
    printf("3) BAD_SQR(2+3)  = %d\n", r2);
    printf("   GOOD_SQR(2+3) = %d\n", r3);

    /* 4. MAX 宏的常规使用 */
    int r4 = MAX(3, 5) + 1;        /* 是多少？ */
    printf("4) MAX(3,5)+1 = %d\n", r4);

    /* 5. 副作用陷阱：宏参数被多次求值 */
    int a = 3;
    int r5 = MAX(++a, 5);          /* 展开后 ++a 出现几次？a 最后是几？ */
    printf("5) MAX(++a,5) = %d, a = %d  (生产代码严禁这样写！)\n", r5, a);

    /* 6. 挑战：下面的表达式求值结果是什么？（提示：按优先级展开） */
    int r6 = MAX(2, 3) * 2 + 1;
    printf("6) MAX(2,3)*2+1 = %d\n", r6);

    return 0;
}