// 插入排序
#include <stdio.h>
void insertion_sort(int A[],int n){
    for (int j=1;j<n;j++)
    {
        int key=A[j];  // 选择当前元素作为关键值
        int i=j-1; // 从当前元素的前一个元素开始比较
        while (i>=0 && A[i]>key)
        {
            A[i+1]=A[i];///如果当前元素大于关键值，将当前元素向后移动
            i=i-1;// 继续比较前一个元素
        }
        A[i+1]=key;// 将关键值插入到正确位置
    }
}
int main(){
    int A[] ={5,2,4,6,1,3,7};
    int n = sizeof(A)/sizeof(A[0]); // 计算数组元素个数
    insertion_sort(A,n);
    for(int i=0;i<n;i++)
    {
        printf("%d ",A[i]);// 输出排序后的数组元素
    }
    printf("\n");
    return 0;
}