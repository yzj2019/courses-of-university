#include<stdio.h>
#include<stdlib.h>
#define listsize 100
typedef struct{
	int a[listsize];
	int b[listsize];
	int length;
}sqlist; 
int main(int argc,char *argv[]){
	//用线性表存储约瑟夫环，并依据输入打印出列顺序
	int m,n,i,j,c[listsize];
	sqlist L;
	FILE *fp;
	if((fp=fopen("Data.dat","w"))==NULL){
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
	L.length=n;
	for(i=1;i+3<=argc;i++) L.a[i-1]=atoi(argv[i+2]);
	for(;i<=n;i++){
		printf("输入第%d个密码:",i);						//弥补输入密码数不足 
		scanf("%d",&L.a[i-1]);
	}
	for(i=1;i<=n;i++){
		L.b[i-1]=i;
		if(L.a[i-1]<0){
			printf("ERROR %d",i);
			exit(0);
		}	
	}

	j=0;
	while(L.length>1){
		for(i=1;i<m;i++,j++)
			if(j==L.length) j=0;
		if(j==L.length) j=0;
		m=L.a[j];
		c[n-L.length]=L.b[j];
		for(i=j;i<L.length-1;i++){
			L.a[i]=L.a[i+1];
			L.b[i]=L.b[i+1];
		}
		L.length--;
	} 
	c[n-1]=L.b[0];
	printf("出列顺序为:");
	for(i=0;i<n;i++){
		printf("%d ",c[i]);
		fprintf(fp,"%d",c[i]);
	}
	fclose(fp);
	return 0;
}
