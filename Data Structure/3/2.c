#include<stdio.h>
#include<stdlib.h>

typedef enum {link,thread}PointerTag;	//link==0:指针,thread==1:线索
typedef struct bithrnode{
	//线索三叉链表存储线索二叉树 
	char data;
	struct bithrnode *lchild,*rchild,*parent;
	PointerTag ltag,rtag;
}bithrnode,*bithrtree;

int preorder_create_bithrtree(bithrtree *T,bithrtree p){
	//先序输入二叉树中结点的值，空格字符表示空树，指针p指向双亲 
	//创建线索三叉链表存储的线索二叉树T
	char c;
	c=getchar();
	if(c==' ') (*T)=NULL;
	else{
		if(!((*T)=(bithrnode*)malloc(sizeof(bithrnode))))	exit(0);
		(*T)->data=c;
		(*T)->parent=p;
		(*T)->ltag=link;												//均置为链，线索化的时候再改 
		(*T)->rtag=link;
		preorder_create_bithrtree(&((*T)->lchild),*T);
		preorder_create_bithrtree(&((*T)->rchild),*T);
	}
	return 1;
}

void post_threading(bithrtree *T,bithrtree *pre){
	if(*T){
		post_threading(&((*T)->lchild),pre);
		post_threading(&((*T)->rchild),pre);
		if(!((*T)->lchild)){
			(*T)->ltag=thread;
			(*T)->lchild=(*pre);
		}
		if(!((*pre)->rchild)){
			(*pre)->rtag=thread;
			(*pre)->rchild=(*T);
		}
		(*pre)=(*T);
	}
}

int postorder_threading(bithrtree *Thrt,bithrtree *T){
	//后续遍历二叉树T，并将其后序线索化
	//Thrt指向头节点，头节点的rchild指向第一个遍历的节点 
	bithrtree pre; 												//此时该指针已经被分配了用于存储指针的空间，在后续递归栈中可以调用
	if(!((*Thrt)=(bithrtree)malloc(sizeof(bithrnode))))	exit(0);
	(*Thrt)->data='\n';											//作为判断头节点的依据  
	(*Thrt)->ltag=link;
	(*Thrt)->rtag=thread;
	(*Thrt)->rchild=NULL;										//右指针置空，为了使其指向第一个后序遍历的节点  
	if(!(*T)){
		(*Thrt)->lchild=(*Thrt);								//若二叉树为空，则左右指针回指  
		(*Thrt)->rchild=(*Thrt);
	}
	else{
		(*Thrt)->lchild=(*T);
		(*T)->parent=(*Thrt);									//作为判断根节点的依据 
		pre=(*Thrt);
		post_threading(T,&pre);									//后序遍历进行后序线索化
	} 
	return 1;
}

int posthr_visit(bithrtree Thrt){
	//通过线索，后序遍历以Thrt为头结点的后序线索二叉树
	bithrtree p,q;
	p=Thrt->rchild;
	while(p!=Thrt){
		//直到遍历完根节点回到头结点时结束 
		q=p->parent;
		printf("%c",p->data);
		if(p==q->lchild && q->ltag==link && q->rtag==link){
			//该结点是双亲的左孩子，且双亲有右子树
			while(q->rtag==link){
				q=q->rchild;
				while(q->ltag==link) q=q->lchild;
			}
		}
		p=q;
	}
	return 1;
}

int main(){
	bithrtree T,Thrt;
	printf("enter your bithrtree:\n");
	preorder_create_bithrtree(&T,NULL);
	postorder_threading(&Thrt,&T);
	posthr_visit(Thrt);
	return 0;
}




//10.30:编写完成后续线索化部分；尚未遍历验证正确性。 
//11.6 20:50 更改，完成遍历，遍历出错  
//11.6 21:15 完成遍历修改，成功输出后续遍历 
//11.6 23:05 现把遍历不小心写成了递归形式。。。修改。。。 
