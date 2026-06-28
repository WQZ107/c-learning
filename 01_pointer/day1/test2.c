#include<stdio.h>

int main() {
    int score = 80;
    int *p = &score;

    *p = 999;

    printf("score = %d\n", score);
    return 0;
}