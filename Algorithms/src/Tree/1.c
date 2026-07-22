#include <stdio.h>
#include <stdlib.h>

typedef struct BiTNode
{
    int *data;
    struct BiTNode *lchild;
    struct BiTNode *rchild;
}BiTNode,*BiTree;

BiTNode* createNode(int val)
{
    BiTNode* node=(BiTNode*)malloc(sizeof(BiTNode));
    node->data=(int*)malloc(sizeof(int));
    node->data[0]=val;
    node->lchild=NULL;
    node->rchild=NULL;
    return node;
}

BiTree buildTestTree()
{
    BiTNode* root = createNode(1);
    root->lchild = createNode(2);
    root->rchild = createNode(3);
    root->lchild->lchild = createNode(4);
    root->lchild->rchild = createNode(5);
    root->lchild->lchild->lchild= createNode(6);
    root->lchild->lchild->rchild= createNode(7);
    root->lchild->rchild->lchild = createNode(8);
    root->lchild->rchild->rchild = createNode(9);
    return root;
}


void preOrder(BiTree root)
{
    if (root == NULL) return;
    printf("%d ", *(root->data));   // 访问根节点
    preOrder(root->lchild);          // 递归左子树
    preOrder(root->rchild);          // 递归右子树
}

void inOrder(BiTree root)
{
    if (root == NULL) return;
    inOrder(root->lchild);           // 递归左子树
    printf("%d ", *(root->data));   // 访问根节点
    inOrder(root->rchild);           // 递归右子树
}

void postOrder(BiTree root)
{
    if (root == NULL) return;
    postOrder(root->lchild);         // 递归左子树
    postOrder(root->rchild);         // 递归右子树
    printf("%d ", *(root->data));   // 访问根节点
}

void destroyTree(BiTree root)
{
    if (root == NULL) return;
    destroyTree(root->lchild);
    destroyTree(root->rchild);
    free(root->data);
    free(root);
}
int main()
{
    BiTree root = buildTestTree();
    printf("二叉树构建完成！根节点值：%d\n", *(root->data));
    printf("根节点左孩子：%d\n", *(root->lchild->data));
    printf("根节点右孩子：%d\n", *(root->rchild->data));
    printf("根节点左孩子的左孩子：%d\n", *(root->lchild->lchild->data));
    printf("根节点左孩子的左、右孩子：%d\n", *(root->lchild->rchild->data));
    preOrder(root);
    printf("\n");
    inOrder(root);
    printf("\n");
    postOrder(root);
    printf("\n");
    destroyTree(root);
    return 0;
}













