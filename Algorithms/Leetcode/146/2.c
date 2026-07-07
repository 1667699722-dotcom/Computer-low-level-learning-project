#include <stdio.h>
#include <stdlib.h>

static int CAPACITY;

typedef struct DLinkedNode
{
    int key;
    int val;
    struct DLinkedNode* prev;
    struct DLinkedNode* next;
    struct DLinkedNode* hashNext;
}DLinkedNode;

typedef struct
{
    DLinkedNode* dummyHead;
    DLinkedNode* dummyTail;
    DLinkedNode** hashMap;
    int size;
    int capacity;
}LRUCache ;

DLinkedNode* createNode(int key,int value)
{
    DLinkedNode* node=(DLinkedNode*)malloc(sizeof(DLinkedNode));
    node->key=key;
    node->val=value;
    node->prev=NULL;
    node->next=NULL;
    node->hashNext=NULL;
    return node;
}

LRUCache* LRUCacheCreate(int capacity)
{
    CAPACITY=capacity*3;
    LRUCache* cache=(LRUCache*)malloc(sizeof(LRUCache));
    cache->dummyHead=createNode(0,0);
    cache->dummyTail=createNode(0,0);
    cache->dummyHead->next=cache->dummyTail;
    cache->dummyTail->prev=cache->dummyHead;
    cache->size=0;
    cache->capacity=capacity;
    cache->hashMap=(DLinkedNode**)malloc(sizeof(DLinkedNode*)*CAPACITY);
    for(int i=0;i<CAPACITY;i++)
    {
        (cache->hashMap)[i]=NULL;
    }
    return cache;
}

void removeNode(DLinkedNode* node)
{
    node->prev->next = node->next;
    node->next->prev = node->prev;
}

void addToTail(DLinkedNode* dummyTail, DLinkedNode* node)
{
    node->prev = dummyTail->prev;
    node->next = dummyTail;
    dummyTail->prev->next = node;
    dummyTail->prev = node;
}

void moveToTail(LRUCache* cache, DLinkedNode* node)
{
    removeNode(node);
    addToTail(cache->dummyTail, node);
}

int LRUCacheget(LRUCache* cache,int key)
{
    int index=key%CAPACITY;
    if(index<0) index+=CAPACITY;
    DLinkedNode* node=cache->hashMap[index];
    while(node!=NULL)
    {
        if(node->key==key)
        {
            moveToTail(cache, node);
            return node->val;
        }
        node=node->hashNext;
    }
    return -1;
}

void lRUCacheput(LRUCache* cache,int key,int val)
{
    int index = key % CAPACITY;
    if (index < 0) index += CAPACITY;
    DLinkedNode* node = cache->hashMap[index];
    while(node!=NULL)
    {
        if(node->key==key)
        {
            node->val = val;
            moveToTail(cache, node);
            return;
        }
        node=node->hashNext;
    }

    DLinkedNode* newNode = createNode(key, val);
    newNode->hashNext = cache->hashMap[index];
    cache->hashMap[index] = newNode;
    addToTail(cache->dummyTail, newNode);
    cache->size++;

     if (cache->size > cache->capacity)
    {
        DLinkedNode* removed = cache->dummyHead->next;
        removeNode(removed);
        int removedIndex = removed->key % CAPACITY;
        if (removedIndex < 0) removedIndex += CAPACITY;
        DLinkedNode** prev = &(cache->hashMap[removedIndex]);
        while(*prev != NULL)
        {
            if(*prev == removed)
            {
                *prev = removed->hashNext;
                break;
            }
            prev = &((*prev)->hashNext);
        }
        free(removed);
        cache->size--;
    }

}

void LRUCachefree(LRUCache* cache)
{
    DLinkedNode* p = cache->dummyHead;
    while (p != NULL)
    {
        DLinkedNode* temp = p;
        p = p->next;
        free(temp);
    }
    free(cache->hashMap);
    free(cache);
}

int main()
{
    LRUCache* cache = LRUCacheCreate(2);
    lRUCacheput(cache, 1, 1);
    lRUCacheput(cache, 2, 2);
    printf("get 1: %d\n", LRUCacheget(cache, 1));
    lRUCacheput(cache, 3, 3);
    printf("get 2: %d\n", LRUCacheget(cache, 2));
    lRUCacheput(cache, 4, 4);
    printf("get 1: %d\n", LRUCacheget(cache, 1));
    printf("get 3: %d\n", LRUCacheget(cache, 3));
    printf("get 4: %d\n", LRUCacheget(cache, 4));
    LRUCachefree(cache);
    return 0;
}
