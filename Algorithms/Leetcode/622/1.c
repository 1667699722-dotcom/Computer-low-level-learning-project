#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


typedef struct {
    int* data;
    int head;
    int tail;
    int capacity;
} MyCircularQueue;


MyCircularQueue* myCircularQueueCreate(int k) {
    MyCircularQueue* q=malloc(sizeof(MyCircularQueue));
    q->data=(int*)malloc((k+1)*sizeof(int));
    q->head=0;
    q->tail=0;
    q->capacity=k+1;
    return q;
}

bool myCircularQueueIsEmpty(MyCircularQueue* obj) {
    if(obj->head==obj->tail)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool myCircularQueueIsFull(MyCircularQueue* obj) {
    if((obj->tail+1)%obj->capacity==obj->head)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void myCircularQueueFree(MyCircularQueue* obj) {
    free(obj->data);
    free(obj);
}

bool myCircularQueueEnQueue(MyCircularQueue* obj, int value) {
    if(myCircularQueueIsFull(obj))
    {
        return false;
    }
    obj->data[obj->tail]=value;
    if(obj->tail==obj->capacity-1)
    {
        obj->tail=0;
    }
    else
    {
        obj->tail++;
    }
    return true;
}

bool myCircularQueueDeQueue(MyCircularQueue* obj) {
    if(myCircularQueueIsEmpty(obj))
    {
        return false;
    }
    int val=obj->data[obj->head];
    if(obj->head==obj->capacity-1)
    {
        obj->head=0;
    }
    else
    {
        obj->head++;
    }
    return true;
}

int myCircularQueueFront(MyCircularQueue* obj) {
    
    if(myCircularQueueIsEmpty(obj))
    {return -1;}
    return obj->data[obj->head];

    
}

int myCircularQueueRear(MyCircularQueue* obj) {
    if(myCircularQueueIsEmpty(obj))
    {
        return -1;
    }
    if(obj->tail==0)
    {
        return obj->data[obj->capacity-1];
    }
    else {
        return obj->data[obj->tail-1];
    }
}

int main()
{
    MyCircularQueue* q=myCircularQueueCreate(3);
    myCircularQueueEnQueue(q,1);
    myCircularQueueEnQueue(q,2);
    myCircularQueueEnQueue(q,3);
    printf("%d\n",myCircularQueueFront(q));
    printf("%d\n",myCircularQueueRear(q));
    myCircularQueueDeQueue(q);
    printf("%d\n",myCircularQueueFront(q));
    printf("%d\n",myCircularQueueRear(q));
    myCircularQueueEnQueue(q,4);
    printf("%d\n",myCircularQueueFront(q));
    printf("%d\n",myCircularQueueRear(q));
    return 0;
}