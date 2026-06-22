#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>

typedef struct
{
    char* data;
    int top;
    int capacity;
}stack;

bool isfull(stack* s)
{
    if(s->top==s->capacity-1)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void initstack(stack* s,int capacity)
{
    s->data=(char*)malloc(capacity*sizeof(char));
    s->top=-1;
    s->capacity=capacity;
}
void push(stack* s,char val)
{
    if(isfull(s))
    {
        printf("overflow\n");
        return;  // 溢出时直接返回，不继续操作
    }
    s->top++;
    s->data[s->top]=val;
}
char pop(stack* s)
{
    if(s->top==-1)
    {
        printf("underflow\n");
        return '\0';  // 下溢时返回空字符
    }
    char val=s->data[s->top];
    s->top--;
    return val;
}

void destory(stack* s)
{
    free(s->data);
}


bool isempty(stack* s)
{
    if (s->top==-1)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool isValid(char* s) 
{
    int len=strlen(s);
    stack st;
    initstack(&st,len);
    for(int i=0;i<len;i++)
    {
        char c=s[i];
        if(c=='(' ||c=='{'||c=='[')
        {
            push(&st,c);
        }
        else
        {
            if(isempty(&st))
            {
                destory(&st);
                return false;
            }
            else
            {
                char top=pop(&st);
                if((c==')' && top!='(')||
                (c==']' && top!='[')||
                (c=='}' && top!='{'))
                {
                    destory(&st);
                    return false;
                }
            }
        }
    }
    bool ans=isempty(&st);
    destory(&st);
    return ans;
}

int main() {
    char* test1 = "()[]{}";
    char* test2 = "(]";
    char* test3 = "([{}])";
    char* test4 = "([)]";
    
    printf("\"%s\" -> %s\n", test1, isValid(test1) ? "true" : "false");
    printf("\"%s\" -> %s\n", test2, isValid(test2) ? "true" : "false");
    printf("\"%s\" -> %s\n", test3, isValid(test3) ? "true" : "false");
    printf("\"%s\" -> %s\n", test4, isValid(test4) ? "true" : "false");
    
    return 0;
}