//写两个函数 `add_one_value(int n)` 和 `add_one_ptr(int *n)`
//分别用值传递和指针传递，观察函数内外打印的区别，写注释说明为啥

#include<stdio.h>
void add_one_value(int n){
    n = n + 1;
    printf("add_one_value: n = %d\n", n);
}

void add_one_ptr(int *n){
    *n = *n + 1;
    printf("add_one_ptr: *n = %d\n",*n);
}

int main(void){
    int x = 100;
    printf("调用前： x = %d\n",x);

    add_one_value(x);
    printf("值传递调用后： x = %d\n",x);

    add_one_ptr(&x);
    printf("指针传递调用后： x = %d\n",x);
    return 0;
}