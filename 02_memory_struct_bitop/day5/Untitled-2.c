#include <stdio.h>

typedef struct {
    unsigned int enable  : 1;
    unsigned int mode    : 3;
    unsigned int channel : 4;
    unsigned int status  : 8;
} Register;

// 用联合体把位域和整型重叠在一起，直接查看原始值
typedef union {
    Register reg;
    unsigned int raw;
} RegView;

int main(void)
{
    RegView rv = {0};

    rv.reg.enable  = 1;
    rv.reg.mode    = 5;
    rv.reg.channel = 12;

    printf("enable=%d mode=%d channel=%d status=%d\n",
            rv.reg.enable, rv.reg.mode, rv.reg.channel, rv.reg.status);

    printf("原始 32 位值: 0x%08X\n", rv.raw);
    // 输出: 0x000000CB

    // 逐位打印（手动展开二进制）
    printf("二进制: ");
    for (int i = 31; i >= 0; i--) {
        printf("%d", (rv.raw >> i) & 1);
        if (i % 8 == 0) printf(" ");
    }
    printf("\n");

    return 0;
}