#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct
{
    char* data;
    int head;
    int tail;
    int capacity;
}queue;

void initqueue(queue* q,int capacity)
{
    q->data=(char*)malloc((capacity+1)*sizeof(char));
    q->head=0;
    q->tail=0;
    q->capacity=capacity+1;
}

void destoryqueue(queue* q)
{
    free(q->data);
}
bool isfull(queue* q)
{
    if((q->tail+1)%q->capacity==q->head)
    {
        return true;
    }
    else
    {
        return false;
    }
}
bool isempty(queue* q)
{
    if(q->head==q->tail)
    {
        return true;
    }
    else
    {
        return false;
    }
}
void enqueue(queue* q,char val)
{   
    if(isfull(q))
    {
        printf("overflow\n");
        return;  // 溢出时直接返回，不继续操作
    }
    q->data[q->tail]=val;
    if(q->tail==q->capacity-1)
    {
        q->tail=0;
    }
    else
    {
        q->tail++;
    }
}
char dequeue(queue* q)
{
    if(isempty(q))
    {
        printf("underflow\n");
        return '\0';  // 下溢时返回空字符
    }
    char val=q->data[q->head];
    if(q->head==q->capacity-1)
    {
        q->head=0;
    }
    else
    {
        q->head++;
    }
    return val;
}

int main()
{
    queue q;
    initqueue(&q,10);
    enqueue(&q,'a');
    enqueue(&q,'b');
    enqueue(&q,'c');
    enqueue(&q,'d');
    enqueue(&q,'e');
    printf("%c\n",dequeue(&q));
    printf("%c\n",dequeue(&q));
    printf("%c\n",dequeue(&q));
    printf("%c\n",dequeue(&q));
    printf("%c\n",dequeue(&q));
    return 0;
}
