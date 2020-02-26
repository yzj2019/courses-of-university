#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define STACKINITSIZE 100
#define STACKINCREMENT 10
//数据存储规则：（翻译规则个数） （魔王语言i） （翻译语言i长度） （翻译语言i） （待翻译的魔王语言长度） （待翻译魔王语言） 
typedef struct{
	char *base;
	char *top;
	int stacksize;
}sqstack;
typedef struct{
	char *base;
	int front;
	int rear;
}sqqueue;
int initstack(sqstack *S){									//创建空栈 
	(*S).base=(char*)malloc(STACKINITSIZE*sizeof(char));
	if(!(*S).base) exit(0);									//存储分配失败 
	(*S).top=(*S).base;
	(*S).stacksize=STACKINITSIZE;
	return 1;
}//initstack
int push(sqstack *S, char e){								//入栈		
	if((*S).top-(*S).base>=(*S).stacksize){					//栈满，追加存储空间  
		(*S).base=(char*)realloc((*S).base,((*S).stacksize+STACKINCREMENT)*sizeof(char));
		if(!(*S).base) exit(0);
		(*S).top=(*S).base+(*S).stacksize;
		(*S).stacksize+=STACKINCREMENT;
	}
	*(*S).top++=e;
	return 1;
}//push
void translate1(sqstack *S, sqqueue *Q, char t[]){				//根据翻译规则翻译大写字母 
	int i,j;
	j=strlen(t);
	for(i=0;t[i]!='\0';i++)
		(*Q).base[(*Q).rear++]=t[j-i-1];
	while((*Q).front!=(*Q).rear)						
		push(S,(*Q).base[(*Q).front++]);
	(*Q).front=(*S).top-(*S).base;								//队列初始化
	(*Q).rear=(*S).top-(*S).base; 
}//translate1 
void translate2(sqstack *S, sqqueue *Q){						//翻译括号中的内容  
	char c;
	c=*(--(*S).top);
	(*Q).base[(*Q).rear++]=c;
	while(*(--(*S).top)!=')'){									//入队列方便逆序处理 
		(*Q).base[(*Q).rear++]=*(*S).top;
		(*Q).base[(*Q).rear++]=c;
	}
	while((*Q).front!=(*Q).rear)								//再次入栈以逆序并处理魔王语言，后括号覆盖了  
		push(S,(*Q).base[(*Q).front++]);
	(*Q).front=(*S).top-(*S).base;								//队列初始化
	(*Q).rear=(*S).top-(*S).base; 
}//translate2
int main(int argc,char *argv[]){
	int i,m,n,l;								//m作为输出状态判断位 
	char a[100],b[10],t[10][100];
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
	sqstack S; sqqueue Q;
	initstack(&S);									
	Q.base=S.base;								//存储共享
	fscanf(in,"%d",&n);							//读入翻译规则的个数 
	fgetc(in);
	for(i=0;i<n;i++){							//规则读入
		fscanf(in,"%c",&b[i]);					//读入魔王语言
		fgetc(in);
		fscanf(in,"%d",&l);						//读入翻译后语言长度 
		fgetc(in);
		fgets(t[i],l+1,in);						//读入翻译后语言 
		fgetc(in);
	}											
	fscanf(in,"%d",&l);							//读入待翻译魔王语言长度 
	fgetc(in);
	fgets(a,l+2,in);							//读入待翻译魔王语言					
	for(i=0;a[i]!='\0';i++) printf("%c",a[i]);
	printf("->");	
	Q.front=i;	Q.rear=i;		
	for(i=i-1;i>=0;i--) push(&S,a[i]);			//逆序入栈 
	while(S.top!=S.base){
		--S.top;
		m=1;									//输出判断符号m 
		if(*S.top=='('){
			translate2(&S,&Q);
			continue;
		}									 
		for(i=0;i<n;i++)
			if(*S.top==b[i]){
				translate1(&S,&Q,t[i]);
				m=0;
				break;
			}
		if(m){
			printf("%c",*S.top);
			fprintf(out,"%c",*S.top);
		}	
	}
	return 0;
} 
/*j=strlen(t);//防止溢出
	if((&Q).rear+2*j>=stacksize){
		i=(*S).top-(*S).base;
		(*S).base=(char*)realloc((*S).base,((*S).stacksize+2*j)*sizeof(char));
		if(!(*S).base) exit(0);
		(*S).top=(*S).base+i;
		(&Q).base=(*S).base;
		(&Q).front=i;	(&Q).rear=i; 
		(*S).stacksize+=STACKINCREMENT;
	}
*/
//1、基本架构 
//2、实现题目要求
//3、实现附加题
//4、增加命令行参数实现文件读写 
