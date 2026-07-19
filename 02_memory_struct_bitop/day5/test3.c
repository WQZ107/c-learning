#include<stdio.h>

typedef struct {
    unsigned int id : 11; //11位标准ID
    unsigned int rtr : 1; //远程帧标志
    unsigned int ide : 1; //扩展帧标志
    unsigned int rsv : 3; //保留位
}CAN_Header;

int main(void)
{
    CAN_Header header = {0};

    header.id = 0x123;  //ID = 291
    header.rtr = 0;     //数据帧
    header.ide = 0;     //标准帧

    printf("CAN ID=0x%03X RTR=%d IDE=%d\n",
            header.id,header.rtr,header.ide);
    
    return 0;
    
}