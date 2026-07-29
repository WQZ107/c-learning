//指针的大小
#include<stdio.h>

int main()
{
    printf("int*大小:%zu字节\n",sizeof(int*));
    printf("char*大小:%zu字节\n",sizeof(char*));
    printf("double*大小:%zu字节\n",sizeof(double*));
    printf("void*大小:%zu字节\n",sizeof(void*));

    //64位系统下输出的都是8字节
    //32位系统下输出的都是4字节
    //看系统，和类型无关

    return 0;
}