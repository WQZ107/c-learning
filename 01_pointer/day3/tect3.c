//写一个函数 `print_safe(const int *arr, int n)`，只读不写
//如果在函数内尝试 `arr[0] = 0;` 看编译器报什么错

#include<stdio.h>

void print_safe(const int *arr,int n){
    for(int i = 0;i < n;i++){
        printf("%d",arr[i]);
    }
    printf("\n");

    //arr[0] = 999; //尝试修改只读数组，编译器会报错
}

int main(void){
    int data[5] = {1,2,3,4,5};
    print_safe(data,5);
    return 0;
}