#include <stdio.h>

void Merge(int low,int mid,int high,int A[],int B[])
{
    int i,j,k;
    for(k=low;k<=high;k++)
    {
        B[k]=A[k];
    }
    i=low;
    j=mid+1;
    k=low;
    while(i<=mid && j<=high)
    {
        if(B[i]<B[j])
        {
            A[k++]=B[i++];
        }
        else
        {
            A[k++]=B[j++];
        }
    }
    while(i<=mid)
    {
        A[k++]=B[i++];
    }
    while(j<=high)
    {
        A[k++]=B[j++];
    }
}

void Mergesort(int low,int high,int A[],int B[])
{
    if(low<high)
        {
            int mid=(low+high)/2;
            Mergesort(low,mid,A,B);
            Mergesort(mid+1,high,A,B);
            Merge(low,mid,high,A,B);
        }
}

int main()
{
    int A[] = {0,5,2,4,6,1,3,7,8}; 
    int n = sizeof(A)/sizeof(A[0]) - 1; 
    int B[n+1];
    Mergesort(1,n,A,B);
    for(int i=1;i<=n;i++)
    {
        printf("%d ",A[i]);
    }
    printf("\n");
    return 0;
}