#include<stdio.h>
#include<stdlib.h>

typedef struct bitnode{
	//二叉链表存储二叉树 
	char data;
	struct bitnode *lchild,*rchild;
}bitnode,*bitree;

int create_bitree_2(bitree *T){
	//先序("_2")输入二叉树中结点的值，空格字符表示空树
	//创建二叉链表存储的二叉树T
	char c;
	c=getchar();
	if(c==' ') (*T)=NULL;
	else{
		if(!((*T)=(bitnode*)malloc(sizeof(bitnode))))	exit(0);
		(*T)->data=c;
		create_bitree_2(&((*T)->lchild));
		create_bitree_2(&((*T)->rchild));
	}
	return 1;
}//createbitree

void print_bitree_2(bitree T){
	//题目2 先序遍历二叉树 
	if(T!=NULL){
		putchar(T->data);
		print_bitree_2(T->lchild);
		print_bitree_2(T->rchild);
	}
	else	putchar(' ');
}//printbitree

int main(){
	bitree T;
	printf("enter your bitree:\n");
	create_bitree_2(&T);
	print_bitree_2(T); 
	return 0;
}
