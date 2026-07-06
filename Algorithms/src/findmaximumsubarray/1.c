#include <stdio.h>

struct Result
{
    int low;
    int high;
    int sum;
};
 struct Result findmaximumsubarray(int A[],int n)
{
    struct Result res;
    int B[n*(n-1)/2];
    int C[n*(n-1)/2];
    int D[n*(n-1)/2];
    int k=0;
    for(int i=0;i<n;i++)
    {
        for(int j=i+1;j<n;j++)
        {
            C[k]=j;
            D[k]=i;
             B[k++]=A[j]-A[i];
        }
    }
    int max=B[0];
    int maxindex=0;
     for (int i = 1; i < n*(n-1)/2; i++) 
     {
        if (B[i] > max) 
        {
            max = B[i];
            maxindex=i;
        }
    }
    res.high=C[maxindex];
    res.low=D[maxindex];
    res.sum=max;
    return res;
}
int main()
{
    int A[]={100,113,110,85,105,102,86,63,81,101,94,106,101,79,94,90,97};
    int n=sizeof(A)/sizeof(A[0]);
    struct Result data=findmaximumsubarray(A,n);
    int low=data.low;
    int high=data.high;
    int sum=data.sum;
    printf("%d,%d,%d,%d \n",n,low,high,sum);
    return 0;
}
