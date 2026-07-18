#include<stdio.h>

typedef struct {
    unsigned int id : 11;
    unsigned int rtr : 1;
    unsigned int ide : 1;
    unsigned int rsv : 3;
}CAN_Header;

int main(void)
{
    CAN_Header header = {0};

    header.id = 0x123;
    header.rtr = 0;
    header.ide = 0;

    printf("CAN ID=0x%03X RTR=%d IDE=%d\n",
            header.id,header.rtr,header.ide);
    
    return 0;
    
}