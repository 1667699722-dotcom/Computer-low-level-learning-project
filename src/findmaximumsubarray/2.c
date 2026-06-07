#include <stdio.h>
struct Result
{
    int low;
    int high;
    int sum;
};
struct Result findmaxcrossingsubarray(int A[],int low,int mid,int high)
{
    int leftsum=-1000000;
    int rightsum=-1000000;
    int sum=0;
    int max_left;
    int max_right;
    for(int i=mid;i>=low;i--)
    {
        sum+=A[i];
        if(sum>leftsum)
        {
            leftsum=sum;
            max_left=i;
        }
    }
    sum=0;
    for(int i=mid+1;i<=high;i++)
    {
        sum+=A[i];
        if(sum>rightsum)
        {
            rightsum=sum;
            max_right=i;
        }
    }
    return (struct Result){max_left,max_right,leftsum+rightsum};
}
struct Result findmaximumsubarray(int A[],int low,int high)
{
    if(high==low)
    {
        return (struct Result){low,high,A[low]};
    }
    else
    {
        int mid=(low+high)/2;
        struct Result resleft=findmaximumsubarray(A,low,mid);
        struct Result resright=findmaximumsubarray(A,mid+1,high);
        struct Result rescross=findmaxcrossingsubarray(A,low,mid,high);
        if(resleft.sum>=resright.sum && resleft.sum>=rescross.sum)
        {
            return resleft;
        }
        else if (resright.sum>=resleft.sum && resright.sum>=rescross.sum)
        {
            return resright;
        }
        else
        {
            return rescross;
        }
    }
}
int main()
{
    int B[]={100,113,110,85,105,102,86,63,81,101,94,106,101,79,94,90,97};
    int n=sizeof(B)/sizeof(B[0]);
    int A[n-1];
    for(int i=0;i<n-1;i++)
    {
        A[i]=B[i+1]-B[i];
    }
    int len=sizeof(A)/sizeof(A[0]);
    struct Result data=findmaximumsubarray(A,0,len-1);
    printf("%d,%d,%d,%d \n",len,data.low,data.high,data.sum);
    return 0;
}
