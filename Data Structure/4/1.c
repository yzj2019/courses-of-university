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


/**********数组表示法存储无向网**********/
typedef struct arccell{
    //边
    //由于不需要每边的相关信息，此处未定义
    int         adj;                                //权值，infinity代表不相邻
} arccell, adjmatrix[max_vertex_num][max_vertex_num];
typedef struct {
    char            vexs[max_vertex_num];
    adjmatrix       arcs;
    int             vexnum, arcnum;                 //图的当前顶点数和弧数
    graphkind       kind;                           //图的种类
} MGraph;


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
void CreatMGraph(MGraph *G){
    //通过直接读入点集与邻接矩阵来构建无向网
    int i, j, k, n;
    (*G).kind = UDN;
    printf("enter vexnum:");                        //顶点数
    scanf("%d", &((*G).vexnum));
    printf("enter arcnum:");                        //边数
    scanf("%d", &((*G).arcnum));
    printf("enter vexs:\n");                        //顶点信息:‘顶点1’‘ ’‘顶点二’‘ ’‘……’
    fflush(stdin);
    for (i = 0; i < (*G).vexnum; i++){
        (*G).vexs[i] = getchar();
        getchar();
    }
    for (i = 0; i < (*G).vexnum; i++)
        for (j = 0; j < (*G).vexnum; j++)
            (*G).arcs[i][j].adj = infinity;
    for(i = 0; i < (*G).arcnum; i++){
        printf("enter arc %d:",i);                 //边信息逐边输入:端点、端点、权值
        scanf("%d%d%d", &j, &k, &n);
        (*G).arcs[j][k].adj = n;
        (*G).arcs[k][j].adj = n;
    }
}


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
        /*if(p==NULL){
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
        p = (*G).vertics[k].firstarc;
        if(p==NULL){
            (*G).vertics[k].firstarc = (arcnode *)malloc(sizeof(arcnode));
            p = (*G).vertics[k].firstarc;
            p->adjvex = j;
            p->adj = n;
            p->nextarc = NULL;
        }
        else{
            while(p->nextarc!=NULL)
                p = p->nextarc;
            p->nextarc = (arcnode *)malloc(sizeof(arcnode));
            p = p->nextarc;
            p->adjvex = j;
            p->adj = n;
            p->nextarc = NULL;
        }*/
    }
}


/************邻接矩阵的深度优先遍历************/
void DFS_MGraph(MGraph G, int v0){
    //从v0开始进行深度优先搜索
    int i,v;
    sqstack S;
    initstack(&S);
    push(&S, v0);
    putchar(G.vexs[v0]);
    visited[v0] = 1;
    while(!stackempty(&S)){
        gettop(&S, &v);
        for(i = 0; i < G.vexnum; i++)
            if((G.arcs[v][i].adj<infinity)&&(!visited[i]))
                break;
        if(i < G.vexnum){
            push(&S, i);
            putchar(G.vexs[i]);
            visited[i] = 1;
        }
        else
            pop(&S, &v);
    }
    putchar('\n');
}


/****************邻接矩阵的广度优先遍历**************/
void BFS_MGraph(MGraph G){
    //从v0开始广度优先搜索
    int i, j, v;
    sqqueue Q;
    initqueue(&Q);
    for(i = 0; i < G.vexnum; i++)
        if(!visited[i]){
            enqueue(&Q, i);
            putchar(G.vexs[i]);
            visited[i] = 1;
            while(Q.front!=Q.rear){
                dequeue(&Q,&v);
                for(j = 0; j < G.vexnum; j++)
                    if((G.arcs[v][j].adj<infinity)&&(!visited[j])){
                        enqueue(&Q, j);
                        putchar(G.vexs[j]);
                        visited[j] = 1;
                    }//if
            }//while
        }//if
    putchar('\n');
}//BFS_MGraph



int main(){
    int i,j,k,n;
    arcnode *p;
    MGraph G1;
    ALGraph G2;
    CreatMGraph(&G1);
    for(i=0; i < G1.vexnum; i++)
        for(j = 0; j < G1.vexnum; j++)
            if(G1.arcs[i][j].adj<infinity)
                printf("%c,%c,%d\n", G1.vexs[i], G1.vexs[j], G1.arcs[i][j].adj);        //验证
    /*CreatALGraph(&G2);
    for(i=0; i < G2.vexnum; i++){
        for (p = G2.vertics[i].firstarc; p != NULL;p=p->nextarc){
            k=p->adjvex;
            n = p->adj;
            printf("%c,%c,%d\n", G2.vertics[i].data, G2.vertics[k].data, n);
        }
    }*/                                                                                 //验证
    for (i = 0; i < G1.vexnum; i++)
        visited[i] = 0;
    for (i = 0; i < G1.vexnum; i++)
        if(!visited[i])
            DFS_MGraph(G1, i);
    for (i = 0; i < G1.vexnum; i++)
        visited[i] = 0;
    BFS_MGraph(G1);
}






//14:14     完成邻接矩阵的创建
//14:34     完成邻接表的创建
//14:45     完成深度优先遍历
//14:50     完成广度优先遍历