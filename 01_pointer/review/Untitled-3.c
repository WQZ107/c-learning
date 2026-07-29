//指针类型的意义
#include<stdio.h>

int main()
{
    int a = 0x11223344;//一个4字节的整数，十六进制表示方便观察
    int *p_int = &a;//int型指针
    char *p_char = (char*)&a;//char型指针，强制类型转换
    printf("*p_int = 0x%x\n",*p_int);
    printf("*p_char = 0x%x\n",*p_char);

    //为什么只读到0x44？因为x86是小端存储，低地址存低位数据
    //内存中实际是这样存的：
    //地址：0x100 0x101 0x102 0x103
    //数据： 0x44 0x33 0x22 0x11

    return 0;
}