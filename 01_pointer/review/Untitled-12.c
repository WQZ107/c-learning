//函数指针的核心用途：回调函数
//一个通用的排序函数，比较逻辑由调用者决定
 #include<stdio.h>

 void sort(int *arr,int n,int (*compare)(int,int)) {
    for (int i = 0;i < n - 1;i++) {
        for (int j = 0;j < n - 1;j++) {
            if (compare(arr[j],arr[j + 1]) > 0) {
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
 }

 int ascending(int a,int b) {return a - b;}
 int descending(int a,int b) {return b - a;}

 int main()
 {
    int data[] = {5,2,8,1,9};

    sort(data,5,ascending);
    printf("%d\n",sort);

    sort(data,5,descending);

    return 0;
 }