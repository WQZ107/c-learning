//用函数指针数组实现一个简易计算器
//根据用户输入的运算符调用对应函数

#include<stdio.h>

int add(int a,int b) {return a + b;}
int sub(int a,int b) {return a - b;}
int mul(int a,int b) {return a * b;}
int div_op(int a,int b) {return b ? a / b : 0;} // Avoid division by zero

int main(void)
{
    int (*ops[])(int,int) = {add, sub, mul, div_op}; // Array of function pointers
    char symbols[] = {'+', '-', '*', '/'};

    int a,b;
    char op;

    printf("Enter an expression (e.g., 3 + 4): ");
    scanf("%d %c %d", &a, &op, &b);

    for (int i = 0;i < 4;i++)
    {
        if (op == symbols[i])
        {
            printf("%d %c %d = %d\n",a,op,b,ops[i](a,b));
            return 0;
        }
    }

    printf("Unsupported operator: %c\n",op);
    return 0;
    
}