#include<stdio.h>
#include<stdlib.h>
typedef struct lnode{
	int order;
	int code;
	struct lnode *next;
}lnode,*linklist;
int main(int argc,char *argv[]){
	//用不带头节点的单循环链表存储约瑟夫环，并依据输入打印出列顺序
	int m,n,a[100],b=0,i;
	linklist L,p,q;
	FILE *fp;
	if((fp=fopen("Data1.txt","w"))==NULL){
		printf("ERROR file");
		exit(0);
	}
	if(argc==1){
		printf("输入形成约瑟夫环的人数n:");					//判断输入的数据数是否正确，若不正确，提醒补充输入 
		scanf("%d",&n);
		printf("输入初始的报数上限m:");
		scanf("%d",&m);
	} 
	else if(argc==2){
		printf("输入初始的报数上限m:");
		scanf("%d",&m);
	}
	else{
		n=atoi(argv[1]);
		m=atoi(argv[2]);
	}
	if(m<1){
		printf("ERROR m");		//判断输入的人数、报数上限是否正确 
		exit(0);
	}
	if(n<1){
		printf("ERROR n");		 
		exit(0);
	}
	for(i=1;i+3<=argc;i++) a[i-1]=atoi(argv[i+2]);
	for(;i<=n;i++){
		printf("输入第%d个密码:",i);						//弥补输入密码数不足 
		scanf("%d",&a[i-1]);
	}
	L=(linklist)malloc(sizeof(lnode));						//构建单循环链表 
	p=L;
	for(i=1;i<=n;i++){
		p->next=(linklist)malloc(sizeof(lnode));
		p=p->next;
		if(a[i-1]<1){
			printf("ERROR %d",i);
			exit(0);
		}
		p->code=a[i-1];
		p->order=i;
	} 
	q=L;
	L=L->next;
	p->next=L;
	free(q);
	p=L; 
	while(n-b>1){
		for(i=1;i<m;i++){
			q=p;
			p=p->next;
		}
		q->next=p->next;
		q=p;
		p=p->next;
		m=q->code;
		a[b]=q->order;
		free(q);
		b++;
	} 
	a[b]=p->order;
	free(p);
	printf("出列顺序为:");
	for(i=0;i<=b;i++){
		printf("%d",a[i]);
		fprintf(fp,"%d",a[i]);
	}
	fclose(fp);
	return 0;
}
