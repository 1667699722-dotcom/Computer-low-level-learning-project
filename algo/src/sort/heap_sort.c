#include <stdio.h>

int Parent(int i)
{
    return i/2;
}

int Leftchild(int i)
{
    return 2*i;
}

int Rightchild(int i)
{
    return 2*i+1;
}

void Maxheapify(int A[],int heap_size,int i)
{
    int l=Leftchild(i);
    int r=Rightchild(i);
    int largest=i;
    if(l<=heap_size && A[l]>A[largest])
    {
        largest=l;
    }
    if(r<=heap_size && A[r]>A[largest])
    {
        largest=r;
    }
    if(i!=largest)
    {
        int temp=A[i];
        A[i]=A[largest];
        A[largest]=temp;
        Maxheapify(A,heap_size,largest);
    }
}

void BuildmaxHeap(int A[],int n)
{
    int heap_size=n;
    for(int i=Parent(n);i>=1;i--)
    {
        Maxheapify(A,heap_size,i);
    }
}
void Heapsort(int A[],int n)
{
    int heap_size=n;
    BuildmaxHeap(A,n);
    for(int i=n;i>=2;i--)
    {
        int temp=A[1];
        A[1]=A[i];
        A[i]=temp;
        heap_size--;
        Maxheapify(A,heap_size,1);
    }

}
int main()
{
    int A[] = {0,5,2,4,6,1,3,7,8}; 
    int n = sizeof(A)/sizeof(A[0]) - 1; 
    Heapsort(A,n);
    for(int i=1;i<=n;i++)
    {
        printf("%d ",A[i]);
    }
    printf("\n");
    return 0;
}