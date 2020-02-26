#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>

typedef struct bitnode{
	//二叉链表存储二叉树  
	char data;
	struct bitnode *lchild,*rchild;
}bitnode,*bitree;

int create_bitree_pre(bitree *T){
	//先序输入二叉树中结点的值，空格字符表示空树
	//创建二叉链表存储的表达式树T
	char c;
	c=getchar();
	if(!((*T)=(bitnode*)malloc(sizeof(bitnode))))	exit(0);
	(*T)->data=c;
	if(isalnum(c)){
		(*T)->lchild=NULL;
		(*T)->rchild=NULL;
	}
	else{
		create_bitree_pre(&((*T)->lchild));
		create_bitree_pre(&((*T)->rchild));
	}
	return 1;
}//createbitree

void print_bitree_pre(bitree T){
	//先序遍历二叉树，即为前缀表达式 
	if(T!=NULL){
		putchar(T->data);
		print_bitree_pre(T->lchild);
		print_bitree_pre(T->rchild);
	}
}//printbitree

void print_bitree_in(bitree T){
	//中序遍历二叉树，与中缀不同，因为未考虑优先级加括号 
	if(T!=NULL){
		print_bitree_in(T->lchild);
		putchar(T->data);
		print_bitree_in(T->rchild);
	} 
}

void print_bitree_post(bitree T){
	//后序遍历二叉树，即为后缀表达式 
	if(T!=NULL){
		print_bitree_post(T->lchild);
		print_bitree_post(T->rchild);
		putchar(T->data);
	} 
}

void print_infix(bitree T){
	//输出表达式树的中缀表达式
	if(T!=NULL){
		if(T->data=='*' || T->data=='/'){
			if(!isalnum((T->lchild)->data)){			
				//乘除号的左操作数是一个表达式 
				putchar('(');
				print_infix(T->lchild);
				putchar(')');
			}
			else	print_infix(T->lchild);
			putchar(T->data);
			if(!isalnum((T->rchild)->data)){			
				//乘除号的右操作数是一个表达式 
				putchar('(');
				print_infix(T->rchild);
				putchar(')');
			}
			else	print_infix(T->rchild);
		}
		else{
			print_infix(T->lchild);
			putchar(T->data);
			print_infix(T->rchild);
		}
	} 
}

float print_cal(bitree T){
	//基于表达式树求值(操作数为1位十进制数)
	float a,b,sum;
	char c=T->data;
	if(isalnum(c)){
		//是操作数 
		sum=atoi(&c);
	}
	else{
		//是操作符
		a=print_cal(T->lchild);
		b=print_cal(T->rchild);
		switch(c){
			case'+': sum=a+b; break;
			case'-': sum=a-b; break;
			case'*': sum=a*b; break;
			case'/': sum=a/b; break;
		} 
	} 
	return sum;
}

int main(){
	//先序创建表达式树T; 先序、中序、后续遍历该树 
	bitree T;
	printf("enter your bitree:\n");
	create_bitree_pre(&T);
	print_bitree_pre(T);
	putchar('\n'); 
	print_bitree_in(T);
	putchar('\n');
	print_bitree_post(T);
	putchar('\n');
	print_infix(T);
	putchar('=');
	printf("%f",print_cal(T));
	return 0;
}




//11.6 22:00 完成6(A)、7题 
//11.6 23:37 完成8(B)  
//11.13 16:35 完成8(A) 
