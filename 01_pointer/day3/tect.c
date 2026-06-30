#include<stdio.h>
//写一个函数 `split_array`，接收一个数组和长度，
//通过指针参数同时返回数组的和与平均值
void split_array(int arr[], int n,int *sum,double *avg)
{
    *sum = 0;
    for(int i = 0; i < n; i++){
        *sum += arr[i];
    }
    *avg = (double)(*sum) / n;
}

int main(void)
{
    int arr[] = {1, 2, 3, 4, 5};
    int s;
    double a;

    split_array(arr,5,&s,&a);
    printf("和=%d,平均值=%.2f\n",s,a);

    return 0;
}