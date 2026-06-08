#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef int** Matrix;

typedef struct {
    Matrix m11;
    Matrix m12;
    Matrix m21;
    Matrix m22;
} SplitMat;

Matrix createMatrix(int size)
{
    Matrix mat=(Matrix)malloc(size*sizeof(int*));
    for(int i=0;i<size;i++)
    {
        mat[i]=(int*)malloc(size*sizeof(int));
    }
    return mat;
}

void freeMatrix(Matrix mat, int size) {
    for (int i = 0; i < size; i++) {
        free(mat[i]);
    }
    free(mat);
}
void printMatrix(Matrix mat,int size)
{
    for(int i=0;i<size;i++)
    {
        for(int j=0;j<size;j++)
        {
            printf("%d  ",mat[i][j]);
        }
        printf("\n");
    }
}

int nextpoweroftwo(int n)
{
    return (int)pow(2,ceil(log2(n)));
}

Matrix padMatrix(Matrix origin,int originsize)
{
    int padsize=nextpoweroftwo(originsize);
    Matrix pad=createMatrix(padsize);
    for(int i=0;i<padsize;i++)
    {
        for(int j=0;j<padsize;j++)
        {
            if(i<originsize && j<originsize)
            {
                pad[i][j]=origin[i][j];
            }
            else 
            {
                pad[i][j]=0;
            }
        }
    }
    return pad;
}
Matrix combieMatrix(Matrix C1,Matrix C2,Matrix C3,Matrix C4,int halfsize)
{
    Matrix C=createMatrix(halfsize*2);
    for(int i=0;i<halfsize*2;i++)
    {
        for(int j=0;j<halfsize*2;j++)
        {
            if(i<halfsize && j<halfsize)
            {
                C[i][j]=C1[i][j];
            }
            else if(i<halfsize && j>=halfsize)
            {
                C[i][j]=C2[i][j-halfsize];
            }
            else if(i>=halfsize && j<halfsize)
            {
                C[i][j]=C3[i-halfsize][j];
            }
            else 
            {
                C[i][j]=C4[i-halfsize][j-halfsize];
            }
        }
    }
    return C;
}

SplitMat SplitMatrix(Matrix mat,int size)
{
    int halfsize=size/2;
    Matrix m11,m12,m21,m22;
    SplitMat splitmat;
    m11=createMatrix(halfsize);
    m12=createMatrix(halfsize);
    m21=createMatrix(halfsize);
    m22=createMatrix(halfsize);
    for(int i=0;i<halfsize;i++)
    {
        for(int j=0;j<halfsize;j++)
        {
            m11[i][j]=mat[i][j];
            m12[i][j]=mat[i][j+halfsize];
            m21[i][j]=mat[i+halfsize][j];
            m22[i][j]=mat[i+halfsize][j+halfsize];
        }
    }
    splitmat.m11=m11;
    splitmat.m12=m12;
    splitmat.m21=m21;
    splitmat.m22=m22;
    return splitmat;
}

Matrix AddMatrix(Matrix A,Matrix B,int size)
{
    Matrix C=createMatrix(size);
    for(int i=0;i<size;i++)
    {
        for(int j=0;j<size;j++)
        {
            C[i][j]=A[i][j]+B[i][j];
        }
    }
    return C;
}

Matrix SquareMatrixMultiply(Matrix A,Matrix B,int size)
{
    Matrix C=createMatrix(size);
    if(size==1)
    { 
        C[0][0]=A[0][0]*B[0][0];
    }
    else 
    {
    int halfsize=size/2;
    Matrix C1=createMatrix(halfsize);
    Matrix C2=createMatrix(halfsize);
    Matrix C3=createMatrix(halfsize);
    Matrix C4=createMatrix(halfsize);

    SplitMat splitA=SplitMatrix(A,size);
    SplitMat splitB=SplitMatrix(B,size);

    Matrix D1=createMatrix(halfsize);
    Matrix D2=createMatrix(halfsize);
    Matrix D3=createMatrix(halfsize);
    Matrix D4=createMatrix(halfsize);
    Matrix D5=createMatrix(halfsize);
    Matrix D6=createMatrix(halfsize);
    Matrix D7=createMatrix(halfsize);
    Matrix D8=createMatrix(halfsize);

    D1=SquareMatrixMultiply(splitA.m11,splitB.m11,halfsize);
    D2=SquareMatrixMultiply(splitA.m12,splitB.m21,halfsize);
    D3=SquareMatrixMultiply(splitA.m11,splitB.m12,halfsize);
    D4=SquareMatrixMultiply(splitA.m12,splitB.m22,halfsize);
    D5=SquareMatrixMultiply(splitA.m21,splitB.m11,halfsize);
    D6=SquareMatrixMultiply(splitA.m22,splitB.m21,halfsize);
    D7=SquareMatrixMultiply(splitA.m21,splitB.m12,halfsize);
    D8=SquareMatrixMultiply(splitA.m22,splitB.m22,halfsize);

    C1=AddMatrix(D1,D2,halfsize);
    C2=AddMatrix(D3,D4,halfsize);
    C3=AddMatrix(D5,D6,halfsize);
    C4=AddMatrix(D7,D8,halfsize);
    C=combieMatrix(C1,C2,C3,C4,halfsize);

    freeMatrix(C1,halfsize);
    freeMatrix(C2,halfsize);
    freeMatrix(C3,halfsize);
    freeMatrix(C4,halfsize);
    freeMatrix(D1,halfsize);
    freeMatrix(D2,halfsize);
    freeMatrix(D3,halfsize);
    freeMatrix(D4,halfsize);
    freeMatrix(D5,halfsize);
    freeMatrix(D6,halfsize);
    freeMatrix(D7,halfsize);
    freeMatrix(D8,halfsize);
    }
    return C;
}

int main()
{
    int n=3;
    Matrix A=createMatrix(n);
    Matrix B=createMatrix(n);
    int count=1;
    for(int i=0;i<n;i++)
    {
        for(int j=0;j<n;j++)
        {
            A[i][j]=count;
            B[i][j]=count++;
        }
    }
    printMatrix(A,n);

    Matrix padA=padMatrix(A,n);
    printMatrix(padA,nextpoweroftwo(n));
    Matrix padB=padMatrix(B,n);
    Matrix C=SquareMatrixMultiply(padA,padB,nextpoweroftwo(n));
    printMatrix(C,n);
    
    freeMatrix(C,n);
    freeMatrix(padA,nextpoweroftwo(n));
    freeMatrix(padB,nextpoweroftwo(n));
    freeMatrix(A,n);
    freeMatrix(B,n);
    return 0;
}