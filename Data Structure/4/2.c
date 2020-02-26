#include<stdio.h>
#include<stdlib.h>
#define STACKINITSIZE 100
#define STACKINCREMENT 10
#define MAXQSIZE 100
#define infinity INT_MAX
#define max_vertex_num 20
typedef enum{
    DG,         //有向图
    DN,         //有向网
    UDG,        //无向图
    UDN         //无向网
} graphkind;
int visited[max_vertex_num];


/*************栈的相关操作*************/
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
int stackempty(sqstack *S){
    //判断是否为空栈
    if((*S).top==(*S).base) 
        return 1;
    else
        return 0;
}


/************队列的相关操作***********/
typedef struct{
	int *base;
	int front;
	int rear;
}sqqueue;
int initqueue(sqqueue *Q){						
    //创建空队列 
	(*Q).base=(int*)malloc(MAXQSIZE*sizeof(char));
	if(!(*Q).base) exit(0);
	(*Q).front=(*Q).rear=0;
	return 1;
}
void enqueue(sqqueue *Q, int e){
    //入队列
    //保证不越界，每轮使用都初始化首尾
    (*Q).base[(*Q).rear++] = e;
}
void dequeue(sqqueue *Q, int *e){
    //出队列
    (*e) = (*Q).base[(*Q).front++];
}


/**********邻接表表示法存储无向网**********/
/*********此时需要配合visited数组*********/
typedef struct arcnode{
    //边
    int             adjvex;                         //邻顶
    int             adj;                            //权值
    struct arcnode  *nextarc;                       //下一条边
} arcnode;
typedef struct vnode{
    //顶
    char        data;                               //编号
    arcnode     *firstarc;                          //指向第一条依附该顶点的边的指针
} vnode, adjlist[max_vertex_num];
typedef struct{
    adjlist     vertics;
    int vexnum, arcnum;                             //图的当前顶点数和弧数
    graphkind   kind;                               //图的种类
} ALGraph;


/************创建算法************/
void creatarc(ALGraph *G, int j, int k, int n){
    //新建边
    arcnode *p;
    p = (*G).vertics[j].firstarc;
    if(p==NULL){
        (*G).vertics[j].firstarc = (arcnode *)malloc(sizeof(arcnode));
        p = (*G).vertics[j].firstarc;
        p->adjvex = k;
        p->adj = n;
        p->nextarc = NULL;
    }
    else{
        while(p->nextarc!=NULL)
            p = p->nextarc;
        p->nextarc = (arcnode *)malloc(sizeof(arcnode));
        p = p->nextarc;
        p->adjvex = k;
        p->adj = n;
        p->nextarc = NULL;
    }
}

void CreatALGraph(ALGraph *G){
    //邻接表
    int i, j, k, n;
    arcnode *p;
    (*G).kind = UDN;
    printf("enter vexnum:");                        //顶点数
    scanf("%d", &((*G).vexnum));
    printf("enter arcnum:");                        //边数
    scanf("%d", &((*G).arcnum));
    printf("enter vexs:\n");                        //顶点信息
    fflush(stdin);
    for (i = 0; i < (*G).vexnum; i++){
        (*G).vertics[i].data=getchar();
        getchar();                                  //空格分隔
    }
    for (i = 0; i < (*G).vexnum; i++)
        (*G).vertics[i].firstarc = NULL;
    for (i = 0; i < (*G).arcnum; i++){
        printf("enter arc %d:", i);                //依次输入两端点的存储位置以及边权
        scanf("%d%d%d", &j, &k, &n);
        p = (*G).vertics[j].firstarc;
        creatarc(G, j, k, n);
        creatarc(G, k, j, n);
    }
}

int count;          //对访问计数
int low[max_vertex_num];
void DFSArticul(ALGraph G, int v0){
    //从v0点出发，深度优先遍历图G，查找并输出关节点
    //此时的子树以v0为根
    int min,w;
    arcnode *p;
    visited[v0] = min = count++;
    for (p = G.vertics[v0].firstarc; p != NULL; p=p->nextarc){
        //对v0每个邻接顶检查
        w = p->adjvex;
        if(visited[w]==0){
            DFSArticul(G,w);        //调用结束前已求得low[w]
            if(low[w]<min)
                min = low[w];
            if(low[w]>=visited[v0])
                printf("%c is Articul\n", G.vertics[v0].data);
        }
        else if(visited[w]<min)
            min = visited[w];
    }
    low[v0] = min;
}

void FindArticul(ALGraph G){
    //以邻接表存储的连通图G，查找并输出G上全部关节点
    int i,v;    
    arcnode *p;
    count = 1;
    visited[0] = 1;             //设定邻接表上0号元素为根
    for (i = 1; i < G.vexnum; i++)
        visited[i] = 0;
    p = G.vertics[0].firstarc;
    v = p->adjvex;
    DFSArticul(G, v);
    if(count<G.vexnum){
        printf("%c is Articul\n", G.vertics[0].data);
        while(p->nextarc!=NULL){
            p = p->nextarc;
            v = p->adjvex;
            if(visited[v]==0)
                DFSArticul(G, v);
        }
    }
}


int main(){
    int i,k,n;
    arcnode *p;
    ALGraph G;
    CreatALGraph(&G);
    for(i=0; i < G.vexnum; i++){
        for (p = G.vertics[i].firstarc; p != NULL;p=p->nextarc){
            k=p->adjvex;
            n = p->adj;
            printf("%c,%c,%d\n", G.vertics[i].data, G.vertics[k].data, n);
        }
    }                                                                                   //验证
    FindArticul(G);
}



//15:25     完成求无向连通图中关键节点