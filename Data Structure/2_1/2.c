#include<stdio.h>
#include<stdlib.h>
#define STACKINITSIZE 100
#define STACKINCREMENT 10
//数据存储规则：背包总体积 可选物品数 各物品大小  
typedef struct{
	int *base;
	int *top;
	int stacksize;
}sqstack;
int initstack(sqstack *S){
	//创建空栈 
	(*S).base=(int*)malloc(STACKINCREMENT*sizeof(int));
	if(!(*S).base) exit(0);
	(*S).top=(*S).base;
	(*S).stacksize=STACKINCREMENT;
	return 1;
}//initstack
int push(sqstack *S, int e){
	//将e入栈  
	if((*S).top-(*S).base>=(*S).stacksize){
		(*S).base=(int*)realloc((*S).base,((*S).stacksize+STACKINCREMENT)*sizeof(int));
		if(!(*S).base) exit(0);
		(*S).top=(*S).base+(*S).stacksize;
		(*S).stacksize+=STACKINCREMENT;
	}
	*(*S).top++=e;
	return 1;
}//push
int pop(sqstack *S, int *e){
	//出栈 
	if((*S).top==(*S).base) return 0;
	*e=*(*S).top--;
	return 1;
}//pop
int gettop(sqstack *S, int *e){
	//取栈顶元素
	if((*S).top==(*S).base) return 0;
	*e=*((*S).top-1);
	return 1; 
}//gettop
int full(sqstack *S, int a[], int t){
	//检查是否装满背包，如果超出返回1，恰好满返回0，未满返回-1
	int i,sum=0;
	for(i=0;i<(*S).top-(*S).base;i++) sum+=a[*((*S).base+i)];
	if(sum>t) return 1;
	if(sum<t) return -1;
	if(sum==t) return 0;
}//full
int main(int argc,char *argv[]){
	int a[100],t,n,i,p,q;
	char *c,*d;
	FILE *in,*out;
	c=argv[1];	d=argv[2];						//命令行参数按照【程序名 读文件 写文件】的规则读入 
	if((in=fopen(c,"r"))==NULL){				//打开读取参数的文件 
		printf("ERROR file");
		exit(0);
	}
	if((out=fopen(d,"w"))==NULL){
		printf("ERROR file");
		exit(0);
	}
	sqstack S;
	initstack(&S);
	fscanf(in,"%d",&t);							//输入背包的总体积 
	fscanf(in,"%d",&n);							//输入物品数 
	for(i=0;i<n;i++) fscanf(in,"%d",&a[i]); 	//输入各物品大小 
	push(&S,0);									//装入第一个 
	printf("装满该背包的方法有：\n");
	while(*S.base!=n-1){						//循环结束条件：栈底元素代表末尾的物品 
		gettop(&S,&q);
		if(q==n){								//最后选取的物品已经过了末尾 
			pop(&S,&q);
			(*(S.top-1))++;
		} 
		else if(q<n){							//最后选取的物品未过末尾 
			p=full(&S,a,t);
			if(p==0){							//恰能装满，则输出并更新循环条件 
				for(i=0;i<S.top-S.base;i++){
					printf("%d ",a[*(S.base+i)]);
					fprintf(out,"%d ",a[*(S.base+i)]);
				}	
				putchar('\n');
				fputc('\n',out);
				(*(S.top-1))++;
			}
			if(p==-1) 		push(&S,q+1);		//未装满，则选取一件新的 
			else if(p==1) 	(*(S.top-1))++;		//装过满，则栈顶元素代表的物品更换为它的下一件 
		}
	}
	if(a[n-1]==t){								//末尾恰能装满 
		fprintf(out,"%d\n",a[n-1]);
	}
}	
//栈中存储数组元素的下标 
//1、大致架构写好 
//2、实现题目要求
//3、增加命令行参数实现文件读写 
