#include<stdio.h>

typedef struct {
    unsigned int ready : 1;
    unsigned int error : 1;
    unsigned int mode : 2;
    unsigned int priority : 4;
}StatusReg;

void print_status(const StatusReg *s)
{
    printf("ready=%d error=%d mode=%d priority=%d\n",
            s->ready,s->error,s->mode,s->priority);
}

int main(void)
{
    StatusReg status = {0};

    status.ready = 1;
    status.mode = 2;
    status.priority = 5;

    print_status(&status);

    return 0;
}