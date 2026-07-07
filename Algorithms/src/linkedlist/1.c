#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct ListNode
{
    int val;
    struct ListNode* prev;
    struct ListNode* next;
}ListNode;

ListNode* createNode(int val)
{
    ListNode* node=(ListNode*)malloc(sizeof(ListNode));
    node->val=val;
    node->prev=NULL;
    node->next=NULL;
    return node;
}

void printList(ListNode* head)
{
    printf("list forward:");
    ListNode* p=head;
    while(p!=NULL)
    {
        printf("%d",p->val);
        p=p->next;
    }
    printf("\n");
}

ListNode* insertAtHead(ListNode* head,int val)
{
    ListNode* newNode=createNode(val);
    if(head==NULL)
    {
        return newNode;
    }
    newNode->next=head;
    head->prev=newNode;
    return newNode;
}

ListNode* insertAtTail(ListNode* head,int val)
{
    ListNode* newNode=createNode(val);
    if(head==NULL)
    {
        return newNode;
    }
    ListNode* p=head;
    while(p->next!=NULL)
    {
        p=p->next;
    }
    p->next=newNode;
    newNode->prev=p;
    return head;
}

ListNode* deleteNode(ListNode* head,int val)
{
    if(head==NULL)
    {
        return NULL;
    }
    ListNode* p=head;
    while(p!=NULL && p->val!=val)
    {
        p=p->next;
    }
    if(p==NULL)
    {
        return head;
    }
    if(p->prev!=NULL)
    {
        p->prev->next=p->next;
    }
    else {
        head=p->next;
    }
    if(p->next!=NULL)
    {
        p->next->prev=p->prev;
    }
    free(p);
    return head;
}

ListNode* findNode(ListNode* head,int val)
{
    ListNode* p=head;
    while(p!=NULL)
    {
        if(p->val==val)
        {
            return p;
        }
        p=p->next;
    }
    return NULL;
}

void destroyList(ListNode* head)
{
    ListNode* p=head;
    while(p!=NULL)
    {
        ListNode* temp=p;
        p=p->next;
        free(temp);
    }
}

int main()
{
    ListNode* head=NULL;
    head=insertAtTail(head,1);
    head=insertAtTail(head,2);
    head=insertAtTail(head,3);
    printList(head);
    head=insertAtHead(head,0);
    printList(head);
    head=deleteNode(head,2);
    printList(head);
    ListNode* found=findNode(head,3);
    if(found!=NULL)
    {
        printf("found %d\n",found->val);
    }
    else
    {
        printf("not found\n");
    }
    destroyList(head);
    return 0;
}


